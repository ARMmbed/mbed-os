/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************
 *                                        
 * This is ROM code section. 
 *
 ******************************************************************************/
#ifndef __ROM_RTW_PSK_H_
#define __ROM_RTW_PSK_H_

int rom_psk_PasswordHash (
				unsigned char *password,
				int passwordlength,
				unsigned char *ssid,
				int ssidlength,
				unsigned char *output);

void rom_psk_CalcPTK(	unsigned char *addr1, unsigned char *addr2,
			unsigned char *nonce1, unsigned char *nonce2,
			unsigned char *keyin, int keyinlen,
			unsigned char *keyout, int keyoutlen);

void rom_psk_CalcGTK(unsigned char *addr, unsigned char *nonce,
				unsigned char *keyin, int keyinlen,
				unsigned char *keyout, int keyoutlen);

#endif	//__ROM_RTW_PSK_H_

