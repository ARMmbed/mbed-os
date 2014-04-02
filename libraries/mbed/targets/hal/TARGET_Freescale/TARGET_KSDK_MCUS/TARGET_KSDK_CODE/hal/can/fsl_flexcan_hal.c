/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "fsl_flexcan_hal.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_EXT_MASK    (0x3FFFFFFFU)  /*!< FlexCAN RX FIFO ID filter*/
                                                                     /*! format A extended mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_EXT_SHIFT   (1U)           /*!< FlexCAN RX FIFO ID filter*/
                                                                     /*! format A extended shift.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_STD_MASK    (0x3FF80000U)  /*!< FlexCAN RX FIFO ID filter*/
                                                                     /*! format A standard mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_STD_SHIFT   (19U)          /*!< FlexCAN RX FIFO ID filter*/
                                                                     /*! format A standard shift.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_MASK    (0x3FFFU)      /*!< FlexCAN RX FIFO ID filter*/
                                                                     /*! format B extended mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT1  (16U)          /*!< FlexCAN RX FIFO ID filter*/
                                                                     /*! format B extended mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT2  (0U)           /*!< FlexCAN RX FIFO ID filter*/
                                                                     /*! format B extended mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_MASK    (0x3FF8U)      /*!< FlexCAN RX FIFO ID filter*/
                                                                     /*! format B standard mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_SHIFT1  (19U)          /*!< FlexCAN RX FIFO ID filter*/
                                                                     /*! format B standard shift1.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_SHIFT2  (3U)           /*!< FlexCAN RX FIFO ID filter*/
                                                                     /*! format B standard shift2.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK        (0xFFU)        /*!< FlexCAN RX FIFO ID filter*/
                                                                     /*! format C mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT1      (24U)          /*!< FlexCAN RX FIFO ID filter*/
                                                                     /*! format C shift1.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT2      (16U)          /*!< FlexCAN RX FIFO ID filter*/
                                                                     /*! format C shift2.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT3      (8U)           /*!< FlexCAN RX FIFO ID filter*/
                                                                     /*! format C shift3.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT4      (0U)           /*!< FlexCAN RX FIFO ID filter*/
                                                                     /*! format C shift4.*/
#define FLEXCAN_ALL_INT                               (0x0007U)      /*!< Masks for wakeup, error, bus off*/
                                                                     /*! interrupts*/
#define FLEXCAN_BYTE_DATA_FIELD_MASK                  (0xFFU)        /*!< Masks for byte data field.*/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_enable
 * Description   : Enable FlexCAN module.
 * This function will enable FlexCAN module clock.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_enable(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Check for low power mode*/
    if(BR_CAN_MCR_LPMACK(instance))
    {
        /* Enable clock*/
        HW_CAN_MCR_CLR(instance, BM_CAN_MCR_MDIS);
        /* Wait until enabled*/
        while (BR_CAN_MCR_LPMACK(instance)){}
    }

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_disable
 * Description   : Disable FlexCAN module.
 * This function will disable FlexCAN module clock.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_disable(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* To access the memory mapped registers*/
    /* Entre disable mode (hard reset).*/
    if(BR_CAN_MCR_MDIS(instance) == 0x0)
    {
      /* Clock disable (module)*/
      HW_CAN_MCR_SET(instance, BM_CAN_MCR_MDIS);

      /* Wait until disable mode acknowledged*/
      while (!(BR_CAN_MCR_LPMACK(instance))){}
    }

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_sw_reset
 * Description   : Reset FlexCAN module.
 * This function will reset FlexCAN module.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_sw_reset(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Check for low power mode*/
    if (BR_CAN_MCR_LPMACK(instance))
    {
        /* Enable clock*/
        HW_CAN_MCR_CLR(instance, BM_CAN_MCR_MDIS);
        /* Wait until enabled*/
        while (BR_CAN_MCR_LPMACK(instance)){}
    }

    /* Reset the FLEXCAN*/
    HW_CAN_MCR_SET(instance, BM_CAN_MCR_SOFTRST);

    /* Wait for reset cycle to complete*/
    while (BR_CAN_MCR_SOFTRST(instance)){}

    /* Set Freeze, Halt*/
    HW_CAN_MCR_SET(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* check for freeze Ack*/
    while ((!BR_CAN_MCR_FRZACK(instance)) ||
       (!BR_CAN_MCR_NOTRDY(instance))){}

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_select_clk
 * Description   : Select FlexCAN clock source.
 * This function will select either internal bus clock or external clock as
 * FlexCAN clock source.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_select_clk(
    uint8_t instance,
    flexcan_clk_source_t clk)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    if (clk == kFlexCanClkSource_Ipbus)
    {
       /* Internal bus clock (fsys/2)*/
        HW_CAN_CTRL1_SET(instance, BM_CAN_CTRL1_CLKSRC);
    }
    else if (clk == kFlexCanClkSource_Osc)
    {
        /* External clock*/
        HW_CAN_CTRL1_CLR(instance, BM_CAN_CTRL1_CLKSRC);
    }
    else
    {
       return (kStatus_FLEXCAN_InvalidArgument);
    }

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_init
 * Description   : Initialize FlexCAN module.
 * This function will reset FlexCAN module, set maximum number of message
 * buffers, initialize all message buffers as inactive, enable RX FIFO
 * if needed, mask all mask bits, and disable all MB interrupts. 
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_init(uint8_t instance, const flexcan_user_config_t *data)
{
    uint32_t i;
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);

    volatile CAN_Type  *flexcan_reg_ptr;
    flexcan_reg_ptr = ((CAN_Type *)REGS_CAN_BASE(instance));
    if (NULL == flexcan_reg_ptr)
    {
        return (kStatus_FLEXCAN_InvalidArgument);
    }

    /* Reset FLEXCAN, Halt, freeze mode*/
    if(flexcan_hal_sw_reset(instance))
    {
        return (kStatus_FLEXCAN_Fail);
    }

    /* Wait for entering the freeze mode*/
    while(!(BR_CAN_MCR_FRZACK(instance))) {}

    /* Set maximum number of message buffers*/
    BW_CAN_MCR_MAXMB(instance, data->max_num_mb);
    
    /* Initialize all message buffers as inactive*/
    for (i = 0; i < data->max_num_mb; i++)
    {
        flexcan_reg_ptr->MB[i].CS = 0x0;
        flexcan_reg_ptr->MB[i].ID = 0x0;
        flexcan_reg_ptr->MB[i].WORD0 = 0x0;
        flexcan_reg_ptr->MB[i].WORD1 = 0x0;
    }

    /* Enable RX FIFO if need*/
    if (data->is_rx_fifo_needed)
    {
        /* Enable RX FIFO*/
        HW_CAN_MCR_SET(instance, BM_CAN_MCR_RFEN);
        /* Set the number of the RX FIFO filters needed*/
        BW_CAN_CTRL2_RFFN(instance, data->num_id_filters);
        /* RX FIFO global mask*/
        HW_CAN_RXFGMASK_WR(instance, CAN_ID_EXT(CAN_RXFGMASK_FGM_MASK));
        for (i = 0; i < data->max_num_mb; i++)
        {
            /* RX individual mask*/
            HW_CAN_RXIMRn_WR(instance, i, CAN_ID_EXT(CAN_RXIMR_MI_MASK));
        }
    }

    /* Rx global mask*/
    HW_CAN_RXMGMASK_WR(instance, CAN_ID_EXT(CAN_RXMGMASK_MG_MASK));

    /* Rx reg 14 mask*/
    HW_CAN_RX14MASK_WR(instance, CAN_ID_EXT(CAN_RX14MASK_RX14M_MASK));

    /* Rx reg 15 mask*/
    HW_CAN_RX15MASK_WR(instance, CAN_ID_EXT(CAN_RX15MASK_RX15M_MASK));

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, BM_CAN_MCR_FRZ);

    /* Wait till exit of freeze mode*/
    while(BR_CAN_MCR_FRZACK(instance)){}

    /* Disable all MB interrupts*/
    HW_CAN_IMASK1_WR(instance, 0x0);

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_time_segments
 * Description   : Set FlexCAN time segments.
 * This function will set all FlexCAN time segments which define the length of
 * Propagation Segment in the bit time, the length of Phase Buffer Segment 2 in
 * the bit time, the length of Phase Buffer Segment 1 in the bit time, the ratio
 * between the PE clock frequency and the Serial Clock (Sclock) frequency, and
 * the maximum number of time quanta that a bit time can be changed by one
 * resynchronization. (One time quantum is equal to the Sclock period.)
 *
 *END**************************************************************************/
void flexcan_hal_set_time_segments(
    uint8_t instance,
    flexcan_time_segment_t *time_seg)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait for entering the freeze mode*/
    while(!(BR_CAN_MCR_FRZACK(instance))) {}

    /* Set FlexCAN time segments*/
    HW_CAN_CTRL1_CLR(instance, (CAN_CTRL1_PROPSEG_MASK | CAN_CTRL1_PSEG2_MASK |
                                CAN_CTRL1_PSEG1_MASK | CAN_CTRL1_PRESDIV_MASK) |
                                CAN_CTRL1_RJW_MASK);
    HW_CAN_CTRL1_SET(instance, (CAN_CTRL1_PROPSEG(time_seg->propseg) |
                                CAN_CTRL1_PSEG2(time_seg->pseg2) |
                                CAN_CTRL1_PSEG1(time_seg->pseg1) |
                                CAN_CTRL1_PRESDIV(time_seg->pre_divider) |
                                CAN_CTRL1_RJW(time_seg->rjw)));

    /* De-assert Freeze mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));
    /* Wait till exit of freeze mode*/
    while(BR_CAN_MCR_FRZACK(instance)){}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_get_time_segments
 * Description   : Get FlexCAN time segments.
 * This function will get all FlexCAN time segments defined.
 *
 *END**************************************************************************/
void flexcan_hal_get_time_segments(
    uint8_t instance,
    flexcan_time_segment_t *time_seg)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    time_seg->pre_divider = BR_CAN_CTRL1_PRESDIV(instance);
    time_seg->propseg = BR_CAN_CTRL1_PROPSEG(instance);
    time_seg->pseg1 = BR_CAN_CTRL1_PSEG1(instance);
    time_seg->pseg2 = BR_CAN_CTRL1_PSEG2(instance);
    time_seg->rjw = BR_CAN_CTRL1_RJW(instance);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_mb_tx
 * Description   : Configure a message buffer for transmission.
 * This function will first check if RX FIFO is enabled. If RX FIFO is enabled,
 * the function will make sure if the MB requested is not occupied by RX FIFO
 * and ID filter table. Then this function will copy user's buffer into the
 * message buffer data area and configure the message buffer as required for
 * transmission.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_set_mb_tx(
    uint8_t instance,
    const flexcan_user_config_t *data,
    uint32_t mb_idx,
    flexcan_mb_code_status_tx_t *cs,
    uint32_t msg_id,
    uint8_t *mb_data)
{
    uint32_t i;
    uint32_t val1, val2 = 1, temp, temp1;
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);

    volatile CAN_Type  *flexcan_reg_ptr;

    flexcan_reg_ptr = ((CAN_Type *)REGS_CAN_BASE(instance));
   if (NULL == flexcan_reg_ptr)
   {
       return (kStatus_FLEXCAN_InvalidArgument);
   }

    if (data->num_mb > data->max_num_mb)
    {
        return (kStatus_FLEXCAN_OutOfRange);
    }

    if (mb_idx >= data->max_num_mb)
    {
        return (kStatus_FLEXCAN_OutOfRange);
    }

    /* Check if RX FIFO is enabled*/
    if (BR_CAN_MCR_RFEN(instance))
    {
        /* Get the number of RX FIFO Filters*/
        val1 = (BR_CAN_CTRL2_RFFN(instance));
        /* Get the number if MBs occupied by RX FIFO and ID filter table*/
        /* the Rx FIFO occupies the memory space originally reserved for MB0-5*/
        /* Every number of RFFN means 8 number of RX FIFO filters*/
        /* and every 4 number of RX FIFO filters occupied one MB*/
        val2 = 6 + (val1 + 1) * 8 / 4;

        if (mb_idx <= (val2 - 1))
        {
            return (kStatus_FLEXCAN_InvalidArgument);
        }
    }

    /* Copy user's buffer into the message buffer data area*/
    if (mb_data != NULL)
    {
        flexcan_reg_ptr->MB[mb_idx].WORD0 = 0x0;
        flexcan_reg_ptr->MB[mb_idx].WORD1 = 0x0;

        for ( i=0 ; i < cs->data_length ; i++ )
        {
            temp1 = (*(mb_data + i));
            if (i < 4)
            {
                temp = temp1 << ((3 - i) * 8);
                flexcan_reg_ptr->MB[mb_idx].WORD0 |= temp;
            }
            else
            {
                temp = temp1 << ((7 - i) * 8);
                flexcan_reg_ptr->MB[mb_idx].WORD1 |= temp;
            }
        }
    }

    /* Set the ID according the format structure*/
    if (cs->msg_id_type == kFlexCanMbId_Ext)
    {
        /* ID [28-0]*/
        flexcan_reg_ptr->MB[mb_idx].ID &= ~(CAN_ID_STD_MASK | CAN_ID_EXT_MASK);
        flexcan_reg_ptr->MB[mb_idx].ID |= (msg_id & (CAN_ID_STD_MASK | CAN_ID_EXT_MASK));

        /* Set IDE*/
        flexcan_reg_ptr->MB[mb_idx].CS |= CAN_CS_IDE_MASK;

        /* Clear SRR bit*/
        flexcan_reg_ptr->MB[mb_idx].CS &= ~CAN_CS_SRR_MASK;

        /* Set the length of data in bytes*/
        flexcan_reg_ptr->MB[mb_idx].CS &= ~CAN_CS_DLC_MASK;
        flexcan_reg_ptr->MB[mb_idx].CS |= CAN_CS_DLC(cs->data_length);

        /* Set MB CODE*/
        /* Reset the code*/
        if (cs->code != kFlexCanTX_NotUsed)
        {
            if (cs->code == kFlexCanTX_Remote)
            {
                /* Set RTR bit*/
                flexcan_reg_ptr->MB[mb_idx].CS |= CAN_CS_RTR_MASK;
                cs->code = kFlexCanTX_Data;
            }

            /* Reset the code*/
            flexcan_reg_ptr->MB[mb_idx].CS &= ~(CAN_CS_CODE_MASK);

            /* Activating message buffer*/
            flexcan_reg_ptr->MB[mb_idx].CS |= CAN_CS_CODE(cs->code);
        }
    }
    else if(cs->msg_id_type == kFlexCanMbId_Std)
    {
        /* ID[28-18]*/
        flexcan_reg_ptr->MB[mb_idx].ID &= ~CAN_ID_STD_MASK;
        flexcan_reg_ptr->MB[mb_idx].ID |= CAN_ID_STD(msg_id);

        /* make sure IDE and SRR are not set*/
        flexcan_reg_ptr->MB[mb_idx].CS &= ~(CAN_CS_IDE_MASK | CAN_CS_SRR_MASK);

        /* Set the length of data in bytes*/
        flexcan_reg_ptr->MB[mb_idx].CS &= ~CAN_CS_DLC_MASK;
        flexcan_reg_ptr->MB[mb_idx].CS |= (cs->data_length) << CAN_CS_DLC_SHIFT;

        /* Set MB CODE*/
        if (cs->code != kFlexCanTX_NotUsed)
        {
            if (cs->code == kFlexCanTX_Remote)
            {
                /* Set RTR bit*/
                flexcan_reg_ptr->MB[mb_idx].CS |= CAN_CS_RTR_MASK;
                cs->code = kFlexCanTX_Data;
            }

            /* Reset the code*/
            flexcan_reg_ptr->MB[mb_idx].CS &= ~CAN_CS_CODE_MASK;

            /* Set the code*/
            flexcan_reg_ptr->MB[mb_idx].CS |= CAN_CS_CODE(cs->code);
        }
    }
    else
    {
        return (kStatus_FLEXCAN_InvalidArgument);
    }

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_mb_rx
 * Description   : Configure a message buffer for receiving.
 * This function will first check if RX FIFO is enabled. If RX FIFO is enabled,
 * the function will make sure if the MB requested is not occupied by RX FIFO
 * and ID filter table. Then this function will configure the message buffer as
 * required for receiving.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_set_mb_rx(
    uint8_t instance,
    const flexcan_user_config_t *data,
    uint32_t mb_idx,
    flexcan_mb_code_status_rx_t *cs,
    uint32_t msg_id)
{
    uint32_t val1, val2 = 1;
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);

    volatile CAN_Type  *flexcan_reg_ptr;

    flexcan_reg_ptr = ((CAN_Type *)REGS_CAN_BASE(instance));
    if (NULL == flexcan_reg_ptr)
    {
        return (kStatus_FLEXCAN_InvalidArgument);
    }

    if (data->num_mb > data->max_num_mb)
    {
        return (kStatus_FLEXCAN_OutOfRange);
    }

    if (mb_idx >= data->num_mb)
    {
        return (kStatus_FLEXCAN_OutOfRange);
    }

    /* Check if RX FIFO is enabled*/
    if (BR_CAN_MCR_RFEN(instance))
    {
        /* Get the number of RX FIFO Filters*/
        val1 = BR_CAN_CTRL2_RFFN(instance);
        /* Get the number if MBs occupied by RX FIFO and ID filter table*/
        /* the Rx FIFO occupies the memory space originally reserved for MB0-5*/
        /* Every number of RFFN means 8 number of RX FIFO filters*/
        /* and every 4 number of RX FIFO filters occupied one MB*/
        val2 = 6 + (val1 + 1) * 8 / 4;

        if (mb_idx <= (val2 - 1))
        {
            return (kStatus_FLEXCAN_InvalidArgument);
        }
    }

    /* Set the ID according the format structure*/
    if (cs->msg_id_type == kFlexCanMbId_Ext)
    {
        /* Set IDE*/
        flexcan_reg_ptr->MB[mb_idx].CS |= CAN_CS_IDE_MASK;

        /* Clear SRR bit*/
        flexcan_reg_ptr->MB[mb_idx].CS &= ~CAN_CS_SRR_MASK;

        /* Set the length of data in bytes*/
        flexcan_reg_ptr->MB[mb_idx].CS &= ~CAN_CS_DLC_MASK;
        flexcan_reg_ptr->MB[mb_idx].CS |= CAN_CS_DLC(cs->data_length);

        /* ID [28-0]*/
        flexcan_reg_ptr->MB[mb_idx].ID &= ~(CAN_ID_STD_MASK | CAN_ID_EXT_MASK);
        flexcan_reg_ptr->MB[mb_idx].ID |= (msg_id & (CAN_ID_STD_MASK | CAN_ID_EXT_MASK));

        /* Set MB CODE*/
        if (cs->code != kFlexCanRX_NotUsed)
        {
            flexcan_reg_ptr->MB[mb_idx].CS &= ~CAN_CS_CODE_MASK;
            flexcan_reg_ptr->MB[mb_idx].CS |= CAN_CS_CODE(cs->code);
        }
    }
    else if(cs->msg_id_type == kFlexCanMbId_Std)
    {
        /* Make sure IDE and SRR are not set*/
        flexcan_reg_ptr->MB[mb_idx].CS &= ~(CAN_CS_IDE_MASK | CAN_CS_SRR_MASK);

        /* Set the length of data in bytes*/
        flexcan_reg_ptr->MB[mb_idx].CS &= ~CAN_CS_DLC_MASK;
        flexcan_reg_ptr->MB[mb_idx].CS |= (cs->data_length) << CAN_CS_DLC_SHIFT;

        /* ID[28-18]*/
        flexcan_reg_ptr->MB[mb_idx].ID &= ~CAN_ID_STD_MASK;
        flexcan_reg_ptr->MB[mb_idx].ID |= CAN_ID_STD(msg_id);

        /* Set MB CODE*/
        if (cs->code != kFlexCanRX_NotUsed)
        {
            flexcan_reg_ptr->MB[mb_idx].CS &= ~CAN_CS_CODE_MASK;
            flexcan_reg_ptr->MB[mb_idx].CS |= CAN_CS_CODE(cs->code);
        }
    }
    else
    {
        return (kStatus_FLEXCAN_InvalidArgument);
    }

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_get_mb
 * Description   : Get a message buffer field values.
 * This function will first check if RX FIFO is enabled. If RX FIFO is enabled,
 * the function will make sure if the MB requested is not occupied by RX FIFO
 * and ID filter table. Then this function will get the message buffer field
 * values and copy the MB data field into user's buffer.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_get_mb(
    uint8_t instance,
    const flexcan_user_config_t *data,
    uint32_t mb_idx,
    flexcan_mb_t *mb)
{
    uint32_t i;
    uint32_t val1, val2 = 1;
    volatile CAN_Type  *flexcan_reg_ptr;
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);

    flexcan_reg_ptr = ((CAN_Type *)REGS_CAN_BASE(instance));
    if (NULL == flexcan_reg_ptr)
    {
        return (kStatus_FLEXCAN_InvalidArgument);
    }

    if (mb_idx >= data->max_num_mb)
    {
        return (kStatus_FLEXCAN_OutOfRange);
    }

    /* Check if RX FIFO is enabled*/
    if (BR_CAN_MCR_RFEN(instance))
    {
        /* Get the number of RX FIFO Filters*/
        val1 = BR_CAN_CTRL2_RFFN(instance);
        /* Get the number if MBs occupied by RX FIFO and ID filter table*/
        /* the Rx FIFO occupies the memory space originally reserved for MB0-5*/
        /* Every number of RFFN means 8 number of RX FIFO filters*/
        /* and every 4 number of RX FIFO filters occupied one MB*/
        val2 = 6 + (val1 + 1) * 8 / 4;

        if (mb_idx <= (val2 - 1))
        {
            return (kStatus_FLEXCAN_InvalidArgument);
        }
    }

    /* Get a MB field values*/
    mb->cs = flexcan_reg_ptr->MB[mb_idx].CS;
    if ((mb->cs) & CAN_CS_IDE_MASK)
    {
        mb->msg_id = flexcan_reg_ptr->MB[mb_idx].ID;
    }
    else
    {
        mb->msg_id = (flexcan_reg_ptr->MB[mb_idx].ID) >> CAN_ID_STD_SHIFT;
    }

    /* Copy MB data field into user's buffer*/
    for (i=0 ; i < kFlexCanMessageSize ; i++)
    {
        if (i < 4)
        {
            mb->data[3 - i] = ((flexcan_reg_ptr->MB[mb_idx].WORD0) >> (i * 8)) &
                              FLEXCAN_BYTE_DATA_FIELD_MASK;
        }
        else
        {
            mb->data[11 - i] = ((flexcan_reg_ptr->MB[mb_idx].WORD1) >> ((i - 4) * 8)) &
                               FLEXCAN_BYTE_DATA_FIELD_MASK;
        }
    }

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_lock_rx_mb
 * Description   : Lock the RX message buffer.
 * This function will the RX message buffer.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_lock_rx_mb(
    uint8_t instance,
    const flexcan_user_config_t *data,
    uint32_t mb_idx)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);

    volatile CAN_Type  *flexcan_reg_ptr;

    flexcan_reg_ptr = ((CAN_Type *)REGS_CAN_BASE(instance));
    if (NULL == flexcan_reg_ptr)
    {
        return (kStatus_FLEXCAN_InvalidArgument);
    }

    if (mb_idx >= data->max_num_mb)
    {
        return (kStatus_FLEXCAN_OutOfRange);
    }

    /* Lock the mailbox*/
    flexcan_reg_ptr->MB[mb_idx].CS;

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_enable_rx_fifo
 * Description   : Enable Rx FIFO feature.
 * This function will enable the Rx FIFO feature.
 *
 *END**************************************************************************/
void flexcan_hal_enable_rx_fifo(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait for entering the freeze mode*/
    while (!(BR_CAN_MCR_FRZACK(instance))){}

    /* Enable RX FIFO*/
    HW_CAN_MCR_SET(instance, BM_CAN_MCR_RFEN);

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_disable_rx_fifo
 * Description   : Disable Rx FIFO feature.
 * This function will disable the Rx FIFO feature.
 *
 *END**************************************************************************/
void flexcan_hal_disable_rx_fifo(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait for entering the freeze mode*/
    while(!(BR_CAN_MCR_FRZACK(instance))){}

    /* Disable RX FIFO*/
    HW_CAN_MCR_CLR(instance, BM_CAN_MCR_RFEN);

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_rx_fifo_filters_number
 * Description   : Set the number of Rx FIFO filters.
 * This function will define the number of Rx FIFO filters.
 *
 *END**************************************************************************/
void flexcan_hal_set_rx_fifo_filters_number(
    uint8_t instance,
    uint32_t number)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait for entering the freeze mode*/
    while(!(BR_CAN_MCR_FRZACK(instance))){}

    /* Set the number of RX FIFO ID filters*/
    BW_CAN_CTRL2_RFFN(instance, number);

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_max_mb_number
 * Description   : Set the number of the last Message Buffers.
 * This function will define the number of the last Message Buffers
 *
*END**************************************************************************/
void flexcan_hal_set_max_mb_number(
    uint8_t instance,
    const flexcan_user_config_t *data)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT);

    /* Wait for entering the freeze mode*/
    while(!(BR_CAN_MCR_FRZACK(instance))){}

    /* Set the maximum number of MBs*/
    BW_CAN_MCR_MAXMB(instance, data->max_num_mb);

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_id_filter_table_elements
 * Description   : Set ID filter table elements.
 * This function will set up ID filter table elements.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_set_id_filter_table_elements(
    uint8_t instance,
    const flexcan_user_config_t *data,
    flexcan_rx_fifo_id_element_format_t id_format,
    flexcan_id_table_t *id_filter_table)
{
    uint32_t i, j;
    uint32_t val1, val2, val;
    volatile CAN_Type  *flexcan_reg_ptr;

    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);

    flexcan_reg_ptr = ((CAN_Type *)REGS_CAN_BASE(instance));
    if (NULL == flexcan_reg_ptr)
    {
        return (kStatus_FLEXCAN_InvalidArgument);
    }

   switch(id_format)
    {
    case (kFlexCanRxFifoIdElementFormat_A):
        /* One full ID (standard and extended) per ID Filter Table element.*/
        BW_CAN_MCR_IDAM(instance, kFlexCanRxFifoIdElementFormat_A);
        if (id_filter_table->is_remote_mb)
        {
            val = 1U << 31U;
        }
        if (id_filter_table->is_extended_mb)
        {
            val |= 1 << 30;
            j = 0;
            for (i = 0; i < (data->num_id_filters + 1) * 8; i += 4)
            {
                flexcan_reg_ptr->MB[6 + i - j * 3].CS =
                val + ((*(id_filter_table->id_filter + i)) <<
                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_EXT_SHIFT &
                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_EXT_MASK);
                flexcan_reg_ptr->MB[6 + i - j * 3].ID =
                val + ((*(id_filter_table->id_filter + i + 1)) <<
                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_EXT_SHIFT &
                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_EXT_MASK);
                flexcan_reg_ptr->MB[6 + i - j * 3].WORD0 =
                val + ((*(id_filter_table->id_filter + i + 2)) <<
                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_EXT_SHIFT &
                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_EXT_MASK);
                flexcan_reg_ptr->MB[6 + i - j * 3].WORD1 =
                val + ((*(id_filter_table->id_filter + i + 3)) <<
                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_EXT_SHIFT &
                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_EXT_MASK);
                j++;
            }
        }
        else
        {
            j = 0;
            for (i = 0; i < (data->num_id_filters + 1) * 8; i += 4)
            {
                flexcan_reg_ptr->MB[6 + i - j * 3].CS =
                val + ((*(id_filter_table->id_filter + i)) <<
                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_STD_SHIFT &
                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_STD_MASK);
                flexcan_reg_ptr->MB[6 + i - j * 3].ID =
                val + ((*(id_filter_table->id_filter + i + 1)) <<
                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_STD_SHIFT &
                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_STD_MASK);
                flexcan_reg_ptr->MB[6 + i - j * 3].WORD0 =
                val + ((*(id_filter_table->id_filter + i + 2)) <<
                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_STD_SHIFT &
                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_STD_MASK);
                flexcan_reg_ptr->MB[6 + i - j * 3].WORD1 =
                val + ((*(id_filter_table->id_filter + i + 3)) <<
                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_STD_SHIFT &
                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_STD_MASK);
                j++;
            }
        }
        break;
    case (kFlexCanRxFifoIdElementFormat_B):
        /* Two full standard IDs or two partial 14-bit (standard and extended) IDs*/
        /* per ID Filter Table element.*/
        BW_CAN_MCR_IDAM(instance, kFlexCanRxFifoIdElementFormat_B);
        if (id_filter_table->is_remote_mb)
        {
            val1 = 1U << 31U;
            val2 = 1 << 15;
        }
        if (id_filter_table->is_extended_mb)
        {
            val1 |= 1 << 30;
            val2 |= 1 << 14;
            j = 0;
            for (i = 0; i < (data->num_id_filters + 1) * 8; i += 8)
            {
                flexcan_reg_ptr->MB[6 + i - j * 3].CS =
                val1 + ((*(id_filter_table->id_filter + i)) &
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_MASK <<
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT1);
                flexcan_reg_ptr->MB[6 + i - j * 3].CS |=
                val2 + ((*(id_filter_table->id_filter + i + 1)) &
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_MASK <<
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT2);

                flexcan_reg_ptr->MB[6 + i - j * 3].ID =
                val1 + ((*(id_filter_table->id_filter + i + 2)) &
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_MASK <<
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT1);
                flexcan_reg_ptr->MB[6 + i - j * 3].ID |=
                val2 + ((*(id_filter_table->id_filter + i + 3)) &
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_MASK <<
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT2);

                flexcan_reg_ptr->MB[6 + i - j * 3].WORD0 =
                val1 + ((*(id_filter_table->id_filter + i + 4)) &
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_MASK <<
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT1);
                flexcan_reg_ptr->MB[6 + i - j * 3].WORD0 |=
                val2 + ((*(id_filter_table->id_filter + i + 5)) &
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_MASK <<
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT2);

                flexcan_reg_ptr->MB[6 + i - j * 3].WORD1 =
                val1 + ((*(id_filter_table->id_filter + i + 6)) &
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_MASK <<
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT1);
                flexcan_reg_ptr->MB[6 + i - j * 3].WORD1 |=
                val2 + ((*(id_filter_table->id_filter + i + 7)) &
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_MASK <<
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT2);
                j++;
            }
        }
        else
        {
            j = 0;
            for (i = 0; i < (data->num_id_filters + 1) * 8; i += 8)
            {
                flexcan_reg_ptr->MB[6 + i - j * 3].CS =
                val1 + (((*(id_filter_table->id_filter + i)) &
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_MASK) <<
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_SHIFT1);
                flexcan_reg_ptr->MB[6 + i - j * 3].CS |=
                val2 + (((*(id_filter_table->id_filter + i + 1)) &
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_MASK) <<
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_SHIFT2);

                flexcan_reg_ptr->MB[6 + i - j * 3].ID =
                val1 + (((*(id_filter_table->id_filter + i + 2)) &
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_MASK) <<
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_SHIFT1);
                flexcan_reg_ptr->MB[6 + i - j * 3].ID |=
                val2 + (((*(id_filter_table->id_filter + i + 3)) &
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_MASK) <<
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_SHIFT2);

                flexcan_reg_ptr->MB[6 + i - j * 3].WORD0 =
                val1 + (((*(id_filter_table->id_filter + i + 4)) &
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_MASK) <<
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_SHIFT1);
                flexcan_reg_ptr->MB[6 + i - j * 3].WORD0 |=
                val2 + (((*(id_filter_table->id_filter + i + 5)) &
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_MASK) <<
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_SHIFT2);

                flexcan_reg_ptr->MB[6 + i - j * 3].WORD1 =
                val1 + (((*(id_filter_table->id_filter + i + 6)) &
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_MASK) <<
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_SHIFT1);
                flexcan_reg_ptr->MB[6 + i - j * 3].WORD1 |=
                val2 + (((*(id_filter_table->id_filter + i + 7)) &
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_MASK) <<
                         FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_SHIFT2);
                j++;
            }
        }
      break;
    case (kFlexCanRxFifoIdElementFormat_C):
        /* Four partial 8-bit Standard IDs per ID Filter Table element.*/
        BW_CAN_MCR_IDAM(instance, kFlexCanRxFifoIdElementFormat_C);
        j = 0;
        for (i = 0; i < (data->num_id_filters + 1) * 8; i += 16)
        {
            flexcan_reg_ptr->MB[6 + i - j * 3].CS =
            ((*(id_filter_table->id_filter + i)) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK <<
             FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT1);
            flexcan_reg_ptr->MB[6 + i - j * 3].CS |=
            ((*(id_filter_table->id_filter + i + 1)) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK <<
             FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT2);
            flexcan_reg_ptr->MB[6 + i - j * 3].CS |=
            ((*(id_filter_table->id_filter + i + 2)) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK <<
             FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT3);
            flexcan_reg_ptr->MB[6 + i - j * 3].CS |=
            ((*(id_filter_table->id_filter + i + 3)) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK <<
             FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT4);

            flexcan_reg_ptr->MB[6 + i - j * 3].ID =
            ((*(id_filter_table->id_filter + i + 4)) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK <<
             FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT1);
            flexcan_reg_ptr->MB[6 + i - j * 3].ID |=
            ((*(id_filter_table->id_filter + i + 5)) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK <<
             FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT2);
            flexcan_reg_ptr->MB[6 + i - j * 3].ID |=
            ((*(id_filter_table->id_filter + i + 6)) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK <<
             FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT3);
            flexcan_reg_ptr->MB[6 + i - j * 3].ID |=
            ((*(id_filter_table->id_filter + i + 7)) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK <<
             FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT4);

            flexcan_reg_ptr->MB[6 + i - j * 3].WORD0 =
            ((*(id_filter_table->id_filter + i + 8)) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK <<
             FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT1);
            flexcan_reg_ptr->MB[6 + i - j * 3].WORD0 |=
            ((*(id_filter_table->id_filter + i + 9)) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK <<
             FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT2);
            flexcan_reg_ptr->MB[6 + i - j * 3].WORD0 |=
            ((*(id_filter_table->id_filter + i + 10)) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK <<
             FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT3);
            flexcan_reg_ptr->MB[6 + i - j * 3].WORD0 |=
            ((*(id_filter_table->id_filter + i + 11)) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK <<
             FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT4);

            flexcan_reg_ptr->MB[6 + i - j * 3].WORD1 =
            ((*(id_filter_table->id_filter + i + 12)) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK <<
             FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT1);
            flexcan_reg_ptr->MB[6 + i - j * 3].WORD1 |=
            ((*(id_filter_table->id_filter + i + 13)) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK <<
             FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT2);
            flexcan_reg_ptr->MB[6 + i - j * 3].WORD1 |=
            ((*(id_filter_table->id_filter + i + 14)) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK <<
             FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT3);
            flexcan_reg_ptr->MB[6 + i - j * 3].WORD1 |=
            ((*(id_filter_table->id_filter + i + 15)) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK <<
             FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT4);

            j++;
        }
        break;
    case (kFlexCanRxFifoIdElementFormat_D):
        /* All frames rejected.*/
        BW_CAN_MCR_IDAM(instance, kFlexCanRxFifoIdElementFormat_D);
        break;
    default:
        return kStatus_FLEXCAN_InvalidArgument;
   }

   return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_rx_fifo
 * Description   : Confgure RX FIFO ID filter table elements.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_set_rx_fifo(
    uint8_t instance,
    const flexcan_user_config_t *data,
    flexcan_rx_fifo_id_element_format_t id_format,
    flexcan_id_table_t *id_filter_table)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);

    if (!data->is_rx_fifo_needed)
    {
        return (kStatus_FLEXCAN_InvalidArgument);
    }

    if (data->num_mb > data->max_num_mb)
    {
        return (kStatus_FLEXCAN_OutOfRange);
    }

    /* Set RX FIFO ID filter table elements*/
    return flexcan_hal_set_id_filter_table_elements(instance, data, id_format, id_filter_table);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_enable_mb_interrupt
 * Description   : Enable the corresponding Message Buffer interrupt.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_enable_mb_interrupt(
    uint8_t instance,
    const flexcan_user_config_t *data,
    uint32_t mb_idx)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);
    uint32_t temp;

    if ( mb_idx >= data->max_num_mb)
    {
        return (kStatus_FLEXCAN_OutOfRange);
    }

    /* Enable the corresponding message buffer Interrupt*/
    temp = 0x1 << mb_idx;
    HW_CAN_IMASK1_SET(instance, temp);

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_disable_mb_interrupt
 * Description   : Disable the corresponding Message Buffer interrupt.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_disable_mb_interrupt(
    uint8_t instance,
    const flexcan_user_config_t *data,
    uint32_t mb_idx)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);
    uint32_t temp;

    if (mb_idx >= data->max_num_mb)
    {
        return (kStatus_FLEXCAN_OutOfRange);
    }

    /* Disable the corresponding message buffer Interrupt*/
    temp = 0x1 << mb_idx;
    HW_CAN_IMASK1_CLR(instance, temp);

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_enable_error_interrupt
 * Description   : Enable the error interrupts.
 * This function will enable Error interrupt.
 *
 *END**************************************************************************/
void flexcan_hal_enable_error_interrupt(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Enable Error interrupt*/
    HW_CAN_CTRL1_SET(instance, CAN_CTRL1_ERRMSK_MASK);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_disable_error_interrupt
 * Description   : Disable the error interrupts.
 * This function will disable Error interrupt.
 *
 *END**************************************************************************/
void flexcan_hal_disable_error_interrupt(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Disable Error interrupt*/
    HW_CAN_CTRL1_CLR(instance, CAN_CTRL1_ERRMSK_MASK);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_enable_bus_off_interrupt
 * Description   : Enable the Bus off interrupts.
 * This function will enable Bus Off interrupt.
 *
 *END**************************************************************************/
void flexcan_hal_enable_bus_off_interrupt(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Enable Bus Off interrupt*/
    HW_CAN_CTRL1_SET(instance, CAN_CTRL1_BOFFMSK_MASK);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_disable_bus_off_interrupt
 * Description   : Disable the Bus off interrupts.
 * This function will disable Bus Off interrupt.
 *
 *END**************************************************************************/
void flexcan_hal_disable_bus_off_interrupt(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Disable Bus Off interrupt*/
    HW_CAN_CTRL1_CLR(instance, CAN_CTRL1_BOFFMSK_MASK);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_enable_wakeup_interrupt
 * Description   : Enable the wakeup interrupts.
 * This function will enable Wake up interrupt.
 *
 *END**************************************************************************/
void flexcan_hal_enable_wakeup_interrupt(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Enable Wake Up interrupt*/
    BW_CAN_MCR_WAKMSK(instance, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_disable_wakeup_interrupt
 * Description   : Disable the wakeup interrupts.
 * This function will disable Wake up interrupt.
 *
 *END**************************************************************************/
void flexcan_hal_disable_wakeup_interrupt(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Disable Wake Up interrupt*/
    BW_CAN_MCR_WAKMSK(instance, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_enable_tx_warning_interrupt
 * Description   : Enable the TX warning interrupts.
 * This function will enable TX warning interrupt.
 *
 *END**************************************************************************/
void flexcan_hal_enable_tx_warning_interrupt(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Enable TX warning interrupt*/
    HW_CAN_CTRL1_SET(instance, CAN_CTRL1_TWRNMSK_MASK);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_disable_tx_warning_interrupt
 * Description   : Disable the TX warning interrupts.
 * This function will disable TX warning interrupt.
 *
 *END**************************************************************************/
void flexcan_hal_disable_tx_warning_interrupt(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Disable TX warning interrupt*/
    HW_CAN_CTRL1_CLR(instance, CAN_CTRL1_TWRNMSK_MASK);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_enable_rx_warning_interrupt
 * Description   : Enable the RX warning interrupts.
 * This function will enable RX warning interrupt.
 *
 *END**************************************************************************/
void flexcan_hal_enable_rx_warning_interrupt(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Enable RX warning interrupt*/
    HW_CAN_CTRL1_SET(instance, CAN_CTRL1_RWRNMSK_MASK);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_disable_rx_warning_interrupt
 * Description   : Disable the RX warning interrupts.
 * This function will disable RX warning interrupt.
 *
 *END**************************************************************************/
void flexcan_hal_disable_rx_warning_interrupt(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Disable RX warning interrupt*/
    HW_CAN_CTRL1_CLR(instance, CAN_CTRL1_RWRNMSK_MASK);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_exit_freeze_mode
 * Description   : Exit of freeze mode.
 *
 *END**************************************************************************/
void flexcan_hal_exit_freeze_mode(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_enter_freeze_mode
 * Description   : Enter the freeze mode.
 *
 *END**************************************************************************/
void flexcan_hal_enter_freeze_mode(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);
    HW_CAN_MCR_SET(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait for entering the freeze mode*/
    while (!(BR_CAN_MCR_FRZACK(instance))){}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_get_mb_int_flag
 * Description   : Get the corresponding message buffer interrupt flag.
 *
 *END**************************************************************************/
uint8_t flexcan_hal_get_mb_int_flag(
    uint8_t instance,
    const flexcan_user_config_t *data,
    uint32_t mb_idx)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);
    assert(mb_idx < data->max_num_mb);
    uint32_t temp;

    /* Get the corresponding message buffer interrupt flag*/
    temp = 0x1 << mb_idx;
    if (HW_CAN_IFLAG1_RD(instance) & temp)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_get_err_counter
 * Description   : Get transmit error counter and receive error counter.
 *
 *END**************************************************************************/
void flexcan_hal_get_err_counter(
    uint8_t instance,
    flexcan_berr_counter_t *err_cnt)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Get transmit error counter and receive error counter*/
    err_cnt->rxerr = HW_CAN_ECR(instance).B.RXERRCNT;
    err_cnt->txerr = HW_CAN_ECR(instance).B.TXERRCNT;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_clear_err_interrupt_status
 * Description   : Clear all error interrupt status.
 *
 *END**************************************************************************/
void flexcan_hal_clear_err_interrupt_status(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    if(HW_CAN_IFLAG1_RD(instance) & FLEXCAN_ALL_INT)
    {
        HW_CAN_ESR1_SET(instance, FLEXCAN_ALL_INT);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_read_fifo
 * Description   : Read Rx FIFO data.
 * This function will copy MB[0] data field into user's buffer.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_read_fifo(
    uint8_t instance,
    flexcan_mb_t *rx_fifo)
{
    uint32_t i;
    volatile CAN_Type  *flexcan_reg_ptr;
    assert(instance < HW_CAN_INSTANCE_COUNT);

    flexcan_reg_ptr = ((CAN_Type *)REGS_CAN_BASE(instance));
    if (NULL == flexcan_reg_ptr)
    {
        return (kStatus_FLEXCAN_InvalidArgument);
    }

    rx_fifo->cs = flexcan_reg_ptr->MB[0].CS;

    if ((rx_fifo->cs) & CAN_CS_IDE_MASK)
    {
        rx_fifo->msg_id = flexcan_reg_ptr->MB[0].ID;
    }
    else
    {
        rx_fifo->msg_id = (flexcan_reg_ptr->MB[0].ID) >> CAN_ID_STD_SHIFT;
    }

    /* Copy MB[0] data field into user's buffer*/
    for ( i=0 ; i < kFlexCanMessageSize ; i++ )
    {
        if (i < 4)
        {
            rx_fifo->data[3 - i] = ((flexcan_reg_ptr->MB[0].WORD0) >> (i * 8)) &
                                   FLEXCAN_BYTE_DATA_FIELD_MASK;
        }
        else
        {
            rx_fifo->data[11 - i] = ((flexcan_reg_ptr->MB[0].WORD1) >> ((i - 4) * 8)) &
                                    FLEXCAN_BYTE_DATA_FIELD_MASK;
        }
    }
    
    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_mask_type
 * Description   : Set RX masking type.
 * This function will set RX masking type as RX global mask or RX individual
 * mask.
 *
 *END**************************************************************************/
void flexcan_hal_set_mask_type(
    uint8_t instance,
    flexcan_rx_mask_type_t type)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT);

    /* Wait for entering the freeze mode*/
    while (!(BR_CAN_MCR_FRZACK(instance))){}

    /* Set RX masking type (RX global mask or RX individual mask)*/
    if (type == kFlexCanRxMask_Global)
    {
        /* Enable Global RX masking*/
        HW_CAN_MCR_CLR(instance, BM_CAN_MCR_IRMQ);
    }
    else
    {
        /* Enable Individual Rx Masking and Queue*/
        HW_CAN_MCR_SET(instance, BM_CAN_MCR_IRMQ);
    }

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_rx_fifo_global_std_mask
 * Description   : Set Rx FIFO global mask as the 11-bit standard mask.
 *
 *END**************************************************************************/
void flexcan_hal_set_rx_fifo_global_std_mask(
    uint8_t instance,
    uint32_t std_mask)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT);

    /* Wait for entering the freeze mode*/
    while (!(BR_CAN_MCR_FRZACK(instance))){}

    /* 11 bit standard mask*/
    HW_CAN_RXFGMASK_WR(instance, CAN_ID_STD(std_mask));

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_rx_fifo_global_ext_mask
 * Description   : Set Rx FIFO global mask as the 29-bit extended mask.
 *
 *END**************************************************************************/
void flexcan_hal_set_rx_fifo_global_ext_mask(
    uint8_t instance,
    uint32_t ext_mask)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT);

    /* Wait for entering the freeze mode*/
    while (!(BR_CAN_MCR_FRZACK(instance))){}

    /* 29-bit extended mask*/
    HW_CAN_RXFGMASK_WR(instance, CAN_ID_EXT(ext_mask));

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_rx_individual_std_mask
 * Description   : Set Rx individual mask as the 11-bit standard mask.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_set_rx_individual_std_mask(
    uint8_t instance,
    const flexcan_user_config_t * data,
    uint32_t mb_idx,
    uint32_t std_mask)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);

    if (mb_idx >= data->max_num_mb)
    {
        return (kStatus_FLEXCAN_OutOfRange);
    }

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT);

    /* Wait for entering the freeze mode*/
    while (!(BR_CAN_MCR_FRZACK(instance))){}

    /* 11 bit standard mask*/
    HW_CAN_RXIMRn_WR(instance, mb_idx, CAN_ID_STD(std_mask));

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_rx_individual_ext_mask
 * Description   : Set Rx individual mask as the 29-bit extended mask.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_set_rx_individual_ext_mask(
    uint8_t instance,
    const flexcan_user_config_t * data,
    uint32_t mb_idx,
    uint32_t ext_mask)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);

    if (mb_idx >= data->max_num_mb)
    {
        return (kStatus_FLEXCAN_OutOfRange);
    }

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT);

    /* Wait for entering the freeze mode*/
    while (!(BR_CAN_MCR_FRZACK(instance))){}

    /* 29-bit extended mask*/
    HW_CAN_RXIMRn_WR(instance, mb_idx, CAN_ID_EXT(ext_mask));

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_rx_mb_global_std_mask
 * Description   : Set Rx Message Buffer global mask as the 11-bit standard mask.
 *
 *END**************************************************************************/
void flexcan_hal_set_rx_mb_global_std_mask(
    uint8_t instance,
    uint32_t std_mask)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT);

    /* Wait for entering the freeze mode*/
    while (!(BR_CAN_MCR_FRZACK(instance))){}

    /* 11 bit standard mask*/
    HW_CAN_RXMGMASK_WR(instance, CAN_ID_STD(std_mask));

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_rx_mb_buf14_std_mask
 * Description   : Set Rx Message Buffer 14 mask as the 11-bit standard mask.
 *
 *END**************************************************************************/
void flexcan_hal_set_rx_mb_buf14_std_mask(
    uint8_t instance,
    uint32_t std_mask)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT);

    /* Wait for entering the freeze mode*/
    while (!(BR_CAN_MCR_FRZACK(instance))){}

    /* 11 bit standard mask*/
    HW_CAN_RX14MASK_WR(instance, CAN_ID_STD(std_mask));

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_rx_mb_buf15_std_mask
 * Description   : Set Rx Message Buffer 15 mask as the 11-bit standard mask.
 *
 *END**************************************************************************/
void flexcan_hal_set_rx_mb_buf15_std_mask(
    uint8_t instance,
    uint32_t std_mask)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT);

    /* Wait for entering the freeze mode*/
    while (!(BR_CAN_MCR_FRZACK(instance))){}

    /* 11 bit standard mask*/
    HW_CAN_RX15MASK_WR(instance, CAN_ID_STD(std_mask));

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_rx_mb_global_ext_mask
 * Description   : Set Rx Message Buffer global mask as the 29-bit extended mask.
 *
 *END**************************************************************************/
void flexcan_hal_set_rx_mb_global_ext_mask(
    uint8_t instance,
    uint32_t ext_mask)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT);

    /* Wait for entering the freeze mode*/
    while (!(HW_CAN_MCR_RD(instance))){}

    /* 29-bit extended mask*/
    HW_CAN_RXMGMASK_WR(instance, CAN_ID_EXT(ext_mask));

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_rx_mb_buf14_ext_mask
 * Description   : Set Rx Message Buffer 14 mask as the 29-bit extended mask.
 *
 *END**************************************************************************/
void flexcan_hal_set_rx_mb_buf14_ext_mask(
    uint8_t instance,
    uint32_t ext_mask)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT);

    /* Wait for entering the freeze mode*/
    while (!(BR_CAN_MCR_FRZACK(instance))){}

    /* 29-bit extended mask*/
    HW_CAN_RX14MASK_WR(instance, CAN_ID_EXT(ext_mask));

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_set_rx_mb_buf15_ext_mask
 * Description   : Set Rx Message Buffer 15 mask as the 29-bit extended mask.
 *
 *END**************************************************************************/
void flexcan_hal_set_rx_mb_buf15_ext_mask(
    uint8_t instance,
    uint32_t ext_mask)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT);

    /* Wait for entering the freeze mode*/
    while (!(BR_CAN_MCR_FRZACK(instance))){}

    /* 29-bit extended mask*/
    HW_CAN_RX15MASK_WR(instance, CAN_ID_EXT(ext_mask));

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_enable_operation_mode
 * Description   : Enable a FlexCAN operation mode.
 * This function will enable one of the modes listed in flexcan_operation_modes_t.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_enable_operation_mode(
    uint8_t instance,
    flexcan_operation_modes_t mode)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    if (mode == kFlexCanFreezeMode)
    {
        /* Debug mode, Halt and Freeze*/
        HW_CAN_MCR_SET(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

        /* Wait for entering the freeze mode*/
        while (!(BR_CAN_MCR_FRZACK(instance))){}

        return (kStatus_FLEXCAN_Success);
    }
    else if (mode == kFlexCanDisableMode)
    {
        /* Debug mode, Halt and Freeze*/
        HW_CAN_MCR_SET(instance, BM_CAN_MCR_MDIS);
        return (kStatus_FLEXCAN_Success);
    }

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT);

    /* Wait for entering the freeze mode*/
    while (!(BR_CAN_MCR_FRZACK(instance))){}

    if (mode == kFlexCanNormalMode)
    {
        HW_CAN_MCR_CLR(instance, BM_CAN_MCR_SUPV);
    }
    else if (mode == kFlexCanListenOnlyMode)
    {
        HW_CAN_CTRL1_SET(instance, BM_CAN_CTRL1_LOM);
    }
    else if (mode == kFlexCanLoopBackMode)
    {
        HW_CAN_CTRL1_SET(instance, BM_CAN_CTRL1_LPB);
    }
    else
    {
        return kStatus_FLEXCAN_InvalidArgument;
    }

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexcan_hal_disable_operation_mode
 * Description   : Disable a FlexCAN operation mode.
 * This function will disable one of the modes listed in flexcan_operation_modes_t.
 *
 *END**************************************************************************/
flexcan_status_t flexcan_hal_disable_operation_mode(
    uint8_t instance,
    flexcan_operation_modes_t mode)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    if (mode == kFlexCanFreezeMode)
    {
        /* De-assert Freeze Mode*/
        HW_CAN_MCR_CLR(instance, BM_CAN_MCR_FRZ);

        /* Wait till exit of freeze mode*/
        while (BR_CAN_MCR_FRZACK(instance)){}

        return (kStatus_FLEXCAN_Success);
    }
    else if (mode == kFlexCanDisableMode)
    {
        /* Enable module mode*/
        HW_CAN_MCR_CLR(instance, BM_CAN_MCR_MDIS);
        return (kStatus_FLEXCAN_Success);
    }

    /* Set Freeze mode*/
    HW_CAN_MCR_SET(instance, BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT);

    /* Wait for entering the freeze mode*/
    while (!(BR_CAN_MCR_FRZACK(instance))){}

    if (mode == kFlexCanNormalMode)
    {
        HW_CAN_MCR_SET(instance, BM_CAN_MCR_SUPV);
    }
    else if (mode == kFlexCanListenOnlyMode)
    {
        HW_CAN_CTRL1_CLR(instance, BM_CAN_CTRL1_LOM);
    }
    else if (mode == kFlexCanLoopBackMode)
    {
        HW_CAN_CTRL1_CLR(instance, BM_CAN_CTRL1_LPB);
    }
    else
    {
        return kStatus_FLEXCAN_InvalidArgument;
    }

    /* De-assert Freeze Mode*/
    HW_CAN_MCR_CLR(instance, (BM_CAN_MCR_FRZ | BM_CAN_MCR_HALT));

    /* Wait till exit of freeze mode*/
    while (BR_CAN_MCR_FRZACK(instance)){}

    return (kStatus_FLEXCAN_Success);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

