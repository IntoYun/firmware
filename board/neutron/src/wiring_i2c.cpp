/**
 ******************************************************************************
 * @file     : wiring_i2c.cpp
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

#include "wiring_i2c.h"
#include "wiring_usartserial.h"

#define TRANSMITTER             0x00
#define RECEIVER                0x01


/* I2C3 DMA1 I2C3_RX: stream2 channel3;    I2C3_TX: stream4 channel3 */
/* I2C1 DMA1 I2C1_RX: stream5 channel5;    I2C1_TX: stream6 channel1 */
/* I2C1: SCL PB8, SDA PB9 */
/* I2C3: SCL PA8, SDA PB4 */
static void I2C_DMA_Init(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C1)
    {
        static DMA_HandleTypeDef hdma_tx1;
        static DMA_HandleTypeDef hdma_rx1;

        GPIO_InitTypeDef  GPIO_InitStruct;

        /*##-1- Enable GPIO Clocks #################################################*/
        /* Enable GPIO Tx/Rx clock */
        __HAL_RCC_GPIOB_CLK_ENABLE(); // I2C PB8, PB9

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* I2C TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_8;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* I2C RX GPIO pin configuration  */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /*##-3- Enable DMA peripheral Clock ########################################*/
        /* Enable DMA1 clock */
        __HAL_RCC_DMA1_CLK_ENABLE();

        /*##-4- Configure the DMA streams ##########################################*/
        /* Configure the DMA handler for Transmission process */
        hdma_tx1.Instance                 = DMA1_Stream6;

        hdma_tx1.Init.Channel             = DMA_CHANNEL_1;
        hdma_tx1.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tx1.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tx1.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tx1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_tx1.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_tx1.Init.Mode                = DMA_NORMAL;
        hdma_tx1.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_tx1.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        hdma_tx1.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
        hdma_tx1.Init.MemBurst            = DMA_MBURST_INC4;
        hdma_tx1.Init.PeriphBurst         = DMA_PBURST_INC4;

        HAL_DMA_Init(&hdma_tx1);

        /* Associate the initialized DMA handle to the the I2C handle */
        __HAL_LINKDMA(hi2c, hdmatx, hdma_tx1);

        /* Configure the DMA handler for Transmission process */
        hdma_rx1.Instance                 = DMA1_Stream5;

        hdma_rx1.Init.Channel             = DMA_CHANNEL_1;
        hdma_rx1.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_rx1.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_rx1.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_rx1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_rx1.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_rx1.Init.Mode                = DMA_NORMAL;
        hdma_rx1.Init.Priority            = DMA_PRIORITY_HIGH;
        hdma_rx1.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        hdma_rx1.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
        hdma_rx1.Init.MemBurst            = DMA_MBURST_INC4;
        hdma_rx1.Init.PeriphBurst         = DMA_PBURST_INC4;

        HAL_DMA_Init(&hdma_rx1);

        /* Associate the initialized DMA handle to the the I2C handle */
        __HAL_LINKDMA(hi2c, hdmarx, hdma_rx1);

        /*##-5- Enable peripheral Clock ############################################*/
        /* Enable I2C1 clock */
        __HAL_RCC_I2C1_CLK_ENABLE();

        // /*##-6- Configure the NVIC for DMA #########################################*/
        // /* NVIC configuration for DMA transfer complete interrupt (I2C1_TX) */
        // HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 1, 0);
        // HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

        // /* NVIC configuration for DMA transfer complete interrupt (I2C1_RX) */
        // HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
        // HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
    }
    else if(hi2c->Instance == I2C3)
    {
        static DMA_HandleTypeDef hdma_tx3;
        static DMA_HandleTypeDef hdma_rx3;

        GPIO_InitTypeDef  GPIO_InitStruct;

        /*##-1- Enable GPIO Clocks #################################################*/
        /* Enable GPIO Tx/Rx clock */
        __HAL_RCC_GPIOA_CLK_ENABLE(); // I2C3 SCL PA8
        __HAL_RCC_GPIOB_CLK_ENABLE(); // I2C3 SDA PB4
        /*##-2- Configure peripheral GPIO ##########################################*/
        /* I2C TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_8;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* I2C RX GPIO pin configuration  */
        GPIO_InitStruct.Pin = GPIO_PIN_4;
        GPIO_InitStruct.Alternate = GPIO_AF9_I2C3;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /*##-3- Enable DMA peripheral Clock ########################################*/
        /* Enable DMA1 clock */
        __HAL_RCC_DMA1_CLK_ENABLE();

        /*##-4- Configure the DMA streams ##########################################*/
        /* Configure the DMA handler for Transmission process */
        hdma_tx3.Instance                 = DMA1_Stream4;

        hdma_tx3.Init.Channel             = DMA_CHANNEL_3;
        hdma_tx3.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tx3.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tx3.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tx3.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_tx3.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_tx3.Init.Mode                = DMA_NORMAL;
        hdma_tx3.Init.Priority            = DMA_PRIORITY_MEDIUM;
        hdma_tx3.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        hdma_tx3.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
        hdma_tx3.Init.MemBurst            = DMA_MBURST_INC4;
        hdma_tx3.Init.PeriphBurst         = DMA_PBURST_INC4;

        HAL_DMA_Init(&hdma_tx3);

        /* Associate the initialized DMA handle to the the I2C handle */
        __HAL_LINKDMA(hi2c, hdmatx, hdma_tx3);

        /* Configure the DMA handler for Transmission process */
        hdma_rx3.Instance                 = DMA1_Stream2;

        hdma_rx3.Init.Channel             = DMA_CHANNEL_3;
        hdma_rx3.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_rx3.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_rx3.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_rx3.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_rx3.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_rx3.Init.Mode                = DMA_NORMAL;
        hdma_rx3.Init.Priority            = DMA_PRIORITY_HIGH;  //DMA_PRIORITY_MEDIUM; //DMA_PRIORITY_HIGH;
        hdma_rx3.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        hdma_rx3.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
        hdma_rx3.Init.MemBurst            = DMA_MBURST_INC4;
        hdma_rx3.Init.PeriphBurst         = DMA_PBURST_INC4;

        HAL_DMA_Init(&hdma_rx3);

        /* Associate the initialized DMA handle to the the I2C handle */
        __HAL_LINKDMA(hi2c, hdmarx, hdma_rx3);

        /*##-5- Enable peripheral Clock ############################################*/
        /* Enable I2C1 clock */
        __HAL_RCC_I2C3_CLK_ENABLE();

        // /*##-6- Configure the NVIC for DMA #########################################*/
        // /* NVIC configuration for DMA transfer complete interrupt (I2C1_TX) */
        // HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 1, 0);
        // HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

        // /* NVIC configuration for DMA transfer complete interrupt (I2C1_RX) */
        // HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 0, 0);
        // HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
    }
}


TwoWire::TwoWire(I2C_TypeDef *I2Cx):I2C_Type(I2Cx)
{
    I2C_ClockSpeed = CLOCK_SPEED_100KHZ;
    I2C_NoStretchMode = I2C_NOSTRETCH_DISABLE;
    I2C_SetAsSlave = false;
    I2C_Enabled = false;
    I2C_Initialized = false;

    rxBufferIndex = 0;
    rxBufferLength = 0;

    txAddress = 0;
    txBufferIndex = 0;
    txBufferLength = 0;

    transmitting = 0;
}

void TwoWire::setSpeed(uint32_t clockSpeed)
{
    I2C_ClockSpeed = clockSpeed;
}

void TwoWire::stretchClock(bool stretch)
{
    if(stretch == true)
    {
        I2C_NoStretchMode = I2C_NOSTRETCH_DISABLE;
    }
    else
    {
        I2C_NoStretchMode = I2C_NOSTRETCH_ENABLE;
    }
}


void TwoWire::init(uint8_t Type)
{
    // if (I2C_Initialized)
    // {return;}

    // //read buffer
    // rxBufferIndex = 0;
    // rxBufferLength = 0;
    // //translate buffer
    // txBufferIndex = 0;
    // txBufferLength = 0;

    // I2CHandle.Instance = I2C_Type;
    // I2CHandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    // I2CHandle.Init.ClockSpeed = I2C_ClockSpeed;
    // I2CHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    // I2CHandle.Init.DutyCycle = I2C_DUTYCYCLE_2;
    // I2CHandle.Init.NoStretchMode = I2C_NoStretchMode;
    // // I2CHandle.Init.OwnAddress1 =
    // I2CHandle.Init.OwnAddress2 = 0xFE;

    // I2C_DMA_Init(&I2CHandle); // initialize the pin and DMA1

    // if(HAL_I2C_Init(&I2CHandle) != HAL_OK)
    // {
    //     /* Initialization Error */
    //     // Error_Handler();
    //     while(1)
    //     {}
    // }
    // I2C_Initialized = true;
}

#if 1
void TwoWire::begin(void)
{
    if (I2C_Enabled)
        return;
    I2C_SetAsSlave = false;

    I2CHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    I2CHandle.Init.ClockSpeed      = 400000;
    I2CHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    I2CHandle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
    I2CHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    I2CHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    I2CHandle.Init.OwnAddress1     = 0x00;
    I2CHandle.Init.OwnAddress2     = 0x00;
    GPIO_InitTypeDef  GPIO_InitStruct;

    if (I2C_Type == I2C1)
    {

        I2CHandle.Instance = I2C1;

        /*##-1- Enable GPIO Clocks #################################################*/
        /* Enable GPIO TX/RX clock */
        __HAL_RCC_GPIOB_CLK_ENABLE(); // I2C PB8, PB9
        __HAL_RCC_I2C1_CLK_ENABLE();

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* I2C TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_8;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* I2C RX GPIO pin configuration  */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    }
    else if (I2C_Type == I2C3)
    {
        I2CHandle.Instance = I2C3;

        /*##-1- Enable GPIO Clocks #################################################*/
        /* Enable GPIO TX/RX clock */
        __HAL_RCC_GPIOA_CLK_ENABLE(); // I2C SCL tx PA8
        __HAL_RCC_GPIOB_CLK_ENABLE(); // I2C SDA rx PB4
        __HAL_RCC_I2C3_CLK_ENABLE();


        /*##-2- Configure peripheral GPIO ##########################################*/
        /* I2C TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_8;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* I2C RX GPIO pin configuration  */
        GPIO_InitStruct.Pin = GPIO_PIN_4;
        GPIO_InitStruct.Alternate = GPIO_AF9_I2C3;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }

    if(HAL_I2C_Init(&I2CHandle) != HAL_OK)
    {
        /* Initialization Error */
        //Error_Handler();
        while(1)
        {}
    }

    I2C_Enabled = true;
}
#endif

void TwoWire::begin(uint8_t address)
{
    I2C_SetAsSlave = true;
    I2CHandle.Init.OwnAddress1 = address;

    init(1);
    I2C_Enabled = true;
}

void TwoWire::begin(int address)
{
    begin((uint8_t)address);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
    if(quantity > I2C_BUFFER_LENGTH)
    {
        quantity = I2C_BUFFER_LENGTH;
    }

    address = address << 1;
    // address |= 0x01;
#if(useDMA)
    HAL_StatusTypeDef requestStatus = HAL_I2C_Master_Receive_DMA(&I2CHandle, address, rxBuffer, quantity);
    for ( int i=0; i < 10; ++i )
    {
        Serial.printf("\r\nrequestStatus %d!\r\n", requestStatus);
    }
    while( requestStatus != HAL_OK);
#else
    while(HAL_I2C_Master_Receive(&I2CHandle, address, rxBuffer, quantity, 100) != HAL_OK)
#endif
    {
        /* Error_handler() function is called when Timeout error occurs. */
        if(HAL_I2C_GetError(&I2CHandle) != HAL_I2C_ERROR_AF)
        {
            // Error_Handler();
            while(1)
            {}
        }
    }

    rxBufferIndex = 0;
    rxBufferLength = quantity;

    return rxBufferLength;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

void TwoWire::beginTransmission(uint8_t address)
{
    // indicate that we are transmitting
    transmitting = 1;
    // set address of targeted slave
    txAddress = address << 1;

    txBufferIndex = 0;
    txBufferLength = 0;
}

void TwoWire::beginTransmission(int address)
{
    beginTransmission((uint8_t)address);
}

uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
#if(useDMA)
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit_DMA( &I2CHandle, (uint16_t) txAddress, (uint8_t*)txBuffer, txBufferLength);
    while( status != HAL_OK);
#else
    while(HAL_I2C_Master_Transmit( &I2CHandle, (uint16_t)txAddress, &txBuffer[0], txBufferLength, 1000) != HAL_OK)
#endif
    {
        if( HAL_I2C_GetError(&I2CHandle) != HAL_I2C_ERROR_AF)
        {
            Serial.printf("\r\n12345nendTransmission!\r\n");

            while(1)
            {}
            // Error_Handler();
        }
        for ( int i=0; i < 10; ++i )
        {
            Serial.printf("\r\nabcdefghidkalldjflaj!\r\n");
        }
    }

    // reset tx buffer iterator vars
    txBufferIndex = 0;
    txBufferLength = 0;

    // indicate that we are done transmitting
    transmitting = 0;

    while (HAL_I2C_GetState(&I2CHandle) != HAL_I2C_STATE_READY)
    {
        HAL_I2C_StateTypeDef a = HAL_I2C_GetState(&I2CHandle);

        Serial.printf("\r\n%d\r\n",a);
    }
    return 0;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission(void)
{
    return endTransmission(true);
}

size_t TwoWire::write(uint8_t data)
{
    //if(transmitting)
    {
        // in master/slave transmitter mode
        // don't bother if buffer is full
        if(txBufferLength >= I2C_BUFFER_LENGTH)
        {
            // setWriteError();
            return 0;
        }
        // put byte in tx buffer
        txBuffer[txBufferIndex++] = data;
        // update amount in buffer
        txBufferLength = txBufferIndex;
    }
    if(Serial.isEnabled() == false)
    {
        Serial.begin();
    }

    return 1;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
    //if(transmitting)
    {
        // in master/slave transmitter mode
        for(size_t i = 0; i < quantity; ++i)
        {
            write(data[i]);
        }
    }
    return quantity;
}


int TwoWire::available(void)
{
    return rxBufferLength - rxBufferIndex;
}

int TwoWire::read(void)
{
    int value = -1;

    // get each successive byte on each call
    if(rxBufferIndex < rxBufferLength)
    {
        value = rxBuffer[rxBufferIndex++];
    }
    return value;
}

int TwoWire::peek(void)
{
    int value = -1;

    if(rxBufferIndex < rxBufferLength)
    {
        value = rxBuffer[rxBufferIndex];
    }
    return value;
}

void TwoWire::flush(void)
{
    // XXX: to be implemented.
}



bool TwoWire::isEnabled()
{
    return I2C_Enabled;
}

// #if I2C_INTERFACES_COUNT > 0
TwoWire Wire1 = TwoWire(I2C1); // for internal use
// #endif

//#if I2C_INTERFACES_COUNT > 1
TwoWire Wire = TwoWire(I2C3); // for the users
//#endif
