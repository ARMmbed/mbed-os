/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager connected isochronous stream master interface file.
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

#ifndef LMGR_API_CIS_MASTER_H
#define LMGR_API_CIS_MASTER_H

#include "lmgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LMGR_API_CIS_MST
 *  \{
 */

/**************************************************************************************************
  Data Types
**************************************************************************************************/
/*! \brief      Master role device parameter definition. */
typedef struct
{
  uint8_t           maxNumCis;          /*!< Maximum number of CIS. */

  bool_t           createCisPend;       /*!< TRUE if create CIS command is pending. */

} lmgrCisMstCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/
extern lmgrCisMstCtrlBlk_t lmgrCisMstCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LmgrMstCisInit(void);

/* Status */
uint8_t LmgrMstCisGetMaxNumCis(void);

/*! \} */    /* LL_LMGR_API_CIS_MST */

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_CIS_MASTER_H */
