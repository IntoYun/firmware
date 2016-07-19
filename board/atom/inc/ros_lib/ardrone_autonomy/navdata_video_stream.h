#ifndef _ROS_ardrone_autonomy_navdata_video_stream_h
#define _ROS_ardrone_autonomy_navdata_video_stream_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace ardrone_autonomy
{

  class navdata_video_stream : public ros::Msg
  {
    public:
      std_msgs::Header header;
      float drone_time;
      uint16_t tag;
      uint16_t size;
      uint8_t quant;
      uint32_t frame_size;
      uint32_t frame_number;
      uint32_t atcmd_ref_seq;
      uint32_t atcmd_mean_ref_gap;
      float atcmd_var_ref_gap;
      uint32_t atcmd_ref_quality;
      uint32_t desired_bitrate;
      int32_t data2;
      int32_t data3;
      int32_t data4;
      int32_t data5;
      uint32_t fifo_queue_level;

    navdata_video_stream():
      header(),
      drone_time(0),
      tag(0),
      size(0),
      quant(0),
      frame_size(0),
      frame_number(0),
      atcmd_ref_seq(0),
      atcmd_mean_ref_gap(0),
      atcmd_var_ref_gap(0),
      atcmd_ref_quality(0),
      desired_bitrate(0),
      data2(0),
      data3(0),
      data4(0),
      data5(0),
      fifo_queue_level(0)
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
      *(outbuffer + offset + 0) = (this->quant >> (8 * 0)) & 0xFF;
      offset += sizeof(this->quant);
      *(outbuffer + offset + 0) = (this->frame_size >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->frame_size >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->frame_size >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->frame_size >> (8 * 3)) & 0xFF;
      offset += sizeof(this->frame_size);
      *(outbuffer + offset + 0) = (this->frame_number >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->frame_number >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->frame_number >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->frame_number >> (8 * 3)) & 0xFF;
      offset += sizeof(this->frame_number);
      *(outbuffer + offset + 0) = (this->atcmd_ref_seq >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->atcmd_ref_seq >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->atcmd_ref_seq >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->atcmd_ref_seq >> (8 * 3)) & 0xFF;
      offset += sizeof(this->atcmd_ref_seq);
      *(outbuffer + offset + 0) = (this->atcmd_mean_ref_gap >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->atcmd_mean_ref_gap >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->atcmd_mean_ref_gap >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->atcmd_mean_ref_gap >> (8 * 3)) & 0xFF;
      offset += sizeof(this->atcmd_mean_ref_gap);
      union {
        float real;
        uint32_t base;
      } u_atcmd_var_ref_gap;
      u_atcmd_var_ref_gap.real = this->atcmd_var_ref_gap;
      *(outbuffer + offset + 0) = (u_atcmd_var_ref_gap.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_atcmd_var_ref_gap.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_atcmd_var_ref_gap.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_atcmd_var_ref_gap.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->atcmd_var_ref_gap);
      *(outbuffer + offset + 0) = (this->atcmd_ref_quality >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->atcmd_ref_quality >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->atcmd_ref_quality >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->atcmd_ref_quality >> (8 * 3)) & 0xFF;
      offset += sizeof(this->atcmd_ref_quality);
      *(outbuffer + offset + 0) = (this->desired_bitrate >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->desired_bitrate >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->desired_bitrate >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->desired_bitrate >> (8 * 3)) & 0xFF;
      offset += sizeof(this->desired_bitrate);
      union {
        int32_t real;
        uint32_t base;
      } u_data2;
      u_data2.real = this->data2;
      *(outbuffer + offset + 0) = (u_data2.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_data2.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_data2.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_data2.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->data2);
      union {
        int32_t real;
        uint32_t base;
      } u_data3;
      u_data3.real = this->data3;
      *(outbuffer + offset + 0) = (u_data3.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_data3.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_data3.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_data3.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->data3);
      union {
        int32_t real;
        uint32_t base;
      } u_data4;
      u_data4.real = this->data4;
      *(outbuffer + offset + 0) = (u_data4.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_data4.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_data4.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_data4.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->data4);
      union {
        int32_t real;
        uint32_t base;
      } u_data5;
      u_data5.real = this->data5;
      *(outbuffer + offset + 0) = (u_data5.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_data5.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_data5.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_data5.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->data5);
      *(outbuffer + offset + 0) = (this->fifo_queue_level >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->fifo_queue_level >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->fifo_queue_level >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->fifo_queue_level >> (8 * 3)) & 0xFF;
      offset += sizeof(this->fifo_queue_level);
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
      this->quant =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->quant);
      this->frame_size =  ((uint32_t) (*(inbuffer + offset)));
      this->frame_size |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->frame_size |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->frame_size |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->frame_size);
      this->frame_number =  ((uint32_t) (*(inbuffer + offset)));
      this->frame_number |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->frame_number |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->frame_number |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->frame_number);
      this->atcmd_ref_seq =  ((uint32_t) (*(inbuffer + offset)));
      this->atcmd_ref_seq |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->atcmd_ref_seq |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->atcmd_ref_seq |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->atcmd_ref_seq);
      this->atcmd_mean_ref_gap =  ((uint32_t) (*(inbuffer + offset)));
      this->atcmd_mean_ref_gap |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->atcmd_mean_ref_gap |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->atcmd_mean_ref_gap |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->atcmd_mean_ref_gap);
      union {
        float real;
        uint32_t base;
      } u_atcmd_var_ref_gap;
      u_atcmd_var_ref_gap.base = 0;
      u_atcmd_var_ref_gap.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_atcmd_var_ref_gap.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_atcmd_var_ref_gap.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_atcmd_var_ref_gap.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->atcmd_var_ref_gap = u_atcmd_var_ref_gap.real;
      offset += sizeof(this->atcmd_var_ref_gap);
      this->atcmd_ref_quality =  ((uint32_t) (*(inbuffer + offset)));
      this->atcmd_ref_quality |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->atcmd_ref_quality |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->atcmd_ref_quality |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->atcmd_ref_quality);
      this->desired_bitrate =  ((uint32_t) (*(inbuffer + offset)));
      this->desired_bitrate |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->desired_bitrate |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->desired_bitrate |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->desired_bitrate);
      union {
        int32_t real;
        uint32_t base;
      } u_data2;
      u_data2.base = 0;
      u_data2.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_data2.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_data2.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_data2.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->data2 = u_data2.real;
      offset += sizeof(this->data2);
      union {
        int32_t real;
        uint32_t base;
      } u_data3;
      u_data3.base = 0;
      u_data3.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_data3.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_data3.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_data3.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->data3 = u_data3.real;
      offset += sizeof(this->data3);
      union {
        int32_t real;
        uint32_t base;
      } u_data4;
      u_data4.base = 0;
      u_data4.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_data4.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_data4.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_data4.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->data4 = u_data4.real;
      offset += sizeof(this->data4);
      union {
        int32_t real;
        uint32_t base;
      } u_data5;
      u_data5.base = 0;
      u_data5.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_data5.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_data5.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_data5.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->data5 = u_data5.real;
      offset += sizeof(this->data5);
      this->fifo_queue_level =  ((uint32_t) (*(inbuffer + offset)));
      this->fifo_queue_level |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->fifo_queue_level |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->fifo_queue_level |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->fifo_queue_level);
     return offset;
    }

    const char * getType(){ return "ardrone_autonomy/navdata_video_stream"; };
    const char * getMD5(){ return "caf560356d435eb563ce5b9e8278da00"; };

  };

}
#endif