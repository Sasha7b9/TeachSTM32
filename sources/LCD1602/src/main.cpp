#include "Hardware/HAL/HAL.h"
#include "Display/Display.h"
#include "Hardware/LCD1602/LCD1602.h"


int main(void)
{
	HAL::Init();

	Display::Init();

    LCD1602::Init();

	while (1)
	{
		Display::Update();
	}
}
