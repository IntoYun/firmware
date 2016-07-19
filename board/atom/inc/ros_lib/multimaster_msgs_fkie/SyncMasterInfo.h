#ifndef _ROS_multimaster_msgs_fkie_SyncMasterInfo_h
#define _ROS_multimaster_msgs_fkie_SyncMasterInfo_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "multimaster_msgs_fkie/SyncTopicInfo.h"
#include "multimaster_msgs_fkie/SyncServiceInfo.h"

namespace multimaster_msgs_fkie
{

  class SyncMasterInfo : public ros::Msg
  {
    public:
      const char* masteruri;
      uint8_t nodes_length;
      char* st_nodes;
      char* * nodes;
      uint8_t publisher_length;
      multimaster_msgs_fkie::SyncTopicInfo st_publisher;
      multimaster_msgs_fkie::SyncTopicInfo * publisher;
      uint8_t subscriber_length;
      multimaster_msgs_fkie::SyncTopicInfo st_subscriber;
      multimaster_msgs_fkie::SyncTopicInfo * subscriber;
      uint8_t services_length;
      multimaster_msgs_fkie::SyncServiceInfo st_services;
      multimaster_msgs_fkie::SyncServiceInfo * services;

    SyncMasterInfo():
      masteruri(""),
      nodes_length(0), nodes(NULL),
      publisher_length(0), publisher(NULL),
      subscriber_length(0), subscriber(NULL),
      services_length(0), services(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_masteruri = strlen(this->masteruri);
      memcpy(outbuffer + offset, &length_masteruri, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->masteruri, length_masteruri);
      offset += length_masteruri;
      *(outbuffer + offset++) = nodes_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < nodes_length; i++){
      uint32_t length_nodesi = strlen(this->nodes[i]);
      memcpy(outbuffer + offset, &length_nodesi, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->nodes[i], length_nodesi);
      offset += length_nodesi;
      }
      *(outbuffer + offset++) = publisher_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < publisher_length; i++){
      offset += this->publisher[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset++) = subscriber_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < subscriber_length; i++){
      offset += this->subscriber[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset++) = services_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < services_length; i++){
      offset += this->services[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_masteruri;
      memcpy(&length_masteruri, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_masteruri; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_masteruri-1]=0;
      this->masteruri = (char *)(inbuffer + offset-1);
      offset += length_masteruri;
      uint8_t nodes_lengthT = *(inbuffer + offset++);
      if(nodes_lengthT > nodes_length)
        this->nodes = (char**)realloc(this->nodes, nodes_lengthT * sizeof(char*));
      offset += 3;
      nodes_length = nodes_lengthT;
      for( uint8_t i = 0; i < nodes_length; i++){
      uint32_t length_st_nodes;
      memcpy(&length_st_nodes, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_nodes; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_nodes-1]=0;
      this->st_nodes = (char *)(inbuffer + offset-1);
      offset += length_st_nodes;
        memcpy( &(this->nodes[i]), &(this->st_nodes), sizeof(char*));
      }
      uint8_t publisher_lengthT = *(inbuffer + offset++);
      if(publisher_lengthT > publisher_length)
        this->publisher = (multimaster_msgs_fkie::SyncTopicInfo*)realloc(this->publisher, publisher_lengthT * sizeof(multimaster_msgs_fkie::SyncTopicInfo));
      offset += 3;
      publisher_length = publisher_lengthT;
      for( uint8_t i = 0; i < publisher_length; i++){
      offset += this->st_publisher.deserialize(inbuffer + offset);
        memcpy( &(this->publisher[i]), &(this->st_publisher), sizeof(multimaster_msgs_fkie::SyncTopicInfo));
      }
      uint8_t subscriber_lengthT = *(inbuffer + offset++);
      if(subscriber_lengthT > subscriber_length)
        this->subscriber = (multimaster_msgs_fkie::SyncTopicInfo*)realloc(this->subscriber, subscriber_lengthT * sizeof(multimaster_msgs_fkie::SyncTopicInfo));
      offset += 3;
      subscriber_length = subscriber_lengthT;
      for( uint8_t i = 0; i < subscriber_length; i++){
      offset += this->st_subscriber.deserialize(inbuffer + offset);
        memcpy( &(this->subscriber[i]), &(this->st_subscriber), sizeof(multimaster_msgs_fkie::SyncTopicInfo));
      }
      uint8_t services_lengthT = *(inbuffer + offset++);
      if(services_lengthT > services_length)
        this->services = (multimaster_msgs_fkie::SyncServiceInfo*)realloc(this->services, services_lengthT * sizeof(multimaster_msgs_fkie::SyncServiceInfo));
      offset += 3;
      services_length = services_lengthT;
      for( uint8_t i = 0; i < services_length; i++){
      offset += this->st_services.deserialize(inbuffer + offset);
        memcpy( &(this->services[i]), &(this->st_services), sizeof(multimaster_msgs_fkie::SyncServiceInfo));
      }
     return offset;
    }

    const char * getType(){ return "multimaster_msgs_fkie/SyncMasterInfo"; };
    const char * getMD5(){ return "ba4b02792114bcc4bae507c60dd14d66"; };

  };

}
#endif