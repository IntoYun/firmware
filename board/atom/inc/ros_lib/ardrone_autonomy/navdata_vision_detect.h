#ifndef _ROS_ardrone_autonomy_navdata_vision_detect_h
#define _ROS_ardrone_autonomy_navdata_vision_detect_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "ardrone_autonomy/matrix33.h"
#include "ardrone_autonomy/vector31.h"

namespace ardrone_autonomy
{

  class navdata_vision_detect : public ros::Msg
  {
    public:
      std_msgs::Header header;
      float drone_time;
      uint16_t tag;
      uint16_t size;
      uint32_t nb_detected;
      uint8_t type_length;
      uint32_t st_type;
      uint32_t * type;
      uint8_t xc_length;
      uint32_t st_xc;
      uint32_t * xc;
      uint8_t yc_length;
      uint32_t st_yc;
      uint32_t * yc;
      uint8_t width_length;
      uint32_t st_width;
      uint32_t * width;
      uint8_t height_length;
      uint32_t st_height;
      uint32_t * height;
      uint8_t dist_length;
      uint32_t st_dist;
      uint32_t * dist;
      uint8_t orientation_angle_length;
      float st_orientation_angle;
      float * orientation_angle;
      uint8_t rotation_length;
      ardrone_autonomy::matrix33 st_rotation;
      ardrone_autonomy::matrix33 * rotation;
      uint8_t translation_length;
      ardrone_autonomy::vector31 st_translation;
      ardrone_autonomy::vector31 * translation;
      uint8_t camera_source_length;
      uint32_t st_camera_source;
      uint32_t * camera_source;

    navdata_vision_detect():
      header(),
      drone_time(0),
      tag(0),
      size(0),
      nb_detected(0),
      type_length(0), type(NULL),
      xc_length(0), xc(NULL),
      yc_length(0), yc(NULL),
      width_length(0), width(NULL),
      height_length(0), height(NULL),
      dist_length(0), dist(NULL),
      orientation_angle_length(0), orientation_angle(NULL),
      rotation_length(0), rotation(NULL),
      translation_length(0), translation(NULL),
      camera_source_length(0), camera_source(NULL)
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
      *(outbuffer + offset + 0) = (this->nb_detected >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->nb_detected >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->nb_detected >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->nb_detected >> (8 * 3)) & 0xFF;
      offset += sizeof(this->nb_detected);
      *(outbuffer + offset++) = type_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < type_length; i++){
      *(outbuffer + offset + 0) = (this->type[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->type[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->type[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->type[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->type[i]);
      }
      *(outbuffer + offset++) = xc_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < xc_length; i++){
      *(outbuffer + offset + 0) = (this->xc[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->xc[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->xc[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->xc[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->xc[i]);
      }
      *(outbuffer + offset++) = yc_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < yc_length; i++){
      *(outbuffer + offset + 0) = (this->yc[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->yc[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->yc[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->yc[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->yc[i]);
      }
      *(outbuffer + offset++) = width_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < width_length; i++){
      *(outbuffer + offset + 0) = (this->width[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->width[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->width[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->width[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->width[i]);
      }
      *(outbuffer + offset++) = height_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < height_length; i++){
      *(outbuffer + offset + 0) = (this->height[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->height[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->height[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->height[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->height[i]);
      }
      *(outbuffer + offset++) = dist_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < dist_length; i++){
      *(outbuffer + offset + 0) = (this->dist[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->dist[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->dist[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->dist[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->dist[i]);
      }
      *(outbuffer + offset++) = orientation_angle_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < orientation_angle_length; i++){
      union {
        float real;
        uint32_t base;
      } u_orientation_anglei;
      u_orientation_anglei.real = this->orientation_angle[i];
      *(outbuffer + offset + 0) = (u_orientation_anglei.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_orientation_anglei.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_orientation_anglei.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_orientation_anglei.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->orientation_angle[i]);
      }
      *(outbuffer + offset++) = rotation_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < rotation_length; i++){
      offset += this->rotation[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset++) = translation_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < translation_length; i++){
      offset += this->translation[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset++) = camera_source_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < camera_source_length; i++){
      *(outbuffer + offset + 0) = (this->camera_source[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->camera_source[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->camera_source[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->camera_source[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->camera_source[i]);
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
      this->nb_detected =  ((uint32_t) (*(inbuffer + offset)));
      this->nb_detected |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->nb_detected |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->nb_detected |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->nb_detected);
      uint8_t type_lengthT = *(inbuffer + offset++);
      if(type_lengthT > type_length)
        this->type = (uint32_t*)realloc(this->type, type_lengthT * sizeof(uint32_t));
      offset += 3;
      type_length = type_lengthT;
      for( uint8_t i = 0; i < type_length; i++){
      this->st_type =  ((uint32_t) (*(inbuffer + offset)));
      this->st_type |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->st_type |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->st_type |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->st_type);
        memcpy( &(this->type[i]), &(this->st_type), sizeof(uint32_t));
      }
      uint8_t xc_lengthT = *(inbuffer + offset++);
      if(xc_lengthT > xc_length)
        this->xc = (uint32_t*)realloc(this->xc, xc_lengthT * sizeof(uint32_t));
      offset += 3;
      xc_length = xc_lengthT;
      for( uint8_t i = 0; i < xc_length; i++){
      this->st_xc =  ((uint32_t) (*(inbuffer + offset)));
      this->st_xc |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->st_xc |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->st_xc |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->st_xc);
        memcpy( &(this->xc[i]), &(this->st_xc), sizeof(uint32_t));
      }
      uint8_t yc_lengthT = *(inbuffer + offset++);
      if(yc_lengthT > yc_length)
        this->yc = (uint32_t*)realloc(this->yc, yc_lengthT * sizeof(uint32_t));
      offset += 3;
      yc_length = yc_lengthT;
      for( uint8_t i = 0; i < yc_length; i++){
      this->st_yc =  ((uint32_t) (*(inbuffer + offset)));
      this->st_yc |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->st_yc |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->st_yc |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->st_yc);
        memcpy( &(this->yc[i]), &(this->st_yc), sizeof(uint32_t));
      }
      uint8_t width_lengthT = *(inbuffer + offset++);
      if(width_lengthT > width_length)
        this->width = (uint32_t*)realloc(this->width, width_lengthT * sizeof(uint32_t));
      offset += 3;
      width_length = width_lengthT;
      for( uint8_t i = 0; i < width_length; i++){
      this->st_width =  ((uint32_t) (*(inbuffer + offset)));
      this->st_width |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->st_width |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->st_width |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->st_width);
        memcpy( &(this->width[i]), &(this->st_width), sizeof(uint32_t));
      }
      uint8_t height_lengthT = *(inbuffer + offset++);
      if(height_lengthT > height_length)
        this->height = (uint32_t*)realloc(this->height, height_lengthT * sizeof(uint32_t));
      offset += 3;
      height_length = height_lengthT;
      for( uint8_t i = 0; i < height_length; i++){
      this->st_height =  ((uint32_t) (*(inbuffer + offset)));
      this->st_height |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->st_height |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->st_height |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->st_height);
        memcpy( &(this->height[i]), &(this->st_height), sizeof(uint32_t));
      }
      uint8_t dist_lengthT = *(inbuffer + offset++);
      if(dist_lengthT > dist_length)
        this->dist = (uint32_t*)realloc(this->dist, dist_lengthT * sizeof(uint32_t));
      offset += 3;
      dist_length = dist_lengthT;
      for( uint8_t i = 0; i < dist_length; i++){
      this->st_dist =  ((uint32_t) (*(inbuffer + offset)));
      this->st_dist |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->st_dist |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->st_dist |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->st_dist);
        memcpy( &(this->dist[i]), &(this->st_dist), sizeof(uint32_t));
      }
      uint8_t orientation_angle_lengthT = *(inbuffer + offset++);
      if(orientation_angle_lengthT > orientation_angle_length)
        this->orientation_angle = (float*)realloc(this->orientation_angle, orientation_angle_lengthT * sizeof(float));
      offset += 3;
      orientation_angle_length = orientation_angle_lengthT;
      for( uint8_t i = 0; i < orientation_angle_length; i++){
      union {
        float real;
        uint32_t base;
      } u_st_orientation_angle;
      u_st_orientation_angle.base = 0;
      u_st_orientation_angle.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_orientation_angle.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_orientation_angle.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_orientation_angle.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_orientation_angle = u_st_orientation_angle.real;
      offset += sizeof(this->st_orientation_angle);
        memcpy( &(this->orientation_angle[i]), &(this->st_orientation_angle), sizeof(float));
      }
      uint8_t rotation_lengthT = *(inbuffer + offset++);
      if(rotation_lengthT > rotation_length)
        this->rotation = (ardrone_autonomy::matrix33*)realloc(this->rotation, rotation_lengthT * sizeof(ardrone_autonomy::matrix33));
      offset += 3;
      rotation_length = rotation_lengthT;
      for( uint8_t i = 0; i < rotation_length; i++){
      offset += this->st_rotation.deserialize(inbuffer + offset);
        memcpy( &(this->rotation[i]), &(this->st_rotation), sizeof(ardrone_autonomy::matrix33));
      }
      uint8_t translation_lengthT = *(inbuffer + offset++);
      if(translation_lengthT > translation_length)
        this->translation = (ardrone_autonomy::vector31*)realloc(this->translation, translation_lengthT * sizeof(ardrone_autonomy::vector31));
      offset += 3;
      translation_length = translation_lengthT;
      for( uint8_t i = 0; i < translation_length; i++){
      offset += this->st_translation.deserialize(inbuffer + offset);
        memcpy( &(this->translation[i]), &(this->st_translation), sizeof(ardrone_autonomy::vector31));
      }
      uint8_t camera_source_lengthT = *(inbuffer + offset++);
      if(camera_source_lengthT > camera_source_length)
        this->camera_source = (uint32_t*)realloc(this->camera_source, camera_source_lengthT * sizeof(uint32_t));
      offset += 3;
      camera_source_length = camera_source_lengthT;
      for( uint8_t i = 0; i < camera_source_length; i++){
      this->st_camera_source =  ((uint32_t) (*(inbuffer + offset)));
      this->st_camera_source |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->st_camera_source |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->st_camera_source |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->st_camera_source);
        memcpy( &(this->camera_source[i]), &(this->st_camera_source), sizeof(uint32_t));
      }
     return offset;
    }

    const char * getType(){ return "ardrone_autonomy/navdata_vision_detect"; };
    const char * getMD5(){ return "ab5f6b8c66aead58358d45a7c2dce7f6"; };

  };

}
#endif