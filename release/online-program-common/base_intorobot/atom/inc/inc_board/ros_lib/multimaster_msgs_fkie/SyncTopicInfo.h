#ifndef _ROS_multimaster_msgs_fkie_SyncTopicInfo_h
#define _ROS_multimaster_msgs_fkie_SyncTopicInfo_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace multimaster_msgs_fkie
{

  class SyncTopicInfo : public ros::Msg
  {
    public:
      const char* topic;
      const char* node;
      const char* nodeuri;

    SyncTopicInfo():
      topic(""),
      node(""),
      nodeuri("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_topic = strlen(this->topic);
      memcpy(outbuffer + offset, &length_topic, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->topic, length_topic);
      offset += length_topic;
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
      uint32_t length_topic;
      memcpy(&length_topic, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_topic; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_topic-1]=0;
      this->topic = (char *)(inbuffer + offset-1);
      offset += length_topic;
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

    const char * getType(){ return "multimaster_msgs_fkie/SyncTopicInfo"; };
    const char * getMD5(){ return "1b296160d6f5a9d7f6550f6130da2ba4"; };

  };

}
#endif