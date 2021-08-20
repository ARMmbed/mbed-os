/*
 * Copyright 2021, Cypress Semiconductor Corporation (an Infineon company)
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
 *  The whd Thread allows thread safe access to the whd hardware bus
 *  This is an whd internal file and should not be used by functions outside whd.
 *
 *  This file provides prototypes for functions which allow multiple threads to use the whd hardware bus (SDIO or SPI)
 *  This is achieved by having a single thread (the "whd Thread") which queues messages to be sent, sending
 *  them sequentially, as well as receiving messages as they arrive.
 *
 *  Messages to be sent come from the @ref whd_sdpcm_send_common function in whd_sdpcm.c .  The messages already
 *  contain SDPCM headers, but not any bus headers (GSPI), and are passed via a queue
 *  This function can be called from any thread.
 *
 *  Messages are received by way of a callback supplied by in whd_sdpcm.c - whd_sdpcm_process_rx_packet
 *  Received messages are delivered in the context of the whd Thread, so the callback function needs to avoid blocking.
 *
 */
#include "cyabs_rtos.h"
#include "whd.h"

#ifndef INCLUDED_WHD_THREAD_H_
#define INCLUDED_WHD_THREAD_H_

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*             Constants
******************************************************/
#define WHD_THREAD_RX_BOUND           (20)
#define WHD_MAX_BUS_FAIL              (10)

typedef struct whd_thread_info
{

    volatile whd_bool_t thread_quit_flag;
    volatile whd_bool_t whd_inited;
    cy_thread_t whd_thread;
    cy_semaphore_t transceive_semaphore;
    volatile whd_bool_t bus_interrupt;
    void *thread_stack_start;
    uint32_t thread_stack_size;
    cy_thread_priority_t thread_priority;

} whd_thread_info_t;

void whd_thread_info_init(whd_driver_t whd_driver, whd_init_config_t *whd_init_config);

/** Initialises the whd Thread
 *
 * Initialises the whd thread, and its flags/semaphores,
 * then starts it running
 *
 * @return    whd result code
 */
extern whd_result_t whd_thread_init(whd_driver_t whd_driver);


/** Terminates the whd Thread
 *
 * Sets a flag then wakes the whd Thread to force it to terminate.
 *
 */
extern void whd_thread_quit(whd_driver_t whd_driver);


extern void whd_thread_notify(whd_driver_t whd_driver);
extern void whd_thread_notify_irq(whd_driver_t whd_driver);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_WHD_THREAD_H_ */

