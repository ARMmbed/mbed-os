/*
 * Copyright (c) 2015-2018, ARM Limited, All Rights Reserved
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
 * \file type4_target.c
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

#define __DEBUG__ 0
#ifndef __MODULE__
#define __MODULE__ "type4_target.c"
#endif

#include "stack/nfc_errors.h"

#include "type4_target.h"
#include "tech/iso7816/iso7816_defs.h"

#define TYPE4_NDEF_VERSION 2

#if TYPE4_NDEF_VERSION == 2
static const uint8_t aid[] = { 0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x01 };
#else
static const uint8_t aid[] = { 0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x00 };
#endif
#define CC_FILE 0xE103 //Must not be changed
#define NDEF_FILE 0xA443
#define DEFAULT_FILE 0x0000

static void app_selected(nfc_tech_iso7816_app_t *pIso7816App, void *pUserData);
static void app_deselected(nfc_tech_iso7816_app_t *pIso7816App, void *pUserData);
static void app_apdu(nfc_tech_iso7816_app_t *pIso7816App, void *pUserData);

static nfc_err_t data_read(nfc_tech_type4_target_t *pType4Target, ac_buffer_t *pBuf, uint16_t file, size_t off, size_t len);
static nfc_err_t data_write(nfc_tech_type4_target_t *pType4Target, ac_buffer_t *pBuf, uint16_t file, size_t off);

void nfc_tech_type4_target_init(nfc_tech_type4_target_t *pType4Target, nfc_tech_iso7816_t *pIso7816, ndef_msg_t *pNdef)
{
    ac_buffer_builder_init(&pType4Target->ccFileBldr, pType4Target->ccFileBuf, /*sizeof(pType4Target->ccFileBuf)*/15);

    ac_buffer_builder_init(&pType4Target->ndefFileBldr, pType4Target->ndefFileBuf, /*sizeof(pType4Target->ndefFileBuf)*/2);

    pType4Target->selFile = DEFAULT_FILE;
    pType4Target->pNdef = pNdef;
    pType4Target->written = false;

    nfc_tech_iso7816_app_init(&pType4Target->app, pIso7816, aid, sizeof(aid), app_selected, app_deselected, app_apdu, pType4Target);

    nfc_tech_iso7816_add_app(pIso7816, &pType4Target->app);
}

void app_selected(nfc_tech_iso7816_app_t *pIso7816App, void *pUserData)
{
    nfc_tech_type4_target_t *pType4Target = (nfc_tech_type4_target_t *) pUserData;
    NFC_DBG("Selected");

    (void) pIso7816App;

    ac_buffer_builder_reset(ndef_msg_buffer_builder(pType4Target->pNdef));

    //Populate CC file
    ac_buffer_builder_reset(&pType4Target->ccFileBldr);
    ac_buffer_builder_write_nu16(&pType4Target->ccFileBldr, 15);   //CC file is 15 bytes long
#if TYPE4_NDEF_VERSION == 2
    ac_buffer_builder_write_nu8(&pType4Target->ccFileBldr, 0x20);   //NFC Forum Tag Type 4 V2.0 compliant
#else
    ac_buffer_builder_write_nu8(&pType4Target->ccFileBldr, 0x10);   //NFC Forum Tag Type 4 V1.0 compliant
#endif
    ac_buffer_builder_write_nu16(&pType4Target->ccFileBldr, 256 /* Max frame size */ - 2 /* SW */ - 3 /* ISO-DEP PFB + DID + NAD */);   //Max data size that can be read from the tag
    ac_buffer_builder_write_nu16(&pType4Target->ccFileBldr, 256 /* Max frame size */ - 6 /* CLA INS P1 P2 LC LE */ - 3 /* ISO-DEP PFB + DID + NAD */);   //Max data size that can be written to the tag
    ac_buffer_builder_write_nu8(&pType4Target->ccFileBldr, 0x04);   //NDEF File Control TLV - Type
    ac_buffer_builder_write_nu8(&pType4Target->ccFileBldr, 6);   //NDEF File Control TLV - Length
    ac_buffer_builder_write_nu16(&pType4Target->ccFileBldr, NDEF_FILE);   //NDEF file id
    ac_buffer_builder_write_nu16(&pType4Target->ccFileBldr, 2 /* length header */ + ac_buffer_builder_writable(ndef_msg_buffer_builder(pType4Target->pNdef)));     //Max size of NDEF data
    ac_buffer_builder_write_nu8(&pType4Target->ccFileBldr, 0x00);   //Open read access
    ac_buffer_builder_write_nu8(&pType4Target->ccFileBldr, 0x00);   //Open write access

    //Encode NDEF file
    ndef_msg_encode(pType4Target->pNdef);

    //Populate NDEF file
    ac_buffer_builder_init(&pType4Target->ndefFileBldr, pType4Target->ndefFileBuf, /*sizeof(pType4Target->ndefFileBuf)*/2);

    ac_buffer_builder_write_nu16(&pType4Target->ndefFileBldr, ac_buffer_reader_readable(ac_buffer_builder_buffer(ndef_msg_buffer_builder(pType4Target->pNdef))));

    //Pad NDEF file with 0s
    while (ac_buffer_builder_writable(ndef_msg_buffer_builder(pType4Target->pNdef)) > 0) {
        ac_buffer_builder_write_nu8(ndef_msg_buffer_builder(pType4Target->pNdef), 0);
    }

    //No file selected
    pType4Target->selFile = DEFAULT_FILE;

    pType4Target->written = false;
}

void app_deselected(nfc_tech_iso7816_app_t *pIso7816App, void *pUserData)
{
    nfc_tech_type4_target_t *pType4Target = (nfc_tech_type4_target_t *) pUserData;

    (void) pIso7816App;

    //Reset buffers
    ac_buffer_builder_reset(&pType4Target->ccFileBldr);
    ac_buffer_builder_set_full(&pType4Target->ndefFileBldr); //To read length
    ac_buffer_builder_reset(ndef_msg_buffer_builder(pType4Target->pNdef));

    NFC_DBG("Deselected");

    if (pType4Target->written) {
        NFC_DBG("New content has been written");
        //Try to parse NDEF
        //Set buffer length based on file header
        size_t length = ac_buffer_read_nu16(ac_buffer_builder_buffer(&pType4Target->ndefFileBldr));
        NFC_DBG("Length is %lu", length);
        if (length < ac_buffer_builder_writable(ndef_msg_buffer_builder(pType4Target->pNdef))) {
            ac_buffer_builder_set_write_offset(ndef_msg_buffer_builder(pType4Target->pNdef), length);
            ndef_msg_decode(pType4Target->pNdef);
        } else {
            NFC_ERR("Invalid length");
        }
    }
}

void app_apdu(nfc_tech_iso7816_app_t *pIso7816App, void *pUserData)
{
    nfc_tech_type4_target_t *pType4Target = (nfc_tech_type4_target_t *) pUserData;

    //Reset buffers
    ac_buffer_builder_set_full(&pType4Target->ccFileBldr);
    ac_buffer_builder_set_full(&pType4Target->ndefFileBldr);
    ac_buffer_builder_set_full(ndef_msg_buffer_builder(pType4Target->pNdef));   //Set offset to 0, size to max

    ac_buffer_set_next(ac_buffer_builder_buffer(&pType4Target->ndefFileBldr), ac_buffer_builder_buffer(ndef_msg_buffer_builder(pType4Target->pNdef)));

    //Recover PDU
    nfc_tech_iso7816_c_apdu_t *pCApdu = nfc_tech_iso7816_app_c_apdu(pIso7816App);
    nfc_tech_iso7816_r_apdu_t *pRApdu = nfc_tech_iso7816_app_r_apdu(pIso7816App);

    nfc_err_t ret;
    switch (pCApdu->ins) {
        case ISO7816_INS_SELECT:
            switch (pCApdu->p1) {
                case 0x00: //Selection by ID
                case 0x02: //Selection by child ID
                    if (ac_buffer_reader_readable(&pCApdu->dataIn) != 2) {
                        pRApdu->sw = ISO7816_SW_NOT_FOUND;
                        break;
                    }

                    uint16_t file = ac_buffer_read_nu16(&pCApdu->dataIn);
                    if (file == NDEF_FILE) {
                        pType4Target->selFile = NDEF_FILE;
                        NFC_DBG("NDEF File selected");
                        pRApdu->sw = ISO7816_SW_OK;
                    } else if (file == CC_FILE) {
                        pType4Target->selFile = CC_FILE;
                        NFC_DBG("CC File selected");
                        pRApdu->sw = ISO7816_SW_OK;
                    } else {
                        //file = DEFAULT_FILE;
                        NFC_DBG("Could not select file %04X", file);
                        pRApdu->sw = ISO7816_SW_NOT_FOUND;
                    }
                    break;
                default:
                    pRApdu->sw = ISO7816_SW_NOT_FOUND;
                    break;
            }
            break;
        case 0xB0: //Read binary
            NFC_DBG("Trying to read %d bytes at offset %d from file %04x", pCApdu->maxRespLength, (pCApdu->p1 << 8) | pCApdu->p2, pType4Target->selFile);
            ret = data_read(pType4Target, &pRApdu->dataOut, pType4Target->selFile, (pCApdu->p1 << 8) | pCApdu->p2, pCApdu->maxRespLength);
            if (ret == NFC_OK) {
                NFC_DBG("Read %d bytes", ac_buffer_reader_readable(&pRApdu->dataOut));
                NFC_DBG_BLOCK(ac_buffer_dump(&pRApdu->dataOut);)
                pRApdu->sw = ISO7816_SW_OK;
            } else {
                NFC_DBG("Failed with ret code %d", ret);
                pRApdu->sw = ISO7816_SW_WRONG_LENGTH;
            }
            break;
        case 0xD6: //Update binary
            NFC_DBG("Trying to write %d bytes at offset %d to file %04x", ac_buffer_reader_readable(&pCApdu->dataIn), (pCApdu->p1 << 8) | pCApdu->p2, pType4Target->selFile);
            ret = data_write(pType4Target, &pCApdu->dataIn, pType4Target->selFile, (pCApdu->p1 << 8) | pCApdu->p2);
            if (ret == NFC_OK) {
                NFC_DBG("OK");
                pRApdu->sw = ISO7816_SW_OK;
                pType4Target->written = true;
            } else {
                NFC_DBG("Failed with ret code %d", ret);
                pRApdu->sw = ISO7816_SW_WRONG_LENGTH;
            }
            break;
        default:
            pRApdu->sw = ISO7816_SW_INVALID_INS;
            break;
    }

    //Send reply
    nfc_tech_iso7816_app_reply(pIso7816App);
}

nfc_err_t data_read(nfc_tech_type4_target_t *pType4Target, ac_buffer_t *pBuf, uint16_t file, size_t off, size_t len)
{
    ac_buffer_t *pFile;
    switch (file) {
        case CC_FILE:
            pFile = ac_buffer_builder_buffer(&pType4Target->ccFileBldr);
            break;
        case NDEF_FILE:
            pFile = ac_buffer_builder_buffer(&pType4Target->ndefFileBldr);
            break;
        default:
            return NFC_ERR_NOT_FOUND;
    }

    if (off > ac_buffer_reader_readable(pFile)) {
        return NFC_ERR_LENGTH;
    }

    ac_buffer_read_n_skip(pFile, off);

    if (len > ac_buffer_reader_readable(pFile)) {
        len = ac_buffer_reader_readable(pFile);
    }

    ac_buffer_split(pBuf, pFile, pFile, len);

    return NFC_OK;
}

nfc_err_t data_write(nfc_tech_type4_target_t *pType4Target, ac_buffer_t *pBuf, uint16_t file, size_t off)
{
    ac_buffer_t *pFile;
    switch (file) {
        case NDEF_FILE:
            pFile = ac_buffer_builder_buffer(&pType4Target->ndefFileBldr);
            break;
        case CC_FILE: //Cannot write to CC file!
        default:
            return NFC_ERR_NOT_FOUND;
    }

    size_t len = ac_buffer_reader_readable(pBuf);

    if (off > ac_buffer_reader_readable(pFile)) {
        return NFC_ERR_LENGTH;
    }

    ac_buffer_read_n_skip(pFile, off);

    if (len > ac_buffer_reader_readable(pFile)) {
        len = ac_buffer_reader_readable(pFile);
    }

    while (len > 0) {
        size_t cpy;
        ac_buffer_builder_t builder;
        ac_buffer_dup(ac_buffer_builder_buffer(&builder), pFile);
        ac_buffer_builder_from_buffer(&builder);
        cpy = ac_buffer_builder_writable(&builder);
        cpy = MIN(cpy, len);
        ac_buffer_builder_copy_n_bytes(&builder, pBuf, cpy);
        pFile = ac_buffer_next(pFile);
        len -= cpy;
    }

    return NFC_OK;
}


