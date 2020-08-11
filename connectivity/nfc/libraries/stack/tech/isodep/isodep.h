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
 * \file isodep.h
 * \copyright Copyright (c) ARM Ltd 2014
 * \author Donatien Garnier
 */

#ifndef ISODEP_H_
#define ISODEP_H_

#include "stack/nfc_common.h"
#include "transceiver/transceiver.h"

#ifdef __cplusplus
extern "C" {
#endif

struct nfc_tech_isodep;
typedef struct nfc_tech_isodep nfc_tech_isodep_t;

typedef void (*nfc_tech_isodep_cb_t)(nfc_tech_isodep_t *pIsodep, nfc_err_t ret, void *pUserData);
typedef void (*nfc_tech_isodep_disconnected_cb)(nfc_tech_isodep_t *pIsodep, bool deselected, void *pUserData);

#ifdef __cplusplus
}
#endif

#endif /* ISODEP_H_ */
