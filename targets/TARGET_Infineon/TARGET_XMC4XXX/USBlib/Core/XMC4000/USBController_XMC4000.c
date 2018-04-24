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

#include "../../Common/Common.h"

#define  __INCLUDE_FROM_USB_DRIVER
#define  __INCLUDE_FROM_USB_CONTROLLER_C
#include "../USBController.h"

USB_Device_t device;

uint8_t endpoint0_in_buffer[256];
uint8_t endpoint0_out_buffer[256];

/**
 * \ingroup USBD_Core_Driver
 * \defgroup EP0_Handling Handling of EP0 Control Request
 * \brief
 * @{
 */

/**
 * \brief Handle protocol stall on EP0
 *
 * Stalls EP0 and then restarts a new transfer including setting state to \ref IDLE.
 */
void USBD_HandleEP0_Stall() {
	/* When we stall ep0 as protocol stall, we go back into idle state and start a new read */
	device.Driver->EndpointStall(ENDPOINT_DIR_IN | 0,1);
	device.EP0_State = IDLE;
	device.Driver->EndpointReadStart(0,24);
}

/**
 * \brief Handle device request on endpoint 0
 *
 * By default any USB device has to support a subset of device control request defiened by
 * the USB specification. All request required by the spec are handle within this function.
 * Before a handling of the request is done by this function, it dispatches it to the
 * \ref EVENT_USB_Device_ControlRequest function. There the user has the chance to complete
 * some custom request or override the handling of this function. If the user has handled the
 * request, he has to call \ref Endpoint_ClearSETUP.
 *
 */
void USB_Device_ProcessControlRequest() {
	uint32_t length = 0,ret;
	uint16_t status = 0;
	void *buffer;
	uint8_t Value;
	uint16_t Index;

	Value = USB_ControlRequest.wValue & 0x00FF;

	/* Handling of descriptors */
	EVENT_USB_Device_ControlRequest();
	if (!device.IsSetupRecieved)
		return;

	/* default request handling */
	switch (USB_ControlRequest.bRequest) {
	case REQ_ClearFeature:
		if ((USB_ControlRequest.bmRequestType & 0x3) == REQREC_ENDPOINT) {
			Index = USB_ControlRequest.wIndex & ENDPOINT_EPNUM_MASK;
			if ((USB_DeviceState == DEVICE_STATE_Configured || USB_ControlRequest.wIndex==0)  &&
					device.Endpoints[Index].IsConfigured==1) {
				device.Endpoints[Index].IsHalted = 0;
				device.Driver->EndpointStall(USB_ControlRequest.wIndex,0);
			} else {
				USBD_HandleEP0_Stall();
			}
			break;
		}
		if ((USB_ControlRequest.bmRequestType & 0x3) == REQREC_DEVICE) {
			 device.RemoteWakeUp = 0;
			 break;
		}
		USBD_HandleEP0_Stall();
		break;

	case REQ_GetConfiguration:
		device.Driver->EndpointWrite(0,&device.Configuration,1);
		break;

	case REQ_GetDescriptor:
		length = CALLBACK_USB_GetDescriptor(USB_ControlRequest.wValue,USB_ControlRequest.wIndex,(void*)&buffer);
		if (length==0)
			USBD_HandleEP0_Stall();
		else {
			length = length < USB_ControlRequest.wLength ?
					length : USB_ControlRequest.wLength;
			ret = device.Driver->EndpointWrite(0,buffer,length);
			device.Endpoints[0].InDataLeft = length - ret;
			device.Endpoints[0].InDataBuffer = (uint8_t *)((uint32_t)buffer + ret);
		}
		break;

	case REQ_GetInterface:
		if (USB_DeviceState == DEVICE_STATE_Configured) {
			device.Driver->EndpointWrite(0,&device.InterfaceSettings[USB_ControlRequest.wIndex],1);
			break;
		}
		if (USB_DeviceState == DEVICE_STATE_Addressed) {
			USBD_HandleEP0_Stall();
			break;
		}
		break;

	case REQ_GetStatus:
		if ((USB_ControlRequest.bmRequestType & 0x3) == REQREC_DEVICE) {
			status = device.RemoteWakeUp << 1 | device.SelfPowered;
			device.Driver->EndpointWrite(0,(uint8_t*)&status,2);
			break;
		}
		if ((USB_ControlRequest.bmRequestType & 0x3) == REQREC_INTERFACE && USB_DeviceState == DEVICE_STATE_Configured) {
			status = 0;
			device.Driver->EndpointWrite(0,(uint8_t*)&status,2);
			break;
		}
		/* print endpoint status only when(or):
		 * - Device_Address_state and ep == 0
		 * - Device_configured_state and ep is configured
		 */
		if ((USB_ControlRequest.bmRequestType & 0x3) == REQREC_ENDPOINT) {
			Index = USB_ControlRequest.wIndex & 0xFF & ENDPOINT_EPNUM_MASK;
			if ((USB_DeviceState == DEVICE_STATE_Configured || USB_ControlRequest.wIndex==0) &&
					device.Endpoints[Index].IsConfigured==1) {
				status = device.Endpoints[Index].IsHalted;
				device.Driver->EndpointWrite(0,(uint8_t*)&status,2);
				break;
			}
		}
		/* default stall */
		USBD_HandleEP0_Stall();
		break;

	case REQ_SetAddress:
		if (Value == 0)
			USB_DeviceState = DEVICE_STATE_Default;
		else
			USB_DeviceState = DEVICE_STATE_Addressed;
		device.Driver->DeviceSetAddress(Value,XMC_USBD_SET_ADDRESS_STAGE_SETUP);
		break;

	case REQ_SetConfiguration:
		/* Regardless the state update the configuration to unconfigure endpoints */
		device.Configuration = Value;
		EVENT_USB_Device_ConfigurationChanged();
		/* when config 0 is choosen, we are back in address state */
		if (Value == 0) {
			USB_DeviceState = DEVICE_STATE_Addressed;
			break;
		}
		/* go ahead only with vailid config. (must be set in event) */
		if (device.IsConfigured == 1)
			USB_DeviceState = DEVICE_STATE_Configured;
		else
			USBD_HandleEP0_Stall();
		break;

	case REQ_SetDescriptor:
		/* Set Descriptor not supported, so stall */
		USBD_HandleEP0_Stall();
		break;

	case REQ_SetInterface:
		if (USB_DeviceState == DEVICE_STATE_Configured) {
			/* TODO: Check if interface and altsetting exists and configuration is allowed, else stall */
			device.InterfaceSettings[USB_ControlRequest.wIndex] = USB_ControlRequest.wValue;
			break;
		}
		if (USB_DeviceState == DEVICE_STATE_Addressed) {
			USBD_HandleEP0_Stall();
			break;
		}
		break;

	case REQ_SetFeature:
		/* we do not support test mode */
		if (Value == FEATURE_SEL_TestMode) {
			USBD_HandleEP0_Stall();
			break;
		}
		/* configured state */
		if (USB_DeviceState == DEVICE_STATE_Configured) {
			switch (Value) {
			case FEATURE_SEL_DeviceRemoteWakeup:
				device.RemoteWakeUp = 1;
				break;
			case FEATURE_SEL_EndpointHalt:
				Index = USB_ControlRequest.wIndex & 0xFF & (uint8_t)XMC_USBD_ENDPOINT_NUMBER_MASK;
				if (device.Endpoints[Index].IsConfigured == 0)
					USBD_HandleEP0_Stall();
				else {
					device.Endpoints[Index].IsHalted = 1;
					device.Driver->EndpointStall(USB_ControlRequest.wIndex,1);
				}
				break;
			}
			break;
		}
		/* when addressed, only ep0 can be halted */
		if (USB_DeviceState == DEVICE_STATE_Addressed) {
			if (Value == FEATURE_SEL_EndpointHalt &&
					(USB_ControlRequest.bmRequestType & 0x3) == REQREC_ENDPOINT &&
					(USB_ControlRequest.wIndex & 0x00FF) == 0x0) {
				device.Endpoints[0].IsHalted = 1;
				USBD_HandleEP0_Stall();
				break;
			}
		}
		/* default behaviour is stall */
		USBD_HandleEP0_Stall();
		break;

	case REQ_SynchFrame:
		/* Not yet supported */
		USBD_HandleEP0_Stall();
		break;

	default:
		USBD_HandleEP0_Stall();
	}
	device.IsSetupRecieved = 0;
}

/**
 * \brief Handle complete IN transfer on EP0
 *
 * In \ref IN_DATA state it starts a receive and switches to \ref OUT_STATUS state.
 * In \ref IN_STATUS state its starts a new read of setup packets and switches to \ref IDLE.
 */
void USBD_HandleEP0_IN() {
	if (device.EP0_State == IN_DATA) {
		/* Read zero length out data packet */
		device.Driver->EndpointReadStart(0,0);
		device.EP0_State = OUT_STATUS;
	}
	if (device.EP0_State == IN_STATUS) {
		/* Request new setup packet */
		device.Driver->EndpointReadStart(device.Endpoints[0].Address,24);
		device.EP0_State = IDLE;
	}
}


/**
 * \brief Handle complete OUT transfer on EP0
 *
 * Handles the OUT packet based on the state of endpoint 0. Starts a new read for new SETUP packets, when in \ref OUT_STATUS.
 * When endpoint 0 is in \ref OUT_DATA state, it handles the received data and starts a write transaction for \ref IN_STATUS.
 */
void USBD_HandleEP0_OUT() {
	if (device.EP0_State == OUT_DATA) {
		/* Now we have the data for handling the request */
		USB_Device_ProcessControlRequest();
		/* Zero length packet for status stage */
		device.Driver->EndpointWrite(ENDPOINT_DIR_MASK & 0,0,0);
		device.EP0_State = IN_STATUS;
	}
	if (device.EP0_State == OUT_STATUS) {
		/* Request new setup packet */
		device.Driver->EndpointReadStart(device.Endpoints[0].Address,24);
		device.EP0_State = IDLE;
	}
}

/**
 * \brief Handle SETUP packet on EP0
 *
 * Handles the setup package an switches to correct state. If data is send from host to device it switches into \ref OUT_DATA state.
 * When the hosts sends all data within the setup package and \ref wLength equals zero, starts processing the request and sends a
 * in status reponse including the switch to \ref IN_STATUS.
 * When the host expects data from the device, the function processes the control request and switches to \ref IN_DATA state.
 */
void USBD_HandleEP0_SETUP() {
	/* read setup package from ep0 */
	int32_t ret_val = device.Driver->EndpointRead(0,(void*)&USB_ControlRequest,8);
#if NO_COPY_DRIVER
	MEMCPY(&USB_ControlRequest,device.Endpoints[0].OutBuffer,sizeof(USB_ControlRequest));
#endif
	device.IsSetupRecieved = true;
	if (ret_val != 8 )
		return;

	/* preprocess */
	/* if length is zero we have only a in_status phase */
	if (USB_ControlRequest.wLength==0) {
		device.EP0_State = IN_STATUS;
		USB_Device_ProcessControlRequest();
		device.Driver->EndpointWrite(0,0,0);
	} else {
		if (USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_DIRECTION) {
			device.EP0_State = IN_DATA;
			USB_Device_ProcessControlRequest();
		}
		else {
			device.EP0_State = OUT_DATA;
			/* Do not process request here, first read data */
			device.Driver->EndpointReadStart(0,USB_ControlRequest.wLength);
		}
	}
}

/**
 * @}
 */

/**
 * \brief Device event handler for XMC Driver
 *
 * The device can have several events, where it notifies the application about.
 *
 * \note Not all events are available on all chip series. (Power Events are only supported on XMC4500)
 * \see USB_USBD_EVENT
 */
void USBD_SignalDeviceEventHandler(XMC_USBD_EVENT_t event) {
	int i;
	switch (event) {
	case XMC_USBD_EVENT_RESET:
		USB_DeviceState = DEVICE_STATE_Default;
		device.EP0_State = IDLE;
		device.RemoteWakeUp = 0;
		/* Reset endpoints and configuration */
		for (i=0;i<(uint8_t)XMC_USBD_NUM_EPS; i++) {
			device.Endpoints[i].InInUse = 0;
			device.Endpoints[i].OutInUse = 0;
			device.Endpoints[i].IsHalted = 0;
			if (i!=0 && device.Endpoints[i].IsConfigured) {
				device.Driver->EndpointUnconfigure(device.Endpoints[i].Address);
				device.Endpoints[i].IsConfigured = 0;
				device.Endpoints[i].IsEnabled = 0;
			}
		}
		device.Configuration = 0;
		for (i=0;i<NUM_INTERFACES;i++) {
			device.InterfaceSettings[i] = 0;
		}
		device.Driver->EndpointReadStart(device.Endpoints[0].Address,24);
		break;
	case XMC_USBD_EVENT_SOF:
		EVENT_USB_Device_StartOfFrame();
		break;
	case XMC_USBD_EVENT_CONNECT:
		EVENT_USB_Device_Connect();
		break;
	case XMC_USBD_EVENT_DISCONNECT:
		USB_DeviceState = DEVICE_STATE_Powered;
		EVENT_USB_Device_Disconnect();
		break;
	case XMC_USBD_EVENT_POWER_OFF:
		 USB_DeviceState = DEVICE_STATE_Unattached;
		 device.Driver->EndpointUnconfigure(0);
		 break;
    case XMC_USBD_EVENT_POWER_ON:
		 USB_DeviceState = DEVICE_STATE_Powered;
		 memset(&device,0x0,sizeof(USB_Device_t));
		 device.Driver = &Driver_USBD0;
		 device.EP0_State = IDLE;
		 device.Endpoints[0].InBuffer = endpoint0_in_buffer;
		 device.Endpoints[0].InBufferLength = 256;
		 device.Endpoints[0].OutBuffer = endpoint0_out_buffer;
		 device.Endpoints[0].OutBufferLength = 256;
		 device.Endpoints[0].Direction = 0;
		 device.Endpoints[0].IsConfigured = 1;
		 device.Endpoints[0].IsEnabled = 1;
		 device.Endpoints[0].MaxPacketSize = 64;

		  /* then configure endpoint 0 */
		 device.Driver->EndpointConfigure(0,XMC_USBD_ENDPOINT_TYPE_CONTROL,
					   (uint8_t)XMC_USBD_MAX_PACKET_SIZE);

		break;
	case XMC_USBD_EVENT_REMOTE_WAKEUP:
		break;
	case XMC_USBD_EVENT_RESUME:
		USB_DeviceState = device.PreSuspendDeviceState;
		EVENT_USB_Device_WakeUp();
		break;
	case XMC_USBD_EVENT_SUSPEND:
		device.PreSuspendDeviceState = USB_DeviceState;
		USB_DeviceState = DEVICE_STATE_Suspended;
		EVENT_USB_Device_Suspend();
		break;
	default:
		return;
	}
}

/**
 * \brief Endpoint event handler for the XMC driver
 *
 * If the driver detects an event (See \ref USB_USBD_EP_EVENT) for a specified endpoint it
 * calls this function. Based on the event some further action is taken, e.g. process control
 * request or update transfer information and read data from the driver into the core buffer.
 *
 * \param[in] ep_addr Endpoint address
 * \param[in] ep_event Endpoint event type
 *
 */
void USBD_SignalEndpointEvent_Handler (uint8_t ep_addr, XMC_USBD_EP_EVENT_t ep_event) {
	USBD_Endpoint_t *ep =  &device.Endpoints[ep_addr & ENDPOINT_EPNUM_MASK];
	uint8_t temp_num,temp_dir;
	int32_t data_count;
	/* store CurrentEndpoint and direction for restore after handling */
	temp_num = device.CurrentEndpoint;
	temp_dir = device.CurrentDirection;
	/* select the given endpoint */
	device.CurrentEndpoint = ep_addr & ENDPOINT_EPNUM_MASK;
	device.CurrentDirection = ep_addr & ENDPOINT_DIR_MASK;
	/* choose what to do based on the event */
	switch (ep_event) {
	case XMC_USBD_EP_EVENT_SETUP:
		ep->OutInUse = 0;
		switch(device.CurrentEndpoint) {
		case 0:
			USBD_HandleEP0_SETUP();
			break;
		default:
			break;
		}
		break;
	case XMC_USBD_EP_EVENT_OUT:
		ep->IsOutRecieved = 1;
		if (ep->OutBytesAvailable == 0) {
			ep->OutOffset = 0; /* clear offset, new data is there */
			ep->OutBytesAvailable = device.Driver->EndpointRead(ep->Address,ep->OutBuffer,ep->OutBufferLength);
		}
		ep->OutInUse = 0;
		switch(device.CurrentEndpoint) {
		case 0:
			USBD_HandleEP0_OUT();
			break;
		default:
			break;
		}
		break;
	case XMC_USBD_EP_EVENT_IN:
		/* loop write transfers */
		if (ep->InDataLeft> 0) {
			data_count = device.Driver->EndpointWrite(ep->Address,ep->InDataBuffer,ep->InDataLeft);
			ep->InDataLeft -= data_count;
			ep->InDataBuffer+= data_count;
			return;
		} else if (ep->Number == 0 && ep->InBytesAvailable > 0 && ep->InBytesAvailable!=USB_ControlRequest.wLength && ep->InBytesAvailable % ep->MaxPacketSize == 0) {
			/* if the amount of data for endpoint 0 is exact the requested amount, then no zlp has to be send */
			device.Driver->EndpointWrite(ep->Address,0,0);
		}
		ep->InBytesAvailable = 0;
		ep->InInUse = 0;
		switch(device.CurrentEndpoint) {
		case 0:
			USBD_HandleEP0_IN();
			break;
		default:
			break;
		}
		break;
	}
	device.CurrentEndpoint = temp_num;
	device.CurrentDirection = temp_dir;
}

/**
 * \brief Initialize driver core and driver
 *
 * Intializes the USB driver core data structures and sets it into default state. Afterwards
 * it initializes the USB device controller driver and prepare it for connection via \ref USBD_Connect.
 */
int USBD_Initialize(XMC_USBD_t *usb_init) {
	int32_t status = 0;
	memset(&device,0x0,sizeof(USB_Device_t));
	USB_DeviceState = DEVICE_STATE_Unattached;
	device.Driver = &Driver_USBD0;
	device.EP0_State = IDLE;
	device.Endpoints[0].InBuffer = endpoint0_in_buffer;
	device.Endpoints[0].InBufferLength = 256;
	device.Endpoints[0].OutBuffer = endpoint0_out_buffer;
	device.Endpoints[0].OutBufferLength = 256;
	device.Endpoints[0].Direction = 0;
	device.Endpoints[0].IsConfigured = 1;
	device.Endpoints[0].IsEnabled = 1;
	device.Endpoints[0].MaxPacketSize = 64;

	/* First initalize the device */
	status = device.Driver->Initialize(usb_init);
	if (status != XMC_USBD_STATUS_OK)
		return -1;

	/* then configure endpoint 0 */
	device.Driver->EndpointConfigure(0,XMC_USBD_ENDPOINT_TYPE_CONTROL,
			(uint8_t)XMC_USBD_MAX_PACKET_SIZE);
	if (status != XMC_USBD_STATUS_OK) {
		device.Driver->Uninitialize();
		return -1;
	}

	return 0;
}

/**
 * \brief Is the enumeration finished?
 *
 * Tell the USB device controller driver if the enumeration interrupt have been reached
 */
int USB_IsEnumDone(void){
	if (device.Driver->IsEnumDone())
		return 1;
	else
		return 0;
 }


/**
 * \brief Set the buffer for an endpoint
 *
 * The user has to ensure the an endpoint has a valid buffer set
 * in order to ensure proper work.
 *
 * \param[in] addr Endpoint address
 * \param[in] buf Buffer pointer
 * \param[in] len Buffer length
 */
void USBD_SetEndpointBuffer(uint8_t addr,uint8_t *buf,uint16_t len) {
	uint8_t number  = addr & ENDPOINT_EPNUM_MASK;
	if (addr & ENDPOINT_DIR_MASK) {
		device.Endpoints[number].InBuffer = buf;
		device.Endpoints[number].InBufferLength = len;
	} else {
		device.Endpoints[number].OutBuffer = buf;
		device.Endpoints[number].OutBufferLength = len;
	}
}

