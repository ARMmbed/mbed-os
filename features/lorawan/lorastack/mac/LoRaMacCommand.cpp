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
#include "LoRaMacCommand.h"
#include "LoRaMac.h"

#if defined(FEATURE_COMMON_PAL)
#include "mbed_trace.h"
#define TRACE_GROUP "LMACC"
#else
#define tr_debug(...) (void(0)) //dummies if feature common pal is not added
#define tr_info(...)  (void(0)) //dummies if feature common pal is not added
#define tr_error(...) (void(0)) //dummies if feature common pal is not added
#endif //defined(FEATURE_COMMON_PAL)

/*!
 * LoRaMAC max EIRP (dBm) table.
 */
static const uint8_t LoRaMacMaxEirpTable[] = { 8, 10, 12, 13, 14, 16, 18, 20, 21, 24, 26, 27, 29, 30, 33, 36 };


LoRaMacCommand::LoRaMacCommand(LoRaMac& lora_mac)
    : _lora_mac(lora_mac)
{
    MacCommandsInNextTx = false;
    MacCommandsBufferIndex = 0;
    MacCommandsBufferToRepeatIndex = 0;
    //uint8_t MacCommandsBuffer[LORA_MAC_COMMAND_MAX_LENGTH];
    //uint8_t MacCommandsBufferToRepeat[LORA_MAC_COMMAND_MAX_LENGTH];
}

LoRaMacCommand::~LoRaMacCommand()
{
}

LoRaMacStatus_t LoRaMacCommand::AddMacCommand(uint8_t cmd, uint8_t p1, uint8_t p2)
{
    LoRaMacStatus_t status = LORAMAC_STATUS_BUSY;
    // The maximum buffer length must take MAC commands to re-send into account.
    const uint8_t bufLen = LORA_MAC_COMMAND_MAX_LENGTH - MacCommandsBufferToRepeatIndex;

    switch( cmd )
    {
        case MOTE_MAC_LINK_CHECK_REQ:
            if( MacCommandsBufferIndex < bufLen )
            {
                MacCommandsBuffer[MacCommandsBufferIndex++] = cmd;
                // No payload for this command
                status = LORAMAC_STATUS_OK;
            }
            break;
        case MOTE_MAC_LINK_ADR_ANS:
            if( MacCommandsBufferIndex < ( bufLen - 1 ) )
            {
                MacCommandsBuffer[MacCommandsBufferIndex++] = cmd;
                // Margin
                MacCommandsBuffer[MacCommandsBufferIndex++] = p1;
                status = LORAMAC_STATUS_OK;
            }
            break;
        case MOTE_MAC_DUTY_CYCLE_ANS:
            if( MacCommandsBufferIndex < bufLen )
            {
                MacCommandsBuffer[MacCommandsBufferIndex++] = cmd;
                // No payload for this answer
                status = LORAMAC_STATUS_OK;
            }
            break;
        case MOTE_MAC_RX_PARAM_SETUP_ANS:
            if( MacCommandsBufferIndex < ( bufLen - 1 ) )
            {
                MacCommandsBuffer[MacCommandsBufferIndex++] = cmd;
                // Status: Datarate ACK, Channel ACK
                MacCommandsBuffer[MacCommandsBufferIndex++] = p1;
                // This is a sticky MAC command answer. Setup indication
                _lora_mac.SetMlmeScheduleUplinkIndication();
                status = LORAMAC_STATUS_OK;
            }
            break;
        case MOTE_MAC_DEV_STATUS_ANS:
            if( MacCommandsBufferIndex < ( bufLen - 2 ) )
            {
                MacCommandsBuffer[MacCommandsBufferIndex++] = cmd;
                // 1st byte Battery
                // 2nd byte Margin
                MacCommandsBuffer[MacCommandsBufferIndex++] = p1;
                MacCommandsBuffer[MacCommandsBufferIndex++] = p2;
                status = LORAMAC_STATUS_OK;
            }
            break;
        case MOTE_MAC_NEW_CHANNEL_ANS:
            if( MacCommandsBufferIndex < ( bufLen - 1 ) )
            {
                MacCommandsBuffer[MacCommandsBufferIndex++] = cmd;
                // Status: Datarate range OK, Channel frequency OK
                MacCommandsBuffer[MacCommandsBufferIndex++] = p1;
                status = LORAMAC_STATUS_OK;
            }
            break;
        case MOTE_MAC_RX_TIMING_SETUP_ANS:
            if( MacCommandsBufferIndex < bufLen )
            {
                MacCommandsBuffer[MacCommandsBufferIndex++] = cmd;
                // No payload for this answer
                // This is a sticky MAC command answer. Setup indication
                _lora_mac.SetMlmeScheduleUplinkIndication();
                status = LORAMAC_STATUS_OK;
            }
            break;
        case MOTE_MAC_TX_PARAM_SETUP_ANS:
            if( MacCommandsBufferIndex < bufLen )
            {
                MacCommandsBuffer[MacCommandsBufferIndex++] = cmd;
                // No payload for this answer
                status = LORAMAC_STATUS_OK;
            }
            break;
        case MOTE_MAC_DL_CHANNEL_ANS:
            if( MacCommandsBufferIndex < bufLen )
            {
                MacCommandsBuffer[MacCommandsBufferIndex++] = cmd;
                // Status: Uplink frequency exists, Channel frequency OK
                MacCommandsBuffer[MacCommandsBufferIndex++] = p1;
                // This is a sticky MAC command answer. Setup indication
                _lora_mac.SetMlmeScheduleUplinkIndication();
                status = LORAMAC_STATUS_OK;
            }
            break;
        default:
            return LORAMAC_STATUS_SERVICE_UNKNOWN;
    }
    if( status == LORAMAC_STATUS_OK )
    {
        MacCommandsInNextTx = true;
    }
    return status;
}

void LoRaMacCommand::ClearCommandBuffer()
{
    MacCommandsBufferIndex = 0;
}

uint8_t LoRaMacCommand::GetLength() const
{
    return MacCommandsBufferIndex;
}

uint8_t *LoRaMacCommand::GetMacCommandsBuffer()
{
    return MacCommandsBuffer;
}

void LoRaMacCommand::ParseMacCommandsToRepeat()
{
    uint8_t i = 0;
    uint8_t cmdCount = 0;

    for( i = 0; i < MacCommandsBufferIndex; i++ )
    {
        switch( MacCommandsBuffer[i] )
        {
            // STICKY
            case MOTE_MAC_DL_CHANNEL_ANS:
            case MOTE_MAC_RX_PARAM_SETUP_ANS:
            { // 1 byte payload
                MacCommandsBufferToRepeat[cmdCount++] = MacCommandsBuffer[i++];
                MacCommandsBufferToRepeat[cmdCount++] = MacCommandsBuffer[i];
                break;
            }
            case MOTE_MAC_RX_TIMING_SETUP_ANS:
            { // 0 byte payload
                MacCommandsBufferToRepeat[cmdCount++] = MacCommandsBuffer[i];
                break;
            }
            // NON-STICKY
            case MOTE_MAC_DEV_STATUS_ANS:
            { // 2 bytes payload
                i += 2;
                break;
            }
            case MOTE_MAC_LINK_ADR_ANS:
            case MOTE_MAC_NEW_CHANNEL_ANS:
            { // 1 byte payload
                i++;
                break;
            }
            case MOTE_MAC_TX_PARAM_SETUP_ANS:
            case MOTE_MAC_DUTY_CYCLE_ANS:
            case MOTE_MAC_LINK_CHECK_REQ:
            { // 0 byte payload
                break;
            }
            default:
                break;
        }
    }

    if( cmdCount > 0 ) {
        MacCommandsInNextTx = true;
    } else {
        MacCommandsInNextTx = false;
    }
}


void LoRaMacCommand::ClearRepeatBuffer()
{
    MacCommandsBufferToRepeatIndex = 0;
}

void LoRaMacCommand::CopyRepeatCommandsToBuffer()
{
    // Copy the MAC commands which must be re-send into the MAC command buffer
    memcpy(&MacCommandsBuffer[MacCommandsBufferIndex], MacCommandsBufferToRepeat, MacCommandsBufferToRepeatIndex);
    MacCommandsBufferIndex += MacCommandsBufferToRepeatIndex;
}

uint8_t LoRaMacCommand::GetRepeatLength() const
{
    return MacCommandsBufferToRepeatIndex;
}

void LoRaMacCommand::ClearMacCommandsInNextTx()
{
    MacCommandsInNextTx = false;
}

bool LoRaMacCommand::IsMacCommandsInNextTx() const
{
    return MacCommandsInNextTx;
}

void LoRaMacCommand::ProcessMacCommands(uint8_t *payload, uint8_t macIndex, uint8_t commandsSize, uint8_t snr,
                                        MlmeConfirm_t &MlmeConfirm, LoRaMacCallback_t *LoRaMacCallbacks,
                                        LoRaMacParams_t &LoRaMacParams, LoRaPHY &lora_phy)
{
    uint8_t status = 0;

    while( macIndex < commandsSize )
    {
        // Decode Frame MAC commands
        switch( payload[macIndex++] )
        {
            case SRV_MAC_LINK_CHECK_ANS:
                MlmeConfirm.Status = LORAMAC_EVENT_INFO_STATUS_OK;
                MlmeConfirm.DemodMargin = payload[macIndex++];
                MlmeConfirm.NbGateways = payload[macIndex++];
                break;
            case SRV_MAC_LINK_ADR_REQ:
                {
                    LinkAdrReqParams_t linkAdrReq;
                    int8_t linkAdrDatarate = DR_0;
                    int8_t linkAdrTxPower = TX_POWER_0;
                    uint8_t linkAdrNbRep = 0;
                    uint8_t linkAdrNbBytesParsed = 0;

                    // Fill parameter structure
                    linkAdrReq.Payload = &payload[macIndex - 1];
                    linkAdrReq.PayloadSize = commandsSize - ( macIndex - 1 );
                    linkAdrReq.AdrEnabled = LoRaMacParams.AdrCtrlOn;
                    linkAdrReq.UplinkDwellTime = LoRaMacParams.UplinkDwellTime;
                    linkAdrReq.CurrentDatarate = LoRaMacParams.ChannelsDatarate;
                    linkAdrReq.CurrentTxPower = LoRaMacParams.ChannelsTxPower;
                    linkAdrReq.CurrentNbRep = LoRaMacParams.ChannelsNbRep;

                    // Process the ADR requests
                    status = lora_phy.link_ADR_request(&linkAdrReq, &linkAdrDatarate,
                                                       &linkAdrTxPower, &linkAdrNbRep,
                                                       &linkAdrNbBytesParsed);

                    if( ( status & 0x07 ) == 0x07 )
                    {
                        LoRaMacParams.ChannelsDatarate = linkAdrDatarate;
                        LoRaMacParams.ChannelsTxPower = linkAdrTxPower;
                        LoRaMacParams.ChannelsNbRep = linkAdrNbRep;
                    }

                    // Add the answers to the buffer
                    for( uint8_t i = 0; i < ( linkAdrNbBytesParsed / 5 ); i++ )
                    {
                        AddMacCommand( MOTE_MAC_LINK_ADR_ANS, status, 0 );
                    }
                    // Update MAC index
                    macIndex += linkAdrNbBytesParsed - 1;
                }
                break;
            case SRV_MAC_DUTY_CYCLE_REQ:
                LoRaMacParams.MaxDCycle = payload[macIndex++];
                LoRaMacParams.AggregatedDCycle = 1 << LoRaMacParams.MaxDCycle;
                AddMacCommand( MOTE_MAC_DUTY_CYCLE_ANS, 0, 0 );
                break;
            case SRV_MAC_RX_PARAM_SETUP_REQ:
                {
                    RxParamSetupReqParams_t rxParamSetupReq;
                    status = 0x07;

                    rxParamSetupReq.DrOffset = ( payload[macIndex] >> 4 ) & 0x07;
                    rxParamSetupReq.Datarate = payload[macIndex] & 0x0F;
                    macIndex++;

                    rxParamSetupReq.Frequency =  ( uint32_t )payload[macIndex++];
                    rxParamSetupReq.Frequency |= ( uint32_t )payload[macIndex++] << 8;
                    rxParamSetupReq.Frequency |= ( uint32_t )payload[macIndex++] << 16;
                    rxParamSetupReq.Frequency *= 100;

                    // Perform request on region
                    status = lora_phy.setup_rx_params(&rxParamSetupReq);

                    if( ( status & 0x07 ) == 0x07 )
                    {
                        LoRaMacParams.Rx2Channel.Datarate = rxParamSetupReq.Datarate;
                        LoRaMacParams.Rx2Channel.Frequency = rxParamSetupReq.Frequency;
                        LoRaMacParams.Rx1DrOffset = rxParamSetupReq.DrOffset;
                    }
                    AddMacCommand( MOTE_MAC_RX_PARAM_SETUP_ANS, status, 0 );
                }
                break;
            case SRV_MAC_DEV_STATUS_REQ:
                {
                    uint8_t batteryLevel = BAT_LEVEL_NO_MEASURE;
                    if( ( LoRaMacCallbacks != NULL ) && ( LoRaMacCallbacks->GetBatteryLevel != NULL ) )
                    {
                        batteryLevel = LoRaMacCallbacks->GetBatteryLevel( );
                    }
                    AddMacCommand( MOTE_MAC_DEV_STATUS_ANS, batteryLevel, snr );
                    break;
                }
            case SRV_MAC_NEW_CHANNEL_REQ:
                {
                    NewChannelReqParams_t newChannelReq;
                    ChannelParams_t chParam;
                    status = 0x03;

                    newChannelReq.ChannelId = payload[macIndex++];
                    newChannelReq.NewChannel = &chParam;

                    chParam.Frequency = ( uint32_t )payload[macIndex++];
                    chParam.Frequency |= ( uint32_t )payload[macIndex++] << 8;
                    chParam.Frequency |= ( uint32_t )payload[macIndex++] << 16;
                    chParam.Frequency *= 100;
                    chParam.Rx1Frequency = 0;
                    chParam.DrRange.Value = payload[macIndex++];

                    status = lora_phy.request_new_channel(&newChannelReq);

                    AddMacCommand( MOTE_MAC_NEW_CHANNEL_ANS, status, 0 );
                }
                break;
            case SRV_MAC_RX_TIMING_SETUP_REQ:
                {
                    uint8_t delay = payload[macIndex++] & 0x0F;

                    if( delay == 0 )
                    {
                        delay++;
                    }
                    LoRaMacParams.ReceiveDelay1 = delay * 1000;
                    LoRaMacParams.ReceiveDelay2 = LoRaMacParams.ReceiveDelay1 + 1000;
                    AddMacCommand( MOTE_MAC_RX_TIMING_SETUP_ANS, 0, 0 );
                }
                break;
            case SRV_MAC_TX_PARAM_SETUP_REQ:
                {
                    TxParamSetupReqParams_t txParamSetupReq;
                    uint8_t eirpDwellTime = payload[macIndex++];

                    txParamSetupReq.UplinkDwellTime = 0;
                    txParamSetupReq.DownlinkDwellTime = 0;

                    if( ( eirpDwellTime & 0x20 ) == 0x20 )
                    {
                        txParamSetupReq.DownlinkDwellTime = 1;
                    }
                    if( ( eirpDwellTime & 0x10 ) == 0x10 )
                    {
                        txParamSetupReq.UplinkDwellTime = 1;
                    }
                    txParamSetupReq.MaxEirp = eirpDwellTime & 0x0F;

                    // Check the status for correctness
                    if( lora_phy.setup_tx_params(&txParamSetupReq ) != -1 )
                    {
                        // Accept command
                        LoRaMacParams.UplinkDwellTime = txParamSetupReq.UplinkDwellTime;
                        LoRaMacParams.DownlinkDwellTime = txParamSetupReq.DownlinkDwellTime;
                        LoRaMacParams.MaxEirp = LoRaMacMaxEirpTable[txParamSetupReq.MaxEirp];
                        // Add command response
                        AddMacCommand( MOTE_MAC_TX_PARAM_SETUP_ANS, 0, 0 );
                    }
                }
                break;
            case SRV_MAC_DL_CHANNEL_REQ:
                {
                    DlChannelReqParams_t dlChannelReq;
                    status = 0x03;

                    dlChannelReq.ChannelId = payload[macIndex++];
                    dlChannelReq.Rx1Frequency = ( uint32_t )payload[macIndex++];
                    dlChannelReq.Rx1Frequency |= ( uint32_t )payload[macIndex++] << 8;
                    dlChannelReq.Rx1Frequency |= ( uint32_t )payload[macIndex++] << 16;
                    dlChannelReq.Rx1Frequency *= 100;

                    status = lora_phy.dl_channel_request(&dlChannelReq);

                    AddMacCommand( MOTE_MAC_DL_CHANNEL_ANS, status, 0 );
                }
                break;
            default:
                // Unknown command. ABORT MAC commands processing
                return;
        }
    }
}

bool LoRaMacCommand::IsStickyMacCommandPending()
{
    if( MacCommandsBufferToRepeatIndex > 0 )
    {
        // Sticky MAC commands pending
        return true;
    }
    return false;
}
