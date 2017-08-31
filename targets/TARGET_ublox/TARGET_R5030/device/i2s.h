#ifndef INCLUDED_I2S
#define INCLUDED_I2S
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** i2s HAL Spreadsheet version number */
#define I2S_HAL_VERSION 1

/**  cdn_i2ssc_addr_map
*/
struct i2s_s {
   /** I2S control register. After end of reset bit sfr_rst triggers to 1 and bit fifo_rst triggers to 1. Therefore register value after end of reset is 000001B8 at address offset 0x000, read-write */
   uint32_t i2s_ctrl;
   /** I2S Full-Duplex Mode Control register. After end of reset bit fifo_rst triggers to 1. Therefore register value after end of reset is 00000010 at address offset 0x004, read-write */
   uint32_t i2s_ctrl_fdx;
   /** Transceiver Sample Resolution at address offset 0x008, read-write */
   uint32_t i2s_sres;
   /** Full Duplex Mode Receive Samples Resolution at address offset 0x00C, read-write */
   uint32_t i2s_sres_fdr;
   /** Transceiver Sample Rate at address offset 0x010, read-write */
   uint32_t i2s_srate;
   /** I2S-SC Status Flags Register at address offset 0x014, read-only */
   uint32_t i2s_stat;
   /** FIFO Using Level Register (read only) at address offset 0x018, read-only */
   uint32_t fifo_level;
   /** FIFO Almost Empty Level at address offset 0x01C, read-write */
   uint32_t fifo_aempty;
   /** FIFO Almost Empty Level at address offset 0x020, read-write */
   uint32_t fifo_afull;
   /** Full-Duplex Mode Receiver FIFO Level Register (read only) at address offset 0x024, read-only */
   uint32_t fifo_level_fdr;
   /** Full-Duplex Mode Receiver FIFO Almost Empty Level at address offset 0x028, read-write */
   uint32_t fifo_aempty_fdr;
   /** Full-Duplex Mode Receiver FIFO Almost Full Level at address offset 0x02C, read-write */
   uint32_t fifo_afull_fdr;
   /** Time Division Multiplexing Control Register at address offset 0x030, read-write */
   uint32_t tdm_ctrl;
   /** Time Division Multiplexing Full-Duplex Mode Channels Direction Register at address offset 0x034, read-write */
   uint32_t tdm_fd_dir;
};

/** bit field defines for i2s_s#i2s_ctrl */
#define I2S_I2S_CTRL_I2S_EN_OFFSET 0
#define I2S_I2S_CTRL_I2S_EN_SIZE 1
#define I2S_I2S_CTRL_DIR_CFG_OFFSET 1
#define I2S_I2S_CTRL_DIR_CFG_SIZE 1
#define I2S_I2S_CTRL_MS_CFG_OFFSET 2
#define I2S_I2S_CTRL_MS_CFG_SIZE 1
#define I2S_I2S_CTRL_SFR_RST_OFFSET 3
#define I2S_I2S_CTRL_SFR_RST_SIZE 1
#define I2S_I2S_CTRL_FIFO_RST_OFFSET 4
#define I2S_I2S_CTRL_FIFO_RST_SIZE 1
#define I2S_I2S_CTRL_CHN_WIDTH_OFFSET 5
#define I2S_I2S_CTRL_CHN_WIDTH_SIZE 3
#define I2S_I2S_CTRL_WS_MODE_OFFSET 8
#define I2S_I2S_CTRL_WS_MODE_SIZE 4
#define I2S_I2S_CTRL_MONO_MODE_OFFSET 12
#define I2S_I2S_CTRL_MONO_MODE_SIZE 1
#define I2S_I2S_CTRL_AUDIO_MODE_OFFSET 13
#define I2S_I2S_CTRL_AUDIO_MODE_SIZE 1
#define I2S_I2S_CTRL_SCK_POLAR_OFFSET 14
#define I2S_I2S_CTRL_SCK_POLAR_SIZE 1
#define I2S_I2S_CTRL_WS_POLAR_OFFSET 15
#define I2S_I2S_CTRL_WS_POLAR_SIZE 1
#define I2S_I2S_CTRL_DATA_WS_DEL_OFFSET 16
#define I2S_I2S_CTRL_DATA_WS_DEL_SIZE 5
#define I2S_I2S_CTRL_DATA_ALIGN_OFFSET 21
#define I2S_I2S_CTRL_DATA_ALIGN_SIZE 1
#define I2S_I2S_CTRL_DATA_ORDER_OFFSET 22
#define I2S_I2S_CTRL_DATA_ORDER_SIZE 1
#define I2S_I2S_CTRL_HOST_DATA_ALIGN_OFFSET 23
#define I2S_I2S_CTRL_HOST_DATA_ALIGN_SIZE 1
#define I2S_I2S_CTRL_I2S_STB_OFFSET 24
#define I2S_I2S_CTRL_I2S_STB_SIZE 1
#define I2S_I2S_CTRL_INTREQ_MASK_OFFSET 25
#define I2S_I2S_CTRL_INTREQ_MASK_SIZE 1
#define I2S_I2S_CTRL_I2S_MASK_OFFSET 26
#define I2S_I2S_CTRL_I2S_MASK_SIZE 1
#define I2S_I2S_CTRL_FIFO_EMPTY_MASK_OFFSET 27
#define I2S_I2S_CTRL_FIFO_EMPTY_MASK_SIZE 1
#define I2S_I2S_CTRL_FIFO_AEMPTY_MASK_OFFSET 28
#define I2S_I2S_CTRL_FIFO_AEMPTY_MASK_SIZE 1
#define I2S_I2S_CTRL_FIFO_FULL_MASK_OFFSET 29
#define I2S_I2S_CTRL_FIFO_FULL_MASK_SIZE 1
#define I2S_I2S_CTRL_FIFO_AFULL_MASK_OFFSET 30
#define I2S_I2S_CTRL_FIFO_AFULL_MASK_SIZE 1
#define I2S_I2S_CTRL_LR_PACK_OFFSET 31
#define I2S_I2S_CTRL_LR_PACK_SIZE 1

/** bit field defines for i2s_s#i2s_ctrl_fdx */
#define I2S_I2S_CTRL_FDX_FULL_DUPLEX_OFFSET 0
#define I2S_I2S_CTRL_FDX_FULL_DUPLEX_SIZE 1
#define I2S_I2S_CTRL_FDX_I2S_FTX_EN_OFFSET 1
#define I2S_I2S_CTRL_FDX_I2S_FTX_EN_SIZE 1
#define I2S_I2S_CTRL_FDX_I2S_FRX_EN_OFFSET 2
#define I2S_I2S_CTRL_FDX_I2S_FRX_EN_SIZE 1
#define I2S_I2S_CTRL_FDX_FIFO_RST_OFFSET 4
#define I2S_I2S_CTRL_FDX_FIFO_RST_SIZE 1
#define I2S_I2S_CTRL_FDX_RI2S_MASK_OFFSET 26
#define I2S_I2S_CTRL_FDX_RI2S_MASK_SIZE 1
#define I2S_I2S_CTRL_FDX_RFIFO_EMPTY_MASK_OFFSET 27
#define I2S_I2S_CTRL_FDX_RFIFO_EMPTY_MASK_SIZE 1
#define I2S_I2S_CTRL_FDX_RFIFO_AEMPTY_MASK_OFFSET 28
#define I2S_I2S_CTRL_FDX_RFIFO_AEMPTY_MASK_SIZE 1
#define I2S_I2S_CTRL_FDX_RFIFO_FULL_MASK_OFFSET 29
#define I2S_I2S_CTRL_FDX_RFIFO_FULL_MASK_SIZE 1
#define I2S_I2S_CTRL_FDX_RFIFO_AFULL_MASK_OFFSET 30
#define I2S_I2S_CTRL_FDX_RFIFO_AFULL_MASK_SIZE 1

/** bit field defines for i2s_s#i2s_sres */
#define I2S_I2S_SRES_RESOLUTION_OFFSET 0
#define I2S_I2S_SRES_RESOLUTION_SIZE 5

/** bit field defines for i2s_s#i2s_sres_fdr */
#define I2S_I2S_SRES_FDR_RRESOLUTION_OFFSET 0
#define I2S_I2S_SRES_FDR_RRESOLUTION_SIZE 5

/** bit field defines for i2s_s#i2s_srate */
#define I2S_I2S_SRATE_SAMPLE_RATE_OFFSET 0
#define I2S_I2S_SRATE_SAMPLE_RATE_SIZE 20

/** bit field defines for i2s_s#i2s_stat */
#define I2S_I2S_STAT_TDATA_UNDERR_OFFSET 0
#define I2S_I2S_STAT_TDATA_UNDERR_SIZE 1
#define I2S_I2S_STAT_RDATA_OVERR_OFFSET 1
#define I2S_I2S_STAT_RDATA_OVERR_SIZE 1
#define I2S_I2S_STAT_FIFO_EMPTY_OFFSET 2
#define I2S_I2S_STAT_FIFO_EMPTY_SIZE 1
#define I2S_I2S_STAT_FIFO_AEMPTY_OFFSET 3
#define I2S_I2S_STAT_FIFO_AEMPTY_SIZE 1
#define I2S_I2S_STAT_FIFO_FULL_OFFSET 4
#define I2S_I2S_STAT_FIFO_FULL_SIZE 1
#define I2S_I2S_STAT_FIFO_AFULL_OFFSET 5
#define I2S_I2S_STAT_FIFO_AFULL_SIZE 1
#define I2S_I2S_STAT_RESERVED_OFFSET 6
#define I2S_I2S_STAT_RESERVED_SIZE 10
#define I2S_I2S_STAT_RFIFO_EMPTY_OFFSET 16
#define I2S_I2S_STAT_RFIFO_EMPTY_SIZE 1
#define I2S_I2S_STAT_RFIFO_AEMPTY_OFFSET 17
#define I2S_I2S_STAT_RFIFO_AEMPTY_SIZE 1
#define I2S_I2S_STAT_RFIFO_FULL_OFFSET 18
#define I2S_I2S_STAT_RFIFO_FULL_SIZE 1
#define I2S_I2S_STAT_RFIFO_AFULL_OFFSET 19
#define I2S_I2S_STAT_RFIFO_AFULL_SIZE 1

/** bit field defines for i2s_s#fifo_level */
#define I2S_FIFO_LEVEL_FIFO_LEVEL_OFFSET 0
#define I2S_FIFO_LEVEL_FIFO_LEVEL_SIZE 4

/** bit field defines for i2s_s#fifo_aempty */
#define I2S_FIFO_AEMPTY_AEMPTY_THRESHOLD_OFFSET 0
#define I2S_FIFO_AEMPTY_AEMPTY_THRESHOLD_SIZE 4

/** bit field defines for i2s_s#fifo_afull */
#define I2S_FIFO_AFULL_AFULL_THRESHOLD_OFFSET 0
#define I2S_FIFO_AFULL_AFULL_THRESHOLD_SIZE 4

/** bit field defines for i2s_s#fifo_level_fdr */
#define I2S_FIFO_LEVEL_FDR_FIFO_LEVEL_OFFSET 0
#define I2S_FIFO_LEVEL_FDR_FIFO_LEVEL_SIZE 4

/** bit field defines for i2s_s#fifo_aempty_fdr */
#define I2S_FIFO_AEMPTY_FDR_AEMPTY_THRESHOLD_OFFSET 0
#define I2S_FIFO_AEMPTY_FDR_AEMPTY_THRESHOLD_SIZE 4

/** bit field defines for i2s_s#fifo_afull_fdr */
#define I2S_FIFO_AFULL_FDR_AFULL_THRESHOLD_OFFSET 0
#define I2S_FIFO_AFULL_FDR_AFULL_THRESHOLD_SIZE 4

/** bit field defines for i2s_s#tdm_ctrl */
#define I2S_TDM_CTRL_TDM_EN_OFFSET 0
#define I2S_TDM_CTRL_TDM_EN_SIZE 1
#define I2S_TDM_CTRL_CHN_NO_OFFSET 1
#define I2S_TDM_CTRL_CHN_NO_SIZE 4
#define I2S_TDM_CTRL_CH0_EN_OFFSET 16
#define I2S_TDM_CTRL_CH0_EN_SIZE 1
#define I2S_TDM_CTRL_CH1_EN_OFFSET 17
#define I2S_TDM_CTRL_CH1_EN_SIZE 1
#define I2S_TDM_CTRL_CH2_EN_OFFSET 18
#define I2S_TDM_CTRL_CH2_EN_SIZE 1
#define I2S_TDM_CTRL_CH3_EN_OFFSET 19
#define I2S_TDM_CTRL_CH3_EN_SIZE 1
#define I2S_TDM_CTRL_CH4_EN_OFFSET 20
#define I2S_TDM_CTRL_CH4_EN_SIZE 1
#define I2S_TDM_CTRL_CH5_EN_OFFSET 21
#define I2S_TDM_CTRL_CH5_EN_SIZE 1
#define I2S_TDM_CTRL_CH6_EN_OFFSET 22
#define I2S_TDM_CTRL_CH6_EN_SIZE 1
#define I2S_TDM_CTRL_CH7_EN_OFFSET 23
#define I2S_TDM_CTRL_CH7_EN_SIZE 1
#define I2S_TDM_CTRL_CH8_EN_OFFSET 24
#define I2S_TDM_CTRL_CH8_EN_SIZE 1
#define I2S_TDM_CTRL_CH9_EN_OFFSET 25
#define I2S_TDM_CTRL_CH9_EN_SIZE 1
#define I2S_TDM_CTRL_CH10_EN_OFFSET 26
#define I2S_TDM_CTRL_CH10_EN_SIZE 1
#define I2S_TDM_CTRL_CH11_EN_OFFSET 27
#define I2S_TDM_CTRL_CH11_EN_SIZE 1
#define I2S_TDM_CTRL_CH12_EN_OFFSET 28
#define I2S_TDM_CTRL_CH12_EN_SIZE 1
#define I2S_TDM_CTRL_CH13_EN_OFFSET 29
#define I2S_TDM_CTRL_CH13_EN_SIZE 1
#define I2S_TDM_CTRL_CH14_EN_OFFSET 30
#define I2S_TDM_CTRL_CH14_EN_SIZE 1
#define I2S_TDM_CTRL_CH15_EN_OFFSET 31
#define I2S_TDM_CTRL_CH15_EN_SIZE 1

/** bit field defines for i2s_s#tdm_fd_dir */
#define I2S_TDM_FD_DIR_CH0_TXEN_OFFSET 0
#define I2S_TDM_FD_DIR_CH0_TXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH1_TXEN_OFFSET 1
#define I2S_TDM_FD_DIR_CH1_TXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH2_TXEN_OFFSET 2
#define I2S_TDM_FD_DIR_CH2_TXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH3_TXEN_OFFSET 3
#define I2S_TDM_FD_DIR_CH3_TXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH4_TXEN_OFFSET 4
#define I2S_TDM_FD_DIR_CH4_TXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH5_TXEN_OFFSET 5
#define I2S_TDM_FD_DIR_CH5_TXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH6_TXEN_OFFSET 6
#define I2S_TDM_FD_DIR_CH6_TXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH7_TXEN_OFFSET 7
#define I2S_TDM_FD_DIR_CH7_TXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH8_TXEN_OFFSET 8
#define I2S_TDM_FD_DIR_CH8_TXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH9_TXEN_OFFSET 9
#define I2S_TDM_FD_DIR_CH9_TXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH10_TXEN_OFFSET 10
#define I2S_TDM_FD_DIR_CH10_TXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH11_TXEN_OFFSET 11
#define I2S_TDM_FD_DIR_CH11_TXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH12_TXEN_OFFSET 12
#define I2S_TDM_FD_DIR_CH12_TXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH13_TXEN_OFFSET 13
#define I2S_TDM_FD_DIR_CH13_TXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH14_TXEN_OFFSET 14
#define I2S_TDM_FD_DIR_CH14_TXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH15_TXEN_OFFSET 15
#define I2S_TDM_FD_DIR_CH15_TXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH0_RXEN_OFFSET 16
#define I2S_TDM_FD_DIR_CH0_RXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH1_RXEN_OFFSET 17
#define I2S_TDM_FD_DIR_CH1_RXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH2_RXEN_OFFSET 18
#define I2S_TDM_FD_DIR_CH2_RXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH3_RXEN_OFFSET 19
#define I2S_TDM_FD_DIR_CH3_RXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH4_RXEN_OFFSET 20
#define I2S_TDM_FD_DIR_CH4_RXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH5_RXEN_OFFSET 21
#define I2S_TDM_FD_DIR_CH5_RXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH6_RXEN_OFFSET 22
#define I2S_TDM_FD_DIR_CH6_RXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH7_RXEN_OFFSET 23
#define I2S_TDM_FD_DIR_CH7_RXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH8_RXEN_OFFSET 24
#define I2S_TDM_FD_DIR_CH8_RXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH9_RXEN_OFFSET 25
#define I2S_TDM_FD_DIR_CH9_RXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH10_RXEN_OFFSET 26
#define I2S_TDM_FD_DIR_CH10_RXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH11_RXEN_OFFSET 27
#define I2S_TDM_FD_DIR_CH11_RXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH12_RXEN_OFFSET 28
#define I2S_TDM_FD_DIR_CH12_RXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH13_RXEN_OFFSET 29
#define I2S_TDM_FD_DIR_CH13_RXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH14_RXEN_OFFSET 30
#define I2S_TDM_FD_DIR_CH14_RXEN_SIZE 1
#define I2S_TDM_FD_DIR_CH15_RXEN_OFFSET 31
#define I2S_TDM_FD_DIR_CH15_RXEN_SIZE 1

/* EOF i2s.h */
#endif
