/**
 * @file lorawan_data_structures.h
 *
 * @brief Contains common data structures used by Mbed-OS
 *        LoRaWAN mplementation.
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
 * Description: LoRa PHY layer
 *
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *
 * Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef LORAWAN_SYSTEM_LORAWAN_DATA_STRUCTURES_H_
#define LORAWAN_SYSTEM_LORAWAN_DATA_STRUCTURES_H_

#include <inttypes.h>
#include "platform/Callback.h"

/*!
 * \brief Timer time variable definition
 */
#ifndef lorawan_time_t
typedef uint32_t lorawan_time_t;
#endif

// Radio wake-up time from sleep - unit ms.
#define RADIO_WAKEUP_TIME                           1

/**
 * Option Flags for send(), receive() APIs
 */
#define MSG_UNCONFIRMED_FLAG                  0x01
#define MSG_CONFIRMED_FLAG                    0x02
#define MSG_MULTICAST_FLAG                    0x04
#define MSG_PROPRIETARY_FLAG                  0x08

/**
 * A macro to test a if a bit is on in a channel mask or not.
 */
//#define MASK_BIT_TEST(mask, bit)    (mask & (1U << bit))
//#define MASK_BIT_TEST(mask, bit)    ((mask)[(bit) / 16] & (1U << ((bit) % 16)))
/**
 * A macro to clear a bit in a channel mask.
 */
//#define MASK_BIT_CLEAR(mask, bit)   (mask &= ~(1 << bit))

/**
 * A macro to clear a bit in a channel mask.
 */
//#define MASK_BIT_SET(mask, bit)   (mask |= (1 << bit))

/**
 * Bit mask for message flags
 */

#define MSG_FLAG_MASK                         0x0F

/**
 * Mask for unconfirmed multicast message
 */
#define MSG_UNCONFIRMED_MULTICAST              0x05

/**
 * Mask for confirmed multicast message
 */
#define MSG_CONFIRMED_MULTICAST                0x06

/**
 * Mask for unconfirmed message proprietary message
 */
#define MSG_UNCONFIRMED_PROPRIETARY            0x09

/**
 * Mask for confirmed proprietary message
 */
#define MSG_CONFIRMED_PROPRIETARY              0x0A

/*!
 * Sets the length of the LoRaMAC footer field.
 * Mainly indicates the MIC field length.
 */
#define LORAMAC_MFR_LEN                             4

/*!
 * The FRMPayload overhead to be used when setting the `Radio.SetMaxPayloadLength`
 * in the `RxWindowSetup` function.
 * The maximum PHYPayload = MaxPayloadOfDatarate/MaxPayloadOfDatarateRepeater + LORA_MAC_FRMPAYLOAD_OVERHEAD
 */
#define LORA_MAC_FRMPAYLOAD_OVERHEAD                13 // MHDR(1) + FHDR(7) + Port(1) + MIC(4)

/**
 * LoRaMac maximum number of channels
 */
#define LORA_MAX_NB_CHANNELS                        16

/**
 * Maximum PHY layer payload size for reception.
 */
#define LORAMAC_PHY_MAXPAYLOAD                      255

/**
 *
 * Default user application maximum data size for transmission
 */
// reject if user tries to set more than MTU
#if MBED_CONF_LORA_TX_MAX_SIZE > 255
    #warning "Cannot set TX Max size more than MTU=255"
    #define MBED_CONF_LORA_TX_MAX_SIZE              255
#endif

/*!
 * LoRaWAN device classes definition.
 *
 * LoRaWAN Specification V1.0.2, chapter 2.1.
 */
typedef enum {
    /*!
     * LoRaWAN device class A.
     *
     * LoRaWAN Specification V1.0.2, chapter 3.
     */
    CLASS_A,
    /*!
     * LoRaWAN device class B.
     *
     * LoRaWAN Specification V1.0.2, chapter 8.
     */
    CLASS_B,
    /*!
     * LoRaWAN device class C.
     *
     * LoRaWAN Specification V1.0.2, chapter 17.
     */
    CLASS_C,
} device_class_t;

/*!
 * LoRaMAC channel parameters definition.
 */
typedef union {
    /*!
     * Byte-access to the bits.
     */
    int8_t value;
    /*!
     * The structure to store the minimum and the maximum datarate.
     */
    struct sFields
    {
         /*!
         * The minimum data rate.
         *
         * LoRaWAN Regional Parameters V1.0.2rB.
         *
         * The allowed ranges are region-specific. Please refer to \ref DR_0 to \ref DR_15 for details.
         */
        int8_t min : 4;
        /*!
         * The maximum data rate.
         *
         * LoRaWAN Regional Parameters V1.0.2rB.
         *
         * The allowed ranges are region-specific. Please refer to \ref DR_0 to \ref DR_15 for details.
         */
        int8_t max : 4;
    } fields;
} dr_range_t;

/*!
 * LoRaMAC channel definition.
 */
typedef struct {
    /*!
     * The frequency in Hz.
     */
    uint32_t frequency;
    /*!
     * The alternative frequency for RX window 1.
     */
    uint32_t rx1_frequency;
    /*!
     * The data rate definition.
     */
    dr_range_t dr_range;
    /*!
     * The band index.
     */
    uint8_t band;
} channel_params_t;

/*!
 * LoRaMAC band parameters definition.
 */
typedef struct {
    /*!
     * The duty cycle.
     */
    uint16_t duty_cycle;
    /*!
     * The maximum TX power.
     */
    int8_t max_tx_pwr;
    /*!
     * The timestamp of the last Join Request TX frame.
     */
    lorawan_time_t last_join_tx_time;
    /*!
     * The timestamp of the last TX frame.
     */
    lorawan_time_t last_tx_time;
    /*!
     * The device off time.
     */
    lorawan_time_t off_time;
    /*!
     * Lower band boundry
     */
    uint32_t lower_band_freq;
    /*!
     * Higher band boundry
     */
    uint32_t higher_band_freq;
} band_t;

/*!
 * LoRaMAC receive window 2 channel parameters.
 */
typedef struct {
    /*!
     * The frequency in Hz.
     */
    uint32_t frequency;
    /*!
     * The data rate.
     *
     * LoRaWAN Regional Parameters V1.0.2rB.
     *
     * The allowed ranges are region-specific. Please refer to \ref DR_0 to \ref DR_15 for details.
     */
    uint8_t  datarate;
} rx2_channel_params;

/*!
 * LoRaMAC receive window enumeration
 */
typedef enum {
    /*!
     * LoRaMAC receive window 1
     */
    RX_SLOT_WIN_1,
    /*!
     * LoRaMAC receive window 2
     */
    RX_SLOT_WIN_2,
    /*!
     * LoRaMAC receive window 2 for class c - continuous listening
     */
    RX_SLOT_WIN_CLASS_C,
    /*!
     * LoRaMAC class b ping slot window
     */
    RX_SLOT_WIN_PING_SLOT
} rx_slot_t;

/*!
 * The global MAC layer parameters.
 */
typedef struct {
    /*!
     * The TX power in channels.
     */
    int8_t channel_tx_power;
    /*!
     * The data rate in channels.
     */
    int8_t channel_data_rate;
    /*!
     * The system overall timing error in milliseconds.
     * [-SystemMaxRxError : +SystemMaxRxError]
     * Default: +/-10 ms
     */
    uint32_t max_sys_rx_error;
    /*!
     * The minimum number of symbols required to detect an RX frame.
     * Default: 6 symbols
     */
    uint8_t min_rx_symb;
    /*!
     * LoRaMac maximum time a reception window stays open.
     */
    uint32_t max_rx_win_time;
    /*!
     * Receive delay 1.
     */
    uint32_t recv_delay1;
    /*!
     * Receive delay 2.
     */
    uint32_t recv_delay2;
    /*!
     * Join accept delay 1.
     */
    uint32_t join_accept_delay1;
    /*!
     * Join accept delay 1.
     */
    uint32_t join_accept_delay2;
    /*!
     * The number of uplink messages repetitions (confirmed messages only).
     */
    uint8_t retry_num;
    /*!
     * The datarate offset between uplink and downlink on first window.
     */
    uint8_t rx1_dr_offset;
    /*!
     * LoRaMAC 2nd reception window settings.
     */
    rx2_channel_params rx2_channel;
    /*!
     * The uplink dwell time configuration. 0: No limit, 1: 400ms
     */
    uint8_t uplink_dwell_time;
    /*!
     * The downlink dwell time configuration. 0: No limit, 1: 400ms
     */
    uint8_t downlink_dwell_time;
    /*!
     * The maximum possible EIRP.
     */
    float max_eirp;
    /*!
     * The antenna gain of the node.
     */
    float antenna_gain;

    /*!
     * Maximum duty cycle
     * \remark Possibility to shutdown the device.
     */
    uint8_t max_duty_cycle;
    /*!
     * Aggregated duty cycle management
     */
    uint16_t aggregated_duty_cycle;

    /*!
     * LoRaMac ADR control status
     */
    bool adr_on;
} lora_mac_system_params_t;

/*!
 * LoRaMAC multicast channel parameter.
 */
typedef struct multicast_params_s {
    /*!
     * Address.
     */
    uint32_t address;
    /*!
     * Network session key.
     */
    uint8_t nwk_skey[16];
    /*!
     * Application session key.
     */
    uint8_t app_skey[16];
    /*!
     * Downlink counter.
     */
    uint32_t dl_frame_counter;
    /*!
     * A reference pointer to the next multicast channel parameters in the list.
     */
    struct multicast_params_s *next;
} multicast_params_t;

/*!
 * LoRaMAC frame types.
 *
 * LoRaWAN Specification V1.0.2, chapter 4.2.1, table 1.
 */
typedef enum {
    /*!
     * LoRaMAC join request frame.
     */
    FRAME_TYPE_JOIN_REQ              = 0x00,
    /*!
     * LoRaMAC join accept frame.
     */
    FRAME_TYPE_JOIN_ACCEPT           = 0x01,
    /*!
     * LoRaMAC unconfirmed uplink frame.
     */
    FRAME_TYPE_DATA_UNCONFIRMED_UP   = 0x02,
    /*!
     * LoRaMAC unconfirmed downlink frame.
     */
    FRAME_TYPE_DATA_UNCONFIRMED_DOWN = 0x03,
    /*!
     * LoRaMAC confirmed uplink frame.
     */
    FRAME_TYPE_DATA_CONFIRMED_UP     = 0x04,
    /*!
     * LoRaMAC confirmed downlink frame.
     */
    FRAME_TYPE_DATA_CONFIRMED_DOWN   = 0x05,
    /*!
     * LoRaMAC RFU frame.
     */
    FRAME_TYPE_RFU                   = 0x06,
    /*!
     * LoRaMAC proprietary frame.
     */
    FRAME_TYPE_PROPRIETARY           = 0x07,
} mac_frame_type_t;

/*!
 * LoRaMAC mote MAC commands.
 *
 * LoRaWAN Specification V1.0.2, chapter 5, table 4.
 */
typedef enum {
    /*!
     * LinkCheckReq
     */
    MOTE_MAC_LINK_CHECK_REQ          = 0x02,
    /*!
     * LinkADRAns
     */
    MOTE_MAC_LINK_ADR_ANS            = 0x03,
    /*!
     * DutyCycleAns
     */
    MOTE_MAC_DUTY_CYCLE_ANS          = 0x04,
    /*!
     * RXParamSetupAns
     */
    MOTE_MAC_RX_PARAM_SETUP_ANS      = 0x05,
    /*!
     * DevStatusAns
     */
    MOTE_MAC_DEV_STATUS_ANS          = 0x06,
    /*!
     * NewChannelAns
     */
    MOTE_MAC_NEW_CHANNEL_ANS         = 0x07,
    /*!
     * RXTimingSetupAns
     */
    MOTE_MAC_RX_TIMING_SETUP_ANS     = 0x08,
    /*!
     * TXParamSetupAns
     */
    MOTE_MAC_TX_PARAM_SETUP_ANS      = 0x09,
    /*!
     * DlChannelAns
     */
    MOTE_MAC_DL_CHANNEL_ANS          = 0x0A
} mote_mac_cmds_t;

/*!
 * LoRaMAC server MAC commands.
 *
 * LoRaWAN Specification V1.0.2 chapter 5, table 4.
 */
typedef enum {
    /*!
     * LinkCheckAns
     */
    SRV_MAC_LINK_CHECK_ANS           = 0x02,
    /*!
     * LinkADRReq
     */
    SRV_MAC_LINK_ADR_REQ             = 0x03,
    /*!
     * DutyCycleReq
     */
    SRV_MAC_DUTY_CYCLE_REQ           = 0x04,
    /*!
     * RXParamSetupReq
     */
    SRV_MAC_RX_PARAM_SETUP_REQ       = 0x05,
    /*!
     * DevStatusReq
     */
    SRV_MAC_DEV_STATUS_REQ           = 0x06,
    /*!
     * NewChannelReq
     */
    SRV_MAC_NEW_CHANNEL_REQ          = 0x07,
    /*!
     * RXTimingSetupReq
     */
    SRV_MAC_RX_TIMING_SETUP_REQ      = 0x08,
    /*!
     * NewChannelReq
     */
    SRV_MAC_TX_PARAM_SETUP_REQ       = 0x09,
    /*!
     * DlChannelReq
     */
    SRV_MAC_DL_CHANNEL_REQ           = 0x0A,
} server_mac_cmds_t;

/*!
 * LoRaMAC battery level indicator.
 */
typedef enum {
    /*!
     * An external power source.
     */
    BAT_LEVEL_EXT_SRC                = 0x00,
    /*!
     * Battery level empty.
     */
    BAT_LEVEL_EMPTY                  = 0x01,
    /*!
     * Battery level full.
     */
    BAT_LEVEL_FULL                   = 0xFE,
    /*!
     * Battery level - no measurement available.
     */
    BAT_LEVEL_NO_MEASURE             = 0xFF,
} device_battery_level_t;

/*!
 * LoRaMAC header field definition (MHDR field).
 *
 * LoRaWAN Specification V1.0.2, chapter 4.2.
 */
typedef union {
    /*!
     * Byte-access to the bits.
     */
    uint8_t value;
    /*!
     * The structure containing single access to header bits.
     */
    struct hdr_bits_s
    {
        /*!
         * Major version.
         */
        uint8_t major           : 2;
        /*!
         * RFU
         */
        uint8_t RFU             : 3;
        /*!
         * Message type
         */
        uint8_t mtype           : 3;
    } bits;
} loramac_mhdr_t;

/*!
 * LoRaMAC frame control field definition (FCtrl).
 *
 * LoRaWAN Specification V1.0.2, chapter 4.3.1.
 */
typedef union {
    /*!
     * Byte-access to the bits.
     */
    uint8_t value;
    /*!
     * The structure containing single access to bits.
     */
    struct ctrl_bits_s
    {
        /*!
         * Frame options length.
         */
        uint8_t fopts_len        : 4;
        /*!
         * Frame pending bit.
         */
        uint8_t fpending        : 1;
        /*!
         * Message acknowledge bit.
         */
        uint8_t ack             : 1;
        /*!
         * ADR acknowledgment request bit.
         */
        uint8_t adr_ack_req       : 1;
        /*!
         * ADR control in the frame header.
         */
        uint8_t adr             : 1;
    } bits;
} loramac_frame_ctrl_t;

/*!
 * The enumeration containing the status of the operation of a MAC service.
 */
typedef enum {
    /*!
     * Service performed successfully.
     */
    LORAMAC_EVENT_INFO_STATUS_OK = 0,
    /*!
     * An error occurred during the execution of the service.
     */
    LORAMAC_EVENT_INFO_STATUS_ERROR,
    /*!
     * A TX timeout occurred.
     */
    LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT,
    /*!
     * An RX timeout occurred on receive window 1.
     */
    LORAMAC_EVENT_INFO_STATUS_RX1_TIMEOUT,
    /*!
     * An RX timeout occurred on receive window 2.
     */
    LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT,
    /*!
     * An RX error occurred on receive window 1.
     */
    LORAMAC_EVENT_INFO_STATUS_RX1_ERROR,
    /*!
     * An RX error occurred on receive window 2.
     */
    LORAMAC_EVENT_INFO_STATUS_RX2_ERROR,
    /*!
     * An error occurred in the join procedure.
     */
    LORAMAC_EVENT_INFO_STATUS_JOIN_FAIL,
    /*!
     * A frame with an invalid downlink counter was received. The
     * downlink counter of the frame was equal to the local copy
     * of the downlink counter of the node.
     */
    LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED,
    /*!
     * The MAC could not retransmit a frame since the MAC decreased the datarate. The
     * payload size is not applicable for the datarate.
     */
    LORAMAC_EVENT_INFO_STATUS_TX_DR_PAYLOAD_SIZE_ERROR,
    /*!
     * The node has lost MAX_FCNT_GAP or more frames.
     */
    LORAMAC_EVENT_INFO_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOSS,
    /*!
     * An address error occurred.
     */
    LORAMAC_EVENT_INFO_STATUS_ADDRESS_FAIL,
    /*!
     * Message integrity check failure.
     */
    LORAMAC_EVENT_INFO_STATUS_MIC_FAIL,
    /*!
     * Crypto methods failure
     */
    LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL,
} loramac_event_info_status_t;

/*!
 * LoRaMac service state flags.
 */
typedef union {
    /*!
     * Byte-access to the bits.
     */
    uint8_t value;
    /*!
     * The structure containing single access to bits.
     */
    struct mac_flag_bits_s
    {
        /*!
         * MCPS-Req pending
         */
        uint8_t mcps_req         : 1;
        /*!
         * MCPS-Ind pending
         */
        uint8_t mcps_ind         : 1;
        /*!
         * MCPS-Ind pending. Skip indication to the application layer.
         */
        uint8_t mcps_ind_skip     : 1;
        /*!
         * MLME-Req pending
         */
        uint8_t mlme_req         : 1;
        /*!
         * MLME-Ind pending
         */
        uint8_t mlme_ind         : 1;
        /*!
         * MAC cycle done
         */
        uint8_t mac_done         : 1;
    } bits;
} loramac_flags_t;

/*!
 *
 * \brief   LoRaMAC data services
 *
 * \details The following table list the primitives supported by a
 *          specific MAC data service:
 *
 * Name                  | Request | Indication | Response | Confirm
 * --------------------- | :-----: | :--------: | :------: | :-----:
 * \ref MCPS_UNCONFIRMED | YES     | YES        | NO       | YES
 * \ref MCPS_CONFIRMED   | YES     | YES        | NO       | YES
 * \ref MCPS_MULTICAST   | NO      | YES        | NO       | NO
 * \ref MCPS_PROPRIETARY | YES     | YES        | NO       | YES
 *
 * The following table provides links to the function implementations of the
 * related MCPS primitives:
 *
 * Primitive        | Function
 * ---------------- | :---------------------:
 * MCPS-Request     | LoRaMacMlmeRequest
 * MCPS-Confirm     | MacMcpsConfirm in \ref loramac_primitives_t
 * MCPS-Indication  | MacMcpsIndication in \ref loramac_primitives_t
 */
typedef enum {
    /*!
     * Unconfirmed LoRaMAC frame.
     */
    MCPS_UNCONFIRMED,
    /*!
     * Confirmed LoRaMAC frame.
     */
    MCPS_CONFIRMED,
    /*!
     * Multicast LoRaMAC frame.
     */
    MCPS_MULTICAST,
    /*!
     * Proprietary frame.
     */
    MCPS_PROPRIETARY,
} mcps_type_t;

/*!
 * LoRaMAC MCPS-Request for an unconfirmed frame.
 */
typedef struct {
    /*!
     * Frame port field. Must be set if the payload is not empty. Use the
     * application-specific frame port values: [1...223].
     *
     * LoRaWAN Specification V1.0.2, chapter 4.3.2.
     */
    uint8_t fport;

    /*!
     * Uplink datarate, if ADR is off.
     */
    int8_t data_rate;
} mcps_req_unconfirmed_t;

/*!
 * LoRaMAC MCPS-Request for a confirmed frame.
 */
typedef struct {
    /*!
     * Frame port field. Must be set if the payload is not empty. Use the
     * application-specific frame port values: [1...223].
     *
     * LoRaWAN Specification V1.0.2, chapter 4.3.2.
     */
    uint8_t fport;

    /*!
     * Uplink datarate, if ADR is off.
     */
    int8_t data_rate;
    /*!
     * The number of trials to transmit the frame, if the LoRaMAC layer did not
     * receive an acknowledgment. The MAC performs a datarate adaptation
     * according to the LoRaWAN Specification V1.0.2, chapter 18.4, as in
     * the following table:
     *
     * Transmission nb | Data Rate
     * ----------------|-----------
     * 1 (first)       | DR
     * 2               | DR
     * 3               | max(DR-1,0)
     * 4               | max(DR-1,0)
     * 5               | max(DR-2,0)
     * 6               | max(DR-2,0)
     * 7               | max(DR-3,0)
     * 8               | max(DR-3,0)
     *
     * Note that if nb_trials is set to 1 or 2, the MAC will not decrease
     * the datarate, if the LoRaMAC layer did not receive an acknowledgment.
     */
    uint8_t nb_trials;
} mcps_req_confirmed_t;

/*!
 * LoRaMAC MCPS-Request for a proprietary frame.
 */
typedef struct {
    /*!
     * Uplink datarate, if ADR is off.
     */
    int8_t data_rate;
} mcps_req_proprietary_t;

/*!
 * LoRaMAC MCPS-Request structure.
 */
typedef struct {
    /*!
     * MCPS-Request type.
     */
    mcps_type_t type;

    /*!
     * MCPS-Request parameters.
     */
    union
    {
        /*!
         * MCPS-Request parameters for an unconfirmed frame.
         */
        mcps_req_unconfirmed_t unconfirmed;
        /*!
         * MCPS-Request parameters for a confirmed frame.
         */
        mcps_req_confirmed_t confirmed;
        /*!
         * MCPS-Request parameters for a proprietary frame.
         */
        mcps_req_proprietary_t proprietary;
    } req;

    /** Payload data
      *
      * A pointer to the buffer of the frame payload.
      */
     void *f_buffer;
     /** Payload size
      *
      * The size of the frame payload.
      */
     uint16_t f_buffer_size;

} loramac_mcps_req_t;

/*!
 * LoRaMAC MCPS-Confirm.
 */
typedef struct {
    /*!
     * Holds the previously performed MCPS-Request type. i.e., the type of
     * the MCPS request for which this confirmation is being generated
     */
    mcps_type_t req_type;
    /*!
     * The status of the operation.
     */
    loramac_event_info_status_t status;
    /*!
     * The uplink datarate.
     */
    uint8_t data_rate;
    /*!
     * The transmission power.
     */
    int8_t tx_power;
    /*!
     * Set if an acknowledgement was received.
     */
    bool ack_received;
    /*!
     * Provides the number of retransmissions.
     */
    uint8_t nb_retries;
    /*!
     * The transmission time on air of the frame.
     */
    lorawan_time_t tx_toa;
    /*!
     * The uplink counter value related to the frame.
     */
    uint32_t ul_frame_counter;
    /*!
     * The uplink frequency related to the frame.
     */
    uint32_t ul_frequency;
} loramac_mcps_confirm_t;

/*!
 * LoRaMAC MCPS-Indication primitive.
 */
typedef struct {
    /*!
     * MCPS-Indication type.
     */
    mcps_type_t type;
    /*!
     * The status of the operation.
     */
    loramac_event_info_status_t status;
    /*!
     * Multicast.
     */
    uint8_t multicast;
    /*!
     * The application port.
     */
    uint8_t port;
    /*!
     * The downlink datarate.
     */
    uint8_t rx_datarate;
    /*!
     * Frame pending status.
     */
    uint8_t fpending_status;
    /*!
     * A pointer to the received data stream.
     */
    uint8_t *buffer;
    /*!
     * The size of the received data stream.
     */
    uint16_t buffer_size;
    /*!
     * Indicates, if data is available.
     */
    bool is_data_recvd;
    /*!
     * The RSSI of the received packet.
     */
    int16_t rssi;
    /*!
     * The SNR of the received packet.
     */
    uint8_t snr;
    /*!
     * The receive window.
     *
     * [0: Rx window 1, 1: Rx window 2]
     */
    rx_slot_t rx_slot;
    /*!
     * Set if an acknowledgement was received.
     */
    bool is_ack_recvd;
    /*!
     * The downlink counter value for the received frame.
     */
    uint32_t dl_frame_counter;
} loramac_mcps_indication_t;

/*!
 * \brief LoRaMAC management services.
 *
 * \details The following table list the primitives supported by a
 *          specific MAC management service:
 *
 * Name                         | Request | Indication | Response | Confirm
 * ---------------------------- | :-----: | :--------: | :------: | :-----:
 * \ref MLME_JOIN               | YES     | NO         | NO       | YES
 * \ref MLME_LINK_CHECK         | YES     | NO         | NO       | YES
 * \ref MLME_TXCW               | YES     | NO         | NO       | YES
 * \ref MLME_SCHEDULE_UPLINK    | NO      | YES        | NO       | NO
 *
 * The following table provides links to the function implementations of the
 * related MLME primitives.
 *
 * Primitive        | Function
 * ---------------- | :---------------------:
 * MLME-Request     | LoRaMacMlmeRequest
 * MLME-Confirm     | MacMlmeConfirm in \ref loramac_primitives_t
 * MLME-Indication  | MacMlmeIndication in \ref loramac_primitives_t
 */
typedef enum {
    /*!
     * Initiates the Over-the-Air activation.
     *
     * LoRaWAN Specification V1.0.2, chapter 6.2.
     */
    MLME_JOIN,
    /*!
     * LinkCheckReq - Connectivity validation.
     *
     * LoRaWAN Specification V1.0.2, chapter 5, table 4.
     */
    MLME_LINK_CHECK,
    /*!
     * Sets TX continuous wave mode.
     *
     * LoRaWAN end-device certification.
     */
    MLME_TXCW,
    /*!
     * Sets TX continuous wave mode (new LoRa-Alliance CC definition).
     *
     * LoRaWAN end-device certification.
     */
    MLME_TXCW_1,
    /*!
     * Indicates that the application shall perform an uplink as
     * soon as possible.
     */
    MLME_SCHEDULE_UPLINK
} mlme_type_t;

/*!
 * LoRaMAC MLME-Request for the join service.
 */
typedef struct {
    /*!
     * A globally unique end-device identifier.
     *
     * LoRaWAN Specification V1.0.2, chapter 6.2.1.
     */
    uint8_t *dev_eui;
    /*!
     * An application identifier.
     *
     * LoRaWAN Specification V1.0.2, chapter 6.1.2
     */
    uint8_t *app_eui;
    /*!
     * AES-128 application key.
     *
     * LoRaWAN Specification V1.0.2, chapter 6.2.2.
     */
    uint8_t *app_key;
    /*!
     * The number of trials for the join request.
     */
    uint8_t nb_trials;
} mlme_join_req_t;

/*!
 * LoRaMAC MLME-Request for TX continuous wave mode.
 */
typedef struct {
    /*!
     * The time while the radio is kept in continuous wave mode, in seconds.
     */
    uint16_t timeout;
    /*!
     * The RF frequency to set (only used with the new way).
     */
    uint32_t frequency;
    /*!
     * The RF output power to set (only used with the new way).
     */
    uint8_t power;
} mlme_cw_tx_mode_t;

/*!
 * LoRaMAC MLME-Request structure.
 */
typedef struct {
    /*!
     * MLME-Request type.
     */
    mlme_type_t type;

    /*!
     * MLME-Request parameters.
     */
    union {
        /*!
         * MLME-Request parameters for a join request.
         */
        mlme_join_req_t join;
        /*!
         * MLME-Request parameters for TX continuous mode request.
         */
        mlme_cw_tx_mode_t cw_tx_mode;
    } req;
} loramac_mlme_req_t;

/*!
 * LoRaMAC MLME-Confirm primitive.
 */
typedef struct {
    /*!
     * The previously performed MLME-Request. i.e., the request type
     * for which the confirmation is being generated
     */
    mlme_type_t req_type;
    /*!
     * The status of the operation.
     */
    loramac_event_info_status_t status;
    /*!
     * The transmission time on air of the frame.
     */
    lorawan_time_t tx_toa;
    /*!
     * The demodulation margin. Contains the link margin [dB] of the last LinkCheckReq
     * successfully received.
     */
    uint8_t demod_margin;
    /*!
     * The number of gateways which received the last LinkCheckReq.
     */
    uint8_t nb_gateways;
    /*!
     * The number of retransmissions.
     */
    uint8_t nb_retries;
} loramac_mlme_confirm_t;

/*!
 * LoRaMAC MLME-Indication primitive
 */
typedef struct {
    /*!
     * MLME-Indication type
     */
    mlme_type_t indication_type;
} loramac_mlme_indication_t;

/*!
 * LoRa MAC Information Base (MIB).
 *
 * The following table lists the MIB parameters and the related attributes:
 *
 * Attribute                         | Get | Set
 * --------------------------------- | :-: | :-:
 * \ref MIB_DEVICE_CLASS             | YES | YES
 * \ref MIB_NETWORK_JOINED           | YES | YES
 * \ref MIB_ADR                      | YES | YES
 * \ref MIB_NET_ID                   | YES | YES
 * \ref MIB_DEV_ADDR                 | YES | YES
 * \ref MIB_NWK_SKEY                 | YES | YES
 * \ref MIB_APP_SKEY                 | YES | YES
 * \ref MIB_PUBLIC_NETWORK           | YES | YES
 * \ref MIB_REPEATER_SUPPORT         | YES | YES
 * \ref MIB_CHANNELS                 | YES | NO
 * \ref MIB_RX2_CHANNEL              | YES | YES
 * \ref MIB_CHANNELS_MASK            | YES | YES
 * \ref MIB_CHANNELS_DEFAULT_MASK    | YES | YES
 * \ref MIB_CHANNELS_NB_REP          | YES | YES
 * \ref MIB_MAX_RX_WINDOW_DURATION   | YES | YES
 * \ref MIB_RECEIVE_DELAY_1          | YES | YES
 * \ref MIB_RECEIVE_DELAY_2          | YES | YES
 * \ref MIB_JOIN_ACCEPT_DELAY_1      | YES | YES
 * \ref MIB_JOIN_ACCEPT_DELAY_2      | YES | YES
 * \ref MIB_CHANNELS_DATARATE        | YES | YES
 * \ref MIB_CHANNELS_DEFAULT_DATARATE| YES | YES
 * \ref MIB_CHANNELS_TX_POWER        | YES | YES
 * \ref MIB_CHANNELS_DEFAULT_TX_POWER| YES | YES
 * \ref MIB_UPLINK_COUNTER           | YES | YES
 * \ref MIB_DOWNLINK_COUNTER         | YES | YES
 * \ref MIB_MULTICAST_CHANNEL        | YES | NO
 * \ref MIB_SYSTEM_MAX_RX_ERROR      | YES | YES
 * \ref MIB_MIN_RX_SYMBOLS           | YES | YES
 * \ref MIB_ANTENNA_GAIN             | YES | YES
 *
 * The following table provides links to the function implementations of the
 * related MIB primitives:
 *
 * Primitive        | Function
 * ---------------- | :---------------------:
 * MIB-Set          | LoRaMacMibSetRequestConfirm
 * MIB-Get          | LoRaMacMibGetRequestConfirm
 */
typedef enum {
    /*!
     * LoRaWAN device class.
     *
     * LoRaWAN Specification V1.0.2.
     */
    MIB_DEVICE_CLASS,
    /*!
     * LoRaWAN Network joined attribute.
     *
     * LoRaWAN Specification V1.0.2.
     */
    MIB_NETWORK_JOINED,
    /*!
     * Adaptive data rate.
     *
     * LoRaWAN Specification V1.0.2, chapter 4.3.1.1.
     *
     * [true: ADR enabled, false: ADR disabled].
     */
    MIB_ADR,
    /*!
     * Network identifier.
     *
     * LoRaWAN Specification V1.0.2, chapter 6.1.1.
     */
    MIB_NET_ID,
    /*!
     * End-device address.
     *
     * LoRaWAN Specification V1.0.2, chapter 6.1.1.
     */
    MIB_DEV_ADDR,
    /*!
     * Network session key.
     *
     * LoRaWAN Specification V1.0.2, chapter 6.1.3.
     */
    MIB_NWK_SKEY,
    /*!
     * Application session key.
     *
     * LoRaWAN Specification V1.0.2, chapter 6.1.4.
     */
    MIB_APP_SKEY,
    /*!
     * Set the network type to public or private.
     *
     * LoRaWAN Regional Parameters V1.0.2rB.
     *
     * [true: public network, false: private network]
     */
    MIB_PUBLIC_NETWORK,
    /*!
     * Support the operation with repeaters.
     *
     * LoRaWAN Regional Parameters V1.0.2rB.
     *
     * [true: repeater support enabled, false: repeater support disabled]
     */
    MIB_REPEATER_SUPPORT,
    /*!
     * Communication channels. A GET request will return a
     * pointer that references the first entry of the channel list. The
     * list is of size LORA_MAX_NB_CHANNELS.
     *
     * LoRaWAN Regional Parameters V1.0.2rB.
     */
    MIB_CHANNELS,
    /*!
     * Set receive window 2 channel.
     *
     * LoRaWAN Specification V1.0.2, chapter 3.3.1.
     */
    MIB_RX2_CHANNEL,
    /*!
     * Set receive window 2 channel.
     *
     * LoRaWAN Specification V1.0.2, chapter 3.3.2.
     */
    MIB_RX2_DEFAULT_CHANNEL,
    /*!
     * LoRaWAN channels mask.
     *
     * LoRaWAN Regional Parameters V1.0.2rB.
     */
    MIB_CHANNELS_MASK,
    /*!
     * LoRaWAN default channels mask.
     *
     * LoRaWAN Regional Parameters V1.0.2rB.
     */
    MIB_CHANNELS_DEFAULT_MASK,
    /*!
     * Set the number of repetitions on a channel.
     *
     * LoRaWAN Specification V1.0.2, chapter 5.2.
     */
    MIB_CHANNELS_NB_REP,
    /*!
     * The maximum receive window duration in [ms].
     *
     * LoRaWAN Specification V1.0.2, chapter 3.3.3.
     */
    MIB_MAX_RX_WINDOW_DURATION,
    /*!
     * The receive delay 1 in [ms].
     *
     * LoRaWAN Regional Parameters V1.0.2rB.
     */
    MIB_RECEIVE_DELAY_1,
    /*!
     * The receive delay 2 in [ms].
     *
     * LoRaWAN Regional Parameters V1.0.2rB.
     */
    MIB_RECEIVE_DELAY_2,
    /*!
     * The join accept delay 1 in [ms].
     *
     * LoRaWAN Regional Parameters V1.0.2rB.
     */
    MIB_JOIN_ACCEPT_DELAY_1,
    /*!
     * The join accept delay 2 in [ms].
     *
     * LoRaWAN Regional Parameters V1.0.2rB.
     */
    MIB_JOIN_ACCEPT_DELAY_2,
    /*!
     * The default data rate of a channel.
     *
     * LoRaWAN Regional Parameters V1.0.2rB.
     *
     * The allowed ranges are region-specific. Please refer to \ref DR_0 to \ref DR_15 for details.
     */
    MIB_CHANNELS_DEFAULT_DATARATE,
    /*!
     * The data rate of a channel.
     *
     * LoRaWAN Regional Parameters V1.0.2rB.
     *
     * The allowed ranges are region-specific. Please refer to \ref DR_0 to \ref DR_15 for details.
     */
    MIB_CHANNELS_DATARATE,
    /*!
     * The transmission power of a channel.
     *
     * LoRaWAN Regional Parameters V1.0.2rB.
     *
     * The allowed ranges are region-specific. Please refer to \ref TX_POWER_0 to \ref TX_POWER_15 for details.
     */
    MIB_CHANNELS_TX_POWER,
    /*!
     * The transmission power of a channel.
     *
     * LoRaWAN Regional Parameters V1.0.2rB.
     *
     * The allowed ranges are region-specific. Please refer to \ref TX_POWER_0 to \ref TX_POWER_15 for details.
     */
    MIB_CHANNELS_DEFAULT_TX_POWER,
    /*!
     * LoRaWAN uplink counter.
     *
     * LoRaWAN Specification V1.0.2, chapter 4.3.1.5.
     */
    MIB_UPLINK_COUNTER,
    /*!
     * LoRaWAN downlink counter.
     *
     * LoRaWAN Specification V1.0.2, chapter 4.3.1.5.
     */
    MIB_DOWNLINK_COUNTER,
    /*!
     * Multicast channels. A GET request will return a pointer to the first
     * entry of the multicast channel linked list. If the pointer is equal to
     * NULL, the list is empty.
     */
    MIB_MULTICAST_CHANNEL,
    /*!
     * System overall timing error in milliseconds.
     * [-SystemMaxRxError : +SystemMaxRxError]
     * Default: +/-10 ms
     */
    MIB_SYSTEM_MAX_RX_ERROR,
    /*!
     * The minimum  number of symbols required to detect an RX frame.
     * Default: 6 symbols
     */
    MIB_MIN_RX_SYMBOLS,
    /*!
     * The antenna gain of the node. The default value is region-specific.
     * The antenna gain is used to calculate the TX power of the node.
     * The formula is:
     * radioTxPower = ( int8_t )floor( maxEirp - antennaGain )
     */
    MIB_ANTENNA_GAIN
} mib_type_t;

/*!
 * LoRaMAC MIB parameters.
 */
typedef union {
    /*!
     * LoRaWAN device class.
     *
     * Related MIB type: \ref MIB_DEVICE_CLASS
     */
    device_class_t dev_class;
    /*!
     * LoRaWAN network joined attribute
     *
     * Related MIB type: \ref MIB_NETWORK_JOINED
     */
    bool is_nwk_joined;
    /*!
     * Activation state of ADR
     *
     * Related MIB type: \ref MIB_ADR
     */
    bool is_adr_enable;
    /*!
     * Network identifier
     *
     * Related MIB type: \ref MIB_NET_ID
     */
    uint32_t net_id;
    /*!
     * End-device address
     *
     * Related MIB type: \ref MIB_DEV_ADDR
     */
    uint32_t dev_addr;
    /*!
     * Network session key
     *
     * Related MIB type: \ref MIB_NWK_SKEY
     */
    uint8_t *nwk_skey;
    /*!
     * Application session key
     *
     * Related MIB type: \ref MIB_APP_SKEY
     */
    uint8_t *app_skey;
    /*!
     * Enable or disable a public network
     *
     * Related MIB type: \ref MIB_PUBLIC_NETWORK
     */
    bool enable_public_nwk;
    /*!
     * Enable or disable repeater support
     *
     * Related MIB type: \ref MIB_REPEATER_SUPPORT
     */
    bool enable_repeater_support;
    /*!
     * LoRaWAN channel
     *
     * Related MIB type: \ref MIB_CHANNELS
     */
    channel_params_t* channel_list;
     /*!
     * Channel for the receive window 2
     *
     * Related MIB type: \ref MIB_RX2_CHANNEL
     */
    rx2_channel_params rx2_channel;
     /*!
     * Channel for the receive window 2
     *
     * Related MIB type: \ref MIB_RX2_DEFAULT_CHANNEL
     */
    rx2_channel_params default_rx2_channel;
    /*!
     * Channel mask
     *
     * Related MIB type: \ref MIB_CHANNELS_MASK
     */
    uint16_t* channel_mask;
    /*!
     * Default channel mask
     *
     * Related MIB type: \ref MIB_CHANNELS_DEFAULT_MASK
     */
    uint16_t* default_channel_mask;
    /*!
     * Number of frame repetitions
     *
     * Related MIB type: \ref MIB_CHANNELS_NB_REP
     */
    uint8_t channel_nb_rep;
    /*!
     * Maximum receive window duration
     *
     * Related MIB type: \ref MIB_MAX_RX_WINDOW_DURATION
     */
    uint32_t max_rx_window;
    /*!
     * Receive delay 1
     *
     * Related MIB type: \ref MIB_RECEIVE_DELAY_1
     */
    uint32_t recv_delay1;
    /*!
     * Receive delay 2
     *
     * Related MIB type: \ref MIB_RECEIVE_DELAY_2
     */
    uint32_t recv_delay2;
    /*!
     * Join accept delay 1
     *
     * Related MIB type: \ref MIB_JOIN_ACCEPT_DELAY_1
     */
    uint32_t join_accept_delay1;
    /*!
     * Join accept delay 2
     *
     * Related MIB type: \ref MIB_JOIN_ACCEPT_DELAY_2
     */
    uint32_t join_accept_delay2;
    /*!
     * Channels data rate
     *
     * Related MIB type: \ref MIB_CHANNELS_DEFAULT_DATARATE
     */
    int8_t default_channel_data_rate;
    /*!
     * Channels data rate
     *
     * Related MIB type: \ref MIB_CHANNELS_DATARATE
     */
    int8_t channel_data_rate;
    /*!
     * Channels TX power
     *
     * Related MIB type: \ref MIB_CHANNELS_DEFAULT_TX_POWER
     */
    int8_t default_channel_tx_pwr;
    /*!
     * Channels TX power
     *
     * Related MIB type: \ref MIB_CHANNELS_TX_POWER
     */
    int8_t channel_tx_pwr;
    /*!
     * LoRaWAN uplink counter
     *
     * Related MIB type: \ref MIB_UPLINK_COUNTER
     */
    uint32_t ul_frame_counter;
    /*!
     * LoRaWAN downlink counter
     *
     * Related MIB type: \ref MIB_DOWNLINK_COUNTER
     */
    uint32_t dl_frame_counter;
    /*!
     * Multicast channel
     *
     * Related MIB type: \ref MIB_MULTICAST_CHANNEL
     */
    multicast_params_t* multicast_list;
    /*!
     * System overall timing error in milliseconds
     *
     * Related MIB type: \ref MIB_SYSTEM_MAX_RX_ERROR
     */
    uint32_t max_rx_sys_error;
    /*!
     * Minimum required number of symbols to detect an RX frame
     *
     * Related MIB type: \ref MIB_MIN_RX_SYMBOLS
     */
    uint8_t min_rx_symb;
    /*!
     * Antenna gain
     *
     * Related MIB type: \ref MIB_ANTENNA_GAIN
     */
    float antenna_gain;
} mib_params_t;

/*!
 * LoRaMAC MIB-RequestConfirm structure
 */
typedef struct {
    /*!
     * MIB-Request type
     */
    mib_type_t type;

    /*!
     * MLME-RequestConfirm parameters
     */
    mib_params_t param;
}loramac_mib_req_confirm_t;

/*!
 * LoRaMAC TX information
 */
typedef struct {
    /*!
     * Defines the size of the applicable payload that can be processed.
     */
    uint8_t max_possible_payload_size;
    /*!
     * The current payload size, dependent on the current datarate.
     */
    uint8_t current_payload_size;
} loramac_tx_info_t;

/** LoRaMAC status.
 *
 */
typedef enum lorawan_status {
    LORAWAN_STATUS_OK = 0,                         /**< Service started successfully */
    LORAWAN_STATUS_BUSY = -1000,                   /**< Service not started - LoRaMAC is busy */
    LORAWAN_STATUS_WOULD_BLOCK = -1001,            /**< LoRaMAC cannot send at the moment or have nothing to read */
    LORAWAN_STATUS_SERVICE_UNKNOWN = -1002,        /**< Service unknown */
    LORAWAN_STATUS_PARAMETER_INVALID = -1003,      /**< Service not started - invalid parameter */
    LORAWAN_STATUS_FREQUENCY_INVALID = -1004,      /**< Service not started - invalid frequency */
    LORAWAN_STATUS_DATARATE_INVALID = -1005,       /**< Service not started - invalid datarate */
    LORAWAN_STATUS_FREQ_AND_DR_INVALID = -1006,    /**< Service not started - invalid frequency and datarate */
    LORAWAN_STATUS_NO_NETWORK_JOINED = -1009,      /**< Service not started - the device is not in a LoRaWAN */
    LORAWAN_STATUS_LENGTH_ERROR = -1010,           /**< Service not started - payload lenght error */
    LORAWAN_STATUS_DEVICE_OFF = -1011,             /**< Service not started - the device is switched off */
    LORAWAN_STATUS_NOT_INITIALIZED = -1012,        /**< Service not started - stack not initialized */
    LORAWAN_STATUS_UNSUPPORTED = -1013,            /**< Service not supported */
    LORAWAN_STATUS_CRYPTO_FAIL = -1014,            /**< Service not started - crypto failure */
    LORAWAN_STATUS_PORT_INVALID = -1015,           /**< Invalid port */
    LORAWAN_STATUS_CONNECT_IN_PROGRESS = -1016,    /**< Services started - Connection in progress */
    LORAWAN_STATUS_NO_ACTIVE_SESSIONS = -1017,            /**< Services not started - No active session */
    LORAWAN_STATUS_IDLE = -1018,                   /**< Services started - Idle at the moment */
#if defined(LORAWAN_COMPLIANCE_TEST)
    LORAWAN_STATUS_COMPLIANCE_TEST_ON = -1019,     /**< Compliance test - is on-going */
#endif
} lorawan_status_t;

/*!
 * LoRaMAC events structure.
 * Used to notify upper layers of MAC events.
 */
typedef struct {
    /*!
     * \brief   MCPS-Confirm primitive.
     *
     * \param   [OUT] MCPS-Confirm parameters.
     */
    mbed::Callback<void(loramac_mcps_confirm_t*)> mcps_confirm;

    /*!
     * \brief   MCPS-Indication primitive.
     *
     * \param   [OUT] MCPS-Indication parameters.
     */
    mbed::Callback<void(loramac_mcps_indication_t*)> mcps_indication;

    /*!
     * \brief   MLME-Confirm primitive.
     *
     * \param   [OUT] MLME-Confirm parameters.
     */
    mbed::Callback<void(loramac_mlme_confirm_t*)> mlme_confirm;

    /*!
     * \brief   MLME-Indication primitive
     *
     * \param   [OUT] MLME-Indication parameters
     */
    mbed::Callback<void(loramac_mlme_indication_t*)> mlme_indication;
}loramac_primitives_t;

/** End-device states.
 *
 */
typedef enum device_states {
    DEVICE_STATE_NOT_INITIALIZED,
    DEVICE_STATE_INIT,
    DEVICE_STATE_JOINING,
    DEVICE_STATE_ABP_CONNECTING,
    DEVICE_STATE_JOINED,
    DEVICE_STATE_SEND,
    DEVICE_STATE_IDLE,
#if defined(LORAWAN_COMPLIANCE_TEST)
    DEVICE_STATE_COMPLIANCE_TEST,
#endif
    DEVICE_STATE_SHUTDOWN
} device_states_t;

/** Enum of LoRaWAN connection type.
 *
 * The LoRaWAN connection type specifies how an end-device connects to the gateway.
 */
typedef enum lorawan_connect_type {
    LORAWAN_CONNECTION_OTAA = 0,    /**< Over The Air Activation */
    LORAWAN_CONNECTION_ABP          /**< Activation By Personalization */
} lorawan_connect_type_t;

/** The lorawan_connect_otaa structure.
 *
 * A structure representing the LoRaWAN Over The Air Activation
 * parameters.
 */
typedef struct {
    /** End-device identifier
     *
     * LoRaWAN Specification V1.0.2, chapter 6.2.1
     */
    uint8_t *dev_eui;
    /** Application identifier
     *
     * LoRaWAN Specification V1.0.2, chapter 6.1.2
     */
    uint8_t *app_eui;
    /** AES-128 application key
     *
     * LoRaWAN Specification V1.0.2, chapter 6.2.2
     */
    uint8_t *app_key;
    /** Join request trials
     *
     * Number of trials for the join request.
     */
    uint8_t nb_trials;
} lorawan_connect_otaa_t;

/** The lorawan_connect_abp structure.
 *
 * A structure representing the LoRaWAN Activation By Personalization
 * parameters.
 */
typedef struct {
    /** Network identifier
     *
     * LoRaWAN Specification V1.0.2, chapter 6.1.1
     */
    uint32_t nwk_id;
    /** End-device address
     *
     * LoRaWAN Specification V1.0.2, chapter 6.1.1
     */
    uint32_t dev_addr;
    /** Network session key
     *
     * LoRaWAN Specification V1.0.2, chapter 6.1.3
     */
    uint8_t *nwk_skey;
    /** Application session key
     *
     * LoRaWAN Specification V1.0.2, chapter 6.1.4
     */
    uint8_t *app_skey;
} lorawan_connect_abp_t;

/**
 * Stack level TX message structure
 */
typedef struct {

    /**
     * TX Ongoing flag
     */
    bool tx_ongoing;

    /**
     * Application Port Number
     */
    uint8_t port;

    /**
     * Message type
     */
    mcps_type_t type;
    /** Message parameters.
     *
     */
    union message {
        /** An unconfirmed frame.
         *
         * The message parameters for an unconfirmed frame.
         */
        mcps_req_unconfirmed_t unconfirmed;
        /** A confirmed frame.
         *
         * The message parameters for a confirmed frame.
         */
        mcps_req_confirmed_t confirmed;
        /** A proprietary frame.
         *
         * The message parameters for a proprietary frame.
         */
        mcps_req_proprietary_t proprietary;
    } message_u;

    /** Payload data
     *
     * Base pointer to the buffer
     */
    uint8_t f_buffer[MBED_CONF_LORA_TX_MAX_SIZE];

    /** Payload size.
     *
     * The size of the frame payload.
     */
    uint16_t f_buffer_size;

    /**
     * Pending data size
     */
    uint16_t pending_size;

} loramac_tx_message_t;

/** lora_mac_rx_message_type_t
 *
 * An enum representing a structure for RX messages.
 */
typedef enum  {
    LORAMAC_RX_MLME_CONFIRM = 0,    /**< lora_mac_mlme_confirm_t */
    LORAMAC_RX_MCPS_CONFIRM,        /**< lora_mac_mcps_confirm_t */
    LORAMAC_RX_MCPS_INDICATION      /**< lora_mac_mcps_indication_t */
} rx_msg_type;

/** lora_mac_rx_message_by_type_t union
 *
 * A union representing a structure for RX messages.
 */
typedef union {
    loramac_mlme_confirm_t mlme_confirm;
    loramac_mcps_confirm_t mcps_confirm;
    loramac_mcps_indication_t mcps_indication;
} rx_message_u;

/** loramac_rx_message_t
 *
 * A structure representing a structure for an RX message.
 */
typedef struct {
    bool receive_ready;
    rx_msg_type type;
    rx_message_u msg;
    uint16_t pending_size;
    uint16_t prev_read_size;
} loramac_rx_message_t;

/**
 * Structure to hold A list of LoRa Channels
 */
typedef struct lora_channels_s {
    uint8_t id;
    channel_params_t ch_param;
} loramac_channel_t;


/** lorawan_connect_t structure
 *
 * A structure representing the parameters for different connections.
 */
typedef struct lorawan_connect {
    /*!
     * Select the connection type, either LORAWAN_CONNECTION_OTAA
     * or LORAWAN_CONNECTION_ABP.
     */
    uint8_t connect_type;

    union {
        /*!
         * Join the network using OTA
         */
        lorawan_connect_otaa_t otaa;
        /*!
         * Authentication by personalization
         */
        lorawan_connect_abp_t abp;
    } connection_u;

} lorawan_connect_t;

/** LoRaWAN session
 *
 * A structure for keeping session details.
 */
typedef struct lorawan_session {
    /**
     * True if the session is active
     */
    bool active;

    lorawan_connect_t connection;
    /**
     * LoRaWAN uplink counter
     *
     * Related MIB type: LORA_MIB_UPLINK_COUNTER
     */
    uint32_t uplink_counter;
    /**
     * LoRaWAN downlink counter
     *
     * Related MIB type: LORA_MIB_DOWNLINK_COUNTER
     */
    uint32_t downlink_counter;
} lorawan_session_t;

/** Commissioning data
 *
 * A structure for data in commission.
 */
typedef struct {
    /** Connection information
     *
     * Saves information for etc. keys
     */
    lorawan_connect_t connection;
    /**
     * LoRaWAN Up-link counter
     *
     * Related MIB type: LORA_MIB_UPLINK_COUNTER
     */
    uint32_t uplink_counter;
    /**
     * LoRaWAN Down-link counter
     *
     * Related MIB type: LORA_MIB_DOWNLINK_COUNTER
     */
    uint32_t downlink_counter;
} lorawan_dev_commission_t;

#if defined(LORAWAN_COMPLIANCE_TEST)
/**  LoRaWAN compliance tests support data
 *
 */
typedef struct compliance_test {
    /** Is test running
     *
     */
    bool running;
    /** State of test
     *
     */
    uint8_t state;
    /** Is TX confirmed
     *
     */
    bool is_tx_confirmed;
    /** Port used by the application
     *
     */
    uint8_t app_port;
    /** Maximum size of data used by application
     *
     */
    uint8_t app_data_size;
    /** Data provided by application
     *
     */
    uint8_t *app_data_buffer;
    /** Downlink counter
     *
     */
    uint16_t downlink_counter;
    /** Is link check required
     *
     */
    bool link_check;
    /** Demodulation margin
     *
     */
    uint8_t demod_margin;
    /** Number of gateways
     *
     */
    uint8_t nb_gateways;
} compliance_test_t;
#endif

/** Structure containing the uplink status
 *
 */
typedef struct {
    /** Is acked
     *
     */
    uint8_t acked;
    /** Uplink data rate
     *
     */
    int8_t datarate;
    /** Uplink counter
     *
     */
    uint16_t uplink_counter;
    /** Port is used by application
     *
     */
    uint8_t port;
    /** Payload
     *
     */
    uint8_t *buffer;
    /** Payload size
     *
     */
    uint8_t buffer_size;
} loramac_uplink_status_t;

/** A structure containing the downlink status
 *
 */
typedef struct {
    /** RSSI of downlink
     *
     */
    int16_t rssi;
    /** SNR of downlink
     *
     */
    int8_t snr;
    /** Downlink counter
     *
     */
    uint16_t downlink_counter;
    /** Is RX data received
     *
     */
    bool rx_data;
    /** Port used by application
     *
     */
    uint8_t port;
    /** Payload
     *
     */
    uint8_t *buffer;
    /** Payload size
     *
     */
    uint8_t buffer_size;
} loramac_downlink_status_t;

/*!
 * The parameter structure for the function for regional rx configuration.
 */
typedef struct {
    /*!
     * The RX channel.
     */
    uint8_t channel;
    /*!
     * The RX datarate.
     */
    uint8_t datarate;
    /*!
     * The RX bandwidth.
     */
    uint8_t bandwidth;
    /*!
     * The RX datarate offset.
     */
    int8_t dr_offset;
    /*!
     * The RX frequency.
     */
    uint32_t frequency;
    /*!
     * The RX window timeout
     */
     uint32_t window_timeout;
    /*!
     * The RX window offset
     */
    int32_t window_offset;
    /*!
     * The downlink dwell time.
     */
    uint8_t dl_dwell_time;
    /*!
     * Set to true, if a repeater is supported.
     */
    bool is_repeater_supported;
    /*!
     * Set to true, if RX should be continuous.
     */
    bool is_rx_continuous;
    /*!
     * Sets the RX window.
     */
    rx_slot_t rx_slot;
} rx_config_params_t;

/*!
 * \brief Timer object description
 */
typedef struct {
    mbed::Callback<void()> callback;
    int timer_id;
} timer_event_t;

/*!
 * LoRaMac internal states
 */
typedef enum {
    LORAMAC_IDLE          = 0x00000000,
    LORAMAC_TX_RUNNING    = 0x00000001,
    LORAMAC_RX            = 0x00000002,
    LORAMAC_ACK_REQ       = 0x00000004,
    LORAMAC_ACK_RETRY     = 0x00000008,
    LORAMAC_TX_DELAYED    = 0x00000010,
    LORAMAC_TX_CONFIG     = 0x00000020,
    LORAMAC_RX_ABORT      = 0x00000040,
} loramac_internal_state;

typedef struct {
    /*!
     * Device IEEE EUI
     */
    uint8_t *dev_eui;

    /*!
     * Application IEEE EUI
     */
    uint8_t *app_eui;

    /*!
     * AES encryption/decryption cipher application key
     */
    uint8_t *app_key;

    /*!
     * AES encryption/decryption cipher network session key
     */
    uint8_t nwk_skey[16];

    /*!
     * AES encryption/decryption cipher application session key
     */
    uint8_t app_skey[16];

} loramac_keys;

typedef struct {
      /*!
       * Aggregated duty cycle management
       */
      lorawan_time_t aggregated_last_tx_time;
      lorawan_time_t aggregated_timeoff;

      /*!
       * Stores the time at LoRaMac initialization.
       *
       * \remark Used for the BACKOFF_DC computation.
       */
      lorawan_time_t mac_init_time;


      /*!
       * Last transmission time on air
       */
      lorawan_time_t tx_toa;

      /*!
       * LoRaMac timer used to check the LoRaMacState (runs every second)
       */
      timer_event_t mac_state_check_timer;

      /*!
       * LoRaMac duty cycle delayed Tx timer
       */
      timer_event_t tx_delayed_timer;

      /*!
       * LoRaMac reception windows timers
       */
      timer_event_t rx_window1_timer;
      timer_event_t rx_window2_timer;

      /*!
       * Acknowledge timeout timer. Used for packet retransmissions.
       */
      timer_event_t ack_timeout_timer;

} lorawan_timers;

typedef struct {

    /*!
     * Actual device class
     */
    device_class_t dev_class;

    /*!
     * Holds the type of current Receive window slot
     */
     rx_slot_t rx_slot;

    /*!
     * Indicates if the node is connected to a private or public network
     */
    bool is_nwk_public;

    /*!
     * Indicates if the node supports repeaters
     */
    bool is_repeater_supported;

    /*!
     * IsPacketCounterFixed enables the MIC field tests by fixing the
     * ul_frame_counter value
     */
    bool is_ul_frame_counter_fixed;

    /*!
     * Used for test purposes. Disables the opening of the reception windows.
     */
    bool is_rx_window_enabled;

    /*!
     * Indicates if the MAC layer has already joined a network.
     */
    bool is_nwk_joined;

    /*!
     * If the node has sent a FRAME_TYPE_DATA_CONFIRMED_UP this variable indicates
     * if the nodes needs to manage the server acknowledgement.
     */
    bool is_node_ack_requested;

    /*!
     * If the server has sent a FRAME_TYPE_DATA_CONFIRMED_DOWN this variable indicates
     * if the ACK bit must be set for the next transmission
     */
    bool is_srv_ack_requested;

    /*!
     * Enables/Disables duty cycle management (Test only)
     */
    bool is_dutycycle_on;

    /*!
     * Set to true, if the last uplink was a join request
     */
    bool is_last_tx_join_request;

    /*!
     * Indicates if the AckTimeout timer has expired or not
     */
    bool is_ack_retry_timeout_expired;

    /*!
     * Current channel index
     */
    uint8_t channel;

    /*!
     * Current channel index
     */
    uint8_t last_channel_idx;

    /*!
     * Uplink messages repetitions counter
     */
    uint8_t ul_nb_rep_counter;

    /*!
     * Buffer containing the data to be sent or received.
     */
    uint8_t buffer[LORAMAC_PHY_MAXPAYLOAD];

    /*!
     * Length of packet in LoRaMacBuffer
     */
    uint16_t buffer_pkt_len;

    /*!
     * Buffer containing the upper layer data.
     */
    uint8_t payload[LORAMAC_PHY_MAXPAYLOAD];

    /*!
     * Length of the payload in LoRaMacBuffer
     */
    uint8_t payload_length;

    /*!
     * Number of trials to get a frame acknowledged
     */
    uint8_t max_ack_timeout_retries;

    /*!
     * Number of trials to get a frame acknowledged
     */
    uint8_t ack_timeout_retry_counter;

    /*!
     * Maximum number of trials for the Join Request
     */
    uint8_t max_join_request_trials;

    /*!
     * Number of trials for the Join Request
     */
    uint8_t join_request_trial_counter;

    /*!
     * Mac keys
     */
    loramac_keys keys;

    /*!
     * LoRaMac tx/rx operation state
     */
    loramac_flags_t flags;

    /*!
     * Device nonce is a random value extracted by issuing a sequence of RSSI
     * measurements
     */
    uint16_t dev_nonce;

    /*!
     * Network ID ( 3 bytes )
     */
    uint32_t net_id;

    /*!
     * Mote Address
     */
    uint32_t dev_addr;

    /*!
     * LoRaMAC frame counter. Each time a packet is sent the counter is incremented.
     * Only the 16 LSB bits are sent
     */
    uint32_t ul_frame_counter;

    /*!
     * LoRaMAC frame counter. Each time a packet is received the counter is incremented.
     * Only the 16 LSB bits are received
     */
    uint32_t dl_frame_counter;

    /*!
     * Counts the number of missed ADR acknowledgements
     */
    uint32_t adr_ack_counter;

    /*!
     * LoRaMac internal state
     */
    uint32_t mac_state;

    /*!
     * LoRaMac reception windows delay
     * \remark normal frame: RxWindowXDelay = ReceiveDelayX - RADIO_WAKEUP_TIME
     *         join frame  : RxWindowXDelay = JoinAcceptDelayX - RADIO_WAKEUP_TIME
     */
    uint32_t rx_window1_delay;
    uint32_t rx_window2_delay;

    /*!
     * Timer objects and stored values
     */
    lorawan_timers timers;

    /*!
     * LoRaMac parameters
     */
    lora_mac_system_params_t sys_params;

    /*!
     * Receive Window configurations for PHY layer
     */
    rx_config_params_t rx_window1_config;
    rx_config_params_t rx_window2_config;

    /*!
     * Multicast channels linked list
     */
    multicast_params_t *multicast_channels;

} loramac_protocol_params;

/** LoRaWAN callback functions
 *
 */
typedef enum lora_events {
    CONNECTED=0,
    DISCONNECTED,
    TX_DONE,
    TX_TIMEOUT,
    TX_ERROR,
    TX_CRYPTO_ERROR,
    TX_SCHEDULING_ERROR,
    RX_DONE,
    RX_TIMEOUT,
    RX_ERROR,
    JOIN_FAILURE,
} lorawan_event_t;

typedef struct  {
     // Mandatory. Event Callback must be provided
     mbed::Callback<void(lorawan_event_t)> events;

     // Rest are optional
     // If the user do not assign these callbacks, these callbacks would return
     // null if checked with bool operator
     // link_check_resp callback and other such callbacks will be maped in
     // future releases of Mbed-OS
     mbed::Callback<void(uint8_t, uint8_t)> link_check_resp;

     // Battery level callback goes in the down direction, i.e., it informs
     // the stack about the battery level by calling a function provided
     // by the upper layers
     mbed::Callback<uint8_t(void)> battery_level;
 } lorawan_app_callbacks_t;

typedef struct lora_channelplan {
    uint8_t nb_channels;    // number of channels
    loramac_channel_t *channels;
} lorawan_channelplan_t;

#endif /* LORAWAN_SYSTEM_LORAWAN_DATA_STRUCTURES_H_ */
