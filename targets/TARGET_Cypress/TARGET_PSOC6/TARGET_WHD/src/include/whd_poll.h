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
 *  Header for using WHD with no RTOS or network stack
 *
 *  It is possible to use these WHD without any operating system. To do this,
 *  the user application is required to periodically use the functions in this
 *  file to allow WHD to send and receive data across the SPI/SDIO bus.
 *
 */

#include "whd.h"

#ifndef INCLUDED_WHD_POLL_H
#define INCLUDED_WHD_POLL_H

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*             Function declarations
******************************************************/


/** Sends the first queued packet
 *
 * Checks the queue to determine if there is any packets waiting
 * to be sent. If there are, then it sends the first one.
 *
 * This function is normally used by the WHD Thread, but can be
 * called periodically by systems which have no RTOS to ensure
 * packets get sent.
 *
 * @return    1 : packet was sent
 *            0 : no packet sent
 */
extern int8_t whd_thread_send_one_packet(whd_driver_t whd_driver);


/** Receives a packet if one is waiting
 *
 * Checks the wifi chip fifo to determine if there is any packets waiting
 * to be received. If there are, then it receives the first one, and calls
 * the callback @ref whd_sdpcm_process_rx_packet (in whd_sdpcm.c).
 *
 * This function is normally used by the WHD Thread, but can be
 * called periodically by systems which have no RTOS to ensure
 * packets get received properly.
 *
 * @return    1 : packet was received
 *            0 : no packet waiting
 */
extern int8_t whd_thread_receive_one_packet(whd_driver_t whd_driver);


/** Sends and Receives all waiting packets
 *
 * Repeatedly calls whd_thread_send_one_packet and whd_thread_receive_one_packet
 * to send and receive packets, until there are no more packets waiting to
 * be transferred.
 *
 * This function is normally used by the WHD Thread, but can be
 * called periodically by systems which have no RTOS to ensure
 * packets get send and received properly.
 *
 * @return    1 : packet was sent or received
 *            0 : no packet was sent or received
 */
extern int8_t whd_thread_poll_all(whd_driver_t whd_driver);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* ifndef INCLUDED_WHD_POLL_H */

