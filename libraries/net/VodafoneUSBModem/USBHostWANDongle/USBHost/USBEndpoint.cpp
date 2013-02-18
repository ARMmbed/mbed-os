/* Copyright (c) 2010-2012 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#define __DEBUG__ 0 //Maximum verbosity
#ifndef __MODULE__
#define __MODULE__ "USBEndpoint.cpp"
#endif

#include "core/dbg.h"
#include <cstdint>

#include "USBEndpoint.h"


void USBEndpoint::init(HCED * hced, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir, uint32_t size, uint8_t ep_number, HCTD* td_list[2]) {
    this->hced = hced;
    this->type = type;
    this->dir = /*(type == CONTROL_ENDPOINT) ? OUT :*/ dir;
    setup = (type == CONTROL_ENDPOINT) ? true : false;

    //TDs have been allocated by the host
    memcpy((HCTD**)this->td_list, td_list, sizeof(HCTD*)*2); //TODO: Maybe should add a param for td_list size... at least a define
    memcpy(td_list[0], 0, sizeof(HCTD));
    memcpy(td_list[1], 0, sizeof(HCTD));

    this->hced->control = 0;
    //Empty queue
    this->hced->tailTD = (uint32_t)td_list[0];
    this->hced->headTD = (uint32_t)td_list[0];
    this->hced->nextED = 0;

    this->hced->control = ((ep_number & 0x7F) << 7)                         // Endpoint address
                          | (type != CONTROL_ENDPOINT ? ( dir << 11) : 0 )  // direction : Out = 1, 2 = In
                          | ((size & 0x3ff) << 16);                         // MaxPkt Size
                          
    //carry = false;
    transfer_len = 0;
    transferred = 0;
    buf_start = 0;
    nextEp = NULL;

    td_current = td_list[0];
    td_next = td_list[1];

    state = USB_TYPE_IDLE;
}

void USBEndpoint::setSize(uint32_t size) {
    hced->control &= ~(0x3ff << 16);
    hced->control |= (size << 16);
}


uint32_t USBEndpoint::getSize() {
    return (hced->control >> 16) & 0x3ff;
}

void USBEndpoint::setDeviceAddress(uint8_t addr) {
    hced->control &= ~(0x7f);
    hced->control |= (addr & 0x7F);
}

uint8_t USBEndpoint::getDeviceAddress() {
    return hced->control & 0x7f;
}

void USBEndpoint::setSpeed(uint8_t speed) {
    if(speed) {
        DBG("SET LOW SPEED");
    }
    hced->control &= ~(1 << 13);
    hced->control |= (speed << 13);
}


void USBEndpoint::setNextToken(uint32_t token) { //Only for control Eps
    switch (token) {
        case TD_SETUP:
            dir = OUT;
            setup = true;
            break;
        case TD_IN:
            dir = IN;
            setup = false;
            break;
        case TD_OUT:
            dir = OUT;
            setup = false;
            break;
    }
}

volatile HCTD* USBEndpoint::getNextTD()
{
  return td_current/*(HCTD*) hced->tailTD*/; //It's the tailing one
}

void USBEndpoint::queueTransfer() {
  //Try with OHCI impl
  //Caller of getNextTD() has now populated the td
  //So insert it into queue

  //Find an OTHER free td
  //TODO: if we had more than 2 tds, this would have to be changed
  /*HCTD* toSendTD = (HCTD*) hced->tailTD;*/
  //HCTD* freeTD;
/*
  if( hced->tailTD == td_list[0] )
  {
    freeTD = td_list[1];
  }
  else *//*if( hced->tailTD == (uint32_t) td_list[1] )*/
  /*{
    freeTD = td_list[0];
  }
  */

  /*
  freeTD->control = 0;
  freeTD->currBufPtr = 0;
  freeTD->bufEnd = 0;
  freeTD->nextTD = 0;

  td_current = toSendTD;
*/
  transfer_len = td_current->bufEnd - td_current->currBufPtr + 1;
  transferred = transfer_len;
  buf_start = td_current->currBufPtr;

  //No add this free TD at this end of the queue
  state = USB_TYPE_PROCESSING;
  td_current->nextTD = (volatile uint32_t)td_next;
  hced->tailTD = (volatile uint32_t)td_next;

  #if 0
    // if TD list empty -> we put the head of the list
    if (!hced->headTD) {
        state = USB_TYPE_IDLE;
        hced->headTD = (uint32_t)(td);
        hced->tailTD = (uint32_t)(td);
        tailTD = (HCTD *) (hced->headTD);
        //DBG("queue null transfer: endpoint: %p,  %08X\r\n", this, (uint32_t)(td));
    } else {
        state = USB_TYPE_PROCESSING;
        td->nextTD = (uint32_t)headTD & ~(0x0f);
        hced->headTD = (uint32_t)(td) | ((carry) ? 0x2 : 0);
    }
    headTD = (HCTD *) ((hced->headTD) & ~(0x3));
    transfer_len = td->bufEnd - td->currBufPtr + 1;
    transferred = transfer_len;
    buf_start = td->currBufPtr;
#endif
    //printf("queue real transfer: endpoint: %p \t headTD: %p \t head: %08X \t tail: %08X \t td: %08X \t nexttd: %08X\r\n", this, hced->headTD, hced->headTD, ((HCTD *)((hced->headTD) & ~(0x0f)))->nextTD, toSendTD, toSendTD->nextTD);
}

volatile HCTD * USBEndpoint::getProcessedTD()
{
  return td_current;
}

void USBEndpoint::setLengthTransferred(int len) {
    transferred = len;
}

uint32_t USBEndpoint::getBufStart() {
    return buf_start;
}

void USBEndpoint::unqueueTransfer(volatile HCTD * td) {
    //printf("unqueue transfer: %p on endpoint: %p\r\n", (void *)td, this);
    //headTD = tailTD; //FIXME FIXME
//  hced->headTD = hced->headTD | (td->  & 0x02);
  if(td != td_current)
  {
    ERR("MISMATCH");
    ERR("this=%p, td_current = %p, td_next=%p, td=%p", this, td_current, td_next, td);
    error("");
  }
  td->control=0;
  td->currBufPtr=0;
  td->bufEnd=0;
  td->nextTD=0;
  hced->headTD = hced->tailTD | (hced->headTD & 0x2); //Carry bit
  td_current = td_next;
  td_next = td;
  DBG("current:%p, next:%p", td_current, td_next);
}

ENDPOINT_TYPE USBEndpoint::getType() {
    return type;
}


USBEndpoint * USBEndpoint::nextEndpoint() {
    return (USBEndpoint*)nextEp;
}


void USBEndpoint::queueEndpoint(USBEndpoint * ed) {
    nextEp = ed;
    hced->nextED = (ed == NULL) ? 0 : (uint32_t)ed->getHCED();
}

volatile HCED * USBEndpoint::getHCED() {
    return hced;
}


volatile HCTD * USBEndpoint::getHeadTD() {
    //return headTD;
  return (volatile HCTD*) (hced->headTD & ~0xF);
}

volatile HCTD ** USBEndpoint::getTDList()
{
  return td_list;
}
