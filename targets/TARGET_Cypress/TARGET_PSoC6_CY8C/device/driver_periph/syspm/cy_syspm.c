/***************************************************************************//**
* \file cy_syspm.c
* \version 1.0
*
* This driver provides the source code to the API for the power management.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#include "cy_syspm.h"

static cy_stc_syspm_callback_t* callbackRoot = NULL;
static uint32_t currentRegisteredCallbacksNumber = 0u;

static void Cy_SysPm_SetDelayAfterDeepSleep(void);

#if(0u != SRSS_SIMOBUCK_PRESENT)

    static void Cy_SysPm_SetVoltageBitForFlash(bool setBit);

#endif /* (0u != SRSS_SIMOBUCK_PRESENT) */


/*******************************************************************************
* Function Name: Cy_SysPm_ReadStatus
****************************************************************************//**
*
* Reads the status of the core(s).
*
* \return The current power mode. See \ref group_syspm_return_status.
*
*******************************************************************************/
uint32_t Cy_SysPm_ReadStatus(void)
{
    uint32_t interruptState;
    uint32_t pmStatus = 0u;
    interruptState = Cy_SysLib_EnterCriticalSection();

    /* Check whether CM4 is in DeepSleep mode*/
    if((0u != _FLD2VAL(CPUSS_CM4_STATUS_SLEEPING, CPUSS->CM4_STATUS)) &&
       (0u != _FLD2VAL(CPUSS_CM4_STATUS_SLEEPDEEP, CPUSS->CM4_STATUS)))
    {
        pmStatus |= CY_SYSPM_STATUS_CM4_DEEPSLEEP;
    }
    /* Check whether CM4 is in Sleep mode*/
    else if(0u != _FLD2VAL(CPUSS_CM4_STATUS_SLEEPING, CPUSS->CM4_STATUS))
    {
        pmStatus |= CY_SYSPM_STATUS_CM4_SLEEP;
    }
        else
        {
            pmStatus |= CY_SYSPM_STATUS_CM4_ACTIVE;
        }

    /* Check whether CM0p is in DeepSleep mode*/
    if((0u != _FLD2VAL(CPUSS_CM0_STATUS_SLEEPING, CPUSS->CM0_STATUS)) &&
       (0u != _FLD2VAL(CPUSS_CM0_STATUS_SLEEPDEEP, CPUSS->CM0_STATUS)))
    {
        pmStatus |= (uint32_t) CY_SYSPM_STATUS_CM0_DEEPSLEEP;
    }
    /* Check whether CM0p is in Sleep mode*/
    else if (0u != _FLD2VAL(CPUSS_CM0_STATUS_SLEEPING, CPUSS->CM0_STATUS))
    {
        pmStatus |= CY_SYSPM_STATUS_CM0_SLEEP;
    }
        else
        {
            pmStatus |= CY_SYSPM_STATUS_CM0_ACTIVE;
        }

    /* Check whether device is in Low Power mode by reading
    *  the Active Reference status
    */
    if(0u != (_FLD2VAL(SRSS_PWR_CTL_ACT_REF_DIS, SRSS->PWR_CTL)))
    {
        pmStatus |= CY_SYSPM_STATUS_SYSTEM_LOWPOWER;
    }
    Cy_SysLib_ExitCriticalSection(interruptState);

    return(pmStatus);
}


/*******************************************************************************
* Function Name: Cy_SysPm_Sleep
****************************************************************************//**
*
* Sets a CPU core to Sleep mode.
*
* Puts the core into the Sleep power mode, if none of callback functions were
* registered.
*
* If at least one callback function with CY_SYSPM_SLEEP type was registered, the
* next algorithm is executed.
* Prior to entering Sleep mode, all callback functions with CY_SYSPM_SLEEP type
* with CY_SYSPM_CHECK_READY parameter are called allowing a driver
* to signal whether it is ready to enter the low power mode. If any of
* callbacks with CY_SYSPM_SLEEP type with CY_SYSPM_CHECK_READY parameter
* returns CY_SYSPM_FAIL the remaining callback with CY_SYSPM_SLEEP type with
* CY_SYSPM_CHECK_READY parameter calls are skipped.
* After a CY_SYSPM_FAIL, all of the CY_SYSPM_SLEEP callbacks with
* CY_SYSPM_CHECK_FAIL parameter are executed that correspond to the callback
* with callbacks with CY_SYSPM_SLEEP type with CY_SYSPM_CHECK_READY
* parameter calls that occurred up to the point of failure. Sleep mode is not
* entered and the Cy_SysPm_Sleep API returns CY_SYSPM_FAIL.
*
* If all of the callbacks with CY_SYSPM_SLEEP type with CY_SYSPM_CHECK_READY
* parameter calls return CY_SYSPM_SUCCESS then all callbacks with CY_SYSPM_SLEEP
* type with CY_SYSPM_CHECK_FAIL parameters calls are skipped and all callbacks
* with CY_SYSPM_SLEEP type and CY_SYSPM_BEFORE_ENTER parameter calls are
* executed, allowing peripherals to prepare for Sleep. Sleep mode is then
* entered for the CPU. This is a CPU-centric power mode. It means that the CPU
* has entered Sleep mode and its main clock is removed. It is identical to
* Active from a peripheral point of view. Any enabled interrupts can cause a
* wakeup from Sleep mode.
*
* After wakeup from Sleep all of the registered callbacks with CY_SYSPM_SLEEP
* type and with CY_SYSPM_AFTER_EXIT parameter are executed to return peripherals
* to Active operation. The Cy_SysPm_Sleep API returns CY_SYSPM_SUCCESS.
* No callbacks with CY_SYSPM_SLEEP type with CY_SYSPM_BEFORE_ENTER parameter or
* callbacks with CY_SYSPM_SLEEP type and CY_SYSPM_AFTER_EXIT parameter callbacks
* are executed if Sleep mode is not entered.
*
* \param enWaitFor Selects wait for action. See \ref cy_en_syspm_waitfor_t.
*
* \return
* CY_SYSPM_SUCCESS - Entered and exited from Sleep <br>
* CY_SYSPM_FAIL - Sleep not entered. See \ref cy_en_syspm_status_t.
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_Sleep(cy_en_syspm_waitfor_t enWaitFor)
{
    uint32_t interruptState;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;

    /* Call registered callback functions with CY_SYSPM_CHECK_READY parameter */
    if(0u != currentRegisteredCallbacksNumber)
    {
        retVal = (cy_en_syspm_status_t) Cy_SysPm_ExecuteCallback(CY_SYSPM_SLEEP, CY_SYSPM_CHECK_READY, 0u);
    }

    /* The device (core) can switch into Sleep power mode only when
    *  all executed registered callback functions with CY_SYSPM_CHECK_READY
    *  parameter returned CY_SYSPM_SUCCESS.
    */
    if(retVal == CY_SYSPM_SUCCESS)
    {
        /* Call registered callback functions with CY_SYSPM_BEFORE_ENTER
        *  parameter. Return value is ignored.
        */
        interruptState = Cy_SysLib_EnterCriticalSection();
        if(0u != currentRegisteredCallbacksNumber)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_SLEEP, CY_SYSPM_BEFORE_ENTER, 0u);
        }

        /* CPU enters Sleep power mode upon execution of WFI/WFE */
        SCB->SCR = _CLR_SET_FLD32U((SCB->SCR), SCB_SCR_SLEEPDEEP, 0u);

        if(enWaitFor != CY_SYSPM_WAIT_FOR_EVENT)
        {
            __WFI();
        }
        else
        {
            __WFE();
        }
        Cy_SysLib_ExitCriticalSection(interruptState);

        /* Call registered callback functions with CY_SYSPM_AFTER_EXIT
        *  parameter. Return value is ignored.
        */
        if(0u != currentRegisteredCallbacksNumber)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_SLEEP, CY_SYSPM_AFTER_EXIT, 0u);
        }
    }
    else
    {
        /* Execute callback functions with CY_SYSPM_CHECK_FAIL parameter to
        *  undo everything done in the CY_SYSPM_CHECK_READY callback. Return
        * value is ignored.
        */
        (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_SLEEP, CY_SYSPM_CHECK_FAIL, (uint32_t) retVal);
        retVal = CY_SYSPM_FAIL;
    }
    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_DeepSleep
****************************************************************************//**
*
* Sets a CPU core to DeepSleep mode.
*
* Puts the core into the Deep Sleep power mode. Prior to entering Deep Sleep
* mode, all callbacks with CY_SYSPM_DEEPSLEEP type with CY_SYSPM_CHECK_READY
* parameter registered callbacks are called, allowing a driver to signal whether
* it is ready to enter the power mode. If any CY_SYSPM_DEEPSLEEP
* type with CY_SYSPM_CHECK_READY parameter call returns CY_SYSPM_FAIL, the
* remaining callback CY_SYSPM_DEEPSLEEP type with CY_SYSPM_CHECK_READY parameter
* calls are skipped. After a CY_SYSPM_FAIL, all of the callbacks with
* CY_SYSPM_DEEPSLEEP type with CY_SYSPM_CHECK_FAIL parameter are executed that
* correspond to the callbacks with CY_SYSPM_DEEPSLEEP type with
* CY_SYSPM_CHECK_READY parameter calls that occurred up to the point of failure.
* Deep Sleep mode is not entered and the Cy_SysPm_Sleep API returns
* CY_SYSPM_FAIL.
*
* If all callbacks with CY_SYSPM_DEEPSLEEP type with CY_SYSPM_CHECK_READY
* parameter calls return CY_SYSPM_SUCCESS, then all callbacks with
* CY_SYSPM_DEEPSLEEP type with CY_SYSPM_CHECK_FAIL parameter calls are skipped
* and all callbacks with CY_SYSPM_DEEPSLEEP type with CY_SYSPM_BEFORE_ENTER
* parameter calls are executed, allowing peripherals to prepare for Deep Sleep.
* Deep Sleep mode is then entered. Any enabled interrupts can cause a wakeup
* from Deep Sleep mode.
*
* If the firmware attempts to enter this mode before the system is ready (that
* is, when PWR_CONTROL.LPM_READY = 0), then the device will go into the (LP)
* Sleep mode instead and automatically enter the DeepSleep mode when the
* system is ready.
*
* The SRSS puts the whole device into DeepSleep mode when all the processors
* are in SLEEPDEEP, there are no busy peripherals, the debugger is not active,
* and the DEEPSLEEP circuits in the SRSS are ready (PWR_CONTROL.LPM_READY=1).
*
* Peripherals not needing a clock or that receive a clock from their external
* interface (e.g. I2C/SPI) continue operating. All circuits using the current
* from Vccdpslp supply are under the current limitation that is controlled
* by DeepSleep regulator.
*
* Wakeup occurs when an interrupt is received from a DeepSleep active
* peripheral. For more details, see the corresponding peripheral's datasheet.
*
* A normal wakeup from DeepSleep returns to either LPActive, LPSleep, Active, or
* Sleep, depending on the previous state and programmed behaviour for the
* particular wakeup interrupt.
*
* After wakeup from Deep Sleep, all of the registered callbacks with
* CY_SYSPM_DEEPSLEEP type with CY_SYSPM_AFTER_EXIT are executed to return
* peripherals to Active operation. The Cy_SysPm_DeepSleep API returns
* CY_SYSPM_SUCCESS. No callbacks are executed with CY_SYSPM_DEEPSLEEP type with
* CY_SYSPM_BEFORE_ENTER or CY_SYSPM_AFTER_EXIT parameter, if Deep Sleep
* mode is not entered.
*
* \param enWaitFor Selects wait for action. See \ref cy_en_syspm_waitfor_t.
*
* \return
* CY_SYSPM_SUCCESS - Entered and exited from DeepSleep <br>
* CY_SYSPM_FAIL - DeepSleep not entered. See \ref cy_en_syspm_status_t.
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_DeepSleep(cy_en_syspm_waitfor_t enWaitFor)
{
    uint32_t interruptState;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;

    /* Call registered callback functions with CY_SYSPM_CHECK_READY parameter */
    if(0u != currentRegisteredCallbacksNumber)
    {
        retVal = (cy_en_syspm_status_t) Cy_SysPm_ExecuteCallback(CY_SYSPM_DEEPSLEEP, CY_SYSPM_CHECK_READY, 0u);
    }

    /* The device (core) can switch into Deep Sleep power mode only when
    *  all executed registered callback functions with CY_SYSPM_CHECK_READY
    *  parameter returned CY_SYSPM_SUCCESS.
    */
    if(retVal == CY_SYSPM_SUCCESS)
    {
        /* Call registered callback functions with CY_SYSPM_BEFORE_ENTER
        * parameter. Return value is ignored.
        */
        interruptState = Cy_SysLib_EnterCriticalSection();
        if(0u != currentRegisteredCallbacksNumber)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_DEEPSLEEP, CY_SYSPM_BEFORE_ENTER, 0u);
        }
        Cy_SysPm_SetDelayAfterDeepSleep();

        /* CPU enters DeepSleep mode upon execution of WFI/WFE */
        SCB->SCR = _CLR_SET_FLD32U((SCB->SCR), SCB_SCR_SLEEPDEEP, 1u);

        if(enWaitFor != CY_SYSPM_WAIT_FOR_EVENT)
        {
            __WFI();
        }
        else
        {
            __WFE();
        }
        Cy_SysLib_ExitCriticalSection(interruptState);

        /* Call registered callback functions with CY_SYSPM_AFTER_EXIT
        *  parameter. Return value is ignored.
        */
        if(0u != currentRegisteredCallbacksNumber)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_DEEPSLEEP, CY_SYSPM_AFTER_EXIT, 0u);
        }
    }
    else
    {
        /* Execute callbacks functions with CY_SYSPM_CHECK_FAIL parameter to
        *  undo everything done in the CY_SYSPM_CHECK_READY callback. Return
        *  value is ignored.
        */
        (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_DEEPSLEEP, CY_SYSPM_CHECK_FAIL, (uint32_t) retVal);
        retVal = CY_SYSPM_FAIL;
    }
    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_Hibernate
****************************************************************************//**
*
* Sets the device into Hibernate mode.
*
* Puts the core into the Hibernate power mode. Prior to entering Hibernate mode
* all callbacks with CY_SYSPM_HIBERNATE type are executed.
* At the first time callbacks with CY_SYSPM_HIBERNATE type and with
* CY_SYSPM_CHECK_READY parameter are called allowing a driver to signal it is
* not ready to enter the power mode. If any of callbacks CY_SYSPM_HIBERNATE type
* with CY_SYSPM_CHECK_READY parameter call returns CY_SYSPM_FAIL the
* remaining CY_SYSPM_HIBERNATE callbacks with CY_SYSPM_CHECK_READY parameter
* calls are skipped. After a CY_SYSPM_FAIL all of the CY_SYSPM_HIBERNATE
* callbacks with CY_SYSPM_CHECK_FAIL parameter are executed that correspond to
* the CY_SYSPM_HIBERNATE callbacks with CY_SYSPM_CHECK_READY parameter calls
* that occurred up to the point of failure. Hibernate mode is not entered
* and the Cy_SysPm_Hibernate API returns CY_SYSPM_FAIL.
*
* If all CY_SYSPM_HIBERNATE callbacks with CY_SYSPM_CHECK_READY parameter calls
* return CY_SYSPM_SUCCESS, then all CY_SYSPM_HIBERNATE callbacks with
* CY_SYSPM_CHECK_FAIL calls are skipped and all CY_SYSPM_HIBERNATE callbacks
* CY_SYSPM_BEFORE_ENTER parameter calls are executed allowing peripherals to
* prepare for Hibernate. I/O output state is frozen and Hibernate mode is then
* entered. In Hibernate mode all internal supplies are off and no internal state
* is retained. There is no handshake with the CPUs and the chip will begin entry
* to Hibernate immediately.
*
* A wakeup from Hibernate is triggered by toggling the wakeup pin(s), a WDT
* match, or back up domain alarm expiration depending on how the HIBERNATE
* register is configured. A wakeup causes a normal boot procedure to occur.
* To configure the wakeup pin(s), a Digital Input Pin must be configured, and
* resistively pulled up or down to the inverse state of the wakeup polarity. To
* distinguish a wakeup from the Hibernate mode and a general reset event,
* Cy_SysLib_GetResetReason() function can be used. The wakeup pin is
* active-low by default. The wakeup pin polarity can be changed with the
* \ref Cy_SysPm_WakeupPinLevel() function. This function call will not return if
* Hibernate mode is entered. The CY_SYSPM_HIBERNATE callbacks with
* CY_SYSPM_AFTER_EXIT parameter are never executed.
*
* This function freezes IO cells implicitly. It is not possible to enter
* the Hibernate mode before freezing the IO cells. The IO cells remain frozen
* after waking from Hibernate mode until firmware unfreezes them
* with a \ref Cy_SysPm_IoUnfreeze() function call.
*
* \return
* CY_SYSPM_SUCCESS - Not valid as hibernate; can only exit through a reset. <br>
* CY_SYSPM_FAIL - Hibernate not entered. See \ref cy_en_syspm_status_t.
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_Hibernate(void)
{
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;

    /* Call registered callback functions with CY_SYSPM_CHECK_READY parameter */
    if(0u != currentRegisteredCallbacksNumber)
    {
        retVal = (cy_en_syspm_status_t) Cy_SysPm_ExecuteCallback(CY_SYSPM_HIBERNATE, CY_SYSPM_CHECK_READY, 0u);
    }

    /* The device (core) can switch into Hibernate power mode only when
    *  all executed registered callback functions with CY_SYSPM_CHECK_READY
    *  parameter returned CY_SYSPM_SUCCESS.
    */
    if(retVal == CY_SYSPM_SUCCESS)
    {
        /* Call registered callback functions with CY_SYSPM_BEFORE_ENTER
        *  parameter. Return value is ignored.
        */
        (void) Cy_SysLib_EnterCriticalSection();
        if(0u != currentRegisteredCallbacksNumber)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_HIBERNATE, CY_SYSPM_BEFORE_ENTER, 0u);
        }

        /* Safe retain bit-fields before entering into Hibernate power mode. */
        SRSS->PWR_HIBERNATE &= CY_SYSPM_PWR_RETAIN_HIBERNATE_STATUS;

        /* All three writes to HIBERNATE register use same value:
        *  PWR_HIBERNATE.FREEZE=1, .UNLOCK=0x3A, .HIBERANTE=1,
        */
        SRSS->PWR_HIBERNATE |=  CY_SYSPM_PWR_HIBERNATE_UNLOCK |
                                _VAL2FLD(SRSS_PWR_HIBERNATE_FREEZE, 1u) |
                                _VAL2FLD(SRSS_PWR_HIBERNATE_HIBERNATE, 1u);

        SRSS->PWR_HIBERNATE |=  CY_SYSPM_PWR_HIBERNATE_UNLOCK |
                                _VAL2FLD(SRSS_PWR_HIBERNATE_FREEZE, 1u) |
                                _VAL2FLD(SRSS_PWR_HIBERNATE_HIBERNATE, 1u);

        SRSS->PWR_HIBERNATE |=  CY_SYSPM_PWR_HIBERNATE_UNLOCK |
                                _VAL2FLD(SRSS_PWR_HIBERNATE_FREEZE, 1u) |
                                _VAL2FLD(SRSS_PWR_HIBERNATE_HIBERNATE, 1u);

        /* Wait for transition */
        __WFI();

        /* The callback functions calls with CY_SYSPM_AFTER_EXIT parameter in
        *  Hibernate power mode is is not applicable as device wake-up is made
        *  on device reboot.
        */

        /* A wakeup from Hibernate is performed by toggling of wakeup pins, or
        *  WDT matches, or Backup domain alarm expires. Depends on what item is
        *  configured in HIBERNATE register. After a wakeup event, a normal Boot
        *  procedure occurs. No need to exit from the critical section.
        */
    }
    else
    {
        /* Execute callback functions with CY_SYSPM_CHECK_FAIL parameter to
        *  undo everything done in the CY_SYSPM_CHECK_READY callback. Return
        *  value is ignored.
        */
        (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_HIBERNATE, CY_SYSPM_CHECK_FAIL, (uint32_t) retVal);
        retVal = CY_SYSPM_FAIL;
    }
    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_EnterLpMode
****************************************************************************//**
*
* Sets device into Low Power mode.
*
* The low power (LPActive/LPSleep) modes are similar to the Active/Sleep modes.
* The difference is that the current is limited and some functions have limited
* features/performance.
*
* The feature of the low power mode is limited current. Restrictions are placed
* on the clock frequencies and allowed peripherals to achieve current limit.
*
* Before entering low power mode, the user should configure the system so the total
* current drawn from Vccd is less that value presented in the technical
* reference manual (TRM). Refer to TRM for the maximum load for low power
* operation and clock limitations in low power mode with different core supply
* regulator voltages.
*
* * Peripherals can use the knowledge of LPActive/LPSleep mode to make
* trade-offs that consume less current. For more details, see the corresponding
* peripheral's datasheet.
* * High-speed clock sources are available with appropriate predivider settings
* to limit the system current.
* Refer to the TRM for the maximum frequency values for low power operation using
* different Core Regulators output voltages.
*
* Puts the device into the Low Power mode. Prior to entering Low Power mode, all
* registered CY_SYSPM_ENTER_LP_MODE callbacks with CY_SYSPM_CHECK_READY
* parameter are called. This allows a driver to signal it is not ready to enter the
* Low Power mode. If any CY_SYSPM_ENTER_LP_MODE callbacks with
* CY_SYSPM_CHECK_READY parameter call returns CY_SYSPM_FAIL the remaining
* CY_SYSPM_ENTER_LP_MODE callbacks with CY_SYSPM_CHECK_READY parameter calls are
* skipped.
* After a CY_SYSPM_FAIL all of the CY_SYSPM_ENTER_LP_MODE callbacks with
* CY_SYSPM_CHECK_FAIL parameter are executed that correspond to the
* CY_SYSPM_ENTER_LP_MODE callbacks with CY_SYSPM_CHECK_READY parameter calls
* that occurred up to the point of failure. Low Power mode is not entered and
* the Cy_SysPm_EnterLpMode API returns CY_SYSPM_FAIL.
*
* If all CY_SYSPM_ENTER_LP_MODE callbacks with CY_SYSPM_CHECK_READY parameter
* calls return CY_SYSPM_SUCCESS then all CY_SYSPM_ENTER_LP_MODE callbacks with
* CY_SYSPM_CHECK_FAIL calls are skipped and all CY_SYSPM_ENTER_LP_MODE callbacks
* with CY_SYSPM_BEFORE_ENTER parameter calls are executed. This allows peripherals
* to prepare for Low Power. Low Power mode is then entered.
*
* After entering Low Power mode, all of the registered CY_SYSPM_ENTER_LP_MODE
* callbacks with CY_SYSPM_AFTER_EXIT parameter are executed to complete
* preparing peripherals for Low Power operation. The Cy_SysPm_EnterLpMode API
* returns CY_SYSPM_SUCCESS. No CY_SYSPM_ENTER_LP_MODE callbacks with
* CY_SYSPM_BEFORE_ENTER or CY_SYSPM_AFTER_EXIT parameter are executed, if Low
* Power mode is not entered.
*
* \note The callbacks are not executed if device already is not in low power
* mode.
*
* \return
* CY_SYSPM_SUCCESS - Entered LPActive mode or device is already in LPActive. <br>
* CY_SYSPM_FAIL - LPActive mode not entered or low power circuits are not ready
* to enter into low power mode. See \ref cy_en_syspm_status_t.
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_EnterLpMode(void)
{
    uint32_t interruptState;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;

    /* Check whether device is in Low Power mode */
    if(0u == (_FLD2VAL(SRSS_PWR_CTL_ACT_REF_DIS, SRSS->PWR_CTL)))
    {
        /* The entering into low power mode is permitted when low
        *  power circuits are ready to enter into low power mode.
        */
        if(0u != _FLD2VAL(SRSS_PWR_CTL_LPM_READY, SRSS->PWR_CTL))
        {
            /* Call registered callback functions with CY_SYSPM_CHECK_READY parameter */
            if(0u != currentRegisteredCallbacksNumber)
            {
                retVal = (cy_en_syspm_status_t) Cy_SysPm_ExecuteCallback(CY_SYSPM_ENTER_LP_MODE, CY_SYSPM_CHECK_READY, 0u);
            }

            /* The device (core) can switch into Low power mode only when
            *  all executed registered callback functions with CY_SYSPM_CHECK_READY
            *  parameter returned CY_SYSPM_SUCCESS.
            */
            if(retVal == CY_SYSPM_SUCCESS)
            {

                /* Call registered callback functions with CY_SYSPM_BEFORE_ENTER
                *  parameter. Return value is ignored.
                */
                interruptState = Cy_SysLib_EnterCriticalSection();
                if(0u != currentRegisteredCallbacksNumber)
                {
                    (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_ENTER_LP_MODE, CY_SYSPM_BEFORE_ENTER, 0u);
                }
                /* Configure low-power operating mode for Linear Core regulator */
                if(Cy_SysPm_LdoIsEnabled())
                {
                    SRSS->PWR_CTL |= (_VAL2FLD(SRSS_PWR_CTL_LINREG_LPMODE, 1u) |
                                      _VAL2FLD(SRSS_PWR_CTL_PORBOD_LPMODE, 1u) |
                                      _VAL2FLD(SRSS_PWR_CTL_BGREF_LPMODE, 1u));
                }
                else
                {
                    /* Configure low-power operating mode for SIMO Buck regulator */
                    SRSS->PWR_CTL |= (_VAL2FLD(SRSS_PWR_CTL_PORBOD_LPMODE, 1u) |
                                      _VAL2FLD(SRSS_PWR_CTL_BGREF_LPMODE, 1u));
                }

                /* This wait time allows the circuits to remove their dependence on
                *  Active mode circuits, such as the Active Reference
                */
                Cy_SysLib_DelayUs(CY_SYSPM_PWR_ACTIVE_TO_LP_WAIT_US);

                /* Disabling active reference */
                SRSS->PWR_CTL |= _VAL2FLD(SRSS_PWR_CTL_ACT_REF_DIS, 1u);

                Cy_SysLib_ExitCriticalSection(interruptState);

                /* Call registered callback functions with CY_SYSPM_AFTER_EXIT
                *  parameter. Return value is ignored.
                */
                if(0u != currentRegisteredCallbacksNumber)
                {
                    (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_ENTER_LP_MODE, CY_SYSPM_AFTER_EXIT, 0u);
                }
            }
            else
            {
                /* Execute callbacks functions with CY_SYSPM_CHECK_FAIL parameter to
                *  undo everything done in the CY_SYSPM_CHECK_READY callback. Return
                *  value is ignored.
                */
                (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_ENTER_LP_MODE, CY_SYSPM_CHECK_FAIL, (uint32_t) retVal);
                retVal = CY_SYSPM_FAIL;
            }
        }
        else
        {
            retVal = CY_SYSPM_FAIL;
        }
    }
    else
    {
        /* Do nothing as device is already in low-power mode. */
    }
    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_ExitLpMode
****************************************************************************//**
*
* Sets device out of Low Power mode.
*
* Returns device to the Active mode. In active power mode, operating current can
* be increased to the normal mode limit. Clock frequencies also can be increased
* to the normal mode limit. Refer to the device TRM for current and frequency
* limitations in Active power mode.
*
* Prior to exiting Low Power mode, all registered CY_SYSPM_EXIT_LP_MODE callbacks
* with CY_SYSPM_CHECK_READY parameter are called. This allows
* a driver to signal it is not ready to exit the
* Low Power mode. If any CY_SYSPM_EXIT_LP_MODE callbacks with
* CY_SYSPM_CHECK_READY parameter call returns CY_SYSPM_FAIL, the remaining
* CY_SYSPM_EXIT_LP_MODE callbacks with CY_SYSPM_CHECK_READY parameter calls are
* skipped. After a CY_SYSPM_FAIL, all of the CY_SYSPM_EXIT_LP_MODE callbacks with
* CY_SYSPM_CHECK_FAIL parameter are executed that correspond to the
* CY_SYSPM_EXIT_LP_MODE callbacks with CY_SYSPM_CHECK_READY parameter calls that
* occurred up to the point of failure. Active mode is not entered and the
* Cy_SysPm_ExitLpMode API returns CY_SYSPM_FAIL.
*
* If all CY_SYSPM_EXIT_LP_MODE callbacks with CY_SYSPM_CHECK_READY calls return
* CY_SYSPM_SUCCESS then all CY_SYSPM_EXIT_LP_MODE callbacks with
* CY_SYSPM_CHECK_FAIL parameter calls are skipped and all CY_SYSPM_EXIT_LP_MODE
* callbacks CY_SYSPM_BEFORE_ENTER parameter calls are executed allowing
* peripherals to prepare for Active mode. Low Power mode is then exited.
*
* After exiting Low Power mode all of the registered callbacks are executed
* CY_SYSPM_EXIT_LP_MODE callbacks with CY_SYSPM_AFTER_EXIT parameter to complete
* preparing peripherals for Active mode operation. The Cy_SysPm_ExitLpMode API
* returns CY_SYSPM_SUCCESS. No CY_SYSPM_EXIT_LP_MODE callbacks with
* CY_SYSPM_BEFORE_ENTER or CY_SYSPM_AFTER_EXIT parameter are executed if Low
* Power mode is not exited.
*
* \note The callbacks are not executed if device is already not in a low power
* mode.
*
* \return
* CY_SYSPM_SUCCESS - Exited from LPActive power mode, or device is already in
* Active power. <br>
* CY_SYSPM_FAIL - the exit from LPActive mode not done,
* see \ref cy_en_syspm_status_t.
*
* \warning This function blocks as it waits until Active Reference is ready
* to enter into normal mode.
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_ExitLpMode(void)
{
    uint32_t interruptState;
    uint32_t timeOut = CY_SYSPM_WAIT_DELAY_TRYES;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;

    /* Check if device is in Low Power mode */
    if(0u != (_FLD2VAL(SRSS_PWR_CTL_ACT_REF_DIS, SRSS->PWR_CTL)))
    {
        /* Call registered callback functions with CY_SYSPM_CHECK_READY parameter */
        if(0u != currentRegisteredCallbacksNumber)
        {
            retVal = (cy_en_syspm_status_t) Cy_SysPm_ExecuteCallback(CY_SYSPM_EXIT_LP_MODE, CY_SYSPM_CHECK_READY, 0u);
        }

        /* The device (core) can switch into Low power mode only in condition
        *  that all executed registered callback functions with CY_SYSPM_CHECK_READY
        *  parameter returned CY_SYSPM_SUCCESS
        */
        if(retVal == CY_SYSPM_SUCCESS)
        {
            /* Call registered callback functions with CY_SYSPM_BEFORE_ENTER
            *  parameter. Return value is ignored.
            */
            interruptState = Cy_SysLib_EnterCriticalSection();
            if(0u != currentRegisteredCallbacksNumber)
            {
                (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_EXIT_LP_MODE, CY_SYSPM_BEFORE_ENTER, 0u);
            }

            /* Set normal operation mode for Linear Core regulator in condition
            *  that it is enabled
            */
            if(Cy_SysPm_LdoIsEnabled())
            {
                SRSS->PWR_CTL &= ((uint32_t)~(_VAL2FLD(SRSS_PWR_CTL_LINREG_LPMODE, 1u)));
            }

            /* Configure normal operating mode for POR/BOD circuits and for
            *  Bandgap Voltage and Current References.
            */
            SRSS->PWR_CTL &= ((uint32_t)~(_VAL2FLD(SRSS_PWR_CTL_PORBOD_LPMODE, 1u) |
                                          _VAL2FLD(SRSS_PWR_CTL_ACT_REF_DIS, 1u) |
                                          _VAL2FLD(SRSS_PWR_CTL_VREFBUF_LPMODE, 1u)));

            /* This wait time allows Active Reference to settle */
            Cy_SysLib_DelayUs(CY_SYSPM_PWR_LP_TO_ACTIVE_WAIT_BEFORE_US);

            while((0u == _FLD2VAL(SRSS_PWR_CTL_ACT_REF_OK, SRSS->PWR_CTL)) && (0u != timeOut))
            {
                timeOut--;
            }

            if(0u == timeOut)
            {
                retVal = CY_SYSPM_TIMEOUT;
            }

            /* Configure the normal operation mode */
            SRSS->PWR_CTL &= (uint32_t) (~_VAL2FLD(SRSS_PWR_CTL_BGREF_LPMODE, 1u));

            /* This wait time allows the settle Active Reference */
            Cy_SysLib_DelayUs(CY_SYSPM_PWR_LP_TO_ACTIVE_WAIT_AFTER_US);

            Cy_SysLib_ExitCriticalSection(interruptState);

            /* Call registered callback functions with CY_SYSPM_AFTER_EXIT
            *  parameter. Return value is ignored.
            */
            if(0u != currentRegisteredCallbacksNumber)
            {
                (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_EXIT_LP_MODE, CY_SYSPM_AFTER_EXIT, 0u);
            }
        }
        else
        {
            /* Execute callback functions with CY_SYSPM_CHECK_FAIL parameter to
            *  undo everything done in the CY_SYSPM_CHECK_READY callback. Return
            *  value is ignored.
            */
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_EXIT_LP_MODE, CY_SYSPM_CHECK_FAIL, (uint32_t) retVal);
            retVal = CY_SYSPM_FAIL;
        }
    }
    else
    {
        /* Do nothing as device is already in active power mode. */
    }
    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_SleepOnExit
****************************************************************************//**
*
* Set Sleep on the Exit mode.
*
* This API gives a possibility to keep the system in a sleep mode at all times
* and only wakeup to run a single interrupt and then immediately go back to
* sleep.
*
* \param
* enable true - enable Sleep on the Exit mode <br> false - disable
* Sleep on the Exit mode.
*
*******************************************************************************/
void Cy_SysPm_SleepOnExit(bool enable)
{
    uint32_t interruptState;
    interruptState = Cy_SysLib_EnterCriticalSection();

    if(enable)
    {
        /* Enable SleepOnExit mode */
        SCB->SCR |= _VAL2FLD(SCB_SCR_SLEEPONEXIT, 1u);
    }
    else
    {
        /* Disable SleepOnExit mode */
        SCB->SCR &= ((uint32_t) ~(_VAL2FLD(SCB_SCR_SLEEPONEXIT, 1u)));
    }
    Cy_SysLib_ExitCriticalSection(interruptState);
}

#if(0u != SRSS_SIMOBUCK_PRESENT)
    /*******************************************************************************
    * Function Name: Cy_SysPm_SwitchToSimoBuck
    ****************************************************************************//**
    *
    * Switch the power supply regulator to Single-Input Multiple Output (SIMO)
    * Buck regulator instead of Linear Regulator. The SIMO Buck core regulator
    * can supply Vccd with higher efficiency than the Linear core regulator (LDO)
    * under some conditions, such as high external supply voltages.
    *
    * Before change from LDO to SIMO buck ensure that:
    * * Circuit board has connected Vccbuck1 to Vccd and also populated the
    * necessary external components for SIMO Buck regulator, including an
    * inductor and a capacitor for each output.
    * Refer to the device TRM for more details.
    *
    * * Operating frequencies are reduced so it is within the static timing
    * closure limits at the new frequency.
    *
    * * Current consumption on the core is reduced to meet 0.9 V (nominal) output
    * voltage. The main reason is that while changing from LDO to SIMO Buck,
    * the switch happens on 0.9 V (nominal).
    *
    * * Appropriate Wait states values are set for Flash using the
    * Cy_SysLib_SetWaitStates() function. Refer to the System Library (syslib) driver
    * for more details.
    *
    * In general, use the next sequence to obtain SIMO Buck 0.9 V (nominal):
    * 1) Set appropriate wait states for ULP by calling
    * Cy_SysLib_SetWaitStates(true, hfClkFreqMz), where hfClkFreqMz -
    * desired frequency of HfClk0 in MHz.
    *
    * 2) Set 0.9 V (nominal) for LDO regulator.
    *
    * 3) Switch the regulators by calling the Cy_SysPm_SwitchToSimoBuck() function.
    *
    * To obtain SIMO Buck 1.1 V (nominal) use next sequence:
    * 1) Set appropriate wait states for ULP by calling
    * Cy_SysLib_SetWaitStates(true, hfClkFreqMz), where hfClkFreqMz -
    * desired frequency of HfClk0 in MHz
    *
    * 2) Set 0.9 V (nominal) for LDO regulator
    *
    * 3) Switch the regulators by calling the Cy_SysPm_SwitchToSimoBuck() function
    *
    * 4) Change SIMO Buck output voltage by calling the
    * Cy_SysPm_SimoBuckSetVoltage1(CY_SYSPM_SIMO_BUCK_OUT1_VOLTAGE_1_1V)
    *
    * 5) Set appropriate wait states for LP by calling
    * Cy_SysLib_SetWaitStates(false, hfClkFreqMz), where hfClkFreqMz -
    * desired frequency of HfClk0 in MHz
    *
    * \warning System does not have a way to know when Vccd is settled at actual
    * 0.9 V or 1.1 V for transition, as this voltage depends on conditions
    * including load current. The firmware works with
    * nominal 0.9 V and 1.1 V voltages.
    *
    * \warning Please be aware that the Flash access time when core
    * output voltage is 0.9 V (nominal) is longer that using 1.1 V (nominal).
    * Therefore the clock to the Flash should be decreased. Use the
    * Cy_SysLib_SetWaitStates() function to set appropriate wait state values
    * for Flash. Call the Cy_SysLib_SetWaitStates() function before
    * Cy_SysPm_SwitchToSimoBuck() if you want to change voltage
    * into 0.9 V (nominal). Call the Cy_SysLib_SetWaitStates() function after
    * Cy_SysPm_SwitchToSimoBuck() if you change voltage from 0.9 V (nominal)
    * to 1.1 V (nominal).
    *
    * \warning There is no way to back to Linear Regulator after SIMO Buck
    * regulator supply core.
    *
    * In addition if 0.9 V (nominal) output is set, the Flash works
    * in Read only operation.
    *
    * If 1.1 V (nominal) output is set, the Flash works in Read and Write
    * operation and clock frequencies may be increased up to the static
    * timing closure limits for the new voltage.
    * Refer to the device TRM for more details.
    *
    * \note
    * Function is applicable for devices with SIMO Buck Regulator.
    *
    * Function does not have effect if SIMO Buck regulator is already enabled.
    *
    *******************************************************************************/
    void Cy_SysPm_SwitchToSimoBuck(void)
    {
        /* Check is the SIMO Buck regulator already enabled */
        if(!Cy_SysPm_SimoBuckIsEnabled())
        {
            /* Disable the DeepSleep, nWell, and Retention regulators */
            SRSS->PWR_CTL |= (_VAL2FLD(SRSS_PWR_CTL_DPSLP_REG_DIS, 1u) |
                              _VAL2FLD(SRSS_PWR_CTL_RET_REG_DIS, 1u) |
                              _VAL2FLD(SRSS_PWR_CTL_NWELL_REG_DIS, 1u));

            /* Configure SIMO Buck regulator */
            SRSS->PWR_BUCK_CTL = _CLR_SET_FLD32U((SRSS->PWR_BUCK_CTL),
                                                  SRSS_PWR_BUCK_CTL_BUCK_OUT1_SEL,
                                                 (uint32_t) CY_SYSPM_SIMO_BUCK_OUT1_VOLTAGE_0_9V);

            SRSS->PWR_BUCK_CTL |= _VAL2FLD(SRSS_PWR_BUCK_CTL_BUCK_EN, 1u);
            SRSS->PWR_BUCK_CTL |= _VAL2FLD(SRSS_PWR_BUCK_CTL_BUCK_OUT1_EN, 1u);

            /* Wait for SIMO Buck is stable */
            Cy_SysLib_DelayUs(CY_SYSPM_SIMO_BUCK_CORE_SUPPLY_STABLE_US);

            /* Disable linear regulator LDO as Vbuckout1 and LDO are shorted */
            SRSS->PWR_CTL |= _VAL2FLD(SRSS_PWR_CTL_LINREG_DIS, 1u);
        }
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_SimoBuckSetVoltage1
    ****************************************************************************//**
    *
    * Set the output 1 voltage for an SIMO Buck regulator that can supply cores.
    * This output can supply cores instead of the Linear regulator.
    *
    * When changing from a higher voltage to a lower voltage make sure that:
    * * Operating frequency is reduced so it is within the static timing closure
    * limits at the new frequency.
    *
    * * Appropriate Wait states values are set for Flash using the
    * Cy_SysLib_SetWaitStates() function. Refer to the System Library (syslib) driver
    * for more details.
    *
    * \warning Please be aware that the Flash access time when core
    * output voltage is 0.9 V (nominal) is longer that using 1.1 V (nominal).
    * Therefore, the clock to the Flash should be decreased.
    *
    * In addition if 0.9 V (nominal) output is set, the Flash works in
    * Read-only operation.
    *
    * When changing from a lower voltage to a higher voltage, make sure that
    * appropriate wait states values are set for Flash using the
    * Cy_SysLib_SetWaitStates() function. Refer to the System Library (syslib) driver
    * for more details.
    *
    * If 1.1 V (nominal) output is set, the Flash works in Read and Write
    * operations and clock frequency may now be increased up to the static
    * timing closure limits for the new voltage.
    *
    * \warning System does not have a way to know when Vccd is settled at actual
    * 0.9 V or 1.1 V for transition, as this voltage depends on conditions
    * including load current. The firmware works with nominal
    * 0.9 V and 1.1 V voltages.
    *
    * \param
    * voltage Desired output 1 regulator voltage (Vccbuck1).
    * See \ref cy_en_syspm_simo_buck_voltage1_t
    *
    *******************************************************************************/
    void Cy_SysPm_SimoBuckSetVoltage1(cy_en_syspm_simo_buck_voltage1_t voltage)
    {
        /* Check is the required voltage is equal to current voltage */
        if(voltage != Cy_SysPm_SimoBuckGetVoltage1())
        {
            /* Set analog signal bit for flash before voltage is changed
            * from 1.1V to 0.9V
            */
            if(CY_SYSPM_SIMO_BUCK_OUT1_VOLTAGE_0_9V == voltage)
            {
                Cy_SysPm_SetVoltageBitForFlash(true);
            }

            /* The system may continue operating while the voltage on Vccd
            *  discharges to the new voltage. The time it takes to reach the
            *  new voltage depends on conditions, including the load current on
            *  Vccd and the external capacitor size.
            */
            SRSS->PWR_BUCK_CTL =
            _CLR_SET_FLD32U((SRSS->PWR_BUCK_CTL), SRSS_PWR_BUCK_CTL_BUCK_OUT1_SEL, (uint32_t) voltage);

            /* Delay for the supply to stabilize at the new voltage is required
            *  when changing from a lower voltage to a higher voltage.
            */
            if(CY_SYSPM_SIMO_BUCK_OUT1_VOLTAGE_1_1V == voltage)
            {
                Cy_SysLib_DelayUs(CY_SYSPM_SIMO_BUCK_IS_STABLE_US);

                /* Set analog signal bit for flash before voltage is changed
                 * from 0.9 V to 1.1 V.
                 */
                Cy_SysPm_SetVoltageBitForFlash(false);
            }
        }
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_SimoBuckGetVoltage1
    ****************************************************************************//**
    *
    * Get current nominal output 1 voltage (Vccbuck1) of the SIMO Buck regulator.
    *
    * \note The actual device output 1 voltage (Vccbuck1) can be different from the
    * actual voltage as actual voltage value depends on conditions including
    * load current.
    *
    * \note The actual device output 1 voltage (Vccbuck1) can be different from the
    * nominal voltage as actual voltage value depends on conditions including
    * load current.
    *
    * \return
    * Nominal output voltage 1 (Vccbuck1) of the SIMO Buck regulator.
    * See \ref cy_en_syspm_simo_buck_voltage1_t.
    *
    *******************************************************************************/
    cy_en_syspm_simo_buck_voltage1_t Cy_SysPm_SimoBuckGetVoltage1(void)
    {
        uint32_t retVal;
        retVal = _FLD2VAL(SRSS_PWR_BUCK_CTL_BUCK_OUT1_SEL, SRSS->PWR_BUCK_CTL);

        return((cy_en_syspm_simo_buck_voltage1_t) retVal);
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_EnableVoltage2
    ****************************************************************************//**
    *
    * Enable output 2 voltage (Vbuckrf) of the SIMO Buck regulator. The output 2
    * voltage (Vbuckrf) of the SIMO Buck regulator is used to supply the BLE HW block.
    *
    * The SIMO Buck regulator should be enabled before the Cy_SysPm_EnableVoltage2()
    * function call.
    *
    * \note The function does not have effect if SIMO Buck regulator is disabled.
    *
    *******************************************************************************/
    void Cy_SysPm_EnableVoltage2(void)
    {
        if(!Cy_SysPm_SimoBuckIsEnabled())
        {
            /* Enable SIMO Buck output 2 */
            SRSS->PWR_BUCK_CTL2 |= _VAL2FLD(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_EN, 1u);

            /* Wait until output is stable */
            Cy_SysLib_DelayUs(CY_SYSPM_SIMO_BUCK_BLE_SUPPLY_STABLE_US);
        }
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_EnableVoltage2
    ****************************************************************************//**
    *
    * Disable output 2 voltage (Vbuckrf) of the SIMO Buck regulator. The output 2
    * voltage (Vbuckrf) of the SIMO Buck regulator is used to supply the BLE HW block.
    *
    *******************************************************************************/
    void Cy_SysPm_DisableVoltage2(void)
    {
        /* Disable Vbuck2 output */
        SRSS->PWR_BUCK_CTL2 &= ((uint32_t) ~ (_VAL2FLD(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_EN, 1u)));
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_SimoBuckSetVoltage2
    ****************************************************************************//**
    *
    * Set current voltage 2 voltage of the SIMO Buck regulator.
    *
    * \param
    * voltage voltage of Linear regulator.
    * See \ref cy_en_syspm_simo_buck_voltage2_t.
    *
    *******************************************************************************/
    void Cy_SysPm_SimoBuckSetVoltage2(cy_en_syspm_simo_buck_voltage2_t voltage)
    {
        if(voltage != Cy_SysPm_SimoBuckGetVoltage2())
        {
            SRSS->PWR_BUCK_CTL2 =
            _CLR_SET_FLD32U((SRSS->PWR_BUCK_CTL2), SRSS_PWR_BUCK_CTL2_BUCK_OUT2_SEL, (uint32_t) voltage);

            Cy_SysLib_DelayUs(CY_SYSPM_SIMO_BUCK_IS_STABLE_US);
        }

    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_SimoBuckGetVoltage2
    ****************************************************************************//**
    *
    * Get current output 2 voltage (Vbuckrf) of the SIMO Buck regulator.
    *
    * \note The actual device output 2 voltage (Vbuckrf) can be different from the
    * nominal voltage as actual voltage value depends on conditions including
    * load current.
    *
    * \return
    * Nominal output voltage of the SIMO Buck regulator output 2 voltage (Vbuckrf).
    * See \ref cy_en_syspm_simo_buck_voltage2_t.
    *
    *******************************************************************************/
    cy_en_syspm_simo_buck_voltage2_t Cy_SysPm_SimoBuckGetVoltage2(void)
    {
        uint32_t retVal;
        retVal = _FLD2VAL(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_SEL, SRSS->PWR_BUCK_CTL2);

        return((cy_en_syspm_simo_buck_voltage2_t) retVal);
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_SimoBuckOutputIsEnabled
    ****************************************************************************//**
    *
    * Get current output status of the SIMO Buck outputs.
    *
    * \param
    * output SIMO Buck regulator output. See \ref cy_en_syspm_buck_out_t.
    *
    * \return
    * current state of requested output. True if requested output is enabled.
    * False if requested output is disabled.
    *
    *******************************************************************************/
    bool Cy_SysPm_SimoBuckOutputIsEnabled(cy_en_syspm_buck_out_t output)
    {
        bool retVal;
        if(output == CY_SYSPM_BUCK_VBUCK_1)
        {
            retVal = ((0u != _FLD2VAL(SRSS_PWR_BUCK_CTL_BUCK_OUT1_EN, SRSS->PWR_BUCK_CTL)) ? true : false);
        }
        else
        {
            if((0u != _FLD2VAL(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_HW_SEL, SRSS->PWR_BUCK_CTL2)) ||
               (0u != _FLD2VAL(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_EN, SRSS->PWR_BUCK_CTL2)))
            {
                retVal = true;
            }
            else
            {
                retVal = false;
            }
        }
        return(retVal);
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_SimoBuckIsEnabled
    ****************************************************************************//**
    *
    * Get current status of SIMO Buck regulator.
    *
    * \return
    * current state of the SIMO Buck regulator. True if SIMO Buck regulator is enabled,
    * false if it is disabled
    *
    *******************************************************************************/
    bool Cy_SysPm_SimoBuckIsEnabled(void)
    {
        return((0u !=_FLD2VAL(SRSS_PWR_BUCK_CTL_BUCK_EN, SRSS->PWR_BUCK_CTL)) ? true : false);
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_SimoBuckSetHwControl
    ****************************************************************************//**
    *
    * Set hardware control for SIMO Buck output 2 (Vbuckrf)
    *
    * Hardware control for Vbuckrf output. When this bit is set, the value in
    * BUCK_OUT2_EN is ignored and a hardware signal is used instead. If the
    * product has supporting hardware, it can directly control the enable signal
    * for Vbuckrf.
    *
    * \param
    * output enable/disable SIMO Buck regulator outputs,
    * see \ref cy_en_syspm_buck_out.
    *
    *******************************************************************************/
    void Cy_SysPm_SimoBuckSetHwControl(bool hwControl)
    {
        if(hwControl)
        {
            SRSS->PWR_BUCK_CTL2 |= _VAL2FLD(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_HW_SEL, 1u);
        }
        else
        {
            SRSS->PWR_BUCK_CTL2 &= ((uint32_t)~(_VAL2FLD(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_HW_SEL, 1u)));
        }
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_SimoBuckGetHwControl
    ****************************************************************************//**
    *
    * Get hardware control for Vbuckrf
    *
    * Hardware control for Vbuckrf output. When this bit is set, the value in
    * BUCK_OUT2_EN is ignored and a hardware signal is used instead.  If the
    * product has supporting hardware, it can directly control the enable signal
    * for Vbuckrf.
    *
    * \return
    * true if HW control is set, false if FW control is set for SIMO Buck output 2
    *
    *******************************************************************************/
    bool Cy_SysPm_SimoBuckGetHwControl(void)
    {
        return((0u != _FLD2VAL(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_HW_SEL, SRSS->PWR_BUCK_CTL2)) ? true : false);
    }

#endif /* (0u != SRSS_SIMOBUCK_PRESENT) */


/*******************************************************************************
* Function Name: Cy_SysPm_LdoSetVoltage
****************************************************************************//**
*
* Set the voltage on linear regulator (LDO).
* When changing from a higher voltage to a lower voltage make sure that:
* * Operating frequencies are reduced so these are within the static timing
* closure limits at the new frequency.
*
* * Current consumption on core is reduced to meet new output voltage.
*
* * Appropriate Wait states values are set for Flash using the
* Cy_SysLib_SetWaitStates() function. Refer to the System Library (syslib) driver
* for more details.
*
* \warning Please be aware that the Flash access time in condition that core
* output voltage is 0.9 V (nominal) is longer that using 1.1 V (nominal).
* Therefore the clock to the Flash should be decreased.
*
* In addition if 0.9 V (nominal) output is set, the Flash works in Read
* only operation.
*
* When changing from a lower voltage to a higher voltage make sure that
* appropriate Wait states values is set for Flash using the
* Cy_SysLib_SetWaitStates() function. Refer to System Library (syslib) driver
* for more details.
*
* If 1.1 V (nominal) output is set, the Flash works in Read and Write
* operations. Clock frequency may now be increased up to the static
* timing closure limits for the new voltage.
*
* \note The actual device Vccd voltage can be different from the
* nominal voltage as actual voltage value depends on conditions including
* load current.
*
* \param
* voltage Desired output regulator voltage.
* See \ref cy_en_syspm_ldo_voltage_t voltage
*
*******************************************************************************/
void Cy_SysPm_LdoSetVoltage(cy_en_syspm_ldo_voltage_t voltage)
{
    if(voltage != Cy_SysPm_LdoGetVoltage())
    {
        /* Set analog signal bit for flash before voltage is changed
        * from 1.1V to 0.9V
        */
        if(CY_SYSPM_LDO_VOLTAGE_0_9V == voltage)
        {
            Cy_SysPm_SetVoltageBitForFlash(true);
        }

        /* The system may continue operating while the voltage on Vccd
        *  discharges to the new voltage. The time it takes to reach the
        *  new voltage depends on conditions, including the load current on
        *  Vccd and the external capacitor size
        */
        SRSS->PWR_TRIM_PWRSYS_CTL =
        _CLR_SET_FLD32U((SRSS->PWR_TRIM_PWRSYS_CTL), SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, voltage);

        if(CY_SYSPM_LDO_VOLTAGE_1_1V == voltage)
        {
            /* Delay for the supply to stabilize at the new higher voltage */
            Cy_SysLib_DelayUs(CY_SYSPM_LDO_IS_STABLE_US);

            /* Set analog signal bit to flash macro register after output voltage is 1.1 V */
            Cy_SysPm_SetVoltageBitForFlash(false);
        }
    }
}


/*******************************************************************************
* Function Name: Cy_SysPm_LdoGetVoltage
****************************************************************************//**
*
* Get current output voltage value of Linear regulator.
*
* \note The actual device Vccd voltage can be different from the
* nominal voltage as actual voltage value depends on conditions including
* load current.
*
* \return
* Nominal output voltage of the Linear regulator.
* See \ref cy_en_syspm_ldo_voltage_t.
*
*******************************************************************************/
cy_en_syspm_ldo_voltage_t Cy_SysPm_LdoGetVoltage(void)
{
    uint32_t retVal;
    retVal = _FLD2VAL(SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, SRSS->PWR_TRIM_PWRSYS_CTL);

    return((cy_en_syspm_ldo_voltage_t) retVal);
}


/*******************************************************************************
* Function Name: Cy_SysPm_LdoIsEnabled
****************************************************************************//**
*
* Read the current status of Linear regulator.
*
* \return
* True means the Linear regulator is enabled. False means it is if disabled
*
*******************************************************************************/
bool Cy_SysPm_LdoIsEnabled(void)
{
    return((0u != _FLD2VAL(SRSS_PWR_CTL_LINREG_DIS, SRSS->PWR_CTL)) ? false : true);
}


/*******************************************************************************
* Function Name: Cy_SysPm_WakeupPinLevel
****************************************************************************//**
*
* This function enables/disables wakeup pins and configure their active
* (high or low) level.
*
* A wake up from the Hibernate mode can be performed by toggling the wakeup pins,
* causing a normal boot procedure to occur. A wakeup is supported by up to
* two pins with programmable polarity. These pins may be connected to IO pins
* or on-chip peripherals under some conditions.
* Setting the wakeup pin to this level will cause a wakeup from the Hibernate
* mode. The wakeup pins are active-low by default.
*
* Configure only one pin at one API call.
*
* \param
* pinNum Selects the wakeup pin. The valid range is [0-1]. The number of the pin.
*
* \param enPinLevel Selects polarity. See \ref cy_en_syspm_pinstate_t.
*
* pinNum value corresponds to an appropriate pin. For example, value 1
* corresponds to second wakeup pin. For information about wakeup pin
* assignment in specific families devices, refer to the appropriate device
* TRM.
*
*******************************************************************************/
void Cy_SysPm_WakeupPinLevel(uint32_t pinNum, cy_en_syspm_pinstate_t enPinLevel)
{
    uint32_t interruptState;
    interruptState = Cy_SysLib_EnterCriticalSection();

    pinNum &= CY_SYSPM_PWR_MASK_HIBPIN_PIN_MASK;

    if (CY_SYSPM_PIN_HIGH_LEVEL == enPinLevel)
    {
        /* Set high level of selected wakeup pin */
        SRSS->PWR_HIBERNATE |=
        _VAL2FLD(SRSS_PWR_HIBERNATE_MASK_HIBPIN, (CY_SYSPM_WAKEUP_PIN_BIT << pinNum)) |
        _VAL2FLD(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, (CY_SYSPM_WAKEUP_PIN_BIT << pinNum));
    }
    else if(CY_SYSPM_PIN_LOW_LEVEL == enPinLevel)
    {
        /* Set low level of selected wakeup pin */
        SRSS->PWR_HIBERNATE |=
        _VAL2FLD(SRSS_PWR_HIBERNATE_MASK_HIBPIN, (CY_SYSPM_WAKEUP_PIN_BIT << pinNum));

        SRSS->PWR_HIBERNATE &=
        ((uint32_t) ~_VAL2FLD(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, (CY_SYSPM_WAKEUP_PIN_BIT << pinNum)));
    }
    else
    {
        /* Disable selected wakeup pin */
        SRSS->PWR_HIBERNATE &=
        ((uint32_t) ~_VAL2FLD(SRSS_PWR_HIBERNATE_MASK_HIBPIN, (CY_SYSPM_WAKEUP_PIN_BIT << pinNum)));
    }
    Cy_SysLib_ExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Cy_SysPm_WakeupLpCompLevel
****************************************************************************//**
*
* This function is configure wakeup low-power comparators (LPComp) and
* assigns their active (high or low) level.
*
* A wake up from the Hibernate mode can be performed by toggling the LPComps,
* causing a normal boot procedure to occur. A wakeup is supported by up to
* two LPComps with programmable polarity. These LPComp may be connected to IO
* pins or on-chip peripherals under some conditions.
* Setting the LPComp to this level will cause a wakeup from the Hibernate
* mode. The wakeup LPComp are active-low by default.
*
* Configure one of the LPComps at one API call.
*
* \note The Low-power comparators should be configured and enabled before
* switching into the Hibernate low power mode. Refer to LPComp
* driver description for more details.
*
* \param
* lpCompNum Selects the LPComp. The valid range is [0-1]. The number of the LPComp.
*
* \param enLpCompLevel Selects polarity. See \ref cy_en_syspm_lpcomp_state_t.
*
* lpCompNum value corresponds to an appropriate LPComp. For example, value 1
* corresponds to second LP Comparator. For information about wakeup LPComp
* assignment in specific families devices, refer to the appropriate device
* TRM.
*
*******************************************************************************/
void Cy_SysPm_WakeupLpCompLevel(uint32_t lpCompNum, cy_en_syspm_lpcomp_state_t enLpCompLevel)
{
    uint32_t interruptState;
    interruptState = Cy_SysLib_EnterCriticalSection();

    lpCompNum &= CY_SYSPM_LPCOMPARATOR_MASK;

    if(CY_SYSPM_LPCOMP_HIGH_LEVEL == enLpCompLevel)
    {
        /* Set high level of selected wakeup pin */
        SRSS->PWR_HIBERNATE |=
        _VAL2FLD(SRSS_PWR_HIBERNATE_MASK_HIBPIN, CY_SYSPM_LPCOMPARATOR_BIT << lpCompNum) |
        _VAL2FLD(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, (CY_SYSPM_LPCOMPARATOR_BIT << lpCompNum));
    }
    else if(CY_SYSPM_LPCOMP_LOW_LEVEL == enLpCompLevel)
    {
        /* Set low level of selected wakeup pin */
        SRSS->PWR_HIBERNATE |=
        _VAL2FLD(SRSS_PWR_HIBERNATE_MASK_HIBPIN, (CY_SYSPM_LPCOMPARATOR_BIT << lpCompNum));

        SRSS->PWR_HIBERNATE &=
        ((uint32_t) ~_VAL2FLD(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, (CY_SYSPM_LPCOMPARATOR_BIT << lpCompNum)));
    }
    else
    {
        /* Disable selected wakeup pin */
        SRSS->PWR_HIBERNATE &=
        ((uint32_t) ~_VAL2FLD(SRSS_PWR_HIBERNATE_MASK_HIBPIN, (CY_SYSPM_LPCOMPARATOR_BIT << lpCompNum)));
    }
    Cy_SysLib_ExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Cy_SysPm_WakeupWdt
****************************************************************************//**
*
* This function enables/disables the Watchdog (WDT) to wake up the device from
* Hibernate power mode after WDT interrupt.
*
* \note The WDT should be configured and enabled before entering into Hibernate
* power mode.
*
* A wakeup from the Hibernate mode can be performed by the Watchdog interrupt,
* causing a normal boot procedure to occur.
* The wakeup is performed by a WDT interrupt and a normal boot procedure
* after device reset. Device can wake up from Hibernate after WDT device reset,
* if WDT was not configured to wake up the device on its interrupt and WDT
* was enabled.
* Refer to Watchdog Timer (WDT) driver description for more details.
*
* \param
* configureWdt true if WDT interrupt should wake up device from Hibernate and <br>
* false if WDT interrupt should not wake up device from Hibernate.
*
*******************************************************************************/
void Cy_SysPm_WakeupWdt(bool configureWdt)
{
    if(configureWdt)
    {
        SRSS->PWR_HIBERNATE |= _VAL2FLD(SRSS_PWR_HIBERNATE_MASK_HIBWDT, 1u);
    }
    else
    {
        SRSS->PWR_HIBERNATE &= ((uint32_t) (~_VAL2FLD(SRSS_PWR_HIBERNATE_MASK_HIBWDT, 1u)));
    }
}


/*******************************************************************************
* Function Name: Cy_SysPm_WakeupRtc
****************************************************************************//**
*
* This function enables/disables the alarms from RTC to wake up the device from
* Hibernate power mode after RTC alarm assert.
*
* \note The RTC alarm should be configured and enabled before entering into
* Hibernate power mode.
*
* A wakeup from the Hibernate mode can be performed by the RTC alarm,
* causing a normal boot procedure to occur.
* The wakeup is performed by the RTC alarm and a normal boot procedure
* after device reset.
* Refer to Real-Time Clock (RTC) driver description for more details.
*
* \param
* configureRtc true if RTC alarm should wake up device from Hibernate <br>
* false if RTC alarm should not wake up device from Hibernate
*
*******************************************************************************/
void Cy_SysPm_WakeupRtc(bool configureRtc)
{
    if(configureRtc)
    {
        SRSS->PWR_HIBERNATE |= _VAL2FLD(SRSS_PWR_HIBERNATE_MASK_HIBALARM, 1u);
    }
    else
    {
        SRSS->PWR_HIBERNATE &= ((uint32_t) ~_VAL2FLD(SRSS_PWR_HIBERNATE_MASK_HIBALARM, 1u));
    }
}



/*******************************************************************************
* Function Name: Cy_SysPm_RegisterCallback
****************************************************************************//**
*
* Registers a new syspm callback.
*
* \note The registered callbacks will be executed in order of how callbacks were
* registered: the first registered callback will be always the first executed. And the
* last registered callback will be executed as the last callback.
*
* A callback is a function called after an event in a driver or
* middleware-module has occurred. The handler callback API will be executed if
* specific event occurs. See \ref cy_stc_syspm_callback_t.
*
* \param
* handler syspm callback structure. See \ref cy_stc_syspm_callback_t.
*
* \return
* True: callback was registered; <br>
* False: callback was not registered.
*
* It is allowed to register up to 32 callbacks.
*
*******************************************************************************/
bool Cy_SysPm_RegisterCallback(cy_stc_syspm_callback_t* handler)
{
    uint32_t interruptState;
    volatile bool regCallbackReturnValue = true;
    cy_stc_syspm_callback_t* currentCallback = callbackRoot;

    interruptState = Cy_SysLib_EnterCriticalSection();

    /* Check if the maximum callbacks were registered */
    if((CY_SYSPM_PWR_MAX_CALLBACKS_NUMBER > currentRegisteredCallbacksNumber) &&
    (handler->Callback != NULL))
    {
        /* Check if was at least one callback registered */
        if(callbackRoot == NULL)
        {
            callbackRoot = handler;
            handler->nextItm = NULL;

            currentRegisteredCallbacksNumber++;
        }
        else
        {
            /* Check was the required callback already registered */
            while(regCallbackReturnValue && (currentCallback->nextItm != NULL))
            {
                if((currentCallback == handler) && (currentCallback->base == handler->base))
                {
                    /* Condition to switch-off from checking loop and return false as requested callback is
                    * already registered
                    */
                    regCallbackReturnValue = false;
                }

                currentCallback = currentCallback->nextItm;
            }

            /* Check last element of callback list */
            if((currentCallback == handler) && (currentCallback->base == handler->base))
            {
                /* Condition to switch of from checking and return false */
                regCallbackReturnValue = false;
            }

            if(false != regCallbackReturnValue)
            {
                /* Condition when last registered callback was found */
                currentCallback->nextItm = handler;
                handler->nextItm = NULL;

                currentRegisteredCallbacksNumber++;
            }
        }
    }
    else
    {
        regCallbackReturnValue = false;
    }
    Cy_SysLib_ExitCriticalSection(interruptState);

    return(regCallbackReturnValue);
}


/*******************************************************************************
* Function Name: Cy_SysPm_UnregisterCallback
****************************************************************************//**
*
* Unregister a callback.
*
* The registered callback can be unregistered. Otherwise, false will be
* returned.
*
* \param
* handler the list item and callback handler. See
* \ref cy_stc_syspm_callback_t.
*
* \return
* True: on success <br>
* False: if it was not unregistered.
*
*******************************************************************************/
bool Cy_SysPm_UnregisterCallback(cy_stc_syspm_callback_t* handler)
{
    uint32_t interruptState;
    volatile bool unRegCallbackValue;
    volatile bool unRegCallbackCheckStatus;

    cy_stc_syspm_callback_t* currentCallback = callbackRoot;
    cy_stc_syspm_callback_t* nextCallback;

    unRegCallbackCheckStatus = true;

    interruptState = Cy_SysLib_EnterCriticalSection();

    /* Check if was at least one callback registered */
    if(0u != currentRegisteredCallbacksNumber)
    {
        if((callbackRoot == handler) && (callbackRoot->base == handler->base))
        {
            callbackRoot = callbackRoot->nextItm;
            unRegCallbackCheckStatus = false;
            unRegCallbackValue = true;
            currentRegisteredCallbacksNumber--;
        }

        while(unRegCallbackCheckStatus && (currentCallback != NULL))
        {
            /* Searching callback by handler */
            if(currentCallback->nextItm == handler)
            {
                nextCallback = currentCallback->nextItm;

                /* Check is it required callback by it address */
                if((nextCallback->base == handler->base) && (currentCallback->nextItm == handler))
                {
                    currentCallback->nextItm = handler->nextItm;
                    unRegCallbackCheckStatus = false;
                    unRegCallbackValue = true;
                    currentRegisteredCallbacksNumber--;
                }
            }
            else
            {
                unRegCallbackValue = false;
            }
            currentCallback = currentCallback->nextItm;
        }
    }
    else
    {
        unRegCallbackValue = false;
    }
    Cy_SysLib_ExitCriticalSection(interruptState);

    return(unRegCallbackValue);
}


/*******************************************************************************
* Function Name: Cy_SysPm_ExecuteCallback
****************************************************************************//**
*
* Internal API that executes all registered callbacks with type.
* \note The registered callbacks will be executed in order of how callbacks were
* registered: the first registered callback will be first executed. And the
* last registered callback will be executed as the last callback.
*
* Executed callbacks in the registered callback list from index 0 to index
* lastCallback-1. If lastCallback = 0 then all callbacks are executed. Callbacks
* are executed until the first one that returns CY_SYSPM_FAIL. Returns the
* index+1 of the callback that failed. If no callbacks are registered, returns
* CY_SYSPM_SUCCESS.
*
* \param
* type callback type. See \ref cy_en_syspm_callback_type_t.
*
* \param
* mode callback mode. See \ref cy_en_syspm_callback_mode_t.
*
* \param
* lastCallback Index+1 of last registered callback to execute.
* if lastCallback = 0 - Execute all registered callbacks
*
* \return
* CY_SYSPM_SUCCESS Callback successfully completed or nor callbacks registered.
* Something different than CY_SYSPM_SUCCESS - Index+1 of callback that failed.
*
*******************************************************************************/
uint32_t Cy_SysPm_ExecuteCallback(cy_en_syspm_callback_type_t type,
                                  cy_en_syspm_callback_mode_t mode,
                                  uint32_t lastCallback)
{
    uint32_t executedCallbacksCounter = 0u;
    uint32_t retVal = (uint32_t) CY_SYSPM_SUCCESS;
    cy_stc_syspm_callback_t* currentCallback = callbackRoot;

    if(0u == lastCallback)
    {
        lastCallback = CY_SYSPM_PWR_MAX_CALLBACKS_NUMBER;
    }

    while((currentCallback != NULL) && (retVal != ((uint32_t) CY_SYSPM_FAIL)) && (0u != lastCallback))
    {
        if (currentCallback->type == type)
        {
            retVal = (uint32_t) currentCallback->Callback(currentCallback->base, currentCallback->content, mode);
            executedCallbacksCounter++;
            lastCallback--;
        }
        currentCallback = currentCallback->nextItm;
    }

    /* Return Index+1 of callback that failed if callback function returned
    *  CY_SYSPM_FAIL
    */
    if(retVal == (uint32_t) CY_SYSPM_FAIL)
    {
        retVal = executedCallbacksCounter;
    }
    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_GetIoFreezeStatus
****************************************************************************//**
*
* Checks whether IoFreeze is enabled.
*
* \return Returns TRUE if IOs are frozen <br> FALSE if IOs are unfrozen.
*
******************************************************************************/
bool Cy_SysPm_GetIoFreezeStatus(void)
{
    return(0u != _FLD2VAL(SRSS_PWR_HIBERNATE_FREEZE, SRSS->PWR_HIBERNATE));
}


/*******************************************************************************
* Function Name: Cy_SysPm_IoFreeze
****************************************************************************//**
*
* Freezes IOs.
*
* Freezes IO-Cells directly to save the IO-Cell state on a wakeup from the
* Hibernate. You don't need to call this function before entering the
* Hibernate mode, because Cy_SysPm_Hibernate() function freezes IO-Cells
* implicitly.
*
******************************************************************************/
void Cy_SysPm_IoFreeze(void)
{
    uint32_t interruptState;
    interruptState = Cy_SysLib_EnterCriticalSection();

    /* Check the FREEZE state to avoid recurrent IO-Cells freeze attempt,
    * because the second call to this function will cause an accidental switch
    * to the Hibernate mode (the system will enter Hibernate mode immediately
    * after writing to Hibernate bit because both UNLOCK and FREEZE were set
    * correctly in previous call to this function).
    */
    if(0u == _FLD2VAL(SRSS_PWR_HIBERNATE_FREEZE, SRSS->PWR_HIBERNATE))
    {
        /* Clear unlock field for correct freeze of IO-Cells */
        SRSS->PWR_HIBERNATE = _CLR_SET_FLD32U((SRSS->PWR_HIBERNATE), SRSS_PWR_HIBERNATE_UNLOCK, 0u);

        /* Preserve last-reset reason and disable overriding by peripherals next
        *  pin-freeze command
        */
        SRSS->PWR_HIBERNATE |=  CY_SYSPM_PWR_HIBERNATE_UNLOCK |
                                _VAL2FLD(SRSS_PWR_HIBERNATE_FREEZE, 1u) |
                                _VAL2FLD(SRSS_PWR_HIBERNATE_HIBERNATE, 1u);

        /* If reading after writing, read this register three times to delay
        *  enough time for internal settling.
        */
        (void) SRSS->PWR_HIBERNATE;
        (void) SRSS->PWR_HIBERNATE;

        /* Second write causes freeze of IO-Cells to save IO-Cell state */
        SRSS->PWR_HIBERNATE = SRSS->PWR_HIBERNATE;
    }
    Cy_SysLib_ExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Cy_SysPm_IoUnfreeze
****************************************************************************//**
*
* Unfreezes the IOs.
*
* The IO-Cells remain frozen after awaking from the Hibernate mode until the
* firmware unfreezes them after booting. The call of this function unfreezes the
* IO-Cells explicitly.
*
* If the firmware intent is to retain the data value on the port, then the
* value must be read and re-written to the data register before calling this
* API. Furthermore, the drive mode must be re-programmed. If this is not done,
* the pin state will change to default state the moment the freeze is removed.
*
*******************************************************************************/
void Cy_SysPm_IoUnfreeze(void)
{
    uint32_t interruptState;
    interruptState = Cy_SysLib_EnterCriticalSection();

    /* Preserve last reset reason and wakeup polarity. Then, unfreeze I/O:
     * write PWR_HIBERNATE.FREEZE=0, .UNLOCK=0x3A, .HIBERANTE=0,
     */
    SRSS->PWR_HIBERNATE =
    CY_SYSPM_PWR_HIBERNATE_UNLOCK | (SRSS->PWR_HIBERNATE & CY_SYSPM_PWR_RETAIN_HIBERNATE_STATUS);

    /* If reading stands after writing, read this register three times to delay
    *  enough time for internal settling.
    */
    (void) SRSS->PWR_HIBERNATE;
    (void) SRSS->PWR_HIBERNATE;

    /* Lock HIBERNATE mode: write PWR_HIBERNATE.HIBERNATE=0, UNLOCK=0x00, HIBERANTE=0, .TOKEN */
    SRSS->PWR_HIBERNATE &= CY_SYSPM_PWR_RETAIN_HIBERNATE_STATUS;

    Cy_SysLib_ExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Cy_SysPm_SetDelayAfterDeepSleep
****************************************************************************//**
*
* \Internal function for setting the delay after switching the device from
* DeepSleep power mode.
*
* If ECO or PLL is enabled, the 17 us delay should be
* configured before entering into the DeepSleep. If ECO or PLL are
* disabled, the delay is not required.
*
*******************************************************************************/
static void Cy_SysPm_SetDelayAfterDeepSleep(void)
{
    uint32_t clkPath = 0u;
    uint32_t waitClocks = 0u;

    if (_FLD2VAL(SRSS_CLK_ECO_CONFIG_ECO_EN, SRSS->CLK_ECO_CONFIG) == 0u)
    {
        while((CY_SYSPM_PWR_MAX_PLL_NUMBER > clkPath) && (CY_SYSPM_PWR_TRIM_WAKE_CNTS != waitClocks))
        {
            if(_FLD2VAL(SRSS_CLK_PLL_CONFIG_ENABLE, SRSS->CLK_PLL_CONFIG[clkPath]) != 0u)
            {
               waitClocks = CY_SYSPM_PWR_TRIM_WAKE_CNTS;
            }
            clkPath++;
        }
    }
    else
    {
        waitClocks = CY_SYSPM_PWR_TRIM_WAKE_CNTS;
    }

    if(0u != waitClocks)
    {
        SRSS->PWR_TRIM_WAKE_CTL =
        _CLR_SET_FLD32U((SRSS->PWR_TRIM_WAKE_CTL), SRSS_PWR_TRIM_WAKE_CTL_WAKE_DELAY, waitClocks);
    }
}


/*******************************************************************************
* Function Name: Cy_SysPm_SetVoltageBitForFlash
****************************************************************************//**
*
* Internal function that changes the Vcc setting for flash.
*
* \param
* setBit set/clear bit for flash macro register. This bit should be set when
* voltage for core regulators is less than 0.99 V. This bit should be set if
* output voltage for core regulators is higher than 0.99 V.
*
*
*******************************************************************************/
static void Cy_SysPm_SetVoltageBitForFlash(bool setBit)
{
    if(setBit)
    {
        FLASHC_FM_CTL->ANA_CTL0 |= _VAL2FLD(FLASHC_FM_CTL_ANA_CTL0_VCC_SEL, 1u);
    }
    else
    {
        FLASHC_FM_CTL->ANA_CTL0 &= ((uint32_t) (~_VAL2FLD(FLASHC_FM_CTL_ANA_CTL0_VCC_SEL, 1u)));
    }
}


/* [] END OF FILE */
