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

lorawan_status_t LoRaMacMcps::set_request(loramac_mcps_req_t *mcpsRequest,
                                          loramac_protocol_params *params)
{

    if (mcpsRequest == NULL || _lora_phy == NULL || _lora_mac == NULL) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    GetPhyParams_t getPhy;
    PhyParam_t phyParam;
    lorawan_status_t status = LORAWAN_STATUS_SERVICE_UNKNOWN;
    loramac_mhdr_t macHdr;
    VerifyParams_t verify;
    uint8_t fPort = 0;
    void *fBuffer;
    uint16_t fBufferSize;
    int8_t datarate = DR_0;
    bool readyToSend = false;

    macHdr.value = 0;

    // Before performing any MCPS request, clear the confirmation structure
    memset((uint8_t*) &confirmation, 0, sizeof(confirmation));

    confirmation.status = LORAMAC_EVENT_INFO_STATUS_ERROR;

    // ack_timeout_retry_counter must be reset every time a new request (unconfirmed or confirmed) is performed.
    params->ack_timeout_retry_counter = 1;

    switch (mcpsRequest->type) {
        case MCPS_UNCONFIRMED: {
            readyToSend = true;
            params->max_ack_timeout_retries = 1;

            macHdr.bits.mtype = FRAME_TYPE_DATA_UNCONFIRMED_UP;
            fPort = mcpsRequest->req.unconfirmed.fport;
            fBuffer = mcpsRequest->f_buffer;
            fBufferSize = mcpsRequest->f_buffer_size;
            datarate = mcpsRequest->req.unconfirmed.data_rate;
            break;
        }
        case MCPS_CONFIRMED: {
            readyToSend = true;
            params->max_ack_timeout_retries = mcpsRequest->req.confirmed.nb_trials;

            macHdr.bits.mtype = FRAME_TYPE_DATA_CONFIRMED_UP;
            fPort = mcpsRequest->req.confirmed.fport;
            fBuffer = mcpsRequest->f_buffer;
            fBufferSize = mcpsRequest->f_buffer_size;
            datarate = mcpsRequest->req.confirmed.data_rate;
            break;
        }
        case MCPS_PROPRIETARY: {
            readyToSend = true;
            params->max_ack_timeout_retries = 1;

            macHdr.bits.mtype = FRAME_TYPE_PROPRIETARY;
            fBuffer = mcpsRequest->f_buffer;
            fBufferSize = mcpsRequest->f_buffer_size;
            datarate = mcpsRequest->req.proprietary.data_rate;
            break;
        }
        default:
            break;
    }

    // Filter fPorts
    // TODO: Does not work with PROPRIETARY messages
    //    if( IsFPortAllowed( fPort ) == false )
    //    {
    //        return LORAWAN_STATUS_PARAMETER_INVALID;
    //    }

    // Get the minimum possible datarate
    getPhy.Attribute = PHY_MIN_TX_DR;
    getPhy.UplinkDwellTime = params->sys_params.uplink_dwell_time;
    phyParam = _lora_phy->get_phy_params(&getPhy);

    // Apply the minimum possible datarate.
    // Some regions have limitations for the minimum datarate.
    datarate = MAX(datarate, (int8_t)phyParam.Value);

    if (readyToSend == true) {
        if (params->sys_params.adr_on == false) {
            verify.DatarateParams.Datarate = datarate;
            verify.DatarateParams.UplinkDwellTime =
                    params->sys_params.uplink_dwell_time;

            if (_lora_phy->verify(&verify, PHY_TX_DR) == true) {
                params->sys_params.channel_data_rate =
                        verify.DatarateParams.Datarate;
            } else {
                return LORAWAN_STATUS_PARAMETER_INVALID;
            }
        }

        status = _lora_mac->Send(&macHdr, fPort, fBuffer, fBufferSize);
        if (status == LORAWAN_STATUS_OK) {
            confirmation.req_type = mcpsRequest->type;
            params->flags.bits.mcps_req = 1;
        } else {
            params->is_node_ack_requested = false;
        }
    }

    return status;
}
