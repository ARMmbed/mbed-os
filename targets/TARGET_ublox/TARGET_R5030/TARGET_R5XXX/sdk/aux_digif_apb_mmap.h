#ifndef INCLUDED_AUX_DIGIF_APB_MMAP
#define INCLUDED_AUX_DIGIF_APB_MMAP
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** aux_digif_apb_mmap HAL Spreadsheet version number */
#define AUX_DIGIF_APB_MMAP_HAL_VERSION 203

/** This is the Aux ADC control register description
*/
struct aux_digif_apb_mmap_s {
   /** Control bits for reference buffers supply at address offset 0x000, read-write */
   uint32_t ref_ldo;
   /** Control bits for cal path LDO supply at address offset 0x004, read-write */
   uint32_t cal_ldo;
   /** block enables at address offset 0x008, read-write */
   uint32_t aux_clk;
   /** Control bits for adc analog supply at address offset 0x00C, read-write */
   uint32_t adc_ldo_auxtop;
   /** Enable bits for Aux at address offset 0x010, read-write */
   uint32_t enables;
   /** Enable temp cell bits for AUX at address offset 0x014, read-write */
   uint32_t temp_enables;
   /** Master enables for Rx anatest at address offset 0x018, read-write */
   uint32_t atst_aux_auxtop;
   /** Sublevel enables for aux anatest at address offset 0x01C, read-write */
   uint32_t atst_enables;
   /** Toplevel enables for Aux anatest at address offset 0x020, read-write */
   uint32_t atst_toplevel_auxtop;
   /**  at address offset 0x024, read-write */
   uint32_t input_cond;
   /** enables for input conditioning anatest at address offset 0x028, read-write */
   uint32_t input_cond_modes;
   /** enables for input conditioning cm selection at address offset 0x02C, read-write */
   uint32_t input_ref_sel;
   /** enables for input conditioning temperature sense at address offset 0x030, read-write */
   uint32_t input_cond_temp_enables;
   /** enables for input conditioning et filter sense at address offset 0x034, read-write */
   uint32_t input_cond_et_select;
   /** Control bits for Clock MUX at address offset 0x038, read-write */
   uint32_t adc_clkmux_auxtop;
   /** Control bits for bandgap reference at address offset 0x03C, read-write */
   uint32_t bandgap_auxtop;
   /**  at address offset 0x040, read-write */
   uint32_t afc_dac;
   /** Remaining bits to control AFC DAC at address offset 0x044, read-write */
   uint32_t afc_dac2;
   /**  at address offset 0x048, read-only */
   uint32_t afc_dac_ctrl_rdback;
   /** Remaining lower bits to control AFC DAC read back at address offset 0x04C, read-only */
   uint32_t afc_dac2_ctrl_rdback;
   /**  at address offset 0x050, read-write */
   uint32_t cal;
   /** calibration gain reg at address offset 0x054, read-write */
   uint32_t cal_gain;
   /** aux spares register at address offset 0x058, read-write */
   uint32_t spares;
   /** Debug register for testing the loading of data from the shadow to config registers. at address offset 0x05C, read-write */
   uint32_t dbg_mmap;
   /** I-ADC Mode Control at address offset 0x060, read-write */
   uint32_t adc_mode_i;
   /** ADC Offline Calibration Status at address offset 0x064, read-only */
   uint32_t adc_cal_status;
   /** I-ADC Calibration Clock Divider at address offset 0x068, read-write */
   uint32_t adc_i_calclk;
   /** I-ADC Dither Control for Calibration at address offset 0x06C, read-write */
   uint32_t adc_i_dither;
   /** ADC IQ Mux Control at address offset 0x070, read-write */
   uint32_t adc_iqmux;
   /** ADC Cal Readback Control at address offset 0x074, read-write */
   uint32_t adc_cal_readback_ctrl;
   /** I ADC Cal Readback Result at address offset 0x078, read-only */
   uint32_t adc_cal_readback_i;
   /** I-ADC Calibration Control at address offset 0x07C, read-write */
   uint32_t chi1_cal;
   /** I-ADC Calibration Stage Control at address offset 0x080, read-write */
   uint32_t chi1_calstagectrl;
   /** I-ADC Channel 1 - channel gain control at address offset 0x084, read-write */
   uint32_t chi1_channelgain;
   /** I-ADC Control Stage 1 Negative DAC in manual calibration mode. at address offset 0x088, read-write */
   uint32_t chi1_dacn1;
   /** I-ADC Control Stage 1 Positive DAC in manual calibration mode. at address offset 0x08C, read-write */
   uint32_t chi1_dacp1;
   /** I-ADC Control Stage 2 Negative DAC in manual calibration mode. at address offset 0x090, read-write */
   uint32_t chi1_dacn2;
   /** I-ADC Control Stage 2 Positive DAC in manual calibration mode. at address offset 0x094, read-write */
   uint32_t chi1_dacp2;
   /** I-ADC Amplifier Control at address offset 0x098, read-write */
   uint32_t chi1_amp;
   /** Adjust the size of the fixed DAC array. This is mostly redundant. at address offset 0x09C, read-write */
   uint32_t chi1_msbcal;
   /** I-ADC Comparator 12 (MSB) Positive Offset Control at address offset 0x0A0, read-write */
   uint32_t chi1_comp12calp;
   /** I-ADC Comparator 12 (MSB) Negative Offset Control at address offset 0x0A4, read-write */
   uint32_t chi1_comp12caln;
   /** I-ADC Comparator 11 Positive Offset Control at address offset 0x0A8, read-write */
   uint32_t chi1_comp11calp;
   /** I-ADC Comparator 11 Negative Offset Control at address offset 0x0AC, read-write */
   uint32_t chi1_comp11caln;
   /** I-ADC Comparator 10 Positive Offset Control at address offset 0x0B0, read-write */
   uint32_t chi1_comp10calp;
   /** I-ADC Comparator 10 Negative Offset Control at address offset 0x0B4, read-write */
   uint32_t chi1_comp10caln;
   /** I-ADC Comparator 9 Positive Offset Control at address offset 0x0B8, read-write */
   uint32_t chi1_comp9calp;
   /** I-ADC Comparator 9 Negative Offset Control at address offset 0x0BC, read-write */
   uint32_t chi1_comp9caln;
   /** I-ADC Comparator 8 Positive Offset Control at address offset 0x0C0, read-write */
   uint32_t chi1_comp8calp;
   /** I-ADC Comparator 8 Negative Offset Control at address offset 0x0C4, read-write */
   uint32_t chi1_comp8caln;
   /** I-ADC Comparator 7 Positive Offset Control at address offset 0x0C8, read-write */
   uint32_t chi1_comp7calp;
   /** I-ADC Comparator 7 Negative Offset Control at address offset 0x0CC, read-write */
   uint32_t chi1_comp7caln;
   /** I-ADC Comparator 6 Positive Offset Control at address offset 0x0D0, read-write */
   uint32_t chi1_comp6calp;
   /** I-ADC Comparator 6 Negative Offset Control at address offset 0x0D4, read-write */
   uint32_t chi1_comp6caln;
   /** I-ADC Comparator 5 Positive Offset Control at address offset 0x0D8, read-write */
   uint32_t chi1_comp5calp;
   /** I-ADC Comparator 5 Negative Offset Control at address offset 0x0DC, read-write */
   uint32_t chi1_comp5caln;
   /** I-ADC Comparator 4 Positive Offset Control at address offset 0x0E0, read-write */
   uint32_t chi1_comp4calp;
   /** I-ADC Comparator 4 Negative Offset Control at address offset 0x0E4, read-write */
   uint32_t chi1_comp4caln;
   /** I-ADC Comparator 3 Positive Offset Control at address offset 0x0E8, read-write */
   uint32_t chi1_comp3calp;
   /** I-ADC Comparator 3 Negative Offset Control at address offset 0x0EC, read-write */
   uint32_t chi1_comp3caln;
   /** I-ADC Comparator 2 Positive Offset Control at address offset 0x0F0, read-write */
   uint32_t chi1_comp2calp;
   /** I-ADC Comparator 2 Negative Offset Control at address offset 0x0F4, read-write */
   uint32_t chi1_comp2caln;
   /** I-ADC Comparator 1 Positive Offset Control at address offset 0x0F8, read-write */
   uint32_t chi1_comp1calp;
   /** I-ADC Comparator 1 Negative Offset Control at address offset 0x0FC, read-write */
   uint32_t chi1_comp1caln;
   /** I-ADC Comparator 0 (LSB) Positive Offset Control at address offset 0x100, read-write */
   uint32_t chi1_comp0calp;
   /** I-ADC Comparator 0 (LSB) Negative Offset Control at address offset 0x104, read-write */
   uint32_t chi1_comp0caln;
   /** I-ADC Comparator Clock Delay Control at address offset 0x108, read-write */
   uint32_t chi1_compdelay;
   /** I-ADC conversion fail flags at address offset 0x10C, read-only */
   uint32_t chi1_fail_detect;
   /** Spare Bits at address offset 0x110, read-write */
   uint32_t chi1_spare;
   /** Calibration State Machine Counters at address offset 0x114, read-write */
   uint32_t chi1_n_values;
   /** I-ADC Comparator 12 (MSB) Offset Error Threshold at address offset 0x118, read-write */
   uint32_t chi1_threshold12;
   /** I-ADC Comparator 11 Offset Error Threshold at address offset 0x11C, read-write */
   uint32_t chi1_threshold11;
   /** I-ADC Comparator 10 Offset Error Threshold at address offset 0x120, read-write */
   uint32_t chi1_threshold10;
   /** I-ADC Comparator 9 Offset Error Threshold at address offset 0x124, read-write */
   uint32_t chi1_threshold9;
   /** I-ADC Comparator 8 Offset Error Threshold at address offset 0x128, read-write */
   uint32_t chi1_threshold8;
   /** I-ADC Comparator 7 Offset Error Threshold at address offset 0x12C, read-write */
   uint32_t chi1_threshold7;
   /** I-ADC Comparator 6 Offset Error Threshold at address offset 0x130, read-write */
   uint32_t chi1_threshold6;
   /** I-ADC Comparator 5 Offset Error Threshold at address offset 0x134, read-write */
   uint32_t chi1_threshold5;
   /** I-ADC Comparator 4 Offset Error Threshold at address offset 0x138, read-write */
   uint32_t chi1_threshold4;
   /** I-ADC Comparator 3 Offset Error Threshold at address offset 0x13C, read-write */
   uint32_t chi1_threshold3;
   /** I-ADC Comparator 2 Offset Error Threshold at address offset 0x140, read-write */
   uint32_t chi1_threshold2;
   /** I-ADC Calibration Control at address offset 0x144, read-write */
   uint32_t chi2_cal;
   /** I-ADC Calibration Stage Control at address offset 0x148, read-write */
   uint32_t chi2_calstagectrl;
   /** I-ADC Channel 2 - channel gain control at address offset 0x14C, read-write */
   uint32_t chi2_channelgain;
   /** I-ADC Control Stage 1 Negative DAC in manual calibration mode. at address offset 0x150, read-write */
   uint32_t chi2_dacn1;
   /** I-ADC Control Stage 1 Positive DAC in manual calibration mode. at address offset 0x154, read-write */
   uint32_t chi2_dacp1;
   /** I-ADC Control Stage 2 Negative DAC in manual calibration mode. at address offset 0x158, read-write */
   uint32_t chi2_dacn2;
   /** I-ADC Control Stage 2 Positive DAC in manual calibration mode. at address offset 0x15C, read-write */
   uint32_t chi2_dacp2;
   /** I-ADC Amplifier Control at address offset 0x160, read-write */
   uint32_t chi2_amp;
   /** Adjust the size of the fixed DAC array. This is mostly redundant. at address offset 0x164, read-write */
   uint32_t chi2_msbcal;
   /** I-ADC Comparator 12 (MSB) Positive Offset Control at address offset 0x168, read-write */
   uint32_t chi2_comp12calp;
   /** I-ADC Comparator 12 (MSB) Negative Offset Control at address offset 0x16C, read-write */
   uint32_t chi2_comp12caln;
   /** I-ADC Comparator 11 Positive Offset Control at address offset 0x170, read-write */
   uint32_t chi2_comp11calp;
   /** I-ADC Comparator 11 Negative Offset Control at address offset 0x174, read-write */
   uint32_t chi2_comp11caln;
   /** I-ADC Comparator 10 Positive Offset Control at address offset 0x178, read-write */
   uint32_t chi2_comp10calp;
   /** I-ADC Comparator 10 Negative Offset Control at address offset 0x17C, read-write */
   uint32_t chi2_comp10caln;
   /** I-ADC Comparator 9 Positive Offset Control at address offset 0x180, read-write */
   uint32_t chi2_comp9calp;
   /** I-ADC Comparator 9 Negative Offset Control at address offset 0x184, read-write */
   uint32_t chi2_comp9caln;
   /** I-ADC Comparator 8 Positive Offset Control at address offset 0x188, read-write */
   uint32_t chi2_comp8calp;
   /** I-ADC Comparator 8 Negative Offset Control at address offset 0x18C, read-write */
   uint32_t chi2_comp8caln;
   /** I-ADC Comparator 7 Positive Offset Control at address offset 0x190, read-write */
   uint32_t chi2_comp7calp;
   /** I-ADC Comparator 7 Negative Offset Control at address offset 0x194, read-write */
   uint32_t chi2_comp7caln;
   /** I-ADC Comparator 6 Positive Offset Control at address offset 0x198, read-write */
   uint32_t chi2_comp6calp;
   /** I-ADC Comparator 6 Negative Offset Control at address offset 0x19C, read-write */
   uint32_t chi2_comp6caln;
   /** I-ADC Comparator 5 Positive Offset Control at address offset 0x1A0, read-write */
   uint32_t chi2_comp5calp;
   /** I-ADC Comparator 5 Negative Offset Control at address offset 0x1A4, read-write */
   uint32_t chi2_comp5caln;
   /** I-ADC Comparator 4 Positive Offset Control at address offset 0x1A8, read-write */
   uint32_t chi2_comp4calp;
   /** I-ADC Comparator 4 Negative Offset Control at address offset 0x1AC, read-write */
   uint32_t chi2_comp4caln;
   /** I-ADC Comparator 3 Positive Offset Control at address offset 0x1B0, read-write */
   uint32_t chi2_comp3calp;
   /** I-ADC Comparator 3 Negative Offset Control at address offset 0x1B4, read-write */
   uint32_t chi2_comp3caln;
   /** I-ADC Comparator 2 Positive Offset Control at address offset 0x1B8, read-write */
   uint32_t chi2_comp2calp;
   /** I-ADC Comparator 2 Negative Offset Control at address offset 0x1BC, read-write */
   uint32_t chi2_comp2caln;
   /** I-ADC Comparator 1 Positive Offset Control at address offset 0x1C0, read-write */
   uint32_t chi2_comp1calp;
   /** I-ADC Comparator 1 Negative Offset Control at address offset 0x1C4, read-write */
   uint32_t chi2_comp1caln;
   /** I-ADC Comparator 0 (LSB) Positive Offset Control at address offset 0x1C8, read-write */
   uint32_t chi2_comp0calp;
   /** I-ADC Comparator 0 (LSB) Negative Offset Control at address offset 0x1CC, read-write */
   uint32_t chi2_comp0caln;
   /** I-ADC Comparator Clock Delay Control at address offset 0x1D0, read-write */
   uint32_t chi2_compdelay;
   /** I-ADC conversion fail flags at address offset 0x1D4, read-only */
   uint32_t chi2_fail_detect;
   /** Spare Bits at address offset 0x1D8, read-write */
   uint32_t chi2_spare;
   /** Calibration State Machine Counters at address offset 0x1DC, read-write */
   uint32_t chi2_n_values;
   /** I-ADC Comparator 12 (MSB) Offset Error Threshold at address offset 0x1E0, read-write */
   uint32_t chi2_threshold12;
   /** I-ADC Comparator 11 Offset Error Threshold at address offset 0x1E4, read-write */
   uint32_t chi2_threshold11;
   /** I-ADC Comparator 10 Offset Error Threshold at address offset 0x1E8, read-write */
   uint32_t chi2_threshold10;
   /** I-ADC Comparator 9 Offset Error Threshold at address offset 0x1EC, read-write */
   uint32_t chi2_threshold9;
   /** I-ADC Comparator 8 Offset Error Threshold at address offset 0x1F0, read-write */
   uint32_t chi2_threshold8;
   /** I-ADC Comparator 7 Offset Error Threshold at address offset 0x1F4, read-write */
   uint32_t chi2_threshold7;
   /** I-ADC Comparator 6 Offset Error Threshold at address offset 0x1F8, read-write */
   uint32_t chi2_threshold6;
   /** I-ADC Comparator 5 Offset Error Threshold at address offset 0x1FC, read-write */
   uint32_t chi2_threshold5;
   /** I-ADC Comparator 4 Offset Error Threshold at address offset 0x200, read-write */
   uint32_t chi2_threshold4;
   /** I-ADC Comparator 3 Offset Error Threshold at address offset 0x204, read-write */
   uint32_t chi2_threshold3;
   /** I-ADC Comparator 2 Offset Error Threshold at address offset 0x208, read-write */
   uint32_t chi2_threshold2;
};

/** bit field defines for aux_digif_apb_mmap_s#ref_ldo */
#define AUX_DIGIF_APB_MMAP_REF_LDO_BYPASS_OFFSET 0
#define AUX_DIGIF_APB_MMAP_REF_LDO_BYPASS_SIZE 1
#define AUX_DIGIF_APB_MMAP_REF_LDO_ENABLE_OFFSET 1
#define AUX_DIGIF_APB_MMAP_REF_LDO_ENABLE_SIZE 1
#define AUX_DIGIF_APB_MMAP_REF_LDO_STARTUP_OFFSET 2
#define AUX_DIGIF_APB_MMAP_REF_LDO_STARTUP_SIZE 1
#define AUX_DIGIF_APB_MMAP_REF_LDO_VOUT_CTRL_OFFSET 3
#define AUX_DIGIF_APB_MMAP_REF_LDO_VOUT_CTRL_SIZE 4

/** bit field defines for aux_digif_apb_mmap_s#cal_ldo */
#define AUX_DIGIF_APB_MMAP_CAL_LDO_BYPASS_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CAL_LDO_BYPASS_SIZE 1
#define AUX_DIGIF_APB_MMAP_CAL_LDO_ENABLE_OFFSET 1
#define AUX_DIGIF_APB_MMAP_CAL_LDO_ENABLE_SIZE 1
#define AUX_DIGIF_APB_MMAP_CAL_LDO_FAST_LOCK_OFFSET 2
#define AUX_DIGIF_APB_MMAP_CAL_LDO_FAST_LOCK_SIZE 1
#define AUX_DIGIF_APB_MMAP_CAL_LDO_VOUT_CTRL_OFFSET 3
#define AUX_DIGIF_APB_MMAP_CAL_LDO_VOUT_CTRL_SIZE 4

/** bit field defines for aux_digif_apb_mmap_s#aux_clk */
#define AUX_DIGIF_APB_MMAP_AUX_CLK_TX_SYN_REF_EN_OFFSET 0
#define AUX_DIGIF_APB_MMAP_AUX_CLK_TX_SYN_REF_EN_SIZE 1
#define AUX_DIGIF_APB_MMAP_AUX_CLK_RX_SYN_REF_EN_OFFSET 1
#define AUX_DIGIF_APB_MMAP_AUX_CLK_RX_SYN_REF_EN_SIZE 1
#define AUX_DIGIF_APB_MMAP_AUX_CLK_RXCA_SYN_REF_EN_OFFSET 2
#define AUX_DIGIF_APB_MMAP_AUX_CLK_RXCA_SYN_REF_EN_SIZE 1
#define AUX_DIGIF_APB_MMAP_AUX_CLK_DIG_CORE_REF_CLK_X2_EN_OFFSET 3
#define AUX_DIGIF_APB_MMAP_AUX_CLK_DIG_CORE_REF_CLK_X2_EN_SIZE 1
#define AUX_DIGIF_APB_MMAP_AUX_CLK_TX_REF_EN_OFFSET 4
#define AUX_DIGIF_APB_MMAP_AUX_CLK_TX_REF_EN_SIZE 1
#define AUX_DIGIF_APB_MMAP_AUX_CLK_RX_REF_EN_OFFSET 5
#define AUX_DIGIF_APB_MMAP_AUX_CLK_RX_REF_EN_SIZE 1
#define AUX_DIGIF_APB_MMAP_AUX_CLK_AUX_REF_EN_OFFSET 6
#define AUX_DIGIF_APB_MMAP_AUX_CLK_AUX_REF_EN_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#adc_ldo_auxtop */
#define AUX_DIGIF_APB_MMAP_ADC_LDO_AUXTOP_BYPASS_OFFSET 0
#define AUX_DIGIF_APB_MMAP_ADC_LDO_AUXTOP_BYPASS_SIZE 1
#define AUX_DIGIF_APB_MMAP_ADC_LDO_AUXTOP_ENABLE_OFFSET 1
#define AUX_DIGIF_APB_MMAP_ADC_LDO_AUXTOP_ENABLE_SIZE 1
#define AUX_DIGIF_APB_MMAP_ADC_LDO_AUXTOP_FAST_LOCK_OFFSET 2
#define AUX_DIGIF_APB_MMAP_ADC_LDO_AUXTOP_FAST_LOCK_SIZE 1
#define AUX_DIGIF_APB_MMAP_ADC_LDO_AUXTOP_VOUT_CTRL_OFFSET 3
#define AUX_DIGIF_APB_MMAP_ADC_LDO_AUXTOP_VOUT_CTRL_SIZE 4

/** bit field defines for aux_digif_apb_mmap_s#enables */
#define AUX_DIGIF_APB_MMAP_ENABLES_ADC_I_EN_OFFSET 0
#define AUX_DIGIF_APB_MMAP_ENABLES_ADC_I_EN_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#temp_enables */
#define AUX_DIGIF_APB_MMAP_TEMP_ENABLES_TEMPCELL_EN_OFFSET 0
#define AUX_DIGIF_APB_MMAP_TEMP_ENABLES_TEMPCELL_EN_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#atst_aux_auxtop */
#define AUX_DIGIF_APB_MMAP_ATST_AUX_AUXTOP_MASTER_EN_OFFSET 0
#define AUX_DIGIF_APB_MMAP_ATST_AUX_AUXTOP_MASTER_EN_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_AUX_AUXTOP_MASTER_PULLDOWN_OFFSET 1
#define AUX_DIGIF_APB_MMAP_ATST_AUX_AUXTOP_MASTER_PULLDOWN_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_AUX_AUXTOP_AUXADC_EN_OFFSET 2
#define AUX_DIGIF_APB_MMAP_ATST_AUX_AUXTOP_AUXADC_EN_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#atst_enables */
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_ADC_LDO_EN_A_OFFSET 0
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_ADC_LDO_EN_A_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_REF_LDO_EN_A_OFFSET 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_REF_LDO_EN_A_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_ADC_I_EN_A_OFFSET 2
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_ADC_I_EN_A_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_ADC_I_EN_B_OFFSET 3
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_ADC_I_EN_B_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_AFC_DAC_EN_A_OFFSET 4
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_AFC_DAC_EN_A_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_INPUT_COND_P_A_OFFSET 5
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_INPUT_COND_P_A_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_INPUT_COND_P_B_OFFSET 6
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_INPUT_COND_P_B_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_INPUT_COND_N_A_OFFSET 7
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_INPUT_COND_N_A_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_INPUT_COND_N_B_OFFSET 8
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_INPUT_COND_N_B_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_TEMPCELL_EN_A_OFFSET 9
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_TEMPCELL_EN_A_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_TEMPCELL_EN_B_OFFSET 10
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_TEMPCELL_EN_B_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_TEMPCELL_EN_C_OFFSET 11
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_TEMPCELL_EN_C_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_TEMPCELL_EN_D_OFFSET 12
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_TEMPCELL_EN_D_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_TEMPCELL_EN_E_OFFSET 13
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_TEMPCELL_EN_E_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_TEMPCELL_EN_F_OFFSET 14
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_TEMPCELL_EN_F_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_CAL_EN_A_OFFSET 15
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_CAL_EN_A_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_CAL_EN_B_OFFSET 16
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_CAL_EN_B_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_CAL_EN_C_OFFSET 17
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_CAL_EN_C_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_CAL_LDO_EN_A_OFFSET 18
#define AUX_DIGIF_APB_MMAP_ATST_ENABLES_CAL_LDO_EN_A_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#atst_toplevel_auxtop */
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_ADC_LDO_EN_A_OFFSET 0
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_ADC_LDO_EN_A_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_REF_LDO_EN_A_OFFSET 1
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_REF_LDO_EN_A_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_ADC_I_EN_A_OFFSET 2
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_ADC_I_EN_A_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_ADC_I_EN_B_OFFSET 3
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_ADC_I_EN_B_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_INPUT_COND_P_A_OFFSET 4
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_INPUT_COND_P_A_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_INPUT_COND_P_B_OFFSET 5
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_INPUT_COND_P_B_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_INPUT_COND_N_A_OFFSET 6
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_INPUT_COND_N_A_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_INPUT_COND_N_B_OFFSET 7
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_INPUT_COND_N_B_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_TEMPCELL_EN_A_OFFSET 8
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_TEMPCELL_EN_A_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_TEMPCELL_EN_B_OFFSET 9
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_TEMPCELL_EN_B_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_TEMPCELL_EN_C_OFFSET 10
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_TEMPCELL_EN_C_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_TEMPCELL_EN_D_OFFSET 11
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_TEMPCELL_EN_D_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_TEMPCELL_EN_E_OFFSET 12
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_TEMPCELL_EN_E_SIZE 1
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_TEMPCELL_EN_F_OFFSET 13
#define AUX_DIGIF_APB_MMAP_ATST_TOPLEVEL_AUXTOP_TEMPCELL_EN_F_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#input_cond */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_RES_TRIM_OFFSET 0
#define AUX_DIGIF_APB_MMAP_INPUT_COND_RES_TRIM_SIZE 4
#define AUX_DIGIF_APB_MMAP_INPUT_COND_COMP_CAP_OFFSET 4
#define AUX_DIGIF_APB_MMAP_INPUT_COND_COMP_CAP_SIZE 2
#define AUX_DIGIF_APB_MMAP_INPUT_COND_ITRIM_OUTPUT_STAGE_OFFSET 6
#define AUX_DIGIF_APB_MMAP_INPUT_COND_ITRIM_OUTPUT_STAGE_SIZE 2
#define AUX_DIGIF_APB_MMAP_INPUT_COND_ITRIM_INPUT_STAGE_OFFSET 8
#define AUX_DIGIF_APB_MMAP_INPUT_COND_ITRIM_INPUT_STAGE_SIZE 2
#define AUX_DIGIF_APB_MMAP_INPUT_COND_SPARES_OFFSET 10
#define AUX_DIGIF_APB_MMAP_INPUT_COND_SPARES_SIZE 4

/** bit field defines for aux_digif_apb_mmap_s#input_cond_modes */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_MODES_COND_SELECT_OFFSET 0
#define AUX_DIGIF_APB_MMAP_INPUT_COND_MODES_COND_SELECT_SIZE 4
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_MODES_COND_SELECT_AUX_VALUE 13
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_MODES_COND_SELECT_DISABLED_VALUE 0
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_MODES_COND_SELECT_MX1_VALUE 7
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_MODES_COND_SELECT_MX2_VALUE 8
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_MODES_COND_SELECT_RX1_VALUE 1
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_MODES_COND_SELECT_RX2_VALUE 2
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_MODES_COND_SELECT_RXCA1_VALUE 3
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_MODES_COND_SELECT_RXCA2_VALUE 4
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_MODES_COND_SELECT_RXCA_SYN_VALUE 10
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_MODES_COND_SELECT_RX_SYN_VALUE 9
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_MODES_COND_SELECT_TX1_VALUE 5
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_MODES_COND_SELECT_TX2_VALUE 6
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_MODES_COND_SELECT_TX_SYN_N_VALUE 12
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_MODES_COND_SELECT_TX_SYN_P_VALUE 11

/** bit field defines for aux_digif_apb_mmap_s#input_ref_sel */
#define AUX_DIGIF_APB_MMAP_INPUT_REF_SEL_COND_SELECT_OFFSET 0
#define AUX_DIGIF_APB_MMAP_INPUT_REF_SEL_COND_SELECT_SIZE 2
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_REF_SEL_COND_SELECT_COMM_VALUE 1
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_REF_SEL_COND_SELECT_GND_VALUE 0

/** bit field defines for aux_digif_apb_mmap_s#input_cond_temp_enables */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_P_SELECT_OFFSET 0
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_P_SELECT_SIZE 4
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_P_SELECT_AUX_VALUE 12
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_P_SELECT_DISABLED_VALUE 0
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_P_SELECT_GND_VALUE 13
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_P_SELECT_MX1_VALUE 7
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_P_SELECT_MX2_VALUE 8
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_P_SELECT_RX1_VALUE 1
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_P_SELECT_RX2_VALUE 2
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_P_SELECT_RXCA1_VALUE 3
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_P_SELECT_RXCA2_VALUE 4
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_P_SELECT_RXCA_SYN_VALUE 10
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_P_SELECT_RX_SYN_VALUE 9
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_P_SELECT_TX1_VALUE 5
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_P_SELECT_TX2_VALUE 6
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_P_SELECT_TX_SYN_VALUE 11
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_N_SELECT_OFFSET 4
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_N_SELECT_SIZE 4
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_N_SELECT_AUX_VALUE 12
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_N_SELECT_DISABLED_VALUE 0
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_N_SELECT_GND_VALUE 13
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_N_SELECT_MX1_VALUE 7
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_N_SELECT_MX2_VALUE 8
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_N_SELECT_RX1_VALUE 1
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_N_SELECT_RX2_VALUE 2
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_N_SELECT_RXCA1_VALUE 3
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_N_SELECT_RXCA2_VALUE 4
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_N_SELECT_RXCA_SYN_VALUE 10
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_N_SELECT_RX_SYN_VALUE 9
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_N_SELECT_TX1_VALUE 5
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_N_SELECT_TX2_VALUE 6
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_TEMP_ENABLES_TEMP_N_SELECT_TX_SYN_VALUE 11

/** bit field defines for aux_digif_apb_mmap_s#input_cond_et_select */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_ET_SELECT_ET_FILT_P_SELECT_OFFSET 0
#define AUX_DIGIF_APB_MMAP_INPUT_COND_ET_SELECT_ET_FILT_P_SELECT_SIZE 2
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_ET_SELECT_ET_FILT_P_SELECT_DISABLED_VALUE 0
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_ET_SELECT_ET_FILT_P_SELECT_TX1_ET_VALUE 1
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_ET_SELECT_ET_FILT_P_SELECT_TX2_ET_VALUE 2
#define AUX_DIGIF_APB_MMAP_INPUT_COND_ET_SELECT_ET_FILT_N_SELECT_OFFSET 2
#define AUX_DIGIF_APB_MMAP_INPUT_COND_ET_SELECT_ET_FILT_N_SELECT_SIZE 2
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_ET_SELECT_ET_FILT_N_SELECT_DISABLED_VALUE 0
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_ET_SELECT_ET_FILT_N_SELECT_TX1_ET_VALUE 1
/**  */
#define AUX_DIGIF_APB_MMAP_INPUT_COND_ET_SELECT_ET_FILT_N_SELECT_TX2_ET_VALUE 2

/** bit field defines for aux_digif_apb_mmap_s#adc_clkmux_auxtop */
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_CLKIN_SEL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_CLKIN_SEL_SIZE 2
/**  */
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_CLKIN_SEL_NO_CLOCK_VALUE 0
/**  */
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_CLKIN_SEL_REF_CLKCLEAN_VALUE 2
/**  */
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_CLKIN_SEL_REF_CLOCK_VALUE 1
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_ADC_CLK_SEL_OFFSET 2
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_ADC_CLK_SEL_SIZE 2
/**  */
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_ADC_CLK_SEL_DIV2_VALUE 3
/**  */
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_ADC_CLK_SEL_NO_CLOCK_VALUE 0
/**  */
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_ADC_CLK_SEL_UNDIV_VALUE 2
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_DIG_CLK_SEL_OFFSET 4
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_DIG_CLK_SEL_SIZE 2
/**  */
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_DIG_CLK_SEL_DIV2_VALUE 3
/**  */
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_DIG_CLK_SEL_NO_CLOCK_VALUE 0
/**  */
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_DIG_CLK_SEL_UNDIV_VALUE 2
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_TX_CLK_SEL_OFFSET 6
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_TX_CLK_SEL_SIZE 2
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_ADC_DFE_CLK_INVERT_OFFSET 8
#define AUX_DIGIF_APB_MMAP_ADC_CLKMUX_AUXTOP_ADC_DFE_CLK_INVERT_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#bandgap_auxtop */
#define AUX_DIGIF_APB_MMAP_BANDGAP_AUXTOP_VOUT_CTRL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_BANDGAP_AUXTOP_VOUT_CTRL_SIZE 4
#define AUX_DIGIF_APB_MMAP_BANDGAP_AUXTOP_INPUT_SEL_OFFSET 4
#define AUX_DIGIF_APB_MMAP_BANDGAP_AUXTOP_INPUT_SEL_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#afc_dac */
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_WORD_UPPER_OFFSET 0
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_WORD_UPPER_SIZE 11
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_MODE_SEL_OFFSET 11
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_MODE_SEL_SIZE 1
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_DAC_IREF_EN_OFFSET 12
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_DAC_IREF_EN_SIZE 1
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_DAC_DISABLE_TO_MID_OFFSET 13
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_DAC_DISABLE_TO_MID_SIZE 1
/**  */
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_DAC_DISABLE_TO_MID_FORCED_TO_GROUND_VALUE 1
/**  */
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_DAC_DISABLE_TO_MID_FORCED_TO_MIDRAIL_VALUE 0
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_ITRIM_SELECT_OFFSET 14
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_ITRIM_SELECT_SIZE 4
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_CLK_INVERT_EN_OFFSET 18
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_CLK_INVERT_EN_SIZE 1
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_CLK_DELAY_SELECT_OFFSET 19
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_CLK_DELAY_SELECT_SIZE 2
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_CLK_PULSE_BYPASS_EN_OFFSET 21
#define AUX_DIGIF_APB_MMAP_AFC_DAC_AFC_CLK_PULSE_BYPASS_EN_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#afc_dac2 */
#define AUX_DIGIF_APB_MMAP_AFC_DAC2_AFC_WORD_LOWER_OFFSET 0
#define AUX_DIGIF_APB_MMAP_AFC_DAC2_AFC_WORD_LOWER_SIZE 30

/** bit field defines for aux_digif_apb_mmap_s#afc_dac_ctrl_rdback */
#define AUX_DIGIF_APB_MMAP_AFC_DAC_CTRL_RDBACK_AFC_WORD_UPPER_OFFSET 0
#define AUX_DIGIF_APB_MMAP_AFC_DAC_CTRL_RDBACK_AFC_WORD_UPPER_SIZE 11

/** bit field defines for aux_digif_apb_mmap_s#afc_dac2_ctrl_rdback */
#define AUX_DIGIF_APB_MMAP_AFC_DAC2_CTRL_RDBACK_AFC_WORD_LOWER_OFFSET 0
#define AUX_DIGIF_APB_MMAP_AFC_DAC2_CTRL_RDBACK_AFC_WORD_LOWER_SIZE 30

/** bit field defines for aux_digif_apb_mmap_s#cal */
#define AUX_DIGIF_APB_MMAP_CAL_EN_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CAL_EN_SIZE 1
#define AUX_DIGIF_APB_MMAP_CAL_IBIAS_OFFSET 1
#define AUX_DIGIF_APB_MMAP_CAL_IBIAS_SIZE 2
/**  */
#define AUX_DIGIF_APB_MMAP_CAL_IBIAS_DEF_VALUE 2
/**  */
#define AUX_DIGIF_APB_MMAP_CAL_IBIAS_MAX_VALUE 3
/**  */
#define AUX_DIGIF_APB_MMAP_CAL_IBIAS_MIN_VALUE 0
#define AUX_DIGIF_APB_MMAP_CAL_CONSTGM_OFFSET 3
#define AUX_DIGIF_APB_MMAP_CAL_CONSTGM_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#cal_gain */
#define AUX_DIGIF_APB_MMAP_CAL_GAIN_GM_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CAL_GAIN_GM_SIZE 3
/**  */
#define AUX_DIGIF_APB_MMAP_CAL_GAIN_GM_MAX_VALUE 7
/**  */
#define AUX_DIGIF_APB_MMAP_CAL_GAIN_GM_MIN_VALUE 0
#define AUX_DIGIF_APB_MMAP_CAL_GAIN_FB_OFFSET 3
#define AUX_DIGIF_APB_MMAP_CAL_GAIN_FB_SIZE 1
#define AUX_DIGIF_APB_MMAP_CAL_GAIN_CASN_OFFSET 4
#define AUX_DIGIF_APB_MMAP_CAL_GAIN_CASN_SIZE 3
/**  */
#define AUX_DIGIF_APB_MMAP_CAL_GAIN_CASN_MAX_VALUE 7
/**  */
#define AUX_DIGIF_APB_MMAP_CAL_GAIN_CASN_MIN_VALUE 0
#define AUX_DIGIF_APB_MMAP_CAL_GAIN_CASP_OFFSET 7
#define AUX_DIGIF_APB_MMAP_CAL_GAIN_CASP_SIZE 3
/**  */
#define AUX_DIGIF_APB_MMAP_CAL_GAIN_CASP_MAX_VALUE 7
/**  */
#define AUX_DIGIF_APB_MMAP_CAL_GAIN_CASP_MIN_VALUE 0

/** bit field defines for aux_digif_apb_mmap_s#spares */
#define AUX_DIGIF_APB_MMAP_SPARES_SPARES_OFFSET 0
#define AUX_DIGIF_APB_MMAP_SPARES_SPARES_SIZE 32
/**  */
#define AUX_DIGIF_APB_MMAP_SPARES_SPARES_AFC_GMIBIAS_VALUE 128
/**  */
#define AUX_DIGIF_APB_MMAP_SPARES_SPARES_ANA_IREF_VALUE 64
/**  */
#define AUX_DIGIF_APB_MMAP_SPARES_SPARES_CORE_LDO_BYPASS_VALUE 8192
/**  */
#define AUX_DIGIF_APB_MMAP_SPARES_SPARES_CORE_LDO_VOUT_CTRL_SHIFT_VALUE 14
/**  */
#define AUX_DIGIF_APB_MMAP_SPARES_SPARES_DIG_LDO_150MA_VOUT_CTRL_LEVEL_VALUE 10
/**  */
#define AUX_DIGIF_APB_MMAP_SPARES_SPARES_INPUT_COND_EN_VALUE 1
/**  */
#define AUX_DIGIF_APB_MMAP_SPARES_SPARES_LDO_IBIAS_VALUE 0
/**  */
#define AUX_DIGIF_APB_MMAP_SPARES_SPARES_MPHY_LDO_BYPASS_VALUE 256
/**  */
#define AUX_DIGIF_APB_MMAP_SPARES_SPARES_MPHY_LDO_VOUT_CTRL_SHIFT_VALUE 9

/** bit field defines for aux_digif_apb_mmap_s#dbg_mmap */
#define AUX_DIGIF_APB_MMAP_DBG_MMAP_MEMMAP_VERSION_SHDW_OFFSET 0
#define AUX_DIGIF_APB_MMAP_DBG_MMAP_MEMMAP_VERSION_SHDW_SIZE 16
#define AUX_DIGIF_APB_MMAP_DBG_MMAP_MEMMAP_VERSION_CFG_OFFSET 16
#define AUX_DIGIF_APB_MMAP_DBG_MMAP_MEMMAP_VERSION_CFG_SIZE 16

/** bit field defines for aux_digif_apb_mmap_s#adc_mode_i */
#define AUX_DIGIF_APB_MMAP_ADC_MODE_I_ADCMODE_OFFSET 0
#define AUX_DIGIF_APB_MMAP_ADC_MODE_I_ADCMODE_SIZE 2
/**  */
#define AUX_DIGIF_APB_MMAP_ADC_MODE_I_ADCMODE_MAN_VALUE 0
/**  */
#define AUX_DIGIF_APB_MMAP_ADC_MODE_I_ADCMODE_OFFLINE_VALUE 1
/**  */
#define AUX_DIGIF_APB_MMAP_ADC_MODE_I_ADCMODE_ONLINE_VALUE 2
#define AUX_DIGIF_APB_MMAP_ADC_MODE_I_INTERLEAVEMODE_OFFSET 2
#define AUX_DIGIF_APB_MMAP_ADC_MODE_I_INTERLEAVEMODE_SIZE 1
/**  */
#define AUX_DIGIF_APB_MMAP_ADC_MODE_I_INTERLEAVEMODE_OFF_VALUE 0
/**  */
#define AUX_DIGIF_APB_MMAP_ADC_MODE_I_INTERLEAVEMODE_ON_VALUE 1
#define AUX_DIGIF_APB_MMAP_ADC_MODE_I_CH1EN_OFFSET 3
#define AUX_DIGIF_APB_MMAP_ADC_MODE_I_CH1EN_SIZE 1
#define AUX_DIGIF_APB_MMAP_ADC_MODE_I_CH2EN_OFFSET 4
#define AUX_DIGIF_APB_MMAP_ADC_MODE_I_CH2EN_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#adc_cal_status */
#define AUX_DIGIF_APB_MMAP_ADC_CAL_STATUS_CHI1OFFLINECALDONE_OFFSET 0
#define AUX_DIGIF_APB_MMAP_ADC_CAL_STATUS_CHI1OFFLINECALDONE_SIZE 1
#define AUX_DIGIF_APB_MMAP_ADC_CAL_STATUS_CHI2OFFLINECALDONE_OFFSET 1
#define AUX_DIGIF_APB_MMAP_ADC_CAL_STATUS_CHI2OFFLINECALDONE_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#adc_i_calclk */
#define AUX_DIGIF_APB_MMAP_ADC_I_CALCLK_CALCLKDIV_OFFSET 0
#define AUX_DIGIF_APB_MMAP_ADC_I_CALCLK_CALCLKDIV_SIZE 4

/** bit field defines for aux_digif_apb_mmap_s#adc_i_dither */
#define AUX_DIGIF_APB_MMAP_ADC_I_DITHER_DITHEREN_OFFSET 0
#define AUX_DIGIF_APB_MMAP_ADC_I_DITHER_DITHEREN_SIZE 1
#define AUX_DIGIF_APB_MMAP_ADC_I_DITHER_DITHERREVERT_OFFSET 1
#define AUX_DIGIF_APB_MMAP_ADC_I_DITHER_DITHERREVERT_SIZE 1
#define AUX_DIGIF_APB_MMAP_ADC_I_DITHER_DITHERDELAY_OFFSET 2
#define AUX_DIGIF_APB_MMAP_ADC_I_DITHER_DITHERDELAY_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#adc_iqmux */
#define AUX_DIGIF_APB_MMAP_ADC_IQMUX_EN_I_OFFSET 0
#define AUX_DIGIF_APB_MMAP_ADC_IQMUX_EN_I_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#adc_cal_readback_ctrl */
#define AUX_DIGIF_APB_MMAP_ADC_CAL_READBACK_CTRL_SELECTI_OFFSET 0
#define AUX_DIGIF_APB_MMAP_ADC_CAL_READBACK_CTRL_SELECTI_SIZE 5

/** bit field defines for aux_digif_apb_mmap_s#adc_cal_readback_i */
#define AUX_DIGIF_APB_MMAP_ADC_CAL_READBACK_I_RESULT_OFFSET 0
#define AUX_DIGIF_APB_MMAP_ADC_CAL_READBACK_I_RESULT_SIZE 32

/** bit field defines for aux_digif_apb_mmap_s#chi1_cal */
#define AUX_DIGIF_APB_MMAP_CHI1_CAL_CH1CALMODE_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_CAL_CH1CALMODE_SIZE 2
#define AUX_DIGIF_APB_MMAP_CHI1_CAL_SHORTADCINPUT_OFFSET 2
#define AUX_DIGIF_APB_MMAP_CHI1_CAL_SHORTADCINPUT_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#chi1_calstagectrl */
#define AUX_DIGIF_APB_MMAP_CHI1_CALSTAGECTRL_STAGEDISABLE_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_CALSTAGECTRL_STAGEDISABLE_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_channelgain */
#define AUX_DIGIF_APB_MMAP_CHI1_CHANNELGAIN_CH1CHANNELGAIN_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_CHANNELGAIN_CH1CHANNELGAIN_SIZE 5

/** bit field defines for aux_digif_apb_mmap_s#chi1_dacn1 */
#define AUX_DIGIF_APB_MMAP_CHI1_DACN1_CH1CAPCONTROLN1_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_DACN1_CH1CAPCONTROLN1_SIZE 9

/** bit field defines for aux_digif_apb_mmap_s#chi1_dacp1 */
#define AUX_DIGIF_APB_MMAP_CHI1_DACP1_CH1CAPCONTROLP1_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_DACP1_CH1CAPCONTROLP1_SIZE 9

/** bit field defines for aux_digif_apb_mmap_s#chi1_dacn2 */
#define AUX_DIGIF_APB_MMAP_CHI1_DACN2_CH1CAPCONTROLN2_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_DACN2_CH1CAPCONTROLN2_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_dacp2 */
#define AUX_DIGIF_APB_MMAP_CHI1_DACP2_CH1CAPCONTROLP2_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_DACP2_CH1CAPCONTROLP2_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_amp */
#define AUX_DIGIF_APB_MMAP_CHI1_AMP_CH1AMPGAINCALP_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_AMP_CH1AMPGAINCALP_SIZE 7
#define AUX_DIGIF_APB_MMAP_CHI1_AMP_CH1AMPGAINCALN_OFFSET 7
#define AUX_DIGIF_APB_MMAP_CHI1_AMP_CH1AMPGAINCALN_SIZE 7
#define AUX_DIGIF_APB_MMAP_CHI1_AMP_CH1INCREASEAMPDELAY_OFFSET 14
#define AUX_DIGIF_APB_MMAP_CHI1_AMP_CH1INCREASEAMPDELAY_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#chi1_msbcal */
#define AUX_DIGIF_APB_MMAP_CHI1_MSBCAL_CH1MSBCAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_MSBCAL_CH1MSBCAL_SIZE 10

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp12calp */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP12CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP12CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp12caln */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP12CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP12CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp11calp */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP11CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP11CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp11caln */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP11CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP11CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp10calp */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP10CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP10CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp10caln */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP10CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP10CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp9calp */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP9CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP9CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp9caln */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP9CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP9CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp8calp */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP8CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP8CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp8caln */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP8CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP8CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp7calp */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP7CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP7CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp7caln */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP7CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP7CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp6calp */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP6CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP6CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp6caln */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP6CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP6CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp5calp */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP5CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP5CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp5caln */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP5CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP5CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp4calp */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP4CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP4CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp4caln */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP4CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP4CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp3calp */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP3CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP3CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp3caln */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP3CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP3CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp2calp */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP2CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP2CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp2caln */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP2CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP2CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp1calp */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP1CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP1CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp1caln */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP1CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP1CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp0calp */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP0CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP0CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_comp0caln */
#define AUX_DIGIF_APB_MMAP_CHI1_COMP0CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMP0CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi1_compdelay */
#define AUX_DIGIF_APB_MMAP_CHI1_COMPDELAY_COMPDELAY_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_COMPDELAY_COMPDELAY_SIZE 13

/** bit field defines for aux_digif_apb_mmap_s#chi1_fail_detect */
#define AUX_DIGIF_APB_MMAP_CHI1_FAIL_DETECT_CH1P1COMPFAILFLAG_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_FAIL_DETECT_CH1P1COMPFAILFLAG_SIZE 1
#define AUX_DIGIF_APB_MMAP_CHI1_FAIL_DETECT_CH1P2COMPFAILFLAG_OFFSET 1
#define AUX_DIGIF_APB_MMAP_CHI1_FAIL_DETECT_CH1P2COMPFAILFLAG_SIZE 1
#define AUX_DIGIF_APB_MMAP_CHI1_FAIL_DETECT_CH1AMPFAILFLAG_OFFSET 2
#define AUX_DIGIF_APB_MMAP_CHI1_FAIL_DETECT_CH1AMPFAILFLAG_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#chi1_spare */
#define AUX_DIGIF_APB_MMAP_CHI1_SPARE_SPARE_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_SPARE_SPARE_SIZE 16

/** bit field defines for aux_digif_apb_mmap_s#chi1_n_values */
#define AUX_DIGIF_APB_MMAP_CHI1_N_VALUES_N_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_N_VALUES_N_SIZE 4
#define AUX_DIGIF_APB_MMAP_CHI1_N_VALUES_N2_OFFSET 4
#define AUX_DIGIF_APB_MMAP_CHI1_N_VALUES_N2_SIZE 4
#define AUX_DIGIF_APB_MMAP_CHI1_N_VALUES_N4_OFFSET 8
#define AUX_DIGIF_APB_MMAP_CHI1_N_VALUES_N4_SIZE 4

/** bit field defines for aux_digif_apb_mmap_s#chi1_threshold12 */
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD12_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD12_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi1_threshold11 */
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD11_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD11_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi1_threshold10 */
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD10_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD10_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi1_threshold9 */
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD9_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD9_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi1_threshold8 */
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD8_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD8_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi1_threshold7 */
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD7_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD7_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi1_threshold6 */
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD6_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD6_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi1_threshold5 */
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD5_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD5_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi1_threshold4 */
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD4_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD4_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi1_threshold3 */
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD3_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD3_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi1_threshold2 */
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD2_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI1_THRESHOLD2_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi2_cal */
#define AUX_DIGIF_APB_MMAP_CHI2_CAL_CH2CALMODE_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_CAL_CH2CALMODE_SIZE 2
#define AUX_DIGIF_APB_MMAP_CHI2_CAL_SHORTADCINPUT_OFFSET 2
#define AUX_DIGIF_APB_MMAP_CHI2_CAL_SHORTADCINPUT_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#chi2_calstagectrl */
#define AUX_DIGIF_APB_MMAP_CHI2_CALSTAGECTRL_STAGEDISABLE_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_CALSTAGECTRL_STAGEDISABLE_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_channelgain */
#define AUX_DIGIF_APB_MMAP_CHI2_CHANNELGAIN_CH2CHANNELGAIN_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_CHANNELGAIN_CH2CHANNELGAIN_SIZE 5

/** bit field defines for aux_digif_apb_mmap_s#chi2_dacn1 */
#define AUX_DIGIF_APB_MMAP_CHI2_DACN1_CH2CAPCONTROLN1_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_DACN1_CH2CAPCONTROLN1_SIZE 9

/** bit field defines for aux_digif_apb_mmap_s#chi2_dacp1 */
#define AUX_DIGIF_APB_MMAP_CHI2_DACP1_CH2CAPCONTROLP1_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_DACP1_CH2CAPCONTROLP1_SIZE 9

/** bit field defines for aux_digif_apb_mmap_s#chi2_dacn2 */
#define AUX_DIGIF_APB_MMAP_CHI2_DACN2_CH2CAPCONTROLN2_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_DACN2_CH2CAPCONTROLN2_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_dacp2 */
#define AUX_DIGIF_APB_MMAP_CHI2_DACP2_CH2CAPCONTROLP2_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_DACP2_CH2CAPCONTROLP2_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_amp */
#define AUX_DIGIF_APB_MMAP_CHI2_AMP_CH2AMPGAINCALP_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_AMP_CH2AMPGAINCALP_SIZE 7
#define AUX_DIGIF_APB_MMAP_CHI2_AMP_CH2AMPGAINCALN_OFFSET 7
#define AUX_DIGIF_APB_MMAP_CHI2_AMP_CH2AMPGAINCALN_SIZE 7
#define AUX_DIGIF_APB_MMAP_CHI2_AMP_CH2INCREASEAMPDELAY_OFFSET 14
#define AUX_DIGIF_APB_MMAP_CHI2_AMP_CH2INCREASEAMPDELAY_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#chi2_msbcal */
#define AUX_DIGIF_APB_MMAP_CHI2_MSBCAL_CH2MSBCAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_MSBCAL_CH2MSBCAL_SIZE 10

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp12calp */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP12CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP12CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp12caln */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP12CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP12CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp11calp */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP11CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP11CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp11caln */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP11CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP11CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp10calp */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP10CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP10CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp10caln */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP10CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP10CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp9calp */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP9CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP9CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp9caln */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP9CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP9CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp8calp */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP8CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP8CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp8caln */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP8CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP8CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp7calp */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP7CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP7CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp7caln */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP7CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP7CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp6calp */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP6CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP6CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp6caln */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP6CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP6CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp5calp */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP5CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP5CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp5caln */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP5CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP5CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp4calp */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP4CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP4CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp4caln */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP4CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP4CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp3calp */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP3CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP3CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp3caln */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP3CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP3CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp2calp */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP2CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP2CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp2caln */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP2CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP2CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp1calp */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP1CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP1CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp1caln */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP1CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP1CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp0calp */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP0CALP_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP0CALP_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_comp0caln */
#define AUX_DIGIF_APB_MMAP_CHI2_COMP0CALN_COMP_CAL_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMP0CALN_COMP_CAL_SIZE 8

/** bit field defines for aux_digif_apb_mmap_s#chi2_compdelay */
#define AUX_DIGIF_APB_MMAP_CHI2_COMPDELAY_COMPDELAY_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_COMPDELAY_COMPDELAY_SIZE 13

/** bit field defines for aux_digif_apb_mmap_s#chi2_fail_detect */
#define AUX_DIGIF_APB_MMAP_CHI2_FAIL_DETECT_CH2P1COMPFAILFLAG_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_FAIL_DETECT_CH2P1COMPFAILFLAG_SIZE 1
#define AUX_DIGIF_APB_MMAP_CHI2_FAIL_DETECT_CH2P2COMPFAILFLAG_OFFSET 1
#define AUX_DIGIF_APB_MMAP_CHI2_FAIL_DETECT_CH2P2COMPFAILFLAG_SIZE 1
#define AUX_DIGIF_APB_MMAP_CHI2_FAIL_DETECT_CH2AMPFAILFLAG_OFFSET 2
#define AUX_DIGIF_APB_MMAP_CHI2_FAIL_DETECT_CH2AMPFAILFLAG_SIZE 1

/** bit field defines for aux_digif_apb_mmap_s#chi2_spare */
#define AUX_DIGIF_APB_MMAP_CHI2_SPARE_SPARE_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_SPARE_SPARE_SIZE 16

/** bit field defines for aux_digif_apb_mmap_s#chi2_n_values */
#define AUX_DIGIF_APB_MMAP_CHI2_N_VALUES_N_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_N_VALUES_N_SIZE 4
#define AUX_DIGIF_APB_MMAP_CHI2_N_VALUES_N2_OFFSET 4
#define AUX_DIGIF_APB_MMAP_CHI2_N_VALUES_N2_SIZE 4
#define AUX_DIGIF_APB_MMAP_CHI2_N_VALUES_N4_OFFSET 8
#define AUX_DIGIF_APB_MMAP_CHI2_N_VALUES_N4_SIZE 4

/** bit field defines for aux_digif_apb_mmap_s#chi2_threshold12 */
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD12_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD12_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi2_threshold11 */
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD11_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD11_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi2_threshold10 */
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD10_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD10_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi2_threshold9 */
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD9_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD9_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi2_threshold8 */
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD8_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD8_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi2_threshold7 */
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD7_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD7_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi2_threshold6 */
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD6_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD6_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi2_threshold5 */
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD5_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD5_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi2_threshold4 */
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD4_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD4_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi2_threshold3 */
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD3_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD3_THRESHOLD_SIZE 6

/** bit field defines for aux_digif_apb_mmap_s#chi2_threshold2 */
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD2_THRESHOLD_OFFSET 0
#define AUX_DIGIF_APB_MMAP_CHI2_THRESHOLD2_THRESHOLD_SIZE 6

/* EOF aux_digif_apb_mmap.h */
#endif
