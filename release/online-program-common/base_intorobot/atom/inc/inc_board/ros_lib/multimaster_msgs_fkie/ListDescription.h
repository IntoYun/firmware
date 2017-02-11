#ifndef _ROS_SERVICE_ListDescription_h
#define _ROS_SERVICE_ListDescription_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "multimaster_msgs_fkie/Capability.h"

namespace multimaster_msgs_fkie
{

static const char LISTDESCRIPTION[] = "multimaster_msgs_fkie/ListDescription";

  class ListDescriptionRequest : public ros::Msg
  {
    public:

    ListDescriptionRequest()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
     return offset;
    }

    const char * getType(){ return LISTDESCRIPTION; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class ListDescriptionResponse : public ros::Msg
  {
    public:
      const char* robot_name;
      const char* robot_type;
      uint8_t robot_images_length;
      char* st_robot_images;
      char* * robot_images;
      const char* robot_descr;
      uint8_t capabilities_length;
      multimaster_msgs_fkie::Capability st_capabilities;
      multimaster_msgs_fkie::Capability * capabilities;

    ListDescriptionResponse():
      robot_name(""),
      robot_type(""),
      robot_images_length(0), robot_images(NULL),
      robot_descr(""),
      capabilities_length(0), capabilities(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_robot_name = strlen(this->robot_name);
      memcpy(outbuffer + offset, &length_robot_name, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->robot_name, length_robot_name);
      offset += length_robot_name;
      uint32_t length_robot_type = strlen(this->robot_type);
      memcpy(outbuffer + offset, &length_robot_type, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->robot_type, length_robot_type);
      offset += length_robot_type;
      *(outbuffer + offset++) = robot_images_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < robot_images_length; i++){
      uint32_t length_robot_imagesi = strlen(this->robot_images[i]);
      memcpy(outbuffer + offset, &length_robot_imagesi, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->robot_images[i], length_robot_imagesi);
      offset += length_robot_imagesi;
      }
      uint32_t length_robot_descr = strlen(this->robot_descr);
      memcpy(outbuffer + offset, &length_robot_descr, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->robot_descr, length_robot_descr);
      offset += length_robot_descr;
      *(outbuffer + offset++) = capabilities_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < capabilities_length; i++){
      offset += this->capabilities[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_robot_name;
      memcpy(&length_robot_name, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_robot_name; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_robot_name-1]=0;
      this->robot_name = (char *)(inbuffer + offset-1);
      offset += length_robot_name;
      uint32_t length_robot_type;
      memcpy(&length_robot_type, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_robot_type; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_robot_type-1]=0;
      this->robot_type = (char *)(inbuffer + offset-1);
      offset += length_robot_type;
      uint8_t robot_images_lengthT = *(inbuffer + offset++);
      if(robot_images_lengthT > robot_images_length)
        this->robot_images = (char**)realloc(this->robot_images, robot_images_lengthT * sizeof(char*));
      offset += 3;
      robot_images_length = robot_images_lengthT;
      for( uint8_t i = 0; i < robot_images_length; i++){
      uint32_t length_st_robot_images;
      memcpy(&length_st_robot_images, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_robot_images; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_robot_images-1]=0;
      this->st_robot_images = (char *)(inbuffer + offset-1);
      offset += length_st_robot_images;
        memcpy( &(this->robot_images[i]), &(this->st_robot_images), sizeof(char*));
      }
      uint32_t length_robot_descr;
      memcpy(&length_robot_descr, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_robot_descr; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_robot_descr-1]=0;
      this->robot_descr = (char *)(inbuffer + offset-1);
      offset += length_robot_descr;
      uint8_t capabilities_lengthT = *(inbuffer + offset++);
      if(capabilities_lengthT > capabilities_length)
        this->capabilities = (multimaster_msgs_fkie::Capability*)realloc(this->capabilities, capabilities_lengthT * sizeof(multimaster_msgs_fkie::Capability));
      offset += 3;
      capabilities_length = capabilities_lengthT;
      for( uint8_t i = 0; i < capabilities_length; i++){
      offset += this->st_capabilities.deserialize(inbuffer + offset);
        memcpy( &(this->capabilities[i]), &(this->st_capabilities), sizeof(multimaster_msgs_fkie::Capability));
      }
     return offset;
    }

    const char * getType(){ return LISTDESCRIPTION; };
    const char * getMD5(){ return "a36ba5a97ce785b9b964aa015b26332b"; };

  };

  class ListDescription {
    public:
    typedef ListDescriptionRequest Request;
    typedef ListDescriptionResponse Response;
  };

}
#endif
