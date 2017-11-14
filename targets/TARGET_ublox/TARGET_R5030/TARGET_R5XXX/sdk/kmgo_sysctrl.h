#ifndef INCLUDED_KMGO_SYSCTRL
#define INCLUDED_KMGO_SYSCTRL
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** kmgo_sysctrl HAL Spreadsheet version number */
#define KMGO_SYSCTRL_HAL_VERSION 4

/** System Controller Module
*/
struct kmgo_sysctrl_s {
   /** Specifies Reset vector for BOOT CPU - Not used for KM project at address offset 0x000, read-write */
   uint32_t cpu_boot_vinit;
   /** Control register for MODEM CPU at address offset 0x004, read-write */
   uint32_t cpu_mdm_config;
   /** Control register for RF CPU at address offset 0x008, read-write */
   uint32_t cpu_rf_config;
   /** Control register for APP CPU Not used in Gottardo project at address offset 0x00C, read-write */
   uint32_t cpu_app_config;
   /** Control register for PHY CPU at address offset 0x010, read-write */
   uint32_t cpu_phy_config;
   /** Reset Reason register for CPU 1  (MDM) at address offset 0x014, read-write */
   uint32_t rst_reason_cpu_mdm;
   /** Reset Reason register for CPU 2 (RF) at address offset 0x018, read-write */
   uint32_t rst_reason_cpu_rf;
   /** Reserved register in GO project - Not used Reset Reason register for CPU 3  (APP) at address offset 0x01C, read-write */
   uint32_t rst_reason_cpu_app;
   /** Reserved register in GO project - not used Reset Reason register for CPU 4 (PHY) at address offset 0x020, read-write */
   uint32_t rst_reason_cpu_phy;
   /** Allows the state of the resets of the baseband to be activated. When a one is set in any bit of this register, the corresponding IP will be put in reset at address offset 0x024, write-only */
   uint32_t rst_sw_set_1s;
   /** Allows the state of the resets of the baseband to be de-activated. When a one is set in any bit of this register, the corresponding IP will be taken out of reset. at address offset 0x028, write-only */
   uint32_t rst_sw_set_1c;
   /** Allows the state of the resets of the baseband to be read 0 value indicates corresponding IP is in reset 1 value indicates corresponding IP is out of reset at address offset 0x02C, read-only */
   uint32_t rst_sw_set_1;
   /** Allows the state of the resets of the baseband to be activated. When a one is set in any bit of this register, the corresponding IP will be put in reset at address offset 0x030, write-only */
   uint32_t rst_sw_set_2s;
   /** Allows the state of the resets of the baseband to be de-activated. When a one is set in any bit of this register, the corresponding IP will be taken out of reset. at address offset 0x034, write-only */
   uint32_t rst_sw_set_2c;
   /** Allows the state of the resets of the baseband to be read 0 value indicates corresponding IP is in reset 1 value indicates corresponding IP is out of reset at address offset 0x038, read-only */
   uint32_t rst_sw_set_2;
   /** Allows the state of the resets of the baseband to be activated for certain time and then de-activated. When a one is set in any bit of this register, the corresponding reset will be active for time governed by STROBE_COUNT register setting and then reset will be inactivated by HW. at address offset 0x03C, write-only */
   uint32_t rst_sw_strobe_1;
   /** Allows the state of the resets of the baseband to be activated for certain time and then de-activated. When a one is set in any bit of this register, the corresponding reset will be active for time governed by STROBE_COUNT register setting and then reset will be inactivated by HW. at address offset 0x040, read-write */
   uint32_t rst_sw_strobe_2;
   /** Configures reset strobe duration at address offset 0x044, read-write */
   uint32_t rst_sw_strobe_count;
   /** When MDM watchdog occurs, allows certain resets to be masked. at address offset 0x048, read-write */
   uint32_t rst_wdg_mdm_disable_1;
   /** When MDM watchdog occurs, allows certain resets to be masked. at address offset 0x04C, read-write */
   uint32_t rst_wdg_mdm_disable_2;
   /** When RF watchdog occurs, allows certain resets to be masked. at address offset 0x050, read-write */
   uint32_t rst_wdg_rf_disable_1;
   /** When RF watchdog occurs, allows certain resets to be masked. at address offset 0x054, read-write */
   uint32_t rst_wdg_rf_disable_2;
   /** Not used in GO project When APP watchdog occurs, allows certain resets to be masked. at address offset 0x058, read-write */
   uint32_t rst_wdg_app_disable_1;
   /** Not used in GO project When APP watchdog occurs, allows certain resets to be masked. at address offset 0x05C, read-write */
   uint32_t rst_wdg_app_disable_2;
   /** Not used in GO project When PHY watchdog occurs, allows certain resets to be masked. at address offset 0x060, read-write */
   uint32_t rst_wdg_phy_disable_1;
   /** Not used in GO project When  PHY watchdog occurs, allows certain resets to be masked. at address offset 0x064, read-write */
   uint32_t rst_wdg_phy_disable_2;
   /** Not used in GO project Enables pre-warning towards Psram IP at address offset 0x068, read-write */
   uint32_t rst_ctrl;
   /** System controller general purpose register at address offset 0x06C, read-write */
   uint32_t sys_gp;
   /** Not used in GO project Master control register for clock control unit (CCU) in  at address offset 0x070, read-write */
   uint32_t rf_ccu_ctrl;
   /** PERIPHERAL_ID 4 at address offset 0x074, read-only */
   uint32_t peripheral_id4;
   /** PERIPHERAL_ID 5 at address offset 0x078, read-only */
   uint32_t peripheral_id5;
   /** PERIPHERAL_ID 6 at address offset 0x07C, read-only */
   uint32_t peripheral_id6;
   /** PERIPHERAL_ID 7 at address offset 0x080, read-only */
   uint32_t peripheral_id7;
   /** PERIPHERAL_ID 0 Register value for GO = 0xA7 Register value for KM = 0xA6 at address offset 0x084, read-only */
   uint32_t peripheral_id0;
   /** PERIPHERAL_ID 1 at address offset 0x088, read-only */
   uint32_t peripheral_id1;
   /** PERIPHERAL_ID 2 at address offset 0x08C, read-only */
   uint32_t peripheral_id2;
   /** PERIPHERAL_ID 3 at address offset 0x090, read-only */
   uint32_t peripheral_id3;
   /** COMPONENT_ID0 at address offset 0x094, read-only */
   uint32_t component_id0;
   /** COMPONENT_ID1 at address offset 0x098, read-only */
   uint32_t component_id1;
   /** COMPONENT_ID2 at address offset 0x09C, read-only */
   uint32_t component_id2;
   /** COMPONENT_ID3 at address offset 0x0A0, read-only */
   uint32_t component_id3;
   /** Controls functional memory bist at address offset 0x0A4, read-write */
   uint32_t func_mbist_ctrl;
   /** Status register for functional memory bist at address offset 0x0A8, read-write */
   uint32_t func_mbist_stat;
   /** Functional mbist enable register at address offset 0x0AC, read-write */
   uint32_t func_mbist_enable_1;
   /** Functional mbist enable register at address offset 0x0B0, read-write */
   uint32_t func_mbist_enable_2;
   /** Functional mbist fail status register This register gets cleared when SW clears FUNC_MBIST_CTRL.DONE bit at address offset 0x0B4, read-only */
   uint32_t func_mbist_fail_1;
   /** Functional mbist fail status register This register gets cleared when SW clears FUNC_MBIST_CTRL.DONE bit at address offset 0x0B8, read-only */
   uint32_t func_mbist_fail_2;
   /** Functional mbist done status register This register gets cleared when SW clears FUNC_MBIST_CTRL.DONE bit at address offset 0x0BC, read-only */
   uint32_t func_mbist_done_1;
   /** Functional mbist done status register This register gets cleared when SW clears FUNC_MBIST_CTRL.DONE bit at address offset 0x0C0, read-only */
   uint32_t func_mbist_done_2;
   /** When functional mbist done trigger occurs, allows certain resets to be masked. at address offset 0x0C4, read-write */
   uint32_t rst_func_mbist_disable_1;
   /** When functional mbist done trigger occurs, allows certain resets to be masked. at address offset 0x0C8, read-write */
   uint32_t rst_func_mbist_disable_2;
   /** Status register for reset controller FSM at address offset 0x0CC, read-only */
   uint32_t rst_ctrl_status;
   /** Reserved space */
   uint8_t fill0[816];
   /** For PLL1  fractional divider settings at address offset 0x400, read-write */
   uint32_t pll1_freq_conf;
   /** For PLL1 post divider 1 controls at address offset 0x404, read-write */
   uint32_t pll1_pdiv1_conf;
   /** For PLL1 post divider 2 controls at address offset 0x408, read-write */
   uint32_t pll1_pdiv2_conf;
   /** For PLL1 controls at address offset 0x40C, read-write */
   uint32_t pll1_ctrl;
   /** For PLL1 Status at address offset 0x410, read-only */
   uint32_t pll1_status;
   /** For PLL2  fractional divider settings at address offset 0x414, read-write */
   uint32_t pll2_freq_conf;
   /** For PLL2 post divider 1 controls at address offset 0x418, read-write */
   uint32_t pll2_pdiv1_conf;
   /** For PLL2 post divider 2 controls at address offset 0x41C, read-write */
   uint32_t pll2_pdiv2_conf;
   /** For PLL2 controls at address offset 0x420, read-write */
   uint32_t pll2_ctrl;
   /** For PLL2 Status at address offset 0x424, read-only */
   uint32_t pll2_status;
   /** Reserved space */
   uint8_t fill1[40];
   /** Defines wait time for PLLs to be locked at address offset 0x450, read-write */
   uint32_t pll_lockcnt;
   /** Reserved space */
   uint8_t fill2[12];
   /** Configuration register for root clock VSP - parent clock for vsp clock - max. 300MHz at address offset 0x460, read-write */
   uint32_t rtclk_vsp_conf;
   /** Configuration register for root clock CPU - parent clock for all cpus, main, periph, dbg, tpiu clocks - max. 300MHz at address offset 0x464, read-write */
   uint32_t rtclk_cpu_conf;
   /** Configuration register for root clock EXTIF - parent clock for all interface clocks for Ips (uart, spi, iic, psram etc.) except for ospi and sdio clocks- max. 300MHz  at address offset 0x468, read-write */
   uint32_t rtclk_extif_conf;
   /** Configuration register for root clock EXTIF_FAST - parent clock for ospi and sdio clocks - max. 200MHz at address offset 0x46C, read-write */
   uint32_t rtclk_extif_fast_conf;
   /** Not used in Klein-Matterhorn project - Reserved reg in KM Configuration register for root clock ETH for ethernet - should be programmed to fixed 250MHz frequency at address offset 0x470, read-write */
   uint32_t rtclk_eth_conf;
   /** Not used in Gottardo project - Reserved reg in GO Configuration register for root clock USB for usb sub-system - should be programmed to fixed 12/24/10 MHz frequency at address offset 0x474, read-write */
   uint32_t rtclk_usb_conf;
   /** Reserved space */
   uint8_t fill3[8];
   /** Configuration register for main fabric clock - max 150MHz at address offset 0x480, read-write */
   uint32_t clk_main_conf;
   /** Configuration register for APB peripheral clock - max 75MHz This clock should be equal or less than MAIN clock at address offset 0x484, read-write */
   uint32_t clk_periph_conf;
   /** Configuration register for CPU MDM clock - max 300MHz at address offset 0x488, read-write */
   uint32_t clk_cpu_mdm_conf;
   /** Configuration register for CPU RF clock - max 150MHz at address offset 0x48C, read-write */
   uint32_t clk_cpu_rf_conf;
   /** Configuration register for CPU APP clock - max 300MHz at address offset 0x490, read-write */
   uint32_t clk_cpu_app_conf;
   /** Configuration register for CPU PHY clock - max 150MHz at address offset 0x494, read-write */
   uint32_t clk_cpu_phy_conf;
   /** Configuration register for TPIU clock - max 150MHz at address offset 0x498, read-write */
   uint32_t clk_tpiu_conf;
   /** Configuration register for DBG clock - max 75MHz at address offset 0x49C, read-write */
   uint32_t clk_dbg_conf;
   /** Configuration register for VSP clock - max 300MHz at address offset 0x4A0, read-write */
   uint32_t clk_vsp_conf;
   /** Configuration register for UART clock - max 150MHz at address offset 0x4A4, read-write */
   uint32_t clk_uart_conf;
   /** Not used in Gottardo project  Configuration register for Audio clock - max 150MHz at address offset 0x4A8, read-write */
   uint32_t clk_audio_conf;
   /** Configuration register for OSPI clock - max 200MHz at address offset 0x4AC, read-write */
   uint32_t clk_ospi_conf;
   /** Not used in Gottardo - Reserved reg in GO Configuration register for PSRAM clock - max 150MHz at address offset 0x4B0, read-write */
   uint32_t clk_psram_conf;
   /** Configuration register for SPI clock - max 150MHz at address offset 0x4B4, read-write */
   uint32_t clk_spi_conf;
   /** Configuration register for I2C clock - max 150MHz at address offset 0x4B8, read-write */
   uint32_t clk_i2c_conf;
   /** Not used in Gottardo project - Reserved register in GO Configuration register for SDIO clock - max 200MHz at address offset 0x4BC, read-write */
   uint32_t clk_sdio_conf;
   /** Not used in Klein Matterhorn project - Reserved register in KM Configuration register for RFTSF clock - max 1MHz at address offset 0x4C0, read-write */
   uint32_t clk_rftsf_conf;
   /** Not used in GO project Configuration register for TSCU clock - max 150MHz at address offset 0x4C4, read-write */
   uint32_t clk_cpu_tscu_conf;
   /** Reserved space */
   uint8_t fill4[8];
   /** Configuration register for TMR_CPU clock - max 1 MHz at address offset 0x4D0, read-only */
   uint32_t clk_tmr_conf;
   /** Configuration register for TMR_CPU clock - max 1 MHz at address offset 0x4D4, read-write */
   uint32_t clk_tmr_cpu_conf;
   /** Reserved space */
   uint8_t fill5[8];
   /** Configuration register for Leti DVS TMFLT (timing fault) ring clock generated from PLL1 at address offset 0x4E0, read-write */
   uint32_t clk_pll1_tmfltrng_conf;
   /** Configuration register for Leti DVS TMFLT (timing fault) ring clock generated from PLL2 at address offset 0x4E4, read-write */
   uint32_t clk_pll2_tmfltrng_conf;
   /** Reserved space */
   uint8_t fill6[20];
   /** Configuration register for Audio NCO at address offset 0x4FC, read-write */
   uint32_t clk_audio_nco_conf;
   /** Clock enable control register Writing 1 enables the corresponding clock Writing 0 has no effect at address offset 0x500, write-only */
   uint32_t clken_1_set;
   /** Clock disable control register Writing 1 disables the clock Writing 0 has no effect at address offset 0x504, write-only */
   uint32_t clken_1_clr;
   /** Clock status register - provides clock enable status as programmed by software at address offset 0x508, read-only */
   uint32_t clken_1_val;
   /** Clock enable read-back register - provides clock enable status as propagated to system which is combination of SW programmed value and HW internal forcing of clock gating logic at address offset 0x50C, read-only */
   uint32_t clken_1_rdbk;
   /** Clock enable disable control register used only when power mode CLK_SLEEP_MODE is activated inside power controller  writing 1 enables clock, writing 0 disables the clock at address offset 0x510, read-write */
   uint32_t slp_clken_1;
   /** Reserved space */
   uint8_t fill7[12];
   /** Clock enable control register Writing 1 enables the corresponding clock Writing 0 has no effect at address offset 0x520, write-only */
   uint32_t clken_2_set;
   /** Clock disable control register Writing 1 disables the clock Writing 0 has no effect at address offset 0x524, write-only */
   uint32_t clken_2_clr;
   /** Clock status register - provides clock enable status as programmed by software at address offset 0x528, read-only */
   uint32_t clken_2_val;
   /** Clock enable read-back register - provides clock enable status as propagated to system which is combination of SW programmed value and HW internal forcing of clock gating logic at address offset 0x52C, read-only */
   uint32_t clken_2_rdbk;
   /** Clock enable disable control register used only when power mode CLK_SLEEP_MODE is activated inside power controller  writing 1 enables clock, writing 0 disables the clock at address offset 0x530, read-write */
   uint32_t slp_clken_2;
   /** Reserved space */
   uint8_t fill8[12];
   /** Mode setting register for clock controller at address offset 0x540, read-write */
   uint32_t clkctrl_mode;
   /** Clock controller misc status register at address offset 0x544, read-only */
   uint32_t clkctrl_status;
   /** Not used in GO project Enables local control of clock gating for different parts of Soc When 0 - local clock gating is disabled and corresponding clock stays enabled When 1 - local clock gating logic  takes control at address offset 0x548, read-write */
   uint32_t clk_enable_local_ctrl;
};

/** bit field defines for kmgo_sysctrl_s#cpu_boot_vinit */
#define KMGO_SYSCTRL_CPU_BOOT_VINIT_NONE_OFFSET 0
#define KMGO_SYSCTRL_CPU_BOOT_VINIT_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#cpu_mdm_config */
#define KMGO_SYSCTRL_CPU_MDM_CONFIG_CPUHALT_OFFSET 0
#define KMGO_SYSCTRL_CPU_MDM_CONFIG_CPUHALT_SIZE 1
#define KMGO_SYSCTRL_CPU_MDM_CONFIG_WDG_RETAIN_VINIT_OFFSET 2
#define KMGO_SYSCTRL_CPU_MDM_CONFIG_WDG_RETAIN_VINIT_SIZE 1
#define KMGO_SYSCTRL_CPU_MDM_CONFIG_CPU_HALTED_OFFSET 15
#define KMGO_SYSCTRL_CPU_MDM_CONFIG_CPU_HALTED_SIZE 1

/** bit field defines for kmgo_sysctrl_s#cpu_rf_config */
#define KMGO_SYSCTRL_CPU_RF_CONFIG_CPUHALT_OFFSET 0
#define KMGO_SYSCTRL_CPU_RF_CONFIG_CPUHALT_SIZE 1
#define KMGO_SYSCTRL_CPU_RF_CONFIG_CPU_HALTED_OFFSET 15
#define KMGO_SYSCTRL_CPU_RF_CONFIG_CPU_HALTED_SIZE 1

/** bit field defines for kmgo_sysctrl_s#cpu_app_config */
#define KMGO_SYSCTRL_CPU_APP_CONFIG_CPUHALT_OFFSET 0
#define KMGO_SYSCTRL_CPU_APP_CONFIG_CPUHALT_SIZE 1
#define KMGO_SYSCTRL_CPU_APP_CONFIG_WDG_RETAIN_VINIT_OFFSET 2
#define KMGO_SYSCTRL_CPU_APP_CONFIG_WDG_RETAIN_VINIT_SIZE 1
#define KMGO_SYSCTRL_CPU_APP_CONFIG_CPU_HALTED_OFFSET 15
#define KMGO_SYSCTRL_CPU_APP_CONFIG_CPU_HALTED_SIZE 1

/** bit field defines for kmgo_sysctrl_s#cpu_phy_config */
#define KMGO_SYSCTRL_CPU_PHY_CONFIG_CPUHALT_OFFSET 0
#define KMGO_SYSCTRL_CPU_PHY_CONFIG_CPUHALT_SIZE 1
#define KMGO_SYSCTRL_CPU_PHY_CONFIG_CPU_HALTED_OFFSET 15
#define KMGO_SYSCTRL_CPU_PHY_CONFIG_CPU_HALTED_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_reason_cpu_mdm */
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_REASON_POR_RST_OFFSET 0
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_REASON_POR_RST_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_REASON_SW_RST_OFFSET 1
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_REASON_SW_RST_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_REASON_CPU_FMBIST_DONE_OFFSET 2
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_REASON_CPU_FMBIST_DONE_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_REASON_CPU_LOCK_OFFSET 3
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_REASON_CPU_LOCK_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_REASON_CPU_PWRDN_OFFSET 4
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_REASON_CPU_PWRDN_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_REASON_CPU_PWR_WDG_OFFSET 5
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_REASON_CPU_PWR_WDG_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_REASON_CPU_WDG_OFFSET 6
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_REASON_CPU_WDG_SIZE 4
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_REASON_LBIST_DONE_OFFSET 10
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_REASON_LBIST_DONE_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_CPU_LOCK_INDICATOR_OFFSET 31
#define KMGO_SYSCTRL_RST_REASON_CPU_MDM_CPU_LOCK_INDICATOR_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_reason_cpu_rf */
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_REASON_POR_RST_OFFSET 0
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_REASON_POR_RST_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_REASON_SW_RST_OFFSET 1
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_REASON_SW_RST_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_REASON_CPU_FMBIST_DONE_OFFSET 2
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_REASON_CPU_FMBIST_DONE_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_REASON_CPU_LOCK_OFFSET 3
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_REASON_CPU_LOCK_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_REASON_CPU_PWRDN_OFFSET 4
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_REASON_CPU_PWRDN_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_REASON_CPU_PWR_WDG_OFFSET 5
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_REASON_CPU_PWR_WDG_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_REASON_CPU_WDG_OFFSET 6
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_REASON_CPU_WDG_SIZE 4
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_REASON_LBIST_DONE_OFFSET 10
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_REASON_LBIST_DONE_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_CPU_LOCK_INDICATOR_OFFSET 31
#define KMGO_SYSCTRL_RST_REASON_CPU_RF_CPU_LOCK_INDICATOR_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_reason_cpu_app */
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_REASON_POR_RST_OFFSET 0
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_REASON_POR_RST_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_REASON_SW_RST_OFFSET 1
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_REASON_SW_RST_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_REASON_CPU_FMBIST_DONE_OFFSET 2
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_REASON_CPU_FMBIST_DONE_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_REASON_CPU_LOCK_OFFSET 3
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_REASON_CPU_LOCK_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_REASON_CPU_PWRDN_OFFSET 4
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_REASON_CPU_PWRDN_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_REASON_CPU_PWR_WDG_OFFSET 5
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_REASON_CPU_PWR_WDG_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_REASON_CPU_WDG_OFFSET 6
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_REASON_CPU_WDG_SIZE 4
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_REASON_LBIST_DONE_OFFSET 10
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_REASON_LBIST_DONE_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_CPU_LOCK_INDICATOR_OFFSET 31
#define KMGO_SYSCTRL_RST_REASON_CPU_APP_CPU_LOCK_INDICATOR_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_reason_cpu_phy */
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_REASON_POR_RST_OFFSET 0
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_REASON_POR_RST_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_REASON_SW_RST_OFFSET 1
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_REASON_SW_RST_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_REASON_CPU_FMBIST_DONE_OFFSET 2
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_REASON_CPU_FMBIST_DONE_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_REASON_CPU_LOCK_OFFSET 3
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_REASON_CPU_LOCK_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_REASON_CPU_PWRDN_OFFSET 4
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_REASON_CPU_PWRDN_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_REASON_CPU_PWR_WDG_OFFSET 5
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_REASON_CPU_PWR_WDG_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_REASON_CPU_WDG_OFFSET 6
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_REASON_CPU_WDG_SIZE 4
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_REASON_LBIST_DONE_OFFSET 10
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_REASON_LBIST_DONE_SIZE 1
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_CPU_LOCK_INDICATOR_OFFSET 31
#define KMGO_SYSCTRL_RST_REASON_CPU_PHY_CPU_LOCK_INDICATOR_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_sw_set_1s */
#define KMGO_SYSCTRL_RST_SW_SET_1S_MAIN_OFFSET 0
#define KMGO_SYSCTRL_RST_SW_SET_1S_MAIN_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_CPU_MDM_OFFSET 2
#define KMGO_SYSCTRL_RST_SW_SET_1S_CPU_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_CPU_RF_OFFSET 3
#define KMGO_SYSCTRL_RST_SW_SET_1S_CPU_RF_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_CPU_APP_OFFSET 4
#define KMGO_SYSCTRL_RST_SW_SET_1S_CPU_APP_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_CPU_PHY_OFFSET 5
#define KMGO_SYSCTRL_RST_SW_SET_1S_CPU_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_VSP_OFFSET 6
#define KMGO_SYSCTRL_RST_SW_SET_1S_VSP_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_DBG_OFFSET 8
#define KMGO_SYSCTRL_RST_SW_SET_1S_DBG_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_UART_1_OFFSET 10
#define KMGO_SYSCTRL_RST_SW_SET_1S_UART_1_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_UART_2_OFFSET 11
#define KMGO_SYSCTRL_RST_SW_SET_1S_UART_2_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_UART_3_OFFSET 12
#define KMGO_SYSCTRL_RST_SW_SET_1S_UART_3_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_UART_4_OFFSET 13
#define KMGO_SYSCTRL_RST_SW_SET_1S_UART_4_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_I2C_1_OFFSET 14
#define KMGO_SYSCTRL_RST_SW_SET_1S_I2C_1_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_I2C_2_OFFSET 15
#define KMGO_SYSCTRL_RST_SW_SET_1S_I2C_2_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_I2C_3_OFFSET 16
#define KMGO_SYSCTRL_RST_SW_SET_1S_I2C_3_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_SPI_1_OFFSET 17
#define KMGO_SYSCTRL_RST_SW_SET_1S_SPI_1_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_SPI_2_OFFSET 18
#define KMGO_SYSCTRL_RST_SW_SET_1S_SPI_2_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_SPI_3_OFFSET 19
#define KMGO_SYSCTRL_RST_SW_SET_1S_SPI_3_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_OSPI_OFFSET 20
#define KMGO_SYSCTRL_RST_SW_SET_1S_OSPI_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_PSRAM_OFFSET 21
#define KMGO_SYSCTRL_RST_SW_SET_1S_PSRAM_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_AUDIO_OFFSET 22
#define KMGO_SYSCTRL_RST_SW_SET_1S_AUDIO_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_SDIO_OFFSET 23
#define KMGO_SYSCTRL_RST_SW_SET_1S_SDIO_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_TMR_OFFSET 24
#define KMGO_SYSCTRL_RST_SW_SET_1S_TMR_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_SYSCTRL_OFFSET 26
#define KMGO_SYSCTRL_RST_SW_SET_1S_SYSCTRL_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_ETH_OFFSET 28
#define KMGO_SYSCTRL_RST_SW_SET_1S_ETH_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_USB_OFFSET 29
#define KMGO_SYSCTRL_RST_SW_SET_1S_USB_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_RF_SS_OFFSET 30
#define KMGO_SYSCTRL_RST_SW_SET_1S_RF_SS_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1S_USIM_OFFSET 31
#define KMGO_SYSCTRL_RST_SW_SET_1S_USIM_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_sw_set_1c */
#define KMGO_SYSCTRL_RST_SW_SET_1C_MAIN_OFFSET 0
#define KMGO_SYSCTRL_RST_SW_SET_1C_MAIN_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_CPU_MDM_OFFSET 2
#define KMGO_SYSCTRL_RST_SW_SET_1C_CPU_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_CPU_RF_OFFSET 3
#define KMGO_SYSCTRL_RST_SW_SET_1C_CPU_RF_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_CPU_APP_OFFSET 4
#define KMGO_SYSCTRL_RST_SW_SET_1C_CPU_APP_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_CPU_PHY_OFFSET 5
#define KMGO_SYSCTRL_RST_SW_SET_1C_CPU_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_VSP_OFFSET 6
#define KMGO_SYSCTRL_RST_SW_SET_1C_VSP_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_DBG_OFFSET 8
#define KMGO_SYSCTRL_RST_SW_SET_1C_DBG_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_UART_1_OFFSET 10
#define KMGO_SYSCTRL_RST_SW_SET_1C_UART_1_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_UART_2_OFFSET 11
#define KMGO_SYSCTRL_RST_SW_SET_1C_UART_2_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_UART_3_OFFSET 12
#define KMGO_SYSCTRL_RST_SW_SET_1C_UART_3_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_UART_4_OFFSET 13
#define KMGO_SYSCTRL_RST_SW_SET_1C_UART_4_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_I2C_1_OFFSET 14
#define KMGO_SYSCTRL_RST_SW_SET_1C_I2C_1_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_I2C_2_OFFSET 15
#define KMGO_SYSCTRL_RST_SW_SET_1C_I2C_2_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_I2C_3_OFFSET 16
#define KMGO_SYSCTRL_RST_SW_SET_1C_I2C_3_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_SPI_1_OFFSET 17
#define KMGO_SYSCTRL_RST_SW_SET_1C_SPI_1_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_SPI_2_OFFSET 18
#define KMGO_SYSCTRL_RST_SW_SET_1C_SPI_2_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_SPI_3_OFFSET 19
#define KMGO_SYSCTRL_RST_SW_SET_1C_SPI_3_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_OSPI_OFFSET 20
#define KMGO_SYSCTRL_RST_SW_SET_1C_OSPI_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_PSRAM_OFFSET 21
#define KMGO_SYSCTRL_RST_SW_SET_1C_PSRAM_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_AUDIO_OFFSET 22
#define KMGO_SYSCTRL_RST_SW_SET_1C_AUDIO_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_SDIO_OFFSET 23
#define KMGO_SYSCTRL_RST_SW_SET_1C_SDIO_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_TMR_OFFSET 24
#define KMGO_SYSCTRL_RST_SW_SET_1C_TMR_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_SYSCTRL_OFFSET 26
#define KMGO_SYSCTRL_RST_SW_SET_1C_SYSCTRL_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_ETH_OFFSET 28
#define KMGO_SYSCTRL_RST_SW_SET_1C_ETH_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_USB_OFFSET 29
#define KMGO_SYSCTRL_RST_SW_SET_1C_USB_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_RF_SS_OFFSET 30
#define KMGO_SYSCTRL_RST_SW_SET_1C_RF_SS_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1C_USIM_OFFSET 31
#define KMGO_SYSCTRL_RST_SW_SET_1C_USIM_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_sw_set_1 */
#define KMGO_SYSCTRL_RST_SW_SET_1_MAIN_OFFSET 0
#define KMGO_SYSCTRL_RST_SW_SET_1_MAIN_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_CPU_MDM_OFFSET 2
#define KMGO_SYSCTRL_RST_SW_SET_1_CPU_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_CPU_RF_OFFSET 3
#define KMGO_SYSCTRL_RST_SW_SET_1_CPU_RF_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_CPU_APP_OFFSET 4
#define KMGO_SYSCTRL_RST_SW_SET_1_CPU_APP_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_CPU_PHY_OFFSET 5
#define KMGO_SYSCTRL_RST_SW_SET_1_CPU_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_VSP_OFFSET 6
#define KMGO_SYSCTRL_RST_SW_SET_1_VSP_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_DBG_OFFSET 8
#define KMGO_SYSCTRL_RST_SW_SET_1_DBG_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_UART_1_OFFSET 10
#define KMGO_SYSCTRL_RST_SW_SET_1_UART_1_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_UART_2_OFFSET 11
#define KMGO_SYSCTRL_RST_SW_SET_1_UART_2_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_UART_3_OFFSET 12
#define KMGO_SYSCTRL_RST_SW_SET_1_UART_3_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_UART_4_OFFSET 13
#define KMGO_SYSCTRL_RST_SW_SET_1_UART_4_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_I2C_1_OFFSET 14
#define KMGO_SYSCTRL_RST_SW_SET_1_I2C_1_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_I2C_2_OFFSET 15
#define KMGO_SYSCTRL_RST_SW_SET_1_I2C_2_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_I2C_3_OFFSET 16
#define KMGO_SYSCTRL_RST_SW_SET_1_I2C_3_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_SPI_1_OFFSET 17
#define KMGO_SYSCTRL_RST_SW_SET_1_SPI_1_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_SPI_2_OFFSET 18
#define KMGO_SYSCTRL_RST_SW_SET_1_SPI_2_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_SPI_3_OFFSET 19
#define KMGO_SYSCTRL_RST_SW_SET_1_SPI_3_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_OSPI_OFFSET 20
#define KMGO_SYSCTRL_RST_SW_SET_1_OSPI_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_PSRAM_OFFSET 21
#define KMGO_SYSCTRL_RST_SW_SET_1_PSRAM_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_AUDIO_OFFSET 22
#define KMGO_SYSCTRL_RST_SW_SET_1_AUDIO_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_SDIO_OFFSET 23
#define KMGO_SYSCTRL_RST_SW_SET_1_SDIO_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_TMR_OFFSET 24
#define KMGO_SYSCTRL_RST_SW_SET_1_TMR_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_SYSCTRL_OFFSET 26
#define KMGO_SYSCTRL_RST_SW_SET_1_SYSCTRL_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_ETH_OFFSET 28
#define KMGO_SYSCTRL_RST_SW_SET_1_ETH_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_USB_OFFSET 29
#define KMGO_SYSCTRL_RST_SW_SET_1_USB_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_RF_SS_OFFSET 30
#define KMGO_SYSCTRL_RST_SW_SET_1_RF_SS_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_1_USIM_OFFSET 31
#define KMGO_SYSCTRL_RST_SW_SET_1_USIM_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_sw_set_2s */
#define KMGO_SYSCTRL_RST_SW_SET_2S_PIO_OFFSET 0
#define KMGO_SYSCTRL_RST_SW_SET_2S_PIO_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2S_OTP_OFFSET 1
#define KMGO_SYSCTRL_RST_SW_SET_2S_OTP_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2S_TSCU_SCU_OFFSET 2
#define KMGO_SYSCTRL_RST_SW_SET_2S_TSCU_SCU_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2S_TSCU_OFFSET 3
#define KMGO_SYSCTRL_RST_SW_SET_2S_TSCU_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2S_SF_AO_OFFSET 4
#define KMGO_SYSCTRL_RST_SW_SET_2S_SF_AO_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2S_SF_OFFSET 5
#define KMGO_SYSCTRL_RST_SW_SET_2S_SF_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2S_SECURITY_OFFSET 6
#define KMGO_SYSCTRL_RST_SW_SET_2S_SECURITY_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2S_WDG_MDM_OFFSET 7
#define KMGO_SYSCTRL_RST_SW_SET_2S_WDG_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2S_WDG_APP_OFFSET 8
#define KMGO_SYSCTRL_RST_SW_SET_2S_WDG_APP_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2S_WDG_PHY_OFFSET 9
#define KMGO_SYSCTRL_RST_SW_SET_2S_WDG_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2S_DISPLAY_OFFSET 10
#define KMGO_SYSCTRL_RST_SW_SET_2S_DISPLAY_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_sw_set_2c */
#define KMGO_SYSCTRL_RST_SW_SET_2C_PIO_OFFSET 0
#define KMGO_SYSCTRL_RST_SW_SET_2C_PIO_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2C_OTP_OFFSET 1
#define KMGO_SYSCTRL_RST_SW_SET_2C_OTP_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2C_TSCU_SCU_OFFSET 2
#define KMGO_SYSCTRL_RST_SW_SET_2C_TSCU_SCU_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2C_TSCU_OFFSET 3
#define KMGO_SYSCTRL_RST_SW_SET_2C_TSCU_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2C_SF_AO_OFFSET 4
#define KMGO_SYSCTRL_RST_SW_SET_2C_SF_AO_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2C_SF_OFFSET 5
#define KMGO_SYSCTRL_RST_SW_SET_2C_SF_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2C_SECURITY_OFFSET 6
#define KMGO_SYSCTRL_RST_SW_SET_2C_SECURITY_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2C_WDG_MDM_OFFSET 7
#define KMGO_SYSCTRL_RST_SW_SET_2C_WDG_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2C_WDG_APP_OFFSET 8
#define KMGO_SYSCTRL_RST_SW_SET_2C_WDG_APP_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2C_WDG_PHY_OFFSET 9
#define KMGO_SYSCTRL_RST_SW_SET_2C_WDG_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2C_DISPLAY_OFFSET 10
#define KMGO_SYSCTRL_RST_SW_SET_2C_DISPLAY_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_sw_set_2 */
#define KMGO_SYSCTRL_RST_SW_SET_2_PIO_OFFSET 0
#define KMGO_SYSCTRL_RST_SW_SET_2_PIO_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2_OTP_OFFSET 1
#define KMGO_SYSCTRL_RST_SW_SET_2_OTP_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2_TSCU_SCU_OFFSET 2
#define KMGO_SYSCTRL_RST_SW_SET_2_TSCU_SCU_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2_TSCU_OFFSET 3
#define KMGO_SYSCTRL_RST_SW_SET_2_TSCU_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2_SF_AO_OFFSET 4
#define KMGO_SYSCTRL_RST_SW_SET_2_SF_AO_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2_SF_OFFSET 5
#define KMGO_SYSCTRL_RST_SW_SET_2_SF_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2_SECURITY_OFFSET 6
#define KMGO_SYSCTRL_RST_SW_SET_2_SECURITY_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2_WDG_MDM_OFFSET 7
#define KMGO_SYSCTRL_RST_SW_SET_2_WDG_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2_WDG_APP_OFFSET 8
#define KMGO_SYSCTRL_RST_SW_SET_2_WDG_APP_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2_WDG_PHY_OFFSET 9
#define KMGO_SYSCTRL_RST_SW_SET_2_WDG_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_SW_SET_2_DISPLAY_OFFSET 10
#define KMGO_SYSCTRL_RST_SW_SET_2_DISPLAY_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_sw_strobe_1 */
#define KMGO_SYSCTRL_RST_SW_STROBE_1_MAIN_OFFSET 0
#define KMGO_SYSCTRL_RST_SW_STROBE_1_MAIN_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_CPU_MDM_OFFSET 2
#define KMGO_SYSCTRL_RST_SW_STROBE_1_CPU_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_CPU_RF_OFFSET 3
#define KMGO_SYSCTRL_RST_SW_STROBE_1_CPU_RF_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_CPU_APP_OFFSET 4
#define KMGO_SYSCTRL_RST_SW_STROBE_1_CPU_APP_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_CPU_PHY_OFFSET 5
#define KMGO_SYSCTRL_RST_SW_STROBE_1_CPU_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_VSP_OFFSET 6
#define KMGO_SYSCTRL_RST_SW_STROBE_1_VSP_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_DBG_OFFSET 8
#define KMGO_SYSCTRL_RST_SW_STROBE_1_DBG_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_UART_1_OFFSET 10
#define KMGO_SYSCTRL_RST_SW_STROBE_1_UART_1_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_UART_2_OFFSET 11
#define KMGO_SYSCTRL_RST_SW_STROBE_1_UART_2_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_UART_3_OFFSET 12
#define KMGO_SYSCTRL_RST_SW_STROBE_1_UART_3_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_UART_4_OFFSET 13
#define KMGO_SYSCTRL_RST_SW_STROBE_1_UART_4_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_I2C_1_OFFSET 14
#define KMGO_SYSCTRL_RST_SW_STROBE_1_I2C_1_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_I2C_2_OFFSET 15
#define KMGO_SYSCTRL_RST_SW_STROBE_1_I2C_2_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_I2C_3_OFFSET 16
#define KMGO_SYSCTRL_RST_SW_STROBE_1_I2C_3_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_SPI_1_OFFSET 17
#define KMGO_SYSCTRL_RST_SW_STROBE_1_SPI_1_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_SPI_2_OFFSET 18
#define KMGO_SYSCTRL_RST_SW_STROBE_1_SPI_2_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_SPI_3_OFFSET 19
#define KMGO_SYSCTRL_RST_SW_STROBE_1_SPI_3_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_OSPI_OFFSET 20
#define KMGO_SYSCTRL_RST_SW_STROBE_1_OSPI_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_PSRAM_OFFSET 21
#define KMGO_SYSCTRL_RST_SW_STROBE_1_PSRAM_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_AUDIO_OFFSET 22
#define KMGO_SYSCTRL_RST_SW_STROBE_1_AUDIO_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_SDIO_OFFSET 23
#define KMGO_SYSCTRL_RST_SW_STROBE_1_SDIO_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_TMR_OFFSET 24
#define KMGO_SYSCTRL_RST_SW_STROBE_1_TMR_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_SYSCTRL_OFFSET 26
#define KMGO_SYSCTRL_RST_SW_STROBE_1_SYSCTRL_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_ETH_OFFSET 28
#define KMGO_SYSCTRL_RST_SW_STROBE_1_ETH_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_USB_OFFSET 29
#define KMGO_SYSCTRL_RST_SW_STROBE_1_USB_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_RF_SS_OFFSET 30
#define KMGO_SYSCTRL_RST_SW_STROBE_1_RF_SS_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_1_USIM_OFFSET 31
#define KMGO_SYSCTRL_RST_SW_STROBE_1_USIM_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_sw_strobe_2 */
#define KMGO_SYSCTRL_RST_SW_STROBE_2_PIO_OFFSET 0
#define KMGO_SYSCTRL_RST_SW_STROBE_2_PIO_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_2_OTP_OFFSET 1
#define KMGO_SYSCTRL_RST_SW_STROBE_2_OTP_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_2_TSCU_SCU_OFFSET 2
#define KMGO_SYSCTRL_RST_SW_STROBE_2_TSCU_SCU_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_2_TSCU_OFFSET 3
#define KMGO_SYSCTRL_RST_SW_STROBE_2_TSCU_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_2_SF_AO_OFFSET 4
#define KMGO_SYSCTRL_RST_SW_STROBE_2_SF_AO_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_2_SF_OFFSET 5
#define KMGO_SYSCTRL_RST_SW_STROBE_2_SF_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_2_SECURITY_OFFSET 6
#define KMGO_SYSCTRL_RST_SW_STROBE_2_SECURITY_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_2_WDG_MDM_OFFSET 7
#define KMGO_SYSCTRL_RST_SW_STROBE_2_WDG_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_2_WDG_APP_OFFSET 8
#define KMGO_SYSCTRL_RST_SW_STROBE_2_WDG_APP_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_2_WDG_PHY_OFFSET 9
#define KMGO_SYSCTRL_RST_SW_STROBE_2_WDG_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_SW_STROBE_2_DISPLAY_OFFSET 10
#define KMGO_SYSCTRL_RST_SW_STROBE_2_DISPLAY_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_sw_strobe_count */
#define KMGO_SYSCTRL_RST_SW_STROBE_COUNT_RESET_COUNT_OFFSET 0
#define KMGO_SYSCTRL_RST_SW_STROBE_COUNT_RESET_COUNT_SIZE 20

/** bit field defines for kmgo_sysctrl_s#rst_wdg_mdm_disable_1 */
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_MAIN_OFFSET 0
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_MAIN_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_CPU_MDM_OFFSET 2
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_CPU_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_CPU_RF_OFFSET 3
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_CPU_RF_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_CPU_APP_OFFSET 4
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_CPU_APP_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_CPU_PHY_OFFSET 5
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_CPU_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_VSP_OFFSET 6
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_VSP_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_DBG_OFFSET 8
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_DBG_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_UART_1_OFFSET 10
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_UART_1_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_UART_2_OFFSET 11
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_UART_2_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_UART_3_OFFSET 12
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_UART_3_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_UART_4_OFFSET 13
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_UART_4_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_I2C_1_OFFSET 14
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_I2C_1_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_I2C_2_OFFSET 15
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_I2C_2_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_I2C_3_OFFSET 16
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_I2C_3_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_SPI_1_OFFSET 17
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_SPI_1_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_SPI_2_OFFSET 18
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_SPI_2_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_SPI_3_OFFSET 19
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_SPI_3_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_OSPI_OFFSET 20
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_OSPI_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_PSRAM_OFFSET 21
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_PSRAM_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_AUDIO_OFFSET 22
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_AUDIO_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_SDIO_OFFSET 23
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_SDIO_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_TMR_OFFSET 24
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_TMR_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_SYSCTRL_OFFSET 26
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_SYSCTRL_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_ETH_OFFSET 28
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_ETH_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_USB_OFFSET 29
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_USB_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_RF_SS_OFFSET 30
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_RF_SS_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_USIM_OFFSET 31
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_1_USIM_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_wdg_mdm_disable_2 */
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_PIO_OFFSET 0
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_PIO_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_OTP_OFFSET 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_OTP_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_TSCU_SCU_OFFSET 2
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_TSCU_SCU_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_TSCU_OFFSET 3
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_TSCU_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_SF_AO_OFFSET 4
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_SF_AO_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_SF_OFFSET 5
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_SF_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_SECURITY_OFFSET 6
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_SECURITY_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_WDG_MDM_OFFSET 7
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_WDG_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_WDG_APP_OFFSET 8
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_WDG_APP_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_WDG_PHY_OFFSET 9
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_WDG_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_DISPLAY_OFFSET 10
#define KMGO_SYSCTRL_RST_WDG_MDM_DISABLE_2_DISPLAY_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_wdg_rf_disable_1 */
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_MAIN_OFFSET 0
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_MAIN_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_CPU_MDM_OFFSET 2
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_CPU_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_CPU_RF_OFFSET 3
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_CPU_RF_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_CPU_APP_OFFSET 4
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_CPU_APP_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_CPU_PHY_OFFSET 5
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_CPU_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_VSP_OFFSET 6
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_VSP_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_DBG_OFFSET 8
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_DBG_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_UART_1_OFFSET 10
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_UART_1_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_UART_2_OFFSET 11
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_UART_2_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_UART_3_OFFSET 12
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_UART_3_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_UART_4_OFFSET 13
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_UART_4_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_I2C_1_OFFSET 14
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_I2C_1_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_I2C_2_OFFSET 15
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_I2C_2_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_I2C_3_OFFSET 16
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_I2C_3_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_SPI_1_OFFSET 17
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_SPI_1_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_SPI_2_OFFSET 18
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_SPI_2_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_SPI_3_OFFSET 19
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_SPI_3_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_OSPI_OFFSET 20
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_OSPI_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_PSRAM_OFFSET 21
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_PSRAM_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_AUDIO_OFFSET 22
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_AUDIO_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_SDIO_OFFSET 23
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_SDIO_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_TMR_OFFSET 24
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_TMR_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_SYSCTRL_OFFSET 26
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_SYSCTRL_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_ETH_OFFSET 28
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_ETH_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_USB_OFFSET 29
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_USB_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_RF_SS_OFFSET 30
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_RF_SS_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_USIM_OFFSET 31
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_1_USIM_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_wdg_rf_disable_2 */
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_PIO_OFFSET 0
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_PIO_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_OTP_OFFSET 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_OTP_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_TSCU_SCU_OFFSET 2
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_TSCU_SCU_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_TSCU_OFFSET 3
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_TSCU_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_SF_AO_OFFSET 4
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_SF_AO_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_SF_OFFSET 5
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_SF_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_SECURITY_OFFSET 6
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_SECURITY_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_WDG_MDM_OFFSET 7
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_WDG_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_WDG_APP_OFFSET 8
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_WDG_APP_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_WDG_PHY_OFFSET 9
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_WDG_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_DISPLAY_OFFSET 10
#define KMGO_SYSCTRL_RST_WDG_RF_DISABLE_2_DISPLAY_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_wdg_app_disable_1 */
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_MAIN_OFFSET 0
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_MAIN_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_CPU_MDM_OFFSET 2
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_CPU_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_CPU_RF_OFFSET 3
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_CPU_RF_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_CPU_APP_OFFSET 4
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_CPU_APP_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_CPU_PHY_OFFSET 5
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_CPU_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_VSP_OFFSET 6
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_VSP_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_DBG_OFFSET 8
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_DBG_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_UART_1_OFFSET 10
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_UART_1_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_UART_2_OFFSET 11
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_UART_2_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_UART_3_OFFSET 12
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_UART_3_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_UART_4_OFFSET 13
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_UART_4_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_I2C_1_OFFSET 14
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_I2C_1_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_I2C_2_OFFSET 15
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_I2C_2_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_I2C_3_OFFSET 16
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_I2C_3_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_SPI_1_OFFSET 17
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_SPI_1_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_SPI_2_OFFSET 18
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_SPI_2_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_SPI_3_OFFSET 19
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_SPI_3_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_OSPI_OFFSET 20
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_OSPI_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_PSRAM_OFFSET 21
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_PSRAM_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_AUDIO_OFFSET 22
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_AUDIO_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_SDIO_OFFSET 23
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_SDIO_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_TMR_OFFSET 24
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_TMR_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_SYSCTRL_OFFSET 26
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_SYSCTRL_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_ETH_OFFSET 28
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_ETH_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_USB_OFFSET 29
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_USB_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_RF_SS_OFFSET 30
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_RF_SS_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_USIM_OFFSET 31
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_1_USIM_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_wdg_app_disable_2 */
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_PIO_OFFSET 0
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_PIO_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_OTP_OFFSET 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_OTP_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_TSCU_SCU_OFFSET 2
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_TSCU_SCU_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_TSCU_OFFSET 3
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_TSCU_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_SF_AO_OFFSET 4
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_SF_AO_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_SF_OFFSET 5
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_SF_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_SECURITY_OFFSET 6
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_SECURITY_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_WDG_MDM_OFFSET 7
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_WDG_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_WDG_APP_OFFSET 8
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_WDG_APP_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_WDG_PHY_OFFSET 9
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_WDG_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_DISPLAY_OFFSET 10
#define KMGO_SYSCTRL_RST_WDG_APP_DISABLE_2_DISPLAY_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_wdg_phy_disable_1 */
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_MAIN_OFFSET 0
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_MAIN_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_CPU_MDM_OFFSET 2
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_CPU_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_CPU_RF_OFFSET 3
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_CPU_RF_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_CPU_APP_OFFSET 4
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_CPU_APP_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_CPU_PHY_OFFSET 5
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_CPU_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_VSP_OFFSET 6
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_VSP_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_DBG_OFFSET 8
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_DBG_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_UART_1_OFFSET 10
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_UART_1_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_UART_2_OFFSET 11
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_UART_2_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_UART_3_OFFSET 12
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_UART_3_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_UART_4_OFFSET 13
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_UART_4_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_I2C_1_OFFSET 14
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_I2C_1_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_I2C_2_OFFSET 15
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_I2C_2_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_I2C_3_OFFSET 16
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_I2C_3_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_SPI_1_OFFSET 17
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_SPI_1_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_SPI_2_OFFSET 18
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_SPI_2_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_SPI_3_OFFSET 19
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_SPI_3_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_OSPI_OFFSET 20
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_OSPI_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_PSRAM_OFFSET 21
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_PSRAM_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_AUDIO_OFFSET 22
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_AUDIO_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_SDIO_OFFSET 23
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_SDIO_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_TMR_OFFSET 24
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_TMR_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_SYSCTRL_OFFSET 26
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_SYSCTRL_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_ETH_OFFSET 28
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_ETH_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_USB_OFFSET 29
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_USB_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_RF_SS_OFFSET 30
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_RF_SS_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_USIM_OFFSET 31
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_1_USIM_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_wdg_phy_disable_2 */
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_PIO_OFFSET 0
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_PIO_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_OTP_OFFSET 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_OTP_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_TSCU_SCU_OFFSET 2
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_TSCU_SCU_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_TSCU_OFFSET 3
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_TSCU_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_SF_AO_OFFSET 4
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_SF_AO_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_SF_OFFSET 5
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_SF_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_SECURITY_OFFSET 6
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_SECURITY_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_WDG_MDM_OFFSET 7
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_WDG_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_WDG_APP_OFFSET 8
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_WDG_APP_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_WDG_PHY_OFFSET 9
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_WDG_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_DISPLAY_OFFSET 10
#define KMGO_SYSCTRL_RST_WDG_PHY_DISABLE_2_DISPLAY_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_ctrl */
#define KMGO_SYSCTRL_RST_CTRL_PREWARN_ENABLE_WDG_MDM_OFFSET 0
#define KMGO_SYSCTRL_RST_CTRL_PREWARN_ENABLE_WDG_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_CTRL_PREWARN_ENABLE_WDG_RF_OFFSET 1
#define KMGO_SYSCTRL_RST_CTRL_PREWARN_ENABLE_WDG_RF_SIZE 1
#define KMGO_SYSCTRL_RST_CTRL_PREWARN_ENABLE_WDG_APP_OFFSET 2
#define KMGO_SYSCTRL_RST_CTRL_PREWARN_ENABLE_WDG_APP_SIZE 1
#define KMGO_SYSCTRL_RST_CTRL_PREWARN_ENABLE_WDG_PHY_OFFSET 3
#define KMGO_SYSCTRL_RST_CTRL_PREWARN_ENABLE_WDG_PHY_SIZE 1

/** bit field defines for kmgo_sysctrl_s#sys_gp */
#define KMGO_SYSCTRL_SYS_GP_GP_OFFSET 0
#define KMGO_SYSCTRL_SYS_GP_GP_SIZE 8

/** bit field defines for kmgo_sysctrl_s#rf_ccu_ctrl */
#define KMGO_SYSCTRL_RF_CCU_CTRL_BYPASS_OFFSET 0
#define KMGO_SYSCTRL_RF_CCU_CTRL_BYPASS_SIZE 1
#define KMGO_SYSCTRL_RF_CCU_CTRL_FORCE_RESET_DISABLE_OFFSET 1
#define KMGO_SYSCTRL_RF_CCU_CTRL_FORCE_RESET_DISABLE_SIZE 1

/** bit field defines for kmgo_sysctrl_s#peripheral_id4 */
#define KMGO_SYSCTRL_PERIPHERAL_ID4_NONE_OFFSET 0
#define KMGO_SYSCTRL_PERIPHERAL_ID4_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#peripheral_id5 */
#define KMGO_SYSCTRL_PERIPHERAL_ID5_NONE_OFFSET 0
#define KMGO_SYSCTRL_PERIPHERAL_ID5_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#peripheral_id6 */
#define KMGO_SYSCTRL_PERIPHERAL_ID6_NONE_OFFSET 0
#define KMGO_SYSCTRL_PERIPHERAL_ID6_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#peripheral_id7 */
#define KMGO_SYSCTRL_PERIPHERAL_ID7_NONE_OFFSET 0
#define KMGO_SYSCTRL_PERIPHERAL_ID7_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#peripheral_id0 */
#define KMGO_SYSCTRL_PERIPHERAL_ID0_NONE_OFFSET 0
#define KMGO_SYSCTRL_PERIPHERAL_ID0_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#peripheral_id1 */
#define KMGO_SYSCTRL_PERIPHERAL_ID1_NONE_OFFSET 0
#define KMGO_SYSCTRL_PERIPHERAL_ID1_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#peripheral_id2 */
#define KMGO_SYSCTRL_PERIPHERAL_ID2_NONE_OFFSET 0
#define KMGO_SYSCTRL_PERIPHERAL_ID2_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#peripheral_id3 */
#define KMGO_SYSCTRL_PERIPHERAL_ID3_NONE_OFFSET 0
#define KMGO_SYSCTRL_PERIPHERAL_ID3_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#component_id0 */
#define KMGO_SYSCTRL_COMPONENT_ID0_NONE_OFFSET 0
#define KMGO_SYSCTRL_COMPONENT_ID0_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#component_id1 */
#define KMGO_SYSCTRL_COMPONENT_ID1_NONE_OFFSET 0
#define KMGO_SYSCTRL_COMPONENT_ID1_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#component_id2 */
#define KMGO_SYSCTRL_COMPONENT_ID2_NONE_OFFSET 0
#define KMGO_SYSCTRL_COMPONENT_ID2_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#component_id3 */
#define KMGO_SYSCTRL_COMPONENT_ID3_NONE_OFFSET 0
#define KMGO_SYSCTRL_COMPONENT_ID3_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#func_mbist_ctrl */
#define KMGO_SYSCTRL_FUNC_MBIST_CTRL_START_OFFSET 0
#define KMGO_SYSCTRL_FUNC_MBIST_CTRL_START_SIZE 1

/** bit field defines for kmgo_sysctrl_s#func_mbist_stat */
#define KMGO_SYSCTRL_FUNC_MBIST_STAT_DONE_OFFSET 0
#define KMGO_SYSCTRL_FUNC_MBIST_STAT_DONE_SIZE 1
#define KMGO_SYSCTRL_FUNC_MBIST_STAT_BUSY_OFFSET 1
#define KMGO_SYSCTRL_FUNC_MBIST_STAT_BUSY_SIZE 1

/** bit field defines for kmgo_sysctrl_s#func_mbist_enable_1 */
#define KMGO_SYSCTRL_FUNC_MBIST_ENABLE_1_NONE_OFFSET 0
#define KMGO_SYSCTRL_FUNC_MBIST_ENABLE_1_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#func_mbist_enable_2 */
#define KMGO_SYSCTRL_FUNC_MBIST_ENABLE_2_NONE_OFFSET 0
#define KMGO_SYSCTRL_FUNC_MBIST_ENABLE_2_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#func_mbist_fail_1 */
#define KMGO_SYSCTRL_FUNC_MBIST_FAIL_1_NONE_OFFSET 0
#define KMGO_SYSCTRL_FUNC_MBIST_FAIL_1_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#func_mbist_fail_2 */
#define KMGO_SYSCTRL_FUNC_MBIST_FAIL_2_NONE_OFFSET 0
#define KMGO_SYSCTRL_FUNC_MBIST_FAIL_2_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#func_mbist_done_1 */
#define KMGO_SYSCTRL_FUNC_MBIST_DONE_1_NONE_OFFSET 0
#define KMGO_SYSCTRL_FUNC_MBIST_DONE_1_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#func_mbist_done_2 */
#define KMGO_SYSCTRL_FUNC_MBIST_DONE_2_NONE_OFFSET 0
#define KMGO_SYSCTRL_FUNC_MBIST_DONE_2_NONE_SIZE 32

/** bit field defines for kmgo_sysctrl_s#rst_func_mbist_disable_1 */
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_MAIN_OFFSET 0
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_MAIN_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_CPU_MDM_OFFSET 2
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_CPU_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_CPU_RF_OFFSET 3
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_CPU_RF_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_CPU_APP_OFFSET 4
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_CPU_APP_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_CPU_PHY_OFFSET 5
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_CPU_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_VSP_OFFSET 6
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_VSP_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_DBG_OFFSET 8
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_DBG_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_UART_1_OFFSET 10
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_UART_1_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_UART_2_OFFSET 11
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_UART_2_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_UART_3_OFFSET 12
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_UART_3_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_UART_4_OFFSET 13
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_UART_4_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_I2C_1_OFFSET 14
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_I2C_1_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_I2C_2_OFFSET 15
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_I2C_2_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_I2C_3_OFFSET 16
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_I2C_3_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_SPI_1_OFFSET 17
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_SPI_1_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_SPI_2_OFFSET 18
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_SPI_2_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_SPI_3_OFFSET 19
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_SPI_3_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_OSPI_OFFSET 20
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_OSPI_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_PSRAM_OFFSET 21
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_PSRAM_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_AUDIO_OFFSET 22
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_AUDIO_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_SDIO_OFFSET 23
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_SDIO_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_TMR_OFFSET 24
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_TMR_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_SYSCTRL_OFFSET 26
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_SYSCTRL_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_ETH_OFFSET 28
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_ETH_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_USB_OFFSET 29
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_USB_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_RF_SS_OFFSET 30
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_RF_SS_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_USIM_OFFSET 31
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_1_USIM_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_func_mbist_disable_2 */
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_PIO_OFFSET 0
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_PIO_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_OTP_OFFSET 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_OTP_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_TSCU_SCU_OFFSET 2
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_TSCU_SCU_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_TSCU_OFFSET 3
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_TSCU_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_SF_AO_OFFSET 4
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_SF_AO_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_SF_OFFSET 5
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_SF_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_SECURITY_OFFSET 6
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_SECURITY_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_WDG_MDM_OFFSET 7
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_WDG_MDM_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_WDG_APP_OFFSET 8
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_WDG_APP_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_WDG_PHY_OFFSET 9
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_WDG_PHY_SIZE 1
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_DISPLAY_OFFSET 10
#define KMGO_SYSCTRL_RST_FUNC_MBIST_DISABLE_2_DISPLAY_SIZE 1

/** bit field defines for kmgo_sysctrl_s#rst_ctrl_status */
#define KMGO_SYSCTRL_RST_CTRL_STATUS_SW_RST_CMD_REQ_OFFSET 0
#define KMGO_SYSCTRL_RST_CTRL_STATUS_SW_RST_CMD_REQ_SIZE 1
#define KMGO_SYSCTRL_RST_CTRL_STATUS_SW_RST_CMD_ACK_OFFSET 1
#define KMGO_SYSCTRL_RST_CTRL_STATUS_SW_RST_CMD_ACK_SIZE 1

/** bit field defines for kmgo_sysctrl_s#pll1_freq_conf */
#define KMGO_SYSCTRL_PLL1_FREQ_CONF_PLLDIVFF_OFFSET 0
#define KMGO_SYSCTRL_PLL1_FREQ_CONF_PLLDIVFF_SIZE 24
#define KMGO_SYSCTRL_PLL1_FREQ_CONF_PLLDIVFI_OFFSET 24
#define KMGO_SYSCTRL_PLL1_FREQ_CONF_PLLDIVFI_SIZE 8

/** bit field defines for kmgo_sysctrl_s#pll1_pdiv1_conf */
#define KMGO_SYSCTRL_PLL1_PDIV1_CONF_PDIVEN_OFFSET 0
#define KMGO_SYSCTRL_PLL1_PDIV1_CONF_PDIVEN_SIZE 1
#define KMGO_SYSCTRL_PLL1_PDIV1_CONF_PDIVQ_OFFSET 1
#define KMGO_SYSCTRL_PLL1_PDIV1_CONF_PDIVQ_SIZE 5

/** bit field defines for kmgo_sysctrl_s#pll1_pdiv2_conf */
#define KMGO_SYSCTRL_PLL1_PDIV2_CONF_PDIVEN_OFFSET 0
#define KMGO_SYSCTRL_PLL1_PDIV2_CONF_PDIVEN_SIZE 1
#define KMGO_SYSCTRL_PLL1_PDIV2_CONF_PDIVQ_OFFSET 1
#define KMGO_SYSCTRL_PLL1_PDIV2_CONF_PDIVQ_SIZE 5

/** bit field defines for kmgo_sysctrl_s#pll1_ctrl */
#define KMGO_SYSCTRL_PLL1_CTRL_PLLBYPASS_OFFSET 0
#define KMGO_SYSCTRL_PLL1_CTRL_PLLBYPASS_SIZE 1
#define KMGO_SYSCTRL_PLL1_CTRL_PLLREFRANGE_OFFSET 2
#define KMGO_SYSCTRL_PLL1_CTRL_PLLREFRANGE_SIZE 1
#define KMGO_SYSCTRL_PLL1_CTRL_PLLPWRDN_OFFSET 3
#define KMGO_SYSCTRL_PLL1_CTRL_PLLPWRDN_SIZE 1
#define KMGO_SYSCTRL_PLL1_CTRL_PLLUPDATE_OFFSET 8
#define KMGO_SYSCTRL_PLL1_CTRL_PLLUPDATE_SIZE 1

/** bit field defines for kmgo_sysctrl_s#pll1_status */
#define KMGO_SYSCTRL_PLL1_STATUS_LOCKCNTOVER_OFFSET 0
#define KMGO_SYSCTRL_PLL1_STATUS_LOCKCNTOVER_SIZE 1
#define KMGO_SYSCTRL_PLL1_STATUS_PLLLOCK_OFFSET 1
#define KMGO_SYSCTRL_PLL1_STATUS_PLLLOCK_SIZE 1
#define KMGO_SYSCTRL_PLL1_STATUS_PLLDIVACK_OFFSET 2
#define KMGO_SYSCTRL_PLL1_STATUS_PLLDIVACK_SIZE 1

/** bit field defines for kmgo_sysctrl_s#pll2_freq_conf */
#define KMGO_SYSCTRL_PLL2_FREQ_CONF_PLLDIVFF_OFFSET 0
#define KMGO_SYSCTRL_PLL2_FREQ_CONF_PLLDIVFF_SIZE 24
#define KMGO_SYSCTRL_PLL2_FREQ_CONF_PLLDIVFI_OFFSET 24
#define KMGO_SYSCTRL_PLL2_FREQ_CONF_PLLDIVFI_SIZE 8

/** bit field defines for kmgo_sysctrl_s#pll2_pdiv1_conf */
#define KMGO_SYSCTRL_PLL2_PDIV1_CONF_PDIVEN_OFFSET 0
#define KMGO_SYSCTRL_PLL2_PDIV1_CONF_PDIVEN_SIZE 1
#define KMGO_SYSCTRL_PLL2_PDIV1_CONF_PDIVQ_OFFSET 1
#define KMGO_SYSCTRL_PLL2_PDIV1_CONF_PDIVQ_SIZE 5

/** bit field defines for kmgo_sysctrl_s#pll2_pdiv2_conf */
#define KMGO_SYSCTRL_PLL2_PDIV2_CONF_PDIVEN_OFFSET 0
#define KMGO_SYSCTRL_PLL2_PDIV2_CONF_PDIVEN_SIZE 1
#define KMGO_SYSCTRL_PLL2_PDIV2_CONF_PDIVQ_OFFSET 1
#define KMGO_SYSCTRL_PLL2_PDIV2_CONF_PDIVQ_SIZE 5

/** bit field defines for kmgo_sysctrl_s#pll2_ctrl */
#define KMGO_SYSCTRL_PLL2_CTRL_PLLBYPASS_OFFSET 0
#define KMGO_SYSCTRL_PLL2_CTRL_PLLBYPASS_SIZE 1
#define KMGO_SYSCTRL_PLL2_CTRL_PLLREFRANGE_OFFSET 2
#define KMGO_SYSCTRL_PLL2_CTRL_PLLREFRANGE_SIZE 1
#define KMGO_SYSCTRL_PLL2_CTRL_PLLPWRDN_OFFSET 3
#define KMGO_SYSCTRL_PLL2_CTRL_PLLPWRDN_SIZE 1
#define KMGO_SYSCTRL_PLL2_CTRL_PLLUPDATE_OFFSET 8
#define KMGO_SYSCTRL_PLL2_CTRL_PLLUPDATE_SIZE 1

/** bit field defines for kmgo_sysctrl_s#pll2_status */
#define KMGO_SYSCTRL_PLL2_STATUS_LOCKCNTOVER_OFFSET 0
#define KMGO_SYSCTRL_PLL2_STATUS_LOCKCNTOVER_SIZE 1
#define KMGO_SYSCTRL_PLL2_STATUS_PLLLOCK_OFFSET 1
#define KMGO_SYSCTRL_PLL2_STATUS_PLLLOCK_SIZE 1
#define KMGO_SYSCTRL_PLL2_STATUS_PLLDIVACK_OFFSET 2
#define KMGO_SYSCTRL_PLL2_STATUS_PLLDIVACK_SIZE 1

/** bit field defines for kmgo_sysctrl_s#pll_lockcnt */
#define KMGO_SYSCTRL_PLL_LOCKCNT_PLLLOCKWAIT_OFFSET 0
#define KMGO_SYSCTRL_PLL_LOCKCNT_PLLLOCKWAIT_SIZE 16

/** bit field defines for kmgo_sysctrl_s#rtclk_vsp_conf */
#define KMGO_SYSCTRL_RTCLK_VSP_CONF_CLKBYPASS_OFFSET 0
#define KMGO_SYSCTRL_RTCLK_VSP_CONF_CLKBYPASS_SIZE 1
#define KMGO_SYSCTRL_RTCLK_VSP_CONF_CLKBYPASS_SEL_OFFSET 1
#define KMGO_SYSCTRL_RTCLK_VSP_CONF_CLKBYPASS_SEL_SIZE 1
#define KMGO_SYSCTRL_RTCLK_VSP_CONF_CLKSRCSEL_OFFSET 3
#define KMGO_SYSCTRL_RTCLK_VSP_CONF_CLKSRCSEL_SIZE 2
#define KMGO_SYSCTRL_RTCLK_VSP_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_RTCLK_VSP_CONF_MUXEDCLK_STATUS_SIZE 3

/** bit field defines for kmgo_sysctrl_s#rtclk_cpu_conf */
#define KMGO_SYSCTRL_RTCLK_CPU_CONF_CLKBYPASS_OFFSET 0
#define KMGO_SYSCTRL_RTCLK_CPU_CONF_CLKBYPASS_SIZE 1
#define KMGO_SYSCTRL_RTCLK_CPU_CONF_CLKBYPASS_SEL_OFFSET 1
#define KMGO_SYSCTRL_RTCLK_CPU_CONF_CLKBYPASS_SEL_SIZE 1
#define KMGO_SYSCTRL_RTCLK_CPU_CONF_CLKSRCSEL_OFFSET 3
#define KMGO_SYSCTRL_RTCLK_CPU_CONF_CLKSRCSEL_SIZE 2
#define KMGO_SYSCTRL_RTCLK_CPU_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_RTCLK_CPU_CONF_MUXEDCLK_STATUS_SIZE 3

/** bit field defines for kmgo_sysctrl_s#rtclk_extif_conf */
#define KMGO_SYSCTRL_RTCLK_EXTIF_CONF_CLKBYPASS_OFFSET 0
#define KMGO_SYSCTRL_RTCLK_EXTIF_CONF_CLKBYPASS_SIZE 1
#define KMGO_SYSCTRL_RTCLK_EXTIF_CONF_CLKBYPASS_SEL_OFFSET 1
#define KMGO_SYSCTRL_RTCLK_EXTIF_CONF_CLKBYPASS_SEL_SIZE 1
#define KMGO_SYSCTRL_RTCLK_EXTIF_CONF_CLKSRCSEL_OFFSET 3
#define KMGO_SYSCTRL_RTCLK_EXTIF_CONF_CLKSRCSEL_SIZE 2
#define KMGO_SYSCTRL_RTCLK_EXTIF_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_RTCLK_EXTIF_CONF_MUXEDCLK_STATUS_SIZE 3

/** bit field defines for kmgo_sysctrl_s#rtclk_extif_fast_conf */
#define KMGO_SYSCTRL_RTCLK_EXTIF_FAST_CONF_CLKBYPASS_OFFSET 0
#define KMGO_SYSCTRL_RTCLK_EXTIF_FAST_CONF_CLKBYPASS_SIZE 1
#define KMGO_SYSCTRL_RTCLK_EXTIF_FAST_CONF_CLKBYPASS_SEL_OFFSET 1
#define KMGO_SYSCTRL_RTCLK_EXTIF_FAST_CONF_CLKBYPASS_SEL_SIZE 1
#define KMGO_SYSCTRL_RTCLK_EXTIF_FAST_CONF_CLKSRCSEL_OFFSET 3
#define KMGO_SYSCTRL_RTCLK_EXTIF_FAST_CONF_CLKSRCSEL_SIZE 2
#define KMGO_SYSCTRL_RTCLK_EXTIF_FAST_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_RTCLK_EXTIF_FAST_CONF_MUXEDCLK_STATUS_SIZE 3

/** bit field defines for kmgo_sysctrl_s#rtclk_eth_conf */
#define KMGO_SYSCTRL_RTCLK_ETH_CONF_CLKBYPASS_OFFSET 0
#define KMGO_SYSCTRL_RTCLK_ETH_CONF_CLKBYPASS_SIZE 1
#define KMGO_SYSCTRL_RTCLK_ETH_CONF_CLKBYPASS_SEL_OFFSET 1
#define KMGO_SYSCTRL_RTCLK_ETH_CONF_CLKBYPASS_SEL_SIZE 1
#define KMGO_SYSCTRL_RTCLK_ETH_CONF_CLKSRCSEL_OFFSET 3
#define KMGO_SYSCTRL_RTCLK_ETH_CONF_CLKSRCSEL_SIZE 2
#define KMGO_SYSCTRL_RTCLK_ETH_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_RTCLK_ETH_CONF_MUXEDCLK_STATUS_SIZE 3

/** bit field defines for kmgo_sysctrl_s#rtclk_usb_conf */
#define KMGO_SYSCTRL_RTCLK_USB_CONF_CLKBYPASS_OFFSET 0
#define KMGO_SYSCTRL_RTCLK_USB_CONF_CLKBYPASS_SIZE 1
#define KMGO_SYSCTRL_RTCLK_USB_CONF_CLKBYPASS_SEL_OFFSET 1
#define KMGO_SYSCTRL_RTCLK_USB_CONF_CLKBYPASS_SEL_SIZE 1
#define KMGO_SYSCTRL_RTCLK_USB_CONF_CLKSRCSEL_OFFSET 3
#define KMGO_SYSCTRL_RTCLK_USB_CONF_CLKSRCSEL_SIZE 2
#define KMGO_SYSCTRL_RTCLK_USB_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_RTCLK_USB_CONF_MUXEDCLK_STATUS_SIZE 3

/** bit field defines for kmgo_sysctrl_s#clk_main_conf */
#define KMGO_SYSCTRL_CLK_MAIN_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_MAIN_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_MAIN_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_MAIN_CONF_CLKDIVSEL_SIZE 5

/** bit field defines for kmgo_sysctrl_s#clk_periph_conf */
#define KMGO_SYSCTRL_CLK_PERIPH_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_PERIPH_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_PERIPH_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_PERIPH_CONF_CLKDIVSEL_SIZE 5

/** bit field defines for kmgo_sysctrl_s#clk_cpu_mdm_conf */
#define KMGO_SYSCTRL_CLK_CPU_MDM_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_CPU_MDM_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_CPU_MDM_CONF_TSCLKCHANGE_OFFSET 12
#define KMGO_SYSCTRL_CLK_CPU_MDM_CONF_TSCLKCHANGE_SIZE 1
#define KMGO_SYSCTRL_CLK_CPU_MDM_CONF_ENABLE_LOCAL_CLK_GATING_OFFSET 13
#define KMGO_SYSCTRL_CLK_CPU_MDM_CONF_ENABLE_LOCAL_CLK_GATING_SIZE 1
#define KMGO_SYSCTRL_CLK_CPU_MDM_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_CPU_MDM_CONF_CLKDIVSEL_SIZE 5

/** bit field defines for kmgo_sysctrl_s#clk_cpu_rf_conf */
#define KMGO_SYSCTRL_CLK_CPU_RF_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_CPU_RF_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_CPU_RF_CONF_TSCLKCHANGE_OFFSET 12
#define KMGO_SYSCTRL_CLK_CPU_RF_CONF_TSCLKCHANGE_SIZE 1
#define KMGO_SYSCTRL_CLK_CPU_RF_CONF_ENABLE_LOCAL_CLK_GATING_OFFSET 13
#define KMGO_SYSCTRL_CLK_CPU_RF_CONF_ENABLE_LOCAL_CLK_GATING_SIZE 1
#define KMGO_SYSCTRL_CLK_CPU_RF_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_CPU_RF_CONF_CLKDIVSEL_SIZE 6

/** bit field defines for kmgo_sysctrl_s#clk_cpu_app_conf */
#define KMGO_SYSCTRL_CLK_CPU_APP_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_CPU_APP_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_CPU_APP_CONF_TSCLKCHANGE_OFFSET 12
#define KMGO_SYSCTRL_CLK_CPU_APP_CONF_TSCLKCHANGE_SIZE 1
#define KMGO_SYSCTRL_CLK_CPU_APP_CONF_ENABLE_LOCAL_CLK_GATING_OFFSET 13
#define KMGO_SYSCTRL_CLK_CPU_APP_CONF_ENABLE_LOCAL_CLK_GATING_SIZE 1
#define KMGO_SYSCTRL_CLK_CPU_APP_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_CPU_APP_CONF_CLKDIVSEL_SIZE 5

/** bit field defines for kmgo_sysctrl_s#clk_cpu_phy_conf */
#define KMGO_SYSCTRL_CLK_CPU_PHY_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_CPU_PHY_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_CPU_PHY_CONF_TSCLKCHANGE_OFFSET 12
#define KMGO_SYSCTRL_CLK_CPU_PHY_CONF_TSCLKCHANGE_SIZE 1
#define KMGO_SYSCTRL_CLK_CPU_PHY_CONF_ENABLE_LOCAL_CLK_GATING_OFFSET 13
#define KMGO_SYSCTRL_CLK_CPU_PHY_CONF_ENABLE_LOCAL_CLK_GATING_SIZE 1
#define KMGO_SYSCTRL_CLK_CPU_PHY_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_CPU_PHY_CONF_CLKDIVSEL_SIZE 5

/** bit field defines for kmgo_sysctrl_s#clk_tpiu_conf */
#define KMGO_SYSCTRL_CLK_TPIU_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_TPIU_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_TPIU_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_TPIU_CONF_CLKDIVSEL_SIZE 6

/** bit field defines for kmgo_sysctrl_s#clk_dbg_conf */
#define KMGO_SYSCTRL_CLK_DBG_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_DBG_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_DBG_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_DBG_CONF_CLKDIVSEL_SIZE 6

/** bit field defines for kmgo_sysctrl_s#clk_vsp_conf */
#define KMGO_SYSCTRL_CLK_VSP_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_VSP_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_VSP_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_VSP_CONF_CLKDIVSEL_SIZE 6

/** bit field defines for kmgo_sysctrl_s#clk_uart_conf */
#define KMGO_SYSCTRL_CLK_UART_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_UART_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_UART_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_UART_CONF_CLKDIVSEL_SIZE 6

/** bit field defines for kmgo_sysctrl_s#clk_audio_conf */
#define KMGO_SYSCTRL_CLK_AUDIO_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_AUDIO_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_AUDIO_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_AUDIO_CONF_CLKDIVSEL_SIZE 6

/** bit field defines for kmgo_sysctrl_s#clk_ospi_conf */
#define KMGO_SYSCTRL_CLK_OSPI_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_OSPI_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_OSPI_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_OSPI_CONF_CLKDIVSEL_SIZE 6

/** bit field defines for kmgo_sysctrl_s#clk_psram_conf */
#define KMGO_SYSCTRL_CLK_PSRAM_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_PSRAM_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_PSRAM_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_PSRAM_CONF_CLKDIVSEL_SIZE 6

/** bit field defines for kmgo_sysctrl_s#clk_spi_conf */
#define KMGO_SYSCTRL_CLK_SPI_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_SPI_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_SPI_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_SPI_CONF_CLKDIVSEL_SIZE 6

/** bit field defines for kmgo_sysctrl_s#clk_i2c_conf */
#define KMGO_SYSCTRL_CLK_I2C_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_I2C_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_I2C_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_I2C_CONF_CLKDIVSEL_SIZE 6

/** bit field defines for kmgo_sysctrl_s#clk_sdio_conf */
#define KMGO_SYSCTRL_CLK_SDIO_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_SDIO_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_SDIO_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_SDIO_CONF_CLKDIVSEL_SIZE 6

/** bit field defines for kmgo_sysctrl_s#clk_rftsf_conf */
#define KMGO_SYSCTRL_CLK_RFTSF_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_RFTSF_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_RFTSF_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_RFTSF_CONF_CLKDIVSEL_SIZE 10

/** bit field defines for kmgo_sysctrl_s#clk_cpu_tscu_conf */
#define KMGO_SYSCTRL_CLK_CPU_TSCU_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_CPU_TSCU_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_CPU_TSCU_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_CPU_TSCU_CONF_CLKDIVSEL_SIZE 5

/** bit field defines for kmgo_sysctrl_s#clk_tmr_conf */
#define KMGO_SYSCTRL_CLK_TMR_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_TMR_CONF_MUXEDCLK_STATUS_SIZE 3

/** bit field defines for kmgo_sysctrl_s#clk_tmr_cpu_conf */
#define KMGO_SYSCTRL_CLK_TMR_CPU_CONF_MUXEDCLK_STATUS_OFFSET 8
#define KMGO_SYSCTRL_CLK_TMR_CPU_CONF_MUXEDCLK_STATUS_SIZE 3
#define KMGO_SYSCTRL_CLK_TMR_CPU_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_TMR_CPU_CONF_CLKDIVSEL_SIZE 6

/** bit field defines for kmgo_sysctrl_s#clk_pll1_tmfltrng_conf */
#define KMGO_SYSCTRL_CLK_PLL1_TMFLTRNG_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_PLL1_TMFLTRNG_CONF_CLKDIVSEL_SIZE 4

/** bit field defines for kmgo_sysctrl_s#clk_pll2_tmfltrng_conf */
#define KMGO_SYSCTRL_CLK_PLL2_TMFLTRNG_CONF_CLKDIVSEL_OFFSET 16
#define KMGO_SYSCTRL_CLK_PLL2_TMFLTRNG_CONF_CLKDIVSEL_SIZE 4

/** bit field defines for kmgo_sysctrl_s#clk_audio_nco_conf */
#define KMGO_SYSCTRL_CLK_AUDIO_NCO_CONF_NCOCONTROL_OFFSET 0
#define KMGO_SYSCTRL_CLK_AUDIO_NCO_CONF_NCOCONTROL_SIZE 24
#define KMGO_SYSCTRL_CLK_AUDIO_NCO_CONF_NCOENABLE_OFFSET 31
#define KMGO_SYSCTRL_CLK_AUDIO_NCO_CONF_NCOENABLE_SIZE 1

/** bit field defines for kmgo_sysctrl_s#clken_1_set */
#define KMGO_SYSCTRL_CLKEN_1_SET_CPU_MDM_OFFSET 2
#define KMGO_SYSCTRL_CLKEN_1_SET_CPU_MDM_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_CPU_RF_OFFSET 3
#define KMGO_SYSCTRL_CLKEN_1_SET_CPU_RF_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_CPU_APP_OFFSET 4
#define KMGO_SYSCTRL_CLKEN_1_SET_CPU_APP_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_CPU_PHY_OFFSET 5
#define KMGO_SYSCTRL_CLKEN_1_SET_CPU_PHY_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_VSP_OFFSET 6
#define KMGO_SYSCTRL_CLKEN_1_SET_VSP_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_TPIU_OFFSET 7
#define KMGO_SYSCTRL_CLKEN_1_SET_TPIU_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_DBG_OFFSET 8
#define KMGO_SYSCTRL_CLKEN_1_SET_DBG_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_RFTSF_OFFSET 9
#define KMGO_SYSCTRL_CLKEN_1_SET_RFTSF_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_UART_1_OFFSET 10
#define KMGO_SYSCTRL_CLKEN_1_SET_UART_1_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_UART_2_OFFSET 11
#define KMGO_SYSCTRL_CLKEN_1_SET_UART_2_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_UART_3_OFFSET 12
#define KMGO_SYSCTRL_CLKEN_1_SET_UART_3_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_UART_4_OFFSET 13
#define KMGO_SYSCTRL_CLKEN_1_SET_UART_4_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_I2C_1_OFFSET 14
#define KMGO_SYSCTRL_CLKEN_1_SET_I2C_1_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_I2C_2_OFFSET 15
#define KMGO_SYSCTRL_CLKEN_1_SET_I2C_2_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_I2C_3_OFFSET 16
#define KMGO_SYSCTRL_CLKEN_1_SET_I2C_3_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_SPI_1_OFFSET 17
#define KMGO_SYSCTRL_CLKEN_1_SET_SPI_1_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_SPI_2_OFFSET 18
#define KMGO_SYSCTRL_CLKEN_1_SET_SPI_2_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_SPI_3_OFFSET 19
#define KMGO_SYSCTRL_CLKEN_1_SET_SPI_3_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_OSPI_OFFSET 20
#define KMGO_SYSCTRL_CLKEN_1_SET_OSPI_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_PSRAM_OFFSET 21
#define KMGO_SYSCTRL_CLKEN_1_SET_PSRAM_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_AUDIO_OFFSET 22
#define KMGO_SYSCTRL_CLKEN_1_SET_AUDIO_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_SDIO_OFFSET 23
#define KMGO_SYSCTRL_CLKEN_1_SET_SDIO_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_TMR_OFFSET 24
#define KMGO_SYSCTRL_CLKEN_1_SET_TMR_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_TMR_CPU_OFFSET 25
#define KMGO_SYSCTRL_CLKEN_1_SET_TMR_CPU_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_ETHTX_OFFSET 27
#define KMGO_SYSCTRL_CLKEN_1_SET_ETHTX_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_ETH_OFFSET 28
#define KMGO_SYSCTRL_CLKEN_1_SET_ETH_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_USB_OFFSET 29
#define KMGO_SYSCTRL_CLKEN_1_SET_USB_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_RF_SS_OFFSET 30
#define KMGO_SYSCTRL_CLKEN_1_SET_RF_SS_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_SET_USIM_OFFSET 31
#define KMGO_SYSCTRL_CLKEN_1_SET_USIM_SIZE 1

/** bit field defines for kmgo_sysctrl_s#clken_1_clr */
#define KMGO_SYSCTRL_CLKEN_1_CLR_CPU_MDM_OFFSET 2
#define KMGO_SYSCTRL_CLKEN_1_CLR_CPU_MDM_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_CPU_RF_OFFSET 3
#define KMGO_SYSCTRL_CLKEN_1_CLR_CPU_RF_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_CPU_APP_OFFSET 4
#define KMGO_SYSCTRL_CLKEN_1_CLR_CPU_APP_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_CPU_PHY_OFFSET 5
#define KMGO_SYSCTRL_CLKEN_1_CLR_CPU_PHY_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_VSP_OFFSET 6
#define KMGO_SYSCTRL_CLKEN_1_CLR_VSP_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_TPIU_OFFSET 7
#define KMGO_SYSCTRL_CLKEN_1_CLR_TPIU_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_DBG_OFFSET 8
#define KMGO_SYSCTRL_CLKEN_1_CLR_DBG_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_RFTSF_OFFSET 9
#define KMGO_SYSCTRL_CLKEN_1_CLR_RFTSF_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_UART_1_OFFSET 10
#define KMGO_SYSCTRL_CLKEN_1_CLR_UART_1_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_UART_2_OFFSET 11
#define KMGO_SYSCTRL_CLKEN_1_CLR_UART_2_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_UART_3_OFFSET 12
#define KMGO_SYSCTRL_CLKEN_1_CLR_UART_3_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_UART_4_OFFSET 13
#define KMGO_SYSCTRL_CLKEN_1_CLR_UART_4_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_I2C_1_OFFSET 14
#define KMGO_SYSCTRL_CLKEN_1_CLR_I2C_1_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_I2C_2_OFFSET 15
#define KMGO_SYSCTRL_CLKEN_1_CLR_I2C_2_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_I2C_3_OFFSET 16
#define KMGO_SYSCTRL_CLKEN_1_CLR_I2C_3_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_SPI_1_OFFSET 17
#define KMGO_SYSCTRL_CLKEN_1_CLR_SPI_1_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_SPI_2_OFFSET 18
#define KMGO_SYSCTRL_CLKEN_1_CLR_SPI_2_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_SPI_3_OFFSET 19
#define KMGO_SYSCTRL_CLKEN_1_CLR_SPI_3_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_OSPI_OFFSET 20
#define KMGO_SYSCTRL_CLKEN_1_CLR_OSPI_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_PSRAM_OFFSET 21
#define KMGO_SYSCTRL_CLKEN_1_CLR_PSRAM_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_AUDIO_OFFSET 22
#define KMGO_SYSCTRL_CLKEN_1_CLR_AUDIO_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_SDIO_OFFSET 23
#define KMGO_SYSCTRL_CLKEN_1_CLR_SDIO_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_TMR_OFFSET 24
#define KMGO_SYSCTRL_CLKEN_1_CLR_TMR_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_TMR_CPU_OFFSET 25
#define KMGO_SYSCTRL_CLKEN_1_CLR_TMR_CPU_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_ETHTX_OFFSET 27
#define KMGO_SYSCTRL_CLKEN_1_CLR_ETHTX_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_ETH_OFFSET 28
#define KMGO_SYSCTRL_CLKEN_1_CLR_ETH_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_USB_OFFSET 29
#define KMGO_SYSCTRL_CLKEN_1_CLR_USB_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_RF_SS_OFFSET 30
#define KMGO_SYSCTRL_CLKEN_1_CLR_RF_SS_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_CLR_USIM_OFFSET 31
#define KMGO_SYSCTRL_CLKEN_1_CLR_USIM_SIZE 1

/** bit field defines for kmgo_sysctrl_s#clken_1_val */
#define KMGO_SYSCTRL_CLKEN_1_VAL_CPU_MDM_OFFSET 2
#define KMGO_SYSCTRL_CLKEN_1_VAL_CPU_MDM_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_CPU_RF_OFFSET 3
#define KMGO_SYSCTRL_CLKEN_1_VAL_CPU_RF_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_CPU_APP_OFFSET 4
#define KMGO_SYSCTRL_CLKEN_1_VAL_CPU_APP_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_CPU_PHY_OFFSET 5
#define KMGO_SYSCTRL_CLKEN_1_VAL_CPU_PHY_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_VSP_OFFSET 6
#define KMGO_SYSCTRL_CLKEN_1_VAL_VSP_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_TPIU_OFFSET 7
#define KMGO_SYSCTRL_CLKEN_1_VAL_TPIU_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_DBG_OFFSET 8
#define KMGO_SYSCTRL_CLKEN_1_VAL_DBG_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_RFTSF_OFFSET 9
#define KMGO_SYSCTRL_CLKEN_1_VAL_RFTSF_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_UART_1_OFFSET 10
#define KMGO_SYSCTRL_CLKEN_1_VAL_UART_1_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_UART_2_OFFSET 11
#define KMGO_SYSCTRL_CLKEN_1_VAL_UART_2_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_UART_3_OFFSET 12
#define KMGO_SYSCTRL_CLKEN_1_VAL_UART_3_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_UART_4_OFFSET 13
#define KMGO_SYSCTRL_CLKEN_1_VAL_UART_4_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_I2C_1_OFFSET 14
#define KMGO_SYSCTRL_CLKEN_1_VAL_I2C_1_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_I2C_2_OFFSET 15
#define KMGO_SYSCTRL_CLKEN_1_VAL_I2C_2_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_I2C_3_OFFSET 16
#define KMGO_SYSCTRL_CLKEN_1_VAL_I2C_3_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_SPI_1_OFFSET 17
#define KMGO_SYSCTRL_CLKEN_1_VAL_SPI_1_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_SPI_2_OFFSET 18
#define KMGO_SYSCTRL_CLKEN_1_VAL_SPI_2_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_SPI_3_OFFSET 19
#define KMGO_SYSCTRL_CLKEN_1_VAL_SPI_3_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_OSPI_OFFSET 20
#define KMGO_SYSCTRL_CLKEN_1_VAL_OSPI_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_PSRAM_OFFSET 21
#define KMGO_SYSCTRL_CLKEN_1_VAL_PSRAM_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_AUDIO_OFFSET 22
#define KMGO_SYSCTRL_CLKEN_1_VAL_AUDIO_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_SDIO_OFFSET 23
#define KMGO_SYSCTRL_CLKEN_1_VAL_SDIO_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_TMR_OFFSET 24
#define KMGO_SYSCTRL_CLKEN_1_VAL_TMR_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_TMR_CPU_OFFSET 25
#define KMGO_SYSCTRL_CLKEN_1_VAL_TMR_CPU_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_ETHTX_OFFSET 27
#define KMGO_SYSCTRL_CLKEN_1_VAL_ETHTX_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_ETH_OFFSET 28
#define KMGO_SYSCTRL_CLKEN_1_VAL_ETH_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_USB_OFFSET 29
#define KMGO_SYSCTRL_CLKEN_1_VAL_USB_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_RF_SS_OFFSET 30
#define KMGO_SYSCTRL_CLKEN_1_VAL_RF_SS_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_VAL_USIM_OFFSET 31
#define KMGO_SYSCTRL_CLKEN_1_VAL_USIM_SIZE 1

/** bit field defines for kmgo_sysctrl_s#clken_1_rdbk */
#define KMGO_SYSCTRL_CLKEN_1_RDBK_CPU_MDM_OFFSET 2
#define KMGO_SYSCTRL_CLKEN_1_RDBK_CPU_MDM_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_CPU_RF_OFFSET 3
#define KMGO_SYSCTRL_CLKEN_1_RDBK_CPU_RF_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_CPU_APP_OFFSET 4
#define KMGO_SYSCTRL_CLKEN_1_RDBK_CPU_APP_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_CPU_PHY_OFFSET 5
#define KMGO_SYSCTRL_CLKEN_1_RDBK_CPU_PHY_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_VSP_OFFSET 6
#define KMGO_SYSCTRL_CLKEN_1_RDBK_VSP_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_TPIU_OFFSET 7
#define KMGO_SYSCTRL_CLKEN_1_RDBK_TPIU_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_DBG_OFFSET 8
#define KMGO_SYSCTRL_CLKEN_1_RDBK_DBG_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_RFTSF_OFFSET 9
#define KMGO_SYSCTRL_CLKEN_1_RDBK_RFTSF_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_UART_1_OFFSET 10
#define KMGO_SYSCTRL_CLKEN_1_RDBK_UART_1_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_UART_2_OFFSET 11
#define KMGO_SYSCTRL_CLKEN_1_RDBK_UART_2_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_UART_3_OFFSET 12
#define KMGO_SYSCTRL_CLKEN_1_RDBK_UART_3_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_UART_4_OFFSET 13
#define KMGO_SYSCTRL_CLKEN_1_RDBK_UART_4_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_I2C_1_OFFSET 14
#define KMGO_SYSCTRL_CLKEN_1_RDBK_I2C_1_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_I2C_2_OFFSET 15
#define KMGO_SYSCTRL_CLKEN_1_RDBK_I2C_2_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_I2C_3_OFFSET 16
#define KMGO_SYSCTRL_CLKEN_1_RDBK_I2C_3_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_SPI_1_OFFSET 17
#define KMGO_SYSCTRL_CLKEN_1_RDBK_SPI_1_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_SPI_2_OFFSET 18
#define KMGO_SYSCTRL_CLKEN_1_RDBK_SPI_2_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_SPI_3_OFFSET 19
#define KMGO_SYSCTRL_CLKEN_1_RDBK_SPI_3_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_OSPI_OFFSET 20
#define KMGO_SYSCTRL_CLKEN_1_RDBK_OSPI_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_PSRAM_OFFSET 21
#define KMGO_SYSCTRL_CLKEN_1_RDBK_PSRAM_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_AUDIO_OFFSET 22
#define KMGO_SYSCTRL_CLKEN_1_RDBK_AUDIO_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_SDIO_OFFSET 23
#define KMGO_SYSCTRL_CLKEN_1_RDBK_SDIO_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_TMR_OFFSET 24
#define KMGO_SYSCTRL_CLKEN_1_RDBK_TMR_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_TMR_CPU_OFFSET 25
#define KMGO_SYSCTRL_CLKEN_1_RDBK_TMR_CPU_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_ETHTX_OFFSET 27
#define KMGO_SYSCTRL_CLKEN_1_RDBK_ETHTX_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_ETH_OFFSET 28
#define KMGO_SYSCTRL_CLKEN_1_RDBK_ETH_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_USB_OFFSET 29
#define KMGO_SYSCTRL_CLKEN_1_RDBK_USB_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_RF_SS_OFFSET 30
#define KMGO_SYSCTRL_CLKEN_1_RDBK_RF_SS_SIZE 1
#define KMGO_SYSCTRL_CLKEN_1_RDBK_USIM_OFFSET 31
#define KMGO_SYSCTRL_CLKEN_1_RDBK_USIM_SIZE 1

/** bit field defines for kmgo_sysctrl_s#slp_clken_1 */
#define KMGO_SYSCTRL_SLP_CLKEN_1_MAIN_OFFSET 0
#define KMGO_SYSCTRL_SLP_CLKEN_1_MAIN_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_PERIPH_OFFSET 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_PERIPH_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_CPU_MDM_OFFSET 2
#define KMGO_SYSCTRL_SLP_CLKEN_1_CPU_MDM_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_CPU_RF_OFFSET 3
#define KMGO_SYSCTRL_SLP_CLKEN_1_CPU_RF_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_CPU_APP_OFFSET 4
#define KMGO_SYSCTRL_SLP_CLKEN_1_CPU_APP_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_CPU_PHY_OFFSET 5
#define KMGO_SYSCTRL_SLP_CLKEN_1_CPU_PHY_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_VSP_OFFSET 6
#define KMGO_SYSCTRL_SLP_CLKEN_1_VSP_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_TPIU_OFFSET 7
#define KMGO_SYSCTRL_SLP_CLKEN_1_TPIU_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_DBG_OFFSET 8
#define KMGO_SYSCTRL_SLP_CLKEN_1_DBG_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_RFTSF_OFFSET 9
#define KMGO_SYSCTRL_SLP_CLKEN_1_RFTSF_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_UART_1_OFFSET 10
#define KMGO_SYSCTRL_SLP_CLKEN_1_UART_1_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_UART_2_OFFSET 11
#define KMGO_SYSCTRL_SLP_CLKEN_1_UART_2_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_UART_3_OFFSET 12
#define KMGO_SYSCTRL_SLP_CLKEN_1_UART_3_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_UART_4_OFFSET 13
#define KMGO_SYSCTRL_SLP_CLKEN_1_UART_4_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_I2C_1_OFFSET 14
#define KMGO_SYSCTRL_SLP_CLKEN_1_I2C_1_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_I2C_2_OFFSET 15
#define KMGO_SYSCTRL_SLP_CLKEN_1_I2C_2_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_I2C_3_OFFSET 16
#define KMGO_SYSCTRL_SLP_CLKEN_1_I2C_3_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_SPI_1_OFFSET 17
#define KMGO_SYSCTRL_SLP_CLKEN_1_SPI_1_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_SPI_2_OFFSET 18
#define KMGO_SYSCTRL_SLP_CLKEN_1_SPI_2_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_SPI_3_OFFSET 19
#define KMGO_SYSCTRL_SLP_CLKEN_1_SPI_3_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_OSPI_OFFSET 20
#define KMGO_SYSCTRL_SLP_CLKEN_1_OSPI_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_PSRAM_OFFSET 21
#define KMGO_SYSCTRL_SLP_CLKEN_1_PSRAM_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_AUDIO_OFFSET 22
#define KMGO_SYSCTRL_SLP_CLKEN_1_AUDIO_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_SDIO_OFFSET 23
#define KMGO_SYSCTRL_SLP_CLKEN_1_SDIO_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_TMR_OFFSET 24
#define KMGO_SYSCTRL_SLP_CLKEN_1_TMR_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_TMR_CPU_OFFSET 25
#define KMGO_SYSCTRL_SLP_CLKEN_1_TMR_CPU_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_ETHTX_OFFSET 27
#define KMGO_SYSCTRL_SLP_CLKEN_1_ETHTX_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_ETH_OFFSET 28
#define KMGO_SYSCTRL_SLP_CLKEN_1_ETH_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_USB_OFFSET 29
#define KMGO_SYSCTRL_SLP_CLKEN_1_USB_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_RF_SS_OFFSET 30
#define KMGO_SYSCTRL_SLP_CLKEN_1_RF_SS_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_1_USIM_OFFSET 31
#define KMGO_SYSCTRL_SLP_CLKEN_1_USIM_SIZE 1

/** bit field defines for kmgo_sysctrl_s#clken_2_set */
#define KMGO_SYSCTRL_CLKEN_2_SET_PIO_OFFSET 0
#define KMGO_SYSCTRL_CLKEN_2_SET_PIO_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_SET_OTP_OFFSET 1
#define KMGO_SYSCTRL_CLKEN_2_SET_OTP_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_SET_TSCU_SCU_OFFSET 2
#define KMGO_SYSCTRL_CLKEN_2_SET_TSCU_SCU_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_SET_TSCU_OFFSET 3
#define KMGO_SYSCTRL_CLKEN_2_SET_TSCU_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_SET_SF_AO_OFFSET 4
#define KMGO_SYSCTRL_CLKEN_2_SET_SF_AO_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_SET_SF_OFFSET 5
#define KMGO_SYSCTRL_CLKEN_2_SET_SF_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_SET_SECURITY_OFFSET 6
#define KMGO_SYSCTRL_CLKEN_2_SET_SECURITY_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_SET_WDG_MDM_OFFSET 7
#define KMGO_SYSCTRL_CLKEN_2_SET_WDG_MDM_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_SET_WDG_APP_OFFSET 8
#define KMGO_SYSCTRL_CLKEN_2_SET_WDG_APP_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_SET_WDG_PHY_OFFSET 9
#define KMGO_SYSCTRL_CLKEN_2_SET_WDG_PHY_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_SET_DISPLAY_OFFSET 10
#define KMGO_SYSCTRL_CLKEN_2_SET_DISPLAY_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_SET_PLL1_TMFLT_RNG_OFFSET 12
#define KMGO_SYSCTRL_CLKEN_2_SET_PLL1_TMFLT_RNG_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_SET_PLL2_TMFLT_RNG_OFFSET 13
#define KMGO_SYSCTRL_CLKEN_2_SET_PLL2_TMFLT_RNG_SIZE 1

/** bit field defines for kmgo_sysctrl_s#clken_2_clr */
#define KMGO_SYSCTRL_CLKEN_2_CLR_PIO_OFFSET 0
#define KMGO_SYSCTRL_CLKEN_2_CLR_PIO_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_CLR_OTP_OFFSET 1
#define KMGO_SYSCTRL_CLKEN_2_CLR_OTP_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_CLR_TSCU_SCU_OFFSET 2
#define KMGO_SYSCTRL_CLKEN_2_CLR_TSCU_SCU_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_CLR_TSCU_OFFSET 3
#define KMGO_SYSCTRL_CLKEN_2_CLR_TSCU_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_CLR_SF_AO_OFFSET 4
#define KMGO_SYSCTRL_CLKEN_2_CLR_SF_AO_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_CLR_SF_OFFSET 5
#define KMGO_SYSCTRL_CLKEN_2_CLR_SF_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_CLR_SECURITY_OFFSET 6
#define KMGO_SYSCTRL_CLKEN_2_CLR_SECURITY_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_CLR_WDG_MDM_OFFSET 7
#define KMGO_SYSCTRL_CLKEN_2_CLR_WDG_MDM_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_CLR_WDG_APP_OFFSET 8
#define KMGO_SYSCTRL_CLKEN_2_CLR_WDG_APP_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_CLR_WDG_PHY_OFFSET 9
#define KMGO_SYSCTRL_CLKEN_2_CLR_WDG_PHY_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_CLR_DISPLAY_OFFSET 10
#define KMGO_SYSCTRL_CLKEN_2_CLR_DISPLAY_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_CLR_PLL1_TMFLT_RNG_OFFSET 12
#define KMGO_SYSCTRL_CLKEN_2_CLR_PLL1_TMFLT_RNG_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_CLR_PLL2_TMFLT_RNG_OFFSET 13
#define KMGO_SYSCTRL_CLKEN_2_CLR_PLL2_TMFLT_RNG_SIZE 1

/** bit field defines for kmgo_sysctrl_s#clken_2_val */
#define KMGO_SYSCTRL_CLKEN_2_VAL_PIO_OFFSET 0
#define KMGO_SYSCTRL_CLKEN_2_VAL_PIO_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_VAL_OTP_OFFSET 1
#define KMGO_SYSCTRL_CLKEN_2_VAL_OTP_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_VAL_TSCU_SCU_OFFSET 2
#define KMGO_SYSCTRL_CLKEN_2_VAL_TSCU_SCU_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_VAL_TSCU_OFFSET 3
#define KMGO_SYSCTRL_CLKEN_2_VAL_TSCU_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_VAL_SF_AO_OFFSET 4
#define KMGO_SYSCTRL_CLKEN_2_VAL_SF_AO_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_VAL_SF_OFFSET 5
#define KMGO_SYSCTRL_CLKEN_2_VAL_SF_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_VAL_SECURITY_OFFSET 6
#define KMGO_SYSCTRL_CLKEN_2_VAL_SECURITY_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_VAL_WDG_MDM_OFFSET 7
#define KMGO_SYSCTRL_CLKEN_2_VAL_WDG_MDM_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_VAL_WDG_APP_OFFSET 8
#define KMGO_SYSCTRL_CLKEN_2_VAL_WDG_APP_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_VAL_WDG_PHY_OFFSET 9
#define KMGO_SYSCTRL_CLKEN_2_VAL_WDG_PHY_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_VAL_DISPLAY_OFFSET 10
#define KMGO_SYSCTRL_CLKEN_2_VAL_DISPLAY_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_VAL_PLL1_TMFLT_RNG_OFFSET 12
#define KMGO_SYSCTRL_CLKEN_2_VAL_PLL1_TMFLT_RNG_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_VAL_PLL2_TMFLT_RNG_OFFSET 13
#define KMGO_SYSCTRL_CLKEN_2_VAL_PLL2_TMFLT_RNG_SIZE 1

/** bit field defines for kmgo_sysctrl_s#clken_2_rdbk */
#define KMGO_SYSCTRL_CLKEN_2_RDBK_PIO_OFFSET 0
#define KMGO_SYSCTRL_CLKEN_2_RDBK_PIO_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_RDBK_OTP_OFFSET 1
#define KMGO_SYSCTRL_CLKEN_2_RDBK_OTP_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_RDBK_TSCU_SCU_OFFSET 2
#define KMGO_SYSCTRL_CLKEN_2_RDBK_TSCU_SCU_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_RDBK_TSCU_OFFSET 3
#define KMGO_SYSCTRL_CLKEN_2_RDBK_TSCU_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_RDBK_SF_AO_OFFSET 4
#define KMGO_SYSCTRL_CLKEN_2_RDBK_SF_AO_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_RDBK_SF_OFFSET 5
#define KMGO_SYSCTRL_CLKEN_2_RDBK_SF_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_RDBK_SECURITY_OFFSET 6
#define KMGO_SYSCTRL_CLKEN_2_RDBK_SECURITY_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_RDBK_WDG_MDM_OFFSET 7
#define KMGO_SYSCTRL_CLKEN_2_RDBK_WDG_MDM_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_RDBK_WDG_APP_OFFSET 8
#define KMGO_SYSCTRL_CLKEN_2_RDBK_WDG_APP_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_RDBK_WDG_PHY_OFFSET 9
#define KMGO_SYSCTRL_CLKEN_2_RDBK_WDG_PHY_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_RDBK_DISPLAY_OFFSET 10
#define KMGO_SYSCTRL_CLKEN_2_RDBK_DISPLAY_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_RDBK_PLL1_TMFLT_RNG_OFFSET 12
#define KMGO_SYSCTRL_CLKEN_2_RDBK_PLL1_TMFLT_RNG_SIZE 1
#define KMGO_SYSCTRL_CLKEN_2_RDBK_PLL2_TMFLT_RNG_OFFSET 13
#define KMGO_SYSCTRL_CLKEN_2_RDBK_PLL2_TMFLT_RNG_SIZE 1

/** bit field defines for kmgo_sysctrl_s#slp_clken_2 */
#define KMGO_SYSCTRL_SLP_CLKEN_2_PIO_OFFSET 0
#define KMGO_SYSCTRL_SLP_CLKEN_2_PIO_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_2_OTP_OFFSET 1
#define KMGO_SYSCTRL_SLP_CLKEN_2_OTP_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_2_TSCU_SCU_OFFSET 2
#define KMGO_SYSCTRL_SLP_CLKEN_2_TSCU_SCU_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_2_TSCU_OFFSET 3
#define KMGO_SYSCTRL_SLP_CLKEN_2_TSCU_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_2_SF_AO_OFFSET 4
#define KMGO_SYSCTRL_SLP_CLKEN_2_SF_AO_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_2_SF_OFFSET 5
#define KMGO_SYSCTRL_SLP_CLKEN_2_SF_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_2_SECURITY_OFFSET 6
#define KMGO_SYSCTRL_SLP_CLKEN_2_SECURITY_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_2_WDG_MDM_OFFSET 7
#define KMGO_SYSCTRL_SLP_CLKEN_2_WDG_MDM_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_2_WDG_APP_OFFSET 8
#define KMGO_SYSCTRL_SLP_CLKEN_2_WDG_APP_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_2_WDG_PHY_OFFSET 9
#define KMGO_SYSCTRL_SLP_CLKEN_2_WDG_PHY_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_2_DISPLAY_OFFSET 10
#define KMGO_SYSCTRL_SLP_CLKEN_2_DISPLAY_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_2_PLL1_TMFLT_RNG_OFFSET 12
#define KMGO_SYSCTRL_SLP_CLKEN_2_PLL1_TMFLT_RNG_SIZE 1
#define KMGO_SYSCTRL_SLP_CLKEN_2_PLL2_TMFLT_RNG_OFFSET 13
#define KMGO_SYSCTRL_SLP_CLKEN_2_PLL2_TMFLT_RNG_SIZE 1

/** bit field defines for kmgo_sysctrl_s#clkctrl_mode */
#define KMGO_SYSCTRL_CLKCTRL_MODE_EVAL_OFFSET 0
#define KMGO_SYSCTRL_CLKCTRL_MODE_EVAL_SIZE 1

/** bit field defines for kmgo_sysctrl_s#clkctrl_status */
#define KMGO_SYSCTRL_CLKCTRL_STATUS_PWR_CLK_SLEEP_MODE_OFFSET 0
#define KMGO_SYSCTRL_CLKCTRL_STATUS_PWR_CLK_SLEEP_MODE_SIZE 1

/** bit field defines for kmgo_sysctrl_s#clk_enable_local_ctrl */
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_MDM_PHY_CPU_OFFSET 0
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_MDM_PHY_CPU_SIZE 1
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_MDM_GPV_OFFSET 1
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_MDM_GPV_SIZE 1
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_MDM_CPU_OFFSET 2
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_MDM_CPU_SIZE 1
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_MDM_CTRL_OFFSET 3
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_MDM_CTRL_SIZE 1
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_MDM_RF_DATAPATH_OFFSET 4
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_MDM_RF_DATAPATH_SIZE 1
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_MDM_MAIN_OFFSET 5
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_MDM_MAIN_SIZE 1
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_PSRAM_OFFSET 6
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_PSRAM_SIZE 1
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_APP_CPU_OFFSET 7
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_APP_CPU_SIZE 1
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_APP_MAIN_OFFSET 8
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_APP_MAIN_SIZE 1
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_APP_MAIN_64_OFFSET 9
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_APP_MAIN_64_SIZE 1
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_APP_DBG_OFFSET 10
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_APP_DBG_SIZE 1
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_APP_MAIN_PERIPH_OFFSET 11
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_APP_MAIN_PERIPH_SIZE 1
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_APP_GPV_OFFSET 12
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_NIC_APP_GPV_SIZE 1
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_VSPSS_OFFSET 13
#define KMGO_SYSCTRL_CLK_ENABLE_LOCAL_CTRL_VSPSS_SIZE 1

/* EOF kmgo_sysctrl.h */
#endif
