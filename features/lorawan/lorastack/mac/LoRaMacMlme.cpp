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
#include "lorastack/mac/LoRaMacMlme.h"

LoRaMacMlme::LoRaMacMlme()
: _lora_mac(NULL), _lora_phy(NULL), _mac_cmd(NULL)
{
}

LoRaMacMlme::~LoRaMacMlme()
{
}

void LoRaMacMlme::activate_mlme_subsystem(LoRaMac *mac, LoRaPHY *phy,
                                          LoRaMacCommand *cmd)
{
    _lora_mac = mac;
    _lora_phy = phy;
    _mac_cmd = cmd;
}

lorawan_status_t LoRaMacMlme::set_request(loramac_mlme_req_t *request,
                                          loramac_protocol_params *params)
{
    if (request && params && _lora_mac && _lora_phy && _mac_cmd) {

        lorawan_status_t status = LORAWAN_STATUS_SERVICE_UNKNOWN;
        loramac_mhdr_t machdr;

        verification_params_t verify;
        get_phy_params_t get_phy;
        phy_param_t phy_param;


        if (params->mac_state != LORAMAC_IDLE) {
            return LORAWAN_STATUS_BUSY;
        }

        // Before setting a new MLME request, clear the MLME confirmation
        // structure
        memset((uint8_t*) &confirmation, 0, sizeof(confirmation));

        confirmation.status = LORAMAC_EVENT_INFO_STATUS_ERROR;

        switch (request->type) {
            case MLME_JOIN: {
                if ((params->mac_state & LORAMAC_TX_DELAYED)
                        == LORAMAC_TX_DELAYED) {
                    return LORAWAN_STATUS_BUSY;
                }

                if ((request->req.join.dev_eui == NULL)
                        || (request->req.join.app_eui == NULL)
                        || (request->req.join.app_key == NULL)
                        || (request->req.join.nb_trials == 0)) {
                    return LORAWAN_STATUS_PARAMETER_INVALID;
                }

                // Verify the parameter NbTrials for the join procedure
                verify.nb_join_trials = request->req.join.nb_trials;

                if (_lora_phy->verify(&verify, PHY_NB_JOIN_TRIALS) == false) {
                    // Value not supported, get default
                    get_phy.attribute = PHY_DEF_NB_JOIN_TRIALS;
                    phy_param = _lora_phy->get_phy_params(&get_phy);
                    request->req.join.nb_trials = (uint8_t) phy_param.value;
                }

                params->flags.bits.mlme_req = 1;
                confirmation.req_type = request->type;

                params->keys.dev_eui = request->req.join.dev_eui;
                params->keys.app_eui = request->req.join.app_eui;
                params->keys.app_key = request->req.join.app_key;
                params->max_join_request_trials = request->req.join.nb_trials;

                // Reset variable JoinRequestTrials
                params->join_request_trial_counter = 0;

                // Setup header information
                machdr.value = 0;
                machdr.bits.mtype = FRAME_TYPE_JOIN_REQ;

                _lora_mac->reset_mac_parameters();

                params->sys_params.channel_data_rate =
                        _lora_phy->get_alternate_DR(params->join_request_trial_counter + 1);

                status = _lora_mac->send(&machdr, 0, NULL, 0);
                break;
            }
            case MLME_LINK_CHECK: {
                params->flags.bits.mlme_req = 1;
                // LoRaMac will send this command piggy-backed
                confirmation.req_type = request->type;

                status = _mac_cmd->add_mac_command(MOTE_MAC_LINK_CHECK_REQ, 0, 0);
                break;
            }
            case MLME_TXCW: {
                confirmation.req_type = request->type;
                params->flags.bits.mlme_req = 1;
                status = _lora_mac->set_tx_continuous_wave(request->req.cw_tx_mode.timeout);
                break;
            }
            case MLME_TXCW_1: {
                confirmation.req_type = request->type;
                params->flags.bits.mlme_req = 1;
                status = _lora_mac->set_tx_continuous_wave1(request->req.cw_tx_mode.timeout,
                                              request->req.cw_tx_mode.frequency,
                                              request->req.cw_tx_mode.power);
                break;
            }
            default:
                break;
        }

        if (status != LORAWAN_STATUS_OK) {
            params->is_node_ack_requested = false;
            params->flags.bits.mlme_req = 0;
        }

        return status;
    }

    return LORAWAN_STATUS_PARAMETER_INVALID;
}
