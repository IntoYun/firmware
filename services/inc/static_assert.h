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

#ifndef STATIC_ASSERT_H_
#define	STATIC_ASSERT_H_

#ifdef	__cplusplus
extern "C" {
#endif

#define STATIC_ASSERT_EXPR(name, condition) ((void)sizeof(char[1 - 2*!!(condition)]))

#if defined(__cplusplus) && __cplusplus >= 201103L
#define STATIC_ASSERT(name,condition) static_assert(condition,#name)
#else
#define STATIC_ASSERT(name,condition) typedef char assert_##name[(condition)?0:-1]
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* STATIC_ASSERT_H_ */

