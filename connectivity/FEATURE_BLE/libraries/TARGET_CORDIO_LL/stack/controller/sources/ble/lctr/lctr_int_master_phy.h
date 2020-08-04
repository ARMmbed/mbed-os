/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller PHY features (master) interface file.
 *
 *  Copyright (c) 2016-2017 ARM Ltd. All Rights Reserved.
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

#ifndef LCTR_INT_PHY_MASTER_H
#define LCTR_INT_PHY_MASTER_H

#include "lctr_int.h"
#include "lctr_int_conn.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* State machine */
bool_t lctrMstLlcpExecutePhyUpdateSm(lctrConnCtx_t *pCtx, uint8_t event);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_PHY_MASTER_H */
