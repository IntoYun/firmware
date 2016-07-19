#ifndef _ROS_SERVICE_LedAnim_h
#define _ROS_SERVICE_LedAnim_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace ardrone_autonomy
{

static const char LEDANIM[] = "ardrone_autonomy/LedAnim";

  class LedAnimRequest : public ros::Msg
  {
    public:
      uint8_t type;
      float freq;
      uint8_t duration;

    LedAnimRequest():
      type(0),
      freq(0),
      duration(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->type);
      union {
        float real;
        uint32_t base;
      } u_freq;
      u_freq.real = this->freq;
      *(outbuffer + offset + 0) = (u_freq.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_freq.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_freq.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_freq.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->freq);
      *(outbuffer + offset + 0) = (this->duration >> (8 * 0)) & 0xFF;
      offset += sizeof(this->duration);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->type);
      union {
        float real;
        uint32_t base;
      } u_freq;
      u_freq.base = 0;
      u_freq.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_freq.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_freq.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_freq.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->freq = u_freq.real;
      offset += sizeof(this->freq);
      this->duration =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->duration);
     return offset;
    }

    const char * getType(){ return LEDANIM; };
    const char * getMD5(){ return "23392fc8200b12a3585ff6a32d597821"; };

  };

  class LedAnimResponse : public ros::Msg
  {
    public:
      bool result;

    LedAnimResponse():
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

    const char * getType(){ return LEDANIM; };
    const char * getMD5(){ return "eb13ac1f1354ccecb7941ee8fa2192e8"; };

  };

  class LedAnim {
    public:
    typedef LedAnimRequest Request;
    typedef LedAnimResponse Response;
  };

}
#endif
