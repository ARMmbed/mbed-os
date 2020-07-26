/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      BLE topology manager internal interface file.
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

#ifndef SCH_INT_TM_H
#define SCH_INT_TM_H

#include "sch_api.h"
#include "sch_api_ble.h"
#include "ll_math.h"
#include "cfg_mac_ble.h"
#include "pal_bb.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Maximum link amount. */
#define         SCH_TM_MAX_LINK                       (LL_MAX_CONN + LL_MAX_PER_SCAN + LL_MAX_CIG)

/*! \brief      Decide if given time is in the future compared to reference time. */
#define         SCH_TM_IS_IN_FUTURE(x, ref)           (BbGetTargetTimeDelta(x, ref) > 0)

/*! \brief      Decide if given time is in the past compared to reference time. */
#define         SCH_TM_IS_IN_PAST(x, ref)             (BbGetTargetTimeDelta(ref, x) > 0)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Topology link descriptor. */
typedef struct
{
  bool_t enabled;               /*!< TRUE if the link is enabled. */
  bool_t movable;               /*!< TRUE if the link is movable. */
  uint32_t interUsec;           /*!< Interval in microseconds. */
  uint32_t durUsec;             /*!< Duration in microseconds. */
  GetTopRefTimeCb_t refTimeCb;  /*!< Callback function to get reference time of the handle. */
} schTmLink_t;

/*! \brief      Topology manager control block. */
typedef struct
{
  schTmLink_t tlink[SCH_TM_MAX_LINK];       /*!< Information for each topology link. */
} SchTmCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern SchTmCb_t schTmCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

uint32_t SchTmGetFirstAnchor(uint32_t refTime, uint32_t defOffsUsec, uint32_t interUsec, uint32_t durUsec);
bool_t SchTmCheckConflict(uint32_t refBegin, uint32_t interUsec, uint32_t durUsec);

#ifdef __cplusplus
};
#endif

#endif /* SCH_INT_TM_H */
