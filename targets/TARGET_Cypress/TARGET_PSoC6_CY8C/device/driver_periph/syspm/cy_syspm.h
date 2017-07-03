/***************************************************************************//**
* \file cy_syspm.h
* \version 1.0
*
* Provides the function definitions for the power management API.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*
*******************************************************************************/

/**
* \defgroup group_syspm System Power Management (SysPm)
* \{
* Power Modes
*
* Devices support the following power modes (in the order of
* high-to-low power consumption): Active, Sleep, Low Power (LP) Active,
* LP Sleep, DeepSleep, and Hibernate.
* Active and Sleep are the standard ARM-defined power modes, supported by the
* ARM CPUs.
* DeepSleep is a lower-power mode where high-frequency clocks are disabled.
* Most of the register state is retained and the platform supports saving a
* configurable amount of the SRAM state.
* Hibernate is an even lower power mode that is entered from firmware,
* just like DeepSleep. However, on a wakeup the CPU and all peripherals go
* through a full reset.
*
* The power mode library is used to enter low-power states to reduce system
* power consumption in power sensitive designs. This library allows the user to
* individually power down each CPU into Sleep or DeepSleep.
*
* <table class="doxtable">
*   <tr><th>CM0+ Mode</th><th>CM4 Mode</th><th>Power Mode (SRSS)</th>
*   <th>Mode Description</th></tr>
*   <tr>
*     <td>Active</td>
*     <td>Active</td>
*     <td>Normal</td>
*     <td>All Clocks and peripherals may operate or optionally can
*         be disabled. Current load limitation is set as for
*         active power mode(*1).</td>
*   </tr>
*   <tr>
*     <td>Sleep</td>
*     <td>Active</td>
*     <td>Normal</td>
*     <td>All Clocks and peripherals may operate or optionally can
*         be disabled. Current load limitation is set as for
*         active power mode(*1).</td>
*   </tr>
*   <tr>
*     <td>DeepSleep</td>
*     <td>Active</td>
*     <td>Normal</td>
*     <td>All Clocks and peripherals may operate or optionally can
*         be disabled. Current load limitation is set as for
*         active power mode(*1).</td>
*   </tr>
*   <tr>
*     <td>Sleep</td>
*     <td>Sleep</td>
*     <td>Normal</td>
*     <td>All Clocks and peripherals may operate, CPUs Off. Current load
*         limitation is set as for active power mode(*1).</td>
*   </tr>
*   <tr>
*     <td>DeepSleep</td>
*     <td>Sleep</td>
*     <td>Normal</td>
*     <td>All Clocks and peripherals may operate, CPUs Off. Current load
*         limitation is set as for active power mode(*1).</td>
*   </tr>
*   <tr>
*     <td>Active</td>
*     <td>DeepSleep</td>
*     <td>Normal</td>
*     <td>All Clocks and peripherals may operate or optionally can
*         be disabled. Current load limitation is set as for
*         active power mode(*1).</td>
*   </tr>
*   <tr>
*     <td>Sleep</td>
*     <td>DeepSleep</td>
*     <td>Normal</td>
*     <td>All Clocks and peripherals may operate, CPUs Off. Current load
*         limitation is set as for active power mode(*1).</td>
*   </tr>
*   <tr>
*     <td>DeepSleep</td>
*     <td>DeepSleep</td>
*     <td>DeepSleep</td>
*     <td>All the MHz clocks are off, but so is the DSI routing, and most Analog
*         and voltage references are off by default. Only ILO/WCO may operate,
*         limited peripherals.</td>
*   </tr>
*   <tr>
*     <td>Active (LPActive)</td>
*     <td>Active (LPActive)</td>
*     <td>Low power</td>
*     <td>All the MHz clocks frequencies are decreased to meet current load
*         limitations (*1). Peripherals are also slowed and optionally disabled
*         to meet current load limitations in LPActive power mode.</td>
*   </tr>
*   <tr>
*     <td>Sleep (LPSleep)</td>
*     <td>Active (LPActive)</td>
*     <td>Low power</td>
*     <td>All the MHz clocks frequencies are decreased to meet current load
*         limitations (*1). Peripherals are also slowed and optionally disabled
*         to meet current load limitations in LPActive power mode.</td>
*   </tr>
*   <tr>
*     <td>DeepSleep</td>
*     <td>Active (LPActive)</td>
*     <td>Low power</td>
*     <td>All the MHz clocks frequencies are decreased to meet current load
*         limitations (*1). Peripherals are also slowed and optionally disabled
*         to meet current load limitations in LPActive power mode.</td>
*   </tr>
*   <tr>
*     <td>Active (LPActive)</td>
*     <td>Sleep (LPSleep)</td>
*     <td>Low power</td>
*     <td>All the MHz clocks frequencies are decreased to meet current load
*         limitations (*1). Peripherals are also slowed and optionally disabled
*         to meet current load limitations in LPActive power mode.</td>
*   </tr>
*   <tr>
*     <td>Sleep (LPSleep)</td>
*     <td>Sleep (LPSleep)</td>
*     <td>Low power</td>
*     <td>All the MHz clocks frequencies are decreased to meet current load
*         limitations (*1). Peripherals are also slowed and optionally disabled
*         to meet current load limitations in LPActive power mode.
*         CPUs are off.</td>
*   </tr>
*   <tr>
*     <td>DeepSleep</td>
*     <td>Sleep (LPSleep)</td>
*     <td>Low power</td>
*     <td>All the MHz clocks frequencies are decreased to meet current load
*         limitations (*1). Peripherals are also slowed and optionally disabled
*         to meet current load limitations in LPActive power mode.
*         CPUs are off</td>
*   </tr>
*   <tr>
*     <td>Active (LPActive)</td>
*     <td>DeepSleep</td>
*     <td>Low power</td>
*     <td>All the MHz clocks frequencies are decreased to meet current load
*         limitations (*1). Peripherals are also slowed and optionally disabled
*         to meet current load limitations in LPActive power mode.</td>
*   </tr>
*   <tr>
*     <td>DeepSleep</td>
*     <td>DeepSleep</td>
*     <td>DeepSleep</td>
*     <td>The entire chip is in a DeepSleep mode, since it forces the MHz
*         clocks, DSI, UDBs, most peripherals, analog, etc., to be shut down.
*         Only ILO/WCO may operate, limited peripherals</td>
*   </tr>
*   <tr>
*     <td>-</td>
*     <td>-</td>
*     <td>Hibernate</td>
*     <td>Once Hibernate mode is set, everything goes into Hibernate mode.
*         Only ILO may operate to assert wake-up device using WDT or RTC.</td>
*   </tr>
* </table>
*
* (*1) For more details about power modes and current load limitations refer to
* the device Technical Reference Manual (TRM).
*
* Power modes:
* * Active - In this mode the CPU executes code, and all logic and memories
* are powered. Firmware may decide to disable clocks for specific peripherals
* and to power down specific analog power domains. The chip enters Active upon
* initial reset. For internally-generated resets, the chip enters the most
* recently used power mode among Active and LPActive.
*
* * Sleep - In this mode the CPU does not execute code and its clock is
* stopped. It is identical to ACTIVE from a peripheral point of view.
*
* * LPActive - LPActive (Low Power Active) is similar to Active, with clock
* restrictions and limited/slowed peripheral to achieve a lower system current.
*
* * LPSleep - LPSleep (Low Power Sleep) mode operates the same as Sleep mode
* with the clocking limitations of LPActive mode to further reduce system power.
*
* * DeepSleep - In DeepSleep mode the main system clock is off for CPU, Flash,
* and high-speed logic. Optionally, the low frequency (32.768 kHz) clock
* remains on, and low-frequency peripherals continue operating.
* Peripherals that do not need a clock or receive a clock from their external
* interface (e.g., I2C/SPI) continue operating, and all circuits that draw
* current from the Vccdpslp supply are subject to the current sourcing
* limitations of the DeepSleep Regulator (if the core is supplied by the
* Linear core regulator). If the core is supplied by the SIMO Buck regulator all
* deepsleep peripherals are supplied by Vbuck1.
* A normal wakeup from DeepSleep returns to either LPActive, LPSleep, Active, or
* Sleep, depending on the previous state and programmed behavior for the
* configured wakeup interrupt.
* Likewise, a debug wakeup from DeepSleep returns to LPSleep or Sleep, depending
* on which mode was used to enter DeepSleep.
*
* * Hibernate - Is the lowest power mode available when external supplies are
* present. It is intended for applications in a dormant state.
* Hibernate is implemented using high voltage logic and switches off all
* internal regulators. The Backup domain continues functioning normally.
* The I/O's state is frozen so that the output driver state is held.
* Note in this mode the CPU and all peripherals lose state, so the system and
* firmware reboot on a wakeup event.
* Wakeup is supported from device specific pin(s) with programmable
* polarity. Additionally, unregulated peripherals can wake the device under
* some conditions. For example, a low-power comparator can wake the device by
* comparing two external voltages but may not support comparison to an
* internally-generated voltage. The Backup domain remains functional, and if
* present it can schedule an alarm to wake the device from Hibernate using RTC.
* The combination of Hibernate and Backup alarms can yield low current duty
* cycling for applications, such as waking once per second for a CapSense
* scan. Alternatively, the Watchdog Timer (WDT) can be configured to wake-up the device by
* WDT interrupt.
* Backup memory (BMEM) can be used to store system state for use on the
* next reboot.
*
* The system can wakeup from Hibernate power mode by configuring:
* * Wakeup pins
* * WDT interrupt
* * LPComparator
* * WDT reset - this can be configured without using the syspm driver. Just
* configure and enable the WDT before entering Hibernate power mode. All
* other events require use of the API from the syspm driver.
*
* * XRES - is the state of the part when XRES is applied. This is obviously a
* reset condition, but it is also considered a power mode because some low-power
* applications, such as remote controls, expect very low current consumption
* when XRES is asserted.
*
* * OFF - The OFF state simply represents the state of the part with no power
* applied to it (except perhaps the Backup domain). In some applications, power
* to the device will be switched on and off  by another device in the system to
* make the product implement the lowest power consumption level.
*
* Wakeup time after a reset is device-dependent because of the boot run-time.
* The wakeup time varies depending on the number and type of trim parameters and
* the size of device Flash.
*
* For ARM-based devices, an interrupt is required for the CPU to
* wake up.
*
* * All pending interrupts should be cleared before the device is put into a
* low-power mode, even if they are masked.
*
* \section group_syspm_section_configuration Configuration Considerations
* After initial device boot the device goes into Active power mode. After some
* code execution the device could be sent into low power modes. The
* Cy_SysPm_Sleep() and Cy_SysPm_DeepSleep() functions only switch the core that
* calls the function into Sleep or DeepSleep. To set the whole device into Sleep
* or DeepSleep device, ensure that each core calls the sleep functions.
*
* There are situations when the device does not switch into DeepSleep power
* mode immediately after the second core calls Cy_SysPm_DeepSleep(). The device
* will switch into DeepSleep mode automatically a little bit later, after the
* low-power circuits are ready to switch into DeepSleep.
* Refer to the Cy_SysPm_DeepSleep() description for more details.
*
* If you call Cy_SysPm_Hibernate() the device will be switched
* into Hibernate power mode directly, as there is no handshake between cores.
*
* Before switching into LPActive power mode ensure that the device meets the
* current load limitation by decreasing the Mhz clocks, and slowing or disabling
* peripherals.
* Refer to the device TRM for more details about current load limitations in
* different low-power modes.
*
* The syspm driver implements a syspm callbacks mechanism. Using a
* syspm callback, you can perform actions right before entering
* low-power mode, and directly after switching out of low-power mode. With the
* callback mechanism you can prevent switching into low power mode if, for
* example, some required peripheral is not ready to enter into requested
* low-power mode.
*
* Fill in the cy_stc_syspm_callback_t structure for each callback function.
* Each callback function is of the type cy_SysPMCallback. You assign
* the mode for each callback function. Refer to cy_en_syspm_callback_mode_t.
* Also refer to Cy_SysPm_Sleep(), Cy_SysPm_DeepSleep(), Cy_SysPm_EnterLpMode(),
* Cy_SysPm_ExitLpMode(), Cy_SysPm_Hibernate() for more details.
*
* Refer to Cy_SysPm_RegisterCallback() description for more details
* about registering a callback. You may have up to 32 callback functions.
*
* \section group_syspm_section_more_information More Information
*
* For more information on the Power Modes (syspm) driver,
* refer to the Technical Reference Manual (TRM).
*
* \section group_syspm_MISRA MISRA-C Compliance
* The syspm driver does not have any specific deviations.
*
* \section group_syspm_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_syspm_macro Macro
* \defgroup group_syspm_functions Functions
* \defgroup group_syspm_data_structures Data Structures
* \defgroup group_syspm_data_enumerates Enumerated types
*/

#ifndef _CY_SYSPM_H_
#define _CY_SYSPM_H_

#include <stdbool.h>
#include <stddef.h>
#include "cy_device_headers.h"
#include "syslib/cy_syslib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
*       Register Constants
*******************************************************************************/

/**
* \addtogroup group_syspm_macro
* \{
*/

/** Driver major version */
#define CY_SYSPM_DRV_VERSION_MAJOR       1

/** Driver minor version */
#define CY_SYSPM_DRV_VERSION_MINOR       0

/** \cond INTERNAL */

/** Internal define for SysPm_DeepSleep() function */
#define CY_SYSPM_PWR_TRIM_WAKE_CNTS                      (0x8Bu)

/** Internal define for Cy_SysPm_ExitLpMode() function */
#define CY_SYSPM_WAIT_DELAY_TRYES                        (100u)

/** Internal define for SysPm_SwitchToSimoBuck() function */
#define CY_SYSPM_1M_THRESHOLD                            (1000000u)

/** Internal define for SysPm_WakeupPinLevel() function */
#define CY_SYSPM_PWR_MASK_HIBPIN_PIN_MASK                (0x03u)

/** Internal define for SysPm_WakeupPinLevel() function */
#define CY_SYSPM_WAKEUP_PIN_BIT                          (1ul)

/* Internal define for Cy_SysPm_WakeupLpCompLevel() function */
#define CY_SYSPM_LPCOMPARATOR_BIT                        (4ul)

/* Define for mask for LPComs in Cy_SysPm_WakeupLpCompLevel() function */
#define CY_SYSPM_LPCOMPARATOR_MASK                       (12u)

/** \endcond */

/** syspm driver identifier */
#define CY_SYSPM_ID                                      (CY_PDL_DRV_ID(0x10u))

/**
* \defgroup group_syspm_return_status Power Mode Status
* \{
* Current power mode status defines
*/

/** CM4 is Active */
#define CY_SYSPM_STATUS_CM4_ACTIVE       (0x01u)

/** CM4 is in Sleep */
#define CY_SYSPM_STATUS_CM4_SLEEP        (0x02u)

/** CM4 is in DeepSleep */
#define CY_SYSPM_STATUS_CM4_DEEPSLEEP    (0x04u)

/** CM4 is Lop Power mode */
#define CY_SYSPM_STATUS_CM4_LOWPOWER     (0x80u)

/** CM0 is Active */
#define CY_SYSPM_STATUS_CM0_ACTIVE       ((uint32_t) ((uint32_t)0x01u << 8u))

/** CM0 is in Sleep */
#define CY_SYSPM_STATUS_CM0_SLEEP        ((uint32_t) ((uint32_t)0x02u << 8u))

/** CM0 is in DeepSleep */
#define CY_SYSPM_STATUS_CM0_DEEPSLEEP    ((uint32_t) ((uint32_t)0x04u << 8u))

/** CM4 is in Low Power mode */
#define CY_SYSPM_STATUS_CM0_LOWPOWER     ((uint32_t) ((uint32_t)0x80u << 8u))

/** Device is in Low Power mode*/
#define CY_SYSPM_STATUS_SYSTEM_LOWPOWER  ((uint32_t) (CY_SYSPM_STATUS_CM0_LOWPOWER | (CY_SYSPM_STATUS_CM4_LOWPOWER)))

/** \} group_syspm_return_status */

/** Define to update token to indicate Hibernate mode transition */
#define CY_SYSPM_PWR_HIBERNATE_TOKEN_HIBERNATE              (0xf1u)

/** Wait time for transition device from Active into the LPActive */
#define CY_SYSPM_PWR_ACTIVE_TO_LP_WAIT_US                   (1u)

/** Wait time for transition device from Active into the LPActive before Active
* reference is settled
*/
#define CY_SYSPM_PWR_LP_TO_ACTIVE_WAIT_BEFORE_US    (8u)

/** Wait time for transition device from Active into the LPActive after Active
* reference is settled
*/
#define CY_SYSPM_PWR_LP_TO_ACTIVE_WAIT_AFTER_US    (1u)

/** Maximum callbacks number */
#define CY_SYSPM_PWR_MAX_CALLBACKS_NUMBER         (32u)

/** Mask to unlock Hibernate mode */
#define CY_SYSPM_PWR_HIBERNATE_UNLOCK             ((uint32_t) 0x3Au << SRSS_PWR_HIBERNATE_UNLOCK_Pos)

/** Mask to retain Hibernate status */
#define CY_SYSPM_PWR_RETAIN_HIBERNATE_STATUS      ((uint32_t) SRSS_PWR_HIBERNATE_TOKEN_Msk |\
                                                              SRSS_PWR_HIBERNATE_MASK_HIBALARM_Msk |\
                                                              SRSS_PWR_HIBERNATE_MASK_HIBWDT_Msk |\
                                                              SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Msk |\
                                                              SRSS_PWR_HIBERNATE_MASK_HIBPIN_Msk)


/** Maximum value of callbacks */
#define CY_SYSPM_PWR_MAX_PLL_NUMBER                     (16u)

/** Definition for delay of SIMO Buck supply regulator after it is enabled */
#define CY_SYSPM_SIMO_BUCK_CORE_SUPPLY_STABLE_US       (900u)

/** Definition for delay of SIMO Buck output 2 */
#define CY_SYSPM_SIMO_BUCK_BLE_SUPPLY_STABLE_US        (600u)

/** Definition for delay of Linear Regulator after it output voltage
* is changed
*/
#define CY_SYSPM_LDO_IS_STABLE_US                      (9u)

/** Definition for delay of core supply regulator after it output voltage
* is changed
*/
#define CY_SYSPM_SIMO_BUCK_IS_STABLE_US                 (200u)
/** \} group_syspm_macro */

/*******************************************************************************
*       Configuration Structures
*******************************************************************************/

/**
* \addtogroup group_syspm_data_enumerates
* \{
*/

/** This enumeration is used to initialize the wakeup pin state */
typedef enum
{
    CY_SYSPM_PIN_DISABLED,    /**< Disable selected wakeup pin */
    CY_SYSPM_PIN_HIGH_LEVEL,  /**< Configure high level for selected wakeup-pin */
    CY_SYSPM_PIN_LOW_LEVEL    /**< Configure low level for selected wakeup-pin */
} cy_en_syspm_pinstate_t;


/** This enumeration is used to initialize the Low-power Comparator
* (LPComp) state
*/
typedef enum
{
    CY_SYSPM_LPCOMP_DISABLED,    /**< Disable selected LPComp */
    CY_SYSPM_LPCOMP_HIGH_LEVEL,  /**< Configure high level for selected LPComp */
    CY_SYSPM_LPCOMP_LOW_LEVEL    /**< Configure low level for selected LPComp */
} cy_en_syspm_lpcomp_state_t;

/**
* This enumeration is used to initialize a wait action - an interrupt or
* event. Refer to the CMSIS for the WFE and WFI instruction explanations.
*/
typedef enum
{
    CY_SYSPM_WAIT_FOR_INTERRUPT,      /**< Wait for interrupt */
    CY_SYSPM_WAIT_FOR_EVENT           /**< Wait for event */
} cy_en_syspm_waitfor_t;

/**
* The enumeration for core voltages for both the SIMO Buck and the
* Linear Regulator
*/
typedef enum
{
    CY_SYSPM_LDO_VOLTAGE_0_9V = 0x05u,         /**< 0.9 V nominal core voltage */
    CY_SYSPM_LDO_VOLTAGE_1_1V = 0x17u          /**< 1.1 V nominal core voltage */
} cy_en_syspm_ldo_voltage_t;

/**
* The enumeration for core voltages for both the SIMO Buck and the
* Linear Regulator
*/
typedef enum
{
    CY_SYSPM_SIMO_BUCK_OUT1_VOLTAGE_0_9V = 0x01u,         /**< 0.9 V nominal core voltage */
    CY_SYSPM_SIMO_BUCK_OUT1_VOLTAGE_1_1V = 0x05u          /**< 1.1 V nominal core voltage */
} cy_en_syspm_simo_buck_voltage1_t;


/**
* The enumeration for core voltages for both the SIMO Buck and the
* Linear Regulator
*/
typedef enum
{
    CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_0_95V = 0u,    /**< 0.95 V nominal core voltage */
    CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_0V,          /**< 1.0 V nominal core voltage */
    CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_05V,         /**< 1.05 V nominal core voltage */
    CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_1V,          /**< 1.1 V nominal core voltage */
    CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_2V,          /**< 1.2 V nominal core voltage */
    CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_3V,          /**< 1.3 V nominal core voltage */
    CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_4V,          /**< 1.4 V nominal core voltage */
    CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_5V,          /**< 1.5 V nominal core voltage */
} cy_en_syspm_simo_buck_voltage2_t;

/**
* The enumeration are used to configure SIMO Buck regulator outputs
*/
typedef enum
{
    CY_SYSPM_BUCK_VBUCK_1,          /**< SIMO Buck output 1 Voltage (Vback1) */
    CY_SYSPM_BUCK_VRF               /**< SIMO Buck out 2 Voltage (Vbackrf) */
} cy_en_syspm_buck_out_t;

/** SysPm status definitions */
typedef enum
{
    CY_SYSPM_SUCCESS       = 0x00u,                                        /**< Successful */
    CY_SYSPM_BAD_PARAM     = CY_SYSPM_ID | CY_PDL_STATUS_ERROR | 0x01u,    /**< One or more invalid parameters */
    CY_SYSPM_TIMEOUT       = CY_SYSPM_ID | CY_PDL_STATUS_ERROR | 0x02u,    /**< Time-out occurs */
    CY_SYSPM_INVALID_STATE = CY_SYSPM_ID | CY_PDL_STATUS_ERROR | 0x03u,    /**< Operation not setup or is in an
                                                                                improper state */
    CY_SYSPM_FAIL          = CY_SYSPM_ID | CY_PDL_STATUS_ERROR | 0xFFu     /**< Unknown failure */
} cy_en_syspm_status_t;

/**
* This enumeration is used for selecting the low power mode on which the
* appropriate registered callback handler will be executed. For example,
* registered callback with type CY_SYSPM_SLEEP will be executed while
* switching into the Sleep power mode.
*/
typedef enum
{
    CY_SYSPM_SLEEP,         /**< Sleep enum callback type */
    CY_SYSPM_DEEPSLEEP,     /**< DeepSleep enum callback type */
    CY_SYSPM_HIBERNATE,     /**< Hibernate enum callback type */
    CY_SYSPM_ENTER_LP_MODE, /**< Enter LPActive mode enum callback type */
    CY_SYSPM_EXIT_LP_MODE,  /**< Exit LPActive mode enum callback type */
} cy_en_syspm_callback_type_t;

/** Callback mode enumeration. This enum define the callback mode */
typedef enum
{
    CY_SYSPM_CHECK_READY,       /**< Callbacks with this mode are executed before entering into low-power mode.
                                     It checks if device is ready to enter low-power mode */
    CY_SYSPM_CHECK_FAIL,        /**< Callbacks with this mode are executed after the previous callbacks execution
                                     with CY_SYSPM_CHECK_READY returned CY_SYSPM_FAIL. Callback with
                                     CY_SYSPM_CHECK_FAIL mode should rollback the actions which were done in
                                     callbacks executed previously with CY_SYSPM_CHECK_READY */
    CY_SYSPM_BEFORE_ENTER,      /**< Actions that should be done before enter low-power mode */
    CY_SYSPM_AFTER_EXIT,        /**< Actions that should be done after exit low-power mode */
} cy_en_syspm_callback_mode_t;

/** \} group_syspm_data_enumerates */

/**
* \addtogroup group_syspm_data_structures
* \{
*/

/** Type for syspm callbacks */
typedef cy_en_syspm_status_t (*Cy_SysPmCallback) (void *base, void *content, cy_en_syspm_callback_mode_t mode);

/** The structure which contain the syspm callback configuration elements */
typedef struct cy_stc_syspm_callback
{
    Cy_SysPmCallback Callback;                /**< Callback handler function */
    cy_en_syspm_callback_type_t type;         /**< Callback type, see \ref cy_en_syspm_callback_type_t */
    void *base;                               /**< Base address of HW instance, matches with driver name in
                                                   the API for base address */
    void *content;                            /**< Content for handler function */
    struct cy_stc_syspm_callback *nextItm;    /**< Next list item */
} cy_stc_syspm_callback_t;
/** \} group_syspm_data_structures */

/**
* \addtogroup group_syspm_functions
* \{
*/
__STATIC_INLINE bool Cy_SysPm_Cm4IsActive(void);
__STATIC_INLINE bool Cy_SysPm_Cm4IsSleep(void);
__STATIC_INLINE bool Cy_SysPm_Cm4IsDeepSleep(void);
__STATIC_INLINE bool Cy_SysPm_Cm4IsLowPower(void);
__STATIC_INLINE bool Cy_SysPm_Cm0IsActive(void);
__STATIC_INLINE bool Cy_SysPm_Cm0IsSleep(void);
__STATIC_INLINE bool Cy_SysPm_Cm0IsDeepSleep(void);
__STATIC_INLINE bool Cy_SysPm_Cm0IsLowPower(void);
__STATIC_INLINE bool Cy_SysPm_IsLowPower(void);

uint32_t Cy_SysPm_ReadStatus(void);
cy_en_syspm_status_t Cy_SysPm_Sleep(cy_en_syspm_waitfor_t enWaitFor);
cy_en_syspm_status_t Cy_SysPm_DeepSleep(cy_en_syspm_waitfor_t enWaitFor);
cy_en_syspm_status_t Cy_SysPm_Hibernate(void);
cy_en_syspm_status_t Cy_SysPm_EnterLpMode(void);
cy_en_syspm_status_t Cy_SysPm_ExitLpMode(void);
void Cy_SysPm_SleepOnExit(bool enable);

void Cy_SysPm_WakeupPinLevel(uint32_t pinNum, cy_en_syspm_pinstate_t enPinLevel);
void Cy_SysPm_WakeupLpCompLevel(uint32_t lpCompNum, cy_en_syspm_lpcomp_state_t enLpCompLevel);
void Cy_SysPm_WakeupWdt(bool configureWdt);
void Cy_SysPm_WakeupRtc(bool configureRtc);

bool Cy_SysPm_RegisterCallback(cy_stc_syspm_callback_t* handler);
bool Cy_SysPm_UnregisterCallback(cy_stc_syspm_callback_t* handler);
uint32_t Cy_SysPm_ExecuteCallback(cy_en_syspm_callback_type_t type,
                                  cy_en_syspm_callback_mode_t mode,
                                  uint32_t lastCallback);

bool Cy_SysPm_GetIoFreezeStatus(void);
void Cy_SysPm_IoFreeze(void);
void Cy_SysPm_IoUnfreeze(void);

void Cy_SysPm_LdoSetVoltage(cy_en_syspm_ldo_voltage_t voltage);
cy_en_syspm_ldo_voltage_t Cy_SysPm_LdoGetVoltage(void);
bool Cy_SysPm_LdoIsEnabled(void);

#if(0u != SRSS_SIMOBUCK_PRESENT)

    void Cy_SysPm_SwitchToSimoBuck(void);
    void Cy_SysPm_SimoBuckSetVoltage1(cy_en_syspm_simo_buck_voltage1_t voltage);
    cy_en_syspm_simo_buck_voltage1_t Cy_SysPm_SimoBuckGetVoltage1(void);

    void Cy_SysPm_EnableVoltage2(void);
    void Cy_SysPm_DisableVoltage2(void);
    void Cy_SysPm_SimoBuckSetVoltage2(cy_en_syspm_simo_buck_voltage2_t voltage);
    cy_en_syspm_simo_buck_voltage2_t Cy_SysPm_SimoBuckGetVoltage2(void);

    bool Cy_SysPm_SimoBuckOutputIsEnabled(cy_en_syspm_buck_out_t output);
    bool Cy_SysPm_SimoBuckIsEnabled(void);

    void Cy_SysPm_SimoBuckSetHwControl(bool hwControl);
    bool Cy_SysPm_SimoBuckGetHwControl(void);

#endif /* (0u != SRSS_SIMOBUCK_PRESENT) */


/*******************************************************************************
* Function Name: Cy_SysPm_Cm4IsActive
****************************************************************************//**
*
* Check if CM4 is in Active mode.
*
* \return
* true - if CM4 is in Active mode, false - if CM4 is not in Active mode.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_Cm4IsActive(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM4_ACTIVE) != 0u);
}


/*******************************************************************************
* Function Name: Cy_SysPm_Cm4IsSleep
****************************************************************************//**
*
* Check if CM4 is in Sleep mode.
*
* \return
* true - if CM4 is in Sleep mode, false - if CM4 is not in Sleep mode.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_Cm4IsSleep(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM4_SLEEP) != 0u);
}


/*******************************************************************************
* Function Name: Cy_SysPm_Cm4IsDeepSleep
****************************************************************************//**
*
* Check if CM4 is in DeepSleep mode.
*
* \return
* true - if CM4 is in DeepSleep mode, false - if CM4 is not in DeepSleep mode.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_Cm4IsDeepSleep(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM4_DEEPSLEEP) != 0u);
}


/*******************************************************************************
* Function Name: Cy_SysPm_Cm4IsLowPower
****************************************************************************//**
*
* Check if CM4 is in LowPower mode.
*
* \return
* true - if CM4 is in LowPower mode, false - if CM4 is not in LowPower mode.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_Cm4IsLowPower(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM4_LOWPOWER) != 0u);
}


/*******************************************************************************
* Function Name: Cy_SysPm_Cm0IsActive
****************************************************************************//**
*
* Check if CM0+ is in Active mode.
*
* \return
* true - if CM0+ is in Active mode, false - if CM0+ is not in Active mode.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_Cm0IsActive(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM0_ACTIVE) != 0u);
}


/*******************************************************************************
* Function Name: Cy_SysPm_Cm0IsSleep
****************************************************************************//**
*
* Check if CM0+ is in Sleep mode.
*
* \return
* true - if CM0+ is in Sleep mode, false - if CM0+ is not in Sleep mode.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_Cm0IsSleep(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM0_SLEEP) != 0u);
}


/*******************************************************************************
* Function Name: Cy_SysPm_Cm0IsDeepSleep
****************************************************************************//**
*
* Check if CM0+ is in DeepSleep mode.
*
* \return
* true - if CM0+ is in DeepSleep mode, false - if CM0+ is not in DeepSleep mode.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_Cm0IsDeepSleep(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM0_DEEPSLEEP) != 0u);
}


/*******************************************************************************
* Function Name: Cy_SysPm_Cm0IsLowPower
****************************************************************************//**
*
* Check if CM0+ is in LowPower mode.
*
* \return
* true - if CM0+ is in LowPower mode, false - if CM0+ is not in LowPower mode.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_Cm0IsLowPower(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM0_LOWPOWER) != 0u);
}


/*******************************************************************************
* Function Name: Cy_SysPm_IsLowPower
****************************************************************************//**
*
* Check if device is in Low Power mode.
*
* \return
* true - system is in Low Power mode,
* false - if system is is not in Low Power mode.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_IsLowPower(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_SYSTEM_LOWPOWER) != 0u);
}

#ifdef __cplusplus
}
#endif

#endif /* _CY_SYSPM_H_ */

/** \} group_syspm_functions*/
/** \} group_syspm */


/* [] END OF FILE */
