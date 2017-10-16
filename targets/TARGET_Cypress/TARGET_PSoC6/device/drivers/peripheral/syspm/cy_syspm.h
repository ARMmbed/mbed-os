/***************************************************************************//**
* \file cy_syspm.h
* \version 2.0
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
* The SysPm driver is used to enter low-power states to reduce system
* power consumption in power sensitive designs. For multi-core devices this 
* library allows the user to individually power down each CPU into Sleep or
* DeepSleep.
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
*     <td>Sleep</td>
*     <td>Normal</td>
*     <td>All Clocks and peripherals may operate or optionally can
*         be disabled. Current load limitation is set as for
*         active power mode(*1).</td>
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
*     <td>All the MHz clocks frequencies should be decreased, if needed, to meet
*         current load limitations (*1). Peripherals should be slowed,
*         if needed, or/and optionally disabled to meet current load limitations
*         in LPActive power mode.</td>
*   </tr>
*   <tr>
*     <td>Sleep (LPSleep)</td>
*     <td>Active (LPActive)</td>
*     <td>Low power</td>
*     <td>All the MHz clocks frequencies should be decreased, if needed, to meet
*         current load limitations (*1). Peripherals should be slowed,
*         if needed, or/and optionally disabled to meet current load limitations
*         in LPActive power mode.</td>
*   </tr>
*   <tr>
*     <td>DeepSleep</td>
*     <td>Active (LPActive)</td>
*     <td>Low power</td>
*     <td>All the MHz clocks frequencies should be decreased, if needed, to meet
*         current load limitations (*1). Peripherals should be slowed,
*         if needed, or/and optionally disabled to meet current load limitations
*         in LPActive power mode.</td>
*   </tr>
*   <tr>
*     <td>Active (LPActive)</td>
*     <td>Sleep (LPSleep)</td>
*     <td>Low power</td>
*     <td>All the MHz clocks frequencies should be decreased, if needed, to meet
*         current load limitations (*1). Peripherals should be slowed,
*         if needed, or/and optionally disabled to meet current load limitations
*         in LPActive power mode.</td>
*   </tr>
*   <tr>
*     <td>Sleep (LPSleep)</td>
*     <td>Sleep (LPSleep)</td>
*     <td>Low power</td>
*     <td>All the MHz clocks frequencies should be decreased, if needed, to meet
*         current load limitations (*1). Peripherals should be slowed,
*         if needed, or/and optionally disabled to meet current load limitations
*         in LPActive mode. The CPUs are off.</td>
*   </tr>
*   <tr>
*     <td>DeepSleep</td>
*     <td>Sleep (LPSleep)</td>
*     <td>Low power</td>
*     <td>All the MHz clocks frequencies should be decreased, if needed, to meet
*         current load limitations (*1). Peripherals should be slowed,
*         if needed, or/and optionally disabled to meet current load limitations
*         in LPActive. The CPUs are off</td>
*   </tr>
*   <tr>
*     <td>Active (LPActive)</td>
*     <td>DeepSleep</td>
*     <td>Low power</td>
*     <td>All the MHz clocks frequencies should be decreased, if needed, to meet
*         current load limitations (*1). Peripherals should be slowed,
*         if needed, or/and optionally disabled to meet current load limitations
*         in LPActive power mode.</td>
*   </tr>
*   <tr>
*     <td>Sleep (LPSleep)</td>
*     <td>DeepSleep</td>
*     <td>Low power</td>
*     <td>All the MHz clocks frequencies should be decreased, if needed, to meet
*         current load limitations (*1). Peripherals should be slowed,
*         if needed, or/and optionally disabled to meet current load limitations
*         in LPActive power mode. The CPUs are off</td>
*   </tr>
*   <tr>
*     <td>DeepSleep</td>
*     <td>DeepSleep</td>
*     <td>DeepSleep</td>
*     <td>The entire chip is in a DeepSleep, since it forces the MHz
*         clocks, DSI, UDBs, most peripherals, analog, etc., to be shut down.
*         Only ILO/WCO may operate, limited peripherals.</td>
*   </tr>
*   <tr>
*     <td>-</td>
*     <td>-</td>
*     <td>Hibernate</td>
*     <td>Once Hibernate is set, everything goes into the hibernate power mode.
*         Only ILO may operate to assert wake-up device using WDT or RTC.</td>
*   </tr>
* </table>
*
* (*1) When the device switches into LPActive power mode the SysPm driver does 
* not decrease clocks or peripherals. The SysPm driver only switches the supply
* regulators into low power mode. It is the user's responsibility to configure 
* clocks and/or peripherals to meet current load limitation in LPActive power 
* mode. For more details about power modes and current load limitations refer to
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
* * LPActive - Low-Power Active is similar to Active, with clock
* restrictions and limited/slowed peripheral to achieve a lower system current.
*
* * LPSleep - Low-Power Sleep mode operates the same as Sleep mode
* with the clocking limitations of LPActive mode to further reduce system power.
*
* * DeepSleep - In DeepSleep mode the main system clock is off for CPU, 
* Flash, and high-speed logic. Optionally, the low frequency (32.768 kHz) clock
* remains on, and low-frequency peripherals continue operating.
* Peripherals that do not need a clock or receive a clock from their external
* interface (e.g., I2C/SPI) continue operating, and all circuits that draw
* current from the Vccdpslp supply are subject to the current sourcing
* limitations of the DeepSleep Regulator (if the core is supplied by the
* Linear core regulator). If the core is supplied by the SIMO Buck regulator all
* deep sleep peripherals are supplied by Vbuck1.
* A normal wakeup from deep sleep returns to either LPActive, LPSleep, 
* Active, or Sleep, depending on the previous state and programmed behavior for
* the configured wakeup interrupt.
* Likewise, a debug wakeup from deep sleep returns to LPSleep or Sleep, 
* depending on which mode was used to enter the deep sleep power mode.
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
* scan. Alternatively, the Watchdog Timer (WDT) can be configured to wake-up the
* device by WDT interrupt.
* Backup memory (BMEM) can be used to store system state for use on the
* next reboot.
* 
* The system can wakeup from hibernate power mode by configuring:
* * Wakeup pin
* * WDT interrupt
* * LP Comparator
* * WDT reset - this can be configured without using the SysPm driver. Just
* configure and enable the WDT before entering the hibernate power mode. All
* other events require use of the API from the SysPm driver.
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
* All pending interrupts should be cleared before the device is put into a
* low-power mode, even if they are masked.
*
* \section group_syspm_section_configuration Configuration Considerations
* After initial device boot the device goes into Active power mode. After some
* code execution the device could be sent into low power mode.
*
* For multi-core devices the Cy_SysPm_Sleep() and Cy_SysPm_DeepSleep() 
* functions only switch the core that calls the function into the sleep or the 
* deep sleep power mode. To set the whole device the sleep or the deep sleep 
* power mode, ensure that each core calls the Cy_SysPm_Sleep() or
* Cy_SysPm_DeepSleep() functions.
* 
* There are situations when the device does not switch into the deep sleep power
* mode immediately after the second core calls Cy_SysPm_DeepSleep(). The device
* will switch into the deep sleep mode automatically a little bit later, 
* after the low-power circuits are ready to switch into deep sleep.
* Refer to the Cy_SysPm_DeepSleep() description for more details.
*
* If you call Cy_SysPm_Hibernate() the device will be switched
* into the hibernate power mode directly, as there is no handshake 
* between cores.
*
* Before switching into LPActive power mode ensure that the device meets the 
* current load limitation by decreasing the Mhz clocks, and slowing or disabling
* peripherals. Refer to the device TRM for more details about current load 
* limitations in different low-power modes.
*
* The SysPm driver also provides an API to configure the external power 
* management integrated circuit (PMIC) that supplies Vddd.
* Use the API to enable the PMIC output that is routed to pmic_wakeup_out pin, 
* and configure the polarity of the PMIC input (pmic_wakeup_in) that is used to 
* wakeup the PMIC.
*
* The PMIC is automatically enabled when: <br>
* * the PMIC is locked <br>
* * the configured polarity of PMIC input and the polarity driven to 
* pmic_wakeup_in pin matches.
*
* Note that PMIC remain disabled (either the PMIC or PMIC output) only when 
* the PMIC is unlocked. See Cy_SysPm_UnlockPmic() for more details. Use 
* Cy_SysPm_IsPmicLocked() to read the current PMIC lock status.
* 
* To enable the PMIC, use these functions in this order:
* 1 Cy_SysPm_UnlockPmic();
* 2 Cy_SysPm_EnablePmic();
* 3 CY_SysPm_LockPmic();
* 
* After this sequence the PMIC will be enabled regardless to its lock state.
*
* To disable the PMIC block, call Cy_SysPm_DisablePmic() with the inverted 
* value of the current active state of the pmic_wakeup_in pin. 
* For example, assume the current state of the pmic_wakeup_in pin is active low.
* To disable the PMIC call these functions in this order:
* 1 Cy_SysPm_UnlockPmic();
* 2 Cy_SysPm_DisablePmic(CY_SYSPM_PMIC_POLARITY_HIGH);
* Do not call Cy_SysPm_LockPmic() (which automatically enables the PMIC).
* 
* While disabled, the PMIC block is automatically enabled when the 
* pmic_wakeup_in pin state is changed into high state.
*
* To disable the PMIC output, call these functions in this order:
* Cy_SysPm_UnlockPmic();
* Cy_SysPm_DisablePmicOutput();
* Do not call Cy_SysPm_LockPmic() (which automatically enables the PMIC output).
* 
* When disabled, the PMIC output is enabled when the PMIC is locked, or by 
* calling Cy_SysPm_EnablePmicOutput().
*
* \section group_syspm_callbacks SysPm Callbacks
* The SysPm driver implements a callback mechanism. Using a SysPm callback,
* you perform actions right before entering low-power mode, and directly
* after switching out of low-power mode. With the callback mechanism you 
* can prevent switching into low power mode if, for example, a peripheral 
* is not ready to enter into the requested low-power mode.
*
* \subsection group__syspm_callback_config Callback Configuration Considerations
*
* Firmware must register each callback. You may have up to 32 callback 
* functions.
* 
* For each callback, provide a cy_stc_syspm_callback_t structure. Some fields 
* in this structure are maintained by the driver. Use NULL for <b>prevItm</b>  
* and <b>nextItm</b>. The driver uses these fields to build a linked list of 
* callback functions.
*
* The <b>callbackParams</b> element is a cy_stc_syspm_callback_params_t 
* structure. The <b>mode</b> element of this structure is for internal use. 
* The driver sets that field to the correct value before calling the callback 
* function. The callback function reads the value and acts based on the
* mode. 
*
* The <b>base</b> and <b>context</b> pointers are optional and can be NULL. 
* A callback function is typically associated with a particular driver. Some 
* drivers require a base hardware address and a context. If your callback 
* routine needs access to the driver registers or context, provide those values.
* Be aware of MISRA warnings if these parameters are NULL.
*
* Call Cy_SysPm_RegisterCallback() to register each callback function. The 
* functions will be called based on the order in which they are registered. 
* In some cases they are called first to last; in other cases last to first.
*
* \warning The Cy_SysPm_RegisterCallback() function stores a pointer to the 
* cy_stc_syspm_callback_t variable. Do not modify elements of the 
* cy_stc_syspm_callback_t structure after the callback is registered. 
* It is the user's responsibility to ensure that the variable remains in scope. 
* Typically the structure is declared as a global or static variable, or as a 
* local variable in the main() function.
*
* See the SysPM Callbacks Example section for pseudocode that handles 
configuration.
*  
* Also refer to Cy_SysPm_Sleep(), Cy_SysPm_DeepSleep(), Cy_SysPm_EnterLpMode(),
* Cy_SysPm_ExitLpMode(), and Cy_SysPm_Hibernate() for more details.
*
* \subsection group__syspm_callback_flow SysPm Callbacks Flow
*
* This section explains what happens during a power transition, when callbacks 
* are implemented and set up correctly. The SysPm Callbacks Example section 
* provides pseudocode that shows how to do this.
*
* The following discussion assumes:
* * All required callback functions are defined and implemented <br>
* * All cy_stc_syspm_callback_t structures are filled with required values <br>
* * All callbacks are successfully registered <br>
*
* When a power transition occurs, the driver uses Cy_SysPm_ExecuteCallback(). 
* There are four possible modes of operation, defined in 
* cy_en_syspm_callback_mode_t. A particular callback function can skip modes. 
* The modes are:
* * CY_SYSPM_CHECK_READY 
* * CY_SYSPM_CHECK_FAIL 
* * CY_SYSPM_BEFORE_TRANSITION 
* * CY_SYSPM_AFTER_TRANSITION 
*
* The SysPm driver calls each registered callback in order, with the same mode. 
* For example, each callback is called and should handle the "ready" event. 
* The driver then moves on to the next mode, and repeats calling all the 
* registered callbacks. 
*
* When a function is called that attempts to switch a device into low 
* power mode, the flow of events is:<br>
* 1. All registered callbacks are called with CY_SYSPM_CHECK_READY argument. 
* Callbacks are executed in order registered (first to last).<br>
* 2. If a callback returns CY_SYSPM_FAIL for the readiness check, then:<br>
* A. The remaining callbacks are not executed.<br>
* B. Any callbacks that have already executed are called again, in reverse 
* order, with CY_SYSPM_CHECK_FAIL. They are called from last to first to 
* allow the system to return to the previous state.<br>
* C. The function that attempted to switch the device into a low power mode
* returns CY_SYSPM_FAIL.<br>
* 3. If all callback function pass the CY_SYSPM_CHECK_READY process, then:<br> 
* A. All callback functions are called with THE CY_SYSPM_BEFORE_TRANSITION 
* argument. Callbacks are executed in order registered (first to last). <br>
* B. When the callbacks are complete the device enters the low power mode. <br>
* C. The device returns from low power mode based on a wakeup event 
* (interrupt, event). (This step is not applicable for the transitions: 
* LPActive to Active and from Active to LPActive which must be called from the
* code.) <br>
* D. After wakeup, all callbacks are called with the CY_SYSPM_AFTER_TRANSITION 
* argument. Callbacks are executed last to first. This step is not applicable 
* for wakeup from the Hibernate power mode, because the device is rebooted. <br>
* E. The function that switched the device to low power mode returns 
* CY_SYSPM_PASS. <br>
*
* If there are no callbacks registered, the device just executes the 
* power mode transition. 
*
* A callback should return CY_SYSPM_FAIL only in response to 
* CY_SYSPM_CHECK_READY. The callback function should always return CY_SYSPM_PASS
* for other modes: 
* CY_SYSPM_CHECK_FAIL, CY_SYSPM_BEFORE_TRANSITION, and 
* CY_SYSPM_AFTER_TRANSITION.
*
* Callbacks that reconfigure global resources, such as clock frequencies, 
* should be registered last.
*
* \subsection group__syspm_callback_example SysPm Callbacks Example
*
* The following pseudo code example demonstrates how use the SysPm callbacks
* mechanism. It also shows a prototype for an application Deep Sleep 
* callback function. This code assumes:<br>
* * This is a dual-core device: CM0+ and CM4 cores <br>
* * The CM0+ core is already in Deep Sleep <br>
* * The code is executed on the CM4 <br>
*
* The main.c file of CM4 core would look like this:
*
* \snippet syspm/syspm_2_0_sut_01.cydsn/main_cm4.c SysPm Callback Example
*
* \section group_syspm_section_more_information More Information
* For more information on the Power Modes (SysPm) driver,
* refer to the Technical Reference Manual (TRM).
*
* \section group_syspm_MISRA MISRA-C Compliance
* The SysPm driver does not have any specific deviations.
*
* \section group_syspm_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
*   <tr>
*     <td>2.0</td>
*     <td>Enhancement and defect fixes: <br> 
*         * Added input parameter(s) validation to all public functions. <br>
*         * Removed "_SysPm_" prefixes from the internal functions names. <br>
*         * Changed the type of elements with limited set of values, from 
*           uint32_t to enumeration.
*         * Enhanced syspm callback mechanism.
*     </td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_syspm_macros Macros
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
#include "ipc/cy_ipc_sema.h"
#include "ipc/cy_ipc_drv.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
*       Register Constants
*******************************************************************************/

/**
* \addtogroup group_syspm_macros
* \{
*/

/** Driver major version */
#define CY_SYSPM_DRV_VERSION_MAJOR       2

/** Driver minor version */
#define CY_SYSPM_DRV_VERSION_MINOR       0

/** \cond INTERNAL */

/** The internal define for clock divider */
#define CY_SYSPM_CLK_DIVIDER                             (9U)

/** The internal define of the unlock value for the PMIC functions */
#define CY_SYSPM_PMIC_UNLOCK_KEY                         (0x3AU)

/** The internal define of the tries number in the Cy_SysPm_ExitLpMode() 
* function
*/
#define CY_SYSPM_WAIT_DELAY_TRYES                        (100U)

/** The internal define of the first wakeup pin bit used in the
* Cy_SysPm_SetHibWakeupSource() function
*/
#define CY_SYSPM_WAKEUP_PIN0_BIT                          (1UL)

/** The internal define of the second wakeup pin bit 
* used in the Cy_SysPm_SetHibWakeupSource() function
*/
#define CY_SYSPM_WAKEUP_PIN1_BIT                          (2UL)

/**
* The internal define of the first LPComparator bit 
* used in the Cy_SysPm_SetHibWakeupSource() function
*/
#define CY_SYSPM_WAKEUP_LPCOMP0_BIT                       (4UL)

/**
* The internal define for the second LPComparator bit 
* used in the Cy_SysPm_SetHibWakeupSource() function
*/
#define CY_SYSPM_WAKEUP_LPCOMP1_BIT                       (8UL)

/**
* The internal define of the first LPComparator value 
* used in the Cy_SysPm_SetHibWakeupSource() function
*/
#define CY_SYSPM_WAKEUP_LPCOMP0    ((uint32_t) CY_SYSPM_WAKEUP_LPCOMP0_BIT << \
                                               SRSS_PWR_HIBERNATE_MASK_HIBPIN_Pos)

/**
* The internal define of the second LPComparator value 
* used in the Cy_SysPm_SetHibWakeupSource() function
*/
#define CY_SYSPM_WAKEUP_LPCOMP1    ((uint32_t) CY_SYSPM_WAKEUP_LPCOMP1_BIT << \
                                               SRSS_PWR_HIBERNATE_MASK_HIBPIN_Pos)

/**
* The internal define of the first wake-up pin value 
* used in the Cy_SysPm_SetHibWakeupSource() function
*/
#define CY_SYSPM_WAKEUP_PIN0       ((uint32_t) CY_SYSPM_WAKEUP_PIN0_BIT << \
                                               SRSS_PWR_HIBERNATE_MASK_HIBPIN_Pos)

/**
* The internal define of the second wake-up pin value used 
* in the Cy_SysPm_SetHibWakeupSource() function
*/
#define CY_SYSPM_WAKEUP_PIN1       ((uint32_t) CY_SYSPM_WAKEUP_PIN1_BIT << \
                                               SRSS_PWR_HIBERNATE_MASK_HIBPIN_Pos)

/** The internal define for the first LPComparator polarity configuration */
#define CY_SYSPM_WAKEUP_LPCOMP0_POLARITY_HIGH    ((uint32_t) CY_SYSPM_WAKEUP_LPCOMP0_BIT << \
                                                             SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Pos)

/** The internal define for the second LPComparator polarity configuration */
#define CY_SYSPM_WAKEUP_LPCOMP1_POLARITY_HIGH    ((uint32_t) CY_SYSPM_WAKEUP_LPCOMP1_BIT << \
                                                             SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Pos)

/** The internal define for the first wake-up pin polarity configuration */
#define CY_SYSPM_WAKEUP_PIN0_POLARITY_HIGH       ((uint32_t) CY_SYSPM_WAKEUP_PIN0_BIT << \
                                                             SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Pos)

/** The internal define for the second wake-up pin polarity configuration */
#define CY_SYSPM_WAKEUP_PIN1_POLARITY_HIGH      ((uint32_t) CY_SYSPM_WAKEUP_PIN1_BIT << \
                                                             SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Pos)

/* Mask for the fast clock divider value */
#define CY_SYSPM_FAST_CLK_DIV_Msk            (0xFF000000UL)

/* Position for the fast clock divider value */
#define CY_SYSPM_FAST_CLK_DIV_Pos            (24UL)

#define CY_SYSPM_IPC_STC                     IPC_STRUCT7

/* Mask for the slow clock divider value */
#define CY_SYSPM_SLOW_CLK_DIV_Msk            (0x00FF0000UL)

/* Position for the slow clock divider value */
#define CY_SYSPM_SLOW_CLK_DIV_Pos            (16UL)

#if(0u != CY_CPU_CORTEX_M4)
    #define CY_SYSPM_CUR_CORE_DP_MASK          (0x01UL)
    #define CY_SYSPM_OTHER_CORE_DP_MASK        (0x02UL)
#else
    #define CY_SYSPM_CUR_CORE_DP_MASK          (0x02UL)
    #define CY_SYSPM_OTHER_CORE_DP_MASK        (0x01UL)
#endif

/** \endcond */

/** syspm driver identifier */
#define CY_SYSPM_ID                             (CY_PDL_DRV_ID(0x10U))

/**
* \defgroup group_syspm_return_status The Power Mode Status Defines
* \{
* The defines of the core(s) and device power mode status.
*/
#if(0u != CY_IP_M4CPUSS)

    /** The CM4 is Active */
    #define CY_SYSPM_STATUS_CM4_ACTIVE       (0x01U)

    /** The CM4 is in Sleep */
    #define CY_SYSPM_STATUS_CM4_SLEEP        (0x02U)

    /** The CM4 is in DeepSleep */
    #define CY_SYSPM_STATUS_CM4_DEEPSLEEP    (0x04U)

    /** The CM4 is Low Power mode */
    #define CY_SYSPM_STATUS_CM4_LOWPOWER     (0x80U)
    
    /** The define of retained power mode of the CM4 */
    #define CY_SYSPM_CM4_PWR_RETAINED        (2UL)
    
#endif /* (0u != CY_IP_M4CPUSS) */

/** The CM0 is Active */
#define CY_SYSPM_STATUS_CM0_ACTIVE       ((uint32_t) ((uint32_t)0x01U << 8U))

/** The CM0 is in Sleep */
#define CY_SYSPM_STATUS_CM0_SLEEP        ((uint32_t) ((uint32_t)0x02U << 8U))

/** The CM0 is in DeepSleep */
#define CY_SYSPM_STATUS_CM0_DEEPSLEEP    ((uint32_t) ((uint32_t)0x04U << 8U))

/** The CM0 is in low power mode */
#define CY_SYSPM_STATUS_CM0_LOWPOWER     ((uint32_t) ((uint32_t)0x80U << 8U))

#if(0u != CY_IP_M4CPUSS)

    /** The device is in the low power mode define */
    #define CY_SYSPM_STATUS_SYSTEM_LOWPOWER    ((uint32_t) (CY_SYSPM_STATUS_CM0_LOWPOWER | \
                                                           (CY_SYSPM_STATUS_CM4_LOWPOWER)))
#else
    /** The device is in the low power mode define */
    #define CY_SYSPM_STATUS_SYSTEM_LOWPOWER    CY_SYSPM_STATUS_CM0_LOWPOWER

#endif /* (0u != CY_IP_M4CPUSS) */
/** \} group_syspm_return_status */

/** The define to update the token to indicate the transition into Hibernate */
#define CY_SYSPM_PWR_TOKEN_HIBERNATE      ((uint32_t) 0x1BU << SRSS_PWR_HIBERNATE_TOKEN_Pos)

/** The wait time for transition of the device from the Active into 
* the LPActive
*/
#define CY_SYSPM_ACTIVE_TO_LP_WAIT_US           (1U)

/** The wait delay time which occurs before the Active reference is settled.
* This delay is used in transition of the device from Active into the LPActive 
* low-power mode 
*/
#define CY_SYSPM_LP_TO_ACTIVE_WAIT_BEFORE_US    (8U)

/** The wait delay time which occurs after the Active reference is settled. 
* This delay is used in transition the device from Active into the LPActive 
* low-power mode 
*/
#define CY_SYSPM_LP_TO_ACTIVE_WAIT_AFTER_US    (1U)

/** The maximum callbacks number */
#define CY_SYSPM_CALLBACKS_NUMBER_MAX         (32U)

/** The mask to unlock the Hibernate power mode */
#define CY_SYSPM_PWR_HIBERNATE_UNLOCK             ((uint32_t) 0x3Au << SRSS_PWR_HIBERNATE_UNLOCK_Pos)

/** The mask to set the Hibernate power mode */
#define CY_SYSPM_PWR_SET_HIBERNATE                 ((uint32_t) CY_SYSPM_PWR_HIBERNATE_UNLOCK |\
                                                                   SRSS_PWR_HIBERNATE_FREEZE_Msk |\
                                                                   SRSS_PWR_HIBERNATE_HIBERNATE_Msk)

/** The mask to retain the Hibernate power mode status */
#define CY_SYSPM_PWR_RETAIN_HIBERNATE_STATUS      ((uint32_t) SRSS_PWR_HIBERNATE_TOKEN_Msk |\
                                                              SRSS_PWR_HIBERNATE_MASK_HIBALARM_Msk |\
                                                              SRSS_PWR_HIBERNATE_MASK_HIBWDT_Msk |\
                                                              SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Msk |\
                                                              SRSS_PWR_HIBERNATE_MASK_HIBPIN_Msk)

/** The mask for the wakeup sources */
#define CY_SYSPM_PWR_WAKEUP_HIB_MASK              ((uint32_t) SRSS_PWR_HIBERNATE_MASK_HIBALARM_Msk |\
                                                              SRSS_PWR_HIBERNATE_MASK_HIBWDT_Msk |\
                                                              SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Msk |\
                                                              SRSS_PWR_HIBERNATE_MASK_HIBPIN_Msk)

/**
* \defgroup group_syspm_skip_callback_modes The defines to skip the callbacks modes
* \{
* The defines of the syspm callbacks modes that can be skipped during execution.
* For more information about callbacks modes refer 
* to \ref cy_en_syspm_callback_mode_t.
*/
#define CY_SYSPM_SKIP_CHECK_READY                   (0x01U)   /**< The define to skip the check ready mode in the syspm callback */
#define CY_SYSPM_SKIP_CHECK_FAIL                    (0x02U)   /**< The define to skip the check fail mode in the syspm callback */
#define CY_SYSPM_SKIP_BEFORE_TRANSITION             (0x04U)   /**< The define to skip the before transition mode in the syspm callback */
#define CY_SYSPM_SKIP_AFTER_TRANSITION              (0x08U)   /**< The define to skip the after transition mode in the syspm callback */
/** \} group_syspm_skip_callback_modes */

#if(0u != SRSS_SIMOBUCK_PRESENT)

    /** The definition for the delay of the SIMO Buck supply regulator 
    * stabilization after it was configured with enabled SIMO Buck output 1 */
    #define CY_SYSPM_SIMO_BUCK_CORE_SUPPLY_STABLE_US       (900U)

    /** The definition for the delay of the SIMO Buck supply regulator 
    * stabilization after it was configured with enabled SIMO Buck 
    * output 2 only
    */
    #define CY_SYSPM_SIMO_BUCK_BLE_SUPPLY_STABLE_US        (600U)

    /** The definition for the delay of the Linear Regulator after its output 
    * voltage is changed
    */
    #define CY_SYSPM_LDO_IS_STABLE_US                      (9U)

    /** The definition for the delay of the SIMO Buck regulator after its output
    * voltage is changed
    */
    #define CY_SYSPM_SIMO_BUCK_IS_STABLE_US                (200U)

#endif /* (0u != SRSS_SIMOBUCK_PRESENT) */

/** \cond INTERNAL */

/* Internal macro of all possible wakeup sources from hibernate power mode */
#define CY_SYSPM_HIB_WAKEUP_SOURSE_MASK        (CY_SYSPM_LPCOMP0_HIGH | CY_SYSPM_LPCOMP1_HIGH | \
                                                CY_SYSPM_HIBALARM     | CY_SYSPM_HIBWDT | \
                                                CY_SYSPM_HIBPIN0_HIGH | CY_SYSPM_HIBPIN1_HIGH)

/* Macro to validate parameters in Cy_SysPm_SetHibWakeupSource() and for Cy_SysPm_ClearHibWakeupSource() function */
#define CY_SYSPM_IS_WAKE_UP_SOURCE_VALID(wakeupSource)   (0UL == ((wakeupSource) & \
                                                         ((uint32_t) ~(CY_SYSPM_HIB_WAKEUP_SOURSE_MASK))))

/* Macro to validate parameters in Cy_SysPm_DisablePmic() function */
#define CY_SYSPM_IS_POLARITY_VALID(polarity)            (((polarity) == CY_SYSPM_PMIC_POLARITY_LOW) || \
                                                         ((polarity) == CY_SYSPM_PMIC_POLARITY_HIGH))

/* Macro to validate parameters in Cy_SysPm_SimoBuckSetVoltage1() function */
#define CY_SYSPM_IS_SIMO_BUCK_VOLTAGE1_VALID(voltage)   (((voltage) == CY_SYSPM_SIMO_BUCK_OUT1_VOLTAGE_0_9V) || \
                                                         ((voltage) == CY_SYSPM_SIMO_BUCK_OUT1_VOLTAGE_1_1V))

/* Macro to validate parameters in Cy_SysPm_SimoBuckSetVoltage2() function */
#define CY_SYSPM_IS_SIMO_BUCK_VOLTAGE2_VALID(voltage)   (((voltage) == CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_15V) || \
                                                         ((voltage) == CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_2V)  || \
                                                         ((voltage) == CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_25V) || \
                                                         ((voltage) == CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_3V)  || \
                                                         ((voltage) == CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_35V) || \
                                                         ((voltage) == CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_4V)  || \
                                                         ((voltage) == CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_45V) || \
                                                         ((voltage) == CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_5V))

/* Macro to validate parameters in Cy_SysPm_SimoBuckOutputIsEnabled() function */
#define CY_SYSPM_IS_SIMO_BUCK_OUTPUT_VALID(output)      (((output) == CY_SYSPM_BUCK_VBUCK_1) || \
                                                         ((output) == CY_SYSPM_BUCK_VRF))

/* Macro to validate parameters in Cy_SysPm_LdoSetVoltage() function */
#define CY_SYSPM_IS_LDO_VOLTAGE_VALID(voltage)          (((voltage) == CY_SYSPM_LDO_VOLTAGE_0_9V) || \
                                                         ((voltage) == CY_SYSPM_LDO_VOLTAGE_1_1V))

/* Macro to validate parameters in Cy_SysPm_ExecuteCallback() function */
#define CY_SYSPM_IS_CALLBACK_TYPE_VALID(type)           (((type) == CY_SYSPM_SLEEP) || \
                                                         ((type) == CY_SYSPM_DEEPSLEEP) || \
                                                         ((type) == CY_SYSPM_HIBERNATE) || \
                                                         ((type) == CY_SYSPM_ENTER_LP_MODE) || \
                                                         ((type) == CY_SYSPM_EXIT_LP_MODE))

/* Macro to validate parameters in Cy_SysPm_ExecuteCallback() function */
#define CY_SYSPM_IS_CALLBACK_MODE_VALID(mode)           (((mode) == CY_SYSPM_CHECK_READY) || \
                                                         ((mode) == CY_SYSPM_CHECK_FAIL) || \
                                                         ((mode) == CY_SYSPM_BEFORE_TRANSITION) || \
                                                         ((mode) == CY_SYSPM_AFTER_TRANSITION))

/* Macro to validate parameters in Cy_SysPm_Sleep() and for Cy_SysPm_DeepSleep() function */
#define CY_SYSPM_IS_WAIT_FOR_VALID(waitFor)             (((waitFor) == CY_SYSPM_WAIT_FOR_INTERRUPT) || \
                                                         ((waitFor) == CY_SYSPM_WAIT_FOR_EVENT))

/** \endcond */

/** \} group_syspm_macros */

/*******************************************************************************
*       Configuration Structures
*******************************************************************************/

/**
* \addtogroup group_syspm_data_enumerates
* \{
*/

/** This enumeration is used to configure sources for wakeup from Hibernate 
*   power mode
*/
typedef enum
{
    /** Configure a low level for the first LPComp */
    CY_SYSPM_LPCOMP0_LOW = 
    ((uint32_t) CY_SYSPM_WAKEUP_LPCOMP0_BIT << SRSS_PWR_HIBERNATE_MASK_HIBPIN_Pos),

    /** Configure a high level for the first LPComp */
    CY_SYSPM_LPCOMP0_HIGH = 
    ((uint32_t) ((uint32_t) CY_SYSPM_WAKEUP_LPCOMP0_BIT << SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Pos) | 
                ((uint32_t) CY_SYSPM_WAKEUP_LPCOMP0_BIT << SRSS_PWR_HIBERNATE_MASK_HIBPIN_Pos)),

     /** Configure a low level for the second LPComp */
    CY_SYSPM_LPCOMP1_LOW = ((uint32_t) CY_SYSPM_WAKEUP_LPCOMP1_BIT << SRSS_PWR_HIBERNATE_MASK_HIBPIN_Pos),
    
    /** Configure a high level for the second LPComp */
    CY_SYSPM_LPCOMP1_HIGH =
    ((uint32_t) ((uint32_t) CY_SYSPM_WAKEUP_LPCOMP1_BIT << SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Pos) | 
                ((uint32_t) CY_SYSPM_WAKEUP_LPCOMP1_BIT << SRSS_PWR_HIBERNATE_MASK_HIBPIN_Pos)),

    /** Configure the RTC alarm */
    CY_SYSPM_HIBALARM = SRSS_PWR_HIBERNATE_MASK_HIBALARM_Msk,
    
    /** Configure the WDT interrupt */
    CY_SYSPM_HIBWDT = SRSS_PWR_HIBERNATE_MASK_HIBWDT_Msk,

    /** Configure a low level for the first wakeup-pin */
    CY_SYSPM_HIBPIN0_LOW = ((uint32_t) CY_SYSPM_WAKEUP_PIN0_BIT << SRSS_PWR_HIBERNATE_MASK_HIBPIN_Pos),
    
    /** Configure a high level for the first wakeup-pin */
    CY_SYSPM_HIBPIN0_HIGH =
    ((uint32_t) ((uint32_t) CY_SYSPM_WAKEUP_PIN0_BIT << SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Pos) | 
                ((uint32_t) CY_SYSPM_WAKEUP_PIN0_BIT << SRSS_PWR_HIBERNATE_MASK_HIBPIN_Pos)),

    /** Configure a low level for the second wakeup-pin */
    CY_SYSPM_HIBPIN1_LOW = ((uint32_t) CY_SYSPM_WAKEUP_PIN1_BIT << SRSS_PWR_HIBERNATE_MASK_HIBPIN_Pos),
    
   /** Configure a high level for the second wakeup-pin */
    CY_SYSPM_HIBPIN1_HIGH =
    ((uint32_t) ((uint32_t) CY_SYSPM_WAKEUP_PIN1_BIT << SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Pos) | 
                ((uint32_t) CY_SYSPM_WAKEUP_PIN1_BIT << SRSS_PWR_HIBERNATE_MASK_HIBPIN_Pos)),
} cy_en_syspm_hib_wakeup_source_t;

/**
* This enumeration is used to initialize a wait action - an interrupt or
* an event. Refer to the CMSIS for the WFE and WFI instruction explanations.
*/
typedef enum
{
    CY_SYSPM_WAIT_FOR_INTERRUPT,      /**< Wait for an interrupt */
    CY_SYSPM_WAIT_FOR_EVENT           /**< Wait for an event */
} cy_en_syspm_waitfor_t;

/** The enumeration is used to select output voltage for the Linear Regulator */
typedef enum
{
    CY_SYSPM_LDO_VOLTAGE_0_9V = 0x07U,      /**< 0.9 V nominal core voltage */
    CY_SYSPM_LDO_VOLTAGE_1_1V = 0x17U       /**< 1.1 V nominal core voltage */
} cy_en_syspm_ldo_voltage_t;

#if(0u != SRSS_SIMOBUCK_PRESENT)

    /**
    *  The enumeration is used to select the output voltage for the SIMO Buck 
    *  output 1, which can supply a core(s).
    */
    typedef enum
    {
        CY_SYSPM_SIMO_BUCK_OUT1_VOLTAGE_0_9V = 0x02U,    /**< 0.9 V nominal core voltage */
        CY_SYSPM_SIMO_BUCK_OUT1_VOLTAGE_1_1V = 0x05U     /**< 1.1 V nominal core voltage */
    } cy_en_syspm_simo_buck_voltage1_t;

    /**
    * The enumeration is used to select the output voltage for the SIMO Buck 
    * output 2, which can BLE HW block.
    */
    typedef enum
    {
        CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_15V = 0U,    /**< 1.15 V nominal voltage */
        CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_2V  = 1U,    /**< 1.20 V nominal voltage */
        CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_25V = 2U,    /**< 1.25 V nominal voltage */
        CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_3V  = 3U,    /**< 1.3 V nominal voltage  */
        CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_35V = 4U,    /**< 1.35 V nominal voltage */
        CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_4V  = 5U,    /**< 1.4 V nominal voltage */
        CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_45V = 6U,    /**< 1.45 V nominal voltage */
        CY_SYSPM_SIMO_BUCK_OUT2_VOLTAGE_1_5V  = 7U     /**< 1.5 V nominal voltage */
    } cy_en_syspm_simo_buck_voltage2_t;

    /**
    * The enumerations are used to select the SIMO Buck regulator outputs
    */
    typedef enum
    {
        CY_SYSPM_BUCK_VBUCK_1,    /**< SIMO Buck output 1 Voltage (Vbuck1) */
        CY_SYSPM_BUCK_VRF         /**< SIMO Buck out 2 Voltage (Vbuckrf) */
    } cy_en_syspm_buck_out_t;

#endif /* (0u != SRSS_SIMOBUCK_PRESENT) */

/** The SysPm status definitions */
typedef enum
{
    CY_SYSPM_SUCCESS       = 0x00U,                                        /**< Successful */
    CY_SYSPM_BAD_PARAM     = CY_SYSPM_ID | CY_PDL_STATUS_ERROR | 0x01U,    /**< One or more invalid parameters */
    CY_SYSPM_TIMEOUT       = CY_SYSPM_ID | CY_PDL_STATUS_ERROR | 0x02U,    /**< A time-out occurs */
    CY_SYSPM_INVALID_STATE = CY_SYSPM_ID | CY_PDL_STATUS_ERROR | 0x03U,    /**< The operation is not setup or is in an
                                                                                improper state */
    CY_SYSPM_FAIL          = CY_SYSPM_ID | CY_PDL_STATUS_ERROR | 0xFFU     /**< An unknown failure */
} cy_en_syspm_status_t;

/**
* This enumeration is used for selecting the low power mode on which the
* appropriate registered callback handler will be executed. For example,
* the registered callback of the type CY_SYSPM_SLEEP will be executed while
* switching into the Sleep power mode.
*/
typedef enum
{
    CY_SYSPM_SLEEP,          /**< The Sleep enum callback type */
    CY_SYSPM_DEEPSLEEP,      /**< The DeepSleep enum callback type */
    CY_SYSPM_HIBERNATE,      /**< The Hibernate enum callback type */
    CY_SYSPM_ENTER_LP_MODE,  /**< The enter into the LPActive mode enum callback type */
    CY_SYSPM_EXIT_LP_MODE,   /**< The exit out of the LPActive mode enum callback type */
} cy_en_syspm_callback_type_t;

/** The callback mode enumeration. This enum defines the callback mode */
typedef enum
{
    CY_SYSPM_CHECK_READY        = 0x01U,    /**< Callbacks with this mode are executed before entering into the 
                                                 low-power mode. Callback function check if the device is ready
                                                 to enter the low-power mode */
    CY_SYSPM_CHECK_FAIL         = 0x02U,    /**< Callbacks with this mode are executed after the previous callbacks
                                                 execution with CY_SYSPM_CHECK_READY return CY_SYSPM_FAIL. The callback
                                                 with the CY_SYSPM_CHECK_FAIL mode should roll back the actions done in
                                                 the callbacks executed previously with CY_SYSPM_CHECK_READY */
    CY_SYSPM_BEFORE_TRANSITION  = 0x04U,    /**< The actions to be done before entering into the low-power mode */
    CY_SYSPM_AFTER_TRANSITION   = 0x08U,    /**< The actions to be done after exiting the low-power mode */
} cy_en_syspm_callback_mode_t;

/**
* This enumeration is used to set a polarity for the PMIC input. The PMIC is 
* automatically enabled when configured polarity of PMIC input and the polarity
* driven to pmic_wakeup_in pin matches.
*/
typedef enum
{
    CY_SYSPM_PMIC_POLARITY_LOW   = 0U,    /**< Set active low state for the PMIC input */
    CY_SYSPM_PMIC_POLARITY_HIGH  = 1U     /**< Set active high state for the PMIC input */
} cy_en_syspm_pmic_wakeup_polarity_t;

/** \} group_syspm_data_enumerates */

/**
* \addtogroup group_syspm_data_structures
* \{
*/

/** The structure with the syspm callback parameters */
typedef struct
{
    cy_en_syspm_callback_mode_t mode;   /**< The callback mode, element for internal usage, see 
                                             \ref cy_en_syspm_callback_mode_t. This element should not be defined 
                                             as it is updated every time before the callback function is executed */
    void *base;                         /**< The base address of a HW instance, matches name of the driver in
                                             the API for the base address. Can be not defined if not required */
    void *context;                      /**< The context for the handler function. This item can be 
                                             skipped if not required. Can be not defined if not required */

} cy_stc_syspm_callback_params_t;

/** The type for syspm callbacks */
typedef cy_en_syspm_status_t (*Cy_SysPmCallback) (cy_stc_syspm_callback_params_t *callbackParams);

/** The structure with the syspm callback configuration elements */
typedef struct cy_stc_syspm_callback
{
    Cy_SysPmCallback callback;                         /**< The callback handler function */
    cy_en_syspm_callback_type_t type;                  /**< The callback type, see \ref cy_en_syspm_callback_type_t */
    uint32_t skipMode;                                 /**< The mask of modes to be skipped during callback 
                                                             execution, see \ref group_syspm_skip_callback_modes. The
                                                             corresponding callback mode won't execute if the 
                                                             appropriate define is set. These values can be ORed.
                                                             If all modes are required to be executed this element
                                                             should be equal to zero. */

    cy_stc_syspm_callback_params_t *callbackParams;    /**< The base address of a HW instance, matches name of the 
                                                            driver in the API for the base address */

    struct cy_stc_syspm_callback *prevItm;              /**< The previous list item. This element is for internal usage
                                                             to link this structure to the next registered structure. 
                                                             It will be updated during callback registering. Do not 
                                                             modify this element in run-time */
                                                             
    struct cy_stc_syspm_callback *nextItm;              /**< The next list item. This element is for internal usage to 
                                                             link this structure to the previous registered structure.
                                                             It will be updated during callback registering. Do not 
                                                             modify this element in run-time */
} cy_stc_syspm_callback_t;

#ifdef CY_IP_MXUDB

    /** \cond INTERNAL */
    #define CY_SYSPM_PERI_GR3_UDB       (3U)
    
    /** This internal structure stores non-retained registers in the Deep 
    *  Sleep power mode.
    */
    typedef struct
    {
        /* The UDB interface control register */
        uint32_t CY_UDB_UDBIF_BANK_CTL_REG;

        /* The UDB bank control registers */
        uint32_t CY_UDB_BCTL_MDCLK_EN_REG;
        uint32_t CY_UDB_BCTL_MBCLK_EN_REG;
        uint32_t CY_UDB_BCTL_BOTSEL_L_REG;
        uint32_t CY_UDB_BCTL_BOTSEL_U_REG;
        uint32_t CY_UDB_BCTL_QCLK_EN0_REG;
        uint32_t CY_UDB_BCTL_QCLK_EN1_REG;
        uint32_t CY_UDB_BCTL_QCLK_EN2_REG;
    } cy_stc_syspm_backup_regs_t;
    /** \endcond */

#endif /* CY_IP_MXUDB */

/** \} group_syspm_data_structures */

/**
* \addtogroup group_syspm_functions
* \{
*/

#if(0u != CY_IP_M4CPUSS)

    __STATIC_INLINE bool Cy_SysPm_Cm4IsActive(void);
    __STATIC_INLINE bool Cy_SysPm_Cm4IsSleep(void);
    __STATIC_INLINE bool Cy_SysPm_Cm4IsDeepSleep(void);
    __STATIC_INLINE bool Cy_SysPm_Cm4IsLowPower(void);

#endif /* (0u != CY_IP_M4CPUSS) */

__STATIC_INLINE bool Cy_SysPm_Cm0IsActive(void);
__STATIC_INLINE bool Cy_SysPm_Cm0IsSleep(void);
__STATIC_INLINE bool Cy_SysPm_Cm0IsDeepSleep(void);
__STATIC_INLINE bool Cy_SysPm_Cm0IsLowPower(void);
__STATIC_INLINE bool Cy_SysPm_IsLowPower(void);

__STATIC_INLINE cy_en_syspm_ldo_voltage_t Cy_SysPm_LdoGetVoltage(void);
__STATIC_INLINE bool Cy_SysPm_LdoIsEnabled(void);
__STATIC_INLINE bool Cy_SysPm_GetIoFreezeStatus(void);

__STATIC_INLINE void Cy_SysPm_EnablePmic(void);
__STATIC_INLINE void Cy_SysPm_DisablePmic(cy_en_syspm_pmic_wakeup_polarity_t polarity); 
__STATIC_INLINE void Cy_SysPm_AlwaysEnablePmic(void);
__STATIC_INLINE void Cy_SysPm_EnablePmicOutput(void);
__STATIC_INLINE void Cy_SysPm_DisablePmicOutput(void); 
__STATIC_INLINE void Cy_SysPm_LockPmic(void);
__STATIC_INLINE void Cy_SysPm_UnlockPmic(void);
__STATIC_INLINE bool Cy_SysPm_IsPmicEnabled(void);
__STATIC_INLINE bool Cy_SysPm_IsPmicOutputEnabled(void);
__STATIC_INLINE bool Cy_SysPm_IsPmicLocked(void);

#if(0u != SRSS_SIMOBUCK_PRESENT)

    __STATIC_INLINE cy_en_syspm_simo_buck_voltage1_t Cy_SysPm_SimoBuckGetVoltage1(void);
    __STATIC_INLINE cy_en_syspm_simo_buck_voltage2_t Cy_SysPm_SimoBuckGetVoltage2(void);
    __STATIC_INLINE void Cy_SysPm_DisableVoltage2(void);
    __STATIC_INLINE bool Cy_SysPm_SimoBuckIsEnabled(void);
    __STATIC_INLINE void Cy_SysPm_SimoBuckSetHwControl(bool hwControl);
    __STATIC_INLINE bool Cy_SysPm_SimoBuckGetHwControl(void);

    void Cy_SysPm_SwitchToSimoBuck(void);
    void Cy_SysPm_SimoBuckSetVoltage1(cy_en_syspm_simo_buck_voltage1_t voltage);
    void Cy_SysPm_EnableVoltage2(void);
    void Cy_SysPm_SimoBuckSetVoltage2(cy_en_syspm_simo_buck_voltage2_t voltage, bool waitToSettle);
    bool Cy_SysPm_SimoBuckOutputIsEnabled(cy_en_syspm_buck_out_t output);

#endif /* (0u != SRSS_SIMOBUCK_PRESENT) */

uint32_t Cy_SysPm_ReadStatus(void);
cy_en_syspm_status_t Cy_SysPm_Sleep(cy_en_syspm_waitfor_t waitFor);
cy_en_syspm_status_t Cy_SysPm_DeepSleep(cy_en_syspm_waitfor_t waitFor);
cy_en_syspm_status_t Cy_SysPm_Hibernate(void);
cy_en_syspm_status_t Cy_SysPm_EnterLpMode(void);
cy_en_syspm_status_t Cy_SysPm_ExitLpMode(void);
void Cy_SysPm_SleepOnExit(bool enable);

void Cy_SysPm_SetHibWakeupSource(cy_en_syspm_hib_wakeup_source_t wakeupSource);
void Cy_SysPm_ClearHibWakeupSource(cy_en_syspm_hib_wakeup_source_t wakeupSource);
void Cy_SysPm_LdoSetVoltage(cy_en_syspm_ldo_voltage_t voltage);
bool Cy_SysPm_RegisterCallback(cy_stc_syspm_callback_t *handler);
bool Cy_SysPm_UnregisterCallback(cy_stc_syspm_callback_t const *handler);
cy_en_syspm_status_t Cy_SysPm_ExecuteCallback(cy_en_syspm_callback_type_t type, cy_en_syspm_callback_mode_t mode);
void Cy_SysPm_IoFreeze(void);
void Cy_SysPm_IoUnfreeze(void);

#if(0u != CY_IP_M4CPUSS)
    /*******************************************************************************
    * Function Name: Cy_SysPm_Cm4IsActive
    ****************************************************************************//**
    *
    * Checks if CM4 is in the active mode.
    *
    * \return
    * true - if CM4 is in the active mode, false - if the CM4 is not in active mode.
    *
    *******************************************************************************/
    __STATIC_INLINE bool Cy_SysPm_Cm4IsActive(void)
    {
        return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM4_ACTIVE) != 0U);
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_Cm4IsSleep
    ****************************************************************************//**
    *
    * Checks if the CM4 is in the sleep mode.
    *
    * \return
    * true - if the CM4 is in the sleep mode, 
    * false - if the CM4 is not in the sleep mode.
    *
    *******************************************************************************/
    __STATIC_INLINE bool Cy_SysPm_Cm4IsSleep(void)
    {
        return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM4_SLEEP) != 0U);
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_Cm4IsDeepSleep
    ****************************************************************************//**
    *
    * Checks if the CM4 is in the deep sleep mode.
    *
    * \return
    * true - if CM4 is in the deep sleep mode, false - if the CM4 is not in 
    * the deep sleep mode.
    *
    *******************************************************************************/
    __STATIC_INLINE bool Cy_SysPm_Cm4IsDeepSleep(void)
    {
        return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM4_DEEPSLEEP) != 0U);
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_Cm4IsLowPower
    ****************************************************************************//**
    *
    * Checks if the CM4 is in the low power mode.
    *
    * \return
    * true - if the CM4 is in the low power mode, 
    * false - if the CM4 is not in the low power mode.
    *
    *******************************************************************************/
    __STATIC_INLINE bool Cy_SysPm_Cm4IsLowPower(void)
    {
        return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM4_LOWPOWER) != 0U);
    }

#endif /* (0u != CY_IP_M4CPUSS) */


/*******************************************************************************
* Function Name: Cy_SysPm_Cm0IsActive
****************************************************************************//**
*
* Checks if the CM0+ is in the active mode.
*
* \return
* true - if the CM0+ is in the active mode, 
* false - if the CM0+ is not in the active mode.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_Cm0IsActive(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM0_ACTIVE) != 0U);
}


/*******************************************************************************
* Function Name: Cy_SysPm_Cm0IsSleep
****************************************************************************//**
*
* Checks if the CM0+ is in the sleep mode.
*
* \return
* true - if the CM0+ is in the sleep mode, 
* false - if the CM0+ is not in the sleep mode.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_Cm0IsSleep(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM0_SLEEP) != 0U);
}


/*******************************************************************************
* Function Name: Cy_SysPm_Cm0IsDeepSleep
****************************************************************************//**
*
* Checks if the CM0+ is in the deep sleep mode.
*
* \return
* true - if the CM0+ is in the deep sleep mode, 
* false - if the CM0+ is not in the deep sleep mode.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_Cm0IsDeepSleep(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM0_DEEPSLEEP) != 0U);
}


/*******************************************************************************
* Function Name: Cy_SysPm_Cm0IsLowPower
****************************************************************************//**
*
* Checks if the CM0+ is in the low power mode.
*
* \return
* true - if the CM0+ is in the low power mode, 
* false - if the CM0+ is not in the low power mode.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_Cm0IsLowPower(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM0_LOWPOWER) != 0U);
}


/*******************************************************************************
* Function Name: Cy_SysPm_IsLowPower
****************************************************************************//**
*
* Checks if the device is in the low power mode.
*
* \return
* true - the system is in the low power mode,
* false - the system is is not in the low power mode.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_IsLowPower(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_SYSTEM_LOWPOWER) != 0U);
}

#if(0u != SRSS_SIMOBUCK_PRESENT)
    /*******************************************************************************
    * Function Name: Cy_SysPm_SimoBuckIsEnabled
    ****************************************************************************//**
    *
    * Get the current status of the SIMO Buck regulator.
    *
    * \return
    * The current state of the SIMO Buck regulator. True if the SIMO Buck regulator
    * is enabled, false if it is disabled
    *
    *******************************************************************************/
    __STATIC_INLINE bool Cy_SysPm_SimoBuckIsEnabled(void)
    {
        return((0u !=_FLD2VAL(SRSS_PWR_BUCK_CTL_BUCK_EN, SRSS->PWR_BUCK_CTL)) ? true : false);
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_SimoBuckGetVoltage2
    ****************************************************************************//**
    *
    * Get the current output 2 nominal voltage (Vbuckrf) of the SIMO Buck regulator.
    *
    * \note The actual device output 2 voltage (Vbuckrf) can be different from the 
    * nominal voltage because the actual voltage value depends on the conditions 
    * including the load current.
    *
    * \return
    * The nominal output voltage of the SIMO Buck regulator output 2 
    * voltage (Vbuckrf).
    * See \ref cy_en_syspm_simo_buck_voltage2_t.
    *
    *******************************************************************************/
    __STATIC_INLINE cy_en_syspm_simo_buck_voltage2_t Cy_SysPm_SimoBuckGetVoltage2(void)
    {
        uint32_t retVal;
        retVal = _FLD2VAL(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_SEL, SRSS->PWR_BUCK_CTL2);

        return((cy_en_syspm_simo_buck_voltage2_t) retVal);
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_SimoBuckGetVoltage1
    ****************************************************************************//**
    *
    * Get the current nominal output 1 voltage (Vccbuck1) of 
    * the SIMO Buck regulator.
    *
    * \note The actual device output 1 voltage (Vccbuck1) can be different from
    * the actual voltage because the actual voltage value depends on the 
    * conditions including the load current.
    *
    * \note The actual device output 1 voltage (Vccbuck1) can be different from the 
    * nominal voltage because the actual voltage value depends on the conditions
    * including the load current.
    *
    * \return
    * The nominal output voltage 1 (Vccbuck1) of the SIMO Buck regulator.
    * See \ref cy_en_syspm_simo_buck_voltage1_t.
    *
    *******************************************************************************/
    __STATIC_INLINE cy_en_syspm_simo_buck_voltage1_t Cy_SysPm_SimoBuckGetVoltage1(void)
    {
        uint32_t retVal;
        retVal = _FLD2VAL(SRSS_PWR_BUCK_CTL_BUCK_OUT1_SEL, SRSS->PWR_BUCK_CTL);

        return((cy_en_syspm_simo_buck_voltage1_t) retVal);
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_DisableVoltage2
    ****************************************************************************//**
    *
    * Disable the output 2 voltage (Vbuckrf) of the SIMO Buck regulator. The 
    * output 2 voltage (Vbuckrf) of the SIMO Buck regulator is used to supply 
    * the BLE HW block.
    *
    * \note The function does not have effect if the SIMO Buck regulator is
    * switched off.
    *
    * \note Ensure that the new voltage supply for the BLE HW block is settled 
    * and it is stable before calling the Cy_SysPm_DisableVoltage2() function.
    *
    *******************************************************************************/
    __STATIC_INLINE void Cy_SysPm_DisableVoltage2(void)
    {
        /* Disable the Vbuck2 output */
        SRSS->PWR_BUCK_CTL2 &= ((uint32_t) ~ (_VAL2FLD(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_EN, 1U)));
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_SimoBuckSetHwControl
    ****************************************************************************//**
    *
    * Sets the hardware control for the SIMO Buck output 2 (Vbuckrf).
    *
    * The hardware control for the Vbuckrf output. When this bit is set, the 
    * value in BUCK_OUT2_EN is ignored and hardware signal is used instead. If the 
    * product has supporting hardware, it can directly control the enable signal
    * for Vbuckrf.
    *
    * \param hwControl
    * Enables/disables hardware control for the SIMO Buck output 2
    *
    * Function does not have an effect if SIMO Buck regulator is disabled.
    *
    *******************************************************************************/
    __STATIC_INLINE void Cy_SysPm_SimoBuckSetHwControl(bool hwControl)
    {
        if(Cy_SysPm_SimoBuckIsEnabled())
        {
            if(hwControl)
            {
                SRSS->PWR_BUCK_CTL2 |= _VAL2FLD(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_HW_SEL, 1U);
            }
            else
            {
                SRSS->PWR_BUCK_CTL2 &= ((uint32_t)~(_VAL2FLD(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_HW_SEL, 1U)));
            }
        }
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_SimoBuckGetHwControl
    ****************************************************************************//**
    *
    * Gets the hardware control for Vbuckrf.
    *
    * The hardware control for the Vbuckrf output. When this bit is set, the 
    * value in BUCK_OUT2_EN is ignored and hardware signal is used instead. If the 
    * product has supporting hardware, it can directly control the enable signal
    * for Vbuckrf.
    *
    * \return
    * True if the HW control is set, false if the FW control is set for the 
    * SIMO Buck output 2.
    *
    *******************************************************************************/
    __STATIC_INLINE bool Cy_SysPm_SimoBuckGetHwControl(void)
    {
        return((0U != _FLD2VAL(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_HW_SEL, SRSS->PWR_BUCK_CTL2)) ? true : false);
    }

#endif /* (0u != SRSS_SIMOBUCK_PRESENT) */

/*******************************************************************************
* Function Name: Cy_SysPm_LdoGetVoltage
****************************************************************************//**
*
* Get the current output voltage value of the Linear regulator.
*
* \note The actual device Vccd voltage can be different from the 
* nominal voltage because the actual voltage value depends on the conditions 
* including the load current.
*
* \return
* The nominal output voltage of the Linear regulator.
* See \ref cy_en_syspm_ldo_voltage_t.
*
*******************************************************************************/
__STATIC_INLINE cy_en_syspm_ldo_voltage_t Cy_SysPm_LdoGetVoltage(void)
{
    uint32_t retVal;
    retVal = _FLD2VAL(SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, SRSS->PWR_TRIM_PWRSYS_CTL);
    
    return((cy_en_syspm_ldo_voltage_t) retVal);
}


/*******************************************************************************
* Function Name: Cy_SysPm_LdoIsEnabled
****************************************************************************//**
*
* Reads the current status of the Linear regulator.
*
* \return
* True means the Linear regulator is enabled. False means it is disabled
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_LdoIsEnabled(void)
{
    return((0U != _FLD2VAL(SRSS_PWR_CTL_LINREG_DIS, SRSS->PWR_CTL)) ? false : true);
}


/*******************************************************************************
* Function Name: Cy_SysPm_GetIoFreezeStatus
****************************************************************************//**
*
* Checks whether IoFreeze is enabled.
*
* \return Returns True if IOs are frozen <br> False if IOs are unfrozen.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_GetIoFreezeStatus(void)
{
    return(0U != _FLD2VAL(SRSS_PWR_HIBERNATE_FREEZE, SRSS->PWR_HIBERNATE));
}


/*******************************************************************************
* Function Name: Cy_SysPm_EnablePmic
****************************************************************************//**
*
* Enable the external PMIC that supplies Vddd (if present).
*
* For information about the PMIC input and output pins and their assignment in 
* the specific families devices, refer to the appropriate device TRM.
* 
* The function is not effective when the PMIC is locked. Call 
* Cy_SysPm_UnlockPmic() before enabling the PMIC.
* 
*******************************************************************************/
__STATIC_INLINE void Cy_SysPm_EnablePmic(void)
{
    if(CY_SYSPM_PMIC_UNLOCK_KEY == _FLD2VAL(BACKUP_PMIC_CTL_UNLOCK, BACKUP->PMIC_CTL))
    {
        BACKUP->PMIC_CTL =
        _VAL2FLD(BACKUP_PMIC_CTL_UNLOCK, CY_SYSPM_PMIC_UNLOCK_KEY) |
        _VAL2FLD(BACKUP_PMIC_CTL_PMIC_EN_OUTEN, 1U) |
        _VAL2FLD(BACKUP_PMIC_CTL_PMIC_EN, 1U);
    }
}


/*******************************************************************************
* Function Name: Cy_SysPm_DisablePmic
****************************************************************************//**
*
* Disables the PMIC. This function does not affect the output pin. Configures 
* the PMIC input pin polarity. The PMIC input pin has programmable polarity to 
* enable the PMIC using different input polarities. The PMIC output pin is 
* automatically enabled when input polarity and configured polarity matches.
* The function is not effective when the active level of PMIC input pin 
* is equal to configured PMIC polarity.
*
* The function is not effective when the PMIC is locked. Call 
* Cy_SysPm_UnlockPmic() before enabling the PMIC.
*
* \param polarity
* Configures the PMIC wakeup input pin to be active low or active 
* high. See \ref cy_en_syspm_pmic_wakeup_polarity_t.
*
* The PMIC will be enabled automatically by any of RTC alarm or PMIC wakeup 
* event regardless of the PMIC lock state.
*
* \warning
* The PMIC is enabled automatically when you call Cy_SysPm_LockPmic(). 
* To keep the PMIC disabled, the PMIC must remain unlocked.
*
* For information about the PMIC input and output pins and their assignment in 
* the specific families devices, refer to the appropriate device TRM.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysPm_DisablePmic(cy_en_syspm_pmic_wakeup_polarity_t polarity)
{
    CY_ASSERT_L3(CY_SYSPM_IS_POLARITY_VALID(polarity));
    
    if(CY_SYSPM_PMIC_UNLOCK_KEY == _FLD2VAL(BACKUP_PMIC_CTL_UNLOCK, BACKUP->PMIC_CTL))
    {
        BACKUP->PMIC_CTL = 
        (_VAL2FLD(BACKUP_PMIC_CTL_UNLOCK, CY_SYSPM_PMIC_UNLOCK_KEY) | 
         _CLR_SET_FLD32U(BACKUP->PMIC_CTL, BACKUP_PMIC_CTL_POLARITY, (uint32_t) polarity)) &
        ((uint32_t) ~ _VAL2FLD(BACKUP_PMIC_CTL_PMIC_EN, 1U));
    }
}


/*******************************************************************************
* Function Name: Cy_SysPm_AlwaysEnablePmic
****************************************************************************//**
*
* Enables the signal through the PMIC output pin. This is a Write once API, 
* ensure that the PMIC cannot be disabled or polarity changed until a next 
* device reset.
* 
* For information about the PMIC input and output pins and their assignment in 
* the specific families devices, refer to the appropriate device TRM.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysPm_AlwaysEnablePmic(void)
{
    BACKUP->PMIC_CTL |= _VAL2FLD(BACKUP_PMIC_CTL_PMIC_ALWAYSEN, 1U);
}


/*******************************************************************************
* Function Name: Cy_SysPm_EnablePmicOutput
****************************************************************************//**
*
* Enable the PMIC output. 
*
* The function is not effective when the PMIC is locked. Call 
* Cy_SysPm_UnlockPmic() before enabling the PMIC.
*
* For information about the PMIC output pin and it assignment in 
* the specific families devices, refer to the appropriate device TRM.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysPm_EnablePmicOutput(void)
{
    if(CY_SYSPM_PMIC_UNLOCK_KEY == _FLD2VAL(BACKUP_PMIC_CTL_UNLOCK, BACKUP->PMIC_CTL))
    {
        BACKUP->PMIC_CTL |=
        _VAL2FLD(BACKUP_PMIC_CTL_UNLOCK, CY_SYSPM_PMIC_UNLOCK_KEY) | _VAL2FLD(BACKUP_PMIC_CTL_PMIC_EN_OUTEN, 1U);
    }
}


/*******************************************************************************
* Function Name: Cy_SysPm_DisablePmicOutput
****************************************************************************//**
*
* Disable the PMIC output. 
*
* When PMIC output pin is disabled and is unlocked the pmic output pin can be 
* used for the another purpose.
*
* The function is not effective when the PMIC is locked. Call 
* Cy_SysPm_UnlockPmic() before enabling the PMIC.
*
* For information about the PMIC output pin and it assignment in 
* the specific families devices, refer to the appropriate device TRM.
*
* \warning
* The PMIC output is enabled automatically when you call Cy_SysPm_LockPmic(). 
* To keep PMIC output disabled, the PMIC must remain unlocked.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysPm_DisablePmicOutput(void)
{
    if(CY_SYSPM_PMIC_UNLOCK_KEY == _FLD2VAL(BACKUP_PMIC_CTL_UNLOCK, BACKUP->PMIC_CTL))
    {
        BACKUP->PMIC_CTL = 
        (BACKUP->PMIC_CTL | _VAL2FLD(BACKUP_PMIC_CTL_UNLOCK, CY_SYSPM_PMIC_UNLOCK_KEY)) &
        ((uint32_t) ~ _VAL2FLD(BACKUP_PMIC_CTL_PMIC_EN_OUTEN, 1U));
    } 
}


/*******************************************************************************
* Function Name: Cy_SysPm_LockPmic
****************************************************************************//**
*
* Locks the PMIC control register so that no changes can be made. The changes 
* are related to the PMIC enabling/disabling and PMIC output signal 
* enabling/disabling.
*
* \warning
* The PMIC and/or the PMIC output are enabled automatically when 
* you call Cy_SysPm_LockPmic(). To keep the PMIC or PMIC output disabled,
* the PMIC must remain unlocked.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysPm_LockPmic(void)
{
    BACKUP->PMIC_CTL = _CLR_SET_FLD32U(BACKUP->PMIC_CTL, BACKUP_PMIC_CTL_UNLOCK, 0U);
}


/*******************************************************************************
* Function Name: Cy_SysPm_UnlockPmic
****************************************************************************//**
*
* Unlocks the PMIC control register so that changes can be made. The changes are
* related to the PMIC enabling/disabling and PMIC output signal 
* enabling/disabling.
*
* \warning
* The PMIC and/or the PMIC output are enabled automatically when 
* you call Cy_SysPm_LockPmic(). To keep the PMIC or PMIC output disabled,
* the PMIC must remain unlocked.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysPm_UnlockPmic(void)
{
    BACKUP->PMIC_CTL = _CLR_SET_FLD32U(BACKUP->PMIC_CTL, BACKUP_PMIC_CTL_UNLOCK, CY_SYSPM_PMIC_UNLOCK_KEY);
}


/*******************************************************************************
* Function Name: Cy_SysPm_IsPmicEnabled
****************************************************************************//**
* 
* The function returns the status of the PMIC.
*
* \return
* True - The PMIC is enabled. <br>
* False - The PMIC is disabled. <br>
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_IsPmicEnabled(void)
{
    return(0U != _FLD2VAL(BACKUP_PMIC_CTL_PMIC_EN, BACKUP->PMIC_CTL));
}


/*******************************************************************************
* Function Name: Cy_SysPm_IsPmicOutputEnabled
****************************************************************************//**
* 
* The function returns the status of the PMIC output.
*
* \return
* True - The PMIC output is enabled. <br>
* False - The PMIC output is disabled. <br>
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_IsPmicOutputEnabled(void)
{
    return(0U != _FLD2VAL(BACKUP_PMIC_CTL_PMIC_EN_OUTEN, BACKUP->PMIC_CTL));
}


/*******************************************************************************
* Function Name: Cy_SysPm_IsPmicLocked
****************************************************************************//**
*
* Returns the PMIC lock status
*
* \return
* True - The PMIC is locked. <br>
* False - The PMIC is unlocked. <br>
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysPm_IsPmicLocked(void)
{
    return((_FLD2VAL(BACKUP_PMIC_CTL_UNLOCK, BACKUP->PMIC_CTL) == CY_SYSPM_PMIC_UNLOCK_KEY) ? false : true);
}

#ifdef __cplusplus
}
#endif

#endif /* _CY_SYSPM_H_ */

/** \} group_syspm_functions*/
/** \} group_syspm */


/* [] END OF FILE */
