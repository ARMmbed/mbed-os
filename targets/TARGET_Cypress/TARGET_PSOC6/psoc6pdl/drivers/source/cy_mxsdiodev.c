/***************************************************************************//**
* \file cy_mxsdiodev.c
* \version 1.0
*
* Provides an API implementation of the MXSDIODEV driver
*
********************************************************************************
* \copyright
* Copyright 2020 Cypress Semiconductor Corporation
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

#if defined (CY_IP_MXSDIODEV)

#include "cy_mxsdiodev.h"
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

/** \cond internal */
#define CY_SDIO_DEV_IS_SPEED_MODE_VALID(speedMode)       ((CY_SDIO_DEV_SDR12 == (speedMode)) || \
                                                         (CY_SDIO_DEV_SDR25 == (speedMode)) || \
                                                         (CY_SDIO_DEV_SDR50 == (speedMode)) || \
                                                         (CY_SDIO_DEV_SDR104 == (speedMode)) || \
                                                         (CY_SDIO_DEV_DDR50 == (speedMode)))

#define CY_SDIO_DEV_IS_BLOCK_SIZE_VALID(blockSize)       ((CY_SDIO_DEV_BLK_SIZE_64 == (blockSize)) || \
                                                         (CY_SDIO_DEV_BLK_SIZE_128 == (blockSize)) || \
                                                         (CY_SDIO_DEV_BLK_SIZE_256 == (blockSize)) || \
                                                         (CY_SDIO_DEV_BLK_SIZE_512 == (blockSize)))

#define CY_SDIO_DEV_IS_SDIO_VERSION_VALID(version)       ((CY_SDIO_DEV_VERSION_3_0 == (version)) || \
                                                         (CY_SDIO_DEV_VERSION_2_0 == (version)))

#define CY_SDIO_DEV_IS_MANF_ID_CONFIG_VALID(manf_id)     ((CY_SDIO_DEV_USE_DEFAULT_MANF_ID == (manf_id)) || \
                                                         (CY_SDIO_DEV_USER_PROVIDED_MANF_ID == (manf_id)))
                                                         
#define MIN(a,b) (((a)<(b))?(a):(b))

/** \endcond */
cy_en_sdio_dev_status_t Cy_SDIO_DEV_Init(MXSDIO_Type *base, 
                    cy_stc_sdio_dev_config_t const *config,
                    cy_stc_sdio_dev_context_t *context)
{
    cy_en_sdio_dev_status_t ret = CY_SDIO_DEV_SUCCESS;

    /* Check for the NULL pointer. */
    if ((NULL != base) && (NULL != config) && (NULL != context))
    {
        /* Check input values */
        CY_ASSERT_L2(CY_SDIO_DEV_IS_SPEED_MODE_VALID(config->max_speed_config));
        CY_ASSERT_L2(CY_SDIO_DEV_IS_BLOCK_SIZE_VALID(config->blk_size));
        CY_ASSERT_L2(CY_SDIO_DEV_IS_SDIO_VERSION_VALID(config->version));
        CY_ASSERT_L2(CY_SDIO_DEV_IS_MANF_ID_CONFIG_VALID(config->use_default_manfid));
        
        context->current_speed_config = config->max_speed_config;
        context->current_blk_size = config->blk_size;
                
        /* The driver assumes that the device has been reset and waiting for 
         * OTP Shadow registers to be configurated
         */
         MXSDIO_OTPSHADOWREG1(base) = _VAL2FLD(MXSDIO_OTPSHADOWREG1_SDIOVERSION, config->version) |
                                      _VAL2FLD(MXSDIO_OTPSHADOWREG1_PID, config->use_default_manfid) |
                                      _VAL2FLD(MXSDIO_OTPSHADOWREG1_F2BLOCKSIZE, config->blk_size) |
                                      _VAL2FLD(MXSDIO_OTPSHADOWREG1_MAXSPEEDCODING, config->max_speed_config);

         

         if (config->use_default_manfid == CY_SDIO_DEV_USER_PROVIDED_MANF_ID)
         {
            MXSDIO_OTPSHADOWREG2(base) = _VAL2FLD(MXSDIO_OTPSHADOWREG2_MANFID0, (uint16_t) config->manfid[0]);
            MXSDIO_OTPSHADOWREG3(base) = _VAL2FLD(MXSDIO_OTPSHADOWREG3_MANFID1, (uint16_t) config->manfid[1]) |
                                         _VAL2FLD(MXSDIO_OTPSHADOWREG3_MANFID2, (uint16_t) config->manfid[2]);
         }
    }
    else
    {
        ret = CY_SDIO_DEV_ERROR_INVALID_PARAMETER;
    }
    return ret;
}

void Cy_SDIO_DEV_DeInit(MXSDIO_Type *base)
{
    /* Reset OTP Shadow Registers */
    MXSDIO_OTPSHADOWREG1(base) = 0;
    MXSDIO_OTPSHADOWREG2(base) = 0;
    MXSDIO_OTPSHADOWREG3(base) = 0;
}

cy_en_sdio_dev_status_t Cy_SDIO_DEV_Enable(MXSDIO_Type *base)
{
    cy_en_sdio_dev_status_t ret = CY_SDIO_DEV_SUCCESS;

    /* Check for the NULL pointer. */
    if (NULL != base)
    {
        /* Take SDIO Device out of reset */
        MXSDIO_CLOCKCTRLSTATUS(base) |= _VAL2FLD(MXSDIO_CLOCKCTRLSTATUS_FORCEHTPREQUEST, 1u);
        
        /* Configure Receive interrupt for each frame received */
        /* TBD : What if user doesn't want to subscribe for interrupts */
        MXSDIO_INTRCVLAZY(base) = _VAL2FLD(MXSDIO_INTRCVLAZY_FRAMECOUNT, 1u);
        
        /* Mark F2 is ready */
        MXSDIO_CORECONTROL(base) |= _VAL2FLD(MXSDIO_CORECONTROL_F2READY, 1u);
    }
    else
    {
        ret = CY_SDIO_DEV_ERROR_INVALID_PARAMETER;
    }
    return ret;
}

cy_en_sdio_dev_status_t Cy_SDIO_DEV_Disable(MXSDIO_Type *base)
{
    /* Disable Frame based receive interrupt */
    MXSDIO_INTRCVLAZY(base) = 0;
    
    /* Disable host interrupts */
    MXSDIO_INTHOSTMASK(base) = 0;
    
    /* Stop the Clock */
    MXSDIO_CLOCKCTRLSTATUS(base) = 0;

    /* Diable Core - Mark F2 not ready */
    MXSDIO_CORECONTROL(base) = 0;

    return CY_SDIO_DEV_SUCCESS;
}

cy_en_sdio_dev_status_t Cy_SDIO_DEV_GetStatus(MXSDIO_Type const *base, bool *sdStatus)
{
    /* Check for the NULL pointer. */
    if ((NULL != base) && (NULL != sdStatus))
    {
        *sdStatus = (bool)(MXSDIO_CORESTATUS(base) & MXSDIO_CORESTATUS_SDIOIOE2_Msk);
    }
    else
    {
        return CY_SDIO_DEV_ERROR_INVALID_PARAMETER;
    }
    return CY_SDIO_DEV_SUCCESS;
}

cy_en_sdio_dev_status_t Cy_SDIO_DEV_ConfigReadBuf(MXSDIO_Type const *base, uint8_t *buffer, uint32_t size, cy_stc_sdio_dev_context_t *context)
{
    /* Check for the NULL pointer. */
    if ((NULL != base) && (NULL != context))
    {
        context->rx_buffer = buffer;
        context->max_rx_buf_len = size;
    }
    else
    {
        return CY_SDIO_DEV_ERROR_INVALID_PARAMETER;
    }    
    return CY_SDIO_DEV_SUCCESS;
}

/* Returns length of the packet if the packet is valid */
static uint32_t Is_packet_valid(uint8_t *buffer)
{
    cy_stc_sdio_dev_rx_header_t *rxh = NULL;
    
    /* First 4 bytes indicate hardware tag */
    rxh = (cy_stc_sdio_dev_rx_header_t *) buffer;
    
    /* Check if any of the discard flag set */
    if (rxh->flags & RXF_DISCARD)
    {
        return 0;
    }
    /* Packet too short */
    if(rxh->len < CY_SDIO_DEV_FRAMETAG_LEN)
    {
        return 0;
    }
    return rxh->len;
}
static bool dma_tx_reset(MXSDIO_Type *base)
{
    uint32_t    retry = CY_SDIO_DEV_RETRY_TIME;
    uint32_t    status = 0;
    
    /* If DMA is already in reset, do not reset. */
    if (_FLD2VAL(MXSDIO_XMTSTATUS0_XMTSTATE, MXSDIO_XMTSTATUS0(base)) ==
        CY_SDIO_DEV_XS_DISABLED)
        return true;
        
    /* suspend tx DMA first */
    MXSDIO_XMTCONTROL(base) |= MXSDIO_XMTCONTROL_SUSPEN_Msk;
    
    status = _FLD2VAL(MXSDIO_XMTSTATUS0_XMTSTATE, MXSDIO_XMTSTATUS0(base));
            
    while ( (retry > 0UL) &&
            (status != CY_SDIO_DEV_XS_DISABLED) && 
            (status != CY_SDIO_DEV_XS_IDLE) && 
            (status != CY_SDIO_DEV_XS_STOPPED) )
    {
        status = _FLD2VAL(MXSDIO_XMTSTATUS0_XMTSTATE, MXSDIO_XMTSTATUS0(base));
        Cy_SysLib_Delay(1);
        retry--;
    }

    MXSDIO_XMTCONTROL(base) = 0;
    
    retry = CY_SDIO_DEV_RETRY_TIME;    
    status = _FLD2VAL(MXSDIO_XMTSTATUS0_XMTSTATE, MXSDIO_XMTSTATUS0(base));
    
    while ( (retry > 0UL) &&
            (status != CY_SDIO_DEV_XS_DISABLED))
    {
        status = _FLD2VAL(MXSDIO_XMTSTATUS0_XMTSTATE, MXSDIO_XMTSTATUS0(base));
        Cy_SysLib_Delay(1);
        retry--;
    }

    /* We should be disabled at this point */
    return (status == CY_SDIO_DEV_XS_DISABLED);
}

static bool dma_rx_reset(MXSDIO_Type *base)
{
    uint32_t    retry = CY_SDIO_DEV_RETRY_TIME;
    uint32_t    status = 0;

    MXSDIO_RCVCONTROL(base) = 0;
    
    retry = CY_SDIO_DEV_RETRY_TIME;    
    status = _FLD2VAL(MXSDIO_RCVSTATUS0_RCVSTATE, MXSDIO_RCVSTATUS0(base));
    while ( (retry > 0UL) &&
            (status != CY_SDIO_DEV_RS_DISABLED))
    {
        status = _FLD2VAL(MXSDIO_RCVSTATUS0_RCVSTATE, MXSDIO_RCVSTATUS0(base));
        Cy_SysLib_Delay(1);
        retry--;
    }

    return (status == CY_SDIO_DEV_RS_DISABLED);
}

void Cy_SDIO_DEV_Interrupt(MXSDIO_Type *base, cy_stc_sdio_dev_context_t *context)
{
    uint32 intstatus;
    
    /* Read interrupt status register */
    intstatus = MXSDIO_INTSTATUS(base);
    
    /* Transmit Complete Event */
    if (intstatus & MXSDIO_INTSTATUS_XMTINT_Msk)
    {
        context->tx_buffer = NULL;
        context->tx_buf_len = 0;
        
        if (context->cbEvents)
            context->cbEvents(CY_SDIO_DEV_TRANSMIT_CMPLT_EVENT);
    }

    /* Receive Complete Event */
    if (intstatus & MXSDIO_INTSTATUS_RCVINT_Msk)
    {
        if (context->cbEvents)
        {
            if (Is_packet_valid(context->rx_buffer))
            {
                context->cbEvents(CY_SDIO_DEV_RECEIVE_CMPLT_EVENT);
            }
            else
            {
                context->cbEvents(CY_SDIO_DEV_RECEIVE_PKT_ERROR);
            }
        }
        context->rx_buffer = NULL;
        context->max_rx_buf_len = 0;
    }
    /* Receive Packet - Underflow */
    if (intstatus & MXSDIO_INTSTATUS_RCVDESCUF_Msk)
    {
        /* Try receiving packet if user has configured receive buufer */
        (void) Cy_SDIO_DEV_Receive_Buffer(base, context);
    }
    
    /* Descriptor protocol error */
    if (intstatus & MXSDIO_INTSTATUS_DESCPROTERR_Msk)
    {
        /* Reset DMA engine and retransmit */
        /* Check if TX operation stopped or RX operation stopped */
        if (CY_SDIO_DEV_XS_STOPPED == _FLD2VAL(MXSDIO_XMTSTATUS0_XMTSTATE, MXSDIO_XMTSTATUS0(base)))
        {
            if (dma_tx_reset(base))
            {
                (void) Cy_SDIO_DEV_Transmit_Buffer(base, context->tx_buffer, context->tx_buf_len, context);
            }
            else
            {
                /* Do nothing: Dropping the frame */
            }
        }
        if (CY_SDIO_DEV_RS_STOPPED == _FLD2VAL(MXSDIO_RCVSTATUS0_RCVSTATE, MXSDIO_RCVSTATUS0(base)))
        {
            if (dma_rx_reset(base))
            {
                (void) Cy_SDIO_DEV_Receive_Buffer(base, context);
            }
            {
                /* Do nothing: Dropping the frame */
            }
        }
    }
    
    /* Clear the interrupt status after processing */
    MXSDIO_INTSTATUS(base) = intstatus;
    
    return;
}

void Cy_SDIO_DEV_RegisterEventCallback(MXSDIO_Type const *base, cy_cb_sdio_dev_handle_events_t callback, cy_stc_sdio_dev_context_t *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) base;

    if (NULL != context)
    {
        context->cbEvents = callback;
    }

    return;
}

cy_en_sdio_dev_status_t  Cy_SDIO_DEV_Transmit_Buffer(MXSDIO_Type const *base, uint8_t *buffer, uint16_t buf_len, cy_stc_sdio_dev_context_t *context)
{
    if ((NULL != context) && (buffer != NULL) && (buf_len > 0))
    {
        cy_stc_sdio_dev_dma_descr_t tx_descriptor;
        uint16_t inv_buf_len = ~(buf_len);
        
        memcpy(buffer, &buf_len, 2);
        memcpy(&buffer[2], &inv_buf_len, 2);
        
        context->tx_buffer = buffer;
        context->tx_buf_len = buf_len;
        
        /* Setup descriptors */
        tx_descriptor.ctrl1 = D64_CTRL1_EOF | D64_CTRL1_SOF;
        
        /* Setup DMA channel transmitter */
        MXSDIO_XMTADDRESSHI(base)    = 0;
        MXSDIO_XMTADDRESSLOW(base)    = ((uint32_t)&tx_descriptor);  // Transmit descriptor table address
        MXSDIO_XMTPTR(base)            = ((uint32_t)&tx_descriptor + sizeof(cy_stc_sdio_dev_dma_descr_t)) & 0xffff; // needs to be lower 16 bits of descriptor address
        
        MXSDIO_XMTCONTROL(base)        = ( _BOOL2FLD(MXSDIO_XMTCONTROL_PTYCHKDISABLE, 1u) |
                                        _VAL2FLD(MXSDIO_XMTCONTROL_BURSTLEN, 2u) |
                                        _VAL2FLD(MXSDIO_XMTCONTROL_PREFETCHCTL,  (1U)) );
        
        tx_descriptor.addrlow = (uint32_t) buffer;
        tx_descriptor.ctrl2   = D64_CTRL2_BC_USABLE_MASK & buf_len;
        
         /* Fire off the DMA */
        MXSDIO_XMTCONTROL(base) |= MXSDIO_XMTCONTROL_XMTEN_Msk;
    }
    else
    {
        return CY_SDIO_DEV_ERROR_INVALID_PARAMETER;
    }
    return CY_SDIO_DEV_SUCCESS;
}

__STATIC_INLINE cy_en_sdio_dev_status_t Cy_SDIO_DEV_Poll_Transmit_Complete(MXSDIO_Type const *base)
{
    cy_en_sdio_dev_status_t ret   = CY_SDIO_DEV_ERROR_TIMEOUT;
    uint32_t               retry = CY_SDIO_DEV_RETRY_TIME;
    bool                   transmitComplete;

    while (retry > 0UL)
    {
        transmitComplete = _FLD2BOOL(MXSDIO_INTSTATUS_XMTINT,
                                MXSDIO_INTSTATUS(base));

        if (false == transmitComplete)
        {
            ret = CY_SDIO_DEV_SUCCESS;
            break;
        }

        Cy_SysLib_Delay(1);
        retry--;
    }

    return ret;
}

__STATIC_INLINE cy_en_sdio_dev_status_t Cy_SDIO_DEV_Poll_Receive_Complete(MXSDIO_Type const *base)
{
    cy_en_sdio_dev_status_t ret   = CY_SDIO_DEV_ERROR_TIMEOUT;
    uint32_t               retry = CY_SDIO_DEV_RETRY_TIME;
    bool                   receiveComplete;

    while (retry > 0UL)
    {
        receiveComplete = _FLD2BOOL(MXSDIO_INTSTATUS_RCVINT,
                                MXSDIO_INTSTATUS(base));

        if (false == receiveComplete)
        {
            ret = CY_SDIO_DEV_SUCCESS;
            break;
        }

        Cy_SysLib_Delay(1);
        retry--;
    }

    return ret;
}

cy_en_sdio_dev_status_t  Cy_SDIO_DEV_Transmit_Buffer_Blocking(MXSDIO_Type const *base, uint8_t *buffer, uint16_t buf_len)
{
    cy_en_sdio_dev_status_t ret = CY_SDIO_DEV_SUCCESS;
    cy_stc_sdio_dev_dma_descr_t tx_descriptor;
    uint32_t interrupt_mask;
    
    /* Setup descriptors */
    tx_descriptor.ctrl1 = D64_CTRL1_EOF | D64_CTRL1_SOF;
    
    /* Setup DMA channel transmitter */
    MXSDIO_XMTADDRESSHI(base)    = 0;
    MXSDIO_XMTADDRESSLOW(base)    = ((uint32_t)&tx_descriptor);  // Transmit descriptor table address
    MXSDIO_XMTPTR(base)            = ((uint32_t)&tx_descriptor + sizeof(cy_stc_sdio_dev_dma_descr_t)) & 0xffff; // needs to be lower 16 bits of descriptor address
    MXSDIO_XMTCONTROL(base)        = ( _BOOL2FLD(MXSDIO_XMTCONTROL_PTYCHKDISABLE, 1u) |
                                    _VAL2FLD(MXSDIO_XMTCONTROL_BURSTLEN, 2u) |
                                    _VAL2FLD(MXSDIO_XMTCONTROL_PREFETCHCTL,  (1U)) );
    
    if ( buf_len > 0 )
    {
        uint16_t inv_buf_len = ~(buf_len);
        
        memcpy(buffer, &buf_len, 2);
        memcpy(&buffer[2], &inv_buf_len, 2);

        tx_descriptor.addrlow = (uint32_t) buffer;
        tx_descriptor.ctrl2   = buf_len;
        
        /* Disable interrupts */
        interrupt_mask = MXSDIO_INTHOSTMASK(base);
        MXSDIO_INTHOSTMASK(base) = 0;
        
         /* Fire off the DMA */
        MXSDIO_XMTCONTROL(base) |= MXSDIO_XMTCONTROL_XMTEN_Msk;

        ret = Cy_SDIO_DEV_Poll_Transmit_Complete(base);
        
        MXSDIO_XMTCONTROL(base) &= (~MXSDIO_XMTCONTROL_XMTEN_Msk);

        /* Enable interrupts */
        MXSDIO_INTHOSTMASK(base) = interrupt_mask;
    }
        
    return ret;
}

cy_en_sdio_dev_status_t  Cy_SDIO_DEV_Receive_Buffer(MXSDIO_Type const *base, cy_stc_sdio_dev_context_t *context)
{
    if ((context->rx_buffer != NULL) && (context->max_rx_buf_len > 0))
    {
        cy_stc_sdio_dev_dma_descr_t rx_descriptor;
        /* Setup descriptors */
        rx_descriptor.ctrl1 = D64_CTRL1_EOF | D64_CTRL1_SOF;
        
        /* Setup DMA channel transmitter */
        MXSDIO_RCVADDRESSHI(base)    = 0;
        MXSDIO_RCVADDRESSLOW(base)    = ((uint32_t)&rx_descriptor);  // Transmit descriptor table address
        MXSDIO_RCVPTR(base)            = ((uint32_t)&rx_descriptor + sizeof(cy_stc_sdio_dev_dma_descr_t)) & 0xffff; // needs to be lower 16 bits of descriptor address
        MXSDIO_RCVCONTROL(base)        = ( _BOOL2FLD(MXSDIO_RCVCONTROL_PTYCHKDISABLE, 1u) |
                                        _VAL2FLD(MXSDIO_RCVCONTROL_BURSTLEN, 2u) |
                                        _VAL2FLD(MXSDIO_RCVCONTROL_PREFETCHCTL, 0u) );    

        rx_descriptor.addrlow = (uint32_t) context->rx_buffer;
        rx_descriptor.ctrl2   = D64_CTRL2_BC_USABLE_MASK & context->max_rx_buf_len;
        
         /* Fire off the DMA */
        MXSDIO_RCVCONTROL(base) |= MXSDIO_RCVCONTROL_RCVEN_Msk;
    }
    else
    {
        return CY_SDIO_DEV_ERROR_INVALID_PARAMETER;
    }

    return CY_SDIO_DEV_SUCCESS;
}

cy_en_sdio_dev_status_t Cy_SDIO_DEV_Receive_Buffer_Blocking(MXSDIO_Type const *base, char **buffer, uint32_t *buf_len)
{
    cy_en_sdio_dev_status_t ret = CY_SDIO_DEV_SUCCESS;
    cy_stc_sdio_dev_dma_descr_t rx_descriptor;
    uint32_t interrupt_mask;
    
    /* Setup descriptors */
    rx_descriptor.ctrl1 = D64_CTRL1_EOF | D64_CTRL1_SOF;
    
    /* Setup DMA channel transmitter */
    MXSDIO_RCVADDRESSHI(base)    = 0;
    MXSDIO_RCVADDRESSLOW(base)    = ((uint32_t)&rx_descriptor);  // Transmit descriptor table address
    MXSDIO_RCVPTR(base)            = ((uint32_t)&rx_descriptor + sizeof(cy_stc_sdio_dev_dma_descr_t)) & 0xffff; // needs to be lower 16 bits of descriptor address
    MXSDIO_RCVCONTROL(base)        = ( _BOOL2FLD(MXSDIO_RCVCONTROL_PTYCHKDISABLE, 1u) |
                                    _VAL2FLD(MXSDIO_RCVCONTROL_BURSTLEN, 2u) |
                                    _VAL2FLD(MXSDIO_RCVCONTROL_PREFETCHCTL, 1u) );    
    
    while ( *buf_len > 0 )
    {
        uint32_t read_size = MIN( *buf_len, D64_CTRL2_BC_USABLE_MASK );

        rx_descriptor.addrlow = (uint32_t) *buffer;
        rx_descriptor.ctrl2   = D64_CTRL2_BC_USABLE_MASK & read_size;
        
        /* Disable interrupts */
        interrupt_mask = MXSDIO_INTHOSTMASK(base);
        MXSDIO_INTHOSTMASK(base) = 0;
        
         /* Fire off the DMA */
        MXSDIO_RCVCONTROL(base) |= MXSDIO_RCVCONTROL_RCVEN_Msk;

        /* Update variables */
        *buf_len        -= read_size;
        *buffer        += read_size;

        ret = Cy_SDIO_DEV_Poll_Receive_Complete(base);
        
        MXSDIO_RCVCONTROL(base) &= (~MXSDIO_RCVCONTROL_RCVEN_Msk);
        
        /* Enable interrupts */
        MXSDIO_INTHOSTMASK(base) = interrupt_mask;
    }
        
    return ret;
}

uint32_t Cy_SDIO_DEV_GetInterruptStatus(MXSDIO_Type const *base)
{
    return MXSDIO_INTSTATUS(base);
}

void Cy_SDIO_DEV_ClearInterruptStatus(MXSDIO_Type const *base, uint32_t status)
{
    MXSDIO_INTSTATUS(base) = status;
    (void) MXSDIO_INTSTATUS(base);
}

uint32_t Cy_SDIO_DEV_GetInterruptMask(MXSDIO_Type const *base)
{
    return (MXSDIO_INTHOSTMASK(base));
}

void Cy_SDIO_DEV_SetInterruptMask(MXSDIO_Type const *base, uint32_t interruptMask)
{
    MXSDIO_INTHOSTMASK(base) = interruptMask;
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXSDIODEV */

/* [] END OF FILE */
