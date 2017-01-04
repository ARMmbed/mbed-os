/*!
 *****************************************************************************
   @file:    adi_pwr_config.h
   @brief:   Configuration options for PWR service.
             This is specific to the PWR service  and will be included by the srouce file.
             It is not required for the application to include this header file.
   @version: $Revision: 
   @date:    $Date: 2016-02-08 14:17:03 +0000 (Mon, 08 Feb 2016) $
  -----------------------------------------------------------------------------

Copyright (c) 2015 Analog Devices, Inc.

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

#ifndef __ADI_PWR_CONFIG_H__
#define __ADI_PWR_CONFIG_H__
#include <adi_global_config.h>

#ifdef __ICCARM__
/* IAR MISRA C 2004 error suppressions.
*
* Pm009 (rule 5.1): identifiers shall not rely on significance of more than 31 characters.
*   The YODA-generated headers rely on more. The IAR compiler supports that.
*/
#pragma diag_suppress=Pm009
#endif /* __ICCARM__ */

/** @defgroup PWR_Driver_Cfg PWR Device Driver Configuration
 *  @ingroup Configuration_macros
 */

/*! \addtogroup  PWR_Driver_Cfg PWR Device Driver Configuration
 *  @{
 */

/*!
   Set this macro to 1 for to enable static controller initializations
   during the driver initialization routing.
   To eliminate static driver configuration, set this macro to 0.*/

#define ADI_PWR_CFG_ENABLE_STATIC_CONFIG_SUPPORT            0

#define ADI_PWR_CFG_ENABLE_CLOCK_SOURCE_GPIO                0
/*-------------------------------------------------------------------------------
           Set of MACRO for configuring the clock 
--------------------------------------------------------------------------------*/
/*!
    32 KHz clock select mux.  This clock connects to beeper, RTC.\n
   0 -  Internal 32 KHz oscillator is selected.\n
   1 -  External 32 KHz crystal is selected..
*/
#define ADI_PWR_LF_CLOCK_MUX                                0


/*!
    High frequency internal oscillator enable\n
   0 -  The HFOSC oscillator is disabled and placed in a low power state\n
   1 -  The HFOSC oscillator is enabled.
*/
#define ADI_PWR_HFOSC_CLOCK_ENABLE                          0

/*!
    Low  frequency external oscillator enable and placed in a low power state\n
   0 -  The LFXTL oscillator is disabled\n
   1 -  The LFXTL oscillator is enabled.
*/
#define ADI_PWR_LFXTL_CLOCK_ENABLE                          0

/*!
    High frequency external  oscillator enable\n
   0 -  The HFXTAL  oscillator is disabled and placed in a low power state\n
   1 -  The HFXTAL  oscillator is enabled.
*/
#define ADI_PWR_HFXTL_CLOCK_ENABLE                         0

/*!
    Selecting the input clock for  Root Clock mux.  Determines which single shared clock source
    is used by the PCLK,  and HCLK dividers. \n
    0 -  HFOSC High frequency internal oscillator \n
    1 -  HFXTL High frequency external oscillator\n
    2 -  SPLL  Output of System PLL is selected\n
    3 -  External GPIO port is selected
*/
#define ADI_PWR_INPUT_TO_ROOT_CLOCK_MUX                     0

/*!
    Flash reference clock and HPBUCK clock source mux.  \n
    0 -  sourcing from HFOSC (High frequency internal oscillator) \n
    2 -  sourcing from external HFXTAL(  High frequency external oscillator 26M Hz )\n
    3 -  sourcing from external HFXTAL(  High frequency external oscillator 16M Hz )

*/
#define ADI_PWR_INPUT_TO_OUTPUT_RCLK_MUX                      0

/*!
    Selecting the input clock for the system PLL clock.  \n
    0 -  sourcing from  HFOSC (High frequency internal oscillator) \n
    1 -  sourcing from  HFXTAL(High frequency external oscillator)
*/
#define ADI_PWR_INPUT_TO_SPLL_MUX                             0

/*!
    External Low frequency crystal interrupt enable.\n
    0 -  Disable the interrupt for LF clock \n
    1 -  Enable the interrupt for LF clock 
*/
#define ADI_PWR_LFXTL_CLOCK_INTERRUPT_ENABLE                  0

/*!
    External Hight frequency crystal interrupt enable.\n
    0 -  Disable the interrupt for HFXTL clock \n
    1 -  Enable the interrupt for  HFXTL clock 
*/
#define ADI_PWR_HFXTL_CLOCK_INTERRUPT_ENABLE                  0

/*!
    HCLK divide count.Determines the HCLK rate based on the following equation: HCLK = ROOT_CLK/HCLKDIVCNT. 
    0 - 63 is valid range. 
*/
#define ADI_PWR_HCLK_DIVIDE_COUNT                             0

/*!
    PCLK divide count.Determines the PCLK rate based on the following equation: PCLK = ROOT_CLK/PCLKDIVCNT.
    0 - 63 is valid range. 
*/
#define ADI_PWR_PCLK_DIVIDE_COUNT                             0

/*!
    ACLK divide count.Determines the ACLK rate based on the following equation: ACLK = ROOT_CLK/ACLKDIVCNT.
    0 - 63 is valid range. 
*/
#define ADI_PWR_ACLK_DIVIDE_COUNT                             0


/*!
    System PLL N multiplier(SPLL_NSEL).  Sets the N value used to obtain the multiplication 
    factor N/M of the PLL.
    8 - 31 is valid range. 
*/
#define ADI_PWR_SPLL_MUL_FACTOR                               26

/*!
    System PLL M Divider(SPLL_MSEL).  Sets the M value used to obtain the multiplication 
    factor N/M of the PLL.
    2 - 15 is valid range. 
*/
#define ADI_PWR_SPLL_DIV_FACTOR                               13

/*!
    System PLL division by 2.  Controls if an optional divide by two is placed on the PLL output.\n
    0 -  The System PLL is not divided. Its output frequency equals that selected by the N/M ratio \n
    1 -  The System PLL is divided by two. Its output frequency equals that selected by the N/M ratio
         with an additional divide by 2
*/
#define ADI_PWR_SPLL_ENABLE_DIV2                              0

/*!
    system PLL multiply by 2.  This bit is used to configure if the VCO clock frequency should be multiplied by 2 or 1.\n
    0 -  The System PLL is multiplied by 1.\n
    1 -  The System PLL is multiplied by 2.
*/
#define ADI_PWR_SPLL_ENABLE_MUL2                              0

/*!
    System PLL enable.  Controls if the PLL should be enabled or placed in its low power state. \n
    0 - The system PLL is disabled and is in its power down state\n
    1 - The system  PLL is enabled. 
*/
#define ADI_PWR_SPLL_ENABLE                                   0

/*!
    System PLL interrupt enable.Controls if the core should be interrupted on a PLL lock/PLL unlock or no interrupt generated.\n
    0 - Disable the SPLL interrupt generation\n
    1 - Enable the SPLL interrupt generation 
*/
#define ADI_PWR_SPLL_INTERRUPT_ENABLE                         0

/*!
    This can be used to enable/disable all clocks connected to peripherals.  \n
    0 - Disable  the Clock supply to  peripherals\n
    1 - Enable the Clock supply to  peripherals 
*/
#define ADI_PWR_PCLK_ENABLE                                   1

/*!
    This can be used to enable/disable clock to GPIO.  \n
    0 - Disable the clock to GPIO\n
    1 - Enable the clock to  GPIO
*/
#define ADI_PWR_GPIO_CLOCK_ENABLE                             1

/*!
    This can be  used to enable/disable clock to I2C.  \n
    0 - Disable  the clock to I2C\n
    1 - Enable the clock to   I2C 
*/
#define ADI_PWR_I2C_CLOCK_ENABLE                              1

/*!
    This can be  used to enable/disable clock to GPT0.  \n
    0 - Disable  the clock to GPT0\n
    1 - Enable the clock to   GPT0 
*/
#define ADI_PWR_GPT0_CLOCK_ENABLE                              1

/*!
    This can be  used to enable/disable clock to GPT1.  \n
    0 - Disable  the clock to GPT1\n
    1 - Enable the clock to   GPT1 
*/
#define ADI_PWR_GPT1_CLOCK_ENABLE                              1
/*!
    This can be  used to enable/disable clock to GPT2.  \n
    0 - Disable  the clock to GPT2\n
    1 - Enable the clock to   GPT2 
*/
#define ADI_PWR_GPT2_CLOCK_ENABLE                              1

/*-------------------------------------------------------------------------------
           Set of MACRO for configuring the power management module
--------------------------------------------------------------------------------*/
/*!
    Enabling the interrupt if the  Battery  voltage falls below 1.8V.\n
   0 -  Disable Battery voltage interrupt \n
   1 -  Enable Battery voltage interrupt.
*/
#define ADI_PWR_ENABLE_VBAT_INTERRUPT                       0

/*!
    Enabling the interrupt for under VREG voltage (i.e less than 1V).\n
   0 -  Disable VREG under voltage  interrupt \n
   1 -  Enable VREG  unter voltage  interrupt.
*/
#define ADI_PWR_ENABLE_VREG_UNDER_VOLTAGE_INTERRUPT         0

/*!
    Enabling the interrupt for over  VREG voltage (i.e above  than 1.32V).\n
   0 -  Disable VREG over  voltage  interrupt \n
   1 -  Enable VREG  over voltage  interrupt.
*/
#define ADI_PWR_ENABLE_VREG_OVER_VOLTAGE_INTERRUPT          0

/*!
    Enabling the interrupt for Battery range.\n
   0 -  Disable  battery voltage range  interrupt \n
   1 -  Enable   battery voltage range  interrupt 
*/
#define ADI_PWR_ENABLE_BATTERY_VOLTAGE_RANGE_INTERRUPT      0

/*!
    Battery voltage range for generating the interrupt.\n
   1 - Battery voltage range is between 2.2 to 2.75\n
   2 - Battery voltage range is between 1.6  to 2.2
*/
#define ADI_PWR_BATTERY_VOLTAGE_RANGE_FOR_INTERRUPT      0

/*!
    Enabling battery voltage monitoring during the hibernate.\n
   0 -  Disable  battery voltage monitoring during the hibernate \n
   1 -  Enable   battery voltage monitoring during the hibernate 
*/
#define ADI_PWR_ENABLE_BATTERY_VOLTAGE_MONITORING           0

/*!
    This can be  used to enable/disable clock to GPT0.  \n
    0 - Disable  the clock to GPT0\n
    1 - Enable the clock to   GPT0 
*/
#define ADI_PWR_GPT0_CLOCK_ENABLE                           1

/*! @} */

#ifdef __ICCARM__
#pragma diag_default=Pm009
#endif /* __ICCARM__ */

#endif /* __ADI_PWR_CONFIG_H__ */
