#ifndef INCLUDED_CORTEX_M7_SCS
#define INCLUDED_CORTEX_M7_SCS
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>


/** System control, MPU, debug and NVIC registers
*/
struct cortex_m7_scs_s {
   /** Reserved space */
   uint8_t fill0[4];
   /** Interrupt Control Type Register. Read the Interrupt Controller Type Register to see the number of interrupt lines that the NVIC supports. at address offset 0x004, read-only */
   uint32_t ictr;
   /** Auxiliary Control Register. Use the Auxiliary Control Register to disable certain aspects of functionality within the processor at address offset 0x008, read-write */
   uint32_t actlr;
   /** Reserved space */
   uint8_t fill1[4];
   /** SysTick Control and Status Register. Use the SysTick Control and Status Register to enable the SysTick features. at address offset 0x010, read-write */
   uint32_t syst_csr;
   /** SysTick Reload Value Register. Use the SysTick Reload Value Register to specify the start value to load into the current value register when the counter reaches 0. It can be any value between 1 and 0x00FFFFFF. A start value of 0 is possible, but has no effect because the SysTick interrupt and COUNTFLAG are activated when counting from 1 to 0. at address offset 0x014, read-write */
   uint32_t syst_rvr;
   /** SysTick Current Value Register. Use the SysTick Current Value Register to find the current value in the register. at address offset 0x018, read-write */
   uint32_t syst_cvr;
   /** SysTick Calibration Value Register. Use the SysTick Calibration Value Register to enable software to scale to any required speed using divide and multiply. at address offset 0x01C, read-only */
   uint32_t syst_calib;
   /** Reserved space */
   uint8_t fill2[224];
   /** Interupt Set_Enable register at address offset 0x100, read-write */
   uint32_t nvic_iser[8];
   /** Reserved space */
   uint8_t fill3[96];
   /** Interupt Clear_Enable register at address offset 0x180, read-write */
   uint32_t nvic_icer[8];
   /** Reserved space */
   uint8_t fill4[96];
   /** Interupt Set_Pending register at address offset 0x200, read-write */
   uint32_t nvic_ispr[8];
   /** Reserved space */
   uint8_t fill5[96];
   /** Interupt Clear_Pending register at address offset 0x280, read-write */
   uint32_t nvic_icpr[8];
   /** Reserved space */
   uint8_t fill6[96];
   /** Interupt active bit register at address offset 0x300, read-only */
   uint32_t nvic_iabr[8];
   /** Reserved space */
   uint8_t fill7[224];
   /** Interupt active bit register at address offset 0x400, read-write */
   uint32_t nvic_ipr[60];
   /** Reserved space */
   uint8_t fill8[2064];
   /** CPUID Base Register. Read the CPU ID Base Register to determine: the ID number of the processor core, the version number of the processor core, the implementation details of the processor core. at address offset 0xD00, read-only */
   uint32_t cpuid;
   /** Interrupt Control State Register. Use the Interrupt Control State Register to set a pending Non-Maskable Interrupt (NMI), set or clear a pending SVC, set or clear a pending SysTick, check for pending exceptions, check the vector number of the highest priority pended exception, check the vector number of the active exception. at address offset 0xD04, read-write */
   uint32_t icsr;
   /** Vector Table Offset Register. Use the Vector Table Offset Register to determine: if the vector table is in RAM or code memory, the vector table offset. at address offset 0xD08, read-write */
   uint32_t vtor;
   /** Application Interrupt/Reset Control Register. Use the Application Interrupt and Reset Control Register to: determine data endianness, clear all active state information for debug or to recover from a hard failure, execute a system reset, alter the priority grouping position (binary point). at address offset 0xD0C, read-write */
   uint32_t aircr;
   /** System Control Register. Use the System Control Register for power-management functions: signal to the system when the processor can enter a low power state, control how the processor enters and exits low power states. at address offset 0xD10, read-write */
   uint32_t scr;
   /** Configuration Control Register. Use the Configuration Control Register to: enable NMI, HardFault and FAULTMASK to ignore bus fault, trap divide by zero and unaligned accesses, enable user access to the Software Trigger Exception Register, control entry to Thread Mode. at address offset 0xD14, read-write */
   uint32_t ccr;
   /** System Handlers 4-7 Priority Register. Use the three System Handler Priority Registers to prioritize the following system handlers: memory manage, bus fault, usage fault, debug monitor, SVC, SysTick, PendSV. System Handlers are a special class of exception handler that can have their priority set to any of the priority levels. Most can be masked on (enabled) or off (disabled). When disabled, the fault is always treated as a Hard Fault. at address offset 0xD18, read-write */
   uint32_t shpr1;
   /** System Handlers 8-11 Priority Register. Use the three System Handler Priority Registers to prioritize the following system handlers: memory manage, bus fault, usage fault, debug monitor, SVC, SysTick, PendSV. System Handlers are a special class of exception handler that can have their priority set to any of the priority levels. Most can be masked on (enabled) or off (disabled). When disabled, the fault is always treated as a Hard Fault. at address offset 0xD1C, read-write */
   uint32_t shpr2;
   /** System Handlers 12-15 Priority Register. Use the three System Handler Priority Registers to prioritize the following system handlers: memory manage, bus fault, usage fault, debug monitor, SVC, SysTick, PendSV. System Handlers are a special class of exception handler that can have their priority set to any of the priority levels. Most can be masked on (enabled) or off (disabled). When disabled, the fault is always treated as a Hard Fault. at address offset 0xD20, read-write */
   uint32_t shpr3;
   /** System Handler Control and State Register. Use the System Handler Control and State Register to: enable or disable the system handlers, determine the pending status of bus fault, mem manage fault, and SVC, determine the active status of the system handlers. If a fault condition occurs while its fault handler is disabled, the fault escalates to a Hard Fault. at address offset 0xD24, read-write */
   uint32_t shcsr;
   /** Configurable Fault Status Registers. Use the three Configurable Fault Status Registers to obtain information about local faults. These registers include: Memory Manage Fault Status Register (0xE000ED28), Bus Fault Status Register (0xE000ED29), Usage Fault Status Register (0xE000ED2A). The fl gs in these registers indicate the causes of local faults. Multiple flags can be set if more than one fault occurs. These register are read/write-clear. This means that they can be read normally, but writi g a 1 to any bit clears that bit. at address offset 0xD28, read-write */
   uint32_t cfsr;
   /** Hard Fault Status Register. Use the Hard Fault Status Register (HFSR) to obtain information about events that activate the Hard Fault handler. The HFSR is a write-clear register. This means that writing a 1 to a bit clears that bit at address offset 0xD2C, read-write */
   uint32_t hfsr;
   /** Debug Fault Status Register. Use the Debug Fault Status Register to monitor: external debug requests, vector catches, data watchpoint match, BKPT instruction execution, halt requests. Multiple flags in the Debug Fault Status Register can be set when multiple fault conditions occur. The register is read/write clear. This means that it can be read normally. Writing a 1 to a bit clears that bit. Note that these bits are not set unless the event is caught. This means that it causes a stop of some sort. If halting debug is enabled, these events stop the processor into debug. If debug is disabled and the debug monitor is enabled, then this becomes a debug monitor handler call, if priority permits. If debug and the monitor are both disabled, some of these events are Hard Faults, and the DBGEVT bit is set in the Hard Fault status register, and some are ignored. at address offset 0xD30, read-write */
   uint32_t dfsr;
   /** Mem Manage Fault Address Register. Use the Memory Manage Fault Address Register to read the address of the location that caused a Memory Manage Fault. at address offset 0xD34, read-write */
   uint32_t mmfar;
   /** Bus Fault Address Register. Use the Bus Fault Address Register to read the address of the location that generated a Bus Fault. at address offset 0xD38, read-write */
   uint32_t bfar;
   /** Reserved space */
   uint8_t fill9[4];
   /** Processor Feature register0. Processor Feature register0 at address offset 0xD40, read-only */
   uint32_t id_pfr0;
   /** Processor Feature register1. Processor Feature register1 at address offset 0xD44, read-only */
   uint32_t id_pfr1;
   /** Debug Feature register0. This register provides a high level view of the debug system. Further details are provided in the debug infrastructure itself. at address offset 0xD48, read-only */
   uint32_t id_dfr0;
   /** Auxiliary Feature register0. This register provides some freedom for IMPLEMENTATION DEFINED features to be registered against the CPUID. Not used in Cortex-M3. at address offset 0xD4C, read-only */
   uint32_t id_afr0;
   /** Memory Model Feature register0. General information on the memory model and memory management support. at address offset 0xD50, read-only */
   uint32_t id_mmfr0;
   /** Memory Model Feature register1. General information on the memory model and memory management support. at address offset 0xD54, read-only */
   uint32_t id_mmfr1;
   /** Memory Model Feature register2. General information on the memory model and memory management support. at address offset 0xD58, read-only */
   uint32_t id_mmfr2;
   /** Memory Model Feature register3. General information on the memory model and memory management support. at address offset 0xD5C, read-only */
   uint32_t id_mmfr3;
   /** SA Feature register0. Information on the instruction set attributes register at address offset 0xD60, read-only */
   uint32_t id_isar0;
   /** ISA Feature register1. Information on the instruction set attributes register at address offset 0xD64, read-only */
   uint32_t id_isar1;
   /** ISA Feature register2. Information on the instruction set attributes register at address offset 0xD68, read-only */
   uint32_t id_isar2;
   /** ISA Feature register3. Information on the instruction set attributes register at address offset 0xD6C, read-only */
   uint32_t id_isar3;
   /** ISA Feature register4. Information on the instruction set attributes register at address offset 0xD70, read-only */
   uint32_t id_isar4;
   /** Reserved space */
   uint8_t fill10[4];
   /** Cache level ID register at address offset 0xD78, read-only */
   uint32_t clidr;
   /** Cache type register at address offset 0xD7C, read-only */
   uint32_t ctr;
   /** Cache size ID register at address offset 0xD80, read-only */
   uint32_t ccsidr;
   /** Cache size selection at address offset 0xD84, read-write */
   uint32_t csselr;
   /** Coprocessor Access Control Register. The Coprocessor Access Control Register (CPACR) specifies the access privileges for coprocessors. at address offset 0xD88, read-write */
   uint32_t cpacr;
   /** Reserved space */
   uint8_t fill11[4];
   /** MPU Type Register. Use the MPU Type Register to see how many regions the MPU supports. Read bits [15:8] to determine if an MPU is present. at address offset 0xD90, read-only */
   uint32_t mpu_type;
   /** MPU Control Register. Use the MPU Control Register to enable the MPU, enable the default memory map (background region), and enable the MPU when in Hard Fault, Non-maskable Interrupt (NMI), and FAULTMASK escalated handlers. When the MPU is enabled, at least one region of the memory map must be enabled for the MPU to function unless the PRIVDEFENA bit is set. If the PRIVDEFENA bit is set and no regions are enabled, then only privileged code can operate. When the MPU is disabled, the default address map is used, as if no MPU is present. When the MPU is enabled, only the system partition and vector table loads are always accessible. Other areas are accessible based on regions and whether PRIVDEFENA is enabled. Unless HFNMIENA is set, the MPU is not enabled when the exception priority is 1 or 2. These priorities are only possible when in Hard fault, NMI, or when FAULTMASK is enabled. The HFNMIENA bit enables the MPU when operating with these two priorities. at address offset 0xD94, read-write */
   uint32_t mpu_ctrl;
   /** MPU Region Number Register. Use the MPU Region Number Register to select which protection region is accessed. Then write to the MPU Region Base Address Register or the MPU Attributes and Size Register to configure the characteristics of the protection region. at address offset 0xD98, read-write */
   uint32_t mpu_rnr;
   /** MPU Region Base Address Register. Use the MPU Region Base Address Register to write the base address of a region. The Region Base Address Register also contains a REGION field that you can use to override the REGION field in the MPU Region Number Register, if the VALID bit is set. The Region Base Address register sets the base for the region. It is aligned by the size. So, a 64-KB sized region must be aligned on a multiple of 64KB, for example, 0x00010000 or 0x00020000. The region always reads back as the current MPU region number. VALID always reads back as 0. Writing with VALID = 1 and REGION = n changes the region number to n. This is a short-hand way to write the MPU Region Number Register. This register is Unpredictable if accessed other than as a word. at address offset 0xD9C, read-write */
   uint32_t mpu_rbar;
   /** MPU Region Attribute and Size Register. Use the MPU Region Attribute and Size Register to control the MPU access permissions. The register is made up of two part registers, each of halfword size. These can be accessed using the individual size, or they can both be simultaneously accessed using a word operation. The sub-region disable bits are not supported for region sizes of 32 bytes, 64 bytes, and 128 bytes. When these region sizes are used, the subregion disable bits must be programmed as 0. at address offset 0xDA0, read-write */
   uint32_t mpu_rasr;
   /** MPU Alias 1 Region Base Address register. Alias of 0xE000ED9C. at address offset 0xDA4, read-write */
   uint32_t mpu_rbar_a1;
   /** MPU Alias 1 Region Attribute and Size register. Alias of 0xE000EDA0. at address offset 0xDA8, read-write */
   uint32_t mpu_rasr_a1;
   /** MPU Alias 2 Region Base Address register. Alias of 0xE000ED9C. at address offset 0xDAC, read-write */
   uint32_t mpu_rbar_a2;
   /** MPU Alias 2 Region Attribute and Size register. Alias of 0xE000EDA0. at address offset 0xDB0, read-write */
   uint32_t mpu_rasr_a2;
   /** MPU Alias 3 Region Base Address register. Alias of 0xE000ED9C. at address offset 0xDB4, read-write */
   uint32_t mpu_rbar_a3;
   /** MPU Alias 3 Region Attribute and Size register. Alias of 0xE000EDA0. at address offset 0xDB8, read-write */
   uint32_t mpu_rasr_a3;
   /** Reserved space */
   uint8_t fill12[52];
   /** Debug Halting Control and Status Register. The purpose of the Debug Halting Control and Status Register (DHCSR) is to provide status information about the state of the processor, enable core debug, halt and step the processor. For writes, 0xA05F must be written to bits [31:16], otherwise the write operation is ignored and no bits are written into the register. If not enabled for Halting mode, C_DEBUGEN = 1, all other fields are disabled. This register is not reset on a system reset. It is reset by a power-on reset. However, the C_HALT bit always clears on a system reset. To halt on a reset, the following bits must be enabled: bit [0], VC_CORERESET, of the Debug Exception and Monitor Control Register and bit [0],C_DEBUGEN, of the Debug Halting Control and Status Register. Note that writes to this register in any size other than word are Unpredictable. It is acceptable to read in any size, and you can use it to avoid or intentionally change a sticky bit. Bit 16 of DHCSR is Unpredictable on reset at address offset 0xDF0, read-write */
   uint32_t dhcsr;
   /** Deubg Core Register Selector Register. The purpose of the Debug Core Register Selector Register (DCRSR) is to select the processor register to transfer data to or from. This write-only register generates a handshake to the core to transfer data to or from Debug Core Register Data Register and the selected register. Until this core transaction is complete, bit [16], S_REGRDY, of the DHCSR is 0. Note that writes to this register in any size but word are Unpredictable. Note that PSR registers are fully accessible this way, whereas some read as 0 when using MRS instructions. Note that all bits can be written, but some combinations cause a fault when execution is resumed. Note that IT might be written and behaves as though in an IT block. at address offset 0xDF4, write-only */
   uint32_t dcrsr;
   /** Debug Core Register Data Register. The purpose of the Debug Core Register Data Register (DCRDR) is to hold data for reading and writing registers to and from the processor. This is the data value written to the register selected by the Debug Register Selector Register. When the processor receives a request from the Debug Core Register Selector, this register is read or written by the processor using a normal load-store unit operation. If core register transfers are not being performed, software-based debug monitors can use this register for communication in non-halting debug. For example, OS RSD and Real View Monitor. This enables flags and bits to acknowledge state and indicate if commands have been accepted to, replied to, or accepted and replied to. at address offset 0xDF8, read-write */
   uint32_t dcrdr;
   /** Debug Exception and Monitor Control Register. The purpose of the Debug Exception and Monitor Control Register (DEMCR) is Vector catching and Debug monitor control.  This register manages exception behavior under debug. Vector catching is only available to halting debug. The upper halfword is for monitor controls and the lower halfword is for halting exception support. This register is not reset on a system reset. This register is reset by a power-on reset. Bits [19:16] are always cleared on a core reset. The debug monitor is enabled by software in the reset handler or later, or by the AHB-AP port. Vector catching is semi-synchronous. When a matching event is seen, a Halt is requested. Because the processor can only halt on an instruction boundary, it must wait until the next instruction boundary. As a result, it stops on the first instruction of the exception handler. 
       However, two special cases exist when a vector catch has triggered: 1. If a fault is taken during a vector read or stack push error the halt occurs on the corresponding fault handler for the vector error or stack push. 2. If a late arriving interrupt detected during a vector read or stack push error it is not taken. That is, an implementation that supports the late arrival optimization must suppress it in this case. at address offset 0xDFC, read-write */
   uint32_t demcr;
   /** Reserved space */
   uint8_t fill13[256];
   /** Software Trigger Interrupt Register. Use the Software Trigger Interrupt Register to pend an interrupt to trigger. at address offset 0xF00, write-only */
   uint32_t stir;
   /** Reserved space */
   uint8_t fill14[76];
   /** Instruction cache invalidate all to Point of Unificaton at address offset 0xF50, write-only */
   uint32_t iciallu;
   /** Reserved space */
   uint8_t fill15[4];
   /** Instruction cache invalidate by address to PoU at address offset 0xF58, write-only */
   uint32_t icimvau;
   /** Data cache invalidate by address to PoC at address offset 0xF5C, write-only */
   uint32_t dcimvac;
   /** Data cache invalidate by set/way at address offset 0xF60, write-only */
   uint32_t dcisw;
   /** Data cache by address to PoU at address offset 0xF64, write-only */
   uint32_t dccmvau;
   /** Data cache clean by address to PoC at address offset 0xF68, write-only */
   uint32_t dccmvac;
   /** Data cache clean by set/way at address offset 0xF6C, write-only */
   uint32_t dccsw;
   /** Data cache clean and invalidate by address to PoC at address offset 0xF70, write-only */
   uint32_t dccimvac;
   /** Data cache clean and invalidate by set/way at address offset 0xF74, write-only */
   uint32_t dccisw;
   /** Not implemented RAZ/WI at address offset 0xF78, read-only */
   uint32_t bpisll;
   /** Reserved space */
   uint8_t fill16[20];
   /** Instruction TCM control register at address offset 0xF90, read-write */
   uint32_t cm7_itcmcr;
   /** Data TCM control register at address offset 0xF94, read-write */
   uint32_t cm7_dtcmcr;
   /** AHB control register at address offset 0xF98, read-write */
   uint32_t cm7_ahbpcr;
   /** L1 cache control register at address offset 0xF9C, read-write */
   uint32_t cm7_cacr;
   /** AHB slave control register at address offset 0xFA0, read-write */
   uint32_t cm7_ahbscr;
   /** Reserved space */
   uint8_t fill17[4];
   /** Auxiliary bus fault status register at address offset 0xFA8, read-write */
   uint32_t cm7_abfsr;
   /** Reserved space */
   uint8_t fill18[4];
   /** Instruction error bank register at address offset 0xFB0, read-write */
   uint32_t iebr0;
   /** Instruction error bank register at address offset 0xFB4, read-write */
   uint32_t iebr1;
   /** Data error bank register at address offset 0xFB8, read-write */
   uint32_t debr0;
   /** Data error bank register at address offset 0xFBC, read-write */
   uint32_t debr1;
   /** Reserved space */
   uint8_t fill19[16];
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

/** bit field defines for cortex_m7_scs_s#ictr */
#define CORTEX_M7_SCS_ICTR_INTLINESNUM_OFFSET 0
#define CORTEX_M7_SCS_ICTR_INTLINESNUM_SIZE 4

/** bit field defines for cortex_m7_scs_s#actlr */
#define CORTEX_M7_SCS_ACTLR_DISFOLD_OFFSET 2
#define CORTEX_M7_SCS_ACTLR_DISFOLD_SIZE 1
#define CORTEX_M7_SCS_ACTLR_FPEXCODIS_OFFSET 10
#define CORTEX_M7_SCS_ACTLR_FPEXCODIS_SIZE 1
#define CORTEX_M7_SCS_ACTLR_DISRAMODE_OFFSET 11
#define CORTEX_M7_SCS_ACTLR_DISRAMODE_SIZE 1
#define CORTEX_M7_SCS_ACTLR_DISITMATBFLUSH_OFFSET 12
#define CORTEX_M7_SCS_ACTLR_DISITMATBFLUSH_SIZE 1
#define CORTEX_M7_SCS_ACTLR_DISBTACREAD_OFFSET 13
#define CORTEX_M7_SCS_ACTLR_DISBTACREAD_SIZE 1
#define CORTEX_M7_SCS_ACTLR_DISBTACALLOC_OFFSET 14
#define CORTEX_M7_SCS_ACTLR_DISBTACALLOC_SIZE 1
#define CORTEX_M7_SCS_ACTLR_DISCRITAXIRUR_OFFSET 15
#define CORTEX_M7_SCS_ACTLR_DISCRITAXIRUR_SIZE 1
#define CORTEX_M7_SCS_ACTLR_DISDI_OFFSET 16
#define CORTEX_M7_SCS_ACTLR_DISDI_SIZE 5
#define CORTEX_M7_SCS_ACTLR_DISISSCH1_OFFSET 21
#define CORTEX_M7_SCS_ACTLR_DISISSCH1_SIZE 5
#define CORTEX_M7_SCS_ACTLR_DISDYNADD_OFFSET 26
#define CORTEX_M7_SCS_ACTLR_DISDYNADD_SIZE 1
#define CORTEX_M7_SCS_ACTLR_DISCRITAXIRUW_OFFSET 27
#define CORTEX_M7_SCS_ACTLR_DISCRITAXIRUW_SIZE 1
#define CORTEX_M7_SCS_ACTLR_DISFPUISSOPT_OFFSET 28
#define CORTEX_M7_SCS_ACTLR_DISFPUISSOPT_SIZE 1

/** bit field defines for cortex_m7_scs_s#syst_csr */
#define CORTEX_M7_SCS_SYST_CSR_ENABLE_OFFSET 0
#define CORTEX_M7_SCS_SYST_CSR_ENABLE_SIZE 1
#define CORTEX_M7_SCS_SYST_CSR_TICKINT_OFFSET 1
#define CORTEX_M7_SCS_SYST_CSR_TICKINT_SIZE 1
#define CORTEX_M7_SCS_SYST_CSR_CLKSOURCE_OFFSET 2
#define CORTEX_M7_SCS_SYST_CSR_CLKSOURCE_SIZE 1
#define CORTEX_M7_SCS_SYST_CSR_COUNTFLAG_OFFSET 16
#define CORTEX_M7_SCS_SYST_CSR_COUNTFLAG_SIZE 1

/** bit field defines for cortex_m7_scs_s#syst_rvr */
#define CORTEX_M7_SCS_SYST_RVR_RELOAD_OFFSET 0
#define CORTEX_M7_SCS_SYST_RVR_RELOAD_SIZE 24

/** bit field defines for cortex_m7_scs_s#syst_cvr */
#define CORTEX_M7_SCS_SYST_CVR_CURRENT_OFFSET 0
#define CORTEX_M7_SCS_SYST_CVR_CURRENT_SIZE 32

/** bit field defines for cortex_m7_scs_s#syst_calib */
#define CORTEX_M7_SCS_SYST_CALIB_TENMS_OFFSET 0
#define CORTEX_M7_SCS_SYST_CALIB_TENMS_SIZE 24
#define CORTEX_M7_SCS_SYST_CALIB_SKEW_OFFSET 30
#define CORTEX_M7_SCS_SYST_CALIB_SKEW_SIZE 1
#define CORTEX_M7_SCS_SYST_CALIB_NOREF_OFFSET 31
#define CORTEX_M7_SCS_SYST_CALIB_NOREF_SIZE 1

/** bit field defines for cortex_m7_scs_s#nvic_iser */
#define CORTEX_M7_SCS_NVIC_ISER_SETENA_OFFSET 0
#define CORTEX_M7_SCS_NVIC_ISER_SETENA_SIZE 32

/** bit field defines for cortex_m7_scs_s#nvic_icer */
#define CORTEX_M7_SCS_NVIC_ICER_CLRENA_OFFSET 0
#define CORTEX_M7_SCS_NVIC_ICER_CLRENA_SIZE 32

/** bit field defines for cortex_m7_scs_s#nvic_ispr */
#define CORTEX_M7_SCS_NVIC_ISPR_SETPEND_OFFSET 0
#define CORTEX_M7_SCS_NVIC_ISPR_SETPEND_SIZE 32

/** bit field defines for cortex_m7_scs_s#nvic_icpr */
#define CORTEX_M7_SCS_NVIC_ICPR_CLRPEND_OFFSET 0
#define CORTEX_M7_SCS_NVIC_ICPR_CLRPEND_SIZE 32

/** bit field defines for cortex_m7_scs_s#nvic_iabr */
#define CORTEX_M7_SCS_NVIC_IABR_ACTIVE_OFFSET 0
#define CORTEX_M7_SCS_NVIC_IABR_ACTIVE_SIZE 32

/** bit field defines for cortex_m7_scs_s#nvic_ipr */
#define CORTEX_M7_SCS_NVIC_IPR_PRI_0_OFFSET 0
#define CORTEX_M7_SCS_NVIC_IPR_PRI_0_SIZE 8
#define CORTEX_M7_SCS_NVIC_IPR_PRI_1_OFFSET 8
#define CORTEX_M7_SCS_NVIC_IPR_PRI_1_SIZE 8
#define CORTEX_M7_SCS_NVIC_IPR_PRI_2_OFFSET 16
#define CORTEX_M7_SCS_NVIC_IPR_PRI_2_SIZE 8
#define CORTEX_M7_SCS_NVIC_IPR_PRI_3_OFFSET 24
#define CORTEX_M7_SCS_NVIC_IPR_PRI_3_SIZE 8

/** bit field defines for cortex_m7_scs_s#cpuid */
#define CORTEX_M7_SCS_CPUID_REVISION_OFFSET 0
#define CORTEX_M7_SCS_CPUID_REVISION_SIZE 4
#define CORTEX_M7_SCS_CPUID_PARTNO_OFFSET 4
#define CORTEX_M7_SCS_CPUID_PARTNO_SIZE 12
#define CORTEX_M7_SCS_CPUID_ARCHITECTURE_OFFSET 16
#define CORTEX_M7_SCS_CPUID_ARCHITECTURE_SIZE 4
#define CORTEX_M7_SCS_CPUID_VARIANT_OFFSET 20
#define CORTEX_M7_SCS_CPUID_VARIANT_SIZE 4
#define CORTEX_M7_SCS_CPUID_IMPLEMENTER_OFFSET 24
#define CORTEX_M7_SCS_CPUID_IMPLEMENTER_SIZE 8

/** bit field defines for cortex_m7_scs_s#icsr */
#define CORTEX_M7_SCS_ICSR_VECTACTIVE_OFFSET 0
#define CORTEX_M7_SCS_ICSR_VECTACTIVE_SIZE 9
#define CORTEX_M7_SCS_ICSR_RETTOBASE_OFFSET 11
#define CORTEX_M7_SCS_ICSR_RETTOBASE_SIZE 1
#define CORTEX_M7_SCS_ICSR_VECTPENDING_OFFSET 12
#define CORTEX_M7_SCS_ICSR_VECTPENDING_SIZE 6
#define CORTEX_M7_SCS_ICSR_ISRPENDING_OFFSET 22
#define CORTEX_M7_SCS_ICSR_ISRPENDING_SIZE 1
#define CORTEX_M7_SCS_ICSR_ISRPREEMPT_OFFSET 23
#define CORTEX_M7_SCS_ICSR_ISRPREEMPT_SIZE 1
#define CORTEX_M7_SCS_ICSR_PENDSTCLR_OFFSET 25
#define CORTEX_M7_SCS_ICSR_PENDSTCLR_SIZE 1
#define CORTEX_M7_SCS_ICSR_PENDSTSET_OFFSET 26
#define CORTEX_M7_SCS_ICSR_PENDSTSET_SIZE 1
#define CORTEX_M7_SCS_ICSR_PENDSVCLR_OFFSET 27
#define CORTEX_M7_SCS_ICSR_PENDSVCLR_SIZE 1
#define CORTEX_M7_SCS_ICSR_PENDSVSET_OFFSET 28
#define CORTEX_M7_SCS_ICSR_PENDSVSET_SIZE 1
#define CORTEX_M7_SCS_ICSR_NMIPENDSET_OFFSET 31
#define CORTEX_M7_SCS_ICSR_NMIPENDSET_SIZE 1

/** bit field defines for cortex_m7_scs_s#vtor */
#define CORTEX_M7_SCS_VTOR_TBLOFF_OFFSET 7
#define CORTEX_M7_SCS_VTOR_TBLOFF_SIZE 25

/** bit field defines for cortex_m7_scs_s#aircr */
#define CORTEX_M7_SCS_AIRCR_VECTRESET_OFFSET 0
#define CORTEX_M7_SCS_AIRCR_VECTRESET_SIZE 1
#define CORTEX_M7_SCS_AIRCR_VECTCLRACTIVE_OFFSET 1
#define CORTEX_M7_SCS_AIRCR_VECTCLRACTIVE_SIZE 1
#define CORTEX_M7_SCS_AIRCR_SYSRESETREQ_OFFSET 2
#define CORTEX_M7_SCS_AIRCR_SYSRESETREQ_SIZE 1
#define CORTEX_M7_SCS_AIRCR_PRIGROUP_OFFSET 8
#define CORTEX_M7_SCS_AIRCR_PRIGROUP_SIZE 3
#define CORTEX_M7_SCS_AIRCR_ENDIANESS_OFFSET 15
#define CORTEX_M7_SCS_AIRCR_ENDIANESS_SIZE 1
#define CORTEX_M7_SCS_AIRCR_VECTKEY_OFFSET 16
#define CORTEX_M7_SCS_AIRCR_VECTKEY_SIZE 16

/** bit field defines for cortex_m7_scs_s#scr */
#define CORTEX_M7_SCS_SCR_SLEEPONEXIT_OFFSET 1
#define CORTEX_M7_SCS_SCR_SLEEPONEXIT_SIZE 1
#define CORTEX_M7_SCS_SCR_SLEEPDEEP_OFFSET 2
#define CORTEX_M7_SCS_SCR_SLEEPDEEP_SIZE 1
#define CORTEX_M7_SCS_SCR_SEVONPEND_OFFSET 4
#define CORTEX_M7_SCS_SCR_SEVONPEND_SIZE 1

/** bit field defines for cortex_m7_scs_s#ccr */
#define CORTEX_M7_SCS_CCR_NONBASETHREDENA_OFFSET 0
#define CORTEX_M7_SCS_CCR_NONBASETHREDENA_SIZE 1
#define CORTEX_M7_SCS_CCR_USERSETMPEND_OFFSET 1
#define CORTEX_M7_SCS_CCR_USERSETMPEND_SIZE 1
#define CORTEX_M7_SCS_CCR_UNALIGN_TRP_OFFSET 3
#define CORTEX_M7_SCS_CCR_UNALIGN_TRP_SIZE 1
#define CORTEX_M7_SCS_CCR_DIV_0_TRP_OFFSET 4
#define CORTEX_M7_SCS_CCR_DIV_0_TRP_SIZE 1
#define CORTEX_M7_SCS_CCR_BFHFNMIGN_OFFSET 8
#define CORTEX_M7_SCS_CCR_BFHFNMIGN_SIZE 1
#define CORTEX_M7_SCS_CCR_STKALIGN_OFFSET 9
#define CORTEX_M7_SCS_CCR_STKALIGN_SIZE 1

/** bit field defines for cortex_m7_scs_s#shpr1 */
#define CORTEX_M7_SCS_SHPR1_PRI_4_OFFSET 0
#define CORTEX_M7_SCS_SHPR1_PRI_4_SIZE 8
#define CORTEX_M7_SCS_SHPR1_PRI_5_OFFSET 8
#define CORTEX_M7_SCS_SHPR1_PRI_5_SIZE 8
#define CORTEX_M7_SCS_SHPR1_PRI_6_OFFSET 16
#define CORTEX_M7_SCS_SHPR1_PRI_6_SIZE 8
#define CORTEX_M7_SCS_SHPR1_PRI_7_OFFSET 24
#define CORTEX_M7_SCS_SHPR1_PRI_7_SIZE 8

/** bit field defines for cortex_m7_scs_s#shpr2 */
#define CORTEX_M7_SCS_SHPR2_PRI_8_OFFSET 0
#define CORTEX_M7_SCS_SHPR2_PRI_8_SIZE 8
#define CORTEX_M7_SCS_SHPR2_PRI_9_OFFSET 8
#define CORTEX_M7_SCS_SHPR2_PRI_9_SIZE 8
#define CORTEX_M7_SCS_SHPR2_PRI_10_OFFSET 16
#define CORTEX_M7_SCS_SHPR2_PRI_10_SIZE 8
#define CORTEX_M7_SCS_SHPR2_PRI_11_OFFSET 24
#define CORTEX_M7_SCS_SHPR2_PRI_11_SIZE 8

/** bit field defines for cortex_m7_scs_s#shpr3 */
#define CORTEX_M7_SCS_SHPR3_PRI_12_OFFSET 0
#define CORTEX_M7_SCS_SHPR3_PRI_12_SIZE 8
#define CORTEX_M7_SCS_SHPR3_PRI_13_OFFSET 8
#define CORTEX_M7_SCS_SHPR3_PRI_13_SIZE 8
#define CORTEX_M7_SCS_SHPR3_PRI_14_OFFSET 16
#define CORTEX_M7_SCS_SHPR3_PRI_14_SIZE 8
#define CORTEX_M7_SCS_SHPR3_PRI_15_OFFSET 24
#define CORTEX_M7_SCS_SHPR3_PRI_15_SIZE 8

/** bit field defines for cortex_m7_scs_s#shcsr */
#define CORTEX_M7_SCS_SHCSR_MEMFAULTACT_OFFSET 0
#define CORTEX_M7_SCS_SHCSR_MEMFAULTACT_SIZE 1
#define CORTEX_M7_SCS_SHCSR_BUSFAULTACT_OFFSET 1
#define CORTEX_M7_SCS_SHCSR_BUSFAULTACT_SIZE 1
#define CORTEX_M7_SCS_SHCSR_USGFAULTACT_OFFSET 3
#define CORTEX_M7_SCS_SHCSR_USGFAULTACT_SIZE 1
#define CORTEX_M7_SCS_SHCSR_SVCALLACT_OFFSET 7
#define CORTEX_M7_SCS_SHCSR_SVCALLACT_SIZE 1
#define CORTEX_M7_SCS_SHCSR_MONITORACT_OFFSET 8
#define CORTEX_M7_SCS_SHCSR_MONITORACT_SIZE 1
#define CORTEX_M7_SCS_SHCSR_PENDSVACT_OFFSET 10
#define CORTEX_M7_SCS_SHCSR_PENDSVACT_SIZE 1
#define CORTEX_M7_SCS_SHCSR_SYSTICKACT_OFFSET 11
#define CORTEX_M7_SCS_SHCSR_SYSTICKACT_SIZE 1
#define CORTEX_M7_SCS_SHCSR_USGFAULTPENDED_OFFSET 12
#define CORTEX_M7_SCS_SHCSR_USGFAULTPENDED_SIZE 1
#define CORTEX_M7_SCS_SHCSR_MEMFAULTPENDED_OFFSET 13
#define CORTEX_M7_SCS_SHCSR_MEMFAULTPENDED_SIZE 1
#define CORTEX_M7_SCS_SHCSR_BUSFAULTPENDED_OFFSET 14
#define CORTEX_M7_SCS_SHCSR_BUSFAULTPENDED_SIZE 1
#define CORTEX_M7_SCS_SHCSR_SVCALLPENDED_OFFSET 15
#define CORTEX_M7_SCS_SHCSR_SVCALLPENDED_SIZE 1
#define CORTEX_M7_SCS_SHCSR_MEMFAULTENA_OFFSET 16
#define CORTEX_M7_SCS_SHCSR_MEMFAULTENA_SIZE 1
#define CORTEX_M7_SCS_SHCSR_BUSFAULTENA_OFFSET 17
#define CORTEX_M7_SCS_SHCSR_BUSFAULTENA_SIZE 1
#define CORTEX_M7_SCS_SHCSR_USGFAULTENA_OFFSET 18
#define CORTEX_M7_SCS_SHCSR_USGFAULTENA_SIZE 1

/** bit field defines for cortex_m7_scs_s#cfsr */
#define CORTEX_M7_SCS_CFSR_IACCVIOL_OFFSET 0
#define CORTEX_M7_SCS_CFSR_IACCVIOL_SIZE 1
#define CORTEX_M7_SCS_CFSR_DACCVIOL_OFFSET 1
#define CORTEX_M7_SCS_CFSR_DACCVIOL_SIZE 1
#define CORTEX_M7_SCS_CFSR_MUNSTKERR_OFFSET 3
#define CORTEX_M7_SCS_CFSR_MUNSTKERR_SIZE 1
#define CORTEX_M7_SCS_CFSR_MSTKERR_OFFSET 4
#define CORTEX_M7_SCS_CFSR_MSTKERR_SIZE 1
#define CORTEX_M7_SCS_CFSR_MLSPERR_OFFSET 5
#define CORTEX_M7_SCS_CFSR_MLSPERR_SIZE 1
#define CORTEX_M7_SCS_CFSR_MMARVALID_OFFSET 7
#define CORTEX_M7_SCS_CFSR_MMARVALID_SIZE 1
#define CORTEX_M7_SCS_CFSR_IBUSERR_OFFSET 8
#define CORTEX_M7_SCS_CFSR_IBUSERR_SIZE 1
#define CORTEX_M7_SCS_CFSR_PRECISERR_OFFSET 9
#define CORTEX_M7_SCS_CFSR_PRECISERR_SIZE 1
#define CORTEX_M7_SCS_CFSR_IMPRECISERR_OFFSET 10
#define CORTEX_M7_SCS_CFSR_IMPRECISERR_SIZE 1
#define CORTEX_M7_SCS_CFSR_UNSTKERR_OFFSET 11
#define CORTEX_M7_SCS_CFSR_UNSTKERR_SIZE 1
#define CORTEX_M7_SCS_CFSR_STKERR_OFFSET 12
#define CORTEX_M7_SCS_CFSR_STKERR_SIZE 1
#define CORTEX_M7_SCS_CFSR_LSPERR_OFFSET 13
#define CORTEX_M7_SCS_CFSR_LSPERR_SIZE 1
#define CORTEX_M7_SCS_CFSR_BFARVALID_OFFSET 15
#define CORTEX_M7_SCS_CFSR_BFARVALID_SIZE 1
#define CORTEX_M7_SCS_CFSR_UNDEFINSTR_OFFSET 16
#define CORTEX_M7_SCS_CFSR_UNDEFINSTR_SIZE 1
#define CORTEX_M7_SCS_CFSR_INVSTATE_OFFSET 17
#define CORTEX_M7_SCS_CFSR_INVSTATE_SIZE 1
#define CORTEX_M7_SCS_CFSR_INVPC_OFFSET 18
#define CORTEX_M7_SCS_CFSR_INVPC_SIZE 1
#define CORTEX_M7_SCS_CFSR_NOCP_OFFSET 19
#define CORTEX_M7_SCS_CFSR_NOCP_SIZE 1
#define CORTEX_M7_SCS_CFSR_UNALIGNED_OFFSET 24
#define CORTEX_M7_SCS_CFSR_UNALIGNED_SIZE 1
#define CORTEX_M7_SCS_CFSR_DIVBYZERO_OFFSET 25
#define CORTEX_M7_SCS_CFSR_DIVBYZERO_SIZE 1

/** bit field defines for cortex_m7_scs_s#hfsr */
#define CORTEX_M7_SCS_HFSR_VECTTBL_OFFSET 1
#define CORTEX_M7_SCS_HFSR_VECTTBL_SIZE 1
#define CORTEX_M7_SCS_HFSR_FORCED_OFFSET 30
#define CORTEX_M7_SCS_HFSR_FORCED_SIZE 1
#define CORTEX_M7_SCS_HFSR_DEBUGEVT_OFFSET 31
#define CORTEX_M7_SCS_HFSR_DEBUGEVT_SIZE 1

/** bit field defines for cortex_m7_scs_s#dfsr */
#define CORTEX_M7_SCS_DFSR_HALTED_OFFSET 0
#define CORTEX_M7_SCS_DFSR_HALTED_SIZE 1
#define CORTEX_M7_SCS_DFSR_BKPT_OFFSET 1
#define CORTEX_M7_SCS_DFSR_BKPT_SIZE 1
#define CORTEX_M7_SCS_DFSR_DWTTRAP_OFFSET 2
#define CORTEX_M7_SCS_DFSR_DWTTRAP_SIZE 1
#define CORTEX_M7_SCS_DFSR_VCATCH_OFFSET 3
#define CORTEX_M7_SCS_DFSR_VCATCH_SIZE 1
#define CORTEX_M7_SCS_DFSR_EXTERNAL_OFFSET 4
#define CORTEX_M7_SCS_DFSR_EXTERNAL_SIZE 1

/** bit field defines for cortex_m7_scs_s#mmfar */
#define CORTEX_M7_SCS_MMFAR_ADDRESS_OFFSET 0
#define CORTEX_M7_SCS_MMFAR_ADDRESS_SIZE 32

/** bit field defines for cortex_m7_scs_s#bfar */
#define CORTEX_M7_SCS_BFAR_ADDRESS_OFFSET 0
#define CORTEX_M7_SCS_BFAR_ADDRESS_SIZE 32

/** bit field defines for cortex_m7_scs_s#id_pfr0 */
#define CORTEX_M7_SCS_ID_PFR0_STATE0_OFFSET 0
#define CORTEX_M7_SCS_ID_PFR0_STATE0_SIZE 4
#define CORTEX_M7_SCS_ID_PFR0_STATE1_OFFSET 4
#define CORTEX_M7_SCS_ID_PFR0_STATE1_SIZE 4

/** bit field defines for cortex_m7_scs_s#id_pfr1 */
#define CORTEX_M7_SCS_ID_PFR1_MICROCONTROLLER_PROGRAMMERS_MODEL_OFFSET 8
#define CORTEX_M7_SCS_ID_PFR1_MICROCONTROLLER_PROGRAMMERS_MODEL_SIZE 4

/** bit field defines for cortex_m7_scs_s#id_dfr0 */
#define CORTEX_M7_SCS_ID_DFR0_MICROCONTROLLER_DEBUG_MODEL_OFFSET 20
#define CORTEX_M7_SCS_ID_DFR0_MICROCONTROLLER_DEBUG_MODEL_SIZE 4

/** bit field defines for cortex_m7_scs_s#id_afr0 */
#define CORTEX_M7_SCS_ID_AFR0_IMPLEMENTATION_DEFINED0_OFFSET 0
#define CORTEX_M7_SCS_ID_AFR0_IMPLEMENTATION_DEFINED0_SIZE 4
#define CORTEX_M7_SCS_ID_AFR0_IMPLEMENTATION_DEFINED1_OFFSET 4
#define CORTEX_M7_SCS_ID_AFR0_IMPLEMENTATION_DEFINED1_SIZE 4
#define CORTEX_M7_SCS_ID_AFR0_IMPLEMENTATION_DEFINED2_OFFSET 8
#define CORTEX_M7_SCS_ID_AFR0_IMPLEMENTATION_DEFINED2_SIZE 4
#define CORTEX_M7_SCS_ID_AFR0_IMPLEMENTATION_DEFINED3_OFFSET 12
#define CORTEX_M7_SCS_ID_AFR0_IMPLEMENTATION_DEFINED3_SIZE 4

/** bit field defines for cortex_m7_scs_s#id_mmfr0 */
#define CORTEX_M7_SCS_ID_MMFR0_PMSA_SUPPORT_OFFSET 4
#define CORTEX_M7_SCS_ID_MMFR0_PMSA_SUPPORT_SIZE 4
#define CORTEX_M7_SCS_ID_MMFR0_CACHE_COHERENCE_SUPPORT_OFFSET 8
#define CORTEX_M7_SCS_ID_MMFR0_CACHE_COHERENCE_SUPPORT_SIZE 4
#define CORTEX_M7_SCS_ID_MMFR0_OUTER_NON_SHARABLE_SUPPORT_OFFSET 12
#define CORTEX_M7_SCS_ID_MMFR0_OUTER_NON_SHARABLE_SUPPORT_SIZE 4
#define CORTEX_M7_SCS_ID_MMFR0_AUXILIARY_REGISTER_SUPPORT_OFFSET 20
#define CORTEX_M7_SCS_ID_MMFR0_AUXILIARY_REGISTER_SUPPORT_SIZE 4

/** bit field defines for cortex_m7_scs_s#id_mmfr1 */
#define CORTEX_M7_SCS_ID_MMFR1_ID_MMFR1_OFFSET 0
#define CORTEX_M7_SCS_ID_MMFR1_ID_MMFR1_SIZE 32

/** bit field defines for cortex_m7_scs_s#id_mmfr2 */
#define CORTEX_M7_SCS_ID_MMFR2_WAIT_FOR_INTERRUPT_STALLING_OFFSET 24
#define CORTEX_M7_SCS_ID_MMFR2_WAIT_FOR_INTERRUPT_STALLING_SIZE 4

/** bit field defines for cortex_m7_scs_s#id_mmfr3 */
#define CORTEX_M7_SCS_ID_MMFR3_ID_MMFR3_OFFSET 0
#define CORTEX_M7_SCS_ID_MMFR3_ID_MMFR3_SIZE 32

/** bit field defines for cortex_m7_scs_s#id_isar0 */
#define CORTEX_M7_SCS_ID_ISAR0_BITCOUNT_INSTRS_OFFSET 4
#define CORTEX_M7_SCS_ID_ISAR0_BITCOUNT_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR0_BITFIELD_INSTRS_OFFSET 8
#define CORTEX_M7_SCS_ID_ISAR0_BITFIELD_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR0_CMPBRANCH_INSTRS_OFFSET 12
#define CORTEX_M7_SCS_ID_ISAR0_CMPBRANCH_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR0_COPROC_INSTRS_OFFSET 16
#define CORTEX_M7_SCS_ID_ISAR0_COPROC_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR0_DEBUG_INSTRS_OFFSET 20
#define CORTEX_M7_SCS_ID_ISAR0_DEBUG_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR0_DIVIDE_INSTRS_OFFSET 24
#define CORTEX_M7_SCS_ID_ISAR0_DIVIDE_INSTRS_SIZE 4

/** bit field defines for cortex_m7_scs_s#id_isar1 */
#define CORTEX_M7_SCS_ID_ISAR1_EXTEND_INSRS_OFFSET 12
#define CORTEX_M7_SCS_ID_ISAR1_EXTEND_INSRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR1_IFTHEN_INSTRS_OFFSET 16
#define CORTEX_M7_SCS_ID_ISAR1_IFTHEN_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR1_IMMEDIATE_INSTRS_OFFSET 20
#define CORTEX_M7_SCS_ID_ISAR1_IMMEDIATE_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR1_INTERWORK_INSTRS_OFFSET 24
#define CORTEX_M7_SCS_ID_ISAR1_INTERWORK_INSTRS_SIZE 4

/** bit field defines for cortex_m7_scs_s#id_isar2 */
#define CORTEX_M7_SCS_ID_ISAR2_LOADSTORE_INSTRS_OFFSET 0
#define CORTEX_M7_SCS_ID_ISAR2_LOADSTORE_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR2_MEMHINT_INSTRS_OFFSET 4
#define CORTEX_M7_SCS_ID_ISAR2_MEMHINT_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR2_MULTIACCESSINT_INSTRS_OFFSET 8
#define CORTEX_M7_SCS_ID_ISAR2_MULTIACCESSINT_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR2_MULT_INSTRS_OFFSET 12
#define CORTEX_M7_SCS_ID_ISAR2_MULT_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR2_MULTS_INSTRS_OFFSET 16
#define CORTEX_M7_SCS_ID_ISAR2_MULTS_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR2_MULTU_INSTRS_OFFSET 20
#define CORTEX_M7_SCS_ID_ISAR2_MULTU_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR2_REVERSAL_INSTRS_OFFSET 28
#define CORTEX_M7_SCS_ID_ISAR2_REVERSAL_INSTRS_SIZE 4

/** bit field defines for cortex_m7_scs_s#id_isar3 */
#define CORTEX_M7_SCS_ID_ISAR3_SATRUATE_INSTRS_OFFSET 0
#define CORTEX_M7_SCS_ID_ISAR3_SATRUATE_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR3_SIMD_INSTRS_OFFSET 4
#define CORTEX_M7_SCS_ID_ISAR3_SIMD_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR3_SVC_INSTRS_OFFSET 8
#define CORTEX_M7_SCS_ID_ISAR3_SVC_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR3_SYNCPRIM_INSTRS_OFFSET 12
#define CORTEX_M7_SCS_ID_ISAR3_SYNCPRIM_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR3_TABBRANCH_INSTRS_OFFSET 16
#define CORTEX_M7_SCS_ID_ISAR3_TABBRANCH_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR3_THUMBCOPY_INSTRS_OFFSET 20
#define CORTEX_M7_SCS_ID_ISAR3_THUMBCOPY_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR3_TRUENOP_INSTRS_OFFSET 24
#define CORTEX_M7_SCS_ID_ISAR3_TRUENOP_INSTRS_SIZE 4

/** bit field defines for cortex_m7_scs_s#id_isar4 */
#define CORTEX_M7_SCS_ID_ISAR4_UNPRIV_INSTRS_OFFSET 0
#define CORTEX_M7_SCS_ID_ISAR4_UNPRIV_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR4_WITHSHIFTS_INSTRS_OFFSET 4
#define CORTEX_M7_SCS_ID_ISAR4_WITHSHIFTS_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR4_WRITEBACK_INSTRS_OFFSET 8
#define CORTEX_M7_SCS_ID_ISAR4_WRITEBACK_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR4_BARRIER_INSTRS_OFFSET 16
#define CORTEX_M7_SCS_ID_ISAR4_BARRIER_INSTRS_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR4_SYNCPRIM_INSTRS_FRAC_OFFSET 20
#define CORTEX_M7_SCS_ID_ISAR4_SYNCPRIM_INSTRS_FRAC_SIZE 4
#define CORTEX_M7_SCS_ID_ISAR4_PSR_M_INSTRS_OFFSET 24
#define CORTEX_M7_SCS_ID_ISAR4_PSR_M_INSTRS_SIZE 4

/** bit field defines for cortex_m7_scs_s#clidr */
#define CORTEX_M7_SCS_CLIDR_LEVEL1_CACHE_TYPE_OFFSET 0
#define CORTEX_M7_SCS_CLIDR_LEVEL1_CACHE_TYPE_SIZE 3
#define CORTEX_M7_SCS_CLIDR_LOC_OFFSET 24
#define CORTEX_M7_SCS_CLIDR_LOC_SIZE 3
#define CORTEX_M7_SCS_CLIDR_LOUU_OFFSET 27
#define CORTEX_M7_SCS_CLIDR_LOUU_SIZE 3

/** bit field defines for cortex_m7_scs_s#ctr */
#define CORTEX_M7_SCS_CTR_CTR_OFFSET 0
#define CORTEX_M7_SCS_CTR_CTR_SIZE 32

/** bit field defines for cortex_m7_scs_s#ccsidr */
#define CORTEX_M7_SCS_CCSIDR_LINESIZE_OFFSET 0
#define CORTEX_M7_SCS_CCSIDR_LINESIZE_SIZE 3
#define CORTEX_M7_SCS_CCSIDR_ASSOCIATIVITY_OFFSET 3
#define CORTEX_M7_SCS_CCSIDR_ASSOCIATIVITY_SIZE 10
#define CORTEX_M7_SCS_CCSIDR_NUMSETS_OFFSET 13
#define CORTEX_M7_SCS_CCSIDR_NUMSETS_SIZE 15
#define CORTEX_M7_SCS_CCSIDR_WA_OFFSET 28
#define CORTEX_M7_SCS_CCSIDR_WA_SIZE 1
#define CORTEX_M7_SCS_CCSIDR_RA_OFFSET 29
#define CORTEX_M7_SCS_CCSIDR_RA_SIZE 1
#define CORTEX_M7_SCS_CCSIDR_WB_OFFSET 30
#define CORTEX_M7_SCS_CCSIDR_WB_SIZE 1
#define CORTEX_M7_SCS_CCSIDR_WT_OFFSET 31
#define CORTEX_M7_SCS_CCSIDR_WT_SIZE 1

/** bit field defines for cortex_m7_scs_s#csselr */
#define CORTEX_M7_SCS_CSSELR_IND_OFFSET 0
#define CORTEX_M7_SCS_CSSELR_IND_SIZE 1
#define CORTEX_M7_SCS_CSSELR_LEVEL_OFFSET 1
#define CORTEX_M7_SCS_CSSELR_LEVEL_SIZE 3

/** bit field defines for cortex_m7_scs_s#cpacr */
#define CORTEX_M7_SCS_CPACR_CP0_OFFSET 0
#define CORTEX_M7_SCS_CPACR_CP0_SIZE 2
#define CORTEX_M7_SCS_CPACR_CP1_OFFSET 2
#define CORTEX_M7_SCS_CPACR_CP1_SIZE 2
#define CORTEX_M7_SCS_CPACR_CP2_OFFSET 4
#define CORTEX_M7_SCS_CPACR_CP2_SIZE 2
#define CORTEX_M7_SCS_CPACR_CP3_OFFSET 6
#define CORTEX_M7_SCS_CPACR_CP3_SIZE 2
#define CORTEX_M7_SCS_CPACR_CP4_OFFSET 8
#define CORTEX_M7_SCS_CPACR_CP4_SIZE 2
#define CORTEX_M7_SCS_CPACR_CP5_OFFSET 10
#define CORTEX_M7_SCS_CPACR_CP5_SIZE 2
#define CORTEX_M7_SCS_CPACR_CP6_OFFSET 12
#define CORTEX_M7_SCS_CPACR_CP6_SIZE 2
#define CORTEX_M7_SCS_CPACR_CP7_OFFSET 14
#define CORTEX_M7_SCS_CPACR_CP7_SIZE 2
#define CORTEX_M7_SCS_CPACR_CP10_OFFSET 20
#define CORTEX_M7_SCS_CPACR_CP10_SIZE 2
#define CORTEX_M7_SCS_CPACR_CP11_OFFSET 22
#define CORTEX_M7_SCS_CPACR_CP11_SIZE 2

/** bit field defines for cortex_m7_scs_s#mpu_type */
#define CORTEX_M7_SCS_MPU_TYPE_SEPARATE_OFFSET 0
#define CORTEX_M7_SCS_MPU_TYPE_SEPARATE_SIZE 1
#define CORTEX_M7_SCS_MPU_TYPE_DREGION_OFFSET 8
#define CORTEX_M7_SCS_MPU_TYPE_DREGION_SIZE 8
#define CORTEX_M7_SCS_MPU_TYPE_IREGION_OFFSET 16
#define CORTEX_M7_SCS_MPU_TYPE_IREGION_SIZE 8

/** bit field defines for cortex_m7_scs_s#mpu_ctrl */
#define CORTEX_M7_SCS_MPU_CTRL_ENABLE_OFFSET 0
#define CORTEX_M7_SCS_MPU_CTRL_ENABLE_SIZE 1
#define CORTEX_M7_SCS_MPU_CTRL_HFNMIENA_OFFSET 1
#define CORTEX_M7_SCS_MPU_CTRL_HFNMIENA_SIZE 1
#define CORTEX_M7_SCS_MPU_CTRL_PRIVDEFENA_OFFSET 2
#define CORTEX_M7_SCS_MPU_CTRL_PRIVDEFENA_SIZE 1

/** bit field defines for cortex_m7_scs_s#mpu_rnr */
#define CORTEX_M7_SCS_MPU_RNR_REGION_OFFSET 0
#define CORTEX_M7_SCS_MPU_RNR_REGION_SIZE 8

/** bit field defines for cortex_m7_scs_s#mpu_rbar */
#define CORTEX_M7_SCS_MPU_RBAR_REGION_OFFSET 0
#define CORTEX_M7_SCS_MPU_RBAR_REGION_SIZE 4
#define CORTEX_M7_SCS_MPU_RBAR_VALID_OFFSET 4
#define CORTEX_M7_SCS_MPU_RBAR_VALID_SIZE 1
#define CORTEX_M7_SCS_MPU_RBAR_ADDR_OFFSET 5
#define CORTEX_M7_SCS_MPU_RBAR_ADDR_SIZE 27

/** bit field defines for cortex_m7_scs_s#mpu_rasr */
#define CORTEX_M7_SCS_MPU_RASR_ENABLE_OFFSET 0
#define CORTEX_M7_SCS_MPU_RASR_ENABLE_SIZE 1
#define CORTEX_M7_SCS_MPU_RASR_SIZE_OFFSET 1
#define CORTEX_M7_SCS_MPU_RASR_SIZE_SIZE 5
#define CORTEX_M7_SCS_MPU_RASR_SRD_OFFSET 8
#define CORTEX_M7_SCS_MPU_RASR_SRD_SIZE 8
#define CORTEX_M7_SCS_MPU_RASR_B_OFFSET 16
#define CORTEX_M7_SCS_MPU_RASR_B_SIZE 1
#define CORTEX_M7_SCS_MPU_RASR_C_OFFSET 17
#define CORTEX_M7_SCS_MPU_RASR_C_SIZE 1
#define CORTEX_M7_SCS_MPU_RASR_S_OFFSET 18
#define CORTEX_M7_SCS_MPU_RASR_S_SIZE 1
#define CORTEX_M7_SCS_MPU_RASR_TEX_OFFSET 19
#define CORTEX_M7_SCS_MPU_RASR_TEX_SIZE 3
#define CORTEX_M7_SCS_MPU_RASR_AP_OFFSET 24
#define CORTEX_M7_SCS_MPU_RASR_AP_SIZE 3
#define CORTEX_M7_SCS_MPU_RASR_XN_OFFSET 28
#define CORTEX_M7_SCS_MPU_RASR_XN_SIZE 1

/** bit field defines for cortex_m7_scs_s#mpu_rbar_a1 */
#define CORTEX_M7_SCS_MPU_RBAR_A1_MPU_RBAR_A1_OFFSET 0
#define CORTEX_M7_SCS_MPU_RBAR_A1_MPU_RBAR_A1_SIZE 32

/** bit field defines for cortex_m7_scs_s#mpu_rasr_a1 */
#define CORTEX_M7_SCS_MPU_RASR_A1_MPU_RASR_A1_OFFSET 0
#define CORTEX_M7_SCS_MPU_RASR_A1_MPU_RASR_A1_SIZE 32

/** bit field defines for cortex_m7_scs_s#mpu_rbar_a2 */
#define CORTEX_M7_SCS_MPU_RBAR_A2_MPU_RBAR_A2_OFFSET 0
#define CORTEX_M7_SCS_MPU_RBAR_A2_MPU_RBAR_A2_SIZE 32

/** bit field defines for cortex_m7_scs_s#mpu_rasr_a2 */
#define CORTEX_M7_SCS_MPU_RASR_A2_MPU_RASR_A2_OFFSET 0
#define CORTEX_M7_SCS_MPU_RASR_A2_MPU_RASR_A2_SIZE 32

/** bit field defines for cortex_m7_scs_s#mpu_rbar_a3 */
#define CORTEX_M7_SCS_MPU_RBAR_A3_MPU_RBAR_A3_OFFSET 0
#define CORTEX_M7_SCS_MPU_RBAR_A3_MPU_RBAR_A3_SIZE 32

/** bit field defines for cortex_m7_scs_s#mpu_rasr_a3 */
#define CORTEX_M7_SCS_MPU_RASR_A3_MPU_RASR_A3_OFFSET 0
#define CORTEX_M7_SCS_MPU_RASR_A3_MPU_RASR_A3_SIZE 32

/** bit field defines for cortex_m7_scs_s#dhcsr */
#define CORTEX_M7_SCS_DHCSR_C_DEBUGEN_OFFSET 0
#define CORTEX_M7_SCS_DHCSR_C_DEBUGEN_SIZE 1
#define CORTEX_M7_SCS_DHCSR_C_HALT_OFFSET 1
#define CORTEX_M7_SCS_DHCSR_C_HALT_SIZE 1
#define CORTEX_M7_SCS_DHCSR_C_STEP_OFFSET 2
#define CORTEX_M7_SCS_DHCSR_C_STEP_SIZE 1
#define CORTEX_M7_SCS_DHCSR_C_MASKINTS_OFFSET 3
#define CORTEX_M7_SCS_DHCSR_C_MASKINTS_SIZE 1
#define CORTEX_M7_SCS_DHCSR_C_SNAPSTALL_OFFSET 5
#define CORTEX_M7_SCS_DHCSR_C_SNAPSTALL_SIZE 1
#define CORTEX_M7_SCS_DHCSR_S_REGRDY_OFFSET 16
#define CORTEX_M7_SCS_DHCSR_S_REGRDY_SIZE 1
#define CORTEX_M7_SCS_DHCSR_S_HALT_OFFSET 17
#define CORTEX_M7_SCS_DHCSR_S_HALT_SIZE 1
#define CORTEX_M7_SCS_DHCSR_S_SLEEP_OFFSET 18
#define CORTEX_M7_SCS_DHCSR_S_SLEEP_SIZE 1
#define CORTEX_M7_SCS_DHCSR_S_LOCKUP_OFFSET 19
#define CORTEX_M7_SCS_DHCSR_S_LOCKUP_SIZE 1
#define CORTEX_M7_SCS_DHCSR_DEBUG_KEY0_OFFSET 20
#define CORTEX_M7_SCS_DHCSR_DEBUG_KEY0_SIZE 4
#define CORTEX_M7_SCS_DHCSR_S_RETIRE_ST_OFFSET 24
#define CORTEX_M7_SCS_DHCSR_S_RETIRE_ST_SIZE 1
#define CORTEX_M7_SCS_DHCSR_S_RESET_ST_OFFSET 25
#define CORTEX_M7_SCS_DHCSR_S_RESET_ST_SIZE 1
#define CORTEX_M7_SCS_DHCSR_DEBUG_KEY1_OFFSET 26
#define CORTEX_M7_SCS_DHCSR_DEBUG_KEY1_SIZE 6

/** bit field defines for cortex_m7_scs_s#dcrsr */
#define CORTEX_M7_SCS_DCRSR_REGSEL_OFFSET 0
#define CORTEX_M7_SCS_DCRSR_REGSEL_SIZE 7
#define CORTEX_M7_SCS_DCRSR_REGWNR_OFFSET 16
#define CORTEX_M7_SCS_DCRSR_REGWNR_SIZE 1

/** bit field defines for cortex_m7_scs_s#dcrdr */
#define CORTEX_M7_SCS_DCRDR_DCRDR_OFFSET 0
#define CORTEX_M7_SCS_DCRDR_DCRDR_SIZE 32

/** bit field defines for cortex_m7_scs_s#demcr */
#define CORTEX_M7_SCS_DEMCR_VC_CORERESET_OFFSET 0
#define CORTEX_M7_SCS_DEMCR_VC_CORERESET_SIZE 1
#define CORTEX_M7_SCS_DEMCR_VC_MMERR_OFFSET 4
#define CORTEX_M7_SCS_DEMCR_VC_MMERR_SIZE 1
#define CORTEX_M7_SCS_DEMCR_VC_NOCPERR_OFFSET 5
#define CORTEX_M7_SCS_DEMCR_VC_NOCPERR_SIZE 1
#define CORTEX_M7_SCS_DEMCR_VC_CHKERR_OFFSET 6
#define CORTEX_M7_SCS_DEMCR_VC_CHKERR_SIZE 1
#define CORTEX_M7_SCS_DEMCR_VC_STATERR_OFFSET 7
#define CORTEX_M7_SCS_DEMCR_VC_STATERR_SIZE 1
#define CORTEX_M7_SCS_DEMCR_VC_BUSERR_OFFSET 8
#define CORTEX_M7_SCS_DEMCR_VC_BUSERR_SIZE 1
#define CORTEX_M7_SCS_DEMCR_VC_INTERR_OFFSET 9
#define CORTEX_M7_SCS_DEMCR_VC_INTERR_SIZE 1
#define CORTEX_M7_SCS_DEMCR_VC_HARDERR_OFFSET 10
#define CORTEX_M7_SCS_DEMCR_VC_HARDERR_SIZE 1
#define CORTEX_M7_SCS_DEMCR_MON_EN_OFFSET 16
#define CORTEX_M7_SCS_DEMCR_MON_EN_SIZE 1
#define CORTEX_M7_SCS_DEMCR_MON_PEND_OFFSET 17
#define CORTEX_M7_SCS_DEMCR_MON_PEND_SIZE 1
#define CORTEX_M7_SCS_DEMCR_MON_STEP_OFFSET 18
#define CORTEX_M7_SCS_DEMCR_MON_STEP_SIZE 1
#define CORTEX_M7_SCS_DEMCR_MON_REQ_OFFSET 19
#define CORTEX_M7_SCS_DEMCR_MON_REQ_SIZE 1
#define CORTEX_M7_SCS_DEMCR_TRCENA_OFFSET 24
#define CORTEX_M7_SCS_DEMCR_TRCENA_SIZE 1

/** bit field defines for cortex_m7_scs_s#stir */
#define CORTEX_M7_SCS_STIR_INTID_OFFSET 0
#define CORTEX_M7_SCS_STIR_INTID_SIZE 9

/** bit field defines for cortex_m7_scs_s#iciallu */
#define CORTEX_M7_SCS_ICIALLU_ICIALLU_OFFSET 0
#define CORTEX_M7_SCS_ICIALLU_ICIALLU_SIZE 32

/** bit field defines for cortex_m7_scs_s#icimvau */
#define CORTEX_M7_SCS_ICIMVAU_MVA_OFFSET 5
#define CORTEX_M7_SCS_ICIMVAU_MVA_SIZE 27

/** bit field defines for cortex_m7_scs_s#dcimvac */
#define CORTEX_M7_SCS_DCIMVAC_MVA_OFFSET 5
#define CORTEX_M7_SCS_DCIMVAC_MVA_SIZE 27

/** bit field defines for cortex_m7_scs_s#dcisw */
#define CORTEX_M7_SCS_DCISW_SET_OFFSET 5
#define CORTEX_M7_SCS_DCISW_SET_SIZE 9
#define CORTEX_M7_SCS_DCISW_WAY_OFFSET 30
#define CORTEX_M7_SCS_DCISW_WAY_SIZE 2

/** bit field defines for cortex_m7_scs_s#dccmvau */
#define CORTEX_M7_SCS_DCCMVAU_MVA_OFFSET 5
#define CORTEX_M7_SCS_DCCMVAU_MVA_SIZE 27

/** bit field defines for cortex_m7_scs_s#dccmvac */
#define CORTEX_M7_SCS_DCCMVAC_MVA_OFFSET 5
#define CORTEX_M7_SCS_DCCMVAC_MVA_SIZE 27

/** bit field defines for cortex_m7_scs_s#dccsw */
#define CORTEX_M7_SCS_DCCSW_SET_OFFSET 5
#define CORTEX_M7_SCS_DCCSW_SET_SIZE 9
#define CORTEX_M7_SCS_DCCSW_WAY_OFFSET 30
#define CORTEX_M7_SCS_DCCSW_WAY_SIZE 2

/** bit field defines for cortex_m7_scs_s#dccimvac */
#define CORTEX_M7_SCS_DCCIMVAC_MVA_OFFSET 5
#define CORTEX_M7_SCS_DCCIMVAC_MVA_SIZE 27

/** bit field defines for cortex_m7_scs_s#dccisw */
#define CORTEX_M7_SCS_DCCISW_SET_OFFSET 5
#define CORTEX_M7_SCS_DCCISW_SET_SIZE 9
#define CORTEX_M7_SCS_DCCISW_WAY_OFFSET 30
#define CORTEX_M7_SCS_DCCISW_WAY_SIZE 2

/** bit field defines for cortex_m7_scs_s#bpisll */
#define CORTEX_M7_SCS_BPISLL_BPISLL_OFFSET 0
#define CORTEX_M7_SCS_BPISLL_BPISLL_SIZE 32

/** bit field defines for cortex_m7_scs_s#cm7_itcmcr */
#define CORTEX_M7_SCS_CM7_ITCMCR_EN_OFFSET 0
#define CORTEX_M7_SCS_CM7_ITCMCR_EN_SIZE 1
#define CORTEX_M7_SCS_CM7_ITCMCR_RMW_OFFSET 1
#define CORTEX_M7_SCS_CM7_ITCMCR_RMW_SIZE 1
#define CORTEX_M7_SCS_CM7_ITCMCR_RETEN_OFFSET 2
#define CORTEX_M7_SCS_CM7_ITCMCR_RETEN_SIZE 1
#define CORTEX_M7_SCS_CM7_ITCMCR_SZ_OFFSET 3
#define CORTEX_M7_SCS_CM7_ITCMCR_SZ_SIZE 4

/** bit field defines for cortex_m7_scs_s#cm7_dtcmcr */
#define CORTEX_M7_SCS_CM7_DTCMCR_EN_OFFSET 0
#define CORTEX_M7_SCS_CM7_DTCMCR_EN_SIZE 1
#define CORTEX_M7_SCS_CM7_DTCMCR_RMW_OFFSET 1
#define CORTEX_M7_SCS_CM7_DTCMCR_RMW_SIZE 1
#define CORTEX_M7_SCS_CM7_DTCMCR_RETEN_OFFSET 2
#define CORTEX_M7_SCS_CM7_DTCMCR_RETEN_SIZE 1
#define CORTEX_M7_SCS_CM7_DTCMCR_SZ_OFFSET 3
#define CORTEX_M7_SCS_CM7_DTCMCR_SZ_SIZE 4

/** bit field defines for cortex_m7_scs_s#cm7_ahbpcr */
#define CORTEX_M7_SCS_CM7_AHBPCR_EN_OFFSET 0
#define CORTEX_M7_SCS_CM7_AHBPCR_EN_SIZE 1
#define CORTEX_M7_SCS_CM7_AHBPCR_SZ_OFFSET 1
#define CORTEX_M7_SCS_CM7_AHBPCR_SZ_SIZE 3

/** bit field defines for cortex_m7_scs_s#cm7_cacr */
#define CORTEX_M7_SCS_CM7_CACR_SIWT_OFFSET 0
#define CORTEX_M7_SCS_CM7_CACR_SIWT_SIZE 1
#define CORTEX_M7_SCS_CM7_CACR_ECCDIS_OFFSET 1
#define CORTEX_M7_SCS_CM7_CACR_ECCDIS_SIZE 1
#define CORTEX_M7_SCS_CM7_CACR_FORCEWT_OFFSET 2
#define CORTEX_M7_SCS_CM7_CACR_FORCEWT_SIZE 1

/** bit field defines for cortex_m7_scs_s#cm7_ahbscr */
#define CORTEX_M7_SCS_CM7_AHBSCR_CTL_OFFSET 0
#define CORTEX_M7_SCS_CM7_AHBSCR_CTL_SIZE 2
#define CORTEX_M7_SCS_CM7_AHBSCR_TPRI_OFFSET 2
#define CORTEX_M7_SCS_CM7_AHBSCR_TPRI_SIZE 9
#define CORTEX_M7_SCS_CM7_AHBSCR_INITCOUNT_OFFSET 11
#define CORTEX_M7_SCS_CM7_AHBSCR_INITCOUNT_SIZE 5

/** bit field defines for cortex_m7_scs_s#cm7_abfsr */
#define CORTEX_M7_SCS_CM7_ABFSR_ITCM_OFFSET 0
#define CORTEX_M7_SCS_CM7_ABFSR_ITCM_SIZE 1
#define CORTEX_M7_SCS_CM7_ABFSR_DTCM_OFFSET 1
#define CORTEX_M7_SCS_CM7_ABFSR_DTCM_SIZE 1
#define CORTEX_M7_SCS_CM7_ABFSR_AHBP_OFFSET 2
#define CORTEX_M7_SCS_CM7_ABFSR_AHBP_SIZE 1
#define CORTEX_M7_SCS_CM7_ABFSR_AXIM_OFFSET 3
#define CORTEX_M7_SCS_CM7_ABFSR_AXIM_SIZE 1
#define CORTEX_M7_SCS_CM7_ABFSR_EPPB_OFFSET 4
#define CORTEX_M7_SCS_CM7_ABFSR_EPPB_SIZE 1
#define CORTEX_M7_SCS_CM7_ABFSR_AXIMTYPE_OFFSET 8
#define CORTEX_M7_SCS_CM7_ABFSR_AXIMTYPE_SIZE 2

/** bit field defines for cortex_m7_scs_s#iebr0 */
#define CORTEX_M7_SCS_IEBR0_VALID_OFFSET 0
#define CORTEX_M7_SCS_IEBR0_VALID_SIZE 1
#define CORTEX_M7_SCS_IEBR0_LOCKED_OFFSET 1
#define CORTEX_M7_SCS_IEBR0_LOCKED_SIZE 1
#define CORTEX_M7_SCS_IEBR0_RAM_LOCATION_OFFSET 2
#define CORTEX_M7_SCS_IEBR0_RAM_LOCATION_SIZE 14
#define CORTEX_M7_SCS_IEBR0_RAM_BANK_OFFSET 16
#define CORTEX_M7_SCS_IEBR0_RAM_BANK_SIZE 1
#define CORTEX_M7_SCS_IEBR0_TYPE_OF_ERROR_OFFSET 17
#define CORTEX_M7_SCS_IEBR0_TYPE_OF_ERROR_SIZE 1
#define CORTEX_M7_SCS_IEBR0_USER_DEFINED_OFFSET 30
#define CORTEX_M7_SCS_IEBR0_USER_DEFINED_SIZE 2

/** bit field defines for cortex_m7_scs_s#iebr1 */
#define CORTEX_M7_SCS_IEBR1_VALID_OFFSET 0
#define CORTEX_M7_SCS_IEBR1_VALID_SIZE 1
#define CORTEX_M7_SCS_IEBR1_LOCKED_OFFSET 1
#define CORTEX_M7_SCS_IEBR1_LOCKED_SIZE 1
#define CORTEX_M7_SCS_IEBR1_RAM_LOCATION_OFFSET 2
#define CORTEX_M7_SCS_IEBR1_RAM_LOCATION_SIZE 14
#define CORTEX_M7_SCS_IEBR1_RAM_BANK_OFFSET 16
#define CORTEX_M7_SCS_IEBR1_RAM_BANK_SIZE 1
#define CORTEX_M7_SCS_IEBR1_TYPE_OF_ERROR_OFFSET 17
#define CORTEX_M7_SCS_IEBR1_TYPE_OF_ERROR_SIZE 1
#define CORTEX_M7_SCS_IEBR1_USER_DEFINED_OFFSET 30
#define CORTEX_M7_SCS_IEBR1_USER_DEFINED_SIZE 2

/** bit field defines for cortex_m7_scs_s#debr0 */
#define CORTEX_M7_SCS_DEBR0_VALID_OFFSET 0
#define CORTEX_M7_SCS_DEBR0_VALID_SIZE 1
#define CORTEX_M7_SCS_DEBR0_LOCKED_OFFSET 1
#define CORTEX_M7_SCS_DEBR0_LOCKED_SIZE 1
#define CORTEX_M7_SCS_DEBR0_RAM_LOCATION_OFFSET 2
#define CORTEX_M7_SCS_DEBR0_RAM_LOCATION_SIZE 14
#define CORTEX_M7_SCS_DEBR0_RAM_BANK_OFFSET 16
#define CORTEX_M7_SCS_DEBR0_RAM_BANK_SIZE 1
#define CORTEX_M7_SCS_DEBR0_TYPE_OF_ERROR_OFFSET 17
#define CORTEX_M7_SCS_DEBR0_TYPE_OF_ERROR_SIZE 1
#define CORTEX_M7_SCS_DEBR0_USER_DEFINED_OFFSET 30
#define CORTEX_M7_SCS_DEBR0_USER_DEFINED_SIZE 2

/** bit field defines for cortex_m7_scs_s#debr1 */
#define CORTEX_M7_SCS_DEBR1_VALID_OFFSET 0
#define CORTEX_M7_SCS_DEBR1_VALID_SIZE 1
#define CORTEX_M7_SCS_DEBR1_LOCKED_OFFSET 1
#define CORTEX_M7_SCS_DEBR1_LOCKED_SIZE 1
#define CORTEX_M7_SCS_DEBR1_RAM_LOCATION_OFFSET 2
#define CORTEX_M7_SCS_DEBR1_RAM_LOCATION_SIZE 14
#define CORTEX_M7_SCS_DEBR1_RAM_BANK_OFFSET 16
#define CORTEX_M7_SCS_DEBR1_RAM_BANK_SIZE 1
#define CORTEX_M7_SCS_DEBR1_TYPE_OF_ERROR_OFFSET 17
#define CORTEX_M7_SCS_DEBR1_TYPE_OF_ERROR_SIZE 1
#define CORTEX_M7_SCS_DEBR1_USER_DEFINED_OFFSET 30
#define CORTEX_M7_SCS_DEBR1_USER_DEFINED_SIZE 2

/** bit field defines for cortex_m7_scs_s#pid4 */
#define CORTEX_M7_SCS_PID4_PID4_OFFSET 0
#define CORTEX_M7_SCS_PID4_PID4_SIZE 32

/** bit field defines for cortex_m7_scs_s#pid5 */
#define CORTEX_M7_SCS_PID5_PID5_OFFSET 0
#define CORTEX_M7_SCS_PID5_PID5_SIZE 32

/** bit field defines for cortex_m7_scs_s#pid6 */
#define CORTEX_M7_SCS_PID6_PID6_OFFSET 0
#define CORTEX_M7_SCS_PID6_PID6_SIZE 32

/** bit field defines for cortex_m7_scs_s#pid7 */
#define CORTEX_M7_SCS_PID7_PID7_OFFSET 0
#define CORTEX_M7_SCS_PID7_PID7_SIZE 32

/** bit field defines for cortex_m7_scs_s#pid0 */
#define CORTEX_M7_SCS_PID0_PID0_OFFSET 0
#define CORTEX_M7_SCS_PID0_PID0_SIZE 32

/** bit field defines for cortex_m7_scs_s#pid1 */
#define CORTEX_M7_SCS_PID1_PID1_OFFSET 0
#define CORTEX_M7_SCS_PID1_PID1_SIZE 32

/** bit field defines for cortex_m7_scs_s#pid2 */
#define CORTEX_M7_SCS_PID2_PID2_OFFSET 0
#define CORTEX_M7_SCS_PID2_PID2_SIZE 32

/** bit field defines for cortex_m7_scs_s#pid3 */
#define CORTEX_M7_SCS_PID3_PID3_OFFSET 0
#define CORTEX_M7_SCS_PID3_PID3_SIZE 32

/** bit field defines for cortex_m7_scs_s#cid0 */
#define CORTEX_M7_SCS_CID0_CID0_OFFSET 0
#define CORTEX_M7_SCS_CID0_CID0_SIZE 32

/** bit field defines for cortex_m7_scs_s#cid1 */
#define CORTEX_M7_SCS_CID1_CID1_OFFSET 0
#define CORTEX_M7_SCS_CID1_CID1_SIZE 32

/** bit field defines for cortex_m7_scs_s#cid2 */
#define CORTEX_M7_SCS_CID2_CID2_OFFSET 0
#define CORTEX_M7_SCS_CID2_CID2_SIZE 32

/** bit field defines for cortex_m7_scs_s#cid3 */
#define CORTEX_M7_SCS_CID3_CID3_OFFSET 0
#define CORTEX_M7_SCS_CID3_CID3_SIZE 32

/* EOF cortex_m7_scs.h */
#endif
