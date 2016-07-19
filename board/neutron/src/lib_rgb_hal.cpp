//=================================================================================================================
//input
/****************************************************************************
 *Private Included Files
 ****************************************************************************/
#include "main.h"
#include "cmsis_os.h"

#include <stdint.h>
#include "lib_system_all.h"
#include "wiring.h"
#include "wiring_hal.h"
#include "wiring_flash_memory.h"
#include "device_config_hal.h"

//=================================================================================================================
//come true hidden
/************************************************************************************
* Private Types
************************************************************************************/

//#define DAT_FLASH_SAVE_TEST


typedef enum rgb_mode
{
	START_,
	OFF_,
	COLOR,
	BLINK,
	BREATH
} rgb_mode_t;



typedef enum key_mode
{
	MODE_NONE,
	MODE_CONFIG,
	MODE_DEFFW,
	MODE_COM,
	MODE_FAC,
	MODE_NC,
	MODE_RESET
} key_mode_t;


void mo_RGBClass_color_hal(uint8_t ared, uint8_t agreen, uint8_t ablue);
void mo_RGBClass_off_hal(void);


/************************************************************************************
* Private Variables
************************************************************************************/
static rgb_mode_t mode;
static uint8_t	red;
static uint8_t	gre;
static uint8_t	blu;
static uint16_t per;

static osThreadId handle_rgb;			//RGB主任务句柄

static int32_t value_now=1,value_old=1;
static uint32_t time_press=0,time_release=0,time_now=0;
static key_mode_t key_mode=MODE_NONE;
/************************************************************************************
* Private Functions
************************************************************************************/
void mo_key_manage()
{
	value_now=digitalRead(BT);
	if( (value_old==1)&&(value_now==0) )	//按下
	{
		time_press=mo_millis_hal();
	}

	if( (value_old==0)&&(value_now==1) )	//释放
	{
		if(key_mode==MODE_CONFIG)	//配置模式
		{
			MO_INFO(("press config mode"));
			intorobot_system_param.config_flag = !intorobot_system_param.config_flag;
            MO_DEBUG(("intorobot_system_param.config_flag: %d\r\n",intorobot_system_param.config_flag));
			saveSystemParams(&intorobot_system_param);
			mo_system_reboot_hal();		//重启
		}
		else if(key_mode==MODE_DEFFW)		//恢复灯程序
		{
			MO_INFO(("press DEFFW mode"));
			intorobot_boot_param.boot_flag=1;
			saveBootParams(&intorobot_boot_param);
			mo_system_reboot_hal();		//重启
		}
		else if(key_mode==MODE_COM)  //串口转发
		{
			MO_INFO(( "press COM mode"));
            intorobot_boot_param.boot_flag=2;
			saveBootParams(&intorobot_boot_param);
			delay(1000);
			mo_system_reboot_hal();		//重启
		}
		else if(key_mode==MODE_FAC)		//恢复出厂设置
		{
			MO_INFO(("press FAC mode"));
            intorobot_boot_param.boot_flag=3;
			saveBootParams(&intorobot_boot_param);
			mo_system_reboot_hal();		//重启
		}
        else if(key_mode==MODE_NC)		//不做操作
		{
			MO_INFO(("press NC mode"));
			mo_system_reboot_hal();		//重启
		}
        else if(key_mode==MODE_RESET)	//终极初始化
		{
			MO_INFO(("press RESET"));
            intorobot_boot_param.boot_flag=5;
			saveBootParams(&intorobot_boot_param);
			mo_system_reboot_hal();		//重启
		}
	}

	if(value_now==0)
	{
		time_now=mo_millis_hal();
		if( (time_now-time_press)>30000 )//恢复出厂设置  清除密钥
		{
			key_mode=MODE_RESET;
			mo_RGBClass_color_hal(1,1,0);	//黄灯打开
		}
        else if( (time_now-time_press)>20000 )//退出 exit mode
		{
			key_mode=MODE_NC;
			mo_RGBClass_color_hal(0,0,0);	//关灯
		}
		else if( (time_now-time_press)>13000 )//恢复出厂设置  不清除密钥
		{
			key_mode=MODE_FAC;
			mo_RGBClass_color_hal(0,1,1);	//浅蓝灯打开
		}
		else if( (time_now-time_press)>10000 )//进入串口转发程序
		{
			key_mode=MODE_COM;
			mo_RGBClass_color_hal(0,0,1);	//蓝灯打开
		}
		else if( (time_now-time_press)>7000 )	//恢复默认出厂程序
		{
			key_mode=MODE_DEFFW;
			mo_RGBClass_color_hal(0,1,0);	//绿灯打开
		}
        else if( (time_now-time_press)>3000 )		//配置模式
		{
			key_mode=MODE_CONFIG;
			mo_RGBClass_color_hal(1,0,0);	//红灯打开
		}
	}
	value_old=value_now;
}

void task_mo_rgb_hal(void const *argument)
{
	//恢复出厂设置按钮pinmode
	pinMode(BT,INPUT);

	pinMode(LR,OUTPUT);
	pinMode(LG,OUTPUT);
	pinMode(LB,OUTPUT);

	// digitalWrite(LR,1);
	// digitalWrite(LG,1);
	// digitalWrite(LB,1);
	// light blue color
	digitalWrite(LR, 1);
	digitalWrite(LG, 0);
	digitalWrite(LB, 0);

	mode = START_;

	while(1)
	{
		if(mode == START_)
		{
			digitalWrite(LR, 1);
			digitalWrite(LG, 0);
			digitalWrite(LB, 0);
		}
		else if(mode==OFF_)
		{
			digitalWrite(LR,1);
			digitalWrite(LG,1);
			digitalWrite(LB,1);
		}
		else if(mode==COLOR)
		{
			digitalWrite(LR,!red);
			digitalWrite(LG,!gre);
			digitalWrite(LB,!blu);
		}
		else if(mode==BLINK)
		{
			static uint32_t time_count;
			if(mo_timerIsEnd_hal(time_count, per>>1))
			{
				time_count=mo_millis_hal();

				static uint8_t flag=0;
				if( (flag++)%2 == 0 )
				{
					digitalWrite(LR,!red);
					digitalWrite(LG,!gre);
					digitalWrite(LB,!blu);
				}
				else
				{
					digitalWrite(LR,1);
					digitalWrite(LG,1);
					digitalWrite(LB,1);
				}
			}
		}
		else if(mode==BREATH)
		{
			MO_ERROR(("not support"));
		}
		else
		{
			MO_ERROR(("Error RGB mode"));
		}
		mo_key_manage();
		delay(100);
	}
}



//=================================================================================================================
//come true export
/************************************************************************************
* Public Type
************************************************************************************/



/************************************************************************************
*  Public Functions
************************************************************************************/




void mo_RGBClass_off_hal(void)
{
	mode=OFF_;
   // MO_ERROR(("not support"));
   // return 0;
}

void mo_RGBClass_color_hal(uint8_t ared, uint8_t agreen, uint8_t ablue)
{
	mode=COLOR;
	red=ared;
	gre=agreen;
	blu=ablue;
 // MO_ERROR(("test:%d",__LINE__));
 //   return 0;
}

void mo_RGBClass_blink_hal(uint8_t ared, uint8_t agreen, uint8_t ablue, uint16_t period)
{
	mode=BLINK;
	red=ared;
	gre=agreen;
	blu=ablue;
	per=period;
//MO_ERROR(("test:%d",__LINE__));
 //   MO_ERROR(("not support"));
 //   return 0;
}

void mo_RGBClass_breath_hal(uint8_t ared, uint8_t agreen, uint8_t ablue, uint16_t period)
{
	MO_ERROR(("not support"));
  //  return 0;
}




void mo_RGBClass_hal()
{
	//创建RGB 和按键 检测 任务
	osThreadDef(RGB_HAL,task_mo_rgb_hal, osPriorityNormal, 0, 1024);
	handle_rgb = osThreadCreate(osThread(RGB_HAL),NULL);
	MO_ASSERT((handle_rgb!=NULL));
}
