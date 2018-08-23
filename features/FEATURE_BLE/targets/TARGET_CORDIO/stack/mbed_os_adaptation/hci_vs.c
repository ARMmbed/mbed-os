/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "hci_mbed_os_adaptation.h"
#include "hci_defs.h"
#include "hci_api.h"
#include "wsf_os.h"
#include "hci_main.h"

void hciCoreResetStart(void)
{
    // forward to mbed OS
    hci_mbed_os_start_reset_sequence();
}

void hciCoreResetSequence(uint8_t *pMsg) 
{ 
    // forward to mbed os 
    hci_mbed_os_handle_reset_sequence(pMsg);
}

void hci_mbed_os_signal_reset_sequence_done(void) { 
    /* last command in sequence; set resetting state and call callback */
    wsfMsgHdr_t    hdr;
    hciCb.resetting = FALSE;
    hdr.param = 0;
    hdr.event = HCI_RESET_SEQ_CMPL_CBACK_EVT;
    (*hciCb.evtCback)((hciEvt_t *) &hdr);
}

/**
 * Perform internal HCI processing of vendor specific command complete events.
 * @param opcode HCI command opcode.
 * @param pMsg Pointer to input HCI event parameter byte stream.
 * @param len Parameter byte stream length.
 *
 * @return HCI callback event code or zero.
 *
 * @note not forwarded to mbed OS at the moment
 */
uint8_t hciCoreVsCmdCmplRcvd(uint16_t opcode, uint8_t *pMsg, uint8_t len)
{
    return HCI_VENDOR_SPEC_CMD_CMPL_CBACK_EVT;
}

uint8_t hciCoreVsEvtRcvd(uint8_t *p, uint8_t len)
{
    // supposed to process vendor specific event. 
    // disabled at the moment.
    return HCI_VENDOR_SPEC_EVT;
}


uint8_t hciCoreHwErrorRcvd(uint8_t *p)
{
    // note: not use by the stack at the moment s
    return 0;
}

void HciVsInit(uint8_t param)
{
    // forward to mbed os if necessary 
}
