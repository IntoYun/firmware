#ifndef _ROS_SERVICE_LoadLaunch_h
#define _ROS_SERVICE_LoadLaunch_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace multimaster_msgs_fkie
{

static const char LOADLAUNCH[] = "multimaster_msgs_fkie/LoadLaunch";

  class LoadLaunchRequest : public ros::Msg
  {
    public:
      const char* package;
      const char* file;
      const char* argv;

    LoadLaunchRequest():
      package(""),
      file(""),
      argv("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_package = strlen(this->package);
      memcpy(outbuffer + offset, &length_package, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->package, length_package);
      offset += length_package;
      uint32_t length_file = strlen(this->file);
      memcpy(outbuffer + offset, &length_file, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->file, length_file);
      offset += length_file;
      uint32_t length_argv = strlen(this->argv);
      memcpy(outbuffer + offset, &length_argv, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->argv, length_argv);
      offset += length_argv;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_package;
      memcpy(&length_package, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_package; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_package-1]=0;
      this->package = (char *)(inbuffer + offset-1);
      offset += length_package;
      uint32_t length_file;
      memcpy(&length_file, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_file; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_file-1]=0;
      this->file = (char *)(inbuffer + offset-1);
      offset += length_file;
      uint32_t length_argv;
      memcpy(&length_argv, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_argv; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_argv-1]=0;
      this->argv = (char *)(inbuffer + offset-1);
      offset += length_argv;
     return offset;
    }

    const char * getType(){ return LOADLAUNCH; };
    const char * getMD5(){ return "e7cc67269dc4de0d6d7a3648f287878b"; };

  };

  class LoadLaunchResponse : public ros::Msg
  {
    public:

    LoadLaunchResponse()
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

    const char * getType(){ return LOADLAUNCH; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class LoadLaunch {
    public:
    typedef LoadLaunchRequest Request;
    typedef LoadLaunchResponse Response;
  };

}
#endif
