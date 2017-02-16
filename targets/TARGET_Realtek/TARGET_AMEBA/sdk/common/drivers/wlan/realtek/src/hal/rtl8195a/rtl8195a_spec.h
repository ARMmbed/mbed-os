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
 *******************************************************************************/
#ifndef __RTL8195A_SPEC_H__
#define __RTL8195A_SPEC_H__

#include <drv_conf.h>


#define HAL_NAV_UPPER_UNIT_8723B		128		// micro-second

//-----------------------------------------------------
//
//	0x0000h ~ 0x00FFh	System Configuration
//
//-----------------------------------------------------
#define REG_BT_WIFI_ANTENNA_SWITCH_8723B	0x0038
#define REG_PAD_CTRL1_8723B		0x0064
#define REG_AFE_CTRL_4_8723B		0x0078
#define REG_HMEBOX_DBG_0_8723B	0x0088
#define REG_HMEBOX_DBG_1_8723B	0x008A
#define REG_HMEBOX_DBG_2_8723B	0x008C
#define REG_HMEBOX_DBG_3_8723B	0x008E

//-----------------------------------------------------
//
//	0x0100h ~ 0x01FFh	MACTOP General Configuration
//
//-----------------------------------------------------
#define REG_C2HEVT_CMD_ID_8723B	0x01A0
#define REG_C2HEVT_CMD_LEN_8723B	0x01AE
#ifdef CONFIG_WOWLAN
#define REG_WOWLAN_WAKE_REASON 0x01C7
#endif

#define REG_HMEBOX_EXT0_8723B			0x01F0
#define REG_HMEBOX_EXT1_8723B			0x01F4
#define REG_HMEBOX_EXT2_8723B			0x01F8
#define REG_HMEBOX_EXT3_8723B			0x01FC

//-----------------------------------------------------
//
//	0x0200h ~ 0x027Fh	TXDMA Configuration
//
//-----------------------------------------------------

//-----------------------------------------------------
//
//	0x0280h ~ 0x02FFh	RXDMA Configuration
//
//-----------------------------------------------------
#define REG_RXDMA_MODE_CTRL_8723B		0x0290

//-----------------------------------------------------
//
//	0x0300h ~ 0x03FFh	PCIe
//
//-----------------------------------------------------

//-----------------------------------------------------
//
//	0x0400h ~ 0x047Fh	Protocol Configuration
//
//-----------------------------------------------------
#define REG_TXPKTBUF_BCNQ_BDNY_8195A        0x0424
#define REG_TXPKTBUF_MGQ_BDNY_8195A	        0x0425
#define REG_TXPKTBUF_WMAC_LBK_BF_HD_8195A   0x045D
#ifdef CONFIG_WOWLAN
#define REG_TXPKTBUF_IV_LOW             0x0484
#define REG_TXPKTBUF_IV_HIGH            0x0488
#endif

//-----------------------------------------------------
//
//	0x0500h ~ 0x05FFh	EDCA Configuration
//
//-----------------------------------------------------
#define REG_SECONDARY_CCA_CTRL_8723B	0x0577

//-----------------------------------------------------
//
//	0x0600h ~ 0x07FFh	WMAC Configuration
//
//-----------------------------------------------------


//============================================================
// SDIO Bus Specification
//============================================================

//-----------------------------------------------------
// SDIO CMD Address Mapping
//-----------------------------------------------------

//-----------------------------------------------------
// I/O bus domain (Host)
//-----------------------------------------------------

//-----------------------------------------------------
// SDIO register
//-----------------------------------------------------
#define SDIO_REG_HCPWM1_8723B	0x025 // HCI Current Power Mode 1


//============================================================================
//	8723 Regsiter Bit and Content definition
//============================================================================

//2 HSISR
// interrupt mask which needs to clear
#define MASK_HSISR_CLEAR		(HSISR_GPIO12_0_INT |\
								HSISR_SPS_OCP_INT |\
								HSISR_RON_INT |\
								HSISR_PDNINT |\
								HSISR_GPIO9_INT)

//-----------------------------------------------------
//
//	0x0100h ~ 0x01FFh	MACTOP General Configuration
//
//-----------------------------------------------------

#define RXDMA_AGG_MODE_EN		BIT(1)
//-----------------------------------------------------
//
//	0x0200h ~ 0x027Fh	TXDMA Configuration
//
//-----------------------------------------------------

//-----------------------------------------------------
//
//	0x0280h ~ 0x02FFh	RXDMA Configuration
//
//-----------------------------------------------------
#ifdef CONFIG_WOWLAN
#define RXPKT_RELEASE_POLL		BIT(16)
#define RXDMA_IDLE				BIT(17)
#define RW_RELEASE_EN			BIT(18)
#endif

//-----------------------------------------------------
//
//	0x0400h ~ 0x047Fh	Protocol Configuration
//
//-----------------------------------------------------

//-----------------------------------------------------
//
//	0x0500h ~ 0x05FFh	EDCA Configuration
//
//-----------------------------------------------------

//-----------------------------------------------------
//
//	0x0600h ~ 0x07FFh	WMAC Configuration
//
//-----------------------------------------------------

#endif

//====================================================
//			EEPROM/Efuse PG Offset for 8195A
//====================================================
// 0x10 ~ 0x63 = TX power area.
#define	EEPROM_TX_PWR_INX_8195A				0x20
#define	EEPROM_ChannelPlan_8195A			0xC8
#define	EEPROM_XTAL_8195A					0xC9
#define	EEPROM_THERMAL_METER_8195A			0xCA

#define	EEPROM_IQK_LCK_8195A				0xCB
#define	EEPROM_2G_5G_PA_TYPE_8195A			0xCC
#define	EEPROM_2G_LNA_TYPE_GAIN_SEL_8195A	0xCD
#define	EEPROM_5G_LNA_TYPE_GAIN_SEL_8195A	0xCE

#define	EEPROM_RF_BOARD_OPTION_8195A		0x131

#define	EEPROM_FEATURE_OPTION_8195A			0x132
#define	EEPROM_RF_BT_SETTING_8195A			0x133

#define	EEPROM_VERSION_8195A				0x134
#define	EEPROM_CustomID_8195A				0x135

#define	EEPROM_TX_BBSWING_2G_8195A			0x136
#define	EEPROM_TX_PWR_CAL_RATE_8195A		0x138
#define	EEPROM_RF_ANTENNA_OPT_8195A			0x139
#define	EEPROM_RFE_OPTION_8195A				0x13A

//RTL8723BU
#define EEPROM_MAC_ADDR_8723BU				0x107
#define EEPROM_VID_8723BU					0x100
#define EEPROM_PID_8723BU					0x102
#define EEPROM_PA_TYPE_8723BU				0xBC
#define EEPROM_LNA_TYPE_2G_8723BU			0xBD

//RTL8723BS
#define	EEPROM_MAC_ADDR_8723BS				0x11A
#define	EEPROM_MAC_ADDR_8195A				0x11A

#define EEPROM_Voltage_ADDR_8723B			0x8

#define EEPROM_TX_KFREE_8195A				0xEE

#define EEPROM_PACKAGE_TYPE_8195A			0xF8

//====================================================
//			EEPROM/Efuse Value Type
//====================================================
#define EETYPE_TX_PWR							0x0
//====================================================
//			EEPROM/Efuse Default Value
//====================================================
#define EEPROM_CID_DEFAULT					0x0
#define EEPROM_CID_DEFAULT_EXT				0xFF // Reserved for Realtek
#define EEPROM_CID_TOSHIBA						0x4
#define EEPROM_CID_CCX							0x10
#define EEPROM_CID_QMI							0x0D
#define EEPROM_CID_WHQL 						0xFE

#define EEPROM_CHANNEL_PLAN_FCC				0x0
#define EEPROM_CHANNEL_PLAN_IC				0x1
#define EEPROM_CHANNEL_PLAN_ETSI				0x2
#define EEPROM_CHANNEL_PLAN_SPAIN			0x3
#define EEPROM_CHANNEL_PLAN_FRANCE			0x4
#define EEPROM_CHANNEL_PLAN_MKK				0x5
#define EEPROM_CHANNEL_PLAN_MKK1				0x6
#define EEPROM_CHANNEL_PLAN_ISRAEL			0x7
#define EEPROM_CHANNEL_PLAN_TELEC			0x8
#define EEPROM_CHANNEL_PLAN_GLOBAL_DOMAIN	0x9
#define EEPROM_CHANNEL_PLAN_WORLD_WIDE_13	0xA
#define EEPROM_CHANNEL_PLAN_NCC_TAIWAN		0xB
#define EEPROM_CHANNEL_PLAN_CHIAN			0XC
#define EEPROM_CHANNEL_PLAN_SINGAPORE_INDIA_MEXICO  0XD
#define EEPROM_CHANNEL_PLAN_KOREA			0xE
#define EEPROM_CHANNEL_PLAN_TURKEY              	0xF
#define EEPROM_CHANNEL_PLAN_JAPAN                 	0x10
#define EEPROM_CHANNEL_PLAN_FCC_NO_DFS		0x11
#define EEPROM_CHANNEL_PLAN_JAPAN_NO_DFS	0x12
#define EEPROM_CHANNEL_PLAN_WORLD_WIDE_5G	0x13
#define EEPROM_CHANNEL_PLAN_TAIWAN_NO_DFS 	0x14

#define EEPROM_USB_OPTIONAL1					0xE
#define EEPROM_CHANNEL_PLAN_BY_HW_MASK		0x80

#define RTL_EEPROM_ID							0x8129
#define EEPROM_Default_TSSI						0x0
#define EEPROM_Default_BoardType				0x02
#define EEPROM_Default_ThermalMeter			0x12
#define EEPROM_Default_ThermalMeter_92SU		0x7
#define EEPROM_Default_ThermalMeter_88E		0x18
#define EEPROM_Default_ThermalMeter_8812		0x18
#define	EEPROM_Default_ThermalMeter_8192E			0x1A
#define	EEPROM_Default_ThermalMeter_8723B		0x18
#define	EEPROM_Default_ThermalMeter_8195A		0x1A


#define EEPROM_Default_CrystalCap				0x0
#define EEPROM_Default_CrystalCap_8723B			0x20
#define EEPROM_Default_CrystalCap_8195A			0x20
#define EEPROM_Default_CrystalFreq				0x0
#define EEPROM_Default_TxPowerLevel_92C		0x22
#define EEPROM_Default_TxPowerLevel_2G			0x2C
#define EEPROM_Default_TxPowerLevel_5G			0x22
#define EEPROM_Default_TxPowerLevel			0x22
#define EEPROM_Default_HT40_2SDiff				0x0
#define EEPROM_Default_HT20_Diff				2
#define EEPROM_Default_LegacyHTTxPowerDiff		0x3
#define EEPROM_Default_LegacyHTTxPowerDiff_92C	0x3
#define EEPROM_Default_LegacyHTTxPowerDiff_92D	0x4	
#define EEPROM_Default_HT40_PwrMaxOffset		0
#define EEPROM_Default_HT20_PwrMaxOffset		0

#define EEPROM_Default_PID						0x1234
#define EEPROM_Default_VID						0x5678
#define EEPROM_Default_CustomerID				0xAB
#define EEPROM_Default_CustomerID_8188E		0x00
#define EEPROM_Default_SubCustomerID			0xCD
#define EEPROM_Default_Version					0

#define EEPROM_Default_externalPA_C9		0x00
#define EEPROM_Default_externalPA_CC		0xFF
#define EEPROM_Default_internalPA_SP3T_C9	0xAA
#define EEPROM_Default_internalPA_SP3T_CC	0xAF
#define EEPROM_Default_internalPA_SPDT_C9	0xAA
#ifdef CONFIG_PCI_HCI
#define EEPROM_Default_internalPA_SPDT_CC	0xA0
#else
#define EEPROM_Default_internalPA_SPDT_CC	0xFA
#endif
#define EEPROM_Default_PAType						0
#define EEPROM_Default_LNAType						0

//New EFUSE deafult value
#define EEPROM_DEFAULT_24G_CCK_INDEX		0x20
#define EEPROM_DEFAULT_24G_40M_INDEX		0x20
#define EEPROM_DEFAULT_24G_HT20_DIFF		0X00
#define EEPROM_DEFAULT_24G_OFDM_DIFF		0X02

#define EEPROM_DEFAULT_5G_INDEX				0X2A
#define EEPROM_DEFAULT_5G_HT20_DIFF			0X00
#define EEPROM_DEFAULT_5G_OFDM_DIFF			0X04

#define EEPROM_DEFAULT_DIFF					0XFE
#define EEPROM_DEFAULT_CHANNEL_PLAN			0x7F
#define EEPROM_DEFAULT_BOARD_OPTION			0x01	// Enable power by rate and power limit
#define EEPROM_DEFAULT_RFE_OPTION			0x04
#define EEPROM_DEFAULT_FEATURE_OPTION		0x00
#define EEPROM_DEFAULT_BT_OPTION			0x10
#define EEPROM_DEFAULT_TX_CALIBRATE_RATE	0x00






#ifdef CONFIG_USB_HCI
//should be renamed and moved to another file
typedef	enum _BOARD_TYPE_8192CUSB{
	BOARD_USB_DONGLE 			= 0,		// USB dongle
	BOARD_USB_High_PA 		= 1,		// USB dongle with high power PA
	BOARD_MINICARD		  	= 2,		// Minicard
	BOARD_USB_SOLO 		 	= 3,		// USB solo-Slim module
	BOARD_USB_COMBO			= 4,		// USB Combo-Slim module
} BOARD_TYPE_8723BUSB, *PBOARD_TYPE_8723BUSB;

#endif

#if defined (CONFIG_PCI_HCI) || defined(CONFIG_LX_HCI)
#define RT_BCN_INT_MASKS				(IMR_BcnInt | IMR_TBDOK | IMR_TBDER)
#define RT_AC_INT_MASKS				(IMR_VIDOK | IMR_VODOK | IMR_BEDOK|IMR_BKDOK)
#endif
