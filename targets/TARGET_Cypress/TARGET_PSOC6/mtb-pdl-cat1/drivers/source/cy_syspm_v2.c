/***************************************************************************//**
* \file cy_syspm.c
* \version 4.20
*
* This driver provides the source code for API power management.
*
********************************************************************************
* \copyright
* Copyright 2016-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include "cy_device.h"

#if defined (CY_IP_MXS28SRSS)

#include "cy_syspm.h"

//#include "cy_ipc_drv.h"
//#include "cy_ipc_sema.h"
//#include "cy_ipc_pipe.h"
//#include "cy_prot.h"

/*******************************************************************************
*       Internal Functions
*******************************************************************************/
static bool EnterDeepSleepRam(cy_en_syspm_waitfor_t waitFor);

static void SetReadMarginTrimUlp(void);
static void SetReadMarginTrimLp(void);
static void SetWriteAssistTrimUlp(void);
static void SetWriteAssistTrimLp(void);
static bool IsVoltageChangePossible(void);


/*******************************************************************************
*       Internal Defines
*******************************************************************************/
#ifndef CY_PSOC6ABLE2_REV_0A_SUPPORT_DISABLE

    /** The internal define for clock divider */
    #define SYSPM_CLK_DIVIDER         (9U)

    /* Mask for the fast clock divider value */
    #define SYSPM_FAST_CLK_DIV_Msk    (0xFF000000UL)

    /* Position for the fast clock divider value */
    #define SYSPM_FAST_CLK_DIV_Pos    (24UL)

    /* Mask for the slow clock divider value */
    #define SYSPM_SLOW_CLK_DIV_Msk    (0x00FF0000UL)

    /* Position for the slow clock divider value */
    #define SYSPM_SLOW_CLK_DIV_Pos    (16UL)

    /* Mask for both slow and fast mask clock dividers */
    #define SYSPM_CLK_DIV_MASK        (SYSPM_FAST_CLK_DIV_Msk | SYSPM_SLOW_CLK_DIV_Msk)

    #if (CY_CPU_CORTEX_M4)
        #define CUR_CORE_DP_MASK      (0x01UL)
        #define OTHER_CORE_DP_MASK    (0x02UL)
    #else
        #define CUR_CORE_DP_MASK      (0x02UL)
        #define OTHER_CORE_DP_MASK    (0x01UL)
    #endif

#endif /* #ifndef CY_PSOC6ABLE2_REV_0A_SUPPORT_DISABLE */

/* The define for the current active bus master */
#if (CY_CPU_CORTEX_M0P)
    #define ACTIVE_BUS_MASTER           CPUSS_MS_ID_CM0
#else
    #define ACTIVE_BUS_MASTER           CPUSS_MS_ID_CM4
#endif /* (CY_CPU_CORTEX_M0P) */

#if 0 //TBD
/* Define of MMIO group where UDB is located */
#define MMIO_UDB_GROUP_NR              (4U)

/* Define of MMIO group where UDB is located */
#define MMIO_UDB_SLAVE_NR              (3U)

/* The UDB placement on MMIO slave level */
#define PERI_UDB_SLAVE_ENABLED         ((uint32_t) 1UL << MMIO_UDB_GROUP_NR)
#endif

/* The definition for the delay of the LDO after its output 
* voltage is changed
*/
#define LDO_STABILIZATION_DELAY_US     (9U)

/* Define to indicate that a 10 us delay is needed */
#define NEED_DELAY                     (0x0U)

/* Slow output register */
#define CLK_OUTPUT_SLOW_MASK           (0x06U)

/* Slow control register */
#define TST_DDFT_FAST_CTL_MASK         (62U)

/* Load value for the timer to count delay after exiting Deep Sleep */
#define IMO_10US_DELAY                 (68U)

/* Define to indicate that a 10 us delay was done after exiting Deep Sleep */
#define DELAY_DONE                     (0xAAAAAAAAU)

/* Define for transitional 0.95 V for the LDO regulator */
#define LDO_OUT_VOLTAGE_0_95V          (0x0BU)

/* Define for transitional 1.1 V for the LDO regulator */
#define LDO_OUT_VOLTAGE_1_1V           (0x17U)

/* Define for transitional 1.15 V for the LDO regulator */
#define LDO_OUT_VOLTAGE_1_15V          (0x1BU)

/* The definition for the delay of the Buck supply regulator 
* stabilization after it is configured with enabled Buck output 1 */
#define BUCK_INIT_STABILIZATION_US     (900U)

/* The definition for the delay of the Buck supply regulator 
* stabilization after it is configured with enabled Buck 
* output 2 only
*/
#define BUCK_OUT2_INIT_DELAY_US        (600U)

/* The definition for the delay of the Buck regulator after its output
* voltage is changed
*/
#define BUCK_OUT2_STABILIZATION_DELAY_US    (200U)

/* Define for transitional 0.95 V for buck regulator */
#define BUCK_OUT1_VOLTAGE_0_95V             (3U)

/* Define for a Buck regulator stabilization delay from 0.9 V to 0.95 V */
#define BUCK_OUT1_0_9V_TO_0_95V_DELAY_US    (52U)

/* Define for a Buck regulator stabilization delay from 0.95 V to 1.1 V */
#define BUCK_OUT1_0_95V_TO_1_1V_DELAY_US    (145U)

/* Define for an LDO stabilization delay from 0.9 V to 0.95 V */
#define LDO_0_9V_TO_0_95V_DELAY_US          (3U)

/* Define for an LDO regulator stabilization delay from 0.95 V to 1.1 V */
#define LDO_0_95V_TO_1_1V_DELAY_US          (7U)

/* Define for ROM trim in LP mode */
#define CPUSS_TRIM_ROM_LP                 (0x00000013U)

/* Define for RAM trim in LP mode */
#define CPUSS_TRIM_RAM_LP                 (0x00004013U)

/* Define for ROM trim in ULP mode   */
#define CPUSS_TRIM_ROM_ULP                (0x00000012U)

/* Define for trim RAM in ULP mode */
#define CPUSS_TRIM_RAM_ULP                (0x00006012U)

/* Define for IPC0 notification */
//#define SYSPM_IPC_NOTIFY_STRUCT0          ((uint32_t) 0x1UL << CY_IPC_INTR_SYSCALL1) //TBD

/* The define of bit positions of the syscall return status */
#define SYSCALL_STATUS_MASK               (0xFF000000U)

/* The define for the success return status of the syscall */
#define SYSCALL_STATUS_SUCCESS            (0xA0000000U)

/* The define for device TO *B Revision ID */
#define SYSPM_DEVICE_PSOC6ABLE2_REV_0B    (0x22U)

/* The pointer to the Cy_EnterDeepSleep() function in the ROM */
#define ROM_ENTER_DEEP_SLEEP_ADDR         (*(uint32_t *) 0x00000D30UL)

/* The define to call the ROM Cy_EnterDeepSleep() function. 
* The ROM Cy_EnterDeepSleep() function prepares the system for the Deep Sleep 
* and restores the system after wakeup from the Deep Sleep. */
typedef void (*cy_cb_syspm_deep_sleep_t)(cy_en_syspm_waitfor_t waitFor, bool *wasEventSent);

#define EnterDeepSleepSrom(waitFor, wasEventSent) \
       ((cy_cb_syspm_deep_sleep_t) ROM_ENTER_DEEP_SLEEP_ADDR)((waitFor), &(wasEventSent))

/* Mask for the RAM read assist bits */
#define CPUSS_TRIM_RAM_CTL_RA_MASK                   ((uint32_t) 0x3U << 8U)

/* The define for SROM opcode to set the flash voltage bit */
#define FLASH_VOLTAGE_BIT_ULP_OPCODE                 (0x0C000003U)

/* The define for SROM opcode to clear the flash voltage bit */
#define FLASH_VOLTAGE_BIT_LP_OPCODE                  (0x0C000001U)

/* The define for SROM opcode to set the flash voltage bit */
#define FLASH_VOLTAGE_BIT_ULP_PSOC6ABLE2_OPCODE      (0x30000101U)

/* The define for SROM to clear the flash voltage bit */
#define FLASH_VOLTAGE_BIT_LP_PSOC6ABLE2_OPCODE       (0x30000001U)

/* The wait time for transition into the minimum regulator current mode
*/
#define SET_MIN_CURRENT_MODE_DELAY_US        (1U)

/* The wait delay time that occurs before the active reference is settled.
*  Intermediate delay is used in transition into the normal regulator current 
*  mode
*/
#define ACT_REF_SETTLE_DELAY_US              (8U)

/* The wait delay time that occurs after the active reference is settled. 
*  Final delay is used in transition into the normal regulator current mode
*/
#define SET_NORMAL_CURRENT_MODE_DELAY_US     (1U)

/* The internal define of the tries number in the 
* Cy_SysPm_SystemSetMinRegulatorCurrent() function
*/
#define WAIT_DELAY_TRYES                (100U)

/* The define of retained power mode of the CM4 */
#define CM4_PWR_STS_RETAINED            (2UL)

/* The define for number of callback roots */
#define CALLBACK_ROOT_NR                (5U)

/* Mask for checking the CM4 Deep Sleep status */
#define CM4_DEEPSLEEP_MASK     (CPUSS_CM4_STATUS_SLEEPING_Msk | CPUSS_CM4_STATUS_SLEEPDEEP_Msk)

/* Mask for checking the CM0P Deep Sleep status */
#define CM0_DEEPSLEEP_MASK     (CPUSS_CM0_STATUS_SLEEPING_Msk | CPUSS_CM0_STATUS_SLEEPDEEP_Msk)

/* The mask to unlock the Hibernate power mode */
#define HIBERNATE_UNLOCK_VAL                 ((uint32_t) 0x3Au << SRSS_PWR_HIBERNATE_UNLOCK_Pos)

/* The mask to set the Hibernate power mode */
#define SET_HIBERNATE_MODE                   ((HIBERNATE_UNLOCK_VAL |\
                                               SRSS_PWR_HIBERNATE_FREEZE_Msk |\
                                               SRSS_PWR_HIBERNATE_HIBERNATE_Msk))

/* The mask to retain the Hibernate power mode status */
#define HIBERNATE_RETAIN_STATUS_MASK         ((SRSS_PWR_HIBERNATE_TOKEN_Msk |\
                                               SRSS_PWR_HIBERNATE_MASK_HIBALARM_Msk |\
                                               SRSS_PWR_HIBERNATE_MASK_HIBWDT_Msk |\
                                               SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Msk |\
                                               SRSS_PWR_HIBERNATE_MASK_HIBPIN_Msk))

/** The mask for the Hibernate wakeup sources */
#define HIBERNATE_WAKEUP_MASK               ((SRSS_PWR_HIBERNATE_MASK_HIBALARM_Msk |\
                                              SRSS_PWR_HIBERNATE_MASK_HIBWDT_Msk |\
                                              SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Msk |\
                                              SRSS_PWR_HIBERNATE_MASK_HIBPIN_Msk))

/** The define to update the token to indicate the transition into Hibernate */
#define HIBERNATE_TOKEN                    ((uint32_t) 0x1BU << SRSS_PWR_HIBERNATE_TOKEN_Pos)


/* The mask for low power modes the power circuits (POR/BOD, Bandgap reference,
*  Reference buffer, Current reference) when active core regulator is LDO
*/
//#define PWR_CIRCUITS_SET_LPMODE_LDO_MASK        (SRSS_PWR_CTL_LINREG_LPMODE_Msk | PWR_CIRCUITS_SET_LPMODE_BUCK_MASK) //TBD

/* The mask for low power modes the power circuits (POR/BOD, Bandgap reference, 
*  Reference buffer, Current reference) when active core regulator is Buck
*/
#if 0 //TBD
#define PWR_CIRCUITS_SET_LPMODE_BUCK_MASK       (SRSS_PWR_CTL_PORBOD_LPMODE_Msk |\
                                                 SRSS_PWR_CTL_BGREF_LPMODE_Msk |\
                                                 SRSS_PWR_CTL_VREFBUF_LPMODE_Msk |\
                                                 SRSS_PWR_CTL_IREF_LPMODE_Msk)
#endif
/*******************************************************************************
*       Internal Variables
*******************************************************************************/

/* Array of the callback roots */
static cy_stc_syspm_callback_t* pmCallbackRoot[CALLBACK_ROOT_NR] = {NULL, NULL, NULL, NULL, NULL};

/* The array of the pointers to failed callback */
static cy_stc_syspm_callback_t* failedCallback[CALLBACK_ROOT_NR] = {NULL, NULL, NULL, NULL, NULL};

/* Structure for registers that should retain while Deep Sleep mode */
//static cy_stc_syspm_backup_regs_t bkpRegs;//TBD

#if (CY_CPU_CORTEX_M4)
    /* Global boolean variable used to clear the  Event Register of the CM4 core */
    static bool wasEventSent = false;
#endif /* (CY_CPU_CORTEX_M4) */


uint32_t Cy_SysPm_ReadStatus(void)
{
    uint32_t pmStatus = 0UL;
#if 0 //TBD
    /* Check whether CM4 is in Deep Sleep mode */
    if ((CPUSS_CM4_STATUS & CM4_DEEPSLEEP_MASK) == CM4_DEEPSLEEP_MASK)
    {
        pmStatus |= CY_SYSPM_STATUS_CM4_DEEPSLEEP;
    }
    /* Check whether CM4 is in Sleep mode */
    else if(0U != _FLD2VAL(CPUSS_CM4_STATUS_SLEEPING, CPUSS_CM4_STATUS))
    {
        pmStatus |= CY_SYSPM_STATUS_CM4_SLEEP;
    }
    else
    {
        pmStatus |= CY_SYSPM_STATUS_CM4_ACTIVE;
    }

    /* Check whether CM0p is in Deep Sleep mode */
    if ((CPUSS_CM0_STATUS & CM0_DEEPSLEEP_MASK) == CM0_DEEPSLEEP_MASK)
    {
        pmStatus |= CY_SYSPM_STATUS_CM0_DEEPSLEEP;
    }
    /* Check whether CM0p is in Sleep mode */
    else if (0U != _FLD2VAL(CPUSS_CM0_STATUS_SLEEPING, CPUSS_CM0_STATUS))
    {
        pmStatus |= CY_SYSPM_STATUS_CM0_SLEEP;
    }
    else
    {
        pmStatus |= CY_SYSPM_STATUS_CM0_ACTIVE;
    }
#endif
    /* Check whether the device is in LP mode by reading 
    *  the core voltage:
    *  - 0.9V (nominal) - System ULP mode
    *  - 1.1V (nominal) - System LP mode
    */
    
    /* Read current active regulator */
    if (Cy_SysPm_LdoIsEnabled())
    {
        /* Current active regulator is LDO */
        if (Cy_SysPm_LdoGetVoltage() != CY_SYSPM_LDO_VOLTAGE_ULP)
        {
            pmStatus |= CY_SYSPM_STATUS_SYSTEM_LP;
        }
        else
        {
            pmStatus |= CY_SYSPM_STATUS_SYSTEM_ULP;
        }
    }
    else
    {
        /* Current active regulator is Buck */
        if (Cy_SysPm_BuckGetVoltage1() != CY_SYSPM_BUCK_OUT1_VOLTAGE_ULP)
        {
            pmStatus |= CY_SYSPM_STATUS_SYSTEM_LP;
        }
        else
        {
            pmStatus |= CY_SYSPM_STATUS_SYSTEM_ULP;
        }
    }

    return pmStatus;
}


cy_en_syspm_status_t Cy_SysPm_CpuEnterSleep(cy_en_syspm_waitfor_t waitFor)
{
    uint32_t interruptState;
    uint32_t cbSleepRootIdx = (uint32_t) CY_SYSPM_SLEEP;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;

    CY_ASSERT_L3(CY_SYSPM_IS_WAIT_FOR_VALID(waitFor));

    /* Call registered callback functions with CY_SYSPM_CHECK_READY parameter */
    if (pmCallbackRoot[cbSleepRootIdx] != NULL)
    {
        retVal = Cy_SysPm_ExecuteCallback(CY_SYSPM_SLEEP, CY_SYSPM_CHECK_READY);
    }

    /* The CPU can switch into the Sleep power mode only when
    *  all executed registered callback functions with the CY_SYSPM_CHECK_READY
    *  parameter return CY_SYSPM_SUCCESS.
    */
    if(retVal == CY_SYSPM_SUCCESS)
    {
        /* Call the registered callback functions with
        * CY_SYSPM_BEFORE_TRANSITION parameter
        */
        interruptState = Cy_SysLib_EnterCriticalSection();
        if (pmCallbackRoot[cbSleepRootIdx] != NULL)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_SLEEP, CY_SYSPM_BEFORE_TRANSITION);
        }

        /* The CPU enters the Sleep power mode upon execution of WFI/WFE */
        SCB_SCR &= (uint32_t) ~SCB_SCR_SLEEPDEEP_Msk;

        if(waitFor != CY_SYSPM_WAIT_FOR_EVENT)
        {
            __WFI();
        }
        else
        {
            __WFE();
        #if 0 //TBD
        #if (CY_CPU_CORTEX_M4)
            if (Cy_SysLib_GetDevice() == CY_SYSLIB_DEVICE_PSOC6ABLE2)
            {
                /* For the CM4 CPU, the WFE instruction is called twice.
                *  The second WFE call clears the Event Register of CM4 CPU.
                *  Cypress ID #279077.
                */
                if(wasEventSent)
                {
                    __WFE();
                }

                wasEventSent = true;
            }
        #endif /* (CY_CPU_CORTEX_M4) */
        #endif
        }
        Cy_SysLib_ExitCriticalSection(interruptState);

        /* Call the registered callback functions with the
        *  CY_SYSPM_AFTER_TRANSITION parameter
        */
        if (pmCallbackRoot[cbSleepRootIdx] != NULL)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_SLEEP, CY_SYSPM_AFTER_TRANSITION);
        }
    }
    else
    {
        /* Execute callback functions with the CY_SYSPM_CHECK_FAIL parameter to
        *  undo everything done in the callback with the CY_SYSPM_CHECK_READY
        *  parameter
        */
        (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_SLEEP, CY_SYSPM_CHECK_FAIL);
        retVal = CY_SYSPM_FAIL;
    }
    return retVal;
}


cy_en_syspm_status_t Cy_SysPm_CpuEnterDeepSleep(cy_en_syspm_waitfor_t waitFor)
{
    uint32_t interruptState;
    uint32_t cbDeepSleepRootIdx = (uint32_t) CY_SYSPM_DEEPSLEEP;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;

    CY_ASSERT_L3(CY_SYSPM_IS_WAIT_FOR_VALID(waitFor));

    /* Call the registered callback functions with the CY_SYSPM_CHECK_READY
    *  parameter
    */
    if (pmCallbackRoot[cbDeepSleepRootIdx] != NULL)
    {
        retVal = Cy_SysPm_ExecuteCallback(CY_SYSPM_DEEPSLEEP, CY_SYSPM_CHECK_READY);
    }

    /* The CPU can switch into the Deep Sleep power mode only when
    *  all executed registered callback functions with the CY_SYSPM_CHECK_READY
    *  parameter return CY_SYSPM_SUCCESS
    */
    if (retVal == CY_SYSPM_SUCCESS)
    {
        /* System Deep Sleep indicator */
        bool wasSystemDeepSleep = false;
        CY_UNUSED_PARAM(wasSystemDeepSleep);
        
        /* Call the registered callback functions with the
        * CY_SYSPM_BEFORE_TRANSITION parameter
        */
        interruptState = Cy_SysLib_EnterCriticalSection();
        if (pmCallbackRoot[cbDeepSleepRootIdx] != NULL)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_DEEPSLEEP, CY_SYSPM_BEFORE_TRANSITION);
        }
#if 0 //TBD
        if (0U != cy_device->udbPresent)
        {
            /* Check whether the UDB disabled on MMIO level */
            if (0UL != (PERI_GR_SL_CTL(MMIO_UDB_SLAVE_NR) & PERI_UDB_SLAVE_ENABLED))
            {
                /* Save non-retained registers */
                Cy_SysPm_SaveRegisters(&bkpRegs);
            }
        }
#endif
        /* Different device families and revisions have a different Deep Sleep entries */
        if (Cy_SysLib_GetDevice() == CY_SYSLIB_DEVICE_PSOC6ABLE2) //TBD
        {
            /* The CPU enters Deep Sleep and wakes up in the RAM */
            wasSystemDeepSleep = EnterDeepSleepRam(waitFor);
        }
        else
        {
        #if 0  //TBD
        #if (CY_CPU_CORTEX_M0P)
            
            /* Check if there is a pending syscall */
            if (Cy_IPC_Drv_IsLockAcquired(Cy_IPC_Drv_GetIpcBaseAddress(CY_IPC_CHAN_SYSCALL)) != false)
            {
                /* Do not put the CPU into Deep Sleep and return pending status */
                retVal = CY_SYSPM_SYSCALL_PENDING;
            }
            else
        #endif /* (CY_CPU_CORTEX_M0P) */
        #endif
            {
            #if 0 //TBD
            #if (CY_CPU_CORTEX_M4)
                /* Repeat the WFI/WFE instruction if a wake up was not intended. 
                *  Cypress ID #272909
                */
                do
                {
            #endif /* (CY_CPU_CORTEX_M4) */
            #endif

                    /* The CPU enters Deep Sleep mode upon execution of WFI/WFE */
                    SCB_SCR |= SCB_SCR_SLEEPDEEP_Msk;

                    if(waitFor != CY_SYSPM_WAIT_FOR_EVENT)
                    {
                        __WFI();
                    }
                    else
                    {
                        __WFE();
                    }
            #if 0 //TBD
            #if (CY_CPU_CORTEX_M4)
                } while (_FLD2VAL(CPUSS_CM4_PWR_CTL_PWR_MODE, CPUSS_CM4_PWR_CTL) == CM4_PWR_STS_RETAINED);
            #endif /* (CY_CPU_CORTEX_M4) */
            #endif
            }
        }

#if 0 //TBD
        if (0U != cy_device->udbPresent)
        {
            /* Do not restore the UDBs if there was no system Deep Sleep mode or 
            *  UDBs are disabled on MMIO level
            */
            if (wasSystemDeepSleep && (0UL != (PERI_GR_SL_CTL(MMIO_UDB_SLAVE_NR) & PERI_UDB_SLAVE_ENABLED)))
            {
                cy_stc_syspm_backup_regs_t *ptrRegs;
                
            #ifndef CY_PSOC6ABLE2_REV_0A_SUPPORT_DISABLE
                if (Cy_SysLib_GetDeviceRevision() == CY_SYSLIB_DEVICE_REV_0A)
                {
                    ptrRegs = &bkpRegs;
                }
                else
            #endif /* #ifndef CY_PSOC6ABLE2_REV_0A_SUPPORT_DISABLE */   
                {
                    //ptrRegs = (cy_stc_syspm_backup_regs_t *) REG_IPC_STRUCT_DATA(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT));//TBD
                }
                
                /* Restore non-retained registers */
                Cy_SysPm_RestoreRegisters(ptrRegs);
            }
        }
#endif
        Cy_SysLib_ExitCriticalSection(interruptState);
    }
    
    if (retVal == CY_SYSPM_SUCCESS)
    {
        /* Call the registered callback functions with the CY_SYSPM_AFTER_TRANSITION 
        *  parameter
        */
        if (pmCallbackRoot[cbDeepSleepRootIdx] != NULL)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_DEEPSLEEP, CY_SYSPM_AFTER_TRANSITION);
        }  
    }
    else 
    {
        /* Execute callback functions with the CY_SYSPM_CHECK_FAIL parameter to 
        *  undo everything done in the callback with the CY_SYSPM_CHECK_READY 
        *  parameter
        */
        if (pmCallbackRoot[cbDeepSleepRootIdx] != NULL)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_DEEPSLEEP, CY_SYSPM_CHECK_FAIL);
        }
        
        /* Rewrite return value to indicate fail */
        if (retVal != CY_SYSPM_SYSCALL_PENDING)
        {
            retVal = CY_SYSPM_FAIL;  
        }
    }
    return retVal;
}


cy_en_syspm_status_t Cy_SysPm_SystemEnterHibernate(void)
{
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;
    uint32_t cbHibernateRootIdx = (uint32_t) CY_SYSPM_HIBERNATE;
    /* Call the registered callback functions with the
    * CY_SYSPM_CHECK_READY parameter
    */
    if (pmCallbackRoot[cbHibernateRootIdx] != NULL)
    {
        retVal = Cy_SysPm_ExecuteCallback(CY_SYSPM_HIBERNATE, CY_SYSPM_CHECK_READY);
    }

    /* The system can switch into Hibernate power mode only when
    *  all executed registered callback functions with CY_SYSPM_CHECK_READY
    *  parameter return CY_SYSPM_SUCCESS.
    */
    if(retVal == CY_SYSPM_SUCCESS)
    {
        /* Call registered callback functions with CY_SYSPM_BEFORE_TRANSITION
        *  parameter
        */
        (void) Cy_SysLib_EnterCriticalSection();
        if (pmCallbackRoot[cbHibernateRootIdx] != NULL)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_HIBERNATE, CY_SYSPM_BEFORE_TRANSITION);
        }

        /* Preserve the token that will be retained through a wakeup sequence.
         * This could be used by Cy_SysLib_GetResetReason() to differentiate
         * Wakeup from a general reset event.
         * Preserve the wakeup source(s) configuration.
         */
        SRSS_PWR_HIBERNATE = (SRSS_PWR_HIBERNATE & HIBERNATE_WAKEUP_MASK) | HIBERNATE_TOKEN;

        /* Disable overriding by the peripherals the next pin-freeze command */
        SRSS_PWR_HIBERNATE |= SET_HIBERNATE_MODE;

        /* The second write causes freezing of I/O cells to save the I/O-cell state */
        SRSS_PWR_HIBERNATE |= SET_HIBERNATE_MODE;

        /* Third write cause system to enter Hibernate */
        SRSS_PWR_HIBERNATE |= SET_HIBERNATE_MODE;

        /* Read register to make sure it is settled */
        (void) SRSS_PWR_HIBERNATE;

        /* Wait for transition */
        __WFI();

        /* The callback function calls with the CY_SYSPM_AFTER_TRANSITION
        * parameter in the Hibernate power mode are not applicable as system
        * wake-up was made on system reboot.
        */

        /* A wakeup from Hibernate is performed by toggling of the wakeup
        * pins, or WDT matches, or Backup domain alarm expires. This depends on
        * what item is configured in the Hibernate register. After a wakeup
        * event, a normal Boot procedure occurs.
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


cy_en_syspm_status_t Cy_SysPm_SystemEnterLp(void)
{
    uint32_t interruptState;
    uint32_t cbLpRootIdx = (uint32_t) CY_SYSPM_LP;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;

    /* Call the registered callback functions with the
    * CY_SYSPM_CHECK_READY parameter
    */
    if (pmCallbackRoot[cbLpRootIdx] != NULL)
    {
        retVal = Cy_SysPm_ExecuteCallback(CY_SYSPM_LP, CY_SYSPM_CHECK_READY);
    }

    /* The system can switch into LP only when
    * all executed registered callback functions with the
    * CY_SYSPM_CHECK_READY parameter return CY_SYSPM_SUCCESS
    */
    if (retVal == CY_SYSPM_SUCCESS)
    {

        /* Call the registered callback functions with the
        * CY_SYSPM_BEFORE_TRANSITION parameter
        */
        interruptState = Cy_SysLib_EnterCriticalSection();
        if (pmCallbackRoot[cbLpRootIdx] != NULL)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_LP, CY_SYSPM_BEFORE_TRANSITION);
        }

        /* Read current active regulator and set LP voltage */
        if (Cy_SysPm_LdoIsEnabled())
        {
            /* Current active regulator is LDO */
            if (Cy_SysPm_LdoGetVoltage() != CY_SYSPM_LDO_VOLTAGE_LP)
            {
                retVal = Cy_SysPm_LdoSetVoltage(CY_SYSPM_LDO_VOLTAGE_LP);
            }
        }
        else
        {
            /* Current active regulator is Buck */
            if (Cy_SysPm_BuckGetVoltage1() != CY_SYSPM_BUCK_OUT1_VOLTAGE_LP)
            {
                retVal = Cy_SysPm_BuckSetVoltage1(CY_SYSPM_BUCK_OUT1_VOLTAGE_LP);
            }
        }

        Cy_SysLib_ExitCriticalSection(interruptState);

        /* Call the registered callback functions with the
        * CY_SYSPM_AFTER_TRANSITION parameter
        */
        if (pmCallbackRoot[cbLpRootIdx] != NULL)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_LP, CY_SYSPM_AFTER_TRANSITION);
        }
    }
    else
    {
        /* Execute callback functions with the CY_SYSPM_CHECK_FAIL parameter to
        * undo everything done in the callback with the CY_SYSPM_CHECK_READY
        * parameter
        */
        (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_LP, CY_SYSPM_CHECK_FAIL);
        retVal = CY_SYSPM_FAIL;
    }

    return retVal;
}


cy_en_syspm_status_t Cy_SysPm_SystemEnterUlp(void)
{
    uint32_t interruptState;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;
    uint32_t cbUlpRootIdx = (uint32_t) CY_SYSPM_ULP;

    /* Call the registered callback functions with the
    * CY_SYSPM_CHECK_READY parameter
    */
    if (pmCallbackRoot[cbUlpRootIdx] != NULL)
    {
        retVal = Cy_SysPm_ExecuteCallback(CY_SYSPM_ULP, CY_SYSPM_CHECK_READY);
    }

    /* The system can switch into the ULP only when
    * all executed registered callback functions with the
    * CY_SYSPM_CHECK_READY parameter return CY_SYSPM_SUCCESS
    */
    if (retVal == CY_SYSPM_SUCCESS)
    {
        /* Call the registered callback functions with the
        * CY_SYSPM_BEFORE_TRANSITION parameter
        */
        interruptState = Cy_SysLib_EnterCriticalSection();
        if (pmCallbackRoot[cbUlpRootIdx] != NULL)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_ULP, CY_SYSPM_BEFORE_TRANSITION);
        }

        /* Read current active regulator and set ULP voltage */
        if (Cy_SysPm_LdoIsEnabled())
        {
            /* Current active regulator is LDO */
            if (Cy_SysPm_LdoGetVoltage() != CY_SYSPM_LDO_VOLTAGE_ULP)
            {
                retVal = Cy_SysPm_LdoSetVoltage(CY_SYSPM_LDO_VOLTAGE_ULP);
            }
        }
        else
        {
            /* Current active regulator is Buck */
            if (Cy_SysPm_BuckGetVoltage1() != CY_SYSPM_BUCK_OUT1_VOLTAGE_ULP)
            {
                retVal = Cy_SysPm_BuckSetVoltage1(CY_SYSPM_BUCK_OUT1_VOLTAGE_ULP);
            }
        }

        Cy_SysLib_ExitCriticalSection(interruptState);

        /* Call the registered callback functions with the
        * CY_SYSPM_AFTER_TRANSITION parameter
        */
        if (pmCallbackRoot[cbUlpRootIdx] != NULL)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_ULP, CY_SYSPM_AFTER_TRANSITION);
        }
    }
    else
    {
        /* Execute callback functions with the CY_SYSPM_CHECK_FAIL parameter to
        * undo everything done in the callback with the CY_SYSPM_CHECK_READY
        * parameter
        */
        (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_ULP, CY_SYSPM_CHECK_FAIL);
        retVal = CY_SYSPM_FAIL;
    }

    return retVal;
}


cy_en_syspm_status_t Cy_SysPm_SystemSetMinRegulatorCurrent(void)
{
    cy_en_syspm_status_t retVal = CY_SYSPM_CANCELED;

    /* Check are the power circuits are ready to enter into regulator minimum
    *  current mode
    */
    if (0U != _FLD2VAL(SRSS_PWR_CTL_LPM_READY, SRSS_PWR_CTL))
    {
        /* Configure the minimum current mode for LDO regulator */
        if(Cy_SysPm_LdoIsEnabled())
        {
            //SRSS_PWR_CTL |= PWR_CIRCUITS_SET_LPMODE_LDO_MASK; //TBD
        }
        else
        {
            /* Configure the minimum current mode for Buck regulator */
            //SRSS_PWR_CTL |= PWR_CIRCUITS_SET_LPMODE_BUCK_MASK; //TBD
        }

        /* This wait time allows the circuits to remove their dependence on
        *  the Active mode circuits, such as active Reference
        */
        Cy_SysLib_DelayUs(SET_MIN_CURRENT_MODE_DELAY_US);

        /* Disable active reference */
       // SRSS_PWR_CTL |= SRSS_PWR_CTL_ACT_REF_DIS_Msk; //TBD
        
        retVal = CY_SYSPM_SUCCESS;
    }
    
    return retVal;
}


cy_en_syspm_status_t Cy_SysPm_SystemSetNormalRegulatorCurrent(void)
{
    uint32_t timeOut = WAIT_DELAY_TRYES;
    cy_en_syspm_status_t retVal = CY_SYSPM_TIMEOUT;

    /* Configure the regulator normal current mode for the POR/BOD circuits
    *  and for the Bandgap Voltage and Current References
    */
    if (Cy_SysPm_LdoIsEnabled())
    {
        //SRSS_PWR_CTL &= (uint32_t) ~CY_SYSPM_PWR_CIRCUITS_LPMODE_ACTIVE_LDO_MASK;//TBD
    }
    else
    {
       // SRSS_PWR_CTL &= (uint32_t) ~CY_SYSPM_PWR_CIRCUITS_LPMODE_ACTIVE_BUCK_MASK; //TBD
    }

    /* This wait time allows setting active Reference */
    Cy_SysLib_DelayUs(ACT_REF_SETTLE_DELAY_US);
#if 0 //TBD
    while ((0U == _FLD2VAL(SRSS_PWR_CTL_ACT_REF_OK, SRSS_PWR_CTL)) && (0U != timeOut))
    {
        timeOut--;
    }
#endif
    if (0U != timeOut)
    {
        /* Disable the low-power for Bandgap reference circuit */
        //SRSS_PWR_CTL &= (uint32_t) ~SRSS_PWR_CTL_BGREF_LPMODE_Msk;//TBD

        /* Delay to finally set the normal current mode */
        Cy_SysLib_DelayUs(SET_NORMAL_CURRENT_MODE_DELAY_US);

        retVal= CY_SYSPM_SUCCESS;
    }

    return retVal;
}


void Cy_SysPm_CpuSleepOnExit(bool enable)
{
    if(enable)
    {
        /* Enable sleep-on-exit feature */
        SCB_SCR |= SCB_SCR_SLEEPONEXIT_Msk;
    }
    else
    {
        /* Disable sleep-on-exit feature */
        SCB_SCR &= (uint32_t) ~(SCB_SCR_SLEEPONEXIT_Msk);
    }
}


void Cy_SysPm_SetHibernateWakeupSource(uint32_t wakeupSource)
{
    CY_ASSERT_L3(CY_SYSPM_IS_WAKE_UP_SOURCE_VALID(wakeupSource));

    uint32_t polarityMask = 0U;

    if (0U != _FLD2VAL(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, wakeupSource))
    {
        /* Reconfigure the wakeup pins and LPComp polarity based on the input */
        if (0U != (wakeupSource & CY_SYSPM_HIB_WAKEUP_LPCOMP0_MASK))
        {
            polarityMask |= CY_SYSPM_HIB_WAKEUP_LPCOMP0_POLARITY_HIGH_MASK;
        }

        if (0U != (wakeupSource & CY_SYSPM_HIB_WAKEUP_LPCOMP1_MASK))
        {
            polarityMask |= CY_SYSPM_HIB_WAKEUP_LPCOMP1_POLARITY_HIGH_MASK;
        }

        if (0U != (wakeupSource & CY_SYSPM_HIB_WAKEUP_PIN0_MASK))
        {
            polarityMask |= CY_SYSPM_HIB_WAKEUP_PIN0_POLARITY_HIGH_MASK;
        }

        if (0U != (wakeupSource & CY_SYSPM_HIB_WAKEUP_PIN1_MASK))
        {
            polarityMask |= CY_SYSPM_HIB_WAKEUP_PIN1_POLARITY_HIGH_MASK;
        }
    }

    SRSS_PWR_HIBERNATE = (SRSS_PWR_HIBERNATE & (uint32_t) ~polarityMask) | wakeupSource;

    /* Read register to make sure it is settled */
    (void) SRSS_PWR_HIBERNATE;
}


void Cy_SysPm_ClearHibernateWakeupSource(uint32_t wakeupSource)
{
    CY_ASSERT_L3(CY_SYSPM_IS_WAKE_UP_SOURCE_VALID(wakeupSource));

    uint32_t clearWakeupSourceMask = wakeupSource & (uint32_t) ~SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Msk;

    if (0U != _FLD2VAL(SRSS_PWR_HIBERNATE_POLARITY_HIBPIN, wakeupSource))
    {
        /* Clear the high active level of the requested sources */
        if ((uint32_t) CY_SYSPM_HIBERNATE_LPCOMP0_HIGH == (wakeupSource & (uint32_t) CY_SYSPM_HIBERNATE_LPCOMP0_HIGH))
        {
            clearWakeupSourceMask |= CY_SYSPM_HIB_WAKEUP_LPCOMP0_POLARITY_HIGH_MASK;
        }

        if ((uint32_t) CY_SYSPM_HIBERNATE_LPCOMP1_HIGH == (wakeupSource & (uint32_t) CY_SYSPM_HIBERNATE_LPCOMP1_HIGH))
        {
            clearWakeupSourceMask |= CY_SYSPM_HIB_WAKEUP_LPCOMP1_POLARITY_HIGH_MASK;
        }

        if ((uint32_t) CY_SYSPM_HIBERNATE_PIN0_HIGH == (wakeupSource & (uint32_t) CY_SYSPM_HIBERNATE_PIN0_HIGH))
        {
            clearWakeupSourceMask |= CY_SYSPM_HIB_WAKEUP_PIN0_POLARITY_HIGH_MASK;
        }

        if ((uint32_t) CY_SYSPM_HIBERNATE_PIN1_HIGH == (wakeupSource & (uint32_t) CY_SYSPM_HIBERNATE_PIN1_HIGH))
        {
            clearWakeupSourceMask |= CY_SYSPM_HIB_WAKEUP_PIN1_POLARITY_HIGH_MASK;
        }
    }

    SRSS_PWR_HIBERNATE &= (uint32_t) ~clearWakeupSourceMask;

    /* Read register to make sure it is settled */
    (void) SRSS_PWR_HIBERNATE;
}


cy_en_syspm_status_t Cy_SysPm_BuckEnable(cy_en_syspm_buck_voltage1_t voltage)
{
    CY_ASSERT_L3(CY_SYSPM_IS_BUCK_VOLTAGE1_VALID(voltage));

    cy_en_syspm_status_t retVal = CY_SYSPM_INVALID_STATE;

    /* Enable the Buck regulator only if it was not enabled previously.
    *  If the LDO is disabled, the device is sourced by the Buck regulator
    */
    if (Cy_SysPm_LdoIsEnabled())
    {
        uint32_t interruptState;
        interruptState = Cy_SysLib_EnterCriticalSection();

        /* Update the RAM and ROM trim values when final target Buck 0.9 V */
        if (CY_SYSPM_BUCK_OUT1_VOLTAGE_0_9V == voltage)
        {
            if (Cy_SysPm_LdoGetVoltage() != CY_SYSPM_LDO_VOLTAGE_0_9V)
            {
                retVal = Cy_SysPm_LdoSetVoltage(CY_SYSPM_LDO_VOLTAGE_0_9V);
            }
            else
            {
                retVal = CY_SYSPM_SUCCESS;
            }

            if (CY_SYSPM_SUCCESS == retVal)
            {
            #if 0 //TBD
                /* Increase LDO output voltage to 0.95 V nominal */
                SRSS_PWR_TRIM_PWRSYS_CTL = _CLR_SET_FLD32U((SRSS_PWR_TRIM_PWRSYS_CTL),
                SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, LDO_OUT_VOLTAGE_0_95V);
            #endif
            }
        }

        /* Update the RAM and ROM trim values when the final target Buck 1.1 V */
        if (CY_SYSPM_BUCK_OUT1_VOLTAGE_1_1V == voltage)
        {
            if (Cy_SysPm_LdoGetVoltage() != CY_SYSPM_LDO_VOLTAGE_1_1V)
            {
                retVal = Cy_SysPm_LdoSetVoltage(CY_SYSPM_LDO_VOLTAGE_1_1V);
            }
            else
            {
                retVal = CY_SYSPM_SUCCESS;
            }

            if (CY_SYSPM_SUCCESS == retVal)
            {
            #if 0 //TBD
                /* Set the LDO 1.15 V as final Buck output is 1.1 V */
                SRSS_PWR_TRIM_PWRSYS_CTL = _CLR_SET_FLD32U((SRSS_PWR_TRIM_PWRSYS_CTL),
                SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, LDO_OUT_VOLTAGE_1_15V);
            #endif
            }
        }

        /* Proceed only if previous settings were done successfully */
        if (CY_SYSPM_SUCCESS == retVal)
        {
            /* A delay for the supply to stabilize at the new voltage */
            Cy_SysLib_DelayUs(LDO_STABILIZATION_DELAY_US);
#if 0 //TBD
            /* Disable the Deep Sleep, nWell, and Retention regulators */
            SRSS_PWR_CTL |= (_VAL2FLD(SRSS_PWR_CTL_DPSLP_REG_DIS, 1U) |
                             _VAL2FLD(SRSS_PWR_CTL_RET_REG_DIS, 1U) |
                             _VAL2FLD(SRSS_PWR_CTL_NWELL_REG_DIS, 1U));
#endif
#if 0 //TBD
            /* Configure the Buck regulator */
            SRSS_PWR_BUCK_CTL =
            _CLR_SET_FLD32U((SRSS_PWR_BUCK_CTL), SRSS_PWR_BUCK_CTL_BUCK_OUT1_SEL, (uint32_t) voltage);

            SRSS_PWR_BUCK_CTL |= _VAL2FLD(SRSS_PWR_BUCK_CTL_BUCK_EN, 1U);

            SRSS_PWR_BUCK_CTL |= _VAL2FLD(SRSS_PWR_BUCK_CTL_BUCK_OUT1_EN, 1U);
#endif

            /* Wait until Buck output 1 is stable */
            Cy_SysLib_DelayUs(BUCK_INIT_STABILIZATION_US);
#if 0 //TBD
            /* Disable the LDO, because Vbuckout1 and LDO are shorted */
            SRSS_PWR_CTL |= _VAL2FLD(SRSS_PWR_CTL_LINREG_DIS, 1U);
#endif
        }

        Cy_SysLib_ExitCriticalSection(interruptState);
    }
    else
    {
        /* The Buck is already enabled, so just update the Buck voltage */
        cy_en_syspm_buck_voltage1_t curBuckVoltage = Cy_SysPm_BuckGetVoltage1();

        if (voltage != curBuckVoltage)
        {
            retVal = Cy_SysPm_BuckSetVoltage1(voltage);
        }
        else
        {
            retVal = CY_SYSPM_SUCCESS;
        }
    }

    return retVal;
}


cy_en_syspm_status_t Cy_SysPm_BuckSetVoltage1(cy_en_syspm_buck_voltage1_t voltage)
{
    CY_ASSERT_L3(CY_SYSPM_IS_BUCK_VOLTAGE1_VALID(voltage));

    cy_en_syspm_status_t retVal = CY_SYSPM_INVALID_STATE;


    /* Change the voltage only if protection context is set to zero (PC = 0)
    *  or the device revision supports modifying registers via syscall
    */
    if (IsVoltageChangePossible())
    {
        uint32_t interruptState;
        interruptState = Cy_SysLib_EnterCriticalSection();

        if (CY_SYSPM_BUCK_OUT1_VOLTAGE_0_9V == voltage)
        {
            /* Set bit of the flash voltage control register before ULP mode is set */
            retVal = Cy_SysPm_WriteVoltageBitForFlash(CY_SYSPM_FLASH_VOLTAGE_BIT_ULP);

            if (CY_SYSPM_SUCCESS == retVal)
            {
                /* Update read-write margin value for the ULP mode */
                SetReadMarginTrimUlp();
            }
        }
        else
        {
#if 0 //TBD
            /* Increase Buck output voltage to 0.95 V nominal */
            SRSS_PWR_BUCK_CTL =
            _CLR_SET_FLD32U((SRSS_PWR_BUCK_CTL), SRSS_PWR_BUCK_CTL_BUCK_OUT1_SEL, BUCK_OUT1_VOLTAGE_0_95V);
#endif
            /* Wait until regulator is stable on higher intermediate voltage */
            Cy_SysLib_DelayUs(BUCK_OUT1_0_9V_TO_0_95V_DELAY_US);

            /* Update write assist value for the LP mode */
            SetWriteAssistTrimLp();

            retVal = CY_SYSPM_SUCCESS;
        }

        /* Proceed only if previous settings were done successfully */
        if (CY_SYSPM_SUCCESS == retVal)
        {
            /* The system may continue operating while the voltage on Vccd
            * discharges to the new voltage. The time it takes to reach the
            * new voltage depends on the conditions, including the load current
            * on Vccd and the external capacitor size.
            */
#if 0 //TBD
            SRSS_PWR_BUCK_CTL =
            _CLR_SET_FLD32U((SRSS_PWR_BUCK_CTL), SRSS_PWR_BUCK_CTL_BUCK_OUT1_SEL, (uint32_t) voltage);
#endif

            if (CY_SYSPM_BUCK_OUT1_VOLTAGE_0_9V == voltage)
            {
                /* Update write assist value for the ULP mode */
                SetWriteAssistTrimUlp();
            }
            else
            {
                /* Delay stabilizing at the new voltage is required only
                *  when changing from a lower voltage to a higher voltage
                */
                Cy_SysLib_DelayUs(BUCK_OUT1_0_95V_TO_1_1V_DELAY_US);

                /* Update read-write margin value for the LP mode */
                SetReadMarginTrimLp();

                /* Clear bit of the flash voltage control register after
                * the LP mode is set
                */
                retVal = Cy_SysPm_WriteVoltageBitForFlash(CY_SYSPM_FLASH_VOLTAGE_BIT_LP);
            }
        }

        Cy_SysLib_ExitCriticalSection(interruptState);
    }

    return retVal;
}


bool Cy_SysPm_BuckIsOutputEnabled(cy_en_syspm_buck_out_t output)
{
    CY_ASSERT_L3(CY_SYSPM_IS_BUCK_OUTPUT_VALID(output));

    bool retVal = false;

    if (output == CY_SYSPM_BUCK_VBUCK_1)
    {
    #if 0 //TBD
        retVal = (_FLD2BOOL(SRSS_PWR_BUCK_CTL_BUCK_OUT1_EN, SRSS_PWR_BUCK_CTL));
    #endif
    }

    /* Return false if device does not have the second Buck output (SIMO) */
#if 0 //TBD
    if (0U != cy_device->sysPmSimoPresent) //TBD
    {
        if(output == CY_SYSPM_BUCK_VRF)
        {
        #if 0 //TBD
            retVal = ((0U != _FLD2VAL(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_HW_SEL, SRSS_PWR_BUCK_CTL2)) ||
                      (0U != _FLD2VAL(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_EN, SRSS_PWR_BUCK_CTL2)));
        #endif
        }
    }
#endif
    return(retVal);
}


void Cy_SysPm_BuckEnableVoltage2(void)
{
    /* Do nothing if device does not have the second Buck output (SIMO) */
#if 0 //TBD
    if (0U != cy_device->sysPmSimoPresent)
    {
        if (!Cy_SysPm_BuckIsEnabled())
        {
        #if 0 //TBD
            /* Enable the SIMO Buck regulator */
            SRSS_PWR_BUCK_CTL |= _VAL2FLD(SRSS_PWR_BUCK_CTL_BUCK_EN, 1U);
        #endif
        }

        /* Enable the SIMO Buck output 2 */
        //SRSS_PWR_BUCK_CTL2 |= _VAL2FLD(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_EN, 1U); //TBD

        /* Wait until the output is stable */
        Cy_SysLib_DelayUs(BUCK_OUT2_INIT_DELAY_US);
    }
#endif
}


void Cy_SysPm_BuckSetVoltage2(cy_en_syspm_buck_voltage2_t voltage, bool waitToSettle)
{
#if 0 //TBD
    /* Do nothing if device does not have the second Buck output (SIMO) */
    if (0U != cy_device->sysPmSimoPresent)
    {
        uint32_t curVoltage;

        CY_ASSERT_L3(CY_SYSPM_IS_BUCK_VOLTAGE2_VALID(voltage));

        /* Get the current voltage */
        curVoltage = (uint32_t) Cy_SysPm_BuckGetVoltage2();

        if ((uint32_t) voltage != curVoltage)
        {
        #if 0 //TBD
            SRSS_PWR_BUCK_CTL2 = 
            _CLR_SET_FLD32U((SRSS_PWR_BUCK_CTL2), SRSS_PWR_BUCK_CTL2_BUCK_OUT2_SEL, (uint32_t) voltage);
        #endif

            /* Delay stabilizing at the new voltage is required only
            *  when changing from a lower voltage to a higher voltage.
            */
            if(waitToSettle && ((uint32_t) voltage > curVoltage))
            {
                Cy_SysLib_DelayUs(BUCK_OUT2_STABILIZATION_DELAY_US);
            }
        }
    }
#endif
}


cy_en_syspm_status_t Cy_SysPm_LdoSetVoltage(cy_en_syspm_ldo_voltage_t voltage)
{
    CY_ASSERT_L3(CY_SYSPM_IS_LDO_VOLTAGE_VALID(voltage));

    cy_en_syspm_status_t retVal = CY_SYSPM_INVALID_STATE;

    /* Change the voltage only if protection context is set to zero (PC = 0),
    *  or the device revision supports modifying registers via syscall
    */
    if (IsVoltageChangePossible())
    {
        uint32_t interruptState;
        //uint32_t trimVoltage; //TBD

        interruptState = Cy_SysLib_EnterCriticalSection();

        if (CY_SYSPM_LDO_VOLTAGE_0_9V == voltage)
        {
            /* Remove additional wakeup delay from Deep Sleep
            *  for 1.1 V LDO. Cypress ID #290172
            */
            //SRSS_PWR_TRIM_WAKE_CTL = 0UL; //TBD
            
           // trimVoltage =  SFLASH_LDO_0P9V_TRIM;//TBD

            /* Set bit of the flash voltage control register before the ULP is set */
            retVal = Cy_SysPm_WriteVoltageBitForFlash(CY_SYSPM_FLASH_VOLTAGE_BIT_ULP);

            if (CY_SYSPM_SUCCESS == retVal)
            {
                /* Update read-write margin value for the ULP mode */
                SetReadMarginTrimUlp();
            }
        }
        else
        {
            /* Configure additional wakeup delay from Deep Sleep
            *  for 1.1 V LDO. Cypress ID #290172
            */
            //SRSS_PWR_TRIM_WAKE_CTL = SFLASH_PWR_TRIM_WAKE_CTL; //TBD

           // trimVoltage = SFLASH_LDO_1P1V_TRIM;//TBD
#if 0 //TBD
            SRSS_PWR_TRIM_PWRSYS_CTL =
            _CLR_SET_FLD32U((SRSS_PWR_TRIM_PWRSYS_CTL), SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, LDO_OUT_VOLTAGE_0_95V);
#endif
            /* A delay for the supply to stabilize at the new higher voltage */
            Cy_SysLib_DelayUs(LDO_0_9V_TO_0_95V_DELAY_US);

            /* Update write assist value for the LP mode */
            SetWriteAssistTrimLp();

            retVal = CY_SYSPM_SUCCESS;
        }

        if (CY_SYSPM_SUCCESS == retVal)
        {
            /* The system may continue operating while the voltage on Vccd
            *  discharges to the new voltage. The time it takes to reach the
            *  new voltage depends on the conditions, including the load current
            *  on Vccd and the external capacitor size.
            */
            #if 0 //TBD
            SRSS_PWR_TRIM_PWRSYS_CTL =
            _CLR_SET_FLD32U((SRSS_PWR_TRIM_PWRSYS_CTL), SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM, trimVoltage);
            #endif

            if (CY_SYSPM_LDO_VOLTAGE_0_9V == voltage)
            {
                /* Update write assist value for the ULP mode */
                SetWriteAssistTrimUlp();
            }
            else
            {
                /* A delay for the supply to stabilize at the new intermediate voltage */
                Cy_SysLib_DelayUs(LDO_0_95V_TO_1_1V_DELAY_US);

                /* Update read-write margin value for the LP mode */
                SetReadMarginTrimLp();

                /* Clear bit of the flash voltage control register after
                * the LP mode is set
                */
                retVal = Cy_SysPm_WriteVoltageBitForFlash(CY_SYSPM_FLASH_VOLTAGE_BIT_LP);
            }
        }

        Cy_SysLib_ExitCriticalSection(interruptState);
    }

    return retVal;
}


cy_en_syspm_status_t Cy_SysPm_LdoSetMode(cy_en_syspm_ldo_mode_t mode)
{
    CY_ASSERT_L3(CY_SYSPM_IS_LDO_MODE_VALID(mode));

    cy_en_syspm_status_t retVal = CY_SYSPM_CANCELED;

    switch (mode)
    {
        case CY_SYSPM_LDO_MODE_NORMAL:
        {
            retVal = Cy_SysPm_SystemSetNormalRegulatorCurrent();
        }
        break;

        case CY_SYSPM_LDO_MODE_MIN:
        {
            retVal = Cy_SysPm_SystemSetMinRegulatorCurrent();
        }
        break;

        case CY_SYSPM_LDO_MODE_DISABLED:
        {
        #if 0 //TBD
            /* Disable the LDO, Deep Sleep, nWell, and Retention regulators */
            SRSS_PWR_CTL |= (_VAL2FLD(SRSS_PWR_CTL_DPSLP_REG_DIS, 1U) |
                             _VAL2FLD(SRSS_PWR_CTL_RET_REG_DIS, 1U) |
                             _VAL2FLD(SRSS_PWR_CTL_NWELL_REG_DIS, 1U) |
                             _VAL2FLD(SRSS_PWR_CTL_LINREG_DIS, 1U));
        #endif

            retVal = CY_SYSPM_SUCCESS;
        }
        break;

        default:
            retVal = CY_SYSPM_FAIL;
        break;
    }

    return retVal;
}


cy_en_syspm_ldo_mode_t Cy_SysPm_LdoGetMode(void)
{
    cy_en_syspm_ldo_mode_t retVal;

    if (!Cy_SysPm_LdoIsEnabled())
    {
        retVal = CY_SYSPM_LDO_MODE_DISABLED;
    }
    else if (Cy_SysPm_SystemIsMinRegulatorCurrentSet())
    {
        retVal = CY_SYSPM_LDO_MODE_MIN;
    }
    else
    {
        retVal = CY_SYSPM_LDO_MODE_NORMAL;
    }

    return retVal;
}


bool Cy_SysPm_RegisterCallback(cy_stc_syspm_callback_t* handler)
{
    bool retVal = false;

    /* Verify the input parameters. */
    if ((handler != NULL) && (handler->callbackParams != NULL) && (handler->callback != NULL))
    {
        uint32_t callbackRootIdx = (uint32_t) handler->type;

        /* If the callback list is not empty. */
        if (pmCallbackRoot[callbackRootIdx] != NULL)
        {
            cy_stc_syspm_callback_t* curCallback = pmCallbackRoot[callbackRootIdx];
            cy_stc_syspm_callback_t* insertPos  = curCallback;

            /* Find the callback after which the new callback is to be
             * inserted. Ensure the given callback has not been registered.
             */
            while ((NULL != curCallback->nextItm) && (curCallback != handler))
            {
                curCallback = curCallback->nextItm;
                /* Callbacks with the same order value are stored in the order
                 * they are registered.
                 */
                if (curCallback->order <= handler->order)
                {
                    insertPos = curCallback;
                }
            }
            /* If the callback has not been registered. */
            if (curCallback != handler)
            {
                /* If the callback is to be inserted at the beginning of the list. */
                if ((insertPos->prevItm == NULL) && (handler->order < insertPos->order))
                {
                    handler->nextItm = insertPos;
                    handler->prevItm = NULL;
                    handler->nextItm->prevItm = handler;
                    pmCallbackRoot[callbackRootIdx] = handler;
                }
                else
                {
                    handler->nextItm = insertPos->nextItm;
                    handler->prevItm = insertPos;

                    /* If the callback is not inserted at the end of the list. */
                    if (handler->nextItm != NULL)
                    {
                        handler->nextItm->prevItm = handler;
                    }
                    insertPos->nextItm = handler;
                }
                retVal = true;
            }
        }
        else
        {
            /* The callback list is empty. */
            pmCallbackRoot[callbackRootIdx] = handler;
            handler->nextItm = NULL;
            handler->prevItm = NULL;
            retVal = true;
        }
    }
    return retVal;
}


bool Cy_SysPm_UnregisterCallback(cy_stc_syspm_callback_t const *handler)
{
    bool retVal = false;

    if (handler != NULL)
    {
        uint32_t callbackRootIdx = (uint32_t) handler->type;
        cy_stc_syspm_callback_t* curCallback = pmCallbackRoot[callbackRootIdx];

        /* Search requested callback item in the linked list */
        while (curCallback != NULL)
        {
            /* Requested callback is found */
            if (curCallback == handler)
            {
                retVal = true;
                break;
            }

            /* Go to next callback item in the linked list */
            curCallback = curCallback->nextItm;
        }

        if (retVal)
        {
            /* Requested callback is first in the list */
            if (pmCallbackRoot[callbackRootIdx] == handler)
            {
                /* Check whether this the only callback registered */
                if (pmCallbackRoot[callbackRootIdx]->nextItm != NULL)
                {
                    pmCallbackRoot[callbackRootIdx] = pmCallbackRoot[callbackRootIdx]->nextItm;
                    pmCallbackRoot[callbackRootIdx]->prevItm = NULL;
                }
                else
                {
                    /* We had only one callback */
                    pmCallbackRoot[callbackRootIdx] = NULL;
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
        }
    }

    return retVal;
}


cy_en_syspm_status_t Cy_SysPm_ExecuteCallback(cy_en_syspm_callback_type_t type, cy_en_syspm_callback_mode_t mode)
{
    CY_ASSERT_L3(CY_SYSPM_IS_CALLBACK_TYPE_VALID(type));
    CY_ASSERT_L3(CY_SYSPM_IS_CALLBACK_MODE_VALID(mode));

    static cy_stc_syspm_callback_t* lastExecutedCallback = NULL;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;
    cy_stc_syspm_callback_t* curCallback = pmCallbackRoot[(uint32_t) type];
    cy_stc_syspm_callback_params_t curParams;

    if ((mode == CY_SYSPM_BEFORE_TRANSITION) || (mode == CY_SYSPM_CHECK_READY))
    {
        /* Execute registered callbacks with order from first registered to the
        *  last registered. Stop executing if CY_SYSPM_FAIL was returned in
        *  CY_SYSPM_CHECK_READY mode
        */
        while ((curCallback != NULL) && ((retVal != CY_SYSPM_FAIL) || (mode != CY_SYSPM_CHECK_READY)))
        {
            /* The modes defined in the .skipMode element are not executed */
            if (0UL == ((uint32_t) mode & curCallback->skipMode))
            {
                /* Update elements for local callback parameter values */
                curParams.base = curCallback->callbackParams->base;
                curParams.context = curCallback->callbackParams->context;

                retVal = curCallback->callback(&curParams, mode);

                /* Update callback pointer with value of executed callback.
                * Such update is required to execute further callbacks in
                * backward order after exit from LP mode or to undo
                * configuration after callback returned fail: from last called
                * to first registered.
                */
                lastExecutedCallback = curCallback;
            }
            curCallback = curCallback->nextItm;
        }

        if (mode == CY_SYSPM_CHECK_READY)
        {
            /* Update the pointer to  the failed callback with the result of the callback execution.
            *  If the callback fails, the value of the pointer will be updated
            *  with the address of the callback which returned CY_SYSPM_FAIL, else,
            *  it will be updated with NULL.
            */
            if(retVal == CY_SYSPM_FAIL)
            {
                failedCallback[(uint32_t) type] = lastExecutedCallback;
            }
            else
            {
                failedCallback[(uint32_t) type] = NULL;
            }
        }
    }
    else
    {
        /* Execute registered callbacks with order from lastCallback or last
        * executed to the first registered callback. Such a flow is required if
        * a previous callback function returned CY_SYSPM_FAIL or a previous
        * callback mode was CY_SYSPM_BEFORE_TRANSITION. Such an order is
        * required to undo configurations in correct backward order.
        */
        if (mode != CY_SYSPM_CHECK_FAIL)
        {
            while (curCallback->nextItm != NULL)
            {
                curCallback = curCallback->nextItm;
            }
        }
        else
        {
            /* Skip last executed callback that returns CY_SYSPM_FAIL, as this
            *  callback already knows that it failed.
            */
            curCallback = lastExecutedCallback;

            if (curCallback != NULL)
            {
                curCallback = curCallback->prevItm;
            }
        }

        /* Execute callback functions with required type and mode */
        while (curCallback != NULL)
        {
            /* The modes defined in the .skipMode element are not executed */
            if (0UL == ((uint32_t) mode & curCallback->skipMode))
            {
                /* Update elements for local callback parameter values */
                curParams.base = curCallback->callbackParams->base;
                curParams.context = curCallback->callbackParams->context;

                retVal = curCallback->callback(&curParams, mode);
            }
            curCallback = curCallback->prevItm;
        }
    }

    return retVal;
}


cy_stc_syspm_callback_t* Cy_SysPm_GetFailedCallback(cy_en_syspm_callback_type_t type)
{
    return failedCallback[(uint32_t) type];
}


void Cy_SysPm_IoUnfreeze(void)
{
    uint32_t interruptState;
    interruptState = Cy_SysLib_EnterCriticalSection();

    /* Preserve the last reset reason and wakeup polarity. Then, unfreeze I/O:
     * write PWR_HIBERNATE.FREEZE=0, .UNLOCK=0x3A, .HIBERANTE=0
     */
    SRSS_PWR_HIBERNATE = (SRSS_PWR_HIBERNATE & HIBERNATE_RETAIN_STATUS_MASK) | HIBERNATE_UNLOCK_VAL;

    /* Lock the Hibernate mode:
    * write PWR_HIBERNATE.HIBERNATE=0, UNLOCK=0x00, HIBERANTE=0
    */
    SRSS_PWR_HIBERNATE &= HIBERNATE_RETAIN_STATUS_MASK;

    /* Read register to make sure it is settled */
    (void) SRSS_PWR_HIBERNATE;

    Cy_SysLib_ExitCriticalSection(interruptState);
}


cy_en_syspm_status_t Cy_SysPm_WriteVoltageBitForFlash(cy_en_syspm_flash_voltage_bit_t value)
{
    CY_ASSERT_L3(CY_SYSPM_IS_BIT_FOR_FLASH_VALID(value));

    cy_en_syspm_status_t retVal = CY_SYSPM_CANCELED;

    uint16_t curDeviceRevision = Cy_SysLib_GetDeviceRevision();
    uint16_t curDevice = Cy_SysLib_GetDevice();
#if 0 //TBD
    /* Check the current protection context value. We can have a direct register
    *  update if protection context is = 0 */
    if ((Cy_Prot_GetActivePC(ACTIVE_BUS_MASTER) == 0U) && (curDevice == CY_SYSLIB_DEVICE_PSOC6ABLE2) &&
                                                          (curDeviceRevision <= SYSPM_DEVICE_PSOC6ABLE2_REV_0B))
    {
        FLASHC_FM_CTL_ANA_CTL0 =
        _CLR_SET_FLD32U((FLASHC_FM_CTL_ANA_CTL0), FLASHC_FM_CTL_ANA_CTL0_VCC_SEL, value);

        retVal = CY_SYSPM_SUCCESS;
    }
#endif
    /* Update the flash voltage bit using a syscall. This can be done on devices
    *  that support modifying registers via syscall.
    */
    if (((curDevice == CY_SYSLIB_DEVICE_PSOC6ABLE2) && (curDeviceRevision > SYSPM_DEVICE_PSOC6ABLE2_REV_0B)) ||
                                                       (curDevice != CY_SYSLIB_DEVICE_PSOC6ABLE2))
    {
       // uint32_t syscallCode; //TBD
        //IPC_STRUCT_Type *ipcSyscallBase = Cy_IPC_Drv_GetIpcBaseAddress(CY_IPC_CHAN_SYSCALL);//TBD

        /* Set required syscall code */
        if (curDevice == CY_SYSLIB_DEVICE_PSOC6ABLE2)
        {
        #if 0 //TBD
            syscallCode = (CY_SYSPM_FLASH_VOLTAGE_BIT_LP != value) ?
            FLASH_VOLTAGE_BIT_ULP_PSOC6ABLE2_OPCODE : FLASH_VOLTAGE_BIT_LP_PSOC6ABLE2_OPCODE;
        #endif
        }
        else
        {
        #if 0 //TBD
            syscallCode = (CY_SYSPM_FLASH_VOLTAGE_BIT_LP != value) ?
            FLASH_VOLTAGE_BIT_ULP_OPCODE : FLASH_VOLTAGE_BIT_LP_OPCODE;
        #endif
        }
        #if 0 //TBD
        /* Tries to acquire the IPC structure and pass the arguments to SROM API */
        if (Cy_IPC_Drv_SendMsgWord(ipcSyscallBase, SYSPM_IPC_NOTIFY_STRUCT0, syscallCode) == CY_IPC_DRV_SUCCESS)
        {
            /* Checks whether the IPC structure is not locked */
            while (Cy_IPC_Drv_IsLockAcquired(ipcSyscallBase))
            {
                /* Polls whether the IPC is released */
            }

            /* Check the return status of a syscall */
            uint32_t syscallStatus = Cy_IPC_Drv_ReadDataValue(ipcSyscallBase);

            if (SYSCALL_STATUS_SUCCESS == (syscallStatus & SYSCALL_STATUS_MASK))
            {
                retVal = CY_SYSPM_SUCCESS;
            }
        }
        #endif
    }

    return retVal;
}


void Cy_SysPm_SaveRegisters(cy_stc_syspm_backup_regs_t *regs)
{
    CY_ASSERT_L1(NULL != regs);
#if 0 //TBD
    /* Save the registers before Deep Sleep */
    regs->CY_SYSPM_UDB_UDBIF_BANK_CTL_REG = UDB_UDBIF_BANK_CTL;

    regs->CY_SYSPM_UDB_BCTL_MDCLK_EN_REG = UDB_BCTL_MDCLK_EN;
    regs->CY_SYSPM_UDB_BCTL_MBCLK_EN_REG = UDB_BCTL_MBCLK_EN;
    regs->CY_SYSPM_UDB_BCTL_BOTSEL_L_REG = UDB_BCTL_BOTSEL_L;
    regs->CY_SYSPM_UDB_BCTL_BOTSEL_U_REG = UDB_BCTL_BOTSEL_U;
    regs->CY_SYSPM_UDB_BCTL_QCLK_EN0_REG = UDB_BCTL_QCLK_EN_0;
    regs->CY_SYSPM_UDB_BCTL_QCLK_EN1_REG = UDB_BCTL_QCLK_EN_1;
    regs->CY_SYSPM_UDB_BCTL_QCLK_EN2_REG = UDB_BCTL_QCLK_EN_2;
#endif
}


void Cy_SysPm_RestoreRegisters(cy_stc_syspm_backup_regs_t const *regs)
{
    CY_ASSERT_L1(NULL != regs);
#if 0 //TBD    
    /* Restore the registers after Deep Sleep */
    UDB_BCTL_MDCLK_EN  = regs->CY_SYSPM_UDB_BCTL_MDCLK_EN_REG;
    UDB_BCTL_MBCLK_EN  = regs->CY_SYSPM_UDB_BCTL_MBCLK_EN_REG;
    UDB_BCTL_BOTSEL_L  = regs->CY_SYSPM_UDB_BCTL_BOTSEL_L_REG;
    UDB_BCTL_BOTSEL_U  = regs->CY_SYSPM_UDB_BCTL_BOTSEL_U_REG;
    UDB_BCTL_QCLK_EN_0 = regs->CY_SYSPM_UDB_BCTL_QCLK_EN0_REG;
    UDB_BCTL_QCLK_EN_1 = regs->CY_SYSPM_UDB_BCTL_QCLK_EN1_REG;
    UDB_BCTL_QCLK_EN_2 = regs->CY_SYSPM_UDB_BCTL_QCLK_EN2_REG;

    UDB_UDBIF_BANK_CTL = regs->CY_SYSPM_UDB_UDBIF_BANK_CTL_REG;
#endif
}


/*******************************************************************************
* Function Name: EnterDeepSleepRam
****************************************************************************//**
*
* The internal function that prepares the system for Deep Sleep and 
* restores the system after a wakeup from Deep Sleep.
*
* \param waitFor
* Selects wait for action. See \ref cy_en_syspm_waitfor_t.
*
* \return
* - true - System Deep Sleep was occurred. 
* - false - System Deep Sleep was not occurred.
*
*******************************************************************************/
#if defined (__ICCARM__)
    #pragma diag_suppress=Ta023
    __ramfunc
#else
    CY_SECTION(".cy_ramfunc") CY_NOINLINE
#endif
static bool EnterDeepSleepRam(cy_en_syspm_waitfor_t waitFor)
{
    /* Store the address of the Deep Sleep indicator into the RAM */
    //volatile uint32_t *delayDoneFlag = &FLASHC_BIST_DATA_0;

    /* Indicator of System Deep Sleep mode */
    bool retVal = false;
    #if 0 //TBD
    /* Acquire the IPC to prevent changing of the shared resources at the same time */
    while (0U == _FLD2VAL(IPC_STRUCT_ACQUIRE_SUCCESS, REG_IPC_STRUCT_ACQUIRE(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT))))
    {
        /* Wait until the IPC structure is released by another CPU */
    }
    #endif
#if 0 //TBD
#ifndef CY_PSOC6ABLE2_REV_0A_SUPPORT_DISABLE
    if (Cy_SysLib_GetDeviceRevision() == CY_SYSLIB_DEVICE_REV_0A)
    {
        /* Set the flag that the current CPU entered Deep Sleep */
        REG_IPC_STRUCT_DATA(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT)) |= CUR_CORE_DP_MASK;

        /* Change the slow and fast clock dividers only under the condition that 
        *  the other CPU is already in Deep Sleep. Cypress ID #284516
        */
        if (0U != (REG_IPC_STRUCT_DATA(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT)) & OTHER_CORE_DP_MASK))
        {
            /* Get the divider values of the slow and high clocks and store them into 
            *  the IPC data register
            */
            REG_IPC_STRUCT_DATA(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT)) = 
            (REG_IPC_STRUCT_DATA(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT)) & ((uint32_t) ~(SYSPM_CLK_DIV_MASK))) |
            (((uint32_t)(_FLD2VAL(CPUSS_CM0_CLOCK_CTL_SLOW_INT_DIV, CPUSS_CM0_CLOCK_CTL) << SYSPM_SLOW_CLK_DIV_Pos)) |
             ((uint32_t)(_FLD2VAL(CPUSS_CM4_CLOCK_CTL_FAST_INT_DIV, CPUSS_CM4_CLOCK_CTL) << SYSPM_FAST_CLK_DIV_Pos)));

            /* Increase the clock divider for the slow and fast clocks to SYSPM_CLK_DIVIDER */
            CPUSS_CM0_CLOCK_CTL = 
            _CLR_SET_FLD32U(CPUSS_CM0_CLOCK_CTL, CPUSS_CM0_CLOCK_CTL_SLOW_INT_DIV, SYSPM_CLK_DIVIDER);

            CPUSS_CM4_CLOCK_CTL = 
            _CLR_SET_FLD32U(CPUSS_CM4_CLOCK_CTL, CPUSS_CM4_CLOCK_CTL_FAST_INT_DIV, SYSPM_CLK_DIVIDER);

            /* Read the divider value to make sure it is set */
            (void) CPUSS_CM0_CLOCK_CTL;
            (void) CPUSS_CM4_CLOCK_CTL;
        }
    }
    else
#endif /* #ifndef CY_PSOC6ABLE2_REV_0A_SUPPORT_DISABLE */
#endif
    {
        /* Update pointer to the latest saved UDB structure */
        //REG_IPC_STRUCT_DATA(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT)) = (uint32_t) &bkpRegs; //TBD
    }
#if 0 //TBD
    /* Release the IPC */
    REG_IPC_STRUCT_RELEASE(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT)) = 0U;
#endif

#if 0 //TBD
#if (CY_CPU_CORTEX_M4)

    /* Store the address of the CM4 power status register */
    volatile uint32_t *cpussCm4PwrCtlAddr = &CPUSS_CM4_PWR_CTL;

    /* Repeat the WFI/WFE instruction if a wake up was not intended. 
    *  Cypress ID #272909
    */
    do
    {
#endif /* (CY_CPU_CORTEX_M4) */
#endif
        /* The CPU enters Deep Sleep mode upon execution of WFI/WFE */
        SCB_SCR |= SCB_SCR_SLEEPDEEP_Msk;

        if(waitFor != CY_SYSPM_WAIT_FOR_EVENT)
        {
            __WFI();
        }
        else
        {
            __WFE();
        #if 0 //TBD
        #if (CY_CPU_CORTEX_M4)
            /* Call the WFE instruction twice to clear the Event register 
            *  of the CM4 CPU. Cypress ID #279077
            */
            if(wasEventSent)
            {
                __WFE();
            }
            wasEventSent = true;
        #endif /* (CY_CPU_CORTEX_M4) */
        #endif
        }
#if 0 //TBD
#if (CY_CPU_CORTEX_M4)
    } while (_FLD2VAL(CPUSS_CM4_PWR_CTL_PWR_MODE, (*cpussCm4PwrCtlAddr)) == CM4_PWR_STS_RETAINED);
#endif /* (CY_CPU_CORTEX_M4) */
#endif

#if 0 //TBD
    /* Acquire the IPC to prevent changing of the shared resources at the same time */
    while (0U == _FLD2VAL(IPC_STRUCT_ACQUIRE_SUCCESS, REG_IPC_STRUCT_ACQUIRE(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT))))
    {
        /* Wait until the IPC structure is released by another CPU */
    }
#endif
#if 0 //TBD
#ifndef CY_PSOC6ABLE2_REV_0A_SUPPORT_DISABLE
    if (Cy_SysLib_GetDeviceRevision() == CY_SYSLIB_DEVICE_REV_0A)
    {
        /* Read and change the slow and fast clock dividers only under the condition 
        * that the other CPU is already in Deep Sleep. Cypress ID #284516
        */
        if (0U != (REG_IPC_STRUCT_DATA(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT)) & OTHER_CORE_DP_MASK))
        {
            /* Restore the clock dividers for the slow and fast clocks */
            CPUSS_CM0_CLOCK_CTL = 
            _CLR_SET_FLD32U(CPUSS_CM0_CLOCK_CTL, CPUSS_CM0_CLOCK_CTL_SLOW_INT_DIV, 
                           (_FLD2VAL(SYSPM_SLOW_CLK_DIV, REG_IPC_STRUCT_DATA(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT)))));

            CPUSS_CM4_CLOCK_CTL = 
            _CLR_SET_FLD32U(CPUSS_CM4_CLOCK_CTL, CPUSS_CM4_CLOCK_CTL_FAST_INT_DIV, 
                           (_FLD2VAL(SYSPM_FAST_CLK_DIV, REG_IPC_STRUCT_DATA(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT)))));
                           
            retVal = true;
        }

        /* Indicate that the current CPU is out of Deep Sleep */
        REG_IPC_STRUCT_DATA(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT)) &= ((uint32_t) ~CUR_CORE_DP_MASK);
    }
    else
#endif /* #ifndef CY_PSOC6ABLE2_REV_0A_SUPPORT_DISABLE */
#endif
    {
#if 0 //TBD
        /* Set 10 uS delay only under condition that the FLASHC_BIST_DATA[0] is 
        *  cleared. Cypress ID #288510
        */
        if (*delayDoneFlag == NEED_DELAY)
        {
            uint32_t ddftSlowCtl;
            uint32_t clkOutputSlow;
            uint32_t ddftFastCtl;

            /* Save timer configuration */
            ddftSlowCtl   = SRSS_TST_DDFT_SLOW_CTL_REG;
            clkOutputSlow = SRSS_CLK_OUTPUT_SLOW;
            ddftFastCtl   = SRSS_TST_DDFT_FAST_CTL_REG;

            /* Configure the counter to be sourced by IMO */
            SRSS_TST_DDFT_SLOW_CTL_REG = SRSS_TST_DDFT_SLOW_CTL_MASK;
            SRSS_CLK_OUTPUT_SLOW       = CLK_OUTPUT_SLOW_MASK;
            SRSS_TST_DDFT_FAST_CTL_REG = TST_DDFT_FAST_CTL_MASK;

            /* Load the down-counter to count the 10 us */
            SRSS_CLK_CAL_CNT1 = IMO_10US_DELAY;

            while (0U == (SRSS_CLK_CAL_CNT1 & SRSS_CLK_CAL_CNT1_CAL_COUNTER_DONE_Msk))
            {
                /* Wait until the counter stops counting */
            }

            /* Indicate that delay was done */
            *delayDoneFlag = DELAY_DONE;
            
            /* Restore timer configuration */
            SRSS_TST_DDFT_SLOW_CTL_REG = ddftSlowCtl;
            SRSS_CLK_OUTPUT_SLOW       = clkOutputSlow;
            SRSS_TST_DDFT_FAST_CTL_REG = ddftFastCtl;
            
            retVal = true;
        }
#endif
    }
#if 0 //TBD
    /* Release the IPC */
    REG_IPC_STRUCT_RELEASE(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT)) = 0U;
#endif
    return retVal;
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
    if (Cy_SysLib_GetDevice() == CY_SYSLIB_DEVICE_PSOC6ABLE2)
    {
    #if 0 //TBD
        CPUSS_TRIM_RAM_CTL = (CPUSS_TRIM_RAM_CTL & ((uint32_t) ~CPUSS_TRIM_RAM_CTL_TRIM_Msk)) |
                             (CPUSS_TRIM_RAM_ULP & CPUSS_TRIM_RAM_CTL_TRIM_Msk);

        CPUSS_TRIM_ROM_CTL = (CPUSS_TRIM_ROM_CTL & ((uint32_t) ~CPUSS_TRIM_ROM_CTL_TRIM_Msk)) |
                             (CPUSS_TRIM_ROM_ULP & CPUSS_TRIM_ROM_CTL_TRIM_Msk);
    #endif
    }
    else
    {
        #if 0 //TBD
        CPUSS_TRIM_RAM_CTL = (SFLASH_CPUSS_TRIM_RAM_CTL_HALF_ULP & ((uint32_t) ~CPUSS_TRIM_RAM_CTL_RA_MASK)) | 
                             (CPUSS_TRIM_RAM_CTL | CPUSS_TRIM_RAM_CTL_RA_MASK);

        CPUSS_TRIM_ROM_CTL = SFLASH_CPUSS_TRIM_ROM_CTL_HALF_ULP;
        #endif
    }
}


/*******************************************************************************
* Function Name: SetReadMarginTrimLp
****************************************************************************//**
*
* The internal function that updates the read-margin trim values for the 
* RAM and ROM. The trim update is done during transition of regulator voltage 
* from a lower to a higher one.
*
*******************************************************************************/
static void SetReadMarginTrimLp(void)
{
    /* Update read-write margin value for the LP mode. Cypress ID#297292 */
    if (Cy_SysLib_GetDevice() == CY_SYSLIB_DEVICE_PSOC6ABLE2)
    {
        #if 0 //TBD
        CPUSS_TRIM_RAM_CTL = (CPUSS_TRIM_RAM_CTL & ((uint32_t) ~CPUSS_TRIM_RAM_CTL_RM_Msk)) |
                             (CPUSS_TRIM_RAM_LP & CPUSS_TRIM_RAM_CTL_RM_Msk);

        CPUSS_TRIM_ROM_CTL = (CPUSS_TRIM_ROM_CTL & ((uint32_t) ~CPUSS_TRIM_ROM_CTL_RM_Msk)) |
                             (CPUSS_TRIM_ROM_LP & CPUSS_TRIM_ROM_CTL_RM_Msk);
        #endif
    }
    else
    {
        #if 0 //TBD
        CPUSS_TRIM_RAM_CTL = (SFLASH_CPUSS_TRIM_ROM_CTL_LP & ((uint32_t) ~CPUSS_TRIM_RAM_CTL_RA_MASK)) | 
                             (CPUSS_TRIM_RAM_CTL | CPUSS_TRIM_RAM_CTL_RA_MASK);

        CPUSS_TRIM_ROM_CTL =  SFLASH_CPUSS_TRIM_ROM_CTL_LP;
        #endif
    }
}


/*******************************************************************************
* Function Name: SetWriteAssistTrimUlp
****************************************************************************//**
*
* The internal function that updates the write assistant trim value for the 
* RAM. The trim update is done during transition of regulator voltage 
* from higher to a lower.
*
*******************************************************************************/
static void SetWriteAssistTrimUlp(void)
{
    /* Update write assist value for the LP mode. Cypress ID#297292 */
    if (Cy_SysLib_GetDevice() == CY_SYSLIB_DEVICE_PSOC6ABLE2)
    {
        #if 0 //TBD
        CPUSS_TRIM_RAM_CTL = (CPUSS_TRIM_RAM_CTL & ((uint32_t) ~CPUSS_TRIM_RAM_CTL_WA_Msk)) |
                             (CPUSS_TRIM_RAM_ULP & CPUSS_TRIM_RAM_CTL_WA_Msk);
        #endif
    }
    else
    {
        #if 0 //TBD
        CPUSS_TRIM_RAM_CTL = (SFLASH_CPUSS_TRIM_ROM_CTL_ULP & ((uint32_t) ~CPUSS_TRIM_RAM_CTL_RA_MASK)) |
                             (CPUSS_TRIM_RAM_CTL | CPUSS_TRIM_RAM_CTL_RA_MASK);
        #endif
    }
}


/*******************************************************************************
* Function Name: SetWriteAssistTrimLp
****************************************************************************//**
*
* The internal function that updates the write assistant trim value for the 
* RAM. The trim update is done during transition of regulator voltage 
* from lower to a higher one.
*
*******************************************************************************/
static void SetWriteAssistTrimLp(void)
{
    /* Update write assist value for the LP mode. Cypress ID#297292 */
    if (Cy_SysLib_GetDevice() == CY_SYSLIB_DEVICE_PSOC6ABLE2)
    {
        #if 0 //TBD
        CPUSS_TRIM_RAM_CTL = (CPUSS_TRIM_RAM_CTL & ((uint32_t) ~CPUSS_TRIM_RAM_CTL_WA_Msk)) |
                             (CPUSS_TRIM_RAM_LP & CPUSS_TRIM_RAM_CTL_WA_Msk);
        #endif
    }
    else
    {
        #if 0 //TBD
        CPUSS_TRIM_RAM_CTL = (SFLASH_CPUSS_TRIM_ROM_CTL_HALF_LP & ((uint32_t) ~CPUSS_TRIM_RAM_CTL_RA_MASK)) |
                             (CPUSS_TRIM_RAM_CTL | CPUSS_TRIM_RAM_CTL_RA_MASK);

        CPUSS_TRIM_ROM_CTL = SFLASH_CPUSS_TRIM_ROM_CTL_HALF_LP;
        #endif
    }
}


/*******************************************************************************
* Function Name: IsVoltageChangePossible
****************************************************************************//**
*
* The internal function that checks wherever it is possible to change the core 
* voltage. The voltage change is possible only when the protection context is 
* set to zero (PC = 0), or the device supports modifying registers via syscall.
*
*******************************************************************************/
static bool IsVoltageChangePossible(void)
{
#if 0 //TBD
    bool retVal = true;

    if (Cy_SysLib_GetDevice() == CY_SYSLIB_DEVICE_PSOC6ABLE2)
    {
        uint32_t curProtContext = Cy_Prot_GetActivePC(ACTIVE_BUS_MASTER);

        retVal = ((Cy_SysLib_GetDeviceRevision() > SYSPM_DEVICE_PSOC6ABLE2_REV_0B) || (curProtContext == 0U));
    }

    return retVal;
#endif
    return false;

}

bool Cy_SysPm_Cm4IsActive(void)
{
    return ((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM4_ACTIVE) != 0U);
}


bool Cy_SysPm_Cm4IsSleep(void)
{
    return ((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM4_SLEEP) != 0U);
}


bool Cy_SysPm_Cm4IsDeepSleep(void)
{
    return ((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM4_DEEPSLEEP) != 0U);
}


bool Cy_SysPm_Cm0IsActive(void)
{
    return ((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM0_ACTIVE) != 0U);
}


bool Cy_SysPm_Cm0IsSleep(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM0_SLEEP) != 0U);
}


bool Cy_SysPm_Cm0IsDeepSleep(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_CM0_DEEPSLEEP) != 0U);
}


bool Cy_SysPm_IsSystemLp(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_SYSTEM_LP) != 0U);
}


bool Cy_SysPm_IsSystemUlp(void)
{
    return((Cy_SysPm_ReadStatus() & CY_SYSPM_STATUS_SYSTEM_ULP) != 0U);
}


void Cy_SysPm_CpuSendWakeupEvent(void)
{
    __SEV();
}


bool Cy_SysPm_SystemIsMinRegulatorCurrentSet(void)
{
    return false;
}


bool Cy_SysPm_BuckIsEnabled(void)
{
//TBD
//    return (0U != _FLD2VAL(SRSS_PWR_BUCK_CTL_BUCK_EN, SRSS_PWR_BUCK_CTL));
    return false;
}


cy_en_syspm_buck_voltage1_t Cy_SysPm_BuckGetVoltage1(void)
{
    uint32_t retVal = 0;

    return ((cy_en_syspm_buck_voltage1_t) retVal);
}


cy_en_syspm_buck_voltage2_t Cy_SysPm_BuckGetVoltage2(void)
{

    uint32_t retVal = 0UL;
#if 0 //TBD

    if (0U != cy_device->sysPmSimoPresent)
    {
        retVal = _FLD2VAL(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_SEL, SRSS_PWR_BUCK_CTL2);
    }
#endif

    return ((cy_en_syspm_buck_voltage2_t) retVal);

 
}


void Cy_SysPm_BuckDisableVoltage2(void)
{
#if 0 //TBD

    if (0U != cy_device->sysPmSimoPresent)
    {
        /* Disable the Vbuck2 output */
        SRSS_PWR_BUCK_CTL2 &= (uint32_t) ~_VAL2FLD(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_EN, 1U);
    }
#endif
}


void Cy_SysPm_BuckSetVoltage2HwControl(bool hwControl)
{
#if 0 //TBD

    bool isBuckEnabled = Cy_SysPm_BuckIsEnabled();
    
    if ((0U != cy_device->sysPmSimoPresent) && isBuckEnabled)
    {
        if(hwControl)
        {
            SRSS_PWR_BUCK_CTL2 |= _VAL2FLD(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_HW_SEL, 1U);
        }
        else
        {
            SRSS_PWR_BUCK_CTL2 &= (uint32_t) ~_VAL2FLD(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_HW_SEL, 1U);
        }
    }
#endif
}


bool Cy_SysPm_BuckIsVoltage2HwControlled(void)
{
    bool retVal = false;
#if 0 //TBD

    if (0U != cy_device->sysPmSimoPresent)
    {
        retVal = (0U != _FLD2VAL(SRSS_PWR_BUCK_CTL2_BUCK_OUT2_HW_SEL, SRSS_PWR_BUCK_CTL2));
    }
#endif
    return retVal;
}


cy_en_syspm_ldo_voltage_t Cy_SysPm_LdoGetVoltage(void)
{
    return (cy_en_syspm_ldo_voltage_t)0;//TBD
}


bool Cy_SysPm_LdoIsEnabled(void)
{
    return false;
}


bool Cy_SysPm_IoIsFrozen(void)
{
    return (0U != _FLD2VAL(SRSS_PWR_HIBERNATE_FREEZE, SRSS_PWR_HIBERNATE));
}


void Cy_SysPm_PmicEnable(void)
{
#if 0 //TBD
    if (CY_SYSPM_PMIC_UNLOCK_KEY == _FLD2VAL(BACKUP_PMIC_CTL_UNLOCK, BACKUP_PMIC_CTL))
    {
        BACKUP_PMIC_CTL =
        _VAL2FLD(BACKUP_PMIC_CTL_UNLOCK, CY_SYSPM_PMIC_UNLOCK_KEY) |
        _VAL2FLD(BACKUP_PMIC_CTL_PMIC_EN_OUTEN, 1U) |
        _VAL2FLD(BACKUP_PMIC_CTL_PMIC_EN, 1U);
    }
#endif
}


void Cy_SysPm_PmicDisable(cy_en_syspm_pmic_wakeup_polarity_t polarity)
{
#if 0 //TBD
    CY_ASSERT_L3(CY_SYSPM_IS_POLARITY_VALID(polarity));
    
    if (CY_SYSPM_PMIC_UNLOCK_KEY == _FLD2VAL(BACKUP_PMIC_CTL_UNLOCK, BACKUP_PMIC_CTL))
    {
        BACKUP_PMIC_CTL = 
        (_VAL2FLD(BACKUP_PMIC_CTL_UNLOCK, CY_SYSPM_PMIC_UNLOCK_KEY) | 
         _CLR_SET_FLD32U(BACKUP_PMIC_CTL, BACKUP_PMIC_CTL_POLARITY, (uint32_t) polarity)) &
        ((uint32_t) ~ _VAL2FLD(BACKUP_PMIC_CTL_PMIC_EN, 1U));
    }
#endif
}


void Cy_SysPm_PmicAlwaysEnable(void)
{
#if 0 //TBD
    BACKUP_PMIC_CTL |= _VAL2FLD(BACKUP_PMIC_CTL_PMIC_ALWAYSEN, 1U);
#endif
}


void Cy_SysPm_PmicEnableOutput(void)
{
#if 0 //TBD
    if (CY_SYSPM_PMIC_UNLOCK_KEY == _FLD2VAL(BACKUP_PMIC_CTL_UNLOCK, BACKUP_PMIC_CTL))
    {
        BACKUP_PMIC_CTL |=
        _VAL2FLD(BACKUP_PMIC_CTL_UNLOCK, CY_SYSPM_PMIC_UNLOCK_KEY) | _VAL2FLD(BACKUP_PMIC_CTL_PMIC_EN_OUTEN, 1U);
    }
#endif
}


void Cy_SysPm_PmicDisableOutput(void)
{
#if 0 //TBD
    if (CY_SYSPM_PMIC_UNLOCK_KEY == _FLD2VAL(BACKUP_PMIC_CTL_UNLOCK, BACKUP_PMIC_CTL))
    {
        BACKUP_PMIC_CTL = 
        (BACKUP_PMIC_CTL | _VAL2FLD(BACKUP_PMIC_CTL_UNLOCK, CY_SYSPM_PMIC_UNLOCK_KEY)) &
        ((uint32_t) ~ _VAL2FLD(BACKUP_PMIC_CTL_PMIC_EN_OUTEN, 1U));
    } 
#endif
}


void Cy_SysPm_PmicLock(void)
{
#if 0 //TBD
    BACKUP_PMIC_CTL = _CLR_SET_FLD32U(BACKUP_PMIC_CTL, BACKUP_PMIC_CTL_UNLOCK, 0U);
#endif
}


void Cy_SysPm_PmicUnlock(void)
{
#if 0 //TBD
    BACKUP_PMIC_CTL = _CLR_SET_FLD32U(BACKUP_PMIC_CTL, BACKUP_PMIC_CTL_UNLOCK, CY_SYSPM_PMIC_UNLOCK_KEY);
#endif
}


bool Cy_SysPm_PmicIsEnabled(void)
{
#if 0 //TBD
    return (0U != _FLD2VAL(BACKUP_PMIC_CTL_PMIC_EN, BACKUP_PMIC_CTL));
#endif
    return false;
}


bool Cy_SysPm_PmicIsOutputEnabled(void)
{
#if 0 //TBD
    return (0U != _FLD2VAL(BACKUP_PMIC_CTL_PMIC_EN_OUTEN, BACKUP_PMIC_CTL));
#endif
    return false;
}


bool Cy_SysPm_PmicIsLocked(void)
{
#if 0 //TBD
    return ((_FLD2VAL(BACKUP_PMIC_CTL_UNLOCK, BACKUP_PMIC_CTL) == CY_SYSPM_PMIC_UNLOCK_KEY) ? false : true);
#endif
    return false;
}


void Cy_SysPm_BackupSetSupply(cy_en_syspm_vddbackup_control_t vddBackControl)
{
#if 0 //TBD
    CY_ASSERT_L3(CY_SYSPM_IS_VDDBACKUP_VALID(vddBackControl));

    BACKUP_CTL = _CLR_SET_FLD32U((BACKUP_CTL), BACKUP_CTL_VDDBAK_CTL, (uint32_t) vddBackControl);
#endif
}


cy_en_syspm_vddbackup_control_t Cy_SysPm_BackupGetSupply(void)
{
    uint32_t retVal = 0;
#if 0 //TBD
    retVal = _FLD2VAL(BACKUP_CTL_VDDBAK_CTL, BACKUP_CTL);
#endif
    return ((cy_en_syspm_vddbackup_control_t) retVal);
}


void Cy_SysPm_BackupEnableVoltageMeasurement(void)
{
#if 0 //TBD
    BACKUP_CTL |= BACKUP_CTL_VBACKUP_MEAS_Msk;
#endif
}


void Cy_SysPm_BackupDisableVoltageMeasurement(void)
{
#if 0 //TBD
    BACKUP_CTL &= ((uint32_t) ~BACKUP_CTL_VBACKUP_MEAS_Msk);
#endif
}


void Cy_SysPm_BackupSuperCapCharge(cy_en_syspm_sc_charge_key_t key)
{
#if 0 //TBD
    CY_ASSERT_L3(CY_SYSPM_IS_SC_CHARGE_KEY_VALID(key));
    
    if(key == CY_SYSPM_SC_CHARGE_ENABLE)
    {
        BACKUP_CTL = _CLR_SET_FLD32U((BACKUP_CTL), BACKUP_CTL_EN_CHARGE_KEY, (uint32_t) CY_SYSPM_SC_CHARGE_ENABLE);
    }
    else
    {
        BACKUP_CTL &= ((uint32_t) ~BACKUP_CTL_EN_CHARGE_KEY_Msk);
    }
#endif
}

#endif
/* [] END OF FILE */
