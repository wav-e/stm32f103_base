/**
* @file   startup.c
* @brief  analogue legacy .s file
*/

#include <stdint.h>
#include "main.c"

#define SRAM_START 		0x20000000U
#define SRAM_SIZE 		(20U * 1024U) //20kb
#define SRAM_END		((SRAM_START)+(SRAM_SIZE))

#define STACK_START		SRAM_END

extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

void Reset_handler(void);

/* other exeption handlers prototipes */
/* if any undeclared - it calls Default_handler */
void NMI_Handler				(void) __attribute__ ((weak,alias("Default_handler")));
void HardFault_Handler			(void) __attribute__ ((weak,alias("Default_handler")));
void MemManage_Handler			(void) __attribute__ ((weak,alias("Default_handler")));
void BusFault_Handler			(void) __attribute__ ((weak,alias("Default_handler")));
void UsageFault_Handler			(void) __attribute__ ((weak,alias("Default_handler")));
void SVC_Handler				(void) __attribute__ ((weak,alias("Default_handler")));
void DebugMon_Handler			(void) __attribute__ ((weak,alias("Default_handler")));
void PendSV_Handler				(void) __attribute__ ((weak,alias("Default_handler")));
void SysTick_Handler			(void) __attribute__ ((weak,alias("Default_handler")));
void WWDG_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void PVD_IRQHandler				(void) __attribute__ ((weak,alias("Default_handler")));
void TAMPER_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void RTC_IRQHandler				(void) __attribute__ ((weak,alias("Default_handler")));
void FLASH_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void RCC_IRQHandler				(void) __attribute__ ((weak,alias("Default_handler")));
void EXTI0_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void EXTI1_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void EXTI2_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void EXTI3_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void EXTI4_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void DMA1_Channel1_IRQHandler	(void) __attribute__ ((weak,alias("Default_handler")));
void DMA1_Channel2_IRQHandler	(void) __attribute__ ((weak,alias("Default_handler")));
void DMA1_Channel3_IRQHandler	(void) __attribute__ ((weak,alias("Default_handler")));
void DMA1_Channel4_IRQHandler	(void) __attribute__ ((weak,alias("Default_handler")));
void DMA1_Channel5_IRQHandler	(void) __attribute__ ((weak,alias("Default_handler")));
void DMA1_Channel6_IRQHandler	(void) __attribute__ ((weak,alias("Default_handler")));
void DMA1_Channel7_IRQHandler	(void) __attribute__ ((weak,alias("Default_handler")));
void ADC1_2_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void USB_HP_CAN1_TX_IRQHandler	(void) __attribute__ ((weak,alias("Default_handler")));
void USB_LP_CAN1_RX0_IRQHandler	(void) __attribute__ ((weak,alias("Default_handler")));
void CAN1_RX1_IRQHandler		(void) __attribute__ ((weak,alias("Default_handler")));
void CAN1_SCE_IRQHandler		(void) __attribute__ ((weak,alias("Default_handler")));
void EXTI9_5_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void TIM1_BRK_IRQHandler		(void) __attribute__ ((weak,alias("Default_handler")));
void TIM1_UP_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void TIM1_TRG_COM_IRQHandler	(void) __attribute__ ((weak,alias("Default_handler")));
void TIM1_CC_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void TIM2_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void TIM3_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void TIM4_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void I2C1_EV_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void I2C1_ER_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void I2C2_EV_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void I2C2_ER_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void SPI1_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void SPI2_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void USART1_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void USART2_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void USART3_IRQHandler			(void) __attribute__ ((weak,alias("Default_handler")));
void EXTI15_10_IRQHandler		(void) __attribute__ ((weak,alias("Default_handler")));
void RTC_Alarm_IRQHandler		(void) __attribute__ ((weak,alias("Default_handler")));
void USBWakeUp_IRQHandler		(void) __attribute__ ((weak,alias("Default_handler")));
void BootRAM 					(void) __attribute__ ((weak,alias("Default_handler")));



uint32_t vectors[] __attribute__ ((section (".isr_vector"))) = {
	STACK_START,			
	(uint32_t)Reset_handler,
	(uint32_t)NMI_Handler,
	(uint32_t)HardFault_Handler,
	(uint32_t)MemManage_Handler,
	(uint32_t)BusFault_Handler,
	(uint32_t)UsageFault_Handler,
	(uint32_t)0,
	(uint32_t)0,
	(uint32_t)0,
	(uint32_t)0,
	(uint32_t)SVC_Handler,
	(uint32_t)DebugMon_Handler,
	(uint32_t)0,
	(uint32_t)PendSV_Handler,
	(uint32_t)SysTick_Handler,
	(uint32_t)WWDG_IRQHandler,
	(uint32_t)PVD_IRQHandler,
	(uint32_t)TAMPER_IRQHandler,
	(uint32_t)RTC_IRQHandler,
	(uint32_t)FLASH_IRQHandler,
	(uint32_t)RCC_IRQHandler,
	(uint32_t)EXTI0_IRQHandler,
	(uint32_t)EXTI1_IRQHandler,
	(uint32_t)EXTI2_IRQHandler,
	(uint32_t)EXTI3_IRQHandler,
	(uint32_t)EXTI4_IRQHandler,
	(uint32_t)DMA1_Channel1_IRQHandler,
	(uint32_t)DMA1_Channel2_IRQHandler,
	(uint32_t)DMA1_Channel3_IRQHandler,
	(uint32_t)DMA1_Channel4_IRQHandler,
	(uint32_t)DMA1_Channel5_IRQHandler,
	(uint32_t)DMA1_Channel6_IRQHandler,
	(uint32_t)DMA1_Channel7_IRQHandler,
	(uint32_t)ADC1_2_IRQHandler,
	(uint32_t)USB_HP_CAN1_TX_IRQHandler,
	(uint32_t)USB_LP_CAN1_RX0_IRQHandler,
	(uint32_t)CAN1_RX1_IRQHandler,
	(uint32_t)CAN1_SCE_IRQHandler,
	(uint32_t)EXTI9_5_IRQHandler,
	(uint32_t)TIM1_BRK_IRQHandler,
	(uint32_t)TIM1_UP_IRQHandler,
	(uint32_t)TIM1_TRG_COM_IRQHandler,
	(uint32_t)TIM1_CC_IRQHandler,
	(uint32_t)TIM2_IRQHandler,
	(uint32_t)TIM3_IRQHandler,
	(uint32_t)TIM4_IRQHandler,
	(uint32_t)I2C1_EV_IRQHandler,
	(uint32_t)I2C1_ER_IRQHandler,
	(uint32_t)I2C2_EV_IRQHandler,
	(uint32_t)I2C2_ER_IRQHandler,
	(uint32_t)SPI1_IRQHandler,
	(uint32_t)SPI2_IRQHandler,
	(uint32_t)USART1_IRQHandler,
	(uint32_t)USART2_IRQHandler,
	(uint32_t)USART3_IRQHandler,
	(uint32_t)EXTI15_10_IRQHandler,
	(uint32_t)RTC_Alarm_IRQHandler,
	(uint32_t)USBWakeUp_IRQHandler,
	(uint32_t)0,
	(uint32_t)0,
	(uint32_t)0,
	(uint32_t)0,
	(uint32_t)0,
	(uint32_t)0,
	(uint32_t)0,
	(uint32_t)BootRAM 
};

void Reset_handler(void)
{
	uint32_t size;
	uint8_t* pSrc;
	uint8_t* pDst;

	/* copy .data section to sram */
	size = (uint32_t)&_edata - (uint32_t)&_sdata;
	pSrc = (uint8_t*)&_etext;
	pDst = (uint8_t*)&_sdata;

	for (uint32_t i=0; i<size; i++){
		*pDst++ = *pSrc++;
	}
	/* init .bss section to 0 in sram */
	size = (uint32_t)&_ebss - (uint32_t)&_sbss;
	pDst = (uint8_t*)&_sbss;

	for (uint32_t i=0; i<size; i++)
	{
		*pDst++ = 0;
	}
	/* call main */

	main();
}

void Default_handler(void)
{
	while(1){;}	
}


