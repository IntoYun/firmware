#ifndef _ROS_SERVICE_FlightAnim_h
#define _ROS_SERVICE_FlightAnim_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace ardrone_autonomy
{

static const char FLIGHTANIM[] = "ardrone_autonomy/FlightAnim";

  class FlightAnimRequest : public ros::Msg
  {
    public:
      uint8_t type;
      uint32_t duration;

    FlightAnimRequest():
      type(0),
      duration(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->type);
      *(outbuffer + offset + 0) = (this->duration >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->duration >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->duration >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->duration >> (8 * 3)) & 0xFF;
      offset += sizeof(this->duration);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->type);
      this->duration =  ((uint32_t) (*(inbuffer + offset)));
      this->duration |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->duration |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->duration |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->duration);
     return offset;
    }

    const char * getType(){ return FLIGHTANIM; };
    const char * getMD5(){ return "1babbb93af7ae39105f8dba705eb91ac"; };

  };

  class FlightAnimResponse : public ros::Msg
  {
    public:
      bool result;

    FlightAnimResponse():
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

    const char * getType(){ return FLIGHTANIM; };
    const char * getMD5(){ return "eb13ac1f1354ccecb7941ee8fa2192e8"; };

  };

  class FlightAnim {
    public:
    typedef FlightAnimRequest Request;
    typedef FlightAnimResponse Response;
  };

}
#endif
