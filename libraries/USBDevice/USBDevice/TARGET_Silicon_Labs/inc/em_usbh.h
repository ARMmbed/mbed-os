/***************************************************************************//**
 * @file em_usbh.h
 * @brief USB protocol stack library API for EFM32.
 * @version 3.20.14
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
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
