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

#ifndef _WLAND_FLASH_H_
#define _WLAND_FLASH_H_
#include "wland_types.h"
#include "rda5981_flash.h"
#include "rda_ccfg_api.h"

//#define FLASH_READ_CHECK

#ifndef min
#define min(a, b) ((a)<(b)?(a):(b))
#endif /*min*/

/** This struct contains all smartconfig mbed flash parameter. */
typedef struct
{
    char ssid[36];
    char key[64];
} wland_sta_data_t;

/** This struct contains ap data, include ssid key and channel. */
typedef struct
{
    u8 channel;
    char ssid[35];
    char key[64];
} wland_ap_data_t;

/** This struct contains ap net data. */
typedef struct
{
    u32 ip;
    u32 msk;
    u32 gw;
    u32 dhcps;
    u32 dhcpe;
} wland_ap_net_data_t;

typedef struct
{
    u32 fixip;
    u32 ip;
    u32 msk;
    u32 gw;
} wland_dhcp_t;

#define RDA5991H_USER_DATA_FLAG_UNINITIALIZED (0xFFFFFFFF & \
    (~(RDA5991H_USER_DATA_FLAG_MAC | \
    RDA5991H_USER_DATA_FLAG_STA | \
    RDA5991H_USER_DATA_FLAG_PMK | \
    RDA5991H_USER_DATA_FLAG_IP | \
    RDA5991H_USER_DATA_FLAG_PARTER_DATA_LEN | \
    RDA5991H_USER_DATA_FLAG_TX_POWER | \
    RDA5991H_USER_DATA_FLAG_XTAL_CAL | \
    RDA5991H_USER_DATA_FLAG_TX_POWER_RF | \
    RDA5991H_USER_DATA_FLAG_TX_POWER_PHY_GN | \
    RDA5991H_USER_DATA_FLAG_TX_POWER_PHY_B | \
    RDA5991H_USER_DATA_FLAG_AP | \
    RDA5991H_USER_DATA_FLAG_APNET | \
    RDA5991H_USER_DATA_FLAG_DHCP | \
    RDA5991H_USER_DATA_FLAG_UART | \
    RDA5991H_USER_DATA_FLAG_RF | \
    RDA5991H_USER_DATA_FLAG_RF_CHANNELS | \
    RDA5991H_USER_DATA_FLAG_PHY | \
    RDA5991H_USER_DATA_FLAG_PHY_CHANNELS | \
    RDA5991H_USER_DATA_FLAG_TX_POWER_OFFSET)))

#define RDA5991H_USER_DATA_IP_LENGTH 8

#define RDA5981_FLAG_FLAG "RDA5981"

typedef struct
{
    u32 flag;
    u8 rda5981_flag[8];
    u8 mac_addr[6];
    u8 tp_offset;
    u8 padding1;//alignment
    wland_sta_data_t sta_data;
    u8 pmk[32];
    u8 ip[RDA5991H_USER_DATA_IP_LENGTH];
    u32 parter_data_len;
    wland_tx_power_t tx_power;
    u16 xtal_cal;
    u8 padding2[2];//alignment
    u16 tx_power_rf[14];
    u8 tx_power_phy_gn;
    u8 tx_power_phy_b;
    u8 padding3[2];
    wland_ap_data_t ap_data;
    wland_ap_net_data_t ap_net_data;
    wland_dhcp_t dhcp;
    u32 uart;
    wland_rf_t rf;
    wland_rf_channels_t rf_channels;
    wland_phy_t phy;
    wland_phy_channels_t phy_channels;
}rda5991h_user_data;

#define SECTOR_SIZE     4096
#define FLASH_SIZE 0x100000

#define RDA5991H_PARTITION_TABLE_END_ADDR 0x18001000 //partition table end addr

#define RDA5991H_USER_DATA_ADDR 0x180fb000 //4k(partition table)+500k+500k:
#define RDA5991H_3RDPARTER_DATA_ADDR 0x180fc000
#define RDA5991H_3RDPARTER_DATA_LEN 0x1000

#define FLASH_CTL_REG_BASE 0x17fff000
#define FLASH_CTL_TX_CMD_ADDR_REG (FLASH_CTL_REG_BASE + 0x00)
#define CMD_64KB_BLOCK_ERASE (0x000000d8UL)
#define WRITE_REG32(REG, VAL)    ((*(volatile unsigned int*)(REG)) = (unsigned int)(VAL))

#define FLASH_ERASE_FUN_ADDR 0x21f1//smartlink_erase_for_mbed
#define FLASH_WRITE_FUN_ADDR 0x2241//smartlink_write_for_mbed
#define FLASH_READ_FUN_ADDR 0x2243//smartlink_read_for_mbed
#define FLASH_ERASE_PARTITION_FUN_ADDR 0x2139//spi_flash_erase_partition
#define SPI_FLASH_READ_DATA_FOR_MBED_ADDR 0x2007//spi_flash_read_data_for_mbed
#define spi_flash_disable_cache_addr 0x1eb7//spi_flash_disable_cache
#define spi_flash_flush_cache_addr 0x1ecd//spi_flash_flush_cache
#define spi_flash_cfg_cache_addr 0x1e9f//spi_flash_cfg_cache
#define spi_flash_erase_4KB_sector_addr 0x23a3
#define spi_wip_reset_addr 0x1d8b
#define spi_write_reset_addr 0x1d9f
#define wait_busy_down_addr 0x1d81

#define FLASH_ERASE_FUN_ADDR_4 0x2221//smartlink_erase_for_mbed
#define FLASH_WRITE_FUN_ADDR_4 0x2271//smartlink_write_for_mbed
#define FLASH_READ_FUN_ADDR_4 0x2273//smartlink_read_for_mbed
#define FLASH_ERASE_PARTITION_FUN_ADDR_4 0x2169//spi_flash_erase_partition
#define SPI_FLASH_READ_DATA_FOR_MBED_ADDR_4 0x2037//spi_flash_read_data_for_mbed
#define spi_flash_disable_cache_addr_4 0x1ee7//spi_flash_disable_cache
#define spi_flash_flush_cache_addr_4 0x1efd//spi_flash_flush_cache
#define spi_flash_cfg_cache_addr_4 0x1ecf//spi_flash_cfg_cache
#define spi_flash_erase_4KB_sector_addr_4 0x23d3
#define spi_wip_reset_addr_4 0x1dbb
#define spi_write_reset_addr_4 0x1dcf
#define wait_busy_down_addr_4 0x1db1

/*
 * return 0 if find
 */
int rda5981_flash_read_pmk(u8 *pmk);
int rda5981_flash_read_sta_data(char *ssid, char *passwd);
void rda5981_spi_erase_partition(void *src, u32 counts);

//@len must be 4k aligment
//int rda5981_write_flash(u32 addr, char *buf, u32 len);
//int rda5981_read_flash(u32 addr, char *buf, u32 len);

#define VERSION_SZ          24
struct firmware_info
{
    u32 magic;
    u8 version[VERSION_SZ];

    u32 addr;
    u32 size;
    u32 crc32;
    u32 bootaddr;//add for rf_test
    u32 bootmagic;
};

#define RDA5981_FIRMWARE_MAGIC      0xEAEA

static inline void wait_busy_down_2(void)
{
    ((void(*)(void))wait_busy_down_addr)();
}

static inline void spi_write_reset_2(void)
{
    ((void(*)(void))spi_write_reset_addr)();
}

static inline void spi_wip_reset_2(void)
{
    ((void(*)(void))spi_wip_reset_addr)();
}

static inline void wait_busy_down_4(void)
{
    ((void(*)(void))wait_busy_down_addr_4)();
}

static inline void spi_write_reset_4(void)
{
    ((void(*)(void))spi_write_reset_addr_4)();
}

static inline void spi_wip_reset_4(void)
{
    ((void(*)(void))spi_wip_reset_addr_4)();
}

static void wait_busy_down(void)
{
    if (rda_ccfg_hwver() >= 4) {
        wait_busy_down_4();
    } else {
        wait_busy_down_2();
    }
}

static void spi_write_reset(void)
{
    if (rda_ccfg_hwver() >= 4) {
        spi_write_reset_4();
    } else {
        spi_write_reset_2();
    }
}

static void spi_wip_reset(void)
{
    if (rda_ccfg_hwver() >= 4) {
        spi_wip_reset_4();
    } else {
        spi_wip_reset_2();
    }
}

static inline void spi_flash_enable_cache(void)
{
    unsigned int func = spi_flash_cfg_cache_addr;
    if (rda_ccfg_hwver() >= 4) {
        func = spi_flash_cfg_cache_addr_4;
    }
    ((void(*)(void))func)();
}

static inline void spi_flash_disable_cache(void)
{
    unsigned int func = spi_flash_disable_cache_addr;
    if (rda_ccfg_hwver() >= 4) {
        func = spi_flash_disable_cache_addr_4;
    }
    ((void(*)(void))func)();
}

static inline void spi_flash_flush_cache(void)
{
    unsigned int func = spi_flash_flush_cache_addr;
    if (rda_ccfg_hwver() >= 4) {
        func = spi_flash_flush_cache_addr_4;
    }
    ((void(*)(void))func)();
}

static inline void rda5981_spi_flash_erase_4KB_sector(u32 addr)
{
    unsigned int func = spi_flash_erase_4KB_sector_addr;
    if (rda_ccfg_hwver() >= 4) {
        func = spi_flash_erase_4KB_sector_addr_4;
    }
    ((void(*)(u32))func)(addr);
}

static inline void RDA5991H_ERASE_FLASH(void *addr, u32 len)
{
    unsigned int func = FLASH_ERASE_FUN_ADDR;
    if (rda_ccfg_hwver() >= 4) {
        func = FLASH_ERASE_FUN_ADDR_4;
    }
    ((void(*)(void *, u32))func)(addr, len);
}

static inline void RDA5991H_WRITE_FLASH(u32 addr, u8 *data, u32 len)
{
    unsigned int func = FLASH_WRITE_FUN_ADDR;
    if (rda_ccfg_hwver() >= 4) {
        func = FLASH_WRITE_FUN_ADDR_4;
    }
    ((void(*)(u32, u8 *, u32))func)(addr, data, len);
}

static inline void RDA5991H_READ_FLASH(u32 addr, u8 *buf, u32 len)
{
    unsigned int func = FLASH_READ_FUN_ADDR;
    if (rda_ccfg_hwver() >= 4) {
        func = FLASH_READ_FUN_ADDR_4;
    }
    ((void(*)(u32, u8 *, u32))func)(addr, buf, len);
}

static inline void SPI_FLASH_READ_DATA_FOR_MBED(void *addr, void *buf, u32 len)
{
    unsigned int func = SPI_FLASH_READ_DATA_FOR_MBED_ADDR;
    if (rda_ccfg_hwver() >= 4) {
        func = SPI_FLASH_READ_DATA_FOR_MBED_ADDR_4;
    }
    ((void(*)(void *, void *, u32))func)(buf, addr, len);
}

#endif /*_WLAND_FLASH_H_*/

