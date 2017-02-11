#ifndef _ROS_multimaster_msgs_fkie_SyncServiceInfo_h
#define _ROS_multimaster_msgs_fkie_SyncServiceInfo_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace multimaster_msgs_fkie
{

  class SyncServiceInfo : public ros::Msg
  {
    public:
      const char* service;
      const char* serviceuri;
      const char* node;
      const char* nodeuri;

    SyncServiceInfo():
      service(""),
      serviceuri(""),
      node(""),
      nodeuri("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_service = strlen(this->service);
      memcpy(outbuffer + offset, &length_service, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->service, length_service);
      offset += length_service;
      uint32_t length_serviceuri = strlen(this->serviceuri);
      memcpy(outbuffer + offset, &length_serviceuri, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->serviceuri, length_serviceuri);
      offset += length_serviceuri;
      uint32_t length_node = strlen(this->node);
      memcpy(outbuffer + offset, &length_node, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->node, length_node);
      offset += length_node;
      uint32_t length_nodeuri = strlen(this->nodeuri);
      memcpy(outbuffer + offset, &length_nodeuri, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->nodeuri, length_nodeuri);
      offset += length_nodeuri;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_service;
      memcpy(&length_service, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_service; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_service-1]=0;
      this->service = (char *)(inbuffer + offset-1);
      offset += length_service;
      uint32_t length_serviceuri;
      memcpy(&length_serviceuri, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_serviceuri; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_serviceuri-1]=0;
      this->serviceuri = (char *)(inbuffer + offset-1);
      offset += length_serviceuri;
      uint32_t length_node;
      memcpy(&length_node, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_node; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_node-1]=0;
      this->node = (char *)(inbuffer + offset-1);
      offset += length_node;
      uint32_t length_nodeuri;
      memcpy(&length_nodeuri, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_nodeuri; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_nodeuri-1]=0;
      this->nodeuri = (char *)(inbuffer + offset-1);
      offset += length_nodeuri;
     return offset;
    }

    const char * getType(){ return "multimaster_msgs_fkie/SyncServiceInfo"; };
    const char * getMD5(){ return "8c21bb9ea24403924441840b8c167c40"; };

  };

}
#endif