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

#include "lorawan/system/lorawan_data_structures.h"

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
 * Region       | SF
 * ------------ | :-----:
 * AS923        | SF12 - BW125
 * AU915        | SF10 - BW125
 * CN470        | SF12 - BW125
 * CN779        | SF12 - BW125
 * EU433        | SF12 - BW125
 * EU868        | SF12 - BW125
 * IN865        | SF12 - BW125
 * KR920        | SF12 - BW125
 * US915        | SF10 - BW125
 * US915_HYBRID | SF10 - BW125
 */
#define DR_0                                        0

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | SF11 - BW125
 * AU915        | SF9  - BW125
 * CN470        | SF11 - BW125
 * CN779        | SF11 - BW125
 * EU433        | SF11 - BW125
 * EU868        | SF11 - BW125
 * IN865        | SF11 - BW125
 * KR920        | SF11 - BW125
 * US915        | SF9  - BW125
 * US915_HYBRID | SF9  - BW125
 */
#define DR_1                                        1

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | SF10 - BW125
 * AU915        | SF8  - BW125
 * CN470        | SF10 - BW125
 * CN779        | SF10 - BW125
 * EU433        | SF10 - BW125
 * EU868        | SF10 - BW125
 * IN865        | SF10 - BW125
 * KR920        | SF10 - BW125
 * US915        | SF8  - BW125
 * US915_HYBRID | SF8  - BW125
 */
#define DR_2                                        2

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | SF9  - BW125
 * AU915        | SF7  - BW125
 * CN470        | SF9  - BW125
 * CN779        | SF9  - BW125
 * EU433        | SF9  - BW125
 * EU868        | SF9  - BW125
 * IN865        | SF9  - BW125
 * KR920        | SF9  - BW125
 * US915        | SF7  - BW125
 * US915_HYBRID | SF7  - BW125
 */
#define DR_3                                        3

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | SF8  - BW125
 * AU915        | SF8  - BW500
 * CN470        | SF8  - BW125
 * CN779        | SF8  - BW125
 * EU433        | SF8  - BW125
 * EU868        | SF8  - BW125
 * IN865        | SF8  - BW125
 * KR920        | SF8  - BW125
 * US915        | SF8  - BW500
 * US915_HYBRID | SF8  - BW500
 */
#define DR_4                                        4

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | SF7  - BW125
 * AU915        | RFU
 * CN470        | SF7  - BW125
 * CN779        | SF7  - BW125
 * EU433        | SF7  - BW125
 * EU868        | SF7  - BW125
 * IN865        | SF7  - BW125
 * KR920        | SF7  - BW125
 * US915        | RFU
 * US915_HYBRID | RFU
 */
#define DR_5                                        5

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | SF7  - BW250
 * AU915        | RFU
 * CN470        | SF12 - BW125
 * CN779        | SF7  - BW250
 * EU433        | SF7  - BW250
 * EU868        | SF7  - BW250
 * IN865        | SF7  - BW250
 * KR920        | RFU
 * US915        | RFU
 * US915_HYBRID | RFU
 */
#define DR_6                                        6

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | FSK
 * AU915        | RFU
 * CN470        | SF12 - BW125
 * CN779        | FSK
 * EU433        | FSK
 * EU868        | FSK
 * IN865        | FSK
 * KR920        | RFU
 * US915        | RFU
 * US915_HYBRID | RFU
 */
#define DR_7                                        7

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | RFU
 * AU915        | SF12 - BW500
 * CN470        | RFU
 * CN779        | RFU
 * EU433        | RFU
 * EU868        | RFU
 * IN865        | RFU
 * KR920        | RFU
 * US915        | SF12 - BW500
 * US915_HYBRID | SF12 - BW500
 */
#define DR_8                                        8

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | RFU
 * AU915        | SF11 - BW500
 * CN470        | RFU
 * CN779        | RFU
 * EU433        | RFU
 * EU868        | RFU
 * IN865        | RFU
 * KR920        | RFU
 * US915        | SF11 - BW500
 * US915_HYBRID | SF11 - BW500
 */
#define DR_9                                        9

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | RFU
 * AU915        | SF10 - BW500
 * CN470        | RFU
 * CN779        | RFU
 * EU433        | RFU
 * EU868        | RFU
 * IN865        | RFU
 * KR920        | RFU
 * US915        | SF10 - BW500
 * US915_HYBRID | SF10 - BW500
 */
#define DR_10                                       10

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | RFU
 * AU915        | SF9  - BW500
 * CN470        | RFU
 * CN779        | RFU
 * EU433        | RFU
 * EU868        | RFU
 * IN865        | RFU
 * KR920        | RFU
 * US915        | SF9  - BW500
 * US915_HYBRID | SF9  - BW500
 */
#define DR_11                                       11

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | RFU
 * AU915        | SF8  - BW500
 * CN470        | RFU
 * CN779        | RFU
 * EU433        | RFU
 * EU868        | RFU
 * IN865        | RFU
 * KR920        | RFU
 * US915        | SF8  - BW500
 * US915_HYBRID | SF8  - BW500
 */
#define DR_12                                       12

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | RFU
 * AU915        | SF7  - BW500
 * CN470        | RFU
 * CN779        | RFU
 * EU433        | RFU
 * EU868        | RFU
 * IN865        | RFU
 * KR920        | RFU
 * US915        | SF7  - BW500
 * US915_HYBRID | SF7  - BW500
 */
#define DR_13                                       13

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | RFU
 * AU915        | RFU
 * CN470        | RFU
 * CN779        | RFU
 * EU433        | RFU
 * EU868        | RFU
 * IN865        | RFU
 * KR920        | RFU
 * US915        | RFU
 * US915_HYBRID | RFU
 */
#define DR_14                                       14

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | RFU
 * AU915        | RFU
 * CN470        | RFU
 * CN779        | RFU
 * EU433        | RFU
 * EU868        | RFU
 * IN865        | RFU
 * KR920        | RFU
 * US915        | RFU
 * US915_HYBRID | RFU
 */
#define DR_15                                       15



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

/*!
 * Enumeration of PHY attributes.
 */
typedef enum phy_attributes__e
{
    /*!
     * The minimum RX datarate.
     */
    PHY_MIN_RX_DR,
    /*!
     * The minimum TX datarate.
     */
    PHY_MIN_TX_DR,
    /*!
     * The maximum RX datarate.
     */
    PHY_MAX_RX_DR,
    /*!
     * The maximum TX datarate.
     */
    PHY_MAX_TX_DR,
    /*!
     * The TX datarate.
     */
    PHY_TX_DR,
    /*!
     * The default TX datarate.
     */
    PHY_DEF_TX_DR,
    /*!
     * The RX datarate.
     */
    PHY_RX_DR,
    /*!
     * The TX power.
     */
    PHY_TX_POWER,
    /*!
     * The default TX power.
     */
    PHY_DEF_TX_POWER,
    /*!
     * Maximum payload possible.
     */
    PHY_MAX_PAYLOAD,
    /*!
     * Maximum payload possible when the repeater support is enabled.
     */
    PHY_MAX_PAYLOAD_REPEATER,
    /*!
     * The duty cycle.
     */
    PHY_DUTY_CYCLE,
    /*!
     * The maximum receive window duration.
     */
    PHY_MAX_RX_WINDOW,
    /*!
     * The receive delay for window 1.
     */
    PHY_RECEIVE_DELAY1,
    /*!
     * The receive delay for window 2.
     */
    PHY_RECEIVE_DELAY2,
    /*!
     * The join accept delay for window 1.
     */
    PHY_JOIN_ACCEPT_DELAY1,
    /*!
     * The join accept delay for window 2.
     */
    PHY_JOIN_ACCEPT_DELAY2,
    /*!
     * The maximum frame counter gap.
     */
    PHY_MAX_FCNT_GAP,
    /*!
     * The acknowledgement time out.
     */
    PHY_ACK_TIMEOUT,
    /*!
     * The default datarate offset for window 1.
     */
    PHY_DEF_DR1_OFFSET,
    /*!
     * The default receive window 2 frequency.
     */
    PHY_DEF_RX2_FREQUENCY,
    /*!
     * The default receive window 2 datarate.
     */
    PHY_DEF_RX2_DR,
    /*!
     * The channels mask.
     */
    PHY_CHANNEL_MASK,
    /*!
     * The default channel mask.
     */
    PHY_DEFAULT_CHANNEL_MASK,
    /*!
     * The maximum number of supported channels.
     */
    PHY_MAX_NB_CHANNELS,
    /*!
     * The channels.
     */
    PHY_CHANNELS,
    /*!
     * The PHYs that support dynamic channel plan (non-custom)
     * support do not let the user add/remove to the channel plan.
     * The network guides the device for the plan. So only
     * EU like regions support custom channel planning.
     */
    PHY_CUSTOM_CHANNEL_PLAN_SUPPORT,
    /*!
     * The default value of the uplink dwell time.
     */
    PHY_DEF_UPLINK_DWELL_TIME,
    /*!
     * The default value of the downlink dwell time.
     */
    PHY_DEF_DOWNLINK_DWELL_TIME,
    /*!
     * The default value of the MaxEIRP.
     */
    PHY_DEF_MAX_EIRP,
    /*!
     * The default value of the antenna gain.
     */
    PHY_DEF_ANTENNA_GAIN,
    /*!
     * The value for the number of join trials.
     */
    PHY_NB_JOIN_TRIALS,
    /*!
     * The default value for the number of join trials.
     */
    PHY_DEF_NB_JOIN_TRIALS,
    /*!
     * The next lower datarate.
     */
    PHY_NEXT_LOWER_TX_DR
} phy_attributes_t;

/*!
 * Keeps value in response to a call to
 * get_phy_params() API.
 */
typedef union phy_param_u
{
    /*!
     * A parameter value.
     */
    uint32_t value;
    /*!
     * A floating point value.
     */
    float f_value;
    /*!
     * A pointer to the channels mask.
     */
    uint16_t* channel_mask;
    /*!
     * A pointer to the channels.
     */
    channel_params_t* channel_params;
} phy_param_t;

/**
 * The parameter structure for the function RegionGetPhyParam.
 */
typedef struct
{
    /**
     * Set up the parameter to get.
     */
    phy_attributes_t attribute;

    /**
     * The parameter is needed for the following queries:
     * PHY_MAX_PAYLOAD, PHY_MAX_PAYLOAD_REPEATER, PHY_NEXT_LOWER_TX_DR.
     */
    int8_t datarate;

} get_phy_params_t;

/**
 * The parameter structure for the function RegionSetBandTxDone.
 */
typedef struct
{
    /**
     * The channel to update.
     */
    uint8_t channel;
    /**
     * Joined set to true, if the node has joined the network.
     */
    bool joined;
    /**
     * The last TX done time.
     */
    lorawan_time_t last_tx_done_time;
} set_band_txdone_params_t;

/**
 * The parameter verification structure.
 */
typedef union
{
    /**
     * The TX power to verify.
     */
    int8_t tx_power;
    /**
     * Set to true, if the duty cycle is enabled, otherwise false.
     */
    bool duty_cycle;
    /**
     * The number of join trials.
     */
    uint8_t nb_join_trials;
    /**
     * The datarate to verify.
     */
    int8_t datarate;

} verification_params_t;

/**
 * The parameter structure for the function RegionApplyCFList.
 */
typedef struct
{
    /**
     * The payload containing the CF list.
     */
    uint8_t* payload;
    /**
     * The size of the payload.
     */
    uint8_t size;
} cflist_params_t;

/**
 * TX configuration parameters.
 */
typedef struct
{
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
typedef struct
{
    /*!
     * A pointer to the payload containing the MAC commands.
     */
    uint8_t* payload;
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
typedef struct link_adr_params_s
{
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
typedef struct verify_adr_params_s
{
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
typedef struct rx_param_setup_req_s
{
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
 * Contains tx parameter setup request coming from
 * network server.
 */
typedef struct tx_param_setup_req_s
{
    /**
     * The uplink dwell time.
     */
    uint8_t ul_dwell_time;
    /**
     * The downlink dwell time.
     */
    uint8_t dl_dwell_time;
    /**
     * The max EIRP.
     */
    uint8_t max_eirp;
} tx_param_setup_req_t;

/**
 * A structure that holds new channel parameters coming
 * from the network server.
 */
typedef struct new_channel_req_params_s
{
    /**
     * A pointer to the new channel's parameters.
     */
    channel_params_t* new_channel;
    /**
     * The channel ID.
     */
    int8_t channel_id;

} new_channel_req_params_t;

/**
 * The parameter structure for the function RegionDlChannelReq.
 */
typedef struct dl_channel_req_params_s
{
    /**
     * The channel ID to add the frequency.
     */
    uint8_t channel_id;
    /**
     * The alternative frequency for the Rx1 window.
     */
    uint32_t rx1_frequency;
} dl_channel_req_params_t;

/*!
 * The parameter structure for the function RegionCalcBackOff.
 */
typedef struct backoff_params_s
{
    /**
     * Set to true, if the node has already joined a network, otherwise false.
     */
    bool joined;
    /**
     * set to true, if the last uplink was a join request.
     */
    bool last_tx_was_join_req;
    /**
     * Set to true, if the duty cycle is enabled, otherwise false.
     */
    bool dc_enabled;
    /**
     * The current channel index.
     */
    uint8_t channel;
    /**
     * Elapsed time since the start of the node.
     */
    lorawan_time_t elapsed_time;
    /**
     * Time-on-air of the last transmission.
     */
    lorawan_time_t tx_toa;

} backoff_params_t;

/**
 * The parameter structure for the function RegionNextChannel.
 */
typedef struct channel_selection_params_s
{
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
typedef struct continuous_wave_mode_params_s
{
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
    channel_params_t *channel_list;
    uint8_t channel_list_size;

    uint16_t *mask;
    uint16_t *default_mask;
    uint8_t  mask_size;

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
