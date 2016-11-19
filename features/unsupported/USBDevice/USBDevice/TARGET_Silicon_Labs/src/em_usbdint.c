/**************************************************************************//**
 * @file em_usbdint.c
 * @brief USB protocol stack library, USB device peripheral interrupt handlers.
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

#include "em_cmu.h"
#include "em_usbtypes.h"
#include "em_usbhal.h"
#include "em_usbd.h"

#ifdef __MBED__
extern void usbhal_allow_em2(bool em2_allow);
#endif

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#define HANDLE_INT( x ) if ( status & x ) { Handle_##x(); status &= ~x; }

static void Handle_USB_GINTSTS_ENUMDONE  ( void );
static void Handle_USB_GINTSTS_IEPINT    ( void );
static void Handle_USB_GINTSTS_OEPINT    ( void );
static void Handle_USB_GINTSTS_RESETDET  ( void );
static void Handle_USB_GINTSTS_SOF       ( void );
static void Handle_USB_GINTSTS_USBRST    ( void );
static void Handle_USB_GINTSTS_USBSUSP   ( void );
static void Handle_USB_GINTSTS_WKUPINT   ( void );
#if defined( USB_DOEP0INT_STUPPKTRCVD )
static void HandleOutEpIntr( uint32_t status, USBD_Ep_TypeDef *ep );
#else
static void ProcessSetup                 ( void );
static void ProcessOepData               ( USBD_Ep_TypeDef *ep );
#endif

#if ( USB_PWRSAVE_MODE )
/* Variables and prototypes for USB powerdown (suspend) functionality. */
static bool UsbPowerDown( void );
static bool UsbPowerUp( void );

volatile bool USBD_poweredDown = false;

/* Storage for backing up USB core registers. */
static uint32_t  x_USB_GINTMSK;
#if defined(_USB_GOTGCTL_MASK)
static uint32_t  x_USB_GOTGCTL;
#endif
static uint32_t  x_USB_GAHBCFG;
static uint32_t  x_USB_GUSBCFG;
static uint32_t  x_USB_GRXFSIZ;
static uint32_t  x_USB_GNPTXFSIZ;
static uint32_t  x_USB_DCFG;
static uint32_t  x_USB_DCTL;
static uint32_t  x_USB_DAINTMSK;
static uint32_t  x_USB_DIEPMSK;
static uint32_t  x_USB_DOEPMSK;
static uint32_t  x_USB_PCGCCTL;

#if ( NUM_EP_USED > 0 )
static uint32_t  x_USB_EP_CTL[ NUM_EP_USED ];
static uint32_t  x_USB_EP_TSIZ[ NUM_EP_USED ];
static uint32_t  x_USB_EP_DMAADDR[ NUM_EP_USED ];
#endif

#if ( NUM_EP_USED > MAX_NUM_TX_FIFOS )
#define FIFO_CNT MAX_NUM_TX_FIFOS
#else
#define FIFO_CNT NUM_EP_USED
#endif

#if ( FIFO_CNT > 0 )
static uint32_t  x_USB_DIEPTXFS[ FIFO_CNT ];
#endif

#if ( USB_PWRSAVE_MODE )
static uint32_t cmuStatus = 0;
#endif

#endif /* if ( USB_PWRSAVE_MODE ) */

/*
 * USB_IRQHandler() is the first level handler for the USB peripheral interrupt.
 */
void USB_IRQHandler( void )
{
  uint32_t status;
  bool servedVbusInterrupt = false;

  INT_Disable();

#if ( USB_PWRSAVE_MODE )
  if ( USBD_poweredDown )
  {
    /* Switch USBC clock from 32kHz to a 48MHz clock to be able to  */
    /* read USB peripheral registers.                               */
    /* If we woke up from EM2, HFCLK is now HFRCO.                  */

    /* Restore clock oscillators.*/
#if defined( CMU_OSCENCMD_USHFRCOEN )
    if ( ( CMU->STATUS & CMU_STATUS_USHFRCOENS ) == 0 )/*Wakeup from EM2 ?*/
    {
      CMU->OSCENCMD = ( cmuStatus
                        & ( CMU_STATUS_AUXHFRCOENS | CMU_STATUS_HFXOENS ) )
                      | CMU_OSCENCMD_USHFRCOEN;
    }
#else
    if ( ( CMU->STATUS & CMU_STATUS_HFXOENS ) == 0 ) /* Wakeup from EM2 ? */
    {
      CMU->OSCENCMD = cmuStatus
                      & ( CMU_STATUS_AUXHFRCOENS | CMU_STATUS_HFXOENS );
    }
#endif

    /* Select correct USBC clock.*/
#if defined( CMU_OSCENCMD_USHFRCOEN )
    CMU->CMD = CMU_CMD_USBCCLKSEL_USHFRCO;
    while ( ( CMU->STATUS & CMU_STATUS_USBCUSHFRCOSEL ) == 0 ){}
#else
    CMU->CMD = CMU_CMD_USBCCLKSEL_HFCLKNODIV;
    while ( ( CMU->STATUS & CMU_STATUS_USBCHFCLKSEL ) == 0 ){}
#endif
  }
#endif /* if ( USB_PWRSAVE_MODE ) */

  if ( USB->IF && ( USB->CTRL & USB_CTRL_VREGOSEN ) )
  {
    if ( USB->IF & USB_IF_VREGOSH )
    {
      USB->IFC = USB_IFC_VREGOSH;

      if ( USB->STATUS & USB_STATUS_VREGOS )
      {
        servedVbusInterrupt = true;
        DEBUG_USB_INT_LO_PUTS( "\nVboN" );

#if ( USB_PWRSAVE_MODE )
        if ( UsbPowerUp() )
        {
          USBDHAL_EnableUsbResetAndSuspendInt();
        }
        USBD_SetUsbState( USBD_STATE_POWERED );
#endif
      }
    }

    if ( USB->IF & USB_IF_VREGOSL )
    {
      USB->IFC = USB_IFC_VREGOSL;

      if ( ( USB->STATUS & USB_STATUS_VREGOS ) == 0 )
      {
        servedVbusInterrupt = true;
        DEBUG_USB_INT_LO_PUTS( "\nVboF" );

#if ( USB_PWRSAVE_MODE )
#if ( USB_PWRSAVE_MODE & USB_PWRSAVE_MODE_ONVBUSOFF )
        if ( !USBD_poweredDown )
        {
          USB->GINTMSK = 0;
          USB->GINTSTS = 0xFFFFFFFF;
        }

        UsbPowerDown();
#endif
        USBD_SetUsbState( USBD_STATE_NONE );
#endif
      }
    }
  }

  status = USBHAL_GetCoreInts();
  if ( status == 0 )
  {
    INT_Enable();
    if ( !servedVbusInterrupt )
    {
      DEBUG_USB_INT_LO_PUTS( "\nSinT" );
    }
    return;
  }

  HANDLE_INT( USB_GINTSTS_RESETDET   )
  HANDLE_INT( USB_GINTSTS_WKUPINT    )
  HANDLE_INT( USB_GINTSTS_USBSUSP    )
  HANDLE_INT( USB_GINTSTS_SOF        )
  HANDLE_INT( USB_GINTSTS_ENUMDONE   )
  HANDLE_INT( USB_GINTSTS_USBRST     )
  HANDLE_INT( USB_GINTSTS_IEPINT     )
  HANDLE_INT( USB_GINTSTS_OEPINT     )

  INT_Enable();

  if ( status != 0 )
  {
    DEBUG_USB_INT_LO_PUTS( "\nUinT" );
  }
}

/*
 * Handle port enumeration interrupt. This has nothing to do with normal
 * device enumeration.
 */
static void Handle_USB_GINTSTS_ENUMDONE( void )
{
#if ( USB_PWRSAVE_MODE )
  UsbPowerUp();
#endif

  USBDHAL_Ep0Activate( dev->ep0MpsCode );
  dev->ep[ 0 ].state = D_EP_IDLE;
  USBDHAL_EnableInts( dev );
  DEBUG_USB_INT_LO_PUTS( "EnumD" );
}

/*
 * Handle IN endpoint transfer interrupt.
 */
static void Handle_USB_GINTSTS_IEPINT( void )
{
  int epnum;
  uint16_t epint;
  uint16_t epmask;
  uint32_t status;
  USBD_Ep_TypeDef *ep;

  DEBUG_USB_INT_HI_PUTCHAR( 'i' );

  epint = USBDHAL_GetAllInEpInts();
  for ( epnum = 0,                epmask = 1;
        epnum <= MAX_NUM_IN_EPS;
        epnum++,                  epmask <<= 1 )
  {
    if ( epint & epmask )
    {
      ep = USBD_GetEpFromAddr( USB_SETUP_DIR_MASK | epnum );
      status = USBDHAL_GetInEpInts( ep );

      if ( status & USB_DIEP_INT_XFERCOMPL )
      {
        USB_DINEPS[ epnum ].INT = USB_DIEP_INT_XFERCOMPL;

        DEBUG_USB_INT_HI_PUTCHAR( 'c' );

        if ( epnum == 0 )
        {
          if ( ep->remaining > ep->packetSize )
          {
            ep->remaining -= ep->packetSize;
            ep->xferred += ep->packetSize;
          }
          else
          {
            ep->xferred += ep->remaining;
            ep->remaining = 0;
          }
          USBDEP_Ep0Handler( dev );
        }
        else
        {
          ep->xferred = ep->remaining -
                        ( ( USB_DINEPS[ epnum ].TSIZ      &
                            _USB_DIEP_TSIZ_XFERSIZE_MASK    ) >>
                          _USB_DIEP_TSIZ_XFERSIZE_SHIFT          );
          ep->remaining -= ep->xferred;

          USBDEP_EpHandler( ep->addr );
#if defined( USB_DOEP0INT_STUPPKTRCVD )
          if ( USB_DINEPS[ ep->num ].INT & USB_DIEP_INT_NAKINTRPT )
          {
            USB_DINEPS[ ep->num ].INT = USB_DIEP_INT_NAKINTRPT;
          }
#endif
        }
      }
    }
  }
}

/*
 * Handle OUT endpoint transfer interrupt.
 */
static void Handle_USB_GINTSTS_OEPINT( void )
{
  int epnum;
  uint16_t epint;
  uint16_t epmask;
  uint32_t status;
  USBD_Ep_TypeDef *ep;

  DEBUG_USB_INT_HI_PUTCHAR( 'o' );

  epint = USBDHAL_GetAllOutEpInts();
  for ( epnum = 0,                epmask = 1;
        epnum <= MAX_NUM_OUT_EPS;
        epnum++,                  epmask <<= 1 )
  {
    if ( epint & epmask )
    {
      ep = USBD_GetEpFromAddr( epnum );
      status = USBDHAL_GetOutEpInts( ep );

#if defined( USB_DOEP0INT_STUPPKTRCVD )
      HandleOutEpIntr( status, ep );
#else
      if ( status & USB_DOEP_INT_XFERCOMPL )
      {
        USB_DOUTEPS[ epnum ].INT = USB_DOEP_INT_XFERCOMPL;
        DEBUG_USB_INT_HI_PUTCHAR( 'c' );
        ProcessOepData( ep );
      }

      /* Setup Phase Done */
      if ( status & USB_DOEP0INT_SETUP )
      {
        ProcessSetup();
      }
#endif
    }
  }
}

#if !defined( USB_DOEP0INT_STUPPKTRCVD )
static void ProcessOepData( USBD_Ep_TypeDef *ep )
{
  if ( ep->num == 0 )
  {

#ifdef __MBED__
    int xfer_size = ep->packetSize - (( USB->DOEP0TSIZ & _USB_DOEP0TSIZ_XFERSIZE_MASK )
                                      >> _USB_DOEP0TSIZ_XFERSIZE_SHIFT);
    int setup_pkt_received = USBDHAL_GetOutEpInts( ep ) & USB_DOEP0INT_SETUP;

    if ( (!setup_pkt_received && xfer_size == 0) ||
         (setup_pkt_received && xfer_size == 8) )
    {
      /* Higher levels need to see the correct transfer amount for ZLPs */
      ep->remaining = 0;
      ep->xferred = 0;
    }
    else
    {
      /* FIXME - does not work if actual read size > 56 */
      if ( setup_pkt_received ) xfer_size -= 8;

      ep->xferred = xfer_size;
      ep->remaining -= xfer_size;
    }
#else
    if ( ep->remaining > ep->packetSize )
    {
      ep->remaining -= ep->packetSize;
      ep->xferred += ep->packetSize;
    }
    else
    {
      ep->xferred += ep->remaining;
      ep->remaining = 0;
    }
#endif

    USBDEP_Ep0Handler( dev );
  }
  else
  {
    ep->xferred = ep->hwXferSize -
        ( ( USB_DOUTEPS[ ep->num ].TSIZ & _USB_DOEP_TSIZ_XFERSIZE_MASK )>>
          _USB_DOEP_TSIZ_XFERSIZE_SHIFT );
    ep->remaining -= ep->xferred;
    USBDEP_EpHandler( ep->addr );
  }
}
#endif

#if !defined( USB_DOEP0INT_STUPPKTRCVD )
static void ProcessSetup( void )
{
  DEBUG_USB_INT_LO_PUTS( "\nS" );

  if ( USB->DOEP0INT & USB_DOEP0INT_BACK2BACKSETUP )
  {                           /* Back to back setup packets received */
    USB->DOEP0INT = USB_DOEP0INT_BACK2BACKSETUP;
    DEBUG_USB_INT_LO_PUTS( "B2B" );

    dev->setup = (USB_Setup_TypeDef*)( USB->DOEP0DMAADDR - USB_SETUP_PKT_SIZE );
  }
  else
  {
    /* Read SETUP packet counter from hw. */
    int supCnt = ( USB->DOEP0TSIZ & _USB_DOEP0TSIZ_SUPCNT_MASK )
                 >> _USB_DOEP0TSIZ_SUPCNT_SHIFT;

    if ( supCnt == 3 )
      supCnt = 2;

    dev->setup = &dev->setupPkt[ 2 - supCnt ];
  }
  USB->DOEP0TSIZ |= 3 << _USB_DOEP0TSIZ_SUPCNT_SHIFT;
  USB->DOEP0DMAADDR = (uint32_t)dev->setupPkt;
  USB->DOEP0INT = USB_DOEP0INT_SETUP;

  USBDEP_Ep0Handler( dev );   /* Call the SETUP handler for EP0 */
}
#endif

/*
 * Handle USB reset detected interrupt in suspend mode.
 */
static void Handle_USB_GINTSTS_RESETDET  ( void )
{
#if ( USB_PWRSAVE_MODE )
  if ( ! USBD_poweredDown )
  {
    USB->GINTSTS = USB_GINTSTS_RESETDET;
  }

  if ( UsbPowerUp() )
  {
    USB->GINTSTS = USB_GINTSTS_RESETDET;
  }

#if ( USB_PWRSAVE_MODE & USB_PWRSAVE_MODE_ONVBUSOFF )
  /* Power down immediately if VBUS is off. */
  if ( ! ( USB->STATUS & USB_STATUS_VREGOS ) )
  {
    UsbPowerDown();
  }
#endif

#else
  USB->GINTSTS = USB_GINTSTS_RESETDET;
#endif /* if ( USB_PWRSAVE_MODE ) */

  if ( USB->STATUS & USB_STATUS_VREGOS )
  {
    USBD_SetUsbState( USBD_STATE_DEFAULT );
  }
  else
  {
    USBD_SetUsbState( USBD_STATE_NONE );
  }
  DEBUG_USB_INT_LO_PUTS( "RsuP\n" );
}

/*
 * Handle Start Of Frame (SOF) interrupt.
 */
static void Handle_USB_GINTSTS_SOF( void )
{
  USB->GINTSTS = USB_GINTSTS_SOF;

  if ( dev->callbacks->sofInt )
  {
    dev->callbacks->sofInt(
      ( USB->DSTS & _USB_DSTS_SOFFN_MASK ) >> _USB_DSTS_SOFFN_SHIFT );
  }
}

/*
 * Handle USB port reset interrupt.
 */
static void Handle_USB_GINTSTS_USBRST( void )
{
  int i;

  DEBUG_USB_INT_LO_PUTS( "ReseT" );

  /* Clear Remote Wakeup Signalling */
  USB->DCTL &= ~( DCTL_WO_BITMASK | USB_DCTL_RMTWKUPSIG );
  USBHAL_FlushTxFifo( 0 );

  /* Clear pending interrupts */
  for ( i = 0; i <= MAX_NUM_IN_EPS; i++ )
  {
    USB_DINEPS[ i ].INT = 0xFFFFFFFF;
  }

  for ( i = 0; i <= MAX_NUM_OUT_EPS; i++ )
  {
    USB_DOUTEPS[ i ].INT = 0xFFFFFFFF;
  }

  USB->DAINTMSK = USB_DAINTMSK_INEPMSK0 | USB_DAINTMSK_OUTEPMSK0;
#if defined( USB_DOEPMSK_STSPHSERCVDMSK )
  USB->DOEPMSK  = USB_DOEPMSK_SETUPMSK  | USB_DOEPMSK_XFERCOMPLMSK
                  | USB_DOEPMSK_STSPHSERCVDMSK;
#else
  USB->DOEPMSK  = USB_DOEPMSK_SETUPMSK  | USB_DOEPMSK_XFERCOMPLMSK;
#endif
  USB->DIEPMSK  = USB_DIEPMSK_XFERCOMPLMSK;

  /* Reset Device Address */
  USB->DCFG &= ~_USB_DCFG_DEVADDR_MASK;

  /* Setup EP0 to receive SETUP packets */
  USBDHAL_StartEp0Setup( dev );
  USBDHAL_EnableInts( dev );

  if ( dev->callbacks->usbReset )
  {
    dev->callbacks->usbReset();
  }

  USBD_SetUsbState( USBD_STATE_DEFAULT );
  USBDHAL_AbortAllTransfers( USB_STATUS_DEVICE_RESET );
}

/*
 * Handle USB port suspend interrupt.
 */
static void Handle_USB_GINTSTS_USBSUSP( void )
{
  USBD_State_TypeDef state;

  USB->GINTSTS = USB_GINTSTS_USBSUSP;
  USBDHAL_AbortAllTransfers( USB_STATUS_DEVICE_SUSPENDED );
  DEBUG_USB_INT_LO_PUTS( "\nSusP" );

  if ( USBD_GetUsbState() == USBD_STATE_NONE )
  {
    USBD_SetUsbState( USBD_STATE_POWERED );
  }

  state = USBD_GetUsbState();
  if ( ( state == USBD_STATE_POWERED    ) ||
       ( state == USBD_STATE_DEFAULT    ) ||
       ( state == USBD_STATE_ADDRESSED  ) ||
       ( state == USBD_STATE_CONFIGURED )    )
  {
#if ( USB_PWRSAVE_MODE )
    UsbPowerDown();
#endif
    USBD_SetUsbState( USBD_STATE_SUSPENDED );
  }
}

/*
 * Handle USB port wakeup interrupt.
 */
static void Handle_USB_GINTSTS_WKUPINT( void )
{
#if ( USB_PWRSAVE_MODE )
  if ( ! USBD_poweredDown )
  {
    USB->GINTSTS = USB_GINTSTS_WKUPINT;
  }

  if ( UsbPowerUp() )
  {
    USB->GINTSTS = USB_GINTSTS_WKUPINT;
    USBDHAL_StartEp0Setup( dev );
    USBDHAL_Ep0Activate( dev->ep0MpsCode );
  }
#else
  USB->GINTSTS = USB_GINTSTS_WKUPINT;
#endif

  USBD_SetUsbState( dev->savedState );
  DEBUG_USB_INT_LO_PUTS( "WkuP\n" );
}

#if ( USB_PWRSAVE_MODE )
/*
 * Backup essential USB core registers, and set the core in partial powerdown
 * mode. Optionally prepare entry into EM2.
 */
static bool UsbPowerDown( void )
{
#if ( NUM_EP_USED > 0 ) || ( FIFO_CNT > 0 )
  int i;
#endif
#if ( NUM_EP_USED > 0 )
  int epNum;
  USBD_Ep_TypeDef *ep;
#endif

  if ( !USBD_poweredDown )
  {
    USBD_poweredDown = true;
    DEBUG_USB_INT_LO_PUTCHAR( '\\' );

    /* Backup USB core registers. */
    x_USB_GINTMSK   = USB->GINTMSK;
#if defined(_USB_GOTGCTL_MASK)
    x_USB_GOTGCTL   = USB->GOTGCTL;
#endif
    x_USB_GAHBCFG   = USB->GAHBCFG;
    x_USB_GUSBCFG   = USB->GUSBCFG;
    x_USB_GRXFSIZ   = USB->GRXFSIZ;
    x_USB_GNPTXFSIZ = USB->GNPTXFSIZ;
    x_USB_DCFG      = USB->DCFG;
    x_USB_DCTL      = USB->DCTL;
    x_USB_DAINTMSK  = USB->DAINTMSK;
    x_USB_DIEPMSK   = USB->DIEPMSK;
    x_USB_DOEPMSK   = USB->DOEPMSK;
    x_USB_PCGCCTL   = USB->PCGCCTL;

#if ( NUM_EP_USED > 0 )
    for ( i = 0; i < NUM_EP_USED; i++ )
    {
      ep = &dev->ep[ i+1 ];
      epNum = ep->num;
      if ( ep->in )
      {
        x_USB_EP_CTL[     i ] = USB_DINEPS[ epNum ].CTL;
        x_USB_EP_TSIZ[    i ] = USB_DINEPS[ epNum ].TSIZ;
        x_USB_EP_DMAADDR[ i ] = USB_DINEPS[ epNum ].DMAADDR;
      }
      else
      {
        x_USB_EP_CTL[     i ] = USB_DOUTEPS[ epNum ].CTL;
        x_USB_EP_TSIZ[    i ] = USB_DOUTEPS[ epNum ].TSIZ;
        x_USB_EP_DMAADDR[ i ] = USB_DOUTEPS[ epNum ].DMAADDR;
      }
    }
#endif

#if ( FIFO_CNT > 0 )
    for ( i = 0; i < FIFO_CNT; i++ )
    {
      x_USB_DIEPTXFS[ i ] = USB_DIEPTXFS[ i ];
    }
#endif

    /* Prepare for wakeup on resume and reset. */
    USB->DCFG    = (USB->DCFG & ~_USB_DCFG_RESVALID_MASK) |
                   (4 << _USB_DCFG_RESVALID_SHIFT);
    USB->DCFG   |= USB_DCFG_ENA32KHZSUSP;
    USB->GINTMSK = USB_GINTMSK_RESETDETMSK | USB_GINTMSK_WKUPINTMSK;

    /* Enter partial powerdown mode. */
    USB->PCGCCTL |= USB_PCGCCTL_PWRCLMP;
    USB->PCGCCTL |= USB_PCGCCTL_RSTPDWNMODULE;
    USB->PCGCCTL |= USB_PCGCCTL_STOPPCLK;

    /* Record current clock settings. */
    cmuStatus = CMU->STATUS;

#if ( USB_PWRSAVE_MODE & USB_PWRSAVE_MODE_ENTEREM2 )
#ifndef __MBED__
    /* Enter EM2 on interrupt exit. */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk;
#else
    usbhal_allow_em2(true);
#endif
#endif

    /* Switch USBC clock to 32 kHz. */
#if ( USB_USBC_32kHz_CLK == USB_USBC_32kHz_CLK_LFXO )
    CMU->CMD = CMU_CMD_USBCCLKSEL_LFXO;
    while ( ( CMU->STATUS & CMU_STATUS_USBCLFXOSEL ) == 0 ){}
#else
    CMU->CMD = CMU_CMD_USBCCLKSEL_LFRCO;
    while ( ( CMU->STATUS & CMU_STATUS_USBCLFRCOSEL ) == 0 ){}
#endif

    return true;
  }
  return false;
}
#endif /* if ( USB_PWRSAVE_MODE ) */

#if ( USB_PWRSAVE_MODE )
/*
 * Exit USB core partial powerdown mode, restore essential USB core registers.
 * Will prevent re-entry back to EM2.
 * Returns true if a powerup sequence was performed.
 */
static bool UsbPowerUp( void )
{
#if ( NUM_EP_USED > 0 ) || ( FIFO_CNT > 0 )
  int i;
#endif
#if ( NUM_EP_USED > 0 )
  int epNum;
  uint32_t tmp;
  USBD_Ep_TypeDef *ep;
#endif

  if ( USBD_poweredDown )
  {
    USBD_poweredDown = false;
    DEBUG_USB_INT_LO_PUTCHAR( '/' );

#if !defined( USB_CORECLK_HFRCO ) || !defined( CMU_OSCENCMD_USHFRCOEN )
    /* Switch HFCLK from HFRCO to HFXO. */
    CMU_ClockSelectSet( cmuClock_HF, cmuSelect_HFXO );
#endif

    /* Turn off HFRCO when not needed. */
    if ( ( cmuStatus & CMU_STATUS_HFRCOENS ) == 0 )
    {
      CMU->OSCENCMD = CMU_OSCENCMD_HFRCODIS;
    }

    /* Exit partial powerdown mode. */
    USB->PCGCCTL &= ~USB_PCGCCTL_STOPPCLK;
    USB->PCGCCTL &= ~(USB_PCGCCTL_PWRCLMP | USB_PCGCCTL_RSTPDWNMODULE);

    if (( USB->GINTSTS & ( USB_GINTSTS_WKUPINT | USB_GINTSTS_RESETDET ) ) == 0)
    {
      USB->DCTL = x_USB_DCTL | USB_DCTL_RMTWKUPSIG;
      USB->DCTL = x_USB_DCTL;
    }

    /* Restore USB core registers. */
    USB->GUSBCFG = x_USB_GUSBCFG;
    USB->DCFG    = x_USB_DCFG;

#if ( FIFO_CNT > 0 )
    for ( i = 0; i < FIFO_CNT; i++ )
    {
      USB_DIEPTXFS[ i ] = x_USB_DIEPTXFS[ i ];
    }
#endif

#if ( NUM_EP_USED > 0 )
    for ( i = 0; i < NUM_EP_USED; i++ )
    {
      ep = &dev->ep[ i+1 ];
      epNum = ep->num;

      tmp = x_USB_EP_CTL[ i ] &
            ~( USB_DIEP_CTL_CNAK       | USB_DIEP_CTL_SNAK       |
               USB_DIEP_CTL_SETD0PIDEF | USB_DIEP_CTL_SETD1PIDOF   );

      if ( x_USB_EP_CTL[ i ] & USB_DIEP_CTL_DPIDEOF )
        tmp |= USB_DIEP_CTL_SETD1PIDOF;
      else
        tmp |= USB_DIEP_CTL_SETD0PIDEF;

      if ( x_USB_EP_CTL[ i ] & USB_DIEP_CTL_NAKSTS )
        tmp |= USB_DIEP_CTL_SNAK;
      else
        tmp |= USB_DIEP_CTL_CNAK;

      if ( ep->in )
      {
        USB_DINEPS[ epNum ].CTL     = tmp;
        USB_DINEPS[ epNum ].TSIZ    = x_USB_EP_TSIZ[    i ];
        USB_DINEPS[ epNum ].DMAADDR = x_USB_EP_DMAADDR[ i ];
      }
      else
      {
        USB_DOUTEPS[ epNum ].CTL     = tmp;
        USB_DOUTEPS[ epNum ].TSIZ    = x_USB_EP_TSIZ[    i ];
        USB_DOUTEPS[ epNum ].DMAADDR = x_USB_EP_DMAADDR[ i ];
      }
    }
#endif

    USB->PCGCCTL   = x_USB_PCGCCTL;
    USB->DOEPMSK   = x_USB_DOEPMSK;
    USB->DIEPMSK   = x_USB_DIEPMSK;
    USB->DAINTMSK  = x_USB_DAINTMSK;
    USB->DCTL      = x_USB_DCTL;
    USB->GNPTXFSIZ = x_USB_GNPTXFSIZ;
    USB->GRXFSIZ   = x_USB_GRXFSIZ;
    USB->GAHBCFG   = x_USB_GAHBCFG;
#if defined(_USB_GOTGCTL_MASK)
    USB->GOTGCTL   = x_USB_GOTGCTL;
#endif
    USB->GINTMSK   = x_USB_GINTMSK;

    USB->DCTL |= USB_DCTL_PWRONPRGDONE;

#if ( USB_PWRSAVE_MODE & USB_PWRSAVE_MODE_ENTEREM2 )
#ifndef __MBED__
    /* Do not reenter EM2 on interrupt exit. */
    SCB->SCR &= ~(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk);
#else
    usbhal_allow_em2(false);
#endif
#endif

    return true;
  }
  return false;
}
#endif /* if ( USB_PWRSAVE_MODE ) */

#if defined( USB_DOEP0INT_STUPPKTRCVD )
static void HandleOutEpIntr( uint32_t status, USBD_Ep_TypeDef *ep )
{
  uint32_t doeptsiz;

  if ( ep->num == 0 )
  {
    if ( status & USB_DOEP0INT_XFERCOMPL )
    {
      USB->DOEP0INT = USB_DOEP0INT_XFERCOMPL;
      doeptsiz      = USB->DOEP0TSIZ;

      if ( ep->state == D_EP_IDLE )
      {
        if ( status & USB_DOEP0INT_STUPPKTRCVD )
        {
          USB->DOEP0INT = USB_DOEP0INT_STUPPKTRCVD;
        }
        status = USBDHAL_GetOutEpInts( ep );
        doeptsiz = USB->DOEP0TSIZ;

        if ( status & USB_DOEP0INT_SETUP )
        {
retry:
          /* Already started data stage, clear setup */
          USB->DOEP0INT = USB_DOEP0INT_SETUP;
          status       &= ~USB_DOEP0INT_SETUP;
          {
            int supCnt = ( doeptsiz & _USB_DOEP0TSIZ_SUPCNT_MASK )
                         >> _USB_DOEP0TSIZ_SUPCNT_SHIFT;

            if ( supCnt == 3 )
              supCnt = 2;

            dev->setup = &dev->setupPkt[ 2 - supCnt ];
          }
          DEBUG_USB_INT_LO_PUTS( "\nS" );
          USBDEP_Ep0Handler( dev );

          /* Prepare for more setup packets */
          if ( ep->state == D_EP0_IN_STATUS || ep->state == D_EP_TRANSMITTING )
          {
            USBDHAL_StartEp0Setup( dev );
          }
        }
        else /* xfercompl && idle && !setup */
        {
            status = USBDHAL_GetOutEpInts( ep );
            if ( status & USB_DOEP0INT_SETUP )
              goto retry;
            USBDHAL_StartEp0Setup( dev );
        }
      }
      else /* ep0state != EP0_IDLE */
      {
#ifdef __MBED__
        if ( ep->state == D_EP_RECEIVING )
        {
          int xfer_size = ep->packetSize - (( USB->DOEP0TSIZ & _USB_DOEP0TSIZ_XFERSIZE_MASK )
                                            >> _USB_DOEP0TSIZ_XFERSIZE_SHIFT);
          int setup_pkt_received = status & USB_DOEP0INT_SETUP;

          if ( (!setup_pkt_received && xfer_size == 0) ||
               (setup_pkt_received && xfer_size == 8) )
          {
            /* Higher levels need to see the correct transfer amount for ZLPs */
            ep->remaining = 0;
            ep->xferred = 0;
          }
          else
          {
            /* FIXME - does not work if actual read size > 56 */
            if ( setup_pkt_received ) xfer_size -= 8;

            ep->xferred = xfer_size;
            ep->remaining -= xfer_size;
          }

          USBDEP_Ep0Handler( dev );
        }
#else
        if ( ep->state == D_EP_RECEIVING )
        {
          if ( ep->remaining > ep->packetSize )
          {
            ep->remaining -= ep->packetSize;
            ep->xferred += ep->packetSize;
          }
          else
          {
            ep->xferred += ep->remaining;
            ep->remaining = 0;
          }
          USBDEP_Ep0Handler( dev );
        }
        else if ( ep->state == D_EP0_OUT_STATUS )
        {
          USBDEP_Ep0Handler( dev );
        }
#endif
      }
    } /* if ( status & USB_DOEP0INT_XFERCOMPL ) */

    if ( status & USB_DOEP0INT_STSPHSERCVD )
    {
      USB->DOEP0INT = USB_DOEP0INT_STSPHSERCVD;
    }

    if ( status & USB_DOEP0INT_SETUP )
    {
      USB->DOEP0INT = USB_DOEP0INT_SETUP;
      {
        int supCnt = ( USB->DOEP0TSIZ & _USB_DOEP0TSIZ_SUPCNT_MASK )
                     >> _USB_DOEP0TSIZ_SUPCNT_SHIFT;

        if ( supCnt == 3 )
          supCnt = 2;

        dev->setup = &dev->setupPkt[ 2 - supCnt ];
      }
      DEBUG_USB_INT_LO_PUTS( "\nS" );
      USBDEP_Ep0Handler( dev );
    }
  }
  else /* epnum != 0 */
  {
    if ( status & USB_DOEP_INT_XFERCOMPL )
    {
      USB_DOUTEPS[ ep->num ].INT = USB_DOEP_INT_XFERCOMPL;

      ep->xferred = ep->hwXferSize -
          ( ( USB_DOUTEPS[ ep->num ].TSIZ & _USB_DOEP_TSIZ_XFERSIZE_MASK )>>
            _USB_DOEP_TSIZ_XFERSIZE_SHIFT );
      ep->remaining -= ep->xferred;

      USBDEP_EpHandler( ep->addr );
    }
  }
}
#endif

/** @endcond */

#endif /* defined( USB_DEVICE ) */
#endif /* defined( USB_PRESENT ) && ( USB_COUNT == 1 ) */
