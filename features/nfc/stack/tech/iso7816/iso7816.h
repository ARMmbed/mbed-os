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
 * \file iso7816.h
 * \copyright Copyright (c) ARM Ltd 2014
 * \author Donatien Garnier
 */

#ifndef ISO7816_H_
#define ISO7816_H_

#include "stack/nfc_common.h"

#include "transceiver/protocols.h"
#include "tech/isodep/isodep_target.h"

#ifdef __cplusplus
extern "C" {
#endif

struct nfc_tech_iso7816_c_apdu {
    uint8_t cla;
    uint8_t ins;
    uint8_t p1;
    uint8_t p2;
    ac_buffer_t dataIn;
    size_t maxRespLength;
};

struct nfc_tech_iso7816_r_apdu {
    ac_buffer_t dataOut;
    uint16_t sw;
};

#define ISO7816_RX_BUFFER_SIZE 256

typedef struct nfc_tech_iso7816_c_apdu nfc_tech_iso7816_c_apdu_t;
typedef struct nfc_tech_iso7816_r_apdu nfc_tech_iso7816_r_apdu_t;

typedef struct nfc_tech_iso7816 nfc_tech_iso7816_t;

typedef void (*nfc_tech_iso7816_disconnected_cb)(nfc_tech_iso7816_t *pIso7816, void *pUserData);

struct nfc_tech_iso7816_app;
typedef struct nfc_tech_iso7816_app nfc_tech_iso7816_app_t;

struct nfc_tech_iso7816 {
    nfc_tech_isodep_target_t isoDepTarget;

    nfc_tech_iso7816_app_t *pAppList;
    nfc_tech_iso7816_app_t *pSelectedApp;

    bool disconnected;

    nfc_tech_iso7816_c_apdu_t cApdu;
    nfc_tech_iso7816_r_apdu_t rApdu;

    bool responseReady;

    nfc_tech_iso7816_disconnected_cb disconnectedCb;
    void *pUserData;

    ac_buffer_t hist; //Historical bytes

    ac_istream_t inputStream;
    ac_ostream_t outputStream;

    //PDU buffer (tx)
    uint8_t txBuf[2];
    ac_buffer_builder_t txBldr;

    //Receive buffer
    uint8_t rxBuf[ISO7816_RX_BUFFER_SIZE];
    ac_buffer_builder_t rxBldr;
};

void nfc_tech_iso7816_init(nfc_tech_iso7816_t *pIso7816, nfc_transceiver_t *pTransceiver, nfc_tech_iso7816_disconnected_cb disconnectedCb, void *pUserData);
void nfc_tech_iso7816_add_app(nfc_tech_iso7816_t *pIso7816, nfc_tech_iso7816_app_t *pIso7816App);
void nfc_tech_iso7816_connect(nfc_tech_iso7816_t *pIso7816);
void nfc_tech_iso7816_disconnect(nfc_tech_iso7816_t *pIso7816);
nfc_err_t nfc_tech_iso7816_reply(nfc_tech_iso7816_t *pIso7816);

inline static nfc_tech_iso7816_c_apdu_t *nfc_tech_iso7816_c_apdu(nfc_tech_iso7816_t *pIso7816)
{
    return &pIso7816->cApdu;
}

inline static nfc_tech_iso7816_r_apdu_t *nfc_tech_iso7816_r_apdu(nfc_tech_iso7816_t *pIso7816)
{
    return &pIso7816->rApdu;
}

#ifdef __cplusplus
}
#endif

#endif /* ISO7816_H_ */
