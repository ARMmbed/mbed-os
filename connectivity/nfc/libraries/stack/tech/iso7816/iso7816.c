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
 * \file iso7816.c
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

#define __DEBUG__ 0
#ifndef __MODULE__
#define __MODULE__ "iso7816.c"
#endif

#include "stack/nfc_errors.h"

#include "iso7816.h"
#include "iso7816_app.h"
#include "iso7816_defs.h"
#include "tech/isodep/isodep_target.h"
#include "platform/nfc_debug.h"

static void iso7816_disconnected(nfc_tech_iso7816_t *pIso7816, bool deselected);

static nfc_err_t iso7816_parse(nfc_tech_iso7816_t *pIso7816);

static void iso7816_receive(nfc_tech_iso7816_t *pIso7816);
static nfc_err_t iso7816_transmit(nfc_tech_iso7816_t *pIso7816);

static bool iso7816_mf_command(nfc_tech_iso7816_t *pIso7816);

static void iso_dep_received_cb(nfc_tech_isodep_t *pIsodep, nfc_err_t ret, void *pUserData);
static void iso_dep_transmitted_cb(nfc_tech_isodep_t *pIsodep, nfc_err_t ret, void *pUserData);
static void iso_dep_disconnected_cb(nfc_tech_isodep_t *pIsodep, bool deselected, void *pUserData);

static void iso_dep_stream_transmit_cb(ac_buffer_t *ppDataIn, bool *pClose, size_t maxLength, void *pUserParam);
static void iso_dep_stream_receive_cb(ac_buffer_t *pDataOut, bool closed, void *pUserParam);

void nfc_tech_iso7816_init(nfc_tech_iso7816_t *pIso7816, nfc_transceiver_t *pTransceiver, nfc_tech_iso7816_disconnected_cb disconnectedCb, void *pUserData)
{
    ac_buffer_init(&pIso7816->hist, NULL, 0);
    nfc_tech_isodep_target_init(&pIso7816->isoDepTarget, pTransceiver, &pIso7816->hist, iso_dep_disconnected_cb, pIso7816);
    pIso7816->pAppList = NULL;
    pIso7816->pSelectedApp = NULL;
    pIso7816->disconnectedCb = disconnectedCb;

    ac_istream_init(&pIso7816->inputStream, iso_dep_stream_transmit_cb, pIso7816);
    ac_ostream_init(&pIso7816->outputStream, iso_dep_stream_receive_cb, pIso7816);

    ac_buffer_builder_init(&pIso7816->txBldr, pIso7816->txBuf, 2); //Just enough to fit sw

    ac_buffer_builder_init(&pIso7816->rxBldr, pIso7816->rxBuf, ISO7816_RX_BUFFER_SIZE);

    pIso7816->pUserData = pUserData;
}

void nfc_tech_iso7816_connect(nfc_tech_iso7816_t *pIso7816)
{
    pIso7816->disconnected = false;
    pIso7816->responseReady = true;

    iso7816_receive(pIso7816);
    nfc_tech_isodep_target_connect(&pIso7816->isoDepTarget);
}

void nfc_tech_iso7816_disconnect(nfc_tech_iso7816_t *pIso7816)
{
    nfc_tech_isodep_target_disconnect(&pIso7816->isoDepTarget);
}

void nfc_tech_iso7816_add_app(nfc_tech_iso7816_t *pIso7816, nfc_tech_iso7816_app_t *pIso7816App)
{
    nfc_tech_iso7816_app_t **ppPrevApp = &pIso7816->pAppList;
    while (*ppPrevApp != NULL) {
        ppPrevApp = &((*ppPrevApp)->pNext);
    }
    *ppPrevApp = pIso7816App;
    pIso7816App->pNext = NULL;
}

nfc_err_t nfc_tech_iso7816_reply(nfc_tech_iso7816_t *pIso7816)
{
    nfc_err_t ret;

    //Serialize APDU and send
    ac_buffer_builder_reset(&pIso7816->txBldr);
    ac_buffer_builder_write_nu16(&pIso7816->txBldr, pIso7816->rApdu.sw);

    ac_buffer_append(&pIso7816->rApdu.dataOut, ac_buffer_builder_buffer(&pIso7816->txBldr));

    NFC_DBG("R-ADPU: (LE):%02X SW:%04X", ac_buffer_reader_readable(&pIso7816->rApdu.dataOut), pIso7816->rApdu.sw);
    NFC_DBG_BLOCK(ac_buffer_dump(&pIso7816->rApdu.dataOut);)

    ret = iso7816_transmit(pIso7816);
    if (ret) {
        return ret;
    }

    return NFC_OK;
}

void iso7816_disconnected(nfc_tech_iso7816_t *pIso7816, bool deselected)
{
    pIso7816->disconnected = true;
    if (pIso7816->pSelectedApp != NULL) {
        //Deselect previous app
        pIso7816->pSelectedApp->deselected(pIso7816->pSelectedApp, pIso7816->pSelectedApp->pUserData);
        pIso7816->pSelectedApp = NULL;
    }
    if (!deselected) {
        pIso7816->disconnectedCb(pIso7816, pIso7816->pUserData);
    }
}

nfc_err_t iso7816_parse(nfc_tech_iso7816_t *pIso7816)
{
    //Reset R-APDU
    ac_buffer_init(&pIso7816->rApdu.dataOut, NULL, 0);
    pIso7816->rApdu.sw = ISO7816_SW_OK;

    NFC_DBG_BLOCK(ac_buffer_dump(ac_buffer_builder_buffer(&pIso7816->rxBldr));)

    if (ac_buffer_reader_readable(ac_buffer_builder_buffer(&pIso7816->rxBldr)) < 4) {
        NFC_ERR("C-APDU is too small");
        pIso7816->rApdu.sw = ISO7816_SW_INVALID_CLASS;
        nfc_tech_iso7816_reply(pIso7816);
        return NFC_ERR_PROTOCOL;
    }

    pIso7816->cApdu.cla = ac_buffer_read_nu8(ac_buffer_builder_buffer(&pIso7816->rxBldr));
    pIso7816->cApdu.ins = ac_buffer_read_nu8(ac_buffer_builder_buffer(&pIso7816->rxBldr));
    pIso7816->cApdu.p1 = ac_buffer_read_nu8(ac_buffer_builder_buffer(&pIso7816->rxBldr));
    pIso7816->cApdu.p2 = ac_buffer_read_nu8(ac_buffer_builder_buffer(&pIso7816->rxBldr));
    ac_buffer_init(&pIso7816->cApdu.dataIn, NULL, 0);
    pIso7816->cApdu.maxRespLength = 0;

    if (ac_buffer_reader_readable(ac_buffer_builder_buffer(&pIso7816->rxBldr)) > 1) {
        size_t lc = ac_buffer_read_nu8(ac_buffer_builder_buffer(&pIso7816->rxBldr));
        if (ac_buffer_reader_readable(ac_buffer_builder_buffer(&pIso7816->rxBldr)) >= lc) {
            ac_buffer_split(&pIso7816->cApdu.dataIn, ac_buffer_builder_buffer(&pIso7816->rxBldr), ac_buffer_builder_buffer(&pIso7816->rxBldr), lc);
        } else {
            pIso7816->rApdu.sw = ISO7816_SW_WRONG_LENGTH;
            nfc_tech_iso7816_reply(pIso7816);
            return NFC_ERR_LENGTH; //Not a valid frame
        }
    }

    if (ac_buffer_reader_readable(ac_buffer_builder_buffer(&pIso7816->rxBldr)) >= 1) {
        pIso7816->cApdu.maxRespLength = ac_buffer_read_nu8(ac_buffer_builder_buffer(&pIso7816->rxBldr));
    }

    NFC_DBG("C-APDU: CLA:%02X INS:%02X P1:%02X P2:%02X LC:%02X LE:%02X", pIso7816->cApdu.cla, pIso7816->cApdu.ins, pIso7816->cApdu.p1, pIso7816->cApdu.p2,
            ac_buffer_reader_readable(&pIso7816->cApdu.dataIn), pIso7816->cApdu.maxRespLength);

    if (ac_buffer_reader_readable(ac_buffer_builder_buffer(&pIso7816->rxBldr)) > 0) {
        pIso7816->rApdu.sw = ISO7816_SW_WRONG_LENGTH;
        nfc_tech_iso7816_reply(pIso7816);
        return NFC_ERR_LENGTH; //Not a valid frame
    }

    //See if can select an app
    if (iso7816_mf_command(pIso7816)) {
        nfc_tech_iso7816_reply(pIso7816);
        return NFC_OK;
    }

    //Pass command to selected app
    if (pIso7816->pSelectedApp == NULL) {
        pIso7816->rApdu.sw = ISO7816_SW_NOT_FOUND;
        nfc_tech_iso7816_reply(pIso7816);
        return NFC_ERR_NOT_FOUND; //Not a valid frame
    }

    pIso7816->pSelectedApp->apdu(pIso7816->pSelectedApp, pIso7816->pSelectedApp->pUserData);

    return NFC_OK;
}

void iso7816_receive(nfc_tech_iso7816_t *pIso7816)
{
    ac_buffer_builder_reset(&pIso7816->rxBldr);
    nfc_tech_isodep_target_receive(&pIso7816->isoDepTarget, &pIso7816->outputStream, iso_dep_received_cb, pIso7816);
}

nfc_err_t iso7816_transmit(nfc_tech_iso7816_t *pIso7816)
{
    return nfc_tech_isodep_target_transmit(&pIso7816->isoDepTarget, &pIso7816->inputStream, iso_dep_transmitted_cb, pIso7816);
}

/** Handle ISO7816-4 command
 * \param pTarget pointer to target instance
 * \param CLA ISO7816-4 class byte
 * \param INS ISO7816-4 instruction byte
 * \param P1 ISO7816-4 P1 byte
 * \param P2 ISO7816-4 P2 byte
 * \param pDataIn ISO7816-4 command payload
 * \param pDataOut ISO7816-4 response payload
 * \param SW status word
 * \return true if command was handled, false if it should be passed to the selected application
 */
bool iso7816_mf_command(nfc_tech_iso7816_t *pIso7816)
{
    nfc_tech_iso7816_app_t *pApp;
    if (pIso7816->cApdu.cla != 0x00) {
        return false;
    }
    switch (pIso7816->cApdu.ins) {
        case ISO7816_INS_SELECT:
            switch (pIso7816->cApdu.p1) {
                case 0x04: //Selection by DF name
                    pApp = pIso7816->pAppList;
                    while (pApp != NULL) {
                        if (ac_buffer_reader_readable(&pIso7816->cApdu.dataIn) <= pApp->aidSize) {
                            if (ac_buffer_reader_cmp_bytes(&pIso7816->cApdu.dataIn, pApp->aid, ac_buffer_reader_readable(&pIso7816->cApdu.dataIn))) {
                                if (pIso7816->pSelectedApp != NULL) {
                                    //Deselect previous app
                                    pIso7816->pSelectedApp->deselected(pIso7816->pSelectedApp, pIso7816->pSelectedApp->pUserData);
                                }
                                pIso7816->pSelectedApp = pApp;
                                pIso7816->pSelectedApp->selected(pIso7816->pSelectedApp, pIso7816->pSelectedApp->pUserData);
                                pIso7816->rApdu.sw = ISO7816_SW_OK;
                                return true;
                            }
                        }
                        pApp = pApp->pNext;
                    }
                    pIso7816->rApdu.sw = ISO7816_SW_NOT_FOUND;
                    return true;
                default:
                    if (pIso7816->pSelectedApp == NULL) {
                        pIso7816->rApdu.sw = ISO7816_SW_NOT_FOUND;
                        return true;
                    } else {
                        return false;
                    }
            }
            break;
        default:
            if (pIso7816->pSelectedApp == NULL) {
                pIso7816->rApdu.sw = ISO7816_SW_INVALID_INS;
                return true;
            } else {
                return false;
            }
            break;
    }
}

void iso_dep_received_cb(nfc_tech_isodep_t *pIsodep, nfc_err_t ret, void *pUserData)
{
    nfc_tech_iso7816_t *pIso7816 = (nfc_tech_iso7816_t *) pUserData;

    (void) pIsodep;

    if (ret) {
        NFC_WARN("Got error %d", ret);
        return;
    }

    //Parse received APDU
    ret = iso7816_parse(pIso7816);
    if (ret) {
        NFC_WARN("Got error %d", ret);
        return;
    }
}

void iso_dep_transmitted_cb(nfc_tech_isodep_t *pIsodep, nfc_err_t ret, void *pUserData)
{
    nfc_tech_iso7816_t *pIso7816 = (nfc_tech_iso7816_t *) pUserData;

    (void) pIsodep;

    if (ret) {
        NFC_WARN("Got error %d", ret);
        return;
    }

    //Advertise that we have space in our buffer?

    //Switch to receive mode!
    iso7816_receive(pIso7816);
}

void iso_dep_disconnected_cb(nfc_tech_isodep_t *pIsodep, bool deselected, void *pUserData)
{
    nfc_tech_iso7816_t *pIso7816 = (nfc_tech_iso7816_t *) pUserData;

    (void) pIsodep;

    NFC_DBG("ISO DEP %s", deselected ? "deselected" : "disconnected");
    iso7816_disconnected(pIso7816, deselected);

    if (deselected) {
        // Re-connect immediately
        nfc_tech_iso7816_connect(pIso7816);
    }
}

void iso_dep_stream_transmit_cb(ac_buffer_t *pDataIn, bool *pClose, size_t maxLength, void *pUserParam)
{
    nfc_tech_iso7816_t *pIso7816 = (nfc_tech_iso7816_t *) pUserParam;

    //Only close if buffer fits in this frame
    if (maxLength >= ac_buffer_reader_readable(&pIso7816->rApdu.dataOut))
        //if( ac_buffer_total_length(&pLlcp->tx) <= maxLength )
    {
        maxLength = ac_buffer_reader_readable(&pIso7816->rApdu.dataOut);
        *pClose = true;
    } else {
        *pClose = false;
    }

    ac_buffer_split(pDataIn, &pIso7816->rApdu.dataOut, &pIso7816->rApdu.dataOut, maxLength);
}

void iso_dep_stream_receive_cb(ac_buffer_t *pDataOut, bool closed, void *pUserParam)
{
    nfc_tech_iso7816_t *pIso7816 = (nfc_tech_iso7816_t *) pUserParam;

    (void) closed;

    if (ac_buffer_reader_readable(pDataOut) > ac_buffer_builder_writable(&pIso7816->rxBldr)) {
        NFC_ERR("Frame will not fit (%u > %u)", ac_buffer_reader_readable(pDataOut), ac_buffer_builder_writable(&pIso7816->rxBldr));
    }

    //Feed rx buffer
    ac_buffer_builder_copy_n_bytes(&pIso7816->rxBldr, pDataOut, ac_buffer_reader_readable(pDataOut));
}

