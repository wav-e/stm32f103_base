#include "i2cDrv.h"
#include <stdio.h>


static int i2c_start(I2C_DRV *drv);
static int i2c_addr(I2C_DRV *drv, uint8_t DevAddress);
static int i2c_stop(I2C_DRV *drv);
static int i2c_write(I2C_DRV *drv, uint8_t *buf, uint32_t size);
static int i2c_writeMem(I2C_DRV *drv, uint8_t mem_addr, uint8_t *buf, uint32_t size);


/**
 * @brief 	gpio as alternate functions
 * @todo 	i2c1 other pins 
 * 
 */
void i2c_init_ll(I2C_DRV *drv){

	uint32_t base_reg = (uint32_t)drv->Instance;

	switch (base_reg)
	{
		case I2C1_BASE:{
			/* pb6=scl, pb7=sda (pb8=scl, pb9=sda) */
			/* clocks PB, AFIO, i2c1  */
			SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);
			SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN); 
			SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN);

			/* Alternate function open drain */
			MODIFY_REG(GPIOB->CRL, GPIO_CRL_CNF6_Msk, GPIO_CRL_CNF6_0 | GPIO_CRL_CNF6_1); 
			MODIFY_REG(GPIOB->CRL, GPIO_CRL_CNF7_Msk, GPIO_CRL_CNF7_0 | GPIO_CRL_CNF7_1); 

			/* output, SPEED 50MHz */
			MODIFY_REG(GPIOB->CRL, GPIO_CRL_MODE6_Msk, (GPIO_CRL_MODE6_0 | GPIO_CRL_MODE6_1));	
			MODIFY_REG(GPIOB->CRL, GPIO_CRL_MODE7_Msk, (GPIO_CRL_MODE7_0 | GPIO_CRL_MODE7_1));	

		}break;
		case I2C2_BASE: 
		{
			/* clocks PB, AFIO, i2c1  */
			SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);
			SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN); 
			SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C2EN);
			/* Alternate function open drain */
			/* pb10 = scl, pb11 = sda */

			/* Alternate function open drain */
			MODIFY_REG(GPIOB->CRH, GPIO_CRH_CNF10_Msk, GPIO_CRH_CNF10_0 | GPIO_CRH_CNF10_1); 
			MODIFY_REG(GPIOB->CRH, GPIO_CRH_CNF11_Msk, GPIO_CRH_CNF11_0 | GPIO_CRH_CNF11_1); 

			/* output, SPEED 50MHz */
			MODIFY_REG(GPIOB->CRH, GPIO_CRH_MODE10_Msk, (GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1));	
			MODIFY_REG(GPIOB->CRH, GPIO_CRH_MODE11_Msk, (GPIO_CRH_MODE11_0 | GPIO_CRH_MODE11_1));	


		}break;
		default: break;
	}
}


void i2cDrv_init(I2C_DRV *drv)
{
	/*
	The following is the required sequence in master mode.
	• Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
	• Configure the clock control registers
	• Configure the rise time register
	• Program the I2C_CR1 register to enable the peripheral
	• Set the START bit in the I2C_CR1 register to generate a Start condition
	The peripheral input clock frequency must be at least:
	• 2 MHz in Sm mode
	• 4 MHz in Fm mode
	*/


	drv->Instance = (uint32_t *)I2C1_BASE;
	/* now in master mode only */
	i2c_init_ll(drv);	
	drv->Instance->CR1 &= ~I2C_CR1_PE;

	drv->Instance->CR1 |= I2C_CR1_SWRST;
	drv->Instance->CR1 &= ~I2C_CR1_SWRST;


	//drv->Instance->CR1 |= I2C_CR1_NOSTRETCH;

	drv->Instance->OAR1 = 0x4000;
/*
	drv->Instance->CR2 = 0x32;
	drv->Instance->CCR = 0x8028; 
	drv->Instance->TRISE = 0xf;



*/

#if 1	
	/* CCR = (T_high + T_rise)/Pclk1 
	   CCR = (5000ms + 1000ns)/ (1000/36)ns = 180 */
	//drv->Instance->CCR = 0xB4;
	drv->Instance->CCR = 0x801E;


	/* Trise = (T_rise)/Pclk1 = 1000ns/ (1000/36ns) = 36 */	
	//drv->Instance->TRISE = 36;
	drv->Instance->TRISE = 0xb;

	drv->Instance->CR2 = 0x24; //0x08; /* apb1 speed match*/

	drv->Instance->CR1 |= I2C_CR1_PE;

#endif


}




/**
 * @todo
 * 	- check addr NACK
 * 
 * */
void i2cDrv_txMem(I2C_DRV *drv, uint8_t dev_adr, uint8_t mem_addr, uint8_t *data, uint32_t size)
{
	i2c_start(drv);
	i2c_addr(drv, dev_adr);
	i2c_writeMem(drv, mem_addr, data, size);
	i2c_stop(drv);

}


void i2cDrv_tx(I2C_DRV *drv, uint8_t dev_adr, uint8_t *data, uint32_t size)
{
	i2c_start(drv);
	i2c_addr(drv, dev_adr);
	i2c_write(drv, data, size);
	i2c_stop(drv);
}





static int check_ACK(I2C_DRV *drv)
{
	volatile uint32_t reg;

	reg = drv->Instance->SR1;
	while(0 == (reg & I2C_SR1_BTF))
	{
		if (reg & I2C_SR1_AF)
		{
			drv->errors++;
			printf("ack failing\r\n");
			return -1;
	    }

	    reg = drv->Instance->SR1;
	}
	return 0;
}



static int i2c_start(I2C_DRV *drv)
{
 	volatile uint32_t reg;

    /* Disable Pos */
    CLEAR_BIT(drv->Instance->CR1, I2C_CR1_POS);

    //drv->Instance->SR1 = (uint16_t)0x0;

	reg = drv->Instance->SR1;
	reg = drv->Instance->SR2;

    /* Generate Start */
    SET_BIT(drv->Instance->CR1, I2C_CR1_START);

  	/* Wait until SB flag is set */
    while(!(drv->Instance->SR1 & I2C_SR1_SB))
    {
    	//printf("1");
	}

	(void)reg;
	return 0;
}

static int i2c_stop(I2C_DRV *drv)
{
 	volatile uint32_t reg;

	SET_BIT(drv->Instance->CR1, I2C_CR1_STOP);
	reg = drv->Instance->SR1;
	reg = drv->Instance->SR2;

	/* it helps missing stop event and stuck bus */
	/* now achive 100% bus speed*/
	CLEAR_BIT(drv->Instance->CR1, I2C_CR1_STOP); 

	(void)reg;
	return 0;
}

/**
 * @brief sends addr 

 * @return 0 when ACK
 */
static int i2c_addr(I2C_DRV *drv, uint8_t DevAddress)
{
	volatile uint32_t reg;  
	uint32_t cnt = 0;  

  	/* Send slave address */
	drv->Instance->DR = DevAddress;


	cnt = 0;
	reg = drv->Instance->SR1;
  	/* Wait until ADDR flag is set */
	while((reg & I2C_SR1_ADDR) == 0)
	{
		if (1000000 == cnt++){
			//reg = drv->Instance->SR1;
			printf("i2c bus stuck! SR1 = 0x%04X, SR2 = 0x%04X" , drv->Instance->SR1, drv->Instance->SR2);
			//i2cDrv_init(drv);
			/* reinit bus is not helps */

			SET_BIT(drv->Instance->CR1, I2C_CR1_STOP);
			reg = drv->Instance->SR1;
			reg = drv->Instance->SR2;

			/* it helps missing stop event and stuck bus */
			/* now achive 100% bus speed*/
			//CLEAR_BIT(drv->Instance->CR1, I2C_CR1_STOP); 
			RCC->APB1RSTR = RCC_APB1RSTR_I2C1RST;
			i2cDrv_init(drv);
			return 2;
		}
		
		if (reg & I2C_SR1_AF)
		{

			//printf("2");
			/* ADDR NACK, Generate Stop */
			SET_BIT(drv->Instance->CR1, I2C_CR1_STOP);

			/* clear ACK FAIL flag */
			drv->Instance->SR1 &= ~(uint16_t)I2C_SR1_AF;


		  	return 1;
		}

		if (reg & I2C_SR1_BERR)
		{
			printf("i2c bus error\r\n");
		}
		if (reg & I2C_SR1_ARLO)
		{
			printf("i2c arbitration lost\r\n");
		}
		if (reg & I2C_SR1_OVR)
		{
			printf("i2c overrun\r\n");
		}
		if (reg & I2C_SR1_PECERR)
		{
			printf("i2c PECERR\r\n");
		}
		// if (0 == (reg & I2C_SR1_SB))
		// {
		// 	printf("i2c START proeban after adr\r\n");
		// }

		reg = drv->Instance->SR1; 
	}

	reg = drv->Instance->SR1;
	reg = drv->Instance->SR2;

	return 0;
}

static int i2c_write(I2C_DRV *drv, uint8_t *buf, uint32_t size)
{
	int res;

	while(size)
	{
		/* byte transfer finished? */
		//if ((drv->Instance->SR1 & I2C_SR1_BTF) == SET)
		drv->Instance->DR = *buf++;

		res = check_ACK(drv);
		if (res){
			return 1;
		}
		

		size--;
	}
	return 0;
}

static int i2c_read(I2C_DRV *drv, uint8_t *buf, uint32_t size)
{
	return 0;
}



/**
 * @todo working with 16-32bit address
 */
static int i2c_writeMem(I2C_DRV *drv, uint8_t mem_addr, uint8_t *buf, uint32_t size)
{
	int res;

	/* transmitting first byte. 
	   It sets BTF (byte transfer finished) flag */
	drv->Instance->DR = mem_addr;
	res = check_ACK(drv);
	if (res)
		return 1;

	while(size)
	{
		/* byte transfer finished? */
		//if ((drv->Instance->SR1 & I2C_SR1_BTF) == SET)
		drv->Instance->DR = *buf++;

		res = check_ACK(drv);
		if (res){
			return 1;
		}
		

		size--;
	}
	return 0;
}

static int i2c_readMem(I2C_DRV *drv, uint8_t mem_addr, uint8_t *buf, uint32_t size)
{
	#if 0
	uint32_t i;
	uint8_t res;

	set_ack();
	drv->Instance->DR = mem_addr;

	for (i=0; i<size; i++)
	{
		if (last_byte)
			set_NACK();
		else
			set_ACK();

		drv->Instance->DR = buf[i];
		res = check_ACK();
	}
	#endif

	return 0;
}





void i2cDrv_scanBusHal(I2C_DRV *drv)
{
	int res;
	for (uint8_t i=1; i<128; i++)
	{

			i2c_start(drv);
			res = i2c_addr(drv, i<<1);
			i2c_stop(drv);

			if (0 == res)
				printf("addr 0x%02X found\r\n", i);
			if (2 == res){
				printf("i2c stuck\r\n");

			}
	}
}


#if 0




/**** STEPS FOLLOWED  ************
1. Enable the ACK
2. Send the START condition 
3. Wait for the SB ( Bit 0 in SR1) to set. This indicates that the start condition is generated
*/	
void I2C_Start(void)
{
	I2C1->CR1 &= I2C_CR1_STOP;  // Generate START

	I2C1->CR1 |= (1<<10);  // Enable the ACK
	I2C1->CR1 |= (1<<8);  // Generate START
}

void I2C_Address (uint8_t addr)
{
	I2C1->DR = addr;  //  send the address
	while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set
	uint8_t temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit
}

/**** STEPS FOLLOWED  ************
1. Wait for the TXE (bit 7 in SR1) to set. This indicates that the DR is empty
2. Send the DATA to the DR Register
3. Wait for the BTF (bit 2 in SR1) to set. This indicates the end of LAST DATA transmission
*/
void I2C_Write(uint8_t data)
{
	while (!(I2C1->SR1 & (1<<7)));  // wait for TXE bit to set
	I2C1->DR = data;
	while (!(I2C1->SR1 & (1<<2)));  // wait for BTF bit to set
}

void I2C_Stop(void)
{
	SET_BIT(I2C1->CR1, I2C_CR1_STOP);
}

void master_write(I2C_DRV *drv)
{	
	I2C_Stop ();

	I2C_Start ();
	I2C_Address (0x3C<< 1);
	//I2C_Write (0x0);
	//I2C_Write (Data);
	I2C_Stop ();
}






static master_start(I2C_DRV *drv)
{
	volatile uint32_t reg;
	drv->Instance->CR1 &= ~I2C_CR1_POS;
	drv->Instance->CR1 |= I2C_CR1_START;

	reg = drv->Instance->SR1;
	while(!(reg & I2C_SR1_SB)){
		reg = drv->Instance->SR1;
		printf("start waiting SR1 = 0x%X\r\n",reg);
	}
	reg = drv->Instance->SR1;
}


void master_sendAdr(I2C_DRV *drv, uint8_t address)
{
	uint32_t reg1;
	uint32_t reg2;

	drv->Instance->DR = (address << 1) | 0x00;
	reg1 = drv->Instance->SR1;
	while(!(reg1 & I2C_SR1_ADDR)){
		reg1 = drv->Instance->SR1;
		printf("ADDR waining SR1 = 0x%X\r\n",reg1);
	}
	reg1 = 0x00;
	reg2 = 0;
	reg1 = drv->Instance->SR1;
	reg2 = drv->Instance->SR2;
	while(!(reg1 & I2C_SR1_TXE)){
		reg1 = drv->Instance->SR1;
		printf("TXE waining SR1 = 0x%X\r\n",reg1);
	}
}



#endif

