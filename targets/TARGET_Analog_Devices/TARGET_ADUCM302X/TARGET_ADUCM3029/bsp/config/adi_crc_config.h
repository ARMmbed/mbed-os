/*!
 *****************************************************************************
   @file:    adi_crc_config.h
   @brief:   Configuration options for CRC driver.
             This is specific to the CRC driver and will be included by the driver.
             It is not required for the application to include this header file.
  -----------------------------------------------------------------------------

Copyright (c) 2016 Analog Devices, Inc.

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
    or more patent holders. This license does not release you from the
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

#ifndef ADI_CRC_CONFIG_H
#define ADI_CRC_CONFIG_H

#include <adi_global_config.h>

/** @defgroup CRC_Driver_Cfg Static Configuration
 *  @ingroup CRC_Driver
 */

/** @addtogroup CRC_Driver_Cfg Static Configuration
*  @{
*/

/************* CRC Driver configurations ***************/
/*!
   Enable DMA support in the driver code.\n
   1 -  To have the DMA support code in the driver.\n
   0 -  To eliminate the DMA support. Operates in core mode.\n
*/
#define ADI_CRC_CFG_ENABLE_DMA_SUPPORT         0

/*!
   Enable Byte mirroring option\n
   1 -  To enable byte mirroring \n
   0 -  To disable the byte mirroring.
*/
#define ADI_CFG_CRC_ENABLE_BYTE_MIRRORING      0
/*!
   Enable Bit mirroring option\n
   1 -  To enable bit mirroring \n
   0 -  To disable the bit mirroring.
*/
#define ADI_CFG_CRC_ENABLE_BIT_MIRRORING       0

/*!
   To specify the seed value for CRC computation
*/

#define ADI_CFG_CRC_SEED_VALUE                 (0xFFFFFFFFu)

/*!
   To specify the polynomial to be used for CRC computation
*/
#define ADI_CFG_CRC_POLYNOMIAL                 (0x04C11DB7u)

/*!
   To specify the Software DMA channel to be used for the CRC computation
   0 -> DMA channel SIP0, ..., 7 -> DMA channel SIP7
*/
#define ADI_CFG_CRC_SOFTWARE_DMA_CHANNEL_ID     7

#endif /* ADI_CRC_CONFIG_H */
/*! @} */
