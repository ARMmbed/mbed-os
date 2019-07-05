
/* Copyright (c) 2019 Arm Limited
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
 * \file
 * \brief Extended advertising common header file */
/*************************************************************************************************/

#ifndef LCTR_INT_ADV_AE_H
#define LCTR_INT_ADV_AE_H

#include "ll_defs.h"
#include "lctr_api.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief      Acad parameter indicies */
enum
{
  LCTR_ACAD_ID_CHAN_MAP_UPDATE,

  /* Total number of Acad IDs. */
  LCTR_ACAD_NUM_ID,

  /* Invalid ID type. */
  LCTR_ACAD_INVALID_ID = 0xFF
};

/*! \brief      Acad states */
enum
{
  LCTR_ACAD_STATE_DISABLED = 0,
  LCTR_ACAD_STATE_ENABLED,

  LCTR_ACAD_STATE_TOTAL
};

/*! \brief      Acad Common events */
enum
{
  LCTR_ACAD_COMM_MSG_START,
  LCTR_ACAD_COMM_MSG_FINISH,

  LCTR_ACAD_COMM_MSG_TOTAL,

  LCTR_ACAD_COMM_MSG_INVALID = 0xFF
};

/*! \brief      Acad header */
typedef struct
{
  uint8_t state;        /* State of Acad. */
  uint8_t opcode;       /* Opcode of Acad. */
  uint8_t len;          /* Length of Acad data field. */
} lctrAcadHdr_t;

/*! \brief      Acad data field for channel map update */
typedef struct
{
  lctrAcadHdr_t hdr;          /* Acad header. */
  uint64_t      chanMask;     /* Channel mask for the update. */
  uint16_t      instant;      /* Instant for the update. */
} lctrAcadChanMapUpd_t;

/*! \brief      Generic Acad packet. */
typedef union
{
  lctrAcadHdr_t               hdr;              /* Acad header. */
  lctrAcadChanMapUpd_t        chanMapUpdate;    /* Channel map update. */
} lctrAcadParam_t;

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_ADV_AE_H */
