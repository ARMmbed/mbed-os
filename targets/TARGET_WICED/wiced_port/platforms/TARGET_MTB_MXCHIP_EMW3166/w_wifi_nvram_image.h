/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
 */

#ifndef INCLUDED_NVRAM_IMAGE_H_
#define INCLUDED_NVRAM_IMAGE_H_

#include <string.h>
#include <stdint.h>
#include "../generated_mac_address.txt"

#ifdef __cplusplus
extern "C" {
#endif

static const char wifi_nvram_image[] =
        "manfid=0x2d0"                                                       "\x00"
        "prodid=0x492"                                                       "\x00"
        "vendid=0x14e4"                                                      "\x00"
        "devid=0x4343"                                                       "\x00"
        "boardtype=0x05a0"                                                   "\x00"
        "boardrev=0x1301"                                                    "\x00"
        "boardnum=777"                                                       "\x00"
        "xtalfreq=26000"                                                     "\x00"
        "boardflags=0xa00"                                                   "\x00"
        "sromrev=3"                                                          "\x00"
        "wl0id=0x431b"                                                       "\x00"
        NVRAM_GENERATED_MAC_ADDRESS                                          "\x00"
        "aa2g=3"                                                             "\x00"
        "ag0=2"                                                              "\x00"
        "maxp2ga0=68"                                                        "\x00"
        "ofdm2gpo=0x44444444"                                                "\x00"
        "mcs2gpo0=0x3333"                                                    "\x00"
        "mcs2gpo1=0x6333"                                                    "\x00"
        "pa0maxpwr=80"                                                       "\x00"
        "pa0b0=0x133E"                                                       "\x00"
        "pa0b1=0xFDBA"                                                       "\x00"
        "pa0b2=0xFF53"                                                       "\x00"
        "pa0itssit=62"                                                       "\x00"
        "pa1itssit=62"                                                       "\x00"
        "temp_based_dutycy_en=1"                                             "\x00"
        "tx_duty_cycle_ofdm=100"                                             "\x00"
        "tx_duty_cycle_cck=100"                                              "\x00"
        "tx_ofdm_temp_0=115"                                                 "\x00"
        "tx_cck_temp_0=115"                                                  "\x00"
        "tx_ofdm_dutycy_0=40"                                                "\x00"
        "tx_cck_dutycy_0=40"                                                 "\x00"
        "tx_ofdm_temp_1=255"                                                 "\x00"
        "tx_cck_temp_1=255"                                                  "\x00"
        "tx_ofdm_dutycy_1=40"                                                "\x00"
        "tx_cck_dutycy_1=40"                                                 "\x00"
        "tx_tone_power_index=40"                                             "\x00"
        "tx_tone_power_index.fab.3=48"                                       "\x00"
        "cckPwrOffset=0"                                                     "\x00"
        "ccode=0"                                                            "\x00"
        "rssismf2g=0xa"                                                      "\x00"
        "rssismc2g=0x3"                                                      "\x00"
        "rssisav2g=0x7"                                                      "\x00"
        "triso2g=0"                                                          "\x00"
        "noise_cal_enable_2g=0"                                              "\x00"
        "noise_cal_po_2g=0"                                                  "\x00"
        "noise_cal_po_2g.fab.3=-2"                                           "\x00"
        "swctrlmap_2g=0x050c050c,0x030a030a,0x030a030a,0x0,0x1ff"            "\x00"
        "temp_add=29767"                                                     "\x00"
        "temp_mult=425"                                                      "\x00"
        "temp_q=10"                                                          "\x00"
        "initxidx2g=45"                                                      "\x00"
        "tssitime=1"                                                         "\x00"
        "rfreg033=0x19"                                                      "\x00"
        "rfreg033_cck=0x1f"                                                  "\x00"
        "cckPwrIdxCorr=-8"                                                   "\x00"
        "spuravoid_enable2g=1"                                               "\x00"
        "edonthd=-70"                                                        "\x00"
        "edoffthd=-76"                                                       "\x00"
        "\x00\x00";

#else /* ifndef INCLUDED_NVRAM_IMAGE_H_ */

#error Wi-Fi NVRAM image included twice

#endif /* ifndef INCLUDED_NVRAM_IMAGE_H_ */

#ifdef __cplusplus
} /* extern "C" */
#endif
