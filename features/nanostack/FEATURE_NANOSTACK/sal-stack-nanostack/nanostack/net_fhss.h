/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
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
 * \file net_fhss.h
 * \brief FHSS API
 *
 */

#ifndef NET_FHSS_H_
#define NET_FHSS_H_

#include "ns_types.h"

#ifndef RPL_SYNCHRONIZATION_INSTANCE_ID
#define RPL_SYNCHRONIZATION_INSTANCE_ID 1
#endif

/** FHSS configuration structure */
typedef struct fhss_configuration_s
{
    /** Number of broadcast channels. */
    uint8_t fhss_number_of_bc_channels;

    /** TX slots per channel. */
    uint8_t fhss_number_of_tx_slots;

    /** Length of superframe, in microseconds. */
    uint16_t fhss_superframe_length;

    /** Number of superframes, after which the counter will start from zero. */
    uint8_t fhss_number_of_superframes;

    /**
     * Interval of beacon sending, in milliseconds.
     * Note1: Value 0 disables them.
     * Note2: This will likely be removed when beacons are sent only when needed.
     */
    uint32_t fhss_beacon_send_interval;
} fhss_configuration_s;

/** FHSS Platform callbacks */
typedef struct
{
    int (*fhss_timer_start)(uint32_t, void (*fhss_timer_callback)(int8_t, uint16_t), int8_t);
    int (*fhss_timer_stop)(void);
    uint32_t (*fhss_get_remaining_slots)(void);
    uint32_t (*fhss_get_timestamp)(void);
    int (*fhss_time_measure_start)(void);
    uint32_t (*fhss_time_measure_read)(void);
    int (*fhss_time_measure_stop)(void);
    uint8_t fhss_resolution_divider;
} fhss_platform_functions_s;

/** FHSS tuning parameters */
typedef struct
{
    /** Processing delay between synch info written and TX start. */
    uint32_t synch_tx_processing_time;
    /** Processing delay between TX done and synch info read. */
    uint32_t synch_rx_processing_time;
    /** Processing delay between data pushed to driver and transmission started. */
    uint32_t data_tx_processing_time;
    /** Processing delay between TX done and Ack TX start. */
    uint32_t data_rx_processing_time;
} fhss_platform_tuning_params_s;

/** FHSS Channel mode */
typedef enum fhss_channel_mode_e {
    SINGLE_CHANNEL,     //< Single channel
    FHSS,               //< Frequency hopping mode
} fhss_channel_mode_e;

#if 0
/**
 * \brief Enable FHSS mode. Note: arm_nwk_interface_configure_6lowpan_bootstrap_set() must be called before enabling FHSS.
 *
 * \param interface_id Network interface ID.
 * \param fhss_platform_functions Platform functions.
 * \param fhss_configuration Given configuration.
 * \return 0 on success.
 * \return -1 in case of invalid input parameters.
 * \return -2 if no channels are available in channel list.
 * \return -3 if the broadcast channels or TX slots are 0 in the bootstrap mode Border Router/RF access point.
 * \return -4 if the number of super frames does not work with TX slots in bootstrap mode Border Router/RF access point.
 * \return -5 if FHSS tasklet creation fails.
 * \return -6 if PHY driver mode cannot be changed.
 * \return -7 if the used bootstrap mode is not supported.
 * \return -8 if FHSS is already enabled.
 * \return -9 if memory allocation failed.
 */
extern int8_t arm_fhss_enable(int8_t interface_id, fhss_platform_functions_s *fhss_platform_functions,
                              const fhss_configuration_s *fhss_configuration);

/**
 * \brief Disable FHSS mode.
 *
 * \param interface_id Network interface ID.
 * \return 0 on success, -1 on fail.
 */
extern int8_t arm_fhss_disable(int8_t interface_id);

/**
 * \brief Set platform tuning parameters.
 *
 * \param interface_id Network interface ID.
 * \param fhss_tuning_params Tuning parameters.
 * \return 0 on success, -1 on fail.
 */
extern int8_t arm_fhss_set_tuning_params(int8_t interface_id, const fhss_platform_tuning_params_s *fhss_tuning_params);

#else
NS_DUMMY_DEFINITIONS_OK
#define arm_fhss_enable(interface_id, fhss_platform_functions,fhss_configuration) (-1)
#define arm_fhss_disable(interface_id) (-1)
#define arm_fhss_set_tuning_params(interface_id, fhss_tuning_params) (-1)
#endif

#endif /* NET_FHSS_H_ */
