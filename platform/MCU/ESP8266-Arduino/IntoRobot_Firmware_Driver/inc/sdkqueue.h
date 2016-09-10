#ifndef SDK_QUEUE_H__
#define SDK_QUEUE_H__


#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

#define SDK_OK                       (1)
#define SDK_EQU                      (0)
#define SDK_ERR                      (-1)
#define SDK_TIME_OUT                 (-2)
#define SDK_PARA_ERR                 (-3)

#define SDK_MAX_QUEUE_SIZE           (256)

typedef struct
{
    int32_t siHead;
    int32_t siTail;
    uint32_t uiSize;
    uint8_t *heData;
}SDK_QUEUE;


extern int32_t sdkInitialQueue(SDK_QUEUE * const pstQueue, uint32_t uiQueueSize);
extern int32_t sdkGetQueueHead(SDK_QUEUE const* const pstQueue);
extern int32_t sdkClearQueue(SDK_QUEUE * const pstQueue);
extern int32_t sdkInsertQueue(SDK_QUEUE * pstQueue ,const uint8_t *phe , uint32_t siLen);
extern bool sdkIsQueueEmpty(SDK_QUEUE const * const pstQueue);
extern bool sdkIsQueueFull(SDK_QUEUE const * const pstQueue);
extern bool sdkTryQueueData(const SDK_QUEUE *pstQueue , int32_t siHead , uint8_t *pucOut);
extern int32_t sdkSetQueueHead(SDK_QUEUE * const pstQueue , int32_t siHead);
extern bool sdkGetQueueData(SDK_QUEUE * const pstQueue, uint8_t *pucOut);
extern int32_t sdkGetQueueDataLen(SDK_QUEUE * const pstQueue);
extern int32_t sdkReleaseQueue(SDK_QUEUE * const pstQueue);

#ifdef __cplusplus
}
#endif


#endif /* SDK_QUEUE_H__ */
