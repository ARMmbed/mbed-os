/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  ATT optional signed PDU processing functions.
 *
 *  Copyright (c) 2011-2018 Arm Ltd. All Rights Reserved.
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
#ifndef ATT_SIGN_H
#define ATT_SIGN_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* CMAC algorithm subkey length */
#define ATT_CMAC_SUBKEY_LEN       16

/* CMAC algorithm block length */
#define ATT_CMAC_BLOCK_LEN        16

/* CMAC constant Rb */
#define ATT_CMAC_RB               0x87

/* CMAC signature result length */
#define ATT_CMAC_RESULT_LEN       8


#ifdef __cplusplus
};
#endif

#endif /* ATT_SIGN_H */
