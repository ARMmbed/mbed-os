#ifndef INCLUDED_CORTEX_M7_DWT
#define INCLUDED_CORTEX_M7_DWT
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
struct cortex_m7_dwt_s {
   /** DWT Control Register. Use the DWT Control Register to enable the DWT unit. at address offset 0x000, read-write */
   uint32_t dwt_ctrl;
   /** DWT Current PC Sampler Cycle Count Register. Use the DWT Current PC Sampler Cycle Count Register to count the number of core cycles. This count can measure elapsed execution time. This is a free-running counter. The counter has three functions: (1) When PCSAMPLENA is set, the PC is sampled and emitted when the selected tapped bit changes value (0 to 1 or 1 to 0) and any post-scalar value counts to 0. (2) When CYCEVTENA is set (and PCSAMPLENA is clear), an event is emitted when the selected tapped bit changes value (0 to 1 or 1 to 0) and any post-scalar value counts to 0. (3) Applications and debuggers can use the counter to measure elapsed execution time. By subtracting a start and an end time, an application can measure time between in-core clocks (other than when Halted in debug). This is valid to 232 core clock cycles (for example, almost 86 seconds at 50MHz). at address offset 0x004, read-write */
   uint32_t dwt_cyccnt;
   /** DWT CPI Count Register. Use the DWT CPI Count Register to count the total number of instruction cycles beyond the first cycle. at address offset 0x008, read-write */
   uint32_t dwt_cpicnt;
   /** DWT Exception Overhead Count Register. Use the DWT Exception Overhead Count Register to count the total cycles spent in interrupt processing. at address offset 0x00C, read-write */
   uint32_t dwt_exccnt;
   /** DWT Sleep Count Register. Use the DWT Sleep Count Register to count the total number of cycles during which the processor is sleeping. at address offset 0x010, read-write */
   uint32_t dwt_sleepcnt;
   /** DWT LSU Count Register. Use the DWT LSU Count Register to count the total number of cycles during which the processor is processing an LSU operation beyond the first cycle. at address offset 0x014, read-write */
   uint32_t dwt_lsucnt;
   /** DWT Fold Count Register. Use the DWT Fold Count Register to count the total number of folded instructions. This counts 1 for each instruction that takes 0 cycles. at address offset 0x018, read-write */
   uint32_t dwt_foldcnt;
   /** DWT Program Counter Sample Register. Use the DWT Program Counter Sample Register (PCSR) to enable coarse-grained software profiling using a debug agent, without changing the currently executing code. If the core is not in debug state, the value returned is the instruction address of a recently executed instruction. If the core is in debug state, the value returned is 0xFFFFFFFF. at address offset 0x01C, read-only */
   uint32_t dwt_pcsr;
   /** DWT Comparator Register 0. Use the DWT Comparator Registers 0-3 to write the values that trigger watchpoint events. at address offset 0x020, read-write */
   uint32_t dwt_comp0;
   /** DWT Mask Register 0. Use the DWT Mask Registers 0-3 to apply a mask to data addresses when matching against COMP. at address offset 0x024, read-write */
   uint32_t dwt_mask0;
   /** DWT Function Register 0. Use the DWT Function Registers 0-3 to control the operation of the comparator. Each comparator can: 1. Match against either the PC or the data address. This is controlled by CYCMATCH. This function is only available for comparator 0 (DWT_COMP0). 2. Perform data value comparisons if associated address comparators have performed an address match. This function is only available for comparator 1 (DWT_COMP1). 3. Emit data or PC couples, trigger the ETM, or generate a watchpoint depending on the operation defined by FUNCTION. at address offset 0x028, read-write */
   uint32_t dwt_function0;
   /** Reserved space */
   uint8_t fill0[4];
   /** DWT Comparator Register 1. Use the DWT Comparator Registers 0-3 to write the values that trigger watchpoint events. at address offset 0x030, read-write */
   uint32_t dwt_comp1;
   /** DWT Mask Register 1. Use the DWT Mask Registers 0-3 to apply a mask to data addresses when matching against COMP. at address offset 0x034, read-write */
   uint32_t dwt_mask1;
   /** DWT Function Register 1. Use the DWT Function Registers 0-3 to control the operation of the comparator. Each comparator can: 1. Match against either the PC or the data address. This is controlled by CYCMATCH. This function is only available for comparator 0 (DWT_COMP0). 2. Perform data value comparisons if associated address comparators have performed an address match. This function is only available for comparator 1 (DWT_COMP1). 3. Emit data or PC couples, trigger the ETM, or generate a watchpoint depending on the operation defined by FUNCTION. at address offset 0x038, read-write */
   uint32_t dwt_function1;
   /** Reserved space */
   uint8_t fill1[4];
   /** DWT Comparator Register 2. Use the DWT Comparator Registers 0-3 to write the values that trigger watchpoint events. at address offset 0x040, read-write */
   uint32_t dwt_comp2;
   /** DWT Mask Register 2. Use the DWT Mask Registers 0-3 to apply a mask to data addresses when matching against COMP. at address offset 0x044, read-write */
   uint32_t dwt_mask2;
   /** DWT Function Register 2. Use the DWT Function Registers 0-3 to control the operation of the comparator. Each comparator can: 1. Match against either the PC or the data address. This is controlled by CYCMATCH. This function is only available for comparator 0 (DWT_COMP0). 2. Perform data value comparisons if associated address comparators have performed an address match. This function is only available for comparator 1 (DWT_COMP1). 3. Emit data or PC couples, trigger the ETM, or generate a watchpoint depending on the operation defined by FUNCTION. at address offset 0x048, read-write */
   uint32_t dwt_function2;
   /** Reserved space */
   uint8_t fill2[4];
   /** DWT Comparator Register 3. Use the DWT Comparator Registers 0-3 to write the values that trigger watchpoint events. at address offset 0x050, read-write */
   uint32_t dwt_comp3;
   /** DWT Mask Register 3. Use the DWT Mask Registers 0-3 to apply a mask to data addresses when matching against COMP. at address offset 0x054, read-write */
   uint32_t dwt_mask3;
   /** DWT Function Register 3. Use the DWT Function Registers 0-3 to control the operation of the comparator. Each comparator can: 1. Match against either the PC or the data address. This is controlled by CYCMATCH. This function is only available for comparator 0 (DWT_COMP0). 2. Perform data value comparisons if associated address comparators have performed an address match. This function is only available for comparator 1 (DWT_COMP1). 3. Emit data or PC couples, trigger the ETM, or generate a watchpoint depending on the operation defined by FUNCTION. at address offset 0x058, read-write */
   uint32_t dwt_function3;
   /** Reserved space */
   uint8_t fill3[3924];
   /** DWT Lock Access Register. Use this register to prevent write accesses to the Control Register. at address offset 0xFB0, write-only */
   uint32_t dwt_lar;
   /** DWT Lock Status Register. Use this register to enable write accesses to the Control Register. at address offset 0xFB4, read-only */
   uint32_t dwt_lsr;
   /** Reserved space */
   uint8_t fill4[24];
   /** Peripheral identification register (PID4). at address offset 0xFD0, read-only */
   uint32_t pid4;
   /** Peripheral identification register (PID5). at address offset 0xFD4, read-only */
   uint32_t pid5;
   /** Peripheral identification register (PID6). at address offset 0xFD8, read-only */
   uint32_t pid6;
   /** Peripheral identification register (PID7). at address offset 0xFDC, read-only */
   uint32_t pid7;
   /** Peripheral identification register Bits 7:0 (PID0). at address offset 0xFE0, read-only */
   uint32_t pid0;
   /** Peripheral identification register Bits 15:8 (PID1). at address offset 0xFE4, read-only */
   uint32_t pid1;
   /** Peripheral identification register Bits 23:16 (PID2). at address offset 0xFE8, read-only */
   uint32_t pid2;
   /** Peripheral identification register Bits 31:24 (PID3). at address offset 0xFEC, read-only */
   uint32_t pid3;
   /** Component identification register Bits 7:0 (CID0). at address offset 0xFF0, read-only */
   uint32_t cid0;
   /** Component identification register Bits 15:8 (CID1). at address offset 0xFF4, read-only */
   uint32_t cid1;
   /** Component identification register Bits 23:16 (CID2). at address offset 0xFF8, read-only */
   uint32_t cid2;
   /** Component identification register Bits 31:24 (CID3). at address offset 0xFFC, read-only */
   uint32_t cid3;
};

/** bit field defines for cortex_m7_dwt_s#dwt_ctrl */
#define CORTEX_M7_DWT_DWT_CTRL_CYCCNTENA_OFFSET 0
#define CORTEX_M7_DWT_DWT_CTRL_CYCCNTENA_SIZE 1
#define CORTEX_M7_DWT_DWT_CTRL_POSTPRESET_OFFSET 1
#define CORTEX_M7_DWT_DWT_CTRL_POSTPRESET_SIZE 4
#define CORTEX_M7_DWT_DWT_CTRL_POSTCNT_OFFSET 5
#define CORTEX_M7_DWT_DWT_CTRL_POSTCNT_SIZE 4
#define CORTEX_M7_DWT_DWT_CTRL_CYCTAP_OFFSET 9
#define CORTEX_M7_DWT_DWT_CTRL_CYCTAP_SIZE 1
#define CORTEX_M7_DWT_DWT_CTRL_SYNCTAP_OFFSET 10
#define CORTEX_M7_DWT_DWT_CTRL_SYNCTAP_SIZE 2
#define CORTEX_M7_DWT_DWT_CTRL_PCSAMPLEENA_OFFSET 12
#define CORTEX_M7_DWT_DWT_CTRL_PCSAMPLEENA_SIZE 1
#define CORTEX_M7_DWT_DWT_CTRL_EXCTRCENA_OFFSET 16
#define CORTEX_M7_DWT_DWT_CTRL_EXCTRCENA_SIZE 1
#define CORTEX_M7_DWT_DWT_CTRL_CPIEVTENA_OFFSET 17
#define CORTEX_M7_DWT_DWT_CTRL_CPIEVTENA_SIZE 1
#define CORTEX_M7_DWT_DWT_CTRL_EXCEVTENA_OFFSET 18
#define CORTEX_M7_DWT_DWT_CTRL_EXCEVTENA_SIZE 1
#define CORTEX_M7_DWT_DWT_CTRL_SLEEPEVTENA_OFFSET 19
#define CORTEX_M7_DWT_DWT_CTRL_SLEEPEVTENA_SIZE 1
#define CORTEX_M7_DWT_DWT_CTRL_LSUEVTENA_OFFSET 20
#define CORTEX_M7_DWT_DWT_CTRL_LSUEVTENA_SIZE 1
#define CORTEX_M7_DWT_DWT_CTRL_FOLDEVTENA_OFFSET 21
#define CORTEX_M7_DWT_DWT_CTRL_FOLDEVTENA_SIZE 1
#define CORTEX_M7_DWT_DWT_CTRL_CYCEVTENA_OFFSET 22
#define CORTEX_M7_DWT_DWT_CTRL_CYCEVTENA_SIZE 1
#define CORTEX_M7_DWT_DWT_CTRL_NOPRFCNT_OFFSET 24
#define CORTEX_M7_DWT_DWT_CTRL_NOPRFCNT_SIZE 1
#define CORTEX_M7_DWT_DWT_CTRL_NOCYCCNT_OFFSET 25
#define CORTEX_M7_DWT_DWT_CTRL_NOCYCCNT_SIZE 1
#define CORTEX_M7_DWT_DWT_CTRL_NOEXTTRIG_OFFSET 26
#define CORTEX_M7_DWT_DWT_CTRL_NOEXTTRIG_SIZE 1
#define CORTEX_M7_DWT_DWT_CTRL_NOTRCPKT_OFFSET 27
#define CORTEX_M7_DWT_DWT_CTRL_NOTRCPKT_SIZE 1
#define CORTEX_M7_DWT_DWT_CTRL_NUMCOMP_OFFSET 28
#define CORTEX_M7_DWT_DWT_CTRL_NUMCOMP_SIZE 4

/** bit field defines for cortex_m7_dwt_s#dwt_cyccnt */
#define CORTEX_M7_DWT_DWT_CYCCNT_CYCCNT_OFFSET 0
#define CORTEX_M7_DWT_DWT_CYCCNT_CYCCNT_SIZE 32

/** bit field defines for cortex_m7_dwt_s#dwt_cpicnt */
#define CORTEX_M7_DWT_DWT_CPICNT_CPICNT_OFFSET 0
#define CORTEX_M7_DWT_DWT_CPICNT_CPICNT_SIZE 8

/** bit field defines for cortex_m7_dwt_s#dwt_exccnt */
#define CORTEX_M7_DWT_DWT_EXCCNT_EXCCNT_OFFSET 0
#define CORTEX_M7_DWT_DWT_EXCCNT_EXCCNT_SIZE 8

/** bit field defines for cortex_m7_dwt_s#dwt_sleepcnt */
#define CORTEX_M7_DWT_DWT_SLEEPCNT_SLEEPCNT_OFFSET 0
#define CORTEX_M7_DWT_DWT_SLEEPCNT_SLEEPCNT_SIZE 8

/** bit field defines for cortex_m7_dwt_s#dwt_lsucnt */
#define CORTEX_M7_DWT_DWT_LSUCNT_LSUCNT_OFFSET 0
#define CORTEX_M7_DWT_DWT_LSUCNT_LSUCNT_SIZE 8

/** bit field defines for cortex_m7_dwt_s#dwt_foldcnt */
#define CORTEX_M7_DWT_DWT_FOLDCNT_FOLDCNT_OFFSET 0
#define CORTEX_M7_DWT_DWT_FOLDCNT_FOLDCNT_SIZE 8

/** bit field defines for cortex_m7_dwt_s#dwt_pcsr */
#define CORTEX_M7_DWT_DWT_PCSR_EIASAMPLE_OFFSET 0
#define CORTEX_M7_DWT_DWT_PCSR_EIASAMPLE_SIZE 32

/** bit field defines for cortex_m7_dwt_s#dwt_comp0 */
#define CORTEX_M7_DWT_DWT_COMP0_COMP_OFFSET 0
#define CORTEX_M7_DWT_DWT_COMP0_COMP_SIZE 32

/** bit field defines for cortex_m7_dwt_s#dwt_mask0 */
#define CORTEX_M7_DWT_DWT_MASK0_MASK_OFFSET 0
#define CORTEX_M7_DWT_DWT_MASK0_MASK_SIZE 5

/** bit field defines for cortex_m7_dwt_s#dwt_function0 */
#define CORTEX_M7_DWT_DWT_FUNCTION0_FUNC_OFFSET 0
#define CORTEX_M7_DWT_DWT_FUNCTION0_FUNC_SIZE 4
#define CORTEX_M7_DWT_DWT_FUNCTION0_EMITRANGE_OFFSET 5
#define CORTEX_M7_DWT_DWT_FUNCTION0_EMITRANGE_SIZE 1
#define CORTEX_M7_DWT_DWT_FUNCTION0_CYCMATCH_OFFSET 7
#define CORTEX_M7_DWT_DWT_FUNCTION0_CYCMATCH_SIZE 1
#define CORTEX_M7_DWT_DWT_FUNCTION0_DATAVMATCH_OFFSET 8
#define CORTEX_M7_DWT_DWT_FUNCTION0_DATAVMATCH_SIZE 1
#define CORTEX_M7_DWT_DWT_FUNCTION0_LNK1ENA_OFFSET 9
#define CORTEX_M7_DWT_DWT_FUNCTION0_LNK1ENA_SIZE 1
#define CORTEX_M7_DWT_DWT_FUNCTION0_DATAVSIZE_OFFSET 10
#define CORTEX_M7_DWT_DWT_FUNCTION0_DATAVSIZE_SIZE 2
#define CORTEX_M7_DWT_DWT_FUNCTION0_DATAVADDR0_OFFSET 12
#define CORTEX_M7_DWT_DWT_FUNCTION0_DATAVADDR0_SIZE 4
#define CORTEX_M7_DWT_DWT_FUNCTION0_DATAVADDR1_OFFSET 16
#define CORTEX_M7_DWT_DWT_FUNCTION0_DATAVADDR1_SIZE 4
#define CORTEX_M7_DWT_DWT_FUNCTION0_MATCHED_OFFSET 24
#define CORTEX_M7_DWT_DWT_FUNCTION0_MATCHED_SIZE 1

/** bit field defines for cortex_m7_dwt_s#dwt_comp1 */
#define CORTEX_M7_DWT_DWT_COMP1_COMP_OFFSET 0
#define CORTEX_M7_DWT_DWT_COMP1_COMP_SIZE 32

/** bit field defines for cortex_m7_dwt_s#dwt_mask1 */
#define CORTEX_M7_DWT_DWT_MASK1_MASK_OFFSET 0
#define CORTEX_M7_DWT_DWT_MASK1_MASK_SIZE 5

/** bit field defines for cortex_m7_dwt_s#dwt_function1 */
#define CORTEX_M7_DWT_DWT_FUNCTION1_FUNC_OFFSET 0
#define CORTEX_M7_DWT_DWT_FUNCTION1_FUNC_SIZE 4
#define CORTEX_M7_DWT_DWT_FUNCTION1_EMITRANGE_OFFSET 5
#define CORTEX_M7_DWT_DWT_FUNCTION1_EMITRANGE_SIZE 1
#define CORTEX_M7_DWT_DWT_FUNCTION1_DATAVMATCH_OFFSET 8
#define CORTEX_M7_DWT_DWT_FUNCTION1_DATAVMATCH_SIZE 1
#define CORTEX_M7_DWT_DWT_FUNCTION1_LNK1ENA_OFFSET 9
#define CORTEX_M7_DWT_DWT_FUNCTION1_LNK1ENA_SIZE 1
#define CORTEX_M7_DWT_DWT_FUNCTION1_DATAVSIZE_OFFSET 10
#define CORTEX_M7_DWT_DWT_FUNCTION1_DATAVSIZE_SIZE 2
#define CORTEX_M7_DWT_DWT_FUNCTION1_DATAVADDR0_OFFSET 12
#define CORTEX_M7_DWT_DWT_FUNCTION1_DATAVADDR0_SIZE 4
#define CORTEX_M7_DWT_DWT_FUNCTION1_DATAVADDR1_OFFSET 16
#define CORTEX_M7_DWT_DWT_FUNCTION1_DATAVADDR1_SIZE 4
#define CORTEX_M7_DWT_DWT_FUNCTION1_MATCHED_OFFSET 24
#define CORTEX_M7_DWT_DWT_FUNCTION1_MATCHED_SIZE 1

/** bit field defines for cortex_m7_dwt_s#dwt_comp2 */
#define CORTEX_M7_DWT_DWT_COMP2_COMP_OFFSET 0
#define CORTEX_M7_DWT_DWT_COMP2_COMP_SIZE 32

/** bit field defines for cortex_m7_dwt_s#dwt_mask2 */
#define CORTEX_M7_DWT_DWT_MASK2_MASK_OFFSET 0
#define CORTEX_M7_DWT_DWT_MASK2_MASK_SIZE 5

/** bit field defines for cortex_m7_dwt_s#dwt_function2 */
#define CORTEX_M7_DWT_DWT_FUNCTION2_FUNC_OFFSET 0
#define CORTEX_M7_DWT_DWT_FUNCTION2_FUNC_SIZE 4
#define CORTEX_M7_DWT_DWT_FUNCTION2_EMITRANGE_OFFSET 5
#define CORTEX_M7_DWT_DWT_FUNCTION2_EMITRANGE_SIZE 1
#define CORTEX_M7_DWT_DWT_FUNCTION2_DATAVMATCH_OFFSET 8
#define CORTEX_M7_DWT_DWT_FUNCTION2_DATAVMATCH_SIZE 1
#define CORTEX_M7_DWT_DWT_FUNCTION2_LNK1ENA_OFFSET 9
#define CORTEX_M7_DWT_DWT_FUNCTION2_LNK1ENA_SIZE 1
#define CORTEX_M7_DWT_DWT_FUNCTION2_DATAVSIZE_OFFSET 10
#define CORTEX_M7_DWT_DWT_FUNCTION2_DATAVSIZE_SIZE 2
#define CORTEX_M7_DWT_DWT_FUNCTION2_DATAVADDR0_OFFSET 12
#define CORTEX_M7_DWT_DWT_FUNCTION2_DATAVADDR0_SIZE 4
#define CORTEX_M7_DWT_DWT_FUNCTION2_DATAVADDR1_OFFSET 16
#define CORTEX_M7_DWT_DWT_FUNCTION2_DATAVADDR1_SIZE 4
#define CORTEX_M7_DWT_DWT_FUNCTION2_MATCHED_OFFSET 24
#define CORTEX_M7_DWT_DWT_FUNCTION2_MATCHED_SIZE 1

/** bit field defines for cortex_m7_dwt_s#dwt_comp3 */
#define CORTEX_M7_DWT_DWT_COMP3_COMP_OFFSET 0
#define CORTEX_M7_DWT_DWT_COMP3_COMP_SIZE 32

/** bit field defines for cortex_m7_dwt_s#dwt_mask3 */
#define CORTEX_M7_DWT_DWT_MASK3_MASK_OFFSET 0
#define CORTEX_M7_DWT_DWT_MASK3_MASK_SIZE 5

/** bit field defines for cortex_m7_dwt_s#dwt_function3 */
#define CORTEX_M7_DWT_DWT_FUNCTION3_FUNC_OFFSET 0
#define CORTEX_M7_DWT_DWT_FUNCTION3_FUNC_SIZE 4
#define CORTEX_M7_DWT_DWT_FUNCTION3_EMITRANGE_OFFSET 5
#define CORTEX_M7_DWT_DWT_FUNCTION3_EMITRANGE_SIZE 1
#define CORTEX_M7_DWT_DWT_FUNCTION3_DATAVMATCH_OFFSET 8
#define CORTEX_M7_DWT_DWT_FUNCTION3_DATAVMATCH_SIZE 1
#define CORTEX_M7_DWT_DWT_FUNCTION3_LNK1ENA_OFFSET 9
#define CORTEX_M7_DWT_DWT_FUNCTION3_LNK1ENA_SIZE 1
#define CORTEX_M7_DWT_DWT_FUNCTION3_DATAVSIZE_OFFSET 10
#define CORTEX_M7_DWT_DWT_FUNCTION3_DATAVSIZE_SIZE 2
#define CORTEX_M7_DWT_DWT_FUNCTION3_DATAVADDR0_OFFSET 12
#define CORTEX_M7_DWT_DWT_FUNCTION3_DATAVADDR0_SIZE 4
#define CORTEX_M7_DWT_DWT_FUNCTION3_DATAVADDR1_OFFSET 16
#define CORTEX_M7_DWT_DWT_FUNCTION3_DATAVADDR1_SIZE 4
#define CORTEX_M7_DWT_DWT_FUNCTION3_MATCHED_OFFSET 24
#define CORTEX_M7_DWT_DWT_FUNCTION3_MATCHED_SIZE 1

/** bit field defines for cortex_m7_dwt_s#dwt_lar */
#define CORTEX_M7_DWT_DWT_LAR_DWT_LAR_OFFSET 0
#define CORTEX_M7_DWT_DWT_LAR_DWT_LAR_SIZE 32

/** bit field defines for cortex_m7_dwt_s#dwt_lsr */
#define CORTEX_M7_DWT_DWT_LSR_DWT_LSR_OFFSET 0
#define CORTEX_M7_DWT_DWT_LSR_DWT_LSR_SIZE 32

/** bit field defines for cortex_m7_dwt_s#pid4 */
#define CORTEX_M7_DWT_PID4_PID4_OFFSET 0
#define CORTEX_M7_DWT_PID4_PID4_SIZE 32

/** bit field defines for cortex_m7_dwt_s#pid5 */
#define CORTEX_M7_DWT_PID5_PID5_OFFSET 0
#define CORTEX_M7_DWT_PID5_PID5_SIZE 32

/** bit field defines for cortex_m7_dwt_s#pid6 */
#define CORTEX_M7_DWT_PID6_PID6_OFFSET 0
#define CORTEX_M7_DWT_PID6_PID6_SIZE 32

/** bit field defines for cortex_m7_dwt_s#pid7 */
#define CORTEX_M7_DWT_PID7_PID7_OFFSET 0
#define CORTEX_M7_DWT_PID7_PID7_SIZE 32

/** bit field defines for cortex_m7_dwt_s#pid0 */
#define CORTEX_M7_DWT_PID0_PID0_OFFSET 0
#define CORTEX_M7_DWT_PID0_PID0_SIZE 32

/** bit field defines for cortex_m7_dwt_s#pid1 */
#define CORTEX_M7_DWT_PID1_PID1_OFFSET 0
#define CORTEX_M7_DWT_PID1_PID1_SIZE 32

/** bit field defines for cortex_m7_dwt_s#pid2 */
#define CORTEX_M7_DWT_PID2_PID2_OFFSET 0
#define CORTEX_M7_DWT_PID2_PID2_SIZE 32

/** bit field defines for cortex_m7_dwt_s#pid3 */
#define CORTEX_M7_DWT_PID3_PID3_OFFSET 0
#define CORTEX_M7_DWT_PID3_PID3_SIZE 32

/** bit field defines for cortex_m7_dwt_s#cid0 */
#define CORTEX_M7_DWT_CID0_CID0_OFFSET 0
#define CORTEX_M7_DWT_CID0_CID0_SIZE 32

/** bit field defines for cortex_m7_dwt_s#cid1 */
#define CORTEX_M7_DWT_CID1_CID1_OFFSET 0
#define CORTEX_M7_DWT_CID1_CID1_SIZE 32

/** bit field defines for cortex_m7_dwt_s#cid2 */
#define CORTEX_M7_DWT_CID2_CID2_OFFSET 0
#define CORTEX_M7_DWT_CID2_CID2_SIZE 32

/** bit field defines for cortex_m7_dwt_s#cid3 */
#define CORTEX_M7_DWT_CID3_CID3_OFFSET 0
#define CORTEX_M7_DWT_CID3_CID3_SIZE 32

/* EOF cortex_m7_dwt.h */
#endif
