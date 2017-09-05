#ifndef INCLUDED_STM_CR
#define INCLUDED_STM_CR
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

#define STM_CR_PERIPHERALID4 0x04
#define STM_CR_PERIPHERALID5 0x00
#define STM_CR_PERIPHERALID6 0x00
#define STM_CR_PERIPHERALID7 0x00
#define STM_CR_PERIPHERALID0 0x62
#define STM_CR_PERIPHERALID1 0xB9
#define STM_CR_PERIPHERALID2 0x1B
#define STM_CR_PERIPHERALID3 0x00
#define STM_CR_COMPONENTID0 0x0D
#define STM_CR_COMPONENTID1 0x90
#define STM_CR_COMPONENTID2 0x05
#define STM_CR_COMPONENTID3 0xB1

/** Configuration Registers
*/
struct stm_cr_s {
   /** Stimulus port. Provides up to 32 stimulus ports. Write accesses to these basic stimulus ports are identical to write accesses to the I_DMTS variant of the corresponding extended stimulus ports 0-31 on master 0. Read accesses are used to determine if a future write to the register is accepted. at address offset 0x000, read-write */
   uint32_t stimr[32];
   /** Reserved space */
   uint8_t fill0[2948];
   /** DMA Transfer Start Register at address offset 0xC04, write-only */
   uint32_t dmastartr;
   /** DMA Transfer Stop Register at address offset 0xC08, write-only */
   uint32_t dmastopr;
   /** DMA Transfer Status Register at address offset 0xC0C, read-only */
   uint32_t dmastatr;
   /** DMA Control Register at address offset 0xC10, read-write */
   uint32_t dmactlr;
   /** Reserved space */
   uint8_t fill1[232];
   /** DMA ID Register at address offset 0xCFC, read-only */
   uint32_t dmaidr;
   /** Hardware Event Enable Register. This register must always be initialized for each bank before enabling event tracing in the STMHEMCR. at address offset 0xD00, read-write */
   uint32_t heer;
   /** Reserved space */
   uint8_t fill2[28];
   /** Hardware Event Trigger Enable Register. This register must always be initialized for each bank before enabling event tracing in the STMHEMCR. at address offset 0xD20, read-write */
   uint32_t heter;
   /** Reserved space */
   uint8_t fill3[64];
   /** Hardware Event Main Control Register at address offset 0xD64, read-write */
   uint32_t hemcr;
   /** Reserved space */
   uint8_t fill4[140];
   /** Hardware Event Master Number Register at address offset 0xDF4, read-only */
   uint32_t hemastr;
   /** Hardware Event Features 1 Register at address offset 0xDF8, read-only */
   uint32_t hefeat1r;
   /** Hardware Event ID Register at address offset 0xDFC, read-only */
   uint32_t heidr;
   /** Stimulus Port Enable Register at address offset 0xE00, read-write */
   uint32_t sper;
   /** Reserved space */
   uint8_t fill5[28];
   /** Stimulus Port Trigger Enable Register at address offset 0xE20, read-write */
   uint32_t spter;
   /** Reserved space */
   uint8_t fill6[60];
   /** Stimulus Port Select Configuration Register at address offset 0xE60, read-write */
   uint32_t spscr;
   /** Stimulus Port Master Select Configuration Register at address offset 0xE64, read-write */
   uint32_t spmscr;
   /** Stimulus Port Override Register at address offset 0xE68, read-write */
   uint32_t spoverrider;
   /** Stimulus Port Master Override Register at address offset 0xE6C, read-write */
   uint32_t spmoverrider;
   /** Stimulus Port Trigger Control and Status Register at address offset 0xE70, read-write */
   uint32_t sptrigcsr;
   /** Reserved space */
   uint8_t fill7[12];
   /** Trace Control and Status Register at address offset 0xE80, read-write */
   uint32_t tcsr;
   /** Timestamp Stimulus Register at address offset 0xE84, write-only */
   uint32_t tsstimr;
   /** Reserved space */
   uint8_t fill8[4];
   /** Timestamp Frequency Register at address offset 0xE8C, read-write */
   uint32_t tsfreqr;
   /** Synchronization Control Register at address offset 0xE90, read-write */
   uint32_t syncr;
   /** Auxiliary Control Register at address offset 0xE94, read-write */
   uint32_t auxcr;
   /** Reserved space */
   uint8_t fill9[8];
   /** Features 1 Register at address offset 0xEA0, read-only */
   uint32_t feat1r;
   /** Features 2 Register at address offset 0xEA4, read-only */
   uint32_t feat2r;
   /** Features 3 Register at address offset 0xEA8, read-only */
   uint32_t feat3r;
   /** Reserved space */
   uint8_t fill10[60];
   /** Integration test mode register for cross trigger outputs. at address offset 0xEE8, write-only */
   uint32_t ittrigger;
   /** Controls the value of the ATDATAM output in integration mode. at address offset 0xEEC, write-only */
   uint32_t itatbdata0;
   /** Returns the value of the ATREADYM and AFVALIDM inputs in integration mode. at address offset 0xEF0, read-only */
   uint32_t itatbctr2;
   /** Controls the valud of the ATIDM output in integration mode. at address offset 0xEF4, write-only */
   uint32_t itatbid;
   /** Controls the value of the ATVALIDM, AFREADYM and ATBYTESM outputs in integration mode. at address offset 0xEF8, write-only */
   uint32_t itatbctr0;
   /** Reserved space */
   uint8_t fill11[4];
   /** Integration Mode Control Register at address offset 0xF00, read-write */
   uint32_t itctrl;
   /** Reserved space */
   uint8_t fill12[156];
   /** Claim Tag Set Register at address offset 0xFA0, read-write */
   uint32_t claimset;
   /** Claim Tag Clear Register at address offset 0xFA4, read-write */
   uint32_t claimclr;
   /** Reserved space */
   uint8_t fill13[8];
   /** Lock Access Register at address offset 0xFB0, write-only */
   uint32_t lar;
   /** Lock Status Register at address offset 0xFB4, read-only */
   uint32_t lsr;
   /** Authentication Status Register at address offset 0xFB8, read-only */
   uint32_t authstatus;
   /** Reserved space */
   uint8_t fill14[12];
   /** Device Configuration Register at address offset 0xFC8, read-only */
   uint32_t devid;
   /** Device Type Register at address offset 0xFCC, read-only */
   uint32_t devtype;
   /** Peripheral ID Register at address offset 0xFD0, read-only constant 0x00000004 */
   uint32_t peripheralid4;
   /** Peripheral ID Register at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** Peripheral ID Register at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** Peripheral ID Register at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** Peripheral ID Register at address offset 0xFE0, read-only constant 0x00000062 */
   uint32_t peripheralid0;
   /** Peripheral ID Register at address offset 0xFE4, read-only constant 0x000000B9 */
   uint32_t peripheralid1;
   /** Peripheral ID Register at address offset 0xFE8, read-only constant 0x0000001B */
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

/** bit field defines for stm_cr_s#stimr */
#define STM_CR_STIMR_READY_OFFSET 0
#define STM_CR_STIMR_READY_SIZE 1

/** bit field defines for stm_cr_s#dmastartr */
#define STM_CR_DMASTARTR_START_OFFSET 0
#define STM_CR_DMASTARTR_START_SIZE 1

/** bit field defines for stm_cr_s#dmastopr */
#define STM_CR_DMASTOPR_STOP_OFFSET 0
#define STM_CR_DMASTOPR_STOP_SIZE 1

/** bit field defines for stm_cr_s#dmastatr */
#define STM_CR_DMASTATR_STATUS_OFFSET 0
#define STM_CR_DMASTATR_STATUS_SIZE 1

/** bit field defines for stm_cr_s#dmactlr */
#define STM_CR_DMACTLR_SENS_OFFSET 2
#define STM_CR_DMACTLR_SENS_SIZE 2

/** bit field defines for stm_cr_s#dmaidr */
#define STM_CR_DMAIDR_STM_CLASS_OFFSET 0
#define STM_CR_DMAIDR_STM_CLASS_SIZE 4
#define STM_CR_DMAIDR_STM_CLASSREV_OFFSET 4
#define STM_CR_DMAIDR_STM_CLASSREV_SIZE 4
#define STM_CR_DMAIDR_STM_VENDSPEC_OFFSET 8
#define STM_CR_DMAIDR_STM_VENDSPEC_SIZE 4

/** bit field defines for stm_cr_s#heer */
#define STM_CR_HEER_HEE_OFFSET 0
#define STM_CR_HEER_HEE_SIZE 32

/** bit field defines for stm_cr_s#heter */
#define STM_CR_HETER_HTE_OFFSET 0
#define STM_CR_HETER_HTE_SIZE 32

/** bit field defines for stm_cr_s#hemcr */
#define STM_CR_HEMCR_EN_OFFSET 0
#define STM_CR_HEMCR_EN_SIZE 1
#define STM_CR_HEMCR_COMPEN_OFFSET 1
#define STM_CR_HEMCR_COMPEN_SIZE 1
#define STM_CR_HEMCR_ERRDETECT_OFFSET 2
#define STM_CR_HEMCR_ERRDETECT_SIZE 1
#define STM_CR_HEMCR_TRIGCTL_OFFSET 4
#define STM_CR_HEMCR_TRIGCTL_SIZE 1
#define STM_CR_HEMCR_TRIGSTATUS_OFFSET 5
#define STM_CR_HEMCR_TRIGSTATUS_SIZE 1
#define STM_CR_HEMCR_TRIGCLEAR_OFFSET 6
#define STM_CR_HEMCR_TRIGCLEAR_SIZE 1
#define STM_CR_HEMCR_ATBTRIGEN_OFFSET 7
#define STM_CR_HEMCR_ATBTRIGEN_SIZE 1

/** bit field defines for stm_cr_s#hemastr */
#define STM_CR_HEMASTR_MASTER_OFFSET 0
#define STM_CR_HEMASTR_MASTER_SIZE 16

/** bit field defines for stm_cr_s#hefeat1r */
#define STM_CR_HEFEAT1R_HETER_OFFSET 0
#define STM_CR_HEFEAT1R_HETER_SIZE 1
#define STM_CR_HEFEAT1R_HEERR_OFFSET 2
#define STM_CR_HEFEAT1R_HEERR_SIZE 1
#define STM_CR_HEFEAT1R_HEMASTR_OFFSET 3
#define STM_CR_HEFEAT1R_HEMASTR_SIZE 1
#define STM_CR_HEFEAT1R_HECOMP_OFFSET 4
#define STM_CR_HEFEAT1R_HECOMP_SIZE 2
#define STM_CR_HEFEAT1R_NUMHE_OFFSET 15
#define STM_CR_HEFEAT1R_NUMHE_SIZE 9

/** bit field defines for stm_cr_s#heidr */
#define STM_CR_HEIDR_STM_CLASS_OFFSET 0
#define STM_CR_HEIDR_STM_CLASS_SIZE 4
#define STM_CR_HEIDR_STM_CLASSREV_OFFSET 4
#define STM_CR_HEIDR_STM_CLASSREV_SIZE 4
#define STM_CR_HEIDR_STM_VENDSPEC_OFFSET 8
#define STM_CR_HEIDR_STM_VENDSPEC_SIZE 4

/** bit field defines for stm_cr_s#sper */
#define STM_CR_SPER_SPE_OFFSET 0
#define STM_CR_SPER_SPE_SIZE 32

/** bit field defines for stm_cr_s#spter */
#define STM_CR_SPTER_SPTE_OFFSET 0
#define STM_CR_SPTER_SPTE_SIZE 32

/** bit field defines for stm_cr_s#spscr */
#define STM_CR_SPSCR_PORTCTL_OFFSET 0
#define STM_CR_SPSCR_PORTCTL_SIZE 2
#define STM_CR_SPSCR_PORTSEL_OFFSET 20
#define STM_CR_SPSCR_PORTSEL_SIZE 12

/** bit field defines for stm_cr_s#spmscr */
#define STM_CR_SPMSCR_MASTCTL_OFFSET 0
#define STM_CR_SPMSCR_MASTCTL_SIZE 1
#define STM_CR_SPMSCR_MASTSEL_OFFSET 15
#define STM_CR_SPMSCR_MASTSEL_SIZE 17

/** bit field defines for stm_cr_s#spoverrider */
#define STM_CR_SPOVERRIDER_OVERCTL_OFFSET 0
#define STM_CR_SPOVERRIDER_OVERCTL_SIZE 2
#define STM_CR_SPOVERRIDER_OVERTS_OFFSET 2
#define STM_CR_SPOVERRIDER_OVERTS_SIZE 1
#define STM_CR_SPOVERRIDER_PORTSEL_OFFSET 15
#define STM_CR_SPOVERRIDER_PORTSEL_SIZE 17

/** bit field defines for stm_cr_s#spmoverrider */
#define STM_CR_SPMOVERRIDER_MASTCTL_OFFSET 0
#define STM_CR_SPMOVERRIDER_MASTCTL_SIZE 1
#define STM_CR_SPMOVERRIDER_MASTSEL_OFFSET 15
#define STM_CR_SPMOVERRIDER_MASTSEL_SIZE 17

/** bit field defines for stm_cr_s#sptrigcsr */
#define STM_CR_SPTRIGCSR_TRIGCTL_OFFSET 0
#define STM_CR_SPTRIGCSR_TRIGCTL_SIZE 1
#define STM_CR_SPTRIGCSR_TRIGSTATUS_OFFSET 1
#define STM_CR_SPTRIGCSR_TRIGSTATUS_SIZE 1
#define STM_CR_SPTRIGCSR_TRIGCLEAR_OFFSET 2
#define STM_CR_SPTRIGCSR_TRIGCLEAR_SIZE 1
#define STM_CR_SPTRIGCSR_ATBTRIGEN_TE_OFFSET 3
#define STM_CR_SPTRIGCSR_ATBTRIGEN_TE_SIZE 1
#define STM_CR_SPTRIGCSR_ATBTRIGEN_DIR_OFFSET 4
#define STM_CR_SPTRIGCSR_ATBTRIGEN_DIR_SIZE 1

/** bit field defines for stm_cr_s#tcsr */
#define STM_CR_TCSR_EN_OFFSET 0
#define STM_CR_TCSR_EN_SIZE 1
#define STM_CR_TCSR_TSEN_OFFSET 1
#define STM_CR_TCSR_TSEN_SIZE 1
#define STM_CR_TCSR_SYNCEN_OFFSET 2
#define STM_CR_TCSR_SYNCEN_SIZE 1
#define STM_CR_TCSR_COMPEN_OFFSET 5
#define STM_CR_TCSR_COMPEN_SIZE 1
#define STM_CR_TCSR_TRACEID_OFFSET 16
#define STM_CR_TCSR_TRACEID_SIZE 7
#define STM_CR_TCSR_BUSY_OFFSET 23
#define STM_CR_TCSR_BUSY_SIZE 1

/** bit field defines for stm_cr_s#tsstimr */
#define STM_CR_TSSTIMR_FORCETS_OFFSET 0
#define STM_CR_TSSTIMR_FORCETS_SIZE 1

/** bit field defines for stm_cr_s#tsfreqr */
#define STM_CR_TSFREQR_FREQ_OFFSET 0
#define STM_CR_TSFREQR_FREQ_SIZE 32

/** bit field defines for stm_cr_s#syncr */
#define STM_CR_SYNCR_COUNT_OFFSET 0
#define STM_CR_SYNCR_COUNT_SIZE 12
#define STM_CR_SYNCR_MODE_OFFSET 12
#define STM_CR_SYNCR_MODE_SIZE 1

/** bit field defines for stm_cr_s#auxcr */
#define STM_CR_AUXCR_FIFOAF_OFFSET 0
#define STM_CR_AUXCR_FIFOAF_SIZE 1
#define STM_CR_AUXCR_ASYNCPE_OFFSET 1
#define STM_CR_AUXCR_ASYNCPE_SIZE 1
#define STM_CR_AUXCR_PRIORINVDIS_OFFSET 2
#define STM_CR_AUXCR_PRIORINVDIS_SIZE 1
#define STM_CR_AUXCR_CLKON_OFFSET 3
#define STM_CR_AUXCR_CLKON_SIZE 1
#define STM_CR_AUXCR_AFREADYHIGH_OFFSET 4
#define STM_CR_AUXCR_AFREADYHIGH_SIZE 1

/** bit field defines for stm_cr_s#feat1r */
#define STM_CR_FEAT1R_PROT_OFFSET 0
#define STM_CR_FEAT1R_PROT_SIZE 4
#define STM_CR_FEAT1R_TS_OFFSET 4
#define STM_CR_FEAT1R_TS_SIZE 2
#define STM_CR_FEAT1R_TSFREQ_OFFSET 6
#define STM_CR_FEAT1R_TSFREQ_SIZE 1
#define STM_CR_FEAT1R_FORCETS_OFFSET 7
#define STM_CR_FEAT1R_FORCETS_SIZE 1
#define STM_CR_FEAT1R_SYNC_OFFSET 8
#define STM_CR_FEAT1R_SYNC_SIZE 2
#define STM_CR_FEAT1R_TRACEBUS_OFFSET 10
#define STM_CR_FEAT1R_TRACEBUS_SIZE 4
#define STM_CR_FEAT1R_TRIGCTL_OFFSET 14
#define STM_CR_FEAT1R_TRIGCTL_SIZE 2
#define STM_CR_FEAT1R_TSPRESCALE_OFFSET 16
#define STM_CR_FEAT1R_TSPRESCALE_SIZE 2
#define STM_CR_FEAT1R_HWTEN_OFFSET 18
#define STM_CR_FEAT1R_HWTEN_SIZE 2
#define STM_CR_FEAT1R_SYNCEN_OFFSET 20
#define STM_CR_FEAT1R_SYNCEN_SIZE 2
#define STM_CR_FEAT1R_SWOEN_OFFSET 22
#define STM_CR_FEAT1R_SWOEN_SIZE 2

/** bit field defines for stm_cr_s#feat2r */
#define STM_CR_FEAT2R_SPTER_OFFSET 0
#define STM_CR_FEAT2R_SPTER_SIZE 2
#define STM_CR_FEAT2R_SPER_OFFSET 2
#define STM_CR_FEAT2R_SPER_SIZE 1
#define STM_CR_FEAT2R_SPCOMP_OFFSET 4
#define STM_CR_FEAT2R_SPCOMP_SIZE 2
#define STM_CR_FEAT2R_SPOVERRIDE_OFFSET 6
#define STM_CR_FEAT2R_SPOVERRIDE_SIZE 1
#define STM_CR_FEAT2R_PRIVMASK_OFFSET 7
#define STM_CR_FEAT2R_PRIVMASK_SIZE 2
#define STM_CR_FEAT2R_SPTRTYPE_OFFSET 9
#define STM_CR_FEAT2R_SPTRTYPE_SIZE 2
#define STM_CR_FEAT2R_DSIZE_OFFSET 12
#define STM_CR_FEAT2R_DSIZE_SIZE 4
#define STM_CR_FEAT2R_SPTYPE_OFFSET 16
#define STM_CR_FEAT2R_SPTYPE_SIZE 2

/** bit field defines for stm_cr_s#feat3r */
#define STM_CR_FEAT3R_NUMMAST_OFFSET 0
#define STM_CR_FEAT3R_NUMMAST_SIZE 7

/** bit field defines for stm_cr_s#ittrigger */
#define STM_CR_ITTRIGGER_TRIGOUTSPTE_W_OFFSET 0
#define STM_CR_ITTRIGGER_TRIGOUTSPTE_W_SIZE 1
#define STM_CR_ITTRIGGER_TRIGOUTSW_W_OFFSET 1
#define STM_CR_ITTRIGGER_TRIGOUTSW_W_SIZE 1
#define STM_CR_ITTRIGGER_TRIGOUTHETE_W_OFFSET 2
#define STM_CR_ITTRIGGER_TRIGOUTHETE_W_SIZE 1
#define STM_CR_ITTRIGGER_ASYNCOUT_W_OFFSET 3
#define STM_CR_ITTRIGGER_ASYNCOUT_W_SIZE 1

/** bit field defines for stm_cr_s#itatbdata0 */
#define STM_CR_ITATBDATA0_ATDATAM0_W_OFFSET 0
#define STM_CR_ITATBDATA0_ATDATAM0_W_SIZE 1
#define STM_CR_ITATBDATA0_ATDATAM7_W_OFFSET 1
#define STM_CR_ITATBDATA0_ATDATAM7_W_SIZE 1
#define STM_CR_ITATBDATA0_ATDATAM15_W_OFFSET 2
#define STM_CR_ITATBDATA0_ATDATAM15_W_SIZE 1
#define STM_CR_ITATBDATA0_ATDATAM23_W_OFFSET 3
#define STM_CR_ITATBDATA0_ATDATAM23_W_SIZE 1
#define STM_CR_ITATBDATA0_ATDATAM31_W_OFFSET 4
#define STM_CR_ITATBDATA0_ATDATAM31_W_SIZE 1

/** bit field defines for stm_cr_s#itatbctr2 */
#define STM_CR_ITATBCTR2_ATREADYM_R_OFFSET 0
#define STM_CR_ITATBCTR2_ATREADYM_R_SIZE 1
#define STM_CR_ITATBCTR2_AFVALIDM_R_OFFSET 1
#define STM_CR_ITATBCTR2_AFVALIDM_R_SIZE 1

/** bit field defines for stm_cr_s#itatbid */
#define STM_CR_ITATBID_ATIDM_OFFSET 0
#define STM_CR_ITATBID_ATIDM_SIZE 7

/** bit field defines for stm_cr_s#itatbctr0 */
#define STM_CR_ITATBCTR0_ATVALIDM_W_OFFSET 0
#define STM_CR_ITATBCTR0_ATVALIDM_W_SIZE 1
#define STM_CR_ITATBCTR0_AFREADYM_W_OFFSET 1
#define STM_CR_ITATBCTR0_AFREADYM_W_SIZE 1
#define STM_CR_ITATBCTR0_ATBYTESM_W_OFFSET 8
#define STM_CR_ITATBCTR0_ATBYTESM_W_SIZE 2

/** bit field defines for stm_cr_s#itctrl */
#define STM_CR_ITCTRL_INTEGRATION_MODE_OFFSET 0
#define STM_CR_ITCTRL_INTEGRATION_MODE_SIZE 1

/** bit field defines for stm_cr_s#claimset */
#define STM_CR_CLAIMSET_CLAIMSET_OFFSET 0
#define STM_CR_CLAIMSET_CLAIMSET_SIZE 32

/** bit field defines for stm_cr_s#claimclr */
#define STM_CR_CLAIMCLR_CLAIMCLR_OFFSET 0
#define STM_CR_CLAIMCLR_CLAIMCLR_SIZE 32

/** bit field defines for stm_cr_s#lar */
#define STM_CR_LAR_LAR_OFFSET 0
#define STM_CR_LAR_LAR_SIZE 32

/** bit field defines for stm_cr_s#lsr */
#define STM_CR_LSR_LSR_PRESENT_OFFSET 0
#define STM_CR_LSR_LSR_PRESENT_SIZE 1
#define STM_CR_LSR_LSR_LOCKED_OFFSET 1
#define STM_CR_LSR_LSR_LOCKED_SIZE 1
#define STM_CR_LSR_LSR_TYPE_OFFSET 2
#define STM_CR_LSR_LSR_TYPE_SIZE 1

/** bit field defines for stm_cr_s#authstatus */
#define STM_CR_AUTHSTATUS_NSID_OFFSET 0
#define STM_CR_AUTHSTATUS_NSID_SIZE 2
#define STM_CR_AUTHSTATUS_NSNID_OFFSET 2
#define STM_CR_AUTHSTATUS_NSNID_SIZE 2
#define STM_CR_AUTHSTATUS_SID_OFFSET 4
#define STM_CR_AUTHSTATUS_SID_SIZE 2
#define STM_CR_AUTHSTATUS_SNID_OFFSET 6
#define STM_CR_AUTHSTATUS_SNID_SIZE 2

/** bit field defines for stm_cr_s#devid */
#define STM_CR_DEVID_NUMSP_OFFSET 0
#define STM_CR_DEVID_NUMSP_SIZE 17

/** bit field defines for stm_cr_s#devtype */
#define STM_CR_DEVTYPE_DEVTYPE_OFFSET 0
#define STM_CR_DEVTYPE_DEVTYPE_SIZE 8

/** bit field defines for stm_cr_s#peripheralid4 */
#define STM_CR_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define STM_CR_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for stm_cr_s#peripheralid5 */
#define STM_CR_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define STM_CR_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for stm_cr_s#peripheralid6 */
#define STM_CR_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define STM_CR_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for stm_cr_s#peripheralid7 */
#define STM_CR_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define STM_CR_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for stm_cr_s#peripheralid0 */
#define STM_CR_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define STM_CR_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for stm_cr_s#peripheralid1 */
#define STM_CR_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define STM_CR_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for stm_cr_s#peripheralid2 */
#define STM_CR_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define STM_CR_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for stm_cr_s#peripheralid3 */
#define STM_CR_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define STM_CR_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for stm_cr_s#componentid0 */
#define STM_CR_COMPONENTID0_COMPONENTID0_OFFSET 0
#define STM_CR_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for stm_cr_s#componentid1 */
#define STM_CR_COMPONENTID1_COMPONENTID1_OFFSET 0
#define STM_CR_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for stm_cr_s#componentid2 */
#define STM_CR_COMPONENTID2_COMPONENTID2_OFFSET 0
#define STM_CR_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for stm_cr_s#componentid3 */
#define STM_CR_COMPONENTID3_COMPONENTID3_OFFSET 0
#define STM_CR_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF stm_cr.h */
#endif
