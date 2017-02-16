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
#ifndef __INC_HAL8195APHYCFG_H__
#define __INC_HAL8195APHYCFG_H__

/*--------------------------Define Parameters-------------------------------*/
#define LOOP_LIMIT				5
#define MAX_STALL_TIME			50		//us
#define AntennaDiversityValue	0x80	//(Adapter->bSoftwareAntennaDiversity ? 0x00:0x80)
#define MAX_TXPWR_IDX_NMODE_92S	63
#define Reset_Cnt_Limit			3

#if defined (CONFIG_PCI_HCI) || defined(CONFIG_LX_HCI)
#define MAX_AGGR_NUM	0x0B
#else
#define MAX_AGGR_NUM	0x07
#endif // CONFIG_PCI_HCI


/*--------------------------Define Parameters End-------------------------------*/


/*------------------------------Define structure----------------------------*/

/* BB/RF related */

typedef struct _R_ANTENNA_SELECT_OFDM{	
	u32			r_tx_antenna:4;	
	u32			r_ant_l:4;
	u32			r_ant_non_ht:4;	
	u32			r_ant_ht1:4;
	u32			r_ant_ht2:4;
	u32			r_ant_ht_s1:4;
	u32			r_ant_non_ht_s1:4;
	u32			OFDM_TXSC:2;
	u32			Reserved:2;
}R_ANTENNA_SELECT_OFDM;

typedef struct _R_ANTENNA_SELECT_CCK{
	u8			r_cckrx_enable_2:2;	
	u8			r_cckrx_enable:2;
	u8			r_ccktx_enable:4;
}R_ANTENNA_SELECT_CCK;

/*------------------------------Define structure End----------------------------*/

/*--------------------------Exported Function prototype---------------------*/

u32
PHY_QueryBBReg_8195A(
	IN	PADAPTER	Adapter,
	IN	u32		RegAddr,
	IN	u32		BitMask
	);

VOID
PHY_SetBBReg_8195A(
	IN	PADAPTER	Adapter,
	IN	u32		RegAddr,
	IN	u32		BitMask,
	IN	u32		Data
	);

u32
PHY_QueryRFReg_8195A(
	IN	PADAPTER			Adapter,
	IN	u32				eRFPath,
	IN	u32				RegAddr,
	IN	u32				BitMask
	);

VOID
PHY_SetRFReg_8195A(
	IN	PADAPTER			Adapter,
	IN	u32				eRFPath,
	IN	u32				RegAddr,
	IN	u32				BitMask,
	IN	u32				Data
	);

u32 PHY_QueryBBReg_8195A_Safe(
	IN PADAPTER         Adapter,
	IN u32              RegAddr,
	IN u32              BitMask
);

VOID PHY_SetBBReg_8195A_Safe(
	IN PADAPTER         Adapter,
	IN u32              RegAddr,
	IN u32              BitMask,
	IN u32              Data
);

#define PHY_QueryBBReg(Adapter, RegAddr, BitMask) PHY_QueryBBReg_8195A_Safe((Adapter), (RegAddr), (BitMask))
#define PHY_SetBBReg(Adapter, RegAddr, BitMask, Data) PHY_SetBBReg_8195A_Safe((Adapter), (RegAddr), (BitMask), (Data))
#define PHY_QueryRFReg(Adapter, eRFPath, RegAddr, BitMask) PHY_QueryRFReg_8195A((Adapter), (eRFPath), (RegAddr), (BitMask))
#define PHY_SetRFReg(Adapter, eRFPath, RegAddr, BitMask, Data) PHY_SetRFReg_8195A((Adapter), (eRFPath), (RegAddr), (BitMask), (Data))

#define PHY_SetMacReg	PHY_SetBBReg
#define PHY_QueryMacReg PHY_QueryBBReg
/* MAC/BB/RF HAL config */
int PHY_BBConfig8195A(PADAPTER	Adapter	);

int PHY_RFConfig8195A(PADAPTER	Adapter	);

s32 PHY_MACConfig8195A(PADAPTER padapter);

#ifdef CONFIG_SUDO_PHY_SETTING
int PHY_SudoPhyConfig8195A(PADAPTER Adapter);
#endif

int
PHY_ConfigRFWithParaFile_8195A(
	IN	PADAPTER			Adapter,
	IN	u8* 				pFileName,
	RF_PATH				eRFPath
);
int
PHY_ConfigRFWithHeaderFile_8723B(
	IN	PADAPTER			Adapter,
	RF_PATH				eRFPath
);

u8
PHY_GetTxPowerIndex_8195A(
	IN	PADAPTER			pAdapter,
	IN	u8					RFPath,
	IN	u8					Rate,	
	IN	CHANNEL_WIDTH		BandWidth,	
	IN	u8					Channel
	);

VOID
PHY_SetTxPowerLevel8195A(
	IN	PADAPTER		Adapter,
	IN	u8			channel
	);

VOID
PHY_SetBWMode8195A(
	IN	PADAPTER				Adapter,
	IN	CHANNEL_WIDTH			Bandwidth,	// 20M or 40M
	IN	unsigned char				Offset		// Upper, Lower, or Don't care
);

VOID
PHY_SwChnl8195A(	// Call after initialization
	IN	PADAPTER	Adapter,
	IN	u8		channel
	);

VOID
PHY_SetSwChnlBWMode8195A(
	IN	PADAPTER			Adapter,
	IN	u8					channel,
	IN	CHANNEL_WIDTH		Bandwidth,
	IN	u8					Offset40,
	IN	u8					Offset80
);

VOID PHY_SetRFPathSwitch_8723B(
	IN	PADAPTER	pAdapter,
	IN	BOOLEAN		bMain
	);

/*--------------------------Exported Function prototype End---------------------*/

#endif

