#include "hw_config.h"
#include "ui_hal.h"


#define USER_LED_GPIO_PIN          GPIO_PIN_8
#define USER_LED_GPIO_PORT         GPIOA

#define TR_LED_GPIO_PIN            GPIO_PIN_4
#define TR_LED_GPIO_PORT           GPIOB

#define MODE_BOTTON_GPIO_PIN       GPIO_PIN_15
#define MODE_BOTTON_GPIO_PORT      GPIOA


volatile uint32_t BUTTON_last_state = 0;
volatile uint32_t TimingBUTTON=0;

void HAL_UI_Initial(void)
{
    //用户指示灯初始化
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pin = USER_LED_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(USER_LED_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(USER_LED_GPIO_PORT, USER_LED_GPIO_PIN, GPIO_PIN_SET);

    //收发指示灯初始化
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = TR_LED_GPIO_PIN;
    HAL_GPIO_Init(TR_LED_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(TR_LED_GPIO_PORT, TR_LED_GPIO_PIN, GPIO_PIN_SET);

    //侧边配置按键管脚初始化
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = MODE_BOTTON_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(MODE_BOTTON_GPIO_PORT, &GPIO_InitStruct);
}

uint8_t HAL_UI_Mode_BUTTON_GetState(Button_TypeDef Button)
{
    return HAL_GPIO_ReadPin(MODE_BOTTON_GPIO_PORT, MODE_BOTTON_GPIO_PIN);
}

uint32_t HAL_UI_Mode_Button_Pressed(void)
{
    return TimingBUTTON;
}

int HAL_UI_RGB_Get_Info(rgb_info_t *pinfo)
{
    return 0;
}

int HAL_UI_RGB_Set_Info(rgb_info_t info)
{
    return 0;
}

void HAL_UI_RGB_Color(uint32_t color)
{

}

void HAL_UI_RGB_Blink(uint32_t color, uint16_t period)
{

}

void HAL_UI_RGB_Breath(uint32_t color, uint16_t period)
{
}

void HAL_UI_UserLED_Control(uint8_t value)
{
    if(value) {
        HAL_GPIO_WritePin(USER_LED_GPIO_PORT, USER_LED_GPIO_PIN, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(USER_LED_GPIO_PORT, USER_LED_GPIO_PIN, GPIO_PIN_SET);
    }
}

void HAL_UI_TrLED_Control(uint8_t value)
{
    if(value) {
        HAL_GPIO_WritePin(TR_LED_GPIO_PORT, TR_LED_GPIO_PIN, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(TR_LED_GPIO_PORT, TR_LED_GPIO_PIN, GPIO_PIN_SET);
    }
}

void HAL_UI_SysTick_Handler(void)
{
    //侧边按键处理
    if(!HAL_UI_Mode_BUTTON_GetState(BUTTON1)) {
        if(!BUTTON_last_state) {
            TimingBUTTON = 0;
            BUTTON_last_state = 1;
        } else {
            TimingBUTTON++;
        }
    } else {
        if(BUTTON_last_state) {
            TimingBUTTON = 0;
            BUTTON_last_state = 0;
        }
    }
}
