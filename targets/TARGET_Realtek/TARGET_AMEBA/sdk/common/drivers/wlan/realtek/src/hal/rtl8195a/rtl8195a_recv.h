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
#ifndef __RTL8195A_RECV_H__
#define __RTL8195A_RECV_H__

#ifdef CONFIG_WLAN_HAL_TEST
#define MAX_RECVBUF_SZ	1200//(RX_DMA_SIZE_8195A - RX_DMA_RESERVED_SIZE_8195A)
#else
#if (SKB_PRE_ALLOCATE_RX==1)
#define MAX_RECVBUF_SZ	MAX_SKB_BUF_SIZE //1650 //(RX_DMA_SIZE_8195A - RX_DMA_RESERVED_SIZE_8195A)
#else
#define MAX_RECVBUF_SZ	(HAL_INTERFACE_OVERHEAD_SKB_DATA+RX_DRIVER_INFO+\
						RXDESC_SIZE +\
						(MAX_RX_PKT_LIMIT * 512) +\
						SKB_RESERVED_FOR_SAFETY)		// 0+32+24+512*4+0 = 2104
#endif
#endif

//DWORD 0
#define SET_RX_STATUS_DESC_PKT_LEN_8812(__pRxStatusDesc, __Value)		SET_BITS_TO_LE_4BYTE( __pRxStatusDesc, 0, 14, __Value)
#define SET_RX_STATUS_DESC_EOR_8812(__pRxStatusDesc, __Value)		SET_BITS_TO_LE_4BYTE( __pRxStatusDesc, 30, 1, __Value)
#define SET_RX_STATUS_DESC_OWN_8812(__pRxStatusDesc, __Value)		SET_BITS_TO_LE_4BYTE( __pRxStatusDesc, 31, 1, __Value)

#define GET_RX_STATUS_DESC_PKT_LEN_8812(__pRxStatusDesc)			LE_BITS_TO_4BYTE( __pRxStatusDesc, 0, 14)
#define GET_RX_STATUS_DESC_CRC32_8812(__pRxStatusDesc)			LE_BITS_TO_4BYTE( __pRxStatusDesc, 14, 1)
#define GET_RX_STATUS_DESC_ICV_8812(__pRxStatusDesc)				LE_BITS_TO_4BYTE( __pRxStatusDesc, 15, 1)
#define GET_RX_STATUS_DESC_DRVINFO_SIZE_8812(__pRxStatusDesc)		LE_BITS_TO_4BYTE( __pRxStatusDesc, 16, 4)
#define GET_RX_STATUS_DESC_SECURITY_8812(__pRxStatusDesc)			LE_BITS_TO_4BYTE( __pRxStatusDesc, 20, 3)
#define GET_RX_STATUS_DESC_QOS_8812(__pRxStatusDesc)				LE_BITS_TO_4BYTE( __pRxStatusDesc, 23, 1)
#define GET_RX_STATUS_DESC_SHIFT_8812(__pRxStatusDesc)			LE_BITS_TO_4BYTE( __pRxStatusDesc, 24, 2)
#define GET_RX_STATUS_DESC_PHY_STATUS_8812(__pRxStatusDesc)			LE_BITS_TO_4BYTE( __pRxStatusDesc, 26, 1)
#define GET_RX_STATUS_DESC_SWDEC_8812(__pRxStatusDesc)			LE_BITS_TO_4BYTE( __pRxStatusDesc, 27, 1)
#define GET_RX_STATUS_DESC_LAST_SEG_8812(__pRxStatusDesc)			LE_BITS_TO_4BYTE( __pRxStatusDesc, 28, 1)
#define GET_RX_STATUS_DESC_FIRST_SEG_8812(__pRxStatusDesc)			LE_BITS_TO_4BYTE( __pRxStatusDesc, 29, 1)
#define GET_RX_STATUS_DESC_EOR_8812(__pRxStatusDesc)				LE_BITS_TO_4BYTE( __pRxStatusDesc, 30, 1)
#define GET_RX_STATUS_DESC_OWN_8812(__pRxStatusDesc)				LE_BITS_TO_4BYTE( __pRxStatusDesc, 31, 1)

//DWORD 1
#define GET_RX_STATUS_DESC_MACID_8812(__pRxDesc) 					LE_BITS_TO_4BYTE(__pRxDesc+4, 0, 7)
#define GET_RX_STATUS_DESC_TID_8812(__pRxDesc) 					LE_BITS_TO_4BYTE(__pRxDesc+4, 8, 4)
#define GET_RX_STATUS_DESC_AMSDU_8812(__pRxDesc) 					LE_BITS_TO_4BYTE(__pRxDesc+4, 13, 1)
#define GET_RX_STATUS_DESC_RXID_MATCH_8812(__pRxDesc)		LE_BITS_TO_4BYTE( __pRxDesc+4, 14, 1)
#define GET_RX_STATUS_DESC_PAGGR_8812(__pRxDesc)				LE_BITS_TO_4BYTE( __pRxDesc+4, 15, 1)
#define GET_RX_STATUS_DESC_A1_FIT_8812(__pRxDesc)				LE_BITS_TO_4BYTE( __pRxDesc+4, 16, 4)
#define GET_RX_STATUS_DESC_CHKERR_8812(__pRxDesc)				LE_BITS_TO_4BYTE( __pRxDesc+4, 20, 1)
#define GET_RX_STATUS_DESC_IPVER_8812(__pRxDesc)			LE_BITS_TO_4BYTE(__pRxDesc+4, 21, 1)
#define GET_RX_STATUS_DESC_IS_TCPUDP__8812(__pRxDesc)		LE_BITS_TO_4BYTE(__pRxDesc+4, 22, 1)
#define GET_RX_STATUS_DESC_CHK_VLD_8812(__pRxDesc)		LE_BITS_TO_4BYTE(__pRxDesc+4, 23, 1)
#define GET_RX_STATUS_DESC_PAM_8812(__pRxDesc)				LE_BITS_TO_4BYTE( __pRxDesc+4, 24, 1)
#define GET_RX_STATUS_DESC_PWR_8812(__pRxDesc)				LE_BITS_TO_4BYTE( __pRxDesc+4, 25, 1)
#define GET_RX_STATUS_DESC_MORE_DATA_8812(__pRxDesc)			LE_BITS_TO_4BYTE( __pRxDesc+4, 26, 1)
#define GET_RX_STATUS_DESC_MORE_FRAG_8812(__pRxDesc)			LE_BITS_TO_4BYTE( __pRxDesc+4, 27, 1)
#define GET_RX_STATUS_DESC_TYPE_8812(__pRxDesc)			LE_BITS_TO_4BYTE( __pRxDesc+4, 28, 2)
#define GET_RX_STATUS_DESC_MC_8812(__pRxDesc)				LE_BITS_TO_4BYTE( __pRxDesc+4, 30, 1)
#define GET_RX_STATUS_DESC_BC_8812(__pRxDesc)				LE_BITS_TO_4BYTE( __pRxDesc+4, 31, 1)

//DWORD 2
#define GET_RX_STATUS_DESC_SEQ_8812(__pRxStatusDesc)					LE_BITS_TO_4BYTE( __pRxStatusDesc+8, 0, 12)
#define GET_RX_STATUS_DESC_FRAG_8812(__pRxStatusDesc)				LE_BITS_TO_4BYTE( __pRxStatusDesc+8, 12, 4)
#define GET_RX_STATUS_DESC_RX_IS_QOS_8812(__pRxStatusDesc)			LE_BITS_TO_4BYTE( __pRxStatusDesc+8, 16, 1)
#define GET_RX_STATUS_DESC_WLANHD_IV_LEN_8812(__pRxStatusDesc)		LE_BITS_TO_4BYTE( __pRxStatusDesc+8, 18, 6)
#define GET_RX_STATUS_DESC_RPT_SEL_8812(__pRxStatusDesc)			LE_BITS_TO_4BYTE( __pRxStatusDesc+8, 28, 1)

//DWORD 3
#define GET_RX_STATUS_DESC_RX_RATE_8812(__pRxStatusDesc)				LE_BITS_TO_4BYTE( __pRxStatusDesc+12, 0, 7)
#define GET_RX_STATUS_DESC_HTC_8812(__pRxStatusDesc)					LE_BITS_TO_4BYTE( __pRxStatusDesc+12, 10, 1)
#define GET_RX_STATUS_DESC_EOSP_8812(__pRxStatusDesc)					LE_BITS_TO_4BYTE( __pRxStatusDesc+12, 11, 1)
#define GET_RX_STATUS_DESC_BSSID_FIT_8812(__pRxStatusDesc)			LE_BITS_TO_4BYTE( __pRxStatusDesc+12, 12, 2)
#ifdef CONFIG_USB_RX_AGGREGATION
#define GET_RX_STATUS_DESC_USB_AGG_PKTNUM_8812(__pRxStatusDesc)	LE_BITS_TO_4BYTE( __pRxStatusDesc+12, 16, 8)
#endif
#define GET_RX_STATUS_DESC_PATTERN_MATCH_8812(__pRxDesc)			LE_BITS_TO_4BYTE( __pRxDesc+12, 29, 1)
#define GET_RX_STATUS_DESC_UNICAST_MATCH_8812(__pRxDesc)			LE_BITS_TO_4BYTE( __pRxDesc+12, 30, 1)
#define GET_RX_STATUS_DESC_MAGIC_MATCH_8812(__pRxDesc)			LE_BITS_TO_4BYTE( __pRxDesc+12, 31, 1)

//DWORD 6
#define GET_RX_STATUS_DESC_SPLCP_8812(__pRxDesc)			LE_BITS_TO_4BYTE( __pRxDesc+16, 0, 1)
#define GET_RX_STATUS_DESC_LDPC_8812(__pRxDesc)			LE_BITS_TO_4BYTE( __pRxDesc+16, 1, 1)
#define GET_RX_STATUS_DESC_STBC_8812(__pRxDesc)			LE_BITS_TO_4BYTE( __pRxDesc+16, 2, 1)
#define GET_RX_STATUS_DESC_BW_8812(__pRxDesc)			LE_BITS_TO_4BYTE( __pRxDesc+16, 4, 2)

//DWORD 5
#define GET_RX_STATUS_DESC_TSFL_8812(__pRxStatusDesc)				LE_BITS_TO_4BYTE( __pRxStatusDesc+20, 0, 32)

#define GET_RX_STATUS_DESC_BUFF_ADDR_8812(__pRxDesc) 		LE_BITS_TO_4BYTE(__pRxDesc+24, 0, 32)
#define GET_RX_STATUS_DESC_BUFF_ADDR64_8812(__pRxDesc) 		LE_BITS_TO_4BYTE(__pRxDesc+28, 0, 32)

#define SET_RX_STATUS_DESC_BUFF_ADDR_8812(__pRxDesc, __Value) 	SET_BITS_TO_LE_4BYTE(__pRxDesc+24, 0, 32, __Value)

typedef struct rxreport_8723b
{
	//DWORD 0
	u32 pktlen:14;
	u32 crc32:1;
	u32 icverr:1;
	u32 drvinfosize:4;
	u32 security:3;
	u32 qos:1;
	u32 shift:2;
	u32 physt:1;
	u32 swdec:1;
	u32 rsvd0028:2;
	u32 eor:1;
	u32 rsvd0031:1;

	//DWORD 1
	u32 macid:7;
	u32 rsvd0407:1;
	u32 tid:4;
	u32 macid_vld:1;
	u32 amsdu:1;
	u32 rxid_match:1;
	u32 paggr:1;
	u32 a1fit:4;
	u32 chkerr:1;  //20
	u32 rx_ipv:1;
	u32 rx_is_tcp_udp:1;
	u32 chk_vld:1;   //23
	u32 pam:1;
	u32 pwr:1;
	u32 md:1;
	u32 mf:1;
	u32 type:2;
	u32 mc:1;
	u32 bc:1;

	//DWORD 2
	u32 seq:12;
	u32 frag:4;
	u32 rx_is_qos:1;
	u32 rsvd0817:1;
	u32 wlanhd_iv_len:6;
	u32 hwrsvd0824:4;
	u32 c2h_ind:1;
	u32 rsvd0829:2;
	u32 fcs_ok:1;

	//DWORD 3
	u32 rx_rate:7;
	u32 rsvd1207:3;
	u32 htc:1;
	u32 esop:1;
	u32 bssid_fit:2;
	u32 rsvd1214:2;
	u32 dma_agg_num:8;
	u32 rsvd1224:5;
	u32 patternmatch:1;
	u32 unicastwake:1;
	u32 magicwake:1;
	
	//DWORD 4
	u32 splcp:1;	//Ofdm sgi or cck_splcp
	u32 ldpc:1;
	u32 stbc:1;
	u32 not_sounding:1;
	u32 bw:2;
	u32 rsvd1606:26;

	//DWORD 5
	u32 tsfl;
} RXREPORT, *PRXREPORT;

typedef struct phystatus_8723b
{
	u32 rxgain_a:7;
	u32 trsw_a:1;
	u32 rxgain_b:7;
	u32 trsw_b:1;
	u32 chcorr_l:16;

	u32 sigqualcck:8;
	u32 cfo_a:8;
	u32 cfo_b:8;
	u32 chcorr_h:8;

	u32 noisepwrdb_h:8;
	u32 cfo_tail_a:8;
	u32 cfo_tail_b:8;
	u32 rsvd0824:8;

	u32 rsvd1200:8;
	u32 rxevm_a:8;
	u32 rxevm_b:8;
	u32 rxsnr_a:8;

	u32 rxsnr_b:8;
	u32 noisepwrdb_l:8;
	u32 rsvd1616:8;
	u32 postsnr_a:8;

	u32 postsnr_b:8;
	u32 csi_a:8;
	u32 csi_b:8;
	u32 targetcsi_a:8;

	u32 targetcsi_b:8;
	u32 sigevm:8;
	u32 maxexpwr:8;
	u32 exintflag:1;
	u32 sgien:1;
	u32 rxsc:2;
	u32 idlelong:1;
	u32 anttrainen:1;
	u32 antselb:1;
	u32 antsel:1;
} PHYSTATUS, *PPHYSTATUS;


s32 rtl8195a_init_recv_priv(PADAPTER padapter);
void rtl8195a_free_recv_priv(PADAPTER padapter);
void rtl8195a_query_rx_desc_status(union recv_frame *precvframe, u8 *pdesc);
void rtl8195a_query_rx_phy_status(union recv_frame	*precvframe, u8 *pphy_status);

#endif

