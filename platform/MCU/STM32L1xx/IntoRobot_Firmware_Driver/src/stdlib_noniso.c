/*
 core_esp8266_noniso.c - nonstandard (but usefull) conversion functions

 Copyright (c) 2014 Ivan Grokhotkov. All rights reserved.
 This file is part of the esp8266 core for Arduino environment.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 Modified 03 April 2015 by Markus Sattler

 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include "stdlib_noniso.h"

int _atoi(const char* str) {
    int total = 0;
    char sign = '+';

    // 跳过空格
    while( isspace( *str ) ) {
        ++str;
    }
    // 检查是否指定符号
    if( *str == '-' || *str == '+' ) {
        sign = *str++;
    }
    while( isdigit( *str ) ) {
        total = 10 * total + ( (*str++) - '0' );
    }
    return (sign == '-') ? -total : total;
}

long _atol(const char* str) {
    long total = 0;
    char sign = '+';

    // 跳过空格
    while( isspace( *str ) ) {
        ++str;
    }
    // 检查是否指定符号
    if( *str == '-' || *str == '+' ) {
        sign = *str++;
    }
    while( isdigit( *str ) ) {
        total = 10 * total + ( (*str++) - '0' );
    }
    return (sign == '-') ? -total : total;
}

double _atof(const char *str) {
    double total = 0.0;
    double decimal = 1.0;
    char sign = '+';

    // 跳过空格
    while( isspace( *str ) ) {
        ++str;
    }
    // 检查是否指定符号
    if( *str == '-' || *str == '+' ) {
        sign = *str++;
    }
    while( isdigit( *str ) ) {
        total = 10.0 * total + ( (*str++) - '0' );
    }
    if( *str=='.' ) {
        str++;
    }
    while( isdigit( *str ) ) {
        total = 10.0 * total + ( (*str++) - '0' );
        decimal *= 10.0;
    }
    total = total/decimal;
    return (sign == '-') ? -total : total;
}

int atoi(const char* s) {
    return _atoi(s);
}

long atol(const char* s) {
    return _atol(s);
}

double atof(const char* s) {
    return _atof(s);
}
