
/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Extended advertising common header file
 *
 *  Copyright (c) 2018-2019 Arm Ltd. All Rights Reserved.
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

#ifndef LCTR_INT_ADV_AE_H
#define LCTR_INT_ADV_AE_H

#include "ll_defs.h"
#include "lctr_api.h"
#include "lctr_api_adv_acad.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief      ACAD parameter indices */
enum
{
  LCTR_ACAD_ID_CHAN_MAP_UPDATE,
  LCTR_ACAD_ID_BIG_INFO,

  /* Total number of ACAD IDs. */
  LCTR_ACAD_NUM_ID,

  /* Invalid ID type. */
  LCTR_ACAD_INVALID_ID = 0xFF
};

/*! \brief      ACAD states */
enum
{
  LCTR_ACAD_STATE_DISABLED = 0,
  LCTR_ACAD_STATE_ENABLED,
  LCTR_ACAD_STATE_TOTAL
};

/*! \brief      ACAD Common events */
enum
{
  LCTR_ACAD_COMM_MSG_START,
  LCTR_ACAD_COMM_MSG_FINISH,
  LCTR_ACAD_COMM_MSG_TOTAL,
  LCTR_ACAD_COMM_MSG_INVALID = 0xFF
};

/*! \brief      Generic ACAD packet. */
typedef union
{
  LctrAcadHdr_t        hdr;              /*!< ACAD header. */
  LctrAcadChanMapUpd_t chanMapUpdate;    /*!< Channel map update. */
  LctrAcadBigInfo_t    bigInfo;          /*!< BIG info. */
} lctrAcadParam_t;

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_ADV_AE_H */
