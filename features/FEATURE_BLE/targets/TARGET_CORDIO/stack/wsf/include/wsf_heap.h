/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief Buffer heap service.
 */
/*************************************************************************************************/
#ifndef WSF_HEAP_H
#define WSF_HEAP_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WSF_HEAP_API
 *  \{ */

/*************************************************************************************************/
/*!
 *  \brief      Get heap available.
 *
 *  \return     Number of bytes of heap memory available.
 */
/*************************************************************************************************/
uint32_t WsfHeapCountAvailable(void);

/*************************************************************************************************/
/*!
 *  \brief      Get heap used.
 *
 *  \return     Number of bytes of heap memory used.
 */
/*************************************************************************************************/
uint32_t WsfHeapCountUsed(void);

/*************************************************************************************************/
/*!
 *  \brief      Reserve heap memory.
 *
 *  \param      size    Number of bytes of heap memory used.
 *
 *  \return     None
 */
/*************************************************************************************************/
void WsfHeapAlloc(uint32_t size);

/*************************************************************************************************/
/*!
 *  \brief      Get next available heap memory.
 *
 *  \return     Address of the start of heap memory.
 */
/*************************************************************************************************/
void *WsfHeapGetFreeStartAddress(void);

/*! \} */    /* WSF_HEAP_API */

#ifdef __cplusplus
};
#endif

#endif /* WSF_HEAP_H */

