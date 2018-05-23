/**
 * @file xmc4_gpio.h
 * @date 2015-10-09
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
 *      
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API
 *
 * 2015-10-09:
 *     - Added PORT MACRO checks and definitions for XMC4800/4700 devices
 * @endcond
 *
 */

#ifndef XMC4_GPIO_H
#define XMC4_GPIO_H

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/

#include "xmc_common.h"

#if UC_FAMILY == XMC4

#include "xmc4_gpio_map.h"

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup GPIO
 * @{
 */

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

#if defined(PORT0)
#define XMC_GPIO_PORT0 ((XMC_GPIO_PORT_t *) PORT0_BASE)
#define XMC_GPIO_CHECK_PORT0(port) (port == XMC_GPIO_PORT0)
#else
#define XMC_GPIO_CHECK_PORT0(port) 0
#endif

#if defined(PORT1)
#define XMC_GPIO_PORT1 ((XMC_GPIO_PORT_t *) PORT1_BASE)
#define XMC_GPIO_CHECK_PORT1(port) (port == XMC_GPIO_PORT1)
#else
#define XMC_GPIO_CHECK_PORT1(port) 0
#endif

#if defined(PORT2)
#define XMC_GPIO_PORT2 ((XMC_GPIO_PORT_t *) PORT2_BASE)
#define XMC_GPIO_CHECK_PORT2(port) (port == XMC_GPIO_PORT2)
#else
#define XMC_GPIO_CHECK_PORT2(port) 0
#endif

#if defined(PORT3)
#define XMC_GPIO_PORT3 ((XMC_GPIO_PORT_t *) PORT3_BASE)
#define XMC_GPIO_CHECK_PORT3(port) (port == XMC_GPIO_PORT3)
#else
#define XMC_GPIO_CHECK_PORT3(port) 0
#endif

#if defined(PORT4)
#define XMC_GPIO_PORT4 ((XMC_GPIO_PORT_t *) PORT4_BASE)
#define XMC_GPIO_CHECK_PORT4(port) (port == XMC_GPIO_PORT4)
#else
#define XMC_GPIO_CHECK_PORT4(port) 0
#endif

#if defined(PORT5)
#define XMC_GPIO_PORT5 ((XMC_GPIO_PORT_t *) PORT5_BASE)
#define XMC_GPIO_CHECK_PORT5(port) (port == XMC_GPIO_PORT5)
#else
#define XMC_GPIO_CHECK_PORT5(port) 0
#endif

#if defined(PORT6)
#define XMC_GPIO_PORT6 ((XMC_GPIO_PORT_t *) PORT6_BASE)
#define XMC_GPIO_CHECK_PORT6(port) (port == XMC_GPIO_PORT6)
#else
#define XMC_GPIO_CHECK_PORT6(port) 0
#endif

#if defined(PORT7)
#define XMC_GPIO_PORT7 ((XMC_GPIO_PORT_t *) PORT7_BASE)
#define XMC_GPIO_CHECK_PORT7(port) (port == XMC_GPIO_PORT7)
#else
#define XMC_GPIO_CHECK_PORT7(port) 0
#endif

#if defined(PORT8)
#define XMC_GPIO_PORT8 ((XMC_GPIO_PORT_t *) PORT8_BASE)
#define XMC_GPIO_CHECK_PORT8(port) (port == XMC_GPIO_PORT8)
#else
#define XMC_GPIO_CHECK_PORT8(port) 0
#endif

#if defined(PORT9)
#define XMC_GPIO_PORT9 ((XMC_GPIO_PORT_t *) PORT9_BASE)
#define XMC_GPIO_CHECK_PORT9(port) (port == XMC_GPIO_PORT9)
#else
#define XMC_GPIO_CHECK_PORT9(port) 0
#endif

#if defined(PORT14)
#define XMC_GPIO_PORT14 ((XMC_GPIO_PORT_t *) PORT14_BASE)
#define XMC_GPIO_CHECK_PORT14(port) (port == XMC_GPIO_PORT14)
#else
#define XMC_GPIO_CHECK_PORT14(port) 0
#endif

#if defined(PORT15)
#define XMC_GPIO_PORT15 ((XMC_GPIO_PORT_t *) PORT15_BASE)
#define XMC_GPIO_CHECK_PORT15(port) (port == XMC_GPIO_PORT15)
#else
#define XMC_GPIO_CHECK_PORT15(port) 0
#endif

#define XMC_GPIO_CHECK_PORT(port) (XMC_GPIO_CHECK_PORT0(port) || \
                                   XMC_GPIO_CHECK_PORT1(port) || \
                                   XMC_GPIO_CHECK_PORT2(port) || \
                                   XMC_GPIO_CHECK_PORT3(port) || \
                                   XMC_GPIO_CHECK_PORT4(port) || \
                                   XMC_GPIO_CHECK_PORT5(port) || \
                                   XMC_GPIO_CHECK_PORT6(port) || \
                                   XMC_GPIO_CHECK_PORT7(port) || \
                                   XMC_GPIO_CHECK_PORT8(port) || \
                                   XMC_GPIO_CHECK_PORT9(port) || \
                                   XMC_GPIO_CHECK_PORT14(port) || \
                                   XMC_GPIO_CHECK_PORT15(port))

#define XMC_GPIO_CHECK_OUTPUT_PORT(port) (XMC_GPIO_CHECK_PORT0(port) || \
                                          XMC_GPIO_CHECK_PORT1(port) || \
                                          XMC_GPIO_CHECK_PORT2(port) || \
                                          XMC_GPIO_CHECK_PORT3(port) || \
                                          XMC_GPIO_CHECK_PORT4(port) || \
                                          XMC_GPIO_CHECK_PORT5(port) || \
                                          XMC_GPIO_CHECK_PORT6(port) || \
                                          XMC_GPIO_CHECK_PORT7(port) || \
                                          XMC_GPIO_CHECK_PORT8(port) || \
                                          XMC_GPIO_CHECK_PORT9(port))

#define XMC_GPIO_CHECK_ANALOG_PORT(port) (XMC_GPIO_CHECK_PORT14(port) || \
                                          XMC_GPIO_CHECK_PORT15(port))

#define XMC_GPIO_CHECK_OUTPUT_STRENGTH(strength) ((strength == XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE) ||\
                                                 (strength == XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE) ||\
                                                 (strength == XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE) ||\
                                                 (strength == XMC_GPIO_OUTPUT_STRENGTH_STRONG_SLOW_EDGE) ||\
                                                 (strength == XMC_GPIO_OUTPUT_STRENGTH_MEDIUM) ||\
                                                 (strength == XMC_GPIO_OUTPUT_STRENGTH_WEAK))

/**********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/

/**
 * Defines the direction and characteristics of a pin. Use type \a XMC_GPIO_MODE_t for this enum. For the operation
 * with alternate functions, the port pins are directly connected to input or output functions of the on-chip periphery.
 */

typedef enum XMC_GPIO_MODE
{
  XMC_GPIO_MODE_INPUT_TRISTATE = 0x0UL << PORT0_IOCR0_PC0_Pos, 	         /**< No internal pull device active */
  XMC_GPIO_MODE_INPUT_PULL_DOWN = 0x1UL << PORT0_IOCR0_PC0_Pos, 	     /**< Internal pull-down device active */
  XMC_GPIO_MODE_INPUT_PULL_UP = 0x2UL << PORT0_IOCR0_PC0_Pos, 	         /**< Internal pull-up device active */
  XMC_GPIO_MODE_INPUT_SAMPLING = 0x3UL << PORT0_IOCR0_PC0_Pos, 	         /**< No internal pull device active;Pn_OUTx continuously samples the input value */
  XMC_GPIO_MODE_INPUT_INVERTED_TRISTATE = 0x4UL << PORT0_IOCR0_PC0_Pos,  /**< Inverted no internal pull device active */
  XMC_GPIO_MODE_INPUT_INVERTED_PULL_DOWN = 0x5UL << PORT0_IOCR0_PC0_Pos, /**< Inverted internal pull-down device active */
  XMC_GPIO_MODE_INPUT_INVERTED_PULL_UP = 0x6UL << PORT0_IOCR0_PC0_Pos,   /**< Inverted internal pull-up device active */
  XMC_GPIO_MODE_INPUT_INVERTED_SAMPLING = 0x7UL << PORT0_IOCR0_PC0_Pos,  /**< Inverted no internal pull device active; Pn_OUTx continuously samples the input value */
  XMC_GPIO_MODE_OUTPUT_PUSH_PULL = 0x80UL, 			                     /**< Push-pull general-purpose output */
  XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN = 0xc0UL, 		                     /**< Open-drain general-purpose output */
  XMC_GPIO_MODE_OUTPUT_ALT1 = 0x1UL << PORT0_IOCR0_PC0_Pos,
  XMC_GPIO_MODE_OUTPUT_ALT2 = 0x2UL << PORT0_IOCR0_PC0_Pos,
  XMC_GPIO_MODE_OUTPUT_ALT3 = 0x3UL << PORT0_IOCR0_PC0_Pos,
  XMC_GPIO_MODE_OUTPUT_ALT4 = 0x4UL << PORT0_IOCR0_PC0_Pos,
  XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1 = XMC_GPIO_MODE_OUTPUT_PUSH_PULL | XMC_GPIO_MODE_OUTPUT_ALT1, 	/**<  Push-pull alternate output function 1 */
  XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2 = XMC_GPIO_MODE_OUTPUT_PUSH_PULL | XMC_GPIO_MODE_OUTPUT_ALT2, 	/**<  Push-pull alternate output function 2 */
  XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT3 = XMC_GPIO_MODE_OUTPUT_PUSH_PULL | XMC_GPIO_MODE_OUTPUT_ALT3, 	/**<  Push-pull alternate output function 3 */
  XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT4 = XMC_GPIO_MODE_OUTPUT_PUSH_PULL | XMC_GPIO_MODE_OUTPUT_ALT4, 	/**<  Push-pull alternate output function 4 */
  XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT1 = XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN | XMC_GPIO_MODE_OUTPUT_ALT1, 	/**<  Open drain alternate output function 1 */
  XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT2 = XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN | XMC_GPIO_MODE_OUTPUT_ALT2, 	/**<  Open drain alternate output function 2 */
  XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT3 = XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN | XMC_GPIO_MODE_OUTPUT_ALT3, 	/**<  Open drain alternate output function 3 */
  XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT4 = XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN | XMC_GPIO_MODE_OUTPUT_ALT4, 	/**<  Open drain alternate output function 4 */
} XMC_GPIO_MODE_t;

/**
 * Defines output strength and slew rate of a pin. Use type \a XMC_GPIO_OUTPUT_STRENGTH_t for this enum.
 *
 */
typedef enum XMC_GPIO_OUTPUT_STRENGTH
{
  XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE  = 0x0U,  /**<  Defines pad driver mode, for high speed 3.3V LVTTL outputs */
  XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE = 0x1U,  /**<  Defines pad driver mode, for high speed 3.3V LVTTL outputs  */
  XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE   = 0x2U,  /**<  Defines pad driver mode, medium speed 3.3V LVTTL outputs  */
  XMC_GPIO_OUTPUT_STRENGTH_STRONG_SLOW_EDGE   = 0x3U,  /**<  Defines pad driver mode, medium speed 3.3V LVTTL outputs  */
  XMC_GPIO_OUTPUT_STRENGTH_MEDIUM             = 0x4U,  /**<  Defines pad driver mode, for low speed 3.3V LVTTL outputs  */
  XMC_GPIO_OUTPUT_STRENGTH_WEAK               = 0x7U   /**<  Defines pad driver mode, low speed 3.3V LVTTL outputs  */
} XMC_GPIO_OUTPUT_STRENGTH_t;


/**********************************************************************************************************************
 * DATA STRUCTURES
 *********************************************************************************************************************/
/**
 *  Structure points  port hardware registers. Use type XMC_GPIO_PORT_t for this structure.
 */

typedef struct XMC_GPIO_PORT {
  __IO uint32_t  OUT;			/**< The port output register determines the value of a GPIO pin when it is selected by
  	  	  	  	  	  	  	  	  	  	  Pn_IOCRx as output */
  __O  uint32_t  OMR;			/**< The port output modification register contains control bits that make it possible
  	  	  	  	  	  	  	  	  	 to individually set, reset, or toggle the logic state of a single port line*/
  __I  uint32_t  RESERVED0[2];
  __IO uint32_t  IOCR[4];		/**< The port input/output control registers select the digital output and input driver
											functionality and characteristics of a GPIO port pin */
  __I  uint32_t  RESERVED1;
  __I  uint32_t  IN;			/**< The logic level of a GPIO pin can be read via the read-only port input register
  	  	  	  	  	  	  	  	  	  Pn_IN */
  __I  uint32_t  RESERVED2[6];
  __IO uint32_t  PDR[2];		/**< Pad Driver Mode Registers */

  __I  uint32_t  RESERVED3[6];
  __IO uint32_t  PDISC;			/**< Pin Function Decision Control Register is to disable/enable the digital pad
  	  	  	  	  	  	  	  	  	 structure in shared analog and digital ports*/
  __I  uint32_t  RESERVED4[3];
  __IO uint32_t  PPS;			/**< Pin Power Save Register */
  __IO uint32_t  HWSEL;			/**< Pin Hardware Select Register */
} XMC_GPIO_PORT_t;

/**
 *  Structure initializes port pin. Use type XMC_GPIO_CONFIG_t for this structure.
 */
typedef struct XMC_GPIO_CONFIG
{
  XMC_GPIO_MODE_t mode;							/**< Defines the direction and characteristics of a pin */
  XMC_GPIO_OUTPUT_LEVEL_t output_level;			/**< Defines output level of a pin */
  XMC_GPIO_OUTPUT_STRENGTH_t output_strength;	/**< Defines pad driver mode of a pin */
} XMC_GPIO_CONFIG_t;

/**********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/

__STATIC_INLINE bool XMC_GPIO_IsModeValid(XMC_GPIO_MODE_t mode)
{
  return ((mode == XMC_GPIO_MODE_INPUT_TRISTATE) ||
          (mode == XMC_GPIO_MODE_INPUT_PULL_DOWN) ||
          (mode == XMC_GPIO_MODE_INPUT_PULL_UP) ||
          (mode == XMC_GPIO_MODE_INPUT_SAMPLING) ||
          (mode == XMC_GPIO_MODE_INPUT_INVERTED_TRISTATE) ||
          (mode == XMC_GPIO_MODE_INPUT_INVERTED_PULL_DOWN) ||
          (mode == XMC_GPIO_MODE_INPUT_INVERTED_PULL_UP) ||
          (mode == XMC_GPIO_MODE_INPUT_INVERTED_SAMPLING) ||
          (mode == XMC_GPIO_MODE_OUTPUT_PUSH_PULL) ||
          (mode == XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1) ||
          (mode == XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2) ||
          (mode == XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT3) ||
          (mode == XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT4) ||
          (mode == XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN) ||
          (mode == XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT1) ||
          (mode == XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT2) ||
          (mode == XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT3) ||
          (mode == XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT4));
}

/**
 *
 * @param  port		constant pointer pointing to GPIO port, to access hardware register Pn_PDR.
 * @param  pin		Port pin number.
 * @param  strength Output driver mode selection. Refer data structure @ref XMC_GPIO_OUTPUT_STRENGTH_t for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets port pin output strength and slew rate. It configures hardware registers Pn_PDR. \a strength is initially
 * configured during initialization in XMC_GPIO_Init(). Call this API to alter output driver mode as needed later in
 * the program.
 *
 * \par<b>Related APIs:</b><BR>
 *  None
 *
 * \par<b>Note:</b><br>
 * Prior to this api, user has to configure port pin to output mode using XMC_GPIO_SetMode().
 *
 */

void XMC_GPIO_SetOutputStrength(XMC_GPIO_PORT_t *const port, const uint8_t pin, XMC_GPIO_OUTPUT_STRENGTH_t strength);

/**
 * @} (end addtogroup GPIO)
 */

/**
 * @} (end addtogroup XMClib)
 */

#endif /* UC_FAMILY == XMC4 */

#endif /* XMC4_GPIO_H */
 
