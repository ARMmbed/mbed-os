/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
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
#ifndef _RTW_HT_H_
#define _RTW_HT_H_

#include "wifi.h"

struct ht_priv
{
	u32	ht_option;	
	u32	ampdu_enable;//for enable Tx A-MPDU
	//u8	baddbareq_issued[16];
	//u32	tx_amsdu_enable;//for enable Tx A-MSDU
	//u32	tx_amdsu_maxlen; // 1: 8k, 0:4k ; default:8k, for tx
	//u32	rx_ampdu_maxlen; //for rx reordering ctrl win_sz, updated when join_callback.
	
	u8	bwmode;//
	u8	ch_offset;//PRIME_CHNL_OFFSET
	u8	sgi;//short GI

	//for processing Tx A-MPDU
	u8	agg_enable_bitmap;
	//u8	ADDBA_retry_count;
	u8	candidate_tid_bitmap;

	u8	stbc_cap;
	
	struct rtw_ieee80211_ht_cap ht_cap;
	
};

#define	STBC_HT_ENABLE_RX			BIT0
#define	STBC_HT_ENABLE_TX			BIT1
#define	STBC_HT_TEST_TX_ENABLE		BIT2
#define	STBC_HT_CAP_TX				BIT3

typedef enum AGGRE_SIZE{
	HT_AGG_SIZE_8K = 0,
	HT_AGG_SIZE_16K = 1,
	HT_AGG_SIZE_32K = 2,
	HT_AGG_SIZE_64K = 3,
	VHT_AGG_SIZE_128K = 4,
	VHT_AGG_SIZE_256K = 5,
	VHT_AGG_SIZE_512K = 6,
	VHT_AGG_SIZE_1024K = 7,
}AGGRE_SIZE_E, *PAGGRE_SIZE_E;

#endif	//_RTL871X_HT_H_

