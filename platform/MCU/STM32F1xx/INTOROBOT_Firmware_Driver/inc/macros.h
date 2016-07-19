/**
 ******************************************************************************
 * @file     : macros.h
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

#ifndef   MACROS_H_
#define   MACROS_H_

#if !defined(arraySize)
#   define arraySize(a)            (sizeof((a))/sizeof((a[0])))
#endif

#define INVALID_CASE(c) PANIC(InvalidCase,"Invalid Case %d",(c))
#define UNUSED(var) (void)(var)

#define _CAT(a, b) a ## b
#define CAT(a, b) _CAT(a, b)

#define CCASSERT(predicate) _x_CCASSERT_LINE(predicate, __LINE__)
#define _x_CCASSERT_LINE(predicate, line) typedef char CAT(constraint_violated_on_line_,line)[2*((predicate)!=0)-1];

// Seconds to Us
#define S2u(s) ((s)*1000000)
// Mili Seconds to Us
#define MS2u(m) ((m)*1000)

// Seconds to Ms
#define S2M(s) ((s)*1000)

#endif /* MACROS_H_ */
