#ifndef INCLUDED_TMC
#define INCLUDED_TMC
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

#define TMC_PERIPHERALID4 0x04
#define TMC_PERIPHERALID5 0x00
#define TMC_PERIPHERALID6 0x00
#define TMC_PERIPHERALID7 0x00
#define TMC_PERIPHERALID0 0x61
#define TMC_PERIPHERALID1 0xB9
#define TMC_PERIPHERALID2 0x1B
#define TMC_PERIPHERALID3 0x00
#define TMC_COMPONENTID0 0x0D
#define TMC_COMPONENTID1 0x90
#define TMC_COMPONENTID2 0x05
#define TMC_COMPONENTID3 0xB1

/** CoreSight Trace Memory Controller
*/
struct tmc_s {
   /** Reserved space */
   uint8_t fill0[4];
   /** RAM Size Register. at address offset 0x004, read-only */
   uint32_t rsz;
   /** Reserved space */
   uint8_t fill1[4];
   /** Status Register. at address offset 0x00C, read-only */
   uint32_t sts;
   /** RAM Read Data Register. at address offset 0x010, read-only */
   uint32_t rrd;
   /** RAM Read Pointer Register. at address offset 0x014, read-write */
   uint32_t rrp;
   /** RAM Write Pointer Register. at address offset 0x018, read-write */
   uint32_t rwp;
   /** Trigger Counter Register. at address offset 0x01C, read-write */
   uint32_t trg;
   /** Control Register. at address offset 0x020, read-write */
   uint32_t ctl;
   /** RAM Write Data Register. at address offset 0x024, write-only */
   uint32_t rwd;
   /** Mode Register. at address offset 0x028, read-write */
   uint32_t mode;
   /** Latched Buffer Fill Level. at address offset 0x02C, read-only */
   uint32_t lbuflevel;
   /** Circular Buffer Fill Level. at address offset 0x030, read-only */
   uint32_t cbuflevel;
   /** Buffer Level Water Mark. at address offset 0x034, read-write */
   uint32_t bufwm;
   /** RAM Read Pointer High Register. at address offset 0x038, read-write */
   uint32_t rrphi;
   /** RAM Write Pointer High Register. at address offset 0x03C, read-write */
   uint32_t rwphi;
   /** Reserved space */
   uint8_t fill2[208];
   /** AXI Control Register at address offset 0x110, read-write */
   uint32_t axictl;
   /** Reserved space */
   uint8_t fill3[4];
   /** Data Buffer Address Low Register at address offset 0x118, read-write */
   uint32_t dbalo;
   /** Data Buffer Address High Register at address offset 0x11C, read-write */
   uint32_t dbahi;
   /** Reserved space */
   uint8_t fill4[480];
   /** Formatter and Flush Status Register. at address offset 0x300, read-only */
   uint32_t ffsr;
   /** Formatter and Flush Control Register. at address offset 0x304, read-write */
   uint32_t ffcr;
   /** Periodic Synchronization Counter Register. at address offset 0x308, read-write */
   uint32_t pscr;
   /** Reserved space */
   uint8_t fill5[3012];
   /** Integration Test ATB Master Data Register 0. at address offset 0xED0, write-only */
   uint32_t itatbmdata0;
   /** Integration Test ATB Master Interface Control 2 Register. at address offset 0xED4, read-only */
   uint32_t itatbmctr2;
   /** Integration Test ATB Master Control Register 1. at address offset 0xED8, write-only */
   uint32_t itatbmctr1;
   /** Integration Test ATB Master Control 0 Register. at address offset 0xEDC, write-only */
   uint32_t itatbmctr0;
   /** Integration Test Miscallaneous Output Register 0. at address offset 0xEE0, write-only */
   uint32_t itmiscop0;
   /** Reserved space */
   uint8_t fill6[4];
   /** Integration Test Trigger In and Flush In Register. at address offset 0xEE8, read-only */
   uint32_t ittrflin;
   /** Integration Test ATB Data Register 0. at address offset 0xEEC, read-only */
   uint32_t itatbdata0;
   /** Integration Test ATB Control 2 Register. at address offset 0xEF0, write-only */
   uint32_t itatbctr2;
   /** Integration Test ATB Control 1 Register. at address offset 0xEF4, read-only */
   uint32_t itatbctr1;
   /** Integration Test ATB Control 0 Register. at address offset 0xEF8, read-only */
   uint32_t itatbctr0;
   /** Reserved space */
   uint8_t fill7[4];
   /** Integration Mode Control Register. at address offset 0xF00, read-write */
   uint32_t itctrl;
   /** Reserved space */
   uint8_t fill8[156];
   /** Claim Tag Set Register. at address offset 0xFA0, read-write */
   uint32_t claimset;
   /** Claim Tag Clear Register. at address offset 0xFA4, read-write */
   uint32_t claimclr;
   /** Reserved space */
   uint8_t fill9[8];
   /** Lock Access Register. at address offset 0xFB0, write-only */
   uint32_t lar;
   /** Lock Status Register. at address offset 0xFB4, read-only */
   uint32_t lsr;
   /** Authentication Status Register. at address offset 0xFB8, read-only */
   uint32_t authstatus;
   /** Reserved space */
   uint8_t fill10[12];
   /** Device configuration Register. at address offset 0xFC8, read-only */
   uint32_t devid;
   /** Device type register. at address offset 0xFCC, read-only */
   uint32_t devtype;
   /** Peripheral ID Register. at address offset 0xFD0, read-only constant 0x00000004 */
   uint32_t peripheralid4;
   /** Peripheral ID Register. at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** Peripheral ID Register. at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** Peripheral ID Register. at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** Peripheral ID Register. at address offset 0xFE0, read-only constant 0x00000061 */
   uint32_t peripheralid0;
   /** Peripheral ID Register. at address offset 0xFE4, read-only constant 0x000000B9 */
   uint32_t peripheralid1;
   /** Peripheral ID Register. at address offset 0xFE8, read-only constant 0x0000001B */
   uint32_t peripheralid2;
   /** Peripheral ID Register. at address offset 0xFEC, read-only constant 0x00000000 */
   uint32_t peripheralid3;
   /** Preamble. at address offset 0xFF0, read-only constant 0x0000000D */
   uint32_t componentid0;
   /** Component class is CoreSight Component. at address offset 0xFF4, read-only constant 0x00000090 */
   uint32_t componentid1;
   /** Preamble. at address offset 0xFF8, read-only constant 0x00000005 */
   uint32_t componentid2;
   /** Preamble. at address offset 0xFFC, read-only constant 0x000000B1 */
   uint32_t componentid3;
};

/** bit field defines for tmc_s#rsz */
#define TMC_RSZ_RSZ_OFFSET 0
#define TMC_RSZ_RSZ_SIZE 31

/** bit field defines for tmc_s#sts */
#define TMC_STS_FULL_OFFSET 0
#define TMC_STS_FULL_SIZE 1
#define TMC_STS_TRIGGERED_OFFSET 1
#define TMC_STS_TRIGGERED_SIZE 1
#define TMC_STS_TMCREADY_OFFSET 2
#define TMC_STS_TMCREADY_SIZE 1
#define TMC_STS_FTEMPTY_OFFSET 3
#define TMC_STS_FTEMPTY_SIZE 1
#define TMC_STS_EMPTY_OFFSET 4
#define TMC_STS_EMPTY_SIZE 1
#define TMC_STS_MEMERR_OFFSET 5
#define TMC_STS_MEMERR_SIZE 1

/** bit field defines for tmc_s#rrd */
#define TMC_RRD_RRD_OFFSET 0
#define TMC_RRD_RRD_SIZE 32

/** bit field defines for tmc_s#rrp */
#define TMC_RRP_RRP_OFFSET 0
#define TMC_RRP_RRP_SIZE 32

/** bit field defines for tmc_s#rwp */
#define TMC_RWP_RWP_OFFSET 0
#define TMC_RWP_RWP_SIZE 32

/** bit field defines for tmc_s#trg */
#define TMC_TRG_TRG_OFFSET 0
#define TMC_TRG_TRG_SIZE 32

/** bit field defines for tmc_s#ctl */
#define TMC_CTL_TRACECAPTEN_OFFSET 0
#define TMC_CTL_TRACECAPTEN_SIZE 1

/** bit field defines for tmc_s#rwd */
#define TMC_RWD_RWD_OFFSET 0
#define TMC_RWD_RWD_SIZE 32

/** bit field defines for tmc_s#mode */
#define TMC_MODE_MODE_OFFSET 0
#define TMC_MODE_MODE_SIZE 2

/** bit field defines for tmc_s#lbuflevel */
#define TMC_LBUFLEVEL_LBUFLEVEL_OFFSET 0
#define TMC_LBUFLEVEL_LBUFLEVEL_SIZE 32

/** bit field defines for tmc_s#cbuflevel */
#define TMC_CBUFLEVEL_CBUFLEVEL_OFFSET 0
#define TMC_CBUFLEVEL_CBUFLEVEL_SIZE 32

/** bit field defines for tmc_s#bufwm */
#define TMC_BUFWM_BUFWM_OFFSET 0
#define TMC_BUFWM_BUFWM_SIZE 32

/** bit field defines for tmc_s#rrphi */
#define TMC_RRPHI_RRPHI_OFFSET 0
#define TMC_RRPHI_RRPHI_SIZE 8

/** bit field defines for tmc_s#rwphi */
#define TMC_RWPHI_RWPHI_OFFSET 0
#define TMC_RWPHI_RWPHI_SIZE 8

/** bit field defines for tmc_s#axictl */
#define TMC_AXICTL_PROTCTRLBIT0_OFFSET 0
#define TMC_AXICTL_PROTCTRLBIT0_SIZE 1
#define TMC_AXICTL_PROTCTRLBIT1_OFFSET 1
#define TMC_AXICTL_PROTCTRLBIT1_SIZE 1
#define TMC_AXICTL_CACHECTRLBIT0_OFFSET 2
#define TMC_AXICTL_CACHECTRLBIT0_SIZE 1
#define TMC_AXICTL_CACHECTRLBIT1_OFFSET 3
#define TMC_AXICTL_CACHECTRLBIT1_SIZE 1
#define TMC_AXICTL_CACHECTRLBIT2_OFFSET 4
#define TMC_AXICTL_CACHECTRLBIT2_SIZE 1
#define TMC_AXICTL_CACHECTRLBIT3_OFFSET 5
#define TMC_AXICTL_CACHECTRLBIT3_SIZE 1
#define TMC_AXICTL_SCATTERGATHERMODE_OFFSET 7
#define TMC_AXICTL_SCATTERGATHERMODE_SIZE 1
#define TMC_AXICTL_WRBURSTLEN_OFFSET 8
#define TMC_AXICTL_WRBURSTLEN_SIZE 4

/** bit field defines for tmc_s#dbalo */
#define TMC_DBALO_BUFADDRLO_OFFSET 0
#define TMC_DBALO_BUFADDRLO_SIZE 32

/** bit field defines for tmc_s#dbahi */
#define TMC_DBAHI_BUFADDRLO_OFFSET 0
#define TMC_DBAHI_BUFADDRLO_SIZE 8

/** bit field defines for tmc_s#ffsr */
#define TMC_FFSR_FLINPROG_OFFSET 0
#define TMC_FFSR_FLINPROG_SIZE 1
#define TMC_FFSR_FTSTOPPED_OFFSET 1
#define TMC_FFSR_FTSTOPPED_SIZE 1

/** bit field defines for tmc_s#ffcr */
#define TMC_FFCR_ENFT_OFFSET 0
#define TMC_FFCR_ENFT_SIZE 1
#define TMC_FFCR_ENTI_OFFSET 1
#define TMC_FFCR_ENTI_SIZE 1
#define TMC_FFCR_FONFLIN_OFFSET 4
#define TMC_FFCR_FONFLIN_SIZE 1
#define TMC_FFCR_FONTRIGEVT_OFFSET 5
#define TMC_FFCR_FONTRIGEVT_SIZE 1
#define TMC_FFCR_FLUSHMAN_OFFSET 6
#define TMC_FFCR_FLUSHMAN_SIZE 1
#define TMC_FFCR_TRIGONTRIGIN_OFFSET 8
#define TMC_FFCR_TRIGONTRIGIN_SIZE 1
#define TMC_FFCR_TRIGONTRIGEVT_OFFSET 9
#define TMC_FFCR_TRIGONTRIGEVT_SIZE 1
#define TMC_FFCR_TRIGONFL_OFFSET 10
#define TMC_FFCR_TRIGONFL_SIZE 1
#define TMC_FFCR_STOPONFL_OFFSET 12
#define TMC_FFCR_STOPONFL_SIZE 1
#define TMC_FFCR_STOPONTRIGEVT_OFFSET 13
#define TMC_FFCR_STOPONTRIGEVT_SIZE 1
#define TMC_FFCR_DRAINBUFFER_OFFSET 14
#define TMC_FFCR_DRAINBUFFER_SIZE 1

/** bit field defines for tmc_s#pscr */
#define TMC_PSCR_PSCOUNT_OFFSET 0
#define TMC_PSCR_PSCOUNT_SIZE 5

/** bit field defines for tmc_s#itatbmdata0 */
#define TMC_ITATBMDATA0_ITATBMDATA0_OFFSET 0
#define TMC_ITATBMDATA0_ITATBMDATA0_SIZE 32

/** bit field defines for tmc_s#itatbmctr2 */
#define TMC_ITATBMCTR2_ITATBMCTR2_OFFSET 0
#define TMC_ITATBMCTR2_ITATBMCTR2_SIZE 32

/** bit field defines for tmc_s#itatbmctr1 */
#define TMC_ITATBMCTR1_ITATBMCTR1_OFFSET 0
#define TMC_ITATBMCTR1_ITATBMCTR1_SIZE 32

/** bit field defines for tmc_s#itatbmctr0 */
#define TMC_ITATBMCTR0_ITATBMCTR0_OFFSET 0
#define TMC_ITATBMCTR0_ITATBMCTR0_SIZE 32

/** bit field defines for tmc_s#itmiscop0 */
#define TMC_ITMISCOP0_ITMISCOP0_OFFSET 0
#define TMC_ITMISCOP0_ITMISCOP0_SIZE 32

/** bit field defines for tmc_s#ittrflin */
#define TMC_ITTRFLIN_ITTRFLIN_OFFSET 0
#define TMC_ITTRFLIN_ITTRFLIN_SIZE 32

/** bit field defines for tmc_s#itatbdata0 */
#define TMC_ITATBDATA0_ITATBDATA0_OFFSET 0
#define TMC_ITATBDATA0_ITATBDATA0_SIZE 32

/** bit field defines for tmc_s#itatbctr2 */
#define TMC_ITATBCTR2_ITATBCTR2_OFFSET 0
#define TMC_ITATBCTR2_ITATBCTR2_SIZE 32

/** bit field defines for tmc_s#itatbctr1 */
#define TMC_ITATBCTR1_ITATBCTR1_OFFSET 0
#define TMC_ITATBCTR1_ITATBCTR1_SIZE 32

/** bit field defines for tmc_s#itatbctr0 */
#define TMC_ITATBCTR0_ITATBCTR0_OFFSET 0
#define TMC_ITATBCTR0_ITATBCTR0_SIZE 32

/** bit field defines for tmc_s#itctrl */
#define TMC_ITCTRL_ITCTRL_OFFSET 0
#define TMC_ITCTRL_ITCTRL_SIZE 32

/** bit field defines for tmc_s#claimset */
#define TMC_CLAIMSET_CLAIMSET_OFFSET 0
#define TMC_CLAIMSET_CLAIMSET_SIZE 32

/** bit field defines for tmc_s#claimclr */
#define TMC_CLAIMCLR_CLAIMCLR_OFFSET 0
#define TMC_CLAIMCLR_CLAIMCLR_SIZE 32

/** bit field defines for tmc_s#lar */
#define TMC_LAR_LAR_OFFSET 0
#define TMC_LAR_LAR_SIZE 32

/** bit field defines for tmc_s#lsr */
#define TMC_LSR_LSR_OFFSET 0
#define TMC_LSR_LSR_SIZE 32

/** bit field defines for tmc_s#authstatus */
#define TMC_AUTHSTATUS_AUTHSTATUS_OFFSET 0
#define TMC_AUTHSTATUS_AUTHSTATUS_SIZE 32

/** bit field defines for tmc_s#devid */
#define TMC_DEVID_ATBINPORTCOUNT_OFFSET 0
#define TMC_DEVID_ATBINPORTCOUNT_SIZE 5
#define TMC_DEVID_CLKSCHEME_OFFSET 5
#define TMC_DEVID_CLKSCHEME_SIZE 1
#define TMC_DEVID_CONFIGTYPE_OFFSET 6
#define TMC_DEVID_CONFIGTYPE_SIZE 2
#define TMC_DEVID_MEMWIDTH_OFFSET 8
#define TMC_DEVID_MEMWIDTH_SIZE 3
#define TMC_DEVID_WBUFDEPTH_OFFSET 11
#define TMC_DEVID_WBUFDEPTH_SIZE 3

/** bit field defines for tmc_s#devtype */
#define TMC_DEVTYPE_DEVTYPE_OFFSET 0
#define TMC_DEVTYPE_DEVTYPE_SIZE 8

/** bit field defines for tmc_s#peripheralid4 */
#define TMC_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define TMC_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for tmc_s#peripheralid5 */
#define TMC_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define TMC_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for tmc_s#peripheralid6 */
#define TMC_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define TMC_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for tmc_s#peripheralid7 */
#define TMC_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define TMC_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for tmc_s#peripheralid0 */
#define TMC_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define TMC_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for tmc_s#peripheralid1 */
#define TMC_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define TMC_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for tmc_s#peripheralid2 */
#define TMC_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define TMC_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for tmc_s#peripheralid3 */
#define TMC_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define TMC_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for tmc_s#componentid0 */
#define TMC_COMPONENTID0_COMPONENTID0_OFFSET 0
#define TMC_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for tmc_s#componentid1 */
#define TMC_COMPONENTID1_COMPONENTID1_OFFSET 0
#define TMC_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for tmc_s#componentid2 */
#define TMC_COMPONENTID2_COMPONENTID2_OFFSET 0
#define TMC_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for tmc_s#componentid3 */
#define TMC_COMPONENTID3_COMPONENTID3_OFFSET 0
#define TMC_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF tmc.h */
#endif
