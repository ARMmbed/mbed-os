/*
 * Copyright (c) 2019 Arm Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SMSC9220_EMAC_CONFIG_H
#define SMSC9220_EMAC_CONFIG_H

#include "cmsis_os.h"

#define SMSC9220_HWADDR_SIZE                 6U
#define SMSC9220_BUFF_ALIGNMENT              4U

/*
 * Maximum Transfer Unit
 * The IEEE 802.3 specification limits the data portion of the 802.3 frame
 * to a minimum of 46 and a maximum of 1522 bytes, this is on L2 level.
 */
#define SMSC9220_ETH_MTU_SIZE                1500U
#define SMSC9220_ETH_IF_NAME                 "smsc9220"
#define SMSC9220_ETH_MAX_FRAME_SIZE          1522U

/** \brief Defines for receiver thread */
#define FLAG_RX                              1U
#define LINK_STATUS_THREAD_PRIORITY          (osPriorityNormal)
#define LINK_STATUS_THREAD_STACKSIZE         512U
#define LINK_STATUS_TASK_PERIOD_MS           200U
#define PHY_STATE_LINK_DOWN                  false
#define PHY_STATE_LINK_UP                    true
#define CRC_LENGTH_BYTES                     4U

#endif /* SMSC9220_EMAC_CONFIG_H */
