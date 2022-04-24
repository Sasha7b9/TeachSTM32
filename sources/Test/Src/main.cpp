#include "Hardware/HAL/HAL.h"
#include <stm32746g_LCD.h>
#include <GT811.h>

TS_StateTypeDef  TS_State = { 0 };
uint8_t TS_flag;
uint8_t TouchPoit;
TS_StateTypeDef  TS_BKState;
uint8_t value = 0;
uint16_t i;
uint32_t PointColor[] = { LCD_COLOR_BLUE,LCD_COLOR_GREEN,LCD_COLOR_RED,LCD_COLOR_MAGENTA,LCD_COLOR_YELLOW };


int main(void)
{
	HAL::Init();

	LCD::Init();
	GT811::Init();

	LCD::SetLayerVisible(1, DISABLE);
	LCD::SelectLayer(0);
	LCD::SetTextColor(LCD_COLOR_BLUE);
	LCD::SetBackColor(LCD_COLOR_BLACK);
	LCD::Clear(LCD::GetBackColor());
	LCD::SetFont(&Font24);
	LCD::DisplayStringAt(0, 0, (uint8_t *)"Capacitive touch screen test", CENTER_MODE);
	LCD::SetTextColor(LCD_COLOR_WHITE);
	LCD::DrawHLine(0, 30, 1024);

	while (1)
	{
		if (TS_flag == 1)
		{
			GT811::GetState(&TS_State);
			if (TS_State.touchDetected != 0)
			{
				TouchPoit = TS_BKState.touchDetected;
				for (i = 0; i < 5; i++)
				{
					if (TouchPoit & 0x01)
					{
						LCD::SetTextColor(LCD_COLOR_BLACK);
						LCD::FillCircle(TS_BKState.touchX[i], TS_BKState.touchY[i], 20);
						LCD::DrawVLine(TS_BKState.touchX[i], 55, 580);
						LCD::DrawHLine(5, TS_BKState.touchY[i], 1004);
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

						LCD::SetTextColor(PointColor[i]);
						LCD::FillCircle(TS_State.touchX[i], TS_State.touchY[i], 20);
						LCD::DrawVLine(TS_State.touchX[i], 55, 580);
						LCD::DrawHLine(5, TS_State.touchY[i], 1004);
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


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_7)
	{
		TS_flag = 1;
	}
}


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
