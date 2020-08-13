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
 * \file iso7816_app.c
 * \copyright Copyright (c) ARM Ltd 2015-2018
 * \author Donatien Garnier
 */

#include "iso7816_app.h"

void nfc_tech_iso7816_app_init(nfc_tech_iso7816_app_t *pIso7816App,
                               nfc_tech_iso7816_t *pIso7816,
                               const uint8_t *aid, size_t aidSize,
                               nfc_tech_iso7816_app_cb selected,
                               nfc_tech_iso7816_app_cb deselected,
                               nfc_tech_iso7816_app_cb apdu,
                               void *pUserData
                              )
{
    pIso7816App->pIso7816 = pIso7816;
    pIso7816App->aid = aid;
    pIso7816App->aidSize = aidSize;
    pIso7816App->selected = selected;
    pIso7816App->deselected = deselected;
    pIso7816App->apdu = apdu;
    pIso7816App->pUserData = pUserData;
    pIso7816App->pNext = NULL;
}
