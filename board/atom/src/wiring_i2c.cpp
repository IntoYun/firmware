/**
 ******************************************************************************
 * @file     : wiring_i2c.cpp
 * @author   : robin
 * @version	 : V1.0.0
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


#define TRANSMITTER             0x00
#define RECEIVER                0x01


/*********************************************************************************
  *Function     : void Wire_DMAConfig(I2C_TypeDef *I2Cx, uint8_t *pBuffer, uint32_t BufferSize, uint32_t Direction)
  *Description  : Initializes DMA channel used by the I2C peripheral based on Direction
  *Input        : I2Cx:I2C1 or I2C2
  				  pBuffer:The data buffer
  				  BufferSize:Data size
  				  Direction:Transfer direction
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       :         
**********************************************************************************/
void Wire_DMAConfig(I2C_TypeDef *I2Cx, uint8_t *pBuffer, uint32_t BufferSize, uint32_t Direction)
{
    DMA_InitTypeDef  DMA_InitStructure;

    /* Configure the DMA Tx/Rx Channel with the buffer address and the buffer size */
    if(I2Cx == I2C1)
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)0x40005410;

    if(I2Cx == I2C2)
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)0x40005810;

    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pBuffer;
    DMA_InitStructure.DMA_BufferSize = (uint32_t)BufferSize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    if (Direction == TRANSMITTER)
    {
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
        if(I2Cx == I2C1)
        {
            /* Disable DMA TX Channel */
            DMA_Cmd(DMA1_Channel6, DISABLE);
            /* DMA1 channel6 configuration */
            DMA_Init(DMA1_Channel6, &DMA_InitStructure);
        }

        if(I2Cx == I2C2)
        {
            /* Disable DMA TX Channel */
            DMA_Cmd(DMA1_Channel4, DISABLE);
            /* DMA1 channel6 configuration */
            DMA_Init(DMA1_Channel4, &DMA_InitStructure);
        }
    }
    else
    {
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
        if(I2Cx == I2C1)
        {
            /* Disable DMA RX Channel */
            DMA_Cmd(DMA1_Channel7, DISABLE);
            /* DMA1 channel7 configuration */
            DMA_Init(DMA1_Channel7, &DMA_InitStructure);
        }
        if(I2Cx == I2C2)
        {
            /* Disable DMA RX Channel */
            DMA_Cmd(DMA1_Channel5, DISABLE);
            /* DMA1 channel7 configuration */
            DMA_Init(DMA1_Channel5, &DMA_InitStructure);
        }
    }
}

/*********************************************************************************
  *Function     : TwoWire::TwoWire(I2C_TypeDef *_twi, void(*_initCb)(uint8_t)) :I2C_Type(_twi), initCb(_initCb) {
  *Description  : Initializes 
  *Input        : _twi:I2C1 or I2C2
  				  _initCb:I2C Initial call back
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       :         
**********************************************************************************/
TwoWire::TwoWire(I2C_TypeDef *_twi, void(*_initCb)(uint8_t)) :I2C_Type(_twi), initCb(_initCb) 
{
    I2C_ClockSpeed = CLOCK_SPEED_100KHZ;
    I2C_SetAsSlave = false;
    I2C_Enabled = false;
    I2C_Initialized = false;

    rxBufferIndex = 0;
    rxBufferLength = 0;

    txAddress = 0;
    txBufferIndex = 0;
    txBufferLength = 0;
    txBufferLength = 0;

    transmitting = 0;
}

/*********************************************************************************
  *Function     : void TwoWire::setSpeed(uint32_t clockSpeed)
  *Description  : set I2C clock speed 
  *Input        : clockSpeed:I2C Clock speed
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       : setSpeed() should be called before begin() else default to 100KHz
**********************************************************************************/
void TwoWire::setSpeed(uint32_t clockSpeed)
{
    I2C_ClockSpeed = clockSpeed;
}

/*********************************************************************************
  *Function     : void TwoWire::stretchClock(bool stretch)
  *Description  : I2C clock stretching 
  *Input        : stretch:ENABLE or DISABLE
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       :        
**********************************************************************************/
void TwoWire::stretchClock(bool stretch)
{
    if(stretch == true)
    {
        I2C_StretchClockCmd(I2C_Type, ENABLE);
    }
    else
    {
        I2C_StretchClockCmd(I2C_Type, DISABLE);
    }
}


/*********************************************************************************
  *Function     : void TwoWire::init(uint8_t Type) 
  *Description  : Initialize I2C
  *Input        : Type: 1 or 0
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       :        
**********************************************************************************/
void TwoWire::init(uint8_t Type) 
{
    if (I2C_Initialized)
    {return;}

    //read buffer
    rxBufferIndex = 0;
    rxBufferLength = 0;
    //translate buffer
    txBufferIndex = 0;
    txBufferLength = 0;

    if(initCb)
    {initCb(Type);}

    I2C_DeInit(I2C_Type);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C_ClockSpeed;
    I2C_Init(I2C_Type, &I2C_InitStructure);
    I2C_Cmd(I2C_Type, ENABLE);
    I2C_Initialized = true;
}

/*********************************************************************************
  *Function     : void TwoWire::begin(void)
  *Description  : I2C begin
  *Input        : none
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       : If you want to use I2C call this function to initialize
**********************************************************************************/
void TwoWire::begin(void)
{
    I2C_SetAsSlave = false;
    I2C_InitStructure.I2C_OwnAddress1=0x00;
    //初始化  主机模式
    init(0);
    I2C_Enabled = true;
}

/*********************************************************************************
  *Function     : void TwoWire::begin(void)
  *Description  : I2C begin
  *Input        : address:set I2C slave address
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       : If you want to use I2C call this function to initialize
**********************************************************************************/
void TwoWire::begin(uint8_t address)
{
    I2C_SetAsSlave = true;
    I2C_InitStructure.I2C_OwnAddress1 = address << 1;
    init(1);
    I2C_Enabled = true;
}

/*********************************************************************************
  *Function     : void TwoWire::begin(void)
  *Description  : I2C begin
  *Input        : address:set I2C slave address
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       : If you want to use I2C call this function to initialize
**********************************************************************************/
void TwoWire::begin(int address)
{
    begin((uint8_t)address);
}

/*********************************************************************************
  *Function     : uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
  *Description  : I2C Host read data
  *Input        : address:set I2C slave address
  				  quantity:data length
  				  sendStop:1 or 0 Whether to send a stop bit
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       :         
**********************************************************************************/
uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
    system_tick_t _millis;

    // clamp to buffer length
    if(quantity > BUFFER_LENGTH)
    {
        quantity = BUFFER_LENGTH;
    }

    /* Send START condition */
    I2C_GenerateSTART(I2C_Type, ENABLE);

    _millis = millis();
    while(!I2C_CheckEvent(I2C_Type, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if(EVENT_TIMEOUT < (millis() - _millis)) return 0;
    }

    /* Send Slave address for read */
    I2C_Send7bitAddress(I2C_Type, address, I2C_Direction_Receiver);

    _millis = millis();
    while(!I2C_CheckEvent(I2C_Type, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if(EVENT_TIMEOUT < (millis() - _millis)) return 0;
    }

	if(quantity == 1)
	{
			/* Disable Acknowledgement */
			I2C_AcknowledgeConfig(I2C_Type, DISABLE); // 一个字节无应答

			/* Send STOP Condition */
			I2C_GenerateSTOP(I2C_Type, ENABLE); 

		    while(!I2C_CheckEvent(I2C_Type, I2C_EVENT_MASTER_BYTE_RECEIVED)) /* Test on EV7 and clear it */
		    {
		        if(EVENT_TIMEOUT < (millis() - _millis)) return 0;
		    }
			/* Read a byte from the EEPROM */
			rxBuffer[0] = I2C_ReceiveData(I2C_Type);

			rxBufferIndex = 0;
			rxBufferLength = 1;// LZ
			/* Enable Acknowledgement to be ready for another reception */
			I2C_AcknowledgeConfig(I2C_Type, ENABLE);
			return rxBufferLength;
	}
	else
	{
    	//Wire_DMAConfig(rxBuffer, quantity, RECEIVER);
   	 	Wire_DMAConfig(I2C_Type,rxBuffer, quantity, RECEIVER); // lz

	    /* Enable DMA NACK automatic generation */
	    I2C_DMALastTransferCmd(I2C_Type, ENABLE);

	    /* Enable I2C DMA request */
	    I2C_DMACmd(I2C_Type, ENABLE);

	    /* Enable DMA RX Channel */
	    //DMA_Cmd(DMA1_Channel7, ENABLE);
	    DMA_Control(I2C_Type,RECEIVER,ENABLE); // lz
	    /* Wait until DMA Transfer Complete */

	    _millis = millis();

	    // while(!DMA_GetFlagStatus(DMA1_FLAG_TC7))
	    while(!GetDMAFlagStatus(I2C_Type,RECEIVER)) // LZ
	    {
	        if(EVENT_TIMEOUT < (millis() - _millis)) break;
	    }

	    /* Disable DMA RX Channel */
	    //DMA_Cmd(DMA1_Channel7, DISABLE);
	    DMA_Control(I2C_Type,RECEIVER,DISABLE); // lz

	    /* Disable I2C DMA request */
	    I2C_DMACmd(I2C_Type, DISABLE);

	    /* Clear DMA RX Transfer Complete Flag */

	    //DMA_ClearFlag(DMA1_FLAG_TC7);
	    ClearDMAFlag(I2C_Type,RECEIVER);//LZ

	    /* Send STOP Condition */
	    if(sendStop == true)
	    {
	    /* Send STOP condition */
	    I2C_GenerateSTOP(I2C_Type, ENABLE);
	    }

	    /* Enable Acknowledgement to be ready for another reception */
	    I2C_AcknowledgeConfig(I2C_Type, ENABLE);

	    // set rx buffer iterator vars
	    rxBufferIndex = 0;

	    // rxBufferLength = quantity - DMA_GetCurrDataCounter(DMA1_Channel7);
	    rxBufferLength = quantity - GetDMACurrDataCounter(I2C_Type); // LZ
	    return rxBufferLength;
    }
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

/*********************************************************************************
  *Function     : void TwoWire::beginTransmission(uint8_t address)
  *Description  : I2C transmission initialize
  *Input        : address:set I2C slave address
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       :         
**********************************************************************************/
void TwoWire::beginTransmission(uint8_t address)
{
    // indicate that we are transmitting
    transmitting = 1;
    // set address of targeted slave
    txAddress = address;
    // reset tx buffer iterator vars
    txBufferIndex = 0;
    txBufferLength = 0;
}

void TwoWire::beginTransmission(int address)
{
    beginTransmission((uint8_t)address);
}

/*********************************************************************************
  *Function     : uint8_t TwoWire::endTransmission(uint8_t sendStop)
  *Description  : I2C host send data to slave 
  *Input        : sendStop: 1 or 0 whether to send a stop bit
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       :         
**********************************************************************************/
uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
    uint32_t _millis;

    /* Send START condition */
    I2C_GenerateSTART(I2C_Type, ENABLE);

    _millis = millis();
    while(!I2C_CheckEvent(I2C_Type, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if(EVENT_TIMEOUT < (millis() - _millis)) return 4;
    }

    /* Send Slave address for write */
    I2C_Send7bitAddress(I2C_Type, txAddress, I2C_Direction_Transmitter);

    _millis = millis();
    while(!I2C_CheckEvent(I2C_Type, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if(EVENT_TIMEOUT < (millis() - _millis)) return 4;
    }

    //Wire_DMAConfig(txBuffer, txBufferLength+1, TRANSMITTER);
    Wire_DMAConfig(I2C_Type,txBuffer, txBufferLength+1, TRANSMITTER);

    /* Enable DMA NACK automatic generation */
    I2C_DMALastTransferCmd(I2C_Type, ENABLE);

    /* Enable I2C DMA request */
    I2C_DMACmd(I2C_Type, ENABLE);

    /* Enable DMA TX Channel */
    // DMA_Cmd(DMA1_Channel6, ENABLE);
    DMA_Control(I2C_Type,TRANSMITTER,ENABLE); // lz

    /* Wait until DMA Transfer Complete */
    _millis = millis();

    //while(!DMA_GetFlagStatus(DMA1_FLAG_TC6))
    while(!GetDMAFlagStatus(I2C_Type,TRANSMITTER)) // LZ
    {
        if(EVENT_TIMEOUT < (millis() - _millis)) return 4;
    }

    /* Disable DMA TX Channel */
    //DMA_Cmd(DMA1_Channel6, DISABLE);
    DMA_Control(I2C_Type,TRANSMITTER,DISABLE); // lz

    /* Disable I2C DMA request */
    I2C_DMACmd(I2C_Type, DISABLE);

    /* Clear DMA TX Transfer Complete Flag */

    // DMA_ClearFlag(DMA1_FLAG_TC6);
    ClearDMAFlag(I2C_Type,TRANSMITTER);//LZ

    /* Send STOP Condition */
    if(sendStop == true)
    {
        /* Send STOP condition */
        I2C_GenerateSTOP(I2C_Type, ENABLE);
    }

    // reset tx buffer iterator vars
    txBufferIndex = 0;
    txBufferLength = 0;

    // indicate that we are done transmitting
    transmitting = 0;

    return 0;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission(void)
{
    return endTransmission(true);
}

/*********************************************************************************
  *Function     : size_t TwoWire::write(uint8_t data)
  *Description  : Send the data to the buffer 
  *Input        : data: Need to send data
  *Output       : none
  *Return       : return status 1:ok 0:error
  *author       : lz
  *date         : 6-December-2014
  *Others       : must be called in:slave tx event callback or after beginTransmission(address)
**********************************************************************************/
size_t TwoWire::write(uint8_t data)
{
    if(transmitting)
    {
        // in master/slave transmitter mode
        // don't bother if buffer is full
        if(txBufferLength >= BUFFER_LENGTH)
        {
            setWriteError();
            return 0;
            }
        // put byte in tx buffer
        txBuffer[txBufferIndex++] = data;
        // update amount in buffer
        txBufferLength = txBufferIndex;
    }
    return 1;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
    if(transmitting)
    {
        // in master/slave transmitter mode
        for(size_t i = 0; i < quantity; ++i)
        {
            write(data[i]);
        }
    }
    return quantity;
}


/*********************************************************************************
  *Function     : int TwoWire::available(void)
  *Description  : Effective length of the data
  *Input        : none
  *Output       : none
  *Return       : return data lenght
  *author       : lz
  *date         : 6-December-2014
  *Others       : must be called in:slave tx event callback or after requestFrom(address, numBytes)
**********************************************************************************/
int TwoWire::available(void)
{
    return rxBufferLength - rxBufferIndex;
}

/*********************************************************************************
  *Function     : int TwoWire::read(void)
  *Description  : I2C Host read a byte data
  *Input        : none
  *Output       : none
  *Return       : return a byte data 
  *author       : lz
  *date         : 6-December-2014
  *Others       : must be called in:slave tx event callback or after requestFrom(address, numBytes)
**********************************************************************************/
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

/*********************************************************************************
  *Function     : int TwoWire::peek(void)
  *Description  : sets function called on slave write
  *Input        : none
  *Output       : none
  *Return       : none 
  *author       : lz
  *date         : 6-December-2014
  *Others       : must be called in:slave tx event callback or after requestFrom(address, numBytes)
**********************************************************************************/
void TwoWire::onReceive( void (*function)(int) )
{
    user_onReceive = function;
}

// sets function called on slave read
void TwoWire::onRequest( void (*function)(void) )
{
    user_onRequest = function;
}

/*********************************************************************************
  *Function     : void TwoWire::slaveEventHandler(void)
  *Description  : Treatment from the press event
  *Input        : none
  *Output       : none
  *Return       : none 
  *author       : lz
  *date         : 6-December-2014
  *Others       :        
**********************************************************************************/
void TwoWire::slaveEventHandler(void)
{
    __IO uint32_t SR1Register = 0;
    __IO uint32_t SR2Register = 0;

    /* Read the I2C1 SR1 and SR2 status registers */
    SR1Register = I2C_Type->SR1;
    SR2Register = I2C_Type->SR2;

    /* If I2C1 is slave (MSL flag = 0) */
    if ((SR2Register &0x0001) != 0x0001)
    {
        /* If ADDR = 1: EV1 */
        if ((SR1Register & 0x0002) == 0x0002)
        {
            /* Clear SR1Register and SR2Register variables to prepare for next IT */
            SR1Register = 0;
            SR2Register = 0;

            // indicate that we are transmitting
            transmitting = 1;
            // reset tx buffer iterator vars
            txBufferIndex = 0;
            txBufferLength = 0;
            // set rx buffer iterator vars
            rxBufferIndex = 0;
            rxBufferLength = 0;

            if(NULL != user_onRequest)
            {
                // alert user program
                user_onRequest();
            }

            //Wire_DMAConfig(txBuffer, txBufferLength+1, TRANSMITTER);
            Wire_DMAConfig(I2C_Type,txBuffer, txBufferLength+1, TRANSMITTER); //lz

            /* Enable DMA NACK automatic generation */
            I2C_DMALastTransferCmd(I2C_Type, ENABLE);
            /* Enable I2C DMA request */
            I2C_DMACmd(I2C_Type, ENABLE);
            /* Enable DMA TX Channel */
            //DMA_Cmd(DMA1_Channel6, ENABLE);
            DMA_Control(I2C_Type,TRANSMITTER,ENABLE); // lz

            //Wire_DMAConfig(rxBuffer, BUFFER_LENGTH, RECEIVER);
            Wire_DMAConfig(I2C_Type,rxBuffer, BUFFER_LENGTH, RECEIVER); // lz
            /* Enable DMA NACK automatic generation */
            I2C_DMALastTransferCmd(I2C_Type, ENABLE);
            /* Enable I2C DMA request */
            I2C_DMACmd(I2C_Type, ENABLE);
            /* Enable DMA RX Channel */
            // DMA_Cmd(DMA1_Channel7, ENABLE);
            DMA_Control(I2C_Type,RECEIVER,ENABLE); // lz
        }

        /* If STOPF = 1: EV4 (Slave has detected a STOP condition on the bus */
        if (( SR1Register & 0x0010) == 0x0010)
        {
            I2C_Type->CR1 |= ((uint16_t)0x0001);
            SR1Register = 0;
            SR2Register = 0;

            // indicate that we are done transmitting
            transmitting = 0;
            // set rx buffer iterator vars
            rxBufferIndex = 0;
            //rxBufferLength = BUFFER_LENGTH - DMA_GetCurrDataCounter(DMA1_Channel7);
            rxBufferLength = BUFFER_LENGTH - GetDMACurrDataCounter(I2C_Type); // LZ

            if(NULL != user_onReceive)
            {
                // alert user program
                user_onReceive(rxBufferLength);
            }
        }
    }
}

/*********************************************************************************
  *Function     : bool TwoWire::isEnabled()
  *Description  : return I2C enabled status
  *Input        : none
  *Output       : none
  *Return       : none 
  *author       : lz
  *date         : 6-December-2014
  *Others       :         
**********************************************************************************/
bool TwoWire::isEnabled()
{
    return I2C_Enabled;
}

/*********************************************************************************
  *Function     : void TwoWire::DMA_Control(I2C_TypeDef *I2Cx,uint32_t direction,FunctionalState NewState)
  *Description  : I2C-DMA enable control
  *Input        : I2Cx:I2C1 or I2C2
  				  direction:data transmission direction
  				  NewState: enable 1 or 0
  *Output       : none
  *Return       : none 
  *author       : lz
  *date         : 6-December-2014
  *Others       :         
**********************************************************************************/
void TwoWire::DMA_Control(I2C_TypeDef *I2Cx,uint32_t direction,FunctionalState NewState)
{
    if(I2Cx == I2C1)
    {
        if(direction == TRANSMITTER) 
            DMA_Cmd(DMA1_Channel6,NewState);
        else 
            DMA_Cmd(DMA1_Channel7,NewState);
    }

    if(I2Cx == I2C2)
    {
        if(direction == TRANSMITTER) 
            DMA_Cmd(DMA1_Channel4,NewState);
        else 
            DMA_Cmd(DMA1_Channel5,NewState);
    }
}

/*********************************************************************************
  *Function     : FlagStatus TwoWire:: GetDMAFlagStatus(I2C_TypeDef *I2Cx,uint32_t direction)
  *Description  : get DMA Flag staus
  *Input        : I2Cx:I2C1 or I2C2
  				  direction:data transmission direction
  *Output       : none
  *Return       : return  1 or 0 
  *author       : lz
  *date         : 6-December-2014
  *Others       :         
**********************************************************************************/
FlagStatus TwoWire:: GetDMAFlagStatus(I2C_TypeDef *I2Cx,uint32_t direction)
{
    if(I2Cx == I2C1)
    {
        if(direction == TRANSMITTER)
            return DMA_GetFlagStatus(DMA1_FLAG_TC6);
        else 
            return DMA_GetFlagStatus(DMA1_FLAG_TC7);
    }

    if(I2Cx == I2C2)
    {
        if(direction == TRANSMITTER)
            return DMA_GetFlagStatus(DMA1_FLAG_TC4);
        else 
            return DMA_GetFlagStatus(DMA1_FLAG_TC5);
    }
    return (FlagStatus)0;
}

/*********************************************************************************
  *Function     :       void TwoWire::ClearDMAFlag(I2C_TypeDef *I2Cx,uint32_t direction)
  *Description  :   clear DMA Flag staus
  *Input        :          I2Cx:I2C1 or I2C2
  				   direction:data transmission direction
  *Output       :         none
  *Return       :         none
  *author       :         lz
  *date         :           6-December-2014
  *Others       :         
**********************************************************************************/
void TwoWire::ClearDMAFlag(I2C_TypeDef *I2Cx,uint32_t direction)
{
    if(I2Cx == I2C1)
    {
        if(direction == TRANSMITTER)
            DMA_ClearFlag(DMA1_FLAG_TC6);
        else 
            DMA_ClearFlag(DMA1_FLAG_TC7);
    }

    if(I2Cx == I2C2)
    {
        if(direction == TRANSMITTER)
            DMA_ClearFlag(DMA1_FLAG_TC4);
        else 
            DMA_ClearFlag(DMA1_FLAG_TC5);
    }
}

/*********************************************************************************
  *Function     : uint16_t TwoWire::GetDMACurrDataCounter(I2C_TypeDef *I2Cx)
  *Description  : get DMA current data counter
  *Input        : I2Cx:I2C1 or I2C2
  *Output       : none
  *Return       : return counter
  *author       : lz
  *date         : 6-December-2014
  *Others       :         
**********************************************************************************/
uint16_t TwoWire::GetDMACurrDataCounter(I2C_TypeDef *I2Cx)
{
    if(I2Cx == I2C1)
    {
        return DMA_GetCurrDataCounter(DMA1_Channel7);
    }

    if(I2Cx == I2C2)
    {
        return DMA_GetCurrDataCounter(DMA1_Channel5);
    }
    return 0;
}

// Preinstantiate Objects //////////////////////////////////////////////////////
#if I2C_INTERFACES_COUNT > 0
/*********************************************************************************
  *Function     : static void Wire_Init(uint8_t Type) 
  *Description  : I2C1 initialize
  *Input        : Type:1 or 0
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       :         
**********************************************************************************/
static void Wire_Init(uint8_t Type) 
{
    NVIC_InitTypeDef  NVIC_InitStructure;

    /* Enable I2C1 clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    /* Enable DMA1 clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /*config GPIO*/
    pinMode(PIN_I2C_SCL, AF_OUTPUT_DRAIN);
    pinMode(PIN_I2C_SDA, AF_OUTPUT_DRAIN);

    if(Type==1)
    {
        NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
        NVIC_Init(&NVIC_InitStructure);
        I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_ERR, ENABLE);
    }
}

TwoWire Wire = TwoWire(I2C_INTERFACE1, Wire_Init);

/*******************************************************************************
  *Function     : Wiring_I2C1_EV_Interrupt_Handler 
  *Description  : I2C1 initialize
  *Input        : Type:1 or 0
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       :  
 *******************************************************************************/
void Wiring_I2C1_EV_Interrupt_Handler(void)
{
    Wire.slaveEventHandler();
}

void Wiring_I2C1_ER_Interrupt_Handler(void)
{
    __IO uint32_t SR1Register = 0;

    /* Read the I2C1 status register */
    SR1Register = I2C1->SR1;

    /* If AF = 1 */
    if ((SR1Register & 0x0400) == 0x0400)
    {
        I2C1->SR1 &= 0xFBFF;
        SR1Register = 0;
    }

    /* If ARLO = 1 */
    if ((SR1Register & 0x0200) == 0x0200)
    {
        I2C1->SR1 &= 0xFBFF;
        SR1Register = 0;
    }

    /* If BERR = 1 */
    if ((SR1Register & 0x0100) == 0x0100)
    {
        I2C1->SR1 &= 0xFEFF;
        SR1Register = 0;
    }

    /* If OVR = 1 */
    if ((SR1Register & 0x0800) == 0x0800)
    {
        I2C1->SR1 &= 0xF7FF;
        SR1Register = 0;
    }
}
#endif

#if I2C_INTERFACES_COUNT > 1
/*********************************************************************************
  *Function     : static void Wire1_Init(uint8_t Type) 
  *Description  : I2C2 initialize
  *Input        : Type:1 or 0
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       :         
**********************************************************************************/
static void Wire1_Init(uint8_t Type) 
{
    NVIC_InitTypeDef  NVIC_InitStructure;
    /* Enable I2C2 clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    /* Enable DMA1 clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    pinMode(PIN_I2C_SCL1, AF_OUTPUT_DRAIN);
    pinMode(PIN_I2C_SDA1, AF_OUTPUT_DRAIN);

    if(Type==1)
    {
        NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn;
        NVIC_Init(&NVIC_InitStructure);
        I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_ERR, ENABLE);
    }
}

TwoWire Wire1 = TwoWire(I2C_INTERFACE2, Wire1_Init);

/*******************************************************************************
 *Function      : Wiring_I2C2_EV_Interrupt_Handler (Declared as weak in stm32_it.cpp)
 *Description   : This function handles I2C1 Event interrupt request(Only for Slave mode).
 *Input         : None.
 *Output        : None.
 *Return        : None.
 *author        : lz
 *date          : 6-December-2014
 *Others        : 
 *******************************************************************************/
void Wiring_I2C2_EV_Interrupt_Handler(void)
{
    Wire1.slaveEventHandler();
}

/*******************************************************************************
 *Function      : Wiring_I2C1_ER_Interrupt_Handler (Declared as weak in stm32_it.cpp)
 *Description   : This function handles I2C1 Error interrupt request.
 *Input         : None.
 *Output        : None.
 *Return        : None.
 *author        : lz
 *date          : 6-December-2014
 *Others        : 
 *******************************************************************************/
void Wiring_I2C2_ER_Interrupt_Handler(void)
{
    __IO uint32_t SR1Register = 0;

    /* Read the I2C1 status register */
    SR1Register = I2C2->SR1;

    /* If AF = 1 */
    if ((SR1Register & 0x0400) == 0x0400)
    {
        I2C2->SR1 &= 0xFBFF;
        SR1Register = 0;
    }

    /* If ARLO = 1 */
    if ((SR1Register & 0x0200) == 0x0200)
    {
        I2C2->SR1 &= 0xFBFF;
        SR1Register = 0;
    }

    /* If BERR = 1 */
    if ((SR1Register & 0x0100) == 0x0100)
    {
        I2C2->SR1 &= 0xFEFF;
        SR1Register = 0;
    }

    /* If OVR = 1 */
    if ((SR1Register & 0x0800) == 0x0800)
    {
        I2C2->SR1 &= 0xF7FF;
        SR1Register = 0;
    }
}
#endif

