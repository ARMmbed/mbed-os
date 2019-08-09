// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// · Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// · Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// · Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#ifndef __TARGADDRS_H__
#define __TARGADDRS_H__

#include "bmi_msg.h"

/*
 * AR6K option bits, to enable/disable various features.
 * By default, all option bits are 0.
 * These bits can be set in LOCAL_SCRATCH register 0.
 */
#define AR6K_OPTION_BMI_DISABLE 0x01   /* Disable BMI comm with Host */
#define AR6K_OPTION_SERIAL_ENABLE 0x02 /* Enable serial port msgs */
#define AR6K_OPTION_WDT_DISABLE 0x04   /* WatchDog Timer override */
#define AR6K_OPTION_SLEEP_DISABLE 0x08 /* Disable system sleep */
#define AR6K_OPTION_STOP_BOOT 0x10     /* Stop boot processes (for ATE) */
#define AR6K_OPTION_ENABLE_NOANI 0x20  /* Operate without ANI */
#define AR6K_OPTION_DSET_DISABLE 0x40  /* Ignore DataSets */
#define AR6K_OPTION_IGNORE_FLASH 0x80  /* Ignore flash during bootup */

/*
 * xxx_HOST_INTEREST_ADDRESS is the address in Target RAM of the
 * host_interest structure.  It must match the address of the _host_interest
 * symbol (see linker script).
 *
 * Host Interest is shared between Host and Target in order to coordinate
 * between the two, and is intended to remain constant (with additions only
 * at the end) across software releases.
 *
 * All addresses are available here so that it's possible to
 * write a single binary that works with all Target Types.
 * May be used in assembler code as well as C.
 */

#define AR4100_HOST_INTEREST_ADDRESS 0x00540600
#define AR4002_HOST_INTEREST_ADDRESS 0x00428800

#if ATH_FIRMWARE_TARGET == TARGET_AR4100_REV2
#define HOST_INTEREST ((uint32_t *)AR4100_HOST_INTEREST_ADDRESS)
#define HOST_INTEREST_MAX_SIZE 0x100
#elif ATH_FIRMWARE_TARGET == TARGET_AR400X_REV1
#define HOST_INTEREST ((uint32_t *)AR4002_HOST_INTEREST_ADDRESS)
#define HOST_INTEREST_MAX_SIZE 0x200
#endif

#if !defined(__ASSEMBLER__)
struct register_dump_s;
struct dbglog_hdr_s;

/*
 * These are items that the Host may need to access
 * via BMI or via the Diagnostic Window. The position
 * of items in this structure must remain constant
 * across firmware revisions!
 *
 * Types for each item must be fixed size across
 * target and host platforms.
 *
 * More items may be added at the end.
 */
struct host_interest_s
{
    /*
     * Pointer to application-defined area, if any.
     * Set by Target application during startup.
     */
    uint32_t hi_app_host_interest; /* 0x00 */

    /* Pointer to register dump area, valid after Target crash. */
    uint32_t hi_failure_state; /* 0x04 */

    /* Pointer to debug logging header */
    uint32_t hi_dbglog_hdr; /* 0x08 */

    /* Indicates whether or not flash is present on Target.
     * NB: flash_is_present indicator is here not just
     * because it might be of interest to the Host; but
     * also because it's set early on by Target's startup
     * asm code and we need it to have a special RAM address
     * so that it doesn't get reinitialized with the rest
     * of data.
     */
    uint32_t hi_flash_is_present; /* 0x0c */

    /*
     * General-purpose flag bits, similar to AR6000_OPTION_* flags.
     * Can be used by application rather than by OS.
     */
    uint32_t hi_option_flag; /* 0x10 */

    /*
     * Boolean that determines whether or not to
     * display messages on the serial port.
     */
    uint32_t hi_serial_enable; /* 0x14 */

    /* Start address of Flash DataSet index, if any */
    uint32_t hi_dset_list_head; /* 0x18 */

    /* Override Target application start address */
    uint32_t hi_app_start; /* 0x1c */

    /* Clock and voltage tuning */
    uint32_t hi_skip_clock_init;          /* 0x20 */
    uint32_t hi_core_clock_setting;       /* 0x24 */
    uint32_t hi_cpu_clock_setting;        /* 0x28 */
    uint32_t hi_system_sleep_setting;     /* 0x2c */
    uint32_t hi_xtal_control_setting;     /* 0x30 */
    uint32_t hi_pll_ctrl_setting_24ghz;   /* 0x34 */
    uint32_t hi_pll_ctrl_setting_5ghz;    /* 0x38 */
    uint32_t hi_ref_voltage_trim_setting; /* 0x3c */
    uint32_t hi_clock_info;               /* 0x40 */

    /*
     * Flash configuration overrides, used only
     * when firmware is not executing from flash.
     * (When using flash, modify the global variables
     * with equivalent names.)
     */
    uint32_t hi_bank0_addr_value;   /* 0x44 */
    uint32_t hi_bank0_read_value;   /* 0x48 */
    uint32_t hi_bank0_write_value;  /* 0x4c */
    uint32_t hi_bank0_config_value; /* 0x50 */

    /* Pointer to Board Data  */
    uint32_t hi_board_data; /* 0x54 */

    /*
     * Indication of Board Data state:
     *    0: board data is not yet initialized.
     *    1: board data is initialized; unknown size
     *   >1: number of bytes of initialized board data (varies with board type)
     */
    uint32_t hi_board_data_initialized; /* 0x58 */

    uint32_t hi_dset_RAM_index_table; /* 0x5c */

    uint32_t hi_desired_baud_rate;  /* 0x60 */
    uint32_t hi_dbglog_config;      /* 0x64 */
    uint32_t hi_end_RAM_reserve_sz; /* 0x68 */
    uint32_t hi_mbox_io_block_sz;   /* 0x6c */

    uint32_t hi_num_bpatch_streams;   /* 0x70 -- unused */
    uint32_t hi_mbox_isr_yield_limit; /* 0x74 */

    uint32_t hi_refclk_hz;                /* 0x78 */
    uint32_t hi_ext_clk_detected;         /* 0x7c */
    uint32_t hi_dbg_uart_txpin;           /* 0x80 */
    uint32_t hi_dbg_uart_rxpin;           /* 0x84 */
    uint32_t hi_hci_uart_baud;            /* 0x88 */
    uint32_t hi_hci_uart_pin_assignments; /* 0x8C */
    /* NOTE: byte [0] = tx pin, [1] = rx pin, [2] = rts pin, [3] = cts pin */
    uint32_t hi_hci_uart_baud_scale_val; /* 0x90 */
    uint32_t hi_hci_uart_baud_step_val;  /* 0x94 */

    uint32_t hi_allocram_start;        /* 0x98 */
    uint32_t hi_allocram_sz;           /* 0x9c */
    uint32_t hi_hci_bridge_flags;      /* 0xa0 */
    uint32_t hi_hci_uart_support_pins; /* 0xa4 */
    /* NOTE: byte [0] = RESET pin (bit 7 is polarity), bytes[1]..bytes[3] are for future use */
    uint32_t hi_hci_uart_pwr_mgmt_params; /* 0xa8 */
                                          /* 0xa8 - [0]: 1 = enable, 0 = disable
                                           *        [1]: 0 = UART FC active low, 1 = UART FC active high
                                           * 0xa9 - [7:0]: wakeup timeout in ms
                                           * 0xaa, 0xab - [15:0]: idle timeout in ms
                                           */
    /* Pointer to extended board Data  */
    uint32_t hi_board_ext_data;        /* 0xac */
    uint32_t hi_board_ext_data_config; /* 0xb0 */
                                       /*
                                        * Bit [0]  :   valid
                                        * Bit[31:16:   size
                                        */
    /*
      * hi_reset_flag is used to do some stuff when target reset.
      * such as restore app_start after warm reset or
      * preserve host Interest area, or preserve ROM data, literals etc.
      */
    uint32_t hi_reset_flag; /* 0xb4 */
    /* indicate hi_reset_flag is valid */
    uint32_t hi_reset_flag_valid;             /* 0xb8 */
    uint32_t hi_hci_uart_pwr_mgmt_params_ext; /* 0xbc */
                                              /* 0xbc - [31:0]: idle timeout in ms
                                               */
    /* ACS flags */
    uint32_t hi_acs_flags;     /* 0xc0 */
    uint32_t hi_console_flags; /* 0xc4 */
    uint32_t hi_nvram_state;   /* 0xc8 */
    uint32_t hi_option_flag2;  /* 0xcc */

    /* If non-zero, override values sent to Host in WMI_READY event. */
    uint32_t hi_sw_version_override;  /* 0xd0 */
    uint32_t hi_abi_version_override; /* 0xd4 */
                                      /* Percentage of high priority RX traffic to total expected RX traffic -
                                       * applicable only to ar6004 */
    uint32_t hi_hp_rx_traffic_ratio;  /* 0xd8 */

    /* test applications flags */
    uint32_t hi_test_apps_related; /* 0xdc */
    /* location of test script */
    uint32_t hi_ota_testscript; /* 0xe0 */
    /* location of CAL data */
    uint32_t hi_cal_data; /* 0xe4 */

    /* Number of packet log buffers */
    uint32_t hi_pktlog_num_buffers; /* 0xe8 */

    /* wow extension configuration */
    uint32_t hi_wow_ext_config; /* 0xec */

    uint32_t hi_pwr_save_flags; /*  0xf0 */

    /* Spatial Multiplexing Power Save (SMPS) options */
    uint32_t hi_smps_options; /* 0xf4 */
    /* rx coalescing filter configuration */
    uint32_t hi_rx_coalesce_filter_config; /* 0xf8 */

    uint32_t hi_bt_sharing_info; /* 0xfc */

#if (HOST_INTEREST_MAX_SIZE > 0x100)

    uint32_t hi_i2c_slv_cfg; /* 0x100 */

    uint32_t hi_reserved_3; /* 0x104*/
    uint32_t hi_reserved_4; /* 0x108 */
    uint32_t hi_reserved_5; /* 0x10c */
    uint32_t hi_reserved_6; /* 0x110 */

    /* RTOS-specific pointer-sized value */
    uint32_t hi_rtos; /* 0x114 */

    /* Extending General purpose Flags */
    uint32_t hi_option_flag3; /* 0x118 */
#endif                        /*HOST_INTEREST_MAX_SIZE*/
};

/* bitmap for hi_test_apps_related */
#define HI_TEST_APPS_TESTSCRIPT_LOADED 0x00000001
#define HI_TEST_APPS_CAL_DATA_AVAIL 0x00000002
#define HI_TEST_APPS_ENABLE_WHCK_LOG 0x00000004

/* Bits defined in hi_option_flag */
#define HI_OPTION_TIMER_WAR 0x01         /* Enable timer workaround */
#define HI_OPTION_BMI_CRED_LIMIT 0x02    /* Limit BMI command credits */
#define HI_OPTION_RELAY_DOT11_HDR 0x04   /* Relay Dot11 hdr to/from host */
#define HI_OPTION_MAC_ADDR_METHOD 0x08   /* MAC addr method 0-locally administred 1-globally unique addrs */
#define HI_OPTION_FW_BRIDGE 0x10         /* Firmware Bridging */
#define HI_OPTION_ENABLE_PROFILE 0x20    /* Enable CPU profiling */
#define HI_OPTION_DISABLE_DBGLOG 0x40    /* Disable debug logging */
#define HI_OPTION_SKIP_ERA_TRACKING 0x80 /* Skip Era Tracking */
#define HI_OPTION_PAPRD_DISABLE 0x100    /* Disable PAPRD (debug) */
#define HI_OPTION_NUM_DEV_LSB 0x200
#define HI_OPTION_NUM_DEV_MSB 0x800
#define HI_OPTION_DEV_MODE_LSB 0x1000
#define HI_OPTION_ATTUNNEL_MODE 0x2000
#define HI_OPTION_DEV_MODE_MSB 0x8000000
#define HI_OPTION_NO_LFT_STBL 0x10000000   /* Disable LowFreq Timer Stabilization */
#define HI_OPTION_SKIP_REG_SCAN 0x20000000 /* Skip regulatory scan */
#define HI_OPTION_INIT_REG_SCAN                                                   \
    0x40000000                           /* Do regulatory scan during init before \
                                          * sending WMI ready event to host */
#define HI_OPTION_SKIP_MEMMAP 0x80000000 /* REV6: Do not adjust memory map */

#define HI_OPTION_MAC_ADDR_METHOD_SHIFT 3

/* 2 bits of hi_option_flag are used to represent 3 modes */
#define HI_OPTION_FW_MODE_IBSS 0x0    /* IBSS Mode */
#define HI_OPTION_FW_MODE_BSS_STA 0x1 /* STA Mode */
#define HI_OPTION_FW_MODE_AP 0x2      /* AP Mode */
#define HI_OPTION_FW_MODE_BT30AMP 0x3 /* BT30 AMP Mode */

/* 2 bits of hi_option flag are usedto represent 4 submodes */
#define HI_OPTION_FW_SUBMODE_NONE 0x0      /* Normal mode */
#define HI_OPTION_FW_SUBMODE_P2PDEV 0x1    /* p2p device mode */
#define HI_OPTION_FW_SUBMODE_P2PCLIENT 0x2 /* p2p client mode */
#define HI_OPTION_FW_SUBMODE_P2PGO 0x3     /* p2p go mode */

/* Num dev Mask */
#define HI_OPTION_NUM_DEV_MASK 0x7
#define HI_OPTION_NUM_DEV_SHIFT 0x9

/* firmware bridging */
#define HI_OPTION_FW_BRIDGE_SHIFT 0x04

/* Fw Mode/SubMode Mask
|-------------------------------------------------------------------------------|
|   SUB   |   SUB   |   SUB   |  SUB    |         |         |         |         |
| MODE[3] | MODE[2] | MODE[1] | MODE[0] | MODE[3] | MODE[2] | MODE[1] | MODE[0] |
|   (2)   |   (2)   |   (2)   |   (2)   |   (2)   |   (2)   |   (2)   |   (2)   |
|-------------------------------------------------------------------------------|
*/
#define HI_OPTION_FW_MODE_BITS 0x2
#define HI_OPTION_FW_MODE_MASK 0x3
#define HI_OPTION_FW_MODE_SHIFT 0xC
#define HI_OPTION_ALL_FW_MODE_MASK 0xFF

#define HI_OPTION_FW_SUBMODE_BITS 0x2
#define HI_OPTION_FW_SUBMODE_MASK 0x3
#define HI_OPTION_FW_SUBMODE_SHIFT 0x14
#define HI_OPTION_ALL_FW_SUBMODE_MASK 0xFF00
#define HI_OPTION_ALL_FW_SUBMODE_SHIFT 0x8

/* hi_option_flag2 options */
#define HI_OPTION_OFFLOAD_AMSDU 0x01
#define HI_OPTION_DFS_SUPPORT 0x02               /* Enable DFS support */
#define HI_OPTION_ENABLE_RFKILL 0x04             /* RFKill Enable Feature*/
#define HI_OPTION_RADIO_RETENTION_DISABLE 0x08   /* Disable radio retention */
#define HI_OPTION_ENABLE_REALTIME_DEBUGGING 0x10 /* Enable realtime debugging */
#define HI_OPTION_DISABLE_RTT 0x20               /* Disable RTT Feature */
#define HI_OPTION_DISABLE_MAC_OTP 0x40           /* Disable MAC address overwrite via OTP Feature */
#define HI_OPTION_USE_MBOX_CREDIT_REPORT 0x80    /* Enable MBOX Credit report mechanism <IOT feature> */
#define HI_OPTION_DISABLE_P2P_DEDICATE 0x100     /* Disable p2p dedicate device */
#define HI_OPTION_INCREASE_TXBUF 0x200           /* Increase txbuf flag for host */
#define HI_OPTION_AP_CLIENT_CNT 0x3c00           /* alternate number of clients in apmode */
#define HI_OPTION_AP_DTIM_VAL 0x3c000            /* alternate dtim val in apmode */
#define HI_OPTION_IOE_STA_OFFLOAD 0x40000        /* provision to enable station switching in apmode */
#define HI_OPTION_EXT_FW_DOWNLOAD_DONE 0x80000   /* ext patch download finished after WMI ready*/
#define HI_OPTION_ENABLE_NDIS_WAKE_WAR 0x100000  /* Enable NDIS wake WAR */
#define HI_OPTION_ENABLE_LTE_COEX 0x200000       /* enable ltecoex hook up function in ROM */
#define HI_OPTION_WOW_ACTIVE_LOW 0x400000        /* WoW in active low manner */
#define HI_OPTION_BAM2BAM_MODE 0x800000          /* Enable BAM2BAM mode */
#define HI_OPTION_ENABLE_PANASONIC_WAR 0x1000000 /* Enable Panasonic's WAR */
#define HI_OPTION_ENABLE_ONE_SHOT_NOA 0x2000000  /* Enable one shot ona */
#define HI_OPTION_ENABLE_SB_SPECIFIC 0x4000000   /* Enable customer specific function */
#define HI_OPTION_WOW_SINGLE_CHAIN 0x8000000     /* Enable single chain in wow */
#define HI_OPTION_ENABLE_WLAN_HB 0x10000000      /* Enable wlan hb function */
#define HI_OPTION_ENABLE_SOFTAP_CH13 0x20000000  /* Enable Softap in Channel 13 */
#define HI_OPTION_IGNORE_11D_BEACON 0x40000000   /* Ignore 11d beacon */
#define HI_OPTION_MCC_ENABLE \
    0x80000000 /* Disable Multichannel Concurrency (MCC) - used last bit to sync with mainline */
#define HI_OPTION_RF_KILL_SHIFT 0x2
#define HI_OPTION_RF_KILL_MASK 0x1

#define HI_OPTION_AP_CLIENT_CNT_SHIFT 0x0A
#define HI_OPTION_AP_DTIM_VAL_SHIFT 0x0E
#define HI_OPTION_IOE_STA_OFFLOAD_SHIFT 0x12

/* 4-bits(Bit 13 - Bit 10) to notify number of clients in apmode */
#define HI_OPTION_AP_CLIENT_CNT_MASK 0x0F
/* 4-bits(Bit 17 - Bit 14)) use this 4 bit mask for dtim val in apmode*/
#define HI_OPTION_AP_DTIM_VAL_MASK 0x0F
/* 1-bit(Bit 18)) use this 1 bit mask to enable sta-sta switching offload in apmode*/
#define HI_OPTION_IOE_STA_OFFLOAD_MASK 0x01

/* hi_option_flag3 options */
#define HI_OPTION_USE_OFFLOAD_P2P 0x01
#define HI_OPTION_EN_HOST_PROXY_FW_DLOAD_MODE 0x02 /* Controls host proxy boot mode */

/* hi_reset_flag */
#define HI_RESET_FLAG_PRESERVE_APP_START 0x01     /* preserve App Start address */
#define HI_RESET_FLAG_PRESERVE_HOST_INTEREST 0x02 /* preserve host interest */
#define HI_RESET_FLAG_PRESERVE_ROMDATA 0x04       /* preserve ROM data */
#define HI_RESET_FLAG_PRESERVE_NVRAM_STATE 0x08
#define HI_RESET_FLAG_PRESERVE_BOOT_INFO 0x10
#define HI_RESET_FLAG_WARM_RESET 0x20

#define HI_RESET_FLAG_IS_VALID 0x12345678 /* indicate the reset flag is valid */

#define HI_ACS_FLAGS_ENABLED (1 << 0)       /* ACS is enabled */
#define HI_ACS_FLAGS_USE_WWAN (1 << 1)      /* Use physical WWAN device */
#define HI_ACS_FLAGS_TEST_VAP (1 << 2)      /* Use test VAP */
#define HI_ACS_FLAGS_ATTUNNEL_MODE (1 << 3) /* Use at tunnel mode */
#define HI_ACS_FLAGS_RADIO_OFF (1 << 4)     /* WWAN radio off */
#define HI_ACS_FLAGS_USE_VAP (1 << 5)       /* Use VAP as a client */

/* Bits defined in hi_customer_flags */
#define HI_CE_FLAG_SPCIFIC_ENABLE 0x01

#define ON_RESET_FLAGS_VALID() (HOST_INTEREST->hi_reset_flag_valid == HI_RESET_FLAG_IS_VALID)

#define RESET_FLAGS_VALIDATE() (HOST_INTEREST->hi_reset_flag_valid = HI_RESET_FLAG_IS_VALID)

#define RESET_FLAGS_INVALIDATE() (HOST_INTEREST->hi_reset_flag_valid = 0)

#define ON_RESET_PRESERVE_APP_START() (HOST_INTEREST->hi_reset_flag & HI_RESET_FLAG_PRESERVE_APP_START)

#define ON_RESET_PRESERVE_NVRAM_STATE() (HOST_INTEREST->hi_reset_flag & HI_RESET_FLAG_PRESERVE_NVRAM_STATE)

#define ON_RESET_PRESERVE_HOST_INTEREST() (HOST_INTEREST->hi_reset_flag & HI_RESET_FLAG_PRESERVE_HOST_INTEREST)

#define ON_RESET_PRESERVE_ROMDATA() (HOST_INTEREST->hi_reset_flag & HI_RESET_FLAG_PRESERVE_ROMDATA)

#define ON_RESET_PRESERVE_BOOT_INFO() (HOST_INTEREST->hi_reset_flag & HI_RESET_FLAG_PRESERVE_BOOT_INFO)

#define ON_RESET_WARM_RESET() (HOST_INTEREST->hi_reset_flag & HI_RESET_FLAG_WARM_RESET)

#define IS_ATTUNNEL_MODE_ENABLED() (HOST_INTEREST->hi_acs_flags & HI_ACS_FLAGS_ATTUNNEL_MODE)

#define IS_WWAN_RADIO_OFF() (HOST_INTEREST->hi_acs_flags & HI_ACS_FLAGS_RADIO_OFF)

/* CONSOLE FLAGS
 *
 * Bit Range  Meaning
 * ---------  --------------------------------
 *   2..0     UART ID (0 = Default)
 *    3       Baud Select (0 = 9600, 1 = 115200)
 *   30..4    Reserved
 *    31      Enable Console
 *
 * */

#define HI_CONSOLE_FLAGS_ENABLE (1 << 31)
#define HI_CONSOLE_FLAGS_UART_MASK (0x7)
#define HI_CONSOLE_FLAGS_UART_SHIFT 0
#define HI_CONSOLE_FLAGS_BAUD_SELECT (1 << 3)

/* SM power save options */
#define HI_SMPS_ALLOW_MASK (0x00000001)
#define HI_SMPS_MODE_MASK (0x00000002)
#define HI_SMPS_MODE_STATIC (0x00000000)
#define HI_SMPS_MODE_DYNAMIC (0x00000002)
#define HI_SMPS_DISABLE_AUTO_MODE (0x00000004)
#define HI_SMPS_DATA_THRESH_MASK (0x000007f8)
#define HI_SMPS_DATA_THRESH_SHIFT (3)
#define HI_SMPS_RSSI_THRESH_MASK (0x0007f800)
#define HI_SMPS_RSSI_THRESH_SHIFT (11)
#define HI_SMPS_LOWPWR_CM_MASK (0x00380000)
#define HI_SMPS_LOWPWR_CM_SHIFT (15)
#define HI_SMPS_HIPWR_CM_MASK (0x03c00000)
#define HI_SMPS_HIPWR_CM_SHIFT (19)

#define HOST_INTEREST_SMPS_GET_MODE() (HOST_INTEREST->hi_smps_options & HI_SMPS_MODE_MASK)
#define HOST_INTEREST_SMPS_GET_DATA_THRESH() \
    ((HOST_INTEREST->hi_smps_options & HI_SMPS_DATA_THRESH_MASK) >> HI_SMPS_DATA_THRESH_SHIFT)
#define HOST_INTEREST_SMPS_SET_DATA_THRESH(x) (((x) << HI_SMPS_DATA_THRESH_SHIFT) & HI_SMPS_DATA_THRESH_MASK)
#define HOST_INTEREST_SMPS_GET_RSSI_THRESH() \
    ((HOST_INTEREST->hi_smps_options & HI_SMPS_RSSI_THRESH_MASK) >> HI_SMPS_RSSI_THRESH_SHIFT)
#define HOST_INTEREST_SMPS_SET_RSSI_THRESH(x) (((x) << HI_SMPS_RSSI_THRESH_SHIFT) & HI_SMPS_RSSI_THRESH_MASK)
#define HOST_INTEREST_SMPS_SET_LOWPWR_CM() \
    ((HOST_INTEREST->hi_smps_options & HI_SMPS_LOWPWR_CM_MASK) >> HI_SMPS_LOWPWR_CM_SHIFT)
#define HOST_INTEREST_SMPS_SET_HIPWR_CM() \
    ((HOST_INTEREST->hi_smps_options << HI_SMPS_HIPWR_CM_MASK) & HI_SMPS_HIPWR_CM_SHIFT)
#define HOST_INTEREST_SMPS_IS_AUTO_MODE_DISABLED() (HOST_INTEREST->hi_smps_options & HI_SMPS_DISABLE_AUTO_MODE)

/* WOW Extension configuration
 *
 * Bit Range  Meaning
 * ---------  --------------------------------
 *   8..0     Size of each WOW pattern (max 511)
 *   15..9    Number of patterns per list (max 127)
 *   17..16   Number of lists (max 4)
 *   30..18   Reserved
 *   31       Enabled
 *
 *  set values (except enable) to zeros for default settings
 *
 * */

#define HI_WOW_EXT_ENABLED_MASK (1 << 31)
#define HI_WOW_EXT_NUM_LIST_SHIFT 16
#define HI_WOW_EXT_NUM_LIST_MASK (0x3 << HI_WOW_EXT_NUM_LIST_SHIFT)
#define HI_WOW_EXT_NUM_PATTERNS_SHIFT 9
#define HI_WOW_EXT_NUM_PATTERNS_MASK (0x7F << HI_WOW_EXT_NUM_PATTERNS_SHIFT)
#define HI_WOW_EXT_PATTERN_SIZE_SHIFT 0
#define HI_WOW_EXT_PATTERN_SIZE_MASK (0x1FF << HI_WOW_EXT_PATTERN_SIZE_SHIFT)

#define HI_WOW_EXT_MAKE_CONFIG(num_lists, count, size)                             \
    ((((num_lists) << HI_WOW_EXT_NUM_LIST_SHIFT) & HI_WOW_EXT_NUM_LIST_MASK) |     \
     (((count) << HI_WOW_EXT_NUM_PATTERNS_SHIFT) & HI_WOW_EXT_NUM_PATTERNS_MASK) | \
     (((size) << HI_WOW_EXT_PATTERN_SIZE_SHIFT) & HI_WOW_EXT_PATTERN_SIZE_MASK))

#define HI_WOW_EXT_GET_NUM_LISTS(config) (((config)&HI_WOW_EXT_NUM_LIST_MASK) >> HI_WOW_EXT_NUM_LIST_SHIFT)
#define HI_WOW_EXT_GET_NUM_PATTERNS(config) (((config)&HI_WOW_EXT_NUM_PATTERNS_MASK) >> HI_WOW_EXT_NUM_PATTERNS_SHIFT)
#define HI_WOW_EXT_GET_PATTERN_SIZE(config) (((config)&HI_WOW_EXT_PATTERN_SIZE_MASK) >> HI_WOW_EXT_PATTERN_SIZE_SHIFT)

/* RX Coalescing filter configuration
 *
 * Bit Range  Meaning
 * ---------  --------------------------------
 *   7..0     Number of filters (0 = use default)
 *   11..8    Number of tests per filter (0 = use default)
 *   15..12   Buffer threshold multiplier (Rx filter threshold = RX buffers * multipler / divisor)
 *   19..16   Buffer threshold divisor
 *   23..20   RX idle recheck interval (in 25MS increments)
 *   31       Enabled
 *
 *  set values (except enable) to zeros for default settings
 *
 * */

#define HI_RX_COALESCE_FILTER_ENABLED_MASK (1 << 31)
#define HI_RX_COALESCE_FILTER_COUNT_SHIFT 0
#define HI_RX_COALESCE_FILTER_COUNT_MASK (0xFF << HI_RX_COALESCE_FILTER_COUNT_SHIFT)
#define HI_RX_COALESCE_FILTER_TESTS_SHIFT 8
#define HI_RX_COALESCE_FILTER_TESTS_MASK (0xF << HI_RX_COALESCE_FILTER_TESTS_SHIFT)
#define HI_RX_COALESCE_FILTER_BUF_MULT_SHIFT 12
#define HI_RX_COALESCE_FILTER_BUF_MULT_MASK (0xF << HI_RX_COALESCE_FILTER_BUF_MULT_SHIFT)
#define HI_RX_COALESCE_FILTER_BUF_DIV_SHIFT 16
#define HI_RX_COALESCE_FILTER_BUF_DIV_MASK (0xF << HI_RX_COALESCE_FILTER_BUF_DIV_SHIFT)
#define HI_RX_COALESCE_FILTER_REARM_INTVL_SHIFT 20
#define HI_RX_COALESCE_FILTER_REARM_INTVL_MASK (0xF << HI_RX_COALESCE_FILTER_REARM_INTVL_SHIFT)

#define HI_GET_RX_COALESCE_FILTER_COUNT(config) \
    (((config)&HI_RX_COALESCE_FILTER_COUNT_MASK) >> HI_RX_COALESCE_FILTER_COUNT_SHIFT)

#define HI_GET_RX_COALESCE_FILTER_TESTS(config) \
    (((config)&HI_RX_COALESCE_FILTER_TESTS_MASK) >> HI_RX_COALESCE_FILTER_TESTS_SHIFT)

#define HI_GET_RX_COALESCE_FILTER_BUF_MULTIPLIER(config) \
    (((config)&HI_RX_COALESCE_FILTER_BUF_MULT_MASK) >> HI_RX_COALESCE_FILTER_BUF_MULT_SHIFT)

#define HI_GET_RX_COALESC_FILTER_BUF_DIVIDER(config) \
    (((config)&HI_RX_COALESCE_FILTER_BUF_DIV_MASK) >> HI_RX_COALESCE_FILTER_BUF_DIV_SHIFT)

#define HI_GET_RX_COALESCE_FILTER_REARM_INTERVAL(config) \
    (((config)&HI_RX_COALESCE_FILTER_REARM_INTVL_MASK) >> HI_RX_COALESCE_FILTER_REARM_INTVL_SHIFT)

#define HI_RX_COALESCE_FILTER_MAKE_CONFIG(nfilt, tpf, mult, div, rx_check)                         \
    \
((((nfilt) << HI_RX_COALESCE_FILTER_COUNT_SHIFT) & HI_RX_COALESCE_FILTER_COUNT_MASK) |             \
        (((tpf) << HI_RX_COALESCE_FILTER_TESTS_SHIFT) & HI_RX_COALESCE_FILTER_TESTS_MASK) |        \
        (((mult) << HI_RX_COALESCE_FILTER_BUF_MULT_SHIFT) & HI_RX_COALESCE_FILTER_BUF_MULT_MASK) | \
        (((div) << HI_RX_COALESCE_FILTER_BUF_DIV_SHIFT) & HI_RX_COALESCE_FILTER_BUF_DIV_MASK) |    \
        (((rx_check) << HI_RX_COALESCE_FILTER_REARM_INTVL_SHIFT) & HI_RX_COALESCE_FILTER_REARM_INTVL_MASK)\
)

/* input values for hi_i2c_slv_cfg */
#define HI_SET_I2C_SLV_CFG(_sda, _sck) (((_sda) << 16) & 0xffff0000) | ((_sck)&0x0000ffff)
#define HI_GET_I2C_SLV_CFG_SDA() ((HOST_INTEREST->hi_i2c_slv_cfg >> 16) & 0x0000ffff)
#define HI_GET_I2C_SLV_CFG_SCK() ((HOST_INTEREST->hi_i2c_slv_cfg) & 0x0000ffff)

/*
 * Intended for use by Host software, this macro returns the Target RAM
 * address of any item in the host_interest structure.
 * Example: target_addr = AR4100_HOST_INTEREST_ITEM_ADDRESS(hi_board_data);
 */
#define AR4100_HOST_INTEREST_ITEM_ADDRESS(item) \
    ((uint32_t) & ((((struct host_interest_s *)(AR4100_HOST_INTEREST_ADDRESS))->item)))

#define AR4002_HOST_INTEREST_ITEM_ADDRESS(item) \
    ((uint32_t) & ((((struct host_interest_s *)(AR4002_HOST_INTEREST_ADDRESS))->item)))

#define HOST_INTEREST_DBGLOG_IS_ENABLED() (!(HOST_INTEREST->hi_option_flag & HI_OPTION_DISABLE_DBGLOG))

#define HOST_INTEREST_PKTLOG_IS_ENABLED() ((HOST_INTEREST->hi_pktlog_num_buffers))

#define HOST_INTEREST_PROFILE_IS_ENABLED() (HOST_INTEREST->hi_option_flag & HI_OPTION_ENABLE_PROFILE)

#define LF_TIMER_STABILIZATION_IS_ENABLED() (!(HOST_INTEREST->hi_option_flag & HI_OPTION_NO_LFT_STBL))

#define IS_AMSDU_OFFLAOD_ENABLED() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_OFFLOAD_AMSDU))

#define HOST_INTEREST_DFS_IS_ENABLED() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_DFS_SUPPORT))

#define HOST_INTEREST_REALTIME_DEBUGGING_IS_ENABLED() \
    ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_ENABLE_REALTIME_DEBUGGING))

#define HOST_INTEREST_RTT_IS_DISABLED() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_DISABLE_RTT))

#define HOST_INTEREST_MAC_OTP_IS_DISABLED() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_DISABLE_MAC_OTP))

#define HOST_INTEREST_DISABLE_P2P_DEDICATE() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_DISABLE_P2P_DEDICATE))

#define HOST_INTEREST_INCREASE_TXBUF_IS_ENABLED() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_INCREASE_TXBUF))

#define HOST_INTEREST_PANASONIC_WAR_ENABLE() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_ENABLE_PANASONIC_WAR))

#define HOST_INTEREST_ONE_SHOT_NOA_ENABLED() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_ENABLE_ONE_SHOT_NOA))

#define HOST_INTEREST_SB_SPECIFIC_ENABLED() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_ENABLE_SB_SPECIFIC))

#define HOST_INTEREST_EXT_FW_DOWNLOAD_DONE() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_EXT_FW_DOWNLOAD_DONE))
#define HOST_INTEREST_WOW_ACTIVE_IS_LOW() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_WOW_ACTIVE_LOW))

#define HOST_INTEREST_NDIS_WAKE_WAR_IS_ENABLEED() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_ENABLE_NDIS_WAKE_WAR))
#define HOST_INTEREST_LTECOEX_ENABLED() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_ENABLE_LTE_COEX))

#define HOST_INTEREST_IS_BAM2BAM_MODE() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_BAM2BAM_MODE))

#define HOST_INTEREST_WLAN_HB_ENABLED() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_ENABLE_WLAN_HB))

#define HOST_INTEREST_MCC_ENABLE() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_MCC_ENABLE))

#define HOST_INTEREST_IGNORE_11D_BEACON() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_IGNORE_11D_BEACON))

#define HOST_INTEREST_ENABLE_SOFTAP_CH13() ((HOST_INTEREST->hi_option_flag2 & HI_OPTION_ENABLE_SOFTAP_CH13))

#define HOST_INTEREST_OFFLOAD_P2P_IS_ENABLED() ((HOST_INTEREST->hi_option_flag3 & HI_OPTION_USE_OFFLOAD_P2P))

/*power save flag bit definitions*/
#define HI_PWR_SAVE_LPL_ENABLED 0x1
/*b1-b3 reserved*/
/*b4-b5 : dev0 LPL type : 0 - none
                          1- Reduce Pwr Search
                          2- Reduce Pwr Listen*/
/*b6-b7 : dev1 LPL type and so on for Max 8 devices*/
#define HI_PWR_SAVE_LPL_DEV0_LSB 4
#define HI_PWR_SAVE_LPL_DEV_MASK 0x3
/*power save related utility macros*/
#define HI_LPL_ENABLED() ((HOST_INTEREST->hi_pwr_save_flags & HI_PWR_SAVE_LPL_ENABLED))
#define HI_DEV_LPL_TYPE_GET(_devix) \
    (HOST_INTEREST->hi_pwr_save_flags & ((HI_PWR_SAVE_LPL_DEV_MASK) << (HI_PWR_SAVE_LPL_DEV0_LSB + (_devix)*2)))

#define HOST_INTEREST_SMPS_IS_ALLOWED() ((HOST_INTEREST->hi_smps_options & HI_SMPS_ALLOW_MASK))

/* Convert a Target virtual address into a Target physical address */

#define AR4100_VTOP(vaddr) ((vaddr)&0x001fffff)
#define AR4002_VTOP(vaddr) (vaddr)

#if ATH_FIRMWARE_TARGET == TARGET_AR4100_REV2
#define EXPECTED_MAX_WRITE_BUFFER_SPACE 3163
#define TARG_VTOP(vaddr) AR4100_VTOP(vaddr)
#define HOST_INTEREST_ITEM_ADDRESS(item) AR4100_HOST_INTEREST_ITEM_ADDRESS(item)
#elif ATH_FIRMWARE_TARGET == TARGET_AR400X_REV1
#define EXPECTED_MAX_WRITE_BUFFER_SPACE 3163 // FIXME: confirm this value
#define TARG_VTOP(vaddr) AR4002_VTOP(vaddr)
#define HOST_INTEREST_ITEM_ADDRESS(item) AR4002_HOST_INTEREST_ITEM_ADDRESS(item)
#define AR400X_BOARD_DATA_ADDRESS 0x46B800
#define AR400X_BOARD_DATA_SIZE 6144
#define AR400X_EXT_BOARD_DATA_SIZE 0
#define AR400X_RAM_RESERVE_SIZE 18432
#else
#error Must have defined value for ATH_FIRMWARE_TARGET
#endif

/* # of uint32_t entries in targregs, used by DIAG_FETCH_TARG_REGS */
#define AR4100_FETCH_TARG_REGS_COUNT 64

#define AR4100_CONFIGFOUND_ADDR 0x540720
#define AR4100_CONFIGFOUND_VAL 0
#define AR4100_CONFIGFOUND_STK_ADDR 0x544239
#define AR4100_NVRAM_SAMPLE_VAL 0x1e0104
#define AR4100_NVRAM_SAMPLE_ADDR 0x54070c

/*  hi_flash_is_present value definition  */

#define HOST_PROXY_INIT 0x01
#define HOST_PROXY_NORMAL_BOOT 0x02
#define HOST_PROXY_BMI_BOOT 0x03
#define HOST_PROXY_BOOTCTL_MASK 0x03

#endif /* !__ASSEMBLER__ */

#endif /* __TARGADDRS_H__ */
