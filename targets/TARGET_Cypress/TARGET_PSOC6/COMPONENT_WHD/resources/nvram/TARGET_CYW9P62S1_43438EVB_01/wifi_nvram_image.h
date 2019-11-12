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
 *  NVRAM file for CYW9P62S1-43438EVB-01
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
 * Generated from cyw943438_26MHz_aw-cu427.txt
 */
static const char wifi_nvram_image[] =
        "NVRAMRev=$Rev: 717633 $"                                            "\x00"
        "manfid=0x2d0"                                                       "\x00"
        "prodid=0x0727"                                                      "\x00"
        "vendid=0x14e4"                                                      "\x00"
        "devid=0x43e2"                                                       "\x00"
        "boardtype=0x0865"                                                   "\x00"
        "boardrev=0x1100"                                                    "\x00"
        "boardnum=22"                                                        "\x00"
        NVRAM_GENERATED_MAC_ADDRESS                                          "\x00"
        "sromrev=11"                                                         "\x00"
        "boardflags=0x00404201"                                              "\x00"
        "boardflags3=0x08000000"                                             "\x00"
        "xtalfreq=26000"                                                     "\x00"
        "nocrc=1"                                                            "\x00"
        "ag0=255"                                                            "\x00"
        "aa2g=1"                                                             "\x00"
        "ccode=ALL"                                                          "\x00"
        "swdiv_en=1"                                                         "\x00"
        "swdiv_gpio=1"                                                       "\x00"
        "swdiv_gpio_1=2"                                                     "\x00"
        "pa0itssit=0x20"                                                     "\x00"
        "extpagain2g=0"                                                      "\x00"
        "pa2ga0=-148,6421,-710"                                              "\x00"
        "AvVmid_c0=0x0,0xc8"                                                 "\x00"
        "cckpwroffset0=5"                                                    "\x00"
        "maxp2ga0=84"                                                        "\x00"
        "txpwrbckof=6"                                                       "\x00"
        "cckbw202gpo=0"                                                      "\x00"
        "legofdmbw202gpo=0x66111111"                                         "\x00"
        "mcsbw202gpo=0x77711111"                                             "\x00"
        "propbw202gpo=0xdd"                                                  "\x00"
        "ofdmdigfilttype=18"                                                 "\x00"
        "ofdmdigfilttypebe=18"                                               "\x00"
        "papdmode=1"                                                         "\x00"
        "papdvalidtest=1"                                                    "\x00"
        "pacalidx2g=32"                                                      "\x00"
        "papdepsoffset=-36"                                                  "\x00"
        "papdendidx=61"                                                      "\x00"
        "wl0id=0x431b"                                                       "\x00"
        "deadman_to=0xffffffff"                                              "\x00"
        "muxenab=0x11"                                                       "\x00"
        "spurconfig=0x3"                                                     "\x00"
        "rssicorrnorm=3"                                                     "\x00"
        "\x00\x00";

#ifdef __cplusplus
} /*extern "C" */
#endif

#else /* ifndef INCLUDED_NVRAM_IMAGE_H_ */
#error Wi-Fi NVRAM image included twice
#endif /* ifndef INCLUDED_NVRAM_IMAGE_H_ */

