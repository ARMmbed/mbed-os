/*!
 * @file        apm32f4xx_usb.c
 *
 * @brief       This file provides all the USB dirver functions
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
#include "apm32f4xx_usb.h"

/** @addtogroup APM32F4xx_StdPeriphDriver
  @{
*/

/** @addtogroup USB_Driver USB Driver
  @{
*/

/** @defgroup USB_Functions Functions
  @{
*/

/*!
 * @brief     Pop the RxFIFO status with data
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @retval    Rx fifo status
 */
uint32_t USB_OTG_PopRxFifoStatus(USB_OTG_GLOBAL_T* usbx)
{
    return ((uint32_t)usbx->GRXSTSP);
}

/*!
 * @brief     Config HS PHY or FS Serial Transceiver.
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @param     phy : Select PHY.
 *                    This value can be one of the following values:
 *                    @arg USB_OTG_PHY_SP_FS : FS Serial Transceiver
 *                    @arg USB_OTG_PHY_SP_HS : HS PHY
 *
 * @retval    None
 */
void USB_OTG_ConfigPHY(USB_OTG_GLOBAL_T* usbx, USB_OTG_PHY_SPEED_T phy)
{
    /* Select PHY */
    usbx->GUSBCFG_B.FSSTSEL = phy;
}

/*!
 * @brief     Read USB OTG current work mode
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @retval    USB OTG work mode
 */
uint8_t USB_OTG_ReadMode(USB_OTG_GLOBAL_T* usbx)
{
    return (uint8_t)usbx->GCINT_B.CURMOSEL;
}

/*!
 * @brief     Config size of RxFifo
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @param     size : value of depth(16 - 256 words)
 *
 * @retval    None
 */
void USB_OTG_ConfigRxFifoSize(USB_OTG_GLOBAL_T* usbx, uint16_t size)
{
    usbx->GRXFIFO_B.RXFDEP = size;
}

/*!
 * @brief     Enable the specified Global interrupt
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @param     interrupt: interrupt
 *
 * @retval    None
 */
void USB_OTG_EnableGlobalInterrupt(USB_OTG_GLOBAL_T* usbx, uint32_t interrupt)
{
    usbx->GINTMASK |= (uint32_t)(interrupt);
}

/*!
 * @brief     Disable the specified Global interrupt
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @param     interrupt: interrupt
 *
 * @retval    None
 */
void USB_OTG_DisableGlobalInterrupt(USB_OTG_GLOBAL_T* usbx, uint32_t interrupt)
{
    usbx->GINTMASK &= (~(uint32_t)(interrupt));
}

/*!
 * @brief     Read USB OTG HS DMA enable status
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @retval    None
 */
uint8_t USB_OTG_ReadDMAStatus(USB_OTG_GLOBAL_T* usbx)
{
    return (usbx->GAHBCFG_B.DMAEN);
}

/*!
 * @brief     Enable All the Global interrupt
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @retval    None
 */
void USB_OTG_EnableAllGlobalInterrupt(USB_OTG_GLOBAL_T* usbx)
{
    usbx->GAHBCFG_B.GINTMASK = BIT_SET;
}

/*!
 * @brief     Disable All the Global interrupt
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @retval    None
 */
void USB_OTG_DisableAllGlobalInterrupt(USB_OTG_GLOBAL_T* usbx)
{
    usbx->GAHBCFG_B.GINTMASK = BIT_RESET;
}

/*!
 * @brief     Read the specified global interrupt flag
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @param     interrupt: interrupt
 *
 * @retval    Flag status. 1 to set and 0 to not set
 */
uint8_t USB_OTG_ReadGlobalIntFlag(USB_OTG_GLOBAL_T* usbx, uint32_t interrupt)
{
    uint32_t temp;

    temp = usbx->GCINT;
    
    temp &= usbx->GINTMASK;

    return ((temp & (uint32_t)(interrupt)) ? SET : RESET);
}

/*!
 * @brief     Read all global interrupt flag
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @retval    Match status
 */
uint32_t USB_OTG_ReadInterrupts(USB_OTG_GLOBAL_T* usbx)
{
    uint32_t temp;

    temp = usbx->GCINT;
    temp &= usbx->GINTMASK;

    return temp;
}

/*!
 * @brief     Clear the specified global interrupt flag(Write 1 to clear)
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @param     interrupt
 *
 * @retval    None
 */
void USB_OTG_ClearGlobalIntFlag(USB_OTG_GLOBAL_T* usbx, uint32_t interrupt)
{
    usbx->GCINT = interrupt;
}

/*!
 * @brief     Flush specific TxFIFO
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @param     num: TxFIFO number(max:16)
 *
 * @retval    None
 */
void USB_OTG_FlushTxFIFO(USB_OTG_GLOBAL_T* usbx, uint8_t num)
{
    __IO uint32_t timeout = 0;

    /* Wait for AHB master idle */
    while (usbx->GRSTCTRL_B.AHBMIDL == BIT_RESET)
    {
        if (++timeout > 0x30000)
        {
            break;
        }
    }

    usbx->GRSTCTRL_B.TXFNUM = num;
    usbx->GRSTCTRL_B.TXFFLU = BIT_SET;

    timeout = 0;
    while (usbx->GRSTCTRL_B.TXFFLU != BIT_RESET)
    {
        if (++timeout > 0x30000)
        {
            break;
        }
    }
}

/*!
 * @brief     Flush RxFIFO
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @retval    None
 */
void USB_OTG_FlushRxFIFO(USB_OTG_GLOBAL_T* usbx)
{
    __IO uint32_t timeout = 0;

    usbx->GRSTCTRL = 0;
    usbx->GRSTCTRL_B.RXFFLU = BIT_SET;

    while (usbx->GRSTCTRL_B.RXFFLU != BIT_RESET)
    {
        if (++timeout > 0x30000)
        {
            break;
        }
    }
}

/*!
 * @brief     USB core Soft reset
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @retval    None
 */
void USB_OTG_CoreReset(USB_OTG_GLOBAL_T* usbx)
{
    __IO uint32_t timeout = 0;

    /* Wait for AHB master idle */
    while (usbx->GRSTCTRL_B.AHBMIDL == BIT_RESET)
    {
        if (++timeout > 0x30000)
        {
            return;
        }
    }

    timeout = 0;

    usbx->GRSTCTRL_B.CSRST = BIT_SET;

    /* Wait for Reset ok */
    while (usbx->GRSTCTRL_B.CSRST != BIT_RESET)
    {
        if (++timeout > 0x30000)
        {
            return;
        }
    }
}

/*!
 * @brief     Enable power down.
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @retval    None
 */
void USB_OTG_EnablePowerDown(USB_OTG_GLOBAL_T* usbx)
{
    usbx->GGCCFG_B.PWEN = BIT_RESET;
}

/*!
 * @brief     Disable power down.
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @retval    None
 */
void USB_OTG_DisablePowerDown(USB_OTG_GLOBAL_T* usbx)
{
    usbx->GGCCFG_B.PWEN = BIT_SET;
}

/*!
 * @brief     Enable USB OTG DMA
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @retval    None
 *
 * @note      Available only in HS mode
 */
void USB_OTG_EnableDMA(USB_OTG_GLOBAL_T* usbx)
{
    usbx->GAHBCFG_B.DMAEN = BIT_SET;
}

/*!
 * @brief     Set USB OTG burst length
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @param     burstType: Select the burst type
 *
 * @retval    None
 *
 * @note      Available only in HS mode
 */
void USB_OTG_SetBurstLenght(USB_OTG_GLOBAL_T* usbx, USB_OTG_BURST_T burstType)
{
    usbx->GAHBCFG_B.BLT = burstType;
}

/*!
 * @brief     Configure turnaround time
 *
 * @param     usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @param     hclkFreq: HCLK frequency
 *
 * @param     speed: device speed
 *
 * @retval    None
 */
void USB_OTG_SetTurnaroundTime(USB_OTG_GLOBAL_T* usbx, uint32_t hclkFreq, uint8_t speed)
{
    uint8_t turnaroundTime;
    uint32_t hclkFreqTemp = (uint32_t)hclkFreq / 100000;

    switch (speed)
    {
        case USB_OTG_SPEED_HSFSLS:
            turnaroundTime = USBD_HS_TRDT_VAL;
            break;

        case USB_OTG_SPEED_FSLS:
            if ((hclkFreqTemp >= 142) && (hclkFreqTemp < 150))
            {
                turnaroundTime = 0x0F;
            }
            else if ((hclkFreqTemp >= 150) && (hclkFreqTemp < 160))
            {
                turnaroundTime = 0x0E;
            }
            else if ((hclkFreqTemp >= 160) && (hclkFreqTemp < 172))
            {
                turnaroundTime = 0x0D;
            }
            else if ((hclkFreqTemp >= 172) && (hclkFreqTemp < 185))
            {
                turnaroundTime = 0x0C;
            }
            else if ((hclkFreqTemp >= 185) && (hclkFreqTemp < 200))
            {
                turnaroundTime = 0x0B;
            }
            else if ((hclkFreqTemp >= 200) && (hclkFreqTemp < 218))
            {
                turnaroundTime = 0x0A;
            }
            else if ((hclkFreqTemp >= 218) && (hclkFreqTemp < 240))
            {
                turnaroundTime = 0x09;
            }
            else if ((hclkFreqTemp >= 240) && (hclkFreqTemp < 277))
            {
                turnaroundTime = 0x08;
            }
            else if ((hclkFreqTemp >= 277) && (hclkFreqTemp < 320))
            {
                turnaroundTime = 0x07;
            }
            else
            {
                turnaroundTime = 0x06;
            }
            break;

        default:
            turnaroundTime = USBD_DEFAULT_TRDT_VAL;
            break;
    }

    usbx->GUSBCFG_B.TRTIM = (turnaroundTime) & 0x0F;
}

/*!
 * @brief       Force USB to device ot host mode.
 *
 * @param       usbx: Select the USB port.
 *              @arg USB_OTG_FS
 *              @arg USB_OTG_HS
 *
 * @param       mode : Select mode.
 *                    This value can be one of the following values:
 *                    @arg USB_MODE_DEVICE : Force device mode.
 *                    @arg USB_MODE_HOST   : Force host mode.
 *
 * @retval      None
 */
void USB_OTG_ConfigMode(USB_OTG_GLOBAL_T* usbx, USB_OTG_MODE_T mode)
{
    uint32_t timeout = 0;

    usbx->GUSBCFG_B.FHMODE = BIT_RESET;
    usbx->GUSBCFG_B.FDMODE = BIT_RESET;

    if (mode == USB_OTG_MODE_HOST)
    {
        usbx->GUSBCFG_B.FHMODE = BIT_SET;

        while (USB_OTG_ReadMode(usbx) != USB_OTG_MODE_HOST)
        {
            timeout++;

            if (timeout >= 0x1FFFF)
            {
                break;
            }
        }
    }
    else
    {
        usbx->GUSBCFG_B.FDMODE = BIT_SET;

        while (USB_OTG_ReadMode(usbx) != USB_OTG_MODE_DEVICE)
        {
            timeout++;

            if (timeout >= 0x1FFFF)
            {
                break;
            }
        }
    }
}

/*!
 * @brief     Enable port power
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @retval    None
 */
void USB_OTG_H_EnablePortPower(USB_OTG_HOST_T* usbhx)
{
    usbhx->HPORTCSTS_B.PP = BIT_SET;
}

/*!
 * @brief     Disable port power
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @retval    None
 */
void USB_OTG_H_DisablePortPower(USB_OTG_HOST_T* usbhx)
{
    usbhx->HPORTCSTS_B.PP = BIT_RESET;
}

/*!
 * @brief     Enable low speed device
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum:  channel number
 *
 * @retval    None
 */
void USB_OTG_H_EnableLowSpeedDevice(USB_OTG_HOST_T* usbhx, uint8_t chNum)
{
    usbhx->REGS_HCH[chNum].HCH_B.LSDV = BIT_SET;
}

/*!
 * @brief     Disable low speed device
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum:  channel number
 *
 * @retval    None
 */
void USB_OTG_H_DisableLowSpeedDevice(USB_OTG_HOST_T* usbhx, uint8_t chNum)
{
    usbhx->REGS_HCH[chNum].HCH_B.LSDV = BIT_RESET;
}

/*!
 * @brief     Set host channel device address
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum:  channel number
 *
 * @param     devAddr: device address
 *
 * @retval    None
 */
void USB_OTG_H_ConfigDeviceAddr(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint8_t devAddr)
{
    usbhx->REGS_HCH[chNum].HCH_B.DVADDR = (devAddr & 0x7F);
}

/*!
 * @brief     Set host channel endpoint type
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum:  channel number
 *
 * @param     epType: endpoint type
 *
 * @retval    None
 */
void USB_OTG_H_ConfigEpType(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint8_t epType)
{
    usbhx->REGS_HCH[chNum].HCH_B.EDPTYP = (epType & 0x03);
}

/*!
 * @brief     Set host channel packet max size
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum:  channel number
 *
 * @param     packetMaxSize: packet data max size
 *
 * @retval    None
 */
void USB_OTG_H_ConfigEpMaxPSize(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint16_t packetMaxSize)
{
    usbhx->REGS_HCH[chNum].HCH_B.MAXPSIZE = (packetMaxSize & 0x7FF);
}

/*!
 * @brief     Set host channel endpoint number
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum:  channel number
 *
 * @param     epNum:  endpoint number
 *
 * @retval    None
 */
void USB_OTG_H_ConfigEpNum(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint8_t epNum)
{
    usbhx->REGS_HCH[chNum].HCH_B.EDPNUM = (epNum & 0x7F);
}

/*!
 * @brief     Use odd frame to transfer
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum: channel number
 *
 * @retval    None
 */
void USB_OTG_H_TxUseOddFrame(USB_OTG_HOST_T* usbhx, uint8_t chNum)
{
    usbhx->REGS_HCH[chNum].HCH_B.ODDF = BIT_SET;
}

/*!
 * @brief     Use even frame to transfer
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum:  channel number
 *
 * @retval    None
 */
void USB_OTG_H_TxUseEvenFrame(USB_OTG_HOST_T* usbhx, uint8_t chNum)
{
    usbhx->REGS_HCH[chNum].HCH_B.ODDF = BIT_RESET;
}

/*!
 * @brief     USB host driver VBUS status
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     state: Select the USB port power status
 *              @arg USBH_PORT_PWR_OFF
 *              @arg USBH_PORT_PWR_ON
 *
 * @retval    None
 *
 */
void USB_OTG_H_DriverVbus(USB_OTG_HOST_T* usbhx, USBH_PORT_PWR_STA_T state)
{
    if (state == USBH_PORT_PWR_ON)
    {
        if (usbhx->HPORTCSTS_B.PP == RESET)
        {
            USB_OTG_H_EnablePortPower(usbhx);
        }
    }
    else
    {
        if (usbhx->HPORTCSTS_B.PP == SET)
        {
            USB_OTG_H_DisablePortPower(usbhx);
        }
    }
}

/*!
 * @brief     Enable channel
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum:  channel number
 *
 * @retval    None
 */
void USB_OTG_H_EnableChannel(USB_OTG_HOST_T* usbhx, uint8_t chNum)
{
    usbhx->REGS_HCH[chNum].HCH_B.CHINT = BIT_RESET;

    usbhx->REGS_HCH[chNum].HCH_B.CHEN = BIT_SET;
}

/*!
 * @brief     Read Periodic Transmit Request Queue Available Space
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @retval    spave value
 */
uint32_t USB_OTG_H_ReadTxQueueRemainSpace(USB_OTG_HOST_T* usbhx)
{
    return (usbhx->HPTXSTS_B.QSPACE);
}

/*!
 * @brief     Read packet count
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum:  channel number
 *
 * @retval    packet count
 */
uint32_t USB_OTG_H_ReadPacketCount(USB_OTG_HOST_T* usbhx, uint8_t chNum)
{
    return (usbhx->REGS_HCH[chNum].HCHTSIZE_B.PCKTCNT);
}

/*!
 * @brief     Read all channel interrupt flag
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     channel: channel
 *
 * @retval    channel interrupt status
 */
uint32_t USB_OTG_H_ReadAllChannelsIntFlag(USB_OTG_HOST_T* usbhx, uint16_t channel)
{
    uint32_t temp = 0;
    
    temp = usbhx->HACHINT;
    
    temp &= usbhx->HACHIMASK;
    
    return (((temp) & (uint32_t)(channel)) ? SET : RESET);
}

/*!
 * @brief     Read host port speed
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @retval    Port speed
 */
uint8_t USB_OTG_H_ReadPortSpeedType(USB_OTG_HOST_T* usbhx)
{
    return (usbhx->HPORTCSTS_B.PSPDSEL);
}

/*!
 * @brief     Read host port POVC change status
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @retval    Port POVC change status
 */
uint8_t USB_OTG_H_PovcBitIsChange(USB_OTG_HOST_T* usbhx)
{
    return ((usbhx->HPORTCSTS_B.POVCCHG) ? SET : RESET);
}

/*!
 * @brief     Clear host port POVC change status
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @retval    None
 */
void USB_OTG_H_ClearPovcBitChangeFlag(USB_OTG_HOST_T* usbhx)
{
    usbhx->HPORTCSTS_B.POVCCHG = BIT_SET;
}

/*!
 * @brief     Read host port connect status
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @retval    Connect status
 */
uint8_t USB_OTG_H_PortIsConnect(USB_OTG_HOST_T* usbhx)
{
    return ((usbhx->HPORTCSTS_B.PCNNTFLG) ? SET : RESET);
}

/*!
 * @brief     Read host port PEN change status
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @retval    Bit change status
 */
uint8_t USB_OTG_H_PenBitIsChange(USB_OTG_HOST_T* usbhx)
{
    return ((usbhx->HPORTCSTS_B.PENCHG) ? SET : RESET);
}

/*!
 * @brief     Clear host port PEN change status
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @retval    None
 */
void USB_OTG_H_ClearPenBitChangeFlag(USB_OTG_HOST_T* usbhx)
{
    usbhx->HPORTCSTS_B.PENCHG = BIT_SET;
}

/*!
 * @brief     Read host port enable status
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @retval    Port enable status
 */
uint8_t USB_OTG_H_PortIsEnable(USB_OTG_HOST_T* usbhx)
{
    return ((usbhx->HPORTCSTS_B.PEN) ? SET : RESET);
}

/*!
 * @brief     Read host endpoint direction
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @retval    Endpoint direction
 */
uint8_t USB_OTG_H_ReadEpDirection(USB_OTG_HOST_T* usbhx, uint8_t chNum)
{
    return (usbhx->REGS_HCH[chNum].HCH_B.EDPDRT);
}

/*!
 * @brief     Configure host phy clock
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     phyClock: Select the USB phy clock
 *              @arg PHYCLK_30_60_MHZ
 *              @arg PHYCLK_48_MHZ
 *              @arg PHYCLK_6_MHZ
 *
 * @retval    None
 */
void USB_OTG_H_ConfigHostClk(USB_OTG_HOST_T* usbhx, USBH_PHYCLK_T phyClock)
{
    usbhx->HCFG_B.PHYCLKSEL = (phyClock & 0x03);
}

/*!
 * @brief     Set frame interval value
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     freq: Frequency
 *
 * @retval    None
 */
void USB_OTG_H_SetFrameInterval(USB_OTG_HOST_T* usbhx, uint16_t freq)
{
    usbhx->HFIVL_B.FIVL = (freq & 0xFFFF);
}

/*!
 * @brief     Set channel transfer size
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum: channel number
 *
 * @param     size: transfer size(0 ~ 0x7FFF)
 *
 * @retval    None
 */
void USB_OTG_H_ConfigTransferSize(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint32_t size)
{
    usbhx->REGS_HCH[chNum].HCHTSIZE_B.TSFSIZE = size & 0x7FFFF;
}

/*!
 * @brief     Set channel transfer packet count
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum: channel number
 *
 * @param     count: packet count(0 ~ 0x3FF)
 *
 * @retval    None
 */
void USB_OTG_H_ConfigPacketCount(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint32_t count)
{
    usbhx->REGS_HCH[chNum].HCHTSIZE_B.PCKTCNT = count & 0x3FF;
}

/*!
 * @brief     Set channel transfer data PID
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum: channel number
 *
 * @param     pid: data pid
 *
 * @retval    None
 */
void USB_OTG_H_ConfigDataPID(USB_OTG_HOST_T* usbhx, uint8_t chNum, USB_OTG_DATA_PID_T pid)
{
    usbhx->REGS_HCH[chNum].HCHTSIZE_B.DATAPID = pid;
}

/*!
 * @brief     Set channel transfer DMA address
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum: channel number
 *
 * @param     addr: DMA address
 *
 * @retval    None
 */
void USB_OTG_H_ConfigDMAAddr(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint32_t addr)
{
    usbhx->REGS_HCH[chNum].HCHDMA = addr;
}

/*!
 * @brief     Read host frame number
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @retval    frame number
 */
uint16_t USB_OTG_H_ReadFrameNum(USB_OTG_HOST_T* usbhx)
{
    return (usbhx->HFIFM_B.FNUM);
}

/*!
 * @brief     Set frame interval value
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum: channel number
 *
 * @retval    endpoint type
 */
uint8_t USB_OTG_H_ReadEpType(USB_OTG_HOST_T* usbhx, uint8_t chNum)
{
    return (usbhx->REGS_HCH[chNum].HCH_B.EDPTYP);
}

/*!
 * @brief     Read usb host transfer size
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum: channel number
 *
 * @retval    transfer size
 */
uint32_t USB_OTG_H_ReadTransferSize(USB_OTG_HOST_T* usbhx, uint8_t chNum)
{
    return (usbhx->REGS_HCH[chNum].HCHTSIZE_B.TSFSIZE);
}

/*!
 * @brief     Read channel enable status
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum: channel number
 *
 * @retval    channel enable status
 */
uint8_t USB_OTG_H_ReadChannelStatus(USB_OTG_HOST_T* usbhx, uint8_t chNum)
{
    return (usbhx->REGS_HCH[chNum].HCH_B.CHEN);
}

/*!
 * @brief     Read channel interrrupt status
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     interrrupt: channel interrrupt
 *              @arg CHANNEL_INT_TSFCMPN
 *
 * @retval    interrupt status
 */
uint8_t USB_OTG_H_ReadChannelIntFlag(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint32_t interrupt)
{
    uint32_t temp;

    temp = (usbhx->REGS_HCH[chNum].HCHINT);

    temp &= usbhx->REGS_HCH[chNum].HCHIMASK;
    
    return ((temp & (uint32_t)(interrupt)) ? SET : RESET);
}

/*!
 * @brief     Clear channel interrrupt status
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum: channel number
 *
 * @param     interrrupt: channel interrrupt
 *              @arg CHANNEL_INT_TSFCMPN
 *
 * @retval    None
 */
void USB_OTG_H_ClearChannelIntFlag(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint32_t interrupt)
{
    usbhx->REGS_HCH[chNum].HCHINT = (interrupt);
}

/*!
 * @brief     Clear all channel interrrupt status
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @param     chNum: channel number
 *
 * @retval    None
 */
void USB_OTG_H_ClearAllChannelIntFlag(USB_OTG_HOST_T* usbhx, uint8_t chNum)
{
    usbhx->REGS_HCH[chNum].HCHINT = 0xFFFFFFFF;
    usbhx->REGS_HCH[chNum].HCHIMASK = 0;
}

/*!
 * @brief     Read port connect status
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @retval    Port connect status
 */
uint8_t USB_OTG_H_ReadPortConnectIntTriFlag(USB_OTG_HOST_T* usbhx)
{
    return (usbhx->HPORTCSTS_B.PCINTFLG);
}

/*!
 * @brief     Clear port connect status
 *
 * @param     usbhx: Select the USB host port.
 *              @arg USB_OTG_FS_H
 *              @arg USB_OTG_HS_H
 *
 * @retval    None
 */
void USB_OTG_H_ClearPortConnectIntTriFlag(USB_OTG_HOST_T* usbhx)
{
    usbhx->HPORTCSTS_B.PCINTFLG = BIT_SET;
}

/*!
 * @brief     Configure and activate endpoint
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     ep: endpoint structure
 *
 * @retval    None
 */
void USB_OTG_D_ConfigEP(USB_OTG_DEVICE_T* usbdx, USB_OTG_ENDPOINT_INFO_T* ep)
{
    if (ep->epDir)
    {
        usbdx->DAEPIMASK_B.AINM  |= (0x0001 << ep->epNum);

        if (usbdx->EP_IN[ep->epNum].DIEPCTRL_B.USBAEP == BIT_RESET)
        {
            usbdx->EP_IN[ep->epNum].DIEPCTRL_B.MAXPS    = ep->mps;
            usbdx->EP_IN[ep->epNum].DIEPCTRL_B.EPTYPE   = ep->epType;
            usbdx->EP_IN[ep->epNum].DIEPCTRL_B.TXFNUM   = ep->epNum;
            usbdx->EP_IN[ep->epNum].DIEPCTRL_B.DPIDSET  = BIT_SET;
            usbdx->EP_IN[ep->epNum].DIEPCTRL_B.USBAEP   = BIT_SET;
        }
    }
    else
    {
        usbdx->DAEPIMASK_B.AOUTM  |= (0x0001 << ep->epNum);

        if (usbdx->EP_OUT[ep->epNum].DOEPCTRL_B.USBAEP == BIT_RESET)
        {
            usbdx->EP_OUT[ep->epNum].DOEPCTRL_B.MAXPS    = ep->mps;
            usbdx->EP_OUT[ep->epNum].DOEPCTRL_B.EPTYPE   = ep->epType;
            usbdx->EP_OUT[ep->epNum].DOEPCTRL_B.DPIDSET  = BIT_SET;
            usbdx->EP_OUT[ep->epNum].DOEPCTRL_B.USBAEP   = BIT_SET;
        }

    }
}

/*!
 * @brief     Reset and de-activate endpoint
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     ep: endpoint structure
 *
 * @retval    None
 */
void USB_OTG_D_ResetEP(USB_OTG_DEVICE_T* usbdx, USB_OTG_ENDPOINT_INFO_T* ep)
{
    if (ep->epDir)
    {
        if (usbdx->EP_IN[ep->epNum].DIEPCTRL_B.EPEN == BIT_SET)
        {
            usbdx->EP_IN[ep->epNum].DIEPCTRL_B.EPDIS = BIT_SET;
            usbdx->EP_IN[ep->epNum].DIEPCTRL_B.NAKSET = BIT_SET;
        }

        usbdx->DAEPIMASK_B.AINM  &= ~(0x0001 << ep->epNum);
        usbdx->DEPIMASK_B.IN1M &= ~(0x0001 << ep->epNum);

        usbdx->EP_IN[ep->epNum].DIEPCTRL_B.MAXPS = BIT_RESET;
        usbdx->EP_IN[ep->epNum].DIEPCTRL_B.USBAEP = BIT_RESET;
        usbdx->EP_IN[ep->epNum].DIEPCTRL_B.TXFNUM = BIT_RESET;
        usbdx->EP_IN[ep->epNum].DIEPCTRL_B.EPTYPE = BIT_RESET;
        usbdx->EP_IN[ep->epNum].DIEPCTRL_B.DPIDSET = BIT_RESET;
    }
    else
    {
        if (usbdx->EP_OUT[ep->epNum].DOEPCTRL_B.EPEN == BIT_SET)
        {
            usbdx->EP_OUT[ep->epNum].DOEPCTRL_B.EPDIS = BIT_SET;
            usbdx->EP_OUT[ep->epNum].DOEPCTRL_B.NAKSET = BIT_SET;
        }

        usbdx->DAEPIMASK_B.AOUTM  &= ~(0x0001 << ep->epNum);
        usbdx->DEPIMASK_B.OUT1M &= ~(0x0001 << ep->epNum);

        usbdx->EP_OUT[ep->epNum].DOEPCTRL_B.MAXPS = BIT_RESET;
        usbdx->EP_OUT[ep->epNum].DOEPCTRL_B.USBAEP = BIT_RESET;
        usbdx->EP_OUT[ep->epNum].DOEPCTRL_B.EPTYPE = BIT_RESET;
        usbdx->EP_OUT[ep->epNum].DOEPCTRL_B.DPIDSET = BIT_RESET;
    }
}

/*!
 * @brief     Configure soft disconnect status
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     status: Select the soft disconnect status
 *              @arg USBD_SOFT_DISC_NORMAL
 *              @arg USBD_SOFT_DISC_ENABLE
 *
 * @retval    None
 */
void USB_OTG_D_ConfigSoftDisconnect(USB_OTG_DEVICE_T* usbdx, USBD_SOFT_DISC_T status)
{
    if (status == USBD_SOFT_DISC_ENABLE)
    {
        usbdx->DCTRL_B.SDCNNT = BIT_SET;
    }
    else
    {
        usbdx->DCTRL_B.SDCNNT = BIT_RESET;
    }
}

/*!
 * @brief     Configure periodic frame interval
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     interval: Frame interval
 *              @arg USBD_FRAME_INTERVAL_80
 *              @arg USBD_FRAME_INTERVAL_85
 *              @arg USBD_FRAME_INTERVAL_90
 *              @arg USBD_FRAME_INTERVAL_95
 *
 * @retval    None
 */
void USB_OTG_D_ConfigPeriodicFrameInterval(USB_OTG_DEVICE_T* usbdx, USBD_FRAME_INTERVAL_T interval)
{
    usbdx->DCFG_B.PFITV = interval;
}

/*!
 * @brief     Configure device speed
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     speed: device speed
 *              @arg USBD_DEV_SPEED_HS
 *              @arg USBD_DEV_SPEED_HS_IN_FS
 *              @arg USBD_DEV_SPEED_FS
 *
 * @retval    None
 */
void USB_OTG_D_ConfigDeviceSpeed(USB_OTG_DEVICE_T* usbdx, USBD_DEV_SPEED_T speed)
{
    usbdx->DCFG_B.DSPDSEL = speed;
}

/*!
 * @brief     Read enum speed
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @retval    None
 */
uint8_t USB_OTG_D_ReadEnumSpeed(USB_OTG_DEVICE_T* usbdx)
{
    return usbdx->DSTS_B.ENUMSPD;
}

/*!
 * @brief     Configure device address
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     address: device address
 *
 * @retval    None
 */
void USB_OTG_D_ConfigDeviceAddress(USB_OTG_DEVICE_T* usbdx, uint8_t address)
{
    usbdx->DCFG_B.DADDR = address;
}

/*!
 * @brief     Enable USB Remote Wakeup.
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @retval    None
 */
void USB_OTG_D_EnableRemoteWakeupSignal(USB_OTG_DEVICE_T* usbdx)
{
    usbdx->DCTRL_B.RWKUPS = BIT_SET;
}

/*!
 * @brief     Disable USB Remote Wakeup.
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @retval    None
 */
void USB_OTG_D_DisableRemoteWakeupSignal(USB_OTG_DEVICE_T* usbdx)
{
    usbdx->DCTRL_B.RWKUPS = BIT_RESET;
}

/*!
 * @brief     Enable OUT endpoint the whole interrupt mask
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     epNum: Endpoint number
 *              @arg USBD_EP_0
 *              @arg USBD_EP_1
 *              @arg USBD_EP_2
 *              @arg USBD_EP_3
 *              @arg USBD_EP_4
 *              @arg USBD_EP_5
 *              @arg USBD_EP_6
 *              @arg USBD_EP_7
 *
 * @retval    None
 */
void USB_OTG_D_EnableOutEpInterruptMask(USB_OTG_DEVICE_T* usbdx, uint8_t epNum)
{
    usbdx->DAEPIMASK_B.AOUTM |= (0x0001 << epNum);
}

/*!
 * @brief     Disable OUT endpoint the whole interrupt mask
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     epNum: Endpoint number
 *              @arg USBD_EP_0
 *              @arg USBD_EP_1
 *              @arg USBD_EP_2
 *              @arg USBD_EP_3
 *              @arg USBD_EP_4
 *              @arg USBD_EP_5
 *              @arg USBD_EP_6
 *              @arg USBD_EP_7
 *
 * @retval    None
 */
void USB_OTG_D_DisableOutEpInterruptMask(USB_OTG_DEVICE_T* usbdx, uint8_t epNum)
{
    usbdx->DAEPIMASK_B.AOUTM &= ~(0x0001 << epNum);
}

/*!
 * @brief     Enable IN endpoint the whole interrupt mask
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     epNum: Endpoint number
 *              @arg USBD_EP_0
 *              @arg USBD_EP_1
 *              @arg USBD_EP_2
 *              @arg USBD_EP_3
 *              @arg USBD_EP_4
 *              @arg USBD_EP_5
 *              @arg USBD_EP_6
 *              @arg USBD_EP_7
 *
 * @retval    None
 */
void USB_OTG_D_EnableInEpInterruptMask(USB_OTG_DEVICE_T* usbdx, uint8_t epNum)
{
    usbdx->DAEPIMASK_B.AINM |= (0x0001 << epNum);
}

/*!
 * @brief     Disable IN endpoint the whole interrupt mask
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     epNum: Endpoint number
 *              @arg USBD_EP_0
 *              @arg USBD_EP_1
 *              @arg USBD_EP_2
 *              @arg USBD_EP_3
 *              @arg USBD_EP_4
 *              @arg USBD_EP_5
 *              @arg USBD_EP_6
 *              @arg USBD_EP_7
 *
 * @retval    None
 */
void USB_OTG_D_DisableInEpInterruptMask(USB_OTG_DEVICE_T* usbdx, uint8_t epNum)
{
    usbdx->DAEPIMASK_B.AINM &= ~(0x0001 << epNum);
}

/*!
 * @brief     Enable IN endpoint empty interrupt
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     epNum: endpoint number
 *
 * @retval    None
 */
void USB_OTG_D_EnableInEpEmptyInterrupt(USB_OTG_DEVICE_T* usbdx, uint8_t epNum)
{
    usbdx->DIEIMASK |= ((uint32_t)(1 << epNum));
}

/*!
 * @brief     Disable IN endpoint empty interrupt
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     epNum: endpoint number
 *
 * @retval    None
 */
void USB_OTG_D_DisableInEpEmptyInterrupt(USB_OTG_DEVICE_T* usbdx, uint8_t epNum)
{
    usbdx->DIEIMASK &= ~((uint32_t)(1 << epNum));
}

/*!
 * @brief     Disable all in endpoint interrupt
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @retval    None
 */
void USB_OTG_D_DisableAllInEpInterrupt(USB_OTG_DEVICE_T* usbdx)
{
    usbdx->DINIMASK = 0x0;
}

/*!
 * @brief     Disable all out endpoint interrupt
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @retval    None
 */
void USB_OTG_D_DisableAllOutEpInterrupt(USB_OTG_DEVICE_T* usbdx)
{
    usbdx->DOUTIMASK = 0x0;
}

/*!
 * @brief     Disable all endpoint interrupt
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @retval    None
 */
void USB_OTG_D_DisableAllEpInterrupt(USB_OTG_DEVICE_T* usbdx)
{
    usbdx->DAEPIMASK = 0x0;
}

/*!
 * @brief     Enable IN endpoint 1 interrupt
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     interrupt: IN endpoint 1 interrupt
 *              @arg USBD_INT_EP_IN_TSFCMP
 *              @arg USBD_INT_EP_IN_EPDIS
 *              @arg USBD_INT_EP_IN_TO
 *              @arg USBD_INT_EP_IN_ITXEMP
 *              @arg USBD_INT_EP_IN_IEPMM
 *              @arg USBD_INT_EP_IN_IEPNAKE
 *              @arg USBD_INT_EP_IN_TXFUDR
 *              @arg USBD_INT_EP_IN_BNA
 *
 * @retval    None
 */
void USB_OTG_D_EnableInEp1Interrupt(USB_OTG_DEVICE_T* usbdx, uint32_t interrupt)
{
    usbdx->DIN1IMASK |= interrupt;
}

/*!
 * @brief     Disble IN endpoint 1 interrupt
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     interrupt: IN endpoint 1 interrupt
 *              @arg USBD_INT_EP_IN_TSFCMP
 *              @arg USBD_INT_EP_IN_EPDIS
 *              @arg USBD_INT_EP_IN_TO
 *              @arg USBD_INT_EP_IN_ITXEMP
 *              @arg USBD_INT_EP_IN_IEPMM
 *              @arg USBD_INT_EP_IN_IEPNAKE
 *              @arg USBD_INT_EP_IN_TXFUDR
 *              @arg USBD_INT_EP_IN_BNA
 *
 * @retval    None
 */
void USB_OTG_D_DisableInEp1Interrupt(USB_OTG_DEVICE_T* usbdx, uint32_t interrupt)
{
    usbdx->DIN1IMASK &= ~interrupt;
}

/*!
 * @brief     Enable OUT endpoint 1 interrupt
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     interrupt: OUT endpoint 1 interrupt
 *              @arg USBD_INT_EP_OUT_TSFCMP
 *              @arg USBD_INT_EP_OUT_EPDIS
 *              @arg USBD_INT_EP_OUT_AHBERR
 *              @arg USBD_INT_EP_OUT_OPERR
 *              @arg USBD_INT_EP_OUT_BNA
 *              @arg USBD_INT_EP_OUT_BERR
 *              @arg USBD_INT_EP_OUT_NAK
 *              @arg USBD_INT_EP_OUT_NYET
 *
 * @retval    None
 */
void USB_OTG_D_EnableOutEp1Interrupt(USB_OTG_DEVICE_T* usbdx, uint32_t interrupt)
{
    usbdx->DOUT1MASK |= interrupt;
}

/*!
 * @brief     Disable OUT endpoint 1 interrupt
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     interrupt: OUT endpoint 1 interrupt
 *              @arg USBD_INT_EP_OUT_TSFCMP
 *              @arg USBD_INT_EP_OUT_EPDIS
 *              @arg USBD_INT_EP_OUT_AHBERR
 *              @arg USBD_INT_EP_OUT_OPERR
 *              @arg USBD_INT_EP_OUT_BNA
 *              @arg USBD_INT_EP_OUT_BERR
 *              @arg USBD_INT_EP_OUT_NAK
 *              @arg USBD_INT_EP_OUT_NYET
 *
 * @retval    None
 */
void USB_OTG_D_DisbleOutEp1Interrupt(USB_OTG_DEVICE_T* usbdx, uint32_t interrupt)
{
    usbdx->DOUT1MASK &= ~interrupt;
}

/*!
 * @brief     Enable IN endpoint interrupt
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     interrupt: IN endpoint interrupt
 *              @arg USBD_INT_EP_IN_TSFCMP
 *              @arg USBD_INT_EP_IN_EPDIS
 *              @arg USBD_INT_EP_IN_TO
 *              @arg USBD_INT_EP_IN_ITXEMP
 *              @arg USBD_INT_EP_IN_IEPMM
 *              @arg USBD_INT_EP_IN_IEPNAKE
 *              @arg USBD_INT_EP_IN_TXFUDR
 *              @arg USBD_INT_EP_IN_BNA
 *
 * @retval    None
 */
void USB_OTG_D_EnableInEpInterrupt(USB_OTG_DEVICE_T* usbdx, uint32_t interrupt)
{
    usbdx->DINIMASK |= interrupt;
}

/*!
 * @brief     Disable IN endpoint interrupt
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     interrupt: IN endpoint interrupt
 *              @arg USBD_INT_EP_IN_TSFCMP
 *              @arg USBD_INT_EP_IN_EPDIS
 *              @arg USBD_INT_EP_IN_TO
 *              @arg USBD_INT_EP_IN_ITXEMP
 *              @arg USBD_INT_EP_IN_IEPMM
 *              @arg USBD_INT_EP_IN_IEPNAKE
 *              @arg USBD_INT_EP_IN_TXFUDR
 *              @arg USBD_INT_EP_IN_BNA
 *
 * @retval    None
 */
void USB_OTG_D_DisableInEpInterrupt(USB_OTG_DEVICE_T* usbdx, uint32_t interrupt)
{
    usbdx->DINIMASK &= ~interrupt;
}

/*!
 * @brief     Enable OUT endpoint interrupt
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     interrupt: OUT endpoint interrupt
 *                  @arg USBD_INT_EP_OUT_TSFCMP : Transfer completed.
 *                  @arg USBD_INT_EP_OUT_EPDIS  : Endpoint disabled.
 *                  @arg USBD_INT_EP_OUT_SETPCMP: setup completed.
 *                  @arg USBD_INT_EP_OUT_RXOTDIS: OUT token received when endpoint disabled.
 *                  @arg USBD_INT_EP_OUT_RXBSP  : Received Back-to-back SETUP packets over 3.
 *                  @arg USBD_INT_EP_OUT_ALL    : all interrupt above.
 *
 * @retval    None
 */
void USB_OTG_D_EnableOutEpInterrupt(USB_OTG_DEVICE_T* usbdx, uint32_t interrupt)
{
    usbdx->DOUTIMASK |= interrupt;
}

/*!
 * @brief     Disable OUT endpoint interrupt
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     interrupt: OUT endpoint interrupt
 *                  @arg USBD_INT_EP_OUT_TSFCMP : Transfer completed.
 *                  @arg USBD_INT_EP_OUT_EPDIS  : Endpoint disabled.
 *                  @arg USBD_INT_EP_OUT_SETPCMP: setup completed.
 *                  @arg USBD_INT_EP_OUT_RXOTDIS: OUT token received when endpoint disabled.
 *                  @arg USBD_INT_EP_OUT_RXBSP  : Received Back-to-back SETUP packets over 3.
 *                  @arg USBD_INT_EP_OUT_ALL    : all interrupt above.
 *
 * @retval    None
 */
void USB_OTG_D_DisableOutEpInterrupt(USB_OTG_DEVICE_T* usbdx, uint32_t interrupt)
{
    usbdx->DOUTIMASK &= ~interrupt;
}

/*!
 * @brief     Read all OUT endpoint interrupt number
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @retval    OUT endpoint interrupt number
 */
uint16_t USB_OTG_D_ReadAllOutEpIntNumber(USB_OTG_DEVICE_T* usbdx)
{
    uint16_t temp;

    temp = usbdx->DAEPINT_B.OUTEPINT;
    temp &= usbdx->DAEPIMASK_B.AOUTM;

    return temp;
}

/*!
 * @brief     Read all IN endpoint interrupt number
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @retval    INT endpoint interrupt number
 */
uint16_t USB_OTG_D_ReadAllInEpIntNumber(USB_OTG_DEVICE_T* usbdx)
{
    uint16_t temp;

    temp = usbdx->DAEPINT_B.INEPINT;
    temp &= usbdx->DAEPIMASK_B.AINM;

    return temp;
}

/*!
 * @brief     Read the OUT endpoint interrupt status
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     epNum : OUT endpiont number
 *
 * @param     interrupt : OUT ep interrupt flag.
 *                     This parameter can be one of the following value:
 *                     @arg USBD_INT_EP_OUT_TSFCMP : Transfer completed
 *                     @arg USBD_INT_EP_OUT_EPDIS  : Endpoint disabled
 *                     @arg USBD_INT_EP_OUT_SETPCMP: setup completed
 *                     @arg USBD_INT_EP_OUT_RXOTDIS: OUT token received when endpoint disabled
 *                     @arg USBD_INT_EP_OUT_RXBSP  : Received Back-to-back SETUP packets over 3
 *
 * @retval    OUT endpoint interrupt status
 */
uint8_t USB_OTG_D_ReadOutEpIntStatus(USB_OTG_DEVICE_T* usbdx, uint8_t epNum, uint32_t interrupt)
{
    uint8_t status = RESET;

    if ((((uint8_t)usbdx->EP_OUT[epNum].DOEPINT) & interrupt) != RESET)
    {
        status = SET;
    }
    else
    {
        status = RESET;
    }

    return status;
}

/*!
 * @brief     Read the IN endpoint interrupt status
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     epNum : IN endpiont number
 *
 * @param     interrupt : IN ep interrupt flag.
 *                     This parameter can be one of the following value:
 *                     @arg USBD_INT_EP_IN_TSFCMP : Transfer completed
 *                     @arg USBD_INT_EP_IN_EPDIS  : Endpoint disabled
 *                     @arg USBD_INT_EP_IN_TO     : Timeout
 *                     @arg USBD_INT_EP_IN_ITXEMP : IN token received when TxFIFO is empty
 *                     @arg USBD_INT_EP_IN_IEPNAKE: IN endpoint NAK effective
 *                     @arg USBD_INT_EP_IN_TXFE   : TxFIFO empty
 *
 * @retval    IN endpoint interrupt status
 */
uint8_t USB_OTG_D_ReadInEpIntStatus(USB_OTG_DEVICE_T* usbdx, uint8_t epNum, uint32_t interrupt)
{
    return (((uint8_t)usbdx->EP_IN[epNum].DIEPINT) & interrupt) ? SET : RESET;
}

/*!
 * @brief     Clear the OUT endpoint interrupt status
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     epNum : OUT endpiont number
 *
 * @param     interrupt: OUT ep interrupt flag.
 *                  This parameter can be any combination of the following value:
 *                  @arg USBD_INT_EP_OUT_TSFCMP : Transfer completed.
 *                  @arg USBD_INT_EP_OUT_EPDIS  : Endpoint disabled.
 *                  @arg USBD_INT_EP_OUT_SETPCMP: setup completed.
 *                  @arg USBD_INT_EP_OUT_RXOTDIS: OUT token received when endpoint disabled.
 *                  @arg USBD_INT_EP_OUT_RXBSP  : Received Back-to-back SETUP packets over 3.
 *                  @arg USBD_INT_EP_OUT_ALL    : all interrupt above.
 *
 * @retval    OUT endpoint interrupt status
 */
void USB_OTG_D_ClearOutEpIntStatus(USB_OTG_DEVICE_T* usbdx, uint8_t epNum, uint32_t interrupt)
{
    usbdx->EP_OUT[epNum].DOEPINT |= interrupt;
}

/*!
 * @brief     Clear the IN endpoint interrupt status
 *
 * @param     usbdx: Select the USB device port.
 *              @arg USB_OTG_FS_D
 *              @arg USB_OTG_HS_D
 *
 * @param     epNum : IN endpiont number
 *
 * @param     interrupt: IN ep interrupt flag.
 *                  This parameter can be any combination of the following value:
 *                   @arg USBD_INT_EP_IN_TSFCMP  : Transfer completed.
 *                   @arg USBD_INT_EP_IN_EPDIS   : Endpoint disabled.
 *                   @arg USBD_INT_EP_IN_TO      : Timeout.
 *                   @arg USBD_INT_EP_IN_ITXEMP  : IN token received when TxFIFO is empty.
 *                   @arg USBD_INT_EP_IN_IEPNAKE : IN endpoint NAK effective.
 *                   @arg USBD_INT_EP_IN_TXFE    : TxFIFO empty.
 *                   @arg USBD_INT_EP_IN_ALL     : all interrupt above.
 *
 * @retval    IN endpoint interrupt status
 */
void USB_OTG_D_ClearInEpIntStatus(USB_OTG_DEVICE_T* usbdx, uint8_t epNum, uint32_t interrupt)
{
    usbdx->EP_IN[epNum].DIEPINT |= interrupt;
}

/*!
 * @brief       Write a buffer of data to a selected endpoint
 *
 * @param       usbFifo
 *
 * @param       chEpNum:  channel endpoint number
 *
 * @param       wBuf:   The pointer to the buffer of data to be written to the endpoint
 *
 * @param       wLen:   Number of data to be written (in bytes)
 *
 * @param       dmaStatus: DMA enable status
 *
 * @retval      None
 */
void USB_OTG_FIFO_WriteFifoPacket(USB_OTG_FIFO_T* usbFifo, uint8_t chEpNum, uint8_t* wBuf, uint32_t wlen, uint8_t dmaStatus)
{
    uint32_t wordCnt = 0, i = 0;

    if (dmaStatus == DISABLE)
    {
        wordCnt = (wlen + 3) / 4;

        for (i = 0; i < wordCnt; i++)
        {
            usbFifo->FIFO[chEpNum].DATA = (((const struct USB_FIFO_PACKET_T*)(const void*)(wBuf))->val);
            wBuf += 4;
        }
    }
}

/*!
 * @brief       Read the RxFIFO packet to buffer
 *
 * @param       usbFifo
 *
 * @param       rBuf:    Buffer pointer
 *
 * @param       rLen:    Buffer length
 *
 * @retval      None
 */
void USB_OTG_FIFO_ReadRxFifoPacket(USB_OTG_FIFO_T* usbFifo, uint8_t* rBuf, uint32_t rLen)
{
    uint32_t i = 0;
    uint32_t temp = 0;

    for (i = 0; i < rLen / 4; i++)
    {
        (((struct USB_FIFO_PACKET_T*)(void*)(rBuf))->val) = usbFifo->FIFO[0].DATA;
        rBuf += 4 ;
    }

    if (rLen & 0x3)
    {
        temp = usbFifo->FIFO[0].DATA;
        for (i = 0; i < (rLen & 0x3); i++)
        {
            rBuf[i] = temp >> (i << 3);
        }
    }
}

/**@} end of group USB_Functions*/
/**@} end of group USB_Driver*/
/**@} end of group APM32F4xx_StdPeriphDriver*/
