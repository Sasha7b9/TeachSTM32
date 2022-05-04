#include "Hardware/LCD1602/LCD1602.h"
#include <stm32f7xx_hal.h>


namespace LCD1602
{
    void FunctionStart();
}


void LCD1602::Init()
{
    GPIO_InitTypeDef is;

    is.Pin = GPIO_PIN_3;                // RS
    is.Mode = GPIO_MODE_OUTPUT_PP;
    is.Pull = GPIO_NOPULL;
    is.Speed = GPIO_SPEED_LOW;
    
    HAL_GPIO_Init(GPIOA, &is);
    
    is.Pin = GPIO_PIN_0;                // E
    
    HAL_GPIO_Init(GPIOB, &is);
    
    is.Pin = GPIO_PIN_11 |              // D4
        GPIO_PIN_12 |                   // D5
        GPIO_PIN_13 |                   // D6
        GPIO_PIN_5;                     // D7
    
    HAL_GPIO_Init(GPIOB, &is);
    
    FunctionStart();
}


void LCD1602::FunctionStart()
{
    
}
