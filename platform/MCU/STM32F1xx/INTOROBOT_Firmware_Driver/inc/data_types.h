/**
 ******************************************************************************
 * @file     : data_types.h
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



#ifndef   DATA_TYPES_H_
#define   DATA_TYPES_H_

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef	__cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL        		(0)
#endif

#ifndef FALSE
#define FALSE       		(0)
#endif

#ifndef TRUE
#define TRUE        		(!FALSE)
#endif

#ifndef _INT8
#define _INT8
typedef signed char   		INT8;
#endif

#ifndef _UINT8
#define _UINT8
typedef unsigned char   	UINT8;
#endif

#ifndef _INT16
#define _INT16
typedef signed short  		INT16;
#endif

#ifndef _UINT16
#define _UINT16
typedef unsigned short   	UINT16;
#endif

#ifndef _INT32
#define _INT32
typedef signed long   		INT32;
#endif

#ifndef _UINT32
#define _UINT32
typedef unsigned long   	UINT32;
#endif

typedef char            	CHAR;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* DATA_TYPES_H_ */
