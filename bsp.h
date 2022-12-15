/******************************************************************************
 * @file 	bsp.h
 * @brief 	Contains macros and functions for BluePill platform
 *****************************************************************************/

#ifndef __BSP_H

#include "stm32f1xx.h"

#define LED_GREEN_ON() 			GPIOC->BSRR = GPIO_BSRR_BR13; /// enable led
#define LED_GREEN_OFF()			GPIOC->BSRR = GPIO_BSRR_BS13; /// disable led


//#define SysTick_Handler bspSysTick_Handler

#define SYSTEM_CLOCK_FREQ 		72000000



void bspCtl_LedOn(void);
void bspCtl_LedOff(void);
void bspCtl_LedToggle(void);



/**
  * @brief  Initialize a system clock. Clock init at 72MHz
  * @retval None
  */
void bspInit_sysClk(void);

void bspInit_sysTimer(void);

void delay_ms(uint32_t time_ms);

/**
  * @brief  Initialize gpio.
  * 		Enable clocks, connect function, select speed.
  *
  * @note	led init
  * @retval None
  */
void bspInit_GPIO(void);

void bspCtl_LedOn(void);
void bspCtl_LedOff(void);




/**
  * @brief  Initialize UART.
  * 		Enable clocks, connect pins, select speed.
  * @retval None
  */
void bsp_init_UART_pins(void);




#endif
