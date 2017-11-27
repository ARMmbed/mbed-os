/**
 *  @file LoRaPHYUS915Hybrid.cpp
 *
 *  @brief Implements LoRaPHY for US 915 MHz Hybrid band
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

#include "LoRaPHYUS915Hybrid.h"

#include "lora_phy_ds.h"
#include "LoRaRadio.h"


/*!
 * Minimal datarate that can be used by the node
 */
#define US915_HYBRID_TX_MIN_DATARATE                DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define US915_HYBRID_TX_MAX_DATARATE                DR_4

/*!
 * Minimal datarate that can be used by the node
 */
#define US915_HYBRID_RX_MIN_DATARATE                DR_8

/*!
 * Maximal datarate that can be used by the node
 */
#define US915_HYBRID_RX_MAX_DATARATE                DR_13

/*!
 * Default datarate used by the node
 */
#define US915_HYBRID_DEFAULT_DATARATE               DR_0

/*!
 * Minimal Rx1 receive datarate offset
 */
#define US915_HYBRID_MIN_RX1_DR_OFFSET              0

/*!
 * Maximal Rx1 receive datarate offset
 */
#define US915_HYBRID_MAX_RX1_DR_OFFSET              3

/*!
 * Default Rx1 receive datarate offset
 */
#define US915_HYBRID_DEFAULT_RX1_DR_OFFSET          0

/*!
 * Minimal Tx output power that can be used by the node
 */
#define US915_HYBRID_MIN_TX_POWER                   TX_POWER_10

/*!
 * Maximal Tx output power that can be used by the node
 */
#define US915_HYBRID_MAX_TX_POWER                   TX_POWER_0

/*!
 * Default Tx output power used by the node
 */
#define US915_HYBRID_DEFAULT_TX_POWER               TX_POWER_0

/*!
 * Default Max ERP
 */
#define US915_HYBRID_DEFAULT_MAX_ERP                30.0f

/*!
 * ADR Ack limit
 */
#define US915_HYBRID_ADR_ACK_LIMIT                  64

/*!
 * ADR Ack delay
 */
#define US915_HYBRID_ADR_ACK_DELAY                  32

/*!
 * Enabled or disabled the duty cycle
 */
#define US915_HYBRID_DUTY_CYCLE_ENABLED             0

/*!
 * Maximum RX window duration
 */
#define US915_HYBRID_MAX_RX_WINDOW                  3000

/*!
 * Receive delay 1
 */
#define US915_HYBRID_RECEIVE_DELAY1                 1000

/*!
 * Receive delay 2
 */
#define US915_HYBRID_RECEIVE_DELAY2                 2000

/*!
 * Join accept delay 1
 */
#define US915_HYBRID_JOIN_ACCEPT_DELAY1             5000

/*!
 * Join accept delay 2
 */
#define US915_HYBRID_JOIN_ACCEPT_DELAY2             6000

/*!
 * Maximum frame counter gap
 */
#define US915_HYBRID_MAX_FCNT_GAP                   16384

/*!
 * Ack timeout
 */
#define US915_HYBRID_ACKTIMEOUT                     2000

/*!
 * Random ack timeout limits
 */
#define US915_HYBRID_ACK_TIMEOUT_RND                1000

/*!
 * Second reception window channel frequency definition.
 */
#define US915_HYBRID_RX_WND_2_FREQ                  923300000

/*!
 * Second reception window channel datarate definition.
 */
#define US915_HYBRID_RX_WND_2_DR                    DR_8

/*!
 * Band 0 definition
 * { DutyCycle, TxMaxPower, LastTxDoneTime, TimeOff }
 */
#define US915_HYBRID_BAND0                          { 1, US915_HYBRID_MAX_TX_POWER, 0,  0 } //  100.0 %

/*!
 * Defines the first channel for RX window 1 for US band
 */
#define US915_HYBRID_FIRST_RX1_CHANNEL              ( (uint32_t) 923300000 )

/*!
 * Defines the last channel for RX window 1 for US band
 */
#define US915_HYBRID_LAST_RX1_CHANNEL               ( (uint32_t) 927500000 )

/*!
 * Defines the step width of the channels for RX window 1
 */
#define US915_HYBRID_STEPWIDTH_RX1_CHANNEL          ( (uint32_t) 600000 )

/*!
 * Data rates table definition
 */
static const uint8_t DataratesUS915_HYBRID[]  = { 10, 9, 8,  7,  8,  0,  0, 0, 12, 11, 10, 9, 8, 7, 0, 0 };

/*!
 * Bandwidths table definition in Hz
 */
static const uint32_t BandwidthsUS915_HYBRID[] = { 125000, 125000, 125000, 125000, 500000, 0, 0, 0, 500000, 500000, 500000, 500000, 500000, 500000, 0, 0 };

/*!
 * Up/Down link data rates offset definition
 */
static const int8_t DatarateOffsetsUS915_HYBRID[5][4] =
{
    { DR_10, DR_9 , DR_8 , DR_8  }, // DR_0
    { DR_11, DR_10, DR_9 , DR_8  }, // DR_1
    { DR_12, DR_11, DR_10, DR_9  }, // DR_2
    { DR_13, DR_12, DR_11, DR_10 }, // DR_3
    { DR_13, DR_13, DR_12, DR_11 }, // DR_4
};

/*!
 * Maximum payload with respect to the datarate index. Cannot operate with repeater.
 */
static const uint8_t MaxPayloadOfDatarateUS915_HYBRID[] = { 11, 53, 125, 242, 242, 0, 0, 0, 53, 129, 242, 242, 242, 242, 0, 0 };

/*!
 * Maximum payload with respect to the datarate index. Can operate with repeater.
 */
static const uint8_t MaxPayloadOfDatarateRepeaterUS915_HYBRID[] = { 11, 53, 125, 242, 242, 0, 0, 0, 33, 109, 222, 222, 222, 222, 0, 0 };


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
    switch( BandwidthsUS915_HYBRID[drIndex] )
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

static void ReenableChannels( uint16_t mask, uint16_t* channelsMask )
{
    uint16_t blockMask = mask;

    for( uint8_t i = 0, j = 0; i < 4; i++, j += 2 )
    {
        channelsMask[i] = 0;
        if( ( blockMask & ( 1 << j ) ) != 0 )
        {
            channelsMask[i] |= 0x00FF;
        }
        if( ( blockMask & ( 1 << ( j + 1 ) ) ) != 0 )
        {
            channelsMask[i] |= 0xFF00;
        }
    }
    channelsMask[4] = blockMask;
    channelsMask[5] = 0x0000;
}

static uint8_t CountBits( uint16_t mask, uint8_t nbBits )
{
    uint8_t nbActiveBits = 0;

    for( uint8_t j = 0; j < nbBits; j++ )
    {
        if( ( mask & ( 1 << j ) ) == ( 1 << j ) )
        {
            nbActiveBits++;
        }
    }
    return nbActiveBits;
}

int8_t LoRaPHYUS915Hybrid::LimitTxPower( int8_t txPower, int8_t maxBandTxPower, int8_t datarate, uint16_t* channelsMask )
{
    int8_t txPowerResult = txPower;

    // Limit tx power to the band max
    txPowerResult =  MAX( txPower, maxBandTxPower );

    if( datarate == DR_4 )
    {// Limit tx power to max 26dBm
        txPowerResult = MAX( txPower, TX_POWER_2 );
    }
    else
    {
        if( num_active_channels( channelsMask, 0, 4 ) < 50 )
        {// Limit tx power to max 21dBm
            txPowerResult = MAX( txPower, TX_POWER_5 );
        }
    }
    return txPowerResult;
}

static bool ValidateChannelsMask( uint16_t* channelsMask )
{
    bool chanMaskState = false;
    uint16_t block1 = 0;
    uint16_t block2 = 0;
    uint8_t index = 0;
    uint16_t channelsMaskCpy[6];

    // Copy channels mask to not change the input
    for( uint8_t i = 0; i < 4; i++ )
    {
        channelsMaskCpy[i] = channelsMask[i];
    }

    for( uint8_t i = 0; i < 4; i++ )
    {
        block1 = channelsMaskCpy[i] & 0x00FF;
        block2 = channelsMaskCpy[i] & 0xFF00;

        if( CountBits( block1, 16 ) > 5 )
        {
            channelsMaskCpy[i] &= block1;
            channelsMaskCpy[4] = 1 << ( i * 2 );
            chanMaskState = true;
            index = i;
            break;
        }
        else if( CountBits( block2, 16 ) > 5 )
        {
            channelsMaskCpy[i] &= block2;
            channelsMaskCpy[4] = 1 << ( i * 2 + 1 );
            chanMaskState = true;
            index = i;
            break;
        }
    }

    // Do only change the channel mask, if we have found a valid block.
    if( chanMaskState == true )
    {
        // Copy channels mask back again
        for( uint8_t i = 0; i < 4; i++ )
        {
            channelsMask[i] = channelsMaskCpy[i];

            if( i != index )
            {
                channelsMask[i] = 0;
            }
        }
        channelsMask[4] = channelsMaskCpy[4];
    }
    return chanMaskState;
}

uint8_t LoRaPHYUS915Hybrid::CountNbOfEnabledChannels( uint8_t datarate, uint16_t* channelsMask, ChannelParams_t* channels, Band_t* bands, uint8_t* enabledChannels, uint8_t* delayTx )
{
    uint8_t nbEnabledChannels = 0;
    uint8_t delayTransmission = 0;

    for( uint8_t i = 0, k = 0; i < US915_HYBRID_MAX_NB_CHANNELS; i += 16, k++ )
    {
        for( uint8_t j = 0; j < 16; j++ )
        {
            if( ( channelsMask[k] & ( 1 << j ) ) != 0 )
            {
                if( channels[i + j].Frequency == 0 )
                { // Check if the channel is enabled
                    continue;
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

LoRaPHYUS915Hybrid::LoRaPHYUS915Hybrid()
{
    const Band_t band0 = US915_HYBRID_BAND0;
    Bands[0] = band0;
}

LoRaPHYUS915Hybrid::~LoRaPHYUS915Hybrid()
{
}

PhyParam_t LoRaPHYUS915Hybrid::get_phy_params(GetPhyParams_t* getPhy)
{
    PhyParam_t phyParam = { 0 };

    switch( getPhy->Attribute )
    {
        case PHY_MIN_RX_DR:
        {
            phyParam.Value = US915_HYBRID_RX_MIN_DATARATE;
            break;
        }
        case PHY_MIN_TX_DR:
        {
            phyParam.Value = US915_HYBRID_TX_MIN_DATARATE;
            break;
        }
        case PHY_DEF_TX_DR:
        {
            phyParam.Value = US915_HYBRID_DEFAULT_DATARATE;
            break;
        }
        case PHY_NEXT_LOWER_TX_DR:
        {
            phyParam.Value = GetNextLowerTxDr( getPhy->Datarate, US915_HYBRID_TX_MIN_DATARATE );
            break;
        }
        case PHY_DEF_TX_POWER:
        {
            phyParam.Value = US915_HYBRID_DEFAULT_TX_POWER;
            break;
        }
        case PHY_MAX_PAYLOAD:
        {
            phyParam.Value = MaxPayloadOfDatarateUS915_HYBRID[getPhy->Datarate];
            break;
        }
        case PHY_MAX_PAYLOAD_REPEATER:
        {
            phyParam.Value = MaxPayloadOfDatarateRepeaterUS915_HYBRID[getPhy->Datarate];
            break;
        }
        case PHY_DUTY_CYCLE:
        {
            phyParam.Value = US915_HYBRID_DUTY_CYCLE_ENABLED;
            break;
        }
        case PHY_MAX_RX_WINDOW:
        {
            phyParam.Value = US915_HYBRID_MAX_RX_WINDOW;
            break;
        }
        case PHY_RECEIVE_DELAY1:
        {
            phyParam.Value = US915_HYBRID_RECEIVE_DELAY1;
            break;
        }
        case PHY_RECEIVE_DELAY2:
        {
            phyParam.Value = US915_HYBRID_RECEIVE_DELAY2;
            break;
        }
        case PHY_JOIN_ACCEPT_DELAY1:
        {
            phyParam.Value = US915_HYBRID_JOIN_ACCEPT_DELAY1;
            break;
        }
        case PHY_JOIN_ACCEPT_DELAY2:
        {
            phyParam.Value = US915_HYBRID_JOIN_ACCEPT_DELAY2;
            break;
        }
        case PHY_MAX_FCNT_GAP:
        {
            phyParam.Value = US915_HYBRID_MAX_FCNT_GAP;
            break;
        }
        case PHY_ACK_TIMEOUT:
        {
            phyParam.Value = ( US915_HYBRID_ACKTIMEOUT + get_random( -US915_HYBRID_ACK_TIMEOUT_RND, US915_HYBRID_ACK_TIMEOUT_RND ) );
            break;
        }
        case PHY_DEF_DR1_OFFSET:
        {
            phyParam.Value = US915_HYBRID_DEFAULT_RX1_DR_OFFSET;
            break;
        }
        case PHY_DEF_RX2_FREQUENCY:
        {
            phyParam.Value = US915_HYBRID_RX_WND_2_FREQ;
            break;
        }
        case PHY_DEF_RX2_DR:
        {
            phyParam.Value = US915_HYBRID_RX_WND_2_DR;
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
            phyParam.Value = US915_HYBRID_MAX_NB_CHANNELS;
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
        case PHY_DEF_ANTENNA_GAIN:
        {
            phyParam.fValue = 0;
            break;
        }
        case PHY_NB_JOIN_TRIALS:
        case PHY_DEF_NB_JOIN_TRIALS:
        {
            phyParam.Value = 2;
            break;
        }
        default:
        {
            break;
        }
    }

    return phyParam;
}

void LoRaPHYUS915Hybrid::set_band_tx_done(SetBandTxDoneParams_t* txDone)
{
    set_last_tx_done( txDone->Joined, &Bands[Channels[txDone->Channel].Band], txDone->LastTxDoneTime );
}

void LoRaPHYUS915Hybrid::load_defaults(InitType_t type)
{
    switch( type )
    {
        case INIT_TYPE_INIT:
        {
            // Channels
            // 125 kHz channels
            for( uint8_t i = 0; i < US915_HYBRID_MAX_NB_CHANNELS - 8; i++ )
            {
                Channels[i].Frequency = 902300000 + i * 200000;
                Channels[i].DrRange.Value = ( DR_3 << 4 ) | DR_0;
                Channels[i].Band = 0;
            }
            // 500 kHz channels
            for( uint8_t i = US915_HYBRID_MAX_NB_CHANNELS - 8; i < US915_HYBRID_MAX_NB_CHANNELS; i++ )
            {
                Channels[i].Frequency = 903000000 + ( i - ( US915_HYBRID_MAX_NB_CHANNELS - 8 ) ) * 1600000;
                Channels[i].DrRange.Value = ( DR_4 << 4 ) | DR_4;
                Channels[i].Band = 0;
            }

            // ChannelsMask
            ChannelsDefaultMask[0] = 0x00FF;
            ChannelsDefaultMask[1] = 0x0000;
            ChannelsDefaultMask[2] = 0x0000;
            ChannelsDefaultMask[3] = 0x0000;
            ChannelsDefaultMask[4] = 0x0001;
            ChannelsDefaultMask[5] = 0x0000;

            // Copy channels default mask
            copy_channel_mask( ChannelsMask, ChannelsDefaultMask, 6 );

            // Copy into channels mask remaining
            copy_channel_mask( ChannelsMaskRemaining, ChannelsMask, 6 );
            break;
        }
        case INIT_TYPE_RESTORE:
        {
            ReenableChannels( ChannelsDefaultMask[4], ChannelsMask );

            for( uint8_t i = 0; i < 6; i++ )
            { // Copy-And the channels mask
                ChannelsMaskRemaining[i] &= ChannelsMask[i];
            }
        }
        default:
        {
            break;
        }
    }
}

bool LoRaPHYUS915Hybrid::verify(VerifyParams_t* verify, PhyAttribute_t phyAttribute)
{
    switch( phyAttribute )
    {
        case PHY_TX_DR:
        {
            return val_in_range( verify->DatarateParams.Datarate, US915_HYBRID_TX_MIN_DATARATE, US915_HYBRID_TX_MAX_DATARATE );
        }
        case PHY_DEF_TX_DR:
        {
            return val_in_range( verify->DatarateParams.Datarate, DR_0, DR_5 );
        }
        case PHY_RX_DR:
        {
            return val_in_range( verify->DatarateParams.Datarate, US915_HYBRID_RX_MIN_DATARATE, US915_HYBRID_RX_MAX_DATARATE );
        }
        case PHY_DEF_TX_POWER:
        case PHY_TX_POWER:
        {
            // Remark: switched min and max!
            return val_in_range( verify->TxPower, US915_HYBRID_MAX_TX_POWER, US915_HYBRID_MIN_TX_POWER );
        }
        case PHY_DUTY_CYCLE:
        {
            return US915_HYBRID_DUTY_CYCLE_ENABLED;
        }
        case PHY_NB_JOIN_TRIALS:
        {
            if( verify->NbJoinTrials < 2 )
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

void LoRaPHYUS915Hybrid::apply_cf_list(ApplyCFListParams_t* applyCFList)
{
    return;
}

bool LoRaPHYUS915Hybrid::set_channel_mask(ChanMaskSetParams_t* chanMaskSet)
{
    uint8_t nbChannels = num_active_channels( chanMaskSet->ChannelsMaskIn, 0, 4 );

    // Check the number of active channels
    if( ( nbChannels < 2 ) &&
        ( nbChannels > 0 ) )
    {
        return false;
    }

    // Validate the channels mask
    if( ValidateChannelsMask( chanMaskSet->ChannelsMaskIn ) == false  )
    {
        return false;
    }

    switch( chanMaskSet->ChannelsMaskType )
    {
        case CHANNELS_MASK:
        {
            copy_channel_mask( ChannelsMask, chanMaskSet->ChannelsMaskIn, 6 );

            for( uint8_t i = 0; i < 6; i++ )
            { // Copy-And the channels mask
                ChannelsMaskRemaining[i] &= ChannelsMask[i];
            }
            break;
        }
        case CHANNELS_DEFAULT_MASK:
        {
            copy_channel_mask( ChannelsDefaultMask, chanMaskSet->ChannelsMaskIn, 6 );
            break;
        }
        default:
            return false;
    }
    return true;
}

bool LoRaPHYUS915Hybrid::get_next_ADR(AdrNextParams_t* adrNext, int8_t* drOut,
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
        if( datarate == US915_HYBRID_TX_MIN_DATARATE )
        {
            *adrAckCounter = 0;
            adrAckReq = false;
        }
        else
        {
            if( adrNext->AdrAckCounter >= US915_HYBRID_ADR_ACK_LIMIT )
            {
                adrAckReq = true;
                txPower = US915_HYBRID_MAX_TX_POWER;
            }
            else
            {
                adrAckReq = false;
            }
            if( adrNext->AdrAckCounter >= ( US915_HYBRID_ADR_ACK_LIMIT + US915_HYBRID_ADR_ACK_DELAY ) )
            {
                if( ( adrNext->AdrAckCounter % US915_HYBRID_ADR_ACK_DELAY ) == 1 )
                {
                    // Decrease the datarate
                    getPhy.Attribute = PHY_NEXT_LOWER_TX_DR;
                    getPhy.Datarate = datarate;
                    getPhy.UplinkDwellTime = adrNext->UplinkDwellTime;
                    phyParam = get_phy_params( &getPhy );
                    datarate = phyParam.Value;

                    if( datarate == US915_HYBRID_TX_MIN_DATARATE )
                    {
                        // We must set adrAckReq to false as soon as we reach the lowest datarate
                        adrAckReq = false;
                        if( adrNext->UpdateChanMask == true )
                        {
                            // Re-enable default channels
                            ReenableChannels( ChannelsMask[4], ChannelsMask );
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

void LoRaPHYUS915Hybrid::compute_rx_win_params(int8_t datarate, uint8_t minRxSymbols,
                                               uint32_t rxError, RxConfigParams_t *rxConfigParams)
{
    double tSymbol = 0.0;

    // Get the datarate, perform a boundary check
    rxConfigParams->Datarate = MIN( datarate, US915_HYBRID_RX_MAX_DATARATE );
    rxConfigParams->Bandwidth = GetBandwidth( rxConfigParams->Datarate );

    tSymbol = compute_symb_timeout_lora( DataratesUS915_HYBRID[rxConfigParams->Datarate], BandwidthsUS915_HYBRID[rxConfigParams->Datarate] );

    get_rx_window_params( tSymbol, minRxSymbols, rxError, RADIO_WAKEUP_TIME, &rxConfigParams->WindowTimeout, &rxConfigParams->WindowOffset );
}

bool LoRaPHYUS915Hybrid::rx_config(RxConfigParams_t* rxConfig, int8_t* datarate)
{
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
        frequency = US915_HYBRID_FIRST_RX1_CHANNEL + ( rxConfig->Channel % 8 ) * US915_HYBRID_STEPWIDTH_RX1_CHANNEL;
    }

    // Read the physical datarate from the datarates table
    phyDr = DataratesUS915_HYBRID[dr];

    _radio->set_channel( frequency );

    // Radio configuration
    _radio->set_rx_config( MODEM_LORA, rxConfig->Bandwidth, phyDr, 1, 0, 8, rxConfig->WindowTimeout, false, 0, false, 0, 0, true, rxConfig->RxContinuous );

    if( rxConfig->RepeaterSupport == true )
    {
        maxPayload = MaxPayloadOfDatarateRepeaterUS915_HYBRID[dr];
    }
    else
    {
        maxPayload = MaxPayloadOfDatarateUS915_HYBRID[dr];
    }
    _radio->set_max_payload_length( MODEM_LORA, maxPayload + LORA_MAC_FRMPAYLOAD_OVERHEAD );

    *datarate = (uint8_t) dr;
    return true;
}

bool LoRaPHYUS915Hybrid::tx_config(TxConfigParams_t* txConfig, int8_t* txPower,
                                   TimerTime_t* txTimeOnAir)
{
    int8_t phyDr = DataratesUS915_HYBRID[txConfig->Datarate];
    int8_t txPowerLimited = LimitTxPower( txConfig->TxPower, Bands[Channels[txConfig->Channel].Band].TxMaxPower, txConfig->Datarate, ChannelsMask );
    uint32_t bandwidth = GetBandwidth( txConfig->Datarate );
    int8_t phyTxPower = 0;

    // Calculate physical TX power
    phyTxPower = compute_tx_power( txPowerLimited, US915_HYBRID_DEFAULT_MAX_ERP, 0 );

    // Setup the radio frequency
    _radio->set_channel( Channels[txConfig->Channel].Frequency );

    _radio->set_tx_config( MODEM_LORA, phyTxPower, 0, bandwidth, phyDr, 1, 8, false, true, 0, 0, false, 3000 );

    // Setup maximum payload lenght of the radio driver
    _radio->set_max_payload_length( MODEM_LORA, txConfig->PktLen );
    // Get the time-on-air of the next tx frame
    *txTimeOnAir = _radio->time_on_air( MODEM_LORA, txConfig->PktLen );
    *txPower = txPowerLimited;

    return true;
}

uint8_t LoRaPHYUS915Hybrid::link_ADR_request(LinkAdrReqParams_t* linkAdrReq,
                                             int8_t* drOut, int8_t* txPowOut,
                                             uint8_t* nbRepOut,
                                             uint8_t* nbBytesParsed)
{
    uint8_t status = 0x07;
    RegionCommonLinkAdrParams_t linkAdrParams;
    uint8_t nextIndex = 0;
    uint8_t bytesProcessed = 0;
    uint16_t channelsMask[6] = { 0, 0, 0, 0, 0, 0 };
    GetPhyParams_t getPhy;
    PhyParam_t phyParam;
    RegionCommonLinkAdrReqVerifyParams_t linkAdrVerifyParams;

    // Initialize local copy of channels mask
    copy_channel_mask( channelsMask, ChannelsMask, 6 );

    while( bytesProcessed < linkAdrReq->PayloadSize )
    {
        nextIndex = parse_link_ADR_req( &( linkAdrReq->Payload[bytesProcessed] ), &linkAdrParams );

        if( nextIndex == 0 )
            break; // break loop, since no more request has been found

        // Update bytes processed
        bytesProcessed += nextIndex;

        // Revert status, as we only check the last ADR request for the channel mask KO
        status = 0x07;

        if( linkAdrParams.ChMaskCtrl == 6 )
        {
            // Enable all 125 kHz channels
            channelsMask[0] = 0xFFFF;
            channelsMask[1] = 0xFFFF;
            channelsMask[2] = 0xFFFF;
            channelsMask[3] = 0xFFFF;
            // Apply chMask to channels 64 to 71
            channelsMask[4] = linkAdrParams.ChMask;
        }
        else if( linkAdrParams.ChMaskCtrl == 7 )
        {
            // Disable all 125 kHz channels
            channelsMask[0] = 0x0000;
            channelsMask[1] = 0x0000;
            channelsMask[2] = 0x0000;
            channelsMask[3] = 0x0000;
            // Apply chMask to channels 64 to 71
            channelsMask[4] = linkAdrParams.ChMask;
        }
        else if( linkAdrParams.ChMaskCtrl == 5 )
        {
            // RFU
            status &= 0xFE; // Channel mask KO
        }
        else
        {
            channelsMask[linkAdrParams.ChMaskCtrl] = linkAdrParams.ChMask;
        }
    }

    // FCC 15.247 paragraph F mandates to hop on at least 2 125 kHz channels
    if( ( linkAdrParams.Datarate < DR_4 ) && ( num_active_channels( channelsMask, 0, 4 ) < 2 ) )
    {
        status &= 0xFE; // Channel mask KO
    }

    if( ValidateChannelsMask( channelsMask ) == false )
    {
        status &= 0xFE; // Channel mask KO
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
    linkAdrVerifyParams.NbChannels = US915_HYBRID_MAX_NB_CHANNELS;
    linkAdrVerifyParams.ChannelsMask = channelsMask;
    linkAdrVerifyParams.MinDatarate = ( int8_t )phyParam.Value;
    linkAdrVerifyParams.MaxDatarate = US915_HYBRID_TX_MAX_DATARATE;
    linkAdrVerifyParams.Channels = Channels;
    linkAdrVerifyParams.MinTxPower = US915_HYBRID_MIN_TX_POWER;
    linkAdrVerifyParams.MaxTxPower = US915_HYBRID_MAX_TX_POWER;

    // Verify the parameters and update, if necessary
    status = verify_link_ADR_req( &linkAdrVerifyParams, &linkAdrParams.Datarate, &linkAdrParams.TxPower, &linkAdrParams.NbRep );

    // Update channelsMask if everything is correct
    if( status == 0x07 )
    {
        // Copy Mask
        copy_channel_mask( ChannelsMask, channelsMask, 6 );

        ChannelsMaskRemaining[0] &= ChannelsMask[0];
        ChannelsMaskRemaining[1] &= ChannelsMask[1];
        ChannelsMaskRemaining[2] &= ChannelsMask[2];
        ChannelsMaskRemaining[3] &= ChannelsMask[3];
        ChannelsMaskRemaining[4] = ChannelsMask[4];
        ChannelsMaskRemaining[5] = ChannelsMask[5];
    }

    // Update status variables
    *drOut = linkAdrParams.Datarate;
    *txPowOut = linkAdrParams.TxPower;
    *nbRepOut = linkAdrParams.NbRep;
    *nbBytesParsed = bytesProcessed;

    return status;
}

uint8_t LoRaPHYUS915Hybrid::setup_rx_params(RxParamSetupReqParams_t* rxParamSetupReq)
{
    uint8_t status = 0x07;
    uint32_t freq = rxParamSetupReq->Frequency;

    // Verify radio frequency
    if( ( _radio->check_rf_frequency( freq ) == false ) ||
        ( freq < US915_HYBRID_FIRST_RX1_CHANNEL ) ||
        ( freq > US915_HYBRID_LAST_RX1_CHANNEL ) ||
        ( ( ( freq - ( uint32_t ) US915_HYBRID_FIRST_RX1_CHANNEL ) % ( uint32_t ) US915_HYBRID_STEPWIDTH_RX1_CHANNEL ) != 0 ) )
    {
        status &= 0xFE; // Channel frequency KO
    }

    // Verify datarate
    if( val_in_range( rxParamSetupReq->Datarate, US915_HYBRID_RX_MIN_DATARATE, US915_HYBRID_RX_MAX_DATARATE ) == 0 )
    {
        status &= 0xFD; // Datarate KO
    }
    if( ( val_in_range( rxParamSetupReq->Datarate, DR_5, DR_7 ) == 1 ) ||
        ( rxParamSetupReq->Datarate > DR_13 ) )
    {
        status &= 0xFD; // Datarate KO
    }

    // Verify datarate offset
    if( val_in_range( rxParamSetupReq->DrOffset, US915_HYBRID_MIN_RX1_DR_OFFSET, US915_HYBRID_MAX_RX1_DR_OFFSET ) == 0 )
    {
        status &= 0xFB; // Rx1DrOffset range KO
    }

    return status;
}

uint8_t LoRaPHYUS915Hybrid::request_new_channel(NewChannelReqParams_t* newChannelReq)
{
    // Datarate and frequency KO
    return 0;
}

int8_t LoRaPHYUS915Hybrid::setup_tx_params(TxParamSetupReqParams_t* txParamSetupReq)
{
    return -1;
}

uint8_t LoRaPHYUS915Hybrid::dl_channel_request(DlChannelReqParams_t* dlChannelReq)
{
    return 0;
}

int8_t LoRaPHYUS915Hybrid::get_alternate_DR(AlternateDrParams_t* alternateDr)
{
    int8_t datarate = 0;

    // Re-enable 500 kHz default channels
    ReenableChannels( ChannelsMask[4], ChannelsMask );

    if( ( alternateDr->NbTrials & 0x01 ) == 0x01 )
    {
        datarate = DR_4;
    }
    else
    {
        datarate = DR_0;
    }
    return datarate;
}

void LoRaPHYUS915Hybrid::calculate_backoff(CalcBackOffParams_t* calcBackOff)
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

bool LoRaPHYUS915Hybrid::set_next_channel(NextChanParams_t* nextChanParams,
                                          uint8_t* channel, TimerTime_t* time,
                                          TimerTime_t* aggregatedTimeOff)
{
    uint8_t nbEnabledChannels = 0;
    uint8_t delayTx = 0;
    uint8_t enabledChannels[US915_HYBRID_MAX_NB_CHANNELS] = { 0 };
    TimerTime_t nextTxDelay = 0;

    // Count 125kHz channels
    if( num_active_channels( ChannelsMaskRemaining, 0, 4 ) == 0 )
    { // Reactivate default channels
        copy_channel_mask( ChannelsMaskRemaining, ChannelsMask, 4  );
    }
    // Check other channels
    if( nextChanParams->Datarate >= DR_4 )
    {
        if( ( ChannelsMaskRemaining[4] & 0x00FF ) == 0 )
        {
            ChannelsMaskRemaining[4] = ChannelsMask[4];
        }
    }

    if( nextChanParams->AggrTimeOff <= TimerGetElapsedTime( nextChanParams->LastAggrTx ) )
    {
        // Reset Aggregated time off
        *aggregatedTimeOff = 0;

        // Update bands Time OFF
        nextTxDelay = update_band_timeoff( nextChanParams->Joined, nextChanParams->DutyCycleEnabled, Bands, US915_HYBRID_MAX_NB_BANDS );

        // Search how many channels are enabled
        nbEnabledChannels = CountNbOfEnabledChannels( nextChanParams->Datarate,
                                                      ChannelsMaskRemaining, Channels,
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
        // Disable the channel in the mask
        disable_channel( ChannelsMaskRemaining, *channel, US915_HYBRID_MAX_NB_CHANNELS - 8 );

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
        // Datarate not supported by any channel
        *time = 0;
        return false;
    }
}

LoRaMacStatus_t LoRaPHYUS915Hybrid::add_channel(ChannelAddParams_t* channelAdd)
{
    return LORAMAC_STATUS_PARAMETER_INVALID;
}

bool LoRaPHYUS915Hybrid::remove_channel(ChannelRemoveParams_t* channelRemove)
{
    return LORAMAC_STATUS_PARAMETER_INVALID;
}

void LoRaPHYUS915Hybrid::set_tx_cont_mode(ContinuousWaveParams_t* continuousWave)
{
    int8_t txPowerLimited = LimitTxPower( continuousWave->TxPower, Bands[Channels[continuousWave->Channel].Band].TxMaxPower, continuousWave->Datarate, ChannelsMask );
    int8_t phyTxPower = 0;
    uint32_t frequency = Channels[continuousWave->Channel].Frequency;

    // Calculate physical TX power
    phyTxPower = compute_tx_power( txPowerLimited, US915_HYBRID_DEFAULT_MAX_ERP, 0 );

    _radio->set_tx_continuous_wave( frequency, phyTxPower, continuousWave->Timeout );
}

uint8_t LoRaPHYUS915Hybrid::apply_DR_offset(uint8_t downlinkDwellTime, int8_t dr, int8_t drOffset)
{
    int8_t datarate = DatarateOffsetsUS915_HYBRID[dr][drOffset];

    if( datarate < 0 )
    {
        datarate = DR_0;
    }
    return datarate;
}
