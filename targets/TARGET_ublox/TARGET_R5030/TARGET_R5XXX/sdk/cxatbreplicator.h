#ifndef INCLUDED_CXATBREPLICATOR
#define INCLUDED_CXATBREPLICATOR
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

#define CXATBREPLICATOR_DEVID 0x02
#define CXATBREPLICATOR_DEVTYPE 0x22
#define CXATBREPLICATOR_PERIPHERALID4 0x04
#define CXATBREPLICATOR_PERIPHERALID0 0x09
#define CXATBREPLICATOR_PERIPHERALID1 0xB9
#define CXATBREPLICATOR_PERIPHERALID2 0x2B
#define CXATBREPLICATOR_PERIPHERALID3 0x00
#define CXATBREPLICATOR_COMPONENTID0 0x0D
#define CXATBREPLICATOR_COMPONENTID1 0x90
#define CXATBREPLICATOR_COMPONENTID2 0x05
#define CXATBREPLICATOR_COMPONENTID3 0xB1

/** CoreSight ATB Replicator
*/
struct cxatbreplicator_s {
   /** ID filtering for ATB master port 0 at address offset 0x000, read-write */
   uint32_t idfilter0;
   /** ID filtering for ATB master port 1 at address offset 0x004, read-write */
   uint32_t idfilter1;
   /** Reserved space */
   uint8_t fill0[3824];
   /** itatbctr1 integration test register at address offset 0xEF8, read-only */
   uint32_t itatbctr1;
   /** itatbctr0 integration test register at address offset 0xEFC, write-only */
   uint32_t itatbctr0;
   /** Integration Mode Control Register at address offset 0xF00, read-write */
   uint32_t itctrl;
   /** Reserved space */
   uint8_t fill1[156];
   /** Claim Tag Set Register at address offset 0xFA0, read-write */
   uint32_t claimset;
   /** Claim Tag Clear Register at address offset 0xFA4, read-write */
   uint32_t claimclr;
   /** Reserved space */
   uint8_t fill2[8];
   /** Lock Access register at address offset 0xFB0, write-only */
   uint32_t lockaccess;
   /** Lock Status Register at address offset 0xFB4, read-only */
   uint32_t lockstatus;
   /** Authentication Status Register at address offset 0xFB8, read-only */
   uint32_t authstatus;
   /** Reserved space */
   uint8_t fill3[12];
   /** Device configuration register at address offset 0xFC8, read-only constant 0x00000002 */
   uint32_t devid;
   /** Device type register at address offset 0xFCC, read-only constant 0x00000022 */
   uint32_t devtype;
   /** Peripheral ID Register at address offset 0xFD0, read-only constant 0x00000004 */
   uint32_t peripheralid4;
   /** Reserved space */
   uint8_t fill4[12];
   /** Peripheral ID Register at address offset 0xFE0, read-only constant 0x00000009 */
   uint32_t peripheralid0;
   /** Peripheral ID Register at address offset 0xFE4, read-only constant 0x000000B9 */
   uint32_t peripheralid1;
   /** Peripheral ID Register at address offset 0xFE8, read-only constant 0x0000002B */
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

/** bit field defines for cxatbreplicator_s#idfilter0 */
#define CXATBREPLICATOR_IDFILTER0_ID0_00_0F_OFFSET 0
#define CXATBREPLICATOR_IDFILTER0_ID0_00_0F_SIZE 1
#define CXATBREPLICATOR_IDFILTER0_ID0_10_1F_OFFSET 1
#define CXATBREPLICATOR_IDFILTER0_ID0_10_1F_SIZE 1
#define CXATBREPLICATOR_IDFILTER0_ID0_20_2F_OFFSET 2
#define CXATBREPLICATOR_IDFILTER0_ID0_20_2F_SIZE 1
#define CXATBREPLICATOR_IDFILTER0_ID0_30_3F_OFFSET 3
#define CXATBREPLICATOR_IDFILTER0_ID0_30_3F_SIZE 1
#define CXATBREPLICATOR_IDFILTER0_ID0_40_4F_OFFSET 4
#define CXATBREPLICATOR_IDFILTER0_ID0_40_4F_SIZE 1
#define CXATBREPLICATOR_IDFILTER0_ID0_50_5F_OFFSET 5
#define CXATBREPLICATOR_IDFILTER0_ID0_50_5F_SIZE 1
#define CXATBREPLICATOR_IDFILTER0_ID0_60_6F_OFFSET 6
#define CXATBREPLICATOR_IDFILTER0_ID0_60_6F_SIZE 1
#define CXATBREPLICATOR_IDFILTER0_ID0_70_7F_OFFSET 7
#define CXATBREPLICATOR_IDFILTER0_ID0_70_7F_SIZE 1

/** bit field defines for cxatbreplicator_s#idfilter1 */
#define CXATBREPLICATOR_IDFILTER1_ID1_00_0F_OFFSET 0
#define CXATBREPLICATOR_IDFILTER1_ID1_00_0F_SIZE 1
#define CXATBREPLICATOR_IDFILTER1_ID1_10_1F_OFFSET 1
#define CXATBREPLICATOR_IDFILTER1_ID1_10_1F_SIZE 1
#define CXATBREPLICATOR_IDFILTER1_ID1_20_2F_OFFSET 2
#define CXATBREPLICATOR_IDFILTER1_ID1_20_2F_SIZE 1
#define CXATBREPLICATOR_IDFILTER1_ID1_30_3F_OFFSET 3
#define CXATBREPLICATOR_IDFILTER1_ID1_30_3F_SIZE 1
#define CXATBREPLICATOR_IDFILTER1_ID1_40_4F_OFFSET 4
#define CXATBREPLICATOR_IDFILTER1_ID1_40_4F_SIZE 1
#define CXATBREPLICATOR_IDFILTER1_ID1_50_5F_OFFSET 5
#define CXATBREPLICATOR_IDFILTER1_ID1_50_5F_SIZE 1
#define CXATBREPLICATOR_IDFILTER1_ID1_60_6F_OFFSET 6
#define CXATBREPLICATOR_IDFILTER1_ID1_60_6F_SIZE 1
#define CXATBREPLICATOR_IDFILTER1_ID1_70_7F_OFFSET 7
#define CXATBREPLICATOR_IDFILTER1_ID1_70_7F_SIZE 1

/** bit field defines for cxatbreplicator_s#itatbctr1 */
#define CXATBREPLICATOR_ITATBCTR1_ATREADYM0_OFFSET 0
#define CXATBREPLICATOR_ITATBCTR1_ATREADYM0_SIZE 1
#define CXATBREPLICATOR_ITATBCTR1_ATREADYM1_OFFSET 1
#define CXATBREPLICATOR_ITATBCTR1_ATREADYM1_SIZE 1
#define CXATBREPLICATOR_ITATBCTR1_ATVALIDS_OFFSET 3
#define CXATBREPLICATOR_ITATBCTR1_ATVALIDS_SIZE 1

/** bit field defines for cxatbreplicator_s#itatbctr0 */
#define CXATBREPLICATOR_ITATBCTR0_ATVALIDM0_OFFSET 0
#define CXATBREPLICATOR_ITATBCTR0_ATVALIDM0_SIZE 1
#define CXATBREPLICATOR_ITATBCTR0_ATVALIDM1_OFFSET 2
#define CXATBREPLICATOR_ITATBCTR0_ATVALIDM1_SIZE 1
#define CXATBREPLICATOR_ITATBCTR0_ATREADYS_OFFSET 4
#define CXATBREPLICATOR_ITATBCTR0_ATREADYS_SIZE 1

/** bit field defines for cxatbreplicator_s#itctrl */
#define CXATBREPLICATOR_ITCTRL_IME_OFFSET 0
#define CXATBREPLICATOR_ITCTRL_IME_SIZE 1

/** bit field defines for cxatbreplicator_s#claimset */
#define CXATBREPLICATOR_CLAIMSET_CLAIMSET_OFFSET 0
#define CXATBREPLICATOR_CLAIMSET_CLAIMSET_SIZE 4

/** bit field defines for cxatbreplicator_s#claimclr */
#define CXATBREPLICATOR_CLAIMCLR_CLAIMCLR_OFFSET 0
#define CXATBREPLICATOR_CLAIMCLR_CLAIMCLR_SIZE 4

/** bit field defines for cxatbreplicator_s#lockaccess */
#define CXATBREPLICATOR_LOCKACCESS_LOCKACCESS_OFFSET 0
#define CXATBREPLICATOR_LOCKACCESS_LOCKACCESS_SIZE 32

/** bit field defines for cxatbreplicator_s#lockstatus */
#define CXATBREPLICATOR_LOCKSTATUS_LOCKSTATUS_OFFSET 0
#define CXATBREPLICATOR_LOCKSTATUS_LOCKSTATUS_SIZE 3

/** bit field defines for cxatbreplicator_s#authstatus */
#define CXATBREPLICATOR_AUTHSTATUS_AUTHSTATUS_OFFSET 0
#define CXATBREPLICATOR_AUTHSTATUS_AUTHSTATUS_SIZE 8

/** bit field defines for cxatbreplicator_s#devid */
#define CXATBREPLICATOR_DEVID_PORTNUM_OFFSET 0
#define CXATBREPLICATOR_DEVID_PORTNUM_SIZE 4

/** bit field defines for cxatbreplicator_s#devtype */
#define CXATBREPLICATOR_DEVTYPE_MAJOR_OFFSET 0
#define CXATBREPLICATOR_DEVTYPE_MAJOR_SIZE 4
#define CXATBREPLICATOR_DEVTYPE_SUB_OFFSET 4
#define CXATBREPLICATOR_DEVTYPE_SUB_SIZE 4

/** bit field defines for cxatbreplicator_s#peripheralid4 */
#define CXATBREPLICATOR_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define CXATBREPLICATOR_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for cxatbreplicator_s#peripheralid0 */
#define CXATBREPLICATOR_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define CXATBREPLICATOR_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for cxatbreplicator_s#peripheralid1 */
#define CXATBREPLICATOR_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define CXATBREPLICATOR_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for cxatbreplicator_s#peripheralid2 */
#define CXATBREPLICATOR_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define CXATBREPLICATOR_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for cxatbreplicator_s#peripheralid3 */
#define CXATBREPLICATOR_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define CXATBREPLICATOR_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for cxatbreplicator_s#componentid0 */
#define CXATBREPLICATOR_COMPONENTID0_COMPONENTID0_OFFSET 0
#define CXATBREPLICATOR_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for cxatbreplicator_s#componentid1 */
#define CXATBREPLICATOR_COMPONENTID1_COMPONENTID1_OFFSET 0
#define CXATBREPLICATOR_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for cxatbreplicator_s#componentid2 */
#define CXATBREPLICATOR_COMPONENTID2_COMPONENTID2_OFFSET 0
#define CXATBREPLICATOR_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for cxatbreplicator_s#componentid3 */
#define CXATBREPLICATOR_COMPONENTID3_COMPONENTID3_OFFSET 0
#define CXATBREPLICATOR_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF cxatbreplicator.h */
#endif
