#ifndef _ROS_ardrone_autonomy_navdata_euler_angles_h
#define _ROS_ardrone_autonomy_navdata_euler_angles_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace ardrone_autonomy
{

  class navdata_euler_angles : public ros::Msg
  {
    public:
      std_msgs::Header header;
      float drone_time;
      uint16_t tag;
      uint16_t size;
      float theta_a;
      float phi_a;

    navdata_euler_angles():
      header(),
      drone_time(0),
      tag(0),
      size(0),
      theta_a(0),
      phi_a(0)
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
      union {
        float real;
        uint32_t base;
      } u_theta_a;
      u_theta_a.real = this->theta_a;
      *(outbuffer + offset + 0) = (u_theta_a.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_theta_a.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_theta_a.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_theta_a.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->theta_a);
      union {
        float real;
        uint32_t base;
      } u_phi_a;
      u_phi_a.real = this->phi_a;
      *(outbuffer + offset + 0) = (u_phi_a.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_phi_a.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_phi_a.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_phi_a.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->phi_a);
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
      union {
        float real;
        uint32_t base;
      } u_theta_a;
      u_theta_a.base = 0;
      u_theta_a.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_theta_a.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_theta_a.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_theta_a.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->theta_a = u_theta_a.real;
      offset += sizeof(this->theta_a);
      union {
        float real;
        uint32_t base;
      } u_phi_a;
      u_phi_a.base = 0;
      u_phi_a.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_phi_a.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_phi_a.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_phi_a.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->phi_a = u_phi_a.real;
      offset += sizeof(this->phi_a);
     return offset;
    }

    const char * getType(){ return "ardrone_autonomy/navdata_euler_angles"; };
    const char * getMD5(){ return "ce675f5dfa1b9c65f4844f2311d307aa"; };

  };

}
#endif