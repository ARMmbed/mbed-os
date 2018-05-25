; 
; Copyright (C) u-blox
; All rights reserved.
; This source file is the sole property of u-blox. Reproduction or utilization
; of this source in whole or part is forbidden without the written consent of
; u-blox.
; 
; FILE: km_ofdm_demux.hs

KM_OFDM_DEMUX_HAL_VERSION EQU 2
KM_OFDM_DEMUX_PHASETABLE_0 EQU 0x000
KM_OFDM_DEMUX_PHASETABLE_1 EQU 0x004
KM_OFDM_DEMUX_PHASETABLE_2 EQU 0x008
KM_OFDM_DEMUX_PHASETABLE_3 EQU 0x00C
KM_OFDM_DEMUX_DCCORRECTION_0 EQU 0x080
KM_OFDM_DEMUX_DCCORRECTION_1 EQU 0x084
KM_OFDM_DEMUX_DCCORRECTION_2 EQU 0x088
KM_OFDM_DEMUX_DCCORRECTION_3 EQU 0x08C
KM_OFDM_DEMUX_COMMONPARAMETERS_0 EQU 0x100
KM_OFDM_DEMUX_CTRL_0 EQU 0x104
KM_OFDM_DEMUX_PHASETABLESELECT_0 EQU 0x108
KM_OFDM_DEMUX_DCSELECT_0 EQU 0x10C
KM_OFDM_DEMUX_FFTSHIFTIN_0 EQU 0x110
KM_OFDM_DEMUX_FFTOUT_0 EQU 0x114
KM_OFDM_DEMUX_DMADEST_0 EQU 0x118
KM_OFDM_DEMUX_DMAINCR_0 EQU 0x11C
KM_OFDM_DEMUX_DMADESTMEAS_0 EQU 0x120
KM_OFDM_DEMUX_STATUSCTRL_0 EQU 0x124
KM_OFDM_DEMUX_COMMONPARAMETERS_1 EQU 0x128
KM_OFDM_DEMUX_CTRL_1 EQU 0x12C
KM_OFDM_DEMUX_PHASETABLESELECT_1 EQU 0x130
KM_OFDM_DEMUX_DCSELECT_1 EQU 0x134
KM_OFDM_DEMUX_FFTSHIFTIN_1 EQU 0x138
KM_OFDM_DEMUX_FFTOUT_1 EQU 0x13C
KM_OFDM_DEMUX_DMADEST_1 EQU 0x140
KM_OFDM_DEMUX_DMAINCR_1 EQU 0x144
KM_OFDM_DEMUX_DMADESTMEAS_1 EQU 0x148
KM_OFDM_DEMUX_STATUSCTRL_1 EQU 0x14C
KM_OFDM_DEMUX_STATUSOFDMDEMUX EQU 0x150
KM_OFDM_DEMUX_ADDROFFSET_0 EQU 0x200
KM_OFDM_DEMUX_ADDROFFSET_1 EQU 0x204
KM_OFDM_DEMUX_ADDROFFSET_2 EQU 0x208
KM_OFDM_DEMUX_ADDROFFSET_3 EQU 0x20C
KM_OFDM_DEMUX_ADDROFFSET_4 EQU 0x210
KM_OFDM_DEMUX_ADDROFFSET_5 EQU 0x214
KM_OFDM_DEMUX_ADDROFFSET_6 EQU 0x218
KM_OFDM_DEMUX_ADDROFFSET_7 EQU 0x21C
KM_OFDM_DEMUX_ADDROFFSET_8 EQU 0x220
KM_OFDM_DEMUX_ADDROFFSET_9 EQU 0x224
KM_OFDM_DEMUX_ADDROFFSET_10 EQU 0x228
KM_OFDM_DEMUX_ADDROFFSET_11 EQU 0x22C
KM_OFDM_DEMUX_ADDROFFSET_12 EQU 0x230
KM_OFDM_DEMUX_ADDROFFSET_13 EQU 0x234
KM_OFDM_DEMUX_ADDROFFSET_14 EQU 0x238
KM_OFDM_DEMUX_ADDROFFSET_15 EQU 0x23C
KM_OFDM_DEMUX_ADDROFFSET_16 EQU 0x240
KM_OFDM_DEMUX_ADDROFFSET_17 EQU 0x244
KM_OFDM_DEMUX_ADDROFFSET_18 EQU 0x248
KM_OFDM_DEMUX_ADDROFFSET_19 EQU 0x24C
KM_OFDM_DEMUX_ADDROFFSET_20 EQU 0x250
KM_OFDM_DEMUX_ADDROFFSET_21 EQU 0x254
KM_OFDM_DEMUX_ADDROFFSET_22 EQU 0x258
KM_OFDM_DEMUX_ADDROFFSET_23 EQU 0x25C
KM_OFDM_DEMUX_ADDROFFSET_24 EQU 0x260
KM_OFDM_DEMUX_ADDROFFSET_25 EQU 0x264
KM_OFDM_DEMUX_ADDROFFSET_26 EQU 0x268
KM_OFDM_DEMUX_ADDROFFSET_27 EQU 0x26C
KM_OFDM_DEMUX_EVENTSTATUS EQU 0x300
KM_OFDM_DEMUX_INTERRUPTMASK_0 EQU 0x304
KM_OFDM_DEMUX_INTERRUPTSTATUS_0 EQU 0x308
KM_OFDM_DEMUX_INTERRUPTMASK_1 EQU 0x30C
KM_OFDM_DEMUX_INTERRUPTSTATUS_1 EQU 0x310
KM_OFDM_DEMUX_INTERRUPTMASK_2 EQU 0x314
KM_OFDM_DEMUX_INTERRUPTSTATUS_2 EQU 0x318
KM_OFDM_DEMUX_INTERRUPTMASK_3 EQU 0x31C
KM_OFDM_DEMUX_INTERRUPTSTATUS_3 EQU 0x320
KM_OFDM_DEMUX_HWOBS EQU 0x324
KM_OFDM_DEMUX_PERIPHERALID4 EQU 0xFD0
KM_OFDM_DEMUX_PERIPHERALID5 EQU 0xFD4
KM_OFDM_DEMUX_PERIPHERALID6 EQU 0xFD8
KM_OFDM_DEMUX_PERIPHERALID7 EQU 0xFDC
KM_OFDM_DEMUX_PERIPHERALID0 EQU 0xFE0
KM_OFDM_DEMUX_PERIPHERALID1 EQU 0xFE4
KM_OFDM_DEMUX_PERIPHERALID2 EQU 0xFE8
KM_OFDM_DEMUX_PERIPHERALID3 EQU 0xFEC
KM_OFDM_DEMUX_COMPONENTID0 EQU 0xFF0
KM_OFDM_DEMUX_COMPONENTID1 EQU 0xFF4
KM_OFDM_DEMUX_COMPONENTID2 EQU 0xFF8
KM_OFDM_DEMUX_COMPONENTID3 EQU 0xFFC
; EOF: km_ofdm_demux.hs
   END
