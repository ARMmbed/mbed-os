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

#include "LoRaMac.h"
#include "lorastack/mac/LoRaMacMcps.h"

LoRaMacMcps::LoRaMacMcps()
: _lora_mac(NULL), _lora_phy(NULL)
{
}

LoRaMacMcps::~LoRaMacMcps()
{
}

void LoRaMacMcps::activate_mcps_subsystem(LoRaMac *mac, LoRaPHY *phy)
{
    _lora_mac = mac;
    _lora_phy = phy;
}

LoRaMacStatus_t LoRaMacMcps::set_request(McpsReq_t *mcpsRequest,
                                          lora_mac_protocol_params *params)
{

    if (mcpsRequest == NULL || _lora_phy == NULL || _lora_mac == NULL) {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    if (params->LoRaMacState != LORAMAC_IDLE) {
        return LORAMAC_STATUS_BUSY;
    }

    GetPhyParams_t getPhy;
    PhyParam_t phyParam;
    LoRaMacStatus_t status = LORAMAC_STATUS_SERVICE_UNKNOWN;
    LoRaMacHeader_t macHdr;
    VerifyParams_t verify;
    uint8_t fPort = 0;
    void *fBuffer;
    uint16_t fBufferSize;
    int8_t datarate = DR_0;
    bool readyToSend = false;

    macHdr.Value = 0;

    // Before performing any MCPS request, clear the confirmation structure
    memset((uint8_t*) &confirmation, 0, sizeof(confirmation));

    confirmation.Status = LORAMAC_EVENT_INFO_STATUS_ERROR;

    // AckTimeoutRetriesCounter must be reset every time a new request (unconfirmed or confirmed) is performed.
    params->AckTimeoutRetriesCounter = 1;

    switch (mcpsRequest->Type) {
        case MCPS_UNCONFIRMED: {
            readyToSend = true;
            params->AckTimeoutRetries = 1;

            macHdr.Bits.MType = FRAME_TYPE_DATA_UNCONFIRMED_UP;
            fPort = mcpsRequest->Req.Unconfirmed.fPort;
            fBuffer = mcpsRequest->Req.Unconfirmed.fBuffer;
            fBufferSize = mcpsRequest->Req.Unconfirmed.fBufferSize;
            datarate = mcpsRequest->Req.Unconfirmed.Datarate;
            break;
        }
        case MCPS_CONFIRMED: {
            readyToSend = true;
            params->AckTimeoutRetries = mcpsRequest->Req.Confirmed.NbTrials;

            macHdr.Bits.MType = FRAME_TYPE_DATA_CONFIRMED_UP;
            fPort = mcpsRequest->Req.Confirmed.fPort;
            fBuffer = mcpsRequest->Req.Confirmed.fBuffer;
            fBufferSize = mcpsRequest->Req.Confirmed.fBufferSize;
            datarate = mcpsRequest->Req.Confirmed.Datarate;
            break;
        }
        case MCPS_PROPRIETARY: {
            readyToSend = true;
            params->AckTimeoutRetries = 1;

            macHdr.Bits.MType = FRAME_TYPE_PROPRIETARY;
            fBuffer = mcpsRequest->Req.Proprietary.fBuffer;
            fBufferSize = mcpsRequest->Req.Proprietary.fBufferSize;
            datarate = mcpsRequest->Req.Proprietary.Datarate;
            break;
        }
        default:
            break;
    }

    // Filter fPorts
    // TODO: Does not work with PROPRIETARY messages
    //    if( IsFPortAllowed( fPort ) == false )
    //    {
    //        return LORAMAC_STATUS_PARAMETER_INVALID;
    //    }

    // Get the minimum possible datarate
    getPhy.Attribute = PHY_MIN_TX_DR;
    getPhy.UplinkDwellTime = params->sys_params.UplinkDwellTime;
    phyParam = _lora_phy->get_phy_params(&getPhy);

    // Apply the minimum possible datarate.
    // Some regions have limitations for the minimum datarate.
    datarate = MAX(datarate, phyParam.Value);

    if (readyToSend == true) {
        if (params->sys_params.AdrCtrlOn == false) {
            verify.DatarateParams.Datarate = datarate;
            verify.DatarateParams.UplinkDwellTime =
                    params->sys_params.UplinkDwellTime;

            if (_lora_phy->verify(&verify, PHY_TX_DR) == true) {
                params->sys_params.ChannelsDatarate =
                        verify.DatarateParams.Datarate;
            } else {
                return LORAMAC_STATUS_PARAMETER_INVALID;
            }
        }

        status = _lora_mac->Send(&macHdr, fPort, fBuffer, fBufferSize);
        if (status == LORAMAC_STATUS_OK) {
            confirmation.McpsRequest = mcpsRequest->Type;
            params->LoRaMacFlags.Bits.McpsReq = 1;
        } else {
            params->NodeAckRequested = false;
        }
    }

    return status;
}
