/**************************************************************************//**
 * @file em_usbhal.c
 * @brief USB protocol stack library, low level USB peripheral access.
 * @version 3.20.14
 ******************************************************************************
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

#include "em_device.h"
#if defined( USB_PRESENT ) && ( USB_COUNT == 1 )
#include "em_usb.h"
#if defined( USB_DEVICE ) || defined( USB_HOST )

#include "em_usbtypes.h"
#include "em_usbhal.h"
#if defined( USB_DEVICE )
#include "em_usbd.h"
#endif
#if defined( USB_HOST )
#include "em_usbh.h"
#endif
#include "em_cmu.h"
#include "em_gpio.h"

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#define EPABORT_BREAK_LOOP_COUNT 15000              /* Approx. 100 ms */

/* NOTE: The sequence of error message strings must agree with the    */
/*       definition of USB_Status_TypeDef enum.                       */
static const char * const errMsg[] =
{
  [  USB_STATUS_OK                  ] = "No errors",
  [ -USB_STATUS_REQ_ERR             ] = "Setup request error",
  [ -USB_STATUS_EP_BUSY             ] = "Endpoint is busy",
  [ -USB_STATUS_REQ_UNHANDLED       ] = "Setup request not handled",
  [ -USB_STATUS_ILLEGAL             ] = "Illegal operation attempted",
  [ -USB_STATUS_EP_STALLED          ] = "Endpoint is stalled",
  [ -USB_STATUS_EP_ABORTED          ] = "Transfer aborted",
  [ -USB_STATUS_EP_ERROR            ] = "Transfer error",
  [ -USB_STATUS_EP_NAK              ] = "Endpoint NAK",
  [ -USB_STATUS_DEVICE_UNCONFIGURED ] = "Device is not configured",
  [ -USB_STATUS_DEVICE_SUSPENDED    ] = "Device is suspended",
  [ -USB_STATUS_DEVICE_RESET        ] = "Device has been reset",
  [ -USB_STATUS_TIMEOUT             ] = "Transfer timeout",
  [ -USB_STATUS_DEVICE_REMOVED      ] = "Device removed",
  [ -USB_STATUS_HC_BUSY             ] = "Host channel is busy",
  [ -USB_STATUS_DEVICE_MALFUNCTION  ] = "Device malfunction",
  [ -USB_STATUS_PORT_OVERCURRENT    ] = "VBUS overcurrent",
};
/** @endcond */


/***************************************************************************//**
 * @brief
 *   Return an error message string for a given error code.
 *
 * @param[in] error
 *   Error code, see \ref USB_Status_TypeDef.
 *
 * @return
 *   Error message string pointer.
 ******************************************************************************/
char *USB_GetErrorMsgString( int error )
{
  if ( error >= 0 )
    return (char*)errMsg[ 0 ];

  return (char*)errMsg[ -error ];
}


#if defined( USB_USE_PRINTF )
/***************************************************************************//**
 * @brief
 *   Format and print a text string given an error code, prepends an optional user
 *   supplied leader string.
 *
 * @param[in] pre
 *   Optional leader string to prepend to error message string.
 *
 * @param[in] error
 *   Error code, see \ref USB_Status_TypeDef.
 ******************************************************************************/
void USB_PrintErrorMsgString( char *pre, int error )
{
  if ( pre )
  {
    USB_PRINTF( "%s", pre );
  }

  if ( error > USB_STATUS_OK )
  {
    USB_PRINTF( "%d", error );
  }
  else
  {
    USB_PRINTF( "%s", USB_GetErrorMsgString( error ) );
  }
}
#endif /* defined( USB_USE_PRINTF ) */

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#if defined( DEBUG_EFM_USER )
static void PrintI( int i )
{
#if !defined ( USER_PUTCHAR )
  (void)i;
#else
  if ( i >= 10 )
  {
    PrintI( i / 10 );
  }

  DEBUG_USB_API_PUTCHAR( ( i % 10 ) + '0' );
#endif
}

void assertEFM( const char *file, int line )
{
#if !defined ( USER_PUTCHAR )
  (void)file;
#endif

  DEBUG_USB_API_PUTS( "\nASSERT " );
  DEBUG_USB_API_PUTS( file );
  DEBUG_USB_API_PUTCHAR( ' ' );
  PrintI( line );
  for(;;){}
}
#endif /* defined( DEBUG_EFM_USER ) */

#if defined ( USER_PUTCHAR )
void USB_Puts( const char *p )
{
  while( *p )
    USB_PUTCHAR( *p++ );
}
#endif /* defined ( USER_PUTCHAR ) */

void USBHAL_CoreReset( void )
{
  USB->PCGCCTL &= ~USB_PCGCCTL_STOPPCLK;
  USB->PCGCCTL &= ~(USB_PCGCCTL_PWRCLMP | USB_PCGCCTL_RSTPDWNMODULE);

  /* Core Soft Reset */
  USB->GRSTCTL |= USB_GRSTCTL_CSFTRST;
  while ( USB->GRSTCTL & USB_GRSTCTL_CSFTRST ) {}

  USBTIMER_DelayUs( 1 );

  /* Wait for AHB master IDLE state. */
  while ( !( USB->GRSTCTL & USB_GRSTCTL_AHBIDLE ) ) {}
}

#ifdef USB_DEVICE
void USBDHAL_Connect( void )
{
  USB->DCTL &= ~( DCTL_WO_BITMASK | USB_DCTL_SFTDISCON );
}

USB_Status_TypeDef USBDHAL_CoreInit( uint32_t totalRxFifoSize,
                                     uint32_t totalTxFifoSize )
{
  uint8_t i, j;
  uint16_t start, depth;
  USBD_Ep_TypeDef *ep;

#if !defined( USB_VBUS_SWITCH_NOT_PRESENT )
  CMU_ClockEnable( cmuClock_GPIO, true );
  GPIO_PinModeSet( gpioPortF, 5, gpioModePushPull, 0 ); /* Enable VBUSEN pin */
  USB->ROUTE = USB_ROUTE_PHYPEN | USB_ROUTE_VBUSENPEN;  /* Enable PHY pins.  */
#else
  USB->ROUTE = USB_ROUTE_PHYPEN;                        /* Enable PHY pins.  */
#endif

  USBHAL_CoreReset();                                   /* Reset USB core    */

#if defined( USB_GUSBCFG_FORCEHSTMODE )
  /* Force Device Mode */
  USB->GUSBCFG = ( USB->GUSBCFG                                    &
                  ~(GUSBCFG_WO_BITMASK | USB_GUSBCFG_FORCEHSTMODE )  ) |
                 USB_GUSBCFG_FORCEDEVMODE;
#endif

  INT_Enable();
  USBTIMER_DelayMs( 50 );
  INT_Disable();

  /* Set device speed */
  USB->DCFG = ( USB->DCFG & ~_USB_DCFG_DEVSPD_MASK ) | 3; /* Full speed PHY */

  /* Stall on non-zero len status OUT packets (ctrl transfers). */
  USB->DCFG |= USB_DCFG_NZSTSOUTHSHK;

  /* Set periodic frame interval to 80% */
  USB->DCFG &= ~_USB_DCFG_PERFRINT_MASK;

  USB->GAHBCFG = ( USB->GAHBCFG & ~_USB_GAHBCFG_HBSTLEN_MASK ) |
                 USB_GAHBCFG_DMAEN | USB_GAHBCFG_HBSTLEN_INCR;

  /* Ignore frame numbers on ISO transfers. */
  USB->DCTL = ( USB->DCTL & ~DCTL_WO_BITMASK ) | USB_DCTL_IGNRFRMNUM;

  /* Set Rx FIFO size */
  start = EFM32_MAX( totalRxFifoSize, MIN_EP_FIFO_SIZE_INWORDS );
  USB->GRXFSIZ = ( start << _USB_GRXFSIZ_RXFDEP_SHIFT ) &
                 _USB_GRXFSIZ_RXFDEP_MASK;

  /* Set Tx EP0 FIFO size */
  depth = EFM32_MAX( dev->ep[ 0 ].fifoSize, MIN_EP_FIFO_SIZE_INWORDS );
  USB->GNPTXFSIZ = ( ( depth << _USB_GNPTXFSIZ_NPTXFINEPTXF0DEP_SHIFT ) &
                     _USB_GNPTXFSIZ_NPTXFINEPTXF0DEP_MASK                 ) |
                   ( ( start << _USB_GNPTXFSIZ_NPTXFSTADDR_SHIFT ) &
                     _USB_GNPTXFSIZ_NPTXFSTADDR_MASK                      );


  /* Set Tx EP FIFO sizes for all IN ep's */
  for ( j = 1; j <= MAX_NUM_TX_FIFOS; j++ )
  {
    for ( i = 1; i <= MAX_NUM_IN_EPS; i++ )
    {
      ep = USBD_GetEpFromAddr( USB_SETUP_DIR_MASK | i );
      if ( ep )                             /* Is EP in use ? */
      {
        if ( ep->txFifoNum == j )           /* Is it correct FIFO number ? */
        {
          start += depth;
          depth = EFM32_MAX( ep->fifoSize, MIN_EP_FIFO_SIZE_INWORDS );
          USB_DIEPTXFS[ ep->txFifoNum - 1 ] =
                              ( depth << _USB_DIEPTXF1_INEPNTXFDEP_SHIFT   ) |
                              ( start &  _USB_DIEPTXF1_INEPNTXFSTADDR_MASK );
        }
      }
    }
  }

  if ( totalRxFifoSize + totalTxFifoSize > MAX_DEVICE_FIFO_SIZE_INWORDS )
    return USB_STATUS_ILLEGAL;

  if ( start > MAX_DEVICE_FIFO_SIZE_INWORDS )
    return USB_STATUS_ILLEGAL;

  /* Flush the FIFO's */
  USBHAL_FlushTxFifo( 0x10 );      /* All Tx FIFO's */
  USBHAL_FlushRxFifo();            /* The Rx FIFO   */

  /* Disable all device interrupts */
  USB->DIEPMSK  = 0;
  USB->DOEPMSK  = 0;
  USB->DAINTMSK = 0;
  USB->DIEPEMPMSK = 0;

  /* Disable all EP's, clear all EP ints. */
  for ( i = 0; i <= MAX_NUM_IN_EPS; i++ )
  {
    USB_DINEPS[ i ].CTL  = 0;
    USB_DINEPS[ i ].TSIZ = 0;
    USB_DINEPS[ i ].INT  = 0xFFFFFFFF;
  }

  for ( i = 0; i <= MAX_NUM_OUT_EPS; i++ )
  {
    USB_DOUTEPS[ i ].CTL  = 0;
    USB_DOUTEPS[ i ].TSIZ = 0;
    USB_DOUTEPS[ i ].INT  = 0xFFFFFFFF;
  }

#if ( USB_DCTL_SFTDISCON_DEFAULT != 0 )
  USBD_Connect();
#endif

  /* Enable VREGO sense. */
  USB->CTRL |= USB_CTRL_VREGOSEN;
  USB->IFC   = USB_IFC_VREGOSH | USB_IFC_VREGOSL;
  USB->IEN   = USB_IFC_VREGOSH | USB_IFC_VREGOSL;
  /* Force a VREGO interrupt. */
  if ( USB->STATUS & USB_STATUS_VREGOS)
    USB->IFS = USB_IFS_VREGOSH;
  else
    USB->IFS = USB_IFS_VREGOSL;

  return USB_STATUS_OK;
}

USB_Status_TypeDef USBDHAL_ReconfigureFifos( uint32_t totalRxFifoSize,
                                             uint32_t totalTxFifoSize )
{
  uint8_t i, j;
  uint16_t start, depth;
  USBD_Ep_TypeDef *ep;

  /* Set Rx FIFO size */
  start = EFM32_MAX( totalRxFifoSize, MIN_EP_FIFO_SIZE_INWORDS );
  USB->GRXFSIZ = ( start << _USB_GRXFSIZ_RXFDEP_SHIFT ) &
                 _USB_GRXFSIZ_RXFDEP_MASK;

  /* Set Tx EP0 FIFO size */
  depth = EFM32_MAX( dev->ep[ 0 ].fifoSize, MIN_EP_FIFO_SIZE_INWORDS );
  USB->GNPTXFSIZ = ( ( depth << _USB_GNPTXFSIZ_NPTXFINEPTXF0DEP_SHIFT ) &
                     _USB_GNPTXFSIZ_NPTXFINEPTXF0DEP_MASK                 ) |
                   ( ( start << _USB_GNPTXFSIZ_NPTXFSTADDR_SHIFT ) &
                     _USB_GNPTXFSIZ_NPTXFSTADDR_MASK                      );


  /* Set Tx EP FIFO sizes for all IN ep's */
  for ( j = 1; j <= MAX_NUM_TX_FIFOS; j++ )
  {
    for ( i = 1; i <= MAX_NUM_IN_EPS; i++ )
    {
      ep = USBD_GetEpFromAddr( USB_SETUP_DIR_MASK | i );
      if ( ep )                             /* Is EP in use ? */
      {
        if ( ep->txFifoNum == j )           /* Is it correct FIFO number ? */
        {
          start += depth;
          depth = EFM32_MAX( ep->fifoSize, MIN_EP_FIFO_SIZE_INWORDS );
          USB_DIEPTXFS[ ep->txFifoNum - 1 ] =
                              ( depth << _USB_DIEPTXF1_INEPNTXFDEP_SHIFT   ) |
                              ( start &  _USB_DIEPTXF1_INEPNTXFSTADDR_MASK );
        }
      }
    }
  }

  if ( totalRxFifoSize + totalTxFifoSize > MAX_DEVICE_FIFO_SIZE_INWORDS )
    return USB_STATUS_ILLEGAL;

  if ( start > MAX_DEVICE_FIFO_SIZE_INWORDS )
    return USB_STATUS_ILLEGAL;

  /* Flush the FIFO's */
  USBHAL_FlushTxFifo( 0x10 );      /* All Tx FIFO's */
  USBHAL_FlushRxFifo();            /* The Rx FIFO   */

  return USB_STATUS_OK;
}

void USBDHAL_Disconnect( void )
{
  USB->DCTL = ( USB->DCTL & ~DCTL_WO_BITMASK ) | USB_DCTL_SFTDISCON;
}

void USBDHAL_AbortEpIn( USBD_Ep_TypeDef *ep )
{
  /* Clear epdis & inepnakeff INT's */
  USB_DINEPS[ ep->num ].INT |= USB_DIEP_INT_EPDISBLD |
                               USB_DIEP_INT_INEPNAKEFF;

  /* Enable epdis & inepnakeff INT's */
  USB->DIEPMSK |= USB_DIEPMSK_EPDISBLDMSK | USB_DIEPMSK_INEPNAKEFFMSK;
  USB_DINEPS[ ep->num ].CTL = ( USB_DINEPS[ ep->num ].CTL  &
                                ~DEPCTL_WO_BITMASK           ) |
                              USB_DIEP_CTL_SNAK;

  /* Wait for inepnakeff INT */
  while ( !( USBDHAL_GetInEpInts( ep ) & USB_DIEP_INT_INEPNAKEFF ) ) {}
  USB_DINEPS[ ep->num ].INT = USB_DIEP_INT_INEPNAKEFF;
  USB->DIEPMSK &= ~USB_DIEPMSK_INEPNAKEFFMSK;

  DEBUG_USB_INT_LO_PUTCHAR( '.' );

  USBDHAL_SetEPDISNAK( ep );
  /* Wait for epdis INT */
  while ( !( USBDHAL_GetInEpInts( ep ) & USB_DIEP_INT_EPDISBLD ) ) {}
  USB_DINEPS[ ep->num ].INT = USB_DIEP_INT_EPDISBLD;
  USB->DIEPMSK &= ~USB_DIEPMSK_EPDISBLDMSK;
  USBHAL_FlushTxFifo( ep->txFifoNum );

  /* Clear any interrupts generated by the abort sequence. */
  NVIC_ClearPendingIRQ( USB_IRQn );

  DEBUG_USB_INT_LO_PUTCHAR( '.' );
}

void USBDHAL_AbortEpOut( USBD_Ep_TypeDef *ep )
{
  int cnt;

  /* Clear epdis INT's */
  USB_DOUTEPS[ ep->num ].INT |= USB_DOEP_INT_EPDISBLD;

  /* Clear Global OUT NAK if already set */
  USB->DCTL = ( USB->DCTL & ~DCTL_WO_BITMASK ) | USB_DCTL_CGOUTNAK;
  USB->GINTMSK |= USB_GINTMSK_GOUTNAKEFFMSK;    /* Enable GOUTNAKEFF int */

  /* Set Global OUT NAK */
  USB->DCTL = ( USB->DCTL & ~DCTL_WO_BITMASK ) | USB_DCTL_SGOUTNAK;

  /* Wait for goutnakeff */
  cnt = EPABORT_BREAK_LOOP_COUNT;
  while ( !( USB->GINTSTS & USB_GINTSTS_GOUTNAKEFF ) && cnt )
  {
    cnt--;
  }

  USB->GINTMSK &= ~USB_GINTMSK_GOUTNAKEFFMSK; /* Disable GOUTNAKEFF int  */
  USB->DOEPMSK |= USB_DOEPMSK_EPDISBLDMSK;    /* Enable EPDIS interrupt  */

  DEBUG_USB_INT_LO_PUTCHAR( ',' );

  USBDHAL_SetEPDISNAK( ep );                  /* Disable ep */

  /* Wait for epdis INT */
  cnt = EPABORT_BREAK_LOOP_COUNT;
  while ( !( USBDHAL_GetOutEpInts( ep ) & USB_DOEP_INT_EPDISBLD ) && cnt )
  {
    cnt--;
  }

  USB_DOUTEPS[ ep->num ].INT = USB_DOEP_INT_EPDISBLD;
  USB->DOEPMSK &= ~USB_DOEPMSK_EPDISBLDMSK;     /* Disable EPDIS interrupt */

  /* Clear Global OUT NAK */
  USB->DCTL = ( USB->DCTL & ~DCTL_WO_BITMASK ) | USB_DCTL_CGOUTNAK;

  /* Clear any interrupts generated by the abort sequence. */
  NVIC_ClearPendingIRQ( USB_IRQn );

  DEBUG_USB_INT_LO_PUTCHAR( ',' );
}

void USBDHAL_AbortAllEps( void )
{
  int i, cnt;
  USBD_Ep_TypeDef *ep;
  uint16_t im, om, inmask=0, outmask=0;

  /* Clear epdis & inepnakeff INT's */
  for ( i = 1; i <= NUM_EP_USED; i++ )
  {
    ep = &dev->ep[i];
    if ( ep->state != D_EP_IDLE )
    {
      if ( ep->in )
      {
        inmask |= ep->mask;
        USB_DINEPS[ ep->num ].INT |= USB_DIEP_INT_EPDISBLD |
                                     USB_DIEP_INT_INEPNAKEFF;
      }
      else
      {
        outmask |= ep->mask;
        USB_DOUTEPS[ ep->num ].INT |= USB_DOEP_INT_EPDISBLD;
      }
    }
  }

  if ( inmask )
  {
    /* Enable epdis & inepnakeff INT's */
    USB->DIEPMSK |= USB_DIEPMSK_EPDISBLDMSK | USB_DIEPMSK_INEPNAKEFFMSK;

    /* Set NAK on all IN ep's */
    im = inmask;
    for ( i = 1; i <= NUM_EP_USED; i++ )
    {
      ep = &dev->ep[i];
      if ( im & ep->mask )
      {
        USB_DINEPS[ ep->num ].CTL = ( USB_DINEPS[ ep->num ].CTL &
                                      ~DEPCTL_WO_BITMASK          ) |
                                    USB_DIEP_CTL_SNAK;
      }
    }
  }

  if ( outmask )
  {
    /* Clear Global OUT NAK if already set */
    USB->DCTL = ( USB->DCTL & ~DCTL_WO_BITMASK ) | USB_DCTL_CGOUTNAK;

    USB->GINTMSK |= USB_GINTMSK_GOUTNAKEFFMSK;    /* Enable GOUTNAKEFF int */

    /* Set Global OUT NAK */
    USB->DCTL = ( USB->DCTL & ~DCTL_WO_BITMASK ) | USB_DCTL_SGOUTNAK;

    /* Wait for goutnakeff */
    cnt = EPABORT_BREAK_LOOP_COUNT;
    while ( !( USB->GINTSTS & USB_GINTSTS_GOUTNAKEFF ) && cnt )
    {
      cnt--;
    }
    USB->GINTMSK &= ~USB_GINTMSK_GOUTNAKEFFMSK; /* Disable GOUTNAKEFF int  */
    USB->DOEPMSK |= USB_DOEPMSK_EPDISBLDMSK;    /* Enable EPDIS interrupt  */
  }

  if ( inmask )
  {
    /* Wait for inepnakeff INT on all IN ep's */
    im  = inmask;
    cnt = EPABORT_BREAK_LOOP_COUNT;
    do
    {
      for ( i = 1; i <= NUM_EP_USED; i++ )
      {
        ep = &dev->ep[i];
        if ( im & ep->mask )
        {
          if ( USBDHAL_GetInEpInts( ep ) & USB_DIEP_INT_INEPNAKEFF )
          {
            USB_DINEPS[ ep->num ].INT = USB_DIEP_INT_INEPNAKEFF;
            im &= ~ep->mask;
          }
        }
      }
      cnt--;
    } while ( im && cnt );
    USB->DIEPMSK &= ~USB_DIEPMSK_INEPNAKEFFMSK;
  }

  DEBUG_USB_INT_LO_PUTCHAR( '\'' );

  /* Disable ep's */
  for ( i = 1; i <= NUM_EP_USED; i++ )
  {
    ep = &dev->ep[i];
    if ( ep->state != D_EP_IDLE )
    {
      USBDHAL_SetEPDISNAK( ep );
    }
  }

  /* Wait for epdis INT */
  im  = inmask;
  om  = outmask;
  cnt = EPABORT_BREAK_LOOP_COUNT;
  do
  {
    for ( i = 1; i <= NUM_EP_USED; i++ )
    {
      ep = &dev->ep[i];
      if ( ep->in && ( im & ep->mask ) )
      {
        if ( USBDHAL_GetInEpInts( ep ) & USB_DIEP_INT_EPDISBLD )
        {
          USB_DINEPS[ ep->num ].INT = USB_DIEP_INT_EPDISBLD;
          im &= ~ep->mask;
        }
      }

      if ( !ep->in && ( om & ep->mask ) )
      {
        if ( USBDHAL_GetOutEpInts( ep ) & USB_DOEP_INT_EPDISBLD )
        {
          USB_DOUTEPS[ ep->num ].INT = USB_DOEP_INT_EPDISBLD;
          om &= ~ep->mask;
        }
      }
    }
    cnt--;
  } while ( ( im || om ) && cnt );

  if ( inmask )
  {
    USB->DIEPMSK &= ~USB_DIEPMSK_EPDISBLDMSK;     /* Disable EPDIS interrupt */
    USBHAL_FlushTxFifo( 0x10 );                   /* Flush all Tx FIFO's     */
  }

  if ( outmask )
  {
    USB->DOEPMSK &= ~USB_DOEPMSK_EPDISBLDMSK;     /* Disable EPDIS interrupt */
    /* Clear Global OUT NAK */
    USB->DCTL = ( USB->DCTL & ~DCTL_WO_BITMASK ) | USB_DCTL_CGOUTNAK;
  }

  DEBUG_USB_INT_LO_PUTCHAR( '\'' );
}

void USBDHAL_AbortAllTransfers( USB_Status_TypeDef reason )
{
  int i;
  USBD_Ep_TypeDef *ep;
  USB_XferCompleteCb_TypeDef callback;

  if ( reason != USB_STATUS_DEVICE_RESET )
  {
    USBDHAL_AbortAllEps();
  }

  for ( i = 1; i <= NUM_EP_USED; i++ )
  {
    ep = &(dev->ep[i]);
    if ( ep->state != D_EP_IDLE )
    {
      ep->state = D_EP_IDLE;
      if ( ep->xferCompleteCb )
      {
        callback = ep->xferCompleteCb;
        ep->xferCompleteCb = NULL;

        if ( ( dev->lastState == USBD_STATE_CONFIGURED ) &&
             ( dev->state     == USBD_STATE_ADDRESSED  )    )
        {
          USBDHAL_DeactivateEp( ep );
        }

        DEBUG_TRACE_ABORT( reason );
        callback( reason, ep->xferred, ep->remaining );
      }
    }
  }

  /* Clear any interrupts generated by the abort sequence. */
  NVIC_ClearPendingIRQ( USB_IRQn );
}
#endif /* defined( USB_DEVICE ) */

#if defined( USB_HOST )
USB_Status_TypeDef USBHHAL_CoreInit( uint32_t rxFifoSize,
                                     uint32_t nptxFifoSize,
                                     uint32_t ptxFifoSize )
{
  uint8_t i;

  rxFifoSize   /= 4;              /* Convert from byte count to word count.  */
  nptxFifoSize /= 4;
  ptxFifoSize  /= 4;

  CMU_ClockEnable( cmuClock_GPIO, true );
  GPIO_PinModeSet( gpioPortF, 5, gpioModePushPull, 0 ); /* Enable VBUSEN pin */

#if ( USB_VBUSOVRCUR_PORT != USB_VBUSOVRCUR_PORT_NONE )
  /* Enable VBUS overcurrent flag pin. */
  GPIO_PinModeSet( USB_VBUSOVRCUR_PORT, USB_VBUSOVRCUR_PIN, gpioModeInput, 0 );
#endif

  USB->ROUTE = USB_ROUTE_PHYPEN | USB_ROUTE_VBUSENPEN;  /* Enable PHY pins.  */
  USBHAL_CoreReset();                                   /* Reset USB core    */

  /* Force Host Mode */
  USB->GUSBCFG = ( USB->GUSBCFG                                     &
                   ~(GUSBCFG_WO_BITMASK | USB_GUSBCFG_FORCEDEVMODE )  ) |
                 USB_GUSBCFG_FORCEHSTMODE;

  INT_Enable();
  USBTIMER_DelayMs( 100 );
  INT_Disable();

  /* Set 48 MHz PHY clock, FS/LS mode */
  USB->HCFG = ( USB->HCFG & ~_USB_HCFG_FSLSPCLKSEL_MASK ) |
              ( 1 << _USB_HCFG_FSLSPCLKSEL_SHIFT        ) |
              ( USB_HCFG_FSLSSUPP                       );

  USB->GAHBCFG = ( USB->GAHBCFG & ~_USB_GAHBCFG_HBSTLEN_MASK ) |
                 USB_GAHBCFG_DMAEN | USB_GAHBCFG_HBSTLEN_INCR;

  /* Set Rx FIFO size */
  USB->GRXFSIZ = ( rxFifoSize << _USB_GRXFSIZ_RXFDEP_SHIFT ) &
                 _USB_GRXFSIZ_RXFDEP_MASK;

  /* Set Tx FIFO sizes */
  USB->GNPTXFSIZ = ( ( nptxFifoSize <<
                       _USB_GNPTXFSIZ_NPTXFINEPTXF0DEP_SHIFT ) &
                     _USB_GNPTXFSIZ_NPTXFINEPTXF0DEP_MASK        ) |
                   ( ( rxFifoSize <<
                       _USB_GNPTXFSIZ_NPTXFSTADDR_SHIFT      ) &
                     _USB_GNPTXFSIZ_NPTXFSTADDR_MASK             );

  USB->HPTXFSIZ  = ( ( ptxFifoSize  << _USB_HPTXFSIZ_PTXFSIZE_SHIFT   ) &
                     _USB_HPTXFSIZ_PTXFSIZE_MASK                          ) |
                   ( ( ( rxFifoSize + nptxFifoSize )
                                    << _USB_HPTXFSIZ_PTXFSTADDR_SHIFT ) &
                     _USB_HPTXFSIZ_PTXFSTADDR_MASK                        );

  /* Flush Tx and Rx FIFO's */
  USBHAL_FlushTxFifo( 0x10 );
  USBHAL_FlushRxFifo();

  for ( i = 0; i < MAX_NUM_HOSTCHANNELS; i++ )
  {
    USB->HC[ i ].CHAR = USB_HC_CHAR_CHDIS;      /* Disable channel          */
    USB->HC[ i ].INT = 0xFFFFFFFF;              /* Clear pending interrupts */
  }

  /* Enable and halt all channels */
  for ( i = 0; i < MAX_NUM_HOSTCHANNELS; i++ )
  {
    USB->HC[ i ].CHAR |= USB_HC_CHAR_CHDIS | USB_HC_CHAR_CHENA;
    do
    {
      __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    }
    while ( USB->HC[ i ].CHAR & USB_HC_CHAR_CHENA );
  }

  /* Disable all interrupts */
  for ( i = 0; i < MAX_NUM_HOSTCHANNELS; i++ )
  {
    USB->HC[ i ].INTMSK = 0;
  }

  USB->HAINTMSK = 0;

  return USB_STATUS_OK;
}

void USBHHAL_HCHalt( int hcnum, uint32_t hcchar )
{
  hcchar |= USB_HC_CHAR_CHENA | USB_HC_CHAR_CHDIS;
  USB->HC[ hcnum ].CHAR = hcchar;
}

void USBHHAL_HCInit( int hcnum )
{
  USBH_Ep_TypeDef *ep;

  ep = hcs[ hcnum ].ep;
  USB->HC[ hcnum ].INT = 0xFFFFFFFF;      /* Clear all interrupt flags      */

  switch ( ep->type )                     /* Enable host channel int. types */
  {
    case USB_EPTYPE_CTRL:
    case USB_EPTYPE_BULK:
    case USB_EPTYPE_INTR:
      USB->HC[ hcnum ].INTMSK = USB_HC_INT_CHHLTD;
      break;
  }

  hcs[ hcnum ].errorCnt = 0;

  USB->HAINTMSK |= 1 << hcnum;            /* Enable host channel interrupt  */

  USB->HC[ hcnum ].CHAR =                 /* Program HCCHAR register        */
      ( ep->parentDevice->addr     <<   _USB_HC_CHAR_DEVADDR_SHIFT     ) |
      ( ( ep->addr & USB_EPNUM_MASK ) << _USB_HC_CHAR_EPNUM_SHIFT      ) |
      ( ep->type                   <<   _USB_HC_CHAR_EPTYPE_SHIFT      ) |
      ( ep->packetSize             <<   _USB_HC_CHAR_MPS_SHIFT         ) |
      ( ep->in                      ?   USB_HC_CHAR_EPDIR         : 0  ) |
      ( ep->parentDevice->speed ==
                           HPRT_L_SPEED >> _USB_HPRT_PRTSPD_SHIFT
                                    ?   USB_HC_CHAR_LSPDDEV       : 0  );
}

void USBHHAL_HCStart( int hcnum )
{
  USBH_Hc_TypeDef *hc;
  uint16_t packets, len;

  hc = &hcs[ hcnum ];
  hc->status = 0;
  hc->idle = false;

  if ( hc->remaining > 0 )
  {
    packets = ( hc->remaining + hc->ep->packetSize - 1 ) / hc->ep->packetSize;
  }
  else
  {
    packets = 1;
  }

  if ( hc->ep->in )
  {
    len = packets * hc->ep->packetSize;
  }
  else
  {
    len = hc->remaining;
  }

  /* Initialize the HCTSIZn register */
  hc->hwXferSize = len;
  USB->HC[ hcnum ].TSIZ =
          ( ( len             << _USB_HC_TSIZ_XFERSIZE_SHIFT ) &
                                 _USB_HC_TSIZ_XFERSIZE_MASK       ) |
          ( ( packets         << _USB_HC_TSIZ_PKTCNT_SHIFT   ) &
                                 _USB_HC_TSIZ_PKTCNT_MASK         ) |
          ( ( hc->ep->toggle  << _USB_HC_TSIZ_PID_SHIFT      ) &
                                 _USB_HC_TSIZ_PID_MASK            );

  USB->HC[ hcnum ].DMAADDR = (uint32_t)hc->buf;

  USBHHAL_HCActivate( hcnum,
                      USB->HC[ hcnum ].CHAR,
                      hc->ep->type == USB_EPTYPE_INTR );
}
#endif /* defined( USB_HOST ) */

/** @endcond */

#endif /* defined( USB_DEVICE ) || defined( USB_HOST ) */
#endif /* defined( USB_PRESENT ) && ( USB_COUNT == 1 ) */
