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
#ifndef __INC_HAL8195APHYREG_H__
#define __INC_HAL8195APHYREG_H__

//
// 1. PMAC duplicate register due to connection: RF_Mode, TRxRN, NumOf L-STF
// Page1(0x100)
//
#define		rPMAC_Reset					0x100

//
// Page8(0x800)
//
#define		rFPGA0_RFMOD				0x800	//RF mode & CCK TxSC // RF BW Setting??
#define		rFPGA0_TxInfo				0x804	// Status report??
#define		rFPGA0_TxGainStage			0x80c	// Set TX PWR init gain?
#define		rFPGA0_XA_HSSIParameter1	0x820	// RF 3 wire register
#define		rFPGA0_XA_HSSIParameter2	0x824
#define		rFPGA0_XB_HSSIParameter1	0x828
#define		rFPGA0_XB_HSSIParameter2	0x82c
#define		rTxAGC_B_Rate18_06			0x830
#define		rTxAGC_B_Rate54_24			0x834
#define		rTxAGC_B_CCK1_55_Mcs32		0x838
#define		rTxAGC_B_Mcs03_Mcs00		0x83c
#define		rFPGA0_XA_LSSIParameter		0x840
#define		rFPGA0_XB_LSSIParameter		0x844
#define		rTxAGC_B_Mcs07_Mcs04		0x848
#define		rTxAGC_B_Mcs11_Mcs08		0x84c
#define		rFPGA0_XCD_SwitchControl	0x85c
#define		rFPGA0_XA_RFInterfaceOE		0x860	// RF Channel switch
#define		rFPGA0_XB_RFInterfaceOE		0x864
#define		rTxAGC_B_CCK11_A_CCK2_11	0x86c
#define		rTxAGC_B_Mcs15_Mcs12		0x868
#define		rFPGA0_XAB_RFInterfaceSW	0x870	// RF Interface Software Control
#define		rFPGA0_XCD_RFInterfaceSW	0x874
#define		rFPGA0_XAB_RFParameter		0x878	// RF Parameter
#define		rFPGA0_XA_LSSIReadBack		0x8a0	// Tranceiver LSSI Readback
#define		rFPGA0_XB_LSSIReadBack		0x8a4
#define		rFPGA0_XC_LSSIReadBack		0x8a8
#define		rFPGA0_XD_LSSIReadBack		0x8ac
#define		TransceiverA_HSPI_Readback	0x8b8	// Transceiver A HSPI Readback
#define		TransceiverB_HSPI_Readback	0x8bc	// Transceiver B HSPI Readback

//
// Page9(0x900)
//
#define		rFPGA1_RFMOD				0x900	//RF mode & OFDM TxSC // RF BW Setting??
#define		rFPGA1_TxInfo				0x90c	// Useless now // Status report??
#define 	rS0S1_PathSwitch   			0x948
#define		rRXDFIR_Filter				0x954

//
// PageA(0xA00)
//
// Set Control channel to upper or lower. These settings are required only for 40MHz
#define		rCCK0_System				0xa00
#define		rCCK0_AFESetting			0xa04	// Disable init gain now // Select RX path by RSSI

//
// PageB(0xB00)
//
#define rPdp_AntA						0xb00
#define rPdp_AntA_4						0xb04
#define rPdp_AntA_8						0xb08
#define rPdp_AntA_C						0xb0c
#define rPdp_AntA_10					0xb10
#define rPdp_AntA_14					0xb14
#define rPdp_AntA_18					0xb18
#define rPdp_AntA_1C					0xb1c
#define rPdp_AntA_20					0xb20
#define rPdp_AntA_24					0xb24

#define rConfig_Pmpd_AntA 				0xb28
#define rConfig_ram64x16				0xb2c

#define rBndA							0xb30
#define rHssiPar						0xb34

#define rConfig_AntA					0xb68
#define rConfig_AntB					0xb6c

#define rPdp_AntB						0xb70
#define rPdp_AntB_4						0xb74
#define rPdp_AntB_8						0xb78
#define rPdp_AntB_C						0xb7c
#define rPdp_AntB_10					0xb80
#define rPdp_AntB_14					0xb84
#define rPdp_AntB_18					0xb88
#define rPdp_AntB_1C					0xb8c
#define rPdp_AntB_20					0xb90
#define rPdp_AntB_24					0xb94

#define rConfig_Pmpd_AntB				0xb98

#define rBndB							0xba0

#define rAPK							0xbd8
#define rPm_Rx0_AntA					0xbdc
#define rPm_Rx1_AntA					0xbe0
#define rPm_Rx2_AntA					0xbe4
#define rPm_Rx3_AntA					0xbe8
#define rPm_Rx0_AntB					0xbec
#define rPm_Rx1_AntB					0xbf0
#define rPm_Rx2_AntB					0xbf4
#define rPm_Rx3_AntB					0xbf8

//
// PageC(0xC00)
//
#define		rOFDM0_TRxPathEnable		0xc04
#define		rOFDM0_TRMuxPar				0xc08
#define		rOFDM0_XARxAFE				0xc10	// RxIQ DC offset, Rx digital filter, DC notch filter
#define		rOFDM0_XARxIQImbalance    	0xc14	// RxIQ imblance matrix
#define		rOFDM0_XBRxIQImbalance    	0xc1c
#define		rOFDM0_RxDetector1			0xc30	// PD,BW & SBD	// DM tune init gain
#define		rOFDM0_ECCAThreshold		0xc4c	// energy CCA
#define		rOFDM0_XAAGCCore1			0xc50	// DIG
#define		rOFDM0_XBAGCCore1			0xc58
#define		rOFDM0_AGCRSSITable			0xc78
#define		rOFDM0_XATxIQImbalance		0xc80	// TX PWR TRACK and DIG
#define		rOFDM0_XBTxIQImbalance		0xc88
#define		rOFDM0_XCTxAFE            	0xc94
#define		rOFDM0_XDTxAFE				0xc9c
#define		rOFDM0_RxIQExtAnta			0xca0
#define		rOFDM0_TxPseudoNoiseWgt		0xce4	// Double ADC

//
// PageD(0xD00)
//
#define		rOFDM1_LSTF					0xd00
#define		rOFDM1_TRxPathEnable		0xd04

//
// PageE(0xE00)
//
#define		rTxAGC_A_Rate18_06			0xe00
#define		rTxAGC_A_Rate54_24			0xe04
#define		rTxAGC_A_CCK1_Mcs32			0xe08
#define		rTxAGC_A_Mcs03_Mcs00		0xe10
#define		rTxAGC_A_Mcs07_Mcs04		0xe14
#define		rTxAGC_A_Mcs11_Mcs08		0xe18
#define		rTxAGC_A_Mcs15_Mcs12		0xe1c
#define		rFPGA0_IQK					0xe28
#define		rTx_IQK_Tone_A				0xe30
#define		rRx_IQK_Tone_A				0xe34
#define		rTx_IQK_PI_A				0xe38
#define		rRx_IQK_PI_A				0xe3c
#define		rTx_IQK 					0xe40
#define		rRx_IQK						0xe44
#define		rIQK_AGC_Pts				0xe48
#define		rIQK_AGC_Rsp				0xe4c
#define		rTx_IQK_Tone_B				0xe50
#define		rRx_IQK_Tone_B				0xe54
#define		rTx_IQK_PI_B				0xe58
#define		rRx_IQK_PI_B				0xe5c
#define		rIQK_AGC_Cont				0xe60
#define		rBlue_Tooth					0xe6c
#define		rRx_Wait_CCA				0xe70	// Rx ADC clock
#define		rTx_CCK_RFON				0xe74
#define		rTx_CCK_BBON				0xe78
#define		rTx_OFDM_RFON				0xe7c
#define		rTx_OFDM_BBON				0xe80
#define		rTx_To_Rx					0xe84
#define		rTx_To_Tx					0xe88
#define		rRx_CCK						0xe8c
#define		rTx_Power_Before_IQK_A		0xe94
#define		rTx_Power_After_IQK_A		0xe9c
#define		rRx_Power_Before_IQK_A_2	0xea4
#define		rRx_Power_After_IQK_A_2		0xeac
#define		rTx_Power_Before_IQK_B		0xeb4
#define		rTx_Power_After_IQK_B		0xebc
#define		rRx_Power_Before_IQK_B		0xec0
#define		rRx_Power_Before_IQK_B_2	0xec4
#define		rRx_Power_After_IQK_B		0xec8
#define		rRx_Power_After_IQK_B_2		0xecc
#define		rRx_OFDM					0xed0
#define		rRx_Wait_RIFS 				0xed4
#define		rRx_TO_Rx 					0xed8
#define		rStandby 					0xedc
#define		rSleep 						0xee0
#define		rPMPD_ANAEN					0xeec


//for PutRegsetting & GetRegSetting BitMask
#define		bMaskH3Bytes				0xffffff00

//
// RL6052 Register definition
//
#define		RF_AC						0x00	//
#define		RF_TXM_IDAC					0x08	// 
#define		RF_CHNLBW					0x18	// RF channel and BW switch
#define		RF_RCK_OS					0x30	// RF TX PA control
#define		RF_TXPA_G1					0x31	// RF TX PA control
#define		RF_TXPA_G2					0x32	// RF TX PA control
#define		RF_WE_LUT					0xEF

//
//Bit Mask
//
// 1. Page1(0x100)
#define		bBBResetB					0x100	// Useless now?

// 2. Page8(0x800)
#define		bRFMOD						0x1			// Reg 0x800 rFPGA0_RFMOD
#define		bCCKEn						0x1000000
#define		bOFDMEn						0x2000000
#define		bXBTxAGC                  	0xf00	// Reg 80c rFPGA0_TxGainStage
#define		bXCTxAGC                  	0xf000
#define		bXDTxAGC                  	0xf0000
#define		b3WireDataLength          	0x800		// Reg 0x820~84f rFPGA0_XA_HSSIParameter1
#define		b3WireAddressLength       	0x400
#define		bRFSI_RFENV               	0x10		// Reg 0x870 rFPGA0_XAB_RFInterfaceSW
#define		bLSSIReadAddress          	0x7f800000	// T65 RF
#define		bLSSIReadEdge             	0x80000000	//LSSI "Read" edge signal
#define		bLSSIReadBackData         	0xfffff		// T65 RF

// 3. Page9(0x900)

// 4. PageA(0xA00)
#define		bCCKBBMode					0x3
#define		bCCKSideBand				0x10		// Reg 0xa00 rCCK0_System 20/40 switch
#define		bCCKScramble				0x8
#define		bCCKTxRate					0x3000


// 5. PageC(0xC00)

// 6. PageE(0xE00)
#define		bOFDMContinueTx				0x10000000
#define		bOFDMSingleCarrier			0x20000000
#define		bOFDMSingleTone				0x40000000

//
// Other Definition
//

//for PutRegsetting & GetRegSetting BitMask
#define		bMaskByte0                	0xff
#define		bMaskByte1                	0xff00
#define		bMaskByte2                	0xff0000
#define		bMaskByte3                	0xff000000
#define		bMaskHWord                	0xffff0000
#define		bMaskLWord                	0x0000ffff
#define		bMaskDWord                	0xffffffff
#define		bMask12Bits					0xfff
#define		bMaskH4Bits					0xf0000000	

#define		bEnable						0x1
#define		bDisable					0x0


#endif

