#ifndef INCLUDED_VCU_CR
#define INCLUDED_VCU_CR
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** Number of VSP Cores */
#define VSP_NRDE 1
#define VCU_CR_PERIPHERALID4 0x07
#define VCU_CR_PERIPHERALID5 0x00
#define VCU_CR_PERIPHERALID6 0x00
#define VCU_CR_PERIPHERALID7 0x00
#define VCU_CR_PERIPHERALID0 0x02
#define VCU_CR_PERIPHERALID1 0xDA
#define VCU_CR_PERIPHERALID2 0x08
#define VCU_CR_PERIPHERALID3 0x00
#define VCU_CR_COMPONENTID0 0x0D
#define VCU_CR_COMPONENTID1 0xF0
#define VCU_CR_COMPONENTID2 0x05
#define VCU_CR_COMPONENTID3 0xB1

/** VSP Control Unit
*/
struct vcu_cr_s {
   /** Reserved space */
   uint8_t fill0[16];
   /** nr of DE units implemented at address offset 0x010, read-only */
   uint32_t nrvsp;
   /** nr of VDMA units implemented at address offset 0x014, read-only */
   uint32_t nrvdma;
   /** Reserved space */
   uint8_t fill1[8];
   /** DMEM Memory Size at address offset 0x020, read-only */
   uint32_t dmemsize;
   /** nr of DMEM banks at address offset 0x024, read-only */
   uint32_t dmembanks;
   /** Reserved space */
   uint8_t fill2[32];
   /** Memory Disable Status Bits at address offset 0x048, read-only */
   uint32_t memdisstat;
   /** Reserved space */
   uint8_t fill3[20];
   /** VSP CORE ITCM Lightsleep controller threshold value. at address offset 0x060, read-write */
   uint32_t itcm_ls_ctrl;
   /** Reserved space */
   uint8_t fill4[12];
   /** VSP CORE DTCM Lightsleep controller threshold value. at address offset 0x070, read-write */
   uint32_t dtcm_ls_ctrl;
   /** Reserved space */
   uint8_t fill5[12];
   /** DMEM Bank 0/1 Lightsleep controller threshold value at address offset 0x080, read-write */
   uint32_t dmem0to1_ls_ctrl_thresh;
   /** Reserved space */
   uint8_t fill6[4];
   /** DMEM Bank 2/3 Lightsleep controller threshold value at address offset 0x088, read-write */
   uint32_t dmem2to3_ls_ctrl_thresh;
   /** Reserved space */
   uint8_t fill7[3908];
   /** Peripheral ID4 Register at address offset 0xFD0, read-only constant 0x00000007 */
   uint32_t peripheralid4;
   /** Peripheral ID5 Register at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** Peripheral ID6 Register at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** Peripheral ID7 Register at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** Peripheral ID0 Register at address offset 0xFE0, read-only constant 0x00000002 */
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

/** bit field defines for vcu_cr_s#nrvsp */
#define VCU_CR_NRVSP_NRVSP_OFFSET 0
#define VCU_CR_NRVSP_NRVSP_SIZE 32

/** bit field defines for vcu_cr_s#nrvdma */
#define VCU_CR_NRVDMA_NRVDMA_OFFSET 0
#define VCU_CR_NRVDMA_NRVDMA_SIZE 32

/** bit field defines for vcu_cr_s#dmemsize */
#define VCU_CR_DMEMSIZE_DMEMSIZE_OFFSET 0
#define VCU_CR_DMEMSIZE_DMEMSIZE_SIZE 32

/** bit field defines for vcu_cr_s#dmembanks */
#define VCU_CR_DMEMBANKS_DMEMBANKS_OFFSET 0
#define VCU_CR_DMEMBANKS_DMEMBANKS_SIZE 32

/** bit field defines for vcu_cr_s#memdisstat */
#define VCU_CR_MEMDISSTAT_MEMDISSTAT_OFFSET 0
#define VCU_CR_MEMDISSTAT_MEMDISSTAT_SIZE 32

/** bit field defines for vcu_cr_s#itcm_ls_ctrl */
#define VCU_CR_ITCM_LS_CTRL_LS_CTRL_THRESH_OFFSET 0
#define VCU_CR_ITCM_LS_CTRL_LS_CTRL_THRESH_SIZE 3

/** bit field defines for vcu_cr_s#dtcm_ls_ctrl */
#define VCU_CR_DTCM_LS_CTRL_LS_CTRL_THRESH_OFFSET 0
#define VCU_CR_DTCM_LS_CTRL_LS_CTRL_THRESH_SIZE 3

/** bit field defines for vcu_cr_s#dmem0to1_ls_ctrl_thresh */
#define VCU_CR_DMEM0TO1_LS_CTRL_THRESH_LS_CTRL_THRESH_OFFSET 0
#define VCU_CR_DMEM0TO1_LS_CTRL_THRESH_LS_CTRL_THRESH_SIZE 3

/** bit field defines for vcu_cr_s#dmem2to3_ls_ctrl_thresh */
#define VCU_CR_DMEM2TO3_LS_CTRL_THRESH_LS_CTRL_THRESH_OFFSET 0
#define VCU_CR_DMEM2TO3_LS_CTRL_THRESH_LS_CTRL_THRESH_SIZE 3

/** bit field defines for vcu_cr_s#peripheralid4 */
#define VCU_CR_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define VCU_CR_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for vcu_cr_s#peripheralid5 */
#define VCU_CR_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define VCU_CR_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for vcu_cr_s#peripheralid6 */
#define VCU_CR_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define VCU_CR_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for vcu_cr_s#peripheralid7 */
#define VCU_CR_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define VCU_CR_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for vcu_cr_s#peripheralid0 */
#define VCU_CR_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define VCU_CR_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for vcu_cr_s#peripheralid1 */
#define VCU_CR_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define VCU_CR_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for vcu_cr_s#peripheralid2 */
#define VCU_CR_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define VCU_CR_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for vcu_cr_s#peripheralid3 */
#define VCU_CR_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define VCU_CR_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for vcu_cr_s#componentid0 */
#define VCU_CR_COMPONENTID0_COMPONENTID0_OFFSET 0
#define VCU_CR_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for vcu_cr_s#componentid1 */
#define VCU_CR_COMPONENTID1_COMPONENTID1_OFFSET 0
#define VCU_CR_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for vcu_cr_s#componentid2 */
#define VCU_CR_COMPONENTID2_COMPONENTID2_OFFSET 0
#define VCU_CR_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for vcu_cr_s#componentid3 */
#define VCU_CR_COMPONENTID3_COMPONENTID3_OFFSET 0
#define VCU_CR_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF vcu_cr.h */
#endif
