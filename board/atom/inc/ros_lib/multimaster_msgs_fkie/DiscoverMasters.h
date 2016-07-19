#ifndef _ROS_SERVICE_DiscoverMasters_h
#define _ROS_SERVICE_DiscoverMasters_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "multimaster_msgs_fkie/ROSMaster.h"

namespace multimaster_msgs_fkie
{

static const char DISCOVERMASTERS[] = "multimaster_msgs_fkie/DiscoverMasters";

  class DiscoverMastersRequest : public ros::Msg
  {
    public:

    DiscoverMastersRequest()
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

    const char * getType(){ return DISCOVERMASTERS; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class DiscoverMastersResponse : public ros::Msg
  {
    public:
      uint8_t masters_length;
      multimaster_msgs_fkie::ROSMaster st_masters;
      multimaster_msgs_fkie::ROSMaster * masters;

    DiscoverMastersResponse():
      masters_length(0), masters(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset++) = masters_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < masters_length; i++){
      offset += this->masters[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint8_t masters_lengthT = *(inbuffer + offset++);
      if(masters_lengthT > masters_length)
        this->masters = (multimaster_msgs_fkie::ROSMaster*)realloc(this->masters, masters_lengthT * sizeof(multimaster_msgs_fkie::ROSMaster));
      offset += 3;
      masters_length = masters_lengthT;
      for( uint8_t i = 0; i < masters_length; i++){
      offset += this->st_masters.deserialize(inbuffer + offset);
        memcpy( &(this->masters[i]), &(this->st_masters), sizeof(multimaster_msgs_fkie::ROSMaster));
      }
     return offset;
    }

    const char * getType(){ return DISCOVERMASTERS; };
    const char * getMD5(){ return "bc7525cc24dd3b880f044a2cdfb95aad"; };

  };

  class DiscoverMasters {
    public:
    typedef DiscoverMastersRequest Request;
    typedef DiscoverMastersResponse Response;
  };

}
#endif
