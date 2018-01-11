/**
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech
 ___ _____ _   ___ _  _____ ___  ___  ___ ___
/ __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
\__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
|___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
embedded.connectivity.solutions===============

Description: LoRa MAC layer implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )

Copyright (c) 2017, Arm Limited and affiliates.

SPDX-License-Identifier: BSD-3-Clause
*/
#include <stdlib.h>
#include "LoRaMac.h"
#include "LoRaMacCrypto.h"

#if defined(FEATURE_COMMON_PAL)
#include "mbed_trace.h"
#define TRACE_GROUP "LMAC"
#else
#define tr_debug(...) (void(0)) //dummies if feature common pal is not added
#define tr_info(...)  (void(0)) //dummies if feature common pal is not added
#define tr_error(...) (void(0)) //dummies if feature common pal is not added
#endif //defined(FEATURE_COMMON_PAL)

using namespace events;

/**
 * EventQueue object storage
 */
static EventQueue *ev_queue;

/*!
 * Maximum length of the fOpts field
 */
#define LORA_MAC_COMMAND_MAX_FOPTS_LENGTH           15

/*!
 * LoRaMac duty cycle for the back-off procedure during the first hour.
 */
#define BACKOFF_DC_1_HOUR                           100

/*!
 * LoRaMac duty cycle for the back-off procedure during the next 10 hours.
 */
#define BACKOFF_DC_10_HOURS                         1000

/*!
 * LoRaMac duty cycle for the back-off procedure during the next 24 hours.
 */
#define BACKOFF_DC_24_HOURS                         10000

/*!
 * Check the MAC layer state every MAC_STATE_CHECK_TIMEOUT in ms.
 */
#define MAC_STATE_CHECK_TIMEOUT                     1000

/*!
 * The maximum number of times the MAC layer tries to get an acknowledge.
 */
#define MAX_ACK_RETRIES                             8

/*!
 * The frame direction definition for uplink communications.
 */
#define UP_LINK                                     0

/*!
 * The frame direction definition for downlink communications.
 */
#define DOWN_LINK                                   1


LoRaMac::LoRaMac(LoRaWANTimeHandler &lora_time)
    : mac_commands(*this), _lora_time(lora_time)
{
    lora_phy = NULL;
    //radio_events_t RadioEvents;
    _params.keys.LoRaMacDevEui = NULL;
    _params.keys.LoRaMacAppEui = NULL;
    _params.keys.LoRaMacAppKey = NULL;

    memset(_params.keys.LoRaMacNwkSKey, 0, sizeof(_params.keys.LoRaMacNwkSKey));
    memset(_params.keys.LoRaMacAppSKey, 0, sizeof(_params.keys.LoRaMacAppSKey));

    _params.LoRaMacDevNonce = 0;
    _params.LoRaMacNetID = 0;
    _params.LoRaMacDevAddr = 0;
    _params.LoRaMacBufferPktLen = 0;
    _params.LoRaMacTxPayloadLen = 0;
    _params.UpLinkCounter = 0;
    _params.DownLinkCounter = 0;
    _params.IsUpLinkCounterFixed = false;
    _params.IsRxWindowsEnabled = true;
    _params.IsLoRaMacNetworkJoined = false;
    _params.AdrAckCounter = 0;
    _params.NodeAckRequested = false;
    _params.SrvAckRequested = false;
    _params.ChannelsNbRepCounter = 0;
    _params.timers.LoRaMacInitializationTime = 0;
    _params.LoRaMacState = LORAMAC_IDLE;
    _params.AckTimeoutRetries = 1;
    _params.AckTimeoutRetriesCounter = 1;
    _params.AckTimeoutRetry = false;
    _params.timers.TxTimeOnAir = 0;

    _params.MulticastChannels = NULL;

    _params.sys_params.AdrCtrlOn = false;
    _params.sys_params.MaxDCycle = 0;
}

LoRaMac::~LoRaMac()
{
}


/***************************************************************************
 * ISRs - Handlers                                                         *
 **************************************************************************/
void LoRaMac::handle_tx_done(void)
{
    ev_queue->call(this, &LoRaMac::OnRadioTxDone);
}

void LoRaMac::handle_rx_done(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
    ev_queue->call(this, &LoRaMac::OnRadioRxDone, payload, size, rssi, snr);
}

void LoRaMac::handle_rx_error(void)
{
    ev_queue->call(this, &LoRaMac::OnRadioRxError);
}

void LoRaMac::handle_rx_timeout(void)
{
    ev_queue->call(this, &LoRaMac::OnRadioRxTimeout);
}

void LoRaMac::handle_tx_timeout(void)
{
    ev_queue->call(this, &LoRaMac::OnRadioTxTimeout);
}

void LoRaMac::handle_cad_done(bool cad)
{
    //TODO Not implemented yet
    //ev_queue->call(this, &LoRaMac::OnRadioCadDone, cad);
}

void LoRaMac::handle_fhss_change_channel(uint8_t cur_channel)
{
    // TODO Not implemented yet
    //ev_queue->call(this, &LoRaMac::OnRadioFHSSChangeChannel, cur_channel);
}

void LoRaMac::handle_mac_state_check_timer_event(void)
{
    ev_queue->call(this, &LoRaMac::OnMacStateCheckTimerEvent);
}

void LoRaMac::handle_delayed_tx_timer_event(void)
{
    ev_queue->call(this, &LoRaMac::OnTxDelayedTimerEvent);
}

void LoRaMac::handle_ack_timeout()
{
    ev_queue->call(this, &LoRaMac::OnAckTimeoutTimerEvent);
}

void LoRaMac::handle_rx1_timer_event(void)
{
    ev_queue->call(this, &LoRaMac::OnRxWindow1TimerEvent);
}

void LoRaMac::handle_rx2_timer_event(void)
{
    ev_queue->call(this, &LoRaMac::OnRxWindow2TimerEvent);
}

/***************************************************************************
 * Radio event callbacks - delegated to Radio driver                       *
 **************************************************************************/
void LoRaMac::OnRadioTxDone( void )
{
    GetPhyParams_t getPhy;
    PhyParam_t phyParam;
    SetBandTxDoneParams_t txDone;
    TimerTime_t curTime = _lora_time.TimerGetCurrentTime( );
    MlmeConfirm_t mlme_confirm = mlme.get_confirmation();

    if( _params.LoRaMacDeviceClass != CLASS_C )
    {
        lora_phy->put_radio_to_sleep();
    }
    else
    {
        OpenContinuousRx2Window( );
    }

    // Setup timers
    if( _params.IsRxWindowsEnabled == true )
    {
        _lora_time.TimerSetValue( &_params.timers.RxWindowTimer1, _params.RxWindow1Delay );
        _lora_time.TimerStart( &_params.timers.RxWindowTimer1 );
        if( _params.LoRaMacDeviceClass != CLASS_C )
        {
            _lora_time.TimerSetValue( &_params.timers.RxWindowTimer2, _params.RxWindow2Delay );
            _lora_time.TimerStart( &_params.timers.RxWindowTimer2 );
        }
        if( ( _params.LoRaMacDeviceClass == CLASS_C ) || ( _params.NodeAckRequested == true ) )
        {
            getPhy.Attribute = PHY_ACK_TIMEOUT;
            phyParam = lora_phy->get_phy_params(&getPhy);
            _lora_time.TimerSetValue( &_params.timers.AckTimeoutTimer, _params.RxWindow2Delay + phyParam.Value );
            _lora_time.TimerStart( &_params.timers.AckTimeoutTimer );
        }
    }
    else
    {
        mcps.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_OK;
        mlme_confirm.Status = LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT;

        if( _params.LoRaMacFlags.Value == 0 )
        {
            _params.LoRaMacFlags.Bits.McpsReq = 1;
        }
        _params.LoRaMacFlags.Bits.MacDone = 1;
    }

    // Verify if the last uplink was a join request
    if( ( _params.LoRaMacFlags.Bits.MlmeReq == 1 ) && ( mlme_confirm.MlmeRequest == MLME_JOIN ) )
    {
        _params.LastTxIsJoinRequest = true;
    }
    else
    {
        _params.LastTxIsJoinRequest = false;
    }

    // Store last Tx channel
    _params.LastTxChannel = _params.Channel;
    // Update last tx done time for the current channel
    txDone.Channel = _params.Channel;
    txDone.Joined = _params.IsLoRaMacNetworkJoined;
    txDone.LastTxDoneTime = curTime;
    lora_phy->set_band_tx_done(&txDone);
    // Update Aggregated last tx done time
    _params.timers.AggregatedLastTxDoneTime = curTime;

    if( _params.NodeAckRequested == false )
    {
        mcps.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_OK;
        _params.ChannelsNbRepCounter++;
    }
}

void LoRaMac::PrepareRxDoneAbort( void )
{
    _params.LoRaMacState |= LORAMAC_RX_ABORT;

    if( _params.NodeAckRequested )
    {
        handle_ack_timeout();
    }

    _params.LoRaMacFlags.Bits.McpsInd = 1;
    _params.LoRaMacFlags.Bits.MacDone = 1;

    // Trig OnMacCheckTimerEvent call as soon as possible
    _lora_time.TimerSetValue( &_params.timers.MacStateCheckTimer, 1 );
    _lora_time.TimerStart( &_params.timers.MacStateCheckTimer );
}

void LoRaMac::OnRadioRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    LoRaMacHeader_t macHdr;
    LoRaMacFrameCtrl_t fCtrl;
    ApplyCFListParams_t applyCFList;
    GetPhyParams_t getPhy;
    PhyParam_t phyParam;
    bool skipIndication = false;

    uint8_t pktHeaderLen = 0;
    uint32_t address = 0;
    uint8_t appPayloadStartIndex = 0;
    uint8_t port = 0xFF;
    uint8_t frameLen = 0;
    uint32_t mic = 0;
    uint32_t micRx = 0;

    uint16_t sequenceCounter = 0;
    uint16_t sequenceCounterPrev = 0;
    uint16_t sequenceCounterDiff = 0;
    uint32_t downLinkCounter = 0;

    MulticastParams_t *curMulticastParams = NULL;
    uint8_t *nwkSKey = _params.keys.LoRaMacNwkSKey;
    uint8_t *appSKey = _params.keys.LoRaMacAppSKey;

    uint8_t multicast = 0;

    bool isMicOk = false;

    mcps.get_confirmation().AckReceived = false;
    mcps.get_indication().Rssi = rssi;
    mcps.get_indication().Snr = snr;
    mcps.get_indication().RxSlot = _params.RxSlot;
    mcps.get_indication().Port = 0;
    mcps.get_indication().Multicast = 0;
    mcps.get_indication().FramePending = 0;
    mcps.get_indication().Buffer = NULL;
    mcps.get_indication().BufferSize = 0;
    mcps.get_indication().RxData = false;
    mcps.get_indication().AckReceived = false;
    mcps.get_indication().DownLinkCounter = 0;
    mcps.get_indication().McpsIndication = MCPS_UNCONFIRMED;

    lora_phy->put_radio_to_sleep();

    _lora_time.TimerStop( &_params.timers.RxWindowTimer2 );

    macHdr.Value = payload[pktHeaderLen++];

    switch( macHdr.Bits.MType )
    {
        case FRAME_TYPE_JOIN_ACCEPT:
            if( _params.IsLoRaMacNetworkJoined == true )
            {
                mcps.get_indication().Status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                PrepareRxDoneAbort( );
                return;
            }

            if (0 != LoRaMacJoinDecrypt( payload + 1, size - 1,
                                         _params.keys.LoRaMacAppKey,
                                         _params.LoRaMacRxPayload + 1 )) {
                mcps.get_indication().Status = LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
                return;
            }

            _params.LoRaMacRxPayload[0] = macHdr.Value;

            if (0 != LoRaMacJoinComputeMic( _params.LoRaMacRxPayload,
                                            size - LORAMAC_MFR_LEN,
                                            _params.keys.LoRaMacAppKey,
                                            &mic )) {
                mcps.get_indication().Status = LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
                return;
            }

            micRx |= ( uint32_t ) _params.LoRaMacRxPayload[size - LORAMAC_MFR_LEN];
            micRx |= ( ( uint32_t ) _params.LoRaMacRxPayload[size - LORAMAC_MFR_LEN + 1] << 8 );
            micRx |= ( ( uint32_t ) _params.LoRaMacRxPayload[size - LORAMAC_MFR_LEN + 2] << 16 );
            micRx |= ( ( uint32_t ) _params.LoRaMacRxPayload[size - LORAMAC_MFR_LEN + 3] << 24 );

            if( micRx == mic )
            {
                if (0 != LoRaMacJoinComputeSKeys( _params.keys.LoRaMacAppKey,
                                                  _params.LoRaMacRxPayload + 1,
                                                  _params.LoRaMacDevNonce,
                                                  _params.keys.LoRaMacNwkSKey,
                                                  _params.keys.LoRaMacAppSKey )) {
                    mcps.get_indication().Status = LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
                    return;
                }

                _params.LoRaMacNetID = ( uint32_t ) _params.LoRaMacRxPayload[4];
                _params.LoRaMacNetID |= ( ( uint32_t ) _params.LoRaMacRxPayload[5] << 8 );
                _params.LoRaMacNetID |= ( ( uint32_t ) _params.LoRaMacRxPayload[6] << 16 );

                _params.LoRaMacDevAddr = ( uint32_t ) _params.LoRaMacRxPayload[7];
                _params.LoRaMacDevAddr |= ( ( uint32_t ) _params.LoRaMacRxPayload[8] << 8 );
                _params.LoRaMacDevAddr |= ( ( uint32_t ) _params.LoRaMacRxPayload[9] << 16 );
                _params.LoRaMacDevAddr |= ( ( uint32_t ) _params.LoRaMacRxPayload[10] << 24 );

                // DLSettings
                _params.sys_params.Rx1DrOffset = ( _params.LoRaMacRxPayload[11] >> 4 ) & 0x07;
                _params.sys_params.Rx2Channel.Datarate = _params.LoRaMacRxPayload[11] & 0x0F;

                // RxDelay
                _params.sys_params.ReceiveDelay1 = ( _params.LoRaMacRxPayload[12] & 0x0F );
                if( _params.sys_params.ReceiveDelay1 == 0 )
                {
                    _params.sys_params.ReceiveDelay1 = 1;
                }
                _params.sys_params.ReceiveDelay1 *= 1000;
                _params.sys_params.ReceiveDelay2 = _params.sys_params.ReceiveDelay1 + 1000;

                // Apply CF list
                applyCFList.Payload = &_params.LoRaMacRxPayload[13];
                // Size of the regular payload is 12. Plus 1 byte MHDR and 4 bytes MIC
                applyCFList.Size = size - 17;

                lora_phy->apply_cf_list(&applyCFList);

                mlme.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_OK;
                _params.IsLoRaMacNetworkJoined = true;
            }
            else
            {
                mlme.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_JOIN_FAIL;
            }
            break;
        case FRAME_TYPE_DATA_CONFIRMED_DOWN:
        case FRAME_TYPE_DATA_UNCONFIRMED_DOWN:
            {
                // Check if the received payload size is valid
                getPhy.UplinkDwellTime = _params.sys_params.DownlinkDwellTime;
                getPhy.Datarate = mcps.get_indication().RxDatarate;
                getPhy.Attribute = PHY_MAX_PAYLOAD;

                // Get the maximum payload length
                if( _params.RepeaterSupport == true )
                {
                    getPhy.Attribute = PHY_MAX_PAYLOAD_REPEATER;
                }
                phyParam = lora_phy->get_phy_params(&getPhy);
                if( MAX( 0, ( int16_t )( ( int16_t )size - ( int16_t )LORA_MAC_FRMPAYLOAD_OVERHEAD ) ) > (int32_t)phyParam.Value )
                {
                    mcps.get_indication().Status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                    PrepareRxDoneAbort( );
                    return;
                }

                address = payload[pktHeaderLen++];
                address |= ( (uint32_t)payload[pktHeaderLen++] << 8 );
                address |= ( (uint32_t)payload[pktHeaderLen++] << 16 );
                address |= ( (uint32_t)payload[pktHeaderLen++] << 24 );

                if( address != _params.LoRaMacDevAddr )
                {
                    curMulticastParams = _params.MulticastChannels;
                    while( curMulticastParams != NULL )
                    {
                        if( address == curMulticastParams->Address )
                        {
                            multicast = 1;
                            nwkSKey = curMulticastParams->NwkSKey;
                            appSKey = curMulticastParams->AppSKey;
                            downLinkCounter = curMulticastParams->DownLinkCounter;
                            break;
                        }
                        curMulticastParams = curMulticastParams->Next;
                    }
                    if( multicast == 0 )
                    {
                        // We are not the destination of this frame.
                        mcps.get_indication().Status = LORAMAC_EVENT_INFO_STATUS_ADDRESS_FAIL;
                        PrepareRxDoneAbort( );
                        return;
                    }
                }
                else
                {
                    multicast = 0;
                    nwkSKey = _params.keys.LoRaMacNwkSKey;
                    appSKey = _params.keys.LoRaMacAppSKey;
                    downLinkCounter = _params.DownLinkCounter;
                }

                fCtrl.Value = payload[pktHeaderLen++];

                sequenceCounter = ( uint16_t )payload[pktHeaderLen++];
                sequenceCounter |= ( uint16_t )payload[pktHeaderLen++] << 8;

                appPayloadStartIndex = 8 + fCtrl.Bits.FOptsLen;

                micRx |= ( uint32_t )payload[size - LORAMAC_MFR_LEN];
                micRx |= ( ( uint32_t )payload[size - LORAMAC_MFR_LEN + 1] << 8 );
                micRx |= ( ( uint32_t )payload[size - LORAMAC_MFR_LEN + 2] << 16 );
                micRx |= ( ( uint32_t )payload[size - LORAMAC_MFR_LEN + 3] << 24 );

                sequenceCounterPrev = ( uint16_t )downLinkCounter;
                sequenceCounterDiff = ( sequenceCounter - sequenceCounterPrev );

                if( sequenceCounterDiff < ( 1 << 15 ) )
                {
                    downLinkCounter += sequenceCounterDiff;
                    LoRaMacComputeMic( payload, size - LORAMAC_MFR_LEN, nwkSKey, address, DOWN_LINK, downLinkCounter, &mic );
                    if( micRx == mic )
                    {
                        isMicOk = true;
                    }
                }
                else
                {
                    // check for sequence roll-over
                    uint32_t  downLinkCounterTmp = downLinkCounter + 0x10000 + ( int16_t )sequenceCounterDiff;
                    LoRaMacComputeMic( payload, size - LORAMAC_MFR_LEN, nwkSKey, address, DOWN_LINK, downLinkCounterTmp, &mic );
                    if( micRx == mic )
                    {
                        isMicOk = true;
                        downLinkCounter = downLinkCounterTmp;
                    }
                }

                // Check for a the maximum allowed counter difference
                getPhy.Attribute = PHY_MAX_FCNT_GAP;
                phyParam = lora_phy->get_phy_params( &getPhy );
                if( sequenceCounterDiff >= phyParam.Value )
                {
                    mcps.get_indication().Status = LORAMAC_EVENT_INFO_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOSS;
                    mcps.get_indication().DownLinkCounter = downLinkCounter;
                    PrepareRxDoneAbort( );
                    return;
                }

                if( isMicOk == true )
                {
                    mcps.get_indication().Status = LORAMAC_EVENT_INFO_STATUS_OK;
                    mcps.get_indication().Multicast = multicast;
                    mcps.get_indication().FramePending = fCtrl.Bits.FPending;
                    mcps.get_indication().Buffer = NULL;
                    mcps.get_indication().BufferSize = 0;
                    mcps.get_indication().DownLinkCounter = downLinkCounter;

                    mcps.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_OK;

                    _params.AdrAckCounter = 0;
                    mac_commands.ClearRepeatBuffer();

                    // Update 32 bits downlink counter
                    if( multicast == 1 )
                    {
                        mcps.get_indication().McpsIndication = MCPS_MULTICAST;

                        if( ( curMulticastParams->DownLinkCounter == downLinkCounter ) &&
                            ( curMulticastParams->DownLinkCounter != 0 ) )
                        {
                            mcps.get_indication().Status = LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED;
                            mcps.get_indication().DownLinkCounter = downLinkCounter;
                            PrepareRxDoneAbort( );
                            return;
                        }
                        curMulticastParams->DownLinkCounter = downLinkCounter;
                    }
                    else
                    {
                        if( macHdr.Bits.MType == FRAME_TYPE_DATA_CONFIRMED_DOWN )
                        {
                            _params.SrvAckRequested = true;
                            mcps.get_indication().McpsIndication = MCPS_CONFIRMED;

                            if( ( _params.DownLinkCounter == downLinkCounter ) &&
                                ( _params.DownLinkCounter != 0 ) )
                            {
                                // Duplicated confirmed downlink. Skip indication.
                                // In this case, the MAC layer shall accept the MAC commands
                                // which are included in the downlink retransmission.
                                // It should not provide the same frame to the application
                                // layer again.
                                skipIndication = true;
                            }
                        }
                        else
                        {
                            _params.SrvAckRequested = false;
                            mcps.get_indication().McpsIndication = MCPS_UNCONFIRMED;

                            if( ( _params.DownLinkCounter == downLinkCounter ) &&
                                ( _params.DownLinkCounter != 0 ) )
                            {
                                mcps.get_indication().Status = LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED;
                                mcps.get_indication().DownLinkCounter = downLinkCounter;
                                PrepareRxDoneAbort( );
                                return;
                            }
                        }
                        _params.DownLinkCounter = downLinkCounter;
                    }

                    // This must be done before parsing the payload and the MAC commands.
                    // We need to reset the MacCommandsBufferIndex here, since we need
                    // to take retransmissions and repetitions into account. Error cases
                    // will be handled in function OnMacStateCheckTimerEvent.
                    if( mcps.get_confirmation().McpsRequest == MCPS_CONFIRMED )
                    {
                        if( fCtrl.Bits.Ack == 1 )
                        {// Reset MacCommandsBufferIndex when we have received an ACK.
                            mac_commands.ClearCommandBuffer();
                        }
                    }
                    else
                    {// Reset the variable if we have received any valid frame.
                        mac_commands.ClearCommandBuffer();
                    }

                    // Process payload and MAC commands
                    if( ( ( size - 4 ) - appPayloadStartIndex ) > 0 )
                    {
                        port = payload[appPayloadStartIndex++];
                        frameLen = ( size - 4 ) - appPayloadStartIndex;

                        mcps.get_indication().Port = port;

                        if( port == 0 )
                        {
                            // Only allow frames which do not have fOpts
                            if( fCtrl.Bits.FOptsLen == 0 )
                            {
                                if (0 != LoRaMacPayloadDecrypt( payload + appPayloadStartIndex,
                                                                frameLen,
                                                                nwkSKey,
                                                                address,
                                                                DOWN_LINK,
                                                                downLinkCounter,
                                                                _params.LoRaMacRxPayload )) {
                                    mcps.get_indication().Status =  LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
                                }

                                // Decode frame payload MAC commands
                                mac_commands.ProcessMacCommands( _params.LoRaMacRxPayload, 0, frameLen, snr,
                                                                 mlme.get_confirmation(), LoRaMacCallbacks,
                                                                 _params.sys_params, *lora_phy );
                            }
                            else
                            {
                                skipIndication = true;
                            }
                        }
                        else
                        {
                            if( fCtrl.Bits.FOptsLen > 0 )
                            {
                                // Decode Options field MAC commands. Omit the fPort.
                                mac_commands.ProcessMacCommands( payload, 8, appPayloadStartIndex - 1, snr,
                                                                 mlme.get_confirmation(), LoRaMacCallbacks,
                                                                 _params.sys_params, *lora_phy );
                            }

                            if (0 != LoRaMacPayloadDecrypt( payload + appPayloadStartIndex,
                                                            frameLen,
                                                            appSKey,
                                                            address,
                                                            DOWN_LINK,
                                                            downLinkCounter,
                                                            _params.LoRaMacRxPayload )) {
                                mcps.get_indication().Status =  LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
                            }

                            if( skipIndication == false )
                            {
                                mcps.get_indication().Buffer = _params.LoRaMacRxPayload;
                                mcps.get_indication().BufferSize = frameLen;
                                mcps.get_indication().RxData = true;
                            }
                        }
                    }
                    else
                    {
                        if( fCtrl.Bits.FOptsLen > 0 )
                        {
                            // Decode Options field MAC commands
                            mac_commands.ProcessMacCommands( payload, 8, appPayloadStartIndex, snr,
                                                             mlme.get_confirmation(), LoRaMacCallbacks,
                                                             _params.sys_params, *lora_phy );
                        }
                    }

                    if( skipIndication == false )
                    {
                        // Check if the frame is an acknowledgement
                        if( fCtrl.Bits.Ack == 1 )
                        {
                            mcps.get_confirmation().AckReceived = true;
                            mcps.get_indication().AckReceived = true;

                            // Stop the AckTimeout timer as no more retransmissions
                            // are needed.
                            _lora_time.TimerStop( &_params.timers.AckTimeoutTimer );
                        }
                        else
                        {
                            mcps.get_confirmation().AckReceived = false;

                            if( _params.AckTimeoutRetriesCounter > _params.AckTimeoutRetries )
                            {
                                // Stop the AckTimeout timer as no more retransmissions
                                // are needed.
                                _lora_time.TimerStop( &_params.timers.AckTimeoutTimer );
                            }
                        }
                    }
                    // Provide always an indication, skip the callback to the user application,
                    // in case of a confirmed downlink retransmission.
                    _params.LoRaMacFlags.Bits.McpsInd = 1;
                    _params.LoRaMacFlags.Bits.McpsIndSkip = skipIndication;
                }
                else
                {
                    mcps.get_indication().Status = LORAMAC_EVENT_INFO_STATUS_MIC_FAIL;

                    PrepareRxDoneAbort( );
                    return;
                }
            }
            break;
        case FRAME_TYPE_PROPRIETARY:
            {
                memcpy( _params.LoRaMacRxPayload, &payload[pktHeaderLen], size );

                mcps.get_indication().McpsIndication = MCPS_PROPRIETARY;
                mcps.get_indication().Status = LORAMAC_EVENT_INFO_STATUS_OK;
                mcps.get_indication().Buffer = _params.LoRaMacRxPayload;
                mcps.get_indication().BufferSize = size - pktHeaderLen;

                _params.LoRaMacFlags.Bits.McpsInd = 1;
                break;
            }
        default:
            mcps.get_indication().Status = LORAMAC_EVENT_INFO_STATUS_ERROR;
            PrepareRxDoneAbort( );
            break;
    }
    _params.LoRaMacFlags.Bits.MacDone = 1;

    // Trig OnMacCheckTimerEvent call as soon as possible
    _lora_time.TimerSetValue( &_params.timers.MacStateCheckTimer, 1 );
    _lora_time.TimerStart( &_params.timers.MacStateCheckTimer );
}

void LoRaMac::OnRadioTxTimeout( void )
{
    if( _params.LoRaMacDeviceClass != CLASS_C )
    {
        lora_phy->put_radio_to_sleep();
    }
    else
    {
        OpenContinuousRx2Window( );
    }

    mcps.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT;

    mlme.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT;

    _params.LoRaMacFlags.Bits.MacDone = 1;
}

void LoRaMac::OnRadioRxError( void )
{
    if( _params.LoRaMacDeviceClass != CLASS_C )
    {
        lora_phy->put_radio_to_sleep();
    }
    else
    {
        OpenContinuousRx2Window( );
    }

    if( _params.RxSlot == RX_SLOT_WIN_1 )
    {
        if( _params.NodeAckRequested == true )
        {
            mcps.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_RX1_ERROR;
        }

        mlme.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_RX1_ERROR;

        if( _lora_time.TimerGetElapsedTime( _params.timers.AggregatedLastTxDoneTime ) >= _params.RxWindow2Delay )
        {
            _lora_time.TimerStop( &_params.timers.RxWindowTimer2 );
            _params.LoRaMacFlags.Bits.MacDone = 1;
        }
    }
    else
    {
        if( _params.NodeAckRequested == true )
        {
            mcps.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_RX2_ERROR;
        }

        mlme.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_RX2_ERROR;

        _params.LoRaMacFlags.Bits.MacDone = 1;
    }
}

void LoRaMac::OnRadioRxTimeout( void )
{
    if( _params.LoRaMacDeviceClass != CLASS_C )
    {
        lora_phy->put_radio_to_sleep();
    }
    else
    {
        OpenContinuousRx2Window( );
    }

    if( _params.RxSlot == RX_SLOT_WIN_1 )
    {
        if( _params.NodeAckRequested == true )
        {
            mcps.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_RX1_TIMEOUT;
        }
        mlme.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_RX1_TIMEOUT;

        if( _lora_time.TimerGetElapsedTime( _params.timers.AggregatedLastTxDoneTime ) >= _params.RxWindow2Delay )
        {
            _lora_time.TimerStop( &_params.timers.RxWindowTimer2 );
            _params.LoRaMacFlags.Bits.MacDone = 1;
        }
    }
    else
    {
        if( _params.NodeAckRequested == true )
        {
            mcps.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT;
        }

        mlme.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT;

        if( _params.LoRaMacDeviceClass != CLASS_C )
        {
            _params.LoRaMacFlags.Bits.MacDone = 1;
        }
    }
}

/***************************************************************************
 * Timer event callbacks - deliberated locally                             *
 **************************************************************************/
void LoRaMac::OnMacStateCheckTimerEvent( void )
{
    GetPhyParams_t getPhy;
    PhyParam_t phyParam;
    bool txTimeout = false;

    _lora_time.TimerStop( &_params.timers.MacStateCheckTimer );

    if( _params.LoRaMacFlags.Bits.MacDone == 1 )
    {
        if( ( _params.LoRaMacState & LORAMAC_RX_ABORT ) == LORAMAC_RX_ABORT )
        {
            _params.LoRaMacState &= ~LORAMAC_RX_ABORT;
            _params.LoRaMacState &= ~LORAMAC_TX_RUNNING;
        }

        if( ( _params.LoRaMacFlags.Bits.MlmeReq == 1 ) || ( ( _params.LoRaMacFlags.Bits.McpsReq == 1 ) ) )
        {
            if( ( mcps.get_confirmation().Status == LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT ) ||
                ( mlme.get_confirmation().Status == LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT ) )
            {
                // Stop transmit cycle due to tx timeout.
                _params.LoRaMacState &= ~LORAMAC_TX_RUNNING;
                mac_commands.ClearCommandBuffer();
                mcps.get_confirmation().NbRetries = _params.AckTimeoutRetriesCounter;
                mcps.get_confirmation().AckReceived = false;
                mcps.get_confirmation().TxTimeOnAir = 0;
                txTimeout = true;
            }
        }

        if( ( _params.NodeAckRequested == false ) && ( txTimeout == false ) )
        {
            if( ( _params.LoRaMacFlags.Bits.MlmeReq == 1 ) || ( ( _params.LoRaMacFlags.Bits.McpsReq == 1 ) ) )
            {
                if( ( _params.LoRaMacFlags.Bits.MlmeReq == 1 ) && ( mlme.get_confirmation().MlmeRequest == MLME_JOIN ) )
                {// Procedure for the join request
                    mlme.get_confirmation().NbRetries = _params.JoinRequestTrials;

                    if( mlme.get_confirmation().Status == LORAMAC_EVENT_INFO_STATUS_OK )
                    {// Node joined successfully
                        _params.UpLinkCounter = 0;
                        _params.ChannelsNbRepCounter = 0;
                        _params.LoRaMacState &= ~LORAMAC_TX_RUNNING;
                    }
                    else
                    {
                        if( _params.JoinRequestTrials >= _params.MaxJoinRequestTrials )
                        {
                            _params.LoRaMacState &= ~LORAMAC_TX_RUNNING;
                        }
                        else
                        {
                            _params.LoRaMacFlags.Bits.MacDone = 0;
                            // Sends the same frame again
                            handle_delayed_tx_timer_event();
                        }
                    }
                }
                else
                {// Procedure for all other frames
                    if( ( _params.ChannelsNbRepCounter >= _params.sys_params.ChannelsNbRep ) || ( _params.LoRaMacFlags.Bits.McpsInd == 1 ) )
                    {
                        if( _params.LoRaMacFlags.Bits.McpsInd == 0 )
                        {   // Maximum repetitions without downlink. Reset MacCommandsBufferIndex. Increase ADR Ack counter.
                            // Only process the case when the MAC did not receive a downlink.
                            mac_commands.ClearCommandBuffer();
                            _params.AdrAckCounter++;
                        }

                        _params.ChannelsNbRepCounter = 0;

                        if( _params.IsUpLinkCounterFixed == false )
                        {
                            _params.UpLinkCounter++;
                        }

                        _params.LoRaMacState &= ~LORAMAC_TX_RUNNING;
                    }
                    else
                    {
                        _params.LoRaMacFlags.Bits.MacDone = 0;
                        // Sends the same frame again
                        handle_delayed_tx_timer_event();
                    }
                }
            }
        }

        if( _params.LoRaMacFlags.Bits.McpsInd == 1 )
        {// Procedure if we received a frame
            if( ( mcps.get_confirmation().AckReceived == true ) ||
                ( _params.AckTimeoutRetriesCounter > _params.AckTimeoutRetries ) )
            {
                _params.AckTimeoutRetry = false;
                _params.NodeAckRequested = false;
                if( _params.IsUpLinkCounterFixed == false )
                {
                    _params.UpLinkCounter++;
                }
                mcps.get_confirmation().NbRetries = _params.AckTimeoutRetriesCounter;

                _params.LoRaMacState &= ~LORAMAC_TX_RUNNING;
            }
        }

        if( ( _params.AckTimeoutRetry == true ) && ( ( _params.LoRaMacState & LORAMAC_TX_DELAYED ) == 0 ) )
        {// Retransmissions procedure for confirmed uplinks
            _params.AckTimeoutRetry = false;
            if( ( _params.AckTimeoutRetriesCounter < _params.AckTimeoutRetries ) &&
                ( _params.AckTimeoutRetriesCounter <= MAX_ACK_RETRIES ) )
            {
                _params.AckTimeoutRetriesCounter++;

                if( ( _params.AckTimeoutRetriesCounter % 2 ) == 1 )
                {
                    getPhy.Attribute = PHY_NEXT_LOWER_TX_DR;
                    getPhy.UplinkDwellTime = _params.sys_params.UplinkDwellTime;
                    getPhy.Datarate = _params.sys_params.ChannelsDatarate;
                    phyParam = lora_phy->get_phy_params( &getPhy );
                    _params.sys_params.ChannelsDatarate = phyParam.Value;
                }
                // Try to send the frame again
                if( ScheduleTx( ) == LORAMAC_STATUS_OK )
                {
                    _params.LoRaMacFlags.Bits.MacDone = 0;
                }
                else
                {
                    // The DR is not applicable for the payload size
                    mcps.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_TX_DR_PAYLOAD_SIZE_ERROR;

                    mac_commands.ClearCommandBuffer();
                    _params.LoRaMacState &= ~LORAMAC_TX_RUNNING;
                    _params.NodeAckRequested = false;
                    mcps.get_confirmation().AckReceived = false;
                    mcps.get_confirmation().NbRetries = _params.AckTimeoutRetriesCounter;
                    mcps.get_confirmation().Datarate = _params.sys_params.ChannelsDatarate;
                    if( _params.IsUpLinkCounterFixed == false )
                    {
                        _params.UpLinkCounter++;
                    }
                }
            }
            else
            {
                lora_phy->load_defaults(INIT_TYPE_RESTORE);

                _params.LoRaMacState &= ~LORAMAC_TX_RUNNING;

                mac_commands.ClearCommandBuffer();
                _params.NodeAckRequested = false;
                mcps.get_confirmation().AckReceived = false;
                mcps.get_confirmation().NbRetries = _params.AckTimeoutRetriesCounter;
                if( _params.IsUpLinkCounterFixed == false )
                {
                    _params.UpLinkCounter++;
                }
            }
        }
    }
    // Handle reception for Class B and Class C
    if( ( _params.LoRaMacState & LORAMAC_RX ) == LORAMAC_RX )
    {
        _params.LoRaMacState &= ~LORAMAC_RX;
    }
    if( _params.LoRaMacState == LORAMAC_IDLE )
    {
        if( _params.LoRaMacFlags.Bits.McpsReq == 1 )
        {
            _params.LoRaMacFlags.Bits.McpsReq = 0;
            LoRaMacPrimitives->MacMcpsConfirm( &mcps.get_confirmation() );
        }

        if( _params.LoRaMacFlags.Bits.MlmeReq == 1 )
        {
            _params.LoRaMacFlags.Bits.MlmeReq = 0;
            LoRaMacPrimitives->MacMlmeConfirm(&mlme.get_confirmation());
        }

        // Verify if sticky MAC commands are pending or not
        if( mac_commands.IsStickyMacCommandPending( ) == true )
        {// Setup MLME indication
            SetMlmeScheduleUplinkIndication( );
        }

        // Procedure done. Reset variables.
        _params.LoRaMacFlags.Bits.MacDone = 0;
    }
    else
    {
        // Operation not finished restart timer
        _lora_time.TimerSetValue( &_params.timers.MacStateCheckTimer,
                                  MAC_STATE_CHECK_TIMEOUT );
        _lora_time.TimerStart( &_params.timers.MacStateCheckTimer );
    }

    // Handle MCPS indication
    if( _params.LoRaMacFlags.Bits.McpsInd == 1 )
    {
        _params.LoRaMacFlags.Bits.McpsInd = 0;
        if( _params.LoRaMacDeviceClass == CLASS_C )
        {// Activate RX2 window for Class C
            OpenContinuousRx2Window( );
        }
        if( _params.LoRaMacFlags.Bits.McpsIndSkip == 0 )
        {
            LoRaMacPrimitives->MacMcpsIndication( &mcps.get_indication() );
        }
        _params.LoRaMacFlags.Bits.McpsIndSkip = 0;
    }

    // Handle MLME indication
    if( _params.LoRaMacFlags.Bits.MlmeInd == 1 )
    {
        _params.LoRaMacFlags.Bits.MlmeInd = 0;
        LoRaMacPrimitives->MacMlmeIndication(&mlme.get_indication());
    }
}

void LoRaMac::OnTxDelayedTimerEvent( void )
{
    LoRaMacHeader_t macHdr;
    LoRaMacFrameCtrl_t fCtrl;
    AlternateDrParams_t altDr;

    _lora_time.TimerStop( &_params.timers.TxDelayedTimer );
    _params.LoRaMacState &= ~LORAMAC_TX_DELAYED;

    if( ( _params.LoRaMacFlags.Bits.MlmeReq == 1 ) && ( mlme.get_confirmation().MlmeRequest == MLME_JOIN ) )
    {
        ResetMacParameters( );

        altDr.NbTrials = _params.JoinRequestTrials + 1;
        _params.sys_params.ChannelsDatarate = lora_phy->get_alternate_DR(&altDr);

        macHdr.Value = 0;
        macHdr.Bits.MType = FRAME_TYPE_JOIN_REQ;

        fCtrl.Value = 0;
        fCtrl.Bits.Adr = _params.sys_params.AdrCtrlOn;

        /* In case of join request retransmissions, the stack must prepare
         * the frame again, because the network server keeps track of the random
         * LoRaMacDevNonce values to prevent reply attacks. */
        PrepareFrame( &macHdr, &fCtrl, 0, NULL, 0 );
    }

    ScheduleTx( );
}

void LoRaMac::OnRxWindow1TimerEvent( void )
{
    _lora_time.TimerStop( &_params.timers.RxWindowTimer1 );
    _params.RxSlot = RX_SLOT_WIN_1;

    _params.RxWindow1Config.Channel = _params.Channel;
    _params.RxWindow1Config.DrOffset = _params.sys_params.Rx1DrOffset;
    _params.RxWindow1Config.DownlinkDwellTime = _params.sys_params.DownlinkDwellTime;
    _params.RxWindow1Config.RepeaterSupport = _params.RepeaterSupport;
    _params.RxWindow1Config.RxContinuous = false;
    _params.RxWindow1Config.RxSlot = _params.RxSlot;

    if( _params.LoRaMacDeviceClass == CLASS_C )
    {
        lora_phy->put_radio_to_standby();
    }

    lora_phy->rx_config(&_params.RxWindow1Config, ( int8_t* )&mcps.get_indication().RxDatarate);
    RxWindowSetup( _params.RxWindow1Config.RxContinuous, _params.sys_params.MaxRxWindow );
}

void LoRaMac::OnRxWindow2TimerEvent( void )
{
    _lora_time.TimerStop( &_params.timers.RxWindowTimer2 );

    _params.RxWindow2Config.Channel = _params.Channel;
    _params.RxWindow2Config.Frequency = _params.sys_params.Rx2Channel.Frequency;
    _params.RxWindow2Config.DownlinkDwellTime = _params.sys_params.DownlinkDwellTime;
    _params.RxWindow2Config.RepeaterSupport = _params.RepeaterSupport;
    _params.RxWindow2Config.RxSlot = RX_SLOT_WIN_2;

    if( _params.LoRaMacDeviceClass != CLASS_C )
    {
        _params.RxWindow2Config.RxContinuous = false;
    }
    else
    {
        // Setup continuous listening for class c
        _params.RxWindow2Config.RxContinuous = true;
    }

    if(lora_phy->rx_config(&_params.RxWindow2Config, ( int8_t* )&mcps.get_indication().RxDatarate) == true )
    {
        RxWindowSetup( _params.RxWindow2Config.RxContinuous, _params.sys_params.MaxRxWindow );
        _params.RxSlot = RX_SLOT_WIN_2;
    }
}

void LoRaMac::OnAckTimeoutTimerEvent( void )
{
    _lora_time.TimerStop( &_params.timers.AckTimeoutTimer );

    if( _params.NodeAckRequested == true )
    {
        _params.AckTimeoutRetry = true;
        _params.LoRaMacState &= ~LORAMAC_ACK_REQ;
    }
    if( _params.LoRaMacDeviceClass == CLASS_C )
    {
        _params.LoRaMacFlags.Bits.MacDone = 1;
    }
}

void LoRaMac::RxWindowSetup( bool rxContinuous, uint32_t maxRxWindow )
{
    lora_phy->setup_rx_window(rxContinuous, maxRxWindow);
}

bool LoRaMac::ValidatePayloadLength( uint8_t lenN, int8_t datarate, uint8_t fOptsLen )
{
    GetPhyParams_t getPhy;
    PhyParam_t phyParam;
    uint16_t maxN = 0;
    uint16_t payloadSize = 0;

    // Setup PHY request
    getPhy.UplinkDwellTime = _params.sys_params.UplinkDwellTime;
    getPhy.Datarate = datarate;
    getPhy.Attribute = PHY_MAX_PAYLOAD;

    // Get the maximum payload length
    if( _params.RepeaterSupport == true )
    {
        getPhy.Attribute = PHY_MAX_PAYLOAD_REPEATER;
    }
    phyParam = lora_phy->get_phy_params(&getPhy);
    maxN = phyParam.Value;

    // Calculate the resulting payload size
    payloadSize = ( lenN + fOptsLen );

    // Validation of the application payload size
    if( ( payloadSize <= maxN ) && ( payloadSize <= LORAMAC_PHY_MAXPAYLOAD ) )
    {
        return true;
    }
    return false;
}

void LoRaMac::SetMlmeScheduleUplinkIndication( void )
{
    mlme.get_indication().MlmeIndication = MLME_SCHEDULE_UPLINK;
    _params.LoRaMacFlags.Bits.MlmeInd = 1;
}

// This is not actual transmission. It just schedules a message in response
// to MCPS request
LoRaMacStatus_t LoRaMac::Send( LoRaMacHeader_t *macHdr, uint8_t fPort, void *fBuffer, uint16_t fBufferSize )
{
    LoRaMacFrameCtrl_t fCtrl;
    LoRaMacStatus_t status = LORAMAC_STATUS_PARAMETER_INVALID;

    fCtrl.Value = 0;
    fCtrl.Bits.FOptsLen      = 0;
    fCtrl.Bits.FPending      = 0;
    fCtrl.Bits.Ack           = false;
    fCtrl.Bits.AdrAckReq     = false;
    fCtrl.Bits.Adr           = _params.sys_params.AdrCtrlOn;

    // Prepare the frame
    status = PrepareFrame( macHdr, &fCtrl, fPort, fBuffer, fBufferSize );

    // Validate status
    if( status != LORAMAC_STATUS_OK )
    {
        return status;
    }

    // Reset confirm parameters
    mcps.get_confirmation().NbRetries = 0;
    mcps.get_confirmation().AckReceived = false;
    mcps.get_confirmation().UpLinkCounter = _params.UpLinkCounter;

    status = ScheduleTx( );

    return status;
}

LoRaMacStatus_t LoRaMac::ScheduleTx( void )
{
    TimerTime_t dutyCycleTimeOff = 0;
    NextChanParams_t nextChan;

    // Check if the device is off
    if( _params.sys_params.MaxDCycle == 255 )
    {
        return LORAMAC_STATUS_DEVICE_OFF;
    }
    if( _params.sys_params.MaxDCycle == 0 )
    {
        _params.timers.AggregatedTimeOff = 0;
    }

    // Update Backoff
    CalculateBackOff( _params.LastTxChannel );

    nextChan.AggrTimeOff = _params.timers.AggregatedTimeOff;
    nextChan.Datarate = _params.sys_params.ChannelsDatarate;
    _params.DutyCycleOn = MBED_CONF_LORA_DUTY_CYCLE_ON;
    nextChan.DutyCycleEnabled = _params.DutyCycleOn;
    nextChan.Joined = _params.IsLoRaMacNetworkJoined;
    nextChan.LastAggrTx = _params.timers.AggregatedLastTxDoneTime;

    // Select channel
    while( lora_phy->set_next_channel(&nextChan, &_params.Channel, &dutyCycleTimeOff,
                                      &_params.timers.AggregatedTimeOff ) == false )
    {
        // Set the default datarate
        _params.sys_params.ChannelsDatarate = _params.def_sys_params.ChannelsDatarate;
        // Update datarate in the function parameters
        nextChan.Datarate = _params.sys_params.ChannelsDatarate;
    }

    tr_debug("Next Channel Idx=%d, DR=%d", _params.Channel, nextChan.Datarate);

    // Compute Rx1 windows parameters
    uint8_t dr_offset = lora_phy->apply_DR_offset(_params.sys_params.DownlinkDwellTime,
                                                 _params.sys_params.ChannelsDatarate,
                                                 _params.sys_params.Rx1DrOffset);

    lora_phy->compute_rx_win_params(dr_offset, _params.sys_params.MinRxSymbols,
                                     _params.sys_params.SystemMaxRxError,
                                     &_params.RxWindow1Config );
    // Compute Rx2 windows parameters
    lora_phy->compute_rx_win_params(_params.sys_params.Rx2Channel.Datarate,
                                    _params.sys_params.MinRxSymbols,
                                    _params.sys_params.SystemMaxRxError,
                                    &_params.RxWindow2Config );

    if( _params.IsLoRaMacNetworkJoined == false )
    {
        _params.RxWindow1Delay = _params.sys_params.JoinAcceptDelay1 + _params.RxWindow1Config.WindowOffset;
        _params.RxWindow2Delay = _params.sys_params.JoinAcceptDelay2 + _params.RxWindow2Config.WindowOffset;
    }
    else
    {
        if( ValidatePayloadLength( _params.LoRaMacTxPayloadLen,
                                   _params.sys_params.ChannelsDatarate,
                                   mac_commands.GetLength() ) == false )
        {
            return LORAMAC_STATUS_LENGTH_ERROR;
        }
        _params.RxWindow1Delay = _params.sys_params.ReceiveDelay1 + _params.RxWindow1Config.WindowOffset;
        _params.RxWindow2Delay = _params.sys_params.ReceiveDelay2 + _params.RxWindow2Config.WindowOffset;
    }

    // Schedule transmission of frame
    if( dutyCycleTimeOff == 0 )
    {
        // Try to send now
        return SendFrameOnChannel( _params.Channel );
    }
    else
    {
        // Send later - prepare timer
        _params.LoRaMacState |= LORAMAC_TX_DELAYED;
        tr_debug("Next Transmission in %lu ms", dutyCycleTimeOff);

        _lora_time.TimerSetValue( &_params.timers.TxDelayedTimer, dutyCycleTimeOff );
        _lora_time.TimerStart( &_params.timers.TxDelayedTimer );

        return LORAMAC_STATUS_OK;
    }
}

void LoRaMac::CalculateBackOff( uint8_t channel )
{
    CalcBackOffParams_t calcBackOff;

    calcBackOff.Joined = _params.IsLoRaMacNetworkJoined;
    _params.DutyCycleOn = MBED_CONF_LORA_DUTY_CYCLE_ON;
    calcBackOff.DutyCycleEnabled = _params.DutyCycleOn;
    calcBackOff.Channel = channel;
    calcBackOff.ElapsedTime = _lora_time.TimerGetElapsedTime( _params.timers.LoRaMacInitializationTime );
    calcBackOff.TxTimeOnAir = _params.timers.TxTimeOnAir;
    calcBackOff.LastTxIsJoinRequest = _params.LastTxIsJoinRequest;

    // Update regional back-off
    lora_phy->calculate_backoff(&calcBackOff);

    // Update aggregated time-off
    _params.timers.AggregatedTimeOff = _params.timers.AggregatedTimeOff +
            ( _params.timers.TxTimeOnAir * _params.sys_params.AggregatedDCycle - _params.timers.TxTimeOnAir );
}

void LoRaMac::ResetMacParameters( void )
{
    _params.IsLoRaMacNetworkJoined = false;

    // Counters
    _params.UpLinkCounter = 0;
    _params.DownLinkCounter = 0;
    _params.AdrAckCounter = 0;

    _params.ChannelsNbRepCounter = 0;

    _params.AckTimeoutRetries = 1;
    _params.AckTimeoutRetriesCounter = 1;
    _params.AckTimeoutRetry = false;

    _params.sys_params.MaxDCycle = 0;
    _params.sys_params.AggregatedDCycle = 1;

    mac_commands.ClearCommandBuffer();
    mac_commands.ClearRepeatBuffer();
    mac_commands.ClearMacCommandsInNextTx();

    _params.IsRxWindowsEnabled = true;

    _params.sys_params.ChannelsTxPower = _params.def_sys_params.ChannelsTxPower;
    _params.sys_params.ChannelsDatarate = _params.def_sys_params.ChannelsDatarate;
    _params.sys_params.Rx1DrOffset = _params.def_sys_params.Rx1DrOffset;
    _params.sys_params.Rx2Channel = _params.def_sys_params.Rx2Channel;
    _params.sys_params.UplinkDwellTime = _params.def_sys_params.UplinkDwellTime;
    _params.sys_params.DownlinkDwellTime = _params.def_sys_params.DownlinkDwellTime;
    _params.sys_params.MaxEirp = _params.def_sys_params.MaxEirp;
    _params.sys_params.AntennaGain = _params.def_sys_params.AntennaGain;

    _params.NodeAckRequested = false;
    _params.SrvAckRequested = false;

    // Reset Multicast downlink counters
    MulticastParams_t *cur = _params.MulticastChannels;
    while( cur != NULL )
    {
        cur->DownLinkCounter = 0;
        cur = cur->Next;
    }

    // Initialize channel index.
    _params.Channel = 0;
    _params.LastTxChannel = _params.Channel;
}

bool LoRaMac::IsFPortAllowed( uint8_t fPort )
{
    if( ( fPort == 0 ) || ( fPort > 224 ) )
    {
        return false;
    }
    return true;
}

void LoRaMac::OpenContinuousRx2Window( void )
{
    handle_rx2_timer_event( );
    _params.RxSlot = RX_SLOT_WIN_CLASS_C;
}

static void memcpy_convert_endianess( uint8_t *dst, const uint8_t *src, uint16_t size )
{
    dst = dst + ( size - 1 );
    while( size-- )
    {
        *dst-- = *src++;
    }
}

LoRaMacStatus_t LoRaMac::PrepareFrame( LoRaMacHeader_t *macHdr, LoRaMacFrameCtrl_t *fCtrl, uint8_t fPort, void *fBuffer, uint16_t fBufferSize )
{
    AdrNextParams_t adrNext;
    uint16_t i;
    uint8_t pktHeaderLen = 0;
    uint32_t mic = 0;
    const void* payload = fBuffer;
    uint8_t framePort = fPort;
    LoRaMacStatus_t status = LORAMAC_STATUS_OK;

    _params.LoRaMacBufferPktLen = 0;

    _params.NodeAckRequested = false;

    if( fBuffer == NULL )
    {
        fBufferSize = 0;
    }

    _params.LoRaMacTxPayloadLen = fBufferSize;

    _params.LoRaMacBuffer[pktHeaderLen++] = macHdr->Value;

    switch( macHdr->Bits.MType )
    {
        case FRAME_TYPE_JOIN_REQ:
            _params.LoRaMacBufferPktLen = pktHeaderLen;

            memcpy_convert_endianess( _params.LoRaMacBuffer + _params.LoRaMacBufferPktLen,
                                      _params.keys.LoRaMacAppEui, 8 );
            _params.LoRaMacBufferPktLen += 8;
            memcpy_convert_endianess( _params.LoRaMacBuffer + _params.LoRaMacBufferPktLen,
                                      _params.keys.LoRaMacDevEui, 8 );
            _params.LoRaMacBufferPktLen += 8;

            _params.LoRaMacDevNonce = lora_phy->get_radio_rng();

            _params.LoRaMacBuffer[_params.LoRaMacBufferPktLen++] = _params.LoRaMacDevNonce & 0xFF;
            _params.LoRaMacBuffer[_params.LoRaMacBufferPktLen++] = ( _params.LoRaMacDevNonce >> 8 ) & 0xFF;

            if (0 != LoRaMacJoinComputeMic( _params.LoRaMacBuffer,
                                            _params.LoRaMacBufferPktLen & 0xFF,
                                            _params.keys.LoRaMacAppKey, &mic )) {
                return LORAMAC_STATUS_CRYPTO_FAIL;
            }

            _params.LoRaMacBuffer[_params.LoRaMacBufferPktLen++] = mic & 0xFF;
            _params.LoRaMacBuffer[_params.LoRaMacBufferPktLen++] = ( mic >> 8 ) & 0xFF;
            _params. LoRaMacBuffer[_params.LoRaMacBufferPktLen++] = ( mic >> 16 ) & 0xFF;
            _params.LoRaMacBuffer[_params.LoRaMacBufferPktLen++] = ( mic >> 24 ) & 0xFF;

            break;
        case FRAME_TYPE_DATA_CONFIRMED_UP:
            _params.NodeAckRequested = true;
            //Intentional fallthrough
        case FRAME_TYPE_DATA_UNCONFIRMED_UP:
        {
            if( _params.IsLoRaMacNetworkJoined == false )
            {
                return LORAMAC_STATUS_NO_NETWORK_JOINED; // No network has been joined yet
            }

            // Adr next request
            adrNext.UpdateChanMask = true;
            adrNext.AdrEnabled = fCtrl->Bits.Adr;
            adrNext.AdrAckCounter = _params.AdrAckCounter;
            adrNext.Datarate = _params.sys_params.ChannelsDatarate;
            adrNext.TxPower = _params.sys_params.ChannelsTxPower;
            adrNext.UplinkDwellTime = _params.sys_params.UplinkDwellTime;

            fCtrl->Bits.AdrAckReq = lora_phy->get_next_ADR(&adrNext,
                                                          &_params.sys_params.ChannelsDatarate,
                                                          &_params.sys_params.ChannelsTxPower,
                                                          &_params.AdrAckCounter);

            if( _params.SrvAckRequested == true )
            {
                _params.SrvAckRequested = false;
                fCtrl->Bits.Ack = 1;
            }

            _params.LoRaMacBuffer[pktHeaderLen++] = ( _params.LoRaMacDevAddr ) & 0xFF;
            _params.LoRaMacBuffer[pktHeaderLen++] = ( _params.LoRaMacDevAddr >> 8 ) & 0xFF;
            _params.LoRaMacBuffer[pktHeaderLen++] = ( _params.LoRaMacDevAddr >> 16 ) & 0xFF;
            _params.LoRaMacBuffer[pktHeaderLen++] = ( _params.LoRaMacDevAddr >> 24 ) & 0xFF;

            _params.LoRaMacBuffer[pktHeaderLen++] = fCtrl->Value;

            _params.LoRaMacBuffer[pktHeaderLen++] = _params.UpLinkCounter & 0xFF;
            _params.LoRaMacBuffer[pktHeaderLen++] = ( _params.UpLinkCounter >> 8 ) & 0xFF;

            // Copy the MAC commands which must be re-send into the MAC command buffer
            mac_commands.CopyRepeatCommandsToBuffer();

            const uint8_t mac_commands_len = mac_commands.GetLength();
            if( ( payload != NULL ) && ( _params.LoRaMacTxPayloadLen > 0 ) )
            {
                if( mac_commands.IsMacCommandsInNextTx() == true )
                {
                    if( mac_commands_len <= LORA_MAC_COMMAND_MAX_FOPTS_LENGTH )
                    {
                        fCtrl->Bits.FOptsLen += mac_commands_len;

                        // Update FCtrl field with new value of OptionsLength
                        _params.LoRaMacBuffer[0x05] = fCtrl->Value;

                        const uint8_t *buffer = mac_commands.GetMacCommandsBuffer();
                        for( i = 0; i < mac_commands_len; i++ )
                        {
                            _params.LoRaMacBuffer[pktHeaderLen++] = buffer[i];
                        }
                    }
                    else
                    {
                        _params.LoRaMacTxPayloadLen = mac_commands_len;
                        payload = mac_commands.GetMacCommandsBuffer();
                        framePort = 0;
                    }
                }
            }
            else
            {
                if( ( mac_commands_len > 0 ) && ( mac_commands.IsMacCommandsInNextTx() == true ) )
                {
                    _params.LoRaMacTxPayloadLen = mac_commands_len;
                    payload = mac_commands.GetMacCommandsBuffer();
                    framePort = 0;
                }
            }

            // Store MAC commands which must be re-send in case the device does not receive a downlink anymore
            mac_commands.ParseMacCommandsToRepeat();

            if( ( payload != NULL ) && ( _params.LoRaMacTxPayloadLen > 0 ) )
            {
                _params.LoRaMacBuffer[pktHeaderLen++] = framePort;

                if( framePort == 0 )
                {
                    // Reset buffer index as the mac commands are being sent on port 0
                    mac_commands.ClearCommandBuffer();
                    if (0 != LoRaMacPayloadEncrypt( (uint8_t* ) payload,
                                                    _params.LoRaMacTxPayloadLen,
                                                    _params.keys.LoRaMacNwkSKey,
                                                    _params.LoRaMacDevAddr,
                                                    UP_LINK,
                                                    _params.UpLinkCounter,
                                                    &_params.LoRaMacBuffer[pktHeaderLen] )) {
                        status = LORAMAC_STATUS_CRYPTO_FAIL;
                    }
                }
                else
                {
                    if (0 != LoRaMacPayloadEncrypt( (uint8_t* ) payload,
                                                    _params.LoRaMacTxPayloadLen,
                                                    _params.keys.LoRaMacAppSKey,
                                                    _params.LoRaMacDevAddr,
                                                    UP_LINK,
                                                    _params.UpLinkCounter,
                                                    &_params.LoRaMacBuffer[pktHeaderLen] )) {
                        status = LORAMAC_STATUS_CRYPTO_FAIL;
                    }
                }
            }
            _params.LoRaMacBufferPktLen = pktHeaderLen + _params.LoRaMacTxPayloadLen;

            if (0 != LoRaMacComputeMic( _params.LoRaMacBuffer,
                                        _params.LoRaMacBufferPktLen,
                                        _params.keys.LoRaMacNwkSKey,
                                        _params.LoRaMacDevAddr,
                                        UP_LINK,
                                        _params.UpLinkCounter,
                                        &mic )) {
                status = LORAMAC_STATUS_CRYPTO_FAIL;
            }

            _params.LoRaMacBuffer[_params.LoRaMacBufferPktLen + 0] = mic & 0xFF;
            _params.LoRaMacBuffer[_params.LoRaMacBufferPktLen + 1] = ( mic >> 8 ) & 0xFF;
            _params.LoRaMacBuffer[_params.LoRaMacBufferPktLen + 2] = ( mic >> 16 ) & 0xFF;
            _params.LoRaMacBuffer[_params.LoRaMacBufferPktLen + 3] = ( mic >> 24 ) & 0xFF;

            _params.LoRaMacBufferPktLen += LORAMAC_MFR_LEN;
        }
        break;
        case FRAME_TYPE_PROPRIETARY:
            if( ( fBuffer != NULL ) && (_params.LoRaMacTxPayloadLen > 0 ) )
            {
                memcpy( _params.LoRaMacBuffer + pktHeaderLen, ( uint8_t* ) fBuffer, _params.LoRaMacTxPayloadLen );
                _params.LoRaMacBufferPktLen = pktHeaderLen + _params.LoRaMacTxPayloadLen;
            }
            break;
        default:
            status = LORAMAC_STATUS_SERVICE_UNKNOWN;
    }

    return status;
}

LoRaMacStatus_t LoRaMac::SendFrameOnChannel( uint8_t channel )
{
    TxConfigParams_t txConfig;
    int8_t txPower = 0;

    txConfig.Channel = channel;
    txConfig.Datarate = _params.sys_params.ChannelsDatarate;
    txConfig.TxPower = _params.sys_params.ChannelsTxPower;
    txConfig.MaxEirp = _params.sys_params.MaxEirp;
    txConfig.AntennaGain = _params.sys_params.AntennaGain;
    txConfig.PktLen = _params.LoRaMacBufferPktLen;

    lora_phy->tx_config(&txConfig, &txPower, &_params.timers.TxTimeOnAir);

    mlme.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_ERROR;

    mcps.get_confirmation().Status = LORAMAC_EVENT_INFO_STATUS_ERROR;
    mcps.get_confirmation().Datarate = _params.sys_params.ChannelsDatarate;
    mcps.get_confirmation().TxPower = txPower;

    // Store the time on air
    mcps.get_confirmation().TxTimeOnAir = _params.timers.TxTimeOnAir;
    mlme.get_confirmation().TxTimeOnAir = _params.timers.TxTimeOnAir;

    // Starts the MAC layer status check timer
    _lora_time.TimerSetValue( &_params.timers.MacStateCheckTimer, MAC_STATE_CHECK_TIMEOUT );
    _lora_time.TimerStart( &_params.timers.MacStateCheckTimer );

    if( _params.IsLoRaMacNetworkJoined == false )
    {
        _params.JoinRequestTrials++;
    }

    // Send now
    lora_phy->handle_send(_params.LoRaMacBuffer, _params.LoRaMacBufferPktLen);

    _params.LoRaMacState |= LORAMAC_TX_RUNNING;

    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t LoRaMac::SetTxContinuousWave( uint16_t timeout )
{
    ContinuousWaveParams_t continuousWave;

    continuousWave.Channel = _params.Channel;
    continuousWave.Datarate = _params.sys_params.ChannelsDatarate;
    continuousWave.TxPower = _params.sys_params.ChannelsTxPower;
    continuousWave.MaxEirp = _params.sys_params.MaxEirp;
    continuousWave.AntennaGain = _params.sys_params.AntennaGain;
    continuousWave.Timeout = timeout;

    lora_phy->set_tx_cont_mode(&continuousWave);

    // Starts the MAC layer status check timer
    _lora_time.TimerSetValue( &_params.timers.MacStateCheckTimer, MAC_STATE_CHECK_TIMEOUT );
    _lora_time.TimerStart( &_params.timers.MacStateCheckTimer );

    _params.LoRaMacState |= LORAMAC_TX_RUNNING;

    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t LoRaMac::SetTxContinuousWave1( uint16_t timeout, uint32_t frequency, uint8_t power )
{
    lora_phy->setup_tx_cont_wave_mode(frequency, power, timeout);

    // Starts the MAC layer status check timer
    _lora_time.TimerSetValue( &_params.timers.MacStateCheckTimer, MAC_STATE_CHECK_TIMEOUT );
    _lora_time.TimerStart( &_params.timers.MacStateCheckTimer );

    _params.LoRaMacState |= LORAMAC_TX_RUNNING;

    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t LoRaMac::LoRaMacInitialization(LoRaMacPrimitives_t *primitives,
                                               LoRaMacCallback_t *callbacks,
                                               LoRaPHY *phy,
                                               EventQueue *queue)
{
    GetPhyParams_t getPhy;
    PhyParam_t phyParam;

    //store event queue pointer
    ev_queue = queue;

    if(!primitives || !callbacks)
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    lora_phy = phy;

    // Activate MLME subsystem
    mlme.activate_mlme_subsystem(this, lora_phy, &mac_commands);

    // Activate MCPS subsystem
    mcps.activate_mcps_subsystem(this, lora_phy);

    // Activate MIB subsystem
    mib.activate_mib_subsystem(this, lora_phy);

    LoRaMacPrimitives = primitives;
    LoRaMacCallbacks = callbacks;

    _params.LoRaMacFlags.Value = 0;

    _params.LoRaMacDeviceClass = CLASS_A;
    _params.LoRaMacState = LORAMAC_IDLE;

    _params.JoinRequestTrials = 0;
    _params.MaxJoinRequestTrials = 1;
    _params.RepeaterSupport = false;

    // Reset duty cycle times
    _params.timers.AggregatedLastTxDoneTime = 0;
    _params.timers.AggregatedTimeOff = 0;

    // Reset to defaults
    getPhy.Attribute = PHY_DUTY_CYCLE;
    phyParam = lora_phy->get_phy_params(&getPhy);
    // load default at this moment. Later can be changed using json
    _params.DutyCycleOn = ( bool ) phyParam.Value;

    getPhy.Attribute = PHY_DEF_TX_POWER;
    phyParam = lora_phy->get_phy_params( &getPhy );
    _params.def_sys_params.ChannelsTxPower = phyParam.Value;

    getPhy.Attribute = PHY_DEF_TX_DR;
    phyParam = lora_phy->get_phy_params( &getPhy );
    _params.def_sys_params.ChannelsDatarate = phyParam.Value;

    getPhy.Attribute = PHY_MAX_RX_WINDOW;
    phyParam = lora_phy->get_phy_params( &getPhy );
    _params.def_sys_params.MaxRxWindow = phyParam.Value;

    getPhy.Attribute = PHY_RECEIVE_DELAY1;
    phyParam = lora_phy->get_phy_params( &getPhy );
    _params.def_sys_params.ReceiveDelay1 = phyParam.Value;

    getPhy.Attribute = PHY_RECEIVE_DELAY2;
    phyParam = lora_phy->get_phy_params( &getPhy );
    _params.def_sys_params.ReceiveDelay2 = phyParam.Value;

    getPhy.Attribute = PHY_JOIN_ACCEPT_DELAY1;
    phyParam = lora_phy->get_phy_params( &getPhy );
    _params.def_sys_params.JoinAcceptDelay1 = phyParam.Value;

    getPhy.Attribute = PHY_JOIN_ACCEPT_DELAY2;
    phyParam = lora_phy->get_phy_params( &getPhy );
    _params.def_sys_params.JoinAcceptDelay2 = phyParam.Value;

    getPhy.Attribute = PHY_DEF_DR1_OFFSET;
    phyParam = lora_phy->get_phy_params( &getPhy );
    _params.def_sys_params.Rx1DrOffset = phyParam.Value;

    getPhy.Attribute = PHY_DEF_RX2_FREQUENCY;
    phyParam = lora_phy->get_phy_params( &getPhy );
    _params.def_sys_params.Rx2Channel.Frequency = phyParam.Value;

    getPhy.Attribute = PHY_DEF_RX2_DR;
    phyParam = lora_phy->get_phy_params( &getPhy );
    _params.def_sys_params.Rx2Channel.Datarate = phyParam.Value;

    getPhy.Attribute = PHY_DEF_UPLINK_DWELL_TIME;
    phyParam = lora_phy->get_phy_params( &getPhy );
    _params.def_sys_params.UplinkDwellTime = phyParam.Value;

    getPhy.Attribute = PHY_DEF_DOWNLINK_DWELL_TIME;
    phyParam = lora_phy->get_phy_params( &getPhy );
    _params.def_sys_params.DownlinkDwellTime = phyParam.Value;

    getPhy.Attribute = PHY_DEF_MAX_EIRP;
    phyParam = lora_phy->get_phy_params( &getPhy );
    _params.def_sys_params.MaxEirp = phyParam.fValue;

    getPhy.Attribute = PHY_DEF_ANTENNA_GAIN;
    phyParam = lora_phy->get_phy_params( &getPhy );
    _params.def_sys_params.AntennaGain = phyParam.fValue;

    lora_phy->load_defaults(INIT_TYPE_INIT);

    // Init parameters which are not set in function ResetMacParameters
    _params.def_sys_params.ChannelsNbRep = 1;
    _params.def_sys_params.SystemMaxRxError = 10;
    _params.def_sys_params.MinRxSymbols = 6;

    _params.sys_params.SystemMaxRxError = _params.def_sys_params.SystemMaxRxError;
    _params.sys_params.MinRxSymbols = _params.def_sys_params.MinRxSymbols;
    _params.sys_params.MaxRxWindow = _params.def_sys_params.MaxRxWindow;
    _params.sys_params.ReceiveDelay1 = _params.def_sys_params.ReceiveDelay1;
    _params.sys_params.ReceiveDelay2 = _params.def_sys_params.ReceiveDelay2;
    _params.sys_params.JoinAcceptDelay1 = _params.def_sys_params.JoinAcceptDelay1;
    _params.sys_params.JoinAcceptDelay2 = _params.def_sys_params.JoinAcceptDelay2;
    _params.sys_params.ChannelsNbRep = _params.def_sys_params.ChannelsNbRep;

    ResetMacParameters( );

    // Random seed initialization
    srand(lora_phy->get_radio_rng());

    _params.PublicNetwork = MBED_CONF_LORA_PUBLIC_NETWORK;
    lora_phy->setup_public_network_mode(_params.PublicNetwork);
    lora_phy->put_radio_to_sleep();

    // Initialize timers
    _lora_time.TimerInit(&_params.timers.MacStateCheckTimer,
                         mbed::callback(this, &LoRaMac::handle_mac_state_check_timer_event));
    _lora_time.TimerSetValue(&_params.timers.MacStateCheckTimer, MAC_STATE_CHECK_TIMEOUT);

    _lora_time.TimerInit(&_params.timers.TxDelayedTimer,
                         mbed::callback(this, &LoRaMac::handle_delayed_tx_timer_event));
    _lora_time.TimerInit(&_params.timers.RxWindowTimer1,
                         mbed::callback(this, &LoRaMac::handle_rx1_timer_event));
    _lora_time.TimerInit(&_params.timers.RxWindowTimer2,
                         mbed::callback(this, &LoRaMac::handle_rx2_timer_event));
    _lora_time.TimerInit(&_params.timers.AckTimeoutTimer,
                         mbed::callback(this, &LoRaMac::handle_ack_timeout));

    // Store the current initialization time
    _params.timers.LoRaMacInitializationTime = _lora_time.TimerGetCurrentTime();

    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t LoRaMac::LoRaMacQueryTxPossible( uint8_t size, LoRaMacTxInfo_t* txInfo )
{
    AdrNextParams_t adrNext;
    GetPhyParams_t getPhy;
    PhyParam_t phyParam;
    int8_t datarate = _params.def_sys_params.ChannelsDatarate;
    int8_t txPower = _params.def_sys_params.ChannelsTxPower;
    uint8_t fOptLen = mac_commands.GetLength() + mac_commands.GetRepeatLength();

    if( txInfo == NULL )
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    // Setup ADR request
    adrNext.UpdateChanMask = false;
    adrNext.AdrEnabled = _params.sys_params.AdrCtrlOn;
    adrNext.AdrAckCounter = _params.AdrAckCounter;
    adrNext.Datarate = _params.sys_params.ChannelsDatarate;
    adrNext.TxPower = _params.sys_params.ChannelsTxPower;
    adrNext.UplinkDwellTime = _params.sys_params.UplinkDwellTime;

    // We call the function for information purposes only. We don't want to
    // apply the datarate, the tx power and the ADR ack counter.
    lora_phy->get_next_ADR(&adrNext, &datarate, &txPower, &_params.AdrAckCounter);

    // Setup PHY request
    getPhy.UplinkDwellTime = _params.sys_params.UplinkDwellTime;
    getPhy.Datarate = datarate;
    getPhy.Attribute = PHY_MAX_PAYLOAD;

    // Change request in case repeater is supported
    if( _params.RepeaterSupport == true )
    {
        getPhy.Attribute = PHY_MAX_PAYLOAD_REPEATER;
    }
    phyParam = lora_phy->get_phy_params( &getPhy );
    txInfo->CurrentPayloadSize = phyParam.Value;

    // Verify if the fOpts fit into the maximum payload
    if( txInfo->CurrentPayloadSize >= fOptLen )
    {
        txInfo->MaxPossiblePayload = txInfo->CurrentPayloadSize - fOptLen;
    }
    else
    {
        txInfo->MaxPossiblePayload = txInfo->CurrentPayloadSize;
        // The fOpts don't fit into the maximum payload. Omit the MAC commands to
        // ensure that another uplink is possible.
        fOptLen = 0;
        mac_commands.ClearCommandBuffer();
        mac_commands.ClearRepeatBuffer();
    }

    // Verify if the fOpts and the payload fit into the maximum payload
    if( ValidatePayloadLength( size, datarate, fOptLen ) == false )
    {
        return LORAMAC_STATUS_LENGTH_ERROR;
    }
    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t LoRaMac::LoRaMacChannelAdd( uint8_t id, ChannelParams_t params )
{
    ChannelAddParams_t channelAdd;

    // Validate if the MAC is in a correct state
    if( ( _params.LoRaMacState & LORAMAC_TX_RUNNING ) == LORAMAC_TX_RUNNING )
    {
        if( ( _params.LoRaMacState & LORAMAC_TX_CONFIG ) != LORAMAC_TX_CONFIG )
        {
            return LORAMAC_STATUS_BUSY;
        }
    }

    channelAdd.NewChannel = &params;
    channelAdd.ChannelId = id;

    return lora_phy->add_channel(&channelAdd);
}

LoRaMacStatus_t LoRaMac::LoRaMacChannelRemove( uint8_t id )
{
    ChannelRemoveParams_t channelRemove;

    if( ( _params.LoRaMacState & LORAMAC_TX_RUNNING ) == LORAMAC_TX_RUNNING )
    {
        if( ( _params.LoRaMacState & LORAMAC_TX_CONFIG ) != LORAMAC_TX_CONFIG )
        {
            return LORAMAC_STATUS_BUSY;
        }
    }

    channelRemove.ChannelId = id;

    if(lora_phy->remove_channel(&channelRemove) == false)
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    lora_phy->put_radio_to_sleep();

    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t LoRaMac::LoRaMacMulticastChannelLink( MulticastParams_t *channelParam )
{
    if( channelParam == NULL )
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }
    if( ( _params.LoRaMacState & LORAMAC_TX_RUNNING ) == LORAMAC_TX_RUNNING )
    {
        return LORAMAC_STATUS_BUSY;
    }

    // Reset downlink counter
    channelParam->DownLinkCounter = 0;

    if( _params.MulticastChannels == NULL )
    {
        // New node is the fist element
        _params.MulticastChannels = channelParam;
    }
    else
    {
        MulticastParams_t *cur = _params.MulticastChannels;

        // Search the last node in the list
        while( cur->Next != NULL )
        {
            cur = cur->Next;
        }
        // This function always finds the last node
        cur->Next = channelParam;
    }

    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t LoRaMac::LoRaMacMulticastChannelUnlink( MulticastParams_t *channelParam )
{
    if( channelParam == NULL )
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }
    if( ( _params.LoRaMacState & LORAMAC_TX_RUNNING ) == LORAMAC_TX_RUNNING )
    {
        return LORAMAC_STATUS_BUSY;
    }

    if( _params.MulticastChannels != NULL )
    {
        if( _params.MulticastChannels == channelParam )
        {
          // First element
            _params.MulticastChannels = channelParam->Next;
        }
        else
        {
            MulticastParams_t *cur = _params.MulticastChannels;

            // Search the node in the list
            while( cur->Next && cur->Next != channelParam )
            {
                cur = cur->Next;
            }
            // If we found the node, remove it
            if( cur->Next )
            {
                cur->Next = channelParam->Next;
            }
        }
        channelParam->Next = NULL;
    }

    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t LoRaMac::LoRaMacMlmeRequest( MlmeReq_t *mlmeRequest )
{
    return mlme.set_request(mlmeRequest, &_params);
}

LoRaMacStatus_t LoRaMac::LoRaMacMcpsRequest( McpsReq_t *mcpsRequest )
{
    return mcps.set_request(mcpsRequest, &_params);
}

LoRaMacStatus_t LoRaMac::LoRaMacMibGetRequestConfirm( MibRequestConfirm_t *mibGet )
{
    return mib.get_request(mibGet, &_params);
}

LoRaMacStatus_t LoRaMac::LoRaMacMibSetRequestConfirm( MibRequestConfirm_t *mibSet )
{
    return mib.set_request(mibSet, &_params);
}

radio_events_t *LoRaMac::GetPhyEventHandlers()
{
    RadioEvents.tx_done = mbed::callback(this, &LoRaMac::handle_tx_done);
    RadioEvents.rx_done = mbed::callback(this, &LoRaMac::handle_rx_done);
    RadioEvents.rx_error = mbed::callback(this, &LoRaMac::handle_rx_error);
    RadioEvents.tx_timeout = mbed::callback(this, &LoRaMac::handle_tx_timeout);
    RadioEvents.rx_timeout = mbed::callback(this, &LoRaMac::handle_rx_timeout);

    return &RadioEvents;
}

#if defined(LORAWAN_COMPLIANCE_TEST)
/***************************************************************************
 * Compliance testing                                                      *
 **************************************************************************/

LoRaMacStatus_t LoRaMac::LoRaMacSetTxTimer( uint32_t TxDutyCycleTime )
{
    _lora_time.TimerSetValue(&TxNextPacketTimer, TxDutyCycleTime);
    _lora_time.TimerStart(&TxNextPacketTimer);

    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t LoRaMac::LoRaMacStopTxTimer( )

{
    _lora_time.TimerStop(&TxNextPacketTimer);

    return LORAMAC_STATUS_OK;
}

void LoRaMac::LoRaMacTestRxWindowsOn( bool enable )
{
    _params.IsRxWindowsEnabled = enable;
}

void LoRaMac::LoRaMacTestSetMic( uint16_t txPacketCounter )
{
    _params.UpLinkCounter = txPacketCounter;
    _params.IsUpLinkCounterFixed = true;
}

void LoRaMac::LoRaMacTestSetDutyCycleOn( bool enable )
{
    VerifyParams_t verify;

    verify.DutyCycle = enable;

    if(lora_phy->verify(&verify, PHY_DUTY_CYCLE) == true)
    {
        _params.DutyCycleOn = enable;
    }
}

void LoRaMac::LoRaMacTestSetChannel( uint8_t channel )
{
    _params.Channel = channel;
}
#endif
