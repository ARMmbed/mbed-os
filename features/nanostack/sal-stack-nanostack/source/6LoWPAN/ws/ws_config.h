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

#define WS_RPL_DIO_IMIN_SMALL 15
#define WS_RPL_DIO_DOUBLING_SMALL 2
#define WS_RPL_DIO_REDUNDANCY_SMALL 0

#define WS_RPL_DIO_IMIN_MEDIUM 15
#define WS_RPL_DIO_DOUBLING_MEDIUM 5
#define WS_RPL_DIO_REDUNDANCY_MEDIUM 10

#define WS_RPL_DIO_IMIN_LARGE 19
#define WS_RPL_DIO_DOUBLING_LARGE 1
#define WS_RPL_DIO_REDUNDANCY_LARGE 10 // May need some tuning still

#define WS_RPL_DIO_IMIN_AUTOMATIC 14
#define WS_RPL_DIO_DOUBLING_AUTOMATIC 3
#define WS_RPL_DIO_REDUNDANCY_AUTOMATIC 0

#define WS_RPL_MIN_HOP_RANK_INCREASE 196
#define WS_RPL_MAX_HOP_RANK_INCREASE 2048

#define WS_CERTIFICATE_RPL_MIN_HOP_RANK_INCREASE 128
#define WS_CERTIFICATE_RPL_MAX_HOP_RANK_INCREASE 0

/*
 *  RPL DAO timeout maximum value. This will force DAO timeout to happen before this time
 */
#define WS_RPL_DAO_MAX_TIMOUT (3600*2)

/* Border router version change interval
 *
 * Amount of version increases border router makes during PAN_TIMEOUT time
 */

#define PAN_VERSION_CHANGE_INTERVAL 3

// RPL version number update intervall
// after restart version numbers are increased faster and then slowed down when network is stable
#define RPL_VERSION_LIFETIME 12*3600
#define RPL_VERSION_LIFETIME_RESTART 3600

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

#define DATA_MESSAGE_IMIN 10
#define DATA_MESSAGE_TIMER_EXPIRATIONS 3
#define DATA_MESSAGE_IMAX 80
#define DATA_MESSAGE_K 8
#define MPL_SEED_SET_ENTRY_TIMEOUT (DATA_MESSAGE_IMAX * 24 * 4) // 10 seconds per hop making this 240 seconds

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
#define FRAME_COUNTER_STORE_TRIGGER      5    // Delay (on seconds) before storing, when storing of frame counters is triggered
#define FRAME_COUNTER_INCREMENT          1000 // How much frame counter is incremented on start up
#define FRAME_COUNTER_STORE_THRESHOLD    800  // How much frame counter must increment before it is stored


/*
 *  RPL Configuration parameters
 */
#define WS_MAX_DAO_RETRIES 3 // With 40s, 80s, 160s, 320s, 640s
#define WS_MAX_DAO_INITIAL_TIMEOUT 400 // With 40s initial value exponentially increasing
#define WS_MIN_DIO_MULTICAST_CONFIG_ADVERTISMENT_COUNT 0xff // Advertisment config at every MC DIO
#define WS_MAX_PARENT_SET_COUNT 2 // maximum amount of parents selected by node

#define WS_NODE_RPL_SOFT_MEM_LIMIT 4*1024 // Limit when RPL start purge unused data
#define WS_NODE_RPL_HARD_MEM_LIMIT 6*1024 // Limit when RPL memory allocation start limit allocation

/*
 * Candidate parent list parameters
 */

#define WS_PARENT_LIST_SIZE 10
#define WS_PARENT_LIST_MAX_AGE 3600*10         // 1 hour in 100ms ticks
#define WS_PARENT_LIST_MAX_PAN_IN_DISCOVERY 5  // During discovery state how many neighbours per pan
#define WS_PARENT_LIST_MAX_PAN_IN_ACTIVE 2     // During active state two nodes per pan is allowed

/*
 * Modifications for base specification.
 *
 * ERRATA changes after 1.0 specification release.
 */
#define WISUN_1_0_ERRATA_FIX

/*
 *  Security protocol message retry configuration parameters
 */
#define SEC_PROT_SMALL_IMIN 30              // Retries done in 30 seconds
#define SEC_PROT_SMALL_IMAX 90              // Largest value 90 seconds
#define SEC_PROT_RETRY_TIMEOUT_SMALL 330    // Retry timeout for small network additional 30 seconds for authenticator delay

#define SEC_PROT_LARGE_IMIN 60              // Retries done in 60 seconds
#define SEC_PROT_LARGE_IMAX 240             // Largest value 240 seconds
#define SEC_PROT_RETRY_TIMEOUT_LARGE 750    // Retry timeout for large network additional 30 seconds for authenticator delay

#define SEC_PROT_TIMER_EXPIRATIONS 2        // Number of retries

// Maximum number of simultaneous EAP-TLS negotiations
#define MAX_SIMULTANEOUS_EAP_TLS_NEGOTIATIONS_SMALL     3
#define MAX_SIMULTANEOUS_EAP_TLS_NEGOTIATIONS_MEDIUM    20
#define MAX_SIMULTANEOUS_EAP_TLS_NEGOTIATIONS_LARGE       50

/*
 *  Security protocol timer configuration parameters
 */
#define MINUTES_IN_DAY   24 * 60
#define DEFAULT_GTK_EXPIRE_OFFSET               43200                    // 30 days
#define DEFAULT_PMK_LIFETIME                    4 * 30 * MINUTES_IN_DAY  // 4 months
#define DEFAULT_PTK_LIFETIME                    2 * 30 * MINUTES_IN_DAY  // 2 months
#define DEFAULT_GTK_NEW_ACTIVATION_TIME         720                      // default 1/720 * 30 days --> 60 minutes
#define DEFAULT_REVOCATION_LIFETIME_REDUCTION   30                       // default 1/30 * 30 days --> 1 day
#define DEFAULT_GTK_REQUEST_IMIN                4                        // 4 minutes
#define DEFAULT_GTK_REQUEST_IMAX                64                       // 64 minutes
#define DEFAULT_GTK_MAX_MISMATCH                64                       // 64 minutes
#define DEFAULT_GTK_NEW_INSTALL_REQUIRED        80                       // 80 percent of GTK lifetime --> 24 days

#endif /* WS_CONFIG_H_ */
