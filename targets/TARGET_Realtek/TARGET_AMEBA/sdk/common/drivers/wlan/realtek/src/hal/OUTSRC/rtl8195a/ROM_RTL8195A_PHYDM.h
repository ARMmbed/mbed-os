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
#ifndef	__ROM_RTL8195A_PHYDM_H__
#define __ROM_RTL8195A_PHYDM_H__

typedef struct _FALSE_ALARM_STATISTICS{
	u4Byte	Cnt_Parity_Fail;
	u4Byte	Cnt_Rate_Illegal;
	u4Byte	Cnt_Crc8_fail;
	u4Byte	Cnt_Mcs_fail;
	u4Byte	Cnt_Ofdm_fail;
	u4Byte	Cnt_Ofdm_fail_pre;	//For RTL8881A
	u4Byte	Cnt_Cck_fail;
	u4Byte	Cnt_all;
	u4Byte	Cnt_Fast_Fsync;
	u4Byte	Cnt_SB_Search_fail;
	u4Byte	Cnt_OFDM_CCA;
	u4Byte	Cnt_CCK_CCA;
	u4Byte	Cnt_CCA_all;
	u4Byte	Cnt_BW_USC;	//Gary
	u4Byte	Cnt_BW_LSC;	//Gary
}FALSE_ALARM_STATISTICS, *PFALSE_ALARM_STATISTICS;

typedef struct _CFO_TRACKING_
{
	BOOLEAN			bATCStatus;
	BOOLEAN			largeCFOHit;
	BOOLEAN			bAdjust;
	u1Byte			CrystalCap;
	u1Byte			DefXCap;
	int				CFO_tail[2];
	int				CFO_ave_pre;
	u4Byte			packetCount;
	u4Byte			packetCount_pre;
	BOOLEAN			bForceXtalCap;
	BOOLEAN			bReset;
	u1Byte			CFO_TH_XTAL_HIGH;
	u1Byte			CFO_TH_XTAL_LOW;
	u1Byte			CFO_TH_ATC;
}CFO_TRACKING, *PCFO_TRACKING;

typedef struct _ROM_INFO{
	u1Byte	EEPROMVersion;
	u1Byte	CrystalCap;
	u8Byte	DebugComponents;
	u4Byte	DebugLevel;
}ROM_INFO, *PROM_INFO;

extern FALSE_ALARM_STATISTICS 	FalseAlmCnt;
extern CFO_TRACKING    			DM_CfoTrack;
extern ROM_INFO					ROMInfo;

u1Byte
ROM_odm_QueryRxPwrPercentage(
	IN		s1Byte		AntPower
);


u1Byte 
ROM_odm_EVMdbToPercentage(
    IN		s1Byte Value
);

s4Byte
ROM_odm_SignalScaleMapping_8195A(	
	IN u1Byte SupportInterface,
	IN s4Byte CurrSig 
);

VOID
ROM_odm_FalseAlarmCounterStatistics(
	IN		PVOID					pDM_VOID
);

VOID 
ROM_odm_SetEDCCAThreshold(
	IN		PVOID		pDM_VOID,
	IN		s1Byte		H2L,
	IN		s1Byte		L2H
);

VOID
ROM_odm_SetTRxMux(
	IN		PVOID			pDM_VOID,
	IN		ODM_Trx_MUX_Type			txMode,
	IN		ODM_Trx_MUX_Type			rxMode
);	


VOID
ROM_odm_SetCrystalCap(
	IN		PVOID					pDM_VOID,
	IN		u1Byte					CrystalCap
);

u1Byte
ROM_odm_GetDefaultCrytaltalCap(
	IN		PVOID					pDM_VOID
);

VOID
ROM_ODM_CfoTrackingReset(
	IN		PVOID					pDM_VOID
);

VOID
ROM_odm_CfoTrackingFlow(
	IN		PVOID					pDM_VOID
);

#endif
