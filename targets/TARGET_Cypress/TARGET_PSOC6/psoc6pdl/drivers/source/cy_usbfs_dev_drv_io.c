/***************************************************************************//**
* \file cy_usbfs_dev_drv_io.c
* \version 1.10
*
* Provides data transfer API implementation of the USBFS driver.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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

#include <string.h>
#include "cy_usbfs_dev_drv_pvt.h"

#ifdef CY_IP_MXUSBFS

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
*                        Internal Constants
*******************************************************************************/

/* Dedicated IN and OUT endpoints buffer: 32 bytes (2^5) */
#define ENDPOINTS_BUFFER_SIZE   (0x55UL)

/* Number of bytes transferred by 1 Y-loop */
#define DMA_YLOOP_INCREMENT     (32UL)

/* All arbiter interrupt sources */
#define ENDPOINT_ARB_INTR_SOURCES_ALL    (USBFS_USBDEV_ARB_EP_IN_BUF_FULL_Msk | \
                                          USBFS_USBDEV_ARB_EP_DMA_GNT_Msk     | \
                                          USBFS_USBDEV_ARB_EP_BUF_OVER_Msk    | \
                                          USBFS_USBDEV_ARB_EP_BUF_UNDER_Msk   | \
                                          USBFS_USBDEV_ARB_EP_ERR_Msk         | \
                                          USBFS_USBDEV_ARB_EP_DMA_TERMIN_Msk)

/* Arbiter interrupt sources for OUT and IN endpoints when mode is
* CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO.
*/
#define ENDPOINT_ARB_INTR_SOURCES_CPU       (USBFS_USBDEV_ARB_EP_ERR_Msk)

#define OUT_ENDPOINT_ARB_INTR_SOURCES_CPU   (USBFS_USBDEV_ARB_EP_ERR_Msk)

#define IN_ENDPOINT_ARB_INTR_SOURCES_DMA    (USBFS_USBDEV_ARB_EP_IN_BUF_FULL_Msk | \
                                             USBFS_USBDEV_ARB_EP_ERR_Msk)

#define OUT_ENDPOINT_ARB_INTR_SOURCES_DMA   (USBFS_USBDEV_ARB_EP_DMA_GNT_Msk | \
                                             USBFS_USBDEV_ARB_EP_ERR_Msk)


/*******************************************************************************
*                        Internal Functions Prototypes
*******************************************************************************/
static void DisableEndpoint(USBFS_Type *base, uint32_t endpoint, cy_stc_usbfs_dev_drv_context_t *context);


/*******************************************************************************
* Function Name: DisableEndpoint
****************************************************************************//**
*
* Disables endpoint operation.
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint
* The data endpoint number.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
*******************************************************************************/
static void DisableEndpoint(USBFS_Type *base, uint32_t endpoint, cy_stc_usbfs_dev_drv_context_t *context)
{
    /* Disable endpoint SIE mode and set appropriate state */
    Cy_USBFS_Dev_Drv_SetSieEpMode(base, endpoint, CY_USBFS_DEV_DRV_EP_CR_DISABLE);
    context->epPool[endpoint].state = CY_USB_DEV_EP_DISABLED;
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_ConfigDevice
****************************************************************************//**
*
* Set basic device configuration (clears previous configuration). 
* Call this function before configuring endpoints for operation.
*
* \param base
* The pointer to the USBFS instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
*******************************************************************************/
void Cy_USBFS_Dev_Drv_ConfigDevice(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context)
{
    uint32_t endpoint;

    /* Clear buffer pointer */
    context->curBufAddr = 0U;

    /* Disable SIE and Arbiter interrupts */
    USBFS_DEV_SIE_EP_INT_EN(base) = 0UL;
    USBFS_DEV_ARB_INT_EN(base)    = 0UL;

    /* Prepare endpoints for configuration */
    for (endpoint = 0U; endpoint <  CY_USBFS_DEV_DRV_NUM_EPS_MAX; ++endpoint)
    {
        /* Disable endpoint operation */
        DisableEndpoint(base, endpoint, context);

        /* Set default arbiter configuration */
        Cy_USBFS_Dev_Drv_SetArbEpConfig(base, endpoint, (USBFS_USBDEV_ARB_EP1_CFG_CRC_BYPASS_Msk |
                                                         USBFS_USBDEV_ARB_EP1_CFG_RESET_PTR_Msk));
    }

    if (CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU != context->mode)
    {
        uint32_t regVal = _VAL2FLD(USBFS_USBDEV_ARB_CFG_DMA_CFG, context->mode);

        if (CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO == context->mode)
        {
            regVal |=  USBFS_USBDEV_ARB_CFG_AUTO_MEM_Msk;
        }

        /* Allow re-configuration: clear configuration complete bit */
        USBFS_DEV_ARB_CFG(base) = regVal;
        (void) USBFS_DEV_ARB_CFG(base);

        if (CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO == context->mode)
        {
            /* Reset configuration registers */
            USBFS_DEV_EP_ACTIVE(base) = 0UL;
            USBFS_DEV_EP_TYPE(base)   = CY_USBFS_DEV_DRV_WRITE_ODD(0UL);

            /* Configure DMA burst size */
            USBFS_DEV_BUF_SIZE(base)    = ENDPOINTS_BUFFER_SIZE;
            USBFS_DEV_DMA_THRES16(base) = DMA_YLOOP_INCREMENT;
        }
    }
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_UnConfigureDevice
****************************************************************************//**
*
* Clear device configuration. 
* Call this function if endpoints configuration fail.
*
* \param base
* The pointer to the USBFS instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
*******************************************************************************/
void Cy_USBFS_Dev_Drv_UnConfigureDevice(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context)
{
    uint32_t endpoint;

    /* Clear endpoints configuration */
    for (endpoint = 0U; endpoint <  CY_USBFS_DEV_DRV_NUM_EPS_MAX; ++endpoint)
    {
        /* Disable endpoint operation */
        DisableEndpoint(base, endpoint, context);
    }
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_ConfigDeviceComplete
****************************************************************************//**
*
* Locks device configuration before following operation.
*
* \param base
* The pointer to the USBFS instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
*******************************************************************************/
void Cy_USBFS_Dev_Drv_ConfigDeviceComplete(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t const *context)
{
    if (CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU != context->mode)
    {
        /* Configuration complete: set configuration complete bit (generate rising edge) */
        USBFS_DEV_ARB_CFG(base) |= USBFS_USBDEV_ARB_CFG_CFG_CMP_Msk;
    }
}


/*******************************************************************************
* Function Name: GetEndpointBuffer
****************************************************************************//**
*
* Returns start position in the allocated buffer for data endpoint.
*
* \param size
* Data endpoint buffer size.
*
* \param idx
* Start position of the allocated endpoint buffer.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* Status code of the function execution \ref cy_en_usbfs_dev_drv_status_t.
*
*******************************************************************************/
cy_en_usbfs_dev_drv_status_t GetEndpointBuffer(uint32_t size, uint32_t *idx, cy_stc_usbfs_dev_drv_context_t *context)
{
    cy_en_usbfs_dev_drv_status_t retStatus = CY_USBFS_DEV_DRV_BUF_ALLOC_FAILED;
    uint32_t bufSize;
    uint32_t nextBufAddr;

    /* Get max buffer size */
    bufSize = (CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO != context->mode) ? 
                CY_USBFS_DEV_DRV_HW_BUFFER_SIZE : 
                context->epSharedBufSize;

    /* Get next buffer address. Note the end buffer size must be even for 16-bit access */
    nextBufAddr  = (context->curBufAddr + size);
    nextBufAddr += (context->useReg16) ? (size & 0x01U) : 0U;

    /* Check whether there is enough space in the buffer */
    if (nextBufAddr <= bufSize)
    {
        /* Update pointers */
        *idx = context->curBufAddr;
        context->curBufAddr = (uint16_t) nextBufAddr;

        retStatus = CY_USBFS_DEV_DRV_SUCCESS;
    }

    return retStatus;
}


/*******************************************************************************
* Function Name: RestoreEndpointHwBuffer
****************************************************************************//**
*
* Restores endpoint active configuration for 
* \ref CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU and 
* \ref CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA modes.
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint
* The OUT data endpoint number.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
*******************************************************************************/
void RestoreEndpointHwBuffer(USBFS_Type *base, 
                             cy_en_usbfs_dev_drv_ep_management_mode_t mode,
                             cy_stc_usbfs_dev_drv_endpoint_data_t *endpointData)
{
    bool inDirection  = IS_EP_DIR_IN(endpointData->address);
    uint32_t endpoint = EPADDR2PHY(endpointData->address);

    /* Clear state: MEM_DATA is not non-retention */
    endpointData->state = CY_USB_DEV_EP_IDLE;

    /* Set Arbiter read and write pointers */
    Cy_USBFS_Dev_Drv_SetArbWriteAddr(base, endpoint, (uint32_t) endpointData->startBuf);
    Cy_USBFS_Dev_Drv_SetArbReadAddr (base, endpoint, (uint32_t) endpointData->startBuf);

    /* Enable endpoint Arbiter interrupt sources */
    if (CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA == mode)
    {
        /* Enable Arbiter interrupt sources for endpoint */
        Cy_USBFS_Dev_Drv_SetArbEpInterruptMask(base, endpoint, (inDirection ?
                                                                IN_ENDPOINT_ARB_INTR_SOURCES_DMA :
                                                                OUT_ENDPOINT_ARB_INTR_SOURCES_DMA));

        /* Enable Arbiter interrupt for endpoint */
        Cy_USBFS_Dev_Drv_EnableArbEpInterrupt(base, endpoint);

        /* Enable DMA channel */
         Cy_DMA_Channel_Enable(endpointData->base, endpointData->chNum);
    }
    else
    {
        /* Enable error interrupt triggered by SIE */
        Cy_USBFS_Dev_Drv_SetArbEpInterruptMask(base, endpoint, ENDPOINT_ARB_INTR_SOURCES_CPU);
    }

    /* Enable SIE interrupt for endpoint */
    Cy_USBFS_Dev_Drv_EnableSieEpInterrupt(base, endpoint);

    /* Set endpoint mode to not respond to host */
    Cy_USBFS_Dev_Drv_SetSieEpMode(base, endpoint, GetEndpointInactiveMode((uint32_t) endpointData->sieMode));
}


/*******************************************************************************
* Function Name: AddEndpointHwBuffer
****************************************************************************//**
*
* Implements \ref Cy_USBFS_Dev_Drv_AddEndpoint for 
* \ref CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU and 
* \ref CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA modes.
*
* \param base
* The pointer to the USBFS instance.
*
* \param config
* The pointer to data endpoint configuration \ref cy_stc_usb_dev_ep_config_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* Status code of the function execution \ref cy_en_usbfs_dev_drv_status_t.
*
*******************************************************************************/
cy_en_usbfs_dev_drv_status_t AddEndpointHwBuffer(USBFS_Type *base,
                                                 cy_stc_usb_dev_ep_config_t const *config,
                                                 cy_stc_usbfs_dev_drv_context_t   *context)
{
    uint32_t endpoint = EPADDR2PHY(config->endpointAddr);

    /* Get pointer to endpoint data */
    cy_stc_usbfs_dev_drv_endpoint_data_t *endpointData = &context->epPool[endpoint]; 

    /* Get buffer for endpoint using hardware buffer */
    if (config->allocBuffer)
    {
        uint32_t bufOffset;

        /* Get buffer for endpoint */
        cy_en_usbfs_dev_drv_status_t retStatus = GetEndpointBuffer((uint32_t) config->bufferSize, &bufOffset, context);
        if (CY_USBFS_DEV_DRV_SUCCESS != retStatus)
        {
            return retStatus;
        }

        /* Set Arbiter read and write pointers */
        endpointData->startBuf = (uint16_t) bufOffset;
        Cy_USBFS_Dev_Drv_SetArbWriteAddr(base, endpoint, bufOffset);
        Cy_USBFS_Dev_Drv_SetArbReadAddr (base, endpoint, bufOffset);
    }

    /* Enable endpoint for the operation */
    if (config->enableEndpoint)
    {
        bool inDirection = IS_EP_DIR_IN(config->endpointAddr);

        /* Configure endpoint */
        endpointData->state   = CY_USB_DEV_EP_IDLE;
        endpointData->address = config->endpointAddr;
        endpointData->toggle  = 0U;
        endpointData->bufferSize = config->maxPacketSize;
        endpointData->sieMode = GetEndpointActiveMode(inDirection, config->attributes);
        endpointData->isPending = false;

        /* Flush IN endpoint buffer to discard loaded data. 
        * It happens when: alternate settings change is requested and IN 
        * endpoint buffer is full (not read by the Host).
        */
        if (inDirection)
        {
            Cy_USBFS_Dev_Drv_FlushInBuffer(base, endpoint);
        }

        /* Enable endpoint arbiter interrupt sources */
        if (CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA == context->mode)
        {
            /* Configure DMA based on endpoint direction */
            cy_en_usbfs_dev_drv_status_t retStatus = DmaEndpointInit(base, context->mode, context->useReg16, &context->epPool[endpoint]);
            if (CY_USBFS_DEV_DRV_SUCCESS != retStatus)
            {
                return retStatus;
            }

            /* Enable arbiter interrupt sources for endpoint */
            Cy_USBFS_Dev_Drv_SetArbEpInterruptMask(base, endpoint, (inDirection ?
                                                                    IN_ENDPOINT_ARB_INTR_SOURCES_DMA :
                                                                    OUT_ENDPOINT_ARB_INTR_SOURCES_DMA));

            /* Enable arbiter interrupt for endpoint */
            Cy_USBFS_Dev_Drv_EnableArbEpInterrupt(base, endpoint);
        }
        else
        {
            /* Enable error interrupt triggered by SIE */
            Cy_USBFS_Dev_Drv_SetArbEpInterruptMask(base, endpoint, ENDPOINT_ARB_INTR_SOURCES_CPU);
        }

        /* Enable SIE interrupt for endpoint */
        Cy_USBFS_Dev_Drv_EnableSieEpInterrupt(base, endpoint);

        /* Set endpoint mode to not respond to host */
        Cy_USBFS_Dev_Drv_SetSieEpMode(base, endpoint, GetEndpointInactiveMode((uint32_t) endpointData->sieMode));
    }

    return CY_USBFS_DEV_DRV_SUCCESS;
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_RemoveEndpoint
****************************************************************************//**
*
* Removes data endpoint (release hardware resources allocated by data endpoint).
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpointAddr
* The data endpoint address (7 bit - direction, 3-0 bits - endpoint number).
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* Status code of the function execution \ref cy_en_usbfs_dev_drv_status_t.
*
*******************************************************************************/
cy_en_usbfs_dev_drv_status_t Cy_USBFS_Dev_Drv_RemoveEndpoint(USBFS_Type *base,
                                                             uint32_t    endpointAddr,
                                                             cy_stc_usbfs_dev_drv_context_t *context)
{
    uint32_t endpoint = EPADDR2PHY(endpointAddr);

    /* Disable endpoint operation */
    DisableEndpoint(base, endpoint, context);

    /* Disable SIE and Arbiter interrupt for endpoint */
    Cy_USBFS_Dev_Drv_DisableSieEpInterrupt(base, endpoint);
    Cy_USBFS_Dev_Drv_DisableArbEpInterrupt(base, endpoint);

    /* Clear SIE and Arbiter interrupt for endpoint */
    Cy_USBFS_Dev_Drv_ClearSieEpInterrupt(base, endpoint);
    Cy_USBFS_Dev_Drv_ClearArbEpInterrupt(base, endpoint, ENDPOINT_ARB_INTR_SOURCES_ALL);

    return CY_USBFS_DEV_DRV_SUCCESS;
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_EnableOutEndpoint
****************************************************************************//**
*
* Enables the OUT data endpoint to be read by the Host.
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint
* The OUT data endpoint number.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* Status code of the function execution \ref cy_en_usbfs_dev_drv_status_t.
*
* \note
* The OUT endpoints are not enabled by default. The endpoints must be enabled
* before calling \ref Cy_USBFS_Dev_Drv_ReadOutEndpoint to read data from an endpoint.
*
*******************************************************************************/
void Cy_USBFS_Dev_Drv_EnableOutEndpoint(USBFS_Type *base,
                                        uint32_t    endpoint,
                                        cy_stc_usbfs_dev_drv_context_t *context)
{
    CY_ASSERT_L1(IS_EP_VALID(endpoint));

    endpoint = EP2PHY(endpoint);
    CY_ASSERT_L1(IS_EP_DIR_OUT(context->epPool[endpoint].address));

    /* Get pointer to endpoint data */
    cy_stc_usbfs_dev_drv_endpoint_data_t *endpointData = &context->epPool[endpoint];

    /* Clear transfer complete notification */
    endpointData->state = CY_USB_DEV_EP_PENDING;

    /* Arm endpoint: Host is allowed to write data */
    Cy_USBFS_Dev_Drv_SetSieEpMode(base, endpoint, (uint32_t) endpointData->sieMode);
}


/*******************************************************************************
* Function Name: LoadInEndpointCpu
****************************************************************************//**
*
* Implements \ref Cy_USBFS_Dev_Drv_LoadInEndpoint for 
* \ref CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU mode.
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint
* The IN data endpoint number.
*
* \param buffer
* The pointer to the buffer containing data bytes to load.
*
* \param size
* The number of bytes to load into endpoint.
* This value must be less than or equal to endpoint maximum packet size.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* Status code of the function execution \ref cy_en_usbfs_dev_drv_status_t.
*
*******************************************************************************/
cy_en_usbfs_dev_drv_status_t LoadInEndpointCpu(USBFS_Type   *base,
                                               uint32_t      endpoint,
                                               uint8_t const *buffer,
                                               uint32_t      size,
                                               cy_stc_usbfs_dev_drv_context_t *context)
{
    /* Get pointer to endpoint data */
    cy_stc_usbfs_dev_drv_endpoint_data_t *endpointData = &context->epPool[endpoint];

    /* Request to load more bytes than endpoint buffer */
    if (size > endpointData->bufferSize)
    {
        return CY_USBFS_DEV_DRV_BAD_PARAM;
    }

    /* Clear transfer completion notification */
    endpointData->state = CY_USB_DEV_EP_PENDING;

    /* Set count and data toggle */
    Cy_USBFS_Dev_Drv_SetSieEpCount(base, endpoint, size, (uint32_t) endpointData->toggle);
    
    if (0U == size)
    {
        /* Arm endpoint: endpoint ACK Host request */
        Cy_USBFS_Dev_Drv_SetSieEpMode(base, endpoint, (uint32_t) endpointData->sieMode);
    }
    else
    {
        uint32_t idx;

        if (context->useReg16)
        {
            /* Get pointer to the buffer */
            uint16_t const *ptr = (uint16_t const *) buffer;

            /* Get number of writes into the 16-bit register */
            size = GET_SIZE16(size);

            /* Copy data from the user buffer into the hardware buffer */
            for (idx = 0u; idx < size; ++idx)
            {
                Cy_USBFS_Dev_Drv_WriteData16(base, endpoint, ptr[idx]);
            }
        }
        else
        {

            /* Copy data from the user buffer into the hardware buffer */
            for (idx = 0u; idx < size; ++idx)
            {
                Cy_USBFS_Dev_Drv_WriteData(base, endpoint, buffer[idx]);
            }
        }

        /* Arm endpoint: endpoint ACK host request */
        Cy_USBFS_Dev_Drv_SetSieEpMode(base, endpoint, (uint32_t) endpointData->sieMode);
    }

    return CY_USBFS_DEV_DRV_SUCCESS;
}


/*******************************************************************************
* Function Name: ReadOutEndpointCpu
****************************************************************************//**
*
* Implements \ref Cy_USBFS_Dev_Drv_ReadOutEndpoint for 
* \ref CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU mode.
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint
* The OUT data endpoint number.
*
* \param buffer
* Pointer to the buffer that stores data that was read.
*
* \param size
* The number of bytes to read from endpoint.
* This value must be less than or equal to endpoint maximum packet size.
*
* \param actSize
* The number of bytes that were actually read.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* Status code of the function execution \ref cy_en_usbfs_dev_drv_status_t.
*
*******************************************************************************/
cy_en_usbfs_dev_drv_status_t ReadOutEndpointCpu(USBFS_Type *base,
                                                uint32_t   endpoint,
                                                uint8_t    *buffer,
                                                uint32_t   size,
                                                uint32_t   *actSize,
                                                cy_stc_usbfs_dev_drv_context_t *context)
{
    uint32_t idx;
    uint32_t numToCopy;

    /* Suppress a compiler warning about unused variables */
    (void) context;

    /* Get number of received bytes */
    numToCopy = Cy_USBFS_Dev_Drv_GetSieEpCount(base, endpoint);

    /* Initialize actual number of copied bytes */
    *actSize = 0U;
    
    /* Endpoint received more bytes than provided buffer */
    if (numToCopy > size)
    {
        return CY_USBFS_DEV_DRV_BAD_PARAM;
    }

    /* Nothing to copy (zero length packet) return success */
    if (0U == numToCopy)
    {
        return CY_USBFS_DEV_DRV_SUCCESS;
    }

    /* Update number of copied bytes */
    *actSize = numToCopy;

    if (context->useReg16)
    {
        /* Get pointer to the buffer */
        uint16_t *ptr = (uint16_t *) buffer;

        /* Get number of reads from 16-bit register */
        numToCopy = GET_SIZE16(numToCopy);

        /* Copy data from the hardware buffer into user buffer */
        for (idx = 0U; idx < numToCopy; ++idx)
        {
            ptr[idx] = Cy_USBFS_Dev_Drv_ReadData16(base, endpoint);
        }
    }
    else
    {
        /* Copy data from the hardware buffer into the user buffer */
        for (idx = 0U; idx < numToCopy; ++idx)
        {
            buffer[idx] = Cy_USBFS_Dev_Drv_ReadData(base, endpoint);
        }
    }

    return CY_USBFS_DEV_DRV_SUCCESS;
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_Abort
****************************************************************************//**
*
* Starts abort operation for data endpoint. 
* If this function returns \ref CY_USB_DEV_EP_PENDING state, the firmware must 
* wait for 1 millisecond to allow the USB Host to complete a possible on-going transfer. 
* After this time is passed, call \ref Cy_USBFS_Dev_Drv_GetEndpointState to 
* define endpoint state. If endpoint is still pending, call
* \ref Cy_USBFS_Dev_Drv_AbortComplete to complete abort operation.
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint
* The data endpoint number.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* Data endpoint state \ref cy_en_usb_dev_ep_state_t after abort was applied.
*
* \note
* This abort operation is not supported for ISOC endpoints because 
* these endpoints do not have handshake and are always accessible to the 
* USB Host. Therefore, abort can cause unexpected behavior.
*
*******************************************************************************/
cy_en_usb_dev_ep_state_t Cy_USBFS_Dev_Drv_Abort(USBFS_Type *base,
                                                 uint32_t   endpoint,
                                                 cy_stc_usbfs_dev_drv_context_t const *context)
{
    cy_en_usb_dev_ep_state_t epState = CY_USB_DEV_EP_INVALID;

    CY_ASSERT_L1(CY_USBFS_DEV_DRV_IS_EP_VALID(endpoint));

    endpoint = EP2PHY(endpoint);

    /* Get endpoint state */
    epState = context->epPool[endpoint].state;

    switch(Cy_USBFS_Dev_Drv_GetSieEpMode(base, endpoint))
    {
        case CY_USBFS_DEV_DRV_EP_CR_ISO_OUT:
        case CY_USBFS_DEV_DRV_EP_CR_ISO_IN:
            /* ISOC endpoint do not support abort operation */
            epState = CY_USB_DEV_EP_INVALID;
        break;

        case CY_USBFS_DEV_DRV_EP_CR_ACK_OUT:
            /* State must be CY_USB_DEV_EP_PENDING */
            Cy_USBFS_Dev_Drv_SetSieEpMode(base, endpoint, CY_USBFS_DEV_DRV_EP_CR_NAK_OUT);
        break;
        
        case CY_USBFS_DEV_DRV_EP_CR_ACK_IN:
            /* State must be CY_USB_DEV_EP_PENDING */
            Cy_USBFS_Dev_Drv_SetSieEpMode(base, endpoint, CY_USBFS_DEV_DRV_EP_CR_NAK_IN);
        break;

        case CY_USBFS_DEV_DRV_EP_CR_NAK_OUT:
        case CY_USBFS_DEV_DRV_EP_CR_NAK_IN:
            /* State must be CY_USB_DEV_EP_COMPLETED or CY_USB_DEV_EP_IDLE */
        break;

        default:
            epState = CY_USB_DEV_EP_INVALID;
        break;
    }

    return (epState);
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_AbortComplete
****************************************************************************//**
*
* Completes abort operation for data endpoint when endpoint is still in pending 
* state. Find the details of the abort procedure in the \ref Cy_USBFS_Dev_Drv_Abort
* function description.
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint
* The data endpoint number.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
*******************************************************************************/
cy_en_usbfs_dev_drv_status_t Cy_USBFS_Dev_Drv_AbortComplete(USBFS_Type *base,
                                                            uint32_t   endpoint,
                                                            cy_stc_usbfs_dev_drv_context_t *context)
{
    cy_en_usbfs_dev_drv_status_t retStatus = CY_USBFS_DEV_DRV_BAD_PARAM;
    bool inDirection;

    endpoint = EP2PHY(endpoint);
    inDirection = IS_EP_DIR_IN(context->epPool[endpoint].address);
    
    /* Initialize pointers to functions that work with data endpoint */
    switch(context->mode)
    {
        case CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU:
        case CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA:
        {
            /* IN endpoint: flush buffer to discard loaded data. 
            *  OUT endpoint: leave written data in the buffer.
            */
            if (inDirection)
            {
                Cy_USBFS_Dev_Drv_FlushInBuffer(base, endpoint);
            }

            retStatus = CY_USBFS_DEV_DRV_SUCCESS;
        }
        break;

        case CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO:
        {
            /* IN endpoint: flush buffer to discard loaded data. 
            *  OUT endpoint: wait for DMA complete if transfer was started.
            */
            retStatus = DynamicEndpointReConfiguration(base, inDirection, endpoint);
        }
        break;

        default:
            break;
    }

    /* Abort operation was successful: set endpoint idle state */
    context->epPool[endpoint].state = CY_USB_DEV_EP_IDLE;

    return retStatus;
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_StallEndpoint
****************************************************************************//**
*
* Configures data endpoint to STALL any request intended for it.
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint
* The data endpoint number.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* Status code of the function execution \ref cy_en_usbfs_dev_drv_status_t.
*
*******************************************************************************/
cy_en_usbfs_dev_drv_status_t Cy_USBFS_Dev_Drv_StallEndpoint(USBFS_Type *base,
                                                            uint32_t    endpoint,
                                                            cy_stc_usbfs_dev_drv_context_t *context)
{
    cy_stc_usbfs_dev_drv_endpoint_data_t *endpointData;

    /* Check if endpoint is supported by driver */
    if (false == IS_EP_VALID(endpoint))
    {
        return CY_USBFS_DEV_DRV_BAD_PARAM;
    }

    endpoint = EP2PHY(endpoint);
    endpointData = &context->epPool[endpoint];

    /* Check whether endpoint is configured */
    if (CY_USB_DEV_EP_DISABLED == endpointData->state)
    {
        return CY_USBFS_DEV_DRV_BAD_PARAM;
    }


    /* Store current endpoint state to restore it after Stall clear */
    endpointData->isPending = (CY_USB_DEV_EP_PENDING == endpointData->state);

    /* Stall endpoint */
    Cy_USBFS_Dev_Drv_SetSieEpStall(base, IS_EP_DIR_IN(endpointData->address), endpoint);
    endpointData->state = CY_USB_DEV_EP_STALLED;

    return CY_USBFS_DEV_DRV_SUCCESS;
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_UnStallEndpoint
****************************************************************************//**
*
* Release data endpoint STALL condition and clears data toggle bit.
* The endpoint is returned to the same state as it was before STALL request.
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint
* The data endpoint number.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* Status code of the function execution \ref cy_en_usbfs_dev_drv_status_t.
*
*******************************************************************************/
cy_en_usbfs_dev_drv_status_t Cy_USBFS_Dev_Drv_UnStallEndpoint(USBFS_Type *base,
                                                              uint32_t    endpoint,
                                                              cy_stc_usbfs_dev_drv_context_t *context)
{
    cy_stc_usbfs_dev_drv_endpoint_data_t *endpointData;

    /* Check whether the endpoint is supported by the driver */
    if (false == IS_EP_VALID(endpoint))
    {
        return CY_USBFS_DEV_DRV_BAD_PARAM;
    }

    endpoint = EP2PHY(endpoint);
    endpointData = &context->epPool[endpoint];

    /* Check whether the endpoint is configured */
    if (CY_USB_DEV_EP_DISABLED == endpointData->state)
    {
        return CY_USBFS_DEV_DRV_BAD_PARAM;
    }

    /* Clear toggle bit */
    endpointData->toggle = 0U;

    if (endpointData->isPending)
    {
        /* Restore pending state */
        endpointData->state = CY_USB_DEV_EP_PENDING;

        /* Clear toggle and enable endpoint to respond to host */
        Cy_USBFS_Dev_Drv_ClearSieEpToggle(base, endpoint);
        Cy_USBFS_Dev_Drv_ClearSieEpStall (base, endpoint, (uint32_t) endpointData->sieMode);
    }
    else
    {
        /* Endpoint is ready for operation */
        context->epPool[endpoint].state = CY_USB_DEV_EP_IDLE;

        /* Set endpoint inactive mode */
        Cy_USBFS_Dev_Drv_ClearSieEpToggle(base, endpoint);
        Cy_USBFS_Dev_Drv_ClearSieEpStall (base, endpoint, GetEndpointInactiveMode((uint32_t) endpointData->sieMode));
    }

    return CY_USBFS_DEV_DRV_SUCCESS;
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXUSBFS */


/* [] END OF FILE */
