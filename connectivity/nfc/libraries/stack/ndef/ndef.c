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
 * \file ndef.c
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 * \details NDEF tag abstraction
 */


#include "ndef.h"

/** \addtogroup NDEF
 *  @{
 *  \name Generic NDEF Tag
 *  @{
 */

/** Initialize NDEF tag abstraction
 * \param pNdefTag pointer to ndef_tag_t structure to initialize
 * \param encode function that will be called to generate the NDEF message before sending it to the other party
 * \param decode function that will be called to parse the NDEF message after receiving it from the other party
 * \param buffer underlying buffer to use (it should be big enough so that any NDEF message you might need could be stored inside)
 * \param buffer_size size of the underlying buffer
 * \param pImpl pointer to actual implementation
 */
void ndef_msg_init(ndef_msg_t *pNdef, ndef_encode_fn_t encode, ndef_decode_fn_t decode, uint8_t *data, size_t size, void *pUserData)
{
    pNdef->encode = encode;
    pNdef->decode = decode;
    ac_buffer_builder_init(&pNdef->bufferBldr, data, size);
    pNdef->pUserData = pUserData;
}

/** Get NDEF tag implementation
 * \param pNdefTag pointer to ndef_tag_t structure
 * \return implementation
 */
/*
void* ndef_tag_impl(ndef_tag_t* pNdefTag)
{
  return pNdefTag->pImpl;
}
*/

/**
 * @}
 * @}
 * */
