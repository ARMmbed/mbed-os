/**
 * \file      LoRaMac.h
 *
 * \brief     LoRa MAC layer implementation
 *
 * \copyright Revised BSD License, see LICENSE.TXT file include in the project
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013 Semtech
 *
 *               ___ _____ _   ___ _  _____ ___  ___  ___ ___
 *              / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 *              \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 *              |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 *              embedded.connectivity.solutions===============
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Daniel Jaeckle ( STACKFORCE )
 *
 * \defgroup  LORAMAC LoRa MAC layer implementation
 *            This module specifies the API implementation of the LoRaMAC layer.
 *            This is a placeholder for a detailed description of the LoRaMac
 *            layer and the supported features.
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __LORAMAC_H__
#define __LORAMAC_H__

#include "lorawan/system/LoRaWANTimer.h"
#include "netsocket/LoRaRadio.h"
#include "lorastack/phy/LoRaPHY.h"
#include "lorawan/system/lorawan_data_structures.h"
#include "LoRaMacCommand.h"
#include "events/EventQueue.h"


/*!
 * Maximum PHY layer payload size
 */
#define LORAMAC_PHY_MAXPAYLOAD                      255


class LoRaMac
{
public:
    /*!
     * \brief   Constructor
     */
    LoRaMac();

    /*!
     * \brief   Destructor
     */
    ~LoRaMac();

    /*!
     * \brief   LoRaMAC layer initialization
     *
     * \details In addition to the initialization of the LoRaMAC layer, this
     *          function initializes the callback primitives of the MCPS and
     *          MLME services. Every data field of \ref LoRaMacPrimitives_t must be
     *          set to a valid callback function.
     *
     * \param   primitives [in] - A pointer to the structure defining the LoRaMAC
     *                            event functions. Refer to \ref LoRaMacPrimitives_t.
     *
     * \param   callbacks [in] - A pointer to the structure defining the LoRaMAC
     *                        callback functions. Refer to \ref LoRaMacCallback_t.
     *
     * \param   phy [in]- A pointer to the selected PHY layer.
     *
     * \param   queue [in]- A pointer to the application provided EventQueue.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAMAC_STATUS_OK
     *          \ref LORAMAC_STATUS_PARAMETER_INVALID
     *          \ref LORAMAC_STATUS_REGION_NOT_SUPPORTED.
     */
    LoRaMacStatus_t LoRaMacInitialization(LoRaMacPrimitives_t *primitives,
                                          LoRaMacCallback_t *callbacks,
                                          LoRaPHY *phy,
                                          events::EventQueue *queue);

    /*!
     * \brief   Queries the LoRaMAC whether it is possible to send the next frame with
     *          a given payload size. The LoRaMAC takes the scheduled MAC commands into
     *          account and reports when the frame can be sent.
     *
     * \param   size [in]- The size of the applicable payload to be sent next.
     * \param   txInfo  [out] - The structure \ref LoRaMacTxInfo_t contains
     *                         information on the actual maximum payload possible
     *                         (according to the configured datarate or the next
     *                         datarate according to ADR), and the maximum frame
     *                         size, taking the scheduled MAC commands into account.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. When the parameters are
     *          not valid, the function returns \ref LORAMAC_STATUS_PARAMETER_INVALID.
     *          In case of a length error caused by the applicable payload in combination
     *          with the MAC commands, the function returns \ref LORAMAC_STATUS_LENGTH_ERROR.
     *          Please note that if the size of the MAC commands in the queue do
     *          not fit into the payload size on the related datarate, the LoRaMAC will
     *          omit the MAC commands.
     *          If the query is valid, and the LoRaMAC is able to send the frame,
     *          the function returns \ref LORAMAC_STATUS_OK.
     */
    LoRaMacStatus_t LoRaMacQueryTxPossible( uint8_t size, LoRaMacTxInfo_t* txInfo );

    /*!
     * \brief   LoRaMAC channel add service.
     *
     * \details Adds a new channel to the channel list and activates the ID in
     *          the channel mask. Please note that this functionality is not available
     *          in all regions. Information on the allowed ranges is available at the LoRaWAN Regional Parameters V1.0.2rB.
     *
     * \param   id [in] - The ID of the channel.
     *
     * \param   params [in] - The channel parameters to set.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAMAC_STATUS_OK
     *          \ref LORAMAC_STATUS_BUSY
     *          \ref LORAMAC_STATUS_PARAMETER_INVALID
     */
    LoRaMacStatus_t LoRaMacChannelAdd( uint8_t id, ChannelParams_t params );

    /*!
     * \brief   LoRaMAC channel remove service.
     *
     * \details Deactivates the ID in the channel mask.
     *
     * \param   id - Id of the channel.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAMAC_STATUS_OK
     *          \ref LORAMAC_STATUS_BUSY
     *          \ref LORAMAC_STATUS_PARAMETER_INVALID
     */
    LoRaMacStatus_t LoRaMacChannelRemove( uint8_t id );

    /*!
     * \brief   LoRaMAC multicast channel link service.
     *
     * \details Links a multicast channel into the linked list.
     *
     * \param   [in] channelParam - The multicast channel parameters to link.
     *
     * \retval  `LoRaMacStatus_t` The  status of the operation. The possible values are:
     *          \ref LORAMAC_STATUS_OK
     *          \ref LORAMAC_STATUS_BUSY
     *          \ref LORAMAC_STATUS_PARAMETER_INVALID
     */
    LoRaMacStatus_t LoRaMacMulticastChannelLink( MulticastParams_t *channelParam );

    /*!
     * \brief   LoRaMAC multicast channel unlink service.
     *
     * \details Unlinks a multicast channel from the linked list.
     *
     * \param   [in] channelParam - The multicast channel parameters to unlink.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAMAC_STATUS_OK
     *          \ref LORAMAC_STATUS_BUSY
     *          \ref LORAMAC_STATUS_PARAMETER_INVALID
     */
    LoRaMacStatus_t LoRaMacMulticastChannelUnlink( MulticastParams_t *channelParam );

    /*!
     * \brief   LoRaMAC MIB-GET.
     *
     * \details The MAC information base service to get the attributes of the LoRaMac layer.
     *
     *          The following code-snippet shows how to use the API to get the
     *          parameter `AdrEnable`, defined by the enumeration type
     *          \ref MIB_ADR.
     * \code
     * MibRequestConfirm_t mibReq;
     * mibReq.Type = MIB_ADR;
     *
     * if( LoRaMacMibGetRequestConfirm( &mibReq ) == LORAMAC_STATUS_OK )
     * {
     *   // LoRaMAC updated the parameter mibParam.AdrEnable
     * }
     * \endcode
     *
     * \param   [in] mibGet - The MIB-GET request to perform. Refer to \ref MibRequestConfirm_t.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAMAC_STATUS_OK
     *          \ref LORAMAC_STATUS_SERVICE_UNKNOWN
     *          \ref LORAMAC_STATUS_PARAMETER_INVALID
     */
    LoRaMacStatus_t LoRaMacMibGetRequestConfirm( MibRequestConfirm_t *mibGet );

    /*!
     * \brief   LoRaMAC MIB-SET.
     *
     * \details The MAC information base service to set the attributes of the LoRaMac layer.
     *
     *          The following code-snippet shows how to use the API to set the
     *          parameter `AdrEnable`, defined by the enumeration type
     *          \ref MIB_ADR.
     *
     * \code
     * MibRequestConfirm_t mibReq;
     * mibReq.Type = MIB_ADR;
     * mibReq.Param.AdrEnable = true;
     *
     * if( LoRaMacMibGetRequestConfirm( &mibReq ) == LORAMAC_STATUS_OK )
     * {
     *   // LoRaMAC updated the parameter
     * }
     * \endcode
     *
     * \param   [in] mibSet - The MIB-SET request to perform. Refer to \ref MibRequestConfirm_t.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAMAC_STATUS_OK
     *          \ref LORAMAC_STATUS_BUSY
     *          \ref LORAMAC_STATUS_SERVICE_UNKNOWN
     *          \ref LORAMAC_STATUS_PARAMETER_INVALID
     */
    LoRaMacStatus_t LoRaMacMibSetRequestConfirm( MibRequestConfirm_t *mibSet );

    /*!
     * \brief   LoRaMAC MLME request
     *
     * \details The MAC layer management entity handles the management services. The
     *          following code-snippet shows how to use the API to perform a
     *          network join request.
     *
     * \code
     * static uint8_t DevEui[] =
     * {
     *   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
     * };
     * static uint8_t AppEui[] =
     * {
     *   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
     * };
     * static uint8_t AppKey[] =
     * {
     *   0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
     *   0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
     * };
     *
     * MlmeReq_t mlmeReq;
     * mlmeReq.Type = MLME_JOIN;
     * mlmeReq.Req.Join.DevEui = DevEui;
     * mlmeReq.Req.Join.AppEui = AppEui;
     * mlmeReq.Req.Join.AppKey = AppKey;
     *
     * if( LoRaMacMlmeRequest( &mlmeReq ) == LORAMAC_STATUS_OK )
     * {
     *   // Service started successfully. Waiting for the Mlme-Confirm event
     * }
     * \endcode
     *
     * \param   [in] mlmeRequest - The MLME request to perform. Refer to \ref MlmeReq_t.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAMAC_STATUS_OK
     *          \ref LORAMAC_STATUS_BUSY
     *          \ref LORAMAC_STATUS_SERVICE_UNKNOWN
     *          \ref LORAMAC_STATUS_PARAMETER_INVALID
     *          \ref LORAMAC_STATUS_NO_NETWORK_JOINED
     *          \ref LORAMAC_STATUS_LENGTH_ERROR
     *          \ref LORAMAC_STATUS_DEVICE_OFF
     */
    LoRaMacStatus_t LoRaMacMlmeRequest( MlmeReq_t *mlmeRequest );

    /*!
     * \brief   LoRaMAC MCPS request
     *
     * \details The MAC Common Part Sublayer handles the data services. The following
     *          code-snippet shows how to use the API to send an unconfirmed
     *          LoRaMAC frame.
     *
     * \code
     * uint8_t myBuffer[] = { 1, 2, 3 };
     *
     * McpsReq_t mcpsReq;
     * mcpsReq.Type = MCPS_UNCONFIRMED;
     * mcpsReq.Req.Unconfirmed.fPort = 1;
     * mcpsReq.Req.Unconfirmed.fBuffer = myBuffer;
     * mcpsReq.Req.Unconfirmed.fBufferSize = sizeof( myBuffer );
     *
     * if( LoRaMacMcpsRequest( &mcpsReq ) == LORAMAC_STATUS_OK )
     * {
     *   // Service started successfully. Waiting for the MCPS-Confirm event
     * }
     * \endcode
     *
     * \param   [in] mcpsRequest - The MCPS request to perform. Refer to \ref McpsReq_t.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAMAC_STATUS_OK
     *          \ref LORAMAC_STATUS_BUSY
     *          \ref LORAMAC_STATUS_SERVICE_UNKNOWN
     *          \ref LORAMAC_STATUS_PARAMETER_INVALID
     *          \ref LORAMAC_STATUS_NO_NETWORK_JOINED
     *          \ref LORAMAC_STATUS_LENGTH_ERROR
     *          \ref LORAMAC_STATUS_DEVICE_OFF
     */
    LoRaMacStatus_t LoRaMacMcpsRequest( McpsReq_t *mcpsRequest );

    /**
     * \brief LoRaMAC layer provides its callback functions for
     *        PHY layer
     *
     * \return Pointer to callback functions for radio events
     */
    radio_events_t *GetPhyEventHandlers();

    /*!
     * \brief Configures the events to trigger an MLME-Indication with
     *        a MLME type of MLME_SCHEDULE_UPLINK.
     */
    void SetMlmeScheduleUplinkIndication( void );

#if defined(LORAWAN_COMPLIANCE_TEST)
public: // Test interface

    /**
     * \brief   LoRaMAC set tx timer.
     *
     * \details Sets up a timer for next transmission (application specific timers).
     *
     * \param   [in] NextTxTime - Periodic time for next uplink.

     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAMAC_STATUS_OK
     *          \ref LORAMAC_STATUS_PARAMETER_INVALID
     */
    LoRaMacStatus_t LoRaMacSetTxTimer( uint32_t NextTxTime );

    /**
     * \brief   LoRaMAC stop tx timer.
     *
     * \details Stops the next tx timer.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAMAC_STATUS_OK
     *          \ref LORAMAC_STATUS_PARAMETER_INVALID
     */
    LoRaMacStatus_t LoRaMacStopTxTimer( );

    /**
     * \brief   Enabled or disables the reception windows
     *
     * \details This is a test function. It shall be used for testing purposes only.
     *          Changing this attribute may lead to a non-conformance LoRaMac operation.
     *
     * \param   [in] enable - Enabled or disables the reception windows
     */
    void LoRaMacTestRxWindowsOn( bool enable );

    /**
     * \brief   Enables the MIC field test
     *
     * \details This is a test function. It shall be used for testing purposes only.
     *          Changing this attribute may lead to a non-conformance LoRaMac operation.
     *
     * \param   [in] txPacketCounter - Fixed Tx packet counter value
     */
    void LoRaMacTestSetMic( uint16_t txPacketCounter );

    /**
     * \brief   Enabled or disables the duty cycle
     *
     * \details This is a test function. It shall be used for testing purposes only.
     *          Changing this attribute may lead to a non-conformance LoRaMac operation.
     *
     * \param   [in] enable - Enabled or disables the duty cycle
     */
    void LoRaMacTestSetDutyCycleOn( bool enable );

    /**
     * \brief   Sets the channel index
     *
     * \details This is a test function. It shall be used for testing purposes only.
     *          Changing this attribute may lead to a non-conformance LoRaMac operation.
     *
     * \param   [in] channel - Channel index
     */
    void LoRaMacTestSetChannel( uint8_t channel );

private:
    /**
     * Timer to handle the application data transmission duty cycle
     */
    TimerEvent_t TxNextPacketTimer;
#endif

private:
    /*!
     * \brief Function to be executed on Radio Tx Done event
     */
    void OnRadioTxDone( void );

    /*!
     * \brief This function prepares the MAC to abort the execution of function
     *        OnRadioRxDone in case of a reception error.
     */
    void PrepareRxDoneAbort( void );

    /*!
     * \brief Function to be executed on Radio Rx Done event
     */
    void OnRadioRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

    /*!
     * \brief Function executed on Radio Tx Timeout event
     */
    void OnRadioTxTimeout( void );

    /*!
     * \brief Function executed on Radio Rx error event
     */
    void OnRadioRxError( void );

    /*!
     * \brief Function executed on Radio Rx Timeout event
     */
    void OnRadioRxTimeout( void );

    /*!
     * \brief Function executed on Resend Frame timer event.
     */
    void OnMacStateCheckTimerEvent( void );

    /*!
     * \brief Function executed on duty cycle delayed Tx  timer event
     */
    void OnTxDelayedTimerEvent( void );

    /**
     * \brief Function to be executed when next Tx is possible
     */
    void OnNextTx( void );

    /*!
     * \brief Function executed on first Rx window timer event
     */
    void OnRxWindow1TimerEvent( void );

    /*!
     * \brief Function executed on second Rx window timer event
     */
    void OnRxWindow2TimerEvent( void );

    /*!
     * \brief Function executed on AckTimeout timer event
     */
    void OnAckTimeoutTimerEvent( void );

    /*!
     * \brief Initializes and opens the reception window
     *
     * \param [IN] rxContinuous Set to true, if the RX is in continuous mode
     * \param [IN] maxRxWindow Maximum RX window timeout
     */
    void RxWindowSetup( bool rxContinuous, uint32_t maxRxWindow );

    /*!
     * \brief Validates if the payload fits into the frame, taking the datarate
     *        into account.
     *
     * \details Refer to chapter 4.3.2 of the LoRaWAN specification, v1.0
     *
     * \param lenN Length of the application payload. The length depends on the
     *             datarate and is region specific
     *
     * \param datarate Current datarate
     *
     * \param fOptsLen Length of the fOpts field
     *
     * \retval [false: payload does not fit into the frame, true: payload fits into
     *          the frame]
     */
    bool ValidatePayloadLength( uint8_t lenN, int8_t datarate, uint8_t fOptsLen );

    /*!
     * \brief LoRaMAC layer generic send frame
     *
     * \param [IN] macHdr      MAC header field
     * \param [IN] fPort       MAC payload port
     * \param [IN] fBuffer     MAC data buffer to be sent
     * \param [IN] fBufferSize MAC data buffer size
     * \retval status          Status of the operation.
     */
    LoRaMacStatus_t Send( LoRaMacHeader_t *macHdr, uint8_t fPort, void *fBuffer, uint16_t fBufferSize );

    /*!
     * \brief LoRaMAC layer frame buffer initialization
     *
     * \param [IN] macHdr      MAC header field
     * \param [IN] fCtrl       MAC frame control field
     * \param [IN] fOpts       MAC commands buffer
     * \param [IN] fPort       MAC payload port
     * \param [IN] fBuffer     MAC data buffer to be sent
     * \param [IN] fBufferSize MAC data buffer size
     * \retval status          Status of the operation.
     */
    LoRaMacStatus_t PrepareFrame( LoRaMacHeader_t *macHdr, LoRaMacFrameCtrl_t *fCtrl, uint8_t fPort, void *fBuffer, uint16_t fBufferSize );

    /*
     * \brief Schedules the frame according to the duty cycle
     *
     * \retval Status of the operation
     */
    LoRaMacStatus_t ScheduleTx( void );

    /*
     * \brief Calculates the back-off time for the band of a channel.
     *
     * \param [IN] channel     The last Tx channel index
     */
    void CalculateBackOff( uint8_t channel );

    /*!
     * \brief LoRaMAC layer prepared frame buffer transmission with channel specification
     *
     * \remark PrepareFrame must be called at least once before calling this
     *         function.
     *
     * \param [IN] channel     Channel to transmit on
     * \retval status          Status of the operation.
     */
    LoRaMacStatus_t SendFrameOnChannel( uint8_t channel );

    /*!
     * \brief Sets the radio in continuous transmission mode
     *
     * \remark Uses the radio parameters set on the previous transmission.
     *
     * \param [IN] timeout     Time in seconds while the radio is kept in continuous wave mode
     * \retval status          Status of the operation.
     */
    LoRaMacStatus_t SetTxContinuousWave( uint16_t timeout );

    /*!
     * \brief Sets the radio in continuous transmission mode
     *
     * \remark Uses the radio parameters set on the previous transmission.
     *
     * \param [IN] timeout     Time in seconds while the radio is kept in continuous wave mode
     * \param [IN] frequency   RF frequency to be set.
     * \param [IN] power       RF output power to be set.
     * \retval status          Status of the operation.
     */
    LoRaMacStatus_t SetTxContinuousWave1( uint16_t timeout, uint32_t frequency, uint8_t power );

    /*!
     * \brief Resets MAC specific parameters to default
     */
    void ResetMacParameters( void );

    /*!
     * \brief Resets MAC specific parameters to default
     *
     * \param [IN] fPort     The fPort
     *
     * \retval [false: fPort not allowed, true: fPort allowed]
     */
    bool IsFPortAllowed( uint8_t fPort );

    /*!
     * \brief Opens up a continuous RX 2 window. This is used for
     *        class c devices.
     */
    void OpenContinuousRx2Window( void );

    /**
     * Prototypes for ISR handlers
     */
    static void handle_cad_done(bool cad);
    static void handle_tx_done(void);
    static void handle_rx_done(uint8_t *payload, uint16_t size, int16_t rssi,
                                  int8_t snr);
    static void handle_rx_error(void);
    static void handle_rx_timeout(void);
    static void handle_tx_timeout(void);
    static void handle_fhss_change_channel(uint8_t cur_channel);
    static void handle_rx1_timer_event(void);
    static void handle_rx2_timer_event(void);
    static void handle_ack_timeout(void);
    static void handle_delayed_tx_timer_event(void);
    static void handle_mac_state_check_timer_event(void);
    static void handle_next_tx_timer_event(void);

private:
    /**
     * LoRa PHY layer object storage
     */
    LoRaPHY *lora_phy;

    LoRaMacCommand mac_commands;


    /**
     * Radio event callback handlers for MAC
     */
    radio_events_t RadioEvents;


    /*!
     * Device IEEE EUI
     */
    uint8_t *LoRaMacDevEui;

    /*!
     * Application IEEE EUI
     */
    uint8_t *LoRaMacAppEui;

    /*!
     * AES encryption/decryption cipher application key
     */
    uint8_t *LoRaMacAppKey;

    /*!
     * AES encryption/decryption cipher network session key
     */
    uint8_t LoRaMacNwkSKey[16];

    /*!
     * AES encryption/decryption cipher application session key
     */
    uint8_t LoRaMacAppSKey[16];

    /*!
     * Device nonce is a random value extracted by issuing a sequence of RSSI
     * measurements
     */
    uint16_t LoRaMacDevNonce;

    /*!
     * Network ID ( 3 bytes )
     */
    uint32_t LoRaMacNetID;

    /*!
     * Mote Address
     */
    uint32_t LoRaMacDevAddr;

    /*!
     * Multicast channels linked list
     */
    MulticastParams_t *MulticastChannels;

    /*!
     * Actual device class
     */
    DeviceClass_t LoRaMacDeviceClass;

    /*!
     * Indicates if the node is connected to a private or public network
     */
    bool PublicNetwork;

    /*!
     * Indicates if the node supports repeaters
     */
    bool RepeaterSupport;

    /*!
     * Buffer containing the data to be sent or received.
     */
    uint8_t LoRaMacBuffer[LORAMAC_PHY_MAXPAYLOAD];

    /*!
     * Length of packet in LoRaMacBuffer
     */
    uint16_t LoRaMacBufferPktLen;

    /*!
     * Length of the payload in LoRaMacBuffer
     */
    uint8_t LoRaMacTxPayloadLen;

    /*!
     * Buffer containing the upper layer data.
     */
    uint8_t LoRaMacRxPayload[LORAMAC_PHY_MAXPAYLOAD];

    /*!
     * LoRaMAC frame counter. Each time a packet is sent the counter is incremented.
     * Only the 16 LSB bits are sent
     */
    uint32_t UpLinkCounter;

    /*!
     * LoRaMAC frame counter. Each time a packet is received the counter is incremented.
     * Only the 16 LSB bits are received
     */
    uint32_t DownLinkCounter;

    /*!
     * IsPacketCounterFixed enables the MIC field tests by fixing the
     * UpLinkCounter value
     */
    bool IsUpLinkCounterFixed;

    /*!
     * Used for test purposes. Disables the opening of the reception windows.
     */
    bool IsRxWindowsEnabled;

    /*!
     * Indicates if the MAC layer has already joined a network.
     */
    bool IsLoRaMacNetworkJoined;

    /*!
     * Counts the number of missed ADR acknowledgements
     */
    uint32_t AdrAckCounter;

    /*!
     * If the node has sent a FRAME_TYPE_DATA_CONFIRMED_UP this variable indicates
     * if the nodes needs to manage the server acknowledgement.
     */
    bool NodeAckRequested;

    /*!
     * If the server has sent a FRAME_TYPE_DATA_CONFIRMED_DOWN this variable indicates
     * if the ACK bit must be set for the next transmission
     */
    bool SrvAckRequested;

    /*!
     * LoRaMac parameters
     */
    LoRaMacParams_t LoRaMacParams;

    /*!
     * LoRaMac default parameters
     */
    LoRaMacParams_t LoRaMacParamsDefaults;

    /*!
     * Uplink messages repetitions counter
     */
    uint8_t ChannelsNbRepCounter;

    /*!
     * Aggregated duty cycle management
     */
    TimerTime_t AggregatedLastTxDoneTime;
    TimerTime_t AggregatedTimeOff;

    /*!
     * Enables/Disables duty cycle management (Test only)
     */
    bool DutyCycleOn;

    /*!
     * Current channel index
     */
    uint8_t Channel;

    /*!
     * Current channel index
     */
    uint8_t LastTxChannel;

    /*!
     * Set to true, if the last uplink was a join request
     */
    bool LastTxIsJoinRequest;

    /*!
     * Stores the time at LoRaMac initialization.
     *
     * \remark Used for the BACKOFF_DC computation.
     */
    TimerTime_t LoRaMacInitializationTime;

    /*!
     * LoRaMac internal states
     */
    enum eLoRaMacState
    {
        LORAMAC_IDLE          = 0x00000000,
        LORAMAC_TX_RUNNING    = 0x00000001,
        LORAMAC_RX            = 0x00000002,
        LORAMAC_ACK_REQ       = 0x00000004,
        LORAMAC_ACK_RETRY     = 0x00000008,
        LORAMAC_TX_DELAYED    = 0x00000010,
        LORAMAC_TX_CONFIG     = 0x00000020,
        LORAMAC_RX_ABORT      = 0x00000040,
    };

    /*!
     * LoRaMac internal state
     */
    uint32_t LoRaMacState;

    /*!
     * LoRaMac timer used to check the LoRaMacState (runs every second)
     */
    TimerEvent_t MacStateCheckTimer;

    /*!
     * LoRaMac upper layer event functions
     */
    LoRaMacPrimitives_t *LoRaMacPrimitives;

    /*!
     * LoRaMac upper layer callback functions
     */
    LoRaMacCallback_t *LoRaMacCallbacks;

    /*!
     * LoRaMac duty cycle delayed Tx timer
     */
    TimerEvent_t TxDelayedTimer;

    /*!
     * LoRaMac reception windows timers
     */
    TimerEvent_t RxWindowTimer1;
    TimerEvent_t RxWindowTimer2;

    /*!
     * LoRaMac reception windows delay
     * \remark normal frame: RxWindowXDelay = ReceiveDelayX - RADIO_WAKEUP_TIME
     *         join frame  : RxWindowXDelay = JoinAcceptDelayX - RADIO_WAKEUP_TIME
     */
    uint32_t RxWindow1Delay;
    uint32_t RxWindow2Delay;

    /*!
     * LoRaMac Rx windows configuration
     */
    RxConfigParams_t RxWindow1Config;
    RxConfigParams_t RxWindow2Config;

    /*!
     * Acknowledge timeout timer. Used for packet retransmissions.
     */
    TimerEvent_t AckTimeoutTimer;

    /*!
     * Number of trials to get a frame acknowledged
     */
    uint8_t AckTimeoutRetries;

    /*!
     * Number of trials to get a frame acknowledged
     */
    uint8_t AckTimeoutRetriesCounter;

    /*!
     * Indicates if the AckTimeout timer has expired or not
     */
    bool AckTimeoutRetry;

    /*!
     * Last transmission time on air
     */
    TimerTime_t TxTimeOnAir;

    /*!
     * Number of trials for the Join Request
     */
    uint8_t JoinRequestTrials;

    /*!
     * Maximum number of trials for the Join Request
     */
    uint8_t MaxJoinRequestTrials;

    /*!
     * Structure to hold MCPS indication data.
     */
    McpsIndication_t McpsIndication;

    /*!
     * Structure to hold MCPS confirm data.
     */
    McpsConfirm_t McpsConfirm;

    /*!
     * Structure to hold MLME indication data.
     */
    MlmeIndication_t MlmeIndication;

    /*!
     * Structure to hold MLME confirm data.
     */
    MlmeConfirm_t MlmeConfirm;

    /*!
     * Holds the current rx window slot
     */
    LoRaMacRxSlot_t RxSlot;

    /*!
     * LoRaMac tx/rx operation state
     */
    LoRaMacFlags_t LoRaMacFlags;

};

#endif // __LORAMAC_H__
