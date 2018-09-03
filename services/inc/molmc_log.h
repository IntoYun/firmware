/*
 * Copyright (c) 2013-2018 Molmc Group. All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __MOLMC_LOG_H__
#define __MOLMC_LOG_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MOLMC_LOG_LOCAL_LEVEL
#define MOLMC_LOG_LOCAL_LEVEL  FIRMWARE_CONFIG_LOG_DEFAULT_LEVEL
#endif

/**
 * @brief Log level
 *
 */
typedef enum {
    MOLMC_LOG_NONE,       /*!< No log output */
    MOLMC_LOG_ERROR,      /*!< Critical errors, software module can not recover on its own */
    MOLMC_LOG_WARN,       /*!< Error conditions from which recovery measures have been taken */
    MOLMC_LOG_INFO,       /*!< Information messages which describe normal flow of events */
    MOLMC_LOG_DEBUG,      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    MOLMC_LOG_VERBOSE     /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
} molmc_log_level_t;

typedef void (*log_output_fn_t)(const char *);

/**
 * @brief Set log level for given tag
 *
 * If logging for given component has already been enabled, changes previous setting.
 *
 * @param tag Tag of the log entries to enable. Must be a non-NULL zero terminated string.
 *            Value "*" resets log level for all tags to the given value.
 *
 * @param level  Selects log level to enable. Only logs at this and lower levels will be shown.
 */
void molmc_log_level_set(const char* tag, molmc_log_level_t level);

/**
 * @brief Set function used to output log entries
 *
 * By default, log output goes to UART0. This function can be used to redirect log
 * output to some other destination, such as file or network. Returns the original
 * log handler, which may be necessary to return output to the previous destination.
 *
 * @param func new Function used for output. Must have same signature as vprintf.
 *
 * @return func old Function used for output.
 */
log_output_fn_t molmc_log_set_output(log_output_fn_t func);

/**
 * @brief Function which returns timestamp to be used in log output
 *
 * This function is used in expansion of MOLMC_LOGx macros.
 * In the 2nd stage bootloader, and at early application startup stage
 * this function uses CPU cycle counter as time source. Later when
 * FreeRTOS scheduler start running, it switches to FreeRTOS tick count.
 *
 * For now, we ignore millisecond counter overflow.
 *
 * @return timestamp, in milliseconds
 */
uint32_t molmc_log_timestamp(void);

/**
 * @brief whether log should output
 *
 * This function or these macros should not be used from an interrupt.
 */
bool molmc_log_should_output(const char* tag, molmc_log_level_t level);

/**
 * @brief Write message into the log
 *
 * This function is not intended to be used directly. Instead, use one of
 * MOLMC_LOGE, MOLMC_LOGW, MOLMC_LOGI, MOLMC_LOGD, MOLMC_LOGV macros.
 *
 * This function or these macros should not be used from an interrupt.
 */
void molmc_log_write(const char* tag, molmc_log_level_t level, const char* format, ...);

//these functions do not check level versus MOLMC_LOCAL_LEVEL, this should be done in iotx_log_api.h
/**
 * @brief Log a buffer of hex bytes at Info level
 *
 * @param  tag      description tag
 *
 * @param level level of the log
 *
 * @param  buffer   Pointer to the buffer array
 *
 * @param  buff_len length of buffer in bytes
 *
 * @see ``molmc_log_buffer_hex_level``
 *
 */
void molmc_log_buffer_hex_internal(const char *tag, molmc_log_level_t level, const void *buffer, uint16_t buff_len);

/**
 * @brief Log a buffer of characters at Info level. Buffer should contain only printable characters.
 *
 * @param  tag      description tag
 *
 * @param level level of the log
 *
 * @param  buffer   Pointer to the buffer array
 *
 * @param  buff_len length of buffer in bytes
 *
 * @see ``molmc_log_buffer_char_level``
 *
 */
void molmc_log_buffer_char_internal(const char *tag, molmc_log_level_t level, const void *buffer, uint16_t buff_len);

/**
 * @brief Dump a buffer to the log at specified level.
 *
 * The dump log shows just like the one below:
 *
 *      W (195) log_example: 0x3ffb4280   45 53 50 33 32 20 69 73  20 67 72 65 61 74 2c 20  |Molmc is great, |
 *      W (195) log_example: 0x3ffb4290   77 6f 72 6b 69 6e 67 20  61 6c 6f 6e 67 20 77 69  |working along wi|
 *      W (205) log_example: 0x3ffb42a0   74 68 20 74 68 65 20 49  44 46 2e 00              |th the SDK..|
 *
 * It is highly recommend to use terminals with over 102 text width.
 * *
 * @param tag description tag
 *
 * @param level level of the log
 *
 * @param buffer Pointer to the buffer array
 *
 * @param buff_len length of buffer in bytes
 */

void molmc_log_buffer_hexdump_internal(const char *tag, molmc_log_level_t log_level, const void *buffer, uint16_t buff_len);

#if CONFIG_MOLMC_LOG_COLORS
#define MOLMC_LOG_COLOR_BLACK   "30"
#define MOLMC_LOG_COLOR_RED     "31"
#define MOLMC_LOG_COLOR_GREEN   "32"
#define MOLMC_LOG_COLOR_BROWN   "33"
#define MOLMC_LOG_COLOR_BLUE    "34"
#define MOLMC_LOG_COLOR_PURPLE  "35"
#define MOLMC_LOG_COLOR_CYAN    "36"
#define MOLMC_LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
#define MOLMC_LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
#define MOLMC_LOG_RESET_COLOR   "\033[0m"
#define MOLMC_LOG_COLOR_E       MOLMC_LOG_COLOR(MOLMC_LOG_COLOR_RED)
#define MOLMC_LOG_COLOR_W       MOLMC_LOG_COLOR(MOLMC_LOG_COLOR_BROWN)
#define MOLMC_LOG_COLOR_I       MOLMC_LOG_COLOR(MOLMC_LOG_COLOR_GREEN)
#define MOLMC_LOG_COLOR_D
#define MOLMC_LOG_COLOR_V
#else //CONFIG_MOLMC_LOG_COLORS
#define MOLMC_LOG_COLOR_E
#define MOLMC_LOG_COLOR_W
#define MOLMC_LOG_COLOR_I
#define MOLMC_LOG_COLOR_D
#define MOLMC_LOG_COLOR_V
#define MOLMC_LOG_RESET_COLOR
#endif //CONFIG_MOLMC_LOG_COLORS

#define MOLMC_LOG_FORMAT_HEADER(letter)        MOLMC_LOG_COLOR_##letter #letter " [%010u]:[%-12.12s]: " MOLMC_LOG_RESET_COLOR
#define MOLMC_LOG_FORMAT_TEXT(letter, format)  MOLMC_LOG_COLOR_##letter format MOLMC_LOG_RESET_COLOR

#define MOLMC_LOGE_SHOULD_OUTPUT( tag ) molmc_log_should_output( tag, MOLMC_LOG_ERROR )
#define MOLMC_LOGW_SHOULD_OUTPUT( tag ) molmc_log_should_output( tag, MOLMC_LOG_WARN )
#define MOLMC_LOGI_SHOULD_OUTPUT( tag ) molmc_log_should_output( tag, MOLMC_LOG_INFO )
#define MOLMC_LOGD_SHOULD_OUTPUT( tag ) molmc_log_should_output( tag, MOLMC_LOG_DEBUG )
#define MOLMC_LOGV_SHOULD_OUTPUT( tag ) molmc_log_should_output( tag, MOLMC_LOG_VERBOSE )

#if MOLMC_LOG_LOCAL_LEVEL == MOLMC_LOG_NONE
#define MOLMC_LOGE_HEADER( tag )
#define MOLMC_LOGW_HEADER( tag )
#define MOLMC_LOGI_HEADER( tag )
#define MOLMC_LOGD_HEADER( tag )
#define MOLMC_LOGV_HEADER( tag )

#define MOLMC_LOGE_TEXT( tag, format, ... )
#define MOLMC_LOGW_TEXT( tag, format, ... )
#define MOLMC_LOGI_TEXT( tag, format, ... )
#define MOLMC_LOGD_TEXT( tag, format, ... )
#define MOLMC_LOGV_TEXT( tag, format, ... )

#define MOLMC_LOGE( tag, format, ... )
#define MOLMC_LOGW( tag, format, ... )
#define MOLMC_LOGI( tag, format, ... )
#define MOLMC_LOGD( tag, format, ... )
#define MOLMC_LOGV( tag, format, ... )

#define MOLMC_LOGE_BUFFER_CHAR( tag, buffer, buff_len )
#define MOLMC_LOGW_BUFFER_CHAR( tag, buffer, buff_len )
#define MOLMC_LOGI_BUFFER_CHAR( tag, buffer, buff_len )
#define MOLMC_LOGD_BUFFER_CHAR( tag, buffer, buff_len )
#define MOLMC_LOGV_BUFFER_CHAR( tag, buffer, buff_len )

#define MOLMC_LOGE_BUFFER_HEX( tag, buffer, buff_len )
#define MOLMC_LOGW_BUFFER_HEX( tag, buffer, buff_len )
#define MOLMC_LOGI_BUFFER_HEX( tag, buffer, buff_len )
#define MOLMC_LOGD_BUFFER_HEX( tag, buffer, buff_len )
#define MOLMC_LOGV_BUFFER_HEX( tag, buffer, buff_len )

#define MOLMC_LOGE_BUFFER_HEXDUMP( tag, buffer, buff_len )
#define MOLMC_LOGW_BUFFER_HEXDUMP( tag, buffer, buff_len )
#define MOLMC_LOGI_BUFFER_HEXDUMP( tag, buffer, buff_len )
#define MOLMC_LOGD_BUFFER_HEXDUMP( tag, buffer, buff_len )
#define MOLMC_LOGV_BUFFER_HEXDUMP( tag, buffer, buff_len )

#else

#define MOLMC_LOGE_HEADER( tag ) do { molmc_log_write( tag, MOLMC_LOG_ERROR,    MOLMC_LOG_FORMAT_HEADER(E), molmc_log_timestamp(), tag ); } while(0)
#define MOLMC_LOGW_HEADER( tag ) do { molmc_log_write( tag, MOLMC_LOG_WARN,     MOLMC_LOG_FORMAT_HEADER(W), molmc_log_timestamp(), tag ); } while(0)
#define MOLMC_LOGI_HEADER( tag ) do { molmc_log_write( tag, MOLMC_LOG_INFO,     MOLMC_LOG_FORMAT_HEADER(I), molmc_log_timestamp(), tag ); } while(0)
#define MOLMC_LOGD_HEADER( tag ) do { molmc_log_write( tag, MOLMC_LOG_DEBUG,    MOLMC_LOG_FORMAT_HEADER(D), molmc_log_timestamp(), tag ); } while(0)
#define MOLMC_LOGV_HEADER( tag ) do { molmc_log_write( tag, MOLMC_LOG_VERBOSE,  MOLMC_LOG_FORMAT_HEADER(V), molmc_log_timestamp(), tag ); } while(0)

#define MOLMC_LOGE_TEXT( tag, format, ... ) do { molmc_log_write( tag, MOLMC_LOG_ERROR,   MOLMC_LOG_FORMAT_TEXT(E, format), ##__VA_ARGS__ ); } while(0)
#define MOLMC_LOGW_TEXT( tag, format, ... ) do { molmc_log_write( tag, MOLMC_LOG_WARN,    MOLMC_LOG_FORMAT_TEXT(W, format), ##__VA_ARGS__ ); } while(0)
#define MOLMC_LOGI_TEXT( tag, format, ... ) do { molmc_log_write( tag, MOLMC_LOG_INFO,    MOLMC_LOG_FORMAT_TEXT(I, format), ##__VA_ARGS__ ); } while(0)
#define MOLMC_LOGD_TEXT( tag, format, ... ) do { molmc_log_write( tag, MOLMC_LOG_DEBUG,   MOLMC_LOG_FORMAT_TEXT(D, format), ##__VA_ARGS__ ); } while(0)
#define MOLMC_LOGV_TEXT( tag, format, ... ) do { molmc_log_write( tag, MOLMC_LOG_VERBOSE, MOLMC_LOG_FORMAT_TEXT(V, format), ##__VA_ARGS__ ); } while(0)

#define MOLMC_LOGE( tag, format, ... ) do { MOLMC_LOGE_HEADER( tag ); MOLMC_LOGE_TEXT( tag, format, ##__VA_ARGS__ ); MOLMC_LOGE_TEXT( tag, "%s", "\n" ); } while(0)
#define MOLMC_LOGW( tag, format, ... ) do { MOLMC_LOGW_HEADER( tag ); MOLMC_LOGW_TEXT( tag, format, ##__VA_ARGS__ ); MOLMC_LOGW_TEXT( tag, "%s", "\n" ); } while(0)
#define MOLMC_LOGI( tag, format, ... ) do { MOLMC_LOGI_HEADER( tag ); MOLMC_LOGI_TEXT( tag, format, ##__VA_ARGS__ ); MOLMC_LOGI_TEXT( tag, "%s", "\n" ); } while(0)
#define MOLMC_LOGD( tag, format, ... ) do { MOLMC_LOGD_HEADER( tag ); MOLMC_LOGD_TEXT( tag, format, ##__VA_ARGS__ ); MOLMC_LOGD_TEXT( tag, "%s", "\n" ); } while(0)
#define MOLMC_LOGV( tag, format, ... ) do { MOLMC_LOGV_HEADER( tag ); MOLMC_LOGV_TEXT( tag, format, ##__VA_ARGS__ ); MOLMC_LOGV_TEXT( tag, "%s", "\n" ); } while(0)

#define MOLMC_LOGE_BUFFER_CHAR( tag, buffer, buff_len ) do { molmc_log_buffer_char_internal( tag, MOLMC_LOG_ERROR,   buffer, buff_len ); } while(0)
#define MOLMC_LOGW_BUFFER_CHAR( tag, buffer, buff_len ) do { molmc_log_buffer_char_internal( tag, MOLMC_LOG_WARN,    buffer, buff_len ); } while(0)
#define MOLMC_LOGI_BUFFER_CHAR( tag, buffer, buff_len ) do { molmc_log_buffer_char_internal( tag, MOLMC_LOG_INFO,    buffer, buff_len ); } while(0)
#define MOLMC_LOGD_BUFFER_CHAR( tag, buffer, buff_len ) do { molmc_log_buffer_char_internal( tag, MOLMC_LOG_DEBUG,   buffer, buff_len ); } while(0)
#define MOLMC_LOGV_BUFFER_CHAR( tag, buffer, buff_len ) do { molmc_log_buffer_char_internal( tag, MOLMC_LOG_VERBOSE, buffer, buff_len ); } while(0)

#define MOLMC_LOGE_BUFFER_HEX( tag, buffer, buff_len ) do { molmc_log_buffer_hex_internal( tag, MOLMC_LOG_ERROR,   buffer, buff_len ); } while(0)
#define MOLMC_LOGW_BUFFER_HEX( tag, buffer, buff_len ) do { molmc_log_buffer_hex_internal( tag, MOLMC_LOG_WARN,    buffer, buff_len ); } while(0)
#define MOLMC_LOGI_BUFFER_HEX( tag, buffer, buff_len ) do { molmc_log_buffer_hex_internal( tag, MOLMC_LOG_INFO,    buffer, buff_len ); } while(0)
#define MOLMC_LOGD_BUFFER_HEX( tag, buffer, buff_len ) do { molmc_log_buffer_hex_internal( tag, MOLMC_LOG_DEBUG,   buffer, buff_len ); } while(0)
#define MOLMC_LOGV_BUFFER_HEX( tag, buffer, buff_len ) do { molmc_log_buffer_hex_internal( tag, MOLMC_LOG_VERBOSE, buffer, buff_len ); } while(0)

#define MOLMC_LOGE_BUFFER_HEXDUMP( tag, buffer, buff_len ) do { molmc_log_buffer_hexdump_internal( tag, MOLMC_LOG_ERROR,   buffer, buff_len ); } while(0)
#define MOLMC_LOGW_BUFFER_HEXDUMP( tag, buffer, buff_len ) do { molmc_log_buffer_hexdump_internal( tag, MOLMC_LOG_WARN,    buffer, buff_len ); } while(0)
#define MOLMC_LOGI_BUFFER_HEXDUMP( tag, buffer, buff_len ) do { molmc_log_buffer_hexdump_internal( tag, MOLMC_LOG_INFO,    buffer, buff_len ); } while(0)
#define MOLMC_LOGD_BUFFER_HEXDUMP( tag, buffer, buff_len ) do { molmc_log_buffer_hexdump_internal( tag, MOLMC_LOG_DEBUG,   buffer, buff_len ); } while(0)
#define MOLMC_LOGV_BUFFER_HEXDUMP( tag, buffer, buff_len ) do { molmc_log_buffer_hexdump_internal( tag, MOLMC_LOG_VERBOSE, buffer, buff_len ); } while(0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MOLMC_LOG_H__ */
