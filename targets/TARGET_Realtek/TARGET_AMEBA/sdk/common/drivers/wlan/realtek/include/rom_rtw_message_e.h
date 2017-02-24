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
// Debug message
// DBG_PWR_INDEX
ROM_E_RTW_MSGPOOL(PWR_INDEX_1,	"BandWidth = %d, Rate = %d, Channel = %d\n\r")
ROM_E_RTW_MSGPOOL(PWR_INDEX_2,	"Base = %d, DiffByRate = %d, limit = %d, DiffByTrack = %d, Tx Power = %d\n\n\r")
// DBG_RX_INFO
ROM_E_RTW_MSGPOOL(RX_INFO_1,	"============ Rx Info dump ===================\n")
ROM_E_RTW_MSGPOOL(RX_INFO_2,	"bLinked = %d, RSSI_Min = %d(%%), CurrentIGI = 0x%x\n")
ROM_E_RTW_MSGPOOL(RX_INFO_3,	"Cnt_Cck_fail = %d, Cnt_Ofdm_fail = %d, Total False Alarm = %d\n")
ROM_E_RTW_MSGPOOL(RX_INFO_4,	"RxRate = 0x%x, RSSI_A = %d(%%), RSSI_B = %d(%%)\n")
// DBG_TX_RATE
ROM_E_RTW_MSGPOOL(TX_RATE_1,	"Rate: 0x%x\n\r")
ROM_E_RTW_MSGPOOL(TX_RATE_2,	"%s(): mac_id=%d raid=0x%x bw=%d mask=0x%x init_rate=0x%x\n")
// DBG_DM_RA
ROM_E_RTW_MSGPOOL(DM_RA_1,	"==> ReadRateMask = 0x%x RAMASK[%d] = 0x%x\n")
ROM_E_RTW_MSGPOOL(DM_RA_2,	"==> TMP_rate = %x highest_rate = 0x%02X, lowest_rate = 0x%02X\n")
ROM_E_RTW_MSGPOOL(DM_RA_3,	"==> MacID = %d rateid = 0x%x sgi = %d bw_idx = %d\n\r")
ROM_E_RTW_MSGPOOL(DM_RA_4,	"%s(): mac_id=%d raid=0x%x bw=%d mask=0x%x\r\n")
// DBG_DM_DIG
ROM_E_RTW_MSGPOOL(DM_DIG_1,	"CurrentIGI(0x%02x)\n\n")
// DBG_PWR_TRACKING
ROM_E_RTW_MSGPOOL(PWR_TRACKING_1,	"Thermal = 0x%02X\r\n")
ROM_E_RTW_MSGPOOL(PWR_TRACKING_2,	"delta = %d, AVG Thermal = 0x%02X, EFUSE = 0x%02X, PackageType = 0x%02X\r\n")
ROM_E_RTW_MSGPOOL(PWR_TRACKING_3,	"Channel = %d, CCK PwrBase = 0x%02X, HT40M PwrBase = 0x%02X, OFDMdiff = %d, 20Mdiff = %d \n\r")
ROM_E_RTW_MSGPOOL(PWR_TRACKING_4,	"Remnant_CCKSwingIdx  = %d\n\r")
ROM_E_RTW_MSGPOOL(PWR_TRACKING_5,	"Remnant_OFDMSwingIdx = %d\n\r")
ROM_E_RTW_MSGPOOL(PWR_TRACKING_6,	"CCK2~11: 0x86c = 0x%08X\r\n")
ROM_E_RTW_MSGPOOL(PWR_TRACKING_7,	"MCS7~4 : 0xe14 = 0x%08X\r\n")
// DBG_RF_IQK
ROM_E_RTW_MSGPOOL(RF_IQK_1,	"Path A Tx IQK Success!\n")
ROM_E_RTW_MSGPOOL(RF_IQK_2,	"Path A Rx IQK Success!\n")
ROM_E_RTW_MSGPOOL(RF_IQK_3,	"Path A IQK failed!\n")
ROM_E_RTW_MSGPOOL(RF_IQK_4,	"IQK finished\n")
ROM_E_RTW_MSGPOOL(RF_IQK_5,	"LCK finished\n")
// DBG_DM_ADAPTIVITY
ROM_E_RTW_MSGPOOL(DM_ADAPTIVITY_1, "IGI_Base=0x%x, TH_L2H_ini = %d, TH_EDCCA_HL_diff = %d\n")
ROM_E_RTW_MSGPOOL(DM_ADAPTIVITY_2, "DynamicLinkAdaptivity = %d, Adaptivity_enable = %d\n")
ROM_E_RTW_MSGPOOL(DM_ADAPTIVITY_3, "IGI=0x%x, TH_L2H_dmc = 0x%x, TH_H2L_dmc = 0x%x\n\n")

// freertos_ioctl.c
// mac_reg_dump, bb_reg_dump, rf_reg_dump
ROM_E_RTW_MSGPOOL(MAC_REG_DUMP_1,	"\n======= MAC REG =======\n")
ROM_E_RTW_MSGPOOL(BB_REG_DUMP_1,	"\n======= BB REG =======\n")
ROM_E_RTW_MSGPOOL(RF_REG_DUMP_1,	"\n======= RF REG =======\n")
ROM_E_RTW_MSGPOOL(RF_REG_DUMP_2,	"\nRF_Path(%x)\n")
ROM_E_RTW_MSGPOOL(REG_DUMP_1,	"0x%02x ")
ROM_E_RTW_MSGPOOL(REG_DUMP_2,	" 0x%08x ")
ROM_E_RTW_MSGPOOL(REG_DUMP_3,	"\n")
// 0x70 read reg
ROM_E_RTW_MSGPOOL(READ_REG_1,	"rtw_read8(0x%x)=0x%02x\n")
ROM_E_RTW_MSGPOOL(READ_REG_2,	"rtw_read16(0x%x)=0x%04x\n")
ROM_E_RTW_MSGPOOL(READ_REG_3,	"rtw_read32(0x%x)=0x%08x\n")
// 0x71 write reg
ROM_E_RTW_MSGPOOL(WRITE_REG_1,	"rtw_write8(0x%x)=0x%02x\n")
ROM_E_RTW_MSGPOOL(WRITE_REG_2,	"rtw_write16(0x%x)=0x%04x\n")
ROM_E_RTW_MSGPOOL(WRITE_REG_3,	"rtw_write32(0x%x)=0x%08x\n")
// 0x72 read bb
ROM_E_RTW_MSGPOOL(READ_BB_1,	"read_bbreg(0x%x)=0x%x\n")
// 0x73 write bb
ROM_E_RTW_MSGPOOL(WRITE_BB_1,	"write_bbreg(0x%x)=0x%x\n")
// 0x74 read rf
ROM_E_RTW_MSGPOOL(READ_RF_1,	"read RF_reg path(0x%02x),offset(0x%x),value(0x%08x)\n")
// 0x75 write rf
ROM_E_RTW_MSGPOOL(WRITE_RF_1,	"write RF_reg path(0x%02x),offset(0x%x),value(0x%08x)\n")
// 0x17 fix channel
ROM_E_RTW_MSGPOOL(FIX_CHANNEL_1,	"=>Fixed channel to %d\n")
ROM_E_RTW_MSGPOOL(FIX_CHANNEL_2,	"Invalid channel number(%d)\n")
// 0x22 enable / disable power saving mode
ROM_E_RTW_MSGPOOL(PWR_SAVE_MODE_1,	"wlan power saving mode = %s\n")
// 0xaa fix rate
ROM_E_RTW_MSGPOOL(FIX_RATE_1,	"chang data rate to :0x%02x\n")
// 0xc0 get odm dbg flag
ROM_E_RTW_MSGPOOL(GET_ODM_DBG_FLAG_1,	"get odm dbg flag : 0x%08x\n")
// 0xc1 set odm dbg flag
ROM_E_RTW_MSGPOOL(SET_ODM_DBG_FLAG_1,	"set odm dbg flag : 0x%08x\n")
// 0xcc open power index debug message (power by rate, power limit, power tracking)
ROM_E_RTW_MSGPOOL(DUMP_PWR_IDX_1,	"Fixed rate = %d\n")
// 0xdd dump info
ROM_E_RTW_MSGPOOL(DUMP_INFO_1,	"Tx power:\n")
ROM_E_RTW_MSGPOOL(DUMP_INFO_2,	"CCK 1(0xe08)= 0x%x\n")
ROM_E_RTW_MSGPOOL(DUMP_INFO_3,	"CCK 11~2(0x86c)= 0x%x\n")
ROM_E_RTW_MSGPOOL(DUMP_INFO_4,	"OFDM 18~6(0xe00)= 0x%x\n")
ROM_E_RTW_MSGPOOL(DUMP_INFO_5,	"OFDM 54~24(0xe04)= 0x%x\n")
ROM_E_RTW_MSGPOOL(DUMP_INFO_6,	"MCS 3~0(0xe10)= 0x%x\n")
ROM_E_RTW_MSGPOOL(DUMP_INFO_7,	"MCS 7~4(0xe14)= 0x%x\n")
ROM_E_RTW_MSGPOOL(DUMP_INFO_8,	"Country code: 0x%x\n")
// 0xee turn on/off dynamic funcs
ROM_E_RTW_MSGPOOL(DM_FUNC_FLAG_1,	" === DMFlag(0x%08x) === \n")
ROM_E_RTW_MSGPOOL(DM_FUNC_FLAG_2,	"extra_arg = 0 - disable all dynamic func\n")
ROM_E_RTW_MSGPOOL(DM_FUNC_FLAG_3,	"extra_arg = 1 - enable all dynamic func\n")
ROM_E_RTW_MSGPOOL(DM_FUNC_FLAG_4,	"extra_arg = 2 - disable DIG\n")
ROM_E_RTW_MSGPOOL(DM_FUNC_FLAG_5,	"extra_arg = 3 - enable DIG\n")
ROM_E_RTW_MSGPOOL(DM_FUNC_FLAG_6,	"extra_arg = 4 - disable tx power tracking\n")
ROM_E_RTW_MSGPOOL(DM_FUNC_FLAG_7,	"extra_arg = 5 - enable tx power tracking\n")
ROM_E_RTW_MSGPOOL(DM_FUNC_FLAG_8,	"extra_arg = 6 - disable adaptivity\n")
ROM_E_RTW_MSGPOOL(DM_FUNC_FLAG_9,	"extra_arg = 7 - enable adaptivity\n")

// lxbus_ops.c
ROM_E_RTW_MSGPOOL(RX_MPDU_1,	"Drop packet! crc_err = %d, icv_err = %d, rx_pkt_len = %d, skb_pkt_len = %d\n")

// wlan driver DBG_871X_LEVEL
#define ROM_E_RTW_MSGPOOL_871X(name,str) ROM_E_RTW_MSGPOOL(name,DRIVER_PREFIX str)
// rtw_ap.c
ROM_E_RTW_MSGPOOL_871X(AP_TIMEOUT_CHK_1,	"Asoc expire "MAC_FMT"\n")
// rtw_intfs.c
ROM_E_RTW_MSGPOOL_871X(INIT_DRV_SW_1,	"The driver is for MP\n")
// rtw_ioctl_set.c
ROM_E_RTW_MSGPOOL_871X(SET_BSSID_1,	"set BSSID: %02x:%02x:%02x:%02x:%02x:%02x\n")
ROM_E_RTW_MSGPOOL_871X(SET_SSID_1,	"set ssid [%s] \n")
// rtw_mlme_ext.c
ROM_E_RTW_MSGPOOL_871X(ON_BEACON_1,	"ap has changed, disconnect now\n ")
ROM_E_RTW_MSGPOOL_871X(ON_AUTH_1,	"+OnAuth: "MAC_FMT"\n")
ROM_E_RTW_MSGPOOL_871X(ON_AUTH_2,	" Exceed the upper limit(%d) of supported clients...\n")
ROM_E_RTW_MSGPOOL_871X(ON_AUTH_CLIENT_1,	"auth success, start assoc\n")
ROM_E_RTW_MSGPOOL_871X(ON_ASSOC_REQ_1,	"+OnAssocReq\n")
ROM_E_RTW_MSGPOOL_871X(ON_ASSOC_RSP_1,	"association success(res=%d)\n")
ROM_E_RTW_MSGPOOL_871X(ON_DE_AUTH_1,	"ap recv deauth reason code(%d) sta:"MAC_FMT"\n")
ROM_E_RTW_MSGPOOL_871X(ON_DE_AUTH_2,	"sta recv deauth reason code(%d) sta:"MAC_FMT"\n")
ROM_E_RTW_MSGPOOL_871X(ON_DISASSOC_1,	"ap recv disassoc reason code(%d) sta:"MAC_FMT"\n")
ROM_E_RTW_MSGPOOL_871X(ON_DISASSOC_2,	"sta recv disassoc reason code(%d) sta:"MAC_FMT"\n")
ROM_E_RTW_MSGPOOL_871X(ISSUE_BEACON_1,	"beacon frame too large\n")
ROM_E_RTW_MSGPOOL_871X(ISSUE_PROBERSP_1,	"probersp frame too large\n")
ROM_E_RTW_MSGPOOL_871X(ISSUE_PROBEREQ_1,	"probereq frame too large\n")
ROM_E_RTW_MSGPOOL_871X(ISSUE_AUTH_1,	"auth frame too large\n")
ROM_E_RTW_MSGPOOL_871X(ISSUE_ASSOCRSP_1,	"assocrsp frame too large\n")
ROM_E_RTW_MSGPOOL_871X(ISSUE_ASSOCREQ_1,	"assocreq frame too large\n")
ROM_E_RTW_MSGPOOL_871X(ISSUE_NULLDATA_1,	"nulldata frame too large\n")
ROM_E_RTW_MSGPOOL_871X(ISSUE_QOS_NULLDATA_1,	"qos nulldata frame too large\n")
ROM_E_RTW_MSGPOOL_871X(ISSUE_DEAUTH_1,	"deauth frame too large\n")
ROM_E_RTW_MSGPOOL_871X(ISSUE_ACTION_BA_1,	"action BA frame too large\n")
ROM_E_RTW_MSGPOOL_871X(ISSUE_BSS_COEXIST_1,	"action BSSCoexist frame too large\n")
ROM_E_RTW_MSGPOOL_871X(START_CLNT_AUTH_1,	"start auth to %02x:%02x:%02x:%02x:%02x:%02x\n")
ROM_E_RTW_MSGPOOL_871X(LINKED_STATUS_CHK_1,	"no beacon for a long time, disconnect or roaming\n")
ROM_E_RTW_MSGPOOL_871X(SETKEY_HDL_1,	"set group key to hw: alg:%d(WEP40-1 WEP104-5 TKIP-2 AES-4) keyid:%d\n")
ROM_E_RTW_MSGPOOL_871X(SET_STAKEY_HDL_1,	"set pairwise key to hw: alg:%d(WEP40-1 WEP104-5 TKIP-2 AES-4)\n")
ROM_E_RTW_MSGPOOL_871X(SET_STAKEY_HDL_2,	"set pairwise key to hw: alg:%d(WEP40-1 WEP104-5 TKIP-2 AES-4) for %x:%x:%x:%x:%x:%x\n")
// rtw_p2p.c
ROM_E_RTW_MSGPOOL_871X(P2P_BUILD_MGNT_FRAME_1,	"p2p mgnt frame too large\n")
// rtw_psk.c
ROM_E_RTW_MSGPOOL_871X(SEND_EAPOL_1,	"ap mode 4-1\n")
ROM_E_RTW_MSGPOOL_871X(SEND_EAPOL_2,	"ap mode 4-3\n")
ROM_E_RTW_MSGPOOL_871X(SEND_EAPOL_3,	"ap mode 2-1 to WPA_STA(%d)\n")
ROM_E_RTW_MSGPOOL_871X(EAPOL_KEY_RECVD_1,	"ap mode 4-2\n")
ROM_E_RTW_MSGPOOL_871X(EAPOL_KEY_RECVD_2,	"ap mode 4-4\n")
ROM_E_RTW_MSGPOOL_871X(EAPOL_KEY_RECVD_3,	"ap mode 2-2 from WPA_STA(%d)\n")
// rtw_recv.c
ROM_E_RTW_MSGPOOL_871X(FREE_RECVFRAME_1,	"%s free_recvframe_cnt:%d > %d refree happen !!!!\n")
// hal_com.c
ROM_E_RTW_MSGPOOL_871X(VAR_PORT_SWITCH_1,	"port switch - port0("ADPT_FMT"), port1("ADPT_FMT")\n")
ROM_E_RTW_MSGPOOL_871X(VAR_PORT_SWITCH_2,	"port switch - port0("ADPT_FMT"), port1("ADPT_FMT")\n")
// osdep_service.c
ROM_E_RTW_MSGPOOL_871X(DOWN_SEMA_1,	"%s(%p) failed, retry\n")


#undef ROM_E_RTW_MSGPOOL
#undef ROM_E_RTW_MSGPOOL_871X
