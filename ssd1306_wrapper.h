/**
* @file ssd1306_wrapper.h
* @brief wrappers for ssd1306
*/

#ifndef __SSD1306_WRAPPER_H__
#define __SSD1306_WRAPPER_H__

#include <stdint.h>
/**
 * @brief delay ms wrapper
 */
void ssd1306_wrapper_delay(uint32_t ms);

/**
 * @brief transmit i2c data wrapper
 */
void ssd1306_wrapper_i2cTx(uint8_t dev_addr, uint8_t* buf, uint32_t size);

/**
 * @brief transmit i2c packet into memory by mem_addr wrapper
 */
void ssd1306_wrapper_i2cTx_mem(uint8_t dev_addr, uint8_t mem_addr, uint8_t* buf, uint32_t size);

uint32_t ssd1306_wrapper_get_tick(void);


#endif