#include "ssd1306_wrapper.h"
#include "i2cDrv.h"
#include "bsp.h"

extern I2C_DRV i2c1;


void ssd1306_wrapper_delay(uint32_t ms)
{
	delay_ms(ms);
}

void ssd1306_wrapper_i2cTx(uint8_t dev_addr, uint8_t* buf, uint32_t size)
{
	
}


void ssd1306_wrapper_i2cTx_mem(uint8_t dev_addr, uint8_t mem_addr, uint8_t* buf, uint32_t size)
{
	i2cDrv_txMem(&i2c1, dev_addr, mem_addr, buf, size);
}


uint32_t ssd1306_wrapper_get_tick(void)
{
	return bsp_getTick();
}
