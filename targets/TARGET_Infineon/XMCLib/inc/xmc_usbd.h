/**
 * @file xmc_usbd.h
 * @date 2015-06-20
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
 * 2015-02-16:
 *     - Initial Version.<br>           
 * 2015-03-18:
 *     - Updated the doxygen comments for documentation. <br>
 *     - Updated the XMC_USBD_PATCH_VERSION to 4. <br>
 *
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API.<br>
 *     - Updated the doxygen comments for API XMC_USBD_IsEnumDone().<br>
 *     - Updated the copy right in the file header.<br>
 *
 * @endcond 
 *
 */

#ifndef XMC_USBD_H
#define XMC_USBD_H

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
#include "xmc_common.h"

#if defined(USB0)

#include <stdlib.h>
#include <string.h>
#include "xmc_usbd_regs.h"
#include "xmc_scu.h"

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup USBD
 * @brief Universal Serial Bus Device (USBD) driver for the XMC4000 microcontroller family.
 *
 * The USBD is the device driver for the USB0 hardware module on XMC4000 family of microcontrollers.
 * The USB0 module can be used to establish a USB interface between outside world and XMC4000 family of controllers.
 * The USB module includes the following features in device mode:
 * -# Complies with the USB 2.0 Specification.
 * -# Support for the Full-Speed (12-Mbps) mode.
 * -# Supports up to 7 bidirectional endpoints, including control endpoint 0.
 * -# Supports SOFs in Full-Speed modes.
 * -# Supports clock gating for power saving.
 * -# Supports USB suspend/resume.
 * -# Supports USB soft disconnect.
 * -# Supports DMA mode.
 * -# Supports FIFO mode.
 * 
 * The below figure shows the overview of USB0 module in XMC4 microntroller.
 * @image html USB_module_overview.png
 * @image latex ../images/USB_module_overview.png 
 *
 * The below figure shows the USB device connection of USB0 module.
 * @image html USB_device_connection.png
 * @image latex ../images/USB_device_connection.png
 *
 * The USBD device driver supports the following features:\n
 * -# Initialize/Uninitialize the USB0 module on XMC4000 device.
 * -# Connect the USB device to host.
 * -# Get USB device state.
 * -# Set the USB device address.
 * -# Configure/Unconfigure the USB endpoints.
 * -# Stall/Abort the USB endpoints.
 * -# USB IN transfers on EP0 and non EP0 endpoints.
 * -# USB OUT transfers on EP0 and non EP0 endpoints.
 * 
 * The USBD device driver provides the configuration structure ::XMC_USBD_t which user need to configure before initializing the USB.\n
 * The following elements of configuration structure need to be initialized before calling the ::XMC_USBD_Init API:
 * -# cb_xmc_device_event of type ::XMC_USBD_SignalDeviceEvent_t.
 * -# cb_endpoint_event of type ::XMC_USBD_SignalEndpointEvent_t.
 * -# usbd_max_num_eps of type ::XMC_USBD_MAX_NUM_EPS_t.
 * -# usbd_transfer_mode of type ::XMC_USBD_TRANSFER_MODE_t.
 *
 * @{
 */

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
 
#define XMC_USBD_NUM_TX_FIFOS                          (7U) 	/**< Number of hardware transmission endpoint fifos */

#define XMC_USBD_MAX_FIFO_SIZE                         (2048U)  /**< Maximum USBD endpoint fifo size */

#define XMC_USBD_NUM_EPS                               (7U)		/**< Number of hardware endpoints */

#define	XMC_USBD_MAX_PACKET_SIZE                       (64U)	/**< Maximum packet size for all endpoints 
	                                                                (including ep0) */

/**< Maximum transfer size for endpoints.
 *
 * It's based on the maximum payload, due to the fact,
 * that we only can transfer 2^10 - 1 packets and this is less than the
 * transfer size field can hold.
 */
#define XMC_USBD_MAX_TRANSFER_SIZE                     (((uint32_t)((uint32_t)1U << (uint32_t)10U) - 1U) * (uint32_t)XMC_USBD_MAX_PACKET_SIZE)

#define XMC_USBD_MAX_TRANSFER_SIZE_EP0                 (64U)	/**< Maximum transfer size for endpoint 0*/

#define XMC_USBD_SETUP_COUNT                           (3U)		/**< The number of USB setup packets */

#define XMC_USBD_SETUP_SIZE                            (8U)		/**< The size of USB setup data */

#define XMC_USBD_EP_NUM_MASK                           (0x0FU)	/**< USB Endpoint number mask. */

#define XMC_USBD_EP_DIR_MASK                           (0x80U)	/**< USB Endpoint direction mask */

#define XMC_USBD_DCFG_DEVSPD_FS	                       (0x3U)	/*USB Full Speed device flag in DCFG register */

#define XMC_USBD_TX_FIFO_REG_OFFSET                    (0x1000U)/* First endpoint fifo register offset from base address */

#define XMC_USBD_TX_FIFO_OFFSET		                   (0x1000U)/* Offset for each fifo register */

#define XMC_USBD_ENDPOINT_NUMBER_MASK                  (0x0FU)	/**< USB Endpoint number mask to get the EP number from address. */

#define XMC_USBD_ENDPOINT_DIRECTION_MASK               (0x80U)	/**< USB Endpoint direction mask to get the EP direction from address. */

#define XMC_USBD_ENDPOINT_MAX_PACKET_SIZE_MASK         (0x07FFU)/**< USB Endpoint Maximum Packet Size mask */

#define XMC_USBD_ENDPOINT_MFRAME_TR_MASK 			         (0x1800U)/* USB Endpoint micro frame TR mask */
#define XMC_USBD_ENDPOINT_MFRAME_TR_1                  (0x0000U)/* Selects USB Endpoint micro frame TR1 */
#define XMC_USBD_ENDPOINT_MFRAME_TR_2                  (0x0800U)/* Selects USB Endpoint micro frame TR2 */
#define XMC_USBD_ENDPOINT_MFRAME_TR_3                  (0x1000U)/* Selects USB Endpoint micro frame TR3 */


#define XMC_USBD_SPEED_FULL                            (1U)		/**< Speed Mode. Full Speed */

#define XMC_USBD_EP0_BUFFER_SIZE                       (64U)	/* Endpoint 0 buffer size */

#define XMC_USBD_EP1_BUFFER_SIZE                       (64U)	/* Endpoint 1 buffer size */

#define XMC_USBD_EP2_BUFFER_SIZE                       (64U)	/* Endpoint 2 buffer size */

#define XMC_USBD_EP3_BUFFER_SIZE                       (64U)	/* Endpoint 3 buffer size */

#define XMC_USBD_EP4_BUFFER_SIZE                       (64U)	/* Endpoint 4 buffer size */

#define XMC_USBD_EP5_BUFFER_SIZE                       (64U)	/* Endpoint 5 buffer size */

#define XMC_USBD_EP6_BUFFER_SIZE                       (64U)	/* Endpoint 6 buffer size */


/**********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/

/**
 * Defines the options for the global receive fifo packet status.
 * Use type ::XMC_USBD_GRXSTS_PKTSTS_t for this enum.
 * */
typedef enum XMC_USBD_GRXSTS_PKTSTS {
  XMC_USBD_GRXSTS_PKTSTS_GOUTNAK   = 0x1U,      				/**< Global out nack send ( triggers an interrupt ) */
  XMC_USBD_GRXSTS_PKTSTS_OUTDATA   = 0x2U,      				/**< OUT data packet received */
  XMC_USBD_GRXSTS_PKTSTS_OUTCMPL   = 0x3U,      				/**< OUT transfer completed (triggers an interrupt) */
  XMC_USBD_GRXSTS_PKTSTS_SETUPCMPL = 0x4U,      				/**< SETUP transaction completed (triggers an interrupt) */
  XMC_USBD_GRXSTS_PKTSTS_SETUP     = 0x6U       				/**< SETUP data packet received */
} XMC_USBD_GRXSTS_PKTSTS_t;

/**
*  Defines the options for the USB endpoint type. The values are from the USB 2.0 specification.
*  Use type ::XMC_USBD_ENDPOINT_TYPE_t for this enum.
*/
typedef enum XMC_USBD_ENDPOINT_TYPE {
  XMC_USBD_ENDPOINT_TYPE_CONTROL        = 0x0U,      			/**< Control endpoint */
  XMC_USBD_ENDPOINT_TYPE_ISOCHRONOUS    = 0x01U,     			/**< Isochronous endpoint */
  XMC_USBD_ENDPOINT_TYPE_BULK           = 0x02U,     			/**< Bulk endpoint */
  XMC_USBD_ENDPOINT_TYPE_INTERRUPT      = 0x03U      			/**< Interrupt endpoint */
} XMC_USBD_ENDPOINT_TYPE_t;


/**
*  Defines the options for USB device state while setting the address.
*  Use type ::XMC_USBD_SET_ADDRESS_STAGE_t for this enum.
*/
typedef enum XMC_USBD_SET_ADDRESS_STAGE {
  XMC_USBD_SET_ADDRESS_STAGE_SETUP,             				/**< Setup address */
  XMC_USBD_SET_ADDRESS_STAGE_STATUS             				/**< Status address */
} XMC_USBD_SET_ADDRESS_STAGE_t;


/**
* Defines the USB Device Status of executed operation.
* Use type ::XMC_USBD_STATUS_t for this enum.
*/
typedef enum XMC_USBD_STATUS {
  XMC_USBD_STATUS_OK       = 0U,         						/**< USBD Status: Operation succeeded*/
  XMC_USBD_STATUS_BUSY     = 2U,         						/**< Driver is busy and cannot handle request */
  XMC_USBD_STATUS_ERROR    = 1U          						/**< USBD Status: Unspecified error*/
} XMC_USBD_STATUS_t;


/**
* Defines the USB Device events.
* Use type ::XMC_USBD_EVENT_t for this enum.
*/
typedef enum XMC_USBD_EVENT {
  XMC_USBD_EVENT_POWER_ON,              						/**< USB Device Power On */
  XMC_USBD_EVENT_POWER_OFF,             						/**< USB Device Power Off */
  XMC_USBD_EVENT_CONNECT,               						/**< USB Device connected */
  XMC_USBD_EVENT_DISCONNECT,            						/**< USB Device disconnected */
  XMC_USBD_EVENT_RESET,                 						/**< USB Reset occurred */
  XMC_USBD_EVENT_HIGH_SPEED,            						/**< USB switch to High Speed occurred */
  XMC_USBD_EVENT_SUSPEND,               						/**< USB Suspend occurred */
  XMC_USBD_EVENT_RESUME,                						/**< USB Resume occurred */
  XMC_USBD_EVENT_REMOTE_WAKEUP,       							/**< USB Remote wakeup */
  XMC_USBD_EVENT_SOF, 											/**< USB Start of frame event */
  XMC_USBD_EVENT_EARLYSUSPEND,          						/**< USB Early suspend */
  XMC_USBD_EVENT_ENUMDONE,              						/**< USB enumeration done */
  XMC_USBD_EVENT_ENUMNOTDONE,           						/**< USB enumeration not done */
  XMC_USBD_EVENT_OUTEP,                 						/**< USB OUT endpoint */
  XMC_USBD_EVENT_INEP                   						/**< USB IN endpoint */
} XMC_USBD_EVENT_t;

/**
* Defines the USB IN endpoint events.
* Use type ::XMC_USBD_EVENT_IN_EP_t for this enum.
*/
typedef enum XMC_USBD_EVENT_IN_EP {
  XMC_USBD_EVENT_IN_EP_TX_COMPLET = 1U,      					/**< USB IN ep transmission complete */
  XMC_USBD_EVENT_IN_EP_DISABLED = 2U,        					/**< USB IN ep disabled */
  XMC_USBD_EVENT_IN_EP_AHB_ERROR = 4U,       					/**< USB IN ep AHB error */
  XMC_USBD_EVENT_IN_EP_TIMEOUT = 8U,         					/**< USB IN ep timeout */
} XMC_USBD_EVENT_IN_EP_t;

/**
* Defines the USB OUT endpoint events.
* Use type ::XMC_USBD_EVENT_OUT_EP_t for this enum.
*/
typedef enum XMC_USBD_EVENT_OUT_EP {
  XMC_USBD_EVENT_OUT_EP_TX_COMPLET = 1U,      					/**< USB OUT ep transmission complete */
  XMC_USBD_EVENT_OUT_EP_DISABLED = 2U,        					/**< USB OUT ep disabled */
  XMC_USBD_EVENT_OUT_EP_AHB_ERROR = 4U,       					/**< USB OUT ep AHB error */
  XMC_USBD_EVENT_OUT_EP_SETUP = 8U,           					/**< USB OUT ep setup */
} XMC_USBD_EVENT_OUT_EP_t;


/**
* Defines the generic USB endpoint events.
* Use type ::XMC_USBD_EP_EVENT_t for this enum.
*/
typedef enum XMC_USBD_EP_EVENT {
  XMC_USBD_EP_EVENT_SETUP,              						/**< SETUP packet*/
  XMC_USBD_EP_EVENT_OUT,                						/**< OUT packet*/
  XMC_USBD_EP_EVENT_IN                  						/**< IN packet*/
} XMC_USBD_EP_EVENT_t;

/**
* Defines the options for the USB data transfer modes.
* Use type ::XMC_USBD_TRANSFER_MODE_t for this enum.
*/
typedef enum XMC_USBD_TRANSFER_MODE {
  XMC_USBD_USE_DMA,  											/**< Transfer by DMA*/
  XMC_USBD_USE_FIFO  											/**< Transfer by FIFO*/
} XMC_USBD_TRANSFER_MODE_t;

/**
* Defines the options for the maximum number of endpoints used.
* Use type ::XMC_USBD_MAX_NUM_EPS_t for this enum.
*/
typedef enum XMC_USBD_MAX_NUM_EPS {
  XMC_USBD_MAX_NUM_EPS_1      = 1U,								/**< Maximum 1 endpoint used*/
  XMC_USBD_MAX_NUM_EPS_2      = 2U,								/**< Maximum 2 endpoints used*/
  XMC_USBD_MAX_NUM_EPS_3      = 3U,								/**< Maximum 3 endpoints used*/
  XMC_USBD_MAX_NUM_EPS_4      = 4U,								/**< Maximum 4 endpoints used*/
  XMC_USBD_MAX_NUM_EPS_5      = 5U,								/**< Maximum 5 endpoints used*/
  XMC_USBD_MAX_NUM_EPS_6      = 6U,								/**< Maximum 6 endpoints used*/
  XMC_USBD_MAX_NUM_EPS_7      = 7U								/**< Maximum 2 endpoints used*/
} XMC_USBD_MAX_NUM_EPS_t;

/**
* USB device/endpoint event function pointers
*/
typedef void (*XMC_USBD_SignalDeviceEvent_t)   (XMC_USBD_EVENT_t event);/**< Pointer to USB device event call back.
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 Uses type ::XMC_USBD_EVENT_t as the argument of callback.*/
typedef void (*XMC_USBD_SignalEndpointEvent_t) (uint8_t ep_addr, XMC_USBD_EP_EVENT_t ep_event);/**< Pointer to USB endpoint event call back.
																								Uses type ::XMC_USBD_EP_EVENT_t and EP address as the argument of callback.*/

/**********************************************************************************************************************
 * DATA STRUCTURES
 *********************************************************************************************************************/

/**
 * Describes the USB Device Driver Capabilities.
 */
typedef struct XMC_USBD_CAPABILITIES {
  uint32_t event_power_on      :  1;    						/**< Signal Power On event*/
  uint32_t event_power_off     :  1;    						/**< Signal Power Off event*/
  uint32_t event_connect       :  1;    						/**< Signal Connect event*/
  uint32_t event_disconnect    :  1;    						/**< Signal Disconnect event*/
  uint32_t event_reset         :  1;    						/**< Signal Reset event*/
  uint32_t event_high_speed    :  1;    						/**< Signal switch to High-speed event*/
  uint32_t event_suspend       :  1;    						/**< Signal Suspend event*/
  uint32_t event_resume        :  1;    						/**< Signal Resume event*/
  uint32_t event_remote_wakeup :  1;    						/**< Signal Remote Wake up event*/
  uint32_t reserved            : 23;    						/**< Reserved for future use*/
} XMC_USBD_CAPABILITIES_t;

/**
 * Describes the current USB Device State.
 */
typedef struct XMC_USBD_STATE {
  uint32_t powered     : 1;        								/**< USB Device powered flag*/
  uint32_t connected   : 1;        								/**< USB Device connected flag*/
  uint32_t active      : 1;        								/**< USB Device active lag*/
  uint32_t speed       : 2;        								/**< USB Device speed */
} XMC_USBD_STATE_t;

/**
 * Describes a USB endpoint<BR>
 *
 * All information to control an endpoint is stored in this structure.
 * It contains information about the endpoints and the status of the device.
 */
typedef struct {
  union {
    uint32_t address            : 8;     						/**< The endpoint address including the direction */
      struct {
        uint32_t number         : 4;     						/**< The endpoint number.It can be from 0 to 6 */
        uint32_t pading         : 3;     						/**< Padding between number and direction */
        uint32_t direction      : 1;     						/**< The endpoint direction */
	  } address_st;
    } address_u;
  uint32_t type                 : 2;     						/**< The endpoint type */
  uint32_t isConfigured         : 1;     						/**< The flag showing, if the endpoint is configured */
  volatile uint32_t inInUse     : 1;     						/**< Sets if the selected USB IN endpoint is currently in use */
  volatile uint32_t outInUse    : 1;     						/**< Sets if the selected USB OUT endpoint is currently in use */
  uint32_t isStalled            : 1;     						/**< Sets if the selected USB endpoint is stalled. */
  uint32_t txFifoNum            : 4;     						/**< Endpoint transmit Fifo Number */
  uint32_t sendZeroLengthPacket : 1;							/**< If set, a zero length packet will be send at the end of the transfer */
  uint32_t maxPacketSize        : 7;							/**< The maximum size of packet for USB endpoint ( due to FS Speed device only 64 Byte )*/
  uint32_t maxTransferSize      : 19;							/**< The maximum amount of data the core can send at once.*/
  uint8_t *outBuffer;                    						/**< The buffer for operation as OUT endpoint */
  uint32_t outBytesAvailable;            						/**< The number of bytes available in the EP OUT buffer */
  uint32_t outBufferSize;                						/**< The size of the EP OUT buffer */
  uint32_t outOffset;                    						/**< The read offset of the EP OUT buffer */
  uint8_t *inBuffer;                     						/**< The buffer for operation as IN endpoint */
  uint32_t inBufferSize;                 						/**< The size of the EP IN buffer */
  uint8_t *xferBuffer;                   						/**< The buffer of the current transfer */
  uint32_t xferLength;                   						/**< The length of the current transfer */
  uint32_t xferCount;                    						/**< Bytes transfered of the current USB data transfer */
  uint32_t xferTotal;                    						/**< The length of total data in buffer */
} XMC_USBD_EP_t;

/**
 * Describes the XMC USB device<BR>
 *
 * All information to control an XMC USB device is stored in
 * this structure. It contains register, callbacks, information
 * about the endpoints and the status of the device.
 */
typedef struct XMC_USBD_DEVICE {
  XMC_USBD_EP_t ep[8];                                                     	/**< Endpoints of the USB device. It is of type ::XMC_USBD_EP_t */
  dwc_otg_core_global_regs_t *global_register;                             	/**< Global register interface */
  dwc_otg_device_global_regs_t *device_register;                           	/**< Device register interface */
  dwc_otg_dev_in_ep_regs_t *endpoint_in_register[(uint8_t)XMC_USBD_NUM_EPS];/**< IN Endpoint register interface */
  dwc_otg_dev_out_ep_regs_t *endpoint_out_register[(uint8_t)XMC_USBD_NUM_EPS];/**< OUT Endpoint register interface */
  volatile uint32_t *fifo[(uint8_t)XMC_USBD_NUM_TX_FIFOS];                  /**< Transmit fifo interface */
  uint16_t txfifomsk;                                                      	/**< Mask of used TX fifos */
  uint32_t IsConnected       : 1;                                           /**< Sets if device is connected */
  uint32_t IsActive          : 1;                                           /**< Sets if device is currently active */
  uint32_t IsPowered		 : 1;                                           /**< Sets if device is powered by Vbus */
  XMC_USBD_SignalDeviceEvent_t DeviceEvent_cb;                              /**< The USB device event callback. */
  XMC_USBD_SignalEndpointEvent_t EndpointEvent_cb;                          /**< The USB endpoint event callback. */
} XMC_USBD_DEVICE_t;


/**
 * USB device initialization structure
 */
typedef struct XMC_USBD_OBJ
{
  USB0_GLOBAL_TypeDef *const usbd;                   						/**< USB Module Pointer. The USB0 module base address. */
  XMC_USBD_SignalDeviceEvent_t  cb_xmc_device_event; 						/**< USB device event callback. Use ::XMC_USBD_SignalDeviceEvent_t type of function pointer. */
  XMC_USBD_SignalEndpointEvent_t cb_endpoint_event;  						/**< USB endpoint event callback. Use ::XMC_USBD_SignalEndpointEvent_t type of function pointer.*/
  XMC_USBD_MAX_NUM_EPS_t usbd_max_num_eps;           						/**< Maximum number of end points used. The maximum range can be 7.*/
  XMC_USBD_TRANSFER_MODE_t usbd_transfer_mode;       						/**< USB data transfer mode.Use ::XMC_USBD_TRANSFER_MODE_t type to specify the transfer mode. */
} XMC_USBD_t;

/**
 * Defines the access structure of the USB Device Driver.
 */
typedef struct XMC_USBD_DRIVER {


  XMC_USBD_CAPABILITIES_t (*GetCapabilities)    (void);									 /**< Pointer to @ref XMC_USBD_GetCapabilities : Get driver capabilities.*/


  XMC_USBD_STATUS_t       (*Initialize)         (XMC_USBD_t *obj);						/**< Pointer to  @ref XMC_USBD_Init : Initialize USB Device Interface.*/


  XMC_USBD_STATUS_t       (*Uninitialize)       (void);									/**< Pointer to @ref XMC_USBD_Uninitialize : De-initialize USB Device Interface.*/


  XMC_USBD_STATUS_t       (*DeviceConnect)      (void);									/**< Pointer to @ref XMC_USBD_DeviceConnect : Connect USB Device.*/


  XMC_USBD_STATUS_t       (*DeviceDisconnect)   (void);									/**< Pointer to @ref XMC_USBD_DeviceDisconnect : Disconnect USB Device.*/


  XMC_USBD_STATE_t        (*DeviceGetState)     (const XMC_USBD_t *const obj);			/**< Pointer to @ref XMC_USBD_DeviceGetState : Get current USB Device State.*/


  XMC_USBD_STATUS_t       (*DeviceSetAddress)   (uint8_t dev_addr, XMC_USBD_SET_ADDRESS_STAGE_t stage);/**< Pointer to @ref XMC_USBD_DeviceSetAddress : Set USB Device Address.*/


  XMC_USBD_STATUS_t       (*EndpointConfigure)  (uint8_t ep_addr,XMC_USBD_ENDPOINT_TYPE_t ep_type, uint16_t ep_max_packet_size);/**< Pointer to @ref XMC_USBD_EndpointConfigure : Configure USB Endpoint.*/


  XMC_USBD_STATUS_t       (*EndpointUnconfigure)(uint8_t ep_addr);						/**< Pointer to @ref XMC_USBD_EndpointUnconfigure : Unconfigure USB Endpoint.*/


  XMC_USBD_STATUS_t       (*EndpointStall)      (uint8_t ep_addr, bool stall);			/**< Pointer to @ref XMC_USBD_EndpointStall : Set/Clear Stall for USB Endpoint.*/


  XMC_USBD_STATUS_t       (*EndpointReadStart)  (uint8_t ep_addr, uint32_t len);		/**< Pointer to @ref XMC_USBD_EndpointReadStart : Start USB Endpoint Read operation.*/


  int32_t                 (*EndpointRead)       (uint8_t ep_addr, uint8_t *buf, uint32_t len);/**< Pointer to @ref XMC_USBD_EndpointRead : Read data from USB Endpoint.*/


  int32_t                 (*EndpointWrite)      (uint8_t ep_addr, const uint8_t *buf, uint32_t len); /**< Pointer to @ref XMC_USBD_EndpointWrite : Write data to USB Endpoint.*/


  XMC_USBD_STATUS_t       (*EndpointAbort)      (uint8_t ep_addr);						/**< Pointer to @ref XMC_USBD_EndpointAbort : Abort current USB Endpoint transfer.*/


  uint16_t                (*GetFrameNumber)     (void);									/**< Pointer to @ref XMC_USBD_GetFrameNumber : Get current USB Frame Number.*/


  uint32_t    	          (*IsEnumDone)         (void);									/**< Pointer to @ref XMC_USBD_IsEnumDone : Is enumeration done in Host?.*/
} const XMC_USBD_DRIVER_t;

/**
 * Defines the driver interface function table.
 * To access the XMC device controller driver interface use this table of functions.
 **/
extern const XMC_USBD_DRIVER_t Driver_USBD0;


/**
 * Defines the XMC USB device data
 * The instance of ::XMC_USBD_DEVICE_t structure describing the XMC device.
 **/
extern XMC_USBD_DEVICE_t xmc_device;

/**********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/
 
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param	None.
 *
 * @return 	None.
 *
 * \par<b>Description:</b><BR>
 * Enables the USB module in the XMC controller.<BR>
 * It de-asserts the peripheral reset on USB0 module and enables the USB power.
 *
 * \par<b>Note:</b><BR>
 * This API is called inside the XMC_USBD_Init().\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USBD_Disable()\n
 **/
void XMC_USBD_Enable(void);

/**
 * @param	None.
 *
 * @return 	None.
 *
 * \par<b>Description:</b><BR>
 * Disables the USB module in the XMC controller.<BR>
 * It asserts the peripheral reset on USB0 module and disables the USB power.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USBD_Enable()\n
 **/
void XMC_USBD_Disable(void);

/**
 * @param	event The single event that needs to be cleared. Use ::XMC_USBD_EVENT_t as argument.\n
 *
 * @return 	None.
 *
 * \par<b>Description:</b><BR>
 * Clears the selected USBD \a event.<BR>
 * It clears the event by writing to the GINTSTS register.
 *
 * \par<b>Note:</b><BR>
 * This API is called inside the USB interrupt handler to clear the event XMC_USBD_EVENT_t
 * and maintain the device state machine.\n
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_ClearEventOUTEP(),::XMC_USBD_ClearEventINEP()\n
 **/
void XMC_USBD_ClearEvent(XMC_USBD_EVENT_t event);


/**
 * @param	event The single event or multiple events that need to be cleared.
 *
 * @param	ep_num The IN endpoint number on which the events to be cleared.
 *
 * @return 	None.
 *
 * \par<b>Description:</b><BR>
 * Clears the single event or multiple events of the selected IN endpoint.<BR>
 * The multiple events can be selected by the bitwise OR operation of ::XMC_USBD_EVENT_IN_EP_t elements.\n
 * It clears the event by programming DIEPINT register.\n
 *
 * \par<b>Note:</b><BR>
 * This API is called inside the USB IN EP interrupt handler to clear the ::XMC_USBD_EVENT_IN_EP_t event
 * and maintain the device state machine.\n
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_ClearEventOUTEP()\n
 **/
void XMC_USBD_ClearEventINEP(uint32_t event,uint8_t ep_num);


/**
 * @param	event The single event or multiple events that need to be cleared.
 *
 * @param	ep_num The OUT endpoint number on which the events to be cleared.
 *
 * @return 	None.
 *
 * \par<b>Description:</b><BR>
 * Clears the single \a event or multiple events of the selected OUT endpoint.<BR>
 * The multiple events can be selected by the bitwise OR operation of ::XMC_USBD_EVENT_OUT_EP_t elements.
 * It clears the event by writing to DOEPINT register.
 *
 * \par<b>Note:</b><BR>
 * This API is called inside the USB OUT EP interrupt handler to clear the ::XMC_USBD_EVENT_OUT_EP_t event
 * and maintain the device state machine.\n
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_ClearEventINEP()\n
 **/
void XMC_USBD_ClearEventOUTEP(uint32_t event,uint8_t ep_num);

/**
 * @param	event The single event or multiple events that need to be enabled.
 *
 * @return 	None.
 *
 * \par<b>Description:</b><BR>
 * Enables the event or multiple events of the OUT endpoints.<BR>
 * The multiple events can be selected by the bitwise OR operation of ::XMC_USBD_EVENT_OUT_EP_t elements.
 * It enables the event by programming DOEPMSK register.
 *
 * \par<b>Note:</b><BR>
 * This API is called inside the ::XMC_USBD_Init() to enable the OUT EP interrupts.\n
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_EnableEventINEP()\n
 **/
void XMC_USBD_EnableEventOUTEP(uint32_t event);

/**
 * @param	event The single event or multiple events that need to be enabled.
 *
 * @return 	None.
 *
 * \par<b>Description:</b><BR>
 * Enables the \a event or multiple events of the USB IN endpoints.<BR>
 * The multiple events can be selected by the bitwise OR operation of ::XMC_USBD_EVENT_IN_EP_t elements.
 * It enables the event by programming DIEPMSK register.
 *
 * \par<b>Note:</b><BR>
 * This API is called inside the ::XMC_USBD_Init() to enable the IN EP interrupts.\n
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_EnableEventOUTEP()\n
 **/
void XMC_USBD_EnableEventINEP(uint32_t event);

/**
 * @param	None.
 *
 * @return 	::XMC_USBD_CAPABILITIES_t.
 *
 * \par<b>Description:</b><BR>
 * Retrieves the USB device capabilities of type \a XMC_USBD_CAPABILITIES_t<BR>
 * The USB device capabilities supported by the USBD driver, like power on/off, connect/disconnect,
 * reset,suspend/resume,USB speed etc are retrieved.
 *
 * It can be called after initializing the USB device to get the information on the USBD capabilities.
 *
 **/
XMC_USBD_CAPABILITIES_t XMC_USBD_GetCapabilities(void);


/**
 * @param	obj The pointer to the USB device handle ::XMC_USBD_t.
 *
 * @return 	XMC_USBD_STATUS_t The USB device status of type ::XMC_USBD_STATUS_t.
 *
 * \par<b>Description:</b><BR>
 * Initializes the USB device to get ready for connect to USB host.<BR>
 * Enables the USB module,sets the EP buffer sizes,registers the device and EP event call backs.
 * Initializes the global,device and FIFO register base addresses.
 * Configures the global AHB,enables the global interrupt and DMA by programming GAHBCFG register.
 * Configures the USB in to device mode and enables the session request protocol by programming GUSBCFG register.
 * Configures the USB device speed to full speed by programming DCFG register.
 * Disconnects the USB device by programming DCTL register.
 * Enables the USB common and device interrupts by programming GINTMSK register.
 *
 * \par<b>Note:</b><BR>
 * This API makes the USB device ready to connect to host.The user has to explicitly call
 * the ::XMC_USBD_DeviceConnect() after the USB initialization to connect to USB host.\n
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_DeviceConnect()\n
 **/
XMC_USBD_STATUS_t XMC_USBD_Init(XMC_USBD_t *obj);

/**
 * @param	None.
 *
 * @return 	::XMC_USBD_STATUS_t.
 *
 * \par<b>Description:</b><BR>
 * Uninitialises the USB device.<BR>
 * Disconnects the USB device by programming DCTL register and resets the XMC USB device data.
 *
 * \par<b>Note:</b><BR>
 * Once this API is called, USB device will not be accessible from host.\n
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_Init()\n
 **/
XMC_USBD_STATUS_t XMC_USBD_Uninitialize(void);

/**
 * @param	None.
 *
 * @return 	::XMC_USBD_STATUS_t.
 *
 * \par<b>Description:</b><BR>
 * Connects the USB device to host and triggers the USB enumeration.<BR>
 * Connects the USB device to host by programming DCTL register.\n
 * It resets the soft disconnect bit, which activates the speed pull up at d+ line of USB.
 * ::XMC_USBD_Init() should be called before calling this API.
 *
 * \par<b>Note:</b><BR>
 * Once this API is called, USB host starts the enumeration process and the device should
 * handle the descriptor requests.\n
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_Init()\n
 **/
XMC_USBD_STATUS_t XMC_USBD_DeviceConnect(void);

/**
 * @param	None.
 *
 * @return 	::XMC_USBD_STATUS_t.
 *
 * \par<b>Description:</b><BR>
 * Disconnects the USB device from host.<BR>
 * By programming DCTL register, it sets the soft disconnect bit, which deactivates\n
 * the speed pull up at d+ line of USB.
 *
 * \par<b>Note:</b><BR>
 * Once this API is called, USB device will not be accessible from host.\n
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_DeviceConnect()\n
 **/
XMC_USBD_STATUS_t XMC_USBD_DeviceDisconnect(void);

/**
 * @param	obj The pointer to the USB device handle structure \a XMC_USBD_t.
 *
 * @return 	::XMC_USBD_STATE_t.
 *
 * \par<b>Description:</b><BR>
 * Retrieves the current USB device state.<BR>
 * Power,active,speed and connection status data are retrieved.\n
 *
 * \par<b>Note:</b><BR>
 * Before calling this API, USB should be initialized with ::XMC_USBD_Init.\n
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_Init()\n
 **/
XMC_USBD_STATE_t XMC_USBD_DeviceGetState(const XMC_USBD_t *const obj);


/**
 * @param	address The address to be set for the USB device .
 * @param	stage 	The device request stage-setup or status ::XMC_USBD_SET_ADDRESS_STAGE_t.
 *
 * @return 	::XMC_USBD_STATUS_t.
 *
 * \par<b>Description:</b><BR>
 * Sets the USB device address.<BR>
 * The device address is programmed in the DCFG register.<BR>
 *
 * The address should be more than 0; as 0 is the default USB device address at the starting of enumeration.
 * As part of enumeration, host sends the control request to the device to set the USB address; and in turn,\n
 * in the USB device event call back handler, user has to set the address using this API for the set address request.<BR>
 *
 * The stage parameter should be XMC_USBD_SET_ADDRESS_STAGE_SETUP from the enum ::XMC_USBD_SET_ADDRESS_STAGE_t.
 *
 * \par<b>Note:</b><BR>
 * Before calling this API, USB should be initialized with ::XMC_USBD_Init () and connected to
 * USB host using ::XMC_USBD_DeviceConnect() \n
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_Init(), ::XMC_USBD_DeviceConnect()\n
 **/
XMC_USBD_STATUS_t XMC_USBD_DeviceSetAddress(uint8_t address,XMC_USBD_SET_ADDRESS_STAGE_t stage);

/**
 * @param	ep_addr The address of the USB endpoint, which needs to be configured.
 * @param	ep_type The ::XMC_USBD_ENDPOINT_TYPE_t.
 * @param	ep_max_packet_size The maximum packet size of endpoint in USB full speed.
 *
 * @return 	::XMC_USBD_STATUS_t.
 *
 * \par<b>Description:</b><BR>
 * Configures the USB endpoint.<BR>
 * The endpoint is configured by programming the DAINT,DIEPCTL and DOEPCTL registers.<BR>
 *
 * Configures the EP type, FIFO number,maximum packet size, enables endpoint and sets the DATA0 PID.
 * This function also initializes the internal buffer handling for the specified endpoint,
 * but does not start any transfers.<BR>
 *
 * As part of enumeration, host sends the control request to the device to set the configuration; and in turn,\n
 * in the USB device event call back handler, user has to set the configuration and configure the endpoints \n
 * required for the device.\n
 *
 * \par<b>Note:</b><BR>
 * This API should only be used as part of enumeration.\n
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_Init(),::XMC_USBD_DeviceConnect(),::XMC_USBD_EndpointUnconfigure()\n
 **/
XMC_USBD_STATUS_t XMC_USBD_EndpointConfigure(uint8_t ep_addr,
                                             XMC_USBD_ENDPOINT_TYPE_t ep_type,
		                                     uint16_t ep_max_packet_size);

/**
 * @param	ep_addr The address of the USB endpoint, which needs to be unconfigured.
 *
 * @return 	::XMC_USBD_STATUS_t.
 *
 * \par<b>Description:</b><BR>
 * Unconfigures the USB endpoint.<BR>
 * The endpoint is unconfigured by programming the DAINT,DIEPCTL and DOEPCTL registers.\n
 * Disables the endpoint, unassign the fifo, deactivate it and only send nacks.\n
 * Waits until the endpoint has finished operation and disables it. All (eventuallly) allocated buffers gets freed.
 * Forces the endpoint to stop immediately, any pending transfers are killed(Can cause device reset).
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_EndpointConfigure()\n
 **/
XMC_USBD_STATUS_t XMC_USBD_EndpointUnconfigure(uint8_t ep_addr);

/**
 * @param	ep_addr The address of the USB endpoint, on which stall needs to be set or cleared.
 * @param	stall 	The boolean variable to decide on set or clear of stall on EP.
 *
 * @return 	::XMC_USBD_STATUS_t.
 *
 * \par<b>Description:</b><BR>
 * Set or Clear stall on the USB endpoint \a ep_addr, based on \a stall parameter.<BR>
 *
 * By programming stall bit in the doepctl and diepctl, it sets or clears the stall on the endpoint.
 * The endpoint can be stalled when a non supported request comes from the USB host.
 * The XMC_USBD_EndpointStall() should be called with \a stall set to 0, in the clear feature standard request
 * in the USB device event call back handler. *
 *
 * \par<b>Note:</b><BR>
 * The host should clear the stall set on the endpoint by sending the clear feature standard
 * request on the non EP0 endpoints. On EP0, the stall will automatically gets cleared on the next control request.\n
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_EndpointAbort()\n
 **/
XMC_USBD_STATUS_t XMC_USBD_EndpointStall(uint8_t ep_addr, bool stall);


/**
 * @param	ep_addr The address of the USB endpoint, from which data need to be read.
 * @param	size 	The number of bytes to be read.
 *
 * @return 	::XMC_USBD_STATUS_t.
 *
 * \par<b>Description:</b><BR>
 * Prepares an endpoint to receive OUT tokens from the USB host.<BR>
 * The selected endpoint gets configured, so that it receives the specified amount of data from the host.
 * As part of streaming of OUT data, after reading the current OUT buffer using ::XMC_USBD_EndpointRead(),\n
 * user can prepare endpoint for the next OUT packet by using ::XMC_USBD_EndpointReadStart().
 *
 * The registers DOEPDMA,DOEPTSIZ and DOEPCTL are programmed to start a new read request.
 *
 * \par<b>Note:</b><BR>
 * For the data received on OUT EP buffer, use ::XMC_USBD_EndpointRead().\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USBD_EndpointRead()\n
 **/
XMC_USBD_STATUS_t XMC_USBD_EndpointReadStart(const uint8_t ep_addr, uint32_t size);


/**
 * @param	ep_addr The address of the USB OUT endpoint, from which data need to be read.
 * @param	buffer 	The pointer to the user buffer,in which data need to be received.
 * @param	length 	The number of bytes to be read from OUT EP.
 *
 * @return 	<BR>
 * 			The actual number of bytes received.
 *
 * \par<b>Description:</b><BR>
 * Read \a length number of bytes from an OUT endpoint \a ep_addr.<BR>
 * If data has been received for this endpoint, it gets copied into the user buffer until its full
 * or no data is left in the driver buffer.
 *
 * \par<b>Note:</b><BR>
 * For preparing the next OUT token, use ::XMC_USBD_EndpointReadStart() after ::XMC_USBD_EndpointRead().\n
 *
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_EndpointReadStart()\n
 **/
int32_t XMC_USBD_EndpointRead(const uint8_t ep_addr,uint8_t * buffer, uint32_t length);


/**
 * @param	ep_addr The address of the USB IN endpoint, on which data should be sent.
 * @param	buffer 	The pointer to the data buffer, to write to the endpoint.
 * @param	length 	The number of bytes to be written to IN EP.
 *
 * @return 	<BR>
 * 			The actual amount of data written to the endpoint buffer.
 *
 * \par<b>Description:</b><BR>
 * Write the \a length bytes of data to an IN endpoint \a ep_addr.<BR>
 * The User data gets copied into the driver buffer or will be send directly based on the buffer concept
 * selected in the ::XMC_USBD_TRANSFER_MODE_t configuration.
 *
 * Then the endpoint is set up to transfer the data to the host.\n
 * DIEPDMA,DIEPTSIZ and DIEPCTL registers are programmed to start the IN transfer.
 *
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_EndpointRead()\n
 **/
int32_t XMC_USBD_EndpointWrite(const uint8_t ep_addr,const uint8_t * buffer,uint32_t length);


/**
 * @param	ep_addr The address of the USB endpoint, on which the data need to be aborted.
 *
 * @return 	::XMC_USBD_STATUS_t
 *
 * \par<b>Description:</b><BR>
 * Abort the transfer on endpoint \a ep_addr.<BR>
 * On any failure with the USB transmission user can reset the endpoint into default state and clear all
 * assigned buffers, to start from a clean point. The endpoint will not be unconfigured or disabled.
 *
 * \par<b>Related APIs:</b><BR>
 * ::XMC_USBD_EndpointUnconfigure()\n
 **/
XMC_USBD_STATUS_t XMC_USBD_EndpointAbort(uint8_t ep_addr);

/**
 * @param	None.
 *
 * @return 	The 16 bit current USB frame number.
 *
 * \par<b>Description:</b><BR>
 * Read the current USB frame number.<BR> *
 * Reads the device status register (DSTS) and returns the SOFFN field.
 *
 **/
uint16_t XMC_USBD_GetFrameNumber(void);

/**
 * @param	None.
 *
 * @return 	Returns 1, if the speed enumeration is done and 0 otherwise.
 *
 * \par<b>Description:</b><BR>
 * Gets the speed enumeration completion status of the USB device.<BR>
 *
 * \par<b>Note:</b><BR>
 * This should not be used for the actual USB enumeration completion status. For the actual USB enumeration status,
 * the application layer should check for the completion of USB standard request 'Set configuration'.\n
 *
 **/
uint32_t XMC_USBD_IsEnumDone(void);


/**
 * @param	obj	The pointer to the USB device handle structure.
 *
 * @return 	None.
 *
 * \par<b>Description:</b><BR>
 * USB device default IRQ handler.<BR>
 * USBD Peripheral LLD provides default implementation of ISR.
 * The user needs to explicitly either use our default implementation or use its own one using the LLD APIs.
 *
 * For example:
 *  XMC_USBD_t *obj;
 *  void USB0_0_IRQHandler(void)
 *   {
 *    XMC_USBD_IRQHandler(obj);
 *   }
 *
 * \par<b>Note:</b><BR>
 * The user should initialize the XMC USB device configuration structure before calling
 * ::XMC_USBD_IRQHandler() in the actual USB0 IRQ handler.
 *
 **/
void XMC_USBD_IRQHandler(const XMC_USBD_t *const obj);

#ifdef __cplusplus
}
#endif

/**
 * MISRA C 2004 Deviations
 *
 * 1. Function like macro- defined- MISRA Advisory Rule 19.7
 * 2. usage of unions - MISRA Required Rule 18.4
 */

/**
 * @}
 */

/**
 * @}
 */
 
#endif /* defined(USB0) */

#endif /* XMC_USBD_H */
