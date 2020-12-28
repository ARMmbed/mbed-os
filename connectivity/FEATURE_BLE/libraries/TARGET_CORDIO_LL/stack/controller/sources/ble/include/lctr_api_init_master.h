/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller initiating master interface file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
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

#ifndef LCTR_API_INIT_MASTER_H
#define LCTR_API_INIT_MASTER_H

#include "lctr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LCTR_API_INIT_MST
 *  \{
 */

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Master initiate task messages for \a LCTR_DISP_INIT dispatcher. */
enum
{
  /* Broadcast events */
  LCTR_INIT_MSG_RESET                   = LCTR_MSG_RESET,   /*!< Reset API message. */
  /* Initiate events */
  LCTR_INIT_MSG_INITIATE,               /*!< Initiate API event. */
  LCTR_INIT_MSG_INITIATE_CANCEL,        /*!< Initiate cancel API event. */
  LCTR_INIT_MSG_TERMINATE,              /*!< Scan BOD terminated event. */
  LCTR_INIT_MSG_TOTAL                   /*!< Total number of initiate events. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/


/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrMstInitInit(void);
void LctrMstInitDefaults(void);

/* Utility */
bool_t LctrMstInitIsEnabled(void);
bool_t LctrMstInitIsPrivAddr(void);

/*! \} */    /* LL_LCTR_API_INIT_MST */

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_INIT_MASTER_H */
