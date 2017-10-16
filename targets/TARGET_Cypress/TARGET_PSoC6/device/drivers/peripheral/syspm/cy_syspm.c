/***************************************************************************//**
* \file cy_syspm.c
* \version 2.0
*
* This driver provides the source code for the API for power management.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#include "cy_syspm.h"

static cy_stc_syspm_callback_t* callbackRoot = NULL;
static uint32_t curRegisteredCallbacks = 0U;

#ifdef CY_IP_MXUDB
    static void Cy_SaveRegisters(cy_stc_syspm_backup_regs_t *regs);
    static void Cy_RestoreRegisters(cy_stc_syspm_backup_regs_t const *regs);
#endif /* CY_IP_MXUDB */

#if(0u != SRSS_SIMOBUCK_PRESENT)
    static void Cy_SetVoltageBitForFlash(bool setBit);
#endif /* (0u != SRSS_SIMOBUCK_PRESENT) */

static void Cy_EnterDeepSleep(cy_en_syspm_waitfor_t waitFor);

#if(0u != CY_CPU_CORTEX_M4)
    static bool wasEventSent = false;
#endif /* (0u != CY_CPU_CORTEX_M4) */



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
    uint32_t pmStatus = 0U;
    interruptState = Cy_SysLib_EnterCriticalSection();

#if(0u != CY_IP_M4CPUSS)

    /* Check whether CM4 is in the deep sleep mode*/
    if((0U != _FLD2VAL(CPUSS_CM4_STATUS_SLEEPING, CPUSS->CM4_STATUS)) &&
       (0U != _FLD2VAL(CPUSS_CM4_STATUS_SLEEPDEEP, CPUSS->CM4_STATUS)))
    {
        pmStatus |= CY_SYSPM_STATUS_CM4_DEEPSLEEP;
    }
    /* Check whether CM4 is in the sleep mode*/
    else if(0U != _FLD2VAL(CPUSS_CM4_STATUS_SLEEPING, CPUSS->CM4_STATUS))
    {
        pmStatus |= CY_SYSPM_STATUS_CM4_SLEEP;
    }
        else
        {
            pmStatus |= CY_SYSPM_STATUS_CM4_ACTIVE;
        }
#endif /* (0u != CY_IP_M4CPUSS) */

    /* Check whether CM0p is in the deep sleep mode*/
    if((0U != _FLD2VAL(CPUSS_CM0_STATUS_SLEEPING, CPUSS->CM0_STATUS)) &&
       (0U != _FLD2VAL(CPUSS_CM0_STATUS_SLEEPDEEP, CPUSS->CM0_STATUS)))
    {
        pmStatus |= (uint32_t) CY_SYSPM_STATUS_CM0_DEEPSLEEP;
    }
    /* Check whether CM0p is in the sleep mode*/
    else if (0U != _FLD2VAL(CPUSS_CM0_STATUS_SLEEPING, CPUSS->CM0_STATUS))
    {
        pmStatus |= CY_SYSPM_STATUS_CM0_SLEEP;
    }
    else
    {
        pmStatus |= CY_SYSPM_STATUS_CM0_ACTIVE;
    }

    /* Check whether the device is in the low power mode by reading 
    *  the Active Reference status
    */
    if(0U != (_FLD2VAL(SRSS_PWR_CTL_ACT_REF_DIS, SRSS->PWR_CTL)))
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
* Sets a CPU core to the sleep mode.
*
* Puts the core into the Sleep power mode, if none of callback functions were
* registered.
*
* For more details about switching into the sleep power mode and debug,
* refer to the device TRM.
*
* If at least one callback function with the CY_SYSPM_SLEEP type was registered,
* the next algorithm is executed:
* Prior to entering the sleep mode, all callback functions of the CY_SYSPM_SLEEP
* type with the CY_SYSPM_CHECK_READY parameter are called allowing the driver
* to signal whether it is ready to enter the low power mode. If any of the
* callbacks of the CY_SYSPM_SLEEP type with the CY_SYSPM_CHECK_READY parameter 
* returns CY_SYSPM_FAIL, the remaining callback of the CY_SYSPM_SLEEP type with
* the CY_SYSPM_CHECK_READY parameter calls are skipped.
* After CY_SYSPM_FAIL, all the CY_SYSPM_SLEEP callbacks with 
* the CY_SYSPM_CHECK_FAIL parameter are executed. These are the callbacks 
* of the CY_SYSPM_SLEEP type with the CY_SYSPM_CHECK_READY 
* parameter that were previously executed before getting CY_SYSPM_FAIL. 
* The sleep mode is not entered and the Cy_SysPm_Sleep() function returns 
* CY_SYSPM_FAIL.
*
* If all of the callbacks of the CY_SYSPM_SLEEP type with the 
* CY_SYSPM_CHECK_READY parameter calls return CY_SYSPM_SUCCESS, then all 
* callbacks of the CY_SYSPM_SLEEP type with the CY_SYSPM_CHECK_FAIL parameters 
* calls are skipped, and all callbacks of the CY_SYSPM_SLEEP type and 
* CY_SYSPM_BEFORE_TRANSITION parameter calls are executed, allowing the 
* peripherals to prepare for sleep. The sleep mode is then entered for the CPU.
* This is a CPU-centric power mode. It means that the CPU has entered the sleep
* mode and its main clock is removed. It is identical to Active from a 
* peripheral point of view. Any enabled interrupt can cause a wakeup from the 
* sleep mode.
*
* After a wakeup from the sleep, all of the registered callbacks of the 
* CY_SYSPM_SLEEP type and with the CY_SYSPM_AFTER_TRANSITION parameter are 
* executed to return the peripherals to Active operation. The Cy_SysPm_Sleep() 
* function returns CY_SYSPM_SUCCESS.
* No callbacks of the CY_SYSPM_SLEEP type with the CY_SYSPM_BEFORE_TRANSITION 
* parameter or callbacks of the CY_SYSPM_SLEEP type and 
* CY_SYSPM_AFTER_TRANSITION parameter callbacks are executed if the sleep mode 
* is not entered.
*
* \note The last callback which returned CY_SYSPM_FAIL is not executed with the 
* CY_SYSPM_CHECK_FAIL parameter because of the FAIL.
*
* The return values from executed callback functions with the 
* CY_SYSPM_CHECK_FAIL, CY_SYSPM_BEFORE_TRANSITION, and CY_SYSPM_AFTER_TRANSITION
* modes are ignored.
*
* \ref cy_en_syspm_callback_mode_t, except the CY_SYSPM_CHECK_READY, are ignored
*
* \param waitFor Selects wait for action. See \ref cy_en_syspm_waitfor_t.
*
* \return
* Entered status, see \ref cy_en_syspm_status_t.
*
* \sideeffect
* This function clears the Event Register of CM4 core after wakeup from WFE.
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_Sleep(cy_en_syspm_waitfor_t waitFor)
{
    uint32_t interruptState;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;

    CY_ASSERT_L3(CY_SYSPM_IS_WAIT_FOR_VALID(waitFor));

    /* Call registered callback functions with CY_SYSPM_CHECK_READY parameter */
    if(0U != curRegisteredCallbacks)
    {
        retVal = Cy_SysPm_ExecuteCallback(CY_SYSPM_SLEEP, CY_SYSPM_CHECK_READY);
    }

    /* The device (core) can switch into the sleep power mode only when
    *  all executed registered callback functions with the CY_SYSPM_CHECK_READY 
    *  parameter returned CY_SYSPM_SUCCESS.
    */
    if(retVal == CY_SYSPM_SUCCESS)
    {
        /* Call the registered callback functions with 
        * CY_SYSPM_BEFORE_TRANSITION parameter. The return value should be 
        * CY_SYSPM_SUCCESS.
        */
        interruptState = Cy_SysLib_EnterCriticalSection();
        if(0U != curRegisteredCallbacks)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_SLEEP, CY_SYSPM_BEFORE_TRANSITION);
        }

        /* The CPU enters the sleep power mode upon execution of WFI/WFE */
        SCB->SCR = _CLR_SET_FLD32U((SCB->SCR), SCB_SCR_SLEEPDEEP, 0U);

        if(waitFor != CY_SYSPM_WAIT_FOR_EVENT)
        {
            __WFI();
        }
        else
        {
            __WFE();

        #if(0u != CY_CPU_CORTEX_M4)

            /* For the CM4 core the WFE instructions are called twice. 
            *  The second WFE call clears the Event register of CM4 core.
            *  Cypress ID #279077.
            */
            if(wasEventSent)
            {
                __WFE();
            }

            wasEventSent = true;
        #endif /* (0u != CY_CPU_CORTEX_M4) */
        }
        Cy_SysLib_ExitCriticalSection(interruptState);

        /* Call the registered callback functions with the 
        *  CY_SYSPM_AFTER_TRANSITION parameter. The return value should be 
        *  CY_SYSPM_SUCCESS.
        */
        if(0U != curRegisteredCallbacks)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_SLEEP, CY_SYSPM_AFTER_TRANSITION);
        }
    }
    else
    {
        /* Execute callback functions with the CY_SYSPM_CHECK_FAIL parameter to
        *  undo everything done in the callback with the CY_SYSPM_CHECK_READY 
        *  parameter. The return value should be CY_SYSPM_SUCCESS.
        */
        (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_SLEEP, CY_SYSPM_CHECK_FAIL);
        retVal = CY_SYSPM_FAIL;
    }
    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_DeepSleep
****************************************************************************//**
*
* Sets a CPU core to the deep sleep mode.
*
* Puts the core into the deep sleep power mode. Prior to entering the deep sleep
* mode, all callbacks of the CY_SYSPM_DEEPSLEEP type with the 
* CY_SYSPM_CHECK_READY parameter registered callbacks are called, allowing the 
* driver to signal whether it is ready to enter the power mode. If any 
* CY_SYSPM_DEEPSLEEP type with the CY_SYSPM_CHECK_READY parameter call returns 
* CY_SYSPM_FAIL, the remaining callback CY_SYSPM_DEEPSLEEP type with the 
* CY_SYSPM_CHECK_READY parameter calls are skipped. After a CY_SYSPM_FAIL, all
* of the callbacks of the CY_SYSPM_DEEPSLEEP type with the CY_SYSPM_CHECK_FAIL 
* parameter are executed that correspond to the callbacks with 
* CY_SYSPM_DEEPSLEEP type with CY_SYSPM_CHECK_READY parameter calls that 
* occurred up to the point of failure.
* The deep sleep mode is not entered and the Cy_SysPm_DeepSleep() function 
* returns CY_SYSPM_FAIL.
*
* If all callbacks of the CY_SYSPM_DEEPSLEEP type with the CY_SYSPM_CHECK_READY
* parameter calls return CY_SYSPM_SUCCESS, then all callbacks of the 
* CY_SYSPM_DEEPSLEEP type with the CY_SYSPM_CHECK_FAIL parameter calls are 
* skipped and all callbacks of the CY_SYSPM_DEEPSLEEP type with the 
* CY_SYSPM_BEFORE_TRANSITION parameter calls are executed, allowing the 
* peripherals to prepare for deep sleep.
* The deep sleep mode is then entered. Any enabled interrupt can cause a wakeup 
* from the deep sleep mode.
*
* \note The last callback which returned CY_SYSPM_FAIL is not executed with the 
* CY_SYSPM_CHECK_FAIL parameter because of the FAIL.
*
* The return values from executed callback functions with the 
* CY_SYSPM_CHECK_FAIL, CY_SYSPM_BEFORE_TRANSITION, and CY_SYSPM_AFTER_TRANSITION
* modes are ignored.
*
* If the firmware attempts to enter this mode before the system is ready (that
* is, when PWR_CONTROL.LPM_READY = 0), then the device will go into the (LP)
* Sleep mode instead and automatically enter the deep sleep mode when the
* system is ready.
*
* The system puts the whole device into the deep sleep mode when all the 
* processor(s) is (are) in deep sleep, there are no busy peripherals, the 
* debugger is not active, and the deep sleep circuits are 
* ready (PWR_CONTROL.LPM_READY=1).
*
* The peripherals that do not need a clock or that receive a clock from their
* external interface (e.g. I2C/SPI) continue operating. All circuits using the
* current from Vccdpslp supply are under the current limitation that is 
* controlled by deep sleep regulator.
*
* Wakeup occurs when an interrupt asserts from a deep sleep active peripheral.
* For more details, see the corresponding peripheral's datasheet.
*
* \note For multi-core devices the second core, that did not participate in 
* device wakeup, remain execute the deep sleep instructions. Any deep sleep 
* capable interrupt routed to this core can wakeup it.
*
* For more details about switching into the deep sleep power mode and debug,
* refer to the device TRM.
*
* A normal wakeup from the deep sleep power mode returns to either LPActive, 
* LPSleep, Active, or Sleep, depending on the previous state and programmed 
* behaviour for the particular wakeup interrupt.
*
* After wakeup from Deep Sleep, all of the registered callbacks with
* CY_SYSPM_DEEPSLEEP type with CY_SYSPM_AFTER_TRANSITION are executed to return 
* peripherals to Active operation. The Cy_SysPm_DeepSleep() function returns 
* CY_SYSPM_SUCCESS. No callbacks are executed with CY_SYSPM_DEEPSLEEP type with 
* CY_SYSPM_BEFORE_TRANSITION or CY_SYSPM_AFTER_TRANSITION parameter, if deep 
* sleep mode was not entered.
*
* \param waitFor Selects wait for action. See \ref cy_en_syspm_waitfor_t.
*
* \sideeffect
* This side effect is applicable only for devices with UDB IP block available. 
* You can obtain unpredictable behavior of the UDB block after the device wakeup
* from Deep Sleep.
* Unpredictable behavior scenario:
* * The first core saves non-retained UDB configuration registers and goes into
* the Deep Sleep (Cy_SysPm_DeepSleep() function).
* * These non-retained UDB configuration registers are modified in runtime by 
* another (second) active core.
* * The second core saves non-retained UDB configuration registers and goes into
* the Deep Sleep (Cy_SysPm_DeepSleep() function).
* These conditions save different values of the non-retained UDB configuration
* registers. The prevented scenario: on the first core wakeup, these registers 
* are restored by the values saved on the first core, and after the second core 
* wakeup, these registers are "reconfigured" by the values saved on the second 
* core.
* Be aware of such a situation.
*
* \sideeffect
* This function clears the Event Register of CM4 core after wakeup from WFE.
*
* \sideeffect
* This function changes the the slow and fast clock dividers to 
* CY_SYSPM_CLK_DIVIDER right before entering into the deep sleep and restores 
* these dividers after the wakeup.
*
* \return
* Entered status, see \ref cy_en_syspm_status_t.
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_DeepSleep(cy_en_syspm_waitfor_t waitFor)
{
    uint32_t interruptState;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;

    CY_ASSERT_L3(CY_SYSPM_IS_WAIT_FOR_VALID(waitFor));

    /* Call the registered callback functions with 
    * the CY_SYSPM_CHECK_READY parameter.
    */
    if(0U != curRegisteredCallbacks)
    {
        retVal = Cy_SysPm_ExecuteCallback(CY_SYSPM_DEEPSLEEP, CY_SYSPM_CHECK_READY);
    }

    /* The device (core) can switch into the deep sleep power mode only when
    *  all executed registered callback functions with the CY_SYSPM_CHECK_READY
    *  parameter returned CY_SYSPM_SUCCESS.
    */
    if(retVal == CY_SYSPM_SUCCESS)
    {
        /* Call the registered callback functions with the 
        * CY_SYSPM_BEFORE_TRANSITION parameter. The return value should be 
        * CY_SYSPM_SUCCESS.
        */
        interruptState = Cy_SysLib_EnterCriticalSection();
        if(0U != curRegisteredCallbacks)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_DEEPSLEEP, CY_SYSPM_BEFORE_TRANSITION);
        }

    #ifdef CY_IP_MXUDB

        static cy_stc_syspm_backup_regs_t regs;

        /* Check is the UDB disabled on MMIO level */
        if(_FLD2BOOL(PERI_GR_SL_CTL_ENABLED_4, PERI->GR[CY_SYSPM_PERI_GR3_UDB].SL_CTL))
        {

            /* Save non-retained registers */
            Cy_SaveRegisters(&regs);
        }
    #endif /* CY_IP_MXUDB */

        Cy_EnterDeepSleep(waitFor);

    #ifdef CY_IP_MXUDB

        /* Do not restore the UDB if it is disabled on MMIO level */
        if(_FLD2BOOL(PERI_GR_SL_CTL_ENABLED_4, PERI->GR[CY_SYSPM_PERI_GR3_UDB].SL_CTL))
        {
            /* Restore non-retained registers */
            Cy_RestoreRegisters(&regs);
        }
    #endif /* CY_IP_MXUDB */

        Cy_SysLib_ExitCriticalSection(interruptState);

        /* Call the registered callback functions with the CY_SYSPM_AFTER_TRANSITION 
        *  parameter. The return value should be CY_SYSPM_SUCCESS.
        */
        if(0U != curRegisteredCallbacks)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_DEEPSLEEP, CY_SYSPM_AFTER_TRANSITION);
        }
    }
    else
    {
        /* Execute callback functions with the CY_SYSPM_CHECK_FAIL parameter to 
        * undo everything done in the callback with the CY_SYSPM_CHECK_READY 
        * parameter. The return value should be CY_SYSPM_SUCCESS.
        */
        (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_DEEPSLEEP, CY_SYSPM_CHECK_FAIL);
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
* Puts the core into the hibernate power mode. Prior to entering the hibernate 
* mode, all callbacks of the CY_SYSPM_HIBERNATE type are executed. 
* First, callbacks of the CY_SYSPM_HIBERNATE type and with 
* CY_SYSPM_CHECK_READY parameter are called allowing the driver to signal if it 
* is not ready to enter the power mode. If any of the callbacks of the 
* CY_SYSPM_HIBERNATE type with the CY_SYSPM_CHECK_READY parameter call returns 
* CY_SYSPM_FAIL, the remaining CY_SYSPM_HIBERNATE callbacks with the 
* CY_SYSPM_CHECK_READY parameter calls are skipped. After CY_SYSPM_FAIL, all 
* of the CY_SYSPM_HIBERNATE callbacks with CY_SYSPM_CHECK_FAIL parameter are 
* executed that correspond to the CY_SYSPM_HIBERNATE callbacks with 
* CY_SYSPM_CHECK_READY parameter calls that occurred up to the point of failure.
* The hibernate mode is not entered and the Cy_SysPm_Hibernate() function 
* returns CY_SYSPM_FAIL.
*
* If all CY_SYSPM_HIBERNATE callbacks with the CY_SYSPM_CHECK_READY parameter 
* calls return CY_SYSPM_SUCCESS, then all CY_SYSPM_HIBERNATE callbacks with 
* CY_SYSPM_CHECK_FAIL calls are skipped and all CY_SYSPM_HIBERNATE callbacks 
* CY_SYSPM_BEFORE_TRANSITION parameter calls are executed allowing the 
* peripherals to prepare for Hibernate. The I/O output state is frozen and the 
* hibernate mode is then entered. In the hibernate mode, all internal supplies 
* are off and no internal state is retained. There is no handshake with the 
* CPUs and the chip will begin entry to Hibernate immediately.
*
* The I/O output state is frozen and the hibernate mode is then 
* entered. In the hibernate mode all internal supplies are off and no 
* internal state is retained.
* For multi-core devices there is no handshake with the CPUs and the chip 
* will begin entry to the hibernate power mode immediately.
*
* \note The last callback which returned CY_SYSPM_FAIL is not executed with the 
* CY_SYSPM_CHECK_FAIL parameter because of the FAIL.
*
* The return values from executed callback functions with the 
* CY_SYSPM_CHECK_FAIL, CY_SYSPM_BEFORE_TRANSITION, and CY_SYSPM_AFTER_TRANSITION
* modes are ignored.
*
* A wakeup from Hibernate is triggered by toggling the wakeup pin(s), a WDT
* match, or back-up domain alarm expiration depending on how the they were 
* configured. A wakeup causes a normal boot procedure.
* To configure the wakeup pin(s), a Digital Input Pin must be configured, and
* resistively pulled up or down to the inverse state of the wakeup polarity. To
* distinguish a wakeup from the Hibernate mode and a general reset event, the
* Cy_SysLib_GetResetReason() function can be used. The wakeup pin, low-power 
* comparators are active-low by default. The wakeup pin or the LPComparators 
* polarity can be changed with the \ref Cy_SysPm_SetHibWakeupSource() function.
* This function call will not return if the hibernate mode is entered. 
* The CY_SYSPM_HIBERNATE callbacks with the CY_SYSPM_AFTER_TRANSITION parameter
* are never executed.
*
* This function freezes the IO cells implicitly. It is not possible to enter
* the Hibernate mode before freezing the IO cells. The IO cells remain frozen
* after waking from the hibernate mode until the firmware unfreezes them
* with a \ref Cy_SysPm_IoUnfreeze() function call.
*
* \return
* Entered status, see \ref cy_en_syspm_status_t.
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_Hibernate(void)
{
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;

    /* Call the registered callback functions with the 
    * CY_SYSPM_CHECK_READY parameter
    */
    if(0U != curRegisteredCallbacks)
    {
        retVal = Cy_SysPm_ExecuteCallback(CY_SYSPM_HIBERNATE, CY_SYSPM_CHECK_READY);
    }

    /* The device (core) can switch into Hibernate power mode only when
    *  all executed registered callback functions with CY_SYSPM_CHECK_READY
    *  parameter returned CY_SYSPM_SUCCESS.
    */
    if(retVal == CY_SYSPM_SUCCESS)
    {
        /* Call registered callback functions with CY_SYSPM_BEFORE_TRANSITION 
        *  parameter. Return value should be CY_SYSPM_SUCCESS.
        */
        (void) Cy_SysLib_EnterCriticalSection();
        if(0U != curRegisteredCallbacks)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_HIBERNATE, CY_SYSPM_BEFORE_TRANSITION);
        }

        /* Preserve the token that will retain through a wakeup sequence
         * thus could be used by Cy_SysLib_GetResetReason() to differentiate
         * Wakeup from a general reset event.
         * Preserve the wakeup source(s) configuration.
         */
        SRSS->PWR_HIBERNATE = 
        (SRSS->PWR_HIBERNATE & CY_SYSPM_PWR_WAKEUP_HIB_MASK) | CY_SYSPM_PWR_TOKEN_HIBERNATE;

        /* All the three writes to hibernate register use the same value:
        *  PWR_HIBERNATE.FREEZE=1, .UNLOCK=0x3A, .HIBERANTE=1,
        */
        SRSS->PWR_HIBERNATE |= CY_SYSPM_PWR_SET_HIBERNATE;
  
        SRSS->PWR_HIBERNATE |= CY_SYSPM_PWR_SET_HIBERNATE;

        SRSS->PWR_HIBERNATE |= CY_SYSPM_PWR_SET_HIBERNATE;
        
        /* Read register to make sure it is settled */
        (void) SRSS->PWR_HIBERNATE;

        /* Wait for transition */
        __WFI();

        /* The callback functions calls with the CY_SYSPM_AFTER_TRANSITION 
        * parameter in the hibernate power mode are not applicable as device 
        * wake-up was made on device reboot.
        */

        /* A wakeup from the hibernate is performed by toggling of the wakeup 
        * pins, or WDT matches, or Backup domain alarm expires. Depends on what 
        * item is configured in the hibernate register. After a wakeup event, a
        * normal Boot procedure occurs. 
        * No need to exit from the critical section.
        */
    }
    else
    {
        /* Execute callback functions with the CY_SYSPM_CHECK_FAIL parameter to 
        * undo everything done in the callback with the CY_SYSPM_CHECK_READY 
        * parameter. The return value should be CY_SYSPM_SUCCESS.
        */
        (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_HIBERNATE, CY_SYSPM_CHECK_FAIL);
        retVal = CY_SYSPM_FAIL;
    }
    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_EnterLpMode
****************************************************************************//**
*
* Sets device into the low power mode.
*
* The low power (LPActive/LPSleep) modes are similar to the active/sleep modes.
* The difference is that the current is limited and some functions have limited 
* features/performance.
*
* The feature of the low power mode is the limited current. Restrictions are 
* placed on the clock frequencies and allow the peripherals to achieve 
* a current limit.
*
* Before entering the low power mode, the user should configure the system so 
* the total current drawn from Vccd is less that the value presented in the 
* technical reference manual (TRM). Refer to the TRM for the maximum load for 
* low power operation and clock limitations in the low power mode with different
* core supply regulator voltages.
*
* * Peripherals can use the knowledge of the LPActive/LPSleep mode to make
* trade-offs that consume less current. For more details, see the corresponding
* peripherals' datasheet.
* * High-speed clock sources are available with the appropriate pre-divider 
* settings to limit the system current.
* Refer to the TRM for the maximum frequency values for low power operation 
* using different Core Regulators output voltages.
*
* Puts the device into the low power mode. Prior to entering the low power mode,
* all the registered CY_SYSPM_ENTER_LP_MODE callbacks with CY_SYSPM_CHECK_READY 
* parameter are called. This allows the driver to signal if it is not ready to 
* enter the low power mode. If any CY_SYSPM_ENTER_LP_MODE callbacks with the
* CY_SYSPM_CHECK_READY parameter call returns CY_SYSPM_FAIL, the remaining
* CY_SYSPM_ENTER_LP_MODE callbacks with the CY_SYSPM_CHECK_READY parameter 
* calls are skipped.
*
* After a CY_SYSPM_FAIL all of the CY_SYSPM_ENTER_LP_MODE callbacks with 
* CY_SYSPM_CHECK_FAIL parameter are executed that correspond to the 
* CY_SYSPM_ENTER_LP_MODE callbacks with CY_SYSPM_CHECK_READY parameter calls 
* that occurred up to the point of failure. Low Power mode is not entered and 
* the Cy_SysPm_EnterLpMode() function returns CY_SYSPM_FAIL.
*
* If all CY_SYSPM_ENTER_LP_MODE callbacks with the CY_SYSPM_CHECK_READY 
* parameter calls return CY_SYSPM_SUCCESS, then all CY_SYSPM_ENTER_LP_MODE 
* callbacks with CY_SYSPM_CHECK_FAIL calls are skipped and all 
* CY_SYSPM_ENTER_LP_MODE callbacks with the CY_SYSPM_BEFORE_TRANSITION parameter
* calls are executed. This allows the peripherals to prepare for low power.
* The low power mode is then entered.
*
* After entering the low power mode, all of the registered 
* CY_SYSPM_ENTER_LP_MODE callbacks with the CY_SYSPM_AFTER_TRANSITION parameter 
* are executed to complete preparing the peripherals for low power operation. 
* The Cy_SysPm_EnterLpMode() function returns CY_SYSPM_SUCCESS. 
* No CY_SYSPM_ENTER_LP_MODE callbacks with the CY_SYSPM_BEFORE_TRANSITION or 
* CY_SYSPM_AFTER_TRANSITION parameter are executed, if the low power mode is not
* entered.
*
* \note The last callback which returned CY_SYSPM_FAIL is not executed with 
* the CY_SYSPM_CHECK_FAIL parameter because of the FAIL.
*
* The return values from executed callback functions with the 
* CY_SYSPM_CHECK_FAIL, CY_SYSPM_BEFORE_TRANSITION, and CY_SYSPM_AFTER_TRANSITION
* modes are ignored.
*
* \note The callbacks are not executed if the device is already not in 
* the low power mode.
*
* \return
* See \ref cy_en_syspm_status_t. <br>
* CY_SYSPM_SUCCESS - Entered the LPActive mode or the device is already 
* in LPActive.<br>
* CY_SYSPM_FAIL - The LPActive mode is not entered or low power circuits 
* are not ready to enter the low power mode. 
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_EnterLpMode(void)
{
    uint32_t interruptState;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;

    /* Check whether device is in the low power mode. */
    if(0U == (_FLD2VAL(SRSS_PWR_CTL_ACT_REF_DIS, SRSS->PWR_CTL)))
    {
        /* The entering into the low power mode is permitted when low 
        *  power circuits are ready to enter into the low power mode.
        */
        if(0U != _FLD2VAL(SRSS_PWR_CTL_LPM_READY, SRSS->PWR_CTL))
        {
            /* Call the registered callback functions with the 
            * CY_SYSPM_CHECK_READY parameter.
            */
            if(0U != curRegisteredCallbacks)
            {
                retVal = Cy_SysPm_ExecuteCallback(CY_SYSPM_ENTER_LP_MODE, CY_SYSPM_CHECK_READY);
            }

            /* The device (core) can switch into the low power mode only when
            * all executed registered callback functions with the 
            * CY_SYSPM_CHECK_READY parameter returned CY_SYSPM_SUCCESS.
            */
            if(retVal == CY_SYSPM_SUCCESS)
            {
                
                /* Call the registered callback functions with the 
                * CY_SYSPM_BEFORE_TRANSITION parameter. The return value 
                * should be CY_SYSPM_SUCCESS.
                */
                interruptState = Cy_SysLib_EnterCriticalSection();
                if(0U != curRegisteredCallbacks)
                {
                    (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_ENTER_LP_MODE, CY_SYSPM_BEFORE_TRANSITION);
                }

                /* Configure the low-power operating mode for Linear Core regulator */
                if(Cy_SysPm_LdoIsEnabled())
                {
                    SRSS->PWR_CTL |= (_VAL2FLD(SRSS_PWR_CTL_LINREG_LPMODE, 1U) |
                                      _VAL2FLD(SRSS_PWR_CTL_PORBOD_LPMODE, 1U) |
                                      _VAL2FLD(SRSS_PWR_CTL_BGREF_LPMODE, 1U) |
                                      _VAL2FLD(SRSS_PWR_CTL_VREFBUF_LPMODE, 1U) |
                                      _VAL2FLD(SRSS_PWR_CTL_IREF_LPMODE, 1U));
                }
                else
                {
                    /* Configure the low-power operating mode for SIMO Buck regulator */
                    SRSS->PWR_CTL |= (_VAL2FLD(SRSS_PWR_CTL_PORBOD_LPMODE, 1U) |
                                      _VAL2FLD(SRSS_PWR_CTL_BGREF_LPMODE, 1U) |
                                      _VAL2FLD(SRSS_PWR_CTL_VREFBUF_LPMODE, 1U) |
                                      _VAL2FLD(SRSS_PWR_CTL_IREF_LPMODE, 1U));
                }

                /* This wait time allows the circuits to remove their dependence on
                *  the Active mode circuits, such as Active Reference.
                */
                Cy_SysLib_DelayUs(CY_SYSPM_ACTIVE_TO_LP_WAIT_US);

                /* Disabling active reference */
                SRSS->PWR_CTL |= _VAL2FLD(SRSS_PWR_CTL_ACT_REF_DIS, 1U);

                Cy_SysLib_ExitCriticalSection(interruptState);

                /* Call the registered callback functions with the 
                * CY_SYSPM_AFTER_TRANSITION parameter. The return value 
                * should be CY_SYSPM_SUCCESS.
                */
                if(0U != curRegisteredCallbacks)
                {
                    (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_ENTER_LP_MODE, CY_SYSPM_AFTER_TRANSITION);
                }
            }
            else
            {
                /* Execute callback functions with the CY_SYSPM_CHECK_FAIL parameter to 
                * undo everything done in the callback with the CY_SYSPM_CHECK_READY 
                * parameter. The return value should be CY_SYSPM_SUCCESS.
                */
                (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_ENTER_LP_MODE, CY_SYSPM_CHECK_FAIL);
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
        /* Do nothing because the device is already in the low power mode. */
    }
    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_ExitLpMode
****************************************************************************//**
*
* Sets the device out of the low power mode.
*
* Returns the device to the active mode. In the active power mode, the operating
* current can be increased to the normal mode limit. The clock frequencies also 
* can be increased to the normal mode limit. Refer to the device TRM for the 
* current and frequency limitations in the active power mode.
* 
* Prior to exiting the low power mode, all the registered CY_SYSPM_EXIT_LP_MODE 
* callbacks with the CY_SYSPM_CHECK_READY parameter are called. This allows
* the driver to signal if it is not ready to exit the 
* low power mode. If any CY_SYSPM_EXIT_LP_MODE callbacks with 
* the CY_SYSPM_CHECK_READY parameter call returns CY_SYSPM_FAIL, the remaining
* CY_SYSPM_EXIT_LP_MODE callbacks with the CY_SYSPM_CHECK_READY parameter calls
* are skipped. After a CY_SYSPM_FAIL, all of the CY_SYSPM_EXIT_LP_MODE callbacks 
* with CY_SYSPM_CHECK_FAIL parameter are executed that correspond to the
* CY_SYSPM_EXIT_LP_MODE callbacks with CY_SYSPM_CHECK_READY parameter calls that
* occurred up to the point of failure. The active mode is not entered and the 
* Cy_SysPm_ExitLpMode() function returns CY_SYSPM_FAIL.
*
* If all CY_SYSPM_EXIT_LP_MODE callbacks with CY_SYSPM_CHECK_READY calls return 
* CY_SYSPM_SUCCESS, then all the CY_SYSPM_EXIT_LP_MODE callbacks with 
* the CY_SYSPM_CHECK_FAIL parameter calls are skipped and all 
* CY_SYSPM_EXIT_LP_MODE callbacks with the CY_SYSPM_BEFORE_TRANSITION parameter 
* calls are executed allowing the peripherals to prepare for the active mode. 
* The low power mode is then exited.
*
* After exiting the low power mode, all of the registered callbacks are executed 
* CY_SYSPM_EXIT_LP_MODE callbacks with the CY_SYSPM_AFTER_TRANSITION parameter 
* to complete preparing the peripherals for the active mode operation. 
* The Cy_SysPm_ExitLpMode() function returns CY_SYSPM_SUCCESS. 
* No CY_SYSPM_EXIT_LP_MODE callbacks with the CY_SYSPM_BEFORE_TRANSITION or 
* CY_SYSPM_AFTER_TRANSITION parameter are executed if the low power mode is 
* not exited.
*
* \note The last callback which returned CY_SYSPM_FAIL is not executed with the 
* CY_SYSPM_CHECK_FAIL parameter because of the FAIL.
*
* The return values from executed callback functions with the 
* CY_SYSPM_CHECK_FAIL, CY_SYSPM_BEFORE_TRANSITION, and CY_SYSPM_AFTER_TRANSITION
* modes are ignored.
*
* \note The callbacks are not executed if the device is not already in the low 
* power mode.
*
* \return
* See \ref cy_en_syspm_status_t. <br>
* CY_SYSPM_SUCCESS - Exited from the LPActive power mode, or the device is 
* already in the active power. <br> 
* CY_SYSPM_FAIL - Exit from the LPActive mode is not done.
*
* \warning This function blocks as it waits until Active Reference is ready
* to enter into the normal mode.
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_ExitLpMode(void)
{
    uint32_t interruptState;
    uint32_t timeOut = CY_SYSPM_WAIT_DELAY_TRYES;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;

    /* Check if the device is in the low power mode */
    if(0U != (_FLD2VAL(SRSS_PWR_CTL_ACT_REF_DIS, SRSS->PWR_CTL)))
    {
        /* Call the registered callback functions with the 
        *  CY_SYSPM_CHECK_READY parameter.
        */
        if(0U != curRegisteredCallbacks)
        {
            retVal = Cy_SysPm_ExecuteCallback(CY_SYSPM_EXIT_LP_MODE, CY_SYSPM_CHECK_READY);
        }

        /* The device (core) can switch into the low power mode only in 
        * condition that all executed registered callback functions with the 
        * CY_SYSPM_CHECK_READY parameter returned CY_SYSPM_SUCCESS.
        */
        if(retVal == CY_SYSPM_SUCCESS)
        {
            /* Call the registered callback functions with the 
            * CY_SYSPM_BEFORE_TRANSITION parameter. The return value should be 
            * CY_SYSPM_SUCCESS.
            */
            interruptState = Cy_SysLib_EnterCriticalSection();
            if(0U != curRegisteredCallbacks)
            {
                (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_EXIT_LP_MODE, CY_SYSPM_BEFORE_TRANSITION);
            }

            /* Set the normal operation mode for Linear Core regulator if
            *  it is enabled
            */
            if(Cy_SysPm_LdoIsEnabled())
            {
                SRSS->PWR_CTL &= ((uint32_t)~(SRSS_PWR_CTL_LINREG_LPMODE_Msk));
            }

            /* Configure the normal operating mode for the POR/BOD circuits and 
            * for the Bandgap Voltage and Current References
            */
            SRSS->PWR_CTL &= ((uint32_t)~(_VAL2FLD(SRSS_PWR_CTL_PORBOD_LPMODE, 1U) |
                                          _VAL2FLD(SRSS_PWR_CTL_ACT_REF_DIS, 1U) |
                                          _VAL2FLD(SRSS_PWR_CTL_VREFBUF_LPMODE, 1U) |
                                          _VAL2FLD(SRSS_PWR_CTL_IREF_LPMODE, 1U)));

            /* This wait time allows setting Active Reference */
            Cy_SysLib_DelayUs(CY_SYSPM_LP_TO_ACTIVE_WAIT_BEFORE_US);

            while((0U == _FLD2VAL(SRSS_PWR_CTL_ACT_REF_OK, SRSS->PWR_CTL)) && (0U != timeOut))
            {
                timeOut--;
            }

            if(0U == timeOut)
            {
                retVal = CY_SYSPM_TIMEOUT;
                
                Cy_SysLib_ExitCriticalSection(interruptState);
            }
            else
            {
                /* Configure the normal operation mode */
                SRSS->PWR_CTL &= ((uint32_t) (~SRSS_PWR_CTL_BGREF_LPMODE_Msk));
                
                /* This wait time allows setting Active Reference */
                Cy_SysLib_DelayUs(CY_SYSPM_LP_TO_ACTIVE_WAIT_AFTER_US);

                Cy_SysLib_ExitCriticalSection(interruptState);
            
                /* Call registered callback functions with CY_SYSPM_AFTER_TRANSITION 
                *  parameter. Return value should be CY_SYSPM_SUCCESS.
                */
                if(0U != curRegisteredCallbacks)
                {
                    (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_EXIT_LP_MODE, CY_SYSPM_AFTER_TRANSITION);
                }                
            }

        }
        else
        {
            /* Execute callback functions with the CY_SYSPM_CHECK_FAIL parameter to 
            * undo everything done in the callback with the CY_SYSPM_CHECK_READY 
            * parameter. The return value should be CY_SYSPM_SUCCESS.
            */
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_EXIT_LP_MODE, CY_SYSPM_CHECK_FAIL);
            retVal = CY_SYSPM_FAIL;
        }
    }
    else
    {
        /* Do nothing because the device is already in the active power mode. */
    }
    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_SleepOnExit
****************************************************************************//**
*
* Set Sleep on the Exit mode.
*
* This API gives a possibility to keep the system in the sleep mode at all times
* and only wakeup to run a single interrupt and then immediately go back to
* sleep.
*
* \param enable
* True - enable Sleep on the Exit mode <br> false - disable
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
        SCB->SCR |= _VAL2FLD(SCB_SCR_SLEEPONEXIT, 1U);
    }
    else
    {
        /* Disable SleepOnExit mode */
        SCB->SCR &= ((uint32_t) ~(SCB_SCR_SLEEPONEXIT_Msk));
    }
    Cy_SysLib_ExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Cy_SysPm_SetHibWakeupSource
****************************************************************************//**
*
* This function configures sources to wake up the device from the hibernate 
* power mode. Such sources can be wakeup pins, LPComparators, Watchdog (WDT)
* interrupt, or Real-Time clock (RTC) alarm (interrupt).
*
* Wakeup pins:
*
* A wakeup is supported by up to two pins with programmable polarity. These pins
* may be connected to the IO pins or on-chip peripherals under some conditions.
* Setting the wakeup pin to this level will cause a wakeup from the hibernate
* mode. The wakeup pins are active/low by default.
*
* LPComparators:
*
* A wakeup is supported by up to two LPComps with programmable polarity. These 
* LPComp may be connected to the IO pins or on-chip peripherals under some 
* conditions.
* Setting the LPComp to this level will cause a wakeup from the hibernate
* mode. The wakeup LPComp are active/low by default.
*
* \note The low-power comparators should be configured and enabled before 
* switching into the hibernate low power mode. Refer to the LPComp
* driver description for more details.
*
* Watchdog Timer:
* 
* \note The WDT should be configured and enabled before entering into the 
* hibernate power mode.
*
* A wakeup is performed by a WDT interrupt and a normal boot procedure 
* after a device reset. The device can wake up from Hibernate after a WDT 
* device reset, if the WDT was not configured to wake up, the device on its 
* interrupt and WDT was enabled.
*
* Real-time Clock:
*
* A wakeup is performed by the RTC alarm and a normal boot procedure 
* after a device reset.
* Refer to the Real-Time Clock (RTC) driver description for more details.
*
* For information about wakeup sources and their assignment in the specific 
* families devices, refer to the appropriate device TRM.
*
* \param wakeupSource 
* The source to be configured as a wakeup source from 
* the hibernate power mode. The input parameters values can be ORed.
* For example, if you want to set LPComp0 (active high) and WDT, call 
* this function:
* Cy_SysPm_SetHibWakeupSource(CY_SYSPM_LPCOMP0_HIGH | CY_SYSPM_HIBWDT).
*
* \warning Do not call this function with different polarity levels for the same
* wakeup source. For example, do not call a function like this:
* Cy_SysPm_SetHibWakeupSource(CY_SYSPM_LPCOMP0_LOW, CY_SYSPM_LPCOMP0_HIGH);
*
*******************************************************************************/
void Cy_SysPm_SetHibWakeupSource(cy_en_syspm_hib_wakeup_source_t wakeupSource)
{
    CY_ASSERT_L3(CY_SYSPM_IS_WAKE_UP_SOURCE_VALID(wakeupSource));
    
    /* Reconfigure the wake-up pins and LPComp polarity based on the input */
    if(0U != ((uint32_t) wakeupSource & CY_SYSPM_WAKEUP_LPCOMP0))
    {
        SRSS->PWR_HIBERNATE &= 
        ((uint32_t) ~_VAL2FLD(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, CY_SYSPM_WAKEUP_LPCOMP0_BIT));
    }
    
    if(0U != ((uint32_t) wakeupSource & CY_SYSPM_WAKEUP_LPCOMP1))
    {
        SRSS->PWR_HIBERNATE &= 
        ((uint32_t) ~_VAL2FLD(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, CY_SYSPM_WAKEUP_LPCOMP1_BIT));
    }
    
    if(0U != ((uint32_t) wakeupSource & CY_SYSPM_WAKEUP_PIN0))
    {
        SRSS->PWR_HIBERNATE &= 
        ((uint32_t) ~_VAL2FLD(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, CY_SYSPM_WAKEUP_PIN0_BIT));
    }
    
    if(0U != ((uint32_t) wakeupSource & CY_SYSPM_WAKEUP_PIN1))
    {
        SRSS->PWR_HIBERNATE &= 
        ((uint32_t) ~_VAL2FLD(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, CY_SYSPM_WAKEUP_PIN1_BIT));
    }

    SRSS->PWR_HIBERNATE |= ((uint32_t) wakeupSource);

    /* Read register to make sure it is settled */
    (void) SRSS->PWR_HIBERNATE;
}


/*******************************************************************************
* Function Name: Cy_SysPm_ClearHibWakeupSource
****************************************************************************//**
*
* This function disables a wakeup source which was previously configured to 
* wake up the device from the hibernate power mode.
*
* \param wakeupSource
* The source to be disabled. The input parameters values can 
* be ORed. For example, if you want to disable LPComp0 (active high) and WDT
* call this function:
* Cy_SysPm_ClearHibWakeupSource(CY_SYSPM_LPCOMP0_HIGH | CY_SYSPM_HIBWDT).
*
*******************************************************************************/
void Cy_SysPm_ClearHibWakeupSource(cy_en_syspm_hib_wakeup_source_t wakeupSource)
{
    uint32_t clearWakeupSource;

    CY_ASSERT_L3(CY_SYSPM_IS_WAKE_UP_SOURCE_VALID(wakeupSource));

    if (0U != _FLD2VAL(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, wakeupSource))
    {
        /* Clear the high active level of the requested sources */
        if((uint32_t) CY_SYSPM_LPCOMP0_HIGH == ((uint32_t) wakeupSource & (uint32_t) CY_SYSPM_LPCOMP0_HIGH))
        {
            SRSS->PWR_HIBERNATE &= 
            ((uint32_t) ~_VAL2FLD(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, CY_SYSPM_WAKEUP_LPCOMP0_BIT));
        }
        
        if((uint32_t) CY_SYSPM_LPCOMP1_HIGH == ((uint32_t) wakeupSource & (uint32_t) CY_SYSPM_LPCOMP1_HIGH))
        {
            SRSS->PWR_HIBERNATE &= 
            ((uint32_t) ~_VAL2FLD(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, CY_SYSPM_WAKEUP_LPCOMP1_BIT));
        }
        
        if((uint32_t) CY_SYSPM_HIBPIN0_HIGH == ((uint32_t) wakeupSource & (uint32_t) CY_SYSPM_HIBPIN0_HIGH))
        {
            SRSS->PWR_HIBERNATE &= 
            ((uint32_t) ~_VAL2FLD(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, CY_SYSPM_WAKEUP_PIN0_BIT));
        }
        
        if((uint32_t) CY_SYSPM_HIBPIN1_HIGH == ((uint32_t) wakeupSource & (uint32_t) CY_SYSPM_HIBPIN1_HIGH))
        {
            SRSS->PWR_HIBERNATE &= 
            ((uint32_t) ~_VAL2FLD(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, CY_SYSPM_WAKEUP_PIN1_BIT));
        }
    }

    /* Remove the polarity bits from the input value */
    clearWakeupSource = ((uint32_t) wakeupSource & ((uint32_t) ~ SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Msk));

    SRSS->PWR_HIBERNATE &= ((uint32_t) ~ clearWakeupSource);

    /* Read register to make sure it is settled */
    (void) SRSS->PWR_HIBERNATE;
}


#if(0u != SRSS_SIMOBUCK_PRESENT)
    /*******************************************************************************
    * Function Name: Cy_SysPm_SwitchToSimoBuck
    ****************************************************************************//**
    *
    * Switch the power supply regulator to Single-Input Multiple Output (SIMO) 
    * Buck regulator instead of Linear regulator. The SIMO Buck core regulator 
    * provides two outputs using one external inductor and can supply Vccd with 
    * higher efficiency than the Linear core regulator (LDO) under some 
    * conditions, such as high external supply voltage.
    *
    * Before changing from LDO to SIMO Buck, ensure that:
    * * The circuit board has connected Vccbuck1 to Vccd and also populated the 
    * necessary external components for the SIMO Buck regulator, including an 
    * inductor and a capacitor for each output.
    * Refer to the device TRM for more details.
    *
    * * The operating frequency is reduced so it is within the static timing 
    * closure limits at the new frequency.
    * 
    * * The current consumption on the core is reduced to meet the 0.9 V (nominal)
    * output voltage. The main reason is that while changing from LDO to SIMO Buck,
    * the switch happens at 0.9 V (nominal).
    *
    * * The appropriate Wait states values are set for Flash using the 
    * Cy_SysLib_SetWaitStates() function. Refer to the System Library (syslib) 
    * driver for more details.
    *
    * In general, use the next sequence to obtain SIMO Buck 0.9 V (nominal):
    * 1) Set the appropriate Wait states for 0.9 V, by calling 
    * Cy_SysLib_SetWaitStates(true, hfClkFreqMz), where hfClkFreqMz -
    * the desired frequency of HfClk0 in MHz.
    * 
    * 2) Set 0.9 V (nominal) for the LDO regulator.
    *
    * 3) Switch the regulators by calling the Cy_SysPm_SwitchToSimoBuck() function.
    *
    * To obtain SIMO Buck 1.1 V (nominal) use the next sequence:
    * 1) Set the appropriate Wait states for 0.9 V by calling 
    * Cy_SysLib_SetWaitStates(true, hfClkFreqMz), where hfClkFreqMz -
    * the desired frequency of HfClk0 in MHz
    * 
    * 2) Set 0.9 V (nominal) for the LDO regulator.
    *
    * 3) Switch the regulators by calling the Cy_SysPm_SwitchToSimoBuck() function.
    *
    * 4) Change the SIMO Buck output voltage by calling the 
    * Cy_SysPm_SimoBuckSetVoltage1(CY_SYSPM_SIMO_BUCK_OUT1_VOLTAGE_1_1V).
    *
    * 5) Set the appropriate Wait states for LP by calling 
    * Cy_SysLib_SetWaitStates(false, hfClkFreqMz), where hfClkFreqMz -
    * the desired frequency of HfClk0 in MHz
    *
    * \warning The system cannot recognize when Vccd is settled at actual
    * 0.9 V or 1.1 V for transition, because this voltage depends on the conditions 
    * including the load current. The firmware works with the 
    * nominal 0.9 V and 1.1 V voltages.
    *
    * \warning The Flash access time when the core
    * output voltage is 0.9 V (nominal) is longer than at 1.1 V (nominal). 
    * Therefore, the clock to the Flash should be decreased. Use the
    * Cy_SysLib_SetWaitStates() function to set the appropriate Wait state values 
    * for Flash. Call the Cy_SysLib_SetWaitStates() function before 
    * Cy_SysPm_SwitchToSimoBuck() if you want to change the voltage 
    * to 0.9 V (nominal). Call the Cy_SysLib_SetWaitStates() function after 
    * Cy_SysPm_SwitchToSimoBuck() if you change the voltage from 0.9 V (nominal)
    * to 1.1 V (nominal).
    * 
    * \warning There is no way to go back to the Linear regulator after the SIMO 
    * Buck regulator supplies a core. The function switches off the LDO.
    *
    * In addition, if the 0.9 V (nominal) output is set, the Flash works 
    * in the Read-only operation.
    *
    * If the 1.1 V (nominal) output is set, the Flash works in the Read and Write 
    * operation and clock frequencies may be increased up to the static 
    * timing closure limits for a new voltage. 
    * Refer to the device TRM for more details.
    *
    * \note 
    * The function is applicable for devices with the SIMO Buck regulator.
    *
    *******************************************************************************/
    void Cy_SysPm_SwitchToSimoBuck(void)
    {
        /* Disable the DeepSleep, nWell, and Retention regulators */
        SRSS->PWR_CTL |= (_VAL2FLD(SRSS_PWR_CTL_DPSLP_REG_DIS, 1U) |
                          _VAL2FLD(SRSS_PWR_CTL_RET_REG_DIS, 1U) |
                          _VAL2FLD(SRSS_PWR_CTL_NWELL_REG_DIS, 1U));

        /* Configure the SIMO Buck regulator */
        SRSS->PWR_BUCK_CTL = _CLR_SET_FLD32U((SRSS->PWR_BUCK_CTL), 
                                              SRSS_PWR_BUCK_CTL_BUCK_OUT1_SEL, 
                                             (uint32_t) CY_SYSPM_SIMO_BUCK_OUT1_VOLTAGE_0_9V);

        /* Check is the SIMO Buck regulator is already enabled */
        if(!Cy_SysPm_SimoBuckIsEnabled())
        {
            SRSS->PWR_BUCK_CTL |= _VAL2FLD(SRSS_PWR_BUCK_CTL_BUCK_EN, 1U);
        }

        SRSS->PWR_BUCK_CTL |= _VAL2FLD(SRSS_PWR_BUCK_CTL_BUCK_OUT1_EN, 1U);

        /* Wait until SIMO Buck output 1 is stable */
        Cy_SysLib_DelayUs(CY_SYSPM_SIMO_BUCK_CORE_SUPPLY_STABLE_US);

        /* Disable the Linear regulator LDO because Vbuckout1 and LDO are shorted */
        SRSS->PWR_CTL |= _VAL2FLD(SRSS_PWR_CTL_LINREG_DIS, 1U);
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_SimoBuckSetVoltage1
    ****************************************************************************//**
    *
    * Set the output 1 voltage for the SIMO Buck regulator that can supply cores.
    * This output can supply cores instead of the Linear regulator.
    *
    * When changing from a higher voltage to a lower voltage, make sure that:
    * * The operating frequency is reduced so it is within the static timing closure
    * limits at the new frequency.
    *
    * * The appropriate Wait states values are set for Flash using the
    * Cy_SysLib_SetWaitStates() function. Refer to the System Library (syslib) 
    * driver for more details.
    *
    * \warning The Flash access time when the core
    * output voltage is 0.9 V (nominal) is longer than at 1.1 V (nominal). 
    * Therefore, the clock to the Flash should be decreased.
    *
    * In addition, if the 0.9 V (nominal) output is set, the Flash works in 
    * the Read-only operation.
    *
    * When changing from a lower voltage to a higher voltage, make sure that 
    * the appropriate Wait states values are set for Flash using the
    * Cy_SysLib_SetWaitStates() function. Refer to the System Library (syslib) 
    * driver for more details.
    *
    * If the 1.1 V (nominal) output is set, the Flash works in the Read and Write 
    * operations and the clock frequency may now be increased up to the static 
    * timing closure limits for the new voltage.
    *
    * \warning The system cannot recognize when Vccd is settled at the actual
    * 0.9 V or 1.1 V for transition, because this voltage depends on the conditions
    * including load current. The firmware works with the nominal 
    * 0.9 V and 1.1 V voltages.
    *
    * \param voltage
    * The desired output 1 regulator voltage (Vccbuck1).
    * See \ref cy_en_syspm_simo_buck_voltage1_t
    *
    *******************************************************************************/
    void Cy_SysPm_SimoBuckSetVoltage1(cy_en_syspm_simo_buck_voltage1_t voltage)
    {
        CY_ASSERT_L3(CY_SYSPM_IS_SIMO_BUCK_VOLTAGE1_VALID(voltage));
        
        /* Check if the required voltage is equal to the current voltage */
        if(voltage != Cy_SysPm_SimoBuckGetVoltage1())
        {
            /* Set the analog signal bit for the flash before the voltage is 
            * changed from 1.1V to 0.9V
            */
            if(CY_SYSPM_SIMO_BUCK_OUT1_VOLTAGE_0_9V == voltage)
            {
                Cy_SetVoltageBitForFlash(true);
            }

            /* The system may continue operating while the voltage on Vccd 
            * discharges to the new voltage. The time it takes to reach the 
            * new voltage depends on the conditions, including the load current
            * on Vccd and the external capacitor size.
            */
            SRSS->PWR_BUCK_CTL = 
            _CLR_SET_FLD32U((SRSS->PWR_BUCK_CTL), SRSS_PWR_BUCK_CTL_BUCK_OUT1_SEL, (uint32_t) voltage);

            /* Delay to stabilize at the new voltage is required only
            *  when changing from a lower voltage to a higher voltage.
            */
            if(CY_SYSPM_SIMO_BUCK_OUT1_VOLTAGE_1_1V == voltage)
            {
                Cy_SysLib_DelayUs(CY_SYSPM_SIMO_BUCK_IS_STABLE_US);

                /* Set analog signal bit for flash before voltage is changed 
                 * from 0.9 V to 1.1 V.
                 */
                Cy_SetVoltageBitForFlash(false);
            }
        }
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_EnableVoltage2
    ****************************************************************************//**
    *
    * Enable the output 2 voltage (Vbuckrf) of the SIMO Buck regulator. 
    * The output 2 voltage (Vbuckrf) of the SIMO Buck regulator is used to supply 
    * the BLE HW block.
    * When the SIMO Buck regulator is switched off, the function enables the  
    * regulator and after it, enables output 2.
    *
    * \note The function does not affect SIMO Buck output 1 that can supply 
    * a core.
    *
    * \warning The function does not select the SIMO Buck output 2 voltage and 
    * does not set/clear the HW-controlled bit for SIMO Buck output 2. Call
    * Cy_SysPm_SimoBuckSetVoltage2() or Cy_SysPm_SimoBuckSetHwControl() to 
    * configure the SIMO Buck output 2.
    *
    *******************************************************************************/
    void Cy_SysPm_EnableVoltage2(void)
    {
        if(!Cy_SysPm_SimoBuckIsEnabled())
        {
            /* Enable the SIMO Buck regulator */
            SRSS->PWR_BUCK_CTL |= _VAL2FLD(SRSS_PWR_BUCK_CTL_BUCK_EN, 1U);
        }

        /* Enable the SIMO Buck output 2 */
        SRSS->PWR_BUCK_CTL2 |= _VAL2FLD(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_EN, 1U);

        /* Wait until the output is stable */
        Cy_SysLib_DelayUs(CY_SYSPM_SIMO_BUCK_BLE_SUPPLY_STABLE_US);
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_SimoBuckSetVoltage2
    ****************************************************************************//**
    *
    * Set output voltage 2 of the SIMO Buck regulator.
    * 
    * \param voltage
    * The voltage of the SIMO Buck regulator output 2. 
    * See \ref cy_en_syspm_simo_buck_voltage2_t.
    *
    * \param waitToSettle
    * True - Enable the 200 uS delay after setting a higher voltage.
    * False - Disable the 200 uS delay after setting a higher voltage.
    *
    * \warning It is strongly recommended to enable the delay (waitToSettle = true)
    * while changing from a lower voltage to a higher voltage.
    *
    * \note The 200 uS delay is required only under changing from a
    * lower voltage to a higher voltage. Changing from a higher voltage to a lower
    * the delay is not required.
    *
    *******************************************************************************/
    void Cy_SysPm_SimoBuckSetVoltage2(cy_en_syspm_simo_buck_voltage2_t voltage, bool waitToSettle)
    {
        uint32_t curVoltage;

        CY_ASSERT_L3(CY_SYSPM_IS_SIMO_BUCK_VOLTAGE2_VALID(voltage));
        
        /* Get the current voltage */
        curVoltage = (uint32_t) Cy_SysPm_SimoBuckGetVoltage2();

        if((uint32_t) voltage != curVoltage)
        {
            SRSS->PWR_BUCK_CTL2 = 
            _CLR_SET_FLD32U((SRSS->PWR_BUCK_CTL2), SRSS_PWR_BUCK_CTL2_BUCK_OUT2_SEL, (uint32_t) voltage);

            /* Delay to stabilize at the new voltage is required only
            *  when changing from a lower voltage to a higher voltage.
            */
            if(waitToSettle && ((uint32_t) voltage > curVoltage))
            {
                Cy_SysLib_DelayUs(CY_SYSPM_SIMO_BUCK_IS_STABLE_US);
            }
        }
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_SimoBuckOutputIsEnabled
    ****************************************************************************//**
    *
    * Get the current output status of the SIMO Buck outputs.
    *
    * \param output
    * The SIMO Buck regulator output. See \ref cy_en_syspm_buck_out_t.
    *
    * \return
    * The current state of the requested output. True if the requested output 
    * is enabled.
    * False if the requested output is disabled.
    *
    *******************************************************************************/
    bool Cy_SysPm_SimoBuckOutputIsEnabled(cy_en_syspm_buck_out_t output)
    {
        bool retVal;

        CY_ASSERT_L3(CY_SYSPM_IS_SIMO_BUCK_OUTPUT_VALID(output));
        
        if(output == CY_SYSPM_BUCK_VBUCK_1)
        {
            retVal = (_FLD2BOOL(SRSS_PWR_BUCK_CTL_BUCK_OUT1_EN, SRSS->PWR_BUCK_CTL));
        }
        else
        {
            if((0U != _FLD2VAL(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_HW_SEL, SRSS->PWR_BUCK_CTL2)) ||
               (0U != _FLD2VAL(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_EN, SRSS->PWR_BUCK_CTL2)))
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

#endif /* (0u != SRSS_SIMOBUCK_PRESENT) */


/*******************************************************************************
* Function Name: Cy_SysPm_LdoSetVoltage
****************************************************************************//**
*
* Set the voltage on the Linear regulator (LDO).
* When changing from a higher voltage to a lower voltage, make sure that:
* * The operating frequencies are reduced so these are within the static timing 
* closure limits at the new frequency.
*
* * The current consumption on the core is reduced to meet the new output 
* voltage.
*
* * The appropriate Wait states values are set for Flash using the
* Cy_SysLib_SetWaitStates() function. Refer to the System Library (syslib) 
* driver for more details.
*
* \warning NOTE: The Flash access time when the core
* output voltage is 0.9 V (nominal) is longer than at 1.1 V (nominal). 
* Therefore, the clock to the Flash should be decreased.
*
* In addition, if the 0.9 V (nominal) output is set, the Flash works in 
* the Read-only operation.
*
* When changing from a lower voltage to a higher voltage, make sure that 
* the appropriate Wait states values are set for the Flash using the
* Cy_SysLib_SetWaitStates() function. Refer to the System Library (syslib)
* driver for more details.
*
* If the 1.1 V (nominal) output is set, the Flash works in the Read and Write 
* operations. The clock frequency may now be increased up to the static 
* timing closure limits for the new voltage.
*
* \note The actual device Vccd voltage can be different from the 
* nominal voltage because the actual voltage value depends on the conditions 
* including the load current.
*
* \param voltage
* The desired output regulator voltage.
* See \ref cy_en_syspm_ldo_voltage_t voltage
*
*******************************************************************************/
void Cy_SysPm_LdoSetVoltage(cy_en_syspm_ldo_voltage_t voltage)
{
    CY_ASSERT_L3(CY_SYSPM_IS_LDO_VOLTAGE_VALID(voltage));
    
    if(voltage != Cy_SysPm_LdoGetVoltage())
    {
        /* Set the analog signal bit for the flash before the voltage is changed 
        * from 1.1V to 0.9V
        */
        if(CY_SYSPM_LDO_VOLTAGE_0_9V == voltage)
        {
            Cy_SetVoltageBitForFlash(true);
        }

        /* The system may continue operating while the voltage on Vccd 
        *  discharges to the new voltage. The time it takes to reach the 
        *  new voltage depends on the conditions, including the load current on
        *  Vccd and the external capacitor size
        */
        SRSS->PWR_TRIM_PWRSYS_CTL = 
        _CLR_SET_FLD32U((SRSS->PWR_TRIM_PWRSYS_CTL), SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, voltage);

        if(CY_SYSPM_LDO_VOLTAGE_1_1V == voltage)
        {
            /* A delay for the supply to stabilize at the new higher voltage */
            Cy_SysLib_DelayUs(CY_SYSPM_LDO_IS_STABLE_US);

            /* Set the analog signal bit to the flash macro register after 
            * the output voltage is 1.1 V
            */
            Cy_SetVoltageBitForFlash(false);
        }
    }
}


/*******************************************************************************
* Function Name: Cy_SysPm_RegisterCallback
****************************************************************************//**
*
* Registers a new syspm callback.
*
* A callback is a function called after an event in the driver or
* middleware module has occurred. The handler callback API will be executed if
* the specific event occurs. See \ref cy_stc_syspm_callback_t.
*
* \note The registered callbacks are executed in two orders, based on callback 
* mode \ref cy_en_syspm_callback_mode_t. For modes CY_SYSPM_CHECK_READY and 
* CY_SYSPM_BEFORE_TRANSITION the order is next: the first registered callback 
* will be always the first executed. And the last registered callback will be 
* executed as the last callback. For modes CY_SYSPM_AFTER_TRANSITION and 
* CY_SYSPM_CHECK_FAIL the order is next: the first registered callback will be 
* always the last executed. And the last registered callback will be executed 
* as the first callback.
*
* \param handler
* The address of the syspm callback structure.
* See \ref cy_stc_syspm_callback_t.
* \note Do not modify the registered structure in run-time.
*
* \return
* True if a callback was registered; <br>
* False if a callback was not registered or maximum callbacks were registered.
*
* It is allowed to register up to 32 callbacks.
*
*******************************************************************************/
bool Cy_SysPm_RegisterCallback(cy_stc_syspm_callback_t* handler)
{
    uint32_t interruptState;
    bool retStatus = false;

    interruptState = Cy_SysLib_EnterCriticalSection();

    /* Check if the maximum callbacks were registered and verify input */
    retStatus = ((handler != NULL) && (curRegisteredCallbacks < CY_SYSPM_CALLBACKS_NUMBER_MAX));

    if (retStatus)
    {
        if ((handler->callbackParams != NULL) && (handler->callback != NULL))
        {
            cy_stc_syspm_callback_t* curCallback = callbackRoot;
            cy_stc_syspm_callback_t* lastRegCallback = NULL;

            /* Search last registered callback item */
            while (curCallback != NULL)
            {
                if (curCallback == handler)
                {
                    /* Do not register already registered callback item */
                    retStatus = false;
                    break;
                }

                /* Safe callback before switching into the next item */
                lastRegCallback = curCallback;

                curCallback = curCallback->nextItm;
            }

            /* Link requested callback item to the linked list */
            if (retStatus)
            {
                if (callbackRoot == NULL)
                {
                    /* Link first callback item to the linked list */
                    callbackRoot = handler;
                }
                else
                {
                    /* Link requested item to previous item */
                    lastRegCallback->nextItm = handler;
                }

                /* Update links to next and previous callback items of requested
                *  callback item
                */
                handler->prevItm = lastRegCallback;
                handler->nextItm = NULL;

                /* Increment the value with number of registered callbacks */
                ++curRegisteredCallbacks;
            }
        }
        else
        {
            retStatus = false;
        }
    }

    Cy_SysLib_ExitCriticalSection(interruptState);

    return(retStatus);
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
* \param handler The item that should be unregistered.
* See \ref cy_stc_syspm_callback_t.
*
* \return
* True if on success <br>
* False if it was not unregistered or none callbacks were registered.
*
*******************************************************************************/
bool Cy_SysPm_UnregisterCallback(cy_stc_syspm_callback_t const *handler)
{
    uint32_t interruptState;
    bool retStatus = false;
    
    interruptState = Cy_SysLib_EnterCriticalSection();

    /* Check if there was at least one callback registered */
    if (curRegisteredCallbacks > 0UL)
    {
        cy_stc_syspm_callback_t* curCallback = callbackRoot;
        
        /* Search requested callback item in the linked list */
        while (curCallback != NULL)
        {
            /* Requested callback is found */
            if (curCallback == handler)
            {
                retStatus = true;
                break;
            }
            
            /* Go to next callback item in the linked list */
            curCallback = curCallback->nextItm;
        }
        
        if (retStatus)
        {
            /* Requested callback is first in the list */
            if (callbackRoot == handler)
            {
                /* Check is this the only one callback registered */
                if (callbackRoot->nextItm != NULL)
                {
                    callbackRoot = callbackRoot->nextItm;
                    callbackRoot->prevItm = NULL;
                }
            }
            else
            {
                /* Update links of related to unregistered callback items */
                curCallback->prevItm->nextItm = curCallback->nextItm;
                
                if (curCallback->nextItm != NULL)
                {
                    curCallback->nextItm->prevItm = curCallback->prevItm;
                }
            }
            
            /* Decrement the value with number of registered callbacks */
            --curRegisteredCallbacks;
            
            if (curRegisteredCallbacks == 0UL)
            {
                callbackRoot = NULL;
            }
        }
    }

    Cy_SysLib_ExitCriticalSection(interruptState);

    return(retStatus);
}


/*******************************************************************************
* Function Name: Cy_SysPm_ExecuteCallback
****************************************************************************//**
*
* The function executes all registered callbacks with provided type and mode.
* \note The registered callbacks will be executed in order based on lastCallback
* value. The are possible two orders of callbacks execution: <br>
* * From first registered to last registered. Such order is relevant to 
* callbacks with mode CY_SYSPM_CHECK_READY and CY_SYSPM_BEFORE_TRANSITION.
* * Backward flow execution: from last executed callback to the 
* first registered. Such order is relevant to callbacks with mode 
* CY_SYSPM_AFTER_TRANSITION and CY_SYSPM_CHECK_FAIL. Note that, the last
* registered callback function is skipped with mode CY_SYSPM_CHECK_FAIL. This 
* is because callback which returned CY_SYSPM_FAIL already know that it failed.
*
* If no callbacks are registered, returns CY_SYSPM_SUCCESS.
*
* \param type
* The callback type. See \ref cy_en_syspm_callback_type_t.
*
* \param mode
* The callback mode. See \ref cy_en_syspm_callback_mode_t.
*
* \note
* If mode is CY_SYSPM_CHECK_READY or CY_SYSPM_BEFORE_TRANSITION the 
* all required callbacks would be executed in order from first
* registered to last registered.
* If mode is CY_SYSPM_CHECK_FAIL or CY_SYSPM_AFTER_TRANSITION the 
* all required callbacks would be executed in order from last executed callback
* to first registered.
*
* \return
* CY_SYSPM_SUCCESS Callback successfully completed or nor callbacks registered.
* CY_SYSPM_FAIL one of executed callback(s) returned fail.
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_ExecuteCallback(cy_en_syspm_callback_type_t type, cy_en_syspm_callback_mode_t mode)
{
    static cy_stc_syspm_callback_t* lastExecutedCallback = NULL;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;
    cy_stc_syspm_callback_t* curCallback;
    cy_stc_syspm_callback_params_t curParams;
    
    CY_ASSERT_L3(CY_SYSPM_IS_CALLBACK_TYPE_VALID(type));
    CY_ASSERT_L3(CY_SYSPM_IS_CALLBACK_MODE_VALID(mode));
    
   if((mode == CY_SYSPM_BEFORE_TRANSITION) || (mode == CY_SYSPM_CHECK_READY))
    {
        /* Execute registered callbacks with order from first registered to the
        *  last registered. The modes defined in the .skipMode element are not
        *  executed
        */
        curCallback = callbackRoot;
        while((curCallback != NULL) && (retVal != CY_SYSPM_FAIL))
        {
            if((curCallback->type == type) && ((0U == curCallback->skipMode) ||
            (0U == ((uint32_t) mode & curCallback->skipMode))))
            {
                /* Update elements for local callback parameter values */
                curParams.base = curCallback->callbackParams->base;
                curParams.context = curCallback->callbackParams->context;
                curParams.mode = mode;
                
                retVal = curCallback->callback(&curParams);
                
                /* Update callback pointer with value of executed callback. 
                * Such update is required to execute further callbacks in 
                * backward order after exit from low power mode or to undo 
                * configuration after callback returned fail: from last executed
                * to first registered.
                */
                lastExecutedCallback = curCallback;
            }
            curCallback = curCallback->nextItm;
        }
    }
    else
    {
        /* Execute registered callbacks with order from lastCallback to
        * the first registered callback. Such flow is required if previous 
        * callback function returned CY_SYSPM_FAIL or previous callback mode was 
        * CY_SYSPM_BEFORE_TRANSITION. Such order to undo configurations in 
        * correct backward order.
        */
        curCallback = lastExecutedCallback;
        
        /* Skip last executed callback that returned CY_SYSPM_FAIL, as this 
        *  callback already know that it failed
        */
        if ((mode == CY_SYSPM_CHECK_FAIL) && (curCallback != NULL))
        {
            curCallback = curCallback->prevItm;
        }

        /* Execute all registered callback functions with required type and mode */
        while ((curCallback != NULL) && (retVal != CY_SYSPM_FAIL))
        {
            if ((curCallback->type == type) && ((curCallback->skipMode == 0UL) || 
            (((uint32_t) mode & curCallback->skipMode) == 0UL)))
            {
                /* Update elements for local callback parameter values */
                curParams.base = curCallback->callbackParams->base;
                curParams.context = curCallback->callbackParams->context;
                curParams.mode = mode;
                
                retVal = curCallback->callback(&curParams);
            }
            curCallback = curCallback->prevItm;
        }
    }
    return (retVal);
}


/*******************************************************************************
* Function Name: Cy_SysPm_IoFreeze
****************************************************************************//**
*
* Freezes IOs.
*
* Freezes the IO cells directly to save the IO-cell state on a wakeup from the
* Hibernate. Do not call this function before entering the
* Hibernate mode, because Cy_SysPm_Hibernate() function freezes the IO cells.
*
******************************************************************************/
void Cy_SysPm_IoFreeze(void)
{
    uint32_t interruptState;
    uint32_t regValue;
    
    interruptState = Cy_SysLib_EnterCriticalSection();

    /* Check the FREEZE state to avoid a recurrent IO-cells freeze attempt,
    * because the second call to this function will cause an accidental switch
    * to the hibernate mode (the system will enter the hibernate mode immediately
    * after writing to the hibernate bit because both UNLOCK and FREEZE were set
    * correctly in the previous call to this function).
    */
    if(0U == _FLD2VAL(SRSS_PWR_HIBERNATE_FREEZE, SRSS->PWR_HIBERNATE))
    {
        /* Clear the unlock field for correct freeze of the IO cells */
        SRSS->PWR_HIBERNATE = _CLR_SET_FLD32U((SRSS->PWR_HIBERNATE), SRSS_PWR_HIBERNATE_UNLOCK, 0U);
        
        /* Disable overriding by the peripherals the next pin-freeze command */
        SRSS->PWR_HIBERNATE |=  CY_SYSPM_PWR_HIBERNATE_UNLOCK | 
                                _VAL2FLD(SRSS_PWR_HIBERNATE_FREEZE, 1U) |
                                _VAL2FLD(SRSS_PWR_HIBERNATE_HIBERNATE, 1U);

        /* The second Write causes freeze of IO cells to save the IO-cell state */
        regValue = SRSS->PWR_HIBERNATE;
        SRSS->PWR_HIBERNATE = regValue;

        /* Read register to make sure it is settled */
        (void) SRSS->PWR_HIBERNATE;
    }
    Cy_SysLib_ExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Cy_SysPm_IoUnfreeze
****************************************************************************//**
*
* Unfreezes the IOs.
*
* The IO-cells remain frozen after a wakeup from the hibernate mode until the
* firmware unfreezes them after booting. The call of this function unfreezes the
* IO-cells explicitly.
*
* If the firmware needs to retain the data value on the port, then the
* value must be read and re-written to the data register before calling this
* API. Furthermore, the drive mode must be re-programmed. If this is not done,
* the pin state will change to the default state the moment the freeze 
* is removed.
*
*******************************************************************************/
void Cy_SysPm_IoUnfreeze(void)
{
    uint32_t interruptState;
    interruptState = Cy_SysLib_EnterCriticalSection();

    /* Preserve the last reset reason and wakeup polarity. Then, unfreeze I/O:
     * write PWR_HIBERNATE.FREEZE=0, .UNLOCK=0x3A, .HIBERANTE=0,
     */
    SRSS->PWR_HIBERNATE = (SRSS->PWR_HIBERNATE & CY_SYSPM_PWR_RETAIN_HIBERNATE_STATUS) | CY_SYSPM_PWR_HIBERNATE_UNLOCK;

    /* Lock the hibernate mode: 
    * write PWR_HIBERNATE.HIBERNATE=0, UNLOCK=0x00, HIBERANTE=0
    */
    SRSS->PWR_HIBERNATE &= CY_SYSPM_PWR_RETAIN_HIBERNATE_STATUS;
    
    /* Read register to make sure it is settled */
    (void) SRSS->PWR_HIBERNATE;

    Cy_SysLib_ExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Cy_SetVoltageBitForFlash
****************************************************************************//**
*
* The internal function that changes the Vcc setting for the flash.
* 
* \param setBit
* Set/clear the bit for the flash macro register. This bit should be set (true)
* when the voltage for the core regulators is less than 0.99 V. This bit should 
* be set if the output voltage for the core regulators is higher than 0.99 V.
*
*******************************************************************************/
static void Cy_SetVoltageBitForFlash(bool setBit)
{
    if(setBit)
    {
        FLASHC_FM_CTL->ANA_CTL0 |= _VAL2FLD(FLASHC_FM_CTL_ANA_CTL0_VCC_SEL, 1U);
    }
    else
    {
        FLASHC_FM_CTL->ANA_CTL0 &= ((uint32_t) (~_VAL2FLD(FLASHC_FM_CTL_ANA_CTL0_VCC_SEL, 1U)));
    }
}

#ifdef CY_IP_MXUDB

    /*******************************************************************************
    * Function Name: Cy_SaveRegisters
    ****************************************************************************//**
    *
    * The internal function saves non-retained registers before entering Deep Sleep.
    * Cypress ID #280370.
    *
    * \param regs
    * The structure where the registers are saved before Deep Sleep
    *
    *******************************************************************************/
    static void Cy_SaveRegisters(cy_stc_syspm_backup_regs_t *regs)
    {
        /* Save the registers before deep sleep */
        regs->CY_UDB_UDBIF_BANK_CTL_REG = UDB->UDBIF.BANK_CTL;

        regs->CY_UDB_BCTL_MDCLK_EN_REG = UDB->BCTL.MDCLK_EN;
        regs->CY_UDB_BCTL_MBCLK_EN_REG = UDB->BCTL.MBCLK_EN;
        regs->CY_UDB_BCTL_BOTSEL_L_REG = UDB->BCTL.BOTSEL_L;
        regs->CY_UDB_BCTL_BOTSEL_U_REG = UDB->BCTL.BOTSEL_U;
        regs->CY_UDB_BCTL_QCLK_EN0_REG = UDB->BCTL.QCLK_EN[0U];
        regs->CY_UDB_BCTL_QCLK_EN1_REG = UDB->BCTL.QCLK_EN[1U];
        regs->CY_UDB_BCTL_QCLK_EN2_REG = UDB->BCTL.QCLK_EN[2U];
    }
    
    
    /*******************************************************************************
    * Function Name: Cy_RestoreRegisters
    ****************************************************************************//**
    *
    * The internal function restores the non-retained registers after 
    * leaving the Deep Sleep power mode. Cypress ID #280370.
    *
    * \param regs
    * The structure with data stored into the required non-retained registers
    * after Deep Sleep.
    *
    *******************************************************************************/
    static void Cy_RestoreRegisters(cy_stc_syspm_backup_regs_t const *regs)
    {
        /* Restore the registers after deep sleep */
        UDB->BCTL.MDCLK_EN = regs->CY_UDB_BCTL_MDCLK_EN_REG;
        UDB->BCTL.MBCLK_EN = regs->CY_UDB_BCTL_MBCLK_EN_REG;
        UDB->BCTL.BOTSEL_L = regs->CY_UDB_BCTL_BOTSEL_L_REG;
        UDB->BCTL.BOTSEL_U = regs->CY_UDB_BCTL_BOTSEL_U_REG;
        UDB->BCTL.QCLK_EN[0U] = regs->CY_UDB_BCTL_QCLK_EN0_REG;
        UDB->BCTL.QCLK_EN[1U] = regs->CY_UDB_BCTL_QCLK_EN1_REG;
        UDB->BCTL.QCLK_EN[2U] = regs->CY_UDB_BCTL_QCLK_EN2_REG;

        UDB->UDBIF.BANK_CTL = regs->CY_UDB_UDBIF_BANK_CTL_REG;
    }
#endif /* CY_IP_MXUDB */


/*******************************************************************************
* Function Name: Cy_EnterDeepSleep
****************************************************************************//**
*
* The internal function that prepares the system for Deep Sleep and 
* restores the system after a wakeup from Deep Sleep.
*
* \param waitFor Selects wait for action. See \ref cy_en_syspm_waitfor_t.
*
*******************************************************************************/
#if defined (__ICCARM__)
    __ramfunc 
#else
    CY_SECTION(".cy_ramfunc")
#endif
static void Cy_EnterDeepSleep(cy_en_syspm_waitfor_t waitFor)
{
    /* Aquire the IPC to prevent changing of the shared resources at the same time */
    while(0U == _FLD2VAL(IPC_STRUCT_ACQUIRE_SUCCESS, CY_SYSPM_IPC_STC->ACQUIRE))
    {
        /* Wait until the IPC structure is released by another core */
    }

    /* Set the flag that current core went to Deep Sleep */
    CY_SYSPM_IPC_STC->DATA |= CY_SYSPM_CUR_CORE_DP_MASK;

    /* Change the slow and fast clock dividers only under condition that 
    *  the other core is already in Deep Sleep. Cypress ID #284516
    */
    if (0U != (CY_SYSPM_IPC_STC->DATA & CY_SYSPM_OTHER_CORE_DP_MASK))
    {
        /* Get the divider values of the slow and high clocks and store them into 
        *  the IPC data register
        */
        CY_SYSPM_IPC_STC->DATA = 
        (CY_SYSPM_IPC_STC->DATA & ((uint32_t) ~(CY_SYSPM_FAST_CLK_DIV_Msk | CY_SYSPM_SLOW_CLK_DIV_Msk))) |
        (((uint32_t) (_FLD2VAL(CPUSS_CM0_CLOCK_CTL_SLOW_INT_DIV, CPUSS->CM0_CLOCK_CTL) << CY_SYSPM_SLOW_CLK_DIV_Pos)) |
         ((uint32_t) (_FLD2VAL(CPUSS_CM4_CLOCK_CTL_FAST_INT_DIV, CPUSS->CM4_CLOCK_CTL) << CY_SYSPM_FAST_CLK_DIV_Pos)));
        
        /* Increase the clock divider for the slow and fast clocks to CY_SYSPM_CLK_DIVIDER */
        CPUSS->CM0_CLOCK_CTL = 
        _CLR_SET_FLD32U(CPUSS->CM0_CLOCK_CTL, CPUSS_CM0_CLOCK_CTL_SLOW_INT_DIV, CY_SYSPM_CLK_DIVIDER);

        CPUSS->CM4_CLOCK_CTL = 
        _CLR_SET_FLD32U(CPUSS->CM4_CLOCK_CTL, CPUSS_CM4_CLOCK_CTL_FAST_INT_DIV, CY_SYSPM_CLK_DIVIDER);

        /* Read the divider value to make sure it is set */
        (void) CPUSS->CM0_CLOCK_CTL;
        (void) CPUSS->CM4_CLOCK_CTL;
    }

    /* Release the IPC */
    CY_SYSPM_IPC_STC->RELEASE = 0U;

    /* Read this register to make sure it is settled */
    (void) CY_SYSPM_IPC_STC->RELEASE;

#if(0u != CY_CPU_CORTEX_M0P)

    /* The CPU enters the deep sleep mode upon execution of WFI/WFE */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    if(waitFor != CY_SYSPM_WAIT_FOR_EVENT)
    {
        __WFI();
    }
    else
    {
        __WFE();
    }
#else

    /* Repeat the WFI/WFE instructions if a wake up was not intended. 
    *  Cypress ID #272909
    */
    do
    {
        /* The CPU enters Deep Sleep mode upon execution of WFI/WFE */
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

        if(waitFor != CY_SYSPM_WAIT_FOR_EVENT)
        {
            __WFI();
        }
        else
        {
            __WFE();

            /* Call the WFE instructions twice to clear the Event register 
            *  of the CM4 core. Cypress ID #279077
            */
            if(wasEventSent)
            {
                __WFE();
            }

            wasEventSent = true;
        }
    } while (_FLD2VAL(CPUSS_CM4_PWR_CTL_PWR_MODE, CPUSS->CM4_PWR_CTL) == CY_SYSPM_CM4_PWR_RETAINED);

#endif /* (0u != CY_CPU_CORTEX_M0P) */

    /* Aquire the IPC to prevent changing of the shared resources at the same time */
    while(0U == _FLD2VAL(IPC_STRUCT_ACQUIRE_SUCCESS, CY_SYSPM_IPC_STC->ACQUIRE))
    {
        /* Wait until the IPC structure is released by another core */
    }

    /* Read and change the slow and fast clock dividers only under condition 
    * that the other core is already in Deep Sleep. Cypress ID #284516
    */
    if(0U != (CY_SYSPM_IPC_STC->DATA & CY_SYSPM_OTHER_CORE_DP_MASK))
    {
        /* Restore the clock dividers for the slow and fast clocks */
        CPUSS->CM0_CLOCK_CTL = 
        (_CLR_SET_FLD32U(CPUSS->CM0_CLOCK_CTL, CPUSS_CM0_CLOCK_CTL_SLOW_INT_DIV, 
                        (_FLD2VAL(CY_SYSPM_SLOW_CLK_DIV, CY_SYSPM_IPC_STC->DATA))));

        CPUSS->CM4_CLOCK_CTL = 
        (_CLR_SET_FLD32U(CPUSS->CM4_CLOCK_CTL, CPUSS_CM4_CLOCK_CTL_FAST_INT_DIV, 
                        (_FLD2VAL(CY_SYSPM_FAST_CLK_DIV, CY_SYSPM_IPC_STC->DATA))));
    }

    /* Indicate that the current core is out of Deep Sleep */
    CY_SYSPM_IPC_STC->DATA &= ((uint32_t) ~CY_SYSPM_CUR_CORE_DP_MASK);

    /* Release the IPC */
    CY_SYSPM_IPC_STC->RELEASE = 0U;
}
/* [] END OF FILE */
