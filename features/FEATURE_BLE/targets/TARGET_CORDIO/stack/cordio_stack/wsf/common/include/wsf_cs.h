/*************************************************************************************************/
/*!
 *  \file   wsf_cs.h
 *
 *  \brief  Critical section macros.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
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
#ifndef WSF_CS_H
#define WSF_CS_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WSF_CS_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifndef WSF_CS_STATS
/*! \brief  Use CS statistics hooks. */
#define WSF_CS_STATS    FALSE
#endif

/*************************************************************************************************/
/*!
 *  \def    WSF_CS_INIT
 *
 *  \brief  Initialize critical section.  This macro may define a variable.
 *
 *  \param  cs    Critical section variable to be defined.
 */
/*************************************************************************************************/
#define WSF_CS_INIT(cs)

/*************************************************************************************************/
/*!
 *  \def    WSF_CS_ENTER
 *
 *  \brief  Enter a critical section.
 *
 *  \param  cs    Critical section variable.
 */
/*************************************************************************************************/
#define WSF_CS_ENTER(cs)        WsfCsEnter()

/*************************************************************************************************/
/*!
 *  \def    WSF_CS_EXIT
 *
 *  \brief  Exit a critical section.
 *
 *  \param  cs    Critical section variable.
 */
/*************************************************************************************************/
#define WSF_CS_EXIT(cs)        WsfCsExit()

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief  Timestamp call signature. */
typedef bool_t (*WsfCsTimestamp_t)(uint32_t *);

/*! \brief  Timestamp delta and time base converter call signature. */
typedef uint32_t (*WsfCsTimebase_t)(uint32_t);

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief Statistics global for watermark of critical sections.  Value is in microseconds. */
extern uint16_t wsfCsStatsWatermarkUsec;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Register critical section statistics hooks.
 *
 *  \param  timestampCback  Callback for obtaining the current timestamp.
 *  \param  timebaseCback   Callback for converting timestamp delta into microseconds.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfCsStatsRegister(WsfCsTimestamp_t timestampCback, WsfCsTimebase_t timebaseCback);

/*************************************************************************************************/
/*!
 *  \brief  Enter a critical section.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfCsEnter(void);

/*************************************************************************************************/
/*!
 *  \brief  Enter a critical section.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfCsExit(void);

/*! \} */    /* WSF_CS_API */

#ifdef __cplusplus
};
#endif

#endif /* WSF_CS_H */
