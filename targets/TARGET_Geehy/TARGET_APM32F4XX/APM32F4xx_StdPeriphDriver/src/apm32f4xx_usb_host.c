/*!
 * @file        apm32f4xx_usb_host.h
 *
 * @brief       USB host function handle
 *
 * @version     V1.0.2
 *
 * @date        2023-07-31
 *
 * @attention
 *
 *  Copyright (C) 2023 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

/* Includes */
#include "apm32f4xx_usb_host.h"

/** @addtogroup APM32F4xx_StdPeriphDriver
  @{
*/

/** @addtogroup USB_Host_Driver USB Host Driver
  @{
*/

/** @defgroup USB_Host_Functions Functions
  @{
*/

void delay_t(int time)
{
    for(int i = 0; i < 1000; i++)
    {
        for(int j = 0; j < time; j++);
    }
}

/*!
 * @brief     USB host configure data PID
 *
 * @param     usbhh: USB host handler.
 *
 * @param     channelNum: channel number
 *
 * @param     dataPid: data PID
 *
 * @retval    None
 */
void USBH_ConfigDataPid(USBH_HANDLE_T* usbhh, uint8_t channelNum, uint8_t dataPid)
{
    if (usbhh->xferPipe[channelNum].epDir == EP_DIR_IN)
    {
        usbhh->xferPipe[channelNum].pidToggleIn = dataPid;
    }
    else
    {
        usbhh->xferPipe[channelNum].pidToggleOut = dataPid;
    }
}

/*!
 * @brief       Handle Disconnect detected interrupt
 *
 * @param       usbhh: USB host handler.
 *
 * @retval      None
 */
static void USBH_DisconnectIsrHandler(USBH_HANDLE_T* usbhh)
{
    /* Clear interrupt */
    USB_OTG_ClearGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_DEDIS);

    /* Host port is disconnected */
    if (USB_OTG_H_PortIsConnect(usbhh->usbHost) == RESET)
    {
        /* Clear USB FIFO*/
        USB_OTG_FlushTxFIFO(usbhh->usbGlobal, 0x10);
        USB_OTG_FlushRxFIFO(usbhh->usbGlobal);

        /* Reset FS clock*/
        USB_OTG_H_ConfigHostClk(usbhh->usbHost, PHYCLK_48_MHZ);
        USB_OTG_H_SetFrameInterval(usbhh->usbHost, 48000);

        /* Disconnect event callback */
        USBH_DisconnectCallback(usbhh);
    }
}

/*!
 * @brief       Handle RxFIFO no empty interrupt
 *
 * @param       usbhh: USB host handler.
 *
 * @retval      None
 */
static void USBH_RxFifoNoEmptyIsrHandler(USBH_HANDLE_T* usbhh)
{
    USBH_FIFO_STA_T fifoStatus;
    uint16_t channelNum;
    uint8_t packetStatus;
    uint16_t packetCnt;

    /* Disable RxFIFO no empty Interrup */
    USB_OTG_DisableGlobalInterrupt(usbhh->usbGlobal, USB_INT_G_RXFNONE);

    /* Read and pop the RxFIFO status data */
    fifoStatus.FIFO_STATUS = USB_OTG_PopRxFifoStatus(usbhh->usbGlobal);
    channelNum = fifoStatus.FIFO_STATUS_B.chNum;
    packetStatus = fifoStatus.FIFO_STATUS_B.packetStatus;
    packetCnt = fifoStatus.FIFO_STATUS_B.byteCount;

    switch (packetStatus)
    {
        case USBH_PKTSTS_IN:
            /* Read the data into the host buffer */
            if ((packetCnt > 0) && (usbhh->xferPipe[channelNum].buffer != NULL))
            {
                if ((usbhh->xferPipe[channelNum].bufCount + packetCnt) > \
                        usbhh->xferPipe[channelNum].bufLen)
                {
                    usbhh->xferPipe[channelNum].urbStatus = USB_URB_ERROR;
                }
                else
                {
                    USB_OTG_FIFO_ReadRxFifoPacket(usbhh->usbFifo, usbhh->xferPipe[channelNum].buffer \
                                                  , packetCnt);

                    /*manage multiple Xfer */
                    usbhh->xferPipe[channelNum].buffer     += packetCnt;
                    usbhh->xferPipe[channelNum].bufCount   += packetCnt;

                    if ((USB_OTG_H_ReadPacketCount(usbhh->usbHost, channelNum) > 0) && \
                            (usbhh->xferPipe[channelNum].maxPackSize == packetCnt))
                    {
                        /* re-activate the channel when more packets are expected */
                        USB_OTG_H_EnableChannel(usbhh->usbHost, channelNum);

                        usbhh->xferPipe[channelNum].pidToggleIn ^= 1;
                    }
                }
            }
            break;

        case USBH_PKTSTS_IN_XFER_COMPLETE:
            break;

        case USBH_PKTSTS_DATA_ISO_ERR:
            break;

        case USBH_PKTSTS_CHANNEL_HALT:
            break;

        default:
            break;
    }

    /* Enable RxFIFO no empty Interrupt */
    USB_OTG_EnableGlobalInterrupt(usbhh->usbGlobal, USB_INT_G_RXFNONE);
}

/*!
 * @brief     Halt usb host channel
 *
 * @param     usbhh: USB host handler.
 *
 * @param     chNum: host channel number
 *
 * @retval    None
 */
void USB_OTG_H_HaltChannel(USBH_HANDLE_T* usbhh, uint8_t chNum)
{
    __IO uint32_t timeout = 0;
    uint8_t epTypte;
    uint8_t chEnableSta;
    uint8_t dmaEnableSta;

    epTypte = USB_OTG_H_ReadEpType(usbhh->usbHost, chNum);
    chEnableSta = USB_OTG_H_ReadChannelStatus(usbhh->usbHost, chNum);
    dmaEnableSta = USB_OTG_ReadDMAStatus(usbhh->usbGlobal);

    /* HS DMA enable status */
    if ((dmaEnableSta == ENABLE) && (chEnableSta == DISABLE))
    {
        return;
    }

    /* Read Remain space to issue the halt event */
    switch (epTypte)
    {
        case EP_TYPE_CONTROL:
        case EP_TYPE_BULK:
            /* stop channel */
            usbhh->usbHost->REGS_HCH[chNum].HCH_B.CHINT = BIT_SET;

            if (dmaEnableSta == DISABLE)
            {
                if (usbhh->usbGlobal->GNPTXFQSTS_B.NPTXRSA == 0)
                {
                    usbhh->usbHost->REGS_HCH[chNum].HCH_B.CHEN = BIT_RESET;
                    usbhh->usbHost->REGS_HCH[chNum].HCH_B.CHEN = BIT_SET;

                    while (usbhh->usbHost->REGS_HCH[chNum].HCH_B.CHEN)
                    {
                        timeout++;

                        if (timeout >= 0x400)
                        {
                            break;
                        }
                    }
                }
                else
                {
                    usbhh->usbHost->REGS_HCH[chNum].HCH_B.CHEN = BIT_SET;
                }
            }
            break;

        default:
            /* stop channel */
            usbhh->usbHost->REGS_HCH[chNum].HCH_B.CHINT = BIT_SET;

            if (USB_OTG_H_ReadTxQueueRemainSpace(usbhh->usbHost) == 0)
            {
                usbhh->usbHost->REGS_HCH[chNum].HCH_B.CHEN = BIT_RESET;
                usbhh->usbHost->REGS_HCH[chNum].HCH_B.CHEN = BIT_SET;

                while (usbhh->usbHost->REGS_HCH[chNum].HCH_B.CHEN)
                {
                    timeout++;

                    if (timeout >= 0x400)
                    {
                        break;
                    }
                }
            }
            else
            {
                usbhh->usbHost->REGS_HCH[chNum].HCH_B.CHEN = BIT_SET;
            }
            break;
    }
}

/*!
 * @brief     Handle Host port in interrupt
 *
 * @param     usbhh: USB host handler.
 *
 * @param     channelNum : Select channel.
 *
 * @retval    None
 */
static void USBH_IN_HostChannelIsrHandler(USBH_HANDLE_T* usbhh, uint8_t channelNum)
{
    /* AHB Error only in HS mode*/
    if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_AHBERR))
    {
        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_AHBERR);
        USB_OTG_H_HaltChannel(usbhh, channelNum);

        usbhh->xferPipe[channelNum].pipeState = PIPE_XACTERR;
    }
    /* Babble Error */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_BABBLE))
    {
        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_BABBLE);
        USB_OTG_H_HaltChannel(usbhh, channelNum);

        usbhh->xferPipe[channelNum].pipeState = PIPE_BBLERR;
    }
    /* ACK Response Received / Transmitted */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXTXACK))
    {
        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXTXACK);
    }
    /* STALL Response Received */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXSTALL))
    {
        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXSTALL);
        USB_OTG_H_HaltChannel(usbhh, channelNum);

        usbhh->xferPipe[channelNum].pipeState = PIPE_STALL;
    }
    /* Data Toggle Error */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_DTOG))
    {
        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_DTOG);
        USB_OTG_H_HaltChannel(usbhh, channelNum);

        usbhh->xferPipe[channelNum].pipeState = PIPE_DATATGLERR;
    }
    /* Transaction Error */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_TERR))
    {
        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_TERR);
        USB_OTG_H_HaltChannel(usbhh, channelNum);

        usbhh->xferPipe[channelNum].pipeState = PIPE_XACTERR;
    }
    /* Frame Overrun Error */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_FOVR))
    {
        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_FOVR);
        USB_OTG_H_HaltChannel(usbhh, channelNum);
    }
    /* Transfer Complete Normally */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_TSFCMPN))
    {
        if (usbhh->usbCfg.dmaStatus != DISABLE)
        {
            usbhh->xferPipe[channelNum].bufCount = usbhh->xferPipe[channelNum].bufSize - \
                                                   USB_OTG_H_ReadTransferSize(usbhh->usbHost, channelNum);
        }

        usbhh->xferPipe[channelNum].pipeState = PIPE_XFRC;
        usbhh->xferPipe[channelNum].errorCnt = 0;

        switch (usbhh->xferPipe[channelNum].epType)
        {
            case EP_TYPE_CONTROL:
            case EP_TYPE_BULK:
                USB_OTG_H_HaltChannel(usbhh, channelNum);
                USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXNAK);
                break;

            case EP_TYPE_INTERRUPT:
            case EP_TYPE_ISO:
                USB_OTG_H_TxUseOddFrame(usbhh->usbHost, channelNum);
                usbhh->xferPipe[channelNum].urbStatus = USB_URB_OK;

                /* USB update URB status callback */
                USBH_UpdateUrbCallback(usbhh);
                break;

            default:
                break;
        }

        if (usbhh->usbCfg.dmaStatus == ENABLE)
        {
            if (((usbhh->xferPipe[channelNum].bufSize / \
                    usbhh->xferPipe[channelNum].maxPackSize) & 0x01) != 0)
            {
                usbhh->xferPipe[channelNum].pidToggleIn ^= 1;
            }
        }
        else
        {
            usbhh->xferPipe[channelNum].pidToggleIn ^= 1;
        }

        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_TSFCMPN);
    }
    /* Transfer Complete Abnormally */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_TSFCMPAN))
    {
        switch (usbhh->xferPipe[channelNum].pipeState)
        {
            case PIPE_XFRC:
                usbhh->xferPipe[channelNum].urbStatus = USB_URB_OK;
                break;

            case PIPE_STALL:
                usbhh->xferPipe[channelNum].urbStatus = USB_URB_STALL;
                break;

            case PIPE_XACTERR:
            case PIPE_DATATGLERR:
                usbhh->xferPipe[channelNum].errorCnt++;
                if (usbhh->xferPipe[channelNum].errorCnt >= 3)
                {
                    usbhh->xferPipe[channelNum].errorCnt = 0;
                    usbhh->xferPipe[channelNum].urbStatus = USB_URB_ERROR;
                }
                else
                {
                    usbhh->xferPipe[channelNum].urbStatus = USB_URB_NOREADY;

                    /* reactivate the host channel */
                    usbhh->usbHost->REGS_HCH[channelNum].HCH_B.CHINT = BIT_RESET;
                    usbhh->usbHost->REGS_HCH[channelNum].HCH_B.CHEN = BIT_SET;
                }
                break;

            case PIPE_NAK:
                usbhh->xferPipe[channelNum].urbStatus = USB_URB_NOREADY;

                /* reactivate the host channel */
                usbhh->usbHost->REGS_HCH[channelNum].HCH_B.CHINT = BIT_RESET;
                usbhh->usbHost->REGS_HCH[channelNum].HCH_B.CHEN = BIT_SET;
                break;

            case PIPE_BBLERR:
                usbhh->xferPipe[channelNum].errorCnt++;
                usbhh->xferPipe[channelNum].urbStatus = USB_URB_ERROR;
                break;

            default:
                break;
        }

        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_TSFCMPAN);

        /* USB update URB status callback */
        USBH_UpdateUrbCallback(usbhh);
    }
    /* NAK Response Received */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXNAK))
    {
        switch (usbhh->xferPipe[channelNum].epType)
        {
            case EP_TYPE_CONTROL:
            case EP_TYPE_BULK:
                usbhh->xferPipe[channelNum].errorCnt = 0;

                if (usbhh->usbCfg.dmaStatus == DISABLE)
                {
                    usbhh->xferPipe[channelNum].pipeState = PIPE_NAK;
                    USB_OTG_H_HaltChannel(usbhh, channelNum);
                }
                break;

            case EP_TYPE_INTERRUPT:
                usbhh->xferPipe[channelNum].errorCnt = 0;
                USB_OTG_H_HaltChannel(usbhh, channelNum);
                break;

            default:
                break;
        }

        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXNAK);
    }
    else
    {

    }
}

/*!
 * @brief       Handle Host port out interrupt
 *
 * @param       usbhh: USB host handler.
 *
 * @param       channelNum : Select channel.
 *
 * @retval      None
 */
static void USBH_OUT_HostChannelIsrHandler(USBH_HANDLE_T* usbhh, uint8_t channelNum)
{
    /* AHB Error only in HS mode*/
    if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_AHBERR))
    {
        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_AHBERR);
        USB_OTG_H_HaltChannel(usbhh, channelNum);

        usbhh->xferPipe[channelNum].pipeState = PIPE_XACTERR;
    }
    /* ACK Response Received/Transmitted */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXTXACK))
    {
        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXTXACK);

        if (usbhh->xferPipe[channelNum].pingStatus == ENABLE)
        {
            usbhh->xferPipe[channelNum].pingStatus = DISABLE;
            usbhh->xferPipe[channelNum].urbStatus = USB_URB_NOREADY;
            USB_OTG_H_HaltChannel(usbhh, channelNum);
        }
    }
    /* Frame Overrun Error */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_FOVR))
    {
        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_FOVR);
        USB_OTG_H_HaltChannel(usbhh, channelNum);
    }
    /* Transfer Complete Normally */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_TSFCMPN))
    {
        usbhh->xferPipe[channelNum].errorCnt = 0;

        /* Update ping status */
        if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXNYET))
        {
            usbhh->xferPipe[channelNum].pingStatus = ENABLE;
            USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXNYET);
        }

        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_TSFCMPN);
        usbhh->xferPipe[channelNum].pipeState = PIPE_XFRC;
        USB_OTG_H_HaltChannel(usbhh, channelNum);
    }
    /* NYET Response Received */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXNYET))
    {
        usbhh->xferPipe[channelNum].pingStatus = ENABLE;
        usbhh->xferPipe[channelNum].errorCnt = 0;
        usbhh->xferPipe[channelNum].pipeState = PIPE_NYET;
        USB_OTG_H_HaltChannel(usbhh, channelNum);
        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXNYET);
    }
    /* STALL Response Received */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXSTALL))
    {
        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXSTALL);

        usbhh->xferPipe[channelNum].pipeState = PIPE_STALL;
        USB_OTG_H_HaltChannel(usbhh, channelNum);
    }
    /* NAK Response Received */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXNAK))
    {
        usbhh->xferPipe[channelNum].errorCnt = 0;
        usbhh->xferPipe[channelNum].pipeState = PIPE_NAK;

        if (usbhh->xferPipe[channelNum].pingStatus == DISABLE)
        {
            if (usbhh->xferPipe[channelNum].speed == USBH_DEV_SPEED_HIGH)
            {
                usbhh->xferPipe[channelNum].pingStatus = ENABLE;
            }
        }

        USB_OTG_H_HaltChannel(usbhh, channelNum);
        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_RXNAK);
    }
    /* Transaction Error */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_TERR))
    {
        if (usbhh->usbCfg.dmaStatus == DISABLE)
        {
            usbhh->xferPipe[channelNum].pipeState = PIPE_XACTERR;
            USB_OTG_H_HaltChannel(usbhh, channelNum);
        }
        else
        {
            usbhh->xferPipe[channelNum].errorCnt++;
            if (usbhh->xferPipe[channelNum].errorCnt >= 2)
            {
                usbhh->xferPipe[channelNum].errorCnt = 0;
                usbhh->xferPipe[channelNum].urbStatus = USB_URB_ERROR;

                /* USB update URB status callback */
                USBH_UpdateUrbCallback(usbhh);
            }
            else
            {
                usbhh->xferPipe[channelNum].urbStatus = USB_URB_NOREADY;
            }
        }
        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_TERR);
    }
    /* Data Toggle Error */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_DTOG))
    {
        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_DTOG);
        USB_OTG_H_HaltChannel(usbhh, channelNum);

        usbhh->xferPipe[channelNum].pipeState = PIPE_DATATGLERR;
    }
    /* Transfer Complete Abnormally */
    else if (USB_OTG_H_ReadChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_TSFCMPAN))
    {
        switch (usbhh->xferPipe[channelNum].pipeState)
        {
            case PIPE_XFRC:
                usbhh->xferPipe[channelNum].urbStatus = USB_URB_OK;
                if ((usbhh->xferPipe[channelNum].epType == EP_TYPE_BULK) || \
                        (usbhh->xferPipe[channelNum].epType == EP_TYPE_INTERRUPT))
                {
                    if (usbhh->usbCfg.dmaStatus == DISABLE)
                    {
                        usbhh->xferPipe[channelNum].pidToggleOut ^= 1;
                    }

                    if ((usbhh->usbCfg.dmaStatus == ENABLE) && \
                            (usbhh->xferPipe[channelNum].bufLen > 0))
                    {
                        if ((((usbhh->xferPipe[channelNum].bufLen + \
                                usbhh->xferPipe[channelNum].maxPackSize - 1) /
                                usbhh->xferPipe[channelNum].maxPackSize) & 0x01) != 0)
                        {
                            usbhh->xferPipe[channelNum].pidToggleOut ^= 1;
                        }
                    }
                }
                break;

            case PIPE_NYET:
                usbhh->xferPipe[channelNum].urbStatus = USB_URB_NOREADY;
                break;

            case PIPE_STALL:
                usbhh->xferPipe[channelNum].urbStatus = USB_URB_STALL;
                break;

            case PIPE_XACTERR:
            case PIPE_DATATGLERR:
                usbhh->xferPipe[channelNum].errorCnt++;
                if (usbhh->xferPipe[channelNum].errorCnt >= 3)
                {
                    usbhh->xferPipe[channelNum].errorCnt = 0;
                    usbhh->xferPipe[channelNum].urbStatus = USB_URB_ERROR;
                }
                else
                {
                    usbhh->xferPipe[channelNum].urbStatus = USB_URB_NOREADY;
                    /* reactivate the host channel */
                    usbhh->usbHost->REGS_HCH[channelNum].HCH_B.CHINT = BIT_RESET;
                    usbhh->usbHost->REGS_HCH[channelNum].HCH_B.CHEN = BIT_SET;
                }
                break;

            case PIPE_NAK:
                usbhh->xferPipe[channelNum].urbStatus = USB_URB_NOREADY;
                break;

            default:
                break;
        }

        USB_OTG_H_ClearChannelIntFlag(usbhh->usbHost, channelNum, CHANNEL_INT_TSFCMPAN);

        /* USB update URB status callback */
        USBH_UpdateUrbCallback(usbhh);
    }
    else
    {

    }
}

/*!
 * @brief       Handle Host channels interrupt
 *
 * @param       usbhh: USB host handler.
 *
 * @retval      None
 */
static void USBH_HostChannelsIsrHandler(USBH_HANDLE_T* usbhh)
{
    uint32_t i = 0;

    for (i = 0; i < usbhh->usbCfg.hostChannelNum; i++)
    {
        if (USB_OTG_H_ReadAllChannelsIntFlag(usbhh->usbHost, 1 << (i & 0x0F)) == SET)
        {
            if (USB_OTG_H_ReadEpDirection(usbhh->usbHost, i) == EP_DIR_IN)
            {
                USBH_IN_HostChannelIsrHandler(usbhh, i);
            }
            else
            {
                USBH_OUT_HostChannelIsrHandler(usbhh, i);
            }
        }
    }

    /* Clear interrupt */
    USB_OTG_ClearGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_HCHAN);
}

/*!
 * @brief       Handle Host port interrupt
 *
 * @param       usbhh: USB host handler.
 *
 * @retval      None
 */
static void USBH_HostPortIsrHandler(USBH_HANDLE_T* usbhh)
{
    __IO uint32_t temp;

    /* Store port status*/
    temp = usbhh->usbHost->HPORTCSTS;
    temp &= ~(BIT1 | BIT2 | BIT3 | BIT5);

    /* Read Port Connect Flag */
    if (USB_OTG_H_ReadPortConnectIntTriFlag(usbhh->usbHost) == SET)
    {
        if (USB_OTG_H_PortIsConnect(usbhh->usbHost) == SET)
        {
            /* USB host connect callback */
            USBH_ConnectCallback(usbhh);
        }

        /* PCINTFLG */
        temp |= BIT1;
    }

    /* Port Enable is Changed */
    if (USB_OTG_H_PenBitIsChange(usbhh->usbHost) == SET)
    {
        /* PENCHG */
        temp |= BIT3;

        /* Port is Enable */
        if (USB_OTG_H_PortIsEnable(usbhh->usbHost) == SET)
        {
            if (usbhh->usbCfg.phyType == USB_OTG_PHY_EMB)
            {
                if(usbhh->usbCfg.speed == USB_OTG_SPEED_FSLS)
                {
                    if (USB_OTG_H_ReadPortSpeedType(usbhh->usbHost) == USBH_PORT_SPEED_LS)
                    {
                        USB_OTG_H_ConfigHostClk(usbhh->usbHost, PHYCLK_6_MHZ);
                        USB_OTG_H_SetFrameInterval(usbhh->usbHost, 6000);
                    }
                    else
                    {
                        USB_OTG_H_ConfigHostClk(usbhh->usbHost, PHYCLK_48_MHZ);
                        USB_OTG_H_SetFrameInterval(usbhh->usbHost, 48000);
                    }
                }
                else
                {
                    if (USB_OTG_H_ReadPortSpeedType(usbhh->usbHost) == USBH_PORT_SPEED_LS)
                    {
                        USB_OTG_H_ConfigHostClk(usbhh->usbHost, PHYCLK_6_MHZ);
                        USB_OTG_H_SetFrameInterval(usbhh->usbHost, 6000);
                    }
                    else if (USB_OTG_H_ReadPortSpeedType(usbhh->usbHost) == USBH_PORT_SPEED_FS)
                    {
                        USB_OTG_H_ConfigHostClk(usbhh->usbHost, PHYCLK_48_MHZ);
                        USB_OTG_H_SetFrameInterval(usbhh->usbHost, 48000);
                    }
                    else
                    {
                        USB_OTG_H_ConfigHostClk(usbhh->usbHost, PHYCLK_30_60_MHZ);
                    }
                }
            }
            else
            {
                if (usbhh->usbCfg.speed == USB_OTG_SPEED_FSLS)
                {
                    USB_OTG_H_SetFrameInterval(usbhh->usbHost, 60000);
                }
            }

            /* USB host enable callback */
            USBH_PortEnableCallback(usbhh);
        }
        else
        {
            /* USB host disable callback */
            USBH_PortDisableCallback(usbhh);
        }
    }

    /* Port overcurrent is Changed */
    if (USB_OTG_H_PovcBitIsChange(usbhh->usbHost) == SET)
    {
        /* POVCCHG */
        temp |= BIT5;
    }

    usbhh->usbHost->HPORTCSTS = temp;
}

/*!
 * @brief     Handle USB host global interrupt
 *
 * @param     usbhh: USB host handler
 *
 * @retval    None
 */
void USBH_OTG_IsrHandler(USBH_HANDLE_T* usbhh)
{
    if (USB_OTG_ReadMode(usbhh->usbGlobal) == USB_OTG_MODE_HOST)
    {
        /* Avoid spurious interrupt */
        if (USB_OTG_ReadInterrupts(usbhh->usbGlobal) == 0U)
        {
            return;
        }

        /* Nonperiodic TXFIFO Empty Interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_NPTXFEM))
        {
            /* Clear interrupt */
            USB_OTG_ClearGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_NPTXFEM);
        }

        /* Incomplete Periodic Transfer Interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_IP_OUTTX))
        {
            /* Clear interrupt */
            USB_OTG_ClearGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_IP_OUTTX);
        }

        /* Incomplete Isochronous IN Transfer Interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_IIINTX))
        {
            /* Clear interrupt */
            USB_OTG_ClearGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_IIINTX);
        }

        /* Periodic TXFIFO Empty Interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_PTXFE))
        {
            /* Clear interrupt */
            USB_OTG_ClearGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_PTXFE);
        }

        /* Mode Mismatch Interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_MMIS))
        {
            /* Clear interrupt */
            USB_OTG_ClearGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_MMIS);
        }

        /* Handle Disconnect detected interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_DEDIS))
        {
            USBH_DisconnectIsrHandler(usbhh);
        }

        /* Handle Host Port Interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_HPORT))
        {
            USBH_HostPortIsrHandler(usbhh);
        }

        /* Start of Frame Interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_SOF))
        {
            USBH_SOFCallback(usbhh);
            /* Clear interrupt */
            USB_OTG_ClearGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_SOF);
        }

        /* Handle RxFIFO no empty interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_RXFNONE))
        {
            USBH_RxFifoNoEmptyIsrHandler(usbhh);
        }

        /* Handle Host channels interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbhh->usbGlobal, USB_INT_G_HCHAN))
        {
            USBH_HostChannelsIsrHandler(usbhh);
        }
    }
}

/*!
 * @brief     USB host start
 *
 * @param     usbhh: USB host handler
 *
 * @retval    None
 */
void USBH_Start(USBH_HANDLE_T* usbhh)
{
    /* Enable USB OTG port power */
    USB_OTG_H_DriverVbus(usbhh->usbHost, USBH_PORT_PWR_ON);

    /* Enable USB OTG all global interrupt */
    USB_OTG_EnableAllGlobalInterrupt(usbhh->usbGlobal);
}

/*!
 * @brief     USB OTG host reset
 *
 * @param     usbhh: USB host handler
 *
 * @retval    None
 */
void USBH_OTG_ResetHost(USBH_HANDLE_T* usbhh)
{
    usbhh->usbHost->HPORTCSTS_B.PRST = BIT_SET;

    /* This bit need to keep more than 10 ms */
    delay_t(100);
    usbhh->usbHost->HPORTCSTS_B.PRST = BIT_RESET;
    delay_t(10);
}

/*!
 * @brief     USB OTG host read speed
 *
 * @param     usbhh: USB host handler
 *
 * @retval    speed
 */
uint8_t USBH_OTG_ReadSpeed(USBH_HANDLE_T* usbhh)
{
    return (usbhh->usbHost->HPORTCSTS_B.PSPDSEL);
}

/*!
 * @brief     USB OTG host read URB status
 *
 * @param     usbhh: USB host handler
 *
 * @param     channelNum: channel number
 *
 * @retval    URB status
 */
USB_OTG_URB_STA_T USBH_OTG_ReadUrbStatus(USBH_HANDLE_T* usbhh, uint8_t channelNum)
{
    return usbhh->xferPipe[channelNum].urbStatus;
}

/*!
 * @brief     USB OTG stop host port
 *
 * @param     usbhh: USB host handler
 *
 * @retval    None
 */
void USBH_OTG_StopHost(USBH_HANDLE_T* usbhh)
{
    uint8_t i;
    uint32_t timeout = 0;
    uint8_t regNum = 0;

    USB_OTG_DisableAllGlobalInterrupt(usbhh->usbGlobal);

    /* Flush all Tx and Rx FIFOs */
    USB_OTG_FlushTxFIFO(usbhh->usbGlobal, 0x10);
    USB_OTG_FlushRxFIFO(usbhh->usbGlobal);

    if(usbhh->usbCfg.speed == USB_OTG_SPEED_FSLS)
    {
        regNum = 8;
    }
    else
    {
        regNum = 12;
    }
    
    /* Flush out any leftover queued requests */
    for (i = 0; i < regNum; i++)
    {
        usbhh->usbHost->REGS_HCH[i].HCH_B.CHINT     = BIT_SET;
        usbhh->usbHost->REGS_HCH[i].HCH_B.CHEN      = BIT_RESET;
        usbhh->usbHost->REGS_HCH[i].HCH_B.EDPDRT    = BIT_RESET;
    }

    /* Halt all channels and put them into a known state */
    for (i = 0; i < regNum; i++)
    {
        usbhh->usbHost->REGS_HCH[i].HCH_B.CHINT     = BIT_SET;
        usbhh->usbHost->REGS_HCH[i].HCH_B.CHEN      = BIT_SET;
        usbhh->usbHost->REGS_HCH[i].HCH_B.EDPDRT    = BIT_RESET;

        while (usbhh->usbHost->REGS_HCH[i].HCH_B.CHEN)
        {
            timeout++;

            if (timeout >= 0x400)
            {
                break;
            }
        }
    }

    USB_OTG_ClearGlobalIntFlag(usbhh->usbGlobal, 0xFFFFFFFF);

    USB_OTG_EnableAllGlobalInterrupt(usbhh->usbGlobal);
}

/*!
 * @brief     USB host close the channel
 *
 * @param     usbhh : usb host handler
 *
 * @param     channelNum : channel numer
 *
 * @retval    None
 */
void USBH_CloseChannel(USBH_HANDLE_T* usbhh, uint8_t channelNum)
{
    USB_OTG_H_HaltChannel(usbhh, channelNum);
}

/*!
 * @brief     USB host reset
 *
 * @param     usbhh : usb host handler
 *
 * @retval    None
 */
void USBH_Reset(USBH_HANDLE_T* usbhh)
{
    USBH_HardwareResetCallback(usbhh);

    USB_OTG_DisableAllGlobalInterrupt(usbhh->usbGlobal);
}

/*!
 * @brief     USB host open the channel to transfer
 *
 * @param     usbhh : usb host handler
 *
 * @param     channelNum : channel numer
 *
 * @param     endPointNum : end point number
 *
 * @param     devAddr : USB device address
 *
 * @param     devSpeed : USB device speed
 *
 * @param     epType : end point type
 *
 * @param     packetMaxSize : max size of packet
 *
 * @retval    None
 */
void USBH_OTG_OpenChannel(USBH_HANDLE_T* usbhh, uint8_t channelNum, \
                          uint8_t endPointNum, uint8_t devAddr, \
                          uint8_t devSpeed, uint8_t epType, uint16_t packetMaxSize)
{
    /* Store usb host handler */
    usbhh->xferPipe[channelNum].pingStatus  = DISABLE;
    usbhh->xferPipe[channelNum].devAddr     = devAddr;
    usbhh->xferPipe[channelNum].maxPackSize = packetMaxSize;
    usbhh->xferPipe[channelNum].chNum       = channelNum;
    usbhh->xferPipe[channelNum].epType      = epType;
    usbhh->xferPipe[channelNum].epNum       = endPointNum & 0x7F;
    usbhh->xferPipe[channelNum].speed       = devSpeed;

    /* End point direction is IN*/
    if ((endPointNum & 0x80) == 0x80)
    {
        usbhh->xferPipe[channelNum].epDir = EP_DIR_IN;
    }
    else
    {
        usbhh->xferPipe[channelNum].epDir = EP_DIR_OUT;
    }

    /* Init usb host channel */
    /* Clear old interrupt for host channel */
    usbhh->usbHost->REGS_HCH[channelNum].HCHINT = 0xFFFFFFFF;

    /* Enable channel interrupt */
    if ((epType == EP_TYPE_CONTROL) || (epType == EP_TYPE_BULK))
    {
        usbhh->usbHost->REGS_HCH[channelNum].HCHIMASK = (CHANNEL_INT_TSFCMPN |
                CHANNEL_INT_AHBERR  |
                CHANNEL_INT_RXSTALL |
                CHANNEL_INT_TERR    |
                CHANNEL_INT_RXNAK   |
                CHANNEL_INT_DTOG);

        /* EP dir IN */
        if ((endPointNum & 0x80) == 0x80)
        {
            usbhh->usbHost->REGS_HCH[channelNum].HCHIMASK |= CHANNEL_INT_BABBLE;
        }
        else
        {
            /* Add CID judge */
        }
    }
    else if (epType == EP_TYPE_INTERRUPT)
    {
        usbhh->usbHost->REGS_HCH[channelNum].HCHIMASK = (CHANNEL_INT_TSFCMPN |
                CHANNEL_INT_AHBERR  |
                CHANNEL_INT_RXSTALL |
                CHANNEL_INT_TERR    |
                CHANNEL_INT_RXNAK   |
                CHANNEL_INT_FOVR    |
                CHANNEL_INT_DTOG);

        /* EP dir IN */
        if ((endPointNum & 0x80) == 0x80)
        {
            usbhh->usbHost->REGS_HCH[channelNum].HCHIMASK |= CHANNEL_INT_BABBLE;
        }
    }
    else if (epType == EP_TYPE_ISO)
    {
        usbhh->usbHost->REGS_HCH[channelNum].HCHIMASK = (CHANNEL_INT_TSFCMPN |
                CHANNEL_INT_AHBERR  |
                CHANNEL_INT_RXTXACK |
                CHANNEL_INT_FOVR);

        /* EP dir IN */
        if ((endPointNum & 0x80) == 0x80)
        {
            usbhh->usbHost->REGS_HCH[channelNum].HCHIMASK |= (CHANNEL_INT_BABBLE | CHANNEL_INT_TERR);
        }
    }
    else
    {

    }

    /* Enable host channel halt interrupt */
    usbhh->usbHost->REGS_HCH[channelNum].HCHIMASK |= CHANNEL_INT_TSFCMPAN;

    /* Enable host channel interrupt */
    usbhh->usbHost->HACHIMASK |= (0x00000001 << (channelNum & 0x0F));

    /* Enable global host channel interrupt */
    usbhh->usbGlobal->GINTMASK_B.HCHM = BIT_SET;

    /* Configure host channle feature */

    /* End point direction */
    if ((endPointNum & 0x80) == 0x80)
    {
        usbhh->usbHost->REGS_HCH[channelNum].HCH_B.EDPDRT = BIT_SET;
    }
    else
    {
        usbhh->usbHost->REGS_HCH[channelNum].HCH_B.EDPDRT = BIT_RESET;
    }

    /* USB LS device plugged to host */
    if ((devSpeed == USBH_DEV_SPEED_LOW) && \
            (usbhh->usbHost->HPORTCSTS_B.PSPDSEL != USBH_DEV_SPEED_LOW))
    {
        USB_OTG_H_EnableLowSpeedDevice(usbhh->usbHost, channelNum);
    }
    else
    {
        USB_OTG_H_DisableLowSpeedDevice(usbhh->usbHost, channelNum);
    }

    /* Device address */
    USB_OTG_H_ConfigDeviceAddr(usbhh->usbHost, channelNum, devAddr);

    /* Endpoint number */
    USB_OTG_H_ConfigEpNum(usbhh->usbHost, channelNum, endPointNum);

    /* Endpoint type */
    USB_OTG_H_ConfigEpType(usbhh->usbHost, channelNum, epType);

    /* Max data packet size */
    USB_OTG_H_ConfigEpMaxPSize(usbhh->usbHost, channelNum, packetMaxSize);

    /* Interrupt or ISO type */
    if ((epType == EP_TYPE_INTERRUPT) || (epType == EP_TYPE_ISO))
    {
        USB_OTG_H_TxUseOddFrame(usbhh->usbHost, channelNum);
    }
}

/*!
 * @brief     USB host channel start transfer
 *
 * @param     usbhh: USB host handler
 *
 * @param     xferPipe: transfer channel
 *
 * @retval    None
 */
void USB_OTG_ChannelStartXfer(USBH_HANDLE_T* usbhh, USBH_XFER_PIPE_T* xferPipe)
{
    uint16_t packetNum;
    uint16_t transferWordLen = 0;
    uint8_t frameParity = 0;
    uint32_t temp;

    /*  Get the number of packets to transfer */
    if (xferPipe->bufLen > 0)
    {
        packetNum = (uint16_t)((xferPipe->bufLen + xferPipe->maxPackSize - 1) / xferPipe->maxPackSize);

        if (packetNum > 256)
        {
            packetNum = 256;
            xferPipe->bufSize = (uint32_t)packetNum * xferPipe->maxPackSize;
        }
    }
    else
    {
        packetNum = 1;
    }

    if (xferPipe->epDir == EP_DIR_OUT)
    {
        xferPipe->bufSize = xferPipe->bufLen;
    }
    /* For IN channel xfer size is expected to be an integer multiple of maxPackSize size */
    else
    {
        xferPipe->bufSize = (uint32_t)packetNum * xferPipe->maxPackSize;
    }

    /* Config the HCHTSIZE register */
    usbhh->usbHost->REGS_HCH[xferPipe->chNum].HCHTSIZE = (xferPipe->bufSize) |
            (((uint32_t)packetNum << 19)) |
            (((uint32_t)xferPipe->pid << 29));

    if (usbhh->usbCfg.dmaStatus == ENABLE)
    {
        USB_OTG_H_ConfigDMAAddr(usbhh->usbHost, xferPipe->chNum, (uint32_t) xferPipe->buffer);
    }

    frameParity = (((uint32_t)usbhh->usbHost->HFIFM & 0x01) != 0) ? 0 : 1;
    usbhh->usbHost->REGS_HCH[xferPipe->chNum].HCH_B.ODDF = BIT_RESET;
    usbhh->usbHost->REGS_HCH[xferPipe->chNum].HCH_B.ODDF = frameParity;

    temp = usbhh->usbHost->REGS_HCH[xferPipe->chNum].HCH;
    temp &= ~BIT30;

    if (xferPipe->epDir == EP_DIR_OUT)
    {
        temp &= ~BIT15;
    }
    else
    {
        temp |= BIT15;
    }

    /* Must be enabled after the channel are configured */
    /* Enable channel */
    temp |= BIT31;
    usbhh->usbHost->REGS_HCH[xferPipe->chNum].HCH = temp;

    if (usbhh->usbCfg.dmaStatus == ENABLE)
    {
        return;
    }

    if ((xferPipe->bufLen > 0) && (xferPipe->epDir == EP_DIR_OUT))
    {
        if ((xferPipe->epType == EP_TYPE_CONTROL) || \
                (xferPipe->epType == EP_TYPE_BULK))
        {
            transferWordLen = (uint16_t)((xferPipe->bufLen + 3) / 4);

            /* check FIFO space */
            if (transferWordLen > usbhh->usbGlobal->GNPTXFQSTS_B.NPTXFSA)
            {
                /* enable Non-periodic TxFIFO empty interrupt */
                USB_OTG_EnableGlobalInterrupt(usbhh->usbGlobal, USB_INT_G_NPTXFEM);
            }
        }
        else if ((xferPipe->epType == EP_TYPE_INTERRUPT) || \
                 (xferPipe->epType == EP_TYPE_ISO))
        {
            transferWordLen = (uint16_t)((xferPipe->bufLen + 3) / 4);

            /* check FIFO space */
            if (transferWordLen > usbhh->usbHost->HPTXSTS_B.FSPACE)
            {
                /* enable Non-periodic TxFIFO empty interrupt */
                USB_OTG_EnableGlobalInterrupt(usbhh->usbGlobal, USB_INT_G_PTXFE);
            }
        }
        else
        {
        }

        /* Write packet into FIFO */
        USB_OTG_FIFO_WriteFifoPacket(usbhh->usbFifo, xferPipe->chNum, \
                                     xferPipe->buffer, (uint16_t)xferPipe->bufLen, \
                                     usbhh->usbCfg.dmaStatus);
    }
}

/*!
 * @brief     USB host channel submit request handler
 *
 * @param     usbhh: USB host handler
 *
 * @param     channelNum : channel number
 *
 * @param     dir : channel direction
 *
 * @param     epType : endpoint type
 *
 * @param     tokenType : tokenType
 *
 * @param     buffer : URB data
 *
 * @param     length : length of URB data
 *
 * @param     pingStatus : ping status
 *
 * @retval    None
 */
void USBH_OTG_ChannelSubReq(USBH_HANDLE_T* usbhh, uint8_t channelNum, uint8_t dir, \
                            uint8_t epType, uint8_t tokenType, uint8_t* buffer, \
                            uint16_t length, uint8_t pingStatus)
{
    usbhh->xferPipe[channelNum].epDir   = (USB_EP_DIR_T)dir;
    usbhh->xferPipe[channelNum].epType  = epType;

    if (tokenType == USBH_PID_SETUP)
    {
        usbhh->xferPipe[channelNum].pid = USB_OTG_PID_SETUP;
        usbhh->xferPipe[channelNum].pingStatus = pingStatus;
    }
    /* token type = USBH_PID_DATA */
    else
    {
        usbhh->xferPipe[channelNum].pid = USB_OTG_PID_DATA1;
    }

    /* data toggle */
    switch (epType)
    {
        case EP_TYPE_CONTROL:
            if ((tokenType == USBH_PID_DATA) && (dir == EP_DIR_OUT))
            {
                if (length == 0)
                {
                    usbhh->xferPipe[channelNum].pidToggleOut = 1;
                }

                if (usbhh->xferPipe[channelNum].pidToggleOut)
                {
                    usbhh->xferPipe[channelNum].pid = USB_OTG_PID_DATA1;
                }
                else
                {
                    usbhh->xferPipe[channelNum].pid = USB_OTG_PID_DATA0;
                }
            }
            break;

        case EP_TYPE_BULK:
        case EP_TYPE_INTERRUPT:
            if (dir == EP_DIR_OUT)
            {
                if (usbhh->xferPipe[channelNum].pidToggleOut)
                {
                    usbhh->xferPipe[channelNum].pid = USB_OTG_PID_DATA1;
                }
                else
                {
                    usbhh->xferPipe[channelNum].pid = USB_OTG_PID_DATA0;
                }
            }
            else
            {
                if (usbhh->xferPipe[channelNum].pidToggleIn)
                {
                    usbhh->xferPipe[channelNum].pid = USB_OTG_PID_DATA1;
                }
                else
                {
                    usbhh->xferPipe[channelNum].pid = USB_OTG_PID_DATA0;
                }
            }
            break;

        case EP_TYPE_ISO:
            usbhh->xferPipe[channelNum].pid = USB_OTG_PID_DATA0;
            break;

        default:
            break;
    }

    usbhh->xferPipe[channelNum].buffer      = buffer;
    usbhh->xferPipe[channelNum].bufLen      = length;
    usbhh->xferPipe[channelNum].urbStatus   = USB_URB_IDLE;
    usbhh->xferPipe[channelNum].bufCount    = 0;
    usbhh->xferPipe[channelNum].chNum       = channelNum;
    usbhh->xferPipe[channelNum].pipeState   = PIPE_IDLE;

    /* Start transfer */
    USB_OTG_ChannelStartXfer(usbhh, &usbhh->xferPipe[channelNum]);
}

/*!
 * @brief     USB OTG host read size of last xfer
 *
 * @param     usbhh: USB host handler
 *
 * @param     channelNum: channel number
 *
 * @retval    xfer size
 */
uint32_t USBH_OTG_ReadXferSize(USBH_HANDLE_T* usbhh, uint8_t channelNum)
{
    uint32_t xferSize;

    xferSize = usbhh->xferPipe[channelNum].bufCount;

    return xferSize;
}

/*!
 * @brief     USB OTG host read toggle of channel
 *
 * @param     usbhh: USB host handler
 *
 * @param     channelNum: channel number
 *
 * @retval    xfer size
 */
uint8_t USBH_OTG_ReadToggle(USBH_HANDLE_T* usbhh, uint8_t channelNum)
{
    uint8_t toggle;

    if (usbhh->xferPipe[channelNum].epDir)
    {
        toggle = usbhh->xferPipe[channelNum].pidToggleIn;
    }
    else
    {
        toggle = usbhh->xferPipe[channelNum].pidToggleOut;
    }

    return toggle;
}

/*!
 * @brief     USB OTG host configure toggle of channel
 *
 * @param     usbhh: USB host handler
 *
 * @param     channelNum: channel number
 *
 * @param     toggle: toggle
 *
 * @retval    None
 */
void USBH_OTG_ConfigToggle(USBH_HANDLE_T* usbhh, uint8_t channelNum, uint8_t toggle)
{
    if (usbhh->xferPipe[channelNum].epDir)
    {
        usbhh->xferPipe[channelNum].pidToggleIn = toggle;
    }
    else
    {
        usbhh->xferPipe[channelNum].pidToggleOut = toggle;
    }
}

/*!
 * @brief     Config the USB host peripheral according to the specified parameters
 *
 * @param     usbhh: USB host handler
 *
 * @retval    None
 */
void USBH_Config(USBH_HANDLE_T* usbhh)
{
    uint8_t i;

    /* Embedded PHY */
    if (usbhh->usbCfg.phyType == USB_OTG_PHY_EMB)
    {
        /* Embedded FS PHY */
        if(usbhh->usbCfg.speed == USB_OTG_SPEED_FSLS)
        {
            USB_OTG_ConfigPHY(usbhh->usbGlobal, USB_OTG_PHY_SP_FS);

            /* Reset core */
            USB_OTG_CoreReset(usbhh->usbGlobal);
            delay_t(50);

            /* battery status */
            if (usbhh->usbCfg.batteryStatus == ENABLE)
            {
                /* Activate the power down*/
                USB_OTG_EnablePowerDown(usbhh->usbGlobal);
            }
            else
            {
                /* Deactivate the power down*/
                USB_OTG_DisablePowerDown(usbhh->usbGlobal);
            }
        }
        /* Embedded HS2 PHY */
        else
        {
            /* Configure HS2 */
            USB_OTG_HS2->USB_SWITCH_B.usb_switch = BIT_SET;
            USB_OTG_HS2->POWERON_CORE_B.poweron_core = BIT_SET;
            USB_OTG_HS2->OTG_SUSPENDM_B.otg_suspendm = BIT_SET;
            USB_OTG_HS2->SW_RREF_I2C_B.sw_rref_i2c = 0x05;
            
            USB_OTG_ConfigPHY(usbhh->usbGlobal, USB_OTG_PHY_SP_HS);

            /* Reset core */
            USB_OTG_CoreReset(usbhh->usbGlobal);
            delay_t(50);
            
            /* battery status */
            if (usbhh->usbCfg.batteryStatus == ENABLE)
            {
                /* Activate the power down*/
                USB_OTG_EnablePowerDown(usbhh->usbGlobal);
            }
            else
            {
                /* Deactivate the power down*/
                USB_OTG_DisablePowerDown(usbhh->usbGlobal);
            }
        }
    }
    /* External PHY */
    else
    {
        /* Activate the power down*/
        USB_OTG_EnablePowerDown(usbhh->usbGlobal);
        
        USB_OTG_ConfigPHY(usbhh->usbGlobal, USB_OTG_PHY_SP_HS);

        usbhh->usbGlobal->GUSBCFG_B.DPSEL       = BIT_RESET;
        usbhh->usbGlobal->GUSBCFG_B.ULPISEL     = BIT_RESET;

        usbhh->usbGlobal->GUSBCFG_B.ULPIEVDSEL  = BIT_RESET;
        usbhh->usbGlobal->GUSBCFG_B.ULPIEVC     = BIT_RESET;

        if (usbhh->usbCfg.extVbusStatus == ENABLE)
        {
            usbhh->usbGlobal->GUSBCFG_B.ULPIEVDSEL = BIT_SET;
        }
        
        /* Reset core */
        USB_OTG_CoreReset(usbhh->usbGlobal);
        delay_t(50);
    }

    if (usbhh->usbCfg.dmaStatus == ENABLE)
    {
        USB_OTG_SetBurstLenght(usbhh->usbGlobal, USB_OTG_BURST_INCR4);

        USB_OTG_EnableDMA(usbhh->usbGlobal);
    }

    /* Device mode or host mode */
    USB_OTG_ConfigMode(usbhh->usbGlobal, usbhh->usbCfg.mode);

    /* PHY Clock restart */
    usbhh->usbPower->PCGCTRL = 0;

    /* VBUS Sensing Disable */
    usbhh->usbGlobal->GGCCFG_B.VBSDIS = BIT_SET;
    usbhh->usbGlobal->GGCCFG_B.ADVBSEN = BIT_RESET;
    usbhh->usbGlobal->GGCCFG_B.BDVBSEN = BIT_RESET;

    /* Set host speed support */
    usbhh->usbHost->HCFG &= ~BIT2;

    /* Flush all Tx and Rx FIFOs */
    USB_OTG_FlushTxFIFO(usbhh->usbGlobal, 0x10);
    USB_OTG_FlushRxFIFO(usbhh->usbGlobal);

    /* Clear all Channel Interrupts */
    for (i = 0; i < usbhh->usbCfg.hostChannelNum; i++)
    {
        USB_OTG_H_ClearAllChannelIntFlag(usbhh->usbHost, i);
    }

    USB_OTG_DisableAllGlobalInterrupt(usbhh->usbGlobal);

    USB_OTG_ClearGlobalIntFlag(usbhh->usbGlobal, 0xFFFFFFFF);

    /* Configure RX FIFO */
    if (usbhh->usbCfg.speed == USB_OTG_SPEED_FSLS)
    {
        USB_OTG_ConfigRxFifoSize(usbhh->usbGlobal, USBH_FS_RX_FIFO_SIZE);
        usbhh->usbGlobal->GTXFCFG_H_B.NPTXSA       = USBH_FS_RX_FIFO_SIZE;
        usbhh->usbGlobal->GTXFCFG_H_B.NPTXFDEP     = USBH_FS_HNP_TXH_FIFO_SIZE;
        usbhh->usbGlobal->GHPTXFSIZE_B.HPDTXFSA    = USBH_FS_HP_TXH_FIFO_SA;
        usbhh->usbGlobal->GHPTXFSIZE_B.HPDTXFDEP   = USBH_FS_HP_TXH_FIFO_SIZE;
    }
    else
    {
        USB_OTG_ConfigRxFifoSize(usbhh->usbGlobal, USBH_HS_RX_FIFO_SIZE);
        usbhh->usbGlobal->GTXFCFG_H_B.NPTXSA       = USBH_HS_RX_FIFO_SIZE;
        usbhh->usbGlobal->GTXFCFG_H_B.NPTXFDEP     = USBH_HS_HNP_TXH_FIFO_SIZE;
        usbhh->usbGlobal->GHPTXFSIZE_B.HPDTXFSA    = USBH_HS_HP_TXH_FIFO_SA;
        usbhh->usbGlobal->GHPTXFSIZE_B.HPDTXFDEP   = USBH_HS_HP_TXH_FIFO_SIZE;
    }

    if (usbhh->usbCfg.dmaStatus == DISABLE)
    {
        usbhh->usbGlobal->GINTMASK |= USB_INT_G_RXFNONE;
    }

    /* Enable the common interrupts */
    usbhh->usbGlobal->GINTMASK |= (USB_INT_G_HPORT     |
                                   USB_INT_G_HCHAN     |
                                   USB_INT_G_SOF       |
                                   USB_INT_G_DEDIS     |
                                   USB_INT_G_IP_OUTTX  |
                                   USB_INT_G_RWAKE);
}

/*!
 * @brief       USB host URB status update event callback function
 *
 * @param       usbhh: USB host handler.
 *
 * @retval      None
 */
__weak void USBH_UpdateUrbCallback(USBH_HANDLE_T* usbhh)
{
    /* callback interface */
}

/*!
 * @brief       USB host connect event callback function
 *
 * @param       usbhh: USB host handler.
 *
 * @retval      None
 */
__weak void USBH_ConnectCallback(USBH_HANDLE_T* usbhh)
{
    /* callback interface */
}

/*!
 * @brief       USB host disconnect event callback function
 *
 * @param       usbhh: USB host handler.
 *
 * @retval      None
 */
__weak void USBH_DisconnectCallback(USBH_HANDLE_T* usbhh)
{
    /* callback interface */
}

/*!
 * @brief       USB host port enable event callback function
 *
 * @param       usbhh: USB host handler.
 *
 * @retval      None
 */
__weak void USBH_PortEnableCallback(USBH_HANDLE_T* usbhh)
{
    /* callback interface */
}

/*!
 * @brief       USB host port disable event callback function
 *
 * @param       usbhh: USB host handler.
 *
 * @retval      None
 */
__weak void USBH_PortDisableCallback(USBH_HANDLE_T* usbhh)
{
    /* callback interface */
}

/*!
 * @brief       USB host SOF event callback function
 *
 * @param       usbhh: USB host handler.
 *
 * @retval      None
 */
__weak void USBH_SOFCallback(USBH_HANDLE_T* usbhh)
{
    /* callback interface */
}

/*!
 * @brief       USB host hardware reset event callback function
 *
 * @param       usbhh: USB host handler.
 *
 * @retval      None
 */
__weak void USBH_HardwareResetCallback(USBH_HANDLE_T* usbhh)
{
    /* callback interface */
}

/**@} end of group USB_Host_Functions*/
/**@} end of group USB_Host_Driver*/
/**@} end of group APM32F4xx_StdPeriphDriver*/
