#ifndef INCLUDED_VDMA_CR
#define INCLUDED_VDMA_CR
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** Number of Vector DMA Channels */
#define VDMA_NR_CHANNELS 1
#define VDMA_CR_PERIPHERALID4 0x07
#define VDMA_CR_PERIPHERALID5 0x00
#define VDMA_CR_PERIPHERALID6 0x00
#define VDMA_CR_PERIPHERALID7 0x00
#define VDMA_CR_PERIPHERALID0 0x60
#define VDMA_CR_PERIPHERALID1 0xDA
#define VDMA_CR_PERIPHERALID2 0x08
#define VDMA_CR_PERIPHERALID3 0x00
#define VDMA_CR_COMPONENTID0 0x0D
#define VDMA_CR_COMPONENTID1 0xF0
#define VDMA_CR_COMPONENTID2 0x05
#define VDMA_CR_COMPONENTID3 0xB1

/** Channel Running Registers
*/
struct vdma_cr_chr_s {
   /** Channel Running Source Address Register at address offset 0x000, read-only */
   uint32_t src;
   /** Channel Running Destination Address Register at address offset 0x004, read-only */
   uint32_t dst;
   /** Channel Running Size Register at address offset 0x008, read-only */
   uint32_t size;
   /** Channel Running Configuration Register at address offset 0x00C, read-only */
   uint32_t conf;
};

/** bit field defines for vdma_cr_chr_s#src */
#define VDMA_CR_CHR_SRC_SRC_OFFSET 0
#define VDMA_CR_CHR_SRC_SRC_SIZE 32

/** bit field defines for vdma_cr_chr_s#dst */
#define VDMA_CR_CHR_DST_DST_OFFSET 0
#define VDMA_CR_CHR_DST_DST_SIZE 32

/** bit field defines for vdma_cr_chr_s#size */
#define VDMA_CR_CHR_SIZE_SIZE_OFFSET 0
#define VDMA_CR_CHR_SIZE_SIZE_SIZE 32

/** bit field defines for vdma_cr_chr_s#conf */
#define VDMA_CR_CHR_CONF_SEV_O_ENABLE_OFFSET 0
#define VDMA_CR_CHR_CONF_SEV_O_ENABLE_SIZE 1
#define VDMA_CR_CHR_CONF_EVENTS_O_ENABLE_OFFSET 1
#define VDMA_CR_CHR_CONF_EVENTS_O_ENABLE_SIZE 1
#define VDMA_CR_CHR_CONF_DST_ADDR_SELECTOR_OFFSET 2
#define VDMA_CR_CHR_CONF_DST_ADDR_SELECTOR_SIZE 1
#define VDMA_CR_CHR_CONF_SRC_ADDR_SELECTOR_OFFSET 3
#define VDMA_CR_CHR_CONF_SRC_ADDR_SELECTOR_SIZE 1

/** Channel Pending Registers
*/
struct vdma_cr_chp_s {
   /** Channel Pending Source Address Register at address offset 0x000, read-write */
   uint32_t src;
   /** Channel Pending Destination Address Register at address offset 0x004, read-write */
   uint32_t dst;
   /** Channel Pending Size Register at address offset 0x008, read-write */
   uint32_t size;
   /** Channel Pending Configuration Register at address offset 0x00C, read-write */
   uint32_t conf;
};

/** bit field defines for vdma_cr_chp_s#src */
#define VDMA_CR_CHP_SRC_SRC_OFFSET 0
#define VDMA_CR_CHP_SRC_SRC_SIZE 32

/** bit field defines for vdma_cr_chp_s#dst */
#define VDMA_CR_CHP_DST_DST_OFFSET 0
#define VDMA_CR_CHP_DST_DST_SIZE 32

/** bit field defines for vdma_cr_chp_s#size */
#define VDMA_CR_CHP_SIZE_SIZE_OFFSET 0
#define VDMA_CR_CHP_SIZE_SIZE_SIZE 32

/** bit field defines for vdma_cr_chp_s#conf */
#define VDMA_CR_CHP_CONF_SEV_O_ENABLE_OFFSET 0
#define VDMA_CR_CHP_CONF_SEV_O_ENABLE_SIZE 1
#define VDMA_CR_CHP_CONF_EVENTS_O_ENABLE_OFFSET 1
#define VDMA_CR_CHP_CONF_EVENTS_O_ENABLE_SIZE 1
#define VDMA_CR_CHP_CONF_DST_ADDR_SELECTOR_OFFSET 2
#define VDMA_CR_CHP_CONF_DST_ADDR_SELECTOR_SIZE 1
#define VDMA_CR_CHP_CONF_SRC_ADDR_SELECTOR_OFFSET 3
#define VDMA_CR_CHP_CONF_SRC_ADDR_SELECTOR_SIZE 1

/** VDMA Control Registers.
The Vector DMA Controller (VDMA) is used for memory to memory data transfers within the VSP memory system. The VDMA can transfer data to/from any memory, either private or shared, in the VSP memory system. Example use cases for the VDMA are:
   - The instruction code for AR1DE tasks might be located in the shared memory. In this case, the instruction code must be copied to the AR1DEs private instruction memory before the task can be executed.
   - Data copy and move within the VSP memory system, e.g. from the shared memory to a private data memory or between the private data memory of one AR1DE to the private data memory of the other AR1DE.

*/
struct vdma_cr_s {
   /** Channel Pending Registers at address offset 0x000 size: 16 */
   struct vdma_cr_chp_s chp[VDMA_NR_CHANNELS];
   /** Reserved space */
   uint8_t fill0[240];
   /** Channel Running Registers at address offset 0x100 size: 16 */
   struct vdma_cr_chr_s chr[VDMA_NR_CHANNELS];
   /** Reserved space */
   uint8_t fill1[240];
   /** Channel Command Register at address offset 0x200, write-only */
   uint32_t cmd;
   /** Channel Status Register at address offset 0x204, read-only */
   uint32_t stat;
   /** Reserved space */
   uint8_t fill2[248];
   /** Clear Fault Status Register at address offset 0x300, write-only */
   uint32_t fltclear;
   /** Fault Status Register at address offset 0x304, read-only */
   uint32_t fltstatus;
   /** Raw Fault Status Register at address offset 0x308, read-only */
   uint32_t fltrawstatus;
   /** Fault Mask Clear at address offset 0x30C, write-only */
   uint32_t fltmaskclr;
   /** Fault Mask Set at address offset 0x310, write-only */
   uint32_t fltmaskset;
   /** Fault Mask Status at address offset 0x314, read-only */
   uint32_t fltmaskstatus;
   /** Reserved space */
   uint8_t fill3[3256];
   /** Peripheral ID4 Register at address offset 0xFD0, read-only constant 0x00000007 */
   uint32_t peripheralid4;
   /** Peripheral ID5 Register at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** Peripheral ID6 Register at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** Peripheral ID7 Register at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** Peripheral ID0 Register at address offset 0xFE0, read-only constant 0x00000060 */
   uint32_t peripheralid0;
   /** Peripheral ID1 Register at address offset 0xFE4, read-only constant 0x000000DA */
   uint32_t peripheralid1;
   /** Peripheral ID2 Register at address offset 0xFE8, read-only constant 0x00000008 */
   uint32_t peripheralid2;
   /** Peripheral ID3 Register at address offset 0xFEC, read-only constant 0x00000000 */
   uint32_t peripheralid3;
   /** Component ID0 Register at address offset 0xFF0, read-only constant 0x0000000D */
   uint32_t componentid0;
   /** Component ID1 Register at address offset 0xFF4, read-only constant 0x000000F0 */
   uint32_t componentid1;
   /** Component ID2 Register at address offset 0xFF8, read-only constant 0x00000005 */
   uint32_t componentid2;
   /** Component ID3 Register at address offset 0xFFC, read-only constant 0x000000B1 */
   uint32_t componentid3;
};

/** bit field defines for vdma_cr_s#cmd */
#define VDMA_CR_CMD_PENDING_SUSPEND_OFFSET 0
#define VDMA_CR_CMD_PENDING_SUSPEND_SIZE 1
#define VDMA_CR_CMD_PENDING_ABORT_OFFSET 1
#define VDMA_CR_CMD_PENDING_ABORT_SIZE 1
#define VDMA_CR_CMD_RUNNING_SUSPEND_OFFSET 2
#define VDMA_CR_CMD_RUNNING_SUSPEND_SIZE 1
#define VDMA_CR_CMD_RUNNING_ABORT_OFFSET 3
#define VDMA_CR_CMD_RUNNING_ABORT_SIZE 1

/** bit field defines for vdma_cr_s#stat */
#define VDMA_CR_STAT_PENDING_SUSPENDED_OFFSET 0
#define VDMA_CR_STAT_PENDING_SUSPENDED_SIZE 1
#define VDMA_CR_STAT_PENDING_VALID_OFFSET 1
#define VDMA_CR_STAT_PENDING_VALID_SIZE 1
#define VDMA_CR_STAT_RUNNING_SUSPENDED_OFFSET 2
#define VDMA_CR_STAT_RUNNING_SUSPENDED_SIZE 1
#define VDMA_CR_STAT_RUNNING_VALID_OFFSET 3
#define VDMA_CR_STAT_RUNNING_VALID_SIZE 1
#define VDMA_CR_STAT_RUNNING_ABORT_BUSY_OFFSET 4
#define VDMA_CR_STAT_RUNNING_ABORT_BUSY_SIZE 1
#define VDMA_CR_STAT_RUNNING_BUSY_OFFSET 16
#define VDMA_CR_STAT_RUNNING_BUSY_SIZE 1

/** bit field defines for vdma_cr_s#fltclear */
#define VDMA_CR_FLTCLEAR_READ_FAULT_CHANNEL0_OFFSET 0
#define VDMA_CR_FLTCLEAR_READ_FAULT_CHANNEL0_SIZE 1
#define VDMA_CR_FLTCLEAR_WRITE_FAULT_CHANNEL0_OFFSET 1
#define VDMA_CR_FLTCLEAR_WRITE_FAULT_CHANNEL0_SIZE 1

/** bit field defines for vdma_cr_s#fltstatus */
#define VDMA_CR_FLTSTATUS_READ_FAULT_CHANNEL0_OFFSET 0
#define VDMA_CR_FLTSTATUS_READ_FAULT_CHANNEL0_SIZE 1
#define VDMA_CR_FLTSTATUS_WRITE_FAULT_CHANNEL0_OFFSET 1
#define VDMA_CR_FLTSTATUS_WRITE_FAULT_CHANNEL0_SIZE 1

/** bit field defines for vdma_cr_s#fltrawstatus */
#define VDMA_CR_FLTRAWSTATUS_READ_FAULT_CHANNEL0_OFFSET 0
#define VDMA_CR_FLTRAWSTATUS_READ_FAULT_CHANNEL0_SIZE 1
#define VDMA_CR_FLTRAWSTATUS_WRITE_FAULT_CHANNEL0_OFFSET 1
#define VDMA_CR_FLTRAWSTATUS_WRITE_FAULT_CHANNEL0_SIZE 1

/** bit field defines for vdma_cr_s#fltmaskclr */
#define VDMA_CR_FLTMASKCLR_READ_FAULT_CHANNEL0_OFFSET 0
#define VDMA_CR_FLTMASKCLR_READ_FAULT_CHANNEL0_SIZE 1
#define VDMA_CR_FLTMASKCLR_WRITE_FAULT_CHANNEL0_OFFSET 1
#define VDMA_CR_FLTMASKCLR_WRITE_FAULT_CHANNEL0_SIZE 1

/** bit field defines for vdma_cr_s#fltmaskset */
#define VDMA_CR_FLTMASKSET_READ_FAULT_CHANNEL0_OFFSET 0
#define VDMA_CR_FLTMASKSET_READ_FAULT_CHANNEL0_SIZE 1
#define VDMA_CR_FLTMASKSET_WRITE_FAULT_CHANNEL0_OFFSET 1
#define VDMA_CR_FLTMASKSET_WRITE_FAULT_CHANNEL0_SIZE 1

/** bit field defines for vdma_cr_s#fltmaskstatus */
#define VDMA_CR_FLTMASKSTATUS_READ_FAULT_CHANNEL0_OFFSET 0
#define VDMA_CR_FLTMASKSTATUS_READ_FAULT_CHANNEL0_SIZE 1
#define VDMA_CR_FLTMASKSTATUS_WRITE_FAULT_CHANNEL0_OFFSET 1
#define VDMA_CR_FLTMASKSTATUS_WRITE_FAULT_CHANNEL0_SIZE 1

/** bit field defines for vdma_cr_s#peripheralid4 */
#define VDMA_CR_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define VDMA_CR_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for vdma_cr_s#peripheralid5 */
#define VDMA_CR_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define VDMA_CR_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for vdma_cr_s#peripheralid6 */
#define VDMA_CR_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define VDMA_CR_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for vdma_cr_s#peripheralid7 */
#define VDMA_CR_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define VDMA_CR_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for vdma_cr_s#peripheralid0 */
#define VDMA_CR_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define VDMA_CR_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for vdma_cr_s#peripheralid1 */
#define VDMA_CR_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define VDMA_CR_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for vdma_cr_s#peripheralid2 */
#define VDMA_CR_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define VDMA_CR_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for vdma_cr_s#peripheralid3 */
#define VDMA_CR_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define VDMA_CR_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for vdma_cr_s#componentid0 */
#define VDMA_CR_COMPONENTID0_COMPONENTID0_OFFSET 0
#define VDMA_CR_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for vdma_cr_s#componentid1 */
#define VDMA_CR_COMPONENTID1_COMPONENTID1_OFFSET 0
#define VDMA_CR_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for vdma_cr_s#componentid2 */
#define VDMA_CR_COMPONENTID2_COMPONENTID2_OFFSET 0
#define VDMA_CR_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for vdma_cr_s#componentid3 */
#define VDMA_CR_COMPONENTID3_COMPONENTID3_OFFSET 0
#define VDMA_CR_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF vdma_cr.h */
#endif
