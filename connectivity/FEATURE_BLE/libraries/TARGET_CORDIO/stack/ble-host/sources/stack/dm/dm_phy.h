/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  DM PHY module.
 *
 *  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
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
#ifndef DM_PHY_H
#define DM_PHY_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
  Data types
**************************************************************************************************/

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/**************************************************************************************************
  Function declarations
**************************************************************************************************/

/* component interface */
void dmPhyHciHandler(hciEvt_t *pEvent);

#ifdef __cplusplus
};
#endif

#endif /* DM_PHY_H */
