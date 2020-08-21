/*
 * Copyright (c) 2015-2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * \file isodep_target.c
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

#define __DEBUG__ 0
#ifndef __MODULE__
#define __MODULE__ "isodep_target.c"
#endif

#include "isodep_target.h"

#include "stack/nfc_errors.h"
#include "transceiver/transceiver.h"

//Private defines
#define RATS 0xE0
#define FSDI_TO_FSD(x) (((x)<5)?(((x)<<3) + 16):((x)<8)?(((x)<<5)-96):256)
#define FSC_TO_FSCI(x) (((x)<=48)?(((x)-16)>>3):((x)<=128)?(((x)+96)>>5):8) //returns the closest lower or equal value
#define DID(x) ((x) & 0x0F)
#define FSDI(x) (((x) >> 4) & 0x0F)

#define FSCI_TO_FSC(x) FSDI_TO_FSD(x)
#define FSD_TO_FSDI(x) FSC_TO_FSCI(x)

//#define DI_TO_D(x) (1 << (x))
#define DI_TO_BITRATE(x) ((RF_BITRATE)((x) + RF_BITRATE_106K))


#define GET_FRAME_TYPE(pcb) ((((pcb) & 0xF0) == 0xD0)?PPS_FRAME:(((pcb) & 0xC0) == (0x00 << 6))?I_FRAME:((((pcb) & 0xC0) == (0x2 << 6))?R_FRAME:S_FRAME))

#define I_BLOCK_PCB 0
#define R_BLOCK_PCB 2
#define S_BLOCK_PCB 3

#define PCB_TYPE(pcb) (((pcb)>>6)&0x03)

#define BUILD_I_BLOCK_PCB(chaining, cid, nad, block_toggle) ( (0x0 << 6) | (((chaining)?1:0) << 4) \
    | (((cid)?1:0) << 3) | (((nad)?1:0) << 2) | (1 << 1) | (((block_toggle)?1:0)) )
#define BUILD_S_BLOCK_PCB(cid, wtx_n_deselect) ( (0x3 << 6) | (((wtx_n_deselect)?0x3:0) << 4) \
    | (((cid)?1:0) << 3) | (1 << 1) )
#define BUILD_R_BLOCK_PCB(cid, block_toggle, nak) ( (0x2 << 6) | (1 <<5) | (((nak)?1:0) << 4) \
    | (((cid)?1:0) << 3) | (1 << 1) | (((block_toggle)?1:0)) )

#define PCB_IS_CID(pcb) (((pcb) & (1 << 3))?true:false)
#define PCB_BLOCK_TOGGLE(pcb) (((pcb) & 1)?true:false)
#define PCB_CHAINING(pcb) (((pcb) & 0x10)?true:false)
#define PCB_NACK(pcb) (((pcb) & 0x10)?true:false)
#define PCB_WTX(pcb) (((pcb)&0x30)==0x30)

#define WTXM_DEFAULT 10

//Parameters
#define FSC 256 //Maximum frame size the PICC (us) can receive -- TODO should be a parameter at some point -- linked to PN512 buffer
#define SFGI 2 //Guard time ~ 1.2ms
//#define FWI 6 //Max time before answer is ~ 19.3ms
#define FWI 14 //Max time before answer is ~ 19.3ms

typedef enum __dep_type dep_type_t;
enum __dep_type {
    dep_type_information,
    dep_type_response,
    dep_type_supervisory,
};

//Local functions
static void dep_init(nfc_tech_isodep_target_t *pIsodepTarget);
static bool dep_ready(nfc_tech_isodep_target_t *pIsodepTarget);

static void dep_req_information(nfc_tech_isodep_target_t *pIsodepTarget, ac_buffer_t *pReq, bool moreInformation, uint8_t blockNumber);
static void dep_req_response(nfc_tech_isodep_target_t *pIsodepTarget, bool ack, uint8_t blockNumber);
static void dep_req_supervisory(nfc_tech_isodep_target_t *pIsodepTarget, bool wtxNDeselect, uint8_t wtxm);

static dep_type_t dep_res_type(nfc_tech_isodep_target_t *pIsodepTarget);
static void dep_res_information(nfc_tech_isodep_target_t *pIsodepTarget, size_t maxLength, ac_buffer_t **ppRes, bool *pMoreInformation, uint8_t *pBlockNumber);
static void dep_res_response(nfc_tech_isodep_target_t *pIsodepTarget, bool *pAck, uint8_t *pBlockNumber);
static void dep_res_supervisory(nfc_tech_isodep_target_t *pIsodepTarget, bool *pWtxNDeselect, uint8_t *pWtxm);

static void dep_disconnected(nfc_tech_isodep_target_t *pIsodepTarget, bool deselected);

static void command_init(nfc_tech_isodep_target_t *pIsodepTarget);

static nfc_err_t command_ats_req(nfc_tech_isodep_target_t *pIsodepTarget);
static nfc_err_t command_dep_req(nfc_tech_isodep_target_t *pIsodepTarget);

static nfc_err_t command_ats_res(nfc_tech_isodep_target_t *pIsodepTarget);
static nfc_err_t command_dep_res(nfc_tech_isodep_target_t *pIsodepTarget);

static void command_reply(nfc_tech_isodep_target_t *pIsodepTarget, bool depWait);
static void command_transceiver_cb(nfc_transceiver_t *pTransceiver, nfc_err_t ret, void *pUserData);

//High-level Target functions
void nfc_tech_isodep_target_init(nfc_tech_isodep_target_t *pIsodepTarget, nfc_transceiver_t *pTransceiver,
                                 ac_buffer_t *pHist, nfc_tech_isodep_disconnected_cb disconnectedCb, void *pUserData)
{
    pIsodepTarget->pTransceiver = pTransceiver;

    pIsodepTarget->pHist = pHist;

    pIsodepTarget->disconnectedCb = disconnectedCb;
    pIsodepTarget->pUserData = pUserData;

    dep_init(pIsodepTarget);
    command_init(pIsodepTarget);
}

nfc_err_t nfc_tech_isodep_target_connect(nfc_tech_isodep_target_t *pIsodepTarget)
{
    NFC_DBG("Connecting");
    pIsodepTarget->commands.state = ISO_DEP_TARGET_COMMANDS_CONNECTING;

    transceiver_set_crc(pIsodepTarget->pTransceiver, true, true);
    command_transceiver_cb(pIsodepTarget->pTransceiver, NFC_OK, pIsodepTarget);

    return NFC_OK;
}

void nfc_tech_isodep_target_disconnect(nfc_tech_isodep_target_t *pIsodepTarget)
{
    // This should not be called within a callback

    transceiver_abort(pIsodepTarget->pTransceiver);

    dep_disconnected(pIsodepTarget, false);
}

nfc_err_t nfc_tech_isodep_target_transmit(nfc_tech_isodep_target_t *pIsodepTarget, ac_istream_t *pStream, nfc_tech_isodep_cb_t cb, void *pUserData)
{
    if (pIsodepTarget->dep.pReqStream != NULL) {
        return NFC_ERR_BUSY;
    }

    pIsodepTarget->dep.pResStream = pStream;
    pIsodepTarget->dep.resCb = cb;
    pIsodepTarget->dep.pResUserData = pUserData;

    //Do we need to start transceiving?
    if (pIsodepTarget->commands.state == ISO_DEP_TARGET_COMMANDS_DEP_REQ_RECVD) {
        command_reply(pIsodepTarget, false); //Force reply
    }

    return NFC_OK;
}

nfc_err_t nfc_tech_isodep_target_receive(nfc_tech_isodep_target_t *pIsodepTarget, ac_ostream_t *pStream, nfc_tech_isodep_cb_t cb, void *pUserData)
{
    if (pIsodepTarget->dep.pResStream != NULL) {
        return NFC_ERR_BUSY;
    }

    pIsodepTarget->dep.pReqStream = pStream;
    pIsodepTarget->dep.reqCb = cb;
    pIsodepTarget->dep.pReqUserData = pUserData;

    return NFC_OK;
}

//DEP Layer
void dep_init(nfc_tech_isodep_target_t *pIsodepTarget)
{
    //ac_buffer_init(&pIsodepTarget->dep.res, NULL, 0);
    pIsodepTarget->dep.pReqStream = NULL;
    pIsodepTarget->dep.pResStream = NULL;

    pIsodepTarget->dep.reqCb = NULL;
    pIsodepTarget->dep.pReqUserData = NULL;

    pIsodepTarget->dep.resCb = NULL;
    pIsodepTarget->dep.pResUserData = NULL;

    pIsodepTarget->dep.blockNumber = 1; //Rule C

    //pIsodepTarget->dep.pduState = ISO_DEP_TARGET_DEP_PDU_IDLE;
    pIsodepTarget->dep.chaining = false;
    pIsodepTarget->dep.frameState = ISO_DEP_TARGET_DEP_FRAME_IDLE;
}

bool dep_ready(nfc_tech_isodep_target_t *pIsodepTarget)
{
    //Anything to send back?
    if (pIsodepTarget->dep.frameState != ISO_DEP_TARGET_DEP_FRAME_INFORMATION_RECEIVED) {
        return true;
    }

    if ((pIsodepTarget->dep.pResStream != NULL)) {
        return true;
    } else {
        return false;
    }
}

void dep_req_information(nfc_tech_isodep_target_t *pIsodepTarget, ac_buffer_t *pReq, bool moreInformation, uint8_t blockNumber)
{
    (void) blockNumber;

    pIsodepTarget->dep.blockNumber++;
    pIsodepTarget->dep.blockNumber %= 2;

    // Note: callbacks can call nfc_tech_isodep_target_transmit() - however we must make sure that we wait AFTER this routine has been processed to actually transmit
    // To do so, reset state to ATS_RES_SENT state
    pIsodepTarget->commands.state = ISO_DEP_TARGET_COMMANDS_ATS_RES_SENT;

    if (!pIsodepTarget->dep.chaining
            && (pIsodepTarget->dep.pResStream != NULL)) {
        //Sent the full frame
        pIsodepTarget->dep.pResStream = NULL;
        pIsodepTarget->dep.resCb((nfc_tech_isodep_t *)pIsodepTarget, NFC_OK, pIsodepTarget->dep.pResUserData);
    }
    if (pIsodepTarget->dep.pReqStream != NULL) {
        // Pull more
        ac_ostream_push(pIsodepTarget->dep.pReqStream, pReq, !moreInformation);
        if (!moreInformation) {
            //Got the full frame
            pIsodepTarget->dep.pReqStream = NULL;
            pIsodepTarget->dep.reqCb((nfc_tech_isodep_t *)pIsodepTarget, NFC_OK, pIsodepTarget->dep.pReqUserData);
        }
    }

    // Update state
    pIsodepTarget->dep.frameState = ISO_DEP_TARGET_DEP_FRAME_INFORMATION_RECEIVED;
    pIsodepTarget->dep.chaining = moreInformation;
}

void dep_req_response(nfc_tech_isodep_target_t *pIsodepTarget, bool ack, uint8_t blockNumber)
{
    if (blockNumber != pIsodepTarget->dep.blockNumber) {
        //Should be NACK
        pIsodepTarget->dep.frameState = ISO_DEP_TARGET_DEP_FRAME_NACK_DIFF_BLOCK_NUMBER_RECEIVED;
    } else {
        if (ack) {
            pIsodepTarget->dep.frameState = ISO_DEP_TARGET_DEP_FRAME_ACK_RECEIVED;
        } else {
            pIsodepTarget->dep.frameState = ISO_DEP_TARGET_DEP_FRAME_NACK_RECEIVED;
        }
    }
}

void dep_req_supervisory(nfc_tech_isodep_target_t *pIsodepTarget, bool wtxNDeselect, uint8_t wtxm)
{
    (void) wtxm;

    if (wtxNDeselect) {
        if ((pIsodepTarget->dep.frameState != ISO_DEP_TARGET_DEP_FRAME_WTX_SENT)) {
            NFC_WARN("Unexpected WTX frame");
        }
        pIsodepTarget->dep.frameState = ISO_DEP_TARGET_DEP_FRAME_WTX_RECEIVED;
    } else {
        pIsodepTarget->dep.frameState = ISO_DEP_TARGET_DEP_FRAME_DESELECT_RECEIVED;
    }
}

dep_type_t dep_res_type(nfc_tech_isodep_target_t *pIsodepTarget)
{
    dep_type_t depType;
    switch (pIsodepTarget->dep.frameState) {
        case ISO_DEP_TARGET_DEP_FRAME_DESELECT_RECEIVED:
            depType = dep_type_supervisory; //Deselect
            break;
        case ISO_DEP_TARGET_DEP_FRAME_INFORMATION_RECEIVED:
        case ISO_DEP_TARGET_DEP_FRAME_WTX_RECEIVED:
            if (pIsodepTarget->dep.chaining) { //Need to ack?
                depType = dep_type_response;
            } else if (pIsodepTarget->dep.pResStream != NULL) { //Anything to send back?
                depType = dep_type_information;
            } else {
                depType = dep_type_supervisory; //WTX
            }
            break;
        case ISO_DEP_TARGET_DEP_FRAME_ACK_RECEIVED:
            if ((pIsodepTarget->dep.pResStream != NULL) && (pIsodepTarget->dep.chaining)) {
                depType = dep_type_information;
            } else {
                depType = dep_type_supervisory; //WTX
            }
            break;
        case ISO_DEP_TARGET_DEP_FRAME_NACK_DIFF_BLOCK_NUMBER_RECEIVED:
            depType = dep_type_response; //Should send ACK
            break;
        case ISO_DEP_TARGET_DEP_FRAME_NACK_RECEIVED:
            depType = dep_type_information;
            break;
        default:
            depType = dep_type_supervisory; //ATN
            break;
    }
    return depType;
}

void dep_res_information(nfc_tech_isodep_target_t *pIsodepTarget, size_t maxLength, ac_buffer_t **ppRes, bool *pMoreInformation, uint8_t *pBlockNumber)
{
    *pBlockNumber = pIsodepTarget->dep.blockNumber;
    if (pIsodepTarget->dep.frameState != ISO_DEP_TARGET_DEP_FRAME_NACK_RECEIVED) {
        if (pIsodepTarget->dep.pResStream != NULL) {
            bool lastFrame = true;
            ac_istream_pull(pIsodepTarget->dep.pResStream, &pIsodepTarget->dep.res, &lastFrame, maxLength);
            pIsodepTarget->dep.chaining = !lastFrame;
        }
    } else {
        //Retransmit previous frame (leave it as it is)
    }
    *ppRes = &pIsodepTarget->dep.res;
    *pMoreInformation = pIsodepTarget->dep.chaining;
    pIsodepTarget->dep.frameState = ISO_DEP_TARGET_DEP_FRAME_INFORMATION_SENT;
}

void dep_res_response(nfc_tech_isodep_target_t *pIsodepTarget, bool *pAck, uint8_t *pBlockNumber)
{
    //Continue chaining or send ACK
    *pAck = true;
    *pBlockNumber = pIsodepTarget->dep.blockNumber;
    pIsodepTarget->dep.frameState = ISO_DEP_TARGET_DEP_FRAME_ACK_SENT;
}

void dep_res_supervisory(nfc_tech_isodep_target_t *pIsodepTarget, bool *pWtxNDeselect, uint8_t *pWtxm)
{
    if (pIsodepTarget->dep.frameState == ISO_DEP_TARGET_DEP_FRAME_DESELECT_RECEIVED) {
        *pWtxNDeselect = false;
        pIsodepTarget->dep.frameState = ISO_DEP_TARGET_DEP_FRAME_DESELECT_SENT;
    } else {
        *pWtxNDeselect = true;
        *pWtxm = WTXM_DEFAULT;
        pIsodepTarget->dep.frameState = ISO_DEP_TARGET_DEP_FRAME_WTX_SENT;
    }
}

void dep_disconnected(nfc_tech_isodep_target_t *pIsodepTarget, bool deselected)
{
    //Call callbacks if needed
    if (pIsodepTarget->dep.pReqStream != NULL) {
        pIsodepTarget->dep.reqCb((nfc_tech_isodep_t *)pIsodepTarget, NFC_ERR_DISCONNECTED, pIsodepTarget->dep.pReqUserData);
        pIsodepTarget->dep.pReqStream = NULL;
    }
    if (pIsodepTarget->dep.pReqStream != NULL) {
        pIsodepTarget->dep.resCb((nfc_tech_isodep_t *)pIsodepTarget, NFC_ERR_DISCONNECTED, pIsodepTarget->dep.pResUserData);
        pIsodepTarget->dep.pResStream = NULL;
    }
    if (pIsodepTarget->commands.state != ISO_DEP_TARGET_COMMANDS_DISCONNECTED) {
        pIsodepTarget->commands.state = ISO_DEP_TARGET_COMMANDS_DISCONNECTED;
        pIsodepTarget->disconnectedCb((nfc_tech_isodep_t *)pIsodepTarget, deselected, pIsodepTarget->pUserData);
    }
}

//Commands Layer
void command_init(nfc_tech_isodep_target_t *pIsodepTarget)
{
    ac_buffer_builder_init(&pIsodepTarget->commands.respBldr, pIsodepTarget->commands.respBuf, sizeof(pIsodepTarget->commands.respBuf));
    pIsodepTarget->commands.pReq = NULL;
    // Update if/when we support DIDs
    //pIsodepTarget->commands.did = 0;
    //pIsodepTarget->commands.didUsed = false;
    pIsodepTarget->commands.state = ISO_DEP_TARGET_COMMANDS_DISCONNECTED;
    pIsodepTarget->commands.inPayloadSize = 0;
}

nfc_err_t command_ats_req(nfc_tech_isodep_target_t *pIsodepTarget)
{
    //Check we are in a correct state -- this should be the first command received
    if (pIsodepTarget->commands.state != ISO_DEP_TARGET_COMMANDS_CONNECTING) {
        return NFC_ERR_PROTOCOL;
    }

    if (ac_buffer_reader_readable(pIsodepTarget->commands.pReq) < 2) {
        NFC_ERR("Payload too short");
        return NFC_ERR_PROTOCOL;
    }

    ac_buffer_read_n_skip(pIsodepTarget->commands.pReq, 1);

    uint8_t b = ac_buffer_read_nu8(pIsodepTarget->commands.pReq);

    //Save DID -- not supported for now
    //pIsodepTarget->commands.did = DID(b);

    uint8_t fsdi = FSDI(b);
    pIsodepTarget->commands.inPayloadSize = FSDI_TO_FSD(fsdi);

    pIsodepTarget->commands.state = ISO_DEP_TARGET_COMMANDS_ATS_REQ_RECVD;

    return NFC_OK;
}

nfc_err_t command_dep_req(nfc_tech_isodep_target_t *pIsodepTarget)
{
    if (pIsodepTarget->commands.state < ISO_DEP_TARGET_COMMANDS_ATS_RES_SENT) {
        return NFC_ERR_PROTOCOL;
    }

    if (ac_buffer_reader_readable(pIsodepTarget->commands.pReq) < 1) {
        NFC_ERR("Payload too short");
        return NFC_ERR_PROTOCOL;
    }

    uint8_t pcb = ac_buffer_read_nu8(pIsodepTarget->commands.pReq);

    // Udpate if/when we support DIDs
    /*
      if( pfb & PFB_DID )
      {
        uint8_t did = ac_buffer_read_nu8(pIsodepTarget->commands.pReq);
        if( pIsodepTarget->commands.did != did )
        {
          //Not for us
          return NFC_ERR_PROTOCOL;
        }
        pIsodepTarget->commands.didUsed = true;
      }
      else
      {
        pIsodepTarget->commands.didUsed = false;
      }

      if( pfb & PFB_NAD )
      {
        ac_buffer_read_nu8(pIsodepTarget->commands.pReq); //Skip NAD
      }
    */

    uint8_t wtxm = 0;
    switch (PCB_TYPE(pcb)) {
        case I_BLOCK_PCB:
            dep_req_information(pIsodepTarget, pIsodepTarget->commands.pReq, PCB_CHAINING(pcb), PCB_BLOCK_TOGGLE(pcb));
            break;
        case R_BLOCK_PCB:
            dep_req_response(pIsodepTarget, !PCB_NACK(pcb), PCB_BLOCK_TOGGLE(pcb));
            break;
        case S_BLOCK_PCB:
            if (PCB_WTX(pcb)) {
                if (ac_buffer_reader_readable(pIsodepTarget->commands.pReq) < 1) {
                    NFC_ERR("Payload too short");
                    return NFC_ERR_PROTOCOL;
                }
                wtxm = ac_buffer_read_nu8(pIsodepTarget->commands.pReq);
            }
            dep_req_supervisory(pIsodepTarget, PCB_WTX(pcb), wtxm);
            break;
        default:
            NFC_ERR("PCB is invalid");
            return NFC_ERR_PROTOCOL;
    }

    pIsodepTarget->commands.state = ISO_DEP_TARGET_COMMANDS_DEP_REQ_RECVD;

    return NFC_OK;
}

nfc_err_t command_ats_res(nfc_tech_isodep_target_t *pIsodepTarget)
{
    //Send ATS back
    if (ac_buffer_builder_writable(&pIsodepTarget->commands.respBldr) < 5) {
        return NFC_ERR_BUFFER_TOO_SMALL;
    }

    ac_buffer_builder_write_nu8(&pIsodepTarget->commands.respBldr, (5 + ac_buffer_size(pIsodepTarget->pHist)) & 0xFF);

    //T0
    ac_buffer_builder_write_nu8(&pIsodepTarget->commands.respBldr, (0x7 << 4) | FSC_TO_FSCI(FSC));   //TA(1), TB(1) and TC(1) are transmitted

    //TA(1)
    //For now only 106kbps supported
    ac_buffer_builder_write_nu8(&pIsodepTarget->commands.respBldr, (1 << 7) | (0x0 << 4) | 0x0);

    //TODO when supporting other bitrates
    //ac_buffer_builder_write_nu8(&pIsodepTarget->commands.respBldr, (0 << 7) | (0x3 << 4) | 0x3); //106, 212, 414 kbps bitrates

    //TB(1)
    ac_buffer_builder_write_nu8(&pIsodepTarget->commands.respBldr, (FWI << 4) | SFGI); //Specify guard-time and time between frames

    //TC(1)
    ac_buffer_builder_write_nu8(&pIsodepTarget->commands.respBldr, (0 << 1) | 0); //DID not supported, NAD not supported

    ac_buffer_set_next(ac_buffer_builder_buffer(&pIsodepTarget->commands.respBldr), pIsodepTarget->pHist); //Queue general bytes

    pIsodepTarget->commands.state = ISO_DEP_TARGET_COMMANDS_ATS_RES_SENT;

    //TODO PPS

    return NFC_OK;
}

nfc_err_t command_dep_res(nfc_tech_isodep_target_t *pIsodepTarget)
{
    uint8_t pcb = 0;

    // If/when supporting DIDs
    /*
    if( pIsodepTarget->commands.didUsed )
    {
      pcb |= PFB_DID;
    }*/

    ac_buffer_t *pDepBuf = NULL;
    bool moreInformation = false;
    bool ack = false;
    bool wtxNDeselect = false;
    uint8_t wtxm = 0;
    uint8_t blockNumber = 0;

    size_t maxLength = pIsodepTarget->commands.inPayloadSize - 1;

    switch (dep_res_type(pIsodepTarget)) {
        case dep_type_information:
            dep_res_information(pIsodepTarget, maxLength, &pDepBuf, &moreInformation, &blockNumber);
            pcb = BUILD_I_BLOCK_PCB(moreInformation, false, false, blockNumber);
            break;
        case dep_type_response:
            dep_res_response(pIsodepTarget, &ack, &blockNumber);
            pcb = BUILD_R_BLOCK_PCB(0, blockNumber, !ack);
            break;
        case dep_type_supervisory:
            dep_res_supervisory(pIsodepTarget, &wtxNDeselect, &wtxm);
            pcb = BUILD_S_BLOCK_PCB(0, wtxNDeselect);
            break;
    }

    ac_buffer_builder_write_nu8(&pIsodepTarget->commands.respBldr, pcb);
    /*
      if( pIsodepTarget->commands.didUsed )
      {
        ac_buffer_builder_write_nu8(&pIsodepTarget->commands.respBldr, pIsodepTarget->commands.did);
      }
    */
    if (pDepBuf != NULL) {
        ac_buffer_set_next(ac_buffer_builder_buffer(&pIsodepTarget->commands.respBldr), pDepBuf);
    } else if (wtxNDeselect) {
        ac_buffer_builder_write_nu8(&pIsodepTarget->commands.respBldr, wtxm);
    }

    pIsodepTarget->commands.state = ISO_DEP_TARGET_COMMANDS_DEP_RES_SENT;

    return NFC_OK;
}

void command_reply(nfc_tech_isodep_target_t *pIsodepTarget, bool depWait)
{
    nfc_err_t ret;

    //Check whether we want to reply or wait for the higher layer to send us something
    if ((pIsodepTarget->commands.state == ISO_DEP_TARGET_COMMANDS_DEP_REQ_RECVD) && depWait && !dep_ready(pIsodepTarget)) {
        return;
    }

    //Reply
    ac_buffer_builder_reset(&pIsodepTarget->commands.respBldr);

    switch (pIsodepTarget->commands.state) {
        case ISO_DEP_TARGET_COMMANDS_ATS_REQ_RECVD:
            ret = command_ats_res(pIsodepTarget);
            break;
        case ISO_DEP_TARGET_COMMANDS_DEP_REQ_RECVD:
            ret = command_dep_res(pIsodepTarget);
            break;
        default:
            NFC_ERR("Unknown state %d", pIsodepTarget->commands.state);
            //Go back to receive mode
            nfc_transceiver_transceive(pIsodepTarget->pTransceiver, command_transceiver_cb, pIsodepTarget);
            return;
    }

    if (ret) {
        NFC_ERR("Error %d", ret);
        //Go back to receive mode
        nfc_transceiver_transceive(pIsodepTarget->pTransceiver, command_transceiver_cb, pIsodepTarget);
        return;
    }

    NFC_DBG("Transceive");

    if (pIsodepTarget->dep.frameState == ISO_DEP_TARGET_DEP_FRAME_DESELECT_SENT) {
        transceiver_set_transceive_options(pIsodepTarget->pTransceiver, true, false, true);
    } else {
        transceiver_set_transceive_options(pIsodepTarget->pTransceiver, true, true, false);
    }

    NFC_DBG_BLOCK(ac_buffer_dump(ac_buffer_builder_buffer(&pIsodepTarget->commands.respBldr));)

    //Send next frame
    transceiver_set_write(pIsodepTarget->pTransceiver, ac_buffer_builder_buffer(&pIsodepTarget->commands.respBldr));
    nfc_transceiver_transceive(pIsodepTarget->pTransceiver, command_transceiver_cb, pIsodepTarget);

    NFC_DBG("Processed");
}

void command_transceiver_cb(nfc_transceiver_t *pTransceiver, nfc_err_t ret, void *pUserData)
{
    nfc_tech_isodep_target_t *pIsodepTarget = (nfc_tech_isodep_target_t *) pUserData;

    if (ret == NFC_ERR_ABORTED) {
        // Just return
        return;
    }

    if (pIsodepTarget->dep.frameState == ISO_DEP_TARGET_DEP_FRAME_DESELECT_SENT) {
        NFC_DBG("Deselect sent and re-polled: %u", ret);
        //We are now disconnected (deselected)
        //Reset status
        dep_init(pIsodepTarget);
        command_init(pIsodepTarget);

        transceiver_set_crc(pIsodepTarget->pTransceiver, true, true);
        pIsodepTarget->commands.state = ISO_DEP_TARGET_COMMANDS_CONNECTING;

        //Call so that we can reinit higher layer
        dep_disconnected(pIsodepTarget, true); //This will call us again
        return;
    }

    //Prepare default empty reply
    transceiver_set_write(pTransceiver, NULL);
    transceiver_set_transceive_options(pTransceiver, false, true, false);

    if (ret == NFC_ERR_FIELD) {
        NFC_WARN("Lost initiator");
        dep_disconnected(pIsodepTarget, false);
        return;
    } else if (ret) {
        //We should ignore this error and wait for another frame
        NFC_WARN("Got invalid frame (error %d)", ret);

        nfc_transceiver_transceive(pTransceiver, command_transceiver_cb, pIsodepTarget);
        return;
    }

    NFC_DBG("Reading data from initiator");
    ac_buffer_t *pDataInitiator = transceiver_get_read(pTransceiver); //In buffer

    NFC_DBG_BLOCK(ac_buffer_dump(pDataInitiator);)

    //Framing is handled by transceiver
    if ((ac_buffer_reader_readable(pDataInitiator) < 1)) {
        NFC_ERR("Empty initiator message");

        //Go back to receive mode
        nfc_transceiver_transceive(pTransceiver, command_transceiver_cb, pIsodepTarget);
        return;
    }

    pIsodepTarget->commands.pReq = pDataInitiator;

    //Duplicate to peek on req
    ac_buffer_t dataInitiatorDup;
    ac_buffer_dup(&dataInitiatorDup, pDataInitiator);
    uint8_t req = ac_buffer_read_nu8(&dataInitiatorDup);

    switch (req) {
        case RATS:
            ret = command_ats_req(pIsodepTarget);
            break;
        default:
            ret = command_dep_req(pIsodepTarget);
            break;
    }

    if (ret) {
        NFC_ERR("Error %d", ret);

        //Go back to receive mode
        nfc_transceiver_transceive(pTransceiver, command_transceiver_cb, pIsodepTarget);
        return;
    }

    NFC_DBG("Reply");

    //Reply
    command_reply(pIsodepTarget, true); //Make sure we send a WTX frame if we cannot respond straight away
}



