/*
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
 */

#ifndef __HALCOM_RATE_ADAPTIVE_RAM_H__
#define __HALCOM_RATE_ADAPTIVE_RAM_H__

#if RATE_ADAPTIVE_SUPPORT

/*--------------------------Define -------------------------------------------*/

#define FIRST_MACID 0 // This is the connection of STA to AP
// Rate index mapping
#define RATE_CCK_1M         ODM_RATE1M		
#define RATE_CCK_2M         ODM_RATE2M		
#define RATE_CCK_5M         ODM_RATE5_5M	
#define RATE_CCK_11M        ODM_RATE11M		
#define RATE_OFDM_6M        ODM_RATE6M		
#define RATE_OFDM_9M        ODM_RATE9M		
#define RATE_OFDM_12M       ODM_RATE12M		
#define RATE_OFDM_18M       ODM_RATE18M		
#define RATE_OFDM_24M       ODM_RATE24M		
#define RATE_OFDM_36M       ODM_RATE36M		
#define RATE_OFDM_48M       ODM_RATE48M		
#define RATE_OFDM_54M       ODM_RATE54M		
#define RATE_HT_MCS0        ODM_RATEMCS0	
#define RATE_HT_MCS1        ODM_RATEMCS1	
#define RATE_HT_MCS2        ODM_RATEMCS2	
#define RATE_HT_MCS3        ODM_RATEMCS3	
#define RATE_HT_MCS4        ODM_RATEMCS4	
#define RATE_HT_MCS5        ODM_RATEMCS5	
#define RATE_HT_MCS6        ODM_RATEMCS6	
#define RATE_HT_MCS7        ODM_RATEMCS7	
#define RATE_HT_MCS8        ODM_RATEMCS8	
#define RATE_HT_MCS9        ODM_RATEMCS9	
#define RATE_HT_MCS10       ODM_RATEMCS10 
#define RATE_HT_MCS11       ODM_RATEMCS11 
#define RATE_HT_MCS12       ODM_RATEMCS12 
#define RATE_HT_MCS13       ODM_RATEMCS13 
#define RATE_HT_MCS14       ODM_RATEMCS14 
#define RATE_HT_MCS15       ODM_RATEMCS15 

#define MACID_NUM                   128



// TX report format
#define TXRPT_SIZE          16
//offset 0
#define TXRPT_DATARATE      (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6)
#define TXRPT_SGI           BIT7
//offset 1
#define TXRPT_PWRSTS        (BIT0|BIT1|BIT2)
#define TXRPT_TRYNESSCNT    (BIT3|BIT4|BIT5|BIT6)
#define TXRPT_TRYRATE       BIT7
//offset 2
#define TXRPT_TRYRESULT     BIT6
#define TXRPT_TRYFINISH     BIT7
//offset 3
#define TXRPT_PAUSERPT      BIT6
#define TXRPT_RESETRPT      BIT7
//offset 4

//offset 5
#define TXRPT_BW            (BIT0|BIT1)
#define TXRPT_PKTDROP       BIT2



#define RATE_UP 1
#define RATE_DOWN 2

#define  RSSI_TH1 45
#define  RSSI_TH2 25

#define PERENTRY        27
#define RETRYSIZE       5
#define RATESIZE        20
#define RAMASK_SIZE         8

#define SS_PT_TH_High   66
#define SS_PT_TH_low    57
#define SS_PT_off       48
#define SS_RA_INIT_RATE_RSSI       30
#define STEP_DROP       1
#define CONFIG_SGI      0
#define TRY_WAITING   10

// RA mask
#define Mask_length_REG         8
#define Rate_id_NUM             9 // 6 rate id from reg
#define ARFB_table_NUM          7

#define TRY_NESS_CNT_IDX_SIZE   16

/*------------------------------Define Enum-----------------------------------*/
typedef enum _RTL8195_RATEID_IDX_ {
    MODE_BGN_40M_2SS    = 0,
    MODE_BGN_40M_1SS    = 1,
    MODE_BGN_20M_2SS_BN = 2,
    MODE_BGN_20M_1SS_BN = 3,
    MODE_GN_N2SS        = 4,
    MODE_GN_N1SS        = 5,
    MODE_BG             = 6,
    MODE_G              = 7,
    MODE_B              = 8
} RTL8195_RATEID_IDX, *PRTL8195_RATEID_IDX;

typedef enum _VHT_HT_SWITCH_ {
    TYPE_HT             = 0,
    TYPE_VHT            = 1,
    TYPE_MIX1           = 2,
    TYPE_MIX2           = 3
} VHT_SEL_SWITCH, *PVHT_SEL_SWITCH;

/*--------------------------Define MACRO--------------------------------------*/
#define TRYING_DISABLE          0
#define TRYING_ENABLE           1

//RA MASK: INIT_RATE_MASK

//if VHT_HT_SWITCH = 1, it means VHT. 
//Bit[51:12] : VHT 1SS ~ VHT 4SS
//if VHT_HT_SWITCH = 0, it means HT. 
//Bit[43:12] : HT 1SS ~ HT4SS

//offset6
//#define VHT_HT_SWITCH       BIT4


// H2C CMD
//offset0
#define H2CID13_MACID           0x7F
//offset1
#define H2CID13_RATEID          0x1F
#define H2CID13_SGI             BIT7



//offset2
#define H2CID13_BW              (BIT0|BIT1)
#define H2CID13_enldpc          BIT2
#define H2CID13_NOUPDATE        BIT3
#define H2CID13_VHT_EN          (BIT5|BIT4)
#define H2CID13_DISPT           BIT6
#define H2CID13_DISRA           BIT7

//H2C AP_Req_Tx_Rpt
#define H2CID43_RTY_OK_TOTAL BIT0
#define H2CID43_RTY_CNT_MACID   BIT1


//RAInfo
#define MASK_RA_ULDL_STATE      BIT0
#define MASK_RA_STBC_STATE      BIT1
#define MASK_RA_LDPC_CAP_STATE  BIT2
#define MASK_RA_SHORTCUT_STATE  BIT3
#define MASK_RA_SHORTCUT_FLAG   BIT4
#define MASK_RA_INIT_RATE_RSSI_STATE BIT5
#define MASK_RA_BF_STATE		BIT6
#define MASK_RA_DELAY_RATE		BIT7



#define RA_ULDL_STATE_SHT       0
#define RA_STBC_STATE_SHT	    1
#define RA_LDPC_CAP_STATE_SHT	2
#define RA_SHORTCUT_STATE_SHT   3
#define RA_SHORTCUT_FLAG_SHT    4
#define RA_INIT_RATE_RSSI_SHT	  5
#define RA_BF_STATE_SHT			6
#define RA_DELAY_RATE_SHT		7

/*------------------------Export global variable------------------------------*/
//2 Rate Adaptive
//HW Statistic
//extern MEMTYPE_XDATA u16        TOTAL[MACID_NUM];
//extern MEMTYPE_XDATA u1Byte         DROP[MACID_NUM];
//extern MEMTYPE_XDATA u16        RTY[MACID_NUM][5];

//extern MEMTYPE_XDATA STAINFO_RA stainfo_ra[MACID_NUM];
//extern MEMTYPE_XDATA u16        Nsc[MACID_NUM];
//extern MEMTYPE_XDATA u1Byte         RSSI[MACID_NUM];  // add by Gary
//extern MEMTYPE_XDATA u1Byte         BUPDATE[MACID_NUM]; 

/*------------------------------Function declaration--------------------------*/
VOID
InitBBNHM(
    void
);

VOID
BBNHM(
    void
);

VOID 
ODM_InitRAInfo(
    IN   PDM_ODM_T		pDM_Odm 
);

VOID 
H2CHDL_Set_MACID_Config(
	IN   	PDM_ODM_T		pDM_Odm,
	IN	u1Byte 			*pbuf
);

VOID
H2CHDL_SetRssiSingle(
    u1Byte  *pbuf
);


VOID
H2CHDL_APReqTxrpt(
    u1Byte  *pbuf
);

VOID
H2CHDL_InitRateCollect(
    u1Byte *pbuf
);


VOID
TryDone(
    	IN   	PDM_ODM_T		pDM_Odm,
	IN	PODM_RA_INFO_T 	pRaInfo
);

VOID 
RateDownTrying(
	IN   	PDM_ODM_T	pDM_Odm,
	IN 	PODM_RA_INFO_T pRaInfo
);

VOID
RateDecisionRAM8195A(
    IN   	PDM_ODM_T	pDM_Odm,
    IN	PODM_RA_INFO_T pRaInfo
);

VOID 
GetRATRfromREG(
    IN u16 reg_addr,
    IN u1Byte macid
);

VOID 
PHY_DM_RA_SetRSSI_8195A(
	IN 	PDM_ODM_T 		pDM_Odm, 
	IN 	u1Byte 			MacID, 
	IN 	u1Byte 			Rssi
	);

#if 0
extern void 
Rate_trying_decision(
    IN u1Byte   macid, 
    IN u1Byte   rate, 
    IN u1Byte   datarc, 
    IN u1Byte   aggnum
);
#endif


//debug
VOID
ArfrRefresh(
	IN   	PDM_ODM_T		pDM_Odm,
	IN	PODM_RA_INFO_T 	pRaInfo
);

#endif  //#if CONFIG_RATE_ADAPTIVE

#endif  //#ifndef   __HALCOM_RATE_ADAPTIVE_RAM_H__

