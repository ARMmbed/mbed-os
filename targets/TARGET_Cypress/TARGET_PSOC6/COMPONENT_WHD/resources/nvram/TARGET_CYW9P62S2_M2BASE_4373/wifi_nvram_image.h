/*
 * Copyright (c) 2019, Cypress Semiconductor Corporation, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License")
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
 * Generated from brcmfmac4373-sdio.txt
 */

static const char wifi_nvram_image[] =
        "NVRAMRev=$Rev: 722807 $"                                            "\x00"
        "sromrev=11"                                                         "\x00"
        "vendid=0x14e4"                                                      "\x00"
        "devid=0x4418"                                                       "\x00"
        "manfid=0x2d0"                                                       "\x00"
        "prodid=0x070f"                                                      "\x00"
        NVRAM_GENERATED_MAC_ADDRESS                                          "\x00"
        "nocrc=1"                                                            "\x00"
        "boardtype=0x83d"                                                    "\x00"
        "boardrev=0x1353"                                                    "\x00"
        "xtalfreq=37400"                                                     "\x00"
        "boardflags=0x00000001"                                              "\x00"
        "boardflags2=0x00000000"                                             "\x00"
        "boardflags3=0x48202100"                                             "\x00"
        "ext_lpo_margin_frac=0"                                              "\x00"
        "tx_duty_cycle_thermal=40"                                           "\x00"
        "temp_threshold=105"                                                 "\x00"
        "temp_delta=30"                                                      "\x00"
        "phycal_tempdelta=15"                                                "\x00"
        "rxgains2gelnagaina0=0"                                              "\x00"
        "rxgains2gtrisoa0=0"                                                 "\x00"
        "rxgains2gtrelnabypa0=0"                                             "\x00"
        "rxgains5gelnagaina0=0"                                              "\x00"
        "rxgains5gtrisoa0=0"                                                 "\x00"
        "rxgains5gtrelnabypa0=0"                                             "\x00"
        "pdgain5g=3"                                                         "\x00"
        "pdgain2g=3"                                                         "\x00"
        "antswitch=0x6"                                                      "\x00"
        "rxchain=1"                                                          "\x00"
        "txchain=1"                                                          "\x00"
        "aa2g=3"                                                             "\x00"
        "aa5g=3"                                                             "\x00"
        "tssipos5g=1"                                                        "\x00"
        "tssipos2g=1"                                                        "\x00"
        "femctrl=0"                                                          "\x00"
        "pa2ga0=-176,5552,-658"                                              "\x00"
        "pa5ga0=-153,5528,-664,-153,5528,-664,-155,5563,-666,-167,5492,-668" "\x00"
        "pdoffsetcckma0=0xf"                                                 "\x00"
        "pdoffset2g40ma0=0xc"                                                "\x00"
        "pdoffset40ma0=0xffff"                                               "\x00"
        "pdoffset80ma0=0xeeee"                                               "\x00"
        "extpagain5g=2"                                                      "\x00"
        "extpagain2g=2"                                                      "\x00"
        "AvVmid_c0=1,130,0,160,0,160,0,160,0,160"                            "\x00"
        "maxp2ga0=76"                                                        "\x00"
        "maxp5ga0=70,70,70,70"                                               "\x00"
        "cckbw202gpo=0x0000"                                                 "\x00"
        "dot11agofdmhrbw202gpo=0x2222"                                       "\x00"
        "ofdmlrbw202gpo=0x3222"                                              "\x00"
        "mcsbw202gpo=0x88764422"                                             "\x00"
        "mcsbw402gpo=0x88764422"                                             "\x00"
        "mcsbw205glpo=0x87664422"                                            "\x00"
        "mcsbw205gmpo=0x87664422"                                            "\x00"
        "mcsbw205ghpo=0x87664422"                                            "\x00"
        "mcsbw405glpo=0x98664422"                                            "\x00"
        "mcsbw405gmpo=0x98664422"                                            "\x00"
        "mcsbw405ghpo=0x98664422"                                            "\x00"
        "mcsbw805glpo=0xEA886622"                                            "\x00"
        "mcsbw805gmpo=0xEA886622"                                            "\x00"
        "mcsbw805ghpo=0xEA886622"                                            "\x00"
        "swctrlmap_2g=0x00001131,0x00001131,0x00001131,0x313131,0x1ff"       "\x00"
        "swctrlmap_5g=0x00201131,0x40405171,0x40405171,0x313131,0x1ff"       "\x00"
        "swctrlmapext_2g=0x00000000,0x00000000,0x00000000,0x000000,0x000"    "\x00"
        "swctrlmapext_5g=0x00000000,0x00000000,0x00000000,0x000000,0x000"    "\x00"
        "fem_table_init_val=0x1131,0x1131"                                   "\x00"
        "nb_papdcalidx=0x280f"                                               "\x00"
        "nb_txattn=0x0303"                                                   "\x00"
        "nb_rxattn=0x0303"                                                   "\x00"
        "nb_bbmult=0x3948"                                                   "\x00"
        "nb_eps_offset=0x01e601ea"                                           "\x00"
        "rssi_delta_2g_c0=-2,-2,-2,-2"                                       "\x00"
        "rssi_delta_5gl_c0=-2,-2,-3,-3,-1,-1"                                "\x00"
        "rssi_delta_5gml_c0=-2,-2,-3,-3,-1,-1"                               "\x00"
        "rssi_delta_5gmu_c0=0,0,-1,-1,0,0"                                   "\x00"
        "rssi_delta_5gh_c0=-1,-1,-2,-2,0,0"                                  "\x00"
        "ATErcalmode=0"                                                      "\x00"
        "swdiv_en=1"                                                         "\x00"
        "swdiv_gpio=0"                                                       "\x00"
        "swdiv_swctrl_en=2"                                                  "\x00"
        "swdiv_swctrl_ant0=0"                                                "\x00"
        "swdiv_swctrl_ant1=1"                                                "\x00"
        "swdiv_antmap2g_main=1"                                              "\x00"
        "swdiv_antmap5g_main=1"                                              "\x00"
        "swdiv_snrlim=290"                                                   "\x00"
        "swdiv_thresh=2000"                                                  "\x00"
        "swdiv_snrthresh=24"                                                 "\x00"
        "swdiv_timeon=10"                                                    "\x00"
        "swdiv_timeoff=1"                                                    "\x00"
        "swdiv_snr2g20=232"                                                  "\x00"
        "swdiv_snr2g40=257"                                                  "\x00"
        "swdiv_snr5g20=296"                                                  "\x00"
        "swdiv_snr5g40=312"                                                  "\x00"
        "swdiv_snr5g80=296"                                                  "\x00"
        "swdiv_ap_dead_check=1"                                              "\x00"
        "swdiv_ap_div=1"                                                     "\x00"
        "\x00\x00";
#ifdef __cplusplus
} /*extern "C" */
#endif

#else /* ifndef INCLUDED_NVRAM_IMAGE_H_ */

#error Wi-Fi NVRAM image included twice

#endif /* ifndef INCLUDED_NVRAM_IMAGE_H_ */
