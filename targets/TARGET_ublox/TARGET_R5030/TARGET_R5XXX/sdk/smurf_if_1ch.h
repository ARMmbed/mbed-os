#ifndef INCLUDED_SMURF_IF_1CH
#define INCLUDED_SMURF_IF_1CH
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

#define SMURF_IF_1CH_PERIPHERALID4 0x07
#define SMURF_IF_1CH_PERIPHERALID5 0x00
#define SMURF_IF_1CH_PERIPHERALID6 0x00
#define SMURF_IF_1CH_PERIPHERALID7 0x00
#define SMURF_IF_1CH_PERIPHERALID0 0x65
#define SMURF_IF_1CH_PERIPHERALID1 0xDA
#define SMURF_IF_1CH_PERIPHERALID2 0x08
#define SMURF_IF_1CH_PERIPHERALID3 0x00
#define SMURF_IF_1CH_COMPONENTID0 0x0D
#define SMURF_IF_1CH_COMPONENTID1 0xF0
#define SMURF_IF_1CH_COMPONENTID2 0x05
#define SMURF_IF_1CH_COMPONENTID3 0xB1

/** Simple MUltichannel RF interface (SMURF).  The SMURF is the physical interface between the baseband and the transceiver and consists of 2 separate components that need to be used in pairs.
*/
struct smurf_if_1ch_s {
   /** SMURF Control and Configuration Register at address offset 0x000, read-write */
   uint32_t ctrl;
   /** Reserved space */
   uint8_t fill0[764];
   /** Clear Fault Status at address offset 0x300, write-only */
   uint32_t fltclear;
   /** Fault Status Register at address offset 0x304, read-only */
   uint32_t fltstatus;
   /** Raw Fault Status Register at address offset 0x308, read-only */
   uint32_t fltrawstatus;
   /** Fault Mask Clear at address offset 0x30C, write-only */
   uint32_t fltmaskclr;
   /** Fault Mask Set at address offset 0x310, write-only */
   uint32_t fltmaskset;
   /** Fault Mask Status at address offset 0x314, read-only */
   uint32_t fltmaskstatus;
   /** Reserved space */
   uint8_t fill1[3256];
   /** SMURF Data IF Peripheral ID4 Register at address offset 0xFD0, read-only constant 0x00000007 */
   uint32_t peripheralid4;
   /** SMURF Data IF Peripheral ID5 Register at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** SMURF Data IF Peripheral ID6 Register at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** SMURF Data IF Peripheral ID7 Register at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** SMURF Data IF Peripheral ID0 Register at address offset 0xFE0, read-only constant 0x00000065 */
   uint32_t peripheralid0;
   /** SMURF Data IF Peripheral ID1 Register at address offset 0xFE4, read-only constant 0x000000DA */
   uint32_t peripheralid1;
   /** SMURF Data IF Peripheral ID2 Register at address offset 0xFE8, read-only constant 0x00000008 */
   uint32_t peripheralid2;
   /** SMURF Data IF Peripheral ID3 Register at address offset 0xFEC, read-only constant 0x00000000 */
   uint32_t peripheralid3;
   /** SMURF Data IF Component ID0 Register at address offset 0xFF0, read-only constant 0x0000000D */
   uint32_t componentid0;
   /** SMURF Data IF Component ID1 Register at address offset 0xFF4, read-only constant 0x000000F0 */
   uint32_t componentid1;
   /** SMURF Data IF Component ID2 Register at address offset 0xFF8, read-only constant 0x00000005 */
   uint32_t componentid2;
   /** SMURF Data IF Component ID3 Register at address offset 0xFFC, read-only constant 0x000000B1 */
   uint32_t componentid3;
};

/** bit field defines for smurf_if_1ch_s#ctrl */
#define SMURF_IF_1CH_CTRL_LOOPBACK_MODE_OFFSET 0
#define SMURF_IF_1CH_CTRL_LOOPBACK_MODE_SIZE 1

/** bit field defines for smurf_if_1ch_s#fltclear */
#define SMURF_IF_1CH_FLTCLEAR_FLTCLEAR_OFFSET 0
#define SMURF_IF_1CH_FLTCLEAR_FLTCLEAR_SIZE 2

/** bit field defines for smurf_if_1ch_s#fltstatus */
#define SMURF_IF_1CH_FLTSTATUS_FLTSTATUS_OFFSET 0
#define SMURF_IF_1CH_FLTSTATUS_FLTSTATUS_SIZE 2

/** bit field defines for smurf_if_1ch_s#fltrawstatus */
#define SMURF_IF_1CH_FLTRAWSTATUS_FLTRAWSTATUS_OFFSET 0
#define SMURF_IF_1CH_FLTRAWSTATUS_FLTRAWSTATUS_SIZE 2

/** bit field defines for smurf_if_1ch_s#fltmaskclr */
#define SMURF_IF_1CH_FLTMASKCLR_FLTMASKCLR_OFFSET 0
#define SMURF_IF_1CH_FLTMASKCLR_FLTMASKCLR_SIZE 2

/** bit field defines for smurf_if_1ch_s#fltmaskset */
#define SMURF_IF_1CH_FLTMASKSET_FLTMASKSET_OFFSET 0
#define SMURF_IF_1CH_FLTMASKSET_FLTMASKSET_SIZE 2

/** bit field defines for smurf_if_1ch_s#fltmaskstatus */
#define SMURF_IF_1CH_FLTMASKSTATUS_FLTMASKSTATUS_OFFSET 0
#define SMURF_IF_1CH_FLTMASKSTATUS_FLTMASKSTATUS_SIZE 2

/** bit field defines for smurf_if_1ch_s#peripheralid4 */
#define SMURF_IF_1CH_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define SMURF_IF_1CH_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for smurf_if_1ch_s#peripheralid5 */
#define SMURF_IF_1CH_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define SMURF_IF_1CH_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for smurf_if_1ch_s#peripheralid6 */
#define SMURF_IF_1CH_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define SMURF_IF_1CH_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for smurf_if_1ch_s#peripheralid7 */
#define SMURF_IF_1CH_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define SMURF_IF_1CH_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for smurf_if_1ch_s#peripheralid0 */
#define SMURF_IF_1CH_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define SMURF_IF_1CH_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for smurf_if_1ch_s#peripheralid1 */
#define SMURF_IF_1CH_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define SMURF_IF_1CH_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for smurf_if_1ch_s#peripheralid2 */
#define SMURF_IF_1CH_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define SMURF_IF_1CH_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for smurf_if_1ch_s#peripheralid3 */
#define SMURF_IF_1CH_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define SMURF_IF_1CH_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for smurf_if_1ch_s#componentid0 */
#define SMURF_IF_1CH_COMPONENTID0_COMPONENTID0_OFFSET 0
#define SMURF_IF_1CH_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for smurf_if_1ch_s#componentid1 */
#define SMURF_IF_1CH_COMPONENTID1_COMPONENTID1_OFFSET 0
#define SMURF_IF_1CH_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for smurf_if_1ch_s#componentid2 */
#define SMURF_IF_1CH_COMPONENTID2_COMPONENTID2_OFFSET 0
#define SMURF_IF_1CH_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for smurf_if_1ch_s#componentid3 */
#define SMURF_IF_1CH_COMPONENTID3_COMPONENTID3_OFFSET 0
#define SMURF_IF_1CH_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF smurf_if_1ch.h */
#endif
