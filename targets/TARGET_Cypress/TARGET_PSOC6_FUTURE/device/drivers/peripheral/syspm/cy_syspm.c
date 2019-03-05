/***************************************************************************//**
* \file cy_syspm.c
* \version 2.10
*
* This driver provides the source code for API power management.
*
********************************************************************************
* \copyright
* Copyright 2016-2018, Cypress Semiconductor Corporation.  All rights reserved.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#include "cy_syspm.h"

/*******************************************************************************
*       Internal Variables
*******************************************************************************/
static cy_stc_syspm_callback_t* callbackRoot = NULL;
static cy_stc_syspm_callback_t* callbackListLast = NULL;
static uint32_t curRegisteredCallbacks = 0U;

#if(0u != CY_CPU_CORTEX_M4)
    static bool wasEventSent = false;
#endif /* (0u != CY_CPU_CORTEX_M4) */


/*******************************************************************************
*       Internal Functions
*******************************************************************************/
#ifdef CY_IP_MXUDB
    static void SaveRegisters(cy_stc_syspm_backup_regs_t *regs);
    static void RestoreRegisters(cy_stc_syspm_backup_regs_t const *regs);
#endif /* CY_IP_MXUDB */

static void EnterDeepSleep(cy_en_syspm_waitfor_t waitFor);

static void SetReadMarginTrimUlp(void);
static void SetReadMarginTrimLp(void);
static void SetWriteAssistTrimUlp(void);
static void SetWriteAssistTrimLp(void);

static void SetVoltageBitForFlash(void);
static void ClearVoltageBitForFlash(void);

#if defined(CY_DEVICE_PSOC6ABLE2) && !defined(CY_PSOC6ABLE2_REV_0A_SUPPORT_DISABLE)
    static void Cy_EnterDeepSleep(cy_en_syspm_waitfor_t waitFor);
#endif /* defined(CY_DEVICE_PSOC6ABLE2) && !defined(CY_PSOC6ABLE2_REV_0A_SUPPORT_DISABLE) */


/*******************************************************************************
*       Internal Defines
*******************************************************************************/
#if defined(CY_DEVICE_PSOC6ABLE2) && !defined(CY_PSOC6ABLE2_REV_0A_SUPPORT_DISABLE)

    /** The internal define for clock divider */
    #define SYSPM_CLK_DIVIDER                 (9U)

    /* Mask for the fast clock divider value */
    #define SYSPM_FAST_CLK_DIV_Msk            (0xFF000000UL)

    /* Position for the fast clock divider value */
    #define SYSPM_FAST_CLK_DIV_Pos            (24UL)

    /* Mask for the slow clock divider value */
    #define SYSPM_SLOW_CLK_DIV_Msk            (0x00FF0000UL)

    /* Position for the slow clock divider value */
    #define SYSPM_SLOW_CLK_DIV_Pos            (16UL)

    #if(0u != CY_CPU_CORTEX_M4)
        #define CUR_CORE_DP_MASK        (0x01UL)
        #define OTHER_CORE_DP_MASK      (0x02UL)
    #else
        #define CUR_CORE_DP_MASK        (0x02UL)
        #define OTHER_CORE_DP_MASK      (0x01UL)
    #endif

#endif /* defined(CY_DEVICE_PSOC6ABLE2) && !defined(CY_PSOC6ABLE2_REV_0A_SUPPORT_DISABLE) */

/* Redefine for sflash region */
#define DELAY_DONE_FLAG                 FLASHC

/* Slow control register */
#define TST_DDFT_SLOW_CTL_REG          (*(volatile uint32_t *) 0x40260108U)

/* Slow output register */
#define CLK_OUTPUT_SLOW_REG            (*(volatile uint32_t *) 0x40260518U)

/* Fast control register */
#define TST_DDFT_FAST_CTL_REG          (*(volatile uint32_t *) 0x40260104U)

/* Counter register */
#define CLK_CAL_CNT1_REG               (*(volatile uint32_t *) 0x4026051CU)

#ifdef CY_IP_MXUDB

    /* The UDB placement on MMIO slave level */
    #define PERI_UDB_SLAVE_ENABLED     ((uint32_t) (1UL << CY_MMIO_UDB_SLAVE_NR))
#endif /* CY_IP_MXUDB */

/** The definition for the delay of the LDO after its output
* voltage is changed
*/
#define LDO_STABILIZATION_DELAY_US      (9U)

/* Define for the IPC structure used by syspm driver */
#define SYSPM_IPC_STC                  IPC_STRUCT7

/** Define to indicate that a 10 us delay is needed */
#define NEED_DELAY                     (0x0U)

/** Define to set the IMO to count a 10 us delay after exiting Deep Sleep */
#define TST_DDFT_SLOW_CTL_MASK         (0x00001F1EU)

/** Slow output register */
#define CLK_OUTPUT_SLOW_MASK           (0x06U)

/** Slow control register */
#define TST_DDFT_FAST_CTL_MASK         (62U)

/** Load value for the timer to count delay after exiting Deep Sleep */
#define IMO_10US_DELAY                 (68U)

/** Define to indicate that the clock is finished counting */
#define CLK_CAL_CNT1_DONE              ((uint32_t) ((uint32_t) 1U << CLK_CAL_CNT1_DONE_POS))

/** Define to indicate that the clock is finished counting */
#define CLK_CAL_CNT1_DONE_POS          (31U)

/** Define to indicate that a 10 us delay was done after exiting Deep Sleep */
#define DELAY_DONE                     (0xAAAAAAAAU)

/** Define for transitional 0.95 V for the LDO regulator */
#define LDO_OUT_VOLTAGE_0_95V                  (0x0BU)

/** Define for transitional 1.1 V for the LDO regulator */
#define LDO_OUT_VOLTAGE_1_1V                   (0x17U)

/** Define for transitional 1.15 V for the LDO regulator */
#define LDO_OUT_VOLTAGE_1_15V                  (0x1BU)

#if(0u != SRSS_BUCKCTL_PRESENT)

    /** Define for transitional 0.95 V for buck regulator */
    #define BUCK_OUT1_VOLTAGE_0_95V             (3U)
#endif /* (0u != SRSS_BUCKCTL_PRESENT) */

/* These defines will be removed and SFLASH registers used instead */

/** Trim define for ROM in LP mode   */
#define CPUSS_TRIM_ROM_LP                (0x00000013U)

/** Trim define for RAM in LP mode */
#define CPUSS_TRIM_RAM_LP                (0x00004013U)

/** Trim define for ROM in ULP mode   */
#define CPUSS_TRIM_ROM_ULP               (0x00000012U)

/** Trim define for RAM in ULP mode */
#define CPUSS_TRIM_RAM_ULP               (0x00006012U)


/*******************************************************************************
* Function Name: Cy_SysPm_ReadStatus
****************************************************************************//**
*
* Reads the status of the core(s).
*
* \return The current power mode. See \ref group_syspm_return_status.
*
* \funcusage
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_ReadStatus
*
*******************************************************************************/
uint32_t Cy_SysPm_ReadStatus(void)
{
    uint32_t interruptState;
    uint32_t pmStatus = 0U;
    interruptState = Cy_SysLib_EnterCriticalSection();

#if(0u != CY_IP_M4CPUSS)

    /* Check whether CM4 is in Deep Sleep mode*/
    if((0U != _FLD2VAL(CPUSS_CM4_STATUS_SLEEPING, CPUSS->CM4_STATUS)) &&
       (0U != _FLD2VAL(CPUSS_CM4_STATUS_SLEEPDEEP, CPUSS->CM4_STATUS)))
    {
        pmStatus |= CY_SYSPM_STATUS_CM4_DEEPSLEEP;
    }
    /* Check whether CM4 is in Sleep mode*/
    else if(0U != _FLD2VAL(CPUSS_CM4_STATUS_SLEEPING, CPUSS->CM4_STATUS))
    {
        pmStatus |= CY_SYSPM_STATUS_CM4_SLEEP;
    }
        else
        {
            pmStatus |= CY_SYSPM_STATUS_CM4_ACTIVE;
        }
#endif /* (0u != CY_IP_M4CPUSS) */

    /* Check whether CM0p is in Deep Sleep mode*/
    if((0U != _FLD2VAL(CPUSS_CM0_STATUS_SLEEPING, CPUSS->CM0_STATUS)) &&
       (0U != _FLD2VAL(CPUSS_CM0_STATUS_SLEEPDEEP, CPUSS->CM0_STATUS)))
    {
        pmStatus |= (uint32_t) CY_SYSPM_STATUS_CM0_DEEPSLEEP;
    }
    /* Check whether CM0p is in Sleep mode*/
    else if (0U != _FLD2VAL(CPUSS_CM0_STATUS_SLEEPING, CPUSS->CM0_STATUS))
    {
        pmStatus |= CY_SYSPM_STATUS_CM0_SLEEP;
    }
    else
    {
        pmStatus |= CY_SYSPM_STATUS_CM0_ACTIVE;
    }

    /* Check whether the device is in Low Power mode by reading
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
* Sets a CPU core to Sleep mode.
*
* Puts the core into Sleep power mode, if none of callback functions were
* registered.
*
* For more details about switching into Sleep power mode and debug,
* refer to the device TRM.
*
* If at least one callback function with the CY_SYSPM_SLEEP type was registered,
* the next algorithm is executed:
* Prior to entering Sleep mode, all callback functions of the CY_SYSPM_SLEEP
* type with the CY_SYSPM_CHECK_READY parameter are called. This allows the driver
* to signal whether it is ready to enter the Low Power mode. If any of the
* callbacks of the CY_SYSPM_SLEEP type with the CY_SYSPM_CHECK_READY parameter
* returns CY_SYSPM_FAIL, the remaining callback of the CY_SYSPM_SLEEP type with
* the CY_SYSPM_CHECK_READY parameter calls are skipped.
* After CY_SYSPM_FAIL, all the CY_SYSPM_SLEEP callbacks with
* the CY_SYSPM_CHECK_FAIL parameter are executed. These are the callbacks
* of the CY_SYSPM_SLEEP type with the CY_SYSPM_CHECK_READY
* parameter that were previously executed before getting CY_SYSPM_FAIL.
* The Sleep mode is not entered and the Cy_SysPm_Sleep() function returns
* CY_SYSPM_FAIL.
*
* If all of the callbacks of the CY_SYSPM_SLEEP type with the
* CY_SYSPM_CHECK_READY parameter calls return CY_SYSPM_SUCCESS, then all
* callbacks of the CY_SYSPM_SLEEP type with the CY_SYSPM_CHECK_FAIL parameters
* calls are skipped. Also, all callbacks of the CY_SYSPM_SLEEP type and
* CY_SYSPM_BEFORE_TRANSITION parameter calls are executed, allowing the
* peripherals to prepare for Sleep. The CPU then enters Sleep mode.
* This is a CPU-centric power mode. This means that the CPU has entered Sleep
* mode and its main clock is removed. It is identical to Active from a
* peripheral point of view. Any enabled interrupt can cause a wakeup from
* Sleep mode.
*
* After a wakeup from Sleep, all of the registered callbacks of the
* CY_SYSPM_SLEEP type and with the CY_SYSPM_AFTER_TRANSITION parameter are
* executed to return the peripherals to Active operation. The Cy_SysPm_Sleep()
* function returns CY_SYSPM_SUCCESS.
* No callbacks of the CY_SYSPM_SLEEP type with the CY_SYSPM_BEFORE_TRANSITION
* parameter or callbacks of the CY_SYSPM_SLEEP type and
* CY_SYSPM_AFTER_TRANSITION parameter callbacks are executed if Sleep mode
* is not entered.
*
* \note The last callback that returned CY_SYSPM_FAIL is not executed with the
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
* \funcusage
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_Sleep
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

        /* The CPU enters the Sleep power mode upon execution of WFI/WFE */
        SCB->SCR = _CLR_SET_FLD32U((SCB->SCR), SCB_SCR_SLEEPDEEP, 0U);

        if(waitFor != CY_SYSPM_WAIT_FOR_EVENT)
        {
            __WFI();
        }
        else
        {
            __WFE();

        #if(0u != CY_CPU_CORTEX_M4)

            /* For the CM4 core, the WFE instructions are called twice.
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
* Sets a CPU core to the Deep Sleep mode.
*
* Puts the core into the Deep Sleep power mode. Prior to entering the Deep Sleep
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
* The Deep Sleep mode is not entered and the Cy_SysPm_DeepSleep() function
* returns CY_SYSPM_FAIL.
*
* If all callbacks of the CY_SYSPM_DEEPSLEEP type with the CY_SYSPM_CHECK_READY
* parameter calls return CY_SYSPM_SUCCESS, then all callbacks of the
* CY_SYSPM_DEEPSLEEP type with the CY_SYSPM_CHECK_FAIL parameter calls are
* skipped and all callbacks of the CY_SYSPM_DEEPSLEEP type with the
* CY_SYSPM_BEFORE_TRANSITION parameter calls are executed, allowing the
* peripherals to prepare for Deep Sleep.
* The Deep Sleep mode is then entered. Any enabled interrupt can cause a wakeup
* from the Deep Sleep mode.
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
* Sleep mode instead and automatically enter Deep Sleep mode when the
* system is ready.
*
* The system puts the whole device into Deep Sleep mode when all the
* processor(s) is (are) in Deep Sleep, there are no busy peripherals, the
* debugger is not active, and the Deep Sleep circuits are
* ready (PWR_CONTROL.LPM_READY=1).
*
* The peripherals that do not need a clock or that receive a clock from their
* external interface (e.g. I2C/SPI) continue operating. All circuits using the
* current from Vccdpslp supply are under the current limitation, which is
* controlled by the Deep Sleep regulator.
*
* Wakeup occurs when an interrupt asserts from a Deep Sleep active peripheral.
* For more details, see the corresponding peripheral's datasheet.
*
* \note
* For multi-core devices, the second core, which did not participate in
* device wakeup, continues to execute the Deep Sleep instructions. Any Deep Sleep
* capable interrupt routed to this core can wake it.
*
* For more details about switching into the Deep Sleep power mode and debug,
* refer to the device TRM.
*
* A normal wakeup from the Deep Sleep power mode returns to either LPActive or
* Active, depending on the previous state and programmed behavior for the
* particular wakeup interrupt.
*
* After wakeup from Deep Sleep, all of the registered callbacks with
* CY_SYSPM_DEEPSLEEP type with CY_SYSPM_AFTER_TRANSITION are executed to return
* peripherals to Active operation. The Cy_SysPm_DeepSleep() function returns
* CY_SYSPM_SUCCESS. No callbacks are executed with CY_SYSPM_DEEPSLEEP type with
* CY_SYSPM_BEFORE_TRANSITION or CY_SYSPM_AFTER_TRANSITION parameter, if
* Deep Sleep mode was not entered.
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
* are restored by the values saved on the first core. After the second core
* wakeup, these registers are "reconfigured" by the values saved on the second
* core.
* Be aware of this situation.
*
* \sideeffect
* This function clears the Event Register of CM4 core after wakeup from WFE.
*
* \sideeffect
* This side effect is applicable only for rev-08 of the CY8CKIT-062.
* The function changes the slow and fast clock dividers to
* SYSPM_CLK_DIVIDER right before entering into Deep Sleep and restores
* these dividers after wakeup.
*
* \return
* Entered status, see \ref cy_en_syspm_status_t.
*
* \note
* The FLL/PLL are not restored right before the CPU starts executing the
* instructions after Deep Sleep. This can affect the peripheral which is
* driven by PLL/FLL. Ensure that the PLL/FLL were properly restored (locked)
* after the wakeup from Deep Sleep. Refer to the
* \ref group_sysclk driver documentation driver for the information how to
* read the PLL/FLL lock statuses.
*
* \funcusage
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_DeepSleep
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

    /* The device (core) can switch into the Deep Sleep power mode only when
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

        /* Check whether the UDB disabled on MMIO level */
        if (0UL != (PERI->GR[CY_MMIO_UDB_GROUP_NR].SL_CTL & PERI_UDB_SLAVE_ENABLED))
        {

            /* Save non-retained registers */
            SaveRegisters(&regs);
        }
    #endif /* CY_IP_MXUDB */

    #if defined(CY_DEVICE_PSOC6ABLE2) && !defined(CY_PSOC6ABLE2_REV_0A_SUPPORT_DISABLE)

        if (CY_SYSLIB_DEVICE_REV_0A == (uint32_t) Cy_SysLib_GetDeviceRevision())
        {
            Cy_EnterDeepSleep(waitFor);
        }
        else
    #endif /* defined(CY_DEVICE_PSOC6ABLE2) && !defined(CY_PSOC6ABLE2_REV_0A_SUPPORT_DISABLE) */
        {
            cy_en_syspm_ldo_voltage_t curLdoVoltage;

            curLdoVoltage = Cy_SysPm_LdoGetVoltage();

            /* Configure additional wakeup delay from Deep Sleep
            *  for 1.1 V LDO. Cypress ID #290172.
            */
            if ((Cy_SysPm_LdoIsEnabled()) && (CY_SYSPM_LDO_VOLTAGE_1_1V == curLdoVoltage))
            {
                SRSS->PWR_TRIM_WAKE_CTL = CY_SYSPM_SFLASH->PWR_TRIM_WAKE_CTL;
            }
            else
            {
                SRSS->PWR_TRIM_WAKE_CTL = 0UL;
            }

            /* Set the core into Deep Sleep */
            EnterDeepSleep(waitFor);
        }

    #ifdef CY_IP_MXUDB

        /* Do not restore the UDB if it is disabled on MMIO level */
        if (0UL != (PERI->GR[CY_MMIO_UDB_GROUP_NR].SL_CTL & PERI_UDB_SLAVE_ENABLED))
        {
            /* Restore non-retained registers */
            RestoreRegisters(&regs);
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
* Puts the core into the Hibernate power mode. Prior to entering Hibernate
* mode, all callbacks of the CY_SYSPM_HIBERNATE type are executed.
* First, callbacks of the CY_SYSPM_HIBERNATE type and with
* CY_SYSPM_CHECK_READY parameter are called, allowing the driver to signal if it
* is not ready to enter the power mode. If any of the callbacks of the
* CY_SYSPM_HIBERNATE type with the CY_SYSPM_CHECK_READY parameter call returns
* CY_SYSPM_FAIL, the remaining CY_SYSPM_HIBERNATE callbacks with the
* CY_SYSPM_CHECK_READY parameter calls are skipped. After CY_SYSPM_FAIL, all
* of the CY_SYSPM_HIBERNATE callbacks with CY_SYSPM_CHECK_FAIL parameter are
* executed that correspond to the CY_SYSPM_HIBERNATE callbacks with
* CY_SYSPM_CHECK_READY parameter calls that occurred up to the point of failure.
* Hibernate mode is not entered and the Cy_SysPm_Hibernate() function
* returns CY_SYSPM_FAIL.
*
* If all CY_SYSPM_HIBERNATE callbacks with the CY_SYSPM_CHECK_READY parameter
* calls return CY_SYSPM_SUCCESS, then all CY_SYSPM_HIBERNATE callbacks with
* CY_SYSPM_CHECK_FAIL calls are skipped and all CY_SYSPM_HIBERNATE callbacks
* CY_SYSPM_BEFORE_TRANSITION parameter calls are executed allowing the
* peripherals to prepare for Hibernate. The I/O output state is frozen and
* Hibernate mode is then entered. In Hibernate mode, all internal supplies
* are off and no internal state is retained. There is no handshake with the
* CPUs and the chip will enter Hibernate immediately.
*
* The I/O output state is frozen and Hibernate mode is then
* entered. In Hibernate mode, all internal supplies are off and no
* internal state is retained.
* For multi-core devices there is no handshake with the CPUs and the chip
* will enter Hibernate power mode immediately.
*
* \note The last callback that returned CY_SYSPM_FAIL is not executed with the
* CY_SYSPM_CHECK_FAIL parameter because of the FAIL.
*
* The return values from executed callback functions with the
* CY_SYSPM_CHECK_FAIL, CY_SYSPM_BEFORE_TRANSITION, and CY_SYSPM_AFTER_TRANSITION
* modes are ignored.
*
* A wakeup from Hibernate is triggered by toggling the wakeup pin(s), a WDT
* match, or back-up domain alarm expiration, depending on how the they were
* configured. A wakeup causes a normal boot procedure.
* To configure the wakeup pin(s), a Digital Input Pin must be configured, and
* resistively pulled up or down to the inverse state of the wakeup polarity. To
* distinguish a wakeup from Hibernate mode and a general reset event, the
* Cy_SysLib_GetResetReason() function can be used. The wakeup pin and low-power
* comparators are active-low by default. The wakeup pin or the LPComparators
* polarity can be changed with the \ref Cy_SysPm_SetHibernateWakeupSource() function.
* This function call will not return if Hibernate mode is entered.
* The CY_SYSPM_HIBERNATE callbacks with the CY_SYSPM_AFTER_TRANSITION parameter
* are never executed.
*
* This function freezes the I/O cells implicitly. Entering
* Hibernate mode before freezing the I/O cells is not possible. The I/O cells remain frozen
* after waking from Hibernate mode until the firmware unfreezes them
* with a \ref Cy_SysPm_IoUnfreeze() function call.
*
* \return
* Entered status, see \ref cy_en_syspm_status_t.
*
* \funcusage
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_Hibernate
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

        /* Freeze I/O-Cells to save I/O-Cell state */
        Cy_SysPm_IoFreeze();

        SRSS->PWR_HIBERNATE |= CY_SYSPM_PWR_SET_HIBERNATE;

        /* Read register to make sure it is settled */
        (void) SRSS->PWR_HIBERNATE;

        /* Wait for transition */
        __WFI();

        /* The callback functions calls with the CY_SYSPM_AFTER_TRANSITION
        * parameter in the Hibernate power mode are not applicable as device
        * wake-up was made on device reboot.
        */

        /* A wakeup from Hibernate is performed by toggling of the wakeup
        * pins, or WDT matches, or Backup domain alarm expires. This depends on what
        * item is configured in the hibernate register. After a wakeup event, a
        * normal Boot procedure occurs.
        * There is no need to exit from the critical section.
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
* Function Name: Cy_SysPm_EnterLowPowerMode
****************************************************************************//**
*
* This function switches only the supply regulators into Low Power mode.
* You must configure
* clocks and/or peripherals to meet current load limitations in LP Active.
* For more details about power modes and current load limitations refer to
* the device technical reference manual (TRM).
*
* The LPActive mode is similar to the Active mode.
* The difference is that the current is limited and some functions have limited
* features/performance.
*
* The key feature of the Low Power mode is the limited current. Restrictions are
* placed on the clock frequencies and allow the peripherals to achieve
* a current limit.
*
* Before entering Low Power mode, you must configure the system so
* the total current drawn from Vccd is less that the value presented in the
* technical reference manual (TRM). Refer to the TRM for the maximum load for
* low power operation and clock limitations in Low Power mode with different
* core supply regulator voltages.
*
* * Peripherals can use the knowledge of the LPActive mode to make
* trade-offs that consume less current. For more details, see the corresponding
* peripherals' datasheet.
* * High-speed clock sources are available with the appropriate pre-divider
* settings to limit the system current.
* Refer to the TRM for the maximum frequency values for low power operation
* using different Core Regulators' output voltages.
*
* This function puts the device into Low Power mode. Prior to entering Low Power mode,
* all the registered CY_SYSPM_ENTER_LOWPOWER_MODE callbacks with CY_SYSPM_CHECK_READY
* parameter are called. This allows the driver to signal if it is not ready to
* enter Low Power mode. If any CY_SYSPM_ENTER_LOWPOWER_MODE callbacks with the
* CY_SYSPM_CHECK_READY parameter call returns CY_SYSPM_FAIL, the remaining
* CY_SYSPM_ENTER_LOWPOWER_MODE callbacks with the CY_SYSPM_CHECK_READY parameter
* calls are skipped.
*
* After a CY_SYSPM_FAIL, all of the CY_SYSPM_ENTER_LOWPOWER_MODE callbacks with
* CY_SYSPM_CHECK_FAIL parameter are executed that correspond to the
* CY_SYSPM_ENTER_LOWPOWER_MODE callbacks with CY_SYSPM_CHECK_READY parameter calls
* that occurred up to the point of failure. Low Power mode is not entered and
* the Cy_SysPm_EnterLowPowerMode() function returns CY_SYSPM_FAIL.
*
* If all CY_SYSPM_ENTER_LOWPOWER_MODE callbacks with the CY_SYSPM_CHECK_READY
* parameter calls return CY_SYSPM_SUCCESS, then all CY_SYSPM_ENTER_LOWPOWER_MODE
* callbacks with CY_SYSPM_CHECK_FAIL calls are skipped and all
* CY_SYSPM_ENTER_LOWPOWER_MODE callbacks with the CY_SYSPM_BEFORE_TRANSITION parameter
* calls are executed. This allows the peripherals to prepare for low power.
* Low Power mode is then entered.
*
* After entering Low Power mode, all of the registered
* CY_SYSPM_ENTER_LOWPOWER_MODE callbacks with the CY_SYSPM_AFTER_TRANSITION parameter
* are executed to complete preparing the peripherals for low power operation.
* The Cy_SysPm_EnterLowPowerMode() function returns CY_SYSPM_SUCCESS.
* No CY_SYSPM_ENTER_LOWPOWER_MODE callbacks with the CY_SYSPM_BEFORE_TRANSITION or
* CY_SYSPM_AFTER_TRANSITION parameter are executed, if Low Power mode is not
* entered.
*
* \note The last callback that returned CY_SYSPM_FAIL is not executed with
* the CY_SYSPM_CHECK_FAIL parameter because of the FAIL.
*
* The return values from executed callback functions with the
* CY_SYSPM_CHECK_FAIL, CY_SYSPM_BEFORE_TRANSITION, and CY_SYSPM_AFTER_TRANSITION
* modes are ignored.
*
* \note The callbacks are not executed if the device is already not in
* Low Power mode.
*
* \return
* See \ref cy_en_syspm_status_t. <br>
* CY_SYSPM_SUCCESS - Entered the LPActive mode or the device is already
* in LPActive.<br>
* CY_SYSPM_FAIL - The LPActive mode is not entered or low power circuits
* are not ready to enter Low Power mode.
*
* \funcusage
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_EnterLowPowerMode
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_EnterLowPowerMode(void)
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
                retVal = Cy_SysPm_ExecuteCallback(CY_SYSPM_ENTER_LOWPOWER_MODE, CY_SYSPM_CHECK_READY);
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
                    (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_ENTER_LOWPOWER_MODE, CY_SYSPM_BEFORE_TRANSITION);
                }

                /* Configure the low-power operating mode for LDO regulator */
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
                    /* Configure the low-power operating mode for Buck regulator */
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
                    (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_ENTER_LOWPOWER_MODE, CY_SYSPM_AFTER_TRANSITION);
                }
            }
            else
            {
                /* Execute callback functions with the CY_SYSPM_CHECK_FAIL parameter to
                * undo everything done in the callback with the CY_SYSPM_CHECK_READY
                * parameter. The return value should be CY_SYSPM_SUCCESS.
                */
                (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_ENTER_LOWPOWER_MODE, CY_SYSPM_CHECK_FAIL);
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
        /* Do nothing because the device is already in Low Power mode. */
    }
    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_ExitLowPowerMode
****************************************************************************//**
*
* Exits the device from Low Power mode.
*
* Returns the device to the Active mode. In the Active power mode, the operating
* current can be increased to the normal mode limit. The clock frequencies also
* can be increased to the normal mode limit. Refer to the device TRM for the
* current and frequency limitations in the Active power mode.
*
* Prior to exiting Low Power mode, all the registered CY_SYSPM_EXIT_LOWPOWER_MODE
* callbacks with the CY_SYSPM_CHECK_READY parameter are called. This allows
* the driver to signal if it is not ready to exit
* Low Power mode. If any CY_SYSPM_EXIT_LOWPOWER_MODE callbacks with
* the CY_SYSPM_CHECK_READY parameter call returns CY_SYSPM_FAIL, the remaining
* CY_SYSPM_EXIT_LOWPOWER_MODE callbacks with the CY_SYSPM_CHECK_READY parameter calls
* are skipped. After a CY_SYSPM_FAIL, all of the CY_SYSPM_EXIT_LOWPOWER_MODE callbacks
* with CY_SYSPM_CHECK_FAIL parameter are executed that correspond to the
* CY_SYSPM_EXIT_LOWPOWER_MODE callbacks with CY_SYSPM_CHECK_READY parameter calls that
* occurred up to the point of failure. Active mode is not entered and the
* Cy_SysPm_ExitLowPowerMode() function returns CY_SYSPM_FAIL.
*
* If all CY_SYSPM_EXIT_LOWPOWER_MODE callbacks with CY_SYSPM_CHECK_READY calls return
* CY_SYSPM_SUCCESS, then all the CY_SYSPM_EXIT_LOWPOWER_MODE callbacks with
* the CY_SYSPM_CHECK_FAIL parameter calls are skipped and all
* CY_SYSPM_EXIT_LOWPOWER_MODE callbacks with the CY_SYSPM_BEFORE_TRANSITION parameter
* calls are executed allowing the peripherals to prepare for Active mode.
* Low Power mode is then exited.
*
* After exiting Low Power mode, all of the registered callbacks that have
* type CY_SYSPM_EXIT_LOWPOWER_MODE are executed with the CY_SYSPM_AFTER_TRANSITION
* parameter to complete preparing the peripherals for Active mode operation.
* The Cy_SysPm_ExitLowPowerMode() function returns CY_SYSPM_SUCCESS.
* No CY_SYSPM_EXIT_LOWPOWER_MODE callbacks with the CY_SYSPM_BEFORE_TRANSITION or
* CY_SYSPM_AFTER_TRANSITION parameter are executed if Low Power mode is
* not exited.
*
* \note The last callback that returned CY_SYSPM_FAIL is not executed with the
* CY_SYSPM_CHECK_FAIL parameter because of the FAIL.
*
* The return values from executed callback functions with the
* CY_SYSPM_CHECK_FAIL, CY_SYSPM_BEFORE_TRANSITION, and CY_SYSPM_AFTER_TRANSITION
* modes are ignored.
*
* \note The callbacks are not executed if the device is not already in Low
* Power mode.
*
* \return
* See \ref cy_en_syspm_status_t. <br>
* CY_SYSPM_SUCCESS - Exited from the LPActive power mode, or the device is
* already in Active mode. <br>
* CY_SYSPM_FAIL - Exit from the LPActive mode is not done.
*
* \warning This function blocks as it waits until Active Reference is ready
* to enter to the normal mode.
*
* \funcusage
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_ExitLowPowerMode
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_ExitLowPowerMode(void)
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
            retVal = Cy_SysPm_ExecuteCallback(CY_SYSPM_EXIT_LOWPOWER_MODE, CY_SYSPM_CHECK_READY);
        }

        /* The device (core) can switch into the Low Power mode only in the
        * condition that all executed registered callback functions with the
        * CY_SYSPM_CHECK_READY parameter return CY_SYSPM_SUCCESS.
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
                (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_EXIT_LOWPOWER_MODE, CY_SYSPM_BEFORE_TRANSITION);
            }

            /* Set the normal operation mode for LDO regulator if
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
                    (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_EXIT_LOWPOWER_MODE, CY_SYSPM_AFTER_TRANSITION);
                }
            }

        }
        else
        {
            /* Execute callback functions with the CY_SYSPM_CHECK_FAIL parameter to
            * undo everything done in the callback with the CY_SYSPM_CHECK_READY
            * parameter. The return value should be CY_SYSPM_SUCCESS.
            */
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_EXIT_LOWPOWER_MODE, CY_SYSPM_CHECK_FAIL);
            retVal = CY_SYSPM_FAIL;
        }
    }
    else
    {
        /* Do nothing because the device is already in the Active power mode */
    }
    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_SleepOnExit
****************************************************************************//**
*
* This function configures the Sleep-on-exit feature of the core.
*
* This API sets the SLEEPONEXIT bit of the SCR register.
*
* When the Sleep-on-exit feature is enabled (SLEEPONEXIT bit is set),
* the core wakes up to service the interrupt and then immediately goes
* back to sleep. Because of this, the unstacking process is not carried out, so
* this feature is useful for the interrupt driven application and helps to
* reduce the unnecessary stack push and pop operations.
* The core does not go to sleep if the interrupt handler returns to
* another interrupt handler (nested interrupt).
* You can use this feature in applications that require only the core to run
* when an interrupt occurs.
*
* When the Sleep-on-exit feature is disabled (SLEEPONEXIT bit is cleared),
* the core returns back to the main thread after servicing the interrupt
* without going back to sleep.
*
* Refer to the ARM documentation about the Sleep-on-exit feature and
* SLEEPONEXIT of the SCR register.
*
* \param enable
* true - enable Sleep-on-exit feature <br> false - disable
* Sleep-on-exit feature.
*
* \funcusage
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_SleepOnExit
*
*******************************************************************************/
void Cy_SysPm_SleepOnExit(bool enable)
{
    uint32_t interruptState;
    interruptState = Cy_SysLib_EnterCriticalSection();

    if(enable)
    {
        /* Enable Sleep-on-exit mode */
        SCB->SCR |= _VAL2FLD(SCB_SCR_SLEEPONEXIT, 1U);
    }
    else
    {
        /* Disable Sleep-on-exit mode */
        SCB->SCR &= ((uint32_t) ~(SCB_SCR_SLEEPONEXIT_Msk));
    }
    Cy_SysLib_ExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Cy_SysPm_SetHibernateWakeupSource
****************************************************************************//**
*
* This function configures sources to wake up the device from the Hibernate
* power mode. Such sources can be wakeup pins, LPComparators, Watchdog (WDT)
* interrupt, or a Real-Time clock (RTC) alarm (interrupt).
*
* Wakeup pins:
*
* A wakeup is supported by up to two pins with programmable polarity. These pins
* may be connected to the I/O pins or on-chip peripherals under some conditions.
* Setting the wakeup pin to this level will cause a wakeup from Hibernate
* mode. The wakeup pins are active/low by default.
*
* LPComparators:
*
* A wakeup is supported by up to two LPComps with programmable polarity. These
* LPComp may be connected to the I/O pins or on-chip peripherals under some
* conditions.
* Setting the LPComp to this level will cause a wakeup from Hibernate
* mode. The wakeup LPComp are active-low by default.
*
* \note The low-power comparators should be configured and enabled before
* switching into the hibernate low power mode. Refer to the LPComp
* driver description for more details.
*
* Watchdog Timer:
*
* \note The WDT should be configured and enabled before entering into the
* Hibernate power mode.
*
* A wakeup is performed by a WDT interrupt and a normal boot procedure
* after a device reset. The device can wake up from Hibernate after a WDT
* device reset, if the WDT was configured to wake up, the device on its
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
* the Hibernate power mode, see \ref cy_en_syspm_hibernate_wakeup_source_t.
* The input parameters values can be ORed. For example, if you want to set
* LPComp0 (active high) and WDT, call this function:
* Cy_SysPm_SetHibernateWakeupSource(CY_SYSPM_HIBERNATE_LPCOMP0_HIGH | CY_SYSPM_HIBERNATE_WDT).
*
* \warning Do not call this function with different polarity levels for the same
* wakeup source. For example, do not call a function like this:
* Cy_SysPm_SetHibernateWakeupSource(CY_SYSPM_HIBERNATE_LPCOMP0_LOW, CY_SYSPM_HIBERNATE_LPCOMP0_HIGH);
*
* \funcusage
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_SetHibernateWakeupSource
*
*******************************************************************************/
void Cy_SysPm_SetHibernateWakeupSource(uint32_t wakeupSource)
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
* Function Name: Cy_SysPm_ClearHibernateWakeupSource
****************************************************************************//**
*
* This function disables a wakeup source that was previously configured to
* wake up the device from the hibernate power mode.
*
* \param wakeupSource
* For the source to be disabled, see \ref cy_en_syspm_hibernate_wakeup_source_t.
* The input parameters values can be ORed. For example, if you want to disable
* LPComp0 (active high) and WDT call this function:
* Cy_SysPm_ClearHibernateWakeupSource(CY_SYSPM_HIBERNATE_LPCOMP0_HIGH | CY_SYSPM_HIBERNATE_WDT).
*
* \funcusage
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_ClearHibernateWakeupSource
*
*******************************************************************************/
void Cy_SysPm_ClearHibernateWakeupSource(uint32_t wakeupSource)
{
    uint32_t clearWakeupSource;

    CY_ASSERT_L3(CY_SYSPM_IS_WAKE_UP_SOURCE_VALID(wakeupSource));

    if (0U != _FLD2VAL(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, wakeupSource))
    {
        /* Clear the high active level of the requested sources */
        if((uint32_t) CY_SYSPM_HIBERNATE_LPCOMP0_HIGH == ((uint32_t) wakeupSource & (uint32_t) CY_SYSPM_HIBERNATE_LPCOMP0_HIGH))
        {
            SRSS->PWR_HIBERNATE &=
            ((uint32_t) ~_VAL2FLD(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, CY_SYSPM_WAKEUP_LPCOMP0_BIT));
        }

        if((uint32_t) CY_SYSPM_HIBERNATE_LPCOMP1_HIGH == ((uint32_t) wakeupSource & (uint32_t) CY_SYSPM_HIBERNATE_LPCOMP1_HIGH))
        {
            SRSS->PWR_HIBERNATE &=
            ((uint32_t) ~_VAL2FLD(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, CY_SYSPM_WAKEUP_LPCOMP1_BIT));
        }

        if((uint32_t) CY_SYSPM_HIBERNATE_PIN0_HIGH == ((uint32_t) wakeupSource & (uint32_t) CY_SYSPM_HIBERNATE_PIN0_HIGH))
        {
            SRSS->PWR_HIBERNATE &=
            ((uint32_t) ~_VAL2FLD(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, CY_SYSPM_WAKEUP_PIN0_BIT));
        }

        if((uint32_t) CY_SYSPM_HIBERNATE_PIN1_HIGH == ((uint32_t) wakeupSource & (uint32_t) CY_SYSPM_HIBERNATE_PIN1_HIGH))
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


#if(0u != SRSS_BUCKCTL_PRESENT)
    /*******************************************************************************
    * Function Name: Cy_SysPm_BuckEnable
    ****************************************************************************//**
    *
    * Switch the power supply regulator to Buck regulator instead of the LDO.
    * The Buck core regulator provides output voltage(s) using one external
    * inductor and can supply Vccd with higher efficiency than the LDO under some
    * conditions, such as high external supply voltage.
    *
    * Before changing from LDO to Buck, ensure that the circuit board has
    * connected Vccbuck1 to Vccd and also populated the
    * necessary external components for the Buck regulator, including an
    * inductor and a capacitor for each output.
    * Refer to the device TRM for more details.
    *
    * When changing from a higher voltage to a lower voltage
    * (from LDO 1.1V to Buck 0.9V), ensure that:
    * * The device maximum operating frequency for all the Clk_HF paths, peripheral,
    * and slow clock are under the ULP limitations.
    * * The total current consumption is under the ULP limitations.
    *
    * * The appropriate wait states values are set for the flash using
    * the Cy_SysLib_SetWaitStates() function as explained below.
    *
    * <b>Setting wait states values for flash</b>
    *
    * The flash access time when the core output voltage is 0.9 V (nominal) is
    * longer than at 1.1 V (nominal). Therefore, the number of the wait states must
    * be adjusted. Use the Cy_SysLib_SetWaitStates() function to set the appropriate
    * wait state values for flash.
    *
    * To change from a higher voltage (LDO 1.1 V) to a lower voltage (Buck 0.9 V),
    * call the Cy_SysLib_SetWaitStates(true, hfClkFreqMz) function before changing
    * the voltage, where hfClkFreqMz is the frequency of HfClk0 in MHz.
    *
    * To change from a lower voltage (LDO 0.9 V) to a higher voltage (Buck 1.1 V),
    * calling the Cy_SysLib_SetWaitStates(false, hfClkFreqMz) function is to set
    * the wait states is optional, but can be done to improve the performance.
    * The clock frequency may now be increased up to LP mode for a new voltage.
    *
    * \note 1. The final Buck output is set to 0.9 V (nominal) - the flash works
    * in the Read-only operation.
    * \note 2. The final Buck output is set to 1.1 V (nominal) - the flash works
    * in the Read and Write operations.
    * \note 3. The actual device Vccd voltage can be different from the nominal
    * voltage because the actual voltage value depends on the conditions
    * including the load current.
    *
    * \warning There is no way to go back to the LDO after the
    * Buck regulator supplies a core. The function switches off the LDO.
    *
    * For more details refer to the \ref group_syspm_managing_core_regulators
    * section.
    * Refer to the \ref group_syslib driver for more details about setting the wait
    * states.
    *
    * \note
    * The function is applicable for devices with the Buck regulator.
    *
    * \funcusage
    * \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_BuckEnable
    *
    *******************************************************************************/
    void Cy_SysPm_BuckEnable(cy_en_syspm_buck_voltage1_t voltage)
    {
        CY_ASSERT_L3(CY_SYSPM_IS_BUCK_VOLTAGE1_VALID(voltage));

        uint32_t interruptState;
        cy_en_syspm_ldo_voltage_t curLdoVoltage;

        curLdoVoltage = Cy_SysPm_LdoGetVoltage();
        interruptState = Cy_SysLib_EnterCriticalSection();

        /* When the LDO is 1.1V and final target Buck 0.9V need to update the
        *  RAM and ROM trim values
        */
        if ((CY_SYSPM_BUCK_OUT1_VOLTAGE_0_9V == voltage) && (CY_SYSPM_LDO_VOLTAGE_1_1V == curLdoVoltage))
        {
            /* Set the analog signal bit for the flash before the voltage is
            * changed from 1.1V to 0.9V
            */
            SetVoltageBitForFlash();

            /* Update read-write margin value for the ULP mode */
            SetReadMarginTrimUlp();

            /* Reduce LDO output voltage to 0.95 V nominal */
            SRSS->PWR_TRIM_PWRSYS_CTL =
            _CLR_SET_FLD32U((SRSS->PWR_TRIM_PWRSYS_CTL), SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, LDO_OUT_VOLTAGE_0_95V);

            /* Update write assist value for the ULP mode */
            SetWriteAssistTrimUlp();
        }
        /* When the LDO is 0.9V and final target Buck 1.1 V need to update the
        *  RAM and ROM trim values
        */
        else if ((CY_SYSPM_BUCK_OUT1_VOLTAGE_1_1V == voltage) && (CY_SYSPM_LDO_VOLTAGE_0_9V == curLdoVoltage))
        {
            /* Increase LDO to 0.95 V */
            SRSS->PWR_TRIM_PWRSYS_CTL =
            _CLR_SET_FLD32U((SRSS->PWR_TRIM_PWRSYS_CTL), SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, LDO_OUT_VOLTAGE_0_95V);

            /* Wait until regulator is stable */
            Cy_SysLib_DelayUs(LDO_STABILIZATION_DELAY_US);

            /* Update write assist value for the LP mode */
            SetWriteAssistTrimLp();

            /* Increase LDO to 1.1 V */
            SRSS->PWR_TRIM_PWRSYS_CTL =
            _CLR_SET_FLD32U((SRSS->PWR_TRIM_PWRSYS_CTL), SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, LDO_OUT_VOLTAGE_1_1V);

            /* Wait until regulator is stable */
            Cy_SysLib_DelayUs(LDO_STABILIZATION_DELAY_US);

            /* Update read-write margin value for the LP mode */
            SetReadMarginTrimLp();

            /* Set the LDO 1.15V as final Buck output is 1.1 V */
            SRSS->PWR_TRIM_PWRSYS_CTL =
            _CLR_SET_FLD32U((SRSS->PWR_TRIM_PWRSYS_CTL), SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, LDO_OUT_VOLTAGE_1_15V);

            /* Clear the analog signal bit for the flash before the voltage is
            * changed from 1.1 V to 0.9 V
            */
            ClearVoltageBitForFlash();
        }
        /* If LDO is 0.9V and final Buck is 0.9V increase the LDO on 50 mV*/
        else if ((CY_SYSPM_BUCK_OUT1_VOLTAGE_0_9V == voltage) && (CY_SYSPM_LDO_VOLTAGE_0_9V == curLdoVoltage))
        {
            SRSS->PWR_TRIM_PWRSYS_CTL =
            _CLR_SET_FLD32U((SRSS->PWR_TRIM_PWRSYS_CTL), SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, LDO_OUT_VOLTAGE_0_95V);
        }
        /* If LDO is 1.1V and final Buck is 1.1 V increase the LDO on 50 mV*/
        else
        {
            SRSS->PWR_TRIM_PWRSYS_CTL =
            _CLR_SET_FLD32U((SRSS->PWR_TRIM_PWRSYS_CTL), SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, LDO_OUT_VOLTAGE_1_15V);
        }

        /* A delay for the supply to stabilize at the new higher voltage */
        Cy_SysLib_DelayUs(LDO_STABILIZATION_DELAY_US);

        /* Disable the Deep Sleep, nWell, and Retention regulators */
        SRSS->PWR_CTL |= (_VAL2FLD(SRSS_PWR_CTL_DPSLP_REG_DIS, 1U) |
                          _VAL2FLD(SRSS_PWR_CTL_RET_REG_DIS, 1U) |
                          _VAL2FLD(SRSS_PWR_CTL_NWELL_REG_DIS, 1U));

        /* Configure the Buck regulator */
        SRSS->PWR_BUCK_CTL =
        _CLR_SET_FLD32U((SRSS->PWR_BUCK_CTL), SRSS_PWR_BUCK_CTL_BUCK_OUT1_SEL, (uint32_t) voltage);

        /* Check whether the Buck regulator is already enabled */
        if(!Cy_SysPm_BuckIsEnabled())
        {
            SRSS->PWR_BUCK_CTL |= _VAL2FLD(SRSS_PWR_BUCK_CTL_BUCK_EN, 1U);
        }

        SRSS->PWR_BUCK_CTL |= _VAL2FLD(SRSS_PWR_BUCK_CTL_BUCK_OUT1_EN, 1U);

        /* Wait until Buck output 1 is stable */
        Cy_SysLib_DelayUs(CY_SYSPM_BUCK_CORE_SUPPLY_STABLE_US);

        /* Disable the LDO, because Vbuckout1 and LDO are shorted */
        SRSS->PWR_CTL |= _VAL2FLD(SRSS_PWR_CTL_LINREG_DIS, 1U);

        Cy_SysLib_ExitCriticalSection(interruptState);
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_BuckSetVoltage1
    ****************************************************************************//**
    *
    * Sets the output 1 voltage for the Buck regulator that can supply core(s).
    * This output can supply cores instead of the LDO.
    *
    * When changing from a higher voltage to a lower voltage, ensure that:
    * * The device maximum operating frequency for all the Clk_HF paths, peripheral,
    * and slow clock are under the \ref group_syspm_ulp_limitations.
    * * The total current consumption is under the \ref group_syspm_ulp_limitations.
    *
    * * The appropriate wait states values are set for the flash using
    * the Cy_SysLib_SetWaitStates() function as explained below.
    *
    * <b>Setting wait states values for flash</b>
    *
    * The flash access time when the core output voltage is 0.9 V (nominal) is
    * longer than at 1.1 V (nominal). Therefore, the number of the wait states must
    * be adjusted. Use the Cy_SysLib_SetWaitStates() function to set the appropriate
    * wait state values for flash.
    *
    * To change from a higher voltage to a lower voltage 0.9 V (nominal),
    * call the Cy_SysLib_SetWaitStates(true, hfClkFreqMz) function before changing
    * the voltage, where hfClkFreqMz is the frequency of HfClk0 in MHz.
    *
    * To change from a lower voltage to a higher voltage 1.1 V (nominal), calling
    * the Cy_SysLib_SetWaitStates(false, hfClkFreqMz) function is to set the
    * wait states is optional, but can be done to improve the performance.
    * The clock frequency may now be increased up to
    * \ref group_syspm_lp_limitations for a new voltage.
    *
    * \note 1. The output is set to 0.9 V (nominal) - the flash works in the
    * Read-only operation.
    * \note 2. The output is set to 1.1 V (nominal) - the flash works in the Read
    * and Write operations.
    * \note 3. The actual device Vccd voltage can be different from the nominal
    * voltage because the actual voltage value depends on the conditions
    * including the load current.
    *
    * For more details refer to the \ref group_syspm_managing_core_regulators
    * section.
    * Refer to the \ref group_syslib driver for more details about setting the
    * wait states.
    *
    * \param voltage
    * The desired output 1 regulator voltage (Vccbuck1).
    * See \ref cy_en_syspm_buck_voltage1_t
    *
    * \funcusage
    * \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_VoltageRegulator
    *
    *******************************************************************************/
    void Cy_SysPm_BuckSetVoltage1(cy_en_syspm_buck_voltage1_t voltage)
    {
        CY_ASSERT_L3(CY_SYSPM_IS_BUCK_VOLTAGE1_VALID(voltage));

        uint32_t interruptState;
        interruptState = Cy_SysLib_EnterCriticalSection();

        /* Check whether the required voltage is equal to the current voltage */
        if (voltage != Cy_SysPm_BuckGetVoltage1())
        {

            if (CY_SYSPM_BUCK_OUT1_VOLTAGE_0_9V == voltage)
            {
                /* Set the analog signal bit for the flash before the voltage is
                * changed from 1.1 V to 0.9 V
                */
                SetVoltageBitForFlash();

                /* Update read-write margin value for the ULP mode */
                SetReadMarginTrimUlp();

                /* Reduce Buck output voltage to 0.95V nominal */
                SRSS->PWR_BUCK_CTL =
                _CLR_SET_FLD32U((SRSS->PWR_BUCK_CTL), SRSS_PWR_BUCK_CTL_BUCK_OUT1_SEL, BUCK_OUT1_VOLTAGE_0_95V);

                /* Update write assist value for the ULP mode */
                SetWriteAssistTrimUlp();
            }
            else
            {
                /* Increase Buck output voltage to 0.95 V nominal */
                SRSS->PWR_BUCK_CTL =
                _CLR_SET_FLD32U((SRSS->PWR_BUCK_CTL), SRSS_PWR_BUCK_CTL_BUCK_OUT1_SEL, BUCK_OUT1_VOLTAGE_0_95V);

                /* Wait until regulator is stable */
                Cy_SysLib_DelayUs(BUCK_STABILIZATION_DELAY_US);

                /* Update write assist value for the LP mode */
                SetWriteAssistTrimLp();
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
            if(CY_SYSPM_BUCK_OUT1_VOLTAGE_1_1V == voltage)
            {
                Cy_SysLib_DelayUs(BUCK_STABILIZATION_DELAY_US);

                /* Update read-write margin value for the LP mode */
                SetReadMarginTrimLp();

                /* Set analog signal bit for flash before voltage is changed
                 * from 0.9 V to 1.1 V.
                 */
                ClearVoltageBitForFlash();
            }
        }
        Cy_SysLib_ExitCriticalSection(interruptState);
    }


    /*******************************************************************************
    * Function Name: Cy_SysPm_BuckIsOutputEnabled
    ****************************************************************************//**
    *
    * This function gets the current output status of the Buck outputs.
    *
    * \param output
    * The Buck regulator output. See \ref cy_en_syspm_buck_out_t.
    *
    * \return
    * The current state of the requested output. True if the requested output
    * is enabled.
    * False if the requested output is disabled.
    *
    * \funcusage
    * \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_BuckIsOutputEnabled
    *
    *******************************************************************************/
    bool Cy_SysPm_BuckIsOutputEnabled(cy_en_syspm_buck_out_t output)
    {
        CY_ASSERT_L3(CY_SYSPM_IS_BUCK_OUTPUT_VALID(output));

        bool retVal = false;

        if (output == CY_SYSPM_BUCK_VBUCK_1)
        {
            retVal = (_FLD2BOOL(SRSS_PWR_BUCK_CTL_BUCK_OUT1_EN, SRSS->PWR_BUCK_CTL));
        }

    #if (0u != SRSS_SIMOBUCK_PRESENT)
        if(output == CY_SYSPM_BUCK_VRF)
        {
            retVal = ((0U != _FLD2VAL(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_HW_SEL, SRSS->PWR_BUCK_CTL2)) ||
                      (0U != _FLD2VAL(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_EN, SRSS->PWR_BUCK_CTL2)));
        }
    #endif /* (0u != SRSS_SIMOBUCK_PRESENT) */

        return(retVal);
    }

    #if(0u != SRSS_SIMOBUCK_PRESENT)
        /*******************************************************************************
        * Function Name: Cy_SysPm_BuckEnableVoltage2
        ****************************************************************************//**
        *
        * Enable the output 2 voltage (Vbuckrf) of the SIMO Buck regulator.
        * The output 2 voltage (Vbuckrf) of the Buck regulator is used to supply
        * the BLE HW block.
        * When the Buck regulator is switched off, the function enables the
        * regulator and after it, enables output 2.
        *
        * \note The function does not affect Buck output 1 that can supply
        * a core.
        *
        * \warning The function does not select the Buck output 2 voltage and
        * does not set/clear the HW-controlled bit for Buck output 2. Call
        * Cy_SysPm_BuckSetVoltage2() or Cy_SysPm_BuckSetVoltage2HwControl() to
        * configure the Buck output 2.
        *
        * The function is applicable for devices with the SIMO Buck regulator.
        * Refer to device datasheet about information if device contains
        * SIMO Buck.
        *
        * \funcusage
        * \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_BuckEnableVoltage2
        *
        *******************************************************************************/
        void Cy_SysPm_BuckEnableVoltage2(void)
        {
            if(!Cy_SysPm_BuckIsEnabled())
            {
                /* Enable the SIMO Buck regulator */
                SRSS->PWR_BUCK_CTL |= _VAL2FLD(SRSS_PWR_BUCK_CTL_BUCK_EN, 1U);
            }

            /* Enable the SIMO Buck output 2 */
            SRSS->PWR_BUCK_CTL2 |= _VAL2FLD(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_EN, 1U);

            /* Wait until the output is stable */
            Cy_SysLib_DelayUs(CY_SYSPM_BUCK_BLE_SUPPLY_STABLE_US);
        }


        /*******************************************************************************
        * Function Name: Cy_SysPm_BuckSetVoltage2
        ****************************************************************************//**
        *
        * This function sets output voltage 2 of the SIMO Buck regulator.
        *
        * \param voltage
        * The voltage of the Buck regulator output 2.
        * See \ref cy_en_syspm_buck_voltage2_t.
        *
        * \param waitToSettle
        * True - Enable the 200 us delay after setting a higher voltage.
        * False - Disable the 200 us delay after setting a higher voltage.
        *
        * \warning You must enable the delay (waitToSettle = true)
        * while changing from a lower voltage to a higher voltage.
        *
        * \note The 200 us delay is required only when changing from a
        * lower voltage to a higher voltage. Changing from a higher voltage to a lower one,
        * the delay is not required.
        *
        * The function is applicable for devices with the SIMO Buck regulator.
        * Refer to device datasheet about information if device contains
        * SIMO Buck.
        *
        * \funcusage
        * \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_BuckSetVoltage2
        *
        *******************************************************************************/
        void Cy_SysPm_BuckSetVoltage2(cy_en_syspm_buck_voltage2_t voltage, bool waitToSettle)
        {
            uint32_t curVoltage;

            CY_ASSERT_L3(CY_SYSPM_IS_BUCK_VOLTAGE2_VALID(voltage));

            /* Get the current voltage */
            curVoltage = (uint32_t) Cy_SysPm_BuckGetVoltage2();

            if((uint32_t) voltage != curVoltage)
            {
                SRSS->PWR_BUCK_CTL2 =
                _CLR_SET_FLD32U((SRSS->PWR_BUCK_CTL2), SRSS_PWR_BUCK_CTL2_BUCK_OUT2_SEL, (uint32_t) voltage);

                /* Delay to stabilize at the new voltage is required only
                *  when changing from a lower voltage to a higher voltage.
                */
                if(waitToSettle && ((uint32_t) voltage > curVoltage))
                {
                    Cy_SysLib_DelayUs(BUCK_STABILIZATION_DELAY_US);
                }
            }
        }
    #endif /* (0u != SRSS_SIMOBUCK_PRESENT) */
#endif /* (0u != SRSS_BUCKCTL_PRESENT) */


/*******************************************************************************
* Function Name: Cy_SysPm_LdoSetVoltage
****************************************************************************//**
*
* Set an output voltage on the LDO.
*
* When changing from a higher voltage to a lower voltage, ensure that:
* * The device maximum operating frequency for all the Clk_HF paths, peripheral,
* and slow clock are under the \ref group_syspm_ulp_limitations.
* * The total current consumption is under the \ref group_syspm_ulp_limitations.
* * The appropriate wait states values are set for the flash using
* the Cy_SysLib_SetWaitStates() function as explained below.
*
* <b>Setting wait states values for Flash</b>
*
* The flash access time when the core output voltage is 0.9 V (nominal) is
* longer than at 1.1 V (nominal). Therefore, the number of the wait states must
* be adjusted. Use the Cy_SysLib_SetWaitStates() function to set the appropriate
* wait state values for flash.
*
* To change from a higher voltage to a lower voltage 0.9 V (nominal),
* call the Cy_SysLib_SetWaitStates(true, hfClkFreqMz) function before changing
* the voltage, where hfClkFreqMz is the frequency of HfClk0 in MHz.
*
* To change from a lower voltage to a higher voltage 1.1 V (nominal), calling
* the Cy_SysLib_SetWaitStates(false, hfClkFreqMz) function is to set the
* wait states is optional, but can be done to improve the performance.
* The clock frequency may now be increased up to \ref group_syspm_lp_limitations
* for a new voltage.
*
* \note 1. The output is set to 0.9 V (nominal) - the flash works in the
* Read-only operation.
* \note 2. The output is set to 1.1 V (nominal) - the flash works in the Read
* and Write operations.
* \note 3. The actual device Vccd voltage can be different from the nominal
* voltage because the actual voltage value depends on the conditions
* including the load current.
*
* For more details refer to the \ref group_syspm_managing_core_regulators
* section.
* Refer to the \ref group_syslib driver for more details about setting the wait
* states.
*
* \param voltage
* The desired output regulator voltage.
* See \ref cy_en_syspm_ldo_voltage_t voltage
*
* \funcusage
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_VoltageRegulator
*
*******************************************************************************/
void Cy_SysPm_LdoSetVoltage(cy_en_syspm_ldo_voltage_t voltage)
{
    CY_ASSERT_L3(CY_SYSPM_IS_LDO_VOLTAGE_VALID(voltage));

    uint32_t interruptState;
    interruptState = Cy_SysLib_EnterCriticalSection();

    if (Cy_SysPm_LdoGetVoltage() != voltage)
    {
        uint32_t trimVoltage;

        if (CY_SYSPM_LDO_VOLTAGE_0_9V == voltage)
        {
            /* Set the analog signal bit for the flash before the voltage is changed
            * from 1.1 V to 0.9 V. Store trimmed voltage value into the local variable
            */
            SetVoltageBitForFlash();

            /* Update read-write margin value for the ULP mode */
            SetReadMarginTrimUlp();

            SRSS->PWR_TRIM_PWRSYS_CTL =
            _CLR_SET_FLD32U((SRSS->PWR_TRIM_PWRSYS_CTL), SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, LDO_OUT_VOLTAGE_0_95V);

            /* Update write assist value for the ULP mode */
             SetWriteAssistTrimUlp();

            trimVoltage = SFLASH->LDO_0P9V_TRIM;
        }
        else
        {
            SRSS->PWR_TRIM_PWRSYS_CTL =
            _CLR_SET_FLD32U((SRSS->PWR_TRIM_PWRSYS_CTL), SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, LDO_OUT_VOLTAGE_0_95V);

            /* A delay for the supply to stabilize at the new higher voltage */
            Cy_SysLib_DelayUs(LDO_STABILIZATION_DELAY_US);

            /* Update write assist value for the LP mode */
             SetWriteAssistTrimLp();

            trimVoltage = SFLASH->LDO_1P1V_TRIM;
        }

        /* The system may continue operating while the voltage on Vccd
        *  discharges to the new voltage. The time it takes to reach the
        *  new voltage depends on the conditions, including the load current on
        *  Vccd and the external capacitor size
        */
        SRSS->PWR_TRIM_PWRSYS_CTL =
        _CLR_SET_FLD32U((SRSS->PWR_TRIM_PWRSYS_CTL), SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, trimVoltage);

        if (CY_SYSPM_LDO_VOLTAGE_1_1V == voltage)
        {
            /* A delay for the supply to stabilize at the new higher voltage */
            Cy_SysLib_DelayUs(LDO_STABILIZATION_DELAY_US);

            /* Update read-write margin value for the LP mode */
            SetReadMarginTrimLp();

            /* Set the analog signal bit to the flash macro register after
            * the output voltage is 1.1 V
            */
            ClearVoltageBitForFlash();
        }
    }

    Cy_SysLib_ExitCriticalSection(interruptState);
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
* CY_SYSPM_BEFORE_TRANSITION, the order is this: the first registered callback
* will be always the first executed. And the last registered callback will be
* executed as the last callback. For modes CY_SYSPM_AFTER_TRANSITION and
* CY_SYSPM_CHECK_FAIL, the order is this: the first registered callback will be
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
* \funcusage
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_Callback_Func_Declaration
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_Callback_Params_Declaration
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_Callback_Structure_Declaration
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_Callback_Func_Implementation
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_RegisterCallback
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
                callbackListLast = handler;

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
* This function unregisters a callback.
*
* The registered callback can be unregistered. Otherwise, false will be
* returned.
*
* \param handler The item that should be unregistered.
* See \ref cy_stc_syspm_callback_t.
*
* \return
* True if on success <br>
* False if it was not unregistered or no callbacks were registered.
*
* \funcusage
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_UnregisterCallback
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
            /* Update links of related to unregistered callback items */
            if (handler->nextItm != NULL)
            {
                handler->nextItm->prevItm = handler->prevItm;
            }
            if (handler->prevItm != NULL)
            {
                handler->prevItm->nextItm = handler->nextItm;
            }

            /* Requested callback was first in the list */
            if (callbackRoot == handler)
            {
                callbackRoot = callbackRoot->nextItm;
            }

            /* Requested callback was last in the list */
            if (callbackListLast == handler)
            {
                callbackListLast = handler->prevItm;
            }

            /* Decrement the value with number of registered callbacks */
            --curRegisteredCallbacks;
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
* \note This low-level function is being used by \ref Cy_SysPm_Sleep,
* \ref Cy_SysPm_DeepSleep, \ref Cy_SysPm_Hibernate, \ref Cy_SysPm_EnterLowPowerMode
* and \ref Cy_SysPm_ExitLowPowerMode API functions, however might be also useful as
* an independent API function in some custom applications.
* \note The registered callbacks will be executed in order based on
* \ref cy_en_syspm_callback_type_t value.
* The are possible two orders of callbacks execution: <br>
* * From first registered to last registered. Such order is relevant to
* callbacks with mode CY_SYSPM_CHECK_READY and CY_SYSPM_BEFORE_TRANSITION.
* * Backward flow execution: from last executed callback to the
* first registered. Such order is relevant to callbacks with mode
* CY_SYSPM_AFTER_TRANSITION and CY_SYSPM_CHECK_FAIL. Note that, the last
* registered callback function is skipped with mode CY_SYSPM_CHECK_FAIL. This
* is because the callback that returned CY_SYSPM_FAIL already knows that it failed.
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
* \funcusage
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_ExecuteCallback
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
        * the first registered callback. Such a flow is required if previous
        * callback function returned CY_SYSPM_FAIL or previous callback mode was
        * CY_SYSPM_BEFORE_TRANSITION. Such an order is required to undo configurations in
        * correct backward order.
        */
        curCallback = callbackListLast;

        /* Skip last executed callback that returned CY_SYSPM_FAIL, as this
        *  callback already knows that it failed
        */
        if (mode == CY_SYSPM_CHECK_FAIL)
        {
            curCallback = lastExecutedCallback;
            if (curCallback != NULL)
            {
                curCallback = curCallback->prevItm;
            }
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

/** \cond INTERNAL */
/*******************************************************************************
* Function Name: Cy_SysPm_IoFreeze
****************************************************************************//**
*
* This function saves the output states and configuration of I/O cells.
*
* I/O-cell configuration can be changed while I/O-cells are frozen. The new
* configuration becomes effective only after the pins are unfrozen.
*
* Cy_SysPm_Hibernate() calls this function to freeze the I/O cells while
* entering hibernate.
*
* \funcusage
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_Freeze
*
*******************************************************************************/
void Cy_SysPm_IoFreeze(void)
{
    uint32_t interruptState;
    uint32_t regValue;

    interruptState = Cy_SysLib_EnterCriticalSection();

    /* Check the FREEZE state to avoid a recurrent I/O-cells freeze attempt,
    * because the second call to this function will cause an accidental switch
    * to Hibernate mode (the system will enter Hibernate mode immediately
    * after writing to the hibernate bit because both UNLOCK and FREEZE were set
    * correctly in the previous call to this function).
    */
    if(0U == _FLD2VAL(SRSS_PWR_HIBERNATE_FREEZE, SRSS->PWR_HIBERNATE))
    {
        /* Clear the unlock field for correct freeze of the I/O cells */
        SRSS->PWR_HIBERNATE = _CLR_SET_FLD32U((SRSS->PWR_HIBERNATE), SRSS_PWR_HIBERNATE_UNLOCK, 0U);

        /* Disable overriding by the peripherals the next pin-freeze command */
        SRSS->PWR_HIBERNATE |= CY_SYSPM_PWR_SET_HIBERNATE;

        /* The second write causes freeze of I/O cells to save the I/O-cell state */
        regValue = SRSS->PWR_HIBERNATE;
        SRSS->PWR_HIBERNATE = regValue;
    }
    Cy_SysLib_ExitCriticalSection(interruptState);
}
/** \endcond */


/*******************************************************************************
* Function Name: Cy_SysPm_IoUnfreeze
****************************************************************************//**
*
* This function unfreezes the I/O cells which were automatically frozen when the
* Hibernate is entered with the call to \ref Cy_SysPm_Hibernate().
*
* I/O-cells remain frozen after a wakeup from hibernate mode until the
* firmware unfreezes them, by calling this function.
*
* If the firmware must retain the data value on the port, then the
* value must be read and re-written to the data register before calling this
* function. Furthermore, the drive mode must be re-programmed before the pins are
* unfrozen. If this is not done, the pin will change to the default state
* the moment the freeze is removed.
*
* Note that I/O cell configuration can be changed while frozen. The new
* configuration becomes effective only after the pins are unfrozen.
*
* \funcusage
* \snippet syspm/syspm_2_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysPm_IoUnfreeze
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
* Function Name: SetVoltageBitForFlash
****************************************************************************//**
*
* The internal function that changes the Vcc setting for the flash.
*
* Sets the bit for the flash macro register. This bit should be set when the
* voltage for the core regulators is less than 0.99 V.
*
*******************************************************************************/
static void SetVoltageBitForFlash(void)
{
    FLASHC_FM_CTL->ANA_CTL0 |= _VAL2FLD(FLASHC_FM_CTL_ANA_CTL0_VCC_SEL, 1U);
}


/*******************************************************************************
* Function Name: ClearVoltageBitForFlash
****************************************************************************//**
*
* This is the internal function that changes the Vcc setting for the flash.
*
* Clears the bit for the flash macro register. This bit should
* be cleared if the output voltage for the core regulators is higher than 0.99 V.
*
*******************************************************************************/
static void ClearVoltageBitForFlash(void)
{
    FLASHC_FM_CTL->ANA_CTL0 &= ((uint32_t) (~_VAL2FLD(FLASHC_FM_CTL_ANA_CTL0_VCC_SEL, 1U)));
}

#ifdef CY_IP_MXUDB

    /*******************************************************************************
    * Function Name: SaveRegisters
    ****************************************************************************//**
    *
    * The internal function saves non-retained registers before entering Deep Sleep.
    * Cypress ID #280370.
    *
    * \param regs
    * The structure where the registers are saved before Deep Sleep.
    *
    *******************************************************************************/
    static void SaveRegisters(cy_stc_syspm_backup_regs_t *regs)
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
    * Function Name: RestoreRegisters
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
    static void RestoreRegisters(cy_stc_syspm_backup_regs_t const *regs)
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

#if defined(CY_DEVICE_PSOC6ABLE2) && !defined(CY_PSOC6ABLE2_REV_0A_SUPPORT_DISABLE)

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
    #pragma diag_suppress=Ta023
    __ramfunc
    #else
        CY_SECTION(".cy_ramfunc") CY_NOINLINE
    #endif
    static void Cy_EnterDeepSleep(cy_en_syspm_waitfor_t waitFor)
    {
        /* Acquire the IPC to prevent changing of the shared resources at the same time */
        while(0U == _FLD2VAL(IPC_STRUCT_ACQUIRE_SUCCESS, SYSPM_IPC_STC->ACQUIRE))
        {
            /* Wait until the IPC structure is released by another core */
        }

        /* Set the flag that current core entered Deep Sleep */
        SYSPM_IPC_STC->DATA |= CUR_CORE_DP_MASK;

        /* Change the slow and fast clock dividers only under condition that
        *  the other core is already in Deep Sleep. Cypress ID #284516
        */
        if (0U != (SYSPM_IPC_STC->DATA & OTHER_CORE_DP_MASK))
        {
            /* Get the divider values of the slow and high clocks and store them into
            *  the IPC data register
            */
            SYSPM_IPC_STC->DATA =
            (SYSPM_IPC_STC->DATA & ((uint32_t) ~(SYSPM_FAST_CLK_DIV_Msk | SYSPM_SLOW_CLK_DIV_Msk))) |
            (((uint32_t)(_FLD2VAL(CPUSS_CM0_CLOCK_CTL_SLOW_INT_DIV, CPUSS->CM0_CLOCK_CTL) << SYSPM_SLOW_CLK_DIV_Pos)) |
             ((uint32_t)(_FLD2VAL(CPUSS_CM4_CLOCK_CTL_FAST_INT_DIV, CPUSS->CM4_CLOCK_CTL) << SYSPM_FAST_CLK_DIV_Pos)));

            /* Increase the clock divider for the slow and fast clocks to SYSPM_CLK_DIVIDER */
            CPUSS->CM0_CLOCK_CTL =
            _CLR_SET_FLD32U(CPUSS->CM0_CLOCK_CTL, CPUSS_CM0_CLOCK_CTL_SLOW_INT_DIV, SYSPM_CLK_DIVIDER);

            CPUSS->CM4_CLOCK_CTL =
            _CLR_SET_FLD32U(CPUSS->CM4_CLOCK_CTL, CPUSS_CM4_CLOCK_CTL_FAST_INT_DIV, SYSPM_CLK_DIVIDER);

            /* Read the divider value to make sure it is set */
            (void) CPUSS->CM0_CLOCK_CTL;
            (void) CPUSS->CM4_CLOCK_CTL;
        }

        /* Release the IPC */
        SYSPM_IPC_STC->RELEASE = 0U;

        /* Read this register to make sure it is settled */
        (void) SYSPM_IPC_STC->RELEASE;

    #if(0u != CY_CPU_CORTEX_M0P)

        /* The CPU enters the Deep Sleep mode upon execution of WFI/WFE */
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

        /* Acquire the IPC to prevent changing of the shared resources at the same time */
        while(0U == _FLD2VAL(IPC_STRUCT_ACQUIRE_SUCCESS, SYSPM_IPC_STC->ACQUIRE))
        {
            /* Wait until the IPC structure is released by another core */
        }

        /* Read and change the slow and fast clock dividers only under condition
        * that the other core is already in Deep Sleep. Cypress ID #284516
        */
        if(0U != (SYSPM_IPC_STC->DATA & OTHER_CORE_DP_MASK))
        {
            /* Restore the clock dividers for the slow and fast clocks */
            CPUSS->CM0_CLOCK_CTL =
            (_CLR_SET_FLD32U(CPUSS->CM0_CLOCK_CTL, CPUSS_CM0_CLOCK_CTL_SLOW_INT_DIV,
                            (_FLD2VAL(SYSPM_SLOW_CLK_DIV, SYSPM_IPC_STC->DATA))));

            CPUSS->CM4_CLOCK_CTL =
            (_CLR_SET_FLD32U(CPUSS->CM4_CLOCK_CTL, CPUSS_CM4_CLOCK_CTL_FAST_INT_DIV,
                            (_FLD2VAL(SYSPM_FAST_CLK_DIV, SYSPM_IPC_STC->DATA))));
        }

        /* Indicate that the current core is out of Deep Sleep */
        SYSPM_IPC_STC->DATA &= ((uint32_t) ~CUR_CORE_DP_MASK);

        /* Release the IPC */
        SYSPM_IPC_STC->RELEASE = 0U;
    }
    #if defined (__ICCARM__)
        #pragma diag_default=Ta023
    #endif
#endif /* defined(CY_DEVICE_PSOC6ABLE2) && !defined(CY_PSOC6ABLE2_REV_0A_SUPPORT_DISABLE) */


/*******************************************************************************
* Function Name: EnterDeepSleep
****************************************************************************//**
*
* The internal function that prepares the system for Deep Sleep,
* sets the CPU core to the Deep Sleep, and restores the system after a
* wakeup from Deep Sleep.
*
* \param waitFor
* Selects wait for action. See \ref cy_en_syspm_waitfor_t.
*
*******************************************************************************/
#if defined (__ICCARM__)
    #pragma diag_suppress=Ta023
    __ramfunc
#else
    CY_SECTION(".cy_ramfunc") CY_NOINLINE
#endif
static void EnterDeepSleep(cy_en_syspm_waitfor_t waitFor)
{
#if(0u != CY_CPU_CORTEX_M0P)

    /* The CPU enters the Deep Sleep mode upon execution of WFI/WFE */
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

    /* Acquire the IPC to prevent changing of the shared resources at the same time.
    * Shared resources is the BIST_DATA[0] register.
    */
    while(0U == _FLD2VAL(IPC_STRUCT_ACQUIRE_SUCCESS, SYSPM_IPC_STC->ACQUIRE))
    {
        /* Wait until the IPC structure is released by another core */
    }

    /* Set 10 uS delay only under condition that the DELAY_DONE_FLAG is
    *  cleared. Cypress ID #288510
    */
    if(DELAY_DONE_FLAG->BIST_DATA[0] == NEED_DELAY)
    {
        /* Configure the counter to be sourced by IMO */
        TST_DDFT_SLOW_CTL_REG = TST_DDFT_SLOW_CTL_MASK;
        CLK_OUTPUT_SLOW_REG   = CLK_OUTPUT_SLOW_MASK;
        TST_DDFT_FAST_CTL_REG = TST_DDFT_FAST_CTL_MASK;

        /* Load the down-counter to count the 10 uS */
        CLK_CAL_CNT1_REG = IMO_10US_DELAY;

        while(0U == (CLK_CAL_CNT1_REG & CLK_CAL_CNT1_DONE))
        {
            /* Wait until the counter stops counting */
        }

        /* Indicate that 10 uS delay was done */
        DELAY_DONE_FLAG->BIST_DATA[0] = DELAY_DONE;
    }

    /* Release the IPC structure in do while loop just to sure that this code
    * is not optimized
    */
    do
    {
        SYSPM_IPC_STC->RELEASE = 0U;
    } while (0U != 0U);
}
#if defined (__ICCARM__)
    #pragma diag_default=Ta023
#endif


/*******************************************************************************
* Function Name: SetReadMarginTrimUlp
****************************************************************************//**
*
* This is the internal function that updates the read-margin trim values for the
* RAM and ROM. The trim update is done during transition of regulator voltage
* from higher to a lower one.
*
*******************************************************************************/
static void SetReadMarginTrimUlp(void)
{
    /* Update read-write margin value for the ULP mode. Cypress ID#297292 */
    CPUSS->TRIM_RAM_CTL = (CPUSS->TRIM_RAM_CTL & ((uint32_t) ~CPUSS_TRIM_RAM_CTL_RM_Msk)) |
                          (CPUSS_TRIM_RAM_ULP & CPUSS_TRIM_RAM_CTL_RM_Msk);

    CPUSS->TRIM_ROM_CTL = (CPUSS->TRIM_ROM_CTL & ((uint32_t) ~CPUSS_TRIM_ROM_CTL_RM_Msk)) |
                          (CPUSS_TRIM_ROM_ULP & CPUSS_TRIM_ROM_CTL_RM_Msk);
}


/*******************************************************************************
* Function Name: SetReadMarginTrimLp
****************************************************************************//**
*
* The internal function which updates the read-margin trim values for the
* RAM and ROM. The trim update is done during transition of regulator voltage
* from lower to a higher one.
*
*******************************************************************************/
static void SetReadMarginTrimLp(void)
{
    /* Update read-write margin value for the LP mode. Cypress ID#297292 */
    CPUSS->TRIM_RAM_CTL = (CPUSS->TRIM_RAM_CTL & ((uint32_t) ~CPUSS_TRIM_RAM_CTL_RM_Msk)) |
                          (CPUSS_TRIM_RAM_LP & CPUSS_TRIM_RAM_CTL_RM_Msk);

    CPUSS->TRIM_ROM_CTL = (CPUSS->TRIM_ROM_CTL & ((uint32_t) ~CPUSS_TRIM_ROM_CTL_RM_Msk)) |
                          (CPUSS_TRIM_ROM_LP & CPUSS_TRIM_ROM_CTL_RM_Msk);
}


/*******************************************************************************
* Function Name: SetWriteAssistTrimUlp
****************************************************************************//**
*
* This is the internal function that updates the write assistant trim value for the
* RAM. The trim update is done during transition of regulator voltage
* from higher to a lower.
*
*******************************************************************************/
static void SetWriteAssistTrimUlp(void)
{
    /* Update write assist value for the ULP mode. Cypress ID#297292 */
    CPUSS->TRIM_RAM_CTL = (CPUSS->TRIM_RAM_CTL & ((uint32_t) ~CPUSS_TRIM_RAM_CTL_WA_Msk)) |
                          (CPUSS_TRIM_RAM_ULP & CPUSS_TRIM_RAM_CTL_WA_Msk);
}


/*******************************************************************************
* Function Name: SetWriteAssistTrimLp
****************************************************************************//**
*
* This is the internal function that updates the write assistant trim value for the
* RAM. The trim update is done during transition of regulator voltage
* from lower to a higher one.
*
*******************************************************************************/
static void SetWriteAssistTrimLp(void)
{
    /* Update write assist value for the LP mode.  Cypress ID#297292 */
    CPUSS->TRIM_RAM_CTL = (CPUSS->TRIM_RAM_CTL & ((uint32_t) ~CPUSS_TRIM_RAM_CTL_WA_Msk)) |
                          (CPUSS_TRIM_RAM_LP & CPUSS_TRIM_RAM_CTL_WA_Msk);
}


/* [] END OF FILE */
