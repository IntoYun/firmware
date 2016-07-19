#ifndef _ROS_multimaster_msgs_fkie_ROSMaster_h
#define _ROS_multimaster_msgs_fkie_ROSMaster_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace multimaster_msgs_fkie
{

  class ROSMaster : public ros::Msg
  {
    public:
      const char* name;
      const char* uri;
      float timestamp;
      float timestamp_local;
      bool online;
      const char* discoverer_name;
      const char* monitoruri;

    ROSMaster():
      name(""),
      uri(""),
      timestamp(0),
      timestamp_local(0),
      online(0),
      discoverer_name(""),
      monitoruri("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_name = strlen(this->name);
      memcpy(outbuffer + offset, &length_name, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->name, length_name);
      offset += length_name;
      uint32_t length_uri = strlen(this->uri);
      memcpy(outbuffer + offset, &length_uri, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->uri, length_uri);
      offset += length_uri;
      offset += serializeAvrFloat64(outbuffer + offset, this->timestamp);
      offset += serializeAvrFloat64(outbuffer + offset, this->timestamp_local);
      union {
        bool real;
        uint8_t base;
      } u_online;
      u_online.real = this->online;
      *(outbuffer + offset + 0) = (u_online.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->online);
      uint32_t length_discoverer_name = strlen(this->discoverer_name);
      memcpy(outbuffer + offset, &length_discoverer_name, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->discoverer_name, length_discoverer_name);
      offset += length_discoverer_name;
      uint32_t length_monitoruri = strlen(this->monitoruri);
      memcpy(outbuffer + offset, &length_monitoruri, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->monitoruri, length_monitoruri);
      offset += length_monitoruri;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_name;
      memcpy(&length_name, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_name; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_name-1]=0;
      this->name = (char *)(inbuffer + offset-1);
      offset += length_name;
      uint32_t length_uri;
      memcpy(&length_uri, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_uri; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_uri-1]=0;
      this->uri = (char *)(inbuffer + offset-1);
      offset += length_uri;
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->timestamp));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->timestamp_local));
      union {
        bool real;
        uint8_t base;
      } u_online;
      u_online.base = 0;
      u_online.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->online = u_online.real;
      offset += sizeof(this->online);
      uint32_t length_discoverer_name;
      memcpy(&length_discoverer_name, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_discoverer_name; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_discoverer_name-1]=0;
      this->discoverer_name = (char *)(inbuffer + offset-1);
      offset += length_discoverer_name;
      uint32_t length_monitoruri;
      memcpy(&length_monitoruri, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_monitoruri; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_monitoruri-1]=0;
      this->monitoruri = (char *)(inbuffer + offset-1);
      offset += length_monitoruri;
     return offset;
    }

    const char * getType(){ return "multimaster_msgs_fkie/ROSMaster"; };
    const char * getMD5(){ return "08ea76968a3be2b8b1c6550b39616f72"; };

  };

}
#endif