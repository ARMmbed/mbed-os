/*
             LUFA Library
     Copyright (C) Dean Camera, 2014.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2014  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *  \brief USB Controller definitions for the Infineon XMC4000 microcontrollers.
 *  \copydetails Group_USBManagement_XMC4000
 *
 *  \note This file should not be included directly. It is automatically included as needed by the USB driver
 *        dispatch header located in USB/USB.h.
 */

/** \ingroup Group_USBManagement
 *  \defgroup Group_USBManagement_XMC4000 USB Interface Management (XMC4000)
 *  \brief USB Controller definitions for the Infineon XMC4000 microcontrollers.
 *
 *  Functions, macros, variables, enums and types related to the setup and management of the USB interface.
 *
 *  @{
 */

#ifndef __USBCONTROLLER_XMC4000_H__
#define __USBCONTROLLER_XMC4000_H__
        
    /* Includes: */
    #include <xmc_usbd.h>
		#include "../../Common/Common.h"
		#include "../USBMode.h"
		#include "../Events.h"
		#include "../USBTask.h"
		#include "../USBInterrupt.h"

	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks and Defines: */
		#if !defined(__INCLUDE_FROM_USB_DRIVER)
			#error Do not include this file directly. Include USB/USB.h instead.
		#endif

	/* Public Interface - May be used in end-application: */
		/* Macros: */
            #define NO_COPY_DRIVER 0

            #define NUM_INTERFACES 1
        
		/* Macros: */        
			#if (!defined(MAX_ENDPOINT_INDEX) && !defined(CONTROL_ONLY_DEVICE)) || defined(__DOXYGEN__)
				/** Total number of endpoints (including the default control endpoint at address 0) which may
				 *  be used in the device. 
				 */
				#define ENDPOINT_TOTAL_ENDPOINTS            7
			#else
				#if defined(CONTROL_ONLY_DEVICE)
					#define ENDPOINT_TOTAL_ENDPOINTS        1
				#else
					#define ENDPOINT_TOTAL_ENDPOINTS        (MAX_ENDPOINT_INDEX + 1)
				#endif
			#endif

            /**
             * \brief State machine of Endpoint 0
             */
            typedef enum EP0_State 
            {
                DISCONNECT, /**< Device is disconnected */
                IDLE, /**< Endpoint 0 is in IDLE state */
                IN_DATA, /**< Endpoint 0 is sending data */
                IN_STATUS, /**< Endpoint 0 sends a acknowledge to the host */
                OUT_DATA, /**< Endpoint 0 is reading data from the host */
                OUT_STATUS /**< Endpoint 0 waits for an acknowledge from the host */
            } EP0_State_t;

						/* Anonymous structure/union guard start */
						#if defined (__CC_ARM)
							#pragma push
							#pragma anon_unions
						#elif defined (__TASKING__)
							#pragma warning 586
						#endif
						
            /**
             * \brief USBD Endpoint
             *
             * Type define for a USBD Enpoint. This structure represents an endpoint. It contains all necessary data to:
             * - control the state of the endpoint
             * - manage the data buffer
             *
             */
            typedef struct USBD_Endpoint 
            {
                /**
                 * \name Endpoint Parameters
                 */
                /*@{*/
                union {
                    uint32_t Address:8; /**< Full device address (including direction)  */
                    struct {
                        uint32_t Number:4; /**< Endpoint number */
                        uint32_t :3;
                        uint32_t Direction:1; /**< Endpoint direction */
                    };

                };

                /* Endpoint flags */
                uint32_t IsConfigured:1; /**< Flag showing that the endpoint is configured */
                uint32_t IsEnabled:1; /**< Flag showing that the endpoint is enabled */
                uint32_t IsHalted:1; /**< Flag showing that the endpoint is halted */
                volatile uint32_t OutInUse:1; /**< Flag showing that the endpoint is in use as out */
                volatile uint32_t InInUse:1; /**< Flag showing that the endpoint is in use as in */
                volatile uint32_t IsOutRecieved:1; /**< Flag showing that an out packet has been received for that endpoint */
                uint32_t MaxPacketSize:7; /**< Maximum packet size for the endpoint */
                /*@}*/

                /**
                 * \name Out endpoint buffer management
                 */
                /*@{*/
                uint32_t OutBytesAvailable; /**< Amount of data in the out buffer */
                uint32_t OutOffset; /**< Amount of data that have been read from the buffer */
                uint8_t *OutBuffer; /**< Address of the out buffer */
                uint32_t OutBufferLength; /**< Length of the out buffer */
                /*@}*/

                /**
                 * \name In endpoint buffer management
                 */
                /*@{*/
                uint32_t InBytesAvailable; /**< Amount of data written into the buffer */
                uint8_t *InBuffer; /**< Address of in Buffer */
                uint32_t InBufferLength; /**< Length of the in buffer */
                uint32_t InDataLeft; /**< Data left in the in buffer to send */
                uint8_t* InDataBuffer; /**< Current start address of the in buffer */
                /*@}*/
            } USBD_Endpoint_t;

            /** \brief USBD Device
             *
             * Type define for an USB Device. This structure contains the all data of the device for the USB Device Core Driver.
             * Especially the data of the endpoints are stored inside.
             */
            typedef struct USBD_Device 
            {            
                XMC_USBD_DRIVER_t *Driver; /**< USB device controller driver structure */
                USBD_Endpoint_t Endpoints[ENDPOINT_TOTAL_ENDPOINTS]; /**< Endpoint structures describing each endpoint of the device */

                uint8_t InterfaceSettings[NUM_INTERFACES]; /**< Array of the currently selected AlternativeSetting of an interface */
                uint8_t Configuration; /**< Current USB device configuration */
                uint8_t PreSuspendDeviceState; /**< Device State bevor suspend is recieved to restore the old state, when resume is received */
                EP0_State_t EP0_State; /**< Endpoint 0 state */
                uint8_t CurrentEndpoint; /**< Endpoint selected for the LUFA stack */
                uint8_t CurrentDirection; /**< Direction selected for the LUFA stack */
                uint8_t IsConfigured:1; /**< Flag showing if the device was successfully configured */
                volatile uint8_t IsSetupRecieved:1; /**< Flag showing if a setup packet is received */
                uint8_t RemoteWakeUp:1; /**< Flag for feature Remote WakeUP signaling */
                uint8_t SelfPowered:1; /**< Flag for feature Self Powered */
            } USB_Device_t;

						/* Anonymous structure/union guard end */
						#if defined (__CC_ARM)
							#pragma pop
						#elif defined (__TASKING__)
							#pragma warning restore
						#endif
        
    	/* Global Variables: */
            extern USB_Device_t device;

		/* Inline Functions: */
			/** Detaches the device from the USB bus. This has the effect of removing the device from any
			 *  attached host, ceasing USB communications. If no host is present, this prevents any host from
			 *  enumerating the device once attached until \ref USB_Attach() is called.
			 */
			static inline void USB_Detach(void) ATTR_ALWAYS_INLINE;
			static inline void USB_Detach(void)
			{
				device.Driver->DeviceDisconnect();
			}

			/** Attaches the device to the USB bus. This announces the device's presence to any attached
			 *  USB host, starting the enumeration process. If no host is present, attaching the device
			 *  will allow for enumeration once a host is connected to the device.
			 *
			 *  This is inexplicably also required for proper operation while in host mode, to enable the
			 *  attachment of a device to the host. This is despite the bit being located in the device-mode
			 *  register and despite the datasheet making no mention of its requirement in host mode.
			 */
			static inline void USB_Attach(void) ATTR_ALWAYS_INLINE;
			static inline void USB_Attach(void)
			{
				device.Driver->DeviceConnect();
			}

		/* Function Prototypes: */
            /**
             * \brief Initialize the USB device core driver
             *
             * In preparation for using the USB device this function initializes the device core driver and the device controller driver.
             * It executes all necessary tasks to start working as USB device and handle request from the host.
             *
             * \note To start working you need to call \ref USBD_Connect to connect to the host.
             */
            int USBD_Initialize(XMC_USBD_t *usb_init);
        

	/* Private Interface - For use in library only: */
	#if !defined(__DOXYGEN__)
        /**
         * \brief Device Event Handler for the device controller driver
         *
         * Handles device events send by the device controller driver. See \ref USBD_SignalDeviceEvent
         */
         void USBD_SignalDeviceEventHandler(XMC_USBD_EVENT_t event);

        /**
         * \brief Endpoint Event Handler for the device controller driver
         *
         * Handles endpoint events send by the device controller driver. See \ref USBD_SignalEndpointEvent_Handler
         */
         void USBD_SignalEndpointEvent_Handler (uint8_t ep_addr, XMC_USBD_EP_EVENT_t ep_event);
         
        /**
         * \brief Is the enumeration done?
         *
         * Returns 1 if the enumeration is done.
         */
        int USB_IsEnumDone(void);

        /** \brief Configure an USBD Endpoint
         *
         * Convenience function to configure an endpoint. It sets the address, the transfer buffer and its size
         * and the maximum packet size.
         * \param[in] Address Endpoint address
         * \param[in] Buffer Endpoint buffer
         * \param[in] Length Buffer size
         */
        void USBD_SetEndpointBuffer(uint8_t Address,uint8_t *Buffer,uint16_t Length);
         
	#endif

    	/* Includes: */
    		#if defined(USB_CAN_BE_DEVICE) || defined(__DOXYGEN__)
    			#include "../Device.h"
    			#include "../Endpoint.h"
    			#include "../DeviceStandardReq.h"
    			#include "../EndpointStream.h"
    		#endif

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif

/** @} */

