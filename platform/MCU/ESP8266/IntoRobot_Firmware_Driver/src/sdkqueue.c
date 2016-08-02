/**
 ******************************************************************************
  Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
*/

#include "sdkqueue.h"
#include <stdlib.h>
#include <string.h>

int32_t sdkClearQueue(SDK_QUEUE * const pstQueue)
{
    if(pstQueue == NULL)
    {
        return SDK_PARA_ERR;
    }
    memset(pstQueue, 0 , sizeof(*pstQueue));
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

int32_t sdkInsertQueue(SDK_QUEUE *  pstQueue ,const uint8_t *phe , int32_t siLen)
{
    int32_t uii;

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

int32_t sdkSetQueueHead(SDK_QUEUE * const pstQueue , int32_t siHead)
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

int32_t sdkGetQueueHead(SDK_QUEUE const* const pstQueue)
{
    return pstQueue->siHead;
}

bool sdkTryQueueData(const SDK_QUEUE *pstQueue , int32_t siHead , uint8_t *pucOut)
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

bool sdkGetQueueData(SDK_QUEUE * const pstQueue, uint8_t *pucOut)
{
    if( !sdkIsQueueEmpty(pstQueue) )
    {
        *pucOut = pstQueue->heData[pstQueue->siHead];
        pstQueue->siHead = (pstQueue->siHead + 1) % SDK_MAX_QUEUE_SIZE;
        return true;
    }
    return false;
}

int32_t sdkGetQueueDataLen(SDK_QUEUE * const pstQueue)
{
    int32_t uii;

    if( !sdkIsQueueEmpty(pstQueue) )
    {
        if(pstQueue->siHead < pstQueue->siTail)
        {
            uii = pstQueue->siTail - pstQueue->siHead;
        }
        else
        {
            uii = pstQueue->siTail + SDK_MAX_QUEUE_SIZE - pstQueue->siHead;
        }
        return uii;
    }
    return 0;
}


