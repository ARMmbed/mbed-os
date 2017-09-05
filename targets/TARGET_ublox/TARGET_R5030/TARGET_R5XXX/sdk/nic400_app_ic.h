#ifndef INCLUDED_NIC400_APP_IC
#define INCLUDED_NIC400_APP_IC
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>


struct nic400_app_ic_s {
   /**  at address offset 0x000, write-only */
   uint32_t remap;
   /** Reserved space */
   uint8_t fill0[8140];
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
   uint32_t xhb32_appcpus_ib_fn_mod_iss_bm;
   /** Reserved space */
   uint8_t fill2[252];
   /**  at address offset 0x2108, read-write */
   uint32_t xhb32_appcpus_ib_fn_mod;
   /** Reserved space */
   uint8_t fill3[7932];
   /**  at address offset 0x4008, read-write */
   uint32_t ahb32_sdmas_fn_mod_iss_bm;
   /** Reserved space */
   uint8_t fill4[56];
   /**  at address offset 0x4044, read-write */
   uint32_t ahb32_sdmas_ahb_cntl;
   /** Reserved space */
   uint8_t fill5[4032];
   /**  at address offset 0x5008, read-write */
   uint32_t axi32_stms_fn_mod_iss_bm;
   /** Reserved space */
   uint8_t fill6[4092];
   /**  at address offset 0x6008, read-write */
   uint32_t axi64_roms_fn_mod_iss_bm;
   /** Reserved space */
   uint8_t fill7[4092];
   /**  at address offset 0x7008, read-write */
   uint32_t ahb32_usbs_fn_mod_iss_bm;
   /** Reserved space */
   uint8_t fill8[56];
   /**  at address offset 0x7044, read-write */
   uint32_t ahb32_usbs_ahb_cntl;
   /** Reserved space */
   uint8_t fill9[4032];
   /**  at address offset 0x8008, read-write */
   uint32_t xhb32_ospis_ib_fn_mod_iss_bm;
   /** Reserved space */
   uint8_t fill10[252];
   /**  at address offset 0x8108, read-write */
   uint32_t xhb32_ospis_ib_fn_mod;
   /** Reserved space */
   uint8_t fill11[3836];
   /**  at address offset 0x9008, read-write */
   uint32_t axi64_psrams_fn_mod_iss_bm;
   /** Reserved space */
   uint8_t fill12[8188];
   /**  at address offset 0xB008, read-write */
   uint32_t xhb32_periphrams_ib_fn_mod_iss_bm;
   /** Reserved space */
   uint8_t fill13[252];
   /**  at address offset 0xB108, read-write */
   uint32_t xhb32_periphrams_ib_fn_mod;
   /** Reserved space */
   uint8_t fill14[3836];
   /**  at address offset 0xC008, read-write */
   uint32_t axi32_modems_fn_mod_iss_bm;
   /** Reserved space */
   uint8_t fill15[221436];
   /**  at address offset 0x42108, read-write */
   uint32_t axi64_appcpum_fn_mod;
   /** Reserved space */
   uint8_t fill16[4092];
   /**  at address offset 0x43108, read-write */
   uint32_t axi64_modemm_fn_mod;
   /** Reserved space */
   uint8_t fill17[3868];
   /**  at address offset 0x44028, read-write */
   uint32_t ahb32_usbm_fn_mod_ahb;
   /** Reserved space */
   uint8_t fill18[220];
   /**  at address offset 0x44108, read-write */
   uint32_t ahb32_usbm_ib_fn_mod;
   /** Reserved space */
   uint8_t fill19[3864];
   /**  at address offset 0x45024, read-write */
   uint32_t axi64_sdiom_ib_fn_mod2;
   /** Reserved space */
   uint8_t fill20[224];
   /**  at address offset 0x45108, read-write */
   uint32_t axi64_sdiom_ib_fn_mod;
   /** Reserved space */
   uint8_t fill21[4092];
   /**  at address offset 0x46108, read-write */
   uint32_t axi32_dbgm_fn_mod;
   /** Reserved space */
   uint8_t fill22[3868];
   /**  at address offset 0x47028, read-write */
   uint32_t ahb32_appcpum_fn_mod_ahb;
   /** Reserved space */
   uint8_t fill23[220];
   /**  at address offset 0x47108, read-write */
   uint32_t ahb32_appcpum_ib_fn_mod;
   /** Reserved space */
   uint8_t fill24[7964];
   /**  at address offset 0x49028, read-write */
   uint32_t ahb32_sdmam_fn_mod_ahb;
   /** Reserved space */
   uint8_t fill25[220];
   /**  at address offset 0x49108, read-write */
   uint32_t ahb32_sdmam_ib_fn_mod;
   /** Reserved space */
   uint8_t fill26[3868];
   /**  at address offset 0x4A028, read-write */
   uint32_t ahb32_ciphm_fn_mod_ahb;
   /** Reserved space */
   uint8_t fill27[220];
   /**  at address offset 0x4A108, read-write */
   uint32_t ahb32_ciphm_ib_fn_mod;
};

/** bit field defines for nic400_app_ic_s#remap */
#define NIC400_APP_IC_REMAP_REMAP_OFFSET 0
#define NIC400_APP_IC_REMAP_REMAP_SIZE 2

/** bit field defines for nic400_app_ic_s#peripheralid4 */
#define NIC400_APP_IC_PERIPHERALID4_PERIPH_ID_4_OFFSET 0
#define NIC400_APP_IC_PERIPHERALID4_PERIPH_ID_4_SIZE 8

/** bit field defines for nic400_app_ic_s#peripheralid5 */
#define NIC400_APP_IC_PERIPHERALID5_PERIPH_ID_5_OFFSET 0
#define NIC400_APP_IC_PERIPHERALID5_PERIPH_ID_5_SIZE 8

/** bit field defines for nic400_app_ic_s#peripheralid6 */
#define NIC400_APP_IC_PERIPHERALID6_PERIPH_ID_6_OFFSET 0
#define NIC400_APP_IC_PERIPHERALID6_PERIPH_ID_6_SIZE 8

/** bit field defines for nic400_app_ic_s#peripheralid7 */
#define NIC400_APP_IC_PERIPHERALID7_PERIPH_ID_7_OFFSET 0
#define NIC400_APP_IC_PERIPHERALID7_PERIPH_ID_7_SIZE 8

/** bit field defines for nic400_app_ic_s#peripheralid0 */
#define NIC400_APP_IC_PERIPHERALID0_PERIPH_ID_0_OFFSET 0
#define NIC400_APP_IC_PERIPHERALID0_PERIPH_ID_0_SIZE 8

/** bit field defines for nic400_app_ic_s#peripheralid1 */
#define NIC400_APP_IC_PERIPHERALID1_PERIPH_ID_1_OFFSET 0
#define NIC400_APP_IC_PERIPHERALID1_PERIPH_ID_1_SIZE 8

/** bit field defines for nic400_app_ic_s#peripheralid2 */
#define NIC400_APP_IC_PERIPHERALID2_PERIPH_ID_2_OFFSET 0
#define NIC400_APP_IC_PERIPHERALID2_PERIPH_ID_2_SIZE 8

/** bit field defines for nic400_app_ic_s#peripheralid3 */
#define NIC400_APP_IC_PERIPHERALID3_CUST_MOD_NUM_OFFSET 0
#define NIC400_APP_IC_PERIPHERALID3_CUST_MOD_NUM_SIZE 4
#define NIC400_APP_IC_PERIPHERALID3_REV_AND_OFFSET 4
#define NIC400_APP_IC_PERIPHERALID3_REV_AND_SIZE 4

/** bit field defines for nic400_app_ic_s#componentid0 */
#define NIC400_APP_IC_COMPONENTID0_COMP_ID_0_OFFSET 0
#define NIC400_APP_IC_COMPONENTID0_COMP_ID_0_SIZE 8

/** bit field defines for nic400_app_ic_s#componentid1 */
#define NIC400_APP_IC_COMPONENTID1_COMP_ID_1_OFFSET 0
#define NIC400_APP_IC_COMPONENTID1_COMP_ID_1_SIZE 8

/** bit field defines for nic400_app_ic_s#componentid2 */
#define NIC400_APP_IC_COMPONENTID2_COMP_ID_2_OFFSET 0
#define NIC400_APP_IC_COMPONENTID2_COMP_ID_2_SIZE 8

/** bit field defines for nic400_app_ic_s#componentid3 */
#define NIC400_APP_IC_COMPONENTID3_COMP_ID_3_OFFSET 0
#define NIC400_APP_IC_COMPONENTID3_COMP_ID_3_SIZE 8

/** bit field defines for nic400_app_ic_s#xhb32_appcpus_ib_fn_mod_iss_bm */
#define NIC400_APP_IC_XHB32_APPCPUS_IB_FN_MOD_ISS_BM_FN_MOD_ISS_BM_OFFSET 0
#define NIC400_APP_IC_XHB32_APPCPUS_IB_FN_MOD_ISS_BM_FN_MOD_ISS_BM_SIZE 2

/** bit field defines for nic400_app_ic_s#xhb32_appcpus_ib_fn_mod */
#define NIC400_APP_IC_XHB32_APPCPUS_IB_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_APP_IC_XHB32_APPCPUS_IB_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_app_ic_s#ahb32_sdmas_fn_mod_iss_bm */
#define NIC400_APP_IC_AHB32_SDMAS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_OFFSET 0
#define NIC400_APP_IC_AHB32_SDMAS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_SIZE 2

/** bit field defines for nic400_app_ic_s#ahb32_sdmas_ahb_cntl */
#define NIC400_APP_IC_AHB32_SDMAS_AHB_CNTL_DECERR_EN_OFFSET 0
#define NIC400_APP_IC_AHB32_SDMAS_AHB_CNTL_DECERR_EN_SIZE 1
#define NIC400_APP_IC_AHB32_SDMAS_AHB_CNTL_FORCE_INCR_OFFSET 1
#define NIC400_APP_IC_AHB32_SDMAS_AHB_CNTL_FORCE_INCR_SIZE 1

/** bit field defines for nic400_app_ic_s#axi32_stms_fn_mod_iss_bm */
#define NIC400_APP_IC_AXI32_STMS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_OFFSET 0
#define NIC400_APP_IC_AXI32_STMS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_SIZE 2

/** bit field defines for nic400_app_ic_s#axi64_roms_fn_mod_iss_bm */
#define NIC400_APP_IC_AXI64_ROMS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_OFFSET 0
#define NIC400_APP_IC_AXI64_ROMS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_SIZE 2

/** bit field defines for nic400_app_ic_s#ahb32_usbs_fn_mod_iss_bm */
#define NIC400_APP_IC_AHB32_USBS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_OFFSET 0
#define NIC400_APP_IC_AHB32_USBS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_SIZE 2

/** bit field defines for nic400_app_ic_s#ahb32_usbs_ahb_cntl */
#define NIC400_APP_IC_AHB32_USBS_AHB_CNTL_DECERR_EN_OFFSET 0
#define NIC400_APP_IC_AHB32_USBS_AHB_CNTL_DECERR_EN_SIZE 1
#define NIC400_APP_IC_AHB32_USBS_AHB_CNTL_FORCE_INCR_OFFSET 1
#define NIC400_APP_IC_AHB32_USBS_AHB_CNTL_FORCE_INCR_SIZE 1

/** bit field defines for nic400_app_ic_s#xhb32_ospis_ib_fn_mod_iss_bm */
#define NIC400_APP_IC_XHB32_OSPIS_IB_FN_MOD_ISS_BM_FN_MOD_ISS_BM_OFFSET 0
#define NIC400_APP_IC_XHB32_OSPIS_IB_FN_MOD_ISS_BM_FN_MOD_ISS_BM_SIZE 2

/** bit field defines for nic400_app_ic_s#xhb32_ospis_ib_fn_mod */
#define NIC400_APP_IC_XHB32_OSPIS_IB_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_APP_IC_XHB32_OSPIS_IB_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_app_ic_s#axi64_psrams_fn_mod_iss_bm */
#define NIC400_APP_IC_AXI64_PSRAMS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_OFFSET 0
#define NIC400_APP_IC_AXI64_PSRAMS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_SIZE 2

/** bit field defines for nic400_app_ic_s#xhb32_periphrams_ib_fn_mod_iss_bm */
#define NIC400_APP_IC_XHB32_PERIPHRAMS_IB_FN_MOD_ISS_BM_FN_MOD_ISS_BM_OFFSET 0
#define NIC400_APP_IC_XHB32_PERIPHRAMS_IB_FN_MOD_ISS_BM_FN_MOD_ISS_BM_SIZE 2

/** bit field defines for nic400_app_ic_s#xhb32_periphrams_ib_fn_mod */
#define NIC400_APP_IC_XHB32_PERIPHRAMS_IB_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_APP_IC_XHB32_PERIPHRAMS_IB_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_app_ic_s#axi32_modems_fn_mod_iss_bm */
#define NIC400_APP_IC_AXI32_MODEMS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_OFFSET 0
#define NIC400_APP_IC_AXI32_MODEMS_FN_MOD_ISS_BM_FN_MOD_ISS_BM_SIZE 2

/** bit field defines for nic400_app_ic_s#axi64_appcpum_fn_mod */
#define NIC400_APP_IC_AXI64_APPCPUM_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_APP_IC_AXI64_APPCPUM_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_app_ic_s#axi64_modemm_fn_mod */
#define NIC400_APP_IC_AXI64_MODEMM_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_APP_IC_AXI64_MODEMM_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_app_ic_s#ahb32_usbm_fn_mod_ahb */
#define NIC400_APP_IC_AHB32_USBM_FN_MOD_AHB_RD_INC_OVERRIDE_OFFSET 0
#define NIC400_APP_IC_AHB32_USBM_FN_MOD_AHB_RD_INC_OVERRIDE_SIZE 1
#define NIC400_APP_IC_AHB32_USBM_FN_MOD_AHB_WR_INC_OVERRIDE_OFFSET 1
#define NIC400_APP_IC_AHB32_USBM_FN_MOD_AHB_WR_INC_OVERRIDE_SIZE 1

/** bit field defines for nic400_app_ic_s#ahb32_usbm_ib_fn_mod */
#define NIC400_APP_IC_AHB32_USBM_IB_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_APP_IC_AHB32_USBM_IB_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_app_ic_s#axi64_sdiom_ib_fn_mod2 */
#define NIC400_APP_IC_AXI64_SDIOM_IB_FN_MOD2_BYPASS_MERGE_OFFSET 0
#define NIC400_APP_IC_AXI64_SDIOM_IB_FN_MOD2_BYPASS_MERGE_SIZE 1

/** bit field defines for nic400_app_ic_s#axi64_sdiom_ib_fn_mod */
#define NIC400_APP_IC_AXI64_SDIOM_IB_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_APP_IC_AXI64_SDIOM_IB_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_app_ic_s#axi32_dbgm_fn_mod */
#define NIC400_APP_IC_AXI32_DBGM_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_APP_IC_AXI32_DBGM_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_app_ic_s#ahb32_appcpum_fn_mod_ahb */
#define NIC400_APP_IC_AHB32_APPCPUM_FN_MOD_AHB_RD_INC_OVERRIDE_OFFSET 0
#define NIC400_APP_IC_AHB32_APPCPUM_FN_MOD_AHB_RD_INC_OVERRIDE_SIZE 1
#define NIC400_APP_IC_AHB32_APPCPUM_FN_MOD_AHB_WR_INC_OVERRIDE_OFFSET 1
#define NIC400_APP_IC_AHB32_APPCPUM_FN_MOD_AHB_WR_INC_OVERRIDE_SIZE 1

/** bit field defines for nic400_app_ic_s#ahb32_appcpum_ib_fn_mod */
#define NIC400_APP_IC_AHB32_APPCPUM_IB_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_APP_IC_AHB32_APPCPUM_IB_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_app_ic_s#ahb32_sdmam_fn_mod_ahb */
#define NIC400_APP_IC_AHB32_SDMAM_FN_MOD_AHB_RD_INC_OVERRIDE_OFFSET 0
#define NIC400_APP_IC_AHB32_SDMAM_FN_MOD_AHB_RD_INC_OVERRIDE_SIZE 1
#define NIC400_APP_IC_AHB32_SDMAM_FN_MOD_AHB_WR_INC_OVERRIDE_OFFSET 1
#define NIC400_APP_IC_AHB32_SDMAM_FN_MOD_AHB_WR_INC_OVERRIDE_SIZE 1
#define NIC400_APP_IC_AHB32_SDMAM_FN_MOD_AHB_LOCK_OVERRIDE_OFFSET 2
#define NIC400_APP_IC_AHB32_SDMAM_FN_MOD_AHB_LOCK_OVERRIDE_SIZE 1

/** bit field defines for nic400_app_ic_s#ahb32_sdmam_ib_fn_mod */
#define NIC400_APP_IC_AHB32_SDMAM_IB_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_APP_IC_AHB32_SDMAM_IB_FN_MOD_FN_MOD_SIZE 2

/** bit field defines for nic400_app_ic_s#ahb32_ciphm_fn_mod_ahb */
#define NIC400_APP_IC_AHB32_CIPHM_FN_MOD_AHB_RD_INC_OVERRIDE_OFFSET 0
#define NIC400_APP_IC_AHB32_CIPHM_FN_MOD_AHB_RD_INC_OVERRIDE_SIZE 1
#define NIC400_APP_IC_AHB32_CIPHM_FN_MOD_AHB_WR_INC_OVERRIDE_OFFSET 1
#define NIC400_APP_IC_AHB32_CIPHM_FN_MOD_AHB_WR_INC_OVERRIDE_SIZE 1

/** bit field defines for nic400_app_ic_s#ahb32_ciphm_ib_fn_mod */
#define NIC400_APP_IC_AHB32_CIPHM_IB_FN_MOD_FN_MOD_OFFSET 0
#define NIC400_APP_IC_AHB32_CIPHM_IB_FN_MOD_FN_MOD_SIZE 2

/* EOF nic400_app_ic.h */
#endif
