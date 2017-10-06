/******************************************************************************
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
 *
 ******************************************************************************/
#include "tcpip.h"
#include "wifi/wifi_conf.h"

#ifndef CONFIG_WLAN
#define CONFIG_WLAN 1
#endif

#if CONFIG_WLAN
#include <platform/platform_stdlib.h>

// Add extra interfaces to make release sdk able to determine promisc API linking
void promisc_deinit(void *padapter)
{
#ifdef CONFIG_PROMISC
	_promisc_deinit(padapter);
#endif
}

int promisc_recv_func(void *padapter, void *rframe)
{
	// Never reach here if not define CONFIG_PROMISC
#ifdef CONFIG_PROMISC
	return _promisc_recv_func(padapter, rframe);
#else
	return 0;
#endif
}

int promisc_set(rtw_rcr_level_t enabled, void (*callback)(unsigned char*, unsigned int, void*), unsigned char len_used)
{
#ifdef CONFIG_PROMISC
	return _promisc_set(enabled, callback, len_used);
#else
	return -1;
#endif
}

unsigned char is_promisc_enabled(void)
{
#ifdef CONFIG_PROMISC
	return _is_promisc_enabled();
#else
	return 0;
#endif
}

int promisc_get_fixed_channel(void *fixed_bssid, u8 *ssid, int *ssid_length)
{
#ifdef CONFIG_PROMISC
	return _promisc_get_fixed_channel(fixed_bssid, ssid, ssid_length);
#else
	return 0;
#endif
}
// End of Add extra interfaces

struct eth_frame {
	struct eth_frame *prev;
	struct eth_frame *next;
	unsigned char da[6];
	unsigned char sa[6];
	unsigned int len;
	unsigned char type;
	signed char rssi;
};

#if CONFIG_INIC_CMD_RSP
#if defined(__IAR_SYSTEMS_ICC__)
#pragma pack(1)
#endif
struct inic_eth_frame {
	unsigned char da[6];
	unsigned char sa[6];
	unsigned int len;
	unsigned char type;
};
#if defined(__IAR_SYSTEMS_ICC__)
#pragma pack()
#endif

static struct inic_eth_frame *inic_frame, *inic_frame_tail = NULL;
static int inic_frame_cnt = 0;
#define MAX_INIC_FRAME_NUM 50 //maximum packets for each channel
extern void inic_c2h_msg(const char *atcmd, char status, char *msg, u16 msg_len);
#endif

struct eth_buffer {
	struct eth_frame *head;
	struct eth_frame *tail;
};

static struct eth_buffer eth_buffer;

#ifdef CONFIG_PROMISC
#define MAX_PACKET_FILTER_INFO 5
#define FILTER_ID_INIT_VALUE 10
rtw_packet_filter_info_t paff_array[MAX_PACKET_FILTER_INFO]={0, 0, 0, 0, 0};
static u8 packet_filter_enable_num = 0;

void promisc_init_packet_filter()
{
	int i = 0;
	for(i=0; i<MAX_PACKET_FILTER_INFO; i++){
		paff_array[i].filter_id = FILTER_ID_INIT_VALUE;
		paff_array[i].enable = 0;
		paff_array[i].patt.mask_size = 0;
		paff_array[i].rule = RTW_POSITIVE_MATCHING;
		paff_array[i].patt.mask = NULL;
		paff_array[i].patt.pattern = NULL;
	}
	packet_filter_enable_num = 0;
}

int promisc_add_packet_filter(u8 filter_id, rtw_packet_filter_pattern_t *patt, rtw_packet_filter_rule_t rule)
{
	int i = 0;
	while(i < MAX_PACKET_FILTER_INFO){
		if(paff_array[i].filter_id == FILTER_ID_INIT_VALUE){
			break;
		}
		i++;	
	}

	if(i == MAX_PACKET_FILTER_INFO)
		return -1;

	paff_array[i].filter_id = filter_id;

	paff_array[i].patt.offset= patt->offset;
	paff_array[i].patt.mask_size = patt->mask_size;
	paff_array[i].patt.mask = rtw_malloc(patt->mask_size);
	memcpy(paff_array[i].patt.mask, patt->mask, patt->mask_size);
	paff_array[i].patt.pattern= rtw_malloc(patt->mask_size);
	memcpy(paff_array[i].patt.pattern, patt->pattern, patt->mask_size);

	paff_array[i].rule = rule;
	paff_array[i].enable = 0;

	return 0;
}

int promisc_enable_packet_filter(u8 filter_id)
{
	int i = 0;
	while(i < MAX_PACKET_FILTER_INFO){
		if(paff_array[i].filter_id == filter_id)
			break;
		i++;
	}

	if(i == MAX_PACKET_FILTER_INFO)
		return -1;

	paff_array[i].enable = 1;
	packet_filter_enable_num++;
	return 0;
}

int promisc_disable_packet_filter(u8 filter_id)
{
	int i = 0;
	while(i < MAX_PACKET_FILTER_INFO){
		if(paff_array[i].filter_id == filter_id)
			break;
		i++;
	}

	if(i == MAX_PACKET_FILTER_INFO)
		return -1;

	paff_array[i].enable = 0;
	packet_filter_enable_num--;
	return 0;
}

int promisc_remove_packet_filter(u8 filter_id)
{
	int i = 0;
	while(i < MAX_PACKET_FILTER_INFO){
		if(paff_array[i].filter_id == filter_id)
			break;
		i++;
	}

	if(i == MAX_PACKET_FILTER_INFO)
		return -1;

	paff_array[i].filter_id = FILTER_ID_INIT_VALUE;
	paff_array[i].enable = 0;
	paff_array[i].rule = 0;
	if(paff_array[i].patt.mask){
		rtw_mfree((void *) paff_array[i].patt.mask, paff_array[i].patt.mask_size);
		paff_array[i].patt.mask = NULL;
	}
	
	if(paff_array[i].patt.pattern){
		rtw_mfree((void *) paff_array[i].patt.pattern, paff_array[i].patt.mask_size);
		paff_array[i].patt.pattern = NULL;
	}
	paff_array[i].patt.mask_size = 0;
	return 0;
}
#endif

/*	Make callback simple to prevent latency to wlan rx when promiscuous mode */
static void promisc_callback(unsigned char *buf, unsigned int len, void* userdata)
{
	struct eth_frame *frame = (struct eth_frame *) rtw_malloc(sizeof(struct eth_frame));
	
	if(frame) {
		frame->prev = NULL;
		frame->next = NULL;
		memcpy(frame->da, buf, 6);
		memcpy(frame->sa, buf+6, 6);
		frame->len = len;
		frame->rssi = ((ieee80211_frame_info_t *)userdata)->rssi;
		_lock lock;
		_irqL irqL;
		rtw_enter_critical(&lock, &irqL);

		if(eth_buffer.tail) {
			eth_buffer.tail->next = frame;
			frame->prev = eth_buffer.tail;
			eth_buffer.tail = frame;
		}
		else {
			eth_buffer.head = frame;
			eth_buffer.tail = frame;
		}

		rtw_exit_critical(&lock, &irqL);
	}
}

struct eth_frame* retrieve_frame(void)
{
	struct eth_frame *frame = NULL;

	_lock lock;
	_irqL irqL;
	rtw_enter_critical(&lock, &irqL);

	if(eth_buffer.head) {
		frame = eth_buffer.head;

		if(eth_buffer.head->next) {
			eth_buffer.head = eth_buffer.head->next;
			eth_buffer.head->prev = NULL;
		}
		else {
			eth_buffer.head = NULL;
			eth_buffer.tail = NULL;
		}
	}

	rtw_exit_critical(&lock, &irqL);

	return frame;
}

static void promisc_test(int duration, unsigned char len_used)
{
	int ch;
	unsigned int start_time;
	struct eth_frame *frame;
	eth_buffer.head = NULL;
	eth_buffer.tail = NULL;

	wifi_enter_promisc_mode();
	wifi_set_promisc(RTW_PROMISC_ENABLE, promisc_callback, len_used);

	for(ch = 1; ch <= 13; ch ++) {
		if(wifi_set_channel(ch) == 0)
			printf("\n\n\rSwitch to channel(%d)", ch);

		start_time = rtw_get_current_time();

		while(1) {
			unsigned int current_time = rtw_get_current_time();

			if(rtw_systime_to_ms(current_time - start_time) < duration) {
				frame = retrieve_frame();

				if(frame) {
					int i;
					printf("\n\rDA:");
					for(i = 0; i < 6; i ++)
						printf(" %02x", frame->da[i]);
					printf(", SA:");
					for(i = 0; i < 6; i ++)
						printf(" %02x", frame->sa[i]);
					printf(", len=%d", frame->len);
					printf(", RSSI=%d", frame->rssi);
#if CONFIG_INIC_CMD_RSP
					if(inic_frame_tail){
						if(inic_frame_cnt < MAX_INIC_FRAME_NUM){
							memcpy(inic_frame_tail->da, frame->da, 6);
							memcpy(inic_frame_tail->sa, frame->sa, 6);
							inic_frame_tail->len = frame->len;
							inic_frame_tail++;
							inic_frame_cnt++;
						}
					}
#endif	
					rtw_mfree((void *) frame, sizeof(struct eth_frame));
				}
				else
					rtw_mdelay_os(1);	//delay 1 tick
			}
			else
				break;	
		}
#if CONFIG_INIC_CMD_RSP
		if(inic_frame){
			inic_c2h_msg("ATWM", RTW_SUCCESS, (char *)inic_frame, sizeof(struct inic_eth_frame)*inic_frame_cnt);
			memset(inic_frame, '\0', sizeof(struct inic_eth_frame)*MAX_INIC_FRAME_NUM);
				inic_frame_tail = inic_frame;
				inic_frame_cnt = 0;
			rtw_msleep_os(10);
		}
#endif
	}

	wifi_set_promisc(RTW_PROMISC_DISABLE, NULL, 0);

	while((frame = retrieve_frame()) != NULL)
		rtw_mfree((void *) frame, sizeof(struct eth_frame));
}

static void promisc_callback_all(unsigned char *buf, unsigned int len, void* userdata)
{
	struct eth_frame *frame = (struct eth_frame *) rtw_malloc(sizeof(struct eth_frame));
	
	if(frame) {
		frame->prev = NULL;
		frame->next = NULL;
		memcpy(frame->da, buf+4, 6);
		memcpy(frame->sa, buf+10, 6);
		frame->len = len;
		/*  
		* type is the first byte of Frame Control Field of 802.11 frame
		* If the from/to ds information is needed, type could be reused as follows:
		* frame->type = ((((ieee80211_frame_info_t *)userdata)->i_fc & 0x0100) == 0x0100) ? 2 : 1;
		* 1: from ds; 2: to ds
		*/		
		frame->type = *buf;
		frame->rssi = ((ieee80211_frame_info_t *)userdata)->rssi;

		_lock lock;
		_irqL irqL;
		rtw_enter_critical(&lock, &irqL);


		if(eth_buffer.tail) {
			eth_buffer.tail->next = frame;
			frame->prev = eth_buffer.tail;
			eth_buffer.tail = frame;
		}
		else {
			eth_buffer.head = frame;
			eth_buffer.tail = frame;
		}

		rtw_exit_critical(&lock, &irqL);
	}
}
static void promisc_test_all(int duration, unsigned char len_used)
{
	int ch;
	unsigned int start_time;
	struct eth_frame *frame;
	eth_buffer.head = NULL;
	eth_buffer.tail = NULL;

	wifi_enter_promisc_mode();
	wifi_set_promisc(RTW_PROMISC_ENABLE_2, promisc_callback_all, len_used);

	for(ch = 1; ch <= 13; ch ++) {
		if(wifi_set_channel(ch) == 0)
			printf("\n\n\rSwitch to channel(%d)", ch);

		start_time = rtw_get_current_time();

		while(1) {
			unsigned int current_time = rtw_get_current_time();

			if(rtw_systime_to_ms(current_time - start_time) < duration) {
				frame = retrieve_frame();

				if(frame) {
					int i;
					printf("\n\rTYPE: 0x%x, ", frame->type);
					printf("DA:");
					for(i = 0; i < 6; i ++)
						printf(" %02x", frame->da[i]);
					printf(", SA:");
					for(i = 0; i < 6; i ++)
						printf(" %02x", frame->sa[i]);
					printf(", len=%d", frame->len);
					printf(", RSSI=%d", frame->rssi);
#if CONFIG_INIC_CMD_RSP
					if(inic_frame_tail){
						if(inic_frame_cnt < MAX_INIC_FRAME_NUM){
							memcpy(inic_frame_tail->da, frame->da, 6);
							memcpy(inic_frame_tail->sa, frame->sa, 6);
							inic_frame_tail->len = frame->len;
							inic_frame_tail->type = frame->type;
							inic_frame_tail++;
							inic_frame_cnt++;
						}
					}
#endif	
					rtw_mfree((void *) frame, sizeof(struct eth_frame));
				}
				else
					rtw_mdelay_os(1);	//delay 1 tick
			}
			else
				break;	
		}
#if CONFIG_INIC_CMD_RSP
		if(inic_frame){
			inic_c2h_msg("ATWM", RTW_SUCCESS, (char *)inic_frame, sizeof(struct inic_eth_frame)*inic_frame_cnt);
			memset(inic_frame, '\0', sizeof(struct inic_eth_frame)*MAX_INIC_FRAME_NUM);
				inic_frame_tail = inic_frame;
				inic_frame_cnt = 0;
			rtw_msleep_os(10);
		}
#endif
	}

	wifi_set_promisc(RTW_PROMISC_DISABLE, NULL, 0);

	while((frame = retrieve_frame()) != NULL)
		rtw_mfree((void *) frame, sizeof(struct eth_frame));
}

void cmd_promisc(int argc, char **argv)
{
	int duration;
#if CONFIG_INIC_CMD_RSP
	inic_frame_tail = inic_frame = rtw_malloc(sizeof(struct inic_eth_frame)*MAX_INIC_FRAME_NUM);
	if(inic_frame == NULL){
		inic_c2h_msg("ATWM", RTW_BUFFER_UNAVAILABLE_TEMPORARY, NULL, 0);
		return;
	}
#endif
	#ifdef CONFIG_PROMISC
	wifi_init_packet_filter();
	#endif
	if((argc == 2) && ((duration = atoi(argv[1])) > 0))
		//promisc_test(duration, 0);
		promisc_test_all(duration, 0);
	else if((argc == 3) && ((duration = atoi(argv[1])) > 0) && (strcmp(argv[2], "with_len") == 0))
		promisc_test(duration, 1);
	else
		printf("\n\rUsage: %s DURATION_SECONDS [with_len]", argv[0]);
#if CONFIG_INIC_CMD_RSP
	if(inic_frame)
		rtw_mfree(inic_frame, sizeof(struct inic_eth_frame)*MAX_INIC_FRAME_NUM);
	inic_frame_tail = NULL;
	inic_frame_cnt = 0;
#endif
}
#endif	//#if CONFIG_WLAN

