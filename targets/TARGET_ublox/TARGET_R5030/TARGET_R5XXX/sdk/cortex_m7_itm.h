#ifndef INCLUDED_CORTEX_M7_ITM
#define INCLUDED_CORTEX_M7_ITM
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>


/** Instrumentation trace Macrocell registers
*/
struct cortex_m7_itm_s {
   /** ITM Stimulus Port. Each of the 32 stimulus ports has its own address. A write to one of these locations causes data to be written into the FIFO if the corresponding bit in the Trace Enable Register is set. Reading from any of the stimulus ports returns the FIFO status in bit [0]: 0 = full, 1 = not full. The polled FIFO interface does not provide an atomic read-modify-write, so you must use the Cortex-M3 exclusive monitor if a polled printf is used concurrently with ITM usage by interrupts or other threads. at address offset 0x000, read-write */
   uint32_t itm_stim[32];
   /** Reserved space */
   uint8_t fill0[3456];
   /** ITM Trace Enable Register. Use the Trace Enable Register to generate trace data by writing to the corresponding stimulus port. Note: Privileged writes are accepted to this register if ITMENA is set. User writes are accepted to this register if ITMENA is set and the appropriate privilege mask is cleared. Privileged access to the stimulus ports enables an RTOS kernel to guarantee instrumentation slots or bandwidth as required. at address offset 0xE00, read-write */
   uint32_t itm_ter;
   /** Reserved space */
   uint8_t fill1[60];
   /** ITM Trace Privilege Register. Use the ITM Trace Privilege Register to enable an operating system to control which stimulus ports are accessible by user code. Note: You can only write to this register in privileged mode. at address offset 0xE40, read-write */
   uint32_t itm_tpr;
   /** Reserved space */
   uint8_t fill2[60];
   /** ITM Trace Control Register. Use this register to configure and control ITM transfers. Note 1: You can only write to this register in privilege mode. Note 2: DWT is not enabled in the ITM block. However, DWT stimulus entry into the FIFO is controlled by DWTENA. If DWT requires timestamping, the TSSEN bit must be set. at address offset 0xE80, read-write */
   uint32_t itm_tcr;
   /** Reserved space */
   uint8_t fill3[108];
   /** Integration mode: read ATB ready at address offset 0xEF0, read-only */
   uint32_t itm_itatrdy;
   /** Reserved space */
   uint8_t fill4[4];
   /** Integration mode: write ATB valid at address offset 0xEF8, write-only */
   uint32_t itm_itatval;
   /** Reserved space */
   uint8_t fill5[4];
   /** Integration mode control register at address offset 0xF00, read-write */
   uint32_t itm_tctrl;
   /** Reserved space */
   uint8_t fill6[172];
   /** ITM Lock Access Register. Use this register to prevent write accesses to the Control Register. at address offset 0xFB0, write-only */
   uint32_t itm_lar;
   /** ITM Lock Status Register. Use this register to enable write accesses to the Control Register. at address offset 0xFB4, read-only */
   uint32_t itm_lsr;
   /** Reserved space */
   uint8_t fill7[24];
   /** Peripheral identification register (PID4). at address offset 0xFD0, read-only */
   uint32_t pid4;
   /** Peripheral identification register (PID5). at address offset 0xFD4, read-only */
   uint32_t pid5;
   /** Peripheral identification register (PID6). at address offset 0xFD8, read-only */
   uint32_t pid6;
   /** Peripheral identification register (PID7). at address offset 0xFDC, read-only */
   uint32_t pid7;
   /** Peripheral identification register Bits 7:0 (PID0). at address offset 0xFE0, read-only */
   uint32_t pid0;
   /** Peripheral identification register Bits 15:8 (PID1). at address offset 0xFE4, read-only */
   uint32_t pid1;
   /** Peripheral identification register Bits 23:16 (PID2). at address offset 0xFE8, read-only */
   uint32_t pid2;
   /** Peripheral identification register Bits 31:24 (PID3). at address offset 0xFEC, read-only */
   uint32_t pid3;
   /** Component identification register Bits 7:0 (CID0). at address offset 0xFF0, read-only */
   uint32_t cid0;
   /** Component identification register Bits 15:8 (CID1). at address offset 0xFF4, read-only */
   uint32_t cid1;
   /** Component identification register Bits 23:16 (CID2). at address offset 0xFF8, read-only */
   uint32_t cid2;
   /** Component identification register Bits 31:24 (CID3). at address offset 0xFFC, read-only */
   uint32_t cid3;
};

/** bit field defines for cortex_m7_itm_s#itm_stim */
#define CORTEX_M7_ITM_ITM_STIM_FIFOREADY_OFFSET 0
#define CORTEX_M7_ITM_ITM_STIM_FIFOREADY_SIZE 1
#define CORTEX_M7_ITM_ITM_STIM_STIMULUS_OFFSET 1
#define CORTEX_M7_ITM_ITM_STIM_STIMULUS_SIZE 31

/** bit field defines for cortex_m7_itm_s#itm_ter */
#define CORTEX_M7_ITM_ITM_TER_STIMENA_OFFSET 0
#define CORTEX_M7_ITM_ITM_TER_STIMENA_SIZE 32

/** bit field defines for cortex_m7_itm_s#itm_tpr */
#define CORTEX_M7_ITM_ITM_TPR_PRIVMASK_OFFSET 0
#define CORTEX_M7_ITM_ITM_TPR_PRIVMASK_SIZE 4

/** bit field defines for cortex_m7_itm_s#itm_tcr */
#define CORTEX_M7_ITM_ITM_TCR_ITMENA_OFFSET 0
#define CORTEX_M7_ITM_ITM_TCR_ITMENA_SIZE 1
#define CORTEX_M7_ITM_ITM_TCR_TSENA_OFFSET 1
#define CORTEX_M7_ITM_ITM_TCR_TSENA_SIZE 1
#define CORTEX_M7_ITM_ITM_TCR_SYNCENA_OFFSET 2
#define CORTEX_M7_ITM_ITM_TCR_SYNCENA_SIZE 1
#define CORTEX_M7_ITM_ITM_TCR_DWTENA_OFFSET 3
#define CORTEX_M7_ITM_ITM_TCR_DWTENA_SIZE 1
#define CORTEX_M7_ITM_ITM_TCR_SWOENA_OFFSET 4
#define CORTEX_M7_ITM_ITM_TCR_SWOENA_SIZE 1
#define CORTEX_M7_ITM_ITM_TCR_TSPRESCALE_OFFSET 8
#define CORTEX_M7_ITM_ITM_TCR_TSPRESCALE_SIZE 2
#define CORTEX_M7_ITM_ITM_TCR_GTSFREQ_OFFSET 10
#define CORTEX_M7_ITM_ITM_TCR_GTSFREQ_SIZE 2
#define CORTEX_M7_ITM_ITM_TCR_ATBID_OFFSET 16
#define CORTEX_M7_ITM_ITM_TCR_ATBID_SIZE 7
#define CORTEX_M7_ITM_ITM_TCR_BUSY_OFFSET 23
#define CORTEX_M7_ITM_ITM_TCR_BUSY_SIZE 1

/** bit field defines for cortex_m7_itm_s#itm_itatrdy */
#define CORTEX_M7_ITM_ITM_ITATRDY_ITM_ITATRDY_OFFSET 0
#define CORTEX_M7_ITM_ITM_ITATRDY_ITM_ITATRDY_SIZE 32

/** bit field defines for cortex_m7_itm_s#itm_itatval */
#define CORTEX_M7_ITM_ITM_ITATVAL_ITM_ITATVAL_OFFSET 0
#define CORTEX_M7_ITM_ITM_ITATVAL_ITM_ITATVAL_SIZE 32

/** bit field defines for cortex_m7_itm_s#itm_tctrl */
#define CORTEX_M7_ITM_ITM_TCTRL_ITM_TCTRL_OFFSET 0
#define CORTEX_M7_ITM_ITM_TCTRL_ITM_TCTRL_SIZE 32

/** bit field defines for cortex_m7_itm_s#itm_lar */
#define CORTEX_M7_ITM_ITM_LAR_ITM_LAR_OFFSET 0
#define CORTEX_M7_ITM_ITM_LAR_ITM_LAR_SIZE 32

/** bit field defines for cortex_m7_itm_s#itm_lsr */
#define CORTEX_M7_ITM_ITM_LSR_ITM_LSR_OFFSET 0
#define CORTEX_M7_ITM_ITM_LSR_ITM_LSR_SIZE 32

/** bit field defines for cortex_m7_itm_s#pid4 */
#define CORTEX_M7_ITM_PID4_PID4_OFFSET 0
#define CORTEX_M7_ITM_PID4_PID4_SIZE 32

/** bit field defines for cortex_m7_itm_s#pid5 */
#define CORTEX_M7_ITM_PID5_PID5_OFFSET 0
#define CORTEX_M7_ITM_PID5_PID5_SIZE 32

/** bit field defines for cortex_m7_itm_s#pid6 */
#define CORTEX_M7_ITM_PID6_PID6_OFFSET 0
#define CORTEX_M7_ITM_PID6_PID6_SIZE 32

/** bit field defines for cortex_m7_itm_s#pid7 */
#define CORTEX_M7_ITM_PID7_PID7_OFFSET 0
#define CORTEX_M7_ITM_PID7_PID7_SIZE 32

/** bit field defines for cortex_m7_itm_s#pid0 */
#define CORTEX_M7_ITM_PID0_PID0_OFFSET 0
#define CORTEX_M7_ITM_PID0_PID0_SIZE 32

/** bit field defines for cortex_m7_itm_s#pid1 */
#define CORTEX_M7_ITM_PID1_PID1_OFFSET 0
#define CORTEX_M7_ITM_PID1_PID1_SIZE 32

/** bit field defines for cortex_m7_itm_s#pid2 */
#define CORTEX_M7_ITM_PID2_PID2_OFFSET 0
#define CORTEX_M7_ITM_PID2_PID2_SIZE 32

/** bit field defines for cortex_m7_itm_s#pid3 */
#define CORTEX_M7_ITM_PID3_PID3_OFFSET 0
#define CORTEX_M7_ITM_PID3_PID3_SIZE 32

/** bit field defines for cortex_m7_itm_s#cid0 */
#define CORTEX_M7_ITM_CID0_CID0_OFFSET 0
#define CORTEX_M7_ITM_CID0_CID0_SIZE 32

/** bit field defines for cortex_m7_itm_s#cid1 */
#define CORTEX_M7_ITM_CID1_CID1_OFFSET 0
#define CORTEX_M7_ITM_CID1_CID1_SIZE 32

/** bit field defines for cortex_m7_itm_s#cid2 */
#define CORTEX_M7_ITM_CID2_CID2_OFFSET 0
#define CORTEX_M7_ITM_CID2_CID2_SIZE 32

/** bit field defines for cortex_m7_itm_s#cid3 */
#define CORTEX_M7_ITM_CID3_CID3_OFFSET 0
#define CORTEX_M7_ITM_CID3_CID3_SIZE 32

/* EOF cortex_m7_itm.h */
#endif
