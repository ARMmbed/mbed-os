/***************************************************************************//**
* \file cy_wdt.h
* \version 1.0
*
*  This file provides constants and parameter values for the WDT driver.
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
* \defgroup group_wdt Watchdog Timer (WDT)
* \{
*
* The Watchdog Timer is a 16-bit free-running up-counter that can issue
* interrupts and device reset if its interrupts are not handled.
* Use the Watchdog Timer for two main purposes.<br>
* The <b> First use case </b> is recovering from a CPU or firmware failure.
* A timeout period is set up in the watchdog timer, and if a timeout occurs, the
* device is reset (WRES). <br>
* The <b>Second use case</b> is to generate periodic interrupts.
*
* A "reset cause" register exists, and the firmware should check this register
* at startup. An appropriate action can be taken if a WRES reset is detected.
*
* The user's firmware periodically resets the timeout period (clears or "feeds"
* the watchdog) before a timeout occurs. If the firmware fails to do so, that is
* considered to be a CPU crash or a firmware failure, and the reason for a
* device reset.
* The WDT can generate an interrupt instead of a device reset. The Interrupt
* Service Routine (ISR) can handle the interrupt either as a periodic interrupt,
* or as an early indication of a firmware failure and respond accordingly.
*
* <b> Functional Description </b> <br>
* The WDT generates an interrupt when the count value in the counter equals the
* configured match value.
*
* It is important that the counter is not reset on a match. When the counter
* reaches a match value, it generates an interrupt and then keeps counting up
* till it overflows and rolls back to zero and reaches the match value again, at
* which point another interrupt is generated.
*
* To use a WDT to generate a periodic interrupt, the match value should be
* incremented in the ISR. As a result, the next WDT interrupt is generated when
* the counter reaches a new match value.
*
* You can also reduce the entire WDT counter period by
* specifying the number of most significant bits that are cut-off in the WDT
* counter. For example, if the Cy_WDT_SetIgnoreBits() function is called with
* parameter 3, the WDT counter becomes a 13-bit free-running up-counter.
*
* <b> Power Modes </b> <br>
* WDT can operate in all possible low power modes.
* Operation during Hibernate mode is possible because the logic and
* high-voltage internal low oscillator (ILO) are supplied by the external
* high-voltage supply (Vddd). The WDT can be configured to wake the device from
* Hibernate mode.
*
* In Active or LPActive mode, an interrupt request from the WDT is sent to the
* CPU via IRQ 22. In Sleep, LPSleep or Deep Sleep power mode, the CPU subsystem
* is powered down, so the interrupt request from the WDT is sent directly to the
* WakeUp Interrupt Controller (WIC), which will then wake up the CPU. The
* CPU then acknowledges the interrupt request and executes the ISR.
*
* After waking from Deep Sleep, an internal timer value is set to zero until the
* ILO loads the register with the correct value. This  can cause an increase in
* low-power mode current consumption. The work around is to wait for the first
* negative edge of the ILO clock before allowing the WDT_CNT register to be read
* by the Cy_WDT_GetCount() function.
*
* <b> Clock Source </b> <br>
* The WDT is clocked by the 32 kHz ILO. The WDT reset must be disabled
* before disabling the ILO. Otherwise, any register write to disable the ILO is
* ignored. Enabling the WDT reset automatically enables the ILO.
* According to the device datasheet, the ILO accuracy is +/-30% over voltage
* and temperature. This means that the timeout period may vary by 30% from the
* configured value. Appropriate margins should be added while configuring WDT
* intervals to make sure that unwanted device resets do not occur on some
* devices.
*
* Refer to the device datasheet for more information on the oscillator accuracy.
*
* <b> Register Locking </b> <br>
* You can prevent accidental corruption of the WDT configuration by calling
* the Cy_WDT_Lock() function. When the WDT is locked, any writing to the WDT_*,
* CLK_ILO_CONFIG, CLK_SELECT.LFCLK_SEL, and CLK_TRIM_ILO_CTL registers is
* ignored.
* Call the Cy_WDT_Unlock() function to allow WDT registers modification.
*
* <b> Clearing WDT </b> <br>
* The ILO clock is asynchronous to the SysClk. As a result it generally
* takes three ILO cycles for WDT register changes to come into effect. It is
* important to remember that a WDT should be cleared at least four cycles
* (3 + 1 for sure) before a timeout occurs, especially when small
* match values / low-toggle bit numbers are used.
*
* \warning There is a risk of situations when WDT reset can be generated
* faster than device start-up. Be aware of such situations by calculating the
* start-up time and WDT reset time. The WDT reset time should be always smaller
* than device start-up time.
*
* <b> Reset Detection </b> <br>
* Use the Cy_SysLib_GetResetReason() function to detect whether the WDT has
* triggered a device reset.
*
* <b> Interrupt Configuration </b> <br>
* The Global Signal Reference and Interrupt components can be used for ISR
* configuration. If the WDT is configured to generate an interrupt, the pending
* interrupts must be cleared within the ISR (otherwise, the interrupt will be
* generated continuously).
* A pending interrupt to the WDT block must be cleared by the call to the
* Cy_WDT_ClearInterrupt() function. The call to the function will clear the
* unhandled WDT interrupt counter.
*
* Use the WDT ISR as a timer to trigger certain actions
* and to change a next WDT match value.
*
* Ensure that the interrupts from the WDT are passed to the CPU to avoid
* unregistered interrupts. Unregistered WDT interrupts result in continuous
* device reset. To avoid a continuous device reset, call
* Cy_WDT_UnmaskInterrupt(). After that, call the WDT API functions for interrupt
* handling/clearing.
*
* \section group_wdt_configuration Configuration Considerations
*
* To start the WDT, make sure that ILO is enabled.
* After the ILO is enabled, ensure that the WDT is unlocked and disabled by calling
* the Cy_WDT_Unlock() and Cy_WDT_Disable() functions. Set the WDT match value by
* calling Cy_WDT_SetMatch() with the required match value. If needed, set the
* ignore bits for reducing the WDT counter period by calling Cy_WDT_SetIgnoreBits()
* function. After the WDT configuration is set, call Cy_WDT_Enable().
*
* \note Enable a WDT if the power supply can produce
* sudden brownout events that may compromise the CPU functionality. This
* ensures that the system can recover after a brownout.
*
* When the WDT is used to protect against system crashes, the
* WDT interrupt should be cleared by a portion of the code that is not directly
* associated with the WDT interrupt.
* Otherwise, it is possible that the main firmware loop has crashed or is in an
* endless loop, but the WDT interrupt vector continues to operate and service
* the WDT. The user should:
* * Feed the watchdog by clearing the interrupt bit regularly in the main body
* of the firmware code.
*
* * Guarantee that the interrupt is cleared at least once every WDT period.
*
* * Use the WDT ISR only as a timer to trigger certain actions and to change the
* next match value.
*
* \section group_wdt_section_more_information More Information
*
* For more information on the WDT peripheral, refer to the technical reference
* manual (TRM).
*
* \section group_wdt_MISRA MISRA-C Compliance
* The WDT driver does not have any specific deviations.
*
* \section group_wdt_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_wdt_macro Macro
* \defgroup group_wdt_functions Functions
*
*/

#if !defined(_WDT_H_)
#define _WDT_H_

#include <stdint.h>
#include <stdbool.h>
#include "cy_device_headers.h"


#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
*        Function Prototypes
*******************************************************************************/

/**
* \addtogroup group_wdt_functions
* @{
*/
    /* WDT API */
    void     Cy_WDT_Init(void);
    void     Cy_WDT_Enable(void);
    void     Cy_WDT_Disable(void);
    void     Cy_WDT_Lock(void);
    void     Cy_WDT_Unlock(void);
    void     Cy_WDT_SetMatch(uint32_t match);
    uint32_t Cy_WDT_GetMatch(void);
    uint32_t Cy_WDT_GetCount(void);
    void     Cy_WDT_SetIgnoreBits(uint32_t bitsNum);
    uint32_t Cy_WDT_GetIgnoreBits(void);
    void     Cy_WDT_ClearInterrupt(void);
    void     Cy_WDT_MaskInterrupt(void);
    void     Cy_WDT_UnmaskInterrupt(void);
    void     Cy_WDT_ClearWatchdog(void);
/** \} group_wdt_functions */


/*******************************************************************************
*       Function Constants
*******************************************************************************/

/**
* \addtogroup group_wdt_macro
* \{
*/

/** Driver major version */
#define CY_WDT_DRV_VERSION_MAJOR                       1

/** Driver minor version */
#define CY_WDT_DRV_VERSION_MINOR                       0

/** Internal define for first iteration of WDT unlocking */
#define CY_SRSS_WDT_LOCK_BIT0                           ((uint32_t)0x01u << 30u)

/** Internal define for second iteration of WDT unlocking */
#define CY_SRSS_WDT_LOCK_BIT1                           ((uint32_t)0x01u << 31u)

/** WDT default match value */
#define CY_SRSS_WDT_DEFAULT_MATCH_VALUE                 ((uint32_t) 4096u)

/** Default match value of WDT ignore bits */
#define CY_SRSS_WDT_DEFAULT_IGNORE_BITS                 (0u)

/** Default match value of WDT ignore bits */
#define CY_SRSS_WDT_LOCK_BITS                           (3u)

/** WDT driver identifier */
#define CY_WDT_ID                                       CY_PDL_DRV_ID(0x34u)

/** \} group_wdt_macro */

#if defined(__cplusplus)
}
#endif

#endif /* _WDT_H_ */

/** \} group_wdt */


/* [] END OF FILE */
