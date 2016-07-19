#ifndef __sdkQUEUE_H__
#define __sdkQUEUE_H__

#include "variant.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define SDK_OK                       (1)
#define SDK_EQU                      (0)
#define SDK_ERR                      (-1)
#define SDK_TIME_OUT                 (-2)
#define SDK_PARA_ERR                 (-3)

//#define SDK_MAX_QUEUE_SIZE              (4096)
#define SDK_MAX_QUEUE_SIZE              (1024*20)

typedef struct
{
    s32 siHead;
    s32 siTail;
    u8 heData[SDK_MAX_QUEUE_SIZE];
}SDK_QUEUE;

extern s32 sdkGetQueueHead(SDK_QUEUE const* const pstQueue);
extern s32 sdkClearQueue(SDK_QUEUE * const pstQueue);
extern s32 sdkInsertQueue(SDK_QUEUE * pstQueue ,const u8 *phe , s32 siLen);
extern bool sdkIsQueueEmpty(SDK_QUEUE const * const pstQueue);
extern bool sdkIsQueueFull(SDK_QUEUE const * const pstQueue);
extern bool sdkTryQueueData(const SDK_QUEUE *pstQueue , s32 siHead , u8 *pucOut);
extern s32 sdkSetQueueHead(SDK_QUEUE * const pstQueue , s32 siHead);
extern bool sdkGetQueueData(SDK_QUEUE * const pstQueue, u8 *pucOut);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __QUEUE_H__ */
