/***************************************************************************//**
 * @file em_usbtypes.h
 * @brief USB protocol stack library, internal type definitions.
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

#ifndef __EM_USBTYPES_H
#define __EM_USBTYPES_H

#include "em_device.h"
#if defined( USB_PRESENT ) && ( USB_COUNT == 1 )
#include "em_usb.h"
#if defined( USB_DEVICE ) || defined( USB_HOST )

#ifdef __cplusplus
extern "C" {
#endif

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/* Limits imposed by the USB peripheral */
#define NP_RX_QUE_DEPTH       8
#define HP_RX_QUE_DEPTH       8
#define MAX_XFER_LEN          524287L         /* 2^19 - 1 bytes             */
#define MAX_PACKETS_PR_XFER   1023            /* 2^10 - 1 packets           */
#if defined( _USB_DIEPTXF6_MASK )
  #define MAX_NUM_TX_FIFOS      6             /* In addition to EP0 Tx FIFO */
  #define MAX_NUM_IN_EPS        6             /* In addition to EP0         */
  #define MAX_NUM_OUT_EPS       6             /* In addition to EP0         */
  #define MAX_DEVICE_FIFO_SIZE_INWORDS 512U
#else
  #define MAX_NUM_TX_FIFOS      3             /* In addition to EP0 Tx FIFO */
  #define MAX_NUM_IN_EPS        3             /* In addition to EP0         */
  #define MAX_NUM_OUT_EPS       3             /* In addition to EP0         */
  #define MAX_DEVICE_FIFO_SIZE_INWORDS 384U
#endif
#define MIN_EP_FIFO_SIZE_INWORDS  16U         /* Unit is words (32bit)      */
#define MIN_EP_FIFO_SIZE_INBYTES  64U         /* Unit is bytes (8bit)       */

/* For MCU's without USB host capability. */
#if !defined( USB_ROUTE_VBUSENPEN )
#define USB_VBUS_SWITCH_NOT_PRESENT
#endif

/* Limit imposed by the USB standard */
#define MAX_USB_EP_NUM      15

#if defined( USB_DEVICE )
  /* Check power saving modes. */
  #ifndef USB_PWRSAVE_MODE
    /* Default powersave-mode is OFF. */
    #define USB_PWRSAVE_MODE  USB_PWRSAVE_MODE_OFF
  #else
    #if ( USB_PWRSAVE_MODE                                               &  \
          ~( USB_PWRSAVE_MODE_ONSUSPEND | USB_PWRSAVE_MODE_ONVBUSOFF |      \
             USB_PWRSAVE_MODE_ENTEREM2                                 )    )
      #error "Illegal USB powersave mode."
    #endif
  #endif /* ifndef USB_PWRSAVE_MODE */

  /* Check power saving low frequency clock selection. */
  #ifndef USB_USBC_32kHz_CLK
    /* Default clock source is LFXO. */
    #define USB_USBC_32kHz_CLK USB_USBC_32kHz_CLK_LFXO
  #else
    #if ( ( USB_USBC_32kHz_CLK != USB_USBC_32kHz_CLK_LFXO  ) &&  \
          ( USB_USBC_32kHz_CLK != USB_USBC_32kHz_CLK_LFRCO )     )
      #error "Illegal USB 32kHz powersave clock selection."
    #endif
  #endif /* ifndef USB_USBC_32kHz_CLK */
#endif /* defined( USB_DEVICE ) */

#if defined( USB_HOST )
  /* Check VBUS overcurrent definitions. */
  #ifndef USB_VBUSOVRCUR_PORT
    #define USB_VBUSOVRCUR_PORT       gpioPortE
    #define USB_VBUSOVRCUR_PIN        2
    #define USB_VBUSOVRCUR_POLARITY   USB_VBUSOVRCUR_POLARITY_LOW
  #endif
#endif

/* Developer mode debugging macro's */
#if defined( DEBUG_USB_INT_LO )
  #define DEBUG_USB_INT_LO_PUTS( s )    USB_PUTS( s )
  #define DEBUG_USB_INT_LO_PUTCHAR( c ) USB_PUTCHAR( c )
#else
  #define DEBUG_USB_INT_LO_PUTS( s )
  #define DEBUG_USB_INT_LO_PUTCHAR( c )
#endif /* defined( DEBUG_USB_INT_LO ) */

#if defined( DEBUG_USB_INT_HI )
  #define DEBUG_USB_INT_HI_PUTS( s )    USB_PUTS( s )
  #define DEBUG_USB_INT_HI_PUTCHAR( c ) USB_PUTCHAR( c )
#else
  #define DEBUG_USB_INT_HI_PUTS( s )
  #define DEBUG_USB_INT_HI_PUTCHAR( c )
#endif /* defined( DEBUG_USB_INT_HI ) */

#if defined( USB_HOST )
  #if defined( NUM_APP_TIMERS )
    #define HOSTPORT_TIMER_INDEX  (NUM_APP_TIMERS)
  #else
    #define HOSTPORT_TIMER_INDEX  (0)
  #endif
  #define HOSTCH_TIMER_INDEX      (HOSTPORT_TIMER_INDEX + 1 )
#endif

/* Macros for selecting a hardware timer. */
#define USB_TIMER0 0
#define USB_TIMER1 1
#define USB_TIMER2 2
#define USB_TIMER3 3

#if defined( USB_HOST )
#define HCS_NAK       0x01
#define HCS_STALL     0x02
#define HCS_XACT      0x04
#define HCS_TGLERR    0x08
#define HCS_BABBLE    0x10
#define HCS_TIMEOUT   0x20
#define HCS_COMPLETED 0x40
#define HCS_RETRY     0x80
#endif

#if defined( USB_DEVICE )
typedef enum
{
  D_EP_IDLE          = 0,
  D_EP_TRANSMITTING  = 1,
  D_EP_RECEIVING     = 2,
  D_EP0_IN_STATUS    = 3,
  D_EP0_OUT_STATUS   = 4
} USBD_EpState_TypeDef;

typedef struct
{
  bool                        in;
  uint8_t                     zlp;
  uint8_t                     num;
  uint8_t                     addr;
  uint8_t                     type;
  uint8_t                     txFifoNum;
  uint8_t                     *buf;
  uint16_t                    packetSize;
  uint16_t                    mask;
  uint32_t                    remaining;
  uint32_t                    xferred;
  uint32_t                    hwXferSize;
  uint32_t                    fifoSize;
  USBD_EpState_TypeDef        state;
  USB_XferCompleteCb_TypeDef  xferCompleteCb;
} USBD_Ep_TypeDef;

typedef struct
{
  USB_Setup_TypeDef                     *setup;
  USB_Setup_TypeDef                     setupPkt[3];
  uint8_t                               configurationValue; /* Must be DWORD aligned */
  bool                                  remoteWakeupEnabled;
  uint8_t                               numberOfStrings;
  uint8_t                               numberOfInterfaces;
  USBD_State_TypeDef                    state;
  USBD_State_TypeDef                    savedState;
  USBD_State_TypeDef                    lastState;
  const USB_DeviceDescriptor_TypeDef    *deviceDescriptor;
  const USB_ConfigurationDescriptor_TypeDef *configDescriptor;
  const void * const                    *stringDescriptors;
  const USBD_Callbacks_TypeDef          *callbacks;
  USBD_Ep_TypeDef                       ep[ NUM_EP_USED + 1 ];
  uint8_t                               inEpAddr2EpIndex[  MAX_USB_EP_NUM + 1 ];
  uint8_t                               outEpAddr2EpIndex[ MAX_USB_EP_NUM + 1 ];
  uint32_t                              ep0MpsCode;
} USBD_Device_TypeDef;
#endif /* defined( USB_DEVICE ) */

#if defined( USB_HOST )
typedef enum
{
  H_PORT_DISCONNECTED         = 0,
  H_PORT_CONNECTED_DEBOUNCING = 1,
  H_PORT_CONNECTED_RESETTING  = 2,
  H_PORT_CONNECTED            = 3,
  H_PORT_OVERCURRENT          = 4
} USBH_PortState_TypeDef;

typedef struct
{
  int   debounceTime;
  int   resetTime;
} USBH_AttachTiming_TypeDef;

typedef struct
{
  uint8_t                 *buf;
  int                     errorCnt;
  uint32_t                remaining;
  uint32_t                xferred;
  uint32_t                hwXferSize;
  uint8_t                 status;
  bool                    idle;
  USBH_Ep_TypeDef         *ep;
} USBH_Hc_TypeDef;
#endif /* defined( USB_HOST ) */

/** @endcond */

#ifdef __cplusplus
}
#endif

#endif /* defined( USB_DEVICE ) || defined( USB_HOST ) */
#endif /* defined( USB_PRESENT ) && ( USB_COUNT == 1 ) */
#endif /* __EM_USBTYPES_H */
