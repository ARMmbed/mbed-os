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

LoRaMacStatus_t LoRaMacMlme::set_request(MlmeReq_t *mlmeRequest,
                                          lora_mac_protocol_params *params)
{
    if (mlmeRequest && params && _lora_mac && _lora_phy && _mac_cmd) {

        LoRaMacStatus_t status = LORAMAC_STATUS_SERVICE_UNKNOWN;
        LoRaMacHeader_t macHdr;
        AlternateDrParams_t altDr;
        VerifyParams_t verify;
        GetPhyParams_t getPhy;
        PhyParam_t phyParam;


        if (params->LoRaMacState != LORAMAC_IDLE) {
            return LORAMAC_STATUS_BUSY;
        }

        // Before setting a new MLME request, clear the MLME confirmation
        // structure
        memset((uint8_t*) &confirmation, 0, sizeof(confirmation));

        confirmation.Status = LORAMAC_EVENT_INFO_STATUS_ERROR;

        switch (mlmeRequest->Type) {
            case MLME_JOIN: {
                if ((params->LoRaMacState & LORAMAC_TX_DELAYED)
                        == LORAMAC_TX_DELAYED) {
                    return LORAMAC_STATUS_BUSY;
                }

                if ((mlmeRequest->Req.Join.DevEui == NULL)
                        || (mlmeRequest->Req.Join.AppEui == NULL)
                        || (mlmeRequest->Req.Join.AppKey == NULL)
                        || (mlmeRequest->Req.Join.NbTrials == 0)) {
                    return LORAMAC_STATUS_PARAMETER_INVALID;
                }

                // Verify the parameter NbTrials for the join procedure
                verify.NbJoinTrials = mlmeRequest->Req.Join.NbTrials;

                if (_lora_phy->verify(&verify, PHY_NB_JOIN_TRIALS) == false) {
                    // Value not supported, get default
                    getPhy.Attribute = PHY_DEF_NB_JOIN_TRIALS;
                    phyParam = _lora_phy->get_phy_params(&getPhy);
                    mlmeRequest->Req.Join.NbTrials = (uint8_t) phyParam.Value;
                }

                params->LoRaMacFlags.Bits.MlmeReq = 1;
                confirmation.MlmeRequest = mlmeRequest->Type;

                params->keys.LoRaMacDevEui = mlmeRequest->Req.Join.DevEui;
                params->keys.LoRaMacAppEui = mlmeRequest->Req.Join.AppEui;
                params->keys.LoRaMacAppKey = mlmeRequest->Req.Join.AppKey;
                params->MaxJoinRequestTrials = mlmeRequest->Req.Join.NbTrials;

                // Reset variable JoinRequestTrials
                params->JoinRequestTrials = 0;

                // Setup header information
                macHdr.Value = 0;
                macHdr.Bits.MType = FRAME_TYPE_JOIN_REQ;

                _lora_mac->ResetMacParameters();

                altDr.NbTrials = params->JoinRequestTrials + 1;

                params->sys_params.ChannelsDatarate =
                        _lora_phy->get_alternate_DR(&altDr);

                status = _lora_mac->Send(&macHdr, 0, NULL, 0);
                break;
            }
            case MLME_LINK_CHECK: {
                params->LoRaMacFlags.Bits.MlmeReq = 1;
                // LoRaMac will send this command piggy-backed
                confirmation.MlmeRequest = mlmeRequest->Type;

                status = _mac_cmd->AddMacCommand(MOTE_MAC_LINK_CHECK_REQ, 0, 0);
                break;
            }
            case MLME_TXCW: {
                confirmation.MlmeRequest = mlmeRequest->Type;
                params->LoRaMacFlags.Bits.MlmeReq = 1;
                status = _lora_mac->SetTxContinuousWave(mlmeRequest->Req.TxCw.Timeout);
                break;
            }
            case MLME_TXCW_1: {
                confirmation.MlmeRequest = mlmeRequest->Type;
                params->LoRaMacFlags.Bits.MlmeReq = 1;
                status = _lora_mac->SetTxContinuousWave1(mlmeRequest->Req.TxCw.Timeout,
                                              mlmeRequest->Req.TxCw.Frequency,
                                              mlmeRequest->Req.TxCw.Power);
                break;
            }
            default:
                break;
        }

        if (status != LORAMAC_STATUS_OK) {
            params->NodeAckRequested = false;
            params->LoRaMacFlags.Bits.MlmeReq = 0;
        }

        return status;
    }

    return LORAMAC_STATUS_PARAMETER_INVALID;
}
