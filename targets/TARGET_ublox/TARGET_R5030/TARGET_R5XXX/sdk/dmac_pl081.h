#ifndef INCLUDED_DMAC_PL081
#define INCLUDED_DMAC_PL081
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** Number of PL081 DMAC Channels */
#define PL081_NR_CHANNELS 8
#define DMAC_PL081_PERIPHERALID0 0x81
#define DMAC_PL081_PERIPHERALID1 0x10
#define DMAC_PL081_PERIPHERALID2 0x05
#define DMAC_PL081_PERIPHERALID3 0x00
#define DMAC_PL081_COMPONENTID0 0x0D
#define DMAC_PL081_COMPONENTID1 0xF0
#define DMAC_PL081_COMPONENTID2 0x05
#define DMAC_PL081_COMPONENTID3 0xB1

/** PL081 DMAC Channel registers
*/
struct dmac_pl081_dmacchannels_s {
   /** Channel 0 Source Address register at address offset 0x000, read-write */
   uint32_t dmacc0srcaddr;
   /** Channel 0 Destination Address register at address offset 0x004, read-write */
   uint32_t dmacc0dstaddr;
   /** Channel 0 Linke List Item register.  Programming this register when the DMA channel is enabled has unpredictable side effects. at address offset 0x008, read-write */
   uint32_t dmacc0lli;
   /** Channel 0 Control register at address offset 0x00C, read-write */
   uint32_t dmacc0control;
   /** Channel 0 Configuration register at address offset 0x010, read-write */
   uint32_t dmacc0configuration;
   /** Reserved space \private at address offset 0x014, read-only */
   uint32_t fill[3];
};

/** bit field defines for dmac_pl081_dmacchannels_s#dmacc0srcaddr */
#define DMAC_PL081_DMACCHANNELS_DMACC0SRCADDR_DMACC0SRCADDR_OFFSET 0
#define DMAC_PL081_DMACCHANNELS_DMACC0SRCADDR_DMACC0SRCADDR_SIZE 32

/** bit field defines for dmac_pl081_dmacchannels_s#dmacc0dstaddr */
#define DMAC_PL081_DMACCHANNELS_DMACC0DSTADDR_DMACC0DSTADDR_OFFSET 0
#define DMAC_PL081_DMACCHANNELS_DMACC0DSTADDR_DMACC0DSTADDR_SIZE 32

/** bit field defines for dmac_pl081_dmacchannels_s#dmacc0lli */
#define DMAC_PL081_DMACCHANNELS_DMACC0LLI_LLI_OFFSET 2
#define DMAC_PL081_DMACCHANNELS_DMACC0LLI_LLI_SIZE 30

/** bit field defines for dmac_pl081_dmacchannels_s#dmacc0control */
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_TRANSFERSIZE_OFFSET 0
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_TRANSFERSIZE_SIZE 12
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_SBSIZE_OFFSET 12
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_SBSIZE_SIZE 3
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_DBSIZE_OFFSET 15
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_DBSIZE_SIZE 3
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_SWIDTH_OFFSET 18
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_SWIDTH_SIZE 3
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_DWIDTH_OFFSET 21
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_DWIDTH_SIZE 3
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_RESERVED_OFFSET 24
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_RESERVED_SIZE 2
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_SI_OFFSET 26
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_SI_SIZE 1
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_DI_OFFSET 27
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_DI_SIZE 1
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_PROT_OFFSET 28
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_PROT_SIZE 3
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_I_OFFSET 31
#define DMAC_PL081_DMACCHANNELS_DMACC0CONTROL_I_SIZE 1

/** bit field defines for dmac_pl081_dmacchannels_s#dmacc0configuration */
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_E_OFFSET 0
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_E_SIZE 1
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_SRCPERIPHERAL_OFFSET 1
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_SRCPERIPHERAL_SIZE 4
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_RESERVED0_OFFSET 5
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_RESERVED0_SIZE 1
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_DESTPERIPHERAL_OFFSET 6
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_DESTPERIPHERAL_SIZE 4
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_RESERVED1_OFFSET 10
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_RESERVED1_SIZE 1
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_FLOWCNTRL_OFFSET 11
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_FLOWCNTRL_SIZE 3
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_IE_OFFSET 14
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_IE_SIZE 1
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_ITC_OFFSET 15
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_ITC_SIZE 1
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_L_OFFSET 16
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_L_SIZE 1
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_A_OFFSET 17
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_A_SIZE 1
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_H_OFFSET 18
#define DMAC_PL081_DMACCHANNELS_DMACC0CONFIGURATION_H_SIZE 1

/** ARM PL081 Dma Controller
*/
struct dmac_pl081_s {
   /** Interrupt status register at address offset 0x000, read-only */
   uint32_t dmacintstatus;
   /** Interrupt Terminal Count status register at address offset 0x004, read-only */
   uint32_t dmacinttcstatus;
   /** Interrupt Terminal Count clear register at address offset 0x008, write-only */
   uint32_t dmacinttcclear;
   /** Interrupt Error status register at address offset 0x00C, read-only */
   uint32_t dmacinterrorstatus;
   /** Interrupt Error clear register at address offset 0x010, write-only */
   uint32_t dmacinterrorclear;
   /** Interrupt Terminal Count Raw status register at address offset 0x014, read-only */
   uint32_t dmacrawinttcstatus;
   /** Interrupt Error Raw status register at address offset 0x018, read-only */
   uint32_t dmacrawinterrorstatus;
   /** Enabled Channel register at address offset 0x01C, read-only */
   uint32_t dmacenbldchns;
   /** Software Burst Request register at address offset 0x020, read-write */
   uint32_t dmacsoftbreq;
   /** Software Single Request register at address offset 0x024, read-write */
   uint32_t dmacsoftsreq;
   /** Software Last Burst Request register at address offset 0x028, read-write */
   uint32_t dmacsoftlbreq;
   /** Software Last Single Request register at address offset 0x02C, read-write */
   uint32_t dmacsoftlsreq;
   /** Configuration register at address offset 0x030, read-write */
   uint32_t dmacconfiguration;
   /** Synchronization register at address offset 0x034, read-write */
   uint32_t dmacsync;
   /** Reserved space */
   uint8_t fill0[200];
   /** PL081 DMAC Channel registers at address offset 0x100 size: 256 */
   struct dmac_pl081_dmacchannels_s dmacchannels[8];
   /** Reserved space */
   uint8_t fill1[768];
   /** Integration Test Control register at address offset 0x500, read-write */
   uint32_t dmacitcr;
   /** Integration Test Output register 1 at address offset 0x504, read-write */
   uint32_t dmacitop1;
   /** Integration Test Output register 2 at address offset 0x508, read-write */
   uint32_t dmacitop2;
   /** Integration Test Output register 3 at address offset 0x50C, read-write */
   uint32_t dmacitop3;
   /** Reserved space */
   uint8_t fill2[2768];
   /** Peripheral ID0 at address offset 0xFE0, read-only constant 0x00000081 */
   uint32_t peripheralid0;
   /** Peripheral ID1 at address offset 0xFE4, read-only constant 0x00000010 */
   uint32_t peripheralid1;
   /** Peripheral ID2 at address offset 0xFE8, read-only constant 0x00000005 */
   uint32_t peripheralid2;
   /** Peripheral ID3 at address offset 0xFEC, read-only constant 0x00000000 */
   uint32_t peripheralid3;
   /** Component ID0 at address offset 0xFF0, read-only constant 0x0000000D */
   uint32_t componentid0;
   /** Component ID1 at address offset 0xFF4, read-only constant 0x000000F0 */
   uint32_t componentid1;
   /** Component ID2 at address offset 0xFF8, read-only constant 0x00000005 */
   uint32_t componentid2;
   /** Component ID3 at address offset 0xFFC, read-only constant 0x000000B1 */
   uint32_t componentid3;
};

/** bit field defines for dmac_pl081_s#dmacintstatus */
#define DMAC_PL081_DMACINTSTATUS_INTSTATUS_OFFSET 0
#define DMAC_PL081_DMACINTSTATUS_INTSTATUS_SIZE 8

/** bit field defines for dmac_pl081_s#dmacinttcstatus */
#define DMAC_PL081_DMACINTTCSTATUS_INTTCSTATUS_OFFSET 0
#define DMAC_PL081_DMACINTTCSTATUS_INTTCSTATUS_SIZE 8

/** bit field defines for dmac_pl081_s#dmacinttcclear */
#define DMAC_PL081_DMACINTTCCLEAR_INTTCCLEAR_OFFSET 0
#define DMAC_PL081_DMACINTTCCLEAR_INTTCCLEAR_SIZE 8

/** bit field defines for dmac_pl081_s#dmacinterrorstatus */
#define DMAC_PL081_DMACINTERRORSTATUS_INTERRORSTATUS_OFFSET 0
#define DMAC_PL081_DMACINTERRORSTATUS_INTERRORSTATUS_SIZE 8

/** bit field defines for dmac_pl081_s#dmacinterrorclear */
#define DMAC_PL081_DMACINTERRORCLEAR_INTERRCLR_OFFSET 0
#define DMAC_PL081_DMACINTERRORCLEAR_INTERRCLR_SIZE 8

/** bit field defines for dmac_pl081_s#dmacrawinttcstatus */
#define DMAC_PL081_DMACRAWINTTCSTATUS_RAWINTTCSTATUS_OFFSET 0
#define DMAC_PL081_DMACRAWINTTCSTATUS_RAWINTTCSTATUS_SIZE 8

/** bit field defines for dmac_pl081_s#dmacrawinterrorstatus */
#define DMAC_PL081_DMACRAWINTERRORSTATUS_RAWINTERRORSTATUS_OFFSET 0
#define DMAC_PL081_DMACRAWINTERRORSTATUS_RAWINTERRORSTATUS_SIZE 8

/** bit field defines for dmac_pl081_s#dmacenbldchns */
#define DMAC_PL081_DMACENBLDCHNS_ENABLEDCHANNELS_OFFSET 0
#define DMAC_PL081_DMACENBLDCHNS_ENABLEDCHANNELS_SIZE 8

/** bit field defines for dmac_pl081_s#dmacsoftbreq */
#define DMAC_PL081_DMACSOFTBREQ_SOFTBREQ_OFFSET 0
#define DMAC_PL081_DMACSOFTBREQ_SOFTBREQ_SIZE 16

/** bit field defines for dmac_pl081_s#dmacsoftsreq */
#define DMAC_PL081_DMACSOFTSREQ_SOFTSREQ_OFFSET 0
#define DMAC_PL081_DMACSOFTSREQ_SOFTSREQ_SIZE 16

/** bit field defines for dmac_pl081_s#dmacsoftlbreq */
#define DMAC_PL081_DMACSOFTLBREQ_SOFTLBREQ_OFFSET 0
#define DMAC_PL081_DMACSOFTLBREQ_SOFTLBREQ_SIZE 16

/** bit field defines for dmac_pl081_s#dmacsoftlsreq */
#define DMAC_PL081_DMACSOFTLSREQ_SOFTLSREQ_OFFSET 0
#define DMAC_PL081_DMACSOFTLSREQ_SOFTLSREQ_SIZE 16

/** bit field defines for dmac_pl081_s#dmacconfiguration */
#define DMAC_PL081_DMACCONFIGURATION_ENABLE_OFFSET 0
#define DMAC_PL081_DMACCONFIGURATION_ENABLE_SIZE 1
#define DMAC_PL081_DMACCONFIGURATION_ENDIANNESS_OFFSET 1
#define DMAC_PL081_DMACCONFIGURATION_ENDIANNESS_SIZE 1

/** bit field defines for dmac_pl081_s#dmacsync */
#define DMAC_PL081_DMACSYNC_DMACSYNC_OFFSET 0
#define DMAC_PL081_DMACSYNC_DMACSYNC_SIZE 16

/** bit field defines for dmac_pl081_s#dmacitcr */
#define DMAC_PL081_DMACITCR_ENABLE_OFFSET 0
#define DMAC_PL081_DMACITCR_ENABLE_SIZE 1

/** bit field defines for dmac_pl081_s#dmacitop1 */
#define DMAC_PL081_DMACITOP1_DMACCLR_OFFSET 0
#define DMAC_PL081_DMACITOP1_DMACCLR_SIZE 16

/** bit field defines for dmac_pl081_s#dmacitop2 */
#define DMAC_PL081_DMACITOP2_DMACTC_OFFSET 0
#define DMAC_PL081_DMACITOP2_DMACTC_SIZE 16

/** bit field defines for dmac_pl081_s#dmacitop3 */
#define DMAC_PL081_DMACITOP3_DMACINTTC_OFFSET 0
#define DMAC_PL081_DMACITOP3_DMACINTTC_SIZE 1
#define DMAC_PL081_DMACITOP3_DMACINTERR_OFFSET 1
#define DMAC_PL081_DMACITOP3_DMACINTERR_SIZE 1

/** bit field defines for dmac_pl081_s#peripheralid0 */
#define DMAC_PL081_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define DMAC_PL081_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for dmac_pl081_s#peripheralid1 */
#define DMAC_PL081_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define DMAC_PL081_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for dmac_pl081_s#peripheralid2 */
#define DMAC_PL081_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define DMAC_PL081_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for dmac_pl081_s#peripheralid3 */
#define DMAC_PL081_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define DMAC_PL081_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for dmac_pl081_s#componentid0 */
#define DMAC_PL081_COMPONENTID0_COMPONENTID0_OFFSET 0
#define DMAC_PL081_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for dmac_pl081_s#componentid1 */
#define DMAC_PL081_COMPONENTID1_COMPONENTID1_OFFSET 0
#define DMAC_PL081_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for dmac_pl081_s#componentid2 */
#define DMAC_PL081_COMPONENTID2_COMPONENTID2_OFFSET 0
#define DMAC_PL081_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for dmac_pl081_s#componentid3 */
#define DMAC_PL081_COMPONENTID3_COMPONENTID3_OFFSET 0
#define DMAC_PL081_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF dmac_pl081.h */
#endif
