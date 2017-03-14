/*!
 *****************************************************************************
   @file:    adi_adc_config.h
   @brief:   Configuration options for ADC driver.
             This is specific to the ADC driver and will be included by the driver.
             It is not required for the application to include this header file.
   @version: $Revision$
   @date:    $Date$
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

#ifndef __ADI_ADC_CONFIG_H__
#define __ADI_ADC_CONFIG_H__
#include <adi_global_config.h>
/** @defgroup ADC_Driver_Cfg ADC Device Driver Configuration
 *  @ingroup Configuration_macros
 */

/** @addtogroup ADC_Driver_Cfg ADC Device Driver Configuration
*  @{
*/

/************* ADC Driver configurations ***************/


/*! Configure the default ADC configuration. Oversampling support must be enabled for resolution >12-bits.\n
    Valid values are 12 to 16
*/    
#define ADI_ADC_CFG_RESOLUTION                               (12)

/*! Configure the default Vref\n
    2 - External Reference
    1 - 2.5V  Internal Reference\n
    0 - 1.25V Internal Reference\n
*/    
#define ADI_ADC_CFG_VREF                                      (1)

/*! Configure the Mode of Operation for blocking API calls.\n
    1 - Use the same mode as non-blocking APIs\n
    0 - Polling Mode\n
*/    
#define ADI_ADC_CFG_BLOCKING_MODE                             (1)


/*! Configure the default mode of Operation for non-blocking API calls.\n
    1 - DMA Mode\n
    0 - Interrupt Mode\n
*/    
#define ADI_ADC_CFG_NON_BLOCKING_MODE                         (1)

/*! Enable/Disable DMA Support\n
    1 - Enable DMA Support\n
    0 - Disable DMA Support\n
*/    
#define ADI_ADC_ENABLE_DMA_SUPPORT                            (1)

/*! Enable/Disable Interrupt Mode Support\n
    1 - Enable Interrupt Mode Support\n
    0 - Disable Interrupt Mode Support\n
*/    
#define ADI_ADC_ENABLE_INTERRUPT_MODE_SUPPORT                 (1)





/************** Macro validation *****************************/

#if (ADI_ADC_CFG_RESOLUTION < 12) || (ADI_ADC_CFG_RESOLUTION > 16)
#error "ADI_ADC_CFG_RESOLUTION has invalid"
#endif

#if (ADI_ADC_CFG_VREF < 0) || (ADI_ADC_CFG_VREF > 2)
#error "ADI_ADC_CFG_VREF is invalid"
#endif

#if (ADI_ADC_CFG_BLOCKING_MODE != 0) && (ADI_ADC_CFG_BLOCKING_MODE != 1)
#error "ADI_ADC_CFG_BLOCKING_MODE has invalid"
#endif

#if (ADI_ADC_CFG_NON_BLOCKING_MODE != 0) && (ADI_ADC_CFG_NON_BLOCKING_MODE != 1)
#error "ADI_ADC_CFG_NON_BLOCKING_MODE has invalid"
#endif

#if (ADI_ADC_ENABLE_DMA_SUPPORT != 0) && (ADI_ADC_ENABLE_DMA_SUPPORT != 1)
#error "ADI_ADC_ENABLE_DMA_SUPPORT has invalid"
#endif

#if (ADI_ADC_ENABLE_INTERRUPT_MODE_SUPPORT != 0) && (ADI_ADC_ENABLE_INTERRUPT_MODE_SUPPORT != 1)
#error "ADI_ADC_ENABLE_INTERRUPT_MODE_SUPPORT has invalid"
#endif

#if ((ADI_ADC_ENABLE_DMA_SUPPORT == 0) && (ADI_ADC_ENABLE_INTERRUPT_MODE_SUPPORT == 0))
#error "DMA and INTERRUPT support cannot both be disabled"
#endif 

#if ((ADI_ADC_ENABLE_DMA_SUPPORT == 0) && (ADI_ADC_CFG_NON_BLOCKING_MODE == 1))
#error "Non-blocking mode cannot be set to DMA when the DMA support is not enabled"
#endif

#if ((ADI_ADC_ENABLE_INTERRUPT_MODE_SUPPORT == 0) && (ADI_ADC_CFG_NON_BLOCKING_MODE == 0))
#error "Non-blocking mode cannot be set to Interrupt mode when the Interrupt mode support is not enabled"
#endif


/*! @} */

#endif /* __ADI_ADC_CONFIG_H__ */
