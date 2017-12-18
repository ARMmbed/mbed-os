/**
 * @file lorawan_data_structures.h
 *
 * @brief Contains common data structures used by Mbed-OS
 *        LoRaWAN mplementation.
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifndef LORAWAN_SYSTEM_LORAWAN_DATA_STRUCTURES_H_
#define LORAWAN_SYSTEM_LORAWAN_DATA_STRUCTURES_H_

#include <inttypes.h>
#include "platform/Callback.h"

/*!
 * \brief Timer time variable definition
 */
#ifndef TimerTime_t
typedef uint32_t TimerTime_t;
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

/*!
 * LoRaWAN device classes definition.
 *
 * LoRaWAN Specification V1.0.2, chapter 2.1.
 */
typedef enum eDeviceClass
{
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
}DeviceClass_t;

/*!
 * LoRaMAC channel parameters definition.
 */
typedef union uDrRange
{
    /*!
     * Byte-access to the bits.
     */
    int8_t Value;
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
        int8_t Min : 4;
        /*!
         * The maximum data rate.
         *
         * LoRaWAN Regional Parameters V1.0.2rB.
         *
         * The allowed ranges are region-specific. Please refer to \ref DR_0 to \ref DR_15 for details.
         */
        int8_t Max : 4;
    }Fields;
}DrRange_t;

/*!
 * LoRaMAC channel definition.
 */
typedef struct sChannelParams
{
    /*!
     * The frequency in Hz.
     */
    uint32_t Frequency;
    /*!
     * The alternative frequency for RX window 1.
     */
    uint32_t Rx1Frequency;
    /*!
     * The data rate definition.
     */
    DrRange_t DrRange;
    /*!
     * The band index.
     */
    uint8_t Band;
}ChannelParams_t;

/*!
 * LoRaMAC band parameters definition.
 */
typedef struct sBand
{
    /*!
     * The duty cycle.
     */
    uint16_t DCycle;
    /*!
     * The maximum TX power.
     */
    int8_t TxMaxPower;
    /*!
     * The timestamp of the last JoinReq TX frame.
     */
    TimerTime_t LastJoinTxDoneTime;
    /*!
     * The timestamp of the last TX frame.
     */
    TimerTime_t LastTxDoneTime;
    /*!
     * The device off time.
     */
    TimerTime_t TimeOff;
}Band_t;


/*!
 * LoRaMAC receive window 2 channel parameters.
 */
typedef struct sRx2ChannelParams
{
    /*!
     * The frequency in Hz.
     */
    uint32_t Frequency;
    /*!
     * The data rate.
     *
     * LoRaWAN Regional Parameters V1.0.2rB.
     *
     * The allowed ranges are region-specific. Please refer to \ref DR_0 to \ref DR_15 for details.
     */
    uint8_t  Datarate;
}Rx2ChannelParams_t;

/*!
 * The global MAC layer parameters.
 */
typedef struct sLoRaMacParams
{
    /*!
     * The TX power in channels.
     */
    int8_t ChannelsTxPower;
    /*!
     * The data rate in channels.
     */
    int8_t ChannelsDatarate;
    /*!
     * The system overall timing error in milliseconds.
     * [-SystemMaxRxError : +SystemMaxRxError]
     * Default: +/-10 ms
     */
    uint32_t SystemMaxRxError;
    /*!
     * The minimum number of symbols required to detect an RX frame.
     * Default: 6 symbols
     */
    uint8_t MinRxSymbols;
    /*!
     * LoRaMac maximum time a reception window stays open.
     */
    uint32_t MaxRxWindow;
    /*!
     * Receive delay 1.
     */
    uint32_t ReceiveDelay1;
    /*!
     * Receive delay 2.
     */
    uint32_t ReceiveDelay2;
    /*!
     * Join accept delay 1.
     */
    uint32_t JoinAcceptDelay1;
    /*!
     * Join accept delay 1.
     */
    uint32_t JoinAcceptDelay2;
    /*!
     * The number of uplink messages repetitions [1:15] (unconfirmed messages only).
     */
    uint8_t ChannelsNbRep;
    /*!
     * The datarate offset between uplink and downlink on first window.
     */
    uint8_t Rx1DrOffset;
    /*!
     * LoRaMAC 2nd reception window settings.
     */
    Rx2ChannelParams_t Rx2Channel;
    /*!
     * The uplink dwell time configuration. 0: No limit, 1: 400ms
     */
    uint8_t UplinkDwellTime;
    /*!
     * The downlink dwell time configuration. 0: No limit, 1: 400ms
     */
    uint8_t DownlinkDwellTime;
    /*!
     * The maximum possible EIRP.
     */
    float MaxEirp;
    /*!
     * The antenna gain of the node.
     */
    float AntennaGain;
}LoRaMacParams_t;

/*!
 * LoRaMAC multicast channel parameter.
 */
typedef struct sMulticastParams
{
    /*!
     * Address.
     */
    uint32_t Address;
    /*!
     * Network session key.
     */
    uint8_t NwkSKey[16];
    /*!
     * Application session key.
     */
    uint8_t AppSKey[16];
    /*!
     * Downlink counter.
     */
    uint32_t DownLinkCounter;
    /*!
     * A reference pointer to the next multicast channel parameters in the list.
     */
    struct sMulticastParams *Next;
}MulticastParams_t;

/*!
 * LoRaMAC frame types.
 *
 * LoRaWAN Specification V1.0.2, chapter 4.2.1, table 1.
 */
typedef enum eLoRaMacFrameType
{
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
}LoRaMacFrameType_t;

/*!
 * LoRaMAC mote MAC commands.
 *
 * LoRaWAN Specification V1.0.2, chapter 5, table 4.
 */
typedef enum eLoRaMacMoteCmd
{
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
}LoRaMacMoteCmd_t;

/*!
 * LoRaMAC server MAC commands.
 *
 * LoRaWAN Specification V1.0.2 chapter 5, table 4.
 */
typedef enum eLoRaMacSrvCmd
{
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
}LoRaMacSrvCmd_t;

/*!
 * LoRaMAC battery level indicator.
 */
typedef enum eLoRaMacBatteryLevel
{
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
}LoRaMacBatteryLevel_t;

/*!
 * LoRaMAC header field definition (MHDR field).
 *
 * LoRaWAN Specification V1.0.2, chapter 4.2.
 */
typedef union uLoRaMacHeader
{
    /*!
     * Byte-access to the bits.
     */
    uint8_t Value;
    /*!
     * The structure containing single access to header bits.
     */
    struct sHdrBits
    {
        /*!
         * Major version.
         */
        uint8_t Major           : 2;
        /*!
         * RFU
         */
        uint8_t RFU             : 3;
        /*!
         * Message type
         */
        uint8_t MType           : 3;
    }Bits;
}LoRaMacHeader_t;

/*!
 * LoRaMAC frame control field definition (FCtrl).
 *
 * LoRaWAN Specification V1.0.2, chapter 4.3.1.
 */
typedef union uLoRaMacFrameCtrl
{
    /*!
     * Byte-access to the bits.
     */
    uint8_t Value;
    /*!
     * The structure containing single access to bits.
     */
    struct sCtrlBits
    {
        /*!
         * Frame options length.
         */
        uint8_t FOptsLen        : 4;
        /*!
         * Frame pending bit.
         */
        uint8_t FPending        : 1;
        /*!
         * Message acknowledge bit.
         */
        uint8_t Ack             : 1;
        /*!
         * ADR acknowledgment request bit.
         */
        uint8_t AdrAckReq       : 1;
        /*!
         * ADR control in the frame header.
         */
        uint8_t Adr             : 1;
    }Bits;
}LoRaMacFrameCtrl_t;

/*!
 * The enumeration containing the status of the operation of a MAC service.
 */
typedef enum eLoRaMacEventInfoStatus
{
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
}LoRaMacEventInfoStatus_t;

/*!
 * LoRaMac TX/RX operation state.
 */
typedef union eLoRaMacFlags_t
{
    /*!
     * Byte-access to the bits.
     */
    uint8_t Value;
    /*!
     * The structure containing single access to bits.
     */
    struct sMacFlagBits
    {
        /*!
         * MCPS-Req pending
         */
        uint8_t McpsReq         : 1;
        /*!
         * MCPS-Ind pending
         */
        uint8_t McpsInd         : 1;
        /*!
         * MCPS-Ind pending. Skip indication to the application layer.
         */
        uint8_t McpsIndSkip     : 1;
        /*!
         * MLME-Req pending
         */
        uint8_t MlmeReq         : 1;
        /*!
         * MAC cycle done
         */
        uint8_t MacDone         : 1;
    }Bits;
}LoRaMacFlags_t;

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
 * MCPS-Request     | \ref LoRaMacMlmeRequest
 * MCPS-Confirm     | MacMcpsConfirm in \ref LoRaMacPrimitives_t
 * MCPS-Indication  | MacMcpsIndication in \ref LoRaMacPrimitives_t
 */
typedef enum eMcps
{
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
}Mcps_t;

/*!
 * LoRaMAC MCPS-Request for an unconfirmed frame.
 */
typedef struct sMcpsReqUnconfirmed
{
    /*!
     * Frame port field. Must be set if the payload is not empty. Use the
     * application-specific frame port values: [1...223].
     *
     * LoRaWAN Specification V1.0.2, chapter 4.3.2.
     */
    uint8_t fPort;
    /*!
     * A pointer to the buffer of the frame payload.
     */
    void *fBuffer;
    /*!
     * The size of the frame payload.
     */
    uint16_t fBufferSize;
    /*!
     * Uplink datarate, if ADR is off.
     */
    int8_t Datarate;
}McpsReqUnconfirmed_t;

/*!
 * LoRaMAC MCPS-Request for a confirmed frame.
 */
typedef struct sMcpsReqConfirmed
{
    /*!
     * Frame port field. Must be set if the payload is not empty. Use the
     * application-specific frame port values: [1...223].
     *
     * LoRaWAN Specification V1.0.2, chapter 4.3.2.
     */
    uint8_t fPort;
    /*!
     * A pointer to the buffer of the frame payload.
     */
    void *fBuffer;
    /*!
     * The size of the frame payload.
     */
    uint16_t fBufferSize;
    /*!
     * Uplink datarate, if ADR is off.
     */
    int8_t Datarate;
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
     * Note that if NbTrials is set to 1 or 2, the MAC will not decrease
     * the datarate, if the LoRaMAC layer did not receive an acknowledgment.
     */
    uint8_t NbTrials;
}McpsReqConfirmed_t;

/*!
 * LoRaMAC MCPS-Request for a proprietary frame.
 */
typedef struct sMcpsReqProprietary
{
    /*!
     * A pointer to the buffer of the frame payload.
     */
    void *fBuffer;
    /*!
     * The size of the frame payload.
     */
    uint16_t fBufferSize;
    /*!
     * Uplink datarate, if ADR is off.
     */
    int8_t Datarate;
}McpsReqProprietary_t;

/*!
 * LoRaMAC MCPS-Request structure.
 */
typedef struct sMcpsReq
{
    /*!
     * MCPS-Request type.
     */
    Mcps_t Type;

    /*!
     * MCPS-Request parameters.
     */
    union uMcpsParam
    {
        /*!
         * MCPS-Request parameters for an unconfirmed frame.
         */
        McpsReqUnconfirmed_t Unconfirmed;
        /*!
         * MCPS-Request parameters for a confirmed frame.
         */
        McpsReqConfirmed_t Confirmed;
        /*!
         * MCPS-Request parameters for a proprietary frame.
         */
        McpsReqProprietary_t Proprietary;
    }Req;
}McpsReq_t;

/*!
 * LoRaMAC MCPS-Confirm.
 */
typedef struct sMcpsConfirm
{
    /*!
     * Holds the previously performed MCPS-Request.
     */
    Mcps_t McpsRequest;
    /*!
     * The status of the operation.
     */
    LoRaMacEventInfoStatus_t Status;
    /*!
     * The uplink datarate.
     */
    uint8_t Datarate;
    /*!
     * The transmission power.
     */
    int8_t TxPower;
    /*!
     * Set if an acknowledgement was received.
     */
    bool AckReceived;
    /*!
     * Provides the number of retransmissions.
     */
    uint8_t NbRetries;
    /*!
     * The transmission time on air of the frame.
     */
    TimerTime_t TxTimeOnAir;
    /*!
     * The uplink counter value related to the frame.
     */
    uint32_t UpLinkCounter;
    /*!
     * The uplink frequency related to the frame.
     */
    uint32_t UpLinkFrequency;
}McpsConfirm_t;

/*!
 * LoRaMAC MCPS-Indication primitive.
 */
typedef struct sMcpsIndication
{
    /*!
     * MCPS-Indication type.
     */
    Mcps_t McpsIndication;
    /*!
     * The status of the operation.
     */
    LoRaMacEventInfoStatus_t Status;
    /*!
     * Multicast.
     */
    uint8_t Multicast;
    /*!
     * The application port.
     */
    uint8_t Port;
    /*!
     * The downlink datarate.
     */
    uint8_t RxDatarate;
    /*!
     * Frame pending status.
     */
    uint8_t FramePending;
    /*!
     * A pointer to the received data stream.
     */
    uint8_t *Buffer;
    /*!
     * The size of the received data stream.
     */
    uint8_t BufferSize;
    /*!
     * Indicates, if data is available.
     */
    bool RxData;
    /*!
     * The RSSI of the received packet.
     */
    int16_t Rssi;
    /*!
     * The SNR of the received packet.
     */
    uint8_t Snr;
    /*!
     * The receive window.
     *
     * [0: Rx window 1, 1: Rx window 2]
     */
    uint8_t RxSlot;
    /*!
     * Set if an acknowledgement was received.
     */
    bool AckReceived;
    /*!
     * The downlink counter value for the received frame.
     */
    uint32_t DownLinkCounter;
}McpsIndication_t;

/*!
 * \brief LoRaMAC management services.
 *
 * \details The following table list the primitives supported by a
 *          specific MAC management service:
 *
 * Name                  | Request | Indication | Response | Confirm
 * --------------------- | :-----: | :--------: | :------: | :-----:
 * \ref MLME_JOIN        | YES     | NO         | NO       | YES
 * \ref MLME_LINK_CHECK  | YES     | NO         | NO       | YES
 * \ref MLME_TXCW        | YES     | NO         | NO       | YES
 *
 * The following table provides links to the function implementations of the
 * related MLME primitives.
 *
 * Primitive        | Function
 * ---------------- | :---------------------:
 * MLME-Request     | \ref LoRaMacMlmeRequest
 * MLME-Confirm     | MacMlmeConfirm in \ref LoRaMacPrimitives_t
 */
typedef enum eMlme
{
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
}Mlme_t;

/*!
 * LoRaMAC MLME-Request for the join service.
 */
typedef struct sMlmeReqJoin
{
    /*!
     * A globally unique end-device identifier.
     *
     * LoRaWAN Specification V1.0.2, chapter 6.2.1.
     */
    uint8_t *DevEui;
    /*!
     * An application identifier.
     *
     * LoRaWAN Specification V1.0.2, chapter 6.1.2
     */
    uint8_t *AppEui;
    /*!
     * AES-128 application key.
     *
     * LoRaWAN Specification V1.0.2, chapter 6.2.2.
     */
    uint8_t *AppKey;
    /*!
     * The number of trials for the join request.
     */
    uint8_t NbTrials;
}MlmeReqJoin_t;

/*!
 * LoRaMAC MLME-Request for TX continuous wave mode.
 */
typedef struct sMlmeReqTxCw
{
    /*!
     * The time while the radio is kept in continuous wave mode, in seconds.
     */
    uint16_t Timeout;
    /*!
     * The RF frequency to set (only used with the new way).
     */
    uint32_t Frequency;
    /*!
     * The RF output power to set (only used with the new way).
     */
    uint8_t Power;
}MlmeReqTxCw_t;

/*!
 * LoRaMAC MLME-Request structure.
 */
typedef struct sMlmeReq
{
    /*!
     * MLME-Request type.
     */
    Mlme_t Type;

    /*!
     * MLME-Request parameters.
     */
    union uMlmeParam
    {
        /*!
         * MLME-Request parameters for a join request.
         */
        MlmeReqJoin_t Join;
        /*!
         * MLME-Request parameters for TX continuous mode request.
         */
        MlmeReqTxCw_t TxCw;
    }Req;
}MlmeReq_t;

/*!
 * LoRaMAC MLME-Confirm primitive.
 */
typedef struct sMlmeConfirm
{
    /*!
     * The previously performed MLME-Request.
     */
    Mlme_t MlmeRequest;
    /*!
     * The status of the operation.
     */
    LoRaMacEventInfoStatus_t Status;
    /*!
     * The transmission time on air of the frame.
     */
    TimerTime_t TxTimeOnAir;
    /*!
     * The demodulation margin. Contains the link margin [dB] of the last LinkCheckReq
     * successfully received.
     */
    uint8_t DemodMargin;
    /*!
     * The number of gateways which received the last LinkCheckReq.
     */
    uint8_t NbGateways;
    /*!
     * The number of retransmissions.
     */
    uint8_t NbRetries;
}MlmeConfirm_t;

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
 * MIB-Set          | \ref LoRaMacMibSetRequestConfirm
 * MIB-Get          | \ref LoRaMacMibGetRequestConfirm
 */
typedef enum eMib
{
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
}Mib_t;

/*!
 * LoRaMAC MIB parameters.
 */
typedef union uMibParam
{
    /*!
     * LoRaWAN device class.
     *
     * Related MIB type: \ref MIB_DEVICE_CLASS
     */
    DeviceClass_t Class;
    /*!
     * LoRaWAN network joined attribute
     *
     * Related MIB type: \ref MIB_NETWORK_JOINED
     */
    bool IsNetworkJoined;
    /*!
     * Activation state of ADR
     *
     * Related MIB type: \ref MIB_ADR
     */
    bool AdrEnable;
    /*!
     * Network identifier
     *
     * Related MIB type: \ref MIB_NET_ID
     */
    uint32_t NetID;
    /*!
     * End-device address
     *
     * Related MIB type: \ref MIB_DEV_ADDR
     */
    uint32_t DevAddr;
    /*!
     * Network session key
     *
     * Related MIB type: \ref MIB_NWK_SKEY
     */
    uint8_t *NwkSKey;
    /*!
     * Application session key
     *
     * Related MIB type: \ref MIB_APP_SKEY
     */
    uint8_t *AppSKey;
    /*!
     * Enable or disable a public network
     *
     * Related MIB type: \ref MIB_PUBLIC_NETWORK
     */
    bool EnablePublicNetwork;
    /*!
     * Enable or disable repeater support
     *
     * Related MIB type: \ref MIB_REPEATER_SUPPORT
     */
    bool EnableRepeaterSupport;
    /*!
     * LoRaWAN channel
     *
     * Related MIB type: \ref MIB_CHANNELS
     */
    ChannelParams_t* ChannelList;
     /*!
     * Channel for the receive window 2
     *
     * Related MIB type: \ref MIB_RX2_CHANNEL
     */
    Rx2ChannelParams_t Rx2Channel;
     /*!
     * Channel for the receive window 2
     *
     * Related MIB type: \ref MIB_RX2_DEFAULT_CHANNEL
     */
    Rx2ChannelParams_t Rx2DefaultChannel;
    /*!
     * Channel mask
     *
     * Related MIB type: \ref MIB_CHANNELS_MASK
     */
    uint16_t* ChannelsMask;
    /*!
     * Default channel mask
     *
     * Related MIB type: \ref MIB_CHANNELS_DEFAULT_MASK
     */
    uint16_t* ChannelsDefaultMask;
    /*!
     * Number of frame repetitions
     *
     * Related MIB type: \ref MIB_CHANNELS_NB_REP
     */
    uint8_t ChannelNbRep;
    /*!
     * Maximum receive window duration
     *
     * Related MIB type: \ref MIB_MAX_RX_WINDOW_DURATION
     */
    uint32_t MaxRxWindow;
    /*!
     * Receive delay 1
     *
     * Related MIB type: \ref MIB_RECEIVE_DELAY_1
     */
    uint32_t ReceiveDelay1;
    /*!
     * Receive delay 2
     *
     * Related MIB type: \ref MIB_RECEIVE_DELAY_2
     */
    uint32_t ReceiveDelay2;
    /*!
     * Join accept delay 1
     *
     * Related MIB type: \ref MIB_JOIN_ACCEPT_DELAY_1
     */
    uint32_t JoinAcceptDelay1;
    /*!
     * Join accept delay 2
     *
     * Related MIB type: \ref MIB_JOIN_ACCEPT_DELAY_2
     */
    uint32_t JoinAcceptDelay2;
    /*!
     * Channels data rate
     *
     * Related MIB type: \ref MIB_CHANNELS_DEFAULT_DATARATE
     */
    int8_t ChannelsDefaultDatarate;
    /*!
     * Channels data rate
     *
     * Related MIB type: \ref MIB_CHANNELS_DATARATE
     */
    int8_t ChannelsDatarate;
    /*!
     * Channels TX power
     *
     * Related MIB type: \ref MIB_CHANNELS_DEFAULT_TX_POWER
     */
    int8_t ChannelsDefaultTxPower;
    /*!
     * Channels TX power
     *
     * Related MIB type: \ref MIB_CHANNELS_TX_POWER
     */
    int8_t ChannelsTxPower;
    /*!
     * LoRaWAN uplink counter
     *
     * Related MIB type: \ref MIB_UPLINK_COUNTER
     */
    uint32_t UpLinkCounter;
    /*!
     * LoRaWAN downlink counter
     *
     * Related MIB type: \ref MIB_DOWNLINK_COUNTER
     */
    uint32_t DownLinkCounter;
    /*!
     * Multicast channel
     *
     * Related MIB type: \ref MIB_MULTICAST_CHANNEL
     */
    MulticastParams_t* MulticastList;
    /*!
     * System overall timing error in milliseconds
     *
     * Related MIB type: \ref MIB_SYSTEM_MAX_RX_ERROR
     */
    uint32_t SystemMaxRxError;
    /*!
     * Minimum required number of symbols to detect an RX frame
     *
     * Related MIB type: \ref MIB_MIN_RX_SYMBOLS
     */
    uint8_t MinRxSymbols;
    /*!
     * Antenna gain
     *
     * Related MIB type: \ref MIB_ANTENNA_GAIN
     */
    float AntennaGain;
}MibParam_t;

/*!
 * LoRaMAC MIB-RequestConfirm structure
 */
typedef struct eMibRequestConfirm
{
    /*!
     * MIB-Request type
     */
    Mib_t Type;

    /*!
     * MLME-RequestConfirm parameters
     */
    MibParam_t Param;
}MibRequestConfirm_t;

/*!
 * LoRaMAC TX information
 */
typedef struct sLoRaMacTxInfo
{
    /*!
     * Defines the size of the applicable payload that can be processed.
     */
    uint8_t MaxPossiblePayload;
    /*!
     * The current payload size, dependent on the current datarate.
     */
    uint8_t CurrentPayloadSize;
}LoRaMacTxInfo_t;

/*!
 * LoRaMAC status.
 */
typedef enum eLoRaMacStatus
{
    /*!
     * Service started successfully.
     */
    LORAMAC_STATUS_OK,
    /*!
     * Service not started - LoRaMAC is busy.
     */
    LORAMAC_STATUS_BUSY,
    /*!
     * Service unknown.
     */
    LORAMAC_STATUS_SERVICE_UNKNOWN,
    /*!
     * Service not started - invalid parameter.
     */
    LORAMAC_STATUS_PARAMETER_INVALID,
    /*!
     * Service not started - invalid frequency.
     */
    LORAMAC_STATUS_FREQUENCY_INVALID,
    /*!
     * Service not started - invalid datarate.
     */
    LORAMAC_STATUS_DATARATE_INVALID,
    /*!
     * Service not started - invalid frequency and datarate.
     */
    LORAMAC_STATUS_FREQ_AND_DR_INVALID,
    /*!
     * Service not started - the device is not in a LoRaWAN.
     */
    LORAMAC_STATUS_NO_NETWORK_JOINED,
    /*!
     * Service not started - payload length error.
     */
    LORAMAC_STATUS_LENGTH_ERROR,
    /*!
     * Service not started - the device is switched off.
     */
    LORAMAC_STATUS_DEVICE_OFF,
    /*!
     * Service not started - the specified region is not supported
     * or not activated with preprocessor definitions.
     */
    LORAMAC_STATUS_REGION_NOT_SUPPORTED,
    /*!
     * Crypto methods failure.
     */
    LORAMAC_STATUS_CRYPTO_FAIL,
}LoRaMacStatus_t;

/*!
 * LoRaMAC events structure.
 * Used to notify upper layers of MAC events.
 */
typedef struct sLoRaMacPrimitives
{
    /*!
     * \brief   MCPS-Confirm primitive.
     *
     * \param   [OUT] MCPS-Confirm parameters.
     */
    mbed::Callback<void(McpsConfirm_t*)> MacMcpsConfirm;

    /*!
     * \brief   MCPS-Indication primitive.
     *
     * \param   [OUT] MCPS-Indication parameters.
     */
    mbed::Callback<void(McpsIndication_t*)> MacMcpsIndication;

    /*!
     * \brief   MLME-Confirm primitive.
     *
     * \param   [OUT] MLME-Confirm parameters.
     */
    mbed::Callback<void(MlmeConfirm_t*)> MacMlmeConfirm;
}LoRaMacPrimitives_t;

/*!
 * LoRaMAC callback structure.
 */
typedef struct sLoRaMacCallback
{
    /*!
     * \brief   Measures the battery level.
     *
     * \retval  The battery level [0: node is connected to an external
     *          power source, 1..254: battery level, where 1 is the minimum
     *          and 254 is the maximum value, 255: the node was not able
     *          to measure the battery level].
     */
    uint8_t ( *GetBatteryLevel )( void );

}LoRaMacCallback_t;

/**
 * The AES encryption/decryption cipher application session key.
 */
#ifdef MBED_CONF_LORA_APPSKEY
#define LORAWAN_APPSKEY                               MBED_CONF_LORA_APPSKEY
#else
#define LORAWAN_APPSKEY          {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#endif

/**
 * The AES encryption/decryption cipher network session key.
 */
#ifdef MBED_CONF_LORA_NWKSKEY
#define LORAWAN_NWKSKEY                               MBED_CONF_LORA_NWKSKEY
#else
#define LORAWAN_NWKSKEY          {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#endif

/**
 * The device address on the network (big endian).
 */
#ifdef MBED_CONF_LORA_DEVICE_ADDRESS
#define LORAWAN_DEVICE_ADDRESS                        MBED_CONF_LORA_DEVICE_ADDRESS
#else
#define LORAWAN_DEVICE_ADDRESS                        0
#endif

/**
 * The current network ID.
 */
#ifdef MBED_CONF_LORA_NETWORK_ID
#define LORAWAN_NETWORK_ID                            MBED_CONF_LORA_NETWORK_ID
#else
#define LORAWAN_NETWORK_ID                            0
#endif

/**
 * The AES encryption/decryption cipher application key.
 */
#ifdef MBED_CONF_LORA_APPLICATION_KEY
#define LORAWAN_APPLICATION_KEY                       MBED_CONF_LORA_APPLICATION_KEY
#else
#define LORAWAN_APPLICATION_KEY  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#endif

/**
 * The application IEEE EUI.
 */

#ifdef MBED_CONF_LORA_APPLICATION_EUI
#define LORAWAN_APPLICATION_EUI                       MBED_CONF_LORA_APPLICATION_EUI
#else
#define LORAWAN_APPLICATION_EUI                       {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#endif

/**
 * The mote device IEEE EUI.
 */
#ifdef MBED_CONF_LORA_DEVICE_EUI
#define LORAWAN_DEVICE_EUI                            MBED_CONF_LORA_DEVICE_EUI
#else
#define LORAWAN_DEVICE_EUI                            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#endif

/**
 * Indicates how many times join can be tried.
 */
#ifdef MBED_CONF_LORA_NB_TRIALS
#define LORAWAN_NB_TRIALS                             MBED_CONF_LORA_NB_TRIALS
#else
#define LORAWAN_NB_TRIALS                             8
#endif

/**
 * When set to true, the application uses the Over-the-Air activation procedure.
 */
#if defined(MBED_CONF_LORA_OVER_THE_AIR_ACTIVATION) == 1
#define OVER_THE_AIR_ACTIVATION                       MBED_CONF_LORA_OVER_THE_AIR_ACTIVATION
#else
#define OVER_THE_AIR_ACTIVATION                       false
#endif

/**
 * LoRaWAN connects to a public network or private network, true = public network.
 */
#if defined(MBED_CONF_LORA_PUBLIC_NETWORK) == 1
#define LORAWAN_PUBLIC_NETWORK                        MBED_CONF_LORA_PUBLIC_NETWORK
#else
#define LORAWAN_PUBLIC_NETWORK                        false
#endif

/**
 * Maximum PHY layer payload size for reception.
 * This is not user configurable. Its hard coded in LoRaMac.cpp
 * and we don't want to change that file too much
 */
#define LORAMAC_PHY_MAXPAYLOAD                        255

/**
 *
 * Default user application maximum data size for trasnmission
 */
// reject if user tries to set more than MTU
#if defined MBED_CONF_LORA_TX_MAX_SIZE  && MBED_CONF_LORA_TX_MAX_SIZE > 255
#warning "Cannot set TX Max size more than MTU=255"
#define LORAWAN_TX_MAX_SIZE   255
#elif defined MBED_CONF_LORA_TX_MAX_SIZE  && MBED_CONF_LORA_TX_MAX_SIZE < 255
#define LORAWAN_TX_MAX_SIZE                      MBED_CONF_LORA_TX_MAX_SIZE
#else
#define LORAWAN_TX_MAX_SIZE                      64
#endif

/**
 *
 * Defines the application data transmission timer cycle, value in [ms]
 * Used only when automatic duty cycling is off
 */
#ifdef MBED_CONF_APP_TX_TIMER
#define TX_TIMER                              MBED_CONF_APP_TX_TIMER
#else
#define TX_TIMER                              5000
#endif

/**
 *
 * Defines a random delay for application data transmission cycle, value in [ms]
 * Used only when automatic duty cycling is off
 */
#ifdef MBED_CONF_APP_TX_TIMER_RND
#define TX_TIMER_RND                          MBED_CONF_APP_TX_TIMER_RND
#else
#define TX_TIMER_RND                          1000
#endif

/**
 *
 * LoRaWAN Adaptive Data Rate
 *
 * \remark Please note that when ADR is enabled, the end-device should be static.
 */
#if defined(MBED_CONF_LORA_ADR_ON) == 1
#define LORAWAN_ADR_ON                                MBED_CONF_LORA_ADR_ON
#else
#define LORAWAN_ADR_ON                                false
#endif

/**
 *
 * The default application port.
 */
#ifdef MBED_CONF_LORA_APP_PORT
#define LORAWAN_APP_PORT                              MBED_CONF_LORA_APP_PORT
#else
#define LORAWAN_APP_PORT                              0x15
#endif

/**
 * Default duty cycling setting
 */
#if defined(MBED_CONF_LORA_DUTY_CYCLE_ON) == 1
#define LORAWAN_DUTYCYCLE_ON        MBED_CONF_LORA_DUTY_CYCLE_ON
#else
#define LORAWAN_DUTYCYCLE_ON    false
#endif

/**
 * Listen-before-talk setting
 */
#if defined(MBED_CONF_LORA_LBT_ON) == 1
#define LORAWAN_LBT_ON        MBED_CONF_LORA_LBT_ON
#else
#define LORAWAN_LBT_ON    false
#endif

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
    DEVICE_STATE_COMPLIANCE_TEST,
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
typedef struct lorawan_connect_otaa {
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
typedef struct lorawan_connect_abp {
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

/** LoRaMAC data services
 *
 */
typedef enum lora_mac_mcps {
    LORA_MCPS_UNCONFIRMED = 0,  /**< Unconfirmed LoRaMAC frame */
    LORA_MCPS_CONFIRMED,        /**< Confirmed LoRaMAC frame */
    LORA_MCPS_MULTICAST,        /**< Multicast LoRaMAC frame */
    LORA_MCPS_PROPRIETARY,      /**< Proprietary frame */
} lora_mac_mcps_t;

/** LoRaMAC management services
 *
 */
typedef enum lora_mac_mlme {
    LORA_MLME_JOIN,         /**< Initiates the Over-the-Air activation */
    LORA_MLME_LINK_CHECK,   /**< LinkCheckReq - Connectivity validation */
    LORA_MLME_TXCW,         /**< Sets Tx continuous wave mode */
    LORA_MLME_TXCW_1,       /**< Sets Tx continuous wave mode (new LoRa-Alliance CC definition) */
} lora_mac_mlme_t;

/** Unconfirmed message.
 *
 * A message for an unconfirmed frame.
 */
typedef struct lora_mac_unconfirmed {
    /** Frame port field.
     *
     * Must be set if the payload is not empty. Use the
     * application-specific frame port values: [1...223]
     *
     * LoRaWAN Specification V1.0.2, chapter 4.3.2
     */
    uint8_t f_port;
    /** Uplink datarate
     *
     * Used if ADR is off
     */
    int8_t datarate;
} lora_mac_unconfirmed_t;

/** Confirmed message.
 *
 * A message for a confirmed frame.
 */
typedef struct lora_mac_confirmed {
    /** Frame port field.
     *
     * Must be set if the payload is not empty. Use the
     * application-specific frame port values: [1...223]
     *
     * LoRaWAN Specification V1.0.2, chapter 4.3.2
     */
    uint8_t f_port;
    /** Uplink datarate.
     *
     * Used if ADR is off.
     */
    int8_t datarate;
    /** Number of trials.
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
     * Note that if NbTrials is set to 1 or 2, the MAC does not decrease
     * the datarate, if the LoRaMAC layer did not receive an acknowledgment.
     */
    uint8_t nb_trials;
} lora_mac_confirmed_t;

/** A proprietary message.
 *
 * A message for a proprietary frame.
 */
typedef struct lora_mac_proprietary {
    /** Uplink datarate.
     *
     *  Used if ADR is off.
     */
    int8_t datarate;
} lora_mac_proprietary_t;

/** LoRaMAC message structure.
 *
 */
typedef struct lora_mac_tx_message {

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
    lora_mac_mcps_t type;
    /** Message parameters.
     *
     */
    union message {
        /** An unconfirmed frame.
         *
         * The message parameters for an unconfirmed frame.
         */
        lora_mac_unconfirmed_t unconfirmed;
        /** A confirmed frame.
         *
         * The message parameters for a confirmed frame.
         */
        lora_mac_confirmed_t confirmed;
        /** A proprietary frame.
         *
         * The message parameters for a proprietary frame.
         */
        lora_mac_proprietary_t proprietary;
    } message_u;

    /** Payload data
     *
     * Base pointer to the buffer
     */
    uint8_t f_buffer[LORAWAN_TX_MAX_SIZE];

    /** Payload size.
     *
     * The size of the frame payload.
     */
    uint16_t f_buffer_size;

    /**
     * Pending data size
     */
    uint16_t pending_size;

} lora_mac_tx_message_t;

/** LoRaMAC status.
 *
 */
typedef enum lora_mac_status {
    LORA_MAC_STATUS_OK = 0,                         /**< Service started successfully */
    LORA_MAC_STATUS_BUSY = -1000,                   /**< Service not started - LoRaMAC is busy */
    LORA_MAC_STATUS_WOULD_BLOCK = -1001,            /**< LoRaMAC cannot send at the moment or have nothing to read */
    LORA_MAC_STATUS_SERVICE_UNKNOWN = -1002,        /**< Service unknown */
    LORA_MAC_STATUS_PARAMETER_INVALID = -1003,      /**< Service not started - invalid parameter */
    LORA_MAC_STATUS_FREQUENCY_INVALID = -1004,      /**< Service not started - invalid frequency */
    LORA_MAC_STATUS_DATARATE_INVALID = -1005,       /**< Service not started - invalid datarate */
    LORA_MAC_STATUS_FREQ_AND_DR_INVALID = -1006,    /**< Service not started - invalid frequency and datarate */
    LORA_MAC_STATUS_NO_NETWORK_JOINED = -1009,      /**< Service not started - the device is not in a LoRaWAN */
    LORA_MAC_STATUS_LENGTH_ERROR = -1010,           /**< Service not started - payload lenght error */
    LORA_MAC_STATUS_DEVICE_OFF = -1011,             /**< Service not started - the device is switched off */
    LORA_MAC_STATUS_NOT_INITIALIZED = -1012,        /**< Service not started - stack not initialized */
    LORA_MAC_STATUS_UNSUPPORTED = -1013,            /**< Service not supported */
    LORA_MAC_STATUS_CRYPTO_FAIL = -1014,            /**< Service not started - crypto failure */
    LORA_MAC_STATUS_PORT_INVALID = -1015,           /**< Invalid port */
    LORA_MAC_STATUS_CONNECT_IN_PROGRESS = -1016,    /**< Services started - Connection in progress */
    LORA_MAC_STATUS_NO_ACTIVE_SESSIONS = -1017,            /**< Services not started - No active session */
    LORA_MAC_STATUS_IDLE = -1018,                   /**< Services started - Idle at the moment */
    LORA_MAC_STATUS_COMPLIANCE_TEST_ON = -1019         /**< Compliance test - is on-going */
} lora_mac_status_t;

/**
 *
 * Enumeration containing the status of the operation of a MAC service
 */
typedef enum lora_mac_event_info_status {
    LORA_EVENT_INFO_STATUS_OK = 0,                           /**< Service performed successfully */
    LORA_EVENT_INFO_STATUS_ERROR,                            /**< An error occurred during the execution of the service */
    LORA_EVENT_INFO_STATUS_TX_TIMEOUT,                       /**< A TX timeout occurred */
    LORA_EVENT_INFO_STATUS_RX1_TIMEOUT,                      /**< An RX timeout occurred on receive window 1 */
    LORA_EVENT_INFO_STATUS_RX2_TIMEOUT,                      /**< An RX timeout occurred on receive window 2 */
    LORA_EVENT_INFO_STATUS_RX1_ERROR,                        /**< An RX error occurred on receive window 1 */
    LORA_EVENT_INFO_STATUS_RX2_ERROR,                        /**< An RX error occurred on receive window 2 */
    LORA_EVENT_INFO_STATUS_JOIN_FAIL,                        /**< An error occurred in the join procedure */
    LORA_EVENT_INFO_STATUS_DOWNLINK_REPEATED,                /**< A frame with an invalid downlink counter */
    LORA_EVENT_INFO_STATUS_TX_DR_PAYLOAD_SIZE_ERROR,         /**< Payload size is not applicable for the datarate */
    LORA_EVENT_INFO_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOSS,    /**< The node has lost MAX_FCNT_GAP or more frames. */
    LORA_EVENT_INFO_STATUS_ADDRESS_FAIL,                     /**< An address error occurred */
    LORA_EVENT_INFO_STATUS_MIC_FAIL,                         /**< Message integrity check failure */
    LORA_EVENT_INFO_STATUS_CRYPTO_FAIL                       /**< Crypto error*/
} lora_mac_event_info_status_t;

/**
 * LoRaMAC MLME-Request for TX continuous wave mode.
 */
typedef struct mlme_req_tx_cw {
    /**
     * The time while the radio is kept in continuous wave mode, in seconds.
     */
    uint16_t timeout;
    /**
     * RF frequency to set (only used with the new way).
     */
    uint32_t frequency;
    /**
     * RF output power to set (only used with the new way).
     */
    uint8_t power;
} mlme_req_tx_cw_t;

/**
 * LoRaMAC MLME-Request structure
 */
typedef struct lora_mac_mlme_req {
    /**
     * MLME-Request type
     */
    lora_mac_mlme_t type;

    /**
     * MLME-Request parameters
     */
    union mlme_param {
        /**
         * MLME-Request parameters for a join request
         */
        lorawan_connect_otaa_t join;
        /**
         * MLME-Request parameters for TX continuous mode request
         */
        mlme_req_tx_cw_t tx_cw;
    } req;
} lora_mac_mlme_req_t;

/**
 * LoRaMAC MCPS-Request structure
 */
typedef struct lora_mac_mcps_req {
    /**
     * MCPS-Request type
     */
    lora_mac_mcps_t type;

    /**
     * MCPS-Request parameters
     */
    union mcps_param
    {
        /**
         * MCPS-Request parameters for an unconfirmed frame.
         */
        lora_mac_unconfirmed_t unconfirmed;
        /**
         * MCPS-Request parameters for a confirmed frame.
         */
        lora_mac_confirmed_t confirmed;
        /**
         * MCPS-Request parameters for a proprietary frame.
         */
        lora_mac_proprietary_t proprietary;
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
} lora_mac_mcps_req_t;

/** LoRaMAC MLME-Confirm
 *
 */
typedef struct lora_mac_mlme_confirm {
    /** The previous MLME-Request.
     *
     * The previously performed MLME-Request.
     */
    lora_mac_mlme_t mlme_request;
    /** The status of the operation.
     *
     * The current status of the MAC service operation.
     */
    lora_mac_event_info_status_t status;
    /** Time on air.
     *
     * The transmission time on air of the frame.
     */
    TimerTime_t tx_time_on_air;
    /** Demodulation margin.
     *
     * The link margin [dB] of the last LinkCheckReq successfully received.
     */
    uint8_t demod_margin;
    /** The number of gateways.
     *
     * The number of gateways that received the last LinkCheckReq.
     */
    uint8_t nb_gateways;
    /** The retransmission counter.
     *
     * The number of retransmissions.
     */
    uint8_t nb_retries;
} lora_mac_mlme_confirm_t;

/** LoRaMAC MCPS-Confirm
 *
 */
typedef struct lora_mac_mcps_confirm {
    /** MCPS-request
     *
     * Holds the previously performed MCPS-Request.
     */
    lora_mac_mcps_t mcps_request;
    /** The status of the operation.
     *
     * The current status of MAC service operation.
     */
    lora_mac_event_info_status_t status;
    /** Uplink datarate
     *
     */
    uint8_t datarate;
    /** Transmission power
     *
     */
    int8_t tx_power;
    /** ACK-received
     *
     * Set if an acknowledgement was received.
     */
    bool ack_received;
    /** Retransmission counter
     *
     * Provides the number of retransmissions.
     */
    uint8_t nb_retries;
    /** Time on air
     *
     * The transmission time on air of the frame.
     */
    TimerTime_t tx_time_on_air;
    /** Uplink counter
     *
     * The uplink counter value related to the frame.
     */
    uint32_t uplink_counter;
    /** Uplink frequency
     *
     * The uplink frequency related to the frame.
     */
    uint32_t uplink_frequency;
} lora_mac_mcps_confirm_t;

/** LoRaMAC MCPS-Indication
 *
 */
typedef struct lora_mac_mcps_indication {
    /** MCPS-Indication type
     *
     */
    lora_mac_mcps_t mcps_indication;
    /** The status of the operation
     *
     * The current status of MAC service operation.
     */
    lora_mac_event_info_status_t status;
    /** Multicast
     *
     * This is a multicast message.
     */
    uint8_t multicast;
    /** Application port
     *
     */
    uint8_t port;
    /** Downlink datarate
     *
     */
    uint8_t rx_datarate;
    /** Frame pending
     *
     * The frame is pending.
     */
    uint8_t frame_pending;
    /** Payload data
     *
     * A pointer to the received data stream.
     */
    uint8_t buffer[LORAMAC_PHY_MAXPAYLOAD];
    /** Payload size
     *
     * The size of the received data stream.
     */
    uint16_t buffer_size;
    /** RX-data indication
     *
     * Indicates, if data is available.
     */
    bool rx_data;
    /** Packet RSSI
     *
     * The RSSI of the received packet.
     */
    int16_t rssi;
    /** Packet SNR
     *
     * The SNR of the received packet.
     */
    uint8_t snr;
    /** Receive window
     *
     * [0: Rx window 1, 1: Rx window 2]
     */
    uint8_t rx_slot;
    /** ACK-received
     *
     * Set if an acknowledgement was received.
     */
    bool ack_received;
    /** Downlink counter
     *
     * The downlink counter value for the received frame.
     */
    uint32_t downlink_counter;
} lora_mac_mcps_indication_t;

/** lora_mac_rx_message_type_t
 *
 * An enum representing a structure for RX messages.
 */
typedef enum lora_mac_rx_message_type {
    LORAMAC_RX_MLME_CONFIRM = 0,    /**< lora_mac_mlme_confirm_t */
    LORAMAC_RX_MCPS_CONFIRM,        /**< lora_mac_mcps_confirm_t */
    LORAMAC_RX_MCPS_INDICATION      /**< lora_mac_mcps_indication_t */
} lora_mac_rx_message_type_t;

/** lora_mac_rx_message_by_type_t union
 *
 * A union representing a structure for RX messages.
 */
typedef union lora_mac_rx_message_by_type_t {
    lora_mac_mlme_confirm_t mlme_confirm;
    lora_mac_mcps_confirm_t mcps_confirm;
    lora_mac_mcps_indication_t mcps_indication;
} lora_mac_rx_message_by_type_t;

/** lora_mac_rx_message_t
 *
 * A structure representing a structure for an RX message.
 */
typedef struct lora_mac_rx_message {
    bool receive_ready;
    lora_mac_rx_message_type_t type;
    lora_mac_rx_message_by_type_t rx_message;
    uint16_t pending_size;
    uint16_t prev_read_size;
} lora_mac_rx_message_t;

/** lora_mac_dr_range_t union
 *
 * A union representing a structure for the minimum and maximum data rate.
 */
typedef union lora_mac_dr_range {
    /** Byte-access
     *
     * Byte-access to the bits.
     */
    int8_t value;
    /** lora_mac_fields_s
     *
     * A structure to store the minimum and maximum data rate.
     */
    struct lora_mac_fields {
        /** Minimum data rate
         *
         * EU868 - [DR_0, DR_1, DR_2, DR_3, DR_4, DR_5, DR_6, DR_7]
         *
         * US915 - [DR_0, DR_1, DR_2, DR_3, DR_4]
         */
        int8_t min :4;
        /** Maximum data rate
         *
         * EU868 - [DR_0, DR_1, DR_2, DR_3, DR_4, DR_5, DR_6, DR_7]
         *
         * US915 - [DR_0, DR_1, DR_2, DR_3, DR_4]
         */
        int8_t max :4;
    } lora_mac_fields_s;
} lora_mac_dr_range_t;

/** LoRaWAN device class definition
 *
 */
typedef enum lora_mac_device_class {
    LORA_CLASS_A, /**< LoRaWAN device class A */
    LORA_CLASS_B, /**< LoRaWAN device class B */
    LORA_CLASS_C, /**< LoRaWAN device class C */
} lora_mac_device_class_t;

/** LoRaMAC channel definition
 *
 */
typedef struct lora_mac_channel_params {
    /** Frequency in Hz
     *
     */
    uint32_t frequency;
    /** Alternative frequency for RX window 1
     *
     */
    uint32_t rx1_frequency;
    /** Data rate definition
     *
     */
    lora_mac_dr_range_t dr_range;
    /** Band index
     *
     */
    uint8_t band;
} lora_mac_channel_params_t;

/**
 * Structure to hold A list of LoRa Channels
 */
typedef struct lora_channels_s {
    uint8_t id;
    lora_mac_channel_params_t ch_param;
} lora_channels_t;

/** LoRaMAC receive window 2 channel parameters
 *
 */
typedef struct lora_mac_rx2_channel_params {
    /** Frequency in Hz
     *
     */
    uint32_t frequency;
    /** Data rate
     *
     * EU868 - [DR_0, DR_1, DR_2, DR_3, DR_4, DR_5, DR_6, DR_7]
     *
     * US915 - [DR_8, DR_9, DR_10, DR_11, DR_12, DR_13]
     */
    uint8_t datarate;
} lora_mac_rx2_channel_params_t;

/** LoRaMAC multicast channel parameter
 *
 */
typedef struct lora_mac_multicast_params {
    /** Address
     *
     */
    uint32_t address;
    /** Network session key
     *
     */
    uint8_t nwk_skey[16];
    /** Application session key
     *
     */
    uint8_t app_skey[16];
    /** Downlink counter
     *
     */
    uint32_t downlink_counter;
    /** Next multicast
     *
     * A reference pointer to the next multicast channel parameters in the list
     */
    struct lora_mac_multicast_params *next;
} lora_mac_multicast_params_t;

/** Enum lora_mac_mib_t
 *
 */
typedef enum lora_mac_mib {
    LORA_MIB_DEVICE_CLASS,              /**< LoRaWAN device class */
    LORA_MIB_NETWORK_JOINED,            /**< LoRaWAN network joined attribute */
    LORA_MIB_ADR,                       /**< Adaptive data rate */
    LORA_MIB_NET_ID,                    /**< Network identifier */
    LORA_MIB_DEV_ADDR,                  /**< End-device address */
    LORA_MIB_NWK_SKEY,                  /**< Network session key */
    LORA_MIB_APP_SKEY,                  /**< Application session key */
    LORA_MIB_PUBLIC_NETWORK,            /**< Set the network type to public or private */
    LORA_MIB_REPEATER_SUPPORT,          /**< Support the operation with repeaters */
    LORA_MIB_CHANNELS,                  /**< Communication channels */
    LORA_MIB_RX2_CHANNEL,               /**< Set receive window 2 channel */
    LORA_MIB_RX2_DEFAULT_CHANNEL,       /**< Set receive window 2 channel */
    LORA_MIB_CHANNELS_MASK,             /**< LoRaWAN channels mask */
    LORA_MIB_CHANNELS_DEFAULT_MASK,     /**< LoRaWAN default channels mask */
    LORA_MIB_CHANNELS_NB_REP,           /**< Set the number of repetitions on a channel */
    LORA_MIB_MAX_RX_WINDOW_DURATION,    /**< Maximum receive window duration in [ms] */
    LORA_MIB_RECEIVE_DELAY_1,           /**< Receive delay 1 in [ms] */
    LORA_MIB_RECEIVE_DELAY_2,           /**< Receive delay 2 in [ms] */
    LORA_MIB_JOIN_ACCEPT_DELAY_1,       /**< Join accept delay 1 in [ms] */
    LORA_MIB_JOIN_ACCEPT_DELAY_2,       /**< Join accept delay 2 in [ms] */
    LORA_MIB_CHANNELS_DEFAULT_DATARATE, /**< Default data rate of a channel */
    LORA_MIB_CHANNELS_DATARATE,         /**< Data rate of a channel */
    LORA_MIB_CHANNELS_TX_POWER,         /**< Transmission power of a channel */
    LORA_MIB_CHANNELS_DEFAULT_TX_POWER, /**< Transmission power of a channel */
    LORA_MIB_UPLINK_COUNTER,            /**< LoRaWAN uplink counter */
    LORA_MIB_DOWNLINK_COUNTER,          /**< LoRaWAN downlink counter */
    LORA_MIB_MULTICAST_CHANNEL,         /**< Multicast channels */
    LORA_MIB_SYSTEM_MAX_RX_ERROR,       /**< System overall timing error in milliseconds. */
    LORA_MIB_MIN_RX_SYMBOLS,            /**< Minimum number of symbols required to detect an RX frame */
} lora_mac_mib_t;

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
typedef struct lora_dev_commission {
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
} lora_dev_commission_t;

/** LoRaMAC MIB parameters
 *
 */
typedef union lora_mac_mib_param {
    /** LoRaWAN device class
     *
     * Related MIB type: \ref MIB_DEVICE_CLASS
     */
    lora_mac_device_class_t lora_class;
    /** LoRaWAN network joined attribute
     *
     * Related MIB type: \ref MIB_NETWORK_JOINED
     */
    bool is_network_joined;
    /** Activation state of ADR
     *
     * Related MIB type: \ref MIB_ADR
     */
    bool adr_enable;
    /** Network identifier
     *
     * Related MIB type: \ref MIB_NET_ID
     */
    uint32_t net_id;
    /** End-device address
     *
     * Related MIB type: \ref MIB_DEV_ADDR
     */
    uint32_t dev_addr;
    /** Network session key
     *
     * Related MIB type: \ref MIB_NWK_SKEY
     */
    uint8_t *nwk_skey;
    /** Application session key
     *
     * Related MIB type: \ref MIB_APP_SKEY
     */
    uint8_t *app_skey;
    /** Enable public network
     *
     * Enable or disable a public network
     * Related MIB type: \ref MIB_PUBLIC_NETWORK
     */
    bool enable_public_network;
    /** Enable repeater support
     *
     * Enable or disable repeater support
     * Related MIB type: \ref MIB_REPEATER_SUPPORT
     */
    bool enable_repeater_support;
    /** LoRaWAN Channel
     *
     * Related MIB type: \ref MIB_CHANNELS
     */
    lora_mac_channel_params_t *channel_list;
    /** Channel rx 2
     *
     * Channel for the receive window 2
     * Related MIB type: \ref MIB_RX2_CHANNEL
     */
    lora_mac_rx2_channel_params_t rx2_channel;
    /** Default channel rx 2
     *
     * Channel for the receive window 2
     * Related MIB type: \ref MIB_RX2_DEFAULT_CHANNEL
     */
    lora_mac_rx2_channel_params_t rx2_default_channel;
    /** Channel mask
     *
     * Related MIB type: \ref MIB_CHANNELS_MASK
     */
    uint16_t *channels_mask;
    /** Default channel mask
     *
     * Related MIB type: \ref MIB_CHANNELS_DEFAULT_MASK
     */
    uint16_t *channels_default_mask;
    /** Frame repetition number
     *
     * Number of frame repetitions
     * Related MIB type: \ref MIB_CHANNELS_NB_REP
     */
    uint8_t channel_nb_rep;
    /** Maximum receive window duration
     *
     * Related MIB type: \ref MIB_MAX_RX_WINDOW_DURATION
     */
    uint32_t max_rx_window;
    /** Receive delay 1
     *
     * Related MIB type: \ref MIB_RECEIVE_DELAY_1
     */
    uint32_t receive_delay1;
    /** Receive delay 2
     *
     * Related MIB type: \ref MIB_RECEIVE_DELAY_2
     */
    uint32_t receive_delay2;
    /** Join accept delay 1
     *
     * Related MIB type: \ref MIB_JOIN_ACCEPT_DELAY_1
     */
    uint32_t join_accept_delay1;
    /** Join accept delay 2
     *
     * Related MIB type: \ref MIB_JOIN_ACCEPT_DELAY_2
     */
    uint32_t join_accept_delay2;
    /** Channels default data rate
     *
     * Related MIB type: \ref MIB_CHANNELS_DEFAULT_DATARATE
     */
    int8_t channels_default_datarate;
    /** Channels data rate
     *
     * Related MIB type: \ref MIB_CHANNELS_DATARATE
     */
    int8_t channels_datarate;
    /** Channels default TX power
     *
     * Related MIB type: \ref MIB_CHANNELS_DEFAULT_TX_POWER
     */
    int8_t channels_default_tx_power;
    /** Channels TX power
     *
     * Related MIB type: \ref MIB_CHANNELS_TX_POWER
     */
    int8_t channels_tx_power;
    /** LoRaWAN uplink counter
     *
     * Related MIB type: \ref MIB_UPLINK_COUNTER
     */
    uint32_t uplink_counter;
    /** LoRaWAN downlink counter
     *
     * Related MIB type: \ref MIB_DOWNLINK_COUNTER
     */
    uint32_t downlink_counter;
    /** Multicast channel
     *
     * Related MIB type: \ref MIB_MULTICAST_CHANNEL
     */
    lora_mac_multicast_params_t *multicast_list;
    /** Maximum RX error timing
     *
     * System overall timing error in milliseconds.
     * Related MIB type: \ref MIB_SYSTEM_MAX_RX_ERROR
     */
    uint32_t system_max_rx_error;
    /** Minimum RX symbols
     *
     * Minimum required number of symbols to detect an RX frame
     * Related MIB type: \ref MIB_MIN_RX_SYMBOLS
     */
    uint8_t min_rx_symbols;
} lora_mac_mib_param_t;

/** LoRaMAC MIB-RequestConfirm structure
 *
 */
typedef struct lora_mac_mib_request_confirm {
    /** MIB-Request type
     *
     */
    lora_mac_mib_t type;
    /** MIB-RequestConfirm parameters
     *
     */
    lora_mac_mib_param_t param;
} lora_mac_mib_request_confirm_t;

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

/** Structure containing the uplink status
 *
 */
typedef struct loramac_uplink_status {
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
typedef struct loramac_downlink_status {
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
} lora_events_t;

typedef struct  {
     // Mandatory. Event Callback must be provided
     mbed::Callback<void(lora_events_t)> events;

     // Rest are optional
     // If the user do not assign these callbacks, these callbacks would return
     // null if checked with bool operator
     // link_check_resp callback and other such callbacks will be maped in
     // future releases of Mbed-OS
     mbed::Callback<void(uint8_t, uint8_t)> link_check_resp;
 }lorawan_app_callbacks_t;

typedef struct lora_channelplan {
    uint8_t nb_channels;    // number of channels
    lora_channels_t *channels;
} lora_channelplan_t;

#endif /* LORAWAN_SYSTEM_LORAWAN_DATA_STRUCTURES_H_ */
