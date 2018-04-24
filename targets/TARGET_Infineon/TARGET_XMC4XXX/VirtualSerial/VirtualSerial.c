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
 *
 *  Main source file for the VirtualSerial demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "VirtualSerial.h"
#include "xmc_scu.h"

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
{
        .Config =
        {
                .ControlInterfaceNumber   = INTERFACE_ID_CDC_CCI,
                .DataINEndpoint           =
                {
                        .Address          = CDC_TX_EPADDR,
                        .Size             = CDC_TXRX_EPSIZE,
                        .Banks            = 1,
                },
                .DataOUTEndpoint =
                {
                        .Address          = CDC_RX_EPADDR,
                        .Size             = CDC_TXRX_EPSIZE,
                        .Banks            = 1,
                },
                .NotificationEndpoint =
                {
                        .Address          = CDC_NOTIFICATION_EPADDR,
                        .Size             = CDC_NOTIFICATION_EPSIZE,
                        .Banks            = 1,
                },
        },
};

uint8_t ep2_buf[64];
uint8_t ep3_buf[64];
uint8_t ep4_buf[64];

/* USB runtime structure*/
XMC_USBD_t USB_runtime =
{
        .usbd = USB0,
        .usbd_max_num_eps = XMC_USBD_MAX_NUM_EPS_6,
        .usbd_transfer_mode = XMC_USBD_USE_DMA,
        .cb_xmc_device_event = USBD_SignalDeviceEventHandler,
        .cb_endpoint_event = USBD_SignalEndpointEvent_Handler

};

/* Clock configuration */
/* fPLL = 120MHz */
/* fSYS = 120MHz */
/* fUSBPLL = 192MHz */
/* fUSB = 48MHz */
const XMC_SCU_CLOCK_CONFIG_t clock_config =
{
  .syspll_config.p_div = 2,
  .syspll_config.n_div = 80,
  .syspll_config.k_div = 4,
  .syspll_config.mode = XMC_SCU_CLOCK_SYSPLL_MODE_NORMAL,
  .syspll_config.clksrc = XMC_SCU_CLOCK_SYSPLLCLKSRC_OSCHP,
  .enable_oschp = true,
  .calibration_mode = XMC_SCU_CLOCK_FOFI_CALIBRATION_MODE_FACTORY,
  .fsys_clksrc = XMC_SCU_CLOCK_SYSCLKSRC_PLL,
  .fsys_clkdiv = 1,
  .fcpu_clkdiv = 1,
  .fccu_clkdiv = 1,
  .fperipheral_clkdiv = 1
};

/*******************************************************************************
 **                     Public Function Definitions                            **
 *******************************************************************************/

void USB0_0_IRQHandler(void)
{
    XMC_USBD_IRQHandler(&USB_runtime);
}

/*The function initializes the USB core layer and register call backs. */
void USB_VCOM_Init(void)
{

    /* Setup settings for USB clock */
    XMC_SCU_CLOCK_Init(&clock_config);

    XMC_SCU_CLOCK_EnableUsbPll();
    XMC_SCU_CLOCK_StartUsbPll(2, 64);
    XMC_SCU_CLOCK_SetUsbClockDivider(4);
    XMC_SCU_CLOCK_SetUsbClockSource(XMC_SCU_CLOCK_USBCLKSRC_USBPLL);
    XMC_SCU_CLOCK_EnableClock(XMC_SCU_CLOCK_USB);

    SystemCoreClockUpdate();

    /* Init USB */
    USBD_Initialize(&USB_runtime);

    /* Interrupts configuration*/
    NVIC_SetPriority(USB0_0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 63, 0));
    NVIC_ClearPendingIRQ(USB0_0_IRQn);
    NVIC_EnableIRQ(USB0_0_IRQn);

    /* LUFA Class Line Encoding*/
    VirtualSerial_CDC_Interface.State.LineEncoding.BaudRateBPS = 9600;
    VirtualSerial_CDC_Interface.State.LineEncoding.DataBits = 8;

    /* USB Connection*/
    USB_Attach();
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Reset(void)
{
    if(device.IsConfigured)
    {
        USB_VCOM_Init();
        device.IsConfigured=0;
    }
}

/* Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    USBD_SetEndpointBuffer(CDC_NOTIFICATION_EPADDR, ep2_buf, 64);
    USBD_SetEndpointBuffer(CDC_TX_EPADDR, ep3_buf, 64);
    USBD_SetEndpointBuffer(CDC_RX_EPADDR, ep4_buf, 64);
    CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);

    device.IsConfigured = 1;
}

/* Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest()
{
    CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

