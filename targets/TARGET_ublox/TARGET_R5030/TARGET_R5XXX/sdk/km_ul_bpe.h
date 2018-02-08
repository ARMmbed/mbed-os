#ifndef INCLUDED_KM_UL_BPE
#define INCLUDED_KM_UL_BPE
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** km_ul_bpe HAL Spreadsheet version number */
#define KM_UL_BPE_HAL_VERSION 2

/** This is the KM UL BPE register module
*/
struct km_ul_bpe_s {
   /**  at address offset 0x000, read-write */
   uint32_t ctrl;
   /**  at address offset 0x004, read-write */
   uint32_t transportblock;
   /**  at address offset 0x008, read-write */
   uint32_t symbolbuffer;
   /**  at address offset 0x00C, read-write */
   uint32_t cqi;
   /**  at address offset 0x010, read-write */
   uint32_t ack;
   /** General cinit as per 3gpp 36.211 7.2. 31-bit seed at address offset 0x014, read-write */
   uint32_t cinit;
   /**  at address offset 0x018, read-write */
   uint32_t modes;
   /** km_ul_bpe status at address offset 0x01C, read-only */
   uint32_t ulbpestatus;
   /**  at address offset 0x020, read-write */
   uint32_t srcaddr;
   /**  at address offset 0x024, read-write */
   uint32_t dstaddr;
   /**  at address offset 0x028, read-write */
   uint32_t enable;
   /**  at address offset 0x02C, read-only */
   uint32_t runtransportblock;
   /**  at address offset 0x030, read-only */
   uint32_t runsymbolbuffer;
   /**  at address offset 0x034, read-only */
   uint32_t runcqi;
   /**  at address offset 0x038, read-only */
   uint32_t runack;
   /**  at address offset 0x03C, read-only */
   uint32_t runcinit;
   /**  at address offset 0x040, read-only */
   uint32_t runmodes;
   /**  at address offset 0x044, read-write */
   uint32_t runsrcaddr;
   /**  at address offset 0x048, read-write */
   uint32_t rundstaddr;
   /** Reserved space */
   uint8_t fill0[436];
   /**  at address offset 0x200, write-only */
   uint32_t fault;
   /**  at address offset 0x204, read-only */
   uint32_t stat;
   /** Reserved space */
   uint8_t fill1[248];
   /** Clear fault at address offset 0x300, write-only */
   uint32_t fltclear;
   /** Fault status at address offset 0x304, read-only */
   uint32_t fltstatus;
   /** Raw fault status at address offset 0x308, read-only */
   uint32_t fltrawstatus;
   /** Fault mask clear at address offset 0x30C, write-only */
   uint32_t fltmaskclr;
   /** Fault mask set at address offset 0x310, write-only */
   uint32_t fltmaskset;
   /** Fault mask status at address offset 0x314, read-only */
   uint32_t fltmaskstatus;
   /** Reserved space */
   uint8_t fill2[3256];
   /** Peripheral ID4 Register at address offset 0xFD0, read-only */
   uint32_t peripheralid4;
   /** Peripheral ID5 Register at address offset 0xFD4, read-only */
   uint32_t peripheralid5;
   /** Peripheral ID6 Register at address offset 0xFD8, read-only */
   uint32_t peripheralid6;
   /** Peripheral ID7 Register at address offset 0xFDC, read-only */
   uint32_t peripheralid7;
   /** Peripheral ID0 Register at address offset 0xFE0, read-only */
   uint32_t peripheralid0;
   /** Peripheral ID1 Register at address offset 0xFE4, read-only */
   uint32_t peripheralid1;
   /** Peripheral ID2 Register at address offset 0xFE8, read-only */
   uint32_t peripheralid2;
   /** Peripheral ID3 Register at address offset 0xFEC, read-only */
   uint32_t peripheralid3;
   /** Primecell ID register 0 at address offset 0xFF0, read-only */
   uint32_t componentid0;
   /** Primecell ID register 1 at address offset 0xFF4, read-only */
   uint32_t componentid1;
   /** Primecell ID register 2 at address offset 0xFF8, read-only */
   uint32_t componentid2;
   /** Primecell ID register 3 at address offset 0xFFC, read-only */
   uint32_t componentid3;
};

/** bit field defines for km_ul_bpe_s#ctrl */
#define KM_UL_BPE_CTRL_ENABLESF_OFFSET 0
#define KM_UL_BPE_CTRL_ENABLESF_SIZE 1

/** bit field defines for km_ul_bpe_s#transportblock */
#define KM_UL_BPE_TRANSPORTBLOCK_TBS_OFFSET 3
#define KM_UL_BPE_TRANSPORTBLOCK_TBS_SIZE 7
#define KM_UL_BPE_TRANSPORTBLOCK_RV_OFFSET 10
#define KM_UL_BPE_TRANSPORTBLOCK_RV_SIZE 2

/** bit field defines for km_ul_bpe_s#symbolbuffer */
#define KM_UL_BPE_SYMBOLBUFFER_RBRU_OFFSET 0
#define KM_UL_BPE_SYMBOLBUFFER_RBRU_SIZE 4
#define KM_UL_BPE_SYMBOLBUFFER_CP_OFFSET 4
#define KM_UL_BPE_SYMBOLBUFFER_CP_SIZE 1
#define KM_UL_BPE_SYMBOLBUFFER_SRS_OFFSET 5
#define KM_UL_BPE_SYMBOLBUFFER_SRS_SIZE 1
#define KM_UL_BPE_SYMBOLBUFFER_QMIND_OFFSET 6
#define KM_UL_BPE_SYMBOLBUFFER_QMIND_SIZE 1
#define KM_UL_BPE_SYMBOLBUFFER_NSCRU_OFFSET 7
#define KM_UL_BPE_SYMBOLBUFFER_NSCRU_SIZE 2
/** 1 sub carrier */
#define KM_UL_BPE_SYMBOLBUFFER_NSCRU_SC1_VALUE 0
/** 12 sub carriers */
#define KM_UL_BPE_SYMBOLBUFFER_NSCRU_SC12_VALUE 3
/** 3 sub carriers */
#define KM_UL_BPE_SYMBOLBUFFER_NSCRU_SC3_VALUE 1
/** 6 sub carriers */
#define KM_UL_BPE_SYMBOLBUFFER_NSCRU_SC6_VALUE 2
#define KM_UL_BPE_SYMBOLBUFFER_BLOCKID_OFFSET 9
#define KM_UL_BPE_SYMBOLBUFFER_BLOCKID_SIZE 7

/** bit field defines for km_ul_bpe_s#cqi */
#define KM_UL_BPE_CQI_DCQI_OFFSET 0
#define KM_UL_BPE_CQI_DCQI_SIZE 11
#define KM_UL_BPE_CQI_QCQI_OFFSET 11
#define KM_UL_BPE_CQI_QCQI_SIZE 12

/** bit field defines for km_ul_bpe_s#ack */
#define KM_UL_BPE_ACK_OACK_OFFSET 0
#define KM_UL_BPE_ACK_OACK_SIZE 4
#define KM_UL_BPE_ACK_DACK_OFFSET 4
#define KM_UL_BPE_ACK_DACK_SIZE 9
#define KM_UL_BPE_ACK_QACK_OFFSET 13
#define KM_UL_BPE_ACK_QACK_SIZE 11
#define KM_UL_BPE_ACK_TABEN_OFFSET 24
#define KM_UL_BPE_ACK_TABEN_SIZE 1
#define KM_UL_BPE_ACK_WSEL_OFFSET 25
#define KM_UL_BPE_ACK_WSEL_SIZE 2

/** bit field defines for km_ul_bpe_s#cinit */
#define KM_UL_BPE_CINIT_NONE_OFFSET 0
#define KM_UL_BPE_CINIT_NONE_SIZE 31

/** bit field defines for km_ul_bpe_s#modes */
#define KM_UL_BPE_MODES_MODENBIOT_OFFSET 0
#define KM_UL_BPE_MODES_MODENBIOT_SIZE 1
#define KM_UL_BPE_MODES_MODETRBKRPT_OFFSET 1
#define KM_UL_BPE_MODES_MODETRBKRPT_SIZE 1
#define KM_UL_BPE_MODES_MODETRBKLSBFIRST_OFFSET 2
#define KM_UL_BPE_MODES_MODETRBKLSBFIRST_SIZE 1
#define KM_UL_BPE_MODES_MODERMBUFEN_OFFSET 3
#define KM_UL_BPE_MODES_MODERMBUFEN_SIZE 1
#define KM_UL_BPE_MODES_MODESCRAMBUFEN_OFFSET 4
#define KM_UL_BPE_MODES_MODESCRAMBUFEN_SIZE 1
#define KM_UL_BPE_MODES_MODENOSCRAMEN_OFFSET 5
#define KM_UL_BPE_MODES_MODENOSCRAMEN_SIZE 1
#define KM_UL_BPE_MODES_MODEONESC_OFFSET 6
#define KM_UL_BPE_MODES_MODEONESC_SIZE 1

/** bit field defines for km_ul_bpe_s#ulbpestatus */
#define KM_UL_BPE_ULBPESTATUS_MASTERFSM_OFFSET 0
#define KM_UL_BPE_ULBPESTATUS_MASTERFSM_SIZE 2
/**  */
#define KM_UL_BPE_ULBPESTATUS_MASTERFSM_IDLE_VALUE 0
/** aka output */
#define KM_UL_BPE_ULBPESTATUS_MASTERFSM_RATEMATCH_VALUE 2
/** aka input */
#define KM_UL_BPE_ULBPESTATUS_MASTERFSM_ENCODE_VALUE 1
#define KM_UL_BPE_ULBPESTATUS_ENCODEFSM_OFFSET 2
#define KM_UL_BPE_ULBPESTATUS_ENCODEFSM_SIZE 3
/** encode multiplexed ack (small encoder) */
#define KM_UL_BPE_ULBPESTATUS_ENCODEFSM_ACK_MX_VALUE 7
/** encode cqi (small encoder) */
#define KM_UL_BPE_ULBPESTATUS_ENCODEFSM_CQI_VALUE 6
/** idle */
#define KM_UL_BPE_ULBPESTATUS_ENCODEFSM_IDLE_VALUE 0
/** read sys interleaved -> compute p2 (including crc) */
#define KM_UL_BPE_ULBPESTATUS_ENCODEFSM_P2_VALUE 4
/** write sys_crc, write p1_crc */
#define KM_UL_BPE_ULBPESTATUS_ENCODEFSM_SYS_CRC_VALUE 2
/** trbk input -> compute crc, write sys, encode p1 */
#define KM_UL_BPE_ULBPESTATUS_ENCODEFSM_SYS_P1_VALUE 1
/** write tail */
#define KM_UL_BPE_ULBPESTATUS_ENCODEFSM_TAIL_1ST_VALUE 3
/** write tail */
#define KM_UL_BPE_ULBPESTATUS_ENCODEFSM_TAIL_2ND_VALUE 5
#define KM_UL_BPE_ULBPESTATUS_RMCOL_OFFSET 5
#define KM_UL_BPE_ULBPESTATUS_RMCOL_SIZE 4
#define KM_UL_BPE_ULBPESTATUS_RMROW_OFFSET 9
#define KM_UL_BPE_ULBPESTATUS_RMROW_SIZE 7

/** bit field defines for km_ul_bpe_s#srcaddr */
#define KM_UL_BPE_SRCADDR_SRCADDR_OFFSET 2
#define KM_UL_BPE_SRCADDR_SRCADDR_SIZE 30

/** bit field defines for km_ul_bpe_s#dstaddr */
#define KM_UL_BPE_DSTADDR_DSTADDR_OFFSET 2
#define KM_UL_BPE_DSTADDR_DSTADDR_SIZE 30

/** bit field defines for km_ul_bpe_s#enable */
#define KM_UL_BPE_ENABLE_EVENTS_OFFSET 0
#define KM_UL_BPE_ENABLE_EVENTS_SIZE 1
#define KM_UL_BPE_ENABLE_SEV_OFFSET 1
#define KM_UL_BPE_ENABLE_SEV_SIZE 1

/** bit field defines for km_ul_bpe_s#runtransportblock */
#define KM_UL_BPE_RUNTRANSPORTBLOCK_TBS_OFFSET 3
#define KM_UL_BPE_RUNTRANSPORTBLOCK_TBS_SIZE 7
#define KM_UL_BPE_RUNTRANSPORTBLOCK_RV_OFFSET 10
#define KM_UL_BPE_RUNTRANSPORTBLOCK_RV_SIZE 2

/** bit field defines for km_ul_bpe_s#runsymbolbuffer */
#define KM_UL_BPE_RUNSYMBOLBUFFER_RBRU_OFFSET 0
#define KM_UL_BPE_RUNSYMBOLBUFFER_RBRU_SIZE 4
#define KM_UL_BPE_RUNSYMBOLBUFFER_CP_OFFSET 4
#define KM_UL_BPE_RUNSYMBOLBUFFER_CP_SIZE 1
#define KM_UL_BPE_RUNSYMBOLBUFFER_SRS_OFFSET 5
#define KM_UL_BPE_RUNSYMBOLBUFFER_SRS_SIZE 1
#define KM_UL_BPE_RUNSYMBOLBUFFER_QMIND_OFFSET 6
#define KM_UL_BPE_RUNSYMBOLBUFFER_QMIND_SIZE 1
#define KM_UL_BPE_RUNSYMBOLBUFFER_NSCRU_OFFSET 7
#define KM_UL_BPE_RUNSYMBOLBUFFER_NSCRU_SIZE 2
/**  */
#define KM_UL_BPE_RUNSYMBOLBUFFER_NSCRU_SC1_VALUE 0
/**  */
#define KM_UL_BPE_RUNSYMBOLBUFFER_NSCRU_SC12_VALUE 3
/**  */
#define KM_UL_BPE_RUNSYMBOLBUFFER_NSCRU_SC3_VALUE 1
/**  */
#define KM_UL_BPE_RUNSYMBOLBUFFER_NSCRU_SC6_VALUE 2
#define KM_UL_BPE_RUNSYMBOLBUFFER_BLOCKID_OFFSET 9
#define KM_UL_BPE_RUNSYMBOLBUFFER_BLOCKID_SIZE 7

/** bit field defines for km_ul_bpe_s#runcqi */
#define KM_UL_BPE_RUNCQI_DCQI_OFFSET 0
#define KM_UL_BPE_RUNCQI_DCQI_SIZE 11
#define KM_UL_BPE_RUNCQI_QCQI_OFFSET 11
#define KM_UL_BPE_RUNCQI_QCQI_SIZE 12

/** bit field defines for km_ul_bpe_s#runack */
#define KM_UL_BPE_RUNACK_OACK_OFFSET 0
#define KM_UL_BPE_RUNACK_OACK_SIZE 3
#define KM_UL_BPE_RUNACK_DACK_OFFSET 3
#define KM_UL_BPE_RUNACK_DACK_SIZE 4
#define KM_UL_BPE_RUNACK_QACK_OFFSET 7
#define KM_UL_BPE_RUNACK_QACK_SIZE 11
#define KM_UL_BPE_RUNACK_TABEN_OFFSET 18
#define KM_UL_BPE_RUNACK_TABEN_SIZE 1
#define KM_UL_BPE_RUNACK_WSEL_OFFSET 19
#define KM_UL_BPE_RUNACK_WSEL_SIZE 2

/** bit field defines for km_ul_bpe_s#runcinit */
#define KM_UL_BPE_RUNCINIT_NONE_OFFSET 0
#define KM_UL_BPE_RUNCINIT_NONE_SIZE 31

/** bit field defines for km_ul_bpe_s#runmodes */
#define KM_UL_BPE_RUNMODES_MODENBIOT_OFFSET 0
#define KM_UL_BPE_RUNMODES_MODENBIOT_SIZE 1
#define KM_UL_BPE_RUNMODES_MODETRBKRPT_OFFSET 1
#define KM_UL_BPE_RUNMODES_MODETRBKRPT_SIZE 1
#define KM_UL_BPE_RUNMODES_MODERMBUFEN_OFFSET 2
#define KM_UL_BPE_RUNMODES_MODERMBUFEN_SIZE 1
#define KM_UL_BPE_RUNMODES_MODESCRAMBUFEN_OFFSET 3
#define KM_UL_BPE_RUNMODES_MODESCRAMBUFEN_SIZE 1
#define KM_UL_BPE_RUNMODES_MODENOSCRAMEN_OFFSET 4
#define KM_UL_BPE_RUNMODES_MODENOSCRAMEN_SIZE 1
#define KM_UL_BPE_RUNMODES_MODEONESC_OFFSET 5
#define KM_UL_BPE_RUNMODES_MODEONESC_SIZE 1

/** bit field defines for km_ul_bpe_s#runsrcaddr */
#define KM_UL_BPE_RUNSRCADDR_SRCADDR_OFFSET 2
#define KM_UL_BPE_RUNSRCADDR_SRCADDR_SIZE 30

/** bit field defines for km_ul_bpe_s#rundstaddr */
#define KM_UL_BPE_RUNDSTADDR_DSTADDR_OFFSET 2
#define KM_UL_BPE_RUNDSTADDR_DSTADDR_SIZE 30

/** bit field defines for km_ul_bpe_s#fault */
#define KM_UL_BPE_FAULT_PENDINGSUSPEND_OFFSET 0
#define KM_UL_BPE_FAULT_PENDINGSUSPEND_SIZE 1
#define KM_UL_BPE_FAULT_PENDINGABORT_OFFSET 1
#define KM_UL_BPE_FAULT_PENDINGABORT_SIZE 1
#define KM_UL_BPE_FAULT_RUNNINGSUSPEND_OFFSET 2
#define KM_UL_BPE_FAULT_RUNNINGSUSPEND_SIZE 1
#define KM_UL_BPE_FAULT_RUNNINGABORT_OFFSET 3
#define KM_UL_BPE_FAULT_RUNNINGABORT_SIZE 1

/** bit field defines for km_ul_bpe_s#stat */
#define KM_UL_BPE_STAT_PENDINGSUSPENDED_OFFSET 0
#define KM_UL_BPE_STAT_PENDINGSUSPENDED_SIZE 1
#define KM_UL_BPE_STAT_PENDINGVALID_OFFSET 1
#define KM_UL_BPE_STAT_PENDINGVALID_SIZE 1
#define KM_UL_BPE_STAT_RUNNINGSUSPENDED_OFFSET 2
#define KM_UL_BPE_STAT_RUNNINGSUSPENDED_SIZE 1
#define KM_UL_BPE_STAT_RUNNINGABORTBUSY_OFFSET 3
#define KM_UL_BPE_STAT_RUNNINGABORTBUSY_SIZE 1
#define KM_UL_BPE_STAT_RUNNINGBUSY_OFFSET 4
#define KM_UL_BPE_STAT_RUNNINGBUSY_SIZE 1

/** bit field defines for km_ul_bpe_s#fltclear */
#define KM_UL_BPE_FLTCLEAR_AXIREADERROR_OFFSET 0
#define KM_UL_BPE_FLTCLEAR_AXIREADERROR_SIZE 1
#define KM_UL_BPE_FLTCLEAR_AXIWRITEERROR_OFFSET 1
#define KM_UL_BPE_FLTCLEAR_AXIWRITEERROR_SIZE 1

/** bit field defines for km_ul_bpe_s#fltstatus */
#define KM_UL_BPE_FLTSTATUS_AXIREADERROR_OFFSET 0
#define KM_UL_BPE_FLTSTATUS_AXIREADERROR_SIZE 1
#define KM_UL_BPE_FLTSTATUS_AXIWRITEERROR_OFFSET 1
#define KM_UL_BPE_FLTSTATUS_AXIWRITEERROR_SIZE 1

/** bit field defines for km_ul_bpe_s#fltrawstatus */
#define KM_UL_BPE_FLTRAWSTATUS_AXIREADERROR_OFFSET 0
#define KM_UL_BPE_FLTRAWSTATUS_AXIREADERROR_SIZE 1
#define KM_UL_BPE_FLTRAWSTATUS_AXIWRITEERROR_OFFSET 1
#define KM_UL_BPE_FLTRAWSTATUS_AXIWRITEERROR_SIZE 1

/** bit field defines for km_ul_bpe_s#fltmaskclr */
#define KM_UL_BPE_FLTMASKCLR_AXIREADERROR_OFFSET 0
#define KM_UL_BPE_FLTMASKCLR_AXIREADERROR_SIZE 1
#define KM_UL_BPE_FLTMASKCLR_AXIWRITEERROR_OFFSET 1
#define KM_UL_BPE_FLTMASKCLR_AXIWRITEERROR_SIZE 1

/** bit field defines for km_ul_bpe_s#fltmaskset */
#define KM_UL_BPE_FLTMASKSET_AXIREADERROR_OFFSET 0
#define KM_UL_BPE_FLTMASKSET_AXIREADERROR_SIZE 1
#define KM_UL_BPE_FLTMASKSET_AXIWRITEERROR_OFFSET 1
#define KM_UL_BPE_FLTMASKSET_AXIWRITEERROR_SIZE 1

/** bit field defines for km_ul_bpe_s#fltmaskstatus */
#define KM_UL_BPE_FLTMASKSTATUS_AXIREADERROR_OFFSET 0
#define KM_UL_BPE_FLTMASKSTATUS_AXIREADERROR_SIZE 1
#define KM_UL_BPE_FLTMASKSTATUS_AXIWRITEERROR_OFFSET 1
#define KM_UL_BPE_FLTMASKSTATUS_AXIWRITEERROR_SIZE 1

/** bit field defines for km_ul_bpe_s#peripheralid4 */
#define KM_UL_BPE_PERIPHERALID4_NONE_OFFSET 0
#define KM_UL_BPE_PERIPHERALID4_NONE_SIZE 8

/** bit field defines for km_ul_bpe_s#peripheralid5 */
#define KM_UL_BPE_PERIPHERALID5_NONE_OFFSET 0
#define KM_UL_BPE_PERIPHERALID5_NONE_SIZE 8

/** bit field defines for km_ul_bpe_s#peripheralid6 */
#define KM_UL_BPE_PERIPHERALID6_NONE_OFFSET 0
#define KM_UL_BPE_PERIPHERALID6_NONE_SIZE 8

/** bit field defines for km_ul_bpe_s#peripheralid7 */
#define KM_UL_BPE_PERIPHERALID7_NONE_OFFSET 0
#define KM_UL_BPE_PERIPHERALID7_NONE_SIZE 8

/** bit field defines for km_ul_bpe_s#peripheralid0 */
#define KM_UL_BPE_PERIPHERALID0_NONE_OFFSET 0
#define KM_UL_BPE_PERIPHERALID0_NONE_SIZE 8

/** bit field defines for km_ul_bpe_s#peripheralid1 */
#define KM_UL_BPE_PERIPHERALID1_NONE_OFFSET 0
#define KM_UL_BPE_PERIPHERALID1_NONE_SIZE 8

/** bit field defines for km_ul_bpe_s#peripheralid2 */
#define KM_UL_BPE_PERIPHERALID2_NONE_OFFSET 0
#define KM_UL_BPE_PERIPHERALID2_NONE_SIZE 8

/** bit field defines for km_ul_bpe_s#peripheralid3 */
#define KM_UL_BPE_PERIPHERALID3_NONE_OFFSET 0
#define KM_UL_BPE_PERIPHERALID3_NONE_SIZE 8

/** bit field defines for km_ul_bpe_s#componentid0 */
#define KM_UL_BPE_COMPONENTID0_NONE_OFFSET 0
#define KM_UL_BPE_COMPONENTID0_NONE_SIZE 8

/** bit field defines for km_ul_bpe_s#componentid1 */
#define KM_UL_BPE_COMPONENTID1_NONE_OFFSET 0
#define KM_UL_BPE_COMPONENTID1_NONE_SIZE 8

/** bit field defines for km_ul_bpe_s#componentid2 */
#define KM_UL_BPE_COMPONENTID2_NONE_OFFSET 0
#define KM_UL_BPE_COMPONENTID2_NONE_SIZE 8

/** bit field defines for km_ul_bpe_s#componentid3 */
#define KM_UL_BPE_COMPONENTID3_NONE_OFFSET 0
#define KM_UL_BPE_COMPONENTID3_NONE_SIZE 8

/* EOF km_ul_bpe.h */
#endif
