#ifndef _ROS_ardrone_autonomy_navdata_vision_of_h
#define _ROS_ardrone_autonomy_navdata_vision_of_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace ardrone_autonomy
{

  class navdata_vision_of : public ros::Msg
  {
    public:
      std_msgs::Header header;
      float drone_time;
      uint16_t tag;
      uint16_t size;
      uint8_t of_dx_length;
      float st_of_dx;
      float * of_dx;
      uint8_t of_dy_length;
      float st_of_dy;
      float * of_dy;

    navdata_vision_of():
      header(),
      drone_time(0),
      tag(0),
      size(0),
      of_dx_length(0), of_dx(NULL),
      of_dy_length(0), of_dy(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += serializeAvrFloat64(outbuffer + offset, this->drone_time);
      *(outbuffer + offset + 0) = (this->tag >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->tag >> (8 * 1)) & 0xFF;
      offset += sizeof(this->tag);
      *(outbuffer + offset + 0) = (this->size >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->size >> (8 * 1)) & 0xFF;
      offset += sizeof(this->size);
      *(outbuffer + offset++) = of_dx_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < of_dx_length; i++){
      union {
        float real;
        uint32_t base;
      } u_of_dxi;
      u_of_dxi.real = this->of_dx[i];
      *(outbuffer + offset + 0) = (u_of_dxi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_of_dxi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_of_dxi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_of_dxi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->of_dx[i]);
      }
      *(outbuffer + offset++) = of_dy_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < of_dy_length; i++){
      union {
        float real;
        uint32_t base;
      } u_of_dyi;
      u_of_dyi.real = this->of_dy[i];
      *(outbuffer + offset + 0) = (u_of_dyi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_of_dyi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_of_dyi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_of_dyi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->of_dy[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->drone_time));
      this->tag =  ((uint16_t) (*(inbuffer + offset)));
      this->tag |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->tag);
      this->size =  ((uint16_t) (*(inbuffer + offset)));
      this->size |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->size);
      uint8_t of_dx_lengthT = *(inbuffer + offset++);
      if(of_dx_lengthT > of_dx_length)
        this->of_dx = (float*)realloc(this->of_dx, of_dx_lengthT * sizeof(float));
      offset += 3;
      of_dx_length = of_dx_lengthT;
      for( uint8_t i = 0; i < of_dx_length; i++){
      union {
        float real;
        uint32_t base;
      } u_st_of_dx;
      u_st_of_dx.base = 0;
      u_st_of_dx.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_of_dx.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_of_dx.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_of_dx.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_of_dx = u_st_of_dx.real;
      offset += sizeof(this->st_of_dx);
        memcpy( &(this->of_dx[i]), &(this->st_of_dx), sizeof(float));
      }
      uint8_t of_dy_lengthT = *(inbuffer + offset++);
      if(of_dy_lengthT > of_dy_length)
        this->of_dy = (float*)realloc(this->of_dy, of_dy_lengthT * sizeof(float));
      offset += 3;
      of_dy_length = of_dy_lengthT;
      for( uint8_t i = 0; i < of_dy_length; i++){
      union {
        float real;
        uint32_t base;
      } u_st_of_dy;
      u_st_of_dy.base = 0;
      u_st_of_dy.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_of_dy.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_of_dy.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_of_dy.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_of_dy = u_st_of_dy.real;
      offset += sizeof(this->st_of_dy);
        memcpy( &(this->of_dy[i]), &(this->st_of_dy), sizeof(float));
      }
     return offset;
    }

    const char * getType(){ return "ardrone_autonomy/navdata_vision_of"; };
    const char * getMD5(){ return "76d31747173a842fbf71db03104edecd"; };

  };

}
#endif