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
 *  \brief USB mode and feature support definitions.
 *  \copydetails Group_USBMode
 *
 *  \note This file should not be included directly. It is automatically included as needed by the USB driver
 *        dispatch header located in LUFA/Drivers/USB/USB.h.
 */

/** \ingroup Group_USB
 *  \defgroup Group_USBMode USB Mode Tokens
 *  \brief USB mode and feature support definitions.
 *
 *  This file defines macros indicating the type of USB controller the library is being compiled for, and its
 *  capabilities. These macros may then be referenced in the user application to selectively enable or disable
 *  code sections depending on if they are defined or not.
 *
 *  After the inclusion of the master USB driver header, one or more of the following tokens may be defined, to
 *  allow the user code to conditionally enable or disable code based on the USB controller family and allowable
 *  USB modes. These tokens may be tested against to eliminate code relating to a USB mode which is not enabled for
 *  the given compilation.
 *
 *  @{
 */

#ifndef __USBMODE_H__
#define __USBMODE_H__

	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_USB_DRIVER)
			#error Do not include this file directly. Include LUFA/Drivers/USB/USB.h instead.
		#endif

	/* Includes: */
		#include "../Common/Common.h"

	/* Public Interface - May be used in end-application: */
	#if defined(__DOXYGEN__)
		/** Indicates that the target microcontroller and compilation settings allow for the
		 *  target to be configured in USB Device mode when defined.
		 */
		#define USB_CAN_BE_DEVICE
        
		/** Indicates that the target microcontroller and compilation settings allow for the
		 *  target to be configured in USB Host mode when defined.
		 */
		#define USB_CAN_BE_HOST

	#else
		/* Macros: */
			#define USB_CAN_BE_DEVICE
			//#define USB_CAN_BE_HOST

			#if (defined(USB_HOST_ONLY) && defined(USB_DEVICE_ONLY))
				#error USB_HOST_ONLY and USB_DEVICE_ONLY are mutually exclusive.
			#elif defined(USB_HOST_ONLY)
				#if !defined(USB_CAN_BE_HOST)
					#error USB_HOST_ONLY is not available for the currently selected microcontroller model.
				#else
					#undef USB_CAN_BE_DEVICE
				#endif
			#elif defined(USB_DEVICE_ONLY)
				#if !defined(USB_CAN_BE_DEVICE)
					#error USB_DEVICE_ONLY is not available for the currently selected microcontroller model.
				#else
					#undef USB_CAN_BE_HOST
				#endif
			#endif

			#if (defined(USB_CAN_BE_DEVICE) && defined(USB_CAN_BE_HOST))
				#define USB_CAN_BE_BOTH
			#endif

			#if (!defined(USB_CAN_BE_DEVICE) && !defined(USB_CAN_BE_HOST))
				#error The currently selected device, USB mode or architecture is not supported.
			#endif
	#endif

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif

/** @} */

