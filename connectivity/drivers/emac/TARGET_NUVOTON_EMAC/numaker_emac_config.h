/* Copyright (c) 2018 ARM Limited
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
 *
 *
 * Description:   NuMaker EMAC Config header file
 */
 
#ifndef NUMAKER_EMAC_CONFIG_H__
#define NUMAKER_EMAC_CONFIG_H__

#define NU_RX_RING_LEN              (8)
#define NU_TX_RING_LEN              (4)

#define NU_ETH_MAX_FLEN             (1518) 

#define NU_HWADDR_SIZE              (6)

#define NU_ETH_MTU_SIZE              1500
#define NU_ETH_IF_NAME               "nu"

#define THREAD_STACKSIZE             512

#endif // #define NUMAKER_EMAC_CONFIG_H__

