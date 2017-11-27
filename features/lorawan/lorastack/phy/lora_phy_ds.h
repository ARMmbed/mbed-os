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
typedef enum ePhyAttribute
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
    PHY_CHANNELS_MASK,
    /*!
     * The channels default mask.
     */
    PHY_CHANNELS_DEFAULT_MASK,
    /*!
     * The maximum number of supported channels.
     */
    PHY_MAX_NB_CHANNELS,
    /*!
     * The channels.
     */
    PHY_CHANNELS,
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
}PhyAttribute_t;

/*!
 * Enumeration of initialization types.
 */
typedef enum eInitType
{
    /*!
     * Performs an initialization and overwrites all existing data.
     */
    INIT_TYPE_INIT,
    /*!
     * Restores default channels only.
     */
    INIT_TYPE_RESTORE
}InitType_t;

/*!
 * Selects a given or a default channel mask.
 */
typedef enum eChannelsMask
{
    /*!
     * The channels mask.
     */
    CHANNELS_MASK,
    /*!
     * The channels default mask.
     */
    CHANNELS_DEFAULT_MASK
}ChannelsMask_t;

/*!
 * The union for the structure uGetPhyParams.
 */
typedef union uPhyParam
{
    /*!
     * A parameter value.
     */
    uint32_t Value;
    /*!
     * A floating point value.
     */
    float fValue;
    /*!
     * A pointer to the channels mask.
     */
    uint16_t* ChannelsMask;
    /*!
     * A pointer to the channels.
     */
    ChannelParams_t* Channels;
}PhyParam_t;

/*!
 * The parameter structure for the function RegionGetPhyParam.
 */
typedef struct sGetPhyParams
{
    /*!
     * Set up the parameter to get.
     */
    PhyAttribute_t Attribute;
    /*!
     * Datarate.
     * The parameter is needed for the following queries:
     * PHY_MAX_PAYLOAD, PHY_MAX_PAYLOAD_REPEATER, PHY_NEXT_LOWER_TX_DR.
     */
    int8_t Datarate;
    /*!
     * Uplink dwell time.
     * The parameter is needed for the following queries:
     * PHY_MIN_TX_DR, PHY_MAX_PAYLOAD, PHY_MAX_PAYLOAD_REPEATER, PHY_NEXT_LOWER_TX_DR.
     */
    uint8_t UplinkDwellTime;
    /*!
     * Downlink dwell time.
     * The parameter is needed for the following queries:
     * PHY_MIN_RX_DR, PHY_MAX_PAYLOAD, PHY_MAX_PAYLOAD_REPEATER.
     */
    uint8_t DownlinkDwellTime;
}GetPhyParams_t;

/*!
 * The parameter structure for the function RegionSetBandTxDone.
 */
typedef struct sSetBandTxDoneParams
{
    /*!
     * The channel to update.
     */
    uint8_t Channel;
    /*!
     * Joined set to true, if the node has joined the network.
     */
    bool Joined;
    /*!
     * The last TX done time.
     */
    TimerTime_t LastTxDoneTime;
}SetBandTxDoneParams_t;

/*!
 * The parameter structure for the function RegionVerify.
 */
typedef union uVerifyParams
{
    /*!
     * The TX power to verify.
     */
    int8_t TxPower;
    /*!
     * Set to true, if the duty cycle is enabled, otherwise false.
     */
    bool DutyCycle;
    /*!
     * The number of join trials.
     */
    uint8_t NbJoinTrials;
    /*!
     * The datarate to verify.
     */
    struct sDatarateParams
    {
        /*!
        * The datarate to verify.
        */
        int8_t Datarate;
        /*!
        * The downlink dwell time.
        */
        uint8_t DownlinkDwellTime;
        /*!
        * The uplink dwell time.
        */
        uint8_t UplinkDwellTime;
    }DatarateParams;
}VerifyParams_t;

/*!
 * The parameter structure for the function RegionApplyCFList.
 */
typedef struct sApplyCFListParams
{
    /*!
     * The payload containing the CF list.
     */
    uint8_t* Payload;
    /*!
     * The size of the payload.
     */
    uint8_t Size;
}ApplyCFListParams_t;

/*!
 * The parameter structure for the function RegionChanMaskSet.
 */
typedef struct sChanMaskSetParams
{
    /*!
     * A pointer to the channels mask which should be set.
     */
    uint16_t* ChannelsMaskIn;
    /*!
     * A pointer to the channels mask which should be set.
     */
    ChannelsMask_t ChannelsMaskType;
}ChanMaskSetParams_t;

/*!
 * The parameter structure for the function RegionAdrNext.
 */
typedef struct sAdrNextParams
{
    /*!
     * Set to true, if the function should update the channels mask.
     */
    bool UpdateChanMask;
    /*!
     * Set to true, if ADR is enabled.
     */
    bool AdrEnabled;
    /*!
     * The ADR ack counter.
     */
    uint32_t AdrAckCounter;
    /*!
     * The datarate used currently.
     */
    int8_t Datarate;
    /*!
     * The TX power used currently.
     */
    int8_t TxPower;
    /*!
     * UplinkDwellTime
     */
    uint8_t UplinkDwellTime;
}AdrNextParams_t;

/*!
 * The parameter structure for the function RegionRxConfig.
 */
typedef struct sRxConfigParams
{
    /*!
     * The RX channel.
     */
    uint8_t Channel;
    /*!
     * The RX datarate.
     */
    int8_t Datarate;
    /*!
     * The RX bandwidth.
     */
    uint8_t Bandwidth;
    /*!
     * The RX datarate offset.
     */
    int8_t DrOffset;
    /*!
     * The RX frequency.
     */
    uint32_t Frequency;
    /*!
     * The RX window timeout
     */
     uint32_t WindowTimeout;
    /*!
     * The RX window offset
     */
    int32_t WindowOffset;
    /*!
     * The downlink dwell time.
     */
    uint8_t DownlinkDwellTime;
    /*!
     * Set to true, if a repeater is supported.
     */
    bool RepeaterSupport;
    /*!
     * Set to true, if RX should be continuous.
     */
    bool RxContinuous;
    /*!
     * Sets the RX window. 0: RX window 1, 1: RX window 2.
     */
    bool Window;
}RxConfigParams_t;

/*!
 * The parameter structure for the function RegionTxConfig.
 */
typedef struct sTxConfigParams
{
    /*!
     * The TX channel.
     */
    uint8_t Channel;
    /*!
     * The TX datarate.
     */
    int8_t Datarate;
    /*!
     * The TX power.
     */
    int8_t TxPower;
    /*!
     * The Max EIRP, if applicable.
     */
    float MaxEirp;
    /*!
     * The antenna gain, if applicable.
     */
    float AntennaGain;
    /*!
     * The frame length to set up.
     */
    uint16_t PktLen;
}TxConfigParams_t;

/*!
 * The parameter structure for the function RegionLinkAdrReq.
 */
typedef struct sLinkAdrReqParams
{
    /*!
     * A pointer to the payload containing the MAC commands.
     */
    uint8_t* Payload;
    /*!
     * The size of the payload.
     */
    uint8_t PayloadSize;
    /*!
     * The uplink dwell time.
     */
    uint8_t UplinkDwellTime;
    /*!
     * Set to true, if ADR is enabled.
     */
    bool AdrEnabled;
    /*!
     * The current datarate.
     */
    int8_t CurrentDatarate;
    /*!
     * The current TX power.
     */
    int8_t CurrentTxPower;
    /*!
     * The current number of repetitions.
     */
    uint8_t CurrentNbRep;
}LinkAdrReqParams_t;

/*!
 * The parameter structure for the function RegionRxParamSetupReq.
 */
typedef struct sRxParamSetupReqParams
{
    /*!
     * The datarate to set up.
     */
    int8_t Datarate;
    /*!
     * The datarate offset.
     */
    int8_t DrOffset;
    /*!
     * The frequency to set up.
     */
    uint32_t Frequency;
}RxParamSetupReqParams_t;

/*!
 * The parameter structure for the function RegionNewChannelReq.
 */
typedef struct sNewChannelReqParams
{
    /*!
     * A pointer to the new channels.
     */
    ChannelParams_t* NewChannel;
    /*!
     * The channel ID.
     */
    int8_t ChannelId;
}NewChannelReqParams_t;

/*!
 * The parameter structure for the function RegionTxParamSetupReq.
 */
typedef struct sTxParamSetupReqParams
{
    /*!
     * The uplink dwell time.
     */
    uint8_t UplinkDwellTime;
    /*!
     * The downlink dwell time.
     */
    uint8_t DownlinkDwellTime;
    /*!
     * The max EIRP.
     */
    uint8_t MaxEirp;
}TxParamSetupReqParams_t;

/*!
 * The parameter structure for the function RegionDlChannelReq.
 */
typedef struct sDlChannelReqParams
{
    /*!
     * The channel ID to add the frequency.
     */
    uint8_t ChannelId;
    /*!
     * The alternative frequency for the Rx1 window.
     */
    uint32_t Rx1Frequency;
}DlChannelReqParams_t;

/*!
 * The parameter structure for the function RegionAlternateDr.
 */
typedef struct sAlternateDrParams
{
    /*!
     * The number of trials.
     */
    uint16_t NbTrials;
}AlternateDrParams_t;

/*!
 * The parameter structure for the function RegionCalcBackOff.
 */
typedef struct sCalcBackOffParams
{
    /*!
     * Set to true, if the node has already joined a network, otherwise false.
     */
    bool Joined;
    /*!
     * Joined set to true, if the last uplink was a join request.
     */
    bool LastTxIsJoinRequest;
    /*!
     * Set to true, if the duty cycle is enabled, otherwise false.
     */
    bool DutyCycleEnabled;
    /*!
     * The current channel index.
     */
    uint8_t Channel;
    /*!
     * Elapsed time since the start of the node.
     */
    TimerTime_t ElapsedTime;
    /*!
     * Time-on-air of the last transmission.
     */
    TimerTime_t TxTimeOnAir;
}CalcBackOffParams_t;

/*!
 * The parameter structure for the function RegionNextChannel.
 */
typedef struct sNextChanParams
{
    /*!
     * The aggregated time-off time.
     */
    TimerTime_t AggrTimeOff;
    /*!
     * The time of the last aggregated TX.
     */
    TimerTime_t LastAggrTx;
    /*!
     * The current datarate.
     */
    int8_t Datarate;
    /*!
     * Set to true, if the node has already joined a network, otherwise false.
     */
    bool Joined;
    /*!
     * Set to true, if the duty cycle is enabled, otherwise false.
     */
    bool DutyCycleEnabled;
}NextChanParams_t;

/*!
 * The parameter structure for the function RegionChannelsAdd.
 */
typedef struct sChannelAddParams
{
    /*!
     * A pointer to the new channel to add.
     */
    ChannelParams_t* NewChannel;
    /*!
     * The channel ID to add.
     */
    uint8_t ChannelId;
}ChannelAddParams_t;

/*!
 * The parameter structure for the function RegionChannelsRemove.
 */
typedef struct sChannelRemoveParams
{
    /*!
     * The channel ID to remove.
     */
    uint8_t ChannelId;
}ChannelRemoveParams_t;

/*!
 * The parameter structure for the function RegionContinuousWave.
 */
typedef struct sContinuousWaveParams
{
    /*!
     * The current channel index.
     */
    uint8_t Channel;
    /*!
     * The datarate. Used to limit the TX power.
     */
    int8_t Datarate;
    /*!
     * The TX power to set up.
     */
    int8_t TxPower;
    /*!
     * The max EIRP, if applicable.
     */
    float MaxEirp;
    /*!
     * The antenna gain, if applicable.
     */
    float AntennaGain;
    /*!
     * Specifies the time the radio will stay in CW mode.
     */
    uint16_t Timeout;
}ContinuousWaveParams_t;


#endif /* MBED_OS_LORA_PHY_DATASTRUCTURES_ */
