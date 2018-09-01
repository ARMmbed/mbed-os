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
 * \file type4_target.h
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

#ifndef TECH_TYPE4_TYPE4_TARGET_H_
#define TECH_TYPE4_TYPE4_TARGET_H_

#include "stack/nfc_common.h"

#include "tech/iso7816/iso7816.h"
#include "tech/iso7816/iso7816_app.h"
#include "ndef/ndef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nfc_tech_type4_target nfc_tech_type4_target_t;

typedef void (*nfc_tech_type4_cb)(nfc_tech_type4_target_t *pType4Target, nfc_err_t ret, void *pUserData);

struct nfc_tech_type4_target {
    nfc_tech_iso7816_app_t app;

    ndef_msg_t *pNdef;

    uint8_t ccFileBuf[15];
    ac_buffer_builder_t ccFileBldr;

    uint8_t ndefFileBuf[2];
    ac_buffer_builder_t ndefFileBldr;

    uint16_t selFile;

    bool written;
};

void nfc_tech_type4_target_init(nfc_tech_type4_target_t *pType4Target, nfc_tech_iso7816_t *pIso7816, ndef_msg_t *pNdef);

#ifdef __cplusplus
}
#endif

#endif /* TECH_TYPE4_TYPE4_TARGET_H_ */
