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
 * \file pn512_timer.c
 * \copyright Copyright (c) ARM Ltd 2014
 * \author Donatien Garnier
 */

#include "stack/nfc_errors.h"

#include "pn512_timer.h"
#include "pn512_registers.h"

void pn512_timer_config(pn512_t *pPN512, bool autostart, uint16_t prescaler, uint16_t countdown_value)
{
    pn512_timer_stop(pPN512); //just in case...

    pn512_register_write(pPN512, PN512_REG_TRELOADLOW, countdown_value & 0xFF);
    pn512_register_write(pPN512, PN512_REG_TRELOADHIGH, (countdown_value >> 8) & 0xFF);

    pn512_register_write(pPN512, PN512_REG_TPRESCALERLOW, prescaler & 0xFF);
    pn512_register_write(pPN512, PN512_REG_TMODE_TPRESCALERHIGH, (autostart ? 0x80 : 0x00) | ((prescaler >> 8) & 0x0F));
}

void pn512_timer_start(pn512_t *pPN512)
{
    //The control register also contains the initiator bit that we must set correctly
    switch (pPN512->framing) {
        case nfc_framing_initiator_a_106:
        case nfc_framing_initiator_f_212:
        case nfc_framing_initiator_f_424:
            pn512_register_write(pPN512, PN512_REG_CONTROL, 0x50);
            break;
        case nfc_framing_target_mode_detector:
        case nfc_framing_target_a_106:
        case nfc_framing_target_f_212:
        case nfc_framing_target_f_424:
        default:
            pn512_register_write(pPN512, PN512_REG_CONTROL, 0x40);
            break;
    }
}

void pn512_timer_stop(pn512_t *pPN512)
{
    //The control register also contains the initiator bit that we must set correctly
    switch (pPN512->framing) {
        case nfc_framing_initiator_a_106:
        case nfc_framing_initiator_f_212:
        case nfc_framing_initiator_f_424:
            pn512_register_write(pPN512, PN512_REG_CONTROL, 0x90);
            break;
        case nfc_framing_target_mode_detector:
        case nfc_framing_target_a_106:
        case nfc_framing_target_f_212:
        case nfc_framing_target_f_424:
        default:
            pn512_register_write(pPN512, PN512_REG_CONTROL, 0x80);
            break;
    }
}
