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
 
#include "fsl_sai_hal.h"


/******************************************************************************
*Code
******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_init
 * Description   : Initialize the sai register, just set the register vaule to zero.
 *This function just clear the register value of sai.
 *END**************************************************************************/
void sai_hal_init(uint8_t instance)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);  
    /* Reset the internal logic of sai.*/
    BW_I2S_TCSR_SR(instance,1);
    BW_I2S_RCSR_SR(instance,1);
    /*Clear all the registers.*/
    HW_I2S_TCSR_WR(instance, 0);
    HW_I2S_TCR1_WR(instance, 0);
    HW_I2S_TCR2_WR(instance, 0);
    HW_I2S_TCR3_WR(instance, 0);
    HW_I2S_TCR4_WR(instance, 0);
    HW_I2S_TCR5_WR(instance, 0);
    HW_I2S_TMR_WR(instance,0);
    HW_I2S_RCSR_WR(instance, 0);
    HW_I2S_RCR1_WR(instance, 0);
    HW_I2S_RCR2_WR(instance, 0);
    HW_I2S_RCR3_WR(instance, 0);
    HW_I2S_RCR4_WR(instance, 0);
    HW_I2S_RCR5_WR(instance, 0);
    HW_I2S_RMR_WR(instance,0);
    HW_I2S_MDR_WR(instance,0);
    HW_I2S_MCR_WR(instance,0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_set_tx_bus
 * Description   : According to the protocol type to set the registers for tx.
 *The protocol can be I2S left, I2S right, I2S and so on.
 *END**************************************************************************/
void sai_hal_set_tx_bus(uint8_t instance, sai_bus_t bus_mode)
{
    assert(instance < HW_I2S_INSTANCE_COUNT); 

    switch (bus_mode)
    {
        case kSaiBusI2SLeft:
            BW_I2S_TCR2_BCP(instance,1U);/* Bit clock polarity */
            BW_I2S_TCR4_MF(instance,1U);/* MSB transmitted fisrt */
            BW_I2S_TCR4_FSE(instance,1U);/*Frame sync one bit early */
            BW_I2S_TCR4_FSP(instance,1U);/* Frame sync polarity, left channel is high */
            break;

        case kSaiBusI2SRight:
            BW_I2S_TCR2_BCP(instance,1U);/* Bit clock polarity */
            BW_I2S_TCR4_MF(instance,1U);/* MSB transmitted fisrt */
            BW_I2S_TCR4_FSE(instance,1U);/*Frame sync one bit early */
            BW_I2S_TCR4_FSP(instance,1U);/* Frame sync polarity, left chennel is high */
            break;

        case kSaiBusI2SType:
            BW_I2S_TCR2_BCP(instance,1U);/*Bit clock polarity */
            BW_I2S_TCR4_MF(instance,1U);/*MSB transmitted fisrt */
            BW_I2S_TCR4_FSE(instance,1U);/* Frame sync one bit early */
            BW_I2S_TCR4_FSP(instance,0U);/* Frame sync polarity, left channel is low */
            break;

        default:
            break;
    }

}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_set_rx_bus
 * Description   : According to the protocol type to set the registers for rx.
 *The protocol can be I2S left, I2S right, I2S and so on.
 *END**************************************************************************/
void sai_hal_set_rx_bus(uint8_t instance, sai_bus_t bus_mode)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    
    switch (bus_mode)
    {
        case kSaiBusI2SLeft:
            BW_I2S_RCR2_BCP(instance,1);/*Bit clock polarity */
            BW_I2S_RCR4_MF(instance,1);/* MSB transmitted fisrt */
            BW_I2S_RCR4_FSE(instance,1);/* Frame sync one bit early */
            BW_I2S_RCR4_FSP(instance,1);/* Frame sync polarity, left chennel is high */
            break;

        case kSaiBusI2SRight:
            BW_I2S_RCR2_BCP(instance,1);/* Bit clock polarity */
            BW_I2S_RCR4_MF(instance,1);/* MSB transmitted fisrt */
            BW_I2S_RCR4_FSE(instance,1);/* Frame sync one bit early */
            BW_I2S_RCR4_FSP(instance,1);/* Frame sync polarity, left chennel is high */
            break;

        case kSaiBusI2SType:
            BW_I2S_RCR2_BCP(instance,1);/* Bit clock polarity */
            BW_I2S_RCR4_MF(instance,1);/* MSB transmitted fisrt */
            BW_I2S_RCR4_FSE(instance,1);/* Frame sync one bit early */
            BW_I2S_RCR4_FSP(instance,0);/* Frame sync polarity, left channel is low */
            break;

        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_set_mclk_divider
 * Description   : Set the divider from the clock source to get the master clock.
 *The function would compute the divider number and set the number to the registers.
 *END**************************************************************************/
void sai_hal_set_mclk_divider(uint8_t instance, uint32_t mclk, uint32_t src_clk)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    
    uint32_t freq = src_clk;
    uint16_t fract, divide;
    uint32_t remaind = 0;
    uint32_t current_remainder = 0xffffffff;
    uint16_t current_fract = 0;
    uint16_t current_divide = 0;
    uint32_t mul_freq = 0;
    uint32_t max_fract = SAI_FRACT_MAX;
    /*In order to prevent overflow */
    freq /= 10;
    mclk/= 10;
    max_fract = mclk * SAI_DIV_MAX/freq;
    if(max_fract > SAI_FRACT_MAX)
    {
        max_fract = SAI_FRACT_MAX;
    }
    /* Looking for the closet frequency */
    for (fract = 1; fract < max_fract; fract ++)
    {
        mul_freq = freq * fract;
        remaind = mul_freq % mclk;
        divide = mul_freq/mclk;
        /* Find the exactly frequency */
        if (remaind == 0)
        {
            current_fract = fract;
            current_divide = mul_freq/mclk;
            break;
        }
        /* closer to next one */
        if (remaind > mclk/2)
        {
            remaind = mclk - remaind;
            divide += 1;
        }
        /* Update the closest div and fract */
        if (remaind < current_remainder)
        {
            current_fract = fract;
            current_divide = divide;
            current_remainder = remaind;
        }
    }
    HW_I2S_MDR_WR(instance,((uint32_t)(current_fract-1) << (uint32_t)I2S_MDR_FRACT_SHIFT| (uint32_t)(current_divide-1)));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_set_tx_master_slave
 * Description   : Set the tx master or slave mode.
 *The slave or master mode only would affect the clock direction relevant registers.
 *END**************************************************************************/
void sai_hal_set_tx_master_slave(uint8_t instance, sai_master_slave_t master_slave_mode)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    
    if (master_slave_mode == kSaiMaster)
    {
        BW_I2S_TCR2_BCD(instance,1);/* Bit clock generated internal */
        BW_I2S_TCR4_FSD(instance,1);/* Frame sync generated internal */
        BW_I2S_MCR_MOE(instance,1);/* Master clock generated internal */
        BW_I2S_TCR4_FSP(instance,1);/*Frame sync polarity */
    }
    else
    {
        BW_I2S_TCR2_BCD(instance,0);/* Bit clock generated external */
        BW_I2S_TCR4_FSD(instance,0);/* Frame sync generated external */
        BW_I2S_MCR_MOE(instance,0);/* Master clock generated external */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_set_rx_master_slave
 * Description   : Set the rx master or slave mode.
 *The slave or master mode only would affect the clock direction relevant registers.
 *END**************************************************************************/
void sai_hal_set_rx_master_slave(uint8_t instance, sai_master_slave_t master_slave_mode)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    
    if (master_slave_mode == kSaiMaster)
    {
        BW_I2S_RCR2_BCD(instance,1);
        BW_I2S_RCR4_FSD(instance,1);
        BW_I2S_MCR_MOE(instance,1);
    }
    else
    {
        BW_I2S_RCR2_BCD(instance,0);
        BW_I2S_RCR4_FSD(instance,0);
        BW_I2S_MCR_MOE(instance,0);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_set_tx_sync_mode
 * Description   : Set the tx sync mode.
 *Theer are four kinds of sync mode, async, sync, sync with other sai tx, sync with other sai rx.
 *END**************************************************************************/
void sai_hal_set_tx_sync_mode(uint8_t instance, sai_sync_mode_t sync_mode)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (sync_mode)
    {
        case kSaiModeAsync:
            BW_I2S_TCR2_SYNC(instance,0);
            break;
        case kSaiModeSync:
            BW_I2S_TCR2_SYNC(instance,1);
            BW_I2S_RCR2_SYNC(instance,0);/* Receiver must be async mode */
            break;
        case kSaiModeSyncWithOtherTx:
            BW_I2S_TCR2_SYNC(instance,2);
            break;
        case kSaiModeSyncWithOtherRx:
            BW_I2S_TCR2_SYNC(instance,3);
            break;
        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_set_rx_sync_mode
 * Description   : Set the rx sync mode.
 *Theer are four kinds of sync mode, async, sync, sync with other sai rx, sync with other sai tx.
 *END**************************************************************************/
void sai_hal_set_rx_sync_mode(uint8_t instance, sai_sync_mode_t sync_mode)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (sync_mode)
    {
        case kSaiModeAsync:
            BW_I2S_RCR2_SYNC(instance,0);
            break;
        case kSaiModeSync:
            BW_I2S_RCR2_SYNC(instance,1);
            BW_I2S_TCR2_SYNC(instance,0);/* TX should be async mode */
            break;
        case kSaiModeSyncWithOtherTx:
            BW_I2S_RCR2_SYNC(instance,3);
            break;
        case kSaiModeSyncWithOtherRx:
            BW_I2S_RCR2_SYNC(instance,2);
            break;
        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_get_fifo_read_pointer
 * Description   : Get the fifo read pointer.
 *
 *END**************************************************************************/
uint8_t sai_hal_get_fifo_read_pointer(uint8_t instance, sai_io_mode_t io_mode, uint8_t fifo_channel)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (io_mode)
    {
        case kSaiIOModeTransmit:
            return (uint8_t)BR_I2S_TFRn_RFP(instance,fifo_channel);
        case kSaiIOModeReceive:
            return (uint8_t)BR_I2S_RFRn_RFP(instance,fifo_channel);
        default:
            break;
    }
    return 0;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_get_fifo_write_pointer
 * Description   : Get the fifo write pointer.
 *
 *END**************************************************************************/
uint8_t sai_hal_get_fifo_write_pointer(uint8_t instance, sai_io_mode_t io_mode, uint8_t fifo_channel)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (io_mode)
    {
        case kSaiIOModeTransmit:
            return (uint8_t)BR_I2S_TFRn_WFP(instance,fifo_channel);
        case kSaiIOModeReceive:
            return (uint8_t)BR_I2S_RFRn_WFP(instance,fifo_channel);
        default:
            break;
    }
    return 0;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_get_fifo_address
 * Description   : Get the fifo access address.
 *The return value point to TDR/RDR register which is the interface of FIFO.
 *END**************************************************************************/
uint32_t* sai_hal_get_fifo_address(uint8_t instance, sai_io_mode_t io_mode, uint8_t fifo_channel)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (io_mode)
    {
        case kSaiIOModeTransmit:
            return (uint32_t *)HW_I2S_TDRn_ADDR(instance, fifo_channel);
        case kSaiIOModeReceive:
            return (uint32_t *)HW_I2S_RDRn_ADDR(instance,fifo_channel);
        default:
            break;
    }
    return NULL; 
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_enable_tx_interrupt
 * Description   : Enable the interrupt request source for tx.
 *The source can be word start, sync error, FIFO empty, FIFO error and FIFO request.
 *END**************************************************************************/
void sai_hal_enable_tx_interrupt(uint8_t instance, sai_interrupt_request_t source)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (source)
    {
        case kSaiIntrequestWordStart:
            BW_I2S_TCSR_WSIE(instance,1);
            break;
        case kSaiIntrequestSyncError:
            BW_I2S_TCSR_SEIE(instance,1);
            break;
        case kSaiIntrequestFIFOWarning:
            BW_I2S_TCSR_FWIE(instance,1);
            break;
        case kSaiIntrequestFIFOError:
            BW_I2S_TCSR_FEIE(instance,1);
            break;
        case kSaiIntrequestFIFORequest:
            BW_I2S_TCSR_FRIE(instance,1);
            break;
        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_enable_rx_interrupt
 * Description   : Enable the interrupt request source for rx.
 *The source can be word start, sync error, FIFO empty, FIFO error and FIFO request.
 *END**************************************************************************/
void sai_hal_enable_rx_interrupt(uint8_t instance, sai_interrupt_request_t source)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (source)
    {
        case kSaiIntrequestWordStart:
            BW_I2S_RCSR_WSIE(instance,1);
            break;
        case kSaiIntrequestSyncError:
            BW_I2S_RCSR_SEIE(instance,1);
            break;
        case kSaiIntrequestFIFOWarning:
            BW_I2S_RCSR_FWIE(instance,1);
            break;
        case kSaiIntrequestFIFOError:
            BW_I2S_RCSR_FEIE(instance,1);
            break;
        case kSaiIntrequestFIFORequest:
            BW_I2S_RCSR_FRIE(instance,1);
            break;
        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_disable_tx_interrupt
 * Description   : Disable the interrupt request source for tx.
 *The source can be word start, sync error, FIFO empty, FIFO error and FIFO request.
 *END**************************************************************************/
void sai_hal_disable_tx_interrupt(uint8_t instance, sai_interrupt_request_t source)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (source)
    {
        case kSaiIntrequestWordStart:
            BW_I2S_TCSR_WSIE(instance,0);
            break;
        case kSaiIntrequestSyncError:
            BW_I2S_TCSR_SEIE(instance,0);
            break;
        case kSaiIntrequestFIFOWarning:
            BW_I2S_TCSR_FWIE(instance,0);
            break;
        case kSaiIntrequestFIFOError:
            BW_I2S_TCSR_FEIE(instance,0);
            break;
        case kSaiIntrequestFIFORequest:
            BW_I2S_TCSR_FRIE(instance,0);
            break;
        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_disable_rx_interrupt
 * Description   : Disable the interrupt request source for tx.
 *The source can be word start, sync error, FIFO empty, FIFO error and FIFO request.
 *END**************************************************************************/
void sai_hal_disable_rx_interrupt(uint8_t instance, sai_interrupt_request_t source)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (source)
    {
        case kSaiIntrequestWordStart:
            BW_I2S_RCSR_WSIE(instance,0);
            break;
        case kSaiIntrequestSyncError:
            BW_I2S_RCSR_SEIE(instance,0);
            break;
        case kSaiIntrequestFIFOWarning:
            BW_I2S_RCSR_FWIE(instance,0);
            break;
        case kSaiIntrequestFIFOError:
            BW_I2S_RCSR_FEIE(instance,0);
            break;
        case kSaiIntrequestFIFORequest:
            BW_I2S_RCSR_FRIE(instance,0);
            break;
        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_enable_tx_dma
 * Description   : Enable the dma request source for tx.
 *The source can be FIFO empty or FIFO request.
 *END**************************************************************************/
void sai_hal_enable_tx_dma(uint8_t instance, sai_dma_request_t request)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (request)
    {
        case kSaiDmaReqFIFOWarning:
            BW_I2S_TCSR_FWDE(instance,1);
            break;
        case kSaiDmaReqFIFORequest:
            BW_I2S_TCSR_FRDE(instance,1);
            break;
        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_enable_rx_dma
 * Description   : Enable the dma request source for rx.
 *The source can be FIFO empty or FIFO request.
 *END**************************************************************************/
void sai_hal_enable_rx_dma(uint8_t instance, sai_dma_request_t request)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (request)
    {
        case kSaiDmaReqFIFOWarning:
            BW_I2S_RCSR_FWDE(instance,1);
            break;
        case kSaiDmaReqFIFORequest:
            BW_I2S_RCSR_FRDE(instance,1);
            break;
        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_disable_tx_dma
 * Description   : Disable the dma request source for tx.
 *The source can be FIFO empty or FIFO request.
 *END**************************************************************************/
void sai_hal_disable_tx_dma(uint8_t instance, sai_dma_request_t request)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (request)
    {
        case kSaiDmaReqFIFOWarning:
            BW_I2S_TCSR_FWDE(instance,0);
            break;
        case kSaiDmaReqFIFORequest:
            BW_I2S_TCSR_FRDE(instance,0);
            break;
        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_disable_rx_dma
 * Description   : Disable the dma request source for rx.
 *The source can be FIFO empty or FIFO request.
 *END**************************************************************************/
void sai_hal_disable_rx_dma(uint8_t instance, sai_dma_request_t request)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (request)
    {
        case kSaiDmaReqFIFOWarning:
            BW_I2S_RCSR_FWDE(instance,0);
            break;
        case kSaiDmaReqFIFORequest:
            BW_I2S_RCSR_FRDE(instance,0);
            break;
        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_clear_tx_state_flag
 * Description   : Clear the state flag of tx registers.
 *The state flag incudes word start flag, sync error flag and fifo error flag.
 *END**************************************************************************/
void sai_hal_clear_tx_state_flag(uint8_t instance, sai_state_flag_t flag)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (flag)
    {
        case kSaiStateFlagWordStart:
            BW_I2S_TCSR_WSF(instance,1);/* Write logic 1 to clear this bit */
            break;
        case kSaiStateFlagSyncError:
            BW_I2S_TCSR_SEF(instance,1);/* Write logic 1 to clear this bit */
            break;
        case kSaiStateFlagFIFOError:
            BW_I2S_TCSR_FEF(instance,1);/* Write logic 1 to clear this bit */
            break;
        case kSaiStateFlagSoftReset:
            BW_I2S_TCSR_SR(instance, 0);
            break;
        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_clear_rx_state_flag
 * Description   : Clear the state flag of rx registers.
 *The state flag incudes word start flag, sync error flag and fifo error flag.
 *END**************************************************************************/
void sai_hal_clear_rx_state_flag(uint8_t instance, sai_state_flag_t flag)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (flag)
    {
        case kSaiStateFlagWordStart:
            BW_I2S_RCSR_WSF(instance,1);/* Write logic 1 to clear this bit */
            break;
        case kSaiStateFlagSyncError:
            BW_I2S_RCSR_SEF(instance,1);
            break;
        case kSaiStateFlagFIFOError:
            BW_I2S_RCSR_FEF(instance,1);
            break;
        case kSaiStateFlagSoftReset:
            BW_I2S_RCSR_SR(instance, 0);
            break;
        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_reset_tx
 * Description   : Reset tx according to reset mode.
 *The reset mode can be software reset and FIFO reset. 
 *END**************************************************************************/
void sai_hal_reset_tx(uint8_t instance, sai_reset_type_t mode)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    uint32_t val = 0;
    switch (mode)
    {
        case kSaiResetTypeSoftware:
            BW_I2S_TCSR_SR(instance,1);
            break;
        case kSaiResetTypeFIFO:
            val = HW_I2S_TCSR_RD(instance);
            val |= I2S_TCSR_FR_MASK;
            HW_I2S_TCSR_WR(instance,val);
            break;
        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_reset_rx
 * Description   : Reset rx according to reset mode.
 *The reset mode can be software reset and FIFO reset. 
 *END**************************************************************************/
void sai_hal_reset_rx(uint8_t instance, sai_reset_type_t mode)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    uint32_t val = 0;
    switch (mode)
    {
        case kSaiResetTypeSoftware:
            BW_I2S_RCSR_SR(instance,1);
            break;
        case kSaiResetTypeFIFO:
            val = HW_I2S_TCSR_RD(instance);
            val |= I2S_TCSR_FR_MASK;
            HW_I2S_TCSR_WR(instance,val);
            break;
        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_set_tx_mode
 * Description   : Set the work mode for tx.
 *The work mode have stop mode, debug mode and normal mode. 
 *END**************************************************************************/
void sai_hal_set_tx_mode(uint8_t instance, sai_mode_t mode)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (mode)
    {
        case kSaiRunModeStop:
            BW_I2S_TCSR_STOPE(instance,1);/* Stop mode */
            break;
        case kSaiRunModeDebug:
            BW_I2S_TCSR_DBGE(instance,1);/* Debug mode */
            break;
        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_set_rx_mode
 * Description   : Set the work mode for rx.
 *The work mode have stop mode, debug mode and normal mode. 
 *END**************************************************************************/
void sai_hal_set_rx_mode(uint8_t instance, sai_mode_t mode)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (mode)
    {
        case kSaiRunModeStop:
            BW_I2S_RCSR_STOPE(instance,1);
            break;
        case kSaiRunModeDebug:
            BW_I2S_RCSR_DBGE(instance,1);
            break;
        default:
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_set_tx_bclk_swap
 * Description   : Swap the tx bit clock.
 *If the user want to swap the bit clock, just set bool variable to true. 
 *END**************************************************************************/
void sai_hal_set_tx_bclk_swap(uint8_t instance, bool ifswap)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    if (ifswap)/* Swap the bit clock */
    {
        BW_I2S_TCR2_BCS(instance,1);
    }
    else
    {
        BW_I2S_TCR2_BCS(instance,0);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_set_rx_bclk_swap
 * Description   : Swap the rx bit clock.
 *If the user want to swap the bit clock, just set bool variable to true. 
 *END**************************************************************************/
void sai_hal_set_rx_bclk_swap(uint8_t instance, bool ifswap)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    if (ifswap)/* Swap the bit clock */
    {
        BW_I2S_RCR2_BCS(instance,1);
    }
    else
    {
        BW_I2S_RCR2_BCS(instance,0);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_mclk_divider_is_update
 * Description   :Get the state if the master clock divider is updating.
 *
 *END**************************************************************************/
bool sai_hal_mclk_divider_is_update(uint8_t instance)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    return BR_I2S_MCR_DUF(instance);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_word_start_is_detected
 * Description   : The function would tell the user if the word start bit is set.
 * The bit is set by hardware. 
 *END**************************************************************************/
bool sai_hal_word_start_is_detected(uint8_t instance, sai_io_mode_t io_mode)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (io_mode)
    {
        case kSaiIOModeTransmit:
            return BR_I2S_TCSR_WSF(instance);
        case kSaiIOModeReceive:
            return BR_I2S_RCSR_WSF(instance);
        default:
            break;
    }
    return false;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_sync_error_is_detected
 * Description   : The function would tell the user if the sync error bit is set. 
 * The bit is set by hardware.
 *END**************************************************************************/
bool sai_hal_sync_error_is_detected(uint8_t instance, sai_io_mode_t io_mode)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (io_mode)
    {
        case kSaiIOModeTransmit:
            return BR_I2S_TCSR_SEF(instance);
        case kSaiIOModeReceive:
            return BR_I2S_RCSR_SEF(instance);
        default:
            break;
    }
    return false;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_fifo_warning_is_detected
 * Description   : The function would tell the user if the fifo warning bit is set. 
 * The bit is set by hardware.
 *END**************************************************************************/
bool sai_hal_fifo_warning_is_detected(uint8_t instance, sai_io_mode_t io_mode)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (io_mode)
    {
        case kSaiIOModeTransmit:
            return BR_I2S_TCSR_FWF(instance);
        case kSaiIOModeReceive:
            return BR_I2S_RCSR_FWF(instance);
        default:
            break;
    }
    return false;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_fifo_error_is_detected
 * Description   : The function would tell the user if the fifo error bit is set. 
 * The bit is set by hardware.
 *END**************************************************************************/
bool sai_hal_fifo_error_is_detected(uint8_t instance, sai_io_mode_t io_mode)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (io_mode)
    {
        case kSaiIOModeTransmit:
            return BR_I2S_TCSR_FEF(instance);
        case kSaiIOModeReceive:
            return BR_I2S_RCSR_FEF(instance);
        default:
            break;
    }
    return false;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sai_hal_fifo_request_is_detected
 * Description   : The function would tell the user if the fifo request bit is set. 
 * The bit is set by hardware.
 *END**************************************************************************/
bool sai_hal_fifo_request_is_detected(uint8_t instance, sai_io_mode_t io_mode)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    switch (io_mode)
    {
        case kSaiIOModeTransmit:
            return BR_I2S_TCSR_FRF(instance);
        case kSaiIOModeReceive:
            return BR_I2S_RCSR_FRF(instance);
        default:
            break;
    }
    return false;
}

/******************************************************************************
*EOF
******************************************************************************/

