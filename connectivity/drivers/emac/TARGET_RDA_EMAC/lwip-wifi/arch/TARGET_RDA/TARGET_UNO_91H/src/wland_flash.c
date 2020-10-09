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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "critical.h"
#include "wland_flash.h"
#include "wland_dbg.h"
#include "rda5981_flash.h"
#include "wland_flash_wp.h"

#define MACDBG                          "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC2STRDBG(ea)                  (ea)[0], (ea)[1], (ea)[2], (ea)[3], (ea)[4], (ea)[5]

u32 flash_size = FLASH_SIZE;
static u32 user_data_location = RDA5991H_USER_DATA_ADDR;
static u32 third_parter_data_location = RDA5991H_3RDPARTER_DATA_ADDR;
static u32 third_parter_data_len = RDA5991H_3RDPARTER_DATA_LEN;
int rda5981_read_flash(u32 addr, char *buf, u32 len)
{
    int ret = 0;
    char *temp_buf = NULL, *temp_buf_aligned;

    addr &= (flash_size -1);
    if (addr < RDA5991H_PARTITION_TABLE_END_ADDR-0x18000000) {
        WLAND_DBG(ERROR, "couldn't read system data\n");
        return -1;
    }

    if ((u32)buf % 4) {
        temp_buf = malloc(len + 3);
        if (temp_buf == NULL) {
            ret = -1;
            goto out;
        }
        if ((u32)temp_buf % 4) {
            temp_buf_aligned = temp_buf + (4-(u32)temp_buf%4);
        } else {
            temp_buf_aligned = temp_buf;
        }
    } else {
        temp_buf_aligned = buf;
    }

    core_util_critical_section_enter();
    spi_flash_flush_cache();
    SPI_FLASH_READ_DATA_FOR_MBED((void *)addr, temp_buf_aligned, len);
    core_util_critical_section_exit();

    if (temp_buf_aligned != buf) {
        memcpy(buf, temp_buf_aligned, len);
    }

out:
    if (temp_buf) {
        free(temp_buf);
    }

    return ret;
}

void rda5981_spi_flash_erase_64KB_block(u32 addr)
{
    if (rda_ccfg_hwver() < 4) {
        spi_wip_reset_2();
        spi_write_reset_2();
        WRITE_REG32(FLASH_CTL_TX_CMD_ADDR_REG, CMD_64KB_BLOCK_ERASE | (addr<<8));
        wait_busy_down_2();
        spi_wip_reset_2();
    } else {
        spi_wip_reset_4();
        spi_write_reset_4();
        WRITE_REG32(FLASH_CTL_TX_CMD_ADDR_REG, CMD_64KB_BLOCK_ERASE | (addr<<8));
        wait_busy_down_4();
        spi_wip_reset_4();
    }
}
void rda5981_spi_erase_partition(void *src, u32 counts)
{
    u32 a4k, a64k, a64kend, a4kend, atmp;

    if (counts > 0x00) {
        a4k  = ((u32)src                          ) & (~((0x01UL << 12) - 0x01UL));
        a64k = ((u32)src + (0x01UL << 16) - 0x01UL) & (~((0x01UL << 16) - 0x01UL));
        a64kend = ((u32)src + counts                          ) & (~((0x01UL << 16) - 0x01UL));
        a4kend  = ((u32)src + counts + (0x01UL << 12) - 0x01UL) & (~((0x01UL << 12) - 0x01UL));

        for (atmp = a4k; atmp < a4kend; atmp += (0x01UL << 12)) {
            if (a64kend > a64k) {
                if (atmp == a64k) {
                    for (; atmp < a64kend; atmp += (0x01UL << 16)) {
                        core_util_critical_section_enter();
                        rda5981_spi_flash_erase_64KB_block(atmp);
                        core_util_critical_section_exit();
                    }
                    if (atmp == a4kend) {
                        break;
                    }
                }
            }
            core_util_critical_section_enter();
            rda5981_spi_flash_erase_4KB_sector(atmp);
            core_util_critical_section_exit();
        }
    }
}
//@len must be 4k aligment
int rda5981_erase_flash(u32 addr, u32 len)
{
    WLAND_DBG(INFO, "erase flash :%x:%x\n", addr, len);

    addr &= (flash_size - 1);
    if (addr < RDA5991H_PARTITION_TABLE_END_ADDR-0x18000000) {
        WLAND_DBG(ERROR, "couldn't erase system data\n");
        return -1;
    }

    if (len & (SECTOR_SIZE-1)) {
        len = ((len+SECTOR_SIZE) & (~(SECTOR_SIZE-1)));
    }
    flash_wrtie_protect(addr);
    rda5981_spi_erase_partition((void*)addr, len);
    flash_wrtie_protect_all();
    return 0;
}

//@len must be 256 aligment
int rda5981_write_flash(u32 addr, char *buf, u32 len)
{
    int ret = 0;
    u8 *temp_buf = NULL, *temp_buf_aligned;
    u8 *check_buf = NULL;

    addr &= (flash_size -1);
    if (addr < RDA5991H_PARTITION_TABLE_END_ADDR-0x18000000) {
        WLAND_DBG(ERROR, "couldn't write system data\n");
        return -1;
    }

    if ((u32)buf % 4) {
        temp_buf = malloc(len + 3);
        if (temp_buf == NULL) {
            goto out;
        }
        if ((u32)temp_buf % 4) {
            temp_buf_aligned = temp_buf + (4-(u32)temp_buf%4);
        } else {
            temp_buf_aligned = temp_buf;
        }
        memcpy(temp_buf_aligned, buf, len);
    } else {
        temp_buf_aligned = (u8*)buf;
    }
    flash_wrtie_protect(addr);
    core_util_critical_section_enter();
    //RDA5991H_ERASE_FLASH(addr, len);
    RDA5991H_WRITE_FLASH(addr, temp_buf_aligned, len);
    core_util_critical_section_exit();
    flash_wrtie_protect_all();

#ifdef FLASH_READ_CHECK
    if (ret == 0) {
        check_buf = malloc(len);
        rda5981_read_flash(addr, check_buf, len);
        if (memcmp(buf, check_buf, len) == 0) {
            ret = 0;
        } else {
            ret = -1;
        }
    }
#endif /*FLASH_READ_CHECK*/

out:
    if (temp_buf) {
        free(temp_buf);
    }
    if (check_buf) {
        free(check_buf);
    }
    return ret;
}

/*
 * return 0:ok, else:error.
 */
rda5991h_user_data wland_user_data;
u8 read_flag = 0;
int rda5981_write_user_data(u8 *data, u16 len, u32 flag)
{
    WLAND_DBG(INFO, "Enter, flag:0x%x\r\n", flag);

    if (!read_flag) {
        if (0 == rda5981_read_flash(user_data_location,
            (char *)(&wland_user_data), sizeof(wland_user_data))) {
            read_flag = 1;
        } else {
            return -1;
        }
    }

    if ((wland_user_data.flag & RDA5991H_USER_DATA_FLAG_UNINITIALIZED) ||
        strcmp((const char *)(wland_user_data.rda5981_flag), RDA5981_FLAG_FLAG)) {
        memset(&wland_user_data, 0xff, sizeof(wland_user_data));
        wland_user_data.flag = flag;
        strcpy((char *)(wland_user_data.rda5981_flag), RDA5981_FLAG_FLAG);
    }

    wland_user_data.flag |= flag;

    switch (flag) {
    case RDA5991H_USER_DATA_FLAG_STA:
        memcpy(&wland_user_data.sta_data, data,
            min(sizeof(wland_user_data.sta_data), len));
        break;
    case RDA5991H_USER_DATA_FLAG_MAC:
        memcpy(wland_user_data.mac_addr, data,
            min(sizeof(wland_user_data.mac_addr), len));
        break;
    case RDA5991H_USER_DATA_FLAG_PMK:
        memcpy(wland_user_data.pmk, data,
            min(sizeof(wland_user_data.pmk), len));
        break;
    case RDA5991H_USER_DATA_FLAG_IP:
        memcpy(wland_user_data.ip, data,
            min(RDA5991H_USER_DATA_IP_LENGTH, len));
        break;
    case RDA5991H_USER_DATA_FLAG_PARTER_DATA_LEN:
        memcpy(&wland_user_data.parter_data_len, data,
            min(sizeof(wland_user_data.parter_data_len), len));
        break;
    case RDA5991H_USER_DATA_FLAG_TX_POWER:
        memcpy(&wland_user_data.tx_power, data,
            min(sizeof(wland_user_data.tx_power), len));
        break;
    case RDA5991H_USER_DATA_FLAG_XTAL_CAL:
        memcpy(&wland_user_data.xtal_cal, data,
            min(sizeof(wland_user_data.xtal_cal), len));
        break;
    case RDA5991H_USER_DATA_FLAG_TX_POWER_RF:
        memcpy(&wland_user_data.tx_power_rf, data,
            min(sizeof(wland_user_data.tx_power_rf), len));
        break;
    case RDA5991H_USER_DATA_FLAG_TX_POWER_PHY_GN:
        memcpy(&wland_user_data.tx_power_phy_gn, data,
            min(sizeof(wland_user_data.tx_power_phy_gn), len));
        break;
    case RDA5991H_USER_DATA_FLAG_TX_POWER_PHY_B:
        memcpy(&wland_user_data.tx_power_phy_b, data,
            min(sizeof(wland_user_data.tx_power_phy_b), len));
        break;
    case RDA5991H_USER_DATA_FLAG_AP:
        memcpy(&wland_user_data.ap_data, data,
            min(sizeof(wland_user_data.ap_data), len));
        break;
    case RDA5991H_USER_DATA_FLAG_APNET:
        memcpy(&wland_user_data.ap_net_data, data,
            min(sizeof(wland_user_data.ap_net_data), len));
        break;
    case RDA5991H_USER_DATA_FLAG_DHCP:
        memcpy(&wland_user_data.dhcp, data,
            min(sizeof(wland_user_data.dhcp), len));
        break;
    case RDA5991H_USER_DATA_FLAG_UART:
        memcpy(&wland_user_data.uart, data,
            min(sizeof(wland_user_data.uart), len));
        break;
    case RDA5991H_USER_DATA_FLAG_RF:
        memcpy(&wland_user_data.rf, data,
            min(sizeof(wland_user_data.rf), len));
        break;
    case RDA5991H_USER_DATA_FLAG_RF_CHANNELS:
        memcpy(&wland_user_data.rf_channels, data,
            min(sizeof(wland_user_data.rf_channels), len));
        break;
    case RDA5991H_USER_DATA_FLAG_PHY:
        memcpy(&wland_user_data.phy, data,
            min(sizeof(wland_user_data.phy), len));
        break;
    case RDA5991H_USER_DATA_FLAG_PHY_CHANNELS:
        memcpy(&wland_user_data.phy_channels, data,
            min(sizeof(wland_user_data.phy_channels), len));
        break;
    case RDA5991H_USER_DATA_FLAG_TX_POWER_OFFSET:
        memcpy(&wland_user_data.tp_offset, data,
            min(sizeof(wland_user_data.tp_offset), len));
        break;
    default:
        return -1;
    }
    rda5981_erase_flash(user_data_location, sizeof(wland_user_data));
    return rda5981_write_flash(user_data_location,
        (char *)(&wland_user_data), sizeof(wland_user_data));
}

/*
 * return 0:ok, else:error.
 */
int rda5981_erase_user_data(u32 flag)
{
    if (!read_flag) {
        read_flag = 1;
        rda5981_read_flash(user_data_location,
            (char *)(&wland_user_data), sizeof(wland_user_data));
    }

    if (wland_user_data.flag & RDA5991H_USER_DATA_FLAG_UNINITIALIZED) {//flash init is 0xffffffff
        return -1;
    }

    if ((wland_user_data.flag & flag) == 0) {
        return 0;
    }

    wland_user_data.flag &= (~flag);

    if (wland_user_data.flag == 0) {
        wland_user_data.flag = RDA5991H_USER_DATA_FLAG_UNINITIALIZED;
    }
    if (flag & RDA5991H_USER_DATA_FLAG_STA) {
        memset(&wland_user_data.sta_data, 0xff,
            sizeof(wland_user_data.sta_data));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_MAC) {
        memset(wland_user_data.mac_addr, 0xff,
            sizeof(wland_user_data.mac_addr));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_PMK) {
        memset(wland_user_data.pmk, 0xff,
            sizeof(wland_user_data.pmk));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_IP) {
        memset(wland_user_data.ip, 0xff,
            RDA5991H_USER_DATA_IP_LENGTH);
    }
    if (flag & RDA5991H_USER_DATA_FLAG_PARTER_DATA_LEN) {
        memset(&wland_user_data.parter_data_len, 0xff,
            sizeof(wland_user_data.parter_data_len));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_TX_POWER) {
        memset(&wland_user_data.tx_power, 0xff,
            sizeof(wland_user_data.tx_power));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_XTAL_CAL) {
        memset(&wland_user_data.xtal_cal, 0xff,
            sizeof(wland_user_data.xtal_cal));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_TX_POWER_RF) {
        memset(&wland_user_data.tx_power_rf, 0xff,
            sizeof(wland_user_data.tx_power_rf));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_TX_POWER_PHY_GN) {
        memset(&wland_user_data.tx_power_phy_gn, 0xff,
            sizeof(wland_user_data.tx_power_phy_gn));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_TX_POWER_PHY_B) {
        memset(&wland_user_data.tx_power_phy_b, 0xff,
            sizeof(wland_user_data.tx_power_phy_b));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_AP) {
        memset(&wland_user_data.ap_data, 0xff,
            sizeof(wland_user_data.ap_data));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_APNET) {
        memset(&wland_user_data.ap_net_data, 0xff,
            sizeof(wland_user_data.ap_net_data));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_DHCP) {
        memset(&wland_user_data.dhcp, 0xff,
            sizeof(wland_user_data.dhcp));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_UART) {
        memset(&wland_user_data.uart, 0xff,
            sizeof(wland_user_data.uart));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_RF) {
        memset(&wland_user_data.rf, 0xff,
            sizeof(wland_user_data.rf));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_RF_CHANNELS) {
        memset(&wland_user_data.rf_channels, 0xff,
            sizeof(wland_user_data.rf_channels));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_PHY) {
        memset(&wland_user_data.phy, 0xff,
            sizeof(wland_user_data.phy));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_PHY_CHANNELS) {
        memset(&wland_user_data.phy_channels, 0xff,
            sizeof(wland_user_data.phy_channels));
    }
    if (flag & RDA5991H_USER_DATA_FLAG_TX_POWER_OFFSET) {
        memset(&wland_user_data.tp_offset, 0xff,
            sizeof(wland_user_data.tp_offset));
    }
    rda5981_erase_flash(user_data_location, sizeof(wland_user_data));
    return rda5981_write_flash(user_data_location,
        (char *)(&wland_user_data), sizeof(wland_user_data));
}

/*
 * return 0:ok, else:error.
 */
int rda5981_read_user_data(u8 *data, u16 len, u32 flag)
{
    //rda5991h_user_data wland_user_data;
    WLAND_DBG(INFO, "Enter, flag:0x%x\r\n", flag);

    if (!read_flag) {
        read_flag = 1;
        rda5981_read_flash(user_data_location,
            (char *)(&wland_user_data), sizeof(wland_user_data));
    }

    if (wland_user_data.flag & RDA5991H_USER_DATA_FLAG_UNINITIALIZED) {//flash init is 0xffffffff
        return -1;
    }
    if (strcmp((const char *)(wland_user_data.rda5981_flag), RDA5981_FLAG_FLAG)) {
        return -2;
    }
    if ((wland_user_data.flag & flag) == 0) {
        return -3;
    }
    switch (flag) {
    case RDA5991H_USER_DATA_FLAG_STA:
        memcpy(data, &wland_user_data.sta_data,
            min(sizeof(wland_user_data.sta_data), len));
        break;
    case RDA5991H_USER_DATA_FLAG_MAC:
        memcpy(data, wland_user_data.mac_addr,
            min(sizeof(wland_user_data.mac_addr), len));
        break;
    case RDA5991H_USER_DATA_FLAG_PMK:
        memcpy(data, wland_user_data.pmk,
            min(sizeof(wland_user_data.pmk), len));
        break;
    case RDA5991H_USER_DATA_FLAG_IP:
        memcpy(data, wland_user_data.ip,
            min(RDA5991H_USER_DATA_IP_LENGTH, len));
        break;
    case RDA5991H_USER_DATA_FLAG_PARTER_DATA_LEN:
        memcpy(data, &wland_user_data.parter_data_len,
            min(sizeof(wland_user_data.parter_data_len), len));
        break;
    case RDA5991H_USER_DATA_FLAG_TX_POWER:
        memcpy(data, &wland_user_data.tx_power,
            min(sizeof(wland_user_data.tx_power), len));
        break;
    case RDA5991H_USER_DATA_FLAG_XTAL_CAL:
        memcpy(data, &wland_user_data.xtal_cal,
            min(sizeof(wland_user_data.xtal_cal), len));
        break;
    case RDA5991H_USER_DATA_FLAG_TX_POWER_RF:
        memcpy(data, &wland_user_data.tx_power_rf,
            min(sizeof(wland_user_data.tx_power_rf), len));
        break;
    case RDA5991H_USER_DATA_FLAG_TX_POWER_PHY_GN:
        memcpy(data, &wland_user_data.tx_power_phy_gn,
            min(sizeof(wland_user_data.tx_power_phy_gn), len));
        break;
    case RDA5991H_USER_DATA_FLAG_TX_POWER_PHY_B:
        memcpy(data, &wland_user_data.tx_power_phy_b,
            min(sizeof(wland_user_data.tx_power_phy_b), len));
        break;
    case RDA5991H_USER_DATA_FLAG_AP:
        memcpy(data, &wland_user_data.ap_data,
            min(sizeof(wland_user_data.ap_data), len));
        break;
    case RDA5991H_USER_DATA_FLAG_APNET:
        memcpy(data, &wland_user_data.ap_net_data,
            min(sizeof(wland_user_data.ap_net_data), len));
        break;
    case RDA5991H_USER_DATA_FLAG_DHCP:
        memcpy(data, &wland_user_data.dhcp,
            min(sizeof(wland_user_data.dhcp), len));
        break;
    case RDA5991H_USER_DATA_FLAG_UART:
        memcpy(data, &wland_user_data.uart,
            min(sizeof(wland_user_data.uart), len));
        break;
    case RDA5991H_USER_DATA_FLAG_RF:
        memcpy(data, &wland_user_data.rf,
            min(sizeof(wland_user_data.rf), len));
        break;
    case RDA5991H_USER_DATA_FLAG_RF_CHANNELS:
        memcpy(data, &wland_user_data.rf_channels,
            min(sizeof(wland_user_data.rf_channels), len));
        break;
    case RDA5991H_USER_DATA_FLAG_PHY:
        memcpy(data, &wland_user_data.phy,
            min(sizeof(wland_user_data.phy), len));
        break;
    case RDA5991H_USER_DATA_FLAG_PHY_CHANNELS:
        memcpy(data, &wland_user_data.phy_channels,
            min(sizeof(wland_user_data.phy_channels), len));
        break;
    case RDA5991H_USER_DATA_FLAG_TX_POWER_OFFSET:
        memcpy(data, &wland_user_data.tp_offset,
            min(sizeof(wland_user_data.tp_offset), len));
        break;
    default:
        return -1;
    }
    return 0;
}

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_read_mac_addr(u8 *mac_addr)
{
    int ret;
    WLAND_DBG(DEBUG, "Enter\r\n");
    ret = rda5981_read_user_data(mac_addr, 6, RDA5991H_USER_DATA_FLAG_MAC);
    if (ret) {
        WLAND_DBG(ERROR,  "read mac addr from flash fail\r\n");
    } else {
        WLAND_DBG(INFO, "Done(ret:%d)"MACDBG"\r\n", ret, MAC2STRDBG(mac_addr));
    }
    return ret;
}

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_write_mac_addr(u8 *mac_addr)
{
    WLAND_DBG(INFO, "Enter"MACDBG"\r\n", MAC2STRDBG(mac_addr));
    return rda5981_write_user_data(mac_addr, 6, RDA5991H_USER_DATA_FLAG_MAC);
}

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_erase_uart(void)
{
    return rda5981_erase_user_data(RDA5991H_USER_DATA_FLAG_UART);
}

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_read_uart(u32 *uart)
{
    int ret;
    WLAND_DBG(DEBUG, "Enter\r\n");
    ret = rda5981_read_user_data((u8 *)uart, 4, RDA5991H_USER_DATA_FLAG_UART);
    if (ret) {
        WLAND_DBG(ERROR,  "read uart setting from flash fail\r\n");
    }
    return ret;
}

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_write_uart(u32 *uart)
{
    return rda5981_write_user_data((u8 *)uart, 4, RDA5991H_USER_DATA_FLAG_UART);
}

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_erase_sta_data(void)
{
    int ret;
    ret = rda5981_erase_user_data(RDA5991H_USER_DATA_FLAG_PMK |
        RDA5991H_USER_DATA_FLAG_STA);
    return ret;
}

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_read_sta_data(char *ssid, char *passwd)
{
    int ret;
    wland_sta_data_t sta_data;

    WLAND_DBG(INFO, "Enter\r\n");

    ret = rda5981_read_user_data((u8 *)&sta_data, sizeof(sta_data), RDA5991H_USER_DATA_FLAG_STA);
    if (ret == 0) {
        strcpy(ssid, sta_data.ssid);
        strcpy(passwd, sta_data.key);
    }
    return ret;
}

/*
 * return 0:ok, else:error.
 */
extern u8 *rda_get_bssinfo_pmk(void);
extern u8 *rda_get_bssinfo_SSID(void);
int rda5981_flash_write_sta_data(const char *ssid, const char *passwd)
{
    int ret = 0, ret1;
    wland_sta_data_t sta_data;
    u8 *pbss_info_pmk = NULL, *pbss_info_SSID = NULL;
    pbss_info_pmk = rda_get_bssinfo_pmk();
    pbss_info_SSID = rda_get_bssinfo_SSID();

    WLAND_DBG(INFO, "Enter:ssid:%s,pw:%s, pmk:%02x %02x***\r\n",
        ssid, passwd, pbss_info_pmk[0], pbss_info_SSID[1]);
    if (strlen(ssid) == 0) {
        WLAND_DBG(ERROR, "ssid is NULL\n");
        return -1;
    }

    memset(&sta_data, 0xff, sizeof(sta_data));
    strcpy(sta_data.ssid, ssid);
    strcpy(sta_data.key, passwd);
    if (pbss_info_pmk[0] && memcmp(pbss_info_SSID, ssid, 6)==0) {
        ret = rda5981_write_user_data(pbss_info_pmk, sizeof(pbss_info_pmk), RDA5991H_USER_DATA_FLAG_PMK);
    }
    ret1 = rda5981_write_user_data((u8 *)&sta_data, sizeof(sta_data), RDA5991H_USER_DATA_FLAG_STA);
    return ret || ret1;;
}


/*
 * return 0:ok, else:error.
 */
int rda5981_flash_erase_dhcp_data(void)
{
    int ret;//, ret1;
    //WLAND_DBG(INFO, "Enter\r\n");
    ret = rda5981_erase_user_data(RDA5991H_USER_DATA_FLAG_DHCP);
    //ret1 = rda5981_erase_user_data(RDA5991H_USER_DATA_FLAG_PMK);
    return ret;
}

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_read_dhcp_data(unsigned int *fixip, unsigned int *ip, unsigned int *msk, unsigned int *gw)
{
    int ret;
    wland_dhcp_t dhcp;

    WLAND_DBG(INFO, "Enter\r\n");

    ret = rda5981_read_user_data((u8 *)&dhcp, sizeof(dhcp), RDA5991H_USER_DATA_FLAG_DHCP);
    if (ret == 0) {
        *fixip = dhcp.fixip;
        *ip = dhcp.ip;
        *msk = dhcp.msk;
        *gw = dhcp.gw;
    }
    return ret;
}

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_write_dhcp_data(unsigned int fixip, unsigned int ip, unsigned int msk, unsigned int gw)
{
    int ret = 0;
    wland_dhcp_t dhcp;

    memset(&dhcp, 0xff, sizeof(dhcp));
    dhcp.fixip = fixip;
    dhcp.ip = ip;
    dhcp.msk = msk;
    dhcp.gw = gw;
    ret = rda5981_write_user_data((u8 *)&dhcp, sizeof(dhcp), RDA5991H_USER_DATA_FLAG_DHCP);
    return ret;
}


/*
 * return 0:ok, else:error.
 */
int rda5981_flash_erase_ap_data(void)
{
    int ret;
    ret = rda5981_erase_user_data(RDA5991H_USER_DATA_FLAG_AP);
    return ret;
}

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_write_ap_data(const char *ssid, const char *passwd, unsigned char channel)
{
    int ret = 0;
    wland_ap_data_t ap_data;

    if (strlen(ssid) == 0) {
        WLAND_DBG(ERROR, "ssid is NULL\n");
        return -1;
    }

    memset(&ap_data, 0xff, sizeof(ap_data));
    strcpy(ap_data.ssid, ssid);
    strcpy(ap_data.key, passwd);
    ap_data.channel = channel;
    ret = rda5981_write_user_data((u8 *)&ap_data, sizeof(ap_data), RDA5991H_USER_DATA_FLAG_AP);
    return ret;
}

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_read_ap_data(char *ssid, char *passwd, unsigned char *channel)
{
    int ret;
    wland_ap_data_t ap_data;

    WLAND_DBG(INFO, "Enter\r\n");

    ret = rda5981_read_user_data((u8 *)&ap_data, sizeof(ap_data), RDA5991H_USER_DATA_FLAG_AP);
    if (ret == 0) {
        strcpy(ssid, ap_data.ssid);
        strcpy(passwd, ap_data.key);
        *channel = ap_data.channel;
    }
    return ret;
}

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_erase_ap_net_data(void)
{
    int ret;
    ret = rda5981_erase_user_data(RDA5991H_USER_DATA_FLAG_APNET);
    return ret;
}

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_write_ap_net_data(unsigned int ip, unsigned int msk, unsigned int gw,
        unsigned int dhcps, unsigned int dhcpe)
{
    int ret = 0;
    wland_ap_net_data_t ap_net_data;

    WLAND_DBG(INFO, "Enter\r\n");

    memset(&ap_net_data, 0xff, sizeof(ap_net_data));
    ap_net_data.ip = ip;
    ap_net_data.msk = msk;
    ap_net_data.gw = gw;
    ap_net_data.dhcps = dhcps;
    ap_net_data.dhcpe = dhcpe;

    ret = rda5981_write_user_data((u8 *)&ap_net_data, sizeof(ap_net_data), RDA5991H_USER_DATA_FLAG_APNET);
    return ret;
}

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_read_ap_net_data(unsigned int *ip, unsigned int *msk, unsigned int *gw,
        unsigned int *dhcps, unsigned int *dhcpe)
{
    int ret = 0;
    wland_ap_net_data_t ap_net_data;

    WLAND_DBG(INFO, "Enter\r\n");

    ret = rda5981_read_user_data((u8 *)&ap_net_data, sizeof(ap_net_data), RDA5991H_USER_DATA_FLAG_APNET);
    if (ret == 0) {
        *ip = ap_net_data.ip;
        *msk = ap_net_data.msk;
        *gw = ap_net_data.gw;
        *dhcps = ap_net_data.dhcps;
        *dhcpe = ap_net_data.dhcpe;
    }
    return ret;
}


int rda5981_flash_read_pmk(u8 *pmk)
{
    WLAND_DBG(INFO, "Enter\r\n");
    return rda5981_read_user_data(pmk, 32, RDA5991H_USER_DATA_FLAG_PMK);
}

int rda5981_flash_read_ip_addr(u8 *ip_addr, u8 *server_addr)
{
    int ret;
    u8 buf[RDA5991H_USER_DATA_IP_LENGTH];
    WLAND_DBG(DEBUG, "Enter\r\n");
    ret = rda5981_read_user_data(buf, RDA5991H_USER_DATA_IP_LENGTH, RDA5991H_USER_DATA_FLAG_IP);
    if (ret) {
        WLAND_DBG(ERROR,  "read ip addr from flash fail\r\n");
    } else {
        memcpy(ip_addr, buf, 4);
        memcpy(server_addr, buf+4, 4);
        WLAND_DBG(INFO, "read ip:%u.%u.%u.%u\r\n", ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3]);
    }
    return ret;
}

int rda5981_flash_write_ip_addr(u8 *ip_addr, u8 *server_addr)
{
    u8 buf[RDA5991H_USER_DATA_IP_LENGTH];
    WLAND_DBG(INFO, "write:%u.%u.%u.%u\r\n", ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3]);
    memcpy(buf, ip_addr, 4);
    memcpy(buf+4, server_addr, 4);
    return rda5981_write_user_data(buf, RDA5991H_USER_DATA_IP_LENGTH, RDA5991H_USER_DATA_FLAG_IP);
}

int rda5981_flash_read_3rdparter_data_length()
{
    int ret;
    u32 data_len = 0;

    WLAND_DBG(INFO, "Enter\r\n");
    ret = rda5981_read_user_data((u8 *)(&data_len), 4, RDA5991H_USER_DATA_FLAG_PARTER_DATA_LEN);
    if (ret) {
        WLAND_DBG(ERROR,  "read parter data length from flash fail(%d)\r\n", ret);
        return -1;
    }
    return data_len;
}

int rda5981_flash_read_3rdparter_data(u8 *buf, u32 buf_len)
{
    int ret;
    u32 data_len = 0;

    WLAND_DBG(INFO, "Enter: %u\r\n", buf_len);
    ret = rda5981_read_user_data((u8 *)(&data_len), 4, RDA5991H_USER_DATA_FLAG_PARTER_DATA_LEN);
    if (ret) {
        WLAND_DBG(ERROR,  "read parter data length from flash fail(%d)\r\n", ret);
        return -1;
    }

    if (buf_len < data_len)
        WLAND_DBG(ERROR,  "The buf you prepared is to small(%u:%u)\r\n", buf_len, data_len);

    data_len = min(buf_len, data_len);

    ret = rda5981_read_flash(third_parter_data_location,
        (char *)(buf), data_len);
    if (ret) {
        WLAND_DBG(ERROR,  "read parter data from flash fail(%d)\r\n", ret);
        return -2;
    }

    return data_len;
}

int rda5981_flash_erase_3rdparter_data()
{
    return rda5981_erase_user_data(RDA5991H_USER_DATA_FLAG_PARTER_DATA_LEN);
}

int rda5981_flash_write_3rdparter_data(const u8 *buf, u32 buf_len)
{
    int ret;

    WLAND_DBG(INFO, "Enter: %u\r\n", buf_len);
    if (buf_len > third_parter_data_len) {
        WLAND_DBG(ERROR,  "buf too long(%u), we have only %x flash space\r\n",
            buf_len, third_parter_data_len);
        return -1;
    }

    rda5981_erase_flash(third_parter_data_location, buf_len);
    ret = rda5981_write_flash(third_parter_data_location,
        (char *)(buf), buf_len);
    if (ret) {
        WLAND_DBG(ERROR,  "write parter data to flash fail\r\n");
        return -2;
    }

    ret = rda5981_write_user_data((u8 *)(&buf_len), 4, RDA5991H_USER_DATA_FLAG_PARTER_DATA_LEN);
    if (ret) {
        WLAND_DBG(ERROR,  "write parter data length to flash fail\r\n");
        return -3;
    }
    return 0;
}

int rda5981_set_flash_size(const u32 size)
{

    WLAND_DBG(INFO, "Enter set flash size: %x\r\n", size);
    if (size == 0x100000 ||
        size == 0x200000 ||
        size == 0x400000) {
        flash_size = size;
        return 0;
    }

    return -1;
}

int rda5981_set_user_data_addr(const u32 sys_data_addr,
    const u32 user_data_addr, const u32 user_data_len)
{
    WLAND_DBG(INFO, "Enter set userdata addr: %x:%x:%x\r\n",
        sys_data_addr, user_data_addr, user_data_len);
    if ((sys_data_addr&(SECTOR_SIZE-1)) || (user_data_addr&(SECTOR_SIZE-1))) {
        return -1;
    }
    if (sys_data_addr == user_data_addr) {
        return -2;
    }

    if (sys_data_addr<=0x18001000 || user_data_addr<=0x18001000) {
        return -3;
    }
    if (sys_data_addr+0x1000 > 0x18000000+flash_size) {
        return -4;
    }
    if (user_data_addr+user_data_len > 0x18000000+flash_size) {
        return -5;
    }

    user_data_location = sys_data_addr;
    third_parter_data_location = user_data_addr;
    third_parter_data_len= user_data_len;
    return 0;
}

int rda5981_write_user_data_regs(u8 *reg, u8 *value, u32 flag)
{
    u16 reg16 = 0;
    u32 reg32 = 0;
    u8 idx = 0;

    if (!read_flag) {
        if (0 == rda5981_read_flash(user_data_location,
            (char *)(&wland_user_data), sizeof(wland_user_data))) {
            read_flag = 1;
        } else {
            return -1;
        }
    }

    if ((wland_user_data.flag & RDA5991H_USER_DATA_FLAG_UNINITIALIZED) ||
        strcmp((const char *)(wland_user_data.rda5981_flag), RDA5981_FLAG_FLAG)) {
        memset(&wland_user_data, 0xff, sizeof(wland_user_data));
        wland_user_data.flag = flag;
        strcpy((char *)(wland_user_data.rda5981_flag), RDA5981_FLAG_FLAG);
    }

    wland_user_data.flag |= flag;

    switch (flag) {
    case RDA5991H_USER_DATA_FLAG_RF:
        if (wland_user_data.rf.valid != 1) {
            wland_user_data.rf.valid = 1;
            wland_user_data.rf.flag = 0;
        }

        reg16 = *((u16 *)reg);
        if (wland_user_data.rf.flag != 0) {
            for (idx = 0; idx < 8; idx++) {
                if ((wland_user_data.rf.flag & BIT(idx)) &&
                    (wland_user_data.rf.reg_val[idx][0] == reg16)) {
                    wland_user_data.rf.reg_val[idx][1] = *((u16 *)value);
                    break;
                } else {
                    continue;
                }
            }
            if ((8 == idx) && (0xFF == wland_user_data.rf.flag))
                return -2;
        }

        if ((8 == idx) || (0 == wland_user_data.rf.flag)) {
            for (idx = 0; idx < 8; idx++) {
                if (!(wland_user_data.rf.flag & BIT(idx))) {
                    wland_user_data.rf.reg_val[idx][0] = reg16;
                    wland_user_data.rf.reg_val[idx][1] = *((u16 *)value);
                    wland_user_data.rf.flag |= BIT(idx);
                    break;
                } else {
                    continue;
                }
            }
        }

        break;
    case RDA5991H_USER_DATA_FLAG_RF_CHANNELS:
        if (wland_user_data.rf_channels.valid != 1) {
            wland_user_data.rf_channels.valid = 1;
            wland_user_data.rf_channels.flag = 0;
        }

        reg16 = *((u16 *)reg);
        if (wland_user_data.rf_channels.flag != 0) {
            for (idx = 0; idx < 8; idx++) {
                if ((wland_user_data.rf_channels.flag & BIT(idx)) &&
                    (wland_user_data.rf_channels.reg_val[idx][0] == reg16)) {
                    memcpy(&wland_user_data.rf_channels.reg_val[idx][1], value, 14 * sizeof(u16));
                    break;
                } else {
                    continue;
                }
            }
            if ((8 == idx) && (0xFF == wland_user_data.rf_channels.flag)) {
                return -2;
            }
        }

        if ((8 == idx) || (0 == wland_user_data.rf_channels.flag)) {
            for (idx = 0; idx < 8; idx++) {
                if (!(wland_user_data.rf_channels.flag & BIT(idx))) {
                    wland_user_data.rf_channels.reg_val[idx][0] = reg16;
                    memcpy(&wland_user_data.rf_channels.reg_val[idx][1], value, 14 * sizeof(u16));
                    wland_user_data.rf_channels.flag |= BIT(idx);
                    break;
                } else {
                    continue;
                }
            }
        }
        break;
    case RDA5991H_USER_DATA_FLAG_PHY:
        if (wland_user_data.phy.valid != 1) {
            wland_user_data.phy.valid = 1;
            wland_user_data.phy.flag = 0;
        }

        reg32 = *((u32 *)reg);
        if (wland_user_data.phy.flag != 0) {
            for (idx = 0; idx < 8; idx++) {
                if ((wland_user_data.phy.flag & BIT(idx)) &&
                    (wland_user_data.phy.reg_val[idx][0] == reg32)) {
                    wland_user_data.phy.reg_val[idx][1] = *((u32 *)value);
                    break;
                } else {
                    continue;
                }
            }
            if ((8 == idx) && (0xFF == wland_user_data.phy.flag)) {
                return -2;
            }
        }

        if ((8 == idx) || (0 == wland_user_data.phy.flag)) {
            for (idx = 0; idx < 8; idx++) {
                if (!(wland_user_data.phy.flag & BIT(idx))) {
                    wland_user_data.phy.reg_val[idx][0] = reg32;
                    wland_user_data.phy.reg_val[idx][1] = *((u32 *)value);
                    wland_user_data.phy.flag |= BIT(idx);
                    break;
                } else {
                    continue;
                }
            }
        }
        break;
    case RDA5991H_USER_DATA_FLAG_PHY_CHANNELS:
        if (wland_user_data.phy_channels.valid != 1) {
            wland_user_data.phy_channels.valid = 1;
            wland_user_data.phy_channels.flag = 0;
        }

        reg32 = *((u32 *)reg);
        if (wland_user_data.phy_channels.flag != 0) {
            for (idx = 0; idx < 8; idx++) {
                if ((wland_user_data.phy_channels.flag & BIT(idx)) &&
                    (wland_user_data.phy_channels.reg_val[idx][0] == reg32)) {
                    memcpy(&wland_user_data.phy_channels.reg_val[idx][1], value, 14 * sizeof(u32));
                    break;
                } else {
                    continue;
                }
            }
            if ((8 == idx) && (0xFF == wland_user_data.phy_channels.flag)) {
                return -2;
            }
        }

        if ((8 == idx) || (0 == wland_user_data.phy_channels.flag)) {
            for (idx = 0; idx < 8; idx++) {
                if (!(wland_user_data.phy_channels.flag & BIT(idx))) {
                    wland_user_data.phy_channels.reg_val[idx][0] = reg32;
                    memcpy(&wland_user_data.phy_channels.reg_val[idx][1], value, 14 * sizeof(u32));
                    wland_user_data.phy_channels.flag |= BIT(idx);
                    break;
                } else {
                    continue;
                }
            }
        }
        break;
    default:
        WLAND_DBG(ERROR, "Unknown flag:0x%08x\n", flag);
        return -3;
    }

    rda5981_erase_flash(user_data_location, sizeof(wland_user_data));
    return rda5981_write_flash(user_data_location,
        (char *)(&wland_user_data), sizeof(wland_user_data));
}

int rda5981_erase_user_data_regs(u8 *reg, u32 flag)
{
    u16 reg16 = 0;
    u32 reg32 = 0;
    u8 idx = 0;

    if (!read_flag) {
        if (0 == rda5981_read_flash(user_data_location,
            (char *)(&wland_user_data), sizeof(wland_user_data))) {
            read_flag = 1;
        } else {
            return -1;
        }
    }

    if (wland_user_data.flag & RDA5991H_USER_DATA_FLAG_UNINITIALIZED) {//flash init is 0xffffffff
        return -1;
    }
    if ((wland_user_data.flag & flag) == 0) {
        return 0;
    }
    switch (flag) {
    case RDA5991H_USER_DATA_FLAG_RF:
        if (wland_user_data.rf.valid != 1) {
            return 0;
        }

        reg16 = *((u16 *)reg);
        if (wland_user_data.rf.flag != 0) {
            for (idx = 0; idx < 8; idx++) {
                if ((wland_user_data.rf.flag & BIT(idx)) &&
                    (wland_user_data.rf.reg_val[idx][0] == reg16)) {
                    memset(&wland_user_data.rf.reg_val[idx][0], 0xFF, 2 * sizeof(u16));
                    wland_user_data.rf.flag &= ~BIT(idx);
                    break;
                } else {
                    continue;
                }
            }
        }

        if (0 == wland_user_data.rf.flag) {
            wland_user_data.rf.valid = 0xFFFFFFFF;
            wland_user_data.rf.flag = 0xFFFFFFFF;
            wland_user_data.flag &= (~flag);
        }

        break;
    case RDA5991H_USER_DATA_FLAG_RF_CHANNELS:
        if (wland_user_data.rf_channels.valid != 1) {
            return 0;
        }

        reg16 = *((u16 *)reg);
        if (wland_user_data.rf_channels.flag != 0) {
            for (idx = 0; idx < 8; idx++) {
                if ((wland_user_data.rf_channels.flag & BIT(idx)) &&
                    (wland_user_data.rf_channels.reg_val[idx][0] == reg16)) {
                    memset(&wland_user_data.rf_channels.reg_val[idx][0], 0xFF, 15 * sizeof(u16));
                    wland_user_data.rf_channels.flag &= ~BIT(idx);
                    break;
                } else {
                    continue;
                }
            }
        }

        if (0 == wland_user_data.rf_channels.flag) {
            wland_user_data.rf_channels.valid = 0xFFFFFFFF;
            wland_user_data.rf_channels.flag = 0xFFFFFFFF;
            wland_user_data.flag &= (~flag);
        }

        break;
    case RDA5991H_USER_DATA_FLAG_PHY:
        if (wland_user_data.phy.valid != 1) {
            return 0;
        }

        reg32 = *((u32 *)reg);
        if (wland_user_data.phy.flag != 0) {
            for (idx = 0; idx < 8; idx++) {
                if ((wland_user_data.phy.flag & BIT(idx)) &&
                    (wland_user_data.phy.reg_val[idx][0] == reg32)) {
                    memset(&wland_user_data.phy.reg_val[idx][0], 0xFF, 2 * sizeof(u32));
                    wland_user_data.phy.flag &= ~BIT(idx);
                    break;
                } else {
                    continue;
                }
            }
        }

        if (0 == wland_user_data.phy.flag) {
            wland_user_data.phy.valid = 0xFFFFFFFF;
            wland_user_data.phy.flag = 0xFFFFFFFF;
            wland_user_data.flag &= (~flag);
        }

        break;
    case RDA5991H_USER_DATA_FLAG_PHY_CHANNELS:
        if (wland_user_data.phy_channels.valid != 1) {
            return 0;
        }

        reg32 = *((u32 *)reg);
        if (wland_user_data.phy_channels.flag != 0) {
            for (idx = 0; idx < 8; idx++) {
                if ((wland_user_data.phy_channels.flag & BIT(idx)) &&
                    (wland_user_data.phy_channels.reg_val[idx][0] == reg32)) {
                    memset(&wland_user_data.phy_channels.reg_val[idx][0], 0xFF, 15 * sizeof(u32));
                    wland_user_data.phy_channels.flag &= ~BIT(idx);
                    break;
                } else {
                    continue;
                }
            }
        }

        if (0 == wland_user_data.phy_channels.flag) {
            wland_user_data.phy_channels.valid = 0xFFFFFFFF;
            wland_user_data.phy_channels.flag = 0xFFFFFFFF;
            wland_user_data.flag &= (~flag);
        }

        break;
    default:
        WLAND_DBG(ERROR, "Unknown flag:0x%08x\n", flag);
        return -3;
    }

    rda5981_erase_flash(user_data_location, sizeof(wland_user_data));
    return rda5981_write_flash(user_data_location,
        (char *)(&wland_user_data), sizeof(wland_user_data));
}


int rda5981_read_user_data_regs(u8 *reg, u8 *value, u32 flag)
{
    u16 reg16 = 0;
    u32 reg32 = 0;
    u8 idx = 0;

    if (!read_flag) {
        if(0 == rda5981_read_flash(user_data_location,
            (char *)(&wland_user_data), sizeof(wland_user_data))) {
            read_flag = 1;
        } else {
            return -1;
        }
    }

    if (wland_user_data.flag & RDA5991H_USER_DATA_FLAG_UNINITIALIZED) {//flash init is 0xffffffff
        return -1;
    }
    if (strcmp((const char *)(wland_user_data.rda5981_flag), RDA5981_FLAG_FLAG)) {
        return -2;
    }
    if ((wland_user_data.flag & flag) == 0) {
        return -3;
    }
    switch (flag) {
    case RDA5991H_USER_DATA_FLAG_RF:
        if (wland_user_data.rf.valid != 1) {
            return -4;
        }

        reg16 = *((u16 *)reg);
        for (idx = 0; idx < 8; idx++) {
            if ((wland_user_data.rf.flag & BIT(idx)) &&
                (wland_user_data.rf.reg_val[idx][0] == reg16)) {
                memcpy(value, &wland_user_data.rf.reg_val[idx][1], sizeof(u16));
                break;
            } else {
                continue;
            }
        }

        if (8 == idx) {
            return -4;
        }
        break;
    case RDA5991H_USER_DATA_FLAG_RF_CHANNELS:
        if (wland_user_data.rf_channels.valid != 1) {
            return -4;
        }

        reg16 = *((u16 *)reg);
        if (wland_user_data.rf_channels.flag != 0) {
            for (idx = 0; idx < 8; idx++) {
                if ((wland_user_data.rf_channels.flag & BIT(idx)) &&
                    (wland_user_data.rf_channels.reg_val[idx][0] == reg16)) {
                    memcpy(value, &wland_user_data.rf_channels.reg_val[idx][1], 14 * sizeof(u16));
                    break;
                } else {
                    continue;
                }
            }
        }

        if (8 == idx) {
            return -4;
        }
        break;
    case RDA5991H_USER_DATA_FLAG_PHY:
        if (wland_user_data.phy.valid != 1) {
            return -4;
        }

        reg32 = *((u32 *)reg);
        if (wland_user_data.phy.flag != 0) {
            for (idx = 0; idx < 8; idx++) {
                if ((wland_user_data.phy.flag & BIT(idx)) &&
                    (wland_user_data.phy.reg_val[idx][0] == reg32)) {
                    memcpy(value, &wland_user_data.phy.reg_val[idx][1], sizeof(u32));
                    break;
                } else {
                    continue;
                }
            }
        }

        if (8 == idx) {
            return -4;
        }
        break;
    case RDA5991H_USER_DATA_FLAG_PHY_CHANNELS:
        if (wland_user_data.phy_channels.valid != 1) {
            return -4;
        }

        reg32 = *((u32 *)reg);
        if (wland_user_data.phy_channels.flag != 0) {
            for (idx = 0; idx < 8; idx++) {
                if ((wland_user_data.phy_channels.flag & BIT(idx)) &&
                    (wland_user_data.phy_channels.reg_val[idx][0] == reg32)) {
                    memcpy(value, &wland_user_data.phy_channels.reg_val[idx][1], 14 * sizeof(u32));
                    break;
                } else {
                    continue;
                }
            }
        }

        if (8 == idx) {
            return -4;
        }
        break;
    default:
        WLAND_DBG(ERROR, "Unknown flag:0x%08x\n", flag);
        return -3;
    }

    return 0;
}

int rda5981_flash_get_mid(u8 *mid)
{
    *mid = 0xC8;

    return 0;
}

int rda5981_flash_get_did(u8 *did)
{
    *did = 0x13;

    return 0;
}

int rda5981_flash_get_jdid(u16 *jdid)
{
    *jdid = 0x4014;

    return 0;
}

int rda5981_read_default_config(char *buf, u32 len, u32 flag)
{
    int ret = 0;
    u32 addr;
    u32 addr_offset = 0;
    char *temp_buf = NULL, *temp_buf_aligned;

    switch (flag) {
    case RDA5981_VBAT_CAL:
        addr = 0x18000088;
        break;
    case RDA5981_GPADC0_CAL:
    case RDA5981_GPADC1_CAL:
        addr = 0x1800008C;
        break;
    case RDA5981_PRODUCT_ID:
        addr = 0x18000084;
        break;
    case RDA5981_POWER_CLASS:
        addr = 0x18000085;
        break;
    default:
        WLAND_DBG(ERROR, "Unknown flag\n");
        return -1;
    }

    addr_offset = addr % 4;
    addr = (addr - addr_offset) & (flash_size -1);

    if (((u32)buf % 4) || (addr_offset != 0)) {
        temp_buf = (char *)malloc(addr_offset + len + 4);
        if (temp_buf == NULL) {
            ret = -1;
            goto out;
        }
        if ((u32)temp_buf % 4) {
            temp_buf_aligned = temp_buf + (4 - (u32)temp_buf % 4);
        } else {
            temp_buf_aligned = temp_buf;
        }
    } else {
        temp_buf_aligned = buf;
    }

    core_util_critical_section_enter();
    spi_flash_flush_cache();
    SPI_FLASH_READ_DATA_FOR_MBED((void *)addr, temp_buf_aligned, (len + addr_offset));
    core_util_critical_section_exit();

    if (temp_buf_aligned != buf) {
        memcpy(buf, temp_buf_aligned + addr_offset, len);
    }
out:
    if (temp_buf) {
        free(temp_buf);
    }
    return ret;
}

int rda5981_flash_read_vbat_cal(float *k, float *b)
{
    int ret = 0;
    u32 value = 0;
    u32 x1 = 0;//y1 3.0f
    u32 x2 = 0;//y2 4.2f
    float k_tmp = 0;
    float b_tmp = 0;

    ret = rda5981_read_default_config((char *)&value, 4, RDA5981_VBAT_CAL);

    if (ret < 0) {
        WLAND_DBG(ERROR, "read vbat_cal form flash fail\n");
        return -1;
    }
    if ((0 == (value & 0xFFFFFUL)) || (0xFFFFFUL == (value & 0xFFFFFUL))) {
        WLAND_DBG(ERROR, "invalid vbat_cal:0x%08x\n", value);
        return -1;
    } else {
        x1 = value & 0x3FFUL;
        x2 = (value >> 10) & 0x3FFUL;
    }

    if (x1 == x2) {
        return -1;
    }

    k_tmp = (4.2f - 3.0f) / (float)(x2 - x1);
    b_tmp = 4.2f - k_tmp * x2;

    *k = k_tmp;
    *b = b_tmp;

    return ret;
}
