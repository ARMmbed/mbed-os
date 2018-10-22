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

/* IPC Pipe message data structure */
typedef struct
{
  uint32_t  client_id;
  uint32_t  header_length;
  uint32_t  data_length;
  uint8_t   header[IPCPIPE_MAX_HEADER_LENGTH];
  uint8_t   data[IPCPIPE_MAX_DATA_LENGTH];
} IpcPipeMessage;


typedef struct
{
    volatile uint8_t    busy_flag;
    IpcPipeMessage      message;
} IpcPipeBuffer;


typedef void (IpcPipeRxHandler)(uint32_t *message_ptr);
typedef void (IpcPipeTxCompleteHandler)(void);

#ifdef __cplusplus
extern "C" {
#endif

void ipcpipe_write_data(uint32_t client_id, uint8_t* header, uint32_t header_length, uint8_t *data, uint32_t data_length);
void ipcpipe_transport_start(uint32_t client_id, IpcPipeRxHandler *rx_handler, IpcPipeTxCompleteHandler *tx_handler);
void ipcpipe_transport_stop(uint32_t client_id);
void ipcpipe_transport_enable(void);
void ipcpipe_transport_disable(void);

#ifdef __cplusplus
}
#endif


#endif /* IPCPIPE_H4_TRANSPORT_H */

/* [] END OF FILE */
