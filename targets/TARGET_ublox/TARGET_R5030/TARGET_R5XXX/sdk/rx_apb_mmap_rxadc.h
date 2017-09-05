#ifndef INCLUDED_RX_APB_MMAP_RXADC
#define INCLUDED_RX_APB_MMAP_RXADC
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** rx_apb_mmap_rxadc HAL Spreadsheet version number */
#define RX_APB_MMAP_RXADC_HAL_VERSION 203

/** This is the digital receiver path control register description
*/
struct rx_apb_mmap_rxadc_s {
   /** I-ADC Mode Control at address offset 0x000, read-write */
   uint32_t adc_mode_i;
   /** Q-ADC Mode Control at address offset 0x004, read-write */
   uint32_t adc_mode_q;
   /** ADC Offline Calibration Status at address offset 0x008, read-only */
   uint32_t adc_cal_status;
   /** I-ADC Calibration Clock Divider at address offset 0x00C, read-write */
   uint32_t adc_i_calclk;
   /** Q-ADC Calibration Clock Divider at address offset 0x010, read-write */
   uint32_t adc_q_calclk;
   /** I-ADC Dither Control for Calibration at address offset 0x014, read-write */
   uint32_t adc_i_dither;
   /** Q-ADC Dither Control for Calibration at address offset 0x018, read-write */
   uint32_t adc_q_dither;
   /** ADC IQ Mux Control at address offset 0x01C, read-write */
   uint32_t adc_iqmux;
   /** ADC Cal Readback Control at address offset 0x020, read-write */
   uint32_t adc_cal_readback_ctrl;
   /** I ADC Cal Readback Result at address offset 0x024, read-only */
   uint32_t adc_cal_readback_i;
   /** Q ADC Cal Readback Result at address offset 0x028, read-only */
   uint32_t adc_cal_readback_q;
   /** I-ADC Calibration Control at address offset 0x02C, read-write */
   uint32_t chi1_cal;
   /** I-ADC Calibration Stage Control at address offset 0x030, read-write */
   uint32_t chi1_calstagectrl;
   /** I-ADC Channel 1 - channel gain control at address offset 0x034, read-write */
   uint32_t chi1_channelgain;
   /** I-ADC Control Stage 1 Negative DAC in manual calibration mode. at address offset 0x038, read-write */
   uint32_t chi1_dacn1;
   /** I-ADC Control Stage 1 Positive DAC in manual calibration mode. at address offset 0x03C, read-write */
   uint32_t chi1_dacp1;
   /** I-ADC Control Stage 2 Negative DAC in manual calibration mode. at address offset 0x040, read-write */
   uint32_t chi1_dacn2;
   /** I-ADC Control Stage 2 Positive DAC in manual calibration mode. at address offset 0x044, read-write */
   uint32_t chi1_dacp2;
   /** I-ADC Amplifier Control at address offset 0x048, read-write */
   uint32_t chi1_amp;
   /** Adjust the size of the fixed DAC array. This is mostly redundant. at address offset 0x04C, read-write */
   uint32_t chi1_msbcal;
   /** I-ADC Comparator 12 (MSB) Positive Offset Control at address offset 0x050, read-write */
   uint32_t chi1_comp12calp;
   /** I-ADC Comparator 12 (MSB) Negative Offset Control at address offset 0x054, read-write */
   uint32_t chi1_comp12caln;
   /** I-ADC Comparator 11 Positive Offset Control at address offset 0x058, read-write */
   uint32_t chi1_comp11calp;
   /** I-ADC Comparator 11 Negative Offset Control at address offset 0x05C, read-write */
   uint32_t chi1_comp11caln;
   /** I-ADC Comparator 10 Positive Offset Control at address offset 0x060, read-write */
   uint32_t chi1_comp10calp;
   /** I-ADC Comparator 10 Negative Offset Control at address offset 0x064, read-write */
   uint32_t chi1_comp10caln;
   /** I-ADC Comparator 9 Positive Offset Control at address offset 0x068, read-write */
   uint32_t chi1_comp9calp;
   /** I-ADC Comparator 9 Negative Offset Control at address offset 0x06C, read-write */
   uint32_t chi1_comp9caln;
   /** I-ADC Comparator 8 Positive Offset Control at address offset 0x070, read-write */
   uint32_t chi1_comp8calp;
   /** I-ADC Comparator 8 Negative Offset Control at address offset 0x074, read-write */
   uint32_t chi1_comp8caln;
   /** I-ADC Comparator 7 Positive Offset Control at address offset 0x078, read-write */
   uint32_t chi1_comp7calp;
   /** I-ADC Comparator 7 Negative Offset Control at address offset 0x07C, read-write */
   uint32_t chi1_comp7caln;
   /** I-ADC Comparator 6 Positive Offset Control at address offset 0x080, read-write */
   uint32_t chi1_comp6calp;
   /** I-ADC Comparator 6 Negative Offset Control at address offset 0x084, read-write */
   uint32_t chi1_comp6caln;
   /** I-ADC Comparator 5 Positive Offset Control at address offset 0x088, read-write */
   uint32_t chi1_comp5calp;
   /** I-ADC Comparator 5 Negative Offset Control at address offset 0x08C, read-write */
   uint32_t chi1_comp5caln;
   /** I-ADC Comparator 4 Positive Offset Control at address offset 0x090, read-write */
   uint32_t chi1_comp4calp;
   /** I-ADC Comparator 4 Negative Offset Control at address offset 0x094, read-write */
   uint32_t chi1_comp4caln;
   /** I-ADC Comparator 3 Positive Offset Control at address offset 0x098, read-write */
   uint32_t chi1_comp3calp;
   /** I-ADC Comparator 3 Negative Offset Control at address offset 0x09C, read-write */
   uint32_t chi1_comp3caln;
   /** I-ADC Comparator 2 Positive Offset Control at address offset 0x0A0, read-write */
   uint32_t chi1_comp2calp;
   /** I-ADC Comparator 2 Negative Offset Control at address offset 0x0A4, read-write */
   uint32_t chi1_comp2caln;
   /** I-ADC Comparator 1 Positive Offset Control at address offset 0x0A8, read-write */
   uint32_t chi1_comp1calp;
   /** I-ADC Comparator 1 Negative Offset Control at address offset 0x0AC, read-write */
   uint32_t chi1_comp1caln;
   /** I-ADC Comparator 0 (LSB) Positive Offset Control at address offset 0x0B0, read-write */
   uint32_t chi1_comp0calp;
   /** I-ADC Comparator 0 (LSB) Negative Offset Control at address offset 0x0B4, read-write */
   uint32_t chi1_comp0caln;
   /** I-ADC Comparator Clock Delay Control at address offset 0x0B8, read-write */
   uint32_t chi1_compdelay;
   /** I-ADC conversion fail flags at address offset 0x0BC, read-only */
   uint32_t chi1_fail_detect;
   /** Spare Bits at address offset 0x0C0, read-write */
   uint32_t chi1_spare;
   /** Calibration State Machine Counters at address offset 0x0C4, read-write */
   uint32_t chi1_n_values;
   /** I-ADC Comparator 12 (MSB) Offset Error Threshold at address offset 0x0C8, read-write */
   uint32_t chi1_threshold12;
   /** I-ADC Comparator 11 Offset Error Threshold at address offset 0x0CC, read-write */
   uint32_t chi1_threshold11;
   /** I-ADC Comparator 10 Offset Error Threshold at address offset 0x0D0, read-write */
   uint32_t chi1_threshold10;
   /** I-ADC Comparator 9 Offset Error Threshold at address offset 0x0D4, read-write */
   uint32_t chi1_threshold9;
   /** I-ADC Comparator 8 Offset Error Threshold at address offset 0x0D8, read-write */
   uint32_t chi1_threshold8;
   /** I-ADC Comparator 7 Offset Error Threshold at address offset 0x0DC, read-write */
   uint32_t chi1_threshold7;
   /** I-ADC Comparator 6 Offset Error Threshold at address offset 0x0E0, read-write */
   uint32_t chi1_threshold6;
   /** I-ADC Comparator 5 Offset Error Threshold at address offset 0x0E4, read-write */
   uint32_t chi1_threshold5;
   /** I-ADC Comparator 4 Offset Error Threshold at address offset 0x0E8, read-write */
   uint32_t chi1_threshold4;
   /** I-ADC Comparator 3 Offset Error Threshold at address offset 0x0EC, read-write */
   uint32_t chi1_threshold3;
   /** I-ADC Comparator 2 Offset Error Threshold at address offset 0x0F0, read-write */
   uint32_t chi1_threshold2;
   /** I-ADC Calibration Control at address offset 0x0F4, read-write */
   uint32_t chi2_cal;
   /** I-ADC Calibration Stage Control at address offset 0x0F8, read-write */
   uint32_t chi2_calstagectrl;
   /** I-ADC Channel 2 - channel gain control at address offset 0x0FC, read-write */
   uint32_t chi2_channelgain;
   /** I-ADC Control Stage 1 Negative DAC in manual calibration mode. at address offset 0x100, read-write */
   uint32_t chi2_dacn1;
   /** I-ADC Control Stage 1 Positive DAC in manual calibration mode. at address offset 0x104, read-write */
   uint32_t chi2_dacp1;
   /** I-ADC Control Stage 2 Negative DAC in manual calibration mode. at address offset 0x108, read-write */
   uint32_t chi2_dacn2;
   /** I-ADC Control Stage 2 Positive DAC in manual calibration mode. at address offset 0x10C, read-write */
   uint32_t chi2_dacp2;
   /** I-ADC Amplifier Control at address offset 0x110, read-write */
   uint32_t chi2_amp;
   /** Adjust the size of the fixed DAC array. This is mostly redundant. at address offset 0x114, read-write */
   uint32_t chi2_msbcal;
   /** I-ADC Comparator 12 (MSB) Positive Offset Control at address offset 0x118, read-write */
   uint32_t chi2_comp12calp;
   /** I-ADC Comparator 12 (MSB) Negative Offset Control at address offset 0x11C, read-write */
   uint32_t chi2_comp12caln;
   /** I-ADC Comparator 11 Positive Offset Control at address offset 0x120, read-write */
   uint32_t chi2_comp11calp;
   /** I-ADC Comparator 11 Negative Offset Control at address offset 0x124, read-write */
   uint32_t chi2_comp11caln;
   /** I-ADC Comparator 10 Positive Offset Control at address offset 0x128, read-write */
   uint32_t chi2_comp10calp;
   /** I-ADC Comparator 10 Negative Offset Control at address offset 0x12C, read-write */
   uint32_t chi2_comp10caln;
   /** I-ADC Comparator 9 Positive Offset Control at address offset 0x130, read-write */
   uint32_t chi2_comp9calp;
   /** I-ADC Comparator 9 Negative Offset Control at address offset 0x134, read-write */
   uint32_t chi2_comp9caln;
   /** I-ADC Comparator 8 Positive Offset Control at address offset 0x138, read-write */
   uint32_t chi2_comp8calp;
   /** I-ADC Comparator 8 Negative Offset Control at address offset 0x13C, read-write */
   uint32_t chi2_comp8caln;
   /** I-ADC Comparator 7 Positive Offset Control at address offset 0x140, read-write */
   uint32_t chi2_comp7calp;
   /** I-ADC Comparator 7 Negative Offset Control at address offset 0x144, read-write */
   uint32_t chi2_comp7caln;
   /** I-ADC Comparator 6 Positive Offset Control at address offset 0x148, read-write */
   uint32_t chi2_comp6calp;
   /** I-ADC Comparator 6 Negative Offset Control at address offset 0x14C, read-write */
   uint32_t chi2_comp6caln;
   /** I-ADC Comparator 5 Positive Offset Control at address offset 0x150, read-write */
   uint32_t chi2_comp5calp;
   /** I-ADC Comparator 5 Negative Offset Control at address offset 0x154, read-write */
   uint32_t chi2_comp5caln;
   /** I-ADC Comparator 4 Positive Offset Control at address offset 0x158, read-write */
   uint32_t chi2_comp4calp;
   /** I-ADC Comparator 4 Negative Offset Control at address offset 0x15C, read-write */
   uint32_t chi2_comp4caln;
   /** I-ADC Comparator 3 Positive Offset Control at address offset 0x160, read-write */
   uint32_t chi2_comp3calp;
   /** I-ADC Comparator 3 Negative Offset Control at address offset 0x164, read-write */
   uint32_t chi2_comp3caln;
   /** I-ADC Comparator 2 Positive Offset Control at address offset 0x168, read-write */
   uint32_t chi2_comp2calp;
   /** I-ADC Comparator 2 Negative Offset Control at address offset 0x16C, read-write */
   uint32_t chi2_comp2caln;
   /** I-ADC Comparator 1 Positive Offset Control at address offset 0x170, read-write */
   uint32_t chi2_comp1calp;
   /** I-ADC Comparator 1 Negative Offset Control at address offset 0x174, read-write */
   uint32_t chi2_comp1caln;
   /** I-ADC Comparator 0 (LSB) Positive Offset Control at address offset 0x178, read-write */
   uint32_t chi2_comp0calp;
   /** I-ADC Comparator 0 (LSB) Negative Offset Control at address offset 0x17C, read-write */
   uint32_t chi2_comp0caln;
   /** I-ADC Comparator Clock Delay Control at address offset 0x180, read-write */
   uint32_t chi2_compdelay;
   /** I-ADC conversion fail flags at address offset 0x184, read-only */
   uint32_t chi2_fail_detect;
   /** Spare Bits at address offset 0x188, read-write */
   uint32_t chi2_spare;
   /** Calibration State Machine Counters at address offset 0x18C, read-write */
   uint32_t chi2_n_values;
   /** I-ADC Comparator 12 (MSB) Offset Error Threshold at address offset 0x190, read-write */
   uint32_t chi2_threshold12;
   /** I-ADC Comparator 11 Offset Error Threshold at address offset 0x194, read-write */
   uint32_t chi2_threshold11;
   /** I-ADC Comparator 10 Offset Error Threshold at address offset 0x198, read-write */
   uint32_t chi2_threshold10;
   /** I-ADC Comparator 9 Offset Error Threshold at address offset 0x19C, read-write */
   uint32_t chi2_threshold9;
   /** I-ADC Comparator 8 Offset Error Threshold at address offset 0x1A0, read-write */
   uint32_t chi2_threshold8;
   /** I-ADC Comparator 7 Offset Error Threshold at address offset 0x1A4, read-write */
   uint32_t chi2_threshold7;
   /** I-ADC Comparator 6 Offset Error Threshold at address offset 0x1A8, read-write */
   uint32_t chi2_threshold6;
   /** I-ADC Comparator 5 Offset Error Threshold at address offset 0x1AC, read-write */
   uint32_t chi2_threshold5;
   /** I-ADC Comparator 4 Offset Error Threshold at address offset 0x1B0, read-write */
   uint32_t chi2_threshold4;
   /** I-ADC Comparator 3 Offset Error Threshold at address offset 0x1B4, read-write */
   uint32_t chi2_threshold3;
   /** I-ADC Comparator 2 Offset Error Threshold at address offset 0x1B8, read-write */
   uint32_t chi2_threshold2;
   /** Q-ADC Calibration Control at address offset 0x1BC, read-write */
   uint32_t chq1_cal;
   /** Q-ADC Calibration Stage Control at address offset 0x1C0, read-write */
   uint32_t chq1_calstagectrl;
   /** Q-ADC Channel 1 - channel gain control at address offset 0x1C4, read-write */
   uint32_t chq1_channelgain;
   /** Q-ADC Control Stage 1 Negative DAC in manual calibration mode. at address offset 0x1C8, read-write */
   uint32_t chq1_dacn1;
   /** Q-ADC Control Stage 1 Positive DAC in manual calibration mode. at address offset 0x1CC, read-write */
   uint32_t chq1_dacp1;
   /** Q-ADC Control Stage 2 Negative DAC in manual calibration mode. at address offset 0x1D0, read-write */
   uint32_t chq1_dacn2;
   /** Q-ADC Control Stage 2 Positive DAC in manual calibration mode. at address offset 0x1D4, read-write */
   uint32_t chq1_dacp2;
   /** Q-ADC Amplifier Control at address offset 0x1D8, read-write */
   uint32_t chq1_amp;
   /** Adjust the size of the fixed DAC array. This is mostly redundant. at address offset 0x1DC, read-write */
   uint32_t chq1_msbcal;
   /** Q-ADC Comparator 12 (MSB) Positive Offset Control at address offset 0x1E0, read-write */
   uint32_t chq1_comp12calp;
   /** Q-ADC Comparator 12 (MSB) Negative Offset Control at address offset 0x1E4, read-write */
   uint32_t chq1_comp12caln;
   /** Q-ADC Comparator 11 Positive Offset Control at address offset 0x1E8, read-write */
   uint32_t chq1_comp11calp;
   /** Q-ADC Comparator 11 Negative Offset Control at address offset 0x1EC, read-write */
   uint32_t chq1_comp11caln;
   /** Q-ADC Comparator 10 Positive Offset Control at address offset 0x1F0, read-write */
   uint32_t chq1_comp10calp;
   /** Q-ADC Comparator 10 Negative Offset Control at address offset 0x1F4, read-write */
   uint32_t chq1_comp10caln;
   /** Q-ADC Comparator 9 Positive Offset Control at address offset 0x1F8, read-write */
   uint32_t chq1_comp9calp;
   /** Q-ADC Comparator 9 Negative Offset Control at address offset 0x1FC, read-write */
   uint32_t chq1_comp9caln;
   /** Q-ADC Comparator 8 Positive Offset Control at address offset 0x200, read-write */
   uint32_t chq1_comp8calp;
   /** Q-ADC Comparator 8 Negative Offset Control at address offset 0x204, read-write */
   uint32_t chq1_comp8caln;
   /** Q-ADC Comparator 7 Positive Offset Control at address offset 0x208, read-write */
   uint32_t chq1_comp7calp;
   /** Q-ADC Comparator 7 Negative Offset Control at address offset 0x20C, read-write */
   uint32_t chq1_comp7caln;
   /** Q-ADC Comparator 6 Positive Offset Control at address offset 0x210, read-write */
   uint32_t chq1_comp6calp;
   /** Q-ADC Comparator 6 Negative Offset Control at address offset 0x214, read-write */
   uint32_t chq1_comp6caln;
   /** Q-ADC Comparator 5 Positive Offset Control at address offset 0x218, read-write */
   uint32_t chq1_comp5calp;
   /** Q-ADC Comparator 5 Negative Offset Control at address offset 0x21C, read-write */
   uint32_t chq1_comp5caln;
   /** Q-ADC Comparator 4 Positive Offset Control at address offset 0x220, read-write */
   uint32_t chq1_comp4calp;
   /** Q-ADC Comparator 4 Negative Offset Control at address offset 0x224, read-write */
   uint32_t chq1_comp4caln;
   /** Q-ADC Comparator 3 Positive Offset Control at address offset 0x228, read-write */
   uint32_t chq1_comp3calp;
   /** Q-ADC Comparator 3 Negative Offset Control at address offset 0x22C, read-write */
   uint32_t chq1_comp3caln;
   /** Q-ADC Comparator 2 Positive Offset Control at address offset 0x230, read-write */
   uint32_t chq1_comp2calp;
   /** Q-ADC Comparator 2 Negative Offset Control at address offset 0x234, read-write */
   uint32_t chq1_comp2caln;
   /** Q-ADC Comparator 1 Positive Offset Control at address offset 0x238, read-write */
   uint32_t chq1_comp1calp;
   /** Q-ADC Comparator 1 Negative Offset Control at address offset 0x23C, read-write */
   uint32_t chq1_comp1caln;
   /** Q-ADC Comparator 0 (LSB) Positive Offset Control at address offset 0x240, read-write */
   uint32_t chq1_comp0calp;
   /** Q-ADC Comparator 0 (LSB) Negative Offset Control at address offset 0x244, read-write */
   uint32_t chq1_comp0caln;
   /** Q-ADC Comparator Clock Delay Control at address offset 0x248, read-write */
   uint32_t chq1_compdelay;
   /** Q-ADC conversion fail flags at address offset 0x24C, read-only */
   uint32_t chq1_fail_detect;
   /** Spare Bits at address offset 0x250, read-write */
   uint32_t chq1_spare;
   /** Calibration State MaCHQne Counters at address offset 0x254, read-write */
   uint32_t chq1_n_values;
   /** Q-ADC Comparator 12 (MSB) Offset Error Threshold at address offset 0x258, read-write */
   uint32_t chq1_threshold12;
   /** Q-ADC Comparator 11 Offset Error Threshold at address offset 0x25C, read-write */
   uint32_t chq1_threshold11;
   /** Q-ADC Comparator 10 Offset Error Threshold at address offset 0x260, read-write */
   uint32_t chq1_threshold10;
   /** Q-ADC Comparator 9 Offset Error Threshold at address offset 0x264, read-write */
   uint32_t chq1_threshold9;
   /** Q-ADC Comparator 8 Offset Error Threshold at address offset 0x268, read-write */
   uint32_t chq1_threshold8;
   /** Q-ADC Comparator 7 Offset Error Threshold at address offset 0x26C, read-write */
   uint32_t chq1_threshold7;
   /** Q-ADC Comparator 6 Offset Error Threshold at address offset 0x270, read-write */
   uint32_t chq1_threshold6;
   /** Q-ADC Comparator 5 Offset Error Threshold at address offset 0x274, read-write */
   uint32_t chq1_threshold5;
   /** Q-ADC Comparator 4 Offset Error Threshold at address offset 0x278, read-write */
   uint32_t chq1_threshold4;
   /** Q-ADC Comparator 3 Offset Error Threshold at address offset 0x27C, read-write */
   uint32_t chq1_threshold3;
   /** Q-ADC Comparator 2 Offset Error Threshold at address offset 0x280, read-write */
   uint32_t chq1_threshold2;
   /** Q-ADC Calibration Control at address offset 0x284, read-write */
   uint32_t chq2_cal;
   /** Q-ADC Calibration Stage Control at address offset 0x288, read-write */
   uint32_t chq2_calstagectrl;
   /** Q-ADC Channel 2 - channel gain control at address offset 0x28C, read-write */
   uint32_t chq2_channelgain;
   /** Q-ADC Control Stage 1 Negative DAC in manual calibration mode. at address offset 0x290, read-write */
   uint32_t chq2_dacn1;
   /** Q-ADC Control Stage 1 Positive DAC in manual calibration mode. at address offset 0x294, read-write */
   uint32_t chq2_dacp1;
   /** Q-ADC Control Stage 2 Negative DAC in manual calibration mode. at address offset 0x298, read-write */
   uint32_t chq2_dacn2;
   /** Q-ADC Control Stage 2 Positive DAC in manual calibration mode. at address offset 0x29C, read-write */
   uint32_t chq2_dacp2;
   /** Q-ADC Amplifier Control at address offset 0x2A0, read-write */
   uint32_t chq2_amp;
   /** Adjust the size of the fixed DAC array. This is mostly redundant. at address offset 0x2A4, read-write */
   uint32_t chq2_msbcal;
   /** Q-ADC Comparator 12 (MSB) Positive Offset Control at address offset 0x2A8, read-write */
   uint32_t chq2_comp12calp;
   /** Q-ADC Comparator 12 (MSB) Negative Offset Control at address offset 0x2AC, read-write */
   uint32_t chq2_comp12caln;
   /** Q-ADC Comparator 11 Positive Offset Control at address offset 0x2B0, read-write */
   uint32_t chq2_comp11calp;
   /** Q-ADC Comparator 11 Negative Offset Control at address offset 0x2B4, read-write */
   uint32_t chq2_comp11caln;
   /** Q-ADC Comparator 10 Positive Offset Control at address offset 0x2B8, read-write */
   uint32_t chq2_comp10calp;
   /** Q-ADC Comparator 10 Negative Offset Control at address offset 0x2BC, read-write */
   uint32_t chq2_comp10caln;
   /** Q-ADC Comparator 9 Positive Offset Control at address offset 0x2C0, read-write */
   uint32_t chq2_comp9calp;
   /** Q-ADC Comparator 9 Negative Offset Control at address offset 0x2C4, read-write */
   uint32_t chq2_comp9caln;
   /** Q-ADC Comparator 8 Positive Offset Control at address offset 0x2C8, read-write */
   uint32_t chq2_comp8calp;
   /** Q-ADC Comparator 8 Negative Offset Control at address offset 0x2CC, read-write */
   uint32_t chq2_comp8caln;
   /** Q-ADC Comparator 7 Positive Offset Control at address offset 0x2D0, read-write */
   uint32_t chq2_comp7calp;
   /** Q-ADC Comparator 7 Negative Offset Control at address offset 0x2D4, read-write */
   uint32_t chq2_comp7caln;
   /** Q-ADC Comparator 6 Positive Offset Control at address offset 0x2D8, read-write */
   uint32_t chq2_comp6calp;
   /** Q-ADC Comparator 6 Negative Offset Control at address offset 0x2DC, read-write */
   uint32_t chq2_comp6caln;
   /** Q-ADC Comparator 5 Positive Offset Control at address offset 0x2E0, read-write */
   uint32_t chq2_comp5calp;
   /** Q-ADC Comparator 5 Negative Offset Control at address offset 0x2E4, read-write */
   uint32_t chq2_comp5caln;
   /** Q-ADC Comparator 4 Positive Offset Control at address offset 0x2E8, read-write */
   uint32_t chq2_comp4calp;
   /** Q-ADC Comparator 4 Negative Offset Control at address offset 0x2EC, read-write */
   uint32_t chq2_comp4caln;
   /** Q-ADC Comparator 3 Positive Offset Control at address offset 0x2F0, read-write */
   uint32_t chq2_comp3calp;
   /** Q-ADC Comparator 3 Negative Offset Control at address offset 0x2F4, read-write */
   uint32_t chq2_comp3caln;
   /** Q-ADC Comparator 2 Positive Offset Control at address offset 0x2F8, read-write */
   uint32_t chq2_comp2calp;
   /** Q-ADC Comparator 2 Negative Offset Control at address offset 0x2FC, read-write */
   uint32_t chq2_comp2caln;
   /** Q-ADC Comparator 1 Positive Offset Control at address offset 0x300, read-write */
   uint32_t chq2_comp1calp;
   /** Q-ADC Comparator 1 Negative Offset Control at address offset 0x304, read-write */
   uint32_t chq2_comp1caln;
   /** Q-ADC Comparator 0 (LSB) Positive Offset Control at address offset 0x308, read-write */
   uint32_t chq2_comp0calp;
   /** Q-ADC Comparator 0 (LSB) Negative Offset Control at address offset 0x30C, read-write */
   uint32_t chq2_comp0caln;
   /** Q-ADC Comparator Clock Delay Control at address offset 0x310, read-write */
   uint32_t chq2_compdelay;
   /** Q-ADC conversion fail flags at address offset 0x314, read-only */
   uint32_t chq2_fail_detect;
   /** Spare Bits at address offset 0x318, read-write */
   uint32_t chq2_spare;
   /** Calibration State MaCHQne Counters at address offset 0x31C, read-write */
   uint32_t chq2_n_values;
   /** Q-ADC Comparator 12 (MSB) Offset Error Threshold at address offset 0x320, read-write */
   uint32_t chq2_threshold12;
   /** Q-ADC Comparator 11 Offset Error Threshold at address offset 0x324, read-write */
   uint32_t chq2_threshold11;
   /** Q-ADC Comparator 10 Offset Error Threshold at address offset 0x328, read-write */
   uint32_t chq2_threshold10;
   /** Q-ADC Comparator 9 Offset Error Threshold at address offset 0x32C, read-write */
   uint32_t chq2_threshold9;
   /** Q-ADC Comparator 8 Offset Error Threshold at address offset 0x330, read-write */
   uint32_t chq2_threshold8;
   /** Q-ADC Comparator 7 Offset Error Threshold at address offset 0x334, read-write */
   uint32_t chq2_threshold7;
   /** Q-ADC Comparator 6 Offset Error Threshold at address offset 0x338, read-write */
   uint32_t chq2_threshold6;
   /** Q-ADC Comparator 5 Offset Error Threshold at address offset 0x33C, read-write */
   uint32_t chq2_threshold5;
   /** Q-ADC Comparator 4 Offset Error Threshold at address offset 0x340, read-write */
   uint32_t chq2_threshold4;
   /** Q-ADC Comparator 3 Offset Error Threshold at address offset 0x344, read-write */
   uint32_t chq2_threshold3;
   /** Q-ADC Comparator 2 Offset Error Threshold at address offset 0x348, read-write */
   uint32_t chq2_threshold2;
   /** Debug register for testing the loading of data from the shadow to config registers. at address offset 0x34C, read-write */
   uint32_t dbg_rxadc;
};

/** bit field defines for rx_apb_mmap_rxadc_s#adc_mode_i */
#define RX_APB_MMAP_RXADC_ADC_MODE_I_ADCMODE_OFFSET 0
#define RX_APB_MMAP_RXADC_ADC_MODE_I_ADCMODE_SIZE 2
/**  */
#define RX_APB_MMAP_RXADC_ADC_MODE_I_ADCMODE_MAN_VALUE 0
/**  */
#define RX_APB_MMAP_RXADC_ADC_MODE_I_ADCMODE_OFFLINE_VALUE 1
/**  */
#define RX_APB_MMAP_RXADC_ADC_MODE_I_ADCMODE_ONLINE_VALUE 2
#define RX_APB_MMAP_RXADC_ADC_MODE_I_INTERLEAVEMODE_OFFSET 2
#define RX_APB_MMAP_RXADC_ADC_MODE_I_INTERLEAVEMODE_SIZE 1
/**  */
#define RX_APB_MMAP_RXADC_ADC_MODE_I_INTERLEAVEMODE_OFF_VALUE 0
/**  */
#define RX_APB_MMAP_RXADC_ADC_MODE_I_INTERLEAVEMODE_ON_VALUE 1
#define RX_APB_MMAP_RXADC_ADC_MODE_I_CH1EN_OFFSET 3
#define RX_APB_MMAP_RXADC_ADC_MODE_I_CH1EN_SIZE 1
#define RX_APB_MMAP_RXADC_ADC_MODE_I_CH2EN_OFFSET 4
#define RX_APB_MMAP_RXADC_ADC_MODE_I_CH2EN_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#adc_mode_q */
#define RX_APB_MMAP_RXADC_ADC_MODE_Q_ADCMODE_OFFSET 0
#define RX_APB_MMAP_RXADC_ADC_MODE_Q_ADCMODE_SIZE 2
/**  */
#define RX_APB_MMAP_RXADC_ADC_MODE_Q_ADCMODE_MAN_VALUE 0
/**  */
#define RX_APB_MMAP_RXADC_ADC_MODE_Q_ADCMODE_OFFLINE_VALUE 1
/**  */
#define RX_APB_MMAP_RXADC_ADC_MODE_Q_ADCMODE_ONLINE_VALUE 2
#define RX_APB_MMAP_RXADC_ADC_MODE_Q_INTERLEAVEMODE_OFFSET 2
#define RX_APB_MMAP_RXADC_ADC_MODE_Q_INTERLEAVEMODE_SIZE 1
/**  */
#define RX_APB_MMAP_RXADC_ADC_MODE_Q_INTERLEAVEMODE_OFF_VALUE 0
/**  */
#define RX_APB_MMAP_RXADC_ADC_MODE_Q_INTERLEAVEMODE_ON_VALUE 1
#define RX_APB_MMAP_RXADC_ADC_MODE_Q_CH1EN_OFFSET 3
#define RX_APB_MMAP_RXADC_ADC_MODE_Q_CH1EN_SIZE 1
#define RX_APB_MMAP_RXADC_ADC_MODE_Q_CH2EN_OFFSET 4
#define RX_APB_MMAP_RXADC_ADC_MODE_Q_CH2EN_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#adc_cal_status */
#define RX_APB_MMAP_RXADC_ADC_CAL_STATUS_CHI1OFFLINECALDONE_OFFSET 0
#define RX_APB_MMAP_RXADC_ADC_CAL_STATUS_CHI1OFFLINECALDONE_SIZE 1
#define RX_APB_MMAP_RXADC_ADC_CAL_STATUS_CHI2OFFLINECALDONE_OFFSET 1
#define RX_APB_MMAP_RXADC_ADC_CAL_STATUS_CHI2OFFLINECALDONE_SIZE 1
#define RX_APB_MMAP_RXADC_ADC_CAL_STATUS_CHQ1OFFLINECALDONE_OFFSET 2
#define RX_APB_MMAP_RXADC_ADC_CAL_STATUS_CHQ1OFFLINECALDONE_SIZE 1
#define RX_APB_MMAP_RXADC_ADC_CAL_STATUS_CHQ2OFFLINECALDONE_OFFSET 3
#define RX_APB_MMAP_RXADC_ADC_CAL_STATUS_CHQ2OFFLINECALDONE_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#adc_i_calclk */
#define RX_APB_MMAP_RXADC_ADC_I_CALCLK_CALCLKDIV_OFFSET 0
#define RX_APB_MMAP_RXADC_ADC_I_CALCLK_CALCLKDIV_SIZE 4

/** bit field defines for rx_apb_mmap_rxadc_s#adc_q_calclk */
#define RX_APB_MMAP_RXADC_ADC_Q_CALCLK_CALCLKDIV_OFFSET 0
#define RX_APB_MMAP_RXADC_ADC_Q_CALCLK_CALCLKDIV_SIZE 4

/** bit field defines for rx_apb_mmap_rxadc_s#adc_i_dither */
#define RX_APB_MMAP_RXADC_ADC_I_DITHER_DITHEREN_OFFSET 0
#define RX_APB_MMAP_RXADC_ADC_I_DITHER_DITHEREN_SIZE 1
#define RX_APB_MMAP_RXADC_ADC_I_DITHER_DITHERREVERT_OFFSET 1
#define RX_APB_MMAP_RXADC_ADC_I_DITHER_DITHERREVERT_SIZE 1
#define RX_APB_MMAP_RXADC_ADC_I_DITHER_DITHERDELAY_OFFSET 2
#define RX_APB_MMAP_RXADC_ADC_I_DITHER_DITHERDELAY_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#adc_q_dither */
#define RX_APB_MMAP_RXADC_ADC_Q_DITHER_DITHEREN_OFFSET 0
#define RX_APB_MMAP_RXADC_ADC_Q_DITHER_DITHEREN_SIZE 1
#define RX_APB_MMAP_RXADC_ADC_Q_DITHER_DITHERREVERT_OFFSET 1
#define RX_APB_MMAP_RXADC_ADC_Q_DITHER_DITHERREVERT_SIZE 1
#define RX_APB_MMAP_RXADC_ADC_Q_DITHER_DITHERDELAY_OFFSET 2
#define RX_APB_MMAP_RXADC_ADC_Q_DITHER_DITHERDELAY_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#adc_iqmux */
#define RX_APB_MMAP_RXADC_ADC_IQMUX_EN_I_OFFSET 0
#define RX_APB_MMAP_RXADC_ADC_IQMUX_EN_I_SIZE 1
#define RX_APB_MMAP_RXADC_ADC_IQMUX_EN_Q_OFFSET 1
#define RX_APB_MMAP_RXADC_ADC_IQMUX_EN_Q_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#adc_cal_readback_ctrl */
#define RX_APB_MMAP_RXADC_ADC_CAL_READBACK_CTRL_SELECTI_OFFSET 0
#define RX_APB_MMAP_RXADC_ADC_CAL_READBACK_CTRL_SELECTI_SIZE 5
#define RX_APB_MMAP_RXADC_ADC_CAL_READBACK_CTRL_SELECTQ_OFFSET 5
#define RX_APB_MMAP_RXADC_ADC_CAL_READBACK_CTRL_SELECTQ_SIZE 5

/** bit field defines for rx_apb_mmap_rxadc_s#adc_cal_readback_i */
#define RX_APB_MMAP_RXADC_ADC_CAL_READBACK_I_RESULT_OFFSET 0
#define RX_APB_MMAP_RXADC_ADC_CAL_READBACK_I_RESULT_SIZE 32

/** bit field defines for rx_apb_mmap_rxadc_s#adc_cal_readback_q */
#define RX_APB_MMAP_RXADC_ADC_CAL_READBACK_Q_RESULT_OFFSET 0
#define RX_APB_MMAP_RXADC_ADC_CAL_READBACK_Q_RESULT_SIZE 32

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_cal */
#define RX_APB_MMAP_RXADC_CHI1_CAL_CH1CALMODE_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_CAL_CH1CALMODE_SIZE 2
#define RX_APB_MMAP_RXADC_CHI1_CAL_SHORTADCINPUT_OFFSET 2
#define RX_APB_MMAP_RXADC_CHI1_CAL_SHORTADCINPUT_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_calstagectrl */
#define RX_APB_MMAP_RXADC_CHI1_CALSTAGECTRL_STAGEDISABLE_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_CALSTAGECTRL_STAGEDISABLE_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_channelgain */
#define RX_APB_MMAP_RXADC_CHI1_CHANNELGAIN_CH1CHANNELGAIN_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_CHANNELGAIN_CH1CHANNELGAIN_SIZE 5

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_dacn1 */
#define RX_APB_MMAP_RXADC_CHI1_DACN1_CH1CAPCONTROLN1_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_DACN1_CH1CAPCONTROLN1_SIZE 9

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_dacp1 */
#define RX_APB_MMAP_RXADC_CHI1_DACP1_CH1CAPCONTROLP1_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_DACP1_CH1CAPCONTROLP1_SIZE 9

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_dacn2 */
#define RX_APB_MMAP_RXADC_CHI1_DACN2_CH1CAPCONTROLN2_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_DACN2_CH1CAPCONTROLN2_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_dacp2 */
#define RX_APB_MMAP_RXADC_CHI1_DACP2_CH1CAPCONTROLP2_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_DACP2_CH1CAPCONTROLP2_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_amp */
#define RX_APB_MMAP_RXADC_CHI1_AMP_CH1AMPGAINCALP_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_AMP_CH1AMPGAINCALP_SIZE 7
#define RX_APB_MMAP_RXADC_CHI1_AMP_CH1AMPGAINCALN_OFFSET 7
#define RX_APB_MMAP_RXADC_CHI1_AMP_CH1AMPGAINCALN_SIZE 7
#define RX_APB_MMAP_RXADC_CHI1_AMP_CH1INCREASEAMPDELAY_OFFSET 14
#define RX_APB_MMAP_RXADC_CHI1_AMP_CH1INCREASEAMPDELAY_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_msbcal */
#define RX_APB_MMAP_RXADC_CHI1_MSBCAL_CH1MSBCAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_MSBCAL_CH1MSBCAL_SIZE 10

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp12calp */
#define RX_APB_MMAP_RXADC_CHI1_COMP12CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP12CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp12caln */
#define RX_APB_MMAP_RXADC_CHI1_COMP12CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP12CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp11calp */
#define RX_APB_MMAP_RXADC_CHI1_COMP11CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP11CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp11caln */
#define RX_APB_MMAP_RXADC_CHI1_COMP11CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP11CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp10calp */
#define RX_APB_MMAP_RXADC_CHI1_COMP10CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP10CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp10caln */
#define RX_APB_MMAP_RXADC_CHI1_COMP10CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP10CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp9calp */
#define RX_APB_MMAP_RXADC_CHI1_COMP9CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP9CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp9caln */
#define RX_APB_MMAP_RXADC_CHI1_COMP9CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP9CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp8calp */
#define RX_APB_MMAP_RXADC_CHI1_COMP8CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP8CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp8caln */
#define RX_APB_MMAP_RXADC_CHI1_COMP8CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP8CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp7calp */
#define RX_APB_MMAP_RXADC_CHI1_COMP7CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP7CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp7caln */
#define RX_APB_MMAP_RXADC_CHI1_COMP7CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP7CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp6calp */
#define RX_APB_MMAP_RXADC_CHI1_COMP6CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP6CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp6caln */
#define RX_APB_MMAP_RXADC_CHI1_COMP6CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP6CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp5calp */
#define RX_APB_MMAP_RXADC_CHI1_COMP5CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP5CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp5caln */
#define RX_APB_MMAP_RXADC_CHI1_COMP5CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP5CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp4calp */
#define RX_APB_MMAP_RXADC_CHI1_COMP4CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP4CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp4caln */
#define RX_APB_MMAP_RXADC_CHI1_COMP4CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP4CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp3calp */
#define RX_APB_MMAP_RXADC_CHI1_COMP3CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP3CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp3caln */
#define RX_APB_MMAP_RXADC_CHI1_COMP3CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP3CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp2calp */
#define RX_APB_MMAP_RXADC_CHI1_COMP2CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP2CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp2caln */
#define RX_APB_MMAP_RXADC_CHI1_COMP2CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP2CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp1calp */
#define RX_APB_MMAP_RXADC_CHI1_COMP1CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP1CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp1caln */
#define RX_APB_MMAP_RXADC_CHI1_COMP1CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP1CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp0calp */
#define RX_APB_MMAP_RXADC_CHI1_COMP0CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP0CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_comp0caln */
#define RX_APB_MMAP_RXADC_CHI1_COMP0CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMP0CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_compdelay */
#define RX_APB_MMAP_RXADC_CHI1_COMPDELAY_COMPDELAY_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_COMPDELAY_COMPDELAY_SIZE 13

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_fail_detect */
#define RX_APB_MMAP_RXADC_CHI1_FAIL_DETECT_CH1P1COMPFAILFLAG_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_FAIL_DETECT_CH1P1COMPFAILFLAG_SIZE 1
#define RX_APB_MMAP_RXADC_CHI1_FAIL_DETECT_CH1P2COMPFAILFLAG_OFFSET 1
#define RX_APB_MMAP_RXADC_CHI1_FAIL_DETECT_CH1P2COMPFAILFLAG_SIZE 1
#define RX_APB_MMAP_RXADC_CHI1_FAIL_DETECT_CH1AMPFAILFLAG_OFFSET 2
#define RX_APB_MMAP_RXADC_CHI1_FAIL_DETECT_CH1AMPFAILFLAG_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_spare */
#define RX_APB_MMAP_RXADC_CHI1_SPARE_SPARE_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_SPARE_SPARE_SIZE 16

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_n_values */
#define RX_APB_MMAP_RXADC_CHI1_N_VALUES_N_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_N_VALUES_N_SIZE 4
#define RX_APB_MMAP_RXADC_CHI1_N_VALUES_N2_OFFSET 4
#define RX_APB_MMAP_RXADC_CHI1_N_VALUES_N2_SIZE 4
#define RX_APB_MMAP_RXADC_CHI1_N_VALUES_N4_OFFSET 8
#define RX_APB_MMAP_RXADC_CHI1_N_VALUES_N4_SIZE 4

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_threshold12 */
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD12_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD12_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_threshold11 */
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD11_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD11_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_threshold10 */
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD10_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD10_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_threshold9 */
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD9_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD9_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_threshold8 */
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD8_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD8_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_threshold7 */
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD7_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD7_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_threshold6 */
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD6_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD6_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_threshold5 */
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD5_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD5_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_threshold4 */
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD4_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD4_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_threshold3 */
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD3_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD3_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi1_threshold2 */
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD2_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI1_THRESHOLD2_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_cal */
#define RX_APB_MMAP_RXADC_CHI2_CAL_CH2CALMODE_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_CAL_CH2CALMODE_SIZE 2
#define RX_APB_MMAP_RXADC_CHI2_CAL_SHORTADCINPUT_OFFSET 2
#define RX_APB_MMAP_RXADC_CHI2_CAL_SHORTADCINPUT_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_calstagectrl */
#define RX_APB_MMAP_RXADC_CHI2_CALSTAGECTRL_STAGEDISABLE_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_CALSTAGECTRL_STAGEDISABLE_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_channelgain */
#define RX_APB_MMAP_RXADC_CHI2_CHANNELGAIN_CH2CHANNELGAIN_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_CHANNELGAIN_CH2CHANNELGAIN_SIZE 5

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_dacn1 */
#define RX_APB_MMAP_RXADC_CHI2_DACN1_CH2CAPCONTROLN1_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_DACN1_CH2CAPCONTROLN1_SIZE 9

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_dacp1 */
#define RX_APB_MMAP_RXADC_CHI2_DACP1_CH2CAPCONTROLP1_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_DACP1_CH2CAPCONTROLP1_SIZE 9

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_dacn2 */
#define RX_APB_MMAP_RXADC_CHI2_DACN2_CH2CAPCONTROLN2_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_DACN2_CH2CAPCONTROLN2_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_dacp2 */
#define RX_APB_MMAP_RXADC_CHI2_DACP2_CH2CAPCONTROLP2_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_DACP2_CH2CAPCONTROLP2_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_amp */
#define RX_APB_MMAP_RXADC_CHI2_AMP_CH2AMPGAINCALP_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_AMP_CH2AMPGAINCALP_SIZE 7
#define RX_APB_MMAP_RXADC_CHI2_AMP_CH2AMPGAINCALN_OFFSET 7
#define RX_APB_MMAP_RXADC_CHI2_AMP_CH2AMPGAINCALN_SIZE 7
#define RX_APB_MMAP_RXADC_CHI2_AMP_CH2INCREASEAMPDELAY_OFFSET 14
#define RX_APB_MMAP_RXADC_CHI2_AMP_CH2INCREASEAMPDELAY_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_msbcal */
#define RX_APB_MMAP_RXADC_CHI2_MSBCAL_CH2MSBCAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_MSBCAL_CH2MSBCAL_SIZE 10

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp12calp */
#define RX_APB_MMAP_RXADC_CHI2_COMP12CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP12CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp12caln */
#define RX_APB_MMAP_RXADC_CHI2_COMP12CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP12CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp11calp */
#define RX_APB_MMAP_RXADC_CHI2_COMP11CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP11CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp11caln */
#define RX_APB_MMAP_RXADC_CHI2_COMP11CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP11CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp10calp */
#define RX_APB_MMAP_RXADC_CHI2_COMP10CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP10CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp10caln */
#define RX_APB_MMAP_RXADC_CHI2_COMP10CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP10CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp9calp */
#define RX_APB_MMAP_RXADC_CHI2_COMP9CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP9CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp9caln */
#define RX_APB_MMAP_RXADC_CHI2_COMP9CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP9CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp8calp */
#define RX_APB_MMAP_RXADC_CHI2_COMP8CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP8CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp8caln */
#define RX_APB_MMAP_RXADC_CHI2_COMP8CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP8CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp7calp */
#define RX_APB_MMAP_RXADC_CHI2_COMP7CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP7CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp7caln */
#define RX_APB_MMAP_RXADC_CHI2_COMP7CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP7CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp6calp */
#define RX_APB_MMAP_RXADC_CHI2_COMP6CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP6CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp6caln */
#define RX_APB_MMAP_RXADC_CHI2_COMP6CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP6CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp5calp */
#define RX_APB_MMAP_RXADC_CHI2_COMP5CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP5CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp5caln */
#define RX_APB_MMAP_RXADC_CHI2_COMP5CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP5CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp4calp */
#define RX_APB_MMAP_RXADC_CHI2_COMP4CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP4CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp4caln */
#define RX_APB_MMAP_RXADC_CHI2_COMP4CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP4CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp3calp */
#define RX_APB_MMAP_RXADC_CHI2_COMP3CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP3CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp3caln */
#define RX_APB_MMAP_RXADC_CHI2_COMP3CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP3CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp2calp */
#define RX_APB_MMAP_RXADC_CHI2_COMP2CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP2CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp2caln */
#define RX_APB_MMAP_RXADC_CHI2_COMP2CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP2CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp1calp */
#define RX_APB_MMAP_RXADC_CHI2_COMP1CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP1CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp1caln */
#define RX_APB_MMAP_RXADC_CHI2_COMP1CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP1CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp0calp */
#define RX_APB_MMAP_RXADC_CHI2_COMP0CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP0CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_comp0caln */
#define RX_APB_MMAP_RXADC_CHI2_COMP0CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMP0CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_compdelay */
#define RX_APB_MMAP_RXADC_CHI2_COMPDELAY_COMPDELAY_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_COMPDELAY_COMPDELAY_SIZE 13

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_fail_detect */
#define RX_APB_MMAP_RXADC_CHI2_FAIL_DETECT_CH2P1COMPFAILFLAG_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_FAIL_DETECT_CH2P1COMPFAILFLAG_SIZE 1
#define RX_APB_MMAP_RXADC_CHI2_FAIL_DETECT_CH2P2COMPFAILFLAG_OFFSET 1
#define RX_APB_MMAP_RXADC_CHI2_FAIL_DETECT_CH2P2COMPFAILFLAG_SIZE 1
#define RX_APB_MMAP_RXADC_CHI2_FAIL_DETECT_CH2AMPFAILFLAG_OFFSET 2
#define RX_APB_MMAP_RXADC_CHI2_FAIL_DETECT_CH2AMPFAILFLAG_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_spare */
#define RX_APB_MMAP_RXADC_CHI2_SPARE_SPARE_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_SPARE_SPARE_SIZE 16

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_n_values */
#define RX_APB_MMAP_RXADC_CHI2_N_VALUES_N_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_N_VALUES_N_SIZE 4
#define RX_APB_MMAP_RXADC_CHI2_N_VALUES_N2_OFFSET 4
#define RX_APB_MMAP_RXADC_CHI2_N_VALUES_N2_SIZE 4
#define RX_APB_MMAP_RXADC_CHI2_N_VALUES_N4_OFFSET 8
#define RX_APB_MMAP_RXADC_CHI2_N_VALUES_N4_SIZE 4

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_threshold12 */
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD12_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD12_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_threshold11 */
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD11_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD11_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_threshold10 */
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD10_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD10_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_threshold9 */
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD9_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD9_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_threshold8 */
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD8_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD8_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_threshold7 */
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD7_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD7_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_threshold6 */
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD6_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD6_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_threshold5 */
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD5_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD5_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_threshold4 */
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD4_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD4_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_threshold3 */
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD3_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD3_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chi2_threshold2 */
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD2_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHI2_THRESHOLD2_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_cal */
#define RX_APB_MMAP_RXADC_CHQ1_CAL_CH1CALMODE_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_CAL_CH1CALMODE_SIZE 2
#define RX_APB_MMAP_RXADC_CHQ1_CAL_SHORTADCINPUT_OFFSET 2
#define RX_APB_MMAP_RXADC_CHQ1_CAL_SHORTADCINPUT_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_calstagectrl */
#define RX_APB_MMAP_RXADC_CHQ1_CALSTAGECTRL_STAGEDISABLE_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_CALSTAGECTRL_STAGEDISABLE_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_channelgain */
#define RX_APB_MMAP_RXADC_CHQ1_CHANNELGAIN_CH1CHANNELGAIN_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_CHANNELGAIN_CH1CHANNELGAIN_SIZE 5

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_dacn1 */
#define RX_APB_MMAP_RXADC_CHQ1_DACN1_CH1CAPCONTROLN1_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_DACN1_CH1CAPCONTROLN1_SIZE 9

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_dacp1 */
#define RX_APB_MMAP_RXADC_CHQ1_DACP1_CH1CAPCONTROLP1_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_DACP1_CH1CAPCONTROLP1_SIZE 9

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_dacn2 */
#define RX_APB_MMAP_RXADC_CHQ1_DACN2_CH1CAPCONTROLN2_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_DACN2_CH1CAPCONTROLN2_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_dacp2 */
#define RX_APB_MMAP_RXADC_CHQ1_DACP2_CH1CAPCONTROLP2_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_DACP2_CH1CAPCONTROLP2_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_amp */
#define RX_APB_MMAP_RXADC_CHQ1_AMP_CH1AMPGAINCALP_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_AMP_CH1AMPGAINCALP_SIZE 7
#define RX_APB_MMAP_RXADC_CHQ1_AMP_CH1AMPGAINCALN_OFFSET 7
#define RX_APB_MMAP_RXADC_CHQ1_AMP_CH1AMPGAINCALN_SIZE 7
#define RX_APB_MMAP_RXADC_CHQ1_AMP_CH1INCREASEAMPDELAY_OFFSET 14
#define RX_APB_MMAP_RXADC_CHQ1_AMP_CH1INCREASEAMPDELAY_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_msbcal */
#define RX_APB_MMAP_RXADC_CHQ1_MSBCAL_CH1MSBCAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_MSBCAL_CH1MSBCAL_SIZE 10

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp12calp */
#define RX_APB_MMAP_RXADC_CHQ1_COMP12CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP12CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp12caln */
#define RX_APB_MMAP_RXADC_CHQ1_COMP12CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP12CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp11calp */
#define RX_APB_MMAP_RXADC_CHQ1_COMP11CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP11CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp11caln */
#define RX_APB_MMAP_RXADC_CHQ1_COMP11CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP11CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp10calp */
#define RX_APB_MMAP_RXADC_CHQ1_COMP10CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP10CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp10caln */
#define RX_APB_MMAP_RXADC_CHQ1_COMP10CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP10CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp9calp */
#define RX_APB_MMAP_RXADC_CHQ1_COMP9CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP9CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp9caln */
#define RX_APB_MMAP_RXADC_CHQ1_COMP9CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP9CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp8calp */
#define RX_APB_MMAP_RXADC_CHQ1_COMP8CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP8CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp8caln */
#define RX_APB_MMAP_RXADC_CHQ1_COMP8CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP8CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp7calp */
#define RX_APB_MMAP_RXADC_CHQ1_COMP7CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP7CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp7caln */
#define RX_APB_MMAP_RXADC_CHQ1_COMP7CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP7CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp6calp */
#define RX_APB_MMAP_RXADC_CHQ1_COMP6CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP6CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp6caln */
#define RX_APB_MMAP_RXADC_CHQ1_COMP6CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP6CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp5calp */
#define RX_APB_MMAP_RXADC_CHQ1_COMP5CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP5CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp5caln */
#define RX_APB_MMAP_RXADC_CHQ1_COMP5CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP5CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp4calp */
#define RX_APB_MMAP_RXADC_CHQ1_COMP4CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP4CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp4caln */
#define RX_APB_MMAP_RXADC_CHQ1_COMP4CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP4CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp3calp */
#define RX_APB_MMAP_RXADC_CHQ1_COMP3CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP3CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp3caln */
#define RX_APB_MMAP_RXADC_CHQ1_COMP3CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP3CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp2calp */
#define RX_APB_MMAP_RXADC_CHQ1_COMP2CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP2CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp2caln */
#define RX_APB_MMAP_RXADC_CHQ1_COMP2CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP2CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp1calp */
#define RX_APB_MMAP_RXADC_CHQ1_COMP1CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP1CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp1caln */
#define RX_APB_MMAP_RXADC_CHQ1_COMP1CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP1CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp0calp */
#define RX_APB_MMAP_RXADC_CHQ1_COMP0CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP0CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_comp0caln */
#define RX_APB_MMAP_RXADC_CHQ1_COMP0CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMP0CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_compdelay */
#define RX_APB_MMAP_RXADC_CHQ1_COMPDELAY_COMPDELAY_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_COMPDELAY_COMPDELAY_SIZE 13

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_fail_detect */
#define RX_APB_MMAP_RXADC_CHQ1_FAIL_DETECT_CH1P1COMPFAILFLAG_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_FAIL_DETECT_CH1P1COMPFAILFLAG_SIZE 1
#define RX_APB_MMAP_RXADC_CHQ1_FAIL_DETECT_CH1P2COMPFAILFLAG_OFFSET 1
#define RX_APB_MMAP_RXADC_CHQ1_FAIL_DETECT_CH1P2COMPFAILFLAG_SIZE 1
#define RX_APB_MMAP_RXADC_CHQ1_FAIL_DETECT_CH1AMPFAILFLAG_OFFSET 2
#define RX_APB_MMAP_RXADC_CHQ1_FAIL_DETECT_CH1AMPFAILFLAG_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_spare */
#define RX_APB_MMAP_RXADC_CHQ1_SPARE_SPARE_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_SPARE_SPARE_SIZE 16

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_n_values */
#define RX_APB_MMAP_RXADC_CHQ1_N_VALUES_N_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_N_VALUES_N_SIZE 4
#define RX_APB_MMAP_RXADC_CHQ1_N_VALUES_N2_OFFSET 4
#define RX_APB_MMAP_RXADC_CHQ1_N_VALUES_N2_SIZE 4
#define RX_APB_MMAP_RXADC_CHQ1_N_VALUES_N4_OFFSET 8
#define RX_APB_MMAP_RXADC_CHQ1_N_VALUES_N4_SIZE 4

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_threshold12 */
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD12_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD12_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_threshold11 */
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD11_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD11_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_threshold10 */
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD10_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD10_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_threshold9 */
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD9_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD9_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_threshold8 */
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD8_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD8_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_threshold7 */
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD7_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD7_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_threshold6 */
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD6_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD6_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_threshold5 */
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD5_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD5_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_threshold4 */
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD4_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD4_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_threshold3 */
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD3_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD3_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq1_threshold2 */
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD2_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ1_THRESHOLD2_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_cal */
#define RX_APB_MMAP_RXADC_CHQ2_CAL_CH2CALMODE_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_CAL_CH2CALMODE_SIZE 2
#define RX_APB_MMAP_RXADC_CHQ2_CAL_SHORTADCINPUT_OFFSET 2
#define RX_APB_MMAP_RXADC_CHQ2_CAL_SHORTADCINPUT_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_calstagectrl */
#define RX_APB_MMAP_RXADC_CHQ2_CALSTAGECTRL_STAGEDISABLE_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_CALSTAGECTRL_STAGEDISABLE_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_channelgain */
#define RX_APB_MMAP_RXADC_CHQ2_CHANNELGAIN_CH2CHANNELGAIN_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_CHANNELGAIN_CH2CHANNELGAIN_SIZE 5

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_dacn1 */
#define RX_APB_MMAP_RXADC_CHQ2_DACN1_CH2CAPCONTROLN1_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_DACN1_CH2CAPCONTROLN1_SIZE 9

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_dacp1 */
#define RX_APB_MMAP_RXADC_CHQ2_DACP1_CH2CAPCONTROLP1_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_DACP1_CH2CAPCONTROLP1_SIZE 9

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_dacn2 */
#define RX_APB_MMAP_RXADC_CHQ2_DACN2_CH2CAPCONTROLN2_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_DACN2_CH2CAPCONTROLN2_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_dacp2 */
#define RX_APB_MMAP_RXADC_CHQ2_DACP2_CH2CAPCONTROLP2_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_DACP2_CH2CAPCONTROLP2_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_amp */
#define RX_APB_MMAP_RXADC_CHQ2_AMP_CH2AMPGAINCALP_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_AMP_CH2AMPGAINCALP_SIZE 7
#define RX_APB_MMAP_RXADC_CHQ2_AMP_CH2AMPGAINCALN_OFFSET 7
#define RX_APB_MMAP_RXADC_CHQ2_AMP_CH2AMPGAINCALN_SIZE 7
#define RX_APB_MMAP_RXADC_CHQ2_AMP_CH2INCREASEAMPDELAY_OFFSET 14
#define RX_APB_MMAP_RXADC_CHQ2_AMP_CH2INCREASEAMPDELAY_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_msbcal */
#define RX_APB_MMAP_RXADC_CHQ2_MSBCAL_CH2MSBCAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_MSBCAL_CH2MSBCAL_SIZE 10

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp12calp */
#define RX_APB_MMAP_RXADC_CHQ2_COMP12CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP12CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp12caln */
#define RX_APB_MMAP_RXADC_CHQ2_COMP12CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP12CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp11calp */
#define RX_APB_MMAP_RXADC_CHQ2_COMP11CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP11CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp11caln */
#define RX_APB_MMAP_RXADC_CHQ2_COMP11CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP11CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp10calp */
#define RX_APB_MMAP_RXADC_CHQ2_COMP10CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP10CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp10caln */
#define RX_APB_MMAP_RXADC_CHQ2_COMP10CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP10CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp9calp */
#define RX_APB_MMAP_RXADC_CHQ2_COMP9CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP9CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp9caln */
#define RX_APB_MMAP_RXADC_CHQ2_COMP9CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP9CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp8calp */
#define RX_APB_MMAP_RXADC_CHQ2_COMP8CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP8CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp8caln */
#define RX_APB_MMAP_RXADC_CHQ2_COMP8CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP8CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp7calp */
#define RX_APB_MMAP_RXADC_CHQ2_COMP7CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP7CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp7caln */
#define RX_APB_MMAP_RXADC_CHQ2_COMP7CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP7CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp6calp */
#define RX_APB_MMAP_RXADC_CHQ2_COMP6CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP6CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp6caln */
#define RX_APB_MMAP_RXADC_CHQ2_COMP6CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP6CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp5calp */
#define RX_APB_MMAP_RXADC_CHQ2_COMP5CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP5CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp5caln */
#define RX_APB_MMAP_RXADC_CHQ2_COMP5CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP5CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp4calp */
#define RX_APB_MMAP_RXADC_CHQ2_COMP4CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP4CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp4caln */
#define RX_APB_MMAP_RXADC_CHQ2_COMP4CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP4CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp3calp */
#define RX_APB_MMAP_RXADC_CHQ2_COMP3CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP3CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp3caln */
#define RX_APB_MMAP_RXADC_CHQ2_COMP3CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP3CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp2calp */
#define RX_APB_MMAP_RXADC_CHQ2_COMP2CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP2CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp2caln */
#define RX_APB_MMAP_RXADC_CHQ2_COMP2CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP2CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp1calp */
#define RX_APB_MMAP_RXADC_CHQ2_COMP1CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP1CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp1caln */
#define RX_APB_MMAP_RXADC_CHQ2_COMP1CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP1CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp0calp */
#define RX_APB_MMAP_RXADC_CHQ2_COMP0CALP_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP0CALP_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_comp0caln */
#define RX_APB_MMAP_RXADC_CHQ2_COMP0CALN_COMP_CAL_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMP0CALN_COMP_CAL_SIZE 8

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_compdelay */
#define RX_APB_MMAP_RXADC_CHQ2_COMPDELAY_COMPDELAY_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_COMPDELAY_COMPDELAY_SIZE 13

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_fail_detect */
#define RX_APB_MMAP_RXADC_CHQ2_FAIL_DETECT_CH2P1COMPFAILFLAG_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_FAIL_DETECT_CH2P1COMPFAILFLAG_SIZE 1
#define RX_APB_MMAP_RXADC_CHQ2_FAIL_DETECT_CH2P2COMPFAILFLAG_OFFSET 1
#define RX_APB_MMAP_RXADC_CHQ2_FAIL_DETECT_CH2P2COMPFAILFLAG_SIZE 1
#define RX_APB_MMAP_RXADC_CHQ2_FAIL_DETECT_CH2AMPFAILFLAG_OFFSET 2
#define RX_APB_MMAP_RXADC_CHQ2_FAIL_DETECT_CH2AMPFAILFLAG_SIZE 1

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_spare */
#define RX_APB_MMAP_RXADC_CHQ2_SPARE_SPARE_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_SPARE_SPARE_SIZE 16

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_n_values */
#define RX_APB_MMAP_RXADC_CHQ2_N_VALUES_N_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_N_VALUES_N_SIZE 4
#define RX_APB_MMAP_RXADC_CHQ2_N_VALUES_N2_OFFSET 4
#define RX_APB_MMAP_RXADC_CHQ2_N_VALUES_N2_SIZE 4
#define RX_APB_MMAP_RXADC_CHQ2_N_VALUES_N4_OFFSET 8
#define RX_APB_MMAP_RXADC_CHQ2_N_VALUES_N4_SIZE 4

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_threshold12 */
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD12_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD12_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_threshold11 */
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD11_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD11_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_threshold10 */
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD10_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD10_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_threshold9 */
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD9_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD9_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_threshold8 */
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD8_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD8_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_threshold7 */
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD7_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD7_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_threshold6 */
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD6_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD6_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_threshold5 */
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD5_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD5_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_threshold4 */
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD4_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD4_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_threshold3 */
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD3_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD3_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#chq2_threshold2 */
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD2_THRESHOLD_OFFSET 0
#define RX_APB_MMAP_RXADC_CHQ2_THRESHOLD2_THRESHOLD_SIZE 6

/** bit field defines for rx_apb_mmap_rxadc_s#dbg_rxadc */
#define RX_APB_MMAP_RXADC_DBG_RXADC_MEMMAP_VERSION_SHDW_OFFSET 0
#define RX_APB_MMAP_RXADC_DBG_RXADC_MEMMAP_VERSION_SHDW_SIZE 16
#define RX_APB_MMAP_RXADC_DBG_RXADC_MEMMAP_VERSION_CFG_OFFSET 16
#define RX_APB_MMAP_RXADC_DBG_RXADC_MEMMAP_VERSION_CFG_SIZE 16

/* EOF rx_apb_mmap_rxadc.h */
#endif
