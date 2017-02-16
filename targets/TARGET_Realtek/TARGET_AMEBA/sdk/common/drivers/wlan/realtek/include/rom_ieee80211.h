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
#ifndef __ROM_IEEE80211_H
#define __ROM_IEEE80211_H

extern const u8 RTW_WPA_OUI_TYPE[] ;
extern const u8 WPA_CIPHER_SUITE_NONE[];
extern const u8 WPA_CIPHER_SUITE_WEP40[];
extern const u8 WPA_CIPHER_SUITE_TKIP[];
extern const u8 WPA_CIPHER_SUITE_CCMP[];
extern const u8 WPA_CIPHER_SUITE_WEP104[];
extern const u16 RSN_VERSION_BSD;
extern const u8 RSN_AUTH_KEY_MGMT_UNSPEC_802_1X[];
extern const u8 RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X[];
extern const u8 RSN_CIPHER_SUITE_NONE[];
extern const u8 RSN_CIPHER_SUITE_WEP40[];
extern const u8 RSN_CIPHER_SUITE_TKIP[];
extern const u8 RSN_CIPHER_SUITE_CCMP[];
extern const u8 RSN_CIPHER_SUITE_WEP104[];

/* Parsed Information Elements */
struct rtw_ieee802_11_elems {
	u8 *ssid;
	u8 ssid_len;
	u8 *supp_rates;
	u8 supp_rates_len;
	u8 *fh_params;
	u8 fh_params_len;
	u8 *ds_params;
	u8 ds_params_len;
	u8 *cf_params;
	u8 cf_params_len;
	u8 *tim;
	u8 tim_len;
	u8 *ibss_params;
	u8 ibss_params_len;
	u8 *challenge;
	u8 challenge_len;
	u8 *erp_info;
	u8 erp_info_len;
	u8 *ext_supp_rates;
	u8 ext_supp_rates_len;
	u8 *wpa_ie;
	u8 wpa_ie_len;
	u8 *rsn_ie;
	u8 rsn_ie_len;
	u8 *wme;
	u8 wme_len;
	u8 *wme_tspec;
	u8 wme_tspec_len;
	u8 *wps_ie;
	u8 wps_ie_len;
	u8 *power_cap;
	u8 power_cap_len;
	u8 *supp_channels;
	u8 supp_channels_len;
	u8 *mdie;
	u8 mdie_len;
	u8 *ftie;
	u8 ftie_len;
	u8 *timeout_int;
	u8 timeout_int_len;
	u8 *ht_capabilities;
	u8 ht_capabilities_len;
	u8 *ht_operation;
	u8 ht_operation_len;
	u8 *vendor_ht_cap;
	u8 vendor_ht_cap_len;
};

typedef enum { ParseOK = 0, ParseUnknown = 1, ParseFailed = -1 } ParseRes;

ParseRes rtw_ieee802_11_parse_elems(u8 *start, uint len,
                struct rtw_ieee802_11_elems *elems,
                int show_errors);

u8 *rtw_set_fixed_ie(unsigned char *pbuf, unsigned int len, unsigned char *source, unsigned int *frlen);
u8 *rtw_set_ie(u8 *pbuf, sint index, uint len, u8 *source, uint *frlen);
u8 *rtw_get_ie(u8*pbuf, sint index, u32 *len, sint limit);

void rtw_set_supported_rate(u8* SupportedRates, uint mode) ;

unsigned char *rtw_get_wpa_ie(unsigned char *pie, u32 *wpa_ie_len, int limit);
unsigned char *rtw_get_wpa2_ie(unsigned char *pie, u32 *rsn_ie_len, int limit);
int rtw_get_wpa_cipher_suite(u8 *s);
int rtw_get_wpa2_cipher_suite(u8 *s);

int rtw_parse_wpa_ie(u8* wpa_ie, int wpa_ie_len, int *group_cipher, int *pairwise_cipher, int *is_8021x);
int rtw_parse_wpa2_ie(u8* wpa_ie, int wpa_ie_len, int *group_cipher, int *pairwise_cipher, int *is_8021x);

int rtw_get_sec_ie(u8 *in_ie,uint in_len,u8 *rsn_ie,u16 *rsn_len,u8 *wpa_ie,u16 *wpa_len);

u8 *rtw_get_wps_ie(u8 *in_ie, uint in_len, u8 *wps_ie, uint *wps_ielen);
u8 *rtw_get_wps_attr(u8 *wps_ie, uint wps_ielen, u16 target_attr_id ,u8 *buf_attr, u32 *len_attr);
u8 *rtw_get_wps_attr_content(u8 *wps_ie, uint wps_ielen, u16 target_attr_id ,u8 *buf_content, uint *len_content);

uint rtw_get_rateset_len(u8	*rateset);

int rtw_get_bit_value_from_ieee_value(u8 val);

uint rtw_is_cckrates_included(u8 *rate);

uint rtw_is_cckratesonly_included(u8 *rate);

int rtw_check_network_type(unsigned char *rate, int ratelen, int channel);

u8 key_2char2num(u8 hch, u8 lch);

#endif /* __ROM_IEEE80211_H */

