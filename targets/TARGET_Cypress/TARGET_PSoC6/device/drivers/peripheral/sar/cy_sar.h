/***************************************************************************//**
* \file cy_sar.h
* \version 1.0
*
* Header file for the SAR driver.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/**
* \defgroup group_sar SAR ADC Subsystem (SAR)
* \{
* This driver configures and controls the SAR ADC subsystem block.
*
* This SAR ADC subsystem is comprised of:
*   - a 12-bit SAR converter (SARADC)
*   - an embedded reference buffer (REFBUF)
*   - a mux (SARMUX) at the inputs of the converter
*   - a sequence controller (SARSEQ) that enables multi-channel acquisition
*       in a round robin fashion, without CPU intervention, to maximize scan rates.
*
* \image html sar_block_diagram.png
*
* The high level features of the subsystem are:
*   - maximum sample rate of 1 Msps
*   - multiple individually configurable channels (8, 12, or 16 depending on the device)
*   - per channel selectable
*       - choose one of four programmable acquisition times
*       - single-ended and differential input mode
*       - averaging and accumulation
*   - scan can be triggered by firmware or hardware in single shot or continuous mode
*   - hardware averaging from 2 to 256 samples
*   - selectable voltage references (Vdda, Vdda/2, analog reference, or external)
*   - Interrupt generation
*
* \section group_sar_usage Usage
*
* This section will go through the high level steps of using this driver with some example code.
*
* High level usage flow:
*
*   - Initialize the hardware
*   - Enable the hardware
*   - Trigger conversions
*   - Wait for conversion to complete
*   - Retrieve channel results
*
* <b> Initialization </b>
*
* To configure the SAR subsystem, call \ref Cy_SAR_Init. Pass in a pointer to the \ref SAR_Type
* structure for the base hardware register address and pass in the configuration structure,
* \ref cy_stc_sar_config_t. This initializes the SAR subsystem.
*
* <b> Enable </b>
*
* If the \ref cy_stc_sar_config_t.enable field of the configuration structure is set to true
* during initialization, the hardware will be enabled after initialization.
* If set to false, a call to \ref Cy_SAR_Enable is required to enable the hardware.
*
* <b> Triggering conversions </b>
*
* The SAR subsystem has the following modes for triggering a conversion:
* <table class="doxtable">
*   <tr>
*     <th>Mode</th>
*     <th>Description</th>
*     <th>Usage</th>
*   </tr>
*   <tr>
*     <td>Continuous</td>
*     <td>After completing a scan, the
*     SARSEQ will immediately start the next scan. That is, the SARSEQ will always be BUSY.
*     As a result all other triggers, firmware or hardware, are essentially ignored.
*  </td>
*     <td>To enter this mode, call \ref Cy_SAR_StartConvert with \ref CY_SAR_START_CONVERT_CONTINUOUS.</td>
*   </tr>
*   <tr>
*     <td>Firmware single shot</td>
*     <td>A single conversion of all enabled channels is triggered with a function call to \ref Cy_SAR_StartConvert with
*     \ref CY_SAR_START_CONVERT_SINGLE_SHOT.
*     </td>
*     <td>
*     Firmware triggering is always available by calling \ref Cy_SAR_StartConvert with \ref CY_SAR_START_CONVERT_SINGLE_SHOT.
*     To allow only firmware triggering, or disable
*     hardware triggering, set up the \ref cy_stc_sar_config_t config structure with \ref CY_SAR_TRIGGER_MODE_FW_ONLY.
*     </td>
*   </tr>
*   <tr>
*     <td>Hardware edge sensitive</td>
*     <td>A single conversion of all enabled channels is triggered on the rising edge of the hardware
*     trigger signal.</td>
*     <td>To enable this mode, set up the \ref cy_stc_sar_config_t config structure with
*     \ref CY_SAR_TRIGGER_MODE_FW_AND_HWEDGE.</td>
*    </tr>
*    <tr>
*     <td>Hardware level sensitive</td>
*     <td>Conversions are triggered continuously when the hardware trigger signal is high.</td>
*     <td>To enable this mode, set up the \ref cy_stc_sar_config_t config structure with
*     \ref CY_SAR_TRIGGER_MODE_FW_AND_HWLEVEL.</td>
*    </tr>
* </table>

* The trigger mode can be changed during run time with \ref Cy_SAR_SetConvertMode.
*
* Refer to \ref group_trigmux to configure the multiplexer to route a trigger signal to the SAR trigger input.
* When making the required \ref Cy_TrigMux_Connect calls, use the pre-defined enum, TRIG6_OUT_PASS_TR_SAR_IN,
* for the SAR trigger input.
*
* <b> Handling Interrupts </b>
*
* The driver has a function, \ref Cy_SAR_IsEndConversion, that can query the conversion
* status or wait in blocking mode until the conversion is complete. Alternatively,
* implement an interrupt routine and assign it to the SAR interrupt.
* Use the pre-defined enum, pass_interrupt_sar_IRQn, as the interrupt source for the SAR.
*
* See \ref group_sar_code_example for example code.
*
* <b> Getting ADC result </b>
*
* Retrieve the ADC result by calling \ref Cy_SAR_GetResult16 with the desired channel.
* For a voltage value, pass the ADC result to \ref Cy_SAR_CountsTo_Volts, \ref Cy_SAR_CountsTo_mVolts, or
* \ref Cy_SAR_CountsTo_uVolts.
*
* \section group_sar_clock SAR Clock Configuration
*
* Assign a clock to the SAR and set the divider value with \ref Cy_SysClk_PeriphAssignDivider.
* The pre-defined enum, PCLK_PASS_CLOCK_SAR, should be used to identify the SAR subsystem.
* Set the clock divider value to achieve the desired clock rate. The maximum frequency is 18 MHz.
*
* See \ref group_sar_code_example for example code.
*
* \section group_sar_code_example Complete Code Example
*
* \code
*   #include "sar/cy_sar.h"
*   #include "sysanalog/cy_sysanalog.h"
*   #include "sysint/cy_sysint.h"
* \endcode
* \snippet sar_sut_01.cydsn/main_cm0p.c SAR_CODE_EXAMPLE
*
* \section group_sar_more_information More Information
* For more information on the SAR ADC subsystem, refer to the technical reference manual (TRM).
*
* \section group_sar_MISRA MISRA-C Compliance]
*
* This driver has the following specific deviations:
*
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>11.4</td>
*     <td>Advisory</td>
*     <td>A cast should not be performed between a pointer to object type and a different pointer to object type.</td>
*     <td>The cy_syspm driver defines the pointer to void in the \ref cy_stc_syspm_callback_params_t.base field.
*       This SAR driver implements a Deep Sleep callback conforming to the cy_syspm driver requirements.
*       When the callback is called, the base should point to the SAR_Type register address.
*     </td>
*   </tr>
* </table>
*
* \section group_sar_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_sar_macros Macros
* \defgroup group_sar_functions Functions
*   \{
*       \defgroup group_sar_functions_basic         Initialization and Basic Functions
*       \defgroup group_sar_functions_power         Power Mode Functions
*       \defgroup group_sar_functions_config        Run-time Configuration Functions
*       \defgroup group_sar_functions_countsto      Counts Conversion Functions
*       \defgroup group_sar_functions_interrupt     Interrupt Functions
*       \defgroup group_sar_functions_switches      SARMUX Switch Control Functions
*       \defgroup group_sar_functions_helper        Useful Configuration Query Functions
*   \}
* \defgroup group_sar_globals Global Variables
* \defgroup group_sar_data_structures Data structures
* \defgroup group_sar_enums Enumerated Types
*   \{
*       \defgroup group_sar_ctrl_register_enums         Control Register Enums
*       \defgroup group_sar_sample_ctrl_register_enums  Sample Control Register Enums
*       \defgroup group_sar_sample_time_shift_ts          Sample Time Configuration Registers Enums
*       \defgroup group_sar_range_thres_register_enums  Range Configuration Registers Enums
*       \defgroup group_sar_chan_config_register_enums  Channel Configuration Registers Enums
*       \defgroup group_sar_intr_mask_t_register_enums    Interrupt Mask Register Enum
*       \defgroup group_sar_mux_switch_register_enums   SARMUX Switch Control Registers Enums
*   \}
*/

#if !defined(CY_SAR_H)
#define CY_SAR_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "cy_device_headers.h"
#include "syslib/cy_syslib.h"
#include "syspm/cy_syspm.h"

#ifndef CY_IP_MXS40PASS_SAR
    #error "The SAR driver is not supported on this device"
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/** \addtogroup group_sar_macros
* \{
*/

/** Driver major version */
#define CY_SAR_DRV_VERSION_MAJOR        1

/** Driver minor version */
#define CY_SAR_DRV_VERSION_MINOR        0

/** SAR driver identifier */
#define CY_SAR_ID                       CY_PDL_DRV_ID(0x01u)

/** Maximum number of channels */
#define CY_SAR_MAX_NUM_CHANNELS         (PASS_SAR_SAR_CHANNELS)

/** \cond INTERNAL */
#define CY_SAR_DEINIT                   (0uL)             /**< De-init value for most SAR registers */
#define CY_SAR_SAMPLE_TIME_DEINIT       ((3uL << SAR_SAMPLE_TIME01_SAMPLE_TIME0_Pos) | (3uL << SAR_SAMPLE_TIME01_SAMPLE_TIME1_Pos))  /**< De-init value for the SAMPLE_TIME* registers */
#define CY_SAR_CLEAR_ALL_SWITCHES       (0x3FFFFFFFuL)    /**< Value to clear all SARMUX switches */
#define CY_SAR_DEINIT_SQ_CTRL           (SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P0_Msk \
                                        | SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P1_Msk \
                                        | SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P2_Msk \
                                        | SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P3_Msk \
                                        | SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P4_Msk \
                                        | SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P5_Msk \
                                        | SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P6_Msk \
                                        | SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P7_Msk \
                                        | SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_VSSA_Msk \
                                        | SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_TEMP_Msk \
                                        | SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_AMUXBUSA_Msk \
                                        | SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_AMUXBUSB_Msk \
                                        | SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_SARBUS0_Msk \
                                        | SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_SARBUS1_Msk)
#define CY_SAR_REG_CTRL_MASK            (SAR_CTRL_PWR_CTRL_VREF_Msk \
                                        | SAR_CTRL_VREF_SEL_Msk \
                                        | SAR_CTRL_VREF_BYP_CAP_EN_Msk \
                                        | SAR_CTRL_NEG_SEL_Msk \
                                        | SAR_CTRL_SAR_HW_CTRL_NEGVREF_Msk \
                                        | SAR_CTRL_COMP_DLY_Msk \
                                        | SAR_CTRL_REFBUF_EN_Msk \
                                        | SAR_CTRL_COMP_PWR_Msk \
                                        | SAR_CTRL_DEEPSLEEP_ON_Msk \
                                        | SAR_CTRL_DSI_SYNC_CONFIG_Msk \
                                        | SAR_CTRL_DSI_MODE_Msk \
                                        | SAR_CTRL_SWITCH_DISABLE_Msk \
                                        | SAR_CTRL_ENABLED_Msk)
#define CY_SAR_REG_SAMPLE_CTRL_MASK     (SAR_SAMPLE_CTRL_LEFT_ALIGN_Msk \
                                        | SAR_SAMPLE_CTRL_SINGLE_ENDED_SIGNED_Msk \
                                        | SAR_SAMPLE_CTRL_DIFFERENTIAL_SIGNED_Msk \
                                        | SAR_SAMPLE_CTRL_AVG_CNT_Msk \
                                        | SAR_SAMPLE_CTRL_AVG_SHIFT_Msk \
                                        | SAR_SAMPLE_CTRL_AVG_MODE_Msk \
                                        | SAR_SAMPLE_CTRL_CONTINUOUS_Msk \
                                        | SAR_SAMPLE_CTRL_DSI_TRIGGER_EN_Msk \
                                        | SAR_SAMPLE_CTRL_DSI_TRIGGER_LEVEL_Msk \
                                        | SAR_SAMPLE_CTRL_DSI_SYNC_TRIGGER_Msk \
                                        | SAR_SAMPLE_CTRL_UAB_SCAN_MODE_Msk \
                                        | SAR_SAMPLE_CTRL_REPEAT_INVALID_Msk \
                                        | SAR_SAMPLE_CTRL_VALID_SEL_Msk \
                                        | SAR_SAMPLE_CTRL_VALID_SEL_EN_Msk \
                                        | SAR_SAMPLE_CTRL_VALID_IGNORE_Msk \
                                        | SAR_SAMPLE_CTRL_TRIGGER_OUT_EN_Msk \
                                        | SAR_SAMPLE_CTRL_EOS_DSI_OUT_EN_Msk)
#define CY_SAR_REG_CHAN_CONFIG_MASK     (SAR_CHAN_CONFIG_POS_PIN_ADDR_Msk \
                                        | SAR_CHAN_CONFIG_POS_PORT_ADDR_Msk \
                                        | SAR_CHAN_CONFIG_DIFFERENTIAL_EN_Msk \
                                        | SAR_CHAN_CONFIG_AVG_EN_Msk \
                                        | SAR_CHAN_CONFIG_SAMPLE_TIME_SEL_Msk \
                                        | SAR_CHAN_CONFIG_NEG_PIN_ADDR_Msk \
                                        | SAR_CHAN_CONFIG_NEG_PORT_ADDR_Msk \
                                        | SAR_CHAN_CONFIG_NEG_ADDR_EN_Msk \
                                        | SAR_CHAN_CONFIG_DSI_OUT_EN_Msk)
#define CY_SAR_REG_SAMPLE_TIME_MASK     (SAR_SAMPLE_TIME01_SAMPLE_TIME0_Msk | SAR_SAMPLE_TIME01_SAMPLE_TIME1_Msk)

#define CY_SAR_2US_DELAY                (2u)              /**< Delay used in Enable API function to avoid SAR deadlock */
#define CY_SAR_10V_COUNTS               (10.0F)           /**< Value of 10 in volts */
#define CY_SAR_10MV_COUNTS              (10000)           /**< Value of 10 in millivolts */
#define CY_SAR_10UV_COUNTS              (10000000L)       /**< Value of 10 in microvolts */
#define CY_SAR_WRK_MAX_12BIT            (0x00001000uL)    /**< Maximum SAR work register value for a 12-bit resolution */
#define CY_SAR_LIMIT_MAX                (0xFFFFuL)        /**< Maximum value for the low and high limit values */

/**< Macros for conditions used in CY_ASSERT calls */
#define CY_SAR_CHANNUM(chan)            ((chan) < CY_SAR_MAX_NUM_CHANNELS)
#define CY_SAR_CHANMASK(mask)           ((mask) < (1uL << CY_SAR_MAX_NUM_CHANNELS))
#define CY_SAR_LIMITCOND(cond)          ((cond) <= CY_SAR_LIMIT_COND_OUTSIDE)
#define CY_SAR_INTRMASK(mask)           ((mask) <= (uint32_t)(CY_SAR_INTR_EOS_MASK | CY_SAR_INTR_OVERFLOW_MASK | CY_SAR_INTR_FW_COLLISION_MASK))
#define CY_SAR_TRIGGER(mode)            (((mode) == CY_SAR_TRIGGER_MODE_FW_ONLY) \
                                        || ((mode) == CY_SAR_TRIGGER_MODE_FW_AND_HWEDGE) \
                                        || ((mode) == CY_SAR_TRIGGER_MODE_FW_AND_HWLEVEL))
#define CY_SAR_RETURN(mode)             (((mode) == CY_SAR_RETURN_STATUS) || ((mode) == CY_SAR_WAIT_FOR_RESULT))
#define CY_SAR_STARTCONVERT(mode)       (((mode) == CY_SAR_START_CONVERT_SINGLE_SHOT) || ((mode) == CY_SAR_START_CONVERT_CONTINUOUS))
#define CY_SAR_LIMIT(limit)             ((limit) <= CY_SAR_LIMIT_MAX)
#define CY_SAR_SWITCHSELECT(select)     ((select) == CY_SAR_MUX_SWITCH0)
#define CY_SAR_SWITCHMASK(mask)         ((mask) <= CY_SAR_CLEAR_ALL_SWITCHES)
#define CY_SAR_SWITCHSTATE(state)       (((state) == CY_SAR_SWITCH_OPEN) || ((state) == CY_SAR_SWITCH_CLOSE))
#define CY_SAR_SQMASK(mask)             (((mask) & (~CY_SAR_DEINIT_SQ_CTRL)) == 0uL)
#define CY_SAR_SQCTRL(ctrl)             (((ctrl) == CY_SAR_SWITCH_SEQ_CTRL_ENABLE) || ((ctrl) == CY_SAR_SWITCH_SEQ_CTRL_DISABLE))

#define CY_SAR_CTRL(value)              (((value) & (~CY_SAR_REG_CTRL_MASK)) == 0uL)
#define CY_SAR_SAMPLE_CTRL(value)       (((value) & (~CY_SAR_REG_SAMPLE_CTRL_MASK)) == 0uL)
#define CY_SAR_SAMPLE_TIME(value)       (((value) & (~CY_SAR_REG_SAMPLE_TIME_MASK)) == 0uL)
#define CY_SAR_CHAN_CONFIG(value)       (((value) & (~CY_SAR_REG_CHAN_CONFIG_MASK)) == 0uL)
/** \endcond */

/** \} group_sar_macro */

/** \addtogroup group_sar_globals
* \{
*/
/***************************************
*        Global Variables
***************************************/

/** This array is used to calibrates the offset for each channel.
*
* At initialization, the offset array's entries are set to 0, except for channels
* that are Single-Ended, Signed, and have Vneg=Vref, for which they are set to
* -2^(Resolution-1)/Vref(mV). These can be modified using \ref Cy_SAR_SetOffset. The array
* is used by the \ref Cy_SAR_CountsTo_Volts, \ref Cy_SAR_CountsTo_mVolts, and
* \ref Cy_SAR_CountsTo_uVolts functions.
*/
extern volatile int16_t Cy_SAR_offset[CY_SAR_MAX_NUM_CHANNELS];

/** This array is used to calibrate the gain for each channel.
*
* It is set at initialization and the value depends on voltage reference.
* It can be changed using \ref Cy_SAR_SetGain.
*
* The array is used by the \ref Cy_SAR_CountsTo_Volts, \ref Cy_SAR_CountsTo_mVolts, and
* \ref Cy_SAR_CountsTo_uVolts functions.
*/
extern volatile int32_t Cy_SAR_countsPer10Volt[CY_SAR_MAX_NUM_CHANNELS];

/** \} group_sar_globals */

/** \addtogroup group_sar_enums
* \{
*/

/******************************************************************************
 * Enumerations
 *****************************************************************************/

/** The SAR status/error code definitions */
typedef enum
{
    CY_SAR_SUCCESS    = 0x00uL,                                      /**< Success */
    CY_SAR_BAD_PARAM  = CY_SAR_ID | CY_PDL_STATUS_ERROR | 0x01uL,    /**< Invalid input parameters */
    CY_SAR_TIMEOUT    = CY_SAR_ID | CY_PDL_STATUS_ERROR | 0x02uL,    /**< A timeout occurred */
    CY_SAR_CONVERSION_NOT_COMPLETE = CY_SAR_ID | CY_PDL_STATUS_ERROR | 0x03uL,    /**< SAR conversion is not complete */
}cy_en_sar_status_t;

/** Definitions for starting a conversion used in \ref Cy_SAR_StartConvert */
typedef enum
{
    CY_SAR_START_CONVERT_SINGLE_SHOT = 0uL, /**< Start a single scan (one shot) from firmware */
    CY_SAR_START_CONVERT_CONTINUOUS  = 1uL, /**< Continuously scan enabled channels and ignores all triggers, firmware or hardware */
}cy_en_sar_start_convert_sel_t;

/** Definitions for the return mode used in \ref Cy_SAR_IsEndConversion */
typedef enum
{
    CY_SAR_RETURN_STATUS    = 0uL,      /**< Immediately returns the conversion status. */
    CY_SAR_WAIT_FOR_RESULT  = 1uL,    /**< Does not return a result until the conversion of all sequential channels is complete. This mode is blocking. */
}cy_en_sar_return_mode_t;

/** Switch state definitions */
typedef enum
{
    CY_SAR_SWITCH_OPEN      = 0uL,    /**< Open the switch */
    CY_SAR_SWITCH_CLOSE     = 1uL     /**< Close the switch */
}cy_en_sar_switch_state_t;

/** Definitions for sequencer control of switches */
typedef enum
{
    CY_SAR_SWITCH_SEQ_CTRL_DISABLE = 0uL, /**< Disable sequencer control of switch */
    CY_SAR_SWITCH_SEQ_CTRL_ENABLE  = 1uL  /**< Enable sequencer control of switch */
}cy_en_sar_switch_sar_seq_ctrl_t;

/** Switch register selection for \ref Cy_SAR_SetAnalogSwitch and \ref Cy_SAR_GetAnalogSwitch */
typedef enum
{
    CY_SAR_MUX_SWITCH0  = 0uL,      /**< SARMUX switch control register */
}cy_en_sar_switch_register_sel_t;

/** \addtogroup group_sar_ctrl_register_enums
* This set of enumerations aids in configuring the SAR CTRL register
* \{
*/
/** Reference voltage buffer power mode definitions */
typedef enum
{
    CY_SAR_VREF_PWR_100     = 0uL << SAR_CTRL_PWR_CTRL_VREF_Pos,      /**< Full power (100%) */
    CY_SAR_VREF_PWR_80      = 1uL << SAR_CTRL_PWR_CTRL_VREF_Pos,      /**< 80% power */
    CY_SAR_VREF_PWR_60      = 2uL << SAR_CTRL_PWR_CTRL_VREF_Pos,      /**< 60% power */
    CY_SAR_VREF_PWR_50      = 3uL << SAR_CTRL_PWR_CTRL_VREF_Pos,      /**< 50% power */
    CY_SAR_VREF_PWR_40      = 4uL << SAR_CTRL_PWR_CTRL_VREF_Pos,      /**< 40% power */
    CY_SAR_VREF_PWR_30      = 5uL << SAR_CTRL_PWR_CTRL_VREF_Pos,      /**< 30% power */
    CY_SAR_VREF_PWR_20      = 6uL << SAR_CTRL_PWR_CTRL_VREF_Pos,      /**< 20% power */
    CY_SAR_VREF_PWR_10      = 7uL << SAR_CTRL_PWR_CTRL_VREF_Pos,      /**< 10% power */
}cy_en_sar_ctrl_pwr_ctrl_vref_t;

/** Reference voltage selection definitions */
typedef enum
{
    CY_SAR_VREF_SEL_BGR         = 4uL << SAR_CTRL_VREF_SEL_Pos,     /**< System wide bandgap (Vref buffer on)*/
    CY_SAR_VREF_SEL_EXT         = 5uL << SAR_CTRL_VREF_SEL_Pos,     /**< External Vref direct from a pin */
    CY_SAR_VREF_SEL_VDDA_DIV_2  = 6uL << SAR_CTRL_VREF_SEL_Pos,     /**< Vdda/2 (Vref buffer on) */
    CY_SAR_VREF_SEL_VDDA        = 7uL << SAR_CTRL_VREF_SEL_Pos      /**< Vdda */
}cy_en_sar_ctrl_vref_sel_t;

/** Vref bypass cap enable.
* When enabled, a bypass capacitor should
* be connected to the dedicated Vref pin of the device.
* Refer to the device datasheet for the minimum bypass capacitor value to use.
*/
typedef enum
{
    CY_SAR_BYPASS_CAP_DISABLE = 0uL << SAR_CTRL_VREF_BYP_CAP_EN_Pos,    /**< Disable Vref bypass cap */
    CY_SAR_BYPASS_CAP_ENABLE  = 1uL << SAR_CTRL_VREF_BYP_CAP_EN_Pos     /**< Enable Vref bypass cap */
}cy_en_sar_ctrl_bypass_cap_t;

/** Single ended negative terminal (NEG) selection definitions.
*
* The NEG input for single ended channels can be connected to
* Vref, Vssa_Kelvin, or routed out to an external pin.
* The options for routing to a pin are through P1, P3, P5, or P7
* of the SARMUX or an acore wire in AROUTE.
*
* Vssa_Kelvin comes straight from a Vssa pad without any shared branches
* so as to keep quiet and avoid voltage drops.
*/
typedef enum
{
    CY_SAR_NEG_SEL_VSSA_KELVIN  = 0uL << SAR_CTRL_NEG_SEL_Pos,  /**< NEG input is connected to VSSA_KELVIN */
    CY_SAR_NEG_SEL_P1           = 2uL << SAR_CTRL_NEG_SEL_Pos,  /**< NEG input is connected to P1 pin of SARMUX */
    CY_SAR_NEG_SEL_P3           = 3uL << SAR_CTRL_NEG_SEL_Pos,  /**< NEG input is connected to P3 pin of SARMUX */
    CY_SAR_NEG_SEL_P5           = 4uL << SAR_CTRL_NEG_SEL_Pos,  /**< NEG input is connected to P5 pin of SARMUX */
    CY_SAR_NEG_SEL_P7           = 5uL << SAR_CTRL_NEG_SEL_Pos,  /**< NEG input is connected to P6 pin of SARMUX */
    CY_SAR_NEG_SEL_ACORE        = 6uL << SAR_CTRL_NEG_SEL_Pos,  /**< NEG input is connected to an ACORE in AROUTE */
    CY_SAR_NEG_SEL_VREF         = 7uL << SAR_CTRL_NEG_SEL_Pos,  /**< NEG input is shorted to VREF input of SARADC */
}cy_en_sar_ctrl_neg_sel_t;

/** Enable hardware control of the switch between Vref and the NEG input */
typedef enum
{
    CY_SAR_CTRL_NEGVREF_FW_ONLY = 0uL << SAR_CTRL_SAR_HW_CTRL_NEGVREF_Pos,    /**< Only firmware control of the switch*/
    CY_SAR_CTRL_NEGVREF_HW      = 1uL << SAR_CTRL_SAR_HW_CTRL_NEGVREF_Pos     /**< Enable hardware control of the switch */
}cy_en_sar_ctrl_hw_ctrl_negvref_t;

/** Configure the comparator latch delay */
typedef enum
{
    CY_SAR_CTRL_COMP_DLY_2P5    = 0uL << SAR_CTRL_COMP_DLY_Pos,    /**< 2.5 ns delay, use for SAR conversion rate up to 2.5 Msps */
    CY_SAR_CTRL_COMP_DLY_4      = 1uL << SAR_CTRL_COMP_DLY_Pos,    /**< 4 ns delay, use for SAR conversion rate up to 2.0 Msps */
    CY_SAR_CTRL_COMP_DLY_10     = 2uL << SAR_CTRL_COMP_DLY_Pos,    /**< 10 ns delay, use for SAR conversion rate up to 1.5 Msps */
    CY_SAR_CTRL_COMP_DLY_12     = 3uL << SAR_CTRL_COMP_DLY_Pos     /**< 12 ns delay, use for SAR conversion rate up to 1 Msps */
}cy_en_sar_ctrl_comp_delay_t;

/** Enable the reference buffer (REFBUF) and the routing for the reference block.
* The REFBUF should always be enabled for any reference source selection. */
typedef enum
{
    CY_SAR_REFBUF_DISABLE = 0uL << SAR_CTRL_REFBUF_EN_Pos,    /**< Only firmware control of the switch */
    CY_SAR_REFBUF_ENABLE  = 1uL << SAR_CTRL_REFBUF_EN_Pos     /**< Enable hardware control of the switch */
}cy_en_sar_ctrl_refbuf_en_t;

/** Configure the comparator power mode */
typedef enum
{
    CY_SAR_COMP_PWR_100     = 0uL << SAR_CTRL_COMP_PWR_Pos,      /**< 100% power, use this for > 2 Msps */
    CY_SAR_COMP_PWR_80      = 1uL << SAR_CTRL_COMP_PWR_Pos,      /**< 80% power, use this for 1.5 - 2 Msps */
    CY_SAR_COMP_PWR_60      = 2uL << SAR_CTRL_COMP_PWR_Pos,      /**< 60% power, use this for 1.0 - 1.5 Msps */
    CY_SAR_COMP_PWR_50      = 3uL << SAR_CTRL_COMP_PWR_Pos,      /**< 50% power, use this for 500 ksps - 1 Msps */
    CY_SAR_COMP_PWR_40      = 4uL << SAR_CTRL_COMP_PWR_Pos,      /**< 40% power, use this for 250 - 500 ksps */
    CY_SAR_COMP_PWR_30      = 5uL << SAR_CTRL_COMP_PWR_Pos,      /**< 30% power, use this for 100 - 250 ksps */
    CY_SAR_COMP_PWR_20      = 6uL << SAR_CTRL_COMP_PWR_Pos,      /**< 20% power, use this for TDB sps */
    CY_SAR_COMP_PWR_10      = 7uL << SAR_CTRL_COMP_PWR_Pos,      /**< 10% power, use this for < 100 ksps */
}cy_en_sar_ctrl_comp_pwr_t;

/** Enable or disable the SARMUX during Deep Sleep power mode */
typedef enum
{
    CY_SAR_DEEPSLEEP_SARMUX_OFF = 0uL << SAR_CTRL_DEEPSLEEP_ON_Pos,    /**< Disable SARMUX IP during Deep Sleep */
    CY_SAR_DEEPSLEEP_SARMUX_ON  = 1uL << SAR_CTRL_DEEPSLEEP_ON_Pos     /**< Enable SARMUX IP during Deep Sleep */
}cy_en_sar_ctrl_deepsleep_on_t;

/** Enable or disable the SAR sequencer control of routing switches */
typedef enum
{
    CY_SAR_SARSEQ_SWITCH_ENABLE    = 0uL << SAR_CTRL_SWITCH_DISABLE_Pos,    /**< Enable the SAR sequencer to change the routing switches defined in the channel configurations */
    CY_SAR_SARSEQ_SWITCH_DISABLE   = 1uL << SAR_CTRL_SWITCH_DISABLE_Pos     /**< SAR sequencer does not enable any switches. It is up to the firmware to set the routing switches */
}cy_en_sar_ctrl_sarseq_routing_switches_t;

/* \} */

/** \addtogroup group_sar_sample_ctrl_register_enums
* This set of enumerations aids in configuring the SAR SAMPLE_CTRL register
* \{
*/
/** Configure result alignment, either left or right aligned.
*
* Note that averaging always uses right alignment. If the LEFT_ALIGN
* is selected with averaging enabled, it is ignored. */
typedef enum
{
    CY_SAR_RIGHT_ALIGN  = 0uL << SAR_SAMPLE_CTRL_LEFT_ALIGN_Pos,    /**< Right align result data to bits [11:0] with sign extension to 16 bits if channel is signed */
    CY_SAR_LEFT_ALIGN   = 1uL << SAR_SAMPLE_CTRL_LEFT_ALIGN_Pos     /**< Left align result data to bits [15:4] */
}cy_en_sar_sample_ctrl_align_t;

/** Configure format, signed or unsigned, of single-ended channels */
typedef enum
{
    CY_SAR_SINGLE_ENDED_UNSIGNED  = 0uL << SAR_SAMPLE_CTRL_SINGLE_ENDED_SIGNED_Pos,    /**< Result data for single-ended channels is unsigned */
    CY_SAR_SINGLE_ENDED_SIGNED    = 1uL << SAR_SAMPLE_CTRL_SINGLE_ENDED_SIGNED_Pos     /**< Result data for single-ended channels is signed */
}cy_en_sar_sample_ctrl_single_ended_signed_t;

/** Configure format, signed or unsigned, of differential channels */
typedef enum
{
    CY_SAR_DIFFERENTIAL_UNSIGNED  = 0uL << SAR_SAMPLE_CTRL_DIFFERENTIAL_SIGNED_Pos,    /**< Result data for differential channels is unsigned */
    CY_SAR_DIFFERENTIAL_SIGNED    = 1uL << SAR_SAMPLE_CTRL_DIFFERENTIAL_SIGNED_Pos     /**< Result data for differential channels is signed */
}cy_en_sar_sample_ctrl_differential_signed_t;

/** Configure number of samples for averaging.
* This applies only to channels with averaging enabled.
*/
typedef enum
{
    CY_SAR_AVG_CNT_2          = 0uL << SAR_SAMPLE_CTRL_AVG_CNT_Pos,    /**< Set samples averaged to 2 */
    CY_SAR_AVG_CNT_4          = 1uL << SAR_SAMPLE_CTRL_AVG_CNT_Pos,    /**< Set samples averaged to 4 */
    CY_SAR_AVG_CNT_8          = 2uL << SAR_SAMPLE_CTRL_AVG_CNT_Pos,    /**< Set samples averaged to 8 */
    CY_SAR_AVG_CNT_16         = 3uL << SAR_SAMPLE_CTRL_AVG_CNT_Pos,    /**< Set samples averaged to 16 */
    CY_SAR_AVG_CNT_32         = 4uL << SAR_SAMPLE_CTRL_AVG_CNT_Pos,    /**< Set samples averaged to 32 */
    CY_SAR_AVG_CNT_64         = 5uL << SAR_SAMPLE_CTRL_AVG_CNT_Pos,    /**< Set samples averaged to 64 */
    CY_SAR_AVG_CNT_128        = 6uL << SAR_SAMPLE_CTRL_AVG_CNT_Pos,    /**< Set samples averaged to 128 */
    CY_SAR_AVG_CNT_256        = 7uL << SAR_SAMPLE_CTRL_AVG_CNT_Pos     /**< Set samples averaged to 256 */
}cy_en_sar_sample_ctrl_avg_cnt_signed_t;

/** Configure the averaging mode.
*
* - Sequential accumulate and dump: a channel will be sampled back to back.
*   The average result is calculated and stored and then the next enabled
*   channel is sampled. The result is forced to shift right so that it fits
*   in 16 bits.
* - Sequential fixed:  a channel will be sampled back to back.
*   The average result is calculated and stored and then the next enabled
*   channel is sampled. The result is forced to shift right so that it fits
*   in 12 bits.
* - Interleaved: one sample is taken per triggered scan, only in the scan where
*   the final averaging count is reached. A valid average is calculated and stored
*   in the RESULT register. In all other scans, the RESULT register for averaged
*   channels will have an invalid result. In this mode, make sure that the averaging
*   count is low enough to ensure that the intermediate value does not exceed 16-bits.
*   Otherwise, the MSBs will be lost. So for a 12-bit resolution, the averaging count
*   should be set to 16 or less.
*
*   If averaging is enabled for all enabled channels, the interrupt frequency
*   will be reduced by a factor of AVG_CNT (the number of samples averaged).
*/
typedef enum
{
    CY_SAR_AVG_MODE_SEQUENTIAL_ACCUM    = 0uL,                               /**< Set mode to sequential accumulate and dump */
    CY_SAR_AVG_MODE_SEQUENTIAL_FIXED    = SAR_SAMPLE_CTRL_AVG_SHIFT_Msk,     /**< Set mode to sequential 12-bit fixed */
    CY_SAR_AVG_MODE_INTERLEAVED         = SAR_SAMPLE_CTRL_AVG_MODE_Msk,      /**< Set mode to interleaved */
}cy_en_sar_sample_ctrl_avg_mode_t;

/** Configure the trigger mode.
*
* Firmware triggering is always enabled and can be single shot or continuous.
* Additionally, hardware triggering can be enabled with the option to be
* edge or level sensitive.
*
* These modes are passed into \ref Cy_SAR_SetConvertMode.
*/
typedef enum
{
    CY_SAR_TRIGGER_MODE_FW_ONLY        = 0uL,                                /**< Firmware trigger only, disable hardware trigger*/
    CY_SAR_TRIGGER_MODE_FW_AND_HWEDGE  = SAR_SAMPLE_CTRL_DSI_TRIGGER_EN_Msk, /**< Enable edge sensitive hardware trigger. Each rising edge will trigger a single scan. */
    CY_SAR_TRIGGER_MODE_FW_AND_HWLEVEL = SAR_SAMPLE_CTRL_DSI_TRIGGER_EN_Msk | SAR_SAMPLE_CTRL_DSI_TRIGGER_LEVEL_Msk, /**< Enable level sensitive hardware trigger. The SAR will continuously scan while the trigger signal is HIGH. */
}cy_en_sar_sample_ctrl_trigger_mode_t;

/* \} */

/** \addtogroup group_sar_sample_time_shift_ts
* This set of enumerations aids in configuring the SAR SAMPLE_TIME* registers
* \{
*/
/** Configure the sample time by using these shifts */
typedef enum
{
    CY_SAR_SAMPLE_TIME0_SHIFT       = SAR_SAMPLE_TIME01_SAMPLE_TIME0_Pos,             /**< Shift for sample time 0 */
    CY_SAR_SAMPLE_TIME1_SHIFT       = SAR_SAMPLE_TIME01_SAMPLE_TIME1_Pos,             /**< Shift for sample time 1 */
    CY_SAR_SAMPLE_TIME2_SHIFT       = SAR_SAMPLE_TIME23_SAMPLE_TIME2_Pos,             /**< Shift for sample time 2 */
    CY_SAR_SAMPLE_TIME3_SHIFT       = SAR_SAMPLE_TIME23_SAMPLE_TIME3_Pos,             /**< Shift for sample time 3 */
}cy_en_sar_sample_time_shift_t;
/* \} */

/** \addtogroup group_sar_range_thres_register_enums
* This set of enumerations aids in configuring the SAR RANGE* registers
* \{
*/
/** Configure the lower and upper thresholds for range detection
*
* The SARSEQ supports range detection to allow for automatic detection of sample
* values compared to two programmable thresholds without CPU involvement.
* Range detection is defined by two global thresholds and a condition.
* The RANGE_LOW value defines the lower threshold and RANGE_HIGH defines
* the upper threshold of the range.
*
* Range detect is done after averaging, alignment, and sign extension (if applicable).
* In other words, the thresholds values must have the same data format as the result data.
* Range detection is always done for all channels scanned. By making RANGE_INTR_MASK=0,
* the firmware can choose to ignore the range detect interrupt for any channel.
*/
typedef enum
{
    CY_SAR_LIMIT_LOW_SHIFT      = SAR_RANGE_THRES_RANGE_LOW_Pos,        /**< Shift for setting lower limit of range detection */
    CY_SAR_LIMIT_HIGH_SHIFT     = SAR_RANGE_THRES_RANGE_HIGH_Pos,       /**< Shift for setting upper limit of range detection */
}cy_en_sar_range_thres_shift_t;

/** Configure the condition (below, inside, above, or outside) of the range detection */
typedef enum
{
    CY_SAR_LIMIT_COND_BELOW     = 0uL,  /**< Limit detected when result < RANGE_LOW */
    CY_SAR_LIMIT_COND_INSIDE    = 1uL,  /**< Limit detected when RANGE_LOW <= result < RANGE_HIGH */
    CY_SAR_LIMIT_COND_ABOVE     = 2uL,  /**< Limit detected when RANGE_HIGH <= result */
    CY_SAR_LIMIT_COND_OUTSIDE   = 3uL,  /**< Limit detected when result < RANGE_LOW || RANGE_HIGH <= result */
}cy_en_sar_range_detect_condition_t;
/* \} */

/** \addtogroup group_sar_chan_config_register_enums
* This set of enumerations aids in configuring the SAR CHAN_CONFIG register
* \{
*/
/** Configure the input mode of the channel
*
* - Single ended channel: the NEG_SEL in the CTRL register determines what drives the Vminus pin
* - Differential paired: Vplus and Vminus are a pair. The POS_PIN_ADDR[0] is ignored and considered to be 0.
*   In other words, POS_PIN_ADDR points to the even pin of a pin pair. In that case, the even pin of the
*   pair is connected to Vplus and the odd pin of the pair is connected to Vminus.
*   POS_PORT_ADDR is used to identify the port that contains the pins.
* - Differential unpaired: The NEG_PIN_ADDR and NEG_PORT_ADDR determine what drives the Vminus pin.
*   This is a variation of differential mode with no even-odd pair limitation
*/
typedef enum
{
    CY_SAR_CHAN_SINGLE_ENDED            = 0uL,                                     /**< Single ended channel */
    CY_SAR_CHAN_DIFFERENTIAL_PAIRED     = SAR_CHAN_CONFIG_DIFFERENTIAL_EN_Msk,     /**< Differential with even-odd pair limitation */
    CY_SAR_CHAN_DIFFERENTIAL_UNPAIRED   = SAR_CHAN_CONFIG_NEG_ADDR_EN_Msk          /**< Differential with no even-odd pair limitation */
}cy_en_sar_chan_config_input_mode_t;

/** Configure address of the pin connected to the Vplus terminal of the SARADC. */
typedef enum
{
    CY_SAR_CHAN_POS_PIN_ADDR_0     = 0uL,                                            /**< Pin 0 on port specified in POS_PORT_ADDR */
    CY_SAR_CHAN_POS_PIN_ADDR_1     = 1uL << SAR_CHAN_CONFIG_POS_PIN_ADDR_Pos,        /**< Pin 1 on port specified in POS_PORT_ADDR */
    CY_SAR_CHAN_POS_PIN_ADDR_2     = 2uL << SAR_CHAN_CONFIG_POS_PIN_ADDR_Pos,        /**< Pin 2 on port specified in POS_PORT_ADDR */
    CY_SAR_CHAN_POS_PIN_ADDR_3     = 3uL << SAR_CHAN_CONFIG_POS_PIN_ADDR_Pos,        /**< Pin 3 on port specified in POS_PORT_ADDR */
    CY_SAR_CHAN_POS_PIN_ADDR_4     = 4uL << SAR_CHAN_CONFIG_POS_PIN_ADDR_Pos,        /**< Pin 4 on port specified in POS_PORT_ADDR */
    CY_SAR_CHAN_POS_PIN_ADDR_5     = 5uL << SAR_CHAN_CONFIG_POS_PIN_ADDR_Pos,        /**< Pin 5 on port specified in POS_PORT_ADDR */
    CY_SAR_CHAN_POS_PIN_ADDR_6     = 6uL << SAR_CHAN_CONFIG_POS_PIN_ADDR_Pos,        /**< Pin 6 on port specified in POS_PORT_ADDR */
    CY_SAR_CHAN_POS_PIN_ADDR_7     = 7uL << SAR_CHAN_CONFIG_POS_PIN_ADDR_Pos,        /**< Pin 7 on port specified in POS_PORT_ADDR */
}cy_en_sar_chan_config_pos_pin_addr_t;

/** Configure address of the port that contains the pin connected to the Vplus terminal of the SARADC
*
* - Port 0 is 8 pins of the SARMUX
* - Port 1 through 4 are respectively the pins of CTB0, CTB1, CTB2, and CTB3 (if present)
* - Port 7, 5, and 6 (VPORT0/1/2) are the groups of internal signals that can be selected
*   in the SARMUX or AROUTE (if present).
*/
typedef enum
{
    CY_SAR_POS_PORT_ADDR_SARMUX         = 0uL,                                       /**< SARMUX pins */
    CY_SAR_POS_PORT_ADDR_CTB0           = 1uL << SAR_CHAN_CONFIG_POS_PORT_ADDR_Pos,  /**< CTB0 */
    CY_SAR_POS_PORT_ADDR_CTB1           = 2uL << SAR_CHAN_CONFIG_POS_PORT_ADDR_Pos,  /**< CTB1 */
    CY_SAR_POS_PORT_ADDR_CTB2           = 3uL << SAR_CHAN_CONFIG_POS_PORT_ADDR_Pos,  /**< CTB2 */
    CY_SAR_POS_PORT_ADDR_CTB3           = 4uL << SAR_CHAN_CONFIG_POS_PORT_ADDR_Pos,  /**< CTB3 */
    CY_SAR_POS_PORT_ADDR_AROUTE_VIRT2   = 5uL << SAR_CHAN_CONFIG_POS_PORT_ADDR_Pos,  /**< AROUTE virtual port (VPORT2) */
    CY_SAR_POS_PORT_ADDR_AROUTE_VIRT1   = 6uL << SAR_CHAN_CONFIG_POS_PORT_ADDR_Pos,  /**< AROUTE virtual port (VPORT1) */
    CY_SAR_POS_PORT_ADDR_SARMUX_VIRT    = 7uL << SAR_CHAN_CONFIG_POS_PORT_ADDR_Pos,  /**< SARMUX virtual port (VPORT0) */
}cy_en_sar_chan_config_pos_port_addr_t;

/** Enable or disable averaging for the channel */
typedef enum
{
    CY_SAR_CHAN_AVG_DISABLE      = 0uL,                                 /**< Disable averaging for the channel */
    CY_SAR_CHAN_AVG_ENABLE       = 1uL << SAR_CHAN_CONFIG_AVG_EN_Pos    /**< Enable averaging for the channel */
}cy_en_sar_chan_config_avg_en_t;

/** Select which sample time to use for the channel.
* There are four global samples times available.
*/
typedef enum
{
    CY_SAR_CHAN_SAMPLE_TIME_0     = 0uL,                                          /**< Use sample time 0 for the channel */
    CY_SAR_CHAN_SAMPLE_TIME_1     = 1uL << SAR_CHAN_CONFIG_SAMPLE_TIME_SEL_Pos,   /**< Use sample time 1 for the channel */
    CY_SAR_CHAN_SAMPLE_TIME_2     = 2uL << SAR_CHAN_CONFIG_SAMPLE_TIME_SEL_Pos,   /**< Use sample time 2 for the channel */
    CY_SAR_CHAN_SAMPLE_TIME_3     = 3uL << SAR_CHAN_CONFIG_SAMPLE_TIME_SEL_Pos,   /**< Use sample time 3 for the channel */
}cy_en_sar_chan_config_sample_time_t;

/** Configure address of the pin connected to the Vminus terminal of the SARADC. */
typedef enum
{
    CY_SAR_CHAN_NEG_PIN_ADDR_0     = 0uL,                                            /**< Pin 0 on port specified in NEG_PORT_ADDR */
    CY_SAR_CHAN_NEG_PIN_ADDR_1     = 1uL << SAR_CHAN_CONFIG_NEG_PIN_ADDR_Pos,        /**< Pin 1 on port specified in NEG_PORT_ADDR */
    CY_SAR_CHAN_NEG_PIN_ADDR_2     = 2uL << SAR_CHAN_CONFIG_NEG_PIN_ADDR_Pos,        /**< Pin 2 on port specified in NEG_PORT_ADDR */
    CY_SAR_CHAN_NEG_PIN_ADDR_3     = 3uL << SAR_CHAN_CONFIG_NEG_PIN_ADDR_Pos,        /**< Pin 3 on port specified in NEG_PORT_ADDR */
    CY_SAR_CHAN_NEG_PIN_ADDR_4     = 4uL << SAR_CHAN_CONFIG_NEG_PIN_ADDR_Pos,        /**< Pin 4 on port specified in NEG_PORT_ADDR */
    CY_SAR_CHAN_NEG_PIN_ADDR_5     = 5uL << SAR_CHAN_CONFIG_NEG_PIN_ADDR_Pos,        /**< Pin 5 on port specified in NEG_PORT_ADDR */
    CY_SAR_CHAN_NEG_PIN_ADDR_6     = 6uL << SAR_CHAN_CONFIG_NEG_PIN_ADDR_Pos,        /**< Pin 6 on port specified in NEG_PORT_ADDR */
    CY_SAR_CHAN_NEG_PIN_ADDR_7     = 7uL << SAR_CHAN_CONFIG_NEG_PIN_ADDR_Pos,        /**< Pin 7 on port specified in NEG_PORT_ADDR */
}cy_en_sar_chan_config_neg_pin_addr_t;

/** Configure address of the port that contains the pin connected to the Vminus terminal of the SARADC.
*
* - Port 0 is 8 pins of the SARMUX
* - Port 7, 5, and 6 (VPORT0/1/2) are the groups of internal signals that can be selected
*   in the SARMUX or AROUTE (if present).
*/
typedef enum
{
    CY_SAR_NEG_PORT_ADDR_SARMUX         = 0uL,                                       /**< SARMUX pins */
    CY_SAR_NEG_PORT_ADDR_AROUTE_VIRT2   = 5uL << SAR_CHAN_CONFIG_POS_PORT_ADDR_Pos,  /**< AROUTE virtual port (VPORT2) */
    CY_SAR_NEG_PORT_ADDR_AROUTE_VIRT1   = 6uL << SAR_CHAN_CONFIG_POS_PORT_ADDR_Pos,  /**< AROUTE virtual port (VPORT1) */
    CY_SAR_NEG_PORT_ADDR_SARMUX_VIRT    = 7uL << SAR_CHAN_CONFIG_POS_PORT_ADDR_Pos,  /**< SARMUX virtual port (VPORT0) */
}cy_en_sar_chan_config_neg_port_addr_t;

/* \} */

/** \addtogroup group_sar_intr_mask_t_register_enums
* This set of enumerations aids in configuring the SAR INTR_MASK register
* \{
*/
/** Configure which signal will cause an interrupt event.
*
* - End of Scan (EOS): occurs after completing a scan of all enabled channels
* - Overflow: occurs when hardware sets a new EOS interrupt while the previous interrupt
*   has not be cleared by the firmware
* - Firmware collision: occurs when firmware attempts to start one-shot
*   conversion while the SAR is busy.
*
* Enable all, one, or none of the interrupt events.
*/
typedef enum
{
    CY_SAR_INTR_MASK_NONE           = 0uL,                                  /**< Disable all interrupt sources */
    CY_SAR_INTR_EOS_MASK            = SAR_INTR_MASK_EOS_MASK_Msk,           /**< Enable end of scan (EOS) interrupt */
    CY_SAR_INTR_OVERFLOW_MASK       = SAR_INTR_MASK_OVERFLOW_MASK_Msk,      /**< Enable overflow interrupt */
    CY_SAR_INTR_FW_COLLISION_MASK   = SAR_INTR_MASK_FW_COLLISION_MASK_Msk,  /**< Enable firmware collision interrupt */
}cy_en_sar_intr_mask_t;
/* \} */

/** \addtogroup group_sar_mux_switch_register_enums
* This set of enumerations aids in configuring the SAR MUX_SWITCH and MUX_SQ_CTRL register
* \{
*/

/** Firmware control for the SARMUX switches to connect analog signals to the SAR ADC
*
* To close multiple switches, OR the enum values together.
*/
typedef enum
{
    /* SARMUX pins to Vplus */
    CY_SAR_MUX_FW_P0_VPLUS         = SAR_MUX_SWITCH0_MUX_FW_P0_VPLUS_Msk,    /**< Switch between pin P0 of SARMUX and Vplus of SAR ADC */
    CY_SAR_MUX_FW_P1_VPLUS         = SAR_MUX_SWITCH0_MUX_FW_P1_VPLUS_Msk,    /**< Switch between pin P1 of SARMUX and Vplus of SAR ADC */
    CY_SAR_MUX_FW_P2_VPLUS         = SAR_MUX_SWITCH0_MUX_FW_P2_VPLUS_Msk,    /**< Switch between pin P2 of SARMUX and Vplus of SAR ADC */
    CY_SAR_MUX_FW_P3_VPLUS         = SAR_MUX_SWITCH0_MUX_FW_P3_VPLUS_Msk,    /**< Switch between pin P3 of SARMUX and Vplus of SAR ADC */
    CY_SAR_MUX_FW_P4_VPLUS         = SAR_MUX_SWITCH0_MUX_FW_P4_VPLUS_Msk,    /**< Switch between pin P4 of SARMUX and Vplus of SAR ADC */
    CY_SAR_MUX_FW_P5_VPLUS         = SAR_MUX_SWITCH0_MUX_FW_P5_VPLUS_Msk,    /**< Switch between pin P5 of SARMUX and Vplus of SAR ADC */
    CY_SAR_MUX_FW_P6_VPLUS         = SAR_MUX_SWITCH0_MUX_FW_P6_VPLUS_Msk,    /**< Switch between pin P6 of SARMUX and Vplus of SAR ADC */
    CY_SAR_MUX_FW_P7_VPLUS         = SAR_MUX_SWITCH0_MUX_FW_P7_VPLUS_Msk,    /**< Switch between pin P7 of SARMUX and Vplus of SAR ADC */

    /* SARMUX pins to Vminus */
    CY_SAR_MUX_FW_P0_VMINUS        = SAR_MUX_SWITCH0_MUX_FW_P0_VMINUS_Msk,   /**< Switch between pin P0 of SARMUX and Vminus of SAR ADC */
    CY_SAR_MUX_FW_P1_VMINUS        = SAR_MUX_SWITCH0_MUX_FW_P1_VMINUS_Msk,   /**< Switch between pin P1 of SARMUX and Vminus of SAR ADC */
    CY_SAR_MUX_FW_P2_VMINUS        = SAR_MUX_SWITCH0_MUX_FW_P2_VMINUS_Msk,   /**< Switch between pin P2 of SARMUX and Vminus of SAR ADC */
    CY_SAR_MUX_FW_P3_VMINUS        = SAR_MUX_SWITCH0_MUX_FW_P3_VMINUS_Msk,   /**< Switch between pin P3 of SARMUX and Vminus of SAR ADC */
    CY_SAR_MUX_FW_P4_VMINUS        = SAR_MUX_SWITCH0_MUX_FW_P4_VMINUS_Msk,   /**< Switch between pin P4 of SARMUX and Vminus of SAR ADC */
    CY_SAR_MUX_FW_P5_VMINUS        = SAR_MUX_SWITCH0_MUX_FW_P5_VMINUS_Msk,   /**< Switch between pin P5 of SARMUX and Vminus of SAR ADC */
    CY_SAR_MUX_FW_P6_VMINUS        = SAR_MUX_SWITCH0_MUX_FW_P6_VMINUS_Msk,   /**< Switch between pin P6 of SARMUX and Vminus of SAR ADC */
    CY_SAR_MUX_FW_P7_VMINUS        = SAR_MUX_SWITCH0_MUX_FW_P7_VMINUS_Msk,   /**< Switch between pin P7 of SARMUX and Vminus of SAR ADC */

    /* Vssa to Vminus and temperature sensor to Vplus */
    CY_SAR_MUX_FW_VSSA_VMINUS      = SAR_MUX_SWITCH0_MUX_FW_VSSA_VMINUS_Msk,    /**< Switch between Vssa_Kelvin and Vminus of SAR ADC */
    CY_SAR_MUX_FW_TEMP_VPLUS       = SAR_MUX_SWITCH0_MUX_FW_TEMP_VPLUS_Msk,     /**< Switch between temperature sensor and vplus of SAR ADC */

    /* Amuxbus A and B to Vplus and Vminus */
    CY_SAR_MUX_FW_AMUXBUSA_VPLUS   = SAR_MUX_SWITCH0_MUX_FW_AMUXBUSA_VPLUS_Msk,     /**< Switch between amuxbusa and vplus of SAR ADC */
    CY_SAR_MUX_FW_AMUXBUSB_VPLUS   = SAR_MUX_SWITCH0_MUX_FW_AMUXBUSB_VPLUS_Msk,     /**< Switch between amuxbusb and vplus of SAR ADC */
    CY_SAR_MUX_FW_AMUXBUSA_VMINUS  = SAR_MUX_SWITCH0_MUX_FW_AMUXBUSA_VMINUS_Msk,    /**< Switch between amuxbusa and vminus of SAR ADC */
    CY_SAR_MUX_FW_AMUXBUSB_VMINUS  = SAR_MUX_SWITCH0_MUX_FW_AMUXBUSB_VMINUS_Msk,    /**< Switch between amuxbusb and vminus of SAR ADC */

    /* Sarbus 0 and 1 to Vplus and Vminus */
    CY_SAR_MUX_FW_SARBUS0_VPLUS    = SAR_MUX_SWITCH0_MUX_FW_SARBUS0_VPLUS_Msk,      /**< Switch between sarbus0 and vplus of SAR ADC */
    CY_SAR_MUX_FW_SARBUS1_VPLUS    = SAR_MUX_SWITCH0_MUX_FW_SARBUS1_VPLUS_Msk,      /**< Switch between sarbus1 and vplus of SAR ADC */
    CY_SAR_MUX_FW_SARBUS0_VMINUS   = SAR_MUX_SWITCH0_MUX_FW_SARBUS0_VMINUS_Msk,     /**< Switch between sarbus0 and vminus of SAR ADC */
    CY_SAR_MUX_FW_SARBUS1_VMINUS   = SAR_MUX_SWITCH0_MUX_FW_SARBUS1_VMINUS_Msk,     /**< Switch between sarbus1 and vminus of SAR ADC */

    /* SARMUX pins to Core IO */
    CY_SAR_MUX_FW_P4_COREIO0       = SAR_MUX_SWITCH0_MUX_FW_P4_COREIO0_Msk,      /**< Switch between pin P4 of SARMUX and coreio0 */
    CY_SAR_MUX_FW_P5_COREIO1       = SAR_MUX_SWITCH0_MUX_FW_P5_COREIO1_Msk,      /**< Switch between pin P5 of SARMUX and coreio1 */
    CY_SAR_MUX_FW_P6_COREIO2       = SAR_MUX_SWITCH0_MUX_FW_P6_COREIO2_Msk,      /**< Switch between pin P6 of SARMUX and coreio2 */
    CY_SAR_MUX_FW_P7_COREIO3       = SAR_MUX_SWITCH0_MUX_FW_P7_COREIO3_Msk,      /**< Switch between pin P7 of SARMUX and coreio3 */
}cy_en_sar_mux_switch_fw_ctrl_t;

/** Enable SAR sequencer control for the SARMUX switches
*
* To enable sequencer control of multiple switches, OR the enum values together.
*/
typedef enum
{
    CY_SAR_MUX_SQ_CTRL_P0           = SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P0_Msk,        /**< Give SARSEQ control of P0 switches (for Vplus and Vminus) */
    CY_SAR_MUX_SQ_CTRL_P1           = SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P1_Msk,        /**< Give SARSEQ control of P1 switches (for Vplus and Vminus) */
    CY_SAR_MUX_SQ_CTRL_P2           = SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P2_Msk,        /**< Give SARSEQ control of P2 switches (for Vplus and Vminus) */
    CY_SAR_MUX_SQ_CTRL_P3           = SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P3_Msk,        /**< Give SARSEQ control of P3 switches (for Vplus and Vminus) */
    CY_SAR_MUX_SQ_CTRL_P4           = SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P4_Msk,        /**< Give SARSEQ control of P4 switches (for Vplus and Vminus) */
    CY_SAR_MUX_SQ_CTRL_P5           = SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P5_Msk,        /**< Give SARSEQ control of P5 switches (for Vplus and Vminus) */
    CY_SAR_MUX_SQ_CTRL_P6           = SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P6_Msk,        /**< Give SARSEQ control of P6 switches (for Vplus and Vminus) */
    CY_SAR_MUX_SQ_CTRL_P7           = SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P7_Msk,        /**< Give SARSEQ control of P7 switches (for Vplus and Vminus) */
    CY_SAR_MUX_SQ_CTRL_VSSA         = SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_VSSA_Msk,      /**< Give SARSEQ control of the switch between vssa and Vminus */
    CY_SAR_MUX_SQ_CTRL_TEMP         = SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_TEMP_Msk,      /**< Give SARSEQ control of the switch between temp and Vplus */
    CY_SAR_MUX_SQ_CTRL_AMUXBUSA     = SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_AMUXBUSA_Msk,  /**< Give SARSEQ control of amuxbusa switches (vor Vplus and Vminus) */
    CY_SAR_MUX_SQ_CTRL_AMUXBUSB     = SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_AMUXBUSB_Msk,  /**< Give SARSEQ control of amuxbusb switches (vor Vplus and Vminus) */
    CY_SAR_MUX_SQ_CTRL_SARBUS0      = SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_SARBUS0_Msk,   /**< Give SARSEQ control of sarbus0 switches (vor Vplus and Vminus) */
    CY_SAR_MUX_SQ_CTRL_SARBUS1      = SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_SARBUS1_Msk,   /**< Give SARSEQ control of sarbus1 switches (vor Vplus and Vminus) */
}cy_en_sar_mux_switch_sq_ctrl_t;

/* \} */

/** \} group_sar_enums */

/** \addtogroup group_sar_data_structures
* \{
*/

/***************************************
*       Configuration Structures
***************************************/

/** This structure is used to initialize the SAR ADC subsystem.
*
* The SAR ADC subsystem is highly configurable with many options.
* When calling \ref Cy_SAR_Init, provide a pointer to the structure containing this configuration data.
* A set of enumerations is provided in this
* driver to help with configuring this structure.
*
* <table class="doxtable">
*   <tr><th>Data Field</th><th>Configuration</th></tr>
*   <tr><td>ctrl</td>
*     <td>This global configuration register contains options that apply to all channels.
*       Select a value from each of the following enum and "OR" them together to form this ctrl:
*       - \ref cy_en_sar_ctrl_pwr_ctrl_vref_t
*       - \ref cy_en_sar_ctrl_vref_sel_t
*       - \ref cy_en_sar_ctrl_bypass_cap_t
*       - \ref cy_en_sar_ctrl_neg_sel_t
*       - \ref cy_en_sar_ctrl_hw_ctrl_negvref_t
*       - \ref cy_en_sar_ctrl_comp_delay_t
*       - \ref cy_en_sar_ctrl_refbuf_en_t
*       - \ref cy_en_sar_ctrl_comp_pwr_t
*       - \ref cy_en_sar_ctrl_deepsleep_on_t
*       - \ref cy_en_sar_ctrl_sarseq_routing_switches_t
*
*       Note that all these enums begin with cy_en_sar_ctrl_*.
*     </td>
*   </tr>
*   <tr><td>enable</td>
*     <td>If true, enable the hardware at the end of initialization. If false,
*           a call to \ref Cy_SAR_Enable is required to enable the hardware.
*     </td>
*   </tr>
*   <tr><td>sampleCtrl</td>
*     <td>This global configuration register contain options that apply to all channels.
*       Select a value from each of the following enum and "OR" them together to form this sampleCtrl:
*       - \ref cy_en_sar_sample_ctrl_align_t
*       - \ref cy_en_sar_sample_ctrl_single_ended_signed_t
*       - \ref cy_en_sar_sample_ctrl_differential_signed_t
*       - \ref cy_en_sar_sample_ctrl_avg_cnt_signed_t
*       - \ref cy_en_sar_sample_ctrl_avg_mode_t
*       - \ref cy_en_sar_sample_ctrl_trigger_mode_t
*
*     Note that all these enums begin with cy_en_sar_sample*.
*     </td>
*   </tr>
*   <tr><td>sampleTime01</td>
*     <td>Configure the value for sample time choices 0 and 1.
*       The SARADC supports a programmable sample time (aperture). To make this capability
*       available across multiple channels, without slowing them down collectively to the
*       slowest common denominator, there are four fully programmable 10-bit sample times.
*       10 bits allow for a range of 0 to 1023 cycles, however 0 and 1 are invalid.
*       The minimum sample time is 167 ns. With an 18 MHz clock, this is
*       equal to three cycles. The actual sample time is one cycle less than
*       the sample time register value, so the minimum register value is 4.
*
*       Use the shifts defined in \ref cy_en_sar_sample_time_shift_t when setting this register.
*       For example,
*
*       sampleTime01 = (4uL << CY_SAR_SAMPLE_TIME0_SHIFT) |
*                      (5uL << CY_SAR_SAMPLE_TIME1_SHIFT);
*       </td>
*   </tr>
*   <tr><td>sampleTime23</td>
*     <td>Configure the value for sample time choices 2 and 3.
*       Use the shifts defined in \ref cy_en_sar_sample_time_shift_t when setting this register.
*       For example,
*
*       sampleTime23 = (4uL << CY_SAR_SAMPLE_TIME2_SHIFT) |
*                      (5uL << CY_SAR_SAMPLE_TIME3_SHIFT);
*     </td>
*   </tr>
*   <tr><td>rangeThres</td>
*     <td>Configure the upper and lower thresholds for range detection.
*       The SARSEQ supports range detection to allow for automatic detection of sample values
*       compared to two programmable thresholds without CPU involvement.
*       Range detection is defined by two global thresholds and a condition.
*       The RANGE_LOW value defines the lower threshold and RANGE_HIGH defines the upper threshold of the range.
*       Range detect is done after averaging, alignment, and sign extension (if applicable). In other words the
*       thresholds values need to have the same data format as the result data (see Data alignment section above).
*       Range detection is always done for all channels scanned.
*       The values here are interpreted signed or unsigned according to the channel configuration.
*
*       Use the shifts defined in \ref cy_en_sar_range_thres_shift_t when setting this register.
*       For example,
*
*       rangeThres = (0xFFuL  << CY_SAR_LIMIT_LOW_SHIFT) |
*                    (0x3FFuL << CY_SAR_LIMIT_HIGH_SHIFT);
*     </td>
*   </tr>
*   <tr><td>rangeCond</td>
*     <td>Configure the condition that will trigger the range detect interrupt.
*       Select a value from the \ref cy_en_sar_range_detect_condition_t enum.
*   </td>
*   </tr>
*   <tr><td>chanEn</td>
*     <td>Configure which channels will be scanned.
*       All enabled channels will be scanned when the next trigger happens.
*
*       Each bit corresponds to a channel. For example, setting chanEn = 0x7
*       will enable channels 0, 1, and 2.
*     </td>
*   </tr>
*   <tr><td>chanConfig[CY_SAR_MAX_NUM_CHANNELS]</td>
*     <td> Each of the channels has its own channel configuration register.
*       The channel configuration specifies which pin/signal that channel converts
*       and how to do that conversion. The number of channels is 8, 12, or 16 depending on the device.
*       Select a value from each of the following enum and "OR" them together to form the chanConfig:
*       - \ref cy_en_sar_chan_config_input_mode_t
*       - \ref cy_en_sar_chan_config_pos_pin_addr_t
*       - \ref cy_en_sar_chan_config_pos_port_addr_t
*       - \ref cy_en_sar_chan_config_avg_en_t
*       - \ref cy_en_sar_chan_config_sample_time_t
*       - \ref cy_en_sar_chan_config_neg_pin_addr_t
*       - \ref cy_en_sar_chan_config_neg_port_addr_t
*
*       Note that all these enums begin with h cy_en_sar_chan_config*.
*     </td>
*   </tr>
*   <tr><td>intrMask</td>
*     <td>Configure which interrupt source will be serviced by the firmware or ignore.
*       The SAR interrupt to the NVIC is raised any time the intersection (logic and) of the interrupt
*       flags and the corresponding interrupt masks are non-zero.
*       When servicing an interrupt, the interrupt service routine is supposed to clear the interrupt source,
*       after picking up any related data, by writing a 1 to the interrupt bit.
*
*       The \ref cy_en_sar_intr_mask_t enum defines the mask for each interrupt source.
*       Enable more than one by "OR"ing the masks together. For example, setting
*       intrMask = CY_SAR_INTR_EOS_MASK | CY_SAR_INTR_OVERFLOW_MASK, both the
*       end of scan (EOS) and overflow interrupts will be serviced by the NVIC.
*     </td>
*   </tr>
*   <tr><td>satIntrMask</td>
*     <td>Configure which channels will cause a saturate interrupt event.
*       The SARSEQ has a saturation detect that is always applied to every conversion.
*       This feature detects whether a sample value is equal to the minimum or the maximum value.
*       This allows the firmware to take action, for example, discard the result, when the SARADC saturates.
*       The sample value is tested right after conversion, that is, before averaging. This means that it
*       can happen that the interrupt is set while the averaged result in the data register is not
*       equal to the minimum or maximum. To enable a fast response to saturation this interrupt flag
*       is set immediately, that is, before the full scan or even averaging for the channel is completed.
*
*       Each bit corresponds to a channel. For example, setting the satIntrMask
*       to 0x05 will enable channels 0 and 2 for saturation detection.
*     </td>
*   </tr>
*   <tr><td>rangeIntrMask</td>
*     <td>Configure which channels will cause a range detection interrupt event
*       Each bit corresponds to a channel. For example, setting the rangeIntrMask
*       to 0x03 will enable channels 0 and 1 for range detection.
*       Setting rangeIntrMask to 0, the firmware will ignore the range detect interrupt for any channel.
*    </td>
*   </tr>
*   <tr><td>muxSwitch</td>
*     <td>Firmware control of the SARMUX switches.
*     The \ref cy_en_sar_mux_switch_fw_ctrl_t enum defines the mask for the switches.
*     "OR" the masks together to close multiple switches.
*     </td>
*   </tr>
*   <tr><td>muxSwitchSqCtrl</td>
*     <td>Configure SAR sequencer control of the SARMUX switches.
*     The \ref cy_en_sar_mux_switch_sq_ctrl_t enum defines the mask for sequencer control for the switches.
*     "OR" the masks together to enable sequencer control of multiple switches.
*     </td>
*   </tr>
*   <tr><td>configRouting</td>
*     <td>If true, the \ref Cy_SAR_Init function will apply the muxSwitch and muxSwitchSqCtrl values.
*     If false, the two values will be ignored.
*     </td>
*   </tr>
*   <tr><td>vrefMvValue</td>
*     <td>The value of the reference voltage in millivolts used for converting counts
*     to voltage in the \ref Cy_SAR_CountsTo_Volts, \ref Cy_SAR_CountsTo_mVolts, and
*     \ref Cy_SAR_CountsTo_uVolts functions.
*     </td>
* </table>
**/
typedef struct
{
    uint32_t ctrl;                                      /**< Control register */
    bool enable;                                        /**< Enable hardware after initialization */
    uint32_t sampleCtrl;                                /**< Sample control register */
    uint32_t sampleTime01;                              /**< Sample time in ADC clocks for ST0 and ST1 */
    uint32_t sampleTime23;                              /**< Sample time in ADC clocks for ST2 and ST3 */
    uint32_t rangeThres;                                /**< Range detect threshold register for all channels */
    cy_en_sar_range_detect_condition_t rangeCond;       /**< Range detect mode for all channels */
    uint32_t chanEn;                                    /**< Enable bits for the channels */
    uint32_t chanConfig[CY_SAR_MAX_NUM_CHANNELS];       /**< Channel configuration registers */
    uint32_t intrMask;                                  /**< Interrupt enable mask */
    uint32_t satIntrMask;                               /**< Saturate interrupt mask register */
    uint32_t rangeIntrMask;                             /**< Range interrupt mask register */
    uint32_t muxSwitch;                                 /**< SARMUX firmware switches to connect analog signals to SAR */
    uint32_t muxSwitchSqCtrl;                           /**< SARMUX Switch SAR sequencer control */
    bool configRouting;                                 /**< Configure or ignore routing related registers (muxSwitch, muxSwitchSqCtrl) */
    uint32_t vrefMvValue;                               /**< Reference voltage in millivolts used in counts to volts conversion */
} cy_stc_sar_config_t;

/** This structure is used by the driver to backup the state of the SAR
* before entering sleep so that it can be re-enabled after waking up */
typedef struct
{
    uint32_t hwEnabled;         /**< SAR enabled state */
    uint32_t continuous;        /**< State of the continuous bit */
} cy_stc_sar_state_backup_t;

/** \} group_sar_data_structures */

/** \addtogroup group_sar_functions
* \{
*/

/***************************************
*        Function Prototypes
***************************************/

/** \addtogroup group_sar_functions_basic
* This set of functions is for initialization and basic usage
* \{
*/
cy_en_sar_status_t Cy_SAR_Init(SAR_Type *base, const cy_stc_sar_config_t *config);
cy_en_sar_status_t Cy_SAR_DeInit(SAR_Type *base, bool deInitRouting);
void Cy_SAR_Enable(SAR_Type *base);
__STATIC_INLINE void Cy_SAR_Disable(SAR_Type *base);
void Cy_SAR_StartConvert(SAR_Type *base, cy_en_sar_start_convert_sel_t startSelect);
void Cy_SAR_StopConvert(SAR_Type *base);
cy_en_sar_status_t Cy_SAR_IsEndConversion(SAR_Type *base, cy_en_sar_return_mode_t retMode);
int16_t Cy_SAR_GetResult16(const SAR_Type *base, uint32_t chan);
int32_t Cy_SAR_GetResult32(const SAR_Type *base, uint32_t chan);
__STATIC_INLINE uint32_t Cy_SAR_GetChanResultUpdated(const SAR_Type *base);
/** \} */

/** \addtogroup group_sar_functions_power
* This set of functions is for Deep Sleep entry and exit
* \{
*/
cy_en_syspm_status_t Cy_SAR_DeepSleepCallback(cy_stc_syspm_callback_params_t *callbackParams);
void Cy_SAR_Sleep(SAR_Type *base);
void Cy_SAR_Wakeup(SAR_Type *base);
/** \} */

/** \addtogroup group_sar_functions_config
* This set of functions allows changes to the SAR configuration
* after initialization.
* \{
*/
void Cy_SAR_SetConvertMode(SAR_Type *base, cy_en_sar_sample_ctrl_trigger_mode_t mode);
__STATIC_INLINE void Cy_SAR_SetChanMask(SAR_Type *base, uint32_t enableMask);
void Cy_SAR_SetLowLimit(SAR_Type *base, uint32_t lowLimit);
void Cy_SAR_SetHighLimit(SAR_Type *base, uint32_t highLimit);
__STATIC_INLINE void Cy_SAR_SetLimitCondition(SAR_Type *base, cy_en_sar_range_detect_condition_t cond);
/** \} */

/** \addtogroup group_sar_functions_countsto
* This set of functions performs counts to *volts conversions.
* \{
*/
int16_t Cy_SAR_RawCounts2Counts(const SAR_Type *base, uint32_t chan, int16_t adcCounts);
float32_t Cy_SAR_CountsTo_Volts(const SAR_Type *base, uint32_t chan, int16_t adcCounts);
int16_t Cy_SAR_CountsTo_mVolts(const SAR_Type *base, uint32_t chan, int16_t adcCounts);
int32_t Cy_SAR_CountsTo_uVolts(const SAR_Type *base, uint32_t chan, int16_t adcCounts);
cy_en_sar_status_t Cy_SAR_SetOffset(uint32_t chan, int16_t offset);
cy_en_sar_status_t Cy_SAR_SetGain(uint32_t chan, int32_t adcGain);
/** \} */

/** \addtogroup group_sar_functions_switches
* This set of functions is for controlling/querying the SARMUX switches
* The following figure shows the SARMUX switches. See the device datasheet for the exact location of SARMUX pins.
* \image html sar_sarmux_switches.png
* \{
*/
void Cy_SAR_SetAnalogSwitch(SAR_Type *base, cy_en_sar_switch_register_sel_t switchSelect, uint32_t switchMask, cy_en_sar_switch_state_t state);
uint32_t Cy_SAR_GetAnalogSwitch(const SAR_Type *base, cy_en_sar_switch_register_sel_t switchSelect);
__STATIC_INLINE void Cy_SAR_SetVssaVminusSwitch(SAR_Type *base, cy_en_sar_switch_state_t state);
void Cy_SAR_SetSwitchSarSeqCtrl(SAR_Type *base, uint32_t switchMask, cy_en_sar_switch_sar_seq_ctrl_t ctrl);
__STATIC_INLINE void Cy_SAR_SetVssaSarSeqCtrl(SAR_Type *base, cy_en_sar_switch_sar_seq_ctrl_t ctrl);
/** \} */

/** \addtogroup group_sar_functions_interrupt
* This set of functions are related to SAR interrupts
* \{
*/
__STATIC_INLINE uint32_t Cy_SAR_GetInterruptStatus(const SAR_Type *base);
__STATIC_INLINE void Cy_SAR_ClearInterrupt(SAR_Type *base, uint32_t intrMask);
__STATIC_INLINE void Cy_SAR_SetSwInterrupt(SAR_Type *base, uint32_t intrMask);
__STATIC_INLINE void Cy_SAR_SetInterruptMask(SAR_Type *base, uint32_t intrMask);
__STATIC_INLINE uint32_t Cy_SAR_GetInterruptMask(const SAR_Type *base);
__STATIC_INLINE uint32_t Cy_SAR_GetInterruptStatusMasked(const SAR_Type *base);

__STATIC_INLINE uint32_t Cy_SAR_GetLimitInterruptStatus(const SAR_Type *base);
__STATIC_INLINE void Cy_SAR_ClearLimitInterrupt(SAR_Type *base, uint32_t chanMask);
__STATIC_INLINE void Cy_SAR_SetSwLimitInterrupt(SAR_Type *base, uint32_t chanMask);
__STATIC_INLINE void Cy_SAR_SetLimitInterruptMask(SAR_Type *base, uint32_t chanMask);
__STATIC_INLINE uint32_t Cy_SAR_GetLimitInterruptMask(const SAR_Type *base);
__STATIC_INLINE uint32_t Cy_SAR_GetLimitInterruptStatusMasked(const SAR_Type *base);

__STATIC_INLINE uint32_t Cy_SAR_GetSatInterruptStatus(const SAR_Type *base);
__STATIC_INLINE void Cy_SAR_ClearSatInterrupt(SAR_Type *base, uint32_t chanMask);
__STATIC_INLINE void Cy_SAR_SetSwSatInterrupt(SAR_Type *base, uint32_t chanMask);
__STATIC_INLINE void Cy_SAR_SetSatInterruptMask(SAR_Type *base, uint32_t chanMask);
__STATIC_INLINE uint32_t Cy_SAR_GetSatInterruptMask(const SAR_Type *base);
__STATIC_INLINE uint32_t Cy_SAR_GetSatInterruptStatusMasked(const SAR_Type *base);
/** \} */


/** \addtogroup group_sar_functions_helper
* This set of functions is for useful configuration query
* \{
*/
bool Cy_SAR_IsChannelSigned(const SAR_Type *base, uint32_t chan);
bool Cy_SAR_IsChannelSingleEnded(const SAR_Type *base, uint32_t chan);
__STATIC_INLINE bool Cy_SAR_IsChannelDifferential(const SAR_Type *base, uint32_t chan);
/** \} */

/** \addtogroup group_sar_functions_basic
* \{
*/

/*******************************************************************************
* Function Name: Cy_SAR_Disable
****************************************************************************//**
*
* Turn off the hardware block.
*
* \param base
* Pointer to structure describing registers
*
* \return None
*
*******************************************************************************/
__STATIC_INLINE void Cy_SAR_Disable(SAR_Type *base)
{
    base->CTRL &= ~SAR_CTRL_ENABLED_Msk;
}

/*******************************************************************************
* Function Name: Cy_SAR_GetChanResultUpdated
****************************************************************************//**
*
* Gets the data available in the channel result update register.
* Each bit of the result corresponds to the channel.
* When sequential accumulative or interleaved averaging is enabled,
* the channel result will be invalid until the desired number of
* samples has been averaged.
*
* \param base
* Pointer to structure describing registers
*
* \return uint32_t
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SAR_GetChanResultUpdated(const SAR_Type *base)
{
    return base->CHAN_RESULT_UPDATED;
}
/** \} */

/** \addtogroup group_sar_functions_config
* \{
*/
/*******************************************************************************
* Function Name: Cy_SAR_SetChanMask
****************************************************************************//**
*
* Sets the enable/disable mask for the channels.
*
* \param base
* Pointer to structure describing registers
*
* \param enableMask
* Channel enable/disable mask. Each bit corresponds to a channel.
* - 0: the corresponding channel is disabled.
* - 1: the corresponding channel is enabled; it will be included in the next scan.
*
* \return None
*
*******************************************************************************/
__STATIC_INLINE void Cy_SAR_SetChanMask(SAR_Type *base, uint32_t enableMask)
{
    CY_ASSERT_L2(CY_SAR_CHANMASK(enableMask));

    base->CHAN_EN = enableMask;
}

/*******************************************************************************
* Function Name: Cy_SAR_SetLimitCondition
****************************************************************************//**
*
* Sets the condition in which limit detection interrupts are triggered.
*
* \param base
* Pointer to structure describing registers
*
* \param cond
* A value of the enum \ref cy_en_sar_range_detect_condition_t.
*
* \return None
*
*******************************************************************************/
__STATIC_INLINE void Cy_SAR_SetLimitCondition(SAR_Type *base, cy_en_sar_range_detect_condition_t cond)
{
    CY_ASSERT_L3(CY_SAR_LIMITCOND(cond));

    base->RANGE_COND = (uint32_t)cond << SAR_RANGE_COND_RANGE_COND_Pos;
}

/** \} */

/** \addtogroup group_sar_functions_interrupt
* \{
*/
/*******************************************************************************
* Function Name: Cy_SAR_GetInterruptStatus
****************************************************************************//**
*
* Returns the interrupt register status.
*
* \param base
* Pointer to structure describing registers
*
* \return Interrupt status
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SAR_GetInterruptStatus(const SAR_Type *base)
{
    return base->INTR;
}

/*******************************************************************************
* Function Name: Cy_SAR_ClearInterrupt
****************************************************************************//**
*
* Clears the interrupt.
* The interrupt must be cleared with this function so that the hardware
* can set subsequent interrupts and those interrupts can be forwarded
* to the interrupt controller, if enabled.
*
* \param base
* Pointer to structure describing registers
*
* \param intrMask
* The mask of interrupts to clear.
* - Bit 0: EOS_INTR
* - Bit 1: OVERFLOW_INTR
* - Bit 2: FW_COLLISION_INTR
*
* \return None
*
*******************************************************************************/
__STATIC_INLINE void Cy_SAR_ClearInterrupt(SAR_Type *base, uint32_t intrMask)
{
    CY_ASSERT_L2(CY_SAR_INTRMASK(intrMask));

    base->INTR = intrMask;
}

/*******************************************************************************
* Function Name: Cy_SAR_SetSwInterrupt
****************************************************************************//**
*
* Triggers an interrupt with software.
*
* \param base
* Pointer to structure describing registers
*
* \param intrMask
* The mask of interrupts to set.
* - Bit 0: EOS_INTR
* - Bit 1: OVERFLOW_INTR
* - Bit 2: FW_COLLISION_INTR
*
* \return None
*
*******************************************************************************/
__STATIC_INLINE void Cy_SAR_SetSwInterrupt(SAR_Type *base, uint32_t intrMask)
{
    CY_ASSERT_L2(CY_SAR_INTRMASK(intrMask));

    base->INTR_SET = intrMask;
}

/*******************************************************************************
* Function Name: Cy_SAR_SetInterruptMask
****************************************************************************//**
*
* Enables which interrupts can trigger the CPU interrupt controller.
*
* \param base
* Pointer to structure describing registers
*
* \param intrMask
* The mask of interrupts to enable.
* - Bit 0: EOS_INTR
* - Bit 1: OVERFLOW_INTR
* - Bit 2: FW_COLLISION_INTR
*
* \return None
*
*******************************************************************************/
__STATIC_INLINE void Cy_SAR_SetInterruptMask(SAR_Type *base, uint32_t intrMask)
{
    CY_ASSERT_L2(CY_SAR_INTRMASK(intrMask));

    base->INTR_MASK = intrMask;
}

/*******************************************************************************
* Function Name: Cy_SAR_GetInterruptMask
****************************************************************************//**
*
* Returns which interrupts can trigger the CPU interrupt controller
* as configured by \ref Cy_SAR_SetInterruptMask.
*
* \param base
* Pointer to structure describing registers
*
* \return uint32_t
* Interrupt mask
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SAR_GetInterruptMask(const SAR_Type *base)
{
    return base->INTR_MASK;
}

/*******************************************************************************
* Function Name: Cy_SAR_GetInterruptStatusMasked
****************************************************************************//**
*
* Returns the bitwise AND between the interrupt request and mask registers.
* See \ref Cy_SAR_GetInterruptStatus and \ref Cy_SAR_GetInterruptMask.
*
* \param base
* Pointer to structure describing registers
*
* \return uint32_t
* Bitwise AND between the interrupt request and mask registers
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SAR_GetInterruptStatusMasked(const SAR_Type *base)
{
    return base->INTR_MASKED;
}

/*******************************************************************************
* Function Name: Cy_SAR_GetLimitInterruptStatus
****************************************************************************//**
*
* Returns the limit interrupt register status.
*
* \param base
* Pointer to structure describing registers
*
* \return Interrupt status
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SAR_GetLimitInterruptStatus(const SAR_Type *base)
{
    return base->RANGE_INTR;
}

/*******************************************************************************
* Function Name: Cy_SAR_ClearLimitInterrupt
****************************************************************************//**
*
* Clears the limit interrupt for the specified channel mask.
* The interrupt must be cleared with this function so that
* the hardware can set subset interrupts and those interrupts can
* be forwarded to the interrupt controller, if enabled.
*
* \param base
* Pointer to structure describing registers
*
* \param chanMask
* The channel mask. Bit 0 is for channel 0, etc.
*
* \return None
*
*******************************************************************************/
__STATIC_INLINE void Cy_SAR_ClearLimitInterrupt(SAR_Type *base, uint32_t chanMask)
{
    CY_ASSERT_L2(CY_SAR_CHANMASK(chanMask));

    base->RANGE_INTR = chanMask;
}

/*******************************************************************************
* Function Name: Cy_SAR_SetSwLimitInterrupt
****************************************************************************//**
*
* Triggers a limit interrupt with software for the specific channel mask.
*
* \param base
* Pointer to structure describing registers
*
* \param chanMask
* The channel mask. Bit 0 is for channel 0, etc.
*
* \return None
*
*******************************************************************************/
__STATIC_INLINE void Cy_SAR_SetSwLimitInterrupt(SAR_Type *base, uint32_t chanMask)
{
    CY_ASSERT_L2(CY_SAR_CHANMASK(chanMask));

    base->RANGE_INTR_SET = chanMask;
}

/*******************************************************************************
* Function Name: Cy_SAR_SetLimitInterruptMask
****************************************************************************//**
*
* Enables which channels can trigger a limit interrupt.
*
* \param base
* Pointer to structure describing registers
*
* \param chanMask
* The channel mask. Bit 0 is for channel 0, etc.
*
* \return None
*
*******************************************************************************/
__STATIC_INLINE void Cy_SAR_SetLimitInterruptMask(SAR_Type *base, uint32_t chanMask)
{
    CY_ASSERT_L2(CY_SAR_CHANMASK(chanMask));

    base->RANGE_INTR_MASK = chanMask;
}

/*******************************************************************************
* Function Name: Cy_SAR_GetLimitInterruptMask
****************************************************************************//**
*
* Returns which interrupts can trigger a range interrupt as configured by
* \ref Cy_SAR_SetLimitInterruptMask.
*
* \param base
* Pointer to structure describing registers
*
* \return uint32_t
* The limit mask register
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SAR_GetLimitInterruptMask(const SAR_Type *base)
{
    return base->RANGE_INTR_MASK;
}

/*******************************************************************************
* Function Name: Cy_SAR_GetLimitInterruptStatusMasked
****************************************************************************//**
*
* Returns the bitwise AND between the limit interrupt request and mask registers.
* See \ref Cy_SAR_GetLimitInterruptStatus and \ref Cy_SAR_GetLimitInterruptMask.
*
* \param base
* Pointer to structure describing registers
*
* \return uint32_t
* Bitwise AND between the interrupt request and mask registers
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SAR_GetLimitInterruptStatusMasked(const SAR_Type *base)
{
    return base->RANGE_INTR_MASKED;
}

/*******************************************************************************
* Function Name: Cy_SAR_GetSatInterruptStatus
****************************************************************************//**
*
* Returns the saturate interrupt register status.
*
* \param base
* Pointer to structure describing registers
*
* \return Interrupt status
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SAR_GetSatInterruptStatus(const SAR_Type *base)
{
    return base->SATURATE_INTR;
}

/*******************************************************************************
* Function Name: Cy_SAR_ClearSatInterrupt
****************************************************************************//**
*
* Clears the saturate interrupt for the specified channel mask.
* The interrupt must be cleared with this function so that the hardware
* can set subsequent interrupts and those interrupts can be forwarded
* to the interrupt controller, if enabled.
*
* \param base
* Pointer to structure describing registers
*
* \param chanMask
* The channel mask. Bit 0 is for channel 0, etc.
*
* \return None
*
*******************************************************************************/
__STATIC_INLINE void Cy_SAR_ClearSatInterrupt(SAR_Type *base, uint32_t chanMask)
{
    CY_ASSERT_L2(CY_SAR_CHANMASK(chanMask));

    base->SATURATE_INTR = chanMask;
}

/*******************************************************************************
* Function Name: Cy_SAR_SetSwSatInterrupt
****************************************************************************//**
*
* Triggers a saturate interrupt with software for the specific channel mask.
*
* \param base
* Pointer to structure describing registers
*
* \param chanMask
* The channel mask. Bit 0 is for channel 0, etc.
*
* \return None
*
*******************************************************************************/
__STATIC_INLINE void Cy_SAR_SetSwSatInterrupt(SAR_Type *base, uint32_t chanMask)
{
    CY_ASSERT_L2(CY_SAR_CHANMASK(chanMask));

    base->SATURATE_INTR_SET = chanMask;
}

/*******************************************************************************
* Function Name: Cy_SAR_SetSatInterruptMask
****************************************************************************//**
*
* Enables which channels can trigger a saturate interrupt.
*
* \param base
* Pointer to structure describing registers
*
* \param chanMask
* The channel mask. Bit 0 is for channel 0, etc.
*
* \return None
*
*******************************************************************************/
__STATIC_INLINE void Cy_SAR_SetSatInterruptMask(SAR_Type *base, uint32_t chanMask)
{
    CY_ASSERT_L2(CY_SAR_CHANMASK(chanMask));

    base->SATURATE_INTR_MASK = chanMask;
}

/*******************************************************************************
* Function Name: Cy_SAR_GetSatInterruptMask
****************************************************************************//**
*
* Returns which interrupts can trigger a saturate interrupt as configured
* by \ref Cy_SAR_SetSatInterruptMask.
*
* \param base
* Pointer to structure describing registers
*
* \return uint32_t
* The limit mask register
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SAR_GetSatInterruptMask(const SAR_Type *base)
{
    return base->SATURATE_INTR_MASK;
}

/*******************************************************************************
* Function Name: Cy_SAR_GetSatInterruptStatusMasked
****************************************************************************//**
*
* Returns the bitwise AND between the saturate interrupt request and mask registers.
* See \ref Cy_SAR_GetSatInterruptStatus and \ref Cy_SAR_GetSatInterruptMask.
*
* \param base
* Pointer to structure describing registers
*
* \return uint32_t
* Bitwise AND between the interrupt request and mask registers
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SAR_GetSatInterruptStatusMasked(const SAR_Type *base)
{
    return base->SATURATE_INTR_MASKED;
}

/** \} */

/** \addtogroup group_sar_functions_helper
* \{
*/
/*******************************************************************************
* Function Name: Cy_SAR_IsChannelDifferential
****************************************************************************//**
*
* Returns true of channel is differential, else false.
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel to check, starting at 0.
*
* \return bool
* A false is return if chan is invalid.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SAR_IsChannelDifferential(const SAR_Type *base, uint32_t chan)
{
    return !Cy_SAR_IsChannelSingleEnded(base, chan);
}
/** \} */

/** \addtogroup group_sar_functions_switches
* \{
*/
/*******************************************************************************
* Function Name: Cy_SAR_SetVssaVminusSwitch
****************************************************************************//**
*
* Sets the state (open or closed) of the switch between VSSA_KELVIN and VMINUS
* through firmware.
*
* When the SAR sequencer is controlling the SARMUX switches,
* there is no need to use this function.
*
* \param base
* Pointer to structure describing registers
*
* \param state
* A value of the enum \ref cy_en_sar_switch_state_t.
*
* \return None
*
*******************************************************************************/
__STATIC_INLINE void Cy_SAR_SetVssaVminusSwitch(SAR_Type *base, cy_en_sar_switch_state_t state)
{
    Cy_SAR_SetAnalogSwitch(base, CY_SAR_MUX_SWITCH0, SAR_MUX_SWITCH0_MUX_FW_VSSA_VMINUS_Msk, state);
}

/*******************************************************************************
* Function Name: Cy_SAR_SetVssaSarSeqCtrl
****************************************************************************//**
*
* Sets whether the SAR sequencer will control the switch between VSSA_KELVIN and VMINUS
* or not.
*
* \param base
* Pointer to structure describing registers
*
* \param ctrl
* A value of the enum \ref cy_en_sar_switch_sar_seq_ctrl_t.
*
* \return None
*
*******************************************************************************/
__STATIC_INLINE void Cy_SAR_SetVssaSarSeqCtrl(SAR_Type *base, cy_en_sar_switch_sar_seq_ctrl_t ctrl)
{
    Cy_SAR_SetSwitchSarSeqCtrl(base, SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_VSSA_Msk, ctrl);
}
/** \} */

/** \} group_sar_functions */

#if defined(__cplusplus)
}
#endif

#endif /** !defined(CY_SAR_H) */

/** \} group_sar */

/* [] END OF FILE */

