/*
 * Copyright (c) 2018-2021, Pelion and affiliates.
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

#define WS_RPL_DIO_IMIN_MEDIUM 17
#define WS_RPL_DIO_DOUBLING_MEDIUM 3
#define WS_RPL_DIO_REDUNDANCY_MEDIUM 10

#define WS_RPL_DIO_IMIN_LARGE 18
#define WS_RPL_DIO_DOUBLING_LARGE 3
#define WS_RPL_DIO_REDUNDANCY_LARGE 10 // May need some tuning still

#define WS_RPL_DIO_IMIN_XLARGE 18
#define WS_RPL_DIO_DOUBLING_XLARGE 4
#define WS_RPL_DIO_REDUNDANCY_XLARGE 10 // May need some tuning still

#define WS_RPL_DIO_IMIN_AUTOMATIC 14
#define WS_RPL_DIO_DOUBLING_AUTOMATIC 3
#define WS_RPL_DIO_REDUNDANCY_AUTOMATIC 0

#define WS_RPL_MIN_HOP_RANK_INCREASE 196
#define WS_RPL_MAX_HOP_RANK_INCREASE 2048

#define WS_RPL_DEFAULT_LIFETIME        (3600*2) // 2 hours
#define WS_RPL_DEFAULT_LIFETIME_MEDIUM (3600*4) // 4 hours
#define WS_RPL_DEFAULT_LIFETIME_LARGE  (3600*8) // 8 hours
#define WS_RPL_DEFAULT_LIFETIME_XLARGE (3600*12) // 12 hours

#define WS_DHCP_ADDRESS_LIFETIME_SMALL 2*3600 // small networks less than devices 100
#define WS_DHCP_ADDRESS_LIFETIME_MEDIUM 12*3600 // Medium size networks from 100 - 1000 device networks
#define WS_DHCP_ADDRESS_LIFETIME_LARGE 24*3600 // Large size networks 1000 + device networks

#define WS_CERTIFICATE_RPL_MIN_HOP_RANK_INCREASE 128
#define WS_CERTIFICATE_RPL_MAX_HOP_RANK_INCREASE 0

/*
 *  RPL DAO timeout maximum value. This will force DAO timeout to happen before this time
 */
#define WS_RPL_DAO_MAX_TIMOUT (3600*12)

/* Border router version change interval
 *
 * Amount of version increases border router makes during PAN_TIMEOUT time
 */

#define PAN_VERSION_CHANGE_INTERVAL 3

/* If PAN version lifetime would be 10 minutes, 1000 increments is about 7 days i.e. storage must
   be written at least once a week */
#define PAN_VERSION_STORAGE_READ_INCREMENT    1000

// RPL version number update intervall
// after restart version numbers are increased faster and then slowed down when network is stable
#define RPL_VERSION_LIFETIME 12*3600
#define RPL_VERSION_LIFETIME_RESTART_SMALL 3600
#define RPL_VERSION_LIFETIME_RESTART_MEDIUM 2*3600
#define RPL_VERSION_LIFETIME_RESTART_LARGE 4*3600
#define RPL_VERSION_LIFETIME_RESTART_EXTRA_LARGE 8*3600

/* Border router connection lost timeout
 *
 * Interval within which a node expects to detect a change in PAN Version
 * (delivered via a PAN Configuration frame / PAN-IE).
 *
 * the maximum Trickle interval specified for DISC_IMAX (32 minutes).
 *
 */

#define PAN_VERSION_SMALL_NETWORK_TIMEOUT 30*60

#define PAN_VERSION_MEDIUM_NETWORK_TIMEOUT 60*60

#define PAN_VERSION_LARGE_NETWORK_TIMEOUT 90*60

#define PAN_VERSION_XLARGE_NETWORK_TIMEOUT 120*60

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
 */
#define MPL_SAFE_HOP_COUNT 6

/*Border router override to optimize the multicast startup*/
#define MPL_BORDER_ROUTER_MIN_EXPIRATIONS 2
#define MPL_BORDER_ROUTER_MAXIMUM_IMAX 40

/*Small network size*/
#define MPL_SMALL_IMIN 1
#define MPL_SMALL_IMAX 10
#define MPL_SMALL_EXPIRATIONS 1
#define MPL_SMALL_K 8
#define MPL_SMALL_SEED_LIFETIME (MPL_SMALL_IMAX * MPL_SAFE_HOP_COUNT * (MPL_SMALL_EXPIRATIONS + 1)) // time that packet should get to safe distance
/*Medium network size*/
#define MPL_MEDIUM_IMIN 1
#define MPL_MEDIUM_IMAX 32
#define MPL_MEDIUM_EXPIRATIONS 1
#define MPL_MEDIUM_K 8
#define MPL_MEDIUM_SEED_LIFETIME (MPL_MEDIUM_IMAX * MPL_SAFE_HOP_COUNT * (MPL_MEDIUM_EXPIRATIONS + 1)) // time that packet should get to safe distance
/*Large network size*/
#define MPL_LARGE_IMIN 5
#define MPL_LARGE_IMAX 40
#define MPL_LARGE_EXPIRATIONS 2
#define MPL_LARGE_K 8
#define MPL_LARGE_SEED_LIFETIME (MPL_LARGE_IMAX * MPL_SAFE_HOP_COUNT * (MPL_LARGE_EXPIRATIONS + 1)) // time that packet should get to safe distance
/*xtra large network size*/
#define MPL_XLARGE_IMIN 10
#define MPL_XLARGE_IMAX 80
#define MPL_XLARGE_EXPIRATIONS 2
#define MPL_XLARGE_K 8
#define MPL_XLARGE_SEED_LIFETIME (MPL_XLARGE_IMAX * MPL_SAFE_HOP_COUNT * (MPL_XLARGE_EXPIRATIONS + 1)) // time that packet should get to safe distance

/* DHCP client timeout configuration values
 *
 */
#define WS_DHCP_SOLICIT_TIMEOUT         60
#define WS_DHCP_SOLICIT_MAX_RT          900
#define WS_DHCP_SOLICIT_MAX_RC          0
#define WS_DHCP_SOLICIT_MAX_DELAY       5


/* Neighbour table configuration
 *
 * Amount of RPL candidate parents
 * Amount of ND reply entries left
 * rest are used as child count, but is related to neighbour table size
 */
#define WS_RPL_CANDIDATE_PARENT_COUNT 3
#define WS_SMALL_TEMPORARY_NEIGHBOUR_ENTRIES 7
#define WS_MEDIUM_TEMPORARY_NEIGHBOUR_ENTRIES 12
#define WS_LARGE_TEMPORARY_NEIGHBOUR_ENTRIES 22

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
#define FRAME_COUNTER_STORE_INTERVAL        60          // Time interval (on seconds) between checking if frame counter storing is needed
#define FRAME_COUNTER_STORE_FORCE_INTERVAL  (3600 * 20) // Time interval (on seconds) before frame counter storing is forced (if no other storing operations triggered)
#define FRAME_COUNTER_STORE_TRIGGER         5           // Delay (on seconds) before storing, when storing of frame counters is triggered
#define FRAME_COUNTER_INCREMENT             1000000     // How much frame counter is incremented on start up
#define FRAME_COUNTER_STORE_THRESHOLD       994999      // How much frame counter must increment before it is stored


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

#define WS_CONGESTION_PACKET_SIZE 500           // Packet length for calculate how much heap message queue can fit
#define WS_CONGESTION_QUEUE_DELAY 60            // Define message queue max length for given delay. This value is multiple by packet/seconds
#define WS_CONGESTION_RED_DROP_PROBABILITY 10 //10.0%
#define WS_CONGESTION_BR_MIN_QUEUE_SIZE 85000 / WS_CONGESTION_PACKET_SIZE
#define WS_CONGESTION_BR_MAX_QUEUE_SIZE 600000 / WS_CONGESTION_PACKET_SIZE
#define WS_CONGESTION_NODE_MIN_QUEUE_SIZE 10000 / WS_CONGESTION_PACKET_SIZE
#define WS_CONGESTION_NODE_MAX_QUEUE_SIZE 85000 / WS_CONGESTION_PACKET_SIZE
/*
 * Modifications for base specification.
 *
 * ERRATA changes after 1.0 specification release.
 */
#define WISUN_1_0_ERRATA_FIX

/*
 * Security protocol message retry configuration parameters
 *
 * Trickle is reset on start (inconsistent heard is set)
 */
#define SEC_PROT_SMALL_IMIN 60              // Retries done in 60 seconds
#define SEC_PROT_SMALL_IMAX 120             // Largest value 120 seconds
#define SEC_PROT_RETRY_TIMEOUT_SMALL 450    // Retry timeout for small network additional 30 seconds for authenticator delay

#define SEC_PROT_LARGE_IMIN 60              // Retries done in 60 seconds
#define SEC_PROT_LARGE_IMAX 240             // Largest value 240 seconds
#define SEC_PROT_RETRY_TIMEOUT_LARGE 750    // Retry timeout for large network additional 30 seconds for authenticator delay

#define SEC_PROT_TIMER_EXPIRATIONS 4        // Number of retries

// Maximum number of simultaneous security negotiations
#define MAX_SIMULTANEOUS_SECURITY_NEGOTIATIONS_TX_QUEUE_MIN   64
#define MAX_SIMULTANEOUS_SECURITY_NEGOTIATIONS_TX_QUEUE_MAX   192

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

/*
 *  Security protocol initial EAPOL-key parameters
 *
 * Retry time is randomized between minimum and maximum retry time: rand(min,max).
 * For each subsequent retry the maximum retry time is doubled until the maximum
 * limit is reached.
 */

/* Small network initial EAPOL-key retry exponential backoff parameters
 *     1st backoff 3 to 7 minutes, max 7 minutes, retries 2
 *     Minimum time for sequence is 3 + 3 = 6 minutes
 *     Maximum time for sequence is 7 + 7 = 14 minutes
 */
#define SMALL_NW_INITIAL_KEY_RETRY_MIN_SECS               180     // 3
#define SMALL_NW_INITIAL_KEY_RETRY_MAX_SECS               420     // 7
#define SMALL_NW_INITIAL_KEY_RETRY_MAX_LIMIT_SECS         420     // 7
#define SMALL_NW_INITIAL_KEY_RETRY_COUNT                  2

/* Medium network initial EAPOL-key retry exponential backoff parameters
 *     1st backoff 3 to 7 minutes, max 12 minutes, retries 4
 *     Minimum time for sequence is 3 + 3 + 3 + 3 = 12 minutes
 *     Maximum time for sequence is 7 + 12 + 12 + 12 = 43 minutes
 */
#define MEDIUM_NW_INITIAL_KEY_RETRY_MIN_SECS              180     // 3
#define MEDIUM_NW_INITIAL_KEY_RETRY_MAX_SECS              420     // 7
#define MEDIUM_NW_INITIAL_KEY_RETRY_MAX_LIMIT_SECS        720     // 12
#define MEDIUM_NW_INITIAL_KEY_RETRY_COUNT                 4

/* Large network initial EAPOL-key retry exponential backoff parameters
 *     1st backoff 5 to 10 minutes, max 15 minutes, retries 4
 *     Minimum time for sequence is 5 + 5 + 5 + 5 = 20 minutes
 *     Maximum time for sequence is 10 + 15 + 15 + 15 = 55 minutes
 */
#define LARGE_NW_INITIAL_KEY_RETRY_MIN_SECS               300     // 5
#define LARGE_NW_INITIAL_KEY_RETRY_MAX_SECS               600     // 10
#define LARGE_NW_INITIAL_KEY_RETRY_MAX_LIMIT_SECS         900     // 15
#define LARGE_NW_INITIAL_KEY_RETRY_COUNT                  4

/* Extra large network initial EAPOL-key retry exponential backoff parameters
 *     1st backoff 5 to 10 minutes, max 20 minutes, retries 4
 *     Minimum time for sequence is 5 + 5 + 5 + 5 = 20 minutes
 *     Maximum time for sequence is 10 + 20 + 20 + 20 = 70 minutes
 */
#define EXTRA_LARGE_NW_INITIAL_KEY_RETRY_MIN_SECS         300     // 5
#define EXTRA_LARGE_NW_INITIAL_KEY_RETRY_MAX_SECS         600     // 10
#define EXTRA_LARGE_NW_INITIAL_KEY_RETRY_MAX_LIMIT_SECS   1200    // 20
#define EXTRA_LARGE_NW_INITIAL_KEY_RETRY_COUNT            4

/*
 *  RADIUS client retry timer defaults
 */
#define RADIUS_CLIENT_RETRY_IMIN           20       // First retry minimum 1 seconds
#define RADIUS_CLIENT_RETRY_IMAX           30       // First retry maximum 3 seconds
#define RADIUS_CLIENT_TIMER_EXPIRATIONS    3        // Number of retries is three

/*
 *  EAP-TLS fragment length
 *
 *  Configures both EAP-TLS and the RADIUS client (Framed-MTU on RFC 2864)
 */
#define EAP_TLS_FRAGMENT_LEN_VALUE         600       // EAP-TLS fragment length

#endif /* WS_CONFIG_H_ */
