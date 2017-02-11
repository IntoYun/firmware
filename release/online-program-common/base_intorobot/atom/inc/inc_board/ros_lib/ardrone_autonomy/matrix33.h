#ifndef _ROS_ardrone_autonomy_matrix33_h
#define _ROS_ardrone_autonomy_matrix33_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace ardrone_autonomy
{

  class matrix33 : public ros::Msg
  {
    public:
      float m11;
      float m12;
      float m13;
      float m21;
      float m22;
      float m23;
      float m31;
      float m32;
      float m33;

    matrix33():
      m11(0),
      m12(0),
      m13(0),
      m21(0),
      m22(0),
      m23(0),
      m31(0),
      m32(0),
      m33(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_m11;
      u_m11.real = this->m11;
      *(outbuffer + offset + 0) = (u_m11.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m11.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m11.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m11.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m11);
      union {
        float real;
        uint32_t base;
      } u_m12;
      u_m12.real = this->m12;
      *(outbuffer + offset + 0) = (u_m12.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m12.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m12.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m12.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m12);
      union {
        float real;
        uint32_t base;
      } u_m13;
      u_m13.real = this->m13;
      *(outbuffer + offset + 0) = (u_m13.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m13.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m13.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m13.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m13);
      union {
        float real;
        uint32_t base;
      } u_m21;
      u_m21.real = this->m21;
      *(outbuffer + offset + 0) = (u_m21.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m21.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m21.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m21.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m21);
      union {
        float real;
        uint32_t base;
      } u_m22;
      u_m22.real = this->m22;
      *(outbuffer + offset + 0) = (u_m22.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m22.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m22.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m22.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m22);
      union {
        float real;
        uint32_t base;
      } u_m23;
      u_m23.real = this->m23;
      *(outbuffer + offset + 0) = (u_m23.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m23.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m23.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m23.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m23);
      union {
        float real;
        uint32_t base;
      } u_m31;
      u_m31.real = this->m31;
      *(outbuffer + offset + 0) = (u_m31.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m31.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m31.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m31.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m31);
      union {
        float real;
        uint32_t base;
      } u_m32;
      u_m32.real = this->m32;
      *(outbuffer + offset + 0) = (u_m32.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m32.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m32.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m32.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m32);
      union {
        float real;
        uint32_t base;
      } u_m33;
      u_m33.real = this->m33;
      *(outbuffer + offset + 0) = (u_m33.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m33.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m33.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m33.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m33);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_m11;
      u_m11.base = 0;
      u_m11.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m11.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m11.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m11.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m11 = u_m11.real;
      offset += sizeof(this->m11);
      union {
        float real;
        uint32_t base;
      } u_m12;
      u_m12.base = 0;
      u_m12.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m12.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m12.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m12.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m12 = u_m12.real;
      offset += sizeof(this->m12);
      union {
        float real;
        uint32_t base;
      } u_m13;
      u_m13.base = 0;
      u_m13.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m13.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m13.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m13.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m13 = u_m13.real;
      offset += sizeof(this->m13);
      union {
        float real;
        uint32_t base;
      } u_m21;
      u_m21.base = 0;
      u_m21.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m21.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m21.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m21.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m21 = u_m21.real;
      offset += sizeof(this->m21);
      union {
        float real;
        uint32_t base;
      } u_m22;
      u_m22.base = 0;
      u_m22.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m22.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m22.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m22.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m22 = u_m22.real;
      offset += sizeof(this->m22);
      union {
        float real;
        uint32_t base;
      } u_m23;
      u_m23.base = 0;
      u_m23.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m23.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m23.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m23.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m23 = u_m23.real;
      offset += sizeof(this->m23);
      union {
        float real;
        uint32_t base;
      } u_m31;
      u_m31.base = 0;
      u_m31.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m31.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m31.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m31.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m31 = u_m31.real;
      offset += sizeof(this->m31);
      union {
        float real;
        uint32_t base;
      } u_m32;
      u_m32.base = 0;
      u_m32.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m32.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m32.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m32.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m32 = u_m32.real;
      offset += sizeof(this->m32);
      union {
        float real;
        uint32_t base;
      } u_m33;
      u_m33.base = 0;
      u_m33.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m33.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m33.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m33.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m33 = u_m33.real;
      offset += sizeof(this->m33);
     return offset;
    }

    const char * getType(){ return "ardrone_autonomy/matrix33"; };
    const char * getMD5(){ return "d6abfd8481fcb7c359a8d5d9388fa257"; };

  };

}
#endif