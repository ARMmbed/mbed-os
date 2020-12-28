/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller secure connections interface file.
 *
 *  Copyright (c) 2013-2018 ARM Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
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

#ifndef LCTR_API_SC_H
#define LCTR_API_SC_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LCTR_API_SC
 *  \{
 */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrScInit(void);

/* Execution. */
uint8_t LctrGenerateP256KeyPair(void);
uint8_t LctrGenerateDhKey(const uint8_t *pPubKey, const uint8_t *pPrivKey);
uint8_t LctrGenerateDebugDhKey(void);
uint8_t LctrSetValidatePublicKeyMode(uint8_t validateMode);

/*! \} */    /* LL_LCTR_API_SC */

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_SC_H */
