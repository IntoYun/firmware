/**
 ******************************************************************************
 * @file    spi_hal.c
 * @author  Matthew McGowan
 * @version V1.0.0
 * @date    12-Sept-2014
 * @brief
 ******************************************************************************
  Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

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

/* Includes ------------------------------------------------------------------*/
#include "spi_hal.h"
#include "pinmap_impl.h"
#include "Arduino_Nut.h"
#include "service_debug.h"

#define SPI_HAS_TRANSACTION

// This defines are not representing the real Divider of the ESP8266
// the Defines match to an AVR Arduino on 16MHz for better compatibility
#define SPI_CLOCK_DIV2 		0x00101001 //8 MHz
#define SPI_CLOCK_DIV4 		0x00241001 //4 MHz
#define SPI_CLOCK_DIV8 		0x004c1001 //2 MHz
#define SPI_CLOCK_DIV16 	0x009c1001 //1 MHz
#define SPI_CLOCK_DIV32 	0x013c1001 //500 KHz
#define SPI_CLOCK_DIV64 	0x027c1001 //250 KHz
#define SPI_CLOCK_DIV128 	0x04fc1001 //125 KHz

//const uint8_t SPI_MODE0 = 0x00; ///<  CPOL: 0  CPHA: 0
//const uint8_t SPI_MODE1 = 0x01; ///<  CPOL: 0  CPHA: 1
//const uint8_t SPI_MODE2 = 0x10; ///<  CPOL: 1  CPHA: 0
//const uint8_t SPI_MODE3 = 0x11; ///<  CPOL: 1  CPHA: 1

typedef union {
        uint32_t regValue;
        struct {
                unsigned regL :6;
                unsigned regH :6;
                unsigned regN :6;
                unsigned regPre :13;
                unsigned regEQU :1;
        };
} spiClk_t;

bool useHwCs;
void writeBytes_(uint8_t * data, uint8_t size);
void transferBytes_(uint8_t * out, uint8_t * in, uint8_t size);
inline void setDataBits(uint16_t bits);

void setHwCs(bool use) {
    EESP82666_Pin_Info* PIN_MAP = HAL_Pin_Map();
    unsigned char ss_pin = PIN_MAP[SS].gpio_pin;
    if(use) {
        /* __pinMode(ss_pin, SPECIAL); ///< GPIO15 */
        GPC(ss_pin) = (GPC(ss_pin) & (0xF << GPCI)); //SOURCE(GPIO) | DRIVER(NORMAL) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)
        GPEC = (1 << ss_pin); //Disable
        GPF(ss_pin) = GPFFS(GPFFS_BUS(ss_pin));//Set mode to BUS (RX0, TX0, TX1, SPI, HSPI or CLK depending in the pin)

        SPI1U |= (SPIUCSSETUP | SPIUCSHOLD);
    } else {
        if(useHwCs) {
            /* __pinMode(ss_pin, INPUT); */
            GPF(ss_pin) = GPFFS(GPFFS_GPIO(ss_pin));//Set mode to GPIO
            GPEC = (1 << ss_pin); //Disable
            GPC(ss_pin) = (GPC(ss_pin) & (0xF << GPCI)) | (1 << GPCD); //SOURCE(GPIO) | DRIVER(OPEN_DRAIN) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)

            SPI1U &= ~(SPIUCSSETUP | SPIUCSHOLD);
        }
    }
    useHwCs = use;
}

/**
 * calculate the Frequency based on the register value
 * @param reg
 * @return
 */
static uint32_t ClkRegToFreq(spiClk_t * reg) {
    return (ESP8266_CLOCK / ((reg->regPre + 1) * (reg->regN + 1)));
}

void setClockDivider(uint32_t clockDiv) {
    if(clockDiv == 0x80000000) {
        GPMUX |= (1 << 9); // Set bit 9 if sysclock required
    } else {
        GPMUX &= ~(1 << 9);
    }
    SPI1CLK = clockDiv;
}

void setFrequency(uint32_t freq) {
    static uint32_t lastSetFrequency = 0;
    static uint32_t lastSetRegister = 0;

    if(freq >= ESP8266_CLOCK) {
        setClockDivider(0x80000000);
        return;
    }

    if(lastSetFrequency == freq && lastSetRegister == SPI1CLK) {
        // do nothing (speed optimization)
        return;
    }

    const spiClk_t minFreqReg = { 0x7FFFF000 };
    uint32_t minFreq = ClkRegToFreq((spiClk_t*) &minFreqReg);
    if(freq < minFreq) {
        // use minimum possible clock
        setClockDivider(minFreqReg.regValue);
        lastSetRegister = SPI1CLK;
        lastSetFrequency = freq;
        return;
    }

    uint8_t calN = 1;

    spiClk_t bestReg = { 0 };
    int32_t bestFreq = 0;

    // find the best match
    while(calN <= 0x3F) { // 0x3F max for N

        spiClk_t reg = { 0 };
        int32_t calFreq;
        int32_t calPre;
        int8_t calPreVari = -2;

        reg.regN = calN;

        while(calPreVari++ <= 1) { // test different variants for Pre (we calculate in int so we miss the decimals, testing is the easyest and fastest way)
            calPre = (((ESP8266_CLOCK / (reg.regN + 1)) / freq) - 1) + calPreVari;
            if(calPre > 0x1FFF) {
                reg.regPre = 0x1FFF; // 8191
            } else if(calPre <= 0) {
                reg.regPre = 0;
            } else {
                reg.regPre = calPre;
            }

            reg.regL = ((reg.regN + 1) / 2);
            // reg.regH = (reg.regN - reg.regL);

            // test calculation
            calFreq = ClkRegToFreq(&reg);
            //os_printf("-----[0x%08X][%d]\t EQU: %d\t Pre: %d\t N: %d\t H: %d\t L: %d = %d\n", reg.regValue, freq, reg.regEQU, reg.regPre, reg.regN, reg.regH, reg.regL, calFreq);

            if(calFreq == (int32_t) freq) {
                // accurate match use it!
                memcpy(&bestReg, &reg, sizeof(bestReg));
                break;
            } else if(calFreq < (int32_t) freq) {
                // never go over the requested frequency
                if(abs(freq - calFreq) < abs(freq - bestFreq)) {
                    bestFreq = calFreq;
                    memcpy(&bestReg, &reg, sizeof(bestReg));
                }
            }
        }
        if(calFreq == (int32_t) freq) {
            // accurate match use it!
            break;
        }
        calN++;
    }

    // os_printf("[0x%08X][%d]\t EQU: %d\t Pre: %d\t N: %d\t H: %d\t L: %d\t - Real Frequency: %d\n", bestReg.regValue, freq, bestReg.regEQU, bestReg.regPre, bestReg.regN, bestReg.regH, bestReg.regL, ClkRegToFreq(&bestReg));

    setClockDivider(bestReg.regValue);
    lastSetRegister = SPI1CLK;
    lastSetFrequency = freq;

}

void setBitOrder(uint8_t bitOrder)
{
    if(bitOrder == MSBFIRST) {
        SPI1C &= ~(SPICWBO | SPICRBO);
    } else {
        SPI1C |= (SPICWBO | SPICRBO);
    }

}

void setDataMode(uint8_t dataMode)
{
    /**
     SPI_MODE0 0x00 - CPOL: 0  CPHA: 0
     SPI_MODE1 0x01 - CPOL: 0  CPHA: 1
     SPI_MODE2 0x10 - CPOL: 1  CPHA: 0
     SPI_MODE3 0x11 - CPOL: 1  CPHA: 1
     */
    bool CPOL = (dataMode & 0x10); ///< CPOL (Clock Polarity)
    bool CPHA = (dataMode & 0x01); ///< CPHA (Clock Phase)

    if(CPHA) {
        SPI1U |= (SPIUSME);
    } else {
        SPI1U &= ~(SPIUSME);
    }

    if(CPOL) {
        SPI1P |= 1<<29;
    } else {
        SPI1P &= ~(1<<29);
        //todo test whether it is correct to set CPOL like this.
    }
}


inline void setDataBits(uint16_t bits) {
    const uint32_t mask = ~((SPIMMOSI << SPILMOSI) | (SPIMMISO << SPILMISO));
    bits--;
    SPI1U1 = ((SPI1U1 & mask) | ((bits << SPILMOSI) | (bits << SPILMISO)));
}

uint8_t transfer(uint8_t data) {
    while(SPI1CMD & SPIBUSY) {}
    // reset to 8Bit mode
    setDataBits(8);
    SPI1W0 = data;
    SPI1CMD |= SPIBUSY;
    while(SPI1CMD & SPIBUSY) {}
    return (uint8_t) (SPI1W0 & 0xff);
}

void HAL_SPI_Initial(HAL_SPI_Interface spi)
{
    useHwCs = false;
}

void HAL_SPI_Begin(HAL_SPI_Interface spi, uint16_t pin)
{
    /* DEBUG("Enter HAL_SPI_Begin..."); */
    EESP82666_Pin_Info* PIN_MAP = HAL_Pin_Map();
    unsigned char sck_pin = PIN_MAP[SCK].gpio_pin;
    unsigned char miso_pin = PIN_MAP[MISO].gpio_pin;
    unsigned char mosi_pin = PIN_MAP[MOSI].gpio_pin;

    /* __pinMode(sck_pin, SPECIAL); */
    GPC(sck_pin) = (GPC(sck_pin) & (0xF << GPCI)); //SOURCE(GPIO) | DRIVER(NORMAL) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)
    GPEC = (1 << sck_pin); //Disable
    GPF(sck_pin) = GPFFS(GPFFS_BUS(sck_pin));//Set mode to BUS (RX0, TX0, TX1, SPI, HSPI or CLK depending in the pin)

    /* __pinMode(miso_pin, SPECIAL); */
    GPC(sck_pin) = (GPC(sck_pin) & (0xF << GPCI)); //SOURCE(GPIO) | DRIVER(NORMAL) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)
    GPEC = (1 << sck_pin); //Disable
    GPF(sck_pin) = GPFFS(GPFFS_BUS(sck_pin));//Set mode to BUS (RX0, TX0, TX1, SPI, HSPI or CLK depending in the pin)

    /* __pinMode(mosi_pin, SPECIAL); */
    GPC(mosi_pin) = (GPC(mosi_pin) & (0xF << GPCI)); //SOURCE(GPIO) | DRIVER(NORMAL) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)
    GPEC = (1 << mosi_pin); //Disable
    GPF(mosi_pin) = GPFFS(GPFFS_BUS(mosi_pin));//Set mode to BUS (RX0, TX0, TX1, SPI, HSPI or CLK depending in the pin)

    SPI1C = 0;
    setFrequency(1000000); ///< 1MHz
    /* setBitOrder(LSBFIRST); */
    setBitOrder(MSBFIRST); // Adafruit need MSBFIRST
    setDataMode(SPI_MODE0);
    SPI1U = SPIUMOSI | SPIUDUPLEX | SPIUSSE;
    SPI1U1 = (7 << SPILMOSI) | (7 << SPILMISO);
    SPI1C1 = 0;
}

void HAL_SPI_End(HAL_SPI_Interface spi)
{
    /* DEBUG("Enter HAL_SPI_End..."); */
    EESP82666_Pin_Info* PIN_MAP = HAL_Pin_Map();
    unsigned char sck_pin = PIN_MAP[SCK].gpio_pin;
    unsigned char miso_pin = PIN_MAP[MISO].gpio_pin;
    unsigned char mosi_pin = PIN_MAP[MOSI].gpio_pin;
    unsigned char ss_pin = PIN_MAP[SS].gpio_pin;

    /* __pinMode(sck_pin, INPUT);  ///< GPIO14 */
    GPF(sck_pin) = GPFFS(GPFFS_GPIO(sck_pin));//Set mode to GPIO
    GPEC = (1 << sck_pin); //Disable
    GPC(sck_pin) = (GPC(sck_pin) & (0xF << GPCI)) | (1 << GPCD); //SOURCE(GPIO) | DRIVER(OPEN_DRAIN) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)

    /* __pinMode(miso_pin, INPUT); ///< GPIO12 */
    GPF(miso_pin) = GPFFS(GPFFS_GPIO(miso_pin));//Set mode to GPIO
    GPEC = (1 << miso_pin); //Disable
    GPC(miso_pin) = (GPC(miso_pin) & (0xF << GPCI)) | (1 << GPCD); //SOURCE(GPIO) | DRIVER(OPEN_DRAIN) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)

    /* __pinMode(mosi_pin, INPUT); ///< GPIO13 */
    GPF(miso_pin) = GPFFS(GPFFS_GPIO(miso_pin));//Set mode to GPIO
    GPEC = (1 << miso_pin); //Disable
    GPC(miso_pin) = (GPC(miso_pin) & (0xF << GPCI)) | (1 << GPCD); //SOURCE(GPIO) | DRIVER(OPEN_DRAIN) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)

    if(useHwCs) {
        /* __pinMode(ss_pin, INPUT); ///< GPIO13 */
        GPF(ss_pin) = GPFFS(GPFFS_GPIO(ss_pin));//Set mode to GPIO
        GPEC = (1 << ss_pin); //Disable
        GPC(ss_pin) = (GPC(ss_pin) & (0xF << GPCI)) | (1 << GPCD); //SOURCE(GPIO) | DRIVER(OPEN_DRAIN) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)

    }
}

void HAL_SPI_Set_Bit_Order(HAL_SPI_Interface spi, uint8_t bitOrder)
{
    /* DEBUG("Enter HAL_SPI_Set_Bit_Order..."); */
    setBitOrder(bitOrder);
}

void HAL_SPI_Set_Data_Mode(HAL_SPI_Interface spi, uint8_t dataMode)
{
    /* DEBUG("Enter HAL_SPI_Set_Data_Mode..."); */
    setDataMode(dataMode);
}

void HAL_SPI_Set_Clock_Divider(HAL_SPI_Interface spi, uint8_t rate)
{
    /* DEBUG("Enter HAL_SPI_Set_Clock_Mode..."); */
    setClockDivider(rate);
}

uint16_t HAL_SPI_Send_Receive_Data(HAL_SPI_Interface spi, uint16_t data)
{
    /* DEBUG("Enter HAL_SPI_Send_Receive_Mode..."); */
    return transfer((uint8_t)data);
}

bool HAL_SPI_Is_Enabled(HAL_SPI_Interface spi)
{
    return false;
}

void HAL_SPI_DMA_Transfer(HAL_SPI_Interface spi, void* tx_buffer, void* rx_buffer, uint32_t length, HAL_SPI_DMA_UserCallback userCallback)
{
}

void HAL_SPI_Info(HAL_SPI_Interface spi, hal_spi_info_t* info, void* reserved)
{
}

void HAL_SPI_Set_Callback_On_Select(HAL_SPI_Interface spi, HAL_SPI_Select_UserCallback cb, void* reserved)
{
}

void HAL_SPI_DMA_Transfer_Cancel(HAL_SPI_Interface spi)
{
}

int32_t HAL_SPI_DMA_Transfer_Status(HAL_SPI_Interface spi, HAL_SPI_TransferStatus* st)
{
    return 0;
}
