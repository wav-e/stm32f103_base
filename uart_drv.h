/** UART driver */

#ifndef __UART_DRV_H__
#define __UART_DRV_H__

#include <stdint.h>

typedef enum
{
	UART_OK = 0,
	UART_BUSY,
	UART_ERROR

}UART_STATUS;
/*

typedef struct
{
	uint8_t busy_tx;
	uint8_t busy_rx;
}UART_STATUS

*/

typedef struct
{
	uint32_t 	speed;
	uint8_t	 	data_bit;
	uint8_t	 	stop_bit;
	uint8_t	 	parity_bit;
	uint8_t		uart_number;
	uint8_t		is_init;
}UART_DRV_SETTINGS;

typedef struct
{
	uint32_t* 			base_addr;
	uint8_t 			tx_busy;	/// uart not complete transmitt
	uint8_t 			rx_busy;   /// uart not complete receive
	UART_DRV_SETTINGS	init;
}
UART_DRV;


/** transmit data from uart */
UART_STATUS uartDrv_init();

/** transmit data from uart */
UART_STATUS uartDrv_transmitt();

/** receive data from uart with interrupts */
UART_STATUS uartDrv_receive();

/** transmit data from uart with interrupts */
UART_STATUS uartDrv_transmitt_it();

/** receive data from uart with interrupts */
UART_STATUS uartDrv_receive_it();



#define UART_DIV_SAMPLING16(_PCLK_, _BAUD_)            (((_PCLK_)*25U)/(4U*(_BAUD_)))
#define UART_DIVMANT_SAMPLING16(_PCLK_, _BAUD_)        (UART_DIV_SAMPLING16((_PCLK_), (_BAUD_))/100U)
#define UART_DIVFRAQ_SAMPLING16(_PCLK_, _BAUD_)        ((((UART_DIV_SAMPLING16((_PCLK_), (_BAUD_)) - (UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) * 100U)) * 16U) + 50U) / 100U)
/* UART BRR = mantissa + overflow + fraction
            = (UART DIVMANT << 4) + (UART DIVFRAQ & 0xF0) + (UART DIVFRAQ & 0x0FU) */
#define UART_BRR_SAMPLING16(_PCLK_, _BAUD_)            (((UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) << 4U) + \
                                                        (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0xF0U)) + \
                                                        (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0x0FU))

#endif