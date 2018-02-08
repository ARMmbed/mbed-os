/*!
 *****************************************************************************
   @file:    adi_rng_config.h
   @brief:   Configuration options for RNG driver.
             This is specific to the RNG driver and will be included by the driver.
             It is not required for the application to include this header file.
  -----------------------------------------------------------------------------

Copyright (c) 2012-2016 Analog Devices, Inc.

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

#ifndef ADI_RNG_CONFIG_H__
#define ADI_RNG_CONFIG_H__
#include <adi_global_config.h>
/** @defgroup RNG_Driver_Cfg RNG Driver Configuration
 *  @ingroup RNG_Driver

 */

/*! \addtogroup  RNG_Driver_Cfg RNG Driver Configuration
 *  @{
 */

/************* RNG Driver configurations ***************/

/************* RNG controller configurations ***************/

/*! RNG Control Register, bit 3\n
    Enable only 8-bit generation\n
    0 -  Generate 32-bit random number\n
    1 -  Generate only 8-bit random number
*/
#define RNG0_CFG_ONLY_8_BIT                             1

/*! RNG Sample Length Register, bits [11:0]\n
    The register defines the number of samples to accumulate in the
    CRC register when generating a random number.\n

    Bits [11:0] contains the reload value of the sample counter

    */
#define RNG0_CFG_LENGTH_RELOAD                          256u

/*! RNG Sample Length Register, bits [15:12]\n
    The register defines the number of samples to accumulate in the
    CRC register when generating a random number. The number of values
    accumulated in the counter reload value is scaled by 2^prescaler.\n

    Bits [15:12] contains the prescaler for the sample counter

    */
#define RNG0_CFG_LENGTH_PRESCALER                       0u

/************** Macro validation *****************************/

#if ( RNG0_CFG_ONLY_8_BIT > 1 )
#error "Invalid configuration"
#endif

#if ( RNG0_CFG_LENGTH_RELOAD > 4095u )
#error "Invalid value for reload"
#endif

#if ( RNG0_CFG_LENGTH_PRESCALER > 10u )
#error "Invalid value for prescaler"
#endif

/*! @} */

#endif /* __ADI_RNG_CONFIG_H__ */
