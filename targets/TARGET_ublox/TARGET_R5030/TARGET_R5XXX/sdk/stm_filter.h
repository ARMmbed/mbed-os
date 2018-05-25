#ifndef INCLUDED_STM_FILTER
#define INCLUDED_STM_FILTER
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

#define STM_FILTER_PERIPHERALID4 0x07
#define STM_FILTER_PERIPHERALID5 0x00
#define STM_FILTER_PERIPHERALID6 0x00
#define STM_FILTER_PERIPHERALID7 0x00
#define STM_FILTER_PERIPHERALID0 0x89
#define STM_FILTER_PERIPHERALID1 0xDA
#define STM_FILTER_PERIPHERALID2 0x08
#define STM_FILTER_PERIPHERALID3 0x00
#define STM_FILTER_COMPONENTID0 0x0D
#define STM_FILTER_COMPONENTID1 0x90
#define STM_FILTER_COMPONENTID2 0x05
#define STM_FILTER_COMPONENTID3 0xB1

/** STM pre-filter
*/
struct stm_filter_s {
   /** Filter control. Each bit in this register corresponds to a pair of two 256 STM channels on consecutive channel groups. 0 : filter disabled, 1 : filter enabled (transactions are dropped). at address offset 0x000, read-write */
   uint32_t filter_ctrl[4];
   /** Reserved space */
   uint8_t fill0[4032];
   /** Peripheral ID Register at address offset 0xFD0, read-only constant 0x00000007 */
   uint32_t peripheralid4;
   /** Peripheral ID Register at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** Peripheral ID Register at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** Peripheral ID Register at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** Peripheral ID Register at address offset 0xFE0, read-only constant 0x00000089 */
   uint32_t peripheralid0;
   /** Peripheral ID Register at address offset 0xFE4, read-only constant 0x000000DA */
   uint32_t peripheralid1;
   /** Peripheral ID Register at address offset 0xFE8, read-only constant 0x00000008 */
   uint32_t peripheralid2;
   /** Peripheral ID Register at address offset 0xFEC, read-only constant 0x00000000 */
   uint32_t peripheralid3;
   /** Primecell Component ID0 at address offset 0xFF0, read-only constant 0x0000000D */
   uint32_t componentid0;
   /** Primecell Component ID1 at address offset 0xFF4, read-only constant 0x00000090 */
   uint32_t componentid1;
   /** Primecell Component ID2 at address offset 0xFF8, read-only constant 0x00000005 */
   uint32_t componentid2;
   /** Primecell Component ID3 at address offset 0xFFC, read-only constant 0x000000B1 */
   uint32_t componentid3;
};

/** bit field defines for stm_filter_s#filter_ctrl */
#define STM_FILTER_FILTER_CTRL_FILTER_CTRL_OFFSET 0
#define STM_FILTER_FILTER_CTRL_FILTER_CTRL_SIZE 8

/** bit field defines for stm_filter_s#peripheralid4 */
#define STM_FILTER_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define STM_FILTER_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for stm_filter_s#peripheralid5 */
#define STM_FILTER_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define STM_FILTER_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for stm_filter_s#peripheralid6 */
#define STM_FILTER_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define STM_FILTER_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for stm_filter_s#peripheralid7 */
#define STM_FILTER_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define STM_FILTER_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for stm_filter_s#peripheralid0 */
#define STM_FILTER_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define STM_FILTER_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for stm_filter_s#peripheralid1 */
#define STM_FILTER_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define STM_FILTER_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for stm_filter_s#peripheralid2 */
#define STM_FILTER_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define STM_FILTER_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for stm_filter_s#peripheralid3 */
#define STM_FILTER_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define STM_FILTER_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for stm_filter_s#componentid0 */
#define STM_FILTER_COMPONENTID0_COMPONENTID0_OFFSET 0
#define STM_FILTER_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for stm_filter_s#componentid1 */
#define STM_FILTER_COMPONENTID1_COMPONENTID1_OFFSET 0
#define STM_FILTER_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for stm_filter_s#componentid2 */
#define STM_FILTER_COMPONENTID2_COMPONENTID2_OFFSET 0
#define STM_FILTER_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for stm_filter_s#componentid3 */
#define STM_FILTER_COMPONENTID3_COMPONENTID3_OFFSET 0
#define STM_FILTER_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF stm_filter.h */
#endif
