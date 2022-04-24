#include "Hardware/HAL/HAL.h"
#include "Display/Display.h"



int main(void)
{
	HAL::Init();

	Display::Init();

	while (1)
	{
		Display::Update();
	}
}
