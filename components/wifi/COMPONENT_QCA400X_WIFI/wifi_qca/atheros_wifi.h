//------------------------------------------------------------------------------
// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// · Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// · Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// · Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================
#ifndef __ATHEROS_WIFI_H__
#define __ATHEROS_WIFI_H__

/* PORT_NOTE: atheros_wifi.h is intended to provide any system specific data
 * 	structures and definitions required by the OS for a WIFI/Ethernet driver.
 * 	Hence everything in this file is Customized to the requirements of the
 *	host OS. */

#include <a_osapi.h>

#include <wlan_config.h>

// ============================================================================
// This is the ONLY place where the enet.h header files are included!
// ============================================================================

// typedef struct wlan_hardware_config_s {
////	/// The SPI bus instance number. Usually 0 or 1
////	int spi_instance;
////	/// The SPI chip select (dspi_which_pcs_config_t)
////	int spi_cs;
////	/// The SPI baudrate
////	uint32_t spi_baudrate;
////
////	/// The WLAN interrupt's IRQ number from the IRQn enum in MK22F51212.h (or similar)
////	int wlan_irq_number;
////	/// The GPIO ping used for power control of the WLAN chip, e.g. GPIO_MAKE_PIN(GPIOB_IDX, 19)
////	int wlan_power_pin;
//} wlan_hardware_config_t;

#include "qca_structs.h"

#endif /* __ATHEROS_WIFI_H__ */
