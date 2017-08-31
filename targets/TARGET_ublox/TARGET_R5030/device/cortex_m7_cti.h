#ifndef INCLUDED_CORTEX_M7_CTI
#define INCLUDED_CORTEX_M7_CTI
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

#define CORTEX_M7_CTI_DEVID 0x40800
#define CORTEX_M7_CTI_DEVTYPE 0x14
#define CORTEX_M7_CTI_PERIPHERALID4 0x04
#define CORTEX_M7_CTI_PERIPHERALID5 0x00
#define CORTEX_M7_CTI_PERIPHERALID6 0x00
#define CORTEX_M7_CTI_PERIPHERALID7 0x00
#define CORTEX_M7_CTI_PERIPHERALID0 0x06
#define CORTEX_M7_CTI_PERIPHERALID1 0xB9
#define CORTEX_M7_CTI_PERIPHERALID2 0x4B
#define CORTEX_M7_CTI_PERIPHERALID3 0x00
#define CORTEX_M7_CTI_COMPONENTID0 0x0D
#define CORTEX_M7_CTI_COMPONENTID1 0x90
#define CORTEX_M7_CTI_COMPONENTID2 0x05
#define CORTEX_M7_CTI_COMPONENTID3 0xB1

/** Coresight Cross Trigger Interface
*/
struct cortex_m7_cti_s {
   /** CTI Control Register at address offset 0x000, read-write */
   uint32_t cticontrol;
   /** Reserved space */
   uint8_t fill0[12];
   /** CTI Interrupt Acknowledge Register at address offset 0x010, write-only */
   uint32_t ctiintack;
   /** CTI Application Trigger Set Register at address offset 0x014, read-write */
   uint32_t ctiappset;
   /** CTI Application Trigger Clear Register at address offset 0x018, write-only */
   uint32_t ctiappclear;
   /** CTI Application Pulse Register at address offset 0x01C, write-only */
   uint32_t ctiapppulse;
   /** CTI Trigger to Channel Enable Registers at address offset 0x020, read-write */
   uint32_t ctiinen[8];
   /** Reserved space */
   uint8_t fill1[96];
   /** CTI Channel to Trigger Enable Registers at address offset 0x0A0, read-write */
   uint32_t ctiouten[8];
   /** Reserved space */
   uint8_t fill2[112];
   /** CTI Trigger In Status Register at address offset 0x130, read-only */
   uint32_t ctitriginstatus;
   /** CTI Trigger Out Status Register at address offset 0x134, read-only */
   uint32_t ctitrigoutstatus;
   /** CTI Channel In Status Register at address offset 0x138, read-only */
   uint32_t ctichinstatus;
   /** CTI Channel Out Status Register at address offset 0x13C, read-only */
   uint32_t ctichoutstatus;
   /** CTI Channel Gate Register at address offset 0x140, read-write */
   uint32_t ctigate;
   /** External Multiplexor Control Register at address offset 0x144, read-write */
   uint32_t asicctl;
   /** Reserved space */
   uint8_t fill3[3512];
   /** ECT CoreSight defined registers at address offset 0xF00, read-write */
   uint32_t itctrl;
   /** Reserved space */
   uint8_t fill4[156];
   /** Claim Tag Set Register at address offset 0xFA0, read-write */
   uint32_t claimset;
   /** Claim Tag Clear Register at address offset 0xFA4, read-write */
   uint32_t claimclr;
   /** Reserved space */
   uint8_t fill5[8];
   /** Lock Access register at address offset 0xFB0, write-only */
   uint32_t lockaccess;
   /** Lock Status Register at address offset 0xFB4, read-only */
   uint32_t lockstatus;
   /** Authentication Status Register at address offset 0xFB8, read-only */
   uint32_t authstatus;
   /** Reserved space */
   uint8_t fill6[12];
   /** Device configuration register at address offset 0xFC8, read-only constant 0x00040800 */
   uint32_t devid;
   /** Device type register at address offset 0xFCC, read-only constant 0x00000014 */
   uint32_t devtype;
   /** Peripheral ID Register [39:32] at address offset 0xFD0, read-only constant 0x00000004 */
   uint32_t peripheralid4;
   /** Peripheral ID Register [47:40] at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** Peripheral ID Register [55:48] at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** Peripheral ID Register [63:56] at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** Peripheral ID Register [7:0] at address offset 0xFE0, read-only constant 0x00000006 */
   uint32_t peripheralid0;
   /** Peripheral ID Register [15:8] at address offset 0xFE4, read-only constant 0x000000B9 */
   uint32_t peripheralid1;
   /** Peripheral ID Register [23:16] at address offset 0xFE8, read-only constant 0x0000004B */
   uint32_t peripheralid2;
   /** Peripheral ID Register [31:24] at address offset 0xFEC, read-only constant 0x00000000 */
   uint32_t peripheralid3;
   /** Preamble at address offset 0xFF0, read-only constant 0x0000000D */
   uint32_t componentid0;
   /** Component class is CoreSight Component at address offset 0xFF4, read-only constant 0x00000090 */
   uint32_t componentid1;
   /** Preamble at address offset 0xFF8, read-only constant 0x00000005 */
   uint32_t componentid2;
   /** Preamble at address offset 0xFFC, read-only constant 0x000000B1 */
   uint32_t componentid3;
};

/** bit field defines for cortex_m7_cti_s#cticontrol */
#define CORTEX_M7_CTI_CTICONTROL_GLBEN_OFFSET 0
#define CORTEX_M7_CTI_CTICONTROL_GLBEN_SIZE 1

/** bit field defines for cortex_m7_cti_s#ctiintack */
#define CORTEX_M7_CTI_CTIINTACK_INTACK_OFFSET 0
#define CORTEX_M7_CTI_CTIINTACK_INTACK_SIZE 8

/** bit field defines for cortex_m7_cti_s#ctiappset */
#define CORTEX_M7_CTI_CTIAPPSET_APPSET_OFFSET 0
#define CORTEX_M7_CTI_CTIAPPSET_APPSET_SIZE 4

/** bit field defines for cortex_m7_cti_s#ctiappclear */
#define CORTEX_M7_CTI_CTIAPPCLEAR_APPCLEAR_OFFSET 0
#define CORTEX_M7_CTI_CTIAPPCLEAR_APPCLEAR_SIZE 4

/** bit field defines for cortex_m7_cti_s#ctiapppulse */
#define CORTEX_M7_CTI_CTIAPPPULSE_APPULSE_OFFSET 0
#define CORTEX_M7_CTI_CTIAPPPULSE_APPULSE_SIZE 4

/** bit field defines for cortex_m7_cti_s#ctiinen */
#define CORTEX_M7_CTI_CTIINEN_TRIGINEN_OFFSET 0
#define CORTEX_M7_CTI_CTIINEN_TRIGINEN_SIZE 4

/** bit field defines for cortex_m7_cti_s#ctiouten */
#define CORTEX_M7_CTI_CTIOUTEN_TRIGOUTEN_OFFSET 0
#define CORTEX_M7_CTI_CTIOUTEN_TRIGOUTEN_SIZE 4

/** bit field defines for cortex_m7_cti_s#ctitriginstatus */
#define CORTEX_M7_CTI_CTITRIGINSTATUS_TRIGINSTATUS_OFFSET 0
#define CORTEX_M7_CTI_CTITRIGINSTATUS_TRIGINSTATUS_SIZE 8

/** bit field defines for cortex_m7_cti_s#ctitrigoutstatus */
#define CORTEX_M7_CTI_CTITRIGOUTSTATUS_TRIGOUTSTATUS_OFFSET 0
#define CORTEX_M7_CTI_CTITRIGOUTSTATUS_TRIGOUTSTATUS_SIZE 8

/** bit field defines for cortex_m7_cti_s#ctichinstatus */
#define CORTEX_M7_CTI_CTICHINSTATUS_CTICHINSTATUS_OFFSET 0
#define CORTEX_M7_CTI_CTICHINSTATUS_CTICHINSTATUS_SIZE 4

/** bit field defines for cortex_m7_cti_s#ctichoutstatus */
#define CORTEX_M7_CTI_CTICHOUTSTATUS_CTICHOUTSTATUS_OFFSET 0
#define CORTEX_M7_CTI_CTICHOUTSTATUS_CTICHOUTSTATUS_SIZE 4

/** bit field defines for cortex_m7_cti_s#ctigate */
#define CORTEX_M7_CTI_CTIGATE_CTIGATEEN0_OFFSET 0
#define CORTEX_M7_CTI_CTIGATE_CTIGATEEN0_SIZE 1
#define CORTEX_M7_CTI_CTIGATE_CTIGATEEN1_OFFSET 1
#define CORTEX_M7_CTI_CTIGATE_CTIGATEEN1_SIZE 1
#define CORTEX_M7_CTI_CTIGATE_CTIGATEEN2_OFFSET 2
#define CORTEX_M7_CTI_CTIGATE_CTIGATEEN2_SIZE 1
#define CORTEX_M7_CTI_CTIGATE_CTIGATEEN3_OFFSET 3
#define CORTEX_M7_CTI_CTIGATE_CTIGATEEN3_SIZE 1

/** bit field defines for cortex_m7_cti_s#asicctl */
#define CORTEX_M7_CTI_ASICCTL_ASICCTL_OFFSET 0
#define CORTEX_M7_CTI_ASICCTL_ASICCTL_SIZE 8

/** bit field defines for cortex_m7_cti_s#itctrl */
#define CORTEX_M7_CTI_ITCTRL_ITCTRL_OFFSET 0
#define CORTEX_M7_CTI_ITCTRL_ITCTRL_SIZE 1

/** bit field defines for cortex_m7_cti_s#claimset */
#define CORTEX_M7_CTI_CLAIMSET_CLAIMSET_OFFSET 0
#define CORTEX_M7_CTI_CLAIMSET_CLAIMSET_SIZE 4

/** bit field defines for cortex_m7_cti_s#claimclr */
#define CORTEX_M7_CTI_CLAIMCLR_CLAIMCLR_OFFSET 0
#define CORTEX_M7_CTI_CLAIMCLR_CLAIMCLR_SIZE 4

/** bit field defines for cortex_m7_cti_s#lockaccess */
#define CORTEX_M7_CTI_LOCKACCESS_LOCKACCESS_OFFSET 0
#define CORTEX_M7_CTI_LOCKACCESS_LOCKACCESS_SIZE 32

/** bit field defines for cortex_m7_cti_s#lockstatus */
#define CORTEX_M7_CTI_LOCKSTATUS_LOCKSTATUS_OFFSET 0
#define CORTEX_M7_CTI_LOCKSTATUS_LOCKSTATUS_SIZE 2

/** bit field defines for cortex_m7_cti_s#authstatus */
#define CORTEX_M7_CTI_AUTHSTATUS_AUTHSTATUS_OFFSET 0
#define CORTEX_M7_CTI_AUTHSTATUS_AUTHSTATUS_SIZE 4

/** bit field defines for cortex_m7_cti_s#devid */
#define CORTEX_M7_CTI_DEVID_EXTMUXNUM_OFFSET 0
#define CORTEX_M7_CTI_DEVID_EXTMUXNUM_SIZE 5
#define CORTEX_M7_CTI_DEVID_NUMTRIG_OFFSET 8
#define CORTEX_M7_CTI_DEVID_NUMTRIG_SIZE 8
#define CORTEX_M7_CTI_DEVID_NUMCH_OFFSET 16
#define CORTEX_M7_CTI_DEVID_NUMCH_SIZE 4

/** bit field defines for cortex_m7_cti_s#devtype */
#define CORTEX_M7_CTI_DEVTYPE_MAJOR_OFFSET 0
#define CORTEX_M7_CTI_DEVTYPE_MAJOR_SIZE 4
#define CORTEX_M7_CTI_DEVTYPE_SUB_OFFSET 4
#define CORTEX_M7_CTI_DEVTYPE_SUB_SIZE 4

/** bit field defines for cortex_m7_cti_s#peripheralid4 */
#define CORTEX_M7_CTI_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define CORTEX_M7_CTI_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for cortex_m7_cti_s#peripheralid5 */
#define CORTEX_M7_CTI_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define CORTEX_M7_CTI_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for cortex_m7_cti_s#peripheralid6 */
#define CORTEX_M7_CTI_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define CORTEX_M7_CTI_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for cortex_m7_cti_s#peripheralid7 */
#define CORTEX_M7_CTI_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define CORTEX_M7_CTI_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for cortex_m7_cti_s#peripheralid0 */
#define CORTEX_M7_CTI_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define CORTEX_M7_CTI_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for cortex_m7_cti_s#peripheralid1 */
#define CORTEX_M7_CTI_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define CORTEX_M7_CTI_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for cortex_m7_cti_s#peripheralid2 */
#define CORTEX_M7_CTI_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define CORTEX_M7_CTI_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for cortex_m7_cti_s#peripheralid3 */
#define CORTEX_M7_CTI_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define CORTEX_M7_CTI_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for cortex_m7_cti_s#componentid0 */
#define CORTEX_M7_CTI_COMPONENTID0_COMPONENTID0_OFFSET 0
#define CORTEX_M7_CTI_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for cortex_m7_cti_s#componentid1 */
#define CORTEX_M7_CTI_COMPONENTID1_COMPONENTID1_OFFSET 0
#define CORTEX_M7_CTI_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for cortex_m7_cti_s#componentid2 */
#define CORTEX_M7_CTI_COMPONENTID2_COMPONENTID2_OFFSET 0
#define CORTEX_M7_CTI_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for cortex_m7_cti_s#componentid3 */
#define CORTEX_M7_CTI_COMPONENTID3_COMPONENTID3_OFFSET 0
#define CORTEX_M7_CTI_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF cortex_m7_cti.h */
#endif
