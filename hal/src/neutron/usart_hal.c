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

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "usart_hal.h"
#include "pinmap_impl.h"
#include <string.h>

extern void USART1_IRQHandler(void);
extern void USART2_IRQHandler(void);

/* Private typedef -----------------------------------------------------------*/
typedef enum USART_Num_Def {
    USART_A2_A3 = 0,
    USART_TXD_UC_RXD_UC = 1
} USART_Num_Def;

/* Private macro -------------------------------------------------------------*/
// IS_USART_CONFIG_VALID(config) - returns true for 8 data bit, any flow control, any parity, any stop byte configurations
#define IS_USART_CONFIG_VALID(CONFIG) (((CONFIG & SERIAL_VALID_CONFIG) >> 2) != 0b11)

/* Private variables ---------------------------------------------------------*/
typedef struct STM32_USART_Info {
    USART_TypeDef* usart_peripheral;
    uint32_t* usart_Alternate;
    int32_t usart_int_n;
    uint16_t usart_tx_pin;
    uint16_t usart_rx_pin;

    // Buffer pointers. These need to be global for IRQ handler access
    Ring_Buffer* usart_tx_buffer;
    Ring_Buffer* usart_rx_buffer;

    bool usart_enabled;
    bool usart_transmitting;
} STM32_USART_Info;

/*
 * USART mapping
 */
STM32_USART_Info USART_MAP[TOTAL_USARTS] =
{
    /*
     * USART_peripheral (USARTx/UARTx; not using others)
     * gpio_peripheral (GPIOA, GPIOB, GPIOC or GPIOD)
     * Alternate;
     * interrupt number (USARTx_IRQn/UARTx_IRQn)
     * TX pin
     * RX pin
     * <tx_buffer pointer> used internally and does not appear below
     * <rx_buffer pointer> used internally and does not appear below
     * <usart enabled> used internally and does not appear below
     * <usart transmitting> used internally and does not appear below
     */
    { USART2, GPIO_AF7_USART2, USART2_IRQn, TX, RX },        // USART 2
    { USART1, GPIO_AF7_USART1, USART1_IRQn, TXD_UC, RXD_UC } // USART 1
};

UART_HandleTypeDef UartHandle;
static STM32_USART_Info *usartMap[TOTAL_USARTS]; // pointer to USART_MAP[] containing USART peripheral register locations (etc)

/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
inline void store_char(unsigned char c, Ring_Buffer *buffer) __attribute__((always_inline));
inline void store_char(unsigned char c, Ring_Buffer *buffer)
{
    unsigned i = (unsigned int)(buffer->head + 1) % SERIAL_BUFFER_SIZE;

    if (i != buffer->tail)
    {
        buffer->buffer[buffer->head] = c;
        buffer->head = i;
    }
}

static uint16_t USART_ReceiveData(UART_HandleTypeDef *huart)
{
    return (uint16_t)(huart->Instance->DR & (uint16_t)0x01FF);
}

void HAL_USART_Initial(HAL_USART_Serial serial, Ring_Buffer *rx_buffer, Ring_Buffer *tx_buffer)
{
    if(serial == HAL_USART_SERIAL1)
    {
        usartMap[serial] = &USART_MAP[USART_A2_A3];
    }
    else if(serial == HAL_USART_SERIAL2)
    {
        usartMap[serial] = &USART_MAP[USART_TXD_UC_RXD_UC];
    }

    usartMap[serial]->usart_rx_buffer = rx_buffer;
    //usartMap[serial]->usart_tx_buffer = tx_buffer;

    memset(usartMap[serial]->usart_rx_buffer, 0, sizeof(Ring_Buffer));
    //memset(usartMap[serial]->usart_tx_buffer, 0, sizeof(Ring_Buffer));

    usartMap[serial]->usart_enabled = false;
    usartMap[serial]->usart_transmitting = false;
}

void HAL_USART_Begin(HAL_USART_Serial serial, uint32_t baud)
{
    HAL_USART_BeginConfig(serial, baud, 0, 0); // Default serial configuration is 8N1
}

void HAL_USART_BeginConfig(HAL_USART_Serial serial, uint32_t baud, uint32_t config, void *ptr)
{
    // Verify UART configuration, exit if it's invalid.
    if (!IS_USART_CONFIG_VALID(config)) {
        usartMap[serial]->usart_enabled = false;
        return;
    }

    HAL_UART_DeInit(&UartHandle);

    __HAL_RCC_GPIOA_CLK_ENABLE();
    if(HAL_USART_SERIAL1 == serial){
        __HAL_RCC_USART2_CLK_ENABLE();
    }
    else{
        __HAL_RCC_USART1_CLK_ENABLE();
    }

	STM32_Pin_Info* PIN_MAP = HAL_Pin_Map();
    GPIO_InitTypeDef  GPIO_InitStruct;
    /* UART TX RX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = PIN_MAP[usartMap[serial]->usart_tx_pin].gpio_pin|PIN_MAP[usartMap[serial]->usart_rx_pin].gpio_pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = usartMap[serial]->usart_Alternate;
    HAL_GPIO_Init(PIN_MAP[usartMap[serial]->usart_rx_pin].gpio_peripheral, &GPIO_InitStruct);

    UartHandle.Instance          = usartMap[serial]->usart_peripheral;
    UartHandle.Init.BaudRate     = baud;
    UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode         = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    // Stop bit configuration.
    switch (config & SERIAL_STOP_BITS) {
        case 0b00: // 1 stop bit
            UartHandle.Init.StopBits = UART_STOPBITS_1;
            break;
        case 0b01: // 2 stop bits
            UartHandle.Init.StopBits = UART_STOPBITS_2;
            break;
    }

    // Eight / Nine data bit configuration
    if (config & SERIAL_NINE_BITS) {
        // Nine data bits, no parity.
        UartHandle.Init.Parity = UART_PARITY_NONE;
        UartHandle.Init.WordLength = UART_WORDLENGTH_9B;
    } else {
        // eight data bits, parity configuration (impacts word length)
        switch ((config & SERIAL_PARITY_BITS) >> 2) {
            case 0b00: // none
                UartHandle.Init.Parity = UART_PARITY_NONE;
                UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
                break;
            case 0b01: // even
                UartHandle.Init.Parity = UART_PARITY_EVEN;
                UartHandle.Init.WordLength = UART_WORDLENGTH_9B;
                break;
            case 0b10: // odd
                UartHandle.Init.Parity = UART_PARITY_ODD;
                UartHandle.Init.WordLength = UART_WORDLENGTH_9B;
                break;
        }
    }
    HAL_UART_Init(&UartHandle);

    //Configure the NVIC for UART
    HAL_NVIC_SetPriority(usartMap[serial]->usart_int_n, 0x07, 0);
    HAL_NVIC_EnableIRQ(usartMap[serial]->usart_int_n);
    __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);

    usartMap[serial]->usart_enabled = true;
    usartMap[serial]->usart_transmitting = false;
}

void HAL_USART_End(HAL_USART_Serial serial)
{
    // Wait for transmission of outgoing data
    while (usartMap[serial]->usart_tx_buffer->head != usartMap[serial]->usart_tx_buffer->tail);

    HAL_UART_DeInit(&UartHandle);

    if(HAL_USART_SERIAL1 == serial){
        __HAL_RCC_USART2_FORCE_RESET();
        __HAL_RCC_USART2_RELEASE_RESET();
    }
    else{
        __HAL_RCC_USART1_FORCE_RESET();
        __HAL_RCC_USART1_RELEASE_RESET();
    }

    //Disable the NVIC for UART ##########################################*/
    HAL_NVIC_DisableIRQ(usartMap[serial]->usart_int_n);

    // clear any received data
    usartMap[serial]->usart_rx_buffer->head = usartMap[serial]->usart_rx_buffer->tail;

    // Undo any pin re-mapping done for this USART
    // ...
	STM32_Pin_Info* PIN_MAP = HAL_Pin_Map();
    /* Configure UART Tx as alternate function */
    HAL_GPIO_DeInit(PIN_MAP[usartMap[serial]->usart_tx_pin].gpio_peripheral, PIN_MAP[usartMap[serial]->usart_tx_pin].gpio_pin);
    /* Configure UART Rx as alternate function */
    HAL_GPIO_DeInit(PIN_MAP[usartMap[serial]->usart_tx_pin].gpio_peripheral, PIN_MAP[usartMap[serial]->usart_tx_pin].gpio_pin);

    memset(usartMap[serial]->usart_rx_buffer, 0, sizeof(Ring_Buffer));
    //memset(usartMap[serial]->usart_tx_buffer, 0, sizeof(Ring_Buffer));

    usartMap[serial]->usart_enabled = false;
    usartMap[serial]->usart_transmitting = false;
}

uint32_t HAL_USART_Write_Data(HAL_USART_Serial serial, uint8_t data)
{
    HAL_UART_Transmit(&UartHandle, &data, 1, 5);//5ms
    return 1;
}

uint32_t HAL_USART_Write_NineBitData(HAL_USART_Serial serial, uint16_t data)
{
    HAL_UART_Transmit(&UartHandle, &data, 1, 5);//5ms
    return 1;
}

int32_t HAL_USART_Available_Data(HAL_USART_Serial serial)
{
    return (unsigned int)(SERIAL_BUFFER_SIZE + usartMap[serial]->usart_rx_buffer->head - usartMap[serial]->usart_rx_buffer->tail) % SERIAL_BUFFER_SIZE;
}

int32_t HAL_USART_Available_Data_For_Write(HAL_USART_Serial serial)
{
    return 1;
}

int32_t HAL_USART_Read_Data(HAL_USART_Serial serial)
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (usartMap[serial]->usart_rx_buffer->head == usartMap[serial]->usart_rx_buffer->tail)
    {
        return -1;
    }
    else
    {
        unsigned char c = usartMap[serial]->usart_rx_buffer->buffer[usartMap[serial]->usart_rx_buffer->tail];
        usartMap[serial]->usart_rx_buffer->tail = (unsigned int)(usartMap[serial]->usart_rx_buffer->tail + 1) % SERIAL_BUFFER_SIZE;
        return c;
    }
}

int32_t HAL_USART_Peek_Data(HAL_USART_Serial serial)
{
    if (usartMap[serial]->usart_rx_buffer->head == usartMap[serial]->usart_rx_buffer->tail)
    {
        return -1;
    }
    else
    {
        return usartMap[serial]->usart_rx_buffer->buffer[usartMap[serial]->usart_rx_buffer->tail];
    }
}

void HAL_USART_Flush_Data(HAL_USART_Serial serial)
{
    usartMap[serial]->usart_transmitting = false;
}

bool HAL_USART_Is_Enabled(HAL_USART_Serial serial)
{
    return usartMap[serial]->usart_enabled;
}

void HAL_USART_Half_Duplex(HAL_USART_Serial serial, bool Enable)
{
}

// Shared Interrupt Handler for USART2/Serial1 and USART1/Serial2
// WARNING: This function MUST remain reentrance compliant -- no local static variables etc.
static void HAL_USART_Handler(HAL_USART_Serial serial)
{
    if((__HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_RXNE) != RESET) && (__HAL_UART_GET_IT_SOURCE(&UartHandle, UART_IT_RXNE) != RESET))
    {
        // Read byte from the receive data register
        unsigned char c = USART_ReceiveData(&UartHandle);
        store_char(c, usartMap[serial]->usart_rx_buffer);
    }

    if (__HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_ORE) != RESET)
    {
        // If Overrun flag is still set, clear it
        (void)USART_ReceiveData(&UartHandle);
    }
}

// Serial interrupt handler
/*******************************************************************************
 * Function Name  : HAL_USART1_Handler
 * Description    : This function handles USART1 global interrupt request.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void USART2_IRQHandler(void)
{
    HAL_USART_Handler(HAL_USART_SERIAL1);
}

// Serial1 interrupt handler
/*******************************************************************************
 * Function Name  : HAL_USART2_Handler
 * Description    : This function handles USART2 global interrupt request.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void USART1_IRQHandler(void)
{
    HAL_USART_Handler(HAL_USART_SERIAL2);
}

