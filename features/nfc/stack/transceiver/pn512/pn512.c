/*
 * Copyright (c) 2013-2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * \file pn512.c
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 * \details PN512 implementation of the transceiver interface
 */

#define __DEBUG__ 4
#ifndef __MODULE__
#define __MODULE__ "pn512.c"
#endif
#include "stack/nfc_errors.h"

#include "stdlib.h"

#include "acore/ac_buffer.h"

#include "transceiver/transceiver.h"
#include "transceiver/protocols.h"
#include "pn512_rf.h"
#include "pn512_registers.h"
#include "pn512_cmd.h"
#include "pn512_hw.h"
#include "pn512_irq.h"
#include "pn512_poll.h"
#include "pn512_transceive.h"
#include "pn512_internal.h"

#include "pn512.h"

#define DEFAULT_READER_TRANSCEIVE_TIMEOUT 100
#define DEFAULT_TARGET_TRANSCEIVE_TIMEOUT -1


//Prototypes
#include "pn512_internal.h"

/** \addtogroup PN512
 *  @{
 *  \name Transceiver
 *  \details Implementation of the transceiver interface
 *  @{
 */

//PN 512 VTABLE

static const transceiver_impl_t pn512_impl = {
    .set_protocols = pn512_set_protocols,
    .poll = pn512_poll,
    .transceive = pn512_transceive,
    .abort = pn512_abort,
    .set_crc = pn512_set_crc,
    .set_timeout = pn512_set_timeout,
    .set_transceive_options = pn512_set_transceive_options,
    .set_transceive_framing = pn512_set_transceive_framing,
    .set_write = pn512_set_write,
    .get_read = pn512_get_read,
    .set_last_byte_length = pn512_set_last_byte_length,
    .get_last_byte_length = pn512_get_last_byte_length,
    .set_first_byte_align = pn512_set_first_byte_align,
    .close = pn512_close,
    .sleep = pn512_sleep
};

/** Initialize PN512 transceiver
 * \param pPN512 pointer to pn512_t structure to initialize
 * \param pTransport pointer to already initialized nfc_transport_t structure
 * \return NFC_OK (0) on success or NFC_ERR_* error on failure
 */
nfc_err_t pn512_init(pn512_t *pPN512, nfc_transport_t *pTransport, nfc_scheduler_timer_t *pTimer)
{
    ////
    //For Self-test
    ////
#if NFC_PN512_SELFTEST
    const uint8_t null_array[25] = {0};
#endif
    ////
    uint8_t r;

    //Init transceiver
    transceiver_init((nfc_transceiver_t *)pPN512, pTransport, pTimer);

    //Init buffer
    ac_buffer_builder_init(&pPN512->readBufBldr, pPN512->payload, 256);

    pPN512->readFirstByteAlign = 0;
    pPN512->readLastByteLength = 8;
    pPN512->writeLastByteLength = 8;

    //Populate functions
    pPN512->transceiver.fn = &pn512_impl;

    //Init variables
    memset(&pPN512->config.initiators, 0, sizeof(nfc_tech_t));
    memset(&pPN512->config.targets, 0, sizeof(nfc_tech_t));
    pPN512->timeout = -1;
    pPN512->nextFrameMode = pn512_transceive_mode_transceive;

    pn512_hw_init(pPN512);
    pn512_registers_init(pPN512); //Cannot switch page now
    pn512_cmd_init(pPN512);

    pn512_cmd_exec(pPN512, PN512_CMD_SOFTRST);
    pn512_cmd_wait_idle(pPN512, -1);

    //pn512_registers_init(pPN512);
    //Put into known state
    pn512_registers_reset(pPN512);

    pPN512->transceive.mode = pn512_transceive_mode_idle;

    pn512_irq_clear(pPN512, PN512_IRQ_ALL);
    pn512_fifo_clear(pPN512);
    pn512_cmd_exec(pPN512, PN512_CMD_IDLE);
    pn512_cmd_wait_idle(pPN512, -1);

    pn512_rf_field_switch_off(pPN512);

    //Required for polling loop
    srand(4242);

#if NFC_PN512_SELFTEST // Self test
    pn512_cmd_exec(pPN512, PN512_CMD_SOFTRST);
    pn512_cmd_wait_idle(pPN512, -1);

    const uint8_t null_array_buf[25] = {0}; //FIXME
    ac_buffer_t null_array;
    ac_buffer_init(&null_array, null_array_buf, 25);

    //Perform self test
    pn512_fifo_write(pPN512, &null_array);
    pn512_cmd_exec(pPN512, PN512_CMD_CONFIG);
    while (pn512_cmd_get(pPN512) != PN512_CMD_IDLE);
    pn512_register_write(pPN512, PN512_REG_AUTOTEST, 0x09);

    ac_buffer_init(&null_array, null_array_buf, 1);

    pn512_fifo_write(pPN512, &null_array);
    pn512_cmd_exec(pPN512, PN512_CMD_CRC);
    while (pn512_cmd_get(pPN512) != PN512_CMD_IDLE);

    DBGX_ENTER();
    NFC_DBG("Test result:");
    while (pn512_fifo_length(pPN512)) {
        ac_buffer_builder_t read_byte;
        ac_buffer_builder_init(&read_byte, null_array_buf, 1);

        pn512_fifo_read(pPN512, &read_byte);
        DBGX("%02x ", null_array_buf[0]);
    }
    DBGX("\n");
    DBGX_LEAVE();
#endif

    r = pn512_register_read(pPN512, PN512_REG_VERSION);

    NFC_DBG_BLOCK(
        NFC_DBG("PN512 version %02x", r);
    )

    if ((r != 0x82) && (r != 0xB1) && (r != 0xB2)) {
        return NFC_ERR_UNSUPPORTED; //PN512 not found
    }

    return NFC_OK;
}

/** Get pointer to nfc_transceiver_t structure
 * \param pPN512 pointer to pn512_t instance
 * \return pointer to initialized nfc_transceiver_t instance
 */
nfc_transceiver_t *pn512_get_transceiver(pn512_t *pPN512)
{
    return &pPN512->transceiver;
}

void pn512_set_protocols(nfc_transceiver_t *pTransceiver, nfc_tech_t initiators, nfc_tech_t targets, polling_options_t options)
{
    pn512_t *pPN512 = (pn512_t *) pTransceiver;
    //If different, reconfigure
    if (memcmp(&initiators, &pPN512->config.initiators, sizeof(nfc_tech_t)) || memcmp(&targets, &pPN512->config.targets, sizeof(nfc_tech_t))) {
        pPN512->config.initiators = initiators;
        if (memcmp(&targets, &pPN512->config.targets, sizeof(nfc_tech_t))) {
            pPN512->config.targets = targets;
            pn512_poll_setup(pPN512);
        }
        pTransceiver->initiator_ntarget = false;
        memset(&pTransceiver->active_tech, 0, sizeof(nfc_tech_t));
    }
    pPN512->config.options = options;
}

void pn512_poll(nfc_transceiver_t *pTransceiver)
{
    pn512_t *pPN512 = (pn512_t *) pTransceiver;
    pPN512->nextFrameMode = pn512_transceive_mode_transceive;
    pn512_poll_hw(pPN512, pn512_transceiver_callback);
}

void pn512_set_crc(nfc_transceiver_t *pTransceiver, bool crc_out, bool crc_in)
{
    pn512_t *pPN512 = (pn512_t *) pTransceiver;
    pn512_framing_crc_set(pPN512, crc_out, crc_in);
}

void pn512_set_timeout(nfc_transceiver_t *pTransceiver, int timeout)
{
    pn512_t *pPN512 = (pn512_t *) pTransceiver;
    pPN512->timeout = timeout;
}

void pn512_set_transceive_options(nfc_transceiver_t *pTransceiver, bool transmit, bool receive, bool repoll)
{
    pn512_t *pPN512 = (pn512_t *) pTransceiver;
    if (transmit && receive) {
        pPN512->nextFrameMode = pn512_transceive_mode_transceive;
    } else if (transmit && repoll) {
        pPN512->nextFrameMode = pn512_transceive_mode_transmit_and_target_autocoll;
    } else if (transmit) {
        pPN512->nextFrameMode = pn512_transceive_mode_transmit;
    } else if (receive) {
        pPN512->nextFrameMode = pn512_transceive_mode_receive;
    } else {
        pPN512->nextFrameMode = pn512_transceive_mode_target_autocoll;
    }
}

void pn512_set_transceive_framing(nfc_transceiver_t *pTransceiver, nfc_framing_t framing)
{
    pn512_t *pPN512 = (pn512_t *) pTransceiver;
    pn512_framing_set(pPN512, framing);

    //Switch NFC tech if NFC DEP
    if (pTransceiver->active_tech.nfc_nfc_dep_a
            || pTransceiver->active_tech.nfc_nfc_dep_f_212
            || pTransceiver->active_tech.nfc_nfc_dep_f_424) {
        //FIXME
        pTransceiver->active_tech.nfc_nfc_dep_a = 0;
        pTransceiver->active_tech.nfc_nfc_dep_f_212 = 0;
        pTransceiver->active_tech.nfc_nfc_dep_f_424 = 0;
        switch (framing) {
            case nfc_framing_target_a_106:
            case nfc_framing_initiator_a_106:
                pTransceiver->active_tech.nfc_nfc_dep_a = 1;
                break;
            case nfc_framing_target_f_212:
            case nfc_framing_initiator_f_212:
                pTransceiver->active_tech.nfc_nfc_dep_f_212 = 1;
                break;
            case nfc_framing_target_f_424:
            case nfc_framing_initiator_f_424:
                pTransceiver->active_tech.nfc_nfc_dep_f_424 = 1;
                break;
            default:
                break;
        }
    }
}

void pn512_set_write(nfc_transceiver_t *pTransceiver, ac_buffer_t *pWriteBuf)
{
    pn512_t *pPN512 = (pn512_t *) pTransceiver;
    if (pWriteBuf == NULL) {
        ac_buffer_init(&pPN512->writeBuf, NULL, 0);
        return;
    }
    ac_buffer_dup(&pPN512->writeBuf, pWriteBuf);
}

ac_buffer_t *pn512_get_read(nfc_transceiver_t *pTransceiver)
{
    pn512_t *pPN512 = (pn512_t *) pTransceiver;
    return ac_buffer_builder_buffer(&pPN512->readBufBldr);
}

void pn512_set_last_byte_length(nfc_transceiver_t *pTransceiver, size_t lastByteLength)
{
    pn512_t *pPN512 = (pn512_t *) pTransceiver;
    if ((lastByteLength > 8) || (lastByteLength == 0)) {
        lastByteLength = 8;
    }
    pPN512->writeLastByteLength = lastByteLength;
}

void pn512_set_first_byte_align(nfc_transceiver_t *pTransceiver, size_t firstByteAlign)
{
    pn512_t *pPN512 = (pn512_t *) pTransceiver;
    firstByteAlign &= 0x7;
    pPN512->readFirstByteAlign = firstByteAlign;
}

size_t pn512_get_last_byte_length(nfc_transceiver_t *pTransceiver)
{
    pn512_t *pPN512 = (pn512_t *) pTransceiver;
    return pPN512->readLastByteLength;
}

void pn512_transceive(nfc_transceiver_t *pTransceiver)
{
    pn512_t *pPN512 = (pn512_t *) pTransceiver;
    pn512_transceive_hw(pPN512, pPN512->nextFrameMode, pn512_transceiver_callback);
    pPN512->nextFrameMode = pn512_transceive_mode_transceive;
}

void pn512_abort(nfc_transceiver_t *pTransceiver)
{
    pn512_t *pPN512 = (pn512_t *) pTransceiver;
    nfc_scheduler_dequeue_task(&pTransceiver->scheduler, true, &pPN512->transceiver.task);
}

void pn512_close(nfc_transceiver_t *pTransceiver)
{
    //pn512_t* pPN512 = (pn512_t*) pTransceiver;
    (void) pTransceiver;
    //TODO
    return;
}

void pn512_sleep(nfc_transceiver_t *pTransceiver, bool sleep)
{
    pn512_t *pPN512 = (pn512_t *) pTransceiver;

    if (sleep) {
        pn512_register_write(pPN512, PN512_REG_COMMAND, 0x30); //Receiver off + soft power down
    } else {
        pn512_register_write(pPN512, PN512_REG_COMMAND, 0x00);
        while (pn512_register_read(pPN512, PN512_REG_COMMAND) & 0x10);
    }
}

void pn512_transceiver_callback(pn512_t *pPN512, nfc_err_t ret)
{
    transceiver_callback(&pPN512->transceiver, ret);
}

/**
 * @}
 * @}
 * */
