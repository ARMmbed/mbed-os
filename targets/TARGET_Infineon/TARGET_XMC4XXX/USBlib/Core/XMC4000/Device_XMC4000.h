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

/*
 * Copyright (C) 2015 Infineon Technologies AG. All rights Reserved bits.
 *
 * Infineon Technologies AG (Infineon) is supplying this software for use with
 * Infineon's microcontrollers.
 * This file can be freely distributed within development tools that are
 * supporting such microcontrollers.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 */

/** \file
 *  \brief USB Device definitions for the Infineon XMC4000 microcontrollers.
 *  \copydetails Group_Device_XMC
 *
 *  \note This file should not be included directly. It is automatically included as needed by the USB driver
 *        dispatch header located in LUFA/Drivers/USB/USB.h.
 */

/** \ingroup Group_Device
 *  \defgroup Group_Device_XMC Device Management (XMC)
 *  \brief USB Device definitions for the Infineon XMC4000 microcontrollers.
 *
 *  Architecture specific USB Device definitions for the Infineon XMC4000 microcontrollers.
 *
 *  @{
 */

#ifndef __USBDEVICE_XMC4000_H__
#define __USBDEVICE_XMC4000_H__

	/* Includes: */
        #include "../../Common/Common.h"
		#include "../USBController.h"
		#include "../StdDescriptors.h"
		#include "../USBInterrupt.h"
		#include "../Endpoint.h"

	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_USB_DRIVER)
			#error Do not include this file directly. USB/USB.h instead.
		#endif

	/* Public Interface - May be used in end-application: */
			/* Macros: */

			#define USE_INTERNAL_SERIAL            NO_DESCRIPTOR

			#define INTERNAL_SERIAL_LENGTH_BITS    0
			#define INTERNAL_SERIAL_START_ADDRESS  0

		/* Inline Functions: */
			static inline void USB_Device_SetDeviceAddress(const uint8_t Address) ATTR_ALWAYS_INLINE;
			static inline void USB_Device_SetDeviceAddress(const uint8_t Address)
			{
                (void)Address;
			}

			static inline void USB_Device_EnableDeviceAddress(const uint8_t Address) ATTR_ALWAYS_INLINE;
			static inline void USB_Device_EnableDeviceAddress(const uint8_t Address)
			{
                device.Driver->DeviceSetAddress(Address, XMC_USBD_SET_ADDRESS_STAGE_SETUP);
			}

			static inline bool USB_Device_IsAddressSet(void) ATTR_ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT;
			static inline bool USB_Device_IsAddressSet(void)
			{
				dcfg_data_t data;
				data.d32 = xmc_device.device_register->dcfg;
				return ((data.b.devaddr != 0) ? true : false);
			}

			/** Returns the current USB frame number, when in device mode. Every millisecond the USB bus is active (i.e. enumerated to a host)
			 *  the frame number is incremented by one.
			 *
			 *  \return Current USB frame number from the USB controller.
			 */
    
            static inline uint16_t USB_Device_GetFrameNumber(void) ATTR_ALWAYS_INLINE;
            static inline uint16_t USB_Device_GetFrameNumber(void) {
                return device.Driver->GetFrameNumber();
            }

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif
            
#endif /* __USBDEVICE_XMC4000_H__ */

/** @} */

