#ifndef _ROS_ardrone_autonomy_navdata_hdvideo_stream_h
#define _ROS_ardrone_autonomy_navdata_hdvideo_stream_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace ardrone_autonomy
{

  class navdata_hdvideo_stream : public ros::Msg
  {
    public:
      std_msgs::Header header;
      float drone_time;
      uint16_t tag;
      uint16_t size;
      uint32_t hdvideo_state;
      uint32_t storage_fifo_nb_packets;
      uint32_t storage_fifo_size;
      uint32_t usbkey_size;
      uint32_t usbkey_freespace;
      uint32_t frame_number;
      uint32_t usbkey_remaining_time;

    navdata_hdvideo_stream():
      header(),
      drone_time(0),
      tag(0),
      size(0),
      hdvideo_state(0),
      storage_fifo_nb_packets(0),
      storage_fifo_size(0),
      usbkey_size(0),
      usbkey_freespace(0),
      frame_number(0),
      usbkey_remaining_time(0)
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
      *(outbuffer + offset + 0) = (this->hdvideo_state >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->hdvideo_state >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->hdvideo_state >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->hdvideo_state >> (8 * 3)) & 0xFF;
      offset += sizeof(this->hdvideo_state);
      *(outbuffer + offset + 0) = (this->storage_fifo_nb_packets >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->storage_fifo_nb_packets >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->storage_fifo_nb_packets >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->storage_fifo_nb_packets >> (8 * 3)) & 0xFF;
      offset += sizeof(this->storage_fifo_nb_packets);
      *(outbuffer + offset + 0) = (this->storage_fifo_size >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->storage_fifo_size >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->storage_fifo_size >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->storage_fifo_size >> (8 * 3)) & 0xFF;
      offset += sizeof(this->storage_fifo_size);
      *(outbuffer + offset + 0) = (this->usbkey_size >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->usbkey_size >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->usbkey_size >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->usbkey_size >> (8 * 3)) & 0xFF;
      offset += sizeof(this->usbkey_size);
      *(outbuffer + offset + 0) = (this->usbkey_freespace >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->usbkey_freespace >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->usbkey_freespace >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->usbkey_freespace >> (8 * 3)) & 0xFF;
      offset += sizeof(this->usbkey_freespace);
      *(outbuffer + offset + 0) = (this->frame_number >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->frame_number >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->frame_number >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->frame_number >> (8 * 3)) & 0xFF;
      offset += sizeof(this->frame_number);
      *(outbuffer + offset + 0) = (this->usbkey_remaining_time >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->usbkey_remaining_time >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->usbkey_remaining_time >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->usbkey_remaining_time >> (8 * 3)) & 0xFF;
      offset += sizeof(this->usbkey_remaining_time);
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
      this->hdvideo_state =  ((uint32_t) (*(inbuffer + offset)));
      this->hdvideo_state |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->hdvideo_state |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->hdvideo_state |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->hdvideo_state);
      this->storage_fifo_nb_packets =  ((uint32_t) (*(inbuffer + offset)));
      this->storage_fifo_nb_packets |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->storage_fifo_nb_packets |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->storage_fifo_nb_packets |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->storage_fifo_nb_packets);
      this->storage_fifo_size =  ((uint32_t) (*(inbuffer + offset)));
      this->storage_fifo_size |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->storage_fifo_size |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->storage_fifo_size |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->storage_fifo_size);
      this->usbkey_size =  ((uint32_t) (*(inbuffer + offset)));
      this->usbkey_size |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->usbkey_size |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->usbkey_size |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->usbkey_size);
      this->usbkey_freespace =  ((uint32_t) (*(inbuffer + offset)));
      this->usbkey_freespace |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->usbkey_freespace |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->usbkey_freespace |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->usbkey_freespace);
      this->frame_number =  ((uint32_t) (*(inbuffer + offset)));
      this->frame_number |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->frame_number |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->frame_number |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->frame_number);
      this->usbkey_remaining_time =  ((uint32_t) (*(inbuffer + offset)));
      this->usbkey_remaining_time |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->usbkey_remaining_time |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->usbkey_remaining_time |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->usbkey_remaining_time);
     return offset;
    }

    const char * getType(){ return "ardrone_autonomy/navdata_hdvideo_stream"; };
    const char * getMD5(){ return "1ba321578916df95f899ca2f5348f234"; };

  };

}
#endif