#include "Display/Display.h"
#include <stm32746g_lcd.h>


void Display::Init()
{
    LCD::Init();

	LCD::SetLayerVisible(1, DISABLE);
	LCD::SelectLayer(0);
	LCD::SetTextColor(LCD_COLOR_BLUE);
	LCD::SetBackColor(LCD_COLOR_BLACK);
	LCD::Clear(LCD::GetBackColor());
	LCD::SetFont(&Font24);
	LCD::DisplayStringAt(0, 0, (uint8_t *)"Capacitive touch screen test", CENTER_MODE);
	LCD::SetTextColor(LCD_COLOR_WHITE);
	LCD::DrawHLine(0, 30, 1024);
}
