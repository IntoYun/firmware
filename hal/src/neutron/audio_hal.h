#ifndef __AUDIO_HAL_H_
#define __AUDIO_HAL_H_
#include "stdint.h"
/* Audio recording frequency in Hz */
#define REC_FREQ                          8000
/* PDM buffer input size */
#define INTERNAL_BUFF_SIZE                64
#define I2S_PDM_BUFF_SIZE                 64
/* PCM buffer output size */
#define PCM_OUT_SIZE                      16

typedef struct mic_mems_s
{
   uint8_t *P_buf;
   uint8_t status_buf;
} mic_mems_t;


void HAL_Audio_Begin(mic_mems_t *p);
void HAL_Audio_Stop();

#endif
