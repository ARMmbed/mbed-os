/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef WS_CONFIG_H_
#define WS_CONFIG_H_


/*RPL parameters for DIO messages
 *
 * Small scale spec recomendation imin 15, doubling 2, redudancy 0
 * Small scale values imin 14, doubling 3, redudancy 0
 * Large scale network imin 19, doubling 1, redudancy 1
 *
 */

#define WS_RPL_DIO_IMIN 15
#define WS_RPL_DIO_DOUBLING 2
#define WS_RPL_DIO_REDUNDANCY 0


/* Border router version change interval
 *
 * Minimum interval at which a Border Router shall increment its PAN Version value.
 */

#define PAN_VERSION_SMALL_NETWORK_LIFETIME 4*60
#define PAN_VERSION_MEDIUM_NETWORK_LIFETIME 15*60
#define PAN_VERSION_LARGE_NETWORK_LIFETIME 30*60 //30min

#define RPL_VERSION_LIFETIME 5*3600

/* Border router connection lost timeout
 *
 * Interval within which a node expects to detect a change in PAN Version
 * (delivered via a PAN Configuration frame / PAN-IE).
 *
 * the maximum Trickle interval specified for DISC_IMAX (32 minutes).
 *
 */

#define PAN_VERSION_SMALL_NETWORK_TIMEOUT 32*60

#define PAN_VERSION_MEDIUM_NETWORK_TIMEOUT 64*60

#define PAN_VERSION_LARGE_NETWORK_TIMEOUT 90*60

/* Routing Cost Weighting factor
 */
#define PRC_WEIGHT_FACTOR    256

/* Routing Cost Weighting factor
 */
#define PS_WEIGHT_FACTOR      64

/* Smoothing factor for RSL calculation 1/8
 */
#define WS_RSL_SCALING      3

/* Device min sensitivity. This value is dynamically configured and depends on radio
 *
 * Default value for us is -93
 */
extern uint8_t DEVICE_MIN_SENS;

/* Candidate parent Threshold
 */
#define CAND_PARENT_THRESHOLD 10

/* Candidate parent Threshold hysteresis
 */
#define CAND_PARENT_HYSTERISIS 3

/* Maximum amount of Pan Configuration Solicits before restarting Discovery.
 */
#define PCS_MAX 5


/* Multicast MPL data message parameters
 * IMIN = 10 seconds, IMAX = 3 doublings
 */

#define DATA_MESSAGE_IMIN (10 * 1000)
#define DATA_MESSAGE_TIMER_EXPIRATIONS 3
#define DATA_MESSAGE_IMAX (DATA_MESSAGE_IMIN)
#define MPL_SEED_SET_ENTRY_TIMEOUT (DATA_MESSAGE_IMAX * 24 * 4 / 1000) // 10 seconds per hop making this 240 seconds

/* DHCP client timeout configuration values
 *
 */
#define WS_DHCP_SOLICIT_TIMEOUT         60
#define WS_DHCP_SOLICIT_MAX_RT          900
#define WS_DHCP_SOLICIT_MAX_RC          0


/* Neighbour table configuration
 *
 * Amount of RPL candidate parents
 * Amount of ND reply entries left
 * rest are used as child count, but is related to neighbour table size
 */
#define WS_RPL_CANDIDATE_PARENT_COUNT 3 // Largest possible value
#define WS_TEMPORARY_NEIGHBOUR_ENTRIES 7
#define WS_NON_CHILD_NEIGHBOUR_COUNT (WS_RPL_CANDIDATE_PARENT_COUNT + WS_TEMPORARY_NEIGHBOUR_ENTRIES)

/*
 * Neighbour blacklist timers
 */
#define WS_BLACKLIST_ENTRY_LIFETIME      60*30 // initial value for reject
#define WS_BLACKLIST_TIMER_MAX_TIMEOUT   60*60 // Can increase to this
#define WS_BLACKLIST_TIMER_TIMEOUT       60*30 // Blacklist is valid this time after first accept
#define WS_BLACKLIST_ENTRY_MAX_NBR       10
#define WS_BLACKLIST_PURGE_NBR            3
#define WS_BLACKLIST_PURGE_TIMER_TIMEOUT 60

/*
 * MAC frame counter NVM storing configuration
 */
#define FRAME_COUNTER_STORE_INTERVAL     60   // Time interval (on seconds) between frame counter store operations
#define FRAME_COUNTER_INCREMENT          1000 // How much frame counter is incremented on start up
#define FRAME_COUNTER_STORE_THRESHOLD    800  // How much frame counter must increment before it is stored


/*
 *  RPL Configuration parameters
 */
#define WS_MAX_DAO_RETRIES 3 // With 40s, 80s, 160s, 320s, 640s
#define WS_MAX_DAO_INITIAL_TIMEOUT 400 // With 40s initial value exponentially increasing
#define WS_MIN_DIO_MULTICAST_CONFIG_ADVERTISMENT_COUNT 10 // Define 10 multicast advertisment when learn config or learn config update

#endif /* WS_CONFIG_H_ */
