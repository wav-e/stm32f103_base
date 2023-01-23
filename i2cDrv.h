/**
* \file 	i2cDrv.h
* \brief 	i2c Driver.
*/

#ifndef __I2C_DRV__
#define __I2C_DRV__

#include <stdint.h>
#include "stm32f1xx.h"


/**
 * @brief  I2C Configuration Structure definition
 *
 */
typedef struct
{
  uint32_t ClockSpeed;       /*!< Specifies the clock frequency.
                                  This parameter must be set to a value lower than 400kHz */

  uint32_t DutyCycle;        /*!< Specifies the I2C fast mode duty cycle.
                                  This parameter can be a value of @ref I2C_duty_cycle_in_fast_mode */

  uint8_t  isInit;           /*!< driver init flag */

  // uint32_t OwnAddress1;      /*!< Specifies the first device own address.
  //                                 This parameter can be a 7-bit or 10-bit address. */

  // uint32_t AddressingMode;   /*!< Specifies if 7-bit or 10-bit addressing mode is selected.
  //                                 This parameter can be a value of @ref I2C_addressing_mode */

  // uint32_t DualAddressMode;  /*!< Specifies if dual addressing mode is selected.
  //                                 This parameter can be a value of @ref I2C_dual_addressing_mode */

  // uint32_t OwnAddress2;      /*!< Specifies the second device own address if dual addressing mode is selected
  //                                 This parameter can be a 7-bit address. */

  // uint32_t GeneralCallMode;  /*!< Specifies if general call mode is selected.
  //                                 This parameter can be a value of @ref I2C_general_call_addressing_mode */

  // uint32_t NoStretchMode;    /*!< Specifies if nostretch mode is selected.
  //                                 This parameter can be a value of @ref I2C_nostretch_mode */

} I2C_InitTypeDef;

typedef struct
{
  I2C_TypeDef                *Instance;      /*!< I2C registers base address               */

  I2C_InitTypeDef            Init;           /*!< I2C communication parameters             */

  uint8_t                    *pBuffPtr;      /*!< Pointer to I2C transfer buffer           */

  uint16_t                   XferSize;       /*!< I2C transfer size                        */

  __IO uint16_t              XferCount;      /*!< I2C transfer counter                     */

  uint32_t                   errors;          /*!< I2C transfer errors                     */

} I2C_DRV;





void i2cDrv_init(I2C_DRV *drv);
void i2cDrv_tx(I2C_DRV *drv, uint8_t dev_adr, uint8_t *data, uint32_t size);

void i2cDrv_txMem(I2C_DRV *drv, uint8_t dev_adr, uint8_t mem_addr, uint8_t *data, uint32_t size);
void i2cDrv_scanBus();

#endif

