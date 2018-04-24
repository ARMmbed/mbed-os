/**
 * @file xmc_dsd.h
 * @date 2015-09-18
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
 * 2015-03-30:
 *     - Initial version
 *      
 * 2015-06-19:
 *     - Removed version macros and declaration of GetDriverVersion API <BR>
 *     - Added API XMC_DSD_CH_GetRectifyDelay <BR>
 *
 * 2015-07-16:
 *     - Renamed API “XMC_DSD_CH_AuxFilter_SetBoudary()” to “XMC_DSD_CH_AuxFilter_SetBoundary()” <BR>
 *
 * 2015-09-18:
 *     - Added APIs "XMC_DSD_SetResultEventFlag()","XMC_DSD_ClearResultEventFlag()"
 *       "XMC_DSD_SetAlarmEventFlag()" and "XMC_DSD_ClearAlarmEventFlag()" <BR>
 *     - Support added for XMC4800 microcontroller family <BR>
 * @endcond
 *
 */


#ifndef XMC_DSD_H
#define XMC_DSD_H


/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
#include <xmc_common.h>

#if defined(DSD)
#include <xmc_scu.h>

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup DSD
* @{
* @brief Delta Sigma Demodulator (DSD) driver for the XMC4500, XMC4400 and XMC4800 microcontroller family <br>
 *
 * The DSD unit provides a series of digital input channels accepting data streams from external modulators
 * using the Delta/Sigma (DS) conversion principle. The on-chip demodulator channels convert these inputs to 
 * discrete digital values.
 * DSD unit can be used for isolated current/voltage measurement and for sensor interfaces.<br>
 *
 * Driver is divided in six DSD functional blocks -
 * - Main Filter (APIs prefixed with XMC_DSD_CH_MainFilter),
 * - Aux Filter (APIs prefixed with XMC_DSD_CH_AuxFilter),
 * - Integrator (APIs prefixed with XMC_DSD_CH_Integrator),
 * - Timestamp  (APIs prefixed with XMC_DSD_CH_Timestamp),
 * - Rectification (APIs prefixed with XMC_DSD_CH_Rectify),
 * - Carrier Generator (APIs prefixed with XMC_DSD_Generator)
 *
 * DSD driver features:
 * -# DSD channel Configuration structure XMC_DSD_CH_CONFIG_t initialization function XMC_DSD_CH_Init() to configure all the functional blocks (except carrier generator)
 * -# Configuration structure XMC_DSD_GENERATOR_CONFIG_t and initialization function XMC_DSD_Generator_Init() to configure carrier generator
 * -# Configuration structure XMC_DSD_CH_FILTER_CONFIG_t and initialization function XMC_DSD_CH_MainFilter_Init() to configure main  filter
 * -# Configuration structure XMC_DSD_CH_AUX_FILTER_CONFIG_t and initialization function XMC_DSD_CH_AuxFilter_Init() to configure auxilary  filter
 * -# Configuration structure XMC_DSD_CH_INTEGRATOR_CONFIG_t and initialization function XMC_DSD_CH_Integrator_Init() to configure integrator
 * -# Configuration structure XMC_DSD_CH_TIMESTAMP_CONFIG_t and initialization function XMC_DSD_CH_Timestamp_Init() to configure timestamp
 * -# Configuration structure XMC_DSD_CH_RECTIFY_CONFIG_t and initialization function XMC_DSD_CH_Rectify_Init() to configure rectifier
 */
 
 /*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
 #define XMC_DSD_CHECK_MODULE_PTR(PTR)  ( ((PTR)== DSD))
 #define XMC_DSD_CHECK_CHANNEL_PTR(PTR) ( ((PTR) == DSD_CH0) || ((PTR) == DSD_CH1) || ((PTR) == DSD_CH2) || ((PTR) == DSD_CH3))
 
/**********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/
/**
 * DSD Channel
 */
typedef DSD_CH_TypeDef XMC_DSD_CH_t;

/**
 * DSD Module
 */
typedef DSD_GLOBAL_TypeDef XMC_DSD_t;


/**
 * Return types of the API's.Use type @ref XMC_DSD_STATUS_t for this enum.
 */
typedef enum XMC_DSD_STATUS
{

  XMC_DSD_STATUS_OK,        /**< API fulfills request */
  XMC_DSD_STATUS_ERROR      /**< Error detected */

} XMC_DSD_STATUS_t;

/**
 * Enumerates the divider factor for the PWM pattern signal generator.
 * Use divider factor to derive input frequency of the carrier signal generator(fCG),
 * from the selected internal clock source(fCLK).
 * Use type @ref XMC_DSD_GENERATOR_CLKDIV_t for this enum.
 */
typedef enum XMC_DSD_GENERATOR_CLKDIV
{
  XMC_DSD_GENERATOR_CLKDIV_2048  = 0x00U,   /**< fCG = (fCLK/2)/1024 */
  XMC_DSD_GENERATOR_CLKDIV_4096  = 0x01U,   /**< fCG = (fCLK/4)/1024 */
  XMC_DSD_GENERATOR_CLKDIV_6144  = 0x02U,   /**< fCG = (fCLK/6)/1024 */
  XMC_DSD_GENERATOR_CLKDIV_8192  = 0x03U,   /**< fCG = (fCLK/8)/1024 */
  XMC_DSD_GENERATOR_CLKDIV_10240 = 0x04U,   /**< fCG = (fCLK/10)/1024 */
  XMC_DSD_GENERATOR_CLKDIV_12288 = 0x05U,   /**< fCG = (fCLK/12)/1024 */
  XMC_DSD_GENERATOR_CLKDIV_14336 = 0x06U,   /**< fCG = (fCLK/14)/1024 */
  XMC_DSD_GENERATOR_CLKDIV_16384 = 0x07U,   /**< fCG = (fCLK/16)/1024 */
  XMC_DSD_GENERATOR_CLKDIV_18432 = 0x08U,   /**< fCG = (fCLK/18)/1024 */
  XMC_DSD_GENERATOR_CLKDIV_20480 = 0x09U,   /**< fCG = (fCLK/20)/1024 */
  XMC_DSD_GENERATOR_CLKDIV_22528 = 0x0AU,   /**< fCG = (fCLK/22)/1024 */
  XMC_DSD_GENERATOR_CLKDIV_24576 = 0x0BU,   /**< fCG = (fCLK/24)/1024 */
  XMC_DSD_GENERATOR_CLKDIV_26624 = 0x0CU,   /**< fCG = (fCLK/26)/1024 */
  XMC_DSD_GENERATOR_CLKDIV_28672 = 0x0DU,   /**< fCG = (fCLK/28)/1024 */
  XMC_DSD_GENERATOR_CLKDIV_30720 = 0x0EU,   /**< fCG = (fCLK/30)/1024 */
  XMC_DSD_GENERATOR_CLKDIV_32768 = 0x0FU    /**< fCG = (fCLK/32)/1024 */

} XMC_DSD_GENERATOR_CLKDIV_t;


/**
 * Enumerates the carrier generator operating mode.
 * Use type @ref XMC_DSD_GENERATOR_MODE_t for this enum.
 */
typedef enum XMC_DSD_GENERATOR_MODE
{
  XMC_DSD_GENERATOR_MODE_STOPPED   = 0x00U, /**< Stopped */
  XMC_DSD_GENERATOR_MODE_RECTANGLE = 0x01U, /**< Square wave */
  XMC_DSD_GENERATOR_MODE_TRIANGLE  = 0x02U, /**< Triangle */
  XMC_DSD_GENERATOR_MODE_SINE      = 0x03U /**< Sine wave*/

} XMC_DSD_GENERATOR_MODE_t;


/**
 * Enumerates the CIC(cyclic integrating comb) filter type.
 * Use type @ref XMC_DSD_CH_FILTER_TYPE_t for this enum.
 */
typedef enum XMC_DSD_CH_FILTER_TYPE
{

  XMC_DSD_CH_FILTER_TYPE_CIC1,  /**< CIC1 filter*/
  XMC_DSD_CH_FILTER_TYPE_CIC2,  /**< CIC2 filter*/
  XMC_DSD_CH_FILTER_TYPE_CIC3,  /**< CIC3 filter*/
  XMC_DSD_CH_FILTER_TYPE_CICF   /**< CICF filter*/

} XMC_DSD_CH_FILTER_TYPE_t;

/**
 * Enumerates the input data source select.
 * Use type @ref XMC_DSD_CH_DATA_SOURCE_t for this enum.
 */
typedef enum XMC_DSD_CH_DATA_SOURCE
{

  XMC_DSD_CH_DATA_SOURCE_DISCONNECT  = 0U,  /**< Disconnected */
  XMC_DSD_CH_DATA_SOURCE_A_DIRECT    = 2U,  /**< External source, from input A, direct */
  XMC_DSD_CH_DATA_SOURCE_A_INVERTED  = 3U,  /**< External source, from input A, inverted */
  XMC_DSD_CH_DATA_SOURCE_B_DIRECT    = 4U,  /**< External source, from input B, direct */
  XMC_DSD_CH_DATA_SOURCE_B_INVERTED  = 5U   /**< External source, from input B, inverted */

} XMC_DSD_CH_DATA_SOURCE_t;

/**
 * Enumerates the sample clock source select.
 * Use type @ref XMC_DSD_CH_CLOCK_SOURCE_t for this enum.
 */
typedef enum XMC_DSD_CH_CLOCK_SOURCE
{

  XMC_DSD_CH_CLOCK_SOURCE_A = 1U,        /**< External source, from input A */
  XMC_DSD_CH_CLOCK_SOURCE_B = 2U,        /**< External source, from input B */
  XMC_DSD_CH_CLOCK_SOURCE_C = 3U,        /**< External source, from input C */
  XMC_DSD_CH_CLOCK_SOURCE_D = 4U,        /**< External source, from input D */
  XMC_DSD_CH_CLOCK_SOURCE_INTERN = 15U   /**< Internal clock source */

} XMC_DSD_CH_CLOCK_SOURCE_t;


/**
 * Enumerates the data strobe generation Mode.
 * Use type @ref XMC_DSD_CH_STROBE_t for this enum.
 */
typedef enum XMC_DSD_CH_STROBE
{

  XMC_DSD_CH_STROBE_DIRECT_CLOCK_RISE = 1U,  /* Direct clock, a sample trigger is generated at each rising clock edge */
  XMC_DSD_CH_STROBE_DIRECT_CLOCK_FALL = 2U,  /* Direct clock, a sample trigger is generated at each falling clock edge */
  XMC_DSD_CH_STROBE_DOUBLE_DATA       = 3U,  /* Double data, a sample trigger is generated at each rising and falling clock edge */
  XMC_DSD_CH_STROBE_DOUBLE_CLOCK_RISE = 5U,  /* Double clock, a sample trigger is generated at every 2nd rising clock edge */
  XMC_DSD_CH_STROBE_DOUBLE_CLOCK_FALL = 6U   /* Double clock, a sample trigger is generated at every 2nd falling clock edge */

} XMC_DSD_CH_STROBE_t;


/**
 * Enumerates the divider factor for modulator clock (fMOD).
 * Use divider factor to derive modulator clock frequency(fMOD),
 * from the selected internal clock source(fCLK).
 * Use type @ref XMC_DSD_CH_CLK_t for this enum.
 */
typedef enum XMC_DSD_CH_CLK_DIV
{

  XMC_DSD_CH_CLK_DIV_2,        /**< fMOD = fCLK/2 */
  XMC_DSD_CH_CLK_DIV_4,        /**< fMOD = fCLK/4 */
  XMC_DSD_CH_CLK_DIV_6,        /**< fMOD = fCLK/6 */
  XMC_DSD_CH_CLK_DIV_8,        /**< fMOD = fCLK/8 */
  XMC_DSD_CH_CLK_DIV_10,       /**< fMOD = fCLK/10 */
  XMC_DSD_CH_CLK_DIV_12,       /**< fMOD = fCLK/12 */
  XMC_DSD_CH_CLK_DIV_14,       /**< fMOD = fCLK/14 */
  XMC_DSD_CH_CLK_DIV_16,       /**< fMOD = fCLK/16 */
  XMC_DSD_CH_CLK_DIV_18,       /**< fMOD = fCLK/18 */
  XMC_DSD_CH_CLK_DIV_20,       /**< fMOD = fCLK/20 */
  XMC_DSD_CH_CLK_DIV_22,       /**< fMOD = fCLK/22 */
  XMC_DSD_CH_CLK_DIV_24,       /**< fMOD = fCLK/24 */
  XMC_DSD_CH_CLK_DIV_26,       /**< fMOD = fCLK/26 */
  XMC_DSD_CH_CLK_DIV_28,       /**< fMOD = fCLK/28 */
  XMC_DSD_CH_CLK_DIV_30,       /**< fMOD = fCLK/30 */
  XMC_DSD_CH_CLK_DIV_32        /**< fMOD = fCLK/32 */

} XMC_DSD_CH_CLK_t;

/**
 * Enumerates the integrator trigger mode.
 * Use type @ref XMC_DSD_CH_INTEGRATOR_START_t for this enum.
 */
typedef enum XMC_DSD_CH_INTEGRATOR_START
{
  XMC_DSD_CH_INTEGRATOR_START_OFF,             /**< No integration trigger */
  XMC_DSD_CH_INTEGRATOR_START_TRIGGER_FALL,    /**< Trigger event upon a falling edge */
  XMC_DSD_CH_INTEGRATOR_START_TRIGGER_RISE,    /**< Trigger event upon a rising edge */
  XMC_DSD_CH_INTEGRATOR_START_ALLWAYS_ON       /**< No trigger, integrator active all the time */

} XMC_DSD_CH_INTEGRATOR_START_t;

/**
 * Enumerates the integration enable.
 * Use type @ref XMC_DSD_CH_INTEGRATOR_STOP_t for this enum.
 */
typedef enum XMC_DSD_CH_INTEGRATOR_STOP
{
  XMC_DSD_CH_INTEGRATOR_STOP_END_OF_LOOPS,                /**< Integration stopped upon the inverse trigger event */
  XMC_DSD_CH_INTEGRATOR_STOP_ENDLESS_OR_INVERSE_TRIGGER   /**< Integration enabled upon the defined trigger event. */

} XMC_DSD_CH_INTEGRATOR_STOP_t;


/**
 * Enumerates the trigger signal.
 * Use type @ref XMC_DSD_CH_TRIGGER_SOURCE_t for this enum.
 */
typedef enum XMC_DSD_CH_TRIGGER_SOURCE
{

  XMC_DSD_CH_TRIGGER_SOURCE_A = 0U,       /**< Trigger signal,input A*/
  XMC_DSD_CH_TRIGGER_SOURCE_B = 1U,       /**< Trigger signal,input B*/
  XMC_DSD_CH_TRIGGER_SOURCE_C = 2U,       /**< Trigger signal,input C*/
  XMC_DSD_CH_TRIGGER_SOURCE_D = 3U,       /**< Trigger signal,input D*/
  XMC_DSD_CH_TRIGGER_SOURCE_E = 4U,       /**< Trigger signal,input E*/
  XMC_DSD_CH_TRIGGER_SOURCE_F = 5U,       /**< Trigger signal,input F*/
  XMC_DSD_CH_TRIGGER_SOURCE_G = 6U,       /**< Trigger signal,input G*/
  XMC_DSD_CH_TRIGGER_SOURCE_H = 7U        /**< Trigger signal,input H*/

} XMC_DSD_CH_TRIGGER_SOURCE_t;

/**
 * Enumerates the timestamp trigger mode.
 * Use type @ref XMC_DSD_CH_TIMESTAMP_TRIGGER_t for this enum.
 */
typedef enum XMC_DSD_CH_TIMESTAMP_TRIGGER
{
  XMC_DSD_CH_TIMESTAMP_TRIGGER_DISABLE,    /**< No trigger event*/
  XMC_DSD_CH_TIMESTAMP_TRIGGER_FALL,       /**< Trigger event upon a falling edge*/
  XMC_DSD_CH_TIMESTAMP_TRIGGER_RISE,       /**< Trigger event upon a rising edge*/
  XMC_DSD_CH_TIMESTAMP_TRIGGER_BOTH_EDGES  /**< Trigger event upon both the edge*/

} XMC_DSD_CH_TIMESTAMP_TRIGGER_t;

/**
 * Enumerates the carrier generation mode.
 * Use type @ref XMC_DSD_CH_SIGN_SOURCE_t for this enum.
 */
typedef enum XMC_DSD_CH_SIGN_SOURCE
{
  XMC_DSD_CH_SIGN_SOURCE_ON_CHIP_GENERATOR, /**< Carrier is generated internally by DSD */
  XMC_DSD_CH_SIGN_SOURCE_NEXT_CHANNEL,      /**< Carrier sign signal is generated internally by next channel*/
  XMC_DSD_CH_SIGN_SOURCE_EXTERNAL_A,        /**< Carrier is generated externally, External sign signal A*/
  XMC_DSD_CH_SIGN_SOURCE_EXTERNAL_B         /**< Carrier is generated externally, External sign signal B*/

} XMC_DSD_CH_SIGN_SOURCE_t;

/**
 * Enumerates the channel run control bit register value in global run control register.
 * Use type @ref XMC_DSD_CH_ID_t for this enum.
 */
typedef enum XMC_DSD_CH_ID
{
  XMC_DSD_CH_ID_0 = 1U,   /**< Register value for channel 0 */
  XMC_DSD_CH_ID_1 = 2U,   /**< Register value for channel 1 */
  XMC_DSD_CH_ID_2 = 4U,   /**< Register value for channel 2 */
  XMC_DSD_CH_ID_3 = 8U    /**< Register value for channel 3 */

} XMC_DSD_CH_ID_t;

/**
 * Enumerates the service request generation mode for auxiliary filter.
 * Use type @ref XMC_DSD_CH_AUX_EVENT_t for this enum.
 * Note: This is combined ENUM for SRGA + ESEL bit fields
 */
typedef enum XMC_DSD_CH_AUX_EVENT
{
  XMC_DSD_CH_AUX_EVENT_DISABLED            = 0U,   /**< Service request is disabled */
  XMC_DSD_CH_AUX_EVENT_EVERY_NEW_RESULT    = 1U,   /**< Service request generated for aux filter for every new result */
  XMC_DSD_CH_AUX_EVENT_CAPTURE_SIGN_DELAY  = 2U,   /**< Service request generated for alternate source */
  XMC_DSD_CH_AUX_EVENT_INSIDE_BOUNDARY     = 5U,   /**< Service request generated for aux filter if result is inside boundary */
  XMC_DSD_CH_AUX_EVENT_OUTSIDE_BOUNDARY    = 9U    /**< Service request generated for aux filter if result is outside boundary */

} XMC_DSD_CH_AUX_EVENT_t;

/**
 * Enumerates the service request generation for main chain filter.
 * Use type @ref XMC_DSD_CH_RESULT_EVENT_t for this enum.
 */
typedef enum XMC_DSD_CH_RESULT_EVENT
{
	XMC_DSD_CH_RESULT_EVENT_DISABLE  = 0U,       /**< Disable service request */
	XMC_DSD_CH_RESULT_EVENT_ENABLE   = 3U        /**< Enable service request for each new result value */

} XMC_DSD_CH_RESULT_EVENT_t;


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
 * DSD generator generates several pattern and frequencies.
 * Use type @ref XMC_DSD_GENERATOR_CONFIG_t for this data structure.
 * Note: Output pins have to be enabled by user.
 */
typedef struct XMC_DSD_GENERATOR_CONFIG
{
  union
  {
    struct
    {
      uint32_t mode: 2;              /**< carrier generator operating mode. This parameter can take a value of XMC_DSD_GENERATOR_MODE_t */
      uint32_t bit_reverse: 1;       /**< should PWM signal be bit-reversed? 0: Normal mode, 1:Bit-reverse mode */
      uint32_t inverted_polarity: 1; /**< should PWM signal start from negative max. 0: Normal, 1: Inverted */
      uint32_t frequency: 4;         /**< Frequency divider value of PWM signal. This parameter can take a value of XMC_DSD_GENERATOR_CLKDIV_t */
      uint32_t :24;
    };
    uint32_t generator_conf;         /**< Carrier generator configuration register(CGCFG)*/
  };
} XMC_DSD_GENERATOR_CONFIG_t;


/**
 *  DSD filter is the basic module of the DSD. It can be used separately or can be combined with the other modules like
 *  integrator, rectify, auxiliary filter etc.
 *  The filter demodulates the incoming bit stream from the delta sigma modulator to a 16 bit result.
 *  Note: Configure or reconfigure filter parameters while the channel is inactive.
 */
typedef struct XMC_DSD_CH_FILTER_CONFIG
{
  uint32_t clock_divider: 4;	/**< This parameter can take a value of XMC_DSD_CH_CLK_t */
  int16_t offset;				/**< Offset subtracted from result.This parameter can take a value of int16_t */
  union
  {
    struct
    {
      uint32_t data_source: 4;	/**< This parameter can take a value of XMC_DSD_CH_DATA_SOURCE_t */
      uint32_t : 12;
      uint32_t clock_source: 4;	/**< This parameter can take a value of XMC_DSD_CH_CLOCK_SOURCE_t */
      uint32_t strobe: 4;		/**< This parameter can take a value of XMC_DSD_CH_STROBE_t */
      uint32_t :8;
    };
    uint32_t demodulator_conf;  /*Demodulator Input Configuration Register*/
  };
  union
  {
    struct
    {
      uint32_t : 8;
      uint32_t filter_type: 2;			/**< This parameter can take a value of XMC_DSD_CH_FILTER_TYPE_t */
      uint32_t : 4;
      uint32_t result_event : 2;        /**< This parameter can take a value of XMC_DSD_CH_RESULT_EVENT_t */
      uint32_t : 8;
      uint32_t : 8;
    };
    uint32_t main_filter_conf;

  };
  uint32_t decimation_factor;     /**< This parameter can be in range of 4 - 256[dec] */
  uint32_t filter_start_value;	  /**< This parameter can be in range of 4 - decimation_factor[dec]*/

} XMC_DSD_CH_FILTER_CONFIG_t;

/**
 *  The integrator is mainly used for high accurate measurement.
 *	Note:DSD Filter is mandatory to use this block.
 */
typedef struct XMC_DSD_CH_INTEGRATOR_CONFIG
{
  union
  {
	struct
	{
	  uint32_t :8;
	  uint32_t start_condition: 2; /**<  Can take a value of XMC_DSD_CH_INTEGRATOR_START_t.Bitfields \a ITRMODE of \a DICFG.*/
	  uint32_t :2;
	  uint32_t trigger_source: 3;  /**<  Can take a value of XMC_DSD_CH_TRIGGER_SOURCE_t.Bitfields \a TRSEL of \a DICFG. */
	  uint32_t :17;
	};
	uint32_t integrator_trigger;   /**< Demodulator Input Configuration Register(\a DICFG).*/

  };
	uint32_t integration_loop;     /**< Integration loops to see stop condition. Bitfields \a REPVAL of \a IWCTR.*/
	uint32_t discarded_values;     /**< Number of mainfilter results,discarded before integration starts.Bitfields \a NVALDIS of \a IWCTR.*/
	uint32_t stop_condition;	   /**< Integrator stop condition. Can take a value of XMC_DSD_CH_INTEGRATOR_STOP_t.Bitfields \a IWS of \a IWCTR.*/
	uint32_t counted_values;	   /**< Number of mainfilter results, integrated to a integrator result.Bitfields \a NVALINT of \a IWCTR.*/

} XMC_DSD_CH_INTEGRATOR_CONFIG_t;

/**
 *  DSD timestamp saves result, filter counter register and integrator count register.
 *	Note: Trigger source for timestamp and integrator trigger are shared.
 *        DSD Filter is mandatory to use this block.
 */
typedef struct XMC_DSD_CH_TIMESTAMP_CONFIG
{
  union
  {
    struct
    {
      uint32_t :10;
      uint32_t trigger_mode:2; /**<  This parameter can take a value of XMC_DSD_CH_TIMESTAMP_TRIGGER_t */
      uint32_t trigger_source: 3; /**<  This parameter can take a value of XMC_DSD_CH_TRIGGER_SOURCE_t */
      uint32_t : 17;
    };
    uint32_t timestamp_conf;
  };
} XMC_DSD_CH_TIMESTAMP_CONFIG_t;

/**
 *  DSD auxiliary Filter is used as fast filter to detect overvoltage or current by defining the boundaries.
 */
typedef struct XMC_DSD_CH_AUX_FILTER_CONFIG
{
  union
  {
    struct
    {
      uint32_t : 8;                             /**< This parameter can be in range of 4 - 256[dec] */
      uint32_t filter_type                : 2;  /**< This parameter can take a value of XMC_DSD_CH_FILTER_TYPE_t */
      uint32_t result_event_type          : 4;  /**< Result event for aux filter and the event select configuration.
                                                     Use enum XMC_DSD_CH_AUX_EVENT_t */
      uint32_t enable_integrator_coupling : 1;  /**< Only enable AUX filter when Integrator is enabled*/
      uint32_t                            : 17;
    };
    uint32_t aux_filter_conf;
  };
  union
  {
    struct
    {
      uint32_t lower_boundary             : 16;	/**< This parameter can take a value of int16_t */
      uint32_t upper_boundary             : 16; /**< This parameter can take a value of int16_t */
    };
    uint32_t boundary_conf;
  };

  uint32_t decimation_factor;                   /**< This parameter can be in range of 4 - 256[dec]*/

} XMC_DSD_CH_AUX_FILTER_CONFIG_t;


/**
 *  DSD Rectify.
 *  Note: DSD Filter and integrator is mandatory to use this block.
 */
typedef struct XMC_DSD_CH_RECTIFY_CONFIG
{
  union
  {
    struct
    {
      uint32_t :4;
      uint32_t sign_source: 2; /**< Can take a value of XMC_DSD_CH_SIGN_SOURCE_t.Bitfields \a SSRC of \a RECTCFG.*/
      uint32_t :26;
    };
    uint32_t rectify_config;  /**< Rectification configuration register(\a RECTCFG)*/
  };

  uint8_t delay;
  uint8_t half_cycle;

} XMC_DSD_CH_RECTIFY_CONFIG_t;


/*Anonymous structure/union guard end*/
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__TASKING__)
  #pragma warning restore
#endif

/**
 *  DSD Enable Channel
 */
typedef struct XMC_DSD_CONFIG
{
  XMC_DSD_CH_FILTER_CONFIG_t     *const filter;      /**< Pointer to the filter configuration */
  XMC_DSD_CH_INTEGRATOR_CONFIG_t *const integrator;  /**< Pointer to the integrator configuration*/
  XMC_DSD_CH_TIMESTAMP_CONFIG_t  *const timestamp;   /**< Pointer to the time stamp configuration*/
  XMC_DSD_CH_AUX_FILTER_CONFIG_t *const aux;         /**< Pointer to the aux_filter configuration*/
  XMC_DSD_CH_RECTIFY_CONFIG_t    *const rectify;     /**< Pointer to the rectify configuration*/
} XMC_DSD_CH_CONFIG_t;


/**********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param  dsd  Pointer to an instance of DSD module of type @ref XMC_DSD_t
 * @return None
 *
 * \par<b>Description</b><br>
 * De-asserts the DSD module from reset.\n
 * Configures \a PRCLR0 register's \a DSDRS bit field.
 * If running on XMC44/XMC48 device then it will ungate the peripheral clock.
 *
 * \par<b>Note</b><br>
 * It is internally called by XMC_DSD_Init().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_Disable(),XMC_DSD_Init() \n\n\n
 */
void XMC_DSD_Enable(XMC_DSD_t *const dsd);

/**
 * @param  dsd  Pointer to an instance of DSD module of type @ref XMC_DSD_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Asserts the DSD module into reset.\n
 * Configures \a PRSET0 register's \a DSDRS bit field.
 * If running on XMC44/XMC48 device then it will gate the peripheral clock.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_Enable()\n\n\n
 */
void XMC_DSD_Disable(XMC_DSD_t *const dsd);
/**
 * @param  dsd  Pointer to an instance of DSD module of type @ref XMC_DSD_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Enable the module clock and internal module clock for DSD.\n
 * Configures bit field \a MCSEL of register \a GLOBCFG and  bit field \a DISR of register \a CLC.
 *
 * \par<b>Note</b><br>
 * It is internally called by XMC_DSD_Init().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_DisableClock(),XMC_DSD_Init() \n\n\n
 */
void XMC_DSD_EnableClock(XMC_DSD_t *const dsd);
/**
 * @param  dsd  Pointer to an instance of DSD module of type @ref XMC_DSD_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Stop the module clock and disable internal module clock for DSD.\n
 * Configures bit field \a MCSEL of register \a GLOBCFG and  bit field \a DISR of register \a CLC.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_DisableClock()\n\n\n
 */
void XMC_DSD_DisableClock(XMC_DSD_t *const dsd);
/**
 * @param  dsd  Pointer to an instance of DSD module of type @ref XMC_DSD_t
 * @return bool Return 1 if success else 0.
 *
 * \par<b>Description</b><br>
 * Find out if the DSD reset is asserted.\n
 * Read \a PRSTAT0 register's \a DSDRS bit field.\n\n\n
 */
bool XMC_DSD_IsEnabled(XMC_DSD_t *const dsd);

/**
 * @param  dsd  Pointer to an instance of DSD module of type @ref XMC_DSD_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Enable the DSD module and clock.\n
 *
 * \par<b>Note</b><br>
 * This is the first API which application must invoke to configure DSD.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_Enable(),XMC_DSD_EnableClock()\n\n\n
 */
void XMC_DSD_Init(XMC_DSD_t *const dsd);
/**
 * @param  channel  Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t
 * @param  init     Pointer to an instance of data structure of type @ref XMC_DSD_CH_TIMESTAMP_CONFIG_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Initialize timestamp mode of DSD module with \a init.\n
 * Configures bits \a TRSEL and \a TSTRMODE of  register \a DICFG .
 *
 * \par<b>Note</b><br>
 * Trigger source for timestamp and integrator are shared.
 * It is internally called by XMC_DSD_CH_Init().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_CH_Init()\n\n\n
*/
void XMC_DSD_CH_Timestamp_Init(XMC_DSD_CH_t *const channel, const XMC_DSD_CH_TIMESTAMP_CONFIG_t *const init);

/**
 * @param  channel  Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t
 * @param  init     Pointer to an instance of data structure of type @ref XMC_DSD_CH_CONFIG_t
 * @return XMC_DSD_STATUS_t if success Returns @ref XMC_DSD_STATUS_OK
 *                          else return @ref XMC_DSD_STATUS_ERROR.
 *
 * \par<b>Description</b><br>
 * Initialize main filter,auxiliary filter,integrator, rectifier and timestamp of DSD module with \a init.\n
 * Internally this API invoke main filter, auxiliary filter, integrator, rectifier and timestamp init API.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_CH_MainFilter_Init(),XMC_DSD_CH_AuxFilter_Init(),XMC_DSD_CH_Integrator_Init(),
 * XMC_DSD_CH_Rectify_Init(),XMC_DSD_CH_Timestamp_Init()\n\n\n
*/
XMC_DSD_STATUS_t XMC_DSD_CH_Init(XMC_DSD_CH_t *const channel, const XMC_DSD_CH_CONFIG_t *const init);

/**
 * @param  dsd      Pointer to an instance of DSD module of type @ref XMC_DSD_t
 * @param  channel  Channel run control bit register value
 * @return None
 *
 * \par<b>Description</b><br>
 * Start demodulator channel by enabling run control bit.
 * Multiple channel can be start at a time.
 * For an example: To start all four channel, call this function as
 * XMC_DSD_Start(DSD,(uint32_t)(XMC_DSD_CH_ID_0|XMC_DSD_CH_ID_1|XMC_DSD_CH_ID_2|XMC_DSD_CH_ID_3));
 *
 * \par<b>Note</b><br>
 * All filter blocks are cleared when CHxRUN is set.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_Stop(),XMC_DSD_IsChannelStarted()\n\n\n
 */
__STATIC_INLINE void XMC_DSD_Start(XMC_DSD_t *const dsd, const uint32_t channel)
{
  XMC_ASSERT("XMC_DSD_Start:Invalid module pointer", XMC_DSD_CHECK_MODULE_PTR(dsd));
  dsd->GLOBRC |= channel;
}

/**
 * @param  dsd      Pointer to an instance of DSD module of type @ref XMC_DSD_t\n
 * @param  channel  Channel run control bit register value\n
 * @return None\n
 *
 * \par<b>Description</b><br>
 * Stop demodulator channel by resetting run control bit.
 * Multiple channel can be stop at a time.
 * For an example: To stop all four channel, call this function as
 * XMC_DSD_Stop(DSD,(uint32_t)(XMC_DSD_CH_ID_0|XMC_DSD_CH_ID_1|XMC_DSD_CH_ID_2|XMC_DSD_CH_ID_3));
 *
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_Start(),XMC_DSD_IsChannelStarted()\n\n\n
 */
__STATIC_INLINE void XMC_DSD_Stop(XMC_DSD_t *const dsd, const uint32_t channel)
{
  XMC_ASSERT("XMC_DSD_Stop:Invalid module pointer", XMC_DSD_CHECK_MODULE_PTR(dsd));
  dsd->GLOBRC &= (uint32_t) ~channel;
}

/**
 * @param  dsd     Pointer to an instance of DSD module of type @ref XMC_DSD_t\n
 * @param  channel Channel run control bit register value of type @ref XMC_DSD_CH_ID_t\n
 * @return bool    Return 1 if started else 0.\n
 *
 * \par<b>Description</b><br>
 * Find out if particular demodulator channel is started or not.\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_Start(),XMC_DSD_Stop()\n\n\n
 */
__STATIC_INLINE bool XMC_DSD_IsChannelStarted(XMC_DSD_t *const dsd, const XMC_DSD_CH_ID_t channel)
{
  bool status;
  XMC_ASSERT("XMC_DSD_IsChannelStarted:Invalid module pointer", XMC_DSD_CHECK_MODULE_PTR(dsd));
  if (dsd->GLOBRC & (uint32_t)channel)
  {
    status = true;
  }
  else
  {
    status = false;
  }
  return (status);
}

/**
 * @param  channel Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t
 * @param  dsd_Result pointer point to the address of 16 bit variable
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Returns the result of most recent conversion associated with this channel.\n
 *  A call to this API would access the register bit field \a RESMx.RESULT (x = 0 - 3).
 *
 * \par<b>Related APIs:</b><br>
 * XMC_DSD_CH_GetResult_TS()\n\n\n
 */
__STATIC_INLINE void XMC_DSD_CH_GetResult(XMC_DSD_CH_t *const channel, int16_t* dsd_Result) 
{
  uint16_t result;
  result = (uint16_t)((uint32_t)channel->RESM & DSD_CH_RESM_RESULT_Msk);
  *dsd_Result = (int16_t)result;
}

/**
 * @param  channel              Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t\n
 * @param  dsd_Result           pointer point to the address of 16 bit variable holds result value\n
 * @param  dsd_filter_loop      pointer point to the address of 8 bit variable holds decimation counter value\n
 * @param  dsd_integration_loop pointer point to the address of 8 bit integration counter variable holds value\n
 * @return None\n
 *
 * \par<b>Description:</b><br>
 *  API to get the result of the last conversion associated with this channel with
 *  CIC filter decimation counter and number of values counted.\n
 *  A call to this API would access the register bit field \a TSTMPx.RESULT \a TSTMPx.CFMDCNT and \a TSTMPx.NVALCNT where (x = 0 - 3).
 *
 * \par<b>Related APIs:</b><br>
 * XMC_DSD_CH_GetResult(),XMC_DSD_CH_GetResult_TS_Time()\n\n\n
 */
void XMC_DSD_CH_GetResult_TS(
                             XMC_DSD_CH_t *const channel,
                             int16_t* dsd_Result,
                             uint8_t* dsd_filter_loop,
                             uint8_t* dsd_integration_loop);

/**
 * @param  channel    Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t\n
 * @param  dsd_Result pointer point to the address of 16 bit variable holds result value\n
 * @param  time       pointer point to the address of 32 bit variable holds the time\n
 * @return None
 *
 * \par<b>Description:</b><br>
 *  API to get the result of the last conversion with the time, associated with this channel.\n
 *
 * \par<b>Related APIs:</b><br>
 * XMC_DSD_CH_GetResult(),XMC_DSD_CH_GetResult_TS()\n\n\n
 */
void XMC_DSD_CH_GetResult_TS_Time(XMC_DSD_CH_t *const channel, int16_t* dsd_Result, uint32_t* time);

/**
 * @param  channel Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t
 * @param  dsd_Result pointer point to the address of 16 bit variable
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Returns the most recent conversion result values of the auxiliary filter associated with this channel.\n
 *  A call to this API would access the register bit field \a RESAx.RESULT (x = 0 - 3).
 *
 * \par<b>Related APIs:</b><br>
 * XMC_DSD_CH_GetResult_TS()\n\n\n
 */
__STATIC_INLINE void XMC_DSD_CH_GetResult_AUX(XMC_DSD_CH_t *const channel, int16_t* dsd_Result)
{
  uint16_t result;
  result = (uint16_t) (channel->RESA & DSD_CH_RESA_RESULT_Msk);
  *dsd_Result = (int16_t) (result);
}

/**
 * @param  dsd  Pointer to an instance of DSD module of type @ref XMC_DSD_t
 * @param  init Pointer to an instance of data structure of type @ref XMC_DSD_GENERATOR_CONFIG_t
 * @return None
 *
 * \par<b>Description</b><br>
 * This API initialize generator module with \a init, to generate a wave for a resolver.
 * Three types of waveforms can be generated: Rectangle, Triangle and Sine.
 *
*/
void XMC_DSD_Generator_Init(XMC_DSD_t *const dsd, const XMC_DSD_GENERATOR_CONFIG_t *const init);

/**
 * @param  dsd     Pointer to an instance of DSD module of type @ref XMC_DSD_t
 * @param  config  Pointer to an instance of data structure of type @ref XMC_DSD_GENERATOR_CONFIG_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Start carrier generator by configuring operating mode.
 * Configures bit field \a CGMOD of register \a CGCFG.
 *
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_Generator_Stop()\n\n\n
 */
__STATIC_INLINE void XMC_DSD_Generator_Start(XMC_DSD_t *const dsd, const XMC_DSD_GENERATOR_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_DSD_Generator_Start:Invalid module pointer", XMC_DSD_CHECK_MODULE_PTR(dsd));
  XMC_ASSERT("XMC_DSD_Generator_Start:NULL Pointer", (config != (XMC_DSD_GENERATOR_CONFIG_t *)NULL) );
  dsd->CGCFG |= config->mode;
}

/**
 * @param  dsd     Pointer to an instance of DSD module of type @ref XMC_DSD_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Stop carrier generator by configuring operating mode.
 * Reset bit field \a CGMOD of register \a CGCFG.
 *
 * \par<b>Note</b><br>
 * Stopping the carrier generator terminates the PWM output after completion of the current period.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_Generator_Start()\n\n\n
 */
__STATIC_INLINE void XMC_DSD_Generator_Stop(XMC_DSD_t *const dsd)
{
  XMC_ASSERT("XMC_DSD_Generator_Stop:Invalid module pointer", XMC_DSD_CHECK_MODULE_PTR(dsd));
  dsd->CGCFG &= ~((uint32_t)DSD_CGCFG_CGMOD_Msk);
}

/**
 * @param  channel  Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t
 * @param  init     Pointer to an instance of data structure of type @ref XMC_DSD_CH_FILTER_CONFIG_t
 * @return None
 *
 * \par<b>Description</b><br>
 * This API initialize main filter module with \a init.
 * The filter demodulates the incoming bit stream from the delta sigma modulator to a 16 bit result.
 *
 *
 * \par<b>Note</b><br>
 * It is internally called by XMC_DSD_CH_Init().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_CH_Init()\n\n\n
*/
void XMC_DSD_CH_MainFilter_Init(XMC_DSD_CH_t *const channel, const XMC_DSD_CH_FILTER_CONFIG_t *const init);

/**
 * @param  channel  Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t
 * @param  offset   Signed Offset value to be set
 * @return None
 *
 * \par<b>Description</b><br>
 * API set the signed offset value for this channel.
 * This offset value is subtracted from each result before being written to the corresponding result register \a RESMx.
 *
 * \par<b>Note</b><br>
 * The offset value is measured for each channel separately.
 *
*/
__STATIC_INLINE void XMC_DSD_CH_MainFilter_SetOffset(XMC_DSD_CH_t *const channel, const int16_t offset)
{
  XMC_ASSERT("XMC_DSD_CH_MainFilter_SetOffset:Invalid module pointer", XMC_DSD_CHECK_CHANNEL_PTR(channel));
  channel->OFFM = (uint32_t)offset;
}

/**
 * @param  channel  Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t
 * @return None
 *
 * \par<b>Description</b><br>
 * This API enable the service request generation for result of this channel.
 * Result events are generated at the output rate of the configured main filter chain.
 * Configure bit field \a SRGM of register \a FCFGC
 *
*/
__STATIC_INLINE void XMC_DSD_CH_MainFilter_EnableEvent(XMC_DSD_CH_t *const channel)
{
  XMC_ASSERT("XMC_DSD_CH_MainFilter_EnableEvent:Invalid module pointer", XMC_DSD_CHECK_CHANNEL_PTR(channel));
  channel->FCFGC |= (uint32_t)DSD_CH_FCFGC_SRGM_Msk;
}

/**
 * @param  channel  Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t
 * @return None
 *
 * \par<b>Description</b><br>
 * This API disable the service request generation for result of this channel.
 * Configure bit field \a SRGM of register \a FCFGC
 *
*/
__STATIC_INLINE void XMC_DSD_CH_MainFilter_DisableEvent(XMC_DSD_CH_t *const channel)
{
  XMC_ASSERT("XMC_DSD_CH_MainFilter_DisableEvent:Invalid module pointer", XMC_DSD_CHECK_CHANNEL_PTR(channel));
  channel->FCFGC &= ~((uint32_t)DSD_CH_FCFGC_SRGM_Msk);
}

/**
 * @param  channel  Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t
 * @param  init     Pointer to an instance of data structure of type @ref XMC_DSD_CH_AUX_FILTER_CONFIG_t
 * @return None
 *
 * \par<b>Description</b><br>
 * This API initialize auxiliary filter module with \a init. The auxiliary Filter is mainly used as fast filter.\n
 * Adding the auxiliary filter to the system structure helps by defining the boundaries and filter configurations.\n
 *
 *
 * \par<b>Note</b><br>
 * It is internally called by XMC_DSD_CH_Init().\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_CH_Init()\n\n\n
*/
void XMC_DSD_CH_AuxFilter_Init(XMC_DSD_CH_t *const channel, const XMC_DSD_CH_AUX_FILTER_CONFIG_t *const init);

/**
 * @param  channel          Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t
 * @param  lower_boundary   lower boundary value for limit checking
 * @param  upper_boundary   upper boundary value for limit checking
 * @return None
 *
 * \par<b>Description</b><br>
 * Invoke this API, to set the lower and upper boundary for limit checking for this channel.
 * This (two’s complement) value is compared to the results of the parallel filter.
 * Configure bit fields \a BOUNDARYU and \a BOUNDARYL of register \a BOUNDSEL.
 *
 * \par<b>Note</b><br>
 * Lower and upper boundaries are internally configured by function XMC_DSD_CH_AuxFilter_Init().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_CH_AuxFilter_Init()\n\n\n
*/
__STATIC_INLINE void XMC_DSD_CH_AuxFilter_SetBoundary(
                                                   XMC_DSD_CH_t *const channel,
                                                   const int16_t lower_boundary,
                                                   const int16_t upper_boundary)
{
  XMC_ASSERT("XMC_DSD_CH_AuxFilter_SetBoundary:Invalid module pointer", XMC_DSD_CHECK_CHANNEL_PTR(channel));
  channel->BOUNDSEL = (((uint32_t)upper_boundary << (uint32_t)DSD_CH_BOUNDSEL_BOUNDARYU_Pos)
                      | ((uint32_t)lower_boundary & (uint32_t)DSD_CH_BOUNDSEL_BOUNDARYL_Msk));
}

/**
 * @param  channel  Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t
 * @param  event to select the type of event which will be enabled of type @ref XMC_DSD_CH_AUX_EVENT_t
 * @return None
 *
 * \par<b>Description</b><br>
 * This API enable the service request generation for this channel.
 * Result events are generated at the output rate of the configured auxiliary filter chain.
 * Configure bit field \a SRGA and ESEL of register \a FCFGA
 *
*/
__STATIC_INLINE void XMC_DSD_CH_AuxFilter_EnableEvent(XMC_DSD_CH_t *const channel, XMC_DSD_CH_AUX_EVENT_t event)
{
  XMC_ASSERT("XMC_DSD_CH_AuxFilter_EnableEvent:Invalid module pointer", XMC_DSD_CHECK_CHANNEL_PTR(channel));
  channel->FCFGA &= ~((uint32_t)DSD_CH_FCFGA_ESEL_Msk|(uint32_t)DSD_CH_FCFGA_SRGA_Msk);
  channel->FCFGA |= ((uint32_t)event << DSD_CH_FCFGA_SRGA_Pos);
}

/**
 * @param  channel  Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t
 * @return None
 *
 * \par<b>Description</b><br>
 * This API disable the auxiliary filter service request generation for this channel.
 * Clear the bit fields \a SRGA and ESEL of register \a FCFGA.
 *
*/
__STATIC_INLINE void XMC_DSD_CH_AuxFilter_DisableEvent(XMC_DSD_CH_t *const channel)
{
  XMC_ASSERT("XMC_DSD_CH_AuxFilter_DisableEvent:Invalid module pointer", XMC_DSD_CHECK_CHANNEL_PTR(channel));
  channel->FCFGA &= ~((uint32_t)DSD_CH_FCFGA_ESEL_Msk|(uint32_t)DSD_CH_FCFGA_SRGA_Msk);
}

/**
 * @param  channel  Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t
 * @param  init     Pointer to an instance of data structure of type @ref XMC_DSD_CH_INTEGRATOR_CONFIG_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Initialize integrator of DSD module.The integrator is mainly used for resolver feedback but can also be used for high accurate measurement.
 * This API configures number of integration loops, number of results are discarded before integration starts, integrator stop condition,
 * number of integrator loop to get integration result and trigger mode.
 *
 * \par<b>Note</b><br>
 * Trigger source for timestamp and integrator are shared.
 * It is internally called by XMC_DSD_CH_Init().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_CH_Init()\n\n\n
*/
void XMC_DSD_CH_Integrator_Init(XMC_DSD_CH_t *const channel, const XMC_DSD_CH_INTEGRATOR_CONFIG_t *const init);
/**
 * @param  channel  Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t
 * @param  init     Pointer to an instance of data structure of type @ref XMC_DSD_CH_RECTIFY_CONFIG_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Initialize rectification for this channel.
 * In a resolver feedback system, rectifier is used to rectify the result from the integrator.
 * Configure bit field \a RFEN and \a SSRC of register \a RECTCFG. Also configure sign delay
 * value for positive halfwave(\a SDPOS) and negative halfwave(\a SDNEG).
 * \par<b>Note</b><br>
 * For the operational capability of rectifier the filter and the integrator is mandatory.
 * It is internally called by XMC_DSD_CH_Init().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_CH_Init()\n\n\n
*/
void XMC_DSD_CH_Rectify_Init(XMC_DSD_CH_t *const channel, const XMC_DSD_CH_RECTIFY_CONFIG_t *const init);

/**
 * @param  channel  Pointer to an instance of DSD channel of type @ref XMC_DSD_CH_t
 * @param  delay    Captured value
 * @return uint8_t
 *
 * \par<b>Description</b><br>
 * This API, capture sign delay value for DSD channel.
 * Captured value indicates the values counted between the begin of the positive
 * halfwave of the carrier signal and the first received positive value.
 * Read bit field \a SDCAP of register \a CGSYNC
 *
*/
__STATIC_INLINE void XMC_DSD_CH_GetRectifyDelay(XMC_DSD_CH_t *const channel, uint8_t* delay)
{
  XMC_ASSERT("XMC_DSD_CH_GetRectifyDelay:Invalid module pointer", XMC_DSD_CHECK_CHANNEL_PTR(channel));
  *delay = (uint8_t)((channel->CGSYNC & DSD_CH_CGSYNC_SDCAP_Msk ) >> DSD_CH_CGSYNC_SDCAP_Pos);
}
/**
 * @param  dsd  Pointer to an instance of DSD module of type @ref XMC_DSD_t
 * @param  channel_id   Channel number register value of type @ref XMC_DSD_CH_ID_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Set the result event flag and trigger the corresponding event.\n
 * Set bit fields \a RESEVx of register \a EVFLAG.Clearing these bits has no effect.
 *
 * \par<b>Note</b><br>
 * API call for channel-0  : XMC_DSD_SetResultEventFlag(DSD,(uint32_t)XMC_DSD_CH_ID_0);
 * API call for channel-0 and 1: XMC_DSD_SetResultEventFlag(DSD,(uint32_t)(XMC_DSD_CH_ID_0|XMC_DSD_CH_ID_1));
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_ClearResultEventFlag()\n\n\n
 */
__STATIC_INLINE void XMC_DSD_SetResultEventFlag(XMC_DSD_t *const dsd, const uint32_t channel_id)
{
  XMC_ASSERT("XMC_DSD_SetResultEventFlag:Invalid module pointer", XMC_DSD_CHECK_MODULE_PTR(dsd));
  dsd->EVFLAG = channel_id;
}
/**
 * @param  dsd  Pointer to an instance of DSD module of type @ref XMC_DSD_t
 * @param  channel_id   Channel number register value of type @ref XMC_DSD_CH_ID_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Clear the result event flag.\n
 * Set bit fields \a RESECx of register \a EVFLAGCLR.Clearing these bits has no effect.
 *
 * \par<b>Note</b><br>
 * API call for channel-0      : XMC_DSD_ClearResultEventFlag(DSD,(uint32_t)XMC_DSD_CH_ID_0);
 * API call for channel-0 and 1: XMC_DSD_ClearResultEventFlag(DSD,(uint32_t)(XMC_DSD_CH_ID_0|XMC_DSD_CH_ID_1));
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_SetResultEventFlag()\n\n\n
 */
__STATIC_INLINE void XMC_DSD_ClearResultEventFlag(XMC_DSD_t *const dsd, const uint32_t channel_id)
{
  XMC_ASSERT("XMC_DSD_ClearResultEventFlag:Invalid module pointer", XMC_DSD_CHECK_MODULE_PTR(dsd));
  dsd->EVFLAGCLR = channel_id;
}
/**
 * @param  dsd  Pointer to an instance of DSD module of type @ref XMC_DSD_t
 * @param  channel_id   Channel number register value of type @ref XMC_DSD_CH_ID_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Set the alarm event flag.\n
 * Set bit fields \a ALEVx of register \a EVFLAG.Clearing these bits has no effect.
 *
 * \par<b>Note</b><br>
 * API call for channel-0  : XMC_DSD_SetAlarmEventFlag(DSD,(uint32_t)XMC_DSD_CH_ID_0);
 * API call for channel-0 and 1: XMC_DSD_SetAlarmEventFlag(DSD,(uint32_t)(XMC_DSD_CH_ID_0|XMC_DSD_CH_ID_1));
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_ClearAlarmEventFlag()\n\n\n
 */
__STATIC_INLINE void XMC_DSD_SetAlarmEventFlag(XMC_DSD_t *const dsd, const uint32_t channel_id)
{
  XMC_ASSERT("XMC_DSD_SetAlarmEventFlag:Invalid module pointer", XMC_DSD_CHECK_MODULE_PTR(dsd));
  dsd->EVFLAG = (channel_id<< DSD_EVFLAGCLR_ALEC0_Pos);
}
/**
 * @param  dsd  Pointer to an instance of DSD module of type @ref XMC_DSD_t
 * @param  channel_id   Channel number register value of type @ref XMC_DSD_CH_ID_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Clear the result event flag.\n
 * Set bit fields \a ALECx of register \a EVFLAGCLR.Clearing these bits has no effect.
 *
 * \par<b>Note</b><br>
 * API call for channel-0      : XMC_DSD_ClearResultEventFlag(DSD,(uint32_t)XMC_DSD_CH_ID_0);
 * API call for channel-0 and 1: XMC_DSD_ClearResultEventFlag(DSD,(uint32_t)(XMC_DSD_CH_ID_0|XMC_DSD_CH_ID_1));
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_DSD_SetAlarmEventFlag()\n\n\n
 */
__STATIC_INLINE void XMC_DSD_ClearAlarmEventFlag(XMC_DSD_t *const dsd, const uint32_t channel_id)
{
  XMC_ASSERT("XMC_DSD_ClearAlarmEventFlag:Invalid module pointer", XMC_DSD_CHECK_MODULE_PTR(dsd));
  dsd->EVFLAGCLR = (channel_id<<DSD_EVFLAGCLR_ALEC0_Pos);
}
#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * @}
 */


#endif /* DSD_PRESENT */
#endif /*XMC_DSD_H */
