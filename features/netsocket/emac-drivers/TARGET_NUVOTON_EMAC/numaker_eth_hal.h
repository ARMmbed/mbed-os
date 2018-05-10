/*
 * Copyright (c) 2018 Nuvoton Technology Corp. 
 * Copyright (c) 2018 ARM Limited
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
 * Description:   NuMaker EMAC HAL header file
 */

#ifndef  NUMAKER_ETH_HAL_
#define  NUMAKER_ETH_HAL_
 
#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef NU_TRACE
#define NU_DEBUGF(x) { printf x; }
#else
#define NU_DEBUGF(x)
#endif

typedef void (* eth_callback_t) (char, void*);
void mbed_mac_address(char *mac);
void numaker_eth_init(uint8_t *mac_addr);
void numaker_eth_trigger_rx(void);
int numaker_eth_get_rx_buf(uint16_t *len, uint8_t **buf);
uint8_t *numaker_eth_get_tx_buf(void);
void numaker_eth_trigger_tx(uint16_t length, void *p);
int numaker_eth_link_ok(void);
void numaker_eth_set_cb(eth_callback_t eth_cb, void *userData);
void numaker_set_mac_addr(uint8_t *addr);
void numaker_eth_enable_interrupts(void);
void numaker_eth_disable_interrupts(void);       

#ifdef __cplusplus
}
#endif

#endif /* NUMAKER_ETH_HAL_ */
