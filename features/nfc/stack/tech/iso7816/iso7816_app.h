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
 * \file iso7816_app.h
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */
#ifndef TECH_ISO7816_ISO7816_APP_H_
#define TECH_ISO7816_ISO7816_APP_H_

#include "stack/nfc_common.h"
#include "iso7816.h"

#ifdef __cplusplus
extern "C" {
#endif

struct nfc_tech_iso7816;
struct nfc_tech_iso7816_app;

typedef void (*nfc_tech_iso7816_app_cb)(nfc_tech_iso7816_app_t *pIso7816App, void *pUserData);

struct nfc_tech_iso7816_app {
    nfc_tech_iso7816_t *pIso7816;

    const uint8_t *aid;
    size_t aidSize;

    nfc_tech_iso7816_app_cb selected;
    nfc_tech_iso7816_app_cb deselected;
    nfc_tech_iso7816_app_cb apdu;

    void *pUserData;

    nfc_tech_iso7816_app_t *pNext;
};

void nfc_tech_iso7816_app_init(nfc_tech_iso7816_app_t *pIso7816App, nfc_tech_iso7816_t *pIso7816, const uint8_t *aid, size_t aidSize,
                               nfc_tech_iso7816_app_cb selected,
                               nfc_tech_iso7816_app_cb deselected,
                               nfc_tech_iso7816_app_cb apdu,
                               void *pUserData
                              );

inline static nfc_err_t nfc_tech_iso7816_app_reply(nfc_tech_iso7816_app_t *pIso7816App)
{
    return nfc_tech_iso7816_reply(pIso7816App->pIso7816);
}

inline static nfc_tech_iso7816_c_apdu_t *nfc_tech_iso7816_app_c_apdu(nfc_tech_iso7816_app_t *pIso7816App)
{
    return nfc_tech_iso7816_c_apdu(pIso7816App->pIso7816);
}

inline static nfc_tech_iso7816_r_apdu_t *nfc_tech_iso7816_app_r_apdu(nfc_tech_iso7816_app_t *pIso7816App)
{
    return nfc_tech_iso7816_r_apdu(pIso7816App->pIso7816);
}

#ifdef __cplusplus
}
#endif

#endif /* TECH_ISO7816_ISO7816_APP_H_ */
