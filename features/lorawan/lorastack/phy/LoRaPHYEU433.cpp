/**
 *  @file LoRaPHYEU433.cpp
 *
 *  @brief Implements LoRaPHY for European 433 MHz band
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

#include "LoRaPHYEU433.h"

#include "lora_phy_ds.h"
#include "LoRaRadio.h"

/*!
 * Number of default channels
 */
#define EU433_NUMB_DEFAULT_CHANNELS                 3

/*!
 * Number of channels to apply for the CF list
 */
#define EU433_NUMB_CHANNELS_CF_LIST                 5

/*!
 * Minimal datarate that can be used by the node
 */
#define EU433_TX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define EU433_TX_MAX_DATARATE                       DR_7

/*!
 * Minimal datarate that can be used by the node
 */
#define EU433_RX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define EU433_RX_MAX_DATARATE                       DR_7

/*!
 * Default datarate used by the node
 */
#define EU433_DEFAULT_DATARATE                      DR_0

/*!
 * Minimal Rx1 receive datarate offset
 */
#define EU433_MIN_RX1_DR_OFFSET                     0

/*!
 * Maximal Rx1 receive datarate offset
 */
#define EU433_MAX_RX1_DR_OFFSET                     5

/*!
 * Default Rx1 receive datarate offset
 */
#define EU433_DEFAULT_RX1_DR_OFFSET                 0

/*!
 * Minimal Tx output power that can be used by the node
 */
#define EU433_MIN_TX_POWER                          TX_POWER_5

/*!
 * Maximal Tx output power that can be used by the node
 */
#define EU433_MAX_TX_POWER                          TX_POWER_0

/*!
 * Default Tx output power used by the node
 */
#define EU433_DEFAULT_TX_POWER                      TX_POWER_0

/*!
 * Default Max EIRP
 */
#define EU433_DEFAULT_MAX_EIRP                      12.15f

/*!
 * Default antenna gain
 */
#define EU433_DEFAULT_ANTENNA_GAIN                  2.15f

/*!
 * ADR Ack limit
 */
#define EU433_ADR_ACK_LIMIT                         64

/*!
 * ADR Ack delay
 */
#define EU433_ADR_ACK_DELAY                         32

/*!
 * Enabled or disabled the duty cycle
 */
#define EU433_DUTY_CYCLE_ENABLED                    1

/*!
 * Maximum RX window duration
 */
#define EU433_MAX_RX_WINDOW                         3000

/*!
 * Receive delay 1
 */
#define EU433_RECEIVE_DELAY1                        1000

/*!
 * Receive delay 2
 */
#define EU433_RECEIVE_DELAY2                        2000

/*!
 * Join accept delay 1
 */
#define EU433_JOIN_ACCEPT_DELAY1                    5000

/*!
 * Join accept delay 2
 */
#define EU433_JOIN_ACCEPT_DELAY2                    6000

/*!
 * Maximum frame counter gap
 */
#define EU433_MAX_FCNT_GAP                          16384

/*!
 * Ack timeout
 */
#define EU433_ACKTIMEOUT                            2000

/*!
 * Random ack timeout limits
 */
#define EU433_ACK_TIMEOUT_RND                       1000

/*!
 * Verification of default datarate
 */
#if ( EU433_DEFAULT_DATARATE > DR_5 )
#error "A default DR higher than DR_5 may lead to connectivity loss."
#endif

/*!
 * Second reception window channel frequency definition.
 */
#define EU433_RX_WND_2_FREQ                         434665000

/*!
 * Second reception window channel datarate definition.
 */
#define EU433_RX_WND_2_DR                           DR_0

/*!
 * Band 0 definition
 * { DutyCycle, TxMaxPower, LastTxDoneTime, TimeOff }
 */
#define EU433_BAND0                                 { 100, EU433_MAX_TX_POWER, 0,  0 } //  1.0 %

/*!
 * LoRaMac default channel 1
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
#define EU433_LC1                                   { 433175000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 }

/*!
 * LoRaMac default channel 2
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
#define EU433_LC2                                   { 433375000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 }

/*!
 * LoRaMac default channel 3
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
#define EU433_LC3                                   { 433575000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 }

/*!
 * LoRaMac channels which are allowed for the join procedure
 */
#define EU433_JOIN_CHANNELS                         ( uint16_t )( LC( 1 ) | LC( 2 ) | LC( 3 ) )

/*!
 * Data rates table definition
 */
static const uint8_t DataratesEU433[] = { 12, 11, 10,  9,  8,  7,  7, 50 };

/*!
 * Bandwidths table definition in Hz
 */
static const uint32_t BandwidthsEU433[] = { 125000, 125000, 125000, 125000, 125000, 125000, 250000, 0 };

/*!
 * Maximum payload with respect to the datarate index. Cannot operate with repeater.
 */
static const uint8_t MaxPayloadOfDatarateEU433[] = { 51, 51, 51, 115, 242, 242, 242, 242 };

/*!
 * Maximum payload with respect to the datarate index. Can operate with repeater.
 */
static const uint8_t MaxPayloadOfDatarateRepeaterEU433[] = { 51, 51, 51, 115, 222, 222, 222, 222 };


// Static functions
static int8_t GetNextLowerTxDr( int8_t dr, int8_t minDr )
{
    uint8_t nextLowerDr = 0;

    if( dr == minDr )
    {
        nextLowerDr = minDr;
    }
    else
    {
        nextLowerDr = dr - 1;
    }
    return nextLowerDr;
}

static uint32_t GetBandwidth( uint32_t drIndex )
{
    switch( BandwidthsEU433[drIndex] )
    {
        default:
        case 125000:
            return 0;
        case 250000:
            return 1;
        case 500000:
            return 2;
    }
}

static int8_t LimitTxPower( int8_t txPower, int8_t maxBandTxPower, int8_t datarate, uint16_t* channelsMask )
{
    int8_t txPowerResult = txPower;

    // Limit tx power to the band max
    txPowerResult =  MAX( txPower, maxBandTxPower );

    return txPowerResult;
}

static bool VerifyTxFreq( uint32_t freq, LoRaRadio *radio )
{
    // Check radio driver support
    if(radio->check_rf_frequency(freq) == false )
    {
        return false;
    }

    if( ( freq < 433175000 ) || ( freq > 434665000 ) )
    {
        return false;
    }
    return true;
}

uint8_t LoRaPHYEU433::CountNbOfEnabledChannels( bool joined, uint8_t datarate, uint16_t* channelsMask, ChannelParams_t* channels, Band_t* bands, uint8_t* enabledChannels, uint8_t* delayTx )
{
    uint8_t nbEnabledChannels = 0;
    uint8_t delayTransmission = 0;

    for( uint8_t i = 0, k = 0; i < EU433_MAX_NB_CHANNELS; i += 16, k++ )
    {
        for( uint8_t j = 0; j < 16; j++ )
        {
            if( ( channelsMask[k] & ( 1 << j ) ) != 0 )
            {
                if( channels[i + j].Frequency == 0 )
                { // Check if the channel is enabled
                    continue;
                }
                if( joined == false )
                {
                    if( ( EU433_JOIN_CHANNELS & ( 1 << j ) ) == 0 )
                    {
                        continue;
                    }
                }
                if( val_in_range( datarate, channels[i + j].DrRange.Fields.Min,
                                              channels[i + j].DrRange.Fields.Max ) == 0 )
                { // Check if the current channel selection supports the given datarate
                    continue;
                }
                if( bands[channels[i + j].Band].TimeOff > 0 )
                { // Check if the band is available for transmission
                    delayTransmission++;
                    continue;
                }
                enabledChannels[nbEnabledChannels++] = i + j;
            }
        }
    }

    *delayTx = delayTransmission;
    return nbEnabledChannels;
}

LoRaPHYEU433::LoRaPHYEU433()
{
    const Band_t band0 = EU433_BAND0;
    Bands[0] = band0;
}

LoRaPHYEU433::~LoRaPHYEU433()
{
}

PhyParam_t LoRaPHYEU433::get_phy_params(GetPhyParams_t* getPhy)
{
    PhyParam_t phyParam = { 0 };

    switch( getPhy->Attribute )
    {
        case PHY_MIN_RX_DR:
        {
            phyParam.Value = EU433_RX_MIN_DATARATE;
            break;
        }
        case PHY_MIN_TX_DR:
        {
            phyParam.Value = EU433_TX_MIN_DATARATE;
            break;
        }
        case PHY_DEF_TX_DR:
        {
            phyParam.Value = EU433_DEFAULT_DATARATE;
            break;
        }
        case PHY_NEXT_LOWER_TX_DR:
        {
            phyParam.Value = GetNextLowerTxDr( getPhy->Datarate, EU433_TX_MIN_DATARATE );
            break;
        }
        case PHY_DEF_TX_POWER:
        {
            phyParam.Value = EU433_DEFAULT_TX_POWER;
            break;
        }
        case PHY_MAX_PAYLOAD:
        {
            phyParam.Value = MaxPayloadOfDatarateEU433[getPhy->Datarate];
            break;
        }
        case PHY_MAX_PAYLOAD_REPEATER:
        {
            phyParam.Value = MaxPayloadOfDatarateRepeaterEU433[getPhy->Datarate];
            break;
        }
        case PHY_DUTY_CYCLE:
        {
            phyParam.Value = EU433_DUTY_CYCLE_ENABLED;
            break;
        }
        case PHY_MAX_RX_WINDOW:
        {
            phyParam.Value = EU433_MAX_RX_WINDOW;
            break;
        }
        case PHY_RECEIVE_DELAY1:
        {
            phyParam.Value = EU433_RECEIVE_DELAY1;
            break;
        }
        case PHY_RECEIVE_DELAY2:
        {
            phyParam.Value = EU433_RECEIVE_DELAY2;
            break;
        }
        case PHY_JOIN_ACCEPT_DELAY1:
        {
            phyParam.Value = EU433_JOIN_ACCEPT_DELAY1;
            break;
        }
        case PHY_JOIN_ACCEPT_DELAY2:
        {
            phyParam.Value = EU433_JOIN_ACCEPT_DELAY2;
            break;
        }
        case PHY_MAX_FCNT_GAP:
        {
            phyParam.Value = EU433_MAX_FCNT_GAP;
            break;
        }
        case PHY_ACK_TIMEOUT:
        {
            phyParam.Value = ( EU433_ACKTIMEOUT + get_random( -EU433_ACK_TIMEOUT_RND, EU433_ACK_TIMEOUT_RND ) );
            break;
        }
        case PHY_DEF_DR1_OFFSET:
        {
            phyParam.Value = EU433_DEFAULT_RX1_DR_OFFSET;
            break;
        }
        case PHY_DEF_RX2_FREQUENCY:
        {
            phyParam.Value = EU433_RX_WND_2_FREQ;
            break;
        }
        case PHY_DEF_RX2_DR:
        {
            phyParam.Value = EU433_RX_WND_2_DR;
            break;
        }
        case PHY_CHANNELS_MASK:
        {
            phyParam.ChannelsMask = ChannelsMask;
            break;
        }
        case PHY_CHANNELS_DEFAULT_MASK:
        {
            phyParam.ChannelsMask = ChannelsDefaultMask;
            break;
        }
        case PHY_MAX_NB_CHANNELS:
        {
            phyParam.Value = EU433_MAX_NB_CHANNELS;
            break;
        }
        case PHY_CHANNELS:
        {
            phyParam.Channels = Channels;
            break;
        }
        case PHY_DEF_UPLINK_DWELL_TIME:
        case PHY_DEF_DOWNLINK_DWELL_TIME:
        {
            phyParam.Value = 0;
            break;
        }
        case PHY_DEF_MAX_EIRP:
        {
            phyParam.fValue = EU433_DEFAULT_MAX_EIRP;
            break;
        }
        case PHY_DEF_ANTENNA_GAIN:
        {
            phyParam.fValue = EU433_DEFAULT_ANTENNA_GAIN;
            break;
        }
        case PHY_NB_JOIN_TRIALS:
        case PHY_DEF_NB_JOIN_TRIALS:
        {
            phyParam.Value = 48;
            break;
        }
        default:
        {
            break;
        }
    }

    return phyParam;
}

void LoRaPHYEU433::set_band_tx_done(SetBandTxDoneParams_t* txDone)
{
    set_last_tx_done( txDone->Joined, &Bands[Channels[txDone->Channel].Band], txDone->LastTxDoneTime );
}

void LoRaPHYEU433::load_defaults(InitType_t type)
{
    switch( type )
    {
        case INIT_TYPE_INIT:
        {
            // Channels
            const ChannelParams_t channel1 = EU433_LC1;
            const ChannelParams_t channel2 = EU433_LC2;
            const ChannelParams_t channel3 = EU433_LC3;
            Channels[0] = channel1;
            Channels[1] = channel2;
            Channels[2] = channel3;

            // Initialize the channels default mask
            ChannelsDefaultMask[0] = LC( 1 ) + LC( 2 ) + LC( 3 );
            // Update the channels mask
            copy_channel_mask( ChannelsMask, ChannelsDefaultMask, 1 );
            break;
        }
        case INIT_TYPE_RESTORE:
        {
            // Restore channels default mask
            ChannelsMask[0] |= ChannelsDefaultMask[0];
            break;
        }
        default:
        {
            break;
        }
    }
}

bool LoRaPHYEU433::verify(VerifyParams_t* verify, PhyAttribute_t phyAttribute)
{
    switch( phyAttribute )
    {
        case PHY_TX_DR:
        {
            return val_in_range( verify->DatarateParams.Datarate, EU433_TX_MIN_DATARATE, EU433_TX_MAX_DATARATE );
        }
        case PHY_DEF_TX_DR:
        {
            return val_in_range( verify->DatarateParams.Datarate, DR_0, DR_5 );
        }
        case PHY_RX_DR:
        {
            return val_in_range( verify->DatarateParams.Datarate, EU433_RX_MIN_DATARATE, EU433_RX_MAX_DATARATE );
        }
        case PHY_DEF_TX_POWER:
        case PHY_TX_POWER:
        {
            // Remark: switched min and max!
            return val_in_range( verify->TxPower, EU433_MAX_TX_POWER, EU433_MIN_TX_POWER );
        }
        case PHY_DUTY_CYCLE:
        {
            return EU433_DUTY_CYCLE_ENABLED;
        }
        case PHY_NB_JOIN_TRIALS:
        {
            if( verify->NbJoinTrials < 48 )
            {
                return false;
            }
            break;
        }
        default:
            return false;
    }
    return true;
}

void LoRaPHYEU433::apply_cf_list(ApplyCFListParams_t* applyCFList)
{
    ChannelParams_t newChannel;
    ChannelAddParams_t channelAdd;
    ChannelRemoveParams_t channelRemove;

    // Setup default datarate range
    newChannel.DrRange.Value = ( DR_5 << 4 ) | DR_0;

    // Size of the optional CF list
    if( applyCFList->Size != 16 )
    {
        return;
    }

    // Last byte is RFU, don't take it into account
    for( uint8_t i = 0, chanIdx = EU433_NUMB_DEFAULT_CHANNELS; chanIdx < EU433_MAX_NB_CHANNELS; i+=3, chanIdx++ )
    {
        if( chanIdx < ( EU433_NUMB_CHANNELS_CF_LIST + EU433_NUMB_DEFAULT_CHANNELS ) )
        {
            // Channel frequency
            newChannel.Frequency = (uint32_t) applyCFList->Payload[i];
            newChannel.Frequency |= ( (uint32_t) applyCFList->Payload[i + 1] << 8 );
            newChannel.Frequency |= ( (uint32_t) applyCFList->Payload[i + 2] << 16 );
            newChannel.Frequency *= 100;

            // Initialize alternative frequency to 0
            newChannel.Rx1Frequency = 0;
        }
        else
        {
            newChannel.Frequency = 0;
            newChannel.DrRange.Value = 0;
            newChannel.Rx1Frequency = 0;
        }

        if( newChannel.Frequency != 0 )
        {
            channelAdd.NewChannel = &newChannel;
            channelAdd.ChannelId = chanIdx;

            // Try to add all channels
            add_channel( &channelAdd );
        }
        else
        {
            channelRemove.ChannelId = chanIdx;

            remove_channel( &channelRemove );
        }
    }
}

bool LoRaPHYEU433::set_channel_mask(ChanMaskSetParams_t* chanMaskSet)
{
    switch( chanMaskSet->ChannelsMaskType )
    {
        case CHANNELS_MASK:
        {
            copy_channel_mask( ChannelsMask, chanMaskSet->ChannelsMaskIn, 1 );
            break;
        }
        case CHANNELS_DEFAULT_MASK:
        {
            copy_channel_mask( ChannelsDefaultMask, chanMaskSet->ChannelsMaskIn, 1 );
            break;
        }
        default:
            return false;
    }
    return true;
}

bool LoRaPHYEU433::get_next_ADR(AdrNextParams_t* adrNext, int8_t* drOut,
                                int8_t* txPowOut, uint32_t* adrAckCounter)
{
    bool adrAckReq = false;
    int8_t datarate = adrNext->Datarate;
    int8_t txPower = adrNext->TxPower;
    GetPhyParams_t getPhy;
    PhyParam_t phyParam;

    // Report back the adr ack counter
    *adrAckCounter = adrNext->AdrAckCounter;

    if( adrNext->AdrEnabled == true )
    {
        if( datarate == EU433_TX_MIN_DATARATE )
        {
            *adrAckCounter = 0;
            adrAckReq = false;
        }
        else
        {
            if( adrNext->AdrAckCounter >= EU433_ADR_ACK_LIMIT )
            {
                adrAckReq = true;
                txPower = EU433_MAX_TX_POWER;
            }
            else
            {
                adrAckReq = false;
            }
            if( adrNext->AdrAckCounter >= ( EU433_ADR_ACK_LIMIT + EU433_ADR_ACK_DELAY ) )
            {
                if( ( adrNext->AdrAckCounter % EU433_ADR_ACK_DELAY ) == 1 )
                {
                    // Decrease the datarate
                    getPhy.Attribute = PHY_NEXT_LOWER_TX_DR;
                    getPhy.Datarate = datarate;
                    getPhy.UplinkDwellTime = adrNext->UplinkDwellTime;
                    phyParam = get_phy_params( &getPhy );
                    datarate = phyParam.Value;

                    if( datarate == EU433_TX_MIN_DATARATE )
                    {
                        // We must set adrAckReq to false as soon as we reach the lowest datarate
                        adrAckReq = false;
                        if( adrNext->UpdateChanMask == true )
                        {
                            // Re-enable default channels
                            ChannelsMask[0] |= LC( 1 ) + LC( 2 ) + LC( 3 );
                        }
                    }
                }
            }
        }
    }

    *drOut = datarate;
    *txPowOut = txPower;
    return adrAckReq;
}

void LoRaPHYEU433::compute_rx_win_params(int8_t datarate, uint8_t minRxSymbols,
                                         uint32_t rxError,
                                         RxConfigParams_t *rxConfigParams)
{
    double tSymbol = 0.0;

    // Get the datarate, perform a boundary check
    rxConfigParams->Datarate = MIN( datarate, EU433_RX_MAX_DATARATE );
    rxConfigParams->Bandwidth = GetBandwidth( rxConfigParams->Datarate );

    if( rxConfigParams->Datarate == DR_7 )
    { // FSK
        tSymbol = compute_symb_timeout_fsk( DataratesEU433[rxConfigParams->Datarate] );
    }
    else
    { // LoRa
        tSymbol = compute_symb_timeout_lora( DataratesEU433[rxConfigParams->Datarate], BandwidthsEU433[rxConfigParams->Datarate] );
    }

    get_rx_window_params( tSymbol, minRxSymbols, rxError, RADIO_WAKEUP_TIME, &rxConfigParams->WindowTimeout, &rxConfigParams->WindowOffset );
}

bool LoRaPHYEU433::rx_config(RxConfigParams_t* rxConfig, int8_t* datarate)
{
    radio_modems_t modem;
    int8_t dr = rxConfig->Datarate;
    uint8_t maxPayload = 0;
    int8_t phyDr = 0;
    uint32_t frequency = rxConfig->Frequency;

    if( _radio->get_status() != RF_IDLE )
    {
        return false;
    }

    if( rxConfig->Window == 0 )
    {
        // Apply window 1 frequency
        frequency = Channels[rxConfig->Channel].Frequency;
        // Apply the alternative RX 1 window frequency, if it is available
        if( Channels[rxConfig->Channel].Rx1Frequency != 0 )
        {
            frequency = Channels[rxConfig->Channel].Rx1Frequency;
        }
    }

    // Read the physical datarate from the datarates table
    phyDr = DataratesEU433[dr];

    _radio->set_channel( frequency );

    // Radio configuration
    if( dr == DR_7 )
    {
        modem = MODEM_FSK;
        _radio->set_rx_config( modem, 50000, phyDr * 1000, 0, 83333, 5,
                               rxConfig->WindowTimeout, false, 0, true, 0, 0,
                               false, rxConfig->RxContinuous );
    }
    else
    {
        modem = MODEM_LORA;
        _radio->set_rx_config( modem, rxConfig->Bandwidth, phyDr, 1, 0, 8,
                               rxConfig->WindowTimeout, false, 0, false, 0, 0,
                               true, rxConfig->RxContinuous );
    }

    if( rxConfig->RepeaterSupport == true )
    {
        maxPayload = MaxPayloadOfDatarateRepeaterEU433[dr];
    }
    else
    {
        maxPayload = MaxPayloadOfDatarateEU433[dr];
    }
    _radio->set_max_payload_length( modem, maxPayload + LORA_MAC_FRMPAYLOAD_OVERHEAD );

    *datarate = (uint8_t) dr;
    return true;
}

bool LoRaPHYEU433::tx_config(TxConfigParams_t* txConfig, int8_t* txPower,
                             TimerTime_t* txTimeOnAir)
{
    radio_modems_t modem;
    int8_t phyDr = DataratesEU433[txConfig->Datarate];
    int8_t txPowerLimited = LimitTxPower( txConfig->TxPower, Bands[Channels[txConfig->Channel].Band].TxMaxPower, txConfig->Datarate, ChannelsMask );
    uint32_t bandwidth = GetBandwidth( txConfig->Datarate );
    int8_t phyTxPower = 0;

    // Calculate physical TX power
    phyTxPower = compute_tx_power( txPowerLimited, txConfig->MaxEirp, txConfig->AntennaGain );

    // Setup the radio frequency
    _radio->set_channel( Channels[txConfig->Channel].Frequency );

    if( txConfig->Datarate == DR_7 )
    { // High Speed FSK channel
        modem = MODEM_FSK;
        _radio->set_tx_config( modem, phyTxPower, 25000, bandwidth, phyDr * 1000, 0, 5, false, true, 0, 0, false, 3000 );
    }
    else
    {
        modem = MODEM_LORA;
        _radio->set_tx_config( modem, phyTxPower, 0, bandwidth, phyDr, 1, 8, false, true, 0, 0, false, 3000 );
    }

    // Setup maximum payload lenght of the radio driver
    _radio->set_max_payload_length( modem, txConfig->PktLen );
    // Get the time-on-air of the next tx frame
    *txTimeOnAir = _radio->time_on_air( modem, txConfig->PktLen );

    *txPower = txPowerLimited;
    return true;
}

uint8_t LoRaPHYEU433::link_ADR_request(LinkAdrReqParams_t* linkAdrReq,
                                       int8_t* drOut, int8_t* txPowOut,
                                       uint8_t* nbRepOut, uint8_t* nbBytesParsed)
{
    uint8_t status = 0x07;
    RegionCommonLinkAdrParams_t linkAdrParams;
    uint8_t nextIndex = 0;
    uint8_t bytesProcessed = 0;
    uint16_t chMask = 0;
    GetPhyParams_t getPhy;
    PhyParam_t phyParam;
    RegionCommonLinkAdrReqVerifyParams_t linkAdrVerifyParams;

    while( bytesProcessed < linkAdrReq->PayloadSize )
    {
        // Get ADR request parameters
        nextIndex = parse_link_ADR_req( &( linkAdrReq->Payload[bytesProcessed] ), &linkAdrParams );

        if( nextIndex == 0 )
            break; // break loop, since no more request has been found

        // Update bytes processed
        bytesProcessed += nextIndex;

        // Revert status, as we only check the last ADR request for the channel mask KO
        status = 0x07;

        // Setup temporary channels mask
        chMask = linkAdrParams.ChMask;

        // Verify channels mask
        if( ( linkAdrParams.ChMaskCtrl == 0 ) && ( chMask == 0 ) )
        {
            status &= 0xFE; // Channel mask KO
        }
        else if( ( ( linkAdrParams.ChMaskCtrl >= 1 ) && ( linkAdrParams.ChMaskCtrl <= 5 )) ||
                ( linkAdrParams.ChMaskCtrl >= 7 ) )
        {
            // RFU
            status &= 0xFE; // Channel mask KO
        }
        else
        {
            for( uint8_t i = 0; i < EU433_MAX_NB_CHANNELS; i++ )
            {
                if( linkAdrParams.ChMaskCtrl == 6 )
                {
                    if( Channels[i].Frequency != 0 )
                    {
                        chMask |= 1 << i;
                    }
                }
                else
                {
                    if( ( ( chMask & ( 1 << i ) ) != 0 ) &&
                        ( Channels[i].Frequency == 0 ) )
                    {// Trying to enable an undefined channel
                        status &= 0xFE; // Channel mask KO
                    }
                }
            }
        }
    }

        // Get the minimum possible datarate
    getPhy.Attribute = PHY_MIN_TX_DR;
    getPhy.UplinkDwellTime = linkAdrReq->UplinkDwellTime;
    phyParam = get_phy_params( &getPhy );

    linkAdrVerifyParams.Status = status;
    linkAdrVerifyParams.AdrEnabled = linkAdrReq->AdrEnabled;
    linkAdrVerifyParams.Datarate = linkAdrParams.Datarate;
    linkAdrVerifyParams.TxPower = linkAdrParams.TxPower;
    linkAdrVerifyParams.NbRep = linkAdrParams.NbRep;
    linkAdrVerifyParams.CurrentDatarate = linkAdrReq->CurrentDatarate;
    linkAdrVerifyParams.CurrentTxPower = linkAdrReq->CurrentTxPower;
    linkAdrVerifyParams.CurrentNbRep = linkAdrReq->CurrentNbRep;
    linkAdrVerifyParams.NbChannels = EU433_MAX_NB_CHANNELS;
    linkAdrVerifyParams.ChannelsMask = &chMask;
    linkAdrVerifyParams.MinDatarate = ( int8_t )phyParam.Value;
    linkAdrVerifyParams.MaxDatarate = EU433_TX_MAX_DATARATE;
    linkAdrVerifyParams.Channels = Channels;
    linkAdrVerifyParams.MinTxPower = EU433_MIN_TX_POWER;
    linkAdrVerifyParams.MaxTxPower = EU433_MAX_TX_POWER;

    // Verify the parameters and update, if necessary
    status = verify_link_ADR_req( &linkAdrVerifyParams, &linkAdrParams.Datarate, &linkAdrParams.TxPower, &linkAdrParams.NbRep );

    // Update channelsMask if everything is correct
    if( status == 0x07 )
    {
        // Set the channels mask to a default value
        memset( ChannelsMask, 0, sizeof( ChannelsMask ) );
        // Update the channels mask
        ChannelsMask[0] = chMask;
    }

    // Update status variables
    *drOut = linkAdrParams.Datarate;
    *txPowOut = linkAdrParams.TxPower;
    *nbRepOut = linkAdrParams.NbRep;
    *nbBytesParsed = bytesProcessed;

    return status;
}

uint8_t LoRaPHYEU433::setup_rx_params(RxParamSetupReqParams_t* rxParamSetupReq)
{
    uint8_t status = 0x07;

    // Verify radio frequency
    if( _radio->check_rf_frequency( rxParamSetupReq->Frequency ) == false )
    {
        status &= 0xFE; // Channel frequency KO
    }

    // Verify datarate
    if( val_in_range( rxParamSetupReq->Datarate, EU433_RX_MIN_DATARATE, EU433_RX_MAX_DATARATE ) == 0 )
    {
        status &= 0xFD; // Datarate KO
    }

    // Verify datarate offset
    if( val_in_range( rxParamSetupReq->DrOffset, EU433_MIN_RX1_DR_OFFSET, EU433_MAX_RX1_DR_OFFSET ) == 0 )
    {
        status &= 0xFB; // Rx1DrOffset range KO
    }

    return status;
}

uint8_t LoRaPHYEU433::request_new_channel(NewChannelReqParams_t* newChannelReq)
{
    uint8_t status = 0x03;
    ChannelAddParams_t channelAdd;
    ChannelRemoveParams_t channelRemove;

    if( newChannelReq->NewChannel->Frequency == 0 )
    {
        channelRemove.ChannelId = newChannelReq->ChannelId;

        // Remove
        if( remove_channel( &channelRemove ) == false )
        {
            status &= 0xFC;
        }
    }
    else
    {
        channelAdd.NewChannel = newChannelReq->NewChannel;
        channelAdd.ChannelId = newChannelReq->ChannelId;

        switch( add_channel( &channelAdd ) )
        {
            case LORAMAC_STATUS_OK:
            {
                break;
            }
            case LORAMAC_STATUS_FREQUENCY_INVALID:
            {
                status &= 0xFE;
                break;
            }
            case LORAMAC_STATUS_DATARATE_INVALID:
            {
                status &= 0xFD;
                break;
            }
            case LORAMAC_STATUS_FREQ_AND_DR_INVALID:
            {
                status &= 0xFC;
                break;
            }
            default:
            {
                status &= 0xFC;
                break;
            }
        }
    }

    return status;
}

int8_t LoRaPHYEU433::setup_tx_params(TxParamSetupReqParams_t* txParamSetupReq)
{
    return -1;
}

uint8_t LoRaPHYEU433::dl_channel_request(DlChannelReqParams_t* dlChannelReq)
{
    uint8_t status = 0x03;

    // Verify if the frequency is supported
    if( VerifyTxFreq( dlChannelReq->Rx1Frequency, _radio ) == false )
    {
        status &= 0xFE;
    }

    // Verify if an uplink frequency exists
    if( Channels[dlChannelReq->ChannelId].Frequency == 0 )
    {
        status &= 0xFD;
    }

    // Apply Rx1 frequency, if the status is OK
    if( status == 0x03 )
    {
        Channels[dlChannelReq->ChannelId].Rx1Frequency = dlChannelReq->Rx1Frequency;
    }

    return status;
}

int8_t LoRaPHYEU433::get_alternate_DR(AlternateDrParams_t* alternateDr)
{
    int8_t datarate = 0;

    if( ( alternateDr->NbTrials % 48 ) == 0 )
    {
        datarate = DR_0;
    }
    else if( ( alternateDr->NbTrials % 32 ) == 0 )
    {
        datarate = DR_1;
    }
    else if( ( alternateDr->NbTrials % 24 ) == 0 )
    {
        datarate = DR_2;
    }
    else if( ( alternateDr->NbTrials % 16 ) == 0 )
    {
        datarate = DR_3;
    }
    else if( ( alternateDr->NbTrials % 8 ) == 0 )
    {
        datarate = DR_4;
    }
    else
    {
        datarate = DR_5;
    }
    return datarate;
}

void LoRaPHYEU433::calculate_backoff(CalcBackOffParams_t* calcBackOff)
{
    RegionCommonCalcBackOffParams_t calcBackOffParams;

    calcBackOffParams.Channels = Channels;
    calcBackOffParams.Bands = Bands;
    calcBackOffParams.LastTxIsJoinRequest = calcBackOff->LastTxIsJoinRequest;
    calcBackOffParams.Joined = calcBackOff->Joined;
    calcBackOffParams.DutyCycleEnabled = calcBackOff->DutyCycleEnabled;
    calcBackOffParams.Channel = calcBackOff->Channel;
    calcBackOffParams.ElapsedTime = calcBackOff->ElapsedTime;
    calcBackOffParams.TxTimeOnAir = calcBackOff->TxTimeOnAir;

    get_DC_backoff( &calcBackOffParams );
}

bool LoRaPHYEU433::set_next_channel(NextChanParams_t* nextChanParams,
                                    uint8_t* channel, TimerTime_t* time,
                                    TimerTime_t* aggregatedTimeOff)
{
    uint8_t nbEnabledChannels = 0;
    uint8_t delayTx = 0;
    uint8_t enabledChannels[EU433_MAX_NB_CHANNELS] = { 0 };
    TimerTime_t nextTxDelay = 0;

    if( num_active_channels( ChannelsMask, 0, 1 ) == 0 )
    { // Reactivate default channels
        ChannelsMask[0] |= LC( 1 ) + LC( 2 ) + LC( 3 );
    }

    if( nextChanParams->AggrTimeOff <= TimerGetElapsedTime( nextChanParams->LastAggrTx ) )
    {
        // Reset Aggregated time off
        *aggregatedTimeOff = 0;

        // Update bands Time OFF
        nextTxDelay = update_band_timeoff( nextChanParams->Joined, nextChanParams->DutyCycleEnabled, Bands, EU433_MAX_NB_BANDS );

        // Search how many channels are enabled
        nbEnabledChannels = CountNbOfEnabledChannels( nextChanParams->Joined, nextChanParams->Datarate,
                                                      ChannelsMask, Channels,
                                                      Bands, enabledChannels, &delayTx );
    }
    else
    {
        delayTx++;
        nextTxDelay = nextChanParams->AggrTimeOff - TimerGetElapsedTime( nextChanParams->LastAggrTx );
    }

    if( nbEnabledChannels > 0 )
    {
        // We found a valid channel
        *channel = enabledChannels[get_random( 0, nbEnabledChannels - 1 )];

        *time = 0;
        return true;
    }
    else
    {
        if( delayTx > 0 )
        {
            // Delay transmission due to AggregatedTimeOff or to a band time off
            *time = nextTxDelay;
            return true;
        }
        // Datarate not supported by any channel, restore defaults
        ChannelsMask[0] |= LC( 1 ) + LC( 2 ) + LC( 3 );
        *time = 0;
        return false;
    }
}

LoRaMacStatus_t LoRaPHYEU433::add_channel(ChannelAddParams_t* channelAdd)
{
    uint8_t band = 0;
    bool drInvalid = false;
    bool freqInvalid = false;
    uint8_t id = channelAdd->ChannelId;

    if( id >= EU433_MAX_NB_CHANNELS )
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    // Validate the datarate range
    if( val_in_range( channelAdd->NewChannel->DrRange.Fields.Min, EU433_TX_MIN_DATARATE, EU433_TX_MAX_DATARATE ) == 0 )
    {
        drInvalid = true;
    }
    if( val_in_range( channelAdd->NewChannel->DrRange.Fields.Max, EU433_TX_MIN_DATARATE, EU433_TX_MAX_DATARATE ) == 0 )
    {
        drInvalid = true;
    }
    if( channelAdd->NewChannel->DrRange.Fields.Min > channelAdd->NewChannel->DrRange.Fields.Max )
    {
        drInvalid = true;
    }

    // Default channels don't accept all values
    if( id < EU433_NUMB_DEFAULT_CHANNELS )
    {
        // Validate the datarate range for min: must be DR_0
        if( channelAdd->NewChannel->DrRange.Fields.Min > DR_0 )
        {
            drInvalid = true;
        }
        // Validate the datarate range for max: must be DR_5 <= Max <= TX_MAX_DATARATE
        if( val_in_range( channelAdd->NewChannel->DrRange.Fields.Max, DR_5, EU433_TX_MAX_DATARATE ) == 0 )
        {
            drInvalid = true;
        }
        // We are not allowed to change the frequency
        if( channelAdd->NewChannel->Frequency != Channels[id].Frequency )
        {
            freqInvalid = true;
        }
    }

    // Check frequency
    if( freqInvalid == false )
    {
        if( VerifyTxFreq( channelAdd->NewChannel->Frequency, _radio ) == false )
        {
            freqInvalid = true;
        }
    }

    // Check status
    if( ( drInvalid == true ) && ( freqInvalid == true ) )
    {
        return LORAMAC_STATUS_FREQ_AND_DR_INVALID;
    }
    if( drInvalid == true )
    {
        return LORAMAC_STATUS_DATARATE_INVALID;
    }
    if( freqInvalid == true )
    {
        return LORAMAC_STATUS_FREQUENCY_INVALID;
    }

    memcpy( &(Channels[id]), channelAdd->NewChannel, sizeof( Channels[id] ) );
    Channels[id].Band = band;
    ChannelsMask[0] |= ( 1 << id );
    return LORAMAC_STATUS_OK;
}

bool LoRaPHYEU433::remove_channel(ChannelRemoveParams_t* channelRemove)
{
    uint8_t id = channelRemove->ChannelId;

    if( id < EU433_NUMB_DEFAULT_CHANNELS )
    {
        return false;
    }

    // Remove the channel from the list of channels
    const ChannelParams_t empty_channel = { 0, 0, { 0 }, 0 };
    Channels[id] = empty_channel;

    return disable_channel( ChannelsMask, id, EU433_MAX_NB_CHANNELS );
}

void LoRaPHYEU433::set_tx_cont_mode(ContinuousWaveParams_t* continuousWave)
{
    int8_t txPowerLimited = LimitTxPower( continuousWave->TxPower, Bands[Channels[continuousWave->Channel].Band].TxMaxPower, continuousWave->Datarate, ChannelsMask );
    int8_t phyTxPower = 0;
    uint32_t frequency = Channels[continuousWave->Channel].Frequency;

    // Calculate physical TX power
    phyTxPower = compute_tx_power( txPowerLimited, continuousWave->MaxEirp, continuousWave->AntennaGain );

    _radio->set_tx_continuous_wave( frequency, phyTxPower, continuousWave->Timeout );
}

uint8_t LoRaPHYEU433::apply_DR_offset(uint8_t downlinkDwellTime, int8_t dr, int8_t drOffset)
{
    int8_t datarate = dr - drOffset;

    if( datarate < 0 )
    {
        datarate = DR_0;
    }
    return datarate;
}
