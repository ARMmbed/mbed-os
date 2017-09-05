#ifndef INCLUDED_PLL_DIG_IF
#define INCLUDED_PLL_DIG_IF
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** pll_dig_if HAL Spreadsheet version number */
#define PLL_DIG_IF_HAL_VERSION 2

/** This is the synthesizer register description
*/
struct pll_dig_if_s {
   /** PFD_CP and associated regulator control at address offset 0x000, read-write */
   uint32_t pfdcplf_1;
   /** LF and PFD_CP_LF test at address offset 0x004, read-write */
   uint32_t pfdcplf_2;
   /** Prescaler and associated regulator control at address offset 0x008, read-write */
   uint32_t prescaler_1;
   /** LO and associated regulator control at address offset 0x00C, read-write */
   uint32_t lo_1;
   /** VCO regulator control at address offset 0x010, read-write */
   uint32_t vco_1;
   /** VCO control at address offset 0x014, read-write */
   uint32_t vco_2;
   /** VCO ctune test#1 at address offset 0x018, read-write */
   uint32_t vco_3;
   /** VCO ctune test#2 at address offset 0x01C, read-write */
   uint32_t vco_4;
   /** VCO control read back at address offset 0x020, read-only */
   uint32_t vco_5;
   /** Sigma Delta control and test at address offset 0x024, read-write */
   uint32_t sd_1;
   /** Sigma Delta fractional value at address offset 0x028, read-write */
   uint32_t sd_2;
   /** sigma delta ublox modes at address offset 0x02C, read-write */
   uint32_t sd_3;
   /** sigma delta ublox modes at address offset 0x030, read-write */
   uint32_t sd_4;
   /**  at address offset 0x034, read-write */
   uint32_t sd_5;
   /** vco calibration control at address offset 0x038, read-write */
   uint32_t vcocal_1;
   /** vco calibration control at address offset 0x03C, read-write */
   uint32_t vcocal_2;
   /** vco calibration control at address offset 0x040, read-write */
   uint32_t vcocal_3;
   /** vco calibration status at address offset 0x044, read-only */
   uint32_t vcocal_4;
   /** vco calibration status at address offset 0x048, read-only */
   uint32_t vcocal_5;
   /** vco calibration status at address offset 0x04C, read-only */
   uint32_t vcocal_6;
   /** vco calibration status at address offset 0x050, read-only */
   uint32_t vcocal_7;
   /** vco calibration status at address offset 0x054, read-only */
   uint32_t vcocal_8;
   /** vco calibration status at address offset 0x058, read-only */
   uint32_t vcocal_9;
   /** vco calibration status at address offset 0x05C, read-only */
   uint32_t vcocal_10;
   /** vco calibration status at address offset 0x060, read-only */
   uint32_t vcocal_11;
   /** vco calibration status at address offset 0x064, read-only */
   uint32_t vcocal_12;
   /** vco calibration status at address offset 0x068, read-only */
   uint32_t vcocal_13;
   /** vco calibration status at address offset 0x06C, read-only */
   uint32_t vcocal_14;
   /** vco calibration status at address offset 0x070, read-only */
   uint32_t vcocal_15;
   /** vco calibration status at address offset 0x074, read-only */
   uint32_t vcocal_16;
   /** vco calibration status at address offset 0x078, read-only */
   uint32_t vcocal_17;
   /** vco calibration status at address offset 0x07C, read-only */
   uint32_t vcocal_18;
   /** vco calibration status at address offset 0x080, read-only */
   uint32_t vcocal_19;
   /** vco calibration status at address offset 0x084, read-only */
   uint32_t vcocal_20;
   /** vco calibration status at address offset 0x088, read-only */
   uint32_t vcocal_21;
   /** vco calibration status at address offset 0x08C, read-only */
   uint32_t vcocal_22;
   /** vco calibration status at address offset 0x090, read-only */
   uint32_t vcocal_23;
   /** vco calibration status at address offset 0x094, read-only */
   uint32_t vcocal_24;
   /** vco calibration status at address offset 0x098, read-only */
   uint32_t vcocal_25;
   /** vco calibration status at address offset 0x09C, read-only */
   uint32_t vcocal_26;
   /** vco calibration status at address offset 0x0A0, read-only */
   uint32_t vcocal_27;
   /** vco calibration status at address offset 0x0A4, read-only */
   uint32_t vcocal_28;
   /** vco calibration status at address offset 0x0A8, read-only */
   uint32_t vcocal_29;
   /** ripple count readback at address offset 0x0AC, read-only */
   uint32_t vcocal_30;
   /** Iref and associated regulator control at address offset 0x0B0, read-write */
   uint32_t iref_1;
   /** Iref and associated regulator control at address offset 0x0B4, read-write */
   uint32_t iref_2;
   /** Iref and associated regulator control at address offset 0x0B8, read-write */
   uint32_t iref_3;
   /**  at address offset 0x0BC, read-write */
   uint32_t iref_4;
   /** spare 1 at address offset 0x0C0, read-write */
   uint32_t spare1;
   /** spare 2 at address offset 0x0C4, read-write */
   uint32_t spare2;
   /** spare 3 at address offset 0x0C8, read-write */
   uint32_t spare3;
};

/** bit field defines for pll_dig_if_s#pfdcplf_1 */
#define PLL_DIG_IF_PFDCPLF_1_PFDCPLF_REG_EN_OFFSET 0
#define PLL_DIG_IF_PFDCPLF_1_PFDCPLF_REG_EN_SIZE 1
#define PLL_DIG_IF_PFDCPLF_1_PFDCPLF_REG_BYPASS_OFFSET 1
#define PLL_DIG_IF_PFDCPLF_1_PFDCPLF_REG_BYPASS_SIZE 1
#define PLL_DIG_IF_PFDCPLF_1_PFDCPLF_REG_FAST_LOCK_OFFSET 2
#define PLL_DIG_IF_PFDCPLF_1_PFDCPLF_REG_FAST_LOCK_SIZE 1
#define PLL_DIG_IF_PFDCPLF_1_PFDCPLF_REG_CTRL_OFFSET 3
#define PLL_DIG_IF_PFDCPLF_1_PFDCPLF_REG_CTRL_SIZE 4
#define PLL_DIG_IF_PFDCPLF_1_PFD_EN_OFFSET 7
#define PLL_DIG_IF_PFDCPLF_1_PFD_EN_SIZE 1
#define PLL_DIG_IF_PFDCPLF_1_PFD_DELAY_SELECT_OFFSET 8
#define PLL_DIG_IF_PFDCPLF_1_PFD_DELAY_SELECT_SIZE 2
#define PLL_DIG_IF_PFDCPLF_1_CP_EN_OFFSET 10
#define PLL_DIG_IF_PFDCPLF_1_CP_EN_SIZE 1
#define PLL_DIG_IF_PFDCPLF_1_CP_GAIN_OFFSET 11
#define PLL_DIG_IF_PFDCPLF_1_CP_GAIN_SIZE 5
#define PLL_DIG_IF_PFDCPLF_1_CP_MISMATCH_DOWN_OFFSET 16
#define PLL_DIG_IF_PFDCPLF_1_CP_MISMATCH_DOWN_SIZE 5
#define PLL_DIG_IF_PFDCPLF_1_CP_MISMATCH_UP_OFFSET 21
#define PLL_DIG_IF_PFDCPLF_1_CP_MISMATCH_UP_SIZE 5
#define PLL_DIG_IF_PFDCPLF_1_CP_VREF_TRIM_OFFSET 26
#define PLL_DIG_IF_PFDCPLF_1_CP_VREF_TRIM_SIZE 4
#define PLL_DIG_IF_PFDCPLF_1_PFDCPLF_REG_EN_REF_CURRENT_OFFSET 30
#define PLL_DIG_IF_PFDCPLF_1_PFDCPLF_REG_EN_REF_CURRENT_SIZE 1

/** bit field defines for pll_dig_if_s#pfdcplf_2 */
#define PLL_DIG_IF_PFDCPLF_2_FILTER_EN_CL_OFFSET 0
#define PLL_DIG_IF_PFDCPLF_2_FILTER_EN_CL_SIZE 1
#define PLL_DIG_IF_PFDCPLF_2_FILTER_EN_CAL_OFFSET 1
#define PLL_DIG_IF_PFDCPLF_2_FILTER_EN_CAL_SIZE 1
#define PLL_DIG_IF_PFDCPLF_2_FILTER_VREF_TRIM_OFFSET 2
#define PLL_DIG_IF_PFDCPLF_2_FILTER_VREF_TRIM_SIZE 4
#define PLL_DIG_IF_PFDCPLF_2_FILTER_VCO_CAL_OFFSET 6
#define PLL_DIG_IF_PFDCPLF_2_FILTER_VCO_CAL_SIZE 5
#define PLL_DIG_IF_PFDCPLF_2_FILTER_RZ_PROG_OFFSET 11
#define PLL_DIG_IF_PFDCPLF_2_FILTER_RZ_PROG_SIZE 5
#define PLL_DIG_IF_PFDCPLF_2_FILTER_RP_PROG_OFFSET 16
#define PLL_DIG_IF_PFDCPLF_2_FILTER_RP_PROG_SIZE 2
#define PLL_DIG_IF_PFDCPLF_2_PFDCPLF_TEST_EN_OFFSET 18
#define PLL_DIG_IF_PFDCPLF_2_PFDCPLF_TEST_EN_SIZE 1
#define PLL_DIG_IF_PFDCPLF_2_PFDCPLF_TEST_SELECT_OFFSET 19
#define PLL_DIG_IF_PFDCPLF_2_PFDCPLF_TEST_SELECT_SIZE 5
#define PLL_DIG_IF_PFDCPLF_2_PFD_TEST_EN_OFFSET 24
#define PLL_DIG_IF_PFDCPLF_2_PFD_TEST_EN_SIZE 1
#define PLL_DIG_IF_PFDCPLF_2_PFD_TEST_UP_OFFSET 25
#define PLL_DIG_IF_PFDCPLF_2_PFD_TEST_UP_SIZE 1
#define PLL_DIG_IF_PFDCPLF_2_PFD_TEST_DOWN_OFFSET 26
#define PLL_DIG_IF_PFDCPLF_2_PFD_TEST_DOWN_SIZE 1
#define PLL_DIG_IF_PFDCPLF_2_FILTER_EN_TEST_VTUNE_OFFSET 27
#define PLL_DIG_IF_PFDCPLF_2_FILTER_EN_TEST_VTUNE_SIZE 1
#define PLL_DIG_IF_PFDCPLF_2_EN_ANATEST_OFFSET 28
#define PLL_DIG_IF_PFDCPLF_2_EN_ANATEST_SIZE 1
#define PLL_DIG_IF_PFDCPLF_2_EN_AUXADC_TEST_OFFSET 29
#define PLL_DIG_IF_PFDCPLF_2_EN_AUXADC_TEST_SIZE 1

/** bit field defines for pll_dig_if_s#prescaler_1 */
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_REG_EN_OFFSET 0
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_REG_EN_SIZE 1
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_REG_BYPASS_OFFSET 1
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_REG_BYPASS_SIZE 1
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_REG_FAST_LOCK_OFFSET 2
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_REG_FAST_LOCK_SIZE 1
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_REG_CTRL_OFFSET 3
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_REG_CTRL_SIZE 4
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_EN_OFFSET 7
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_EN_SIZE 1
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_BIAS_EN_OFFSET 8
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_BIAS_EN_SIZE 1
#define PLL_DIG_IF_PRESCALER_1_VCO_CAL_CLK_EN_OFFSET 9
#define PLL_DIG_IF_PRESCALER_1_VCO_CAL_CLK_EN_SIZE 1
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_TEST_EN_OFFSET 10
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_TEST_EN_SIZE 1
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_TEST_SELECT_OFFSET 11
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_TEST_SELECT_SIZE 3
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_REG_EN_REF_CURRENT_OFFSET 14
#define PLL_DIG_IF_PRESCALER_1_PRESCALER_REG_EN_REF_CURRENT_SIZE 1

/** bit field defines for pll_dig_if_s#lo_1 */
#define PLL_DIG_IF_LO_1_LO_REG_EN_OFFSET 0
#define PLL_DIG_IF_LO_1_LO_REG_EN_SIZE 1
#define PLL_DIG_IF_LO_1_LO_REG_BYPASS_OFFSET 1
#define PLL_DIG_IF_LO_1_LO_REG_BYPASS_SIZE 1
#define PLL_DIG_IF_LO_1_LO_REG_FAST_LOCK_OFFSET 2
#define PLL_DIG_IF_LO_1_LO_REG_FAST_LOCK_SIZE 1
#define PLL_DIG_IF_LO_1_LO_REG_CTRL_OFFSET 3
#define PLL_DIG_IF_LO_1_LO_REG_CTRL_SIZE 4
#define PLL_DIG_IF_LO_1_LO_EN_OFFSET 7
#define PLL_DIG_IF_LO_1_LO_EN_SIZE 1
#define PLL_DIG_IF_LO_1_LO1_EN_OFFSET 8
#define PLL_DIG_IF_LO_1_LO1_EN_SIZE 1
#define PLL_DIG_IF_LO_1_LO2_EN_OFFSET 9
#define PLL_DIG_IF_LO_1_LO2_EN_SIZE 1
#define PLL_DIG_IF_LO_1_LO_VCO1_EN_OFFSET 10
#define PLL_DIG_IF_LO_1_LO_VCO1_EN_SIZE 1
#define PLL_DIG_IF_LO_1_LO_VCO2_EN_OFFSET 11
#define PLL_DIG_IF_LO_1_LO_VCO2_EN_SIZE 1
#define PLL_DIG_IF_LO_1_LO_VCO1_BYPASS_EN_OFFSET 12
#define PLL_DIG_IF_LO_1_LO_VCO1_BYPASS_EN_SIZE 1
#define PLL_DIG_IF_LO_1_LO_VCO2_DIV_STATE_OFFSET 13
#define PLL_DIG_IF_LO_1_LO_VCO2_DIV_STATE_SIZE 2
/**  */
#define PLL_DIG_IF_LO_1_LO_VCO2_DIV_STATE_DIV_1_VALUE 0
/**  */
#define PLL_DIG_IF_LO_1_LO_VCO2_DIV_STATE_DIV_2_VALUE 1
/**  */
#define PLL_DIG_IF_LO_1_LO_VCO2_DIV_STATE_DIV_3_VALUE 2
/**  */
#define PLL_DIG_IF_LO_1_LO_VCO2_DIV_STATE_DIV_4_VALUE 3
#define PLL_DIG_IF_LO_1_LO_VCO1_EXTLO_EN_OFFSET 15
#define PLL_DIG_IF_LO_1_LO_VCO1_EXTLO_EN_SIZE 1
#define PLL_DIG_IF_LO_1_LO_OUT1_EN_OFFSET 16
#define PLL_DIG_IF_LO_1_LO_OUT1_EN_SIZE 1
#define PLL_DIG_IF_LO_1_LO_OUT2_EN_OFFSET 17
#define PLL_DIG_IF_LO_1_LO_OUT2_EN_SIZE 1
#define PLL_DIG_IF_LO_1_LO_OUT3_EN_OFFSET 18
#define PLL_DIG_IF_LO_1_LO_OUT3_EN_SIZE 1
#define PLL_DIG_IF_LO_1_VCO1_PRESCALER_CLK_EN_OFFSET 19
#define PLL_DIG_IF_LO_1_VCO1_PRESCALER_CLK_EN_SIZE 1
#define PLL_DIG_IF_LO_1_VCO2_PRESCALER_CLK_EN_OFFSET 20
#define PLL_DIG_IF_LO_1_VCO2_PRESCALER_CLK_EN_SIZE 1
#define PLL_DIG_IF_LO_1_LO_TEST_EN_OFFSET 21
#define PLL_DIG_IF_LO_1_LO_TEST_EN_SIZE 1
#define PLL_DIG_IF_LO_1_LO_TEST_SELECT_OFFSET 22
#define PLL_DIG_IF_LO_1_LO_TEST_SELECT_SIZE 3
#define PLL_DIG_IF_LO_1_LO_REG_EN_REF_CURRENT_OFFSET 25
#define PLL_DIG_IF_LO_1_LO_REG_EN_REF_CURRENT_SIZE 1

/** bit field defines for pll_dig_if_s#vco_1 */
#define PLL_DIG_IF_VCO_1_VCO_REG_EN_OFFSET 0
#define PLL_DIG_IF_VCO_1_VCO_REG_EN_SIZE 1
#define PLL_DIG_IF_VCO_1_VCO_REG_BYPASS_OFFSET 1
#define PLL_DIG_IF_VCO_1_VCO_REG_BYPASS_SIZE 1
#define PLL_DIG_IF_VCO_1_VCO_REG_FAST_LOCK_OFFSET 2
#define PLL_DIG_IF_VCO_1_VCO_REG_FAST_LOCK_SIZE 1
#define PLL_DIG_IF_VCO_1_VCO_REG_CTRL_OFFSET 3
#define PLL_DIG_IF_VCO_1_VCO_REG_CTRL_SIZE 4
#define PLL_DIG_IF_VCO_1_VCO_AUX_REG_EN_OFFSET 7
#define PLL_DIG_IF_VCO_1_VCO_AUX_REG_EN_SIZE 1
#define PLL_DIG_IF_VCO_1_VCO_AUX_REG_BYPASS_OFFSET 8
#define PLL_DIG_IF_VCO_1_VCO_AUX_REG_BYPASS_SIZE 1
#define PLL_DIG_IF_VCO_1_VCO_AUX_REG_FAST_LOCK_OFFSET 9
#define PLL_DIG_IF_VCO_1_VCO_AUX_REG_FAST_LOCK_SIZE 1
#define PLL_DIG_IF_VCO_1_VCO_AUX_REG_CTRL_OFFSET 10
#define PLL_DIG_IF_VCO_1_VCO_AUX_REG_CTRL_SIZE 4
#define PLL_DIG_IF_VCO_1_VCO_REG_EN_REF_CURRENT_OFFSET 14
#define PLL_DIG_IF_VCO_1_VCO_REG_EN_REF_CURRENT_SIZE 1
#define PLL_DIG_IF_VCO_1_VCO_AUX_REG_EN_REF_CURRENT_OFFSET 15
#define PLL_DIG_IF_VCO_1_VCO_AUX_REG_EN_REF_CURRENT_SIZE 1

/** bit field defines for pll_dig_if_s#vco_2 */
#define PLL_DIG_IF_VCO_2_VCO1_EN_OFFSET 0
#define PLL_DIG_IF_VCO_2_VCO1_EN_SIZE 1
#define PLL_DIG_IF_VCO_2_VCO2_EN_OFFSET 1
#define PLL_DIG_IF_VCO_2_VCO2_EN_SIZE 1
#define PLL_DIG_IF_VCO_2_VCO_TEMPSENSE_ENABLE_OFFSET 2
#define PLL_DIG_IF_VCO_2_VCO_TEMPSENSE_ENABLE_SIZE 1
#define PLL_DIG_IF_VCO_2_VCO_BANDGAP_TEMPCOMP_EN_OFFSET 3
#define PLL_DIG_IF_VCO_2_VCO_BANDGAP_TEMPCOMP_EN_SIZE 1
#define PLL_DIG_IF_VCO_2_VCO_BANDGAP_TEMPCOMP_TRIM_OFFSET 4
#define PLL_DIG_IF_VCO_2_VCO_BANDGAP_TEMPCOMP_TRIM_SIZE 4
#define PLL_DIG_IF_VCO_2_VCO_TEMPCOMP_SEL_OFFSET 8
#define PLL_DIG_IF_VCO_2_VCO_TEMPCOMP_SEL_SIZE 2
#define PLL_DIG_IF_VCO_2_VCO_FTUNE_OFFSET 10
#define PLL_DIG_IF_VCO_2_VCO_FTUNE_SIZE 4
#define PLL_DIG_IF_VCO_2_VCO_CTUNE_OFFSET 14
#define PLL_DIG_IF_VCO_2_VCO_CTUNE_SIZE 6
#define PLL_DIG_IF_VCO_2_VCO_KV_SEL_OFFSET 20
#define PLL_DIG_IF_VCO_2_VCO_KV_SEL_SIZE 3
#define PLL_DIG_IF_VCO_2_VCO_RTAIL_SEL_OFFSET 23
#define PLL_DIG_IF_VCO_2_VCO_RTAIL_SEL_SIZE 2
#define PLL_DIG_IF_VCO_2_VCO_TEST_EN_OFFSET 25
#define PLL_DIG_IF_VCO_2_VCO_TEST_EN_SIZE 1
#define PLL_DIG_IF_VCO_2_VCO_TEST_SELECT_OFFSET 26
#define PLL_DIG_IF_VCO_2_VCO_TEST_SELECT_SIZE 3
#define PLL_DIG_IF_VCO_2_BANDGAP_FAST_LOCK_OFFSET 29
#define PLL_DIG_IF_VCO_2_BANDGAP_FAST_LOCK_SIZE 1

/** bit field defines for pll_dig_if_s#vco_3 */
#define PLL_DIG_IF_VCO_3_VCO_CTUNE_TEST_EN_OFFSET 0
#define PLL_DIG_IF_VCO_3_VCO_CTUNE_TEST_EN_SIZE 1
#define PLL_DIG_IF_VCO_3_VCO_CTUNE_TEST_OFFSET 1
#define PLL_DIG_IF_VCO_3_VCO_CTUNE_TEST_SIZE 31

/** bit field defines for pll_dig_if_s#vco_4 */
#define PLL_DIG_IF_VCO_4_VCO_CTUNE_TEST_OFFSET 0
#define PLL_DIG_IF_VCO_4_VCO_CTUNE_TEST_SIZE 32

/** bit field defines for pll_dig_if_s#vco_5 */
#define PLL_DIG_IF_VCO_5_VCO_FTUNE_RD_OFFSET 0
#define PLL_DIG_IF_VCO_5_VCO_FTUNE_RD_SIZE 4
#define PLL_DIG_IF_VCO_5_VCO_CTUNE_RD_OFFSET 4
#define PLL_DIG_IF_VCO_5_VCO_CTUNE_RD_SIZE 6

/** bit field defines for pll_dig_if_s#sd_1 */
#define PLL_DIG_IF_SD_1_SD_EN_OFFSET 0
#define PLL_DIG_IF_SD_1_SD_EN_SIZE 1
#define PLL_DIG_IF_SD_1_SD_ORDER_OFFSET 1
#define PLL_DIG_IF_SD_1_SD_ORDER_SIZE 1
#define PLL_DIG_IF_SD_1_INV_SD_CLK_OFFSET 2
#define PLL_DIG_IF_SD_1_INV_SD_CLK_SIZE 1
#define PLL_DIG_IF_SD_1_SD_DITHER_EN_OFFSET 3
#define PLL_DIG_IF_SD_1_SD_DITHER_EN_SIZE 1
#define PLL_DIG_IF_SD_1_SD_DITHER_LENGTH_SEL_OFFSET 4
#define PLL_DIG_IF_SD_1_SD_DITHER_LENGTH_SEL_SIZE 1
#define PLL_DIG_IF_SD_1_SD_DIV_TEST_ENABLE_OFFSET 5
#define PLL_DIG_IF_SD_1_SD_DIV_TEST_ENABLE_SIZE 1
#define PLL_DIG_IF_SD_1_SD_FREQ_INT_OFFSET 6
#define PLL_DIG_IF_SD_1_SD_FREQ_INT_SIZE 9
#define PLL_DIG_IF_SD_1_SD_TEST_FREQ_INT_OFFSET 15
#define PLL_DIG_IF_SD_1_SD_TEST_FREQ_INT_SIZE 9

/** bit field defines for pll_dig_if_s#sd_2 */
#define PLL_DIG_IF_SD_2_SD_FREQ_FRAC_OFFSET 0
#define PLL_DIG_IF_SD_2_SD_FREQ_FRAC_SIZE 24

/** bit field defines for pll_dig_if_s#sd_3 */
#define PLL_DIG_IF_SD_3_SD_MASH_GAIN_OFFSET 0
#define PLL_DIG_IF_SD_3_SD_MASH_GAIN_SIZE 2
#define PLL_DIG_IF_SD_3_EN_LOCK_DET_OFFSET 2
#define PLL_DIG_IF_SD_3_EN_LOCK_DET_SIZE 1
#define PLL_DIG_IF_SD_3_SD_SEL_INTEG0_OFFSET 3
#define PLL_DIG_IF_SD_3_SD_SEL_INTEG0_SIZE 1
#define PLL_DIG_IF_SD_3_SD_B_OFFSET 4
#define PLL_DIG_IF_SD_3_SD_B_SIZE 17
#define PLL_DIG_IF_SD_3_INV_LOCK_CLK_OFFSET 21
#define PLL_DIG_IF_SD_3_INV_LOCK_CLK_SIZE 1

/** bit field defines for pll_dig_if_s#sd_4 */
#define PLL_DIG_IF_SD_4_SD_A_OFFSET 0
#define PLL_DIG_IF_SD_4_SD_A_SIZE 17

/** bit field defines for pll_dig_if_s#sd_5 */
#define PLL_DIG_IF_SD_5_LOCK_OFFSET 0
#define PLL_DIG_IF_SD_5_LOCK_SIZE 1
#define PLL_DIG_IF_SD_5_LOCK_ENTRY_OFFSET 1
#define PLL_DIG_IF_SD_5_LOCK_ENTRY_SIZE 1
#define PLL_DIG_IF_SD_5_LOCK_EXIT_OFFSET 2
#define PLL_DIG_IF_SD_5_LOCK_EXIT_SIZE 1

/** bit field defines for pll_dig_if_s#vcocal_1 */
#define PLL_DIG_IF_VCOCAL_1_SYN_VCOCAL_TARGET_COUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_1_SYN_VCOCAL_TARGET_COUNT_SIZE 20
#define PLL_DIG_IF_VCOCAL_1_SYN_VCO_NN_START_OFFSET 20
#define PLL_DIG_IF_VCOCAL_1_SYN_VCO_NN_START_SIZE 3
#define PLL_DIG_IF_VCOCAL_1_SYN_VCOCAL_TRACE_EN_OFFSET 23
#define PLL_DIG_IF_VCOCAL_1_SYN_VCOCAL_TRACE_EN_SIZE 1
#define PLL_DIG_IF_VCOCAL_1_SYN_VCO_FTUNE_MODE_OFFSET 24
#define PLL_DIG_IF_VCOCAL_1_SYN_VCO_FTUNE_MODE_SIZE 1
#define PLL_DIG_IF_VCOCAL_1_SYN_VCO_CTUNE_MODE_OFFSET 25
#define PLL_DIG_IF_VCOCAL_1_SYN_VCO_CTUNE_MODE_SIZE 1
#define PLL_DIG_IF_VCOCAL_1_RIPPLE_COUNTER_EN_OFFSET 26
#define PLL_DIG_IF_VCOCAL_1_RIPPLE_COUNTER_EN_SIZE 1
#define PLL_DIG_IF_VCOCAL_1_SYN_VCOCAL_KVCAL_EN_OFFSET 27
#define PLL_DIG_IF_VCOCAL_1_SYN_VCOCAL_KVCAL_EN_SIZE 1
#define PLL_DIG_IF_VCOCAL_1_SYN_VCOCAL_FREQCAL_EN_OFFSET 28
#define PLL_DIG_IF_VCOCAL_1_SYN_VCOCAL_FREQCAL_EN_SIZE 1
#define PLL_DIG_IF_VCOCAL_1_SYN_VCOCAL_HB_LB_ENB_OFFSET 29
#define PLL_DIG_IF_VCOCAL_1_SYN_VCOCAL_HB_LB_ENB_SIZE 1

/** bit field defines for pll_dig_if_s#vcocal_2 */
#define PLL_DIG_IF_VCOCAL_2_SYN_VCOCAL_FINE_DEL_OFFSET 0
#define PLL_DIG_IF_VCOCAL_2_SYN_VCOCAL_FINE_DEL_SIZE 4
#define PLL_DIG_IF_VCOCAL_2_SYN_VCOCAL_C5_DEL_OFFSET 4
#define PLL_DIG_IF_VCOCAL_2_SYN_VCOCAL_C5_DEL_SIZE 4
#define PLL_DIG_IF_VCOCAL_2_SYN_VCOCAL_C4_DEL_OFFSET 8
#define PLL_DIG_IF_VCOCAL_2_SYN_VCOCAL_C4_DEL_SIZE 4
#define PLL_DIG_IF_VCOCAL_2_SYN_VCOCAL_C3_DEL_OFFSET 12
#define PLL_DIG_IF_VCOCAL_2_SYN_VCOCAL_C3_DEL_SIZE 4
#define PLL_DIG_IF_VCOCAL_2_SYN_VCOCAL_C2_DEL_OFFSET 16
#define PLL_DIG_IF_VCOCAL_2_SYN_VCOCAL_C2_DEL_SIZE 4
#define PLL_DIG_IF_VCOCAL_2_SYN_VCOCAL_C1_DEL_OFFSET 20
#define PLL_DIG_IF_VCOCAL_2_SYN_VCOCAL_C1_DEL_SIZE 4
#define PLL_DIG_IF_VCOCAL_2_SYN_VCOCAL_C0_DEL_OFFSET 24
#define PLL_DIG_IF_VCOCAL_2_SYN_VCOCAL_C0_DEL_SIZE 4

/** bit field defines for pll_dig_if_s#vcocal_3 */
#define PLL_DIG_IF_VCOCAL_3_SYN_VCOCAL_FINE_MEAS_OFFSET 0
#define PLL_DIG_IF_VCOCAL_3_SYN_VCOCAL_FINE_MEAS_SIZE 2
#define PLL_DIG_IF_VCOCAL_3_SYN_VCOCAL_C5_MEAS_OFFSET 2
#define PLL_DIG_IF_VCOCAL_3_SYN_VCOCAL_C5_MEAS_SIZE 2
#define PLL_DIG_IF_VCOCAL_3_SYN_VCOCAL_C4_MEAS_OFFSET 4
#define PLL_DIG_IF_VCOCAL_3_SYN_VCOCAL_C4_MEAS_SIZE 2
#define PLL_DIG_IF_VCOCAL_3_SYN_VCOCAL_C3_MEAS_OFFSET 6
#define PLL_DIG_IF_VCOCAL_3_SYN_VCOCAL_C3_MEAS_SIZE 2
#define PLL_DIG_IF_VCOCAL_3_SYN_VCOCAL_C2_MEAS_OFFSET 8
#define PLL_DIG_IF_VCOCAL_3_SYN_VCOCAL_C2_MEAS_SIZE 2
#define PLL_DIG_IF_VCOCAL_3_SYN_VCOCAL_C1_MEAS_OFFSET 10
#define PLL_DIG_IF_VCOCAL_3_SYN_VCOCAL_C1_MEAS_SIZE 2
#define PLL_DIG_IF_VCOCAL_3_SYN_VCOCAL_C0_MEAS_OFFSET 12
#define PLL_DIG_IF_VCOCAL_3_SYN_VCOCAL_C0_MEAS_SIZE 2
#define PLL_DIG_IF_VCOCAL_3_SYN_VCOCAL_MEAS_BASE_OFFSET 14
#define PLL_DIG_IF_VCOCAL_3_SYN_VCOCAL_MEAS_BASE_SIZE 16

/** bit field defines for pll_dig_if_s#vcocal_4 */
#define PLL_DIG_IF_VCOCAL_4_SYN_VCOCAL_C3_CTUNE_OFFSET 0
#define PLL_DIG_IF_VCOCAL_4_SYN_VCOCAL_C3_CTUNE_SIZE 6
#define PLL_DIG_IF_VCOCAL_4_SYN_VCOCAL_C2_CTUNE_OFFSET 6
#define PLL_DIG_IF_VCOCAL_4_SYN_VCOCAL_C2_CTUNE_SIZE 6
#define PLL_DIG_IF_VCOCAL_4_SYN_VCOCAL_C1_CTUNE_OFFSET 12
#define PLL_DIG_IF_VCOCAL_4_SYN_VCOCAL_C1_CTUNE_SIZE 6
#define PLL_DIG_IF_VCOCAL_4_SYN_VCOCAL_C0_CTUNE_OFFSET 18
#define PLL_DIG_IF_VCOCAL_4_SYN_VCOCAL_C0_CTUNE_SIZE 6

/** bit field defines for pll_dig_if_s#vcocal_5 */
#define PLL_DIG_IF_VCOCAL_5_SYN_VCOCAL_C5_CTUNE_OFFSET 0
#define PLL_DIG_IF_VCOCAL_5_SYN_VCOCAL_C5_CTUNE_SIZE 6
#define PLL_DIG_IF_VCOCAL_5_SYN_VCOCAL_C4_CTUNE_OFFSET 6
#define PLL_DIG_IF_VCOCAL_5_SYN_VCOCAL_C4_CTUNE_SIZE 6

/** bit field defines for pll_dig_if_s#vcocal_6 */
#define PLL_DIG_IF_VCOCAL_6_SYN_VCOCAL_F7_FTUNE_OFFSET 0
#define PLL_DIG_IF_VCOCAL_6_SYN_VCOCAL_F7_FTUNE_SIZE 4
#define PLL_DIG_IF_VCOCAL_6_SYN_VCOCAL_F6_FTUNE_OFFSET 4
#define PLL_DIG_IF_VCOCAL_6_SYN_VCOCAL_F6_FTUNE_SIZE 4
#define PLL_DIG_IF_VCOCAL_6_SYN_VCOCAL_F5_FTUNE_OFFSET 8
#define PLL_DIG_IF_VCOCAL_6_SYN_VCOCAL_F5_FTUNE_SIZE 4
#define PLL_DIG_IF_VCOCAL_6_SYN_VCOCAL_F4_FTUNE_OFFSET 12
#define PLL_DIG_IF_VCOCAL_6_SYN_VCOCAL_F4_FTUNE_SIZE 4
#define PLL_DIG_IF_VCOCAL_6_SYN_VCOCAL_F3_FTUNE_OFFSET 16
#define PLL_DIG_IF_VCOCAL_6_SYN_VCOCAL_F3_FTUNE_SIZE 4
#define PLL_DIG_IF_VCOCAL_6_SYN_VCOCAL_F2_FTUNE_OFFSET 20
#define PLL_DIG_IF_VCOCAL_6_SYN_VCOCAL_F2_FTUNE_SIZE 4
#define PLL_DIG_IF_VCOCAL_6_SYN_VCOCAL_F1_FTUNE_OFFSET 24
#define PLL_DIG_IF_VCOCAL_6_SYN_VCOCAL_F1_FTUNE_SIZE 4
#define PLL_DIG_IF_VCOCAL_6_SYN_VCOCAL_F0_FTUNE_OFFSET 28
#define PLL_DIG_IF_VCOCAL_6_SYN_VCOCAL_F0_FTUNE_SIZE 4

/** bit field defines for pll_dig_if_s#vcocal_7 */
#define PLL_DIG_IF_VCOCAL_7_SYN_VCOCAL_F15_FTUNE_OFFSET 0
#define PLL_DIG_IF_VCOCAL_7_SYN_VCOCAL_F15_FTUNE_SIZE 4
#define PLL_DIG_IF_VCOCAL_7_SYN_VCOCAL_F14_FTUNE_OFFSET 4
#define PLL_DIG_IF_VCOCAL_7_SYN_VCOCAL_F14_FTUNE_SIZE 4
#define PLL_DIG_IF_VCOCAL_7_SYN_VCOCAL_F13_FTUNE_OFFSET 8
#define PLL_DIG_IF_VCOCAL_7_SYN_VCOCAL_F13_FTUNE_SIZE 4
#define PLL_DIG_IF_VCOCAL_7_SYN_VCOCAL_F12_FTUNE_OFFSET 12
#define PLL_DIG_IF_VCOCAL_7_SYN_VCOCAL_F12_FTUNE_SIZE 4
#define PLL_DIG_IF_VCOCAL_7_SYN_VCOCAL_F11_FTUNE_OFFSET 16
#define PLL_DIG_IF_VCOCAL_7_SYN_VCOCAL_F11_FTUNE_SIZE 4
#define PLL_DIG_IF_VCOCAL_7_SYN_VCOCAL_F10_FTUNE_OFFSET 20
#define PLL_DIG_IF_VCOCAL_7_SYN_VCOCAL_F10_FTUNE_SIZE 4
#define PLL_DIG_IF_VCOCAL_7_SYN_VCOCAL_F9_FTUNE_OFFSET 24
#define PLL_DIG_IF_VCOCAL_7_SYN_VCOCAL_F9_FTUNE_SIZE 4
#define PLL_DIG_IF_VCOCAL_7_SYN_VCOCAL_F8_FTUNE_OFFSET 28
#define PLL_DIG_IF_VCOCAL_7_SYN_VCOCAL_F8_FTUNE_SIZE 4

/** bit field defines for pll_dig_if_s#vcocal_8 */
#define PLL_DIG_IF_VCOCAL_8_SYN_VCO_C0_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_8_SYN_VCO_C0_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_9 */
#define PLL_DIG_IF_VCOCAL_9_SYN_VCO_C1_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_9_SYN_VCO_C1_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_10 */
#define PLL_DIG_IF_VCOCAL_10_SYN_VCO_C2_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_10_SYN_VCO_C2_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_11 */
#define PLL_DIG_IF_VCOCAL_11_SYN_VCO_C3_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_11_SYN_VCO_C3_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_12 */
#define PLL_DIG_IF_VCOCAL_12_SYN_VCO_C4_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_12_SYN_VCO_C4_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_13 */
#define PLL_DIG_IF_VCOCAL_13_SYN_VCO_C5_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_13_SYN_VCO_C5_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_14 */
#define PLL_DIG_IF_VCOCAL_14_SYN_VCO_F0_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_14_SYN_VCO_F0_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_15 */
#define PLL_DIG_IF_VCOCAL_15_SYN_VCO_F1_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_15_SYN_VCO_F1_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_16 */
#define PLL_DIG_IF_VCOCAL_16_SYN_VCO_F2_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_16_SYN_VCO_F2_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_17 */
#define PLL_DIG_IF_VCOCAL_17_SYN_VCO_F3_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_17_SYN_VCO_F3_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_18 */
#define PLL_DIG_IF_VCOCAL_18_SYN_VCO_F4_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_18_SYN_VCO_F4_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_19 */
#define PLL_DIG_IF_VCOCAL_19_SYN_VCO_F5_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_19_SYN_VCO_F5_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_20 */
#define PLL_DIG_IF_VCOCAL_20_SYN_VCO_F6_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_20_SYN_VCO_F6_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_21 */
#define PLL_DIG_IF_VCOCAL_21_SYN_VCO_F7_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_21_SYN_VCO_F7_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_22 */
#define PLL_DIG_IF_VCOCAL_22_SYN_VCO_F8_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_22_SYN_VCO_F8_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_23 */
#define PLL_DIG_IF_VCOCAL_23_SYN_VCO_F9_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_23_SYN_VCO_F9_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_24 */
#define PLL_DIG_IF_VCOCAL_24_SYN_VCO_F10_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_24_SYN_VCO_F10_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_25 */
#define PLL_DIG_IF_VCOCAL_25_SYN_VCO_F11_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_25_SYN_VCO_F11_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_26 */
#define PLL_DIG_IF_VCOCAL_26_SYN_VCO_F12_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_26_SYN_VCO_F12_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_27 */
#define PLL_DIG_IF_VCOCAL_27_SYN_VCO_F13_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_27_SYN_VCO_F13_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_28 */
#define PLL_DIG_IF_VCOCAL_28_SYN_VCO_F14_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_28_SYN_VCO_F14_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_29 */
#define PLL_DIG_IF_VCOCAL_29_SYN_VCO_F15_RCOUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_29_SYN_VCO_F15_RCOUNT_SIZE 20

/** bit field defines for pll_dig_if_s#vcocal_30 */
#define PLL_DIG_IF_VCOCAL_30_SYN_VCOCAL_RIPPLE_COUNT_OFFSET 0
#define PLL_DIG_IF_VCOCAL_30_SYN_VCOCAL_RIPPLE_COUNT_SIZE 20

/** bit field defines for pll_dig_if_s#iref_1 */
#define PLL_DIG_IF_IREF_1_BANDGAP_EN_OFFSET 0
#define PLL_DIG_IF_IREF_1_BANDGAP_EN_SIZE 1
#define PLL_DIG_IF_IREF_1_BANDGAP_TRIM_OFFSET 1
#define PLL_DIG_IF_IREF_1_BANDGAP_TRIM_SIZE 6
#define PLL_DIG_IF_IREF_1_IREF_REG_EN_OFFSET 7
#define PLL_DIG_IF_IREF_1_IREF_REG_EN_SIZE 1
#define PLL_DIG_IF_IREF_1_IREF_REG_BYPASS_OFFSET 8
#define PLL_DIG_IF_IREF_1_IREF_REG_BYPASS_SIZE 1
#define PLL_DIG_IF_IREF_1_IREF_REG_FAST_LOCK_OFFSET 9
#define PLL_DIG_IF_IREF_1_IREF_REG_FAST_LOCK_SIZE 1
#define PLL_DIG_IF_IREF_1_IREF_REG_CTRL_OFFSET 10
#define PLL_DIG_IF_IREF_1_IREF_REG_CTRL_SIZE 4
#define PLL_DIG_IF_IREF_1_IREF_REG_ANATEST_OFFSET 14
#define PLL_DIG_IF_IREF_1_IREF_REG_ANATEST_SIZE 1
#define PLL_DIG_IF_IREF_1_TRIM_RESISTOR_OFFSET 15
#define PLL_DIG_IF_IREF_1_TRIM_RESISTOR_SIZE 4
#define PLL_DIG_IF_IREF_1_IREF_ENABLE_OFFSET 19
#define PLL_DIG_IF_IREF_1_IREF_ENABLE_SIZE 1
#define PLL_DIG_IF_IREF_1_BANDGAP_FAST_LOCK_OFFSET 20
#define PLL_DIG_IF_IREF_1_BANDGAP_FAST_LOCK_SIZE 1
#define PLL_DIG_IF_IREF_1_SYNTH_TRIM_10UA_OFFSET 21
#define PLL_DIG_IF_IREF_1_SYNTH_TRIM_10UA_SIZE 4

/** bit field defines for pll_dig_if_s#iref_2 */
#define PLL_DIG_IF_IREF_2_SYN_CTRL_0_TRIM_OFFSET 0
#define PLL_DIG_IF_IREF_2_SYN_CTRL_0_TRIM_SIZE 8
#define PLL_DIG_IF_IREF_2_SYN_CTRL_1_TRIM_OFFSET 8
#define PLL_DIG_IF_IREF_2_SYN_CTRL_1_TRIM_SIZE 8
#define PLL_DIG_IF_IREF_2_BB1_DAC_UNTRIM_CTRL_10U_I_OFFSET 16
#define PLL_DIG_IF_IREF_2_BB1_DAC_UNTRIM_CTRL_10U_I_SIZE 4
#define PLL_DIG_IF_IREF_2_BB1_DAC_UNTRIM_CTRL_10U_Q_OFFSET 20
#define PLL_DIG_IF_IREF_2_BB1_DAC_UNTRIM_CTRL_10U_Q_SIZE 4
#define PLL_DIG_IF_IREF_2_BB2_DAC_UNTRIM_CTRL_10U_I_OFFSET 24
#define PLL_DIG_IF_IREF_2_BB2_DAC_UNTRIM_CTRL_10U_I_SIZE 4
#define PLL_DIG_IF_IREF_2_BB2_DAC_UNTRIM_CTRL_10U_Q_OFFSET 28
#define PLL_DIG_IF_IREF_2_BB2_DAC_UNTRIM_CTRL_10U_Q_SIZE 4

/** bit field defines for pll_dig_if_s#iref_3 */
#define PLL_DIG_IF_IREF_3_RF1_UNTRIM_CTRL_0_OFFSET 0
#define PLL_DIG_IF_IREF_3_RF1_UNTRIM_CTRL_0_SIZE 1
#define PLL_DIG_IF_IREF_3_RF1_UNTRIM_CTRL_1_OFFSET 1
#define PLL_DIG_IF_IREF_3_RF1_UNTRIM_CTRL_1_SIZE 1
#define PLL_DIG_IF_IREF_3_RF2_UNTRIM_CTRL_0_OFFSET 2
#define PLL_DIG_IF_IREF_3_RF2_UNTRIM_CTRL_0_SIZE 1
#define PLL_DIG_IF_IREF_3_RF2_UNTRIM_CTRL_1_OFFSET 3
#define PLL_DIG_IF_IREF_3_RF2_UNTRIM_CTRL_1_SIZE 1
#define PLL_DIG_IF_IREF_3_AUX_UNTRIM_CTRL_0_OFFSET 4
#define PLL_DIG_IF_IREF_3_AUX_UNTRIM_CTRL_0_SIZE 5
#define PLL_DIG_IF_IREF_3_AUX_UNTRIM_CTRL_1_OFFSET 9
#define PLL_DIG_IF_IREF_3_AUX_UNTRIM_CTRL_1_SIZE 5
#define PLL_DIG_IF_IREF_3_BB1_ETDAC_UNTRIM_CTRL_10U_OFFSET 14
#define PLL_DIG_IF_IREF_3_BB1_ETDAC_UNTRIM_CTRL_10U_SIZE 4
#define PLL_DIG_IF_IREF_3_BB2_ETDAC_UNTRIM_CTRL_10U_OFFSET 18
#define PLL_DIG_IF_IREF_3_BB2_ETDAC_UNTRIM_CTRL_10U_SIZE 4
#define PLL_DIG_IF_IREF_3_AUX_TRIM_CTRL_0_10U_OFFSET 22
#define PLL_DIG_IF_IREF_3_AUX_TRIM_CTRL_0_10U_SIZE 4
#define PLL_DIG_IF_IREF_3_AUX_TRIM_CTRL_1_10U_OFFSET 26
#define PLL_DIG_IF_IREF_3_AUX_TRIM_CTRL_1_10U_SIZE 4

/** bit field defines for pll_dig_if_s#iref_4 */
#define PLL_DIG_IF_IREF_4_AUX_AFCDAC_UNTRIM_CTRL_10U_OFFSET 0
#define PLL_DIG_IF_IREF_4_AUX_AFCDAC_UNTRIM_CTRL_10U_SIZE 4

/** bit field defines for pll_dig_if_s#spare1 */
#define PLL_DIG_IF_SPARE1_SPR1_OFFSET 0
#define PLL_DIG_IF_SPARE1_SPR1_SIZE 32

/** bit field defines for pll_dig_if_s#spare2 */
#define PLL_DIG_IF_SPARE2_SPR2_OFFSET 0
#define PLL_DIG_IF_SPARE2_SPR2_SIZE 32

/** bit field defines for pll_dig_if_s#spare3 */
#define PLL_DIG_IF_SPARE3_SPR3_OFFSET 0
#define PLL_DIG_IF_SPARE3_SPR3_SIZE 32

/* EOF pll_dig_if.h */
#endif
