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

#ifndef LORA_GATEWAY_LOG_H__
#define LORA_GATEWAY_LOG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define LORAWAN_LOG_LEVEL_NONE       (0)
#define LORAWAN_LOG_LEVEL_ERROR      (1)
#define LORAWAN_LOG_LEVEL_WARN       (2)
#define LORAWAN_LOG_LEVEL_INFO       (3)
#define LORAWAN_LOG_LEVEL_DEBUG      (4)
#define LORAWAN_LOG_LEVEL_VERBOSE    (5)

#ifndef CONFIG_LORAWAN_LOG_DEFAULT_LEVEL
#define CONFIG_LORAWAN_LOG_DEFAULT_LEVEL LORAWAN_LOG_LEVEL_NONE
#endif

#ifndef LORAWAN_DEBUG_LEVEL
#define LORAWAN_LOG_LEVEL CONFIG_LORAWAN_LOG_DEFAULT_LEVEL
#else
#define LORAWAN_LOG_LEVEL LORAWAN_DEBUG_LEVEL
#endif

#ifndef CONFIG_LORAWAN_LOG_COLORS
#define CONFIG_LORAWAN_LOG_COLORS 0
#endif

#if CONFIG_LORAWAN_LOG_COLORS
#define LORAWAN_LOG_COLOR_BLACK   "30"
#define LORAWAN_LOG_COLOR_RED     "31" //ERROR
#define LORAWAN_LOG_COLOR_GREEN   "32" //INFO
#define LORAWAN_LOG_COLOR_YELLOW  "33" //WARNING
#define LORAWAN_LOG_COLOR_BLUE    "34"
#define LORAWAN_LOG_COLOR_MAGENTA "35"
#define LORAWAN_LOG_COLOR_CYAN    "36" //DEBUG
#define LORAWAN_LOG_COLOR_GRAY    "37" //VERBOSE
#define LORAWAN_LOG_COLOR_WHITE   "38"

#define LORAWAN_LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
#define LORAWAN_LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
#define LORAWAN_LOG_RESET_COLOR   "\033[0m"

#define LORAWAN_LOG_COLOR_E       LORAWAN_LOG_COLOR(LORAWAN_LOG_COLOR_RED)
#define LORAWAN_LOG_COLOR_W       LORAWAN_LOG_COLOR(LORAWAN_LOG_COLOR_YELLOW)
#define LORAWAN_LOG_COLOR_I       LORAWAN_LOG_COLOR(LORAWAN_LOG_COLOR_GREEN)
#define LORAWAN_LOG_COLOR_D       LORAWAN_LOG_COLOR(LORAWAN_LOG_COLOR_CYAN)
#define LORAWAN_LOG_COLOR_V       LORAWAN_LOG_COLOR(LORAWAN_LOG_COLOR_GRAY)
#else
#define LORAWAN_LOG_COLOR_E
#define LORAWAN_LOG_COLOR_W
#define LORAWAN_LOG_COLOR_I
#define LORAWAN_LOG_COLOR_D
#define LORAWAN_LOG_COLOR_V
#define LORAWAN_LOG_RESET_COLOR
#endif

const char * pathToFileName(const char * path);
void log_gateway_print(const char *fmt, ...);
void log_gateway_print_dump(uint8_t *buf, uint16_t len);

#define LORAWAN_SHORT_LOG_FORMAT(letter, format)  LORAWAN_LOG_COLOR_ ## letter format LORAWAN_LOG_RESET_COLOR ""
#define LORAWAN_LOG_FORMAT(letter, format)  LORAWAN_LOG_COLOR_ ## letter "[" #letter "][%20s:%u] %15s() --> " format LORAWAN_LOG_RESET_COLOR "", pathToFileName(__FILE__), __LINE__, __FUNCTION__

#if LORAWAN_LOG_LEVEL >= LORAWAN_LOG_LEVEL_VERBOSE
#define log_v(format, ...) log_gateway_print(LORAWAN_LOG_FORMAT(V, format), ##__VA_ARGS__)
#define log_v_dump log_gateway_print_dump
#else
#define log_v(format, ...)
#define log_v_dump
#endif

#if LORAWAN_LOG_LEVEL >= LORAWAN_LOG_LEVEL_DEBUG
#define log_d(format, ...) log_gateway_print(LORAWAN_LOG_FORMAT(D, format), ##__VA_ARGS__)
#define log_d_dump log_gateway_print_dump
#else
#define log_d(format, ...)
#define log_d_dump
#endif

#if LORAWAN_LOG_LEVEL >= LORAWAN_LOG_LEVEL_INFO
#define log_i(format, ...) log_gateway_print(LORAWAN_LOG_FORMAT(I, format), ##__VA_ARGS__)
#define log_i_dump log_gateway_print_dump
#else
#define log_i(format, ...)
#define log_i_dump
#endif

#if LORAWAN_LOG_LEVEL >= LORAWAN_LOG_LEVEL_WARN
#define log_w(format, ...) log_gateway_print(LORAWAN_LOG_FORMAT(W, format), ##__VA_ARGS__)
#define log_w_dump log_gateway_print_dump
#else
#define log_w(format, ...)
#define log_w_dump
#endif

#if LORAWAN_LOG_LEVEL >= LORAWAN_LOG_LEVEL_ERROR
#define log_e(format, ...) log_gateway_print(LORAWAN_LOG_FORMAT(E, format), ##__VA_ARGS__)
#define log_e_dump log_gateway_print_dump
#else
#define log_e(format, ...)
#define log_e_dump
#endif

#if !defined(CONFIG_NOASSERT)
#define ASSERT(cond) if(!(cond)) log_gateway_failed(__FILE__, __LINE__)
#else
#define ASSERT(cond) /**/
#endif

void log_gateway_int(void);

#ifdef __cplusplus
}
#endif

#endif /* LORA_GATEWAY_LOG_H__ */
