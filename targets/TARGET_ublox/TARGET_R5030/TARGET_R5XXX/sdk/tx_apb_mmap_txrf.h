#ifndef INCLUDED_TX_APB_MMAP_TXRF
#define INCLUDED_TX_APB_MMAP_TXRF
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** tx_apb_mmap_txrf HAL Spreadsheet version number */
#define TX_APB_MMAP_TXRF_HAL_VERSION 101

/** This is the tx rf line-up register description
*/
struct tx_apb_mmap_txrf_s {
   /** Control bits for rxdivmix supply at address offset 0x000, read-write */
   uint32_t lo_ldo;
   /** Control bits for mix and DA supply at address offset 0x004, read-write */
   uint32_t rf_ldo;
   /** Control bits for cal buffer supply at address offset 0x008, read-write */
   uint32_t cal_ldo;
   /** block enables at address offset 0x00C, read-write */
   uint32_t txrf_enables;
   /** output power level control at address offset 0x010, read-write */
   uint32_t da_gain;
   /** balun tuning capacitors at address offset 0x014, read-write */
   uint32_t balun_tune;
   /** control bits for LOCM DACs at address offset 0x018, read-write */
   uint32_t locm_i;
   /** control bits for LOCM DACs at address offset 0x01C, read-write */
   uint32_t locm_q;
   /** Control bits for i-channel LO DACs at address offset 0x020, read-write */
   uint32_t dac_rfplop_i;
   /** Control bits for i-channel LO DACs at address offset 0x024, read-write */
   uint32_t dac_rfplon_i;
   /** Control bits for i-channel LO DACs at address offset 0x028, read-write */
   uint32_t dac_rfnlop_i;
   /** Control bits for i-channel LO DACs at address offset 0x02C, read-write */
   uint32_t dac_rfnlon_i;
   /** Control bits for i-channel LO DACs at address offset 0x030, read-write */
   uint32_t dac_rfplop_q;
   /** Control bits for i-channel LO DACs at address offset 0x034, read-write */
   uint32_t dac_rfplon_q;
   /** Control bits for i-channel LO DACs at address offset 0x038, read-write */
   uint32_t dac_rfnlop_q;
   /** Control bits for i-channel LO DACs at address offset 0x03C, read-write */
   uint32_t dac_rfnlon_q;
   /** lo_route at address offset 0x040, read-write */
   uint32_t lo_route;
   /** clock gen modulus programming  at address offset 0x044, read-write */
   uint32_t clkgen;
   /** Sublevel enables for RxRF anatest at address offset 0x048, read-write */
   uint32_t atst_rf;
   /** Spare Bits at address offset 0x04C, read-write */
   uint32_t txrf_spare;
   /** Debug register for testing the loading of data from the shadow to config registers. at address offset 0x050, read-write */
   uint32_t dbg_txrf;
};

/** bit field defines for tx_apb_mmap_txrf_s#lo_ldo */
#define TX_APB_MMAP_TXRF_LO_LDO_BYPASS_OFFSET 0
#define TX_APB_MMAP_TXRF_LO_LDO_BYPASS_SIZE 1
#define TX_APB_MMAP_TXRF_LO_LDO_ENABLE_OFFSET 1
#define TX_APB_MMAP_TXRF_LO_LDO_ENABLE_SIZE 1
#define TX_APB_MMAP_TXRF_LO_LDO_FAST_LOCK_OFFSET 2
#define TX_APB_MMAP_TXRF_LO_LDO_FAST_LOCK_SIZE 1
#define TX_APB_MMAP_TXRF_LO_LDO_VOUT_CTRL_OFFSET 3
#define TX_APB_MMAP_TXRF_LO_LDO_VOUT_CTRL_SIZE 4

/** bit field defines for tx_apb_mmap_txrf_s#rf_ldo */
#define TX_APB_MMAP_TXRF_RF_LDO_BYPASS_OFFSET 0
#define TX_APB_MMAP_TXRF_RF_LDO_BYPASS_SIZE 1
#define TX_APB_MMAP_TXRF_RF_LDO_ENABLE_OFFSET 1
#define TX_APB_MMAP_TXRF_RF_LDO_ENABLE_SIZE 1
#define TX_APB_MMAP_TXRF_RF_LDO_FAST_LOCK_OFFSET 2
#define TX_APB_MMAP_TXRF_RF_LDO_FAST_LOCK_SIZE 1
#define TX_APB_MMAP_TXRF_RF_LDO_VOUT_CTRL_OFFSET 3
#define TX_APB_MMAP_TXRF_RF_LDO_VOUT_CTRL_SIZE 4

/** bit field defines for tx_apb_mmap_txrf_s#cal_ldo */
#define TX_APB_MMAP_TXRF_CAL_LDO_BYPASS_OFFSET 0
#define TX_APB_MMAP_TXRF_CAL_LDO_BYPASS_SIZE 1
#define TX_APB_MMAP_TXRF_CAL_LDO_ENABLE_OFFSET 1
#define TX_APB_MMAP_TXRF_CAL_LDO_ENABLE_SIZE 1
#define TX_APB_MMAP_TXRF_CAL_LDO_FAST_LOCK_OFFSET 2
#define TX_APB_MMAP_TXRF_CAL_LDO_FAST_LOCK_SIZE 1
#define TX_APB_MMAP_TXRF_CAL_LDO_VOUT_CTRL_OFFSET 3
#define TX_APB_MMAP_TXRF_CAL_LDO_VOUT_CTRL_SIZE 4

/** bit field defines for tx_apb_mmap_txrf_s#txrf_enables */
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_LB_DA_EN_OFFSET 0
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_LB_DA_EN_SIZE 1
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_HB_DA_EN_OFFSET 1
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_HB_DA_EN_SIZE 1
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_LB_MIX_EN_OFFSET 2
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_LB_MIX_EN_SIZE 1
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_HB_MIX_EN_OFFSET 3
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_HB_MIX_EN_SIZE 1
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_MIX_DAC_EN_OFFSET 4
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_MIX_DAC_EN_SIZE 1
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_DIV_EN_OFFSET 5
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_DIV_EN_SIZE 1
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_LB_DIV_EN_OFFSET 6
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_LB_DIV_EN_SIZE 1
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_HB_DIV_EN_OFFSET 7
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_HB_DIV_EN_SIZE 1
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_CLKGEN_EN_OFFSET 8
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_CLKGEN_EN_SIZE 1
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_LB_CALBUF_EN_OFFSET 9
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_LB_CALBUF_EN_SIZE 1
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_HB_CALBUF_EN_OFFSET 10
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_HB_CALBUF_EN_SIZE 1
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_TEMPCELL_EN_OFFSET 11
#define TX_APB_MMAP_TXRF_TXRF_ENABLES_TEMPCELL_EN_SIZE 1

/** bit field defines for tx_apb_mmap_txrf_s#da_gain */
#define TX_APB_MMAP_TXRF_DA_GAIN_POWLEVEL_OFFSET 0
#define TX_APB_MMAP_TXRF_DA_GAIN_POWLEVEL_SIZE 6
#define TX_APB_MMAP_TXRF_DA_GAIN_GAIN_BOOST_OFFSET 6
#define TX_APB_MMAP_TXRF_DA_GAIN_GAIN_BOOST_SIZE 2

/** bit field defines for tx_apb_mmap_txrf_s#balun_tune */
#define TX_APB_MMAP_TXRF_BALUN_TUNE_CBAL_LB_OFFSET 0
#define TX_APB_MMAP_TXRF_BALUN_TUNE_CBAL_LB_SIZE 5
#define TX_APB_MMAP_TXRF_BALUN_TUNE_CBAL_HB_OFFSET 5
#define TX_APB_MMAP_TXRF_BALUN_TUNE_CBAL_HB_SIZE 5
#define TX_APB_MMAP_TXRF_BALUN_TUNE_CUBAL_LB_OFFSET 10
#define TX_APB_MMAP_TXRF_BALUN_TUNE_CUBAL_LB_SIZE 5
#define TX_APB_MMAP_TXRF_BALUN_TUNE_CUBAL_HB_OFFSET 15
#define TX_APB_MMAP_TXRF_BALUN_TUNE_CUBAL_HB_SIZE 5

/** bit field defines for tx_apb_mmap_txrf_s#locm_i */
#define TX_APB_MMAP_TXRF_LOCM_I_VCM_OFFSET 0
#define TX_APB_MMAP_TXRF_LOCM_I_VCM_SIZE 5

/** bit field defines for tx_apb_mmap_txrf_s#locm_q */
#define TX_APB_MMAP_TXRF_LOCM_Q_VCM_OFFSET 0
#define TX_APB_MMAP_TXRF_LOCM_Q_VCM_SIZE 5

/** bit field defines for tx_apb_mmap_txrf_s#dac_rfplop_i */
#define TX_APB_MMAP_TXRF_DAC_RFPLOP_I_PP_OFFSET 0
#define TX_APB_MMAP_TXRF_DAC_RFPLOP_I_PP_SIZE 6

/** bit field defines for tx_apb_mmap_txrf_s#dac_rfplon_i */
#define TX_APB_MMAP_TXRF_DAC_RFPLON_I_PN_OFFSET 0
#define TX_APB_MMAP_TXRF_DAC_RFPLON_I_PN_SIZE 6

/** bit field defines for tx_apb_mmap_txrf_s#dac_rfnlop_i */
#define TX_APB_MMAP_TXRF_DAC_RFNLOP_I_NP_OFFSET 0
#define TX_APB_MMAP_TXRF_DAC_RFNLOP_I_NP_SIZE 6

/** bit field defines for tx_apb_mmap_txrf_s#dac_rfnlon_i */
#define TX_APB_MMAP_TXRF_DAC_RFNLON_I_NN_OFFSET 0
#define TX_APB_MMAP_TXRF_DAC_RFNLON_I_NN_SIZE 6

/** bit field defines for tx_apb_mmap_txrf_s#dac_rfplop_q */
#define TX_APB_MMAP_TXRF_DAC_RFPLOP_Q_PP_OFFSET 0
#define TX_APB_MMAP_TXRF_DAC_RFPLOP_Q_PP_SIZE 6

/** bit field defines for tx_apb_mmap_txrf_s#dac_rfplon_q */
#define TX_APB_MMAP_TXRF_DAC_RFPLON_Q_PN_OFFSET 0
#define TX_APB_MMAP_TXRF_DAC_RFPLON_Q_PN_SIZE 6

/** bit field defines for tx_apb_mmap_txrf_s#dac_rfnlop_q */
#define TX_APB_MMAP_TXRF_DAC_RFNLOP_Q_NP_OFFSET 0
#define TX_APB_MMAP_TXRF_DAC_RFNLOP_Q_NP_SIZE 6

/** bit field defines for tx_apb_mmap_txrf_s#dac_rfnlon_q */
#define TX_APB_MMAP_TXRF_DAC_RFNLON_Q_NN_OFFSET 0
#define TX_APB_MMAP_TXRF_DAC_RFNLON_Q_NN_SIZE 6

/** bit field defines for tx_apb_mmap_txrf_s#lo_route */
#define TX_APB_MMAP_TXRF_LO_ROUTE_CTRL_OFFSET 0
#define TX_APB_MMAP_TXRF_LO_ROUTE_CTRL_SIZE 8

/** bit field defines for tx_apb_mmap_txrf_s#clkgen */
#define TX_APB_MMAP_TXRF_CLKGEN_MOD_P_OFFSET 0
#define TX_APB_MMAP_TXRF_CLKGEN_MOD_P_SIZE 4

/** bit field defines for tx_apb_mmap_txrf_s#atst_rf */
#define TX_APB_MMAP_TXRF_ATST_RF_DA_EN_A_OFFSET 0
#define TX_APB_MMAP_TXRF_ATST_RF_DA_EN_A_SIZE 1
#define TX_APB_MMAP_TXRF_ATST_RF_DA_EN_B_OFFSET 1
#define TX_APB_MMAP_TXRF_ATST_RF_DA_EN_B_SIZE 1
#define TX_APB_MMAP_TXRF_ATST_RF_DA_EN_C_OFFSET 2
#define TX_APB_MMAP_TXRF_ATST_RF_DA_EN_C_SIZE 1
#define TX_APB_MMAP_TXRF_ATST_RF_DA_EN_D_OFFSET 3
#define TX_APB_MMAP_TXRF_ATST_RF_DA_EN_D_SIZE 1
#define TX_APB_MMAP_TXRF_ATST_RF_DA_EN_E_OFFSET 4
#define TX_APB_MMAP_TXRF_ATST_RF_DA_EN_E_SIZE 1
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_A_OFFSET 5
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_A_SIZE 1
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_B_OFFSET 6
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_B_SIZE 1
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_C_OFFSET 7
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_C_SIZE 1
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_D_OFFSET 8
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_D_SIZE 1
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_E_OFFSET 9
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_E_SIZE 1
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_F_OFFSET 10
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_F_SIZE 1
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_G_OFFSET 11
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_G_SIZE 1
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_H_OFFSET 12
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_H_SIZE 1
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_I_OFFSET 13
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_I_SIZE 1
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_J_OFFSET 14
#define TX_APB_MMAP_TXRF_ATST_RF_MIX_EN_J_SIZE 1
#define TX_APB_MMAP_TXRF_ATST_RF_LDO_EN_A_OFFSET 15
#define TX_APB_MMAP_TXRF_ATST_RF_LDO_EN_A_SIZE 1
#define TX_APB_MMAP_TXRF_ATST_RF_LDO_EN_B_OFFSET 16
#define TX_APB_MMAP_TXRF_ATST_RF_LDO_EN_B_SIZE 1

/** bit field defines for tx_apb_mmap_txrf_s#txrf_spare */
#define TX_APB_MMAP_TXRF_TXRF_SPARE_MAIN_OFFSET 0
#define TX_APB_MMAP_TXRF_TXRF_SPARE_MAIN_SIZE 8
#define TX_APB_MMAP_TXRF_TXRF_SPARE_I_OFFSET 8
#define TX_APB_MMAP_TXRF_TXRF_SPARE_I_SIZE 8
#define TX_APB_MMAP_TXRF_TXRF_SPARE_Q_OFFSET 16
#define TX_APB_MMAP_TXRF_TXRF_SPARE_Q_SIZE 8

/** bit field defines for tx_apb_mmap_txrf_s#dbg_txrf */
#define TX_APB_MMAP_TXRF_DBG_TXRF_MEMMAP_VERSION_SHDW_OFFSET 0
#define TX_APB_MMAP_TXRF_DBG_TXRF_MEMMAP_VERSION_SHDW_SIZE 16
#define TX_APB_MMAP_TXRF_DBG_TXRF_MEMMAP_VERSION_CFG_OFFSET 16
#define TX_APB_MMAP_TXRF_DBG_TXRF_MEMMAP_VERSION_CFG_SIZE 16

/* EOF tx_apb_mmap_txrf.h */
#endif
