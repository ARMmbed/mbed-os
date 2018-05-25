; 
; Copyright (C) u-blox
; All rights reserved.
; This source file is the sole property of u-blox. Reproduction or utilization
; of this source in whole or part is forbidden without the written consent of
; u-blox.
; 
; FILE: km_pwrctrl.hs

KM_PWRCTRL_HAL_VERSION EQU 1
KM_PWRCTRL_PWR_TICKS_PD_START EQU 0x000
KM_PWRCTRL_PWR_TICKS_PD_SMALL EQU 0x004
KM_PWRCTRL_PWR_TICKS_PD_BIG EQU 0x008
KM_PWRCTRL_PWR_TICKS_PD_ISOLATE EQU 0x00C
KM_PWRCTRL_PWR_TICKS_PD_RESET EQU 0x010
KM_PWRCTRL_PWR_HANDSHAKE_STATUS EQU 0x024
KM_PWRCTRL_PWR_HANDSHAKE_CFG EQU 0x028
KM_PWRCTRL_PWR_CURR_MODE EQU 0x030
KM_PWRCTRL_PWR_NEXT_MODE EQU 0x034
KM_PWRCTRL_PWR_UPDATE_MODE EQU 0x038
KM_PWRCTRL_PWR_GPC_0_CFG EQU 0x040
KM_PWRCTRL_PWR_GPC_1_CFG EQU 0x044
KM_PWRCTRL_PWR_GPC_2_CFG EQU 0x048
KM_PWRCTRL_PWR_GPC_3_CFG EQU 0x04C
KM_PWRCTRL_PWR_GPC_4_CFG EQU 0x050
KM_PWRCTRL_PWR_GPC_5_CFG EQU 0x054
KM_PWRCTRL_PWR_GPC_6_CFG EQU 0x058
KM_PWRCTRL_PWR_GPC_7_CFG EQU 0x05C
KM_PWRCTRL_PWR_WAKEUPSRC_0_CFG EQU 0x080
KM_PWRCTRL_PWR_WAKEUPSRC_1_CFG EQU 0x084
KM_PWRCTRL_PWR_WAKEUPSRC_2_CFG EQU 0x088
KM_PWRCTRL_PWR_WAKEUPSRC_3_CFG EQU 0x08C
KM_PWRCTRL_PWR_WAKEUPSRC_4_CFG EQU 0x090
KM_PWRCTRL_PWR_WAKEUPSRC_5_CFG EQU 0x094
KM_PWRCTRL_PWR_WAKEUPSRC_6_CFG EQU 0x098
KM_PWRCTRL_PWR_WAKEUPSRC_7_CFG EQU 0x09C
KM_PWRCTRL_PWR_WAKEUPSRC_8_CFG EQU 0x0A0
KM_PWRCTRL_PWR_WAKEUPSRC_9_CFG EQU 0x0A4
KM_PWRCTRL_PWR_WAKEUPSRC_10_CFG EQU 0x0A8
KM_PWRCTRL_PWR_WAKEUPSRC_11_CFG EQU 0x0AC
KM_PWRCTRL_PWR_WAKEUPSRC_12_CFG EQU 0x0B0
KM_PWRCTRL_PWR_WAKEUPSRC_13_CFG EQU 0x0B4
KM_PWRCTRL_PWR_WAKEUPSRC_14_CFG EQU 0x0B8
KM_PWRCTRL_PWR_WAKEUPSRC_15_CFG EQU 0x0BC
KM_PWRCTRL_PWR_WAKEUPSRC_16_CFG EQU 0x0C0
KM_PWRCTRL_PWR_WAKEUPSRC_17_CFG EQU 0x0C4
KM_PWRCTRL_PWR_WAKEUPSRC_18_CFG EQU 0x0C8
KM_PWRCTRL_PWR_WAKEUPSRC_19_CFG EQU 0x0CC
KM_PWRCTRL_PWR_WAKEUPSRC_20_CFG EQU 0x0D0
KM_PWRCTRL_PWR_WAKEUPSRC_21_CFG EQU 0x0D4
KM_PWRCTRL_PWR_WAKEUPSRC_22_CFG EQU 0x0D8
KM_PWRCTRL_PWR_WAKEUPSRC_23_CFG EQU 0x0DC
KM_PWRCTRL_PWR_WAKEUPSRC_24_CFG EQU 0x0E0
KM_PWRCTRL_PWR_WAKEUPSRC_25_CFG EQU 0x0E4
KM_PWRCTRL_PWR_WAKEUPSRC_26_CFG EQU 0x0E8
KM_PWRCTRL_PWR_WAKEUPSRC_27_CFG EQU 0x0EC
KM_PWRCTRL_PWR_WAKEUPSRC_28_CFG EQU 0x0F0
KM_PWRCTRL_PWR_WAKEUPSRC_29_CFG EQU 0x0F4
KM_PWRCTRL_PWR_WAKEUPSRC_30_CFG EQU 0x0F8
KM_PWRCTRL_PWR_WAKEUPSRC_31_CFG EQU 0x0FC
KM_PWRCTRL_PWR_MODE_P0_CFG EQU 0x100
KM_PWRCTRL_PWR_MODE_P1_CFG EQU 0x104
KM_PWRCTRL_PWR_MODE_P2_CFG EQU 0x108
KM_PWRCTRL_PWR_MODE_P3_CFG EQU 0x10C
KM_PWRCTRL_PWR_MODE_P4_CFG EQU 0x110
KM_PWRCTRL_PWR_MODE_P5_CFG EQU 0x114
KM_PWRCTRL_PWR_MODE_P6_CFG EQU 0x118
KM_PWRCTRL_PWR_MODE_P7_CFG EQU 0x11C
KM_PWRCTRL_PWR_MEM_00_CFG EQU 0x200
KM_PWRCTRL_PWR_MEM_01_CFG EQU 0x204
KM_PWRCTRL_PWR_MEM_02_CFG EQU 0x208
KM_PWRCTRL_PWR_MEM_03_CFG EQU 0x20C
KM_PWRCTRL_PWR_MEM_04_CFG EQU 0x210
KM_PWRCTRL_PWR_MEM_05_CFG EQU 0x214
KM_PWRCTRL_PWR_MEM_06_CFG EQU 0x218
KM_PWRCTRL_PWR_MEM_07_CFG EQU 0x21C
KM_PWRCTRL_PWR_MEM_08_CFG EQU 0x220
KM_PWRCTRL_PWR_MEM_09_CFG EQU 0x224
KM_PWRCTRL_PWR_MEM_10_CFG EQU 0x228
KM_PWRCTRL_PWR_MEM_11_CFG EQU 0x22C
KM_PWRCTRL_PWR_MEM_12_CFG EQU 0x230
KM_PWRCTRL_PWR_MEM_13_CFG EQU 0x234
KM_PWRCTRL_PWR_MEM_14_CFG EQU 0x238
KM_PWRCTRL_PWR_MEM_15_CFG EQU 0x23C
KM_PWRCTRL_PWR_MEM_16_CFG EQU 0x240
KM_PWRCTRL_PWR_MEM_17_CFG EQU 0x244
KM_PWRCTRL_PWR_MEM_18_CFG EQU 0x248
KM_PWRCTRL_PWR_MEM_19_CFG EQU 0x24C
KM_PWRCTRL_PWR_MEM_20_CFG EQU 0x250
KM_PWRCTRL_PWR_MEM_21_CFG EQU 0x254
KM_PWRCTRL_PWR_MEM_22_CFG EQU 0x258
KM_PWRCTRL_PWR_MEM_23_CFG EQU 0x25C
KM_PWRCTRL_PWR_MEM_24_CFG EQU 0x260
KM_PWRCTRL_PWR_MEM_25_CFG EQU 0x264
KM_PWRCTRL_PWR_MEM_26_CFG EQU 0x268
KM_PWRCTRL_PWR_MEM_27_CFG EQU 0x26C
KM_PWRCTRL_PWR_MEM_28_CFG EQU 0x270
KM_PWRCTRL_PWR_MEM_29_CFG EQU 0x274
KM_PWRCTRL_PWR_INTERRUPT_ENABLE EQU 0x300
KM_PWRCTRL_PWR_INTERRUPT_STATUS EQU 0x304
KM_PWRCTRL_PWR_INTERRUPT_CLEAR EQU 0x308
KM_PWRCTRL_PWR_FAULT_INTERRUPT_ENABLE EQU 0x310
KM_PWRCTRL_PWR_FAULT_INTERRUPT_STATUS EQU 0x314
KM_PWRCTRL_PWR_FAULT_INTERRUPT_CLEAR EQU 0x318
KM_PWRCTRL_PWR_MISC_CTRL EQU 0x320
KM_PWRCTRL_CPU_BOOT_VINIT EQU 0xD00
; EOF: km_pwrctrl.hs
   END
