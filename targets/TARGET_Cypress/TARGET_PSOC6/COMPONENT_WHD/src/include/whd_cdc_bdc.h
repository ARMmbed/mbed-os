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

#ifndef INCLUDED_WHD_CDC_BDC_H
#define INCLUDED_WHD_CDC_BDC_H

#include "whd.h"
#include "cyabs_rtos.h"
#include "whd_events_int.h"
#include "whd_types_int.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*             Constants
******************************************************/

/* CDC flag definition taken from bcmcdc.h */
#define CDCF_IOC_SET                (0x02)      /** 0=get, 1=set cmd */
#define WHD_EVENT_HANDLER_LIST_SIZE    (5)      /** Maximum number of simultaneously registered event handlers */
#define WHD_SDALIGN 32

/* CDC flag definitions taken from bcmcdc.h */
#define CDCF_IOC_ERROR              (0x01)      /** 0=success, 1=ioctl cmd failed */
#define CDCF_IOC_IF_MASK          (0xF000)      /** I/F index */
#define CDCF_IOC_IF_SHIFT             (12)      /** # of bits of shift for I/F Mask */
#define CDCF_IOC_ID_MASK      (0xFFFF0000)      /** used to uniquely id an ioctl req/resp pairing */
#define CDCF_IOC_ID_SHIFT             (16)      /** # of bits of shift for ID Mask */

#define DATA_AFTER_HEADER(x)   ( (void *)(&x[1]) )

#define BDC_HEADER_LEN                 (4)

/******************************************************
*                 Enumerations
******************************************************/

typedef enum sdpcm_command_type_enum
{
    CDC_GET = 0x00,
    CDC_SET = CDCF_IOC_SET
} cdc_command_type_t;

/******************************************************
*               Structures
******************************************************/
#pragma pack(1)
typedef struct
{
    uint32_t cmd;    /* ioctl command value */
    uint32_t len;    /* lower 16: output buflen; upper 16: input buflen (excludes header) */
    uint32_t flags;  /* flag defns given in bcmcdc.h */
    uint32_t status; /* status code returned from the device */
} cdc_header_t;

typedef struct
{
    uint8_t flags;      /* Flags */
    uint8_t priority;   /* 802.1d Priority (low 3 bits) */
    uint8_t flags2;
    uint8_t data_offset; /* Offset from end of BDC header to packet data, in 4-uint8_t words.
                          * Leaves room for optional headers.*/
} bdc_header_t;

typedef struct
{
    whd_mac_t destination_address;
    whd_mac_t source_address;
    uint16_t ethertype;
} ethernet_header_t;

#pragma pack()

/** Event list element structure
 *
 * events : A pointer to a whd_event_num_t array that is terminated with a WLC_E_NONE event
 * handler: A pointer to the whd_event_handler_t function that will receive the event
 * handler_user_data : User provided data that will be passed to the handler when a matching event occurs
 */
typedef struct
{
    whd_bool_t event_set;
    whd_event_num_t events[WHD_MAX_EVENT_SUBSCRIPTION];
    whd_event_handler_t handler;
    void *handler_user_data;
    uint8_t ifidx;
} event_list_elem_t;

/** @endcond */

typedef struct whd_cdc_info
{
    /* Event list variables */
    event_list_elem_t whd_event_list[WHD_EVENT_HANDLER_LIST_SIZE];
    cy_semaphore_t event_list_mutex;

    /* IOCTL variables*/
    uint16_t requested_ioctl_id;
    cy_semaphore_t ioctl_mutex;
    whd_buffer_t ioctl_response;
    cy_semaphore_t ioctl_sleep;

} whd_cdc_bdc_info_t;

/******************************************************
*               Function Declarations
******************************************************/

whd_result_t whd_cdc_bdc_info_init(whd_driver_t whd_driver);
void         whd_cdc_bdc_info_deinit(whd_driver_t whd_driver);

whd_result_t whd_cdc_send_iovar(whd_interface_t ifp, cdc_command_type_t type,
                                whd_buffer_t send_buffer_hnd,
                                whd_buffer_t *response_buffer_hnd);

whd_result_t whd_cdc_send_ioctl(whd_interface_t ifp, cdc_command_type_t type, uint32_t command,
                                whd_buffer_t send_buffer_hnd,
                                whd_buffer_t *response_buffer_hnd);

void *whd_cdc_get_iovar_buffer(whd_driver_t whd_driver,
                               whd_buffer_t *buffer,
                               uint16_t data_length,
                               const char *name);
void whd_network_send_ethernet_data(whd_interface_t ifp, whd_buffer_t buffer);

void *whd_cdc_get_ioctl_buffer(whd_driver_t whd_driver,
                               whd_buffer_t *buffer,
                               uint16_t data_length);

void whd_process_cdc(whd_driver_t whd_driver, whd_buffer_t buffer);

void whd_process_bdc(whd_driver_t whd_driver, whd_buffer_t buffer);

void whd_process_bdc_event(whd_driver_t whd_driver, whd_buffer_t buffer, uint16_t size);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_WHD_CDC_BDC_H */

