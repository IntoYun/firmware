/**
 ******************************************************************************
 * @file     : panic_codes.h
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

#ifndef	  PANIC_CODES_H_
#define	  PANIC_CODES_H_

def_panic_codes(Faults,RGB_COLOR_RED,HardFault)
def_panic_codes(Faults,RGB_COLOR_RED,NMIFault)
def_panic_codes(Faults,RGB_COLOR_RED,MemManage)
def_panic_codes(Faults,RGB_COLOR_RED,BusFault)
def_panic_codes(Faults,RGB_COLOR_RED,UsageFault)

def_panic_codes(Cloud,RGB_COLOR_RED,InvalidLenth)

def_panic_codes(System,RGB_COLOR_RED,Exit)
def_panic_codes(System,RGB_COLOR_RED,OutOfHeap)

def_panic_codes(System,RGB_COLOR_RED,SPIOverRun)

def_panic_codes(Software,RGB_COLOR_RED,AssertionFailure)
def_panic_codes(Software,RGB_COLOR_RED,InvalidCase)
def_panic_codes(Software,RGB_COLOR_RED,PureVirtualCall)


#endif /*PANIC_CODES_H_*/

