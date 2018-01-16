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

    memset(MacCommandsBuffer, 0, sizeof(MacCommandsBuffer));
    memset(MacCommandsBufferToRepeat, 0, sizeof(MacCommandsBufferToRepeat));
}

LoRaMacCommand::~LoRaMacCommand()
{
}

lorawan_status_t LoRaMacCommand::AddMacCommand(uint8_t cmd, uint8_t p1, uint8_t p2)
{
    lorawan_status_t status = LORAWAN_STATUS_BUSY;
    // The maximum buffer length must take MAC commands to re-send into account.
    const uint8_t bufLen = LORA_MAC_COMMAND_MAX_LENGTH - MacCommandsBufferToRepeatIndex;

    switch( cmd )
    {
        case MOTE_MAC_LINK_CHECK_REQ:
            if( MacCommandsBufferIndex < bufLen )
            {
                MacCommandsBuffer[MacCommandsBufferIndex++] = cmd;
                // No payload for this command
                status = LORAWAN_STATUS_OK;
            }
            break;
        case MOTE_MAC_LINK_ADR_ANS:
            if( MacCommandsBufferIndex < ( bufLen - 1 ) )
            {
                MacCommandsBuffer[MacCommandsBufferIndex++] = cmd;
                // Margin
                MacCommandsBuffer[MacCommandsBufferIndex++] = p1;
                status = LORAWAN_STATUS_OK;
            }
            break;
        case MOTE_MAC_DUTY_CYCLE_ANS:
            if( MacCommandsBufferIndex < bufLen )
            {
                MacCommandsBuffer[MacCommandsBufferIndex++] = cmd;
                // No payload for this answer
                status = LORAWAN_STATUS_OK;
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
                status = LORAWAN_STATUS_OK;
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
                status = LORAWAN_STATUS_OK;
            }
            break;
        case MOTE_MAC_NEW_CHANNEL_ANS:
            if( MacCommandsBufferIndex < ( bufLen - 1 ) )
            {
                MacCommandsBuffer[MacCommandsBufferIndex++] = cmd;
                // Status: Datarate range OK, Channel frequency OK
                MacCommandsBuffer[MacCommandsBufferIndex++] = p1;
                status = LORAWAN_STATUS_OK;
            }
            break;
        case MOTE_MAC_RX_TIMING_SETUP_ANS:
            if( MacCommandsBufferIndex < bufLen )
            {
                MacCommandsBuffer[MacCommandsBufferIndex++] = cmd;
                // No payload for this answer
                // This is a sticky MAC command answer. Setup indication
                _lora_mac.SetMlmeScheduleUplinkIndication();
                status = LORAWAN_STATUS_OK;
            }
            break;
        case MOTE_MAC_TX_PARAM_SETUP_ANS:
            if( MacCommandsBufferIndex < bufLen )
            {
                MacCommandsBuffer[MacCommandsBufferIndex++] = cmd;
                // No payload for this answer
                status = LORAWAN_STATUS_OK;
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
                status = LORAWAN_STATUS_OK;
            }
            break;
        default:
            return LORAWAN_STATUS_SERVICE_UNKNOWN;
    }
    if( status == LORAWAN_STATUS_OK )
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

lorawan_status_t LoRaMacCommand::ProcessMacCommands(uint8_t *payload, uint8_t macIndex,
                                                    uint8_t commandsSize, uint8_t snr,
                                                    loramac_mlme_confirm_t& MlmeConfirm,
                                                    lora_mac_system_params_t &LoRaMacParams,
                                                    LoRaPHY &lora_phy)
{
    uint8_t status = 0;
    lorawan_status_t ret_value = LORAWAN_STATUS_OK;

    while( macIndex < commandsSize )
    {
        // Decode Frame MAC commands
        switch( payload[macIndex++] )
        {
            case SRV_MAC_LINK_CHECK_ANS:
                MlmeConfirm.status = LORAMAC_EVENT_INFO_STATUS_OK;
                MlmeConfirm.demod_margin = payload[macIndex++];
                MlmeConfirm.nb_gateways = payload[macIndex++];
                break;
            case SRV_MAC_LINK_ADR_REQ:
                {
                    adr_req_params_t linkAdrReq;
                    int8_t linkAdrDatarate = DR_0;
                    int8_t linkAdrTxPower = TX_POWER_0;
                    uint8_t linkAdrNbRep = 0;
                    uint8_t linkAdrNbBytesParsed = 0;

                    // Fill parameter structure
                    linkAdrReq.payload = &payload[macIndex - 1];
                    linkAdrReq.payload_size = commandsSize - ( macIndex - 1 );
                    linkAdrReq.adr_enabled = LoRaMacParams.adr_on;
                    linkAdrReq.ul_dwell_time = LoRaMacParams.uplink_dwell_time;
                    linkAdrReq.current_datarate = LoRaMacParams.channel_data_rate;
                    linkAdrReq.current_tx_power = LoRaMacParams.channel_tx_power;
                    linkAdrReq.current_nb_rep = LoRaMacParams.retry_num;

                    // Process the ADR requests
                    status = lora_phy.link_ADR_request(&linkAdrReq, &linkAdrDatarate,
                                                       &linkAdrTxPower, &linkAdrNbRep,
                                                       &linkAdrNbBytesParsed);

                    if( ( status & 0x07 ) == 0x07 )
                    {
                        LoRaMacParams.channel_data_rate = linkAdrDatarate;
                        LoRaMacParams.channel_tx_power = linkAdrTxPower;
                        LoRaMacParams.retry_num = linkAdrNbRep;
                    }

                    // Add the answers to the buffer
                    for( uint8_t i = 0; i < ( linkAdrNbBytesParsed / 5 ); i++ )
                    {
                        ret_value = AddMacCommand( MOTE_MAC_LINK_ADR_ANS, status, 0 );
                    }
                    // Update MAC index
                    macIndex += linkAdrNbBytesParsed - 1;
                }
                break;
            case SRV_MAC_DUTY_CYCLE_REQ:
                LoRaMacParams.max_duty_cycle = payload[macIndex++];
                LoRaMacParams.aggregated_duty_cycle = 1 << LoRaMacParams.max_duty_cycle;
                ret_value = AddMacCommand( MOTE_MAC_DUTY_CYCLE_ANS, 0, 0 );
                break;
            case SRV_MAC_RX_PARAM_SETUP_REQ:
                {
                    rx_param_setup_req_t rxParamSetupReq;

                    rxParamSetupReq.dr_offset = ( payload[macIndex] >> 4 ) & 0x07;
                    rxParamSetupReq.datarate = payload[macIndex] & 0x0F;
                    macIndex++;

                    rxParamSetupReq.frequency =  ( uint32_t )payload[macIndex++];
                    rxParamSetupReq.frequency |= ( uint32_t )payload[macIndex++] << 8;
                    rxParamSetupReq.frequency |= ( uint32_t )payload[macIndex++] << 16;
                    rxParamSetupReq.frequency *= 100;

                    // Perform request on region
                    status = lora_phy.accept_rx_param_setup_req(&rxParamSetupReq);

                    if( ( status & 0x07 ) == 0x07 )
                    {
                        LoRaMacParams.rx2_channel.datarate = rxParamSetupReq.datarate;
                        LoRaMacParams.rx2_channel.frequency = rxParamSetupReq.frequency;
                        LoRaMacParams.rx1_dr_offset = rxParamSetupReq.dr_offset;
                    }
                    ret_value = AddMacCommand( MOTE_MAC_RX_PARAM_SETUP_ANS, status, 0 );
                }
                break;
            case SRV_MAC_DEV_STATUS_REQ:
                {
                    uint8_t batteryLevel = BAT_LEVEL_NO_MEASURE;
                    // we don't have a mechanism at the moment to measure
                    // battery levels
                    ret_value = AddMacCommand( MOTE_MAC_DEV_STATUS_ANS, batteryLevel, snr );
                    break;
                }
            case SRV_MAC_NEW_CHANNEL_REQ:
                {
                    new_channel_req_params_t newChannelReq;
                    channel_params_t chParam;

                    newChannelReq.channel_id = payload[macIndex++];
                    newChannelReq.new_channel = &chParam;

                    chParam.frequency = ( uint32_t )payload[macIndex++];
                    chParam.frequency |= ( uint32_t )payload[macIndex++] << 8;
                    chParam.frequency |= ( uint32_t )payload[macIndex++] << 16;
                    chParam.frequency *= 100;
                    chParam.rx1_frequency = 0;
                    chParam.dr_range.value = payload[macIndex++];

                    status = lora_phy.request_new_channel(&newChannelReq);

                    ret_value = AddMacCommand( MOTE_MAC_NEW_CHANNEL_ANS, status, 0 );
                }
                break;
            case SRV_MAC_RX_TIMING_SETUP_REQ:
                {
                    uint8_t delay = payload[macIndex++] & 0x0F;

                    if( delay == 0 )
                    {
                        delay++;
                    }
                    LoRaMacParams.recv_delay1 = delay * 1000;
                    LoRaMacParams.recv_delay2 = LoRaMacParams.recv_delay1 + 1000;
                    ret_value = AddMacCommand( MOTE_MAC_RX_TIMING_SETUP_ANS, 0, 0 );
                }
                break;
            case SRV_MAC_TX_PARAM_SETUP_REQ:
                {
                    tx_param_setup_req_t txParamSetupReq;
                    uint8_t eirpDwellTime = payload[macIndex++];

                    txParamSetupReq.ul_dwell_time = 0;
                    txParamSetupReq.dl_dwell_time = 0;

                    if( ( eirpDwellTime & 0x20 ) == 0x20 )
                    {
                        txParamSetupReq.dl_dwell_time = 1;
                    }
                    if( ( eirpDwellTime & 0x10 ) == 0x10 )
                    {
                        txParamSetupReq.ul_dwell_time = 1;
                    }
                    txParamSetupReq.max_eirp = eirpDwellTime & 0x0F;

                    // Check the status for correctness
                    if(lora_phy.accept_tx_param_setup_req(&txParamSetupReq))
                    {
                        // Accept command
                        LoRaMacParams.uplink_dwell_time = txParamSetupReq.ul_dwell_time;
                        LoRaMacParams.downlink_dwell_time = txParamSetupReq.dl_dwell_time;
                        LoRaMacParams.max_eirp = LoRaMacMaxEirpTable[txParamSetupReq.max_eirp];
                        // Add command response
                        ret_value = AddMacCommand( MOTE_MAC_TX_PARAM_SETUP_ANS, 0, 0 );
                    }
                }
                break;
            case SRV_MAC_DL_CHANNEL_REQ:
                {
                    dl_channel_req_params_t dlChannelReq;

                    dlChannelReq.channel_id = payload[macIndex++];
                    dlChannelReq.rx1_frequency = ( uint32_t )payload[macIndex++];
                    dlChannelReq.rx1_frequency |= ( uint32_t )payload[macIndex++] << 8;
                    dlChannelReq.rx1_frequency |= ( uint32_t )payload[macIndex++] << 16;
                    dlChannelReq.rx1_frequency *= 100;

                    status = lora_phy.dl_channel_request(&dlChannelReq);

                    ret_value = AddMacCommand( MOTE_MAC_DL_CHANNEL_ANS, status, 0 );
                }
                break;
            default:
                // Unknown command. ABORT MAC commands processing
                ret_value = LORAWAN_STATUS_UNSUPPORTED;
        }
    }
    return ret_value;
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
