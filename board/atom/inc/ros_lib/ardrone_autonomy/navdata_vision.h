#ifndef _ROS_ardrone_autonomy_navdata_vision_h
#define _ROS_ardrone_autonomy_navdata_vision_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "ardrone_autonomy/vector31.h"

namespace ardrone_autonomy
{

  class navdata_vision : public ros::Msg
  {
    public:
      std_msgs::Header header;
      float drone_time;
      uint16_t tag;
      uint16_t size;
      uint32_t vision_state;
      int32_t vision_misc;
      float vision_phi_trim;
      float vision_phi_ref_prop;
      float vision_theta_trim;
      float vision_theta_ref_prop;
      int32_t new_raw_picture;
      float theta_capture;
      float phi_capture;
      float psi_capture;
      int32_t altitude_capture;
      uint32_t time_capture;
      ardrone_autonomy::vector31 body_v;
      float delta_phi;
      float delta_theta;
      float delta_psi;
      uint32_t gold_defined;
      uint32_t gold_reset;
      float gold_x;
      float gold_y;

    navdata_vision():
      header(),
      drone_time(0),
      tag(0),
      size(0),
      vision_state(0),
      vision_misc(0),
      vision_phi_trim(0),
      vision_phi_ref_prop(0),
      vision_theta_trim(0),
      vision_theta_ref_prop(0),
      new_raw_picture(0),
      theta_capture(0),
      phi_capture(0),
      psi_capture(0),
      altitude_capture(0),
      time_capture(0),
      body_v(),
      delta_phi(0),
      delta_theta(0),
      delta_psi(0),
      gold_defined(0),
      gold_reset(0),
      gold_x(0),
      gold_y(0)
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
      *(outbuffer + offset + 0) = (this->vision_state >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->vision_state >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->vision_state >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->vision_state >> (8 * 3)) & 0xFF;
      offset += sizeof(this->vision_state);
      union {
        int32_t real;
        uint32_t base;
      } u_vision_misc;
      u_vision_misc.real = this->vision_misc;
      *(outbuffer + offset + 0) = (u_vision_misc.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_vision_misc.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_vision_misc.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_vision_misc.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->vision_misc);
      union {
        float real;
        uint32_t base;
      } u_vision_phi_trim;
      u_vision_phi_trim.real = this->vision_phi_trim;
      *(outbuffer + offset + 0) = (u_vision_phi_trim.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_vision_phi_trim.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_vision_phi_trim.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_vision_phi_trim.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->vision_phi_trim);
      union {
        float real;
        uint32_t base;
      } u_vision_phi_ref_prop;
      u_vision_phi_ref_prop.real = this->vision_phi_ref_prop;
      *(outbuffer + offset + 0) = (u_vision_phi_ref_prop.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_vision_phi_ref_prop.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_vision_phi_ref_prop.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_vision_phi_ref_prop.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->vision_phi_ref_prop);
      union {
        float real;
        uint32_t base;
      } u_vision_theta_trim;
      u_vision_theta_trim.real = this->vision_theta_trim;
      *(outbuffer + offset + 0) = (u_vision_theta_trim.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_vision_theta_trim.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_vision_theta_trim.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_vision_theta_trim.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->vision_theta_trim);
      union {
        float real;
        uint32_t base;
      } u_vision_theta_ref_prop;
      u_vision_theta_ref_prop.real = this->vision_theta_ref_prop;
      *(outbuffer + offset + 0) = (u_vision_theta_ref_prop.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_vision_theta_ref_prop.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_vision_theta_ref_prop.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_vision_theta_ref_prop.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->vision_theta_ref_prop);
      union {
        int32_t real;
        uint32_t base;
      } u_new_raw_picture;
      u_new_raw_picture.real = this->new_raw_picture;
      *(outbuffer + offset + 0) = (u_new_raw_picture.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_new_raw_picture.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_new_raw_picture.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_new_raw_picture.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->new_raw_picture);
      union {
        float real;
        uint32_t base;
      } u_theta_capture;
      u_theta_capture.real = this->theta_capture;
      *(outbuffer + offset + 0) = (u_theta_capture.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_theta_capture.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_theta_capture.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_theta_capture.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->theta_capture);
      union {
        float real;
        uint32_t base;
      } u_phi_capture;
      u_phi_capture.real = this->phi_capture;
      *(outbuffer + offset + 0) = (u_phi_capture.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_phi_capture.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_phi_capture.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_phi_capture.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->phi_capture);
      union {
        float real;
        uint32_t base;
      } u_psi_capture;
      u_psi_capture.real = this->psi_capture;
      *(outbuffer + offset + 0) = (u_psi_capture.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_psi_capture.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_psi_capture.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_psi_capture.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->psi_capture);
      union {
        int32_t real;
        uint32_t base;
      } u_altitude_capture;
      u_altitude_capture.real = this->altitude_capture;
      *(outbuffer + offset + 0) = (u_altitude_capture.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_altitude_capture.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_altitude_capture.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_altitude_capture.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->altitude_capture);
      *(outbuffer + offset + 0) = (this->time_capture >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->time_capture >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->time_capture >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->time_capture >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_capture);
      offset += this->body_v.serialize(outbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_delta_phi;
      u_delta_phi.real = this->delta_phi;
      *(outbuffer + offset + 0) = (u_delta_phi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_delta_phi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_delta_phi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_delta_phi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->delta_phi);
      union {
        float real;
        uint32_t base;
      } u_delta_theta;
      u_delta_theta.real = this->delta_theta;
      *(outbuffer + offset + 0) = (u_delta_theta.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_delta_theta.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_delta_theta.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_delta_theta.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->delta_theta);
      union {
        float real;
        uint32_t base;
      } u_delta_psi;
      u_delta_psi.real = this->delta_psi;
      *(outbuffer + offset + 0) = (u_delta_psi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_delta_psi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_delta_psi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_delta_psi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->delta_psi);
      *(outbuffer + offset + 0) = (this->gold_defined >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->gold_defined >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->gold_defined >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->gold_defined >> (8 * 3)) & 0xFF;
      offset += sizeof(this->gold_defined);
      *(outbuffer + offset + 0) = (this->gold_reset >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->gold_reset >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->gold_reset >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->gold_reset >> (8 * 3)) & 0xFF;
      offset += sizeof(this->gold_reset);
      union {
        float real;
        uint32_t base;
      } u_gold_x;
      u_gold_x.real = this->gold_x;
      *(outbuffer + offset + 0) = (u_gold_x.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_gold_x.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_gold_x.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_gold_x.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->gold_x);
      union {
        float real;
        uint32_t base;
      } u_gold_y;
      u_gold_y.real = this->gold_y;
      *(outbuffer + offset + 0) = (u_gold_y.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_gold_y.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_gold_y.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_gold_y.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->gold_y);
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
      this->vision_state =  ((uint32_t) (*(inbuffer + offset)));
      this->vision_state |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->vision_state |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->vision_state |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->vision_state);
      union {
        int32_t real;
        uint32_t base;
      } u_vision_misc;
      u_vision_misc.base = 0;
      u_vision_misc.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_vision_misc.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_vision_misc.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_vision_misc.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->vision_misc = u_vision_misc.real;
      offset += sizeof(this->vision_misc);
      union {
        float real;
        uint32_t base;
      } u_vision_phi_trim;
      u_vision_phi_trim.base = 0;
      u_vision_phi_trim.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_vision_phi_trim.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_vision_phi_trim.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_vision_phi_trim.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->vision_phi_trim = u_vision_phi_trim.real;
      offset += sizeof(this->vision_phi_trim);
      union {
        float real;
        uint32_t base;
      } u_vision_phi_ref_prop;
      u_vision_phi_ref_prop.base = 0;
      u_vision_phi_ref_prop.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_vision_phi_ref_prop.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_vision_phi_ref_prop.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_vision_phi_ref_prop.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->vision_phi_ref_prop = u_vision_phi_ref_prop.real;
      offset += sizeof(this->vision_phi_ref_prop);
      union {
        float real;
        uint32_t base;
      } u_vision_theta_trim;
      u_vision_theta_trim.base = 0;
      u_vision_theta_trim.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_vision_theta_trim.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_vision_theta_trim.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_vision_theta_trim.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->vision_theta_trim = u_vision_theta_trim.real;
      offset += sizeof(this->vision_theta_trim);
      union {
        float real;
        uint32_t base;
      } u_vision_theta_ref_prop;
      u_vision_theta_ref_prop.base = 0;
      u_vision_theta_ref_prop.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_vision_theta_ref_prop.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_vision_theta_ref_prop.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_vision_theta_ref_prop.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->vision_theta_ref_prop = u_vision_theta_ref_prop.real;
      offset += sizeof(this->vision_theta_ref_prop);
      union {
        int32_t real;
        uint32_t base;
      } u_new_raw_picture;
      u_new_raw_picture.base = 0;
      u_new_raw_picture.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_new_raw_picture.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_new_raw_picture.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_new_raw_picture.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->new_raw_picture = u_new_raw_picture.real;
      offset += sizeof(this->new_raw_picture);
      union {
        float real;
        uint32_t base;
      } u_theta_capture;
      u_theta_capture.base = 0;
      u_theta_capture.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_theta_capture.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_theta_capture.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_theta_capture.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->theta_capture = u_theta_capture.real;
      offset += sizeof(this->theta_capture);
      union {
        float real;
        uint32_t base;
      } u_phi_capture;
      u_phi_capture.base = 0;
      u_phi_capture.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_phi_capture.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_phi_capture.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_phi_capture.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->phi_capture = u_phi_capture.real;
      offset += sizeof(this->phi_capture);
      union {
        float real;
        uint32_t base;
      } u_psi_capture;
      u_psi_capture.base = 0;
      u_psi_capture.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_psi_capture.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_psi_capture.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_psi_capture.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->psi_capture = u_psi_capture.real;
      offset += sizeof(this->psi_capture);
      union {
        int32_t real;
        uint32_t base;
      } u_altitude_capture;
      u_altitude_capture.base = 0;
      u_altitude_capture.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_altitude_capture.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_altitude_capture.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_altitude_capture.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->altitude_capture = u_altitude_capture.real;
      offset += sizeof(this->altitude_capture);
      this->time_capture =  ((uint32_t) (*(inbuffer + offset)));
      this->time_capture |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->time_capture |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->time_capture |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->time_capture);
      offset += this->body_v.deserialize(inbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_delta_phi;
      u_delta_phi.base = 0;
      u_delta_phi.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_delta_phi.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_delta_phi.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_delta_phi.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->delta_phi = u_delta_phi.real;
      offset += sizeof(this->delta_phi);
      union {
        float real;
        uint32_t base;
      } u_delta_theta;
      u_delta_theta.base = 0;
      u_delta_theta.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_delta_theta.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_delta_theta.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_delta_theta.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->delta_theta = u_delta_theta.real;
      offset += sizeof(this->delta_theta);
      union {
        float real;
        uint32_t base;
      } u_delta_psi;
      u_delta_psi.base = 0;
      u_delta_psi.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_delta_psi.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_delta_psi.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_delta_psi.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->delta_psi = u_delta_psi.real;
      offset += sizeof(this->delta_psi);
      this->gold_defined =  ((uint32_t) (*(inbuffer + offset)));
      this->gold_defined |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->gold_defined |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->gold_defined |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->gold_defined);
      this->gold_reset =  ((uint32_t) (*(inbuffer + offset)));
      this->gold_reset |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->gold_reset |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->gold_reset |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->gold_reset);
      union {
        float real;
        uint32_t base;
      } u_gold_x;
      u_gold_x.base = 0;
      u_gold_x.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_gold_x.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_gold_x.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_gold_x.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->gold_x = u_gold_x.real;
      offset += sizeof(this->gold_x);
      union {
        float real;
        uint32_t base;
      } u_gold_y;
      u_gold_y.base = 0;
      u_gold_y.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_gold_y.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_gold_y.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_gold_y.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->gold_y = u_gold_y.real;
      offset += sizeof(this->gold_y);
     return offset;
    }

    const char * getType(){ return "ardrone_autonomy/navdata_vision"; };
    const char * getMD5(){ return "55dd9a6c13917332d6d39025ed1318ab"; };

  };

}
#endif