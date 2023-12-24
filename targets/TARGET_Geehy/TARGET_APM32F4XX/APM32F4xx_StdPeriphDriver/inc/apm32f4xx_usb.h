/*!
 * @file        apm32f4xx_usb.h
 *
 * @brief       This file provides all the USB driver functions
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

/* Define to prevent recursive inclusion */
#ifndef __APM32F4XX_USB_H
#define __APM32F4XX_USB_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "apm32f4xx.h"

/** @addtogroup APM32F4xx_StdPeriphDriver
  @{
*/

/** @addtogroup USB_Driver USB Driver
  @{
*/

/** @defgroup USB_Macros Macros
  @{
*/

#define USB_OTG_CORE_ID         0x4F54281A

#define USBD_DEFAULT_TRDT_VAL   9
#define USBD_HS_TRDT_VAL        9
#define USBD_FS_TRDT_VAL        5

/* USB Global interrupt source */
#define USB_INT_G_MMIS          BIT1            //!< Mode mismatch interrupt
#define USB_INT_G_OTG           BIT2            //!< OTG interrupt
#define USB_INT_G_SOF           BIT3            //!< Frame Start of interrupt
#define USB_INT_G_RXFNONE       BIT4            //!< RxFIFO non-empty interrupt
#define USB_INT_G_NPTXFEM       BIT5            //!< Non-periodic TxFIFO empty interrupt
#define USB_INT_G_GINNPNAKE     BIT6            //!< Global IN non-periodic NAK effective interrupt
#define USB_INT_G_GONAKE        BIT7            //!< Global OUT NAK effective interrupt

#define USB_INT_G_ESUS          BIT10           //!< Early suspend interrupt
#define USB_INT_G_USBSUS        BIT11           //!< USB suspend interrupt
#define USB_INT_G_USBRST        BIT12           //!< USB reset interrupt
#define USB_INT_G_ENUMD         BIT13           //!< Enumeration done interrupt
#define USB_INT_G_ISOPD         BIT14           //!< Isochronous OUT packet dropped interrupt
#define USB_INT_G_EOPF          BIT15           //!< End of periodic frame interrupt

#define USB_INT_G_INEP          BIT18           //!< IN endpoint interrupt
#define USB_INT_G_ONEP          BIT19           //!< OUT endpoint interrupt

#define USB_INT_G_IIINTX        BIT20           //!< Incomplete isochronous IN transfer interrupt
#define USB_INT_G_IP_OUTTX      BIT21           //!< Incomplete isochronous OUT transfer interrupt

#define USB_INT_G_HPORT         BIT24           //!< Host port interrupt
#define USB_INT_G_HCHAN         BIT25           //!< Host channels interrupt
#define USB_INT_G_PTXFE         BIT26           //!< Periodic TxFIFO empty

#define USB_INT_G_CIDSTSTC      BIT28           //!< Connector ID status change
#define USB_INT_G_DEDIS         BIT29           //!< Disconnect detected interrupt
#define USB_INT_G_SREQ          BIT30           //!< Session request/new session detected interrupt
#define USB_INT_G_RWAKE         BIT31           //!< Resume/remote wakeup detected interrupt
#define USB_INT_G_ALL           0xFFFFFFFF

/**@} end of group USB_Macros*/

/** @defgroup USB_Enumerations Enumerations
  @{
*/

/**
 * @brief   USB Device mode
 */
typedef enum
{
    USB_OTG_MODE_DEVICE,
    USB_OTG_MODE_HOST,
} USB_OTG_MODE_T;

/**
 * @brief   USB burst type
 */
typedef enum
{
    USB_OTG_BURST_SINGLE    = 0x0,
    USB_OTG_BURST_INCR      = 0x1,
    USB_OTG_BURST_INCR4     = 0x3,
    USB_OTG_BURST_INCR8     = 0x5,
    USB_OTG_BURST_INCR16    = 0x7,
} USB_OTG_BURST_T;

/**
 * @brief   USB PHY
 */
typedef enum
{
    USB_OTG_PHY_EX = 0x1,
    USB_OTG_PHY_EMB,
} USB_OTG_PHY_T;

/**
 * @brief   USB PHY Speed
 */
typedef enum
{
    USB_OTG_PHY_SP_HS,
    USB_OTG_PHY_SP_FS,
} USB_OTG_PHY_SPEED_T;

/**
 * @brief   USB speed
 */
typedef enum
{
    USB_OTG_SPEED_HSFSLS,
    USB_OTG_SPEED_FSLS,
} USB_OTG_SPEED_T;

/**
 * @brief USB OTG PID type of data
 */
typedef enum
{
    USB_OTG_PID_DATA0,
    USB_OTG_PID_DATA2,
    USB_OTG_PID_DATA1,
    USB_OTG_PID_SETUP,
    USB_OTG_PID_MDATA = 0x03,
} USB_OTG_DATA_PID_T;

/**
 * @brief   USB URB status
 */
typedef enum
{
    USB_URB_IDLE,
    USB_URB_NOREADY,
    USB_URB_STALL,
    USB_URB_ERROR,
    USB_URB_NYET,
    USB_URB_OK,
} USB_OTG_URB_STA_T;

/**
 * @brief   USB endpoint direction
 */
typedef enum
{
    EP_DIR_OUT,
    EP_DIR_IN
} USB_EP_DIR_T;

/**
 * @brief   USB endpoint type
 */
typedef enum
{
    EP_TYPE_CONTROL,
    EP_TYPE_ISO,
    EP_TYPE_BULK,
    EP_TYPE_INTERRUPT
} USB_EP_TYPE_T;

/**
 * @brief USB OTG host RX fifo packet status
 */
typedef enum
{
    USBH_PKTSTS_IN                = 2,  //!< recevied IN packet
    USBH_PKTSTS_IN_XFER_COMPLETE  = 3,  //!< Transfer IN packet Complete
    USBH_PKTSTS_DATA_ISO_ERR      = 5,  //!< Packer data Isochronous Error
    USBH_PKTSTS_CHANNEL_HALT      = 7,  //!< channel halt
} USBH_GRXSTS_PKTSTS_T;

/**
 * @brief USB host PID type of data
 */
typedef enum
{
    USBH_PID_SETUP,
    USBH_PID_DATA,
} USBH_PID_T;

/**
 * @brief   USB phy clock
 */
typedef enum
{
    PHYCLK_30_60_MHZ,
    PHYCLK_48_MHZ,
    PHYCLK_6_MHZ
} USBH_PHYCLK_T;

/**
 * @brief   USB pipe status
 */
typedef enum
{
    PIPE_IDLE,
    PIPE_XFRC,
    PIPE_HALTED,
    PIPE_NAK,
    PIPE_NYET,
    PIPE_STALL,
    PIPE_XACTERR,
    PIPE_BBLERR,
    PIPE_DATATGLERR,
} USBH_PIPE_STA_T;

/**
 * @brief   USB channel status
 */
typedef enum
{
    USBH_CHANNEL_FREE,
    USBH_CHANNEL_USED
} USBH_CHANNEL_STA_T;

/**
 * @brief   USB host port speed type
 */
typedef enum
{
    USBH_PORT_SPEED_HS,
    USBH_PORT_SPEED_FS,
    USBH_PORT_SPEED_LS
} USBH_SPEED_TYPE_T;

/**
 * @brief   USB port power status
 */
typedef enum
{
    USBH_PORT_PWR_OFF,
    USBH_PORT_PWR_ON,
} USBH_PORT_PWR_STA_T;

/**
 * @brief   USB channel number
 */
typedef enum
{
    USB_H_CHANNEL0  = BIT0,
    USB_H_CHANNEL1  = BIT1,
    USB_H_CHANNEL2  = BIT2,
    USB_H_CHANNEL3  = BIT3,
    USB_H_CHANNEL4  = BIT4,
    USB_H_CHANNEL5  = BIT5,
    USB_H_CHANNEL6  = BIT6,
    USB_H_CHANNEL7  = BIT7,
    USB_H_CHANNEL8  = BIT8,
    USB_H_CHANNEL9  = BIT9,
    USB_H_CHANNEL10 = BIT10,
    USB_H_CHANNEL11 = BIT11,
    USB_H_CHANNEL12 = BIT12,
    USB_H_CHANNEL13 = BIT13,
    USB_H_CHANNEL14 = BIT14,
    USB_H_CHANNEL15 = BIT15,
    USB_H_CHANNEL_ALL = (uint16_t)0xFFFF
} USBH_CHANNEL_T;

/**
 * @brief   USB host channel interrupt
 */
typedef enum
{
    CHANNEL_INT_TSFCMPN   = BIT0,             //!< Transfer Complete Normally
    CHANNEL_INT_TSFCMPAN  = BIT1,             //!< Transfer Complete Abnormally
    CHANNEL_INT_AHBERR    = BIT2,             //!< AHB Error
    CHANNEL_INT_RXSTALL   = BIT3,             //!< STALL Response Received Interrupt
    CHANNEL_INT_RXNAK     = BIT4,             //!< NAK Response Received Interrupt
    CHANNEL_INT_RXTXACK   = BIT5,             //!< ACK Response Received/Transmitted Interrupt
    CHANNEL_INT_RXNYET    = BIT6,             //!< NYET Response Received Interrupt
    CHANNEL_INT_TERR      = BIT7,             //!< Transaction Error
    CHANNEL_INT_BABBLE    = BIT8,             //!< Babble Error
    CHANNEL_INT_FOVR      = BIT9,             //!< Frame Overrun Error
    CHANNEL_INT_DTOG      = BIT10,            //!< Data Toggle Error
    CHANNEL_INT_ALL       = (uint32_t)0xFFFF  //!< Channel all interrupt
} USBH_CHANNEL_INT_T;

/**
 * @brief USB OTG device RX fifo packet status
 */
typedef enum
{
    USBD_PKTSTS_G_OUT_NAK                = 1,
    USBD_PKTSTS_REV_DATA_OUT_PKT         = 2,
    USBD_PKTSTS_OUT_DONE                 = 3,
    USBD_PKTSTS_SETUP_DONE               = 4,
    USBD_PKTSTS_REV_SETUP_PKT            = 6,
} USBD_GRXSTS_PKTSTS_T;

/**
 * @brief   USB device soft disconnect status
 */
typedef enum
{
    USBD_SOFT_DISC_NORMAL,
    USBD_SOFT_DISC_ENABLE,
} USBD_SOFT_DISC_T;

/**
 * @brief   USB Periodic frame interval
 */
typedef enum
{
    USBD_FRAME_INTERVAL_80,
    USBD_FRAME_INTERVAL_85,
    USBD_FRAME_INTERVAL_90,
    USBD_FRAME_INTERVAL_95
} USBD_FRAME_INTERVAL_T;

/**
 * @brief   USB Device speed
 */
typedef enum
{
    USBD_DEV_SPEED_HS,
    USBD_DEV_SPEED_HS_IN_FS,
    USBD_DEV_SPEED_FS = 3,
} USBD_DEV_SPEED_T;

/**
 * @brief   USB device endpoint id
 */
typedef enum
{
    USBD_EP_0,
    USBD_EP_1,
    USBD_EP_2,
    USBD_EP_3,
    USBD_EP_4,
    USBD_EP_5,
    USBD_EP_6,
    USBD_EP_7,
} USBD_EP_ID_T;

/**
 * @brief   USB device enum speed
 */
typedef enum
{
    USBD_ENUM_SPEED_HS,
    USBD_ENUM_SPEED_HS_IN_FS,
    USBD_ENUM_SPEED_FS = 3,
} USBD_ENUM_SPEED_T;

/**
 * @brief   USB device EP0 maximum packet size
 */
typedef enum
{
    USDB_EP0_MAXPS_64_BYTES,
    USDB_EP0_MAXPS_32_BYTES,
    USDB_EP0_MAXPS_16_BYTES,
    USDB_EP0_MAXPS_8_BYTES,
} USBD_EP0_MAXPS_T;

/**
 * @brief   USB OUT endpoint interrupt
 */
typedef enum
{
    USBD_INT_EP_OUT_TSFCMP  = (uint32_t)BIT0, //!< Transfer completed
    USBD_INT_EP_OUT_EPDIS   = (uint32_t)BIT1, //!< Endpoint disabled
    USBD_INT_EP_OUT_AHBERR  = (uint32_t)BIT2, /* !<  */
    USBD_INT_EP_OUT_SETPCMP = (uint32_t)BIT3, //!< setup completed
    USBD_INT_EP_OUT_RXOTDIS = (uint32_t)BIT4, //!< OUT token received when endpoint disabled
    USBD_INT_EP_OUT_RXOTPR  = (uint32_t)BIT5, //!< Phase received
    USBD_INT_EP_OUT_RXBSP   = (uint32_t)BIT6, //!< Received Back-to-back SETUP packets over 3
    USBD_INT_EP_OUT_OPERR   = (uint32_t)BIT8, //!< OUT Packet error
    USBD_INT_EP_OUT_BNA     = (uint32_t)BIT9, //!< BNA
    USBD_INT_EP_OUT_BERR    = (uint32_t)BIT12,
    USBD_INT_EP_OUT_NAK     = (uint32_t)BIT13,
    USBD_INT_EP_OUT_NYET    = (uint32_t)BIT14,
    USBD_INT_EP_OUT_ALL     = (uint32_t)(0xFF)
} USBD_INT_EP_OUT_T;

/**
 * @brief   USB IN endpoint interrupt
 */
typedef enum
{
    USBD_INT_EP_IN_TSFCMP  = (uint32_t)BIT0, //!< Transfer completed
    USBD_INT_EP_IN_EPDIS   = (uint32_t)BIT1, //!< Endpoint disabled
    USBD_INT_EP_IN_TO      = (uint32_t)BIT3, //!< Timeout
    USBD_INT_EP_IN_ITXEMP  = (uint32_t)BIT4, //!< IN token received when TxFIFO is empty
    USBD_INT_EP_IN_IEPMM   = (uint32_t)BIT5, //!< IN Token Received with Endpoint Mismatch
    USBD_INT_EP_IN_IEPNAKE = (uint32_t)BIT6, //!< IN endpoint NAK effective
    USBD_INT_EP_IN_TXFE    = (uint32_t)BIT7, //!< TxFIFO empty
    USBD_INT_EP_IN_TXFUDR  = (uint32_t)BIT8, /*!< FIFO underrun mask */
    USBD_INT_EP_IN_BNA     = (uint32_t)BIT9, /*!< BNA */
//    USBD_INT_EP_IN_BERR    = (uint32_t)BIT12,
//    USBD_INT_EP_IN_NAK     = (uint32_t)BIT13,
    USBD_INT_EP_IN_ALL     = (uint32_t)(0xFF)
} USBD_INT_EP_IN_T;

/**@} end of group USB_Enumerations*/

/** @defgroup USB_Structures Structures
  @{
*/

/**
 * @brief USB OTG endpoint information structure
 */
typedef struct
{
    uint8_t         epNum;
    uint8_t         epDir;
    uint8_t         epType;
    uint8_t         stallStatus;

    uint8_t         dataPID;
    uint8_t         frameParity;

    uint32_t        dmaAddr;
    uint32_t        mps;
    uint16_t        txFifoNum;
    uint8_t*        buffer;
    uint32_t        bufLen;
    uint32_t        bufCount;
} USB_OTG_ENDPOINT_INFO_T;

/**
 * @brief USB OTG FIFO structure alignment
 */
struct __attribute__((packed, aligned(1))) USB_FIFO_PACKET_T
{
    uint32_t val;
};

/**
 * @brief USB OTG init structure definition
 */
typedef struct
{
    uint32_t            vbusSense;
    uint32_t            lowPowerStatus;     /*!< Low power mode status */
    uint32_t            powerManageStatus;
    uint32_t            sofStatus;          /*!< SOF output status */
    USB_OTG_PHY_T       phyType;
    uint32_t            ep0MaxPackSize;
    uint32_t            devEndpointNum;     /*!< USB device endpoint number */
    uint32_t            dmaStatus;          /*!< Only for OTG HS */
    uint32_t            speed;              /*!< USB core speed */
    uint32_t            speedChannel;       /*!< USB speed channel */
    uint32_t            hostChannelNum;     /*!< USB host channels number */
    uint8_t             batteryStatus;      /*!< BACKUP BAT status */
    uint8_t             extVbusStatus;
    uint8_t             ep1Status;
    USB_OTG_MODE_T      mode;
} USB_OTG_CFG_T;

/**@} end of group USB_Structures*/

/** @defgroup USB_Functions Functions
  @{
*/

/* USB OTG Global */
void USB_OTG_SetTurnaroundTime(USB_OTG_GLOBAL_T* usbx, uint32_t hclkFreq, uint8_t speed);
uint32_t USB_OTG_ReadInterrupts(USB_OTG_GLOBAL_T* usbx);
void USB_OTG_ConfigRxFifoSize(USB_OTG_GLOBAL_T* usbx, uint16_t depth);
void USB_OTG_ConfigMode(USB_OTG_GLOBAL_T* usbx, USB_OTG_MODE_T mode);
void USB_OTG_EnableDMA(USB_OTG_GLOBAL_T* usbx);
void USB_OTG_EnablePowerDown(USB_OTG_GLOBAL_T* usbx);
void USB_OTG_DisablePowerDown(USB_OTG_GLOBAL_T* usbx);
void USB_OTG_SetBurstLenght(USB_OTG_GLOBAL_T* usbx, USB_OTG_BURST_T burstType);
void USB_OTG_CoreReset(USB_OTG_GLOBAL_T* usbx);
uint8_t USB_OTG_ReadDMAStatus(USB_OTG_GLOBAL_T* usbx);
uint32_t USB_OTG_PopRxFifoStatus(USB_OTG_GLOBAL_T* usbx);
void USB_OTG_FlushTxFIFO(USB_OTG_GLOBAL_T* usbx, uint8_t num);
void USB_OTG_FlushRxFIFO(USB_OTG_GLOBAL_T* usbx);
uint8_t USB_OTG_ReadMode(USB_OTG_GLOBAL_T* usbx);
void USB_OTG_ConfigPHY(USB_OTG_GLOBAL_T* usbx, USB_OTG_PHY_SPEED_T phy);
void USB_OTG_EnableAllGlobalInterrupt(USB_OTG_GLOBAL_T* usbx);
void USB_OTG_DisableAllGlobalInterrupt(USB_OTG_GLOBAL_T* usbx);
void USB_OTG_EnableGlobalInterrupt(USB_OTG_GLOBAL_T* usbx, uint32_t interrupt);
void USB_OTG_DisableGlobalInterrupt(USB_OTG_GLOBAL_T* usbx, uint32_t interrupt);
uint8_t USB_OTG_ReadGlobalIntFlag(USB_OTG_GLOBAL_T* usbx, uint32_t interrupt);
void USB_OTG_ClearGlobalIntFlag(USB_OTG_GLOBAL_T* usbx, uint32_t interrupt);

/* USB FIFO */
void USB_OTG_FIFO_WriteFifoPacket(USB_OTG_FIFO_T* usbFifo, uint8_t chEpNum, uint8_t* wBuf, uint32_t wlen, uint8_t dmaStatus);
void USB_OTG_FIFO_ReadRxFifoPacket(USB_OTG_FIFO_T* usbFifo, uint8_t* rBuf, uint32_t rLen);

/* USB OTG Host */
uint16_t USB_OTG_H_ReadFrameNum(USB_OTG_HOST_T* usbhx);
void USB_OTG_H_ConfigDMAAddr(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint32_t addr);
void USB_OTG_H_ConfigDataPID(USB_OTG_HOST_T* usbhx, uint8_t chNum, USB_OTG_DATA_PID_T pid);
void USB_OTG_H_ConfigPacketCount(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint32_t count);
void USB_OTG_H_ConfigTransferSize(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint32_t size);
void USB_OTG_H_TxUseOddFrame(USB_OTG_HOST_T* usbhx, uint8_t chNum);
void USB_OTG_H_TxUseEvenFrame(USB_OTG_HOST_T* usbhx, uint8_t chNum);
void USB_OTG_H_ConfigEpMaxPSize(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint16_t packetMaxSize);
void USB_OTG_H_ConfigEpType(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint8_t epType);
void USB_OTG_H_ConfigEpNum(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint8_t epNum);
void USB_OTG_H_ConfigDeviceAddr(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint8_t devAddr);
void USB_OTG_H_EnableLowSpeedDevice(USB_OTG_HOST_T* usbhx, uint8_t chNum);
void USB_OTG_H_DisableLowSpeedDevice(USB_OTG_HOST_T* usbhx, uint8_t chNum);
void USB_OTG_H_DriverVbus(USB_OTG_HOST_T* usbhx, USBH_PORT_PWR_STA_T state);
uint32_t USB_OTG_H_ReadTransferSize(USB_OTG_HOST_T* usbhx, uint8_t chNum);
void USB_OTG_H_ClearPovcBitChangeFlag(USB_OTG_HOST_T* usbhx);
uint8_t USB_OTG_H_PovcBitIsChange(USB_OTG_HOST_T* usbhx);
uint8_t USB_OTG_H_ReadPortSpeedType(USB_OTG_HOST_T* usbhx);
uint8_t USB_OTG_H_PortIsEnable(USB_OTG_HOST_T* usbhx);
void USB_OTG_H_ClearPenBitChangeFlag(USB_OTG_HOST_T* usbhx);
uint8_t USB_OTG_H_PenBitIsChange(USB_OTG_HOST_T* usbhx);
void USB_OTG_H_ClearPortConnectIntTriFlag(USB_OTG_HOST_T* usbhx);
uint8_t USB_OTG_H_ReadPortConnectIntTriFlag(USB_OTG_HOST_T* usbhx);
uint32_t USB_OTG_H_ReadTxQueueRemainSpace(USB_OTG_HOST_T* usbhx);
uint8_t USB_OTG_H_ReadChannelStatus(USB_OTG_HOST_T* usbhx, uint8_t chNum);
uint8_t USB_OTG_H_ReadEpType(USB_OTG_HOST_T* usbhx, uint8_t chNum);
uint8_t USB_OTG_H_ReadEpDirection(USB_OTG_HOST_T* usbhx, uint8_t chNum);
void USB_OTG_H_EnableChannel(USB_OTG_HOST_T* usbhx, uint8_t chNum);
uint32_t USB_OTG_H_ReadPacketCount(USB_OTG_HOST_T* usbhx, uint8_t chNum);
void USB_OTG_H_EnablePortPower(USB_OTG_HOST_T* usbhx);
void USB_OTG_H_DisablePortPower(USB_OTG_HOST_T* usbhx);
uint8_t USB_OTG_H_PortIsConnect(USB_OTG_HOST_T* usbhx);
void USB_OTG_H_ConfigHostClk(USB_OTG_HOST_T* usbhx, USBH_PHYCLK_T phyClock);
void USB_OTG_H_SetFrameInterval(USB_OTG_HOST_T* usbhx, uint16_t freq);
uint32_t USB_OTG_H_ReadAllChannelsIntFlag(USB_OTG_HOST_T* usbhx, uint16_t channel);
uint8_t USB_OTG_H_ReadChannelIntFlag(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint32_t interrupt);
void USB_OTG_H_ClearChannelIntFlag(USB_OTG_HOST_T* usbhx, uint8_t chNum, uint32_t interrupt);
void USB_OTG_H_ClearAllChannelIntFlag(USB_OTG_HOST_T* usbhx, uint8_t chNum);

/* USB OTG Device */
void USB_OTG_D_ConfigEP(USB_OTG_DEVICE_T* usbdx, USB_OTG_ENDPOINT_INFO_T* ep);
void USB_OTG_D_ResetEP(USB_OTG_DEVICE_T* usbdx, USB_OTG_ENDPOINT_INFO_T* ep);
uint8_t USB_OTG_D_ReadEnumSpeed(USB_OTG_DEVICE_T* usbdx);
void USB_OTG_D_ConfigDeviceAddress(USB_OTG_DEVICE_T* usbdx, uint8_t address);
void USB_OTG_D_EnableRemoteWakeupSignal(USB_OTG_DEVICE_T* usbdx);
void USB_OTG_D_DisableRemoteWakeupSignal(USB_OTG_DEVICE_T* usbdx);
void USB_OTG_D_ConfigSoftDisconnect(USB_OTG_DEVICE_T* usbdx, USBD_SOFT_DISC_T status);
void USB_OTG_D_ConfigPeriodicFrameInterval(USB_OTG_DEVICE_T* usbdx, USBD_FRAME_INTERVAL_T interval);
void USB_OTG_D_ConfigDeviceSpeed(USB_OTG_DEVICE_T* usbdx, USBD_DEV_SPEED_T speed);
void USB_OTG_D_EnableOutEpInterruptMask(USB_OTG_DEVICE_T* usbdx, uint8_t epNum);
void USB_OTG_D_DisableOutEpInterruptMask(USB_OTG_DEVICE_T* usbdx, uint8_t epNum);
void USB_OTG_D_EnableInEpInterruptMask(USB_OTG_DEVICE_T* usbdx, uint8_t epNum);
void USB_OTG_D_DisableInEpInterruptMask(USB_OTG_DEVICE_T* usbdx, uint8_t epNum);
uint16_t USB_OTG_D_ReadAllOutEpIntNumber(USB_OTG_DEVICE_T* usbdx);
uint16_t USB_OTG_D_ReadAllInEpIntNumber(USB_OTG_DEVICE_T* usbdx);
uint8_t USB_OTG_D_ReadOutEpIntStatus(USB_OTG_DEVICE_T* usbdx, uint8_t epNum, uint32_t interrupt);
uint8_t USB_OTG_D_ReadInEpIntStatus(USB_OTG_DEVICE_T* usbdx, uint8_t epNum, uint32_t interrupt);
void USB_OTG_D_ClearOutEpIntStatus(USB_OTG_DEVICE_T* usbdx, uint8_t epNum, uint32_t interrupt);
void USB_OTG_D_ClearInEpIntStatus(USB_OTG_DEVICE_T* usbdx, uint8_t epNum, uint32_t interrupt);
void USB_OTG_D_EnableInEpEmptyInterrupt(USB_OTG_DEVICE_T* usbdx, uint8_t epNum);
void USB_OTG_D_DisableInEpEmptyInterrupt(USB_OTG_DEVICE_T* usbdx, uint8_t epNum);
void USB_OTG_D_DisableAllInEpInterrupt(USB_OTG_DEVICE_T* usbdx);
void USB_OTG_D_DisableAllOutEpInterrupt(USB_OTG_DEVICE_T* usbdx);
void USB_OTG_D_DisableAllEpInterrupt(USB_OTG_DEVICE_T* usbdx);
void USB_OTG_D_EnableInEpInterrupt(USB_OTG_DEVICE_T* usbdx, uint32_t interrupt);
void USB_OTG_D_DisableInEpInterrupt(USB_OTG_DEVICE_T* usbdx, uint32_t interrupt);
void USB_OTG_D_EnableOutEpInterrupt(USB_OTG_DEVICE_T* usbdx, uint32_t interrupt);
void USB_OTG_D_DisableOutEpInterrupt(USB_OTG_DEVICE_T* usbdx, uint32_t interrupt);
void USB_OTG_D_EnableInEp1Interrupt(USB_OTG_DEVICE_T* usbdx, uint32_t interrupt);
void USB_OTG_D_DisableInEp1Interrupt(USB_OTG_DEVICE_T* usbdx, uint32_t interrupt);
void USB_OTG_D_EnableOutEp1Interrupt(USB_OTG_DEVICE_T* usbdx, uint32_t interrupt);
void USB_OTG_D_DisbleOutEp1Interrupt(USB_OTG_DEVICE_T* usbdx, uint32_t interrupt);

/**@} end of group USB_Functions*/
/**@} end of group USB_Driver*/
/**@} end of group APM32F4xx_StdPeriphDriver*/

#ifdef __cplusplus
}
#endif

#endif
