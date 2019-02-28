/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Internal scheduler resource manager interface file.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#ifndef SCH_INT_RM_H
#define SCH_INT_RM_H

#include "sch_api.h"
#include "ll_math.h"
#include "cfg_mac_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Maximum number of reservations per unit of preferred periodicity. */
#define SCH_RM_MAX_RSVN_PREF_PER    2           /* TODO allow run-time configuration */

/*! \brief      Preferred periodicity. */
#define SCH_RM_PREF_PER_USEC        40000       /* TODO allow run-time configuration (e.g. multiples of 10-ms) */
                                                /*      Use 10000 for 1M and 2M only PHY support. */
                                                /*      Use 40000 for Coded S8 PHY support. */

/*! \brief      Binary divide by preferred periodicity (10,000us) (n[max]=75,776). */
#define SCH_RM_DIV_PREF_PER(n)      ((uint32_t)(((uint64_t)(n) * UINT64_C(1717987)) >> 34))

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Reservation descriptor. */
typedef struct
{
  uint8_t handle;           /*!< Reservation handle. */
  uint32_t offsUsec;        /*!< Offset in microseconds. */
  uint32_t interUsec;       /*!< Interval in microseconds. */
} schRmRsvn_t;

/*! \brief      Scheduler resource manager control block. */
typedef struct
{
  uint8_t numRsvn;                              /*!< Number of reservations. */
  uint32_t rsvnInterUsec[SCH_RM_MAX_RSVN];      /*!< Reserved intervals indexed by handle. */
} SchRmCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern SchRmCb_t schRmCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

uint8_t schRmBuildReservationTable(schRmRsvn_t rsvn[], uint32_t rsvnOffs[], uint8_t exclude);
void schRmSortListDescending(uint32_t item[], uint8_t numItems);
void schRmSortRsvnList(schRmRsvn_t rsvn[], uint8_t numRsvn);

#ifdef __cplusplus
};
#endif

#endif /* SCH_INT_RM_H */
