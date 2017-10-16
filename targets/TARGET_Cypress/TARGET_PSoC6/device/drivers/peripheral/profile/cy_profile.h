/***************************************************************************//**
* \file cy_profile.h
* \version 1.0
*
* Provides an API declaration of the profiler driver.
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
* The profiler driver contains an API for configuring and using the 
* profiler hardware. The profiler enables measurement of the relative 
* amount of energy consumed by particular peripherals. Traditional 
* profilers correlate energy consumption to the program counter, which helps you 
* understand when power is consumed. The profiler provides additional insight into the 
* device so you can identify an asynchronous activity that causes energy 
* consumption. In other words, the profiler helps you understand where power is consumed.
*
* The profiler does not measure or report actual energy consumption. It measures either 
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
* The profiler manages a set of counters. For each counter you use, you assign the source
* you want monitored, a reference clock, and a coefficient used to calculate results.
* You can also get the raw count for any counter.
* 
* Each counter is a 32-bit register that counts either a number of clock cycles, 
* or a number of events. It is possible to overflow the 32-bit register. The 
* firmware implements a 32-bit overflow counter. Combined with the 32-bit register, 
* this gives you a 64-bit counter for each monitored source. The profiler generates 
* an interrupt when an overflow occurs. You must configure the interrupt handler 
* using Cy_SysInt_Init(). You can use Cy_Profile_ISR() as the interrupt handler. It 
* increments the overflow counter for each profiling counter that is in use.
*
* When finished profiling, disable the profiler -- do the following:
*  - Stop profiling. See \ref Cy_Profile_StopProfiling().
*  - Clear all profiling configuration. See \ref Cy_Profile_ClearConfiguration().
*  - Disable the profiling interrupt.
*  - Disable the profiler. See \ref Cy_Profile_DeInit().
* 
* See notes on individual function definitions.
*
* \section group_profile_more_information More Information
*
* See the profiler chapter of the device technical reference manual (TRM).
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
*     <td>12.4</td>
*     <td>R</td>
*     <td>Right hand operand of '&&' or '||' is an expression with possible side effects.</td>
*     <td>Function-like macros are used to achieve more efficient code.</td>
*   </tr>
*   <tr>
*     <td>16.7</td>
*     <td>A</td>
*     <td>A pointer parameter can be of type 'pointer to const'.</td>
*     <td>The pointer is cast for comparison purposes and thus can't be a const.</td>
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
* \defgroup group_profile_macros Macros
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

#ifndef CY_IP_MXPROFILE
    #error "The PROFILE driver is not supported on this device"
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/** \addtogroup group_profile_macros
* \{
*/

/** Driver major version */
#define CY_PROFILE_DRV_VERSION_MAJOR  1

/** Driver minor version */
#define CY_PROFILE_DRV_VERSION_MINOR  0

/** Profile driver identifier */
#define CY_PROFILE_ID   CY_PDL_DRV_ID(0x1EU)

/** Start profiling command for the CMD register */
#define CY_PROFILE_START_TR    1UL  

/** Stop profiling command for the CMD register */
#define CY_PROFILE_STOP_TR     2UL  

/** Command to clear all counter registers to 0 */
#define CY_PROFILE_CLR_ALL_CNT 0x100UL

/** \} group_profile_macros */

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

/**
* The possible values for CTR register, CNT_DURATION bitfield. See the
* technical reference manual (TRM) event / duration considerations.
*/
typedef enum 
{
    CY_PROFILE_EVENT    = 0,  /**< count module events  */
    CY_PROFILE_DURATION = 1,  /**< count duration in clock cycles */
} cy_en_profile_duration_t;

/** Profiler status codes */
typedef enum 
{
    /** Operation completed successfully */
    CY_PROFILE_SUCCESS = 0x00U,

    /** One or more of input parameters are invalid */
    CY_PROFILE_BAD_PARAM = CY_PROFILE_ID | CY_PDL_STATUS_ERROR | 1UL

 } cy_en_profile_status_t;

 /** \} group_profile_enums */

/**
* \addtogroup group_profile_data_structures
* \{
*/

/**
* Profiler counter control register structure. For each counter, holds the CTL register fields.
*/
typedef struct
{
    cy_en_profile_duration_t  cntDuration; /**< 0 = events are monitored; 1 = duration is monitored */
    cy_en_profile_ref_clk_t   refClkSel;   /**< The reference clock used by this counter; 3 bits */
    en_ep_mon_sel_t           monSel;      /**< The monitor signal to be observed by this counter; # bits = PROFILE_CNT_STRUCT_PRFL_MONITOR_NR_LOG2  */
} cy_stc_profile_ctr_ctl_t; 

/**
* Structure holding all information for an profiler counter.
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
* Pointer to structure holding all information for an profiler counter.
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
void Cy_Profile_ISR(void);
/** \} group_profile_functions_interrupt */

/**
* \addtogroup group_profile_functions_general
* \{
*/
__STATIC_INLINE void Cy_Profile_Init(void);
__STATIC_INLINE void Cy_Profile_DeInit(void);
void Cy_Profile_StartProfiling(void);
__STATIC_INLINE void Cy_Profile_DeInit(void);
__STATIC_INLINE void Cy_Profile_StopProfiling(void);
__STATIC_INLINE uint32_t Cy_Profile_IsProfiling(void);

/* ========================================================================== */
/* ===============    GENERAL PROFILER FUNCTIONS SECTION    ================= */
/* ========================================================================== */
/*******************************************************************************
* Function Name: Cy_Profile_Init
****************************************************************************//**
*
* Turns on the profiler for profiling. It must be called once when energy profiling is
* desired.  This does not start a profiling session. Use Cy_Profile_StartProfiling() 
* to start a profiling session.
*
* \note Before calling this function, the user must configure the profiler interrupt
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
* Turns off the profiler. It should be called when energy profiling is no longer
* desired.
*
* \note When calling this function, the user should consider also deallocating
* the profiler interrupt.
*******************************************************************************/
__STATIC_INLINE void Cy_Profile_DeInit(void)
{
    PROFILE->CTL = _VAL2FLD(PROFILE_CTL_ENABLED, 0UL/*disabled */);
    PROFILE->INTR_MASK = 0UL; /* clear all counter interrupt mask bits */
}

/*******************************************************************************
* Function Name: Cy_Profile_StopProfiling
****************************************************************************//**
*
* Stops profiling.
*
* \note When calling this function, the user should also disable the profiler
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
void Cy_Profile_ClearConfiguration(void);
__STATIC_INLINE void Cy_Profile_ClearCounters(void);
cy_stc_profile_ctr_ptr_t Cy_Profile_ConfigureCounter(en_ep_mon_sel_t monitor, cy_en_profile_duration_t duration, cy_en_profile_ref_clk_t refClk, uint32_t weight);
cy_en_profile_status_t Cy_Profile_FreeCounter(cy_stc_profile_ctr_ptr_t ctrAddr);
cy_en_profile_status_t Cy_Profile_EnableCounter(cy_stc_profile_ctr_ptr_t ctrAddr);
cy_en_profile_status_t Cy_Profile_DisableCounter(cy_stc_profile_ctr_ptr_t ctrAddr);

/* ========================================================================== */
/* ===================    COUNTER FUNCTIONS SECTION    ====================== */
/* ========================================================================== */
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
/** \} group_profile_functions_counter */

/**
* \addtogroup group_profile_functions_calculation
* \{
*/
/* ========================================================================== */
/* ==================    CALCULATION FUNCTIONS SECTION    =================== */
/* ========================================================================== */
cy_en_profile_status_t Cy_Profile_GetRawCount(cy_stc_profile_ctr_ptr_t ctrAddr, uint64_t *result);
cy_en_profile_status_t Cy_Profile_GetWeightedCount(cy_stc_profile_ctr_ptr_t ctrAddr, uint64_t *result);
uint64_t Cy_Profile_GetSumWeightedCounts(cy_stc_profile_ctr_ptr_t ptrsArray[], uint32_t numCounters);
/** \} group_profile_functions_calculation */

/** \} group_profile_functions */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* CY_PROFILE_H */

/** \} group_profile */


/* [] END OF FILE */
