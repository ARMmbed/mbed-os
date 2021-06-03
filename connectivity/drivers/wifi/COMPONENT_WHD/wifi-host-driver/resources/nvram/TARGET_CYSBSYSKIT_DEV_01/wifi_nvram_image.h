/*
 * Copyright 2021, Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software") is owned by Cypress Semiconductor Corporation
 * or one of its affiliates ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products.  Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

#ifndef INCLUDED_NVRAM_IMAGE_H_
#define INCLUDED_NVRAM_IMAGE_H_

#include <string.h>
#include <stdint.h>
#include "generated_mac_address.txt"

/*
 * CYW43012 Rev C0 SSBU
 * Rev History
 *  1. 06/26/2019 copied from cy8ckit-062S2-43012, updated per file from Rinku Matthew
 *  2. 11/19/2020 updated Nvram file given by Kailash(SUBSFW-923) 
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Character array of NVRAM image generated from
 * cyw943012cysub_rp01_P401_727513.txt
 */

static const char wifi_nvram_image[] =                         
"NVRAMRev=$Rev: 727605 $"                 "\x00"                               
"sromrev=11"                              "\x00"                               
"etmode=0x11"                             "\x00"                               
"cckdigfilttype=4"                        "\x00"                               
"bphyscale=0x20"                          "\x00"                               
"boardflags3=0x40000101"                  "\x00"                               
"vendid=0x14e4"                           "\x00"                               
"devid=0xA804"                            "\x00"                               
"manfid=0x2d0"                            "\x00"                               
"prodid=0x0879"                           "\x00"                               
NVRAM_GENERATED_MAC_ADDRESS               "\x00"                              
"nocrc=1"                                 "\x00"                              
"boardtype=0x0879"                        "\x00"                               
"boardrev=0x1401"                         "\x00"                               
"lpflags=0x00000000"                      "\x00"                               
"xtalfreq=37400"                          "\x00"                               
"boardflags2=0xc0800000"                  "\x00"                               
"boardflags=0x00400001"                   "\x00"                              
"extpagain2g=2"                           "\x00"                             
"extpagain5g=2"                           "\x00"                             
"ccode=0"                                 "\x00"                               
"regrev=0"                                "\x00"                               
"antswitch=0"                             "\x00"                             
"rxgains2gelnagaina0=0"                   "\x00"                               
"rxgains2gtrisoa0=15"                     "\x00"                               
"rxgains2gtrelnabypa0=0"                  "\x00"                               
"rxgains5gelnagaina0=0"                   "\x00"                               
"rxgains5gtrisoa0=9"                      "\x00"                               
"rxgains5gtrelnabypa0=0"                  "\x00"                               
"pdgain5g=0"                              "\x00"                               
"pdgain2g=0"                              "\x00"                               
"tworangetssi2g=0"                        "\x00"                               
"tworangetssi5g=0"                        "\x00"                               
"rxchain=1"                               "\x00"                               
"txchain=1"                               "\x00"                               
"aa2g=1"                                  "\x00"                               
"aa5g=1"                                  "\x00"                               
"tssipos5g=0"                             "\x00"                               
"tssipos2g=0"                             "\x00"                               
"tssisleep_en=0x5"                        "\x00"                               
"femctrl=17"                              "\x00"                               
"subband5gver=4"                          "\x00"                               
"pa2ga0=-136,5831,-672"                   "\x00"                                
"pa5ga0=-64,6470,-718,-65,6464,-717,-68,6449,-724,-58,6515,-726"  "\x00"     
"cckpwroffset0=2"                         "\x00"                               
"pdoffset40ma0=0"                         "\x00"                                
"pdoffset80ma0=0"                         "\x00"                                
"lowpowerrange2g=0"                       "\x00"                                
"lowpowerrange5g=0"                       "\x00"                                
"rssicorrnorm_c0=-10,0"                   "\x00"                                
"rssicorrnorm5g_c0=-10,0,0,-15,0,0,-15,0,0,-15,0,0"                "\x00"                   
"ed_thresh2g=-63"                         "\x00"                               
"ed_thresh5g=-63"                         "\x00"                               
"swctrlmap_2g=0x00000000,0x40404040, 0x40404040,0x000000,0x3e7"    "\x00" 
"swctrlmapext_2g=0x02020202,0x00000000, 0x00000000,0x000000,0x003" "\x00" 
"swctrlmap_5g=0x00000000,0x00000000,0x00000000,0x000000,0x3a7"     "\x00" 
"swctrlmapext_5g=0x00000000,0x01010101, 0x01010101,0x000000,0x001" "\x00"     
"ulpnap=0"                                "\x00"                               
"ulpadc=1"                                "\x00"                              
"ssagc_en=0"                              "\x00"                               
"ds1_nap=0"                               "\x00"                               
"epacal2g=0"                              "\x00"                               
"epacal5g=0"                              "\x00"                               
"epacal2g_mask=0x3fff"                    "\x00"                               
"maxp2ga0=82"                             "\x00"                               
"ofdmlrbw202gpo=0x0077"                   "\x00"                               
"dot11agofdmhrbw202gpo=0x8888"            "\x00"                               
"mcsbw202gpo=0x99999999"                  "\x00"                               
"mac_clkgating=1"                         "\x00"                               

"maxp5ga0=82,82,82,82"                    "\x00"                               
"mcsbw205glpo=0xffc97777"                 "\x00"                               
"mcsbw205gmpo=0xffc97777"                 "\x00"                               
"mcsbw205ghpo=0xffc97777"                 "\x00"                               


"txwbpapden=1"                            "\x00"
"femctrlwar=0"                            "\x00"
"use5gpllfor2g=1"                         "\x00"
"wb_rxattn=0x0303"                        "\x00"
"wb_txattn=0x0202"                        "\x00"
"wb_papdcalidx=0x1015"                    "\x00"
"wb_papdcalidx_5g=0x00101010"             "\x00"
"wb_eps_offset=0x01bf01af"                "\x00"
"wb_eps_offset_5g=0x01bf01bf"             "\x00"
"wb_bbmult=0x67"                          "\x00"

"wb_bbmult_5g=0x004f5052"                 "\x00"

"wb_calref_db=0x1926"                     "\x00"
"wb_tia_gain_mode=0x0606"                 "\x00"
"wb_txbuf_offset=0x1e1e"                  "\x00"
"wb_frac_del=0x78ae"                      "\x00"
"wb_g_frac_bits=0xaa"                     "\x00"

"nb_rxattn=0x0403"                        "\x00"
"nb_txattn=0x0402"                        "\x00"
"nb_papdcalidx= 0x1405"                   "\x00"
"nb_eps_offset= 0x01d701ca"               "\x00"
"nb_bbmult= 0x5A50"                       "\x00"
"nb_tia_gain_mode=0x0006"                 "\x00"
"AvVmid_c0=6,104,7,80,7,80,7,80,7,80"     "\x00"


"lpo_select=4"                            "\x00"
"muxenab=0x01"                            "\x00"
"csml=0x10"                               "\x00"
"pt5db_gaintbl=0"                         "\x00"

"papdcck=0"                               "\x00"
"phycal_tempdelta=15"
"paprrmcsgamma2g=450,500,550,600,675,950,950,950,950,950,950,950" "\x00"
"paprrmcsgamma5g20=550,550,550,600,800,950,1100,1100,1100,1100,1100,1100" "\x00"
"\x00\x00";

#ifdef __cplusplus
} /*extern "C" */
#endif

#else /* ifndef INCLUDED_NVRAM_IMAGE_H_ */
#error Wi-Fi NVRAM image included twice
#endif /* ifndef INCLUDED_NVRAM_IMAGE_H_ */

