#ifndef INCLUDED_RFDMA
#define INCLUDED_RFDMA
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** Number of RF Receive Channels */
#define RFIF_NR_RX_CHANNELS 1
/** Number of RF Transmit Channels */
#define RFIF_NR_TX_CHANNELS 1
#define RFDMA_PERIPHERALID4 0x07
#define RFDMA_PERIPHERALID5 0x00
#define RFDMA_PERIPHERALID6 0x00
#define RFDMA_PERIPHERALID7 0x00
#define RFDMA_PERIPHERALID0 0x62
#define RFDMA_PERIPHERALID1 0xDA
#define RFDMA_PERIPHERALID2 0x08
#define RFDMA_PERIPHERALID3 0x00
#define RFDMA_COMPONENTID0 0x0D
#define RFDMA_COMPONENTID1 0xF0
#define RFDMA_COMPONENTID2 0x05
#define RFDMA_COMPONENTID3 0xB1

/** Receive channel command registers
*/
struct rfdma_rxc_s {
   /** Channel command at address offset 0x000, write-only */
   uint32_t comd;
   /** Channel status at address offset 0x004, read-only */
   uint32_t stat;
   /** RF data shifter control at address offset 0x008, read-write */
   uint32_t conf;
   /** Reserved space \private at address offset 0x00C, read-only */
   uint32_t fill0;
};

/** bit field defines for rfdma_rxc_s#comd */
#define RFDMA_RXC_COMD_PENDING_SUSPEND_OFFSET 0
#define RFDMA_RXC_COMD_PENDING_SUSPEND_SIZE 1
#define RFDMA_RXC_COMD_PENDING_ABORT_OFFSET 1
#define RFDMA_RXC_COMD_PENDING_ABORT_SIZE 1
#define RFDMA_RXC_COMD_RUNNING_SUSPEND_OFFSET 2
#define RFDMA_RXC_COMD_RUNNING_SUSPEND_SIZE 1
#define RFDMA_RXC_COMD_RUNNING_ABORT_OFFSET 3
#define RFDMA_RXC_COMD_RUNNING_ABORT_SIZE 1

/** bit field defines for rfdma_rxc_s#stat */
#define RFDMA_RXC_STAT_PENDING_SUSPENDED_OFFSET 0
#define RFDMA_RXC_STAT_PENDING_SUSPENDED_SIZE 1
#define RFDMA_RXC_STAT_PENDING_VALID_OFFSET 1
#define RFDMA_RXC_STAT_PENDING_VALID_SIZE 1
#define RFDMA_RXC_STAT_RUNNING_SUSPEND_OFFSET 2
#define RFDMA_RXC_STAT_RUNNING_SUSPEND_SIZE 1
#define RFDMA_RXC_STAT_RUNNING_VALID_OFFSET 3
#define RFDMA_RXC_STAT_RUNNING_VALID_SIZE 1
#define RFDMA_RXC_STAT_RUNNING_ABORT_BUSY_OFFSET 4
#define RFDMA_RXC_STAT_RUNNING_ABORT_BUSY_SIZE 1
#define RFDMA_RXC_STAT_FIFO_EMPTY_OFFSET 5
#define RFDMA_RXC_STAT_FIFO_EMPTY_SIZE 1
#define RFDMA_RXC_STAT_RUNNING_BUSY_OFFSET 16
#define RFDMA_RXC_STAT_RUNNING_BUSY_SIZE 1

/** bit field defines for rfdma_rxc_s#conf */
#define RFDMA_RXC_CONF_SH_CNT_OFFSET 0
#define RFDMA_RXC_CONF_SH_CNT_SIZE 3

/** Transmit channel pending registers
*/
struct rfdma_txp_s {
   /** Channel pending source address at address offset 0x000, read-write */
   uint32_t addr;
   /** Channel pending size in bytes at address offset 0x004, read-write */
   uint32_t size;
   /** Channel pending configuration.  A pending descriptor becomes valid when this register is written to. at address offset 0x008, read-write */
   uint32_t conf;
   /** Reserved space \private at address offset 0x00C, read-write */
   uint32_t fill;
};

/** bit field defines for rfdma_txp_s#addr */
#define RFDMA_TXP_ADDR_ADDRESS_OFFSET 2
#define RFDMA_TXP_ADDR_ADDRESS_SIZE 30

/** bit field defines for rfdma_txp_s#size */
#define RFDMA_TXP_SIZE_SIZE_OFFSET 2
#define RFDMA_TXP_SIZE_SIZE_SIZE 30

/** bit field defines for rfdma_txp_s#conf */
#define RFDMA_TXP_CONF_SEV_O_ENABLE_OFFSET 0
#define RFDMA_TXP_CONF_SEV_O_ENABLE_SIZE 1
#define RFDMA_TXP_CONF_EVENTS_O_ENABLE_OFFSET 1
#define RFDMA_TXP_CONF_EVENTS_O_ENABLE_SIZE 1
#define RFDMA_TXP_CONF_LOOP_ENABLE_OFFSET 2
#define RFDMA_TXP_CONF_LOOP_ENABLE_SIZE 1
#define RFDMA_TXP_CONF_BLOCK_CNT_OFFSET 8
#define RFDMA_TXP_CONF_BLOCK_CNT_SIZE 8
#define RFDMA_TXP_CONF_LOOP_CNT_OFFSET 16
#define RFDMA_TXP_CONF_LOOP_CNT_SIZE 16

/** Transmit channel command registers
*/
struct rfdma_txc_s {
   /** Channel command at address offset 0x000, write-only */
   uint32_t comd;
   /** Channel status at address offset 0x004, read-only */
   uint32_t stat;
   /** Reserved space \private at address offset 0x008, read-only */
   uint32_t fill0;
   /** Reserved space \private at address offset 0x00C, read-only */
   uint32_t fill1;
};

/** bit field defines for rfdma_txc_s#comd */
#define RFDMA_TXC_COMD_PENDING_SUSPEND_OFFSET 0
#define RFDMA_TXC_COMD_PENDING_SUSPEND_SIZE 1
#define RFDMA_TXC_COMD_PENDING_ABORT_OFFSET 1
#define RFDMA_TXC_COMD_PENDING_ABORT_SIZE 1
#define RFDMA_TXC_COMD_RUNNING_SUSPEND_OFFSET 2
#define RFDMA_TXC_COMD_RUNNING_SUSPEND_SIZE 1
#define RFDMA_TXC_COMD_RUNNING_ABORT_OFFSET 3
#define RFDMA_TXC_COMD_RUNNING_ABORT_SIZE 1

/** bit field defines for rfdma_txc_s#stat */
#define RFDMA_TXC_STAT_PENDING_SUSPENDED_OFFSET 0
#define RFDMA_TXC_STAT_PENDING_SUSPENDED_SIZE 1
#define RFDMA_TXC_STAT_PENDING_VALID_OFFSET 1
#define RFDMA_TXC_STAT_PENDING_VALID_SIZE 1
#define RFDMA_TXC_STAT_RUNNING_SUSPEND_OFFSET 2
#define RFDMA_TXC_STAT_RUNNING_SUSPEND_SIZE 1
#define RFDMA_TXC_STAT_RUNNING_VALID_OFFSET 3
#define RFDMA_TXC_STAT_RUNNING_VALID_SIZE 1
#define RFDMA_TXC_STAT_RUNNING_ABORT_BUSY_OFFSET 4
#define RFDMA_TXC_STAT_RUNNING_ABORT_BUSY_SIZE 1
#define RFDMA_TXC_STAT_FIFO_FULL_OFFSET 5
#define RFDMA_TXC_STAT_FIFO_FULL_SIZE 1
#define RFDMA_TXC_STAT_RUNNING_BUSY_OFFSET 16
#define RFDMA_TXC_STAT_RUNNING_BUSY_SIZE 1

/** Transmit channel running registers
*/
struct rfdma_txr_s {
   /** Channel running source address at address offset 0x000, read-only */
   uint32_t addr;
   /** Channel running size in bytes at address offset 0x004, read-only */
   uint32_t size;
   /** Channel running configuration at address offset 0x008, read-only */
   uint32_t conf;
   /** Reserved space \private at address offset 0x00C, read-only */
   uint32_t fill;
};

/** bit field defines for rfdma_txr_s#addr */
#define RFDMA_TXR_ADDR_ADDRESS_OFFSET 2
#define RFDMA_TXR_ADDR_ADDRESS_SIZE 30

/** bit field defines for rfdma_txr_s#size */
#define RFDMA_TXR_SIZE_SIZE_OFFSET 2
#define RFDMA_TXR_SIZE_SIZE_SIZE 30

/** bit field defines for rfdma_txr_s#conf */
#define RFDMA_TXR_CONF_SEV_O_ENABLE_OFFSET 0
#define RFDMA_TXR_CONF_SEV_O_ENABLE_SIZE 1
#define RFDMA_TXR_CONF_EVENTS_O_ENABLE_OFFSET 1
#define RFDMA_TXR_CONF_EVENTS_O_ENABLE_SIZE 1
#define RFDMA_TXR_CONF_LOOP_ENABLE_OFFSET 2
#define RFDMA_TXR_CONF_LOOP_ENABLE_SIZE 1
#define RFDMA_TXR_CONF_BLOCK_CNT_OFFSET 8
#define RFDMA_TXR_CONF_BLOCK_CNT_SIZE 8
#define RFDMA_TXR_CONF_LOOP_CNT_OFFSET 16
#define RFDMA_TXR_CONF_LOOP_CNT_SIZE 16

/** Receive channel running registers
*/
struct rfdma_rxr_s {
   /** Channel running source address at address offset 0x000, read-only */
   uint32_t addr;
   /** Channel running size in bytes at address offset 0x004, read-only */
   uint32_t size;
   /** Channel running configuration at address offset 0x008, read-only */
   uint32_t conf;
   /** Reserved space \private at address offset 0x00C, read-only */
   uint32_t fill;
};

/** bit field defines for rfdma_rxr_s#addr */
#define RFDMA_RXR_ADDR_ADDRESS_OFFSET 2
#define RFDMA_RXR_ADDR_ADDRESS_SIZE 30

/** bit field defines for rfdma_rxr_s#size */
#define RFDMA_RXR_SIZE_SIZE_OFFSET 2
#define RFDMA_RXR_SIZE_SIZE_SIZE 30

/** bit field defines for rfdma_rxr_s#conf */
#define RFDMA_RXR_CONF_SEV_O_ENABLE_OFFSET 0
#define RFDMA_RXR_CONF_SEV_O_ENABLE_SIZE 1
#define RFDMA_RXR_CONF_EVENTS_O_ENABLE_OFFSET 1
#define RFDMA_RXR_CONF_EVENTS_O_ENABLE_SIZE 1
#define RFDMA_RXR_CONF_LOOP_ENABLE_OFFSET 2
#define RFDMA_RXR_CONF_LOOP_ENABLE_SIZE 1
#define RFDMA_RXR_CONF_BLOCK_CNT_OFFSET 8
#define RFDMA_RXR_CONF_BLOCK_CNT_SIZE 8
#define RFDMA_RXR_CONF_LOOP_CNT_OFFSET 16
#define RFDMA_RXR_CONF_LOOP_CNT_SIZE 16

/** Receive channel pending registers
*/
struct rfdma_rxp_s {
   /** Channel pending source address at address offset 0x000, read-write */
   uint32_t addr;
   /** Channel pending size in bytes at address offset 0x004, read-write */
   uint32_t size;
   /** Channel pending configuration.  A pending descriptor becomes valid when this register is written to. at address offset 0x008, read-write */
   uint32_t conf;
   /** Reserved space \private at address offset 0x00C, read-write */
   uint32_t fill;
};

/** bit field defines for rfdma_rxp_s#addr */
#define RFDMA_RXP_ADDR_ADDRESS_OFFSET 2
#define RFDMA_RXP_ADDR_ADDRESS_SIZE 30

/** bit field defines for rfdma_rxp_s#size */
#define RFDMA_RXP_SIZE_SIZE_OFFSET 2
#define RFDMA_RXP_SIZE_SIZE_SIZE 30

/** bit field defines for rfdma_rxp_s#conf */
#define RFDMA_RXP_CONF_SEV_O_ENABLE_OFFSET 0
#define RFDMA_RXP_CONF_SEV_O_ENABLE_SIZE 1
#define RFDMA_RXP_CONF_EVENTS_O_ENABLE_OFFSET 1
#define RFDMA_RXP_CONF_EVENTS_O_ENABLE_SIZE 1
#define RFDMA_RXP_CONF_LOOP_ENABLE_OFFSET 2
#define RFDMA_RXP_CONF_LOOP_ENABLE_SIZE 1
#define RFDMA_RXP_CONF_BLOCK_CNT_OFFSET 8
#define RFDMA_RXP_CONF_BLOCK_CNT_SIZE 8
#define RFDMA_RXP_CONF_LOOP_CNT_OFFSET 16
#define RFDMA_RXP_CONF_LOOP_CNT_SIZE 16

/** RFDMA control registers
*/
struct rfdma_s {
   /** Transmit channel pending registers at address offset 0x000 size: 16 */
   struct rfdma_txp_s txp;
   /** Reserved space */
   uint8_t fill0[112];
   /** Receive channel pending registers at address offset 0x080 size: 16 */
   struct rfdma_rxp_s rxp;
   /** Reserved space */
   uint8_t fill1[112];
   /** Transmit channel running registers at address offset 0x100 size: 16 */
   struct rfdma_txr_s txr;
   /** Reserved space */
   uint8_t fill2[112];
   /** Receive channel running registers at address offset 0x180 size: 16 */
   struct rfdma_rxr_s rxr;
   /** Reserved space */
   uint8_t fill3[112];
   /** Transmit channel command registers at address offset 0x200 size: 16 */
   struct rfdma_txc_s txc;
   /** Reserved space */
   uint8_t fill4[112];
   /** Receive channel command registers at address offset 0x280 size: 16 */
   struct rfdma_rxc_s rxc;
   /** Reserved space */
   uint8_t fill5[112];
   /** Clear Fault Status at address offset 0x300, write-only */
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
   uint8_t fill6[232];
   /** Tx diversity mode configuration at address offset 0x400, read-write */
   uint32_t txdiversitycfg;
   /** Reserved space */
   uint8_t fill7[3020];
   /** Peripheral ID4 Register at address offset 0xFD0, read-only constant 0x00000007 */
   uint32_t peripheralid4;
   /** Peripheral ID5 Register at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** Peripheral ID6 Register at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** Peripheral ID7 Register at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** Peripheral ID0 Register at address offset 0xFE0, read-only constant 0x00000062 */
   uint32_t peripheralid0;
   /** Peripheral ID1 Register at address offset 0xFE4, read-only constant 0x000000DA */
   uint32_t peripheralid1;
   /** Peripheral ID2 Register at address offset 0xFE8, read-only constant 0x00000008 */
   uint32_t peripheralid2;
   /** Peripheral ID3 Register at address offset 0xFEC, read-only constant 0x00000000 */
   uint32_t peripheralid3;
   /** Primecell ID register 0 at address offset 0xFF0, read-only constant 0x0000000D */
   uint32_t componentid0;
   /** Primecell ID register 1 at address offset 0xFF4, read-only constant 0x000000F0 */
   uint32_t componentid1;
   /** Primecell ID register 2 at address offset 0xFF8, read-only constant 0x00000005 */
   uint32_t componentid2;
   /** Primecell ID register 3 at address offset 0xFFC, read-only constant 0x000000B1 */
   uint32_t componentid3;
};

/** bit field defines for rfdma_s#fltclear */
#define RFDMA_FLTCLEAR_TX0_AXI_READ_ERROR_OFFSET 0
#define RFDMA_FLTCLEAR_TX0_AXI_READ_ERROR_SIZE 1
#define RFDMA_FLTCLEAR_TX1_AXI_READ_ERROR_OFFSET 1
#define RFDMA_FLTCLEAR_TX1_AXI_READ_ERROR_SIZE 1
#define RFDMA_FLTCLEAR_TX_DIVERSITY_CONFIG_ERROR_OFFSET 8
#define RFDMA_FLTCLEAR_TX_DIVERSITY_CONFIG_ERROR_SIZE 1
#define RFDMA_FLTCLEAR_RX0_AXI_WRITE_ERROR_OFFSET 16
#define RFDMA_FLTCLEAR_RX0_AXI_WRITE_ERROR_SIZE 1
#define RFDMA_FLTCLEAR_RX1_AXI_WRITE_ERROR_OFFSET 17
#define RFDMA_FLTCLEAR_RX1_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for rfdma_s#fltstatus */
#define RFDMA_FLTSTATUS_TX0_AXI_READ_ERROR_OFFSET 0
#define RFDMA_FLTSTATUS_TX0_AXI_READ_ERROR_SIZE 1
#define RFDMA_FLTSTATUS_TX1_AXI_READ_ERROR_OFFSET 1
#define RFDMA_FLTSTATUS_TX1_AXI_READ_ERROR_SIZE 1
#define RFDMA_FLTSTATUS_TX_DIVERSITY_CONFIG_ERROR_OFFSET 8
#define RFDMA_FLTSTATUS_TX_DIVERSITY_CONFIG_ERROR_SIZE 1
#define RFDMA_FLTSTATUS_RX0_AXI_WRITE_ERROR_OFFSET 16
#define RFDMA_FLTSTATUS_RX0_AXI_WRITE_ERROR_SIZE 1
#define RFDMA_FLTSTATUS_RX1_AXI_WRITE_ERROR_OFFSET 17
#define RFDMA_FLTSTATUS_RX1_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for rfdma_s#fltrawstatus */
#define RFDMA_FLTRAWSTATUS_TX0_AXI_READ_ERROR_OFFSET 0
#define RFDMA_FLTRAWSTATUS_TX0_AXI_READ_ERROR_SIZE 1
#define RFDMA_FLTRAWSTATUS_TX1_AXI_READ_ERROR_OFFSET 1
#define RFDMA_FLTRAWSTATUS_TX1_AXI_READ_ERROR_SIZE 1
#define RFDMA_FLTRAWSTATUS_TX_DIVERSITY_CONFIG_ERROR_OFFSET 8
#define RFDMA_FLTRAWSTATUS_TX_DIVERSITY_CONFIG_ERROR_SIZE 1
#define RFDMA_FLTRAWSTATUS_RX0_AXI_WRITE_ERROR_OFFSET 16
#define RFDMA_FLTRAWSTATUS_RX0_AXI_WRITE_ERROR_SIZE 1
#define RFDMA_FLTRAWSTATUS_RX1_AXI_WRITE_ERROR_OFFSET 17
#define RFDMA_FLTRAWSTATUS_RX1_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for rfdma_s#fltmaskclr */
#define RFDMA_FLTMASKCLR_TX0_AXI_READ_ERROR_OFFSET 0
#define RFDMA_FLTMASKCLR_TX0_AXI_READ_ERROR_SIZE 1
#define RFDMA_FLTMASKCLR_TX1_AXI_READ_ERROR_OFFSET 1
#define RFDMA_FLTMASKCLR_TX1_AXI_READ_ERROR_SIZE 1
#define RFDMA_FLTMASKCLR_TX_DIVERSITY_CONFIG_ERROR_OFFSET 8
#define RFDMA_FLTMASKCLR_TX_DIVERSITY_CONFIG_ERROR_SIZE 1
#define RFDMA_FLTMASKCLR_RX0_AXI_WRITE_ERROR_OFFSET 16
#define RFDMA_FLTMASKCLR_RX0_AXI_WRITE_ERROR_SIZE 1
#define RFDMA_FLTMASKCLR_RX1_AXI_WRITE_ERROR_OFFSET 17
#define RFDMA_FLTMASKCLR_RX1_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for rfdma_s#fltmaskset */
#define RFDMA_FLTMASKSET_TX0_AXI_READ_ERROR_OFFSET 0
#define RFDMA_FLTMASKSET_TX0_AXI_READ_ERROR_SIZE 1
#define RFDMA_FLTMASKSET_TX1_AXI_READ_ERROR_OFFSET 1
#define RFDMA_FLTMASKSET_TX1_AXI_READ_ERROR_SIZE 1
#define RFDMA_FLTMASKSET_TX_DIVERSITY_CONFIG_ERROR_OFFSET 8
#define RFDMA_FLTMASKSET_TX_DIVERSITY_CONFIG_ERROR_SIZE 1
#define RFDMA_FLTMASKSET_RX0_AXI_WRITE_ERROR_OFFSET 16
#define RFDMA_FLTMASKSET_RX0_AXI_WRITE_ERROR_SIZE 1
#define RFDMA_FLTMASKSET_RX1_AXI_WRITE_ERROR_OFFSET 17
#define RFDMA_FLTMASKSET_RX1_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for rfdma_s#fltmaskstatus */
#define RFDMA_FLTMASKSTATUS_TX0_AXI_READ_ERROR_OFFSET 0
#define RFDMA_FLTMASKSTATUS_TX0_AXI_READ_ERROR_SIZE 1
#define RFDMA_FLTMASKSTATUS_TX1_AXI_READ_ERROR_OFFSET 1
#define RFDMA_FLTMASKSTATUS_TX1_AXI_READ_ERROR_SIZE 1
#define RFDMA_FLTMASKSTATUS_TX_DIVERSITY_CONFIG_ERROR_OFFSET 8
#define RFDMA_FLTMASKSTATUS_TX_DIVERSITY_CONFIG_ERROR_SIZE 1
#define RFDMA_FLTMASKSTATUS_RX0_AXI_WRITE_ERROR_OFFSET 16
#define RFDMA_FLTMASKSTATUS_RX0_AXI_WRITE_ERROR_SIZE 1
#define RFDMA_FLTMASKSTATUS_RX1_AXI_WRITE_ERROR_OFFSET 17
#define RFDMA_FLTMASKSTATUS_RX1_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for rfdma_s#txdiversitycfg */
#define RFDMA_TXDIVERSITYCFG_TXDIVERSITYCFG_OFFSET 0
#define RFDMA_TXDIVERSITYCFG_TXDIVERSITYCFG_SIZE 1

/** bit field defines for rfdma_s#peripheralid4 */
#define RFDMA_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define RFDMA_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for rfdma_s#peripheralid5 */
#define RFDMA_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define RFDMA_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for rfdma_s#peripheralid6 */
#define RFDMA_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define RFDMA_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for rfdma_s#peripheralid7 */
#define RFDMA_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define RFDMA_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for rfdma_s#peripheralid0 */
#define RFDMA_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define RFDMA_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for rfdma_s#peripheralid1 */
#define RFDMA_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define RFDMA_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for rfdma_s#peripheralid2 */
#define RFDMA_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define RFDMA_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for rfdma_s#peripheralid3 */
#define RFDMA_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define RFDMA_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for rfdma_s#componentid0 */
#define RFDMA_COMPONENTID0_COMPONENTID0_OFFSET 0
#define RFDMA_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for rfdma_s#componentid1 */
#define RFDMA_COMPONENTID1_COMPONENTID1_OFFSET 0
#define RFDMA_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for rfdma_s#componentid2 */
#define RFDMA_COMPONENTID2_COMPONENTID2_OFFSET 0
#define RFDMA_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for rfdma_s#componentid3 */
#define RFDMA_COMPONENTID3_COMPONENTID3_OFFSET 0
#define RFDMA_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF rfdma.h */
#endif
