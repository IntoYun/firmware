
#ifndef SYSTEM_UPDATE_H
#define	SYSTEM_UPDATE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef class Stream Stream;
#include <stdint.h>

void set_start_dfu_flasher_serial_speed(uint32_t speed);
void set_start_ymodem_flasher_serial_speed(uint32_t speed);

void system_lineCodingBitRateHandler(uint32_t bitrate);

#ifdef __cplusplus
}
#endif


#endif	/* SYSTEM_UPDATE_H */

