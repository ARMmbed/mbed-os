#ifndef INCLUDED_IC_RX_SCRATCH_RAM
#define INCLUDED_IC_RX_SCRATCH_RAM
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** ic_rx_scratch_ram HAL Spreadsheet version number */
#define IC_RX_SCRATCH_RAM_HAL_VERSION 4

/** Rx ScratchPad Control Registers
*/
struct ic_rx_scratch_ram_s {
   /** Scratch Rx Enable Register at address offset 0x000, read-write */
   uint32_t scrrxen;
   /**  at address offset 0x004, read-write */
   uint32_t scrrxnumsamples;
   /** Number of cycles to delay before the capture starts less one at address offset 0x008, read-write */
   uint32_t scrrxdelay;
   /** Source selection at address offset 0x00C, read-write */
   uint32_t scrrxsource;
   /** Interrupt Status Register. Set when capture is complete.  Write 1 to clear. at address offset 0x010, read-write */
   uint32_t scrintstat;
   /** Interrupt mask.  Set to enable interrupt output at address offset 0x014, read-write */
   uint32_t scrintmask;
};

/** bit field defines for ic_rx_scratch_ram_s#scrrxen */
#define IC_RX_SCRATCH_RAM_SCRRXEN_ENABLE_OFFSET 0
#define IC_RX_SCRATCH_RAM_SCRRXEN_ENABLE_SIZE 1
/** Disable Capture */
#define IC_RX_SCRATCH_RAM_SCRRXEN_ENABLE_DISABLE_VALUE 0
/** Enable Capture */
#define IC_RX_SCRATCH_RAM_SCRRXEN_ENABLE_ENABLE_VALUE 1
#define IC_RX_SCRATCH_RAM_SCRRXEN_MODE_OFFSET 1
#define IC_RX_SCRATCH_RAM_SCRRXEN_MODE_SIZE 2
/** Rolling buffer Capture with delayed Start */
#define IC_RX_SCRATCH_RAM_SCRRXEN_MODE_DELAY_R_VALUE 3
/** Single buffer Capture with delayed Start */
#define IC_RX_SCRATCH_RAM_SCRRXEN_MODE_DELAY_S_VALUE 2
/** Rolling buffer Capture */
#define IC_RX_SCRATCH_RAM_SCRRXEN_MODE_ROLLING_VALUE 1
/** Single buffer capture */
#define IC_RX_SCRATCH_RAM_SCRRXEN_MODE_SINGLE_VALUE 0
#define IC_RX_SCRATCH_RAM_SCRRXEN_DATA_OFFSET 3
#define IC_RX_SCRATCH_RAM_SCRRXEN_DATA_SIZE 1
/** LSB aligned with sign extend */
#define IC_RX_SCRATCH_RAM_SCRRXEN_DATA_DISABLE_VALUE 0
/** MSB aligned zero padded */
#define IC_RX_SCRATCH_RAM_SCRRXEN_DATA_ENABLE_VALUE 1
#define IC_RX_SCRATCH_RAM_SCRRXEN_CLKMODE_OFFSET 4
#define IC_RX_SCRATCH_RAM_SCRRXEN_CLKMODE_SIZE 2
/** AHB Clock Selected */
#define IC_RX_SCRATCH_RAM_SCRRXEN_CLKMODE_AHB_VALUE 1
/** DFE Clock Selected */
#define IC_RX_SCRATCH_RAM_SCRRXEN_CLKMODE_DFE_VALUE 2
/** No Clock Selected */
#define IC_RX_SCRATCH_RAM_SCRRXEN_CLKMODE_NONE_VALUE 0
#define IC_RX_SCRATCH_RAM_SCRRXEN_WAITSCHTRIG_OFFSET 6
#define IC_RX_SCRATCH_RAM_SCRRXEN_WAITSCHTRIG_SIZE 1
/** Dont wait for trigger. */
#define IC_RX_SCRATCH_RAM_SCRRXEN_WAITSCHTRIG_DISABLE_VALUE 0
/** Wait for trigger. */
#define IC_RX_SCRATCH_RAM_SCRRXEN_WAITSCHTRIG_ENABLE_VALUE 1

/** bit field defines for ic_rx_scratch_ram_s#scrrxnumsamples */
#define IC_RX_SCRATCH_RAM_SCRRXNUMSAMPLES_NUM_OFFSET 0
#define IC_RX_SCRATCH_RAM_SCRRXNUMSAMPLES_NUM_SIZE 12

/** bit field defines for ic_rx_scratch_ram_s#scrrxdelay */
#define IC_RX_SCRATCH_RAM_SCRRXDELAY_DELTA_OFFSET 0
#define IC_RX_SCRATCH_RAM_SCRRXDELAY_DELTA_SIZE 12

/** bit field defines for ic_rx_scratch_ram_s#scrrxsource */
#define IC_RX_SCRATCH_RAM_SCRRXSOURCE_IDX_OFFSET 0
#define IC_RX_SCRATCH_RAM_SCRRXSOURCE_IDX_SIZE 3

/** bit field defines for ic_rx_scratch_ram_s#scrintstat */
#define IC_RX_SCRATCH_RAM_SCRINTSTAT_NONE_OFFSET 0
#define IC_RX_SCRATCH_RAM_SCRINTSTAT_NONE_SIZE 1

/** bit field defines for ic_rx_scratch_ram_s#scrintmask */
#define IC_RX_SCRATCH_RAM_SCRINTMASK_NONE_OFFSET 0
#define IC_RX_SCRATCH_RAM_SCRINTMASK_NONE_SIZE 1

/* EOF ic_rx_scratch_ram.h */
#endif
