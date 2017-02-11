#ifndef _ROS_ardrone_autonomy_navdata_magneto_h
#define _ROS_ardrone_autonomy_navdata_magneto_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "ardrone_autonomy/vector31.h"

namespace ardrone_autonomy
{

  class navdata_magneto : public ros::Msg
  {
    public:
      std_msgs::Header header;
      float drone_time;
      uint16_t tag;
      uint16_t size;
      int16_t mx;
      int16_t my;
      int16_t mz;
      ardrone_autonomy::vector31 magneto_raw;
      ardrone_autonomy::vector31 magneto_rectified;
      ardrone_autonomy::vector31 magneto_offset;
      float heading_unwrapped;
      float heading_gyro_unwrapped;
      float heading_fusion_unwrapped;
      uint8_t magneto_calibration_ok;
      uint32_t magneto_state;
      float magneto_radius;
      float error_mean;
      float error_var;

    navdata_magneto():
      header(),
      drone_time(0),
      tag(0),
      size(0),
      mx(0),
      my(0),
      mz(0),
      magneto_raw(),
      magneto_rectified(),
      magneto_offset(),
      heading_unwrapped(0),
      heading_gyro_unwrapped(0),
      heading_fusion_unwrapped(0),
      magneto_calibration_ok(0),
      magneto_state(0),
      magneto_radius(0),
      error_mean(0),
      error_var(0)
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
        int16_t real;
        uint16_t base;
      } u_mx;
      u_mx.real = this->mx;
      *(outbuffer + offset + 0) = (u_mx.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_mx.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->mx);
      union {
        int16_t real;
        uint16_t base;
      } u_my;
      u_my.real = this->my;
      *(outbuffer + offset + 0) = (u_my.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_my.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->my);
      union {
        int16_t real;
        uint16_t base;
      } u_mz;
      u_mz.real = this->mz;
      *(outbuffer + offset + 0) = (u_mz.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_mz.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->mz);
      offset += this->magneto_raw.serialize(outbuffer + offset);
      offset += this->magneto_rectified.serialize(outbuffer + offset);
      offset += this->magneto_offset.serialize(outbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_heading_unwrapped;
      u_heading_unwrapped.real = this->heading_unwrapped;
      *(outbuffer + offset + 0) = (u_heading_unwrapped.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_heading_unwrapped.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_heading_unwrapped.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_heading_unwrapped.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->heading_unwrapped);
      union {
        float real;
        uint32_t base;
      } u_heading_gyro_unwrapped;
      u_heading_gyro_unwrapped.real = this->heading_gyro_unwrapped;
      *(outbuffer + offset + 0) = (u_heading_gyro_unwrapped.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_heading_gyro_unwrapped.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_heading_gyro_unwrapped.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_heading_gyro_unwrapped.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->heading_gyro_unwrapped);
      union {
        float real;
        uint32_t base;
      } u_heading_fusion_unwrapped;
      u_heading_fusion_unwrapped.real = this->heading_fusion_unwrapped;
      *(outbuffer + offset + 0) = (u_heading_fusion_unwrapped.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_heading_fusion_unwrapped.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_heading_fusion_unwrapped.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_heading_fusion_unwrapped.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->heading_fusion_unwrapped);
      *(outbuffer + offset + 0) = (this->magneto_calibration_ok >> (8 * 0)) & 0xFF;
      offset += sizeof(this->magneto_calibration_ok);
      *(outbuffer + offset + 0) = (this->magneto_state >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->magneto_state >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->magneto_state >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->magneto_state >> (8 * 3)) & 0xFF;
      offset += sizeof(this->magneto_state);
      union {
        float real;
        uint32_t base;
      } u_magneto_radius;
      u_magneto_radius.real = this->magneto_radius;
      *(outbuffer + offset + 0) = (u_magneto_radius.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_magneto_radius.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_magneto_radius.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_magneto_radius.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->magneto_radius);
      union {
        float real;
        uint32_t base;
      } u_error_mean;
      u_error_mean.real = this->error_mean;
      *(outbuffer + offset + 0) = (u_error_mean.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_error_mean.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_error_mean.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_error_mean.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->error_mean);
      union {
        float real;
        uint32_t base;
      } u_error_var;
      u_error_var.real = this->error_var;
      *(outbuffer + offset + 0) = (u_error_var.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_error_var.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_error_var.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_error_var.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->error_var);
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
        int16_t real;
        uint16_t base;
      } u_mx;
      u_mx.base = 0;
      u_mx.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_mx.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->mx = u_mx.real;
      offset += sizeof(this->mx);
      union {
        int16_t real;
        uint16_t base;
      } u_my;
      u_my.base = 0;
      u_my.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_my.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->my = u_my.real;
      offset += sizeof(this->my);
      union {
        int16_t real;
        uint16_t base;
      } u_mz;
      u_mz.base = 0;
      u_mz.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_mz.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->mz = u_mz.real;
      offset += sizeof(this->mz);
      offset += this->magneto_raw.deserialize(inbuffer + offset);
      offset += this->magneto_rectified.deserialize(inbuffer + offset);
      offset += this->magneto_offset.deserialize(inbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_heading_unwrapped;
      u_heading_unwrapped.base = 0;
      u_heading_unwrapped.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_heading_unwrapped.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_heading_unwrapped.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_heading_unwrapped.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->heading_unwrapped = u_heading_unwrapped.real;
      offset += sizeof(this->heading_unwrapped);
      union {
        float real;
        uint32_t base;
      } u_heading_gyro_unwrapped;
      u_heading_gyro_unwrapped.base = 0;
      u_heading_gyro_unwrapped.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_heading_gyro_unwrapped.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_heading_gyro_unwrapped.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_heading_gyro_unwrapped.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->heading_gyro_unwrapped = u_heading_gyro_unwrapped.real;
      offset += sizeof(this->heading_gyro_unwrapped);
      union {
        float real;
        uint32_t base;
      } u_heading_fusion_unwrapped;
      u_heading_fusion_unwrapped.base = 0;
      u_heading_fusion_unwrapped.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_heading_fusion_unwrapped.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_heading_fusion_unwrapped.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_heading_fusion_unwrapped.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->heading_fusion_unwrapped = u_heading_fusion_unwrapped.real;
      offset += sizeof(this->heading_fusion_unwrapped);
      this->magneto_calibration_ok =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->magneto_calibration_ok);
      this->magneto_state =  ((uint32_t) (*(inbuffer + offset)));
      this->magneto_state |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->magneto_state |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->magneto_state |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->magneto_state);
      union {
        float real;
        uint32_t base;
      } u_magneto_radius;
      u_magneto_radius.base = 0;
      u_magneto_radius.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_magneto_radius.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_magneto_radius.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_magneto_radius.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->magneto_radius = u_magneto_radius.real;
      offset += sizeof(this->magneto_radius);
      union {
        float real;
        uint32_t base;
      } u_error_mean;
      u_error_mean.base = 0;
      u_error_mean.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_error_mean.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_error_mean.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_error_mean.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->error_mean = u_error_mean.real;
      offset += sizeof(this->error_mean);
      union {
        float real;
        uint32_t base;
      } u_error_var;
      u_error_var.base = 0;
      u_error_var.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_error_var.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_error_var.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_error_var.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->error_var = u_error_var.real;
      offset += sizeof(this->error_var);
     return offset;
    }

    const char * getType(){ return "ardrone_autonomy/navdata_magneto"; };
    const char * getMD5(){ return "c256b1c1d1ff0cb12a13c36720b3e224"; };

  };

}
#endif