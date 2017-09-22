#ifndef INCLUDED_RX_APB_MMAP_RXRF
#define INCLUDED_RX_APB_MMAP_RXRF
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** rx_apb_mmap_rxrf HAL Spreadsheet version number */
#define RX_APB_MMAP_RXRF_HAL_VERSION 102

/** This is the digital receiver path control register description
*/
struct rx_apb_mmap_rxrf_s {
   /** Control bits for rxdivmix supply at address offset 0x000, read-write */
   uint32_t lo_ldo;
   /** Control bits for lna supply at address offset 0x004, read-write */
   uint32_t rf_ldo;
   /** Control bits for tca supply at address offset 0x008, read-write */
   uint32_t tca_ldo;
   /** block enables at address offset 0x00C, read-write */
   uint32_t rxrf_enables;
   /** lna gain at address offset 0x010, read-write */
   uint32_t lna_gain;
   /** lna bias currents at address offset 0x014, read-write */
   uint32_t lna_ibias;
   /** lna cascode bias voltages at address offset 0x018, read-write */
   uint32_t lna_vbias;
   /** tca i-channel gain at address offset 0x01C, read-write */
   uint32_t tca_i;
   /** tca q-channel gain at address offset 0x020, read-write */
   uint32_t tca_q;
   /** translational filter capacitance at address offset 0x024, read-write */
   uint32_t tf;
   /** control bits for LOCM DACs at address offset 0x028, read-write */
   uint32_t locm_i;
   /** control bits for LOCM DACs at address offset 0x02C, read-write */
   uint32_t locm_q;
   /** Control bits for i-channel IP2 DACs at address offset 0x030, read-write */
   uint32_t dac_rfplop_i;
   /** Control bits for i-channel IP2 DACs at address offset 0x034, read-write */
   uint32_t dac_rfplon_i;
   /** Control bits for i-channel IP2 DACs at address offset 0x038, read-write */
   uint32_t dac_rfnlop_i;
   /** Control bits for i-channel IP2 DACs at address offset 0x03C, read-write */
   uint32_t dac_rfnlon_i;
   /** Control bits for i-channel IP2 DACs at address offset 0x040, read-write */
   uint32_t dac_rfplop_q;
   /** Control bits for i-channel IP2 DACs at address offset 0x044, read-write */
   uint32_t dac_rfplon_q;
   /** Control bits for i-channel IP2 DACs at address offset 0x048, read-write */
   uint32_t dac_rfnlop_q;
   /** Control bits for i-channel IP2 DACs at address offset 0x04C, read-write */
   uint32_t dac_rfnlon_q;
   /** lo_route at address offset 0x050, read-write */
   uint32_t lo_route;
   /** clock gen modulus programming at address offset 0x054, read-write */
   uint32_t clkgen;
   /** Sublevel enables for lna and mix anatest at address offset 0x058, read-write */
   uint32_t atst1;
   /** Sublevel enables for tca and ldo anatest at address offset 0x05C, read-write */
   uint32_t atst2;
   /** Spare Bits at address offset 0x060, read-write */
   uint32_t rxrf_spare;
   /** Debug register for testing the loading of data from the shadow to config registers. at address offset 0x064, read-write */
   uint32_t dbg_rxrf;
};

/** bit field defines for rx_apb_mmap_rxrf_s#lo_ldo */
#define RX_APB_MMAP_RXRF_LO_LDO_BYPASS_OFFSET 0
#define RX_APB_MMAP_RXRF_LO_LDO_BYPASS_SIZE 1
#define RX_APB_MMAP_RXRF_LO_LDO_ENABLE_OFFSET 1
#define RX_APB_MMAP_RXRF_LO_LDO_ENABLE_SIZE 1
#define RX_APB_MMAP_RXRF_LO_LDO_FAST_LOCK_OFFSET 2
#define RX_APB_MMAP_RXRF_LO_LDO_FAST_LOCK_SIZE 1
#define RX_APB_MMAP_RXRF_LO_LDO_VOUT_CTRL_OFFSET 3
#define RX_APB_MMAP_RXRF_LO_LDO_VOUT_CTRL_SIZE 4

/** bit field defines for rx_apb_mmap_rxrf_s#rf_ldo */
#define RX_APB_MMAP_RXRF_RF_LDO_BYPASS_OFFSET 0
#define RX_APB_MMAP_RXRF_RF_LDO_BYPASS_SIZE 1
#define RX_APB_MMAP_RXRF_RF_LDO_ENABLE_OFFSET 1
#define RX_APB_MMAP_RXRF_RF_LDO_ENABLE_SIZE 1
#define RX_APB_MMAP_RXRF_RF_LDO_FAST_LOCK_OFFSET 2
#define RX_APB_MMAP_RXRF_RF_LDO_FAST_LOCK_SIZE 1
#define RX_APB_MMAP_RXRF_RF_LDO_VOUT_CTRL_OFFSET 3
#define RX_APB_MMAP_RXRF_RF_LDO_VOUT_CTRL_SIZE 4

/** bit field defines for rx_apb_mmap_rxrf_s#tca_ldo */
#define RX_APB_MMAP_RXRF_TCA_LDO_BYPASS_OFFSET 0
#define RX_APB_MMAP_RXRF_TCA_LDO_BYPASS_SIZE 1
#define RX_APB_MMAP_RXRF_TCA_LDO_ENABLE_OFFSET 1
#define RX_APB_MMAP_RXRF_TCA_LDO_ENABLE_SIZE 1
#define RX_APB_MMAP_RXRF_TCA_LDO_FAST_LOCK_OFFSET 2
#define RX_APB_MMAP_RXRF_TCA_LDO_FAST_LOCK_SIZE 1
#define RX_APB_MMAP_RXRF_TCA_LDO_VOUT_CTRL_OFFSET 3
#define RX_APB_MMAP_RXRF_TCA_LDO_VOUT_CTRL_SIZE 4

/** bit field defines for rx_apb_mmap_rxrf_s#rxrf_enables */
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_LB_LNA_EN_OFFSET 0
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_LB_LNA_EN_SIZE 1
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_MB_LNA_EN_OFFSET 1
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_MB_LNA_EN_SIZE 1
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_HB_LNA_EN_OFFSET 2
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_HB_LNA_EN_SIZE 1
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_LNA_IP_SEL_OFFSET 3
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_LNA_IP_SEL_SIZE 3
/**  */
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_LNA_IP_SEL_CAL_VALUE 0
/**  */
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_LNA_IP_SEL_LNA1_VALUE 1
/**  */
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_LNA_IP_SEL_LNA2_VALUE 2
/**  */
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_LNA_IP_SEL_LNA3_VALUE 3
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_LB_MIX_EN_OFFSET 6
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_LB_MIX_EN_SIZE 1
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_MB_MIX_EN_OFFSET 7
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_MB_MIX_EN_SIZE 1
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_HB_MIX_EN_OFFSET 8
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_HB_MIX_EN_SIZE 1
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_MIX_DAC_EN_OFFSET 9
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_MIX_DAC_EN_SIZE 1
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_DIV_EN_OFFSET 10
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_DIV_EN_SIZE 1
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_LB_DIV_EN_OFFSET 11
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_LB_DIV_EN_SIZE 1
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_MB_DIV_EN_OFFSET 12
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_MB_DIV_EN_SIZE 1
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_HB_DIV_EN_OFFSET 13
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_HB_DIV_EN_SIZE 1
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_CLKGEN_EN_OFFSET 14
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_CLKGEN_EN_SIZE 1
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_CALBUF_EN_OFFSET 15
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_CALBUF_EN_SIZE 1
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_TEMPCELL_EN_OFFSET 16
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_TEMPCELL_EN_SIZE 1
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_TCA_EN_OFFSET 17
#define RX_APB_MMAP_RXRF_RXRF_ENABLES_TCA_EN_SIZE 1

/** bit field defines for rx_apb_mmap_rxrf_s#lna_gain */
#define RX_APB_MMAP_RXRF_LNA_GAIN_GM_OFFSET 0
#define RX_APB_MMAP_RXRF_LNA_GAIN_GM_SIZE 1
#define RX_APB_MMAP_RXRF_LNA_GAIN_ROUT_OFFSET 1
#define RX_APB_MMAP_RXRF_LNA_GAIN_ROUT_SIZE 2
#define RX_APB_MMAP_RXRF_LNA_GAIN_RFB_OFFSET 3
#define RX_APB_MMAP_RXRF_LNA_GAIN_RFB_SIZE 3

/** bit field defines for rx_apb_mmap_rxrf_s#lna_ibias */
#define RX_APB_MMAP_RXRF_LNA_IBIAS_ICORE_OFFSET 0
#define RX_APB_MMAP_RXRF_LNA_IBIAS_ICORE_SIZE 4

/** bit field defines for rx_apb_mmap_rxrf_s#lna_vbias */
#define RX_APB_MMAP_RXRF_LNA_VBIAS_CASN_OFFSET 0
#define RX_APB_MMAP_RXRF_LNA_VBIAS_CASN_SIZE 3
#define RX_APB_MMAP_RXRF_LNA_VBIAS_CASP_OFFSET 3
#define RX_APB_MMAP_RXRF_LNA_VBIAS_CASP_SIZE 3

/** bit field defines for rx_apb_mmap_rxrf_s#tca_i */
#define RX_APB_MMAP_RXRF_TCA_I_GAIN_MODE_OFFSET 0
#define RX_APB_MMAP_RXRF_TCA_I_GAIN_MODE_SIZE 1
#define RX_APB_MMAP_RXRF_TCA_I_GAIN_OFFSET 1
#define RX_APB_MMAP_RXRF_TCA_I_GAIN_SIZE 6

/** bit field defines for rx_apb_mmap_rxrf_s#tca_q */
#define RX_APB_MMAP_RXRF_TCA_Q_GAIN_OFFSET 0
#define RX_APB_MMAP_RXRF_TCA_Q_GAIN_SIZE 6

/** bit field defines for rx_apb_mmap_rxrf_s#tf */
#define RX_APB_MMAP_RXRF_TF_CAP_OFFSET 0
#define RX_APB_MMAP_RXRF_TF_CAP_SIZE 8

/** bit field defines for rx_apb_mmap_rxrf_s#locm_i */
#define RX_APB_MMAP_RXRF_LOCM_I_VCM_OFFSET 0
#define RX_APB_MMAP_RXRF_LOCM_I_VCM_SIZE 5

/** bit field defines for rx_apb_mmap_rxrf_s#locm_q */
#define RX_APB_MMAP_RXRF_LOCM_Q_VCM_OFFSET 0
#define RX_APB_MMAP_RXRF_LOCM_Q_VCM_SIZE 5

/** bit field defines for rx_apb_mmap_rxrf_s#dac_rfplop_i */
#define RX_APB_MMAP_RXRF_DAC_RFPLOP_I_PP_OFFSET 0
#define RX_APB_MMAP_RXRF_DAC_RFPLOP_I_PP_SIZE 6

/** bit field defines for rx_apb_mmap_rxrf_s#dac_rfplon_i */
#define RX_APB_MMAP_RXRF_DAC_RFPLON_I_PN_OFFSET 0
#define RX_APB_MMAP_RXRF_DAC_RFPLON_I_PN_SIZE 6

/** bit field defines for rx_apb_mmap_rxrf_s#dac_rfnlop_i */
#define RX_APB_MMAP_RXRF_DAC_RFNLOP_I_NP_OFFSET 0
#define RX_APB_MMAP_RXRF_DAC_RFNLOP_I_NP_SIZE 6

/** bit field defines for rx_apb_mmap_rxrf_s#dac_rfnlon_i */
#define RX_APB_MMAP_RXRF_DAC_RFNLON_I_NN_OFFSET 0
#define RX_APB_MMAP_RXRF_DAC_RFNLON_I_NN_SIZE 6

/** bit field defines for rx_apb_mmap_rxrf_s#dac_rfplop_q */
#define RX_APB_MMAP_RXRF_DAC_RFPLOP_Q_PP_OFFSET 0
#define RX_APB_MMAP_RXRF_DAC_RFPLOP_Q_PP_SIZE 6

/** bit field defines for rx_apb_mmap_rxrf_s#dac_rfplon_q */
#define RX_APB_MMAP_RXRF_DAC_RFPLON_Q_PN_OFFSET 0
#define RX_APB_MMAP_RXRF_DAC_RFPLON_Q_PN_SIZE 6

/** bit field defines for rx_apb_mmap_rxrf_s#dac_rfnlop_q */
#define RX_APB_MMAP_RXRF_DAC_RFNLOP_Q_NP_OFFSET 0
#define RX_APB_MMAP_RXRF_DAC_RFNLOP_Q_NP_SIZE 6

/** bit field defines for rx_apb_mmap_rxrf_s#dac_rfnlon_q */
#define RX_APB_MMAP_RXRF_DAC_RFNLON_Q_NN_OFFSET 0
#define RX_APB_MMAP_RXRF_DAC_RFNLON_Q_NN_SIZE 6

/** bit field defines for rx_apb_mmap_rxrf_s#lo_route */
#define RX_APB_MMAP_RXRF_LO_ROUTE_CTRL_OFFSET 0
#define RX_APB_MMAP_RXRF_LO_ROUTE_CTRL_SIZE 8

/** bit field defines for rx_apb_mmap_rxrf_s#clkgen */
#define RX_APB_MMAP_RXRF_CLKGEN_MOD_P_OFFSET 0
#define RX_APB_MMAP_RXRF_CLKGEN_MOD_P_SIZE 4

/** bit field defines for rx_apb_mmap_rxrf_s#atst1 */
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_A_OFFSET 0
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_A_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_B_OFFSET 1
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_B_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_C_OFFSET 2
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_C_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_D_OFFSET 3
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_D_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_E_OFFSET 4
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_E_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_F_OFFSET 5
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_F_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_G_OFFSET 6
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_G_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_H_OFFSET 7
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_H_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_I_OFFSET 8
#define RX_APB_MMAP_RXRF_ATST1_LNA_EN_I_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_A_OFFSET 9
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_A_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_B_OFFSET 10
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_B_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_C_OFFSET 11
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_C_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_D_OFFSET 12
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_D_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_E_OFFSET 13
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_E_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_F_OFFSET 14
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_F_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_G_OFFSET 15
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_G_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_H_OFFSET 16
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_H_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_I_OFFSET 17
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_I_SIZE 1
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_J_OFFSET 18
#define RX_APB_MMAP_RXRF_ATST1_MIX_EN_J_SIZE 1

/** bit field defines for rx_apb_mmap_rxrf_s#atst2 */
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_A_OFFSET 0
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_A_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_B_OFFSET 1
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_B_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_C_OFFSET 2
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_C_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_D_OFFSET 3
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_D_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_E_OFFSET 4
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_E_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_F_OFFSET 5
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_F_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_G_OFFSET 6
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_G_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_H_OFFSET 7
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_H_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_I_OFFSET 8
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_I_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_J_OFFSET 9
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_J_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_K_OFFSET 10
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_K_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_L_OFFSET 11
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_L_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_M_OFFSET 12
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_M_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_N_OFFSET 13
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_N_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_O_OFFSET 14
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_O_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_P_OFFSET 15
#define RX_APB_MMAP_RXRF_ATST2_TCA_EN_P_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_LDO_EN_A_OFFSET 16
#define RX_APB_MMAP_RXRF_ATST2_LDO_EN_A_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_LDO_EN_B_OFFSET 17
#define RX_APB_MMAP_RXRF_ATST2_LDO_EN_B_SIZE 1
#define RX_APB_MMAP_RXRF_ATST2_LDO_EN_C_OFFSET 18
#define RX_APB_MMAP_RXRF_ATST2_LDO_EN_C_SIZE 1

/** bit field defines for rx_apb_mmap_rxrf_s#rxrf_spare */
#define RX_APB_MMAP_RXRF_RXRF_SPARE_MAIN_OFFSET 0
#define RX_APB_MMAP_RXRF_RXRF_SPARE_MAIN_SIZE 8
#define RX_APB_MMAP_RXRF_RXRF_SPARE_I_OFFSET 8
#define RX_APB_MMAP_RXRF_RXRF_SPARE_I_SIZE 8
#define RX_APB_MMAP_RXRF_RXRF_SPARE_Q_OFFSET 16
#define RX_APB_MMAP_RXRF_RXRF_SPARE_Q_SIZE 8

/** bit field defines for rx_apb_mmap_rxrf_s#dbg_rxrf */
#define RX_APB_MMAP_RXRF_DBG_RXRF_MEMMAP_VERSION_SHDW_OFFSET 0
#define RX_APB_MMAP_RXRF_DBG_RXRF_MEMMAP_VERSION_SHDW_SIZE 16
#define RX_APB_MMAP_RXRF_DBG_RXRF_MEMMAP_VERSION_CFG_OFFSET 16
#define RX_APB_MMAP_RXRF_DBG_RXRF_MEMMAP_VERSION_CFG_SIZE 16

/* EOF rx_apb_mmap_rxrf.h */
#endif
