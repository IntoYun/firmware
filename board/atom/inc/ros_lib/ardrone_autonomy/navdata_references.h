#ifndef _ROS_ardrone_autonomy_navdata_references_h
#define _ROS_ardrone_autonomy_navdata_references_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace ardrone_autonomy
{

  class navdata_references : public ros::Msg
  {
    public:
      std_msgs::Header header;
      float drone_time;
      uint16_t tag;
      uint16_t size;
      int32_t ref_theta;
      int32_t ref_phi;
      int32_t ref_theta_I;
      int32_t ref_phi_I;
      int32_t ref_pitch;
      int32_t ref_roll;
      int32_t ref_yaw;
      int32_t ref_psi;
      float vx_ref;
      float vy_ref;
      float theta_mod;
      float phi_mod;
      float k_v_x;
      float k_v_y;
      uint32_t k_mode;
      float ui_time;
      float ui_theta;
      float ui_phi;
      float ui_psi;
      float ui_psi_accuracy;
      int32_t ui_seq;

    navdata_references():
      header(),
      drone_time(0),
      tag(0),
      size(0),
      ref_theta(0),
      ref_phi(0),
      ref_theta_I(0),
      ref_phi_I(0),
      ref_pitch(0),
      ref_roll(0),
      ref_yaw(0),
      ref_psi(0),
      vx_ref(0),
      vy_ref(0),
      theta_mod(0),
      phi_mod(0),
      k_v_x(0),
      k_v_y(0),
      k_mode(0),
      ui_time(0),
      ui_theta(0),
      ui_phi(0),
      ui_psi(0),
      ui_psi_accuracy(0),
      ui_seq(0)
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
        int32_t real;
        uint32_t base;
      } u_ref_theta;
      u_ref_theta.real = this->ref_theta;
      *(outbuffer + offset + 0) = (u_ref_theta.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ref_theta.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ref_theta.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ref_theta.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ref_theta);
      union {
        int32_t real;
        uint32_t base;
      } u_ref_phi;
      u_ref_phi.real = this->ref_phi;
      *(outbuffer + offset + 0) = (u_ref_phi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ref_phi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ref_phi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ref_phi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ref_phi);
      union {
        int32_t real;
        uint32_t base;
      } u_ref_theta_I;
      u_ref_theta_I.real = this->ref_theta_I;
      *(outbuffer + offset + 0) = (u_ref_theta_I.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ref_theta_I.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ref_theta_I.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ref_theta_I.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ref_theta_I);
      union {
        int32_t real;
        uint32_t base;
      } u_ref_phi_I;
      u_ref_phi_I.real = this->ref_phi_I;
      *(outbuffer + offset + 0) = (u_ref_phi_I.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ref_phi_I.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ref_phi_I.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ref_phi_I.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ref_phi_I);
      union {
        int32_t real;
        uint32_t base;
      } u_ref_pitch;
      u_ref_pitch.real = this->ref_pitch;
      *(outbuffer + offset + 0) = (u_ref_pitch.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ref_pitch.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ref_pitch.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ref_pitch.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ref_pitch);
      union {
        int32_t real;
        uint32_t base;
      } u_ref_roll;
      u_ref_roll.real = this->ref_roll;
      *(outbuffer + offset + 0) = (u_ref_roll.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ref_roll.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ref_roll.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ref_roll.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ref_roll);
      union {
        int32_t real;
        uint32_t base;
      } u_ref_yaw;
      u_ref_yaw.real = this->ref_yaw;
      *(outbuffer + offset + 0) = (u_ref_yaw.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ref_yaw.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ref_yaw.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ref_yaw.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ref_yaw);
      union {
        int32_t real;
        uint32_t base;
      } u_ref_psi;
      u_ref_psi.real = this->ref_psi;
      *(outbuffer + offset + 0) = (u_ref_psi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ref_psi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ref_psi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ref_psi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ref_psi);
      union {
        float real;
        uint32_t base;
      } u_vx_ref;
      u_vx_ref.real = this->vx_ref;
      *(outbuffer + offset + 0) = (u_vx_ref.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_vx_ref.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_vx_ref.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_vx_ref.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->vx_ref);
      union {
        float real;
        uint32_t base;
      } u_vy_ref;
      u_vy_ref.real = this->vy_ref;
      *(outbuffer + offset + 0) = (u_vy_ref.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_vy_ref.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_vy_ref.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_vy_ref.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->vy_ref);
      union {
        float real;
        uint32_t base;
      } u_theta_mod;
      u_theta_mod.real = this->theta_mod;
      *(outbuffer + offset + 0) = (u_theta_mod.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_theta_mod.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_theta_mod.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_theta_mod.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->theta_mod);
      union {
        float real;
        uint32_t base;
      } u_phi_mod;
      u_phi_mod.real = this->phi_mod;
      *(outbuffer + offset + 0) = (u_phi_mod.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_phi_mod.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_phi_mod.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_phi_mod.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->phi_mod);
      union {
        float real;
        uint32_t base;
      } u_k_v_x;
      u_k_v_x.real = this->k_v_x;
      *(outbuffer + offset + 0) = (u_k_v_x.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_k_v_x.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_k_v_x.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_k_v_x.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->k_v_x);
      union {
        float real;
        uint32_t base;
      } u_k_v_y;
      u_k_v_y.real = this->k_v_y;
      *(outbuffer + offset + 0) = (u_k_v_y.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_k_v_y.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_k_v_y.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_k_v_y.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->k_v_y);
      *(outbuffer + offset + 0) = (this->k_mode >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->k_mode >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->k_mode >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->k_mode >> (8 * 3)) & 0xFF;
      offset += sizeof(this->k_mode);
      union {
        float real;
        uint32_t base;
      } u_ui_time;
      u_ui_time.real = this->ui_time;
      *(outbuffer + offset + 0) = (u_ui_time.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ui_time.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ui_time.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ui_time.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ui_time);
      union {
        float real;
        uint32_t base;
      } u_ui_theta;
      u_ui_theta.real = this->ui_theta;
      *(outbuffer + offset + 0) = (u_ui_theta.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ui_theta.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ui_theta.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ui_theta.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ui_theta);
      union {
        float real;
        uint32_t base;
      } u_ui_phi;
      u_ui_phi.real = this->ui_phi;
      *(outbuffer + offset + 0) = (u_ui_phi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ui_phi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ui_phi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ui_phi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ui_phi);
      union {
        float real;
        uint32_t base;
      } u_ui_psi;
      u_ui_psi.real = this->ui_psi;
      *(outbuffer + offset + 0) = (u_ui_psi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ui_psi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ui_psi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ui_psi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ui_psi);
      union {
        float real;
        uint32_t base;
      } u_ui_psi_accuracy;
      u_ui_psi_accuracy.real = this->ui_psi_accuracy;
      *(outbuffer + offset + 0) = (u_ui_psi_accuracy.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ui_psi_accuracy.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ui_psi_accuracy.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ui_psi_accuracy.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ui_psi_accuracy);
      union {
        int32_t real;
        uint32_t base;
      } u_ui_seq;
      u_ui_seq.real = this->ui_seq;
      *(outbuffer + offset + 0) = (u_ui_seq.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ui_seq.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ui_seq.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ui_seq.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ui_seq);
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
        int32_t real;
        uint32_t base;
      } u_ref_theta;
      u_ref_theta.base = 0;
      u_ref_theta.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ref_theta.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ref_theta.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ref_theta.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ref_theta = u_ref_theta.real;
      offset += sizeof(this->ref_theta);
      union {
        int32_t real;
        uint32_t base;
      } u_ref_phi;
      u_ref_phi.base = 0;
      u_ref_phi.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ref_phi.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ref_phi.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ref_phi.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ref_phi = u_ref_phi.real;
      offset += sizeof(this->ref_phi);
      union {
        int32_t real;
        uint32_t base;
      } u_ref_theta_I;
      u_ref_theta_I.base = 0;
      u_ref_theta_I.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ref_theta_I.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ref_theta_I.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ref_theta_I.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ref_theta_I = u_ref_theta_I.real;
      offset += sizeof(this->ref_theta_I);
      union {
        int32_t real;
        uint32_t base;
      } u_ref_phi_I;
      u_ref_phi_I.base = 0;
      u_ref_phi_I.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ref_phi_I.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ref_phi_I.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ref_phi_I.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ref_phi_I = u_ref_phi_I.real;
      offset += sizeof(this->ref_phi_I);
      union {
        int32_t real;
        uint32_t base;
      } u_ref_pitch;
      u_ref_pitch.base = 0;
      u_ref_pitch.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ref_pitch.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ref_pitch.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ref_pitch.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ref_pitch = u_ref_pitch.real;
      offset += sizeof(this->ref_pitch);
      union {
        int32_t real;
        uint32_t base;
      } u_ref_roll;
      u_ref_roll.base = 0;
      u_ref_roll.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ref_roll.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ref_roll.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ref_roll.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ref_roll = u_ref_roll.real;
      offset += sizeof(this->ref_roll);
      union {
        int32_t real;
        uint32_t base;
      } u_ref_yaw;
      u_ref_yaw.base = 0;
      u_ref_yaw.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ref_yaw.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ref_yaw.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ref_yaw.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ref_yaw = u_ref_yaw.real;
      offset += sizeof(this->ref_yaw);
      union {
        int32_t real;
        uint32_t base;
      } u_ref_psi;
      u_ref_psi.base = 0;
      u_ref_psi.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ref_psi.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ref_psi.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ref_psi.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ref_psi = u_ref_psi.real;
      offset += sizeof(this->ref_psi);
      union {
        float real;
        uint32_t base;
      } u_vx_ref;
      u_vx_ref.base = 0;
      u_vx_ref.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_vx_ref.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_vx_ref.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_vx_ref.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->vx_ref = u_vx_ref.real;
      offset += sizeof(this->vx_ref);
      union {
        float real;
        uint32_t base;
      } u_vy_ref;
      u_vy_ref.base = 0;
      u_vy_ref.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_vy_ref.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_vy_ref.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_vy_ref.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->vy_ref = u_vy_ref.real;
      offset += sizeof(this->vy_ref);
      union {
        float real;
        uint32_t base;
      } u_theta_mod;
      u_theta_mod.base = 0;
      u_theta_mod.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_theta_mod.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_theta_mod.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_theta_mod.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->theta_mod = u_theta_mod.real;
      offset += sizeof(this->theta_mod);
      union {
        float real;
        uint32_t base;
      } u_phi_mod;
      u_phi_mod.base = 0;
      u_phi_mod.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_phi_mod.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_phi_mod.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_phi_mod.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->phi_mod = u_phi_mod.real;
      offset += sizeof(this->phi_mod);
      union {
        float real;
        uint32_t base;
      } u_k_v_x;
      u_k_v_x.base = 0;
      u_k_v_x.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_k_v_x.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_k_v_x.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_k_v_x.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->k_v_x = u_k_v_x.real;
      offset += sizeof(this->k_v_x);
      union {
        float real;
        uint32_t base;
      } u_k_v_y;
      u_k_v_y.base = 0;
      u_k_v_y.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_k_v_y.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_k_v_y.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_k_v_y.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->k_v_y = u_k_v_y.real;
      offset += sizeof(this->k_v_y);
      this->k_mode =  ((uint32_t) (*(inbuffer + offset)));
      this->k_mode |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->k_mode |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->k_mode |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->k_mode);
      union {
        float real;
        uint32_t base;
      } u_ui_time;
      u_ui_time.base = 0;
      u_ui_time.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ui_time.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ui_time.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ui_time.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ui_time = u_ui_time.real;
      offset += sizeof(this->ui_time);
      union {
        float real;
        uint32_t base;
      } u_ui_theta;
      u_ui_theta.base = 0;
      u_ui_theta.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ui_theta.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ui_theta.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ui_theta.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ui_theta = u_ui_theta.real;
      offset += sizeof(this->ui_theta);
      union {
        float real;
        uint32_t base;
      } u_ui_phi;
      u_ui_phi.base = 0;
      u_ui_phi.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ui_phi.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ui_phi.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ui_phi.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ui_phi = u_ui_phi.real;
      offset += sizeof(this->ui_phi);
      union {
        float real;
        uint32_t base;
      } u_ui_psi;
      u_ui_psi.base = 0;
      u_ui_psi.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ui_psi.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ui_psi.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ui_psi.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ui_psi = u_ui_psi.real;
      offset += sizeof(this->ui_psi);
      union {
        float real;
        uint32_t base;
      } u_ui_psi_accuracy;
      u_ui_psi_accuracy.base = 0;
      u_ui_psi_accuracy.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ui_psi_accuracy.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ui_psi_accuracy.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ui_psi_accuracy.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ui_psi_accuracy = u_ui_psi_accuracy.real;
      offset += sizeof(this->ui_psi_accuracy);
      union {
        int32_t real;
        uint32_t base;
      } u_ui_seq;
      u_ui_seq.base = 0;
      u_ui_seq.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ui_seq.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ui_seq.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ui_seq.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ui_seq = u_ui_seq.real;
      offset += sizeof(this->ui_seq);
     return offset;
    }

    const char * getType(){ return "ardrone_autonomy/navdata_references"; };
    const char * getMD5(){ return "263b844b053f4a098c75c1c26a452911"; };

  };

}
#endif