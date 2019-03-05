/***************************************************************************//**
* \file cy_usbfs_dev_drv.c
* \version 1.10
*
* Provides general API implementation of the USBFS driver.
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

#include "cy_usbfs_dev_drv.h"
#include "cy_usbfs_dev_drv_pvt.h"

#ifdef CY_IP_MXUSBFS

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
*                        Internal Constants
*******************************************************************************/

/* After suspend, disable wait 2 us per MKAD-172 */
#define WAIT_SUSPEND_DISABLE    (2U)

/* The bus reset counter is driven by 100 kHz clock and detects bus reset
* condition after 100 us.
*/
#define BUS_RESET_PERIOD        (10UL)


/*******************************************************************************
*                        Internal Functions Prototypes
*******************************************************************************/

static void LpmIntrHandler(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context);
static void SofIntrHandler(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context);
static void Ep0IntrHandler(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context);
static void BusResetIntrHandler(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context);
static void ArbiterIntrHandler (USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context);
static void SieEnpointIntrHandler(USBFS_Type *base, uint32_t endpoint, cy_stc_usbfs_dev_drv_context_t *context);

static uint32_t WriteEp0Buffer(USBFS_Type *base, uint8_t const *buffer, uint32_t size);
static uint32_t ReadEp0Buffer (USBFS_Type const *base, uint8_t *buffer, uint32_t size);

static void RestoreDeviceConfiguration(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context);

/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_Init
****************************************************************************//**
*
* Initializes the USBFS in device mode. If DMAs are used, initialize the DMAs.
*
* \param base
* The pointer to the USBFS instance.
*
* \param config
* The pointer to the configuration structure \ref cy_stc_usbfs_dev_drv_config_t.
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
cy_en_usbfs_dev_drv_status_t Cy_USBFS_Dev_Drv_Init(USBFS_Type *base, 
                                                   cy_stc_usbfs_dev_drv_config_t const *config,
                                                   cy_stc_usbfs_dev_drv_context_t      *context)
{
    cy_en_usbfs_dev_drv_status_t retStatus = CY_USBFS_DEV_DRV_BAD_PARAM;

    /* Input parameters verification */
    if ((NULL == base) || (NULL == config) || (NULL == context))
    {
        return CY_USBFS_DEV_DRV_BAD_PARAM;
    }

    CY_ASSERT_L3(CY_USBFS_DEV_DRV_IS_MODE_VALID(config->mode));

    /* Enable clock to mxusb IP */
    USBFS_DEV_USB_CLK_EN(base) = CY_USBFS_DEV_DRV_WRITE_ODD(USBFS_USBDEV_USB_CLK_EN_CSR_CLK_EN_Msk);

    /* Clear register (except reserved): the IOMODE = 0 means usb IP controls the usb pins */
    USBFS_DEV_USBIO_CR1(base) = (USBFS_DEV_USBIO_CR1(base) & USBFS_USBDEV_USBIO_CR1_RESERVED_2_Msk);

    /* Set number of clocks (divided version of Clk_Peri) to detect bus reset */
    USBFS_DEV_BUS_RST_CNT(base) = BUS_RESET_PERIOD;

    /* Enable PHY detector and single-ended and differential receivers */
    USBFS_DEV_LPM_POWER_CTL(base) = (USBFS_USBLPM_POWER_CTL_SUSPEND_Msk    |
                                     USBFS_USBLPM_POWER_CTL_ENABLE_DPO_Msk |
                                     USBFS_USBLPM_POWER_CTL_ENABLE_DMO_Msk);
    (void) USBFS_DEV_LPM_POWER_CTL(base);

    /* Suspend clear sequence */
    Cy_SysLib_DelayUs(WAIT_SUSPEND_DISABLE);
    USBFS_DEV_LPM_POWER_CTL(base) &= ~USBFS_USBLPM_POWER_CTL_SUSPEND_Msk;
    (void) USBFS_DEV_LPM_POWER_CTL(base);

    /* Clear register (except reserved) and enable IMO lock */
    USBFS_DEV_CR1(base) = USBFS_USBDEV_CR1_ENABLE_LOCK_Msk |
                      (USBFS_DEV_CR1(base) & USBFS_USBDEV_CR1_RESERVED_3_Msk);

    /* Configure level selection (HI, MED, LO) for each interrupt source */
    USBFS_DEV_LPM_INTR_LVL_SEL(base) = config->intrLevelSel;

    /* Enable interrupt sources: Bus Reset and EP0 */
    USBFS_DEV_LPM_INTR_SIE_MASK(base) = (USBFS_USBLPM_INTR_SIE_BUS_RESET_INTR_Msk |
                                         USBFS_USBLPM_INTR_SIE_EP0_INTR_Msk);

    /* Clear LPM register (disable LPM response) */
    USBFS_DEV_LPM_LPM_CTL(base) = 0UL;

    if (config->enableLpm)
    {
        /* Enable device to ACK LPM requests */
        USBFS_DEV_LPM_LPM_CTL(base) = (USBFS_USBLPM_LPM_CTL_LPM_EN_Msk |
                                       USBFS_USBLPM_LPM_CTL_LPM_ACK_RESP_Msk);
    }

    /* Copy configuration in the context */
    context->mode     = config->mode;
    context->useReg16 = (config->epAccess == CY_USBFS_DEV_DRV_USE_16_BITS_DR);
    context->epSharedBuf     = config->epBuffer;
    context->epSharedBufSize = config->epBufferSize;

    /* Initialize pointers to functions that work with data endpoint */
    switch(config->mode)
    {
        case CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU:
        {
            context->addEndpoint     = &AddEndpointHwBuffer;
            context->loadInEndpoint  = &LoadInEndpointCpu;
            context->readOutEndpoint = &ReadOutEndpointCpu;

            USBFS_DEV_ARB_CFG(base) = _VAL2FLD(USBFS_USBDEV_ARB_CFG_DMA_CFG,
                                            CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU);

            retStatus = CY_USBFS_DEV_DRV_SUCCESS;
        }
        break;

        case CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA:
        {
            context->addEndpoint     = &AddEndpointHwBuffer;
            context->loadInEndpoint  = &LoadInEndpointDma;
            context->readOutEndpoint = &ReadOutEndpointDma;

            USBFS_DEV_ARB_CFG(base) = _VAL2FLD(USBFS_USBDEV_ARB_CFG_DMA_CFG,
                                            CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA);
        }
        break;

        case CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO:
        {
            context->addEndpoint     = &AddEndpointRamBuffer;
            context->loadInEndpoint  = &LoadInEndpointDmaAuto;
            context->readOutEndpoint = &ReadOutEndpointDmaAuto;

            USBFS_DEV_ARB_CFG(base) = (_VAL2FLD(USBFS_USBDEV_ARB_CFG_DMA_CFG,
                                             CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO) |
                                             USBFS_USBDEV_ARB_CFG_AUTO_MEM_Msk);
        }
        break;

        default:
            break;
    }

    /* Configure DMA and store info about DMA channels */
    if (context->mode != CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU)
    {
        retStatus = DmaInit(config, context);   
    }

    return retStatus;
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_DeInit
****************************************************************************//**
*
* De-initializes the USBFS Device hardware (returns the register values to 
* default) and removes all registered callbacks.
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
void Cy_USBFS_Dev_Drv_DeInit(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context)
{
    uint32_t regVal;
    uint32_t endpoint;

    /* Set USBLPM registers into the default state */
    USBFS_DEV_LPM_POWER_CTL(base)     = 0UL;
    USBFS_DEV_LPM_USBIO_CTL(base)     = 0UL;
    USBFS_DEV_LPM_FLOW_CTL(base)      = 0UL;
    USBFS_DEV_LPM_LPM_CTL(base)       = 0UL;
    USBFS_DEV_LPM_INTR_SIE(base)      = 0UL;
    USBFS_DEV_LPM_INTR_SIE_SET(base)  = 0UL;
    USBFS_DEV_LPM_INTR_SIE_MASK(base) = 0UL;
    USBFS_DEV_LPM_INTR_LVL_SEL(base)  = 0UL;

    /* Set USBDEV registers into the default state */
    USBFS_DEV_CR0(base) = 0UL;
    USBFS_DEV_CR1(base) = (USBFS_DEV_CR1(base) & USBFS_USBDEV_CR1_RESERVED_3_Msk);
    USBFS_DEV_USBIO_CR0(base) = 0UL;
    USBFS_DEV_USBIO_CR1(base) = (USBFS_DEV_USBIO_CR1(base) & USBFS_USBDEV_USBIO_CR1_RESERVED_2_Msk);
    regVal = CY_USBFS_DEV_READ_ODD(USBFS_DEV_USBIO_CR2(base));
    USBFS_DEV_USBIO_CR2(base) = (CY_USBFS_DEV_DRV_WRITE_ODD(regVal) & USBFS_USBDEV_USBIO_CR2_RESERVED_7_Msk); 

    USBFS_DEV_BUS_RST_CNT(base) = BUS_RESET_PERIOD;
    USBFS_DEV_USB_CLK_EN(base)  = CY_USBFS_DEV_DRV_WRITE_ODD(0UL);

    USBFS_DEV_EP0_CR(base)  = 0UL;
    USBFS_DEV_EP0_CNT(base) = CY_USBFS_DEV_DRV_WRITE_ODD(0UL);

    USBFS_DEV_ARB_CFG(base)    = 0UL;
    USBFS_DEV_ARB_INT_EN(base) = 0UL;

    USBFS_DEV_DYN_RECONFIG(base) = 0UL;
    USBFS_DEV_BUF_SIZE(base)     = 0UL;
    USBFS_DEV_DMA_THRES16(base)  = 0UL;
    USBFS_DEV_EP_ACTIVE(base)    = 0UL;
    USBFS_DEV_EP_TYPE(base)      = CY_USBFS_DEV_DRV_WRITE_ODD(0UL);
    USBFS_DEV_CWA16(base)        = 0UL;

    USBFS_DEV_SIE_EP_INT_EN(base) = 0UL;
    USBFS_DEV_SIE_EP_INT_SR(base) = 0UL;

    
    for (endpoint = 0UL; endpoint < CY_USBFS_DEV_DRV_NUM_EPS_MAX; ++endpoint)
    {
        /* Set SIE endpoint register into the default state */
        USBFS_DEV_SIE_EP_CR0(base, endpoint)  = 0UL;
        USBFS_DEV_SIE_EP_CNT0(base, endpoint) = 0UL;
        USBFS_DEV_SIE_EP_CNT1(base, endpoint) = 0UL;

        /* Set ARBITER endpoint register into the default state */
        USBFS_DEV_ARB_EP_CFG(base, endpoint)    = 0UL;
        USBFS_DEV_ARB_EP_INT_EN(base, endpoint) = 0UL;
        USBFS_DEV_ARB_RW_WA16(base, endpoint)   = 0UL;
        USBFS_DEV_ARB_RW_RA16(base, endpoint)   = 0UL;
    }

    /* Clean-up context callbacks */
    context->cbSof = NULL;
    context->cbLpm = NULL;
    
    for (endpoint = 0UL; endpoint < CY_USBFS_DEV_DRV_NUM_EPS_MAX; ++endpoint)
    {
        context->epPool[endpoint].address    = 0U;
        context->epPool[endpoint].copyData   = NULL;
        context->epPool[endpoint].epComplete = NULL;
        context->epPool[endpoint].buffer     = NULL;
    }
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_Enable
****************************************************************************//**
*
* Enable the USBFS Device operation.
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
void Cy_USBFS_Dev_Drv_Enable(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t const *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) context;

    /* Clear EP0 count register */
    USBFS_DEV_EP0_CNT(base) = CY_USBFS_DEV_DRV_WRITE_ODD(0UL);

    /* Set EP0.CR: ACK Setup, NAK IN/OUT */
    USBFS_DEV_EP0_CR(base)  = CY_USBFS_DEV_DRV_EP_CR_NAK_INOUT;

    /* Enable D+ pull-up, the device appears on the bus */
    USBFS_DEV_LPM_POWER_CTL(base) |= USBFS_USBLPM_POWER_CTL_DP_UP_EN_Msk;
    (void) USBFS_DEV_LPM_POWER_CTL(base);
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_Disable
****************************************************************************//**
*
* Disables the USBFS Device operation. If DMAs are used, disables the DMAs.
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
void Cy_USBFS_Dev_Drv_Disable(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) context;

    /* Disable D+ pull-up */
    USBFS_DEV_LPM_POWER_CTL(base) &= ~USBFS_USBLPM_POWER_CTL_DP_UP_EN_Msk;
    (void) USBFS_DEV_LPM_POWER_CTL(base);

    /* Disable device to respond to usb traffic */
    USBFS_DEV_CR0(base) &= ~USBFS_USBDEV_CR0_USB_ENABLE_Msk;

    /* Disable DMA channels */
    if (CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU != context->mode)
    {
        DmaDisable(context);
    }
}


/*******************************************************************************
* Function Name: LpmIntrHandler
****************************************************************************//**
*
* LPM (Link Power Manager) interrupt handler.
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
static void LpmIntrHandler(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context)
{
    if (context->cbLpm != NULL)
    {
        context->cbLpm(base, context);
    }
}


/*******************************************************************************
* Function Name: SofIntrHandler
****************************************************************************//**
*
* SOF (Start Of Frame) interrupt handler.
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
static void SofIntrHandler(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context)
{
    if (context->cbSof != NULL)
    {
        context->cbSof(base, context);
    }
}


/*******************************************************************************
* Function Name: Ep0IntrHandler
****************************************************************************//**
*
* Endpoint 0 (Control Endpoint) interrupt handler.
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
static void Ep0IntrHandler(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context)
{
    /* Read CR register */
    uint32_t ep0Cr = USBFS_DEV_EP0_CR(base);

    /* Check whether packet was ACKed */
    if (0U != (ep0Cr & USBFS_USBDEV_EP0_CR_ACKED_TXN_Msk))
    {
        bool updateCr0 = false;

        /* Check packet direction */
        if (_FLD2BOOL(USBFS_USBDEV_EP0_CR_SETUP_RCVD, ep0Cr))
        {
            /* Handle SETUP */
            if (_FLD2VAL(USBFS_USBDEV_EP0_CR_MODE, ep0Cr) == CY_USBFS_DEV_DRV_EP_CR_NAK_INOUT)
            {
                /* Try to unlock CR0 register: read and then write.
                * The success write clears 8-4 bits in the register.
                */
                ep0Cr = USBFS_DEV_EP0_CR(base);
                USBFS_DEV_EP0_CR(base) = ep0Cr;

                /* Check whether CR0 register unlocked (bits cleared) */
                ep0Cr = USBFS_DEV_EP0_CR(base);
                if (false == _FLD2BOOL(USBFS_USBDEV_EP0_CR_SETUP_RCVD, ep0Cr))
                {
                    /* Reset EP0 CNT register (data toggle 0) */
                    context->ep0CntReg = 0UL;

                    /* Call Device layer to service request */
                    context->ep0Setup(base, context);

                    updateCr0 = true;
                }
            }
        }
        /* Handle IN */
        else if (_FLD2BOOL(USBFS_USBDEV_EP0_CR_IN_RCVD, ep0Cr))
        {
            context->ep0In(base, context);
            updateCr0 = true;
        }
        else if (_FLD2BOOL(USBFS_USBDEV_EP0_CR_OUT_RCVD, ep0Cr))
        {
            /* Handle OUT */
            context->ep0Out(base, context);
            updateCr0 = true;
        }
        else
        {
            /* Do nothing - unknown source */
        }

        if (updateCr0)
        {
            /* Check whether CR0 register unlocked (bits are cleared) */
            ep0Cr = USBFS_DEV_EP0_CR(base);

            if (false == _FLD2BOOL(USBFS_USBDEV_EP0_CR_SETUP_RCVD, ep0Cr))
            {
                /* Update count and mode registers */
                USBFS_DEV_EP0_CNT(base) = CY_USBFS_DEV_DRV_WRITE_ODD(context->ep0CntReg);
                USBFS_DEV_EP0_CR(base)  = context->ep0ModeReg;
                USBFS_DEV_EP0_CR(base);
            }
        }
    }
}


/*******************************************************************************
* Function Name: BusResetIntrHandler
****************************************************************************//**
*
* Bus Reset interrupt handler.
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
static void BusResetIntrHandler(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context)
{
    /* Reset driver variables if any */

    /* Cypress ID# 293217: write CR0 when pull-up is enabled */
    if (_FLD2BOOL(USBFS_USBLPM_POWER_CTL_DP_UP_EN, USBFS_DEV_LPM_POWER_CTL(base)))
    {
        /* Pass event to the Device layer */
        context->busReset(base, context);

        /* Clear EP0 count register */
        USBFS_DEV_EP0_CNT(base) = CY_USBFS_DEV_DRV_WRITE_ODD(0UL);

        /* Set EP0.CR: ACK Setup, NAK IN/OUT */
        USBFS_DEV_EP0_CR(base) = CY_USBFS_DEV_DRV_EP_CR_NAK_INOUT;

        /* Enable device to responds to USB traffic with address 0 */
        USBFS_DEV_CR0(base) = USBFS_USBDEV_CR0_USB_ENABLE_Msk;
        (void) USBFS_DEV_CR0(base);
    }
}


/*******************************************************************************
* Function Name: ArbiterIntrHandler
****************************************************************************//**
*
* Arbiter interrupt handler (enabled for operation with DMAs).
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
static void ArbiterIntrHandler(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context)
{
    uint32_t endpoint = 0UL;
    uint32_t intrMask = Cy_USBFS_Dev_Drv_GetArbAllEpsInterruptStatus(base);

    /* Handle active interrupt sources */
    while (0U != intrMask)
    {
        if (0U != (intrMask & 0x01U))
        {
            /* Get endpoint enable interrupt sources */
            uint32_t sourceMask = Cy_USBFS_Dev_Drv_GetArbEpInterruptStatusMasked(base, endpoint);

            /* Get pointer to endpoint data */
            cy_stc_usbfs_dev_drv_endpoint_data_t *endpointData = &context->epPool[endpoint];

            Cy_USBFS_Dev_Drv_ClearArbEpInterrupt(base, endpoint, sourceMask);

            /* Mode 2/3: Handle IN endpoint buffer full event: trigger after
            * endpoint buffer has been loaded
            */
            if (0U != (sourceMask & USBFS_USBDEV_ARB_EP_IN_BUF_FULL_Msk))
            {
                Cy_USBFS_Dev_Drv_ClearArbCfgEpInReady(base, endpoint);

                /* Mode 2: notify the user that data has been copied into endpoint buffer.
                *  Mode 3: the endpoint pending state is set before in LoadInEndpointDmaAuto (no impact).
                */
                endpointData->state = CY_USB_DEV_EP_PENDING;

                /* Arm IN endpoint */
                Cy_USBFS_Dev_Drv_SetSieEpMode(base, endpoint, (uint32_t) endpointData->sieMode);
            }

            /* Mode 2: Handle DMA completion event for OUT endpoints */
            if (0U != (sourceMask & USBFS_USBDEV_ARB_EP_DMA_GNT_Msk))
            {
                /* Notify the user that data has been copied from endpoint buffer */
                endpointData->state = CY_USB_DEV_EP_COMPLETED;
            }

            /* Mode 3: Handle DMA completion event for OUT endpoints */
            if (0U != (sourceMask & USBFS_USBDEV_ARB_EP_DMA_TERMIN_Msk))
            {
                /* Set DMA channel to start new transfer */
                DmaOutEndpointRestore(endpointData);

                /* Update toggle (exclude ISOC endpoints) */
                if (false == IS_EP_ISOC(endpointData->sieMode))
                {
                    endpointData->toggle ^= (uint8_t) USBFS_USBDEV_SIE_EP_DATA_TOGGLE_Msk;
                }                

                /* Set complete event and update data toggle */
                endpointData->state = CY_USB_DEV_EP_COMPLETED;

                /* Involve callback if registered */
                if (endpointData->epComplete != NULL)
                {
                    uint32_t errorType = 0UL;
                    
                    /* Check transfer errors (detect by hardware) */
                    if (0U != Cy_USBFS_Dev_Drv_GetSieEpError(base, endpoint))
                    {
                        errorType |= CY_USBFS_DEV_ENDPOINT_TRANSFER_ERROR;
                    }
                    
                    /* Check data toggle bit of current transfer (exclude ISOC endpoints) */
                    if (false == IS_EP_ISOC(endpointData->sieMode))
                    {
                        if (endpointData->toggle == Cy_USBFS_Dev_Drv_GetSieEpToggle(base, endpoint))
                        {
                            errorType |= CY_USBFS_DEV_ENDPOINT_SAME_DATA_TOGGLE;
                        }
                    }
                    
                    /* Call endpoint complete callback */
                    endpointData->epComplete(base, errorType, context);
                }
            }

            /* This error condition indicates system failure */
            if (0U != (sourceMask & USBFS_USBDEV_ARB_EP_BUF_OVER_Msk))
            {
                /* The DMA is not capable of moving data from the mxusbfs IP 
                * hardware buffer fast enough and so caused overflow. Give DMA 
                * channel for this endpoint greater priority or increase the clock 
                * at which it operates.
                */
                CY_ASSERT_L1(false);
            }

            /* This error condition indicates system failure */
            if (0U != (sourceMask & USBFS_USBDEV_ARB_EP_BUF_UNDER_Msk))
            {
                /* The DMA is not capable of moving data into the mxusbfs IP 
                * hardware buffer fast enough and so caused underflow. Give DMA 
                * channel for this endpoint greater priority or increase the clock 
                * at which it operates.
                */                
                CY_ASSERT_L1(false);
            }
        }

        /* Move to next endpoint */
        intrMask >>= 1UL;
        ++endpoint;
    }
}


/*******************************************************************************
* Function Name: SieEnpointIntrHandler
****************************************************************************//**
*
* SIE (Serial Interface Engine) endpoint interrupt handler. 
* It triggers when communication was completed with data endpoint.
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
static void SieEnpointIntrHandler(USBFS_Type *base, uint32_t endpoint, 
                                  cy_stc_usbfs_dev_drv_context_t *context)
{
    bool inEnpoint;
    bool zeroLengthPacket;

    /* Get pointer to endpoint data */
    cy_stc_usbfs_dev_drv_endpoint_data_t *endpointData = &context->epPool[endpoint];

    Cy_USBFS_Dev_Drv_ClearSieEpInterrupt(base, endpoint);

    /* Special case: DMA Auto
    * IN endpoints: complete transfer when SIE triggers complete.
    * OUT endpoints complete transfer when DMA complete (Arbiter Interrupt, source DMA_TERMIN).
    *               In case of zero length packet complete transfer when SIE triggers complete.
    */
    zeroLengthPacket = (0U == Cy_USBFS_Dev_Drv_GetSieEpCount(base, endpoint));
    inEnpoint = (0U == (Cy_USBFS_Dev_Drv_GetArbEpInterruptMask(base, endpoint) & USBFS_USBDEV_ARB_EP_DMA_TERMIN_Msk));

    if (inEnpoint || zeroLengthPacket)
    {
        /* Update toggle (exclude ISOC endpoints) */
        if (false == IS_EP_ISOC(endpointData->sieMode))
        {
            endpointData->toggle ^= (uint8_t) USBFS_USBDEV_SIE_EP_DATA_TOGGLE_Msk;
        }

        /* Set complete event and update data toggle */
        endpointData->state = CY_USB_DEV_EP_COMPLETED;

        /* Involve callback if it is registered */
        if (endpointData->epComplete != NULL)
        {
            uint32_t errorType = 0UL;
            
            /* Check transfer errors (detect by hardware) */
            if (0U != Cy_USBFS_Dev_Drv_GetSieEpError(base, endpoint))
            {
                errorType |= CY_USBFS_DEV_ENDPOINT_TRANSFER_ERROR;
            }
            
            /* Check data toggle bit of current transfer (exclude ISOC endpoints) */
            if (false == IS_EP_ISOC(endpointData->sieMode))
            {
                if (endpointData->toggle == Cy_USBFS_Dev_Drv_GetSieEpToggle(base, endpoint))
                {
                    errorType |= CY_USBFS_DEV_ENDPOINT_SAME_DATA_TOGGLE;
                }
            }

            /* Call endpoint complete callback */
            endpointData->epComplete(base, errorType, context);
        }
    }
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_Interrupt
****************************************************************************//**
*
* Process interrupt events generated by the USBFS Device.
* The interrupts are mandatory for USBFS Device operation and this function 
* must be called inside the user-defined interrupt service routine.
*
* \param base
* The pointer to the USBFS instance.
*
* \param intrCause
* The interrupt cause register value. Call appropriate function to get  
* interrupt cause (Low, Medium or High): 
* * \ref Cy_USBFS_Dev_Drv_GetInterruptCauseLo
* * \ref Cy_USBFS_Dev_Drv_GetInterruptCauseMed
* * \ref Cy_USBFS_Dev_Drv_GetInterruptCauseHi
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
*******************************************************************************/
void Cy_USBFS_Dev_Drv_Interrupt(USBFS_Type *base, uint32_t intrCause, 
                                cy_stc_usbfs_dev_drv_context_t *context)
{
    uint32_t endpoint = 0u;

    /* Clear SIE interrupts while are served below */
    Cy_USBFS_Dev_Drv_ClearSieInterrupt(base, intrCause);

    /* LPM */
    if (0u != (intrCause & USBFS_USBLPM_INTR_CAUSE_LPM_INTR_Msk))
    {
        LpmIntrHandler(base, context);
    }

    /* Arbiter: Data endpoints */
    if (0u != (intrCause & USBFS_USBLPM_INTR_CAUSE_ARB_EP_INTR_Msk))
    {
        /* This interrupt is cleared inside the handler */
        ArbiterIntrHandler(base, context);
    }

    /* Control EP0 */
    if (0u != (intrCause & USBFS_USBLPM_INTR_CAUSE_EP0_INTR_Msk))
    {
        Ep0IntrHandler(base, context);
    }

    /* SOF */
    if (0u != (intrCause & USBFS_USBLPM_INTR_CAUSE_SOF_INTR_Msk))
    {
        SofIntrHandler(base, context);
    }

    /* Bus Reset */
    if (0u != (intrCause & USBFS_USBLPM_INTR_CAUSE_BUS_RESET_INTR_Msk))
    {
        BusResetIntrHandler(base, context);
    }

    /* Remove handled interrupt statuses */
    intrCause >>= USBFS_USBLPM_INTR_CAUSE_EP1_INTR_Pos;

    /* SIE: Data endpoints */
    while (0u != intrCause)
    {
        if (0u != (intrCause & 0x1u))
        {
            /* These interrupts are cleared inside the handler */
            SieEnpointIntrHandler(base, endpoint, context);
        }

        intrCause >>= 1u;
        ++endpoint;
    }
}


/*******************************************************************************
* Function Name: WriteEp0Buffer
****************************************************************************//**
*
* Writes data into the Endpoint 0 hardware buffer and returns how many bytes 
* were written.
*
* \param base
* The pointer to the USBFS instance.
*
* \param buffer
* The pointer to the data to write in the Endpoint 0.
*
* \param size
* The number of bytes to write into the Endpoint 0.
*
* \return
* The number of bytes that were written.
*
*******************************************************************************/
static uint32_t WriteEp0Buffer(USBFS_Type *base, uint8_t const *buffer, uint32_t size)
{
    uint32_t idx;
    
    /* Cut message size if too many bytes are requested to write  */
    if (size > CY_USBFS_DEV_DRV_EP0_BUFFER_SIZE)
    {
        size = CY_USBFS_DEV_DRV_EP0_BUFFER_SIZE;
    }
    
    /* Write data into the hardware buffer */
    for (idx = 0UL; idx < size; ++idx)
    {
        if (0U == (idx & 0x1U))
        {
            USBFS_DEV_EP0_DR(base, idx) = buffer[idx];
        }
        else
        {
            /* Apply special write for odd offset registers */
            USBFS_DEV_EP0_DR(base, idx) = CY_USBFS_DEV_DRV_WRITE_ODD(buffer[idx]);
        }
    }
    
    return idx;
}


/*******************************************************************************
* Function Name: ReadEp0Buffer
****************************************************************************//**
*
* Reads data from Endpoint 0 hardware and returns how many bytes were read.
*
* \param base
* The pointer to the USBFS instance.
*
* \param buffer
* The pointer to the buffer for data read from the Endpoint 0.
*
* \param size
* The number of bytes to read from the Endpoint 0.
*
* \return
* The number of bytes that were read.
*
*******************************************************************************/
static uint32_t ReadEp0Buffer(USBFS_Type const *base, uint8_t *buffer, uint32_t size)
{
    uint32_t idx;
    
    /* Get number of received bytes */
    uint32_t numToCopy = Cy_USBFS_Dev_Drv_GetEp0Count(base);

    /* Read only received bytes */
    if (size > numToCopy)
    {
        size = numToCopy;
    }
    
    /* Get data from the buffer */
    for (idx = 0UL; idx < size; ++idx)
    {
        if (0U == (idx & 0x1U))
        {
            buffer[idx] = (uint8_t) USBFS_DEV_EP0_DR(base, idx);
        }
        else
        {
            /* Apply special write for odd offset registers */
            buffer[idx] = (uint8_t) CY_USBFS_DEV_READ_ODD(USBFS_DEV_EP0_DR(base, idx));
        }
    }
    
    return idx;
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_Ep0GetSetup
****************************************************************************//**
*
* Reads setup packed from Endpoint 0 hardware buffer.
*
* \param base
* The pointer to the USBFS instance.
*
* \param buffer
* The pointer to the buffer for data read from the Endpoint 0.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
*******************************************************************************/
void Cy_USBFS_Dev_Drv_Ep0GetSetup(USBFS_Type const *base, uint8_t *buffer, 
                                  cy_stc_usbfs_dev_drv_context_t const *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) context;

    (void) ReadEp0Buffer(base, buffer, CY_USBFS_DEV_DRV_EP0_BUFFER_SIZE);
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_Ep0Write
****************************************************************************//**
*
* Writes data into Endpoint 0 hardware buffer and returns how many bytes were 
* written.
*
* \param base
* The pointer to the USBFS instance.
*
* \param buffer
* The pointer to the buffer containing data bytes to write.
*
* \param size
* The number of bytes to write.
* This value must be less than or equal to endpoint 0 maximum packet size.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* The number of bytes that were written.
*
*******************************************************************************/
uint32_t Cy_USBFS_Dev_Drv_Ep0Write(USBFS_Type *base, uint8_t const *buffer, uint32_t size, 
                                   cy_stc_usbfs_dev_drv_context_t *context)
{
    CY_ASSERT_L1((size > 0U) ? (NULL != buffer) : true);
    
    uint32_t numBytes = 0UL;

    if (buffer != NULL)
    {
        /* DATA stage (IN direction): load data to be sent (include zero length packet) */
        
        /* Put data into the buffer */
        numBytes = WriteEp0Buffer(base, buffer, size);

        /* Update data toggle and counter */
        context->ep0CntReg ^= USBFS_USBDEV_EP0_CNT_DATA_TOGGLE_Msk;
        context->ep0CntReg  = _CLR_SET_FLD32U(context->ep0CntReg, USBFS_USBDEV_EP0_CNT_BYTE_COUNT, numBytes);
        
        /* Update EP0 mode register to continue transfer */
        context->ep0ModeReg = CY_USBFS_DEV_DRV_EP_CR_ACK_IN_STATUS_OUT;
    }
    else
    {
        /* STATUS stage (IN direction): prepare return zero-length and get ACK response */
        context->ep0CntReg  = USBFS_USBDEV_EP0_CNT_DATA_TOGGLE_Msk;
        context->ep0ModeReg = CY_USBFS_DEV_DRV_EP_CR_STATUS_IN_ONLY;
    }

    return numBytes;
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_Ep0Read
****************************************************************************//**
*
* Reads data from Endpoint 0 hardware and returns how many bytes were read.
*
* \param base
* The pointer to the USBFS instance.
*
* \param buffer
* The pointer to buffer that stores data that was read.
*
* \param size
* The number of bytes to read.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* The number of bytes which were read.
*
*******************************************************************************/
uint32_t Cy_USBFS_Dev_Drv_Ep0Read(USBFS_Type const *base, uint8_t *buffer, uint32_t size, 
                                  cy_stc_usbfs_dev_drv_context_t *context)
{    
    uint32_t numBytes = 0UL;

    if (buffer != NULL)
    {
        /* DATA stage (OUT direction): get receive data and continue */

        /* Get received data */
        numBytes = ReadEp0Buffer(base, buffer, size);

        /* Update EP0 registers to continue transfer */
        context->ep0CntReg ^= USBFS_USBDEV_EP0_CNT_DATA_TOGGLE_Msk;
        context->ep0ModeReg = CY_USBFS_DEV_DRV_EP_CR_ACK_OUT_STATUS_IN;
    }
    else
    {
        /* STATUS stage (OUT direction): prepare to send ACK handshake */
        context->ep0CntReg  = USBFS_USBDEV_EP0_CNT_DATA_TOGGLE_Msk;
        context->ep0ModeReg = CY_USBFS_DEV_DRV_EP_CR_STATUS_OUT_ONLY;
    }

    return numBytes;
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_RegisterServiceCallback
****************************************************************************//**
*
* Registers a callback function to notify about service events (Bus Reset or 
* Endpoint 0 communication) in \ref Cy_USBFS_Dev_Drv_Interrupt.
* To remove callback function, pass NULL as function pointer.
*
* \param base
* The pointer to the USBFS instance.
*
* \param source
* The event that involves the callback.
*
* \param callback
* The pointer to a callback function.
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
void Cy_USBFS_Dev_Drv_RegisterServiceCallback(USBFS_Type const *base,
                                              cy_en_usb_dev_service_cb_t     source,
                                              cy_cb_usbfs_dev_drv_callback_t callback,
                                              cy_stc_usbfs_dev_drv_context_t *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) base;

    switch(source)
    {
        case CY_USB_DEV_BUS_RESET:
            context->busReset = callback;
        break;
            
        case CY_USB_DEV_EP0_SETUP:
            context->ep0Setup = callback;
        break;
        
        case CY_USB_DEV_EP0_IN:
            context->ep0In = callback;
        break;
            
        case CY_USB_DEV_EP0_OUT:
            context->ep0Out = callback;
        break;
            
        default:
        break;
    }
}

/*******************************************************************************
* Function Name: RestoreDeviceConfiguration
****************************************************************************//**
*
* Restores device configuration and data endpoints for the active mode 
* operation.
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
static void RestoreDeviceConfiguration(USBFS_Type *base,
                                       cy_stc_usbfs_dev_drv_context_t *context)
{
    uint32_t endpoint = 0U;

    /* Set default device configuration */
    Cy_USBFS_Dev_Drv_ConfigDevice(base, context);

    for (endpoint = 0U; endpoint < CY_USBFS_DEV_DRV_NUM_EPS_MAX; ++endpoint)
    {
        if (0U != context->epPool[endpoint].address)
        {
            /* Restore endpoint configuration */
            if (CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO == context->mode)
            {
                RestoreEndpointRamBuffer(base, &context->epPool[endpoint]);
            }
            else
            {
                RestoreEndpointHwBuffer(base, context->mode, &context->epPool[endpoint]);
            }
        }
    }

    /* Complete configuration */
    Cy_USBFS_Dev_Drv_ConfigDeviceComplete(base, context);
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_Suspend
****************************************************************************//**
*
* Prepares the USBFS component to enter Deep Sleep mode.
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
* \note
* After entering low-power mode, the data that is left in the IN or OUT 
* endpoint buffers is not restored after wakeup, and is lost. Therefore, it should
* be stored in the SRAM for OUT endpoint or read by the host for IN endpoint
* before entering low-power mode.
*
*******************************************************************************/
void Cy_USBFS_Dev_Drv_Suspend(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context)
{
    /* Put PHY to suspend mode */
    USBFS_DEV_LPM_POWER_CTL(base) |= USBFS_USBLPM_POWER_CTL_SUSPEND_Msk;
    (void) USBFS_DEV_LPM_POWER_CTL(base);

    /* Disable all DMA channels: DMA registers are retention */
    if (CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU != context->mode)
    {
        DmaDisable(context);
    }
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_Resume
****************************************************************************//**
*
* Prepares the USBFS component for operation after exiting Deep Sleep mode.
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
void Cy_USBFS_Dev_Drv_Resume(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context)
{
    /* Enable clock to mxusbfs IP */
    USBFS_DEV_USB_CLK_EN(base) = CY_USBFS_DEV_DRV_WRITE_ODD(USBFS_USBDEV_USB_CLK_EN_CSR_CLK_EN_Msk);

    /* Set number of clocks (divided version of Clk_Peri) to detect bus reset */
    USBFS_DEV_BUS_RST_CNT(base) = BUS_RESET_PERIOD;

    /* Set EP0.CR: ACK Setup, NAK IN/OUT */
    USBFS_DEV_EP0_CR(base) = CY_USBFS_DEV_DRV_EP_CR_NAK_INOUT;

    /* Restore data endpoints configuration  */
    RestoreDeviceConfiguration(base, context);

    /* Release PHY from suspend mode */
    USBFS_DEV_LPM_POWER_CTL(base) &= ~USBFS_USBLPM_POWER_CTL_SUSPEND_Msk;
    (void) USBFS_DEV_LPM_POWER_CTL(base);
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXUSBFS */


/* [] END OF FILE */
