/*!
 *****************************************************************************
   @file:    adi_adc_config.h
   @brief:   Configuration options for ADC driver.
             This is specific to the ADC driver and will be included by the driver.
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

#ifndef ADI_ADC_CONFIG_H
#define ADI_ADC_CONFIG_H
#include <adi_global_config.h>
/** @defgroup ADC_Driver_Cfg Static Configuration
 *  @ingroup ADC_Driver
 */

/** @addtogroup ADC_Driver_Cfg Static Configuration
*  @{
*/

/************* ADC Driver configurations ***************/


/*! Configure the default ADC configuration. Oversampling support must be enabled for resolution >12-bits.\n
    Valid values are 12 to 16
*/    
#define ADI_ADC_CFG_RESOLUTION                               (12)

/*! Configure the default Vref\n
    3 - External Reference
    2 - Battery Voltage
    1 - 2.5V  Internal Reference\n
    0 - 1.25V Internal Reference\n

*/    
#define ADI_ADC_CFG_VREF                                      (1)

/*! Enable/Disable MULTI acquisitions of ADC data.
    When enabled, DMA will be used for ADC readings which is
    the preferred transfer method for multiple transactions.
    Otherwise all will be interrupt driven. \n
    1 - Enable MULTI (DMA) acquisitions \n
    0 - Disable MULTI (use Interrupt) acquisitions \n
*/ 
#define ADI_ADC_ENABLE_MULTI_ACQUIRE              (1)

/*! Enable/Disable HI/LO Digital Comparator limits \n
    1 - Enable HI/LO Digital Comparator limits\n
    0 - Disable HI/LO Digital Comparator limits\n
*/    
#define ADI_ADC_ENABLE_STATIC_COMPARATOR           (1)

/*! Enable/Disable Channel0 limit comparator \n
    1 - Enable HI Digital Comparator limit\n
    0 - Disable HI Digital Comparator limit\n
*/
#define ADI_ADC_COMPARATOR_AIN0_HI_EN              (0)    /* 0 or 1 */

/*! Set the Channel0 limit comparator value \n
    Sets the HI limit value for the channel, only \n
    relevant if ADI_ADC_COMPARATOR_AIN0_HI_EN is set to 1.\n
*/
#define ADI_ADC_COMPARATOR_AIN0_HI_VAL             (4095) /* Range: 0 to 4095 */

/*! Enable/Disable Channel0 limit comparator \n
    1 - Enable LO Digital Comparator limit\n
    0 - Disable LO Digital Comparator limit\n 
*/
#define ADI_ADC_COMPARATOR_AIN0_LO_EN              (1)    /* 0 or 1 */

/*! Set the Channel0 limit comparator value. \n
    Sets the LO limit value for the channel, only \n
    relevant if ADI_ADC_COMPARATOR_AIN0_LO_EN is set to 1.\n
*/
#define ADI_ADC_COMPARATOR_AIN0_LO_VAL             (0)    /* Range: 0 to 4095 */

/*! Enable/Disable Channel0 hysteresis and monitor cycles \n
    1 - Enable hysteresis and monitor cycles\n
    0 - Disable hysteresis and monitor cycles\n 
*/
#define ADI_ADC_COMPARATOR_AIN0_HYS_EN             (1)   /* 0 or 1 */

/*! Set the Channel0 limit comparator hysteresis value. \n
    Sets the hysteresis value for the channel, only \n
    relevant if ADI_ADC_COMPARATOR_AIN0_HYS_EN is set to 1.\n
*/
#define ADI_ADC_COMPARATOR_AIN0_HYS_VAL            (0)   /* 9 bits, 0 to 511 */

/*! Set the Channel0 limit comparator hysteresis monitor value. \n
    Sets the monitor value for the channel, only \n
    relevant if ADI_ADC_COMPARATOR_AIN0_HYS_EN is set to 1.\n
*/
#define ADI_ADC_COMPARATOR_AIN0_HYS_CYC            (0)   /* 3 bits, 0 to 7 */

/*! Enable/Disable Channel1 limit comparator \n
    1 - Enable HI Digital Comparator limit\n
    0 - Disable HI Digital Comparator limit\n
*/
#define ADI_ADC_COMPARATOR_AIN1_HI_EN              (0)    /* 0 or 1 */

/*! Set the Channel1 limit comparator value \n
    Sets the HI limit value for the channel, only \n
    relevant if ADI_ADC_COMPARATOR_AIN1_HI_EN is set to 1. \n
*/
#define ADI_ADC_COMPARATOR_AIN1_HI_VAL             (4095) /* Range: 0 to 4095 */

/*! Enable/Disable Channel1 limit comparator \n
    1 - Enable LO Digital Comparator limit\n
    0 - Disable LO Digital Comparator limit\n 
*/
#define ADI_ADC_COMPARATOR_AIN1_LO_EN              (0)    /* 0 or 1 */

/*! Set the Channel1 limit comparator value. \n
    Sets the LO limit value for the channel, only \n
    relevant if ADI_ADC_COMPARATOR_AIN1_LO_EN is set to 1.\n
*/
#define ADI_ADC_COMPARATOR_AIN1_LO_VAL             (0)    /* Range: 0 to 4095 */

/*! Enable/Disable Channel1 hysteresis and monitor cycles \n
    1 - Enable hysteresis and monitor cycles\n
    0 - Disable hysteresis and monitor cycles\n 
*/
#define ADI_ADC_COMPARATOR_AIN1_HYS_EN             (0)    /* 0 or 1 */

/*! Set the Channel1 limit comparator hysteresis value. \n
    Sets the hysteresis value for the channel, only \n
    relevant if ADI_ADC_COMPARATOR_AIN1_HYS_EN is set to 1.\n
*/
#define ADI_ADC_COMPARATOR_AIN1_HYS_VAL            (0)    /* 9 bits, 0 to 511 */

/*! Set the Channel1 limit comparator hysteresis monitor value. \n
    Sets the monitor value for the channel, only \n
    relevant if ADI_ADC_COMPARATOR_AIN1_HYS_EN is set to 1.\n
*/
#define ADI_ADC_COMPARATOR_AIN1_HYS_CYC            (0)    /* 3 bits, 0 to 7 */

/*! Enable/Disable Channel2 limit comparator \n
    1 - Enable HI Digital Comparator limit\n
    0 - Disable HI Digital Comparator limit\n
*/
#define ADI_ADC_COMPARATOR_AIN2_HI_EN              (0)    /* 0 or 1 */

/*! Set the Channel2 limit comparator value \n
    Sets the HI limit value for the channel, only \n
    relevant if ADI_ADC_COMPARATOR_AIN2_HI_EN is set to 1. \n
*/
#define ADI_ADC_COMPARATOR_AIN2_HI_VAL             (4095) /* Range: 0 to 4095 */

/*! Enable/Disable Channel2 limit comparator \n
    1 - Enable LO Digital Comparator limit\n
    0 - Disable LO Digital Comparator limit\n 
*/
#define ADI_ADC_COMPARATOR_AIN2_LO_EN              (0)    /* 0 or 1 */

/*! Set the Channel2 limit comparator value. \n
    Sets the LO limit value for the channel, only \n
    relevant if ADI_ADC_COMPARATOR_AIN2_LO_EN is set to 1.\n
*/
#define ADI_ADC_COMPARATOR_AIN2_LO_VAL             (0)    /* Range: 0 to 4095 */

/*! Enable/Disable Channel2 hysteresis and monitor cycles \n
    1 - Enable hysteresis and monitor cycles\n
    0 - Disable hysteresis and monitor cycles\n 
*/
#define ADI_ADC_COMPARATOR_AIN2_HYS_EN             (0)    /* 0 or 1 */

/*! Set the Channel2 limit comparator hysteresis value. \n
    Sets the hysteresis value for the channel, only \n
    relevant if ADI_ADC_COMPARATOR_AIN2_HYS_EN is set to 1.\n
*/
#define ADI_ADC_COMPARATOR_AIN2_HYS_VAL            (0)    /* 9 bits, 0 to 511 */

/*! Set the Channel2 limit comparator hysteresis monitor value. \n
    Sets the monitor value for the channel, only \n
    relevant if ADI_ADC_COMPARATOR_AIN2_HYS_EN is set to 1.\n
*/
#define ADI_ADC_COMPARATOR_AIN2_HYS_CYC            (0)    /* 3 bits, 0 to 7 */

/*! Enable/Disable Channel3 limit comparator \n
    1 - Enable HI Digital Comparator limit\n
    0 - Disable HI Digital Comparator limit\n
*/
#define ADI_ADC_COMPARATOR_AIN3_HI_EN              (0)    /* 0 or 1 */

/*! Set the Channel3 limit comparator value \n
    Sets the HI limit value for the channel, only \n
    relevant if ADI_ADC_COMPARATOR_AIN3_HI_EN is set to 1. \n
*/
#define ADI_ADC_COMPARATOR_AIN3_HI_VAL             (4095) /* Range: 0 to 4095 */

/*! Enable/Disable Channel3 limit comparator \n
    1 - Enable LO Digital Comparator limit\n
    0 - Disable LO Digital Comparator limit\n 
*/
#define ADI_ADC_COMPARATOR_AIN3_LO_EN              (0)   /* 0 or 1 */

/*! Set the Channel3 limit comparator value. \n
    Sets the LO limit value for the channel, only \n
    relevant if ADI_ADC_COMPARATOR_AIN3_LO_EN is set to 1.\n
*/
#define ADI_ADC_COMPARATOR_AIN3_LO_VAL             (0)   /* Range: 0 to 4095 */

/*! Enable/Disable Channel3 hysteresis and monitor cycles \n
    1 - Enable hysteresis and monitor cycles\n
    0 - Disable hysteresis and monitor cycles\n 
*/
#define ADI_ADC_COMPARATOR_AIN3_HYS_EN             (0)   /* 0 or 1 */

/*! Set the Channel3 limit comparator hysteresis value. \n
    Sets the hysteresis value for the channel, only \n
    relevant if ADI_ADC_COMPARATOR_AIN3_HYS_EN is set to 1.\n
*/
#define ADI_ADC_COMPARATOR_AIN3_HYS_VAL            (0)   /* 9 bits, 0 to 511 */

/*! Set the Channel3 limit comparator hysteresis monitor value. \n
    Sets the monitor value for the channel, only \n
    relevant if ADI_ADC_COMPARATOR_AIN3_HYS_EN is set to 1.\n
*/
#define ADI_ADC_COMPARATOR_AIN3_HYS_CYC            (0)   /* 3 bits, 0 to 7 */
   

/************** Macro validation *****************************/

#if (ADI_ADC_CFG_RESOLUTION < 12) || (ADI_ADC_CFG_RESOLUTION > 16)
#error "ADI_ADC_CFG_RESOLUTION is invalid"
#endif

#if (ADI_ADC_CFG_VREF < 0) || (ADI_ADC_CFG_VREF > 3)
#error "ADI_ADC_CFG_VREF is invalid"
#endif

#if (ADI_ADC_COMPARATOR_AIN0_HI_VAL < (0)) || (ADI_ADC_COMPARATOR_AIN0_HI_VAL > (4095))
#error "ADI_ADC_COMPARATOR_AIN0_HI_VAL is invalid"
#endif

#if (ADI_ADC_COMPARATOR_AIN1_HI_VAL < (0)) || (ADI_ADC_COMPARATOR_AIN1_HI_VAL > (4095))
#error "ADI_ADC_COMPARATOR_AIN1_HI_VAL is invalid"
#endif

#if (ADI_ADC_COMPARATOR_AIN2_HI_VAL < (0)) || (ADI_ADC_COMPARATOR_AIN2_HI_VAL > (4095))
#error "ADI_ADC_COMPARATOR_AIN2_HI_VAL is invalid"
#endif

#if (ADI_ADC_COMPARATOR_AIN3_HI_VAL < (0)) || (ADI_ADC_COMPARATOR_AIN3_HI_VAL > (4095))
#error "ADI_ADC_COMPARATOR_AIN3_HI_VAL is invalid"
#endif


#if (ADI_ADC_COMPARATOR_AIN0_LO_VAL < (0)) || (ADI_ADC_COMPARATOR_AIN0_LO_VAL > (4095))
#error "ADI_ADC_COMPARATOR_AIN0_LO_VAL is invalid"
#endif

#if (ADI_ADC_COMPARATOR_AIN1_LO_VAL < (0)) || (ADI_ADC_COMPARATOR_AIN1_LO_VAL > (4095))
#error "ADI_ADC_COMPARATOR_AIN1_LO_VAL is invalid"
#endif

#if (ADI_ADC_COMPARATOR_AIN2_LO_VAL < (0)) || (ADI_ADC_COMPARATOR_AIN2_LO_VAL > (4095))
#error "ADI_ADC_COMPARATOR_AIN2_LO_VAL is invalid"
#endif

#if (ADI_ADC_COMPARATOR_AIN3_LO_VAL < (0)) || (ADI_ADC_COMPARATOR_AIN3_LO_VAL > (4095))
#error "ADI_ADC_COMPARATOR_AIN3_HI_VAL is invalid"
#endif


#if (ADI_ADC_COMPARATOR_AIN0_HYS_VAL < (0)) || (ADI_ADC_COMPARATOR_AIN0_HYS_VAL > (511))
#error "ADI_ADC_COMPARATOR_AIN0_HYS_VAL is invalid"
#endif

#if (ADI_ADC_COMPARATOR_AIN1_HYS_VAL < (0)) || (ADI_ADC_COMPARATOR_AIN1_HYS_VAL > (511))
#error "ADI_ADC_COMPARATOR_AIN1_HYS_VAL is invalid"
#endif

#if (ADI_ADC_COMPARATOR_AIN2_HYS_VAL < (0)) || (ADI_ADC_COMPARATOR_AIN2_HYS_VAL > (511))
#error "ADI_ADC_COMPARATOR_AIN2_HYS_VAL is invalid"
#endif

#if (ADI_ADC_COMPARATOR_AIN3_HYS_VAL < (0)) || (ADI_ADC_COMPARATOR_AIN3_HYS_VAL > (511))
#error "ADI_ADC_COMPARATOR_AIN3_HYS_VAL is invalid"
#endif


#if (ADI_ADC_COMPARATOR_AIN0_HYS_CYC < (0)) || (ADI_ADC_COMPARATOR_AIN0_HYS_CYC > (7))
#error "ADI_ADC_COMPARATOR_AIN0_HYS_CYC is invalid"
#endif

#if (ADI_ADC_COMPARATOR_AIN1_HYS_CYC < (0)) || (ADI_ADC_COMPARATOR_AIN1_HYS_CYC > (7))
#error "ADI_ADC_COMPARATOR_AIN1_HYS_CYC is invalid"
#endif

#if (ADI_ADC_COMPARATOR_AIN2_HYS_CYC < (0)) || (ADI_ADC_COMPARATOR_AIN2_HYS_CYC > (7))
#error "ADI_ADC_COMPARATOR_AIN2_HYS_CYC is invalid"
#endif

#if (ADI_ADC_COMPARATOR_AIN3_HYS_CYC < (0)) || (ADI_ADC_COMPARATOR_AIN3_HYS_CYC > (7))
#error "ADI_ADC_COMPARATOR_AIN3_HYS_CYC is invalid"
#endif




/*! @} */

#endif /* ADI_ADC_CONFIG_H */
