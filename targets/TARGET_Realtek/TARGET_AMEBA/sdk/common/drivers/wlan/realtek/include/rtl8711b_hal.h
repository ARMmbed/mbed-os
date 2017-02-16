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

#ifndef __RTL8711B_HAL_H__
#define __RTL8711B_HAL_H__

#include "drv_types.h"
//#include "rtl8711b/rtl8711b_pmu_task.h"
#include "hal_data.h"

#include "rtl8711b/rtl8711b_spec.h"
#include "rtl8711b/rtl8711b_rf.h"
#include "rtl8711b/rtl8711b_dm.h"
#include "rtl8711b/rtl8711b_recv.h"
#include "rtl8711b/rtl8711b_xmit.h"
#include "rtl8711b/rtl8711b_cmd.h"
//#include "rtl8711b/rtl8711b_pmu_cmd.h"
#include "rtl8711b/rtl8711b_led.h"
#include "rtl8711b/Hal8711BPwrSeq.h"
#include "rtl8711b/Hal8711BPhyReg.h"
#include "rtl8711b/Hal8711BPhyCfg.h"
#include "rtl8711b/rom_Hal8711BPhyCfg.h"

#ifdef DBG_CONFIG_ERROR_DETECT
#include "rtl8711b/rtl8711b_sreset.h"
#endif

#include "../src/hal/OUTSRC/phydm_precomp.h"

#if (RTL8711B_SUPPORT==1)
	//2TODO: We should define 8192S firmware related macro settings here!!
	#define RTL819X_DEFAULT_RF_TYPE			RF_1T2R
	#define RTL819X_TOTAL_RF_PATH				2

//---------------------------------------------------------------------
//		RTL8723BS From file
//---------------------------------------------------------------------
	#define RTL8723B_FW_IMG					"rtl8723B\\rtl8723bfw.bin"
	#define RTL8711B_PHY_REG					"rtl8711B\\PHY_REG_1T.txt"
	#define RTL8711B_PHY_RADIO_A			"rtl8711B\\radio_a_1T.txt"
	#define RTL8711B_PHY_RADIO_B			"rtl8711B\\radio_b_1T.txt"
	#define RTL8711B_TXPWR_TRACK			"rtl8711B\\TxPowerTrack.txt" 
	#define RTL8711B_AGC_TAB					"rtl8711B\\AGC_TAB_1T.txt"
	#define RTL8711B_PHY_MACREG				"rtl87195A\\MAC_REG.txt"
	#define RTL8711B_PHY_REG_PG				"rtl8711B\\PHY_REG_PG.txt"
	#define RTL8711B_PHY_REG_MP				"rtl8711B\\PHY_REG_MP.txt"
	#define RTL8711B_TXPWR_LMT 				"rtl8711B\\TXPWR_LMT.txt"

//---------------------------------------------------------------------
//		RTL8723BS From header
//---------------------------------------------------------------------

	//#define Rtl8723B_FwImageArray				Array_MP_8723B_FW_NIC
	//#define Rtl8723B_FwImgArrayLength			ArrayLength_MP_8723B_FW_NIC
	//#define Rtl8723B_FwWoWImageArray			Array_MP_8723B_FW_WoWLAN
	//#define Rtl8723B_FwWoWImgArrayLength		ArrayLength_MP_8723B_FW_WoWLAN

	#define Rtl8711B_PHY_REG_Array_PG 			Rtl8723SPHY_REG_Array_PG
	#define Rtl8711B_PHY_REG_Array_PGLength	Rtl8723SPHY_REG_Array_PGLength

#if MP_DRIVER == 1
	#define Rtl8711B_FwBTImgArray				Rtl8723BFwBTImgArray
	#define Rtl8711B_FwBTImgArrayLength		Rtl8723BFwBTImgArrayLength

	#define Rtl8711B_FwMPImageArray			Rtl8723BFwMPImgArray
	#define Rtl8711B_FwMPImgArrayLength		Rtl8723BMPImgArrayLength

	#define Rtl8711B_PHY_REG_Array_MP			Rtl8723B_PHYREG_Array_MP
	#define Rtl8711B_PHY_REG_Array_MPLength	Rtl8723B_PHYREG_Array_MPLength
#endif

#endif // RTL8711B_SUPPORT

#define FW_8711B_SIZE			0x8000
#define FW_8711B_START_ADDRESS	0x1000
#define FW_8711B_END_ADDRESS		0x1FFF //0x5FFF

#define IS_FW_HEADER_EXIST_8711B(_pFwHdr)	((GET_FIRMWARE_HDR_SIGNATURE(_pFwHdr)&0xFFF0) == 0x10B0)

typedef struct _RT_FIRMWARE {
	FIRMWARE_SOURCE	eFWSource;
#ifdef CONFIG_EMBEDDED_FWIMG
	u8*			szFwBuffer;
#else
	u8			szFwBuffer[FW_8711B_SIZE];
#endif
	u32			ulFwLength;

#ifdef CONFIG_EMBEDDED_FWIMG
	u8*			szBTFwBuffer;
#else
	u8			szBTFwBuffer[FW_8711B_SIZE];
#endif
	u32			ulBTFwLength;

#ifdef CONFIG_WOWLAN
	u8*			szWoWLANFwBuffer;
	u32			ulWoWLANFwLength;
#endif //CONFIG_WOWLAN
} RT_FIRMWARE_8711B, *PRT_FIRMWARE_8711B;

//
// This structure must be cared byte-ordering
//
// Added by tynli. 2009.12.04.
typedef struct _RT_8723B_FIRMWARE_HDR
{
	// 8-byte alinment required

	//--- LONG WORD 0 ----
	u16		Signature;	// 92C0: test chip; 92C, 88C0: test chip; 88C1: MP A-cut; 92C1: MP A-cut
	u8		Category;	// AP/NIC and USB/PCI
	u8		Function;	// Reserved for different FW function indcation, for further use when driver needs to download different FW in different conditions
	u16		Version;		// FW Version
	u8		Subversion;	// FW Subversion, default 0x00
	u16		Rsvd1;


	//--- LONG WORD 1 ----
	u8		Month;	// Release time Month field
	u8		Date;	// Release time Date field
	u8		Hour;	// Release time Hour field
	u8		Minute;	// Release time Minute field
	u16		RamCodeSize;	// The size of RAM code
	u16		Rsvd2;

	//--- LONG WORD 2 ----
	u32		SvnIdx;	// The SVN entry index
	u32		Rsvd3;

	//--- LONG WORD 3 ----
	u32		Rsvd4;
	u32		Rsvd5;
}RT_8723B_FIRMWARE_HDR, *PRT_8723B_FIRMWARE_HDR;

#define DRIVER_EARLY_INT_TIME_8711B         0x05 // 5ms
#define BCN_DMA_ATIME_INT_TIME_8711B        0x02 // 2ms

// for 8711B
// TX 32K, RX 16K, Page size 128B for TX, 8B for RX
#define PAGE_SIZE_TX_8711B                  128
#define PAGE_SIZE_RX_8711B                  8

#define RX_DMA_SIZE_8711B                   0x4000	// 16K
#define RX_DMA_RESERVED_SIZE_8711B          0x80	// 128B, reserved for tx report
#define RX_DMA_BOUNDARY_8711B               (RX_DMA_SIZE_8711B - RX_DMA_RESERVED_SIZE_8711B - 1)

// Note: We will divide number of page equally for each queue other than public queue!

//For General Reserved Page Number(Beacon Queue is reserved page)
//Beacon:2, PS-Poll:1, Null Data:1,Qos Null Data:1,BT Qos Null Data:1
#ifdef CONFIG_WLAN_HAL_TEST
#define BCNQ_PAGE_NUM_8711B             0x00
#else
#define BCNQ_PAGE_NUM_8711B             0x08
#endif

#ifdef CONFIG_CONCURRENT_MODE
#define BCNQ1_PAGE_NUM_8711B            0x04
#else
#define BCNQ1_PAGE_NUM_8711B            0x00
#endif

//For WoWLan , more reserved page
//ARP Rsp:1, RWC:1, GTK Info:1,GTK RSP:2,GTK EXT MEM:2
#ifdef CONFIG_WOWLAN
#define WOWLAN_PAGE_NUM_8711B           0x07
#else
#define WOWLAN_PAGE_NUM_8711B           0x00
#endif

#ifdef CONFIG_WLAN_HAL_TEST
#define TX_TOTAL_PAGE_NUMBER_8711B      (0xF8 - BCNQ_PAGE_NUM_8711B - BCNQ1_PAGE_NUM_8711B - WOWLAN_PAGE_NUM_8711B)
//#define TX_TOTAL_PAGE_NUMBER_8711B      0x40
#define TX_PAGE_BOUNDARY_8711B          (TX_TOTAL_PAGE_NUMBER_8711B + 1)
#else
#define TX_TOTAL_PAGE_NUMBER_8711B      (0xFF - BCNQ_PAGE_NUM_8711B - BCNQ1_PAGE_NUM_8711B - WOWLAN_PAGE_NUM_8711B)
#define TX_PAGE_BOUNDARY_8711B          (TX_TOTAL_PAGE_NUMBER_8711B + 1)
#endif

#define WMM_NORMAL_TX_TOTAL_PAGE_NUMBER TX_TOTAL_PAGE_NUMBER_8711B
#define WMM_NORMAL_TX_PAGE_BOUNDARY		(WMM_NORMAL_TX_TOTAL_PAGE_NUMBER + 1)

// For Normal Chip Setting
// (HPQ + LPQ + NPQ + PUBQ) shall be TX_TOTAL_PAGE_NUMBER_8711B
#ifdef CONFIG_WLAN_HAL_TEST
#define NORMAL_PAGE_NUM_HPQ_8711B		0x10
#define NORMAL_PAGE_NUM_LPQ_8711B		0x10
#define NORMAL_PAGE_NUM_NPQ_8711B		0x10
#else
#define NORMAL_PAGE_NUM_HPQ_8711B		0x0C
#define NORMAL_PAGE_NUM_LPQ_8711B		0x02
#define NORMAL_PAGE_NUM_NPQ_8711B		0x02
#endif

#ifdef CONFIG_WLAN_HAL_TEST
#define WMM_NORMAL_PAGE_NUM_HPQ_8711B   0x10
#define WMM_NORMAL_PAGE_NUM_LPQ_8711B   0x10
#define WMM_NORMAL_PAGE_NUM_NPQ_8711B   0x10
#else
// Note: For Normal Chip Setting, modify later
#define WMM_NORMAL_PAGE_NUM_HPQ_8711B   0x30
#define WMM_NORMAL_PAGE_NUM_LPQ_8711B   0x20
#define WMM_NORMAL_PAGE_NUM_NPQ_8711B   0x20
#endif

#include "HalVerDef.h"
#include "hal_com.h"

#define LX_DMA_IMR_DISABLED 0
#define FW_IMR_DISABLED     0
#define WL_PMC_IMR_DISABLED 0


//========================================================
//			EFUSE for BT definition
//========================================================
#define EFUSE_BT_REAL_BANK_CONTENT_LEN	512
#define EFUSE_BT_REAL_CONTENT_LEN		1536	// 512*3
#define EFUSE_BT_MAP_LEN				1024	// 1k bytes
#define EFUSE_BT_MAX_SECTION			128		// 1024/8

#define EFUSE_PROTECT_BYTES_BANK		16

#define GET_RF_TYPE(priv)			(GET_HAL_DATA(priv)->rf_type)

// Description: Determine the types of C2H events that are the same in driver and Fw.
// Fisrt constructed by tynli. 2009.10.09.
typedef enum _C2H_EVT
{
	C2H_DBG = 0,
	C2H_TSF = 1,
	C2H_AP_RPT_RSP = 2,
	C2H_CCX_TX_RPT = 3,	// The FW notify the report of the specific tx packet.
	C2H_BT_RSSI = 4,
	C2H_BT_OP_MODE = 5,
	C2H_EXT_RA_RPT = 6,
	C2H_8723B_BT_INFO = 9,
	C2H_HW_INFO_EXCH = 10,
	C2H_8723B_BT_MP_INFO = 11,
	MAX_C2HEVENT
} C2H_EVT;

typedef _PACKED struct _C2H_EVT_HDR
{
	u8	CmdID;
	u8	CmdLen;
	u8	CmdSeq;
} C2H_EVT_HDR, *PC2H_EVT_HDR;

typedef enum tag_Package_Definition
{
	PACKAGE_QFN32,
	PACKAGE_QFN48_MCM,
	PACKAGE_QFN48,
	PACKAGE_QFN68,
}PACKAGE_TYPE_E;

typedef enum tag_ChipID_Definition
{
	CHIPID_8710BN = 0xFF, /* PACKAGE_QFN32 */
	CHIPID_8710BU = 0xFE, /* PACKAGE_QFN48_MCM */
	CHIPID_8711BN = 0xFD, /* PACKAGE_QFN48 */
	CHIPID_8711BG = 0xFC, /* PACKAGE_QFN68 */
}CHIP_TD_E;


#define INCLUDE_MULTI_FUNC_BT(_Adapter)		(GET_HAL_DATA(_Adapter)->MultiFunc & RT_MULTI_FUNC_BT)
#define INCLUDE_MULTI_FUNC_GPS(_Adapter)	(GET_HAL_DATA(_Adapter)->MultiFunc & RT_MULTI_FUNC_GPS)

//========================================================
//			TXBD and RXBD definition
//========================================================
#ifdef CONFIG_MP_INCLUDED	// For MP Tx no idle
#define TX_VIQ_DESC_NUM         4
#define TX_VOQ_DESC_NUM         4
#define TX_BKQ_DESC_NUM         4
#define TX_BEQ_DESC_NUM         32
#else
#define TX_VIQ_DESC_NUM         4
#define TX_VOQ_DESC_NUM         4
#define TX_BKQ_DESC_NUM         4
#define TX_BEQ_DESC_NUM         4
#endif
#ifdef CONFIG_CONCURRENT_MODE
#define TX_BCNQ_DESC_NUM        4
#else
#define TX_BCNQ_DESC_NUM        2
#endif
#define TX_MGQ_DESC_NUM         4
#define TX_H0Q_DESC_NUM         2
#define TX_H1Q_DESC_NUM         2
#define TX_H2Q_DESC_NUM         2
#define TX_H3Q_DESC_NUM         2
#define TX_H4Q_DESC_NUM         2
#define TX_H5Q_DESC_NUM         2
#define TX_H6Q_DESC_NUM         2
#define TX_H7Q_DESC_NUM         2
#define RX_Q_DESC_NUM           4  //16 Reduce rx desc number due to memory limitation

#define SET_VIQ_DES_NUM     (TX_VIQ_DESC_NUM<<16)
#define SET_VOQ_DES_NUM     (TX_VOQ_DESC_NUM)
#define SET_RXQ_DES_NUM     (RX_Q_DESC_NUM<<16)
#define SET_MGQ_DES_NUM     (TX_MGQ_DESC_NUM)
#define SET_BKQ_DES_NUM     (TX_BKQ_DESC_NUM<<16)
#define SET_BEQ_DES_NUM     (TX_BEQ_DESC_NUM)
#define SET_H1Q_DES_NUM     (TX_H1Q_DESC_NUM<<16)
#define SET_H0Q_DES_NUM     (TX_H0Q_DESC_NUM)
#define SET_H3Q_DES_NUM     (TX_H3Q_DESC_NUM<<16)
#define SET_H2Q_DES_NUM     (TX_H2Q_DESC_NUM)
#define SET_H5Q_DES_NUM     (TX_H5Q_DESC_NUM<<16)
#define SET_H4Q_DES_NUM     (TX_H4Q_DESC_NUM)
#define SET_H7Q_DES_NUM     (TX_H7Q_DESC_NUM<<16)
#define SET_H6Q_DES_NUM     (TX_H6Q_DESC_NUM)

#define TX_DESC_MODE        1

//0: 2 segment 
//1: 4 segment 
//2: 8 segment 
//#define TX_DESC_MODE            2

#define MAX_TXBD_SEQMENT_NUM    ((TX_DESC_MODE)? (4*TX_DESC_MODE): 2)
#define TXBD_SEGMENT_SIZE		8



typedef struct _RXBD_ELEMENT_ {
    u32         Dword0;
    u32         PhyAddr;
}RXBD_ELEMENT,*PRXBD_ELEMENT;


typedef struct _TXBD_ELEMENT_ {
    u32         Dword0;
    u32         AddrLow;
}TXBD_ELEMENT,*PTXBD_ELEMENT;

typedef struct _LX_DMA_ELEMENT_ {
    u32         QueueTRxBdBase;
    u32         HwIndex;
    u32         HostIndex;
    u32         AvaliableCnt;
}LX_DMA_ELEMENT, *PLX_DMA_ELEMENT;
#if 1

typedef enum _LX_DMA_QUEUE_TYPE_{
    VO_QUEUE = 0,
    VI_QUEUE = 1,
    BE_QUEUE = 2,
    BK_QUEUE = 3,
    MG_QUEUE = 4,
    RX_QUEUE = 5,
    H0_QUEUE = 6,
    H1_QUEUE = 7,
    H2_QUEUE = 8,
    H3_QUEUE = 9,
    H4_QUEUE = 10,
    H5_QUEUE = 11,
    H6_QUEUE = 12,
    H7_QUEUE = 13,
    BCN_QUEUE = 14,
    MAX_TX_QUEUE = 15,
    ERROR_QUEUE = 16,
}LX_DMA_QUEUE_TYPE, *PLX_DMA_QUEUE_TYPE;

typedef struct _TX_FREE_QUEUE_ {
    _queue               FreeQueue;
    u32                  Qlen;
}TX_FREE_QUEUE, *PTX_FREE_QUEUE;

typedef struct _LX_DMA_MANAGER_ {
    LX_DMA_ELEMENT       QueueTRxBd[MAX_TX_QUEUE];
    u32                  QueueMaxValue[MAX_TX_QUEUE];
    u32                  RxBdSkb[RX_Q_DESC_NUM];
    u32                  RxLen;
    u32                  RemainLen;
    u16                  RxAggregateNum;
    u16                  RxExpectTag;
    u16                  RxSegFlow;
    u16                  Flagls;
    TX_FREE_QUEUE        TxFreeQueue[MAX_TX_QUEUE];
    
}LX_DMA_MANAGER, *PLX_DMA_MANAGER;

#else

typedef struct _LX_DMA_MANAGER_ {
    u32                 *pVoqTXBD;
    u32                 *pViqTXBD;
    u32                 *pBeqTXBD;
    u32                 *pBkqTXBD;
    u32                 *pBcnqTXBD;
    u32                 *pMgqTXBD;
    u32                 *pH0qTXBD;
    u32                 *pH1qTXBD;
    u32                 *pH2qTXBD;
    u32                 *pH3qTXBD;
    u32                 *pH4qTXBD;
    u32                 *pH5qTXBD;
    u32                 *pH6qTXBD;
    u32                 *pH7qTXBD;
    u32                 *pExViqTXBD;
    u32                 *pExVoqTXBD;
    u32                 *pExBeqTXBD;
    u32                 *pExBkqTXBD;
    u32                 *pExMgqTXBD;
    u32                 *pRXBD;
//    u4Byte                  RxAggBufEntry[RX_Q_DESC_NUM];
//    u4Byte                  RxAggLenEntry[RX_Q_DESC_NUM];
    u32                  RxLen;
    u32                  RemainLen;
    u16                  ViqTxWritePoint;
    u16                  ViqTxReadPoint;
    u16                  VoqTxWritePoint;
    u16                  VoqTxReadPoint;
    u16                  BeqTxWritePoint;
    u16                  BeqTxReadPoint;
    u16                  BkqTxWritePoint;
    u16                  BkqTxReadPoint;
    u16                  RxWritePoint;
    u16                  RxReadPoint;
    u16                  RxAggregateNum;
    u16                  RxExpectTag;
    u16                  RxSegFlow;
    u16                  Flagls;
}LX_DMA_MANAGER, *PLX_DMA_MANAGER;
#endif

// rtl8723a_hal_init.c
s32 rtl8711b_FirmwareDownload(PADAPTER padapter, BOOLEAN  bUsedWoWLANFw);
void rtl8711b_FirmwareSelfReset(PADAPTER padapter);
void rtl8711b_InitializeFirmwareVars(PADAPTER padapter);

void rtl8711b_InitAntenna_Selection(PADAPTER padapter);
void rtl8711b_DeinitAntenna_Selection(PADAPTER padapter);
void rtl8711b_CheckAntenna_Selection(PADAPTER padapter);
void rtl8711b_init_default_value(PADAPTER padapter);

s32 rtl8711b_InitLLTTable(PADAPTER padapter);

s32 CardDisableHWSM(PADAPTER padapter, u8 resetMCU);
s32 CardDisableWithoutHWSM(PADAPTER padapter);

// EFuse
//u8 GetEEPROMSize8711b(PADAPTER padapter);
void Hal_InitPGData(PADAPTER padapter, u8 *PROMContent);
void Hal_EfuseParseIDCode(PADAPTER padapter, u8 *hwinfo);
void Hal_EfuseParseTxPowerInfo_8711B(PADAPTER padapter, u8 *PROMContent, BOOLEAN AutoLoadFail);
void Hal_EfuseParseBTCoexistInfo_8711B(PADAPTER padapter, u8 *hwinfo, BOOLEAN AutoLoadFail);
void Hal_EfuseParseEEPROMVer_8711B(PADAPTER padapter, u8 *hwinfo, BOOLEAN AutoLoadFail);
void Hal_EfuseParseChnlPlan_8711B(PADAPTER padapter, u8 *hwinfo, BOOLEAN AutoLoadFail);
void Hal_EfuseParseCustomerID_8711B(PADAPTER padapter, u8 *hwinfo, BOOLEAN AutoLoadFail);
void Hal_EfuseParseAntennaDiversity_8711B(PADAPTER padapter, u8 *hwinfo, BOOLEAN AutoLoadFail);
void Hal_EfuseParseXtal_8711B(PADAPTER pAdapter, u8 *hwinfo, u8 AutoLoadFail);
void Hal_EfuseParseThermalMeter_8711B(PADAPTER padapter, u8 *hwinfo, u8 AutoLoadFail);

#ifdef CONFIG_C2H_PACKET_EN
void C2HPacketHandler_8711B(PADAPTER padapter, u8 *pbuffer, u16 length);
#endif

u8 rtw_flash_read(PADAPTER padapter, u16 addr, u16 cnts, u8 *data);
u8 rtw_flash_write(PADAPTER padapter, u16 addr, u16 cnts, u8 *data);
u8 rtw_config_map_read(PADAPTER padapter, u16 addr, u16 cnts, u8 *data, u8 efuse);
u8 rtw_config_map_write(PADAPTER padapter, u16 addr, u16 cnts, u8 *data, u8 efuse);

void rtl8711b_set_hal_ops(struct hal_ops *pHalFunc);
void lxbus_set_intf_ops(struct _io_ops	*pops);
void SetHwReg8711B(PADAPTER padapter, u8 variable, u8 *val);
void GetHwReg8711B(PADAPTER padapter, u8 variable, u8 *val);
u8 SetHalDefVar8711B(PADAPTER padapter, HAL_DEF_VARIABLE variable, void *pval);
u8 GetHalDefVar8711B(PADAPTER padapter, HAL_DEF_VARIABLE variable, void *pval);
void SetHalODMVar8711B( PADAPTER Adapter, HAL_ODM_VARIABLE eVariable, PVOID pValue1, BOOLEAN bSet);
void GetHalODMVar8711B(PADAPTER Adapter, HAL_ODM_VARIABLE eVariable, PVOID pValue1, BOOLEAN	bSet);

// register
void rtl8711b_InitBeaconParameters(PADAPTER padapter);
void rtl8711b_InitBeaconMaxError(PADAPTER padapter, u8 InfraMode);
void	_InitBurstPktLen_8711BB(PADAPTER Adapter);
#ifdef CONFIG_WOWLAN
void _8051Reset8711b(PADAPTER padapter);
void Hal_DetectWoWMode(PADAPTER pAdapter);
#endif //CONFIG_WOWLAN

void rtl8711b_start_thread(_adapter *padapter);
void rtl8711b_stop_thread(_adapter *padapter);

#if defined(CONFIG_CHECK_BT_HANG) && defined(CONFIG_BT_COEXIST)
void rtl8711bb_init_checkbthang_workqueue(_adapter * adapter);
void rtl8711bb_free_checkbthang_workqueue(_adapter * adapter);
void rtl8711bb_cancle_checkbthang_workqueue(_adapter * adapter);
void rtl8711bb_hal_check_bt_hang(_adapter * adapter);
#endif

#ifdef CONFIG_WOWLAN
void rtw_get_current_ip_address(PADAPTER padapter, u8 *pcurrentip);
void rtw_get_sec_iv(PADAPTER padapter, u8*pcur_dot11txpn, u8 *StaAddr);
#endif

u32 rtl8710b_wlan_suspend(u32 expected_idle_time, void *param);
u32 rtl8710b_wlan_late_resume(u32 expected_idle_time, void *param);
u32 rtl8710b_wlan_resume(u32 expected_idle_time, void *param);

#ifdef CONFIG_GPIO_WAKEUP
void HalSetOutPutGPIO(PADAPTER padapter, u8 index, u8 OutPutValue);
#endif

void CCX_FwC2HTxRpt_8711B(PADAPTER padapter, u8 *pdata, u8 len);
s32 c2h_id_filter_ccx_8711B(u8 *buf);
s32 c2h_handler_8711B(PADAPTER padapter, u8 *pC2hEvent);
u8 MRateToHwRate8723B(u8  rate);
u8 HwRateToMRate8723B(u8	 rate);

#ifdef CONFIG_RF_GAIN_OFFSET
void Hal_ReadRFGainOffset(PADAPTER pAdapter,u8* hwinfo,BOOLEAN AutoLoadFail);
#endif //CONFIG_RF_GAIN_OFFSET


//1TODO: Chris
#if 1

//=============
// [1] Rx Buffer Descriptor (for PCIE) buffer descriptor architecture
//DWORD 0
#define SET_RX_BUFFER_DESC_DATA_LENGTH_92E(__pRxStatusDesc, __Value)		SET_BITS_TO_LE_4BYTE( __pRxStatusDesc, 0, 14, __Value)
#define SET_RX_BUFFER_DESC_LS_92E(__pRxStatusDesc,__Value)	SET_BITS_TO_LE_4BYTE( __pRxStatusDesc, 14, 1, __Value)
#define SET_RX_BUFFER_DESC_FS_92E(__pRxStatusDesc, __Value)		SET_BITS_TO_LE_4BYTE( __pRxStatusDesc, 15, 1, __Value)
#define SET_RX_BUFFER_DESC_RX_TAG_92E(__pRxStatusDesc, __Value)		SET_BITS_TO_LE_4BYTE( __pRxStatusDesc, 16, 13, __Value)

#define GET_RX_BUFFER_DESC_OWN_92E(__pRxStatusDesc)		LE_BITS_TO_4BYTE( __pRxStatusDesc, 31, 1)
#define GET_RX_BUFFER_DESC_LS_92E(__pRxStatusDesc)		LE_BITS_TO_4BYTE( __pRxStatusDesc, 14, 1)
#define GET_RX_BUFFER_DESC_FS_92E(__pRxStatusDesc)		LE_BITS_TO_4BYTE( __pRxStatusDesc, 15, 1)
#define GET_RX_BUFFER_DESC_RX_TAG_92E(__pRxStatusDesc)		LE_BITS_TO_4BYTE( __pRxStatusDesc, 16, 13)
#define GET_RX_BUFFER_DESC_TOTAL_LENGTH_92E(__pRxStatusDesc)LE_BITS_TO_4BYTE( __pRxStatusDesc, 0, 14) 


//DWORD 1
#define SET_RX_BUFFER_PHYSICAL_LOW_92E(__pRxStatusDesc, __Value)		SET_BITS_TO_LE_4BYTE( __pRxStatusDesc+4, 0, 32, __Value)
#define GET_RX_BUFFER_PHYSICAL_LOW_92E(__pRxStatusDesc)		LE_BITS_TO_4BYTE( __pRxStatusDesc+4, 0, 32)

//DWORD 2
#define SET_RX_BUFFER_PHYSICAL_HIGH_92E(__pRxStatusDesc, __Value)		SET_BITS_TO_LE_4BYTE( __pRxStatusDesc+8, 0, 32, __Value)


//=====Tx Desc Buffer content

// config element for each tx buffer 
/*
#define SET_TXBUFFER_DESC_LEN_WITH_OFFSET(__pTxDesc, __Offset, __Valeu) SET_BITS_TO_LE_4BYTE(__pTxDesc+(__Offset*16), 0, 16, __Valeu)
#define SET_TXBUFFER_DESC_AMSDU_WITH_OFFSET(__pTxDesc, __Offset, __Valeu) SET_BITS_TO_LE_4BYTE(__pTxDesc+(__Offset*16), 31, 1, __Valeu)
#define SET_TXBUFFER_DESC_ADD_LOW_WITH_OFFSET(__pTxDesc, __Offset, __Valeu) SET_BITS_TO_LE_4BYTE(__pTxDesc+(__Offset*16)+4, 0, 32, __Valeu)
#define SET_TXBUFFER_DESC_ADD_HIGT_WITH_OFFSET(__pTxDesc, __Offset, __Valeu) SET_BITS_TO_LE_4BYTE(__pTxDesc+(__Offset*16)+8, 0, 32, __Valeu)
*/
#define SET_TXBUFFER_DESC_LEN_WITH_OFFSET(__pTxDesc, __Offset, __Valeu) SET_BITS_TO_LE_4BYTE(__pTxDesc+(__Offset*8), 0, 16, __Valeu)
#define SET_TXBUFFER_DESC_AMSDU_WITH_OFFSET(__pTxDesc, __Offset, __Valeu) SET_BITS_TO_LE_4BYTE(__pTxDesc+(__Offset*8), 31, 1, __Valeu)
#define SET_TXBUFFER_DESC_ADD_LOW_WITH_OFFSET(__pTxDesc, __Offset, __Valeu) SET_BITS_TO_LE_4BYTE(__pTxDesc+(__Offset*8)+4, 0, 32, __Valeu)
#define SET_TXBUFFER_DESC_ADD_HIGT_WITH_OFFSET(__pTxDesc, __Offset, __Valeu) SET_BITS_TO_LE_4BYTE(__pTxDesc+(__Offset*16)+8, 0, 32, __Valeu)

// Dword 0
#define SET_TX_BUFF_DESC_LEN_0_92E(__pTxDesc, __Valeu) SET_BITS_TO_LE_4BYTE(__pTxDesc, 0, 16, __Valeu)
#define SET_TX_BUFF_DESC_PSB_92E(__pTxDesc, __Value) SET_BITS_TO_LE_4BYTE(__pTxDesc, 16, 8, __Value)
#define SET_TX_BUFF_DESC_OWN_92E(__pTxDesc, __Value) SET_BITS_TO_LE_4BYTE(__pTxDesc, 31, 1, __Value)
// Dword 1
#define SET_TX_BUFF_DESC_ADDR_LOW_0_92E(__pTxDesc, __Value) SET_BITS_TO_LE_4BYTE(__pTxDesc+4, 0, 32, __Value)
#define GET_TX_DESC_TX_BUFFER_ADDRESS_92E(__pTxDesc) LE_BITS_TO_4BYTE(__pTxDesc+4, 0,32)


// Dword 2
#define SET_TX_BUFF_DESC_ADDR_HIGH_0_92E(__pTxDesc, __Value) SET_BITS_TO_LE_4BYTE(__pTxDesc+8, 0, 32, __Value)
// Dword 3, RESERVED 


#define SET_TX_DESC_OWN_92E(__pTxDesc, __Value) SET_BITS_TO_LE_4BYTE(__pTxDesc, 31, 1, __Value)

#endif

#endif

