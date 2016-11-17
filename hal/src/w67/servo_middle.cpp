/*************************************************************************
 File Name   : tmp.cpp
 Author      : zhangbaoxian
 Mail        : baoxianzhit@gmail.com
 Created Time: Saturday, September 10, 2016 AM12:02:25 HKT
 ************************************************************************/

#include "servo_middle.h"

uint32_t EspgetCycleCount()
{
    uint32_t ccount;
    __asm__ __volatile__("esync; rsr %0,ccount":"=a" (ccount));
}
