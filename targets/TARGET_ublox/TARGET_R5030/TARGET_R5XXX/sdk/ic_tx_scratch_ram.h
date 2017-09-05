#ifndef INCLUDED_IC_TX_SCRATCH_RAM
#define INCLUDED_IC_TX_SCRATCH_RAM
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** ic_tx_scratch_ram HAL Spreadsheet version number */
#define IC_TX_SCRATCH_RAM_HAL_VERSION 3

/** Tx ScratchPad Control Registers
*/
struct ic_tx_scratch_ram_s {
   /** SCR_TX_CON register at address offset 0x000, read-write */
   uint32_t scrtxcon;
};

/** bit field defines for ic_tx_scratch_ram_s#scrtxcon */
#define IC_TX_SCRATCH_RAM_SCRTXCON_SCRSRC_OFFSET 0
#define IC_TX_SCRATCH_RAM_SCRTXCON_SCRSRC_SIZE 1
/** Send data to Tx1 */
#define IC_TX_SCRATCH_RAM_SCRTXCON_SCRSRC_TX1_VALUE 0
/** Send data to Tx2 */
#define IC_TX_SCRATCH_RAM_SCRTXCON_SCRSRC_TX2_VALUE 1
#define IC_TX_SCRATCH_RAM_SCRTXCON_SCRTXEN_OFFSET 2
#define IC_TX_SCRATCH_RAM_SCRTXCON_SCRTXEN_SIZE 1
/** Disable Replay */
#define IC_TX_SCRATCH_RAM_SCRTXCON_SCRTXEN_DISABLE_VALUE 0
/** Enable Replay */
#define IC_TX_SCRATCH_RAM_SCRTXCON_SCRTXEN_ENABLE_VALUE 1
#define IC_TX_SCRATCH_RAM_SCRTXCON_SCRTXNUM_OFFSET 3
#define IC_TX_SCRATCH_RAM_SCRTXCON_SCRTXNUM_SIZE 12
#define IC_TX_SCRATCH_RAM_SCRTXCON_SCRTXDATAMODE_OFFSET 15
#define IC_TX_SCRATCH_RAM_SCRTXCON_SCRTXDATAMODE_SIZE 1
/** LSB aligned with sign extend */
#define IC_TX_SCRATCH_RAM_SCRTXCON_SCRTXDATAMODE_DISABLE_VALUE 0
/** MSB aligned zero padded */
#define IC_TX_SCRATCH_RAM_SCRTXCON_SCRTXDATAMODE_ENABLE_VALUE 1
#define IC_TX_SCRATCH_RAM_SCRTXCON_CLKMODE_OFFSET 16
#define IC_TX_SCRATCH_RAM_SCRTXCON_CLKMODE_SIZE 2
/** AHB Clock Selected */
#define IC_TX_SCRATCH_RAM_SCRTXCON_CLKMODE_AHB_VALUE 1
/** DFE Clock Selected */
#define IC_TX_SCRATCH_RAM_SCRTXCON_CLKMODE_DFE_VALUE 2
/** No Clock Selected */
#define IC_TX_SCRATCH_RAM_SCRTXCON_CLKMODE_NONE_VALUE 0
#define IC_TX_SCRATCH_RAM_SCRTXCON_WAITSCHTRIG_OFFSET 18
#define IC_TX_SCRATCH_RAM_SCRTXCON_WAITSCHTRIG_SIZE 1
/** Dont wait for trigger. */
#define IC_TX_SCRATCH_RAM_SCRTXCON_WAITSCHTRIG_DISABLE_VALUE 0
/** Wait for trigger. */
#define IC_TX_SCRATCH_RAM_SCRTXCON_WAITSCHTRIG_ENABLE_VALUE 1

/* EOF ic_tx_scratch_ram.h */
#endif
