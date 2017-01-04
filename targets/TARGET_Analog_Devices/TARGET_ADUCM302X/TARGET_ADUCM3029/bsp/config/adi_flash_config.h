/*!
 *****************************************************************************
   @file:    adi_flash_config.h
   @brief:   Configuration options for flash driver.
             This is specific to the flash driver and will be included by the driver.
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

#ifndef __ADI_FLASH_CONFIG_H__
#define __ADI_FLASH_CONFIG_H__
#include <adi_global_config.h>
/** @defgroup Flash_Driver_Cfg Flash Device Driver Configuration
 *  @ingroup Configuration_macros
 */

/*! \addtogroup Flash_Driver_Cfg Flash Device Driver Configuration
 *  @{
 */

/************* Flash Driver configurations ***************/
/*!
 * ECC Start Page
 */
#define ADI_FEE_CFG_ECC_START_PAGE              (0u)

/*!
 * Enable/Disable DMA by default\n
 * 1 - Enable DMA\n
 * 0 - Disable DMA
 */
#define ADI_FEE_CFG_ENABLE_DMA                  (1)

/*!
 * Enable/Disable ECC for info space\n
 * 1 - Enable Info Space\n
 * 0 - Disable Info Space
 */
#define ADI_FEE_CFG_ENABLE_ECC_FOR_INFO_SPACE   (0)

/*!
 * Enable/Disable ECC\n
 * 1 - Enable ECC\n
 * 0 - Disable ECC
 */
#define ADI_FEE_CFG_ENABLE_ECC                  (0)

/*!
 * Configure a response to the ECC error event\n
 * 0 - Do not generate a response to ECC Error Events\n
 * 1 - Generate Bus Errors in response to ECC Error Events\n
 * 2 - Generate IRQs in response to ECC Error Events
 */
#define ADI_FEE_CFG_ECC_ERROR_RESPONSE          (1)

/*!
 * Configure a response to the ECC correction events\n
 * 0 - Do not generate a response to ECC correction Events\n
 * 1 - Generate Bus Errors in response to ECC correction Events\n
 * 2 - Generate IRQs in response to ECC correction Events
 */
#define ADI_FEE_CFG_ECC_CORRECTION_RESPONSE     (2)




/*!
 * Enable/Disable Interrupt Support\n
 * 1 - Enable Interrupt Support\n
 * 0 - Disable Interrupt Support
 */
#define ADI_FEE_CFG_ENABLE_INTERRUPT_SUPPORT    (1)

/*!
 * Enable/Disable DMA Support\n
 * 1 - Enable DMA Support\n
 * 0 - Disable DMA Support
 */
#define ADI_FEE_CFG_ENABLE_DMA_SUPPORT          (1)





#if ((ADI_FEE_CFG_ENABLE_ECC_FOR_INFO_SPACE != 0) && (ADI_FEE_CFG_ENABLE_ECC_FOR_INFO_SPACE != 1))
#error "ADI_FEE_CFG_ENABLE_ECC_FOR_INFO_SPACE should be 1 or 0"
#endif

#if ((ADI_FEE_CFG_ENABLE_ECC != 0) && (ADI_FEE_CFG_ENABLE_ECC != 1))
#error "ADI_FEE_CFG_ENABLE_ECC should be 1 or 0"
#endif

#if ((ADI_FEE_CFG_ECC_ERROR_RESPONSE < 0) || (ADI_FEE_CFG_ECC_ERROR_RESPONSE > 2))
#error "ADI_FEE_CFG_ECC_ERROR_RESPONSE should be in the range 0-2"
#endif

#if ((ADI_FEE_CFG_ECC_CORRECTION_RESPONSE < 0) || (ADI_FEE_CFG_ECC_CORRECTION_RESPONSE > 2))
#error "ADI_FEE_CFG_ECC_CORRECTION_RESPONSE should be in the range 0-2"
#endif

#if ((ADI_FEE_CFG_ENABLE_INTERRUPT_SUPPORT != 0) && (ADI_FEE_CFG_ENABLE_INTERRUPT_SUPPORT != 1))
#error "ADI_FEE_CFG_ENABLE_INTERRUPT_SUPPORT should be 1 or 0"
#endif

#if ((ADI_FEE_CFG_ENABLE_DMA_SUPPORT == 0) && ( ADI_FEE_CFG_ENABLE_DMA != 0))
#error "DMA cannot be enabled if the Flash controller is configured not to support DMA"
#endif


/*! @} */

#endif /* __ADI_FLASH_CONFIG_H__ */
