/***************************************************************************//**
 * @file em_usbh.h
 * @brief USB protocol stack library API for EFM32.
 * @version 3.20.14
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#ifndef __EM_USBH_H
#define __EM_USBH_H

#include "em_device.h"
#if defined( USB_PRESENT ) && ( USB_COUNT == 1 )
#include "em_usb.h"
#if defined( USB_HOST )

#ifdef __cplusplus
extern "C" {
#endif

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

extern USBH_Hc_TypeDef                  hcs[];
extern int                              USBH_attachRetryCount;
extern const USBH_AttachTiming_TypeDef  USBH_attachTiming[];
extern USBH_Init_TypeDef                USBH_initData;
extern volatile USBH_PortState_TypeDef  USBH_portStatus;

USB_Status_TypeDef USBH_CtlSendSetup(   USBH_Ep_TypeDef *ep );
USB_Status_TypeDef USBH_CtlSendData(    USBH_Ep_TypeDef *ep, uint16_t length );
USB_Status_TypeDef USBH_CtlReceiveData( USBH_Ep_TypeDef *ep, uint16_t length );

#if defined( USB_RAW_API )
int USBH_CtlRxRaw( uint8_t pid, USBH_Ep_TypeDef *ep, void *data, int byteCount );
int USBH_CtlTxRaw( uint8_t pid, USBH_Ep_TypeDef *ep, void *data, int byteCount );
#endif

void USBHEP_EpHandler(     USBH_Ep_TypeDef *ep, USB_Status_TypeDef result );
void USBHEP_CtrlEpHandler( USBH_Ep_TypeDef *ep, USB_Status_TypeDef result );
void USBHEP_TransferDone(  USBH_Ep_TypeDef *ep, USB_Status_TypeDef result );

__STATIC_INLINE uint16_t USBH_GetFrameNum( void )
{
  return USBHHAL_GetFrameNum();
}

__STATIC_INLINE bool USBH_FrameNumIsEven( void )
{
  return ( USBHHAL_GetFrameNum() & 1 ) == 0;
}

/** @endcond */

#ifdef __cplusplus
}
#endif

#endif /* defined( USB_HOST ) */
#endif /* defined( USB_PRESENT ) && ( USB_COUNT == 1 ) */
#endif /* __EM_USBH_H      */
