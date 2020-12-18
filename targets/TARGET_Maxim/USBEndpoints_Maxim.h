/*******************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#ifndef USBENDPOINTS_H
#define USBENDPOINTS_H

/* SETUP packet size */
#define SETUP_PACKET_SIZE (8)

/* Options flags for configuring endpoints */
#define DEFAULT_OPTIONS     (0)
#define SINGLE_BUFFERED     (1U << 0)
#define ISOCHRONOUS         (1U << 1)
#define RATE_FEEDBACK_MODE  (1U << 2) /* Interrupt endpoints only */

#define NUMBER_OF_LOGICAL_ENDPOINTS   (8)
#define NUMBER_OF_PHYSICAL_ENDPOINTS  (NUMBER_OF_LOGICAL_ENDPOINTS * 2)

#define EP_TO_INDEX(endpoint)   (((endpoint & 0xf) << 1) | (endpoint & 0x80 ? 1 : 0))

#define DIR_OUT       0x00
#define DIR_IN        0x80
#define EP_NUM(ep)    (ep & 0x0F)
#define IN_EP(ep)     (ep & DIR_IN)
#define OUT_EP(ep)    (!(ep & DIR_IN))

/* Define physical endpoint numbers */

/*      Endpoint    No.   */
/*      ----------------  */
#define EP0OUT      ((0 << 1) | DIR_OUT)
#define EP0IN       ((0 << 1) | DIR_IN)
#define EP1OUT      ((1 << 1) | DIR_OUT)
#define EP1IN       ((1 << 1) | DIR_IN)
#define EP2OUT      ((2 << 1) | DIR_OUT)
#define EP2IN       ((2 << 1) | DIR_IN)
#define EP3OUT      ((3 << 1) | DIR_OUT)
#define EP3IN       ((3 << 1) | DIR_IN)
#define EP4OUT      ((4 << 1) | DIR_OUT)
#define EP4IN       ((4 << 1) | DIR_IN)
#define EP5OUT      ((5 << 1) | DIR_OUT)
#define EP5IN       ((5 << 1) | DIR_IN)
#define EP6OUT      ((6 << 1) | DIR_OUT)
#define EP6IN       ((6 << 1) | DIR_IN)
#define EP7OUT      ((7 << 1) | DIR_OUT)
#define EP7IN       ((7 << 1) | DIR_IN)

/* Maximum Packet sizes */

#define MAX_PACKET_SIZE_EP0  (64)
#define MAX_PACKET_SIZE_EP1  (64)
#define MAX_PACKET_SIZE_EP2  (64)
#define MAX_PACKET_SIZE_EP3  (64)
#define MAX_PACKET_SIZE_EP4  (64)
#define MAX_PACKET_SIZE_EP5  (64)
#define MAX_PACKET_SIZE_EP6  (64)
#define MAX_PACKET_SIZE_EP7  (64)

/* Generic endpoints - intended to be portable accross devices */
/* and be suitable for simple USB devices. */

/* Bulk endpoints */
#define EPBULK_OUT  (EP1OUT)
#define EPBULK_IN   (EP2IN)

/* Interrupt endpoints */
#define EPINT_OUT   (EP3OUT)
#define EPINT_IN    (EP4IN)

/* Isochronous endpoints */
/* NOT SUPPORTED - use invalid endpoint number to prevent built errors */
#define EPISO_OUT   (EP0OUT)
#define EPISO_IN    (EP0IN)

#define MAX_PACKET_SIZE_EPBULK  (64)
#define MAX_PACKET_SIZE_EPINT   (64)

#endif
