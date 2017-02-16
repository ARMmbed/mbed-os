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

#ifndef __HAL_PG_H__
#define __HAL_PG_H__

#include <autoconf.h>

//
// For VHT series TX power by rate table.
// VHT TX power by rate off setArray = 
// Band:-2G&5G = 0 / 1
// RF: at most 4*4 = ABCD=0/1/2/3
// CCK=0 OFDM=1/2 HT-MCS 0-15=3/4/56 VHT=7/8/9/10/11			
//
#define PPG_BB_GAIN_2G_TX_OFFSET_MASK	0x0F
#define PPG_BB_GAIN_2G_TXB_OFFSET_MASK	0xF0

#define PPG_BB_GAIN_5G_TX_OFFSET_MASK	0x1F
#define PPG_THERMAL_OFFSET_MASK			0x1F
#define KFREE_BB_GAIN_2G_TX_OFFSET(_ppg_v) (((_ppg_v) == PPG_BB_GAIN_2G_TX_OFFSET_MASK) ? 0 : (((_ppg_v) & 0x01) ? ((_ppg_v) >> 1) : (-((_ppg_v) >> 1))))
#define KFREE_THERMAL_OFFSET(_ppg_v) (((_ppg_v) == PPG_THERMAL_OFFSET_MASK) ? 0 : (((_ppg_v) & 0x01) ? ((_ppg_v) >> 1) : (-((_ppg_v) >> 1))))



#if defined(NOT_SUPPORT_5G)
#define TX_PWR_BY_RATE_NUM_BAND			1
#else
#define TX_PWR_BY_RATE_NUM_BAND			2
#endif

#if defined(NOT_SUPPORT_RF_MULTIPATH) && defined(NOT_SUPPORT_VHT)
#define TX_PWR_BY_RATE_NUM_RF			1
#define TX_PWR_BY_RATE_NUM_RATE			20 // CCK 1M~11M, OFDM 6M~54M, MCS0~7
#else
#define TX_PWR_BY_RATE_NUM_RF			4
#define TX_PWR_BY_RATE_NUM_RATE			84
#endif

#if defined(NOT_SUPPORT_RF_MULTIPATH)
#define MAX_RF_PATH					1
#define MAX_TX_COUNT				1
#else
#define MAX_RF_PATH					2	// Max 4 for ss larger than 2
#define MAX_TX_COUNT				4	//It must always set to 4, otherwise read efuse table secquence will be wrong.
#endif
#define	MAX_CHNL_GROUP_24G		6 		// ch1~2, ch3~5, ch6~8,ch9~11,ch12~13,CH 14 total three groups
#define	MAX_CHNL_GROUP_5G		14

typedef struct _TxPowerInfo24G{
	u8 IndexCCK_Base[MAX_RF_PATH][MAX_CHNL_GROUP_24G];
	u8 IndexBW40_Base[MAX_RF_PATH][MAX_CHNL_GROUP_24G];
	//If only one tx, only BW20 and OFDM are used.
	s8 OFDM_Diff[MAX_RF_PATH][MAX_TX_COUNT];
	s8 BW20_Diff[MAX_RF_PATH][MAX_TX_COUNT];
#if !defined(NOT_SUPPORT_RF_MULTIPATH)
	s8 CCK_Diff[MAX_RF_PATH][MAX_TX_COUNT];	
	s8 BW40_Diff[MAX_RF_PATH][MAX_TX_COUNT];
#endif
}TxPowerInfo24G, *PTxPowerInfo24G;

#endif


