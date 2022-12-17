#include "main.h"
#include "bsp.h"

int main(void)
{
	bspInit_sysClk();
	bspInit_GPIO();
	bspInit_sysTimer();

	while (1)
	{
		bspCtl_LedOn();
		delay_ms(500);
		bspCtl_LedOff();
		delay_ms(500);
	}
}
