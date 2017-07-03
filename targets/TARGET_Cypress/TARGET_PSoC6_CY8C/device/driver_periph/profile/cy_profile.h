/***************************************************************************//**
* \file cy_profile.h
* \version 1.0
*
* Provides an API declaration of the energy profiler (EP) driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/**
* \defgroup group_energy_profiler Energy Profiler (Profile)
* \{
*
* The energy profiler (EP) driver contains an API for configuring and using the
* energy profiler hardware. The profiler enables measurement of the relative
* amount of energy consumed by particular peripherals. Traditional energy
* profilers correlate energy consumption to the program counter, which helps you
* understand when power is consumed. The EP provides additional insight into the
* device so you can identify an asynchronous activity that causes energy
* consumption. In other words, the EP helps you understand where power is consumed.
*
* The EP does not measure or report actual energy consumption. It measures either
* clock cycles or the number of events that have occurred in the profiling window.
* To derive relative energy consumption for each source, you can multiply the
* absolute count (clock cycles or events) for that source by a coefficient.
*
* Many of the sources available for monitoring are asynchronous operations where
* the cause of energy consumption may be difficult to identify using external hardware.
*
* \section group_profile_configuration Configuration Considerations
*
* There is no PSoC Creator component for this peripheral. To use the profiler you
* include the header file and configure the counters by writing code and calling
* the Profiler API functions.
*
* At the highest level, you perform these tasks:
*  - Initialize the profiling block
*  - Set up the profiling interrupt
*  - Configure, initialize, and enable the counters
*  - Start and stop profiling
*  - Get the results
*  - Exit gracefully
*
* The EP manages a set of counters. For each counter you use, you assign the source
* you want monitored, a reference clock, and a coefficient used to calculate results.
* You can also get the raw count for any counter.
*
* Each counter is a 32-bit register that counts either a number of clock cycles,
* or a number of events. It is possible to overflow the 32-bit register. The
* firmware implements a 32-bit overflow counter. Combined with the 32-bit register,
* this gives you a 64-bit counter for each monitored source. The profiler generates
* an interrupt when an overflow occurs. You must configure the interrupt handler
* using Cy_SysInt_Init(). You can use Cy_Profile_ISR() as the interrupt handler. It
* increments the overflow counter for each profiling counter that is in use.
*
* See notes on individual function definitions.
*
* \section group_profile_more_information More Information
*
* See the EP chapter of the device technical reference manual (TRM).
*
* \section group_profile_MISRA MISRA-C Compliance
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>10.1</td>
*     <td>R</td>
*     <td>The value of an expression of integer type shall not be implicitly converted to a different, underlying type
*         if  the expression is complex.</td>
*     <td>Using a Cypress defined macro to access memory mapped objects.
*         Checking that a function pointer points to within the correct memory region.
*         Calculating address of register structure.</td>
*   </tr>
*   <tr>
*     <td>10.5</td>
*     <td>R</td>
*     <td>If the bitwise operators ~ and << are applied to an operand of underlying type uint8 or uint16, the result
*         shall be immediately cast to the underlying type of the operand.</td>
*     <td>Using a Cypress-defined macro to access memory-mapped objects.</td>
*   </tr>
*   <tr>
*     <td>11.5</td>
*     <td>R</td>
*     <td>Dangerous pointer cast results in loss of volatile qualification.</td>
*     <td>Using a Cypress-defined macro to access memory-mapped objects.</td>
*   </tr>
*   <tr>
*     <td>12.4</td>
*     <td>R</td>
*     <td>Right hand operand of '&&' or '||' is an expression with possible side effects.</td>
*     <td>Function-like macros are used to achieve more efficient code.</td>
*   </tr>
*   <tr>
*     <td>12.7</td>
*     <td>R</td>
*     <td>Bitwise operator applied to signed underlying type.</td>
*     <td>Using a Cypress-defined macro to access memory-mapped objects.</td>
*   </tr>
*   <tr>
*     <td>16.7</td>
*     <td>A</td>
*     <td>A pointer parameter can be of type 'pointer to const'.</td>
*     <td>The pointer is cast for comparison purposes and thus can't be a const.</td>
*   </tr>
*   <tr>
*     <td>19.7</td>
*     <td>A</td>
*     <td>A function shall be used in preference to a function-like macro.</td>
*     <td>Function-like macros are used to achieve more efficient code.</td>
*   </tr>
* </table>
*
* \section group_profile_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_profile_macro Macro
* \{
*   \defgroup group_profile_macro_return    Function return values
* \}
* \defgroup group_profile_functions Functions
* \{
*   \defgroup group_profile_functions_interrupt    Interrupt Functions
*   \defgroup group_profile_functions_general      General Functions
*   \defgroup group_profile_functions_counter      Counter Functions
*   \defgroup group_profile_functions_calculation  Calculation Functions
* \}
* \defgroup group_profile_data_structures Data Structures
* \defgroup group_profile_enums Enumerated Types
*/

#if !defined(CY_PROFILE_H)
#define CY_PROFILE_H

#include "cy_device_headers.h"
#include "syslib/cy_syslib.h"
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/** \addtogroup group_profile_macro
* \{
*/

/** Driver major version */
#define CY_PROFILE_DRV_VERSION_MAJOR  1

/** Driver minor version */
#define CY_PROFILE_DRV_VERSION_MINOR  0

/** Start profiling command for the CMD register */
#define CY_PROFILE_START_TR    1UL

/** Stop profiling command for the CMD register */
#define CY_PROFILE_STOP_TR     2UL

/** Command to clear all counter registers to 0 */
#define CY_PROFILE_CLR_ALL_CNT 0x100UL

/**
* \addtogroup group_profile_returns
* \{
* Specifies return values meaning
*/
/** Command completed with no errors */
#define CY_PROFILE_SUCCESS    0ul
/** Invalid function input parameter */
#define CY_PROFILE_BAD_PARAM  1ul
/** \} group_profile_returns */

/** \} group_profile_macro */


/**
* \addtogroup group_profile_enums
* \{
*/

/**
* The possible values for CTR register, REF_CLK_SEL bitfield. See the
* technical reference manual (TRM) for reference clock considerations.
*/
typedef enum
{
    CY_PROFILE_CLK_TIMER  = 0, /**< See SRSS register CLK_TIMER_CTL.TIMER_SEL */
    CY_PROFILE_CLK_IMO    = 1, /**< Internal main oscillator */
    CY_PROFILE_CLK_ECO    = 2, /**< External crystal oscillator */
    CY_PROFILE_CLK_LF     = 3, /**< See SRSS register CLK_SELECT.LFCLK_SEL */
    CY_PROFILE_CLK_HF     = 4, /**< See SRSS registers CLK_ROOT_SELECT[0].ROOT_MUX and CLK_ROOT_SELECT[0].ROOT_DIV */
    CY_PROFILE_CLK_PERI   = 5, /**< See CPUSS register CM0_CLOCK_CTL.PERI_INT_DIV */
} cy_en_profile_ref_clk_t;
/** \} group_profile_enums */

/**
* \addtogroup group_profile_data_structures
* \{
*/

/**
* EP counter control register structure. For each counter, holds the CTL register fields.
*/
typedef struct
{
    uint8_t                 cntDuration; /**< 0 = events are monitored; 1 = duration is monitored */
    cy_en_profile_ref_clk_t refClkSel;   /**< The reference clock used by this counter; 3 bits */
    en_ep_mon_sel_t         monSel;     /**< The monitor signal to be observed by this counter; # bits = PROFILE_CNT_STRUCT_PRFL_MONITOR_NR_LOG2  */
} cy_stc_profile_ctr_ctl_t;

/**
* Structure holding all information for an EP counter.
*/
typedef struct
{
    uint8_t                   ctrNum;      /**< hardware counter # */
    uint8_t                   used;        /**< used 0 = available, 1 = being used */
    cy_stc_profile_ctr_ctl_t  ctlRegVals;  /**< counter CTL reg bitfield values */
    PROFILE_CNT_STRUCT_Type * cntAddr;     /**< base MMIO addr of counter registers */
    uint32_t                  ctlReg;      /**< also includes enabled/disabled status */
    uint32_t                  cntReg;      /**< current counter value */
    uint32_t                  overflow;    /**< this register and cntReg form a 64-bit counter value */
    uint32_t                  weight;      /**< counter weighting factor */
} cy_stc_profile_ctr_t;
/**
* Pointer to structure holding all information for an EP counter.
*/
typedef cy_stc_profile_ctr_t * cy_stc_profile_ctr_ptr_t;
/** \} group_profile_data_structures */

/**
* \addtogroup group_profile_functions
* \{
*/

/**
* \addtogroup group_profile_functions_interrupt
* \{
*/


/* ========================================================================== */
/* ====================    INTERRUPT FUNCTION SECTION    ==================== */
/* ========================================================================== */
/*******************************************************************************
* Function Name: Cy_Profile_ISR
****************************************************************************//**
*
* EP interrupt handler: Increments the overflow member of the counter structure,
* for each counter that is in use and has an overflow.
*
* This handler is not configured or used automatically. You must configure the
* interrupt handler for the EP, using Cy_SysInt_Init(). Typically you configure
* the system to use \ref Cy_Profile_ISR() as the overflow interrupt handler. You
* can provide a custom interrupt handler to perform additional operations if
* required. Your handler can call \ref Cy_Profile_ISR() to handle counter
* overflow.
*
*******************************************************************************/
void Cy_Profile_ISR(void);
/** \} group_profile_functions_interrupt */

/**
* \addtogroup group_profile_functions_general
* \{
*/


/* ========================================================================== */
/* ==================    GENERAL EP FUNCTIONS SECTION    ==================== */
/* ========================================================================== */
/*******************************************************************************
* Function Name: Cy_Profile_Init
****************************************************************************//**
*
* Turns on the EP for profiling. It must be called once when energy profiling is
* desired.  This does not start a profiling session. Use Cy_Profile_StartProfiling()
* to start a profiling session.
*
* \note Before calling this function, the user must configure the EP interrupt
* so that \ref Cy_Profile_ISR() is executed.
*******************************************************************************/
__STATIC_INLINE void Cy_Profile_Init(void)
{
    PROFILE->CTL = _VAL2FLD(PROFILE_CTL_ENABLED,  1UL/*enabled */) |
                   _VAL2FLD(PROFILE_CTL_WIN_MODE, 0UL/*start/stop mode*/);
    PROFILE->INTR_MASK = 0UL; /* clear all counter interrupt mask bits */
}


/*******************************************************************************
* Function Name: Cy_Profile_DeInit
****************************************************************************//**
*
* Turns off the EP. It should be called when energy profiling is no longer
* desired.
*
* \note When calling this function, the user should consider also unconfiguring
* the EP interrupt.
*******************************************************************************/
__STATIC_INLINE void Cy_Profile_DeInit(void)
{
    PROFILE->CTL = _VAL2FLD(PROFILE_CTL_ENABLED, 0UL/*disabled */);
    PROFILE->INTR_MASK = 0UL; /* clear all counter interrupt mask bits */
}

/*******************************************************************************
* Function Name: Cy_Profile_StartProfiling
****************************************************************************//**
*
* Starts profiling.
*
* \note Before calling this function, the user must enable the EP interrupt.
*******************************************************************************/
void Cy_Profile_StartProfiling(void);

/*******************************************************************************
* Function Name: Cy_Profile_StopProfiling
****************************************************************************//**
*
* Stops profiling.
*
* \note When calling this function, the user should also disable the EP
* interrupt.
*******************************************************************************/
__STATIC_INLINE void Cy_Profile_StopProfiling(void)
{
    PROFILE->CMD = CY_PROFILE_STOP_TR;
}

/*******************************************************************************
* Function Name: Cy_Profile_IsProfiling
****************************************************************************//**
*
* Reports whether or not profiling is active.
*
* \return 0 = profiling is not active; 1 = profiling is active
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_Profile_IsProfiling(void)
{
    return _FLD2VAL(PROFILE_STATUS_WIN_ACTIVE, PROFILE->STATUS);
}
/** \} group_profile_functions_general */

/**
* \addtogroup group_profile_functions_counter
* \{
*/


/* ========================================================================== */
/* ===================    COUNTER FUNCTIONS SECTION    ====================== */
/* ========================================================================== */
/*******************************************************************************
* Function Name: Cy_Profile_ClearConfiguration
****************************************************************************//**
*
* Clears all counter configuration and sets all counters and overflow counters to 0.
* Calls Cy_Profile_ClearCounters() to clear counter registers.
*
*******************************************************************************/
void Cy_Profile_ClearConfiguration(void);

/*******************************************************************************
* Function Name: Cy_Profile_ClearCounters
****************************************************************************//**
*
* Clears all hardware counters to 0.
*
*******************************************************************************/
__STATIC_INLINE void Cy_Profile_ClearCounters(void)
{
    PROFILE->CMD = CY_PROFILE_CLR_ALL_CNT;
}

/*******************************************************************************
* Function Name: Cy_Profile_ConfigureCounter
****************************************************************************//**
*
* Assigns a given monitor source to a counter, and loads the CTL register
* bitfields of an assigned counter.
*
* \param monitor The monitor source #
*
* \param duration Events are monitored (0), or duration is monitored (1)
*
* \param refClk The reference clock to use; see \ref cy_en_profile_ref_clk_t.
* In general, you should use CY_PROFILE_CLK_HF to maximize resolution.
*
* \param weight Weighting factor for the counter value
*
* \return A pointer to the counter data structure. NULL if no counter is
* available.
*
* \note The counter is not enabled by this function. See functions
* \ref Cy_Profile_EnableCounter() and \ref Cy_Profile_DisableCounter(). See the
* technical reference manual (TRM) chapter on the EP for reference clock considerations.
*
*******************************************************************************/
cy_stc_profile_ctr_ptr_t Cy_Profile_ConfigureCounter(en_ep_mon_sel_t monitor, uint32_t duration,
                                                     cy_en_profile_ref_clk_t refClk,  uint32_t weight);

/*******************************************************************************
* Function Name: Cy_Profile_FreeCounter
****************************************************************************//**
*
* Frees up a counter from a previously-assigned monitor source.
* \ref Cy_Profile_ConfigureCounter() must have been called for this counter before
* calling this function.
*
* \param ctrAddr The handle to (address of) the assigned counter, which is
* obtained by a call to \ref Cy_Profile_ConfigureCounter().
*
* \return \ref CY_PROFILE_SUCCESS, or \ref CY_PROFILE_BAD_PARAM for counter not in use.
*
* \note The counter is not disabled by this function. See functions
* \ref Cy_Profile_EnableCounter() and \ref Cy_Profile_DisableCounter().
*******************************************************************************/
uint32_t Cy_Profile_FreeCounter(cy_stc_profile_ctr_ptr_t ctrAddr);

/*******************************************************************************
* Function Name: Cy_Profile_EnableCounter
****************************************************************************//**
*
* Enables an assigned counter. \ref Cy_Profile_ConfigureCounter() must have been
* called for this counter before calling this function.
*
* \param ctrAddr The handle to (address of) the assigned counter, which is
* obtained by a call to \ref Cy_Profile_ConfigureCounter().
*
* \return \ref CY_PROFILE_SUCCESS, or \ref CY_PROFILE_BAD_PARAM for counter not in use.
*
*******************************************************************************/
uint32_t Cy_Profile_EnableCounter(cy_stc_profile_ctr_ptr_t ctrAddr);

/*******************************************************************************
* Function Name: Cy_Profile_DisableCounter
****************************************************************************//**
*
* Disables an assigned counter. \ref Cy_Profile_ConfigureCounter() must have been
* called for this counter before calling this function.
*
* \param ctrAddr The handle to (address of) the assigned counter, which is
* obtained by a call to \ref Cy_Profile_ConfigureCounter().
*
* \return \ref CY_PROFILE_SUCCESS, or \ref CY_PROFILE_BAD_PARAM for counter not in use.
*
*******************************************************************************/
uint32_t Cy_Profile_DisableCounter(cy_stc_profile_ctr_ptr_t ctrAddr);
/** \} group_profile_functions_counter */

/**
* \addtogroup group_profile_functions_calculation
* \{
*/


/* ========================================================================== */
/* ==================    CALCULATION FUNCTIONS SECTION    =================== */
/* ========================================================================== */
/*******************************************************************************
* Function Name: Cy_Profile_GetRawCount
****************************************************************************//**
*
* Reports the count value for a specified counter.
*
* \param ctrAddr the handle to (address of) the assigned counter, which is
* obtained by a call to \ref Cy_Profile_ConfigureCounter().
*
* \param result The address to which to write the result.
*
* \return \ref CY_PROFILE_SUCCESS, or \ref CY_PROFILE_BAD_PARAM for counter not in use.
*
*******************************************************************************/
uint32_t Cy_Profile_GetRawCount(cy_stc_profile_ctr_ptr_t ctrAddr, uint64_t *result);

/*******************************************************************************
* Function Name: Cy_Profile_GetWeightedCount
****************************************************************************//**
*
* Reports the count value for a specified counter, multiplied by the weight
* factor set in \ref Cy_Profile_ConfigureCounter() for that counter.
*
* \param ctrAddr the handle to (address of) the assigned counter, which is
* obtained by a call to \ref Cy_Profile_ConfigureCounter().
*
* \param result The address to which to write the result.
*
* \return \ref CY_PROFILE_SUCCESS, or \ref CY_PROFILE_BAD_PARAM for counter not in use.
*
*******************************************************************************/
uint32_t Cy_Profile_GetWeightedCount(cy_stc_profile_ctr_ptr_t ctrAddr, uint64_t *result);

/*******************************************************************************
* Function Name: Cy_Profile_GetSumWeightedCounts
****************************************************************************//**
*
* Calls \ref Cy_Profile_GetWeightedCount() for all specified counters. Reports the sum
* across all valid counters.
*
* \param ptrsArray Array of handles to (addresses of) assigned counters
*
* \param numCounters Number of scanned elements in ptrsArray[]
*
* \return The sum
*
*******************************************************************************/
uint64_t Cy_Profile_GetSumWeightedCounts(const cy_stc_profile_ctr_ptr_t ptrsArray[],
                                         uint32_t numCounters);
/** \} group_profile_functions_calculation */

/** \} group_profile_functions */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* CY_PROFILE_H */

/** \} group_profile */


/* [] END OF FILE */
