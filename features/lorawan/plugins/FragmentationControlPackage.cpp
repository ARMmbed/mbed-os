/**
 * @file
 *
 * @brief      An implementation of LoRaWAN Fragmentation Control Block Specification v1.0.0
 *
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#include "FragmentationControlPackage.h"
#include "lorawan_data_structures.h"
#include "Callback.h"
#include "mbed-trace/mbed_trace.h"
#define TRACE_GROUP "FPKG"

#define FRAG_PKG_IDENTIFIER                              3
#define FRAG_PKG_VERSION                                 1
#define FEC_FRAG_ALGO                                    0
#define DATA_FRAGMENT_OVERHEAD                           3

typedef enum {
    FRAG_PACKAGE_VERSION    = 0x00, // Unicast only
    FRAG_STATUS             = 0x01, // Unicast & Multicast both
    FRAG_SESSION_SETUP      = 0x02, // Unicast only
    FRAG_SESSION_DELETE     = 0x03, // Unicast only
    DATA_FRAGMENT           = 0x08  // Unicast and Multicast both
} frag_ctrl_cmds;

FragmentationControlPackage::FragmentationControlPackage()
{
    _resp.type = FRAG_SESSION_STATUS;
    _resp.status = FRAG_SESSION_NOT_ACTIVE;
}

FragmentationControlPackage::~FragmentationControlPackage()
{
}

frag_ctrl_response_t *FragmentationControlPackage::parse(const uint8_t *payload,
                                                         uint16_t size,
                                                         uint8_t msg_type,
                                                         uint32_t dev_addr,
                                                         mbed::Callback<frag_bd_opts_t*(uint8_t frag_index, uint32_t desc)> opts_cb,
                                                         const lorawan_mcast_register_t *mcast_register,
                                                         uint32_t expected_desc)
{
    if (!payload || size == 0 || !opts_cb || !mcast_register) {
        return NULL;
    }

    uint8_t i = 0;
    uint8_t idx = 0;

    /**
     * DATA_FRAGMENT is always sent in isolation
     */
    if (payload[i++] == DATA_FRAGMENT) {

        // check size, must be at least 2 bytes
        if (size < DATA_FRAGMENT_OVERHEAD) {
            _resp.type = FRAG_SESSION_STATUS;
            _resp.status = FRAG_SESSION_SIZE_OR_INDEX_INCORRECT;
            return &_resp;
        }

        // Extract first 2 bytes from the pyaload, i.e., the Index&N field
        uint16_t index_and_n_field = payload[i++];
        index_and_n_field += (uint16_t) (payload[i++] << 8);

        // Extract FragIndex and N (the index of coded fragment)
        uint8_t frag_index = (index_and_n_field >> 14);
        uint16_t n = (index_and_n_field & 0x3FFF);

        // check if the session is active for the given frag_index
        if (!BIT_SET_TEST(_session_ctx.active_mask, frag_index)) {
            // silently drop the fragment
            return NULL;
        }

        // If fragment was sent using one of the multicast sessions, check if the
        // session is active and the device address of the received message matches
        // that of the multicast session
        if (msg_type == MSG_MULTICAST_FLAG) {
            bool can_continue = false;
            for (int i = 0; i < MBED_CONF_LORA_MAX_MULTICAST_SESSIONS; i++) {
                if (BIT_SET_TEST(_session_ctx.frag_session[frag_index].mcast_group_mask, i)
                        && BIT_SET_TEST(mcast_register->active_mask, i)
                        && dev_addr == mcast_register->entry[i].addr) {
                    can_continue = true;
                }
            }

            if (!can_continue) {
                // silently drop the fragment
                return NULL;
            }
        }

        // Shave off the overhead
        // (COMMAND_NAME (1 byte) + INDEX&N Field (2 bytes))
        uint16_t frag_size = size - DATA_FRAGMENT_OVERHEAD;

        // Fragment size cannot be different from the frag_size set for the session
        // the index of coded fragment 'n' starts from 1
        if (_session_ctx.frag_session[frag_index].frag_size != frag_size || n == 0) {
            // for now, think something better later
            _resp.type = FRAG_SESSION_STATUS;
            _resp.status = FRAG_SESSION_SIZE_OR_INDEX_INCORRECT;
            return &_resp;
        }

        frag_bd_opts_t *opts = _session_ctx.frag_session[frag_index].bd_opts;

        _session_ctx.frag_session[frag_index].fragments_received++;

        tr_debug("Fragment %d, tot frags %d, recvd %d, missing %d ", n,
                 _session_ctx.frag_session[frag_index].nb_frag,
                 _session_ctx.frag_session[frag_index].fragments_received,
                 _session_ctx.frag_session[frag_index].nb_frag - _session_ctx.frag_session[frag_index].fragments_received);

        // handle one of the fragments,
        if (_session_ctx.frag_session[frag_index].fragments_received
                < _session_ctx.frag_session[frag_index].nb_frag) {

            int r = opts->bd->program(payload + i,
                                      opts->offset + ((n - 1) * frag_size),
                                      frag_size);
            if (r != 0) {
                tr_error("Flash writing error");
                _resp.type = FRAG_SESSION_STATUS;
                _resp.status = FRAG_SESSION_FLASH_WRITE_ERROR;
                return &_resp;
            }

            opts->fasm->set_frag_received(n);

        } else if (_session_ctx.frag_session[frag_index].fragments_received
                == _session_ctx.frag_session[frag_index].nb_frag
                && opts->fasm->get_missing_frag_count() == 0) {
            // Fragmentation complete
            tr_info("**** Frag session complete ****");
            _resp.type = FRAG_SESSION_STATUS;
            _resp.status = FRAG_SESSION_COMPLETE;
            return &_resp;
        } else {
            // handle redundancy messages here
/*            frag_assembler_params_t sess;
            sess.DataSize = _session_ctx.frag_session[frag_index].frag_size;
            sess.NbOfFrag = _session_ctx.frag_session[frag_index].nb_frag;
            sess.Redundancy = opts->redundancy_max;*/
            int ret = opts->fasm->process_redundant_frag(n, payload + i/*, sess*/);

            if (ret != 0xFFFF) {
                tr_info(" ** Frag session complete **");
                _resp.type = FRAG_SESSION_STATUS;
                _resp.status = FRAG_SESSION_COMPLETE;
                return &_resp;
            }
        }

        _resp.type = FRAG_SESSION_STATUS;
        _resp.status = FRAG_SESSION_OK;
        return &_resp;
    }

    while (size > i) {
        switch (payload[i++]) {
            case FRAG_PACKAGE_VERSION: {
                _outbound_buf[idx++] = FRAG_PACKAGE_VERSION;
                _outbound_buf[idx++] = FRAG_PKG_IDENTIFIER;
                _outbound_buf[idx++] = FRAG_PKG_VERSION;
                break;
            }
            case FRAG_STATUS: {
                uint8_t status = payload[i++] & 0x07;
                uint8_t frag_index = (status & 0x06) >> 1;
                uint16_t recvd_fragments = _session_ctx.frag_session[frag_index].fragments_received;
                uint16_t nb_fragmnets = _session_ctx.frag_session[frag_index].nb_frag;
                uint16_t nb_lost_fragments = _session_ctx.frag_session[frag_index].bd_opts->fasm->get_missing_frag_count();

                bool participation_required = BIT_SET_TEST(status, 0) ? true : false;

                // If participation is not required we check if there is an active
                // session mapped to frag_index and there are any missing frames
                // and report otherwise skip
                if (BIT_SET_TEST(_session_ctx.active_mask, frag_index)
                        && (participation_required
                                || recvd_fragments < nb_fragmnets)) {
                    recvd_fragments += (frag_index << 14);
                    _outbound_buf[idx++] = FRAG_STATUS;
                    _outbound_buf[idx++] = recvd_fragments;
                    _outbound_buf[idx++] = (recvd_fragments >> 8);
                    _outbound_buf[idx++] = nb_lost_fragments > 255 ? 255 : nb_lost_fragments;
                    _outbound_buf[idx++] = 0;
                }

                break;
            }
            case FRAG_SESSION_SETUP: {
                _outbound_buf[idx++] = FRAG_SESSION_SETUP;
                uint8_t status_bit_mask = 0;
                uint8_t frag_session = payload[i++];
                // extract out FragIndex
                uint8_t frag_index = (frag_session & 0xF0) >> 4;
                if (frag_index >= MBED_CONF_LORA_MAX_FRAG_SESSIONS) {
                    tr_error("FragIndex %d unsupported", frag_index);
                    // set bit FragIndexNotSupported
                    SET_BIT(status_bit_mask, 2);
                } else {
                    // As per spec, even if a session exists, we are supposed to overwrite it
                    _session_ctx.frag_session[frag_index].frag_index = frag_index;
                    _session_ctx.frag_session[frag_index].mcast_group_mask = frag_session & 0x0F;
                    _session_ctx.frag_session[frag_index].nb_frag = payload[i++];
                    _session_ctx.frag_session[frag_index].nb_frag += payload[i++] << 8;
                    _session_ctx.frag_session[frag_index].frag_size = payload[i++];
                }

                uint8_t control = payload[i++];

                if ((control >> 3) != FEC_FRAG_ALGO) {
                    tr_error("Encoding unsupported, only FEC_FRAG_ALGO is supported");
                    SET_BIT(status_bit_mask, 0);
                } else {
                    _session_ctx.frag_session[frag_index].frag_algo = control >> 3;
                    _session_ctx.frag_session[frag_index].block_ack_delay = control & 0x07;
                }

                _session_ctx.frag_session[frag_index].padding = payload[i++];

                uint32_t incoming_desc = read_four_bytes(payload + i);
                i+=4;

                // check if the user provided an expected descriptor, if not we
                // we accept all incoming descriptors
                if (expected_desc && incoming_desc != expected_desc) {
                    tr_error("Descriptor mismatch");
                    SET_BIT(status_bit_mask, 3);
                }

                _session_ctx.frag_session[frag_index].descriptor = incoming_desc;

                if (status_bit_mask == 0) {
                    frag_bd_opts_t *opts = opts_cb.call(frag_index, incoming_desc);
                    if (opts == NULL) {
                        // Application refuses to attach a block device, set not
                        // enough memory error
                        SET_BIT(status_bit_mask, 1);
                    } else {
                        // Try initializing block device and Frag Assembler
                        if (opts->bd->init() != 0 ||
                                opts->fasm->initialize(opts->bd,
                                                       _session_ctx.frag_session[frag_index].nb_frag,
                                                       _session_ctx.frag_session[frag_index].frag_size,
                                                       opts->redundancy_max,
                                                       opts->offset) != true) {
                            // out of memory
                            tr_error("Critical - Out of memory");
                            SET_BIT(status_bit_mask, 1);;
                        } else {
                            // Everything went well, activate session and store
                            // block device options
                            _session_ctx.frag_session[frag_index].bd_opts = opts;
                            SET_BIT(_session_ctx.active_mask, frag_index);
                            tr_debug("Frag session %d activated", frag_index);
                            tr_debug("Expected Fragments %d, Fragmnet Size = %d",
                                     _session_ctx.frag_session[frag_index].nb_frag,
                                     _session_ctx.frag_session[frag_index].frag_size);
                        }
                    }
                }

                _outbound_buf[idx++] = status_bit_mask;

                break;
            }
            case FRAG_SESSION_DELETE: {
                _outbound_buf[idx++] = FRAG_SESSION_DELETE;
                uint8_t param = payload[i++] & 0x03;
                uint8_t status = 0;
                if (BIT_SET_TEST(_session_ctx.active_mask, param)) {
                    UNSET_BIT(_session_ctx.active_mask, param);
                    status = param & 0x03;
                    _session_ctx.frag_session[param].is_active = false;
                    _session_ctx.frag_session[param].mcast_group_mask = 0;
                    _session_ctx.frag_session[param].nb_frag = 0;
                    _session_ctx.frag_session[param].frag_size = 0;
                    _session_ctx.frag_session[param].frag_index = 0;
                    _session_ctx.frag_session[param].frag_algo = 0;
                    _session_ctx.frag_session[param].descriptor = 0;
                    _session_ctx.frag_session[param].padding = 0;
                    _session_ctx.frag_session[param].block_ack_delay = 0;
                } else {
                    SET_BIT(status, 2);
                }

                _outbound_buf[idx++] = status;
                break;
            }

            default:
                tr_error("Unknown FRAG control command");
                return NULL;
        }
    }

    _resp.type = FRAG_CMD_RESP;
    _resp.cmd_ans.data = _outbound_buf;
    _resp.cmd_ans.size = idx;

    return &_resp;
}
