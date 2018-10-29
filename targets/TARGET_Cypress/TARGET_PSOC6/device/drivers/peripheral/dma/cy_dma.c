/***************************************************************************//**
* \file cy_dma.c
* \version 2.0.1
*
* \brief
* The source code file for the DMA driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2018, Cypress Semiconductor Corporation.  All rights reserved.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "cy_dma.h"

#if defined(__cplusplus)
extern "C" {
#endif


/*******************************************************************************
* Function Name: Cy_DMA_Descriptor_Init
****************************************************************************//**
*
* Initializes the descriptor structure in SRAM from a pre-initialized
* configuration structure.
* This function initializes only the descriptor and not the channel.
*
* \param descriptor
* The descriptor structure instance declared by the user/component.
*
* \param config
* This is a configuration structure that has all initialization information for
* the descriptor.
*
* \return
* The status /ref cy_en_dma_status_t.
*
*******************************************************************************/
cy_en_dma_status_t Cy_DMA_Descriptor_Init(cy_stc_dma_descriptor_t * descriptor, const cy_stc_dma_descriptor_config_t * config)
{
    cy_en_dma_status_t retVal = CY_DMA_BAD_PARAM;

    if ((NULL != descriptor) && (NULL != config))
    {
        CY_ASSERT_L3(CY_DMA_IS_RETRIGGER_VALID(config->retrigger));
        CY_ASSERT_L3(CY_DMA_IS_TRIG_TYPE_VALID(config->interruptType));
        CY_ASSERT_L3(CY_DMA_IS_TRIG_TYPE_VALID(config->triggerOutType));
        CY_ASSERT_L3(CY_DMA_IS_TRIG_TYPE_VALID(config->triggerInType));
        CY_ASSERT_L3(CY_DMA_IS_XFER_SIZE_VALID(config->srcTransferSize));
        CY_ASSERT_L3(CY_DMA_IS_XFER_SIZE_VALID(config->dstTransferSize));
        CY_ASSERT_L3(CY_DMA_IS_CHANNEL_STATE_VALID(config->channelState));
        CY_ASSERT_L3(CY_DMA_IS_DATA_SIZE_VALID(config->dataSize));
        CY_ASSERT_L3(CY_DMA_IS_TYPE_VALID(config->descriptorType));

        descriptor->ctl =
            _VAL2FLD(CY_DMA_CTL_RETRIG, config->retrigger) |
            _VAL2FLD(CY_DMA_CTL_INTR_TYPE, config->interruptType) |
            _VAL2FLD(CY_DMA_CTL_TR_OUT_TYPE, config->triggerOutType) |
            _VAL2FLD(CY_DMA_CTL_TR_IN_TYPE, config->triggerInType) |
            _VAL2FLD(CY_DMA_CTL_SRC_SIZE, config->srcTransferSize) |
            _VAL2FLD(CY_DMA_CTL_DST_SIZE, config->dstTransferSize) |
            _VAL2FLD(CY_DMA_CTL_CH_DISABLE, config->channelState) |
            _VAL2FLD(CY_DMA_CTL_DATA_SIZE, config->dataSize) |
            _VAL2FLD(CY_DMA_CTL_TYPE, config->descriptorType);

        descriptor->src = (uint32_t)config->srcAddress;

        descriptor->dst = (uint32_t)config->dstAddress;

        switch(config->descriptorType)
        {
            case CY_DMA_SINGLE_TRANSFER:
            {
                descriptor->xCtl = (uint32_t)config->nextDescriptor;
                break;
            }
            case CY_DMA_1D_TRANSFER:
            {
                CY_ASSERT_L2(CY_DMA_IS_INCR_VALID(config->srcXincrement));
                CY_ASSERT_L2(CY_DMA_IS_INCR_VALID(config->dstXincrement));
                CY_ASSERT_L2(CY_DMA_IS_COUNT_VALID(config->xCount));

                descriptor->xCtl =
                    _VAL2FLD(CY_DMA_CTL_SRC_INCR, config->srcXincrement) |
                    _VAL2FLD(CY_DMA_CTL_DST_INCR, config->dstXincrement) |
    /* Convert the data count from the user's range (1-256) into the machine range (0-255). */
                    _VAL2FLD(CY_DMA_CTL_COUNT, config->xCount - 1UL);

                descriptor->yCtl = (uint32_t)config->nextDescriptor;
                break;
            }
            case CY_DMA_2D_TRANSFER:
            {
                CY_ASSERT_L2(CY_DMA_IS_INCR_VALID(config->srcXincrement));
                CY_ASSERT_L2(CY_DMA_IS_INCR_VALID(config->dstXincrement));
                CY_ASSERT_L2(CY_DMA_IS_COUNT_VALID(config->xCount));
                CY_ASSERT_L2(CY_DMA_IS_INCR_VALID(config->srcYincrement));
                CY_ASSERT_L2(CY_DMA_IS_INCR_VALID(config->dstYincrement));
                CY_ASSERT_L2(CY_DMA_IS_COUNT_VALID(config->yCount));

                descriptor->xCtl =
                    _VAL2FLD(CY_DMA_CTL_SRC_INCR, config->srcXincrement) |
                    _VAL2FLD(CY_DMA_CTL_DST_INCR, config->dstXincrement) |
    /* Convert the data count from the user's range (1-256) into the machine range (0-255). */
                    _VAL2FLD(CY_DMA_CTL_COUNT, config->xCount - 1UL);

                descriptor->yCtl =
                    _VAL2FLD(CY_DMA_CTL_SRC_INCR, config->srcYincrement) |
                    _VAL2FLD(CY_DMA_CTL_DST_INCR, config->dstYincrement) |
    /* Convert the data count from the user's range (1-256) into the machine range (0-255). */
                    _VAL2FLD(CY_DMA_CTL_COUNT, config->yCount - 1UL);

                descriptor->nextPtr = (uint32_t)config->nextDescriptor;
                break;
            }
            default:
            {
                /* An unsupported type of a descriptor */
                break;
            }
        }

        retVal = CY_DMA_SUCCESS;
    }

    return retVal;
}


/*******************************************************************************
* Function Name: Cy_DMA_Descriptor_DeInit
****************************************************************************//**
*
* Clears the content of the specified descriptor.
*
* \param descriptor
* The descriptor structure instance declared by the user/component.
*
*******************************************************************************/
void Cy_DMA_Descriptor_DeInit(cy_stc_dma_descriptor_t * descriptor)
{
    descriptor->ctl = 0UL;
    descriptor->src = 0UL;
    descriptor->dst = 0UL;
    descriptor->xCtl = 0UL;
    descriptor->yCtl = 0UL;
    descriptor->nextPtr = 0UL;
}


/*******************************************************************************
* Function Name: Cy_DMA_Channel_Init
****************************************************************************//**
*
* Initializes the DMA channel with a descriptor and other parameters.
*
* \param base
* The pointer to the hardware DMA block.
*
* \param channel
* A channel number.
*
* \param channelConfig
* The structure that has the initialization information for the
* channel.
*
* \return
* The status /ref cy_en_dma_status_t.
*
*******************************************************************************/
cy_en_dma_status_t Cy_DMA_Channel_Init(DW_Type * base, uint32_t channel, cy_stc_dma_channel_config_t const * channelConfig)
{
    cy_en_dma_status_t retVal = CY_DMA_BAD_PARAM;

    if (((CY_DMA_IS_DW_CH_NR_VALID(base, channel)) && (NULL != channelConfig) && (NULL != channelConfig->descriptor)))
    {
        CY_ASSERT_L2(CY_DMA_IS_PRIORITY_VALID(channelConfig->priority));

        /* Set the current descriptor */
        base->CH_STRUCT[channel].CH_CURR_PTR = (uint32_t)channelConfig->descriptor;

        /* Set the channel configuration */
        base->CH_STRUCT[channel].CH_CTL = _BOOL2FLD(DW_CH_STRUCT_CH_CTL_PREEMPTABLE, channelConfig->preemptable) |
                                           _VAL2FLD(DW_CH_STRUCT_CH_CTL_PRIO,        channelConfig->priority)    |
                                          _BOOL2FLD(DW_CH_STRUCT_CH_CTL_ENABLED,     channelConfig->enable)      |
                                          _BOOL2FLD(DW_CH_STRUCT_CH_CTL_B,           channelConfig->bufferable);
        retVal = CY_DMA_SUCCESS;
    }

    return (retVal);
}


/*******************************************************************************
* Function Name: Cy_DMA_Channel_DeInit
****************************************************************************//**
*
* Clears the content of registers corresponding to the channel.
*
* \param base
* The pointer to the hardware DMA block.
*
* \param channel
* A channel number.
*
*******************************************************************************/
void Cy_DMA_Channel_DeInit(DW_Type * base, uint32_t channel)
{
    CY_ASSERT_L1(CY_DMA_IS_DW_CH_NR_VALID(base, channel));

    base->CH_STRUCT[channel].CH_CTL = 0UL;
    base->CH_STRUCT[channel].CH_IDX = 0UL;
    base->CH_STRUCT[channel].CH_CURR_PTR = 0UL;
    base->CH_STRUCT[channel].INTR_MASK = 0UL;
}


/*******************************************************************************
* Function Name: Cy_DMA_Descriptor_SetNextDescriptor
****************************************************************************//**
*
* Sets a Next Descriptor parameter for the specified descriptor.
*
* Based on the descriptor type, the offset of the address for the next descriptor may
* vary. For the single-transfer descriptor type, this register is at offset 0x0c.
* For the 1D-transfer descriptor type, this register is at offset 0x10.
* For the 2D-transfer descriptor type, this register is at offset 0x14.
*
* \param descriptor
* The descriptor structure instance declared by the user/component.
*
* \param nextDescriptor
* The pointer to the next descriptor.
*
*******************************************************************************/
void Cy_DMA_Descriptor_SetNextDescriptor(cy_stc_dma_descriptor_t * descriptor, cy_stc_dma_descriptor_t const * nextDescriptor)
{
    CY_ASSERT_L1(NULL != descriptor);

    switch((cy_en_dma_descriptor_type_t) _FLD2VAL(CY_DMA_CTL_TYPE, descriptor->ctl))
    {
        case CY_DMA_SINGLE_TRANSFER:
            descriptor->xCtl = (uint32_t)nextDescriptor;
            break;

        case CY_DMA_1D_TRANSFER:
            descriptor->yCtl = (uint32_t)nextDescriptor;
            break;

        case CY_DMA_2D_TRANSFER:
            descriptor->nextPtr = (uint32_t)nextDescriptor;
            break;

        default:
            /* Unsupported type of descriptor */
            break;
    }
}


/*******************************************************************************
* Function Name: Cy_DMA_Descriptor_GetNextDescriptor
****************************************************************************//**
*
* Returns a next descriptor address of the specified descriptor.
*
* Based on the descriptor type, the offset of the address for the next descriptor may
* vary. For a single-transfer descriptor type, this register is at offset 0x0c.
* For the 1D-transfer descriptor type, this register is at offset 0x10.
* For the 2D-transfer descriptor type, this register is at offset 0x14.
*
* \param descriptor
* The descriptor structure instance declared by the user/component.
*
* \return
* The pointer to the next descriptor.
*
*******************************************************************************/
cy_stc_dma_descriptor_t * Cy_DMA_Descriptor_GetNextDescriptor(cy_stc_dma_descriptor_t const * descriptor)
{
    cy_stc_dma_descriptor_t * retVal = NULL;

    CY_ASSERT_L1(NULL != descriptor);

    switch((cy_en_dma_descriptor_type_t) _FLD2VAL(CY_DMA_CTL_TYPE, descriptor->ctl))
    {
        case CY_DMA_SINGLE_TRANSFER:
            retVal = (cy_stc_dma_descriptor_t*) descriptor->xCtl;
            break;

        case CY_DMA_1D_TRANSFER:
            retVal = (cy_stc_dma_descriptor_t*) descriptor->yCtl;
            break;

        case CY_DMA_2D_TRANSFER:
            retVal = (cy_stc_dma_descriptor_t*) descriptor->nextPtr;
            break;

        default:
            /* An unsupported type of the descriptor */
            break;
    }

    return (retVal);
}


/*******************************************************************************
* Function Name: Cy_DMA_Descriptor_SetDescriptorType
****************************************************************************//**
*
* Sets the descriptor's type for the specified descriptor.
* Moves the next descriptor register value into the proper place in accordance
* to the actual descriptor type.
* During the descriptor's type changing, the Xloop and Yloop settings, such as
* data count and source/destination increment (i.e. the content of the
* xCtl and yCtl descriptor registers) might be lost (overriden by the
* next descriptor value) because of the different descriptor registers structures
* for different descriptor types. Set up carefully the Xloop
* (and Yloop, if used) data count and source/destination increment if the
* descriptor type is changed from a simpler to a more complicated type
* ("single transfer" -> "1D", "1D" -> "2D", etc.).
*
* \param descriptor
* The descriptor structure instance declared by the user/component.
*
* \param descriptorType
* The descriptor type \ref cy_en_dma_descriptor_type_t.
*
*******************************************************************************/
void Cy_DMA_Descriptor_SetDescriptorType(cy_stc_dma_descriptor_t * descriptor, cy_en_dma_descriptor_type_t descriptorType)
{
    CY_ASSERT_L1(NULL != descriptor);
    CY_ASSERT_L3(CY_DMA_IS_TYPE_VALID(descriptorType));

    if (descriptorType != Cy_DMA_Descriptor_GetDescriptorType(descriptor)) /* Don't perform if the type is not changed */
    {
        /* Store the current nextDescriptor pointer. */
        cy_stc_dma_descriptor_t * locNextDescriptor = Cy_DMA_Descriptor_GetNextDescriptor(descriptor);
        /* Change the descriptor type. */
        descriptor->ctl = _CLR_SET_FLD32U(descriptor->ctl, CY_DMA_CTL_TYPE, descriptorType);
        /* Restore the nextDescriptor pointer into the proper place. */
        Cy_DMA_Descriptor_SetNextDescriptor(descriptor, locNextDescriptor);
    }
}


#if defined(__cplusplus)
}
#endif

/* [] END OF FILE */
