/*
 * Copyright (c) 2015-2021, Pelion and affiliates.
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

/**
 * \file fhss_config.h
 * \brief
 */

#ifndef FHSS_CONFIG_H
#define FHSS_CONFIG_H


#ifdef __cplusplus
extern "C" {
#endif

#include "fhss_ws_extension.h"

/**
 * @brief WS channel functions.
 */
typedef enum {
    /** Fixed channel. */
    WS_FIXED_CHANNEL,
    /** TR51 channel function. */
    WS_TR51CF,
    /** Direct Hash channel function. */
    WS_DH1CF,
    /** Vendor Defined channel function. */
    WS_VENDOR_DEF_CF
} fhss_ws_channel_functions;

/**
 * \brief Struct fhss_tuning_parameter defines FHSS tuning parameters.
 * All delays are given in microseconds.
 */
typedef struct fhss_tuning_parameter {
    /** Delay between data pushed to PHY TX function and TX started (Contains CSMA-CA maximum random period). */
    uint32_t tx_processing_delay;

    /** Delay between TX done (by transmitter) and received data pushed to MAC (by receiver). */
    uint32_t rx_processing_delay;

    /** Delay between TX done (by transmitter) and Ack transmission started (by receiver) */
    uint32_t ack_processing_delay;
} fhss_tuning_parameter_t;

/**
 * \brief Struct fhss_configuration defines basic configuration of FHSS.
 */
typedef struct fhss_configuration {
    /** Tuning parameters can be used to enhance synchronization accuracy*/
    fhss_tuning_parameter_t fhss_tuning_parameters;

    /** Maximum used interval for requesting synchronization info from FHSS parent device (seconds). */
    uint16_t fhss_max_synch_interval;

    /** Number of channel retries. */
    uint8_t fhss_number_of_channel_retries;

    /** Channel mask */
    uint32_t channel_mask[8];

} fhss_configuration_t;

/**
 * @brief Get channel using vendor defined channel function.
 * @param api FHSS instance.
 * @param slot Slot number in channel schedule.
 * @param eui64 EUI-64 address of node for which the (unicast) schedule is calculated. NULL for broadcast schedule.
 * @param bsi Broadcast schedule identifier used in (broadcast) schedule calculation.
 * @param number_of_channels Number of channels in schedule.
 * @return Channel.
 */
typedef int32_t fhss_vendor_defined_cf(const fhss_api_t *api, uint16_t slot, uint8_t eui64[8], uint16_t bsi, uint16_t number_of_channels);

/**
 * \brief Struct fhss_config_parameters defines FHSS configuration parameters.
 *
 */
typedef struct fhss_config_parameters {
    /** Number of channel retries defines how many consecutive channels are used when retransmitting a frame after initial transmission channel. */
    uint8_t number_of_channel_retries;
} fhss_config_parameters_t;


/**
 * \brief Struct fhss_ws_configuration defines configuration of WS FHSS.
 */
typedef struct fhss_ws_configuration {
    /** WS unicast channel function. */
    fhss_ws_channel_functions ws_uc_channel_function;

    /** WS broadcast channel function. */
    fhss_ws_channel_functions ws_bc_channel_function;

    /** Broadcast schedule identifier. */
    uint16_t bsi;

    /** Unicast dwell interval. Range: 15-250 milliseconds. */
    uint8_t fhss_uc_dwell_interval;

    /** Broadcast interval. Duration between broadcast dwell intervals. Range: 0-16777216 milliseconds. */
    uint32_t fhss_broadcast_interval;

    /** Broadcast dwell interval. Range: 15-250 milliseconds. */
    uint8_t fhss_bc_dwell_interval;

    /** Unicast fixed channel */
    uint8_t unicast_fixed_channel;

    /** Broadcast fixed channel */
    uint8_t broadcast_fixed_channel;

    /** Channel mask. Wi-SUN will use this for broadcast */
    uint32_t channel_mask[8];

    /** Wi-SUN specific unicast channel mask */
    uint32_t unicast_channel_mask[8];

    /** Channel mask size */
    uint16_t channel_mask_size;

    /** Vendor defined channel function. */
    fhss_vendor_defined_cf *vendor_defined_cf;

    /** Configuration parameters. */
    fhss_config_parameters_t config_parameters;

} fhss_ws_configuration_t;

/**
 * \brief Struct fhss_timer defines interface between FHSS and FHSS platform timer.
 * Application must implement FHSS timer driver which is then used by FHSS with this interface.
 */
typedef struct fhss_timer {
    /** Start timeout (1us). Timer must support multiple simultaneous timeouts */
    int (*fhss_timer_start)(uint32_t, void (*fhss_timer_callback)(const fhss_api_t *fhss_api, uint16_t), const fhss_api_t *fhss_api);

    /** Stop timeout */
    int (*fhss_timer_stop)(void (*fhss_timer_callback)(const fhss_api_t *fhss_api, uint16_t), const fhss_api_t *fhss_api);

    /** Get remaining time of started timeout*/
    uint32_t (*fhss_get_remaining_slots)(void (*fhss_timer_callback)(const fhss_api_t *fhss_api, uint16_t), const fhss_api_t *fhss_api);

    /** Get timestamp since initialization of driver. Overflow of 32-bit counter is allowed (1us) */
    uint32_t (*fhss_get_timestamp)(const fhss_api_t *fhss_api);

    /** Divide 1 microsecond resolution. E.g. to use 64us resolution, use fhss_resolution_divider = 64*/
    uint8_t fhss_resolution_divider;
} fhss_timer_t;

/**
 * \brief Struct fhss_synch_configuration defines the synchronization time configurations.
 * Border router application must define and set these configuration for FHSS network.
 */
typedef struct fhss_synch_configuration {
    /** Number of broadcast channels. */
    uint8_t fhss_number_of_bc_channels;

    /** TX slots per channel. */
    uint8_t fhss_number_of_tx_slots;

    /** Length of superframe(microseconds) * Number of superframes defines the
        channel dwell time. E.g. 50000us * 8 -> Channel dwell time 400ms */
    uint16_t fhss_superframe_length;

    /** Number of superframes. */
    uint8_t fhss_number_of_superframes;
} fhss_synch_configuration_t;


/**
 * \brief Struct fhss_statistics defines the available FHSS statistics.
 */
typedef struct fhss_statistics {
    /** FHSS synchronization drift compensation (us/channel). */
    int16_t fhss_drift_compensation;

    /** FHSS hop count. */
    uint8_t fhss_hop_count;

    /** FHSS synchronization interval (s). */
    uint16_t fhss_synch_interval;

    /** Average of 5 preceding synchronization fixes (us). Updated after every fifth synch fix. */
    int16_t fhss_prev_avg_synch_fix;

    /** FHSS synchronization lost counter. */
    uint32_t fhss_synch_lost;

    /** FHSS TX to unknown neighbour counter. */
    uint32_t fhss_unknown_neighbor;

    /** FHSS channel retry counter. */
    uint32_t fhss_channel_retry;
} fhss_statistics_t;

/**
 * \brief Enumeration fhss_channel_mode_e defines the channel modes.
 */
typedef enum fhss_channel_mode_e {
    SINGLE_CHANNEL,     //< Single channel
    FHSS,               //< Frequency hopping mode
} fhss_channel_mode_e;

#ifdef __cplusplus
}
#endif

#endif // FHSS_CONFIG_H
