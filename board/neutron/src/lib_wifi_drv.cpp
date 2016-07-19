/**
******************************************************************************
* @file
* @authors
* @version V1.0.0
* @date    2014_11_13
* @brief
******************************************************************************
*/
/****************************************************************************
 *Private Included Files
 ****************************************************************************/
#include "main.h"
#include "cmsis_os.h"

#include "lib_system_all.h"
#include "lib_fifo.h"

#include "firmware_update.h"
#include "intorobot_api.h"


extern uint8_t  online_downfile_flag;
extern uint8_t  net_down_flag;


extern uint8_t  smartconfig_over_flag;
/************************************************************************************
 * Private Types
 ************************************************************************************/
#define SEARCH_SIZE 65                              //必须是奇数
//#define SEARCH_SIZE 129

typedef struct exceed_buf_s
{
    int available;             //可读的数据个数   0无
    char buf[SEARCH_SIZE/2];   //缓冲
}exceed_buf_t;

typedef struct tcpc_fifo_ctl_s
{
    fifo_t handle[5];             //open的句柄
    char init_flag[5];         //初始化标志  未初始化 0  已经初始化    1
}tcpc_fifo_ctl_t;


/**/
//p_cmd 必须为字符串并带结束符
/**/

typedef struct mo_cmd_ctl_s
{
    fifo_t handle;             //open的句柄
    char init_flag;            //初始化标志  未初始化 0  已经初始化    1
}mo_cmd_ctl_t;



/*
  返回需要过滤的结果
*/

//永远上报
#define HEAD_WIFI_LIST "+CWLAP:"
#define HEAD_TCPC "+IPD,"

//通过掩码控制是否上报
#define SEND_OK_TCPC "SEND OK"				///0
#define RET_REST "ready"					///1
#define SEND_ACK_TCPC ">"					///2
#define RET_STATUS_2 "STATUS:2"				///3
#define RET_OK	"OK"						///4
#define ALREADY_CONNECT "ALREAY CONNECT"	///5
#define RET_STATUS_5 "STATUS:5"				///6
#define HEAD_IPSTATUS "+CIPSTATUS:"			///7
#define RET_SMARTCONFIG "smartconfig connected"			///8
#define RET_MD5SET "MD5 SET OK"             // 9
#define RET_DOWNFW "+IR_DOWNFILE:0"            //9


/************************************************************************************
 * Private Data
 ************************************************************************************/
static osThreadId handle_wifi_drv;			//wifi主任务句柄

static UART_HandleTypeDef UartHandle;		//STM32串口句柄
static fifo_t uart1_rx_fifo;						//串口接收fifo
static osSemaphoreId uart1_rx_sem;			//串口接收fifo  信号量

char seach_buf[SEARCH_SIZE];						//检索缓冲
char recv_buf[SEARCH_SIZE/2];						//检索缓冲
int recv_size;													//读取个数

static exceed_buf_t exceed_buf;					//剩余数据缓冲
static tcpc_fifo_ctl_t tcpc_ctl;				//tcp 返回上层句柄

static osMutexId mutex_transfer_all;	//transfer all mutex
static osSemaphoreId cond;						//transfer  信号量(事件)


static mo_cmd_ctl_t cmd_ctl;		//cmd fifo句柄	用于返回给上层cmd执行的结果


/*
  功能:

  bit 0--30	可表示30种返回结果的掩码		31 永远为1(默认不屏蔽)

  0:不上报
  1:上报

  示例:
  添加一条返回结果
  1.定义返回结果的宏 并按顺序 增加掩码编号
  2.在get_cmd_num中增加 返回掩码编号
  3.在pthread_mo_uart_filter中增加filter_stream过滤
  4.在set_filter_en中增加使用命令的屏蔽或者不屏蔽掩码


  命令执行(目前没做):
  1.调用set_filter_en设置预期的返回结果 只打开预期的返回值
  2.判断返回字符串


*/
static unsigned int filter_mask=0;


/************************************************************************************
 * Private Functions
 ************************************************************************************/


int read_exceed_buf(char *p_read)
{

#ifdef WIFI_DRV_DEBUG      //print  seach_buf
    MO_PRINT(("rr"));
    MO_PRINTN((exceed_buf.buf,exceed_buf.available));

#endif


    int temp_available;
    temp_available=exceed_buf.available;

    memcpy(p_read,exceed_buf.buf,exceed_buf.available);
    exceed_buf.available=0;

    return temp_available;
}

void write_exceed_buf(char *p_dat,int len)
{
//	printf("exceed w:%d ",len);
    memcpy(exceed_buf.buf,p_dat,len);
    exceed_buf.available=len;
}

int available_exceed_buf()
{
    return exceed_buf.available;
}

static void delay_ms_cnt(uint32_t n)
{
    volatile uint32_t per;

    while(n--)
    {
        for(per=0;per<8650;per++);
    }
}


static void mo_drv_wifi_reset_hardware()
{
    //esp8266 boot 1
    pinMode(ESP_BOOT,OUTPUT);
    digitalWrite(ESP_BOOT,HIGH);

    //esp8266 reset
    pinMode(ESP_RST,OUTPUT);
    digitalWrite(ESP_RST,LOW);
    delay_ms_cnt(200);
    digitalWrite(ESP_RST,HIGH);
}






/*=======UART1=====================================================
  =================================================================*/
void uart1_hardware_init()
{

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();
    GPIO_InitTypeDef  GPIO_InitStruct;
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_9;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    UartHandle.Instance          = USART1;
    //UartHandle.Init.BaudRate     = 115200;
    UartHandle.Init.BaudRate     = 460800;
    UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits     = UART_STOPBITS_1;
    UartHandle.Init.Parity       = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode         = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&UartHandle);

    //Configure the NVIC for UART
    // HAL_NVIC_SetPriority(USART1_IRQn, 0x0f, 0);
    HAL_NVIC_SetPriority(USART1_IRQn, 0x05, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);

}


void mo_uart1_init()
{
    //串口硬件初始化
    uart1_hardware_init();

    //创建信号量
    osSemaphoreDef(UART1_SEM);
    uart1_rx_sem = osSemaphoreCreate(osSemaphore(UART1_SEM) , 1);

    //创建串口接收fifo
    fifo__init(&uart1_rx_fifo,1024*4);

}

int mo_uart1_read(char *p_buf,int len,int timeout)
{

    int ret,status;

    //如果fifo为空则 阻塞
    if( fifo__avaliable(&uart1_rx_fifo)==0 )
    {
        status=osSemaphoreWait(uart1_rx_sem,osWaitForever);
        MO_ASSERT((status==osOK));
    }

    //读取数据
    ret=fifo__read(&uart1_rx_fifo,(uint8_t *)p_buf,len);

#ifdef WIFI_DRV_DEBUG
    MO_PRINT(("r"));
    MO_PRINTN((p_buf,ret));
#endif

    return ret;

}

int mo_uart1_write(const char *p_buf,int len)
{

    //debug
//#ifdef WIFI_DRV_DEBUG
    MO_PRINT(("w"));
    MO_PRINTN((p_buf,len));
//#endif

    HAL_StatusTypeDef ret;

    ret=HAL_UART_Transmit(&UartHandle,(uint8_t *)p_buf,(uint16_t)len,5);//5ms

    if(ret!=HAL_OK)
    {
        // MO_ERROR(("write error"));
        return -1;
    }
    else
    {
        return len;
    }

}


/*=======CMD=====================================================
  =================================================================*/
int mo_cmd_init()
{

    //创建cmd fifo
    fifo__init(&(cmd_ctl.handle),1024);

    //创建 transfer all mutex
    osMutexDef(TRS_MUT);
    mutex_transfer_all = osMutexCreate(osMutex(TRS_MUT));

    //创建 transfer 信号量
    osSemaphoreDef(TRS_SEM);
    cond = osSemaphoreCreate(osSemaphore(TRS_SEM) , 1);

    return 0;

}

/*


  需要和tcp合并一下


  功能:
  cmd_fifo写入，用于返回给上层cmd执行的结果
  参数:
  数据
  数据长度

  返回:
  写入长度
  -1失败

*/
// extern void print(char* p);
int mo_cmd_fifo_write(const char *p_buf,int len)
{
    //check pram
    if((p_buf==NULL)||(len==0))
    {
        printf("%s:%d bad pram\n",__FILE__,__LINE__);
        return -1;
    }

    //debug
#ifdef WIFI_DRV_DEBUG
    MO_PRINT(("fifo_w"));
    MO_PRINTN((p_buf,len));
#endif

    //写入
    fifo__write( &(cmd_ctl.handle),(const uint8_t *)p_buf,len);

    return len;

}



static void mo_cmd_clear_fifo()
{
    uint8_t temp[32];
    while(fifo__read( &(cmd_ctl.handle),temp,sizeof(temp))>0)
    {
        MO_INFO(("mo_cmd_clear_fifo"));
    }
}





/*
  功能:
  设置 命令预期需要 返回的结果(可屏蔽某些结果)

  参数:
  命令(带结束符的字符串)

*/
void set_filter_en(const char *p_cmd)
{
    if(strncmp(p_cmd,"AT+CIPSEND=",strlen("AT+CIPSEND="))==0)
    {
        filter_mask=0xffffffef;		//屏蔽OK返回
    }
    else if(strncmp(p_cmd,"AT+CIPSTATUS",strlen("AT+CIPSTATUS"))==0)
    {
        //filter_mask=0xffffffef;		//屏蔽OK返回
        filter_mask=0xffffffff;		//不屏蔽OK返回
    }
    else if(strncmp(p_cmd,"AT+CWSTARTSMART",strlen("AT+CWSTARTSMART"))==0)  // smartconfig 屏蔽 OK   AT+CWSTARTSMART=1
    {
        //filter_mask=0xffffffef;		//屏蔽OK返回
        filter_mask=0xffffffff;		//不屏蔽OK返回
    }
    else if(strncmp(p_cmd,"AT+IR_DOWNFILE",strlen("AT+IR_DOWNFILE"))==0)
    {
        filter_mask=0xffffffef;		//屏蔽OK返回
    }
    else if(strncmp(p_cmd,"AT+IR_DOWNPROGRESS",strlen("AT+IR_DOWNPROGRESS"))==0)
    {
        filter_mask=0xffffffef;		//屏蔽OK返回
    }
    else if(strncmp(p_cmd,"AT+CWJAP_CUR?",strlen("AT+CWJAP_CUR?"))==0)
    {
        MO_DEBUG(("set filter CWJAP"));
        filter_mask=0xffffffef;		//屏蔽OK返回
    }
    else
    {
        filter_mask=0xffffffff;		//其他 不屏蔽的所有的结果都上报
    }
}


/*
  功能:
  清除掩码

*/
void clear_filter_en()
{
    filter_mask=0;
}

void clear_buf_sf()
{
    memset(seach_buf,'$',SEARCH_SIZE-1);     //clear buf
    seach_buf[SEARCH_SIZE-1]=0x00;            //max  index  SEARCH_SIZE-1
    memset(recv_buf,0x00,sizeof(recv_buf));
}




/**
   参数:
   p_cmd        发送的命令
   len_cmd     发送命令长度    不包含结束符 推荐strlen(cmd)
   p_get          返回结果缓冲区
   len              返回结果缓冲区长度       推荐 sizeof(buf)
   timeout       命令返回超时时间  单位秒
   lock_flag     命令加/释放锁标志        normal:0x03          start:0x01     end:0x02(单独则不发送命令结束符用于tcp发送)  (bit 0  lock_all /bit 1  unlock_all  )

   返回:
   失败        MO_FAILED(-1)
   成功        数据长度

**/



int mo_drv_wifi_cmd_transfer(const char *p_cmd,int len_cmd,char *p_get,int len,unsigned int timeout,unsigned char lock_flag)
{

    //pram check
    if((p_get==NULL)||(len==0))
    {
        MO_ERROR(("bad pram"));
        return MO_FAILED;
    }

    int status;
    //lock all
    if( lock_flag&0x01 )
    {
        if(osMutexWait(mutex_transfer_all, osWaitForever) != osOK)
        {
            MO_ERROR(("error osMutexWait"));
        }
    }

    //clear fifo
    mo_cmd_clear_fifo();

    //set
    set_filter_en(p_cmd);

    MO_INFO(("uart1 write cmd %s\r\n", p_cmd));

    //write cmd
    if(mo_uart1_write(p_cmd,len_cmd)<0)
    {
        MO_ERROR(("mo_uart1_write failed"));
        goto error;
    }

    if(lock_flag!=0x02)   //tcp发送不需要发送结束符
    {
        if(mo_uart1_write("\r\n",2)<0)
        {
            MO_ERROR(("mo_uart1_write failed"));
            goto error;
        }
    }

    // wait signal
    // MO_INFO(("before osSemaphoreWait\r\n"));
    status=osSemaphoreWait(cond, timeout*1000);

    // clear
    clear_filter_en();
    // MO_INFO(("clear_filter_en\r\n"));

    int ret;
    if(status==osOK)
    {
        //read data
        // MO_INFO(("READ  DATA\r\n"));
        ret=fifo__read( &(cmd_ctl.handle) ,(uint8_t *)p_get,len);
        //   MO_INFO(("task2 =====go"));
        goto ok;
    }
    else if(status==osEventTimeout)
    {
        MO_ERROR(("time out osSemaphoreWait"));
        goto error;
    }
    else
    {
        MO_ERROR(("pthread_cond_timedwait error"));
        goto error;
    }


ok:
    //unlock all
    if(lock_flag&0x02)
    {

        status = osMutexRelease(mutex_transfer_all);
        if (status != osOK)
        {
            MO_ERROR(("osMutexRelease failed"));
        }

    }

    return ret;


error:
    //unlock all
    if(lock_flag&0x02)
    {
        status = osMutexRelease(mutex_transfer_all);
        if (status != osOK)
        {
            MO_ERROR(("osMutexRelease failed"));
        }
        //  MO_DEBUG(("trs  unlock"));
    }
    return MO_FAILED;

}






/*

  参数:
  cmd            命令(有结束符的字符串)
  ret_cmd     预期返回结果(有结束符的字符串)
  time_out    超时时间(单位秒)

  返回:
  失败    0
  成功    1

*/
// extern void print(char* p);
int mo_drv_wifi_run_cmd(char *cmd,char *ret_cmd,unsigned int time_out)
{

    unsigned char len_buf;
    len_buf= strlen(ret_cmd)+1;

    char *p_buf;
    p_buf=(char *)pvPortMalloc(len_buf);
    MO_ASSERT((p_buf!=NULL));

    memset(p_buf,0x00,len_buf);


    if(mo_drv_wifi_cmd_transfer(cmd,strlen(cmd),p_buf,len_buf,time_out,0x03)>0)
    {
        MO_DEBUG(("p_buf: %s", p_buf));

        if(memcmp(p_buf,ret_cmd,strlen(ret_cmd))==0)
        {
            MO_INFO(("OK %s",cmd));
            goto ret_ok;
        }
        else
        {
            MO_ERROR(("Failed %s",cmd));
            goto ret_er;
        }
    }
    else
    {
        MO_ERROR(("Time out %s",cmd));
        goto ret_er;
    }


ret_er:
    vPortFree(p_buf);
    return 0;



ret_ok:
    vPortFree(p_buf);
    return 1;


}




/*
  功能:
  过滤TCP回传的数据用
  在缓冲区中搜索TCP回传数据头"+IPD,0,32:",并返回数据通道和数据长度以数据的多一个地址

  参数:
  缓冲区起始地址(0x00结尾)
  返回的通道
  返回的数据长度

  返回:
  跳过头  纯数据的第一个地址
  未发现头 NULL

  示例:
  接收到的数据 +IPD,0,32:http://www.cmsoft.cn QQ:1086

*/
char *strstr_tcp(char *p_dat,unsigned char *channel,int *len)
{
    char *p_index1,*p_index2,*p_index3;

#if 0
    MO_PRINT(("strstr_tcp:"));
    MO_PRINTN((p_dat,SEARCH_SIZE-1));
#endif

    //搜索到 +IPD,
    if((p_index1=strstr(p_dat,HEAD_TCPC))!=NULL)
    {
        // MO_PRINT(("GET:%s\n",p_index1));


        //搜索第二个到 ,
        if((p_index2=strstr(p_index1+sizeof(HEAD_TCPC)-1,","))!=NULL)
        {
            // MO_PRINT(("GET:%s\n",p_index2));

            //搜索到:
            if((p_index3=strstr(p_index2+sizeof(",")-1,":"))!=NULL)
            {
                // MO_PRINT(("GET:%s\n",p_index3));

                //获取通道
                *channel=*(p_index1+5)-'0';
                //获取数据长度
                char temp[8];
                memset(temp,0x00,sizeof(temp));
                memcpy(temp,p_index2+1,p_index3-p_index2);
                *len=atoi(temp);

                //清除s缓冲区
                memset(p_index1,'$',p_index3-p_index1+1);


                // MO_PRINT(("channel=%d len=%d\n",*channel,*len));

                //判断结果
                if( (0<=(*channel)) && ((*channel)<=5) && (0<(*len)) && ((*len)<4096) )
                {
                    return p_index3+1;
                }
                else
                {
                    MO_ERROR(("over flow"));
                }
            }
        }
    }

    //  printf("null\n");
    return NULL;


}




/*========TCP_FIFO=========================================================
  ==========================================================================*/
int mo_drv_wifi_creat_tcpc_fifo(unsigned char fifo_num)
{
    //check pram
    if( (fifo_num<0) || (fifo_num>4) )
    {
        MO_ERROR(("bad pram"));
        return MO_FAILED;
    }

    return fifo__init( &(tcpc_ctl.handle[fifo_num]),1024 );

}

int mo_drv_wifi_destroy_tcpc_fifo(unsigned char fifo_num)
{

    //check pram
    if( (fifo_num<0) || (fifo_num>4) )
    {
        MO_ERROR(("bad pram"));
        return MO_FAILED;
    }

    fifo__deinit(&(tcpc_ctl.handle[fifo_num]));

    return 0;

}

/*
  功能:
  查看tcp缓冲区是否有数据

  参数:
  tcp连接号码

  返回:
  1 有
  0 无
*/
int mo_drv_wifi_available_tcpc_fifo(unsigned char  fifo_num)
{
    //check pram
    if( (fifo_num<0) || (fifo_num>4) )
    {
        MO_ERROR(("bad pram"));
        return MO_FAILED;
    }

    return fifo__avaliable( &(tcpc_ctl.handle[fifo_num]) );
}

int mo_drv_wifi_read_tcpc_fifo(char *p_buf,int len,unsigned char fifi_num)
{

    return fifo__read(&(tcpc_ctl.handle[fifi_num]),(uint8_t *)p_buf,len);

#if 0
    MO_ASSERT((ret>0));
    //debug
    if(ret>0)
    {
        // MO_PRINT(("r_tcpc_fifo"));
        // MO_PRINTN((p_buf,ret));
    }

    return ret;
#endif
}

/*


  返回:
  n  写入长度
  -1  失败

*/

int mo_drv_wifi_write_tcpc_fifo(const char *p_buf,int len,unsigned char fifo_num)
{

    //check pram
    if((p_buf==NULL) ||(fifo_num>4) )
    {
        MO_ERROR(("bad pram %d %d %d",p_buf,fifo_num));
        return MO_FAILED;
    }

    //debug
#ifdef WIFI_DRV_DEBUG
    MO_PRINT(("w_fifo_tcpc[%d]",fifo_num));
    MO_PRINTN((p_buf,len));
#endif

    //write
    fifo__write( &(tcpc_ctl.handle[fifo_num]),(const uint8_t *)p_buf,len);

    return len;

}






/*
  功能:
  获取一条	返回结果的 掩码位置

*/
unsigned char get_cmd_mask_num(const char *p_cmd)
{

    if(strcmp(p_cmd,SEND_OK_TCPC)==0)
    {
        return 0;		//bit 0   0x01
    }
    else if(strcmp(p_cmd,RET_REST)==0)
    {
        return 1;		//bit 1   0x02
    }
    else if(strcmp(p_cmd,SEND_ACK_TCPC)==0)
    {
        return 2;		//bit 2   0x04
    }
    else if(strcmp(p_cmd,RET_STATUS_2)==0)
    {
        return 3;		//bit 4   0x08
    }
    else if(strcmp(p_cmd,RET_OK)==0)
    {
        return 4;		//bit 5   0x10
    }
    else if(strcmp(p_cmd,ALREADY_CONNECT)==0)
    {
        return 5;
    }
    else if(strcmp(p_cmd,RET_STATUS_5)==0)
    {
        return 6;
    }
    else if(strcmp(p_cmd,HEAD_IPSTATUS)==0)
    {
        return 7;
    }
    else
    {
        // MO_ERROR(("Failed get_cmd_mask_num %s",p_cmd));
        MO_INFO(("No cmd mask num %s",p_cmd));
        //return 30;	//上抛结果
        return 31;	//上抛结果
    }

}






/*
  功能:
  过滤TCP状态用
  在缓冲区中搜索TCP状态头,并获取通道、类型 (以获取当前网络状态)

  参数:
  缓冲区起始地址(0x00结尾)
  获取的通道
  获取的类型 Type:'1' tcp  '2' udp

  返回:
  发现:头的首地址
  未发现头: NULL

  示例:
  接收到的数据 +CIPSTATUS:0,"TCP","192.168.0.124",2345,0

*/

char *strstr_ipstaus(char *p_dat,unsigned char *channel, unsigned char *type)
{
    char *p_index1,*p_index2,*p_index3;

#if 0
    MO_PRINT(("strstr_tcp:"));
    MO_PRINTN((p_dat,SEARCH_SIZE-1));
#endif

    //搜索到+CIPSTATUS:
    if((p_index1=strstr(p_dat,HEAD_IPSTATUS))!=NULL)
    {
        //   MO_PRINT(("GET:%s\n",p_index1));
        //搜索到第一个,
        if((p_index2=strstr(p_index1+strlen(HEAD_IPSTATUS),","))!=NULL)
        {
            // MO_PRINT(("GET:%s\n",p_index2));
            //搜索到第二个,
            if((p_index3=strstr(p_index2+strlen(","),","))!=NULL)
            {
                //  MO_PRINT(("GET:%s\n",p_index3));
                //获取通道
                *channel=*(p_index1+11);
                //获取数据长度

                *type=0;
                if(memcmp( (p_index1+14),"TCP",3)==0 )
                {
                    *type='1';
                }
                else if(memcmp( (p_index1+14),"UDP",3)==0 )
                {
                    *type='2';
                }

                // MO_DEBUG(("ip_channel=%c ip_type=%c",*channel,*type));
                memset(p_index1,'$',strlen(HEAD_IPSTATUS));  //清除s缓冲区


                //判断结果
                if( ('0'<=(*channel))&& ((*channel)<'5') && ('0'<(*type)) && ((*type)<'3')  )
                {
                    return p_index1;
                }
            }
        }
    }

    //  printf("null\n");
    return NULL;


}



// void print(char* p)
// {
//     Serial.write("*");
//     while(p)
//     {
//         Serial.write(*p++);
//     }
//     Serial.write("*");
// }

// void printn(char* p, uint8_t n)
// {
//     Serial.write("#");
//     while(n--)
//     {
//         Serial.write(*p++);
//     }
//     Serial.write("#");
// }



/*
  功能:
  从缓冲区过滤出需要的数据并从缓冲区删除，
  并写入cmdfifo，并发送条件信号到上层(根据信号掩码)

  参数:
  缓冲区起始地址(0x00结尾)
  需要过滤的字符串(有结束符)
  是否在写入数据之后向上发送send_signal 1发送 0不发 如果接收到的字符后面需要使用OK发送信号触发，此次传输结束则设置为0

  返回:
  MO_FAILED   失败
  MO_SUCCESS  成功


*/
static int filter_stream(char *seach_buf,const char *p_filter,char en_send_signal)
{

    char *p_index;

    // deal with smartconfig specially
    if (0 == strcmp(p_filter, RET_SMARTCONFIG))
    {
        if ((p_index=strstr(seach_buf,p_filter))!=NULL)
        {
            memset(p_index,'$',strlen(p_filter));  //clear data
            return MO_SUCCESS;
        }
    }


    if((p_index=strstr(seach_buf,p_filter))!=NULL)
    {
        memset(p_index,'$',strlen(p_filter));  //clear data
        unsigned char cmd_num=get_cmd_mask_num(p_filter);

        // if( (signal) ||(mo_drv_wifi_get_cond_flag()) )
        if (filter_mask & (0x01<<cmd_num))
        {
            //write in
            if(mo_cmd_fifo_write(p_filter,strlen(p_filter)+1)<0)
            {
                MO_ERROR(("mo_cmd_fifo_write failed"));
                return MO_FAILED;
            }

            if(en_send_signal)
            {
                osSemaphoreRelease(cond);
            }
        }
        else
        {
            // MO_INFO(("NO need send"));
        }
        return MO_SUCCESS;
    }
    return MO_FAILED;
}


void task_mo_uart_filter(void const *argument)
{
    mo_cmd_init();
    mo_uart1_init();

    memset(seach_buf,'$',SEARCH_SIZE-1);     //clear buf
    seach_buf[SEARCH_SIZE-1]=0x00;            //max  index  SEARCH_SIZE-1

    char *p_index;
    memset(recv_buf,0x00,sizeof(recv_buf));

    while(1)
    {
        //读取数据
        if( available_exceed_buf() )
        {
            recv_size=read_exceed_buf(recv_buf);	//tcp 接收完数据 剩余的数据
        }
        else
        {
            recv_size=mo_uart1_read(recv_buf,sizeof(recv_buf),100*1000);		//串口读取
        }

        //过滤数据
        if(recv_size>0)
        {
        recv_head:

            //debug
#if 0
            MO_PRINT(("recv:"));
            MO_PRINTN((recv_buf,recv_size));
#endif

            memmove(seach_buf,seach_buf+recv_size,SEARCH_SIZE-1-recv_size);
            memcpy(seach_buf+SEARCH_SIZE-1-recv_size, recv_buf, recv_size);

            memset(recv_buf,0x00,sizeof(recv_buf));     //clear buf

        prase_head:

#ifdef WIFI_DRV_DEBUG_S      //print  seach_buf

            MO_PRINT(("s"));
            MO_PRINTN((seach_buf,SEARCH_SIZE-1));

#endif
            unsigned char tcpc_channel;   //接收到的数据通道
            int tcpc_len,tcpc_w_index;    //接收到的数据长度  已经写入数据长度

            if((p_index=strstr_tcp(seach_buf,&tcpc_channel,&tcpc_len))!=NULL)        //TCPC  filter
            {
                //   printf("Get +%s\n",HEAD_TCPC);

                tcpc_w_index=0;
                //seach_buf   tcp head+dat+other head2
                if( p_index != (seach_buf+SEARCH_SIZE-1) )	//seach_buf缓冲区中还包含有数据 则需要写入
                {
                    //seach_buf  data
                    if( ((seach_buf+SEARCH_SIZE-1)-p_index) < tcpc_len )//seach_buf缓冲区 只是部分数据
                    {

                        if( mo_drv_wifi_write_tcpc_fifo(p_index,(seach_buf+SEARCH_SIZE-1)-p_index,tcpc_channel) <0 )
                        {
                            MO_ERROR(("mo_cmd_fifo_write failed"));
                        }

                        tcpc_w_index+=( (seach_buf+SEARCH_SIZE-1)-p_index );
                        memset(p_index,'$',(seach_buf+SEARCH_SIZE-1)-p_index); //清除s缓冲区
                    }
                    else    //seach_buf   tcp head+dat+other head2  //seach_buf缓冲区 包含此次所有数据
                    {
                        //write in
                        if(mo_drv_wifi_write_tcpc_fifo(p_index,tcpc_len,tcpc_channel) <0 )
                        {
                            MO_ERROR(("mo_cmd_fifo_write failed"));
                        }

                        //清空此次数据
                        memset(p_index,'$',tcpc_len);  //清除s缓冲区

                        //跳转到数据解析(有可能出现多个IP包)
                        MO_INFO(("goto prase_head"));
                        goto prase_head;

                    }
                }

                //clear buf
                //memset(seach_buf,'$',SEARCH_SIZE-1);  //clear buf


                while(1)   //seach_buf缓冲区 只是部分数据  接收剩余数据
                {
                    recv_size=mo_uart1_read(recv_buf,sizeof(recv_buf),100*1000);
                    if(recv_size>0)
                    {
                        //debug
                        /* MO_PRINT(("recv_buf"));
                           MO_PRINTN((recv_buf,recv_size));*/

                        tcpc_w_index+=recv_size;

                        //only  dat  数据未完
                        if(tcpc_w_index<tcpc_len)
                        {
                            if(mo_drv_wifi_write_tcpc_fifo(recv_buf,recv_size,tcpc_channel)<0)
                            {
                                MO_ERROR(("mo_cmd_fifo_write failed"));
                            }
                        }//dat+other head   遇到数据结尾
                        else if(tcpc_w_index>=tcpc_len)
                        {
                            int available_len;
                            available_len=recv_size-(tcpc_w_index-tcpc_len);
                            if(mo_drv_wifi_write_tcpc_fifo(recv_buf, available_len,tcpc_channel)<0)
                            {
                                MO_ERROR(("mo_cmd_fifo_write failed"));
                            }



                            // memset(recv_buf,'$',available_len);

                            write_exceed_buf(recv_buf+available_len,recv_size-available_len);
                            break;
                            //memset(seach_buf,'$',sizeof(seach_buf)-1);  //clear buf
                            // MO_INFO(("TCPC END go  recv_head"));
                            //goto recv_head;
                        }
                    }
                }
            }


            if(filter_stream(seach_buf,SEND_OK_TCPC,1)==MO_SUCCESS)            //filter tcp  send ok
            {
                //  MO_INFO(("GET %s",SEND_OK_TCPC));
            }

            if(filter_stream(seach_buf,RET_REST,1)==MO_SUCCESS)           //filter ret reset
            {
                //   MO_INFO(("GET %s",RET_REST));
            }
            if(filter_stream(seach_buf,RET_STATUS_2,0)==MO_SUCCESS)           //filter STATUS:2
            {
                //    MO_INFO(("GET %s",RET_STATUS_2));
            }
            if(filter_stream(seach_buf,RET_STATUS_5,0)==MO_SUCCESS)           //filter STATUS:5
            {
                //    MO_INFO(("GET %s",RET_STATUS_2));
            }

            unsigned char ret_ipstatus[3]={0,0,'$'};  //channel  type  separator('$')
            if((p_index=strstr_ipstaus(seach_buf,&ret_ipstatus[0],&ret_ipstatus[1]))!=NULL)     // TCPC  connected
            {
                //     MO_INFO(("GET ipstaus"));
                //    memset(seach_buf,'$',sizeof(seach_buf)-1);  //clear dat

                unsigned char cmd_num=get_cmd_mask_num(HEAD_IPSTATUS);

                //  if(mo_drv_wifi_get_cond_flag())
                if(filter_mask&(0x01<<cmd_num))
                {
                    //write in
                    if(mo_cmd_fifo_write((const char *)ret_ipstatus,3)<0)
                    {
                        MO_ERROR(("mo_cmd_fifo_write failed"));
                    }

#if 0
                    //send  signal
                    int status;
                    status = pthread_mutex_lock(&mutex);
                    MO_ASSERT((status==0));

                    status = pthread_cond_signal(&cond);
                    MO_ASSERT((status==0));

                    status = pthread_mutex_unlock(&mutex);
                    MO_ASSERT((status==0));
#endif
                }
                else
                {
                    //MO_INFO(("NO need send"));
                }

            }

            // search the comeback data for the commands
            // "AT+CWJAP_CUR?"; "AT+CIPSTAMAC_DEF?"; "AT+CIPAPMAC_DEF?"
            // TODO: add mux lock
            char p_filter[20] = "+CWJAP_CUR:";
            char *ptr1 = NULL;
            char foundstr[64] = {0};
            if((p_index = strstr(seach_buf, p_filter)) != NULL)
            {
                if ( (ptr1 = strstr(p_index, "\r\n")) != NULL )
                {
                    MO_DEBUG(("Found \\r\\n"));
                    memcpy(foundstr, p_index, ptr1 - p_index + 2);
                    MO_DEBUG(("foundstr: %s", foundstr));
                    if(mo_cmd_fifo_write(p_index, ptr1 - p_index + 2) < 0) // +2 for \r\n
                    {
                        MO_ERROR(("mo_cmd_fifo_write failed"));
                    }
                    memset(p_index,'$',strlen(p_filter));  //clear data
                    osSemaphoreRelease(cond);
                }
                else
                {
                    continue;
                }
            }

            //netdown
            memset(p_filter, 0, sizeof(p_filter));
            memcpy(p_filter, "+IR_NETDOWN:", strlen("+IR_NETDOWN:"));
            if((p_index = strstr(seach_buf, p_filter)) != NULL)
            {
                if ( (ptr1 = strstr(p_index, "\r\n")) != NULL )
                {
                    MO_DEBUG(("Found \\r\\n"));
                    memcpy(foundstr, p_index, ptr1 - p_index + 2);
                    if(!strcmp(foundstr,"+IR_NETDOWN:0\r\n"))
                    {
                        net_down_flag = 0;
                    }
                    else if(!strcmp(foundstr,"+IR_NETDOWN:1\r\n"))
                    {
                        net_down_flag = 1;
                    }
                    else if(!strcmp(foundstr,"+IR_NETDOWN:2\r\n"))
                    {
                        net_down_flag = 2;
                    }
                    memset(p_index,'$',strlen(p_filter));  //clear data
                }
                else
                {
                    continue;
                }
            }

            //downfile
            /*
            memset(p_filter, 0, sizeof(p_filter));
            memcpy(p_filter, "+DOWNFILE:", strlen("+DOWNFILE:"));
            if((p_index = strstr(seach_buf, p_filter)) != NULL)
            {
                if ( (ptr1 = strstr(p_index, "\r\n")) != NULL )
                {
                    MO_DEBUG(("Found \\r\\n"));
                    memcpy(foundstr, p_index, ptr1 - p_index + 2);
                    if(!strcmp(foundstr,"+DOWNFILE::0\r\n"))
                    {
                        online_downfile_flag = 0;
                    }
                    else if(!strcmp(foundstr,"+DOWNFILE::1\r\n"))
                    {
                        online_downfile_flag = 1;
                    }
                    else if(!strcmp(foundstr,"+DOWNFILE::2\r\n"))
                    {
                        online_downfile_flag = 2;
                    }
                    memset(p_index,'$',strlen(p_filter));  //clear data
                }
                else
                {
                    continue;
                }
            }*/

            //getversion
            memset(p_filter, 0, sizeof(p_filter));
            memcpy(p_filter, "+IR_GETVERSION", strlen("+IR_GETVERSION"));
            if((p_index = strstr(seach_buf, p_filter)) != NULL)
            {
                if ( (ptr1 = strstr(p_index, "\r\n")) != NULL )
                {
                    MO_DEBUG(("Found \\r\\n"));
                    memcpy(foundstr, p_index, ptr1 - p_index + 2);
                    MO_DEBUG(("foundstr: %s", foundstr));
                    if(mo_cmd_fifo_write(p_index, ptr1 - p_index + 2) < 0) // +2 for \r\n
                    {
                        MO_ERROR(("mo_cmd_fifo_write failed"));
                    }
                    memset(p_index,'$',strlen(p_filter));  //clear data
                    osSemaphoreRelease(cond);
                }
                else
                {
                    continue;
                }
            }

            //progress
            memset(p_filter, 0, sizeof(p_filter));
            memcpy(p_filter, "+IR_DOWNPROGRESS:", strlen("+IR_DOWNPROGRESS:"));
            if((p_index = strstr(seach_buf, p_filter)) != NULL)
            {
                if ( (ptr1 = strstr(p_index, "\r\n")) != NULL )
                {
                    MO_DEBUG(("Found \\r\\n"));
                    memcpy(foundstr, p_index, ptr1 - p_index + 2);
                    MO_DEBUG(("foundstr: %s", foundstr));
                    if(mo_cmd_fifo_write(p_index, ptr1 - p_index + 2) < 0) // +2 for \r\n
                    {
                        MO_ERROR(("mo_cmd_fifo_write failed"));
                    }
                    memset(p_index,'$',strlen(p_filter));  //clear data
                    osSemaphoreRelease(cond);
                }
                else
                {
                    continue;
                }
            }

            // station mac
            memset(p_filter, 0, sizeof(p_filter));
            memcpy(p_filter, "+CIPSTAMAC_DEF:", strlen("+CIPSTAMAC_DEF:"));
            if((p_index = strstr(seach_buf, p_filter)) != NULL)
            {
                if ( (ptr1 = strstr(p_index, "\r\n")) != NULL )
                {
                    MO_DEBUG(("Found \\r\\n"));
                    memcpy(foundstr, p_index, ptr1 - p_index + 2);
                    MO_DEBUG(("foundstr: %s", foundstr));
                    if(mo_cmd_fifo_write(p_index, ptr1 - p_index + 2) < 0) // +2 for \r\n
                    {
                        MO_ERROR(("mo_cmd_fifo_write failed"));
                    }
                    memset(p_index,'$',strlen(p_filter));  //clear data
                    osSemaphoreRelease(cond);
                }
                else
                {
                    continue;
                }
            }

            // apmac
            memset(p_filter, 0, sizeof(p_filter));
            memcpy(p_filter, "+CIPAPMAC_DEF:", strlen("+CIPAPMAC_DEF:"));
            if((p_index = strstr(seach_buf, p_filter)) != NULL)
            {
                if ( (ptr1 = strstr(p_index, "\r\n")) != NULL )
                {
                    MO_DEBUG(("Found \\r\\n"));
                    memcpy(foundstr, p_index, ptr1 - p_index + 2);
                    MO_DEBUG(("foundstr: %s", foundstr));
                    if(mo_cmd_fifo_write(p_index, ptr1 - p_index + 2) < 0) // +2 for \r\n
                    {
                        MO_ERROR(("mo_cmd_fifo_write failed"));
                    }
                    memset(p_index,'$',strlen(p_filter));  //clear data
                    osSemaphoreRelease(cond);
                }
                else
                {
                    continue;
                }
            }


            // wifi list
            memset(p_filter, 0, sizeof(p_filter));
            memcpy(p_filter, "+CWLAP:", strlen("+CWLAP:"));
            //MO_DEBUG(("+CWLAP filter stream"));
            static int filterNum = 0;
            if((p_index = strstr(seach_buf, p_filter)) != NULL)
            {
                if ( (ptr1 = strstr(p_index, "\r\n")) != NULL )
                {
                    memcpy(foundstr, p_index, ptr1 - p_index + 2);
                    MO_DEBUG(("foundstr: %s", foundstr));
                    if(mo_cmd_fifo_write(p_index, ptr1 - p_index + 2) < 0) // +2 for \r\n
                    {
                        MO_ERROR(("mo_cmd_fifo_write failed"));
                    }
                    memset(p_index,'$',strlen(p_filter));  //clear data
                    filterNum++;
                    if (filterNum > 10)
                    {
                        osSemaphoreRelease(cond);
                        filterNum = 0;
                    }
                    else
                    {
                        MO_DEBUG(("continue"));
                        continue;
                    }
                }
                else
                {
                    MO_DEBUG(("continue"));
                    continue;
                }
            }

            // continue for the up part
            if (filterNum > 0) // if the wifi num is less than 10
            {
                MO_DEBUG(("+CWLAP release semaphore"));
                osSemaphoreRelease(cond);
                filterNum = 0;
            }


            if(filter_stream(seach_buf,RET_SMARTCONFIG,1)==MO_SUCCESS)           //filter smartconfig ed
            {
                smartconfig_over_flag = 1;
                MO_INFO(("GET %s",RET_SMARTCONFIG));
            }

            if(filter_stream(seach_buf,RET_MD5SET,1)==MO_SUCCESS)           //filter MD5SET ed
            {
                MO_INFO(("GET %s",RET_MD5SET));
            }
            if(filter_stream(seach_buf,RET_DOWNFW,1)==MO_SUCCESS)           //filter RET_DOWNFW ed
            {
                MO_INFO(("GET %s",RET_DOWNFW));
            }
            if(filter_stream(seach_buf,RET_OK,1)==MO_SUCCESS)                //filter OK
            {
                //    MO_INFO(("GET %s","OK"));
            }
            if(filter_stream(seach_buf,SEND_ACK_TCPC,1)==MO_SUCCESS)           //filter >
            {
                //   MO_INFO(("GET %s",RET_REST));
            }
            if(filter_stream(seach_buf,ALREADY_CONNECT,1)==MO_SUCCESS)      //filter tcp already
            {
                //   MO_INFO(("GET %s",ALREADY_CONNECT));
            }
        }

    }
}



/************************************************************************************
 *  Public Functions
 ************************************************************************************/
extern "C"
{

    void WifiDrv_USART1_Interrupt_Handler(void)
    {

        if(__HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_RXNE ) != RESET)
        {
            // Read byte from the receive data register
            unsigned char c = (uint16_t)(USART1->DR & (uint16_t)0x00FF);
            //MO_INFO(("%c",c));
            fifo__write(&uart1_rx_fifo,&c,1);
            osSemaphoreRelease(uart1_rx_sem);
        }
    }


}

int mo_drv_wifi_set_default_mode()
{
#if 1
    //wifi模块复位
    mo_drv_wifi_reset_hardware();
    //关闭回显
    delay(4000);
    do
    {
        clear_buf_sf();
        MO_DEBUG(("ATE0"));
    }
    while( mo_drv_wifi_run_cmd("ATE0","OK",2)==0 );
#endif

    //tcp设置为多连接
    int ret = 0;
    while ( ret == 0 )
    {
        MO_DEBUG(("CIPMUX"));
        ret = mo_drv_wifi_run_cmd("AT+CIPMUX=1","OK",10);
    }
    return 1;
}

void mo_drv_wifi_init()
{
#ifdef WIFI_HARDWARE_ENABLE
    //creat wifi uart filter task
    osThreadDef(WIFI_DRV, task_mo_uart_filter, osPriorityNormal,0,1024);
    handle_wifi_drv = osThreadCreate(osThread(WIFI_DRV),NULL);
    MO_ASSERT((handle_wifi_drv!=NULL));
#endif

}
