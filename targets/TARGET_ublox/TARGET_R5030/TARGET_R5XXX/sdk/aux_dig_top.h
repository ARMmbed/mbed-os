#ifndef INCLUDED_AUX_DIG_TOP
#define INCLUDED_AUX_DIG_TOP
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** aux_dig_top HAL Spreadsheet version number */
#define AUX_DIG_TOP_HAL_VERSION 13

/** This is the measurement receiver path control register description
*/
struct aux_dig_top_s {
   /** aux post adc control at address offset 0x000, read-write */
   uint32_t padc_gain_ctl;
   /** aux wb dcoc control at address offset 0x004, read-write */
   uint32_t dcoc_ctl;
   /**  dcoc correction at address offset 0x008, read-write */
   uint32_t dcoc_corr;
   /** wideband dcoc estimates at address offset 0x00C, read-only */
   uint32_t dcoc_est;
   /** adc imbalance control at address offset 0x010, read-write */
   uint32_t adc_imb_ctrl;
   /** filter control at address offset 0x014, read-write */
   uint32_t filts_ctrl_1;
   /** aux status flags at address offset 0x018, read-write */
   uint32_t aux_status;
   /** aux wb dcoc control at address offset 0x01C, read-write */
   uint32_t dcoc_ctl_nb;
   /** dcoc correction at address offset 0x020, read-write */
   uint32_t dcoc_corr_nb;
   /** Wideband dcoc estimates at address offset 0x024, read-only */
   uint32_t dcoc_est_nb;
   /** Control for aux power measurement at address offset 0x028, read-write */
   uint32_t aux_pwr_ctrl;
   /** power at address offset 0x02C, read-only */
   uint32_t aux_pwr;
   /** DFE Output sample at address offset 0x030, read-only */
   uint32_t aux_adc_sample;
   /** Control bits for the aux_dfe at address offset 0x034, read-write */
   uint32_t aux_dfe_ctrl;
   /** selects the data source fed to the dfe output  at address offset 0x038, read-write */
   uint32_t adc_tmode;
   /** aux reset control at address offset 0x03C, read-write */
   uint32_t aux_dfe_reset;
   /** adc fault counts at address offset 0x040, read-only */
   uint32_t adci1;
   /** adc fault counts at address offset 0x044, read-only */
   uint32_t adci2;
   /** adc conversion control at address offset 0x048, read-write */
   uint32_t adc_conv;
   /** selects the signals that are ANDed to generate the offline cal done interrupt at address offset 0x04C, read-write */
   uint32_t adc_cal_flag_en;
};

/** bit field defines for aux_dig_top_s#padc_gain_ctl */
#define AUX_DIG_TOP_PADC_GAIN_CTL_PADC_SHIFT_OFFSET 0
#define AUX_DIG_TOP_PADC_GAIN_CTL_PADC_SHIFT_SIZE 4
#define AUX_DIG_TOP_PADC_GAIN_CTL_PADC_FGAIN_SEL_OFFSET 4
#define AUX_DIG_TOP_PADC_GAIN_CTL_PADC_FGAIN_SEL_SIZE 4
#define AUX_DIG_TOP_PADC_GAIN_CTL_PADC_FGAIN_EN_OFFSET 8
#define AUX_DIG_TOP_PADC_GAIN_CTL_PADC_FGAIN_EN_SIZE 1

/** bit field defines for aux_dig_top_s#dcoc_ctl */
#define AUX_DIG_TOP_DCOC_CTL_AUX_DCOC_EST_EN_OFFSET 0
#define AUX_DIG_TOP_DCOC_CTL_AUX_DCOC_EST_EN_SIZE 1
#define AUX_DIG_TOP_DCOC_CTL_AUX_DCOC_CORR_EN_OFFSET 1
#define AUX_DIG_TOP_DCOC_CTL_AUX_DCOC_CORR_EN_SIZE 1
#define AUX_DIG_TOP_DCOC_CTL_AUX_CORR_WIN_OFFSET 2
#define AUX_DIG_TOP_DCOC_CTL_AUX_CORR_WIN_SIZE 20
#define AUX_DIG_TOP_DCOC_CTL_AUX_AUTO_DCOC_EN_OFFSET 22
#define AUX_DIG_TOP_DCOC_CTL_AUX_AUTO_DCOC_EN_SIZE 1

/** bit field defines for aux_dig_top_s#dcoc_corr */
#define AUX_DIG_TOP_DCOC_CORR_AUX_DCOC_CORR_1_OFFSET 0
#define AUX_DIG_TOP_DCOC_CORR_AUX_DCOC_CORR_1_SIZE 11
#define AUX_DIG_TOP_DCOC_CORR_AUX_DCOC_CORR_2_OFFSET 11
#define AUX_DIG_TOP_DCOC_CORR_AUX_DCOC_CORR_2_SIZE 11

/** bit field defines for aux_dig_top_s#dcoc_est */
#define AUX_DIG_TOP_DCOC_EST_DCOC_ESTIMATE_1_OFFSET 0
#define AUX_DIG_TOP_DCOC_EST_DCOC_ESTIMATE_1_SIZE 11
#define AUX_DIG_TOP_DCOC_EST_DCOC_ESTIMATE_2_OFFSET 11
#define AUX_DIG_TOP_DCOC_EST_DCOC_ESTIMATE_2_SIZE 11

/** bit field defines for aux_dig_top_s#adc_imb_ctrl */
#define AUX_DIG_TOP_ADC_IMB_CTRL_AUX_ADCIMB_CORR_EN_OFFSET 0
#define AUX_DIG_TOP_ADC_IMB_CTRL_AUX_ADCIMB_CORR_EN_SIZE 1
#define AUX_DIG_TOP_ADC_IMB_CTRL_AUX_ADCIMB_ODDEN_OFFSET 1
#define AUX_DIG_TOP_ADC_IMB_CTRL_AUX_ADCIMB_ODDEN_SIZE 1
#define AUX_DIG_TOP_ADC_IMB_CTRL_AUX_ADCIMB_CORR_OFFSET 2
#define AUX_DIG_TOP_ADC_IMB_CTRL_AUX_ADCIMB_CORR_SIZE 10
#define AUX_DIG_TOP_ADC_IMB_CTRL_AUX_SPLIT_CH_ADC_EN_OFFSET 12
#define AUX_DIG_TOP_ADC_IMB_CTRL_AUX_SPLIT_CH_ADC_EN_SIZE 1

/** bit field defines for aux_dig_top_s#filts_ctrl_1 */
#define AUX_DIG_TOP_FILTS_CTRL_1_FILTS_EN_OFFSET 0
#define AUX_DIG_TOP_FILTS_CTRL_1_FILTS_EN_SIZE 1
#define AUX_DIG_TOP_FILTS_CTRL_1_INIT_DS_RATE_OFFSET 1
#define AUX_DIG_TOP_FILTS_CTRL_1_INIT_DS_RATE_SIZE 3

/** bit field defines for aux_dig_top_s#aux_status */
#define AUX_DIG_TOP_AUX_STATUS_DCOC_COUNT_DONE_OFFSET 0
#define AUX_DIG_TOP_AUX_STATUS_DCOC_COUNT_DONE_SIZE 1
#define AUX_DIG_TOP_AUX_STATUS_DCOC_COUNT_DONE_NB_OFFSET 1
#define AUX_DIG_TOP_AUX_STATUS_DCOC_COUNT_DONE_NB_SIZE 1
#define AUX_DIG_TOP_AUX_STATUS_AUX_PWR_DONE_OFFSET 2
#define AUX_DIG_TOP_AUX_STATUS_AUX_PWR_DONE_SIZE 1
#define AUX_DIG_TOP_AUX_STATUS_DATA_FAULT_FLAG_OFFSET 3
#define AUX_DIG_TOP_AUX_STATUS_DATA_FAULT_FLAG_SIZE 1
#define AUX_DIG_TOP_AUX_STATUS_CONV_FAULT_FLAG_OFFSET 4
#define AUX_DIG_TOP_AUX_STATUS_CONV_FAULT_FLAG_SIZE 1
#define AUX_DIG_TOP_AUX_STATUS_ADC_CAL_DONE_FLAG_OFFSET 5
#define AUX_DIG_TOP_AUX_STATUS_ADC_CAL_DONE_FLAG_SIZE 1

/** bit field defines for aux_dig_top_s#dcoc_ctl_nb */
#define AUX_DIG_TOP_DCOC_CTL_NB_AUX_DCOC_EST_EN_OFFSET 0
#define AUX_DIG_TOP_DCOC_CTL_NB_AUX_DCOC_EST_EN_SIZE 1
#define AUX_DIG_TOP_DCOC_CTL_NB_AUX_DCOC_CORR_EN_OFFSET 1
#define AUX_DIG_TOP_DCOC_CTL_NB_AUX_DCOC_CORR_EN_SIZE 1
#define AUX_DIG_TOP_DCOC_CTL_NB_AUX_AUTO_DCOC_EN_OFFSET 2
#define AUX_DIG_TOP_DCOC_CTL_NB_AUX_AUTO_DCOC_EN_SIZE 1

/** bit field defines for aux_dig_top_s#dcoc_corr_nb */
#define AUX_DIG_TOP_DCOC_CORR_NB_AUX_DCOC_CORR_OFFSET 0
#define AUX_DIG_TOP_DCOC_CORR_NB_AUX_DCOC_CORR_SIZE 13

/** bit field defines for aux_dig_top_s#dcoc_est_nb */
#define AUX_DIG_TOP_DCOC_EST_NB_DCOC_ESTIMATE_OFFSET 0
#define AUX_DIG_TOP_DCOC_EST_NB_DCOC_ESTIMATE_SIZE 13

/** bit field defines for aux_dig_top_s#aux_pwr_ctrl */
#define AUX_DIG_TOP_AUX_PWR_CTRL_AUX_PWR_WIN_OFFSET 0
#define AUX_DIG_TOP_AUX_PWR_CTRL_AUX_PWR_WIN_SIZE 20
#define AUX_DIG_TOP_AUX_PWR_CTRL_AUX_PWR_EN_OFFSET 20
#define AUX_DIG_TOP_AUX_PWR_CTRL_AUX_PWR_EN_SIZE 1
#define AUX_DIG_TOP_AUX_PWR_CTRL_AUX_ADC_SAMPLE_EN_OFFSET 21
#define AUX_DIG_TOP_AUX_PWR_CTRL_AUX_ADC_SAMPLE_EN_SIZE 1

/** bit field defines for aux_dig_top_s#aux_pwr */
#define AUX_DIG_TOP_AUX_PWR_AUX_PWR_OFFSET 0
#define AUX_DIG_TOP_AUX_PWR_AUX_PWR_SIZE 26

/** bit field defines for aux_dig_top_s#aux_adc_sample */
#define AUX_DIG_TOP_AUX_ADC_SAMPLE_AUX_ADC_SAMPLE_OFFSET 0
#define AUX_DIG_TOP_AUX_ADC_SAMPLE_AUX_ADC_SAMPLE_SIZE 13

/** bit field defines for aux_dig_top_s#aux_dfe_ctrl */
#define AUX_DIG_TOP_AUX_DFE_CTRL_AUX_DFE_SINGLE_CH_EN_OFFSET 0
#define AUX_DIG_TOP_AUX_DFE_CTRL_AUX_DFE_SINGLE_CH_EN_SIZE 1

/** bit field defines for aux_dig_top_s#adc_tmode */
#define AUX_DIG_TOP_ADC_TMODE_TMODE_OFFSET 0
#define AUX_DIG_TOP_ADC_TMODE_TMODE_SIZE 4

/** bit field defines for aux_dig_top_s#aux_dfe_reset */
#define AUX_DIG_TOP_AUX_DFE_RESET_AUX_RSTN_OFFSET 0
#define AUX_DIG_TOP_AUX_DFE_RESET_AUX_RSTN_SIZE 1

/** bit field defines for aux_dig_top_s#adci1 */
#define AUX_DIG_TOP_ADCI1_FAULT_COUNT_CH1STG1_OFFSET 0
#define AUX_DIG_TOP_ADCI1_FAULT_COUNT_CH1STG1_SIZE 7
#define AUX_DIG_TOP_ADCI1_FAULT_COUNT_CH1STG2_OFFSET 7
#define AUX_DIG_TOP_ADCI1_FAULT_COUNT_CH1STG2_SIZE 7
#define AUX_DIG_TOP_ADCI1_FAULT_COUNT_CH1AMP_OFFSET 14
#define AUX_DIG_TOP_ADCI1_FAULT_COUNT_CH1AMP_SIZE 7

/** bit field defines for aux_dig_top_s#adci2 */
#define AUX_DIG_TOP_ADCI2_FAULT_COUNT_CH2STG1_OFFSET 0
#define AUX_DIG_TOP_ADCI2_FAULT_COUNT_CH2STG1_SIZE 7
#define AUX_DIG_TOP_ADCI2_FAULT_COUNT_CH2STG2_OFFSET 7
#define AUX_DIG_TOP_ADCI2_FAULT_COUNT_CH2STG2_SIZE 7
#define AUX_DIG_TOP_ADCI2_FAULT_COUNT_CH2AMP_OFFSET 14
#define AUX_DIG_TOP_ADCI2_FAULT_COUNT_CH2AMP_SIZE 7

/** bit field defines for aux_dig_top_s#adc_conv */
#define AUX_DIG_TOP_ADC_CONV_DATA_OFFSET_EN_OFFSET 0
#define AUX_DIG_TOP_ADC_CONV_DATA_OFFSET_EN_SIZE 1
#define AUX_DIG_TOP_ADC_CONV_FAULT_COUNT_EN_OFFSET 1
#define AUX_DIG_TOP_ADC_CONV_FAULT_COUNT_EN_SIZE 1

/** bit field defines for aux_dig_top_s#adc_cal_flag_en */
#define AUX_DIG_TOP_ADC_CAL_FLAG_EN_ADCI1EN_OFFSET 0
#define AUX_DIG_TOP_ADC_CAL_FLAG_EN_ADCI1EN_SIZE 1
#define AUX_DIG_TOP_ADC_CAL_FLAG_EN_ADCI2EN_OFFSET 1
#define AUX_DIG_TOP_ADC_CAL_FLAG_EN_ADCI2EN_SIZE 1

/* EOF aux_dig_top.h */
#endif
