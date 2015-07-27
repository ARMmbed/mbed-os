/**************************************************************************//**
 * @file em_usbdep.c
 * @brief USB protocol stack library, USB device endpoint handlers.
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
#if defined( USB_DEVICE )

#include "em_usbtypes.h"
#include "em_usbhal.h"
#include "em_usbd.h"

#ifdef USB_USE_PRINTF
static const char *epStatusStr[] = {
    "IDLE","TRANS","RECV","IN_S","OUT_S"
};
#endif

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/*
 * USBDEP_Ep0Handler() is called each time a packet has been transmitted
 * or recieved on the default endpoint.
 * A state machine navigate us through the phases of a control transfer
 * according to "chapter 9" in the USB spec.
 */
#if !defined( USB_DOEP0INT_STUPPKTRCVD )
void USBDEP_Ep0Handler( USBD_Device_TypeDef *device )
{
  int status;
  USBD_Ep_TypeDef *ep;
  static bool statusIn;
  static uint32_t xferred;
  static USB_XferCompleteCb_TypeDef callback;

  ep = &device->ep[ 0 ];

#ifdef __MBED__

  (void)xferred;
  (void)statusIn;
  (void)status;

  USB_PRINTF("USBDEP: ep0 %s, rem %ld, z %d\n", epStatusStr[ep->state], ep->remaining, ep->zlp);

  if ( ( ep->state == D_EP_TRANSMITTING ) || ( ep->state == D_EP_RECEIVING ) )
  {
    ep->state = D_EP_IDLE;

    if ( ep->xferCompleteCb )
    {
      callback = ep->xferCompleteCb;
      ep->xferCompleteCb = NULL;
      callback( USB_STATUS_OK, ep->xferred, ep->remaining );
    }

    USBDHAL_ReenableEp0Setup(device);
  }
  else
  {
    device->callbacks->setupCmd(device->setup);
  }

#else /* not __MBED__ */

  switch ( ep->state )
  {
    case D_EP_IDLE:
      ep->remaining = 0;
      ep->zlp = 0;
      callback = NULL;
      statusIn = false;

      status = USBDCH9_SetupCmd( device );

      if ( status == USB_STATUS_REQ_ERR )
      {
        ep->in = true;
        USBDHAL_StallEp( ep );              /* Stall Ep0 IN                 */
        ep->in = false;                     /* OUT for next SETUP           */
        USBDHAL_StallEp( ep );              /* Stall Ep0 OUT                */
        USBDHAL_ReenableEp0Setup( device ); /* Prepare for next SETUP packet*/
      }
      else /* ( Status == USB_STATUS_OK ) */
      {
        if ( (ep->state == D_EP_RECEIVING) || (ep->state == D_EP_TRANSMITTING) )
        {
          callback = ep->xferCompleteCb;
        }

        if ( ep->state != D_EP_RECEIVING )
        {
          if ( ep->remaining )
          {
            /* Data will be sent to host, check if a ZLP must be appended */
            if ( ( ep->remaining < device->setup->wLength ) &&
                 ( ep->remaining % ep->packetSize == 0    )    )
            {
              ep->zlp = 1;
            }
          }
          else
          {
            /* Prepare for next SETUP packet*/
            USBDHAL_ReenableEp0Setup( device );

            /* No data stage, a ZLP may have been sent. If not, send one */

            xferred = 0;
            if ( ep->zlp == 0 )
            {
              USBD_Write( 0, NULL, 0, NULL );             /* ACK to host */
              ep->state = D_EP0_IN_STATUS;
            }
            else
            {
              ep->state = D_EP_IDLE;
              ep->in = false;                      /* OUT for next SETUP */
            }
          }
        }
      }
      break;

    case D_EP_RECEIVING:
      if ( ep->remaining )
      {
        /* There is more data to receive */
        USBD_ReArmEp0( ep );
      }
      else
      {
        status = USB_STATUS_OK;
        if ( callback != NULL )
        {
          status = callback( USB_STATUS_OK, ep->xferred, 0 );
          callback = NULL;
        }

        if ( status != USB_STATUS_OK )
        {
          ep->in = true;
          USBDHAL_StallEp( ep );              /* Stall Ep0 IN                */
          ep->in = false;                     /* OUT for next SETUP           */
          USBDHAL_StallEp( ep );              /* Stall Ep0 OUT                */
          USBDHAL_ReenableEp0Setup( device ); /* Prepare for next SETUP pkt. */
          ep->state = D_EP_IDLE;
        }
        else /* Everything OK, send a ZLP (ACK) to host */
        {
          USBDHAL_ReenableEp0Setup( device );/* Prepare for next SETUP packet*/

          ep->state = D_EP_IDLE;              /* USBD_Write() sets state back*/
                                              /* to EP_TRANSMITTING          */
          USBD_Write( 0, NULL, 0, NULL );
          ep->state = D_EP0_IN_STATUS;
        }
      }
      break;

    case D_EP_TRANSMITTING:
      if ( ep->remaining )
      {
        /* There is more data to transmit */
        USBD_ReArmEp0( ep );
      }
      else
      {
        /* All data transferred, is a ZLP packet needed ? */
        if ( ep->zlp == 1 )
        {
          xferred   = ep->xferred;
          ep->state = D_EP_IDLE;          /* USBD_Write() sets state back */
                                          /* to EP_TRANSMITTING           */
          USBD_Write( 0, NULL, 0, NULL ); /* Send ZLP                     */
          ep->zlp = 2;
        }
        else
        {
          if ( ep->zlp == 0 )
          {
            xferred = ep->xferred;
          }

          ep->state = D_EP_IDLE;
          USBD_Read( 0, NULL, 0, NULL );  /* Get ZLP packet (ACK) from host */
          statusIn = true;
          ep->state = D_EP0_OUT_STATUS;
        }
      }
      break;

    case D_EP0_IN_STATUS:
    case D_EP0_OUT_STATUS:
      if ( statusIn )
      {
        USBDHAL_ReenableEp0Setup( device );
      }

      if ( callback != NULL )
      {
        callback( USB_STATUS_OK, xferred, 0 );
      }

      ep->state = D_EP_IDLE;
      ep->in = false;                     /* OUT for next SETUP */
      break;

    default:
      EFM_ASSERT( false );
      break;
  }
#endif /* __MBED__ */
}
#endif

#if defined( USB_DOEP0INT_STUPPKTRCVD )
void USBDEP_Ep0Handler( USBD_Device_TypeDef *device )
{
  int status;
  USBD_Ep_TypeDef *ep;
  static uint32_t xferred;
  static USB_XferCompleteCb_TypeDef callback;

#ifdef __MBED__

  (void)xferred;
  (void)status;

  ep = &device->ep[ 0 ];

  if ( ( ep->state == D_EP_TRANSMITTING ) || ( ep->state == D_EP_RECEIVING ) )
  {
    ep->state = D_EP_IDLE;

    if ( ep->xferCompleteCb )
    {
      callback = ep->xferCompleteCb;
      ep->xferCompleteCb = NULL;
      callback( USB_STATUS_OK, ep->xferred, ep->remaining );
    }

    USBDHAL_StartEp0Setup( dev );
  }
  else
  {
    device->callbacks->setupCmd(device->setup);
  }

#else

  ep = &device->ep[ 0 ];

  switch ( ep->state )
  {
    case D_EP_IDLE:
      ep->zlp       = 0;
      ep->remaining = 0;
      callback      = NULL;

      status = USBDCH9_SetupCmd( device );

      if ( status == USB_STATUS_REQ_ERR )
      {
        ep->in = true;
        USBDHAL_StallEp( ep );              /* Stall Ep0 IN                 */
        ep->in = false;                     /* OUT for next SETUP           */
        USBDHAL_StallEp( ep );              /* Stall Ep0 OUT                */
        USBDHAL_StartEp0Setup( dev );       /* Prepare for next SETUP packet*/
      }
      else /* ( Status == USB_STATUS_OK ) */
      {
        if ( (ep->state == D_EP_RECEIVING) || (ep->state == D_EP_TRANSMITTING) )
        {
          callback = ep->xferCompleteCb;
        }

        if ( ep->state != D_EP_RECEIVING )
        {
          if ( ep->remaining )
          {
            /* Data will be sent to host, check if a ZLP must be appended */
            if ( ( ep->remaining < device->setup->wLength ) &&
                 ( ep->remaining % ep->packetSize == 0    )    )
            {
              ep->zlp = 1;
            }
          }
          else
          {
            /* No data stage, a ZLP may have been sent. If not, send one */
            xferred = 0;
            if ( ep->zlp == 0 )
            {
              ep->state = D_EP_IDLE;
              USBD_Write( 0, NULL, 0, NULL );             /* ACK to host */
              ep->state = D_EP0_IN_STATUS;
            }
          }
        }
      }
      break;

    case D_EP_RECEIVING:
      if ( ep->remaining )
      {
        ep->in = false;
        USBD_ReArmEp0( ep );
      }
      else
      {
        status = USB_STATUS_OK;
        if ( callback != NULL )
        {
          status = callback( USB_STATUS_OK, ep->xferred, 0 );
          callback = NULL;
        }

        if ( status != USB_STATUS_OK )
        {
          ep->in = true;
          USBDHAL_StallEp( ep );              /* Stall Ep0 IN                */
          ep->in = false;                     /* OUT for next SETUP          */
          USBDHAL_StallEp( ep );              /* Stall Ep0 OUT               */
          USBDHAL_StartEp0Setup( dev );       /* Prepare for next SETUP pkt. */
          ep->state = D_EP_IDLE;
        }
        else
        {

          USBDHAL_StartEp0Setup( dev );      /* Prepare for next SETUP packet*/
          ep->state = D_EP_IDLE;             /* USBD_Write() sets state back */
                                             /* to EP_TRANSMITTING           */
          USBD_Write( 0, NULL, 0, NULL );
          ep->state = D_EP0_IN_STATUS;
        }
      }
      break;

    case D_EP_TRANSMITTING:
      if ( ep->remaining )
      {
        ep->in = true;
        USBD_ReArmEp0( ep );
      }
      else
      {
        if ( ep->zlp == 1 )
        {
          xferred   = ep->xferred;
          ep->state = D_EP_IDLE;          /* USBD_Write() sets state back */
                                          /* to EP_TRANSMITTING           */
          USBD_Write( 0, NULL, 0, NULL ); /* Send ZLP                     */
          ep->zlp = 2;
        }
        else
        {
          if ( ep->zlp == 0 )
          {
            xferred = ep->xferred;
          }

          ep->state = D_EP_IDLE;
          USBD_Read( 0, NULL, 0, NULL );  /* Get ZLP packet (ACK) from host */
          ep->state = D_EP0_OUT_STATUS;
        }
      }
      break;

    case D_EP0_IN_STATUS:
      if ( ( USB->DOEP0CTL & USB_DOEP0CTL_EPENA ) == 0 )
      {
        /* Prepare for more SETUP Packets */
        USBDHAL_StartEp0Setup( dev );
      }
      if ( callback != NULL )
      {
        callback( USB_STATUS_OK, xferred, 0 );
      }
      ep->state = D_EP_IDLE;
      ep->in = false;                     /* OUT for next SETUP */
      break;

    case D_EP0_OUT_STATUS:
      USBDHAL_StartEp0Setup( dev );       /* Prepare for more SETUP Packets */
      if ( callback != NULL )
      {
        callback( USB_STATUS_OK, xferred, 0 );
      }
      ep->state = D_EP_IDLE;
      ep->in = false;                     /* OUT for next SETUP */
      break;
  }
#endif /* __MBED__ */
}
#endif

/*
 * USBDEP_EpHandler() is called each time a packet has been transmitted
 * or recieved on an endpoint other than the default endpoint.
 */
void USBDEP_EpHandler( uint8_t epAddr )
{
  USB_XferCompleteCb_TypeDef callback;
  USBD_Ep_TypeDef *ep = USBD_GetEpFromAddr( epAddr );

  if ( ( ep->state == D_EP_TRANSMITTING ) || ( ep->state == D_EP_RECEIVING ) )
  {
    ep->state = D_EP_IDLE;
    if ( ep->xferCompleteCb )
    {
      callback = ep->xferCompleteCb;
      ep->xferCompleteCb = NULL;
      callback( USB_STATUS_OK, ep->xferred, ep->remaining );
    }
  }
  else
  {
    EFM_ASSERT( false );
  }
}

/** @endcond */

#endif /* defined( USB_DEVICE ) */
#endif /* defined( USB_PRESENT ) && ( USB_COUNT == 1 ) */
