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
 * \file pn512_hw.c
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 * \details Format and execute PN512 frames
 */

#include "stack/nfc_errors.h"

#include "pn512_hw.h"

//Platform specific
#include "platform/nfc_transport.h"



/** \addtogroup PN512
 *  \internal
 *  @{
 *  \name Hardware
 *  @{
 */

/** \internal Initialize underlying pn512_hw_t structure
 * \param pPN512 pointer to pn512_t structure
 */
void pn512_hw_init(pn512_t *pPN512)
{
    //Nothing to init in this implementation
    (void) pPN512;
}


/**
 * @}
 * @}
 * */

