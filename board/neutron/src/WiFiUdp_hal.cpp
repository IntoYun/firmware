//=================================================================================================================
//input
/****************************************************************************
 *Private Included Files
 ****************************************************************************/

#include <stdint.h>
#include "lib_system_all.h"
#include "lib_wifi_drv.h"




 //=================================================================================================================
//come true hidden
/************************************************************************************
* Private Types
************************************************************************************/


/************************************************************************************
* Private Variables
************************************************************************************/


/************************************************************************************
* Private Functions
************************************************************************************/


//=================================================================================================================
//come true export
/************************************************************************************
* Public Type
************************************************************************************/



/************************************************************************************
*  Public Functions
************************************************************************************/


/*
成功 1
失败 0


AT+CIPSTART=0,"UDP","255.255.255.255",5557,5556,2

0,CONNECT
OK

DNS Fail
ERROR

*/





uint8_t WiFiUDP_begin_hal(uint8_t ahandle,uint16_t aport)
{
  char temp[64];
  snprintf(temp,sizeof(temp),"AT+CIPSTART=%d,\"UDP\",\"255.255.255.255\",5557,%d,2",ahandle,aport);

  if(mo_drv_wifi_run_cmd(temp,"OK",10))
  {
    MO_INFO(("OK WiFiUDP_begin_hal %d",ahandle));
  }
  else
  {
    MO_ERROR(("Failed WiFiUDP_begin_hal %d",ahandle));
  }

}




/*
OK 1

Failed 0


AT+CIPSEND=0,2,"192.168.8.1",5557



*/

uint8_t WiFiUDP_endPacket_hal(uint8_t handle, uint16_t len, char *p_dat)
{
  //check pram
  if( (handle>4)||(len==0)||(p_dat==NULL) )
  {
    MO_ERROR(("bad pram:%ud %ud %08x", handle, len, p_dat));
    return 0;
  }


  //send cmd
  char temp_w[64];
  char temp_r[32];
  memset(temp_w,0x00,sizeof(temp_w));
  snprintf(temp_w,sizeof(temp_w)-1,"AT+CIPSEND=%d,%d",handle,len);

  if(mo_drv_wifi_cmd_transfer((const char *)temp_w, strlen(temp_w),temp_r,sizeof(temp_r),10,0x01)>0)
  {
    if(memcmp(temp_r,">",1)==0)
    {
      MO_DEBUG((temp_r));
      //   MO_INFO(("GET >"));
      MO_INFO(("0.5> udp %d",handle));
    }
    else
    {
      MO_ERROR(("0.5Failed WiFiUDP_endPacket_hal %d",handle));
    }
  }
  else
  {
    MO_ERROR(("0.5Time out WiFiUDP_endPacket_hal %d",handle));
  }

  //send data
  int ret;
  // if( (ret=mo_drv_wifi_cmd_transfer((const char *)p_dat,len,temp_r,sizeof(temp_r),10,0x02)) >0)
  if( (ret=mo_drv_wifi_cmd_transfer((const char *)p_dat,len,temp_r,sizeof(temp_r),10,0x02)) >0)
  {
    if(memcmp(temp_r,"SEND OK",7)==0)
    {
      MO_DEBUG((temp_r));
      MO_INFO(("OK WiFiUDP_endPacket_hal %d",handle));
      //usleep(5000*1000);
      return ret;
    }
    else
    {
      MO_ERROR(("Failed WiFiUDP_endPacket_hal %d",handle));
      return 0;
    }
  }
  else
  {
    MO_ERROR(("Time out WiFiUDP_endPacket_hal %d",handle));
    return 0;
  }

}

int WiFiUDP_read_hal(unsigned char *buff, int size,unsigned char handle)
{
  //check pram
  if( (buff==NULL) || (size==0) ||(handle>4) )
  {
    MO_ERROR(("bad pram"));
    return 0;
  }

  return mo_drv_wifi_read_tcpc_fifo((char *)buff,size,handle);

}


void WiFiUDP_peek_hal(uint8_t *p)
{
  MO_ERROR(("No support"));
}

void WiFiUDP_remoteIP_hal()
{
  MO_ERROR(("No support"));
}


void WiFiUDP_remotePort_hal()
{
  MO_ERROR(("No support"));
}


//=================================================================================================================
//output
/****************************************************************************
*Public Included Files
****************************************************************************/
//=================================================================================================================




