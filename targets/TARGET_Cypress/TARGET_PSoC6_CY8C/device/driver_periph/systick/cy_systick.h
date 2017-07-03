/***************************************************************************//**
* \file cy_systick.h
* \version 1.0
*
* Provides the API declarations of the SysTick driver.
*
********************************************************************************
* \copyright
* Copyright 2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CY_SYSTICK_H_
#define _CY_SYSTICK_H_

/**
* \defgroup group_arm_system_timer ARM System Timer (SysTick)
* \{
* Provides a SysTick API.
*
* \section group_systick_configuration Configuration Considerations
*
* The SysTick timer is part of the CPU. The timer is a down counter with a
* 24-bit reload/tick value that is clocked by the FastClk/SlowClk. The timer has
* the capability to generate an interrupt when the set number of ticks expires
* and the counter is reloaded. This interrupt is available as part of the Nested
* Vectored Interrupt Controller (NVIC) for service by the CPU and can be used
* for general purpose timing control in user code.
*
* Changing the SysTick clock source and/or its frequency will change the
* interrupt interval and therefore Cy_SysTick_SetReload() should be called to
* compensate for this change.
*
* Because the timer is independent of the CPU (except for the clock), this can be
* handy in applications requiring precise timing that do not have a dedicated
* timer/counter available for the job.
*
* \section group_systick_more_information More Information
*
* Refer to the SysTick section of the ARM reference guide for complete details
* on the registers and their use.
*
* \section group_systick_MISRA MISRA-C Compliance
*
* <table class="doxtable">
*   <tr>
*       <th>MISRA Rule</th>
*       <th>Rule Class (Required/Advisory)</th>
*       <th>Rule Description</th>
*       <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*       <td>8.8</td>
*       <td>Required</td>
*       <td>An external object or function shall be declared in one and only one file.</td>
*       <td>The variable is not used within project, so is defined without previous declaration.</td>
*   </tr>
* </table>
*
* \section group_systick_changelog Changelog
*
* <table class="doxtable">
* <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
* <tr>
* <td>1.0</td>
* <td>Initial version</td>
* <td></td>
* </tr>
* </table>
*
* \defgroup group_systick_macro Macro
* \defgroup group_systick_functions Functions
* \defgroup group_systick_data_structures Data Structures
*/

#include <stdint.h>
#include <stdbool.h>
#include "syslib/cy_syslib.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \cond */
extern cy_israddress __ramVectors[];
typedef void (*Cy_SysTick_Callback)(void);
/** \endcond */

/**
* \addtogroup group_systick_data_structures
* \{
*/
typedef enum
{
    /** The low frequency clock clk_lf is selected. The precision of this clock depends on whether the low frequency
    clock source is a SRSS internal RC oscillator (imprecise) or a device external crystal oscillator (precise) */
    CY_SYSTICK_CLOCK_SOURCE_CLK_LF    = 0u,

    /** The internal main oscillator (IMO) clock clk_imo is selected. */
    CY_SYSTICK_CLOCK_SOURCE_CLK_IMO   = 1u,

    /** The external crystal oscillator (ECO) clock clk_eco is selected. */
    CY_SYSTICK_CLOCK_SOURCE_CLK_ECO   = 2u,

    /** The SRSS clk_timer is selected. The clk_timer is a divided/gated version of clk_hf or clk_imo. */
    CY_SYSTICK_CLOCK_SOURCE_CLK_TIMER = 3u,

    /** The CPU clock is selected. */
    CY_SYSTICK_CLOCK_SOURCE_CLK_CPU   = 4u,
} cy_en_systick_clock_source_t;

/** \} group_systick_data_structures */


/**
* \addtogroup group_systick_functions
* \{
*/

void Cy_SysTick_Init(cy_en_systick_clock_source_t clockSource, uint32_t interval);
void Cy_SysTick_Enable(void);
void Cy_SysTick_Disable(void);
void Cy_SysTick_EnableInterrupt(void);
void Cy_SysTick_DisableInterrupt(void);
void Cy_SysTick_SetReload(uint32_t value);
uint32_t Cy_SysTick_GetReload(void);
uint32_t Cy_SysTick_GetValue(void);
Cy_SysTick_Callback Cy_SysTick_SetCallback(uint32_t number, Cy_SysTick_Callback function);
Cy_SysTick_Callback Cy_SysTick_GetCallback(uint32_t number);
void Cy_SysTick_SetClockSource(cy_en_systick_clock_source_t clockSource);
cy_en_systick_clock_source_t Cy_SysTick_GetClockSource(void);

uint32_t Cy_SysTick_GetCountFlag(void);
void Cy_SysTick_Clear(void);

/** \} group_systick_functions */


/**
* \addtogroup group_systick_macro
* \{
*/

/** Driver major version */
#define SYSTICK_DRV_VERSION_MAJOR       1

/** Driver minor version */
#define SYSTICK_DRV_VERSION_MINOR       0

/** Number of the callbacks assigned to the SysTick interrupt */
#define CY_SYS_SYST_NUM_OF_CALLBACKS         (5u)

/** Interrupt number in the vector table */
#define CY_SYSTICK_IRQ_NUM                   (15u)

/** \} group_systick_macro */


#ifdef __cplusplus
}
#endif

#endif /* _CY_SYSTICK_H_ */

/** \} group_systick */


/* [] END OF FILE */
