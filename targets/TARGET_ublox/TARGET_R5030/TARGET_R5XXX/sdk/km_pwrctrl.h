#ifndef INCLUDED_KM_PWRCTRL
#define INCLUDED_KM_PWRCTRL
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** km_pwrctrl HAL Spreadsheet version number */
#define KM_PWRCTRL_HAL_VERSION 1

/** Power Controller Module
*/
struct km_pwrctrl_s {
   /** A delay before the state machine states to turn-on or turn-off switches at address offset 0x000, read-write */
   uint32_t pwr_ticks_pd_start;
   /** The time to wait after all of the pre switch chains has been turned on, or turned off. Note, this time may be larger than the Tall. at address offset 0x004, read-write */
   uint32_t pwr_ticks_pd_small;
   /** The time to wait after an all switch chain has been turned on (or off) until the next switch chain is toggled. Since there may be more than one all switch chain, this can be the time between each big chain is enabled/disabled, or for the last all chain, the time between the chain is switched and the next phase of the sequence. at address offset 0x008, read-write */
   uint32_t pwr_ticks_pd_big;
   /** The time to wait after the isolation has been enabled or disabled. This time should take into account the amount of time it takes the isolation signal to propagate to all the isolation cells. at address offset 0x00C, read-write */
   uint32_t pwr_ticks_pd_isolate;
   /** The time to wait after the reset has been asserted/de-asserted before carrying out the next phase of the sequence at address offset 0x010, read-write */
   uint32_t pwr_ticks_pd_reset;
   /** Reserved space */
   uint8_t fill0[16];
   /** This register allows the status of some of the handshakes to be read by software at address offset 0x024, read-only */
   uint32_t pwr_handshake_status;
   /** This register allows the handshakes between the Fabric and CPUs when entering a power saving mode to be configured. at address offset 0x028, read-write */
   uint32_t pwr_handshake_cfg;
   /** Reserved space */
   uint8_t fill1[4];
   /** This register reports the current power mode of the basebase , in addition to indicating which power domains are current turned on, and the current state of the general purpose controls. at address offset 0x030, read-only */
   uint32_t pwr_curr_mode;
   /** The next power mode of the chip. This register configures the next power mode of the baseband. Note: in order for the transition to take place, a write has to be performed to the next register: PWR_UPDATE_MODE.Writing to this register does not change the power mode. In order to change the power mode, a write to the PWR_UPDATE_MODE register is required. at address offset 0x034, read-write */
   uint32_t pwr_next_mode;
   /**  at address offset 0x038, read-write */
   uint32_t pwr_update_mode;
   /** Reserved space */
   uint8_t fill2[4];
   /** Configuration for general purpose control 0. at address offset 0x040, read-write */
   uint32_t pwr_gpc_0_cfg;
   /** Configuration for general purpose control 0. at address offset 0x044, read-write */
   uint32_t pwr_gpc_1_cfg;
   /** Configuration for general purpose control 0. at address offset 0x048, read-write */
   uint32_t pwr_gpc_2_cfg;
   /** Configuration for general purpose control 0. at address offset 0x04C, read-write */
   uint32_t pwr_gpc_3_cfg;
   /** Configuration for general purpose control 0. at address offset 0x050, read-write */
   uint32_t pwr_gpc_4_cfg;
   /** Configuration for general purpose control 0. at address offset 0x054, read-write */
   uint32_t pwr_gpc_5_cfg;
   /** Configuration for general purpose control 0. at address offset 0x058, read-write */
   uint32_t pwr_gpc_6_cfg;
   /** Configuration for general purpose control 0. at address offset 0x05C, read-write */
   uint32_t pwr_gpc_7_cfg;
   /** Reserved space */
   uint8_t fill3[32];
   /** Configuration for wakeup-source 0. at address offset 0x080, read-write */
   uint32_t pwr_wakeupsrc_0_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x084, read-write */
   uint32_t pwr_wakeupsrc_1_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x088, read-write */
   uint32_t pwr_wakeupsrc_2_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x08C, read-write */
   uint32_t pwr_wakeupsrc_3_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x090, read-write */
   uint32_t pwr_wakeupsrc_4_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x094, read-write */
   uint32_t pwr_wakeupsrc_5_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x098, read-write */
   uint32_t pwr_wakeupsrc_6_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x09C, read-write */
   uint32_t pwr_wakeupsrc_7_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0A0, read-write */
   uint32_t pwr_wakeupsrc_8_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0A4, read-write */
   uint32_t pwr_wakeupsrc_9_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0A8, read-write */
   uint32_t pwr_wakeupsrc_10_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0AC, read-write */
   uint32_t pwr_wakeupsrc_11_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0B0, read-write */
   uint32_t pwr_wakeupsrc_12_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0B4, read-write */
   uint32_t pwr_wakeupsrc_13_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0B8, read-write */
   uint32_t pwr_wakeupsrc_14_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0BC, read-write */
   uint32_t pwr_wakeupsrc_15_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0C0, read-write */
   uint32_t pwr_wakeupsrc_16_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0C4, read-write */
   uint32_t pwr_wakeupsrc_17_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0C8, read-write */
   uint32_t pwr_wakeupsrc_18_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0CC, read-write */
   uint32_t pwr_wakeupsrc_19_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0D0, read-write */
   uint32_t pwr_wakeupsrc_20_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0D4, read-write */
   uint32_t pwr_wakeupsrc_21_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0D8, read-write */
   uint32_t pwr_wakeupsrc_22_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0DC, read-write */
   uint32_t pwr_wakeupsrc_23_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0E0, read-write */
   uint32_t pwr_wakeupsrc_24_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0E4, read-write */
   uint32_t pwr_wakeupsrc_25_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0E8, read-write */
   uint32_t pwr_wakeupsrc_26_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0EC, read-write */
   uint32_t pwr_wakeupsrc_27_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0F0, read-write */
   uint32_t pwr_wakeupsrc_28_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0F4, read-write */
   uint32_t pwr_wakeupsrc_29_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0F8, read-write */
   uint32_t pwr_wakeupsrc_30_cfg;
   /** Configuration for wakeup-source 0. at address offset 0x0FC, read-write */
   uint32_t pwr_wakeupsrc_31_cfg;
   /** Configuration for power mode Power Mode 0. at address offset 0x100, read-write */
   uint32_t pwr_mode_p0_cfg;
   /** Configuration for power mode Power Mode 1. at address offset 0x104, read-write */
   uint32_t pwr_mode_p1_cfg;
   /** Configuration for power mode Power Mode 2. at address offset 0x108, read-write */
   uint32_t pwr_mode_p2_cfg;
   /** Configuration for power mode Power Mode 3. at address offset 0x10C, read-write */
   uint32_t pwr_mode_p3_cfg;
   /** Configuration for power mode Power Mode 4. at address offset 0x110, read-write */
   uint32_t pwr_mode_p4_cfg;
   /** Configuration for power mode Power Mode 5. at address offset 0x114, read-write */
   uint32_t pwr_mode_p5_cfg;
   /** Configuration for power mode Power Mode 6. at address offset 0x118, read-write */
   uint32_t pwr_mode_p6_cfg;
   /** Configuration for power mode Power Mode 7. at address offset 0x11C, read-write */
   uint32_t pwr_mode_p7_cfg;
   /** Reserved space */
   uint8_t fill4[224];
   /**  at address offset 0x200, read-write */
   uint32_t pwr_mem_00_cfg;
   /**  at address offset 0x204, read-write */
   uint32_t pwr_mem_01_cfg;
   /**  at address offset 0x208, read-write */
   uint32_t pwr_mem_02_cfg;
   /**  at address offset 0x20C, read-write */
   uint32_t pwr_mem_03_cfg;
   /**  at address offset 0x210, read-write */
   uint32_t pwr_mem_04_cfg;
   /**  at address offset 0x214, read-write */
   uint32_t pwr_mem_05_cfg;
   /**  at address offset 0x218, read-write */
   uint32_t pwr_mem_06_cfg;
   /**  at address offset 0x21C, read-write */
   uint32_t pwr_mem_07_cfg;
   /**  at address offset 0x220, read-write */
   uint32_t pwr_mem_08_cfg;
   /**  at address offset 0x224, read-write */
   uint32_t pwr_mem_09_cfg;
   /**  at address offset 0x228, read-write */
   uint32_t pwr_mem_10_cfg;
   /**  at address offset 0x22C, read-write */
   uint32_t pwr_mem_11_cfg;
   /**  at address offset 0x230, read-write */
   uint32_t pwr_mem_12_cfg;
   /**  at address offset 0x234, read-write */
   uint32_t pwr_mem_13_cfg;
   /**  at address offset 0x238, read-write */
   uint32_t pwr_mem_14_cfg;
   /**  at address offset 0x23C, read-write */
   uint32_t pwr_mem_15_cfg;
   /**  at address offset 0x240, read-write */
   uint32_t pwr_mem_16_cfg;
   /**  at address offset 0x244, read-write */
   uint32_t pwr_mem_17_cfg;
   /**  at address offset 0x248, read-write */
   uint32_t pwr_mem_18_cfg;
   /**  at address offset 0x24C, read-write */
   uint32_t pwr_mem_19_cfg;
   /**  at address offset 0x250, read-write */
   uint32_t pwr_mem_20_cfg;
   /**  at address offset 0x254, read-write */
   uint32_t pwr_mem_21_cfg;
   /**  at address offset 0x258, read-write */
   uint32_t pwr_mem_22_cfg;
   /**  at address offset 0x25C, read-write */
   uint32_t pwr_mem_23_cfg;
   /**  at address offset 0x260, read-write */
   uint32_t pwr_mem_24_cfg;
   /**  at address offset 0x264, read-write */
   uint32_t pwr_mem_25_cfg;
   /**  at address offset 0x268, read-write */
   uint32_t pwr_mem_26_cfg;
   /**  at address offset 0x26C, read-write */
   uint32_t pwr_mem_27_cfg;
   /**  at address offset 0x270, read-write */
   uint32_t pwr_mem_28_cfg;
   /**  at address offset 0x274, read-write */
   uint32_t pwr_mem_29_cfg;
   /** Reserved space */
   uint8_t fill5[136];
   /** Interrupt enable register. One bit per interrupt. This register allows the interrupt sources of the power controller to be enabled at address offset 0x300, read-write */
   uint32_t pwr_interrupt_enable;
   /** Status register which indicates the cause for the last interrupt event. This register reports the current status of the power controller interrupts. One interrupt source per bit. Needs to be cleared with the PWR_INTERRUPT_CLEAR register at address offset 0x304, read-only */
   uint32_t pwr_interrupt_status;
   /** Interrupt clear register: This register allows the interrupts of the power controller to be cleared.used to clear the PWR_INTERRUPT_STATUS register. Writing a 1  to the corresponding bit will clear the interrupt.  at address offset 0x308, write-only */
   uint32_t pwr_interrupt_clear;
   /** Reserved space */
   uint8_t fill6[4];
   /** read-write Fault interrupt enable register . This register allows the fault interrupt sources of the system controller to be enabled Faults covered - - Software programs PD cfg registers with any of child domains ON and corresponding parent domain OFF - Software attempts to enter non-operating mode without enabling any interrupt (check is performed when trigger bit is set to 1 in the pwr_update_mode register). - Software programs wakeup cfg registers to wakeup in a non-operating mode (< P3) at address offset 0x310, read-write */
   uint32_t pwr_fault_interrupt_enable;
   /** read-only Fault Status register which indicates the cause for the fault interrupt event. Faults covered - - Software programs PD cfg registers with any of child domains ON and corresponding parent domain OFF - Software attempts to enter non-operating mode without enabling any interrupt - Software programs wakeup cfg registers to wakeup in a non-operating mode (< P4)  One interrupt source per bit. Needs to be cleared with the FAULT_INTERRUPT_CLEAR register at address offset 0x314, read-only */
   uint32_t pwr_fault_interrupt_status;
   /** Fault Interrupt clear register: This register allows the fault interrupts of the power controller to be cleared.used to clear the PWR_FAULT_INTERRUPT_STATUS register. Writing a 1  to the corresponding bit will clear the interrupt.  at address offset 0x318, write-only */
   uint32_t pwr_fault_interrupt_clear;
   /** Reserved space */
   uint8_t fill7[4];
   /** Misc Controls at address offset 0x320, read-write */
   uint32_t pwr_misc_ctrl;
   /** Reserved space */
   uint8_t fill8[2524];
   /** Specifies Reset vector for APP BOOT CPU  at address offset 0xD00, read-write */
   uint32_t cpu_boot_vinit;
};

/** bit field defines for km_pwrctrl_s#pwr_ticks_pd_start */
#define KM_PWRCTRL_PWR_TICKS_PD_START_COUNT_OFFSET 0
#define KM_PWRCTRL_PWR_TICKS_PD_START_COUNT_SIZE 16

/** bit field defines for km_pwrctrl_s#pwr_ticks_pd_small */
#define KM_PWRCTRL_PWR_TICKS_PD_SMALL_COUNT_OFFSET 0
#define KM_PWRCTRL_PWR_TICKS_PD_SMALL_COUNT_SIZE 16

/** bit field defines for km_pwrctrl_s#pwr_ticks_pd_big */
#define KM_PWRCTRL_PWR_TICKS_PD_BIG_COUNT_OFFSET 0
#define KM_PWRCTRL_PWR_TICKS_PD_BIG_COUNT_SIZE 16

/** bit field defines for km_pwrctrl_s#pwr_ticks_pd_isolate */
#define KM_PWRCTRL_PWR_TICKS_PD_ISOLATE_COUNT_OFFSET 0
#define KM_PWRCTRL_PWR_TICKS_PD_ISOLATE_COUNT_SIZE 16

/** bit field defines for km_pwrctrl_s#pwr_ticks_pd_reset */
#define KM_PWRCTRL_PWR_TICKS_PD_RESET_COUNT_OFFSET 0
#define KM_PWRCTRL_PWR_TICKS_PD_RESET_COUNT_SIZE 16

/** bit field defines for km_pwrctrl_s#pwr_handshake_status */
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_CPU_MDM_SLEEPING_OFFSET 0
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_CPU_MDM_SLEEPING_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_CPU_RF_SLEEPING_OFFSET 1
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_CPU_RF_SLEEPING_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_CPU_APP_SLEEPING_OFFSET 2
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_CPU_APP_SLEEPING_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_CPU_PHY_SLEEPING_OFFSET 3
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_CPU_PHY_SLEEPING_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_FABRIC_1_CACTIVE_OFFSET 8
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_FABRIC_1_CACTIVE_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_FABRIC_2_CACTIVE_OFFSET 9
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_FABRIC_2_CACTIVE_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_FABRIC_3_CACTIVE_OFFSET 10
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_FABRIC_3_CACTIVE_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_FABRIC_4_CACTIVE_OFFSET 11
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_FABRIC_4_CACTIVE_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_FABRIC_1_CSYSACK_OFFSET 16
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_FABRIC_1_CSYSACK_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_FABRIC_2_CSYSACK_OFFSET 17
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_FABRIC_2_CSYSACK_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_FABRIC_3_CSYSACK_OFFSET 18
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_FABRIC_3_CSYSACK_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_FABRIC_4_CSYSACK_OFFSET 19
#define KM_PWRCTRL_PWR_HANDSHAKE_STATUS_FABRIC_4_CSYSACK_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_handshake_cfg */
#define KM_PWRCTRL_PWR_HANDSHAKE_CFG_WFI_MDM_DISABLE_OFFSET 0
#define KM_PWRCTRL_PWR_HANDSHAKE_CFG_WFI_MDM_DISABLE_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_CFG_WFI_RF_DISABLE_OFFSET 1
#define KM_PWRCTRL_PWR_HANDSHAKE_CFG_WFI_RF_DISABLE_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_CFG_WFI_APP_DISABLE_OFFSET 2
#define KM_PWRCTRL_PWR_HANDSHAKE_CFG_WFI_APP_DISABLE_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_CFG_WFI_PHY_DISABLE_OFFSET 3
#define KM_PWRCTRL_PWR_HANDSHAKE_CFG_WFI_PHY_DISABLE_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_CFG_FABRIC_1_DISABLE_OFFSET 8
#define KM_PWRCTRL_PWR_HANDSHAKE_CFG_FABRIC_1_DISABLE_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_CFG_FABRIC_2_DISABLE_OFFSET 9
#define KM_PWRCTRL_PWR_HANDSHAKE_CFG_FABRIC_2_DISABLE_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_CFG_FABRIC_3_DISABLE_OFFSET 10
#define KM_PWRCTRL_PWR_HANDSHAKE_CFG_FABRIC_3_DISABLE_SIZE 1
#define KM_PWRCTRL_PWR_HANDSHAKE_CFG_FABRIC_4_DISABLE_OFFSET 11
#define KM_PWRCTRL_PWR_HANDSHAKE_CFG_FABRIC_4_DISABLE_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_curr_mode */
#define KM_PWRCTRL_PWR_CURR_MODE_CURRENT_POWER_MODE_OFFSET 0
#define KM_PWRCTRL_PWR_CURR_MODE_CURRENT_POWER_MODE_SIZE 4
#define KM_PWRCTRL_PWR_CURR_MODE_LAST_POWER_MODE_OFFSET 4
#define KM_PWRCTRL_PWR_CURR_MODE_LAST_POWER_MODE_SIZE 4
#define KM_PWRCTRL_PWR_CURR_MODE_REF_CLK_PRESENT_OFFSET 9
#define KM_PWRCTRL_PWR_CURR_MODE_REF_CLK_PRESENT_SIZE 1
#define KM_PWRCTRL_PWR_CURR_MODE_DIFFERENT_AOCLK_OFFSET 10
#define KM_PWRCTRL_PWR_CURR_MODE_DIFFERENT_AOCLK_SIZE 1
#define KM_PWRCTRL_PWR_CURR_MODE_WAKEUP_MULTIPLE_OFFSET 11
#define KM_PWRCTRL_PWR_CURR_MODE_WAKEUP_MULTIPLE_SIZE 1
#define KM_PWRCTRL_PWR_CURR_MODE_CURRENT_AOCLK_SEL_OFFSET 12
#define KM_PWRCTRL_PWR_CURR_MODE_CURRENT_AOCLK_SEL_SIZE 2
#define KM_PWRCTRL_PWR_CURR_MODE_CURRENT_GPC_VAL_OFFSET 16
#define KM_PWRCTRL_PWR_CURR_MODE_CURRENT_GPC_VAL_SIZE 8
#define KM_PWRCTRL_PWR_CURR_MODE_CURRENT_PD_ON_OFFSET 24
#define KM_PWRCTRL_PWR_CURR_MODE_CURRENT_PD_ON_SIZE 7

/** bit field defines for km_pwrctrl_s#pwr_next_mode */
#define KM_PWRCTRL_PWR_NEXT_MODE_NEXT_POWER_MODE_OFFSET 0
#define KM_PWRCTRL_PWR_NEXT_MODE_NEXT_POWER_MODE_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_update_mode */
#define KM_PWRCTRL_PWR_UPDATE_MODE_TRIGGER_OFFSET 0
#define KM_PWRCTRL_PWR_UPDATE_MODE_TRIGGER_SIZE 1
#define KM_PWRCTRL_PWR_UPDATE_MODE_MEM_TRIGGER_OFFSET 4
#define KM_PWRCTRL_PWR_UPDATE_MODE_MEM_TRIGGER_SIZE 1
#define KM_PWRCTRL_PWR_UPDATE_MODE_AOCLK_TRIGGER_OFFSET 8
#define KM_PWRCTRL_PWR_UPDATE_MODE_AOCLK_TRIGGER_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_gpc_0_cfg */
#define KM_PWRCTRL_PWR_GPC_0_CFG_GPC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_GPC_0_CFG_GPC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_GPC_0_CFG_GPC_INVERT_OFFSET 1
#define KM_PWRCTRL_PWR_GPC_0_CFG_GPC_INVERT_SIZE 1
#define KM_PWRCTRL_PWR_GPC_0_CFG_GPC_PRIORITY_OFFSET 4
#define KM_PWRCTRL_PWR_GPC_0_CFG_GPC_PRIORITY_SIZE 4
#define KM_PWRCTRL_PWR_GPC_0_CFG_GPC_OFF_DIV_OFFSET 8
#define KM_PWRCTRL_PWR_GPC_0_CFG_GPC_OFF_DIV_SIZE 8
#define KM_PWRCTRL_PWR_GPC_0_CFG_GPC_ONOFF_COUNT_OFFSET 16
#define KM_PWRCTRL_PWR_GPC_0_CFG_GPC_ONOFF_COUNT_SIZE 16

/** bit field defines for km_pwrctrl_s#pwr_gpc_1_cfg */
#define KM_PWRCTRL_PWR_GPC_1_CFG_GPC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_GPC_1_CFG_GPC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_GPC_1_CFG_GPC_INVERT_OFFSET 1
#define KM_PWRCTRL_PWR_GPC_1_CFG_GPC_INVERT_SIZE 1
#define KM_PWRCTRL_PWR_GPC_1_CFG_GPC_PRIORITY_OFFSET 4
#define KM_PWRCTRL_PWR_GPC_1_CFG_GPC_PRIORITY_SIZE 4
#define KM_PWRCTRL_PWR_GPC_1_CFG_GPC_OFF_DIV_OFFSET 8
#define KM_PWRCTRL_PWR_GPC_1_CFG_GPC_OFF_DIV_SIZE 8
#define KM_PWRCTRL_PWR_GPC_1_CFG_GPC_ONOFF_COUNT_OFFSET 16
#define KM_PWRCTRL_PWR_GPC_1_CFG_GPC_ONOFF_COUNT_SIZE 16

/** bit field defines for km_pwrctrl_s#pwr_gpc_2_cfg */
#define KM_PWRCTRL_PWR_GPC_2_CFG_GPC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_GPC_2_CFG_GPC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_GPC_2_CFG_GPC_INVERT_OFFSET 1
#define KM_PWRCTRL_PWR_GPC_2_CFG_GPC_INVERT_SIZE 1
#define KM_PWRCTRL_PWR_GPC_2_CFG_GPC_PRIORITY_OFFSET 4
#define KM_PWRCTRL_PWR_GPC_2_CFG_GPC_PRIORITY_SIZE 4
#define KM_PWRCTRL_PWR_GPC_2_CFG_GPC_OFF_DIV_OFFSET 8
#define KM_PWRCTRL_PWR_GPC_2_CFG_GPC_OFF_DIV_SIZE 8
#define KM_PWRCTRL_PWR_GPC_2_CFG_GPC_ONOFF_COUNT_OFFSET 16
#define KM_PWRCTRL_PWR_GPC_2_CFG_GPC_ONOFF_COUNT_SIZE 16

/** bit field defines for km_pwrctrl_s#pwr_gpc_3_cfg */
#define KM_PWRCTRL_PWR_GPC_3_CFG_GPC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_GPC_3_CFG_GPC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_GPC_3_CFG_GPC_INVERT_OFFSET 1
#define KM_PWRCTRL_PWR_GPC_3_CFG_GPC_INVERT_SIZE 1
#define KM_PWRCTRL_PWR_GPC_3_CFG_GPC_PRIORITY_OFFSET 4
#define KM_PWRCTRL_PWR_GPC_3_CFG_GPC_PRIORITY_SIZE 4
#define KM_PWRCTRL_PWR_GPC_3_CFG_GPC_OFF_DIV_OFFSET 8
#define KM_PWRCTRL_PWR_GPC_3_CFG_GPC_OFF_DIV_SIZE 8
#define KM_PWRCTRL_PWR_GPC_3_CFG_GPC_ONOFF_COUNT_OFFSET 16
#define KM_PWRCTRL_PWR_GPC_3_CFG_GPC_ONOFF_COUNT_SIZE 16

/** bit field defines for km_pwrctrl_s#pwr_gpc_4_cfg */
#define KM_PWRCTRL_PWR_GPC_4_CFG_GPC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_GPC_4_CFG_GPC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_GPC_4_CFG_GPC_INVERT_OFFSET 1
#define KM_PWRCTRL_PWR_GPC_4_CFG_GPC_INVERT_SIZE 1
#define KM_PWRCTRL_PWR_GPC_4_CFG_GPC_PRIORITY_OFFSET 4
#define KM_PWRCTRL_PWR_GPC_4_CFG_GPC_PRIORITY_SIZE 4
#define KM_PWRCTRL_PWR_GPC_4_CFG_GPC_OFF_DIV_OFFSET 8
#define KM_PWRCTRL_PWR_GPC_4_CFG_GPC_OFF_DIV_SIZE 8
#define KM_PWRCTRL_PWR_GPC_4_CFG_GPC_ONOFF_COUNT_OFFSET 16
#define KM_PWRCTRL_PWR_GPC_4_CFG_GPC_ONOFF_COUNT_SIZE 16

/** bit field defines for km_pwrctrl_s#pwr_gpc_5_cfg */
#define KM_PWRCTRL_PWR_GPC_5_CFG_GPC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_GPC_5_CFG_GPC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_GPC_5_CFG_GPC_INVERT_OFFSET 1
#define KM_PWRCTRL_PWR_GPC_5_CFG_GPC_INVERT_SIZE 1
#define KM_PWRCTRL_PWR_GPC_5_CFG_GPC_PRIORITY_OFFSET 4
#define KM_PWRCTRL_PWR_GPC_5_CFG_GPC_PRIORITY_SIZE 4
#define KM_PWRCTRL_PWR_GPC_5_CFG_GPC_OFF_DIV_OFFSET 8
#define KM_PWRCTRL_PWR_GPC_5_CFG_GPC_OFF_DIV_SIZE 8
#define KM_PWRCTRL_PWR_GPC_5_CFG_GPC_ONOFF_COUNT_OFFSET 16
#define KM_PWRCTRL_PWR_GPC_5_CFG_GPC_ONOFF_COUNT_SIZE 16

/** bit field defines for km_pwrctrl_s#pwr_gpc_6_cfg */
#define KM_PWRCTRL_PWR_GPC_6_CFG_GPC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_GPC_6_CFG_GPC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_GPC_6_CFG_GPC_INVERT_OFFSET 1
#define KM_PWRCTRL_PWR_GPC_6_CFG_GPC_INVERT_SIZE 1
#define KM_PWRCTRL_PWR_GPC_6_CFG_GPC_PRIORITY_OFFSET 4
#define KM_PWRCTRL_PWR_GPC_6_CFG_GPC_PRIORITY_SIZE 4
#define KM_PWRCTRL_PWR_GPC_6_CFG_GPC_OFF_DIV_OFFSET 8
#define KM_PWRCTRL_PWR_GPC_6_CFG_GPC_OFF_DIV_SIZE 8
#define KM_PWRCTRL_PWR_GPC_6_CFG_GPC_ONOFF_COUNT_OFFSET 16
#define KM_PWRCTRL_PWR_GPC_6_CFG_GPC_ONOFF_COUNT_SIZE 16

/** bit field defines for km_pwrctrl_s#pwr_gpc_7_cfg */
#define KM_PWRCTRL_PWR_GPC_7_CFG_GPC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_GPC_7_CFG_GPC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_GPC_7_CFG_GPC_INVERT_OFFSET 1
#define KM_PWRCTRL_PWR_GPC_7_CFG_GPC_INVERT_SIZE 1
#define KM_PWRCTRL_PWR_GPC_7_CFG_GPC_PRIORITY_OFFSET 4
#define KM_PWRCTRL_PWR_GPC_7_CFG_GPC_PRIORITY_SIZE 4
#define KM_PWRCTRL_PWR_GPC_7_CFG_GPC_OFF_DIV_OFFSET 8
#define KM_PWRCTRL_PWR_GPC_7_CFG_GPC_OFF_DIV_SIZE 8
#define KM_PWRCTRL_PWR_GPC_7_CFG_GPC_ONOFF_COUNT_OFFSET 16
#define KM_PWRCTRL_PWR_GPC_7_CFG_GPC_ONOFF_COUNT_SIZE 16

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_0_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_0_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_0_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_0_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_0_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_0_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_0_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_1_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_1_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_1_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_1_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_1_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_1_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_1_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_2_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_2_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_2_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_2_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_2_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_2_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_2_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_3_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_3_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_3_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_3_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_3_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_3_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_3_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_4_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_4_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_4_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_4_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_4_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_4_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_4_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_5_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_5_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_5_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_5_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_5_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_5_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_5_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_6_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_6_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_6_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_6_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_6_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_6_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_6_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_7_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_7_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_7_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_7_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_7_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_7_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_7_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_8_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_8_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_8_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_8_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_8_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_8_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_8_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_9_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_9_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_9_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_9_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_9_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_9_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_9_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_10_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_10_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_10_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_10_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_10_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_10_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_10_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_11_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_11_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_11_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_11_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_11_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_11_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_11_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_12_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_12_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_12_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_12_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_12_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_12_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_12_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_13_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_13_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_13_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_13_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_13_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_13_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_13_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_14_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_14_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_14_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_14_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_14_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_14_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_14_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_15_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_15_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_15_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_15_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_15_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_15_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_15_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_16_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_16_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_16_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_16_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_16_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_16_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_16_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_17_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_17_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_17_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_17_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_17_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_17_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_17_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_18_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_18_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_18_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_18_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_18_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_18_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_18_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_19_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_19_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_19_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_19_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_19_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_19_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_19_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_20_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_20_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_20_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_20_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_20_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_20_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_20_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_21_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_21_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_21_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_21_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_21_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_21_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_21_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_22_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_22_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_22_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_22_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_22_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_22_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_22_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_23_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_23_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_23_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_23_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_23_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_23_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_23_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_24_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_24_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_24_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_24_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_24_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_24_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_24_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_25_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_25_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_25_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_25_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_25_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_25_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_25_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_26_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_26_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_26_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_26_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_26_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_26_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_26_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_27_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_27_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_27_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_27_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_27_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_27_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_27_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_28_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_28_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_28_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_28_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_28_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_28_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_28_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_29_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_29_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_29_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_29_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_29_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_29_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_29_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_30_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_30_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_30_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_30_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_30_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_30_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_30_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_wakeupsrc_31_cfg */
#define KM_PWRCTRL_PWR_WAKEUPSRC_31_CFG_WAKEUP_SRC_ENABLE_OFFSET 0
#define KM_PWRCTRL_PWR_WAKEUPSRC_31_CFG_WAKEUP_SRC_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_WAKEUPSRC_31_CFG_WAKEUP_SRC_PRIORITY_OFFSET 8
#define KM_PWRCTRL_PWR_WAKEUPSRC_31_CFG_WAKEUP_SRC_PRIORITY_SIZE 5
#define KM_PWRCTRL_PWR_WAKEUPSRC_31_CFG_WAKEUP_SRC_DEST_PM_OFFSET 16
#define KM_PWRCTRL_PWR_WAKEUPSRC_31_CFG_WAKEUP_SRC_DEST_PM_SIZE 4

/** bit field defines for km_pwrctrl_s#pwr_mode_p0_cfg */
#define KM_PWRCTRL_PWR_MODE_P0_CFG_PD_APP_ON_OFFSET 0
#define KM_PWRCTRL_PWR_MODE_P0_CFG_PD_APP_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P0_CFG_PD_MODEM_ON_OFFSET 1
#define KM_PWRCTRL_PWR_MODE_P0_CFG_PD_MODEM_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P0_CFG_PD_USB_ON_OFFSET 2
#define KM_PWRCTRL_PWR_MODE_P0_CFG_PD_USB_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P0_CFG_PD_RFANA_DIG_ON_OFFSET 3
#define KM_PWRCTRL_PWR_MODE_P0_CFG_PD_RFANA_DIG_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P0_CFG_PD_RFANA_1V8_ON_OFFSET 4
#define KM_PWRCTRL_PWR_MODE_P0_CFG_PD_RFANA_1V8_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P0_CFG_GPC_VAL_OFFSET 16
#define KM_PWRCTRL_PWR_MODE_P0_CFG_GPC_VAL_SIZE 8
#define KM_PWRCTRL_PWR_MODE_P0_CFG_ALWAYS_ON_CLK_SEL_OFFSET 28
#define KM_PWRCTRL_PWR_MODE_P0_CFG_ALWAYS_ON_CLK_SEL_SIZE 2
#define KM_PWRCTRL_PWR_MODE_P0_CFG_AUTO_CLK_SLEEP_ENABLE_OFFSET 31
#define KM_PWRCTRL_PWR_MODE_P0_CFG_AUTO_CLK_SLEEP_ENABLE_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_mode_p1_cfg */
#define KM_PWRCTRL_PWR_MODE_P1_CFG_PD_APP_ON_OFFSET 0
#define KM_PWRCTRL_PWR_MODE_P1_CFG_PD_APP_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P1_CFG_PD_MODEM_ON_OFFSET 1
#define KM_PWRCTRL_PWR_MODE_P1_CFG_PD_MODEM_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P1_CFG_PD_USB_ON_OFFSET 2
#define KM_PWRCTRL_PWR_MODE_P1_CFG_PD_USB_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P1_CFG_PD_RFANA_DIG_ON_OFFSET 3
#define KM_PWRCTRL_PWR_MODE_P1_CFG_PD_RFANA_DIG_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P1_CFG_PD_RFANA_1V8_ON_OFFSET 4
#define KM_PWRCTRL_PWR_MODE_P1_CFG_PD_RFANA_1V8_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P1_CFG_GPC_VAL_OFFSET 16
#define KM_PWRCTRL_PWR_MODE_P1_CFG_GPC_VAL_SIZE 8
#define KM_PWRCTRL_PWR_MODE_P1_CFG_ALWAYS_ON_CLK_SEL_OFFSET 28
#define KM_PWRCTRL_PWR_MODE_P1_CFG_ALWAYS_ON_CLK_SEL_SIZE 2
#define KM_PWRCTRL_PWR_MODE_P1_CFG_AUTO_CLK_SLEEP_ENABLE_OFFSET 31
#define KM_PWRCTRL_PWR_MODE_P1_CFG_AUTO_CLK_SLEEP_ENABLE_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_mode_p2_cfg */
#define KM_PWRCTRL_PWR_MODE_P2_CFG_PD_APP_ON_OFFSET 0
#define KM_PWRCTRL_PWR_MODE_P2_CFG_PD_APP_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P2_CFG_PD_MODEM_ON_OFFSET 1
#define KM_PWRCTRL_PWR_MODE_P2_CFG_PD_MODEM_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P2_CFG_PD_USB_ON_OFFSET 2
#define KM_PWRCTRL_PWR_MODE_P2_CFG_PD_USB_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P2_CFG_PD_RFANA_DIG_ON_OFFSET 3
#define KM_PWRCTRL_PWR_MODE_P2_CFG_PD_RFANA_DIG_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P2_CFG_PD_RFANA_1V8_ON_OFFSET 4
#define KM_PWRCTRL_PWR_MODE_P2_CFG_PD_RFANA_1V8_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P2_CFG_GPC_VAL_OFFSET 16
#define KM_PWRCTRL_PWR_MODE_P2_CFG_GPC_VAL_SIZE 8
#define KM_PWRCTRL_PWR_MODE_P2_CFG_ALWAYS_ON_CLK_SEL_OFFSET 28
#define KM_PWRCTRL_PWR_MODE_P2_CFG_ALWAYS_ON_CLK_SEL_SIZE 2
#define KM_PWRCTRL_PWR_MODE_P2_CFG_AUTO_CLK_SLEEP_ENABLE_OFFSET 31
#define KM_PWRCTRL_PWR_MODE_P2_CFG_AUTO_CLK_SLEEP_ENABLE_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_mode_p3_cfg */
#define KM_PWRCTRL_PWR_MODE_P3_CFG_PD_APP_ON_OFFSET 0
#define KM_PWRCTRL_PWR_MODE_P3_CFG_PD_APP_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P3_CFG_PD_MODEM_ON_OFFSET 1
#define KM_PWRCTRL_PWR_MODE_P3_CFG_PD_MODEM_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P3_CFG_PD_USB_ON_OFFSET 2
#define KM_PWRCTRL_PWR_MODE_P3_CFG_PD_USB_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P3_CFG_PD_RFANA_DIG_ON_OFFSET 3
#define KM_PWRCTRL_PWR_MODE_P3_CFG_PD_RFANA_DIG_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P3_CFG_PD_RFANA_1V8_ON_OFFSET 4
#define KM_PWRCTRL_PWR_MODE_P3_CFG_PD_RFANA_1V8_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P3_CFG_GPC_VAL_OFFSET 16
#define KM_PWRCTRL_PWR_MODE_P3_CFG_GPC_VAL_SIZE 8
#define KM_PWRCTRL_PWR_MODE_P3_CFG_ALWAYS_ON_CLK_SEL_OFFSET 28
#define KM_PWRCTRL_PWR_MODE_P3_CFG_ALWAYS_ON_CLK_SEL_SIZE 2
#define KM_PWRCTRL_PWR_MODE_P3_CFG_AUTO_CLK_SLEEP_ENABLE_OFFSET 31
#define KM_PWRCTRL_PWR_MODE_P3_CFG_AUTO_CLK_SLEEP_ENABLE_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_mode_p4_cfg */
#define KM_PWRCTRL_PWR_MODE_P4_CFG_PD_APP_ON_OFFSET 0
#define KM_PWRCTRL_PWR_MODE_P4_CFG_PD_APP_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P4_CFG_PD_MODEM_ON_OFFSET 1
#define KM_PWRCTRL_PWR_MODE_P4_CFG_PD_MODEM_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P4_CFG_PD_USB_ON_OFFSET 2
#define KM_PWRCTRL_PWR_MODE_P4_CFG_PD_USB_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P4_CFG_PD_RFANA_DIG_ON_OFFSET 3
#define KM_PWRCTRL_PWR_MODE_P4_CFG_PD_RFANA_DIG_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P4_CFG_PD_RFANA_1V8_ON_OFFSET 4
#define KM_PWRCTRL_PWR_MODE_P4_CFG_PD_RFANA_1V8_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P4_CFG_GPC_VAL_OFFSET 16
#define KM_PWRCTRL_PWR_MODE_P4_CFG_GPC_VAL_SIZE 8
#define KM_PWRCTRL_PWR_MODE_P4_CFG_ALWAYS_ON_CLK_SEL_OFFSET 28
#define KM_PWRCTRL_PWR_MODE_P4_CFG_ALWAYS_ON_CLK_SEL_SIZE 2
#define KM_PWRCTRL_PWR_MODE_P4_CFG_AUTO_CLK_SLEEP_ENABLE_OFFSET 31
#define KM_PWRCTRL_PWR_MODE_P4_CFG_AUTO_CLK_SLEEP_ENABLE_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_mode_p5_cfg */
#define KM_PWRCTRL_PWR_MODE_P5_CFG_PD_APP_ON_OFFSET 0
#define KM_PWRCTRL_PWR_MODE_P5_CFG_PD_APP_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P5_CFG_PD_MODEM_ON_OFFSET 1
#define KM_PWRCTRL_PWR_MODE_P5_CFG_PD_MODEM_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P5_CFG_PD_USB_ON_OFFSET 2
#define KM_PWRCTRL_PWR_MODE_P5_CFG_PD_USB_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P5_CFG_PD_RFANA_DIG_ON_OFFSET 3
#define KM_PWRCTRL_PWR_MODE_P5_CFG_PD_RFANA_DIG_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P5_CFG_PD_RFANA_1V8_ON_OFFSET 4
#define KM_PWRCTRL_PWR_MODE_P5_CFG_PD_RFANA_1V8_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P5_CFG_GPC_VAL_OFFSET 16
#define KM_PWRCTRL_PWR_MODE_P5_CFG_GPC_VAL_SIZE 8
#define KM_PWRCTRL_PWR_MODE_P5_CFG_ALWAYS_ON_CLK_SEL_OFFSET 28
#define KM_PWRCTRL_PWR_MODE_P5_CFG_ALWAYS_ON_CLK_SEL_SIZE 2
#define KM_PWRCTRL_PWR_MODE_P5_CFG_AUTO_CLK_SLEEP_ENABLE_OFFSET 31
#define KM_PWRCTRL_PWR_MODE_P5_CFG_AUTO_CLK_SLEEP_ENABLE_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_mode_p6_cfg */
#define KM_PWRCTRL_PWR_MODE_P6_CFG_PD_APP_ON_OFFSET 0
#define KM_PWRCTRL_PWR_MODE_P6_CFG_PD_APP_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P6_CFG_PD_MODEM_ON_OFFSET 1
#define KM_PWRCTRL_PWR_MODE_P6_CFG_PD_MODEM_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P6_CFG_PD_USB_ON_OFFSET 2
#define KM_PWRCTRL_PWR_MODE_P6_CFG_PD_USB_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P6_CFG_PD_RFANA_DIG_ON_OFFSET 3
#define KM_PWRCTRL_PWR_MODE_P6_CFG_PD_RFANA_DIG_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P6_CFG_PD_RFANA_1V8_ON_OFFSET 4
#define KM_PWRCTRL_PWR_MODE_P6_CFG_PD_RFANA_1V8_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P6_CFG_GPC_VAL_OFFSET 16
#define KM_PWRCTRL_PWR_MODE_P6_CFG_GPC_VAL_SIZE 8
#define KM_PWRCTRL_PWR_MODE_P6_CFG_ALWAYS_ON_CLK_SEL_OFFSET 28
#define KM_PWRCTRL_PWR_MODE_P6_CFG_ALWAYS_ON_CLK_SEL_SIZE 2
#define KM_PWRCTRL_PWR_MODE_P6_CFG_AUTO_CLK_SLEEP_ENABLE_OFFSET 31
#define KM_PWRCTRL_PWR_MODE_P6_CFG_AUTO_CLK_SLEEP_ENABLE_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_mode_p7_cfg */
#define KM_PWRCTRL_PWR_MODE_P7_CFG_PD_APP_ON_OFFSET 0
#define KM_PWRCTRL_PWR_MODE_P7_CFG_PD_APP_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P7_CFG_PD_MODEM_ON_OFFSET 1
#define KM_PWRCTRL_PWR_MODE_P7_CFG_PD_MODEM_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P7_CFG_PD_USB_ON_OFFSET 2
#define KM_PWRCTRL_PWR_MODE_P7_CFG_PD_USB_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P7_CFG_PD_RFANA_DIG_ON_OFFSET 3
#define KM_PWRCTRL_PWR_MODE_P7_CFG_PD_RFANA_DIG_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P7_CFG_PD_RFANA_1V8_ON_OFFSET 4
#define KM_PWRCTRL_PWR_MODE_P7_CFG_PD_RFANA_1V8_ON_SIZE 1
#define KM_PWRCTRL_PWR_MODE_P7_CFG_GPC_VAL_OFFSET 16
#define KM_PWRCTRL_PWR_MODE_P7_CFG_GPC_VAL_SIZE 8
#define KM_PWRCTRL_PWR_MODE_P7_CFG_ALWAYS_ON_CLK_SEL_OFFSET 28
#define KM_PWRCTRL_PWR_MODE_P7_CFG_ALWAYS_ON_CLK_SEL_SIZE 2
#define KM_PWRCTRL_PWR_MODE_P7_CFG_AUTO_CLK_SLEEP_ENABLE_OFFSET 31
#define KM_PWRCTRL_PWR_MODE_P7_CFG_AUTO_CLK_SLEEP_ENABLE_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_mem_00_cfg */
#define KM_PWRCTRL_PWR_MEM_00_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_00_CFG_INSTANCE_ACTIVE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_00_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_00_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_00_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_00_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_00_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_00_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_00_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_00_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_00_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_00_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_00_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_00_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_00_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_00_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_01_cfg */
#define KM_PWRCTRL_PWR_MEM_01_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_01_CFG_INSTANCE_ACTIVE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_01_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_01_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_01_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_01_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_01_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_01_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_01_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_01_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_01_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_01_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_01_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_01_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_01_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_01_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_02_cfg */
#define KM_PWRCTRL_PWR_MEM_02_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_02_CFG_INSTANCE_ACTIVE_SIZE 4
#define KM_PWRCTRL_PWR_MEM_02_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_02_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_02_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_02_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_02_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_02_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_02_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_02_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_02_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_02_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_02_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_02_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_02_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_02_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_03_cfg */
#define KM_PWRCTRL_PWR_MEM_03_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_03_CFG_INSTANCE_ACTIVE_SIZE 12
#define KM_PWRCTRL_PWR_MEM_03_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_03_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_03_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_03_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_03_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_03_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_03_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_03_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_03_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_03_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_03_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_03_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_03_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_03_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_04_cfg */
#define KM_PWRCTRL_PWR_MEM_04_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_04_CFG_INSTANCE_ACTIVE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_04_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_04_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_04_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_04_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_04_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_04_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_04_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_04_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_04_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_04_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_04_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_04_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_04_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_04_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_05_cfg */
#define KM_PWRCTRL_PWR_MEM_05_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_05_CFG_INSTANCE_ACTIVE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_05_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_05_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_05_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_05_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_05_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_05_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_05_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_05_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_05_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_05_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_05_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_05_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_05_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_05_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_06_cfg */
#define KM_PWRCTRL_PWR_MEM_06_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_06_CFG_INSTANCE_ACTIVE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_06_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_06_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_06_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_06_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_06_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_06_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_06_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_06_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_06_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_06_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_06_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_06_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_06_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_06_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_07_cfg */
#define KM_PWRCTRL_PWR_MEM_07_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_07_CFG_INSTANCE_ACTIVE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_07_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_07_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_07_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_07_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_07_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_07_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_07_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_07_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_07_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_07_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_07_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_07_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_07_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_07_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_08_cfg */
#define KM_PWRCTRL_PWR_MEM_08_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_08_CFG_INSTANCE_ACTIVE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_08_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_08_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_08_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_08_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_08_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_08_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_08_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_08_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_08_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_08_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_08_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_08_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_08_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_08_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_09_cfg */
#define KM_PWRCTRL_PWR_MEM_09_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_09_CFG_INSTANCE_ACTIVE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_09_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_09_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_09_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_09_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_09_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_09_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_09_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_09_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_09_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_09_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_09_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_09_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_09_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_09_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_10_cfg */
#define KM_PWRCTRL_PWR_MEM_10_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_10_CFG_INSTANCE_ACTIVE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_10_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_10_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_10_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_10_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_10_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_10_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_10_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_10_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_10_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_10_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_10_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_10_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_10_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_10_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_11_cfg */
#define KM_PWRCTRL_PWR_MEM_11_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_11_CFG_INSTANCE_ACTIVE_SIZE 5
#define KM_PWRCTRL_PWR_MEM_11_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_11_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_11_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_11_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_11_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_11_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_11_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_11_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_11_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_11_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_11_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_11_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_11_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_11_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_12_cfg */
#define KM_PWRCTRL_PWR_MEM_12_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_12_CFG_INSTANCE_ACTIVE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_12_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_12_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_12_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_12_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_12_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_12_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_12_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_12_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_12_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_12_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_12_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_12_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_12_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_12_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_13_cfg */
#define KM_PWRCTRL_PWR_MEM_13_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_13_CFG_INSTANCE_ACTIVE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_13_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_13_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_13_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_13_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_13_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_13_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_13_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_13_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_13_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_13_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_13_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_13_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_13_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_13_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_14_cfg */
#define KM_PWRCTRL_PWR_MEM_14_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_14_CFG_INSTANCE_ACTIVE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_14_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_14_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_14_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_14_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_14_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_14_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_14_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_14_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_14_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_14_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_14_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_14_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_14_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_14_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_15_cfg */
#define KM_PWRCTRL_PWR_MEM_15_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_15_CFG_INSTANCE_ACTIVE_SIZE 5
#define KM_PWRCTRL_PWR_MEM_15_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_15_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_15_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_15_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_15_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_15_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_15_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_15_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_15_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_15_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_15_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_15_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_15_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_15_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_16_cfg */
#define KM_PWRCTRL_PWR_MEM_16_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_16_CFG_INSTANCE_ACTIVE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_16_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_16_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_16_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_16_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_16_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_16_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_16_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_16_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_16_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_16_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_16_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_16_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_16_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_16_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_17_cfg */
#define KM_PWRCTRL_PWR_MEM_17_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_17_CFG_INSTANCE_ACTIVE_SIZE 4
#define KM_PWRCTRL_PWR_MEM_17_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_17_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_17_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_17_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_17_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_17_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_17_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_17_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_17_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_17_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_17_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_17_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_17_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_17_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_18_cfg */
#define KM_PWRCTRL_PWR_MEM_18_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_18_CFG_INSTANCE_ACTIVE_SIZE 12
#define KM_PWRCTRL_PWR_MEM_18_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_18_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_18_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_18_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_18_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_18_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_18_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_18_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_18_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_18_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_18_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_18_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_18_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_18_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_19_cfg */
#define KM_PWRCTRL_PWR_MEM_19_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_19_CFG_INSTANCE_ACTIVE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_19_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_19_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_19_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_19_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_19_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_19_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_19_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_19_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_19_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_19_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_19_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_19_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_19_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_19_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_20_cfg */
#define KM_PWRCTRL_PWR_MEM_20_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_20_CFG_INSTANCE_ACTIVE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_20_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_20_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_20_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_20_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_20_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_20_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_20_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_20_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_20_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_20_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_20_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_20_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_20_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_20_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_21_cfg */
#define KM_PWRCTRL_PWR_MEM_21_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_21_CFG_INSTANCE_ACTIVE_SIZE 8
#define KM_PWRCTRL_PWR_MEM_21_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_21_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_21_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_21_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_21_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_21_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_21_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_21_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_21_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_21_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_21_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_21_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_21_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_21_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_22_cfg */
#define KM_PWRCTRL_PWR_MEM_22_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_22_CFG_INSTANCE_ACTIVE_SIZE 8
#define KM_PWRCTRL_PWR_MEM_22_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_22_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_22_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_22_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_22_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_22_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_22_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_22_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_22_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_22_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_22_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_22_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_22_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_22_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_23_cfg */
#define KM_PWRCTRL_PWR_MEM_23_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_23_CFG_INSTANCE_ACTIVE_SIZE 8
#define KM_PWRCTRL_PWR_MEM_23_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_23_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_23_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_23_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_23_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_23_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_23_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_23_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_23_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_23_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_23_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_23_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_23_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_23_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_24_cfg */
#define KM_PWRCTRL_PWR_MEM_24_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_24_CFG_INSTANCE_ACTIVE_SIZE 8
#define KM_PWRCTRL_PWR_MEM_24_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_24_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_24_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_24_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_24_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_24_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_24_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_24_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_24_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_24_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_24_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_24_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_24_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_24_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_25_cfg */
#define KM_PWRCTRL_PWR_MEM_25_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_25_CFG_INSTANCE_ACTIVE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_25_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_25_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_25_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_25_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_25_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_25_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_25_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_25_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_25_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_25_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_25_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_25_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_25_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_25_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_26_cfg */
#define KM_PWRCTRL_PWR_MEM_26_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_26_CFG_INSTANCE_ACTIVE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_26_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_26_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_26_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_26_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_26_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_26_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_26_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_26_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_26_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_26_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_26_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_26_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_26_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_26_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_27_cfg */
#define KM_PWRCTRL_PWR_MEM_27_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_27_CFG_INSTANCE_ACTIVE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_27_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_27_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_27_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_27_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_27_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_27_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_27_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_27_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_27_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_27_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_27_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_27_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_27_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_27_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_28_cfg */
#define KM_PWRCTRL_PWR_MEM_28_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_28_CFG_INSTANCE_ACTIVE_SIZE 4
#define KM_PWRCTRL_PWR_MEM_28_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_28_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_28_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_28_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_28_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_28_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_28_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_28_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_28_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_28_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_28_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_28_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_28_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_28_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_mem_29_cfg */
#define KM_PWRCTRL_PWR_MEM_29_CFG_INSTANCE_ACTIVE_OFFSET 0
#define KM_PWRCTRL_PWR_MEM_29_CFG_INSTANCE_ACTIVE_SIZE 5
#define KM_PWRCTRL_PWR_MEM_29_CFG_PD_OFF_MEM_STATE_OFFSET 16
#define KM_PWRCTRL_PWR_MEM_29_CFG_PD_OFF_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_29_CFG_PD_ON_MEM_STATE_OFFSET 18
#define KM_PWRCTRL_PWR_MEM_29_CFG_PD_ON_MEM_STATE_SIZE 2
#define KM_PWRCTRL_PWR_MEM_29_CFG_SUPPORTS_DEEPSLEEP_OFFSET 20
#define KM_PWRCTRL_PWR_MEM_29_CFG_SUPPORTS_DEEPSLEEP_SIZE 1
#define KM_PWRCTRL_PWR_MEM_29_CFG_BANKED_OFFSET 21
#define KM_PWRCTRL_PWR_MEM_29_CFG_BANKED_SIZE 1
#define KM_PWRCTRL_PWR_MEM_29_CFG_LIGHT_SLEEP_FORCE_OFFSET 22
#define KM_PWRCTRL_PWR_MEM_29_CFG_LIGHT_SLEEP_FORCE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_29_CFG_LIGHT_SLEEP_ENABLE_OFFSET 23
#define KM_PWRCTRL_PWR_MEM_29_CFG_LIGHT_SLEEP_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MEM_29_CFG_ON_OFF_COUNT_OFFSET 24
#define KM_PWRCTRL_PWR_MEM_29_CFG_ON_OFF_COUNT_SIZE 8

/** bit field defines for km_pwrctrl_s#pwr_interrupt_enable */
#define KM_PWRCTRL_PWR_INTERRUPT_ENABLE_INT_TRANS_SUCCESS_OFFSET 0
#define KM_PWRCTRL_PWR_INTERRUPT_ENABLE_INT_TRANS_SUCCESS_SIZE 1
#define KM_PWRCTRL_PWR_INTERRUPT_ENABLE_INT_WAKEUP_PS_OFFSET 1
#define KM_PWRCTRL_PWR_INTERRUPT_ENABLE_INT_WAKEUP_PS_SIZE 1
#define KM_PWRCTRL_PWR_INTERRUPT_ENABLE_INT_PIO_INT_OFFSET 2
#define KM_PWRCTRL_PWR_INTERRUPT_ENABLE_INT_PIO_INT_SIZE 1
#define KM_PWRCTRL_PWR_INTERRUPT_ENABLE_INT_PWR_MODE_UPDATE_OFFSET 3
#define KM_PWRCTRL_PWR_INTERRUPT_ENABLE_INT_PWR_MODE_UPDATE_SIZE 1
#define KM_PWRCTRL_PWR_INTERRUPT_ENABLE_INT_MEM_PWR_MODE_UPDATE_OFFSET 4
#define KM_PWRCTRL_PWR_INTERRUPT_ENABLE_INT_MEM_PWR_MODE_UPDATE_SIZE 1
#define KM_PWRCTRL_PWR_INTERRUPT_ENABLE_INT_AO_CLK_UPDATE_OFFSET 5
#define KM_PWRCTRL_PWR_INTERRUPT_ENABLE_INT_AO_CLK_UPDATE_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_interrupt_status */
#define KM_PWRCTRL_PWR_INTERRUPT_STATUS_INT_TRANS_SUCCESS_OFFSET 0
#define KM_PWRCTRL_PWR_INTERRUPT_STATUS_INT_TRANS_SUCCESS_SIZE 1
#define KM_PWRCTRL_PWR_INTERRUPT_STATUS_INT_WAKEUP_PS_OFFSET 1
#define KM_PWRCTRL_PWR_INTERRUPT_STATUS_INT_WAKEUP_PS_SIZE 1
#define KM_PWRCTRL_PWR_INTERRUPT_STATUS_INT_PIO_INT_OFFSET 2
#define KM_PWRCTRL_PWR_INTERRUPT_STATUS_INT_PIO_INT_SIZE 1
#define KM_PWRCTRL_PWR_INTERRUPT_STATUS_INT_PWR_MODE_UPDATE_OFFSET 3
#define KM_PWRCTRL_PWR_INTERRUPT_STATUS_INT_PWR_MODE_UPDATE_SIZE 1
#define KM_PWRCTRL_PWR_INTERRUPT_STATUS_INT_MEM_PWR_MODE_UPDATE_OFFSET 4
#define KM_PWRCTRL_PWR_INTERRUPT_STATUS_INT_MEM_PWR_MODE_UPDATE_SIZE 1
#define KM_PWRCTRL_PWR_INTERRUPT_STATUS_INT_AO_CLK_UPDATE_OFFSET 5
#define KM_PWRCTRL_PWR_INTERRUPT_STATUS_INT_AO_CLK_UPDATE_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_interrupt_clear */
#define KM_PWRCTRL_PWR_INTERRUPT_CLEAR_INT_TRANS_SUCCESS_OFFSET 0
#define KM_PWRCTRL_PWR_INTERRUPT_CLEAR_INT_TRANS_SUCCESS_SIZE 1
#define KM_PWRCTRL_PWR_INTERRUPT_CLEAR_INT_WAKEUP_PS_OFFSET 1
#define KM_PWRCTRL_PWR_INTERRUPT_CLEAR_INT_WAKEUP_PS_SIZE 1
#define KM_PWRCTRL_PWR_INTERRUPT_CLEAR_INT_PIO_INT_OFFSET 2
#define KM_PWRCTRL_PWR_INTERRUPT_CLEAR_INT_PIO_INT_SIZE 1
#define KM_PWRCTRL_PWR_INTERRUPT_CLEAR_INT_PWR_MODE_UPDATE_OFFSET 3
#define KM_PWRCTRL_PWR_INTERRUPT_CLEAR_INT_PWR_MODE_UPDATE_SIZE 1
#define KM_PWRCTRL_PWR_INTERRUPT_CLEAR_INT_MEM_PWR_MODE_UPDATE_OFFSET 4
#define KM_PWRCTRL_PWR_INTERRUPT_CLEAR_INT_MEM_PWR_MODE_UPDATE_SIZE 1
#define KM_PWRCTRL_PWR_INTERRUPT_CLEAR_INT_AO_CLK_UPDATE_OFFSET 5
#define KM_PWRCTRL_PWR_INTERRUPT_CLEAR_INT_AO_CLK_UPDATE_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_fault_interrupt_enable */
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_ENABLE_PD_APP_MDM_CHK_OFFSET 0
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_ENABLE_PD_APP_MDM_CHK_SIZE 1
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_ENABLE_WKUP_DEST_CHK_OFFSET 1
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_ENABLE_WKUP_DEST_CHK_SIZE 1
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_ENABLE_WKUP_ENABLE_CHK_OFFSET 2
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_ENABLE_WKUP_ENABLE_CHK_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_fault_interrupt_status */
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_STATUS_PD_APP_MDM_CHK_OFFSET 0
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_STATUS_PD_APP_MDM_CHK_SIZE 1
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_STATUS_WKUP_DEST_CHK_OFFSET 1
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_STATUS_WKUP_DEST_CHK_SIZE 1
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_STATUS_WKUP_ENABLE_CHK_OFFSET 2
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_STATUS_WKUP_ENABLE_CHK_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_fault_interrupt_clear */
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_CLEAR_PD_APP_MDM_CHK_OFFSET 0
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_CLEAR_PD_APP_MDM_CHK_SIZE 1
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_CLEAR_WKUP_DEST_CHK_OFFSET 1
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_CLEAR_WKUP_DEST_CHK_SIZE 1
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_CLEAR_WKUP_ENABLE_CHK_OFFSET 2
#define KM_PWRCTRL_PWR_FAULT_INTERRUPT_CLEAR_WKUP_ENABLE_CHK_SIZE 1

/** bit field defines for km_pwrctrl_s#pwr_misc_ctrl */
#define KM_PWRCTRL_PWR_MISC_CTRL_EXT_RF_RESET_OFFSET 0
#define KM_PWRCTRL_PWR_MISC_CTRL_EXT_RF_RESET_SIZE 1
#define KM_PWRCTRL_PWR_MISC_CTRL_RFANA_TO_RFDIG_ISOLATE_OFFSET 4
#define KM_PWRCTRL_PWR_MISC_CTRL_RFANA_TO_RFDIG_ISOLATE_SIZE 4
#define KM_PWRCTRL_PWR_MISC_CTRL_OTP_MONITOR_PWR_ENABLE_OFFSET 8
#define KM_PWRCTRL_PWR_MISC_CTRL_OTP_MONITOR_PWR_ENABLE_SIZE 1
#define KM_PWRCTRL_PWR_MISC_CTRL_OTP_MONITOR_PWR_STATUS_OFFSET 16
#define KM_PWRCTRL_PWR_MISC_CTRL_OTP_MONITOR_PWR_STATUS_SIZE 1

/** bit field defines for km_pwrctrl_s#cpu_boot_vinit */
#define KM_PWRCTRL_CPU_BOOT_VINIT_NONE_OFFSET 0
#define KM_PWRCTRL_CPU_BOOT_VINIT_NONE_SIZE 32

/* EOF km_pwrctrl.h */
#endif
