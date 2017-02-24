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

#ifndef _RTW_PSK_H_
#define _RTW_PSK_H_

#define GMK_LEN					32
#define GTK_LEN					32
#define PMK_LEN					32
#define KEY_NONCE_LEN			32
#define NumGroupKey				4
#define KEY_RC_LEN				8
#define KEY_IV_LEN				16
#define KEY_RSC_LEN				8
#define KEY_ID_LEN				8
#define KEY_MIC_LEN				16
#define KEY_MATERIAL_LEN		2
#define PTK_LEN_EAPOLMIC        16
#define PTK_LEN_EAPOLENC        16
#define PTK_LEN_TKIP           	64
#define PTK_LEN_CCMP            48
#define LIB1X_ETHER_EAPOL_TYPE	0x888E

#define DescTypePos				0
#define KeyInfoPos				1
#define KeyLenPos				3
#define ReplayCounterPos		5
#define KeyNoncePos				13
#define KeyIVPos				45
#define KeyRSCPos				61
#define KeyIDPos				69
#define KeyMICPos				77
#define KeyDataLenPos			93
#define KeyDataPos				95
#define LIB1X_EAPOL_VER				1	//0000 0001B
#define LIB1X_EAPOL_EAPPKT			0       //0000 0000B
#define LIB1X_EAPOL_START			1	//0000 0001B
#define LIB1X_EAPOL_LOGOFF			2       //0000 0010B
#define LIB1X_EAPOL_KEY				3	//0000 0011B
#define LIB1X_EAPOL_ENCASFALERT			4	//0000 0100B


#define A_SHA_DIGEST_LEN		20
#define ETHER_HDRLEN			14
#define LIB1X_EAPOL_HDRLEN		4
#define INFO_ELEMENT_SIZE       128
#define MAX_EAPOLMSG_LEN        512
#define MAX_EAPOLKEYMSG_LEN		(MAX_EAPOLMSG_LEN-(ETHER_HDRLEN+LIB1X_EAPOL_HDRLEN))
#define EAPOLMSG_HDRLEN			95      //EAPOL-key payload length without KeyData
#define WPA_ELEMENT_ID			0xDD
#define WPA2_ELEMENT_ID			0x30

#ifndef TRUE
#define TRUE					1
#endif
#ifndef FALSE
#define FALSE					0
#endif

#define ETHER_ADDRLEN					6
#define PMK_EXPANSION_CONST				"Pairwise key expansion"
#define PMK_EXPANSION_CONST_SIZE		22
#define GMK_EXPANSION_CONST				"Group key expansion"
#define GMK_EXPANSION_CONST_SIZE		19
#define RANDOM_EXPANSION_CONST			"Init Counter"
#define RANDOM_EXPANSION_CONST_SIZE		12

#define	WLAN_REASON_MIC_FAILURE				14
#define WLAN_REASON_4WAY_HANDSHAKE_TIMEOUT	15

/*
	2008-12-16, For Corega CG-WLCB54GL 54Mbps NIC interoperability issue.
	The behavior of this NIC when it connect to the other AP with WPA/TKIP is:
		AP	<----------------------> 	STA
			....................
			------------> Assoc Rsp (ok)
			------------> EAPOL-key (4-way msg 1)
			<------------ unknown TKIP encryption data
			------------> EAPOL-key (4-way msg 1)
			<------------ unknown TKIP encryption data
			.....................
			<------------ disassoc (code=8, STA is leaving) when the 5 seconds timer timeout counting from Assoc_Rsp is got.
			....................
			------------> Assoc Rsp (ok)
			<-----------> EAPOL-key (4-way handshake success)

	If MAX_RESEND_NUM=3, our AP will send disassoc (code=15, 4-way timeout) to STA before STA sending disassoc to AP.
	And this NIC will always can not connect to our AP.
	set MAX_RESEND_NUM=5 can fix this issue.
 */
//#define MAX_RESEND_NUM	3
#define MAX_RESEND_NUM		5
#define RESEND_TIME			1000

#define GK_REKEY_TIME		3600000		//Set rekey period to 1 hour

typedef enum {
	desc_type_RSN	= 2,
	desc_type_WPA	= 254
} DescTypeRSN;

typedef enum {
	type_Group	= 0,
	type_Pairwise	= 1
} KeyType;

typedef enum {
	key_desc_ver1 	= 1,
	key_desc_ver2 	= 2
} KeyDescVer;

enum {
	PSK_WPA		= 1,
	PSK_WPA2	= 2
};

enum {
	PSK_STATE_IDLE,
	PSK_STATE_PTKSTART,
	PSK_STATE_PTKINITNEGOTIATING,
	PSK_STATE_PTKINITDONE,
};

enum {
	PSK_GSTATE_REKEYNEGOTIATING,
	PSK_GSTATE_REKEYESTABLISHED,
	PSK_GSTATE_KEYERROR,
};

typedef	struct _OCTET_STRING {
	unsigned char	*Octet;
	int		Length;
} OCTET_STRING;

typedef union _LARGE_INTEGER {
	unsigned char 	charData[8];
	struct {
		unsigned long 	HighPart;
		unsigned long 	LowPart;
	} field;
} LARGE_INTEGER, *PLARGE_INTEGER;

typedef union _OCTET16_INTEGER {
	unsigned char 	charData[16];
	struct {
		LARGE_INTEGER	HighPart;
		LARGE_INTEGER	LowPart;
	} field;
} OCTET16_INTEGER;

typedef union  _OCTET32_INTEGER {
	unsigned char	charData[32];
	struct {
		OCTET16_INTEGER	HighPart;
		OCTET16_INTEGER	LowPart;
	} field;
} OCTET32_INTEGER;

// group key info
typedef struct _wpa_global_info {
	OCTET32_INTEGER		Counter;
//Save PSK to global array
//	unsigned char		PSK[A_SHA_DIGEST_LEN * 2];
	int					GTKAuthenticator;
	int					GKeyDoneStations;
	int					GInitAKeys;
	int					GUpdateStationKeys;
	int					GkeyReady;
	OCTET_STRING		AuthInfoElement;
	unsigned char		AuthInfoBuf[INFO_ELEMENT_SIZE];
	unsigned char		MulticastCipher;
	OCTET_STRING		GNonce;
	unsigned char		GNonceBuf[KEY_NONCE_LEN];
	unsigned char		GTK[NumGroupKey][GTK_LEN];
	unsigned char		GMK[GMK_LEN];
	int					GN;
	int					GM;
	int					GTKRekey;
#ifdef CONFIG_GK_REKEY
	struct timer_list	GKRekeyTimer;
#endif
} WPA_GLOBAL_INFO;

// wpa sta info
typedef struct _wpa_sta_info {
	int					state;
	int					gstate;
	int					RSNEnabled;		// bit0-WPA, bit1-WPA2
	int					PInitAKeys;
	unsigned char		UnicastCipher;
	LARGE_INTEGER		CurrentReplayCounter;
	LARGE_INTEGER		ReplayCounterStarted; // david+1-12-2007
	OCTET_STRING		ANonce;
	OCTET_STRING		SNonce;
	unsigned char		AnonceBuf[KEY_NONCE_LEN];
	unsigned char		SnonceBuf[KEY_NONCE_LEN];
	unsigned char		PMK[PMK_LEN];
 	unsigned char		PTK[PTK_LEN_TKIP];
	OCTET_STRING		EAPOLMsgRecvd;
	OCTET_STRING		EAPOLMsgSend;
	OCTET_STRING		EapolKeyMsgRecvd;
	OCTET_STRING		EapolKeyMsgSend;
	
	unsigned char		eapSendBuf[MAX_EAPOLMSG_LEN];
//	unsigned char		eapRecvdBuf[MAX_EAPOLMSG_LEN];
	struct timer_list	resendTimer;
	int					resendCnt;
	int					clientHndshkProcessing;
	int					clientHndshkDone;
	int 				clientGkeyUpdate;
	LARGE_INTEGER		clientMICReportReplayCounter;
} WPA_STA_INFO;

typedef struct _LIB1X_EAPOL_KEY
{
 	unsigned char		key_desc_ver;
 	unsigned char		key_info[2];
 	unsigned char		key_len[2];
 	unsigned char		key_replay_counter[KEY_RC_LEN];
 	unsigned char		key_nounce[KEY_NONCE_LEN];
 	unsigned char		key_iv[KEY_IV_LEN];
 	unsigned char		key_rsc[KEY_RSC_LEN];
 	unsigned char		key_id[KEY_ID_LEN];
 	unsigned char		key_mic[KEY_MIC_LEN];
 	unsigned char		key_data_len[KEY_MATERIAL_LEN];
 	unsigned char		*key_data;
} lib1x_eapol_key;

struct lib1x_eapol
{
 	unsigned char		protocol_version;
 	unsigned char		packet_type;			// This makes it odd in number !
 	unsigned short		packet_body_length;
};

struct wlan_ethhdr_t
{
	unsigned char		daddr[WLAN_ETHADDR_LEN];
	unsigned char		saddr[WLAN_ETHADDR_LEN];
	unsigned short		type;
};

typedef enum{
        DOT11_PortStatus_Unauthorized,
        DOT11_PortStatus_Authorized,
        DOT11_PortStatus_Guest
}DOT11_PORT_STATUS;

#ifdef CONFIG_MOVE_PSK_TO_ROM
static __inline__ OCTET_STRING SubStr(OCTET_STRING f, unsigned short s, unsigned short l)
{
	OCTET_STRING res;

	res.Length = l;
	res.Octet = f.Octet + s;

	return res;
}
#endif

#define SetSubStr(f,a,l)					memcpy(f.Octet+l,a.Octet,a.Length)
#define GetKeyInfo0(f, mask) 				((f.Octet[KeyInfoPos + 1] & mask) ? 1 : 0)
#define SetKeyInfo0(f,mask,b)				(f.Octet[KeyInfoPos + 1] = (f.Octet[KeyInfoPos + 1] & ~mask) | ( b?mask:0x0) )
#define GetKeyInfo1(f, mask)				((f.Octet[KeyInfoPos] & mask) ? 1 : 0)
#define SetKeyInfo1(f,mask,b)				(f.Octet[KeyInfoPos] = (f.Octet[KeyInfoPos] & ~mask) | ( b?mask:0x0) )

// EAPOLKey
#define Message_DescType(f)				(f.Octet[DescTypePos])
#define Message_setDescType(f, type)		(f.Octet[DescTypePos] = type)
// Key Information Filed
#define Message_KeyDescVer(f)			(f.Octet[KeyInfoPos+1] & 0x07)
#define Message_setKeyDescVer(f, v)		(f.Octet[KeyInfoPos+1] &= 0xf8) , f.Octet[KeyInfoPos+1] |= (v & 0x07)
#define Message_KeyType(f)				GetKeyInfo0(f, 0x08)
#define Message_setKeyType(f, b)			SetKeyInfo0(f,0x08,b)
#define Message_KeyIndex(f)				((f.Octet[KeyInfoPos+1] & 0x30) >> 4) & 0x03
#define Message_setKeyIndex(f, v)			(f.Octet[KeyInfoPos+1] &= 0xcf), f.Octet[KeyInfoPos+1] |= ((v<<4) & 0x30)
#define Message_setInstall(f, b)			SetKeyInfo0(f,0x40,b)
#define Message_setKeyAck(f, b)			SetKeyInfo0(f,0x80,b)

#define Message_KeyMIC(f)				GetKeyInfo1(f, 0x01)
#define Message_setKeyMIC(f, b)			SetKeyInfo1(f,0x01,b)
#define Message_Secure(f)				GetKeyInfo1(f,0x02)
#define Message_setSecure(f, b)			SetKeyInfo1(f,0x02,b)
#define Message_Error(f)					GetKeyInfo1(f,0x04)
#define Message_setError(f, b)				SetKeyInfo1(f,0x04,b)
#define Message_Request(f)				GetKeyInfo1(f,0x08)
#define Message_setRequest(f, b)			SetKeyInfo1(f,0x08,b)
#define Message_setReserved(f, v)			(f.Octet[KeyInfoPos] |= (v<<4&0xff))
#define Message_KeyLength(f)				((unsigned short)(f.Octet[KeyLenPos] <<8) + (unsigned short)(f.Octet[KeyLenPos+1]))
#define Message_setKeyLength(f, v)		(f.Octet[KeyLenPos] = (v&0xff00) >>8 ,  f.Octet[KeyLenPos+1] = (v&0x00ff))

#define Message_KeyNonce(f)				SubStr(f, KeyNoncePos, KEY_NONCE_LEN)
#define Message_setKeyNonce(f, v)			SetSubStr(f, v, KeyNoncePos)
#define Message_EqualKeyNonce(f1, f2)		memcmp(f1.Octet + KeyNoncePos, f2.Octet, KEY_NONCE_LEN)? 0:1
#define Message_setKeyIV(f, v)			SetSubStr(f, v, KeyIVPos)
#define Message_setKeyRSC(f, v)			SetSubStr(f, v, KeyRSCPos)
#define Message_setKeyID(f, v)			SetSubStr(f, v, KeyIDPos)
#define Message_setMIC(f, v)				SetSubStr(f, v, KeyMICPos)
#define Message_KeyDataLength(f)			((unsigned short)(f.Octet[KeyDataLenPos] <<8) + (unsigned short)(f.Octet[KeyDataLenPos+1]))
#define Message_setKeyDataLength(f, v)	(f.Octet[KeyDataLenPos] = (v&0xff00) >>8 ,  f.Octet[KeyDataLenPos+1] = (v&0x00ff))
#define Message_setKeyData(f, v)			SetSubStr(f, v, KeyDataPos);

#define Message_CopyReplayCounter(f1, f2)	memcpy(f1.Octet + ReplayCounterPos, f2.Octet + ReplayCounterPos, KEY_RC_LEN)
#define Message_DefaultReplayCounter(li)	(((li.field.HighPart == 0xffffffff) && (li.field.LowPart == 0xffffffff) ) ?1:0)

#define GET_MY_HWADDR(padapter)		((padapter)->eeprompriv.mac_addr)
#define LargeIntegerOverflow(x)			(x.field.HighPart == 0xffffffff) && (x.field.LowPart == 0xffffffff)
#define LargeIntegerZero(x)				memset(&x.charData, 0, 8)
#define Octet16IntegerOverflow(x)			LargeIntegerOverflow(x.field.HighPart) && LargeIntegerOverflow(x.field.LowPart)
#define Octet16IntegerZero(x)				memset(&x.charData, 0, 16)
#define SetNonce(ocDst, oc32Counter)		SetEAPOL_KEYIV(ocDst, oc32Counter)

void ClientSendEAPOL(_adapter *padapter, struct sta_info *psta, int resend);
void SendEAPOL(_adapter *padapter, struct sta_info *psta, int resend);
void EAPOLKeyRecvd(_adapter *padapter, struct sta_info *psta);
void ClientEAPOLKeyRecvd(_adapter *padapter, struct sta_info *psta);
void init_wpa_sta_info(_adapter *padapter, struct sta_info *psta);
void psk_init(_adapter *padapter, unsigned char *pie, unsigned short ielen);
void psk_derive(_adapter *padapter, unsigned char *passphrase, unsigned char *ssid);
u16 psk_strip_rsn_pairwise(u8 *ie, u16 ie_len);
u16 psk_strip_wpa_pairwise(u8 *ie, u16 ie_len);

#endif // _RTW_PSK_H_

