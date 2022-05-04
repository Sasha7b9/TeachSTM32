#include "defines.h"
#include "Hardware/LCD1602/LCD1602.h"
#include "Hardware/Timer.h"
#include <stm32f7xx_hal.h>


namespace LCD1602
{
    void FunctionStart();
    
    void WriteCommand(uint8);
}


void LCD1602::Init()
{
    GPIO_InitTypeDef is;

    is.Pin = GPIO_PIN_3;                // RS
    is.Mode = GPIO_MODE_OUTPUT_PP;
    is.Pull = GPIO_NOPULL;
    is.Speed = GPIO_SPEED_LOW;
    
    HAL_GPIO_Init(GPIOA, &is);
    
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
    
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
    Timer::Delay(15);
    
    WriteCommand(0x30);
    
    Timer::Delay(10);
    
    WriteCommand(0x30);
    
    Timer::Delay(10);
    
    WriteCommand(0x30);
    
    WriteCommand(0x20);
    
    WriteCommand(0x20);
    
    WriteCommand(0xc0);
    
    WriteCommand(0x00);
    
    WriteCommand(0x80);
    
    WriteCommand(0x00);
    
    WriteCommand(0x10);
    
    WriteCommand(0x00);
    
    WriteCommand(0x70);
}


void LCD1602::WriteCommand(uint8 command)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, (command & (1 << 4)) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, (command & (1 << 5)) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, (command & (1 << 6)) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (command & (1 << 7)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
