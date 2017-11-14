#ifndef INCLUDED_CORTEX_M7_ETM
#define INCLUDED_CORTEX_M7_ETM
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>


/** Embedded trace Macrocell registers
*/
struct cortex_m7_etm_s {
   /** Reserved space */
   uint8_t fill0[4];
   /** Programming Control Register at address offset 0x004, read-write */
   uint32_t trcprgctlr;
   /** Processor Select Control Register at address offset 0x008, read-write */
   uint32_t trcprocselr;
   /** Status Register at address offset 0x00C, read-only */
   uint32_t trcstatr;
   /** Trace Configuration Register at address offset 0x010, read-write */
   uint32_t trcconfigr;
   /** Reserved space */
   uint8_t fill1[12];
   /** Event Control 0 Register at address offset 0x020, read-write */
   uint32_t trceventctl0r;
   /** Event Control 1 Register at address offset 0x024, read-write */
   uint32_t trceventctl1r;
   /** Reserved space */
   uint8_t fill2[4];
   /** Stall Control Register at address offset 0x02C, read-write */
   uint32_t trcstallctlr;
   /** Global Timestamp Control Register at address offset 0x030, read-write */
   uint32_t trctsctlr;
   /** Synchronization Period Register at address offset 0x034, read-write */
   uint32_t trcsyncpr;
   /** Cycle Count Control Register at address offset 0x038, read-write */
   uint32_t trcccctlr;
   /** Branch Broadcast Control Register at address offset 0x03C, read-write */
   uint32_t trcbbctlr;
   /** Trace ID Register at address offset 0x040, read-write */
   uint32_t trctraceidr;
   /** Reserved space */
   uint8_t fill3[60];
   /** ViewInst Main Control Register at address offset 0x080, read-write */
   uint32_t trcvictlr;
   /** ViewInst Include-Exclude Control Register at address offset 0x084, read-write */
   uint32_t trcviiectlr;
   /** ViewInst Start-Stop Control Register at address offset 0x088, read-write */
   uint32_t trcvissctlr;
   /** ViewInst Start-Stop Processor Comparator Control Register at address offset 0x08C, read-write */
   uint32_t trcvipcssctlr;
   /** Reserved space */
   uint8_t fill4[16];
   /** ViewData Main Control Register at address offset 0x0A0, read-write */
   uint32_t trcvdctlr;
   /** ViewData Include/Exclude Single Address Comparator Register at address offset 0x0A4, read-write */
   uint32_t trcvdsacctlr;
   /** ViewData Include/Exclude Address Range Comparator Register at address offset 0x0A8, read-write */
   uint32_t trcvdarcctlr;
   /** Reserved space */
   uint8_t fill5[84];
   /** Sequencer State Transition Control Registers 0 at address offset 0x100, read-write */
   uint32_t trcseqevr0;
   /** Sequencer State Transition Control Registers 1 at address offset 0x104, read-write */
   uint32_t trcseqevr1;
   /** Sequencer State Transition Control Registers 2 at address offset 0x108, read-write */
   uint32_t trcseqevr2;
   /** Reserved space */
   uint8_t fill6[12];
   /** Sequencer Reset Control Register at address offset 0x118, read-write */
   uint32_t trcseqrstevr;
   /** Sequencer State Register at address offset 0x11C, read-write */
   uint32_t trcseqstr;
   /** External Input Select Register at address offset 0x120, read-write */
   uint32_t trcextinselr;
   /** Reserved space */
   uint8_t fill7[28];
   /** Counter Reload Value Registers 0 at address offset 0x140, read-write */
   uint32_t trccntrldvr0;
   /** Counter Reload Value Registers 1 at address offset 0x144, read-write */
   uint32_t trccntrldvr1;
   /** Reserved space */
   uint8_t fill8[8];
   /** Counter Control Register 0 at address offset 0x150, read-write */
   uint32_t trccntctlr0;
   /** Counter Control Register 1 at address offset 0x154, read-write */
   uint32_t trccntctlr1;
   /** Reserved space */
   uint8_t fill9[8];
   /** Counter Value Registers 0 at address offset 0x160, read-write */
   uint32_t trccntvr0;
   /** Counter Value Registers 1 at address offset 0x164, read-write */
   uint32_t trccntvr1;
   /** Reserved space */
   uint8_t fill10[24];
   /** ID Register 8 at address offset 0x180, read-only */
   uint32_t trcidr8;
   /** ID Register 9 at address offset 0x184, read-only */
   uint32_t trcidr9;
   /** ID Register 10 at address offset 0x188, read-only */
   uint32_t trcidr10;
   /** ID Register 11 at address offset 0x18C, read-only */
   uint32_t trcidr11;
   /** ID Register 12 at address offset 0x190, read-only */
   uint32_t trcidr12;
   /** ID Register 13 at address offset 0x194, read-only */
   uint32_t trcidr13;
   /** Reserved space */
   uint8_t fill11[40];
   /** Implementation Specific Register 0 at address offset 0x1C0, read-write */
   uint32_t trcimspec0;
   /** Reserved space */
   uint8_t fill12[28];
   /** ID Register 0 at address offset 0x1E0, read-only */
   uint32_t trcidr0;
   /** ID Register 1 at address offset 0x1E4, read-only */
   uint32_t trcidr1;
   /** ID Register 2 at address offset 0x1E8, read-only */
   uint32_t trcidr2;
   /** ID Register 3 at address offset 0x1EC, read-only */
   uint32_t trcidr3;
   /** ID Register 4 at address offset 0x1F0, read-only */
   uint32_t trcidr4;
   /** ID Register 5 at address offset 0x1F4, read-only */
   uint32_t trcidr5;
   /** Reserved space */
   uint8_t fill13[16];
   /** Resource Selection Control Registers 2 at address offset 0x208, read-write */
   uint32_t trcrsctlr2;
   /** Resource Selection Control Registers 3 at address offset 0x20C, read-write */
   uint32_t trcrsctlr3;
   /** Resource Selection Control Registers 4 at address offset 0x210, read-write */
   uint32_t trcrsctlr4;
   /** Resource Selection Control Registers 5 at address offset 0x214, read-write */
   uint32_t trcrsctlr5;
   /** Resource Selection Control Registers 6 at address offset 0x218, read-write */
   uint32_t trcrsctlr6;
   /** Resource Selection Control Registers 7 at address offset 0x21C, read-write */
   uint32_t trcrsctlr7;
   /** Resource Selection Control Registers 8 at address offset 0x220, read-write */
   uint32_t trcrsctlr8;
   /** Resource Selection Control Registers 9 at address offset 0x224, read-write */
   uint32_t trcrsctlr9;
   /** Resource Selection Control Registers 10 at address offset 0x228, read-write */
   uint32_t trcrsctlr10;
   /** Resource Selection Control Registers 11 at address offset 0x22C, read-write */
   uint32_t trcrsctlr11;
   /** Resource Selection Control Registers 12 at address offset 0x230, read-write */
   uint32_t trcrsctlr12;
   /** Resource Selection Control Registers 13 at address offset 0x234, read-write */
   uint32_t trcrsctlr13;
   /** Resource Selection Control Registers 14 at address offset 0x238, read-write */
   uint32_t trcrsctlr14;
   /** Resource Selection Control Registers 15 at address offset 0x23C, read-write */
   uint32_t trcrsctlr15;
   /** Reserved space */
   uint8_t fill14[64];
   /** Single-Shot Comparator Control Register 0 at address offset 0x280, read-write */
   uint32_t trcssccr0;
   /** Reserved space */
   uint8_t fill15[28];
   /** Single-Shot Comparator Status Register 0 at address offset 0x2A0, read-write */
   uint32_t trcsscsr0;
   /** Reserved space */
   uint8_t fill16[28];
   /** Single-shot Processor Comparator Input Control Register 0 at address offset 0x2C0, read-write */
   uint32_t trcsspcicr0;
   /** Reserved space */
   uint8_t fill17[64];
   /** OS Lock Status Register at address offset 0x304, read-only */
   uint32_t trcoslsr;
   /** Reserved space */
   uint8_t fill18[8];
   /** Power Down Control Register at address offset 0x310, read-write */
   uint32_t trcpdcr;
   /** Power Down Status Register at address offset 0x314, read-only */
   uint32_t trcpdsr;
   /** Reserved space */
   uint8_t fill19[232];
   /** Address Comparator Value Registers 0 (low word) at address offset 0x400, read-write */
   uint32_t trcacvr0_31_0;
   /** Reserved space */
   uint8_t fill20[4];
   /** Address Comparator Value Registers 1 (low word) at address offset 0x408, read-write */
   uint32_t trcacvr1_31_0;
   /** Reserved space */
   uint8_t fill21[4];
   /** Address Comparator Value Registers 2 (low word) at address offset 0x410, read-write */
   uint32_t trcacvr2_31_0;
   /** Reserved space */
   uint8_t fill22[4];
   /** Address Comparator Value Registers 3 (low word) at address offset 0x418, read-write */
   uint32_t trcacvr3_31_0;
   /** Reserved space */
   uint8_t fill23[4];
   /** Address Comparator Value Registers 4 (low word) at address offset 0x420, read-write */
   uint32_t trcacvr4_31_0;
   /** Reserved space */
   uint8_t fill24[4];
   /** Address Comparator Value Registers 5 (low word) at address offset 0x428, read-write */
   uint32_t trcacvr5_31_0;
   /** Reserved space */
   uint8_t fill25[4];
   /** Address Comparator Value Registers 6 (low word) at address offset 0x430, read-write */
   uint32_t trcacvr6_31_0;
   /** Reserved space */
   uint8_t fill26[4];
   /** Address Comparator Value Registers 7 (low word) at address offset 0x438, read-write */
   uint32_t trcacvr7_31_0;
   /** Reserved space */
   uint8_t fill27[68];
   /** Address Comparator Access Type Registers 0 at address offset 0x480, read-write */
   uint32_t trcacatr0;
   /** Reserved space */
   uint8_t fill28[4];
   /** Address Comparator Access Type Registers 1 at address offset 0x488, read-write */
   uint32_t trcacatr1;
   /** Reserved space */
   uint8_t fill29[4];
   /** Address Comparator Access Type Registers 2 at address offset 0x490, read-write */
   uint32_t trcacatr2;
   /** Reserved space */
   uint8_t fill30[4];
   /** Address Comparator Access Type Registers 3 at address offset 0x498, read-write */
   uint32_t trcacatr3;
   /** Reserved space */
   uint8_t fill31[4];
   /** Address Comparator Access Type Registers 4 at address offset 0x4A0, read-write */
   uint32_t trcacatr4;
   /** Reserved space */
   uint8_t fill32[4];
   /** Address Comparator Access Type Registers 5 at address offset 0x4A8, read-write */
   uint32_t trcacatr5;
   /** Reserved space */
   uint8_t fill33[4];
   /** Address Comparator Access Type Registers 6 at address offset 0x4B0, read-write */
   uint32_t trcacatr6;
   /** Reserved space */
   uint8_t fill34[4];
   /** Address Comparator Access Type Registers 7 at address offset 0x4B8, read-write */
   uint32_t trcacatr7;
   /** Reserved space */
   uint8_t fill35[68];
   /** Data Value Comparator Value Register 0 at address offset 0x500, read-write */
   uint32_t trcdvcvr0;
   /** Reserved space */
   uint8_t fill36[12];
   /** Data Value Comparator Value Register 1 at address offset 0x510, read-write */
   uint32_t trcdvcvr1;
   /** Reserved space */
   uint8_t fill37[108];
   /** Data Value Comparator Mask Register 0 at address offset 0x580, read-write */
   uint32_t trcdvcmr0;
   /** Reserved space */
   uint8_t fill38[12];
   /** Data Value Comparator Mask Register 1 at address offset 0x590, read-write */
   uint32_t trcdvcmr1;
   /** Reserved space */
   uint8_t fill39[2376];
   /** Integration Miscellaneous Outputs Register at address offset 0xEDC, write-only */
   uint32_t trcitmiscoutr;
   /** Integration Miscellaneous Inputs Register at address offset 0xEE0, read-only */
   uint32_t trcitmiscinr;
   /** Integration ATB Identification Register at address offset 0xEE4, write-only */
   uint32_t trcitatbidr;
   /** Integration Data ATB Data Register at address offset 0xEE8, write-only */
   uint32_t trcitddatar;
   /** Integration Instruction ATB Data Register at address offset 0xEEC, write-only */
   uint32_t trcitidatar;
   /** Integration Data ATB In Register at address offset 0xEF0, read-only */
   uint32_t trcitdatbinr;
   /** Integration Instruction ATB In Register at address offset 0xEF4, read-only */
   uint32_t trcitiatbinr;
   /** Integration Data ATB Out Register at address offset 0xEF8, write-only */
   uint32_t trcitdatboutr;
   /** Integration Instruction ATB Out Register at address offset 0xEFC, write-only */
   uint32_t trcitiatboutr;
   /** Integration Mode Control Register at address offset 0xF00, read-write */
   uint32_t trcitctrl;
   /** Reserved space */
   uint8_t fill40[156];
   /** Claim Tag Set Register at address offset 0xFA0, read-write */
   uint32_t trcclaimset;
   /** Claim Tag Clear Register at address offset 0xFA4, read-write */
   uint32_t trcclaimclr;
   /** Reserved space */
   uint8_t fill41[8];
   /** Software Lock Access Register at address offset 0xFB0, write-only */
   uint32_t trclar;
   /** Software Lock Status Register at address offset 0xFB4, read-only */
   uint32_t trclsr;
   /** Authentication Status Register at address offset 0xFB8, read-only */
   uint32_t trcauthstatus;
   /** Device Architecture Register at address offset 0xFBC, read-only */
   uint32_t trcdevarch;
   /** Reserved space */
   uint8_t fill42[8];
   /** Device ID Register at address offset 0xFC8, read-only */
   uint32_t trcdevid;
   /** Device Type Register at address offset 0xFCC, read-only */
   uint32_t trcdevtype;
   /** Peripheral Identification Register 4 at address offset 0xFD0, read-only */
   uint32_t trcpidr4;
   /** Peripheral Identification Register 5 at address offset 0xFD4, read-only */
   uint32_t trcpidr5;
   /** Peripheral Identification Register 6 at address offset 0xFD8, read-only */
   uint32_t trcpidr6;
   /** Peripheral Identification Register 7 at address offset 0xFDC, read-only */
   uint32_t trcpidr7;
   /** Peripheral Identification Register 0 at address offset 0xFE0, read-only */
   uint32_t trcpidr0;
   /** Peripheral Identification Register 1 at address offset 0xFE4, read-only */
   uint32_t trcpidr1;
   /** Peripheral Identification Register 2 at address offset 0xFE8, read-only */
   uint32_t trcpidr2;
   /** Peripheral Identification Register 3 at address offset 0xFEC, read-only */
   uint32_t trcpidr3;
   /** Component Identification Register 0 at address offset 0xFF0, read-only */
   uint32_t trccidr0;
   /** Component Identification Register 1 at address offset 0xFF4, read-only */
   uint32_t trccidr1;
   /** Component Identification Register 2 at address offset 0xFF8, read-only */
   uint32_t trccidr2;
   /** Component Identification Register 3 at address offset 0xFFC, read-only */
   uint32_t trccidr3;
};

/** bit field defines for cortex_m7_etm_s#trcprgctlr */
#define CORTEX_M7_ETM_TRCPRGCTLR_EN_OFFSET 0
#define CORTEX_M7_ETM_TRCPRGCTLR_EN_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcprocselr */
#define CORTEX_M7_ETM_TRCPROCSELR_PROCSEL_OFFSET 0
#define CORTEX_M7_ETM_TRCPROCSELR_PROCSEL_SIZE 2

/** bit field defines for cortex_m7_etm_s#trcstatr */
#define CORTEX_M7_ETM_TRCSTATR_IDLE_OFFSET 0
#define CORTEX_M7_ETM_TRCSTATR_IDLE_SIZE 1
#define CORTEX_M7_ETM_TRCSTATR_PMSTABLE_OFFSET 1
#define CORTEX_M7_ETM_TRCSTATR_PMSTABLE_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcconfigr */
#define CORTEX_M7_ETM_TRCCONFIGR_INSTP0_OFFSET 1
#define CORTEX_M7_ETM_TRCCONFIGR_INSTP0_SIZE 2
#define CORTEX_M7_ETM_TRCCONFIGR_BB_OFFSET 3
#define CORTEX_M7_ETM_TRCCONFIGR_BB_SIZE 1
#define CORTEX_M7_ETM_TRCCONFIGR_CCI_OFFSET 4
#define CORTEX_M7_ETM_TRCCONFIGR_CCI_SIZE 1
#define CORTEX_M7_ETM_TRCCONFIGR_CID_OFFSET 6
#define CORTEX_M7_ETM_TRCCONFIGR_CID_SIZE 1
#define CORTEX_M7_ETM_TRCCONFIGR_VMID_OFFSET 7
#define CORTEX_M7_ETM_TRCCONFIGR_VMID_SIZE 1
#define CORTEX_M7_ETM_TRCCONFIGR_COND_OFFSET 8
#define CORTEX_M7_ETM_TRCCONFIGR_COND_SIZE 3
#define CORTEX_M7_ETM_TRCCONFIGR_TS_OFFSET 11
#define CORTEX_M7_ETM_TRCCONFIGR_TS_SIZE 1
#define CORTEX_M7_ETM_TRCCONFIGR_RS_OFFSET 12
#define CORTEX_M7_ETM_TRCCONFIGR_RS_SIZE 1
#define CORTEX_M7_ETM_TRCCONFIGR_DA_OFFSET 16
#define CORTEX_M7_ETM_TRCCONFIGR_DA_SIZE 1
#define CORTEX_M7_ETM_TRCCONFIGR_DV_OFFSET 17
#define CORTEX_M7_ETM_TRCCONFIGR_DV_SIZE 1

/** bit field defines for cortex_m7_etm_s#trceventctl0r */
#define CORTEX_M7_ETM_TRCEVENTCTL0R_SEL0_OFFSET 0
#define CORTEX_M7_ETM_TRCEVENTCTL0R_SEL0_SIZE 4
#define CORTEX_M7_ETM_TRCEVENTCTL0R_TYPE0_OFFSET 7
#define CORTEX_M7_ETM_TRCEVENTCTL0R_TYPE0_SIZE 1
#define CORTEX_M7_ETM_TRCEVENTCTL0R_SEL1_OFFSET 8
#define CORTEX_M7_ETM_TRCEVENTCTL0R_SEL1_SIZE 4
#define CORTEX_M7_ETM_TRCEVENTCTL0R_TYPE1_OFFSET 15
#define CORTEX_M7_ETM_TRCEVENTCTL0R_TYPE1_SIZE 1
#define CORTEX_M7_ETM_TRCEVENTCTL0R_SEL2_OFFSET 16
#define CORTEX_M7_ETM_TRCEVENTCTL0R_SEL2_SIZE 4
#define CORTEX_M7_ETM_TRCEVENTCTL0R_TYPE2_OFFSET 23
#define CORTEX_M7_ETM_TRCEVENTCTL0R_TYPE2_SIZE 1
#define CORTEX_M7_ETM_TRCEVENTCTL0R_SEL3_OFFSET 24
#define CORTEX_M7_ETM_TRCEVENTCTL0R_SEL3_SIZE 4
#define CORTEX_M7_ETM_TRCEVENTCTL0R_TYPE3_OFFSET 31
#define CORTEX_M7_ETM_TRCEVENTCTL0R_TYPE3_SIZE 1

/** bit field defines for cortex_m7_etm_s#trceventctl1r */
#define CORTEX_M7_ETM_TRCEVENTCTL1R_INSTEN_OFFSET 0
#define CORTEX_M7_ETM_TRCEVENTCTL1R_INSTEN_SIZE 4
#define CORTEX_M7_ETM_TRCEVENTCTL1R_DATAEN_OFFSET 4
#define CORTEX_M7_ETM_TRCEVENTCTL1R_DATAEN_SIZE 1
#define CORTEX_M7_ETM_TRCEVENTCTL1R_ATB_OFFSET 11
#define CORTEX_M7_ETM_TRCEVENTCTL1R_ATB_SIZE 1
#define CORTEX_M7_ETM_TRCEVENTCTL1R_LPOVERRIDE_OFFSET 12
#define CORTEX_M7_ETM_TRCEVENTCTL1R_LPOVERRIDE_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcstallctlr */
#define CORTEX_M7_ETM_TRCSTALLCTLR_LEVEL_OFFSET 2
#define CORTEX_M7_ETM_TRCSTALLCTLR_LEVEL_SIZE 2
#define CORTEX_M7_ETM_TRCSTALLCTLR_ISTALL_OFFSET 8
#define CORTEX_M7_ETM_TRCSTALLCTLR_ISTALL_SIZE 1
#define CORTEX_M7_ETM_TRCSTALLCTLR_DSTALL_OFFSET 9
#define CORTEX_M7_ETM_TRCSTALLCTLR_DSTALL_SIZE 1
#define CORTEX_M7_ETM_TRCSTALLCTLR_INSTPRIORITY_OFFSET 10
#define CORTEX_M7_ETM_TRCSTALLCTLR_INSTPRIORITY_SIZE 1
#define CORTEX_M7_ETM_TRCSTALLCTLR_DATADISCARD_OFFSET 11
#define CORTEX_M7_ETM_TRCSTALLCTLR_DATADISCARD_SIZE 2

/** bit field defines for cortex_m7_etm_s#trctsctlr */
#define CORTEX_M7_ETM_TRCTSCTLR_SEL_OFFSET 0
#define CORTEX_M7_ETM_TRCTSCTLR_SEL_SIZE 4
#define CORTEX_M7_ETM_TRCTSCTLR_TYPE_OFFSET 7
#define CORTEX_M7_ETM_TRCTSCTLR_TYPE_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcsyncpr */
#define CORTEX_M7_ETM_TRCSYNCPR_PERIOD_OFFSET 0
#define CORTEX_M7_ETM_TRCSYNCPR_PERIOD_SIZE 5

/** bit field defines for cortex_m7_etm_s#trcccctlr */
#define CORTEX_M7_ETM_TRCCCCTLR_THRESHOLD_OFFSET 0
#define CORTEX_M7_ETM_TRCCCCTLR_THRESHOLD_SIZE 12

/** bit field defines for cortex_m7_etm_s#trcbbctlr */
#define CORTEX_M7_ETM_TRCBBCTLR_RANGE_OFFSET 0
#define CORTEX_M7_ETM_TRCBBCTLR_RANGE_SIZE 4
#define CORTEX_M7_ETM_TRCBBCTLR_MODE_OFFSET 8
#define CORTEX_M7_ETM_TRCBBCTLR_MODE_SIZE 1

/** bit field defines for cortex_m7_etm_s#trctraceidr */
#define CORTEX_M7_ETM_TRCTRACEIDR_TRACEID_OFFSET 0
#define CORTEX_M7_ETM_TRCTRACEIDR_TRACEID_SIZE 7

/** bit field defines for cortex_m7_etm_s#trcvictlr */
#define CORTEX_M7_ETM_TRCVICTLR_SEL_OFFSET 0
#define CORTEX_M7_ETM_TRCVICTLR_SEL_SIZE 4
#define CORTEX_M7_ETM_TRCVICTLR_TYPE_OFFSET 7
#define CORTEX_M7_ETM_TRCVICTLR_TYPE_SIZE 1
#define CORTEX_M7_ETM_TRCVICTLR_SSSTATUS_OFFSET 9
#define CORTEX_M7_ETM_TRCVICTLR_SSSTATUS_SIZE 1
#define CORTEX_M7_ETM_TRCVICTLR_TRCRESET_OFFSET 10
#define CORTEX_M7_ETM_TRCVICTLR_TRCRESET_SIZE 1
#define CORTEX_M7_ETM_TRCVICTLR_TRCERR_OFFSET 11
#define CORTEX_M7_ETM_TRCVICTLR_TRCERR_SIZE 1
#define CORTEX_M7_ETM_TRCVICTLR_EXLEVEL_S0_OFFSET 16
#define CORTEX_M7_ETM_TRCVICTLR_EXLEVEL_S0_SIZE 1
#define CORTEX_M7_ETM_TRCVICTLR_EXLEVEL_S3_OFFSET 19
#define CORTEX_M7_ETM_TRCVICTLR_EXLEVEL_S3_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcviiectlr */
#define CORTEX_M7_ETM_TRCVIIECTLR_INCLUDE_OFFSET 0
#define CORTEX_M7_ETM_TRCVIIECTLR_INCLUDE_SIZE 4
#define CORTEX_M7_ETM_TRCVIIECTLR_EXCLUDE_OFFSET 16
#define CORTEX_M7_ETM_TRCVIIECTLR_EXCLUDE_SIZE 4

/** bit field defines for cortex_m7_etm_s#trcvissctlr */
#define CORTEX_M7_ETM_TRCVISSCTLR_START_OFFSET 0
#define CORTEX_M7_ETM_TRCVISSCTLR_START_SIZE 8
#define CORTEX_M7_ETM_TRCVISSCTLR_STOP_OFFSET 16
#define CORTEX_M7_ETM_TRCVISSCTLR_STOP_SIZE 8

/** bit field defines for cortex_m7_etm_s#trcvipcssctlr */
#define CORTEX_M7_ETM_TRCVIPCSSCTLR_START_OFFSET 0
#define CORTEX_M7_ETM_TRCVIPCSSCTLR_START_SIZE 4
#define CORTEX_M7_ETM_TRCVIPCSSCTLR_STOP_OFFSET 16
#define CORTEX_M7_ETM_TRCVIPCSSCTLR_STOP_SIZE 4

/** bit field defines for cortex_m7_etm_s#trcvdctlr */
#define CORTEX_M7_ETM_TRCVDCTLR_SEL_OFFSET 0
#define CORTEX_M7_ETM_TRCVDCTLR_SEL_SIZE 4
#define CORTEX_M7_ETM_TRCVDCTLR_TYPE_OFFSET 7
#define CORTEX_M7_ETM_TRCVDCTLR_TYPE_SIZE 1
#define CORTEX_M7_ETM_TRCVDCTLR_SPREL_OFFSET 8
#define CORTEX_M7_ETM_TRCVDCTLR_SPREL_SIZE 2
#define CORTEX_M7_ETM_TRCVDCTLR_PCREL_OFFSET 10
#define CORTEX_M7_ETM_TRCVDCTLR_PCREL_SIZE 1
#define CORTEX_M7_ETM_TRCVDCTLR_TRCEXDATA_OFFSET 12
#define CORTEX_M7_ETM_TRCVDCTLR_TRCEXDATA_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcvdsacctlr */
#define CORTEX_M7_ETM_TRCVDSACCTLR_INCLUDE_OFFSET 0
#define CORTEX_M7_ETM_TRCVDSACCTLR_INCLUDE_SIZE 8
#define CORTEX_M7_ETM_TRCVDSACCTLR_EXLUDE_OFFSET 16
#define CORTEX_M7_ETM_TRCVDSACCTLR_EXLUDE_SIZE 9

/** bit field defines for cortex_m7_etm_s#trcvdarcctlr */
#define CORTEX_M7_ETM_TRCVDARCCTLR_INCLUDE_OFFSET 0
#define CORTEX_M7_ETM_TRCVDARCCTLR_INCLUDE_SIZE 4
#define CORTEX_M7_ETM_TRCVDARCCTLR_EXLUDE_OFFSET 16
#define CORTEX_M7_ETM_TRCVDARCCTLR_EXLUDE_SIZE 4

/** bit field defines for cortex_m7_etm_s#trcseqevr0 */
#define CORTEX_M7_ETM_TRCSEQEVR0_FSEL_OFFSET 0
#define CORTEX_M7_ETM_TRCSEQEVR0_FSEL_SIZE 4
#define CORTEX_M7_ETM_TRCSEQEVR0_FTYPE_OFFSET 7
#define CORTEX_M7_ETM_TRCSEQEVR0_FTYPE_SIZE 1
#define CORTEX_M7_ETM_TRCSEQEVR0_BSEL_OFFSET 8
#define CORTEX_M7_ETM_TRCSEQEVR0_BSEL_SIZE 4
#define CORTEX_M7_ETM_TRCSEQEVR0_BTYPE_OFFSET 15
#define CORTEX_M7_ETM_TRCSEQEVR0_BTYPE_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcseqevr1 */
#define CORTEX_M7_ETM_TRCSEQEVR1_FSEL_OFFSET 0
#define CORTEX_M7_ETM_TRCSEQEVR1_FSEL_SIZE 4
#define CORTEX_M7_ETM_TRCSEQEVR1_FTYPE_OFFSET 7
#define CORTEX_M7_ETM_TRCSEQEVR1_FTYPE_SIZE 1
#define CORTEX_M7_ETM_TRCSEQEVR1_BSEL_OFFSET 8
#define CORTEX_M7_ETM_TRCSEQEVR1_BSEL_SIZE 4
#define CORTEX_M7_ETM_TRCSEQEVR1_BTYPE_OFFSET 15
#define CORTEX_M7_ETM_TRCSEQEVR1_BTYPE_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcseqevr2 */
#define CORTEX_M7_ETM_TRCSEQEVR2_FSEL_OFFSET 0
#define CORTEX_M7_ETM_TRCSEQEVR2_FSEL_SIZE 4
#define CORTEX_M7_ETM_TRCSEQEVR2_FTYPE_OFFSET 7
#define CORTEX_M7_ETM_TRCSEQEVR2_FTYPE_SIZE 1
#define CORTEX_M7_ETM_TRCSEQEVR2_BSEL_OFFSET 8
#define CORTEX_M7_ETM_TRCSEQEVR2_BSEL_SIZE 4
#define CORTEX_M7_ETM_TRCSEQEVR2_BTYPE_OFFSET 15
#define CORTEX_M7_ETM_TRCSEQEVR2_BTYPE_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcseqrstevr */
#define CORTEX_M7_ETM_TRCSEQRSTEVR_RESETSEL_OFFSET 0
#define CORTEX_M7_ETM_TRCSEQRSTEVR_RESETSEL_SIZE 4
#define CORTEX_M7_ETM_TRCSEQRSTEVR_RESETTYPE_OFFSET 7
#define CORTEX_M7_ETM_TRCSEQRSTEVR_RESETTYPE_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcseqstr */
#define CORTEX_M7_ETM_TRCSEQSTR_STATE_OFFSET 0
#define CORTEX_M7_ETM_TRCSEQSTR_STATE_SIZE 2

/** bit field defines for cortex_m7_etm_s#trcextinselr */
#define CORTEX_M7_ETM_TRCEXTINSELR_TRCEXTINSELR_OFFSET 0
#define CORTEX_M7_ETM_TRCEXTINSELR_TRCEXTINSELR_SIZE 32

/** bit field defines for cortex_m7_etm_s#trccntrldvr0 */
#define CORTEX_M7_ETM_TRCCNTRLDVR0_VALUE_OFFSET 0
#define CORTEX_M7_ETM_TRCCNTRLDVR0_VALUE_SIZE 16

/** bit field defines for cortex_m7_etm_s#trccntrldvr1 */
#define CORTEX_M7_ETM_TRCCNTRLDVR1_VALUE_OFFSET 0
#define CORTEX_M7_ETM_TRCCNTRLDVR1_VALUE_SIZE 16

/** bit field defines for cortex_m7_etm_s#trccntctlr0 */
#define CORTEX_M7_ETM_TRCCNTCTLR0_CNTSEL_OFFSET 0
#define CORTEX_M7_ETM_TRCCNTCTLR0_CNTSEL_SIZE 4
#define CORTEX_M7_ETM_TRCCNTCTLR0_CNTTYPE_OFFSET 7
#define CORTEX_M7_ETM_TRCCNTCTLR0_CNTTYPE_SIZE 1
#define CORTEX_M7_ETM_TRCCNTCTLR0_RLDSEL_OFFSET 8
#define CORTEX_M7_ETM_TRCCNTCTLR0_RLDSEL_SIZE 4
#define CORTEX_M7_ETM_TRCCNTCTLR0_RLDTYPE_OFFSET 15
#define CORTEX_M7_ETM_TRCCNTCTLR0_RLDTYPE_SIZE 1
#define CORTEX_M7_ETM_TRCCNTCTLR0_RLDSELF_OFFSET 16
#define CORTEX_M7_ETM_TRCCNTCTLR0_RLDSELF_SIZE 1

/** bit field defines for cortex_m7_etm_s#trccntctlr1 */
#define CORTEX_M7_ETM_TRCCNTCTLR1_CNTSEL_OFFSET 0
#define CORTEX_M7_ETM_TRCCNTCTLR1_CNTSEL_SIZE 4
#define CORTEX_M7_ETM_TRCCNTCTLR1_CNTTYPE_OFFSET 7
#define CORTEX_M7_ETM_TRCCNTCTLR1_CNTTYPE_SIZE 1
#define CORTEX_M7_ETM_TRCCNTCTLR1_RLDSEL_OFFSET 8
#define CORTEX_M7_ETM_TRCCNTCTLR1_RLDSEL_SIZE 4
#define CORTEX_M7_ETM_TRCCNTCTLR1_RLDTYPE_OFFSET 15
#define CORTEX_M7_ETM_TRCCNTCTLR1_RLDTYPE_SIZE 1
#define CORTEX_M7_ETM_TRCCNTCTLR1_RLDSELF_OFFSET 16
#define CORTEX_M7_ETM_TRCCNTCTLR1_RLDSELF_SIZE 1
#define CORTEX_M7_ETM_TRCCNTCTLR1_CNTCHAIN_OFFSET 17
#define CORTEX_M7_ETM_TRCCNTCTLR1_CNTCHAIN_SIZE 1

/** bit field defines for cortex_m7_etm_s#trccntvr0 */
#define CORTEX_M7_ETM_TRCCNTVR0_VALUE_OFFSET 0
#define CORTEX_M7_ETM_TRCCNTVR0_VALUE_SIZE 16

/** bit field defines for cortex_m7_etm_s#trccntvr1 */
#define CORTEX_M7_ETM_TRCCNTVR1_VALUE_OFFSET 0
#define CORTEX_M7_ETM_TRCCNTVR1_VALUE_SIZE 16

/** bit field defines for cortex_m7_etm_s#trcidr8 */
#define CORTEX_M7_ETM_TRCIDR8_MAXSPEC_OFFSET 0
#define CORTEX_M7_ETM_TRCIDR8_MAXSPEC_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcidr9 */
#define CORTEX_M7_ETM_TRCIDR9_NUMP0KEY_OFFSET 0
#define CORTEX_M7_ETM_TRCIDR9_NUMP0KEY_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcidr10 */
#define CORTEX_M7_ETM_TRCIDR10_NUMP1KEY_OFFSET 0
#define CORTEX_M7_ETM_TRCIDR10_NUMP1KEY_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcidr11 */
#define CORTEX_M7_ETM_TRCIDR11_NUMP1SPC_OFFSET 0
#define CORTEX_M7_ETM_TRCIDR11_NUMP1SPC_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcidr12 */
#define CORTEX_M7_ETM_TRCIDR12_NUMCONDKEY_OFFSET 0
#define CORTEX_M7_ETM_TRCIDR12_NUMCONDKEY_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcidr13 */
#define CORTEX_M7_ETM_TRCIDR13_NUMCONDSPC_OFFSET 0
#define CORTEX_M7_ETM_TRCIDR13_NUMCONDSPC_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcimspec0 */
#define CORTEX_M7_ETM_TRCIMSPEC0_SUPPORT_OFFSET 0
#define CORTEX_M7_ETM_TRCIMSPEC0_SUPPORT_SIZE 4

/** bit field defines for cortex_m7_etm_s#trcidr0 */
#define CORTEX_M7_ETM_TRCIDR0_INSTP0_OFFSET 1
#define CORTEX_M7_ETM_TRCIDR0_INSTP0_SIZE 2
#define CORTEX_M7_ETM_TRCIDR0_TRCDATA_OFFSET 3
#define CORTEX_M7_ETM_TRCIDR0_TRCDATA_SIZE 2
#define CORTEX_M7_ETM_TRCIDR0_TRCBB_OFFSET 5
#define CORTEX_M7_ETM_TRCIDR0_TRCBB_SIZE 1
#define CORTEX_M7_ETM_TRCIDR0_TRCCOND_OFFSET 6
#define CORTEX_M7_ETM_TRCIDR0_TRCCOND_SIZE 1
#define CORTEX_M7_ETM_TRCIDR0_TRCCCI_OFFSET 7
#define CORTEX_M7_ETM_TRCIDR0_TRCCCI_SIZE 1
#define CORTEX_M7_ETM_TRCIDR0_RETSTACK_OFFSET 9
#define CORTEX_M7_ETM_TRCIDR0_RETSTACK_SIZE 1
#define CORTEX_M7_ETM_TRCIDR0_NUMEVENT_OFFSET 10
#define CORTEX_M7_ETM_TRCIDR0_NUMEVENT_SIZE 2
#define CORTEX_M7_ETM_TRCIDR0_CONDTYPE_OFFSET 12
#define CORTEX_M7_ETM_TRCIDR0_CONDTYPE_SIZE 2
#define CORTEX_M7_ETM_TRCIDR0_QFILT_OFFSET 14
#define CORTEX_M7_ETM_TRCIDR0_QFILT_SIZE 1
#define CORTEX_M7_ETM_TRCIDR0_QSUPP_OFFSET 15
#define CORTEX_M7_ETM_TRCIDR0_QSUPP_SIZE 2
#define CORTEX_M7_ETM_TRCIDR0_TRCEXDATA_OFFSET 17
#define CORTEX_M7_ETM_TRCIDR0_TRCEXDATA_SIZE 1
#define CORTEX_M7_ETM_TRCIDR0_TSSIZE_OFFSET 24
#define CORTEX_M7_ETM_TRCIDR0_TSSIZE_SIZE 5
#define CORTEX_M7_ETM_TRCIDR0_COMMOPT_OFFSET 29
#define CORTEX_M7_ETM_TRCIDR0_COMMOPT_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcidr1 */
#define CORTEX_M7_ETM_TRCIDR1_REVISION_OFFSET 0
#define CORTEX_M7_ETM_TRCIDR1_REVISION_SIZE 4
#define CORTEX_M7_ETM_TRCIDR1_TRCARCHMIN_OFFSET 4
#define CORTEX_M7_ETM_TRCIDR1_TRCARCHMIN_SIZE 4
#define CORTEX_M7_ETM_TRCIDR1_TRCARCHMAJ_OFFSET 8
#define CORTEX_M7_ETM_TRCIDR1_TRCARCHMAJ_SIZE 4
#define CORTEX_M7_ETM_TRCIDR1_DESIGNER_OFFSET 24
#define CORTEX_M7_ETM_TRCIDR1_DESIGNER_SIZE 8

/** bit field defines for cortex_m7_etm_s#trcidr2 */
#define CORTEX_M7_ETM_TRCIDR2_IASIZE_OFFSET 0
#define CORTEX_M7_ETM_TRCIDR2_IASIZE_SIZE 5
#define CORTEX_M7_ETM_TRCIDR2_CIDSIZE_OFFSET 5
#define CORTEX_M7_ETM_TRCIDR2_CIDSIZE_SIZE 5
#define CORTEX_M7_ETM_TRCIDR2_VMIDSIZE_OFFSET 10
#define CORTEX_M7_ETM_TRCIDR2_VMIDSIZE_SIZE 5
#define CORTEX_M7_ETM_TRCIDR2_DASIZE_OFFSET 15
#define CORTEX_M7_ETM_TRCIDR2_DASIZE_SIZE 5
#define CORTEX_M7_ETM_TRCIDR2_DVSIZE_OFFSET 20
#define CORTEX_M7_ETM_TRCIDR2_DVSIZE_SIZE 5
#define CORTEX_M7_ETM_TRCIDR2_CCSIZE_OFFSET 25
#define CORTEX_M7_ETM_TRCIDR2_CCSIZE_SIZE 4

/** bit field defines for cortex_m7_etm_s#trcidr3 */
#define CORTEX_M7_ETM_TRCIDR3_CCITMIN_OFFSET 0
#define CORTEX_M7_ETM_TRCIDR3_CCITMIN_SIZE 12
#define CORTEX_M7_ETM_TRCIDR3_EXLEVEL_S_OFFSET 16
#define CORTEX_M7_ETM_TRCIDR3_EXLEVEL_S_SIZE 4
#define CORTEX_M7_ETM_TRCIDR3_EXLEVEL_NS_OFFSET 20
#define CORTEX_M7_ETM_TRCIDR3_EXLEVEL_NS_SIZE 4
#define CORTEX_M7_ETM_TRCIDR3_TRCERR_OFFSET 24
#define CORTEX_M7_ETM_TRCIDR3_TRCERR_SIZE 1
#define CORTEX_M7_ETM_TRCIDR3_SYNCPR_OFFSET 25
#define CORTEX_M7_ETM_TRCIDR3_SYNCPR_SIZE 1
#define CORTEX_M7_ETM_TRCIDR3_STALLCTL_OFFSET 26
#define CORTEX_M7_ETM_TRCIDR3_STALLCTL_SIZE 1
#define CORTEX_M7_ETM_TRCIDR3_SYSSTALL_OFFSET 27
#define CORTEX_M7_ETM_TRCIDR3_SYSSTALL_SIZE 1
#define CORTEX_M7_ETM_TRCIDR3_NUMPROC_OFFSET 28
#define CORTEX_M7_ETM_TRCIDR3_NUMPROC_SIZE 3
#define CORTEX_M7_ETM_TRCIDR3_NOOVERFLOW_OFFSET 31
#define CORTEX_M7_ETM_TRCIDR3_NOOVERFLOW_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcidr4 */
#define CORTEX_M7_ETM_TRCIDR4_NUMACPAIRS_OFFSET 0
#define CORTEX_M7_ETM_TRCIDR4_NUMACPAIRS_SIZE 4
#define CORTEX_M7_ETM_TRCIDR4_NUMDVC_OFFSET 4
#define CORTEX_M7_ETM_TRCIDR4_NUMDVC_SIZE 4
#define CORTEX_M7_ETM_TRCIDR4_SUPPDAC_OFFSET 8
#define CORTEX_M7_ETM_TRCIDR4_SUPPDAC_SIZE 1
#define CORTEX_M7_ETM_TRCIDR4_NUMPC_OFFSET 12
#define CORTEX_M7_ETM_TRCIDR4_NUMPC_SIZE 4
#define CORTEX_M7_ETM_TRCIDR4_NUMRSPAIR_OFFSET 16
#define CORTEX_M7_ETM_TRCIDR4_NUMRSPAIR_SIZE 4
#define CORTEX_M7_ETM_TRCIDR4_NUMSSCC_OFFSET 20
#define CORTEX_M7_ETM_TRCIDR4_NUMSSCC_SIZE 4
#define CORTEX_M7_ETM_TRCIDR4_NUMCIDC_OFFSET 24
#define CORTEX_M7_ETM_TRCIDR4_NUMCIDC_SIZE 4
#define CORTEX_M7_ETM_TRCIDR4_NUMVMIDC_OFFSET 28
#define CORTEX_M7_ETM_TRCIDR4_NUMVMIDC_SIZE 4

/** bit field defines for cortex_m7_etm_s#trcidr5 */
#define CORTEX_M7_ETM_TRCIDR5_NUMEXTIN_OFFSET 0
#define CORTEX_M7_ETM_TRCIDR5_NUMEXTIN_SIZE 9
#define CORTEX_M7_ETM_TRCIDR5_NUMEXTINSEL_OFFSET 9
#define CORTEX_M7_ETM_TRCIDR5_NUMEXTINSEL_SIZE 3
#define CORTEX_M7_ETM_TRCIDR5_TRACEIDSIZE_OFFSET 16
#define CORTEX_M7_ETM_TRCIDR5_TRACEIDSIZE_SIZE 6
#define CORTEX_M7_ETM_TRCIDR5_ATBTRIG_OFFSET 22
#define CORTEX_M7_ETM_TRCIDR5_ATBTRIG_SIZE 1
#define CORTEX_M7_ETM_TRCIDR5_LPOVERRIDE_OFFSET 23
#define CORTEX_M7_ETM_TRCIDR5_LPOVERRIDE_SIZE 1
#define CORTEX_M7_ETM_TRCIDR5_NUMSEQSTATE_OFFSET 25
#define CORTEX_M7_ETM_TRCIDR5_NUMSEQSTATE_SIZE 3
#define CORTEX_M7_ETM_TRCIDR5_NUMCNTR_OFFSET 28
#define CORTEX_M7_ETM_TRCIDR5_NUMCNTR_SIZE 3
#define CORTEX_M7_ETM_TRCIDR5_REDFUNCNTR_OFFSET 31
#define CORTEX_M7_ETM_TRCIDR5_REDFUNCNTR_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcrsctlr2 */
#define CORTEX_M7_ETM_TRCRSCTLR2_SELECT_OFFSET 0
#define CORTEX_M7_ETM_TRCRSCTLR2_SELECT_SIZE 8
#define CORTEX_M7_ETM_TRCRSCTLR2_GROUP_OFFSET 16
#define CORTEX_M7_ETM_TRCRSCTLR2_GROUP_SIZE 3
#define CORTEX_M7_ETM_TRCRSCTLR2_INV_OFFSET 20
#define CORTEX_M7_ETM_TRCRSCTLR2_INV_SIZE 1
#define CORTEX_M7_ETM_TRCRSCTLR2_PAIRINV_OFFSET 21
#define CORTEX_M7_ETM_TRCRSCTLR2_PAIRINV_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcrsctlr3 */
#define CORTEX_M7_ETM_TRCRSCTLR3_SELECT_OFFSET 0
#define CORTEX_M7_ETM_TRCRSCTLR3_SELECT_SIZE 8
#define CORTEX_M7_ETM_TRCRSCTLR3_GROUP_OFFSET 16
#define CORTEX_M7_ETM_TRCRSCTLR3_GROUP_SIZE 3
#define CORTEX_M7_ETM_TRCRSCTLR3_INV_OFFSET 20
#define CORTEX_M7_ETM_TRCRSCTLR3_INV_SIZE 1
#define CORTEX_M7_ETM_TRCRSCTLR3_PAIRINV_OFFSET 21
#define CORTEX_M7_ETM_TRCRSCTLR3_PAIRINV_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcrsctlr4 */
#define CORTEX_M7_ETM_TRCRSCTLR4_SELECT_OFFSET 0
#define CORTEX_M7_ETM_TRCRSCTLR4_SELECT_SIZE 8
#define CORTEX_M7_ETM_TRCRSCTLR4_GROUP_OFFSET 16
#define CORTEX_M7_ETM_TRCRSCTLR4_GROUP_SIZE 3
#define CORTEX_M7_ETM_TRCRSCTLR4_INV_OFFSET 20
#define CORTEX_M7_ETM_TRCRSCTLR4_INV_SIZE 1
#define CORTEX_M7_ETM_TRCRSCTLR4_PAIRINV_OFFSET 21
#define CORTEX_M7_ETM_TRCRSCTLR4_PAIRINV_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcrsctlr5 */
#define CORTEX_M7_ETM_TRCRSCTLR5_SELECT_OFFSET 0
#define CORTEX_M7_ETM_TRCRSCTLR5_SELECT_SIZE 8
#define CORTEX_M7_ETM_TRCRSCTLR5_GROUP_OFFSET 16
#define CORTEX_M7_ETM_TRCRSCTLR5_GROUP_SIZE 3
#define CORTEX_M7_ETM_TRCRSCTLR5_INV_OFFSET 20
#define CORTEX_M7_ETM_TRCRSCTLR5_INV_SIZE 1
#define CORTEX_M7_ETM_TRCRSCTLR5_PAIRINV_OFFSET 21
#define CORTEX_M7_ETM_TRCRSCTLR5_PAIRINV_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcrsctlr6 */
#define CORTEX_M7_ETM_TRCRSCTLR6_SELECT_OFFSET 0
#define CORTEX_M7_ETM_TRCRSCTLR6_SELECT_SIZE 8
#define CORTEX_M7_ETM_TRCRSCTLR6_GROUP_OFFSET 16
#define CORTEX_M7_ETM_TRCRSCTLR6_GROUP_SIZE 3
#define CORTEX_M7_ETM_TRCRSCTLR6_INV_OFFSET 20
#define CORTEX_M7_ETM_TRCRSCTLR6_INV_SIZE 1
#define CORTEX_M7_ETM_TRCRSCTLR6_PAIRINV_OFFSET 21
#define CORTEX_M7_ETM_TRCRSCTLR6_PAIRINV_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcrsctlr7 */
#define CORTEX_M7_ETM_TRCRSCTLR7_SELECT_OFFSET 0
#define CORTEX_M7_ETM_TRCRSCTLR7_SELECT_SIZE 8
#define CORTEX_M7_ETM_TRCRSCTLR7_GROUP_OFFSET 16
#define CORTEX_M7_ETM_TRCRSCTLR7_GROUP_SIZE 3
#define CORTEX_M7_ETM_TRCRSCTLR7_INV_OFFSET 20
#define CORTEX_M7_ETM_TRCRSCTLR7_INV_SIZE 1
#define CORTEX_M7_ETM_TRCRSCTLR7_PAIRINV_OFFSET 21
#define CORTEX_M7_ETM_TRCRSCTLR7_PAIRINV_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcrsctlr8 */
#define CORTEX_M7_ETM_TRCRSCTLR8_SELECT_OFFSET 0
#define CORTEX_M7_ETM_TRCRSCTLR8_SELECT_SIZE 8
#define CORTEX_M7_ETM_TRCRSCTLR8_GROUP_OFFSET 16
#define CORTEX_M7_ETM_TRCRSCTLR8_GROUP_SIZE 3
#define CORTEX_M7_ETM_TRCRSCTLR8_INV_OFFSET 20
#define CORTEX_M7_ETM_TRCRSCTLR8_INV_SIZE 1
#define CORTEX_M7_ETM_TRCRSCTLR8_PAIRINV_OFFSET 21
#define CORTEX_M7_ETM_TRCRSCTLR8_PAIRINV_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcrsctlr9 */
#define CORTEX_M7_ETM_TRCRSCTLR9_SELECT_OFFSET 0
#define CORTEX_M7_ETM_TRCRSCTLR9_SELECT_SIZE 8
#define CORTEX_M7_ETM_TRCRSCTLR9_GROUP_OFFSET 16
#define CORTEX_M7_ETM_TRCRSCTLR9_GROUP_SIZE 3
#define CORTEX_M7_ETM_TRCRSCTLR9_INV_OFFSET 20
#define CORTEX_M7_ETM_TRCRSCTLR9_INV_SIZE 1
#define CORTEX_M7_ETM_TRCRSCTLR9_PAIRINV_OFFSET 21
#define CORTEX_M7_ETM_TRCRSCTLR9_PAIRINV_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcrsctlr10 */
#define CORTEX_M7_ETM_TRCRSCTLR10_SELECT_OFFSET 0
#define CORTEX_M7_ETM_TRCRSCTLR10_SELECT_SIZE 8
#define CORTEX_M7_ETM_TRCRSCTLR10_GROUP_OFFSET 16
#define CORTEX_M7_ETM_TRCRSCTLR10_GROUP_SIZE 3
#define CORTEX_M7_ETM_TRCRSCTLR10_INV_OFFSET 20
#define CORTEX_M7_ETM_TRCRSCTLR10_INV_SIZE 1
#define CORTEX_M7_ETM_TRCRSCTLR10_PAIRINV_OFFSET 21
#define CORTEX_M7_ETM_TRCRSCTLR10_PAIRINV_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcrsctlr11 */
#define CORTEX_M7_ETM_TRCRSCTLR11_SELECT_OFFSET 0
#define CORTEX_M7_ETM_TRCRSCTLR11_SELECT_SIZE 8
#define CORTEX_M7_ETM_TRCRSCTLR11_GROUP_OFFSET 16
#define CORTEX_M7_ETM_TRCRSCTLR11_GROUP_SIZE 3
#define CORTEX_M7_ETM_TRCRSCTLR11_INV_OFFSET 20
#define CORTEX_M7_ETM_TRCRSCTLR11_INV_SIZE 1
#define CORTEX_M7_ETM_TRCRSCTLR11_PAIRINV_OFFSET 21
#define CORTEX_M7_ETM_TRCRSCTLR11_PAIRINV_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcrsctlr12 */
#define CORTEX_M7_ETM_TRCRSCTLR12_SELECT_OFFSET 0
#define CORTEX_M7_ETM_TRCRSCTLR12_SELECT_SIZE 8
#define CORTEX_M7_ETM_TRCRSCTLR12_GROUP_OFFSET 16
#define CORTEX_M7_ETM_TRCRSCTLR12_GROUP_SIZE 3
#define CORTEX_M7_ETM_TRCRSCTLR12_INV_OFFSET 20
#define CORTEX_M7_ETM_TRCRSCTLR12_INV_SIZE 1
#define CORTEX_M7_ETM_TRCRSCTLR12_PAIRINV_OFFSET 21
#define CORTEX_M7_ETM_TRCRSCTLR12_PAIRINV_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcrsctlr13 */
#define CORTEX_M7_ETM_TRCRSCTLR13_SELECT_OFFSET 0
#define CORTEX_M7_ETM_TRCRSCTLR13_SELECT_SIZE 8
#define CORTEX_M7_ETM_TRCRSCTLR13_GROUP_OFFSET 16
#define CORTEX_M7_ETM_TRCRSCTLR13_GROUP_SIZE 3
#define CORTEX_M7_ETM_TRCRSCTLR13_INV_OFFSET 20
#define CORTEX_M7_ETM_TRCRSCTLR13_INV_SIZE 1
#define CORTEX_M7_ETM_TRCRSCTLR13_PAIRINV_OFFSET 21
#define CORTEX_M7_ETM_TRCRSCTLR13_PAIRINV_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcrsctlr14 */
#define CORTEX_M7_ETM_TRCRSCTLR14_SELECT_OFFSET 0
#define CORTEX_M7_ETM_TRCRSCTLR14_SELECT_SIZE 8
#define CORTEX_M7_ETM_TRCRSCTLR14_GROUP_OFFSET 16
#define CORTEX_M7_ETM_TRCRSCTLR14_GROUP_SIZE 3
#define CORTEX_M7_ETM_TRCRSCTLR14_INV_OFFSET 20
#define CORTEX_M7_ETM_TRCRSCTLR14_INV_SIZE 1
#define CORTEX_M7_ETM_TRCRSCTLR14_PAIRINV_OFFSET 21
#define CORTEX_M7_ETM_TRCRSCTLR14_PAIRINV_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcrsctlr15 */
#define CORTEX_M7_ETM_TRCRSCTLR15_SELECT_OFFSET 0
#define CORTEX_M7_ETM_TRCRSCTLR15_SELECT_SIZE 8
#define CORTEX_M7_ETM_TRCRSCTLR15_GROUP_OFFSET 16
#define CORTEX_M7_ETM_TRCRSCTLR15_GROUP_SIZE 3
#define CORTEX_M7_ETM_TRCRSCTLR15_INV_OFFSET 20
#define CORTEX_M7_ETM_TRCRSCTLR15_INV_SIZE 1
#define CORTEX_M7_ETM_TRCRSCTLR15_PAIRINV_OFFSET 21
#define CORTEX_M7_ETM_TRCRSCTLR15_PAIRINV_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcssccr0 */
#define CORTEX_M7_ETM_TRCSSCCR0_SAC_OFFSET 0
#define CORTEX_M7_ETM_TRCSSCCR0_SAC_SIZE 8
#define CORTEX_M7_ETM_TRCSSCCR0_ARC_OFFSET 16
#define CORTEX_M7_ETM_TRCSSCCR0_ARC_SIZE 4
#define CORTEX_M7_ETM_TRCSSCCR0_RST_OFFSET 24
#define CORTEX_M7_ETM_TRCSSCCR0_RST_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcsscsr0 */
#define CORTEX_M7_ETM_TRCSSCSR0_INST_OFFSET 0
#define CORTEX_M7_ETM_TRCSSCSR0_INST_SIZE 1
#define CORTEX_M7_ETM_TRCSSCSR0_DA_OFFSET 1
#define CORTEX_M7_ETM_TRCSSCSR0_DA_SIZE 1
#define CORTEX_M7_ETM_TRCSSCSR0_DV_OFFSET 2
#define CORTEX_M7_ETM_TRCSSCSR0_DV_SIZE 1
#define CORTEX_M7_ETM_TRCSSCSR0_PC_OFFSET 3
#define CORTEX_M7_ETM_TRCSSCSR0_PC_SIZE 1
#define CORTEX_M7_ETM_TRCSSCSR0_STATUS_OFFSET 31
#define CORTEX_M7_ETM_TRCSSCSR0_STATUS_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcsspcicr0 */
#define CORTEX_M7_ETM_TRCSSPCICR0_PC_OFFSET 0
#define CORTEX_M7_ETM_TRCSSPCICR0_PC_SIZE 4

/** bit field defines for cortex_m7_etm_s#trcoslsr */
#define CORTEX_M7_ETM_TRCOSLSR_OSLM_0_OFFSET 0
#define CORTEX_M7_ETM_TRCOSLSR_OSLM_0_SIZE 1
#define CORTEX_M7_ETM_TRCOSLSR_OSLK_OFFSET 1
#define CORTEX_M7_ETM_TRCOSLSR_OSLK_SIZE 1
#define CORTEX_M7_ETM_TRCOSLSR_NTT_OFFSET 2
#define CORTEX_M7_ETM_TRCOSLSR_NTT_SIZE 1
#define CORTEX_M7_ETM_TRCOSLSR_OSLM_1_OFFSET 3
#define CORTEX_M7_ETM_TRCOSLSR_OSLM_1_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcpdcr */
#define CORTEX_M7_ETM_TRCPDCR_PU_OFFSET 3
#define CORTEX_M7_ETM_TRCPDCR_PU_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcpdsr */
#define CORTEX_M7_ETM_TRCPDSR_POWER_OFFSET 0
#define CORTEX_M7_ETM_TRCPDSR_POWER_SIZE 1
#define CORTEX_M7_ETM_TRCPDSR_STICKYPD_OFFSET 1
#define CORTEX_M7_ETM_TRCPDSR_STICKYPD_SIZE 1
#define CORTEX_M7_ETM_TRCPDSR_OSLK_OFFSET 5
#define CORTEX_M7_ETM_TRCPDSR_OSLK_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcacvr0_31_0 */
#define CORTEX_M7_ETM_TRCACVR0_31_0_ADDRESS_OFFSET 0
#define CORTEX_M7_ETM_TRCACVR0_31_0_ADDRESS_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcacvr1_31_0 */
#define CORTEX_M7_ETM_TRCACVR1_31_0_ADDRESS_OFFSET 0
#define CORTEX_M7_ETM_TRCACVR1_31_0_ADDRESS_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcacvr2_31_0 */
#define CORTEX_M7_ETM_TRCACVR2_31_0_ADDRESS_OFFSET 0
#define CORTEX_M7_ETM_TRCACVR2_31_0_ADDRESS_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcacvr3_31_0 */
#define CORTEX_M7_ETM_TRCACVR3_31_0_ADDRESS_OFFSET 0
#define CORTEX_M7_ETM_TRCACVR3_31_0_ADDRESS_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcacvr4_31_0 */
#define CORTEX_M7_ETM_TRCACVR4_31_0_ADDRESS_OFFSET 0
#define CORTEX_M7_ETM_TRCACVR4_31_0_ADDRESS_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcacvr5_31_0 */
#define CORTEX_M7_ETM_TRCACVR5_31_0_ADDRESS_OFFSET 0
#define CORTEX_M7_ETM_TRCACVR5_31_0_ADDRESS_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcacvr6_31_0 */
#define CORTEX_M7_ETM_TRCACVR6_31_0_ADDRESS_OFFSET 0
#define CORTEX_M7_ETM_TRCACVR6_31_0_ADDRESS_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcacvr7_31_0 */
#define CORTEX_M7_ETM_TRCACVR7_31_0_ADDRESS_OFFSET 0
#define CORTEX_M7_ETM_TRCACVR7_31_0_ADDRESS_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcacatr0 */
#define CORTEX_M7_ETM_TRCACATR0_TYPE_OFFSET 0
#define CORTEX_M7_ETM_TRCACATR0_TYPE_SIZE 2
#define CORTEX_M7_ETM_TRCACATR0_CONTEXTTYPE_OFFSET 2
#define CORTEX_M7_ETM_TRCACATR0_CONTEXTTYPE_SIZE 1
#define CORTEX_M7_ETM_TRCACATR0_EXLEVEL_S_THREAD_OFFSET 8
#define CORTEX_M7_ETM_TRCACATR0_EXLEVEL_S_THREAD_SIZE 1
#define CORTEX_M7_ETM_TRCACATR0_EXLEVEL_S_HANDLER_OFFSET 11
#define CORTEX_M7_ETM_TRCACATR0_EXLEVEL_S_HANDLER_SIZE 1
#define CORTEX_M7_ETM_TRCACATR0_DATAMATCH_OFFSET 16
#define CORTEX_M7_ETM_TRCACATR0_DATAMATCH_SIZE 2
#define CORTEX_M7_ETM_TRCACATR0_DATASIZE_OFFSET 18
#define CORTEX_M7_ETM_TRCACATR0_DATASIZE_SIZE 2
#define CORTEX_M7_ETM_TRCACATR0_DATARANGE_OFFSET 20
#define CORTEX_M7_ETM_TRCACATR0_DATARANGE_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcacatr1 */
#define CORTEX_M7_ETM_TRCACATR1_TYPE_OFFSET 0
#define CORTEX_M7_ETM_TRCACATR1_TYPE_SIZE 2
#define CORTEX_M7_ETM_TRCACATR1_CONTEXTTYPE_OFFSET 2
#define CORTEX_M7_ETM_TRCACATR1_CONTEXTTYPE_SIZE 1
#define CORTEX_M7_ETM_TRCACATR1_EXLEVEL_S_0_OFFSET 8
#define CORTEX_M7_ETM_TRCACATR1_EXLEVEL_S_0_SIZE 1
#define CORTEX_M7_ETM_TRCACATR1_EXLEVEL_S_3_OFFSET 11
#define CORTEX_M7_ETM_TRCACATR1_EXLEVEL_S_3_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcacatr2 */
#define CORTEX_M7_ETM_TRCACATR2_TYPE_OFFSET 0
#define CORTEX_M7_ETM_TRCACATR2_TYPE_SIZE 2
#define CORTEX_M7_ETM_TRCACATR2_CONTEXTTYPE_OFFSET 2
#define CORTEX_M7_ETM_TRCACATR2_CONTEXTTYPE_SIZE 1
#define CORTEX_M7_ETM_TRCACATR2_EXLEVEL_S_THREAD_OFFSET 8
#define CORTEX_M7_ETM_TRCACATR2_EXLEVEL_S_THREAD_SIZE 1
#define CORTEX_M7_ETM_TRCACATR2_EXLEVEL_S_HANDLER_OFFSET 11
#define CORTEX_M7_ETM_TRCACATR2_EXLEVEL_S_HANDLER_SIZE 1
#define CORTEX_M7_ETM_TRCACATR2_DATAMATCH_OFFSET 16
#define CORTEX_M7_ETM_TRCACATR2_DATAMATCH_SIZE 2
#define CORTEX_M7_ETM_TRCACATR2_DATASIZE_OFFSET 18
#define CORTEX_M7_ETM_TRCACATR2_DATASIZE_SIZE 2
#define CORTEX_M7_ETM_TRCACATR2_DATARANGE_OFFSET 20
#define CORTEX_M7_ETM_TRCACATR2_DATARANGE_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcacatr3 */
#define CORTEX_M7_ETM_TRCACATR3_TYPE_OFFSET 0
#define CORTEX_M7_ETM_TRCACATR3_TYPE_SIZE 2
#define CORTEX_M7_ETM_TRCACATR3_CONTEXTTYPE_OFFSET 2
#define CORTEX_M7_ETM_TRCACATR3_CONTEXTTYPE_SIZE 1
#define CORTEX_M7_ETM_TRCACATR3_EXLEVEL_S_0_OFFSET 8
#define CORTEX_M7_ETM_TRCACATR3_EXLEVEL_S_0_SIZE 1
#define CORTEX_M7_ETM_TRCACATR3_EXLEVEL_S_3_OFFSET 11
#define CORTEX_M7_ETM_TRCACATR3_EXLEVEL_S_3_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcacatr4 */
#define CORTEX_M7_ETM_TRCACATR4_TYPE_OFFSET 0
#define CORTEX_M7_ETM_TRCACATR4_TYPE_SIZE 2
#define CORTEX_M7_ETM_TRCACATR4_CONTEXTTYPE_OFFSET 2
#define CORTEX_M7_ETM_TRCACATR4_CONTEXTTYPE_SIZE 1
#define CORTEX_M7_ETM_TRCACATR4_EXLEVEL_S_THREAD_OFFSET 8
#define CORTEX_M7_ETM_TRCACATR4_EXLEVEL_S_THREAD_SIZE 1
#define CORTEX_M7_ETM_TRCACATR4_EXLEVEL_S_HANDLER_OFFSET 11
#define CORTEX_M7_ETM_TRCACATR4_EXLEVEL_S_HANDLER_SIZE 1
#define CORTEX_M7_ETM_TRCACATR4_DATAMATCH_OFFSET 16
#define CORTEX_M7_ETM_TRCACATR4_DATAMATCH_SIZE 2
#define CORTEX_M7_ETM_TRCACATR4_DATASIZE_OFFSET 18
#define CORTEX_M7_ETM_TRCACATR4_DATASIZE_SIZE 2
#define CORTEX_M7_ETM_TRCACATR4_DATARANGE_OFFSET 20
#define CORTEX_M7_ETM_TRCACATR4_DATARANGE_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcacatr5 */
#define CORTEX_M7_ETM_TRCACATR5_TYPE_OFFSET 0
#define CORTEX_M7_ETM_TRCACATR5_TYPE_SIZE 2
#define CORTEX_M7_ETM_TRCACATR5_CONTEXTTYPE_OFFSET 2
#define CORTEX_M7_ETM_TRCACATR5_CONTEXTTYPE_SIZE 1
#define CORTEX_M7_ETM_TRCACATR5_EXLEVEL_S_0_OFFSET 8
#define CORTEX_M7_ETM_TRCACATR5_EXLEVEL_S_0_SIZE 1
#define CORTEX_M7_ETM_TRCACATR5_EXLEVEL_S_3_OFFSET 11
#define CORTEX_M7_ETM_TRCACATR5_EXLEVEL_S_3_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcacatr6 */
#define CORTEX_M7_ETM_TRCACATR6_TYPE_OFFSET 0
#define CORTEX_M7_ETM_TRCACATR6_TYPE_SIZE 2
#define CORTEX_M7_ETM_TRCACATR6_CONTEXTTYPE_OFFSET 2
#define CORTEX_M7_ETM_TRCACATR6_CONTEXTTYPE_SIZE 1
#define CORTEX_M7_ETM_TRCACATR6_EXLEVEL_S_THREAD_OFFSET 8
#define CORTEX_M7_ETM_TRCACATR6_EXLEVEL_S_THREAD_SIZE 1
#define CORTEX_M7_ETM_TRCACATR6_EXLEVEL_S_HANDLER_OFFSET 11
#define CORTEX_M7_ETM_TRCACATR6_EXLEVEL_S_HANDLER_SIZE 1
#define CORTEX_M7_ETM_TRCACATR6_DATAMATCH_OFFSET 16
#define CORTEX_M7_ETM_TRCACATR6_DATAMATCH_SIZE 2
#define CORTEX_M7_ETM_TRCACATR6_DATASIZE_OFFSET 18
#define CORTEX_M7_ETM_TRCACATR6_DATASIZE_SIZE 2
#define CORTEX_M7_ETM_TRCACATR6_DATARANGE_OFFSET 20
#define CORTEX_M7_ETM_TRCACATR6_DATARANGE_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcacatr7 */
#define CORTEX_M7_ETM_TRCACATR7_TYPE_OFFSET 0
#define CORTEX_M7_ETM_TRCACATR7_TYPE_SIZE 2
#define CORTEX_M7_ETM_TRCACATR7_CONTEXTTYPE_OFFSET 2
#define CORTEX_M7_ETM_TRCACATR7_CONTEXTTYPE_SIZE 1
#define CORTEX_M7_ETM_TRCACATR7_EXLEVEL_S_0_OFFSET 8
#define CORTEX_M7_ETM_TRCACATR7_EXLEVEL_S_0_SIZE 1
#define CORTEX_M7_ETM_TRCACATR7_EXLEVEL_S_3_OFFSET 11
#define CORTEX_M7_ETM_TRCACATR7_EXLEVEL_S_3_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcdvcvr0 */
#define CORTEX_M7_ETM_TRCDVCVR0_VALUE_OFFSET 0
#define CORTEX_M7_ETM_TRCDVCVR0_VALUE_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcdvcvr1 */
#define CORTEX_M7_ETM_TRCDVCVR1_VALUE_OFFSET 0
#define CORTEX_M7_ETM_TRCDVCVR1_VALUE_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcdvcmr0 */
#define CORTEX_M7_ETM_TRCDVCMR0_MASK_OFFSET 0
#define CORTEX_M7_ETM_TRCDVCMR0_MASK_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcdvcmr1 */
#define CORTEX_M7_ETM_TRCDVCMR1_MASK_OFFSET 0
#define CORTEX_M7_ETM_TRCDVCMR1_MASK_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcitmiscoutr */
#define CORTEX_M7_ETM_TRCITMISCOUTR_EXTOUT_OFFSET 8
#define CORTEX_M7_ETM_TRCITMISCOUTR_EXTOUT_SIZE 4

/** bit field defines for cortex_m7_etm_s#trcitmiscinr */
#define CORTEX_M7_ETM_TRCITMISCINR_EVENTS_OFFSET 0
#define CORTEX_M7_ETM_TRCITMISCINR_EVENTS_SIZE 4
#define CORTEX_M7_ETM_TRCITMISCINR_CPUACTIVE_OFFSET 4
#define CORTEX_M7_ETM_TRCITMISCINR_CPUACTIVE_SIZE 1
#define CORTEX_M7_ETM_TRCITMISCINR_HALTED_OFFSET 5
#define CORTEX_M7_ETM_TRCITMISCINR_HALTED_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcitatbidr */
#define CORTEX_M7_ETM_TRCITATBIDR_ID_OFFSET 0
#define CORTEX_M7_ETM_TRCITATBIDR_ID_SIZE 7

/** bit field defines for cortex_m7_etm_s#trcitddatar */
#define CORTEX_M7_ETM_TRCITDDATAR_ATDATAMD_OFFSET 0
#define CORTEX_M7_ETM_TRCITDDATAR_ATDATAMD_SIZE 9

/** bit field defines for cortex_m7_etm_s#trcitidatar */
#define CORTEX_M7_ETM_TRCITIDATAR_ATDATAMI_OFFSET 0
#define CORTEX_M7_ETM_TRCITIDATAR_ATDATAMI_SIZE 2

/** bit field defines for cortex_m7_etm_s#trcitdatbinr */
#define CORTEX_M7_ETM_TRCITDATBINR_ATREADYM_OFFSET 0
#define CORTEX_M7_ETM_TRCITDATBINR_ATREADYM_SIZE 1
#define CORTEX_M7_ETM_TRCITDATBINR_AFVALIDM_OFFSET 1
#define CORTEX_M7_ETM_TRCITDATBINR_AFVALIDM_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcitiatbinr */
#define CORTEX_M7_ETM_TRCITIATBINR_ATREADYM_OFFSET 0
#define CORTEX_M7_ETM_TRCITIATBINR_ATREADYM_SIZE 1
#define CORTEX_M7_ETM_TRCITIATBINR_AFVALIDM_OFFSET 1
#define CORTEX_M7_ETM_TRCITIATBINR_AFVALIDM_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcitdatboutr */
#define CORTEX_M7_ETM_TRCITDATBOUTR_ATVALID_OFFSET 0
#define CORTEX_M7_ETM_TRCITDATBOUTR_ATVALID_SIZE 1
#define CORTEX_M7_ETM_TRCITDATBOUTR_AFREADY_OFFSET 1
#define CORTEX_M7_ETM_TRCITDATBOUTR_AFREADY_SIZE 1
#define CORTEX_M7_ETM_TRCITDATBOUTR_ATBYTES_OFFSET 8
#define CORTEX_M7_ETM_TRCITDATBOUTR_ATBYTES_SIZE 3

/** bit field defines for cortex_m7_etm_s#trcitiatboutr */
#define CORTEX_M7_ETM_TRCITIATBOUTR_ATVALID_OFFSET 0
#define CORTEX_M7_ETM_TRCITIATBOUTR_ATVALID_SIZE 1
#define CORTEX_M7_ETM_TRCITIATBOUTR_AFREADY_OFFSET 1
#define CORTEX_M7_ETM_TRCITIATBOUTR_AFREADY_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcitctrl */
#define CORTEX_M7_ETM_TRCITCTRL_IME_OFFSET 0
#define CORTEX_M7_ETM_TRCITCTRL_IME_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcclaimset */
#define CORTEX_M7_ETM_TRCCLAIMSET_SET_OFFSET 0
#define CORTEX_M7_ETM_TRCCLAIMSET_SET_SIZE 4

/** bit field defines for cortex_m7_etm_s#trcclaimclr */
#define CORTEX_M7_ETM_TRCCLAIMCLR_CLR_OFFSET 0
#define CORTEX_M7_ETM_TRCCLAIMCLR_CLR_SIZE 4

/** bit field defines for cortex_m7_etm_s#trclar */
#define CORTEX_M7_ETM_TRCLAR_KEY_OFFSET 0
#define CORTEX_M7_ETM_TRCLAR_KEY_SIZE 32

/** bit field defines for cortex_m7_etm_s#trclsr */
#define CORTEX_M7_ETM_TRCLSR_SLI_OFFSET 0
#define CORTEX_M7_ETM_TRCLSR_SLI_SIZE 1
#define CORTEX_M7_ETM_TRCLSR_SLK_OFFSET 1
#define CORTEX_M7_ETM_TRCLSR_SLK_SIZE 1
#define CORTEX_M7_ETM_TRCLSR_NTT_OFFSET 2
#define CORTEX_M7_ETM_TRCLSR_NTT_SIZE 1

/** bit field defines for cortex_m7_etm_s#trcauthstatus */
#define CORTEX_M7_ETM_TRCAUTHSTATUS_NSID_OFFSET 0
#define CORTEX_M7_ETM_TRCAUTHSTATUS_NSID_SIZE 2
#define CORTEX_M7_ETM_TRCAUTHSTATUS_NSNID_OFFSET 2
#define CORTEX_M7_ETM_TRCAUTHSTATUS_NSNID_SIZE 2
#define CORTEX_M7_ETM_TRCAUTHSTATUS_SID_OFFSET 4
#define CORTEX_M7_ETM_TRCAUTHSTATUS_SID_SIZE 2
#define CORTEX_M7_ETM_TRCAUTHSTATUS_SNID_OFFSET 6
#define CORTEX_M7_ETM_TRCAUTHSTATUS_SNID_SIZE 2

/** bit field defines for cortex_m7_etm_s#trcdevarch */
#define CORTEX_M7_ETM_TRCDEVARCH_ARCHID_OFFSET 0
#define CORTEX_M7_ETM_TRCDEVARCH_ARCHID_SIZE 16
#define CORTEX_M7_ETM_TRCDEVARCH_REVISION_OFFSET 16
#define CORTEX_M7_ETM_TRCDEVARCH_REVISION_SIZE 4
#define CORTEX_M7_ETM_TRCDEVARCH_PRESENT_OFFSET 20
#define CORTEX_M7_ETM_TRCDEVARCH_PRESENT_SIZE 1
#define CORTEX_M7_ETM_TRCDEVARCH_ARCHITECT_OFFSET 21
#define CORTEX_M7_ETM_TRCDEVARCH_ARCHITECT_SIZE 11

/** bit field defines for cortex_m7_etm_s#trcdevid */
#define CORTEX_M7_ETM_TRCDEVID_RES0_OFFSET 0
#define CORTEX_M7_ETM_TRCDEVID_RES0_SIZE 32

/** bit field defines for cortex_m7_etm_s#trcdevtype */
#define CORTEX_M7_ETM_TRCDEVTYPE_MAJOR_OFFSET 0
#define CORTEX_M7_ETM_TRCDEVTYPE_MAJOR_SIZE 4
#define CORTEX_M7_ETM_TRCDEVTYPE_SUB_OFFSET 4
#define CORTEX_M7_ETM_TRCDEVTYPE_SUB_SIZE 4

/** bit field defines for cortex_m7_etm_s#trcpidr4 */
#define CORTEX_M7_ETM_TRCPIDR4_RES0_OFFSET 0
#define CORTEX_M7_ETM_TRCPIDR4_RES0_SIZE 8

/** bit field defines for cortex_m7_etm_s#trcpidr5 */
#define CORTEX_M7_ETM_TRCPIDR5_RES0_OFFSET 0
#define CORTEX_M7_ETM_TRCPIDR5_RES0_SIZE 8

/** bit field defines for cortex_m7_etm_s#trcpidr6 */
#define CORTEX_M7_ETM_TRCPIDR6_RES0_OFFSET 0
#define CORTEX_M7_ETM_TRCPIDR6_RES0_SIZE 8

/** bit field defines for cortex_m7_etm_s#trcpidr7 */
#define CORTEX_M7_ETM_TRCPIDR7_RES0_OFFSET 0
#define CORTEX_M7_ETM_TRCPIDR7_RES0_SIZE 8

/** bit field defines for cortex_m7_etm_s#trcpidr0 */
#define CORTEX_M7_ETM_TRCPIDR0_RES0_OFFSET 0
#define CORTEX_M7_ETM_TRCPIDR0_RES0_SIZE 8

/** bit field defines for cortex_m7_etm_s#trcpidr1 */
#define CORTEX_M7_ETM_TRCPIDR1_RES0_OFFSET 0
#define CORTEX_M7_ETM_TRCPIDR1_RES0_SIZE 8

/** bit field defines for cortex_m7_etm_s#trcpidr2 */
#define CORTEX_M7_ETM_TRCPIDR2_RES0_OFFSET 0
#define CORTEX_M7_ETM_TRCPIDR2_RES0_SIZE 8

/** bit field defines for cortex_m7_etm_s#trcpidr3 */
#define CORTEX_M7_ETM_TRCPIDR3_RES0_OFFSET 0
#define CORTEX_M7_ETM_TRCPIDR3_RES0_SIZE 8

/** bit field defines for cortex_m7_etm_s#trccidr0 */
#define CORTEX_M7_ETM_TRCCIDR0_RES0_OFFSET 0
#define CORTEX_M7_ETM_TRCCIDR0_RES0_SIZE 8

/** bit field defines for cortex_m7_etm_s#trccidr1 */
#define CORTEX_M7_ETM_TRCCIDR1_RES0_OFFSET 0
#define CORTEX_M7_ETM_TRCCIDR1_RES0_SIZE 8

/** bit field defines for cortex_m7_etm_s#trccidr2 */
#define CORTEX_M7_ETM_TRCCIDR2_RES0_OFFSET 0
#define CORTEX_M7_ETM_TRCCIDR2_RES0_SIZE 8

/** bit field defines for cortex_m7_etm_s#trccidr3 */
#define CORTEX_M7_ETM_TRCCIDR3_RES0_OFFSET 0
#define CORTEX_M7_ETM_TRCCIDR3_RES0_SIZE 8

/* EOF cortex_m7_etm.h */
#endif
