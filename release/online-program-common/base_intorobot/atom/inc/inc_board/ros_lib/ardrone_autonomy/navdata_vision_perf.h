#ifndef _ROS_ardrone_autonomy_navdata_vision_perf_h
#define _ROS_ardrone_autonomy_navdata_vision_perf_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace ardrone_autonomy
{

  class navdata_vision_perf : public ros::Msg
  {
    public:
      std_msgs::Header header;
      float drone_time;
      uint16_t tag;
      uint16_t size;
      float time_corners;
      float time_compute;
      float time_tracking;
      float time_trans;
      float time_update;
      uint8_t time_custom_length;
      float st_time_custom;
      float * time_custom;

    navdata_vision_perf():
      header(),
      drone_time(0),
      tag(0),
      size(0),
      time_corners(0),
      time_compute(0),
      time_tracking(0),
      time_trans(0),
      time_update(0),
      time_custom_length(0), time_custom(NULL)
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
      } u_time_corners;
      u_time_corners.real = this->time_corners;
      *(outbuffer + offset + 0) = (u_time_corners.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_time_corners.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_time_corners.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_time_corners.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_corners);
      union {
        float real;
        uint32_t base;
      } u_time_compute;
      u_time_compute.real = this->time_compute;
      *(outbuffer + offset + 0) = (u_time_compute.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_time_compute.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_time_compute.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_time_compute.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_compute);
      union {
        float real;
        uint32_t base;
      } u_time_tracking;
      u_time_tracking.real = this->time_tracking;
      *(outbuffer + offset + 0) = (u_time_tracking.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_time_tracking.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_time_tracking.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_time_tracking.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_tracking);
      union {
        float real;
        uint32_t base;
      } u_time_trans;
      u_time_trans.real = this->time_trans;
      *(outbuffer + offset + 0) = (u_time_trans.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_time_trans.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_time_trans.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_time_trans.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_trans);
      union {
        float real;
        uint32_t base;
      } u_time_update;
      u_time_update.real = this->time_update;
      *(outbuffer + offset + 0) = (u_time_update.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_time_update.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_time_update.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_time_update.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_update);
      *(outbuffer + offset++) = time_custom_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < time_custom_length; i++){
      union {
        float real;
        uint32_t base;
      } u_time_customi;
      u_time_customi.real = this->time_custom[i];
      *(outbuffer + offset + 0) = (u_time_customi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_time_customi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_time_customi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_time_customi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_custom[i]);
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
      union {
        float real;
        uint32_t base;
      } u_time_corners;
      u_time_corners.base = 0;
      u_time_corners.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_time_corners.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_time_corners.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_time_corners.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->time_corners = u_time_corners.real;
      offset += sizeof(this->time_corners);
      union {
        float real;
        uint32_t base;
      } u_time_compute;
      u_time_compute.base = 0;
      u_time_compute.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_time_compute.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_time_compute.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_time_compute.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->time_compute = u_time_compute.real;
      offset += sizeof(this->time_compute);
      union {
        float real;
        uint32_t base;
      } u_time_tracking;
      u_time_tracking.base = 0;
      u_time_tracking.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_time_tracking.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_time_tracking.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_time_tracking.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->time_tracking = u_time_tracking.real;
      offset += sizeof(this->time_tracking);
      union {
        float real;
        uint32_t base;
      } u_time_trans;
      u_time_trans.base = 0;
      u_time_trans.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_time_trans.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_time_trans.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_time_trans.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->time_trans = u_time_trans.real;
      offset += sizeof(this->time_trans);
      union {
        float real;
        uint32_t base;
      } u_time_update;
      u_time_update.base = 0;
      u_time_update.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_time_update.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_time_update.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_time_update.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->time_update = u_time_update.real;
      offset += sizeof(this->time_update);
      uint8_t time_custom_lengthT = *(inbuffer + offset++);
      if(time_custom_lengthT > time_custom_length)
        this->time_custom = (float*)realloc(this->time_custom, time_custom_lengthT * sizeof(float));
      offset += 3;
      time_custom_length = time_custom_lengthT;
      for( uint8_t i = 0; i < time_custom_length; i++){
      union {
        float real;
        uint32_t base;
      } u_st_time_custom;
      u_st_time_custom.base = 0;
      u_st_time_custom.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_time_custom.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_time_custom.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_time_custom.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_time_custom = u_st_time_custom.real;
      offset += sizeof(this->st_time_custom);
        memcpy( &(this->time_custom[i]), &(this->st_time_custom), sizeof(float));
      }
     return offset;
    }

    const char * getType(){ return "ardrone_autonomy/navdata_vision_perf"; };
    const char * getMD5(){ return "5ed8267a2e2980a430a3439af8e5c9f7"; };

  };

}
#endif