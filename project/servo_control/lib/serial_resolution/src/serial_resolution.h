#ifndef __serial_resolution_H__
#define __serial_resolution_H__
#include"application.h"
typedef  unsigned char byte;


struct protocal
{
    byte head[3];
    byte data_length=0;
    byte command=0;
    byte data[20]={0};
    byte checksum=0;
};
bool get_num_of_xyz(struct protocal *prot);

byte getChecksum(byte data_length,byte cmd,byte mydata[]);


#endif