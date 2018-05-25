#ifndef INCLUDED_IPC_MBX
#define INCLUDED_IPC_MBX
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

#define IPC_MBX_PERIPHERALID4 0x07
#define IPC_MBX_PERIPHERALID5 0x00
#define IPC_MBX_PERIPHERALID6 0x00
#define IPC_MBX_PERIPHERALID7 0x00
#define IPC_MBX_PERIPHERALID0 0x65
#define IPC_MBX_PERIPHERALID1 0xDA
#define IPC_MBX_PERIPHERALID2 0x08
#define IPC_MBX_PERIPHERALID3 0x00
#define IPC_MBX_COMPONENTID0 0x0D
#define IPC_MBX_COMPONENTID1 0xF0
#define IPC_MBX_COMPONENTID2 0x05
#define IPC_MBX_COMPONENTID3 0xB1

/** IPC Mailbox
*/
struct ipc_mbx_mbx_s {
   /** TX message set register. When TX filtering enabled, only writable by TX master at address offset 0x000, write-only */
   uint32_t tx_set;
   /** TX message clear register. When TX filtering enabled, only writable by RX master at address offset 0x004, write-only */
   uint32_t tx_clr;
   /** TX message status register. Accesses not filtered. at address offset 0x008, read-only */
   uint32_t tx_msg;
   /** TX Master filter enable. Only writable in priviledged mode (HPROT[1]=1), RO otherwise. at address offset 0x00C, read-write */
   uint32_t tx_cfg;
   /** RX message register. When RX filtering enabled, only writable by RX master at address offset 0x010, write-only */
   uint32_t rx_set;
   /** RX message clear register. When RX filtering enabled, only writable by TX master at address offset 0x014, write-only */
   uint32_t rx_clr;
   /** RX message status register. Accesses not filtered. at address offset 0x018, read-only */
   uint32_t rx_msg;
   /** RX Master filter enable. Only writable in priviledged mode (HPROT[1]=1), RO otherwise. at address offset 0x01C, read-write */
   uint32_t rx_cfg;
};

/** bit field defines for ipc_mbx_mbx_s#tx_set */
#define IPC_MBX_MBX_TX_SET_TX_MESSAGE_SET_OFFSET 0
#define IPC_MBX_MBX_TX_SET_TX_MESSAGE_SET_SIZE 32

/** bit field defines for ipc_mbx_mbx_s#tx_clr */
#define IPC_MBX_MBX_TX_CLR_TX_MESSAGE_CLEAR_OFFSET 0
#define IPC_MBX_MBX_TX_CLR_TX_MESSAGE_CLEAR_SIZE 32

/** bit field defines for ipc_mbx_mbx_s#tx_msg */
#define IPC_MBX_MBX_TX_MSG_TX_MESSAGE_OFFSET 0
#define IPC_MBX_MBX_TX_MSG_TX_MESSAGE_SIZE 32

/** bit field defines for ipc_mbx_mbx_s#tx_cfg */
#define IPC_MBX_MBX_TX_CFG_TX_MASTER_FILTER_ENABLE_OFFSET 0
#define IPC_MBX_MBX_TX_CFG_TX_MASTER_FILTER_ENABLE_SIZE 1
#define IPC_MBX_MBX_TX_CFG_TX_MASTER_ID_OFFSET 1
#define IPC_MBX_MBX_TX_CFG_TX_MASTER_ID_SIZE 30
#define IPC_MBX_MBX_TX_CFG_TX_IRQ_MASK_OFFSET 31
#define IPC_MBX_MBX_TX_CFG_TX_IRQ_MASK_SIZE 1

/** bit field defines for ipc_mbx_mbx_s#rx_set */
#define IPC_MBX_MBX_RX_SET_RX_MESSAGE_SET_OFFSET 0
#define IPC_MBX_MBX_RX_SET_RX_MESSAGE_SET_SIZE 32

/** bit field defines for ipc_mbx_mbx_s#rx_clr */
#define IPC_MBX_MBX_RX_CLR_RX_MESSAGE_CLEAR_OFFSET 0
#define IPC_MBX_MBX_RX_CLR_RX_MESSAGE_CLEAR_SIZE 32

/** bit field defines for ipc_mbx_mbx_s#rx_msg */
#define IPC_MBX_MBX_RX_MSG_RX_MESSAGE_OFFSET 0
#define IPC_MBX_MBX_RX_MSG_RX_MESSAGE_SIZE 32

/** bit field defines for ipc_mbx_mbx_s#rx_cfg */
#define IPC_MBX_MBX_RX_CFG_RX_MASTER_FILTER_ENABLE_OFFSET 0
#define IPC_MBX_MBX_RX_CFG_RX_MASTER_FILTER_ENABLE_SIZE 1
#define IPC_MBX_MBX_RX_CFG_RX_MASTER_ID_OFFSET 1
#define IPC_MBX_MBX_RX_CFG_RX_MASTER_ID_SIZE 30
#define IPC_MBX_MBX_RX_CFG_RX_IRQ_MASK_OFFSET 31
#define IPC_MBX_MBX_RX_CFG_RX_IRQ_MASK_SIZE 1

/** IPC Mailbox
*/
struct ipc_mbx_s {
   /** IPC Mailbox at address offset 0x000 size: 64 */
   struct ipc_mbx_mbx_s mbx[2];
   /** Reserved space */
   uint8_t fill0[3984];
   /** Peripheral ID4 at address offset 0xFD0, read-only constant 0x00000007 */
   uint32_t peripheralid4;
   /** Peripheral ID5 at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** Peripheral ID6 at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** Peripheral ID7 at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** Peripheral ID0 at address offset 0xFE0, read-only constant 0x00000065 */
   uint32_t peripheralid0;
   /** Peripheral ID1 at address offset 0xFE4, read-only constant 0x000000DA */
   uint32_t peripheralid1;
   /** Peripheral ID2 at address offset 0xFE8, read-only constant 0x00000008 */
   uint32_t peripheralid2;
   /** Peripheral ID3 at address offset 0xFEC, read-only constant 0x00000000 */
   uint32_t peripheralid3;
   /** Component ID0 at address offset 0xFF0, read-only constant 0x0000000D */
   uint32_t componentid0;
   /** Component ID1 at address offset 0xFF4, read-only constant 0x000000F0 */
   uint32_t componentid1;
   /** Component ID2 at address offset 0xFF8, read-only constant 0x00000005 */
   uint32_t componentid2;
   /** Component ID3 at address offset 0xFFC, read-only constant 0x000000B1 */
   uint32_t componentid3;
};

/** bit field defines for ipc_mbx_s#peripheralid4 */
#define IPC_MBX_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define IPC_MBX_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for ipc_mbx_s#peripheralid5 */
#define IPC_MBX_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define IPC_MBX_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for ipc_mbx_s#peripheralid6 */
#define IPC_MBX_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define IPC_MBX_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for ipc_mbx_s#peripheralid7 */
#define IPC_MBX_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define IPC_MBX_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for ipc_mbx_s#peripheralid0 */
#define IPC_MBX_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define IPC_MBX_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for ipc_mbx_s#peripheralid1 */
#define IPC_MBX_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define IPC_MBX_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for ipc_mbx_s#peripheralid2 */
#define IPC_MBX_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define IPC_MBX_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for ipc_mbx_s#peripheralid3 */
#define IPC_MBX_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define IPC_MBX_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for ipc_mbx_s#componentid0 */
#define IPC_MBX_COMPONENTID0_COMPONENTID0_OFFSET 0
#define IPC_MBX_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for ipc_mbx_s#componentid1 */
#define IPC_MBX_COMPONENTID1_COMPONENTID1_OFFSET 0
#define IPC_MBX_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for ipc_mbx_s#componentid2 */
#define IPC_MBX_COMPONENTID2_COMPONENTID2_OFFSET 0
#define IPC_MBX_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for ipc_mbx_s#componentid3 */
#define IPC_MBX_COMPONENTID3_COMPONENTID3_OFFSET 0
#define IPC_MBX_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF ipc_mbx.h */
#endif
