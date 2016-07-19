#ifndef _ROS_ardrone_autonomy_navdata_kalman_pressure_h
#define _ROS_ardrone_autonomy_navdata_kalman_pressure_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace ardrone_autonomy
{

  class navdata_kalman_pressure : public ros::Msg
  {
    public:
      std_msgs::Header header;
      float drone_time;
      uint16_t tag;
      uint16_t size;
      float offset_pressure;
      float est_z;
      float est_zdot;
      float est_bias_PWM;
      float est_biais_pression;
      float offset_US;
      float prediction_US;
      float cov_alt;
      float cov_PWM;
      float cov_vitesse;
      int32_t bool_effet_sol;
      float somme_inno;
      int32_t flag_rejet_US;
      float u_multisinus;
      float gaz_altitude;
      int32_t Flag_multisinus;
      int32_t Flag_multisinus_debut;

    navdata_kalman_pressure():
      header(),
      drone_time(0),
      tag(0),
      size(0),
      offset_pressure(0),
      est_z(0),
      est_zdot(0),
      est_bias_PWM(0),
      est_biais_pression(0),
      offset_US(0),
      prediction_US(0),
      cov_alt(0),
      cov_PWM(0),
      cov_vitesse(0),
      bool_effet_sol(0),
      somme_inno(0),
      flag_rejet_US(0),
      u_multisinus(0),
      gaz_altitude(0),
      Flag_multisinus(0),
      Flag_multisinus_debut(0)
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
      } u_offset_pressure;
      u_offset_pressure.real = this->offset_pressure;
      *(outbuffer + offset + 0) = (u_offset_pressure.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_offset_pressure.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_offset_pressure.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_offset_pressure.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->offset_pressure);
      union {
        float real;
        uint32_t base;
      } u_est_z;
      u_est_z.real = this->est_z;
      *(outbuffer + offset + 0) = (u_est_z.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_est_z.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_est_z.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_est_z.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->est_z);
      union {
        float real;
        uint32_t base;
      } u_est_zdot;
      u_est_zdot.real = this->est_zdot;
      *(outbuffer + offset + 0) = (u_est_zdot.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_est_zdot.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_est_zdot.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_est_zdot.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->est_zdot);
      union {
        float real;
        uint32_t base;
      } u_est_bias_PWM;
      u_est_bias_PWM.real = this->est_bias_PWM;
      *(outbuffer + offset + 0) = (u_est_bias_PWM.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_est_bias_PWM.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_est_bias_PWM.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_est_bias_PWM.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->est_bias_PWM);
      union {
        float real;
        uint32_t base;
      } u_est_biais_pression;
      u_est_biais_pression.real = this->est_biais_pression;
      *(outbuffer + offset + 0) = (u_est_biais_pression.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_est_biais_pression.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_est_biais_pression.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_est_biais_pression.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->est_biais_pression);
      union {
        float real;
        uint32_t base;
      } u_offset_US;
      u_offset_US.real = this->offset_US;
      *(outbuffer + offset + 0) = (u_offset_US.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_offset_US.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_offset_US.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_offset_US.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->offset_US);
      union {
        float real;
        uint32_t base;
      } u_prediction_US;
      u_prediction_US.real = this->prediction_US;
      *(outbuffer + offset + 0) = (u_prediction_US.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_prediction_US.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_prediction_US.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_prediction_US.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->prediction_US);
      union {
        float real;
        uint32_t base;
      } u_cov_alt;
      u_cov_alt.real = this->cov_alt;
      *(outbuffer + offset + 0) = (u_cov_alt.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_cov_alt.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_cov_alt.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_cov_alt.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->cov_alt);
      union {
        float real;
        uint32_t base;
      } u_cov_PWM;
      u_cov_PWM.real = this->cov_PWM;
      *(outbuffer + offset + 0) = (u_cov_PWM.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_cov_PWM.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_cov_PWM.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_cov_PWM.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->cov_PWM);
      union {
        float real;
        uint32_t base;
      } u_cov_vitesse;
      u_cov_vitesse.real = this->cov_vitesse;
      *(outbuffer + offset + 0) = (u_cov_vitesse.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_cov_vitesse.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_cov_vitesse.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_cov_vitesse.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->cov_vitesse);
      union {
        int32_t real;
        uint32_t base;
      } u_bool_effet_sol;
      u_bool_effet_sol.real = this->bool_effet_sol;
      *(outbuffer + offset + 0) = (u_bool_effet_sol.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_bool_effet_sol.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_bool_effet_sol.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_bool_effet_sol.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->bool_effet_sol);
      union {
        float real;
        uint32_t base;
      } u_somme_inno;
      u_somme_inno.real = this->somme_inno;
      *(outbuffer + offset + 0) = (u_somme_inno.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_somme_inno.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_somme_inno.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_somme_inno.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->somme_inno);
      union {
        int32_t real;
        uint32_t base;
      } u_flag_rejet_US;
      u_flag_rejet_US.real = this->flag_rejet_US;
      *(outbuffer + offset + 0) = (u_flag_rejet_US.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_flag_rejet_US.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_flag_rejet_US.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_flag_rejet_US.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->flag_rejet_US);
      union {
        float real;
        uint32_t base;
      } u_u_multisinus;
      u_u_multisinus.real = this->u_multisinus;
      *(outbuffer + offset + 0) = (u_u_multisinus.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_u_multisinus.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_u_multisinus.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_u_multisinus.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->u_multisinus);
      union {
        float real;
        uint32_t base;
      } u_gaz_altitude;
      u_gaz_altitude.real = this->gaz_altitude;
      *(outbuffer + offset + 0) = (u_gaz_altitude.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_gaz_altitude.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_gaz_altitude.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_gaz_altitude.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->gaz_altitude);
      union {
        int32_t real;
        uint32_t base;
      } u_Flag_multisinus;
      u_Flag_multisinus.real = this->Flag_multisinus;
      *(outbuffer + offset + 0) = (u_Flag_multisinus.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_Flag_multisinus.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_Flag_multisinus.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_Flag_multisinus.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->Flag_multisinus);
      union {
        int32_t real;
        uint32_t base;
      } u_Flag_multisinus_debut;
      u_Flag_multisinus_debut.real = this->Flag_multisinus_debut;
      *(outbuffer + offset + 0) = (u_Flag_multisinus_debut.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_Flag_multisinus_debut.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_Flag_multisinus_debut.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_Flag_multisinus_debut.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->Flag_multisinus_debut);
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
      } u_offset_pressure;
      u_offset_pressure.base = 0;
      u_offset_pressure.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_offset_pressure.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_offset_pressure.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_offset_pressure.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->offset_pressure = u_offset_pressure.real;
      offset += sizeof(this->offset_pressure);
      union {
        float real;
        uint32_t base;
      } u_est_z;
      u_est_z.base = 0;
      u_est_z.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_est_z.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_est_z.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_est_z.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->est_z = u_est_z.real;
      offset += sizeof(this->est_z);
      union {
        float real;
        uint32_t base;
      } u_est_zdot;
      u_est_zdot.base = 0;
      u_est_zdot.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_est_zdot.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_est_zdot.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_est_zdot.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->est_zdot = u_est_zdot.real;
      offset += sizeof(this->est_zdot);
      union {
        float real;
        uint32_t base;
      } u_est_bias_PWM;
      u_est_bias_PWM.base = 0;
      u_est_bias_PWM.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_est_bias_PWM.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_est_bias_PWM.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_est_bias_PWM.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->est_bias_PWM = u_est_bias_PWM.real;
      offset += sizeof(this->est_bias_PWM);
      union {
        float real;
        uint32_t base;
      } u_est_biais_pression;
      u_est_biais_pression.base = 0;
      u_est_biais_pression.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_est_biais_pression.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_est_biais_pression.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_est_biais_pression.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->est_biais_pression = u_est_biais_pression.real;
      offset += sizeof(this->est_biais_pression);
      union {
        float real;
        uint32_t base;
      } u_offset_US;
      u_offset_US.base = 0;
      u_offset_US.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_offset_US.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_offset_US.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_offset_US.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->offset_US = u_offset_US.real;
      offset += sizeof(this->offset_US);
      union {
        float real;
        uint32_t base;
      } u_prediction_US;
      u_prediction_US.base = 0;
      u_prediction_US.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_prediction_US.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_prediction_US.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_prediction_US.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->prediction_US = u_prediction_US.real;
      offset += sizeof(this->prediction_US);
      union {
        float real;
        uint32_t base;
      } u_cov_alt;
      u_cov_alt.base = 0;
      u_cov_alt.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_cov_alt.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_cov_alt.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_cov_alt.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->cov_alt = u_cov_alt.real;
      offset += sizeof(this->cov_alt);
      union {
        float real;
        uint32_t base;
      } u_cov_PWM;
      u_cov_PWM.base = 0;
      u_cov_PWM.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_cov_PWM.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_cov_PWM.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_cov_PWM.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->cov_PWM = u_cov_PWM.real;
      offset += sizeof(this->cov_PWM);
      union {
        float real;
        uint32_t base;
      } u_cov_vitesse;
      u_cov_vitesse.base = 0;
      u_cov_vitesse.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_cov_vitesse.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_cov_vitesse.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_cov_vitesse.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->cov_vitesse = u_cov_vitesse.real;
      offset += sizeof(this->cov_vitesse);
      union {
        int32_t real;
        uint32_t base;
      } u_bool_effet_sol;
      u_bool_effet_sol.base = 0;
      u_bool_effet_sol.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_bool_effet_sol.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_bool_effet_sol.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_bool_effet_sol.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->bool_effet_sol = u_bool_effet_sol.real;
      offset += sizeof(this->bool_effet_sol);
      union {
        float real;
        uint32_t base;
      } u_somme_inno;
      u_somme_inno.base = 0;
      u_somme_inno.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_somme_inno.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_somme_inno.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_somme_inno.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->somme_inno = u_somme_inno.real;
      offset += sizeof(this->somme_inno);
      union {
        int32_t real;
        uint32_t base;
      } u_flag_rejet_US;
      u_flag_rejet_US.base = 0;
      u_flag_rejet_US.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_flag_rejet_US.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_flag_rejet_US.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_flag_rejet_US.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->flag_rejet_US = u_flag_rejet_US.real;
      offset += sizeof(this->flag_rejet_US);
      union {
        float real;
        uint32_t base;
      } u_u_multisinus;
      u_u_multisinus.base = 0;
      u_u_multisinus.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_u_multisinus.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_u_multisinus.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_u_multisinus.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->u_multisinus = u_u_multisinus.real;
      offset += sizeof(this->u_multisinus);
      union {
        float real;
        uint32_t base;
      } u_gaz_altitude;
      u_gaz_altitude.base = 0;
      u_gaz_altitude.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_gaz_altitude.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_gaz_altitude.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_gaz_altitude.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->gaz_altitude = u_gaz_altitude.real;
      offset += sizeof(this->gaz_altitude);
      union {
        int32_t real;
        uint32_t base;
      } u_Flag_multisinus;
      u_Flag_multisinus.base = 0;
      u_Flag_multisinus.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_Flag_multisinus.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_Flag_multisinus.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_Flag_multisinus.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->Flag_multisinus = u_Flag_multisinus.real;
      offset += sizeof(this->Flag_multisinus);
      union {
        int32_t real;
        uint32_t base;
      } u_Flag_multisinus_debut;
      u_Flag_multisinus_debut.base = 0;
      u_Flag_multisinus_debut.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_Flag_multisinus_debut.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_Flag_multisinus_debut.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_Flag_multisinus_debut.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->Flag_multisinus_debut = u_Flag_multisinus_debut.real;
      offset += sizeof(this->Flag_multisinus_debut);
     return offset;
    }

    const char * getType(){ return "ardrone_autonomy/navdata_kalman_pressure"; };
    const char * getMD5(){ return "70734b6caff0fb7ea6fc88ffeea5cde5"; };

  };

}
#endif