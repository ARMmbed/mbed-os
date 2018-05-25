#ifndef INCLUDED_CXTSGEN
#define INCLUDED_CXTSGEN
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

#define CXTSGEN_PERIPHERALID4 0x04
#define CXTSGEN_PERIPHERALID0 0x01
#define CXTSGEN_PERIPHERALID1 0xB1
#define CXTSGEN_PERIPHERALID2 0x1B
#define CXTSGEN_PERIPHERALID3 0x00
#define CXTSGEN_COMPONENTID0 0x0D
#define CXTSGEN_COMPONENTID1 0xF0
#define CXTSGEN_COMPONENTID2 0x05
#define CXTSGEN_COMPONENTID3 0xB1

/** CoreSight Timestamp Generator
*/
struct cxtsgen_s {
   /** Counter Control Register at address offset 0x000, read-write */
   uint32_t cntcr;
   /** Counter Status Register at address offset 0x004, read-only */
   uint32_t cntsr;
   /** Current value of counter[31:0] at address offset 0x008, read-write */
   uint32_t cntcvlw;
   /** Current value of counter[63:32] at address offset 0x00C, read-write */
   uint32_t cntcvup;
   /** Reserved space */
   uint8_t fill0[16];
   /** Base frequency ID. Frequency in number of ticks per second. You can specify up to 4GHz. at address offset 0x020, read-write */
   uint32_t cntfid0;
   /** Reserved space */
   uint8_t fill1[4012];
   /** Peripheral ID Register [39:32] at address offset 0xFD0, read-only constant 0x00000004 */
   uint32_t peripheralid4;
   /** Reserved space */
   uint8_t fill2[12];
   /** Peripheral ID Register [7:0] at address offset 0xFE0, read-only constant 0x00000001 */
   uint32_t peripheralid0;
   /** Peripheral ID Register [15:8] at address offset 0xFE4, read-only constant 0x000000B1 */
   uint32_t peripheralid1;
   /** Peripheral ID Register [23:16] at address offset 0xFE8, read-only constant 0x0000001B */
   uint32_t peripheralid2;
   /** Peripheral ID Register [31:24] at address offset 0xFEC, read-only constant 0x00000000 */
   uint32_t peripheralid3;
   /** Component ID0 Preamble at address offset 0xFF0, read-only constant 0x0000000D */
   uint32_t componentid0;
   /** Component ID1 Component class is CoreSight Component at address offset 0xFF4, read-only constant 0x000000F0 */
   uint32_t componentid1;
   /** Component ID2 Preamble at address offset 0xFF8, read-only constant 0x00000005 */
   uint32_t componentid2;
   /** Component ID3 Preamble at address offset 0xFFC, read-only constant 0x000000B1 */
   uint32_t componentid3;
};

/** bit field defines for cxtsgen_s#cntcr */
#define CXTSGEN_CNTCR_EN_OFFSET 0
#define CXTSGEN_CNTCR_EN_SIZE 1
#define CXTSGEN_CNTCR_HDBG_OFFSET 1
#define CXTSGEN_CNTCR_HDBG_SIZE 1

/** bit field defines for cxtsgen_s#cntsr */
#define CXTSGEN_CNTSR_DBGH_OFFSET 1
#define CXTSGEN_CNTSR_DBGH_SIZE 1

/** bit field defines for cxtsgen_s#cntcvlw */
#define CXTSGEN_CNTCVLW_CNTCVL_L_32_OFFSET 0
#define CXTSGEN_CNTCVLW_CNTCVL_L_32_SIZE 32

/** bit field defines for cxtsgen_s#cntcvup */
#define CXTSGEN_CNTCVUP_CNTCVU_U_32_OFFSET 0
#define CXTSGEN_CNTCVUP_CNTCVU_U_32_SIZE 32

/** bit field defines for cxtsgen_s#cntfid0 */
#define CXTSGEN_CNTFID0_FREQ_OFFSET 0
#define CXTSGEN_CNTFID0_FREQ_SIZE 32

/** bit field defines for cxtsgen_s#peripheralid4 */
#define CXTSGEN_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define CXTSGEN_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for cxtsgen_s#peripheralid0 */
#define CXTSGEN_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define CXTSGEN_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for cxtsgen_s#peripheralid1 */
#define CXTSGEN_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define CXTSGEN_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for cxtsgen_s#peripheralid2 */
#define CXTSGEN_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define CXTSGEN_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for cxtsgen_s#peripheralid3 */
#define CXTSGEN_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define CXTSGEN_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for cxtsgen_s#componentid0 */
#define CXTSGEN_COMPONENTID0_COMPONENTID0_OFFSET 0
#define CXTSGEN_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for cxtsgen_s#componentid1 */
#define CXTSGEN_COMPONENTID1_COMPONENTID1_OFFSET 0
#define CXTSGEN_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for cxtsgen_s#componentid2 */
#define CXTSGEN_COMPONENTID2_COMPONENTID2_OFFSET 0
#define CXTSGEN_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for cxtsgen_s#componentid3 */
#define CXTSGEN_COMPONENTID3_COMPONENTID3_OFFSET 0
#define CXTSGEN_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF cxtsgen.h */
#endif
