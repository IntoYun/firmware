#ifndef __LIB_MIC_HAL_H_
#define __LIB_MIC_HAL_H_
#include "stdint.h"
//#include "application.h"
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


void MicClass_Start_Hal(mic_mems_t *p);
void MicClass_Stop_Hal();

#endif
