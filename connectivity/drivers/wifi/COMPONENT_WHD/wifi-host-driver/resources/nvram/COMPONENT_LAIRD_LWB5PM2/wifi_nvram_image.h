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
 * Character array of NVRAM image (LAIRD Conntectivity's Sterling-LWB5+ with SDIO/UART (M.2) module. Part Number: 453-00048) 
 * Generated from brcmfmac4373-div-fcc_20200921.txt
 */

static const char wifi_nvram_image[] =
        "NVRAMRev=$Rev$"                                                     "\x00"
        "sromrev=11"                                                         "\x00"
        "vendid=0x14e4"                                                      "\x00"
        "devid=0x4418"                                                       "\x00"
        "manfid=0x2d0"                                                       "\x00"
        "prodid=0x070f"                                                      "\x00"
        NVRAM_GENERATED_MAC_ADDRESS                                          "\x00"
        "nocrc=1"                                                            "\x00"
        "boardtype=0x83d"                                                    "\x00"
        "boardrev=0x1306"                                                    "\x00"
        "xtalfreq=37400"                                                     "\x00"
        "boardflags=0x00000001"                                              "\x00"
        "boardflags2=0x00800000"                                             "\x00"
        "boardflags3=0x48202100"                                             "\x00"
        "tempthresh=105"                                                     "\x00"
        "temps_hysteresis=20"                                                "\x00"
        "temps_txduty_lowlimit=0"                                            "\x00"
        "temps_period=1"                                                     "\x00"
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
        "pa2ga0=-214,4762,-584"                                              "\x00"
        "pa5ga0=-153,5528,-664,-153,5528,-664,-155,5563,-666,-167,5492,-668" "\x00"
        "pdoffsetcckma0=0xf"                                                 "\x00"
        "pdoffset2g40ma0=0xc"                                                "\x00"
        "pdoffset40ma0=0xffff"                                               "\x00"
        "pdoffset80ma0=0xeeee"                                               "\x00"
        "extpagain5g=2"                                                      "\x00"
        "extpagain2g=2"                                                      "\x00"
        "AvVmid_c0=1,130,0,160,0,160,0,160,0,160"                            "\x00"
        "maxp2ga0=72"                                                        "\x00"
        "maxp5ga0=70,70,70,70"                                               "\x00"
        "cckbw202gpo=0x1111"                                                 "\x00"
        "dot11agofdmhrbw202gpo=0x4422"                                       "\x00"
        "ofdmlrbw202gpo=0x0022"                                              "\x00"
        "mcsbw202gpo=0xAA666666"                                             "\x00"
        "mcsbw402gpo=0xFFEEEEEE"                                             "\x00"
        "mcsbw205glpo=0xDD662222"                                            "\x00"
        "mcsbw205gmpo=0xDD662222"                                            "\x00"
        "mcsbw205ghpo=0xDD662222"                                            "\x00"
        "mcsbw405glpo=0xDDAAAAAA"                                            "\x00"
        "mcsbw405gmpo=0xDDAAAAAA"                                            "\x00"
        "mcsbw405ghpo=0xDDAAAAAA"                                            "\x00"
        "mcsbw805glpo=0xEECCCCCC"                                            "\x00"
        "mcsbw805gmpo=0xEECCCCCC"                                            "\x00"
        "mcsbw805ghpo=0xEECCCCCC"                                            "\x00"
        "swctrlmap_2g=0x00001131,0x00001131,0x00001131,0x313131,0x1ff"       "\x00"
        "swctrlmap_5g=0x00201131,0x40405171,0x00001131,0x313131,0x1ff"       "\x00"
        "swctrlmapext_2g=0x00000000,0x00000000,0x00000000,0x000000,0x000"    "\x00"
        "swctrlmapext_5g=0x00000000,0x00000000,0x00000000,0x000000,0x000"    "\x00"
        "fem_table_init_val=0x1131,0x1131"                                   "\x00"
        "nb_papdcalidx=0x280f"                                               "\x00"
        "nb_txattn=0x0303"                                                   "\x00"
        "nb_rxattn=0x0303"                                                   "\x00"
        "nb_bbmult=0x3948"                                                   "\x00"
        "nb_eps_offset=0x01e601ea"                                           "\x00"
        "papdmode=1"                                                         "\x00"
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
        "idac_main_mode=1"                                                   "\x00"
        "paldo3p3_voltage=4"                                                 "\x00"
        "fdss_level_2g=4"                                                    "\x00"
        "fdss_level_5g=4"                                                    "\x00"
        "fdss_interp_en=1"                                                   "\x00"
        "ccode=US"                                                           "\x00"
        "regrev=0"                                                           "\x00"
        "\x00\x00";
#ifdef __cplusplus
} /*extern "C" */
#endif

#else /* ifndef INCLUDED_NVRAM_IMAGE_H_ */

#error Wi-Fi NVRAM image included twice

#endif /* ifndef INCLUDED_NVRAM_IMAGE_H_ */
