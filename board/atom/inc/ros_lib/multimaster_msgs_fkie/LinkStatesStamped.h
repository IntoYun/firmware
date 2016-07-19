#ifndef _ROS_multimaster_msgs_fkie_LinkStatesStamped_h
#define _ROS_multimaster_msgs_fkie_LinkStatesStamped_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "multimaster_msgs_fkie/LinkState.h"

namespace multimaster_msgs_fkie
{

  class LinkStatesStamped : public ros::Msg
  {
    public:
      std_msgs::Header header;
      uint8_t links_length;
      multimaster_msgs_fkie::LinkState st_links;
      multimaster_msgs_fkie::LinkState * links;

    LinkStatesStamped():
      header(),
      links_length(0), links(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset++) = links_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < links_length; i++){
      offset += this->links[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      uint8_t links_lengthT = *(inbuffer + offset++);
      if(links_lengthT > links_length)
        this->links = (multimaster_msgs_fkie::LinkState*)realloc(this->links, links_lengthT * sizeof(multimaster_msgs_fkie::LinkState));
      offset += 3;
      links_length = links_lengthT;
      for( uint8_t i = 0; i < links_length; i++){
      offset += this->st_links.deserialize(inbuffer + offset);
        memcpy( &(this->links[i]), &(this->st_links), sizeof(multimaster_msgs_fkie::LinkState));
      }
     return offset;
    }

    const char * getType(){ return "multimaster_msgs_fkie/LinkStatesStamped"; };
    const char * getMD5(){ return "b7c41a46488485ec6593097b0cabe904"; };

  };

}
#endif