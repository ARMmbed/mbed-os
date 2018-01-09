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

Description: LoRaWAN stack layer that controls both MAC and PHY underneath

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )


Copyright (c) 2017, Arm Limited and affiliates.

SPDX-License-Identifier: BSD-3-Clause
*/

#include "lorastack/mac/LoRaMac.h"
#include "lorastack/mac/LoRaMacMib.h"

LoRaMacMib::LoRaMacMib()
: _lora_mac(NULL), _lora_phy(NULL)
{
}

LoRaMacMib::~LoRaMacMib()
{
}

void LoRaMacMib::activate_mib_subsystem(LoRaMac *mac, LoRaPHY *phy)
{
    _lora_mac = mac;
    _lora_phy = phy;
}

LoRaMacStatus_t LoRaMacMib::set_request(MibRequestConfirm_t *mibSet,
                                        lora_mac_protocol_params *params)
{
    if (mibSet == NULL || _lora_phy == NULL || _lora_mac == NULL) {
         return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    LoRaMacStatus_t status = LORAMAC_STATUS_OK;
    ChanMaskSetParams_t chanMaskSet;
    VerifyParams_t verify;


    switch (mibSet->Type) {
        case MIB_DEVICE_CLASS: {
            params->LoRaMacDeviceClass = mibSet->Param.Class;
            switch (params->LoRaMacDeviceClass) {
                case CLASS_A: {
                    // Set the radio into sleep to setup a defined state
                    _lora_phy->put_radio_to_sleep();
                    break;
                }
                case CLASS_B: {
                    break;
                }
                case CLASS_C: {
                    // Set the NodeAckRequested indicator to default
                    params->NodeAckRequested = false;
                    // Set the radio into sleep mode in case we are still in RX mode
                    _lora_phy->put_radio_to_sleep();
                    // Compute Rx2 windows parameters in case the RX2 datarate has changed
                    _lora_phy->compute_rx_win_params(
                            params->sys_params.Rx2Channel.Datarate,
                            params->sys_params.MinRxSymbols,
                            params->sys_params.SystemMaxRxError,
                            &params->RxWindow2Config);
                    _lora_mac->OpenContinuousRx2Window();
                    break;
                }
            }
            break;
        }
        case MIB_NETWORK_JOINED: {
            params->IsLoRaMacNetworkJoined = mibSet->Param.IsNetworkJoined;
            break;
        }
        case MIB_ADR: {
            params->sys_params.AdrCtrlOn = mibSet->Param.AdrEnable;
            break;
        }
        case MIB_NET_ID: {
            params->LoRaMacNetID = mibSet->Param.NetID;
            break;
        }
        case MIB_DEV_ADDR: {
            params->LoRaMacDevAddr = mibSet->Param.DevAddr;
            break;
        }
        case MIB_NWK_SKEY: {
            if (mibSet->Param.NwkSKey != NULL) {
                memcpy(params->keys.LoRaMacNwkSKey, mibSet->Param.NwkSKey,
                       sizeof(params->keys.LoRaMacNwkSKey));
            } else {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_APP_SKEY: {
            if (mibSet->Param.AppSKey != NULL) {
                memcpy(params->keys.LoRaMacAppSKey, mibSet->Param.AppSKey,
                       sizeof(params->keys.LoRaMacAppSKey));
            } else {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_PUBLIC_NETWORK: {
            params->PublicNetwork = mibSet->Param.EnablePublicNetwork;
            _lora_phy->setup_public_network_mode(params->PublicNetwork);
            break;
        }
        case MIB_REPEATER_SUPPORT: {
            params->RepeaterSupport = mibSet->Param.EnableRepeaterSupport;
            break;
        }
        case MIB_RX2_CHANNEL: {
            verify.DatarateParams.Datarate = mibSet->Param.Rx2Channel.Datarate;
            verify.DatarateParams.DownlinkDwellTime =
                    params->sys_params.DownlinkDwellTime;

            if (_lora_phy->verify(&verify, PHY_RX_DR) == true) {
                params->sys_params.Rx2Channel = mibSet->Param.Rx2Channel;

                if ((params->LoRaMacDeviceClass == CLASS_C)
                        && (params->IsLoRaMacNetworkJoined == true)) {
                    // We can only compute the RX window parameters directly, if we are already
                    // in class c mode and joined. We cannot setup an RX window in case of any other
                    // class type.
                    // Set the radio into sleep mode in case we are still in RX mode
                    _lora_phy->put_radio_to_sleep();
                    // Compute Rx2 windows parameters
                    _lora_phy->compute_rx_win_params(
                            params->sys_params.Rx2Channel.Datarate,
                            params->sys_params.MinRxSymbols,
                            params->sys_params.SystemMaxRxError,
                            &params->RxWindow2Config);

                    _lora_mac->OpenContinuousRx2Window();
                }
            } else {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_RX2_DEFAULT_CHANNEL: {
            verify.DatarateParams.Datarate = mibSet->Param.Rx2Channel.Datarate;
            verify.DatarateParams.DownlinkDwellTime =
                    params->sys_params.DownlinkDwellTime;

            if (_lora_phy->verify(&verify, PHY_RX_DR) == true) {
                params->def_sys_params.Rx2Channel =
                        mibSet->Param.Rx2DefaultChannel;
            } else {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_CHANNELS_DEFAULT_MASK: {
            chanMaskSet.ChannelsMaskIn = mibSet->Param.ChannelsMask;
            chanMaskSet.ChannelsMaskType = CHANNELS_DEFAULT_MASK;

            if (_lora_phy->set_channel_mask(&chanMaskSet) == false) {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_CHANNELS_MASK: {
            chanMaskSet.ChannelsMaskIn = mibSet->Param.ChannelsMask;
            chanMaskSet.ChannelsMaskType = CHANNELS_MASK;

            if (_lora_phy->set_channel_mask(&chanMaskSet) == false) {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_CHANNELS_NB_REP: {
            if ((mibSet->Param.ChannelNbRep >= 1)
                    && (mibSet->Param.ChannelNbRep <= 15)) {
                params->sys_params.ChannelsNbRep = mibSet->Param.ChannelNbRep;
            } else {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_MAX_RX_WINDOW_DURATION: {
            params->sys_params.MaxRxWindow = mibSet->Param.MaxRxWindow;
            break;
        }
        case MIB_RECEIVE_DELAY_1: {
            params->sys_params.ReceiveDelay1 = mibSet->Param.ReceiveDelay1;
            break;
        }
        case MIB_RECEIVE_DELAY_2: {
            params->sys_params.ReceiveDelay2 = mibSet->Param.ReceiveDelay2;
            break;
        }
        case MIB_JOIN_ACCEPT_DELAY_1: {
            params->sys_params.JoinAcceptDelay1 =
                    mibSet->Param.JoinAcceptDelay1;
            break;
        }
        case MIB_JOIN_ACCEPT_DELAY_2: {
            params->sys_params.JoinAcceptDelay2 =
                    mibSet->Param.JoinAcceptDelay2;
            break;
        }
        case MIB_CHANNELS_DEFAULT_DATARATE: {
            verify.DatarateParams.Datarate =
                    mibSet->Param.ChannelsDefaultDatarate;

            if (_lora_phy->verify(&verify, PHY_DEF_TX_DR) == true) {
                params->def_sys_params.ChannelsDatarate =
                        verify.DatarateParams.Datarate;
            } else {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_CHANNELS_DATARATE: {
            verify.DatarateParams.Datarate = mibSet->Param.ChannelsDatarate;
            verify.DatarateParams.UplinkDwellTime =
                    params->sys_params.UplinkDwellTime;

            if (_lora_phy->verify(&verify, PHY_TX_DR) == true) {
                params->sys_params.ChannelsDatarate =
                        verify.DatarateParams.Datarate;
            } else {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_CHANNELS_DEFAULT_TX_POWER: {
            verify.TxPower = mibSet->Param.ChannelsDefaultTxPower;

            if (_lora_phy->verify(&verify, PHY_DEF_TX_POWER) == true) {
                params->def_sys_params.ChannelsTxPower = verify.TxPower;
            } else {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_CHANNELS_TX_POWER: {
            verify.TxPower = mibSet->Param.ChannelsTxPower;

            if (_lora_phy->verify(&verify, PHY_TX_POWER) == true) {
                params->sys_params.ChannelsTxPower = verify.TxPower;
            } else {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_UPLINK_COUNTER: {
            params->UpLinkCounter = mibSet->Param.UpLinkCounter;
            break;
        }
        case MIB_DOWNLINK_COUNTER: {
            params->DownLinkCounter = mibSet->Param.DownLinkCounter;
            break;
        }
        case MIB_SYSTEM_MAX_RX_ERROR: {
            params->sys_params.SystemMaxRxError =
                    params->def_sys_params.SystemMaxRxError =
                            mibSet->Param.SystemMaxRxError;
            break;
        }
        case MIB_MIN_RX_SYMBOLS: {
            params->sys_params.MinRxSymbols =
                    params->def_sys_params.MinRxSymbols =
                            mibSet->Param.MinRxSymbols;
            break;
        }
        case MIB_ANTENNA_GAIN: {
            params->sys_params.AntennaGain = mibSet->Param.AntennaGain;
            break;
        }
        default:
            status = LORAMAC_STATUS_SERVICE_UNKNOWN;
            break;
    }

    return status;
}

LoRaMacStatus_t LoRaMacMib::get_request(MibRequestConfirm_t *mibGet,
                                        lora_mac_protocol_params *params)
{
    LoRaMacStatus_t status = LORAMAC_STATUS_OK;
    GetPhyParams_t getPhy;
    PhyParam_t phyParam;

    if( mibGet == NULL )
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    switch( mibGet->Type )
    {
        case MIB_DEVICE_CLASS:
        {
            mibGet->Param.Class = params->LoRaMacDeviceClass;
            break;
        }
        case MIB_NETWORK_JOINED:
        {
            mibGet->Param.IsNetworkJoined = params->IsLoRaMacNetworkJoined;
            break;
        }
        case MIB_ADR:
        {
            mibGet->Param.AdrEnable = params->sys_params.AdrCtrlOn;
            break;
        }
        case MIB_NET_ID:
        {
            mibGet->Param.NetID = params->LoRaMacNetID;
            break;
        }
        case MIB_DEV_ADDR:
        {
            mibGet->Param.DevAddr = params->LoRaMacDevAddr;
            break;
        }
        case MIB_NWK_SKEY:
        {
            mibGet->Param.NwkSKey =params->keys.LoRaMacNwkSKey;
            break;
        }
        case MIB_APP_SKEY:
        {
            mibGet->Param.AppSKey = params->keys.LoRaMacAppSKey;
            break;
        }
        case MIB_PUBLIC_NETWORK:
        {
            mibGet->Param.EnablePublicNetwork = params->PublicNetwork;
            break;
        }
        case MIB_REPEATER_SUPPORT:
        {
            mibGet->Param.EnableRepeaterSupport = params->RepeaterSupport;
            break;
        }
        case MIB_CHANNELS:
        {
            getPhy.Attribute = PHY_CHANNELS;
            phyParam = _lora_phy->get_phy_params( &getPhy );

            mibGet->Param.ChannelList = phyParam.Channels;
            break;
        }
        case MIB_RX2_CHANNEL:
        {
            mibGet->Param.Rx2Channel = params->sys_params.Rx2Channel;
            break;
        }
        case MIB_RX2_DEFAULT_CHANNEL:
        {
            mibGet->Param.Rx2Channel = params->def_sys_params.Rx2Channel;
            break;
        }
        case MIB_CHANNELS_DEFAULT_MASK:
        {
            getPhy.Attribute = PHY_CHANNELS_DEFAULT_MASK;
            phyParam = _lora_phy->get_phy_params( &getPhy );

            mibGet->Param.ChannelsDefaultMask = phyParam.ChannelsMask;
            break;
        }
        case MIB_CHANNELS_MASK:
        {
            getPhy.Attribute = PHY_CHANNELS_MASK;
            phyParam = _lora_phy->get_phy_params( &getPhy );

            mibGet->Param.ChannelsMask = phyParam.ChannelsMask;
            break;
        }
        case MIB_CHANNELS_NB_REP:
        {
            mibGet->Param.ChannelNbRep = params->sys_params.ChannelsNbRep;
            break;
        }
        case MIB_MAX_RX_WINDOW_DURATION:
        {
            mibGet->Param.MaxRxWindow = params->sys_params.MaxRxWindow;
            break;
        }
        case MIB_RECEIVE_DELAY_1:
        {
            mibGet->Param.ReceiveDelay1 = params->sys_params.ReceiveDelay1;
            break;
        }
        case MIB_RECEIVE_DELAY_2:
        {
            mibGet->Param.ReceiveDelay2 = params->sys_params.ReceiveDelay2;
            break;
        }
        case MIB_JOIN_ACCEPT_DELAY_1:
        {
            mibGet->Param.JoinAcceptDelay1 = params->sys_params.JoinAcceptDelay1;
            break;
        }
        case MIB_JOIN_ACCEPT_DELAY_2:
        {
            mibGet->Param.JoinAcceptDelay2 = params->sys_params.JoinAcceptDelay2;
            break;
        }
        case MIB_CHANNELS_DEFAULT_DATARATE:
        {
            mibGet->Param.ChannelsDefaultDatarate = params->def_sys_params.ChannelsDatarate;
            break;
        }
        case MIB_CHANNELS_DATARATE:
        {
            mibGet->Param.ChannelsDatarate = params->sys_params.ChannelsDatarate;
            break;
        }
        case MIB_CHANNELS_DEFAULT_TX_POWER:
        {
            mibGet->Param.ChannelsDefaultTxPower = params->def_sys_params.ChannelsTxPower;
            break;
        }
        case MIB_CHANNELS_TX_POWER:
        {
            mibGet->Param.ChannelsTxPower = params->sys_params.ChannelsTxPower;
            break;
        }
        case MIB_UPLINK_COUNTER:
        {
            mibGet->Param.UpLinkCounter = params->UpLinkCounter;
            break;
        }
        case MIB_DOWNLINK_COUNTER:
        {
            mibGet->Param.DownLinkCounter = params->DownLinkCounter;
            break;
        }
        case MIB_MULTICAST_CHANNEL:
        {
            mibGet->Param.MulticastList = params->MulticastChannels;
            break;
        }
        case MIB_SYSTEM_MAX_RX_ERROR:
        {
            mibGet->Param.SystemMaxRxError = params->sys_params.SystemMaxRxError;
            break;
        }
        case MIB_MIN_RX_SYMBOLS:
        {
            mibGet->Param.MinRxSymbols = params->sys_params.MinRxSymbols;
            break;
        }
        case MIB_ANTENNA_GAIN:
        {
            mibGet->Param.AntennaGain = params->sys_params.AntennaGain;
            break;
        }
        default:
            status = LORAMAC_STATUS_SERVICE_UNKNOWN;
            break;
    }

    return status;
}
