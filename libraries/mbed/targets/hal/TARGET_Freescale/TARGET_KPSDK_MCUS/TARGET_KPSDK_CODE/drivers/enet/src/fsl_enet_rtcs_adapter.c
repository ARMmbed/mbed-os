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
#include "fsl_enet_driver.h"
#include "fsl_enet_hal.h"
#include "fsl_phy_driver.h"
#include "fsl_enet_rtcs_adapter.h"
#include "fsl_clock_manager.h"
#include "fsl_interrupt_manager.h"
#include "fsl_os_abstraction.h"
#include <string.h>

/*******************************************************************************
 * Variables
 ******************************************************************************/
static enet_dev_if_t enetDevIf[HW_ENET_INSTANCE_COUNT];
enet_config_rmii_t rmiiCfg = {kEnetCfgRmii, kEnetCfgSpeed100M, kEnetCfgFullDuplex, false, false};
enet_mac_config_t g_enetMacCfg[HW_ENET_INSTANCE_COUNT] =
{
{
    kEnetMacNormalMode,  /*!< ENET normal mode*/
    kEnetMaxFrameSize,  /*!< ENET receive maximun frame length*/
    kEnetDefaultTruncLen,  /*!< ENET default frame truncation length*/
    kEnetDefaultIpg,       /*!< ENET default transmit inter packet gap*/
    0,                 /*!< ENET Pause duration*/
    NULL,      /*!< ENET mac address*/
    &rmiiCfg,  /*!< ENET rmii interface*/
    kEnetMdioHoldOneClkCycle,
    /*!< enet mac control flag recommended to use enet_mac_control_flag_t
      it is special control for loop mode, sleep mode, crc forward/terminate etc*/
    kEnetRxCrcFwdEnable | kEnetTxCrcBdEnable | kEnetMacEnhancedEnable,
    NULL,     /*!< ENET rxaccelerator config*/
    NULL,     /*!< ENET txaccelerator config*/
    {0, kEnetMinFifoAlmostEmpty, 0, kEnetMinFifoAlmostEmpty}, /*!< ENET Rx FIFO threshold*/
    {0, 0, 0, kEnetMinFifoAlmostEmpty, kEnetDefaultTxFifoAlmostFull}, /*!< ENET Tx FIFO threshold*/
#if FSL_FEATURE_ENET_SUPPORT_PTP
    false,
#endif
},
};

const enet_phy_config_t g_enetPhyCfg[HW_ENET_INSTANCE_COUNT] =
{{false, 0, false }};

/* ENET error describe list*/
static const char * ENET_errlist[kStatus_ENET_AlreadyAddedMulticast - kStatus_ENET_InvalidInput + 1] = {
    "Invaild ENET input parameter"          /* kStatus_ENET_InvalidInput*/
    "Invalid ENET device",                  /* kStatus_ENET_InvalidDevice */
    "Memory allocate failure",           /* kStatus_ENET_MemoryAllocateFail */
    "Get clock frequency failure",   /* kStatus_ENET_GetClockFreqFail */
    "ENET device already initialized",        /* kStatus_ENET_Initialized */
    "Open ENET device",      /* kStatus_ENET_Open */
    "Close ENET device failure",      /* kStatus_ENET_Close*/
    "NULL L2 PTP buffer queue pointer",      /* kStatus_ENET_Layer2QueueNull*/
    "Layer2 packet length over large",       /* kStatus_ENET_Layer2OverLarge*/
    "Layer2 packet buffer full",         /* kStatus_ENET_Layer2BufferFull*/
    "Layer2 packet error type",     /* kStatus_ENET_Layer2TypeError */
    "PTP ring buffer full",        /* kStatus_ENET_PtpringBufferFull*/
    "PTP ring buffer empty",         /* kStatus_ENET_PtpringBufferEmpty */
    "MII uninitialized",             /* kStatus_ENET_Miiuninitialized*/
    "Receive buffer descriptor invalid",   /* kStatus_ENET_RxbdInvalid */
    "Receive buffer descriptor empty",    /* kStatus_ENET_RxbdEmpty */
    "Receive buffer descriptor truncate", /* kStatus_ENET_RxbdTrunc */
    "Receive buffer descriptor error",   /* kStatus_ENET_RxbdError  */
    "Receive buffer descriptor full",    /* kStatus_ENET_RxBdFull  */
    "Small receive buffer size",           /* kStatus_ENET_SmallBdSize*/
    "Receive large buffer full",               /* kStatus_ENET_LargeBufferFull */
    "Transmit large packet",                  /* kStatus_ENET_TxLarge */
    "Transmit buffer descriptor full",        /* kStatus_ENET_TxbdFull*/
    "Transmit buffer descriptor Null",     /* kStatus_ENET_TxbdNull*/
    "Transmit data buffer Null",     /* kStatus_ENET_TxBufferNull*/
    "No more receive buffer left", /* kStatus_ENET_NoRxBufferLeft */
    "Invalid ENET PTP IOCTL command",   /* kStatus_ENET_UnknownCommand*/
    "ENET Timeout",         /* kStatus_ENET_TimeOut*/
    "Null multicast group pointer",         /* kStatus_ENET_MulticastPointerNull */
    "No multicast group address",  /* kStatus_ENET_NoMulticastAdd */
    "Have Already added to multicast group", /* kStatus_ENET_AlreadyAddedMulticast */
};

uint8_t *txBdPtr[HW_ENET_INSTANCE_COUNT], *rxBdPtr[HW_ENET_INSTANCE_COUNT];
uint8_t *txBuffer[HW_ENET_INSTANCE_COUNT], *rxBuffer[HW_ENET_INSTANCE_COUNT], *rxRtcsBuffer[HW_ENET_INSTANCE_COUNT];
uint8_t *rxExtBuffer[HW_ENET_INSTANCE_COUNT];
pcb_queue  packbuffer[HW_ENET_INSTANCE_COUNT];
PCB2 *pcbPtr[HW_ENET_INSTANCE_COUNT];
uint8_t *dataBuffQue;

#if FSL_FEATURE_ENET_SUPPORT_PTP
enet_mac_ptp_ts_data_t ptpTsRxData[ENET_PTP_RXTS_RING_LEN];
enet_mac_ptp_ts_data_t ptpTsTxData[ENET_PTP_TXTS_RING_LEN];
#endif

#if !ENET_RECEIVE_ALL_INTERRUPT
bool frameIsCollected = true;
#else
bool frameIsCollected = false;
#endif
#if BSPCFG_ENABLE_ENET_STATS
ENET_STATS enetStats;
#endif
OSA_TASK_DEFINE(ENET_receive, ENET_TASK_STACK_SIZE);

/*******************************************************************************
 * Code
 ******************************************************************************/
extern void IPE_recv_IP(PCB_PTR pcb, void *handle);
extern void IPE_recv_ARP(PCB_PTR pcb, void *handle);
#if RTCSCFG_ENABLE_IP6
extern void IP6E_recv_IP(PCB_PTR pcb, void *handle);
#endif

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_buffer_init
 * Return Value: The execution status.
 * Description: Initialize enet mac buffer.
 *
 *END*********************************************************************/
uint32_t ENET_buffer_init(enet_dev_if_t * enetIfPtr, enet_buff_config_t *buffCfgPtr)
{
    uint32_t rxBufferSizeAlign, txBufferSizeAlign, rxLargeBufferSizeAlign;
    uint8_t count, *txBufferAlign, *rxBufferAlign, *rxLargeBufferAlign, *rxExtBufferAlign;
    volatile enet_bd_struct_t *txBdPtrAlign, *rxBdPtrAlign;
    PCB2 *pcbbuffer;

    /* Check input parameter*/
    if((!enetIfPtr) || (!buffCfgPtr))
    {
        return kStatus_ENET_InvalidInput;
    }

    /* Allocate ENET receive buffer descriptors*/
    txBdPtr[enetIfPtr->deviceNumber] = (uint8_t *)OSA_MemAllocZero(ENET_TXBD_NUM * sizeof(enet_bd_struct_t) + ENET_BD_ALIGNMENT);
    if (!txBdPtr[enetIfPtr->deviceNumber])
    {
        return kStatus_ENET_MemoryAllocateFail;
    }
   txBdPtrAlign = (volatile enet_bd_struct_t *)ENET_ALIGN((uint32_t)txBdPtr[enetIfPtr->deviceNumber], ENET_BD_ALIGNMENT);

    rxBdPtr[enetIfPtr->deviceNumber] = (uint8_t *)OSA_MemAllocZero(ENET_RXBD_NUM * sizeof(enet_bd_struct_t) + ENET_BD_ALIGNMENT);
    if(!rxBdPtr[enetIfPtr->deviceNumber])
    {
         OSA_MemFree(txBdPtr[enetIfPtr->deviceNumber]);
         return kStatus_ENET_MemoryAllocateFail;
    }
    rxBdPtrAlign = (volatile enet_bd_struct_t *)ENET_ALIGN((uint32_t)rxBdPtr[enetIfPtr->deviceNumber], ENET_BD_ALIGNMENT);

    /* Allocate the transmit and receive date buffers*/
    rxBufferSizeAlign = ENET_RXBuffSizeAlign(ENET_RXBuff_SIZE);
    rxBuffer[enetIfPtr->deviceNumber] = (uint8_t *)OSA_MemAllocZero(ENET_RXBD_NUM * rxBufferSizeAlign  + ENET_RX_BUFFER_ALIGNMENT);
    if (!rxBuffer[enetIfPtr->deviceNumber])
    {
        OSA_MemFree(txBdPtr[enetIfPtr->deviceNumber]);
        OSA_MemFree(rxBdPtr[enetIfPtr->deviceNumber]);
        return kStatus_ENET_MemoryAllocateFail;
    }
    rxBufferAlign = (uint8_t *)ENET_ALIGN((uint32_t)rxBuffer[enetIfPtr->deviceNumber], ENET_RX_BUFFER_ALIGNMENT);

    txBufferSizeAlign = ENET_RXBuffSizeAlign(ENET_TXBuff_SIZE);
    txBuffer[enetIfPtr->deviceNumber] = OSA_MemAllocZero(ENET_TXBD_NUM * txBufferSizeAlign + ENET_TX_BUFFER_ALIGNMENT);
    if (!txBuffer[enetIfPtr->deviceNumber])
    {
        OSA_MemFree(txBdPtr[enetIfPtr->deviceNumber]);
        OSA_MemFree(rxBdPtr[enetIfPtr->deviceNumber]);
        OSA_MemFree(rxBuffer[enetIfPtr->deviceNumber]);
        return kStatus_ENET_MemoryAllocateFail;
    }
    txBufferAlign = (uint8_t *)ENET_ALIGN((uint32_t)txBuffer[enetIfPtr->deviceNumber], ENET_TX_BUFFER_ALIGNMENT);

    /*Initialize the data buffer queue for rtcs */
    rxLargeBufferSizeAlign = ENET_ALIGN(kEnetMaxFrameVlanSize, ENET_RX_BUFFER_ALIGNMENT);
    rxRtcsBuffer[enetIfPtr->deviceNumber] = OSA_MemAllocZero(ENET_RXRTCSBUFF_NUM * rxLargeBufferSizeAlign + ENET_RX_BUFFER_ALIGNMENT);
    if (!rxRtcsBuffer[enetIfPtr->deviceNumber])
    {
        OSA_MemFree(txBdPtr[enetIfPtr->deviceNumber]);
        OSA_MemFree(rxBdPtr[enetIfPtr->deviceNumber]);
        OSA_MemFree(rxBuffer[enetIfPtr->deviceNumber]);
        OSA_MemFree(txBuffer[enetIfPtr->deviceNumber]);
        return kStatus_ENET_MemoryAllocateFail;
    }
    /* Store for buffer free*/
    rxLargeBufferAlign = (uint8_t *)ENET_ALIGN((uint32_t)rxRtcsBuffer[enetIfPtr->deviceNumber], ENET_RX_BUFFER_ALIGNMENT);
    dataBuffQue = NULL;
    for (count = 0; count < ENET_RXRTCSBUFF_NUM; count++)
    {
        enet_mac_enqueue_buffer((void **)&dataBuffQue, rxLargeBufferAlign);
        rxLargeBufferAlign += rxLargeBufferSizeAlign;
    }

    /* Initialize PCB buffer*/
    pcbPtr[enetIfPtr->deviceNumber] = (PCB2 *)OSA_MemAllocZero(PCB_MINIMUM_SIZE * ENET_PCB_NUM);
    pcbbuffer = pcbPtr[enetIfPtr->deviceNumber];
    for (count = 0; count < ENET_PCB_NUM; count++)
    {
        QUEUEADD(packbuffer[enetIfPtr->deviceNumber].pcbHead, packbuffer[enetIfPtr->deviceNumber].pcbTail, (PCB *)pcbbuffer);
        pcbbuffer->FRAG[1].LENGTH = 0;
        pcbbuffer->FRAG[1].FRAGMENT = NULL;
        pcbbuffer->FREE = ENET_free;
        pcbbuffer ++;
    }

#if FSL_FEATURE_ENET_SUPPORT_PTP
    buffCfgPtr->ptpTsRxDataPtr = &ptpTsRxData[0];
    buffCfgPtr->ptpTsRxBuffNum = ENET_PTP_RXTS_RING_LEN;
    buffCfgPtr->ptpTsTxDataPtr = &ptpTsTxData[0];
    buffCfgPtr->ptpTsTxBuffNum = ENET_PTP_TXTS_RING_LEN;
#endif
#if !ENET_RECEIVE_ALL_INTERRUPT
    rxExtBuffer[enetIfPtr->deviceNumber] = (uint8_t *)OSA_MemAllocZero(ENET_EXTRXBD_NUM * rxBufferSizeAlign  + ENET_RX_BUFFER_ALIGNMENT);
    if (!rxExtBuffer[enetIfPtr->deviceNumber])
    {
        OSA_MemFree(txBdPtr[enetIfPtr->deviceNumber]);
        OSA_MemFree(rxBdPtr[enetIfPtr->deviceNumber]);
        OSA_MemFree(rxBuffer[enetIfPtr->deviceNumber]);
        OSA_MemFree(txBuffer[enetIfPtr->deviceNumber]);
        OSA_MemFree(rxRtcsBuffer[enetIfPtr->deviceNumber]);
        return kStatus_ENET_MemoryAllocateFail;
    }
    rxExtBufferAlign = (uint8_t *)ENET_ALIGN((uint32_t)rxExtBuffer[enetIfPtr->deviceNumber], ENET_RX_BUFFER_ALIGNMENT);
    buffCfgPtr->extRxBuffQue = rxExtBufferAlign;
    buffCfgPtr->extRxBuffNum = ENET_EXTRXBD_NUM;
#else
    buffCfgPtr->extRxBuffQue = NULL;
    buffCfgPtr->extRxBuffNum = 0;
#endif

    buffCfgPtr->rxBdNumber = ENET_RXBD_NUM;
    buffCfgPtr->rxBdPtrAlign = rxBdPtrAlign;
    buffCfgPtr->rxBufferAlign = rxBufferAlign;
    buffCfgPtr->rxBuffSizeAlign = rxBufferSizeAlign;
    buffCfgPtr->txBdNumber = ENET_TXBD_NUM;
    buffCfgPtr->txBdPtrAlign = txBdPtrAlign;
    buffCfgPtr->txBufferAlign = txBufferAlign;
    buffCfgPtr->txBuffSizeAlign = txBufferSizeAlign;

    return kStatus_ENET_Success;
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_buffer_deinit
 * Return Value: The execution status.
 * Description: Initialize enet mac buffer.
 *
 *END*********************************************************************/
uint32_t ENET_buffer_deinit(enet_dev_if_t * enetIfPtr)
{
    /* Check input parameter*/
    if(!enetIfPtr)
    {
        return kStatus_ENET_InvalidInput;
    }

    /* Free allocated memory*/
    if(txBdPtr[enetIfPtr->deviceNumber])
    {
        OSA_MemFree(txBdPtr[enetIfPtr->deviceNumber]);
    }
    if(rxBdPtr[enetIfPtr->deviceNumber])
    {
        OSA_MemFree(rxBdPtr[enetIfPtr->deviceNumber]);
    }
    if(txBuffer[enetIfPtr->deviceNumber])
    {
        OSA_MemFree(txBuffer[enetIfPtr->deviceNumber]);
    }
    if(rxBuffer[enetIfPtr->deviceNumber])
    {
        OSA_MemFree(rxBuffer[enetIfPtr->deviceNumber]);
    }
    if(rxRtcsBuffer[enetIfPtr->deviceNumber])
    {
        OSA_MemFree(rxRtcsBuffer[enetIfPtr->deviceNumber]);
    }
#if !ENET_RECEIVE_ALL_INTERRUPT
    if(rxExtBuffer[enetIfPtr->deviceNumber])
    {
        OSA_MemFree(rxExtBuffer[enetIfPtr->deviceNumber]);
    }
#endif

    if(pcbPtr[enetIfPtr->deviceNumber])
    {
        OSA_MemFree(pcbPtr[enetIfPtr->deviceNumber]);
    }
    return kStatus_ENET_Success;
}


/*FUNCTION****************************************************************
 *
 * Function Name: ENET_free
 * Description: ENET packet buffer free.
 *
 *END*********************************************************************/
void ENET_free(PCB_PTR packet)
{
    enet_dev_if_t *param;
    uint8_t count;

    /* Check input Parameter*/
    if (packet == NULL)
    {
        return;
    }

    param = (enet_dev_if_t *)packet->PRIVATE;
    count = param->deviceNumber;

    if ((packet->FRAG[0].FRAGMENT != NULL) && frameIsCollected)
    {
        *(uint32_t *)packet->FRAG[0].FRAGMENT = 0;
        enet_mac_enqueue_buffer((void **)&dataBuffQue, packet->FRAG[0].FRAGMENT);

        /* Clear fragment in the packet*/
        packet->FRAG[0].FRAGMENT = NULL;
    }

    /* Add the used PCB buffer into the PCB buffer queue*/
    QUEUEADD(packbuffer[count].pcbHead, packbuffer[count].pcbTail, packet);


}

#if ENET_RECEIVE_ALL_INTERRUPT
/*FUNCTION****************************************************************
 *
 * Function Name: ENET_find_receiver
 * Description: Find ENET tcp/ip upper layer functions.
 *
 *END*********************************************************************/
uint32_t ENET_find_receiver(void *enetPtr, enet_mac_packet_buffer_t *packetBuffer)
{
    uint8_t *packet, counter;
    uint16_t type, length = 0;
    PCB *pcbPtr;
    uint16_t *typePtr;
    enet_ecb_struct_t *ecbPtr;
    bool packetInvalid = ENET_ERROR;
    enet_dev_if_t *enetDevifPtr = (enet_dev_if_t *)enetPtr;

    /* Collect the frame first*/
    if(!packetBuffer[1].length)
    {
        packet = packetBuffer[0].data;  /* the frame with only one bd */
        length = packetBuffer[0].length;
        frameIsCollected = false;
    }
    else
    {
        /* Dequeue a large  buffer */
        packet = enet_mac_dequeue_buffer((void **)&dataBuffQue);
        if(packet!=NULL)
        {
            for(counter = 0; packetBuffer[counter].next != NULL ; counter ++)
            {
                memcpy(packet + length, packetBuffer[counter].data, packetBuffer[counter].length);
                length += packetBuffer[counter].length;
            }
        }
        else
        {
#if ENET_ENABLE_DETAIL_STATS
            enetDevifPtr->stats.statsRxDiscard ++;
#endif
            return kStatus_ENET_LargeBufferFull;
        }
        frameIsCollected = true;
    }

    /* Process the received frame*/
    typePtr = &((enet_ethernet_header_t *)packet)->type;
    type = RTCS_NTOHS((*typePtr));
    if(type == ENETPROT_8021Q)
    {
        typePtr = &((enet_8021vlan_header_t *)packet)->type;
        type = RTCS_NTOHS((*typePtr));
    }
    if(type <= kEnetMaxFrameDateSize)
    {
        enet_8022_header_ptr llcPtr = (enet_8022_header_ptr)(typePtr + 2);
        type = RTCS_NTOHS(llcPtr->type);
    }
#if FSL_FEATURE_ENET_SUPPORT_PTP
    if (type == ENETPROT_ETHERNET)
    {
        ENET_DRV_Service_l2packet(enetDevifPtr, &packetBuffer[0]);
    }
#endif
    for(ecbPtr = (enet_ecb_struct_t *)enetDevifPtr->netIfPrivate; ecbPtr; ecbPtr = ecbPtr->NEXT)
    {
        if(ecbPtr->TYPE == type)
        {
            packetInvalid = ENET_OK;
            /* Collect frame to PCB structure for upper layer process*/
            QUEUEGET(packbuffer[enetDevifPtr->deviceNumber].pcbHead,packbuffer[enetDevifPtr->deviceNumber].pcbTail, pcbPtr);
            if(pcbPtr)
            {
                pcbPtr->FRAG[0].LENGTH = length;
                pcbPtr->FRAG[0].FRAGMENT = packet;
                pcbPtr->FRAG[1].LENGTH = 0;
                pcbPtr->PRIVATE = enetDevifPtr;
                ecbPtr->SERVICE(pcbPtr, ecbPtr->PRIVATE);
           }
        }
    }

    return packetInvalid;
}
#endif
/*FUNCTION****************************************************************
 *
 * Function Name: ENET_initialize
 * Return Value: The execution status.
 * Description:Initialize the ENET device.
 *
 *END*********************************************************************/
uint32_t ENET_initialize(uint32_t device, _enet_address address,uint32_t flag, _enet_handle *handle)
{
    enet_dev_if_t * enetIfPtr;
    uint32_t result;
    const enet_mac_config_t *macCfgPtr;
    enet_buff_config_t buffCfg;
    osa_status_t osaFlag;

    if (device > HW_ENET_INSTANCE_COUNT)
    {
        return kStatus_ENET_InvalidDevice;
    }

    /* Check the device status*/
    if (enetDevIf[device].isInitialized)
    {
        return kStatus_ENET_Initialized;
    }

    /* Initialize device*/
    enetIfPtr = (enet_dev_if_t *)&enetDevIf[device];
    if (HW_ENET_INSTANCE_COUNT == device)
    {
        enetIfPtr->next = NULL;
    }

    /* 	Configure strucutre*/
    enetIfPtr->deviceNumber = device;
    g_enetMacCfg[device].macAddr = address;
    macCfgPtr = &g_enetMacCfg[device];

#if ENET_RECEIVE_ALL_INTERRUPT
    enetIfPtr->enetNetifcall = ENET_find_receiver;
#endif

    /* Create sync signal*/
    OSA_MutexCreate(&enetIfPtr->enetContextSync);

    /* Initialize enet buffers*/
    result = ENET_buffer_init(enetIfPtr, &buffCfg);
    if(result != kStatus_ENET_Success)
    {
        return result;
    }
    /* Initialize ENET device*/
    result = ENET_DRV_Init(enetIfPtr, macCfgPtr, &buffCfg);
    if (result == kStatus_ENET_Success)
    {
        /* Initialize PHY*/
        if(g_enetPhyCfg[device].isAutodiscoverEnabled)
        {
            uint32_t phyAddr;
            result = PHY_DRV_Autodiscover(device, &phyAddr);
            if(result != kStatus_ENET_Success)
            return result;
            enetIfPtr->phyAddr = phyAddr;
        }
        else
        {
            enetIfPtr->phyAddr = g_enetPhyCfg[device].phyAddr;
        }

        PHY_DRV_Init(device, enetIfPtr->phyAddr, g_enetPhyCfg[device].isLoopEnabled);

        *handle = enetIfPtr;
        enetIfPtr->isInitialized = true;

#if !ENET_RECEIVE_ALL_INTERRUPT
    osaFlag = OSA_EventCreate(&enetIfPtr->enetReceiveSync, kEventAutoClear);
    if(osaFlag != kStatus_OSA_Success)
    {
        return osaFlag;
    }
    /* Create receive task*/
    osaFlag = OSA_TaskCreate(ENET_receive, "receive", ENET_TASK_STACK_SIZE, ENET_receive_stack, ENET_RECEIVE_TASK_PRIO, (task_param_t)enetIfPtr, false, &ENET_receive_task_handler);
    if(osaFlag != kStatus_OSA_Success)
    {
        return osaFlag;
    }
#endif

        return ENET_OK;
    }
    else
    {
        ENET_DRV_Deinit(enetIfPtr);
        ENET_buffer_deinit(enetIfPtr);
        OSA_MutexDestroy(&enetIfPtr->enetContextSync);
#if !ENET_RECEIVE_ALL_INTERRUPT
        OSA_TaskDestroy(ENET_receive_task_handler);
        OSA_EventDestroy(&enetIfPtr->enetReceiveSync);
#endif
        *handle = NULL;
        return ENET_ERROR;
    }

}
#if !ENET_RECEIVE_ALL_INTERRUPT
/*FUNCTION****************************************************************
 *
 * Function Name: ENET_open
 * Return Value: The execution status.
 * Description: Open the ENET device, This interface is used to add the private
 * address to the ENET device structure.
 *
 *END*********************************************************************/
uint32_t ENET_open(_enet_handle  handle, uint16_t type, void (* service)(PCB_PTR, void *), void *private)
{
    enet_dev_if_t * enetIfPtr;

    /* Check input parameter*/
    if ((!handle) || (!private))
    {
        return kStatus_ENET_InvalidInput;
    }

    enetIfPtr = (enet_dev_if_t *)handle;
    OSA_MutexLock(&enetIfPtr->enetContextSync, OSA_WAIT_FOREVER);

    /*add the upper layer netiIF structure into the device structure*/
    enetIfPtr->netIfPrivate = private;
    OSA_MutexUnlock(&enetIfPtr->enetContextSync);

    return ENET_OK;
}
#else
/*FUNCTION****************************************************************
 *
 * Function Name: ENET_open
 * Return Value: The execution status.
 * Description: Open the ENET device, This interface is used to add the private
 * address to the enet device structure.
 *
 *END*********************************************************************/
uint32_t ENET_open(_enet_handle  handle, uint16_t type, void (* service)(PCB_PTR, void *), void *private)
{
    enet_dev_if_t * enetIfPtr;
    enet_ecb_struct_t *ecbPtr, **searchPtr;

    /* Check input parameter*/
    if ((!handle) || (!private))
    {
        return kStatus_ENET_InvalidInput;
    }

    enetIfPtr = (enet_dev_if_t *)handle;
    OSA_MutexLock(&enetIfPtr->enetContextSync, OSA_WAIT_FOREVER);
    for (searchPtr = (enet_ecb_struct_t **)(&enetIfPtr->netIfPrivate); *searchPtr; searchPtr = &(*searchPtr)->NEXT)
    {
        if ((*searchPtr)->TYPE == type)
        {
            OSA_MutexUnlock(&enetIfPtr->enetContextSync);
            return kStatus_ENET_Open;
        }
    }

    ecbPtr = (enet_ecb_struct_t *)OSA_MemAllocZero(sizeof(enet_ecb_struct_t));
    if (!ecbPtr)
    {
        OSA_MutexUnlock(&enetIfPtr->enetContextSync);
        return kStatus_ENET_MemoryAllocateFail;
    }
    ecbPtr->TYPE = type;
    ecbPtr->SERVICE = service;
    ecbPtr->PRIVATE = private;
    ecbPtr->NEXT = NULL;
    *searchPtr = ecbPtr;

    OSA_MutexUnlock(&enetIfPtr->enetContextSync);

    return ENET_OK;
}
#endif
/*FUNCTION****************************************************************
 *
 * Function Name: ENET_shutdown
 * Return Value: The execution status.
 * Description: Stop the ENET device.
 *
 *END*********************************************************************/
uint32_t ENET_shutdown(_enet_handle handle)
{
    enet_dev_if_t * enetIfPtr;
    uint32_t result;

    /* Check the input parameter*/
    if (!handle)
    {
        return ENETERR_INVALID_DEVICE;
    }

    enetIfPtr = (enet_dev_if_t *)handle;

#if ENET_RECEIVE_ALL_INTERRUPT
    /* Make sure upper layers have closed the device*/
    if (enetIfPtr->netIfPrivate)
    {
        return ENETERR_INVALID_DEVICE;
    }
#endif
    /* Close the ENET device*/
    result = ENET_DRV_Deinit(enetIfPtr);
    if (result == kStatus_ENET_Success)
    {
        OSA_MutexDestroy(&enetIfPtr->enetContextSync);

#if !ENET_RECEIVE_ALL_INTERRUPT
        OSA_TaskDestroy(ENET_receive_task_handler);
        OSA_EventDestroy(&enetIfPtr->enetReceiveSync);
#endif
        return ENET_ERROR;
    }

    return ENET_OK;
}

#if !ENET_RECEIVE_ALL_INTERRUPT
/*FUNCTION****************************************************************
 *
 * Function Name: ENET_receive
 * Description:  Net Receive interface.
 *
 *END*********************************************************************/
static void ENET_receive(task_param_t param)
{
    uint8_t *packet;
    uint16_t length = 0, type, counter = 0;
    uint32_t result;
    PCB *pcbPtr;
    uint16_t *typePtr;
    event_flags_t flag = 0x1, flagCheck;
    enet_mac_packet_buffer_t packetBuffer[kEnetMaxFrameBdNumbers];

    /* Init the packet buffer*/
    memset(&packetBuffer[0], 0, kEnetMaxFrameBdNumbers * sizeof(enet_mac_packet_buffer_t));
    for(counter = 0; counter < kEnetMaxFrameBdNumbers; counter++)
    {
        packetBuffer[counter].next = &packetBuffer[counter + 1];
    }
    counter = 0;

    /* Check input parameter*/
    if (!param)
    {
        return ;
    }
    enet_dev_if_t * enetIfPtr = (enet_dev_if_t *)param;

    while(1)
    {
        /* Init the packet buffer*/
        for(counter = 0; counter < kEnetMaxFrameBdNumbers; counter++)
        {
            packetBuffer[counter].data = NULL;
        }

        /* Receive frame*/
        result = ENET_DRV_ReceiveData(enetIfPtr, &packetBuffer[0]);
        if ((result == kStatus_ENET_RxbdEmpty) || (result == kStatus_ENET_InvalidInput))
        {
            OSA_EventWait(&enetIfPtr->enetReceiveSync, flag, false, OSA_WAIT_FOREVER, &flagCheck);
        }

        /* Process with the right packets*/
        if (packetBuffer[0].data != NULL)
        {
            /* Collect the frame first*/
            packet = enet_mac_dequeue_buffer((void **)&dataBuffQue);
            if (packet!=NULL)
            {
                length = 0;
                for(counter = 0; packetBuffer[counter].length != 0 ; counter ++)
                {
                    memcpy(packet + length, packetBuffer[counter].data, packetBuffer[counter].length);
                    length += packetBuffer[counter].length;
                    *(uint32_t *)(packetBuffer[counter].data) = 0;
                    enet_mac_enqueue_buffer((void **)&enetIfPtr->bdContext.extRxBuffQue, packetBuffer[counter].data);
                    packetBuffer[counter].length = 0;
                }
            }
            else
            {
#if ENET_ENABLE_DETAIL_STATS
                enetIfPtr->stats.statsRxMissed++;
#endif
            }

            /* Process the received frame*/
            typePtr = &((enet_ethernet_header_t *)packet)->type;
            type = RTCS_NTOHS((*typePtr));
            if (type == ENETPROT_8021Q)
            {
                typePtr = &((enet_8021vlan_header_t *)packet)->type;
                type = RTCS_NTOHS((*typePtr));
            }
            if (type <= kEnetMaxFrameDateSize)
            {
                enet_8022_header_ptr llcPtr = (enet_8022_header_ptr)(typePtr + 2);
                type = RTCS_NTOHS(llcPtr->type);
            }

#if FSL_FEATURE_ENET_SUPPORT_PTP
            if (type == ENETPROT_ETHERNET)
            {
                ENET_DRV_Service_l2packet(enetIfPtr, &packetBuffer[0]);
            }
#endif
            /* Collect frame to PCB structure for upper layer process*/
            QUEUEGET(packbuffer[enetIfPtr->deviceNumber].pcbHead, packbuffer[enetIfPtr->deviceNumber].pcbTail, pcbPtr);
            if (pcbPtr)
            {
                pcbPtr->FRAG[0].LENGTH = length;
                pcbPtr->FRAG[0].FRAGMENT = packet;
                pcbPtr->FRAG[1].LENGTH = 0;
                pcbPtr->PRIVATE = (void *)enetIfPtr;

                switch (type)
                {
                    case ENETPROT_IP:
                    IPE_recv_IP((PCB *)pcbPtr,enetIfPtr->netIfPrivate);
                        break;
                    case ENETPROT_ARP:
                    IPE_recv_ARP((PCB *)pcbPtr,enetIfPtr->netIfPrivate);
                        break;
#if RTCSCFG_ENABLE_IP6
                    case ENETPROT_IP6:
                    IP6E_recv_IP((PCB *)pcbPtr,enetIfPtr->netIfPrivate);
                        break;
#endif
                    default:
                    PCB_free((PCB *)pcbPtr);
                        break;
                }
            }
            else
            {
#if ENET_ENABLE_DETAIL_STATS
                enetIfPtr->stats.statsRxMissed++;
#endif
            }
        }
    }
}
#endif
/*FUNCTION****************************************************************
 *
 * Function Name: ENET_send
 * Return Value: The execution status.
 * Description:  Net send interface. this is called by tcp/ip stack.
 *
 *END*********************************************************************/
uint32_t ENET_send(_enet_handle handle, PCB_PTR packet, uint32_t type, _enet_address dest, uint32_t flags)
{
    uint8_t headerLen, *frame;
    PCB_FRAGMENT *fragPtr;
    uint16_t size = 0, lenTemp = 0, bdNumUsed = 0;
    enet_dev_if_t *enetIfPtr;
    enet_ethernet_header_t *packetPtr;
    volatile enet_bd_struct_t * curBd;
    uint32_t result, lenoffset = 0;
    /*Check out*/
    if ((!handle) || (!packet))
    {
        return  kStatus_ENET_InvalidInput;
    }

    enetIfPtr = (enet_dev_if_t *)handle;
    /* Default frame header size*/
    headerLen = kEnetEthernetHeadLen;

    /* Check the frame length*/
    for (fragPtr = packet->FRAG; fragPtr->LENGTH; fragPtr++)
    {
        size += fragPtr->LENGTH;
    }
    if (size > enetIfPtr->maxFrameSize)
    {
#if ENET_ENABLE_DETAIL_STATS
        enetIfPtr->stats.statsTxLarge++;
#endif
        return kStatus_ENET_TxLarge;
    }

    /*Add MAC hardware address*/
    packetPtr = (enet_ethernet_header_t *)packet->FRAG[0].FRAGMENT;
    htone(packetPtr->destAddr, dest);
    htone(packetPtr->sourceAddr, enetIfPtr->macAddr);
    packetPtr->type = RTCS_HTONS(type);
    if (flags & ENET_OPT_8021QTAG)
    {
        enet_8021vlan_header_t *vlanHeadPtr = (enet_8021vlan_header_t *)packetPtr;
        vlanHeadPtr->tpidtag = RTCS_HTONS(ENETPROT_8021Q);
        vlanHeadPtr->othertag = RTCS_HTONS((ENET_GETOPT_8021QPRIO(flags) << 13));
        vlanHeadPtr->type = RTCS_HTONS(type);
        headerLen = sizeof(enet_8021vlan_header_t);
        packet->FRAG[0].LENGTH = headerLen;
    }

    if (flags & ENET_OPT_8023)
    {
        enet_8022_header_ptr lcPtr = (enet_8022_header_ptr)(packetPtr->type + 2);
        packetPtr->type = RTCS_HTONS(size - headerLen);
        lcPtr->dsap[0] = 0xAA;
        lcPtr->ssap[0] = 0xAA;
        lcPtr->command[0] = 0x03;
        lcPtr->oui[0] = 0x00;
        lcPtr->oui[1] = 0x00;
        lcPtr->oui[2] = 0x00;
        lcPtr->type = RTCS_HTONS(type);
        packet->FRAG[0].LENGTH = packet->FRAG[0].LENGTH+ sizeof(enet_8022_header_t);
    }

    /* Get the current transmit data buffer in buffer descriptor */
    curBd = enetIfPtr->bdContext.txBdCurPtr;
    frame = ENET_HAL_GetBuffDescripData(curBd);

    /* Send a whole frame with a signal buffer*/
    if(size <= enetIfPtr->bdContext.txBuffSizeAlign)
    {
        bdNumUsed = 1;
        for (fragPtr = packet->FRAG; fragPtr->LENGTH; fragPtr++)
        {
            memcpy(frame + lenTemp, fragPtr->FRAGMENT, fragPtr->LENGTH);
            lenTemp += fragPtr->LENGTH;
        }

        /* Send packet to the device*/
        result = ENET_DRV_SendData(enetIfPtr, size, bdNumUsed);
     
        /* Free the PCB buffer*/
        PCB_free(packet);
        return result;
    }

    /* Copy the Ethernet header first*/
     memcpy(frame, packet->FRAG[0].FRAGMENT, packet->FRAG[0].LENGTH);

    /* Send a whole frame with multiple buffer descriptors*/
    while((size - bdNumUsed* enetIfPtr->bdContext.txBuffSizeAlign) > enetIfPtr->bdContext.txBuffSizeAlign)
    {
        if(bdNumUsed == 0)
        {
            memcpy((void *)(frame + packet->FRAG[0].LENGTH), (void *)(packet->FRAG[1].FRAGMENT), enetIfPtr->bdContext.txBuffSizeAlign - packet->FRAG[0].LENGTH);
            lenoffset += (enetIfPtr->bdContext.txBuffSizeAlign - packet->FRAG[0].LENGTH);
        }
        else
        {
            memcpy((void *)frame, (void *)(packet->FRAG[1].FRAGMENT + lenoffset), enetIfPtr->bdContext.txBuffSizeAlign);
            lenoffset += enetIfPtr->bdContext.txBuffSizeAlign;
        }

        /* Incremenet the buffer descriptor*/
        curBd = ENET_DRV_IncrTxBuffDescripIndex(enetIfPtr, curBd);
        frame = ENET_HAL_GetBuffDescripData(curBd);
        /* Increment the index and parameters*/
        bdNumUsed ++;
    }
     memcpy((void *)frame, (void *)(packet->FRAG[1].FRAGMENT + lenoffset), size - bdNumUsed * enetIfPtr->bdContext.txBuffSizeAlign);
     bdNumUsed ++;
    /* Send packet to the device*/
    result = ENET_DRV_SendData(enetIfPtr, size, bdNumUsed);

    /* Free the PCB buffer*/
    PCB_free(packet);

    return result;
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_get_address
 * Return Value: The execution status.
 * Description:Get the ENET address of an initialized device
 *
 *END*********************************************************************/
uint32_t ENET_get_address(_enet_handle handle, _enet_address address)
{
    /* Check input param*/
    if (!handle)
    {
        return kStatus_ENET_InvalidInput;
    }

    enet_dev_if_t * enetIfPtr = (enet_dev_if_t *)handle;

    memcpy(address, enetIfPtr->macAddr, kEnetMacAddrLen);

    return ENET_OK;
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_get_mac_address
 * Return Value: The execution status.
 * Description:Get the ENET address of an uninitialized device
 *
 *END*********************************************************************/
uint32_t ENET_get_mac_address(uint32_t device, uint32_t value, _enet_address address)
{
    uint8_t g_enetAddress[kEnetMacAddrLen] = ENET_DEFAULT_MAC_ADD;

    address[0] = g_enetAddress[0];
    address[1] = g_enetAddress[1];
    address[2] = g_enetAddress[2];
    address[3] = (value & 0xFF00000U)>>16;
    address[4] = (value & 0xFF00U) >> 8;
    address[5] = (value & 0xFFU);

    return ENET_OK;
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_join
 * Return Value: The execution status.
 * Description: Join a multicast group.
 *
 *END*********************************************************************/
uint32_t ENET_join(_enet_handle handle, uint16_t type, _enet_address address)
{
    enet_dev_if_t * enetIfPtr = (enet_dev_if_t *)handle;
    enet_multicast_group_t *enetMultiGroupPtr;
    uint32_t hash;

    /* Make sure it's a multicast group*/
    if (!(address[0] & 1U))
    {
       return kStatus_ENET_NoMulticastAddr;
    }

    OSA_MutexLock(&enetIfPtr->enetContextSync, OSA_WAIT_FOREVER);

    if (!enetIfPtr->multiGroupPtr)
    {
        enetIfPtr->multiGroupPtr = OSA_MemAlloc(sizeof(enet_multicast_group_t));
        if (enetIfPtr->multiGroupPtr == NULL)
        {
            OSA_MutexUnlock(&enetIfPtr->enetContextSync);
            return kStatus_ENET_MemoryAllocateFail;
        }
        memcpy(enetIfPtr->multiGroupPtr->groupAdddr, address, kEnetMacAddrLen);
        ENET_DRV_AddMulticastGroup(enetIfPtr->deviceNumber, address, &hash);
        enetIfPtr->multiGroupPtr->hash = hash;
        enetIfPtr->multiGroupPtr->next = NULL;
        enetIfPtr->multiGroupPtr->prv = NULL;
    }
    else
    {
        /* Check if we had add the multicast group*/
        enetMultiGroupPtr = enetIfPtr->multiGroupPtr;
        while (enetMultiGroupPtr != NULL)
        {
            if (!memcmp(enetMultiGroupPtr->groupAdddr, address, kEnetMacAddrLen))
            {
                OSA_MutexUnlock(&enetIfPtr->enetContextSync);
            	return kStatus_ENET_AlreadyAddedMulticast;
            }
            if (enetMultiGroupPtr->next == NULL)
            {
                break;
            }
            enetMultiGroupPtr =  enetMultiGroupPtr->next;
        }

        /* Add this multicast group*/
        enetMultiGroupPtr->next = OSA_MemAllocZero(sizeof(enet_multicast_group_t));
        if (enetMultiGroupPtr->next == NULL)
        {
            OSA_MutexUnlock(&enetIfPtr->enetContextSync);
            return kStatus_ENET_MemoryAllocateFail;
        }
        memcpy(enetMultiGroupPtr->next->groupAdddr, address, kEnetMacAddrLen);
        ENET_DRV_AddMulticastGroup(enetIfPtr->deviceNumber, address, &hash);
        enetMultiGroupPtr->next->hash = hash;
        enetMultiGroupPtr->next->next = NULL;
        enetMultiGroupPtr->next->prv = enetMultiGroupPtr;
    }

    OSA_MutexUnlock(&enetIfPtr->enetContextSync);
    return ENET_OK;
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_leave
 * Return Value: The execution status.
 * Description: Leave a multicast group.
 *
 *END*********************************************************************/
uint32_t ENET_leave(_enet_handle handle, uint16_t type, _enet_address address)
{
    enet_dev_if_t * enetIfPtr = (enet_dev_if_t *)handle;
    enet_multicast_group_t *enetMultiGroupPtr, *enetTempPtr;

    /* Make sure it's a multicast group*/
    if (!(address[0] & 1U))
    {
       return kStatus_ENET_NoMulticastAddr;
    }

    OSA_MutexLock(&enetIfPtr->enetContextSync, OSA_WAIT_FOREVER);

    if (!enetIfPtr->multiGroupPtr)
    {
        OSA_MutexUnlock(&enetIfPtr->enetContextSync);
        return kStatus_ENET_MulticastPointerNull;
    }

    /* Check if we had add the multicast group*/
    for (enetMultiGroupPtr = enetIfPtr->multiGroupPtr; enetMultiGroupPtr != NULL;enetMultiGroupPtr = enetMultiGroupPtr->next )
    {
        if (!memcmp(enetMultiGroupPtr->groupAdddr, address, kEnetMacAddrLen))
        {
            ENET_DRV_LeaveMulticastGroup(enetIfPtr->deviceNumber, address);
            memset(enetMultiGroupPtr->groupAdddr, 0, kEnetMacAddrLen);
            enetTempPtr = enetMultiGroupPtr->prv;
            if (enetTempPtr != NULL)
            {
                enetTempPtr->next = enetMultiGroupPtr->next;
            }
            if (enetMultiGroupPtr->next != NULL)
            {
                enetMultiGroupPtr->next->prv = enetTempPtr;
            }
            OSA_MemFree((void *)enetMultiGroupPtr);
            break;
        }
    }

    OSA_MutexUnlock(&enetIfPtr->enetContextSync);

    return ENET_OK;
}

#if BSPCFG_ENABLE_ENET_STATS
/*FUNCTION****************************************************************
 *
 * Function Name: ENET_get_stats
 * Return Value: The execution status.
 * Description: Get ENET packets statistic.
 *
 *END*********************************************************************/
ENET_STATS_PTR ENET_get_stats(_enet_handle handle)
{
#if ENET_ENABLE_DETAIL_STATS
    /* Common stats*/
    memcpy(&enetStats.COMMON, &((enet_dev_if_t *)handle)->stats, sizeof(enetStats.COMMON));
    /* Detail stats*/
    enetStats.ST_RX_ALIGN = ((enet_dev_if_t *)handle)->stats.statsRxAlign;
    enetStats.ST_RX_FCS = ((enet_dev_if_t *)handle)->stats.statsRxFcs;
    enetStats.ST_RX_GIANT = ((enet_dev_if_t *)handle)->stats.statsRxLengthGreater;
    enetStats.ST_RX_LATECOLL = ((enet_dev_if_t *)handle)->stats.statsRxCollision;
    enetStats.ST_RX_OVERRUN = ((enet_dev_if_t *)handle)->stats.statsRxOverRun;
    enetStats.ST_RX_RUNT = ((enet_dev_if_t *)handle)->stats.statsRxTruncate;
    enetStats.ST_TX_EXCESSCOLL = ((enet_dev_if_t *)handle)->stats.statsTxExcessCollision;
    enetStats.ST_TX_UNDERRUN = ((enet_dev_if_t *)handle)->stats.statsTxUnderFlow;
    enetStats.ST_TX_LATECOLL = ((enet_dev_if_t *)handle)->stats.statsTxLateCollision;

    enetStats.ST_TX_COPY_LARGE = ((enet_dev_if_t *)handle)->stats.statsTxLarge;
#endif
    return (ENET_STATS_PTR)&enetStats;
}
#endif
/*FUNCTION****************************************************************
 *
 * Function Name: ENET_link_status
 * Return Value: True if link is up else false.
 * Description: Get ENET link status.
 * If ENET is link up return true else false.
 *
 *END*********************************************************************/
bool ENET_link_status(_enet_handle handle)
{
    enet_dev_if_t * enetIfPtr;
    bool status = false;

    /* Check input parameter*/
    if (handle == NULL)
    {
        return kStatus_ENET_InvalidDevice;
    }
    enetIfPtr = (enet_dev_if_t *)handle;

    PHY_DRV_GetLinkStatus(enetIfPtr->deviceNumber, enetIfPtr->phyAddr, &status);
    return status;
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_get_speed
 * Return Value: The link speed.
 * Description: Get ENET link speed.
 *
 *END*********************************************************************/
uint32_t ENET_get_speed(_enet_handle handle)
{
    enet_dev_if_t * enetIfPtr;
    enet_phy_speed_t speed;
    uint32_t result;

    /* Check input parameter*/
    if (handle == NULL)
    {
        return kStatus_ENET_InvalidDevice;
    }

    enetIfPtr = (enet_dev_if_t *)handle;
    result = PHY_DRV_GetLinkSpeed(enetIfPtr->deviceNumber, enetIfPtr->phyAddr, &speed);
    if (result != kStatus_ENET_Success)
    {
        return result;
    }
    else
    {
        if (speed == (uint32_t)kEnetSpeed100M)
        {
            return 100;
        }
        else if (speed == (uint32_t)kEnetSpeed10M)
        {
            return 10;
        }
    }

    return ENET_ERROR;
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_get_MTU
 * Return Value: The value of MTU.
 * Description: Get ENET MTU.
 *
 *END*********************************************************************/
uint32_t ENET_get_MTU(_enet_handle handle)
{
    enet_dev_if_t * enetIfPtr;

    /* Check input parameter*/
    if (handle == NULL)
    {
        return kStatus_ENET_InvalidDevice;
    }

    enetIfPtr = (enet_dev_if_t *)handle;
    if (!enetIfPtr->maxFrameSize)
    {
        return kEnetMaxFrameDateSize;
    }

    if (enetIfPtr->isVlanTagEnabled)
    {
        return enetIfPtr->maxFrameSize - kEnetEthernetHeadLen - kEnetFrameFcsLen;
    }
    else
    {
       return enetIfPtr->maxFrameSize - sizeof(enet_8021vlan_header_t) - kEnetFrameFcsLen;
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_phy_register
 * Return Value: The number of registers .
 * Description: Read required ENET PHY registers.
 *
 *END*********************************************************************/
bool ENET_phy_registers(_enet_handle handle, uint32_t numRegs, uint32_t *regPtr)
{
    uint32_t counter;
    enet_dev_if_t *enetIfPtr;

    /* Check input parameter*/
    if (handle == NULL)
    {
        return kStatus_ENET_InvalidDevice;
    }

    enetIfPtr = (enet_dev_if_t *)handle;
    if (!enetIfPtr->maxFrameSize)
    {
        return kEnetMaxFrameDateSize;
    }

    for (counter = 0; counter < numRegs; counter++)
    {
        *regPtr = 0;
        if (PHY_DRV_Read(enetIfPtr->deviceNumber, enetIfPtr->phyAddr, counter, regPtr) != kStatus_ENET_Success)
        {
            return false;
        }
        regPtr ++;
    }
    return true;
}
/*FUNCTION****************************************************************
 *
 * Function Name: ENET_get_next_device_handle
 * Return Value: The device structure address .
 * Description: Get the next device structure address.
 *
 *END*********************************************************************/
_enet_handle ENET_get_next_device_handle(_enet_handle handle)
{
    enet_dev_if_t * enetIfPtr;

    /* Check input parameter*/
    if (handle == NULL)
    {
        return NULL;
    }

    enetIfPtr = (enet_dev_if_t *)handle;

    return (void *)enetIfPtr->next;
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_get_options
 * Return Value: The device structure address.
 * Description: Get device option.
 *
 *END*********************************************************************/
uint32_t ENET_get_options(_enet_handle handle)
{
    enet_dev_if_t * enetIfPtr;
    uint32_t option = 0;

    /* Check input parameter*/
    if (handle == NULL)
    {
        return kStatus_ENET_InvalidDevice;
    }

    enetIfPtr = (enet_dev_if_t *)handle;

    if((g_enetMacCfg[enetIfPtr->deviceNumber].macCtlConfigure & kEnetRxAccelEnable) &&
         (g_enetMacCfg[enetIfPtr->deviceNumber].rxAccelerPtr != NULL))
    {
        if (g_enetMacCfg[enetIfPtr->deviceNumber].rxAccelerPtr->isIpcheckEnabled)
        {
            option |= ENET_OPTION_HW_RX_IP_CHECKSUM;
        }
        if (g_enetMacCfg[enetIfPtr->deviceNumber].rxAccelerPtr->isProtocolCheckEnabled)
        {
            option |= ENET_OPTION_HW_RX_PROTOCOL_CHECKSUM;
        }
        if (g_enetMacCfg[enetIfPtr->deviceNumber].rxAccelerPtr->isMacCheckEnabled)
        {
            option |= ENET_OPTION_HW_RX_MAC_ERR;
        }
    }
    if((g_enetMacCfg[enetIfPtr->deviceNumber].macCtlConfigure & kEnetTxAccelEnable)
        && (g_enetMacCfg[enetIfPtr->deviceNumber].txAccelerPtr != NULL))
    {
        if (g_enetMacCfg[enetIfPtr->deviceNumber].txAccelerPtr->isIpCheckEnabled)
        {
            option |= ENET_OPTION_HW_TX_IP_CHECKSUM;
        }
        if (g_enetMacCfg[enetIfPtr->deviceNumber].txAccelerPtr->isProtocolCheckEnabled)
        {
            option |= ENET_OPTION_HW_TX_PROTOCOL_CHECKSUM;
        }
    }

    return option;
}

/*FUNCTION****************************************************************
 *
 * Function Name: ENET_close
 * Return Value: The execution status.
 * Description: Unregisters a protocol type on an Ethernet channel.
 *
 *END*********************************************************************/
uint32_t ENET_close(_enet_handle handle, uint16_t type)
{
#if !ENET_RECEIVE_ALL_INTERRUPT

    /* Check input parameter*/
    if (handle == NULL)
    {
        return kStatus_ENET_InvalidDevice;
    }
#else
    enet_dev_if_t *enetIfPtr;
    enet_ecb_struct_t *ecbPtr, **searchPtr;

    /* Check input parameter*/
    if (handle == NULL)
    {
        return kStatus_ENET_InvalidDevice;
    }
    enetIfPtr = (enet_dev_if_t *)handle;
    OSA_MutexLock(&enetIfPtr->enetContextSync, OSA_WAIT_FOREVER);

    for (searchPtr = (enet_ecb_struct_t **)&enetIfPtr->netIfPrivate;
        *searchPtr; searchPtr = &(*searchPtr)->NEXT)
    {
        if ((*searchPtr)->TYPE == type)
        {
            break;
        }
    }

    if (!*searchPtr)
    {
        OSA_MutexUnlock(&enetIfPtr->enetContextSync);
        return kStatus_ENET_Close;
    }

    ecbPtr = *searchPtr;
    *searchPtr = ecbPtr->NEXT;

    OSA_MutexUnlock(&enetIfPtr->enetContextSync);
    OSA_MemFree(ecbPtr);

#endif

    return ENET_OK;
}
/*FUNCTION****************************************************************
 *
 * Function Name: ENET_mediactl
 * Return Value: The execution status.
 * Description: ENET mediactl interface.
 *
 *END*********************************************************************/
uint32_t ENET_mediactl(_enet_handle handle, uint32_t commandId, void *inOutParam)
{
    return ENET_OK;
}

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name : ENET_strerror
*  Returned Value: pointer to error string
*  Description: Describe an ENET error code
*
*END*-----------------------------------------------------------------*/
const char * ENET_strerror(uint32_t  error)
{
    if (error == ENET_OK)
    {
        return "OK";
    }
    if ((error < kStatus_ENET_InvalidInput) || (error > kStatus_ENET_AlreadyAddedMulticast))
    {
        return "Unknown error";
    }
    return ENET_errlist[error - kStatus_ENET_InvalidInput];
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
