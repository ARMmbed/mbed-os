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
 
#ifndef	__PHYDMRAINFO_H__
#define    __PHYDMRAINFO_H__

#define RAINFO_VERSION	"1.0"

#define	H2C_0X42_LENGTH	5
#define	H2C_MAX_LENGTH	7

#define	RA_FLOOR_UP_GAP		3
#define	RA_FLOOR_TABLE_SIZE	7

#define	ACTIVE_TP_THRESHOLD	150
#define	RA_RETRY_DESCEND_NUM	2
#define	RA_RETRY_LIMIT_LOW	4
#define	RA_RETRY_LIMIT_HIGH	32

#define RAINFO_BE_RX_STATE			BIT0  // 1:RX    //ULDL
#define RAINFO_STBC_STATE			BIT1
//#define RAINFO_LDPC_STATE 			BIT2
#define RAINFO_NOISY_STATE 			BIT2    // set by Noisy_Detection
#define RAINFO_SHURTCUT_STATE 		BIT3
#define RAINFO_SHURTCUT_FLAG 		BIT4
#define RAINFO_INIT_RSSI_RATE_STATE  BIT5
#define RAINFO_BF_STATE 				BIT6
#define RAINFO_BE_TX_STATE 			BIT7 // 1:TX

#define AP_InitRateAdaptiveState	ODM_RateAdaptiveStateApInit

#define		DM_RATR_STA_INIT			0
#define		DM_RATR_STA_HIGH			1
#define 		DM_RATR_STA_MIDDLE		2
#define 		DM_RATR_STA_LOW			3
#if(DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
#define		DM_RATR_STA_ULTRA_LOW	4
#endif

#if(DM_ODM_SUPPORT_TYPE & (ODM_WIN|ODM_CE|ODM_IOT))
typedef struct _Rate_Adaptive_Table_{
	u1Byte		firstconnect;
	#if(DM_ODM_SUPPORT_TYPE==ODM_WIN)
	BOOLEAN		PT_collision_pre;
	#endif

    	//u1Byte	link_tx_rate[ODM_ASSOCIATE_ENTRY_NUM];
	u1Byte	highest_client_tx_order;
	u2Byte	highest_client_tx_rate_order;
	u1Byte	power_tracking_flag;
	u1Byte	RA_threshold_offset;
	u1Byte	RA_offset_direction;
    
}RA_T, *pRA_T;
#endif

typedef struct _ODM_RATE_ADAPTIVE
{
	u1Byte				Type;				// DM_Type_ByFW/DM_Type_ByDriver
	u1Byte				HighRSSIThresh;		// if RSSI > HighRSSIThresh	=> RATRState is DM_RATR_STA_HIGH
	u1Byte				LowRSSIThresh;		// if RSSI <= LowRSSIThresh	=> RATRState is DM_RATR_STA_LOW
	u1Byte				RATRState;			// Current RSSI level, DM_RATR_STA_HIGH/DM_RATR_STA_MIDDLE/DM_RATR_STA_LOW

	#if(DM_ODM_SUPPORT_TYPE & (ODM_WIN|ODM_CE|ODM_IOT))
	u1Byte				LdpcThres;			// if RSSI > LdpcThres => switch from LPDC to BCC
	BOOLEAN				bLowerRtsRate;
	#endif

	#if(DM_ODM_SUPPORT_TYPE & ODM_WIN)
	u1Byte				RtsThres;
	#elif(DM_ODM_SUPPORT_TYPE & (ODM_CE|ODM_IOT))
	BOOLEAN				bUseLdpc;
	#else
	u1Byte				UltraLowRSSIThresh;
	u4Byte				LastRATR;			// RATR Register Content
	#endif

} ODM_RATE_ADAPTIVE, *PODM_RATE_ADAPTIVE;

VOID
odm_RSSIMonitorInit(
	IN		PVOID		pDM_VOID
	);

VOID
odm_RSSIMonitorCheck(
	IN	 	PVOID	 	 pDM_VOID
	);

#if(DM_ODM_SUPPORT_TYPE==ODM_WIN)
VOID
odm_RSSIDumpToRegister(
	IN	PVOID	pDM_VOID
	);
#endif

VOID
odm_RSSIMonitorCheckMP(
	IN		PVOID	 	pDM_VOID
	);

VOID 
odm_RSSIMonitorCheckCE(
	IN		PVOID		pDM_VOID
	);

VOID 
odm_RSSIMonitorCheckIOT(
	IN		PVOID		pDM_VOID
	);

VOID 
odm_RSSIMonitorCheckAP(
	IN		PVOID		 pDM_VOID
	);


VOID
odm_RateAdaptiveMaskInit(
	IN	PVOID	pDM_VOID	
	);

VOID
odm_RefreshRateAdaptiveMask(
	IN	PVOID	pDM_VOID
	);

VOID
odm_RefreshRateAdaptiveMaskMP(
	IN	PVOID	pDM_VOID
	);

VOID
odm_RefreshRateAdaptiveMaskCE(
	IN	PVOID	pDM_VOID	
	);

VOID
odm_RefreshRateAdaptiveMaskIOT(
	IN	PVOID	pDM_VOID	
	);

VOID
odm_RefreshRateAdaptiveMaskAPADSL(
	IN	PVOID	pDM_VOID
	);

BOOLEAN 
ODM_RAStateCheck(
	IN		PVOID			pDM_VOID,
	IN		s4Byte			RSSI,
	IN		BOOLEAN			bForceUpdate,
	OUT		pu1Byte			pRATRState
	);
	
VOID
odm_RefreshBasicRateMask(
	IN	PVOID	pDM_VOID
	);

VOID
phydm_ra_info_init(
	IN	PVOID	pDM_VOID
	);

#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
VOID
ODM_DynamicARFBSelect(
	IN		PVOID			pDM_VOID,
	IN 		u1Byte			rate,
	IN  	BOOLEAN			Collision_State	
	);
	
VOID
ODM_RateAdaptiveStateApInit(	
	IN	PVOID		PADAPTER_VOID,
	IN	PRT_WLAN_STA  	pEntry
	);
#endif

#if (DM_ODM_SUPPORT_TYPE & (ODM_CE|ODM_IOT))
u4Byte 
ODM_Get_Rate_Bitmap(
	IN	PVOID		pDM_VOID,	
	IN	u4Byte		macid,
	IN	u4Byte 		ra_mask,	
	IN	u1Byte 		rssi_level
	);
#endif

#endif //#ifndef	__ODMRAINFO_H__


