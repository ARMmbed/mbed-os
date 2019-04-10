/* mbed Microcontroller Library
 *
 * Copyright 2019 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
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
 *
 */

#include <string.h>
#include "wsf_types.h"
#include "wsf_msg.h"
#include "wsf_buf.h"
#include "wsf_cs.h"
#include "hci_api.h"
#include "CyVscCmd.h"
#include "rtos/rtos.h"

uint16_t vsc_event_mask = 0;
wsfQueue_t      vscCmdQueue;
wsfQueue_t      vscRspQueue;
static volatile int number_of_msg = 0;
static int cy_vsc_init = 0;

typedef struct {
    void        *pNext;
    uint16_t opcode;
    cy_vsc_callback_t *vsc_cb;
} cy_vendor_specific_cmd_t;

typedef struct {
    void        *pNext;
    uint16_t    opcode;
    uint8_t     status;
    uint8_t     param_len;
    uint8_t     *param;
} cy_vendor_specific_rsp_t;

Thread threadVsc(osPriorityNormal, OS_STACK_SIZE, NULL, "VSC_CMD");

static cy_vendor_specific_cmd_t *cy_find_vsc_form_opcode (uint16_t opcode)
{
    if(!(WsfQueueEmpty(&vscCmdQueue)))
    {
        cy_vendor_specific_cmd_t *cy_cmd = (cy_vendor_specific_cmd_t *)WsfQueueDeq(&vscCmdQueue);
        if( cy_cmd != NULL )
        {
            if(opcode == cy_cmd->opcode)
            {
                return cy_cmd;
            }
        }
    }
    return NULL;
}

void vscThread(void)
{
    while(1)
    {
        if(number_of_msg)
        {
            WSF_CS_INIT();
            WSF_CS_ENTER();
            number_of_msg--;
            WSF_CS_EXIT();
            cy_vendor_specific_rsp_t *cy_rsp = (cy_vendor_specific_rsp_t *)WsfQueueDeq(&vscRspQueue);
            if( cy_rsp != NULL )
            {
                cy_vendor_specific_cmd_t *cy_cmd = cy_find_vsc_form_opcode(cy_rsp->opcode);
                if (cy_cmd != NULL)
                {
                    cy_rsp->opcode &= (~(0x3F << 10));
                    if (cy_cmd->vsc_cb != NULL)
                    {
                         (*(cy_cmd->vsc_cb))(cy_rsp->opcode, cy_rsp->status, cy_rsp->param_len, cy_rsp->param);
                    }
                    WsfBufFree(cy_cmd);
                }
                WsfBufFree(cy_rsp);
            }
        }
    }
}

bool cy_send_vendor_specific_cmd(uint16_t opcode, uint8_t param_len, uint8_t *param, cy_vsc_callback_t *vsc_cb)
{
    if (cy_vsc_init == 0)
    {
        cy_vsc_init = 1;
        // Create Thread to send the callbacks
        threadVsc.start(vscThread);

        //Create Queue to store callbacks
        WSF_QUEUE_INIT(&vscCmdQueue);
        WSF_QUEUE_INIT(&vscRspQueue);
    }
    opcode |= (0x3F << 10);

    cy_vendor_specific_cmd_t *vsc_cmd = (cy_vendor_specific_cmd_t *)WsfBufAlloc(sizeof(cy_vendor_specific_cmd_t));
    if (vsc_cmd != NULL )
    {
        vsc_cmd->opcode = opcode;
        vsc_cmd->vsc_cb = vsc_cb;
        WsfQueueEnq(&vscCmdQueue, vsc_cmd);
        HciVendorSpecificCmd(opcode, param_len, param);
        return true;
    }
    return false;
}

extern "C" {
void cy_post_vsc_complete_event(uint16_t opcode, uint8_t status, uint16_t data_len, uint8_t* data)
{
    if(cy_vsc_init)
    {
        cy_vendor_specific_rsp_t *cmd_rsp = (cy_vendor_specific_rsp_t *)WsfBufAlloc(sizeof(cy_vendor_specific_rsp_t) + data_len);
        if(cmd_rsp != NULL)
        {

            cmd_rsp->opcode = opcode;
            cmd_rsp->status = status;
            cmd_rsp->param_len = data_len;
            cmd_rsp->param  = (uint8_t *)(cmd_rsp + 1);

            memcpy((void *)cmd_rsp->param, data, data_len);
            WsfQueueEnq(&vscRspQueue, cmd_rsp);

            WSF_CS_INIT();
            WSF_CS_ENTER();
            number_of_msg++;
            WSF_CS_EXIT();
        }
    }
}
}