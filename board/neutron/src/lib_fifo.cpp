#include <stdlib.h>
#include "application.h"
#include "lib_fifo.h"
#include "lib_system_all.h"
#include "cmsis_os.h"

//可读数据长度
#define FIFO_AVALIABLE(fifo) ( (fifo->p_w >= fifo->p_r)?(fifo->p_w-fifo->p_r):(fifo->p_w + fifo->size - fifo->p_r) );


#define FIFO_PW_NEXT(fifo) {fifo->p_w++;if(fifo->p_w == fifo->size) fifo->p_w = 0;}
#define FIFO_PR_NEXT(fifo) {fifo->p_r++;if(fifo->p_r == fifo->size) fifo->p_r = 0;}



/*
成功 0
失败 -1
*/
int fifo__init(fifo_t *fifo,int size)
{
  fifo->buf=(uint8_t *)pvPortMalloc(size);
	MO_ASSERT((fifo->buf!=NULL));

  fifo->size=size;
  fifo->p_w=0;
  fifo->p_r=0;

	if(fifo->buf==NULL)
	{
		return -1;
	}
	else
	{
		return 0;
	}

}

void fifo__deinit(fifo_t *fifo)
{
	vPortFree(fifo->buf);
}



/*向FIFO 中写入数据 */
__inline void fifo_put( fifo_t *fifo,uint8_t data)
{

  FIFO_PW_NEXT(fifo);
  if(fifo->p_w == fifo->p_r)
  {
    FIFO_PR_NEXT(fifo);
  }

  fifo->buf[fifo->p_w] = data;

}

/*从FIFO 中取出一个数据 */
__inline unsigned char fifo_get(fifo_t *fifo)
{
  FIFO_PR_NEXT(fifo);
  return (uint8_t)(fifo->buf[fifo->p_r]);
}

void fifo__write(fifo_t *fifo,const uint8_t *p_dat,int len)
{
  int i;
  for(i=0;i<len;i++)
  {
    fifo_put(fifo,*(p_dat+i));
  }
}

int fifo__read(fifo_t *fifo,uint8_t *p_buf,int buf_size)
{
  int i,read_len,ava_len;

  ava_len=FIFO_AVALIABLE(fifo);

  read_len= ( buf_size <= ava_len )? buf_size: ava_len;

  for(i=0;i<read_len;i++)
  {
    *(p_buf+i)=fifo_get(fifo);
  }

  return read_len;

}


int fifo__avaliable(fifo_t *fifo)
{
	return FIFO_AVALIABLE(fifo);
}













