#ifndef INCLUDED_HWSEQUENCER
#define INCLUDED_HWSEQUENCER
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** Number of Event Count Registers */
#define HWS_NR_ENGINES 4
#define HWSEQUENCER_PERIPHERALID4 0x07
#define HWSEQUENCER_PERIPHERALID5 0x00
#define HWSEQUENCER_PERIPHERALID6 0x00
#define HWSEQUENCER_PERIPHERALID7 0x00
#define HWSEQUENCER_PERIPHERALID0 0x61
#define HWSEQUENCER_PERIPHERALID1 0xDA
#define HWSEQUENCER_PERIPHERALID2 0x18
#define HWSEQUENCER_PERIPHERALID3 0x00
#define HWSEQUENCER_COMPONENTID0 0x0D
#define HWSEQUENCER_COMPONENTID1 0xF0
#define HWSEQUENCER_COMPONENTID2 0x05
#define HWSEQUENCER_COMPONENTID3 0xB1

/** Hardware Sequencer
*/
struct hwsequencer_s {
   /** Flag status register at address offset 0x000, read-only */
   uint32_t flagstat;
   /** Flag clear register at address offset 0x004, write-only */
   uint32_t flagclr;
   /** Reserved space */
   uint8_t fill0[8];
   /** Barrier Init Register at address offset 0x010, write-only */
   uint32_t barinit;
   /** Barrier Set Register at address offset 0x014, write-only */
   uint32_t barset;
   /** Barrier Decrement Register at address offset 0x018, write-only */
   uint32_t bardec;
   /** Reserved space */
   uint8_t fill1[4];
   /** Barrier Read  Request at address offset 0x020, write-only */
   uint32_t barreadreq;
   /** Barrier Counter at address offset 0x024, read-only */
   uint32_t barcnt;
   /** Barrier Sequence Pointer at address offset 0x028, read-only */
   uint32_t barseqptr;
   /** Reserved space */
   uint8_t fill2[4];
   /** Flag interrupt mask clear register at address offset 0x030, write-only */
   uint32_t flagmask_clear;
   /** Flag interrupt mask set register at address offset 0x034, write-only */
   uint32_t flagmask_set;
   /** Flag interrupt mask status register at address offset 0x038, read-only */
   uint32_t flagmask_status;
   /** Reserved space */
   uint8_t fill3[4];
   /** Task End FIFO Push at address offset 0x040, write-only */
   uint32_t tefpush;
   /** Execute sequence at address offset 0x044, write-only */
   uint32_t exec;
   /** Reserved space */
   uint8_t fill4[8];
   /** Job event status register at address offset 0x050, read-only */
   uint32_t jobevstat;
   /** Job event clear register at address offset 0x054, write-only */
   uint32_t jobevclr;
   /** Job event mask set register at address offset 0x058, write-only */
   uint32_t jobevmask_clear;
   /** Job event mask clear register at address offset 0x05C, write-only */
   uint32_t jobevmask_set;
   /** Job event mask status register at address offset 0x060, read-only */
   uint32_t jobevmask_stat;
   /** Reserved space */
   uint8_t fill5[28];
   /** Job queue 0 Push at address offset 0x080, write-only */
   uint32_t jobqueue0_push;
   /** Job queue 1 Push at address offset 0x084, write-only */
   uint32_t jobqueue1_push;
   /** Reserved space */
   uint8_t fill6[24];
   /** Job queue 0 Status at address offset 0x0A0, read-only */
   uint32_t jobqueue0_status;
   /** Job queue 1 Status at address offset 0x0A4, read-only */
   uint32_t jobqueue1_status;
   /** Reserved space */
   uint8_t fill7[24];
   /** Job queue 0 Configuration at address offset 0x0C0, read-write */
   uint32_t jobqueue0_config;
   /** Job queue 1 Configuration at address offset 0x0C4, read-write */
   uint32_t jobqueue1_config;
   /** Reserved space */
   uint8_t fill8[24];
   /** Job queue irq raw status at address offset 0x0E0, read-only */
   uint32_t jobqueue_irq_raw;
   /** Job queue irq status at address offset 0x0E4, read-only */
   uint32_t jobqueue_irq_status;
   /** Reserved space */
   uint8_t fill9[4];
   /** Job queue irq enable set at address offset 0x0EC, write-only */
   uint32_t jobqueue_irq_enable_set;
   /** Job queue irq enable clear at address offset 0x0F0, write-only */
   uint32_t jobqueue_irq_enable_clear;
   /** Job queue irq enable status at address offset 0x0F4, read-only */
   uint32_t jobqueue_irq_enable_status;
   /** Reserved space */
   uint8_t fill10[8];
   /** Command Register at address offset 0x100, write-only */
   uint32_t cmd;
   /** Status Register at address offset 0x104, read-only */
   uint32_t status;
   /** Task End FIFO Reset at address offset 0x108, write-only */
   uint32_t tefrst;
   /** Task End FIFO FIFO Read Request at address offset 0x10C, write-only */
   uint32_t tefreadreq;
   /** Task End FIFO FIFO Value at address offset 0x110, read-only */
   uint32_t tefval;
   /** Reserved space */
   uint8_t fill11[12];
   /** Engine Enable Set at address offset 0x120, write-only */
   uint32_t engenaset;
   /** Engine Enable Clear at address offset 0x124, write-only */
   uint32_t engenaclr;
   /** Engine Enable Status at address offset 0x128, read-only */
   uint32_t engenastat;
   /** Reserved space */
   uint8_t fill12[4];
   /** Engine Suspend Set at address offset 0x130, write-only */
   uint32_t engsuspset;
   /** Engine Suspend Clear at address offset 0x134, write-only */
   uint32_t engsuspclr;
   /** Engine Suspend Status at address offset 0x138, read-only */
   uint32_t engsuspstat;
   /** Reserved space */
   uint8_t fill13[196];
   /** Sequence Instruction Base Register at address offset 0x200, read-write */
   uint32_t seqinstrbase;
   /** Barrier Base Register at address offset 0x204, read-write */
   uint32_t barrierbase;
   /** Last  Barrier Page Register at address offset 0x208, read-write */
   uint32_t lbp;
   /** Engine Base Register at address offset 0x20C, read-write */
   uint32_t membase[HWS_NR_ENGINES];
   /** Reserved space */
   uint8_t fill14[228];
   /** Fault Clear  Register at address offset 0x300, write-only */
   uint32_t fltclear;
   /** Fault Status Register at address offset 0x304, read-only */
   uint32_t fltstatus;
   /** Fault Raw Status Register at address offset 0x308, read-only */
   uint32_t fltrawstatus;
   /** Fault Mask Clear Register at address offset 0x30C, write-only */
   uint32_t fltmaskclr;
   /** Fault Mask Set Register at address offset 0x310, write-only */
   uint32_t fltmaskset;
   /** Fault Mask Status Register at address offset 0x314, read-only */
   uint32_t fltmaskstatus;
   /** Fault Signature for Barriers at address offset 0x318, read-only */
   uint32_t fltsigbarrier;
   /** Fault Signature for Engines at address offset 0x31C, read-only */
   uint32_t fltsigeng;
   /** Fault Signature for AXI addresses at address offset 0x320, read-only */
   uint32_t fltsigaddr;
   /** Fault Signature for Sequences at address offset 0x324, read-only */
   uint32_t fltseqptr;
   /** Reserved space */
   uint8_t fill15[2776];
   /** Configuration Parameter Register at address offset 0xE00, read-only */
   uint32_t config;
   /** Reserved space */
   uint8_t fill16[460];
   /** Peripheral ID4 at address offset 0xFD0, read-only constant 0x00000007 */
   uint32_t peripheralid4;
   /** Peripheral ID5 at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** Peripheral ID6 at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** Peripheral ID7 at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** Peripheral ID0 at address offset 0xFE0, read-only constant 0x00000061 */
   uint32_t peripheralid0;
   /** Peripheral ID1 at address offset 0xFE4, read-only constant 0x000000DA */
   uint32_t peripheralid1;
   /** Peripheral ID2 at address offset 0xFE8, read-only constant 0x00000018 */
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
   /** Reserved space */
   uint8_t fill17[12288];
   /** Barrier Init at address offset 0x4000, write-only */
   uint32_t bar_init_array[4096];
   /** Barrier Set at address offset 0x8000, write-only */
   uint32_t bar_set_array[4096];
   /** Barrier Decrement at address offset 0xC000, write-only */
   uint32_t bar_dec_array[4096];
};

/** bit field defines for hwsequencer_s#flagstat */
#define HWSEQUENCER_FLAGSTAT_FLAGSTAT_OFFSET 0
#define HWSEQUENCER_FLAGSTAT_FLAGSTAT_SIZE 32

/** bit field defines for hwsequencer_s#flagclr */
#define HWSEQUENCER_FLAGCLR_FLAGCLR_OFFSET 0
#define HWSEQUENCER_FLAGCLR_FLAGCLR_SIZE 32

/** bit field defines for hwsequencer_s#barinit */
#define HWSEQUENCER_BARINIT_IDX_OFFSET 0
#define HWSEQUENCER_BARINIT_IDX_SIZE 12
#define HWSEQUENCER_BARINIT_CNT_OFFSET 12
#define HWSEQUENCER_BARINIT_CNT_SIZE 4
#define HWSEQUENCER_BARINIT_SEQUENCE_POINTER_OFFSET 16
#define HWSEQUENCER_BARINIT_SEQUENCE_POINTER_SIZE 16

/** bit field defines for hwsequencer_s#barset */
#define HWSEQUENCER_BARSET_IDX_OFFSET 0
#define HWSEQUENCER_BARSET_IDX_SIZE 12
#define HWSEQUENCER_BARSET_SEQUENCE_POINTER_OFFSET 16
#define HWSEQUENCER_BARSET_SEQUENCE_POINTER_SIZE 16

/** bit field defines for hwsequencer_s#bardec */
#define HWSEQUENCER_BARDEC_IDX_OFFSET 0
#define HWSEQUENCER_BARDEC_IDX_SIZE 12

/** bit field defines for hwsequencer_s#barreadreq */
#define HWSEQUENCER_BARREADREQ_IDX_OFFSET 0
#define HWSEQUENCER_BARREADREQ_IDX_SIZE 12

/** bit field defines for hwsequencer_s#barcnt */
#define HWSEQUENCER_BARCNT_IDX_OFFSET 0
#define HWSEQUENCER_BARCNT_IDX_SIZE 12
#define HWSEQUENCER_BARCNT_REG_VALID_OFFSET 12
#define HWSEQUENCER_BARCNT_REG_VALID_SIZE 1
#define HWSEQUENCER_BARCNT_CNT_OFFSET 16
#define HWSEQUENCER_BARCNT_CNT_SIZE 4

/** bit field defines for hwsequencer_s#barseqptr */
#define HWSEQUENCER_BARSEQPTR_IDX_OFFSET 0
#define HWSEQUENCER_BARSEQPTR_IDX_SIZE 12
#define HWSEQUENCER_BARSEQPTR_REG_VALID_OFFSET 12
#define HWSEQUENCER_BARSEQPTR_REG_VALID_SIZE 1
#define HWSEQUENCER_BARSEQPTR_SEQUENCE_POINTER_OFFSET 16
#define HWSEQUENCER_BARSEQPTR_SEQUENCE_POINTER_SIZE 16

/** bit field defines for hwsequencer_s#flagmask_clear */
#define HWSEQUENCER_FLAGMASK_CLEAR_FLAGMASK_CLEAR_OFFSET 0
#define HWSEQUENCER_FLAGMASK_CLEAR_FLAGMASK_CLEAR_SIZE 32

/** bit field defines for hwsequencer_s#flagmask_set */
#define HWSEQUENCER_FLAGMASK_SET_FLAGMASK_SET_OFFSET 0
#define HWSEQUENCER_FLAGMASK_SET_FLAGMASK_SET_SIZE 32

/** bit field defines for hwsequencer_s#flagmask_status */
#define HWSEQUENCER_FLAGMASK_STATUS_FLAGMASK_STATUS_OFFSET 0
#define HWSEQUENCER_FLAGMASK_STATUS_FLAGMASK_STATUS_SIZE 32

/** bit field defines for hwsequencer_s#tefpush */
#define HWSEQUENCER_TEFPUSH_IDX_OFFSET 0
#define HWSEQUENCER_TEFPUSH_IDX_SIZE 5
#define HWSEQUENCER_TEFPUSH_SEQUENCE_PARAMETER_OFFSET 8
#define HWSEQUENCER_TEFPUSH_SEQUENCE_PARAMETER_SIZE 8
#define HWSEQUENCER_TEFPUSH_SEQUENCE_POINTER_OFFSET 16
#define HWSEQUENCER_TEFPUSH_SEQUENCE_POINTER_SIZE 16

/** bit field defines for hwsequencer_s#exec */
#define HWSEQUENCER_EXEC_SEQUENCE_POINTER_OFFSET 0
#define HWSEQUENCER_EXEC_SEQUENCE_POINTER_SIZE 16
#define HWSEQUENCER_EXEC_SEQUENCE_PARAMETER_OFFSET 16
#define HWSEQUENCER_EXEC_SEQUENCE_PARAMETER_SIZE 8

/** bit field defines for hwsequencer_s#jobevstat */
#define HWSEQUENCER_JOBEVSTAT_JOBEVSTAT_OFFSET 0
#define HWSEQUENCER_JOBEVSTAT_JOBEVSTAT_SIZE 32

/** bit field defines for hwsequencer_s#jobevclr */
#define HWSEQUENCER_JOBEVCLR_JOBEVCLR_OFFSET 0
#define HWSEQUENCER_JOBEVCLR_JOBEVCLR_SIZE 32

/** bit field defines for hwsequencer_s#jobevmask_clear */
#define HWSEQUENCER_JOBEVMASK_CLEAR_JOBEVMASK_CLEAR_OFFSET 0
#define HWSEQUENCER_JOBEVMASK_CLEAR_JOBEVMASK_CLEAR_SIZE 32

/** bit field defines for hwsequencer_s#jobevmask_set */
#define HWSEQUENCER_JOBEVMASK_SET_JOBEVMASK_SET_OFFSET 0
#define HWSEQUENCER_JOBEVMASK_SET_JOBEVMASK_SET_SIZE 32

/** bit field defines for hwsequencer_s#jobevmask_stat */
#define HWSEQUENCER_JOBEVMASK_STAT_JOBEVMASK_STAT_OFFSET 0
#define HWSEQUENCER_JOBEVMASK_STAT_JOBEVMASK_STAT_SIZE 32

/** bit field defines for hwsequencer_s#jobqueue0_push */
#define HWSEQUENCER_JOBQUEUE0_PUSH_SEQUENCE_POINTER_OFFSET 0
#define HWSEQUENCER_JOBQUEUE0_PUSH_SEQUENCE_POINTER_SIZE 16
#define HWSEQUENCER_JOBQUEUE0_PUSH_SEQUENCE_PARAMETER_OFFSET 16
#define HWSEQUENCER_JOBQUEUE0_PUSH_SEQUENCE_PARAMETER_SIZE 8
#define HWSEQUENCER_JOBQUEUE0_PUSH_JOBID_OFFSET 24
#define HWSEQUENCER_JOBQUEUE0_PUSH_JOBID_SIZE 5

/** bit field defines for hwsequencer_s#jobqueue1_push */
#define HWSEQUENCER_JOBQUEUE1_PUSH_SEQUENCE_POINTER_OFFSET 0
#define HWSEQUENCER_JOBQUEUE1_PUSH_SEQUENCE_POINTER_SIZE 16
#define HWSEQUENCER_JOBQUEUE1_PUSH_SEQUENCE_PARAMETER_OFFSET 16
#define HWSEQUENCER_JOBQUEUE1_PUSH_SEQUENCE_PARAMETER_SIZE 8
#define HWSEQUENCER_JOBQUEUE1_PUSH_JOBID_OFFSET 24
#define HWSEQUENCER_JOBQUEUE1_PUSH_JOBID_SIZE 5

/** bit field defines for hwsequencer_s#jobqueue0_status */
#define HWSEQUENCER_JOBQUEUE0_STATUS_QUEUE0_SPACE_OFFSET 0
#define HWSEQUENCER_JOBQUEUE0_STATUS_QUEUE0_SPACE_SIZE 8
#define HWSEQUENCER_JOBQUEUE0_STATUS_QUEUE0_DEPTH_OFFSET 8
#define HWSEQUENCER_JOBQUEUE0_STATUS_QUEUE0_DEPTH_SIZE 8

/** bit field defines for hwsequencer_s#jobqueue1_status */
#define HWSEQUENCER_JOBQUEUE1_STATUS_QUEUE1_SPACE_OFFSET 0
#define HWSEQUENCER_JOBQUEUE1_STATUS_QUEUE1_SPACE_SIZE 8
#define HWSEQUENCER_JOBQUEUE1_STATUS_QUEUE1_DEPTH_OFFSET 8
#define HWSEQUENCER_JOBQUEUE1_STATUS_QUEUE1_DEPTH_SIZE 8

/** bit field defines for hwsequencer_s#jobqueue0_config */
#define HWSEQUENCER_JOBQUEUE0_CONFIG_QUEUE0_NEAR_EMPTY_WATERMARK_OFFSET 0
#define HWSEQUENCER_JOBQUEUE0_CONFIG_QUEUE0_NEAR_EMPTY_WATERMARK_SIZE 4

/** bit field defines for hwsequencer_s#jobqueue1_config */
#define HWSEQUENCER_JOBQUEUE1_CONFIG_QUEUE1_NEAR_EMPTY_WATERMARK_OFFSET 0
#define HWSEQUENCER_JOBQUEUE1_CONFIG_QUEUE1_NEAR_EMPTY_WATERMARK_SIZE 6

/** bit field defines for hwsequencer_s#jobqueue_irq_raw */
#define HWSEQUENCER_JOBQUEUE_IRQ_RAW_JOBQUEUE_IRQ_RAW_OFFSET 0
#define HWSEQUENCER_JOBQUEUE_IRQ_RAW_JOBQUEUE_IRQ_RAW_SIZE 2

/** bit field defines for hwsequencer_s#jobqueue_irq_status */
#define HWSEQUENCER_JOBQUEUE_IRQ_STATUS_JOBQUEUE_IRQ_STATUS_OFFSET 0
#define HWSEQUENCER_JOBQUEUE_IRQ_STATUS_JOBQUEUE_IRQ_STATUS_SIZE 2

/** bit field defines for hwsequencer_s#jobqueue_irq_enable_set */
#define HWSEQUENCER_JOBQUEUE_IRQ_ENABLE_SET_JOBQUEUE_IRQ_ENABLE_SET_OFFSET 0
#define HWSEQUENCER_JOBQUEUE_IRQ_ENABLE_SET_JOBQUEUE_IRQ_ENABLE_SET_SIZE 2

/** bit field defines for hwsequencer_s#jobqueue_irq_enable_clear */
#define HWSEQUENCER_JOBQUEUE_IRQ_ENABLE_CLEAR_JOBQUEUE_IRQ_ENABLE_CLEAR_OFFSET 0
#define HWSEQUENCER_JOBQUEUE_IRQ_ENABLE_CLEAR_JOBQUEUE_IRQ_ENABLE_CLEAR_SIZE 2

/** bit field defines for hwsequencer_s#jobqueue_irq_enable_status */
#define HWSEQUENCER_JOBQUEUE_IRQ_ENABLE_STATUS_JOBQUEUE_IRQ_ENABLE_STATUS_OFFSET 0
#define HWSEQUENCER_JOBQUEUE_IRQ_ENABLE_STATUS_JOBQUEUE_IRQ_ENABLE_STATUS_SIZE 2

/** bit field defines for hwsequencer_s#cmd */
#define HWSEQUENCER_CMD_ABORT_OFFSET 0
#define HWSEQUENCER_CMD_ABORT_SIZE 1
#define HWSEQUENCER_CMD_FLUSH_OFFSET 1
#define HWSEQUENCER_CMD_FLUSH_SIZE 1

/** bit field defines for hwsequencer_s#status */
#define HWSEQUENCER_STATUS_TASK_END_FIFO_NOT_EMPTY_OFFSET 0
#define HWSEQUENCER_STATUS_TASK_END_FIFO_NOT_EMPTY_SIZE 1
#define HWSEQUENCER_STATUS_TASK_END_FIFO_PENDING_OFFSET 1
#define HWSEQUENCER_STATUS_TASK_END_FIFO_PENDING_SIZE 1
#define HWSEQUENCER_STATUS_READY_FIFO_NOT_EMPTY_OFFSET 2
#define HWSEQUENCER_STATUS_READY_FIFO_NOT_EMPTY_SIZE 1
#define HWSEQUENCER_STATUS_READY_FIFO_FULL_OFFSET 3
#define HWSEQUENCER_STATUS_READY_FIFO_FULL_SIZE 1
#define HWSEQUENCER_STATUS_PREFETCH_NOT_EMPTY_OFFSET 4
#define HWSEQUENCER_STATUS_PREFETCH_NOT_EMPTY_SIZE 1
#define HWSEQUENCER_STATUS_PREFETCH_FULL_OFFSET 5
#define HWSEQUENCER_STATUS_PREFETCH_FULL_SIZE 1
#define HWSEQUENCER_STATUS_PREFETCH_AXI_OFFSET 6
#define HWSEQUENCER_STATUS_PREFETCH_AXI_SIZE 1
#define HWSEQUENCER_STATUS_FETCH_AXI_OFFSET 7
#define HWSEQUENCER_STATUS_FETCH_AXI_SIZE 1
#define HWSEQUENCER_STATUS_SEQINSTR_FIFO_NOT_EMPTY_OFFSET 8
#define HWSEQUENCER_STATUS_SEQINSTR_FIFO_NOT_EMPTY_SIZE 1
#define HWSEQUENCER_STATUS_SEQINSTR_FIFO_FULL_OFFSET 9
#define HWSEQUENCER_STATUS_SEQINSTR_FIFO_FULL_SIZE 1
#define HWSEQUENCER_STATUS_WRITEBUFFER_NOT_EMPTY_OFFSET 10
#define HWSEQUENCER_STATUS_WRITEBUFFER_NOT_EMPTY_SIZE 1
#define HWSEQUENCER_STATUS_WRITEBUFFER_FULL_OFFSET 11
#define HWSEQUENCER_STATUS_WRITEBUFFER_FULL_SIZE 1
#define HWSEQUENCER_STATUS_WRITEBUFFER_AXI_OFFSET 12
#define HWSEQUENCER_STATUS_WRITEBUFFER_AXI_SIZE 1
#define HWSEQUENCER_STATUS_BARRIER_FIFO_EMPTY_OFFSET 13
#define HWSEQUENCER_STATUS_BARRIER_FIFO_EMPTY_SIZE 1
#define HWSEQUENCER_STATUS_BARRIER_FIFO_FULL_OFFSET 14
#define HWSEQUENCER_STATUS_BARRIER_FIFO_FULL_SIZE 1
#define HWSEQUENCER_STATUS_BARRIER_CACHE_DIRTY_OFFSET 15
#define HWSEQUENCER_STATUS_BARRIER_CACHE_DIRTY_SIZE 1
#define HWSEQUENCER_STATUS_BARRIER_CACHE_AXI_OFFSET 16
#define HWSEQUENCER_STATUS_BARRIER_CACHE_AXI_SIZE 1
#define HWSEQUENCER_STATUS_BRANCH_FIFIO_NOT_EMPTY_OFFSET 17
#define HWSEQUENCER_STATUS_BRANCH_FIFIO_NOT_EMPTY_SIZE 1
#define HWSEQUENCER_STATUS_BRANCH_FIFO_FULL_OFFSET 18
#define HWSEQUENCER_STATUS_BRANCH_FIFO_FULL_SIZE 1
#define HWSEQUENCER_STATUS_BRANCH_UNIT_BUSY_OFFSET 19
#define HWSEQUENCER_STATUS_BRANCH_UNIT_BUSY_SIZE 1
#define HWSEQUENCER_STATUS_BREAKPOINT_OFFSET 20
#define HWSEQUENCER_STATUS_BREAKPOINT_SIZE 1
#define HWSEQUENCER_STATUS_BREAKXCPT_OFFSET 21
#define HWSEQUENCER_STATUS_BREAKXCPT_SIZE 1
#define HWSEQUENCER_STATUS_BREAKCTI_OFFSET 22
#define HWSEQUENCER_STATUS_BREAKCTI_SIZE 1

/** bit field defines for hwsequencer_s#tefrst */
#define HWSEQUENCER_TEFRST_TEFRST_OFFSET 0
#define HWSEQUENCER_TEFRST_TEFRST_SIZE 32

/** bit field defines for hwsequencer_s#tefreadreq */
#define HWSEQUENCER_TEFREADREQ_IDX_OFFSET 0
#define HWSEQUENCER_TEFREADREQ_IDX_SIZE 5
#define HWSEQUENCER_TEFREADREQ_SECOND_OFFSET 5
#define HWSEQUENCER_TEFREADREQ_SECOND_SIZE 1

/** bit field defines for hwsequencer_s#tefval */
#define HWSEQUENCER_TEFVAL_IDX_OFFSET 0
#define HWSEQUENCER_TEFVAL_IDX_SIZE 5
#define HWSEQUENCER_TEFVAL_SECOND_OFFSET 5
#define HWSEQUENCER_TEFVAL_SECOND_SIZE 1
#define HWSEQUENCER_TEFVAL_REG_VALID_OFFSET 7
#define HWSEQUENCER_TEFVAL_REG_VALID_SIZE 1
#define HWSEQUENCER_TEFVAL_VALID_OFFSET 8
#define HWSEQUENCER_TEFVAL_VALID_SIZE 2
#define HWSEQUENCER_TEFVAL_READY_OFFSET 12
#define HWSEQUENCER_TEFVAL_READY_SIZE 2
#define HWSEQUENCER_TEFVAL_SEQUENCE_POINTER_OFFSET 16
#define HWSEQUENCER_TEFVAL_SEQUENCE_POINTER_SIZE 16

/** bit field defines for hwsequencer_s#engenaset */
#define HWSEQUENCER_ENGENASET_ENGENASET_OFFSET 0
#define HWSEQUENCER_ENGENASET_ENGENASET_SIZE 32

/** bit field defines for hwsequencer_s#engenaclr */
#define HWSEQUENCER_ENGENACLR_ENGENACLR_OFFSET 0
#define HWSEQUENCER_ENGENACLR_ENGENACLR_SIZE 32

/** bit field defines for hwsequencer_s#engenastat */
#define HWSEQUENCER_ENGENASTAT_ENGENASTAT_OFFSET 0
#define HWSEQUENCER_ENGENASTAT_ENGENASTAT_SIZE 32

/** bit field defines for hwsequencer_s#engsuspset */
#define HWSEQUENCER_ENGSUSPSET_ENGSUSPSET_OFFSET 0
#define HWSEQUENCER_ENGSUSPSET_ENGSUSPSET_SIZE 32

/** bit field defines for hwsequencer_s#engsuspclr */
#define HWSEQUENCER_ENGSUSPCLR_ENGSUSPCLR_OFFSET 0
#define HWSEQUENCER_ENGSUSPCLR_ENGSUSPCLR_SIZE 32

/** bit field defines for hwsequencer_s#engsuspstat */
#define HWSEQUENCER_ENGSUSPSTAT_ENGSUSPSTAT_OFFSET 0
#define HWSEQUENCER_ENGSUSPSTAT_ENGSUSPSTAT_SIZE 32

/** bit field defines for hwsequencer_s#seqinstrbase */
#define HWSEQUENCER_SEQINSTRBASE_POINTER_OFFSET 18
#define HWSEQUENCER_SEQINSTRBASE_POINTER_SIZE 14

/** bit field defines for hwsequencer_s#barrierbase */
#define HWSEQUENCER_BARRIERBASE_POINTER_OFFSET 14
#define HWSEQUENCER_BARRIERBASE_POINTER_SIZE 18

/** bit field defines for hwsequencer_s#lbp */
#define HWSEQUENCER_LBP_PAGE_OFFSET 8
#define HWSEQUENCER_LBP_PAGE_SIZE 4

/** bit field defines for hwsequencer_s#membase */
#define HWSEQUENCER_MEMBASE_POINTER_OFFSET 18
#define HWSEQUENCER_MEMBASE_POINTER_SIZE 14

/** bit field defines for hwsequencer_s#fltclear */
#define HWSEQUENCER_FLTCLEAR_BARRIER_AXI_OFFSET 0
#define HWSEQUENCER_FLTCLEAR_BARRIER_AXI_SIZE 1
#define HWSEQUENCER_FLTCLEAR_INSTR_AXI_OFFSET 1
#define HWSEQUENCER_FLTCLEAR_INSTR_AXI_SIZE 1
#define HWSEQUENCER_FLTCLEAR_DEV_AXI_OFFSET 2
#define HWSEQUENCER_FLTCLEAR_DEV_AXI_SIZE 1
#define HWSEQUENCER_FLTCLEAR_BRANCH_AXI_OFFSET 3
#define HWSEQUENCER_FLTCLEAR_BRANCH_AXI_SIZE 1
#define HWSEQUENCER_FLTCLEAR_CTRL_ENGFIFO_FULL_OFFSET 4
#define HWSEQUENCER_FLTCLEAR_CTRL_ENGFIFO_FULL_SIZE 1
#define HWSEQUENCER_FLTCLEAR_SEQ_ENGFIFO_FULL_OFFSET 5
#define HWSEQUENCER_FLTCLEAR_SEQ_ENGFIFO_FULL_SIZE 1
#define HWSEQUENCER_FLTCLEAR_ENGFIFO_EMPTY_OFFSET 6
#define HWSEQUENCER_FLTCLEAR_ENGFIFO_EMPTY_SIZE 1
#define HWSEQUENCER_FLTCLEAR_CTRL_BARRIER_WRITE_OFFSET 7
#define HWSEQUENCER_FLTCLEAR_CTRL_BARRIER_WRITE_SIZE 1
#define HWSEQUENCER_FLTCLEAR_SEQ_BARRIER_WRITE_OFFSET 8
#define HWSEQUENCER_FLTCLEAR_SEQ_BARRIER_WRITE_SIZE 1
#define HWSEQUENCER_FLTCLEAR_CTRL_BARRIER_IDX_OFFSET 9
#define HWSEQUENCER_FLTCLEAR_CTRL_BARRIER_IDX_SIZE 1
#define HWSEQUENCER_FLTCLEAR_SEQ_BARRIER_IDX_OFFSET 10
#define HWSEQUENCER_FLTCLEAR_SEQ_BARRIER_IDX_SIZE 1
#define HWSEQUENCER_FLTCLEAR_INSTRUCTION_OFFSET 11
#define HWSEQUENCER_FLTCLEAR_INSTRUCTION_SIZE 1
#define HWSEQUENCER_FLTCLEAR_DEADLOCK_OFFSET 12
#define HWSEQUENCER_FLTCLEAR_DEADLOCK_SIZE 1
#define HWSEQUENCER_FLTCLEAR_TRACE_AXI_OFFSET 13
#define HWSEQUENCER_FLTCLEAR_TRACE_AXI_SIZE 1
#define HWSEQUENCER_FLTCLEAR_JOBFIFO_OVERFLOW_OFFSET 14
#define HWSEQUENCER_FLTCLEAR_JOBFIFO_OVERFLOW_SIZE 1

/** bit field defines for hwsequencer_s#fltstatus */
#define HWSEQUENCER_FLTSTATUS_BARRIER_AXI_OFFSET 0
#define HWSEQUENCER_FLTSTATUS_BARRIER_AXI_SIZE 1
#define HWSEQUENCER_FLTSTATUS_INSTR_AXI_OFFSET 1
#define HWSEQUENCER_FLTSTATUS_INSTR_AXI_SIZE 1
#define HWSEQUENCER_FLTSTATUS_DEV_AXI_OFFSET 2
#define HWSEQUENCER_FLTSTATUS_DEV_AXI_SIZE 1
#define HWSEQUENCER_FLTSTATUS_BRANCH_AXI_OFFSET 3
#define HWSEQUENCER_FLTSTATUS_BRANCH_AXI_SIZE 1
#define HWSEQUENCER_FLTSTATUS_CTRL_ENGFIFO_FULL_OFFSET 4
#define HWSEQUENCER_FLTSTATUS_CTRL_ENGFIFO_FULL_SIZE 1
#define HWSEQUENCER_FLTSTATUS_SEQ_ENGFIFO_FULL_OFFSET 5
#define HWSEQUENCER_FLTSTATUS_SEQ_ENGFIFO_FULL_SIZE 1
#define HWSEQUENCER_FLTSTATUS_ENGFIFO_EMPTY_OFFSET 6
#define HWSEQUENCER_FLTSTATUS_ENGFIFO_EMPTY_SIZE 1
#define HWSEQUENCER_FLTSTATUS_CTRL_BARRIER_WRITE_OFFSET 7
#define HWSEQUENCER_FLTSTATUS_CTRL_BARRIER_WRITE_SIZE 1
#define HWSEQUENCER_FLTSTATUS_SEQ_BARRIER_WRITE_OFFSET 8
#define HWSEQUENCER_FLTSTATUS_SEQ_BARRIER_WRITE_SIZE 1
#define HWSEQUENCER_FLTSTATUS_CTRL_BARRIER_IDX_OFFSET 9
#define HWSEQUENCER_FLTSTATUS_CTRL_BARRIER_IDX_SIZE 1
#define HWSEQUENCER_FLTSTATUS_SEQ_BARRIER_IDX_OFFSET 10
#define HWSEQUENCER_FLTSTATUS_SEQ_BARRIER_IDX_SIZE 1
#define HWSEQUENCER_FLTSTATUS_INSTRUCTION_OFFSET 11
#define HWSEQUENCER_FLTSTATUS_INSTRUCTION_SIZE 1
#define HWSEQUENCER_FLTSTATUS_DEADLOCK_OFFSET 12
#define HWSEQUENCER_FLTSTATUS_DEADLOCK_SIZE 1
#define HWSEQUENCER_FLTSTATUS_TRACE_AXI_OFFSET 13
#define HWSEQUENCER_FLTSTATUS_TRACE_AXI_SIZE 1
#define HWSEQUENCER_FLTSTATUS_JOBFIFO_OVERFLOW_OFFSET 14
#define HWSEQUENCER_FLTSTATUS_JOBFIFO_OVERFLOW_SIZE 1

/** bit field defines for hwsequencer_s#fltrawstatus */
#define HWSEQUENCER_FLTRAWSTATUS_BARRIER_AXI_OFFSET 0
#define HWSEQUENCER_FLTRAWSTATUS_BARRIER_AXI_SIZE 1
#define HWSEQUENCER_FLTRAWSTATUS_INSTR_AXI_OFFSET 1
#define HWSEQUENCER_FLTRAWSTATUS_INSTR_AXI_SIZE 1
#define HWSEQUENCER_FLTRAWSTATUS_DEV_AXI_OFFSET 2
#define HWSEQUENCER_FLTRAWSTATUS_DEV_AXI_SIZE 1
#define HWSEQUENCER_FLTRAWSTATUS_BRANCH_AXI_OFFSET 3
#define HWSEQUENCER_FLTRAWSTATUS_BRANCH_AXI_SIZE 1
#define HWSEQUENCER_FLTRAWSTATUS_CTRL_ENGFIFO_FULL_OFFSET 4
#define HWSEQUENCER_FLTRAWSTATUS_CTRL_ENGFIFO_FULL_SIZE 1
#define HWSEQUENCER_FLTRAWSTATUS_SEQ_ENGFIFO_FULL_OFFSET 5
#define HWSEQUENCER_FLTRAWSTATUS_SEQ_ENGFIFO_FULL_SIZE 1
#define HWSEQUENCER_FLTRAWSTATUS_ENGFIFO_EMPTY_OFFSET 6
#define HWSEQUENCER_FLTRAWSTATUS_ENGFIFO_EMPTY_SIZE 1
#define HWSEQUENCER_FLTRAWSTATUS_CTRL_BARRIER_WRITE_OFFSET 7
#define HWSEQUENCER_FLTRAWSTATUS_CTRL_BARRIER_WRITE_SIZE 1
#define HWSEQUENCER_FLTRAWSTATUS_SEQ_BARRIER_WRITE_OFFSET 8
#define HWSEQUENCER_FLTRAWSTATUS_SEQ_BARRIER_WRITE_SIZE 1
#define HWSEQUENCER_FLTRAWSTATUS_CTRL_BARRIER_IDX_OFFSET 9
#define HWSEQUENCER_FLTRAWSTATUS_CTRL_BARRIER_IDX_SIZE 1
#define HWSEQUENCER_FLTRAWSTATUS_SEQ_BARRIER_IDX_OFFSET 10
#define HWSEQUENCER_FLTRAWSTATUS_SEQ_BARRIER_IDX_SIZE 1
#define HWSEQUENCER_FLTRAWSTATUS_INSTRUCTION_OFFSET 11
#define HWSEQUENCER_FLTRAWSTATUS_INSTRUCTION_SIZE 1
#define HWSEQUENCER_FLTRAWSTATUS_DEADLOCK_OFFSET 12
#define HWSEQUENCER_FLTRAWSTATUS_DEADLOCK_SIZE 1
#define HWSEQUENCER_FLTRAWSTATUS_TRACE_AXI_OFFSET 13
#define HWSEQUENCER_FLTRAWSTATUS_TRACE_AXI_SIZE 1
#define HWSEQUENCER_FLTRAWSTATUS_JOBFIFO_OVERFLOW_OFFSET 14
#define HWSEQUENCER_FLTRAWSTATUS_JOBFIFO_OVERFLOW_SIZE 1

/** bit field defines for hwsequencer_s#fltmaskclr */
#define HWSEQUENCER_FLTMASKCLR_BARRIER_AXI_OFFSET 0
#define HWSEQUENCER_FLTMASKCLR_BARRIER_AXI_SIZE 1
#define HWSEQUENCER_FLTMASKCLR_INSTR_AXI_OFFSET 1
#define HWSEQUENCER_FLTMASKCLR_INSTR_AXI_SIZE 1
#define HWSEQUENCER_FLTMASKCLR_DEV_AXI_OFFSET 2
#define HWSEQUENCER_FLTMASKCLR_DEV_AXI_SIZE 1
#define HWSEQUENCER_FLTMASKCLR_BRANCH_AXI_OFFSET 3
#define HWSEQUENCER_FLTMASKCLR_BRANCH_AXI_SIZE 1
#define HWSEQUENCER_FLTMASKCLR_CTRL_ENGFIFO_FULL_OFFSET 4
#define HWSEQUENCER_FLTMASKCLR_CTRL_ENGFIFO_FULL_SIZE 1
#define HWSEQUENCER_FLTMASKCLR_SEQ_ENGFIFO_FULL_OFFSET 5
#define HWSEQUENCER_FLTMASKCLR_SEQ_ENGFIFO_FULL_SIZE 1
#define HWSEQUENCER_FLTMASKCLR_ENGFIFO_EMPTY_OFFSET 6
#define HWSEQUENCER_FLTMASKCLR_ENGFIFO_EMPTY_SIZE 1
#define HWSEQUENCER_FLTMASKCLR_CTRL_BARRIER_WRITE_OFFSET 7
#define HWSEQUENCER_FLTMASKCLR_CTRL_BARRIER_WRITE_SIZE 1
#define HWSEQUENCER_FLTMASKCLR_SEQ_BARRIER_WRITE_OFFSET 8
#define HWSEQUENCER_FLTMASKCLR_SEQ_BARRIER_WRITE_SIZE 1
#define HWSEQUENCER_FLTMASKCLR_CTRL_BARRIER_IDX_OFFSET 9
#define HWSEQUENCER_FLTMASKCLR_CTRL_BARRIER_IDX_SIZE 1
#define HWSEQUENCER_FLTMASKCLR_SEQ_BARRIER_IDX_OFFSET 10
#define HWSEQUENCER_FLTMASKCLR_SEQ_BARRIER_IDX_SIZE 1
#define HWSEQUENCER_FLTMASKCLR_INSTRUCTION_OFFSET 11
#define HWSEQUENCER_FLTMASKCLR_INSTRUCTION_SIZE 1
#define HWSEQUENCER_FLTMASKCLR_DEADLOCK_OFFSET 12
#define HWSEQUENCER_FLTMASKCLR_DEADLOCK_SIZE 1
#define HWSEQUENCER_FLTMASKCLR_TRACE_AXI_OFFSET 13
#define HWSEQUENCER_FLTMASKCLR_TRACE_AXI_SIZE 1
#define HWSEQUENCER_FLTMASKCLR_JOBFIFO_OVERFLOW_OFFSET 14
#define HWSEQUENCER_FLTMASKCLR_JOBFIFO_OVERFLOW_SIZE 1

/** bit field defines for hwsequencer_s#fltmaskset */
#define HWSEQUENCER_FLTMASKSET_BARRIER_AXI_OFFSET 0
#define HWSEQUENCER_FLTMASKSET_BARRIER_AXI_SIZE 1
#define HWSEQUENCER_FLTMASKSET_INSTR_AXI_OFFSET 1
#define HWSEQUENCER_FLTMASKSET_INSTR_AXI_SIZE 1
#define HWSEQUENCER_FLTMASKSET_DEV_AXI_OFFSET 2
#define HWSEQUENCER_FLTMASKSET_DEV_AXI_SIZE 1
#define HWSEQUENCER_FLTMASKSET_BRANCH_AXI_OFFSET 3
#define HWSEQUENCER_FLTMASKSET_BRANCH_AXI_SIZE 1
#define HWSEQUENCER_FLTMASKSET_CTRL_ENGFIFO_FULL_OFFSET 4
#define HWSEQUENCER_FLTMASKSET_CTRL_ENGFIFO_FULL_SIZE 1
#define HWSEQUENCER_FLTMASKSET_SEQ_ENGFIFO_FULL_OFFSET 5
#define HWSEQUENCER_FLTMASKSET_SEQ_ENGFIFO_FULL_SIZE 1
#define HWSEQUENCER_FLTMASKSET_ENGFIFO_EMPTY_OFFSET 6
#define HWSEQUENCER_FLTMASKSET_ENGFIFO_EMPTY_SIZE 1
#define HWSEQUENCER_FLTMASKSET_CTRL_BARRIER_WRITE_OFFSET 7
#define HWSEQUENCER_FLTMASKSET_CTRL_BARRIER_WRITE_SIZE 1
#define HWSEQUENCER_FLTMASKSET_SEQ_BARRIER_WRITE_OFFSET 8
#define HWSEQUENCER_FLTMASKSET_SEQ_BARRIER_WRITE_SIZE 1
#define HWSEQUENCER_FLTMASKSET_CTRL_BARRIER_IDX_OFFSET 9
#define HWSEQUENCER_FLTMASKSET_CTRL_BARRIER_IDX_SIZE 1
#define HWSEQUENCER_FLTMASKSET_SEQ_BARRIER_IDX_OFFSET 10
#define HWSEQUENCER_FLTMASKSET_SEQ_BARRIER_IDX_SIZE 1
#define HWSEQUENCER_FLTMASKSET_INSTRUCTION_OFFSET 11
#define HWSEQUENCER_FLTMASKSET_INSTRUCTION_SIZE 1
#define HWSEQUENCER_FLTMASKSET_DEADLOCK_OFFSET 12
#define HWSEQUENCER_FLTMASKSET_DEADLOCK_SIZE 1
#define HWSEQUENCER_FLTMASKSET_TRACE_AXI_OFFSET 13
#define HWSEQUENCER_FLTMASKSET_TRACE_AXI_SIZE 1
#define HWSEQUENCER_FLTMASKSET_JOBFIFO_OVERFLOW_OFFSET 14
#define HWSEQUENCER_FLTMASKSET_JOBFIFO_OVERFLOW_SIZE 1

/** bit field defines for hwsequencer_s#fltmaskstatus */
#define HWSEQUENCER_FLTMASKSTATUS_BARRIER_AXI_OFFSET 0
#define HWSEQUENCER_FLTMASKSTATUS_BARRIER_AXI_SIZE 1
#define HWSEQUENCER_FLTMASKSTATUS_INSTR_AXI_OFFSET 1
#define HWSEQUENCER_FLTMASKSTATUS_INSTR_AXI_SIZE 1
#define HWSEQUENCER_FLTMASKSTATUS_DEV_AXI_OFFSET 2
#define HWSEQUENCER_FLTMASKSTATUS_DEV_AXI_SIZE 1
#define HWSEQUENCER_FLTMASKSTATUS_BRANCH_AXI_OFFSET 3
#define HWSEQUENCER_FLTMASKSTATUS_BRANCH_AXI_SIZE 1
#define HWSEQUENCER_FLTMASKSTATUS_CTRL_ENGFIFO_FULL_OFFSET 4
#define HWSEQUENCER_FLTMASKSTATUS_CTRL_ENGFIFO_FULL_SIZE 1
#define HWSEQUENCER_FLTMASKSTATUS_SEQ_ENGFIFO_FULL_OFFSET 5
#define HWSEQUENCER_FLTMASKSTATUS_SEQ_ENGFIFO_FULL_SIZE 1
#define HWSEQUENCER_FLTMASKSTATUS_ENGFIFO_EMPTY_OFFSET 6
#define HWSEQUENCER_FLTMASKSTATUS_ENGFIFO_EMPTY_SIZE 1
#define HWSEQUENCER_FLTMASKSTATUS_CTRL_BARRIER_WRITE_OFFSET 7
#define HWSEQUENCER_FLTMASKSTATUS_CTRL_BARRIER_WRITE_SIZE 1
#define HWSEQUENCER_FLTMASKSTATUS_SEQ_BARRIER_WRITE_OFFSET 8
#define HWSEQUENCER_FLTMASKSTATUS_SEQ_BARRIER_WRITE_SIZE 1
#define HWSEQUENCER_FLTMASKSTATUS_CTRL_BARRIER_IDX_OFFSET 9
#define HWSEQUENCER_FLTMASKSTATUS_CTRL_BARRIER_IDX_SIZE 1
#define HWSEQUENCER_FLTMASKSTATUS_SEQ_BARRIER_IDX_OFFSET 10
#define HWSEQUENCER_FLTMASKSTATUS_SEQ_BARRIER_IDX_SIZE 1
#define HWSEQUENCER_FLTMASKSTATUS_INSTRUCTION_OFFSET 11
#define HWSEQUENCER_FLTMASKSTATUS_INSTRUCTION_SIZE 1
#define HWSEQUENCER_FLTMASKSTATUS_DEADLOCK_OFFSET 12
#define HWSEQUENCER_FLTMASKSTATUS_DEADLOCK_SIZE 1
#define HWSEQUENCER_FLTMASKSTATUS_TRACE_AXI_OFFSET 13
#define HWSEQUENCER_FLTMASKSTATUS_TRACE_AXI_SIZE 1
#define HWSEQUENCER_FLTMASKSTATUS_JOBFIFO_OVERFLOW_OFFSET 14
#define HWSEQUENCER_FLTMASKSTATUS_JOBFIFO_OVERFLOW_SIZE 1

/** bit field defines for hwsequencer_s#fltsigbarrier */
#define HWSEQUENCER_FLTSIGBARRIER_IDX_OFFSET 0
#define HWSEQUENCER_FLTSIGBARRIER_IDX_SIZE 12

/** bit field defines for hwsequencer_s#fltsigeng */
#define HWSEQUENCER_FLTSIGENG_IDX_OFFSET 0
#define HWSEQUENCER_FLTSIGENG_IDX_SIZE 6

/** bit field defines for hwsequencer_s#fltsigaddr */
#define HWSEQUENCER_FLTSIGADDR_FLTSIGADDR_OFFSET 0
#define HWSEQUENCER_FLTSIGADDR_FLTSIGADDR_SIZE 32

/** bit field defines for hwsequencer_s#fltseqptr */
#define HWSEQUENCER_FLTSEQPTR_SEQUENCE_POINTER_OFFSET 0
#define HWSEQUENCER_FLTSEQPTR_SEQUENCE_POINTER_SIZE 18

/** bit field defines for hwsequencer_s#config */
#define HWSEQUENCER_CONFIG_HIGHEST_ENGINE_IDX_OFFSET 0
#define HWSEQUENCER_CONFIG_HIGHEST_ENGINE_IDX_SIZE 6

/** bit field defines for hwsequencer_s#peripheralid4 */
#define HWSEQUENCER_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define HWSEQUENCER_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for hwsequencer_s#peripheralid5 */
#define HWSEQUENCER_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define HWSEQUENCER_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for hwsequencer_s#peripheralid6 */
#define HWSEQUENCER_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define HWSEQUENCER_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for hwsequencer_s#peripheralid7 */
#define HWSEQUENCER_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define HWSEQUENCER_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for hwsequencer_s#peripheralid0 */
#define HWSEQUENCER_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define HWSEQUENCER_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for hwsequencer_s#peripheralid1 */
#define HWSEQUENCER_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define HWSEQUENCER_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for hwsequencer_s#peripheralid2 */
#define HWSEQUENCER_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define HWSEQUENCER_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for hwsequencer_s#peripheralid3 */
#define HWSEQUENCER_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define HWSEQUENCER_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for hwsequencer_s#componentid0 */
#define HWSEQUENCER_COMPONENTID0_COMPONENTID0_OFFSET 0
#define HWSEQUENCER_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for hwsequencer_s#componentid1 */
#define HWSEQUENCER_COMPONENTID1_COMPONENTID1_OFFSET 0
#define HWSEQUENCER_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for hwsequencer_s#componentid2 */
#define HWSEQUENCER_COMPONENTID2_COMPONENTID2_OFFSET 0
#define HWSEQUENCER_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for hwsequencer_s#componentid3 */
#define HWSEQUENCER_COMPONENTID3_COMPONENTID3_OFFSET 0
#define HWSEQUENCER_COMPONENTID3_COMPONENTID3_SIZE 8

/** bit field defines for hwsequencer_s#bar_init_array */
#define HWSEQUENCER_BAR_INIT_ARRAY_SEQUENCE_PARAMETER_OFFSET 0
#define HWSEQUENCER_BAR_INIT_ARRAY_SEQUENCE_PARAMETER_SIZE 8
#define HWSEQUENCER_BAR_INIT_ARRAY_SEQUENCE_INITVALUE_OFFSET 12
#define HWSEQUENCER_BAR_INIT_ARRAY_SEQUENCE_INITVALUE_SIZE 4
#define HWSEQUENCER_BAR_INIT_ARRAY_SEQUENCE_POINTER_OFFSET 16
#define HWSEQUENCER_BAR_INIT_ARRAY_SEQUENCE_POINTER_SIZE 16

/** bit field defines for hwsequencer_s#bar_set_array */
#define HWSEQUENCER_BAR_SET_ARRAY_SEQUENCE_PARAMETER_OFFSET 0
#define HWSEQUENCER_BAR_SET_ARRAY_SEQUENCE_PARAMETER_SIZE 8
#define HWSEQUENCER_BAR_SET_ARRAY_SEQUENCE_POINTER_OFFSET 16
#define HWSEQUENCER_BAR_SET_ARRAY_SEQUENCE_POINTER_SIZE 16

/** bit field defines for hwsequencer_s#bar_dec_array */
#define HWSEQUENCER_BAR_DEC_ARRAY_BAR_DEC_ARRAY_OFFSET 0
#define HWSEQUENCER_BAR_DEC_ARRAY_BAR_DEC_ARRAY_SIZE 32

/* EOF hwsequencer.h */
#endif
