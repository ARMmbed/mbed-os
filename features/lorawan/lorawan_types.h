/**
 * @file lorawan_types.h
 *
 * @brief Contains data structures required by LoRaWANBase class.
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
 *
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *
 * Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef MBED_LORAWAN_TYPES_H_
#define MBED_LORAWAN_TYPES_H_

#include "platform/Callback.h"

/**
 * A mask for the network ID.
 */
#define LORAWAN_NETWORK_ID_MASK                     (uint32_t) 0xFE000000

/**
 * Option Flags for send(), receive() APIs
 *
 * Special Notes for UPLINK:
 *  i)  All of the flags are mutually exclusive.
 *  ii) MSG_MULTICAST_FLAG cannot be used.
 */
#define MSG_UNCONFIRMED_FLAG                  0x01
#define MSG_CONFIRMED_FLAG                    0x02
#define MSG_MULTICAST_FLAG                    0x04
#define MSG_PROPRIETARY_FLAG                  0x08

#define LORAWAN_VERSION_1_0_2                 0
#define LORAWAN_VERSION_1_0_3                 1
#define LORAWAN_VERSION_1_1                   10

/**
 * LoRaWAN device classes definition.
 *
 * LoRaWAN Specification V1.0.2, chapter 2.1.
 */
typedef enum {
    /**
     * LoRaWAN device class A.
     *
     * LoRaWAN Specification V1.0.2, chapter 3.
     */
    CLASS_A = 0x00,
    /**
     * LoRaWAN device class B.
     *
     * LoRaWAN Specification V1.0.2, chapter 8.
     */
    CLASS_B = 0x01,
    /**
     * LoRaWAN device class C.
     *
     * LoRaWAN Specification V1.0.2, chapter 17.
     */
    CLASS_C = 0x02,
} device_class_t;

/**
 * lorawan_status_t contains status codes in
 * response to stack operations
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
    LORAWAN_STATUS_NO_ACTIVE_SESSIONS = -1017,     /**< Services not started - No active session */
    LORAWAN_STATUS_IDLE = -1018,                   /**< Services started - Idle at the moment */
    LORAWAN_STATUS_NO_OP = -1019,                  /**< Cannot perform requested operation */
    LORAWAN_STATUS_DUTYCYCLE_RESTRICTED = -1020,   /**< Transmission will continue after duty cycle backoff*/
    LORAWAN_STATUS_NO_CHANNEL_FOUND = -1021,       /**< None of the channels is enabled at the moment*/
    LORAWAN_STATUS_NO_FREE_CHANNEL_FOUND = -1022,  /**< None of the enabled channels is ready for another TX (duty cycle limited)*/
    LORAWAN_STATUS_METADATA_NOT_AVAILABLE = -1023, /**< Meta-data after an RX or TX is stale*/
    LORAWAN_STATUS_ALREADY_CONNECTED = -1024       /**< The device has already joined a network*/
} lorawan_status_t;

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
     * In case of LW1.1 or greater this is same as JoinEUI
     */
    uint8_t *app_eui;
    /** AES-128 application key
     *
     * LoRaWAN Specification V1.0.2, chapter 6.2.2
     */
    uint8_t *app_key;
    /** AES-128 network key
     *
     * In case of LoRaWAN Specification V1.0.2, must be same as app_key!
     * LoRaWAN specification 1.1, chapter 6.2.2
     */
    uint8_t *nwk_key;
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
     * LoRaWAN Spec V1.1 onwards this is used as FNwkSIntKey
     */
    uint8_t *nwk_skey;
    /** Application session key
     *
     * LoRaWAN Specification V1.0.2, chapter 6.1.4
     */
    uint8_t *app_skey;

    /** Serving Network session integrity key
     *
     * LoRaWAN Specification V1.1, chapter 6.1.2.3
     */
    uint8_t *snwk_sintkey;

    /** Network session encryption key
     *
     * LoRaWAN Specification V1.1, chapter 6.1.2.4
     */
    uint8_t *nwk_senckey;
} lorawan_connect_abp_t;

/** lorawan_connect_t structure
 *
 * A structure representing the parameters for different connections.
 */
typedef struct lorawan_connect {
    /**
     * Select the connection type, either LORAWAN_CONNECTION_OTAA
     * or LORAWAN_CONNECTION_ABP.
     */
    uint8_t connect_type;

    union {
        /**
         * Join the network using OTA
         */
        lorawan_connect_otaa_t otaa;
        /**
         * Authentication by personalization
         */
        lorawan_connect_abp_t abp;
    } connection_u;

} lorawan_connect_t;

/**
 * Events needed to announce stack operation results.
 *
 * CONNECTED            - When the connection is complete
 * DISCONNECTED         - When the protocol is shut down in response to disconnect()
 * TX_DONE              - When a packet is sent
 * TX_TIMEOUT,          - When stack was unable to send packet in TX window
 * TX_ERROR,            - A general TX error
 * CRYPTO_ERROR,        - A crypto error indicating wrong keys
 * TX_SCHEDULING_ERROR, - When stack is unable to schedule packet
 * RX_DONE,             - When there is something to receive
 * RX_TIMEOUT,          - Not yet mapped
 * RX_ERROR             - A general RX error
 * JOIN_FAILURE         - When all Joining retries are exhausted
 * UPLINK_REQUIRED      - Stack indicates application that some uplink needed
 * AUTOMATIC_UPLINK_ERROR - Stack tried automatically send uplink but some error occurred.
 *                          Application should initiate uplink as soon as possible.
 *
 */
typedef enum lora_events {
    CONNECTED = 0,
    DISCONNECTED,
    TX_DONE,
    TX_TIMEOUT,
    TX_ERROR,
    CRYPTO_ERROR,
    TX_CRYPTO_ERROR = CRYPTO_ERROR, //keeping this for backward compatibility
    TX_SCHEDULING_ERROR,
    RX_DONE,
    RX_TIMEOUT,
    RX_ERROR,
    JOIN_FAILURE,
    UPLINK_REQUIRED,
    AUTOMATIC_UPLINK_ERROR,
    CLASS_CHANGED, //only in Lorawan 1.1 (ch 18.1)
    SERVER_ACCEPTED_CLASS_IN_USE, //only in Lorawan 1.1 (ch 18.1)
    SERVER_DOES_NOT_SUPPORT_CLASS_IN_USE //only in Lorawan 1.1 (ch 18.1)
} lorawan_event_t;

/**
 * Stack level callback functions
 *
 * 'lorawan_app_callbacks_t' is a structure that holds pointers to the application
 * provided methods which are needed to be called in response to certain
 * requests. The structure is default constructed to set all pointers to NULL.
 * So if the user does not provide the pointer, a response will not be posted.
 * However, the 'lorawan_events' callback is mandatory to be provided as it
 * contains essential events.
 *
 * A link check request could be sent whenever the device tries to send a
 * message and if the network server responds with a link check response,
 * the stack notifies the application be calling the appropriate method set using
 * 'link_check_resp' callback. The result is thus transported to the application
 * via callback function provided.
 *
 * 'battery_level' callback goes in the down direction, i.e., it informs
 * the stack about the battery level by calling a function provided
 * by the upper layers.
 */
typedef struct {
    /**
     * Mandatory. Event Callback must be provided
     */
    mbed::Callback<void(lorawan_event_t)> events;

    /**
     * This callback is optional
     *
     * The first parameter to the callback function is the demodulation margin, and the second
     * parameter is the number of gateways that successfully received the last request.
     */
    mbed::Callback<void(uint8_t, uint8_t)> link_check_resp;

    /**
     * This callback is optional. If the callback is not set, the stack returns 255 to gateway.
     *
     * Battery level return value must follow the specification
     * for DevStatusAns MAC command:
     *
     *     0       The end-device is connected to an external power source
     *     1 - 254 The battery level, 1 being at minimum and 254 being at maximum
     *     255     The end-device was not able to measure the battery level.
     */
    mbed::Callback<uint8_t(void)> battery_level;
} lorawan_app_callbacks_t;

/**
 * DO NOT MODIFY, WILL BREAK THE API!
 *
 * Structure containing a given data rate range.
 */
typedef union {
    /**
     * Byte-access to the bits.
     */
    uint8_t value;
    /**
     * The structure to store the minimum and the maximum datarate.
     */
    struct fields_s {
        /**
         * The minimum data rate.
         *
         * LoRaWAN Regional Parameters V1.0.2rB.
         *
         * The allowed ranges are region-specific.
         * Please refer to \ref DR_0 to \ref DR_15 for details.
         */
        uint8_t min : 4;
        /**
         * The maximum data rate.
         *
         * LoRaWAN Regional Parameters V1.0.2rB.
         *
         * The allowed ranges are region-specific.
         * Please refer to \ref DR_0 to \ref DR_15 for details.
         */
        uint8_t max : 4;
    } fields;
} dr_range_t;

/**
 * DO NOT MODIFY, WILL BREAK THE API!
 *
 * LoRaMAC channel definition.
 */
typedef struct {
    /**
     * The frequency in Hz.
     */
    uint32_t frequency;
    /**
     * The alternative frequency for RX window 1.
     */
    uint32_t rx1_frequency;
    /**
     * The data rate definition.
     */
    dr_range_t dr_range;
    /**
     * The band index.
     */
    uint8_t band;
} channel_params_t;

/**
 * DO NOT MODIFY, WILL BREAK THE API!
 *
 * Structure to hold parameters for a LoRa channel.
 */
typedef struct lora_channels_s {
    uint8_t id;
    channel_params_t ch_param;
} loramac_channel_t;

/**
 * DO NOT MODIFY, WILL BREAK THE API!
 *
 * This data structures contains LoRaWAN channel plan, i.e., a pointer to a
 * list of channels and the total number of channels included in the plan.
 */
typedef struct lora_channelplan {
    uint8_t nb_channels;    // number of channels
    loramac_channel_t *channels;
} lorawan_channelplan_t;

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

/**
 * Enumeration for LoRaWAN connection type.
 */
typedef enum lorawan_connect_type {
    LORAWAN_CONNECTION_OTAA = 0,    /**< Over The Air Activation */
    LORAWAN_CONNECTION_ABP          /**< Activation By Personalization */
} lorawan_connect_type_t;


/**
 * Meta-data collection for a transmission
 */
typedef struct {
    /**
     * The transmission time on air of the frame.
     */
    uint32_t tx_toa;
    /**
     * The uplink channel used for transmission.
     */
    uint32_t channel;
    /**
     * The transmission power.
     */
    int8_t tx_power;
    /**
     * The uplink datarate.
     */
    uint8_t data_rate;
    /**
     * Provides the number of retransmissions.
     */
    uint8_t nb_retries;
    /**
     * A boolean to mark if the meta data is stale
     */
    bool stale;
} lorawan_tx_metadata;

/**
 * Meta-data collection for the received packet
 */
typedef struct {
    /**
     * The RSSI for the received packet.
     */
    int16_t rssi;
    /**
     * Data rate of reception
     */
    uint8_t rx_datarate;
    /**
     * The SNR for the received packet.
     */
    int8_t snr;
    /**
     * A boolean to mark if the meta data is stale
     */
    bool stale;
} lorawan_rx_metadata;

#endif /* MBED_LORAWAN_TYPES_H_ */
