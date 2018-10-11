/**
 *  @file lora_phy_ds.h
 *
 *  @brief Data structures relating to PHY layer
 *
 *  \code
 *   ______                              _
 *  / _____)             _              | |
 * ( (____  _____ ____ _| |_ _____  ____| |__
 *  \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *  _____) ) ____| | | || |_| ____( (___| | | |
 * (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *   (C)2013 Semtech
 *  ___ _____ _   ___ _  _____ ___  ___  ___ ___
 * / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 * \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 * |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 * embedded.connectivity.solutions===============
 *
 * \endcode
 *
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *
 * Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef MBED_OS_LORA_PHY_DATASTRUCTURES_
#define MBED_OS_LORA_PHY_DATASTRUCTURES_

#include "system/lorawan_data_structures.h"

/*!
 * \brief Returns the minimum value between a and b.
 *
 * \param [in] a The first value.
 * \param [in] b The second value.
 * \retval minValue The minimum value.
 */
#ifndef MIN
#define MIN( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#endif

/*!
 * \brief Returns the maximum value between a and b
 *
 * \param [in] a The first value.
 * \param [in] b The second value.
 * \retval maxValue The maximum value.
 */
#ifndef MAX
#define MAX( a, b ) ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#endif

/**
 * LoRaMac maximum number of channels.
 */
#define LORA_MAX_NB_CHANNELS                        16

/*!
 * Macro to compute bit of a channel index.
 */
#define LC( channelIndex )                          ( uint16_t )( 1 << ( channelIndex - 1 ) )




/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | Max EIRP
 * AU915        | Max EIRP
 * CN470        | Max EIRP
 * CN779        | Max EIRP
 * EU433        | Max EIRP
 * EU868        | Max EIRP
 * IN865        | Max EIRP
 * KR920        | Max EIRP
 * US915        | Max ERP
 * US915_HYBRID | Max ERP
 */
#define TX_POWER_0                                  0

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | Max EIRP - 2
 * AU915        | Max EIRP - 2
 * CN470        | Max EIRP - 2
 * CN779        | Max EIRP - 2
 * EU433        | Max EIRP - 2
 * EU868        | Max EIRP - 2
 * IN865        | Max EIRP - 2
 * KR920        | Max EIRP - 2
 * US915        | Max ERP - 2
 * US915_HYBRID | Max ERP - 2
 */
#define TX_POWER_1                                  1

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | Max EIRP - 4
 * AU915        | Max EIRP - 4
 * CN470        | Max EIRP - 4
 * CN779        | Max EIRP - 4
 * EU433        | Max EIRP - 4
 * EU868        | Max EIRP - 4
 * IN865        | Max EIRP - 4
 * KR920        | Max EIRP - 4
 * US915        | Max ERP - 4
 * US915_HYBRID | Max ERP - 4
 */
#define TX_POWER_2                                  2

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | Max EIRP - 6
 * AU915        | Max EIRP - 6
 * CN470        | Max EIRP - 6
 * CN779        | Max EIRP - 6
 * EU433        | Max EIRP - 6
 * EU868        | Max EIRP - 6
 * IN865        | Max EIRP - 6
 * KR920        | Max EIRP - 6
 * US915        | Max ERP - 6
 * US915_HYBRID | Max ERP - 6
 */
#define TX_POWER_3                                  3

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | Max EIRP - 8
 * AU915        | Max EIRP - 8
 * CN470        | Max EIRP - 8
 * CN779        | Max EIRP - 8
 * EU433        | Max EIRP - 8
 * EU868        | Max EIRP - 8
 * IN865        | Max EIRP - 8
 * KR920        | Max EIRP - 8
 * US915        | Max ERP - 8
 * US915_HYBRID | Max ERP - 8
 */
#define TX_POWER_4                                  4

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | Max EIRP - 10
 * AU915        | Max EIRP - 10
 * CN470        | Max EIRP - 10
 * CN779        | Max EIRP - 10
 * EU433        | Max EIRP - 10
 * EU868        | Max EIRP - 10
 * IN865        | Max EIRP - 10
 * KR920        | Max EIRP - 10
 * US915        | Max ERP - 10
 * US915_HYBRID | Max ERP - 10
 */
#define TX_POWER_5                                  5

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | Max EIRP - 12
 * AU915        | Max EIRP - 12
 * CN470        | Max EIRP - 12
 * CN779        | -
 * EU433        | -
 * EU868        | Max EIRP - 12
 * IN865        | Max EIRP - 12
 * KR920        | Max EIRP - 12
 * US915        | Max ERP - 12
 * US915_HYBRID | Max ERP - 12
 */
#define TX_POWER_6                                  6

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | Max EIRP - 14
 * AU915        | Max EIRP - 14
 * CN470        | Max EIRP - 14
 * CN779        | -
 * EU433        | -
 * EU868        | Max EIRP - 14
 * IN865        | Max EIRP - 14
 * KR920        | Max EIRP - 14
 * US915        | Max ERP - 14
 * US915_HYBRID | Max ERP - 14
 */
#define TX_POWER_7                                  7

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | -
 * AU915        | Max EIRP - 16
 * CN470        | -
 * CN779        | -
 * EU433        | -
 * EU868        | -
 * IN865        | Max EIRP - 16
 * KR920        | -
 * US915        | Max ERP - 16
 * US915_HYBRID | Max ERP -16
 */
#define TX_POWER_8                                  8

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | -
 * AU915        | Max EIRP - 18
 * CN470        | -
 * CN779        | -
 * EU433        | -
 * EU868        | -
 * IN865        | Max EIRP - 18
 * KR920        | -
 * US915        | Max ERP - 16
 * US915_HYBRID | Max ERP - 16
 */
#define TX_POWER_9                                  9

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | -
 * AU915        | Max EIRP - 20
 * CN470        | -
 * CN779        | -
 * EU433        | -
 * EU868        | -
 * IN865        | Max EIRP - 20
 * KR920        | -
 * US915        | Max ERP - 10
 * US915_HYBRID | Max ERP - 10
 */
#define TX_POWER_10                                 10

/*!
 * RFU
 */
#define TX_POWER_11                                 11

/*!
 * RFU
 */
#define TX_POWER_12                                 12

/*!
 * RFU
 */
#define TX_POWER_13                                 13

/*!
 * RFU
 */
#define TX_POWER_14                                 14

/*!
 * RFU
 */
#define TX_POWER_15                                 15

/**
 * TX configuration parameters.
 */
typedef struct {
    /**
     * The TX channel.
     */
    uint8_t channel;
    /**
     * The TX datarate.
     */
    int8_t datarate;
    /**
     * The TX power.
     */
    int8_t tx_power;
    /**
     * The Max EIRP, if applicable.
     */
    float max_eirp;
    /**
     * The antenna gain, if applicable.
     */
    float antenna_gain;
    /**
     * The frame length to set up.
     */
    uint16_t pkt_len;
} tx_config_params_t;

/**
 * This structure contains parameters for ADR request coming from
 * network server.
 */
typedef struct {
    /*!
     * A pointer to the payload containing the MAC commands.
     */
    const uint8_t* payload;
    /*!
     * The size of the payload.
     */
    uint8_t payload_size;
    /*!
     * The uplink dwell time.
     */
    uint8_t ul_dwell_time;
    /*!
     * Set to true, if ADR is enabled.
     */
    bool adr_enabled;
    /*!
     * The current datarate.
     */
    int8_t current_datarate;
    /*!
     * The current TX power.
     */
    int8_t current_tx_power;
    /*!
     * The current number of repetitions.
     */
    uint8_t current_nb_rep;
} adr_req_params_t;

/**
 * Structure containing data for local ADR settings
 */
typedef struct link_adr_params_s {
    /**
     * The number of repetitions.
     */
    uint8_t nb_rep;
    /**
     * Datarate.
     */
    int8_t datarate;
    /**
     * TX power.
     */
    int8_t tx_power;
    /**
     * Channels mask control field.
     */
    uint8_t ch_mask_ctrl;
    /**
     * Channels mask field.
     */
    uint16_t channel_mask;
} link_adr_params_t;

/**
 * Structure used to store ADR values received from network
 * for verification (legality) purposes.
 */
typedef struct verify_adr_params_s {
    /*!
     * The current status of the AdrLinkRequest.
     */
    uint8_t status;
    /*!
     * Set to true, if ADR is enabled.
     */
    bool adr_enabled;
    /*!
     * The datarate the AdrLinkRequest wants to set.
     */
    int8_t datarate;
    /*!
     * The TX power the AdrLinkRequest wants to set.
     */
    int8_t tx_power;
    /*!
     * The number of repetitions the AdrLinkRequest wants to set.
     */
    uint8_t nb_rep;
    /*!
     * The current datarate the node is using.
     */
    int8_t current_datarate;
    /*!
     * The current TX power the node is using.
     */
    int8_t current_tx_power;
    /*!
     * The current number of repetitions the node is using.
     */
    int8_t current_nb_rep;

    /*!
     * A pointer to the first element of the channels mask.
     */
    uint16_t* channel_mask;
} verify_adr_params_t;

/**
 * Contains rx parameter setup request coming from
 * network server.
 */
typedef struct rx_param_setup_req_s {
    /**
     * The datarate to set up.
     */
    int8_t datarate;
    /**
     * The datarate offset.
     */
    int8_t dr_offset;
    /**
     * The frequency to set up.
     */
    uint32_t frequency;
} rx_param_setup_req_t;

/**
 * The parameter structure for the function RegionNextChannel.
 */
typedef struct channel_selection_params_s {
    /**
     * The aggregated time-off time.
     */
    lorawan_time_t aggregate_timeoff;
    /**
     * The time of the last aggregated TX.
     */
    lorawan_time_t last_aggregate_tx_time;
    /**
     * The current datarate.
     */
    int8_t current_datarate;
    /**
     * Set to true, if the node has already joined a network, otherwise false.
     */
    bool joined;
    /**
     * Set to true, if the duty cycle is enabled, otherwise false.
     */
    bool dc_enabled;
} channel_selection_params_t;

/*!
 * The parameter structure for the function RegionContinuousWave.
 */
typedef struct continuous_wave_mode_params_s {
    /*!
     * The current channel index.
     */
    uint8_t channel;
    /*!
     * The datarate. Used to limit the TX power.
     */
    int8_t datarate;
    /*!
     * The TX power to set up.
     */
    int8_t tx_power;
    /*!
     * The max EIRP, if applicable.
     */
    float max_eirp;
    /*!
     * The antenna gain, if applicable.
     */
    float antenna_gain;
    /*!
     * Specifies the time the radio will stay in CW mode.
     */
    uint16_t timeout;
} cw_mode_params_t;

typedef struct {
    void *table;
    uint8_t size;
} loraphy_table_t;

typedef struct {

    uint8_t channel_list_size;
    uint8_t  mask_size;

    uint16_t *mask;
    uint16_t *default_mask;
    channel_params_t *channel_list;
} loraphy_channels_t;

typedef struct {
    bool duty_cycle_enabled;
    bool accept_tx_param_setup_req;
    bool fsk_supported;
    bool cflist_supported;
    bool custom_channelplans_supported;
    bool dl_channel_req_supported;

    uint8_t default_channel_cnt;
    uint8_t cflist_channel_cnt;
    uint8_t max_channel_cnt;
    uint8_t min_tx_power;
    uint8_t max_tx_power;
    uint8_t default_tx_power;
    uint8_t adr_ack_limit;
    uint8_t adr_ack_delay;

    uint8_t min_tx_datarate;
    uint8_t max_tx_datarate;
    uint8_t min_rx_datarate;
    uint8_t max_rx_datarate;
    uint8_t default_datarate;
    uint8_t default_max_datarate;
    uint8_t min_rx1_dr_offset;
    uint8_t max_rx1_dr_offset;
    uint8_t default_rx1_dr_offset;
    uint8_t dwell_limit_datarate;

    uint16_t max_rx_window;
    uint16_t recv_delay1;
    uint16_t recv_delay2;
    uint16_t join_accept_delay1;
    uint16_t join_accept_delay2;
    uint16_t join_channel_mask;
    uint16_t max_fcnt_gap;
    uint16_t ack_timeout;
    uint16_t ack_timeout_rnd;

    float default_max_eirp;
    float default_antenna_gain;

    uint8_t rx_window2_datarate;
    uint32_t rx_window2_frequency;

    loraphy_table_t bands;
    loraphy_table_t bandwidths;
    loraphy_table_t datarates;
    loraphy_table_t payloads;
    loraphy_table_t payloads_with_repeater;

    loraphy_channels_t channels;


    unsigned ul_dwell_time_setting : 1;
    unsigned dl_dwell_time_setting : 1;

} loraphy_params_t;


#endif /* MBED_OS_LORA_PHY_DATASTRUCTURES_ */
