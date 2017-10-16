/***************************************************************************//**
* \file cy_ipc_config.c
* \version 1.10
*
*  Description:
*  This C file is not intended to be part of the IPC driver. It is the code
*  required to configure the device specific IPC channels for semaphores
*  and pipes.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "flash/cy_flash.h"
#include "ipc/cy_ipc_drv.h"
#include "ipc/cy_ipc_pipe.h"
#include "ipc/cy_ipc_sema.h"
#include "sysint/cy_sysint.h"

#include "cy_ipc_config.h"

static void Cy_Flash_NotifyHandler(uint32_t * msgPtr);

/* Create an array of endpoint structures */
static cy_stc_ipc_pipe_ep_t        cy_ipc_pipe_sysEpArray[CY_IPC_MAX_ENDPOINTS];

#define CY_CYPIPE_DEFAULT_CONFIG    \
{\
    /* .ep0ConfigData */ {\
        /* .ipcNotifierNumber    */  CY_IPC_INTR_CYPIPE_EP0,\
        /* .ipcNotifierPriority  */  CY_IPC_INTR_CYPIPE_PRIOR_EP0,\
        /* .ipcNotifierMuxNumber */  CY_IPC_INTR_CYPIPE_MUX_EP0,\
        /* .epAddress            */  CY_IPC_EP_CYPIPE_CM0_ADDR,\
        /* .epConfig             */  CY_IPC_CYPIPE_CONFIG_EP0\
    },\
    /* .ep1ConfigData */ {\
        /* .ipcNotifierNumber    */  CY_IPC_INTR_CYPIPE_EP1,\
        /* .ipcNotifierPriority  */  CY_IPC_INTR_CYPIPE_PRIOR_EP1,\
        /* .ipcNotifierMuxNumber */  0u,\
        /* .epAddress            */  CY_IPC_EP_CYPIPE_CM4_ADDR,\
        /* .epConfig             */  CY_IPC_CYPIPE_CONFIG_EP1\
    },\
    /* .endpointClientsCount     */  CY_IPC_CYPIPE_CLIENT_CNT,\
    /* .endpointsCallbacksArray  */  cy_ipc_pipe_sysCbArray,\
    /* .userPipeIsrHandler       */  &Cy_IPC_SystemPipeIsr\
}

/*******************************************************************************
* Function Name: Cy_IPC_SystemSemaInit
****************************************************************************//**
*
* Initializes the system semaphores.
*
*******************************************************************************/
void Cy_IPC_SystemSemaInit(void)
{
/* Create array used for semaphores */
#if !(CY_CPU_CORTEX_M0P)
    (void) Cy_IPC_Sema_Init(CY_IPC_CHAN_SEMA, 0ul, NULL);
#else
    static uint32_t ipcSemaArray[CY_IPC_SEMA_COUNT / CY_IPC_SEMA_PER_WORD];
    (void) Cy_IPC_Sema_Init(CY_IPC_CHAN_SEMA, CY_IPC_SEMA_COUNT, ipcSemaArray);
#endif
}

/*******************************************************************************
* Function Name: Cy_IPC_SystemPipeInit
****************************************************************************//**
*
* Initializes the system pipes. The system pipes are used by BLE.
* \note The function should be called on all CPUs.
*
*******************************************************************************/
void Cy_IPC_SystemPipeInit(void)
{
    uint32_t intr;

    intr = Cy_SysLib_EnterCriticalSection();

    static cy_ipc_pipe_callback_ptr_t cy_ipc_pipe_sysCbArray[CY_IPC_CYPIPE_CLIENT_CNT];

    static const cy_stc_ipc_pipe_config_t systemPipeConfig = CY_CYPIPE_DEFAULT_CONFIG;

    Cy_IPC_Pipe_Config(cy_ipc_pipe_sysEpArray);

    Cy_IPC_Pipe_Init(&systemPipeConfig);

    (void)Cy_IPC_Pipe_RegisterCallback(CY_IPC_EP_CYPIPE_ADDR, &Cy_Flash_NotifyHandler, (uint32_t)CY_FLASH_IPC_CLIENT_ID);

    Cy_SysLib_ExitCriticalSection(intr);
}

/*******************************************************************************
* Function Name: Cy_IPC_SystemPipeIsr
****************************************************************************//**
*
* This is the interrupt service routine for the system pipe.
*
*******************************************************************************/
void Cy_IPC_SystemPipeIsr(void)
{
    Cy_IPC_Pipe_ExecCallback(&cy_ipc_pipe_sysEpArray[CY_IPC_EP_CYPIPE_ADDR]);
}

/*******************************************************************************
* Function Name: Cy_Flash_NotifyHandler
****************************************************************************//**
*
* This is the interrupt service routine for the pipe notifications.
*
*******************************************************************************/
#if defined (__ICCARM__)
    __ramfunc 
#else
    CY_SECTION(".cy_ramfunc")
#endif
static void Cy_Flash_NotifyHandler(uint32_t * msgPtr)
{
    uint32_t intr;

    IPC_MSG *ipcMsgPtr = (IPC_MSG *)msgPtr;

    if (CY_FLASH_ENTER_WAIT_LOOP == ipcMsgPtr->pktType)
    {
        intr = Cy_SysLib_EnterCriticalSection();

        /* Notification to the Flash driver to start the current operation */
        (void) Cy_IPC_Sema_Set(CY_FLASH_WAIT_SEMA, true);

        /* Notification to me about the end of the operation */
        while (CY_IPC_SEMA_STATUS_LOCKED == Cy_IPC_Sema_Status(CY_FLASH_WAIT_SEMA))
        {
        }

        Cy_SysLib_ExitCriticalSection(intr);
    }
}

/* [] END OF FILE */

