#ifndef INCLUDED_TX_DIG_TOP_TXDFE1
#define INCLUDED_TX_DIG_TOP_TXDFE1
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** tx_dig_top_txdfe1 HAL Spreadsheet version number */
#define TX_DIG_TOP_TXDFE1_HAL_VERSION 6

/** This is the tx dfe part 1 line-up register description
*/
struct tx_dig_top_txdfe1_s {
   /** tx dac control at address offset 0x000, read-write */
   uint32_t tx_dac_ctl;
   /** tx post farrow gain control at address offset 0x004, read-write */
   uint32_t tx_gain_ctl;
   /** tx mixer control at address offset 0x008, read-write */
   uint32_t mix_ctl;
   /** tx  dcoc correction at address offset 0x00C, read-write */
   uint32_t dcoc_ctl;
   /** iq correction control at address offset 0x010, read-write */
   uint32_t iq_corr_ctrl;
   /** filter control 1 at address offset 0x014, read-write */
   uint32_t filts_ctrl_1;
   /** filter control 2 at address offset 0x018, read-write */
   uint32_t filts_ctrl_2;
   /** filter control 3 at address offset 0x01C, read-write */
   uint32_t filts_ctrl_3;
   /** lte specific control1 at address offset 0x020, read-write */
   uint32_t lte_ctrl1;
   /** test tone control at address offset 0x024, read-write */
   uint32_t test_ctrl;
   /** complex multiply control profile 1 at address offset 0x028, read-write */
   uint32_t cmplx_mult_p1_ctrl1;
   /** complex multiply control profile 1 at address offset 0x02C, read-write */
   uint32_t cmplx_mult_p1_ctrl2;
   /** complex multiply control profile 1 at address offset 0x030, read-write */
   uint32_t cmplx_mult_p1_ctrl3;
   /** complex multiply control profile 1 at address offset 0x034, read-write */
   uint32_t cmplx_mult_p1_ctrl4;
   /** complex multiply control profile 1 at address offset 0x038, read-write */
   uint32_t cmplx_mult_p1_ctrl5;
   /** complex multiply control profile 1 at address offset 0x03C, read-write */
   uint32_t cmplx_mult_p1_ctrl6;
   /** complex multiply control profile 1 at address offset 0x040, read-write */
   uint32_t cmplx_mult_p1_ctrl7;
   /** complex multiply control profile 1 at address offset 0x044, read-write */
   uint32_t cmplx_mult_p1_ctrl8;
   /** complex multiply control profile 1 at address offset 0x048, read-write */
   uint32_t cmplx_mult_p1_ctrl9;
   /** complex multiply control profile 1 at address offset 0x04C, read-write */
   uint32_t cmplx_mult_p1_ctrl10;
   /** complex multiply control profile 1 at address offset 0x050, read-write */
   uint32_t cmplx_mult_p1_ctrl11;
   /** complex multiply control profile 1 at address offset 0x054, read-write */
   uint32_t cmplx_mult_p1_ctrl12;
   /** complex multiply control profile 1 at address offset 0x058, read-write */
   uint32_t cmplx_mult_p1_ctrl13;
   /** complex multiply control profile 1 at address offset 0x05C, read-write */
   uint32_t cmplx_mult_p1_ctrl14;
   /** complex multiply control profile 1 at address offset 0x060, read-write */
   uint32_t cmplx_mult_p1_ctrl15;
   /** complex multiply control profile 1 at address offset 0x064, read-write */
   uint32_t cmplx_mult_p1_ctrl16;
   /** complex multiply control profile 1 at address offset 0x068, read-write */
   uint32_t cmplx_mult_p1_ctrl17;
   /** complex multiply control profile 1 at address offset 0x06C, read-write */
   uint32_t cmplx_mult_p1_ctrl18;
   /** complex multiply control profile 1 at address offset 0x070, read-write */
   uint32_t cmplx_mult_p1_ctrl19;
   /** complex multiply control profile 1 at address offset 0x074, read-write */
   uint32_t cmplx_mult_p1_ctrl20;
   /** complex multiply control profile 1 at address offset 0x078, read-write */
   uint32_t cmplx_mult_p1_ctrl21;
   /** complex multiply control profile 1 at address offset 0x07C, read-write */
   uint32_t cmplx_mult_p1_ctrl22;
   /** complex multiply control profile 1 at address offset 0x080, read-write */
   uint32_t cmplx_mult_p1_ctrl23;
   /** complex multiply control profile 1 at address offset 0x084, read-write */
   uint32_t cmplx_mult_p1_ctrl24;
   /** complex multiply control profile 1 at address offset 0x088, read-write */
   uint32_t cmplx_mult_p1_ctrl25;
   /** complex multiply control profile 1 at address offset 0x08C, read-write */
   uint32_t cmplx_mult_p1_ctrl26;
   /** complex multiply control profile 1 at address offset 0x090, read-write */
   uint32_t cmplx_mult_p1_ctrl27;
   /** complex multiply control profile 1 at address offset 0x094, read-write */
   uint32_t cmplx_mult_p1_ctrl28;
   /** complex multiply control profile 1 at address offset 0x098, read-write */
   uint32_t cmplx_mult_p1_ctrl29;
   /** complex multiply control profile 1 at address offset 0x09C, read-write */
   uint32_t cmplx_mult_p1_ctrl30;
   /** complex multiply control profile 1 at address offset 0x0A0, read-write */
   uint32_t cmplx_mult_p1_ctrl31;
   /** complex multiply control profile 1 at address offset 0x0A4, read-write */
   uint32_t cmplx_mult_p1_ctrl32;
   /** complex multiply control profile 2 at address offset 0x0A8, read-write */
   uint32_t cmplx_mult_p2_ctrl1;
   /** complex multiply control profile 2 at address offset 0x0AC, read-write */
   uint32_t cmplx_mult_p2_ctrl2;
   /** complex multiply control profile 2 at address offset 0x0B0, read-write */
   uint32_t cmplx_mult_p2_ctrl3;
   /** complex multiply control profile 2 at address offset 0x0B4, read-write */
   uint32_t cmplx_mult_p2_ctrl4;
   /** complex multiply control profile 2 at address offset 0x0B8, read-write */
   uint32_t cmplx_mult_p2_ctrl5;
   /** complex multiply control profile 2 at address offset 0x0BC, read-write */
   uint32_t cmplx_mult_p2_ctrl6;
   /** complex multiply control profile 2 at address offset 0x0C0, read-write */
   uint32_t cmplx_mult_p2_ctrl7;
   /** complex multiply control profile 2 at address offset 0x0C4, read-write */
   uint32_t cmplx_mult_p2_ctrl8;
   /** complex multiply control profile 2 at address offset 0x0C8, read-write */
   uint32_t cmplx_mult_p2_ctrl9;
   /** complex multiply control profile 2 at address offset 0x0CC, read-write */
   uint32_t cmplx_mult_p2_ctrl10;
   /** complex multiply control profile 2 at address offset 0x0D0, read-write */
   uint32_t cmplx_mult_p2_ctrl11;
   /** complex multiply control profile 2 at address offset 0x0D4, read-write */
   uint32_t cmplx_mult_p2_ctrl12;
   /** complex multiply control profile 2 at address offset 0x0D8, read-write */
   uint32_t cmplx_mult_p2_ctrl13;
   /** complex multiply control profile 2 at address offset 0x0DC, read-write */
   uint32_t cmplx_mult_p2_ctrl14;
   /** complex multiply control profile 2 at address offset 0x0E0, read-write */
   uint32_t cmplx_mult_p2_ctrl15;
   /** complex multiply control profile 2 at address offset 0x0E4, read-write */
   uint32_t cmplx_mult_p2_ctrl16;
   /** complex multiply control profile 2 at address offset 0x0E8, read-write */
   uint32_t cmplx_mult_p2_ctrl17;
   /** complex multiply control profile 2 at address offset 0x0EC, read-write */
   uint32_t cmplx_mult_p2_ctrl18;
   /** complex multiply control profile 2 at address offset 0x0F0, read-write */
   uint32_t cmplx_mult_p2_ctrl19;
   /** complex multiply control profile 2 at address offset 0x0F4, read-write */
   uint32_t cmplx_mult_p2_ctrl20;
   /** complex multiply control profile 2 at address offset 0x0F8, read-write */
   uint32_t cmplx_mult_p2_ctrl21;
   /** complex multiply control profile 2 at address offset 0x0FC, read-write */
   uint32_t cmplx_mult_p2_ctrl22;
   /** complex multiply control profile 2 at address offset 0x100, read-write */
   uint32_t cmplx_mult_p2_ctrl23;
   /** complex multiply control profile 2 at address offset 0x104, read-write */
   uint32_t cmplx_mult_p2_ctrl24;
   /** complex multiply control profile 2 at address offset 0x108, read-write */
   uint32_t cmplx_mult_p2_ctrl25;
   /** complex multiply control profile 2 at address offset 0x10C, read-write */
   uint32_t cmplx_mult_p2_ctrl26;
   /** complex multiply control profile 2 at address offset 0x110, read-write */
   uint32_t cmplx_mult_p2_ctrl27;
   /** complex multiply control profile 2 at address offset 0x114, read-write */
   uint32_t cmplx_mult_p2_ctrl28;
   /** complex multiply control profile 2 at address offset 0x118, read-write */
   uint32_t cmplx_mult_p2_ctrl29;
   /** complex multiply control profile 2 at address offset 0x11C, read-write */
   uint32_t cmplx_mult_p2_ctrl30;
   /** complex multiply control profile 2 at address offset 0x120, read-write */
   uint32_t cmplx_mult_p2_ctrl31;
   /** complex multiply control profile 2 at address offset 0x124, read-write */
   uint32_t cmplx_mult_p2_ctrl32;
   /** tx loop control at address offset 0x128, read-write */
   uint32_t tx_loop_ctl;
   /** filter control 4 at address offset 0x12C, read-write */
   uint32_t filts_ctrl_4;
   /** filter control 5 at address offset 0x130, read-write */
   uint32_t filts_ctrl_5;
   /** tx dfe delay control  at address offset 0x134, read-write */
   uint32_t tx_dfe_del_ctrl;
   /** tx reset control at address offset 0x138, read-write */
   uint32_t tx_dfe_reset;
   /** Control muxing of the input from the scratchpad at address offset 0x13C, read-write */
   uint32_t tx_dfe_scratch_ctrl;
};

/** bit field defines for tx_dig_top_txdfe1_s#tx_dac_ctl */
#define TX_DIG_TOP_TXDFE1_TX_DAC_CTL_DAC_EN_I_OFFSET 0
#define TX_DIG_TOP_TXDFE1_TX_DAC_CTL_DAC_EN_I_SIZE 1
#define TX_DIG_TOP_TXDFE1_TX_DAC_CTL_DAC_DEM3_EN_I_OFFSET 1
#define TX_DIG_TOP_TXDFE1_TX_DAC_CTL_DAC_DEM3_EN_I_SIZE 1
#define TX_DIG_TOP_TXDFE1_TX_DAC_CTL_DAC_DEM5_EN_I_OFFSET 2
#define TX_DIG_TOP_TXDFE1_TX_DAC_CTL_DAC_DEM5_EN_I_SIZE 1
#define TX_DIG_TOP_TXDFE1_TX_DAC_CTL_DAC_EN_Q_OFFSET 3
#define TX_DIG_TOP_TXDFE1_TX_DAC_CTL_DAC_EN_Q_SIZE 1
#define TX_DIG_TOP_TXDFE1_TX_DAC_CTL_DAC_DEM3_EN_Q_OFFSET 4
#define TX_DIG_TOP_TXDFE1_TX_DAC_CTL_DAC_DEM3_EN_Q_SIZE 1
#define TX_DIG_TOP_TXDFE1_TX_DAC_CTL_DAC_DEM5_EN_Q_OFFSET 5
#define TX_DIG_TOP_TXDFE1_TX_DAC_CTL_DAC_DEM5_EN_Q_SIZE 1
#define TX_DIG_TOP_TXDFE1_TX_DAC_CTL_DAC_DEM_LEN_OFFSET 9
#define TX_DIG_TOP_TXDFE1_TX_DAC_CTL_DAC_DEM_LEN_SIZE 3
#define TX_DIG_TOP_TXDFE1_TX_DAC_CTL_DAC_DEM_LFSR_RATE_OFFSET 12
#define TX_DIG_TOP_TXDFE1_TX_DAC_CTL_DAC_DEM_LFSR_RATE_SIZE 4

/** bit field defines for tx_dig_top_txdfe1_s#tx_gain_ctl */
#define TX_DIG_TOP_TXDFE1_TX_GAIN_CTL_SHIFT_OFFSET 0
#define TX_DIG_TOP_TXDFE1_TX_GAIN_CTL_SHIFT_SIZE 4
#define TX_DIG_TOP_TXDFE1_TX_GAIN_CTL_FGAIN_SEL_OFFSET 4
#define TX_DIG_TOP_TXDFE1_TX_GAIN_CTL_FGAIN_SEL_SIZE 4
#define TX_DIG_TOP_TXDFE1_TX_GAIN_CTL_FGAIN_EN_OFFSET 8
#define TX_DIG_TOP_TXDFE1_TX_GAIN_CTL_FGAIN_EN_SIZE 1
#define TX_DIG_TOP_TXDFE1_TX_GAIN_CTL_TX_SWAP_IQ_EN_OFFSET 9
#define TX_DIG_TOP_TXDFE1_TX_GAIN_CTL_TX_SWAP_IQ_EN_SIZE 1

/** bit field defines for tx_dig_top_txdfe1_s#mix_ctl */
#define TX_DIG_TOP_TXDFE1_MIX_CTL_TX_MIX_EN_OFFSET 1
#define TX_DIG_TOP_TXDFE1_MIX_CTL_TX_MIX_EN_SIZE 1
#define TX_DIG_TOP_TXDFE1_MIX_CTL_NCO_PHASE_INC_OFFSET 2
#define TX_DIG_TOP_TXDFE1_MIX_CTL_NCO_PHASE_INC_SIZE 24

/** bit field defines for tx_dig_top_txdfe1_s#dcoc_ctl */
#define TX_DIG_TOP_TXDFE1_DCOC_CTL_TX_DCOC_CORR_EN_OFFSET 0
#define TX_DIG_TOP_TXDFE1_DCOC_CTL_TX_DCOC_CORR_EN_SIZE 1
#define TX_DIG_TOP_TXDFE1_DCOC_CTL_TX_DCOC_CORR_I_OFFSET 1
#define TX_DIG_TOP_TXDFE1_DCOC_CTL_TX_DCOC_CORR_I_SIZE 11
#define TX_DIG_TOP_TXDFE1_DCOC_CTL_TX_DCOC_CORR_Q_OFFSET 12
#define TX_DIG_TOP_TXDFE1_DCOC_CTL_TX_DCOC_CORR_Q_SIZE 11

/** bit field defines for tx_dig_top_txdfe1_s#iq_corr_ctrl */
#define TX_DIG_TOP_TXDFE1_IQ_CORR_CTRL_TX_IQCORR_EN_OFFSET 0
#define TX_DIG_TOP_TXDFE1_IQ_CORR_CTRL_TX_IQCORR_EN_SIZE 1
#define TX_DIG_TOP_TXDFE1_IQ_CORR_CTRL_TX_IQCORR_1_OFFSET 1
#define TX_DIG_TOP_TXDFE1_IQ_CORR_CTRL_TX_IQCORR_1_SIZE 12
#define TX_DIG_TOP_TXDFE1_IQ_CORR_CTRL_TX_IQCORR_2_OFFSET 13
#define TX_DIG_TOP_TXDFE1_IQ_CORR_CTRL_TX_IQCORR_2_SIZE 12

/** bit field defines for tx_dig_top_txdfe1_s#filts_ctrl_1 */
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_1_FARROW_EN_OFFSET 0
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_1_FARROW_EN_SIZE 1
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_1_TX_COMP_EN_OFFSET 1
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_1_TX_COMP_EN_SIZE 1
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_1_DROOP_CENTRE_COEFF_OFFSET 4
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_1_DROOP_CENTRE_COEFF_SIZE 6
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_1_CHAN_FILT_SEL_OFFSET 10
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_1_CHAN_FILT_SEL_SIZE 2
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_1_UPSAMP_FILT_SEL_OFFSET 12
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_1_UPSAMP_FILT_SEL_SIZE 7

/** bit field defines for tx_dig_top_txdfe1_s#filts_ctrl_2 */
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_2_TX_FARROW_GAIN_SEL_OFFSET 0
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_2_TX_FARROW_GAIN_SEL_SIZE 5

/** bit field defines for tx_dig_top_txdfe1_s#filts_ctrl_3 */
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_3_FR_CTRL_OFFSET 0
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_3_FR_CTRL_SIZE 32

/** bit field defines for tx_dig_top_txdfe1_s#lte_ctrl1 */
#define TX_DIG_TOP_TXDFE1_LTE_CTRL1_ENV_CALC_EN_OFFSET 0
#define TX_DIG_TOP_TXDFE1_LTE_CTRL1_ENV_CALC_EN_SIZE 1
#define TX_DIG_TOP_TXDFE1_LTE_CTRL1_CMPLX_MULT_EN_OFFSET 1
#define TX_DIG_TOP_TXDFE1_LTE_CTRL1_CMPLX_MULT_EN_SIZE 1
#define TX_DIG_TOP_TXDFE1_LTE_CTRL1_SCL_EN_OFFSET 2
#define TX_DIG_TOP_TXDFE1_LTE_CTRL1_SCL_EN_SIZE 1
#define TX_DIG_TOP_TXDFE1_LTE_CTRL1_RAMP_PSEL_OFFSET 3
#define TX_DIG_TOP_TXDFE1_LTE_CTRL1_RAMP_PSEL_SIZE 1
#define TX_DIG_TOP_TXDFE1_LTE_CTRL1_DPD_PSEL_OFFSET 4
#define TX_DIG_TOP_TXDFE1_LTE_CTRL1_DPD_PSEL_SIZE 1
#define TX_DIG_TOP_TXDFE1_LTE_CTRL1_DPD_CGAIN_SEL_OFFSET 5
#define TX_DIG_TOP_TXDFE1_LTE_CTRL1_DPD_CGAIN_SEL_SIZE 4
#define TX_DIG_TOP_TXDFE1_LTE_CTRL1_DPD_PFGAIN_SEL_OFFSET 9
#define TX_DIG_TOP_TXDFE1_LTE_CTRL1_DPD_PFGAIN_SEL_SIZE 4
#define TX_DIG_TOP_TXDFE1_LTE_CTRL1_DPD_PFGAIN_EN_OFFSET 13
#define TX_DIG_TOP_TXDFE1_LTE_CTRL1_DPD_PFGAIN_EN_SIZE 1

/** bit field defines for tx_dig_top_txdfe1_s#test_ctrl */
#define TX_DIG_TOP_TXDFE1_TEST_CTRL_TEST_TONE_EN_OFFSET 0
#define TX_DIG_TOP_TXDFE1_TEST_CTRL_TEST_TONE_EN_SIZE 1
#define TX_DIG_TOP_TXDFE1_TEST_CTRL_TEST_TONE2_EN_OFFSET 1
#define TX_DIG_TOP_TXDFE1_TEST_CTRL_TEST_TONE2_EN_SIZE 1
#define TX_DIG_TOP_TXDFE1_TEST_CTRL_TEST_TONE2_SCL_OFFSET 2
#define TX_DIG_TOP_TXDFE1_TEST_CTRL_TEST_TONE2_SCL_SIZE 4
#define TX_DIG_TOP_TXDFE1_TEST_CTRL_LNCO_PHASE_INC_TONE2_OFFSET 6
#define TX_DIG_TOP_TXDFE1_TEST_CTRL_LNCO_PHASE_INC_TONE2_SIZE 24
#define TX_DIG_TOP_TXDFE1_TEST_CTRL_TEST_TONE2_TO_PGAIN_OFFSET 30
#define TX_DIG_TOP_TXDFE1_TEST_CTRL_TEST_TONE2_TO_PGAIN_SIZE 1

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl1 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL1_P1_DATA1_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL1_P1_DATA1_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl2 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL2_P1_DATA2_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL2_P1_DATA2_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl3 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL3_P1_DATA3_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL3_P1_DATA3_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl4 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL4_P1_DATA4_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL4_P1_DATA4_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl5 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL5_P1_DATA5_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL5_P1_DATA5_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl6 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL6_P1_DATA6_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL6_P1_DATA6_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl7 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL7_P1_DATA7_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL7_P1_DATA7_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl8 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL8_P1_DATA8_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL8_P1_DATA8_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl9 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL9_P1_DATA9_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL9_P1_DATA9_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl10 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL10_P1_DATA10_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL10_P1_DATA10_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl11 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL11_P1_DATA11_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL11_P1_DATA11_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl12 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL12_P1_DATA12_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL12_P1_DATA12_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl13 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL13_P1_DATA13_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL13_P1_DATA13_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl14 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL14_P1_DATA14_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL14_P1_DATA14_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl15 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL15_P1_DATA15_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL15_P1_DATA15_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl16 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL16_P1_DATA16_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL16_P1_DATA16_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl17 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL17_P1_DATA17_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL17_P1_DATA17_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl18 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL18_P1_DATA18_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL18_P1_DATA18_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl19 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL19_P1_DATA19_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL19_P1_DATA19_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl20 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL20_P1_DATA20_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL20_P1_DATA20_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl21 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL21_P1_DATA21_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL21_P1_DATA21_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl22 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL22_P1_DATA22_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL22_P1_DATA22_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl23 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL23_P1_DATA23_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL23_P1_DATA23_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl24 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL24_P1_DATA24_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL24_P1_DATA24_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl25 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL25_P1_DATA25_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL25_P1_DATA25_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl26 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL26_P1_DATA26_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL26_P1_DATA26_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl27 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL27_P1_DATA27_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL27_P1_DATA27_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl28 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL28_P1_DATA28_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL28_P1_DATA28_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl29 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL29_P1_DATA29_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL29_P1_DATA29_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl30 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL30_P1_DATA30_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL30_P1_DATA30_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl31 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL31_P1_DATA31_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL31_P1_DATA31_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p1_ctrl32 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL32_P1_DATA32_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P1_CTRL32_P1_DATA32_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl1 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL1_P2_DATA1_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL1_P2_DATA1_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl2 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL2_P2_DATA2_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL2_P2_DATA2_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl3 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL3_P2_DATA3_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL3_P2_DATA3_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl4 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL4_P2_DATA4_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL4_P2_DATA4_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl5 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL5_P2_DATA5_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL5_P2_DATA5_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl6 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL6_P2_DATA6_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL6_P2_DATA6_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl7 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL7_P2_DATA7_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL7_P2_DATA7_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl8 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL8_P2_DATA8_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL8_P2_DATA8_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl9 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL9_P2_DATA9_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL9_P2_DATA9_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl10 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL10_P2_DATA10_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL10_P2_DATA10_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl11 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL11_P2_DATA11_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL11_P2_DATA11_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl12 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL12_P2_DATA12_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL12_P2_DATA12_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl13 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL13_P2_DATA13_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL13_P2_DATA13_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl14 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL14_P2_DATA14_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL14_P2_DATA14_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl15 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL15_P2_DATA15_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL15_P2_DATA15_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl16 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL16_P2_DATA16_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL16_P2_DATA16_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl17 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL17_P2_DATA17_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL17_P2_DATA17_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl18 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL18_P2_DATA18_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL18_P2_DATA18_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl19 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL19_P2_DATA19_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL19_P2_DATA19_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl20 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL20_P2_DATA20_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL20_P2_DATA20_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl21 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL21_P2_DATA21_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL21_P2_DATA21_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl22 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL22_P2_DATA22_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL22_P2_DATA22_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl23 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL23_P2_DATA23_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL23_P2_DATA23_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl24 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL24_P2_DATA24_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL24_P2_DATA24_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl25 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL25_P2_DATA25_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL25_P2_DATA25_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl26 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL26_P2_DATA26_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL26_P2_DATA26_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl27 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL27_P2_DATA27_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL27_P2_DATA27_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl28 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL28_P2_DATA28_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL28_P2_DATA28_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl29 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL29_P2_DATA29_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL29_P2_DATA29_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl30 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL30_P2_DATA30_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL30_P2_DATA30_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl31 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL31_P2_DATA31_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL31_P2_DATA31_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#cmplx_mult_p2_ctrl32 */
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL32_P2_DATA32_OFFSET 0
#define TX_DIG_TOP_TXDFE1_CMPLX_MULT_P2_CTRL32_P2_DATA32_SIZE 20

/** bit field defines for tx_dig_top_txdfe1_s#tx_loop_ctl */
#define TX_DIG_TOP_TXDFE1_TX_LOOP_CTL_EN_TXDFE_DATA_OUT_OFFSET 0
#define TX_DIG_TOP_TXDFE1_TX_LOOP_CTL_EN_TXDFE_DATA_OUT_SIZE 1

/** bit field defines for tx_dig_top_txdfe1_s#filts_ctrl_4 */
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_4_TX_FARROW_OFFSET_I_OFFSET 0
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_4_TX_FARROW_OFFSET_I_SIZE 32

/** bit field defines for tx_dig_top_txdfe1_s#filts_ctrl_5 */
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_5_TX_FARROW_OFFSET_Q_OFFSET 0
#define TX_DIG_TOP_TXDFE1_FILTS_CTRL_5_TX_FARROW_OFFSET_Q_SIZE 32

/** bit field defines for tx_dig_top_txdfe1_s#tx_dfe_del_ctrl */
#define TX_DIG_TOP_TXDFE1_TX_DFE_DEL_CTRL_TX_FREEZE_EN_OFFSET 0
#define TX_DIG_TOP_TXDFE1_TX_DFE_DEL_CTRL_TX_FREEZE_EN_SIZE 1
#define TX_DIG_TOP_TXDFE1_TX_DFE_DEL_CTRL_TX_DEL_MODE_OFFSET 1
#define TX_DIG_TOP_TXDFE1_TX_DFE_DEL_CTRL_TX_DEL_MODE_SIZE 2
#define TX_DIG_TOP_TXDFE1_TX_DFE_DEL_CTRL_TX_DEL_VAL_OFFSET 3
#define TX_DIG_TOP_TXDFE1_TX_DFE_DEL_CTRL_TX_DEL_VAL_SIZE 13
#define TX_DIG_TOP_TXDFE1_TX_DFE_DEL_CTRL_FARROW_GD_ADJUST_OFFSET 16
#define TX_DIG_TOP_TXDFE1_TX_DFE_DEL_CTRL_FARROW_GD_ADJUST_SIZE 2
#define TX_DIG_TOP_TXDFE1_TX_DFE_DEL_CTRL_TX_VAL_CTRL_EN_OFFSET 18
#define TX_DIG_TOP_TXDFE1_TX_DFE_DEL_CTRL_TX_VAL_CTRL_EN_SIZE 1

/** bit field defines for tx_dig_top_txdfe1_s#tx_dfe_reset */
#define TX_DIG_TOP_TXDFE1_TX_DFE_RESET_TX_RSTN_OFFSET 0
#define TX_DIG_TOP_TXDFE1_TX_DFE_RESET_TX_RSTN_SIZE 1

/** bit field defines for tx_dig_top_txdfe1_s#tx_dfe_scratch_ctrl */
#define TX_DIG_TOP_TXDFE1_TX_DFE_SCRATCH_CTRL_BYPASS_DFE_OFFSET 0
#define TX_DIG_TOP_TXDFE1_TX_DFE_SCRATCH_CTRL_BYPASS_DFE_SIZE 1

/* EOF tx_dig_top_txdfe1.h */
#endif
