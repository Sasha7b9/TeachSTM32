#include "stm32f7xx_hal.h"
#include "dma.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"
#include "stdio.h"

#include "stm32746g_sdram.h"
#include "stm32746g_LCD.h"
#include "GT811.h"

TS_StateTypeDef  TS_State = { 0 };
uint8_t TS_flag;
uint8_t TouchPoit;
TS_StateTypeDef  TS_BKState;
uint8_t value = 0;
uint16_t i;
uint32_t PointColor[] = { LCD_COLOR_BLUE,LCD_COLOR_GREEN,LCD_COLOR_RED,LCD_COLOR_MAGENTA,LCD_COLOR_YELLOW };

void SystemClock_Config(void);

int main(void)
{
	HAL_Init();

	SystemClock_Config();

	MX_GPIO_Init();
	MX_DMA_Init();
	MX_DMA2D_Init();
	MX_FMC_Init();
	//  MX_I2C4_Init();
	MX_LTDC_Init();
	MX_USART1_UART_Init();

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	BSP_SDRAM_Init();
	BSP_LCD_Init();
	GT811_Init();

	BSP_LCD_SetLayerVisible(1, DISABLE);
	BSP_LCD_SelectLayer(0);
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_Clear(BSP_LCD_GetBackColor());
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"Capacitive touch screen test", CENTER_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_DrawHLine(0, 30, 1024);

	while (1)
	{
		if (TS_flag == 1)
		{
			GT811_GetState(&TS_State);
			if (TS_State.touchDetected != 0)
			{
				TouchPoit = TS_BKState.touchDetected;
				for (i = 0; i < 5; i++)
				{
					if (TouchPoit & 0x01)
					{
						BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
						BSP_LCD_FillCircle(TS_BKState.touchX[i], TS_BKState.touchY[i], 20);
						BSP_LCD_DrawVLine(TS_BKState.touchX[i], 55, 580);
						BSP_LCD_DrawHLine(5, TS_BKState.touchY[i], 1004);
					}
					TouchPoit >>= 1;
				}

				TouchPoit = TS_State.touchDetected;
				for (i = 0; i < 5; i++)
				{
					if (TouchPoit & 0x01)
					{
						if (TS_State.touchY[i] < 75)TS_State.touchY[i] = 75;
						if (TS_State.touchY[i] > 580)TS_State.touchY[i] = 580;
						if (TS_State.touchX[i] < 20)TS_State.touchX[i] = 20;
						if (TS_State.touchX[i] > 1004)TS_State.touchX[i] = 1004;

						BSP_LCD_SetTextColor(PointColor[i]);
						BSP_LCD_FillCircle(TS_State.touchX[i], TS_State.touchY[i], 20);
						BSP_LCD_DrawVLine(TS_State.touchX[i], 55, 580);
						BSP_LCD_DrawHLine(5, TS_State.touchY[i], 1004);
						TS_BKState.touchX[i] = TS_State.touchX[i];
						TS_BKState.touchY[i] = TS_State.touchY[i];
					}
					TouchPoit >>= 1;
				}
				TS_BKState.touchDetected = TS_State.touchDetected;
			}
			TS_flag = 0;
		}

	}
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

	__PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 432;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	HAL_PWREx_ActivateOverDrive();

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
		| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);

	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC | RCC_PERIPHCLK_USART1
		| RCC_PERIPHCLK_I2C4;
	PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
	PeriphClkInitStruct.PLLSAI.PLLSAIR = 3;
	PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
	PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV2;
	PeriphClkInitStruct.PLLSAIDivQ = 1;
	PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
	PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	PeriphClkInitStruct.I2c4ClockSelection = RCC_I2C4CLKSOURCE_SYSCLK;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
/**
  * @}
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_7)
	{
		TS_flag = 1;
		printf("ok\r\n");
	}
}
/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */

  /**
	* @}
  */

  /************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
