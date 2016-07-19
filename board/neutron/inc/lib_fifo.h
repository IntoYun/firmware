#ifndef __MO_LIB_FIFO_H__
#define __MO_LIB_FIFO_H__

#include<stdint.h>

typedef struct 
{
  uint8_t *buf; //缓冲地址
  int p_w;      //写位置
  int p_r;      //读位置
  int size;     //缓冲大小
}fifo_t;

int fifo__init(fifo_t *fifo,int size);
void fifo__write(fifo_t *fifo,const uint8_t *p_dat,int len);
int fifo__read(fifo_t *fifo,uint8_t *p_buf,int buf_size);
int fifo__avaliable(fifo_t *fifo);
void fifo__deinit(fifo_t *fifo);

#endif


