#ifndef _ROS_SERVICE_CamSelect_h
#define _ROS_SERVICE_CamSelect_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace ardrone_autonomy
{

static const char CAMSELECT[] = "ardrone_autonomy/CamSelect";

  class CamSelectRequest : public ros::Msg
  {
    public:
      uint8_t channel;

    CamSelectRequest():
      channel(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->channel >> (8 * 0)) & 0xFF;
      offset += sizeof(this->channel);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->channel =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->channel);
     return offset;
    }

    const char * getType(){ return CAMSELECT; };
    const char * getMD5(){ return "c27320df100593b008f1bb2e1302dbb6"; };

  };

  class CamSelectResponse : public ros::Msg
  {
    public:
      bool result;

    CamSelectResponse():
      result(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_result;
      u_result.real = this->result;
      *(outbuffer + offset + 0) = (u_result.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->result);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_result;
      u_result.base = 0;
      u_result.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->result = u_result.real;
      offset += sizeof(this->result);
     return offset;
    }

    const char * getType(){ return CAMSELECT; };
    const char * getMD5(){ return "eb13ac1f1354ccecb7941ee8fa2192e8"; };

  };

  class CamSelect {
    public:
    typedef CamSelectRequest Request;
    typedef CamSelectResponse Response;
  };

}
#endif
