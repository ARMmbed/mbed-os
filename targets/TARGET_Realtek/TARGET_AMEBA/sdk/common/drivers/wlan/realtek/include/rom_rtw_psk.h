/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This is ROM code section. 
 *
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

