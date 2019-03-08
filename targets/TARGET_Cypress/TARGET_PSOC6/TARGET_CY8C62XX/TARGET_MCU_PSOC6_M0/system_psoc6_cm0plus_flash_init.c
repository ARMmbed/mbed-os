/***************************************************************************//**
* \file system_psoc6_cm0plus_flash_init.c
* \version 2.30
*
* The source for Flash initialization function.
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

#include "system_psoc6.h"
#include "cy_ipc_sema.h"
#include "cy_ipc_pipe.h"
#include "cy_flash.h"

#include "system_psoc6_cm0plus_flash_init.h"

/*******************************************************************************
* Function Name: Cy_SysIpcPipeIsrCm0Local
****************************************************************************//**
*
* This is the interrupt service routine for the system pipe.
* Copy of Cy_SysIpcPipeIsrCm0
*
*******************************************************************************/
static void Cy_SysIpcPipeIsrCm0Local(void)
{
    Cy_IPC_Pipe_ExecuteCallback(CY_IPC_EP_CYPIPE_CM0_ADDR);
}

/* Structure containing semaphores control data */
__attribute__((section(".cy_pub_ram"))) static cy_stc_ipc_sema_t cy_semaData;

static cy_en_ipcsema_status_t Cy_IPC_SemaInitLocal(uint32_t ipcChannel,
                                        uint32_t count, uint32_t memPtr[])
{
    cy_en_ipcsema_status_t retStatus = CY_IPC_SEMA_BAD_PARAM;

    /* Check for non Null pointers and count value */
    if ((NULL != memPtr) && (0u != count))
    {
        cy_semaData.maxSema  = count;
        cy_semaData.arrayPtr = memPtr;

        retStatus = Cy_IPC_Sema_InitExt(ipcChannel, &cy_semaData);
    }
    else
    {
        retStatus = CY_IPC_SEMA_BAD_PARAM;
    }

    return(retStatus);
}

__attribute__((section(".cy_pub_ram")))
    static uint32_t ipcSemaArray[CY_IPC_SEMA_COUNT / CY_IPC_SEMA_PER_WORD];
__attribute__((section(".cy_pub_ram")))
static cy_stc_ipc_pipe_ep_t systemIpcPipeEpArray[CY_IPC_MAX_ENDPOINTS];
__attribute__((section(".cy_pub_ram")))
static cy_ipc_pipe_callback_ptr_t systemIpcPipeSysCbArray[CY_SYS_CYPIPE_CLIENT_CNT];
__attribute__((section(".cy_pub_ram")))
CY_ALIGN(4) static cy_stc_flash_notify_t ipcWaitMessageStc;

void Cy_PubRamVariablesInit(void)
{
    memset(&cy_semaData, 0, sizeof(cy_semaData));
    memset(&ipcSemaArray, 0, sizeof(ipcSemaArray));
    memset(&systemIpcPipeEpArray, 0, sizeof(systemIpcPipeEpArray));
    memset(&systemIpcPipeSysCbArray, 0, sizeof(systemIpcPipeSysCbArray));
    memset(&ipcWaitMessageStc, 0, sizeof(ipcWaitMessageStc));
}

void Cy_SemaIpcFlashInit(void)
{
    /* Initialize cy_pub_ram section variables */
    Cy_PubRamVariablesInit();

    /* Allocate and initialize semaphores for the system operations. */
    (void) Cy_IPC_SemaInitLocal(CY_IPC_CHAN_SEMA, CY_IPC_SEMA_COUNT, ipcSemaArray);

    /* Create an array of endpoint structures */
    Cy_IPC_Pipe_Config(systemIpcPipeEpArray);

    static const cy_stc_ipc_pipe_config_t systemIpcPipeConfigCm0 =
    {
    /* .ep0ConfigData */
        {
            /* .ipcNotifierNumber    */  CY_IPC_INTR_CYPIPE_EP0,
            /* .ipcNotifierPriority  */  CY_SYS_INTR_CYPIPE_PRIOR_EP0,
            /* .ipcNotifierMuxNumber */  CY_SYS_INTR_CYPIPE_MUX_EP0,
            /* .epAddress            */  CY_IPC_EP_CYPIPE_CM0_ADDR,
            /* .epConfig             */  CY_SYS_CYPIPE_CONFIG_EP0
        },
    /* .ep1ConfigData */
        {
            /* .ipcNotifierNumber    */  CY_IPC_INTR_CYPIPE_EP1,
            /* .ipcNotifierPriority  */  CY_SYS_INTR_CYPIPE_PRIOR_EP1,
            /* .ipcNotifierMuxNumber */  0u,
            /* .epAddress            */  CY_IPC_EP_CYPIPE_CM4_ADDR,
            /* .epConfig             */  CY_SYS_CYPIPE_CONFIG_EP1
        },
    /* .endpointClientsCount     */  CY_SYS_CYPIPE_CLIENT_CNT,
    /* .endpointsCallbacksArray  */  systemIpcPipeSysCbArray,
    /* .userPipeIsrHandler       */  &Cy_SysIpcPipeIsrCm0Local
    };

    if (cy_device->flashPipeRequired != 0u)
    {
        Cy_IPC_Pipe_Init(&systemIpcPipeConfigCm0);
    }

    Cy_Flash_InitExt(&ipcWaitMessageStc);
}

/* [] END OF FILE */
