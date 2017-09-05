#ifndef INCLUDED_TX_DIG_TOP_TXDFE2
#define INCLUDED_TX_DIG_TOP_TXDFE2
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** tx_dig_top_txdfe2 HAL Spreadsheet version number */
#define TX_DIG_TOP_TXDFE2_HAL_VERSION 1

/** This is the tx dfe part 2 line-up register description
*/
struct tx_dig_top_txdfe2_s {
   /** ramp scale control profile 1 at address offset 0x000, read-write */
   uint32_t ramp_p1_1;
   /** ramp scale control profile 1 at address offset 0x004, read-write */
   uint32_t ramp_p1_2;
   /** ramp scale control profile 1 at address offset 0x008, read-write */
   uint32_t ramp_p1_3;
   /** ramp scale control profile 1 at address offset 0x00C, read-write */
   uint32_t ramp_p1_4;
   /** ramp scale control profile 1 at address offset 0x010, read-write */
   uint32_t ramp_p1_5;
   /** ramp scale control profile 1 at address offset 0x014, read-write */
   uint32_t ramp_p1_6;
   /** ramp scale control profile 1 at address offset 0x018, read-write */
   uint32_t ramp_p1_7;
   /** ramp scale control profile 1 at address offset 0x01C, read-write */
   uint32_t ramp_p1_8;
   /** ramp scale control profile 1 at address offset 0x020, read-write */
   uint32_t ramp_p1_9;
   /** ramp scale control profile 1 at address offset 0x024, read-write */
   uint32_t ramp_p1_10;
   /** ramp scale control profile 1 at address offset 0x028, read-write */
   uint32_t ramp_p1_11;
   /** ramp scale control profile 1 at address offset 0x02C, read-write */
   uint32_t ramp_p1_12;
   /** ramp scale control profile 1 at address offset 0x030, read-write */
   uint32_t ramp_p1_13;
   /** ramp scale control profile 1 at address offset 0x034, read-write */
   uint32_t ramp_p1_14;
   /** ramp scale control profile 1 at address offset 0x038, read-write */
   uint32_t ramp_p1_15;
   /** ramp scale control profile 1 at address offset 0x03C, read-write */
   uint32_t ramp_p1_16;
   /** ramp scale control profile 1 at address offset 0x040, read-write */
   uint32_t ramp_p1_17;
   /** ramp scale control profile 1 at address offset 0x044, read-write */
   uint32_t ramp_p1_18;
   /** ramp scale control profile 1 at address offset 0x048, read-write */
   uint32_t ramp_p1_19;
   /** ramp scale control profile 1 at address offset 0x04C, read-write */
   uint32_t ramp_p1_20;
   /** ramp scale control profile 1 at address offset 0x050, read-write */
   uint32_t ramp_p1_21;
   /** ramp scale control profile 1 at address offset 0x054, read-write */
   uint32_t ramp_p1_22;
   /** ramp scale control profile 1 at address offset 0x058, read-write */
   uint32_t ramp_p1_23;
   /** ramp scale control profile 1 at address offset 0x05C, read-write */
   uint32_t ramp_p1_24;
   /** ramp scale control profile 1 at address offset 0x060, read-write */
   uint32_t ramp_p1_25;
   /** ramp scale control profile 1 at address offset 0x064, read-write */
   uint32_t ramp_p1_26;
   /** ramp scale control profile 1 at address offset 0x068, read-write */
   uint32_t ramp_p1_27;
   /** ramp scale control profile 1 at address offset 0x06C, read-write */
   uint32_t ramp_p1_28;
   /** ramp scale control profile 1 at address offset 0x070, read-write */
   uint32_t ramp_p1_29;
   /** ramp scale control profile 1 at address offset 0x074, read-write */
   uint32_t ramp_p1_30;
   /** ramp scale control profile 1 at address offset 0x078, read-write */
   uint32_t ramp_p1_31;
   /** ramp scale control profile 1 at address offset 0x07C, read-write */
   uint32_t ramp_p1_32;
   /** ramp scale control profile 2 at address offset 0x080, read-write */
   uint32_t ramp_p2_1;
   /** ramp scale control profile 2 at address offset 0x084, read-write */
   uint32_t ramp_p2_2;
   /** ramp scale control profile 2 at address offset 0x088, read-write */
   uint32_t ramp_p2_3;
   /** ramp scale control profile 2 at address offset 0x08C, read-write */
   uint32_t ramp_p2_4;
   /** ramp scale control profile 2 at address offset 0x090, read-write */
   uint32_t ramp_p2_5;
   /** ramp scale control profile 2 at address offset 0x094, read-write */
   uint32_t ramp_p2_6;
   /** ramp scale control profile 2 at address offset 0x098, read-write */
   uint32_t ramp_p2_7;
   /** ramp scale control profile 2 at address offset 0x09C, read-write */
   uint32_t ramp_p2_8;
   /** ramp scale control profile 2 at address offset 0x0A0, read-write */
   uint32_t ramp_p2_9;
   /** ramp scale control profile 2 at address offset 0x0A4, read-write */
   uint32_t ramp_p2_10;
   /** ramp scale control profile 2 at address offset 0x0A8, read-write */
   uint32_t ramp_p2_11;
   /** ramp scale control profile 2 at address offset 0x0AC, read-write */
   uint32_t ramp_p2_12;
   /** ramp scale control profile 2 at address offset 0x0B0, read-write */
   uint32_t ramp_p2_13;
   /** ramp scale control profile 2 at address offset 0x0B4, read-write */
   uint32_t ramp_p2_14;
   /** ramp scale control profile 2 at address offset 0x0B8, read-write */
   uint32_t ramp_p2_15;
   /** ramp scale control profile 2 at address offset 0x0BC, read-write */
   uint32_t ramp_p2_16;
   /** ramp scale control profile 2 at address offset 0x0C0, read-write */
   uint32_t ramp_p2_17;
   /** ramp scale control profile 2 at address offset 0x0C4, read-write */
   uint32_t ramp_p2_18;
   /** ramp scale control profile 2 at address offset 0x0C8, read-write */
   uint32_t ramp_p2_19;
   /** ramp scale control profile 2 at address offset 0x0CC, read-write */
   uint32_t ramp_p2_20;
   /** ramp scale control profile 2 at address offset 0x0D0, read-write */
   uint32_t ramp_p2_21;
   /** ramp scale control profile 2 at address offset 0x0D4, read-write */
   uint32_t ramp_p2_22;
   /** ramp scale control profile 2 at address offset 0x0D8, read-write */
   uint32_t ramp_p2_23;
   /** ramp scale control profile 2 at address offset 0x0DC, read-write */
   uint32_t ramp_p2_24;
   /** ramp scale control profile 2 at address offset 0x0E0, read-write */
   uint32_t ramp_p2_25;
   /** ramp scale control profile 2 at address offset 0x0E4, read-write */
   uint32_t ramp_p2_26;
   /** ramp scale control profile 2 at address offset 0x0E8, read-write */
   uint32_t ramp_p2_27;
   /** ramp scale control profile 2 at address offset 0x0EC, read-write */
   uint32_t ramp_p2_28;
   /** ramp scale control profile 2 at address offset 0x0F0, read-write */
   uint32_t ramp_p2_29;
   /** ramp scale control profile 2 at address offset 0x0F4, read-write */
   uint32_t ramp_p2_30;
   /** ramp scale control profile 2 at address offset 0x0F8, read-write */
   uint32_t ramp_p2_31;
   /** ramp scale control profile 2 at address offset 0x0FC, read-write */
   uint32_t ramp_p2_32;
};

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_1 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_1_P1_1_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_1_P1_1_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_1_P1_1_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_1_P1_1_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_2 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_2_P1_2_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_2_P1_2_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_2_P1_2_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_2_P1_2_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_3 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_3_P1_3_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_3_P1_3_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_3_P1_3_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_3_P1_3_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_4 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_4_P1_4_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_4_P1_4_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_4_P1_4_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_4_P1_4_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_5 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_5_P1_5_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_5_P1_5_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_5_P1_5_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_5_P1_5_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_6 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_6_P1_6_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_6_P1_6_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_6_P1_6_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_6_P1_6_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_7 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_7_P1_7_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_7_P1_7_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_7_P1_7_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_7_P1_7_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_8 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_8_P1_8_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_8_P1_8_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_8_P1_8_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_8_P1_8_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_9 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_9_P1_9_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_9_P1_9_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_9_P1_9_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_9_P1_9_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_10 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_10_P1_10_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_10_P1_10_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_10_P1_10_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_10_P1_10_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_11 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_11_P1_11_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_11_P1_11_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_11_P1_11_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_11_P1_11_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_12 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_12_P1_12_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_12_P1_12_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_12_P1_12_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_12_P1_12_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_13 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_13_P1_13_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_13_P1_13_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_13_P1_13_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_13_P1_13_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_14 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_14_P1_14_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_14_P1_14_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_14_P1_14_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_14_P1_14_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_15 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_15_P1_15_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_15_P1_15_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_15_P1_15_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_15_P1_15_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_16 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_16_P1_16_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_16_P1_16_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_16_P1_16_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_16_P1_16_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_17 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_17_P1_17_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_17_P1_17_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_17_P1_17_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_17_P1_17_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_18 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_18_P1_18_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_18_P1_18_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_18_P1_18_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_18_P1_18_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_19 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_19_P1_19_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_19_P1_19_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_19_P1_19_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_19_P1_19_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_20 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_20_P1_20_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_20_P1_20_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_20_P1_20_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_20_P1_20_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_21 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_21_P1_21_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_21_P1_21_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_21_P1_21_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_21_P1_21_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_22 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_22_P1_22_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_22_P1_22_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_22_P1_22_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_22_P1_22_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_23 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_23_P1_23_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_23_P1_23_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_23_P1_23_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_23_P1_23_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_24 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_24_P1_24_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_24_P1_24_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_24_P1_24_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_24_P1_24_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_25 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_25_P1_25_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_25_P1_25_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_25_P1_25_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_25_P1_25_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_26 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_26_P1_26_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_26_P1_26_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_26_P1_26_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_26_P1_26_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_27 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_27_P1_27_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_27_P1_27_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_27_P1_27_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_27_P1_27_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_28 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_28_P1_28_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_28_P1_28_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_28_P1_28_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_28_P1_28_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_29 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_29_P1_29_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_29_P1_29_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_29_P1_29_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_29_P1_29_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_30 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_30_P1_30_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_30_P1_30_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_30_P1_30_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_30_P1_30_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_31 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_31_P1_31_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_31_P1_31_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_31_P1_31_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_31_P1_31_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p1_32 */
#define TX_DIG_TOP_TXDFE2_RAMP_P1_32_P1_32_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P1_32_P1_32_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_32_P1_32_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P1_32_P1_32_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_1 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_1_P2_1_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_1_P2_1_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_1_P2_1_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_1_P2_1_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_2 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_2_P2_2_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_2_P2_2_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_2_P2_2_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_2_P2_2_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_3 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_3_P2_3_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_3_P2_3_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_3_P2_3_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_3_P2_3_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_4 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_4_P2_4_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_4_P2_4_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_4_P2_4_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_4_P2_4_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_5 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_5_P2_5_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_5_P2_5_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_5_P2_5_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_5_P2_5_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_6 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_6_P2_6_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_6_P2_6_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_6_P2_6_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_6_P2_6_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_7 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_7_P2_7_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_7_P2_7_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_7_P2_7_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_7_P2_7_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_8 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_8_P2_8_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_8_P2_8_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_8_P2_8_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_8_P2_8_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_9 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_9_P2_9_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_9_P2_9_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_9_P2_9_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_9_P2_9_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_10 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_10_P2_10_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_10_P2_10_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_10_P2_10_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_10_P2_10_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_11 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_11_P2_11_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_11_P2_11_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_11_P2_11_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_11_P2_11_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_12 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_12_P2_12_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_12_P2_12_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_12_P2_12_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_12_P2_12_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_13 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_13_P2_13_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_13_P2_13_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_13_P2_13_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_13_P2_13_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_14 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_14_P2_14_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_14_P2_14_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_14_P2_14_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_14_P2_14_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_15 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_15_P2_15_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_15_P2_15_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_15_P2_15_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_15_P2_15_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_16 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_16_P2_16_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_16_P2_16_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_16_P2_16_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_16_P2_16_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_17 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_17_P2_17_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_17_P2_17_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_17_P2_17_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_17_P2_17_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_18 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_18_P2_18_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_18_P2_18_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_18_P2_18_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_18_P2_18_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_19 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_19_P2_19_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_19_P2_19_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_19_P2_19_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_19_P2_19_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_20 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_20_P2_20_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_20_P2_20_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_20_P2_20_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_20_P2_20_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_21 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_21_P2_21_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_21_P2_21_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_21_P2_21_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_21_P2_21_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_22 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_22_P2_22_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_22_P2_22_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_22_P2_22_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_22_P2_22_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_23 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_23_P2_23_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_23_P2_23_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_23_P2_23_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_23_P2_23_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_24 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_24_P2_24_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_24_P2_24_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_24_P2_24_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_24_P2_24_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_25 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_25_P2_25_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_25_P2_25_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_25_P2_25_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_25_P2_25_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_26 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_26_P2_26_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_26_P2_26_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_26_P2_26_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_26_P2_26_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_27 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_27_P2_27_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_27_P2_27_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_27_P2_27_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_27_P2_27_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_28 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_28_P2_28_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_28_P2_28_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_28_P2_28_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_28_P2_28_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_29 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_29_P2_29_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_29_P2_29_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_29_P2_29_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_29_P2_29_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_30 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_30_P2_30_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_30_P2_30_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_30_P2_30_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_30_P2_30_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_31 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_31_P2_31_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_31_P2_31_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_31_P2_31_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_31_P2_31_DELAY_SIZE 18

/** bit field defines for tx_dig_top_txdfe2_s#ramp_p2_32 */
#define TX_DIG_TOP_TXDFE2_RAMP_P2_32_P2_32_SCALE_OFFSET 0
#define TX_DIG_TOP_TXDFE2_RAMP_P2_32_P2_32_SCALE_SIZE 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_32_P2_32_DELAY_OFFSET 10
#define TX_DIG_TOP_TXDFE2_RAMP_P2_32_P2_32_DELAY_SIZE 18

/* EOF tx_dig_top_txdfe2.h */
#endif
