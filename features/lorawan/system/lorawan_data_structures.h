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
#include "lorawan_types.h"

/*!
 * \brief Timer time variable definition
 */
#ifndef lorawan_time_t
typedef uint32_t lorawan_time_t;
#endif

// Radio wake-up time from sleep - unit ms.
#define RADIO_WAKEUP_TIME                           1.0

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

#define LORAWAN_DEFAULT_QOS                         1

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
     * The number of uplink messages repetitions for QOS set by network server
     * in LinkADRReq mac command (unconfirmed messages only).
     */
    uint8_t nb_trans;
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
    struct hdr_bits_s {
        /*!
         * Major version.
         */
        uint8_t major : 2;
        /*!
         * RFU
         */
        uint8_t RFU : 3;
        /*!
         * Message type
         */
        uint8_t mtype : 3;
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
    struct ctrl_bits_s {
        /*!
         * Frame options length.
         */
        uint8_t fopts_len : 4;
        /*!
         * Frame pending bit.
         */
        uint8_t fpending : 1;
        /*!
         * Message acknowledge bit.
         */
        uint8_t ack : 1;
        /*!
         * ADR acknowledgment request bit.
         */
        uint8_t adr_ack_req : 1;
        /*!
         * ADR control in the frame header.
         */
        uint8_t adr : 1;
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
    LORAMAC_EVENT_INFO_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOST,
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
     * The uplink channel related to the frame.
     */
    uint32_t channel;
} loramac_mcps_confirm_t;

/*!
 * LoRaMAC MCPS-Indication primitive.
 */
typedef struct {
    /*!
     * True if an MCPS indication was pending
     */
    bool pending;
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
    const uint8_t *buffer;
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
    int8_t snr;
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
 * LoRaMAC MLME-Confirm primitive.
 */
typedef struct {
    /*!
     * Indicates if a request is pending or not
     */
    bool pending;
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
    bool pending;
} loramac_mlme_indication_t;

/**
 * End-device states.
 */
typedef enum device_states {
    DEVICE_STATE_NOT_INITIALIZED,
    DEVICE_STATE_JOINING,
    DEVICE_STATE_IDLE,
    DEVICE_STATE_CONNECTING,
    DEVICE_STATE_AWAITING_JOIN_ACCEPT,
    DEVICE_STATE_RECEIVING,
    DEVICE_STATE_CONNECTED,
    DEVICE_STATE_SCHEDULING,
    DEVICE_STATE_SENDING,
    DEVICE_STATE_AWAITING_ACK,
    DEVICE_STATE_STATUS_CHECK,
    DEVICE_STATE_SHUTDOWN
} device_states_t;

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
     *
     * For CONFIRMED Messages:
     *
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
     *
     * For UNCONFIRMED Messages:
     *
     * Provides a certain QOS level set by network server in LinkADRReq MAC
     * command. The device will transmit the given UNCONFIRMED message nb_trials
     * time with same frame counter until a downlink is received. Standard defined
     * range is 1:15. Data rates will NOT be adapted according to chapter 18.4.
     */
    uint8_t nb_trials;

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

/** LoRaWAN session
 *
 * A structure for keeping session details.
 */
typedef struct lorawan_session {
    /**
     * True if the session is active
     */
    bool active;

    /*!
     * Select the connection type, either LORAWAN_CONNECTION_OTAA
     * or LORAWAN_CONNECTION_ABP.
     */
    uint8_t connect_type;

    /**
     * LoRaWAN uplink counter
     */
    uint32_t uplink_counter;
    /**
     * LoRaWAN downlink counter
     */
    uint32_t downlink_counter;
} lorawan_session_t;

/*!
 * The parameter structure for the function for regional rx configuration.
 */
typedef struct {
    /*!
     * The RX channel.
     */
    uint8_t channel;
    /*!
     * The RX datarate index.
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
     * NOTE! LoRaMac determines the length of the key based on sizeof this variable
     */
    uint8_t nwk_skey[16];

    /*!
     * AES encryption/decryption cipher application session key
     * NOTE! LoRaMac determines the length of the key based on sizeof this variable
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
     * LoRaMac duty cycle backoff timer
     */
    timer_event_t backoff_timer;

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
     * TX buffer used for encrypted outgoing frames
     */
    uint8_t tx_buffer[LORAMAC_PHY_MAXPAYLOAD];

    /*!
     * Length of TX buffer
     */
    uint16_t tx_buffer_len;

    /*!
     * Used for storing decrypted RX data.
     */
    uint8_t rx_buffer[LORAMAC_PHY_MAXPAYLOAD];

    /*!
     * Length of the RX buffer
     */
    uint8_t rx_buffer_len;

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

#endif /* LORAWAN_SYSTEM_LORAWAN_DATA_STRUCTURES_H_ */
