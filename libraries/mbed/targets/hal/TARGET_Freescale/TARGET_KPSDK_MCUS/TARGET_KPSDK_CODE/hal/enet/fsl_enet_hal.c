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
 
#include "fsl_enet_hal.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
 
/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_Init
 * Description: Init ENET to reset status.
 * 
 *END*********************************************************************/
uint32_t ENET_HAL_Init(uint32_t baseAddr)
{
    uint32_t timeOut = 0;

    /* Reset ENET*/
    BW_ENET_ECR_RESET(baseAddr, 1);
    /* Check for reset complete*/
    while(BR_ENET_ECR_RESET(baseAddr) && (timeOut < kEnetMaxTimeout))
    {
        timeOut ++;
    }
    /* Check for Timeout*/
    if(timeOut == kEnetMaxTimeout)
    {
        return kStatus_ENET_InitTimeout;
    }

     /* Disable ENET interrupt and Clear interrupt events*/
    HW_ENET_EIMR_WR(baseAddr, 0);
    HW_ENET_EIR_WR(baseAddr, kEnetAllInterrupt);

    /* Clear multicast group and individual hash register*/
    HW_ENET_GALR_WR(baseAddr, 0);
    HW_ENET_GAUR_WR(baseAddr, 0);
    HW_ENET_IALR_WR(baseAddr, 0);
    HW_ENET_IAUR_WR(baseAddr, 0);

    return kStatus_ENET_Success;
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_SetMac
 * Description: Configure Mac controller of the ENET device.
 * 
 *END*********************************************************************/
 void ENET_HAL_SetMac(uint32_t baseAddr, const enet_mac_config_t *macCfgPtr, uint32_t sysClk)
{
    assert(macCfgPtr);
    assert(macCfgPtr->rxMaxFrameLen <= macCfgPtr->rxTruncLen);
    /* Configure operate mode, stop control of MAC controller*/
    ENET_HAL_SetMacMode(baseAddr, macCfgPtr->macMode);
    ENET_HAL_SetStopCmd(baseAddr, macCfgPtr->macCtlConfigure & kEnetStopModeEnable);
    ENET_HAL_SetDebugCmd(baseAddr, macCfgPtr->macCtlConfigure & kEnetDebugModeEnable);

    /* Configure MAC receive controller*/
    ENET_HAL_SetPayloadCheckCmd(baseAddr, macCfgPtr->macCtlConfigure & kEnetPayloadlenCheckEnable);
    ENET_HAL_SetFlowControlCmd(baseAddr, macCfgPtr->macCtlConfigure & kEnetRxFlowControlEnable);
    ENET_HAL_SetRxcrcFwdCmd(baseAddr, macCfgPtr->macCtlConfigure & kEnetRxCrcFwdEnable);
    ENET_HAL_SetPauseFwdCmd(baseAddr, macCfgPtr->macCtlConfigure & kEnetRxPauseFwdEnable);
    ENET_HAL_SetPadRemoveCmd(baseAddr, macCfgPtr->macCtlConfigure & kEnetRxPadRemoveEnable);
    ENET_HAL_SetBroadcastRejectCmd(baseAddr, macCfgPtr->macCtlConfigure & kEnetRxBcRejectEnable);
    ENET_HAL_SetPromiscuousCmd(baseAddr, macCfgPtr->macCtlConfigure & kEnetRxPromiscuousEnable);
    /* Check the rmiiCfgMode if NULL use Default value*/
    if(!macCfgPtr->rmiiCfgPtr)
    {
        enet_config_rmii_t rmiiCfg;
        rmiiCfg.duplex = kEnetCfgFullDuplex;
        rmiiCfg.speed = kEnetCfgSpeed100M;
        rmiiCfg.mode = kEnetCfgRmii;
        rmiiCfg.isLoopEnabled = false;
        rmiiCfg.isRxOnTxDisabled = false;
        ENET_HAL_SetRMIIMode(baseAddr, &rmiiCfg);
    }
    else
    {
        ENET_HAL_SetRMIIMode(baseAddr, macCfgPtr->rmiiCfgPtr);
    }
    ENET_HAL_SetRxMaxFrameLen(baseAddr,  macCfgPtr->rxMaxFrameLen);
    ENET_HAL_SetTruncLen(baseAddr, macCfgPtr->rxTruncLen);

    /* Configure MAC transmit controller*/
    ENET_HAL_SetTxcrcFwdCmd(baseAddr, macCfgPtr->macCtlConfigure & kEnetTxCrcFwdEnable);
    ENET_HAL_SetMacAddrInsertCmd(baseAddr, macCfgPtr->macCtlConfigure & kEnetMacAddrInsert);
    ENET_HAL_SetTxInterPacketGap(baseAddr, macCfgPtr->txInterPacketGap);
    ENET_HAL_SetPauseDuration(baseAddr, macCfgPtr->pauseDuration);

    /* Configure Accelerator control*/
    if(macCfgPtr->macCtlConfigure & kEnetTxAccelEnable)
    {
        ENET_HAL_SetTxAccelerator(baseAddr, macCfgPtr->txAccelerPtr);
    }
    if(macCfgPtr->macCtlConfigure & kEnetRxAccelEnable)
    {
        ENET_HAL_SetRxAccelerator(baseAddr, macCfgPtr->rxAccelerPtr);
    }

    /* Set SMI MDC Clock*/
    ENET_HAL_SetSMI(baseAddr, (sysClk/(2 * kEnetMdcFreq)), macCfgPtr->clkCycle, macCfgPtr->macCtlConfigure & kEnetSMIPreambleDisable);

    /* MIB control*/
    ENET_HAL_SetMibCmd(baseAddr, macCfgPtr->macCtlConfigure & kEnetMacMibEnable);

    /* Configure enhanced MAC*/
    ENET_HAL_SetEnhancedMacCmd(baseAddr, macCfgPtr->macCtlConfigure & kEnetMacEnhancedEnable);

    /* Configuer the Mac address*/
    ENET_HAL_SetMacAddr(baseAddr, macCfgPtr->macAddr);
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_SetTxBuffDescriptors
 * Description: Configure transmit buffer descriptors of the ENET device.
 * 
 *END*********************************************************************/
void ENET_HAL_SetTxBuffDescriptors(uint32_t baseAddr, volatile enet_bd_struct_t * txBds, uint8_t * txBuffer, uint32_t txBdNumber, uint32_t txBuffSizeAlign)
{
    assert(txBuffSizeAlign >= kEnetMinBuffSize);
    /* Initialize transmit buffer descriptor rings start address*/
    ENET_HAL_SetTxBuffDescripAddr(baseAddr, (uint32_t)txBds);

    /* Initialize receive and transmit buffer descriptors*/
    ENET_HAL_InitTxBuffDescriptors(txBds, txBuffer, txBdNumber, txBuffSizeAlign);
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_SetRxBuffDescriptors
 * Description: Configure receive buffer descriptors of the ENET device.
 * 
 *END*********************************************************************/
void ENET_HAL_SetRxBuffDescriptors(uint32_t baseAddr, volatile enet_bd_struct_t *rxBds, uint8_t *rxBuffer, uint32_t rxBdNumber, uint32_t rxBuffSizeAlign)
{
    /* Initialize transmit buffer descriptor rings start address*/
    ENET_HAL_SetRxBuffDescripAddr(baseAddr, (uint32_t)rxBds);

    ENET_HAL_SetRxMaxBuffSize(baseAddr, rxBuffSizeAlign);

    /* Initialize receive and transmit buffer descriptors*/
    ENET_HAL_InitRxBuffDescriptors(rxBds, rxBuffer, rxBdNumber, rxBuffSizeAlign);
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_Set1588TimerStart
 * Description: Configure 1588 timer and run 1588 timer.
 * 
 *END*********************************************************************/
void ENET_HAL_Set1588TimerStart(uint32_t baseAddr, enet_config_ptp_timer_t * ptpCfgPtr)
{
    /* Restart 1588 timer*/
    ENET_HAL_Set1588TimerRestart(baseAddr);
   /* Init 1588 timer*/
    ENET_HAL_Set1588Timer(baseAddr, ptpCfgPtr);
   /* Active 1588 timer*/
    ENET_HAL_Set1588TimerCmd(baseAddr, true);
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_Set1588TimerChnCmp
 * Description: Configure 1588 timer channel compare feature and enable the 
 * 1588 timer channel interupt. This is instead of TS_TIMER for the old silicon
 * which has no TS_TIMER interrup.
 * 
 *END*********************************************************************/
 void ENET_HAL_Set1588TimerChnCmp(uint32_t baseAddr, enet_timer_channel_t channel, uint32_t cmpValOld, uint32_t cmpValNew)
{
    ENET_HAL_Set1588TimerChnCmpVal(baseAddr, channel, cmpValOld);
    ENET_HAL_Set1588TimerChnMode(baseAddr, channel, kEnetChannelToggleCompare);
    ENET_HAL_Set1588TimerChnInt(baseAddr, channel, true);
    ENET_HAL_Set1588TimerChnCmpVal(baseAddr, channel, cmpValNew);
}
/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_SetFifo
 * Description: Configure transmit and receive FIFO of the ENET device.
 * 
 *END*********************************************************************/
 void ENET_HAL_SetFifo(uint32_t baseAddr, const enet_mac_config_t *macCfgPtr)
{
    enet_config_tx_fifo_t txFifoCfg;
    enet_config_rx_fifo_t rxFifoCfg;

    assert(macCfgPtr);
    /* Initialize the transmit FIFO*/
    txFifoCfg.isStoreForwardEnabled = macCfgPtr->txFifo.isStoreForwardEnabled;
    txFifoCfg.txFifoWrite = macCfgPtr->txFifo.txFifoWrite;
    txFifoCfg.txEmpty = macCfgPtr->txFifo.txEmpty;
    txFifoCfg.txAlmostEmpty = macCfgPtr->txFifo.txAlmostEmpty;
    txFifoCfg.txAlmostFull = macCfgPtr->txFifo.txAlmostFull;
    if(((macCfgPtr->macCtlConfigure & kEnetTxAccelEnable)&& (macCfgPtr->txAccelerPtr != NULL)
         && ((macCfgPtr->txAccelerPtr->isIpCheckEnabled) || (macCfgPtr->txAccelerPtr->isProtocolCheckEnabled)))
        || (macCfgPtr->macCtlConfigure & kEnetStoreAndFwdEnable))
    {
        txFifoCfg.isStoreForwardEnabled = 1;
    }
    ENET_HAL_SetTxFifo(baseAddr, &txFifoCfg);  

    /* Initialzie the receive FIFO*/
    rxFifoCfg.rxFull = macCfgPtr->rxFifo.rxFull;
    rxFifoCfg.rxAlmostFull = macCfgPtr->rxFifo.rxAlmostFull;
    rxFifoCfg.rxEmpty = macCfgPtr->rxFifo.rxEmpty;
    rxFifoCfg.rxAlmostEmpty = macCfgPtr->rxFifo.rxAlmostEmpty;

    if(((macCfgPtr->macCtlConfigure & kEnetRxAccelEnable)&& (macCfgPtr->rxAccelerPtr != NULL) 
         &&((macCfgPtr->rxAccelerPtr->isIpcheckEnabled) || (macCfgPtr->rxAccelerPtr->isProtocolCheckEnabled)))
         || (macCfgPtr->macCtlConfigure & kEnetStoreAndFwdEnable))
    {
        rxFifoCfg.rxFull = 0;
    }
    ENET_HAL_SetRxFifo(baseAddr, &rxFifoCfg);
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_GetMibRxStat
 * Description: Get all received statistics from MIB.
 *
 *END*********************************************************************/
void ENET_HAL_GetMibRxStat(uint32_t baseAddr, enet_mib_rx_stat_t *rxStat)
{
    assert(rxStat); 
    rxStat->rxPackets = ENET_HAL_GetRxPackets(baseAddr);
    rxStat->rxBroadcastPackets = ENET_HAL_GetRxBroadCastPacket(baseAddr);
    rxStat->rxMulticastPackets = ENET_HAL_GetRxMultiCastPacket(baseAddr);
    rxStat->rxUnderSizeGoodPackets = ENET_HAL_GetRxUnderSizePacket(baseAddr);
    rxStat->rxUnderSizeBadPackets = ENET_HAL_GetRxFragPacket(baseAddr);
    rxStat->rxOverSizeGoodPackets =  ENET_HAL_GetRxOverSizePacket(baseAddr);
    rxStat->rxOverSizeBadPackets = ENET_HAL_GetRxJabPacket(baseAddr);
    rxStat->rxOctets = ENET_HAL_GetRxOctets(baseAddr);
    rxStat->rxByte1024to2047Packets = ENET_HAL_GetRxByte1024to2047Packet(baseAddr);
    rxStat->rxByte128to255Packets = ENET_HAL_GetRxByte128to255Packet(baseAddr);
    rxStat->rxByte256to511Packets = ENET_HAL_GetRxByte256to511Packet(baseAddr);
    rxStat->rxByte64Packets = ENET_HAL_GetRxByte64Packet(baseAddr);
    rxStat->rxByte65to127Packets = ENET_HAL_GetRxByte65to127Packet(baseAddr);
    rxStat->rxByteOver2048Packets = ENET_HAL_GetRxOverByte2048Packet(baseAddr);
    rxStat->rxCrcAlignErrorPackets = ENET_HAL_GetRxCrcAlignErrorPacket(baseAddr);
    rxStat->ieeerxFrameOk = ENET_HAL_GetRxFramesOk(baseAddr);
    rxStat->ieeerxFrameCrcErr = ENET_HAL_GetRxFramesCrcError(baseAddr);
    rxStat->ieeerxFrameDrop = ENET_HAL_GetRxFramesDrop(baseAddr);
    rxStat->ieeeOctetsrxFrameOk = ENET_HAL_GetRxOtetsFramesOk(baseAddr);
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_GetMibTxStat
 * Description: Get all transmitted statistics from MIB.
 *
 *END*********************************************************************/
void ENET_HAL_GetMibTxStat(uint32_t baseAddr, enet_mib_tx_stat_t *txStat)
{
    assert(txStat);
 
    txStat->txPackets = ENET_HAL_GetTxPackets(baseAddr);
    txStat->txBroadcastPackets = ENET_HAL_GetTxBroadCastPacket(baseAddr);
    txStat->txMulticastPackets = ENET_HAL_GetTxMultiCastPacket(baseAddr);
    txStat->txUnderSizeGoodPackets = ENET_HAL_GetTxUnderSizePacket(baseAddr);
    txStat->txUnderSizeBadPackets = ENET_HAL_GetTxFragPacket(baseAddr);
    txStat->txOverSizeGoodPackets = ENET_HAL_GetTxOverSizePacket(baseAddr);
    txStat->txOverSizeBadPackets = ENET_HAL_GetTxJabPacket(baseAddr);
    txStat->txOctets = ENET_HAL_GetTxOctets(baseAddr);
    txStat->txByte1024to2047Packets = ENET_HAL_GetTxByte1024to2047Packet(baseAddr);
    txStat->txByte128to255Packets = ENET_HAL_GetTxByte128to255Packet(baseAddr);
    txStat->txByte256to511Packets = ENET_HAL_GetTxByte256to511Packet(baseAddr);
    txStat->txByte64Packets = ENET_HAL_GetTxByte64Packet(baseAddr);
    txStat->txByte65to127Packets = ENET_HAL_GetTxByte65to127Packet(baseAddr);
    txStat->txByteOver2048Packets = ENET_HAL_GetTxOverByte2048Packet(baseAddr);
    txStat->txCrcAlignErrorPackets = ENET_HAL_GetTxCrcAlignErrorPacket(baseAddr);
    txStat->ieeetxFrameOk = ENET_HAL_GetTxFramesOk(baseAddr);
    txStat->ieeetxFrameCarrSenseErr= ENET_HAL_GetTxFrameCarrSenseError(baseAddr);
    txStat->ieeetxFrameDelay = ENET_HAL_GetTxFramesDelay(baseAddr);
    txStat->ieeetxFrameLateCollison = ENET_HAL_GetTxFramesLateCollision(baseAddr);
    txStat->ieeetxFrameMultiCollison = ENET_HAL_GetTxFramesMultiCollision(baseAddr);
    txStat->ieeetxFrameOneCollision = ENET_HAL_GetTxFramesOneCollision(baseAddr);
    txStat->ieeetxFrameMacErr = ENET_HAL_GetTxFramesMacError(baseAddr);
    txStat->ieeetxFramePause = ENET_HAL_GetTxFramesPause(baseAddr);
    txStat->ieeeOctetstxFrameOk = ENET_HAL_GetTxOctetFramesOk(baseAddr);
    txStat->ieeetxFrmaeExcCollison = ENET_HAL_GetTxFramesExcessiveCollision(baseAddr);
}
/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_SetMacMode
 * Description: Configures MAC operating mode.
 * Enable sleep mode will disable normal operating mode. When enable the sleep
 * mode, the magic packet detection is also enabled so that a remote agent can 
 * wakeup the node.
 * 
 *END*********************************************************************/
void ENET_HAL_SetMacMode(uint32_t baseAddr, enet_mac_operate_mode_t mode)
{
    BW_ENET_ECR_SLEEP(baseAddr, mode);
    BW_ENET_ECR_MAGICEN(baseAddr, mode);
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_SetMacAddr
 * Description: Sets the six-byte Mac address of the ENET device.
 * 
 *END*********************************************************************/
void ENET_HAL_SetMacAddr(uint32_t baseAddr, uint8_t *hwAddr)
{
    uint32_t address;

    assert(hwAddr);	
    address = (uint32_t)(((uint32_t)hwAddr[0] << 24U)|((uint32_t)hwAddr[1] << 16U)|((uint32_t)hwAddr[2] << 8U)| (uint32_t)hwAddr[3]) ;
    HW_ENET_PALR_WR(baseAddr,address);      /* Set low physical address */
    address = (uint32_t)(((uint32_t)hwAddr[4] << 8U)|((uint32_t)hwAddr[5]));
    BW_ENET_PAUR_PADDR2(baseAddr, address); /* Set high physical address */
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_SetMulticastAddrHash
 * Description: Set multicast group address hash value to the mac register
 * To join the multicast group address.
 *END*********************************************************************/
void ENET_HAL_SetMulticastAddrHash(uint32_t baseAddr, uint32_t crcValue, enet_special_address_filter_t mode)
{
    switch (mode)
    {
        case kEnetSpecialAddressInit:           /* Clear group address register on ENET initialize */
            HW_ENET_GALR_WR(baseAddr,0);
            HW_ENET_GAUR_WR(baseAddr,0);			
            break;
        case kEnetSpecialAddressEnable:         /* Enable a multicast group address*/
            if (!((crcValue >> 31) & 1U))
            {
                HW_ENET_GALR_SET(baseAddr,(1U << ((crcValue >> 26) & kEnetHashValMask))); 
            }
            else
            {
                HW_ENET_GAUR_SET(baseAddr,(1U << ((crcValue >> 26) & kEnetHashValMask)));
            }
            break;
        case kEnetSpecialAddressDisable:       /* Disable a multicast group address*/
            if (!((crcValue >> 31) & 1U))
            {
                HW_ENET_GALR_CLR(baseAddr,(1U << ((crcValue >> 26) & kEnetHashValMask)));
            }
            else
            {
                HW_ENET_GAUR_CLR(baseAddr,(1U << ((crcValue>>26) & kEnetHashValMask))); 
            }
        break;
        default:
        break;
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_SetUnicastAddrHash 
 * Description: Set a specific unicast address hash value to the mac register
 * To receive frames with the individual destination address.  
 *END*********************************************************************/
void ENET_HAL_SetUnicastAddrHash(uint32_t baseAddr, uint32_t crcValue, enet_special_address_filter_t mode)
{
    switch (mode)
    {
        case kEnetSpecialAddressInit:         /* Clear individual address register on ENET initialize */
            HW_ENET_IALR_WR(baseAddr,0);
            HW_ENET_IAUR_WR(baseAddr,0);			
            break;
        case kEnetSpecialAddressEnable:        /* Enable a special address*/
            if (((crcValue >>31) & 1U) == 0)
            {
                HW_ENET_IALR_SET(baseAddr,(1U << ((crcValue>>26)& kEnetHashValMask))); 
            }
            else
            {
                HW_ENET_IAUR_SET(baseAddr,(1U << ((crcValue>>26)& kEnetHashValMask)));
            }
            break;
        case kEnetSpecialAddressDisable:     /* Disable a special address*/
            if (((crcValue >>31) & 1U) == 0)
            {
                HW_ENET_IALR_CLR(baseAddr,(1U << ((crcValue>>26)& kEnetHashValMask)));
            }
            else
            {
                HW_ENET_IAUR_CLR(baseAddr,(1U << ((crcValue>>26)& kEnetHashValMask))); 
            }
            break;
        default:
            break;
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_SetTxFifo
 * Description: Configure ENET transmit FIFO.  
 *END*********************************************************************/
void ENET_HAL_SetTxFifo(uint32_t baseAddr, enet_config_tx_fifo_t *thresholdCfg)
{
    assert(thresholdCfg);
    assert(thresholdCfg->txFifoWrite <= BM_ENET_TFWR_TFWR);
    assert(thresholdCfg->txAlmostEmpty >= kEnetMinFifoAlmostEmpty);
    assert(thresholdCfg->txAlmostFull >= kEnetMinTxFifoAlmostFull);
    
    BW_ENET_TFWR_STRFWD(baseAddr, thresholdCfg->isStoreForwardEnabled);   /* Set store and forward mode*/
    if(!thresholdCfg->isStoreForwardEnabled)
    {
        BW_ENET_TFWR_TFWR(baseAddr, thresholdCfg->txFifoWrite);  /* Set transmit FIFO write bytes*/
    }
    BW_ENET_TSEM_TX_SECTION_EMPTY(baseAddr,thresholdCfg->txEmpty);       /* Set transmit FIFO empty threshold*/
    BW_ENET_TAEM_TX_ALMOST_EMPTY(baseAddr,thresholdCfg->txAlmostEmpty);  /* Set transmit FIFO almost empty threshold*/
    BW_ENET_TAFL_TX_ALMOST_FULL(baseAddr,thresholdCfg->txAlmostFull);    /* Set transmit FIFO almost full threshold*/
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_SetRxFifo
 * Description: Configure ENET receive FIFO.  
 *END*********************************************************************/
void ENET_HAL_SetRxFifo(uint32_t baseAddr,enet_config_rx_fifo_t *thresholdCfg )
{
    assert(thresholdCfg);
    assert(thresholdCfg->rxAlmostEmpty >= kEnetMinFifoAlmostEmpty);
    assert(thresholdCfg->rxAlmostFull >= kEnetMinFifoAlmostEmpty);
    
    if(thresholdCfg->rxFull > 0)
    {
       assert(thresholdCfg->rxFull > thresholdCfg->rxAlmostEmpty);
    }

    BW_ENET_RSFL_RX_SECTION_FULL(baseAddr,thresholdCfg->rxFull);        /* Set receive FIFO full threshold*/
    BW_ENET_RSEM_RX_SECTION_EMPTY(baseAddr,thresholdCfg->rxEmpty);      /* Set receive FIFO empty threshold*/
    BW_ENET_RAEM_RX_ALMOST_EMPTY(baseAddr,thresholdCfg->rxAlmostEmpty); /* Set receive FIFO almost empty threshold*/
    BW_ENET_RAFL_RX_ALMOST_FULL(baseAddr,thresholdCfg->rxAlmostFull);   /* Set receive FIFO almost full threshold*/    
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_InitRxBuffDescriptors
 * Description: Initialize an ENET receive buffer descriptor. The buffer is
 * is the data buffer address, this address must always be evenly divisible by 16.
 *END*********************************************************************/
void ENET_HAL_InitRxBuffDescriptors(volatile enet_bd_struct_t *rxBds, uint8_t *rxBuff, uint32_t rxbdNum, uint32_t rxBuffSizeAlign)
{
    uint16_t count;
    volatile enet_bd_struct_t *curBd;
    assert(rxBds);
    assert(rxBuff);

    curBd = rxBds;
    for(count = 0; count < rxbdNum; count++)
    {
        curBd->buffer = (uint8_t *)NTOHL((uint32_t)&rxBuff[count * rxBuffSizeAlign]); /* Set data buffer address */
        curBd->length = 0;    /* Initialize data length*/
    
        /*The last buffer descriptor should be set with the wrap flag*/
        if (count == rxbdNum - 1)
        {    
            curBd->control |= kEnetRxBdWrap; 
        }
        curBd->control |= kEnetRxBdEmpty;   /* Initialize bd with empty bit*/
        curBd->controlExtend1 |= kEnetRxBdIntrrupt;/* Enable receive interrupt*/
        curBd ++;
    }

}
/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_InitTxBuffDescriptors
 * Description: Initialize an ENET transmit buffer descriptor.
 *END*********************************************************************/

void ENET_HAL_InitTxBuffDescriptors(volatile enet_bd_struct_t *txBds, uint8_t *txBuff, uint32_t txbdNum, uint32_t txBuffSizeAlign)
{
    uint32_t count;
    volatile enet_bd_struct_t *curBd;
    assert(txBds);
    assert(txBuff);

    curBd = txBds;
    for(count = 0; count < txbdNum; count++)
    {
        curBd->buffer = (uint8_t *)NTOHL((uint32_t)&txBuff[count * txBuffSizeAlign]); /* Set data buffer address */
        curBd->length = 0; /* Set data length*/
    
        /*The last buffer descriptor should be set with the wrap flag*/
        if (count == txbdNum - 1)
        {
            curBd->control |= kEnetTxBdWrap;
        } 

        curBd ++;
    }
}


/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_UpdateRxBuffDescriptor
 * Description: Update ENET receive buffer descriptors. The data is the 
 * buffer address and this address must always be evenly divisible by 16.
 *END*********************************************************************/
void ENET_HAL_UpdateRxBuffDescriptor(volatile enet_bd_struct_t *rxBds, uint8_t *data, bool isbufferUpdate)
{
    assert(rxBds);

    if (isbufferUpdate)
    {
        assert(data);
        rxBds->buffer = (uint8_t *)HTONL((uint32_t)data);
    }
    rxBds->control &= kEnetRxBdWrap;  /* Clear status*/
    rxBds->control |= kEnetRxBdEmpty;   /* Set rx bd empty*/
    rxBds->controlExtend1 |= kEnetRxBdIntrrupt;/* Enable interrupt*/
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_UpdateTxBuffDescriptor
 * Description: Update ENET transmit buffer descriptors. The buffer is the 
 * data buffer address and this address must be evenly divided by 16.
 *END*********************************************************************/
void ENET_HAL_UpdateTxBuffDescriptor(volatile enet_bd_struct_t *txBds, /*uint8_t *packet,*/
               uint16_t length, bool isTxtsCfged, bool isTxCrcEnable, bool isLastOne)
{
    assert(txBds);
    
    txBds->length = HTONS(length); /* Set data length*/
//    txBds->buffer = (uint8_t *)HTONL((uint32_t)packet);
    if(isLastOne)
    {
        txBds->control |= kEnetTxBdLast;
        if(isTxCrcEnable)
        {
            txBds->control |= kEnetTxBdTransmitCrc;  /* set control */   
        }
    }
    txBds->control |= kEnetTxBdReady;

    if (isTxtsCfged)
    {
         /* Set receive and timestamp interrupt*/
        txBds->controlExtend1 |= (kEnetTxBdTxInterrupt | kEnetTxBdTimeStamp);	
    }
    else
    {
        /* Set receive interrupt*/
        txBds->controlExtend1 |= kEnetTxBdTxInterrupt;	
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_GetBuffDescripLen
 * Description: Get the data length of buffer descriptors.
 *END*********************************************************************/
uint16_t ENET_HAL_GetBuffDescripLen(volatile enet_bd_struct_t *curBd)
{
    uint16_t length;
    assert(curBd);
    length = curBd->length;
    return NTOHS(length);
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_GetBuffDescripData
 * Description: Get the buffer address of buffer descriptors.
 *END*********************************************************************/
uint8_t* ENET_HAL_GetBuffDescripData(volatile enet_bd_struct_t *curBd)
{
    uint32_t buffer;
    assert(curBd);

    buffer = (uint32_t)(curBd->buffer);
    return  (uint8_t *)NTOHL(buffer);
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_GetBuffDescripTs
 * Description: Get the timestamp of buffer descriptors.
 *END*********************************************************************/
uint32_t ENET_HAL_GetBuffDescripTs(volatile enet_bd_struct_t *curBd)
{
    uint32_t timestamp; 
    assert(curBd);
   
    timestamp = curBd->timestamp;
    return NTOHL(timestamp);
}	

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_SetRMIIMode
 * Description: Configure (R)MII mode.
 *END*********************************************************************/
void ENET_HAL_SetRMIIMode(uint32_t baseAddr, enet_config_rmii_t *rmiiCfgPtr)
{
    assert(rmiiCfgPtr);

    BW_ENET_RCR_MII_MODE(baseAddr,1);             /* Set mii mode */
    BW_ENET_RCR_RMII_MODE(baseAddr,rmiiCfgPtr->mode);
    BW_ENET_RCR_RMII_10T(baseAddr,rmiiCfgPtr->speed);         /* Set speed mode	*/
    BW_ENET_TCR_FDEN(baseAddr,rmiiCfgPtr->duplex);            /* Set duplex mode*/
    if ((!rmiiCfgPtr->duplex) && (rmiiCfgPtr->isRxOnTxDisabled))
    {
        BW_ENET_RCR_DRT(baseAddr,1);              /* Disable receive on transmit*/
    }

    if (rmiiCfgPtr->mode == kEnetCfgMii)                 /* Set internal loop only for mii mode*/
    {             
        BW_ENET_RCR_LOOP(baseAddr,rmiiCfgPtr->isLoopEnabled);
    }
    else
    {
        BW_ENET_RCR_LOOP(baseAddr, 0);    /* Clear internal loop for rmii mode*/
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_SetSMIWrite
 * Description: Set SMI(serial Management interface) command.
 *END*********************************************************************/
void ENET_HAL_SetSMIWrite(uint32_t baseAddr, uint32_t phyAddr, uint32_t phyReg, enet_mii_write_t operation, uint32_t data)
{
    uint32_t mmfrValue = 0 ;

    mmfrValue = BF_ENET_MMFR_ST(1)| BF_ENET_MMFR_OP(operation)| BF_ENET_MMFR_PA(phyAddr) | BF_ENET_MMFR_RA(phyReg)| BF_ENET_MMFR_TA(2) | (data&0xFFFF); /* mii command*/
    HW_ENET_MMFR_WR(baseAddr,mmfrValue);
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_SetSMIRead
 * Description: Set SMI(serial Management interface) command.
 *END*********************************************************************/
void ENET_HAL_SetSMIRead(uint32_t baseAddr, uint32_t phyAddr, uint32_t phyReg, enet_mii_read_t operation)
{
    uint32_t mmfrValue = 0 ;

    mmfrValue = BF_ENET_MMFR_ST(1)| BF_ENET_MMFR_OP(operation)| BF_ENET_MMFR_PA(phyAddr) | BF_ENET_MMFR_RA(phyReg)| BF_ENET_MMFR_TA(2); /* mii command*/
    HW_ENET_MMFR_WR(baseAddr,mmfrValue);
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_SetIntMode
 * Description: Enable or disable different Ethernet interrupts.
 *END*********************************************************************/
void ENET_HAL_SetIntMode(uint32_t baseAddr, enet_interrupt_request_t source, bool enable)
{
    if (enable)
    {
        HW_ENET_EIMR_SET(baseAddr, (uint32_t)source);                     /* Enable interrupt */
    }
    else
    {
        HW_ENET_EIMR_CLR(baseAddr, (uint32_t)source);                     /* Disable interrupt*/
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_SetTxAccelerator
 * Description: Configure Ethernet transmit accelerator features.
 *END*********************************************************************/
void ENET_HAL_SetTxAccelerator(uint32_t baseAddr, enet_config_tx_accelerator_t *txCfgPtr)
{
    assert(txCfgPtr);

    HW_ENET_TACC_WR(baseAddr,0);                                    /* Clear all*/
    BW_ENET_TACC_IPCHK(baseAddr,txCfgPtr->isIpCheckEnabled);        /* Insert ipheader checksum */
    BW_ENET_TACC_PROCHK(baseAddr,txCfgPtr->isProtocolCheckEnabled); /* Insert protocol checksum*/
    BW_ENET_TACC_SHIFT16(baseAddr,txCfgPtr->isShift16Enabled);      /* Set tx fifo shift-16*/
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_SetRxAccelerator
 * Description: Configure Ethernet receive accelerator features.
 *END*********************************************************************/
void ENET_HAL_SetRxAccelerator(uint32_t baseAddr, enet_config_rx_accelerator_t *rxCfgPtr)
{
    assert(rxCfgPtr);

    HW_ENET_RACC_WR(baseAddr,0);                                         /* Clear all*/
    BW_ENET_RACC_IPDIS(baseAddr,rxCfgPtr->isIpcheckEnabled);             /* Set ipchecksum field*/
    BW_ENET_RACC_PRODIS(baseAddr,rxCfgPtr->isProtocolCheckEnabled);      /* Set protocol field*/
    BW_ENET_RACC_LINEDIS(baseAddr,rxCfgPtr->isMacCheckEnabled);         /* Set maccheck field*/
    BW_ENET_RACC_SHIFT16(baseAddr,rxCfgPtr->isShift16Enabled);           /* Set rx fifo shift field*/
    BW_ENET_RACC_PADREM(baseAddr,rxCfgPtr->isPadRemoveEnabled);          /* Set rx padding remove field*/
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_HAL_Set1588Timer
 * Description: Initialize Ethernet ptp timer.
 *
 *END*********************************************************************/
void ENET_HAL_Set1588Timer(uint32_t baseAddr,enet_config_ptp_timer_t *ptpCfgPtr)
{
    assert(ptpCfgPtr);

    BW_ENET_ATINC_INC(baseAddr, ptpCfgPtr->clockIncease);   /* Set increase value for ptp timer*/
    HW_ENET_ATPER_WR(baseAddr, ptpCfgPtr->period);         /* Set wrap time for ptp timer*/
    /* set periodical event and the event signal output assertion*/
    BW_ENET_ATCR_PEREN(baseAddr, 1);
    BW_ENET_ATCR_PINPER(baseAddr, 1);
    /* Set ptp timer slave/master mode*/
    BW_ENET_ATCR_SLAVE(baseAddr, ptpCfgPtr->isSlaveEnabled); 
}


/*******************************************************************************
 * EOF
 ******************************************************************************/

