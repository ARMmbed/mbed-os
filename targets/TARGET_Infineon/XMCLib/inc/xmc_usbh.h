/**
 * @file xmc_usbh.h
 * @date 2016-06-30
 *
 *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2016-06-30:
 *     - Initial Version.<br>
 * 2016-09-01:
 *     - Removed Keil specific inclusions and macros<br>
 *
 * @endcond
 *
 */

#ifndef XMC_USBH_H
#define XMC_USBH_H

#include <stdint.h>
#include "xmc_common.h"
#include "xmc_scu.h"
#include "xmc_gpio.h"

#if((UC_SERIES == XMC45) || (UC_SERIES == XMC44) || (UC_SERIES == XMC43) || (UC_SERIES == XMC47) || (UC_SERIES == XMC48) || defined(DOXYGEN))
/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup USBH
 * @brief Universal Serial Bus Host (USBH) driver for the XMC4000 microcontroller family.
 *
 * The USBH is the host mode device driver for the USB0 hardware module on XMC4000 family of microcontrollers.
 * The USB0 module can be used to establish a USB interface between outside world and XMC4000 family of controllers.
 * The USB module includes the following features in host mode:
 * -# Complies with the USB 2.0 Specification.
 * -# Supports up to 14 bidirectional pipes, including control pipe 0.
 * -# Supports SOFs in Full-Speed modes.
 * -# Supports clock gating for power saving.
 * -# Supports FIFO mode data transaction.
 *
 * The below figure shows the overview of USB0 module in XMC4 microntroller.
 * @image html USB_module_overview.png
 * @image latex ../images/USB_module_overview.png
 *
 *
 * The USBH device driver supports the following features:\n
 * -# Initialize/Uninitialize the USB0 module on XMC4000 device.
 * -# Control VBUS state.
 * -# Reset USB port.
 * -# Set the USB device address.
 * -# Allocate pipe for new endpoint communication.
 * -# Modify an existing pipe.
 * -# Transfer data on selected pipe.
 * -# Abort ongoing data transaction.
 * -# Handle multi packet data transaction by updating toggle information.
 *
 * The USBH device driver expects registration of callback functions ::XMC_USBH_SignalPortEvent_t and ::XMC_USBH_SignalPipeEvent_t to be executed
 * when there is port event interrupt and pipe event interrupt respectively.\n
 * The USBH driver is CMSIS API compatible. Please use Driver_USBH0 to access the USBH API.\n
 * For example, to initialize the USB host controller, use Driver_USBH0.Initialize().\n
 *
 * @{
 */

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
/*Drive VBUS*/
#define XMC_USB_DRIVE_PORT1                         P3_2  /**< Default port(PORT3, pin 2) used to enable VBUS voltage regulator on the board */
#define XMC_USB_DRIVE_PORT2                         P0_1  /**< Alternate port that can be used to enable VBUS voltage regulator(PORT0, pin 1) */

#ifndef USBH0_MAX_PIPE_NUM
#define USBH0_MAX_PIPE_NUM                          (14U) /**< Representation of number of pipes available */
#endif
#if (USBH0_MAX_PIPE_NUM > 14U)
#error  Too many Pipes, maximum Pipes that this driver supports is 14 !!!
#endif

#define XMC_USBH_CLOCK_GATING_ENABLE               1  /**< Used to enable clock gating when the driver is powered down*/
#define XMC_USBH_CLOCK_GATING_DISABLE              0  /**< Used to disable clock gating when the driver is fully powered*/

#define  USB_CH_HCCHARx_MPS(x)                      (((uint32_t)    x   ) & (uint32_t)USB_CH_HCCHAR_MPS_Msk)  /**< Masks maximum packet size information from the HCCHAR register value provided as input */
#define  USB_CH_HCCHARx_EPNUM(x)                    (((uint32_t)    x   << USB_CH_HCCHAR_EPNum_Pos) & (uint32_t)USB_CH_HCCHAR_EPNum_Msk)  /**< Shifts the value to the position of endpoint number(EPNum) in the HCCHAR register*/
#define  USB_CH_HCCHARx_EPTYPE(x)                   (((uint32_t)    x   << USB_CH_HCCHAR_EPType_Pos) & (uint32_t)USB_CH_HCCHAR_EPType_Msk)  /**< Shifts the value to the position of endpoint type(EPType) in the HCCHAR register*/
#define  USB_CH_HCCHARx_MCEC(x)                     (((uint32_t)    x   << USB_CH_HCCHAR_MC_EC_Pos) & (uint32_t)USB_CH_HCCHAR_MC_EC_Msk)  /**< Shifts the value to the position of multi-count(MC_EC) field in the HCCHAR register*/
#define  USB_CH_HCCHARx_DEVADDR(x)                  (((uint32_t)    x   << USB_CH_HCCHAR_DevAddr_Pos) & (uint32_t)USB_CH_HCCHAR_DevAddr_Msk)  /**< Shifts the value to the position of endpoint type(EPType) in the HCCHAR register*/
#define  USB_CH_HCCHARx_EPDIR(x)                    (((uint32_t)    x   << USB_CH_HCCHAR_EPDir_Pos) & (uint32_t)USB_CH_HCCHAR_EPDir_Msk)  /**< Shifts the value to the position of endpoint direction(EPDir) in the HCCHAR register*/
#define  USB_CH_HCCHAR_LSDEV_Msk                    (((uint32_t)    0x1   << 15U) & 0x1U)
#define  USB_CH_HCTSIZx_DPID(x)                     (((uint32_t)    x   << USB_CH_HCTSIZ_BUFFERMODE_Pid_Pos) & (uint32_t)USB_CH_HCTSIZ_BUFFERMODE_Pid_Msk)  /**< Shifts the value to the position of packet ID (PID) in the HCTSIZ register*/
#define  USB_CH_HCTSIZx_DPID_DATA0                  (USB_CH_HCTSIZx_DPID(0U)) /**< Represents DATA toggle DATA0 as in HCTSIZ register*/
#define  USB_CH_HCTSIZx_DPID_DATA2                  (USB_CH_HCTSIZx_DPID(1U)) /**< Represents DATA toggle DATA2 as in HCTSIZ register*/
#define  USB_CH_HCTSIZx_DPID_DATA1                  (USB_CH_HCTSIZx_DPID(2U)) /**< Represents DATA toggle DATA1 as in HCTSIZ register*/
#define  USB_CH_HCTSIZx_DPID_MDATA                  (USB_CH_HCTSIZx_DPID(3U)) /**< Represents DATA toggle MDATA as in HCTSIZ register*/
#define  USB_CH_HCTSIZx_DPID_SETUP                  (USB_CH_HCTSIZx_DPID(3U)) /**< Represents SETUP token as in HCTSIZ register*/
#define  USB_GRXSTSR_HOSTMODE_PktSts_IN_DATA_PKT    0x2 /**< Represents IN data token as in receive status pop register(GRXSTSP)*/
#define  USB_GRXSTSR_HOSTMODE_PktSts_IN_TRSF_CPL    0x3 /**< Represents paket status information as in receive status pop register(GRXSTSP)*/


#define  USB_CH_HCFG_FSLSSUP(x)                     (((uint32_t)    x  <<  USB_HCFG_FSLSSupp_Pos) & USB_HCFG_FSLSSupp_Msk)  /**< Provides register value to update USB full speed related mask FLSSupp of register HCFG*/
#define  USB_CH_HCFG_FSLSPCS(x)                     (((uint32_t)    x        ) & USB_HCFG_FSLSPclkSel_Msk)  /**< Provides register value to update PHY clock selection in register HCFG*/

#define  USB_CH_HCINTx_ALL                          (USB_CH_HCINTMSK_XferComplMsk_Msk | \
                                                      USB_CH_HCINTMSK_ChHltdMsk_Msk | \
                                                      USB_CH_HCINTMSK_StallMsk_Msk | \
                                                      USB_CH_HCINTMSK_NakMsk_Msk | \
                                                      USB_CH_HCINTMSK_AckMsk_Msk | \
                                                      USB_CH_HCINTMSK_XactErrMsk_Msk | \
                                                      USB_CH_HCINTMSK_BblErrMsk_Msk | \
                                                      USB_CH_HCINTMSK_FrmOvrunMsk_Msk | \
                                                      USB_CH_HCINTMSK_DataTglErrMsk_Msk)  /**< Mask for selecting all channel related events*/

#define  USB_CH_HCINTx_ERRORS                       (USB_CH_HCINTMSK_XactErrMsk_Msk | \
                                                      USB_CH_HCINTMSK_BblErrMsk_Msk | \
                                                      USB_CH_HCINTMSK_FrmOvrunMsk_Msk | \
                                                      USB_CH_HCINTMSK_DataTglErrMsk_Msk)  /**< Mask for selecting all channel error related events*/
/*Macro to find pipe index using handle*/
#define USBH_PIPE_GET_INDEX(handle)                 (((uint32_t)handle - (uint32_t)USB0_CH0_BASE)/(0x20U))  /**< Macro provides index of the USB channel based on its base address*/

#define XMC_USBH_API_VERSION                        ((uint16_t)((uint16_t)XMC_LIB_MAJOR_VERSION << 8U) |XMC_LIB_MINOR_VERSION)  /**<  USBH low level driver API version */

/* General return codes */
#define XMC_USBH_DRIVER_OK                           0 /**< Operation succeeded */
#define XMC_USBH_DRIVER_ERROR                       -1 /**< Unspecified error */
#define XMC_USBH_DRIVER_ERROR_BUSY                  -2 /**< Driver is busy*/
#define XMC_USBH_DRIVER_ERROR_TIMEOUT               -3 /**< Timeout occurred */
#define XMC_USBH_DRIVER_ERROR_UNSUPPORTED           -4 /**< Operation not supported*/
#define XMC_USBH_DRIVER_ERROR_PARAMETER             -5 /**< Parameter error*/
#define XMC_USBH_DRIVER_ERROR_SPECIFIC              -6 /**< Start of driver specific errors*/

/* USB Speed */
#define XMC_USBH_SPEED_LOW                          0U       /**< Low-speed USB*/
#define XMC_USBH_SPEED_FULL                         1U       /**< Full-speed USB*/
#define XMC_USBH_SPEED_HIGH                         2U       /**< High-speed USB*/

/* USB Endpoint Type */
#define XMC_USBH_ENDPOINT_CONTROL                   0       /**< Control Endpoint*/
#define XMC_USBH_ENDPOINT_ISOCHRONOUS               1       /**< Isochronous Endpoint*/
#define XMC_USBH_ENDPOINT_BULK                      2       /**< Bulk Endpoint*/
#define XMC_USBH_ENDPOINT_INTERRUPT                 3       /**< Interrupt Endpoint*/

#define XMC_USBH_SignalEndpointEvent_t              XMC_USBH_SignalPipeEvent_t  /**< Legacy name for the pipe event handler*/

/****** USB Host Packet Information *****/
#define XMC_USBH_PACKET_TOKEN_Pos                    0 /**< Packet token position*/
#define XMC_USBH_PACKET_TOKEN_Msk                   (0x0FUL << XMC_USBH_PACKET_TOKEN_Pos)  /**< Packet token mask*/
#define XMC_USBH_PACKET_SETUP                       (0x01UL << XMC_USBH_PACKET_TOKEN_Pos)  /**< SETUP Packet*/
#define XMC_USBH_PACKET_OUT                         (0x02UL << XMC_USBH_PACKET_TOKEN_Pos)  /**< OUT Packet*/
#define XMC_USBH_PACKET_IN                          (0x03UL << XMC_USBH_PACKET_TOKEN_Pos)  /**< IN Packet*/
#define XMC_USBH_PACKET_PING                        (0x04UL << XMC_USBH_PACKET_TOKEN_Pos)  /**< PING Packet*/

#define XMC_USBH_PACKET_DATA_Pos                     4  /**< Packet data PID position*/
#define XMC_USBH_PACKET_DATA_Msk                    (0x0FUL << XMC_USBH_PACKET_DATA_Pos)   /**< Packet data PID mask*/
#define XMC_USBH_PACKET_DATA0                       (0x01UL << XMC_USBH_PACKET_DATA_Pos)   /**< DATA0 PID */
#define XMC_USBH_PACKET_DATA1                       (0x02UL << XMC_USBH_PACKET_DATA_Pos)   /**< DATA1 PID */

#define XMC_USBH_PACKET_SPLIT_Pos                    8
#define XMC_USBH_PACKET_SPLIT_Msk                   (0x0FUL << XMC_USBH_PACKET_SPLIT_Pos)
#define XMC_USBH_PACKET_SSPLIT                      (0x08UL << XMC_USBH_PACKET_SPLIT_Pos)  /**< SSPLIT Packet */
#define XMC_USBH_PACKET_SSPLIT_S                    (0x09UL << XMC_USBH_PACKET_SPLIT_Pos)  /**< SSPLIT Packet: Data Start */
#define XMC_USBH_PACKET_SSPLIT_E                    (0x0AUL << XMC_USBH_PACKET_SPLIT_Pos)  /**< SSPLIT Packet: Data End */
#define XMC_USBH_PACKET_SSPLIT_S_E                  (0x0BUL << XMC_USBH_PACKET_SPLIT_Pos)  /**< SSPLIT Packet: Data All */
#define XMC_USBH_PACKET_CSPLIT                      (0x0CUL << XMC_USBH_PACKET_SPLIT_Pos)  /**< CSPLIT Packet */

#define XMC_USBH_PACKET_PRE                         (1UL << 12)                            /**< PRE Token */


/****** USB Host Port Event *****/
#define XMC_USBH_EVENT_CONNECT                      (1UL << 0)     /**< USB Device Connected to Port */
#define XMC_USBH_EVENT_DISCONNECT                   (1UL << 1)     /**< USB Device Disconnected from Port */
#define XMC_USBH_EVENT_OVERCURRENT                  (1UL << 2)     /**< USB Device caused Overcurrent */
#define XMC_USBH_EVENT_RESET                        (1UL << 3)     /**< USB Reset completed */
#define XMC_USBH_EVENT_SUSPEND                      (1UL << 4)     /**< USB Suspend occurred */
#define XMC_USBH_EVENT_RESUME                       (1UL << 5)     /**< USB Resume occurred */
#define XMC_USBH_EVENT_REMOTE_WAKEUP                (1UL << 6)     /**< USB Device activated Remote Wakeup */

/****** USB Host Pipe Event *****/
#define XMC_USBH_EVENT_TRANSFER_COMPLETE            (1UL << 0)     /**< Transfer completed */
#define XMC_USBH_EVENT_HANDSHAKE_NAK                (1UL << 1)     /**< NAK Handshake received */
#define XMC_USBH_EVENT_HANDSHAKE_NYET               (1UL << 2)     /**< NYET Handshake received */
#define XMC_USBH_EVENT_HANDSHAKE_MDATA              (1UL << 3)     /**< MDATA Handshake received */
#define XMC_USBH_EVENT_HANDSHAKE_STALL              (1UL << 4)     /**< STALL Handshake received */
#define XMC_USBH_EVENT_HANDSHAKE_ERR                (1UL << 5)     /**< ERR Handshake received */
#define XMC_USBH_EVENT_BUS_ERROR                    (1UL << 6)     /**< Bus Error detected */
/*******************************************************************************
 * ENUMS
 *******************************************************************************/
/**
 * @brief General power states of USB peripheral driver
*/
typedef enum XMC_USBH_POWER_STATE {
  XMC_USBH_POWER_OFF,                        /**< Power off: no operation possible */
  XMC_USBH_POWER_LOW,                        /**< Low Power mode: retain state, detect and signal wake-up events */
  XMC_USBH_POWER_FULL                        /**< Power on: full operation at maximum performance */
} XMC_USBH_POWER_STATE_t;
/*******************************************************************************
 * DATA STRUCTURES
 *******************************************************************************/
/**
 * @brief USB host Driver Version
*/
typedef struct XMC_USBH_DRIVER_VERSION {
  uint16_t api;                         /**< API version */
  uint16_t drv;                         /**< Driver version */
} XMC_USBH_DRIVER_VERSION_t;


/**
 * @brief USB Host Port State
*/
typedef struct XMC_USBH_PORT_STATE {
  uint32_t connected   : 1;             /**< USB Host Port connected flag */
  uint32_t overcurrent : 1;             /**< USB Host Port overcurrent flag */
  uint32_t speed       : 2;             /**< USB Host Port speed setting (ARM_USB_SPEED_xxx) */
} XMC_USBH_PORT_STATE_t;

/**
 * @brief USB Host Pipe Handle. It represents the physical address of a USB channel
*/
typedef uint32_t XMC_USBH_PIPE_HANDLE;
#define XMC_USBH_EP_HANDLE XMC_USBH_PIPE_HANDLE  /**< Legacy name for pipe handle used by CMSIS*/

/**
 * @brief USB Host Driver Capabilities.
*/
typedef struct XMC_USBH_CAPABILITIES {
  uint32_t port_mask          : 15;     /**< Root HUB available Ports Mask */
  uint32_t auto_split         :  1;     /**< Automatic SPLIT packet handling */
  uint32_t event_connect      :  1;     /**< Signal Connect event */
  uint32_t event_disconnect   :  1;     /**< Signal Disconnect event */
  uint32_t event_overcurrent  :  1;     /**< Signal Overcurrent event */
} XMC_USBH_CAPABILITIES_t;


typedef void (*XMC_USBH_SignalPortEvent_t) (uint8_t port, uint32_t event);                    /**< Pointer to \ref ARM_USBH_SignalPortEvent : Signal Root HUB Port Event. */
typedef void (*XMC_USBH_SignalPipeEvent_t) (XMC_USBH_PIPE_HANDLE pipe_hndl, uint32_t event);  /**< Pointer to \ref ARM_USBH_SignalPipeEvent : Signal Pipe Event. */

/**
 * @brief Access structure of USB Host Driver.
*/
typedef struct XMC_USBH_DRIVER {
  XMC_USBH_DRIVER_VERSION_t    (*GetVersion)            (void);                                /**< Pointer to \ref ARM_USBH_GetVersion : Get driver version. */
  XMC_USBH_CAPABILITIES_t (*GetCapabilities)       (void);                                     /**< Pointer to \ref ARM_USBH_GetCapabilities : Get driver capabilities. */
  int32_t               (*Initialize)            (XMC_USBH_SignalPortEvent_t cb_port_event,
                                                  XMC_USBH_SignalPipeEvent_t cb_pipe_event); /**< Pointer to \ref ARM_USBH_Initialize : Initialize USB Host Interface. */
  int32_t               (*Uninitialize)          (void);                                     /**< Pointer to \ref ARM_USBH_Uninitialize : De-initialize USB Host Interface. */
  int32_t               (*PowerControl)          (XMC_USBH_POWER_STATE_t state);                    /**< Pointer to \ref ARM_USBH_PowerControl : Control USB Host Interface Power. */
  int32_t               (*PortVbusOnOff)         (uint8_t port, bool vbus);                  /**< Pointer to \ref ARM_USBH_PortVbusOnOff : Root HUB Port VBUS on/off. */
  int32_t               (*PortReset)             (uint8_t port);                             /**< Pointer to \ref ARM_USBH_PortReset : Do Root HUB Port Reset. */
  int32_t               (*PortSuspend)           (uint8_t port);                             /**< Pointer to \ref ARM_USBH_PortSuspend : Suspend Root HUB Port (stop generating SOFs). */
  int32_t               (*PortResume)            (uint8_t port);                             /**< Pointer to \ref ARM_USBH_PortResume : Resume Root HUB Port (start generating SOFs). */
  XMC_USBH_PORT_STATE_t   (*PortGetState)          (uint8_t port);                           /**< Pointer to \ref ARM_USBH_PortGetState : Get current Root HUB Port State. */
  XMC_USBH_PIPE_HANDLE  (*PipeCreate)            (uint8_t dev_addr,
                                                  uint8_t dev_speed,
                                                  uint8_t hub_addr,
                                                  uint8_t hub_port,
                                                  uint8_t ep_addr,
                                                  uint8_t ep_type,
                                                  uint16_t ep_max_packet_size,
                                                  uint8_t ep_interval);                      /**< Pointer to \ref ARM_USBH_PipeCreate : Create Pipe in System. */
  int32_t               (*PipeModify)            (XMC_USBH_PIPE_HANDLE pipe_hndl,
                                                  uint8_t dev_addr,
                                                  uint8_t dev_speed,
                                                  uint8_t hub_addr,
                                                  uint8_t hub_port,
                                                  uint16_t ep_max_packet_size);              /**< Pointer to \ref ARM_USBH_PipeModify : Modify Pipe in System. */
  int32_t               (*PipeDelete)            (XMC_USBH_PIPE_HANDLE pipe_hndl);           /**< Pointer to \ref ARM_USBH_PipeDelete : Delete Pipe from System. */
  int32_t               (*PipeReset)             (XMC_USBH_PIPE_HANDLE pipe_hndl);           /**< Pointer to \ref ARM_USBH_PipeReset : Reset Pipe. */
  int32_t               (*PipeTransfer)          (XMC_USBH_PIPE_HANDLE pipe_hndl,
                                                  uint32_t packet,
                                                  uint8_t *data,
                                                  uint32_t num);                             /**< Pointer to \ref ARM_USBH_PipeTransfer : Transfer packets through USB Pipe. */
  uint32_t              (*PipeTransferGetResult) (XMC_USBH_PIPE_HANDLE pipe_hndl);           /**< Pointer to \ref ARM_USBH_PipeTransferGetResult : Get result of USB Pipe transfer. */
  int32_t               (*PipeTransferAbort)     (XMC_USBH_PIPE_HANDLE pipe_hndl);           /**< Pointer to \ref ARM_USBH_PipeTransferAbort : Abort current USB Pipe transfer. */
  uint16_t              (*GetFrameNumber)        (void);                                     /**< Pointer to \ref ARM_USBH_GetFrameNumber : Get current USB Frame Number. */
} const XMC_USBH_DRIVER_t;


/**
 * @brief Structure to handle various states of USB host driver. An instance exists for each USB channel
 */
typedef struct XMC_USBH0_pipe {
  uint32_t  packet;                 /**< Holds packet token and PID information of ongoing data packet transaction*/
  uint8_t  *data;                   /**< Holds address of data buffer. It represents source buffer for OUT or SETUP transfer and
                                          destination address for IN transfer*/
  uint32_t  num;                    /**< Number of bytes of data to be transmitted*/
  uint32_t  num_transferred_total;  /**< Number of bytes transmitted or received at the moment*/
  uint32_t  num_transferring;       /**< Number of bytes being transmitted currently*/
  uint16_t  ep_max_packet_size;     /**< Maximum packet size for the selected pipe*/
  uint16_t  interval_reload;        /**< For INTERRUPT or ISOCHRONOUS pipe, it represents the period for repeated transfer*/
  uint16_t  interval;               /**< For INTERRUPT or ISOCHRONOUS pipe, it represents the decrementing count to reach 0 for initiating retransmission*/
  uint8_t   ep_type;                /**< Endpoint type for selected pipe*/
  uint8_t   in_use;                 /**< Set to true when transfer is in progress and reset only after the /ref num of bytes is transferred*/
  uint8_t   transfer_active;        /**< Set to true when a transfer has been initiated and reset when event for transfer complete occurs*/
	uint8_t   interrupt_triggered;    /**< For INTERRUPT or ISOCHRONOUS pipe, indicates if retransmit timeout has occurred*/
  uint8_t   event;                  /**< Holds pipe specific event flags*/
} XMC_USBH0_pipe_t;


typedef struct xmc_usb_host_device {
	USB0_GLOBAL_TypeDef *global_register;           /**< Global register interface            */
	USB0_CH_TypeDef *host_channel_registers;        /**< Host channel interface               */
	XMC_USBH_SignalPortEvent_t SignalPortEvent_cb;  /**< Port event callback; set during init */
  XMC_USBH_SignalPipeEvent_t SignalPipeEvent_cb;  /**< Pipe event callback; set during init */
	bool init_done;                                 /**< init status */
	XMC_USBH_POWER_STATE_t power_state;             /**< USB Power status */
	bool port_reset_active;                         /**< Port reset state */
} XMC_USBH0_DEVICE_t;

/*******************************************************************************
 * API PROTOTYPES
 *******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param gintsts USB global interrupt status.
 * @return None.
 *
 * \par<b>Description:</b><br>
 * Updates logical state of the USB host driver based on the input status value. It handles port interrupt
 * and channel interrupt. It responsible for updating data toggle information for multi-packet data transmission.
 * It executes the callback function on transfer completion and reception of data. It also does error management and
 * calls the relevant callback functions to indicate it to the application.
 */
void XMC_USBH_HandleIrq (uint32_t gintsts);
/**
 * @param ms Delay in milliseconds.
 * @return uint8_t Value has no significance for the low level driver.
 *
 * \par<b>Description:</b><br>
 * Function implements time delay logic. The USB host low level driver provides a weak definition
 * for delay which has to re-implemented with time delay logic. The low level driver expects blocking
 * implementation of the delay.
 */
 uint8_t XMC_USBH_osDelay(uint32_t ms);
 
/**
 * @param port Address of the port which has the pin used to enable VBUS charge pump.
 * @param pin Pin number in the port selected in previous argument using which the VBUS charge pump has to be enabled.
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the port pin with alternate output function 1. VBUS enabling pins work with alternate output function 1. \n
 * <i>Note:</i>The input port pin should support USB VBUS as an alternate function. \n
 * Typical ports that support VBUS enable are: P3_2 and P0_1.
 *
 */
void XMC_USBH_Select_VBUS(XMC_GPIO_PORT_t* port, uint32_t pin);

/**
 * @return USB host mode interrupt status. Bit field USB0_BASE->GINTSTS_HOSTMODE
 *
 * \par<b>Description:</b><br>
 * Provides USB host global interrupt status. \n
 * This value can be used to provide interrupt status to the IRQ handler function XMC_USBH_HandleIrq().
 *
 */
uint32_t XMC_USBH_GetInterruptStatus(void);

/**
 * @return None
 *
 * \par<b>Description:</b><br>
 * De-asserts resume bit. \n
 * The function shall be called 20ms after detecting port remote wakeup event. \n
 *
 */
void XMC_USBH_TurnOffResumeBit(void);
#ifdef __cplusplus
}
#endif
/**
 * @}
 */

/**
 * @}
 */
#endif
#endif /* XMC_USBH_H */


