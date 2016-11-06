/* MPS2 Peripheral Library
*
* Copyright (c) 2006-2015 ARM Limited
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, 
* this list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution.
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software without 
* specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE. 
*/

#ifndef _ETH_MPS2_H_
#define _ETH_MPS2_H_

#include "SMM_MPS2.h"

// Function declarations

unsigned int smsc9220_mac_regread(unsigned char regoffset, unsigned int *data);
unsigned int smsc9220_mac_regwrite(unsigned char regoffset, unsigned int data);
unsigned int smsc9220_phy_regread(unsigned char regoffset, unsigned short *data);
unsigned int smsc9220_phy_regwrite(unsigned char regoffset, unsigned short data);

unsigned int smsc9220_read_id(void);
unsigned int smsc9220_soft_reset(void);
void smsc9220_set_txfifo(unsigned int val);
unsigned int smsc9220_wait_eeprom(void);
void smsc9220_init_irqs(void);
unsigned int smsc9220_check_phy(void);
unsigned int smsc9220_reset_phy(void);

void smsc9220_advertise_cap(void);
void smsc9220_establish_link(void);
void smsc9220_enable_xmit(void);
void smsc9220_enable_mac_xmit(void);
void smsc9220_enable_mac_recv(void);
unsigned int smsc9220_check_ready(void);
void smsc9220_set_soft_int(void);
void smsc9220_clear_soft_int(void);

unsigned int smsc9220_recv_packet(unsigned int *recvbuf, unsigned int *index);
void smsc9220_xmit_packet(unsigned char * pkt, unsigned int length);

#endif
