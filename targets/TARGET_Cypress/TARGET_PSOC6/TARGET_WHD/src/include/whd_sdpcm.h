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
 *  Provides whd with function prototypes for IOCTL commands,
 *  and for communicating with the SDPCM module
 *
 */

#ifndef INCLUDED_WHD_SDPCM_H
#define INCLUDED_WHD_SDPCM_H

#include "whd.h"
#include "whd_events_int.h"
#include "cyabs_rtos.h"
#include "whd_network_types.h"
#include "whd_types_int.h"
#include "whd_cdc_bdc.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*             Constants
******************************************************/
typedef enum
{
    DATA_HEADER       = 2,
    ASYNCEVENT_HEADER = 1,
    CONTROL_HEADER    = 0
} sdpcm_header_type_t;

/******************************************************
*             Macros
******************************************************/

#define BUS_HEADER_LEN  (12)
#define IOCTL_OFFSET (sizeof(whd_buffer_header_t) + 12 + 16)

/******************************************************
*             Structures
******************************************************/
typedef struct whd_sdpcm_info
{
    /* Bus data credit variables */
    uint8_t packet_transmit_sequence_number;
    uint8_t last_bus_data_credit;
    uint8_t credit_diff;
    uint8_t largest_credit_diff;

    /* Packet send queue variables */
    cy_semaphore_t send_queue_mutex;
    whd_buffer_t send_queue_head;
    whd_buffer_t send_queue_tail;

} whd_sdpcm_info_t;

typedef struct
{
    whd_buffer_header_t buffer_header;
    uint8_t bus_header[BUS_HEADER_LEN];
} bus_common_header_t;

#pragma pack(1)
typedef struct
{
    bus_common_header_t common;
    cdc_header_t cdc_header;
} control_header_t;

typedef struct
{
    bus_common_header_t common;
    uint8_t _padding[2];
    bdc_header_t bdc_header;
} data_header_t;
#pragma pack()

/******************************************************
*             Function declarations
******************************************************/

extern void whd_sdpcm_process_rx_packet(whd_driver_t whd_driver, whd_buffer_t buffer);
extern whd_result_t whd_sdpcm_init(whd_driver_t whd_driver);
extern void whd_sdpcm_quit(whd_driver_t whd_driver);
extern void whd_sdpcm_bus_vars_init(whd_driver_t whd_driver);
extern void whd_sdpcm_quit(whd_driver_t whd_driver);
extern whd_bool_t whd_sdpcm_has_tx_packet(whd_driver_t whd_driver);

extern whd_result_t whd_sdpcm_get_packet_to_send(whd_driver_t whd_driver, whd_buffer_t *buffer);
extern void whd_sdpcm_update_credit(whd_driver_t whd_driver, uint8_t *data);
extern uint8_t whd_sdpcm_get_available_credits(whd_driver_t whd_driver);
extern void whd_update_host_interface_to_bss_index_mapping(whd_driver_t whd_driver, whd_interface_t interface,
                                                           uint32_t bssid_index);

extern void whd_send_to_bus(whd_driver_t whd_driver, whd_buffer_t buffer,
                            sdpcm_header_type_t header_type);

/******************************************************
*             Global variables
******************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_WHD_SDPCM_H */

