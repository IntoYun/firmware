#ifndef _ROS_ardrone_autonomy_navdata_trims_h
#define _ROS_ardrone_autonomy_navdata_trims_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace ardrone_autonomy
{

  class navdata_trims : public ros::Msg
  {
    public:
      std_msgs::Header header;
      float drone_time;
      uint16_t tag;
      uint16_t size;
      float angular_rates_trim_r;
      float euler_angles_trim_theta;
      float euler_angles_trim_phi;

    navdata_trims():
      header(),
      drone_time(0),
      tag(0),
      size(0),
      angular_rates_trim_r(0),
      euler_angles_trim_theta(0),
      euler_angles_trim_phi(0)
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
      } u_angular_rates_trim_r;
      u_angular_rates_trim_r.real = this->angular_rates_trim_r;
      *(outbuffer + offset + 0) = (u_angular_rates_trim_r.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_angular_rates_trim_r.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_angular_rates_trim_r.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_angular_rates_trim_r.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->angular_rates_trim_r);
      union {
        float real;
        uint32_t base;
      } u_euler_angles_trim_theta;
      u_euler_angles_trim_theta.real = this->euler_angles_trim_theta;
      *(outbuffer + offset + 0) = (u_euler_angles_trim_theta.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_euler_angles_trim_theta.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_euler_angles_trim_theta.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_euler_angles_trim_theta.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->euler_angles_trim_theta);
      union {
        float real;
        uint32_t base;
      } u_euler_angles_trim_phi;
      u_euler_angles_trim_phi.real = this->euler_angles_trim_phi;
      *(outbuffer + offset + 0) = (u_euler_angles_trim_phi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_euler_angles_trim_phi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_euler_angles_trim_phi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_euler_angles_trim_phi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->euler_angles_trim_phi);
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
      } u_angular_rates_trim_r;
      u_angular_rates_trim_r.base = 0;
      u_angular_rates_trim_r.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_angular_rates_trim_r.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_angular_rates_trim_r.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_angular_rates_trim_r.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->angular_rates_trim_r = u_angular_rates_trim_r.real;
      offset += sizeof(this->angular_rates_trim_r);
      union {
        float real;
        uint32_t base;
      } u_euler_angles_trim_theta;
      u_euler_angles_trim_theta.base = 0;
      u_euler_angles_trim_theta.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_euler_angles_trim_theta.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_euler_angles_trim_theta.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_euler_angles_trim_theta.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->euler_angles_trim_theta = u_euler_angles_trim_theta.real;
      offset += sizeof(this->euler_angles_trim_theta);
      union {
        float real;
        uint32_t base;
      } u_euler_angles_trim_phi;
      u_euler_angles_trim_phi.base = 0;
      u_euler_angles_trim_phi.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_euler_angles_trim_phi.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_euler_angles_trim_phi.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_euler_angles_trim_phi.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->euler_angles_trim_phi = u_euler_angles_trim_phi.real;
      offset += sizeof(this->euler_angles_trim_phi);
     return offset;
    }

    const char * getType(){ return "ardrone_autonomy/navdata_trims"; };
    const char * getMD5(){ return "71133941ec04f0892b7754f7e7191d05"; };

  };

}
#endif