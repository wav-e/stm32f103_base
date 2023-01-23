#include "ssd1306_app.h"
#include "ssd1306.h"

#include <string.h>



static void int_to_str(uint32_t number, char *str)
{
    uint32_t i=0;
    uint32_t k=0;
    uint32_t tmp=number;

        /* search digit number */
        str[2] = '\0';
        str[1] = number - ((number/10)*10) + '0';
        str[0] = number/10 + '0';
}




void ssd1306_app_DrawFPS(uint8_t x, uint8_t y, FontDef Font)
{
	uint32_t start; 	//saves tick for calc
	uint32_t stop; 	//saves tick for calc
	static uint32_t fps = 0;
	char str[3] = "65";



	start = ssd1306_wrapper_get_tick();
	ssd1306_SetCursor(x,y);
	int_to_str(fps,str);
	ssd1306_WriteString(str, Font_11x18, White);
	//ssd1306_UpdateScreen();

	stop = ssd1306_wrapper_get_tick();
	fps = 1000/(stop-start);

}
