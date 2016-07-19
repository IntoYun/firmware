#ifndef _ROS_ardrone_autonomy_navdata_adc_data_frame_h
#define _ROS_ardrone_autonomy_navdata_adc_data_frame_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace ardrone_autonomy
{

  class navdata_adc_data_frame : public ros::Msg
  {
    public:
      std_msgs::Header header;
      float drone_time;
      uint16_t tag;
      uint16_t size;
      uint32_t version;
      uint8_t data_frame_length;
      uint8_t st_data_frame;
      uint8_t * data_frame;

    navdata_adc_data_frame():
      header(),
      drone_time(0),
      tag(0),
      size(0),
      version(0),
      data_frame_length(0), data_frame(NULL)
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
      *(outbuffer + offset + 0) = (this->version >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->version >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->version >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->version >> (8 * 3)) & 0xFF;
      offset += sizeof(this->version);
      *(outbuffer + offset++) = data_frame_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < data_frame_length; i++){
      *(outbuffer + offset + 0) = (this->data_frame[i] >> (8 * 0)) & 0xFF;
      offset += sizeof(this->data_frame[i]);
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
      this->version =  ((uint32_t) (*(inbuffer + offset)));
      this->version |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->version |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->version |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->version);
      uint8_t data_frame_lengthT = *(inbuffer + offset++);
      if(data_frame_lengthT > data_frame_length)
        this->data_frame = (uint8_t*)realloc(this->data_frame, data_frame_lengthT * sizeof(uint8_t));
      offset += 3;
      data_frame_length = data_frame_lengthT;
      for( uint8_t i = 0; i < data_frame_length; i++){
      this->st_data_frame =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->st_data_frame);
        memcpy( &(this->data_frame[i]), &(this->st_data_frame), sizeof(uint8_t));
      }
     return offset;
    }

    const char * getType(){ return "ardrone_autonomy/navdata_adc_data_frame"; };
    const char * getMD5(){ return "403dbf67522d768c3f509d9614c23941"; };

  };

}
#endif