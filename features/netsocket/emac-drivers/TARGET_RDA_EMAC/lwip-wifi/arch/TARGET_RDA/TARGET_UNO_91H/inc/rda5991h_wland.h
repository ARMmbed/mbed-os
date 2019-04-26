/* Copyright (c) 2019 Unisoc Communications Inc.
 * SPDX-License-Identifier: Apache-2.0
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
 */

#ifndef _RDA5991H_WLAND_H_
#define _RDA5991H_WLAND_H_

#include "sys_arch.h"
#include "wland_types.h"

/* Mbed interface mac address
 * if MBED_MAC_ADD_x are zero, interface uid sets mac address,
 * otherwise MAC_ADD_x are used.
 */

extern unsigned char user_mac[6];//not save in flash, need fill before wifi init every time
extern unsigned char gssid[32+1];
extern unsigned char gpass[64+1];
extern unsigned char gchannel;
extern unsigned char gbssid[6];

extern unsigned char gssid_ap[32+1];
extern unsigned char gpass_ap[64+1];
extern unsigned char gchannel_ap;
extern void *wland_msgQ;
extern void *wifi_auth_sem;

extern u8 sta_state;

typedef enum {
    WLAND_CONNECT,
    WLAND_RECONNECT,
    WLAND_DISCONNECT,
    WLAND_DISCONNECT_ERROR,
    WLAND_STARTAP,
    WLAND_STOPAP,
    WLAND_ADD_AP_GTK,
    WLAND_AP_EAPOL_3_OF_4,
    WLAND_ADD_AP_PTK,
    WLAND_STAJOINED,
    WLAND_STAEXITED,
    WLAND_STADEAUTH,
    WLAND_STADELETETIMER,
    WLAND_MAC_CONNECTED,
    WLAND_MAC_AP_CONNECTED,
    WLAND_ADD_GTK,
    WLAND_ADD_PTK,
    WLAND_CON_FINISH,
    WLAND_AUTO_RATE,
    WLAND_ARP_OFFLOAD,
    WLAND_SM_START,
    WLAND_SM_STOP,
    WLAND_WPS_CONNECT,
    WLAND_WPS_START,
    WLAND_WPS_DISCONNECT,
}WLAND_MSG;

typedef enum {
    MAIN_CONNECT,
    MAIN_RECONNECT,
    MAIN_DISCONNECT,
    MAIN_STOP_AP,
}MAIN_MSG;

typedef struct {
    unsigned int type;
    unsigned int arg1;
    unsigned int arg2;
    unsigned int arg3;
}rda_msg;


enum {
    D_NONE_LEVEL = 0,
    D_ERROR_LEVEL = 1,
    D_INFO_LEVEL = 2,
    D_DEBUG_LEVEL = 3,
};

#define WLAND_DBG_DUMP    0
#define WPA_DBG_DUMP      0
#define HUT_DBG_DUMP      0
#define WLAND_DBG_LEVEL D_NONE_LEVEL
#define WPA_DBG_LEBEL   D_NONE_LEVEL
#define WLANDLIB_DBG_LEVEL D_NONE_LEVEL

#define ETH_ALEN 6

//encrypt type
#define ENCRYPT_NONE        (0)
#define ENCRYPT_WPA_TKIP    BIT0
#define ENCRYPT_WPA_CCMP    BIT1
#define ENCRYPT_WPA2_TKIP   BIT2
#define ENCRYPT_WPA2_CCMP   BIT3
#define ENCRYPT_WEP         BIT4

/* r91h driver data structure */
typedef struct {
    struct netif *netif_sta;
    struct netif *netif_ap;
    sys_thread_t wland_thread;
    sys_thread_t maclib_thread;
    sys_mbox_t maclib_mbox;
    sys_mbox_t wland_mbox;
} rda_enetdata_t;

__STATIC_INLINE int mac_is_valid(char* mac)
{
    return (mac[0] | mac[1] | mac[2] | mac[3] | mac[4] | mac[5]);
}


#ifdef __cplusplus
extern "C" {
#endif

extern void wland_txip_data(void *data, unsigned int len, int mode);
extern void *wland_get_databuf(void);
extern void wland_sta_init(void);
extern void wland_reg_func(void);
extern void r91h_phy_task(void *data);
extern void wland_task(void *arg);
extern void rda_get_macaddr(u8_t *macaddr, int mode);
extern void rda5981_send_rawdata(char* data, unsigned int len);
extern int rda5981_send_nulldata(int power_save);
extern void rda5981_set_country_code(unsigned char country_code);// 0~china(1-14) 1~NA(1-11) 2~EU(1-13)
extern int rda5981_set_retrans_policy(unsigned char count);
extern int rda5981_set_channel(unsigned char channel);
/* default is 0, receive multicast packet, disable please set 1*/
extern int rda5981_filter_multicast(unsigned char enable);
/* default is 0, 0 ~ no hidden, 1 ~ hidden zero len, 2 ~ hidden zero contents  */
extern void rda5981_set_AP_hidden_type(unsigned char mode);
extern void rda5981_set_AP_link_num(unsigned char num);
extern char* rda5981_get_ver(void);
extern int rda5981_enter_CE_MODE(unsigned char enable);
/*
 * mode    0 - not 11n   1 - 11n
 *
 * -----------------11n(mode 1)(Mbps)-----------------
 * rate          HT20                   HT40
 *       GI(800ns)  GI(400ns)   GI(800ns)  GI(400ns)
 *  0       6.5       7.2          13.5       15
 *  1       13       14.2           27        30
 *  2      19.5      21.7          40.5       45
 *  3       26       28.9           54        60
 *  4       39       43.3           81        90
 *  5       52       57.8           108       120
 *  6      58.5      65            121.5      135  
 *  7       65       72             135       150  
 *
 * --------------not 11n(mode 0)(Mbps)-----------------
 * rate   data rate     rate   data rate
 *  0     autorate        9        9        
 *  1        1           12       12       
 *  2        2           18       18       
 *  5       5.5          24       24       
 *  11       11          36       36       
 *  6        6           48       48
 *
 */
extern int rda5981_set_data_rate(unsigned char mode, unsigned char rate);
extern void rda5981_set_mode(unsigned char bgn_enable);
extern void rda5981_set_auth_timeout(unsigned char timeout_enable);
typedef struct {
    char BSSID[ETH_ALEN];
    char SSID[32+1];
    signed char RSSI;
    unsigned char SSID_len;
    unsigned char channel;
    unsigned char secure_type;//refer #define ENCRYPT_XXX
    unsigned char wmm;
    unsigned char *ie;//user program couldn't free(ie);
    unsigned short capability;
    unsigned int ie_length;
} rda5981_scan_result;

typedef struct {
    unsigned char mac[ETH_ALEN];
    unsigned int ip;
} rda5981_apsta_info;

//scan one or all channel(if channel is 0) once
int rda5981_scan(const char *SSID, const unsigned char SSID_len, const unsigned char channel);
//0 passive mode, 1 active mode, scan time(unit is second)
int rda5981_scan_v2(const char *SSID, const unsigned char SSID_len, const unsigned char channel, const unsigned char mode, \
    const unsigned char scan_time);
int rda5981_get_scan_num();
int rda5981_get_scan_result(rda5981_scan_result *buf, const unsigned char len);
int rda5981_get_scan_result_index(rda5981_scan_result *buf, const unsigned char index);
int rda5981_get_scan_result_name(rda5981_scan_result *buf, const char *name);
int rda5981_get_scan_result_bssid(rda5981_scan_result *buf, const unsigned char *bssid);
int rda5981_check_scan_result_name(const char *name);
int rda5981_check_scan_result(const char *ssid, const char *bssid, const unsigned channel);
int rda5981_check_scan_result_name_bssid(const unsigned char *name, const unsigned char *bssid);
int rda5981_del_scan_all_result(void);
void rda5981_set_expired_time(unsigned int expired_time);
int rda5981_get_joined_AP(rda5981_scan_result *bss);
s8 rda5981_get_rssi();
void rda5981_set_main_queue(void* queue);

void rda5981_set_sta_listen_interval(unsigned char interval);
void rda5981_set_sta_link_loss_time(unsigned char time);
unsigned int rda5981_get_ap_join_info(rda5981_apsta_info *buf, const unsigned char len);
void rda5981_set_AP_white_list(unsigned char flag, unsigned char *mac);
int wland_set_joined_sta_ip(char *mac, unsigned int ip);

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_read_mac_addr(unsigned char *mac_addr);
int rda5981_flash_write_mac_addr(unsigned char *mac_addr);

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_erase_uart(void);
int rda5981_flash_read_uart(unsigned int *uart);
int rda5981_flash_write_uart(unsigned int *uart);

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_read_ip_addr(unsigned char *ip_addr, unsigned char *server_addr);
int rda5981_flash_write_ip_addr(unsigned char *ip_add, unsigned char *server_addr);

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_erase_dhcp_data(void);
int rda5981_flash_read_dhcp_data(unsigned int *enable, unsigned int *ip, unsigned int *msk, unsigned int *gw);
int rda5981_flash_write_dhcp_data(unsigned int enable, unsigned int ip, unsigned int msk, unsigned int gw);

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_read_ap_data(char *ssid, char *passwd, unsigned char *channel);
int rda5981_flash_write_ap_data(const char *ssid, const char *passwd, unsigned char channel);
int rda5981_flash_erase_ap_data(void);

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_read_ap_net_data(unsigned int *ip, unsigned int *msk, unsigned int *gw,
        unsigned int *dhcps, unsigned int *dhcpe);
int rda5981_flash_write_ap_net_data(unsigned int ip, unsigned int msk, unsigned int gw,
            unsigned int dhcps, unsigned int dhcpe);
int rda5981_flash_erase_ap_net_data(void);

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_read_sta_data(char *ssid, char *passwd);
int rda5981_flash_write_sta_data(const char *ssid, const char *passwd);
int rda5981_flash_erase_sta_data(void);


/*
 * read 3rd parter data length from flash
 * return user data length
 */
int rda5981_flash_read_3rdparter_data_length(void);

/*
 * read 3rd parter data from flash
 * @buf, buf to store user data
 * @buf_len, length of buf
 * return user data length
 */
int rda5981_flash_read_3rdparter_data(unsigned char *buf, unsigned int buf_len);

/*
 * write 3rd parter data from flash
 * @buf, data to write
 * @buf_len, length of buf.
 * return 0:ok, else:fail
 */
int rda5981_flash_write_3rdparter_data(const unsigned char *buf, unsigned int buf_len);

/*
 * erase 3rd parter data from flash
 * return 0:ok, else:fail
 */
int rda5981_flash_erase_3rdparter_data(void);

/*
 * set flash size
 * @size, 1MB:0x100000, 2MB:0x200000, 4MB:0x400000. default size: 1MB
 * return 0:ok, else:fail
 */
int rda5981_set_flash_size(const unsigned int size);

/*
 * set userdata location on flash
 * @sys_data_addr, data to save system parameter, user can not operate this area directly.
 *         size:4KB. default location:0x180fb000
 * @user_data_addr, data to save user data. user can save own data in this area
 *         by @rda5981_flash_read_3rdparter_data
 *         and @rda5981_flash_write_3rdparter_data
 *         default location:0x180fc000
 * @user_data_len, user data length, default:4KB
 * return 0:ok, else:fail
 */
int rda5981_set_user_data_addr(const unsigned int sys_data_addr,
    const unsigned int user_data_addr, const unsigned int user_data_len);

/*
 * function: erase flash
 * @addr: mast be 4k alignment
 * @len:  must be 4k alignment. (package 64KB erase and 4KB erase for different condition automatically)
 * return: 0:success, else:fail
 */
int rda5981_erase_flash(unsigned int addr, unsigned int len);

/*
 * function: write flash
 * @addr: mast be 256 alignment
 * @buf: data to be written, best be 4 alignment
 * @len: buffer len, mast be 4 alignment
 * return: 0:success, else:fail
 */
int rda5981_write_flash(unsigned int addr, char *buf, unsigned int len);

/*
 * function: read flash to @buf
 * @addr: best be 4 alignment
 * @buf: best be 4 alignment
 * @len: buffer len
 * return: 0:success, else:fail
 */
int rda5981_read_flash(unsigned int addr, char *buf, unsigned int len);

/*
 * function: read user data
 * @data: data to read
 * @len: length of data in byte
 * @flag: user data flag
 * return: 0:success, else:fail
 */
int rda5981_read_user_data(unsigned char *data, unsigned short len, unsigned int flag);

/*
 * function: write user data
 * @data: data to write
 * @len: length of data in byte
 * @flag: user data flag
 * return: 0:success, else:fail
 */
int rda5981_write_user_data(unsigned char *data, unsigned short len, unsigned int flag);

/*
 * function: erase user data
 * @flag: user data flag
 * return: 0:success, else:fail
 */
int rda5981_erase_user_data(unsigned int flag);

/*
 * function: update tx power from efuse data, for reg 11F and 120
 * return: 0:success, else:fail
 */
int update_tx_power_from_efuse(void);

/*
 * function: update xtal calibration from efuse data, for reg DA
 * return: 0:success, else:fail
 */
int update_xtal_cal_from_efuse(void);

/*
 * function: update mac addr from flash data
 * return: 0:success, else:fail
 */
int update_mac_addr_from_efuse(void);

/*
 * function: update tx power from flash data, Deprecated version
 * return: 0:success, else:fail
 */
int update_tx_power_from_flash(void);

/*
 * function: update tx power from flash data, for reg 8A
 * return: 0:success, else:fail
 */
int update_tx_power_rf_from_flash(void);

/*
 * function: update tx power from flash data, for reg 11F and 120
 * return: 0:success, else:fail
 */
int update_tx_power_phy_from_flash(void);

/*
 * function: update xtal calibration from flash data
 * return: 0:success, else:fail
 */
int update_xtal_cal_from_flash(void);

/*
 * function: update mac addr from flash data
 * return: 0:success, else:fail
 */
int update_mac_addr_from_flash(void);

/*
 * function: write rf reg
 * @reg: rf reg data
 * @value: rf reg value
 * return: 0:success, else:fail
 * eg: 0x00DA:xtal calibration
 */
int wland_rf_write(unsigned short reg, unsigned short value);

/*
 * function: write rf reg
 * @reg: rf reg data
 * @value: rf reg value
 * @len : value length
 * return: 0:success, else:fail
 * eg: 0x008A:tx power rf
 */
int wland_rf_write_all_channels(unsigned short reg, unsigned short *value, unsigned short len);

/*
 * function: read rf reg
 * @reg: rf reg data
 * @value: rf reg value
 * return: 0:success, else:fail
 */
int wland_rf_read(unsigned short reg, unsigned short *value);

/*
 * function: read rf reg
 * @reg: rf reg data
 * @value: rf reg value
 * return: 0:success, else:fail
 * eg: 0x008A:tx power rf
 */
int wland_rf_read_all_channels(unsigned short reg, unsigned short *value);

/*
 * function: write phy reg
 * @reg: phy reg data
 * @value: phy reg value
 * return: 0:success, else:fail
 */
int wland_phy_write(unsigned int reg, unsigned int value);

/*
 * function: write phy reg
 * @reg: phy reg data
 * @value: phy reg value
 * @len : value length
 * return: 0:success, else:fail
 */
int wland_phy_write_all_channels(unsigned int reg, unsigned int *value, unsigned short len);

/*
 * function: read phy reg
 * @reg: phy reg data
 * @value: phy reg value
 * return: 0:success, else:fail
 */
int wland_phy_read(unsigned int reg, unsigned int *value);

/*
 * function: read phy reg
 * @reg: phy reg data
 * @value: phy reg value
 * return: 0:success, else:fail
 */
int wland_phy_read_all_channels(unsigned int reg, unsigned int *value);

/* efuse API start */
/* Efuse CAN ONLY WRITE ONCE! DO NOT CALL THESE API IF YOU DO KNOW WHAT THEY MEANS!!! */

/*
 * function: read all efuse
 * @value: buffer to store efuse data, 28 bytes
 * return: 0:success, else:fail
 */
int wland_read_efuse(unsigned char *value);

/*
 * function: read tx power from efuse
 * @tx_power: 2 bytes, first is mode g/n(range 0x25~0x54), second is mode b(range 0x15~0x54).
 * return: 0:success, else:fail
 */
int wland_read_tx_power_from_efuse(unsigned char *tx_power);

/*
 * function: read tx power from efuse
 * @tx_power: 2 bytes, first is mode g/n(range 0x25~0x54), second is mode b(range 0x15~0x54)
 * @len: must be 2
 * return: 0:success, else:fail
 */
int wland_write_tx_power_to_efuse(unsigned char *tx_power, unsigned char len);

/*
 * function: read xtal cal from efuse
 * @xtal_cal: 1 byte, maximum 0x7F
 * return: 0:success, else:fail
 */
int wland_read_xtal_cal_from_efuse(unsigned char *cal_val);

/*
 * function: write xtal cal to efuse
 * @xtal_cal: 1 byte, maximum 0x7F
 * @len : must be 1
 * return: 0:success, else:fail
 */
int wland_write_xtal_cal_to_efuse(unsigned char *xtal_cal, unsigned char len);

/*
 * function: write mac to efuse
 * @xtal_cal: 6 bytes
 * return: 0:success, else:fail
 */
int wland_read_mac_addr_from_efuse(unsigned char *mac_addr);

/*
 * function: write mac to efuse
 * @xtal_cal: 6 bytes
 * @len : must be 6
 * return: 0:success, else:fail
 */
int wland_write_mac_addr_to_efuse(unsigned char*mac_addr, unsigned char len);
/* efuse API end */

/*
 * function: start rf test
 * @argc: number of argv
 * @argv: args for test, 6 elements for tx test, 4 elements for rx test
 * @is_tx: 1 for tx test, 0 for rx test
 * return: 0:success, else:fail
 */
int wland_start_rf_test(unsigned int argc, unsigned int *argv, unsigned int is_tx);

/*
 * function: stop rx test
 * return: 0:success, else:fail
 */
int wland_stop_rx_test(void);

/*
 * function: get rf test result
 * @result buffer to store rx result
 * return: 0:success, else:fail
 */
int wland_get_rx_result(char *result);

/*
 * function: restart rx test, use last rx test args
 * return: 0:success, else:fail
 */
int wland_restart_rx_test(void);

/*
 * function: stop tx test
 * return: 0:success, else:fail
 */
int wland_stop_tx_test(void);

/*
 * function: restart tx test, use last tx test args
 * return: 0:success, else:fail
 */
int wland_restart_tx_test(void);

#define RDA5981_FIRMWARE_INFO_ADDR 0x18003000
/*
 * function: reboot to assigned addr (onece).
 * reboot to rf test mode, not for OTA
 * @firmware_info_addr: firmware info addr, depend on your flash layout
 * @reboot_addr: reboot addr, 0x18001000-0x1840000
 * return: 0:success, else:fail
 */
int rda5981_reboot_to_addr(unsigned int firmware_info_addr, unsigned int reboot_addr);

/*
 * function: read reg and corresponding value related to test mode stored in flash
 * @reg: reg to read
 * @value: buffer to store value
 * @flag: user data flag
 * return: 0:success, else:fail
 */
int rda5981_read_user_data_regs(unsigned char *reg, unsigned char *value, unsigned int flag);

/*
 * function: write reg and corresponding value related to test mode stored in flash
 * @reg: reg to write
 * @value: buffer that stores value
 * @flag: user data flag
 * return: 0:success, else:fail
 */
int rda5981_write_user_data_regs(unsigned char *reg, unsigned char *value, unsigned int flag);

/*
 * function: erase reg and corresponding value related to test mode stored in flash
 * @reg: reg to erase
 * @flag: user data flag
 * return: 0:success, else:fail
 */
int rda5981_erase_user_data_regs(unsigned char *reg, unsigned int flag);

/*
 * function: get flash Manufacturer ID
 * @mid:
 * return: 0:success, else:fail
 */
int rda5981_flash_get_mid(unsigned char *mid);

/*
 * function: get flash Device ID
 * @did:
 * return: 0:success, else:fail
 */
int rda5981_flash_get_did(unsigned char *did);

/*
 * function: get flash ID
 * @mid:
 * return: 0:success, else:fail
 */
int rda5981_flash_get_jdid(unsigned short *jdid);

/*
 * function: read mac reg
 * @reg: mac reg data
 * @value: mac reg value
 * return: 0:success, else:fail
 */
int wland_mac_reg_read(unsigned short  reg, unsigned int *value);

/*
 * function: write mac reg
 * @reg: mac reg data
 * @value: mac reg value
 * return: 0:success, else:fail
 */
int wland_mac_reg_write(unsigned short reg, unsigned int value);

#ifdef __cplusplus
}
#endif

#endif
