/**
 ******************************************************************************
 * @file     : sringbuffer.cpp
 * @author   : robin
 * @version  : V1.0.0
 * @date     : 6-December-2014
 * @brief    :   
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

 
#include <stdlib.h>
#include <string.h>
#include "stringbuffer.h"

#include "cmsis_os.h"

//Default buffer size for strings
#define BUFFER_SIZE 256
//there is a static buffer allocated, which is used to decode strings to
//strings cannot be longer than the buffer
char global_buffer[BUFFER_SIZE];

/*********************************************************************************
  *Function		:       
  *Description	:   
  *Input		:          
  *Output		:         
  *Return		:        
  *author		:        
  *date			:           
  *Others		:         
**********************************************************************************/
string_buffer *stringBufferCreate(void)
{
    string_buffer* result = (string_buffer *)pvPortMalloc(sizeof(string_buffer));
    if (result == NULL)
    {
        return NULL;
    }
    result->string = global_buffer;
    memset((void*) global_buffer, 0, BUFFER_SIZE);
    //unused - but will be usefull after realloc got fixd
    /*  if (result->string==NULL) {
    free(result);
    return NULL;
    }
    result->memory=BUFFER_DEFAULT_SIZE;*/
    result->memory = BUFFER_SIZE;
    result->string_length = 0;
    return result;
}

/*********************************************************************************
  *Function		:       
  *Description	:   
  *Input		:          
  *Output		:         
  *Return		:        
  *author		:        
  *date			:           
  *Others		:         
**********************************************************************************/
char stringBufferAdd(char value, string_buffer* buffer)
{
    if (buffer->string_length >= buffer->memory)
    {
        //this has to be enabled after realloc works
        /*char* new_string = (char*) realloc((void*) buffer->string, (buffer->memory
        + BUFFER_DEFAULT_SIZE) * sizeof(char));
        if (new_string == NULL)
        {
        free(buffer->string);
        buffer->string = NULL;
        return -1;
        } else {
        buffer->string = new_string;
        }
        buffer->memory += BUFFER_DEFAULT_SIZE;*/
        //in the meantime we just drop it
        return 0; //EOF would be a better choice - but that breaks json decoding
    }
    buffer->string[buffer->string_length] = value;
    buffer->string_length += 1;
    return 0;
}

/*********************************************************************************
  *Function		:       
  *Description	:   
  *Input		:          
  *Output		:         
  *Return		:        
  *author		:        
  *date			:           
  *Others		:         
**********************************************************************************/
char* stringBufferToString(string_buffer* buffer)
    {
    //this is the realloc dependent function - it does not work
    //  char* result = buffer->string;
    //ensure that the string ends with 0
    if (buffer->string_length == 0 || buffer->string[(buffer->string_length - 1)] != 0)
    {
        stringBufferAdd(0, buffer);
    }
    /*  char* string = realloc(result, buffer->string_length);
    if (string==NULL) {
    free(result);
    }
    buffer->string=NULL;
    free(buffer);
    return string;*/
    char* result = (char *)pvPortMalloc(buffer->string_length * sizeof(char));
    if (result == NULL)
    {
        return NULL;
    }
    strcpy(result, global_buffer);
    buffer->string = NULL;
    vPortFree(buffer);
    return result;
}

/*********************************************************************************
  *Function		:       
  *Description	:   
  *Input		:          
  *Output		:         
  *Return		:        
  *author		:        
  *date			:           
  *Others		:         
**********************************************************************************/
void stringBufferFree(string_buffer* buffer)
{
    if (buffer == NULL)
    {
        //hmm it was null before - whatever
        return;
    }
    //this is not needed in this realloc free concept
    /*
    if (buffer->string!=NULL) {
    free(buffer->string);
    }
    */
    vPortFree(buffer);
}

