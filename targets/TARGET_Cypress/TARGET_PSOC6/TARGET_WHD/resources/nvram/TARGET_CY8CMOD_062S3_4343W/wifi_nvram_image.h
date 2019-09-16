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
 * # NVRAM file for CY8CMOD-062S3-4343W (PSoC6 (512K) with CYW4343W WiFi-BT - Murata's
 *   Type 1DX Module) - Copied from bcm94343wwcd1.txt on 08/04/2019
 * # 2.4 GHz, 20 MHz BW mode; With Antenna Diversity
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
 */

static const char wifi_nvram_image[] =
    // # The following parameter values are just placeholders, need to be updated.
    "manfid=0x2d0"                                                       "\x00"
    "prodid=0x086c"                                                      "\x00"
    "vendid=0x14e4"                                                      "\x00"
    "devid=0x43e2"                                                       "\x00"
    "boardtype=0x086c"                                                   "\x00"
    "boardrev=0x1101"                                                    "\x00"
    "boardnum=22"                                                        "\x00"
    NVRAM_GENERATED_MAC_ADDRESS                                          "\x00"
    "sromrev=11"                                                         "\x00"
    "boardflags=0x00404201"                                              "\x00"
    "boardflags3=0x08000000"                                             "\x00"
    "xtalfreq=37400"                                                     "\x00"
    "nocrc=1"                                                            "\x00"
    "ag0=255"                                                            "\x00"
    "aa2g=1"                                                             "\x00"
    "ccode=ALL"                                                          "\x00"

//#Antenna diversity
//#"swdiv_en=1"                                                         "\x00"
//#"swdiv_gpio=2"                                                       "\x00"

    "pa0itssit=0x20"                                                     "\x00"
    "extpagain2g=0"                                                      "\x00"

//#PA parameters for 2.4GHz, measured at CHIP OUTPUT
    "pa2ga0=-168,6905,-799"                                              "\x00"
    "AvVmid_c0=0x0,0xc8"                                                 "\x00"
    "cckpwroffset0=5"                                                    "\x00"

//# PPR params
    "maxp2ga0=84"                                                        "\x00"
    "txpwrbckof=6"                                                       "\x00"
    "cckbw202gpo=0"                                                      "\x00"
    "legofdmbw202gpo=0x66111111"                                         "\x00"
    "mcsbw202gpo=0x77711111"                                             "\x00"
    "propbw202gpo=0xdd"                                                  "\x00"

//# OFDM IIR :
    "ofdmdigfilttype=18"                                                 "\x00"
    "ofdmdigfilttypebe=18"                                               "\x00"

//# PAPD mode:
    "papdmode=1"                                                         "\x00"
    "papdvalidtest=1"                                                    "\x00"
    "pacalidx2g=32"                                                      "\x00"
    "papdepsoffset=-36"                                                  "\x00"
    "papdendidx=61"                                                      "\x00"

//# LTECX flags
//#"ltecxmux=1"                                                         "\x00"
//#"ltecxpadnum=0x02030401"                                             "\x00"
//#"ltecxfnsel=0x3003"                                                  "\x00"
//#"ltecxgcigpio=0x3012"                                                "\x00"

//#il0macaddr=00:90:4c:c5:12:38
    "wl0id=0x431b"                                                       "\x00"

    "deadman_to=0xffffffff"                                              "\x00"
    //# muxenab: 0x1 for UART enable, 0x2 for GPIOs, 0x8 for JTAG, 0x10 for HW OOB
    "muxenab=0x11"                                                        "\x00"
    //# CLDO PWM voltage settings - 0x4 - 1.1 volt
    //#cldo_pwm=0x4                                                      "\x00"

//#VCO freq 326.4MHz
    "spurconfig=0x3"                                                     "\x00"
    "\x00\x00";


#ifdef __cplusplus
} /* extern "C" */
#endif

#else /* ifndef INCLUDED_NVRAM_IMAGE_H_ */

#error Wi-Fi NVRAM image included twice

#endif /* ifndef INCLUDED_NVRAM_IMAGE_H_ */

