#ifndef INCLUDED_AR1DE_DBG
#define INCLUDED_AR1DE_DBG
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** Number of Breakpoint Registers */
#define AR1DE_NR_BP 8
#define AR1DE_DBG_DEVCONF 0x00
#define AR1DE_DBG_DEVTYPE 0x35
#define AR1DE_DBG_PERIPHERALID4 0x07
#define AR1DE_DBG_PERIPHERALID5 0x00
#define AR1DE_DBG_PERIPHERALID6 0x00
#define AR1DE_DBG_PERIPHERALID7 0x00
#define AR1DE_DBG_PERIPHERALID0 0x80
#define AR1DE_DBG_PERIPHERALID1 0xDA
#define AR1DE_DBG_PERIPHERALID2 0x08
#define AR1DE_DBG_PERIPHERALID3 0x00
#define AR1DE_DBG_COMPONENTID0 0x0D
#define AR1DE_DBG_COMPONENTID1 0x90
#define AR1DE_DBG_COMPONENTID2 0x05
#define AR1DE_DBG_COMPONENTID3 0xB1

/** Debug Interface
*/
struct ar1de_dbg_s {
   /** Data Engine Unique Identifier at address offset 0x000, read-only */
   uint32_t deuid;
   /** Data Engine Instance ID at address offset 0x004, read-only */
   uint32_t instid;
   /** Debug Configuration at address offset 0x008, read-only */
   uint32_t dbgconf;
   /** Reserved space */
   uint8_t fill0[116];
   /** Debug Control at address offset 0x080, read-write */
   uint32_t dbgctrl;
   /** OptimoDE Control Status (status and stall sources) at address offset 0x084, read-only */
   uint32_t odectrlstat;
   /** OptimoDE Program Counter at address offset 0x088, read-only */
   uint32_t odepc;
   /** Debug Break State at address offset 0x08C, read-write */
   uint32_t dbgbkstate;
   /** Breakpoint Status  at address offset 0x090, read-only */
   uint32_t bkpstat;
   /** Breakpoint Enable Set at address offset 0x094, read-write */
   uint32_t bkptenset;
   /** Breakpoint Enable Clear at address offset 0x098, read-write */
   uint32_t bkptenclr;
   /** Reserved space */
   uint8_t fill1[4];
   /** Break On Breakpoint Enable Set at address offset 0x0A0, read-write */
   uint32_t break_on_breakpoint_disable_set;
   /** Break On Breakpoint Enable Clear at address offset 0x0A4, read-write */
   uint32_t break_on_breakpoint_disable_clr;
   /** Reserved space */
   uint8_t fill2[88];
   /** Breakpoint Program Counter at address offset 0x100, read-write */
   uint32_t brkptpc[AR1DE_NR_BP];
   /** Reserved space */
   uint8_t fill3[100];
   /** Exception Raw at address offset 0x184, read-only */
   uint32_t excptraw;
   /** Exception Status at address offset 0x188, read-only */
   uint32_t excptstat;
   /** Exception Clear at address offset 0x18C, write-only */
   uint32_t excptclr;
   /** Exception Enable Set at address offset 0x190, write-only */
   uint32_t excptens;
   /** Exception Enable Clear at address offset 0x194, write-only */
   uint32_t excptenc;
   /** Reserved space */
   uint8_t fill4[104];
   /** Observation Address 0 at address offset 0x200, read-write */
   uint32_t obsaddr0;
   /** Observation Data 0 at address offset 0x204, read-write */
   uint32_t obsdata0;
   /** Observation Command 0 at address offset 0x208, write-only */
   uint32_t obscmd0;
   /** Observation Status 0 at address offset 0x20C, read-only */
   uint32_t obsstat0;
   /** Observation Configuration 0.  The OBSCONF0 (observation configuration) register configures the access protocol for the other observation registers. Every configuration bit enables a feature. When the feature is not supported a subsequent read will return a zero value on this bit. The supported features can thus be discovered by writing a one to all fields and read back the result. at address offset 0x210, read-write */
   uint32_t obsconf0;
   /** Reserved space */
   uint8_t fill5[3308];
   /** Integration Mode Control at address offset 0xF00, read-only */
   uint32_t intmctrl;
   /** Reserved space */
   uint8_t fill6[156];
   /** Claim Tag Set Register at address offset 0xFA0, read-only */
   uint32_t claimset;
   /** Claim Tag Clear Register at address offset 0xFA4, read-only */
   uint32_t claimclr;
   /** Reserved space */
   uint8_t fill7[8];
   /** Lock Access Register at address offset 0xFB0, write-only */
   uint32_t lockaccess;
   /** Lock Status register at address offset 0xFB4, read-only */
   uint32_t lockstatus;
   /** Authentication Status Register at address offset 0xFB8, read-only */
   uint32_t authstatus;
   /** Reserved space */
   uint8_t fill8[12];
   /** Device configuration register at address offset 0xFC8, read-only constant 0x00000000 */
   uint32_t devconf;
   /** Device type register at address offset 0xFCC, read-only constant 0x00000035 */
   uint32_t devtype;
   /** Peripheral ID Register [39:32] at address offset 0xFD0, read-only constant 0x00000007 */
   uint32_t peripheralid4;
   /** Peripheral ID Register [47:40] at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** Peripheral ID Register [55:48] at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** Peripheral ID Register [63:56] at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** Peripheral ID Register [7:0] at address offset 0xFE0, read-only constant 0x00000080 */
   uint32_t peripheralid0;
   /** Peripheral ID Register [15:8] at address offset 0xFE4, read-only constant 0x000000DA */
   uint32_t peripheralid1;
   /** Peripheral ID Register [23:16] at address offset 0xFE8, read-only constant 0x00000008 */
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

/** bit field defines for ar1de_dbg_s#deuid */
#define AR1DE_DBG_DEUID_DEUID_OFFSET 0
#define AR1DE_DBG_DEUID_DEUID_SIZE 32

/** bit field defines for ar1de_dbg_s#instid */
#define AR1DE_DBG_INSTID_INSTID_OFFSET 0
#define AR1DE_DBG_INSTID_INSTID_SIZE 32

/** bit field defines for ar1de_dbg_s#dbgconf */
#define AR1DE_DBG_DBGCONF_DEBUG_ARCHITECTURE_VERSION_OFFSET 0
#define AR1DE_DBG_DBGCONF_DEBUG_ARCHITECTURE_VERSION_SIZE 4
#define AR1DE_DBG_DBGCONF_NUMBER_BREAKPOINTS_OFFSET 4
#define AR1DE_DBG_DBGCONF_NUMBER_BREAKPOINTS_SIZE 4
#define AR1DE_DBG_DBGCONF_NUMBER_EXCEPTIONS_OFFSET 8
#define AR1DE_DBG_DBGCONF_NUMBER_EXCEPTIONS_SIZE 5
#define AR1DE_DBG_DBGCONF_BYTEPC_WIDTH_OFFSET 13
#define AR1DE_DBG_DBGCONF_BYTEPC_WIDTH_SIZE 6
#define AR1DE_DBG_DBGCONF_NUMBER_OBSERVATION_PORTS_OFFSET 19
#define AR1DE_DBG_DBGCONF_NUMBER_OBSERVATION_PORTS_SIZE 4
#define AR1DE_DBG_DBGCONF_NUMBER_STALL_SOURCES_OFFSET 23
#define AR1DE_DBG_DBGCONF_NUMBER_STALL_SOURCES_SIZE 5

/** bit field defines for ar1de_dbg_s#dbgctrl */
#define AR1DE_DBG_DBGCTRL_ENABLE_DEBUG_OFFSET 0
#define AR1DE_DBG_DBGCTRL_ENABLE_DEBUG_SIZE 1

/** bit field defines for ar1de_dbg_s#odectrlstat */
#define AR1DE_DBG_ODECTRLSTAT_BUSY_OFFSET 0
#define AR1DE_DBG_ODECTRLSTAT_BUSY_SIZE 1
#define AR1DE_DBG_ODECTRLSTAT_PC_VALID_OFFSET 1
#define AR1DE_DBG_ODECTRLSTAT_PC_VALID_SIZE 1
#define AR1DE_DBG_ODECTRLSTAT_STALL_OFFSET 2
#define AR1DE_DBG_ODECTRLSTAT_STALL_SIZE 1
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_SIMD_CMAC_OFFSET 3
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_SIMD_CMAC_SIZE 1
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_SIMD_ALU_OFFSET 4
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_SIMD_ALU_SIZE 1
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_AGU_OFFSET 5
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_AGU_SIZE 1
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_SCALAR_ALU_OFFSET 6
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_SCALAR_ALU_SIZE 1
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_PPC_OFFSET 7
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_PPC_SIZE 1
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_AXIM_OFFSET 8
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_AXIM_SIZE 1
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_DMEM_RAM_OFFSET 9
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_DMEM_RAM_SIZE 1
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_RD_DMEM_RAM_OFFSET 10
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_RD_DMEM_RAM_SIZE 1
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_DMB_DMEM_RAM_OFFSET 11
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_DMB_DMEM_RAM_SIZE 1
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_RD_DTCM_RAM_OFFSET 12
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_RD_DTCM_RAM_SIZE 1
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_CTRL_OFFSET 13
#define AR1DE_DBG_ODECTRLSTAT_STALL_REQ_CTRL_SIZE 1
#define AR1DE_DBG_ODECTRLSTAT_DBG_HALT_OFFSET 14
#define AR1DE_DBG_ODECTRLSTAT_DBG_HALT_SIZE 1

/** bit field defines for ar1de_dbg_s#odepc */
#define AR1DE_DBG_ODEPC_PAGE_SELECT_OFFSET 0
#define AR1DE_DBG_ODEPC_PAGE_SELECT_SIZE 2
#define AR1DE_DBG_ODEPC_MEM_SELECT_OFFSET 2
#define AR1DE_DBG_ODEPC_MEM_SELECT_SIZE 1
#define AR1DE_DBG_ODEPC_TASK_OFFSET_OFFSET 3
#define AR1DE_DBG_ODEPC_TASK_OFFSET_SIZE 15
#define AR1DE_DBG_ODEPC_TASK_ID_OFFSET 18
#define AR1DE_DBG_ODEPC_TASK_ID_SIZE 14

/** bit field defines for ar1de_dbg_s#dbgbkstate */
#define AR1DE_DBG_DBGBKSTATE_STOP_OFFSET 0
#define AR1DE_DBG_DBGBKSTATE_STOP_SIZE 1
#define AR1DE_DBG_DBGBKSTATE_INSTRUCTION_STEP_OFFSET 1
#define AR1DE_DBG_DBGBKSTATE_INSTRUCTION_STEP_SIZE 1

/** bit field defines for ar1de_dbg_s#bkpstat */
#define AR1DE_DBG_BKPSTAT_BP_HIT_OFFSET 0
#define AR1DE_DBG_BKPSTAT_BP_HIT_SIZE 8

/** bit field defines for ar1de_dbg_s#bkptenset */
#define AR1DE_DBG_BKPTENSET_BP_EN_SET_OFFSET 0
#define AR1DE_DBG_BKPTENSET_BP_EN_SET_SIZE 8
#define AR1DE_DBG_BKPTENSET_BP_ID_EN_SET_OFFSET 16
#define AR1DE_DBG_BKPTENSET_BP_ID_EN_SET_SIZE 8

/** bit field defines for ar1de_dbg_s#bkptenclr */
#define AR1DE_DBG_BKPTENCLR_BP_EN_CLR_OFFSET 0
#define AR1DE_DBG_BKPTENCLR_BP_EN_CLR_SIZE 8
#define AR1DE_DBG_BKPTENCLR_BP_ID_EN_CLR_OFFSET 16
#define AR1DE_DBG_BKPTENCLR_BP_ID_EN_CLR_SIZE 8

/** bit field defines for ar1de_dbg_s#break_on_breakpoint_disable_set */
#define AR1DE_DBG_BREAK_ON_BREAKPOINT_DISABLE_SET_BREAK_ON_BP_EN_SET_OFFSET 0
#define AR1DE_DBG_BREAK_ON_BREAKPOINT_DISABLE_SET_BREAK_ON_BP_EN_SET_SIZE 8

/** bit field defines for ar1de_dbg_s#break_on_breakpoint_disable_clr */
#define AR1DE_DBG_BREAK_ON_BREAKPOINT_DISABLE_CLR_BREAK_ON_BP_EN_CLR_OFFSET 0
#define AR1DE_DBG_BREAK_ON_BREAKPOINT_DISABLE_CLR_BREAK_ON_BP_EN_CLR_SIZE 8

/** bit field defines for ar1de_dbg_s#brkptpc */
#define AR1DE_DBG_BRKPTPC_PAGE_SELECT_OFFSET 0
#define AR1DE_DBG_BRKPTPC_PAGE_SELECT_SIZE 2
#define AR1DE_DBG_BRKPTPC_MEM_SELECT_OFFSET 2
#define AR1DE_DBG_BRKPTPC_MEM_SELECT_SIZE 1
#define AR1DE_DBG_BRKPTPC_TASK_OFFSET_OFFSET 3
#define AR1DE_DBG_BRKPTPC_TASK_OFFSET_SIZE 15
#define AR1DE_DBG_BRKPTPC_TASK_ID_OFFSET 18
#define AR1DE_DBG_BRKPTPC_TASK_ID_SIZE 14

/** bit field defines for ar1de_dbg_s#excptraw */
#define AR1DE_DBG_EXCPTRAW_DTCM_UNALIGNED_READ_ACCESS_OFFSET 0
#define AR1DE_DBG_EXCPTRAW_DTCM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTRAW_DTCM_UNALIGNED_WRITE_ACCESS_OFFSET 1
#define AR1DE_DBG_EXCPTRAW_DTCM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTRAW_DMEM_UNALIGNED_READ_ACCESS_OFFSET 2
#define AR1DE_DBG_EXCPTRAW_DMEM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTRAW_DMEM_UNALIGNED_WRITE_ACCESS_OFFSET 3
#define AR1DE_DBG_EXCPTRAW_DMEM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTRAW_DMEM_READ_FAULT_OFFSET 4
#define AR1DE_DBG_EXCPTRAW_DMEM_READ_FAULT_SIZE 1
#define AR1DE_DBG_EXCPTRAW_DMEM_WRITE_FAULT_OFFSET 5
#define AR1DE_DBG_EXCPTRAW_DMEM_WRITE_FAULT_SIZE 1
#define AR1DE_DBG_EXCPTRAW_SOFTWARE_EXCEPTION_OFFSET 6
#define AR1DE_DBG_EXCPTRAW_SOFTWARE_EXCEPTION_SIZE 1
#define AR1DE_DBG_EXCPTRAW_AXI_READ_EXCEPTION_OFFSET 7
#define AR1DE_DBG_EXCPTRAW_AXI_READ_EXCEPTION_SIZE 1
#define AR1DE_DBG_EXCPTRAW_AXI_WRITE_EXCEPTION_OFFSET 8
#define AR1DE_DBG_EXCPTRAW_AXI_WRITE_EXCEPTION_SIZE 1
#define AR1DE_DBG_EXCPTRAW_DMEM_INSTR_FETCH_READ_FAULT_OFFSET 9
#define AR1DE_DBG_EXCPTRAW_DMEM_INSTR_FETCH_READ_FAULT_SIZE 1

/** bit field defines for ar1de_dbg_s#excptstat */
#define AR1DE_DBG_EXCPTSTAT_DTCM_UNALIGNED_READ_ACCESS_OFFSET 0
#define AR1DE_DBG_EXCPTSTAT_DTCM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTSTAT_DTCM_UNALIGNED_WRITE_ACCESS_OFFSET 1
#define AR1DE_DBG_EXCPTSTAT_DTCM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTSTAT_DMEM_UNALIGNED_READ_ACCESS_OFFSET 2
#define AR1DE_DBG_EXCPTSTAT_DMEM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTSTAT_DMEM_UNALIGNED_WRITE_ACCESS_OFFSET 3
#define AR1DE_DBG_EXCPTSTAT_DMEM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTSTAT_DMEM_READ_FAULT_OFFSET 4
#define AR1DE_DBG_EXCPTSTAT_DMEM_READ_FAULT_SIZE 1
#define AR1DE_DBG_EXCPTSTAT_DMEM_WRITE_FAULT_OFFSET 5
#define AR1DE_DBG_EXCPTSTAT_DMEM_WRITE_FAULT_SIZE 1
#define AR1DE_DBG_EXCPTSTAT_SOFTWARE_EXCEPTION_OFFSET 6
#define AR1DE_DBG_EXCPTSTAT_SOFTWARE_EXCEPTION_SIZE 1
#define AR1DE_DBG_EXCPTSTAT_AXI_READ_EXCEPTION_OFFSET 7
#define AR1DE_DBG_EXCPTSTAT_AXI_READ_EXCEPTION_SIZE 1
#define AR1DE_DBG_EXCPTSTAT_AXI_WRITE_EXCEPTION_OFFSET 8
#define AR1DE_DBG_EXCPTSTAT_AXI_WRITE_EXCEPTION_SIZE 1
#define AR1DE_DBG_EXCPTSTAT_DMEM_INSTR_FETCH_READ_FAULT_OFFSET 9
#define AR1DE_DBG_EXCPTSTAT_DMEM_INSTR_FETCH_READ_FAULT_SIZE 1

/** bit field defines for ar1de_dbg_s#excptclr */
#define AR1DE_DBG_EXCPTCLR_DTCM_UNALIGNED_READ_ACCESS_OFFSET 0
#define AR1DE_DBG_EXCPTCLR_DTCM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTCLR_DTCM_UNALIGNED_WRITE_ACCESS_OFFSET 1
#define AR1DE_DBG_EXCPTCLR_DTCM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTCLR_DMEM_UNALIGNED_READ_ACCESS_OFFSET 2
#define AR1DE_DBG_EXCPTCLR_DMEM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTCLR_DMEM_UNALIGNED_WRITE_ACCESS_OFFSET 3
#define AR1DE_DBG_EXCPTCLR_DMEM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTCLR_DMEM_READ_FAULT_OFFSET 4
#define AR1DE_DBG_EXCPTCLR_DMEM_READ_FAULT_SIZE 1
#define AR1DE_DBG_EXCPTCLR_DMEM_WRITE_FAULT_OFFSET 5
#define AR1DE_DBG_EXCPTCLR_DMEM_WRITE_FAULT_SIZE 1
#define AR1DE_DBG_EXCPTCLR_SOFTWARE_EXCEPTION_OFFSET 6
#define AR1DE_DBG_EXCPTCLR_SOFTWARE_EXCEPTION_SIZE 1
#define AR1DE_DBG_EXCPTCLR_AXI_READ_EXCEPTION_OFFSET 7
#define AR1DE_DBG_EXCPTCLR_AXI_READ_EXCEPTION_SIZE 1
#define AR1DE_DBG_EXCPTCLR_AXI_WRITE_EXCEPTION_OFFSET 8
#define AR1DE_DBG_EXCPTCLR_AXI_WRITE_EXCEPTION_SIZE 1
#define AR1DE_DBG_EXCPTCLR_DMEM_INSTR_FETCH_READ_FAULT_OFFSET 9
#define AR1DE_DBG_EXCPTCLR_DMEM_INSTR_FETCH_READ_FAULT_SIZE 1

/** bit field defines for ar1de_dbg_s#excptens */
#define AR1DE_DBG_EXCPTENS_DTCM_UNALIGNED_READ_ACCESS_OFFSET 0
#define AR1DE_DBG_EXCPTENS_DTCM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTENS_DTCM_UNALIGNED_WRITE_ACCESS_OFFSET 1
#define AR1DE_DBG_EXCPTENS_DTCM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTENS_DMEM_UNALIGNED_READ_ACCESS_OFFSET 2
#define AR1DE_DBG_EXCPTENS_DMEM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTENS_DMEM_UNALIGNED_WRITE_ACCESS_OFFSET 3
#define AR1DE_DBG_EXCPTENS_DMEM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTENS_DMEM_READ_FAULT_OFFSET 4
#define AR1DE_DBG_EXCPTENS_DMEM_READ_FAULT_SIZE 1
#define AR1DE_DBG_EXCPTENS_DMEM_WRITE_FAULT_OFFSET 5
#define AR1DE_DBG_EXCPTENS_DMEM_WRITE_FAULT_SIZE 1
#define AR1DE_DBG_EXCPTENS_SOFTWARE_EXCEPTION_OFFSET 6
#define AR1DE_DBG_EXCPTENS_SOFTWARE_EXCEPTION_SIZE 1
#define AR1DE_DBG_EXCPTENS_AXI_READ_EXCEPTION_OFFSET 7
#define AR1DE_DBG_EXCPTENS_AXI_READ_EXCEPTION_SIZE 1
#define AR1DE_DBG_EXCPTENS_AXI_WRITE_EXCEPTION_OFFSET 8
#define AR1DE_DBG_EXCPTENS_AXI_WRITE_EXCEPTION_SIZE 1
#define AR1DE_DBG_EXCPTENS_DMEM_INSTR_FETCH_READ_FAULT_OFFSET 9
#define AR1DE_DBG_EXCPTENS_DMEM_INSTR_FETCH_READ_FAULT_SIZE 1

/** bit field defines for ar1de_dbg_s#excptenc */
#define AR1DE_DBG_EXCPTENC_DTCM_UNALIGNED_READ_ACCESS_OFFSET 0
#define AR1DE_DBG_EXCPTENC_DTCM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTENC_DTCM_UNALIGNED_WRITE_ACCESS_OFFSET 1
#define AR1DE_DBG_EXCPTENC_DTCM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTENC_DMEM_UNALIGNED_READ_ACCESS_OFFSET 2
#define AR1DE_DBG_EXCPTENC_DMEM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTENC_DMEM_UNALIGNED_WRITE_ACCESS_OFFSET 3
#define AR1DE_DBG_EXCPTENC_DMEM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_DBG_EXCPTENC_DMEM_READ_FAULT_OFFSET 4
#define AR1DE_DBG_EXCPTENC_DMEM_READ_FAULT_SIZE 1
#define AR1DE_DBG_EXCPTENC_DMEM_WRITE_FAULT_OFFSET 5
#define AR1DE_DBG_EXCPTENC_DMEM_WRITE_FAULT_SIZE 1
#define AR1DE_DBG_EXCPTENC_SOFTWARE_EXCEPTION_OFFSET 6
#define AR1DE_DBG_EXCPTENC_SOFTWARE_EXCEPTION_SIZE 1
#define AR1DE_DBG_EXCPTENC_AXI_READ_EXCEPTION_OFFSET 7
#define AR1DE_DBG_EXCPTENC_AXI_READ_EXCEPTION_SIZE 1
#define AR1DE_DBG_EXCPTENC_AXI_WRITE_EXCEPTION_OFFSET 8
#define AR1DE_DBG_EXCPTENC_AXI_WRITE_EXCEPTION_SIZE 1
#define AR1DE_DBG_EXCPTENC_DMEM_INSTR_FETCH_READ_FAULT_OFFSET 9
#define AR1DE_DBG_EXCPTENC_DMEM_INSTR_FETCH_READ_FAULT_SIZE 1

/** bit field defines for ar1de_dbg_s#obsaddr0 */
#define AR1DE_DBG_OBSADDR0_OBSADDR0_OFFSET 0
#define AR1DE_DBG_OBSADDR0_OBSADDR0_SIZE 7

/** bit field defines for ar1de_dbg_s#obsdata0 */
#define AR1DE_DBG_OBSDATA0_OBSDATA0_OFFSET 0
#define AR1DE_DBG_OBSDATA0_OBSDATA0_SIZE 32

/** bit field defines for ar1de_dbg_s#obscmd0 */
#define AR1DE_DBG_OBSCMD0_READ_OFFSET 0
#define AR1DE_DBG_OBSCMD0_READ_SIZE 1
#define AR1DE_DBG_OBSCMD0_WRITE_OFFSET 1
#define AR1DE_DBG_OBSCMD0_WRITE_SIZE 1
#define AR1DE_DBG_OBSCMD0_ERROR_CLEAR_OFFSET 2
#define AR1DE_DBG_OBSCMD0_ERROR_CLEAR_SIZE 1

/** bit field defines for ar1de_dbg_s#obsstat0 */
#define AR1DE_DBG_OBSSTAT0_VALID_OFFSET 0
#define AR1DE_DBG_OBSSTAT0_VALID_SIZE 1
#define AR1DE_DBG_OBSSTAT0_ERROR_OFFSET 1
#define AR1DE_DBG_OBSSTAT0_ERROR_SIZE 1
#define AR1DE_DBG_OBSSTAT0_BUSY_OFFSET 2
#define AR1DE_DBG_OBSSTAT0_BUSY_SIZE 1

/** bit field defines for ar1de_dbg_s#obsconf0 */
#define AR1DE_DBG_OBSCONF0_ADDR_START_READ_OFFSET 0
#define AR1DE_DBG_OBSCONF0_ADDR_START_READ_SIZE 1
#define AR1DE_DBG_OBSCONF0_DATA_START_READ_OFFSET 1
#define AR1DE_DBG_OBSCONF0_DATA_START_READ_SIZE 1
#define AR1DE_DBG_OBSCONF0_DATA_START_WRITE_OFFSET 2
#define AR1DE_DBG_OBSCONF0_DATA_START_WRITE_SIZE 1
#define AR1DE_DBG_OBSCONF0_INCR_ADDR_OFFSET 3
#define AR1DE_DBG_OBSCONF0_INCR_ADDR_SIZE 1

/** bit field defines for ar1de_dbg_s#intmctrl */
#define AR1DE_DBG_INTMCTRL_INTMCTRL_OFFSET 0
#define AR1DE_DBG_INTMCTRL_INTMCTRL_SIZE 32

/** bit field defines for ar1de_dbg_s#claimset */
#define AR1DE_DBG_CLAIMSET_CLAIMSET_OFFSET 0
#define AR1DE_DBG_CLAIMSET_CLAIMSET_SIZE 8

/** bit field defines for ar1de_dbg_s#claimclr */
#define AR1DE_DBG_CLAIMCLR_CLAIMCLR_OFFSET 0
#define AR1DE_DBG_CLAIMCLR_CLAIMCLR_SIZE 8

/** bit field defines for ar1de_dbg_s#lockaccess */
#define AR1DE_DBG_LOCKACCESS_LOCKACCESS_OFFSET 0
#define AR1DE_DBG_LOCKACCESS_LOCKACCESS_SIZE 32

/** bit field defines for ar1de_dbg_s#lockstatus */
#define AR1DE_DBG_LOCKSTATUS_LOCK_CTRL_EXISTS_OFFSET 0
#define AR1DE_DBG_LOCKSTATUS_LOCK_CTRL_EXISTS_SIZE 1
#define AR1DE_DBG_LOCKSTATUS_ACCESS_PERMISSION_OFFSET 1
#define AR1DE_DBG_LOCKSTATUS_ACCESS_PERMISSION_SIZE 1
#define AR1DE_DBG_LOCKSTATUS_EIGHTBIT_LOCK_ACCESS_REG_IMPL_OFFSET 2
#define AR1DE_DBG_LOCKSTATUS_EIGHTBIT_LOCK_ACCESS_REG_IMPL_SIZE 1

/** bit field defines for ar1de_dbg_s#authstatus */
#define AR1DE_DBG_AUTHSTATUS_NON_SECURE_INVASIVE_DEBUG_OFFSET 0
#define AR1DE_DBG_AUTHSTATUS_NON_SECURE_INVASIVE_DEBUG_SIZE 2
#define AR1DE_DBG_AUTHSTATUS_NON_SECURE_NON_INVASIVE_DEBUG_OFFSET 2
#define AR1DE_DBG_AUTHSTATUS_NON_SECURE_NON_INVASIVE_DEBUG_SIZE 2
#define AR1DE_DBG_AUTHSTATUS_SECURE_INVASIVE_DEBUG_OFFSET 4
#define AR1DE_DBG_AUTHSTATUS_SECURE_INVASIVE_DEBUG_SIZE 2
#define AR1DE_DBG_AUTHSTATUS_SECURE_NON_INVASIVE_DEBUG_OFFSET 6
#define AR1DE_DBG_AUTHSTATUS_SECURE_NON_INVASIVE_DEBUG_SIZE 2

/** bit field defines for ar1de_dbg_s#devconf */
#define AR1DE_DBG_DEVCONF_DEVCONF_OFFSET 0
#define AR1DE_DBG_DEVCONF_DEVCONF_SIZE 8

/** bit field defines for ar1de_dbg_s#devtype */
#define AR1DE_DBG_DEVTYPE_DEVTYPE_OFFSET 0
#define AR1DE_DBG_DEVTYPE_DEVTYPE_SIZE 8

/** bit field defines for ar1de_dbg_s#peripheralid4 */
#define AR1DE_DBG_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define AR1DE_DBG_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for ar1de_dbg_s#peripheralid5 */
#define AR1DE_DBG_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define AR1DE_DBG_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for ar1de_dbg_s#peripheralid6 */
#define AR1DE_DBG_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define AR1DE_DBG_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for ar1de_dbg_s#peripheralid7 */
#define AR1DE_DBG_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define AR1DE_DBG_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for ar1de_dbg_s#peripheralid0 */
#define AR1DE_DBG_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define AR1DE_DBG_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for ar1de_dbg_s#peripheralid1 */
#define AR1DE_DBG_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define AR1DE_DBG_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for ar1de_dbg_s#peripheralid2 */
#define AR1DE_DBG_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define AR1DE_DBG_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for ar1de_dbg_s#peripheralid3 */
#define AR1DE_DBG_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define AR1DE_DBG_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for ar1de_dbg_s#componentid0 */
#define AR1DE_DBG_COMPONENTID0_COMPONENTID0_OFFSET 0
#define AR1DE_DBG_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for ar1de_dbg_s#componentid1 */
#define AR1DE_DBG_COMPONENTID1_COMPONENTID1_OFFSET 0
#define AR1DE_DBG_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for ar1de_dbg_s#componentid2 */
#define AR1DE_DBG_COMPONENTID2_COMPONENTID2_OFFSET 0
#define AR1DE_DBG_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for ar1de_dbg_s#componentid3 */
#define AR1DE_DBG_COMPONENTID3_COMPONENTID3_OFFSET 0
#define AR1DE_DBG_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF ar1de_dbg.h */
#endif
