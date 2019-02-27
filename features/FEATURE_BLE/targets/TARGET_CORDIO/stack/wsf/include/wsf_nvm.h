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
 *  \brief NVM service.
 */
/*************************************************************************************************/
#ifndef WSF_NVM_H
#define WSF_NVM_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WSF_NVM_API
 *  \{ */

/*! \brief      Operation completion callback. */
typedef void (*WsfNvmCompEvent_t)(bool_t status);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the WSF NVM.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfNvmInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Read data.
 *
 *  \param  id         Read ID.
 *  \param  pData      Buffer to read to.
 *  \param  len        Data length to read.
 *  \param  compCback  Read callback.
 *
 *  \return if Read NVM successfully.
 */
/*************************************************************************************************/
bool_t WsfNvmReadData(uint32_t id, uint8_t *pData, uint16_t len, WsfNvmCompEvent_t compCback);

/*************************************************************************************************/
/*!
 *  \brief  Write data.
 *
 *  \param  id         Write ID.
 *  \param  pData      Buffer to write.
 *  \param  len        Data length to write.
 *  \param  compCback  Write callback.
 *
 *  \return if write NVM successfully.
 */
/*************************************************************************************************/
bool_t WsfNvmWriteData(uint32_t id, const uint8_t *pData, uint16_t len, WsfNvmCompEvent_t compCback);

/*************************************************************************************************/
/*!
 *  \brief  Erase data.
 *
 *  \param  id         Erase ID.
 *  \param  compCback  Write callback.
 *
 *  \return if erase NVM successfully.
 */
/*************************************************************************************************/
bool_t WsfNvmEraseData(uint32_t id, WsfNvmCompEvent_t compCback);

/*************************************************************************************************/
/*!
 *  \brief  Erase sectors.
 *
 *  \param  numOfSectors       Number of sectors to be erased.
 *  \param  compCback          Erase callback.
 *
 *  \return if erase NVM successfully.
 */
/*************************************************************************************************/
void WsfNvmEraseSector(uint32_t numOfSectors, WsfNvmCompEvent_t compCback);

/*! \} */    /* WSF_NVM_API */

#ifdef __cplusplus
};
#endif

#endif /* WSF_NVM_H */
