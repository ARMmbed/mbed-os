/**
 * @file xmc_eru.h
 * @date 2016-03-10
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
 *     - Initial <br>
 *     - Documentation updates <br>
 *
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API <br>
 * 
 * 2015-10-07:
 *     - Doc update for XMC_ERU_ETL_CONFIG_t field  <br>
 *
 * 2016-03-10:
 *     - XMC_ERU_ETL_GetEdgeDetection() API is added to get the configured edge for event generation. <br>
 *
 * @endcond
 */

#ifndef XMC_ERU_H
#define XMC_ERU_H

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/

#include "xmc_common.h"

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup ERU
 * @brief Event Request Unit (ERU) driver for the XMC microcontroller family.
 *
 * The Event Request Unit (ERU) is a versatile multiple input event detection and processing unit.
 * The ERU module can be used to expand the P-to-P connections of the device: ports-to-peripherals, 
 * peripherals-to-peripherals and ports-to-ports. It also offers configurable logic, that allows the generation of 
 * triggers, pattern detection and real-time signal monitoring.
 *
 * @image html "eru_overview.png"
 *
 * The driver is divided into two sections:
 * \par Event trigger logic (ERU_ETL):
 * This section of the LLD provides the configuration structure XMC_ERU_ETL_CONFIG_t and the initialization function 
 * XMC_ERU_ETL_Init().\n
 * It can be used to:
 * -# Select one out of two inputs (A and B). For each of these two inputs, a vector of 4 possible signals is available.
 *   (XMC_ERU_ETL_SetSource())
 * -# Logically combine the two input signals to a common trigger. (XMC_ERU_ETL_SetSource())
 * -# Define the transition (edge selection, or by software) that leads to a trigger event and can also store this status.
 *   (XMC_ERU_ETL_SetEdgeDetection() and XMC_ERU_ETL_SetStatusFlag())
 * -# Distribute the events and status flags to the output channels. (XMC_ERU_ETL_EnableOutputTrigger())
 *
 * \par Output gating unit (ERU_OGU):
 * This section of the LLD provides the provides the configuration structure XMC_ERU_OGU_CONFIG_t and the initialization
 * function XMC_ERU_ETL_OGU_Init().
 * It can be used to:
 * -# Combine the trigger events and status information and gates the output depending on a gating signal. 
 *   (XMC_ERU_OGU_EnablePatternDetection(), XMC_ERU_OGU_DisablePeripheralTrigger(), XMC_ERU_OGU_SetServiceRequestMode())
 * @{
 */

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

#if defined(ERU0)
#define XMC_ERU0 ((XMC_ERU_t *) ERU0_BASE) /**< ERU module 0 */
#endif

#if defined(ERU1)
#define XMC_ERU1 ((XMC_ERU_t *) ERU1_BASE) /**< ERU module 1, only available in XMC4 family */
#endif

#if UC_FAMILY == XMC1
 #include "xmc1_eru_map.h"
#endif

#if UC_FAMILY == XMC4
 #include "xmc4_eru_map.h"
#endif

#if defined(XMC_ERU0) && defined(XMC_ERU1)
#define XMC_ERU_CHECK_MODULE_PTR(PTR)  (((PTR)== XMC_ERU0) | ((PTR)== XMC_ERU1))
#elif defined(XMC_ERU0)
#define XMC_ERU_CHECK_MODULE_PTR(PTR)  (((PTR)== XMC_ERU0))
#elif defined(XMC_ERU1)
#define XMC_ERU_CHECK_MODULE_PTR(PTR)  (((PTR)== XMC_ERU0))
#endif

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/

/**
 * Defines input signal for path A of ERSx(Event request source, x = [0 to 3]) unit.
 * @image html "eru_input_a.png" "ETLx Input A selection"
*/
typedef enum XMC_ERU_ETL_INPUT_A
{
  XMC_ERU_ETL_INPUT_A0 = 0x0U, /**< input A0 is selected */
  XMC_ERU_ETL_INPUT_A1 = 0x1U, /**< input A1 is selected */
  XMC_ERU_ETL_INPUT_A2 = 0x2U, /**< input A2 is selected */
  XMC_ERU_ETL_INPUT_A3 = 0x3U  /**< input A3 is selected */
} XMC_ERU_ETL_INPUT_A_t;

/**
 * Defines input signal for path B of ERSx(Event request source, x = [0 to 3]) unit.
 * @image html "eru_input_b.png" "ETLx Input B selection"
 */
typedef enum XMC_ERU_ETL_INPUT_B
{
  XMC_ERU_ETL_INPUT_B0 = 0x0U, /**< input B0 is selected */
  XMC_ERU_ETL_INPUT_B1 = 0x1U, /**< input B1 is selected */
  XMC_ERU_ETL_INPUT_B2 = 0x2U, /**< input B2 is selected */
  XMC_ERU_ETL_INPUT_B3 = 0x3U  /**< input B3 is selected */
} XMC_ERU_ETL_INPUT_B_t;

/**
 * Defines input path combination along with polarity for event generation by ERSx(Event request source) unit to
 * ETLx(Event trigger logic),x = [0 to 3] unit.
 * @image html "eru_input_trigger.png" "ETLx input trigger signal generation"
 */
typedef enum XMC_ERU_ETL_SOURCE
{
  XMC_ERU_ETL_SOURCE_A = 0x0U,              /**< select (A) path as a event source */
  XMC_ERU_ETL_SOURCE_B = 0x1U,              /**< select (B) path as a event source */
  XMC_ERU_ETL_SOURCE_A_OR_B = 0x2U,         /**< select (A <b>OR</b> B) path as a event source */
  XMC_ERU_ETL_SOURCE_A_AND_B = 0x3U,        /**< select (A <b>AND</b> B) path as a event source */
  XMC_ERU_ETL_SOURCE_NOT_A = 0x4U,          /**< select (inverted A) path as a event source */
  XMC_ERU_ETL_SOURCE_NOT_A_OR_B = 0x6U,     /**< select (inverted A <b>OR</b> B) path as a event source */
  XMC_ERU_ETL_SOURCE_NOT_A_AND_B = 0x7U,    /**< select (inverted A <b>AND</b> B) path as a event source */
  XMC_ERU_ETL_SOURCE_NOT_B = 0x9U,          /**< select (inverted B) path as a event source */
  XMC_ERU_ETL_SOURCE_A_OR_NOT_B = 0xaU,     /**< select (A <b>OR</b> inverted B) path as a event source */
  XMC_ERU_ETL_SOURCE_A_AND_NOT_B = 0xbU,    /**< select (A <b>AND</b> inverted B) path as a event source */
  XMC_ERU_ETL_SOURCE_NOT_A_OR_NOT_B = 0xeU, /**< select (inverted A <b>OR</b> inverted B) path as a event
                                                           source */
  XMC_ERU_ETL_SOURCE_NOT_A_AND_NOT_B = 0xfU /**< select (inverted A <b>AND</b> inverted B) path as a event
                                                           source */
} XMC_ERU_ETL_SOURCE_t;

/**
 * Defines trigger edge for the event generation by ETLx (Event Trigger Logic, x = [0 to 3]) unit, by getting the signal
 * from ERSx(Event request source, x = [0 to 3]) unit.
 */
typedef enum XMC_ERU_ETL_EDGE_DETECTION
{
  XMC_ERU_ETL_EDGE_DETECTION_DISABLED = 0U, /**< no event enabled */
  XMC_ERU_ETL_EDGE_DETECTION_RISING = 1U,   /**< detection of rising edge generates the event */
  XMC_ERU_ETL_EDGE_DETECTION_FALLING = 2U,  /**< detection of falling edge generates the event */
  XMC_ERU_ETL_EDGE_DETECTION_BOTH = 3U      /**< detection of either edges generates the event */
} XMC_ERU_ETL_EDGE_DETECTION_t;

/**
 * Defines Output Channel of OGUy(Output gating unit y = [0 to 3]) to be mapped by the trigger pulse generated by
 * ETLx(Event Trigger Logic, x = [0 to 3]) unit.
 * @note Generation of output trigger pulse need to be enabled @ref XMC_ERU_OGU_PERIPHERAL_TRIGGER_t
 * @image html "eru_connection_matrix.png" "ERU_ETL ERU_OGU Connection matrix"
 */
typedef enum XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL
{
  XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL0 = 0U, /**< Event from input ETLx triggers output OGU0 */
  XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL1 = 1U, /**< Event from input ETLx triggers output OGU1 */
  XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL2 = 2U, /**< Event from input ETLx triggers output OGU2 */
  XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL3 = 3U, /**< Event from input ETLx triggers output OGU3 */
} XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL_t;

/**
 * Defines generation of the trigger pulse by ETLx(Event Trigger Logic, x = [0 to 3]) unit.
 * Use type XMC_ERU_ETL_OUTPUT_TRIGGER_t for this enum.
 */
typedef enum XMC_ERU_ETL_OUTPUT_TRIGGER
{
  XMC_ERU_ETL_OUTPUT_TRIGGER_DISABLED = 0U, /**< trigger pulse generation disabled */
  XMC_ERU_ETL_OUTPUT_TRIGGER_ENABLED = 1U   /**< trigger pulse generation enabled */
} XMC_ERU_ETL_OUTPUT_TRIGGER_t;

/**
 * Defines status flag reset mode generated by ETLx(Event Trigger Logic, x = [0 to 3]) unit.
 * Use type XMC_ERU_ETL_STATUS_FLAG_MODE_t for this enum.
 */
typedef enum XMC_ERU_ETL_STATUS_FLAG_MODE
{
  XMC_ERU_ETL_STATUS_FLAG_MODE_SWCTRL = 0U, /**< Status flag is in sticky mode. Retain the same state until
                                                 cleared by software. In case of pattern match this mode 
                                                 is used. */
  XMC_ERU_ETL_STATUS_FLAG_MODE_HWCTRL = 1U  /**< Status flag is in non-sticky mode. Automatically cleared by
                                                 the opposite edge detection.\n
                                                 eg. if positive edge is selected as trigger event, for the 
                                                 negative edge event the status flag is cleared. */
} XMC_ERU_ETL_STATUS_FLAG_MODE_t;

/**
 * Defines pattern detection feature to be enabled or not in OGUy(Output gating unit, y = [0 to 3]).
 *        
 */
typedef enum XMC_ERU_OGU_PATTERN_DETECTION
{
  XMC_ERU_OGU_PATTERN_DETECTION_DISABLED = 0U, /**< Pattern match is disabled */
  XMC_ERU_OGU_PATTERN_DETECTION_ENABLED = 1U   /**< Pattern match is enabled, the selected status flags of
                                                    ETLx(Event Trigger Logic, x = [0 to 3]) unit, are 
                                                    used in pattern detection. */
} XMC_ERU_OGU_PATTERN_DETECTION_t;

/**
 * Defines the inputs for Pattern detection. The configured status flag signal from the ETLx(Event Trigger Logic,
 * x = [0 to 3]) unit indicates the pattern to be detected.
 */
typedef enum XMC_ERU_OGU_PATTERN_DETECTION_INPUT
{
  XMC_ERU_OGU_PATTERN_DETECTION_INPUT0 = 1U, /**< Status flag ETL0, participating in pattern match */
  XMC_ERU_OGU_PATTERN_DETECTION_INPUT1 = 2U, /**< Status flag ETL1, participating in pattern match */
  XMC_ERU_OGU_PATTERN_DETECTION_INPUT2 = 4U, /**< Status flag ETL0, participating in pattern match */
  XMC_ERU_OGU_PATTERN_DETECTION_INPUT3 = 8U  /**< Status flag ETL0, participating in pattern match */
} XMC_ERU_OGU_PATTERN_DETECTION_INPUT_t;

/**
 * Defines peripheral trigger signal for event generation. Based on the selected peripheral for event generation,
 * the trigger signal is mapped.
 */
typedef enum XMC_ERU_OGU_PERIPHERAL_TRIGGER
{
  XMC_ERU_OGU_PERIPHERAL_TRIGGER1 = 1U, /**< OGUy1 signal is mapped for event generation */
  XMC_ERU_OGU_PERIPHERAL_TRIGGER2 = 2U, /**< OGUy2 signal is mapped for event generation */
  XMC_ERU_OGU_PERIPHERAL_TRIGGER3 = 3U  /**< OGUy3 signal is mapped for event generation */
} XMC_ERU_OGU_PERIPHERAL_TRIGGER_t;

/**
 * Defines the gating scheme for service request generation. In later stage of the OGUy(Output gating unit,
 * y = [0 to 3]) based on the gating scheme selected ERU_GOUTy(gated output signal) output is defined.
 * @image html "interrupt_gating_signal.png" "Interrupt gating signal"
 */
typedef enum XMC_ERU_OGU_SERVICE_REQUEST
{
  XMC_ERU_OGU_SERVICE_REQUEST_DISABLED = 0U,                      /**< Service request blocked, ERUx_GOUTy = 0 */
  XMC_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER = 1U,                    /**< Service request generated enabled, ERUx_GOUTy = 1 */
  XMC_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER_AND_PATTERN_MATCH = 2U,  /**< Service request generated on trigger
                                                                       event and input pattern match, 
                                                                       ERUx_GOUTy = ~pattern matching result*/
  XMC_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER_AND_PATTERN_MISMATCH = 3U/**< Service request generated on trigger
                                                                       event and input pattern mismatch, 
                                                                       ERUx_GOUTy = pattern matching result*/
} XMC_ERU_OGU_SERVICE_REQUEST_t;

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
 * ERU module
 */
typedef struct {
  union {
    __IO uint32_t  EXISEL;

    struct {
      __IO uint32_t  EXS0A      :  2;
      __IO uint32_t  EXS0B      :  2;
      __IO uint32_t  EXS1A      :  2;
      __IO uint32_t  EXS1B      :  2;
      __IO uint32_t  EXS2A      :  2;
      __IO uint32_t  EXS2B      :  2;
      __IO uint32_t  EXS3A      :  2;
      __IO uint32_t  EXS3B      :  2;
    } EXISEL_b;
  };
  __I  uint32_t  RESERVED0[3];

  union {
    __IO uint32_t  EXICON[4];        

    struct {
      __IO uint32_t  PE         :  1;
      __IO uint32_t  LD         :  1;
      __IO uint32_t  ED         :  2;
      __IO uint32_t  OCS        :  3;
      __IO uint32_t  FL         :  1;
      __IO uint32_t  SS         :  4;
      __I  uint32_t  RESERVED1  :  20;
    } EXICON_b[4];                   
  };

  union {
    __IO uint32_t  EXOCON[4];        

    struct {
      __IO uint32_t  ISS        :  2;
      __IO uint32_t  GEEN       :  1;
      __I  uint32_t  PDR        :  1;
      __IO uint32_t  GP         :  2;
           uint32_t             :  6;
      __IO uint32_t  IPEN       :  4;
      __I  uint32_t  RESERVED2  :  16;
    } EXOCON_b[4];                   
  };
} XMC_ERU_t;

/**
 * \if XMC4
 * Structure for initializing ERUx_ETLy (x = [0..1], y = [0..4]) module.
 * \endif
 * \if XMC1
 * Structure for initializing ERUx_ETLy (x = [0], y = [0..4]) module.
 * \endif
 */
typedef struct XMC_ERU_ETL_CONFIG
{
  union
  {
    uint32_t input; /**< While configuring the bit fields, the values have to be shifted according to the position */
    struct
    {
      uint32_t input_a: 2; /**< Configures input A. Refer @ref XMC_ERU_ETL_INPUT_A_t for valid values */
      uint32_t input_b: 2; /**< Configures input B. Refer @ref XMC_ERU_ETL_INPUT_B_t for valid values */
      uint32_t : 28;
    };
  };

  union
  {
    uint32_t raw;
    struct
    {
      uint32_t enable_output_trigger: 1;  /**< Enables the generation of trigger pulse(PE), for the configured edge 
                                               detection. This accepts boolean values as input. */
      uint32_t status_flag_mode: 1;       /**< Enables the status flag auto clear(LD), for the opposite edge of the 
                                               configured event edge. This accepts boolean values as input. */
      uint32_t edge_detection: 2;         /**< Configure the event trigger edge(FE, RE). 
                                               Refer @ref XMC_ERU_ETL_EDGE_DETECTION_t for valid values. */
      uint32_t output_trigger_channel: 3; /**< Output channel select(OCS) for ETLx output trigger pulse. 
                                               Refer @ref XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL_t for valid values. */
      uint32_t : 1;
      uint32_t source: 4;                 /**< Input path combination along with polarity for event generation.
                                               Refer @ref XMC_ERU_ETL_SOURCE_t for valid values. */
      uint32_t : 20;
    };
  };
} XMC_ERU_ETL_CONFIG_t;

/**
 * \if XMC4
 * Structure for initializing ERUx_OGUy (x = [0..1], y = [0..4]) module.
 * \endif
 * \if XMC1
 * Structure for initializing ERUx_OGUy (x = [0], y = [0..4]) module.
 * \endif
 */
typedef union XMC_ERU_OGU_CONFIG
{
    uint32_t raw;
    
    struct
    {
      uint32_t peripheral_trigger: 2;        /**< peripheral trigger(ISS) input selection. 
                                                  Refer @ref XMC_ERU_OGU_PERIPHERAL_TRIGGER_t for valid values. */
      uint32_t enable_pattern_detection: 1;  /**< Enable generation of(GEEN) event for pattern detection result change.
                                                  This accepts boolean values as input. */
      uint32_t : 1;
      uint32_t service_request: 2;          /**< Gating(GP) on service request generation for pattern detection result. 
                                                 Refer @ref XMC_ERU_OGU_SERVICE_REQUEST_t for valid values. */
      uint32_t : 6;
      uint32_t pattern_detection_input: 4;  /**< Enable input for the pattern detection(IPENx, x = [0 to 3]).  
                                                 Refer @ref XMC_ERU_OGU_PATTERN_DETECTION_INPUT_t for valid values. 
                                                 <b>OR</b> combination of the enum items given as input */
      uint32_t : 16;
    };
} XMC_ERU_OGU_CONFIG_t;

/*Anonymous structure/union guard end */
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__TASKING__)
  #pragma warning restore
#endif
/*********************************************************************************************************************
 * API PROTOTYPES
 ********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * \if XMC4
 * If ERU1 module is selected, it enables clock and releases reset.<br>
 * \endif
 * \if XMC1
 * Abstract API, not mandatory to call. <br>
 * \endif
 * \par
 * This API is called by XMC_ERU_ETL_Init() or XMC_ERU_OGU_Init() and therefore no need to call it explicitly during 
 * initialization sequence. Call this API to enable ERU1 module once again if the module is disabled by calling 
 * XMC_ERU_Disable(). For ERU0 module clock gating and reset features are not available.
 * 
 * \par<b>Note:</b><br>
 * \if XMC4
 * 1. Required to configure ERU1 module again after calling XMC_ERU_Disable(). Since the all the registers are 
 *    reset with default values.
 * \endif
 * \par<b>Related APIs:</b><BR>
 * XMC_ERU_ETL_Init(), XMC_ERU_OGU_Init(), XMC_ERU_Disable().
 */
void XMC_ERU_Enable(XMC_ERU_t *const eru);

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * \if XMC4
 * Disables clock and releases reset for ERU1 module.<br>
 * \endif
 * \if XMC1
 * Abstract API, not mandatory to call. <br>
 * \endif
 *
 * \par<b>Note:</b><br>
 * \if XMC4
 * 1. Required to configure ERU1 module again after calling XMC_ERU_ETL_Init() or XMC_ERU_OGU_Init(). Since the all the
 *   registers are reset with default values.
 * \endif
 * \par<b>Related APIs:</b><BR>
 * XMC_ERU_Enable()
 */
void XMC_ERU_Disable(XMC_ERU_t *const eru);

/* ERU_ETL APIs */

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address
 * @param channel ERU_ETLx(Event trigger logic unit) channel
 *                Range : [0 to 3]
 * @param config pointer to a constant ERU_ETLx configuration data structure. 
 *               Refer data structure XMC_ERU_ETL_CONFIG_t for detail.
 *
 * @return None
 *
 * <b>Description:</b><br>
 * Initializes the selected ERU_ETLx \a channel with the \a config structure.<br> 
 *
 * Invokes XMC_ERU_Enable() to enable \a eru module clock. Then configures 
 * <ul>
 * <li>Input signal for path A and Path B,</li>
 * <li>Trigger pulse generation,</li>
 * <li>status flag clear mode,</li>
 * <li>Event Trigger edge,</li>
 * <li>Output Channel of OGUy(Output gating unit y = [0 to 3]) to be mapped by the trigger pulse,</li>
 * <li>input path combination along with polarity for event generation</li>
 * </ul>.
 */
void XMC_ERU_ETL_Init(XMC_ERU_t *const eru, const uint8_t channel, const XMC_ERU_ETL_CONFIG_t *const config);

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address.
 * @param channel ERU_ETLx(Event trigger logic unit) channel.
 *                Range : [0 to 3]
 * @param input_a input signal for path A of ERSx(Event request source, x = [0 to 3]) unit.\n
 *                Refer XMC_ERU_ETL_INPUT_A_t for valid value or xmc1_eru_map.h/xmc4_eru_map.h file where the mapping of  
 *                the input is done based on selected signal.\n
 *                e.g: ERU0_ETL3_INPUTA_P2_7.
 * @param input_b input signal for path B of ERSx(Event request source, x = [0 to 3]) unit.\n
 *                Refer XMC_ERU_ETL_INPUT_B_t for valid value or xmc1_eru_map.h/xmc4_eru_map.h file where the mapping of  
 *                the input is done based on selected signal.\n
 *                e.g: ERU0_ETL0_INPUTB_P2_0.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the event source for path A and path B in with selected \a input_a and \a input_b respectively.<br>
 * \par
 * These values are set during initialization in XMC_ERU_ETL_Init(). Call this to change the input, as needed later in 
 * the program. According to the ports/peripheral selected, the event source has to be changed. 
 */
void XMC_ERU_ETL_SetInput(XMC_ERU_t *const eru,
                          const uint8_t channel,
                          const XMC_ERU_ETL_INPUT_A_t input_a,
                          const XMC_ERU_ETL_INPUT_B_t input_b);
 
/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address.
 * @param channel ERU_ETLx(Event trigger logic unit) channel.
 *                Range : [0 to 3]
 * @param source input path combination along with polarity for event generation by ERSx(Event request source) unit.
 *               Refer @ref XMC_ERU_ETL_SOURCE_t enum for valid input values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Select input path combination along with polarity for event generation by setting (SS, NA, NB) bits in 
 * ERSx(Event request source) unit <br>
 * \par
 * The signal ERSxO is generated from the selection and this is connected to ETLx(Event trigger logic, 
 * x = [0 to 3]) for further action. These values are set during initialization in XMC_ERU_ETL_Init(). Call this to 
 * change the source, as needed later in the program.
 */
void XMC_ERU_ETL_SetSource(XMC_ERU_t *const eru,
                           const uint8_t channel,
                           const XMC_ERU_ETL_SOURCE_t source);

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address.
 * @param channel ERU_ETLx(Event trigger logic unit) channel.
 *                Range : [0 to 3]
 * @param edge_detection event trigger edge.
 *                       Refer @ref XMC_ERU_ETL_EDGE_DETECTION_t enum for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configure event trigger edge/s by setting (RE, FE) bits of EXICONx(x = [0 to 3]) register.<br>
 * \par
 * Rising edge, falling edge or either edges can be selected to generate the event.These values are set during 
 * initialization in XMC_ERU_ETL_Init(). Call this to change the trigger edge, as needed later in the program.
 */
void XMC_ERU_ETL_SetEdgeDetection(XMC_ERU_t *const eru,
                                  const uint8_t channel,
                                  const XMC_ERU_ETL_EDGE_DETECTION_t edge_detection);

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address.
 * @param channel ERU_ETLx(Event trigger logic unit) channel.
 *                Range : [0 to 3]
 *
 * @return XMC_ERU_ETL_EDGE_DETECTION_t indicate which egde/s is configured for event generation
 *
 * \par<b>Description:</b><br>
 * Return event trigger edge/s by reading (RE, FE) bits of EXICONx(x = [0 to 3]) register.<br>
 * \par
 * Rising edge, falling edge or either edges can be selected to generate the event.
 * Call this to get the configured trigger edge. */								  
XMC_ERU_ETL_EDGE_DETECTION_t XMC_ERU_ETL_GetEdgeDetection(XMC_ERU_t *const eru,
                                                          const uint8_t channel);
/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address.
 * @param channel ERU_ETLx(Event trigger logic unit) channel.
 *                Range : [0 to 3]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set the status flag bit(FL) in EXICONx(x = [0 to 3]).<br>
 * \par
 * The status flag indicates that the configured event has occurred. This status flag is used in Pattern match detection
 * by OGUy(Output gating unit, y = [0 to 3]).
 * \par
 * \par<b>Related APIs:</b><BR>
 * XMC_ERU_ETL_ClearStatusFlag(), XMC_ERU_ETL_GetStatusFlag()
 */
__STATIC_INLINE void XMC_ERU_ETL_SetStatusFlag(XMC_ERU_t *const eru, const uint8_t channel)
{
  XMC_ASSERT("XMC_ERU_ETL_SetStatusFlag:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_ETL_SetStatusFlag:Invalid Channel Number", (channel < 4U));

  eru->EXICON_b[channel].FL = true;
}

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address.
 * @param channel ERU_ETLx(Event trigger logic unit) channel.
 *                Range : [0 to 3]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set the status flag bit(FL) in EXICONx(x = [0 to 3]).<br>
 * \par
 * If auto clear of the status flag is not enabled by detection of the opposite edge of the event edge, this API clears
 * the Flag. SO that next event is considered as new event.
 * \par<b>Related APIs:</b><BR>
 * XMC_ERU_ETL_SetStatusFlag(), XMC_ERU_ETL_GetStatusFlag()
 */
__STATIC_INLINE void XMC_ERU_ETL_ClearStatusFlag(XMC_ERU_t *const eru, const uint8_t channel)
{
  XMC_ASSERT("XMC_ERU_ETL_ClearStatusFlag:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_ETL_ClearStatusFlag:Invalid Channel Number", (channel < 4U));

  eru->EXICON_b[channel].FL = false;
}

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address.
 * @param channel ERU_ETLx(Event trigger logic unit) channel.
 *                Range : [0 to 3].
 *
 * @return uint32_t Current state of the status flag bit(FL). Result is in 32-bit format.
 *
 * \par<b>Description:</b><br>
 * Returns status flag state of \a channel.
 * \par
 * The function can typically be used to clear the status flag using software, when auto clear is not enabled.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_ERU_ETL_SetStatusFlag(), XMC_ERU_ETL_ClearStatusFlag()
 */
__STATIC_INLINE uint32_t XMC_ERU_ETL_GetStatusFlag(XMC_ERU_t *const eru, const uint8_t channel)
{
  XMC_ASSERT("XMC_ERU_ETL_GetStatusFlag:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_ETL_GetStatusFlag:Invalid Channel Number", (channel < 4U));

  return (uint32_t)eru->EXICON_b[channel].FL;
}

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address.
 * @param channel ERU_ETLx(Event trigger logic unit) channel.
 *                Range : [0 to 3].
 * @param mode Set whether status flag has to be cleared by software or hardware. 
 *                Refer @ref XMC_ERU_ETL_STATUS_FLAG_MODE_t for valid value.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set the mode for status flag mode by setting (LD) bit in EXICONx(x = \a channel) register.<br>
 * \par
 * If SWCTRL is selected, status flag has to be cleared by software. This is typically used for pattern match detection. 
 * If HWCTRL is selected, status flag is cleared by hardware. If Positive edge is selected as event edge, for negative 
 * edge status flag is cleared and vice versa.This is typically used for continuous event detection.These values are set
 * during initialization in XMC_ERU_ETL_Init(). Call this to change the trigger edge, as needed later in the program.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_ERU_ETL_ClearStatusFlag(), XMC_ERU_ETL_GetStatusFlag()
 */
void XMC_ERU_ETL_SetStatusFlagMode(XMC_ERU_t *const eru,
                                   const uint8_t channel,
                                   const XMC_ERU_ETL_STATUS_FLAG_MODE_t mode);

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address.
 * @param channel ERU_ETLx(Event trigger logic unit) channel.
 *                Range : [0 to 3].
 * @param trigger Output Channel of OGUy(Output gating unit y = [0 to 3]) to be mapped by the trigger pulse 
 *                Refer @ref XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL_t for valid value.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configure which Channel of OGUy(Output gating unit y = [0 to 3]) to be mapped by the trigger pulse generated by 
 * ETLx(Event Trigger Logic, x = \a channel) by setting (OCS and PE) bit fields.
 * \par
 * The trigger pulse is generated for one clock pulse along with the flag status update. This is typically used to 
 * trigger the ISR for the external events. The configured OGUy(Output gating unit y = [0 to 3]), generates the event 
 * based on the trigger pulse.If output trigger pulse generation is disabled by XMC_ERU_ETL_DisableOutputTrigger(),
 * XMC_ERU_ETL_EnableOutputTrigger() can called to reconfigure. These values are set during initialization in 
 * XMC_ERU_ETL_Init(). Call this to change the trigger edge, as needed later in the program.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_ERU_ETL_DisableOutputTrigger()
 */
void XMC_ERU_ETL_EnableOutputTrigger(XMC_ERU_t *const eru,
                                     const uint8_t channel,
                                     const XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL_t trigger);

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address.
 * @param channel ERU_ETLx(Event trigger logic unit) channel.
 *                Range : [0 to 3].
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the trigger pulse generation by clearing the (PE) of the EXICONx(x = \a channel).
 * \par
 * Typically this can used when only pattern match is being used for event generation.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_ERU_ETL_EnableOutputTrigger()
 */
void XMC_ERU_ETL_DisableOutputTrigger(XMC_ERU_t *const eru, const uint8_t channel);

/* ERU_OGU APIs */

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address
 * @param channel ERU_OGUy(Output gating unit) channel
 *                Range : [0 to 3]
 * @param config pointer to constant ERU_OGUy configuration data structure. 
 *               Refer data structure XMC_ERU_OGU_CONFIG_t for detail.
 *
 * @return None
 *
 * <b>Description:</b><br>
 * Initializes the selected ERU_OGUy \a channel with the \a config structure.<br> 
 *
 * Invokes XMC_ERU_Enable() to enable \a eru module clock. Then configures 
 * <ul>
 * <li>Pattern detection,</li>
 * <li>Peripheral trigger input,</li>
 * <li>Gating for service request generation</li>
 * </ul>.
 */
void XMC_ERU_OGU_Init(XMC_ERU_t *const eru,
                      const uint8_t channel,
                      const XMC_ERU_OGU_CONFIG_t *const config);

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address
 * @param channel ERU_OGUy(Output gating unit) channel
 *                Range : [0 to 3]
 * @param input ERU_ETLx(x = [0 to 3]), for pattern match detection.
 *              Refer @ref XMC_ERU_OGU_PATTERN_DETECTION_INPUT_t for valid values. Logical <b>OR</b> combination of the 
 *              enum items can be passed as the input. 
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures ERU_ETLx(x = [0 to 3]) for pattern match detection by setting IPENx(x = [0 to 3]) and GEEN bits.
 * \par
 * These bits are dedicated to each channel of the ERU_ETLx(x = [0 to 3]). These values are set during initialization in 
 * XMC_ERU_OGU_Init(). Call this to change the pattern, as needed later in the program.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_ERU_OGU_DisablePatternDetection(), XMC_ERU_OGU_GetPatternDetectionStatus()
 */
void XMC_ERU_OGU_EnablePatternDetection(XMC_ERU_t *const eru,
                                        const uint8_t channel,
                                        const XMC_ERU_OGU_PATTERN_DETECTION_INPUT_t input);

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address
 * @param channel ERU_OGUy(Output gating unit) channel
 *                Range : [0 to 3]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disable the pattern detection by clearing (GEEN) bit.
 * \par
 * Typically XMC_ERU_OGU_DisablePatternDetection is used when events has to be generated peripheral triggers.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_ERU_OGU_EnablePatternDetection(), XMC_ERU_OGU_GetPatternDetectionStatus()
 */
void XMC_ERU_OGU_DisablePatternDetection(XMC_ERU_t *const eru, const uint8_t channel);

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address
 * @param channel ERU_OGUy(Output gating unit) channel
 *                Range : [0 to 3]
 *
 * @return uint32_t returns the pattern match result. Result is in 32-bit format.
 *
 * \par<b>Description:</b><br>
 * This API returns the pattern match result by reading (PDR) bit.
 * \par
 * \par<b>Related APIs:</b><BR>
 * XMC_ERU_OGU_EnablePatternDetection(), XMC_ERU_OGU_DisablePatternDetection()
 */
__STATIC_INLINE uint32_t XMC_ERU_OGU_GetPatternDetectionStatus(XMC_ERU_t *const eru,
                                                               const uint8_t channel)
{
  XMC_ASSERT("XMC_ERU_OGU_GetPatternDetectionStatus:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_OGU_GetPatternDetectionStatus:Invalid Channel Number", (channel < 4U));

  return (uint32_t)eru->EXOCON_b[channel].PDR;
}

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address
 * @param channel ERU_OGUy(Output gating unit) channel
 *                Range : [0 to 3]
 * @param peripheral_trigger which peripheral trigger signal is used for event generation.
 *                           Refer @ref XMC_ERU_OGU_PERIPHERAL_TRIGGER_t for the valid values, or 
                             xmc1_eru_map.h/xmc4_eru_map.h file where the mapping of the peripheral input is done based 
                             on input. e.g: ERU0_OGU0_PERIPHERAL_TRIGGER_CCU40_SR0.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures peripheral trigger input, by setting (ISS) bit.
 * \par
 * Based on the peripheral the input signal has to be selected. These values are set during initialization in 
 * XMC_ERU_OGU_Init(). Call this to change the input, as needed later in the program.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_ERU_OGU_DisablePeripheralTrigger()
 */
void XMC_ERU_OGU_EnablePeripheralTrigger(XMC_ERU_t *const eru,
                                         const uint8_t channel,
                                         const XMC_ERU_OGU_PERIPHERAL_TRIGGER_t peripheral_trigger);

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address
 * @param channel ERU_OGUy(Output gating unit) channel
 *                Range : [0 to 3]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables event generation based on peripheral trigger by clearing (ISS) bit.
 * \par
 * This is typically used when peripheral trigger is no longer need. After calling 
 * XMC_ERU_OGU_DisablePeripheralTrigger(), XMC_ERU_OGU_EnablePeripheralTrigger() has to be called to reconfigure the 
 * signals again.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_ERU_OGU_EnablePeripheralTrigger()
 */
void XMC_ERU_OGU_DisablePeripheralTrigger(XMC_ERU_t *const eru,
                                          const uint8_t channel);

/**
 * @param eru A constant pointer to XMC_ERU_t, pointing to the ERU base address
 * @param channel ERU_OGUy(Output gating unit) channel
 *                Range : [0 to 3]
 * @param mode gating scheme for service request generation.
 *             Refer @ref XMC_ERU_OGU_SERVICE_REQUEST_t for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the gating scheme for service request generation by setting (GP) bit.<br>
 * \par
 * Typically this function is used to change the service request generation scheme. These values are set during 
 * initialization in XMC_ERU_OGU_Init(). Call this to change the gating mode, as needed later in the program. 
 * 
 */
void XMC_ERU_OGU_SetServiceRequestMode(XMC_ERU_t *const eru,
                                       const uint8_t channel,
                                       const XMC_ERU_OGU_SERVICE_REQUEST_t mode);

#ifdef __cplusplus
}
#endif

/**
 * @} (end addtogroup ERU)
 */

/**
 * @} (end addtogroup XMClib)
 */

#endif /* XMC_ERU_H */
