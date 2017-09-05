#ifndef INCLUDED_CTI
#define INCLUDED_CTI
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

#define CTI_DEVID 0x40800
#define CTI_DEVTYPE 0x14
#define CTI_PERIPHERALID4 0x04
#define CTI_PERIPHERALID0 0x06
#define CTI_PERIPHERALID1 0xB9
#define CTI_PERIPHERALID2 0x5B
#define CTI_PERIPHERALID3 0x00
#define CTI_COMPONENTID0 0x0D
#define CTI_COMPONENTID1 0x90
#define CTI_COMPONENTID2 0x05
#define CTI_COMPONENTID3 0xB1

/** Coresight Cross Trigger Interface
*/
struct cti_s {
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
   uint8_t fill3[3476];
   /** ITCHINACK Register at address offset 0xEDC, write-only */
   uint32_t itchinack;
   /** ITTRIGINACK Register at address offset 0xEE0, write-only */
   uint32_t ittriginack;
   /** ITCHOUT Register at address offset 0xEE4, write-only */
   uint32_t itchout;
   /** ITTRIGOUT Register at address offset 0xEE8, write-only */
   uint32_t ittrigout;
   /** ITCHOUTACK Register at address offset 0xEEC, read-only */
   uint32_t itchoutack;
   /** ITTRIGOUTACK Register at address offset 0xEF0, read-only */
   uint32_t ittrigoutack;
   /** ITCHIN Register at address offset 0xEF4, read-only */
   uint32_t itchin;
   /** ITTRIGIN Register at address offset 0xEF8, read-only */
   uint32_t ittrigin;
   /** Reserved space */
   uint8_t fill4[4];
   /** ECT CoreSight defined registers at address offset 0xF00, read-write */
   uint32_t itctrl;
   /** Reserved space */
   uint8_t fill5[156];
   /** Claim Tag Set Register at address offset 0xFA0, read-write */
   uint32_t claimset;
   /** Claim Tag Clear Register at address offset 0xFA4, read-write */
   uint32_t claimclr;
   /** Reserved space */
   uint8_t fill6[8];
   /** Lock Access register at address offset 0xFB0, write-only */
   uint32_t lockaccess;
   /** Lock Status Register at address offset 0xFB4, read-only */
   uint32_t lockstatus;
   /** Authentication Status Register at address offset 0xFB8, read-only */
   uint32_t authstatus;
   /** Reserved space */
   uint8_t fill7[12];
   /** Device configuration register at address offset 0xFC8, read-only constant 0x00040800 */
   uint32_t devid;
   /** Device type register at address offset 0xFCC, read-only constant 0x00000014 */
   uint32_t devtype;
   /** Peripheral ID Register [39:32] at address offset 0xFD0, read-only constant 0x00000004 */
   uint32_t peripheralid4;
   /** Reserved space */
   uint8_t fill8[12];
   /** Peripheral ID Register [7:0] at address offset 0xFE0, read-only constant 0x00000006 */
   uint32_t peripheralid0;
   /** Peripheral ID Register [15:8] at address offset 0xFE4, read-only constant 0x000000B9 */
   uint32_t peripheralid1;
   /** Peripheral ID Register [23:16] at address offset 0xFE8, read-only constant 0x0000005B */
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

/** bit field defines for cti_s#cticontrol */
#define CTI_CTICONTROL_GLBEN_OFFSET 0
#define CTI_CTICONTROL_GLBEN_SIZE 1

/** bit field defines for cti_s#ctiintack */
#define CTI_CTIINTACK_INTACK_OFFSET 0
#define CTI_CTIINTACK_INTACK_SIZE 8

/** bit field defines for cti_s#ctiappset */
#define CTI_CTIAPPSET_APPSET_OFFSET 0
#define CTI_CTIAPPSET_APPSET_SIZE 4

/** bit field defines for cti_s#ctiappclear */
#define CTI_CTIAPPCLEAR_APPCLEAR_OFFSET 0
#define CTI_CTIAPPCLEAR_APPCLEAR_SIZE 4

/** bit field defines for cti_s#ctiapppulse */
#define CTI_CTIAPPPULSE_APPULSE_OFFSET 0
#define CTI_CTIAPPPULSE_APPULSE_SIZE 4

/** bit field defines for cti_s#ctiinen */
#define CTI_CTIINEN_TRIGINEN_OFFSET 0
#define CTI_CTIINEN_TRIGINEN_SIZE 4

/** bit field defines for cti_s#ctiouten */
#define CTI_CTIOUTEN_TRIGOUTEN_OFFSET 0
#define CTI_CTIOUTEN_TRIGOUTEN_SIZE 4

/** bit field defines for cti_s#ctitriginstatus */
#define CTI_CTITRIGINSTATUS_TRIGINSTATUS_OFFSET 0
#define CTI_CTITRIGINSTATUS_TRIGINSTATUS_SIZE 8

/** bit field defines for cti_s#ctitrigoutstatus */
#define CTI_CTITRIGOUTSTATUS_TRIGOUTSTATUS_OFFSET 0
#define CTI_CTITRIGOUTSTATUS_TRIGOUTSTATUS_SIZE 8

/** bit field defines for cti_s#ctichinstatus */
#define CTI_CTICHINSTATUS_CTICHINSTATUS_OFFSET 0
#define CTI_CTICHINSTATUS_CTICHINSTATUS_SIZE 4

/** bit field defines for cti_s#ctichoutstatus */
#define CTI_CTICHOUTSTATUS_CTICHOUTSTATUS_OFFSET 0
#define CTI_CTICHOUTSTATUS_CTICHOUTSTATUS_SIZE 4

/** bit field defines for cti_s#ctigate */
#define CTI_CTIGATE_CTIGATEEN0_OFFSET 0
#define CTI_CTIGATE_CTIGATEEN0_SIZE 1
#define CTI_CTIGATE_CTIGATEEN1_OFFSET 1
#define CTI_CTIGATE_CTIGATEEN1_SIZE 1
#define CTI_CTIGATE_CTIGATEEN2_OFFSET 2
#define CTI_CTIGATE_CTIGATEEN2_SIZE 1
#define CTI_CTIGATE_CTIGATEEN3_OFFSET 3
#define CTI_CTIGATE_CTIGATEEN3_SIZE 1

/** bit field defines for cti_s#asicctl */
#define CTI_ASICCTL_ASICCTL_OFFSET 0
#define CTI_ASICCTL_ASICCTL_SIZE 8

/** bit field defines for cti_s#itchinack */
#define CTI_ITCHINACK_ITCHINACK_OFFSET 0
#define CTI_ITCHINACK_ITCHINACK_SIZE 4

/** bit field defines for cti_s#ittriginack */
#define CTI_ITTRIGINACK_ITTRIGINACK_OFFSET 0
#define CTI_ITTRIGINACK_ITTRIGINACK_SIZE 8

/** bit field defines for cti_s#itchout */
#define CTI_ITCHOUT_ITCHOUT_OFFSET 0
#define CTI_ITCHOUT_ITCHOUT_SIZE 4

/** bit field defines for cti_s#ittrigout */
#define CTI_ITTRIGOUT_ITTRIGOUT_OFFSET 0
#define CTI_ITTRIGOUT_ITTRIGOUT_SIZE 8

/** bit field defines for cti_s#itchoutack */
#define CTI_ITCHOUTACK_ITCHOUTACK_OFFSET 0
#define CTI_ITCHOUTACK_ITCHOUTACK_SIZE 4

/** bit field defines for cti_s#ittrigoutack */
#define CTI_ITTRIGOUTACK_ITTRIGOUTACK_OFFSET 0
#define CTI_ITTRIGOUTACK_ITTRIGOUTACK_SIZE 8

/** bit field defines for cti_s#itchin */
#define CTI_ITCHIN_ITCHIN_OFFSET 0
#define CTI_ITCHIN_ITCHIN_SIZE 4

/** bit field defines for cti_s#ittrigin */
#define CTI_ITTRIGIN_ITTRIGIN_OFFSET 0
#define CTI_ITTRIGIN_ITTRIGIN_SIZE 8

/** bit field defines for cti_s#itctrl */
#define CTI_ITCTRL_ITCTRL_OFFSET 0
#define CTI_ITCTRL_ITCTRL_SIZE 1

/** bit field defines for cti_s#claimset */
#define CTI_CLAIMSET_CLAIMSET_OFFSET 0
#define CTI_CLAIMSET_CLAIMSET_SIZE 4

/** bit field defines for cti_s#claimclr */
#define CTI_CLAIMCLR_CLAIMCLR_OFFSET 0
#define CTI_CLAIMCLR_CLAIMCLR_SIZE 4

/** bit field defines for cti_s#lockaccess */
#define CTI_LOCKACCESS_LOCKACCESS_OFFSET 0
#define CTI_LOCKACCESS_LOCKACCESS_SIZE 32

/** bit field defines for cti_s#lockstatus */
#define CTI_LOCKSTATUS_LOCKSTATUS_OFFSET 0
#define CTI_LOCKSTATUS_LOCKSTATUS_SIZE 2

/** bit field defines for cti_s#authstatus */
#define CTI_AUTHSTATUS_AUTHSTATUS_OFFSET 0
#define CTI_AUTHSTATUS_AUTHSTATUS_SIZE 4

/** bit field defines for cti_s#devid */
#define CTI_DEVID_EXTMUXNUM_OFFSET 0
#define CTI_DEVID_EXTMUXNUM_SIZE 5
#define CTI_DEVID_NUMTRIG_OFFSET 8
#define CTI_DEVID_NUMTRIG_SIZE 8
#define CTI_DEVID_NUMCH_OFFSET 16
#define CTI_DEVID_NUMCH_SIZE 4

/** bit field defines for cti_s#devtype */
#define CTI_DEVTYPE_MAJOR_OFFSET 0
#define CTI_DEVTYPE_MAJOR_SIZE 4
#define CTI_DEVTYPE_SUB_OFFSET 4
#define CTI_DEVTYPE_SUB_SIZE 4

/** bit field defines for cti_s#peripheralid4 */
#define CTI_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define CTI_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for cti_s#peripheralid0 */
#define CTI_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define CTI_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for cti_s#peripheralid1 */
#define CTI_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define CTI_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for cti_s#peripheralid2 */
#define CTI_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define CTI_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for cti_s#peripheralid3 */
#define CTI_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define CTI_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for cti_s#componentid0 */
#define CTI_COMPONENTID0_COMPONENTID0_OFFSET 0
#define CTI_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for cti_s#componentid1 */
#define CTI_COMPONENTID1_COMPONENTID1_OFFSET 0
#define CTI_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for cti_s#componentid2 */
#define CTI_COMPONENTID2_COMPONENTID2_OFFSET 0
#define CTI_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for cti_s#componentid3 */
#define CTI_COMPONENTID3_COMPONENTID3_OFFSET 0
#define CTI_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF cti.h */
#endif
