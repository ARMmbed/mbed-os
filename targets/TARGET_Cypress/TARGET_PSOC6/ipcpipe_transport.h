/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#ifndef IPCPIPE_TRANSPORT_H
#define IPCPIPE_TRANSPORT_H

#include <stdint.h>

#if defined(__MBED__)
#define IPCPIPE_ASSERT   MBED_ASSERT
#include "mbed_assert.h"
#else
#include "project.h"
#define IPCPIPE_ASSERT   CY_ASSERT
#endif


#define IPCPIPE_MAX_DATA_LENGTH     256
#define IPCPIPE_MAX_HEADER_LENGTH   4

/* Client IDs */
#define IPCPIPE_CLIENT_H4           0

/** IPC Pipe message data structure
 */
typedef struct {
    uint32_t  client_id;                          ///< ID of the client using this API
    uint32_t  header_length;                      ///< length of the message header in bytes
    uint32_t  data_length;                        ///< length of the message data field
    uint8_t   header[IPCPIPE_MAX_HEADER_LENGTH];  ///< message header
    uint8_t   data[IPCPIPE_MAX_DATA_LENGTH];      ///< message data
} IpcPipeMessage;


/** IPC Pipe message buffer
 * Used to transfer a message to other MCU
 */
typedef struct {
    volatile uint8_t    busy_flag;              ///< indicates whether the transfer is in progress
    IpcPipeMessage      message;                ///< the message itself
} IpcPipeBuffer;

/** Type of rx (buffer received) event handler function
 */
typedef void (IpcPipeRxHandler)(uint32_t *message_ptr);

/** Type of tx complete (buffer sent out) event handler function
 */
typedef void (IpcPipeTxCompleteHandler)(void);

#ifdef __cplusplus
extern "C" {
#endif

/** Send a data block over IPC pipe
 *
 * @param client_id         ID of the client using this service
 * @param header            pointer to the message header to be sent
 * @param header_length     length of the header
 * @param data              pointer to the message data to be sent
 * @param  data_length      length of the data
 */
void ipcpipe_write_data(uint32_t client_id, uint8_t* header, uint32_t header_length, uint8_t *data, uint32_t data_length);

/** Initialize and start IPC pipe transport service
 *
 * @param client_id         ID of the client using this service
 * @param rx_handler        receive event handler
 * @param tx_handler        transmit complete event handler
 */
void ipcpipe_transport_start(uint32_t client_id, IpcPipeRxHandler *rx_handler, IpcPipeTxCompleteHandler *tx_handler);

/** Stop IPC pipe transport service
 *
 * @param client_id         ID of the client using this service
 */
void ipcpipe_transport_stop(uint32_t client_id);


/** Enable IPC pipe transport service
 */
void ipcpipe_transport_enable(void);

/** Disable IPC pipe transport service
 */
void ipcpipe_transport_disable(void);

#ifdef __cplusplus
}
#endif


#endif /* IPCPIPE_H4_TRANSPORT_H */

/* [] END OF FILE */
