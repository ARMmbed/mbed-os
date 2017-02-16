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
#ifndef __RTL8195A_PMU_CMD_H__
#define __RTL8195A_PMU_CMD_H__

typedef enum _RT_MEDIA_STATUS{
    RT_MEDIA_DISCONNECT     = 0,
    RT_MEDIA_CONNECT        = 1
}RT_MEDIA_STATUS;

typedef enum _H2C_CMD_ {
    //1 Class1: Common
    H2CID_RSVDPAGE              = 0x00,
    H2CID_JOININFO              = 0x01,
    H2CID_SCAN                  = 0x02,
    H2CID_KEEP_ALIVE            = 0x03,
    H2CID_DISCONNECT_DECISION   = 0x04,
    H2CID_PSD_OFFLOAD           = 0x05,
    rsvd2                       = 0x06,
    rsvd3                       = 0x07,    
    H2CID_AP_OFFLOAD            = 0x08,        
    H2CID_BCN_RsvdPage          = 0x09,     
    H2CID_Probersp_RsvdPage     = 0x0A,     
    H2CID_AP_OFFLOAD_STAINFO    = 0x0B,      

    H2CID_FAST_CS_RSVDPAGE      = 0x10,
    H2CID_FAST_CHANNEL_SWITCH   = 0x11,
    H2CID_BB_GAIN_REPORT        = 0x12, 
    H2CID_GPIO_CTRL             = 0x13,
    H2CID_HW_INFO               = 0x14,     

    //1 Class2: Power Save
    H2CID_SETPWRMODE            = 0x20,
    H2CID_PSTURNINGPARM         = 0x21,
    H2CID_PSTURNINGPARM2        = 0x22,
    H2CID_PSLPSPARM             = 0x23,
    H2CID_P2PPS_OFFLOAD         = 0x24,
    H2CID_PS_SCAN               = 0x25,
    H2CID_SAPPS                 = 0x26,
    H2CID_INACTIVE_PS           = 0x27,
    H2CID_NOLINK_PS             = 0x28,
    
    //1 Class3: Dynamic Mechaism
    H2CID_MACID_CFG             = 0x40,
    H2CID_TxBF                  = 0x41,
    H2CID_RSSI_SETTING          = 0x42,
    H2CID_AP_REQ_TXRPT          = 0x43,
    H2CID_INIT_RATE_COLLECT     = 0x44,
    H2CID_IQK_OFFLOAD           = 0x45,

    //1 Class4: BT Coex
    H2CID_B_TYPE_TDMA           = 0x60,
    H2CID_BT_INFO               = 0x61,
    H2CID_FORCE_BT_TXPWR        = 0x62,
    H2CID_BT_IGNORE_WLANACT     = 0x63,
    H2CID_DAC_SWING_VALUE       = 0x64,
    H2CID_ANT_SEL_REVERSE       = 0x65,
    H2CID_WL_OPMODE             = 0x66,
    H2CID_BT_MP_OPERATION       = 0x67,
    H2CID_BT_CONTROL            = 0x68,
    H2CID_BT_WIFICTRL           = 0x69,
    H2CID_BT_PATCH_DOWNLOAD     = 0x6A,
    H2CID_BT_SCO_eSCO_OPERATION = 0x6B,     
    H2CID_BT_Page_Scan_Interval = 0x6C,     
    H2CID_WL_Calibraion         = 0x6D,
    H2CID_GNT_BT_CTRL           = 0x6E,
    H2CID_BT_ONLY_TEST          = 0x6F,

    //1 Class5: WOWLAN
    H2CID_WoWLAN                = 0x80,
    H2CID_RemoteWakeCtrl        = 0x81,
    H2CID_AOAC_Global_info      = 0x82,
    H2CID_AOAC_Rsvdpage1        = 0x83,
    H2CID_AOAC_Rsvdpage2        = 0x84,
    H2CID_D0_Scan_offload_info  = 0x85,
    H2CID_D0_Scan_offload_ctrl  = 0x86,
    H2CID_Switch_channel        = 0x87,    
    H2CID_AOAC_Rsvdpage3        = 0x88,
    H2CID_GPIO_WF_Customize     = 0x89,
    H2CID_P2P_RsvdPage          = 0x8A,
    H2CID_P2P_Offload           = 0x8B,

    //1 Class6: LTECOEX
    H2CID_LTECOEX_EN            = 0xA0,
    H2CID_WLAN_High_Priority    = 0xA1,

    //1 Class7: Patch
    H2CID_TSF_RESET             = 0xC0,
    H2CID_BB_NHM                = 0xC1,
    H2CID_BCN_IGNORE_EDCCA      = 0xC2,

    //1 Class8: Testing
    H2CID_H2C2HLB               = 0xE0
    
} H2C_CMD, *PH2C_CMD;

typedef struct _H2CParam_JoinInfo_ {
    BOOLEAN     bConnected:1;
    BOOLEAN     bMacid_ind:1;
    u8          rsvd:6;
    u8          macid;
    u8          macid_end;
}H2CParam_JoinInfo, *PH2CParam_JoinInfo;

typedef struct _H2CParam_RsvdPage_ {
    RSVDPAGE_LOC_8195A    RsvdPageLoc;
    u8                    *ReservedPagePacket;
    u32                   TotalPacketLen;
} H2CParam_RsvdPage, *PH2CParam_RsvdPage;

u32 H2CCmdCommon(PADAPTER padapter, u8 ElementID, u8 *pCmdBuffer);

#endif //__RTL8195A_PMU_CMD_H__