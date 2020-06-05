/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Token trace decode header file.
 *
 *  Copyright (c) 2018-2019 Arm Ltd. All Rights Reserved.
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

#ifndef WSF_DETOKEN_H
#define WSF_DETOKEN_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 Macros
**************************************************************************************************/

/*! \brief Vendor specific event mask opcode. */
#define WSF_DETOKEN_VS_SET_EVENT_MASK_OPCODE    HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E1)

/*! \brief Event mask bits. */
#define WSF_DETOKEN_ENABLE_BIT                  (1<<1)

/*! \brief Vendor specific token event. */
#define WSF_DETOKEN_VS_EVT_TOKEN                0xFFF0

/** \name Platform identifiers
 *
 */
/**@{*/
#define WSF_DETOKEN_TRACE_NORDIC                1
/**@}*/

/** \name Parameter mask bits
 *
 */
/**@{*/
#define WSF_DETOKEN_PARAM_VARIABLE              0
#define WSF_DETOKEN_PARAM_STRING                1
/**@}*/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize detoken trace.
 */
/*************************************************************************************************/
void WsfDetokenInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Enable/disable detoken trace.
 *
 *  \param  enable    TRUE to enable, FALSE to disable.
 */
/*************************************************************************************************/
void WsfDetokenEnable(bool_t enable);

/*************************************************************************************************/
/*!
 *  \brief  Process vendor specific HCI events and decode token trace events from the LL.
 *
 *  \param  len       Length of pBuffer in bytes.
 *  \param  pBuffer   Buffer containing HCI event.
 *
 *  \return TRUE if VS HCI message is a token, else FALSE.
 */
/*************************************************************************************************/
bool_t WsfDetokenProcessHciEvent(uint16_t len, uint8_t *pBuffer);

#ifdef __cplusplus
};
#endif

#endif /* WSF_DETOKEN_H */
