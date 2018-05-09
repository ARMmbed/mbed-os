/*
 * Copyright (c) 2018 Nuvoton Technology Corp.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Description:   NUC472 EMAC driver header file
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
