/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Internal multi-protocol scheduler interface file.
 *
 *  Copyright (c) 2013-2018 ARM Ltd. All Rights Reserved.
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

#ifndef SCH_INT_H
#define SCH_INT_H

#include "sch_api.h"
#include "bb_drv.h"
#include "wsf_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Maximum span of scheduler elements. */
#define SCH_MAX_SPAN            0x80000000

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Scheduler states. */
enum
{
  SCH_STATE_IDLE,               /*!< Scheduler idle. */
  SCH_STATE_LOAD,               /*!< Scheduler loading next BOD. */
  SCH_STATE_EXEC                /*!< Scheduler executing BOD. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

typedef struct
{
  bool_t active;                  /*!< Whether background task is active. */
  BbOpDesc_t *pBod;               /*!< Head element of scheduled list of BOD. */
} SchBackground_t;

/*! \brief      Scheduler control block. */
typedef struct
{
  bool_t state;                 /*!< Current scheduler state. */
  bool_t eventSetFlag;          /*!< Scheduler event set (BB terminated BOD). */
  wsfHandlerId_t handlerId;     /*!< System event handler ID. */

  BbOpDesc_t *pHead;            /*!< Head element of scheduled list of BOD. */
  BbOpDesc_t *pTail;            /*!< Tail element of scheduled list of BOD. */
  SchBackground_t background;   /*!< Background BOD. */
} SchCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern SchCtrlBlk_t schCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Load */
void schLoadNext(void);
bool_t schTryLoadHead(void);

/* List management */
void schRemoveHead(void);

/*************************************************************************************************/
/*!
 *  \brief      Is BOD due time in the future.
 *
 *  \param      pBod    Target BOD.
 *
 *  \return     TRUE if BOD time is in the future, FALSE otherwise.
 */
/*************************************************************************************************/
static inline bool_t schDueTimeInFuture(BbOpDesc_t *pBod)
{
  bool_t result = FALSE;

  const uint32_t curTime = BbDrvGetCurrentTime();
  const uint32_t delta = pBod->due - curTime;

  if (delta < SCH_MAX_SPAN)     /* due time has not passed */
  {
    result = TRUE;
  }

  return result;
}

#ifdef __cplusplus
};
#endif

#endif /* SCH_INT_H */
