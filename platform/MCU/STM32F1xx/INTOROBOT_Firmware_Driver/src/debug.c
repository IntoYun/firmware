/**
 ******************************************************************************
 * @file     : debug.cpp
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
**/

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "macros.h"
#include "debug.h"


uint32_t log_level_at_run_time = LOG_LEVEL_AT_RUN_TIME;


/*********************************************************************************
  *Function:       void log_print_(int level, int line, const char *func, const char *file, const char *msg, ...)
  *Description:
  *Input:
  *Output:         none
  *Return:         none
  *author:         cky
  *date:           6-December-2014
  *Others:
**********************************************************************************/
void log_print_(int level, int line, const char *func, const char *file, const char *msg, ...)
{
    char _buffer[MAX_DEBUG_MESSAGE_LENGTH];
    static char * levels[] = 
    {
        "",
        "LOG  ",
        "DEBUG",
        "WARN ",
        "ERROR",
        "PANIC",
    };
    va_list args;
    va_start(args, msg);
    file = file ? strrchr(file,'/') + 1 : "";
    /*int trunc = snprintf(_buffer, arraySize(_buffer), "%010lu:<%s> %s %s(%d):", GetSystem1MsTick(), levels[level], func, file, line);
    if (debug_output_)
    {
    debug_output_(_buffer);
    if (trunc > arraySize(_buffer))
    {
    debug_output_("...");
    }
    }*/
    int trunc = vsnprintf(_buffer,arraySize(_buffer), msg, args);
    if (debug_output_)
    {
        debug_output_(_buffer);
        if (trunc > arraySize(_buffer))
        {
            debug_output_("...");
        }
        debug_output_("\r\n");
    }
}

