#include "sdkqueue.h"
#include <stdbool.h>

s32 sdkClearQueue(SDK_QUEUE * const pstQueue)
{
    if(pstQueue == NULL)
    {
        return SDK_PARA_ERR;
    }
    memset(pstQueue , 0 , sizeof(*pstQueue));
    return SDK_OK;
}

bool sdkIsQueueEmpty(SDK_QUEUE const * const pstQueue)
{
    if(pstQueue == NULL)
    {
        //Assert(0);
        return true;
    }
    return (pstQueue->siHead == pstQueue->siTail);
}

bool sdkIsQueueFull(SDK_QUEUE const * const pstQueue)
{
    if(pstQueue == NULL)
    {
        //Assert(0);
        return true;
    }

    if(pstQueue->siHead == ((pstQueue->siTail + 1) % SDK_MAX_QUEUE_SIZE))
    {
        return true;
    }
    else
    {
        return false;
    }
}

s32 sdkInsertQueue(SDK_QUEUE *  pstQueue ,const u8 *phe , s32 siLen)
{
    s32 uii;

    if(pstQueue == NULL || phe == NULL || siLen <= 0 )
    {
        return SDK_PARA_ERR;
    }

    for(uii = 0; uii < siLen; uii++)
    {
        if( !sdkIsQueueFull(pstQueue) )
        {
            pstQueue->heData[pstQueue->siTail] = phe[uii];
            pstQueue->siTail = (pstQueue->siTail + 1) % SDK_MAX_QUEUE_SIZE;
        }
        else
        {
            //Assert(0);
            break;
        }
    }
    return uii;
}

s32 sdkSetQueueHead(SDK_QUEUE * const pstQueue , s32 siHead)
{
   if(siHead >= SDK_MAX_QUEUE_SIZE || NULL == pstQueue || siHead < 0 )
    {
        //Assert(0);
        return SDK_PARA_ERR;
    }

    siHead =  siHead % SDK_MAX_QUEUE_SIZE;  //lint !e539

    if( !sdkIsQueueEmpty(pstQueue) )
    {
        if(pstQueue->siHead < pstQueue->siTail)
        {
            if(siHead >= pstQueue->siHead &&  siHead <= pstQueue->siTail)
            {
                pstQueue->siHead = siHead;
            }
            else
            {
                sdkClearQueue(pstQueue);
                return SDK_ERR;
            }
        }
        else
        {
            if(siHead >= pstQueue->siHead || siHead <= pstQueue->siTail)
            {
                pstQueue->siHead = siHead;
            }
            else
            {
                sdkClearQueue(pstQueue);
                return SDK_ERR;
            }
        }
    }
    return SDK_OK;
}

s32 sdkGetQueueHead(SDK_QUEUE const* const pstQueue)
{
    return pstQueue->siHead;
}

bool sdkTryQueueData(const SDK_QUEUE *pstQueue , s32 siHead , u8 *pucOut)
{
    siHead = siHead % SDK_MAX_QUEUE_SIZE;

    if( !sdkIsQueueEmpty(pstQueue) )
    {
        if(pstQueue->siHead < pstQueue->siTail)
        {
            if(siHead >= pstQueue->siHead &&  siHead < pstQueue->siTail)
            {
                *pucOut = pstQueue->heData[siHead];
                return true;
            }
        }
        else
        {
            if(siHead >= pstQueue->siHead || siHead < pstQueue->siTail)
            {
                *pucOut = pstQueue->heData[siHead];
                return true;
            }
        }
    }
    return false;
}

bool sdkGetQueueData(SDK_QUEUE * const pstQueue, u8 *pucOut)
{
    if( !sdkIsQueueEmpty(pstQueue) )
    {
        *pucOut = pstQueue->heData[pstQueue->siHead];
        pstQueue->siHead = (pstQueue->siHead + 1) % SDK_MAX_QUEUE_SIZE;
        return true;
    }
    return false;
}

