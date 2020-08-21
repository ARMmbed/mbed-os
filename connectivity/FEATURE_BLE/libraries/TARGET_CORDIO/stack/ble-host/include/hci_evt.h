/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  HCI event module.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
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
#ifndef HCI_EVT_H
#define HCI_EVT_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup STACK_HCI_API
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief HCI event statistics */
typedef struct
{
  uint16_t numDiscCmplEvt;              /*!< \brief Number discovery complete events. */
  uint16_t numEncChangeEvt;             /*!< \brief Number encryption change events. */
  uint16_t numReadRemoteVerInfoCmpEvt;  /*!< \brief Number read remote version info complete events. */
  uint16_t numCmdCmplEvt;               /*!< \brief Number command complete events. */
  uint16_t numCmdStatusEvt;             /*!< \brief Number command status events. */
  uint16_t numHwErrorEvt;               /*!< \brief Number hardware error events. */
  uint16_t numCmplPktsEvt;              /*!< \brief Number complete packet events. */
  uint16_t numDataBufOverflowEvt;       /*!< \brief Number data buf overflow events. */
  uint16_t numEncKeyRefreshCmplEvt;     /*!< \brief Number encryption key refresh complete events. */
  uint16_t numLeMetaEvt;                /*!< \brief Number LE meta events. */
  uint16_t numVendorSpecEvt;            /*!< \brief Number vendor specific events. */
  uint16_t numAuthToEvt;                /*!< \brief Number authenticated to events. */
} hciEvtStats_t;

/*! \} */    /* STACK_HCI_API */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/


/*************************************************************************************************/
/*!
 *  \brief  Process received HCI events.
 *
 *  \param  pEvt    Buffer containing HCI event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciEvtProcessMsg(uint8_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Get event statistics.
 *
 *  \return Event statistics.
 */
/*************************************************************************************************/
hciEvtStats_t *hciEvtGetStats(void);

#ifdef __cplusplus
};
#endif

#endif /* HCI_EVT_H */
