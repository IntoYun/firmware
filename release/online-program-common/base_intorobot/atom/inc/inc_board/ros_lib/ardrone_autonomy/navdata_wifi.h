#ifndef _ROS_ardrone_autonomy_navdata_wifi_h
#define _ROS_ardrone_autonomy_navdata_wifi_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace ardrone_autonomy
{

  class navdata_wifi : public ros::Msg
  {
    public:
      std_msgs::Header header;
      float drone_time;
      uint16_t tag;
      uint16_t size;
      uint32_t link_quality;

    navdata_wifi():
      header(),
      drone_time(0),
      tag(0),
      size(0),
      link_quality(0)
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
      *(outbuffer + offset + 0) = (this->link_quality >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->link_quality >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->link_quality >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->link_quality >> (8 * 3)) & 0xFF;
      offset += sizeof(this->link_quality);
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
      this->link_quality =  ((uint32_t) (*(inbuffer + offset)));
      this->link_quality |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->link_quality |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->link_quality |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->link_quality);
     return offset;
    }

    const char * getType(){ return "ardrone_autonomy/navdata_wifi"; };
    const char * getMD5(){ return "f88a3241b15d1e7b20ce9d8673bac09a"; };

  };

}
#endif