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
#include "lorastack/mac/LoRaMacCommand.h"
#include "events/EventQueue.h"
#include "lorastack/mac/LoRaMacMlme.h"
#include "lorastack/mac/LoRaMacMcps.h"
#include "lorastack/mac/LoRaMacMib.h"
#include "lorastack/mac/LoRaMacChannelPlan.h"

class LoRaMac
{
public:
    /*!
     * \brief   Constructor
     */
    LoRaMac(LoRaWANTimeHandler &lora_time);

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
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     *          \ref LORAWAN_STATUS_REGION_NOT_SUPPORTED.
     */
    lorawan_status_t LoRaMacInitialization(loramac_primitives_t *primitives,
                                          LoRaPHY *phy,
                                          events::EventQueue *queue);

    /*!
     * \brief   Disconnect LoRaMac layer
     *
     * \details Cancels all outstanding requests and sets LoRaMac's
     *          internal state to idle.
     */
    void disconnect(void);

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
     *          not valid, the function returns \ref LORAWAN_STATUS_PARAMETER_INVALID.
     *          In case of a length error caused by the applicable payload in combination
     *          with the MAC commands, the function returns \ref LORAWAN_STATUS_LENGTH_ERROR.
     *          Please note that if the size of the MAC commands in the queue do
     *          not fit into the payload size on the related datarate, the LoRaMAC will
     *          omit the MAC commands.
     *          If the query is valid, and the LoRaMAC is able to send the frame,
     *          the function returns \ref LORAWAN_STATUS_OK.
     */
    lorawan_status_t LoRaMacQueryTxPossible( uint8_t size, loramac_tx_info_t* txInfo );

    /*!
     * \brief   Adds a channel plan to the system.
     *
     * \details Adds a whole channel plan or a single new channel to the.
     *          Please note that this functionality is not available in all regions.
     *          Information on the allowed ranges is available at the
     *          LoRaWAN Regional Parameters V1.0.2rB.
     *
     * \param   plan [in] - A reference to application provided channel plan.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t AddChannelPlan(const lorawan_channelplan_t& plan);

    /*!
     * \brief   Removes a channel plan from the system.
     *
     * \details Removes the whole active channel plan except the 'Default Channels'..
     *          Please note that this functionality is not available in all regions.
     *          Information on the allowed ranges is available at the
     *          LoRaWAN Regional Parameters V1.0.2rB.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t RemoveChannelPlan();

    /*!
     * \brief   Access active channel plan.
     *
     * \details Provides access to the current active channel plan.
     *
     * \param   plan [out] - A reference to application provided channel plan data
     *                       structure which will be filled in with active channel
     *                       plan.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t GetChannelPlan(lorawan_channelplan_t& plan);

    /*!
     * \brief   Remove a given channel from the active plan.
     *
     * \details Deactivates the given channel.
     *
     * \param   id - Id of the channel.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t RemoveSingleChannel( uint8_t id );

    /*!
     * \brief   LoRaMAC multicast channel link service.
     *
     * \details Links a multicast channel into the linked list.
     *
     * \param   [in] channelParam - The multicast channel parameters to link.
     *
     * \retval  `LoRaMacStatus_t` The  status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t LoRaMacMulticastChannelLink( multicast_params_t *channelParam );

    /*!
     * \brief   LoRaMAC multicast channel unlink service.
     *
     * \details Unlinks a multicast channel from the linked list.
     *
     * \param   [in] channelParam - The multicast channel parameters to unlink.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t LoRaMacMulticastChannelUnlink( multicast_params_t *channelParam );

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
     * if( LoRaMacMibGetRequestConfirm( &mibReq ) == LORAWAN_STATUS_OK )
     * {
     *   // LoRaMAC updated the parameter mibParam.AdrEnable
     * }
     * \endcode
     *
     * \param   [in] mibGet - The MIB-GET request to perform. Refer to \ref MibRequestConfirm_t.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_SERVICE_UNKNOWN
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t LoRaMacMibGetRequestConfirm( loramac_mib_req_confirm_t *mibGet );

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
     * if( LoRaMacMibGetRequestConfirm( &mibReq ) == LORAWAN_STATUS_OK )
     * {
     *   // LoRaMAC updated the parameter
     * }
     * \endcode
     *
     * \param   [in] mibSet - The MIB-SET request to perform. Refer to \ref MibRequestConfirm_t.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_SERVICE_UNKNOWN
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t LoRaMacMibSetRequestConfirm( loramac_mib_req_confirm_t *mibSet );

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
     * if( LoRaMacMlmeRequest( &mlmeReq ) == LORAWAN_STATUS_OK )
     * {
     *   // Service started successfully. Waiting for the Mlme-Confirm event
     * }
     * \endcode
     *
     * \param   [in] mlmeRequest - The MLME request to perform. Refer to \ref MlmeReq_t.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_SERVICE_UNKNOWN
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     *          \ref LORAWAN_STATUS_NO_NETWORK_JOINED
     *          \ref LORAWAN_STATUS_LENGTH_ERROR
     *          \ref LORAWAN_STATUS_DEVICE_OFF
     */
    lorawan_status_t LoRaMacMlmeRequest( loramac_mlme_req_t *mlmeRequest );

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
     * if( LoRaMacMcpsRequest( &mcpsReq ) == LORAWAN_STATUS_OK )
     * {
     *   // Service started successfully. Waiting for the MCPS-Confirm event
     * }
     * \endcode
     *
     * \param   [in] mcpsRequest - The MCPS request to perform. Refer to \ref McpsReq_t.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_SERVICE_UNKNOWN
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     *          \ref LORAWAN_STATUS_NO_NETWORK_JOINED
     *          \ref LORAWAN_STATUS_LENGTH_ERROR
     *          \ref LORAWAN_STATUS_DEVICE_OFF
     */
    lorawan_status_t LoRaMacMcpsRequest( loramac_mcps_req_t *mcpsRequest );

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

    /*!
     * \brief LoRaMAC layer generic send frame
     *
     * \param [IN] macHdr      MAC header field
     * \param [IN] fPort       MAC payload port
     * \param [IN] fBuffer     MAC data buffer to be sent
     * \param [IN] fBufferSize MAC data buffer size
     * \retval status          Status of the operation.
     */
    lorawan_status_t Send( loramac_mhdr_t *macHdr, uint8_t fPort, void *fBuffer, uint16_t fBufferSize );

    /*!
     * \brief Sets the radio in continuous transmission mode
     *
     * \remark Uses the radio parameters set on the previous transmission.
     *
     * \param [IN] timeout     Time in seconds while the radio is kept in continuous wave mode
     * \retval status          Status of the operation.
     */
    lorawan_status_t SetTxContinuousWave( uint16_t timeout );

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
    lorawan_status_t SetTxContinuousWave1( uint16_t timeout, uint32_t frequency, uint8_t power );

    /*!
     * \brief Resets MAC specific parameters to default
     */
    void ResetMacParameters( void );

    /*!
     * \brief Opens up a continuous RX 2 window. This is used for
     *        class c devices.
     */
    void OpenContinuousRx2Window(void);

#if defined(LORAWAN_COMPLIANCE_TEST)
public: // Test interface

    /**
     * \brief   LoRaMAC set tx timer.
     *
     * \details Sets up a timer for next transmission (application specific timers).
     *
     * \param   [in] NextTxTime - Periodic time for next uplink.

     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t LoRaMacSetTxTimer( uint32_t NextTxTime );

    /**
     * \brief   LoRaMAC stop tx timer.
     *
     * \details Stops the next tx timer.
     *
     * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t LoRaMacStopTxTimer( );

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
    timer_event_t tx_next_packet_timer;
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
    lorawan_status_t PrepareFrame( loramac_mhdr_t *macHdr, loramac_frame_ctrl_t *fCtrl, uint8_t fPort, void *fBuffer, uint16_t fBufferSize );

    /*
     * \brief Schedules the frame according to the duty cycle
     *
     * \retval Status of the operation
     */
    lorawan_status_t ScheduleTx( void );

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
    lorawan_status_t SendFrameOnChannel( uint8_t channel );

    /*!
     * \brief Resets MAC specific parameters to default
     *
     * \param [IN] fPort     The fPort
     *
     * \retval [false: fPort not allowed, true: fPort allowed]
     */
    bool IsFPortAllowed( uint8_t fPort );

    /**
     * Prototypes for ISR handlers
     */
    void handle_cad_done(bool cad);
    void handle_tx_done(void);
    void handle_rx_done(uint8_t *payload, uint16_t size, int16_t rssi,
                        int8_t snr);
    void handle_rx_error(void);
    void handle_rx_timeout(void);
    void handle_tx_timeout(void);
    void handle_fhss_change_channel(uint8_t cur_channel);
    void handle_rx1_timer_event(void);
    void handle_rx2_timer_event(void);
    void handle_ack_timeout(void);
    void handle_delayed_tx_timer_event(void);
    void handle_mac_state_check_timer_event(void);
    void handle_next_tx_timer_event(void);

private:
    /**
     * LoRa PHY layer object storage
     */
    LoRaPHY *lora_phy;

    /**
     * MAC command handle
     */
    LoRaMacCommand mac_commands;

    /**
     * MLME subsystem handle
     */
    LoRaMacMlme mlme;

    /**
     * MCPS subsystem handle
     */
    LoRaMacMcps mcps;

    /**
     * MCPS subsystem handle
     */
    LoRaMacMib mib;

    /**
     * Channel planning subsystem
     */
    LoRaMacChannelPlan ch_plan;

    /**
     * Timer subsystem handle
     */
    LoRaWANTimeHandler &_lora_time;

    /**
     * Central MAC layer data storage
     */
    loramac_protocol_params _params;

    /**
     * Radio event callback handlers for MAC
     */
    radio_events_t RadioEvents;

    /*!
     * LoRaMac upper layer event functions
     */
    loramac_primitives_t *LoRaMacPrimitives;
};

#endif // __LORAMAC_H__
