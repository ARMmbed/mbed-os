/***************************************************************************//**
* \file cy_profile.c
* \version 1.0
*
* Provides an API implementation of the energy profiler (EP) driver.
*
********************************************************************************
* \copyright
* Copyright 2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_profile.h"
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/* # of elements in an array */
#define CY_N_ELMTS(a) (sizeof(a)/sizeof((a)[0]))

static uint32_t Cy_Profile_IsPtrValid(const cy_stc_profile_ctr_ptr_t ctrAddr);

/* control and status information for each counter */
static cy_stc_profile_ctr_t cy_ep_ctrs[PROFILE_PRFL_CNT_NR];


/* ========================================================================== */
/* =====================    LOCAL FUNCTIONS SECTION    ====================== */
/* ========================================================================== */
/*******************************************************************************
* Function Name: Cy_Profile_IsPtrValid
****************************************************************************//**
*
* Local utility function: reports (1) whether or not a given pointer points into
* the cy_ep_ctrs[] array, and (2) whether the counter has been assigned.
*
* \param ctrAddr The handle to (address of) the assigned counter
*
* \return CY_PROFILE_SUCCESS, or CY_PROFILE_BAD_PARAM for invalid ctrAddr or counter not
* in use.
*
*******************************************************************************/
static uint32_t Cy_Profile_IsPtrValid(const cy_stc_profile_ctr_ptr_t ctrAddr)
{
    uint32_t retVal = CY_PROFILE_BAD_PARAM;
    /* check for valid ctrAddr */
    uint32_t p_epCtrs = (uint32_t)cy_ep_ctrs;
    if ((p_epCtrs <= (uint32_t)ctrAddr) && ((uint32_t)ctrAddr < (p_epCtrs + (uint32_t)sizeof(cy_ep_ctrs))))
    {
        if (ctrAddr->used != 0u) /* check for counter being used */
        {
            retVal = CY_PROFILE_SUCCESS;
        }
    }
    return (retVal);
}


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
void Cy_Profile_ISR(void)
{
    uint32_t ctr;

    /* Grab a copy of the overflow register. Each bit in the register indicates
       whether or not the respective counter has overflowed. */
    uint32_t ovflowBits = _FLD2VAL(PROFILE_INTR_MASKED_CNT_OVFLW, PROFILE->INTR_MASKED);

    PROFILE->INTR = ovflowBits; /* clear the sources of the interrupts */

    /* scan through the overflow bits, i.e., for each counter */
    for (ctr = 0UL; (ctr < (uint32_t)(PROFILE_PRFL_CNT_NR)) && (ovflowBits != 0UL); ctr++)
    {
        /* Increment the overflow bit only if the counter is being used.
           (Which should always be the case.) */
        if (((ovflowBits & 1UL) != 0UL) && (cy_ep_ctrs[ctr].used != 0u))
        {
            cy_ep_ctrs[ctr].overflow++;
        }
        ovflowBits >>= 1; /* check the next bit, by shifting it into the LS position */
    }
}


/* ========================================================================== */
/* ==================    GENERAL EP FUNCTIONS SECTION    ==================== */
/* ========================================================================== */
/*******************************************************************************
* Function Name: Cy_Profile_StartProfiling
****************************************************************************//**
*
* Starts profiling.
*
* \note Before calling this function, the user must enable the EP interrupt.
*******************************************************************************/
void Cy_Profile_StartProfiling(void)
{
    uint32_t i;

    /* clear all of the counter array overflow variables */
    for (i = 0UL; i < CY_N_ELMTS(cy_ep_ctrs); cy_ep_ctrs[i++].overflow = 0UL){}
    /* send the hardware command */
    PROFILE->CMD = CY_PROFILE_START_TR;
}


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
void Cy_Profile_ClearConfiguration(void)
{
    (void)memset((void *)cy_ep_ctrs, 0, sizeof(cy_ep_ctrs));
    Cy_Profile_ClearCounters();
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
* In general, it is recommended to use CY_PROFILE_CLK_HF to maximize resolution.
*
* \param weight Weighting factor for the counter value
*
* \return A pointer to the counter data structure. NULL if no counter is
* available.
*
* \note The counter is not enabled by this function. See functions
* \ref Cy_Profile_EnableCounter() and \ref Cy_Profile_DisableCounter(). See the
* Technical Reference Manual chapter on the EP for reference clock considerations.
*
*******************************************************************************/
cy_stc_profile_ctr_ptr_t Cy_Profile_ConfigureCounter(en_ep_mon_sel_t monitor, uint32_t duration,
                                                     cy_en_profile_ref_clk_t refClk,  uint32_t weight)
{
    cy_stc_profile_ctr_ptr_t retVal = NULL; /* error value if no counter is available */
    volatile uint8_t i;
    /* scan through the counters for an unused one */
    for (i = 0u; (cy_ep_ctrs[i].used != 0u) && (i < CY_N_ELMTS(cy_ep_ctrs)); i++){}
    if (i < CY_N_ELMTS(cy_ep_ctrs))
    { /* found one, fill in its data structure */
        cy_ep_ctrs[i].ctrNum = i;
        cy_ep_ctrs[i].used = 1u;
        cy_ep_ctrs[i].cntAddr = (PROFILE_CNT_STRUCT_Type *)&(PROFILE->CNT_STRUCT[i]);
        cy_ep_ctrs[i].ctlRegVals.cntDuration = (uint8_t)duration;
        cy_ep_ctrs[i].ctlRegVals.refClkSel = refClk;
        cy_ep_ctrs[i].ctlRegVals.monSel = monitor;
        cy_ep_ctrs[i].overflow = 0UL;
        cy_ep_ctrs[i].weight = weight;
        /* pass back the handle to (address of) the counter data structure */
        retVal = &cy_ep_ctrs[i];

        /* Load the CTL register bitfields of the assigned counter. */
        retVal->cntAddr->CTL =
            _VAL2FLD(PROFILE_CNT_STRUCT_CTL_CNT_DURATION, retVal->ctlRegVals.cntDuration) |
            _VAL2FLD(PROFILE_CNT_STRUCT_CTL_REF_CLK_SEL,  retVal->ctlRegVals.refClkSel)   |
            _VAL2FLD(PROFILE_CNT_STRUCT_CTL_MON_SEL,      retVal->ctlRegVals.monSel);

    }
    return (retVal);
}

/*******************************************************************************
* Function Name: Cy_Profile_FreeCounter
****************************************************************************//**
*
* Frees up a counter from a previously-assigned monitor source.
* \ref Cy_Profile_ConfigureCounter() must have been called for this counter before
* calling this function.
*
* \param ctrAddr The handle to (address of) the assigned counter, which is
* obtained by a call to \ref Cy_Profile_ConfigureCounter()
*
* \return \ref CY_PROFILE_SUCCESS, or \ref CY_PROFILE_BAD_PARAM for counter not in use.
*
* \note The counter is not disabled by this function. See functions
* \ref Cy_Profile_EnableCounter() and \ref Cy_Profile_DisableCounter().
*******************************************************************************/
uint32_t Cy_Profile_FreeCounter(cy_stc_profile_ctr_ptr_t ctrAddr)
{
    uint32_t retVal = Cy_Profile_IsPtrValid(ctrAddr);
    if (retVal == CY_PROFILE_SUCCESS)
    {
        ctrAddr->used = 0u;
    }
    return (retVal);
}

/*******************************************************************************
* Function Name: Cy_Profile_EnableCounter
****************************************************************************//**
*
* Enables an assigned counter. \ref Cy_Profile_ConfigureCounter() must have been
* called for this counter before calling this function.
*
* \param ctrAddr The handle to (address of) the assigned counter, which is
* obtained by a call to \ref Cy_Profile_ConfigureCounter()
*
* \return \ref CY_PROFILE_SUCCESS, or \ref CY_PROFILE_BAD_PARAM for counter not in use.
*
*******************************************************************************/
uint32_t Cy_Profile_EnableCounter(cy_stc_profile_ctr_ptr_t ctrAddr)
{
    uint32_t retVal = Cy_Profile_IsPtrValid(ctrAddr);
    if (retVal == CY_PROFILE_SUCCESS)
    {
        /* set the ENABLED bit */
        ctrAddr->cntAddr->CTL |= _VAL2FLD(PROFILE_CNT_STRUCT_CTL_ENABLED, 1UL);
        /* set the INTR_MASK bit for the counter being used */
        PROFILE->INTR_MASK |= (1UL << (ctrAddr->ctrNum));
    }
    return (retVal);
}

/*******************************************************************************
* Function Name: Cy_Profile_DisableCounter
****************************************************************************//**
*
* Disables an assigned counter. \ref Cy_Profile_ConfigureCounter() must have been
* called for this counter before calling this function.
*
* \param ctrAddr The handle to (address of) the assigned counter, which is
* obtained by a call to \ref Cy_Profile_ConfigureCounter()
*
* \return \ref CY_PROFILE_SUCCESS, or \ref CY_PROFILE_BAD_PARAM for counter not in use.
*
*******************************************************************************/
uint32_t Cy_Profile_DisableCounter(cy_stc_profile_ctr_ptr_t ctrAddr)
{
    uint32_t retVal = Cy_Profile_IsPtrValid(ctrAddr);
    if (retVal == CY_PROFILE_SUCCESS)
    {
        /* clear the ENABLED bit */
        ctrAddr->cntAddr->CTL &= ~(_VAL2FLD(PROFILE_CNT_STRUCT_CTL_ENABLED, 1UL));
        /* clear the INTR_MASK bit for the counter being used */
        PROFILE->INTR_MASK &= ~(1UL << (ctrAddr->ctrNum));
    }
    return (retVal);
}


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
* obtained by a call to \ref Cy_Profile_ConfigureCounter()
*
* \param result the address to which to write the result
*
* \return \ref CY_PROFILE_SUCCESS, or \ref CY_PROFILE_BAD_PARAM for counter not in use.
*
*******************************************************************************/
uint32_t Cy_Profile_GetRawCount(cy_stc_profile_ctr_ptr_t ctrAddr, uint64_t *result)
{
    uint32_t retVal = Cy_Profile_IsPtrValid(ctrAddr);
    if (retVal == CY_PROFILE_SUCCESS)
    {
        /* read the counter control register, and the counter current value */
        ctrAddr->ctlReg = ctrAddr->cntAddr->CTL;
        ctrAddr->cntReg = ctrAddr->cntAddr->CNT;

        /* report the count with overflow */
        *result = ((uint64_t)(ctrAddr->overflow) << 32) | (uint64_t)(ctrAddr->cntReg);
    }
    return (retVal);
}

/*******************************************************************************
* Function Name: Cy_Profile_GetWeightedCount
****************************************************************************//**
*
* Reports the count value for a specified counter, multiplied by the weight
* factor set in \ref Cy_Profile_ConfigureCounter() for that counter.
*
* \param ctrAddr the handle to (address of) the assigned counter, which is
* obtained by a call to \ref Cy_Profile_ConfigureCounter()
*
* \param result the address to which to write the result
*
* \return \ref CY_PROFILE_SUCCESS, or \ref CY_PROFILE_BAD_PARAM for counter not in use.
*
*******************************************************************************/
uint32_t Cy_Profile_GetWeightedCount(cy_stc_profile_ctr_ptr_t ctrAddr, uint64_t *result)
{
    uint64_t temp;
    uint32_t retVal = Cy_Profile_GetRawCount(ctrAddr, &temp);
    if (retVal == CY_PROFILE_SUCCESS)
    {
        /* calculate weighted count */
        *result = temp * (uint64_t)(ctrAddr->weight);
    }
    return (retVal);
}

/*******************************************************************************
* Function Name: Cy_Profile_GetSumWeightedCounts
****************************************************************************//**
*
* Calls \ref Cy_Profile_GetWeightedCount() for all specified counters. Reports the sum
* across all valid counters.
*
* \param ptrsArray array of handles to (addresses of) assigned counters
*
* \param numCounters number of scanned elements in ptrsArray[]
*
* \return The sum
*
*******************************************************************************/
uint64_t Cy_Profile_GetSumWeightedCounts(const cy_stc_profile_ctr_ptr_t ptrsArray[],
                                    uint32_t numCounters)
{
    uint64_t daSum = (uint64_t)0ul;
    uint64_t num;
    uint32_t i;

    for (i = 0ul; i < numCounters; i++)
    {
        /* ignore error reported by Ep_GetWeightedCount() */
        if (Cy_Profile_GetWeightedCount(ptrsArray[i], &num) == CY_PROFILE_SUCCESS)
        {
            daSum += num;
        }
    }

    return (daSum);
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */


/* [] END OF FILE */
