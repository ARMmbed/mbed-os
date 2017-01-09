/* mbed USBHost Library
 * Copyright (c) 2006-2013 ARM Limited
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
#if defined(TARGET_STM) && defined(USBHOST_OTHER)

#include "dbg.h"
#include "USBEndpoint.h"
extern uint32_t HAL_HCD_HC_GetMaxPacket(HCD_HandleTypeDef *hhcd, uint8_t chn_num);
extern uint32_t HAL_HCD_HC_GetType(HCD_HandleTypeDef *hhcd, uint8_t chn_num);


void USBEndpoint::init(HCED * hced_, ENDPOINT_TYPE type_, ENDPOINT_DIRECTION dir_, uint32_t size, uint8_t ep_number, HCTD* td_list_[2])
{
    hced = hced_;
    type = type_;
    dir = dir_;
    setup = (type == CONTROL_ENDPOINT) ? true : false;

    //TDs have been allocated by the host
    memcpy((HCTD**)td_list, td_list_, sizeof(HCTD*)*2); //TODO: Maybe should add a param for td_list size... at least a define
    memset(td_list_[0], 0, sizeof(HCTD));
    memset(td_list_[1], 0, sizeof(HCTD));

    td_list[0]->ep = this;
    td_list[1]->ep = this;

    address = (ep_number & 0x7F) | ((dir - 1) << 7);
    this->size = size;
    this->ep_number = ep_number;
    transfer_len = 0;
    transferred = 0;
    buf_start = 0;
    nextEp = NULL;

    td_current = td_list[0];
    td_next = td_list[1];

    intf_nb = 0;

    state = USB_TYPE_IDLE;
}
void USBEndpoint::setSize(uint32_t size)
{
    this->size = size;
}


void USBEndpoint::setDeviceAddress(uint8_t addr)
{
    this->device_address = addr;
    HAL_HCD_HC_Init((HCD_HandleTypeDef*)hced->hhcd,hced->ch_num, address, addr, HCD_SPEED_FULL,  type, size);
}

void USBEndpoint::setSpeed(uint8_t speed)
{
    this->speed = speed;
}



void USBEndpoint::setState(uint8_t st) {
    if (st > 18)
        return;
    if (state == USB_TYPE_FREE) HAL_HCD_HC_Halt((HCD_HandleTypeDef*)hced->hhcd, hced->ch_num);

    state = (USB_TYPE)st;
}


extern uint32_t HAL_HCD_HC_GetMaxPacket(HCD_HandleTypeDef *hhcd, uint8_t chn_num);
extern uint32_t HAL_HCD_HC_GetType(HCD_HandleTypeDef *hhcd, uint8_t chn_num);

 
USB_TYPE USBEndpoint::queueTransfer()
{
    HCD_HandleTypeDef *hhcd = (HCD_HandleTypeDef*)hced->hhcd;
    uint32_t *addr = &((uint32_t *)hhcd->pData)[hced->ch_num];
    uint32_t type = HAL_HCD_HC_GetType(hhcd, hced->ch_num);
    uint32_t max_size =  HAL_HCD_HC_GetMaxPacket(hhcd, hced->ch_num);
    /*  if a packet is queue on disconnected ; no solution for now */
    if(*addr == (uint32_t) -1) {
        /* set td as disconnected  */
        td_current->state =  USB_TYPE_DISCONNECTED;
        return USB_TYPE_DISCONNECTED;
    }
    MBED_ASSERT(*addr ==0);
    if ((type == EP_TYPE_BULK) || (type== EP_TYPE_CTRL)) {
        transfer_len =   td_current->size <= max_size ? td_current->size : max_size;
        transferred = td_current->size;
    } else  {
        transfer_len =   td_current->size;
        transferred = td_current->size;
        MBED_ASSERT(transferred <= (int)max_size);
    }
    buf_start = (uint8_t *)td_current->currBufPtr;

    //Now add this free TD at this end of the queue
    state = USB_TYPE_PROCESSING;
    /*  one request */
    td_current->nextTD = (hcTd*)0;

    *addr = (uint32_t)td_current;
    /*  dir /setup is inverted for ST */
    /* token is useful only ctrl endpoint */
    /*  last parameter is ping ? */
    MBED_ASSERT(HAL_HCD_HC_SubmitRequest((HCD_HandleTypeDef*)hced->hhcd, hced->ch_num, dir-1, type,!setup,(uint8_t*) td_current->currBufPtr, transfer_len, 0)==HAL_OK);
    return USB_TYPE_PROCESSING;
}

void USBEndpoint::unqueueTransfer(volatile HCTD * td)
{

    uint32_t *addr = &((uint32_t *)((HCD_HandleTypeDef*)hced->hhcd)->pData)[hced->ch_num];
    td->state=0;
    td->currBufPtr=0;
    td->size=0;
    td->nextTD=0;
    *addr = 0;
    td_current = td_next;
    td_next = td;
}

void USBEndpoint::queueEndpoint(USBEndpoint * ed)
{
    nextEp = ed;
}
#endif
