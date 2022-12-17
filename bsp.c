/******************************************************************************
 * @file 	bsp.h
 * @brief 	Contains macros and functions for BluePill platform
 *****************************************************************************/

#include "bsp.h"

volatile uint32_t sys_tick=0;
volatile uint32_t delay_cnt_ms=0;
/**
  * @brief  Initialize a system clock. Clock init at 72MHz
  *
  * @ note 	BluePill runs from external quartz at 8MHz
  * 		and init to speed at 72Mhz.
  *
  * @retval None
  */
void bspInit_sysClk(void)
{
	/* HSE ENABLE */
	SET_BIT(RCC->CR, RCC_CR_HSEON);
	/* Waiting for HSE started*/
	while (!READ_BIT(RCC->CR, RCC_CR_HSERDY))
	{;}

	SET_BIT(RCC->CR, RCC_CR_CSSON); // clock security
	/* AHB prescaler /1 (72MHz, (8MHz now)) */
	MODIFY_REG (RCC->CFGR, RCC_CFGR_HPRE_Msk, RCC_CFGR_HPRE_DIV1);
	/* APB1 prescaler /2 (36MHz(max)) (4MHz now)) */
	MODIFY_REG (RCC->CFGR, RCC_CFGR_PPRE1_Msk, RCC_CFGR_PPRE1_DIV2);
	/* APB high-speed prescaler (APB2) /1 */
	MODIFY_REG (RCC->CFGR, RCC_CFGR_PPRE2_Msk, RCC_CFGR_PPRE2_DIV1);
	/* adc prescaler /6 (14Mhz max) */
	MODIFY_REG (RCC->CFGR, RCC_CFGR_ADCPRE_Msk, RCC_CFGR_ADCPRE_DIV6);
	/* PLL src = HSE */
	MODIFY_REG (RCC->CFGR, RCC_CFGR_PLLSRC_Msk, 1<<RCC_CFGR_PLLSRC_Pos);
	/* PLL devider /1 */
	MODIFY_REG (RCC->CFGR, RCC_CFGR_PLLXTPRE_Msk, RCC_CFGR_PLLXTPRE_HSE);
	/* PLL MUL x9 */
	MODIFY_REG (RCC->CFGR, RCC_CFGR_PLLMULL9_Msk, RCC_CFGR_PLLMULL9);
	/* usb /1.5 (72/1.5=48) */
	MODIFY_REG (RCC->CFGR, RCC_CFGR_USBPRE_Msk, 0<<RCC_CFGR_USBPRE_Pos);

	/* run PLL */
	SET_BIT(RCC->CR, RCC_CR_PLLON);
	while (!READ_BIT(RCC->CR, RCC_CR_PLLRDY))
	{;} // waiting for PLL

	/* Flash latency = 2 wait state for 48<=CLK<=72 */
	MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY_Msk, 2<<FLASH_ACR_LATENCY_Pos);
	/* select PLL for sysclk */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
}

/**
  * @brief  Init systick timer @1ms.
  * 		It counts down from preload value, and generate interrupt.
  *
  * @retval None
  */
void bspInit_sysTimer(void)
{
	SysTick->CTRL &= ~(1 << SysTick_CTRL_ENABLE_Pos); 	//disable count
	SysTick->CTRL |= (1<<2);							//src = sysclk/1
	SysTick->LOAD = (SYSTEM_CLOCK_FREQ/1000U)-1;			//preload for 1ms int
	SysTick->VAL = (SYSTEM_CLOCK_FREQ/1000U)-1;			//preload for 1ms int

	/* enable count & interrupt */
	SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk) | (SysTick_CTRL_TICKINT_Msk);
}

void bspInit_GPIO(void)
{
	/* init PC13 (green led) */
	/* enable clocks portC on APB2 */
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);
	/* Push/PULL mode */
	MODIFY_REG(GPIOC->CRH, GPIO_CRH_CNF13_Msk, 0x00<<GPIO_CRH_CNF13_Pos);
	/* SPEED 50MHz */
	MODIFY_REG(GPIOC->CRH, GPIO_CRH_MODE13_Msk, (GPIO_CRH_MODE13_0 | GPIO_CRH_MODE13_1));
}

/**
  * @brief  Initialize UART.
  * 		Enable clocks, connect pins, select speed.
  * @retval None
  */
void bspInit_UART(void)
{

}




void delay_ms(uint32_t time_ms)
{
	delay_cnt_ms = time_ms;
	while (delay_cnt_ms)
	{;}
}


void bspCtl_LedOn(void){
	LED_GREEN_ON();
}

void bspCtl_LedOff(void){
	LED_GREEN_OFF();
}
void bspCtl_LedToggle(void){
	uint32_t odr = GPIOC->ODR;
	GPIOC->BSRR = (((odr & (1<<13)) << 16) | (~odr & (1<<13)));
}



void SysTick_Handler(void)
{
	sys_tick++;
	if (delay_cnt_ms)
		delay_cnt_ms--;
}
