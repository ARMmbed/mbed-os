#ifndef INCLUDED_CORTEX_M3_ETM
#define INCLUDED_CORTEX_M3_ETM
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
struct cortex_m3_etm_s {
   /** ETM Control. The ETM Control Register controls general operation of the ETM, such as whether tracing is enabled. at address offset 0x000, read-write */
   uint32_t etmcr;
   /** Configuration Code. The ETM Configuration Code Register enables the debugger to read the implementation-specific configuration of the ETM. Bits [22:20] are fixed at 0 and not supplied by the ASIC. Bits [18:17] are supplied by the MAXEXTIN[1:0] input bus, and read the lower value of MAXEXTIN and the number 2 (the number of EXTINs). This indicates: software accesses supported, trace start/stop block present, no CID comparators, FIFOFULL logic is present, no external outputs, 0-2 external inputs (controlled by MAXEXTIN), no sequencer, no counters, no MMDs, no data comparators, no address comparator pairs. at address offset 0x004, read-only */
   uint32_t etmccr;
   /** Trigger Event. Defines the event that controls the trigger. The trace enable event and trigger event are configured using the same mechanism. For each event, two resources are defined, together with a boolean function of those two resources. at address offset 0x008, read-write */
   uint32_t etmtrigger;
   /** Reserved space */
   uint8_t fill0[4];
   /** ETM Status. The ETM Status Register provides information about the current status of the trace and trigger logic. at address offset 0x010, read-write */
   uint32_t etmsr;
   /** System Configuration. The System Configuration Register shows the ETM features supported by the ASIC. at address offset 0x014, read-only */
   uint32_t etmscr;
   /** Reserved space */
   uint8_t fill1[8];
   /** TraceEnable Event. The TraceEnable Event Register defines the TraceEnable enabling event. The trace enable event and trigger event are configured using the same mechanism. For each event, two resources are defined, together with a boolean function of those two resources. at address offset 0x020, write-only */
   uint32_t etmteevr;
   /** TraceEnable Control 1. The TraceEnable Control 1 Register is one of the registers that configures TraceEnable. Only bit [25], Trace start/stop enable, is implemented. The trace start/stop resource (resource 0x5F) is unaffected by the value of this bit. at address offset 0x024, write-only */
   uint32_t etmtecr1;
   /** FIFOFULL Level. The FIFOFULL Level Register holds the level below which the FIFO is considered full, although its function varies for different ETM architectures. The value in this register also controls the point at which data trace suppression occurs. The maximum valid value for this register is the size of the FIFO. This causes FIFOFULL to be asserted whenever the FIFO is not empty. Behavior is UNPREDICTABLE if the value 0 is written to this register and Stall processor or Suppress data is selected in the ETM Control Register, 0x00. at address offset 0x028, read-write */
   uint32_t etmfflr;
   /** Reserved space */
   uint8_t fill2[276];
   /** FIFOFULL Level. The FIFOFULL Level Register holds the level below which the FIFO is considered full, although its function varies for different ETM architectures. The value in this register also controls the point at which data trace suppression occurs. The maximum valid value for this register is the size of the FIFO. This causes FIFOFULL to be asserted whenever the FIFO is not empty. Behavior is UNPREDICTABLE if the value 0 is written to this register and Stall processor or Suppress data is selected in the ETM Control Register, 0x00. at address offset 0x140, read-write */
   uint32_t etmcntrldvr1;
   /** Reserved space */
   uint8_t fill3[156];
   /** Synchronization Frequency. The Synchronization Frequency Register holds the trace synchronization frequency value. Note: This value must be set to a value greater than the size of the FIFO. at address offset 0x1E0, read-only */
   uint32_t etmsyncfr;
   /** ETM ID. The ETM ID Register holds the ETM architecture variant, and precisely defines the programmer's model for the ETM. This indicates: ARM implementor, special branch encoding, 32-bit Thumb instruction supported, core family is found elsewhere, ETMv3.4, implementation revision 2. at address offset 0x1E4, read-only */
   uint32_t etmidr;
   /** Configuration Code Extension. The Configuration Code Extension Register holds additional bits for ETM configuration code. It describes the extended external inputs. This register indicates: start/stop block uses embedded In Circuit Emulator (ICE) inputs, four embedded ICE inputs, no data comparisons supported, all registers are readable, no extended external input supported. at address offset 0x1E8, read-only */
   uint32_t etmccer;
   /** Reserved space */
   uint8_t fill4[4];
   /** TraceEnable Start/Stop Embedded ICE. The Trace Start/Stop EmbeddedICE Control Register specifies the EmbeddedICE watchpoint comparator inputs that are used as trace start and stop resources. at address offset 0x1F0, read-write */
   uint32_t etmtesseicr;
   /** Reserved space */
   uint8_t fill5[4];
   /** Timestamp Event Register. Defines an event that requests the insertion of a timestamp into the trace stream. This register is implemented only when bit [22] of the ETMCCER is set to 1. at address offset 0x1F8, read-only */
   uint32_t etmtsevr;
   /** Reserved space */
   uint8_t fill6[4];
   /** CoreSight Trace ID. The CoreSight Trace ID Register defines the 7-bit Trace ID, for output to the trace bus. This register is used in systems where multiple trace sources are present and tracing simultaneously. For example, where an ETM outputs trace onto the AMBA 3 Advanced Trace Bus (ATB), a unique ID is required for each trace source so that the trace can be uniquely identified as coming from a particular trace source. at address offset 0x200, read-write */
   uint32_t etmtraceidr;
   /** Reserved space */
   uint8_t fill7[4];
   /** ETM ID Register 2. Provides an extension to the ETM ID register, ETMIDR. at address offset 0x208, read-only */
   uint32_t etmidr2;
   /** Reserved space */
   uint8_t fill8[264];
   /** Power Down Status Register. The Power Down Status Register (PDSR) indicates whether the ETM is powered up or not. Only bit [0] is implemented. Note: If the ETM is not powered up, the ETM registers are not accessible. at address offset 0x314, read-only */
   uint32_t etmpdsr;
   /** Reserved space */
   uint8_t fill9[3016];
   /** ITMISCIN. Sets [1:0] to EXTIN[1:0], [4] to COREHALT. at address offset 0xEE0, read-only */
   uint32_t itmiscin;
   /** Reserved space */
   uint8_t fill10[4];
   /** ITTRIGOUT. Sets [0] to TRIGGER. at address offset 0xEE8, write-only */
   uint32_t ittrigout;
   /** Reserved space */
   uint8_t fill11[4];
   /** ITATBCTR2. A read of this bit returns the value of the ETM ATREADYM input. at address offset 0xEF0, read-only */
   uint32_t etm_itatbctr2;
   /** Reserved space */
   uint8_t fill12[4];
   /** ITATBCTR0. Sets [0] to ATVALID. at address offset 0xEF8, write-only */
   uint32_t etm_itatbctr0;
   /** Reserved space */
   uint8_t fill13[4];
   /** Integration Mode Control. The Integration Mode Control Register (ITCTRL) is used to enable topology detection or to check integration testing. at address offset 0xF00, read-write */
   uint32_t etmitctrl;
   /** Reserved space */
   uint8_t fill14[156];
   /** Claim Tag Set Register. The Claim Tag Set Register (CLAIMSET) is used to set bits in the claim tag and find the number of bits supported by the claim tag. at address offset 0xFA0, read-write */
   uint32_t etmclaimset;
   /** Claim Tag Clear Register. The Claim Tag Clear Register (CLAIMCLR) is used to clear bits in the claim tag to 0, and to find the current value of the claim tag. at address offset 0xFA4, read-write */
   uint32_t etmclaimclr;
   /** Reserved space */
   uint8_t fill15[8];
   /** Lock Access. The Lock Access Register (LAR or LOCKACCESS) is used to lock and unlock access to all other ETM registers. A write of 0xC5ACCE55 unlocks the ETM. A write of any other value locks the ETM. Writes to this register from an interface that ignores the lock registers are ignored. at address offset 0xFB0, write-only */
   uint32_t etmlar;
   /** Lock Status. The Lock Status Register (LSR or LOCKSTATUS) has two uses: 1. If you read this register from any interface, you can check bit [0] to find out whether the lock registers are implemented for the interface you are using. 2. If you read this register from an interface for which lock registers are implemented, you can check bit [1] to find out whether the registers are currently locked. at address offset 0xFB4, read-only */
   uint32_t etmlsr;
   /** Authentication Status. The Authentication Status Register (AUTHSTATUS) reports the level of tracing currently permitted by the authentication signals provided to the ETM. at address offset 0xFB8, read-only */
   uint32_t etmauthstatus;
   /** Reserved space */
   uint8_t fill16[16];
   /** Device Type. The Device Type Register (DEVTYPE) returns the CoreSight device type of the component. at address offset 0xFCC, read-only */
   uint32_t etmdevtype;
   /** Peripheral identification register (ETMPID4). at address offset 0xFD0, read-only */
   uint32_t etmpid4;
   /** Peripheral identification register (ETMPID5). at address offset 0xFD4, read-only */
   uint32_t etmpid5;
   /** Peripheral identification register (ETMPID6). at address offset 0xFD8, read-only */
   uint32_t etmpid6;
   /** Peripheral identification register (ETMPID7). at address offset 0xFDC, read-only */
   uint32_t etmpid7;
   /** Peripheral identification register Bits 7:0 (ETMPID0). at address offset 0xFE0, read-only */
   uint32_t etmpid0;
   /** Peripheral identification register Bits 15:8 (ETMPID1). at address offset 0xFE4, read-only */
   uint32_t etmpid1;
   /** Peripheral identification register Bits 23:16 (ETMPID2). at address offset 0xFE8, read-only */
   uint32_t etmpid2;
   /** Peripheral identification register Bits 31:24 (ETMPID3). at address offset 0xFEC, read-only */
   uint32_t etmpid3;
   /** Component identification register Bits 7:0 (ETMCID0). at address offset 0xFF0, read-only */
   uint32_t etmcid0;
   /** Component identification register Bits 15:8 (ETMCID1). at address offset 0xFF4, read-only */
   uint32_t etmcid1;
   /** Component identification register Bits 23:16 (ETMCID2). at address offset 0xFF8, read-only */
   uint32_t etmcid2;
   /** Component identification register Bits 31:24 (ETMCID3). at address offset 0xFFC, read-only */
   uint32_t etmcid3;
};

/** bit field defines for cortex_m3_etm_s#etmcr */
#define CORTEX_M3_ETM_ETMCR_ETM_POWER_DOWN_OFFSET 0
#define CORTEX_M3_ETM_ETMCR_ETM_POWER_DOWN_SIZE 1
#define CORTEX_M3_ETM_ETMCR_PORT_SIZE_BITS_2_0_OFFSET 4
#define CORTEX_M3_ETM_ETMCR_PORT_SIZE_BITS_2_0_SIZE 3
#define CORTEX_M3_ETM_ETMCR_STALL_PROCESSOR_OFFSET 7
#define CORTEX_M3_ETM_ETMCR_STALL_PROCESSOR_SIZE 1
#define CORTEX_M3_ETM_ETMCR_BRANCH_OUTPUT_OFFSET 8
#define CORTEX_M3_ETM_ETMCR_BRANCH_OUTPUT_SIZE 1
#define CORTEX_M3_ETM_ETMCR_DEBUG_REQUEST_CONTROL_OFFSET 9
#define CORTEX_M3_ETM_ETMCR_DEBUG_REQUEST_CONTROL_SIZE 1
#define CORTEX_M3_ETM_ETMCR_ETM_PROGRAMMING_OFFSET 10
#define CORTEX_M3_ETM_ETMCR_ETM_PROGRAMMING_SIZE 1

/** bit field defines for cortex_m3_etm_s#etmccr */
#define CORTEX_M3_ETM_ETMCCR_ETMCCR_OFFSET 0
#define CORTEX_M3_ETM_ETMCCR_ETMCCR_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmtrigger */
#define CORTEX_M3_ETM_ETMTRIGGER_RESOURCE_B_OFFSET 0
#define CORTEX_M3_ETM_ETMTRIGGER_RESOURCE_B_SIZE 7
#define CORTEX_M3_ETM_ETMTRIGGER_RESOURCE_A_OFFSET 7
#define CORTEX_M3_ETM_ETMTRIGGER_RESOURCE_A_SIZE 7
#define CORTEX_M3_ETM_ETMTRIGGER_BOOLEAN_FUNCTION_OFFSET 14
#define CORTEX_M3_ETM_ETMTRIGGER_BOOLEAN_FUNCTION_SIZE 3

/** bit field defines for cortex_m3_etm_s#etmsr */
#define CORTEX_M3_ETM_ETMSR_UNTRACED_OVERFLOW_FLAG_OFFSET 0
#define CORTEX_M3_ETM_ETMSR_UNTRACED_OVERFLOW_FLAG_SIZE 1
#define CORTEX_M3_ETM_ETMSR_ETM_PROGRAMMING_BIT_VALUE_OFFSET 1
#define CORTEX_M3_ETM_ETMSR_ETM_PROGRAMMING_BIT_VALUE_SIZE 1
#define CORTEX_M3_ETM_ETMSR_TRACE_START_STOP_RESOURCE_STATUS_OFFSET 2
#define CORTEX_M3_ETM_ETMSR_TRACE_START_STOP_RESOURCE_STATUS_SIZE 1
#define CORTEX_M3_ETM_ETMSR_TRIGGER_FLAG_OFFSET 3
#define CORTEX_M3_ETM_ETMSR_TRIGGER_FLAG_SIZE 1

/** bit field defines for cortex_m3_etm_s#etmscr */
#define CORTEX_M3_ETM_ETMSCR_MAXIMUM_PORT_SIZE_BITS_2_0_OFFSET 0
#define CORTEX_M3_ETM_ETMSCR_MAXIMUM_PORT_SIZE_BITS_2_0_SIZE 3
#define CORTEX_M3_ETM_ETMSCR_RESERVED_READS_AS_1_OFFSET 3
#define CORTEX_M3_ETM_ETMSCR_RESERVED_READS_AS_1_SIZE 1
#define CORTEX_M3_ETM_ETMSCR_FIFOFULL_SUPPORTED_OFFSET 8
#define CORTEX_M3_ETM_ETMSCR_FIFOFULL_SUPPORTED_SIZE 1
#define CORTEX_M3_ETM_ETMSCR_MAXIMUM_PORT_SIZE_BIT_3_OFFSET 9
#define CORTEX_M3_ETM_ETMSCR_MAXIMUM_PORT_SIZE_BIT_3_SIZE 1
#define CORTEX_M3_ETM_ETMSCR_PORT_SIZE_SUPPORTED_OFFSET 10
#define CORTEX_M3_ETM_ETMSCR_PORT_SIZE_SUPPORTED_SIZE 1
#define CORTEX_M3_ETM_ETMSCR_PORT_MODE_SUPPORTED_OFFSET 11
#define CORTEX_M3_ETM_ETMSCR_PORT_MODE_SUPPORTED_SIZE 1
#define CORTEX_M3_ETM_ETMSCR_NO_FETCH_COMPARISON_OFFSET 17
#define CORTEX_M3_ETM_ETMSCR_NO_FETCH_COMPARISON_SIZE 1

/** bit field defines for cortex_m3_etm_s#etmteevr */
#define CORTEX_M3_ETM_ETMTEEVR_RESOURCE_B_OFFSET 0
#define CORTEX_M3_ETM_ETMTEEVR_RESOURCE_B_SIZE 7
#define CORTEX_M3_ETM_ETMTEEVR_RESOURCE_A_OFFSET 7
#define CORTEX_M3_ETM_ETMTEEVR_RESOURCE_A_SIZE 7
#define CORTEX_M3_ETM_ETMTEEVR_BOOLEAN_FUNCTION_OFFSET 14
#define CORTEX_M3_ETM_ETMTEEVR_BOOLEAN_FUNCTION_SIZE 3

/** bit field defines for cortex_m3_etm_s#etmtecr1 */
#define CORTEX_M3_ETM_ETMTECR1_TRACE_CONTROL_ENABLE_OFFSET 25
#define CORTEX_M3_ETM_ETMTECR1_TRACE_CONTROL_ENABLE_SIZE 1

/** bit field defines for cortex_m3_etm_s#etmfflr */
#define CORTEX_M3_ETM_ETMFFLR_FIFO_FULL_LEVEL_OFFSET 0
#define CORTEX_M3_ETM_ETMFFLR_FIFO_FULL_LEVEL_SIZE 8

/** bit field defines for cortex_m3_etm_s#etmcntrldvr1 */
#define CORTEX_M3_ETM_ETMCNTRLDVR1_FIFO_FULL_LEVEL_OFFSET 0
#define CORTEX_M3_ETM_ETMCNTRLDVR1_FIFO_FULL_LEVEL_SIZE 8

/** bit field defines for cortex_m3_etm_s#etmsyncfr */
#define CORTEX_M3_ETM_ETMSYNCFR_SYNCHRONIZATION_FREQUENCY_OFFSET 0
#define CORTEX_M3_ETM_ETMSYNCFR_SYNCHRONIZATION_FREQUENCY_SIZE 12

/** bit field defines for cortex_m3_etm_s#etmidr */
#define CORTEX_M3_ETM_ETMIDR_ETMIDR_OFFSET 0
#define CORTEX_M3_ETM_ETMIDR_ETMIDR_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmccer */
#define CORTEX_M3_ETM_ETMCCER_ETMCCER_OFFSET 0
#define CORTEX_M3_ETM_ETMCCER_ETMCCER_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmtesseicr */
#define CORTEX_M3_ETM_ETMTESSEICR_START_RESOURCE_SELECT_OFFSET 0
#define CORTEX_M3_ETM_ETMTESSEICR_START_RESOURCE_SELECT_SIZE 4
#define CORTEX_M3_ETM_ETMTESSEICR_STOP_RESOURCE_SELECT_OFFSET 16
#define CORTEX_M3_ETM_ETMTESSEICR_STOP_RESOURCE_SELECT_SIZE 4

/** bit field defines for cortex_m3_etm_s#etmtsevr */
#define CORTEX_M3_ETM_ETMTSEVR_ETMTSEVR_OFFSET 0
#define CORTEX_M3_ETM_ETMTSEVR_ETMTSEVR_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmtraceidr */
#define CORTEX_M3_ETM_ETMTRACEIDR_SIGNALS_TO_TRACE_BUS_BITS_OFFSET 0
#define CORTEX_M3_ETM_ETMTRACEIDR_SIGNALS_TO_TRACE_BUS_BITS_SIZE 7

/** bit field defines for cortex_m3_etm_s#etmidr2 */
#define CORTEX_M3_ETM_ETMIDR2_RFE_TRANSFER_ORDER_OFFSET 0
#define CORTEX_M3_ETM_ETMIDR2_RFE_TRANSFER_ORDER_SIZE 1
#define CORTEX_M3_ETM_ETMIDR2_SWP_TRANSFER_ORDER_OFFSET 1
#define CORTEX_M3_ETM_ETMIDR2_SWP_TRANSFER_ORDER_SIZE 1

/** bit field defines for cortex_m3_etm_s#etmpdsr */
#define CORTEX_M3_ETM_ETMPDSR_ETM_POWERED_UP_OFFSET 0
#define CORTEX_M3_ETM_ETMPDSR_ETM_POWERED_UP_SIZE 1

/** bit field defines for cortex_m3_etm_s#itmiscin */
#define CORTEX_M3_ETM_ITMISCIN_ITMISCIN_OFFSET 0
#define CORTEX_M3_ETM_ITMISCIN_ITMISCIN_SIZE 32

/** bit field defines for cortex_m3_etm_s#ittrigout */
#define CORTEX_M3_ETM_ITTRIGOUT_ITTRIGOUT_OFFSET 0
#define CORTEX_M3_ETM_ITTRIGOUT_ITTRIGOUT_SIZE 32

/** bit field defines for cortex_m3_etm_s#etm_itatbctr2 */
#define CORTEX_M3_ETM_ETM_ITATBCTR2_ETM_ITATBCTR2_OFFSET 0
#define CORTEX_M3_ETM_ETM_ITATBCTR2_ETM_ITATBCTR2_SIZE 32

/** bit field defines for cortex_m3_etm_s#etm_itatbctr0 */
#define CORTEX_M3_ETM_ETM_ITATBCTR0_ETM_ITATBCTR0_OFFSET 0
#define CORTEX_M3_ETM_ETM_ITATBCTR0_ETM_ITATBCTR0_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmitctrl */
#define CORTEX_M3_ETM_ETMITCTRL_ENABLE_INTEGRATION_MODE_OFFSET 0
#define CORTEX_M3_ETM_ETMITCTRL_ENABLE_INTEGRATION_MODE_SIZE 1

/** bit field defines for cortex_m3_etm_s#etmclaimset */
#define CORTEX_M3_ETM_ETMCLAIMSET_CLAIM_TAG_OFFSET 0
#define CORTEX_M3_ETM_ETMCLAIMSET_CLAIM_TAG_SIZE 4

/** bit field defines for cortex_m3_etm_s#etmclaimclr */
#define CORTEX_M3_ETM_ETMCLAIMCLR_CLAIM_TAG_OFFSET 0
#define CORTEX_M3_ETM_ETMCLAIMCLR_CLAIM_TAG_SIZE 4

/** bit field defines for cortex_m3_etm_s#etmlar */
#define CORTEX_M3_ETM_ETMLAR_ETMLAR_OFFSET 0
#define CORTEX_M3_ETM_ETMLAR_ETMLAR_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmlsr */
#define CORTEX_M3_ETM_ETMLSR_ETMLSR_OFFSET 0
#define CORTEX_M3_ETM_ETMLSR_ETMLSR_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmauthstatus */
#define CORTEX_M3_ETM_ETMAUTHSTATUS_NONSECURE_NON_INVASIVE_DEBUG_OFFSET 0
#define CORTEX_M3_ETM_ETMAUTHSTATUS_NONSECURE_NON_INVASIVE_DEBUG_SIZE 2
#define CORTEX_M3_ETM_ETMAUTHSTATUS_SECURE_INVASIVE_DEBUG_OFFSET 4
#define CORTEX_M3_ETM_ETMAUTHSTATUS_SECURE_INVASIVE_DEBUG_SIZE 2
#define CORTEX_M3_ETM_ETMAUTHSTATUS_SECURE_NON_INVASIVE_DEBUG_OFFSET 6
#define CORTEX_M3_ETM_ETMAUTHSTATUS_SECURE_NON_INVASIVE_DEBUG_SIZE 2

/** bit field defines for cortex_m3_etm_s#etmdevtype */
#define CORTEX_M3_ETM_ETMDEVTYPE_MAIN_TYPE_OFFSET 0
#define CORTEX_M3_ETM_ETMDEVTYPE_MAIN_TYPE_SIZE 4
#define CORTEX_M3_ETM_ETMDEVTYPE_SUB_TYPE_OFFSET 4
#define CORTEX_M3_ETM_ETMDEVTYPE_SUB_TYPE_SIZE 4

/** bit field defines for cortex_m3_etm_s#etmpid4 */
#define CORTEX_M3_ETM_ETMPID4_ETMPID4_OFFSET 0
#define CORTEX_M3_ETM_ETMPID4_ETMPID4_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmpid5 */
#define CORTEX_M3_ETM_ETMPID5_ETMPID5_OFFSET 0
#define CORTEX_M3_ETM_ETMPID5_ETMPID5_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmpid6 */
#define CORTEX_M3_ETM_ETMPID6_ETMPID6_OFFSET 0
#define CORTEX_M3_ETM_ETMPID6_ETMPID6_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmpid7 */
#define CORTEX_M3_ETM_ETMPID7_ETMPID7_OFFSET 0
#define CORTEX_M3_ETM_ETMPID7_ETMPID7_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmpid0 */
#define CORTEX_M3_ETM_ETMPID0_ETMPID0_OFFSET 0
#define CORTEX_M3_ETM_ETMPID0_ETMPID0_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmpid1 */
#define CORTEX_M3_ETM_ETMPID1_ETMPID1_OFFSET 0
#define CORTEX_M3_ETM_ETMPID1_ETMPID1_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmpid2 */
#define CORTEX_M3_ETM_ETMPID2_ETMPID2_OFFSET 0
#define CORTEX_M3_ETM_ETMPID2_ETMPID2_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmpid3 */
#define CORTEX_M3_ETM_ETMPID3_ETMPID3_OFFSET 0
#define CORTEX_M3_ETM_ETMPID3_ETMPID3_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmcid0 */
#define CORTEX_M3_ETM_ETMCID0_ETMCID0_OFFSET 0
#define CORTEX_M3_ETM_ETMCID0_ETMCID0_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmcid1 */
#define CORTEX_M3_ETM_ETMCID1_ETMCID1_OFFSET 0
#define CORTEX_M3_ETM_ETMCID1_ETMCID1_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmcid2 */
#define CORTEX_M3_ETM_ETMCID2_ETMCID2_OFFSET 0
#define CORTEX_M3_ETM_ETMCID2_ETMCID2_SIZE 32

/** bit field defines for cortex_m3_etm_s#etmcid3 */
#define CORTEX_M3_ETM_ETMCID3_ETMCID3_OFFSET 0
#define CORTEX_M3_ETM_ETMCID3_ETMCID3_SIZE 32

/* EOF cortex_m3_etm.h */
#endif
