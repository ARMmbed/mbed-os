/***************************************************************************//**
* \file system_psoc6ble_cm0plus.h
* \version 1.0
*
* \brief Device system header file.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef _SYSTEM_PSOC6BLE_CM0PLUS_H_
#define _SYSTEM_PSOC6BLE_CM0PLUS_H_

#define CY_CM0     ((defined(__GNUC__)       &&  (__ARM_ARCH == 6) && (__ARM_ARCH_6M__ == 1)) || \
                    (defined (__ICCARM__)     &&  (__CORE__ == __ARM6M__))  || \
                    (defined(__ARMCC_VERSION) &&  (__TARGET_ARCH_THUMB == 3)))

#if (CY_CM0)

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** startup_m0p driver identifier */
#define CY_STARTUP_M0P_ID       ((uint32_t)((uint32_t)((0x0Eu) & 0x3FFFu) << 18u))


/*******************************************************************************
* Inter Process Communication (IPC)
*******************************************************************************/

#define CY_IPC_CYPIPE_CLIENT_CNT        (8u)                /* Number of the defined clients */

#define CY_IPC_EP_MAX                   (CPUSS_IPC_IPC_NR)  /* Number of IPC structures */
#define CY_IPC_EP_CYPIPE_CM0_ADDR       (0u)
#define CY_IPC_EP_CYPIPE_CM4_ADDR       (1u)
#if (CY_CM0)
    #define CY_IPC_EP_CYPIPE_ADDR       CY_IPC_EP_CYPIPE_CM0_ADDR
#else
    #define CY_IPC_EP_CYPIPE_ADDR       CY_IPC_EP_CYPIPE_CM4_ADDR
#endif  /* (CY_CM0) */


#define CY_IPC_LOCK_COUNT               (128u)              /* Locks number. Must be a multiple of 32 */

/* IPC channels  */
#define CY_IPC_CHAN_SYSCALL_CM0         (0u)                /* System calls for the CM0 processor */
#define CY_IPC_CHAN_SYSCALL_CM4         (1u)                /* System calls for the 1st non-CM0 processor */
#if (CY_CM0)
    #define CY_IPC_CHAN_SYSCALL         CY_IPC_CHAN_SYSCALL_CM0
#else
    #define CY_IPC_CHAN_SYSCALL         CY_IPC_CHAN_SYSCALL_CM4
#endif  /* (CY_CM0) */
#define CY_IPC_CHAN_SYSCALL_DAP         (2u)                /* System call */
#define CY_IPC_CHAN_CRYPTO              (3u)                /* Crypto functions */
#define CY_IPC_CHAN_LOCK                (4u)                /* IPC Locks (System wide) */
#define CY_IPC_CHAN_CYPIPE_CM0          (5u)                /* Cypress Pipe, CM0 side */
#define CY_IPC_CHAN_CYPIPE_CM4          (6u)                /* Cypress Pipe, CM4 side */

/* IPC interrupts */
#define CY_IPC_INTR_SYSCALL_CM0         (0u)
#define CY_IPC_INTR_SYSCALL_CM4         (1u)
#define CY_IPC_INTR_SYSCALL_DAP         (2u)
#define CY_IPC_INTR_CRYPTO_M0           (3u)
#define CY_IPC_INTR_CRYPTO_M4           (4u)
#define CY_IPC_INTR_CYPIPE_CM0          (5u)
#define CY_IPC_INTR_CYPIPE_CM4          (6u)

/* IPC CM0 Interrupts  */
#if (CY_CM0)
    #define CY_IPC_INTR_CYPIPE_SRC      cpuss_interrupts_ipc5_IRQn
    #define CY_IPC_INTR_CYPIPE_CM0SRC   NvicMux28
    #define CY_IPC_INTR_CYPIPE_PRIO     (1u)
#else
    #define CY_IPC_INTR_CYPIPE_SRC      cpuss_interrupts_ipc6_IRQn
    #define CY_IPC_INTR_CYPIPE_CM0SRC   (240u)              /* Default value of CM0_INT_CTLx register */
    #define CY_IPC_INTR_CYPIPE_PRIO     (1u)
#endif

/* The System pipe configuration defines the IPC channel number, interrupt number and the pipe interrupt mask */
#define CY_IPC_CYPIPE_INTR_MASK         (uint32_t)((0x0001ul << CY_IPC_CHAN_CYPIPE_CM0) | (0x0001ul << CY_IPC_CHAN_CYPIPE_CM4) )

#if (CY_CM0)
    #define CY_IPC_CYPIPE_CONFIG        (uint32_t)( (CY_IPC_CYPIPE_INTR_MASK << 16) | (CY_IPC_INTR_CYPIPE_CM0 << 8u) | CY_IPC_CHAN_CYPIPE_CM0)
#else
    #define CY_IPC_CYPIPE_CONFIG        (uint32_t)( (CY_IPC_CYPIPE_INTR_MASK << 16) | (CY_IPC_INTR_CYPIPE_CM4 << 8u) | CY_IPC_CHAN_CYPIPE_CM4)
#endif


/**
* \addtogroup group_startup_functions
* \{
*/
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
/** \} group_startup_functions */


/**
* \addtogroup group_startup_functions
* \{
*/
extern uint32_t Cy_SysGetCM4Status(void);
extern void     Cy_SysEnableCM4(uint32_t vectorTableOffset);
extern void     Cy_SysDisableCM4(void);
extern void     Cy_SysRetainCM4(void);
extern void     Cy_SysResetCM4(void);
/** \} group_startup_functions */


/** \cond */
extern void     Default_Handler (void);
extern uint32_t Cy_SaveIRQ(void);
extern void     Cy_RestoreIRQ(uint32_t saved);

extern void     Cy_SMIF_StartupCallBack(void);
extern void     Cy_SystemInit(void);
extern void     Cy_SystemInitIpc(void);
extern void     Cy_SystemInitFpuEnable(void);

extern uint32_t cy_delayFreqHz;
extern uint32_t cy_delayFreqKhz;
extern uint8_t  cy_delayFreqMhz;
extern uint32_t cy_delay32kMs;

extern uint32_t cy_BleEcoClockFreqHz;
extern uint32_t cy_Hfclk0FreqHz;
extern uint32_t cy_PeriClkFreqHz;
/** \endcond */


/**
* \addtogroup group_startup_macro
* \{
*/

#ifndef CY_CORTEX_M4_APPL_ADDR
    /**  Start address of the Cortex-M4 application */
    #define CY_CORTEX_M4_APPL_ADDR  (0x10080000u)
#endif

#define CY_SYS_CM4_STATUS_ENABLED   (0u)    /**< The Cortex-M4 core is enabled. */
#define CY_SYS_CM4_STATUS_DISABLED  (1u)    /**< The Cortex-M4 core is disabled. */
#define CY_SYS_CM4_STATUS_RETAINED  (2u)    /**< The Cortex-M4 core is retained. */
#define CY_SYS_CM4_STATUS_UNKNOWN   (3u)    /**< The Cortex-M4 core is in the unknown state. Invalid state. */
#define CY_SYS_CM4_STATUS_RESET     (4u)    /**< The Cortex-M4 core is in the Reset mode. */
/** \} group_startup_macro */


/** \cond */
    #define CPUSS_CM4_PWR_CTL_Msk            (CPUSS_CM4_PWR_CTL_ISOLATE_Msk | \
                                              CPUSS_CM4_PWR_CTL_RETAIN_Msk  | CPUSS_CM4_PWR_CTL_POWER_Msk | \
                                              CPUSS_CM4_PWR_CTL_RESET_Msk   | CPUSS_CM4_PWR_CTL_CLOCK_Msk)

    #define CPUSS_CM4_PWR_CTL_DISABLED       (CPUSS_CM4_PWR_CTL_RESET_Msk  | CPUSS_CM4_PWR_CTL_ISOLATE_Msk)
    #define CPUSS_CM4_PWR_CTL_RETAINED       (CPUSS_CM4_PWR_CTL_RETAIN_Msk | CPUSS_CM4_PWR_CTL_ISOLATE_Msk)
    #define CPUSS_CM4_PWR_CTL_ENABLED        (CPUSS_CM4_PWR_CTL_CLOCK_Msk  | CPUSS_CM4_PWR_CTL_POWER_Msk)
    #define CPUSS_CM4_PWR_CTL_RESET_MODE     (CPUSS_CM4_PWR_CTL_RESET_Msk  | CPUSS_CM4_PWR_CTL_POWER_Msk)
/** \endcond */


/** \addtogroup group_startup_globals
* \{
*/

extern uint32_t SystemCoreClock;

/** \} group_startup_globals */

#ifdef __cplusplus
}
#endif

#endif /* CY_CM0 */
#endif /* _SYSTEM_PSOC6BLE_CM0PLUS_H_ */


/* [] END OF FILE */
