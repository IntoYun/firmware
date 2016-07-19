#ifndef _ROS_multimaster_msgs_fkie_LinkState_h
#define _ROS_multimaster_msgs_fkie_LinkState_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace multimaster_msgs_fkie
{

  class LinkState : public ros::Msg
  {
    public:
      const char* destination;
      float quality;

    LinkState():
      destination(""),
      quality(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_destination = strlen(this->destination);
      memcpy(outbuffer + offset, &length_destination, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->destination, length_destination);
      offset += length_destination;
      union {
        float real;
        uint32_t base;
      } u_quality;
      u_quality.real = this->quality;
      *(outbuffer + offset + 0) = (u_quality.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_quality.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_quality.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_quality.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->quality);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_destination;
      memcpy(&length_destination, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_destination; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_destination-1]=0;
      this->destination = (char *)(inbuffer + offset-1);
      offset += length_destination;
      union {
        float real;
        uint32_t base;
      } u_quality;
      u_quality.base = 0;
      u_quality.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_quality.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_quality.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_quality.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->quality = u_quality.real;
      offset += sizeof(this->quality);
     return offset;
    }

    const char * getType(){ return "multimaster_msgs_fkie/LinkState"; };
    const char * getMD5(){ return "d3fe0aab52336c0bd0bfea55b131c66e"; };

  };

}
#endif