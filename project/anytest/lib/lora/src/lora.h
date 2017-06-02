#ifndef __LORA_H__
#define __LORA_H__

#include "platforms.h"

#if PLATFORM_ID == PLATFORM_ANYTEST
#include "radio.h"
#include "timer.h"
#include "sx1276.h"
#include "sx1276-board.h"

void TestSX1276Init(void);
uint8_t ProcessSX1276(int8_t &rssi, int8_t &snr, int8_t *dat);
#endif

#endif
