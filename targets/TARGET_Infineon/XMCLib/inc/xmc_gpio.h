/**
 * @file xmc_gpio.h
 * @date 2015-06-20
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-20:
 *     - Initial draft<br>
 *     - Documentation improved <br>
 *      
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API
 *
 * @endcond
 *
 */

#ifndef XMC_GPIO_H
#define XMC_GPIO_H

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/

#include "xmc_common.h"

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup GPIO
 * @brief General Purpose Input Output (GPIO) driver for the XMC microcontroller family.
 *
 * GPIO driver provide a generic and very flexible software interface for all standard digital I/O port pins. 
 * Each port slice has individual interfaces for the operation as General Purpose I/O and it further provides the 
 * connectivity to the on-chip periphery and the control for the pad characteristics. 
 *
 * The driver is divided into Input and Output mode.
 *
 * Input mode features:
 * -# Configuration structure XMC_GPIO_CONFIG_t and initialization function XMC_GPIO_Init()
 * -# Allows the selection of weak pull-up or pull-down device. Configuration structure XMC_GPIO_MODE_t and function XMC_GPIO_SetMode() 
 * \if XMC1
 * -# Allows the selection of input hysteresis. XMC_GPIO_SetInputHysteresis()
 * \endif
 *
 * 
 * Output mode features:
 * -# Allows the selection of push pull/open drain and Alternate output. Configuration structure XMC_GPIO_MODE_t and function XMC_GPIO_SetMode() 
 * \if XMC4
 * -# Allows the selection of pad driver strength. Configuration structure XMC_GPIO_OUTPUT_STRENGTH_t and function XMC_GPIO_SetOutputStrength()
 * \endif
 *
 * -# Allows the selection of initial output level. Configuration structure XMC_GPIO_OUTPUT_LEVEL_t and function XMC_GPIO_SetOutputLevel()
 *
 *@{
 */
 
/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

#define PORT_IOCR_PC_Pos PORT0_IOCR0_PC0_Pos
#define PORT_IOCR_PC_Msk PORT0_IOCR0_PC0_Msk

#define PORT_IOCR_PC_Size 				(8U)
											

#define XMC_GPIO_CHECK_OUTPUT_LEVEL(level) ((level == XMC_GPIO_OUTPUT_LEVEL_LOW) || \
                                            (level == XMC_GPIO_OUTPUT_LEVEL_HIGH))
                                            
#define XMC_GPIO_CHECK_HWCTRL(hwctrl) ((hwctrl == XMC_GPIO_HWCTRL_DISABLED) || \
                                       (hwctrl == XMC_GPIO_HWCTRL_PERIPHERAL1) || \
                                       (hwctrl == XMC_GPIO_HWCTRL_PERIPHERAL2))                                     
                                            
/**********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/


/**
 * Defines output level of a pin. Use type \a XMC_GPIO_OUTPUT_LEVEL_t for this enum.
 */
typedef enum XMC_GPIO_OUTPUT_LEVEL
{
  XMC_GPIO_OUTPUT_LEVEL_LOW  = 0x10000U, /**<  Reset bit */
  XMC_GPIO_OUTPUT_LEVEL_HIGH = 0x1U, 	/**< Set bit  */
} XMC_GPIO_OUTPUT_LEVEL_t;

/**
 * Defines direct hardware control characteristics of the pin . Use type \a XMC_GPIO_HWCTRL_t for this enum.
 */
typedef enum XMC_GPIO_HWCTRL
{
  XMC_GPIO_HWCTRL_DISABLED     = 0x0U, /**<  Software control only */
  XMC_GPIO_HWCTRL_PERIPHERAL1  = 0x1U, /**<  HWI0/HWO0 control path can override the software configuration */
  XMC_GPIO_HWCTRL_PERIPHERAL2  = 0x2U  /**<  HWI1/HWO1 control path can override the software configuration */
} XMC_GPIO_HWCTRL_t;

/**********************************************************************************************************************
 * DEVICE FAMILY EXTENSIONS
 *********************************************************************************************************************/

 #if UC_FAMILY == XMC1
#include "xmc1_gpio.h"
#elif UC_FAMILY == XMC4
#include "xmc4_gpio.h"
#else
#error "xmc_gpio.h: family device not supported"
#endif

/**********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @param  port	  Constant pointer pointing to GPIO port, to access port registers like Pn_OUT,Pn_OMR,Pn_IOCR etc.
 * @param  pin	  Port pin number.
 * @param  config GPIO configuration data structure. Refer data structure @ref XMC_GPIO_CONFIG_t for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * \if XMC1
 * Initializes input / output mode settings like, pull up / pull down devices,hysteresis, push pull /open drain.
 * Also configures alternate function outputs and clears hardware port control for a selected \a port \a and \a pin.
 * \a config provides selected I/O settings. It configures hardware registers Pn_IOCR,Pn_OUT, Pn_OMR,Pn_PDISC and Pn_PHCR.
 * \endif
 * \if XMC4
 * Initializes input / output mode settings like, pull up / pull down devices,push pull /open drain, and pad driver mode.
 * Also configures alternate function outputs and clears hardware port control for selected \a port and \a pin .
 * It configures hardware registers Pn_IOCR,Pn_OUT,Pn_OMR,Pn_PDISC and Pn_PDR.\n
 * \endif
 *
 * \par<b>Related APIs:</b><BR>
 *  None
 *
 * \par<b>Note:</b><br>
 * This API is called in definition of DAVE_init by code generation and therefore should not be explicitly called
 * for the normal operation. Use other APIs only after DAVE_init is called successfully (returns DAVE_STATUS_SUCCESS).
 *
 *
 */

 
void XMC_GPIO_Init(XMC_GPIO_PORT_t *const port, const uint8_t pin, const XMC_GPIO_CONFIG_t *const config);
 
/**
 *
 * @param  port	Constant pointer pointing to GPIO port, to access hardware register Pn_IOCR.
 * @param  pin	Port pin number.
 * @param  mode input / output functionality selection. Refer @ref XMC_GPIO_MODE_t for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets digital input and output driver functionality and characteristics of a GPIO port pin. It configures hardware
 * registers Pn_IOCR. \a mode is initially configured during initialization in XMC_GPIO_Init(). Call this API to alter
 * the port direction functionality as needed later in the program.
 *
 * \par<b>Related APIs:</b><BR>
 *  None
 *
 */

void XMC_GPIO_SetMode(XMC_GPIO_PORT_t *const port, const uint8_t pin, const XMC_GPIO_MODE_t mode);


/**
 *
 * @param  port	 Constant pointer pointing to GPIO port, to access hardware register Pn_OMR.
 * @param  pin	 Port pin number.
 * @param  level output level selection. Refer @ref XMC_GPIO_OUTPUT_LEVEL_t for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set port pin output level to high or low.It configures hardware registers Pn_OMR.\a level is initially
 * configured during initialization in XMC_GPIO_Init(). Call this API to alter output level as needed later in the program.
 *
 * \par<b>Related APIs:</b><BR>
 *  XMC_GPIO_SetOutputHigh(), XMC_GPIO_SetOutputLow().
 *
 * \par<b>Note:</b><br>
 * Prior to this api, user has to configure port pin to output mode using XMC_GPIO_SetMode().
 *
 */


__STATIC_INLINE void XMC_GPIO_SetOutputLevel(XMC_GPIO_PORT_t *const port, const uint8_t pin, const XMC_GPIO_OUTPUT_LEVEL_t level)
{
  XMC_ASSERT("XMC_GPIO_SetOutputLevel: Invalid port", XMC_GPIO_CHECK_OUTPUT_PORT(port));
  XMC_ASSERT("XMC_GPIO_SetOutputLevel: Invalid output level", XMC_GPIO_CHECK_OUTPUT_LEVEL(level));
  
  port->OMR = (uint32_t)level << pin;
}


/**
 * @param  port constant pointer pointing to GPIO port, to access hardware register Pn_OMR.
 * @param  pin	Port pin number.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Sets port pin output to high. It configures hardware registers Pn_OMR.
 *
 *  \par<b>Related APIs:</b><BR>
 *  XMC_GPIO_SetOutputLow()
 *
 * \par<b>Note:</b><br>
 * Prior to this api, user has to configure port pin to output mode using XMC_GPIO_SetMode().\n
 * Register Pn_OMR is virtual and does not contain any flip-flop. A read action delivers the value of 0.
 *
 */

__STATIC_INLINE void XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT_t *const port, const uint8_t pin)
{
  XMC_ASSERT("XMC_GPIO_SetOutputHigh: Invalid port", XMC_GPIO_CHECK_OUTPUT_PORT(port));

  port->OMR = (uint32_t)0x1U << pin;
}

/**
 *
 * @param  port	constant pointer pointing to GPIO port, to access hardware register Pn_OMR.
 * @param  pin	port pin number.
 *
 * @return  None
 *
 *\par<b>Description:</b><br>
 * Sets port pin output to low. It configures hardware registers Pn_OMR.\n
 *
 * \par<b>Related APIs:</b><BR>>
 * XMC_GPIO_SetOutputHigh()
 *
 *\par<b>Note:</b><br>
 * Prior to this api, user has to configure port pin to output mode using XMC_GPIO_SetMode().
 * Register Pn_OMR is virtual and does not contain any flip-flop. A read action delivers the value of 0.\n
 *
 */

__STATIC_INLINE void XMC_GPIO_SetOutputLow(XMC_GPIO_PORT_t *const port, const uint8_t pin)
{
  XMC_ASSERT("XMC_GPIO_SetOutputLow: Invalid port", XMC_GPIO_CHECK_OUTPUT_PORT(port));

  port->OMR = 0x10000U << pin;
}

/**
 *
 * @param port constant pointer pointing to GPIO port, to access hardware register Pn_OMR.
 * @param pin  port pin number.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures port pin output to Toggle. It configures hardware registers Pn_OMR.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_GPIO_SetOutputHigh(), XMC_GPIO_SetOutputLow().
 *
 * \par<b>Note:</b><br>
 * Prior to this api, user has to configure port pin to output mode using XMC_GPIO_SetMode(). Register Pn_OMR is virtual
 * and does not contain any flip-flop. A read action delivers the value of 0.
 *
 */

__STATIC_INLINE void XMC_GPIO_ToggleOutput(XMC_GPIO_PORT_t *const port, const uint8_t pin)
{
  XMC_ASSERT("XMC_GPIO_ToggleOutput: Invalid port", XMC_GPIO_CHECK_OUTPUT_PORT(port));

  port->OMR = 0x10001U << pin;
}

/**
 *
 * @param port constant pointer pointing to GPIO port, to access hardware register Pn_IN.
 * @param pin  Port pin number.
 *
 * @return uint32_t pin logic level status.
 *
 *\par<b>Description:</b><br>
 * Reads the Pn_IN register and returns the current logical value at the GPIO pin.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 *
 * \par<b>Note:</b><br>
 * Prior to this api, user has to configure port pin to input mode using XMC_GPIO_SetMode().
 *
 */

__STATIC_INLINE uint32_t XMC_GPIO_GetInput(XMC_GPIO_PORT_t *const port, const uint8_t pin)
{
  XMC_ASSERT("XMC_GPIO_GetInput: Invalid port", XMC_GPIO_CHECK_PORT(port));

  return (((port->IN) >> pin) & 0x1U);
}

/**
 * @param port constant pointer pointing to GPIO port, to access hardware register Pn_PPS.
 * @param pin  port pin number.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables pin power save mode and configures Pn_PPS register.This configuration is useful when the controller enters
 * Deep Sleep mode.Port pin enabled with power save mode option are set to a defined state and the input Schmitt-Trigger
 * as well as the output driver stage are switched off. By default port pin does not react to power save mode request.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_GPIO_DisablePowerSaveMode()
 *
 * <b>Note:</b><br>
 * Do not enable the Pin Power Save function for pins configured for Hardware Control (Pn_HWSEL.HWx != 00B). Doing so
 * may result in an undefined behavior of the pin when the device enters the Deep Sleep state.
 *
 */


__STATIC_INLINE void XMC_GPIO_EnablePowerSaveMode(XMC_GPIO_PORT_t *const port, const uint8_t pin)
{
  XMC_ASSERT("XMC_GPIO_EnablePowerSaveMode: Invalid port", XMC_GPIO_CHECK_PORT(port));

  port->PPS |= (uint32_t)0x1U << pin;
}


/**
 *
 * @param port	constant pointer pointing to GPIO port, to access hardware register Pn_PPS.
 * @param pin	port pin number.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables pin power save mode and configures Pn_PPS register.This configuration is useful when the controller enters
 * Deep Sleep mode. This configuration enables input Schmitt-Trigger and output driver stage(if pin is enabled power
 * save mode previously). By default port \a pin does not react to power save mode request.
 *
 * \par<b>Related APIs:</b><BR>
 *  XMC_GPIO_EnablePowerSaveMode()
 *
 *\par<b>Note:</b><br>
 * Do not enable the Pin Power Save function for pins configured for Hardware Control (Pn_HWSEL.HWx != 00B). Doing so
 * may result in an undefined behavior of the pin when the device enters the Deep Sleep state.
 *
 */

__STATIC_INLINE void XMC_GPIO_DisablePowerSaveMode(XMC_GPIO_PORT_t *const port, const uint8_t pin)
{
  XMC_ASSERT("XMC_GPIO_DisablePowerSaveMode: Invalid port", XMC_GPIO_CHECK_PORT(port));

  port->PPS &= ~(uint32_t)((uint32_t)0x1U << pin);
}


/**
 * @param port	 constant pointer pointing to GPIO port, to access hardware register Pn_HWSEL.
 * @param pin	 port pin number.
 * @param hwctrl direct hardware control selection. Refer @ref XMC_GPIO_HWCTRL_t for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Selects direct hard ware control and configures Pn_HWSEL register.This configuration is useful for the port pins
 * overlaid with peripheral functions for which the connected peripheral needs hardware control.
 *
 * \par<b>Related APIs:</b><BR>
 *  None
 *
 *\par<b>Note:</b><br>
 * Do not enable the Pin Power Save function for pins configured for Hardware Control (Pn_HWSEL.HWx != 00B).
 * Doing so may result in an undefined behavior of the pin when the device enters the Deep Sleep state.
 *
 */

void XMC_GPIO_SetHardwareControl(XMC_GPIO_PORT_t *const port, const uint8_t pin, const XMC_GPIO_HWCTRL_t hwctrl);


/**
 * @param port	constant pointer pointing to GPIO port, to access hardware register Pn_PDISC.
 * @param pin	port pin number.
 *
 * @return None
 *
 * \par<b>Related APIs:</b><BR>
 *  None
 *
 * \par<b>Description:</b><br>
 * Enable digital input path for analog pins and configures Pn_PDISC register.This configuration is applicable only for
 * analog port pins.
 *
 */
__STATIC_INLINE void XMC_GPIO_EnableDigitalInput(XMC_GPIO_PORT_t *const port, const uint8_t pin)
{
  XMC_ASSERT("XMC_GPIO_EnableDigitalInput: Invalid analog port", XMC_GPIO_CHECK_ANALOG_PORT(port));
  
  port->PDISC &= ~(uint32_t)((uint32_t)0x1U << pin);
}


/**
 * @param  port	constant pointer pointing to GPIO port, to access hardware register Pn_PDISC.
 * @param  pin	port pin number.
 *
 * @return None
 *
 * \par<b>Related APIs:</b><BR>
 * None
 *
 * \par<b>Description:</b><br>
 * Disable digital input path for analog pins and configures Pn_PDISC register.This configuration is applicable only
 * for analog port pins.
 *
 */

__STATIC_INLINE void XMC_GPIO_DisableDigitalInput(XMC_GPIO_PORT_t *const port, const uint8_t pin)
{
  XMC_ASSERT("XMC_GPIO_EnableDigitalInput: Invalid analog port", XMC_GPIO_CHECK_ANALOG_PORT(port));
  
  port->PDISC |= (uint32_t)0x1U << pin;
}

#ifdef __cplusplus
}
#endif

/**
 * @} (end addtogroup GPIO)
 */

/**
 * @} (end addtogroup XMClib)
 */

#endif /* XMC_GPIO_H */
 
