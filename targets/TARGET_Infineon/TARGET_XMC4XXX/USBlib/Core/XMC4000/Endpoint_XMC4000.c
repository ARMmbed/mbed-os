/*******************************************************************************
 Copyright (c) 2013, Infineon Technologies AG                                 **
 All rights reserved.                                                         **
                                                                              **
 Redistribution and use in source and binary forms, with or without           **
 modification,are permitted provided that the following conditions are met:   **
                                                                              **
 *Redistributions of source code must retain the above copyright notice,      **
 this list of conditions and the following disclaimer.                        **
 *Redistributions in binary form must reproduce the above copyright notice,   **
 this list of conditions and the following disclaimer in the documentation    **
 and/or other materials provided with the distribution.                       **
 *Neither the name of the copyright holders nor the names of its contributors **
 may be used to endorse or promote products derived from this software without**
 specific prior written permission.                                           **
                                                                              **
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  **
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    **
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   **
 ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   **
 LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         **
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         **
 SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    **
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      **
 CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       **
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   **
 POSSIBILITY OF SUCH DAMAGE.                                                  **
                                                                              **
 To improve the quality of the software, users are encouraged to share        **
 modifications, enhancements or bug fixes with Infineon Technologies AG       **
 dave@infineon.com).                                                          **
                                                                              **
********************************************************************************/
/**
 * @file
 *
 * @App Version internal release for evaluation, no public release!
 *
 * @brief Source file for LUFA endpoint functions
 */
#include "../../Common/Common.h"
#define  __INCLUDE_FROM_USB_DRIVER
#include "../USBMode.h"

#if defined(USB_CAN_BE_DEVICE)

#include "../Endpoint.h"


#define USB_STREAM_TIMEOUT_MS 100

extern volatile uint8_t zlp_flag;

uint8_t Endpoint_WaitUntilReady(void)
{
	#if (USB_STREAM_TIMEOUT_MS < 0xFF)
	uint8_t  TimeoutMSRem = USB_STREAM_TIMEOUT_MS;
	#else
	uint16_t TimeoutMSRem = USB_STREAM_TIMEOUT_MS;
	#endif

	uint16_t PreviousFrameNumber = USB_Device_GetFrameNumber();

	for (;;)
	{
		if (Endpoint_GetEndpointDirection() == ENDPOINT_DIR_IN)
		{
			if (Endpoint_IsINReady())
			  return ENDPOINT_READYWAIT_NoError;
		}
		else
		{
			if (Endpoint_IsOUTReceived())
			  return ENDPOINT_READYWAIT_NoError;
		}

		uint8_t USB_DeviceState_LCL = USB_DeviceState;

		if (USB_DeviceState_LCL == DEVICE_STATE_Unattached)
		  return ENDPOINT_READYWAIT_DeviceDisconnected;
		else if (USB_DeviceState_LCL == DEVICE_STATE_Suspended)
		  return ENDPOINT_READYWAIT_BusSuspended;
		else if (Endpoint_IsStalled())
		  return ENDPOINT_READYWAIT_EndpointStalled;

		uint16_t CurrentFrameNumber = USB_Device_GetFrameNumber();

		if (CurrentFrameNumber != PreviousFrameNumber)
		{
			PreviousFrameNumber = CurrentFrameNumber;

			if (!(TimeoutMSRem--))
			  return ENDPOINT_READYWAIT_Timeout;
		}
	}
}

void Endpoint_ClearOUT(void) {
	USBD_Endpoint_t *ep = &device.Endpoints[device.CurrentEndpoint];
	/* if we have data left which isn't read yet, we leave this routine to not override it */
	if (ep->IsEnabled == 0)
		return;
	/* First Check whether we have data in the driver */
	ep->OutBytesAvailable = device.Driver->EndpointRead(ep->Address,ep->OutBuffer,ep->OutBufferLength);
	ep->OutOffset = 0;
	/* If we didn't request new data and all data has been read, request new */
	if (!ep->OutInUse && !ep->OutBytesAvailable) {
		ep->OutInUse = true;
		ep->IsOutRecieved = 0;
		device.Driver->EndpointReadStart(ep->Address,
				ep->OutBufferLength);
	}
}

void Endpoint_ClearIN(void)
{
	USBD_Endpoint_t *ep = &device.Endpoints[device.CurrentEndpoint];
	int32_t data_count;
	/* don't clear if in use or not enabled */
	if (ep->InInUse == 1 || ep->IsEnabled == 0)
		return;
	ep->InInUse = true;
	/* store transfer information to loop over, if underlying is smaller */
	ep->InDataBuffer = ep->InBuffer;
	ep->InDataLeft = ep->InBytesAvailable;
	ep->InBytesAvailable = 0;
	/* make next 3 operations atomic. Do not get interrupted.*/
	NVIC_DisableIRQ(USB0_0_IRQn);
	data_count = device.Driver->EndpointWrite(ep->Address,
			ep->InDataBuffer,ep->InDataLeft);
	ep->InDataBuffer += data_count;
	ep->InDataLeft -= data_count;
	NVIC_EnableIRQ(USB0_0_IRQn);
	if((zlp_flag == true) && (ep->Number != 0))
	{
		/*Send a ZLP from here*/
	  while(ep->InInUse)
	  {
		;
	  }
	  ep->InInUse = true;
	  device.Driver->EndpointWrite(ep->Address,
	  			ep->InDataBuffer,0);
	  zlp_flag = false;
	}
}

bool Endpoint_IsReadWriteAllowed(void) {
	USBD_Endpoint_t *EndPoint = &device.Endpoints[device.CurrentEndpoint];
	bool Retval = false;

	if(EndPoint->Direction)
	{
		Retval = (EndPoint->InBytesAvailable < EndPoint->InBufferLength) ? true : false;
	}
	else
	{
		Retval = (EndPoint->OutBytesAvailable > 0) ? true : false;
	}
	return Retval;
}

void Endpoint_Write_8(const uint8_t Data) {
	USBD_Endpoint_t *EndPoint = &device.Endpoints[device.CurrentEndpoint];
	bool Success = false;

	 do
	  {
	    if(EndPoint->InBytesAvailable < EndPoint->InBufferLength)
	    {
	      EndPoint->InBuffer[EndPoint->InBytesAvailable] = Data;
	      EndPoint->InBytesAvailable++;

	      Success = true;
	    }
	  }while(!Success);
}

uint8_t Endpoint_Read_8(void) {
	USBD_Endpoint_t *EndPoint = &device.Endpoints[device.CurrentEndpoint];
	bool Success = false;
	uint8_t data = 0;
	 do
	  {
		if(EndPoint->OutBytesAvailable > 0)
		{
          data = EndPoint->OutBuffer[EndPoint->OutOffset];
          EndPoint->OutOffset++;
          EndPoint->OutBytesAvailable--;
		  Success = true;
		}
	  }while(!Success);
	 return data;
}

void Endpoint_Write_32_LE(const uint32_t Data) {
	USBD_Endpoint_t *EndPoint = &device.Endpoints[device.CurrentEndpoint];
	bool Success = false;

	do {
		if(EndPoint->InBytesAvailable < (EndPoint->InBufferLength - 3)) {
			*(uint32_t*)(EndPoint->InBuffer + EndPoint->InBytesAvailable) = Data;
			EndPoint->InBytesAvailable+=4;

			Success = true;
		}
	}while(!Success);
}

uint32_t Endpoint_Read_32_LE(void) {
	USBD_Endpoint_t *EndPoint = &device.Endpoints[device.CurrentEndpoint];
	bool Success = false;
	uint32_t data = 0;

	do {
		if(EndPoint->OutBytesAvailable > 3) {
			data = *(uint32_t*)(EndPoint->OutBuffer + EndPoint->OutOffset);
			EndPoint->OutOffset+=4;
			EndPoint->OutBytesAvailable-=4;

			Success = true;
		}
	} while(!Success);
	return data;
}

bool Endpoint_ConfigureEndpointTable(const USB_Endpoint_Table_t* const Table,
			                                     const uint8_t Entries) {
	uint8_t i;
	uint8_t Number;
	for (i=0;i<Entries;i++) {
		Number = Table[i].Address & ENDPOINT_EPNUM_MASK;
		/* Configure endpoint in device controller driver */
		if (device.Driver->EndpointConfigure(Table[i].Address, (XMC_USBD_ENDPOINT_TYPE_t)Table[i].Type, Table[i].Size) != XMC_USBD_STATUS_OK)
			return false;
		/* Set device core values */
		device.Endpoints[Number].Address = Table[i].Address;
		device.Endpoints[Number].MaxPacketSize = Table[i].Size;
		device.Endpoints[Number].IsConfigured = 1;
		device.Endpoints[Number].IsEnabled = 1;
		/* Start read for out endpoints */
		if (!(Table[i].Address & ENDPOINT_DIR_MASK))
			device.Driver->EndpointReadStart(Table[i].Address,
					device.Endpoints[Number].OutBufferLength);
	}
	return true;
}

#endif
