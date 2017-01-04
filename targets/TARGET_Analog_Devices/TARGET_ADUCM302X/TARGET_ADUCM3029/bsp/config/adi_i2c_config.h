/*!
 *****************************************************************************
   @file:    adi_i2c_config.h
   @brief:   Configuration options for I2C driver.
             This is specific to the I2C driver and will be included by the driver.
             It is not required for the application to include this header file.
   @version: $Revision: 33205 $
   @date:    $Date: 2016-01-11 10:46:07 +0000 (Mon, 11 Jan 2016) $
  -----------------------------------------------------------------------------

Copyright (c) 2014 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

#ifndef __ADI_I2C_CONFIG_H__
#define __ADI_I2C_CONFIG_H__
#include <adi_global_config.h>

/** @defgroup I2C_Driver_Cfg I2C Device Driver Configuration
 *  @ingroup Configuration_macros
 */

/** @addtogroup I2C_Driver_Cfg I2C Device Driver Configuration
 *  @{
 */

/************* I2C Driver configurations ***************/

/*!
  Enable DMA support in the driver code.\n
  1 - To have the DMA support code in the driver.\n
  0 - To eliminate the DMA support.
*/
#define ADI_I2C_CFG_ENABLE_DMA_SUPPORT          (1)

/*!
  Enable PIO support in the driver code.\n
  1 - To have the PIO support code in the driver.\n
  0 - To eliminate the PIO support.
*/
#define ADI_I2C_CFG_ENABLE_PIO_SUPPORT          (1)

/*!
  Enable Master support in the driver code.\n
  1 - To have the Master support code in the driver.\n
  0 - To eliminate the Master support.
*/
#define ADI_I2C_CFG_ENABLE_MASTER_SUPPORT       (1)

/*!
  Enable Slave support in the driver code.\n
  1 - To have the Slave support code in the driver.\n
  0 - To eliminate the Slave support.
*/
#define ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT        (1)

/*!
  Enable/Disable DMA by default\n
  1 - Enable DMA by default\n
  0 - Disable DMA by default. PIO mode will be enabled by default.
*/
#define ADI_I2C_CFG_ENABLE_DMA                  (0)

/*!
  Enable/Disable General call for the slave\n
  1 - Enable General call\n
  0 - Disable General call
 */
#define ADI_CFG_I2C_ENABLE_GENERAL_CALL         (0)

/*!
  Enable/Disable internal loopback\n
  1 - Enable Internal Loopback\n
  0 - Disable Internal Loopback
 */
#define ADI_CFG_I2C_ENABLE_INTERNAL_LOOPBACK    (0)

/*!
 * Bit Rate to be configured in KHz
 */
#define ADI_CFG_I2C_BIT_RATE_IN_KHZ            (100u)

/*!
 * Duty cycle to be configured. Valid Range is (1-99)
 */
#define ADI_CFG_I2C_DUTY_CYCLE                 (50u)

/*!
 * Hardware Address Length in bits. Valid values are 7 and 10.
 */
#define ADI_CFG_I2C_HW_ADDR_LEN_IN_BITS        (7)

/*! Hardware address */
#define ADI_CFG_I2C_HWADDR                     (0u)

/*! Alternate Device ID used for Hardware general call */
#define ADI_CFG_I2C_ALT_DEV_ID                 (0u)

/*!
  Sets the maximum automatic SCL clock stretching for master\n

  0 - no SLK clock stretching\n
  1 - stretch SCL up to 2^1 = 2 bit-times\n
  2 - stretch SCL up to 2^2 = 4 bit-times\n
  ....\n
  14 - stretch SCL up to 2^14 bit-times\n
  15 - stretch SCL up to infinity (no timeout)\n
*/
#define ADI_CFG_I2C_MASTER_AUTO_STRETCH_EXP_LEN      (0)

/*!
  Sets the maximum automatic SCL clock stretching for slave\n

  0 - no SLK clock stretching\n
  1 - stretch SCL up to 2^1 = 2 bit-times\n
  2 - stretch SCL up to 2^2 = 4 bit-times\n
  ....\n
  14 - stretch SCL up to 2^14 bit-times\n
  15 - stretch SCL up to infinity (no timeout)\n
*/
#define ADI_CFG_I2C_SLAVE_AUTO_STRETCH_EXP_LEN       (0)



#if ((ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 0) && (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 0))
#error "Both Master and Slave cannot be disabled"
#endif

#if ((ADI_I2C_CFG_ENABLE_DMA_SUPPORT == 0) && (ADI_I2C_CFG_ENABLE_PIO_SUPPORT == 0))
#error "Both DMA and PIO support cannot be disabled"
#endif

#if ((ADI_I2C_CFG_ENABLE_DMA == 1) && (ADI_I2C_CFG_ENABLE_DMA_SUPPORT == 0))
#error "DMA cannot be enabled if DMA support is disabled"
#endif

#if ((ADI_I2C_CFG_ENABLE_DMA == 0) && (ADI_I2C_CFG_ENABLE_PIO_SUPPORT == 0))
#error "DMA cannot be disabled if PIO support is disabled"
#endif

#if ((ADI_CFG_I2C_DUTY_CYCLE < 1u) || (ADI_CFG_I2C_DUTY_CYCLE > 99u))
#error "Duty Cycle should be a value between 1-99"
#endif

#if ((ADI_CFG_I2C_HW_ADDR_LEN_IN_BITS != 7) && (ADI_CFG_I2C_HW_ADDR_LEN_IN_BITS != 10))
#error "Hardware Address Length should be 7bits or 10 bits"
#endif

#if ((ADI_CFG_I2C_MASTER_AUTO_STRETCH_EXP_LEN > 15) ||  (ADI_CFG_I2C_MASTER_AUTO_STRETCH_EXP_LEN < 0))
#error "Master Auto Stretch Exp Len should be in the range 0-15"
#endif

#if ((ADI_CFG_I2C_SLAVE_AUTO_STRETCH_EXP_LEN > 15) ||  (ADI_CFG_I2C_SLAVE_AUTO_STRETCH_EXP_LEN < 0))
#error "Slave Auto Stretch Exp Len should be in the range 0-15"
#endif

/*! @} */

#endif /* __ADI_I2C_CONFIG_H__ */
