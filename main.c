#include "main.h"
#include "bsp.h"
#include "uart_drv.h"
#include <stdio.h>
#include <string.h>


UART_DRV uart3;


int _write(int fd, char* ptr, int len) {
	uartDrv_transmitt(&uart3, ptr, len);
    return len;
}


void uwPrintBinReg(uint32_t reg)
{
	printf ("+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+\r\n");
	printf ("|31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|\r\n");
	printf ("+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+\r\n");
	for(int32_t i=31; i>=0; i--){
		if (reg & 1<<i)
			printf("| %c", '*');
		else
			printf("| %c", ' ');
	}
}


void printClockSettings(void)
{
	uint32_t tmp;
	uint8_t clock_pll_flag = 0;
	uint32_t HSE_clk = 8000000;
	uint32_t HSI_clk = 8000000;



	uint32_t sysclk = 0;
	uint32_t hclk = 0;

	uint32_t pllsrc = 0;
	uint32_t pllmul = 1;

	uint32_t APB1_clk = 0;
	uint32_t APB2_clk = 0;
	uint32_t ADC_clk = 0;

	uint32_t HSE_prescaler = 1;
	uint32_t AHB_prescaler = 1;
	uint32_t APB1_prescaler = 1;
	uint32_t APB2_prescaler = 1;



	printf("    CPU clock settings:\r\n");
	printf("external oscillator:     %d Hz\r\n", HSE_clk);
	printf("System clock switch:     ");
	tmp = (RCC->CFGR & RCC_CFGR_SWS_Msk) >> RCC_CFGR_SWS_Pos;
	switch (tmp)
	{
		case 0: {
			printf("HSI\r\n"); 
			sysclk = HSI_clk; 
			break;
		};
		case 1: {
			printf("HSE\r\n"); 
			sysclk = HSE_clk; 
			break;
		}
		case 2: {
			tmp = (RCC->CFGR & RCC_CFGR_PLLSRC_Msk) >> RCC_CFGR_PLLSRC_Pos;

			switch (tmp)
			{
				case 0: 
				{
					printf("HSI /2"); 
					pllsrc = HSI_clk;
					break;
				}
				case 1: 
				{ 
					printf("HSE"); 
					if ((RCC->CFGR & RCC_CFGR_PLLXTPRE_Msk) >> RCC_CFGR_PLLXTPRE_Pos)
					{
						pllsrc = HSE_clk/2;
						printf("/2"); 
					}
					else{
						pllsrc = HSE_clk;
					}
					break;
				}
				default: printf("ERROR\r\n"); break;
			}
			printf(" -> PLL\r\n");
			
			tmp = (RCC->CFGR & RCC_CFGR_PLLMULL_Msk) >> RCC_CFGR_PLLMULL_Pos;
			pllmul = tmp+2;
			printf("PLL multiplier:    x%d\r\n", pllmul);

			sysclk = pllsrc * pllmul;
		} break;
		default: printf("ERROR\r\n"); break;
	}
	printf("SYSCLK:    %dHz\r\n", sysclk);

	tmp = (RCC->CFGR & RCC_CFGR_HPRE_Msk);
	switch(tmp)
	{
		case RCC_CFGR_HPRE_DIV1: 	AHB_prescaler = 1; break;
		case RCC_CFGR_HPRE_DIV2: 	AHB_prescaler = 2; break;
		case RCC_CFGR_HPRE_DIV4: 	AHB_prescaler = 4; break;
		case RCC_CFGR_HPRE_DIV8:	AHB_prescaler = 8; break;
		case RCC_CFGR_HPRE_DIV16:	AHB_prescaler = 16; break;
		case RCC_CFGR_HPRE_DIV64:	AHB_prescaler = 64; break;
		case RCC_CFGR_HPRE_DIV128:	AHB_prescaler = 128; break;
		case RCC_CFGR_HPRE_DIV256:	AHB_prescaler = 256; break;
		case RCC_CFGR_HPRE_DIV512:	AHB_prescaler = 512; break;
		default: printf("ERROR"); break;

	}
	hclk = sysclk/AHB_prescaler;
	printf("HCLK:      %dHz = SYSCLK/%d\r\n", hclk, AHB_prescaler);

	tmp = (RCC->CFGR & RCC_CFGR_PPRE1_Msk);
	switch(tmp)
	{
		case RCC_CFGR_PPRE1_DIV1: 	APB1_prescaler = 1; break;
		case RCC_CFGR_PPRE1_DIV2: 	APB1_prescaler = 2; break;
		case RCC_CFGR_PPRE1_DIV4: 	APB1_prescaler = 4; break;
		case RCC_CFGR_PPRE1_DIV8:	APB1_prescaler = 8; break;
		case RCC_CFGR_PPRE1_DIV16:	APB1_prescaler = 16; break;
		default: printf("ERROR"); break;
	}
	APB1_clk = hclk/APB1_prescaler;
	printf("PCLK1:     %dHz = HCLK/%d (36MHz max)\r\n", APB1_clk, APB1_prescaler);
	if (APB1_clk>36000000){
		printf("      ERROR: 36MHz max\r\n");
	}

	tmp = (RCC->CFGR & RCC_CFGR_PPRE2_Msk);
	switch(tmp)
	{
		case RCC_CFGR_PPRE2_DIV1: 	APB2_prescaler = 1; break;
		case RCC_CFGR_PPRE2_DIV2: 	APB2_prescaler = 2; break;
		case RCC_CFGR_PPRE2_DIV4: 	APB2_prescaler = 4; break;
		case RCC_CFGR_PPRE2_DIV8:	APB2_prescaler = 8; break;
		case RCC_CFGR_PPRE2_DIV16:	APB2_prescaler = 16; break;
		default: printf("ERROR"); break;
	}
	APB2_clk = hclk/APB2_prescaler;
	printf("PCLK2:     %dHz = HCLK/%d\r\n", APB2_clk, APB2_prescaler);





	// printf("System clock switch:     ");
	// tmp = (RCC->CFGR & RCC_CFGR_SW_Msk) >> RCC_CFGR_SW_Pos;
	// switch (tmp)
	// {
	// 	case 0: printf("HSI"); break;
	// 	case 1: printf("HSE"); break;
	// 	case 2: printf("PLL"); clock_pll_flag = 1; break;
	// 	default: printf("ERROR"); break;
	// }

}

int main(void)
{
	uint8_t buf[] = "\n";
	uint8_t buf2 = 0xFF;


	bspInit_sysClk();
	bspInit_GPIO();
	bspInit_sysTimer();
	uartDrv_init(&uart3);
	

	printf("\r\n\r\nUART3....... OK\r\n");

	printClockSettings();

	while (1)
	{
		printf("hello\n");
		delay_ms(1000);
		bspCtl_LedToggle();

	}
}
