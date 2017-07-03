/***************************************************************************//**
* \file cy_wdt.c
* \version 1.0
*
*  This file provides the source code to the API for the WDT driver.
*
********************************************************************************
* \copyright
* Copyright 2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_wdt.h"
#include "syslib/cy_syslib.h"

#if defined(__cplusplus)
extern "C" {
#endif

static bool Cy_WDT_Locked(void);


/*******************************************************************************
* Function Name: Cy_WDT_Init
****************************************************************************//**
*
* Initialize the Watchdog Timer to its default state.
*
* The default setting of WDT is given as following:
* Whole the Watchdog Timer register counter bits are checked against the match.
* WDT match value is equal - 4096.
*
*******************************************************************************/
void Cy_WDT_Init(void)
{
    uint32_t interruptState;
    interruptState = Cy_SysLib_EnterCriticalSection();

    /* Unlocking WDT by two writes */
    SRSS->WDT_CTL = ((SRSS->WDT_CTL & (uint32_t)(~SRSS_WDT_CTL_WDT_LOCK_Msk)) | CY_SRSS_WDT_LOCK_BIT0);

    SRSS->WDT_CTL |= CY_SRSS_WDT_LOCK_BIT1;

    /* Disabling WDT and setting default values */
    Cy_WDT_Disable();

    Cy_WDT_SetMatch(CY_SRSS_WDT_DEFAULT_MATCH_VALUE);

    Cy_WDT_SetIgnoreBits(CY_SRSS_WDT_DEFAULT_IGNORE_BITS);

    Cy_SysLib_ExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Cy_WDT_Enable
****************************************************************************//**
*
* Enables Watchdog Timer.
*
*******************************************************************************/
void Cy_WDT_Enable(void)
{
    SRSS->WDT_CTL |= _VAL2FLD(SRSS_WDT_CTL_WDT_EN, 1u);
}


/*******************************************************************************
* Function Name: Cy_WDT_Disable
****************************************************************************//**
*
* Disables Watchdog Timer. Watchdog Timer should be unlocked before it is being
* disabled. Call Cy_WDT_Unlock() API to unlock WDT.
*
*******************************************************************************/
void Cy_WDT_Disable(void)
{
    SRSS->WDT_CTL &= ((uint32_t) ~(_VAL2FLD(SRSS_WDT_CTL_WDT_EN, 1u)));
}


/*******************************************************************************
* Function Name: Cy_WDT_Lock
****************************************************************************//**
*
* Locks out configuration changes to the Watchdog Timer register.
*
* After this function is called, the WDT configuration cannot be changed until
* Cy_WDT_Unlock() is called.
*
*******************************************************************************/
void Cy_WDT_Lock(void)
{
    uint32_t interruptState;
    interruptState = Cy_SysLib_EnterCriticalSection();

    SRSS->WDT_CTL |= _VAL2FLD(SRSS_WDT_CTL_WDT_LOCK, CY_SRSS_WDT_LOCK_BITS);

    Cy_SysLib_ExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Cy_WDT_Locked
****************************************************************************//**
* \internal
* Reports the WDT lock state.
*
* \return TRUE - if WDT is locked, and FALSE - if WDT is unlocked.
* \endinternal
*******************************************************************************/
static bool Cy_WDT_Locked(void)
{
    /* Prohibits writing to WDT registers and LFCLK */
    return (0u != _FLD2VAL(SRSS_WDT_CTL_WDT_LOCK, SRSS->WDT_CTL));
}


/*******************************************************************************
* Function Name: Cy_WDT_Unlock
****************************************************************************//**
*
* \brief Unlocks the Watchdog Timer configuration register.
*
*******************************************************************************/
void Cy_WDT_Unlock(void)
{
    uint32_t interruptState;
    interruptState = Cy_SysLib_EnterCriticalSection();

    /* Removing WDT lock requires to be done by two writes */
    SRSS->WDT_CTL = ((SRSS->WDT_CTL & (uint32_t)(~SRSS_WDT_CTL_WDT_LOCK_Msk)) | CY_SRSS_WDT_LOCK_BIT0);

    SRSS->WDT_CTL |= CY_SRSS_WDT_LOCK_BIT1;

    Cy_SysLib_ExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: WDT_SetMatch
****************************************************************************//**
*
* Configures the WDT counter match comparison value. Watchdog Timer
* should be unlocked before changing the match value. Call the Cy_WDT_Unlock()
* function to unlock WDT.
*
* \param match The valid valid range is [0-65535]. The value to be used to match
* against the counter.
*
*******************************************************************************/
void Cy_WDT_SetMatch(uint32_t match)
{
    if (false == Cy_WDT_Locked())
    {
        SRSS->WDT_MATCH = _CLR_SET_FLD32U((SRSS->WDT_MATCH), SRSS_WDT_MATCH_MATCH, match);
    }
}


/*******************************************************************************
* Function Name: Cy_WDT_GetMatch
****************************************************************************//**
*
* Reads the WDT counter match comparison value.
*
* \return The counter match value.
*
*******************************************************************************/
uint32_t Cy_WDT_GetMatch(void)
{
    return ((uint32_t) _FLD2VAL(SRSS_WDT_MATCH_MATCH, SRSS->WDT_MATCH));
}


/*******************************************************************************
* Function Name: Cy_WDT_GetCount
****************************************************************************//**
*
* Reads the current WDT counter value.
*
* \return A live counter value.
*
*******************************************************************************/
uint32_t Cy_WDT_GetCount(void)
{
    return ((uint32_t) _FLD2VAL(SRSS_WDT_CNT_COUNTER, SRSS->WDT_CNT));
}


/*******************************************************************************
* Function Name: Cy_WDT_SetIgnoreBits
****************************************************************************//**
*
* Configures the number of the most significant bits of the Watchdog Timer that
* are not checked against the match. Watchdog Timer should be unlocked before
* ignore bits setting. Call the Cy_WDT_Unlock() API to unlock WDT.
*
* \param
* bitsNum The number of the most significant bits. Valid range is [0-15].
* The bitsNum over 12 are considered as 12.
*
* \details The value of bitsNum controls the time-to-reset of the Watchdog Timer
* (which happens after 3 successive matches).
*
* \warning As this function change the WDT interrupt period, there are possible
* situations when device can go into infinite WDT resets loop. Such situations
* are caused by condition that WDT reset occurs faster that device start-up.
*
*******************************************************************************/
void Cy_WDT_SetIgnoreBits(uint32_t bitsNum)
{
    if (false == Cy_WDT_Locked())
    {
        SRSS->WDT_MATCH = _CLR_SET_FLD32U((SRSS->WDT_MATCH), SRSS_WDT_MATCH_IGNORE_BITS, bitsNum);
    }
}


/*******************************************************************************
* Function Name: Cy_WDT_GetIgnoreBits
****************************************************************************//**
*
* Reads the number of the most significant bits of the Watchdog Timer that are
* not checked against the match.
*
* \return The number of the most significant bits.
*
*******************************************************************************/
uint32_t Cy_WDT_GetIgnoreBits(void)
{
    return((uint32_t) _FLD2VAL(SRSS_WDT_MATCH_IGNORE_BITS ,SRSS->WDT_MATCH));
}


/*******************************************************************************
* Function Name: Cy_WDT_ClearInterrupt
****************************************************************************//**
*
* Cleans the WDT match flag which is set every time the WDT counter reaches a
* WDT match value. Two unserviced interrupts lead to a system reset
* (i.e. at the third match).
*
*******************************************************************************/
void Cy_WDT_ClearInterrupt(void)
{
    SRSS->SRSS_INTR |= _VAL2FLD(SRSS_SRSS_INTR_WDT_MATCH, 1u);

    /* Read SRSS register to ensure that the initial clearing write has been
    * flushed out to the hardware.
    */
    (void) SRSS->SRSS_INTR;
}


/*******************************************************************************
* Function Name: Cy_WDT_MaskInterrupt
****************************************************************************//**
*
* After masking interrupts from WDT, they are not passed to CPU.
* This function does not disable the WDT-reset generation.
*
*******************************************************************************/
void Cy_WDT_MaskInterrupt(void)
{
    SRSS->SRSS_INTR_MASK &= (uint32_t)(~ _VAL2FLD(SRSS_SRSS_INTR_MASK_WDT_MATCH, 1u));
}


/*******************************************************************************
* Function Name: Cy_WDT_UnmaskInterrupt
****************************************************************************//**
*
* After unmasking interrupts from WDT, they are passed to CPU.
* This function does not impact the reset generation.
*
*******************************************************************************/
void Cy_WDT_UnmaskInterrupt(void)
{
    SRSS->SRSS_INTR_MASK |= _VAL2FLD(SRSS_SRSS_INTR_MASK_WDT_MATCH, 1u);
}


/*******************************************************************************
* Function Name: Cy_WDT_ClearWatchdog
****************************************************************************//**
*
* Clears ("feeds") the watchdog, do prevents a XRES device reset.
*
*******************************************************************************/
void Cy_WDT_ClearWatchdog(void)
{
    Cy_WDT_ClearInterrupt();
}

#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */
