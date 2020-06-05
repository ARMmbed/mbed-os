/*************************************************************************************************/
/*!
 *  \file   wsf_nvm.h
 *
 *  \brief  NVM service.
 *
 *  Copyright (c) 2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
 *  \brief  Read data.
 *
 *  \param  charId         charactor arrary for NVM ID.
 *
 *  \return if Read NVM successfully.
 */
/*************************************************************************************************/
static inline uint64_t WsfNvmConvertChar8to64Bit(char *charId)
{
  uint64_t retValue = 0;

  for (uint8_t i = 0; i < 8; i++)
  {
    retValue |= ((uint64_t)charId[7 - i]) << (8*i);
  }
  return retValue;
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize the WSF NVM.
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
 *  \return TRUE if NVM operation is successful, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t WsfNvmReadData(uint64_t id, uint8_t *pData, uint16_t len, WsfNvmCompEvent_t compCback);

/*************************************************************************************************/
/*!
 *  \brief  Write data.
 *
 *  \param  id         Write ID.
 *  \param  pData      Buffer to write.
 *  \param  len        Data length to write.
 *  \param  compCback  Write callback.
 *
 *  \return TRUE if NVM operation is successful, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t WsfNvmWriteData(uint64_t id, const uint8_t *pData, uint16_t len, WsfNvmCompEvent_t compCback);

/*************************************************************************************************/
/*!
 *  \brief  Erase data.
 *
 *  \param  id         Erase ID.
 *  \param  compCback  Write callback.
 *
 *  \return TRUE if NVM operation is successful, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t WsfNvmEraseData(uint64_t id, WsfNvmCompEvent_t compCback);

/*************************************************************************************************/
/*!
 *  \brief  Erase all data located in NVM storage.
 *
 *  \param  compCback          Erase callback.
 *
 *  \note   Security Risk Warning. NVM storage could be shared by multiple Apps.
 */
/*************************************************************************************************/
void WsfNvmEraseDataAll(WsfNvmCompEvent_t compCback);

/*! \} */    /* WSF_NVM_API */

#ifdef __cplusplus
};
#endif

#endif /* WSF_NVM_H */
