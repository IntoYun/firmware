#include "main.h"
#include "cmsis_os.h"

#include "lib_system_all.h"
#include "lib_wifi_drv.h"
#include "wiring_hal.h"


#if 0

/*  sample

    int ret;
    ret=TcpClient_connect("192.168.0.122", 2345);
    MO_ASSERT((ret==MO_SUCCESS));

*/


#endif


int TcpClient_connect_hal(const char *host, uint16_t port,unsigned char handle)
{
    if( (host==NULL) || (port == NULL) ||(handle>4) )
    {
        MO_ERROR(("bad pram"));
        goto error;
    }

    char temp_cmd[64];
    memset(temp_cmd,0x00,sizeof(temp_cmd));
    sprintf(temp_cmd,"AT+CIPSTART=%d,\"TCP\",\"%s\",%d",handle,host,port);
   // MO_DEBUG(("TCP connect:%s",temp_cmd));

    char temp[16];
    memset(temp,0x00,sizeof(temp));

    if(mo_drv_wifi_cmd_transfer(temp_cmd,strlen(temp_cmd),temp,sizeof(temp),20,0x03)>0)
    {
       // MO_DEBUG(("ret {%s} ",temp));
        if(memcmp(temp,"OK",2)==0)
        {
            MO_INFO(("OK TcpClient_connect_hal %d",handle));
            goto ok;
        }
        else if(memcmp(temp,"ALREAY CONNECT",14)==0)
        {
            MO_INFO(("OK TcpClient_connect_hal(already connected) %d",handle));
            goto ok;
        }
        else
        {
            MO_ERROR(("Failed TcpClient_connect_hal %d",handle));
            goto error;
        }
    }
    else
    {
        MO_ERROR(("Time out TcpClient_connect_hal %d",handle));
        goto error;
    }


   ok:
        return MO_SUCCESS;

    error:
        return MO_FAILED;


}


static char TcpClient_handle_manager[5]={0,0,0,0,0};

unsigned char TcpClient_open_hal()
{

    //handle
    int i;
    for(i=0;i<5;i++)
    {
        if(TcpClient_handle_manager[i]==0)
        {
             //creat fifo
            int ret;
            ret=mo_drv_wifi_creat_tcpc_fifo(i);
            if(ret!=MO_SUCCESS)
            {
                MO_ERROR(("mo_drv_wifi_creat_tcpc_fifo failed"));
                return MO_FAILED;
            }

            //assign handle
            TcpClient_handle_manager[i]=1;
            return i;
        }
    }

    MO_ERROR(("TcpClient_open_hal failed handle use up"));
    return MO_FAILED;
}


unsigned char TcpClient_close_hal(unsigned char handle)
{
    //check pram
    if(handle>4)
    {
        MO_ERROR(("bad pram"));
        return MO_FAILED;
    }

    //destory fifo
    if(mo_drv_wifi_destroy_tcpc_fifo(handle)!=MO_SUCCESS)
    {
        MO_ERROR(("mo_drv_wifi_destroy_tcpc_fifo failed"));
        return MO_FAILED;
    }

    //recycle handle
    TcpClient_handle_manager[handle]=0;

    return MO_SUCCESS;

}



int TcpClient_readn_hal(unsigned char *buff, int size,unsigned char tcp_handle)
{
    //check pram
    if( (buff==NULL) || (size==0) ||(tcp_handle>4) )
    {
        MO_ERROR(("bad pram"));
        return 0;
    }

    return mo_drv_wifi_read_tcpc_fifo((char *)buff,size,tcp_handle);
}

int TcpClient_available_hal(unsigned char handle)
{
    //check pram
    if(handle>4)
    {
         MO_ERROR(("bad pram"));
         return 0;
    }

    return mo_drv_wifi_available_tcpc_fifo(handle);
}

/*



return:  failed 0  / write count
*/


int TcpClient_writen_hal(const unsigned char *p_dat,int len,unsigned char tcp_handle)
{
	/*
	MO_ERROR(("tcp_write"));
	MO_PRINTN(((const char *)p_dat,len));
	*/


    //check pram
    if( (p_dat==NULL) ||(len==0)||(tcp_handle>4) )
    {
        MO_ERROR(("bad pram"));
        return 0;
    }

    //send cmd
    char temp_w[24];
    char temp_r[32];
    memset(temp_w,0x00,sizeof(temp_w));
    sprintf(temp_w,"AT+CIPSEND=%d,%d",tcp_handle,len);

    if(mo_drv_wifi_cmd_transfer((const char *)temp_w,strlen(temp_w),temp_r,sizeof(temp_r),10,0x01)>0)
    {
        if(memcmp(temp_r,">",1)==0)
        {
            //   MO_INFO(("GET >"));
            MO_INFO(("0.5> TcpClient_writen_hal %d",tcp_handle));
        }
        else
        {
            MO_ERROR(("0.5Failed TcpClient_writen_hal %d",tcp_handle));
        }
    }
    else
    {
        MO_ERROR(("0.5Time out TcpClient_writen_hal %d",tcp_handle));
    }


    //send data
    int ret;
    if( (ret=mo_drv_wifi_cmd_transfer((const char *)p_dat,len,temp_r,sizeof(temp_r),10,0x02)) >0)
    {
        if(memcmp(temp_r,"SEND OK",7)==0)
        {
            MO_INFO(("OK TcpClient_writen_hal %d",tcp_handle));
            //usleep(5000*1000);
            return ret;
        }
        else
        {
            MO_ERROR(("Failed TcpClient_writen_hal %d",tcp_handle));
            return 0;
        }
    }
    else
    {
        MO_ERROR(("Time out TcpClient_writen_hal %d",tcp_handle));
        return 0;
    }

}




/*==========================================================================================================================================
功能:
	查看tcp连接是否断开

参数:
	tcp句柄(tcp号)
	运行周期(如果在这个时间以内重复运行则直接返回最近一次真实的结果)


返回:
	1 连接
	0 断开
==========================================================================================================================================*/

unsigned char TcpClient_connected_hal(unsigned char handle)
{

  //pram check
  if(handle>4)
  {
    MO_ERROR(("bad pram"));
    return 0;
  }

  char temp[24];
  #define CMD "AT+CIPSTATUS"
  memset(temp,0x00,sizeof(temp));
  if(mo_drv_wifi_cmd_transfer(CMD,strlen(CMD),temp,sizeof(temp),10,0x03)<0)
  {
    MO_ERROR(("Failed TcpClient_connected_hal"));
    return 0;
  }
  else
  {
    //debug
    //MO_DEBUG(("mo_drv_wifi_cmd_transfer:{%s}",temp));

    //right data
    char temp_right[4];
    memset(temp_right,0x00,sizeof(temp_right));
    sprintf(temp_right,"%d1",handle);
    //MO_DEBUG(("temp_right:%s",temp_right));

    //cmp result
    if( strstr(temp,temp_right)!=NULL )
    {
      MO_INFO(("OK TcpClient_connected_hal %d",handle));
      return 1;
    }
    else
    {
      MO_ERROR(("NOT TcpClient_connected_hal %d",handle));
      return 0;
    }

  }

}


void TcpClient_stop_hal(unsigned char handle)
{
    //pram check
    if(handle>4)
    {
        MO_ERROR(("bad pram"));
        return;     //error
    }

    //send close cmd
    char temp_s[16];
    memset(temp_s,0x00,sizeof(temp_s));
    sprintf(temp_s,"AT+CIPCLOSE=%d",handle);

    char temp_r[4];
    memset(temp_r,0x00,sizeof(temp_r));

    if(mo_drv_wifi_cmd_transfer(temp_s,strlen(temp_s),temp_r,sizeof(temp_r),4,0x03)>0)
    {
        if(memcmp(temp_r,"OK",2)==0)
        {
           MO_INFO(("OK TcpClient_stop_hal %d",handle));
        }
        else
        {
           MO_ERROR(("Failed TcpClient_stop_hal %d",handle));
           // return;
        }
    }
    else
    {
        MO_ERROR(("Time out TcpClient_stop_hal %d",handle));
        //return;     //error
    }

    //destory fifo  recycle handle
    unsigned char  ret=TcpClient_close_hal(handle);
    if(ret!=MO_SUCCESS)
    {
        MO_ERROR(("Failed TcpClient_close_hal %d",handle));
        return;     //error
    }
    else
    {
        MO_INFO(("OK TcpClient_close_hal %d",handle));
    }

}

int mo_TcpClient_peek_hal(unsigned char *p_dat)
{
    MO_ERROR(("not support"));
    return 0;
}







