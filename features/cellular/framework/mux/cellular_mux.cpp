/*
 * Copyright (c) , Arm Limited and affiliates.
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

#if MBED_CONF_CELLULAR_MUX_ENABLED

#include <errno.h>
#include "cellular_mux.h"
#include "mbed_retarget.h"

using namespace rtos;
using namespace events;

namespace mbed
{

#define FLAG_SEQUENCE_OCTET                 0xF9u     /* Flag field used in the basic option mode. */
#define ADDRESS_MUX_START_REQ_OCTET         0x03u     /* Address field of the start multiplexer request frame. */
#define ADDRESS_MUX_START_RESP_OCTET        0x03u     /* Address field value of the start multiplexer response frame. */

#define FCS_INPUT_LEN                       3u        /* Length of the input for FCS calculation in number of bytes. */
#define SABM_FRAME_LEN                      6u        /* Length of the SABM frame in number of bytes. */
#define UA_FRAME_LEN                        6u        /* Length of the UA frame in number of bytes. */
#define DM_FRAME_LEN                        6u        /* Length of the DM frame in number of bytes. */
#define UIH_FRAME_MIN_LEN                   6u        /* Minimum length of user frame. */

#define T1_TIMER_VALUE                      300u      /* T1 timer value. */
#define RETRANSMIT_COUNT                    3u        /* Retransmission count for the tx frames requiring a response. */

#define FRAME_TYPE_SABM                     0x2Fu     /* SABM frame type coding in the frame control field. */
#define FRAME_TYPE_UA                       0x63u     /* UA frame type coding in the frame control field. */
#define FRAME_TYPE_DM                       0x0Fu     /* DM frame type coding in the frame control field. */
#define FRAME_TYPE_DISC                     0x43u     /* DISC frame type coding in the frame control field. */
#define FRAME_TYPE_UIH                      0xEFu     /* UIH frame type coding in the frame control field. */

#define PF_BIT                              (1u << 4) /* P/F bit position in the frame control field. */
#define EA_BIT                              (1u << 0) /* E/A bit position in the frame address field. */
#define CR_BIT                              (1u << 1) /* C/R bit position in the frame address field. */

#define DLCI_ID_LOWER_BOUND                 1u        /* Lower bound value of DLCI id.*/
#define DLCI_ID_UPPER_BOUND                 63u       /* Upper bound value of DLCI id.*/

#define FRAME_FLAG_SEQUENCE_FIELD_INDEX     0         /* Index of the frame flag sequence field. */
#define FRAME_ADDRESS_FIELD_INDEX           1u        /* Index of the frame address field. */
#define FRAME_CONTROL_FIELD_INDEX           2u        /* Index of the frame control field. */
#define FRAME_LENGTH_FIELD_INDEX            3u        /* Index of the frame length indicator field. */
#define FRAME_INFORMATION_FIELD_INDEX       4u        /* Index of the frame information field. */

#define FRAME_START_READ_LEN                1u        /* Frame start read length in number of bytes. */
#define FRAME_HEADER_READ_LEN               3u        /* Frame header read length in number of bytes. */
#define FRAME_TRAILER_LEN                   2u        /* Frame trailer read length in number of bytes. */

#define LENGTH_INDICATOR_OCTET              1u        /* Length indicator field value used in frame. */

/* Definition for frame header type. */
struct frame_hdr_t {
    uint8_t flag_seq;       /* Flag sequence field. */
    uint8_t address;        /* Address field. */
    uint8_t control;        /* Control field. */
    uint8_t length;         /* Length field. */
    uint8_t information[1]; /* Begin of the information field if present. */
};

uint8_t Mux::_shared_memory = 0;
FileHandle* Mux::_serial    = NULL;
EventQueue* Mux::_event_q   = NULL;

MuxDataService Mux::_mux_objects[MBED_CONF_MUX_DLCI_COUNT];
Semaphore Mux::_semaphore(0);
PlatformMutex Mux::_mutex;
Mux::tx_context_t Mux::_tx_context;
Mux::rx_context_t Mux::_rx_context;
Mux::state_t Mux::_state;
const uint8_t Mux::_crctable[MUX_CRC_TABLE_LEN] = {
    0x00, 0x91, 0xE3, 0x72, 0x07, 0x96, 0xE4, 0x75,  0x0E, 0x9F, 0xED, 0x7C, 0x09, 0x98, 0xEA, 0x7B,
    0x1C, 0x8D, 0xFF, 0x6E, 0x1B, 0x8A, 0xF8, 0x69,  0x12, 0x83, 0xF1, 0x60, 0x15, 0x84, 0xF6, 0x67,
    0x38, 0xA9, 0xDB, 0x4A, 0x3F, 0xAE, 0xDC, 0x4D,  0x36, 0xA7, 0xD5, 0x44, 0x31, 0xA0, 0xD2, 0x43,
    0x24, 0xB5, 0xC7, 0x56, 0x23, 0xB2, 0xC0, 0x51,  0x2A, 0xBB, 0xC9, 0x58, 0x2D, 0xBC, 0xCE, 0x5F,

    0x70, 0xE1, 0x93, 0x02, 0x77, 0xE6, 0x94, 0x05,  0x7E, 0xEF, 0x9D, 0x0C, 0x79, 0xE8, 0x9A, 0x0B,
    0x6C, 0xFD, 0x8F, 0x1E, 0x6B, 0xFA, 0x88, 0x19,  0x62, 0xF3, 0x81, 0x10, 0x65, 0xF4, 0x86, 0x17,
    0x48, 0xD9, 0xAB, 0x3A, 0x4F, 0xDE, 0xAC, 0x3D,  0x46, 0xD7, 0xA5, 0x34, 0x41, 0xD0, 0xA2, 0x33,
    0x54, 0xC5, 0xB7, 0x26, 0x53, 0xC2, 0xB0, 0x21,  0x5A, 0xCB, 0xB9, 0x28, 0x5D, 0xCC, 0xBE, 0x2F,

    0xE0, 0x71, 0x03, 0x92, 0xE7, 0x76, 0x04, 0x95,  0xEE, 0x7F, 0x0D, 0x9C, 0xE9, 0x78, 0x0A, 0x9B,
    0xFC, 0x6D, 0x1F, 0x8E, 0xFB, 0x6A, 0x18, 0x89,  0xF2, 0x63, 0x11, 0x80, 0xF5, 0x64, 0x16, 0x87,
    0xD8, 0x49, 0x3B, 0xAA, 0xDF, 0x4E, 0x3C, 0xAD,  0xD6, 0x47, 0x35, 0xA4, 0xD1, 0x40, 0x32, 0xA3,
    0xC4, 0x55, 0x27, 0xB6, 0xC3, 0x52, 0x20, 0xB1,  0xCA, 0x5B, 0x29, 0xB8, 0xCD, 0x5C, 0x2E, 0xBF,

    0x90, 0x01, 0x73, 0xE2, 0x97, 0x06, 0x74, 0xE5,  0x9E, 0x0F, 0x7D, 0xEC, 0x99, 0x08, 0x7A, 0xEB,
    0x8C, 0x1D, 0x6F, 0xFE, 0x8B, 0x1A, 0x68, 0xF9,  0x82, 0x13, 0x61, 0xF0, 0x85, 0x14, 0x66, 0xF7,
    0xA8, 0x39, 0x4B, 0xDA, 0xAF, 0x3E, 0x4C, 0xDD,  0xA6, 0x37, 0x45, 0xD4, 0xA1, 0x30, 0x42, 0xD3,
    0xB4, 0x25, 0x57, 0xC6, 0xB3, 0x22, 0x50, 0xC1,  0xBA, 0x2B, 0x59, 0xC8, 0xBD, 0x2C, 0x5E, 0xCF
};

void Mux::module_init()
{
    _state.is_mux_open              = 0;
    _state.is_mux_open_pending      = 0;
    _state.is_mux_open_running      = 0;
    _state.is_dlci_open_pending     = 0;
    _state.is_dlci_open_running     = 0;
    _state.is_system_thread_context = 0;
    _state.is_tx_callback_context   = 0;
    _state.is_user_tx_pending       = 0;
    _state.is_user_rx_ready         = 0;

    _rx_context.offset      = 0;
    _rx_context.read_length = 0;
    _rx_context.rx_state    = RX_FRAME_START;

    _tx_context.tx_state            = TX_IDLE;
    _tx_context.tx_callback_context = 0;

    frame_hdr_t* frame_hdr =
        reinterpret_cast<frame_hdr_t *>(&(Mux::_tx_context.buffer[FRAME_FLAG_SEQUENCE_FIELD_INDEX]));
    frame_hdr->flag_seq    = FLAG_SEQUENCE_OCTET;

    const uint8_t end = sizeof(_mux_objects) / sizeof(_mux_objects[0]);
    for (uint8_t i = 0; i != end; ++i) {
        _mux_objects[i]._dlci = MUX_DLCI_INVALID_ID;
    }
}


void Mux::frame_retransmit_begin()
{
    _tx_context.bytes_remaining = _tx_context.offset;
    _tx_context.offset          = 0;
}


void Mux::on_timeout()
{
    _mutex.lock();

    _state.is_system_thread_context = 1u;

    switch (_tx_context.tx_state) {
        case TX_RETRANSMIT_DONE:
            if (_tx_context.retransmit_counter != 0) {
                --(_tx_context.retransmit_counter);
                frame_retransmit_begin();
                tx_state_change(TX_RETRANSMIT_ENQUEUE, tx_retransmit_enqueu_entry_run, null_action);
            } else {
                /* Retransmission limit reached, change state and release the suspended call thread with appropriate
                   status code. */
                _shared_memory           = MUX_ESTABLISH_TIMEOUT;
                const osStatus os_status = _semaphore.release();
                MBED_ASSERT(os_status == osOK);
                tx_state_change(TX_IDLE, tx_idle_entry_run, null_action);
            }
            break;
        default:
            /* No implementation required. */
            break;
    }

    _state.is_system_thread_context = 0;

    _mutex.unlock();
}


void Mux::dm_response_construct()
{
    frame_hdr_t* frame_hdr =
        reinterpret_cast<frame_hdr_t *>(&(Mux::_tx_context.buffer[FRAME_FLAG_SEQUENCE_FIELD_INDEX]));

    frame_hdr->address        = _rx_context.buffer[FRAME_ADDRESS_FIELD_INDEX];
    frame_hdr->control        = (FRAME_TYPE_DM | PF_BIT);
    frame_hdr->length         = LENGTH_INDICATOR_OCTET;
    frame_hdr->information[0] = fcs_calculate(&(Mux::_tx_context.buffer[FRAME_ADDRESS_FIELD_INDEX]), FCS_INPUT_LEN);
    (++frame_hdr)->flag_seq   = FLAG_SEQUENCE_OCTET;

    _tx_context.bytes_remaining = DM_FRAME_LEN;
}


void Mux::on_rx_frame_sabm()
{
    /* Peer iniated open/establishment is not supported. */
    rx_state_change(RX_HEADER_READ, rx_header_read_entry_run);
}


void Mux::on_rx_frame_ua()
{
    switch (_tx_context.tx_state) {
            osStatus os_status;
            uint8_t  rx_dlci_id;
            uint8_t  tx_dlci_id;
            bool     is_cr_bit_set;
            bool     is_pf_bit_set;
        case TX_RETRANSMIT_DONE:
            is_cr_bit_set = _rx_context.buffer[FRAME_ADDRESS_FIELD_INDEX] & CR_BIT;
            is_pf_bit_set = _rx_context.buffer[FRAME_CONTROL_FIELD_INDEX] & PF_BIT;

            if (is_cr_bit_set && is_pf_bit_set) {
                tx_dlci_id = _tx_context.buffer[FRAME_ADDRESS_FIELD_INDEX] >> 2;
                rx_dlci_id = _rx_context.buffer[FRAME_ADDRESS_FIELD_INDEX] >> 2;
                if (tx_dlci_id == rx_dlci_id) {
                    _event_q->cancel(_tx_context.timer_id);
                    _shared_memory = MUX_ESTABLISH_SUCCESS;
                    if (rx_dlci_id != 0) {
                        dlci_id_append(rx_dlci_id);
                    }

                    os_status = _semaphore.release();
                    MBED_ASSERT(os_status == osOK);
                    tx_state_change(TX_IDLE, tx_idle_entry_run, null_action);
                }
            }
            break;
        default:
            /* No implementation required. */
            break;
    }

    rx_state_change(RX_HEADER_READ, rx_header_read_entry_run);
}


void Mux::on_rx_frame_dm()
{
    switch (_tx_context.tx_state) {
            osStatus os_status;
            uint8_t  rx_dlci_id;
            uint8_t  tx_dlci_id;
            bool     is_cr_bit_set;
            bool     is_pf_bit_set;
        case TX_RETRANSMIT_DONE:
            is_cr_bit_set = _rx_context.buffer[FRAME_ADDRESS_FIELD_INDEX] & CR_BIT;
            is_pf_bit_set = _rx_context.buffer[FRAME_CONTROL_FIELD_INDEX] & PF_BIT;

            if (is_cr_bit_set && is_pf_bit_set) {
                tx_dlci_id = _tx_context.buffer[FRAME_ADDRESS_FIELD_INDEX] >> 2;
                rx_dlci_id = _rx_context.buffer[FRAME_ADDRESS_FIELD_INDEX] >> 2;
                if (tx_dlci_id == rx_dlci_id) {
                    _event_q->cancel(_tx_context.timer_id);
                    _shared_memory = MUX_ESTABLISH_REJECT;
                    os_status      = _semaphore.release();
                    MBED_ASSERT(os_status == osOK);
                    tx_state_change(TX_IDLE, tx_idle_entry_run, null_action);
                }
            }
            break;
        default:
            /* No implementation required. */
            break;
    }

    rx_state_change(RX_HEADER_READ, rx_header_read_entry_run);
}


void Mux::tx_internal_resp_entry_run()
{
    write_do();
}


void Mux::dm_response_send()
{
    dm_response_construct();
    tx_state_change(TX_INTERNAL_RESP, tx_internal_resp_entry_run, tx_idle_exit_run);
}


void Mux::on_rx_frame_disc()
{
    /* Follow the specification: DM response generated for those DLCI IDs which are not established. */

    switch (_tx_context.tx_state) {
            uint8_t dlci_id;
            bool    is_cr_bit_clear;
            bool    is_pf_bit_set;
        case TX_IDLE:
            if (!_state.is_mux_open) {
                dm_response_send();
            } else {
                is_cr_bit_clear = !(_rx_context.buffer[FRAME_ADDRESS_FIELD_INDEX] & CR_BIT);
                is_pf_bit_set   = _rx_context.buffer[FRAME_CONTROL_FIELD_INDEX] & PF_BIT;

                if (is_cr_bit_clear && is_pf_bit_set) {
                    dlci_id = _rx_context.buffer[FRAME_ADDRESS_FIELD_INDEX] >> 2;
                    if (dlci_id != 0) {
                        if (!is_dlci_in_use(dlci_id)) {
                            dm_response_send();
                        } else {
                            /* DLCI close not supported and silently discarded. */
                        }
                    } else {
                        /* Mux close not supported and silently discarded. */
                    }
                }
            }
            break;
        default:
            /* @note: Silently discarded as the current implementation does not support pending DM frame generation. */
            break;
    }

    rx_state_change(RX_HEADER_READ, rx_header_read_entry_run);
}


void Mux::on_rx_frame_uih()
{
    const uint8_t length = (_rx_context.buffer[FRAME_LENGTH_FIELD_INDEX] >> 1);
    if (length != 0) {
        /* Proceed with processing for non 0 length user data frames. */

        const bool is_cr_bit_clear = !(_rx_context.buffer[FRAME_ADDRESS_FIELD_INDEX] & CR_BIT);
        const bool is_pf_bit_clear = !(_rx_context.buffer[FRAME_CONTROL_FIELD_INDEX] & PF_BIT);
        if (is_cr_bit_clear && is_pf_bit_clear) {
            /* Proceed with processing upon P/F and C/R bits being clear. */

            const uint8_t dlci_id = _rx_context.buffer[FRAME_ADDRESS_FIELD_INDEX] >> 2;
            if (dlci_id != MUX_DLCI_INVALID_ID) {
                /* Proceed with processing for non internal invalidate ID type. */

                MuxDataService* obj = file_handle_get(dlci_id);
                if (obj != NULL) {
                    /* Established DLCI exists, proceed with processing. */

                    _state.is_user_rx_ready = 1u;
                    _rx_context.offset      = 0;
                    _rx_context.read_length = length;

                    rx_state_change(RX_SUSPEND, null_action);
                    obj->_sigio_cb();

                    return;
                }
            }
        }
    }

    /* Default behaviour upon Rx of non valid user data frame. */
    rx_state_change(RX_HEADER_READ, rx_header_read_entry_run);
}


void Mux::on_rx_frame_not_supported()
{
    rx_state_change(RX_HEADER_READ, rx_header_read_entry_run);
}


Mux::FrameRxType Mux::frame_rx_type_resolve()
{
    const uint8_t frame_type = (_rx_context.buffer[FRAME_CONTROL_FIELD_INDEX] & ~PF_BIT);

    if (frame_type == FRAME_TYPE_SABM) {
        return FRAME_RX_TYPE_SABM;
    } else if (frame_type == FRAME_TYPE_UA) {
        return FRAME_RX_TYPE_UA;
    } else if (frame_type == FRAME_TYPE_DM) {
        return FRAME_RX_TYPE_DM;
    } else if (frame_type == FRAME_TYPE_DISC) {
        return FRAME_RX_TYPE_DISC;
    } else if (frame_type == FRAME_TYPE_UIH) {
        return FRAME_RX_TYPE_UIH;
    } else {
        return FRAME_RX_TYPE_NOT_SUPPORTED;
    }
}


void Mux::tx_state_change(TxState new_state, tx_state_entry_func_t entry_func, tx_state_exit_func_t exit_func)
{
    exit_func();
    _tx_context.tx_state = new_state;
    entry_func();
}


void Mux::event_queue_enqueue()
{
    const int id = _event_q->call(Mux::on_deferred_call);
    MBED_ASSERT(id != 0);
}


void Mux::tx_retransmit_done_entry_run()
{
    _tx_context.timer_id = _event_q->call_in(T1_TIMER_VALUE, Mux::on_timeout);
    MBED_ASSERT(_tx_context.timer_id != 0);
}


bool Mux::is_dlci_in_use(uint8_t dlci_id)
{
    const uint8_t end = sizeof(_mux_objects) / sizeof(_mux_objects[0]);
    for (uint8_t i = 0; i != end; ++i) {
        if (_mux_objects[i]._dlci== dlci_id) {
            return true;
        }
    }

    return false;
}


void Mux::on_post_tx_frame_sabm()
{
    switch (_tx_context.tx_state) {
        case TX_RETRANSMIT_ENQUEUE:
            tx_state_change(TX_RETRANSMIT_DONE, tx_retransmit_done_entry_run, null_action);
            break;
        default:
            /* Code that should never be reached. */
            MBED_ASSERT(false);
            break;
    }
}


void Mux::pending_self_iniated_mux_open_start()
{
    /* Construct the frame, start the tx sequence, set and reset relevant state contexts. */
    _state.is_mux_open_running = 1u;
    _state.is_mux_open_pending = 0;

    sabm_request_construct(0);
    tx_state_change(TX_RETRANSMIT_ENQUEUE, tx_retransmit_enqueu_entry_run, tx_idle_exit_run);
    _tx_context.retransmit_counter = RETRANSMIT_COUNT;
}


void Mux::pending_self_iniated_dlci_open_start()
{
    /* Construct the frame, start the tx sequence, set and reset relevant state contexts. */
    _state.is_dlci_open_running = 1u;
    _state.is_dlci_open_pending = 0;

    sabm_request_construct(_shared_memory);
    tx_state_change(TX_RETRANSMIT_ENQUEUE, tx_retransmit_enqueu_entry_run, tx_idle_exit_run);
    _tx_context.retransmit_counter = RETRANSMIT_COUNT;
}


void Mux::tx_idle_exit_run()
{
    _tx_context.offset = 0;
}


uint8_t Mux::tx_callback_index_advance()
{
    /* Increment and get the index bit accounting the roll over. */
    uint8_t index = ((_tx_context.tx_callback_context & 0xF0u) >> 4);
    index <<= 1;
    if ((index & 0x0Fu) == 0) {
        index = 1u;
    }

    /* Clear existing index bit and assign the new incremented index bit. */
    _tx_context.tx_callback_context &= 0x0Fu;
    _tx_context.tx_callback_context |= (index << 4);

    return index;
}


uint8_t Mux::tx_callback_pending_mask_get()
{
    return (_tx_context.tx_callback_context & 0x0Fu);
}


void Mux::tx_callback_pending_bit_clear(uint8_t bit)
{
    _tx_context.tx_callback_context &= ~bit;
}


void Mux::tx_callback_pending_bit_set(uint8_t dlci_id)
{
    uint8_t i         = 0;
    uint8_t bit       = 1u;
    const uint8_t end = sizeof(_mux_objects) / sizeof(_mux_objects[0]);

    do {
        if (_mux_objects[i]._dlci== dlci_id) {
            break;
        }

        ++i;
        bit <<= 1;
    } while (i != end);

    MBED_ASSERT(i != end);

    _tx_context.tx_callback_context |= bit;
}


MuxDataService& Mux::tx_callback_lookup(uint8_t bit)
{
    uint8_t i         = 0;
    const uint8_t end = sizeof(_mux_objects) / sizeof(_mux_objects[0]);

    do {
        bit >>= 1;
        if (bit == 0) {
            break;
        }

        ++i;
    } while (i != end);

    MBED_ASSERT(i != end);

    return _mux_objects[i];
}


void Mux::tx_callback_dispatch(uint8_t bit)
{
    MuxDataService& obj = tx_callback_lookup(bit);
    obj._sigio_cb();
}


void Mux::tx_callbacks_run()
{
    uint8_t current_tx_index;
    uint8_t tx_callback_pending_mask = tx_callback_pending_mask_get();

    while (tx_callback_pending_mask != 0) {
        /* Locate 1st pending TX callback. */
        do {
            current_tx_index = tx_callback_index_advance();
        } while ((current_tx_index & tx_callback_pending_mask) == 0);

        /* Clear pending bit and dispatch TX callback. */
        tx_callback_pending_bit_clear(current_tx_index);

        tx_callback_dispatch(current_tx_index);

        /* No valid use case exists for TX cycle activation within TX callback as per design user TX is started within
           this while loop using @ref is_user_tx_pending bit and per system design DLCI establishment is not allowed
           within callback context as this would leave to system thread context dead lock as DLCI establishment includes
           semaphore wait. We will enforce this by MBED_ASSERT below. Note that @ref dlci_establish will have
           MBED_ASSERT to enforce calling that API from system thread context thus the assert below is not absolutely
           necessary. */
        MBED_ASSERT(_tx_context.tx_state == TX_IDLE);

        if (_state.is_user_tx_pending) {
            /* User TX was requested within callback context dispatched above. */

            /* TX buffer was constructed within @ref user_data_tx, now start the TX cycle. */
            _state.is_user_tx_pending = 0;
            tx_state_change(TX_NORETRANSMIT, tx_noretransmit_entry_run, tx_idle_exit_run);
            if (_tx_context.tx_state != TX_IDLE) {
                /* TX cycle not finished within current context, stop callback processing as we will continue when TX
                   cycle finishes and this function is re-entered. */
                break;
            }
        }
        tx_callback_pending_mask = tx_callback_pending_mask_get();
    }
}


void Mux::tx_idle_entry_run()
{
    if (_state.is_mux_open_pending) {
        pending_self_iniated_mux_open_start();
    } else if (_state.is_dlci_open_pending) {
        pending_self_iniated_dlci_open_start();
    } else {
        /* TX callback processing block below could be entered recursively within same thread context. Protection bit
           is used to prevent that. */
        if (!_state.is_tx_callback_context) {
            /* Lock this code block from multiple entries within same thread context by using a protection bit. Check
               and process possible pending user TX callback request as long there is something pending. Round robin
               shceduling used for dispatching pending TX callback in order to prevent starvation of pending callbacks.
            */
            _state.is_tx_callback_context = 1u;
            tx_callbacks_run();
            _state.is_tx_callback_context = 0;
        }
    }
}


void Mux::on_post_tx_frame_dm()
{
    switch (_tx_context.tx_state) {
        case TX_INTERNAL_RESP:
            tx_state_change(TX_IDLE, tx_idle_entry_run, null_action);
            break;
        default:
            /* Code that should never be reached. */
            MBED_ASSERT(false);
            break;
    }
}


void Mux::on_post_tx_frame_uih()
{
    switch (_tx_context.tx_state) {
        case TX_NORETRANSMIT:
            tx_state_change(TX_IDLE, tx_idle_entry_run, null_action);
            break;
        default:
            /* Code that should never be reached. */
            MBED_ASSERT(false);
            break;
    }
}


Mux::FrameTxType Mux::frame_tx_type_resolve()
{
    const uint8_t frame_type = (_tx_context.buffer[FRAME_CONTROL_FIELD_INDEX] & ~PF_BIT);

    if (frame_type == FRAME_TYPE_SABM) {
        return FRAME_TX_TYPE_SABM;
    } else if (frame_type == FRAME_TYPE_DM) {
        return FRAME_TX_TYPE_DM;
    } else if (frame_type == FRAME_TYPE_UIH) {
        return FRAME_TX_TYPE_UIH;
    } else {
        MBED_ASSERT(false);
        return FRAME_TX_TYPE_MAX;
    }
}


void Mux::null_action()
{

}


void Mux::rx_header_read_entry_run()
{
    _rx_context.offset      = FRAME_ADDRESS_FIELD_INDEX;
    _rx_context.read_length = FRAME_HEADER_READ_LEN;
}


void Mux::rx_state_change(RxState new_state, rx_state_entry_func_t entry_func)
{
    entry_func();
    _rx_context.rx_state = new_state;
}


ssize_t Mux::on_rx_read_state_frame_start()
{
    ssize_t read_err;
    _rx_context.buffer[_rx_context.offset] = static_cast<uint8_t>(~FLAG_SEQUENCE_OCTET);
    do {
        read_err = _serial->read(&(_rx_context.buffer[_rx_context.offset]), FRAME_START_READ_LEN);
    } while ((_rx_context.buffer[_rx_context.offset] != FLAG_SEQUENCE_OCTET) && (read_err != -EAGAIN));

    if (_rx_context.buffer[_rx_context.offset] == FLAG_SEQUENCE_OCTET) {
        _rx_context.offset     += FRAME_START_READ_LEN;
        _rx_context.read_length = FRAME_HEADER_READ_LEN;

        rx_state_change(RX_HEADER_READ, rx_header_read_entry_run);
    }

    return read_err;
}


ssize_t Mux::on_rx_read_state_header_read()
{
    ssize_t read_err;

    do {
        read_err = _serial->read(&(_rx_context.buffer[_rx_context.offset]), _rx_context.read_length);
        if (read_err != -EAGAIN) {
            if ((_rx_context.offset == FRAME_ADDRESS_FIELD_INDEX) &&
                    (_rx_context.buffer[_rx_context.offset] == FLAG_SEQUENCE_OCTET)) {
                /* Overlapping block move 1-index possible trailing data after the flag sequence octet. */

                memmove(&(_rx_context.buffer[_rx_context.offset]),
                        &(_rx_context.buffer[_rx_context.offset + 1u]),
                        (read_err - 1u));

                _rx_context.offset      += (read_err - 1u);
                _rx_context.read_length -= (read_err - 1u);
            } else {
                _rx_context.offset      += read_err;
                _rx_context.read_length -= read_err;
            }
        }
    } while ((_rx_context.read_length != 0) && (read_err != -EAGAIN));

    if (_rx_context.offset == (FRAME_HEADER_READ_LEN + FRAME_START_READ_LEN)) {
        /* Decode remaining frame read length and change state. Current implementation supports only 1-byte length
           field, enforce this with MBED_ASSERT along with buffer over write. */

        MBED_ASSERT((_rx_context.buffer[FRAME_LENGTH_FIELD_INDEX] & LENGTH_INDICATOR_OCTET));
        _rx_context.read_length = (_rx_context.buffer[FRAME_LENGTH_FIELD_INDEX] >> 1) + FRAME_TRAILER_LEN;
        MBED_ASSERT(_rx_context.read_length <=
                    (MBED_CONF_MUX_BUFFER_SIZE - (FRAME_START_READ_LEN + FRAME_HEADER_READ_LEN)));

        rx_state_change(RX_TRAILER_READ, null_action);
    }

    return read_err;
}


bool Mux::is_rx_fcs_valid()
{
    const uint8_t expected_fcs = fcs_calculate(&(_rx_context.buffer[FRAME_ADDRESS_FIELD_INDEX]), FCS_INPUT_LEN);
    const uint8_t actual_fcs   =
        _rx_context.buffer[FRAME_INFORMATION_FIELD_INDEX + (_rx_context.buffer[FRAME_LENGTH_FIELD_INDEX] >> 1)];

    return (expected_fcs == actual_fcs);
}


ssize_t Mux::on_rx_read_state_trailer_read()
{
    typedef void (*rx_frame_decoder_func_t)();
    static const rx_frame_decoder_func_t rx_frame_decoder_func[FRAME_RX_TYPE_MAX] = {
        on_rx_frame_sabm,
        on_rx_frame_ua,
        on_rx_frame_dm,
        on_rx_frame_disc,
        on_rx_frame_uih,
        on_rx_frame_not_supported
    };

    ssize_t read_err;
    do {
        read_err = _serial->read(&(_rx_context.buffer[_rx_context.offset]), _rx_context.read_length);
        if (read_err != -EAGAIN) {
            _rx_context.offset      += read_err;
            _rx_context.read_length -= read_err;
        }
    } while ((_rx_context.read_length != 0) && (read_err != -EAGAIN));

    if (_rx_context.read_length == 0) {
        /* Complete frame received, verify FCS and if valid proceed with processing. */

        if (is_rx_fcs_valid()) {
            const Mux::FrameRxType frame_type  = frame_rx_type_resolve();
            const rx_frame_decoder_func_t func = rx_frame_decoder_func[frame_type];

            func();
        } else {
            rx_state_change(RX_HEADER_READ, rx_header_read_entry_run);
        }
    }

    return read_err;
}


ssize_t Mux::on_rx_read_state_suspend()
{
    return -EAGAIN;
}


void Mux::rx_event_do(RxEvent event)
{
    typedef ssize_t (*rx_read_func_t)();
    static const rx_read_func_t rx_read_func[RX_STATE_MAX] = {
        on_rx_read_state_frame_start,
        on_rx_read_state_header_read,
        on_rx_read_state_trailer_read,
        on_rx_read_state_suspend,
    };

    switch (event) {
            ssize_t        read_err;
            rx_read_func_t func;
        case RX_READ:
            do {
                func     = rx_read_func[_rx_context.rx_state];
                read_err = func();
            } while (read_err != -EAGAIN);

            break;
        case RX_RESUME:
            MBED_ASSERT(_rx_context.rx_state == RX_SUSPEND);

            rx_state_change(RX_HEADER_READ, rx_header_read_entry_run);
            event_queue_enqueue();
            break;
        default:
            /* Code that should never be reached. */
            MBED_ASSERT(false);
            break;
    }
}


void Mux::write_do()
{
    switch (_tx_context.tx_state) {
            ssize_t write_err;
        case TX_NORETRANSMIT:
        case TX_RETRANSMIT_ENQUEUE:
        case TX_INTERNAL_RESP:
            write_err = 1;
            do {
                write_err = _serial->write(&(_tx_context.buffer[_tx_context.offset]), _tx_context.bytes_remaining);
                MBED_ASSERT(write_err >= 0);

                _tx_context.bytes_remaining -= write_err;
                _tx_context.offset          += write_err;
            } while ((_tx_context.bytes_remaining != 0) && (write_err > 0));

            if (_tx_context.bytes_remaining == 0) {
                /* Frame write complete, execute correct post processing function for clean-up. */

                typedef void (*post_tx_frame_func_t)();
                static const post_tx_frame_func_t post_tx_func[FRAME_TX_TYPE_MAX] = {
                    on_post_tx_frame_sabm,
                    on_post_tx_frame_dm,
                    on_post_tx_frame_uih
                };
                const Mux::FrameTxType     frame_type = frame_tx_type_resolve();
                const post_tx_frame_func_t func       = post_tx_func[frame_type];
                func();
            }

            break;
        default:
            /* No implementation required. */
            break;
    }
}


void Mux::on_deferred_call()
{
    _mutex.lock();

    _state.is_system_thread_context = 1u;

    rx_event_do(RX_READ);
    write_do();

    _state.is_system_thread_context = 0;

    _mutex.unlock();
}


void Mux::on_sigio()
{
    event_queue_enqueue();
}


void Mux::eventqueue_attach(EventQueue *event_queue)
{
    _event_q = event_queue;
}


void Mux::serial_attach(FileHandle *serial)
{
    _serial = serial;

    _serial->sigio(Mux::on_sigio);
    _serial->set_blocking(false);
}


uint8_t Mux::fcs_calculate(const uint8_t *buffer,  uint8_t input_len)
{
    uint8_t fcs = 0xFFu;

    while (input_len-- != 0) {
        fcs = _crctable[fcs^*buffer++];
    }

    /* Ones complement. */
    fcs = 0xFFu - fcs;

    return fcs;
}


void Mux::sabm_request_construct(uint8_t dlci_id)
{
    frame_hdr_t *frame_hdr =
        reinterpret_cast<frame_hdr_t *>(&(Mux::_tx_context.buffer[FRAME_FLAG_SEQUENCE_FIELD_INDEX]));

    frame_hdr->address        = EA_BIT | CR_BIT | (dlci_id << 2);
    frame_hdr->control        = (FRAME_TYPE_SABM | PF_BIT);
    frame_hdr->length         = LENGTH_INDICATOR_OCTET;
    frame_hdr->information[0] = fcs_calculate(&(Mux::_tx_context.buffer[FRAME_ADDRESS_FIELD_INDEX]), FCS_INPUT_LEN);
    (++frame_hdr)->flag_seq   = FLAG_SEQUENCE_OCTET;

    _tx_context.bytes_remaining = SABM_FRAME_LEN;
}


bool Mux::is_dlci_q_full()
{
    const uint8_t end = sizeof(_mux_objects) / sizeof(_mux_objects[0]);
    for (uint8_t i = 0; i != end; ++i) {
        if (_mux_objects[i]._dlci== MUX_DLCI_INVALID_ID) {
            return false;
        }
    }

    return true;
}


void Mux::dlci_id_append(uint8_t dlci_id)
{
    uint8_t i         = 0;
    const uint8_t end = sizeof(_mux_objects) / sizeof(_mux_objects[0]);
    do {
        if (_mux_objects[i]._dlci== MUX_DLCI_INVALID_ID) {
            _mux_objects[i]._dlci= dlci_id;

            break;
        }

        ++i;
    } while (i != end);

    /* Internal implementation error if append was not done as Q size is checked prior call. */
    MBED_ASSERT(i != end);
}


MuxDataService * Mux::file_handle_get(uint8_t dlci_id)
{
    MuxDataService* obj = NULL;
    const uint8_t end = sizeof(_mux_objects) / sizeof(_mux_objects[0]);
    for (uint8_t i = 0; i != end; ++i) {
        if (_mux_objects[i]._dlci== dlci_id) {
            obj = &(_mux_objects[i]);

            break;
        }
    }

    return obj;
}


Mux::MuxReturnStatus Mux::dlci_establish(uint8_t dlci_id, MuxEstablishStatus &status, FileHandle **obj)
{
    if ((dlci_id < DLCI_ID_LOWER_BOUND) || (dlci_id > DLCI_ID_UPPER_BOUND)) {
        return MUX_STATUS_INVALID_RANGE;
    }

    _mutex.lock();

    MBED_ASSERT(!_state.is_system_thread_context);

    if (!_state.is_mux_open) {
        _mutex.unlock();

        return MUX_STATUS_MUX_NOT_OPEN;
    }
    if (is_dlci_q_full()) {
        _mutex.unlock();

        return MUX_STATUS_NO_RESOURCE;
    }
    if (is_dlci_in_use(dlci_id)) {
        _mutex.unlock();

        return MUX_STATUS_NO_RESOURCE;
    }
    if (_state.is_dlci_open_pending) {
        _mutex.unlock();

        return MUX_STATUS_INPROGRESS;
    }
    if (_state.is_dlci_open_running) {
        _mutex.unlock();

        return MUX_STATUS_INPROGRESS;
    }

    switch (_tx_context.tx_state) {
        int32_t ret_wait;
        case TX_IDLE:
            /* Construct the frame, start the tx sequence, and suspend the call thread upon write sequence success. */
            sabm_request_construct(dlci_id);
            _tx_context.retransmit_counter = RETRANSMIT_COUNT;
            tx_state_change(TX_RETRANSMIT_ENQUEUE, tx_retransmit_enqueu_entry_run, tx_idle_exit_run);
            _state.is_dlci_open_running = 1u;

            _mutex.unlock();
            ret_wait = _semaphore.wait();
            MBED_ASSERT(ret_wait == 1);
            status = static_cast<MuxEstablishStatus>(_shared_memory);
            if (status == MUX_ESTABLISH_SUCCESS) {
                *obj = file_handle_get(dlci_id);
                MBED_ASSERT(*obj != NULL);
            }

            break;
        case TX_INTERNAL_RESP:
        case TX_NORETRANSMIT:
            _state.is_dlci_open_pending = 1u;
            _shared_memory              = dlci_id;

            _mutex.unlock();
            ret_wait = _semaphore.wait();
            MBED_ASSERT(ret_wait == 1);
            status = static_cast<MuxEstablishStatus>(_shared_memory);
            if (status == MUX_ESTABLISH_SUCCESS) {
                *obj = file_handle_get(dlci_id);
                MBED_ASSERT(*obj != NULL);
            }

            break;
        default:
            /* Code that should never be reached. */
            MBED_ASSERT(false);

            break;
    }

    _state.is_dlci_open_running = 0;

    return MUX_STATUS_SUCCESS;
}


void Mux::tx_retransmit_enqueu_entry_run()
{
    write_do();
}


Mux::MuxReturnStatus Mux::mux_start(Mux::MuxEstablishStatus &status)
{
    _mutex.lock();

    if (_state.is_mux_open) {
        _mutex.unlock();

        return MUX_STATUS_NO_RESOURCE;
    }
    if (_state.is_mux_open_pending) {
        _mutex.unlock();

        return MUX_STATUS_INPROGRESS;
    }
    if (_state.is_mux_open_running ) {
        _mutex.unlock();

        return MUX_STATUS_INPROGRESS;
    }

    switch (_tx_context.tx_state) {
            int32_t ret_wait;
        case TX_IDLE:
            /* Construct the frame, start the tx sequence, and suspend the call thread upon write sequence success. */
            sabm_request_construct(0);
            _tx_context.retransmit_counter = RETRANSMIT_COUNT;
            tx_state_change(TX_RETRANSMIT_ENQUEUE, tx_retransmit_enqueu_entry_run, tx_idle_exit_run);
            _state.is_mux_open_running = 1u;

            _mutex.unlock();
            ret_wait = _semaphore.wait();
            MBED_ASSERT(ret_wait == 1);
            status = static_cast<MuxEstablishStatus>(_shared_memory);
            if (status == MUX_ESTABLISH_SUCCESS) {
                _state.is_mux_open = 1u;
            }

            break;
        case TX_INTERNAL_RESP:
            _state.is_mux_open_pending = 1u;

            _mutex.unlock();
            ret_wait = _semaphore.wait();
            MBED_ASSERT(ret_wait == 1);
            status = static_cast<MuxEstablishStatus>(_shared_memory);
            if (status == MUX_ESTABLISH_SUCCESS) {
                _state.is_mux_open = 1u;
            }

            break;
        default:
            /* Code that should never be reached. */
            MBED_ASSERT(false);
            break;
    };

    _state.is_mux_open_running = 0;

    return MUX_STATUS_SUCCESS;
}


void Mux::user_information_construct(uint8_t dlci_id, const void* buffer, size_t size)
{
    frame_hdr_t *frame_hdr =
        reinterpret_cast<frame_hdr_t *>(&(Mux::_tx_context.buffer[FRAME_FLAG_SEQUENCE_FIELD_INDEX]));

    frame_hdr->address  = EA_BIT | CR_BIT | (dlci_id << 2);
    frame_hdr->control  = FRAME_TYPE_UIH;
    frame_hdr->length   = (LENGTH_INDICATOR_OCTET | (size << 1));

    memmove(&(frame_hdr->information[0]), buffer, size);

    uint8_t* fcs_pos = (&(frame_hdr->information[0]) + size);
    *fcs_pos         = fcs_calculate(&(Mux::_tx_context.buffer[FRAME_ADDRESS_FIELD_INDEX]), FCS_INPUT_LEN);
    *(++fcs_pos)     = FLAG_SEQUENCE_OCTET;

    _tx_context.bytes_remaining = UIH_FRAME_MIN_LEN + size;
}


void Mux::tx_noretransmit_entry_run()
{
    write_do();
}


ssize_t Mux::user_data_tx(uint8_t dlci_id, const void* buffer, size_t size)
{
    MBED_ASSERT(buffer != NULL);

    if (size == 0) {
        return 0;
    }

    const size_t max_payload_size = (MBED_CONF_MUX_BUFFER_SIZE -
                                    (FRAME_START_READ_LEN + FRAME_HEADER_READ_LEN + FRAME_TRAILER_LEN));
    if (size > max_payload_size) {
        size = max_payload_size;
    }

    _mutex.lock();

    ssize_t write_ret;
    switch (_tx_context.tx_state) {
        case TX_IDLE:
            if (!_state.is_tx_callback_context) {
                /* Proper state to start TX cycle within current context. */

                user_information_construct(dlci_id, buffer, size);
                tx_state_change(TX_NORETRANSMIT, tx_noretransmit_entry_run, tx_idle_exit_run);

                write_ret = size;
            } else {
                /* Current context is TX callback context. */

                if (!_state.is_user_tx_pending) {
                    /* Signal callback context to start TX cycle and construct the frame. */

                    /* @note: If user is starting a TX to a DLCI, which is after the current DLCI TX callback within the
                     * stored sequence this will result to dispatching 1 unnecessary TX callback, if this is a issue one
                     * should clear the TX callback pending bit marker for this DLCI in this place. */

                    _state.is_user_tx_pending = 1u;
                    user_information_construct(dlci_id, buffer, size);

                    write_ret = size;
                } else {
                    /* TX cycle allready scheduled, set TX callback pending and inform caller by return value that no
                       action was taken. */

                    tx_callback_pending_bit_set(dlci_id);
                    write_ret = 0;
                }
            }

            break;
        default:
            /* TX allready in use, set TX callback pending and inform caller by return value that no action was taken.
             */
            tx_callback_pending_bit_set(dlci_id);
            write_ret = 0;

            break;
    }

    _mutex.unlock();

    return write_ret;
}


size_t Mux::min(uint8_t size_1, size_t size_2)
{
    return (size_1 < size_2) ? size_1 : size_2;
}


ssize_t Mux::user_data_rx(void* buffer, size_t size)
{
    MBED_ASSERT(buffer != NULL);

    _mutex.lock();

    if (_state.is_user_rx_ready) {
        const size_t read_length = min((_rx_context.read_length - _rx_context.offset), size);
        memcpy(buffer, &(_rx_context.buffer[FRAME_INFORMATION_FIELD_INDEX + _rx_context.offset]), read_length);
        _rx_context.offset += read_length;

        if (_rx_context.offset == _rx_context.read_length) {
            /* Current Rx buffer consumed, disable user reading and resume the Rx path. */

            _state.is_user_rx_ready = 0;
            rx_event_do(RX_RESUME);
        }

        _mutex.unlock();

        return static_cast<ssize_t>(read_length);
    } else {
        _mutex.unlock();

        return -EAGAIN;
    }
}


short Mux::poll()
{
    _mutex.lock();

    const bool writable = (_tx_context.tx_state == TX_IDLE);
    const bool readable = (_rx_context.rx_state == RX_SUSPEND);

    _mutex.unlock();

    return ((readable ? POLLIN : 0) | (writable ? POLLOUT : 0));
}

} // namespace mbed

#endif // MBED_CONF_CELLULAR_MUX_ENABLED
