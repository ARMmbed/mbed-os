/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      MAC system configuration.
 *
 *  Copyright (c) 2013-2019 ARM Ltd. All Rights Reserved.
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

#ifndef CFG_MAC_H
#define CFG_MAC_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*** Controller HCI ***/

#ifndef CHCI_BUF_TAILROOM
#define CHCI_BUF_TAILROOM       4       /*!< Extra byte allocation required for buffer (e.g., for MIC). */
#endif

/*** Scheduler ***/

#ifndef SCH_TIMER_REQUIRED
#define SCH_TIMER_REQUIRED      TRUE    /*!< If hardware timer is required for radio access scheduler.*/
#endif

#ifdef __cplusplus
};
#endif

#endif /* CFG_MAC_H */
