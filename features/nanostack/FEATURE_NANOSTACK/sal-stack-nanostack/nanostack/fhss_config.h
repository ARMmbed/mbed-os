/*
 * Copyright (c) 2015-2017 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
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

/**
 * \brief Struct fhss_tuning_parameter defines FHSS tuning parameters.
 * All delays are given in microseconds.
 */
typedef struct fhss_tuning_parameter
{
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
typedef struct fhss_configuration
{
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
 * \brief Struct fhss_timer defines interface between FHSS and FHSS platform timer.
 * Application must implement FHSS timer driver which is then used by FHSS with this interface.
 */
typedef struct fhss_timer
{
    /** Start timeout (1us) */
    int (*fhss_timer_start)(uint32_t, void (*fhss_timer_callback)(const fhss_api_t *fhss_api, uint16_t), const fhss_api_t *fhss_api);

    /** Stop timeout */
    int (*fhss_timer_stop)(const fhss_api_t *fhss_api);

    /** Get remaining time of started timeout*/
    uint32_t (*fhss_get_remaining_slots)(const fhss_api_t *fhss_api);

    /** Get timestamp since initialization of driver. Overflow of 32-bit counter is allowed (1us) */
    uint32_t (*fhss_get_timestamp)(const fhss_api_t *fhss_api);

    /** Divide 1 microsecond resolution. E.g. to use 64us resolution, use fhss_resolution_divider = 64*/
    uint8_t fhss_resolution_divider;
} fhss_timer_t;

/**
 * \brief Struct fhss_synch_configuration defines the synchronization time configurations.
 * Border router application must define and set these configuration for FHSS network.
 */
typedef struct fhss_synch_configuration
{
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
typedef struct fhss_statistics
{
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
