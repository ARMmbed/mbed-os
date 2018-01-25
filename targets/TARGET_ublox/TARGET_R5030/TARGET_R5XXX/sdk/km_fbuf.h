#ifndef INCLUDED_KM_FBUF
#define INCLUDED_KM_FBUF
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** km_fbuf HAL Spreadsheet version number */
#define KM_FBUF_HAL_VERSION 3

/** Register module for the Klein Matterhorn LTE Subsystem Front End Buffer (Fbuf)
*/
struct km_fbuf_s {
   /** Serving cell  control register at address offset 0x000, read-write */
   uint32_t wrctrl;
   /** FBUF Status at address offset 0x004, read-write */
   uint32_t status;
   /** Power Accumulator Status at address offset 0x008, read-only */
   uint32_t pwracc;
   /** DC Accumulator Status at address offset 0x00C, read-only */
   uint32_t dcacc;
   /** Write Pointer Match Interrupt Settings. Match when reg Value = (WrPtr AND Mask) at address offset 0x010, read-write */
   uint32_t match;
   /** Client Request address error checking control at address offset 0x014, read-write */
   uint32_t clierrctrl;
   /** Raw Event Status.  Set by HW. Write 1 to clear at address offset 0x018, read-write */
   uint32_t eventstatus;
   /** Interrupt Control Mask at address offset 0x01C, read-write */
   uint32_t intmask;
   /** Masked Interrupt Status at address offset 0x020, read-only */
   uint32_t intstatus;
   /** When ClientReqErr is set, ErrEvent captures the first error request details for debug purposes at address offset 0x024, read-write */
   uint32_t clierrinfo;
   /**  at address offset 0x028, read-write */
   uint32_t hwobs;
   /** Reserved space */
   uint8_t fill0[4004];
   /** Peripheral ID6 Register at address offset 0xFD0, read-only */
   uint32_t peripheralid6;
   /** Peripheral ID7 Register at address offset 0xFD4, read-only */
   uint32_t peripheralid7;
   /** Peripheral ID0 Register at address offset 0xFD8, read-only */
   uint32_t peripheralid0;
   /** Peripheral ID1 Register at address offset 0xFDC, read-only */
   uint32_t peripheralid1;
   /** Peripheral ID2 Register at address offset 0xFE0, read-only */
   uint32_t peripheralid2;
   /** Peripheral ID3 Register at address offset 0xFE4, read-only */
   uint32_t peripheralid3;
   /** Primecell ID register 0 at address offset 0xFE8, read-only */
   uint32_t componentid0;
   /** Primecell ID register 1 at address offset 0xFEC, read-only */
   uint32_t componentid1;
   /** Primecell ID register 2 at address offset 0xFF0, read-only */
   uint32_t componentid2;
   /** Primecell ID register 3 at address offset 0xFF4, read-only */
   uint32_t componentid3;
};

/** bit field defines for km_fbuf_s#wrctrl */
#define KM_FBUF_WRCTRL_WRPTR_OFFSET 0
#define KM_FBUF_WRCTRL_WRPTR_SIZE 16

/** bit field defines for km_fbuf_s#status */
#define KM_FBUF_STATUS_SEL_OFFSET 0
#define KM_FBUF_STATUS_SEL_SIZE 3
/** Read Status from Client 0 */
#define KM_FBUF_STATUS_SEL_CLIENT0_VALUE 0
/** Read Status from Client 1 */
#define KM_FBUF_STATUS_SEL_CLIENT1_VALUE 1
/** Read Status from Client 2 */
#define KM_FBUF_STATUS_SEL_CLIENT2_VALUE 2
/** Read Status from Client 3 */
#define KM_FBUF_STATUS_SEL_CLIENT3_VALUE 3
/** Read Status from Client 4 */
#define KM_FBUF_STATUS_SEL_CLIENT4_VALUE 4
/** Read Status from the Write Side - only addr field valid for this selection */
#define KM_FBUF_STATUS_SEL_WRSTATUS_VALUE 5
#define KM_FBUF_STATUS_ADDR_OFFSET 3
#define KM_FBUF_STATUS_ADDR_SIZE 16
#define KM_FBUF_STATUS_LEN_OFFSET 19
#define KM_FBUF_STATUS_LEN_SIZE 3
#define KM_FBUF_STATUS_VAL_OFFSET 22
#define KM_FBUF_STATUS_VAL_SIZE 1
#define KM_FBUF_STATUS_HIT_OFFSET 23
#define KM_FBUF_STATUS_HIT_SIZE 1
#define KM_FBUF_STATUS_FSM_OFFSET 24
#define KM_FBUF_STATUS_FSM_SIZE 3
/** FSM State */
#define KM_FBUF_STATUS_FSM_ARMED_VALUE 2
/** FSM State */
#define KM_FBUF_STATUS_FSM_IDLE_VALUE 0
/** FSM State */
#define KM_FBUF_STATUS_FSM_SERVE_VALUE 3
/** FSM State */
#define KM_FBUF_STATUS_FSM_WAIT_VALUE 1

/** bit field defines for km_fbuf_s#pwracc */
#define KM_FBUF_PWRACC_NONE_OFFSET 0
#define KM_FBUF_PWRACC_NONE_SIZE 16

/** bit field defines for km_fbuf_s#dcacc */
#define KM_FBUF_DCACC_RE_OFFSET 0
#define KM_FBUF_DCACC_RE_SIZE 12
#define KM_FBUF_DCACC_IM_OFFSET 12
#define KM_FBUF_DCACC_IM_SIZE 12

/** bit field defines for km_fbuf_s#match */
#define KM_FBUF_MATCH_MASK_OFFSET 0
#define KM_FBUF_MATCH_MASK_SIZE 16
#define KM_FBUF_MATCH_VALUE_OFFSET 16
#define KM_FBUF_MATCH_VALUE_SIZE 16

/** bit field defines for km_fbuf_s#clierrctrl */
#define KM_FBUF_CLIERRCTRL_CLIMARG0_OFFSET 0
#define KM_FBUF_CLIERRCTRL_CLIMARG0_SIZE 5
#define KM_FBUF_CLIERRCTRL_CLIMARG1_OFFSET 5
#define KM_FBUF_CLIERRCTRL_CLIMARG1_SIZE 5
#define KM_FBUF_CLIERRCTRL_CLIMARG2_OFFSET 10
#define KM_FBUF_CLIERRCTRL_CLIMARG2_SIZE 5
#define KM_FBUF_CLIERRCTRL_CLIMARG3_OFFSET 15
#define KM_FBUF_CLIERRCTRL_CLIMARG3_SIZE 5
#define KM_FBUF_CLIERRCTRL_CLIMARG4_OFFSET 20
#define KM_FBUF_CLIERRCTRL_CLIMARG4_SIZE 5

/** bit field defines for km_fbuf_s#eventstatus */
#define KM_FBUF_EVENTSTATUS_MATCH_OFFSET 0
#define KM_FBUF_EVENTSTATUS_MATCH_SIZE 1
#define KM_FBUF_EVENTSTATUS_CLIENTREQERR0_OFFSET 1
#define KM_FBUF_EVENTSTATUS_CLIENTREQERR0_SIZE 1
#define KM_FBUF_EVENTSTATUS_CLIENTREQERR1_OFFSET 2
#define KM_FBUF_EVENTSTATUS_CLIENTREQERR1_SIZE 1
#define KM_FBUF_EVENTSTATUS_CLIENTREQERR2_OFFSET 3
#define KM_FBUF_EVENTSTATUS_CLIENTREQERR2_SIZE 1
#define KM_FBUF_EVENTSTATUS_CLIENTREQERR3_OFFSET 4
#define KM_FBUF_EVENTSTATUS_CLIENTREQERR3_SIZE 1
#define KM_FBUF_EVENTSTATUS_CLIENTREQERR4_OFFSET 5
#define KM_FBUF_EVENTSTATUS_CLIENTREQERR4_SIZE 1

/** bit field defines for km_fbuf_s#intmask */
#define KM_FBUF_INTMASK_MATCH_OFFSET 0
#define KM_FBUF_INTMASK_MATCH_SIZE 1
#define KM_FBUF_INTMASK_CLIENTREQERR0_OFFSET 1
#define KM_FBUF_INTMASK_CLIENTREQERR0_SIZE 1
#define KM_FBUF_INTMASK_CLIENTREQERR1_OFFSET 2
#define KM_FBUF_INTMASK_CLIENTREQERR1_SIZE 1
#define KM_FBUF_INTMASK_CLIENTREQERR2_OFFSET 3
#define KM_FBUF_INTMASK_CLIENTREQERR2_SIZE 1
#define KM_FBUF_INTMASK_CLIENTREQERR3_OFFSET 4
#define KM_FBUF_INTMASK_CLIENTREQERR3_SIZE 1
#define KM_FBUF_INTMASK_CLIENTREQERR4_OFFSET 5
#define KM_FBUF_INTMASK_CLIENTREQERR4_SIZE 1

/** bit field defines for km_fbuf_s#intstatus */
#define KM_FBUF_INTSTATUS_MATCH_OFFSET 0
#define KM_FBUF_INTSTATUS_MATCH_SIZE 1
#define KM_FBUF_INTSTATUS_CLIENTREQERR0_OFFSET 1
#define KM_FBUF_INTSTATUS_CLIENTREQERR0_SIZE 1
#define KM_FBUF_INTSTATUS_CLIENTREQERR1_OFFSET 2
#define KM_FBUF_INTSTATUS_CLIENTREQERR1_SIZE 1
#define KM_FBUF_INTSTATUS_CLIENTREQERR2_OFFSET 3
#define KM_FBUF_INTSTATUS_CLIENTREQERR2_SIZE 1
#define KM_FBUF_INTSTATUS_CLIENTREQERR3_OFFSET 4
#define KM_FBUF_INTSTATUS_CLIENTREQERR3_SIZE 1
#define KM_FBUF_INTSTATUS_CLIENTREQERR4_OFFSET 5
#define KM_FBUF_INTSTATUS_CLIENTREQERR4_SIZE 1

/** bit field defines for km_fbuf_s#clierrinfo */
#define KM_FBUF_CLIERRINFO_CLEAR_OFFSET 0
#define KM_FBUF_CLIERRINFO_CLEAR_SIZE 1
#define KM_FBUF_CLIERRINFO_CLIENT_OFFSET 1
#define KM_FBUF_CLIERRINFO_CLIENT_SIZE 3
#define KM_FBUF_CLIERRINFO_CURRWRPAGE_OFFSET 4
#define KM_FBUF_CLIERRINFO_CURRWRPAGE_SIZE 5
#define KM_FBUF_CLIERRINFO_ADDR_OFFSET 16
#define KM_FBUF_CLIERRINFO_ADDR_SIZE 16

/** bit field defines for km_fbuf_s#hwobs */
#define KM_FBUF_HWOBS_SEL_OFFSET 0
#define KM_FBUF_HWOBS_SEL_SIZE 3

/** bit field defines for km_fbuf_s#peripheralid6 */
#define KM_FBUF_PERIPHERALID6_NONE_OFFSET 0
#define KM_FBUF_PERIPHERALID6_NONE_SIZE 8

/** bit field defines for km_fbuf_s#peripheralid7 */
#define KM_FBUF_PERIPHERALID7_NONE_OFFSET 0
#define KM_FBUF_PERIPHERALID7_NONE_SIZE 8

/** bit field defines for km_fbuf_s#peripheralid0 */
#define KM_FBUF_PERIPHERALID0_NONE_OFFSET 0
#define KM_FBUF_PERIPHERALID0_NONE_SIZE 8

/** bit field defines for km_fbuf_s#peripheralid1 */
#define KM_FBUF_PERIPHERALID1_NONE_OFFSET 0
#define KM_FBUF_PERIPHERALID1_NONE_SIZE 8

/** bit field defines for km_fbuf_s#peripheralid2 */
#define KM_FBUF_PERIPHERALID2_NONE_OFFSET 0
#define KM_FBUF_PERIPHERALID2_NONE_SIZE 8

/** bit field defines for km_fbuf_s#peripheralid3 */
#define KM_FBUF_PERIPHERALID3_NONE_OFFSET 0
#define KM_FBUF_PERIPHERALID3_NONE_SIZE 8

/** bit field defines for km_fbuf_s#componentid0 */
#define KM_FBUF_COMPONENTID0_NONE_OFFSET 0
#define KM_FBUF_COMPONENTID0_NONE_SIZE 8

/** bit field defines for km_fbuf_s#componentid1 */
#define KM_FBUF_COMPONENTID1_NONE_OFFSET 0
#define KM_FBUF_COMPONENTID1_NONE_SIZE 8

/** bit field defines for km_fbuf_s#componentid2 */
#define KM_FBUF_COMPONENTID2_NONE_OFFSET 0
#define KM_FBUF_COMPONENTID2_NONE_SIZE 8

/** bit field defines for km_fbuf_s#componentid3 */
#define KM_FBUF_COMPONENTID3_NONE_OFFSET 0
#define KM_FBUF_COMPONENTID3_NONE_SIZE 8

/* EOF km_fbuf.h */
#endif
