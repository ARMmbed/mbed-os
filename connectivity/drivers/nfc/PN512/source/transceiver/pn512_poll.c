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
 * \file pn512_poll.c
 * \copyright Copyright (c) ARM Ltd 2014
 * \author Donatien Garnier
 */

#define __DEBUG__ 0
#ifndef __MODULE__
#define __MODULE__ "pn512_poll.c"
#endif

#include "stack/nfc_errors.h"

#include "pn512.h"
#include "pn512_poll.h"
#include "pn512_transceive.h"
#include "pn512_registers.h"
#include "pn512_rf.h"
#include "pn512_cmd.h"
#include "pn512_internal.h"

#define TIMEOUT 1000

static void pn512_target_anticollision(pn512_t *pPN512, pn512_cb_t cb);
static void pn512_initiator_isoa_anticollision(pn512_t *pPN512, pn512_cb_t cb);

static inline bool pn512_config_initiator(pn512_t *pPN512)
{
    return (pPN512->config.initiators.nfc_iso_dep_a | pPN512->config.initiators.nfc_iso_dep_b |
            pPN512->config.initiators.nfc_nfc_dep_a | pPN512->config.initiators.nfc_nfc_dep_f_212 | pPN512->config.initiators.nfc_nfc_dep_f_424 |
            pPN512->config.initiators.nfc_type1 | pPN512->config.initiators.nfc_type2 | pPN512->config.initiators.nfc_type3) != 0;
}

static inline bool pn512_config_target(pn512_t *pPN512)
{
    return (pPN512->config.targets.nfc_iso_dep_a | pPN512->config.targets.nfc_iso_dep_b |
            pPN512->config.targets.nfc_nfc_dep_a | pPN512->config.targets.nfc_nfc_dep_f_212 | pPN512->config.initiators.nfc_nfc_dep_f_424 |
            pPN512->config.targets.nfc_type1 | pPN512->config.targets.nfc_type2 | pPN512->config.targets.nfc_type3) != 0;
}

void pn512_target_anticollision_complete(pn512_t *pPN512, nfc_err_t ret)
{

    bool iso14443a = pPN512->config.targets.nfc_type2 || pPN512->config.targets.nfc_iso_dep_a || pPN512->config.targets.nfc_nfc_dep_a; //We do not support type 1 card emulation so irrelevant
    bool felica = pPN512->config.targets.nfc_type3 || pPN512->config.targets.nfc_nfc_dep_f_212 || pPN512->config.targets.nfc_nfc_dep_f_424;

    nfc_transceiver_t *pTransceiver = &pPN512->transceiver;
    if (ret) {
        NFC_WARN("Returned %d", ret);
        pn512_anticollision_callback(pPN512, ret);
        return;
    }

    //Data available in FIFO
    if (pPN512->readLastByteLength != 8) { //We should receive a full byte
        NFC_WARN("Not enough data in FIFO");
        pn512_anticollision_callback(pPN512, NFC_ERR_PROTOCOL);
        return;
    }

    //If successful, update state machine
    if (iso14443a && felica) {
        //Update current protocol accordingly
        uint8_t txmode = pn512_register_read(pPN512, PN512_REG_TXMODE);
        if ((txmode & 0x03) == 0x00) {
            pn512_framing_set(pPN512, nfc_framing_target_a_106);

            NFC_DBG("A 106");
            felica = false;
        } else if ((txmode & 0x03) == 0x02) {
            if ((txmode & 0x70) == 0x20) {
                //424kbps
                NFC_DBG("F 424");
                pn512_framing_set(pPN512, nfc_framing_target_f_424);
            } else {
                //212kbps
                NFC_DBG("F 212");
                pn512_framing_set(pPN512, nfc_framing_target_f_212);
            }
            iso14443a = false;
        } else {
            //Unsupported mode, exit
            pn512_anticollision_callback(pPN512, NFC_ERR_UNSUPPORTED);
            return;
        }
    }

    if (iso14443a) {
        if (ac_buffer_reader_readable(ac_buffer_builder_buffer(&pPN512->readBufBldr)) == 0) {
            pn512_anticollision_callback(pPN512, NFC_ERR_PROTOCOL);
            return;
        }

        //Halt device, so that if anticollision is restarted it's in the correct state (cleared automatically by RF reset)
        pn512_register_write(pPN512, PN512_REG_MIFNFC, 0x62 | 0x04);

        //Copy buffer to peek
        ac_buffer_t readBufDup;
        ac_buffer_dup(&readBufDup, ac_buffer_builder_buffer(&pPN512->readBufBldr));

        uint8_t b0 = ac_buffer_read_nu8(&readBufDup);

        //Get first byte
        //Read FIFO to see if the target was selected as NFC-DEP, ISO-DEP or proprietary (NFC Type 2)
        //F0 --> NFC-DEP
        //E0 --> ISO-DEP
        //Anything else --> NFC Type 2

        //First check if this could be NFC-DEP
        if (pPN512->config.targets.nfc_nfc_dep_a && (b0 == 0xF0)) {
            pTransceiver->active_tech.nfc_nfc_dep_a = true;
        } else if (pPN512->config.targets.nfc_iso_dep_a && (b0 == 0xE0)) {
            pTransceiver->active_tech.nfc_iso_dep_a = true;
        } else if (pPN512->config.targets.nfc_type2) {
            pTransceiver->active_tech.nfc_type2 = true;
        } else {
            //Unknown tech, return error
            pn512_anticollision_callback(pPN512, NFC_ERR_UNSUPPORTED);
            return;
        }

        //Give control to higher layer
        pn512_anticollision_callback(pPN512, NFC_OK);
        return;
    } else if (felica) {
        //First check if this could be NFC-DEP
        if ((pPN512->config.targets.nfc_nfc_dep_f_212 || pPN512->config.targets.nfc_nfc_dep_f_424)) {
            if (pPN512->framing == nfc_framing_target_f_424) {
                pTransceiver->active_tech.nfc_nfc_dep_f_424 = true;
            } else {
                pTransceiver->active_tech.nfc_nfc_dep_f_212 = true;
            }
        } else {
            pn512_anticollision_callback(pPN512, NFC_ERR_UNSUPPORTED);
            return;
        }
    }

    //NFC-IP 1 active mode is not supported by other devices so ignore it for now
    pn512_anticollision_callback(pPN512, NFC_OK);
}


void pn512_target_anticollision(pn512_t *pPN512, pn512_cb_t cb)
{
    pPN512->anticollision.cb = cb;

    //Reset active states
    pPN512->transceiver.initiator_ntarget = false;
    pPN512->transceiver.active_tech.nfc_type1 = pPN512->transceiver.active_tech.nfc_type2 = pPN512->transceiver.active_tech.nfc_type3
                                                                                            = pPN512->transceiver.active_tech.nfc_iso_dep_a = pPN512->transceiver.active_tech.nfc_nfc_dep_a =
                                                                                                        pPN512->transceiver.active_tech.nfc_nfc_dep_f_212 = pPN512->transceiver.active_tech.nfc_nfc_dep_f_424 = 0;

    pn512_set_timeout((nfc_transceiver_t *)pPN512, -1); //Should only fail on RF drop

    pn512_transceive_hw(pPN512, pn512_transceive_mode_target_autocoll, pn512_target_anticollision_complete);
}

// ISO A

#define ISO14443A_BUF_SIZE 8

#define REQA 0x26
#define SEL(n) (0x93 + (n)*2)
#define NVB(bits) ( (((2*8 + (bits))>>3)<<4) | ((bits) & 0x7) )
#define HALTA1 0x50
#define HALTA2 0x00
#define CT 0x88

static void pn512_initiator_isoa_anticollision_reqa(pn512_t *pPN512);
static void pn512_initiator_isoa_anticollision_atqa(pn512_t *pPN512, nfc_err_t ret);
static void pn512_initiator_isoa_anticollision_cascade_1(pn512_t *pPN512);
static void pn512_initiator_isoa_anticollision_cascade_2(pn512_t *pPN512, nfc_err_t ret);
static void pn512_initiator_isoa_anticollision_cascade_3(pn512_t *pPN512, nfc_err_t ret);
static void pn512_initiator_isoa_anticollision_cascade_4(pn512_t *pPN512, nfc_err_t ret);
static void pn512_initiator_isoa_anticollision_complete(pn512_t *pPN512);

void pn512_initiator_isoa_anticollision(pn512_t *pPN512, pn512_cb_t cb)
{
    pPN512->anticollision.cb = cb;

    // Reset transceive mode
    pPN512->transceive.mode = pn512_transceive_mode_idle;

    pn512_initiator_isoa_anticollision_reqa(pPN512);
}

void pn512_initiator_isoa_anticollision_reqa(pn512_t *pPN512)
{
    ac_buffer_builder_t *pDataOutBldr = &pPN512->readBufBldr;
    ac_buffer_builder_reset(pDataOutBldr);

    pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcA.uidLength = 0;

    ac_buffer_builder_write_nu8(pDataOutBldr, REQA);
    pn512_set_last_byte_length((nfc_transceiver_t *)pPN512, 7); //Only 7 bits in this first command
    // FIXME PN512 Anomaly: pn512_register_write(pPN512, PN512_REG_COLL, 0x00); // Set MSB to 0, to accept collisions
    pn512_set_crc((nfc_transceiver_t *)pPN512, false, false);
    pn512_set_timeout((nfc_transceiver_t *)pPN512, 4);
    pn512_set_write((nfc_transceiver_t *)pPN512, ac_buffer_builder_buffer(pDataOutBldr));

    pn512_transceive_hw(pPN512, pn512_transceive_mode_transceive, pn512_initiator_isoa_anticollision_atqa);
}

void pn512_initiator_isoa_anticollision_atqa(pn512_t *pPN512, nfc_err_t ret)
{
    // Clear collisions register
    // FIXME PN512 Anomaly: pn512_register_write(pPN512, PN512_REG_COLL, 0x80); // Set MSB to 1, to treat collisions as errors

    if (ret && (ret != NFC_ERR_COLLISION)) { // There might be collisions here
        NFC_WARN("Did not receive ATQA: error %d", ret);
        pn512_anticollision_callback(pPN512, NFC_ERR_NOPEER);
        return;
    }

    ac_buffer_t *pResp = pn512_get_read((nfc_transceiver_t *)pPN512);

    if (ac_buffer_reader_readable(pResp) != 2) {
        NFC_WARN("Wrong length (%u bytes)", ac_buffer_reader_readable(pResp));
        pn512_anticollision_callback(pPN512, NFC_ERR_PROTOCOL);
        return;
    }

    NFC_DBG("Got ATQA:");
    NFC_DBG_BLOCK(ac_buffer_dump(pResp);)

    // Ignore ATQA as there can be collisions
    ac_buffer_read_n_bytes(pResp, pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcA.atqa, 2);

    //Start cascading
    pPN512->anticollision.iso_a.cascade_level = 1;
    pPN512->anticollision.iso_a.valid_bits = 0;
    pPN512->anticollision.iso_a.more_targets = false;
    memset(pPN512->anticollision.iso_a.cln, 0, 5);

    pn512_initiator_isoa_anticollision_cascade_1(pPN512);
}

void pn512_initiator_isoa_anticollision_cascade_1(pn512_t *pPN512)
{
    ac_buffer_builder_t *pDataOutBldr = &pPN512->readBufBldr;
    ac_buffer_builder_reset(pDataOutBldr);

    //SEL
    ac_buffer_builder_write_nu8(pDataOutBldr, SEL(pPN512->anticollision.iso_a.cascade_level - 1)); //SEL: Cascade level
    ac_buffer_builder_write_nu8(pDataOutBldr, NVB(pPN512->anticollision.iso_a.valid_bits)); //First NVB: Bytecount = 2, Bitcount = 0, then adapt if collision detected

    NFC_DBG("SEL - cascade level %u,  %u valid bits - NVB %u", pPN512->anticollision.iso_a.cascade_level, pPN512->anticollision.iso_a.valid_bits, NVB(pPN512->anticollision.iso_a.valid_bits));

    if (pPN512->anticollision.iso_a.valid_bits > 0) {
        // Transmit first part of uid
        ac_buffer_builder_write_n_bytes(pDataOutBldr, pPN512->anticollision.iso_a.cln, (pPN512->anticollision.iso_a.valid_bits >> 3) + ((pPN512->anticollision.iso_a.valid_bits & 0x7) ? 1 : 0));
        pn512_set_last_byte_length((nfc_transceiver_t *)pPN512, pPN512->anticollision.iso_a.valid_bits & 0x7);
        pn512_set_first_byte_align((nfc_transceiver_t *)pPN512, pPN512->anticollision.iso_a.valid_bits & 0x7);
    }

    // FIXME PN512 Anomaly: pn512_register_write(pPN512, PN512_REG_COLL, 0x00); // Set MSB to 0, to accept collisions
    pn512_set_crc((nfc_transceiver_t *)pPN512, false, false);
    pn512_set_timeout((nfc_transceiver_t *)pPN512, 30);
    pn512_set_write((nfc_transceiver_t *)pPN512, ac_buffer_builder_buffer(pDataOutBldr));


    pn512_register_write(pPN512, PN512_REG_COMIRQ, 2);
    NFC_DBG("IRQ status %04X", ((pn512_register_read(pPN512, PN512_REG_COMIRQ))
                                | ((pn512_register_read(pPN512, PN512_REG_DIVIRQ)) << 8)));

    pn512_transceive_hw(pPN512, pn512_transceive_mode_transceive, pn512_initiator_isoa_anticollision_cascade_2);
}

void pn512_initiator_isoa_anticollision_cascade_2(pn512_t *pPN512, nfc_err_t ret)
{
    ac_buffer_t *pResp = pn512_get_read((nfc_transceiver_t *)pPN512);

    // Load & clear collisions register
    // FIXME PN512 Anomaly: pn512_register_write(pPN512, PN512_REG_COLL, 0x80); // Set MSB to 1, to treat collisions as errors

    if (ret && (ret != NFC_ERR_COLLISION)) { // There might be collisions here
        NFC_WARN("Did not receive response: error %d", ret);
        pn512_anticollision_callback(pPN512, ret);
        return;
    }

    // We should receive 5 bytes back minus all the CLn bits that we have transmitted; we ignore all bits from the first collision
    size_t expected_resp_bits = (5 << 3) - pPN512->anticollision.iso_a.valid_bits;

    // Check for collision
    uint8_t valid_bits = expected_resp_bits;
    if (ret == NFC_ERR_COLLISION) {
        uint8_t coll_reg = pn512_register_read(pPN512, PN512_REG_COLL);

        // FIXME - PN512 error
        //if( !(coll_reg & 0x20) ) // bit 5 is CollPosNotValidSet
        {
            valid_bits = (coll_reg & 0x1f);

            if (valid_bits == 0) {
                valid_bits = 32;
            }

            valid_bits--;

            NFC_DBG("Collision detected, %u valid bits", valid_bits);
            if (valid_bits < expected_resp_bits) {
                // Collision detected
                pPN512->anticollision.iso_a.more_targets = true;
            } else {
                valid_bits = expected_resp_bits;
            }
        }
    }

    size_t resp_sz = (valid_bits >> 3) + ((valid_bits & 0x7) ? 1 : 0);
    if (ac_buffer_reader_readable(pResp) < resp_sz) {
        (void) pn512_register_read(pPN512, PN512_REG_COLL);

        NFC_WARN("Wrong length (%u instead of %u - valid bits %u)", ac_buffer_reader_readable(pResp), resp_sz, valid_bits);
        pn512_anticollision_callback(pPN512, NFC_ERR_PROTOCOL);
        return;
    }

    // Read bytes
    uint8_t bufIn[5] = {0};
    ac_buffer_read_n_bytes(pResp, bufIn + (pPN512->anticollision.iso_a.valid_bits >> 3), resp_sz);

    // Mask out valid bits that are already known
    bufIn[pPN512->anticollision.iso_a.valid_bits >> 3] &= 0xff << (pPN512->anticollision.iso_a.valid_bits & 0x7);

    // Update number of valid bits
    pPN512->anticollision.iso_a.valid_bits += valid_bits;

    // Mask out bits past valid bits in last byte
    bufIn[pPN512->anticollision.iso_a.valid_bits >> 3] &= 0xff >> ((8 - pPN512->anticollision.iso_a.valid_bits) & 0x7);

    // Now remember bits before collision
    for (size_t p = 0; p < 5; p++) {
        pPN512->anticollision.iso_a.cln[p] |= bufIn[p];
    }

    // If we have all bits, then check BCC, go to next step
    if (pPN512->anticollision.iso_a.valid_bits < 5 * 8) { // Collision, add a 1 at the end of known bits to resolve collision
        pPN512->anticollision.iso_a.cln[pPN512->anticollision.iso_a.valid_bits >> 3] |= (1 << (pPN512->anticollision.iso_a.valid_bits & 0x7));
        pPN512->anticollision.iso_a.valid_bits++;

        // Restart first step with more valid bits
        pn512_initiator_isoa_anticollision_cascade_1(pPN512);
        return;
    }

    //Check BCC if all bits are valid
    if (pPN512->anticollision.iso_a.cln[4] != (pPN512->anticollision.iso_a.cln[0] ^ pPN512->anticollision.iso_a.cln[1] ^ pPN512->anticollision.iso_a.cln[2] ^ pPN512->anticollision.iso_a.cln[3])) {
        NFC_WARN("Wrong BCC %02X != %02X", bufIn[4], bufIn[0] ^ bufIn[1] ^ bufIn[2] ^ bufIn[3]);
        pn512_anticollision_callback(pPN512, NFC_ERR_COLLISION);
        return; //TODO handle this properly
    }

    if (pPN512->anticollision.iso_a.cln[0] == CT) {
        //Not the last cascade level
        if (pPN512->anticollision.iso_a.cascade_level == 3) { // not allowed
            NFC_WARN("Cascade tag present in cascade level 3");
            pn512_anticollision_callback(pPN512, NFC_ERR_PROTOCOL);
            return;
        }
        memcpy(&pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcA.uid[(pPN512->anticollision.iso_a.cascade_level - 1) * 3], &pPN512->anticollision.iso_a.cln[1], 3);
        pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcA.uidLength += 3;
    } else {
        //Last cascade level
        memcpy(&pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcA.uid[(pPN512->anticollision.iso_a.cascade_level - 1) * 3], &pPN512->anticollision.iso_a.cln[0], 4);
        pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcA.uidLength += 4;
    }

    ac_buffer_builder_t *pDataOutBldr = &pPN512->readBufBldr;
    ac_buffer_builder_reset(pDataOutBldr);

    //Select and get SAK
    ac_buffer_builder_write_nu8(pDataOutBldr, SEL(pPN512->anticollision.iso_a.cascade_level - 1));  //SEL: Cascade level
    ac_buffer_builder_write_nu8(pDataOutBldr, NVB(40));  //NVB: 40 valid bits = 5 bytes

    //Transmit last 4 transmitted UID bytes + BCC (including cascade byte if relevant)
    ac_buffer_builder_write_n_bytes(pDataOutBldr, pPN512->anticollision.iso_a.cln, 5);

    NFC_DBG("Selecting target");

    //This time compute & check CRC
    pn512_set_crc((nfc_transceiver_t *)pPN512, true, true);
    pn512_set_write((nfc_transceiver_t *)pPN512, ac_buffer_builder_buffer(pDataOutBldr));
    pn512_transceive_hw(pPN512, pn512_transceive_mode_transceive, pn512_initiator_isoa_anticollision_cascade_3);
}

static void pn512_initiator_isoa_anticollision_cascade_3(pn512_t *pPN512, nfc_err_t ret)
{
    ac_buffer_t *pResp = pn512_get_read((nfc_transceiver_t *)pPN512);

    if (ret) {
        NFC_WARN("Did not receive response: error %d", ret);
        pn512_anticollision_callback(pPN512, ret);
        return;
    }

    if (ac_buffer_reader_readable(pResp) != 1) {
        NFC_WARN("Wrong length");
        pn512_anticollision_callback(pPN512, NFC_ERR_PROTOCOL);
        return;
    }

    pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcA.sak = ac_buffer_read_nu8(pResp);
    NFC_DBG("Got SAK %02X", pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcA.sak);

    //Check SAK
    if (pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcA.sak & 0x04) {
        //Continue anticollision
        pPN512->anticollision.iso_a.cascade_level++;
        pPN512->anticollision.iso_a.valid_bits = 0;
        memset(pPN512->anticollision.iso_a.cln, 0, 5);
        pn512_initiator_isoa_anticollision_cascade_1(pPN512);
    } else {
        //Anticollision complete

        NFC_DBG("Found one target- SAK = %02X", pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcA.sak);

        //Analyze SAK
        memset(&pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].type, 0, sizeof(nfc_tech_t));

        if (pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcA.sak & 0x40) { //NFC-IP1 compliant
            pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].type.nfc_nfc_dep_a = true;
        }
        if ((pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcA.sak & 0x20)
                && pPN512->config.initiators.nfc_iso_dep_a) { //ISO-14443A-4 compliant
            pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].type.nfc_iso_dep_a = true;
        }
        if (!(pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcA.sak & 0x60)
                && pPN512->config.initiators.nfc_type2) { //Potentially NFC Type 2 (or Mifare, etc)
            pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].type.nfc_type2 = true;
        }

        // Unknown target
        if (!pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].type.nfc_iso_dep_a
                && !pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].type.nfc_nfc_dep_a
                && !pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].type.nfc_type2
           ) {
            pn512_anticollision_callback(pPN512, NFC_ERR_NOPEER);
            return;
        }

        // Valid target detected
        pPN512->transceiver.active_tech = pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].type;
        pPN512->transceiver.remote_targets_count++;

        if (!pPN512->config.options.bail_at_first_target
                && pPN512->anticollision.iso_a.more_targets
                && (pPN512->transceiver.remote_targets_count < MUNFC_MAX_REMOTE_TARGETS)) {
            // Halt target and continue with others
            ac_buffer_builder_t *pDataOutBldr = &pPN512->readBufBldr;
            ac_buffer_builder_reset(pDataOutBldr);

            //HALTA
            ac_buffer_builder_write_nu8(pDataOutBldr, HALTA1);
            ac_buffer_builder_write_nu8(pDataOutBldr, HALTA2);

            pn512_set_crc((nfc_transceiver_t *)pPN512, true, false);
            pn512_set_timeout((nfc_transceiver_t *)pPN512, 30);
            pn512_set_write((nfc_transceiver_t *)pPN512, ac_buffer_builder_buffer(pDataOutBldr));
            pn512_transceive_hw(pPN512, pn512_transceive_mode_transmit, pn512_initiator_isoa_anticollision_cascade_4);
            return;
        }

        // Leave it activated and finish!
        pn512_initiator_isoa_anticollision_complete(pPN512);
    }
}

static void pn512_initiator_isoa_anticollision_cascade_4(pn512_t *pPN512, nfc_err_t ret)
{
    if (ret) {
        NFC_WARN("Could not halt device: error %d", ret);
        pn512_anticollision_callback(pPN512, ret);
        return;
    }

    // Start again
    pn512_initiator_isoa_anticollision_reqa(pPN512);
}

void pn512_initiator_isoa_anticollision_complete(pn512_t *pPN512)
{
    pn512_anticollision_callback(pPN512, NFC_OK);
}

// ISO B
static void pn512_initiator_isob_anticollision(pn512_t *pPN512, pn512_cb_t cb);
static void pn512_initiator_isob_anticollision_reqb(pn512_t *pPN512);
static void pn512_initiator_isob_anticollision_atqb(pn512_t *pPN512, nfc_err_t ret);
static void pn512_initiator_isob_anticollision_next_slot(pn512_t *pPN512);
static void pn512_initiator_isob_anticollision_haltb_resp(pn512_t *pPN512, nfc_err_t ret);
static void pn512_initiator_isob_anticollision_complete(pn512_t *pPN512);

#define REQB 0x05
#define HALTB 0x50

void pn512_initiator_isob_anticollision(pn512_t *pPN512, pn512_cb_t cb)
{
    pPN512->anticollision.cb = cb;
    pPN512->anticollision.iso_b.slots_num_exponent = 0; // Start with one slot
    pPN512->anticollision.iso_b.slot_number = 0;
    pPN512->anticollision.iso_b.found_one = false;

    // Reset transceive mode
    pPN512->transceive.mode = pn512_transceive_mode_idle;

    pn512_initiator_isob_anticollision_reqb(pPN512);
}

void pn512_initiator_isob_anticollision_reqb(pn512_t *pPN512)
{
    ac_buffer_builder_t *pDataOutBldr = &pPN512->readBufBldr;
    ac_buffer_builder_reset(pDataOutBldr);

    if (pPN512->anticollision.iso_b.slot_number == 0) {
        // Send REQB/WUPB
        pPN512->anticollision.iso_b.more_targets = false;

        ac_buffer_builder_write_nu8(pDataOutBldr, REQB);
        ac_buffer_builder_write_nu8(pDataOutBldr, 0x00); // AFI: All card types should respond
        uint8_t wup = 0;
        if (
            pPN512->anticollision.iso_b.slots_num_exponent == 0
            //&& (pPN512->anticollision.iso_b.slot_number == 0)
        ) {
            wup |= 0x8; // Send Wake-Up command on first iteration
        }
        ac_buffer_builder_write_nu8(pDataOutBldr, wup | (pPN512->anticollision.iso_b.slots_num_exponent & 0x7)); // Param: number of slots
    } else {
        // Just send slot marker
        ac_buffer_builder_write_nu8(pDataOutBldr, REQB | ((pPN512->anticollision.iso_b.slot_number & 0xf) << 4));
    }

    pn512_set_crc((nfc_transceiver_t *)pPN512, true, true);
    pn512_set_timeout((nfc_transceiver_t *)pPN512, 18);
    pn512_set_write((nfc_transceiver_t *)pPN512, ac_buffer_builder_buffer(pDataOutBldr));
    pn512_transceive_hw(pPN512, pn512_transceive_mode_transceive, pn512_initiator_isob_anticollision_atqb);
}

void pn512_initiator_isob_anticollision_atqb(pn512_t *pPN512, nfc_err_t ret)
{
    // Three cases:
    // - 1 response --> store response, halt PICC and check next slot number
    // - No response --> check next slot number
    // - Collision --> check next slot number but we will have to increment number of slots
    ac_buffer_t *pResp = pn512_get_read((nfc_transceiver_t *)pPN512);

    if (ret && (ret != NFC_ERR_COLLISION) && (ret != NFC_ERR_WRONG_COMM) && (ret != NFC_ERR_TIMEOUT)) {
        NFC_WARN("Did not receive ATQB: error %u", ret);
        pn512_anticollision_callback(pPN512, NFC_ERR_PROTOCOL);
        return;
    }

    // Increment slot number
    pPN512->anticollision.iso_b.slot_number++;

    if ((ret == NFC_ERR_COLLISION) || (ret == NFC_ERR_WRONG_COMM)) {
        pPN512->anticollision.iso_b.more_targets = true;
    } else if (!ret) {
        pPN512->anticollision.iso_b.found_one = true;

        // Decode ATQB
        if (ac_buffer_reader_readable(pResp) != 12) {
            NFC_WARN("Wrong length (%u bytes)", ac_buffer_reader_readable(pResp));
            pn512_anticollision_callback(pPN512, NFC_ERR_PROTOCOL);
            return;
        }

        NFC_DBG("Got ATQB:");
        NFC_DBG_BLOCK(ac_buffer_dump(pResp);)

        // Check first byte
        uint8_t atqb0 = ac_buffer_read_nu8(pResp);
        if (atqb0 != 0x50) {
            NFC_WARN("Wrong first byte for ATQB: %02X", atqb0);
            pn512_anticollision_callback(pPN512, NFC_ERR_PROTOCOL);
            return;
        }

        memset(&pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].type, 0, sizeof(nfc_tech_t));
        pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].type.nfc_iso_dep_b = true;

        // Save PUPI, application data & protocol info
        ac_buffer_read_n_bytes(pResp, pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcB.pupi, 4);
        ac_buffer_read_n_bytes(pResp, pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcB.application_data, 4);
        ac_buffer_read_n_bytes(pResp, pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcB.protocol_info, 3);

        // Valid target detected
        pPN512->transceiver.active_tech = pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].type;
        pPN512->transceiver.remote_targets_count++;

        if (!pPN512->config.options.bail_at_first_target
                && (pPN512->anticollision.iso_b.more_targets || (pPN512->anticollision.iso_b.slot_number < (1 << pPN512->anticollision.iso_b.slots_num_exponent)))
                && (pPN512->transceiver.remote_targets_count < MUNFC_MAX_REMOTE_TARGETS)) {
            // Halt target and continue with others
            ac_buffer_builder_t *pDataOutBldr = &pPN512->readBufBldr;
            ac_buffer_builder_reset(pDataOutBldr);

            // Halt PICC and move on to the next slot

            //HALTB
            ac_buffer_builder_write_nu8(pDataOutBldr, HALTB);
            ac_buffer_builder_write_n_bytes(pDataOutBldr, pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count - 1].nfcB.pupi, 4);

            pn512_set_crc((nfc_transceiver_t *)pPN512, true, true);
            pn512_set_timeout((nfc_transceiver_t *)pPN512, 30);
            pn512_set_write((nfc_transceiver_t *)pPN512, ac_buffer_builder_buffer(pDataOutBldr));
            pn512_transceive_hw(pPN512, pn512_transceive_mode_transceive, pn512_initiator_isob_anticollision_haltb_resp);
            return;
        } else {
            pn512_initiator_isob_anticollision_complete(pPN512);
            return;
        }
    }

    // Move on to the next slot
    pn512_initiator_isob_anticollision_next_slot(pPN512);
}

void pn512_initiator_isob_anticollision_next_slot(pn512_t *pPN512)
{
    if (pPN512->anticollision.iso_b.slot_number >= (1 << pPN512->anticollision.iso_b.slots_num_exponent)) {
        if (!pPN512->anticollision.iso_b.more_targets) {
            // No further collisions to resolve
            pn512_initiator_isob_anticollision_complete(pPN512);
            return;
        }
        if (pPN512->anticollision.iso_b.slots_num_exponent >= 4) {
            // Cannot handle more than 16 slots
            pn512_initiator_isob_anticollision_complete(pPN512);
            return;
        }
        pPN512->anticollision.iso_b.slots_num_exponent++;
        pPN512->anticollision.iso_b.slot_number = 0;
    }
    pn512_initiator_isob_anticollision_reqb(pPN512);
}

void pn512_initiator_isob_anticollision_haltb_resp(pn512_t *pPN512, nfc_err_t ret)
{
    // Check for response
    if (ret) {
        NFC_WARN("Did not receive HALTB response: error %u", ret);
        pn512_anticollision_callback(pPN512, NFC_ERR_PROTOCOL);
        return;
    }

    ac_buffer_t *pResp = pn512_get_read((nfc_transceiver_t *)pPN512);

    if (ac_buffer_reader_readable(pResp) != 1) {
        NFC_WARN("Wrong length (%u bytes)", ac_buffer_reader_readable(pResp));
        pn512_anticollision_callback(pPN512, NFC_ERR_PROTOCOL);
        return;
    }

    NFC_DBG("Got HALTB response:");
    NFC_DBG_BLOCK(ac_buffer_dump(pResp);)

    // Check byte
    uint8_t haltbr = ac_buffer_read_nu8(pResp);
    if (haltbr != 0x00) {
        NFC_WARN("Wrong byte for HALTB response: %02X", haltbr);
        pn512_anticollision_callback(pPN512, NFC_ERR_PROTOCOL);
        return;
    }

    // PICC halted, move to next slot
    pn512_initiator_isob_anticollision_next_slot(pPN512);
}

void pn512_initiator_isob_anticollision_complete(pn512_t *pPN512)
{
    if (pPN512->anticollision.iso_b.found_one) {
        pn512_anticollision_callback(pPN512, NFC_OK);
    } else {
        pn512_anticollision_callback(pPN512, NFC_ERR_NOPEER);
    }
}

// Felica
static void pn512_initiator_felica_anticollision(pn512_t *pPN512, pn512_cb_t cb);
static void pn512_initiator_felica_anticollision_reqc(pn512_t *pPN512);
static void pn512_initiator_felica_anticollision_atqc(pn512_t *pPN512, nfc_err_t ret);
static void pn512_initiator_felica_anticollision_complete(pn512_t *pPN512);

#define REQC 0x00

void pn512_initiator_felica_anticollision(pn512_t *pPN512, pn512_cb_t cb)
{
    pPN512->anticollision.cb = cb;

    // Reset transceive mode
    pPN512->transceive.mode = pn512_transceive_mode_idle;

    pn512_initiator_felica_anticollision_reqc(pPN512);
}

void pn512_initiator_felica_anticollision_reqc(pn512_t *pPN512)
{
    ac_buffer_builder_t *pDataOutBldr = &pPN512->readBufBldr;
    ac_buffer_builder_reset(pDataOutBldr);

    ac_buffer_builder_write_nu8(pDataOutBldr, 6); //Length
    ac_buffer_builder_write_nu8(pDataOutBldr, REQC);
    ac_buffer_builder_write_nu16(pDataOutBldr, 0xFFFF); //Any system code
    ac_buffer_builder_write_nu8(pDataOutBldr, 0x00); // Padding
    ac_buffer_builder_write_nu8(pDataOutBldr, 0x07); // 8 time slots, more would overflow the rx buffer if many cards responded

    pn512_set_crc((nfc_transceiver_t *)pPN512, true, true);
    pn512_set_timeout((nfc_transceiver_t *)pPN512, 13); //8 timeslots at 212kbps
    pn512_set_write((nfc_transceiver_t *)pPN512, ac_buffer_builder_buffer(pDataOutBldr));

    pn512_framing_rx_multiple_enable(pPN512); // Set RxMultiple bit

    pn512_transceive_hw(pPN512, pn512_transceive_mode_transceive, pn512_initiator_felica_anticollision_atqc);
}

void pn512_initiator_felica_anticollision_atqc(pn512_t *pPN512, nfc_err_t ret)
{
    ac_buffer_t *pResp = pn512_get_read((nfc_transceiver_t *)pPN512);

    if (ret || (ac_buffer_reader_readable(pResp) == 0)) {
        NFC_WARN("Did not receive ATQC: error %d", ret);
        pn512_anticollision_callback(pPN512, NFC_ERR_NOPEER);
        return;
    }

    // We might have multiple responses
    NFC_DBG("Got ATQC:");
    NFC_DBG_BLOCK(ac_buffer_dump(pResp);)

    while (ac_buffer_reader_readable(pResp) > 0) {
        if (ac_buffer_reader_readable(pResp) != 18 + 1) { // ATQC is 18 bytes, 1 byte for errors added by PN512
            NFC_WARN("Wrong length (%d bytes)", ac_buffer_reader_readable(pResp));
            pn512_anticollision_callback(pPN512, NFC_ERR_PROTOCOL);
            return;
        }

        // First byte is length, check that it's correct
        uint8_t frame_length = ac_buffer_read_nu8(pResp);
        uint8_t atqc0 = ac_buffer_read_nu8(pResp);
        if ((frame_length != 18) || (atqc0 != 0x01)) {
            NFC_WARN("Wrong ATQC frame");
            pn512_anticollision_callback(pPN512, NFC_ERR_PROTOCOL);
            return;
        }

        // Read NFCID2
        ac_buffer_read_n_bytes(pResp, pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcF.nfcid2, 8);

        // Discard padding bytes
        ac_buffer_read_n_skip(pResp, 8);

        // Read error register
        uint8_t err_reg = ac_buffer_read_nu8(pResp);
        if (err_reg & 0x1f) {
            // Error within this time slot, skip
            continue;
        }

        //Populate tech accordingly
        memset(&pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].type, 0, sizeof(nfc_tech_t));
        if (
            (pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcF.nfcid2[0] == 0x01)
            && (pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].nfcF.nfcid2[1] == 0xFE)
        ) {
            // NFC-DEP supported
            pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].type.nfc_nfc_dep_f_212 = 1;
        } else {
            // Type 3 supported
            pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].type.nfc_type3 = 1;
        }
        pPN512->transceiver.active_tech = pPN512->transceiver.remote_targets[pPN512->transceiver.remote_targets_count].type;
        pPN512->transceiver.remote_targets_count++;

        // Only continue if we can have more targets
        if (!pPN512->config.options.bail_at_first_target
                && (pPN512->transceiver.remote_targets_count < MUNFC_MAX_REMOTE_TARGETS)) {
            continue;
        }
        break;
    }

    pn512_initiator_felica_anticollision_complete(pPN512);
}

void pn512_initiator_felica_anticollision_complete(pn512_t *pPN512)
{
    pn512_anticollision_callback(pPN512, NFC_OK);
}

void pn512_poll_setup(pn512_t *pPN512)
{
    bool target = pPN512->config.targets.nfc_type2
                  || pPN512->config.targets.nfc_type3
                  || pPN512->config.targets.nfc_iso_dep_a
                  || pPN512->config.targets.nfc_nfc_dep_a
                  || pPN512->config.targets.nfc_nfc_dep_f_212
                  || pPN512->config.targets.nfc_nfc_dep_f_424;

// No need for initiator-specific configuration at this stage, but keep this just in case
//  bool initiator = pPN512->config.initiators.nfc_type1
//      || pPN512->config.initiators.nfc_type2
//      || pPN512->config.initiators.nfc_type3
//      || pPN512->config.initiators.nfc_iso_dep_a
//      || pPN512->config.initiators.nfc_nfc_dep_a
//      || pPN512->config.initiators.nfc_nfc_dep_f_212
//      || pPN512->config.initiators.nfc_nfc_dep_f_424;

    if (target) {
        NFC_DBG("Configure anticoll/polling response");
        //Setup ATQA, SAK and Felica polling response
        pn512_fifo_clear(pPN512);

        ac_buffer_builder_t *pDataCfgBldr = &pPN512->readBufBldr;
        ac_buffer_builder_reset(pDataCfgBldr);

        //Write ATQA
        ac_buffer_builder_write_nu8(pDataCfgBldr, 0x04);
        ac_buffer_builder_write_nu8(pDataCfgBldr, 0x00);

        //Write NFCID1 (0s as it will be randomly generated) - first byte will be set to 0x08 by HW according to NFC-IP1
        for (int i = 0; i < 3; i++) {
            ac_buffer_builder_write_nu8(pDataCfgBldr, 0x00);
        }

        //Write SAK
        uint8_t sak = 0x00; //Default SAK (UID complete)
        if (pPN512->config.targets.nfc_iso_dep_a) {
            sak |= 0x20; //This target is ISO-14443A-4 compliant
        }
        if (pPN512->config.targets.nfc_nfc_dep_a) {
            sak |= 0x40; //This target is NFC-IP1 compliant
        }
        ac_buffer_builder_write_nu8(pDataCfgBldr, sak);

        //Write NFCID2 (xx 0xfe according to NFC-IP1 and 0s as 6 bytes will be randomly generated)
        if (pPN512->config.targets.nfc_nfc_dep_f_212 || pPN512->config.targets.nfc_nfc_dep_f_424) {
            //Byte 1 is 0x01 if supporting NFC-IP1
            ac_buffer_builder_write_nu8(pDataCfgBldr, 0x01);
        } else if (pPN512->config.targets.nfc_type3) { //NFC-IP will have priority over type 3 tag emulation
            //Byte 1 is 0x02 if supporting Type 3 tag platform
            ac_buffer_builder_write_nu8(pDataCfgBldr, 0x02);
        } else {
            ac_buffer_builder_write_nu8(pDataCfgBldr, 0x00);
        }
        ac_buffer_builder_write_nu8(pDataCfgBldr, 0xFE);

        for (int i = 0; i < 6; i++) {
            ac_buffer_builder_write_nu8(pDataCfgBldr, 0x00);
        }

        //2 PAD0 bytes (Felica spec) - this would code the manufacturer + IC code (0xFFFF for NFC-IP1 tags)
        ac_buffer_builder_write_nu8(pDataCfgBldr, 0xFF);
        ac_buffer_builder_write_nu8(pDataCfgBldr, 0xFF);

        //3 PAD1 bytes
        ac_buffer_builder_write_nu8(pDataCfgBldr, 0x00);
        ac_buffer_builder_write_nu8(pDataCfgBldr, 0x00);
        ac_buffer_builder_write_nu8(pDataCfgBldr, 0x00);

        if (!(pPN512->config.targets.nfc_nfc_dep_f_212 || pPN512->config.targets.nfc_nfc_dep_f_424)
                && pPN512->config.targets.nfc_type3) {
            ac_buffer_builder_write_nu8(pDataCfgBldr, 0x01); //MRTI Check
            ac_buffer_builder_write_nu8(pDataCfgBldr, 0x01); //MRTI Update
        } else {
            ac_buffer_builder_write_nu8(pDataCfgBldr, 0x00);
            ac_buffer_builder_write_nu8(pDataCfgBldr, 0x00);
        }

        //1 PAD2 byte
        ac_buffer_builder_write_nu8(pDataCfgBldr, 0x00);

        //2 system code bytes
        if (!(pPN512->config.targets.nfc_nfc_dep_f_212 || pPN512->config.targets.nfc_nfc_dep_f_424)
                && pPN512->config.targets.nfc_type3) {
            ac_buffer_builder_write_nu8(pDataCfgBldr, 0x12);
            ac_buffer_builder_write_nu8(pDataCfgBldr, 0xFC);
        } else {
            ac_buffer_builder_write_nu8(pDataCfgBldr, 0xFF); //Wildcard system code
            ac_buffer_builder_write_nu8(pDataCfgBldr, 0xFF);
        }

        //Write NFCID3
        ac_buffer_builder_write_nu8(pDataCfgBldr, 0x00);

        pn512_fifo_write(pPN512, ac_buffer_builder_buffer(pDataCfgBldr));

        pn512_cmd_exec(pPN512, PN512_CMD_CONFIG);
        pn512_cmd_wait_idle(pPN512, -1);

        NFC_DBG("Overwrite NFCIDs with random numbers");

        //Overwrite NFCIDs with random numbers
        pn512_cmd_exec(pPN512, PN512_CMD_RNDIDG);
        pn512_cmd_wait_idle(pPN512, -1);
    }
}

static void pn512_poll_iteration(pn512_t *pPN512, nfc_err_t ret);
static void pn512_poll_delay_complete(uint32_t events, void *pUserData)
{
    pn512_t *pPN512 = (pn512_t *) pUserData;

    if (events & EVENT_ABORTED) {
        pn512_poll_callback(pPN512, NFC_ERR_ABORTED);
        return;
    }

    pn512_poll_iteration(pPN512, NFC_OK);
}

static void pn512_poll_delay(pn512_t *pPN512, uint32_t timeout)
{
    task_init(&pPN512->transceiver.task, EVENT_TIMEOUT, timeout, pn512_poll_delay_complete, pPN512);
    nfc_scheduler_queue_task(&pPN512->transceiver.scheduler, &pPN512->transceiver.task);
}

void pn512_poll_iteration(pn512_t *pPN512, nfc_err_t ret)
{
    do {
        if (pPN512->poll.state == pn512_polling_state_start_listening) {
            NFC_DBG("pn512_polling_state_start_listening");
            // Start with listening
            if (!pn512_config_target(pPN512)) {
                // Otherwise switch to next step
                pPN512->poll.state = pn512_polling_state_start_polling;
                continue;
            }

            pPN512->poll.state = pn512_polling_state_listen_wait_for_remote_field;

            // Shortcut if target is halted (means RF field is still here)
            if (pn512_register_read(pPN512, PN512_REG_MIFNFC) & 0x04) {
                continue;
            }

            // Fix for PN512 bug that sometimes detects its own RF field
            //  if(pPN512->rf_on)
            {
                //Switch RF field off
                pn512_rf_field_switch_off(pPN512);
                NFC_DBG("RF field switched off");
            }

            NFC_DBG("Target anticollision");

            bool iso14443a = pPN512->config.targets.nfc_type2 || pPN512->config.targets.nfc_iso_dep_a || pPN512->config.targets.nfc_nfc_dep_a;
            bool felica = pPN512->config.targets.nfc_type3 || pPN512->config.targets.nfc_nfc_dep_f_212 || pPN512->config.targets.nfc_nfc_dep_f_424;

            NFC_DBG("Switch in target mode and set framing: ISO A: %s; Felica: %s", iso14443a ? "Yes" : "No", felica ? "Yes" : "No");

            //Switch in target mode
            if (iso14443a && felica) {
                pn512_framing_set(pPN512, nfc_framing_target_mode_detector);
            } else if (iso14443a) {
                pn512_framing_set(pPN512, nfc_framing_target_a_106);
            } else if (felica) {
                pn512_framing_set(pPN512, nfc_framing_target_f_212);
            }

            pn512_rf_field_wait_for_external(pPN512, pPN512->config.options.listen_for, pn512_poll_iteration);
            return;
        }

        if (pPN512->poll.state == pn512_polling_state_listen_wait_for_remote_field) {
            NFC_DBG("pn512_polling_state_listen_wait_for_remote_field");

            if (!pn512_config_target(pPN512)) {
                // Otherwise switch to next step
                pPN512->poll.state = pn512_polling_state_start_listening;
                continue;
            }

            if (ret == NFC_ERR_TIMEOUT) {
                // Continue polling
                pPN512->poll.state = pn512_polling_state_start_polling;
                continue;
            }

            if (ret) {
                pn512_poll_callback(pPN512, ret);
                return;
            }

            pPN512->poll.state = pn512_polling_state_listen_anticollision;

            pn512_target_anticollision(pPN512, pn512_poll_iteration);
            return;
        }

        if (pPN512->poll.state == pn512_polling_state_listen_anticollision) {
            NFC_DBG("pn512_polling_state_listen_anticollision");

            if (ret == NFC_ERR_FIELD) {
                // This means that a remote field was dropped - give it another chance
                pPN512->poll.state = pn512_polling_state_listen_wait_for_remote_field;
                pn512_rf_field_switch_off(pPN512);
                pn512_rf_field_wait_for_external(pPN512, pPN512->config.options.listen_for, pn512_poll_iteration);
                return;
            }

            if (ret) {
                pn512_poll_callback(pPN512, ret);
                return;
            }

            // Be safe, not sure what the framing is
            pPN512->framing = nfc_framing_unknown;

            pn512_poll_callback(pPN512, NFC_OK);
            return;
        }

        if (pPN512->poll.state == pn512_polling_state_start_polling) {
            NFC_DBG("pn512_polling_state_start_polling");

            if (!pn512_config_initiator(pPN512)) {
                // Otherwise switch to next step
                pPN512->poll.state = pn512_polling_state_start_listening;
                continue;
            }

            // Try to activate RF field
            pPN512->poll.state = pn512_polling_state_rf_collision_avoidance;

            pn512_rf_field_nfcip1_rf_collision_avoidance(pPN512, pn512_poll_iteration);
            return;
        }

        if (pPN512->poll.state == pn512_polling_state_rf_collision_avoidance) {
            NFC_DBG("pn512_polling_state_rf_collision_avoidance");

            if (ret) {
                pn512_poll_callback(pPN512, ret);
                return;
            }

            NFC_DBG("Own RF field is %s", pPN512->rf_on ? "on" : "off");

            if (!pPN512->rf_on) {
                // Go back to listening, target framing is still valid so no need to reset it
                pPN512->poll.state = pn512_polling_state_listen_wait_for_remote_field;
                continue;
            }

            pPN512->poll.state = pn512_polling_state_polling_nfc_a_start;
        }

        if (pPN512->poll.state == pn512_polling_state_polling_nfc_a_start) {
            NFC_DBG("pn512_polling_state_polling_nfc_a_start");

            //Check if ISO A is needed
            bool nfc_a = pPN512->config.initiators.nfc_type2 || pPN512->config.initiators.nfc_iso_dep_a || pPN512->config.initiators.nfc_nfc_dep_a; //We do not support type 1 card emulation so irrelevant
            if (!nfc_a) {
                // Continue with NFC B
                pPN512->poll.state = pn512_polling_state_polling_nfc_b_start;
                continue;
            }

            pPN512->transceiver.initiator_ntarget = true;
            pn512_framing_set(pPN512, nfc_framing_initiator_a_106);

            pPN512->poll.state = pn512_polling_state_polling_nfc_a_gt;
            pn512_poll_delay(pPN512, 10 + 5); // Guard time for ISO A is 5 ms
            return;
        }

        if (pPN512->poll.state == pn512_polling_state_polling_nfc_a_gt) {
            NFC_DBG("pn512_polling_state_polling_nfc_a_gt");

            // Start anticollision
            pPN512->poll.state = pn512_polling_state_polling_nfc_a_anticollision;
            pn512_initiator_isoa_anticollision(pPN512, pn512_poll_iteration);
            return;
        }

        if (pPN512->poll.state == pn512_polling_state_polling_nfc_a_anticollision) {
            NFC_DBG("pn512_polling_state_polling_nfc_a_anticollision");

            if (ret == NFC_ERR_NOPEER) {
                NFC_DBG("Not found");
                // Continue with NFC B
                pPN512->poll.state = pn512_polling_state_polling_nfc_b_start;
                continue;
            }

            if ((ret == NFC_OK) && (pPN512->config.options.bail_at_first_tech || (pPN512->transceiver.remote_targets_count == MUNFC_MAX_REMOTE_TARGETS))) {
                // At least one target found, exit polling loop
                pn512_poll_callback(pPN512, NFC_OK);
                return;
            }

            if (ret == NFC_ERR_ABORTED) {
                pn512_poll_callback(pPN512, ret);
                return;
            }

            // Continue with NFC B
            pPN512->poll.state = pn512_polling_state_polling_nfc_b_start;
            continue;
        }

        if (pPN512->poll.state == pn512_polling_state_polling_nfc_b_start) {
            NFC_DBG("pn512_polling_state_polling_nfc_b_start");

            //Check if ISO B is needed
            bool nfc_b = pPN512->config.initiators.nfc_iso_dep_b;
            if (!nfc_b) {
                // Continue with NFC F
                pPN512->poll.state = pn512_polling_state_polling_nfc_f_start;
                continue;
            }

            pPN512->transceiver.initiator_ntarget = true;
            pn512_framing_set(pPN512, nfc_framing_initiator_b_106);

            pPN512->poll.state = pn512_polling_state_polling_nfc_b_gt;
            pn512_poll_delay(pPN512, 10 + 5); // Guard time for ISO B is 5 ms
            return;
        }

        if (pPN512->poll.state == pn512_polling_state_polling_nfc_b_gt) {
            NFC_DBG("pn512_polling_state_polling_nfc_b_gt");

            // Start anticollision
            pPN512->poll.state = pn512_polling_state_polling_nfc_b_anticollision;
            pn512_initiator_isob_anticollision(pPN512, pn512_poll_iteration);
            return;
        }

        if (pPN512->poll.state == pn512_polling_state_polling_nfc_b_anticollision) {
            NFC_DBG("pn512_polling_state_polling_nfc_b_anticollision");

            if (ret == NFC_ERR_NOPEER) {
                NFC_DBG("Not found");
                // Continue with NFC F
                pPN512->poll.state = pn512_polling_state_polling_nfc_f_start;
                continue;
            }

            if ((ret == NFC_OK)
                    && (pPN512->config.options.bail_at_first_tech
                        || (pPN512->transceiver.remote_targets_count
                            == MUNFC_MAX_REMOTE_TARGETS))) {
                // At least one target found, exit polling loop
                pn512_poll_callback(pPN512, NFC_OK);
                return;
            }

            if (ret == NFC_ERR_ABORTED) {
                pn512_poll_callback(pPN512, ret);
                return;
            }

            // Continue with NFC F
            pPN512->poll.state = pn512_polling_state_polling_nfc_f_start;
            continue;
        }

        if (pPN512->poll.state == pn512_polling_state_polling_nfc_f_start) {
            NFC_DBG("pn512_polling_state_polling_nfc_f_start");

            //Check if Felica is needed
            bool nfc_f = pPN512->config.initiators.nfc_type3 || pPN512->config.initiators.nfc_nfc_dep_f_212;
            if (!nfc_f) {
                // Wrap up
                pPN512->poll.state = pn512_polling_state_finish_polling;
                continue;
            }

            pPN512->transceiver.initiator_ntarget = true;
            pn512_framing_set(pPN512, nfc_framing_initiator_f_212);

            pPN512->poll.state = pn512_polling_state_polling_nfc_f_gt;
            pn512_poll_delay(pPN512, 20); // Guard time for Felica is 20 ms
            return;
        }

        if (pPN512->poll.state == pn512_polling_state_polling_nfc_f_gt) {
            NFC_DBG("pn512_polling_state_polling_nfc_f_gt");

            // Start anticollision
            pPN512->poll.state = pn512_polling_state_polling_nfc_f_anticollision;
            pn512_initiator_felica_anticollision(pPN512, pn512_poll_iteration);
            return;
        }

        if (pPN512->poll.state == pn512_polling_state_polling_nfc_f_anticollision) {
            NFC_DBG("pn512_polling_state_polling_nfc_f_anticollision");

            if (ret == NFC_ERR_NOPEER) {
                NFC_DBG("Not found");
                // Resolve polling
                pPN512->poll.state = pn512_polling_state_finish_polling;
                continue;
            }

            if ((ret == NFC_OK) && (pPN512->config.options.bail_at_first_tech || (pPN512->transceiver.remote_targets_count == MUNFC_MAX_REMOTE_TARGETS))) {
                // At least one target found, exit polling loop
                pn512_poll_callback(pPN512, NFC_OK);
                return;
            }

            if (ret == NFC_ERR_ABORTED) {
                pn512_poll_callback(pPN512, ret);
                return;
            }

            // Resolve polling
            pPN512->poll.state = pn512_polling_state_finish_polling;
            continue;
        }

        if (pPN512->poll.state == pn512_polling_state_finish_polling) {
            if (pPN512->transceiver.remote_targets_count > 0) {
                pn512_poll_callback(pPN512, NFC_OK);
            } else {
                pn512_poll_callback(pPN512, NFC_ERR_NOPEER);
            }
            return;
        }

    } while (true);
}

// Main polling loop function
void pn512_poll_hw(pn512_t *pPN512, pn512_cb_t cb)
{
    nfc_transceiver_t *pTransceiver = (nfc_transceiver_t *)pPN512;
    pPN512->poll.cb = cb;

    //Reset active state
    pTransceiver->initiator_ntarget = false;
    memset(&pTransceiver->active_tech, 0, sizeof(nfc_tech_t));

    //Reset discovered targets
    pTransceiver->remote_targets_count = 0;

    //Initialize state machine
    pPN512->poll.state = pn512_polling_state_start_listening;

    if (!pn512_config_target(pPN512) && !pn512_config_initiator(pPN512)) {
        pn512_poll_callback(pPN512, NFC_ERR_PARAMS);
        return;
    }

    //First iteration
    pn512_poll_iteration(pPN512, NFC_OK);
}
