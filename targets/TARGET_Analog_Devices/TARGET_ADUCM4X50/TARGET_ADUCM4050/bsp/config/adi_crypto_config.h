/*!
 *****************************************************************************
   @file:    adi_crypto_config.h
   @brief:   Configuration options for Crypto driver.
             This is specific to the Crypto driver and will be included by the driver.
             It is not required for the application to include this header file.
  -----------------------------------------------------------------------------

Copyright (c) 2014-2016 Analog Devices, Inc.

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

#ifndef __ADI_CRYPTO_CONFIG_H__
#define __ADI_CRYPTO_CONFIG_H__
#include <adi_global_config.h>

/** @addtogroup Crypto_Driver_Config Static Configuration
 *  @ingroup Crypto_Driver
 *  @{
 */

/************* Crypto Driver configurations ***************/

/*! Enable/Disable ECB Support\n
    1 - Enable ECB Support\n
    0 - Disable ECB Support\n
*/
#define ADI_CRYPTO_ENABLE_ECB_SUPPORT            (1)

/*! Enable/Disable CTR Support\n
    1 - Enable CTR Support\n
    0 - Disable CTR Support\n
*/
#define ADI_CRYPTO_ENABLE_CTR_SUPPORT            (1)

/*! Enable/Disable CBC Support\n
    1 - Enable CBC Support\n
    0 - Disable CBC Support\n
*/
#define ADI_CRYPTO_ENABLE_CBC_SUPPORT            (1)

/*! Enable/Disable CCM Support\n
    1 - Enable CCM Support\n
    0 - Disable CCM Support\n
*/
#define ADI_CRYPTO_ENABLE_CCM_SUPPORT            (1)

/*! Enable/Disable CMAC Support\n
    1 - Enable CMAC Support\n
    0 - Disable CMAC Support\n
*/
#define ADI_CRYPTO_ENABLE_CMAC_SUPPORT           (1)

/*! Enable/Disable HMAC Support\n
    1 - Enable HMAC Support\n
    0 - Disable HMAC Support\n
*/
#define ADI_CRYPTO_ENABLE_HMAC_SUPPORT           (1)

/*! Enable/Disable SHA Support\n
    1 - Enable SHA Support\n
    0 - Disable SHA Support\n
*/
#define ADI_CRYPTO_ENABLE_SHA_SUPPORT            (1)


/*! Enable/Disable DMA Support\n
    1 - Enable DMA Support\n
    0 - Disable DMA Support
*/
#define ADI_CRYPTO_ENABLE_DMA_SUPPORT            (1)

/*! Enable/Disable DMA Transfer by default\n
    1 - Enable DMA \n
    0 - Disable DMA
*/
#define ADI_CRYPTO_ENABLE_DMA                    (1)

/*! SHA output format\n
    1 - Big-Endian \n
    0 - Little-Endian
*/
#define ADI_CRYPTO_SHA_OUTPUT_FORMAT             (1)



/************** Macro validation *****************************/

#if ((ADI_CRYPTO_ENABLE_DMA_SUPPORT != 0) && (ADI_CRYPTO_ENABLE_DMA_SUPPORT != 1))
#error "ADI_CRYPTO_ENABLE_DMA_SUPPORT is invalid"
#endif

#if ((ADI_CRYPTO_ENABLE_DMA != 0) && (ADI_CRYPTO_ENABLE_DMA != 1))
#error "ADI_CRYPTO_ENABLE_DMA is invalid"
#endif

#if ((ADI_CRYPTO_ENABLE_DMA == 1) && (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 0))
#error "DMA cannot be enabled if DMA support is disabled"
#endif

/*! @} */

#endif /* __ADI_CRYPTO_CONFIG_H__ */
