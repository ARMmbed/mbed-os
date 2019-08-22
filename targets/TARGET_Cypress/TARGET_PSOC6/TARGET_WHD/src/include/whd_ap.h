/*
 * Copyright 2019 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
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

/** @file
 *  Provides prototypes / declarations for APSTA functionality
 */
#ifndef INCLUDED_WHD_AP_H
#define INCLUDED_WHD_AP_H

#include "whd.h"
#include "cyabs_rtos.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
*                      Macros
******************************************************/
#define AMPDU_RX_FACTOR_8K          0     /* max receive AMPDU length is 8kb */
#define AMPDU_RX_FACTOR_16K         1     /* max receive AMPDU length is 16kb */
#define AMPDU_RX_FACTOR_32K         2     /* max receive AMPDU length is 32kb */
#define AMPDU_RX_FACTOR_64K         3     /* max receive AMPDU length is 64kb */
#define AMPDU_RX_FACTOR_INVALID  0xff     /* invalid rx factor; ignore */
#define AMPDU_MPDU_AUTO             (-1)  /* Auto number of mpdu in ampdu */


#define BANDWIDTH_20MHZ            (20)  /* 802.11n, 802.11ac 20Mhz Bandwidth */
#define BANDWIDTH_40MHZ            (40)  /* 802.11n, 802.11ac 40Mhz Bandwidth */
#define BANDWIDTH_80MHZ            (80)  /* 802.11ac 80Mhz Bandwidth          */

#define WHD_WIFI_CONFIG_AP_MAX_ASSOC 5

#define CHECK_IOCTL_BUFFER_WITH_SEMAPHORE(buff, \
                                          sema)  if (buff == \
                                                     NULL){ WPRINT_WHD_ERROR( ("Buffer alloc failed in %s at %d \n", \
                                                                               __func__, __LINE__) ); \
                                                            whd_assert("Buffer alloc failed\n", 0 == 1); \
                                                            (void)cy_rtos_deinit_semaphore(sema); \
                                                            return WHD_BUFFER_ALLOC_FAIL; }

#define CHECK_RETURN_WITH_SEMAPHORE(expr, sema)  { whd_result_t check_res = (expr); if (check_res != WHD_SUCCESS) \
                                                   { WPRINT_WHD_ERROR( ("Command failed in %s at %d \n", __func__, \
                                                                        __LINE__) ); \
                                                     whd_assert("Command failed\n", 0 == 1); \
                                                     (void)cy_rtos_deinit_semaphore(sema); \
                                                     return check_res; } }

/******************************************************
*             Structures
******************************************************/
typedef struct whd_ap_int_info
{
    whd_bool_t ap_is_up;
    whd_bool_t is_waiting_event;
    cy_semaphore_t whd_wifi_sleep_flag;

} whd_ap_int_info_t;

/******************************************************
*             Function prototypes
******************************************************/
extern whd_result_t whd_wifi_set_block_ack_window_size_common(whd_interface_t interface, uint16_t ap_win_size,
                                                              uint16_t sta_win_size);
extern whd_result_t whd_wifi_set_ampdu_parameters_common(whd_interface_t interface, uint8_t ba_window_size,
                                                         int8_t ampdu_mpdu, uint8_t rx_factor);
extern void whd_wifi_set_ap_is_up(whd_driver_t whd_driver, whd_bool_t new_state);
extern whd_bool_t whd_wifi_get_ap_is_up(whd_driver_t whd_driver);
void whd_ap_info_init(whd_driver_t whd_driver);
whd_result_t whd_wifi_set_block_ack_window_size(whd_interface_t ifp);


/** Set the AMPDU parameters for both Soft AP and STA
 *
 * Sets various AMPDU parameters for Soft AP and STA to ensure that the number of buffers dedicated to AMPDUs does
 * not exceed the resources of the chip. Both Soft AP and STA interfaces must be down.
 *
 * @return  WHD_SUCCESS : if the AMPDU parameters were successfully set
 *          WHD_ERROR   : if the AMPDU parameters were not successfully set
 */
extern whd_result_t whd_wifi_set_ampdu_parameters(whd_interface_t ifp);

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif /* ifndef INCLUDED_WHD_AP_H */

