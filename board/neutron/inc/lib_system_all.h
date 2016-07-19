/*
此文件包含:

	所有公共配置信息
	打印函数


*/

#ifndef __LIB_MO_SYSTEM_H
#define __LIB_MO_SYSTEM_H


//MO_CONSOLE dep
#include "application.h"


#define WIFI_HARDWARE_ENABLE          //不去连接8266  (1.不创建过滤任务 2.不命令初始化8266 3.不去连接平台)
//#define PRAM_BURN_ENABLE              //写入默认参数  只需要执行一次


//#define MO_CONSOLE SerialUSB  // 打印从USB输出
//#define MO_CONSOLE Serial     //打印从串口2输出




#define INTOROBOT_WLAN_ENABLE     //连接平台
#define INTOROBOT_CONFIG_ENABLE   //配置



/*
print config

*/
//#define MO_INFO_EN
//#define MO_ERROR_EN
//#define MO_DEBUG_EN
//#define MO_ASSERT_EN

/* #define MO_PRINT_EN */
/* #define MO_PRINTN_EN */


/*
return value

*/

#define MO_SUCCESS 0
#define MO_FAILED (-1)






#ifdef MO_INFO_EN

#define MO_INFO(msg)  \
	do{ \
		MO_CONSOLE.printf("{I}"); \
		MO_CONSOLE.printf msg; \
		MO_CONSOLE.printf("[%s %d] \n",__FUNCTION__,__LINE__);\
	}while(0)

#else

#define MO_INFO(ignore) 	((void)0)

#endif


#ifdef MO_PRINT_EN

#define MO_PRINT(msg)  \
	do{ \
		MO_CONSOLE.printf msg; \
	}while(0)

#else

#define MO_PRINT(ignore) 	((void)0)

#endif


#ifdef MO_PRINTN_EN

static void print_n(const char *p,int len)
{
    MO_CONSOLE.printf("{");
    while(len--) MO_CONSOLE.print(*(p++));
    MO_CONSOLE.printf("}\n");
}

#define MO_PRINTN(msg)  \
	do{ \
		print_n msg; \
	}while(0)

#else

#define MO_PRINTN(ignore) 	((void)0)

#endif





#ifdef MO_ERROR_EN

#define MO_ERROR(err_info)  \
	do{ \
		MO_CONSOLE.printf("\033[1;40;31m{E}");\
		MO_CONSOLE.printf err_info; \
		MO_CONSOLE.printf("[%s %d] \033[0m \n",__FUNCTION__,__LINE__);\
	}while(0)

#else

#define MO_ERROR(ignore) 	((void)0)

#endif






#ifdef MO_DEBUG_EN

#define MO_DEBUG(dbg_info) \
	do{ \
		MO_CONSOLE.printf("{D}"); \
		MO_CONSOLE.printf dbg_info; \
		MO_CONSOLE.printf("[%s %d] \n",__FUNCTION__,__LINE__);\
	}while(0)

#else

#define MO_DEBUG(ignore) 	((void)0)

#endif



#ifdef MO_ASSERT_EN

#define MO_ASSERT(e) \
	do{ \
		((e) ? (void)0 : (void)(MO_CONSOLE.printf("\033[1;40;31m{S}assertion failed: %s [func %s, line %d] \033[0m \r\n", #e, __FUNCTION__, __LINE__))); \
	}while(0)

#else

#define MO_ASSERT(ignore) 	((void)0)

#endif









#endif


