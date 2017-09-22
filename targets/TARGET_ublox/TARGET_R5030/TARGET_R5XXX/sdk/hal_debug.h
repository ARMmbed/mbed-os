#ifndef INCLUDED_HAL_DEBUG
#define INCLUDED_HAL_DEBUG
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>


/** Cortex-M3 CTI Trigger Input Configuration */
enum CORTEX_M3_CTI_TRIGIN {
   /** Processor Halted */
   CORTEX_M3_CTI_TRIGIN_DBGHALTED = 0,
   /** DWT TRIGGER 0 */
   CORTEX_M3_CTI_TRIGIN_DWT_TRIGGER_0 = 4,
   /** DWT TRIGGER 1 */
   CORTEX_M3_CTI_TRIGIN_DWT_TRIGGER_1 = 5,
   /** DWT TRIGGER 2 */
   CORTEX_M3_CTI_TRIGIN_DWT_TRIGGER_2 = 6,
   /** ETM TRIGGER output */
   CORTEX_M3_CTI_TRIGIN_ETM_TRIGOUT = 7
};

/** Cortex-M3 CTI Trigger Output Configuration */
enum CORTEX_M3_CTI_TRIGOUT {
   /** Processor Debug Request (Acknowledged by the debugger writing to the CTIINTACK register) */
   CORTEX_M3_CTI_TRIGOUT_DBGREQ = 0,
   /** Interrupt request 0 (Acknowledged by writing to the CTIINTACK register in ISR) */
   CORTEX_M3_CTI_TRIGOUT_INTERRUPT_REQUEST_0 = 2,
   /** Interrupt request 1 (Acknowledged by writing to the CTIINTACK register in ISR) */
   CORTEX_M3_CTI_TRIGOUT_INTERRUPT_REQUEST_1 = 3,
   /** External Input 4 */
   CORTEX_M3_CTI_TRIGOUT_EXITIN_4 = 4,
   /** External Input 5 */
   CORTEX_M3_CTI_TRIGOUT_EXITIN_5 = 5,
   /** Processor Restarted */
   CORTEX_M3_CTI_TRIGOUT_DBGRESTART = 7
};

/** Cortex-M7 CTI Trigger Input Configuration */
enum CORTEX_M7_CTI_TRIGIN {
   /** Processor Halted */
   CORTEX_M7_CTI_TRIGIN_DBGHALTED = 0,
   /** DWT Comparator Output 0 */
   CORTEX_M7_CTI_TRIGIN_DWT_CMP_0 = 1,
   /** DWT Comparator Output 1 */
   CORTEX_M7_CTI_TRIGIN_DWT_CMP_1 = 2,
   /** DWT Comparator Output 2 */
   CORTEX_M7_CTI_TRIGIN_DWT_CMP_2 = 3,
   /** ETM Event Output 0 */
   CORTEX_M7_CTI_TRIGIN_ETM_EVENT_OUT_0 = 4,
   /** ETM Event Output 1 */
   CORTEX_M7_CTI_TRIGIN_ETM_EVENT_OUT_1 = 5,
   /** ETM Event Output 2 */
   CORTEX_M7_CTI_TRIGIN_ETM_EVENT_OUT_2 = 6,
   /** ETM Event Output 3 */
   CORTEX_M7_CTI_TRIGIN_ETM_EVENT_OUT_3 = 7
};

/** Cortex-M7 CTI Trigger Output Configuration */
enum CORTEX_M7_CTI_TRIGOUT {
   /** Processor Debug Request (Acknowledged by the debugger writing to the CTIINTACK register) */
   CORTEX_M7_CTI_TRIGOUT_DBGREQ = 0,
   /** Interrupt request 0 (Acknowledged by writing to the CTIINTACK register in ISR) */
   CORTEX_M7_CTI_TRIGOUT_INTERRUPT_REQUEST_0 = 1,
   /** Interrupt request 1 (Acknowledged by writing to the CTIINTACK register in ISR) */
   CORTEX_M7_CTI_TRIGOUT_INTERRUPT_REQUEST_1 = 2,
   /** ETM Event Input 0 */
   CORTEX_M7_CTI_TRIGOUT_ETM_EVENT_INPUT_0 = 3,
   /** ETM Event Input 0 */
   CORTEX_M7_CTI_TRIGOUT_ETM_EVENT_INPUT_1 = 4,
   /** ETM Event Input 0 */
   CORTEX_M7_CTI_TRIGOUT_ETM_EVENT_INPUT_2 = 5,
   /** ETM Event Input 0 */
   CORTEX_M7_CTI_TRIGOUT_ETM_EVENT_INPUT_3 = 6,
   /** Processor Restarted */
   CORTEX_M7_CTI_TRIGOUT_DBGRESTART = 7
};

/** CoreSight DAP Configuration */
enum DAP_CFG {
   /** AHB-AP port ID for APPCPU Debug Access Port */
   DAP_CFG_APPCPU_AP = 0,
   /** AHB-AP port ID for RFCPU Debug Access Port */
   DAP_CFG_RFCPU_AP = 1,
   /** AHB-AP port ID for MDMCPU Debug Access Port */
   DAP_CFG_MDMCPU_AP = 2,
   /** AHB-AP port ID for PHYCPU Debug Access Port */
   DAP_CFG_PHYCPU_AP = 3,
   /** APB-AP port ID for CoreSight Subsystem Debug Access Port */
   DAP_CFG_CS_AP = 4,
   /** AXI-AP port ID for access to App and Modem sub-system non-debug registers and memories */
   DAP_CFG_SYS_AP = 5,
   /** APB-AP port ID for Security Access Port */
   DAP_CFG_SEC_AP = 6
};

/** CoreSight Funnel Configuration */
enum FUNNEL_CFG {
   /** FUNNEL STM ID */
   FUNNEL_CFG_STM = 0,
   /** FUNNEL Modem CPU Instruction ID */
   FUNNEL_CFG_MDMCPU_INSTR = 1,
   /** FUNNEL Modem CPU Data ID */
   FUNNEL_CFG_MDMCPU_DATA = 2,
   /** FUNNEL RF CPU ETM ID */
   FUNNEL_CFG_RFCPU_ETM = 3,
   /** FUNNEL RF CPU ITM ID */
   FUNNEL_CFG_RFCPU_ITM = 4,
   /** FUNNEL PHY CPU ETM ID */
   FUNNEL_CFG_PHYCPU_ETM = 5,
   /** FUNNEL App CPU Instruction ID */
   FUNNEL_CFG_APPCPU_INSTR = 6,
   /** FUNNEL App CPU Data ID */
   FUNNEL_CFG_APPCPU_DATA = 7
};

/** CoreSight Trace Subsystem CTI Trigger Input Configuration */
enum CTITRACE_TRIGIN {
   /** TMC FULL */
   CTITRACE_TRIGIN_TMC_FULL = 0,
   /** TMC ACQCOMP */
   CTITRACE_TRIGIN_TMC_ACQCOMP = 1,
   /** ETR FULL */
   CTITRACE_TRIGIN_ETR_FULL = 2,
   /** ETR ACQCOMP */
   CTITRACE_TRIGIN_ETR_ACQCOMP = 3,
   /** STM_TRIGOUTSPTE */
   CTITRACE_TRIGIN_STM_TRIGOUTSPTE = 4,
   /** STM_TRIGOUTSW */
   CTITRACE_TRIGIN_STM_TRIGOUTSW = 5,
   /** STM_TRIGOUTHETE */
   CTITRACE_TRIGIN_STM_TRIGOUTHETE = 6,
   /** STM ASYNCOUT */
   CTITRACE_TRIGIN_STM_ASYNCOUT = 7
};

/** CoreSight Trace Subsystem CTI Trigger Output Configuration */
enum CTITRACE_TRIGOUT {
   /** TPIU TRIGGER input (handshaked) */
   CTITRACE_TRIGOUT_TPIU_TRIGIN = 0,
   /** TPIU FLUSH input (handshaked) */
   CTITRACE_TRIGOUT_TPIU_FLUSHIN = 1,
   /** TMC TRIGGER input */
   CTITRACE_TRIGOUT_TMC_TRIGIN = 2,
   /** TMC FLUSH input */
   CTITRACE_TRIGOUT_TMC_FLUSHIN = 3,
   /** ETR TRIGGER input */
   CTITRACE_TRIGOUT_ETR_TRIGIN = 4,
   /** ETR FLUSH input */
   CTITRACE_TRIGOUT_ETR_FLUSHIN = 5,
   /** WATCHDOGS and TIMERS Debug request */
   CTITRACE_TRIGOUT_WDGTIM_DBGREQ = 6,
   /** WATCHDOGS and TIMERS Debug restart */
   CTITRACE_TRIGOUT_WDGTIM_DBGRESTART = 7
};

/** Modem Subsystem CTI Trigger Input Configuration */
enum CTIMODEM_TRIGIN {
   /** HW Sequencer Debug Trigger */
   CTIMODEM_TRIGIN_HWSEQ_DBGTRIGGER = 0
};

/** Modem Subsystem CTI Trigger Output Configuration */
enum CTIMODEM_TRIGOUT {
   /** TCU0 Debug Request (handshaked) */
   CTIMODEM_TRIGOUT_TCU0_DBGREQ = 2,
   /** TCU0 Debug Restart (handshaked) */
   CTIMODEM_TRIGOUT_TCU0_DBGRESTART = 3,
   /** TCU1 Debug Request (handshaked) */
   CTIMODEM_TRIGOUT_TCU1_DBGREQ = 4,
   /** TCU1 Debug Restart (handshaked) */
   CTIMODEM_TRIGOUT_TCU1_DBGRESTART = 5,
   /** HWSEQ Debug Request */
   CTIMODEM_TRIGOUT_HWSEQ_DBGREQ = 6,
   /** HWSEQ Debug Restart */
   CTIMODEM_TRIGOUT_HWSEQ_DBGRESTART = 7
};

/** Performance Monitor Configuration */
enum VSPPMU_CFG {
   /** Software Increment */
   VSPPMU_CFG_SWI = 0,
   /** event */
   VSPPMU_CFG_QUEUE0_VDMA_CHANNEL_START = 1,
   /** event */
   VSPPMU_CFG_QUEUE0_VDMA_CHANNEL_LAST_READ = 2,
   /** event */
   VSPPMU_CFG_QUEUE0_VDMA_CHANNEL_END = 3,
   /** event */
   VSPPMU_CFG_QUEUE1_VDMA_CHANNEL_START = 4,
   /** event */
   VSPPMU_CFG_QUEUE1_VDMA_CHANNEL_LAST_READ = 5,
   /** event */
   VSPPMU_CFG_QUEUE1_VDMA_CHANNEL_END = 6,
   /** event */
   VSPPMU_CFG_DE0_MWU_REGION0_HIT = 7,
   /** event */
   VSPPMU_CFG_DE0_MWU_REGION1_HIT = 8,
   /** event */
   VSPPMU_CFG_DE0_MWU_REGION2_HIT = 9,
   /** event */
   VSPPMU_CFG_DE0_MWU_REGION3_HIT = 10,
   /** event */
   VSPPMU_CFG_DE0_DPU_ITCM_READ_TRANS = 11,
   /** event */
   VSPPMU_CFG_DE0_DPU_DTCM_READ_TRANS = 12,
   /** event */
   VSPPMU_CFG_DE0_DPU_DTCM_WRITE_TRANS = 13,
   /** event */
   VSPPMU_CFG_DE0_DPU_DMEM_READ_TRANS = 14,
   /** event */
   VSPPMU_CFG_DE0_DPU_DMEM_WRITE_TRANS = 15,
   /** event */
   VSPPMU_CFG_DE0_DPU_STALL = 16,
   /** event */
   VSPPMU_CFG_DE0_DPU_DMEM_ACCESS_STALL = 17,
   /** event */
   VSPPMU_CFG_DE0_DPU_PPC_STALL = 18,
   /** event */
   VSPPMU_CFG_DE0_DPU_AXI_STALL = 19,
   /** event */
   VSPPMU_CFG_DE0_SLV_READ_TRANS = 20,
   /** event */
   VSPPMU_CFG_DE0_SLV_WRITE_TRANS = 21,
   /** event */
   VSPPMU_CFG_DE0_START = 22,
   /** event */
   VSPPMU_CFG_DE0_END = 23,
   /** event */
   VSPPMU_CFG_BANK0_CONFLICT = 24,
   /** event */
   VSPPMU_CFG_BANK1_CONFLICT = 25,
   /** event */
   VSPPMU_CFG_BANK2_CONFLICT = 26,
   /** event */
   VSPPMU_CFG_BANK3_CONFLICT = 27,
   /** Counter overflowed */
   VSPPMU_CFG_OVF = 254,
   /** Always increment (every cycle) */
   VSPPMU_CFG_ALWAYS = 255
};

/** VSP Subsystem CTI Trigger Input Configuration */
enum VSPSS_CTI_TRIGIN {
   /** VSPSS Debug Trigger */
   VSPSS_CTI_TRIGIN_VDE_DBGTRIGGER = 0
};

/** VSP Subsystem CTI Trigger Output Configuration */
enum VSPSS_CTI_TRIGOUT {
   /** VDE Debug Request */
   VSPSS_CTI_TRIGOUT_VDE_DBGREQ = 0,
   /** VDMA Debug Request */
   VSPSS_CTI_TRIGOUT_VDMA_DBGREQ = 2,
   /** VDE Debug Restart */
   VSPSS_CTI_TRIGOUT_VDE_DBGRESTART = 4,
   /** VDMA Debug Restart */
   VSPSS_CTI_TRIGOUT_VDMA_DBGRESTART = 6
};
struct cti_s;
#ifdef __cplusplus
namespace debug {
#endif
/** CoreSight Trace Subsystem debug space->
 *Trace CTI->
 *Coresight Cross Trigger Interface
*/
static volatile struct cti_s* const cs_trace_ss_dbg_ctitrace = (struct cti_s*)(0x49810000);
#ifdef __cplusplus
}
#endif

struct cxtsgen_s;
#ifdef __cplusplus
namespace debug {
#endif
/** CoreSight Trace Subsystem debug space->
 *CoreSight Timestamp Generator->
 *CoreSight Timestamp Generator
*/
static volatile struct cxtsgen_s* const cs_trace_ss_dbg_tsgen = (struct cxtsgen_s*)(0x49811000);
#ifdef __cplusplus
}
#endif

struct stm_cr_s;
#ifdef __cplusplus
namespace debug {
#endif
/** CoreSight Trace Subsystem debug space->
 *System Trace Macrocell->
 *Configuration Registers
*/
static volatile struct stm_cr_s* const cs_trace_ss_dbg_stm = (struct stm_cr_s*)(0x49812000);
#ifdef __cplusplus
}
#endif

struct cxatbfunnel_s;
#ifdef __cplusplus
namespace debug {
#endif
/** CoreSight Trace Subsystem debug space->
 *CoreSight Funnel->
 *CoreSight Trace funnel
*/
static volatile struct cxatbfunnel_s* const cs_trace_ss_dbg_funnel = (struct cxatbfunnel_s*)(0x49813000);
#ifdef __cplusplus
}
#endif

struct tmc_s;
#ifdef __cplusplus
namespace debug {
#endif
/** CoreSight Trace Subsystem debug space->
 *CoreSight Trace Memory Controller->
 *CoreSight Trace Memory Controller
*/
static volatile struct tmc_s* const cs_trace_ss_dbg_tmc = (struct tmc_s*)(0x49814000);
#ifdef __cplusplus
}
#endif

struct cxatbreplicator_s;
#ifdef __cplusplus
namespace debug {
#endif
/** CoreSight Trace Subsystem debug space->
 *CoreSight ATB Replicator->
 *CoreSight ATB Replicator
*/
static volatile struct cxatbreplicator_s* const cs_trace_ss_dbg_replicator = (struct cxatbreplicator_s*)(0x49815000);
#ifdef __cplusplus
}
#endif

struct tpiu_s;
#ifdef __cplusplus
namespace debug {
#endif
/** CoreSight Trace Subsystem debug space->
 *CoreSight Trace Port Interface Unit->
 *CoreSight Trace Port Interface Unit
*/
static volatile struct tpiu_s* const cs_trace_ss_dbg_tpiu = (struct tpiu_s*)(0x49816000);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace debug {
#endif
/** CoreSight Trace Subsystem debug space->
 *CoreSight Embedded Trace Router->
 *CoreSight Trace Memory Controller
*/
static volatile struct tmc_s* const cs_trace_ss_dbg_etr = (struct tmc_s*)(0x49817000);
#ifdef __cplusplus
}
#endif

struct etr_limiter_s;
#ifdef __cplusplus
namespace debug {
#endif
/** CoreSight Trace Subsystem debug space->
 *ETR Limiter->
 *ETR Limiter
*/
static volatile struct etr_limiter_s* const cs_trace_ss_dbg_etr_limiter = (struct etr_limiter_s*)(0x49818000);
#ifdef __cplusplus
}
#endif

struct stm_filter_s;
#ifdef __cplusplus
namespace debug {
#endif
/** CoreSight Trace Subsystem debug space->
 *STM pre-filter->
 *STM pre-filter
*/
static volatile struct stm_filter_s* const cs_trace_ss_dbg_stm_filter = (struct stm_filter_s*)(0x49819000);
#ifdef __cplusplus
}
#endif

struct ar1de_dbg_s;
#ifdef __cplusplus
namespace debug {
#endif
/** Baseband MODEM Subsystem debug space->
 *VSP0 Debug Port->
 *DE0 Debug Port->
 *Debug Interface
*/
static volatile struct ar1de_dbg_s* const bb_modem_ss_dbg_vsp0_de0_dbg = (struct ar1de_dbg_s*)(0x49820000);
#ifdef __cplusplus
}
#endif

struct ar1de_mwu_s;
#ifdef __cplusplus
namespace debug {
#endif
/** Baseband MODEM Subsystem debug space->
 *VSP0 Debug Port->
 *DE0 MWU->
 *Memory Watch Unit
*/
static volatile struct ar1de_mwu_s* const bb_modem_ss_dbg_vsp0_de0_mwu = (struct ar1de_mwu_s*)(0x49821000);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace debug {
#endif
/** Baseband MODEM Subsystem debug space->
 *VSP0 Debug Port->
 *Cross-Trigger Interface Unit->
 *Coresight Cross Trigger Interface
*/
static volatile struct cti_s* const bb_modem_ss_dbg_vsp0_cti = (struct cti_s*)(0x49824000);
#ifdef __cplusplus
}
#endif

struct pmu_s;
#ifdef __cplusplus
namespace debug {
#endif
/** Baseband MODEM Subsystem debug space->
 *VSP0 Debug Port->
 *Performance Monitor Unit->
 *Performance Monitor Unit.
The performance monitor is used to profile and optimise the performance of software being developed to run on the Ardbeg cluster, by allowing the occurrence of system events to be monitored and analysed.
The PMU is programmed using the debug APB (via the Debug APB Interface).
The trace output generated by the PMU is routed to the CoreSight subsystem which combines the trace data streams from all trace data sources in the system and routes the combined trace data to the external trace port.

*/
static volatile struct pmu_s* const bb_modem_ss_dbg_vsp0_pmu = (struct pmu_s*)(0x49825000);
#ifdef __cplusplus
}
#endif

struct hwsequencer_dbg_s;
#ifdef __cplusplus
namespace debug {
#endif
/** Baseband MODEM Subsystem debug space->
 *HW Sequencer Debug Port->
 *Hardware Sequencer DBG
*/
static volatile struct hwsequencer_dbg_s* const bb_modem_ss_dbg_hwseq = (struct hwsequencer_dbg_s*)(0x49828000);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace debug {
#endif
/** Baseband MODEM Subsystem debug space->
 *Modem CTI->
 *Coresight Cross Trigger Interface
*/
static volatile struct cti_s* const bb_modem_ss_dbg_ctimodem = (struct cti_s*)(0x49829000);
#ifdef __cplusplus
}
#endif

struct stm_stim_s;
#ifdef __cplusplus
namespace debug {
#endif
/** STM Extended Stimulus Ports->
 *Extended Stimulus Ports
*/
static volatile struct stm_stim_s* const stm_stim = (struct stm_stim_s*)(0x50000000);
#ifdef __cplusplus
}
#endif

/* EOF hal_debug.h */
#endif
