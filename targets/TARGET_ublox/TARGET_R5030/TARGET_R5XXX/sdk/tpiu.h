#ifndef INCLUDED_TPIU
#define INCLUDED_TPIU
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

#define TPIU_DEVID 0xA0
#define TPIU_DEVTYPE 0x11
#define TPIU_PERIPHERALID4 0x04
#define TPIU_PERIPHERALID0 0x12
#define TPIU_PERIPHERALID1 0xB9
#define TPIU_PERIPHERALID2 0x5B
#define TPIU_PERIPHERALID3 0x00
#define TPIU_COMPONENTID0 0x0D
#define TPIU_COMPONENTID1 0x90
#define TPIU_COMPONENTID2 0x05
#define TPIU_COMPONENTID3 0xB1

/** CoreSight Trace Port Interface Unit
*/
struct tpiu_s {
   /** Supported Port Sizes at address offset 0x000, read-only */
   uint32_t sportsize;
   /** Current Port Size - one-hot encoded (writing other values shall result in UNPREDICTABLE behaviour). at address offset 0x004, read-write */
   uint32_t cportsize;
   /** Reserved space */
   uint8_t fill0[248];
   /** Supported Trigger Modes at address offset 0x100, read-only */
   uint32_t trigmode;
   /** Trigger Counter Value at address offset 0x104, read-write */
   uint32_t trigcnt;
   /** Trigger Multiplier at address offset 0x108, read-write */
   uint32_t trigmlt;
   /** Reserved space */
   uint8_t fill1[244];
   /** Supported test pattern at address offset 0x200, read-only */
   uint32_t stestpat;
   /** Current test pattern at address offset 0x204, read-write */
   uint32_t ctestpat;
   /** Test pattern repeat counter at address offset 0x208, read-write */
   uint32_t testpatrpt;
   /** Reserved space */
   uint8_t fill2[244];
   /** Formatter and flush status at address offset 0x300, read-only */
   uint32_t ffstat;
   /** Formatter and flush control at address offset 0x304, read-write */
   uint32_t ffctrl;
   /** Formatter synchronization counter at address offset 0x308, read-write */
   uint32_t ffsync;
   /** Reserved space */
   uint8_t fill3[244];
   /** EXTCTL In Port at address offset 0x400, read-only */
   uint32_t extctlin;
   /** EXTCTL Out Port at address offset 0x404, read-write */
   uint32_t extctlout;
   /** Reserved space */
   uint8_t fill4[2780];
   /** Integration Test Trigger In and Flush In Acknowledge Register at address offset 0xEE4, write-only */
   uint32_t ittrflinack;
   /** Integration Test Trigger In and Flush In Register at address offset 0xEE8, read-only */
   uint32_t ittrflin;
   /** Integration Test ATB Data Register 0 at address offset 0xEEC, write-only */
   uint32_t itatbdata0;
   /** Integration Test ATB Control Register 2 at address offset 0xEF0, write-only */
   uint32_t itatbctr2;
   /** Integration Test ATB Control Register 1 at address offset 0xEF4, read-only */
   uint32_t itatbctr1;
   /** Integration Test ATB Control Register 0 at address offset 0xEF8, read-only */
   uint32_t itatbctr0;
   /** Reserved space */
   uint8_t fill5[4];
   /** Integration Mode Control Register at address offset 0xF00, read-write */
   uint32_t itmctrl;
   /** Reserved space */
   uint8_t fill6[156];
   /** Claim Tag Set Register at address offset 0xFA0, read-write */
   uint32_t claimset;
   /** Claim Tag Clear Register at address offset 0xFA4, read-write */
   uint32_t claimclr;
   /** Reserved space */
   uint8_t fill7[8];
   /** Lock Access register at address offset 0xFB0, write-only */
   uint32_t lockaccess;
   /** Lock Status Register at address offset 0xFB4, read-only */
   uint32_t lockstatus;
   /** Authentication Status Register at address offset 0xFB8, read-only */
   uint32_t authstatus;
   /** Reserved space */
   uint8_t fill8[12];
   /** Device configuration register at address offset 0xFC8, read-only constant 0x000000A0 */
   uint32_t devid;
   /** Device type register at address offset 0xFCC, read-only constant 0x00000011 */
   uint32_t devtype;
   /** Peripheral ID Register [39:32] at address offset 0xFD0, read-only constant 0x00000004 */
   uint32_t peripheralid4;
   /** Reserved space */
   uint8_t fill9[12];
   /** Peripheral ID Register [7:0] at address offset 0xFE0, read-only constant 0x00000012 */
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

/** bit field defines for tpiu_s#sportsize */
#define TPIU_SPORTSIZE_PORT_SIZE_1_OFFSET 0
#define TPIU_SPORTSIZE_PORT_SIZE_1_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_2_OFFSET 1
#define TPIU_SPORTSIZE_PORT_SIZE_2_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_3_OFFSET 2
#define TPIU_SPORTSIZE_PORT_SIZE_3_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_4_OFFSET 3
#define TPIU_SPORTSIZE_PORT_SIZE_4_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_5_OFFSET 4
#define TPIU_SPORTSIZE_PORT_SIZE_5_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_6_OFFSET 5
#define TPIU_SPORTSIZE_PORT_SIZE_6_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_7_OFFSET 6
#define TPIU_SPORTSIZE_PORT_SIZE_7_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_8_OFFSET 7
#define TPIU_SPORTSIZE_PORT_SIZE_8_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_9_OFFSET 8
#define TPIU_SPORTSIZE_PORT_SIZE_9_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_10_OFFSET 9
#define TPIU_SPORTSIZE_PORT_SIZE_10_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_11_OFFSET 10
#define TPIU_SPORTSIZE_PORT_SIZE_11_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_12_OFFSET 11
#define TPIU_SPORTSIZE_PORT_SIZE_12_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_13_OFFSET 12
#define TPIU_SPORTSIZE_PORT_SIZE_13_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_14_OFFSET 13
#define TPIU_SPORTSIZE_PORT_SIZE_14_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_15_OFFSET 14
#define TPIU_SPORTSIZE_PORT_SIZE_15_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_16_OFFSET 15
#define TPIU_SPORTSIZE_PORT_SIZE_16_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_17_OFFSET 16
#define TPIU_SPORTSIZE_PORT_SIZE_17_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_18_OFFSET 17
#define TPIU_SPORTSIZE_PORT_SIZE_18_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_19_OFFSET 18
#define TPIU_SPORTSIZE_PORT_SIZE_19_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_20_OFFSET 19
#define TPIU_SPORTSIZE_PORT_SIZE_20_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_21_OFFSET 20
#define TPIU_SPORTSIZE_PORT_SIZE_21_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_22_OFFSET 21
#define TPIU_SPORTSIZE_PORT_SIZE_22_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_23_OFFSET 22
#define TPIU_SPORTSIZE_PORT_SIZE_23_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_24_OFFSET 23
#define TPIU_SPORTSIZE_PORT_SIZE_24_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_25_OFFSET 24
#define TPIU_SPORTSIZE_PORT_SIZE_25_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_26_OFFSET 25
#define TPIU_SPORTSIZE_PORT_SIZE_26_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_27_OFFSET 26
#define TPIU_SPORTSIZE_PORT_SIZE_27_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_28_OFFSET 27
#define TPIU_SPORTSIZE_PORT_SIZE_28_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_29_OFFSET 28
#define TPIU_SPORTSIZE_PORT_SIZE_29_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_30_OFFSET 29
#define TPIU_SPORTSIZE_PORT_SIZE_30_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_31_OFFSET 30
#define TPIU_SPORTSIZE_PORT_SIZE_31_SIZE 1
#define TPIU_SPORTSIZE_PORT_SIZE_32_OFFSET 31
#define TPIU_SPORTSIZE_PORT_SIZE_32_SIZE 1

/** bit field defines for tpiu_s#cportsize */
#define TPIU_CPORTSIZE_PORT_SIZE_1_OFFSET 0
#define TPIU_CPORTSIZE_PORT_SIZE_1_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_2_OFFSET 1
#define TPIU_CPORTSIZE_PORT_SIZE_2_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_3_OFFSET 2
#define TPIU_CPORTSIZE_PORT_SIZE_3_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_4_OFFSET 3
#define TPIU_CPORTSIZE_PORT_SIZE_4_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_5_OFFSET 4
#define TPIU_CPORTSIZE_PORT_SIZE_5_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_6_OFFSET 5
#define TPIU_CPORTSIZE_PORT_SIZE_6_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_7_OFFSET 6
#define TPIU_CPORTSIZE_PORT_SIZE_7_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_8_OFFSET 7
#define TPIU_CPORTSIZE_PORT_SIZE_8_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_9_OFFSET 8
#define TPIU_CPORTSIZE_PORT_SIZE_9_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_10_OFFSET 9
#define TPIU_CPORTSIZE_PORT_SIZE_10_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_11_OFFSET 10
#define TPIU_CPORTSIZE_PORT_SIZE_11_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_12_OFFSET 11
#define TPIU_CPORTSIZE_PORT_SIZE_12_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_13_OFFSET 12
#define TPIU_CPORTSIZE_PORT_SIZE_13_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_14_OFFSET 13
#define TPIU_CPORTSIZE_PORT_SIZE_14_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_15_OFFSET 14
#define TPIU_CPORTSIZE_PORT_SIZE_15_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_16_OFFSET 15
#define TPIU_CPORTSIZE_PORT_SIZE_16_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_17_OFFSET 16
#define TPIU_CPORTSIZE_PORT_SIZE_17_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_18_OFFSET 17
#define TPIU_CPORTSIZE_PORT_SIZE_18_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_19_OFFSET 18
#define TPIU_CPORTSIZE_PORT_SIZE_19_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_20_OFFSET 19
#define TPIU_CPORTSIZE_PORT_SIZE_20_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_21_OFFSET 20
#define TPIU_CPORTSIZE_PORT_SIZE_21_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_22_OFFSET 21
#define TPIU_CPORTSIZE_PORT_SIZE_22_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_23_OFFSET 22
#define TPIU_CPORTSIZE_PORT_SIZE_23_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_24_OFFSET 23
#define TPIU_CPORTSIZE_PORT_SIZE_24_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_25_OFFSET 24
#define TPIU_CPORTSIZE_PORT_SIZE_25_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_26_OFFSET 25
#define TPIU_CPORTSIZE_PORT_SIZE_26_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_27_OFFSET 26
#define TPIU_CPORTSIZE_PORT_SIZE_27_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_28_OFFSET 27
#define TPIU_CPORTSIZE_PORT_SIZE_28_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_29_OFFSET 28
#define TPIU_CPORTSIZE_PORT_SIZE_29_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_30_OFFSET 29
#define TPIU_CPORTSIZE_PORT_SIZE_30_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_31_OFFSET 30
#define TPIU_CPORTSIZE_PORT_SIZE_31_SIZE 1
#define TPIU_CPORTSIZE_PORT_SIZE_32_OFFSET 31
#define TPIU_CPORTSIZE_PORT_SIZE_32_SIZE 1

/** bit field defines for tpiu_s#trigmode */
#define TPIU_TRIGMODE_MULT2_OFFSET 0
#define TPIU_TRIGMODE_MULT2_SIZE 1
#define TPIU_TRIGMODE_MULT4_OFFSET 1
#define TPIU_TRIGMODE_MULT4_SIZE 1
#define TPIU_TRIGMODE_MULT16_OFFSET 2
#define TPIU_TRIGMODE_MULT16_SIZE 1
#define TPIU_TRIGMODE_MULT256_OFFSET 3
#define TPIU_TRIGMODE_MULT256_SIZE 1
#define TPIU_TRIGMODE_MULT64K_OFFSET 4
#define TPIU_TRIGMODE_MULT64K_SIZE 1
#define TPIU_TRIGMODE_TCOUNT8_OFFSET 8
#define TPIU_TRIGMODE_TCOUNT8_SIZE 1
#define TPIU_TRIGMODE_TRIGGERED_OFFSET 16
#define TPIU_TRIGMODE_TRIGGERED_SIZE 1
#define TPIU_TRIGMODE_TRGRUN_OFFSET 17
#define TPIU_TRIGMODE_TRGRUN_SIZE 1

/** bit field defines for tpiu_s#trigcnt */
#define TPIU_TRIGCNT_TRIGCOUNT_OFFSET 0
#define TPIU_TRIGCNT_TRIGCOUNT_SIZE 8

/** bit field defines for tpiu_s#trigmlt */
#define TPIU_TRIGMLT_MULT2_OFFSET 0
#define TPIU_TRIGMLT_MULT2_SIZE 1
#define TPIU_TRIGMLT_MULT4_OFFSET 1
#define TPIU_TRIGMLT_MULT4_SIZE 1
#define TPIU_TRIGMLT_MULT16_OFFSET 2
#define TPIU_TRIGMLT_MULT16_SIZE 1
#define TPIU_TRIGMLT_MULT256_OFFSET 3
#define TPIU_TRIGMLT_MULT256_SIZE 1
#define TPIU_TRIGMLT_MULT64K_OFFSET 4
#define TPIU_TRIGMLT_MULT64K_SIZE 1

/** bit field defines for tpiu_s#stestpat */
#define TPIU_STESTPAT_PATW1_OFFSET 0
#define TPIU_STESTPAT_PATW1_SIZE 1
#define TPIU_STESTPAT_PATW0_OFFSET 1
#define TPIU_STESTPAT_PATW0_SIZE 1
#define TPIU_STESTPAT_PATA5_OFFSET 2
#define TPIU_STESTPAT_PATA5_SIZE 1
#define TPIU_STESTPAT_PATF0_OFFSET 3
#define TPIU_STESTPAT_PATF0_SIZE 1
#define TPIU_STESTPAT_PTIMEEN_OFFSET 16
#define TPIU_STESTPAT_PTIMEEN_SIZE 1
#define TPIU_STESTPAT_PCONTEN_OFFSET 17
#define TPIU_STESTPAT_PCONTEN_SIZE 1

/** bit field defines for tpiu_s#ctestpat */
#define TPIU_CTESTPAT_PATW1_OFFSET 0
#define TPIU_CTESTPAT_PATW1_SIZE 1
#define TPIU_CTESTPAT_PATW0_OFFSET 1
#define TPIU_CTESTPAT_PATW0_SIZE 1
#define TPIU_CTESTPAT_PATA5_OFFSET 2
#define TPIU_CTESTPAT_PATA5_SIZE 1
#define TPIU_CTESTPAT_PATF0_OFFSET 3
#define TPIU_CTESTPAT_PATF0_SIZE 1
#define TPIU_CTESTPAT_PTIMEEN_OFFSET 16
#define TPIU_CTESTPAT_PTIMEEN_SIZE 1
#define TPIU_CTESTPAT_PCONTEN_OFFSET 17
#define TPIU_CTESTPAT_PCONTEN_SIZE 1

/** bit field defines for tpiu_s#testpatrpt */
#define TPIU_TESTPATRPT_PATTCOUNT_OFFSET 0
#define TPIU_TESTPATRPT_PATTCOUNT_SIZE 8

/** bit field defines for tpiu_s#ffstat */
#define TPIU_FFSTAT_FLINPROG_OFFSET 0
#define TPIU_FFSTAT_FLINPROG_SIZE 1
#define TPIU_FFSTAT_FTSTOPPED_OFFSET 1
#define TPIU_FFSTAT_FTSTOPPED_SIZE 1
#define TPIU_FFSTAT_TCPRESENT_OFFSET 2
#define TPIU_FFSTAT_TCPRESENT_SIZE 1

/** bit field defines for tpiu_s#ffctrl */
#define TPIU_FFCTRL_ENFTC_OFFSET 0
#define TPIU_FFCTRL_ENFTC_SIZE 1
#define TPIU_FFCTRL_ENFCONT_OFFSET 1
#define TPIU_FFCTRL_ENFCONT_SIZE 1
#define TPIU_FFCTRL_FONFLIN_OFFSET 4
#define TPIU_FFCTRL_FONFLIN_SIZE 1
#define TPIU_FFCTRL_FONTRIG_OFFSET 5
#define TPIU_FFCTRL_FONTRIG_SIZE 1
#define TPIU_FFCTRL_FONMAN_OFFSET 6
#define TPIU_FFCTRL_FONMAN_SIZE 1
#define TPIU_FFCTRL_TRIGIN_OFFSET 8
#define TPIU_FFCTRL_TRIGIN_SIZE 1
#define TPIU_FFCTRL_TRIGEVT_OFFSET 9
#define TPIU_FFCTRL_TRIGEVT_SIZE 1
#define TPIU_FFCTRL_TRIGFL_OFFSET 10
#define TPIU_FFCTRL_TRIGFL_SIZE 1
#define TPIU_FFCTRL_STOPFL_OFFSET 12
#define TPIU_FFCTRL_STOPFL_SIZE 1
#define TPIU_FFCTRL_STOPTRIG_OFFSET 13
#define TPIU_FFCTRL_STOPTRIG_SIZE 1

/** bit field defines for tpiu_s#ffsync */
#define TPIU_FFSYNC_CYCCOUNT_OFFSET 0
#define TPIU_FFSYNC_CYCCOUNT_SIZE 12

/** bit field defines for tpiu_s#extctlin */
#define TPIU_EXTCTLIN_EXTCTLIN_OFFSET 0
#define TPIU_EXTCTLIN_EXTCTLIN_SIZE 8

/** bit field defines for tpiu_s#extctlout */
#define TPIU_EXTCTLOUT_EXTCTLOUT_OFFSET 0
#define TPIU_EXTCTLOUT_EXTCTLOUT_SIZE 8

/** bit field defines for tpiu_s#ittrflinack */
#define TPIU_ITTRFLINACK_ITTRFLINACK_OFFSET 0
#define TPIU_ITTRFLINACK_ITTRFLINACK_SIZE 2

/** bit field defines for tpiu_s#ittrflin */
#define TPIU_ITTRFLIN_ITTRFLIN_OFFSET 0
#define TPIU_ITTRFLIN_ITTRFLIN_SIZE 2

/** bit field defines for tpiu_s#itatbdata0 */
#define TPIU_ITATBDATA0_ITATBDATA0_OFFSET 0
#define TPIU_ITATBDATA0_ITATBDATA0_SIZE 5

/** bit field defines for tpiu_s#itatbctr2 */
#define TPIU_ITATBCTR2_ITATBCTR2_OFFSET 0
#define TPIU_ITATBCTR2_ITATBCTR2_SIZE 2

/** bit field defines for tpiu_s#itatbctr1 */
#define TPIU_ITATBCTR1_ITATBCTR1_OFFSET 0
#define TPIU_ITATBCTR1_ITATBCTR1_SIZE 7

/** bit field defines for tpiu_s#itatbctr0 */
#define TPIU_ITATBCTR0_ITATBCTR0_OFFSET 0
#define TPIU_ITATBCTR0_ITATBCTR0_SIZE 10

/** bit field defines for tpiu_s#itmctrl */
#define TPIU_ITMCTRL_ITMCTRL_OFFSET 0
#define TPIU_ITMCTRL_ITMCTRL_SIZE 1

/** bit field defines for tpiu_s#claimset */
#define TPIU_CLAIMSET_CLAIMSET_OFFSET 0
#define TPIU_CLAIMSET_CLAIMSET_SIZE 4

/** bit field defines for tpiu_s#claimclr */
#define TPIU_CLAIMCLR_CLAIMCLR_OFFSET 0
#define TPIU_CLAIMCLR_CLAIMCLR_SIZE 4

/** bit field defines for tpiu_s#lockaccess */
#define TPIU_LOCKACCESS_LOCKACCESS_OFFSET 0
#define TPIU_LOCKACCESS_LOCKACCESS_SIZE 32

/** bit field defines for tpiu_s#lockstatus */
#define TPIU_LOCKSTATUS_LOCKSTATUS_OFFSET 0
#define TPIU_LOCKSTATUS_LOCKSTATUS_SIZE 32

/** bit field defines for tpiu_s#authstatus */
#define TPIU_AUTHSTATUS_AUTHSTATUS_OFFSET 0
#define TPIU_AUTHSTATUS_AUTHSTATUS_SIZE 8

/** bit field defines for tpiu_s#devid */
#define TPIU_DEVID_MUXNUM_OFFSET 0
#define TPIU_DEVID_MUXNUM_SIZE 5
#define TPIU_DEVID_CLKRELAT_OFFSET 5
#define TPIU_DEVID_CLKRELAT_SIZE 1
#define TPIU_DEVID_FIFOSIZE_OFFSET 6
#define TPIU_DEVID_FIFOSIZE_SIZE 3
#define TPIU_DEVID_TCLKDATA_OFFSET 9
#define TPIU_DEVID_TCLKDATA_SIZE 1
#define TPIU_DEVID_SWOMAN_OFFSET 10
#define TPIU_DEVID_SWOMAN_SIZE 1
#define TPIU_DEVID_SWOUARTNRZ_OFFSET 11
#define TPIU_DEVID_SWOUARTNRZ_SIZE 1

/** bit field defines for tpiu_s#devtype */
#define TPIU_DEVTYPE_DEVTYPE_OFFSET 0
#define TPIU_DEVTYPE_DEVTYPE_SIZE 8

/** bit field defines for tpiu_s#peripheralid4 */
#define TPIU_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define TPIU_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for tpiu_s#peripheralid0 */
#define TPIU_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define TPIU_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for tpiu_s#peripheralid1 */
#define TPIU_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define TPIU_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for tpiu_s#peripheralid2 */
#define TPIU_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define TPIU_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for tpiu_s#peripheralid3 */
#define TPIU_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define TPIU_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for tpiu_s#componentid0 */
#define TPIU_COMPONENTID0_COMPONENTID0_OFFSET 0
#define TPIU_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for tpiu_s#componentid1 */
#define TPIU_COMPONENTID1_COMPONENTID1_OFFSET 0
#define TPIU_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for tpiu_s#componentid2 */
#define TPIU_COMPONENTID2_COMPONENTID2_OFFSET 0
#define TPIU_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for tpiu_s#componentid3 */
#define TPIU_COMPONENTID3_COMPONENTID3_OFFSET 0
#define TPIU_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF tpiu.h */
#endif
