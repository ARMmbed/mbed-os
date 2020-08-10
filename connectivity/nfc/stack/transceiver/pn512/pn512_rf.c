/*
 * Copyright (c) 2014-2018, ARM Limited, All Rights Reserved
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
 * \file pn512_rf.c
 * \copyright Copyright (c) ARM Ltd 2014
 * \author Donatien Garnier
 */

#define __DEBUG__ 0
#ifndef __MODULE__
#define __MODULE__ "pn512_rf.c"
#endif

#include "stack/nfc_errors.h"

#include "pn512_callback.h"
#include "pn512_rf.h"
#include "pn512_registers.h"
#include "pn512_timer.h"
#include "pn512_irq.h"
#include "pn512.h"
#include "pn512_internal.h"

#include "stdlib.h" //For rand() func

#define PN512_FRAMING_REGS 6
static const uint8_t framing_registers[] = { PN512_REG_MODE, PN512_REG_TXMODE, PN512_REG_RXMODE, PN512_REG_MODGSP, PN512_REG_RXTHRESHOLD, PN512_REG_MODWIDTH };
static const uint8_t framing_registers_mode_detector[] = { 0x3B, 0x80, 0x80, 0x3F, 0x55, 0x26 };
static const uint8_t framing_registers_initiator_iso14443a_106k[] = { 0x3D, 0x80, 0x80, 0x3F, 0x55, 0x26 };
static const uint8_t framing_registers_initiator_iso14443b_106k[] = { 0x3F, 0x83, 0x83, 0x04, 0x50, 0x26 };
static const uint8_t framing_registers_target_iso14443a_106k[] = { 0x3D, 0x80, 0x80, 0x3F, 0x55, 0x26 };
static const uint8_t framing_registers_felica_212k[] = { 0x3A, 0x92, 0x92, 0x12, 0x55, 0x15 };
static const uint8_t framing_registers_felica_414k[] = { 0x3A, 0xA2, 0xA2, 0x12, 0x55, 0x0A };

nfc_err_t pn512_framing_set(pn512_t *pPN512, nfc_framing_t framing)
{
    if (framing == pPN512->framing) { //No need to do anything
        return NFC_OK;
    }

    NFC_DBG("Switching to %u", framing);

    const uint8_t *framing_registers_values;
    switch (framing) {
        case nfc_framing_target_mode_detector:
            framing_registers_values = framing_registers_mode_detector;
            break;
        case nfc_framing_target_a_106:
            framing_registers_values = framing_registers_target_iso14443a_106k;
            break;
        case nfc_framing_initiator_a_106:
            framing_registers_values = framing_registers_initiator_iso14443a_106k;
            break;
        case nfc_framing_initiator_b_106:
            framing_registers_values = framing_registers_initiator_iso14443b_106k;
            break;
        case nfc_framing_target_f_212:
        case nfc_framing_initiator_f_212:
            framing_registers_values = framing_registers_felica_212k;
            break;
        case nfc_framing_target_f_424:
        case nfc_framing_initiator_f_424:
            framing_registers_values = framing_registers_felica_414k;
            break;
        default:
            return NFC_ERR_UNSUPPORTED;
    }

    for (int i = 0; i < PN512_FRAMING_REGS; i++) {
        pn512_register_write(pPN512, framing_registers[i], framing_registers_values[i]);
    }

    pPN512->framing = framing;
    pPN512->crc.out = true;
    pPN512->crc.in = true;

    //TODO initiator: PN512_REG_MODGSP

    switch (pPN512->framing) {
        case nfc_framing_initiator_a_106:
        case nfc_framing_initiator_b_106:
        case nfc_framing_initiator_f_212:
        case nfc_framing_initiator_f_424:
            pn512_register_write(pPN512, PN512_REG_CONTROL, 0x10); //Act as initiator
            break;
        case nfc_framing_target_mode_detector:
        case nfc_framing_target_a_106:
        case nfc_framing_target_f_212:
        case nfc_framing_target_f_424:
            pn512_register_write(pPN512, PN512_REG_CONTROL, 0x00); //Act as target
            break;
        default:
            return NFC_ERR_UNSUPPORTED;
    }
#if 1
    if (
        pPN512->framing == nfc_framing_initiator_a_106
        //|| (pPN512->framing == pn512_framing_target_iso14443a_106k)
    ) {
        //Enable 100% ASK Modulation
        pn512_register_write(pPN512, PN512_REG_TXAUTO, pn512_register_read(pPN512, PN512_REG_TXAUTO) | 0x40);
    } else {
        pn512_register_write(pPN512, PN512_REG_TXAUTO, pn512_register_read(pPN512, PN512_REG_TXAUTO) & (~0x40));
    }
#endif
    return NFC_OK;
}

nfc_err_t pn512_framing_crc_set(pn512_t *pPN512, bool out, bool in)
{
    const uint8_t *framing_registers_values;
    switch (pPN512->framing) {
        case nfc_framing_target_mode_detector:
            framing_registers_values = framing_registers_mode_detector;
            break;
        case nfc_framing_target_a_106:
            framing_registers_values = framing_registers_target_iso14443a_106k;
            break;
        case nfc_framing_initiator_a_106:
            framing_registers_values = framing_registers_initiator_iso14443a_106k;
            break;
        case nfc_framing_initiator_b_106:
            framing_registers_values = framing_registers_initiator_iso14443b_106k;
            break;
        case nfc_framing_target_f_212:
        case nfc_framing_initiator_f_212:
            framing_registers_values = framing_registers_felica_212k;
            break;
        case nfc_framing_target_f_424:
        case nfc_framing_initiator_f_424:
            framing_registers_values = framing_registers_felica_414k;
            break;
        default:
            return NFC_ERR_UNSUPPORTED;
    }

    if (pPN512->crc.out != out) {
        pn512_register_write(pPN512, framing_registers[1], (framing_registers_values[1] & 0x7F) | (out ? 0x80 : 0x00)); //TXMODE
        pPN512->crc.out = out;
    }
    if (pPN512->crc.in != in) {
        pn512_register_write(pPN512, framing_registers[2], (framing_registers_values[2] & 0x7F) | (in ? 0x80 : 0x00)); //RXMODE
        pPN512->crc.in = in;
    }

    return NFC_OK;
}

nfc_err_t pn512_framing_rx_multiple_enable(pn512_t *pPN512)
{
    const uint8_t *framing_registers_values;
    switch (pPN512->framing) {
        case nfc_framing_target_mode_detector:
            framing_registers_values = framing_registers_mode_detector;
            break;
        case nfc_framing_target_a_106:
            framing_registers_values = framing_registers_target_iso14443a_106k;
            break;
        case nfc_framing_initiator_a_106:
            framing_registers_values = framing_registers_initiator_iso14443a_106k;
            break;
        case nfc_framing_initiator_b_106:
            framing_registers_values = framing_registers_initiator_iso14443b_106k;
            break;
        case nfc_framing_target_f_212:
        case nfc_framing_initiator_f_212:
            framing_registers_values = framing_registers_felica_212k;
            break;
        case nfc_framing_target_f_424:
        case nfc_framing_initiator_f_424:
            framing_registers_values = framing_registers_felica_414k;
            break;
        default:
            return NFC_ERR_UNSUPPORTED;
    }

    pn512_register_write(pPN512, framing_registers[2], (framing_registers_values[2] & 0x7F) | (pPN512->crc.in ? 0x80 : 0x00) | 0x04); //RXMODE

    return NFC_OK;
}

void pn512_rf_field_switch_off(pn512_t *pPN512)
{
    pn512_register_write(pPN512, PN512_REG_TXAUTO, 0x00);
    pn512_register_write(pPN512, PN512_REG_TXCONTROL, 0x80);
    pPN512->rf_on = false;
}

void pn512_rf_field_nfcip1_rf_collision_avoidance_complete(uint32_t events, void *pUserData)
{
    pn512_t *pPN512 = (pn512_t *) pUserData;

    uint16_t irq_res = pn512_irq_get(pPN512);

    (void) events;

    pn512_timer_stop(pPN512);
    pn512_timer_config(pPN512, false, 0, 0xffff); //Deactivate autostart
    pn512_irq_set(pPN512, PN512_IRQ_NONE);
    pn512_irq_clear(pPN512, PN512_IRQ_RF_ON | PN512_IRQ_TIMER);

    if (irq_res & PN512_IRQ_RF_ON) {
        NFC_DBG("External field on");

        //Clear TXAUTO register
        pn512_register_write(pPN512, PN512_REG_TXAUTO, 0x00);

        pPN512->rf_on = false; //External field on
        pn512_rf_callback(pPN512, NFC_OK);
        return;
    }

    //Has our RF field been switched on?
    if (pn512_register_read(pPN512, PN512_REG_TXAUTO) & 0x40) { //InitialRFOn bit is cleared automatically, if the RF field is switched on
        NFC_ERR("InitialRFOn bit still set");
        pn512_rf_callback(pPN512, NFC_ERR_UNKNOWN);
        return;
    }

    pPN512->rf_on = true; //Own field on and guard time ok

    NFC_DBG("RF field enabled");
    pn512_rf_callback(pPN512, NFC_OK);
}

void pn512_rf_field_nfcip1_rf_collision_avoidance(pn512_t *pPN512, pn512_cb_t cb)
{
    pPN512->rf.cb = cb;
    pn512_irq_clear(pPN512, PN512_IRQ_RF_ON | PN512_IRQ_TIMER);

    //If our field is switched on, Wait TIRFG according to NFC-IP1 = 5ms => 67800 clock edges = (3+1)*8475
    pn512_timer_config(pPN512, true, 3, 8475);

    pn512_irq_set(pPN512, PN512_IRQ_RF_ON /* External field switched on */
                  | PN512_IRQ_TIMER /* Timer reached 0 */);

    //Try to enable RF field in compliance with NFC-IP1
    pn512_register_write(pPN512, PN512_REG_TXAUTO, 0x0F);

    //Is external RF Field already on?
    if (pn512_register_read(pPN512, PN512_REG_STATUS1) & 0x4) {
        NFC_DBG("External field already on");
        pPN512->rf_on = false; //External field on

        //Cancel
        pn512_timer_stop(pPN512);
        pn512_timer_config(pPN512, false, 0, 0xffff); //Deactivate autostart
        pn512_irq_clear(pPN512, PN512_IRQ_RF_ON | PN512_IRQ_TIMER);
        pn512_rf_callback(pPN512, NFC_OK);
        return;
    }

    //Queue task to process IRQ
    task_init(&pPN512->transceiver.task, EVENT_HW_INTERRUPT | EVENT_TIMEOUT, -1, pn512_rf_field_nfcip1_rf_collision_avoidance_complete, pPN512);
    nfc_scheduler_queue_task(&pPN512->transceiver.scheduler, &pPN512->transceiver.task);
}

void pn512_rf_field_wait_for_external_complete_task(uint32_t events, void *pUserData)
{
    pn512_t *pPN512 = (pn512_t *) pUserData;

    NFC_DBG("%lu events", events);

    //Wake up PN512
    pn512_register_write(pPN512, PN512_REG_COMMAND, 0x00);
    while (pn512_register_read(pPN512, PN512_REG_COMMAND) & 0x10);

    pn512_irq_set(pPN512, PN512_IRQ_NONE);
    pn512_irq_clear(pPN512, PN512_IRQ_RF_ON);

    if (events & EVENT_ABORTED) {
        pn512_rf_callback(pPN512, NFC_ERR_ABORTED);
        return;
    }

    if (events & EVENT_TIMEOUT) {
        NFC_DBG("Timeout");
        pn512_rf_callback(pPN512, NFC_ERR_TIMEOUT);
        return;
    }

    NFC_DBG("On");
    pn512_rf_callback(pPN512, NFC_OK);
}

void pn512_rf_field_wait_for_external(pn512_t *pPN512, int timeout, pn512_cb_t cb)
{
    pPN512->rf.cb = cb;

    pn512_irq_clear(pPN512, PN512_IRQ_RF_ON);

    NFC_DBG("Wait for RF field to come up (timeout %d)", timeout);

    //Is external RF Field already on?
    pn512_irq_set(pPN512, PN512_IRQ_RF_ON /* External field switched on */);
    if (pn512_register_read(pPN512, PN512_REG_STATUS1) & 0x4) {
        NFC_DBG("RF field already on");
        pn512_irq_set(pPN512, PN512_IRQ_NONE);
        pn512_irq_clear(pPN512, PN512_IRQ_RF_ON);

        pn512_rf_callback(pPN512, NFC_OK);
        return;
    }

    //Send PN512 to sleep mode
    pn512_register_write(pPN512, PN512_REG_COMMAND, 0x30); //Receiver off + soft power down

    //Queue task to process IRQ
    task_init(&pPN512->transceiver.task, EVENT_HW_INTERRUPT | EVENT_TIMEOUT, timeout, pn512_rf_field_wait_for_external_complete_task, pPN512);
    nfc_scheduler_queue_task(&pPN512->transceiver.scheduler, &pPN512->transceiver.task);
}




