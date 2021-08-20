/*
 * Copyright (c) 2019, Cypress Semiconductor Corporation, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file
 *  NVRAM file for CY8CKIT-062-WiFi-BT (PSoC6 WiFi-BT Pioneer Kit) using
 *  Murata's Type 1DX module - Copied from bcm94343wwcd1.txt on 08/04/2019
 *  # 2.4 GHz, 20 MHz BW mode; No Antenna Diversity
 */

#ifndef INCLUDED_NVRAM_IMAGE_H_
#define INCLUDED_NVRAM_IMAGE_H_

#include <string.h>
#include <stdint.h>
#include "generated_mac_address.txt"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Character array of NVRAM image
 *
 * Generated from cyw94343cy8ckit-062-wifi-bt.txt
 */

static const char wifi_nvram_image[] =
        "manfid=0x2d0"                          "\x00"
        "prodid=0x0726"                         "\x00"
        "vendid=0x14e4"                         "\x00"
        "devid=0x43e2"                          "\x00"
        "boardtype=0x0726"                      "\x00"
        "boardrev=0x1202"                       "\x00"
        "boardnum=22"                           "\x00"
        NVRAM_GENERATED_MAC_ADDRESS             "\x00"
        "sromrev=11"                            "\x00"
        "boardflags=0x00404201"                 "\x00"
        "boardflags3=0x04000000"                "\x00"
        "xtalfreq=37400"                        "\x00"
        "nocrc=1"                               "\x00"
        "ag0=0"                                 "\x00"
        "aa2g=1"                                "\x00"
        "ccode=ALL"                             "\x00"
        "extpagain2g=0"                         "\x00"
        "pa2ga0=-145,6667,-751"                 "\x00"
        "AvVmid_c0=0x0,0xc8"                    "\x00"
        "cckpwroffset0=2"                       "\x00"
        "maxp2ga0=74"                           "\x00"
        "cckbw202gpo=0"                         "\x00"
        "legofdmbw202gpo=0x88888888"            "\x00"
        "mcsbw202gpo=0xaaaaaaaa"                "\x00"
        "propbw202gpo=0xdd"                     "\x00"
        "ofdmdigfilttype=18"                    "\x00"
        "ofdmdigfilttypebe=18"                  "\x00"
        "papdmode=1"                            "\x00"
        "papdvalidtest=1"                       "\x00"
        "pacalidx2g=48"                         "\x00"
        "papdepsoffset=-22"                     "\x00"
        "papdendidx=58"                         "\x00"
        "il0"NVRAM_GENERATED_MAC_ADDRESS        "\x00"
        "wl0id=0x431b"                          "\x00"
        "muxenab=0x10"                          "\x00"
        "\x00\x00";

#ifdef __cplusplus
} /* extern "C" */
#endif

#else /* ifndef INCLUDED_NVRAM_IMAGE_H_ */

#error Wi-Fi NVRAM image included twice

#endif /* ifndef INCLUDED_NVRAM_IMAGE_H_ */
