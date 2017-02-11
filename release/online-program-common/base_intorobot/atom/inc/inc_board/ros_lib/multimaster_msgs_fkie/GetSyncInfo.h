#ifndef _ROS_SERVICE_GetSyncInfo_h
#define _ROS_SERVICE_GetSyncInfo_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "multimaster_msgs_fkie/SyncMasterInfo.h"

namespace multimaster_msgs_fkie
{

static const char GETSYNCINFO[] = "multimaster_msgs_fkie/GetSyncInfo";

  class GetSyncInfoRequest : public ros::Msg
  {
    public:

    GetSyncInfoRequest()
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

    const char * getType(){ return GETSYNCINFO; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class GetSyncInfoResponse : public ros::Msg
  {
    public:
      uint8_t hosts_length;
      multimaster_msgs_fkie::SyncMasterInfo st_hosts;
      multimaster_msgs_fkie::SyncMasterInfo * hosts;

    GetSyncInfoResponse():
      hosts_length(0), hosts(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset++) = hosts_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < hosts_length; i++){
      offset += this->hosts[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint8_t hosts_lengthT = *(inbuffer + offset++);
      if(hosts_lengthT > hosts_length)
        this->hosts = (multimaster_msgs_fkie::SyncMasterInfo*)realloc(this->hosts, hosts_lengthT * sizeof(multimaster_msgs_fkie::SyncMasterInfo));
      offset += 3;
      hosts_length = hosts_lengthT;
      for( uint8_t i = 0; i < hosts_length; i++){
      offset += this->st_hosts.deserialize(inbuffer + offset);
        memcpy( &(this->hosts[i]), &(this->st_hosts), sizeof(multimaster_msgs_fkie::SyncMasterInfo));
      }
     return offset;
    }

    const char * getType(){ return GETSYNCINFO; };
    const char * getMD5(){ return "d5261ec56e202860a07fb47b41e1b2a8"; };

  };

  class GetSyncInfo {
    public:
    typedef GetSyncInfoRequest Request;
    typedef GetSyncInfoResponse Response;
  };

}
#endif
