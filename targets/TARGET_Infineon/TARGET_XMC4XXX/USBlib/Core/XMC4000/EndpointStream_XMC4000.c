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
 * @brief Source file for LUFA Endpoint Streaming functions
 */

#include "../../Common/Common.h"

#define  __INCLUDE_FROM_USB_DRIVER
#include "../USBMode.h"

/*Flag to indicate the zlp to be sent or not*/
volatile uint8_t zlp_flag = 0;

#if defined(USB_CAN_BE_DEVICE)

#include "EndpointStream_XMC4000.h"

uint8_t Endpoint_Write_Stream_LE (const void *const Buffer, uint16_t Length, uint16_t *const BytesProcessed)  {
	USBD_Endpoint_t* ep = &device.Endpoints[device.CurrentEndpoint];
	uint16_t Bytes = 0;
	uint16_t BytesTransfered = 0;
	uint8_t ErrorCode;
	uint16_t prev_length = 0;
	if (BytesProcessed!=NULL) {
		Length -= *BytesProcessed;
		BytesTransfered = *BytesProcessed;
	}

	while (Length) {
		if (ep->InInUse)
			continue;
		if (Endpoint_IsReadWriteAllowed()) {
			Bytes = ep->InBufferLength - ep->InBytesAvailable  > Length ? Length : ep->InBufferLength - ep->InBytesAvailable;
			MEMCPY((void *)((uint32_t)ep->InBuffer + (uint32_t)ep->InBytesAvailable), (void *)((uint32_t)Buffer + (uint32_t)BytesTransfered), Bytes);
			ep->InBytesAvailable += Bytes;
			BytesTransfered += Bytes;
			prev_length = Length;
			Length -= Bytes;
		}
		else {
			Endpoint_ClearIN();
			if (BytesProcessed!=NULL) {
				*BytesProcessed = BytesTransfered;
				return ENDPOINT_RWSTREAM_IncompleteTransfer;
			}

			if ((ErrorCode = Endpoint_WaitUntilReady()) != 0) {
				return ErrorCode;
			}


		}
	}
	
	if((Length == 0) && (prev_length == ep->MaxPacketSize))
	{
	   zlp_flag = true;
	}
	return ENDPOINT_RWSTREAM_NoError;
}

void SwapCopy(void *const Dest, const void *const Src,uint32_t Length) {
	uint32_t i = 0;
	while(i<Length) {
		*(uint8_t*)((uint32_t)Dest + (Length - 1 - i)) = *(uint8_t*)((uint32_t)Src + i);
		i++;
	}
}
uint8_t	Endpoint_Write_Stream_BE (const void *const Buffer, uint16_t Length, uint16_t *const BytesProcessed) {
	USBD_Endpoint_t* ep = &device.Endpoints[device.CurrentEndpoint];
	uint16_t Bytes = 0;
	uint16_t BytesTransfered = 0;
	uint8_t ErrorCode;

	if (BytesProcessed!=NULL) {
		Length -= *BytesProcessed;
		BytesTransfered = *BytesProcessed;
	}

	while (Length) {
		if (ep->InInUse)
			continue;
		if (Endpoint_IsReadWriteAllowed()) {
			Bytes = ep->InBufferLength - ep->InBytesAvailable  > Length ? Length : ep->InBufferLength - ep->InBytesAvailable;
			SwapCopy((void *)((uint32_t)ep->InBuffer + (uint32_t)ep->InBytesAvailable),(void *)((uint32_t)Buffer + (uint32_t)BytesTransfered), Bytes);
			ep->InBytesAvailable += Bytes;
			BytesTransfered += Bytes;
			Length -= Bytes;
		}
		else {
			Endpoint_ClearIN();
			if (BytesProcessed!=NULL) {
				*BytesProcessed = BytesTransfered;
				return ENDPOINT_RWSTREAM_IncompleteTransfer;
			}

			if ((ErrorCode = Endpoint_WaitUntilReady()) != 0) {
				return ErrorCode;
			}


		}
	}
	return ENDPOINT_RWSTREAM_NoError;
}

uint8_t	Endpoint_Read_Stream_LE (void *const Buffer, uint16_t Length, uint16_t *const BytesProcessed) {
	USBD_Endpoint_t* ep = &device.Endpoints[device.CurrentEndpoint];
	uint16_t Bytes = 0;
	uint16_t BytesTransfered = 0;
	uint8_t ErrorCode;

	if (BytesProcessed!=NULL) {
		Length -= *BytesProcessed;
		BytesTransfered = *BytesProcessed;
	}

	while (Length) {
		if (ep->OutInUse)
			continue;
		if (Endpoint_IsReadWriteAllowed()) {
			Bytes = ep->OutBytesAvailable  > Length ? Length : ep->OutBytesAvailable;
			MEMCPY((void *)((uint32_t)Buffer + (uint32_t)BytesTransfered), (void *)((uint32_t)ep->OutBuffer + (uint32_t)ep->OutOffset), Bytes);
			ep->OutBytesAvailable -= Bytes;
			ep->OutOffset += Bytes;
			BytesTransfered += Bytes;
			Length -= Bytes;
		}
		else {
			Endpoint_ClearOUT();
			if (BytesProcessed!=NULL) {
				*BytesProcessed = BytesTransfered;
				return ENDPOINT_RWSTREAM_IncompleteTransfer;
			}

			if ((ErrorCode = Endpoint_WaitUntilReady()) != 0) {
				return ErrorCode;
			}

		}
	}
	return ENDPOINT_RWSTREAM_NoError;
}

uint8_t	Endpoint_Read_Stream_BE (void *const Buffer, uint16_t Length, uint16_t *const BytesProcessed) {
	USBD_Endpoint_t* ep = &device.Endpoints[device.CurrentEndpoint];
	uint16_t Bytes = 0;
	uint16_t BytesTransfered = 0;
	uint8_t ErrorCode;

	if (BytesProcessed!=NULL) {
		Length -= *BytesProcessed;
		BytesTransfered = *BytesProcessed;
	}

	while (Length) {
		if (ep->InInUse)
			continue;
		if (Endpoint_IsReadWriteAllowed()) {
			Bytes = ep->OutBytesAvailable  > Length ? Length : ep->OutBytesAvailable;
			SwapCopy((void *)((uint32_t)Buffer + (uint32_t)BytesTransfered), (void *)((uint32_t)ep->OutBuffer + (uint32_t)ep->OutOffset), Bytes);
			ep->OutBytesAvailable -= Bytes;
			ep->OutOffset += Bytes;
			BytesTransfered += Bytes;
			Length -= Bytes;
		}
		else {
			Endpoint_ClearOUT();
			if (BytesProcessed!=NULL) {
				*BytesProcessed = BytesTransfered;
				return ENDPOINT_RWSTREAM_IncompleteTransfer;
			}

			if ((ErrorCode = Endpoint_WaitUntilReady()) != 0) {
				return ErrorCode;
			}


		}
	}
	return ENDPOINT_RWSTREAM_NoError;
}

uint8_t	Endpoint_Write_Control_Stream_LE (const void *const Buffer, uint16_t Length) {
	USBD_Endpoint_t *EndPoint = &device.Endpoints[0];
	uint16_t Bytes;

	while (Length) {
		if (!EndPoint->InInUse) {
			if (EndPoint->InBufferLength > Length) {
				Bytes = Length;
			} else {
				Bytes = EndPoint->InBufferLength;
			}
			MEMCPY(EndPoint->InBuffer,Buffer,Bytes);
			EndPoint->InBytesAvailable += Bytes;
			Length -= Bytes;

			Endpoint_ClearIN();
		}
	}
	return ENDPOINT_RWCSTREAM_NoError;
}

uint8_t	Endpoint_Write_Control_Stream_BE (const void *const Buffer, uint16_t Length) {
	return Endpoint_Write_Control_Stream_LE(Buffer,Length);
}

uint8_t	Endpoint_Read_Control_Stream_LE (void *const Buffer, uint16_t Length) {
	USBD_Endpoint_t *EndPoint = &device.Endpoints[0];
	uint16_t Bytes;

	while (Length) {
		if (EndPoint->IsOutRecieved) {
			Bytes = EndPoint->OutBytesAvailable > Length
					? Length : EndPoint->OutBytesAvailable;
			MEMCPY(Buffer,EndPoint->OutBuffer,Bytes);
			EndPoint->OutBytesAvailable -= Bytes;
			Length -= Bytes;

			Endpoint_ClearOUT();
		}
	}
	return ENDPOINT_RWCSTREAM_NoError;
}

uint8_t	Endpoint_Read_Control_Stream_BE (void *const Buffer, uint16_t Length) {
	return Endpoint_Read_Control_Stream_LE(Buffer,Length);
}

uint8_t Endpoint_Null_Stream(uint16_t Length,
								 uint16_t* const BytesProcessed) {
	USBD_Endpoint_t* ep = &device.Endpoints[device.CurrentEndpoint];
		uint16_t Bytes = 0;
		uint16_t BytesTransfered = 0;
		uint8_t ErrorCode;

		if (BytesProcessed!=NULL) {
			Length -= *BytesProcessed;
			BytesTransfered = *BytesProcessed;
		}

		while (Length) {
			if (ep->InInUse)
				continue;
			if (Endpoint_IsReadWriteAllowed()) {
				Bytes = ep->InBufferLength - ep->InBytesAvailable  > Length ? Length : ep->InBufferLength - ep->InBytesAvailable;
				memset(ep->InBuffer + ep->InBytesAvailable,0x0,Bytes);
				ep->InBytesAvailable += Bytes;
				BytesTransfered += Bytes;
				Length -= Bytes;
			}
			else {
				Endpoint_ClearIN();
				if (BytesProcessed!=NULL) {
					*BytesProcessed = BytesTransfered;
					return ENDPOINT_RWSTREAM_IncompleteTransfer;
				}

				if ((ErrorCode = Endpoint_WaitUntilReady()) != 0) {
					return ErrorCode;
				}


			}
		}
		return ENDPOINT_RWSTREAM_NoError;
}

#endif
