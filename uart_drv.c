#include "uart_drv.h"
#include <stddef.h>
#include "stm32f1xx.h"



/** transmit data from uart */
UART_STATUS uartDrv_init(UART_DRV *uart)
{

#if 0 /* universal driver */
	uint32_t tmpreg = 0;

	disable_uart_irq();

	if (uart == NULL) 
		return UART_ERROR;
	uart->init.is_init = 0;
	uart->tx_busy = 1;
	uart->rx_busy = 1;


	switch (uart->init.uart_number)
	{
		case 1: uart->base_addr = (uint32_t*)1; break;
		case 2: uart->base_addr = (uint32_t*)2; break;
		case 3: uart->base_addr = (uint32_t*)3; break;
		default: return UART_ERROR; break;
	}

	uart->init.is_init = 1;
	return UART_OK;
#else /* hard driver */

	/* use uart 3 */
	
	/* enable uart3 clock */

	/* хз надо ли порт B включать*/
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);
	//SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN); 
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);

	/* Push/PULL mode */
	MODIFY_REG(GPIOB->CRH, GPIO_CRH_CNF10_Msk, 0x2<<GPIO_CRH_CNF10_Pos); /* push/pull alternate */
	/* SPEED 50MHz */
	MODIFY_REG(GPIOB->CRH, GPIO_CRH_MODE10_Msk, (GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1));	


	/* set baudrate, parity, data bits */	
	//baud = fCK /(16*USARTDIV);
	//fCK - Input clock to the peripheral (PCLK1 for USART2, 3, 4, 5 or PCLK2 for USART1)

	/* uart3 is 36Mhz */
	/* calculation USARTDIV
	9600 = 36000000/(16*USARTDIV)
	USARTDIV = 36000000 / (16 * 9600) = 234,375
	MANT = 234 = 0xEA
	FRAC = round(0,375*16) = 6
	BRR = (MANT<<4 | FRAC)
	*/
	USART3->BRR = UART_BRR_SAMPLING16(36000000, 115200);

	/* 1start 8bit, no parity, TX,RX, no Int */
	USART3->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

	/* 1 stop bit*/
	USART3->CR2; 

	/* do nothing */ 
	USART3->CR3; 
	//USART_CR1
	//USART_CR2
	//USART_CR3
	/* set */

#endif
}

/** transmit data from uart */
UART_STATUS uartDrv_transmitt(UART_DRV *uart, uint8_t* msg, uint32_t size)
{
	for (uint32_t i=0; i<size; i++)
	{
		USART3->DR = msg[i];
		while(!(USART3->SR & USART_SR_TC));
	}
}

/** receive data from uart with interrupts */
UART_STATUS uartDrv_receive()
{

}

/** transmit data from uart with interrupts */
UART_STATUS uartDrv_transmitt_it()
{}

/** receive data from uart with interrupts */
UART_STATUS uartDrv_receive_it()
{}
