#ifndef INCLUDED_NIC400_MODEM_IC
#define INCLUDED_NIC400_MODEM_IC
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** ID mask for  PHYCPU AHB master port */
#define NIC_AHB32_PHYCPUM_ID_MASK 0
/** ID value for PHYCPU AHB master port */
#define NIC_AHB32_PHYCPUM_ID_VALUE 0
/** ID mask for  HWAcc AXI master port */
#define NIC_AXI32_HWACCM_ID_MASK 0
/** ID value for HWAcc AXI master port */
#define NIC_AXI32_HWACCM_ID_VALUE 0
/** ID mask for  HWSeq AXI master port */
#define NIC_AXI32_HWSEQM_ID_MASK 0
/** ID value for HWSeq AXI master port */
#define NIC_AXI32_HWSEQM_ID_VALUE 0
/** ID mask for  Main fabric AXI master port */
#define NIC_AXI32_MAINM_ID_MASK 0
/** ID value for Main fabric AXI master port */
#define NIC_AXI32_MAINM_ID_VALUE 0
/** ID mask for  VDMA AXI master port */
#define NIC_AXI32_VDMAM_ID_MASK 0
/** ID value for VDMA AXI master port */
#define NIC_AXI32_VDMAM_ID_VALUE 0
/** ID mask for  VSP AXI master port */
#define NIC_AXI32_VSPM_ID_MASK 0
/** ID value for VSP AXI master port */
#define NIC_AXI32_VSPM_ID_VALUE 0
/** ID mask for  MDMCPU AXI master port */
#define NIC_AXI64_MDMCPUM_ID_MASK 0
/** ID value for MDMCPU AXI master port */
#define NIC_AXI64_MDMCPUM_ID_VALUE 0

struct nic400_modem_ic_s {
   /** Reserved space */
   uint8_t fill0[8144];
   /**  at address offset 0x1FD0, read-only */
   uint32_t peripheralid4;
   /**  at address offset 0x1FD4, read-only */
   uint32_t peripheralid5;
   /**  at address offset 0x1FD8, read-only */
   uint32_t peripheralid6;
   /**  at address offset 0x1FDC, read-only */
   uint32_t peripheralid7;
   /**  at address offset 0x1FE0, read-only */
   uint32_t peripheralid0;
   /**  at address offset 0x1FE4, read-only */
   uint32_t peripheralid1;
   /**  at address offset 0x1FE8, read-only */
   uint32_t peripheralid2;
   /**  at address offset 0x1FEC, read-only */
   uint32_t peripheralid3;
   /**  at address offset 0x1FF0, read-only */
   uint32_t componentid0;
   /**  at address offset 0x1FF4, read-only */
   uint32_t componentid1;
   /**  at address offset 0x1FF8, read-only */
   uint32_t componentid2;
   /**  at address offset 0x1FFC, read-only */
   uint32_t componentid3;
   /** Reserved space */
   uint8_t fill1[8];
   /**  at address offset 0x2008, read-write */
   uint32_t axi32_mdmcpus_ib_fn_mod_iss_bm;
   /** Reserved space */
   uint8_t fill2[252];
   /**  at address offset 0x2108, read-write */
   uint32_t axi32_mdmcpus_ib_fn_mod;
   /** Reserved space */
   uint8_t fill3[3836];
   /**  at address offset 0x3008, read-write */
   uint32_t axi32_vsps_fn_mod_iss_bm;
   /** Reserved space */
   uint8_t fill4[4092];
   /**  at address offset 0x4008, read-write */
   uint32_t axi32_tcus_fn_mod_iss_bm;
   /** Reserved space */
   uint8_t fill5[4092];
   /**  at address offset 0x5008, read-write */
   uint32_t axi32_phycpus_ib_fn_mod_iss_bm;
   /** Reserved space */
   uint8_t fill6[252];
   /**  at address offset 0x5108, read-write */
   uint32_t axi32_phycpus_ib_fn_mod;
   /** Reserved space */
   uint8_t fill7[3836];
   /**  at address offset 0x6008, read-write */
   uint32_t axi32_ahbics_ib_fn_mod_iss_bm;
   /** Reserved space */
   uint8_t fill8[252];
   /**  at address offset 0x6108, read-write */
   uint32_t axi32_ahbics_ib_fn_mod;
   /** Reserved space */
   uint8_t fill9[3836];
   /**  at address offset 0x7008, read-write */
   uint32_t axi32_hwseqs_fn_mod_iss_bm;
   /** Reserved space */
   uint8_t fill10[4092];
   /**  at address offset 0x8008, read-write */
   uint32_t axi64_mains_fn_mod_iss_bm;
   /** Reserved space */
   uint8_t fill11[237820];
   /**  at address offset 0x42108, read-write */
   uint32_t axi64_mdmcpum_fn_mod;
   /** Reserved space */
   uint8_t fill12[4092];
   /**  at address offset 0x43108, read-write */
   uint32_t axi32_mainm_fn_mod;
   /** Reserved space */
   uint8_t fill13[4092];
   /**  at address offset 0x44108, read-write */
   uint32_t axi32_vdmam_fn_mod;
   /** Reserved space */
   uint8_t fill14[4092];
   /**  at address offset 0x45108, read-write */
   uint32_t axi32_hwaccm_fn_mod;
   /** Reserved space */
   uint8_t fill15[4092];
   /**  at address offset 0x46108, read-write */
   uint32_t axi32_vspm_fn_mod;
   /** Reserved space */
   uint8_t fill16[3868];
   /**  at address offset 0x47028, read-write */
   uint32_t ahb32_phycpum_fn_mod_ahb;
   /** Reserved space */
   uint8_t fill17[220];
   /**  at address offset 0x47108, read-write */
   uint32_t ahb32_phycpum_ib_fn_mod;
   /** Reserved space */
   uint8_t fill18[4092];
   /**  at address offset 0x48108, read-write */
   uint32_t axi32_hwseqm_fn_mod;
};

/** bit field defines for nic400_modem_ic_s#peripheralid4 */
#define NIC400_MODEM_IC_PERIPHERALID4_PERIPH_ID_4_OFFSET 0
#define NIC400_MODEM_IC_PERIPHERALID4_PERIPH_ID_4_SIZE 8

/** bit field defines for nic400_modem_ic_s#peripheralid5 */
#define NIC400_MODEM_IC_PERIPHERALID5_PERIPH_ID_5_OFFSET 0
#define NIC400_MODEM_IC_PERIPHERALID5_PERIPH_ID_5_SIZE 8

/** bit field defines for nic400_modem_ic_s#peripheralid6 */
#define NIC400_MODEM_IC_PERIPHERALID6_PERIPH_ID_6_OFFSET 0
#define NIC400_MODEM_IC_PERIPHERALID6_PERIPH_ID_6_SIZE 8

/** bit field defines for nic400_modem_ic_s#peripheralid7 */
#define NIC400_MODEM_IC_PERIPHERALID7_PERIPH_ID_7_OFFSET 0
#define NIC400_MODEM_IC_PERIPHERALID7_PERIPH_ID_7_SIZE 8

/** bit field defines for nic400_modem_ic_s#peripheralid0 */
#define NIC400_MODEM_IC_PERIPHERALID0_PERIPH_ID_0_OFFSET 0
#define NIC400_MODEM_IC_PERIPHERALID0_PERIPH_ID_0_SIZE 8

/** bit field defines for nic400_modem_ic_s#peripheralid1 */
#define NIC400_MODEM_IC_PERIPHERALID1_PERIPH_ID_1_OFFSET 0
#define NIC400_MODEM_IC_PERIPHERALID1_PERIPH_ID_1_SIZE 8

/** bit field defines for nic400_modem_ic_s#peripheralid2 */
#define NIC400_MODEM_IC_PERIPHERALID2_PERIPH_ID_2_OFFSET 0
#define NIC400_MODEM_IC_PERIPHERALID2_PERIPH_ID_2_SIZE 8

/** bit field defines for nic400_modem_ic_s#peripheralid3 */
#define NIC400_MODEM_IC_PERIPHERALID3_CUST_MOD_NUM_OFFSET 0
#define NIC400_MODEM_IC_PERIPHERALID3_CUST_MOD_NUM_SIZE 4
#define NIC400_MODEM_IC_PERIPHERALID3_REV_AND_OFFSET 4
#define NIC400_MODEM_IC_PERIPHERALID3_REV_AND_SIZE 4

/** bit field defines for nic400_modem_ic_s#componentid0 */
#define NIC400_MODEM_IC_COMPONENTID0_COMP_ID_0_OFFSET 0
#define NIC400_MODEM_IC_COMPONENTID0_COMP_ID_0_SIZE 8

/** bit field defines for nic400_modem_ic_s#componentid1 */
#define NIC400_MODEM_IC_COMPONENTID1_COMP_ID_1_OFFSET 0
#define NIC400_MODEM_IC_COMPONENTID1_COMP_ID_1_SIZE 8

/** bit field defines for nic400_modem_ic_s#componentid2 */
#define NIC400_MODEM_IC_COMPONENTID2_COMP_ID_2_OFFSET 0
#define NIC400_MODEM_IC_COMPONENTID2_COMP_ID_2_SIZE 8

/** bit field defines for nic400_modem_ic_s#componentid3 */
#define NIC400_MODEM_IC_COMPONENTID3_COMP_ID_3_OFFSET 0
#define NIC400_MODEM_IC_COMPONENTID3_COMP_ID_3_SIZE 8

/** bit field defines for nic400_modem_ic_s#axi32_mdmcpus_ib_fn_mod_iss_bm */
#define NIC400_MODEM_IC_AXI32_MDMCPUS_IB_FN_MOD_ISS_BM_FN_MOD_ISS_BM_OFFSET 0
#define NIC400_MODEM_IC_AXI32_MDMCPUS_IB_FN_MOD_ISS_BM_FN_MOD_ISS_BM_SIZE 2

/** bit field defines for nic400_modem_ic_s#axi32_mdmcpus_ib_fn_mod */
#define NIC400_MODEM_IC_AXI32_MDMCPUS_IB_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_MODEM_IC_AXI32_MDMCPUS_IB_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_modem_ic_s#axi32_vsps_fn_mod_iss_bm */
#define NIC400_MODEM_IC_AXI32_VSPS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_OFFSET 0
#define NIC400_MODEM_IC_AXI32_VSPS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_SIZE 2

/** bit field defines for nic400_modem_ic_s#axi32_tcus_fn_mod_iss_bm */
#define NIC400_MODEM_IC_AXI32_TCUS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_OFFSET 0
#define NIC400_MODEM_IC_AXI32_TCUS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_SIZE 2

/** bit field defines for nic400_modem_ic_s#axi32_phycpus_ib_fn_mod_iss_bm */
#define NIC400_MODEM_IC_AXI32_PHYCPUS_IB_FN_MOD_ISS_BM_FN_MOD_ISS_BM_OFFSET 0
#define NIC400_MODEM_IC_AXI32_PHYCPUS_IB_FN_MOD_ISS_BM_FN_MOD_ISS_BM_SIZE 2

/** bit field defines for nic400_modem_ic_s#axi32_phycpus_ib_fn_mod */
#define NIC400_MODEM_IC_AXI32_PHYCPUS_IB_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_MODEM_IC_AXI32_PHYCPUS_IB_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_modem_ic_s#axi32_ahbics_ib_fn_mod_iss_bm */
#define NIC400_MODEM_IC_AXI32_AHBICS_IB_FN_MOD_ISS_BM_FN_MOD_ISS_BM_OFFSET 0
#define NIC400_MODEM_IC_AXI32_AHBICS_IB_FN_MOD_ISS_BM_FN_MOD_ISS_BM_SIZE 2

/** bit field defines for nic400_modem_ic_s#axi32_ahbics_ib_fn_mod */
#define NIC400_MODEM_IC_AXI32_AHBICS_IB_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_MODEM_IC_AXI32_AHBICS_IB_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_modem_ic_s#axi32_hwseqs_fn_mod_iss_bm */
#define NIC400_MODEM_IC_AXI32_HWSEQS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_OFFSET 0
#define NIC400_MODEM_IC_AXI32_HWSEQS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_SIZE 2

/** bit field defines for nic400_modem_ic_s#axi64_mains_fn_mod_iss_bm */
#define NIC400_MODEM_IC_AXI64_MAINS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_OFFSET 0
#define NIC400_MODEM_IC_AXI64_MAINS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_SIZE 2

/** bit field defines for nic400_modem_ic_s#axi64_mdmcpum_fn_mod */
#define NIC400_MODEM_IC_AXI64_MDMCPUM_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_MODEM_IC_AXI64_MDMCPUM_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_modem_ic_s#axi32_mainm_fn_mod */
#define NIC400_MODEM_IC_AXI32_MAINM_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_MODEM_IC_AXI32_MAINM_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_modem_ic_s#axi32_vdmam_fn_mod */
#define NIC400_MODEM_IC_AXI32_VDMAM_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_MODEM_IC_AXI32_VDMAM_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_modem_ic_s#axi32_hwaccm_fn_mod */
#define NIC400_MODEM_IC_AXI32_HWACCM_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_MODEM_IC_AXI32_HWACCM_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_modem_ic_s#axi32_vspm_fn_mod */
#define NIC400_MODEM_IC_AXI32_VSPM_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_MODEM_IC_AXI32_VSPM_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_modem_ic_s#ahb32_phycpum_fn_mod_ahb */
#define NIC400_MODEM_IC_AHB32_PHYCPUM_FN_MOD_AHB_RD_INC_OVERRIDE_OFFSET 0
#define NIC400_MODEM_IC_AHB32_PHYCPUM_FN_MOD_AHB_RD_INC_OVERRIDE_SIZE 1
#define NIC400_MODEM_IC_AHB32_PHYCPUM_FN_MOD_AHB_WR_INC_OVERRIDE_OFFSET 1
#define NIC400_MODEM_IC_AHB32_PHYCPUM_FN_MOD_AHB_WR_INC_OVERRIDE_SIZE 1
#define NIC400_MODEM_IC_AHB32_PHYCPUM_FN_MOD_AHB_LOCK_OVERRIDE_OFFSET 2
#define NIC400_MODEM_IC_AHB32_PHYCPUM_FN_MOD_AHB_LOCK_OVERRIDE_SIZE 1

/** bit field defines for nic400_modem_ic_s#ahb32_phycpum_ib_fn_mod */
#define NIC400_MODEM_IC_AHB32_PHYCPUM_IB_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_MODEM_IC_AHB32_PHYCPUM_IB_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_modem_ic_s#axi32_hwseqm_fn_mod */
#define NIC400_MODEM_IC_AXI32_HWSEQM_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_MODEM_IC_AXI32_HWSEQM_FN_MOD_FN_MOD_SIZE 2

/* EOF nic400_modem_ic.h */
#endif
