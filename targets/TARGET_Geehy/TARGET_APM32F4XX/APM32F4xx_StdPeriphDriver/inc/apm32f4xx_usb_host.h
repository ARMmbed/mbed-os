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

/* Define to prevent recursive inclusion */
#ifndef __APM32F4XX_USB_HOST_H
#define __APM32F4XX_USB_HOST_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "apm32f4xx.h"
#include "apm32f4xx_usb.h"

/** @addtogroup APM32F4xx_StdPeriphDriver
  @{
*/

/** @addtogroup USB_Host_Driver USB Host Driver
  @{
*/

/** @defgroup USB_Host_Macros Macros
  @{
*/

#define USBH_FS_RX_FIFO_SIZE                    128
#define USBH_FS_HNP_TXH_FIFO_SIZE               96
#define USBH_FS_HP_TXH_FIFO_SIZE                64
#define USBH_FS_HP_TXH_FIFO_SA                  224

#define USBH_HS_RX_FIFO_SIZE                    512
#define USBH_HS_HNP_TXH_FIFO_SIZE               256
#define USBH_HS_HP_TXH_FIFO_SIZE                224
#define USBH_HS_HP_TXH_FIFO_SA                  768

/**@} end of group USB_Host_Macros*/

/** @defgroup USB_Host_Enumerations Enumerations
  @{
*/

/**
 * @brief USB host device speed type
 */
typedef enum
{
    USBH_DEV_SPEED_HIGH,
    USBH_DEV_SPEED_FULL,
    USBH_DEV_SPEED_LOW,
} USBH_DEV_SPEED_T;

/**@} end of group USB_Host_Enumerations*/

/** @defgroup USB_Host_Structures Structures
  @{
*/

/**
 * @brief USB OTG host FIFO status
 */
typedef struct
{
    union
    {
        __IO uint32_t FIFO_STATUS;

        struct
        {
            __IO uint32_t chNum             : 4;
            __IO uint32_t byteCount         : 11;
            __IO uint32_t dataPid           : 2;
            __IO uint32_t packetStatus      : 4;
        } FIFO_STATUS_B;
    };
} USBH_FIFO_STA_T;

/**
 * @brief   USB Host transfer buffer management
 */
typedef struct
{
    uint8_t*             buffer;
    uint32_t            bufLen;
    uint32_t            bufSize;
    uint32_t            bufCount;
    USB_OTG_URB_STA_T   urbStatus;
    uint16_t            maxPackSize;
    uint8_t             pidToggleIn;
    uint8_t             pidToggleOut;
    uint8_t             pipeState;
    uint32_t            errorCnt;
    uint8_t             epType;
    uint8_t             epNum;
    USB_EP_DIR_T        epDir;
    uint8_t             pingStatus;
    uint8_t             speed;
    uint8_t             devAddr;
    uint8_t             chNum;
    USB_OTG_DATA_PID_T  pid;
} USBH_XFER_PIPE_T;

/**
 * @brief USB OTG host handle
 */
typedef struct
{
    USB_OTG_GLOBAL_T*    usbGlobal;
    USB_OTG_HOST_T*      usbHost;
    USB_OTG_FIFO_T*      usbFifo;
    USB_OTG_PWRCLK_T*    usbPower;
    USB_OTG_CFG_T        usbCfg;
    USBH_XFER_PIPE_T     xferPipe[16];
    void*                dataPoint;
} USBH_HANDLE_T;

/**@} end of group USB_Host_Structures*/

/** @defgroup USB_Host_Functions Functions
  @{
*/

void USBH_Config(USBH_HANDLE_T* usbhh);
void USBH_OTG_IsrHandler(USBH_HANDLE_T* usbhh);
void USBH_OTG_StopHost(USBH_HANDLE_T* usbhh);
void USBH_Start(USBH_HANDLE_T* usbhh);
void USBH_OTG_ResetHost(USBH_HANDLE_T* usbhh);
USB_OTG_URB_STA_T USBH_OTG_ReadUrbStatus(USBH_HANDLE_T* usbhh, uint8_t channelNum);
uint8_t USBH_OTG_ReadSpeed(USBH_HANDLE_T* usbhh);
void USBH_OTG_OpenChannel(USBH_HANDLE_T* usbhh, uint8_t channelNum, \
                          uint8_t endPointNum, uint8_t devAddr, \
                          uint8_t devSpeed, uint8_t epType, uint16_t packetMaxSize);

void USBH_OTG_ChannelSubReq(USBH_HANDLE_T* usbhh, uint8_t channelNum, uint8_t dir, \
                            uint8_t epType, uint8_t tokenType, uint8_t* buffer, \
                            uint16_t length, uint8_t pingStatus);

void USBH_ConfigDataPid(USBH_HANDLE_T* usbhh, uint8_t channelNum, uint8_t dataPid);
void USBH_CloseChannel(USBH_HANDLE_T* usbhh, uint8_t channelNum);
uint32_t USBH_OTG_ReadXferSize(USBH_HANDLE_T* usbhh, uint8_t channelNum);
uint8_t USBH_OTG_ReadToggle(USBH_HANDLE_T* usbhh, uint8_t channelNum);
void USBH_Reset(USBH_HANDLE_T* usbhh);
void USBH_OTG_ConfigToggle(USBH_HANDLE_T* usbhh, uint8_t channelNum, uint8_t toggle);

void USBH_SOFCallback(USBH_HANDLE_T* usbhh);
void USBH_UpdateUrbCallback(USBH_HANDLE_T* usbhh);
void USBH_PortDisableCallback(USBH_HANDLE_T* usbhh);
void USBH_PortEnableCallback(USBH_HANDLE_T* usbhh);
void USBH_ConnectCallback(USBH_HANDLE_T* usbhh);
void USBH_DisconnectCallback(USBH_HANDLE_T* usbhh);
void USBH_HardwareResetCallback(USBH_HANDLE_T* usbhh);

/**@} end of group USB_Host_Functions*/
/**@} end of group USB_Host_Driver*/
/**@} end of group APM32F4xx_StdPeriphDriver*/

#ifdef __cplusplus
}
#endif

#endif
