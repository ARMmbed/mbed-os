/*!
 * @file        apm32f4xx_usb_device.h
 *
 * @brief       USB device function handle
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
#include "apm32f4xx_usb_device.h"
#include "apm32f4xx_rcm.h"

/** @addtogroup APM32F4xx_StdPeriphDriver
  @{
*/

/** @addtogroup USB_Device_Driver USB Device Driver
  @{
*/

/** @defgroup USB_Device_Functions Functions
  @{
*/

void delay(int time)
{
    for(int i = 0; i < 1000; i++)
    {
        for(int j = 0; j < time; j++);
    }
}

/*!
 * @brief       Handle EP0 OUT SETUP transfer
 *
 * @param       usbdh: USB device handler.
 *
 * @param       dmaStatus: DMA status
 *
 * @param       setup: setup packet
 *
 * @retval      None
 */
void USBD_EP0_OutHandler(USBD_HANDLE_T* usbdh, uint8_t dmaStatus, uint8_t* setup)
{
    /* Configure EP0 */
    usbdh->usbDevice->EP_OUT[USBD_EP_0].DOEPTRS = 0;
    usbdh->usbDevice->EP_OUT[USBD_EP_0].DOEPTRS_B.EPPCNT = BIT_SET;
    usbdh->usbDevice->EP_OUT[USBD_EP_0].DOEPTRS_B.PID_SPCNT = 0x03;
    usbdh->usbDevice->EP_OUT[USBD_EP_0].DOEPTRS_B.EPTRS = 24;

    if (dmaStatus == ENABLE)
    {
        usbdh->usbDevice->EP_OUT[USBD_EP_0].DOEPDMA = (uint32_t)setup;

        /* Enable endpoint */
        usbdh->usbDevice->EP_OUT[USBD_EP_0].DOEPCTRL_B.USBAEP = BIT_SET;
        usbdh->usbDevice->EP_OUT[USBD_EP_0].DOEPCTRL_B.EPEN = BIT_SET;
    }
}

/*!
 * @brief       Handle EP OUT transfer complete interrupt
 *
 * @param       usbdh: USB device handler.
 *
 * @param       epNum: endpoint number
 *
 * @retval      None
 */
static void USBD_EP_OutXferHandler(USBD_HANDLE_T* usbdh, uint32_t epNum)
{
    uint8_t setupStatus = usbdh->usbDevice->EP_OUT[epNum].DOEPINT_B.SETPCMP;
    uint8_t rxOutDisStatus = usbdh->usbDevice->EP_OUT[epNum].DOEPINT_B.RXOTDIS;
    uint8_t xferSize = usbdh->usbDevice->EP_OUT[epNum].DOEPTRS_B.EPTRS;

    if (usbdh->usbCfg.dmaStatus == DISABLE)
    {
        /* Make sure is EP0 */
        if ((epNum == 0) && (usbdh->epOUT[epNum].bufLen == 0))
        {
            USBD_EP0_OutHandler(usbdh, DISABLE, (uint8_t*)usbdh->setup);
        }

        /* OUT data stage */
        USBD_DataOutStageCallback(usbdh, epNum);
    }
    else
    {
        /* SETUP stage is Done */
        if (setupStatus)
        {

        }
        else if (rxOutDisStatus)
        {
            usbdh->usbDevice->EP_OUT[epNum].DOEPINT_B.RXOTDIS = BIT_SET;
        }
        else if ((setupStatus | rxOutDisStatus) == 0)
        {
            /* receive OUT data packet on EP0 */
            usbdh->epOUT[epNum].bufCount = usbdh->epOUT[epNum].mps - xferSize;

            usbdh->epOUT[epNum].buffer += usbdh->epOUT[epNum].mps;

            /* Make sure is EP0 */
            if ((epNum == 0) && (usbdh->epOUT[epNum].bufLen == 0))
            {
                USBD_EP0_OutHandler(usbdh, ENABLE, (uint8_t*)usbdh->setup);
            }

            /* OUT data stage */
            USBD_DataOutStageCallback(usbdh, epNum);
        }
        else
        {

        }
    }
}

/*!
 * @brief       Handle EP OUT SETUP transfer interrupt
 *
 * @param       usbdh: USB device handler.
 *
 * @param       epNum: endpoint number
 *
 * @retval      None
 */
static void USBD_EP_OutSetupHandler(USBD_HANDLE_T* usbdh, uint32_t epNum)
{
    USBD_SetupStageCallback(usbdh);
}

/*!
 * @brief       Prepare empty TX FIFO
 *
 * @param       usbdh: USB device handler.
 *
 * @param       epNum: endpoint number
 *
 * @retval      None
 */
void USBD_EP_PushDataEmptyTxFifo(USBD_HANDLE_T* usbdh, uint8_t epNum)
{
    USB_OTG_ENDPOINT_INFO_T* ep;
    uint32_t lengthTemp;
    uint32_t length;

    ep = &usbdh->epIN[epNum];

    if (ep->bufCount > ep->bufLen)
    {
        return;
    }

    length = ep->bufLen - ep->bufCount;

    if (length > ep->mps)
    {
        length = ep->mps;
    }

    lengthTemp = (length + 3) / 4;

    while ((usbdh->usbDevice->EP_IN[epNum].DITXFSTS_B.INEPTXFSA >= lengthTemp) && \
            (ep->bufCount < ep->bufLen) && \
            (ep->bufLen != 0))
    {
        length = ep->bufLen - ep->bufCount;

        if (length > ep->mps)
        {
            length = ep->mps;
        }

        lengthTemp = (length + 3) / 4;

        USB_OTG_FIFO_WriteFifoPacket(usbdh->usbFifo, (uint8_t)epNum, ep->buffer, \
                                     (uint16_t)length, \
                                     usbdh->usbCfg.dmaStatus);

        ep->buffer      += length;
        ep->bufCount    += length;
    }

    if (ep->bufLen <= ep->bufCount)
    {
        USB_OTG_D_DisableInEpEmptyInterrupt(usbdh->usbDevice, epNum);
    }
}

/*!
 * @brief     USB device read EP last receive data size
 *
 * @param     usbdh: USB device handler.
 *
 * @param     epAddr: endpoint address
 *
 * @retval    size of last receive data
 */
uint32_t USBD_EP_ReadRxDataLen(USBD_HANDLE_T* usbdh, uint8_t epAddr)
{
    return usbdh->epOUT[epAddr & 0x0F].bufCount;
}

/*!
 * @brief     USB device EP start transfer
 *
 * @param     usbdh: USB device handler.
 *
 * @param     endpoint : endpoint handler
 *
 * @param     dmaStatus : DMA status
 *
 * @retval    None
 */
void USBD_EP_XferStart(USBD_HANDLE_T* usbdh, USB_OTG_ENDPOINT_INFO_T* endpoint, uint8_t dmaStatus)
{
    uint8_t epNum = endpoint->epNum;
    uint8_t epDir = endpoint->epDir;
    uint16_t epPacketCnt;

    if (epDir == EP_DIR_OUT)
    {
        usbdh->usbDevice->EP_OUT[epNum].DOEPTRS_B.EPPCNT = BIT_RESET;
        usbdh->usbDevice->EP_OUT[epNum].DOEPTRS_B.EPTRS = BIT_RESET;

        if (endpoint->bufLen)
        {
            epPacketCnt = (uint16_t)((endpoint->bufLen + endpoint->mps - 1) / (endpoint->mps));
            usbdh->usbDevice->EP_OUT[epNum].DOEPTRS_B.EPPCNT = epPacketCnt;
            usbdh->usbDevice->EP_OUT[epNum].DOEPTRS_B.EPTRS = endpoint->mps * epPacketCnt;
        }
        else
        {
            usbdh->usbDevice->EP_OUT[epNum].DOEPTRS_B.EPPCNT = 0x01;
            usbdh->usbDevice->EP_OUT[epNum].DOEPTRS_B.EPTRS = endpoint->mps;
        }

        if (dmaStatus == ENABLE)
        {
            if ((uint32_t)endpoint->buffer != 0)
            {
                usbdh->usbDevice->EP_OUT[epNum].DOEPDMA = (uint32_t)endpoint->buffer;
            }
        }

        if (endpoint->epType == EP_TYPE_ISO)
        {
            if (usbdh->usbDevice->DSTS_B.SOFNUM & 0x01)
            {
                usbdh->usbDevice->EP_OUT[epNum].DOEPCTRL_B.OFSET = BIT_SET;
            }
            else
            {
                usbdh->usbDevice->EP_OUT[epNum].DOEPCTRL_B.DPIDSET = BIT_SET;
            }
        }

        /* Enable endpoint */
        usbdh->usbDevice->EP_OUT[epNum].DOEPCTRL_B.NAKCLR = BIT_SET;
        usbdh->usbDevice->EP_OUT[epNum].DOEPCTRL_B.EPEN = BIT_SET;
    }
    else
    {
        /* Handle 0 length packet */
        if (endpoint->bufLen == 0)
        {
            usbdh->usbDevice->EP_IN[epNum].DIEPTRS_B.EPPCNT = BIT_RESET;
            usbdh->usbDevice->EP_IN[epNum].DIEPTRS_B.EPTRS = BIT_RESET;

            usbdh->usbDevice->EP_IN[epNum].DIEPTRS_B.EPPCNT = 0x01;
        }
        else
        {
            usbdh->usbDevice->EP_IN[epNum].DIEPTRS_B.EPPCNT = BIT_RESET;
            usbdh->usbDevice->EP_IN[epNum].DIEPTRS_B.EPTRS = BIT_RESET;

            epPacketCnt = (uint16_t)((endpoint->bufLen + endpoint->mps - 1) / (endpoint->mps));
            usbdh->usbDevice->EP_IN[epNum].DIEPTRS_B.EPPCNT = epPacketCnt;
            usbdh->usbDevice->EP_IN[epNum].DIEPTRS_B.EPTRS = endpoint->bufLen;

            if (endpoint->epType == EP_TYPE_ISO)
            {
                usbdh->usbDevice->EP_IN[epNum].DIEPTRS_B.TXDCNT = BIT_RESET;
                usbdh->usbDevice->EP_IN[epNum].DIEPTRS_B.TXDCNT = 0x01;
            }
        }

        if (dmaStatus == ENABLE)
        {
            if ((uint32_t)endpoint->dmaAddr != 0)
            {
                usbdh->usbDevice->EP_IN[epNum].DIEPDMA = (uint32_t)endpoint->dmaAddr;
            }

            if (endpoint->epType == EP_TYPE_ISO)
            {
                if (usbdh->usbDevice->DSTS_B.SOFNUM & 0x01)
                {
                    usbdh->usbDevice->EP_IN[epNum].DIEPCTRL_B.OFSET = BIT_SET;
                }
                else
                {
                    usbdh->usbDevice->EP_IN[epNum].DIEPCTRL_B.DPIDSET = BIT_SET;
                }
            }

            /* Enable endpoint */
            usbdh->usbDevice->EP_IN[epNum].DIEPCTRL_B.NAKCLR = BIT_SET;
            usbdh->usbDevice->EP_IN[epNum].DIEPCTRL_B.EPEN = BIT_SET;
        }
        else
        {
            /* Enable endpoint */
            usbdh->usbDevice->EP_IN[epNum].DIEPCTRL_B.NAKCLR = BIT_SET;
            usbdh->usbDevice->EP_IN[epNum].DIEPCTRL_B.EPEN = BIT_SET;

            if (endpoint->epType == EP_TYPE_ISO)
            {
                if (usbdh->usbDevice->DSTS_B.SOFNUM & 0x01)
                {
                    usbdh->usbDevice->EP_IN[epNum].DIEPCTRL_B.OFSET = BIT_SET;
                }
                else
                {
                    usbdh->usbDevice->EP_IN[epNum].DIEPCTRL_B.DPIDSET = BIT_SET;
                }

                USB_OTG_FIFO_WriteFifoPacket(usbdh->usbFifo, endpoint->epNum, \
                                             endpoint->buffer, endpoint->bufLen, \
                                             dmaStatus);
            }
            else
            {
                if (endpoint->bufLen)
                {
                    USB_OTG_D_EnableInEpEmptyInterrupt(usbdh->usbDevice, epNum);
                }
            }
        }
    }
}

/*!
 * @brief     USB device EP0 start transfer
 *
 * @param     usbdh: USB device handler.
 *
 * @param     endpoint : endpoint handler
 *
 * @param     dmaStatus : DMA status
 *
 * @retval    None
 */
void USBD_EP0_XferStart(USBD_HANDLE_T* usbdh, USB_OTG_ENDPOINT_INFO_T* endpoint, uint8_t dmaStatus)
{
    uint8_t epNum = endpoint->epNum;

    uint8_t epDir = endpoint->epDir;

    if (epDir == EP_DIR_OUT)
    {
        usbdh->usbDevice->EP_OUT[epNum].DOEPTRS_B.EPPCNT = BIT_RESET;
        usbdh->usbDevice->EP_OUT[epNum].DOEPTRS_B.EPTRS = BIT_RESET;

        if (endpoint->bufLen)
        {
            endpoint->bufLen = endpoint->mps;
        }

        usbdh->usbDevice->EP_OUT[epNum].DOEPTRS_B.EPPCNT = BIT_SET;
        usbdh->usbDevice->EP_OUT[epNum].DOEPTRS_B.EPTRS = endpoint->mps;

        if (dmaStatus == ENABLE)
        {
            if ((uint32_t)endpoint->buffer != 0)
            {
                usbdh->usbDevice->EP_OUT[epNum].DOEPDMA = (uint32_t)endpoint->buffer;
            }
        }

        /* Enable endpoint */
        usbdh->usbDevice->EP_OUT[epNum].DOEPCTRL_B.NAKCLR = BIT_SET;
        usbdh->usbDevice->EP_OUT[epNum].DOEPCTRL_B.EPEN = BIT_SET;
    }
    else
    {
        /* Handle 0 length packet */
        if (endpoint->bufLen == 0)
        {
            usbdh->usbDevice->EP_IN[epNum].DIEPTRS_B.EPPCNT = BIT_RESET;
            usbdh->usbDevice->EP_IN[epNum].DIEPTRS_B.EPTRS = BIT_RESET;

            usbdh->usbDevice->EP_IN[epNum].DIEPTRS_B.EPPCNT = 0x01;
        }
        else
        {
            usbdh->usbDevice->EP_IN[epNum].DIEPTRS_B.EPPCNT = BIT_RESET;
            usbdh->usbDevice->EP_IN[epNum].DIEPTRS_B.EPTRS = BIT_RESET;

            if (endpoint->bufLen > endpoint->mps)
            {
                endpoint->bufLen = endpoint->mps;
            }

            usbdh->usbDevice->EP_IN[epNum].DIEPTRS_B.EPPCNT = 0x01;
            usbdh->usbDevice->EP_IN[epNum].DIEPTRS_B.EPTRS = endpoint->bufLen;
        }

        if (dmaStatus == ENABLE)
        {
            if (endpoint->dmaAddr != 0)
            {
                usbdh->usbDevice->EP_IN[epNum].DIEPDMA = (uint32_t)(endpoint->dmaAddr);
            }

            /* Enable endpoint */
            usbdh->usbDevice->EP_IN[epNum].DIEPCTRL_B.NAKCLR = BIT_SET;
            usbdh->usbDevice->EP_IN[epNum].DIEPCTRL_B.EPEN = BIT_SET;
        }
        else
        {
            /* Enable endpoint */
            usbdh->usbDevice->EP_IN[epNum].DIEPCTRL_B.NAKCLR = BIT_SET;
            usbdh->usbDevice->EP_IN[epNum].DIEPCTRL_B.EPEN = BIT_SET;

            if (endpoint->bufLen)
            {
                USB_OTG_D_EnableInEpEmptyInterrupt(usbdh->usbDevice, epNum);
            }
        }
    }
}

/*!
 * @brief     USB device EP receive handler
 *
 * @param     usbdh: USB device handler.
 *
 * @param     epAddr : endpoint address
 *
 * @param     buffer : data buffer
 *
 * @param     length : length of data
 *
 * @retval    None
 */
void USBD_EP_Receive(USBD_HANDLE_T* usbdh, uint8_t epAddr, \
                     uint8_t* buffer, uint32_t length)
{
    uint8_t epAddrTemp = epAddr & 0x0F;

    usbdh->epOUT[epAddrTemp].epNum = epAddr & 0x0F;
    usbdh->epOUT[epAddrTemp].epDir = EP_DIR_OUT;

    usbdh->epOUT[epAddrTemp].buffer = buffer;
    usbdh->epOUT[epAddrTemp].bufCount = 0;
    usbdh->epOUT[epAddrTemp].bufLen = length;

    if (usbdh->usbCfg.dmaStatus == ENABLE)
    {
        usbdh->epOUT[epAddrTemp].dmaAddr = (uint32_t)buffer;
    }

    if (epAddr & 0x0F)
    {
        USBD_EP_XferStart(usbdh, &usbdh->epOUT[epAddrTemp], usbdh->usbCfg.dmaStatus);
    }
    else
    {
        USBD_EP0_XferStart(usbdh, &usbdh->epOUT[epAddrTemp], usbdh->usbCfg.dmaStatus);
    }
}

/*!
 * @brief     USB device EP transfer handler
 *
 * @param     usbdh: USB device handler.
 *
 * @param     epAddr : endpoint address
 *
 * @param     buffer : data buffer
 *
 * @param     length : length of data
 *
 * @retval    None
 */
void USBD_EP_Transfer(USBD_HANDLE_T* usbdh, uint8_t epAddr, \
                      uint8_t* buffer, uint32_t length)
{
    uint8_t epAddrTemp = epAddr & 0x0F;

    usbdh->epIN[epAddrTemp].epNum = epAddr & 0x0F;
    usbdh->epIN[epAddrTemp].epDir = EP_DIR_IN;

    usbdh->epIN[epAddrTemp].buffer = buffer;
    usbdh->epIN[epAddrTemp].bufCount = 0;
    usbdh->epIN[epAddrTemp].bufLen = length;

    if (usbdh->usbCfg.dmaStatus == ENABLE)
    {
        usbdh->epIN[epAddrTemp].dmaAddr = (uint32_t)buffer;
    }

    if (epAddr & 0x0F)
    {
        USBD_EP_XferStart(usbdh, &usbdh->epIN[epAddrTemp], usbdh->usbCfg.dmaStatus);
    }
    else
    {
        USBD_EP0_XferStart(usbdh, &usbdh->epIN[epAddrTemp], usbdh->usbCfg.dmaStatus);
    }
}

/*!
 * @brief     USB device flush EP handler
 *
 * @param     usbdh: USB device handler.
 *
 * @param     epAddr : endpoint address
 *
 * @retval    usb device status
 */
void USBD_EP_Flush(USBD_HANDLE_T* usbdh, uint8_t epAddr)
{
    if (epAddr & 0x80)
    {
        USB_OTG_FlushTxFIFO(usbdh->usbGlobal, (uint8_t)(epAddr & 0x0F));
    }
    else
    {
        USB_OTG_FlushRxFIFO(usbdh->usbGlobal);
    }
}

/*!
 * @brief       Handle RxFIFO no empty interrupt
 *
 * @param       usbdh: USB device handler.
 *
 * @retval      None
 */
static void USBD_RxFifoNoEmptyIsrHandler(USBD_HANDLE_T* usbdh)
{
    USBD_FIFO_STA_T fifoStatus;
    uint8_t epNum;
    uint8_t packetStatus;
    uint16_t packetCnt;

    /* Disable RxFIFO no empty Interrup */
    USB_OTG_DisableGlobalInterrupt(usbdh->usbGlobal, USB_INT_G_RXFNONE);

    /* Read and pop the RxFIFO status data */
    fifoStatus.FIFO_STATUS = USB_OTG_PopRxFifoStatus(usbdh->usbGlobal);
    epNum = fifoStatus.FIFO_STATUS_B.epNum;
    packetStatus = fifoStatus.FIFO_STATUS_B.packetStatus;
    packetCnt = fifoStatus.FIFO_STATUS_B.byteCount;

    switch (packetStatus)
    {
        case USBD_PKTSTS_G_OUT_NAK:
            break;

        case USBD_PKTSTS_REV_DATA_OUT_PKT:
            /* Receive OUT data packet */
            if ((packetCnt != 0) && (usbdh->epOUT[epNum].buffer != NULL))
            {
                USB_OTG_FIFO_ReadRxFifoPacket(usbdh->usbFifo, \
                                              usbdh->epOUT[epNum].buffer, \
                                              packetCnt);

                usbdh->epOUT[epNum].buffer += packetCnt;
                usbdh->epOUT[epNum].bufCount += packetCnt;
            }
            break;

        case USBD_PKTSTS_OUT_DONE:
            break;

        case USBD_PKTSTS_SETUP_DONE:
            break;

        case USBD_PKTSTS_REV_SETUP_PKT:
            USB_OTG_FIFO_ReadRxFifoPacket(usbdh->usbFifo, \
                                          (uint8_t*)usbdh->setup, \
                                          8);

            usbdh->epOUT[epNum].bufCount += packetCnt;
            break;

        default:
            break;
    }

    /* Enable RxFIFO no empty Interrupt */
    USB_OTG_EnableGlobalInterrupt(usbdh->usbGlobal, USB_INT_G_RXFNONE);
}

/*!
 * @brief       Handle OUT endpiont interrupt
 *
 * @param       usbdh: USB device handler.
 *
 * @retval      None
 */
static void USBD_OutEndpointIsrHandler(USBD_HANDLE_T* usbdh)
{
    uint16_t epIntBits;
    uint8_t epNum;

    /* Read all device endpoint interrupt bit */
    epIntBits = USB_OTG_D_ReadAllOutEpIntNumber(usbdh->usbDevice);

    for (epNum = 0; epNum < 16; epNum++)
    {
        if (!(epIntBits >> epNum))
        {
            break;
        }
        /* Find out all the interrupt OUT endpoint */
        else if ((epIntBits >> epNum) & 0x01)
        {
            /* Transfer completed */
            if (USB_OTG_D_ReadOutEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_OUT_TSFCMP))
            {
                USB_OTG_D_ClearOutEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_OUT_TSFCMP);

                /* Handle OUT transfer EP */
                USBD_EP_OutXferHandler(usbdh, epNum);
            }

            /* Setup completed */
            if (USB_OTG_D_ReadOutEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_OUT_SETPCMP))
            {
                USB_OTG_D_ClearOutEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_OUT_SETPCMP);

                /* Handle SETUP transfer */
                USBD_EP_OutSetupHandler(usbdh, epNum);
            }

            /* OUT token received when endpoint disabled */
            if (USB_OTG_D_ReadOutEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_OUT_RXOTDIS))
            {
                USB_OTG_D_ClearOutEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_OUT_RXOTDIS);
            }

            /* Endpoint disable */
            if (USB_OTG_D_ReadOutEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_OUT_EPDIS))
            {
                USB_OTG_D_ClearOutEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_OUT_EPDIS);
            }

            /* Received Back-to-back SETUP packets over 3 */
            if (USB_OTG_D_ReadOutEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_OUT_RXBSP))
            {
                USB_OTG_D_ClearOutEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_OUT_RXBSP);
            }
        }
    }
}

/*!
 * @brief       Handle IN endpiont interrupt
 *
 * @param       usbdh: USB device handler.
 *
 * @retval      None
 */
static void USBD_InEndpointIsrHandler(USBD_HANDLE_T* usbdh)
{
    uint16_t epIntBits;
    uint8_t epNum;

    /* Read all device endpoint interrupt bit */
    epIntBits = USB_OTG_D_ReadAllInEpIntNumber(usbdh->usbDevice);

    for (epNum = 0; epNum < 16; epNum++)
    {
        if (!(epIntBits >> epNum))
        {
            break;
        }
        /* Find out all the interrupt IN endpoint */
        else if ((epIntBits >> epNum) & 0x01)
        {
            /* Transfer completed */
            if (USB_OTG_D_ReadInEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_IN_TSFCMP))
            {
                USB_OTG_D_DisableInEpEmptyInterrupt(usbdh->usbDevice, epNum & 0x0F);

                USB_OTG_D_ClearInEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_IN_TSFCMP);

                if (usbdh->usbCfg.dmaStatus == ENABLE)
                {
                    usbdh->epIN[epNum].buffer += usbdh->epIN[epNum].mps;

                    /* Prepare EP0 for next setup */
                    if ((usbdh->epIN[epNum].bufLen == 0) && \
                            (epNum == USBD_EP_0))
                    {
                        /* prepare to receive next setup packets */
                        USBD_EP0_OutHandler(usbdh, usbdh->usbCfg.dmaStatus, (uint8_t*)usbdh->setup);
                    }
                }

                /* Callback DATA IN stage */
                USBD_DataInStageCallback(usbdh, epNum);
            }

            /* Timeout */
            if (USB_OTG_D_ReadInEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_IN_TO))
            {
                USB_OTG_D_ClearInEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_IN_TO);
            }

            /* IN token received when TxFIFO is empty */
            if (USB_OTG_D_ReadInEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_IN_ITXEMP))
            {
                USB_OTG_D_ClearInEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_IN_ITXEMP);
            }

            /* IN endpoint NAK effective */
            if (USB_OTG_D_ReadInEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_IN_IEPNAKE))
            {
                USB_OTG_D_ClearInEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_IN_IEPNAKE);
            }

            /* Endpoint disabled */
            if (USB_OTG_D_ReadInEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_IN_EPDIS))
            {
                USB_OTG_D_ClearInEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_IN_EPDIS);
            }

            /* TxFIFO empty */
            if (USB_OTG_D_ReadInEpIntStatus(usbdh->usbDevice, epNum, USBD_INT_EP_IN_TXFE))
            {
                USBD_EP_PushDataEmptyTxFifo(usbdh, epNum);
            }
        }
    }
}

/*!
 * @brief       Handle USB Resume interrupt
 *
 * @param       usbdh: USB device handler.
 *
 * @retval      None
 */
static void USBD_ResumeIsrHandler(USBD_HANDLE_T* usbdh)
{
    USB_OTG_D_DisableRemoteWakeupSignal(usbdh->usbDevice);

    /* Add LP mode handle */

    /* Resume Callback */
    USBD_ResumeCallback(usbdh);

    /* Clear interrupt */
    USB_OTG_ClearGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_RWAKE);
}

/*!
 * @brief       Handle USB Suspend interrupt
 *
 * @param       usbdh: USB device handler.
 *
 * @retval      None
 */
static void USBD_SuspendIsrHandler(USBD_HANDLE_T* usbdh)
{
    if (usbdh->usbDevice->DSTS_B.SUSSTS)
    {
        /* Suspend Callback */
        USBD_SuspendCallback(usbdh);
    }
    
    /* Clear interrupt */
    USB_OTG_ClearGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_USBSUS);
}

/*!
 * @brief       Handle USB Reset interrupt
 *
 * @param       usbdh: USB device handler.
 *
 * @retval      None
 */
static void USBD_ResetIsrHandler(USBD_HANDLE_T* usbdh)
{
    uint8_t i;

    USB_OTG_D_DisableRemoteWakeupSignal(usbdh->usbDevice);
    
    /* Flush all Tx FIFOs */
    USB_OTG_FlushTxFIFO(usbdh->usbGlobal, 0x10);

    /* Init endpoint */
    for (i = 0; i < usbdh->usbCfg.devEndpointNum; i++)
    {
        /* Set OUT endpoint */
        usbdh->usbDevice->EP_OUT[i].DOEPCTRL_B.STALLH = BIT_RESET;
        usbdh->usbDevice->EP_OUT[i].DOEPCTRL_B.NAKSET = BIT_SET;

        /* Clear endpoint Flag */
        usbdh->usbDevice->EP_OUT[i].DOEPINT = 0xFB7F;

        /* Set IN endpoint */
        usbdh->usbDevice->EP_IN[i].DIEPCTRL_B.STALLH = BIT_RESET;
        usbdh->usbDevice->EP_IN[i].DIEPCTRL_B.NAKSET = BIT_SET;

        /* Clear endpoint Flag */
        usbdh->usbDevice->EP_IN[i].DIEPINT = 0xFB7F;
    }

    /* Enable enpoint 0 global interrupt */
    USB_OTG_D_EnableOutEpInterruptMask(usbdh->usbDevice, USBD_EP_0);
    USB_OTG_D_EnableInEpInterruptMask(usbdh->usbDevice, USBD_EP_0);

    /* Only use in HS */
    if (usbdh->usbCfg.ep1Status == ENABLE)
    {
        /* IN EP1 */
        USB_OTG_D_EnableInEp1Interrupt(usbdh->usbDevice, \
                                       USBD_INT_EP_IN_TSFCMP    |
                                       USBD_INT_EP_IN_EPDIS     |
                                       USBD_INT_EP_IN_TO);



        /* OUT EP1 */
        USB_OTG_D_EnableOutEp1Interrupt(usbdh->usbDevice, \
                                        USBD_INT_EP_OUT_TSFCMP  |
                                        USBD_INT_EP_OUT_EPDIS   |
                                        USBD_INT_EP_OUT_SETPCMP);
    }
    else
    {
        /* IN EP */
        USB_OTG_D_EnableInEpInterrupt(usbdh->usbDevice, \
                                      USBD_INT_EP_IN_TSFCMP |
                                      USBD_INT_EP_IN_EPDIS  |
                                      USBD_INT_EP_IN_TO);

        /* OUT EP */
        USB_OTG_D_EnableOutEpInterrupt(usbdh->usbDevice, \
                                       USBD_INT_EP_OUT_TSFCMP   |
                                       USBD_INT_EP_OUT_EPDIS    |
                                       USBD_INT_EP_OUT_SETPCMP  |
                                       USBD_INT_EP_OUT_RXOTPR   |
                                       USBD_INT_EP_OUT_NAK);
    }

    /* Set device address to 0 */
    USB_OTG_D_ConfigDeviceAddress(usbdh->usbDevice, (0x00 & 0x7F));

    /* setup EP0 to receive SETUP packet */
    USBD_EP0_OutHandler(usbdh, usbdh->usbCfg.dmaStatus, (uint8_t*)usbdh->setup);

    /* Clear interrupt */
    USB_OTG_ClearGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_USBRST);
}

/*!
 * @brief       Handle USB Enum Done interrupt
 *
 * @param       usbdh: USB device handler.
 *
 * @retval      None
 */
static void USBD_EnumDoneIsrHandler(USBD_HANDLE_T* usbdh)
{
    uint32_t enumSpeed;

    /* Active EP0 for SETUP */
    usbdh->usbDevice->EP_IN[0].DIEPCTRL_B.MAXPS = USDB_EP0_MAXPS_64_BYTES;

    /* Clear all NAK */
    usbdh->usbDevice->DCTRL_B.GINAKCLR = BIT_SET;

    /* Store USB device speed */
    enumSpeed = USB_OTG_D_ReadEnumSpeed(usbdh->usbDevice);

    switch (enumSpeed)
    {
        case USBD_ENUM_SPEED_HS:
            usbdh->usbCfg.speed = USB_OTG_SPEED_HSFSLS;
            break;

        case USBD_ENUM_SPEED_HS_IN_FS:
        case USBD_ENUM_SPEED_FS:
            usbdh->usbCfg.speed = USB_OTG_SPEED_FSLS;
            break;

        default:
            /* Speed error status */
            usbdh->usbCfg.speed = 0xFF;
            break;
    }

    /* Set turnaround time */
    USB_OTG_SetTurnaroundTime(usbdh->usbGlobal, RCM_ReadHCLKFreq(), usbdh->usbCfg.speed);

    USBD_EnumDoneCallback(usbdh);

    /* Clear interrupt */
    USB_OTG_ClearGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_ENUMD);
}

/*!
 * @brief       Handle USB Start of Frame interrupt
 *
 * @param       usbdh: USB device handler.
 *
 * @retval      None
 */
static void USBD_SofIsrHanlder(USBD_HANDLE_T* usbdh)
{
    USBD_SOFCallback(usbdh);

    /* Clear interrupt */
    USB_OTG_ClearGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_SOF);
}

/*!
 * @brief       Handle USB incomplete ISO IN interrupt
 *
 * @param       usbdh: USB device handler.
 *
 * @retval      None
 */
static void USBD_IsoInInCompleteIsrHanlder(USBD_HANDLE_T* usbdh)
{
    uint8_t epNum = 0;

    USBD_IsoInInCompleteCallback(usbdh, epNum);

    /* Clear interrupt */
    USB_OTG_ClearGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_IIINTX);
}

/*!
 * @brief       Handle USB incomplete ISO OUT interrupt
 *
 * @param       usbdh: USB device handler.
 *
 * @retval      None
 */
static void USBD_IsoOutInCompleteIsrHanlder(USBD_HANDLE_T* usbdh)
{
    uint8_t epNum = 0;

    USBD_IsoOutInCompleteCallback(usbdh, epNum);

    /* Clear interrupt */
    USB_OTG_ClearGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_IP_OUTTX);
}

/*!
 * @brief       Handle USB New Session detected interrupt
 *
 * @param       usbdh: USB device handler.
 *
 * @retval      None
 */
static void USBD_NewSessionIsrHandler(USBD_HANDLE_T* usbdh)
{
    /* Connect callback */
    USBD_ConnectCallback(usbdh);

    /* Clear interrupt */
    USB_OTG_ClearGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_SREQ);
}

/*!
 * @brief       Handle USB OTG interrupt
 *
 * @param       usbdh: USB device handler.
 *
 * @retval      None
 */
static void USBD_OTGIsrHandler(USBD_HANDLE_T* usbdh)
{
    /* Session end */
    if (usbdh->usbGlobal->GINT_B.SEFLG == BIT_SET)
    {
        /* Disconnect callback */
        USBD_DisconnectCallback(usbdh);

        usbdh->usbGlobal->GINT_B.SEFLG = BIT_SET;
    }
}

/*!
 * @brief     Handle USB device global interrupt
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
void USBD_OTG_IsrHandler(USBD_HANDLE_T* usbdh)
{
    if (USB_OTG_ReadMode(usbdh->usbGlobal) == USB_OTG_MODE_DEVICE)
    {
        /* Avoid spurious interrupt */
        if (USB_OTG_ReadInterrupts(usbdh->usbGlobal) == 0U)
        {
            return;
        }

        /* Handle Mode Mismatch Interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_MMIS))
        {
            /* Clear interrupt */
            USB_OTG_ClearGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_MMIS);
        }

        /* Handle RxFIFO no empty interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_RXFNONE))
        {
            USBD_RxFifoNoEmptyIsrHandler(usbdh);
        }

        /* Handle OUT Endpoint interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_ONEP))
        {
            USBD_OutEndpointIsrHandler(usbdh);
        }

        /* Handle IN Endpoint interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_INEP))
        {
            USBD_InEndpointIsrHandler(usbdh);
        }

        /* Handle USB Reset interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_USBRST))
        {
            USBD_ResetIsrHandler(usbdh);
        }

        /* Handle Resume/remote wakeup detected interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_RWAKE))
        {
            USBD_ResumeIsrHandler(usbdh);
        }

        /* Handle USB suspend interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_USBSUS))
        {
            USBD_SuspendIsrHandler(usbdh);
        }

        /* Handle Enumeration done interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_ENUMD))
        {
            USBD_EnumDoneIsrHandler(usbdh);
        }

        /* Handle SOF interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_SOF))
        {
            USBD_SofIsrHanlder(usbdh);
        }

        /* Handle Incomplete ISO IN interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_IIINTX))
        {
            USBD_IsoInInCompleteIsrHanlder(usbdh);
        }

        /* Handle Incomplete ISO OUT interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_IP_OUTTX))
        {
            USBD_IsoOutInCompleteIsrHanlder(usbdh);
        }

        /* Handle Session request/new session detected interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_SREQ))
        {
            USBD_NewSessionIsrHandler(usbdh);
        }

        /* Handle OTG interrupt */
        if (USB_OTG_ReadGlobalIntFlag(usbdh->usbGlobal, USB_INT_G_OTG))
        {
            USBD_OTGIsrHandler(usbdh);
        }
    }
}

/*!
 * @brief     Config Depth and start address of TxFifo
 *
 * @param     usbdh: USB device handler
 *
 * @param     epInNum : IN EP number
 *
 * @param     depth : depth of TxFIFO
 *
 * @retval    None
 */
void USBD_OTG_ConfigDeviceTxFifo(USBD_HANDLE_T* usbdh, uint8_t epInNum, uint16_t depth)
{
    uint32_t txFifoConfig = (((uint32_t)depth) << 16);
    uint32_t txOffset;
    uint8_t i;

    txOffset = usbdh->usbGlobal->GRXFIFO;

    if (epInNum)
    {
        txOffset += (usbdh->usbGlobal->GTXFCFG) >> 16;

        for (i = 0; i < epInNum - 1; i++)
        {
            txOffset += (usbdh->usbGlobal->DTXFIFO[i].word >> 16);
        }

        usbdh->usbGlobal->DTXFIFO[epInNum - 1].word = txFifoConfig | txOffset;
    }
    else
    {
        usbdh->usbGlobal->GTXFCFG = txFifoConfig | txOffset;
    }
}

/*!
 * @brief     Config the USB device peripheral according to the specified parameters
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
void USBD_Config(USBD_HANDLE_T* usbdh)
{
    uint8_t i;
    
    /* Embedded PHY */
    if (usbdh->usbCfg.phyType == USB_OTG_PHY_EMB)
    {
        /* Embedded FS PHY */
        if(usbdh->usbCfg.speed == USB_OTG_SPEED_FSLS)
        {
            USB_OTG_ConfigPHY(usbdh->usbGlobal, USB_OTG_PHY_SP_FS);

            /* Reset core */
            USB_OTG_CoreReset(usbdh->usbGlobal);
            delay(50);

            /* battery status */
            if (usbdh->usbCfg.batteryStatus == ENABLE)
            {
                /* Activate the power down*/
                USB_OTG_EnablePowerDown(usbdh->usbGlobal);
            }
            else
            {
                /* Deactivate the power down*/
                USB_OTG_DisablePowerDown(usbdh->usbGlobal);
            }
        }
        /* Embedded HS2 PHY */
        else
        {
            USB_OTG_HS2->USB_SWITCH_B.usb_switch = BIT_SET;
            USB_OTG_HS2->POWERON_CORE_B.poweron_core = BIT_SET;
            USB_OTG_HS2->OTG_SUSPENDM_B.otg_suspendm = BIT_SET;
            USB_OTG_HS2->SW_RREF_I2C_B.sw_rref_i2c = 0x05;
            
            USB_OTG_ConfigPHY(usbdh->usbGlobal, USB_OTG_PHY_SP_HS);

            /* Reset core */
            USB_OTG_CoreReset(usbdh->usbGlobal);
            
            /* battery status */
            if (usbdh->usbCfg.batteryStatus == ENABLE)
            {
                /* Activate the power down*/
                USB_OTG_EnablePowerDown(usbdh->usbGlobal);
            }
            else
            {
                /* Deactivate the power down*/
                USB_OTG_DisablePowerDown(usbdh->usbGlobal);
            }
        }
    }
    /* External PHY */
    else
    {
        /* Activate the power down*/
        USB_OTG_EnablePowerDown(usbdh->usbGlobal);
        
        USB_OTG_ConfigPHY(usbdh->usbGlobal, USB_OTG_PHY_SP_HS);

        usbdh->usbGlobal->GUSBCFG_B.DPSEL       = BIT_RESET;
        usbdh->usbGlobal->GUSBCFG_B.ULPISEL     = BIT_RESET;

        usbdh->usbGlobal->GUSBCFG_B.ULPIEVDSEL  = BIT_RESET;
        usbdh->usbGlobal->GUSBCFG_B.ULPIEVC     = BIT_RESET;
        
        if (usbdh->usbCfg.extVbusStatus == ENABLE)
        {
            usbdh->usbGlobal->GUSBCFG_B.ULPIEVDSEL = BIT_SET;
        }
        
        /* Reset core */
        USB_OTG_CoreReset(usbdh->usbGlobal);
        delay(50);
    }

    if (usbdh->usbCfg.dmaStatus == ENABLE)
    {
        USB_OTG_SetBurstLenght(usbdh->usbGlobal, USB_OTG_BURST_INCR4);

        USB_OTG_EnableDMA(usbdh->usbGlobal);
    }

    /* Device mode or host mode */
    USB_OTG_ConfigMode(usbdh->usbGlobal, usbdh->usbCfg.mode);

    /* Init control endpoint structure */
    for (i = 0; i < usbdh->usbCfg.devEndpointNum; i++)
    {
        /* OUT control endpoint */
        usbdh->epOUT[i].epNum       = i;
        usbdh->epOUT[i].epDir       = EP_DIR_OUT;

        usbdh->epOUT[i].epType      = EP_TYPE_CONTROL;
        usbdh->epOUT[i].mps         = 0;
        usbdh->epOUT[i].buffer      = 0;
        usbdh->epOUT[i].bufLen      = 0;

        /* IN control endpoint */
        usbdh->epIN[i].epNum        = i;
        usbdh->epIN[i].epDir        = EP_DIR_IN;
        usbdh->epIN[i].txFifoNum    = i;

        usbdh->epIN[i].epType       = EP_TYPE_CONTROL;
        usbdh->epIN[i].mps          = 0;
        usbdh->epIN[i].buffer       = 0;
        usbdh->epIN[i].bufLen       = 0;
    }

    /* Init address */
    usbdh->address = 0;

    /* Init device control register */

    /* Clear IN endpoint FIFO */
    for (i = 0; i < 0x0F; i++)
    {
        usbdh->usbGlobal->DTXFIFO[i].word = 0;
    }

    /* Configure VBUS sense */
    if (usbdh->usbCfg.vbusSense == ENABLE)
    {
        usbdh->usbGlobal->GGCCFG_B.VBSDIS = BIT_RESET;
        usbdh->usbGlobal->GGCCFG_B.BDVBSEN = BIT_SET;
    }
    else
    {
        /* VBUS Sensing Disable */
        USB_OTG_D_ConfigSoftDisconnect(usbdh->usbDevice, USBD_SOFT_DISC_ENABLE);

        usbdh->usbGlobal->GGCCFG_B.VBSDIS = BIT_SET;
        usbdh->usbGlobal->GGCCFG_B.ADVBSEN = BIT_RESET;
        usbdh->usbGlobal->GGCCFG_B.BDVBSEN = BIT_RESET;
    }

    /* PHY Clock restart */
    usbdh->usbPower->PCGCTRL = 0;

    /* Configure device parameters */
    USB_OTG_D_ConfigPeriodicFrameInterval(usbdh->usbDevice, USBD_FRAME_INTERVAL_80);
    
    if(usbdh->usbCfg.phyType == USB_OTG_PHY_EMB)
    {
        if (usbdh->usbCfg.speed == USB_OTG_SPEED_FSLS)
        {
            if(usbdh->usbCfg.speedChannel == USBD_SPEED_CH_FS)
            {
                USB_OTG_D_ConfigDeviceSpeed(usbdh->usbDevice, USBD_DEV_SPEED_FS);
            }
            else
            {
                USB_OTG_D_ConfigDeviceSpeed(usbdh->usbDevice, USBD_DEV_SPEED_HS_IN_FS);
            }
        }
        else
        {
            USB_OTG_D_ConfigDeviceSpeed(usbdh->usbDevice, USBD_DEV_SPEED_HS);
        }
    }
    else
    {
        if (usbdh->usbCfg.speed == USB_OTG_SPEED_FSLS)
        {
            USB_OTG_D_ConfigDeviceSpeed(usbdh->usbDevice, USBD_DEV_SPEED_HS_IN_FS);
        }
        else
        {
            USB_OTG_D_ConfigDeviceSpeed(usbdh->usbDevice, USBD_DEV_SPEED_HS);
        }
    }

    /* Flush all Tx and Rx FIFOs */
    USB_OTG_FlushTxFIFO(usbdh->usbGlobal, 0x10);
    USB_OTG_FlushRxFIFO(usbdh->usbGlobal);

    USB_OTG_D_DisableAllInEpInterrupt(usbdh->usbDevice);
    USB_OTG_D_DisableAllOutEpInterrupt(usbdh->usbDevice);
    USB_OTG_D_DisableAllEpInterrupt(usbdh->usbDevice);

    /* Init endpoint */
    for (i = 0; i < usbdh->usbCfg.devEndpointNum; i++)
    {
        /* Reset OUT endpoint */
        if (usbdh->usbDevice->EP_OUT[i].DOEPCTRL_B.EPEN)
        {
            if (i != 0)
            {
                usbdh->usbDevice->EP_OUT[i].DOEPCTRL_B.EPDIS = BIT_SET;
            }

            usbdh->usbDevice->EP_OUT[i].DOEPCTRL_B.NAKSET = BIT_SET;
        }
        else
        {
            usbdh->usbDevice->EP_OUT[i].DOEPCTRL = 0;
        }

        /* Reset transfer size */
        usbdh->usbDevice->EP_OUT[i].DOEPTRS = 0;

        /* Clear endpoint Flag */
        usbdh->usbDevice->EP_OUT[i].DOEPINT = 0xFB7F;

        /* Reset IN endpoint */
        if (usbdh->usbDevice->EP_IN[i].DIEPCTRL_B.EPEN)
        {
            if (i != 0)
            {
                usbdh->usbDevice->EP_IN[i].DIEPCTRL_B.EPDIS = BIT_SET;
            }

            usbdh->usbDevice->EP_IN[i].DIEPCTRL_B.NAKSET = BIT_SET;
        }
        else
        {
            usbdh->usbDevice->EP_IN[i].DIEPCTRL = 0;
        }

        /* Reset transfer size */
        usbdh->usbDevice->EP_IN[i].DIEPTRS = 0;

        /* Clear endpoint Flag */
        usbdh->usbDevice->EP_IN[i].DIEPINT = 0xFB7F;
    }

    USB_OTG_D_DisableInEpInterrupt(usbdh->usbDevice, USBD_INT_EP_IN_TXFUDR);

    USB_OTG_DisableAllGlobalInterrupt(usbdh->usbGlobal);

    USB_OTG_ClearGlobalIntFlag(usbdh->usbGlobal, 0xBFFFFFFF);

    /* Configure FIFO */
    if (usbdh->usbCfg.speed == USB_OTG_SPEED_FSLS)
    {
        /* Configure RX FIFO */
        USB_OTG_ConfigRxFifoSize(usbdh->usbGlobal, USBD_FS_RX_FIFO_SIZE);

        /* Configure TX FIFO */
        USBD_OTG_ConfigDeviceTxFifo(usbdh, 0, USBD_FS_TX_FIFO_0_SIZE);
        USBD_OTG_ConfigDeviceTxFifo(usbdh, 1, USBD_FS_TX_FIFO_1_SIZE);
    }
    else
    {
        /* Configure RX FIFO */
        USB_OTG_ConfigRxFifoSize(usbdh->usbGlobal, USBD_HS_RX_FIFO_SIZE);

        /* Configure TX FIFO */
        USBD_OTG_ConfigDeviceTxFifo(usbdh, 0, USBD_HS_TX_FIFO_0_SIZE);
        USBD_OTG_ConfigDeviceTxFifo(usbdh, 1, USBD_HS_TX_FIFO_1_SIZE);
    }

    if (usbdh->usbCfg.dmaStatus == DISABLE)
    {
        usbdh->usbGlobal->GINTMASK |= USB_INT_G_RXFNONE;
    }

    if (usbdh->usbCfg.sofStatus == ENABLE)
    {
        usbdh->usbGlobal->GINTMASK |= USB_INT_G_SOF;
    }

    if (usbdh->usbCfg.vbusSense == ENABLE)
    {
        usbdh->usbGlobal->GINTMASK |= USB_INT_G_OTG | USB_INT_G_SREQ;
    }

    /* Enable the common interrupts */
    usbdh->usbGlobal->GINTMASK |= (USB_INT_G_USBSUS     |
                                   USB_INT_G_USBRST     |
                                   USB_INT_G_ENUMD      |
                                   USB_INT_G_INEP       |
                                   USB_INT_G_ONEP       |
                                   USB_INT_G_IIINTX     |
                                   USB_INT_G_IP_OUTTX   |
                                   USB_INT_G_RWAKE);

    /* Reset PHY, gate and PHY CLK */
    usbdh->usbPower->PCGCTRL_B.PCLKSTOP = BIT_RESET;
    usbdh->usbPower->PCGCTRL_B.GCLK = BIT_RESET;

    /* Enable soft disconnect */
    USB_OTG_D_ConfigSoftDisconnect(usbdh->usbDevice, USBD_SOFT_DISC_ENABLE);
}

/*!
 * @brief     USB device open EP
 *
 * @param     usbdh: USB device handler
 *
 * @param     epAddr: endpoint address
 *
 * @param     epType: endpoint type
 *
 * @param     epMps: endpoint maxinum of packet size
 *
 * @retval    None
 */
void USBD_EP_Open(USBD_HANDLE_T* usbdh, uint8_t epAddr, \
                  USB_EP_TYPE_T epType, uint16_t epMps)
{
    uint8_t epAddrTemp = epAddr & 0x0F;

    if ((epAddr & 0x80) == 0x80)
    {
        usbdh->epIN[epAddrTemp].epDir = EP_DIR_IN;

        usbdh->epIN[epAddrTemp].epNum   = epAddrTemp;
        usbdh->epIN[epAddrTemp].epType  = epType;
        usbdh->epIN[epAddrTemp].mps     = epMps;

        usbdh->epIN[epAddrTemp].txFifoNum = usbdh->epIN[epAddrTemp].epNum;
    }
    else
    {
        usbdh->epOUT[epAddrTemp].epDir = EP_DIR_OUT;

        usbdh->epOUT[epAddrTemp].epNum   = epAddrTemp;
        usbdh->epOUT[epAddrTemp].epType  = epType;
        usbdh->epOUT[epAddrTemp].mps     = epMps;
    }

    /* Init data PID */
    if (epType == EP_TYPE_BULK)
    {
        usbdh->epIN[epAddrTemp].dataPID = 0;
    }

    if ((epAddr & 0x80) == 0x80)
    {
        USB_OTG_D_ConfigEP(usbdh->usbDevice, &usbdh->epIN[epAddrTemp]);
    }
    else
    {
        USB_OTG_D_ConfigEP(usbdh->usbDevice, &usbdh->epOUT[epAddrTemp]);
    }
}

/*!
 * @brief     USB device close EP
 *
 * @param     usbdh: USB device handler
 *
 * @param     epAddr: endpoint address
 *
 * @retval    None
 */
void USBD_EP_Close(USBD_HANDLE_T* usbdh, uint8_t epAddr)
{
    uint8_t epAddrTemp = epAddr & 0x0F;

    if ((epAddr & 0x80) == 0x80)
    {
        usbdh->epIN[epAddrTemp].epDir = EP_DIR_IN;
    }
    else
    {
        usbdh->epOUT[epAddrTemp].epDir = EP_DIR_OUT;
    }

    if ((epAddr & 0x80) == 0x80)
    {
        USB_OTG_D_ResetEP(usbdh->usbDevice, &usbdh->epIN[epAddrTemp]);
    }
    else
    {
        USB_OTG_D_ResetEP(usbdh->usbDevice, &usbdh->epOUT[epAddrTemp]);
    }
}

/*!
 * @brief     USB device get EP stall status
 *
 * @param     usbdh: USB device handler
 *
 * @param     epAddr: endpoint address
 *
 * @retval    Stall status
 */
uint8_t USBD_EP_ReadStallStatus(USBD_HANDLE_T* usbdh, uint8_t epAddr)
{
    if ((epAddr & 0x80) == 0x80)
    {
        return (usbdh->epIN[epAddr & 0x7F].stallStatus);
    }
    else
    {
        return (usbdh->epOUT[epAddr & 0x7F].stallStatus);
    }
}

/*!
 * @brief     USB device set EP on stall status
 *
 * @param     usbdh: USB device handler
 *
 * @param     epAddr: endpoint address
 *
 * @retval    None
 */
void USBD_EP_ClearStall(USBD_HANDLE_T* usbdh, uint8_t epAddr)
{
    USB_OTG_ENDPOINT_INFO_T* ep;

    if ((epAddr & 0x0F) > usbdh->usbCfg.devEndpointNum)
    {
        return;
    }

    if ((epAddr & 0x80) == 0x80)
    {
        ep = &usbdh->epIN[epAddr & 0x0F];
        ep->epDir = EP_DIR_IN;
    }
    else
    {
        ep = &usbdh->epOUT[epAddr & 0x0F];
        ep->epDir = EP_DIR_OUT;
    }

    ep->stallStatus = DISABLE;
    ep->epNum = epAddr & 0x0F;

    if (ep->epDir == EP_DIR_IN)
    {
        usbdh->usbDevice->EP_IN[ep->epNum].DIEPCTRL_B.STALLH = BIT_RESET;

        if ((ep->epType == EP_TYPE_BULK) || (ep->epType == EP_TYPE_INTERRUPT))
        {
            usbdh->usbDevice->EP_IN[ep->epNum].DIEPCTRL_B.DPIDSET = BIT_SET;
        }
    }
    else
    {
        usbdh->usbDevice->EP_OUT[ep->epNum].DOEPCTRL_B.STALLH = BIT_RESET;

        if ((ep->epType == EP_TYPE_BULK) || (ep->epType == EP_TYPE_INTERRUPT))
        {
            usbdh->usbDevice->EP_OUT[ep->epNum].DOEPCTRL_B.DPIDSET = BIT_SET;
        }
    }
}

/*!
 * @brief     USB device set device address
 *
 * @param     usbdh: USB device handler
 *
 * @param     address: address
 *
 * @retval    None
 */
void USBD_SetDevAddress(USBD_HANDLE_T* usbdh, uint8_t address)
{
    usbdh->address = address;

    USB_OTG_D_ConfigDeviceAddress(usbdh->usbDevice, address);
}

/*!
 * @brief     USB device set EP on stall status
 *
 * @param     usbdh: USB device handler
 *
 * @param     epAddr: endpoint address
 *
 * @retval    None
 */
void USBD_EP_Stall(USBD_HANDLE_T* usbdh, uint8_t epAddr)
{
    USB_OTG_ENDPOINT_INFO_T* ep;

    if ((epAddr & 0x0F) > usbdh->usbCfg.devEndpointNum)
    {
        return;
    }

    if ((epAddr & 0x80) == 0x80)
    {
        ep = &usbdh->epIN[epAddr & 0x0F];
        ep->epDir = EP_DIR_IN;
    }
    else
    {
        ep = &usbdh->epOUT[epAddr & 0x0F];
        ep->epDir = EP_DIR_OUT;
    }

    ep->stallStatus = ENABLE;
    ep->epNum = epAddr & 0x0F;

    if (ep->epDir == EP_DIR_IN)
    {
        if ((ep->epNum != 0) && \
                (usbdh->usbDevice->EP_IN[ep->epNum].DIEPCTRL_B.EPEN == 0))
        {
            usbdh->usbDevice->EP_IN[ep->epNum].DIEPCTRL_B.EPDIS = BIT_RESET;
        }

        usbdh->usbDevice->EP_IN[ep->epNum].DIEPCTRL_B.STALLH = BIT_SET;
    }
    else
    {
        if ((ep->epNum != 0) && \
                (usbdh->usbDevice->EP_OUT[ep->epNum].DOEPCTRL_B.EPEN == 0))
        {
            usbdh->usbDevice->EP_OUT[ep->epNum].DOEPCTRL_B.EPDIS = BIT_RESET;
        }

        usbdh->usbDevice->EP_OUT[ep->epNum].DOEPCTRL_B.STALLH = BIT_SET;
    }
}

/*!
 * @brief     USB device start
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
void USBD_Start(USBD_HANDLE_T* usbdh)
{
    if ((usbdh->usbCfg.batteryStatus == ENABLE) && \
            (usbdh->usbCfg.phyType != USB_OTG_PHY_EX))
    {
        USB_OTG_DisablePowerDown(usbdh->usbGlobal);
    }

    /* Enable USB OTG all global interrupt */
    USB_OTG_EnableAllGlobalInterrupt(usbdh->usbGlobal);

    /* Reset PHY, gate and PHY CLK */
    usbdh->usbPower->PCGCTRL_B.PCLKSTOP = BIT_RESET;
    usbdh->usbPower->PCGCTRL_B.GCLK = BIT_RESET;

    /* Disable soft disconnect */
    USB_OTG_D_ConfigSoftDisconnect(usbdh->usbDevice, USBD_SOFT_DISC_NORMAL);
}

/*!
 * @brief     USB device stop
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
void USBD_Stop(USBD_HANDLE_T* usbdh)
{
    /* Enable USB OTG all global interrupt */
    USB_OTG_DisableAllGlobalInterrupt(usbdh->usbGlobal);
    
    /* Reset PHY, gate and PHY CLK */
    usbdh->usbPower->PCGCTRL_B.PCLKSTOP = BIT_RESET;
    usbdh->usbPower->PCGCTRL_B.GCLK = BIT_RESET;
    
    /* Enable soft disconnect */
    USB_OTG_D_ConfigSoftDisconnect(usbdh->usbDevice, USBD_SOFT_DISC_ENABLE);
    
    USB_OTG_FlushTxFIFO(usbdh->usbGlobal, 0x10);
    
    if((usbdh->usbCfg.batteryStatus == ENABLE) && \
       (usbdh->usbCfg.phyType != USB_OTG_PHY_EX))
    {
        USB_OTG_EnablePowerDown(usbdh->usbGlobal);
    }
}

/*!
 * @brief     USB device stop
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
void USBD_StopDevice(USBD_HANDLE_T* usbdh)
{
    uint8_t i;
    
    for(i = 0; i < 0x08; i++)
    {
        /* Clear endpoint Flag */
        usbdh->usbDevice->EP_OUT[i].DOEPINT = 0xFB7F;
        /* Clear endpoint Flag */
        usbdh->usbDevice->EP_IN[i].DIEPINT = 0xFB7F;
    }
    
    usbdh->usbDevice->DINIMASK = 0;
    usbdh->usbDevice->DOUTIMASK = 0;
    usbdh->usbDevice->DAEPIMASK = 0;
    
    USB_OTG_FlushRxFIFO(usbdh->usbGlobal);
    
    USB_OTG_FlushTxFIFO(usbdh->usbGlobal, 0x10);
}

/*!
 * @brief     USB OTG device resume callback
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
__weak void USBD_ResumeCallback(USBD_HANDLE_T* usbdh)
{
    /* callback interface */
}

/*!
 * @brief     USB OTG device suspend callback
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
__weak void USBD_SuspendCallback(USBD_HANDLE_T* usbdh)
{
    /* callback interface */
}

/*!
 * @brief     USB OTG device enum done callback
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
__weak void USBD_EnumDoneCallback(USBD_HANDLE_T* usbdh)
{
    /* callback interface */
}

/*!
 * @brief     USB OTG device SETUP stage callback
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
__weak void USBD_SetupStageCallback(USBD_HANDLE_T* usbdh)
{
    /* callback interface */
}

/*!
 * @brief     USB OTG device data IN stage callback
 *
 * @param     usbdh: USB device handler
 *
 * @param     epNum: endpoint number
 *
 * @retval    None
 */
__weak void USBD_DataInStageCallback(USBD_HANDLE_T* usbdh, uint8_t epNum)
{
    /* callback interface */
}

/*!
 * @brief     USB OTG device data OUT stage callback
 *
 * @param     usbdh: USB device handler
 *
 * @param     epNum: endpoint number
 *
 * @retval    None
 */
__weak void USBD_DataOutStageCallback(USBD_HANDLE_T* usbdh, uint8_t epNum)
{
    /* callback interface */
}

/*!
 * @brief       USB OTG device SOF event callback function
 *
 * @param       usbhh: USB host handler.
 *
 * @retval      None
 */
__weak void USBD_SOFCallback(USBD_HANDLE_T* usbdh)
{
    /* callback interface */
}

/*!
 * @brief     USB OTG device ISO IN in complete callback
 *
 * @param     usbdh: USB device handler
 *
 * @param     epNum: endpoint number
 *
 * @retval    None
 */
__weak void USBD_IsoInInCompleteCallback(USBD_HANDLE_T* usbdh, uint8_t epNum)
{
    /* callback interface */
}

/*!
 * @brief     USB OTG device ISO OUT in complete callback
 *
 * @param     usbdh: USB device handler
 *
 * @param     epNum: endpoint number
 *
 * @retval    None
 */
__weak void USBD_IsoOutInCompleteCallback(USBD_HANDLE_T* usbdh, uint8_t epNum)
{
    /* callback interface */
}

/*!
 * @brief     USB OTG device connect callback
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
__weak void USBD_ConnectCallback(USBD_HANDLE_T* usbdh)
{
    /* callback interface */
}

/*!
 * @brief     USB OTG device disconnect callback
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
__weak void USBD_DisconnectCallback(USBD_HANDLE_T* usbdh)
{
    /* callback interface */
}

/**@} end of group USB_Device_Functions*/
/**@} end of group USB_Device_Driver*/
/**@} end of group APM32F4xx_StdPeriphDriver*/


