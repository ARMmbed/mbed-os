#ifndef INCLUDED_CXATBFUNNEL
#define INCLUDED_CXATBFUNNEL
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

#define CXATBFUNNEL_DEVID 0x38
#define CXATBFUNNEL_DEVTYPE 0x12
#define CXATBFUNNEL_PERIPHERALID4 0x04
#define CXATBFUNNEL_PERIPHERALID5 0x00
#define CXATBFUNNEL_PERIPHERALID6 0x00
#define CXATBFUNNEL_PERIPHERALID7 0x00
#define CXATBFUNNEL_PERIPHERALID0 0x08
#define CXATBFUNNEL_PERIPHERALID1 0xB9
#define CXATBFUNNEL_PERIPHERALID2 0x3B
#define CXATBFUNNEL_PERIPHERALID3 0x00
#define CXATBFUNNEL_COMPONENTID0 0x0D
#define CXATBFUNNEL_COMPONENTID1 0x90
#define CXATBFUNNEL_COMPONENTID2 0x05
#define CXATBFUNNEL_COMPONENTID3 0xB1

/** CoreSight Trace funnel
*/
struct cxatbfunnel_s {
   /** cxatbfunnel Control Register at address offset 0x000, read-write */
   uint32_t cstfctrl;
   /** cxatbfunnel Priority Register at address offset 0x004, read-write */
   uint32_t cstfprio;
   /** Reserved space */
   uint8_t fill0[3812];
   /** itatbdata0 integration test register at address offset 0xEEC, read-write */
   uint32_t itatbdata0;
   /** itatbctr2 integration test register at address offset 0xEF0, read-write */
   uint32_t itatbctr2;
   /** itatbctr1 integration test register at address offset 0xEF4, read-write */
   uint32_t itatbctr1;
   /** itatbctr0 integration test register at address offset 0xEF8, read-write */
   uint32_t itatbctr0;
   /** Reserved space */
   uint8_t fill1[4];
   /** Integration Mode Control Register at address offset 0xF00, read-write */
   uint32_t itmctrl;
   /** Reserved space */
   uint8_t fill2[156];
   /** Claim Tag Set Register at address offset 0xFA0, read-write */
   uint32_t claimset;
   /** Claim Tag Clear Register at address offset 0xFA4, read-write */
   uint32_t claimclr;
   /** Reserved space */
   uint8_t fill3[8];
   /** Lock Access register at address offset 0xFB0, write-only */
   uint32_t lockaccess;
   /** Lock Status Register at address offset 0xFB4, read-only */
   uint32_t lockstatus;
   /** Authentication Status Register at address offset 0xFB8, read-only */
   uint32_t authstatus;
   /** Reserved space */
   uint8_t fill4[12];
   /** Device configuration register at address offset 0xFC8, read-only constant 0x00000038 */
   uint32_t devid;
   /** Device type register at address offset 0xFCC, read-only constant 0x00000012 */
   uint32_t devtype;
   /** Peripheral ID Register at address offset 0xFD0, read-only constant 0x00000004 */
   uint32_t peripheralid4;
   /** Peripheral ID Register at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** Peripheral ID Register at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** Peripheral ID Register at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** Peripheral ID Register at address offset 0xFE0, read-only constant 0x00000008 */
   uint32_t peripheralid0;
   /** Peripheral ID Register at address offset 0xFE4, read-only constant 0x000000B9 */
   uint32_t peripheralid1;
   /** Peripheral ID Register at address offset 0xFE8, read-only constant 0x0000003B */
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

/** bit field defines for cxatbfunnel_s#cstfctrl */
#define CXATBFUNNEL_CSTFCTRL_ENS0_OFFSET 0
#define CXATBFUNNEL_CSTFCTRL_ENS0_SIZE 1
#define CXATBFUNNEL_CSTFCTRL_ENS1_OFFSET 1
#define CXATBFUNNEL_CSTFCTRL_ENS1_SIZE 1
#define CXATBFUNNEL_CSTFCTRL_ENS2_OFFSET 2
#define CXATBFUNNEL_CSTFCTRL_ENS2_SIZE 1
#define CXATBFUNNEL_CSTFCTRL_ENS3_OFFSET 3
#define CXATBFUNNEL_CSTFCTRL_ENS3_SIZE 1
#define CXATBFUNNEL_CSTFCTRL_ENS4_OFFSET 4
#define CXATBFUNNEL_CSTFCTRL_ENS4_SIZE 1
#define CXATBFUNNEL_CSTFCTRL_ENS5_OFFSET 5
#define CXATBFUNNEL_CSTFCTRL_ENS5_SIZE 1
#define CXATBFUNNEL_CSTFCTRL_ENS6_OFFSET 6
#define CXATBFUNNEL_CSTFCTRL_ENS6_SIZE 1
#define CXATBFUNNEL_CSTFCTRL_ENS7_OFFSET 7
#define CXATBFUNNEL_CSTFCTRL_ENS7_SIZE 1
#define CXATBFUNNEL_CSTFCTRL_HT_OFFSET 8
#define CXATBFUNNEL_CSTFCTRL_HT_SIZE 4

/** bit field defines for cxatbfunnel_s#cstfprio */
#define CXATBFUNNEL_CSTFPRIO_PRIPORT0_OFFSET 0
#define CXATBFUNNEL_CSTFPRIO_PRIPORT0_SIZE 3
#define CXATBFUNNEL_CSTFPRIO_PRIPORT1_OFFSET 3
#define CXATBFUNNEL_CSTFPRIO_PRIPORT1_SIZE 3
#define CXATBFUNNEL_CSTFPRIO_PRIPORT2_OFFSET 6
#define CXATBFUNNEL_CSTFPRIO_PRIPORT2_SIZE 3
#define CXATBFUNNEL_CSTFPRIO_PRIPORT3_OFFSET 9
#define CXATBFUNNEL_CSTFPRIO_PRIPORT3_SIZE 3
#define CXATBFUNNEL_CSTFPRIO_PRIPORT4_OFFSET 12
#define CXATBFUNNEL_CSTFPRIO_PRIPORT4_SIZE 3
#define CXATBFUNNEL_CSTFPRIO_PRIPORT5_OFFSET 15
#define CXATBFUNNEL_CSTFPRIO_PRIPORT5_SIZE 3
#define CXATBFUNNEL_CSTFPRIO_PRIPORT6_OFFSET 18
#define CXATBFUNNEL_CSTFPRIO_PRIPORT6_SIZE 3
#define CXATBFUNNEL_CSTFPRIO_PRIPORT7_OFFSET 21
#define CXATBFUNNEL_CSTFPRIO_PRIPORT7_SIZE 3

/** bit field defines for cxatbfunnel_s#itatbdata0 */
#define CXATBFUNNEL_ITATBDATA0_ITATBDATA0_OFFSET 0
#define CXATBFUNNEL_ITATBDATA0_ITATBDATA0_SIZE 5

/** bit field defines for cxatbfunnel_s#itatbctr2 */
#define CXATBFUNNEL_ITATBCTR2_ITATBCTR2_OFFSET 0
#define CXATBFUNNEL_ITATBCTR2_ITATBCTR2_SIZE 2

/** bit field defines for cxatbfunnel_s#itatbctr1 */
#define CXATBFUNNEL_ITATBCTR1_ITATBCTR1_OFFSET 0
#define CXATBFUNNEL_ITATBCTR1_ITATBCTR1_SIZE 7

/** bit field defines for cxatbfunnel_s#itatbctr0 */
#define CXATBFUNNEL_ITATBCTR0_ITATBCTR0_OFFSET 0
#define CXATBFUNNEL_ITATBCTR0_ITATBCTR0_SIZE 10

/** bit field defines for cxatbfunnel_s#itmctrl */
#define CXATBFUNNEL_ITMCTRL_ITMCTRL_OFFSET 0
#define CXATBFUNNEL_ITMCTRL_ITMCTRL_SIZE 1

/** bit field defines for cxatbfunnel_s#claimset */
#define CXATBFUNNEL_CLAIMSET_CLAIMSET_OFFSET 0
#define CXATBFUNNEL_CLAIMSET_CLAIMSET_SIZE 4

/** bit field defines for cxatbfunnel_s#claimclr */
#define CXATBFUNNEL_CLAIMCLR_CLAIMCLR_OFFSET 0
#define CXATBFUNNEL_CLAIMCLR_CLAIMCLR_SIZE 4

/** bit field defines for cxatbfunnel_s#lockaccess */
#define CXATBFUNNEL_LOCKACCESS_LOCKACCESS_OFFSET 0
#define CXATBFUNNEL_LOCKACCESS_LOCKACCESS_SIZE 32

/** bit field defines for cxatbfunnel_s#lockstatus */
#define CXATBFUNNEL_LOCKSTATUS_LOCKSTATUS_OFFSET 0
#define CXATBFUNNEL_LOCKSTATUS_LOCKSTATUS_SIZE 3

/** bit field defines for cxatbfunnel_s#authstatus */
#define CXATBFUNNEL_AUTHSTATUS_AUTHSTATUS_OFFSET 0
#define CXATBFUNNEL_AUTHSTATUS_AUTHSTATUS_SIZE 8

/** bit field defines for cxatbfunnel_s#devid */
#define CXATBFUNNEL_DEVID_PORTCOUNT_OFFSET 0
#define CXATBFUNNEL_DEVID_PORTCOUNT_SIZE 4
#define CXATBFUNNEL_DEVID_SCHEME_OFFSET 4
#define CXATBFUNNEL_DEVID_SCHEME_SIZE 4

/** bit field defines for cxatbfunnel_s#devtype */
#define CXATBFUNNEL_DEVTYPE_DEVTYPE_OFFSET 0
#define CXATBFUNNEL_DEVTYPE_DEVTYPE_SIZE 8

/** bit field defines for cxatbfunnel_s#peripheralid4 */
#define CXATBFUNNEL_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define CXATBFUNNEL_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for cxatbfunnel_s#peripheralid5 */
#define CXATBFUNNEL_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define CXATBFUNNEL_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for cxatbfunnel_s#peripheralid6 */
#define CXATBFUNNEL_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define CXATBFUNNEL_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for cxatbfunnel_s#peripheralid7 */
#define CXATBFUNNEL_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define CXATBFUNNEL_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for cxatbfunnel_s#peripheralid0 */
#define CXATBFUNNEL_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define CXATBFUNNEL_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for cxatbfunnel_s#peripheralid1 */
#define CXATBFUNNEL_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define CXATBFUNNEL_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for cxatbfunnel_s#peripheralid2 */
#define CXATBFUNNEL_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define CXATBFUNNEL_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for cxatbfunnel_s#peripheralid3 */
#define CXATBFUNNEL_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define CXATBFUNNEL_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for cxatbfunnel_s#componentid0 */
#define CXATBFUNNEL_COMPONENTID0_COMPONENTID0_OFFSET 0
#define CXATBFUNNEL_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for cxatbfunnel_s#componentid1 */
#define CXATBFUNNEL_COMPONENTID1_COMPONENTID1_OFFSET 0
#define CXATBFUNNEL_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for cxatbfunnel_s#componentid2 */
#define CXATBFUNNEL_COMPONENTID2_COMPONENTID2_OFFSET 0
#define CXATBFUNNEL_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for cxatbfunnel_s#componentid3 */
#define CXATBFUNNEL_COMPONENTID3_COMPONENTID3_OFFSET 0
#define CXATBFUNNEL_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF cxatbfunnel.h */
#endif
