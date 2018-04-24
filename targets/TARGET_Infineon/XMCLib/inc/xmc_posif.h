/**
 * @file xmc_posif.h
 * @date 2017-06-24
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
 * 2015-02-18:
 *     - Initial version
 *      
 * 2015-02-20:
 *     - Driver description added <BR>
 *
 * 2015-06-19:
 *     - Removed version macros and declaration of GetDriverVersion API <BR> 
 *
 * 2015-07-02:
 *     - Updated XMC_POSIF_QD_GetDirection API
 *
 * 2016-03-09:
 *     - Optimization of write only registers
 *
 * 2017-06-24
 *     - Added posif map connectivity file<br>
 *
 * @endcond 
 *
 */


#ifndef XMC_POSIF_H
#define XMC_POSIF_H

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include <xmc_common.h>

#if defined(POSIF0)
#include <xmc_scu.h>
#include "xmc_posif_map.h"

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup POSIF
 * @brief Position Interface Unit (POSIF) driver for the XMC microcontroller family <br>
 *
 * The POSIF unit is a flexible and powerful component for motor control systems that use
 * rotary encoders or hall sensors as feedback loop. It provides interface for motor position and velocity measurement.
 * POSIF unit works with CCU4 and CCU8 to enable position and velocity measurement and to control PWM outputs using multi channel pattern.<br>
 *
 * Driver is divided in three POSIF functional blocks - Hall Sensor Control (POSIF_HSC), Quadrature Decoder (POSIF_QD) and
 * MultiChannel Mode (POSIF_MCM). <br>
 *
 * POSIF driver features:
 * -#  Configuration structure XMC_POSIF_CONFIG_t and initialization function XMC_POSIF_Init() to configure global settings
 * -# Allows to change the operating mode using XMC_POSIF_SetMode()
 * -# Allows the selection of one of the four inputs (A, B, C or D) using XMC_POSIF_SelectInputSource(). In hall sensor control, inputs are
 * hall0, hall1 and hall2 signals. For quadrature decoder mode, inputs are phase A, phase B and index signals.
 * -#  Hall Sensor Control (APIs prefixed with XMC_POSIF_HSC_) <br>
 * 	-  Configuration structure XMC_POSIF_HSC_CONFIG_t and initialization function XMC_POSIF_HSC_Init()
 * 	-  Update current and expected hall pattern in shadow register using XMC_POSIF_HSC_SetHallPatterns()
 * 	- Allows immediate shadow transfer using XMC_POSIF_HSC_UpdateHallPattern()
 * -#  Quadrature Decoder (APIs prefixed with XMC_POSIF_QD_) <br>
 * 	-  Configuration structure XMC_POSIF_QD_CONFIG_t and initialization function XMC_POSIF_QD_Init()
 * 	-  Get direction of rotation using XMC_POSIF_QD_GetDirection()
 * -#  MultiChannel Mode (APIs prefixed with XMC_POSIF_MCM_) <br>
 *	-  Configuration structure XMC_POSIF_MCM_CONFIG_t and initialization function XMC_POSIF_MCM_Init()
 *	-  Update multichannel pattern in shadow register using XMC_POSIF_MCM_SetMultiChannelPattern()
 *	-  Allows immediate shadow transfer using XMC_POSIF_MCM_UpdateMultiChannelPattern()
 * -# User need to call respective init functions to configure POSIF operating mode. e.g to configure POSIF in hall sensor control with multichannel mode
 * call both XMC_POSIF_HSC_Init() and XMC_POSIF_MCM_Init().
 * -# Allows to enable and disable interrupt sources and assign to service request node using XMC_POSIF_EnableEvent(), XMC_POSIF_DisableEvent() and XMC_POSIF_SetInterruptNode()
 * 
 * @note POSIF is not available on XMC11 and XMC12 devices
 *
 * @{
 */

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#if ((UC_SERIES == XMC45) || (UC_SERIES == XMC44) || (UC_SERIES == XMC47) || (UC_SERIES == XMC48) || (UC_SERIES == XMC14))
#define XMC_POSIF_CHECK_MODULE_PTR(PTR)  ( ((PTR)== POSIF0) || ((PTR)== POSIF1) ) /*< Check for valid module pointer */
#else
#define XMC_POSIF_CHECK_MODULE_PTR(PTR)  ( ((PTR)== POSIF0))  /*< Check for valid module pointer */
#endif

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/
/**
 *  Defines the return status, to verify the POSIF related API calls. Use type @ref XMC_POSIF_STATUS_t for this enum.
 */
typedef enum XMC_POSIF_STATUS
{
  XMC_POSIF_STATUS_OK = 0U,     /**< API fulfills request */
  XMC_POSIF_STATUS_ERROR        /**< API cannot fulfill request */
} XMC_POSIF_STATUS_t;

/**
 * Defines POSIF configurable modes.Use type @ref XMC_POSIF_MODE_t for this enum.
 * The members defines the function selector(FSEL) bitfields of \a PCONF register.
 */
typedef enum XMC_POSIF_MODE
{
  XMC_POSIF_MODE_HALL_SENSOR = 0U, /**< Hall sensor mode */
  XMC_POSIF_MODE_QD              , /**< Quadrature Decoder mode */
  XMC_POSIF_MODE_MCM             , /**< Standalone Multichannel mode */
  XMC_POSIF_MODE_MCM_QD           /**< Quadrature Decoder + Standalone Multichannel mode */
} XMC_POSIF_MODE_t;

/**
 * Defines POSIF configurable input ports.Use type @ref XMC_POSIF_INPUT_PORT_t for this enum.
 * The member defines the respective input selector(INSELX) bitfields of \a PCONF register.
 * It selects, which input is used for the phase or Hall input function (depending on the module is set for 
 * Quadrature Decoder or Hall Sensor Mode). Same enum can be used to configure pattern update signal select by configuring
 * \a PCONF register's \a MSETS bit field.
 */
typedef enum XMC_POSIF_INPUT_PORT
{
  XMC_POSIF_INPUT_PORT_A = 0U, /**< INPUT-A */
  XMC_POSIF_INPUT_PORT_B     , /**< INPUT-B */
  XMC_POSIF_INPUT_PORT_C     , /**< INPUT-C */
  XMC_POSIF_INPUT_PORT_D     , /**< INPUT-D */
  XMC_POSIF_INPUT_PORT_E     , /**< INPUT-E */
  XMC_POSIF_INPUT_PORT_F     , /**< INPUT-F */
  XMC_POSIF_INPUT_PORT_G     , /**< INPUT-G */
  XMC_POSIF_INPUT_PORT_H       /**< INPUT-H */
} XMC_POSIF_INPUT_PORT_t;

/**
 *  Defines active level of an input signal.Use type @ref XMC_POSIF_INPUT_ACTIVE_LEVEL_t for this enum.
 */
typedef enum XMC_POSIF_INPUT_ACTIVE_LEVEL
{
  XMC_POSIF_INPUT_ACTIVE_LEVEL_HIGH  = 0U, /**< Input - Active High */
  XMC_POSIF_INPUT_ACTIVE_LEVEL_LOW         /**< Input - Active Low */
} XMC_POSIF_INPUT_ACTIVE_LEVEL_t;

/**
 * Defines POSIF input debounce filter configuration.POSIF inputs are connected to low pass filter and
 * this enum is used to configure low pass filters cut off frequency.
 * Use type @ref XMC_POSIF_FILTER_t for this enum.
 * The member defines the low pass filter configuration(LPC) bitfield of \a PCONF register.
 */
typedef enum XMC_POSIF_FILTER
{
  XMC_POSIF_FILTER_DISABLED     = 0U, /**< No filtering */
  XMC_POSIF_FILTER_1_CLOCK_CYCLE    , /**< Filter of 1 Clock Cycle */
  XMC_POSIF_FILTER_2_CLOCK_CYCLE    , /**< Filter of 2 Clock Cycles */
  XMC_POSIF_FILTER_4_CLOCK_CYCLE    , /**< Filter of 4 Clock Cycles */
  XMC_POSIF_FILTER_8_CLOCK_CYCLE    , /**< Filter of 8 Clock Cycles */
  XMC_POSIF_FILTER_16_CLOCK_CYCLE   , /**< Filter of 16 Clock Cycles */
  XMC_POSIF_FILTER_32_CLOCK_CYCLE   , /**< Filter of 32 Clock Cycles */
  XMC_POSIF_FILTER_64_CLOCK_CYCLE     /**< Filter of 64 Clock Cycles */
} XMC_POSIF_FILTER_t;

/**
 * Defines POSIF events.Use type @ref XMC_POSIF_IRQ_EVENT_t for this enum.
 * The member defines available event sources.It is used to configure which event to be used for
 * interrupt generation using \a PFLGE register. [ PFLG,SPFLG,RPFLG]
 */
typedef enum XMC_POSIF_IRQ_EVENT
{
  XMC_POSIF_IRQ_EVENT_CHE                  = 0U, /**< Hall Mode : Correct Hall Event */
  XMC_POSIF_IRQ_EVENT_WHE                  = 1U, /**< Hall Mode : Wrong Hall Event */
  XMC_POSIF_IRQ_EVENT_HALL_INPUT           = 2U, /**< Hall Mode : Hall Input update */
  XMC_POSIF_IRQ_EVENT_MCP_SHADOW_TRANSFER  = 4U, /**< Hall Mode + MCM Mode : MC Pattern shadow transfer */
  XMC_POSIF_IRQ_EVENT_INDX                 = 8U, /**< Quadrature Mode : Index event detection */
  XMC_POSIF_IRQ_EVENT_ERR                  = 9U, /**< Quadrature Mode : Quadrature Phase Error */
  XMC_POSIF_IRQ_EVENT_CNT                  = 10U, /**< Quadrature Mode : Quadrature Clock event */
  XMC_POSIF_IRQ_EVENT_DIR                  = 11U, /**< Quadrature Mode : Quadrature Direction change event */
  XMC_POSIF_IRQ_EVENT_PCLK                 = 12U  /**< Quadrature Mode : Quadrature period clock generation event */
} XMC_POSIF_IRQ_EVENT_t;

/**
 * Defines POSIF service request lines.Use type @ref XMC_POSIF_SR_ID_t for this enum.
 * It used to connect POSIF event to required service request line.
 * in \a PFLGE register for interrupt generation.
 */
typedef enum XMC_POSIF_SR_ID
{
  XMC_POSIF_SR_ID_0 = 0U, /**< SR-0 */
  XMC_POSIF_SR_ID_1      /**< SR-1 */
} XMC_POSIF_SR_ID_t;

/**
 * Defines position decoder mode selection.Use type @ref XMC_POSIF_QD_MODE_t for this enum.
 * The member defines configuration for the operation of the quadrature decoder mode.
 * It used to configure \a QDC register.
 */
typedef enum XMC_POSIF_QD_MODE
{
  XMC_POSIF_QD_MODE_QUADRATURE       = 0U, /**< Standard Quadrature Mode */
  XMC_POSIF_QD_MODE_DIRECTION_COUNT       /**< Direction Count Mode */
} XMC_POSIF_QD_MODE_t;

/**
 * Defines motor rotation direction.Use type @ref XMC_POSIF_QD_DIR_t for this enum.
 * The member defines the direction in quadrature mode.
 */
typedef enum XMC_POSIF_QD_DIR
{
  XMC_POSIF_QD_DIR_COUNTERCLOCKWISE  = 0U, /**< Counter Clockwise */
  XMC_POSIF_QD_DIR_CLOCKWISE              /**< Clockwise */
} XMC_POSIF_QD_DIR_t;

/**
 * Defines frequency of index signal generation.Use type @ref XMC_POSIF_QD_INDEX_GENERATION_t for this enum.
 * Member represents available configuration for index marker generation using \a ICM  bit field in \a QDC register.
 */
typedef enum XMC_POSIF_QD_INDEX_GENERATION
{
  XMC_POSIF_QD_INDEX_GENERATION_NEVER  = 0U, /**< Never generate the index marker signal */
  XMC_POSIF_QD_INDEX_GENERATION_ONCE      , /**< Generate only once after the first revolution */
  XMC_POSIF_QD_INDEX_GENERATION_ALWAYS      /**< Index marker generated upon every revolution */
} XMC_POSIF_QD_INDEX_GENERATION_t;

/**
 *  Defines trigger edge in hall sensor mode.Use type @ref XMC_POSIF_HSC_TRIGGER_EDGE_t for this enum.
 *  It can be used to configure \a PCONF register's \a SPES and \a MSES bit fields.
 */
typedef enum XMC_POSIF_HSC_TRIGGER_EDGE
{
  XMC_POSIF_HSC_TRIGGER_EDGE_RISING  = 0U, /**< Rising edge */
  XMC_POSIF_HSC_TRIGGER_EDGE_FALLING      /**< Falling edge */
} XMC_POSIF_HSC_TRIGGER_EDGE_t;

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
 *
 * Defines POSIF peripheral register structure.Use type @ref XMC_POSIF_t for this data structure.
 */
typedef POSIF_GLOBAL_TypeDef XMC_POSIF_t;

/**
 *  Defines POSIF quadrature decoder initialization data structure.
 *  Use type @ref XMC_POSIF_QD_CONFIG_t for this data structure.
 *  It used to configure Quadrature mode using \a QDC register.
 */
typedef struct XMC_POSIF_QD_CONFIG
{
  XMC_POSIF_QD_MODE_t mode;      /**< Operational Mode of the quadrature encoder and decoder */
  union
  {
    struct
    {
      uint32_t phase_a: 1;       /**< Phase-A active level configuration */
      uint32_t phase_b: 1;       /**< Phase-B active level configuration */
      uint32_t phase_leader: 1;  /**< Which of the two phase signals[Phase A or Phase B] leads the other? */
      uint32_t : 1;
      uint32_t index: 2;        /**< Index signal generation control. Use @ref XMC_POSIF_QD_INDEX_GENERATION_t to configure this field.*/
      uint32_t : 26;
    };
    uint32_t qdc;
  };
} XMC_POSIF_QD_CONFIG_t;

/**
 * Defines POSIF hall sensor control initialization data structure.
 * Use type @ref XMC_POSIF_HSC_CONFIG_t for this data structure.
 * It used to initialize hall sensor mode configuration using \a PCONF register.
 */
typedef struct XMC_POSIF_HSC_CONFIG
{
  union
  {
    struct
    {
      uint32_t : 4;
      uint32_t disable_idle_signal: 1;   /**< Should idle signal be disabled upon wrong hall event? */
      uint32_t : 11;
      uint32_t sampling_trigger: 1;      /**< Of HSDA and HSDB, which one is to be used to trigger POSIF to sample hall pattern? */
      uint32_t sampling_trigger_edge: 1; /**< Which edge of the sampling trigger signal is to be considered? */
	  uint32_t : 6;
      uint32_t external_error_port: 2;    /**< Of the 4 external error ports, which one is to be considered? */
      uint32_t external_error_enable: 1;  /**< Should external errors lead to Wrong Hall event? */
      uint32_t external_error_level: 1;   /**< What should be the active level of external error signal? */
      uint32_t: 4;
    };
    uint32_t hall_config;
  };
} XMC_POSIF_HSC_CONFIG_t;

/**
 * Defines POSIF multi-channel mode initialization data structure.
 * Use type @ref XMC_POSIF_MCM_CONFIG_t for this data structure.
 * It used to initialize multi channel mode configuration using \a PCONF register.
 */
typedef struct XMC_POSIF_MCM_CONFIG
{
  union
  {
    struct
    {
      uint32_t : 5;
      uint32_t pattern_sw_update: 1; /**< should multi channel pattern updated by SW ? */
      uint32_t : 12;
	  uint32_t pattern_update_trigger: 3; /**< Of the 8 update triggers, which one is to be considered? */
      uint32_t pattern_trigger_edge: 1;  /**< Which edge of the pattern update trigger is to be considered? */
      uint32_t pwm_sync: 2;         /**< Of the 4 pwm sync inputs, which one is to be considered? */
      uint32_t : 8;
    };
    uint32_t mcm_config;
  };
}XMC_POSIF_MCM_CONFIG_t;

/**
 *  Defines POSIF module initialization data structure.
 *  Use type @ref XMC_POSIF_CONFIG_t for this data structure.
 *  It is used to initialize POSIF module using \a PCONF register.
 */
typedef struct XMC_POSIF_CONFIG
{
  union
  {
    struct
    {
      uint32_t mode: 2;    /**< POSIF Operational mode. Use @ref XMC_POSIF_MODE_t to configure */
      uint32_t :6;
      uint32_t input0: 2; /**< Choice of input for Input-1 */
      uint32_t input1: 2; /**< Choice of input for Input-2 */
      uint32_t input2: 2; /**< Choice of input for Input-3 */
      uint32_t :14;
      uint32_t filter: 3; /**< Input filter configuration */
      uint32_t: 1;
    };
    uint32_t pconf;
  };
} XMC_POSIF_CONFIG_t;

/*Anonymous structure/union guard end*/
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__TASKING__)
  #pragma warning restore
#endif

/*********************************************************************************************************************
 * API Prototypes
 ********************************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param peripheral Pointer to an instance of POSIF module of type @ref XMC_POSIF_t
 * @retval None
 *
 * \par<b>Description</b><br>
 * De-asserts the POSIF module from reset and enables the clock.\n
 * Configures \a PRCLR0 register's \a POSIF0RS or \a POSIF1RS bit field depends upon \a peripheral.
 * If running on other than XMC45 device then it will ungate the peripheral clock.
 *
 * \par<b>Note</b><br>
 * This is the first API which application must invoke to configure POSIF.
 * It is internally called by XMC_POSIF_Init().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_Disable(),XMC_POSIF_Init() \n\n\n
 */
void XMC_POSIF_Enable(XMC_POSIF_t *const peripheral);

/**
 * @param peripheral Pointer to an instance of POSIF module of type @ref XMC_POSIF_t
 * @retval None
 *
 * \par<b>Description</b><br>
 * Asserts the POSIF module into reset and disables the clock.\n
 * If running on other than XMC45 device then in addition it will gate the peripheral clock.
 * Configures \a PRCLR0 register's \a POSIF0RS or \a POSIF1RS bitfield depends upon \a peripheral.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_Enable()\n\n\n
 */
void XMC_POSIF_Disable(XMC_POSIF_t *const peripheral);

/**
 * @param peripheral Pointer to an instance of POSIF module of type @ref XMC_POSIF_t
 * @param config Pointer to POSIF configuration data(operation mode,input selection and filter configuration)
 * @retval None
 *
 * \par<b>Description</b><br>
 * Initialize POSIF module with \a config.\n
 * Configures POSIF global registers.This is the first API which application must invoke to configure POSIF.
 * It sets up parameters common to all the POSIF modes - hall sensor,quadrature decoder and multi-channel modes of operation.
 * Configures \a PCONF register with mode of operation,input selection and filter configuration.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_HSC_Init(),XMC_POSIF_QD_Init(),XMC_POSIF_MCM_Init() \n\n\n
*/
void XMC_POSIF_Init(XMC_POSIF_t *const peripheral, const XMC_POSIF_CONFIG_t * const config);

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @param config Pointer to hall sensor control initialization data of type @ref XMC_POSIF_HSC_CONFIG_t
 * @retval XMC_POSIF_STATUS_t  Returns @ref XMC_POSIF_STATUS_OK if configured in Hall Sensor Mode
 *                             else return @ref XMC_POSIF_STATUS_ERROR.
 *
 * \par<b>Description</b><br>
 * Initializes hall sensor control mode.\n
 * Configures \a PCONF register with which POSIF input trigger to be used for
 * sampling hall pattern.Configures \a PCONF register for idle signal generation for wrong hall event.
 *
 * \par<b>Note</b><br>
 * It is necessary to have called XMC_POSIF_Init first with Hall sensor mode before invocation of this API.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_Init() \n\n\n
 */
XMC_POSIF_STATUS_t XMC_POSIF_HSC_Init(XMC_POSIF_t *const peripheral, const XMC_POSIF_HSC_CONFIG_t * const config);

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @param config Pointer to quadrature decoder initialization data
 * @retval XMC_POSIF_STATUS_t Returns quadrature mode initialization status of type @ref XMC_POSIF_STATUS_t
 *
 * \par<b>Description</b><br>
 * Initializes quadrature decoder control mode.\n
 * Configures \a PCONF register with quadrature mode using @ref XMC_POSIF_QD_MODE_t data structure.
 * Initializes \a QDC register with quadrature mode configuration using @ref XMC_POSIF_QD_CONFIG_t structure.
 *
 * \par<b>Note</b><br>
 * It is necessary to have called XMC_POSIF_Init first with Quadrature decoder mode before invocation of this API.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_Init() \n\n\n
 */
XMC_POSIF_STATUS_t XMC_POSIF_QD_Init(XMC_POSIF_t *const peripheral, const XMC_POSIF_QD_CONFIG_t * const config);

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @param config Pointer to quadrature decoder initialization data
 * @retval XMC_POSIF_STATUS_t Returns multi channel pattern initialization status of type @ref XMC_POSIF_STATUS_t
 *
 * \par<b>Description</b><br>
 * Initializes multi channel mode in Hall mode, standalone multi-channel mode and quadrature with multi-channel mode.\n
 * Configures \a PCONF register with multi channel mode using @ref XMC_POSIF_MCM_CONFIG_t data structure.
 *
 * \par<b>Note</b><br>
 * It is necessary to call XMC_POSIF_Init first before invocation of this API.
 * For XMC_POSIF_MODE_HALL_SENSOR, it is necessary to have called XMC_POSIF_HSC_Init before invocation of this API.
 * For XMC_POSIF_MODE_MCM_QD, it is necessary to have called XMC_POSIF_QD_Init before invocation of this API.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_Init(),XMC_POSIF_HSC_Init(),XMC_POSIF_QD_Init() \n\n\n
 */
XMC_POSIF_STATUS_t XMC_POSIF_MCM_Init(XMC_POSIF_t *const peripheral, const XMC_POSIF_MCM_CONFIG_t * const config);

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @param mode POSIF operating mode of type @ref XMC_POSIF_MODE_t
 * @retval None
 *
 * \par<b>Description</b><br>
 * Configures POSIF module for \a mode.\n
 * Configures \a PCONF register's a\ FSEL bitfield with \a mode.
 * Refer @ref XMC_POSIF_MODE_t for available options.
 *
 * \par<b>Note</b><br>
 * POSIF module should be in stopped state while changing the operating mode.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_Stop() \n\n\n
 */
__STATIC_INLINE void XMC_POSIF_SetMode(XMC_POSIF_t *const peripheral, const XMC_POSIF_MODE_t mode)
{
  peripheral->PCONF = ((peripheral->PCONF & ~(uint32_t)(POSIF_PCONF_FSEL_Msk)) |
                     (((uint32_t)mode << POSIF_PCONF_FSEL_Pos) & (uint32_t)POSIF_PCONF_FSEL_Msk));
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @param input0 Choice of input for input 0 [0-3]
 * @param input1 Choice of input for input 1 [0-3]
 * @param input2 Choice of input for input 2 [0-3]
 * @retval None
 *
 * \par<b>Description</b><br>
 * Configures which input to be connected to POSIF module. \n
 * Configures \a PCONF register's INSEL0,INSEL1,INSEL2 bit fields with source for the input connection for \a input0
 * \a input1, \a input2 respectively.
 *
 * \par<b>Note</b><br>
 * Configures which input is used for the Phase X or Hall input X function depending upon the module is
 * set for Quadrature Decoder or Hall Sensor Mode.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_Init() \n\n\n
 */
void XMC_POSIF_SelectInputSource(XMC_POSIF_t *const peripheral, const XMC_POSIF_INPUT_PORT_t input0,
    const XMC_POSIF_INPUT_PORT_t input1, const XMC_POSIF_INPUT_PORT_t input2);


/**
 * @param peripheral Pointer to an instance of POSIF module
 * @retval None
 *
 * \par<b>Description</b><br>
 * Starts POSIF \a peripheral functional state machine.\n
 * Starts POSIF state machine for \a peripheral.Configures \a PRUNS register's \a SRB bit field with 1.
 *
 * \par<b>Note</b><br>
 * Global properties of POSIF along with mode specific properties should have been initialized before starting of POSIF
 * FSM.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_Stop(),XMC_POSIF_IsRunning() \n\n\n
 */
__STATIC_INLINE void XMC_POSIF_Start(XMC_POSIF_t *const peripheral)
{
  peripheral->PRUNS = (uint32_t)POSIF_PRUNS_SRB_Msk;
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @retval None
 *
 * \par<b>Description</b><br>
 * Stops POSIF \a peripheral functional state machine.\n
 * Stop POSIF functional state machine and clears current internal status of the \a peripheral.
 * Configures \a PRUNC register's \a CRB bit field with 1.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_Start(),XMC_POSIF_IsRunning() \n\n\n
 */
__STATIC_INLINE void XMC_POSIF_Stop(XMC_POSIF_t *const peripheral)
{
  peripheral->PRUNC = (uint32_t)(POSIF_PRUNC_CRB_Msk | POSIF_PRUNC_CSM_Msk);
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @retval bool Returns false: IDLE, true:RUNNING
 *
 * \par<b>Description</b><br>
 * Returns the status of POSIF module - Running or IDLE.\n
 * Retrieves the status from \a PRUN register's \a SRB bit.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_Start(),XMC_POSIF_Stop() \n\n\n
 */
__STATIC_INLINE bool XMC_POSIF_IsRunning(XMC_POSIF_t *const peripheral)
{
  return ((bool)peripheral->PRUN);
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @retval uint8_t Returns last sampled hall sensor pattern. Range : [0-7]
 *
 * \par<b>Description</b><br>
 * Returns last sampled hall sensor pattern of \a peripheral.\n
 * Retrieves the last sampled hall sensor pattern from \a PDBG register's \a HSP bit field of \a peripheral.
 * Applications can at any point in time retrieve the last sampled hall sensor pattern by invoking this API.
 *
 * \par<b>Note</b><br>
 * This is applicable only to the hall sensor mode of operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_HSC_GetCurrentPattern(),XMC_POSIF_HSC_GetExpectedPattern() \n\n\n
 */
__STATIC_INLINE uint8_t XMC_POSIF_HSC_GetLastSampledPattern(XMC_POSIF_t *const peripheral)
{
  return ((uint8_t)((peripheral->PDBG) & POSIF_PDBG_HSP_Msk) >> POSIF_PDBG_HSP_Pos);
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @retval uint8_t Returns current hall sensor pattern. Range : [0-7]
 *
 * \par<b>Description</b><br>
 * Returns current sampled hall sensor pattern of \a peripheral.\n
 * Retrieves the current hall sensor pattern from \a HALP register's \a HCP bit field of \a peripheral.
 * Applications can at any point in time retrieve the current hall sensor pattern by invoking this API.
 *
 * \par<b>Note</b><br>
 * This is applicable only to the hall sensor mode of operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_HSC_GetLastSampledPattern(),XMC_POSIF_HSC_GetExpectedPattern() \n\n\n
 */
__STATIC_INLINE uint8_t XMC_POSIF_HSC_GetCurrentPattern(XMC_POSIF_t *const peripheral)
{
  return ((uint8_t)((peripheral->HALP & POSIF_HALP_HCP_Msk) >> POSIF_HALP_HCP_Pos));
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @retval uint8_t Returns expected hall sensor pattern. Range : [0-7]
 *
 * \par<b>Description</b><br>
 * Returns expected hall sensor pattern of \a peripheral.\n
 * Retrieves the expected hall sensor pattern from \a HALP register's \a HEP bit field of \a peripheral.
 * Applications can at any point in time retrieve the expected hall sensor pattern by invoking this API.
 *
 * \par<b>Note</b><br>
 * This is applicable only to the hall sensor mode of operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_HSC_GetLastSampledPattern(),XMC_POSIF_HSC_GetCurrentPattern() \n\n\n
 */
__STATIC_INLINE uint8_t XMC_POSIF_HSC_GetExpectedPattern(XMC_POSIF_t *const peripheral)
{
  return ((uint8_t)((peripheral->HALP & POSIF_HALP_HEP_Msk) >> POSIF_HALP_HEP_Pos));
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @param pattern The hall sensor pattern to be programmed into current pattern [0-7]
 * @retval None
 *
 * \par<b>Description</b><br>
 * Configures current Hall sensor \a pattern of \a peripheral.\n
 * Configures the Current hall sensor pattern on \a HALPS shadow register's \a HCPS bit field of \a peripheral.
 * Applications can set at any point in time program the current hall sensor pattern by invoking this API.
 *
 * \par<b>Note</b><br>
 * This is applicable only to the hall sensor mode of operation. It may be noted that the pattern is merely
 * written to the shadow register. Transfer from the shadow register is based on a hardware transfer trigger
 * or software trigger through API @ref XMC_POSIF_HSC_UpdateHallPattern().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_HSC_GetCurrentPattern(),XMC_POSIF_HSC_SetExpectedPattern() \n\n\n
 */
__STATIC_INLINE void XMC_POSIF_HSC_SetCurrentPattern(XMC_POSIF_t *const peripheral, const uint8_t pattern)
{
  peripheral->HALPS = ((peripheral->HALPS & ~(uint32_t)(POSIF_HALPS_HCPS_Msk)) |
                 (((uint32_t)pattern << POSIF_HALPS_HCPS_Pos) & (uint32_t)POSIF_HALPS_HCPS_Msk));
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @param pattern The hall sensor pattern to be programmed into expected pattern [0-7]
 * @retval None
 *
 * \par<b>Description</b><br>
 * Configures the expected hall sensor \a pattern of \a peripheral.\n
 * Applications can set at any point in time program the hall sensor expected patterns by invoking this API.
 * Configures the expected hall sensor pattern on \a HALPS shadow register's \a HEPS bit field of \a peripheral.
 *
 * \par<b>Note</b><br>
 * This is applicable only to the hall sensor mode of operation.It may be noted that the pattern is merely
 * written to the shadow register. Transfer from the shadow register is based on a hardware transfer trigger
 * or software trigger through API @ref XMC_POSIF_HSC_UpdateHallPattern().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_HSC_GetExpectedPattern() \n\n\n
 */
__STATIC_INLINE void XMC_POSIF_HSC_SetExpectedPattern(XMC_POSIF_t *const peripheral, const uint8_t pattern)
{
  peripheral->HALPS = ((peripheral->HALPS & ~(uint32_t)(POSIF_HALPS_HEPS_Msk)) |
                 (((uint32_t)pattern << POSIF_HALPS_HEPS_Pos) & (uint32_t)POSIF_HALPS_HEPS_Msk));
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @param pattern_mask The hall sensor pattern mask [0-63] Format of mask: (expected_pattern << 3) | (current_pattern)
 * @retval None
 *
 * \par<b>Description</b><br>
 * Configures current and expected hall pattern of \a peripheral. \n
 * Configures \a HALPS register with the Current and Expected hall sensor patterns in one operation.
 * Applications can at any point in time program the current and expected hall sensor pattern by invoking this API.
 *
 * \par<b>Note</b><br>
 * This is applicable only to the hall sensor mode of operation.  It may be noted that the pattern is merely
 * written to the shadow register. Transfer from the shadow register is based on a hardware transfer trigger
 * or software trigger through API @ref XMC_POSIF_HSC_UpdateHallPattern().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_HSC_SetExpectedPattern(),XMC_POSIF_HSC_SetCurrentPattern() \n\n\n
 */
__STATIC_INLINE void XMC_POSIF_HSC_SetHallPatterns(XMC_POSIF_t *const peripheral, const uint8_t pattern_mask)
{
  peripheral->HALPS = (uint32_t)(pattern_mask & (POSIF_HALPS_HCPS_Msk | POSIF_HALPS_HEPS_Msk));
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @retval None
 *
 * \par<b>Description</b><br>
 * Manually performs shadow transfer of hall sensor patterns.\n
 * Configures \a MCMS register's \a STHR bit field with 1.
 * Setting this bit to 1 leads to an immediate update of the fields \a HALP.HCP(Current pattern) and \a HALP.HEP(Expected pattern).
 *
 * \par<b>Note</b><br>
 * The transfer of hall sensor pattern shadow registers content to the sensor pattern register happens under two
 * conditions. A hardware trigger starts the shadow transfer. Alternatively, the shadow transfer can be initiated
 * by application software by means of invocation of this API.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_HSC_SetHallPatterns() \n\n\n
 */
__STATIC_INLINE void XMC_POSIF_HSC_UpdateHallPattern(XMC_POSIF_t *const peripheral)
{
  peripheral->MCMS = (uint32_t)POSIF_MCMS_STHR_Msk;
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @param pattern The 16b multi-channel pattern [0-65535]
 * @retval None
 *
 * \par<b>Description</b><br>
 * Configures \a MCSM register with Multi-Channel Pattern.\n
 * This 16b multi-channel pattern which controls the 16 outputs of all slices of a CCU8 module.
 * Transfer from the shadow register is based on a hardware transfer trigger
 * or software trigger through API @ref XMC_POSIF_MCM_UpdateMultiChannelPattern().
 * Every time that a Multi-Channel pattern transfer is triggered, this value is passed into the field \a MCM.MCMP of \a peripheral
 *
 * \par<b>Note</b><br>
 * It may be noted that the pattern is merely written to the shadow register.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_MCM_GetMultiChannelPattern() \n\n\n
 */
__STATIC_INLINE void XMC_POSIF_MCM_SetMultiChannelPattern(XMC_POSIF_t *const peripheral, const uint16_t pattern)
{
  peripheral->MCSM = pattern;
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @retval uint16_t Returns configured multi channel pattern
 *
 * \par<b>Description</b><br>
 * Returns configured multi channel pattern of \a peripheral. \n
 * Retrieves the Multi-Channel Pattern from \a MCM register's MCMP bit field of \a peripheral
 * Applications can at any point in time retrieve the multi-channel pattern by invoking this API.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_MCM_SetMultiChannelPattern() \n\n\n
 */
__STATIC_INLINE uint16_t XMC_POSIF_MCM_GetMultiChannelPattern(XMC_POSIF_t *const peripheral)
{
  return ((uint16_t)(peripheral->MCM & (uint32_t)POSIF_MCM_MCMP_Msk));
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @retval uint16_t Returns configured multi channel pattern present in shadow transfer register
 *
 * \par<b>Description</b><br>
 * Returns configured multi channel pattern in shadow register of \a peripheral. \n
 * Retrieves the Multi-Channel Pattern from \a MCSM shadow register's \a MCMPS bit field.
 * Applications can at any point in time retrieve the multi-channel pattern by invoking this API.
 *
 * It can be used when MCM is enabled.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_MCM_SetMultiChannelPattern() \n\n\n
 */
__STATIC_INLINE uint16_t XMC_POSIF_MCM_GetShadowMultiChannelPattern(XMC_POSIF_t *const peripheral)
{
  return ((uint16_t)(peripheral->MCSM & (uint32_t)POSIF_MCSM_MCMPS_Msk));
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @retval None
 *
 * \par<b>Description</b><br>
 * Performs shadow transfer of the Multi-Channel Pattern register by configuring \a MCMS register's \a STMR bit field.
 *
 * \par<b>Note</b><br>
 * Transfer multi-channel pattern shadow registers content to the actual pattern register of \a peripheral. \n
 * The transfer of multi-channel pattern shadow registers content to the actual pattern register happens under two
 * conditions. A hardware trigger starts the shadow transfer. Alternatively, the shadow transfer can be initiated
 * by application software by means of invocation of this API.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_MCM_SetMultiChannelPattern() \n\n\n
 */
__STATIC_INLINE void XMC_POSIF_MCM_UpdateMultiChannelPattern(XMC_POSIF_t *const peripheral)
{
  peripheral->MCMS |= (uint32_t)POSIF_MCMS_STMR_Msk;
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @retval None
 *
 * \par<b>Description</b><br>
 * Enables update of the Multi-Channel Pattern by software in standalone multi-channel mode.\n
 * Enabling update of multi-channel pattern happens under two conditions. A hardware trigger enables this update.
 * Alternatively, this can be enabled by software by means of invocation of this API.
 *
 * \par<b>Note</b><br>
 * The update is not done immediately due to the fact that the trigger that synchronizes the update with the PWM is
 * still needed.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_MCM_UpdateMultiChannelPattern() \n\n\n
 */
__STATIC_INLINE void XMC_POSIF_MCM_EnableMultiChannelPatternUpdate(XMC_POSIF_t *const peripheral)
{
  peripheral->MCMS |= (uint32_t)POSIF_MCMS_MNPS_Msk;
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @retval XMC_POSIF_QD_DIR_t Return direction of revolution of the motor of type @ref XMC_POSIF_QD_DIR_t
 *
 * \par<b>Description</b><br>
 * Returns the direction of revolution of the motor.\n
 * Retrieves direction from \a QDC register's \a DVAL bit field in quadrature mode.
 * Applications can at any point in time retrieve the direction of rotation by invoking this API.
 *
 * \par<b>Note</b><br>
 * This is applicable only to the quadrature decoder mode of operation.
 *
 */
__STATIC_INLINE XMC_POSIF_QD_DIR_t XMC_POSIF_QD_GetDirection(XMC_POSIF_t *const peripheral)
{
  return ((XMC_POSIF_QD_DIR_t)((peripheral->QDC & POSIF_QDC_DVAL_Msk) >> POSIF_QDC_DVAL_Pos));
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @retval uint8_t Returns current state of quadrature decoder[Phase B,Phase A]
 *
 * \par<b>Description</b><br>
 * Returns the current state of phase signals in quadrature decoder mode of \a peripheral. \n
 * Retrieves current state of the quadrature decoder from \a PDBG register's \a QCSV bit fields.
 * Applications can at any point in time retrieve the current state of Phase A and Phase B signals
 * by invoking this API.
 *
 * \par<b>Note</b><br>
 * This is applicable only to the quadrature decoder mode of operation.
 *
 * \par<b>Related APIs:</b><BR>
 *  XMC_POSIF_QD_GetPreviousState() \n\n\n
 */
__STATIC_INLINE uint8_t XMC_POSIF_QD_GetCurrentState(XMC_POSIF_t *const peripheral)
{
  return ((uint8_t)((peripheral->PDBG & POSIF_PDBG_QCSV_Msk) >> POSIF_PDBG_QCSV_Pos));
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @retval uint8_t Returns previous state of quadrature decoder[Phase B,Phase A]
 *
 * \par<b>Description</b><br>
 * Returns the previous state of phase signals in quadrature decoder mode of \a peripheral. \n
 * Retrieves previous state of the quadrature decoder from \a PDBG register's \a QPSV bit fields.
 * Applications can at any point in time retrieve the previous state of Phase A and Phase B signals
 * by invoking this API.
 *
 * \par<b>Note</b><br>
 * This is applicable only to the quadrature decoder mode of operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_QD_GetCurrentState() \n\n\n
 */
__STATIC_INLINE uint8_t XMC_POSIF_QD_GetPreviousState(XMC_POSIF_t *const peripheral)
{
  return ((uint8_t)((peripheral->PDBG & POSIF_PDBG_QPSV_Msk) >> POSIF_PDBG_QPSV_Pos));
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @retval uint8_t Returns the index value.[1 - New rotation started, 0 - In-between]
 *
 * \par<b>Description</b><br>
 * Returns the current index value in quadrature decoder mode of \a peripheral. \n
 * Retrieves current index signal value of the quadrature decoder from \a PDBG register's \a IVAL bit field.
 * Applications can at any point in time retrieve the current index signal value of the quadrature decoder by
 * invoking this API.
 *
 * \par<b>Note</b><br>
 * This is applicable only to the quadrature decoder mode of operation.
 *
 */
__STATIC_INLINE uint8_t XMC_POSIF_QD_GetCurrentIndexValue(XMC_POSIF_t *const peripheral)
{
  return ((uint8_t)((peripheral->PDBG & POSIF_PDBG_IVAL_Msk) >> POSIF_PDBG_IVAL_Pos));
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @param event Event to be enabled
 * @retval None
 *
 * \par<b>Description</b><br>
 * Enables \a event generation of \a peripheral. \n
 * Enables an IRQ generation capable \a event by configuring 1 to \a PFLGE register's \a event bit field.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_DisableEvent() \n\n\n
 */
__STATIC_INLINE void XMC_POSIF_EnableEvent(XMC_POSIF_t *const peripheral, const XMC_POSIF_IRQ_EVENT_t event)
{
  peripheral->PFLGE |= (uint32_t)1 << (uint8_t)event;
}

/**
 * @brief Disables an IRQ generation capable event
 * @param peripheral Pointer to an instance of POSIF module
 * @param event Event to be disabled
 * @retval None
 *
 * \par<b>Description</b><br>
 * Disables \a event generation of \a peripheral.\n
 * Disables an IRQ generation capable \a event by configuring 0 to \a PFLGE register's \a event bit field.
 *
 *  \par<b>Related APIs:</b><BR>
 *  XMC_POSIF_EnableEvent() \n\n\n
 */
__STATIC_INLINE void XMC_POSIF_DisableEvent(XMC_POSIF_t *const peripheral, const XMC_POSIF_IRQ_EVENT_t event)
{
  peripheral->PFLGE &= ~((uint32_t)1 << (uint8_t)event);
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @param event Event to be manually asserted
 * @retval None
 *
 * \par<b>Description</b><br>
 *  Manually generates \a event of \a peripheral. \n
 *  Manually asserts an IRQ generation capable event by configuring 1 to \a SPFLG register's \a event bit field.
 *
 *  \par<b>Related APIs:</b><BR>
 *  XMC_POSIF_ClearEvent() \n\n\n
 */
__STATIC_INLINE void XMC_POSIF_SetEvent(XMC_POSIF_t *const peripheral, const XMC_POSIF_IRQ_EVENT_t event)
{
  peripheral->SPFLG = (uint32_t)1 << (uint8_t)event;
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @param event Event to be acknowledged
 * @retval None
 *
 * \par<b>Description</b><br>
 * Clears \a event by acknowledgment of \a peripheral. \n
 * Acknowledges an IRQ event by configuring 1 to \a RPFLG register's \a event bit field.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_SetEvent() \n\n\n
 */
__STATIC_INLINE void XMC_POSIF_ClearEvent(XMC_POSIF_t *const peripheral, const XMC_POSIF_IRQ_EVENT_t event)
{
  peripheral->RPFLG = (uint32_t)1 << (uint8_t)event;
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @param event Event ID to be checked for status
 * @retval uint8_t Returns event status
 *
 * \par<b>Description</b><br>
 * Returns \a event status of \a peripheral. \n
 * Determines if IRQ event is asserted by retrieving data from \a PFLG register's \a event bit field.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_POSIF_SetEvent(),XMC_POSIF_ClearEvent() \n\n\n
 */
__STATIC_INLINE uint8_t XMC_POSIF_GetEventStatus(XMC_POSIF_t *const peripheral, const XMC_POSIF_IRQ_EVENT_t event)
{
  return ((uint8_t)((peripheral->PFLG >> (uint8_t)event) & 1U));
}

/**
 * @param peripheral Pointer to an instance of POSIF module
 * @param event Event to be acknowledged of type @ref XMC_POSIF_IRQ_EVENT_t
 * @param sr Service request line of type @ref XMC_POSIF_SR_ID_t
 * @retval None
 *
 * \par<b>Description</b><br>
 * Configures \a event to generate \a sr (service request) of \a peripheral. \n
 * Binds an IRQ event to a service request line by configuring \a PFLGE register's \a event bit field.
 */
void XMC_POSIF_SetInterruptNode(XMC_POSIF_t *const peripheral, const XMC_POSIF_IRQ_EVENT_t event, const XMC_POSIF_SR_ID_t sr);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * @}
 */

#endif /* #if defined(POSIF0) */

#endif /* XMC_POSIF_H */
