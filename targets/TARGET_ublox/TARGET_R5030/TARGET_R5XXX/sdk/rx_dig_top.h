#ifndef INCLUDED_RX_DIG_TOP
#define INCLUDED_RX_DIG_TOP
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** rx_dig_top HAL Spreadsheet version number */
#define RX_DIG_TOP_HAL_VERSION 8

/** This is the digital receiver path control register description
*/
struct rx_dig_top_s {
   /** rx post adc control at address offset 0x000, read-write */
   uint32_t padc_gain_ctl;
   /** rx mixer control 1 at address offset 0x004, read-write */
   uint32_t mix_ctl_1;
   /** rx mixer control 2 at address offset 0x008, read-write */
   uint32_t mix_ctl_2;
   /** rx wb dcoc control at address offset 0x00C, read-write */
   uint32_t wb_dcoc_ctl;
   /** wide band dcoc correction I terms at address offset 0x010, read-write */
   uint32_t wb_dcoc_corr_i;
   /** wide band dcoc correction I terms at address offset 0x014, read-write */
   uint32_t wb_dcoc_corr_q;
   /** rx nb dcoc control at address offset 0x018, read-write */
   uint32_t nb_dcoc_ctl;
   /** narrow band dcoc correction terms at address offset 0x01C, read-write */
   uint32_t nb_dcoc_corr;
   /** adc imbalance control at address offset 0x020, read-write */
   uint32_t adc_imb_ctrl;
   /** wide band iq correction conttol at address offset 0x024, read-write */
   uint32_t wb_iq_corr_ctrl;
   /** narrow band iq correction conttol at address offset 0x028, read-write */
   uint32_t nb_iq_corr_ctrl;
   /** initial samples control at address offset 0x02C, read-write */
   uint32_t init_sample_ctrl;
   /** rx histogram control at address offset 0x030, read-write */
   uint32_t hist_ctrl;
   /** filter control 1 at address offset 0x034, read-write */
   uint32_t filts_ctrl_1;
   /** filter control 2 at address offset 0x038, read-write */
   uint32_t filts_ctrl_2;
   /** filter control 3 at address offset 0x03C, read-write */
   uint32_t filts_ctrl_3;
   /** programmable_filter taps at address offset 0x040, read-write */
   uint32_t sel_coeffs_1;
   /** programmable_filter taps at address offset 0x044, read-write */
   uint32_t sel_coeffs_2;
   /** programmable_filter taps at address offset 0x048, read-write */
   uint32_t sel_coeffs_3;
   /** programmable_filter taps at address offset 0x04C, read-write */
   uint32_t sel_coeffs_4;
   /** programmable_filter taps at address offset 0x050, read-write */
   uint32_t sel_coeffs_5;
   /** programmable_filter taps at address offset 0x054, read-write */
   uint32_t sel_coeffs_6;
   /** programmable_filter taps at address offset 0x058, read-write */
   uint32_t sel_coeffs_7;
   /** wideband dcoc I estimates at address offset 0x05C, read-only */
   uint32_t wb_dcoc_est_i;
   /** wideband dcoc Q estimates at address offset 0x060, read-only */
   uint32_t wb_dcoc_est_q;
   /**  narrow band dcoc estimates at address offset 0x064, read-only */
   uint32_t nb_dcoc_ests;
   /** rx status flags at address offset 0x068, read-only */
   uint32_t rx_status;
   /** wideband power at address offset 0x06C, read-only */
   uint32_t wb_pwr;
   /** narrow band power at address offset 0x070, read-only */
   uint32_t nb_pwr;
   /** wide band I corr est at address offset 0x074, read-only */
   uint32_t corr_iwb;
   /** wide band Q corr est at address offset 0x078, read-only */
   uint32_t corr_qwb;
   /** wide band IQ corr est at address offset 0x07C, read-only */
   uint32_t corr_iqwb;
   /** narrow band I corr est at address offset 0x080, read-only */
   uint32_t corr_inb;
   /** narrow band Q corr est at address offset 0x084, read-only */
   uint32_t corr_qnb;
   /** narrow band IQ corr est at address offset 0x088, read-only */
   uint32_t corr_iqnb;
   /** wide band I corr odd at address offset 0x08C, read-only */
   uint32_t corr_iwb_1;
   /** wide band I corr even at address offset 0x090, read-only */
   uint32_t corr_iwb_2;
   /** wide band Q corr odd at address offset 0x094, read-only */
   uint32_t corr_qwb_1;
   /** wide band Q corr even at address offset 0x098, read-only */
   uint32_t corr_qwb_2;
   /** histogram I results at address offset 0x09C, read-only */
   uint32_t hist_i_0;
   /** histogram I results at address offset 0x0A0, read-only */
   uint32_t hist_i_1;
   /** histogram I results at address offset 0x0A4, read-only */
   uint32_t hist_i_2;
   /** histogram I results at address offset 0x0A8, read-only */
   uint32_t hist_i_3;
   /** histogram I results at address offset 0x0AC, read-only */
   uint32_t hist_i_4;
   /** histogram I results at address offset 0x0B0, read-only */
   uint32_t hist_i_5;
   /** histogram I results at address offset 0x0B4, read-only */
   uint32_t hist_i_6;
   /** histogram I results at address offset 0x0B8, read-only */
   uint32_t hist_i_7;
   /** histogram I results at address offset 0x0BC, read-only */
   uint32_t hist_i_8;
   /** histogram I results at address offset 0x0C0, read-only */
   uint32_t hist_i_9;
   /** histogram I results at address offset 0x0C4, read-only */
   uint32_t hist_i_10;
   /** histogram I results at address offset 0x0C8, read-only */
   uint32_t hist_i_11;
   /** histogram I results at address offset 0x0CC, read-only */
   uint32_t hist_i_12;
   /** histogram I results at address offset 0x0D0, read-only */
   uint32_t hist_i_13;
   /** histogram I results at address offset 0x0D4, read-only */
   uint32_t hist_i_14;
   /** histogram I results at address offset 0x0D8, read-only */
   uint32_t hist_i_15;
   /** histogram Q results at address offset 0x0DC, read-only */
   uint32_t hist_q_0;
   /** histogram Q results at address offset 0x0E0, read-only */
   uint32_t hist_q_1;
   /** histogram Q results at address offset 0x0E4, read-only */
   uint32_t hist_q_2;
   /** histogram Q results at address offset 0x0E8, read-only */
   uint32_t hist_q_3;
   /** histogram Q results at address offset 0x0EC, read-only */
   uint32_t hist_q_4;
   /** histogram Q results at address offset 0x0F0, read-only */
   uint32_t hist_q_5;
   /** histogram Q results at address offset 0x0F4, read-only */
   uint32_t hist_q_6;
   /** histogram Q results at address offset 0x0F8, read-only */
   uint32_t hist_q_7;
   /** histogram Q results at address offset 0x0FC, read-only */
   uint32_t hist_q_8;
   /** histogram Q results at address offset 0x100, read-only */
   uint32_t hist_q_9;
   /** histogram Q results at address offset 0x104, read-only */
   uint32_t hist_q_10;
   /** histogram Q results at address offset 0x108, read-only */
   uint32_t hist_q_11;
   /** histogram Q results at address offset 0x10C, read-only */
   uint32_t hist_q_12;
   /** histogram Q results at address offset 0x110, read-only */
   uint32_t hist_q_13;
   /** histogram Q results at address offset 0x114, read-only */
   uint32_t hist_q_14;
   /** histogram Q results at address offset 0x118, read-only */
   uint32_t hist_q_15;
   /** adc fault counts at address offset 0x11C, read-only */
   uint32_t adci1;
   /** adc fault counts at address offset 0x120, read-only */
   uint32_t adci2;
   /** adc fault counts at address offset 0x124, read-only */
   uint32_t adcq1;
   /** adc fault counts at address offset 0x128, read-only */
   uint32_t adcq2;
   /** adc conversion control at address offset 0x12C, read-write */
   uint32_t adc_conv;
   /** selects the signals that are ANDed to generate the offline cal done interrupt at address offset 0x130, read-write */
   uint32_t adc_cal_flag_en;
   /** selects the data source fed to the dfe output  at address offset 0x134, read-write */
   uint32_t adc_tmode;
   /** rx loop control at address offset 0x138, read-write */
   uint32_t rx_loop_ctl;
   /** filter control 4 at address offset 0x13C, read-write */
   uint32_t filts_ctrl_4;
   /** filter control 5 at address offset 0x140, read-write */
   uint32_t filts_ctrl_5;
   /** rx reset control at address offset 0x144, read-write */
   uint32_t rx_dfe_reset;
   /** rx dfe startup delay control  at address offset 0x148, read-write */
   uint32_t rx_dfe_strt_ctrl;
   /** rx dfe end sample control at address offset 0x14C, read-write */
   uint32_t rx_dfe_end_ctrl;
   /** Interrupts from the rx_dig_top block. at address offset 0x150, read-write */
   uint32_t rx_irq;
};

/** bit field defines for rx_dig_top_s#padc_gain_ctl */
#define RX_DIG_TOP_PADC_GAIN_CTL_PADC_SHIFT_OFFSET 0
#define RX_DIG_TOP_PADC_GAIN_CTL_PADC_SHIFT_SIZE 4
#define RX_DIG_TOP_PADC_GAIN_CTL_PADC_FGAIN_SEL_OFFSET 4
#define RX_DIG_TOP_PADC_GAIN_CTL_PADC_FGAIN_SEL_SIZE 4
#define RX_DIG_TOP_PADC_GAIN_CTL_PADC_FGAIN_EN_OFFSET 8
#define RX_DIG_TOP_PADC_GAIN_CTL_PADC_FGAIN_EN_SIZE 1
#define RX_DIG_TOP_PADC_GAIN_CTL_RX_SWAP_IQ_EN_OFFSET 9
#define RX_DIG_TOP_PADC_GAIN_CTL_RX_SWAP_IQ_EN_SIZE 1

/** bit field defines for rx_dig_top_s#mix_ctl_1 */
#define RX_DIG_TOP_MIX_CTL_1_RX_REG_C_OFFSET 0
#define RX_DIG_TOP_MIX_CTL_1_RX_REG_C_SIZE 10
#define RX_DIG_TOP_MIX_CTL_1_RX_REG_S_OFFSET 10
#define RX_DIG_TOP_MIX_CTL_1_RX_REG_S_SIZE 10
#define RX_DIG_TOP_MIX_CTL_1_RX_PHADJ_EN_OFFSET 20
#define RX_DIG_TOP_MIX_CTL_1_RX_PHADJ_EN_SIZE 1
#define RX_DIG_TOP_MIX_CTL_1_RX_MIX_EN_OFFSET 22
#define RX_DIG_TOP_MIX_CTL_1_RX_MIX_EN_SIZE 1

/** bit field defines for rx_dig_top_s#mix_ctl_2 */
#define RX_DIG_TOP_MIX_CTL_2_NCO_PHASE_INC_OFFSET 0
#define RX_DIG_TOP_MIX_CTL_2_NCO_PHASE_INC_SIZE 24

/** bit field defines for rx_dig_top_s#wb_dcoc_ctl */
#define RX_DIG_TOP_WB_DCOC_CTL_RX_DCOC_EST_EN_OFFSET 0
#define RX_DIG_TOP_WB_DCOC_CTL_RX_DCOC_EST_EN_SIZE 1
#define RX_DIG_TOP_WB_DCOC_CTL_RX_DCOC_CORR_EN_OFFSET 1
#define RX_DIG_TOP_WB_DCOC_CTL_RX_DCOC_CORR_EN_SIZE 1
#define RX_DIG_TOP_WB_DCOC_CTL_RX_WB_CORR_WIN_OFFSET 2
#define RX_DIG_TOP_WB_DCOC_CTL_RX_WB_CORR_WIN_SIZE 20
#define RX_DIG_TOP_WB_DCOC_CTL_RX_AUTO_DCOC_EN_OFFSET 22
#define RX_DIG_TOP_WB_DCOC_CTL_RX_AUTO_DCOC_EN_SIZE 1

/** bit field defines for rx_dig_top_s#wb_dcoc_corr_i */
#define RX_DIG_TOP_WB_DCOC_CORR_I_RX_DCOC_CORR_1_I_OFFSET 0
#define RX_DIG_TOP_WB_DCOC_CORR_I_RX_DCOC_CORR_1_I_SIZE 11
#define RX_DIG_TOP_WB_DCOC_CORR_I_RX_DCOC_CORR_2_I_OFFSET 11
#define RX_DIG_TOP_WB_DCOC_CORR_I_RX_DCOC_CORR_2_I_SIZE 11

/** bit field defines for rx_dig_top_s#wb_dcoc_corr_q */
#define RX_DIG_TOP_WB_DCOC_CORR_Q_RX_DCOC_CORR_1_Q_OFFSET 0
#define RX_DIG_TOP_WB_DCOC_CORR_Q_RX_DCOC_CORR_1_Q_SIZE 11
#define RX_DIG_TOP_WB_DCOC_CORR_Q_RX_DCOC_CORR_2_Q_OFFSET 11
#define RX_DIG_TOP_WB_DCOC_CORR_Q_RX_DCOC_CORR_2_Q_SIZE 11

/** bit field defines for rx_dig_top_s#nb_dcoc_ctl */
#define RX_DIG_TOP_NB_DCOC_CTL_RX_DCOC_NB_EST_EN_OFFSET 0
#define RX_DIG_TOP_NB_DCOC_CTL_RX_DCOC_NB_EST_EN_SIZE 1
#define RX_DIG_TOP_NB_DCOC_CTL_RX_DCOC_NB_CORR_EN_OFFSET 1
#define RX_DIG_TOP_NB_DCOC_CTL_RX_DCOC_NB_CORR_EN_SIZE 1
#define RX_DIG_TOP_NB_DCOC_CTL_RX_NB_CORR_WIN_OFFSET 2
#define RX_DIG_TOP_NB_DCOC_CTL_RX_NB_CORR_WIN_SIZE 20
#define RX_DIG_TOP_NB_DCOC_CTL_RX_AUTO_DCOC_NB_EN_OFFSET 22
#define RX_DIG_TOP_NB_DCOC_CTL_RX_AUTO_DCOC_NB_EN_SIZE 1

/** bit field defines for rx_dig_top_s#nb_dcoc_corr */
#define RX_DIG_TOP_NB_DCOC_CORR_RX_DCOC_CORR_NB_I_OFFSET 0
#define RX_DIG_TOP_NB_DCOC_CORR_RX_DCOC_CORR_NB_I_SIZE 13
#define RX_DIG_TOP_NB_DCOC_CORR_RX_DCOC_CORR_NB_Q_OFFSET 13
#define RX_DIG_TOP_NB_DCOC_CORR_RX_DCOC_CORR_NB_Q_SIZE 13

/** bit field defines for rx_dig_top_s#adc_imb_ctrl */
#define RX_DIG_TOP_ADC_IMB_CTRL_RX_ADCIMB_CORR_EN_OFFSET 0
#define RX_DIG_TOP_ADC_IMB_CTRL_RX_ADCIMB_CORR_EN_SIZE 1
#define RX_DIG_TOP_ADC_IMB_CTRL_RX_ADCIMB_ODDEN_I_OFFSET 1
#define RX_DIG_TOP_ADC_IMB_CTRL_RX_ADCIMB_ODDEN_I_SIZE 1
#define RX_DIG_TOP_ADC_IMB_CTRL_RX_ADCIMB_ODDEN_Q_OFFSET 2
#define RX_DIG_TOP_ADC_IMB_CTRL_RX_ADCIMB_ODDEN_Q_SIZE 1
#define RX_DIG_TOP_ADC_IMB_CTRL_RX_ADCIMB_CORR_I_OFFSET 3
#define RX_DIG_TOP_ADC_IMB_CTRL_RX_ADCIMB_CORR_I_SIZE 10
#define RX_DIG_TOP_ADC_IMB_CTRL_RX_ADCIMB_CORR_Q_OFFSET 13
#define RX_DIG_TOP_ADC_IMB_CTRL_RX_ADCIMB_CORR_Q_SIZE 10
#define RX_DIG_TOP_ADC_IMB_CTRL_RX_SPLIT_CH_ADC_EN_OFFSET 23
#define RX_DIG_TOP_ADC_IMB_CTRL_RX_SPLIT_CH_ADC_EN_SIZE 1

/** bit field defines for rx_dig_top_s#wb_iq_corr_ctrl */
#define RX_DIG_TOP_WB_IQ_CORR_CTRL_RX_WB_CORR_EST_EN_OFFSET 0
#define RX_DIG_TOP_WB_IQ_CORR_CTRL_RX_WB_CORR_EST_EN_SIZE 1
#define RX_DIG_TOP_WB_IQ_CORR_CTRL_RX_IQCORR_EN_OFFSET 1
#define RX_DIG_TOP_WB_IQ_CORR_CTRL_RX_IQCORR_EN_SIZE 1
#define RX_DIG_TOP_WB_IQ_CORR_CTRL_RX_IQCORR_1_OFFSET 2
#define RX_DIG_TOP_WB_IQ_CORR_CTRL_RX_IQCORR_1_SIZE 12
#define RX_DIG_TOP_WB_IQ_CORR_CTRL_RX_IQCORR_2_OFFSET 14
#define RX_DIG_TOP_WB_IQ_CORR_CTRL_RX_IQCORR_2_SIZE 12

/** bit field defines for rx_dig_top_s#nb_iq_corr_ctrl */
#define RX_DIG_TOP_NB_IQ_CORR_CTRL_RX_NB_CORR_EST_EN_OFFSET 0
#define RX_DIG_TOP_NB_IQ_CORR_CTRL_RX_NB_CORR_EST_EN_SIZE 1
#define RX_DIG_TOP_NB_IQ_CORR_CTRL_RX_IQCORR_NB_EN_OFFSET 1
#define RX_DIG_TOP_NB_IQ_CORR_CTRL_RX_IQCORR_NB_EN_SIZE 1
#define RX_DIG_TOP_NB_IQ_CORR_CTRL_RX_IQCORR_NB_1_OFFSET 2
#define RX_DIG_TOP_NB_IQ_CORR_CTRL_RX_IQCORR_NB_1_SIZE 12
#define RX_DIG_TOP_NB_IQ_CORR_CTRL_RX_IQCORR_NB_2_OFFSET 14
#define RX_DIG_TOP_NB_IQ_CORR_CTRL_RX_IQCORR_NB_2_SIZE 12

/** bit field defines for rx_dig_top_s#init_sample_ctrl */
#define RX_DIG_TOP_INIT_SAMPLE_CTRL_RX_DATA_W_CHOP_OFFSET 0
#define RX_DIG_TOP_INIT_SAMPLE_CTRL_RX_DATA_W_CHOP_SIZE 3

/** bit field defines for rx_dig_top_s#hist_ctrl */
#define RX_DIG_TOP_HIST_CTRL_HIST_COUNT_EN_OFFSET 0
#define RX_DIG_TOP_HIST_CTRL_HIST_COUNT_EN_SIZE 1
#define RX_DIG_TOP_HIST_CTRL_HIST_ABSBIN_EN_OFFSET 1
#define RX_DIG_TOP_HIST_CTRL_HIST_ABSBIN_EN_SIZE 1
#define RX_DIG_TOP_HIST_CTRL_HIST_COUNT_CLR_OFFSET 2
#define RX_DIG_TOP_HIST_CTRL_HIST_COUNT_CLR_SIZE 1
#define RX_DIG_TOP_HIST_CTRL_HIST_MAX_COUNT_OFFSET 3
#define RX_DIG_TOP_HIST_CTRL_HIST_MAX_COUNT_SIZE 20

/** bit field defines for rx_dig_top_s#filts_ctrl_1 */
#define RX_DIG_TOP_FILTS_CTRL_1_FILTS_EN_OFFSET 0
#define RX_DIG_TOP_FILTS_CTRL_1_FILTS_EN_SIZE 4
/**  */
#define RX_DIG_TOP_FILTS_CTRL_1_FILTS_EN_FARROW_EN_VALUE 4
/**  */
#define RX_DIG_TOP_FILTS_CTRL_1_FILTS_EN_HB_EN_VALUE 1
#define RX_DIG_TOP_FILTS_CTRL_1_INIT_DS_RATE_OFFSET 4
#define RX_DIG_TOP_FILTS_CTRL_1_INIT_DS_RATE_SIZE 3
#define RX_DIG_TOP_FILTS_CTRL_1_RX_SEL_GAIN_SEL_OFFSET 7
#define RX_DIG_TOP_FILTS_CTRL_1_RX_SEL_GAIN_SEL_SIZE 5
#define RX_DIG_TOP_FILTS_CTRL_1_COEFF_SEL_OFFSET 12
#define RX_DIG_TOP_FILTS_CTRL_1_COEFF_SEL_SIZE 3
/** LTE 1.4 downsample-by-2 */
#define RX_DIG_TOP_FILTS_CTRL_1_COEFF_SEL_LTE_1P4_2_VALUE 0
/** LTE 1.4 downsample-by-2 (with programmable coefficients) */
#define RX_DIG_TOP_FILTS_CTRL_1_COEFF_SEL_LTE_1P4_2_PROG_VALUE 2
/** LTE 1.4 downsample-by-3 */
#define RX_DIG_TOP_FILTS_CTRL_1_COEFF_SEL_LTE_1P4_3_VALUE 1
/** LTE 1.4 downsample-by-3  (with programmable coefficients) */
#define RX_DIG_TOP_FILTS_CTRL_1_COEFF_SEL_LTE_1P4_3_PROG_VALUE 3
/** Disable the filter */
#define RX_DIG_TOP_FILTS_CTRL_1_COEFF_SEL_OFF_VALUE 7
/** 12 tap filter with programmable coefficients. */
#define RX_DIG_TOP_FILTS_CTRL_1_COEFF_SEL_PROG_12TAP_VALUE 5
/** 16 tap filter with programmable coefficients. */
#define RX_DIG_TOP_FILTS_CTRL_1_COEFF_SEL_PROG_16TAP_VALUE 6
/** 8 tap filter with programmable coefficients. */
#define RX_DIG_TOP_FILTS_CTRL_1_COEFF_SEL_PROG_8TAP_VALUE 4
#define RX_DIG_TOP_FILTS_CTRL_1_P_SHIFT_DOWN_OFFSET 15
#define RX_DIG_TOP_FILTS_CTRL_1_P_SHIFT_DOWN_SIZE 5

/** bit field defines for rx_dig_top_s#filts_ctrl_2 */
#define RX_DIG_TOP_FILTS_CTRL_2_RX_FARROW_GAIN_SEL_OFFSET 0
#define RX_DIG_TOP_FILTS_CTRL_2_RX_FARROW_GAIN_SEL_SIZE 5

/** bit field defines for rx_dig_top_s#filts_ctrl_3 */
#define RX_DIG_TOP_FILTS_CTRL_3_FR_CTRL_OFFSET 0
#define RX_DIG_TOP_FILTS_CTRL_3_FR_CTRL_SIZE 32

/** bit field defines for rx_dig_top_s#sel_coeffs_1 */
#define RX_DIG_TOP_SEL_COEFFS_1_COEFF_P_M000_OFFSET 0
#define RX_DIG_TOP_SEL_COEFFS_1_COEFF_P_M000_SIZE 11
#define RX_DIG_TOP_SEL_COEFFS_1_COEFF_P_M001_OFFSET 11
#define RX_DIG_TOP_SEL_COEFFS_1_COEFF_P_M001_SIZE 11

/** bit field defines for rx_dig_top_s#sel_coeffs_2 */
#define RX_DIG_TOP_SEL_COEFFS_2_COEFF_P_M002_OFFSET 0
#define RX_DIG_TOP_SEL_COEFFS_2_COEFF_P_M002_SIZE 11
#define RX_DIG_TOP_SEL_COEFFS_2_COEFF_P_M003_OFFSET 11
#define RX_DIG_TOP_SEL_COEFFS_2_COEFF_P_M003_SIZE 11
#define RX_DIG_TOP_SEL_COEFFS_2_COEFF_P_M004_OFFSET 22
#define RX_DIG_TOP_SEL_COEFFS_2_COEFF_P_M004_SIZE 9

/** bit field defines for rx_dig_top_s#sel_coeffs_3 */
#define RX_DIG_TOP_SEL_COEFFS_3_COEFF_P_M005_OFFSET 0
#define RX_DIG_TOP_SEL_COEFFS_3_COEFF_P_M005_SIZE 9
#define RX_DIG_TOP_SEL_COEFFS_3_COEFF_P_M006_OFFSET 9
#define RX_DIG_TOP_SEL_COEFFS_3_COEFF_P_M006_SIZE 9
#define RX_DIG_TOP_SEL_COEFFS_3_COEFF_P_M007_OFFSET 18
#define RX_DIG_TOP_SEL_COEFFS_3_COEFF_P_M007_SIZE 9

/** bit field defines for rx_dig_top_s#sel_coeffs_4 */
#define RX_DIG_TOP_SEL_COEFFS_4_COEFF_P_M008_OFFSET 0
#define RX_DIG_TOP_SEL_COEFFS_4_COEFF_P_M008_SIZE 9
#define RX_DIG_TOP_SEL_COEFFS_4_COEFF_P_M009_OFFSET 9
#define RX_DIG_TOP_SEL_COEFFS_4_COEFF_P_M009_SIZE 9
#define RX_DIG_TOP_SEL_COEFFS_4_COEFF_P_M010_OFFSET 18
#define RX_DIG_TOP_SEL_COEFFS_4_COEFF_P_M010_SIZE 8

/** bit field defines for rx_dig_top_s#sel_coeffs_5 */
#define RX_DIG_TOP_SEL_COEFFS_5_COEFF_P_M011_OFFSET 0
#define RX_DIG_TOP_SEL_COEFFS_5_COEFF_P_M011_SIZE 8
#define RX_DIG_TOP_SEL_COEFFS_5_COEFF_P_M012_OFFSET 8
#define RX_DIG_TOP_SEL_COEFFS_5_COEFF_P_M012_SIZE 8
#define RX_DIG_TOP_SEL_COEFFS_5_COEFF_P_M013_OFFSET 16
#define RX_DIG_TOP_SEL_COEFFS_5_COEFF_P_M013_SIZE 8
#define RX_DIG_TOP_SEL_COEFFS_5_COEFF_P_M014_OFFSET 24
#define RX_DIG_TOP_SEL_COEFFS_5_COEFF_P_M014_SIZE 8

/** bit field defines for rx_dig_top_s#sel_coeffs_6 */
#define RX_DIG_TOP_SEL_COEFFS_6_COEFF_P_M015_OFFSET 0
#define RX_DIG_TOP_SEL_COEFFS_6_COEFF_P_M015_SIZE 8
#define RX_DIG_TOP_SEL_COEFFS_6_COEFF_P_M016_OFFSET 8
#define RX_DIG_TOP_SEL_COEFFS_6_COEFF_P_M016_SIZE 8
#define RX_DIG_TOP_SEL_COEFFS_6_COEFF_P_M017_OFFSET 16
#define RX_DIG_TOP_SEL_COEFFS_6_COEFF_P_M017_SIZE 8
#define RX_DIG_TOP_SEL_COEFFS_6_COEFF_P_M018_OFFSET 24
#define RX_DIG_TOP_SEL_COEFFS_6_COEFF_P_M018_SIZE 8

/** bit field defines for rx_dig_top_s#sel_coeffs_7 */
#define RX_DIG_TOP_SEL_COEFFS_7_COEFF_P_M019_OFFSET 0
#define RX_DIG_TOP_SEL_COEFFS_7_COEFF_P_M019_SIZE 8
#define RX_DIG_TOP_SEL_COEFFS_7_COEFF_P_M020_OFFSET 8
#define RX_DIG_TOP_SEL_COEFFS_7_COEFF_P_M020_SIZE 8

/** bit field defines for rx_dig_top_s#wb_dcoc_est_i */
#define RX_DIG_TOP_WB_DCOC_EST_I_DCOC_ESTIMATE_1_I_OFFSET 0
#define RX_DIG_TOP_WB_DCOC_EST_I_DCOC_ESTIMATE_1_I_SIZE 11
#define RX_DIG_TOP_WB_DCOC_EST_I_DCOC_ESTIMATE_2_I_OFFSET 11
#define RX_DIG_TOP_WB_DCOC_EST_I_DCOC_ESTIMATE_2_I_SIZE 11

/** bit field defines for rx_dig_top_s#wb_dcoc_est_q */
#define RX_DIG_TOP_WB_DCOC_EST_Q_DCOC_ESTIMATE_1_Q_OFFSET 0
#define RX_DIG_TOP_WB_DCOC_EST_Q_DCOC_ESTIMATE_1_Q_SIZE 11
#define RX_DIG_TOP_WB_DCOC_EST_Q_DCOC_ESTIMATE_2_Q_OFFSET 11
#define RX_DIG_TOP_WB_DCOC_EST_Q_DCOC_ESTIMATE_2_Q_SIZE 11

/** bit field defines for rx_dig_top_s#nb_dcoc_ests */
#define RX_DIG_TOP_NB_DCOC_ESTS_DCOC_ESTIMATE_NB_I_OFFSET 0
#define RX_DIG_TOP_NB_DCOC_ESTS_DCOC_ESTIMATE_NB_I_SIZE 13
#define RX_DIG_TOP_NB_DCOC_ESTS_DCOC_ESTIMATE_NB_Q_OFFSET 13
#define RX_DIG_TOP_NB_DCOC_ESTS_DCOC_ESTIMATE_NB_Q_SIZE 13

/** bit field defines for rx_dig_top_s#rx_status */
#define RX_DIG_TOP_RX_STATUS_DCOC_COUNT_DONE_OFFSET 0
#define RX_DIG_TOP_RX_STATUS_DCOC_COUNT_DONE_SIZE 1
#define RX_DIG_TOP_RX_STATUS_CORR_COUNT_DONE_OFFSET 1
#define RX_DIG_TOP_RX_STATUS_CORR_COUNT_DONE_SIZE 1
#define RX_DIG_TOP_RX_STATUS_HIST_OVERFLOW_OFFSET 2
#define RX_DIG_TOP_RX_STATUS_HIST_OVERFLOW_SIZE 1
#define RX_DIG_TOP_RX_STATUS_HIST_COUNT_DONE_OFFSET 3
#define RX_DIG_TOP_RX_STATUS_HIST_COUNT_DONE_SIZE 1

/** bit field defines for rx_dig_top_s#wb_pwr */
#define RX_DIG_TOP_WB_PWR_RX_WB_PWR_OFFSET 0
#define RX_DIG_TOP_WB_PWR_RX_WB_PWR_SIZE 23

/** bit field defines for rx_dig_top_s#nb_pwr */
#define RX_DIG_TOP_NB_PWR_RX_NB_PWR_OFFSET 0
#define RX_DIG_TOP_NB_PWR_RX_NB_PWR_SIZE 27

/** bit field defines for rx_dig_top_s#corr_iwb */
#define RX_DIG_TOP_CORR_IWB_CORR_IWB_OUT_OFFSET 0
#define RX_DIG_TOP_CORR_IWB_CORR_IWB_OUT_SIZE 22

/** bit field defines for rx_dig_top_s#corr_qwb */
#define RX_DIG_TOP_CORR_QWB_CORR_QWB_OUT_OFFSET 0
#define RX_DIG_TOP_CORR_QWB_CORR_QWB_OUT_SIZE 22

/** bit field defines for rx_dig_top_s#corr_iqwb */
#define RX_DIG_TOP_CORR_IQWB_CORR_IQWB_OUT_OFFSET 0
#define RX_DIG_TOP_CORR_IQWB_CORR_IQWB_OUT_SIZE 22

/** bit field defines for rx_dig_top_s#corr_inb */
#define RX_DIG_TOP_CORR_INB_CORR_INB_OUT_OFFSET 0
#define RX_DIG_TOP_CORR_INB_CORR_INB_OUT_SIZE 26

/** bit field defines for rx_dig_top_s#corr_qnb */
#define RX_DIG_TOP_CORR_QNB_CORR_QNB_OUT_OFFSET 0
#define RX_DIG_TOP_CORR_QNB_CORR_QNB_OUT_SIZE 26

/** bit field defines for rx_dig_top_s#corr_iqnb */
#define RX_DIG_TOP_CORR_IQNB_CORR_IQNB_OUT_OFFSET 0
#define RX_DIG_TOP_CORR_IQNB_CORR_IQNB_OUT_SIZE 26

/** bit field defines for rx_dig_top_s#corr_iwb_1 */
#define RX_DIG_TOP_CORR_IWB_1_CORR_IWB_1_OUT_OFFSET 0
#define RX_DIG_TOP_CORR_IWB_1_CORR_IWB_1_OUT_SIZE 22

/** bit field defines for rx_dig_top_s#corr_iwb_2 */
#define RX_DIG_TOP_CORR_IWB_2_CORR_IWB_2_OUT_OFFSET 0
#define RX_DIG_TOP_CORR_IWB_2_CORR_IWB_2_OUT_SIZE 22

/** bit field defines for rx_dig_top_s#corr_qwb_1 */
#define RX_DIG_TOP_CORR_QWB_1_CORR_QWB_1_OUT_OFFSET 0
#define RX_DIG_TOP_CORR_QWB_1_CORR_QWB_1_OUT_SIZE 22

/** bit field defines for rx_dig_top_s#corr_qwb_2 */
#define RX_DIG_TOP_CORR_QWB_2_CORR_QWB_2_OUT_OFFSET 0
#define RX_DIG_TOP_CORR_QWB_2_CORR_QWB_2_OUT_SIZE 22

/** bit field defines for rx_dig_top_s#hist_i_0 */
#define RX_DIG_TOP_HIST_I_0_HIST_OUT_I_0_OFFSET 0
#define RX_DIG_TOP_HIST_I_0_HIST_OUT_I_0_SIZE 16
#define RX_DIG_TOP_HIST_I_0_HIST_OUT_I_1_OFFSET 16
#define RX_DIG_TOP_HIST_I_0_HIST_OUT_I_1_SIZE 16

/** bit field defines for rx_dig_top_s#hist_i_1 */
#define RX_DIG_TOP_HIST_I_1_HIST_OUT_I_2_OFFSET 0
#define RX_DIG_TOP_HIST_I_1_HIST_OUT_I_2_SIZE 16
#define RX_DIG_TOP_HIST_I_1_HIST_OUT_I_3_OFFSET 16
#define RX_DIG_TOP_HIST_I_1_HIST_OUT_I_3_SIZE 16

/** bit field defines for rx_dig_top_s#hist_i_2 */
#define RX_DIG_TOP_HIST_I_2_HIST_OUT_I_4_OFFSET 0
#define RX_DIG_TOP_HIST_I_2_HIST_OUT_I_4_SIZE 16
#define RX_DIG_TOP_HIST_I_2_HIST_OUT_I_5_OFFSET 16
#define RX_DIG_TOP_HIST_I_2_HIST_OUT_I_5_SIZE 16

/** bit field defines for rx_dig_top_s#hist_i_3 */
#define RX_DIG_TOP_HIST_I_3_HIST_OUT_I_6_OFFSET 0
#define RX_DIG_TOP_HIST_I_3_HIST_OUT_I_6_SIZE 16
#define RX_DIG_TOP_HIST_I_3_HIST_OUT_I_7_OFFSET 16
#define RX_DIG_TOP_HIST_I_3_HIST_OUT_I_7_SIZE 16

/** bit field defines for rx_dig_top_s#hist_i_4 */
#define RX_DIG_TOP_HIST_I_4_HIST_OUT_I_8_OFFSET 0
#define RX_DIG_TOP_HIST_I_4_HIST_OUT_I_8_SIZE 16
#define RX_DIG_TOP_HIST_I_4_HIST_OUT_I_9_OFFSET 16
#define RX_DIG_TOP_HIST_I_4_HIST_OUT_I_9_SIZE 16

/** bit field defines for rx_dig_top_s#hist_i_5 */
#define RX_DIG_TOP_HIST_I_5_HIST_OUT_I_10_OFFSET 0
#define RX_DIG_TOP_HIST_I_5_HIST_OUT_I_10_SIZE 16
#define RX_DIG_TOP_HIST_I_5_HIST_OUT_I_11_OFFSET 16
#define RX_DIG_TOP_HIST_I_5_HIST_OUT_I_11_SIZE 16

/** bit field defines for rx_dig_top_s#hist_i_6 */
#define RX_DIG_TOP_HIST_I_6_HIST_OUT_I_12_OFFSET 0
#define RX_DIG_TOP_HIST_I_6_HIST_OUT_I_12_SIZE 16
#define RX_DIG_TOP_HIST_I_6_HIST_OUT_I_13_OFFSET 16
#define RX_DIG_TOP_HIST_I_6_HIST_OUT_I_13_SIZE 16

/** bit field defines for rx_dig_top_s#hist_i_7 */
#define RX_DIG_TOP_HIST_I_7_HIST_OUT_I_14_OFFSET 0
#define RX_DIG_TOP_HIST_I_7_HIST_OUT_I_14_SIZE 16
#define RX_DIG_TOP_HIST_I_7_HIST_OUT_I_15_OFFSET 16
#define RX_DIG_TOP_HIST_I_7_HIST_OUT_I_15_SIZE 16

/** bit field defines for rx_dig_top_s#hist_i_8 */
#define RX_DIG_TOP_HIST_I_8_HIST_OUT_I_16_OFFSET 0
#define RX_DIG_TOP_HIST_I_8_HIST_OUT_I_16_SIZE 16
#define RX_DIG_TOP_HIST_I_8_HIST_OUT_I_17_OFFSET 16
#define RX_DIG_TOP_HIST_I_8_HIST_OUT_I_17_SIZE 16

/** bit field defines for rx_dig_top_s#hist_i_9 */
#define RX_DIG_TOP_HIST_I_9_HIST_OUT_I_18_OFFSET 0
#define RX_DIG_TOP_HIST_I_9_HIST_OUT_I_18_SIZE 16
#define RX_DIG_TOP_HIST_I_9_HIST_OUT_I_19_OFFSET 16
#define RX_DIG_TOP_HIST_I_9_HIST_OUT_I_19_SIZE 16

/** bit field defines for rx_dig_top_s#hist_i_10 */
#define RX_DIG_TOP_HIST_I_10_HIST_OUT_I_20_OFFSET 0
#define RX_DIG_TOP_HIST_I_10_HIST_OUT_I_20_SIZE 16
#define RX_DIG_TOP_HIST_I_10_HIST_OUT_I_21_OFFSET 16
#define RX_DIG_TOP_HIST_I_10_HIST_OUT_I_21_SIZE 16

/** bit field defines for rx_dig_top_s#hist_i_11 */
#define RX_DIG_TOP_HIST_I_11_HIST_OUT_I_22_OFFSET 0
#define RX_DIG_TOP_HIST_I_11_HIST_OUT_I_22_SIZE 16
#define RX_DIG_TOP_HIST_I_11_HIST_OUT_I_23_OFFSET 16
#define RX_DIG_TOP_HIST_I_11_HIST_OUT_I_23_SIZE 16

/** bit field defines for rx_dig_top_s#hist_i_12 */
#define RX_DIG_TOP_HIST_I_12_HIST_OUT_I_24_OFFSET 0
#define RX_DIG_TOP_HIST_I_12_HIST_OUT_I_24_SIZE 16
#define RX_DIG_TOP_HIST_I_12_HIST_OUT_I_25_OFFSET 16
#define RX_DIG_TOP_HIST_I_12_HIST_OUT_I_25_SIZE 16

/** bit field defines for rx_dig_top_s#hist_i_13 */
#define RX_DIG_TOP_HIST_I_13_HIST_OUT_I_26_OFFSET 0
#define RX_DIG_TOP_HIST_I_13_HIST_OUT_I_26_SIZE 16
#define RX_DIG_TOP_HIST_I_13_HIST_OUT_I_27_OFFSET 16
#define RX_DIG_TOP_HIST_I_13_HIST_OUT_I_27_SIZE 16

/** bit field defines for rx_dig_top_s#hist_i_14 */
#define RX_DIG_TOP_HIST_I_14_HIST_OUT_I_28_OFFSET 0
#define RX_DIG_TOP_HIST_I_14_HIST_OUT_I_28_SIZE 16
#define RX_DIG_TOP_HIST_I_14_HIST_OUT_I_29_OFFSET 16
#define RX_DIG_TOP_HIST_I_14_HIST_OUT_I_29_SIZE 16

/** bit field defines for rx_dig_top_s#hist_i_15 */
#define RX_DIG_TOP_HIST_I_15_HIST_OUT_I_30_OFFSET 0
#define RX_DIG_TOP_HIST_I_15_HIST_OUT_I_30_SIZE 16
#define RX_DIG_TOP_HIST_I_15_HIST_OUT_I_31_OFFSET 16
#define RX_DIG_TOP_HIST_I_15_HIST_OUT_I_31_SIZE 16

/** bit field defines for rx_dig_top_s#hist_q_0 */
#define RX_DIG_TOP_HIST_Q_0_HIST_OUT_I_0_OFFSET 0
#define RX_DIG_TOP_HIST_Q_0_HIST_OUT_I_0_SIZE 16
#define RX_DIG_TOP_HIST_Q_0_HIST_OUT_I_1_OFFSET 16
#define RX_DIG_TOP_HIST_Q_0_HIST_OUT_I_1_SIZE 16

/** bit field defines for rx_dig_top_s#hist_q_1 */
#define RX_DIG_TOP_HIST_Q_1_HIST_OUT_I_2_OFFSET 0
#define RX_DIG_TOP_HIST_Q_1_HIST_OUT_I_2_SIZE 16
#define RX_DIG_TOP_HIST_Q_1_HIST_OUT_I_3_OFFSET 16
#define RX_DIG_TOP_HIST_Q_1_HIST_OUT_I_3_SIZE 16

/** bit field defines for rx_dig_top_s#hist_q_2 */
#define RX_DIG_TOP_HIST_Q_2_HIST_OUT_I_4_OFFSET 0
#define RX_DIG_TOP_HIST_Q_2_HIST_OUT_I_4_SIZE 16
#define RX_DIG_TOP_HIST_Q_2_HIST_OUT_I_5_OFFSET 16
#define RX_DIG_TOP_HIST_Q_2_HIST_OUT_I_5_SIZE 16

/** bit field defines for rx_dig_top_s#hist_q_3 */
#define RX_DIG_TOP_HIST_Q_3_HIST_OUT_I_6_OFFSET 0
#define RX_DIG_TOP_HIST_Q_3_HIST_OUT_I_6_SIZE 16
#define RX_DIG_TOP_HIST_Q_3_HIST_OUT_I_7_OFFSET 16
#define RX_DIG_TOP_HIST_Q_3_HIST_OUT_I_7_SIZE 16

/** bit field defines for rx_dig_top_s#hist_q_4 */
#define RX_DIG_TOP_HIST_Q_4_HIST_OUT_I_8_OFFSET 0
#define RX_DIG_TOP_HIST_Q_4_HIST_OUT_I_8_SIZE 16
#define RX_DIG_TOP_HIST_Q_4_HIST_OUT_I_9_OFFSET 16
#define RX_DIG_TOP_HIST_Q_4_HIST_OUT_I_9_SIZE 16

/** bit field defines for rx_dig_top_s#hist_q_5 */
#define RX_DIG_TOP_HIST_Q_5_HIST_OUT_I_10_OFFSET 0
#define RX_DIG_TOP_HIST_Q_5_HIST_OUT_I_10_SIZE 16
#define RX_DIG_TOP_HIST_Q_5_HIST_OUT_I_11_OFFSET 16
#define RX_DIG_TOP_HIST_Q_5_HIST_OUT_I_11_SIZE 16

/** bit field defines for rx_dig_top_s#hist_q_6 */
#define RX_DIG_TOP_HIST_Q_6_HIST_OUT_I_12_OFFSET 0
#define RX_DIG_TOP_HIST_Q_6_HIST_OUT_I_12_SIZE 16
#define RX_DIG_TOP_HIST_Q_6_HIST_OUT_I_13_OFFSET 16
#define RX_DIG_TOP_HIST_Q_6_HIST_OUT_I_13_SIZE 16

/** bit field defines for rx_dig_top_s#hist_q_7 */
#define RX_DIG_TOP_HIST_Q_7_HIST_OUT_I_14_OFFSET 0
#define RX_DIG_TOP_HIST_Q_7_HIST_OUT_I_14_SIZE 16
#define RX_DIG_TOP_HIST_Q_7_HIST_OUT_I_15_OFFSET 16
#define RX_DIG_TOP_HIST_Q_7_HIST_OUT_I_15_SIZE 16

/** bit field defines for rx_dig_top_s#hist_q_8 */
#define RX_DIG_TOP_HIST_Q_8_HIST_OUT_I_16_OFFSET 0
#define RX_DIG_TOP_HIST_Q_8_HIST_OUT_I_16_SIZE 16
#define RX_DIG_TOP_HIST_Q_8_HIST_OUT_I_17_OFFSET 16
#define RX_DIG_TOP_HIST_Q_8_HIST_OUT_I_17_SIZE 16

/** bit field defines for rx_dig_top_s#hist_q_9 */
#define RX_DIG_TOP_HIST_Q_9_HIST_OUT_I_18_OFFSET 0
#define RX_DIG_TOP_HIST_Q_9_HIST_OUT_I_18_SIZE 16
#define RX_DIG_TOP_HIST_Q_9_HIST_OUT_I_19_OFFSET 16
#define RX_DIG_TOP_HIST_Q_9_HIST_OUT_I_19_SIZE 16

/** bit field defines for rx_dig_top_s#hist_q_10 */
#define RX_DIG_TOP_HIST_Q_10_HIST_OUT_I_20_OFFSET 0
#define RX_DIG_TOP_HIST_Q_10_HIST_OUT_I_20_SIZE 16
#define RX_DIG_TOP_HIST_Q_10_HIST_OUT_I_21_OFFSET 16
#define RX_DIG_TOP_HIST_Q_10_HIST_OUT_I_21_SIZE 16

/** bit field defines for rx_dig_top_s#hist_q_11 */
#define RX_DIG_TOP_HIST_Q_11_HIST_OUT_I_22_OFFSET 0
#define RX_DIG_TOP_HIST_Q_11_HIST_OUT_I_22_SIZE 16
#define RX_DIG_TOP_HIST_Q_11_HIST_OUT_I_23_OFFSET 16
#define RX_DIG_TOP_HIST_Q_11_HIST_OUT_I_23_SIZE 16

/** bit field defines for rx_dig_top_s#hist_q_12 */
#define RX_DIG_TOP_HIST_Q_12_HIST_OUT_I_24_OFFSET 0
#define RX_DIG_TOP_HIST_Q_12_HIST_OUT_I_24_SIZE 16
#define RX_DIG_TOP_HIST_Q_12_HIST_OUT_I_25_OFFSET 16
#define RX_DIG_TOP_HIST_Q_12_HIST_OUT_I_25_SIZE 16

/** bit field defines for rx_dig_top_s#hist_q_13 */
#define RX_DIG_TOP_HIST_Q_13_HIST_OUT_I_26_OFFSET 0
#define RX_DIG_TOP_HIST_Q_13_HIST_OUT_I_26_SIZE 16
#define RX_DIG_TOP_HIST_Q_13_HIST_OUT_I_27_OFFSET 16
#define RX_DIG_TOP_HIST_Q_13_HIST_OUT_I_27_SIZE 16

/** bit field defines for rx_dig_top_s#hist_q_14 */
#define RX_DIG_TOP_HIST_Q_14_HIST_OUT_I_28_OFFSET 0
#define RX_DIG_TOP_HIST_Q_14_HIST_OUT_I_28_SIZE 16
#define RX_DIG_TOP_HIST_Q_14_HIST_OUT_I_29_OFFSET 16
#define RX_DIG_TOP_HIST_Q_14_HIST_OUT_I_29_SIZE 16

/** bit field defines for rx_dig_top_s#hist_q_15 */
#define RX_DIG_TOP_HIST_Q_15_HIST_OUT_I_30_OFFSET 0
#define RX_DIG_TOP_HIST_Q_15_HIST_OUT_I_30_SIZE 16
#define RX_DIG_TOP_HIST_Q_15_HIST_OUT_I_31_OFFSET 16
#define RX_DIG_TOP_HIST_Q_15_HIST_OUT_I_31_SIZE 16

/** bit field defines for rx_dig_top_s#adci1 */
#define RX_DIG_TOP_ADCI1_FAULT_COUNT_CH1STG1_I_OFFSET 0
#define RX_DIG_TOP_ADCI1_FAULT_COUNT_CH1STG1_I_SIZE 7
#define RX_DIG_TOP_ADCI1_FAULT_COUNT_CH1STG2_I_OFFSET 7
#define RX_DIG_TOP_ADCI1_FAULT_COUNT_CH1STG2_I_SIZE 7
#define RX_DIG_TOP_ADCI1_FAULT_COUNT_CH1AMP_I_OFFSET 14
#define RX_DIG_TOP_ADCI1_FAULT_COUNT_CH1AMP_I_SIZE 7

/** bit field defines for rx_dig_top_s#adci2 */
#define RX_DIG_TOP_ADCI2_FAULT_COUNT_CH2STG1_I_OFFSET 0
#define RX_DIG_TOP_ADCI2_FAULT_COUNT_CH2STG1_I_SIZE 7
#define RX_DIG_TOP_ADCI2_FAULT_COUNT_CH2STG2_I_OFFSET 7
#define RX_DIG_TOP_ADCI2_FAULT_COUNT_CH2STG2_I_SIZE 7
#define RX_DIG_TOP_ADCI2_FAULT_COUNT_CH2AMP_I_OFFSET 14
#define RX_DIG_TOP_ADCI2_FAULT_COUNT_CH2AMP_I_SIZE 7

/** bit field defines for rx_dig_top_s#adcq1 */
#define RX_DIG_TOP_ADCQ1_FAULT_COUNT_CH1STG1_Q_OFFSET 0
#define RX_DIG_TOP_ADCQ1_FAULT_COUNT_CH1STG1_Q_SIZE 7
#define RX_DIG_TOP_ADCQ1_FAULT_COUNT_CH1STG2_Q_OFFSET 7
#define RX_DIG_TOP_ADCQ1_FAULT_COUNT_CH1STG2_Q_SIZE 7
#define RX_DIG_TOP_ADCQ1_FAULT_COUNT_CH1AMP_Q_OFFSET 14
#define RX_DIG_TOP_ADCQ1_FAULT_COUNT_CH1AMP_Q_SIZE 7

/** bit field defines for rx_dig_top_s#adcq2 */
#define RX_DIG_TOP_ADCQ2_FAULT_COUNT_CH2STG1_Q_OFFSET 0
#define RX_DIG_TOP_ADCQ2_FAULT_COUNT_CH2STG1_Q_SIZE 7
#define RX_DIG_TOP_ADCQ2_FAULT_COUNT_CH2STG2_Q_OFFSET 7
#define RX_DIG_TOP_ADCQ2_FAULT_COUNT_CH2STG2_Q_SIZE 7
#define RX_DIG_TOP_ADCQ2_FAULT_COUNT_CH2AMP_Q_OFFSET 14
#define RX_DIG_TOP_ADCQ2_FAULT_COUNT_CH2AMP_Q_SIZE 7

/** bit field defines for rx_dig_top_s#adc_conv */
#define RX_DIG_TOP_ADC_CONV_DATA_OFFSET_EN_OFFSET 0
#define RX_DIG_TOP_ADC_CONV_DATA_OFFSET_EN_SIZE 1
#define RX_DIG_TOP_ADC_CONV_FAULT_COUNT_EN_OFFSET 1
#define RX_DIG_TOP_ADC_CONV_FAULT_COUNT_EN_SIZE 1

/** bit field defines for rx_dig_top_s#adc_cal_flag_en */
#define RX_DIG_TOP_ADC_CAL_FLAG_EN_ADCI1EN_OFFSET 0
#define RX_DIG_TOP_ADC_CAL_FLAG_EN_ADCI1EN_SIZE 1
#define RX_DIG_TOP_ADC_CAL_FLAG_EN_ADCI2EN_OFFSET 1
#define RX_DIG_TOP_ADC_CAL_FLAG_EN_ADCI2EN_SIZE 1
#define RX_DIG_TOP_ADC_CAL_FLAG_EN_ADCQ1EN_OFFSET 2
#define RX_DIG_TOP_ADC_CAL_FLAG_EN_ADCQ1EN_SIZE 1
#define RX_DIG_TOP_ADC_CAL_FLAG_EN_ADCQ2EN_OFFSET 3
#define RX_DIG_TOP_ADC_CAL_FLAG_EN_ADCQ2EN_SIZE 1

/** bit field defines for rx_dig_top_s#adc_tmode */
#define RX_DIG_TOP_ADC_TMODE_TMODE_OFFSET 0
#define RX_DIG_TOP_ADC_TMODE_TMODE_SIZE 4

/** bit field defines for rx_dig_top_s#rx_loop_ctl */
#define RX_DIG_TOP_RX_LOOP_CTL_EN_TXDFE_DATA_IN_OFFSET 0
#define RX_DIG_TOP_RX_LOOP_CTL_EN_TXDFE_DATA_IN_SIZE 1

/** bit field defines for rx_dig_top_s#filts_ctrl_4 */
#define RX_DIG_TOP_FILTS_CTRL_4_FR_OFFSET_I_OFFSET 0
#define RX_DIG_TOP_FILTS_CTRL_4_FR_OFFSET_I_SIZE 32

/** bit field defines for rx_dig_top_s#filts_ctrl_5 */
#define RX_DIG_TOP_FILTS_CTRL_5_FR_OFFSET_Q_OFFSET 0
#define RX_DIG_TOP_FILTS_CTRL_5_FR_OFFSET_Q_SIZE 32

/** bit field defines for rx_dig_top_s#rx_dfe_reset */
#define RX_DIG_TOP_RX_DFE_RESET_RX_RSTN_OFFSET 0
#define RX_DIG_TOP_RX_DFE_RESET_RX_RSTN_SIZE 1

/** bit field defines for rx_dig_top_s#rx_dfe_strt_ctrl */
#define RX_DIG_TOP_RX_DFE_STRT_CTRL_RX_STRT_DEL_MODE_OFFSET 0
#define RX_DIG_TOP_RX_DFE_STRT_CTRL_RX_STRT_DEL_MODE_SIZE 2
#define RX_DIG_TOP_RX_DFE_STRT_CTRL_RX_DEL_VAL_OFFSET 2
#define RX_DIG_TOP_RX_DFE_STRT_CTRL_RX_DEL_VAL_SIZE 12
#define RX_DIG_TOP_RX_DFE_STRT_CTRL_FARROW_GD_ADJUST_OFFSET 14
#define RX_DIG_TOP_RX_DFE_STRT_CTRL_FARROW_GD_ADJUST_SIZE 2
#define RX_DIG_TOP_RX_DFE_STRT_CTRL_RX_START_SCH_EN_OFFSET 16
#define RX_DIG_TOP_RX_DFE_STRT_CTRL_RX_START_SCH_EN_SIZE 1

/** bit field defines for rx_dig_top_s#rx_dfe_end_ctrl */
#define RX_DIG_TOP_RX_DFE_END_CTRL_BURST_LEN_OFFSET 0
#define RX_DIG_TOP_RX_DFE_END_CTRL_BURST_LEN_SIZE 15
#define RX_DIG_TOP_RX_DFE_END_CTRL_BURST_END_OFFSET 15
#define RX_DIG_TOP_RX_DFE_END_CTRL_BURST_END_SIZE 1

/** bit field defines for rx_dig_top_s#rx_irq */
#define RX_DIG_TOP_RX_IRQ_DATA_FAULT_FLAG_I_OFFSET 0
#define RX_DIG_TOP_RX_IRQ_DATA_FAULT_FLAG_I_SIZE 1
#define RX_DIG_TOP_RX_IRQ_CONV_FAULT_FLAG_I_OFFSET 1
#define RX_DIG_TOP_RX_IRQ_CONV_FAULT_FLAG_I_SIZE 1
#define RX_DIG_TOP_RX_IRQ_DATA_FAULT_FLAG_Q_OFFSET 2
#define RX_DIG_TOP_RX_IRQ_DATA_FAULT_FLAG_Q_SIZE 1
#define RX_DIG_TOP_RX_IRQ_CONV_FAULT_FLAG_Q_OFFSET 3
#define RX_DIG_TOP_RX_IRQ_CONV_FAULT_FLAG_Q_SIZE 1
#define RX_DIG_TOP_RX_IRQ_ADC_CAL_DONE_FLAG_OFFSET 4
#define RX_DIG_TOP_RX_IRQ_ADC_CAL_DONE_FLAG_SIZE 1
#define RX_DIG_TOP_RX_IRQ_DCOC_COUNT_DONE_OFFSET 5
#define RX_DIG_TOP_RX_IRQ_DCOC_COUNT_DONE_SIZE 1
#define RX_DIG_TOP_RX_IRQ_CORR_COUNT_DONE_OFFSET 6
#define RX_DIG_TOP_RX_IRQ_CORR_COUNT_DONE_SIZE 1
#define RX_DIG_TOP_RX_IRQ_DCOC_COUNT_NB_DONE_OFFSET 7
#define RX_DIG_TOP_RX_IRQ_DCOC_COUNT_NB_DONE_SIZE 1
#define RX_DIG_TOP_RX_IRQ_CORR_COUNT_NB_DONE_OFFSET 8
#define RX_DIG_TOP_RX_IRQ_CORR_COUNT_NB_DONE_SIZE 1
#define RX_DIG_TOP_RX_IRQ_HIST_OVERFLOW_OFFSET 9
#define RX_DIG_TOP_RX_IRQ_HIST_OVERFLOW_SIZE 1
#define RX_DIG_TOP_RX_IRQ_HIST_COUNT_DONE_OFFSET 10
#define RX_DIG_TOP_RX_IRQ_HIST_COUNT_DONE_SIZE 1

/* EOF rx_dig_top.h */
#endif
