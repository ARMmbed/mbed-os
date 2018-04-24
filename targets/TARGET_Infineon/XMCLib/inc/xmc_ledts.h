/**
 * @file xmc_ledts.h
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
 *     - New API added: XMC_LEDTS_SetActivePADNo() <br>
 *      
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API
 *
 * @endcond
 *
 */

#ifndef XMC_LEDTS_H
#define XMC_LEDTS_H

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include <xmc_common.h>

#if defined(LEDTS0)
#include "xmc_scu.h"

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup LEDTS
 * @brief LED and Touch-Sense control(LEDTS) driver for the XMC controller family.
 * 
 * The LED and Touch-Sense (LEDTS) drives LEDs and controls touch pads used as human-machine interface (HMI) in an
 * application. The LEDTS can measure the capacitance of up to 8 touch pads, can also drive up to 64 LEDs in an LED matrix.
 * Touch pads and LEDs can share pins to minimize the number of pins needed for such applications, this is realized by
 * the module controlling the touch pads and driving the LEDs in a time-division multiplexed manner.
 *
 * This device contains LEDTS kernel that has an LED driving function and a touch-sensing function.
 *
 * It is recommended to set up all configurations for the LEDTS in all Special Function Registers(SFR) before 
 * enabling and starting LED and/or touch-sense function(s).
 *
 * This Low Level Driver(LLD) provides APIs to configure and control LED functionality, Touch-Sense functionality and
 * features common to both functionalities.
 *
 * LED features:
 * -# Configuration structure to configure LED functionality (XMC_LEDTS_LED_CONFIG_t) and initialization funtion
 *    (XMC_LEDTS_InitLED()).
 * -# Selection of number of LED columns, active column level and enabling LED funtionality (XMC_LEDTS_InitLED()).
 * -# Setting line pattern to be displayed on LED column (XMC_LEDTS_SetLEDLinePattern()).
 * -# Brightness control of LED column (XMC_LEDTS_SetColumnBrightness()).
 * -# Setting number of columns to be activated (XMC_LEDTS_SetNumOfLEDColumns()).
 *
 * Touch-Sense features:
 * -# Configuration structure to perform basic Touch-Sense functionality (XMC_LEDTS_TS_CONFIG_BASIC_t) settings and
 *    initialization funtion (XMC_LEDTS_InitTSBasic()).
 * -# Configuration structure to perform advanced Touch-Sense functionality (XMC_LEDTS_TS_CONFIG_ADVANCED_t) settings
 *    and initialization function (XMC_LEDTS_InitTSAdvanced()).
 * -# Setting number of touch inputs and acculumate count on touch input (XMC_LEDTS_InitTSBasic()).
 * -# Enabling/disabling of common compare, Touch-Sense counter auto reset and Touch-Sense funtionality.
 *    (XMC_LEDTS_InitTSBasic()).
 * -# Set number of mask bits for time frame validation and first touch input to be active. (XMC_LEDTS_InitTSAdvanced()).
 * -# Enable/disable time frame interrupt, external pull-up on touch pin and hardware or software control of
 *    pad turn (XMC_LEDTS_InitTSAdvanced()).
 * -# Setting size of common oscillation window for all touch-sense inputs (XMC_LEDTS_SetCommonOscillationWindow()).
 * -# Setting size of oscillation window for a touch-sense input (XMC_LEDTS_SetOscillationWindow()).
 *
 * Common features:
 * -# Global configuration structure XMC_LEDTS_GLOBAL_CONFIG_t and initialization function XMC_LEDTS_InitGlobal().
 * -# Selection of Clock source for LEDTS module (XMC_LEDTS_InitGlobal()).
 * -# Kick-start and stop of LEDTS module (XMC_LEDTS_StartCounter() / XMC_LEDTS_StopCounter()).
 * -# Read and clear of interrupt status flags (XMC_LEDTS_ReadInterruptFlag() / XMC_LEDTS_ClearInterruptFlag()).
 * -# Reading of previous active column number (XMC_LEDTS_ReadFNCOL()).
 * -# Enable/Disable Interrupts(XMC_LEDTS_EnableInterrupt() / XMC_LEDTS_DisableInterrupt()).
 * @{
 */

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

#if defined(LEDTS0)
#define XMC_LEDTS0 ((XMC_LEDTS_GLOBAL_t *) LEDTS0)     /**< Typedef for LEDTS kernel0*/
#define XMC_LEDTS_CHECK_LEDTS0(PTR) (PTR == XMC_LEDTS0)
#else
#define XMC_LEDTS_CHECK_LEDTS0(PTR) 0
#endif

#if defined(LEDTS1)
#define XMC_LEDTS1 ((XMC_LEDTS_GLOBAL_t *) LEDTS1)     /**< Typedef for LEDTS kernel1*/
#define XMC_LEDTS_CHECK_LEDTS1(PTR) (PTR == XMC_LEDTS1)
#else
#define XMC_LEDTS_CHECK_LEDTS1(PTR) 0
#endif

#if defined(LEDTS2)
#define XMC_LEDTS2 ((XMC_LEDTS_GLOBAL_t *) LEDTS2)     /**< Typedef for LEDTS kernel2*/
#define XMC_LEDTS_CHECK_LEDTS2(PTR) (PTR == XMC_LEDTS2)
#else
#define XMC_LEDTS_CHECK_LEDTS2(PTR) 0
#endif

#define XMC_LEDTS_CHECK_KERNEL_PTR(PTR)  (XMC_LEDTS_CHECK_LEDTS0(PTR) || \
                                          XMC_LEDTS_CHECK_LEDTS1(PTR) || \
                                          XMC_LEDTS_CHECK_LEDTS2(PTR))

/**
 * Defines LEDTS module structure. This holds data and configuration registers of LEDTS modules. Use type
 * XMC_LEDTS_GLOBAL_t for this data structure.\n
 */
typedef struct XMC_LEDTS_GLOBAL{                    /*!< (@ 0x50020000) LEDTS Structure                   */
  __I  uint32_t  ID;                                /*!< (@ 0x50020000) Module Identification Register    */
  __IO uint32_t  GLOBCTL;                           /*!< (@ 0x50020004) Global Control Register           */
  __IO uint32_t  FNCTL;                             /*!< (@ 0x50020008) Function Control Register         */
  __O  uint32_t  EVFR;                              /*!< (@ 0x5002000C) Event Flag Register               */
  __IO uint32_t  TSVAL;                             /*!< (@ 0x50020010) Touch-sense TS-Counter Value      */
  __IO uint32_t  LINE[2];                           /*!< (@ 0x50020014) Line Pattern Register 0           */
  __IO uint32_t  LDCMP[2];                          /*!< (@ 0x5002001C) LED Compare Register 0            */
  __IO uint32_t  TSCMP[2];                          /*!< (@ 0x50020024) Touch-sense Compare Register 0    */
 } XMC_LEDTS_GLOBAL_t;

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/
/**
 * Defines typedef for LEDTS Global data structure. Use type XMC_LEDTS_t for this data structure.\n
 */
typedef XMC_LEDTS_GLOBAL_t XMC_LEDTS_t;

#if defined(LEDTS0)
#define XMC_LEDTS0 ((XMC_LEDTS_GLOBAL_t *) LEDTS0)     /**< Typedef for LEDTS kernel0*/
#endif

#if defined(LEDTS1)
#define XMC_LEDTS1 ((XMC_LEDTS_GLOBAL_t *) LEDTS1)     /**< Typedef for LEDTS kernel1*/
#endif


/**
 *  Defines return value of an API. Use type XMC_LEDTS_STATUS_t for this enum.
 */
typedef enum XMC_LEDTS_STATUS
{
  XMC_LEDTS_STATUS_SUCCESS  =  0,           /**< API fulfills request */
  XMC_LEDTS_STATUS_RUNNING  =  1,           /**< The kernel-counter is currently running */
  XMC_LEDTS_STATUS_ERROR    =  2,           /**< API cannot fulfill request */
  XMC_LEDTS_STATUS_IDLE     =  3            /**< The kernel-counter is currently idle */
} XMC_LEDTS_STATUS_t;

/**
 *  Defines return value for checking interrupt flag. Use type XMC_LEDTS_FLAG_STATUS_t for this enum.
 */
typedef enum XMC_LEDTS_FLAG_STATUS
{
  XMC_LEDTS_FLAG_STATUS_NO   = 0,            /**< Flag not raised */
  XMC_LEDTS_FLAG_STATUS_YES  = 1             /**< Flag is raised */
} XMC_LEDTS_FLAG_STATUS_t;

/**
 *  Defines Touch-Sense function enable/disable. Use type XMC_LEDTS_TS_FUNC_t for this enum.
 */
typedef enum XMC_LEDTS_TS_FUNC
{
  XMC_LEDTS_TS_FUNC_DISABLE = 0,             /**< Disable touch-sense function */
  XMC_LEDTS_TS_FUNC_ENABLE  = 1              /**< Enable touch-sense function */
} XMC_LEDTS_TS_FUNC_t;

/**
 *  Defines LED function enable/disable. Use type XMC_LEDTS_LED_FUNC_t for this enum.
 */
typedef enum XMC_LEDTS_LED_FUNC
{
  XMC_LEDTS_LED_FUNC_DISABLE = 0,            /**< Disable LED function */
  XMC_LEDTS_LED_FUNC_ENABLE  = 1             /**< Enable LED function */
} XMC_LEDTS_LED_FUNC_t;

/**
 *  Defines Clock master enable/disable. Use type for XMC_LEDTS_CLOCK_TYPE_t for this enum.
 */
typedef enum XMC_LEDTS_CLOCK_TYPE
{
  XMC_LEDTS_CLOCK_TYPE_MASTER = 0,           /**< Kernel generates its own clock */
  XMC_LEDTS_CLOCK_TYPE_SLAVE  = 1            /**< Clock is taken from another master kernel */
} XMC_LEDTS_CLOCK_TYPE_t;

/**
 *  Defines enable/disable of autoscan time period synchronization. Use type XMC_LEDTS_TP_SYNC_t for this enum.
 */
typedef enum XMC_LEDTS_TP_SYNC
{
  XMC_LEDTS_TP_SYNC_DISABLE = 0,             /**< Synchronization is disabled */
  XMC_LEDTS_TP_SYNC_ENABLE  = 1              /**< Synchronization enabled on Kernel0 autoscan time period */
} XMC_LEDTS_TP_SYNC_t;

/**
 *  Defines Suspend request configuration. Use type XMC_LEDTS_SUSPEND_t for this enum.
 */
typedef enum XMC_LEDTS_SUSPEND
{
  XMC_LEDTS_SUSPEND_DISABLE = 0,             /**< Ignore suspend request */
  XMC_LEDTS_SUSPEND_ENABLE  = 1              /**< Enable suspend according to request */
} XMC_LEDTS_SUSPEND_t;

/**
 *  Defines number of bits to mask for time frame event validation. Use type XMC_LEDTS_TS_COUNTER_MASK_t for this enum.
 */
typedef enum XMC_LEDTS_TS_COUNTER_MASK
{
  XMC_LEDTS_TS_COUNTER_MASK_1_LSB = 0,       /**< Mask LSB bit only */
  XMC_LEDTS_TS_COUNTER_MASK_2_LSB = 1,       /**< Mask 2 LSB bits */
  XMC_LEDTS_TS_COUNTER_MASK_3_LSB = 2,       /**< Mask 3 LSB bits */
  XMC_LEDTS_TS_COUNTER_MASK_4_LSB = 3,       /**< Mask 4 LSB bits */
  XMC_LEDTS_TS_COUNTER_MASK_5_LSB = 4,       /**< Mask 5 LSB bits */
  XMC_LEDTS_TS_COUNTER_MASK_6_LSB = 5,       /**< Mask 6 LSB bits */
  XMC_LEDTS_TS_COUNTER_MASK_7_LSB = 6,       /**< Mask 7 LSB bits */
  XMC_LEDTS_TS_COUNTER_MASK_8_LSB = 7        /**< Mask 8 LSB bits */
} XMC_LEDTS_TS_COUNTER_MASK_t;

/**
 *  Defines Enable/disable of (extended) time frame validation. Use type XMC_LEDTS_TF_VALIDATION_t for this enum.
 */
typedef enum XMC_LEDTS_TF_VALIDATION
{
  XMC_LEDTS_TF_VALIDATION_DISABLE = 0,       /**< Disable time frame validation */
  XMC_LEDTS_TF_VALIDATION_ENABLE  = 1        /**< Enable time frame validation */
} XMC_LEDTS_TF_VALIDATION_t;

/**
 *  Defines Enable or disable interrupts. Use type XMC_LEDTS_INTERRUPT_t for this enum.
 */
typedef enum XMC_LEDTS_INTERRUPT
{
  XMC_LEDTS_INTERRUPT_TIMESLICE  = LEDTS_GLOBCTL_ITS_EN_Msk,  /**< Enable or Disable time slice interrupt */
  XMC_LEDTS_INTERRUPT_TIMEFRAME  = LEDTS_GLOBCTL_ITF_EN_Msk,  /**< Enable or Disable time frame interrupt */
  XMC_LEDTS_INTERRUPT_TIMEPERIOD = LEDTS_GLOBCTL_ITP_EN_Msk   /**< Enable or Disable autoscan time period interrupt */
} XMC_LEDTS_INTERRUPT_t;

/**
 *  Defines Touch-Sense TSIN pad turn. Use type XMC_LEDTS_PAD_TURN_t for this enum.
 */
typedef enum XMC_LEDTS_PAD_TURN
{
  XMC_LEDTS_PAD_TURN_0 = 0,       /**< TSIN0 is next or currently active */
  XMC_LEDTS_PAD_TURN_1 = 1,       /**< TSIN1 is next or currently active */
  XMC_LEDTS_PAD_TURN_2 = 2,       /**< TSIN2 is next or currently active */
  XMC_LEDTS_PAD_TURN_3 = 3,       /**< TSIN3 is next or currently active */
  XMC_LEDTS_PAD_TURN_4 = 4,       /**< TSIN4 is next or currently active */
  XMC_LEDTS_PAD_TURN_5 = 5,       /**< TSIN5 is next or currently active */
  XMC_LEDTS_PAD_TURN_6 = 6,       /**< TSIN6 is next or currently active */
  XMC_LEDTS_PAD_TURN_7 = 7        /**< TSIN7 is next or currently active */
} XMC_LEDTS_PAD_TURN_t;

/**
 *  Defines software control for Touch-Sense pad turn. Use type XMC_LEDTS_PAD_TURN_SW_CONTROL_t for this enum.
 */
typedef enum XMC_LEDTS_PAD_TURN_SW_CONTROL
{
  XMC_LEDTS_SW_CONTROL_DISABLE = 0,             /**< Disable software control. Auto hardware control */
  XMC_LEDTS_SW_CONTROL_ENABLE  = 1              /**< Enable software control for pad turn */
} XMC_LEDTS_PAD_TURN_SW_CONTROL_t;

/**
 *  Defines External pull-up on touch-sense pin. Use type XMC_LEDTS_EXT_PULLUP_COLA_t for this enum.
 */
typedef enum XMC_LEDTS_EXT_PULLUP_COLA
{
  XMC_LEDTS_EXT_PULLUP_COLA_DISABLE = 0,         /**< Disable external pull-up. Internal pull-up is active */
  XMC_LEDTS_EXT_PULLUP_COLA_ENABLE  = 1          /**< Enable external pull-up */
} XMC_LEDTS_EXT_PULLUP_COLA_t;

/**
 *  Defines number of accumulation counts on Touch-Sense input. Use type XMC_LEDTS_ACCUMULATION_COUNT_t for this enum.
 */
typedef enum XMC_LEDTS_ACCUMULATION_COUNT
{
  XMC_LEDTS_ACCUMULATION_COUNT_1_TIME   = 0,         /**< Accumulate once */
  XMC_LEDTS_ACCUMULATION_COUNT_2_TIMES  = 1,         /**< Accumulate twice */
  XMC_LEDTS_ACCUMULATION_COUNT_3_TIMES  = 2,         /**< Accumulate thrice */
  XMC_LEDTS_ACCUMULATION_COUNT_4_TIMES  = 3,         /**< Accumulate 4 times */
  XMC_LEDTS_ACCUMULATION_COUNT_5_TIMES  = 4,         /**< Accumulate 5 times*/
  XMC_LEDTS_ACCUMULATION_COUNT_6_TIMES  = 5,         /**< Accumulate 6 times */
  XMC_LEDTS_ACCUMULATION_COUNT_7_TIMES  = 6,         /**< Accumulate 7 times*/
  XMC_LEDTS_ACCUMULATION_COUNT_8_TIMES  = 7,         /**< Accumulate 8 times */
  XMC_LEDTS_ACCUMULATION_COUNT_9_TIMES  = 8,         /**< Accumulate 9 times*/
  XMC_LEDTS_ACCUMULATION_COUNT_10_TIMES = 9,         /**< Accumulate 10 times */
  XMC_LEDTS_ACCUMULATION_COUNT_11_TIMES = 10,        /**< Accumulate 11 times*/
  XMC_LEDTS_ACCUMULATION_COUNT_12_TIMES = 11,        /**< Accumulate 12 times*/
  XMC_LEDTS_ACCUMULATION_COUNT_13_TIMES = 12,        /**< Accumulate 13 times*/
  XMC_LEDTS_ACCUMULATION_COUNT_14_TIMES = 13,        /**< Accumulate 14 times*/
  XMC_LEDTS_ACCUMULATION_COUNT_15_TIMES = 14,        /**< Accumulate 15 times*/
  XMC_LEDTS_ACCUMULATION_COUNT_16_TIMES = 15         /**< Accumulate 16 times*/
} XMC_LEDTS_ACCUMULATION_COUNT_t;

/**
 *  Defines enable/disable of common compare configuration for Touch-Sense. Use type XMC_LEDTS_COMMON_COMPARE_t
 *  for this enum.
 */
typedef enum XMC_LEDTS_COMMON_COMPARE
{
  XMC_LEDTS_COMMON_COMPARE_DISABLE = 0,         /**< Disable common compare for touch-sense */
  XMC_LEDTS_COMMON_COMPARE_ENABLE  = 1          /**< Enable common compare for touch-sense */
} XMC_LEDTS_COMMON_COMPARE_t;

/**
 *  Defines extended Touch-Sense output for pin-low-level. Use type XMC_LEDTS_EXTEND_TS_OUTPUT_t for this enum.
 */
typedef enum XMC_LEDTS_EXTEND_TS_OUTPUT
{
  XMC_LEDTS_EXTEND_TS_OUTPUT_BY_1_CLK  = 0,     /**< Extend Touch-Sense output for pin-low-level by 1 ledts_clk */
  XMC_LEDTS_EXTEND_TS_OUTPUT_BY_4_CLK  = 1,     /**< Extend Touch-Sense output for pin-low-level by 4 ledts_clk */
  XMC_LEDTS_EXTEND_TS_OUTPUT_BY_8_CLK  = 2,     /**< Extend Touch-Sense output for pin-low-level by 8 ledts_clk */
  XMC_LEDTS_EXTEND_TS_OUTPUT_BY_16_CLK = 3      /**< Extend Touch-Sense output for pin-low-level by 16 ledts_clk */
} XMC_LEDTS_EXTEND_TS_OUTPUT_t;

/**
 *  Defines enable/disable of Touch-Sense counter auto reset configuration. Use type XMC_LEDTS_TS_COUNTER_AUTO_RESET_t
 *  for this enum.
 */
typedef enum XMC_LEDTS_TS_COUNTER_AUTO_RESET
{
  XMC_LEDTS_TS_COUNTER_AUTO_RESET_DISABLE = 0,     /**< Disable Touch-Sense counter automatic reset */
  XMC_LEDTS_TS_COUNTER_AUTO_RESET_ENABLE  = 1      /**< Enable Touch-Sense counter automatic reset to 0x00 */
} XMC_LEDTS_TS_COUNTER_AUTO_RESET_t;

/**
 *  Defines enable/disable of Touch-Sense counter saturation configuration. Use type XMC_LEDTS_TS_COUNTER_SATURATION_t
 *  for this enum.
 */
typedef enum XMC_LEDTS_TS_COUNTER_SATURATION
{
  XMC_LEDTS_TS_COUNTER_SATURATION_DISABLE = 0, /**< Disabled. Touch-Sense counter overflows when it reaches 0xFF */
  XMC_LEDTS_TS_COUNTER_SATURATION_ENABLE  = 1  /**< Enabled. Touch-Sense counter stops counting when it reaches 0xFF */
} XMC_LEDTS_TS_COUNTER_SATURATION_t;

/**
 *  Defines number of Touch-Sense Input (for HW pad turn control). Use type XMC_LEDTS_NUMBER_TS_INPUT_t for this enum.
 */
typedef enum XMC_LEDTS_NUMBER_TS_INPUT
{
  XMC_LEDTS_NUMBER_TS_INPUT_1  = 0,      /**< Only TSIN0 is used */
  XMC_LEDTS_NUMBER_TS_INPUT_2 = 1,       /**< TSIN0 & TSIN1 are used */
  XMC_LEDTS_NUMBER_TS_INPUT_3 = 2,       /**< TSIN0-TSIN2 are used */
  XMC_LEDTS_NUMBER_TS_INPUT_4 = 3,       /**< TSIN0-TSIN3 are used */
  XMC_LEDTS_NUMBER_TS_INPUT_5 = 4,       /**< TSIN0-TSIN4 are used */
  XMC_LEDTS_NUMBER_TS_INPUT_6 = 5,       /**< TSIN0-TSIN5 are used */
  XMC_LEDTS_NUMBER_TS_INPUT_7 = 6,       /**< TSIN0-TSIN6 are used */
  XMC_LEDTS_NUMBER_TS_INPUT_8 = 7        /**< TSIN0-TSIN7 are used */
} XMC_LEDTS_NUMBER_TS_INPUT_t;

/**
 *  Defines level of LED column when active. Use type XMC_LEDTS_ACTIVE_LEVEL_LED_COL_t for this enum.
 */
typedef enum XMC_LEDTS_ACTIVE_LEVEL_LED_COL
{
  XMC_LEDTS_ACTIVE_LEVEL_LED_COL_LOW  = 0, /**< LED column pins output low when active */
  XMC_LEDTS_ACTIVE_LEVEL_LED_COL_HIGH = 1  /**< LED column pins output high when active */
} XMC_LEDTS_ACTIVE_LEVEL_LED_COL_t;

/**
 *  Defines Number of LED columns. Use type XMC_LEDTS_NUMBER_LED_COLUMNS_t for this enum.
 */
typedef enum XMC_LEDTS_NUMBER_LED_COLUMNS
{
  XMC_LEDTS_NUMBER_LED_COLUMNS_1 = 0,     /**< COLA only if TS is enabled, else COL0 only */
  XMC_LEDTS_NUMBER_LED_COLUMNS_2 = 1,     /**< COLA,COL0 if TS is enabled, else COL0-1 */
  XMC_LEDTS_NUMBER_LED_COLUMNS_3 = 2,     /**< COLA,COL0-1 if TS is enabled, else COL0-2 */
  XMC_LEDTS_NUMBER_LED_COLUMNS_4 = 3,     /**< COLA,COL0-2 if TS is enabled, else COL0-3 */
  XMC_LEDTS_NUMBER_LED_COLUMNS_5 = 4,     /**< COLA,COL0-3 if TS is enabled, else COL0-4 */
  XMC_LEDTS_NUMBER_LED_COLUMNS_6 = 5,     /**< COLA,COL0-4 if TS is enabled, else COL0-5 */
  XMC_LEDTS_NUMBER_LED_COLUMNS_7 = 6,     /**< COLA,COL0-5 if TS is enabled, else COL0-6 */
  XMC_LEDTS_NUMBER_LED_COLUMNS_8 = 7      /**< Only possible if TS is disabled; COLA,COL0-6 used */
} XMC_LEDTS_NUMBER_LED_COLUMNS_t;

/**
 *  Defines Interrupt flag status. Use type XMC_LEDTS_TS_INTERRUPT_FLAG_t for this enum.
 */
typedef enum XMC_LEDTS_INTERRUPT_FLAG
{
  XMC_LEDTS_INTERRUPT_FLAG_TIMESLICE = LEDTS_EVFR_TSF_Msk,                 /**< Time slice interrupt flag status */
  XMC_LEDTS_INTERRUPT_FLAG_TIMEFRAME = LEDTS_EVFR_TFF_Msk,                 /**< Time frame interrupt flag status */
  XMC_LEDTS_INTERRUPT_FLAG_TIMEPERIOD = LEDTS_EVFR_TPF_Msk,                /**< Time period interrupt flag status */
  XMC_LEDTS_INTERRUPT_FLAG_TSCOUNTER_OVERFLOW = LEDTS_EVFR_TSCTROVF_Msk,   /**< TS counter overflow flag status */
} XMC_LEDTS_TS_INTERRUPT_FLAG_t;

/**
 *  Defines (Extended) Time frame interrupt flag status. Use type XMC_LEDTS_TF_INTERRUPT_FLAG_t for this enum.
 */
typedef enum XMC_LEDTS_TF_INTERRUPT_FLAG
{
  XMC_LEDTS_TF_INTERRUPT_FLAG_INACTIVE = 0,          /**< (Extended) Time frame interrupt not active */
  XMC_LEDTS_TF_INTERRUPT_FLAG_ACTIVE   = 1           /**< (Extended) Time frame interrupt active */
} XMC_LEDTS_TF_INTERRUPT_FLAG_t;

/**
 *  Defines Autoscan time period interrupt flag status. Use type XMC_LEDTS_AUTOSCAN_INTERRUPT_FLAG_t for this enum. 
 */
typedef enum XMC_LEDTS_AUTOSCAN_INTERRUPT_FLAG
{
  XMC_LEDTS_AUTOSCAN_INTERRUPT_FLAG_INACTIVE = 0,    /**< Autoscan time period interrupt not active */
  XMC_LEDTS_AUTOSCAN_INTERRUPT_FLAG_ACTIVE   = 1     /**< Autoscan time period interrupt active */
} XMC_LEDTS_AUTOSCAN_INTERRUPT_FLAG_t;

/**
 *  Defines Touch-Sense counter overflow indication. Use type XMC_LEDTS_TS_COUNTER_OVERLOW_FLAG_t for this enum.
 */
typedef enum XMC_LEDTS_TS_COUNTER_OVERLOW_FLAG
{
  XMC_LEDTS_TS_COUNTER_OVERLOW_FLAG_NO  = 0,         /**< Touch-sense counter has not overflowed */
  XMC_LEDTS_TS_COUNTER_OVERLOW_FLAG_YES = 1          /**< Touch-sense counter has overflowed at least once */
} XMC_LEDTS_TS_COUNTER_OVERLOW_FLAG_t;

/**
 *  Defines available LED columns. Use type XMC_LEDTS_LED_COLUMN_t for this enum.
 */
typedef enum XMC_LEDTS_LED_COLUMN
{
  XMC_LEDTS_LED_COLUMN_0 = 0,       /**< Denotes LED Column 0 */
  XMC_LEDTS_LED_COLUMN_1 = 1,       /**< Denotes LED Column 1 */
  XMC_LEDTS_LED_COLUMN_2 = 2,       /**< Denotes LED Column 2 */
  XMC_LEDTS_LED_COLUMN_3 = 3,       /**< Denotes LED Column 3 */
  XMC_LEDTS_LED_COLUMN_4 = 4,       /**< Denotes LED Column 4 */
  XMC_LEDTS_LED_COLUMN_5 = 5,       /**< Denotes LED Column 5 */
  XMC_LEDTS_LED_COLUMN_6 = 6,       /**< Denotes LED Column 6 */
  XMC_LEDTS_LED_COLUMN_A = 7        /**< Denotes LED Column A */
} XMC_LEDTS_LED_COLUMN_t;

/**
 *  Defines available Touch-Sense inputs. Use type XMC_LEDTS_TS_INPUT_t for this enum.
 */
typedef enum XMC_LEDTS_TS_INPUT
{
  XMC_LEDTS_TS_INPUT_0 = 0,         /**< TSIN0 - Denotes touch-sense line 1 */
  XMC_LEDTS_TS_INPUT_1 = 1,         /**< TSIN1 - Denotes touch-sense line 2 */
  XMC_LEDTS_TS_INPUT_2 = 2,         /**< TSIN2 - Denotes touch-sense line 3*/
  XMC_LEDTS_TS_INPUT_3 = 3,         /**< TSIN3 - Denotes touch-sense line 4*/
  XMC_LEDTS_TS_INPUT_4 = 4,         /**< TSIN4 - Denotes touch-sense line 5*/
  XMC_LEDTS_TS_INPUT_5 = 5,         /**< TSIN5 - Denotes touch-sense line 6*/
  XMC_LEDTS_TS_INPUT_6 = 6,         /**< TSIN6 - Denotes touch-sense line 7*/
  XMC_LEDTS_TS_INPUT_7 = 7          /**< TSIN7 - Denotes touch-sense line 8*/
} XMC_LEDTS_TS_INPUT_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/
 /*Anonymous structure/union guard start*/
 #if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__TASKING__)
  #pragma warning 586
#endif

/**
 *  Data structure for initialization of global features common to LED and touch-sense function. Use 
 *  type XMC_LEDTS_GLOBAL_CONFIG_t for this structure.
 */
typedef struct XMC_LEDTS_GLOBAL_CONFIG
{
  union
  {
    struct
    {
      uint32_t : 2;
      uint32_t clock_generation:1;          /**< When this bit is set LEDTS counter takes its clock from another master
                                                 kernel. Kernel generates its own clock when this bit is not set (CMTR).
												 Refer  @ref XMC_LEDTS_CLOCK_TYPE_t enum for possible values. */
      
      uint32_t autoscan_synchronization:1;  /**< Set this bit to synchronize start of autoscan time period with master
                                                 kernel(ENSYNC). Refer @ref XMC_LEDTS_TP_SYNC_t enum for possible values. */
      uint32_t : 4;
      uint32_t suspend_response:1;          /**< Suspend request configuration(SUSCFG).
                                                 Refer @ref XMC_LEDTS_SUSPEND_t enum for possible values.*/
    };
    uint32_t globctl;
  };
}XMC_LEDTS_GLOBAL_CONFIG_t;

/**
 *  Data structure for LED function initialization. Use type XMC_LEDTS_LED_CONFIG_t for this structure.
 */
typedef struct XMC_LEDTS_LED_CONFIG
{
  union
  {
    struct
    {
      uint32_t : 28;
      uint32_t column_active_level:1;   /**< When this bit is set LED column level is active high, otherwise column
	                                         level is active low(COLLEV). Refer @ref XMC_LEDTS_ACTIVE_LEVEL_LED_COL_t
											 enum for possible values.*/
                                           
      uint32_t no_of_led_columns:3;     /**< Defines number of LED columns(NR_LEDCOL). Range 0 - 7.
                                             Refer @ref XMC_LEDTS_NUMBER_LED_COLUMNS_t enum for possible values. */
    };
    uint32_t fnctl;
  };
}XMC_LEDTS_LED_CONFIG_t;

/**
 *  Data structure for basic Touch-Sense function initialization. Use type XMC_LEDTS_TS_CONFIG_BASIC_t for
 *  this structure. 
 */
typedef struct XMC_LEDTS_TS_CONFIG_BASIC
{
  union
  {
    struct
    {
      uint32_t : 16;
      uint32_t no_of_accumulation:4; /**< Defines number of times touch-sense input pin is enabled in touch-sense
	                                      time slice of consecutive frames(ACCCNT). Range 0 - 15.
                                          Refer @ref XMC_LEDTS_ACCUMULATION_COUNT_t enum type for possible values. */
                                          
      uint32_t common_compare:1;     /**< When this bit is set it enables common compare for all touch sense inputs.
                                          Disables common compare when not set(TSCCMP).
                                          Refer @ref XMC_LEDTS_COMMON_COMPARE_t enum for possible values.*/
      uint32_t : 2;
      uint32_t counter_auto_reset:1; /**< When this bit is set TS-counter is automatically reset to 00H on first pad
	                                      turn of a new touch-sense pin(TSCTRR).
                                          Refer @ref XMC_LEDTS_TS_COUNTER_AUTO_RESET_t enum for possible values.*/
                                          
      uint32_t counter_saturation:1; /**< When this bit is set TS-counter stops counting in the touch-sense time slice
	                                         of the same frame when it reaches FFH (TSCTRSAT). 
                                             Refer @ref XMC_LEDTS_TS_COUNTER_SATURATION_t enum for possible values. */
                                          
      uint32_t no_of_touch_inputs:3; /**< Defines number of touch-sense inputs (NR_TSIN). Range 0 - 7.
	                                      Refer @ref XMC_LEDTS_NUMBER_TS_INPUT_t enum for possible values. */
    };
    uint32_t fnctl;
  };
}XMC_LEDTS_TS_CONFIG_BASIC_t;

/**
 *  Data structure for advanced Touch-Sense function initialization. Use type XMC_LEDTS_TS_CONFIG_ADVANCED_t
 *  for this structure.
 */
typedef struct XMC_LEDTS_TS_CONFIG_ADVANCED
{
  union
  {
    struct
    {
      uint32_t : 9;
      uint32_t validation_mask:3;       /**< This bit-field defines number of LSB bits to mask for TS counter and shadow
	                                         TS counter comparison when Time Frame validation is enabled(MASKVAL).
                                             Refer @ref XMC_LEDTS_TS_COUNTER_MASK_t enum for possible values.*/
                                            
      uint32_t time_frame_validation:1; /**< Disable or enable (extended) time frame validation(FENVAL).
	                                         when validation fails time frame interrupt is not triggered.
											 Refer @ref XMC_LEDTS_TF_VALIDATION_t enum for possible values.*/
      uint32_t : 1;
      uint32_t : 1;
    };
    uint32_t globctl;
  };
  union
  {
    struct
    {
      uint32_t first_pad_turn:3;        /**< This bit-field denotes TSIN[x] pin on which oscillations are measured
	                                         currently/next(PADT). Refer @ref XMC_LEDTS_PAD_TURN_t enum for possible
											 values.*/
                                            
      uint32_t pad_turn_control:1;      /**< Control pad turn via HW or SW(PADTSW). 
                                             Refer @ref XMC_LEDTS_PAD_TURN_SW_CONTROL_t enum for possible values. */
                                            
      uint32_t external_pullup:1;       /**< Disable or enable external pull-up on touch pin(EPULL).
                                             Refer @ref XMC_LEDTS_EXT_PULLUP_COLA_t enum for possible values. */
      uint32_t : 16;
      uint32_t pin_low_extend:2;        /**< This bit extends touch-sense output for pin-low-level configuration for
                                             adjustment of oscillation per user system. 
											 Refer @ref XMC_LEDTS_EXTEND_TS_OUTPUT_t enum for possible values. */
    };
    uint32_t fnctl;
  };
}XMC_LEDTS_TS_CONFIG_ADVANCED_t;

/*Anonymous structure/union guard end*/
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__TASKING__)
  #pragma warning restore
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*********************************************************************************************************************
 * API Prototypes
 ********************************************************************************************************************/

/**
 *
 * @param ledts   Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 * @param config  Pointer to constant LEDTS Global configuration structure. Refer @ref XMC_LEDTS_GLOBAL_CONFIG_t
 *                data structure.<BR>
 *
 * @return
 *      XMC_LEDTS_STATUS_SUCCESS - on success.<BR>
 *      XMC_LEDTS_STATUS_RUNNING - LEDTS module currently active. Refer @ref XMC_LEDTS_STATUS_t enum type.<BR>
 *
 * \par<b>Description</b><br>
 * Initializes and configures GLOBCTL register of \a ledts with configuration data pointed by \a config. 
 * \par
 * This API selects clock source (GLOBCTL.CMTR), enables/disables auto scan sync(GLOBCTL.ENSYNC) & 
 * suspend config(GLOBCTL.SUSCFG).<BR>
 * Call this API to initialize global register fields common to both LED and touch-sense(TS).\n
 *
 * \par<b>Note</b><BR>
 * LEDTS should be stopped by using XMC_LEDTS_StopCounter()before calling this API.\n
 *
 * \par<b>Related API's</b><BR>
 * XMC_LEDTS_StopCounter()\n
 * 
 */
XMC_LEDTS_STATUS_t XMC_LEDTS_InitGlobal(XMC_LEDTS_t *const ledts, const XMC_LEDTS_GLOBAL_CONFIG_t *config);

/**
 *
 * @param ledts   Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 * @param config  Pointer to constant LEDTS LED configuration structure. Refer @ref XMC_LEDTS_LED_CONFIG_t
 *                data structure.<BR>
 *
 * @return
 *      XMC_LEDTS_STATUS_SUCCESS - on success.<BR>
 *      XMC_LEDTS_STATUS_RUNNING - LEDTS module currently active/running. Refer @ref XMC_LEDTS_STATUS_t enum type.<BR>
 *
 * \par<b>Description</b><br>
 * Configures FNCTL register of \a ledts with configuration data pointed by \a config and enables LED functionality.
 * \par
 * This API sets number of LED columns(FNCTL.NR_LEDCOL), column level(FNCTL.COLLEV) and enables LED
 * functionality(GLOBCTL.LD_EN).
 * \par
 * Call this API to Configure \a FNCTL & \a GLOBCTL registers for LED-driving function. Global initialization of
 * LEDTS module should be done by calling XMC_LEDTS_InitGlobal() prior to calling this API.\n
 *
 * \par<b>Note</b><BR>
 * LEDTS should be stopped by using XMC_LEDTS_StopCounter() before calling this API.\n
 *
 * \par<b>Related API's</b><BR>
 * XMC_LEDTS_StopCounter() XMC_LEDTS_InitGlobal()\n
 * 
 */
XMC_LEDTS_STATUS_t XMC_LEDTS_InitLED(XMC_LEDTS_t *const ledts, const XMC_LEDTS_LED_CONFIG_t *config);

/**
 *
 * @param ledts   Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 * @param config  Pointer to constant LEDTS TS basic configuration structure. Refer @ref XMC_LEDTS_TS_CONFIG_BASIC_t
 *                data structure.<BR>
 *
 * @return
 *      XMC_LEDTS_STATUS_SUCCESS - on success.<BR>
 *      XMC_LEDTS_STATUS_RUNNING - LEDTS module currently active/running. Refer @ref XMC_LEDTS_STATUS_t enum type.<BR>
 *
 * \par<b>Description</b><br>
 * Configures FNCTL register of \a ledts for basic touch sense functionality with configuration data pointed by \a config
 * and enables TS functionality.
 * \par
 * This API sets number of touch inputs(FNCTL.NR_TSIN), accumulate count on touch input(FNCTL.ACCCNT).
 * \par
 * This API Enables/disables common compare(FNCTL.TSCCMP), TS counter auto reset(FNCTL.TSCTRR), counter 
 * saturation(FNCTL.TSCTRSAT) and enables TS functionality(GLOBCTL.TS_EN). 
 * \par
 * Call this API to configure \a FNCTL & \a GLOBCTL registers for basic touch sense function. Global initialization
 * of LEDTS module should be done by calling XMC_LEDTS_InitGlobal() prior to calling this API.\n
 *
 * \par<b>Note</b><BR>
 * LEDTS should be stopped by using XMC_LEDTS_StopCounter() before calling this API.\n
 *
 * \par<b>Related API's</b><BR>
 * XMC_LEDTS_StopCounter() XMC_LEDTS_InitGlobal()\n
 * 
 */
XMC_LEDTS_STATUS_t XMC_LEDTS_InitTSBasic(XMC_LEDTS_t *const ledts, const XMC_LEDTS_TS_CONFIG_BASIC_t *config);

/**
 *
 * @param ledts   Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 * @param config  Pointer to constant LEDTS TS advanced configuration structure.
 *                Refer @ref XMC_LEDTS_TS_CONFIG_ADVANCED_t data structure.<BR>
 *
 * @return
 *      XMC_LEDTS_STATUS_SUCCESS - on success.<BR>
 *      XMC_LEDTS_STATUS_RUNNING - LEDTS module currently active/running. Refer @ref XMC_LEDTS_STATUS_t enum type.<BR>
 *
 * \par<b>Description</b><br>
 * Configures FNCTL register of \a ledts for advanced touch sense functionality with configuration data pointed by
 * \a config.
 * \par
 * This API sets number of mask-bits for time frame validation(GLOBCTL.MASKVAL) & first touch input to be active(if
 * pad turn control is set to S/W)(FNCTL.PADT).
 * \par
 * Enables/disables time frame interrupt(GLOBCTL.ITF_EN), external pull up on touch pin(FNCTL.EPULL) & H/W or S/W
 * control of pad turn(if set to H/W, touch input activation is done in round-robin sequence, starting from TSIN0)
 * (FNCTL.PADTSW).
 * \par
 * Call this API to initialize registers for advanced touch sense function. Before calling this API Call
 * XMC_LEDTS_InitGlobal() to do Global initialization and XMC_LEDTS_InitTSBasic() to do basic init of touch-sense.\n
 *
 * \par<b>Note</b><BR>
 * LEDTS should be stopped by using XMC_LEDTS_StopCounter() before calling this API.\n
 *
 * \par<b>Related API's</b><BR>
 * XMC_LEDTS_StopCounter(), XMC_LEDTS_InitTSBasic().\n
 * 
 */
XMC_LEDTS_STATUS_t XMC_LEDTS_InitTSAdvanced (XMC_LEDTS_t *const ledts, const XMC_LEDTS_TS_CONFIG_ADVANCED_t *config);

/**
 *
 * @param ledts      Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 * @param prescaler  Constant prescaler value. Range: 0H to FFFFH.<BR>
 *
 * @return
 *      None.<BR>
 *
 * \par<b>Description</b><br>
 * Kick-starts the LEDTS module by programming CLK_PS bit field of GLOBCTL register with \a prescaler value to start
 * the LEDTS-counter.
 * \par
 * To set LEDTS counter at least one of the touch-sense or LED function should be enabled.
 * Call this API to start LEDTS counter.\n
 *
 * \par<b>Note</b><BR>
 * This should be called after all used modules have been initialized.\n
 *
 * \par<b>Related API's</b><BR>
 * XMC_LEDTS_StopCounter(), XMC_LEDTS_InitLED(), XMC_LEDTS_InitTSBasic(), XMC_LEDTS_InitTSAdvanced().\n
 * 
 */
void XMC_LEDTS_StartCounter(XMC_LEDTS_t *const ledts, const uint16_t prescaler);

/**
 *
 * @param ledts  Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 *
 * @return
 *      None.<BR>
 *
 * \par<b>Description</b><br>
 * Stops the LEDTS module by programming the CLK_PS bit field(with value = 0) of GLOBCTL register. <br>
 * This could be done when it is required to change some module configuration which requires the LEDTS-counter 
 * to be stopped before the register bit/bit field can be programmed.<br> Call this API to stop LEDTS counter.\n
 *
 * \par<b>Related API's</b><BR>
 * XMC_LEDTS_StartCounter(), XMC_LEDTS_InitLED(), XMC_LEDTS_InitTSBasic(), XMC_LEDTS_InitTSAdvanced().\n
 * 
 */
void XMC_LEDTS_StopCounter(XMC_LEDTS_t *const ledts);

/**
 *
 * @param ledts  Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 * @param interrupt_mask  mask value of @ref XMC_LEDTS_INTERRUPT_t enum type.<BR>
 * 
 * @return
 *      None.<BR>
 *
 * \par<b>Description</b><br>
 * Enables requested interrupt type by configuring GLOBCTL register with masked value \a interrupt_mask.<BR>
 * \par
 * This API can be used to enable time slice(GLOBCTL.ITS_EN) or time frame(GLOBCTL.ITF_EN )or time period
 * (GLOBCTL.ITP_EN)interrupt or any combination of these interrupts by passing appropriate bitwise ORed mask value.\n
 *
 * \par<b>Related API's</b><BR>
 * XMC_LEDTS_DisableInterrupt().\n
 * 
 */
__STATIC_INLINE void XMC_LEDTS_EnableInterrupt(XMC_LEDTS_t *const ledts, uint32_t interrupt_mask)
{
  XMC_ASSERT("XMC_LEDTS_EnableInterrupt:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));

  ledts->GLOBCTL |= interrupt_mask;
}

/**
 *
 * @param ledts  Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 * @param interrupt_mask  mask value of @ref XMC_LEDTS_INTERRUPT_t enum type.<BR>
 * 
 * @return
 *      None.<BR>
 *
 * \par<b>Description</b><br>
 * Disables requested interrupt type by configuring GLOBCTL register with masked value \a interrupt_mask.<BR>
 * \par
 * This API can be used to disable time slice(GLOBCTL.ITS_EN) or time frame(GLOBCTL.ITF_EN )or time period
 * (GLOBCTL.ITP_EN)interrupt or any combination of these interrupts by passing appropriate bitwise ORed mask value.\n
 *
 * \par<b>Related API's</b><BR>
 * XMC_LEDTS_EnableInterrupt().\n
 * 
 */
__STATIC_INLINE void XMC_LEDTS_DisableInterrupt(XMC_LEDTS_t *const ledts, uint32_t interrupt_mask)
{
  XMC_ASSERT("XMC_LEDTS_DisableInterrupt:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));

  ledts->GLOBCTL &= ~interrupt_mask;
}

/**
 *
 * @param ledts  Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 *
 * @return
 *      Status flags for events. Possible event flags are 0x01(time slice), 0x02(time frame),
        0x04(time period), 0x08(TS counter overflow).<BR>
 *
 * \par<b>Description</b><br>
 * Returns interrupt status flag by reading TSF(time slice), TFF(time frame), TPF(time period), TSCTROVF
 * (touch sense counter overflow) fields of EVFR register.<br>
 * Typically used in interrupt handler to find out which event has triggered the interrupt.\n
 *
 * \par<b>Note</b><BR>
 * These flags are set on event regardless of corresponding interrupt is enabled or not.\n
 *
 * \par<b>Related API's</b><BR>
 * XMC_LEDTS_ClearInterruptFlag().\n
 * 
 */
uint32_t XMC_LEDTS_ReadInterruptFlag(XMC_LEDTS_t *const ledts);

/**
 *
 * @param ledts  Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 * @param interrupt_mask  mask value of @ref XMC_LEDTS_INTERRUPT_t enum type.<BR>
 *
 * @return
 *      None.<BR>
 *
 * \par<b>Description</b><br>
 * Clears interrupt status flags in EVFR register as indicated by mask value \a interrupt_mask.<br>
 * This API sets EVFR.CTSF, EVFR.CTFF, EVFR.CTPF bit fields to clear time slice, time frame or time period interrupts
 * respectively.
 * \par
 * Typically used along with XMC_LEDTS_ReadInterruptFlag() to figure out which event triggered the interrupt.\n
 *
 * \par<b>Note</b><BR>
 * Calling this API moves interrupt from pending/active state to inactive state. If the interrupt is pulsed, 
 * failing to clear the event bit might cause CPU to immediately re-enter the interrupt service routine(ISR).\n
 *
 * \par<b>Related API's</b><BR>
 * XMC_LEDTS_ReadInterruptFlag().\n
 * 
 */
void XMC_LEDTS_ClearInterruptFlag(XMC_LEDTS_t *const ledts, uint32_t interrupt_mask);

/**
 *
 * @param ledts    Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 * @param pad_num  Pad number. Range refer @ref XMC_LEDTS_NUMBER_TS_INPUT_t enum type.<BR>
 *
 * @return
 *      None.<BR>
 *
 * \par<b>Description</b><br>
 * Sets TSIN[x] (where x corresponds to \a active pad number to be set) field of TSIN[x](x = 0-7) .<br>
 * This is the TSIN[x] pin that is next or currently active in pad turn.<br>
 * Call this API to set the active pad turn.
 * \par
 * Touch sense functionality should be initialized and configured before calling this API.\n
 *
 */
void XMC_LEDTS_SetActivePADNo(XMC_LEDTS_t *const ledts, XMC_LEDTS_NUMBER_TS_INPUT_t pad_num);

/**
 *
 * @param ledts    Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 * @param column   Column number. Range refer @ref XMC_LEDTS_LED_COLUMN_t enum type.<BR>
 * @param pattern  Pattern to be displayed. Range: 0H to FFH.<BR>
 *
 * @return
 *      None.<BR>
 *
 * \par<b>Description</b><br>
 * Sets LINE_x (where x corresponds to \a column number) field of LINEx(x = 0-1) register to \a pattern value.<br>
 * This value is output on LINE_x when LED \a column x is active.<br> Call this API to set desired LED pattern.
 * \par
 * LED functionality should be initialized and configured before calling this API.\n
 *
 */
void XMC_LEDTS_SetLEDLinePattern(XMC_LEDTS_t *const ledts, XMC_LEDTS_LED_COLUMN_t column, const uint8_t pattern);

/**
 *
 * @param ledts       Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 * @param column      Column number. Range refer @ref XMC_LEDTS_LED_COLUMN_t enum type.<BR>
 * @param brightness  LED brightness level. Range: 0H(min brightness) to FFH(max brightness).<BR>
 *
 * @return
 *      None.<BR>
 *
 * \par<b>Description</b><br>
 * Programs CMP_LDx (where x denotes \a column number) field of LDCMPx(x = 0-1)register to the requested \a brightness
 * level.
 * \par
 * The LDCMPx registers hold the COMPARE values for their respective LED columns. These values are used for LED
 * brightness control. Call this API to control brightness level of the LED.
 * \par
 * LED functionality should be initialized and configured before calling this API.\n
 *
 */
void XMC_LEDTS_SetColumnBrightness(XMC_LEDTS_t *const ledts, XMC_LEDTS_LED_COLUMN_t column, const uint8_t brightness);

/**
 *
 * @param ledts        Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 * @param common_size  Requested common oscillation window width. Range: FFH(min) to 00H.(max)<BR>
 *
 * @return
 *      None.<BR>
 *
 * \par<b>Description</b><br>
 * Programs the respective LDCMP1 register bit field CMP_LDA_TSCOM with \a common_size. 
 * \par
 * Call this API to adjust the size of the common oscillation window to increase/decrease the number of recorded 
 * number of oscillation counts for all touch-sense inputs.\n
 *
 */
void XMC_LEDTS_SetCommonOscillationWindow(XMC_LEDTS_t *const ledts, const uint8_t common_size);

/**
 *
 * @param ledts  Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 *
 * @return
 *      uint32_t Previous active LED column number. Range: 0 to 7.<BR>
 *
 * \par<b>Description</b><br>
 * Returns active LED column number in previous time-slice by reading FNCOL bit field of FNCTL register.
 * Call this API to figure out active column during previous time slice.\n
 *
 */
uint32_t XMC_LEDTS_ReadFNCOL(XMC_LEDTS_t *const ledts);

/**
 *
 * @param ledts  Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure<BR>
 * @param count  Number of LED columns to be enabled. Range: 0 to 7.<BR>
 *
 * @return
 *      None.<BR>
 *
 * \par<b>Description</b><br>
 * Sets \a count number of LED columns active by programming NR_LEDCOL bit field of FNCTL register.<BR>
 * \par
 * Call this API to set desired number of LED columns active.\n
 *
 * \par<b>Note</b><BR>
 * NR_LEDCOL bit field can only be modified when LEDTS counter is not running, use XMC_LEDTS_StopCounter()
 * to stop LEDTS module before calling this API.\n
 *  
 * \par<b>Related API's</b><BR>
 * XMC_LEDTS_StopCounter().\n
 */
void XMC_LEDTS_SetNumOfLEDColumns(XMC_LEDTS_t *const ledts, uint8_t count);

/**
 *
 * @param ledts  Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 *
 * @return
 *      uint16_t Shadow touch sense counter value. Range: 0H to FFFFH.<BR>
 *
 * \par<b>Description</b><br>
 * Returns latched touch sense counter value by reading the TSCTRVALR field of TSVAL register.<BR>
 * \par
 * This API is typically called in time frame(TF) event handler to get oscillation count of the touch-sense input
 * active in previous time frame.\n
 *
 * \par<b>Note</b><BR>
 * This is the latched value of the TS-counter(on every extended time frame event).\n
 *  
 * \par<b>Related API's</b><BR>
 * XMC_LEDTS_ReadInterruptFlag().\n
 */
uint16_t XMC_LEDTS_ReadTSVAL(XMC_LEDTS_t *const ledts);

/**
 *
 * @param ledts     Constant pointer to LEDTS module structure. Refer @ref XMC_LEDTS_GLOBAL_t data structure.<BR>
 * @param touchpad  Touch-sense input pad number. Range refer @ref XMC_LEDTS_NUMBER_TS_INPUT_t enum type.<BR>
 * @param size      Requested oscillation window width. Range: 0H(max) to FFH(min).<BR>
 *
 * @return
 *      None.<BR>
 *
 * \par<b>Description</b><br>
 * Sets the size of \a touchpad touch sense oscillation window to \a size.
 * \par
 * This API programs the respective CMP_TSx(where x is \a touchpad number) bit fields of TSCMPx(x = 0-1) register.
 * \a size value determines the size of the pad oscillation window for each pad input lines during their pad turn.
 * \par
 * Call this API to increase/decrease recorded number of oscillation counts for the requested touch-sense input.\n
 *
 */
void XMC_LEDTS_SetOscillationWindow(XMC_LEDTS_t *const ledts, XMC_LEDTS_NUMBER_TS_INPUT_t touchpad, const uint8_t size);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * @}
 */

#endif /* LEDTS0 */

#endif /* XMC_LEDTS_H */
