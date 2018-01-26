#ifndef INCLUDED_KM_OFDM_DEMUX
#define INCLUDED_KM_OFDM_DEMUX
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** km_ofdm_demux HAL Spreadsheet version number */
#define KM_OFDM_DEMUX_HAL_VERSION 2

/** This is the OFDM register module
*/
struct km_ofdm_demux_s {
   /** Configures symbol phase shift in FBUF_DSP engine. at address offset 0x000, read-write */
   uint32_t phasetable_0;
   /** Configures symbol phase shift in FBUF_DSP engine. at address offset 0x004, read-write */
   uint32_t phasetable_1;
   /** Configures symbol phase shift in FBUF_DSP engine. at address offset 0x008, read-write */
   uint32_t phasetable_2;
   /** Configures symbol phase shift in FBUF_DSP engine. at address offset 0x00C, read-write */
   uint32_t phasetable_3;
   /** Reserved space */
   uint8_t fill0[112];
   /** Configures symbol dc correction in FBUF_DSP engine. at address offset 0x080, read-write */
   uint32_t dccorrection_0;
   /** Configures symbol dc correction in FBUF_DSP engine. at address offset 0x084, read-write */
   uint32_t dccorrection_1;
   /** Configures symbol dc correction in FBUF_DSP engine. at address offset 0x088, read-write */
   uint32_t dccorrection_2;
   /** Configures symbol dc correction in FBUF_DSP engine. at address offset 0x08C, read-write */
   uint32_t dccorrection_3;
   /** Reserved space */
   uint8_t fill1[112];
   /** NOT SHADOWED. at address offset 0x100, read-write */
   uint32_t commonparameters_0;
   /** Control register - h/w reads when block is idle and EnableSf  is set. at address offset 0x104, read-write */
   uint32_t ctrl_0;
   /** Select phase table entry per symbol - 2LSB -> Symbol 0.   Own vector entry for single write for all sym when PhaseTable is updated. at address offset 0x108, read-write */
   uint32_t phasetableselect_0;
   /** Select dc correction entry per symbol - 2LSB -> Symbol 0. Own vector entry for single write for all sym when dc correction  is updated. at address offset 0x10C, read-write */
   uint32_t dcselect_0;
   /** Input data address offset, +ve = timing advance at address offset 0x110, read-write */
   uint32_t fftshiftin_0;
   /** Two banks of FFT output (avoiding central DC). Both banks written consecutively to DMA, BankA then BankB. LoB must be higher HiA else BankB disabled. at address offset 0x114, read-write */
   uint32_t fftout_0;
   /** SHADOWED at address offset 0x118, read-write */
   uint32_t dmadest_0;
   /** NOT SHADOWED  128 x 16b iq = 512 byte = 128 word (4byte) at address offset 0x11C, read-write */
   uint32_t dmaincr_0;
   /** NOT SHADOWED at address offset 0x120, read-write */
   uint32_t dmadestmeas_0;
   /** Status of the OfdmDemux Controller. at address offset 0x124, read-only */
   uint32_t statusctrl_0;
   /** NOT SHADOWED. at address offset 0x128, read-write */
   uint32_t commonparameters_1;
   /** Control register - h/w reads when block is idle and EnableSf  is set. at address offset 0x12C, read-write */
   uint32_t ctrl_1;
   /** Select phase table entry per symbol - 2LSB -> Symbol 0.   Own vector entry for single write for all sym when PhaseTable is updated. at address offset 0x130, read-write */
   uint32_t phasetableselect_1;
   /** Select dc correction entry per symbol - 2LSB -> Symbol 0. Own vector entry for single write for all sym when dc correction  is updated. at address offset 0x134, read-write */
   uint32_t dcselect_1;
   /** Input data address offset, +ve = timing advance at address offset 0x138, read-write */
   uint32_t fftshiftin_1;
   /** Two banks of FFT output (avoiding central DC). Both banks written consecutively to DMA, BankA then BankB. LoB must be higher HiA else BankB disabled. at address offset 0x13C, read-write */
   uint32_t fftout_1;
   /** SHADOWED at address offset 0x140, read-write */
   uint32_t dmadest_1;
   /** NOT SHADOWED  128 x 16b iq = 512 byte = 128 word (4byte) at address offset 0x144, read-write */
   uint32_t dmaincr_1;
   /** NOT SHADOWED at address offset 0x148, read-write */
   uint32_t dmadestmeas_1;
   /** Status of the OfdmDemux Controller. at address offset 0x14C, read-only */
   uint32_t statusctrl_1;
   /** Status of the OfdmDemux Datapath at address offset 0x150, read-only */
   uint32_t statusofdmdemux;
   /** Reserved space */
   uint8_t fill2[172];
   /** Symbol offset relative to current SFB Read pointer at address offset 0x200, read-write */
   uint32_t addroffset_0;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x204, read-write */
   uint32_t addroffset_1;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x208, read-write */
   uint32_t addroffset_2;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x20C, read-write */
   uint32_t addroffset_3;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x210, read-write */
   uint32_t addroffset_4;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x214, read-write */
   uint32_t addroffset_5;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x218, read-write */
   uint32_t addroffset_6;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x21C, read-write */
   uint32_t addroffset_7;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x220, read-write */
   uint32_t addroffset_8;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x224, read-write */
   uint32_t addroffset_9;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x228, read-write */
   uint32_t addroffset_10;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x22C, read-write */
   uint32_t addroffset_11;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x230, read-write */
   uint32_t addroffset_12;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x234, read-write */
   uint32_t addroffset_13;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x238, read-write */
   uint32_t addroffset_14;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x23C, read-write */
   uint32_t addroffset_15;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x240, read-write */
   uint32_t addroffset_16;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x244, read-write */
   uint32_t addroffset_17;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x248, read-write */
   uint32_t addroffset_18;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x24C, read-write */
   uint32_t addroffset_19;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x250, read-write */
   uint32_t addroffset_20;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x254, read-write */
   uint32_t addroffset_21;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x258, read-write */
   uint32_t addroffset_22;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x25C, read-write */
   uint32_t addroffset_23;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x260, read-write */
   uint32_t addroffset_24;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x264, read-write */
   uint32_t addroffset_25;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x268, read-write */
   uint32_t addroffset_26;
   /** Symbol offset relative to current SFB Read pointer at address offset 0x26C, read-write */
   uint32_t addroffset_27;
   /** Reserved space */
   uint8_t fill3[144];
   /** Latched status bits: set by h/w;  cleared by s/w. NOT SHADOWED / ReadOnly at address offset 0x300, read-write */
   uint32_t eventstatus;
   /** Interrupt {0,1,2,3} are levels; Interrupt {3} also has a single clock pulse. at address offset 0x304, read-write */
   uint32_t interruptmask_0;
   /** Write to EventStatus to clear. at address offset 0x308, read-only */
   uint32_t interruptstatus_0;
   /** Interrupt {0,1,2,3} are levels; Interrupt {3} also has a single clock pulse. at address offset 0x30C, read-write */
   uint32_t interruptmask_1;
   /** Write to EventStatus to clear. at address offset 0x310, read-only */
   uint32_t interruptstatus_1;
   /** Interrupt {0,1,2,3} are levels; Interrupt {3} also has a single clock pulse. at address offset 0x314, read-write */
   uint32_t interruptmask_2;
   /** Write to EventStatus to clear. at address offset 0x318, read-only */
   uint32_t interruptstatus_2;
   /** Interrupt {0,1,2,3} are levels; Interrupt {3} also has a single clock pulse. at address offset 0x31C, read-write */
   uint32_t interruptmask_3;
   /** Write to EventStatus to clear. at address offset 0x320, read-only */
   uint32_t interruptstatus_3;
   /**  at address offset 0x324, read-write */
   uint32_t hwobs;
   /** Reserved space */
   uint8_t fill4[3240];
   /** Peripheral ID4 Register at address offset 0xFD0, read-only */
   uint32_t peripheralid4;
   /** Peripheral ID5 Register at address offset 0xFD4, read-only */
   uint32_t peripheralid5;
   /** Peripheral ID6 Register at address offset 0xFD8, read-only */
   uint32_t peripheralid6;
   /** Peripheral ID7 Register at address offset 0xFDC, read-only */
   uint32_t peripheralid7;
   /** Peripheral ID0 Register at address offset 0xFE0, read-only */
   uint32_t peripheralid0;
   /** Peripheral ID1 Register at address offset 0xFE4, read-only */
   uint32_t peripheralid1;
   /** Peripheral ID2 Register at address offset 0xFE8, read-only */
   uint32_t peripheralid2;
   /** Peripheral ID3 Register at address offset 0xFEC, read-only */
   uint32_t peripheralid3;
   /** Primecell ID register 0 at address offset 0xFF0, read-only */
   uint32_t componentid0;
   /** Primecell ID register 1 at address offset 0xFF4, read-only */
   uint32_t componentid1;
   /** Primecell ID register 2 at address offset 0xFF8, read-only */
   uint32_t componentid2;
   /** Primecell ID register 3 at address offset 0xFFC, read-only */
   uint32_t componentid3;
};

/** bit field defines for km_ofdm_demux_s#phasetable_0 */
#define KM_OFDM_DEMUX_PHASETABLE_0_START_OFFSET 0
#define KM_OFDM_DEMUX_PHASETABLE_0_START_SIZE 16
#define KM_OFDM_DEMUX_PHASETABLE_0_DELTA_OFFSET 16
#define KM_OFDM_DEMUX_PHASETABLE_0_DELTA_SIZE 16

/** bit field defines for km_ofdm_demux_s#phasetable_1 */
#define KM_OFDM_DEMUX_PHASETABLE_1_START_OFFSET 0
#define KM_OFDM_DEMUX_PHASETABLE_1_START_SIZE 16
#define KM_OFDM_DEMUX_PHASETABLE_1_DELTA_OFFSET 16
#define KM_OFDM_DEMUX_PHASETABLE_1_DELTA_SIZE 16

/** bit field defines for km_ofdm_demux_s#phasetable_2 */
#define KM_OFDM_DEMUX_PHASETABLE_2_START_OFFSET 0
#define KM_OFDM_DEMUX_PHASETABLE_2_START_SIZE 16
#define KM_OFDM_DEMUX_PHASETABLE_2_DELTA_OFFSET 16
#define KM_OFDM_DEMUX_PHASETABLE_2_DELTA_SIZE 16

/** bit field defines for km_ofdm_demux_s#phasetable_3 */
#define KM_OFDM_DEMUX_PHASETABLE_3_START_OFFSET 0
#define KM_OFDM_DEMUX_PHASETABLE_3_START_SIZE 16
#define KM_OFDM_DEMUX_PHASETABLE_3_DELTA_OFFSET 16
#define KM_OFDM_DEMUX_PHASETABLE_3_DELTA_SIZE 16

/** bit field defines for km_ofdm_demux_s#dccorrection_0 */
#define KM_OFDM_DEMUX_DCCORRECTION_0_RE_OFFSET 0
#define KM_OFDM_DEMUX_DCCORRECTION_0_RE_SIZE 12
#define KM_OFDM_DEMUX_DCCORRECTION_0_IM_OFFSET 12
#define KM_OFDM_DEMUX_DCCORRECTION_0_IM_SIZE 12

/** bit field defines for km_ofdm_demux_s#dccorrection_1 */
#define KM_OFDM_DEMUX_DCCORRECTION_1_RE_OFFSET 0
#define KM_OFDM_DEMUX_DCCORRECTION_1_RE_SIZE 12
#define KM_OFDM_DEMUX_DCCORRECTION_1_IM_OFFSET 12
#define KM_OFDM_DEMUX_DCCORRECTION_1_IM_SIZE 12

/** bit field defines for km_ofdm_demux_s#dccorrection_2 */
#define KM_OFDM_DEMUX_DCCORRECTION_2_RE_OFFSET 0
#define KM_OFDM_DEMUX_DCCORRECTION_2_RE_SIZE 12
#define KM_OFDM_DEMUX_DCCORRECTION_2_IM_OFFSET 12
#define KM_OFDM_DEMUX_DCCORRECTION_2_IM_SIZE 12

/** bit field defines for km_ofdm_demux_s#dccorrection_3 */
#define KM_OFDM_DEMUX_DCCORRECTION_3_RE_OFFSET 0
#define KM_OFDM_DEMUX_DCCORRECTION_3_RE_SIZE 12
#define KM_OFDM_DEMUX_DCCORRECTION_3_IM_OFFSET 12
#define KM_OFDM_DEMUX_DCCORRECTION_3_IM_SIZE 12

/** bit field defines for km_ofdm_demux_s#commonparameters_0 */
#define KM_OFDM_DEMUX_COMMONPARAMETERS_0_SFLEN_OFFSET 0
#define KM_OFDM_DEMUX_COMMONPARAMETERS_0_SFLEN_SIZE 11
#define KM_OFDM_DEMUX_COMMONPARAMETERS_0_SYMLENLOG2_OFFSET 11
#define KM_OFDM_DEMUX_COMMONPARAMETERS_0_SYMLENLOG2_SIZE 3
#define KM_OFDM_DEMUX_COMMONPARAMETERS_0_IFFT_OFFSET 14
#define KM_OFDM_DEMUX_COMMONPARAMETERS_0_IFFT_SIZE 1
#define KM_OFDM_DEMUX_COMMONPARAMETERS_0_FFTBYPASS_OFFSET 15
#define KM_OFDM_DEMUX_COMMONPARAMETERS_0_FFTBYPASS_SIZE 1

/** bit field defines for km_ofdm_demux_s#ctrl_0 */
#define KM_OFDM_DEMUX_CTRL_0_STARTADDR_OFFSET 0
#define KM_OFDM_DEMUX_CTRL_0_STARTADDR_SIZE 16
#define KM_OFDM_DEMUX_CTRL_0_STARTADDRVAL_OFFSET 16
#define KM_OFDM_DEMUX_CTRL_0_STARTADDRVAL_SIZE 1
#define KM_OFDM_DEMUX_CTRL_0_NUMSYM_OFFSET 17
#define KM_OFDM_DEMUX_CTRL_0_NUMSYM_SIZE 4
#define KM_OFDM_DEMUX_CTRL_0_ENABLESF_OFFSET 21
#define KM_OFDM_DEMUX_CTRL_0_ENABLESF_SIZE 1
#define KM_OFDM_DEMUX_CTRL_0_TIMINGADJUST_OFFSET 22
#define KM_OFDM_DEMUX_CTRL_0_TIMINGADJUST_SIZE 5
#define KM_OFDM_DEMUX_CTRL_0_ABORT_OFFSET 27
#define KM_OFDM_DEMUX_CTRL_0_ABORT_SIZE 1
#define KM_OFDM_DEMUX_CTRL_0_MEASCONTEXT_OFFSET 28
#define KM_OFDM_DEMUX_CTRL_0_MEASCONTEXT_SIZE 3

/** bit field defines for km_ofdm_demux_s#phasetableselect_0 */
#define KM_OFDM_DEMUX_PHASETABLESELECT_0_NONE_OFFSET 0
#define KM_OFDM_DEMUX_PHASETABLESELECT_0_NONE_SIZE 28

/** bit field defines for km_ofdm_demux_s#dcselect_0 */
#define KM_OFDM_DEMUX_DCSELECT_0_NONE_OFFSET 0
#define KM_OFDM_DEMUX_DCSELECT_0_NONE_SIZE 28

/** bit field defines for km_ofdm_demux_s#fftshiftin_0 */
#define KM_OFDM_DEMUX_FFTSHIFTIN_0_NONE_OFFSET 0
#define KM_OFDM_DEMUX_FFTSHIFTIN_0_NONE_SIZE 7

/** bit field defines for km_ofdm_demux_s#fftout_0 */
#define KM_OFDM_DEMUX_FFTOUT_0_STARTADDRA_OFFSET 0
#define KM_OFDM_DEMUX_FFTOUT_0_STARTADDRA_SIZE 7
#define KM_OFDM_DEMUX_FFTOUT_0_LENGTHA_OFFSET 8
#define KM_OFDM_DEMUX_FFTOUT_0_LENGTHA_SIZE 8
#define KM_OFDM_DEMUX_FFTOUT_0_STARTADDRB_OFFSET 16
#define KM_OFDM_DEMUX_FFTOUT_0_STARTADDRB_SIZE 7
#define KM_OFDM_DEMUX_FFTOUT_0_LENGTHB_OFFSET 24
#define KM_OFDM_DEMUX_FFTOUT_0_LENGTHB_SIZE 8

/** bit field defines for km_ofdm_demux_s#dmadest_0 */
#define KM_OFDM_DEMUX_DMADEST_0_VALUE_OFFSET 6
#define KM_OFDM_DEMUX_DMADEST_0_VALUE_SIZE 26

/** bit field defines for km_ofdm_demux_s#dmaincr_0 */
#define KM_OFDM_DEMUX_DMAINCR_0_SYMOFFSETVECTORS_OFFSET 0
#define KM_OFDM_DEMUX_DMAINCR_0_SYMOFFSETVECTORS_SIZE 4

/** bit field defines for km_ofdm_demux_s#dmadestmeas_0 */
#define KM_OFDM_DEMUX_DMADESTMEAS_0_VALUE_OFFSET 2
#define KM_OFDM_DEMUX_DMADESTMEAS_0_VALUE_SIZE 30

/** bit field defines for km_ofdm_demux_s#statusctrl_0 */
#define KM_OFDM_DEMUX_STATUSCTRL_0_STATE_OFFSET 0
#define KM_OFDM_DEMUX_STATUSCTRL_0_STATE_SIZE 2
/** Ofdm processing taking place. */
#define KM_OFDM_DEMUX_STATUSCTRL_0_STATE_PROC_VALUE 3
/** Requesting data from the fbuf */
#define KM_OFDM_DEMUX_STATUSCTRL_0_STATE_REQ_VALUE 1
/** FBUF transfer taking place. */
#define KM_OFDM_DEMUX_STATUSCTRL_0_STATE_XFER_VALUE 2
/** Idle, waiting for next sub-frame configuration. */
#define KM_OFDM_DEMUX_STATUSCTRL_0_STATE_IDLE_VALUE 0
#define KM_OFDM_DEMUX_STATUSCTRL_0_SYMBOL_OFFSET 2
#define KM_OFDM_DEMUX_STATUSCTRL_0_SYMBOL_SIZE 4
#define KM_OFDM_DEMUX_STATUSCTRL_0_SFADDR_OFFSET 6
#define KM_OFDM_DEMUX_STATUSCTRL_0_SFADDR_SIZE 16

/** bit field defines for km_ofdm_demux_s#commonparameters_1 */
#define KM_OFDM_DEMUX_COMMONPARAMETERS_1_SFLEN_OFFSET 0
#define KM_OFDM_DEMUX_COMMONPARAMETERS_1_SFLEN_SIZE 11
#define KM_OFDM_DEMUX_COMMONPARAMETERS_1_SYMLENLOG2_OFFSET 11
#define KM_OFDM_DEMUX_COMMONPARAMETERS_1_SYMLENLOG2_SIZE 3
#define KM_OFDM_DEMUX_COMMONPARAMETERS_1_IFFT_OFFSET 14
#define KM_OFDM_DEMUX_COMMONPARAMETERS_1_IFFT_SIZE 1
#define KM_OFDM_DEMUX_COMMONPARAMETERS_1_FFTBYPASS_OFFSET 15
#define KM_OFDM_DEMUX_COMMONPARAMETERS_1_FFTBYPASS_SIZE 1

/** bit field defines for km_ofdm_demux_s#ctrl_1 */
#define KM_OFDM_DEMUX_CTRL_1_STARTADDR_OFFSET 0
#define KM_OFDM_DEMUX_CTRL_1_STARTADDR_SIZE 16
#define KM_OFDM_DEMUX_CTRL_1_STARTADDRVAL_OFFSET 16
#define KM_OFDM_DEMUX_CTRL_1_STARTADDRVAL_SIZE 1
#define KM_OFDM_DEMUX_CTRL_1_NUMSYM_OFFSET 17
#define KM_OFDM_DEMUX_CTRL_1_NUMSYM_SIZE 4
#define KM_OFDM_DEMUX_CTRL_1_ENABLESF_OFFSET 21
#define KM_OFDM_DEMUX_CTRL_1_ENABLESF_SIZE 1
#define KM_OFDM_DEMUX_CTRL_1_TIMINGADJUST_OFFSET 22
#define KM_OFDM_DEMUX_CTRL_1_TIMINGADJUST_SIZE 5
#define KM_OFDM_DEMUX_CTRL_1_ABORT_OFFSET 27
#define KM_OFDM_DEMUX_CTRL_1_ABORT_SIZE 1
#define KM_OFDM_DEMUX_CTRL_1_MEASCONTEXT_OFFSET 28
#define KM_OFDM_DEMUX_CTRL_1_MEASCONTEXT_SIZE 3

/** bit field defines for km_ofdm_demux_s#phasetableselect_1 */
#define KM_OFDM_DEMUX_PHASETABLESELECT_1_NONE_OFFSET 0
#define KM_OFDM_DEMUX_PHASETABLESELECT_1_NONE_SIZE 28

/** bit field defines for km_ofdm_demux_s#dcselect_1 */
#define KM_OFDM_DEMUX_DCSELECT_1_NONE_OFFSET 0
#define KM_OFDM_DEMUX_DCSELECT_1_NONE_SIZE 28

/** bit field defines for km_ofdm_demux_s#fftshiftin_1 */
#define KM_OFDM_DEMUX_FFTSHIFTIN_1_NONE_OFFSET 0
#define KM_OFDM_DEMUX_FFTSHIFTIN_1_NONE_SIZE 7

/** bit field defines for km_ofdm_demux_s#fftout_1 */
#define KM_OFDM_DEMUX_FFTOUT_1_STARTADDRA_OFFSET 0
#define KM_OFDM_DEMUX_FFTOUT_1_STARTADDRA_SIZE 7
#define KM_OFDM_DEMUX_FFTOUT_1_LENGTHA_OFFSET 8
#define KM_OFDM_DEMUX_FFTOUT_1_LENGTHA_SIZE 8
#define KM_OFDM_DEMUX_FFTOUT_1_STARTADDRB_OFFSET 16
#define KM_OFDM_DEMUX_FFTOUT_1_STARTADDRB_SIZE 7
#define KM_OFDM_DEMUX_FFTOUT_1_LENGTHB_OFFSET 24
#define KM_OFDM_DEMUX_FFTOUT_1_LENGTHB_SIZE 8

/** bit field defines for km_ofdm_demux_s#dmadest_1 */
#define KM_OFDM_DEMUX_DMADEST_1_VALUE_OFFSET 6
#define KM_OFDM_DEMUX_DMADEST_1_VALUE_SIZE 26

/** bit field defines for km_ofdm_demux_s#dmaincr_1 */
#define KM_OFDM_DEMUX_DMAINCR_1_SYMOFFSETVECTORS_OFFSET 0
#define KM_OFDM_DEMUX_DMAINCR_1_SYMOFFSETVECTORS_SIZE 4

/** bit field defines for km_ofdm_demux_s#dmadestmeas_1 */
#define KM_OFDM_DEMUX_DMADESTMEAS_1_VALUE_OFFSET 2
#define KM_OFDM_DEMUX_DMADESTMEAS_1_VALUE_SIZE 30

/** bit field defines for km_ofdm_demux_s#statusctrl_1 */
#define KM_OFDM_DEMUX_STATUSCTRL_1_STATE_OFFSET 0
#define KM_OFDM_DEMUX_STATUSCTRL_1_STATE_SIZE 2
/** Idle, waiting for next sub-frame configuration. */
#define KM_OFDM_DEMUX_STATUSCTRL_1_STATE_IDLE_VALUE 0
/** Requesting data from the fbuf */
#define KM_OFDM_DEMUX_STATUSCTRL_1_STATE_REQ_VALUE 1
/** FBUF transfer taking place. */
#define KM_OFDM_DEMUX_STATUSCTRL_1_STATE_XFER_VALUE 2
/** Ofdm processing taking place. */
#define KM_OFDM_DEMUX_STATUSCTRL_1_STATE_PROC_VALUE 3
#define KM_OFDM_DEMUX_STATUSCTRL_1_SYMBOL_OFFSET 2
#define KM_OFDM_DEMUX_STATUSCTRL_1_SYMBOL_SIZE 4
#define KM_OFDM_DEMUX_STATUSCTRL_1_SFADDR_OFFSET 6
#define KM_OFDM_DEMUX_STATUSCTRL_1_SFADDR_SIZE 16

/** bit field defines for km_ofdm_demux_s#statusofdmdemux */
#define KM_OFDM_DEMUX_STATUSOFDMDEMUX_STATE_OFFSET 0
#define KM_OFDM_DEMUX_STATUSOFDMDEMUX_STATE_SIZE 2
/** performing fft */
#define KM_OFDM_DEMUX_STATUSOFDMDEMUX_STATE_FFT_VALUE 2
/** idle, waiting for next symbol to process */
#define KM_OFDM_DEMUX_STATUSOFDMDEMUX_STATE_IDLE_VALUE 0
/** xfer: from fbuf to local mem */
#define KM_OFDM_DEMUX_STATUSOFDMDEMUX_STATE_READ_VALUE 1
/** xfer: from local mem to dma interface */
#define KM_OFDM_DEMUX_STATUSOFDMDEMUX_STATE_WRITE_VALUE 3
#define KM_OFDM_DEMUX_STATUSOFDMDEMUX_STAGE_OFFSET 2
#define KM_OFDM_DEMUX_STATUSOFDMDEMUX_STAGE_SIZE 2

/** bit field defines for km_ofdm_demux_s#addroffset_0 */
#define KM_OFDM_DEMUX_ADDROFFSET_0_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_0_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_1 */
#define KM_OFDM_DEMUX_ADDROFFSET_1_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_1_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_2 */
#define KM_OFDM_DEMUX_ADDROFFSET_2_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_2_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_3 */
#define KM_OFDM_DEMUX_ADDROFFSET_3_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_3_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_4 */
#define KM_OFDM_DEMUX_ADDROFFSET_4_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_4_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_5 */
#define KM_OFDM_DEMUX_ADDROFFSET_5_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_5_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_6 */
#define KM_OFDM_DEMUX_ADDROFFSET_6_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_6_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_7 */
#define KM_OFDM_DEMUX_ADDROFFSET_7_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_7_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_8 */
#define KM_OFDM_DEMUX_ADDROFFSET_8_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_8_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_9 */
#define KM_OFDM_DEMUX_ADDROFFSET_9_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_9_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_10 */
#define KM_OFDM_DEMUX_ADDROFFSET_10_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_10_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_11 */
#define KM_OFDM_DEMUX_ADDROFFSET_11_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_11_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_12 */
#define KM_OFDM_DEMUX_ADDROFFSET_12_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_12_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_13 */
#define KM_OFDM_DEMUX_ADDROFFSET_13_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_13_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_14 */
#define KM_OFDM_DEMUX_ADDROFFSET_14_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_14_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_15 */
#define KM_OFDM_DEMUX_ADDROFFSET_15_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_15_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_16 */
#define KM_OFDM_DEMUX_ADDROFFSET_16_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_16_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_17 */
#define KM_OFDM_DEMUX_ADDROFFSET_17_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_17_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_18 */
#define KM_OFDM_DEMUX_ADDROFFSET_18_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_18_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_19 */
#define KM_OFDM_DEMUX_ADDROFFSET_19_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_19_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_20 */
#define KM_OFDM_DEMUX_ADDROFFSET_20_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_20_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_21 */
#define KM_OFDM_DEMUX_ADDROFFSET_21_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_21_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_22 */
#define KM_OFDM_DEMUX_ADDROFFSET_22_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_22_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_23 */
#define KM_OFDM_DEMUX_ADDROFFSET_23_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_23_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_24 */
#define KM_OFDM_DEMUX_ADDROFFSET_24_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_24_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_25 */
#define KM_OFDM_DEMUX_ADDROFFSET_25_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_25_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_26 */
#define KM_OFDM_DEMUX_ADDROFFSET_26_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_26_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#addroffset_27 */
#define KM_OFDM_DEMUX_ADDROFFSET_27_NONE_OFFSET 0
#define KM_OFDM_DEMUX_ADDROFFSET_27_NONE_SIZE 11

/** bit field defines for km_ofdm_demux_s#eventstatus */
#define KM_OFDM_DEMUX_EVENTSTATUS_NONE_OFFSET 0
#define KM_OFDM_DEMUX_EVENTSTATUS_NONE_SIZE 32

/** bit field defines for km_ofdm_demux_s#interruptmask_0 */
#define KM_OFDM_DEMUX_INTERRUPTMASK_0_NONE_OFFSET 0
#define KM_OFDM_DEMUX_INTERRUPTMASK_0_NONE_SIZE 32

/** bit field defines for km_ofdm_demux_s#interruptstatus_0 */
#define KM_OFDM_DEMUX_INTERRUPTSTATUS_0_NONE_OFFSET 0
#define KM_OFDM_DEMUX_INTERRUPTSTATUS_0_NONE_SIZE 32

/** bit field defines for km_ofdm_demux_s#interruptmask_1 */
#define KM_OFDM_DEMUX_INTERRUPTMASK_1_NONE_OFFSET 0
#define KM_OFDM_DEMUX_INTERRUPTMASK_1_NONE_SIZE 32

/** bit field defines for km_ofdm_demux_s#interruptstatus_1 */
#define KM_OFDM_DEMUX_INTERRUPTSTATUS_1_NONE_OFFSET 0
#define KM_OFDM_DEMUX_INTERRUPTSTATUS_1_NONE_SIZE 32

/** bit field defines for km_ofdm_demux_s#interruptmask_2 */
#define KM_OFDM_DEMUX_INTERRUPTMASK_2_NONE_OFFSET 0
#define KM_OFDM_DEMUX_INTERRUPTMASK_2_NONE_SIZE 32

/** bit field defines for km_ofdm_demux_s#interruptstatus_2 */
#define KM_OFDM_DEMUX_INTERRUPTSTATUS_2_NONE_OFFSET 0
#define KM_OFDM_DEMUX_INTERRUPTSTATUS_2_NONE_SIZE 32

/** bit field defines for km_ofdm_demux_s#interruptmask_3 */
#define KM_OFDM_DEMUX_INTERRUPTMASK_3_NONE_OFFSET 0
#define KM_OFDM_DEMUX_INTERRUPTMASK_3_NONE_SIZE 32

/** bit field defines for km_ofdm_demux_s#interruptstatus_3 */
#define KM_OFDM_DEMUX_INTERRUPTSTATUS_3_NONE_OFFSET 0
#define KM_OFDM_DEMUX_INTERRUPTSTATUS_3_NONE_SIZE 32

/** bit field defines for km_ofdm_demux_s#hwobs */
#define KM_OFDM_DEMUX_HWOBS_SEL_OFFSET 0
#define KM_OFDM_DEMUX_HWOBS_SEL_SIZE 3

/** bit field defines for km_ofdm_demux_s#peripheralid4 */
#define KM_OFDM_DEMUX_PERIPHERALID4_NONE_OFFSET 0
#define KM_OFDM_DEMUX_PERIPHERALID4_NONE_SIZE 8

/** bit field defines for km_ofdm_demux_s#peripheralid5 */
#define KM_OFDM_DEMUX_PERIPHERALID5_NONE_OFFSET 0
#define KM_OFDM_DEMUX_PERIPHERALID5_NONE_SIZE 8

/** bit field defines for km_ofdm_demux_s#peripheralid6 */
#define KM_OFDM_DEMUX_PERIPHERALID6_NONE_OFFSET 0
#define KM_OFDM_DEMUX_PERIPHERALID6_NONE_SIZE 8

/** bit field defines for km_ofdm_demux_s#peripheralid7 */
#define KM_OFDM_DEMUX_PERIPHERALID7_NONE_OFFSET 0
#define KM_OFDM_DEMUX_PERIPHERALID7_NONE_SIZE 8

/** bit field defines for km_ofdm_demux_s#peripheralid0 */
#define KM_OFDM_DEMUX_PERIPHERALID0_NONE_OFFSET 0
#define KM_OFDM_DEMUX_PERIPHERALID0_NONE_SIZE 8

/** bit field defines for km_ofdm_demux_s#peripheralid1 */
#define KM_OFDM_DEMUX_PERIPHERALID1_NONE_OFFSET 0
#define KM_OFDM_DEMUX_PERIPHERALID1_NONE_SIZE 8

/** bit field defines for km_ofdm_demux_s#peripheralid2 */
#define KM_OFDM_DEMUX_PERIPHERALID2_NONE_OFFSET 0
#define KM_OFDM_DEMUX_PERIPHERALID2_NONE_SIZE 8

/** bit field defines for km_ofdm_demux_s#peripheralid3 */
#define KM_OFDM_DEMUX_PERIPHERALID3_NONE_OFFSET 0
#define KM_OFDM_DEMUX_PERIPHERALID3_NONE_SIZE 8

/** bit field defines for km_ofdm_demux_s#componentid0 */
#define KM_OFDM_DEMUX_COMPONENTID0_NONE_OFFSET 0
#define KM_OFDM_DEMUX_COMPONENTID0_NONE_SIZE 8

/** bit field defines for km_ofdm_demux_s#componentid1 */
#define KM_OFDM_DEMUX_COMPONENTID1_NONE_OFFSET 0
#define KM_OFDM_DEMUX_COMPONENTID1_NONE_SIZE 8

/** bit field defines for km_ofdm_demux_s#componentid2 */
#define KM_OFDM_DEMUX_COMPONENTID2_NONE_OFFSET 0
#define KM_OFDM_DEMUX_COMPONENTID2_NONE_SIZE 8

/** bit field defines for km_ofdm_demux_s#componentid3 */
#define KM_OFDM_DEMUX_COMPONENTID3_NONE_OFFSET 0
#define KM_OFDM_DEMUX_COMPONENTID3_NONE_SIZE 8

/* EOF km_ofdm_demux.h */
#endif
