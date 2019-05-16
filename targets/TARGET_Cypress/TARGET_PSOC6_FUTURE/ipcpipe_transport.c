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

#include "ipcpipe_transport.h"
#include "cy_ipc_config.h"
#include "cy_ipc_pipe.h"

#ifdef __cplusplus
extern "C" {
#endif

static IpcPipeBuffer ipcpipe_buffer[2];

volatile uint32_t ipcpipe_current_buffer = 0;
volatile uint32_t ipcpipe_transfer_buffer = 0;

static uint32_t num_registered_clients = 0;

IpcPipeTxCompleteHandler *ipcpipe_xfer_complete_cb[CY_IPC_USRPIPE_CLIENT_CNT] = {NULL};


/** Buffer release callback function
 * Just releases currently transmitted buffer upon tx completion.
 * Used in the case the currently transmitted buffer was not the last one
 * in the queue, i.e. event doesn't complete the whole transmission.
 */
void ipcpipe_buffer_release(void)
{
    IPCPIPE_ASSERT(ipcpipe_buffer[ipcpipe_transfer_buffer].busy_flag == 1);
    ipcpipe_buffer[ipcpipe_transfer_buffer].busy_flag = 0;
}

/** Buffer release callback function
 * Releases currently transmitted buffer upon tx completion
 * and calls associated tx complete event handler.
 */
void ipcpipe_buffer_release_callback(void)
{
    uint32_t client_id = ipcpipe_buffer[ipcpipe_transfer_buffer].message.client_id;
    IpcPipeTxCompleteHandler *handler = ipcpipe_xfer_complete_cb[client_id];

    IPCPIPE_ASSERT(client_id < CY_IPC_USRPIPE_CLIENT_CNT);
    IPCPIPE_ASSERT(ipcpipe_buffer[ipcpipe_transfer_buffer].busy_flag == 1);
    ipcpipe_buffer[ipcpipe_transfer_buffer].busy_flag = 0;

    /* call back transfer complete function */
    if (handler) {
        (*handler)();
    }
}

/** Locks a buffer making it available for transmission.
 *
 * @param current_buffer    index of the buffer to be locked
 */
void ipcpipe_transfer_lock_buffer(uint32_t current_buffer)
{
    IPCPIPE_ASSERT(current_buffer < 2);
    IPCPIPE_ASSERT(ipcpipe_buffer[current_buffer].busy_flag == 0);
    /* make sure previous transfer has ended */
    while (ipcpipe_buffer[ipcpipe_transfer_buffer].busy_flag) {
        /* busy wait */
    }

    ipcpipe_buffer[current_buffer].busy_flag = 1;
    ipcpipe_transfer_buffer = current_buffer;
}

/** Find index of the next available buffer
 * This is a blocking call, it blocks until the buffer becomes available
 * if there is no free buffer at the moment.
 *
 * @return index of the buffer
 */
uint32_t ipcpipe_buffer_aquire(void)
{
    uint32_t buffer_index;
    /* check that we have a buffer available */
    while (ipcpipe_buffer[ipcpipe_current_buffer].busy_flag) {
        /* just wait here */
    }

    buffer_index = ipcpipe_current_buffer;
    ipcpipe_current_buffer = ipcpipe_current_buffer ? 0 : 1;
    return buffer_index;
}

/** Write header and data over IPC pipe.
 */
void ipcpipe_write_data(uint32_t client_id, uint8_t *header, uint32_t header_length, uint8_t *data, uint32_t data_length)
{
    uint32_t sent_idx = 0;
    cy_en_ipc_pipe_status_t  status;
    int avail = 0;

    IPCPIPE_ASSERT(client_id < CY_IPC_USRPIPE_CLIENT_CNT);

    while ((sent_idx < data_length) || (header_length > 0)) {
        uint32_t buffer_id = ipcpipe_buffer_aquire();
        IpcPipeBuffer *buffer = &ipcpipe_buffer[buffer_id];
        uint32_t data_idx = 0;

        /* copy over the header */
        buffer->message.header_length = header_length;
        while (header_length > 0) {
            buffer->message.header[data_idx++] = *header++;
            --header_length;
        }
        /* copy over the data */
        data_idx = 0;
        while ((sent_idx < data_length) && (data_idx < IPCPIPE_MAX_DATA_LENGTH)) {
            buffer->message.data[data_idx++] = data[sent_idx++];
        }
        buffer->message.data_length = data_idx;
        buffer->message.client_id = client_id;
        /* put into the pipe */
        ipcpipe_transfer_lock_buffer(buffer_id);
        do {
            avail = ipcpipe_buffer[ipcpipe_current_buffer].busy_flag == 0;

            status = Cy_IPC_Pipe_SendMessage(CY_IPC_EP_USRPIPE_DEST, /* destination EP */
                                             CY_IPC_EP_USRPIPE_ADDR, /* source EP */
                                             &buffer->message,
                                             avail? ipcpipe_buffer_release : ipcpipe_buffer_release_callback);
            if (status == CY_IPC_PIPE_ERROR_SEND_BUSY) {
                /* busy wait */
            }
        } while (status != CY_IPC_PIPE_SUCCESS);
    }

    /* execute transfer complete callback as appropriate */
    if (avail && ipcpipe_xfer_complete_cb[client_id]) {
        (*ipcpipe_xfer_complete_cb[client_id])();
    }
}

void ipcpipe_transport_start(uint32_t client_id, IpcPipeRxHandler *rx_handler, IpcPipeTxCompleteHandler *tx_handler)
{
    IPCPIPE_ASSERT(client_id < CY_IPC_USRPIPE_CLIENT_CNT);

    /* register/initialize required callbacks */
    Cy_IPC_Pipe_RegisterCallback(CY_IPC_EP_USRPIPE_ADDR, rx_handler,  client_id);
    Cy_IPC_Pipe_RegisterCallbackRel(CY_IPC_EP_USRPIPE_ADDR, ipcpipe_buffer_release);
    ipcpipe_xfer_complete_cb[client_id] = tx_handler;
    if (++num_registered_clients == 1) {
        ipcpipe_transport_enable();
    }
}

void ipcpipe_transport_stop(uint32_t client_id)
{
    Cy_IPC_Pipe_RegisterCallback(CY_IPC_EP_USRPIPE_ADDR, NULL,  client_id);
    Cy_IPC_Pipe_RegisterCallbackRel(CY_IPC_EP_USRPIPE_ADDR, NULL);
    ipcpipe_xfer_complete_cb[client_id] = NULL;
    if (--num_registered_clients == 0) {
        ipcpipe_transport_disable();
    }
}

void ipcpipe_transport_enable(void)
{
    Cy_IPC_Pipe_EndpointResume(CY_IPC_EP_USRPIPE_ADDR);
}


void ipcpipe_transport_disable(void)
{
    Cy_IPC_Pipe_EndpointPause(CY_IPC_EP_USRPIPE_ADDR);
}

#ifdef __cplusplus
}
#endif


/* [] END OF FILE */
