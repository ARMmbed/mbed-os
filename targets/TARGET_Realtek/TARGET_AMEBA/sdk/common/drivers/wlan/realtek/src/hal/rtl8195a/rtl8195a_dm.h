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
 ******************************************************************************/
#ifndef __RTL8195A_DM_H__
#define __RTL8195A_DM_H__
enum{
	UP_LINK,
	DOWN_LINK,	
};

enum{
	LLT,
    TXRPT,
    RXBUFF,
	TXBUFF,	
};

//============================================================
// Description:
//
// This file is for 8723B dynamic mechanism only
//
//
//============================================================

//0x8000: TXRPT:2K
//max macid : 128
//TXRPT SIZE 16 bytes
//0x8800: Rate Mask:1K
//Ramask size  8bytes


#define DYNAMIC_FUNC_BT BIT(0)


#define REPORT_OFFSET   0x8100
#define RAMASK_OFFSET   0x8900
#define LLT_H_ADDR      0x650
#define TXREPORT_H_ADDR 0x660
#define RXBUFF_H_ADDR   0x670
#define TXBUFF_H_ADDR   0x680



//============================================================
// structure and define
//============================================================

//============================================================
// function prototype
//============================================================

void rtl8195a_init_dm_priv(PADAPTER padapter);
void rtl8195a_deinit_dm_priv(PADAPTER padapter);

void rtl8195a_InitHalDm(PADAPTER padapter);
void rtl8195a_HalDmWatchDog(PADAPTER padapter);
void rtl8195a_HalDmWatchDog_in_LPS(PADAPTER padapter);
void rtl8195a_hal_dm_in_lps(PADAPTER padapter);
u8 ReadTxrpt8(IN PADAPTER padapter, IN u8 Macid, IN u8 Offset);
VOID WriteTxrpt8(IN PADAPTER padapter, IN u8 Macid, IN u8 Offset, IN u8 Val);
BOOLEAN GetMediaStatusCommon(IN	PADAPTER	pAdapter, IN u8   macid);
VOID GetTxrptStatistic(IN PDM_ODM_T pDM_Odm, IN PODM_RA_INFO_T pRaInfo,IN u8  Reset_var);
#endif

