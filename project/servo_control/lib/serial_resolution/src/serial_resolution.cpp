#include "serial_resolution.h"

byte getChecksum(byte data_length,byte cmd,byte mydata[]){
    byte checksum=0;
    checksum ^= (data_length&0xFF);
    checksum ^= (cmd&0xFF);
    for(int i=0;i<data_length;i++)
    {
        //Serial.write(mydata[i]);
        checksum ^= (mydata[i]&0xFF);
    }
    return checksum;
}

bool get_num_of_xyz(struct protocal *prot)
{
    uint8_t inChar = 0;
    uint8_t i=0,j=0,p=0;
    bool start=true;
    start=true;
    while(Serial.available()>0)
    {
      if(start)
      {
          prot->head[0]=prot->head[1];
          prot->head[1]=prot->head[2];
          prot->head[2]=Serial.read();
      }
      if(prot->head[0]==0x24&&prot->head[1]==0x4D&&prot->head[2]==0x3E)
      {
         start=false;
         prot->data_length=Serial.read();
         prot->command=Serial.read();
         for(i=0;i<prot->data_length;i++)
         {
            prot->data[i]=Serial.read();
         }
         prot->checksum=Serial.read();
         if(prot->checksum==getChecksum(prot->data_length,prot->command, prot->data))
           {
             return true;
           }
         else
           return false;
      }
    }
}
