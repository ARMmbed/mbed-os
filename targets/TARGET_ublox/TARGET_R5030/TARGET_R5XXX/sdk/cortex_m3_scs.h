#ifndef INCLUDED_CORTEX_M3_SCS
#define INCLUDED_CORTEX_M3_SCS
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>


/** System  control, MPU, debug and NVIC registers
*/
struct cortex_m3_scs_s {
   /** Reserved space */
   uint8_t fill0[4];
   /** Interrupt Control Type Register. Read the Interrupt Controller Type Register to see the number of interrupt lines that the NVIC supports. at address offset 0x004, read-only */
   uint32_t ictr;
   /** Auxiliary Control Register. Use the Auxiliary Control Register to disable certain aspects of functionality within the processor at address offset 0x008, read-write */
   uint32_t actlr;
   /** Reserved space */
   uint8_t fill1[4];
   /** SysTick Control and Status Register. Use the SysTick Control and Status Register to enable the SysTick features. at address offset 0x010, read-write */
   uint32_t stcsr;
   /** SysTick Reload Value Register. Use the SysTick Reload Value Register to specify the start value to load into the current value register when the counter reaches 0. It can be any value between 1 and 0x00FFFFFF. A start value of 0 is possible, but has no effect because the SysTick interrupt and COUNTFLAG are activated when counting from 1 to 0. at address offset 0x014, read-write */
   uint32_t strvr;
   /** SysTick Current Value Register. Use the SysTick Current Value Register to find the current value in the register. at address offset 0x018, read-write */
   uint32_t stcvr;
   /** SysTick Calibration Value Register. Use the SysTick Calibration Value Register to enable software to scale to any required speed using divide and multiply. at address offset 0x01C, read-only */
   uint32_t stcr;
   /** Reserved space */
   uint8_t fill2[224];
   /** Irq 0 to 31 Set Enable Register. Use the Interrupt Set-Enable Registers to enable interrupts and determine which interrupts are currently enabled. at address offset 0x100, read-write */
   uint32_t nvic_iser0;
   /** Irq 32 to 63 Set Enable Register. Use the Interrupt Set-Enable Registers to enable interrupts and determine which interrupts are currently enabled. at address offset 0x104, read-write */
   uint32_t nvic_iser1;
   /** Irq 64 to 95 Set Enable Register. Use the Interrupt Set-Enable Registers to enable interrupts and determine which interrupts are currently enabled. at address offset 0x108, read-write */
   uint32_t nvic_iser2;
   /** Irq 96 to 127 Set Enable Register. Use the Interrupt Set-Enable Registers to enable interrupts and determine which interrupts are currently enabled. at address offset 0x10C, read-write */
   uint32_t nvic_iser3;
   /** Irq 128 to 159 Set Enable Register. Use the Interrupt Set-Enable Registers to enable interrupts and determine which interrupts are currently enabled. at address offset 0x110, read-write */
   uint32_t nvic_iser4;
   /** Irq 160 to 191 Set Enable Register. Use the Interrupt Set-Enable Registers to enable interrupts and determine which interrupts are currently enabled. at address offset 0x114, read-write */
   uint32_t nvic_iser5;
   /** Irq 192 to 223 Set Enable Register. Use the Interrupt Set-Enable Registers to enable interrupts and determine which interrupts are currently enabled. at address offset 0x118, read-write */
   uint32_t nvic_iser6;
   /** Irq 224 to 239 Set Enable Register. Use the Interrupt Set-Enable Registers to enable interrupts and determine which interrupts are currently enabled. at address offset 0x11C, read-write */
   uint32_t nvic_iser7;
   /** Reserved space */
   uint8_t fill3[96];
   /** Irq 0 to 31 Clear Enable Register. Use the Interrupt Clear-Enable Registers to disable interrupts and determine which interrupts are currently enabled. at address offset 0x180, read-write */
   uint32_t nvic_icer0;
   /** Irq 32 to 63 Clear Enable Register. Use the Interrupt Clear-Enable Registers to disable interrupts and determine which interrupts are currently enabled. at address offset 0x184, read-write */
   uint32_t nvic_icer1;
   /** Irq 64 to 95 Clear Enable Register. Use the Interrupt Clear-Enable Registers to disable interrupts and determine which interrupts are currently enabled. at address offset 0x188, read-write */
   uint32_t nvic_icer2;
   /** Irq 96 to 127 Clear Enable Register. Use the Interrupt Clear-Enable Registers to disable interrupts and determine which interrupts are currently enabled. at address offset 0x18C, read-write */
   uint32_t nvic_icer3;
   /** Irq 128 to 159 Clear Enable Register. Use the Interrupt Clear-Enable Registers to disable interrupts and determine which interrupts are currently enabled. at address offset 0x190, read-write */
   uint32_t nvic_icer4;
   /** Irq 160 to 191 Clear Enable Register. Use the Interrupt Clear-Enable Registers to disable interrupts and determine which interrupts are currently enabled. at address offset 0x194, read-write */
   uint32_t nvic_icer5;
   /** Irq 192 to 223 Clear Enable Register. Use the Interrupt Clear-Enable Registers to disable interrupts and determine which interrupts are currently enabled. at address offset 0x198, read-write */
   uint32_t nvic_icer6;
   /** Irq 224 to 239 Clear Enable Register. Use the Interrupt Clear-Enable Registers to disable interrupts and determine which interrupts are currently enabled. at address offset 0x19C, read-write */
   uint32_t nvic_icer7;
   /** Reserved space */
   uint8_t fill4[96];
   /** Irq 0 to 31 Set Pending Register. Use the Interrupt Set-Pending Registers to force interrupts into the pending state and determine which interrupts are currently pending at address offset 0x200, read-write */
   uint32_t nvic_ispr0;
   /** Irq 32 to 63 Set Pending Register. Use the Interrupt Set-Pending Registers to force interrupts into the pending state and determine which interrupts are currently pending at address offset 0x204, read-write */
   uint32_t nvic_ispr1;
   /** Irq 64 to 95 Set Pending Register. Use the Interrupt Set-Pending Registers to force interrupts into the pending state and determine which interrupts are currently pending at address offset 0x208, read-write */
   uint32_t nvic_ispr2;
   /** Irq 96 to 127 Set Pending Register. Use the Interrupt Set-Pending Registers to force interrupts into the pending state and determine which interrupts are currently pending at address offset 0x20C, read-write */
   uint32_t nvic_ispr3;
   /** Irq 128 to 159 Set Pending Register. Use the Interrupt Set-Pending Registers to force interrupts into the pending state and determine which interrupts are currently pending at address offset 0x210, read-write */
   uint32_t nvic_ispr4;
   /** Irq 160 to 191 Set Pending Register. Use the Interrupt Set-Pending Registers to force interrupts into the pending state and determine which interrupts are currently pending at address offset 0x214, read-write */
   uint32_t nvic_ispr5;
   /** Irq 192 to 223 Set Pending Register. Use the Interrupt Set-Pending Registers to force interrupts into the pending state and determine which interrupts are currently pending at address offset 0x218, read-write */
   uint32_t nvic_ispr6;
   /** Irq 224 to 239 Set Pending Register. Use the Interrupt Set-Pending Registers to force interrupts into the pending state and determine which interrupts are currently pending at address offset 0x21C, read-write */
   uint32_t nvic_ispr7;
   /** Reserved space */
   uint8_t fill5[96];
   /** Irq 0 to 31 Clear Pending Register. Use the Interrupt Clear-Pending Registers to clear pending interrupts and determine which interrupts are currently pending. at address offset 0x280, read-write */
   uint32_t nvic_icpr0;
   /** Irq 32 to 63 Clear Pending Register. Use the Interrupt Clear-Pending Registers to clear pending interrupts and determine which interrupts are currently pending. at address offset 0x284, read-write */
   uint32_t nvic_icpr1;
   /** Irq 64 to 95 Clear Pending Register. Use the Interrupt Clear-Pending Registers to clear pending interrupts and determine which interrupts are currently pending. at address offset 0x288, read-write */
   uint32_t nvic_icpr2;
   /** Irq 96 to 127 Clear Pending Register. Use the Interrupt Clear-Pending Registers to clear pending interrupts and determine which interrupts are currently pending. at address offset 0x28C, read-write */
   uint32_t nvic_icpr3;
   /** Irq 128 to 159 Clear Pending Register. Use the Interrupt Clear-Pending Registers to clear pending interrupts and determine which interrupts are currently pending. at address offset 0x290, read-write */
   uint32_t nvic_icpr4;
   /** Irq 160 to 191 Clear Pending Register. Use the Interrupt Clear-Pending Registers to clear pending interrupts and determine which interrupts are currently pending. at address offset 0x294, read-write */
   uint32_t nvic_icpr5;
   /** Irq 192 to 223 Clear Pending Register. Use the Interrupt Clear-Pending Registers to clear pending interrupts and determine which interrupts are currently pending. at address offset 0x298, read-write */
   uint32_t nvic_icpr6;
   /** Irq 224 to 239 Clear Pending Register. Use the Interrupt Clear-Pending Registers to clear pending interrupts and determine which interrupts are currently pending. at address offset 0x29C, read-write */
   uint32_t nvic_icpr7;
   /** Reserved space */
   uint8_t fill6[96];
   /** Irq 0 to 31 Active Bit Register. Read the Active Bit Registers to determine which interrupts are active. Each flag in the register corresponds to one interrupt. at address offset 0x300, read-only */
   uint32_t nvic_iabr0;
   /** Irq 32 to 63 Active Bit Register. Read the Active Bit Registers to determine which interrupts are active. Each flag in the register corresponds to one interrupt. at address offset 0x304, read-only */
   uint32_t nvic_iabr1;
   /** Irq 64 to 95 Active Bit Register. Read the Active Bit Registers to determine which interrupts are active. Each flag in the register corresponds to one interrupt. at address offset 0x308, read-only */
   uint32_t nvic_iabr2;
   /** Irq 96 to 127 Active Bit Register. Read the Active Bit Registers to determine which interrupts are active. Each flag in the register corresponds to one interrupt. at address offset 0x30C, read-only */
   uint32_t nvic_iabr3;
   /** Irq 128 to 159 Active Bit Register. Read the Active Bit Registers to determine which interrupts are active. Each flag in the register corresponds to one interrupt. at address offset 0x310, read-only */
   uint32_t nvic_iabr4;
   /** Irq 160 to 191 Active Bit Register. Read the Active Bit Registers to determine which interrupts are active. Each flag in the register corresponds to one interrupt. at address offset 0x314, read-only */
   uint32_t nvic_iabr5;
   /** Irq 192 to 223 Active Bit Register. Read the Active Bit Registers to determine which interrupts are active. Each flag in the register corresponds to one interrupt. at address offset 0x318, read-only */
   uint32_t nvic_iabr6;
   /** Irq 224 to 239 Active Bit Register. Read the Active Bit Registers to determine which interrupts are active. Each flag in the register corresponds to one interrupt. at address offset 0x31C, read-only */
   uint32_t nvic_iabr7;
   /** Reserved space */
   uint8_t fill7[224];
   /** Irq Priority Register. Use the Interrupt Priority Registers to assign a priority from 0 to 255 to each of the available interrupts. 0 is the highest priority, and 255 is the lowest. at address offset 0x400, read-write */
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
   /** Auxiliary Fault Status Register. Use the Auxiliary Fault Status Register (AFSR) to determine additional system fault information to software. The AFSR flags map directly onto the AUXFAULT inputs of the processor, and a single-cycle high level on an external pin causes the corresponding AFSR bit to become latched as one. The bit can only be cleared by writing a one to the corresponding AFSR bit. When an AFSR bit is written or latched as one, an exception does not occur. If you require an exception, you must use an interrupt. at address offset 0xD3C, read-write */
   uint32_t afsr;
   /** Processor Feature register0. Processor Feature register0 at address offset 0xD40,  */
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
   uint8_t fill9[20];
   /** Coprocessor Access Control Register. The Coprocessor Access Control Register (CPACR) specifies the access privileges for coprocessors. at address offset 0xD88, read-write */
   uint32_t cpacr;
   /** Reserved space */
   uint8_t fill10[4];
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
   uint8_t fill11[52];
   /** Debug Halting Control and Status Register. The purpose of the Debug Halting Control and Status Register (DHCSR) is to provide status information about the state of the processor, enable core debug, halt and step the processor. For writes, 0xA05F must be written to bits [31:16], otherwise the write operation is ignored and no bits are written into the register. If not enabled for Halting mode, C_DEBUGEN = 1, all other fields are disabled. This register is not reset on a system reset. It is reset by a power-on reset. However, the C_HALT bit always clears on a system reset. To halt on a reset, the following bits must be enabled: bit [0], VC_CORERESET, of the Debug Exception and Monitor Control Register and bit [0],C_DEBUGEN, of the Debug Halting Control and Status Register. Note that writes to this register in any size other than word are Unpredictable. It is acceptable to read in any size, and you can use it to avoid or intentionally change a sticky bit. Bit 16 of DHCSR is Unpredictable on reset at address offset 0xDF0, read-write */
   uint32_t dhcsr;
   /** Deubg Core Register Selector Register. The purpose of the Debug Core Register Selector Register (DCRSR) is to select the processor register to transfer data to or from. This write-only register generates a handshake to the core to transfer data to or from Debug Core Register Data Register and the selected register. Until this core transaction is complete, bit [16], S_REGRDY, of the DHCSR is 0. Note that writes to this register in any size but word are Unpredictable. Note that PSR registers are fully accessible this way, whereas some read as 0 when using MRS instructions. Note that all bits can be written, but some combinations cause a fault when execution is resumed. Note that IT might be written and behaves as though in an IT block. at address offset 0xDF4, write-only */
   uint32_t dcrsr;
   /** Debug Core Register Data Register. The purpose of the Debug Core Register Data Register (DCRDR) is to hold data for reading and writing registers to and from the processor. This is the data value written to the register selected by the Debug Register Selector Register. When the processor receives a request from the Debug Core Register Selector, this register is read or written by the processor using a normal load-store unit operation. If core register transfers are not being performed, software-based debug monitors can use this register for communication in non-halting debug. For example, OS RSD and Real View Monitor. This enables flags and bits to acknowledge state and indicate if commands have been accepted to, replied to, or accepted and replied to. at address offset 0xDF8, read-write */
   uint32_t dcrdr;
   /** Debug Exception and Monitor Control Register. The purpose of the Debug Exception and Monitor Control Register (DEMCR) is Vector catching and Debug monitor control.  This register manages exception behavior under debug. Vector catching is only available to halting debug. The upper halfword is for monitor controls and the lower halfword is for halting exception support. This register is not reset on a system reset. This register is reset by a power-on reset. Bits [19:16] are always cleared on a core reset. The debug monitor is enabled by software in the reset handler or later, or by the AHB-AP port. Vector catching is semi-synchronous. When a matching event is seen, a Halt is requested. Because the processor can only halt on an instruction boundary, it must wait until the next instruction boundary. 
       As a result, it stops on the first instruction of the exception handler. However, two special cases exist when a vector catch has triggered: 1. If a fault is taken during a vector read or stack push error the halt occurs on the corresponding fault handler for the vector error or stack push. 2. If a late arriving interrupt detected during a vector read or stack push error it is not taken. That is, an implementation that supports the late arrival optimization must suppress it in this case. at address offset 0xDFC, read-write */
   uint32_t demcr;
   /** Reserved space */
   uint8_t fill12[256];
   /** Software Trigger Interrupt Register. Use the Software Trigger Interrupt Register to pend an interrupt to trigger. at address offset 0xF00, write-only */
   uint32_t stir;
};

/** bit field defines for cortex_m3_scs_s#ictr */
#define CORTEX_M3_SCS_ICTR_INTLINESNUM_OFFSET 0
#define CORTEX_M3_SCS_ICTR_INTLINESNUM_SIZE 5

/** bit field defines for cortex_m3_scs_s#actlr */
#define CORTEX_M3_SCS_ACTLR_DISMCYCINT_OFFSET 0
#define CORTEX_M3_SCS_ACTLR_DISMCYCINT_SIZE 1
#define CORTEX_M3_SCS_ACTLR_DISDEFWBUF_OFFSET 1
#define CORTEX_M3_SCS_ACTLR_DISDEFWBUF_SIZE 1
#define CORTEX_M3_SCS_ACTLR_DISFOLD_OFFSET 2
#define CORTEX_M3_SCS_ACTLR_DISFOLD_SIZE 1

/** bit field defines for cortex_m3_scs_s#stcsr */
#define CORTEX_M3_SCS_STCSR_ENABLE_OFFSET 0
#define CORTEX_M3_SCS_STCSR_ENABLE_SIZE 1
#define CORTEX_M3_SCS_STCSR_TICKINT_OFFSET 1
#define CORTEX_M3_SCS_STCSR_TICKINT_SIZE 1
#define CORTEX_M3_SCS_STCSR_CLKSOURCE_OFFSET 2
#define CORTEX_M3_SCS_STCSR_CLKSOURCE_SIZE 1
#define CORTEX_M3_SCS_STCSR_COUNTFLAG_OFFSET 16
#define CORTEX_M3_SCS_STCSR_COUNTFLAG_SIZE 1

/** bit field defines for cortex_m3_scs_s#strvr */
#define CORTEX_M3_SCS_STRVR_RELOAD_OFFSET 0
#define CORTEX_M3_SCS_STRVR_RELOAD_SIZE 24

/** bit field defines for cortex_m3_scs_s#stcvr */
#define CORTEX_M3_SCS_STCVR_CURRENT_OFFSET 0
#define CORTEX_M3_SCS_STCVR_CURRENT_SIZE 24

/** bit field defines for cortex_m3_scs_s#stcr */
#define CORTEX_M3_SCS_STCR_TENMS_OFFSET 0
#define CORTEX_M3_SCS_STCR_TENMS_SIZE 24
#define CORTEX_M3_SCS_STCR_SKEW_OFFSET 30
#define CORTEX_M3_SCS_STCR_SKEW_SIZE 1
#define CORTEX_M3_SCS_STCR_NOREF_OFFSET 31
#define CORTEX_M3_SCS_STCR_NOREF_SIZE 1

/** bit field defines for cortex_m3_scs_s#nvic_iser0 */
#define CORTEX_M3_SCS_NVIC_ISER0_SETENA_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ISER0_SETENA_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_iser1 */
#define CORTEX_M3_SCS_NVIC_ISER1_SETENA_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ISER1_SETENA_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_iser2 */
#define CORTEX_M3_SCS_NVIC_ISER2_SETENA_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ISER2_SETENA_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_iser3 */
#define CORTEX_M3_SCS_NVIC_ISER3_SETENA_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ISER3_SETENA_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_iser4 */
#define CORTEX_M3_SCS_NVIC_ISER4_SETENA_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ISER4_SETENA_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_iser5 */
#define CORTEX_M3_SCS_NVIC_ISER5_SETENA_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ISER5_SETENA_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_iser6 */
#define CORTEX_M3_SCS_NVIC_ISER6_SETENA_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ISER6_SETENA_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_iser7 */
#define CORTEX_M3_SCS_NVIC_ISER7_SETENA_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ISER7_SETENA_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_icer0 */
#define CORTEX_M3_SCS_NVIC_ICER0_CLRENA_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ICER0_CLRENA_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_icer1 */
#define CORTEX_M3_SCS_NVIC_ICER1_CLRENA_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ICER1_CLRENA_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_icer2 */
#define CORTEX_M3_SCS_NVIC_ICER2_CLRENA_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ICER2_CLRENA_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_icer3 */
#define CORTEX_M3_SCS_NVIC_ICER3_CLRENA_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ICER3_CLRENA_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_icer4 */
#define CORTEX_M3_SCS_NVIC_ICER4_CLRENA_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ICER4_CLRENA_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_icer5 */
#define CORTEX_M3_SCS_NVIC_ICER5_CLRENA_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ICER5_CLRENA_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_icer6 */
#define CORTEX_M3_SCS_NVIC_ICER6_CLRENA_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ICER6_CLRENA_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_icer7 */
#define CORTEX_M3_SCS_NVIC_ICER7_CLRENA_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ICER7_CLRENA_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_ispr0 */
#define CORTEX_M3_SCS_NVIC_ISPR0_SETPEND_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ISPR0_SETPEND_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_ispr1 */
#define CORTEX_M3_SCS_NVIC_ISPR1_SETPEND_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ISPR1_SETPEND_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_ispr2 */
#define CORTEX_M3_SCS_NVIC_ISPR2_SETPEND_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ISPR2_SETPEND_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_ispr3 */
#define CORTEX_M3_SCS_NVIC_ISPR3_SETPEND_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ISPR3_SETPEND_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_ispr4 */
#define CORTEX_M3_SCS_NVIC_ISPR4_SETPEND_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ISPR4_SETPEND_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_ispr5 */
#define CORTEX_M3_SCS_NVIC_ISPR5_SETPEND_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ISPR5_SETPEND_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_ispr6 */
#define CORTEX_M3_SCS_NVIC_ISPR6_SETPEND_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ISPR6_SETPEND_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_ispr7 */
#define CORTEX_M3_SCS_NVIC_ISPR7_SETPEND_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ISPR7_SETPEND_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_icpr0 */
#define CORTEX_M3_SCS_NVIC_ICPR0_CLRPEND_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ICPR0_CLRPEND_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_icpr1 */
#define CORTEX_M3_SCS_NVIC_ICPR1_CLRPEND_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ICPR1_CLRPEND_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_icpr2 */
#define CORTEX_M3_SCS_NVIC_ICPR2_CLRPEND_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ICPR2_CLRPEND_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_icpr3 */
#define CORTEX_M3_SCS_NVIC_ICPR3_CLRPEND_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ICPR3_CLRPEND_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_icpr4 */
#define CORTEX_M3_SCS_NVIC_ICPR4_CLRPEND_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ICPR4_CLRPEND_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_icpr5 */
#define CORTEX_M3_SCS_NVIC_ICPR5_CLRPEND_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ICPR5_CLRPEND_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_icpr6 */
#define CORTEX_M3_SCS_NVIC_ICPR6_CLRPEND_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ICPR6_CLRPEND_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_icpr7 */
#define CORTEX_M3_SCS_NVIC_ICPR7_CLRPEND_OFFSET 0
#define CORTEX_M3_SCS_NVIC_ICPR7_CLRPEND_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_iabr0 */
#define CORTEX_M3_SCS_NVIC_IABR0_ACTIVE_OFFSET 0
#define CORTEX_M3_SCS_NVIC_IABR0_ACTIVE_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_iabr1 */
#define CORTEX_M3_SCS_NVIC_IABR1_ACTIVE_OFFSET 0
#define CORTEX_M3_SCS_NVIC_IABR1_ACTIVE_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_iabr2 */
#define CORTEX_M3_SCS_NVIC_IABR2_ACTIVE_OFFSET 0
#define CORTEX_M3_SCS_NVIC_IABR2_ACTIVE_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_iabr3 */
#define CORTEX_M3_SCS_NVIC_IABR3_ACTIVE_OFFSET 0
#define CORTEX_M3_SCS_NVIC_IABR3_ACTIVE_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_iabr4 */
#define CORTEX_M3_SCS_NVIC_IABR4_ACTIVE_OFFSET 0
#define CORTEX_M3_SCS_NVIC_IABR4_ACTIVE_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_iabr5 */
#define CORTEX_M3_SCS_NVIC_IABR5_ACTIVE_OFFSET 0
#define CORTEX_M3_SCS_NVIC_IABR5_ACTIVE_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_iabr6 */
#define CORTEX_M3_SCS_NVIC_IABR6_ACTIVE_OFFSET 0
#define CORTEX_M3_SCS_NVIC_IABR6_ACTIVE_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_iabr7 */
#define CORTEX_M3_SCS_NVIC_IABR7_ACTIVE_OFFSET 0
#define CORTEX_M3_SCS_NVIC_IABR7_ACTIVE_SIZE 32

/** bit field defines for cortex_m3_scs_s#nvic_ipr */
#define CORTEX_M3_SCS_NVIC_IPR_PRI_0_OFFSET 0
#define CORTEX_M3_SCS_NVIC_IPR_PRI_0_SIZE 8
#define CORTEX_M3_SCS_NVIC_IPR_PRI_1_OFFSET 8
#define CORTEX_M3_SCS_NVIC_IPR_PRI_1_SIZE 8
#define CORTEX_M3_SCS_NVIC_IPR_PRI_2_OFFSET 16
#define CORTEX_M3_SCS_NVIC_IPR_PRI_2_SIZE 8
#define CORTEX_M3_SCS_NVIC_IPR_PRI_3_OFFSET 24
#define CORTEX_M3_SCS_NVIC_IPR_PRI_3_SIZE 8

/** bit field defines for cortex_m3_scs_s#cpuid */
#define CORTEX_M3_SCS_CPUID_REVISION_OFFSET 0
#define CORTEX_M3_SCS_CPUID_REVISION_SIZE 4
#define CORTEX_M3_SCS_CPUID_PARTNO_OFFSET 4
#define CORTEX_M3_SCS_CPUID_PARTNO_SIZE 12
#define CORTEX_M3_SCS_CPUID_CONSTANT_OFFSET 16
#define CORTEX_M3_SCS_CPUID_CONSTANT_SIZE 4
#define CORTEX_M3_SCS_CPUID_VARIANT_OFFSET 20
#define CORTEX_M3_SCS_CPUID_VARIANT_SIZE 4
#define CORTEX_M3_SCS_CPUID_IMPLEMENTER_OFFSET 24
#define CORTEX_M3_SCS_CPUID_IMPLEMENTER_SIZE 8

/** bit field defines for cortex_m3_scs_s#icsr */
#define CORTEX_M3_SCS_ICSR_VECTACTIVE_OFFSET 0
#define CORTEX_M3_SCS_ICSR_VECTACTIVE_SIZE 9
#define CORTEX_M3_SCS_ICSR_RETTOBASE_OFFSET 11
#define CORTEX_M3_SCS_ICSR_RETTOBASE_SIZE 1
#define CORTEX_M3_SCS_ICSR_VECTPENDING_OFFSET 12
#define CORTEX_M3_SCS_ICSR_VECTPENDING_SIZE 6
#define CORTEX_M3_SCS_ICSR_ISRPENDING_OFFSET 22
#define CORTEX_M3_SCS_ICSR_ISRPENDING_SIZE 1
#define CORTEX_M3_SCS_ICSR_ISRPREEMPT_OFFSET 23
#define CORTEX_M3_SCS_ICSR_ISRPREEMPT_SIZE 1
#define CORTEX_M3_SCS_ICSR_PENDSTCLR_OFFSET 25
#define CORTEX_M3_SCS_ICSR_PENDSTCLR_SIZE 1
#define CORTEX_M3_SCS_ICSR_PENDSTSET_OFFSET 26
#define CORTEX_M3_SCS_ICSR_PENDSTSET_SIZE 1
#define CORTEX_M3_SCS_ICSR_PENDSVCLR_OFFSET 27
#define CORTEX_M3_SCS_ICSR_PENDSVCLR_SIZE 1
#define CORTEX_M3_SCS_ICSR_PENDSVSET_OFFSET 28
#define CORTEX_M3_SCS_ICSR_PENDSVSET_SIZE 1
#define CORTEX_M3_SCS_ICSR_NMIPENDSET_OFFSET 31
#define CORTEX_M3_SCS_ICSR_NMIPENDSET_SIZE 1

/** bit field defines for cortex_m3_scs_s#vtor */
#define CORTEX_M3_SCS_VTOR_TBLOFF_OFFSET 7
#define CORTEX_M3_SCS_VTOR_TBLOFF_SIZE 22
#define CORTEX_M3_SCS_VTOR_TBLBASE_OFFSET 29
#define CORTEX_M3_SCS_VTOR_TBLBASE_SIZE 1

/** bit field defines for cortex_m3_scs_s#aircr */
#define CORTEX_M3_SCS_AIRCR_VECTRESET_OFFSET 0
#define CORTEX_M3_SCS_AIRCR_VECTRESET_SIZE 1
#define CORTEX_M3_SCS_AIRCR_VECTCLRACTIVE_OFFSET 1
#define CORTEX_M3_SCS_AIRCR_VECTCLRACTIVE_SIZE 1
#define CORTEX_M3_SCS_AIRCR_SYSRESETREQ_OFFSET 2
#define CORTEX_M3_SCS_AIRCR_SYSRESETREQ_SIZE 1
#define CORTEX_M3_SCS_AIRCR_PRIGROUP_OFFSET 8
#define CORTEX_M3_SCS_AIRCR_PRIGROUP_SIZE 3
#define CORTEX_M3_SCS_AIRCR_ENDIANESS_OFFSET 15
#define CORTEX_M3_SCS_AIRCR_ENDIANESS_SIZE 1
#define CORTEX_M3_SCS_AIRCR_VECTKEY_OFFSET 16
#define CORTEX_M3_SCS_AIRCR_VECTKEY_SIZE 16

/** bit field defines for cortex_m3_scs_s#scr */
#define CORTEX_M3_SCS_SCR_SLEEPONEXIT_OFFSET 1
#define CORTEX_M3_SCS_SCR_SLEEPONEXIT_SIZE 1
#define CORTEX_M3_SCS_SCR_SLEEPDEEP_OFFSET 2
#define CORTEX_M3_SCS_SCR_SLEEPDEEP_SIZE 1
#define CORTEX_M3_SCS_SCR_SEVONPEND_OFFSET 4
#define CORTEX_M3_SCS_SCR_SEVONPEND_SIZE 1

/** bit field defines for cortex_m3_scs_s#ccr */
#define CORTEX_M3_SCS_CCR_NONBASETHREDENA_OFFSET 0
#define CORTEX_M3_SCS_CCR_NONBASETHREDENA_SIZE 1
#define CORTEX_M3_SCS_CCR_USERSETMPEND_OFFSET 1
#define CORTEX_M3_SCS_CCR_USERSETMPEND_SIZE 1
#define CORTEX_M3_SCS_CCR_UNALIGN_TRP_OFFSET 3
#define CORTEX_M3_SCS_CCR_UNALIGN_TRP_SIZE 1
#define CORTEX_M3_SCS_CCR_DIV_0_TRP_OFFSET 4
#define CORTEX_M3_SCS_CCR_DIV_0_TRP_SIZE 1
#define CORTEX_M3_SCS_CCR_BFHFNMIGN_OFFSET 8
#define CORTEX_M3_SCS_CCR_BFHFNMIGN_SIZE 1
#define CORTEX_M3_SCS_CCR_STKALIGN_OFFSET 9
#define CORTEX_M3_SCS_CCR_STKALIGN_SIZE 1

/** bit field defines for cortex_m3_scs_s#shpr1 */
#define CORTEX_M3_SCS_SHPR1_PRI_4_OFFSET 0
#define CORTEX_M3_SCS_SHPR1_PRI_4_SIZE 8
#define CORTEX_M3_SCS_SHPR1_PRI_5_OFFSET 8
#define CORTEX_M3_SCS_SHPR1_PRI_5_SIZE 8
#define CORTEX_M3_SCS_SHPR1_PRI_6_OFFSET 16
#define CORTEX_M3_SCS_SHPR1_PRI_6_SIZE 8
#define CORTEX_M3_SCS_SHPR1_PRI_7_OFFSET 24
#define CORTEX_M3_SCS_SHPR1_PRI_7_SIZE 8

/** bit field defines for cortex_m3_scs_s#shpr2 */
#define CORTEX_M3_SCS_SHPR2_PRI_8_OFFSET 0
#define CORTEX_M3_SCS_SHPR2_PRI_8_SIZE 8
#define CORTEX_M3_SCS_SHPR2_PRI_9_OFFSET 8
#define CORTEX_M3_SCS_SHPR2_PRI_9_SIZE 8
#define CORTEX_M3_SCS_SHPR2_PRI_10_OFFSET 16
#define CORTEX_M3_SCS_SHPR2_PRI_10_SIZE 8
#define CORTEX_M3_SCS_SHPR2_PRI_11_OFFSET 24
#define CORTEX_M3_SCS_SHPR2_PRI_11_SIZE 8

/** bit field defines for cortex_m3_scs_s#shpr3 */
#define CORTEX_M3_SCS_SHPR3_PRI_12_OFFSET 0
#define CORTEX_M3_SCS_SHPR3_PRI_12_SIZE 8
#define CORTEX_M3_SCS_SHPR3_PRI_13_OFFSET 8
#define CORTEX_M3_SCS_SHPR3_PRI_13_SIZE 8
#define CORTEX_M3_SCS_SHPR3_PRI_14_OFFSET 16
#define CORTEX_M3_SCS_SHPR3_PRI_14_SIZE 8
#define CORTEX_M3_SCS_SHPR3_PRI_15_OFFSET 24
#define CORTEX_M3_SCS_SHPR3_PRI_15_SIZE 8

/** bit field defines for cortex_m3_scs_s#shcsr */
#define CORTEX_M3_SCS_SHCSR_MEMFAULTACT_OFFSET 0
#define CORTEX_M3_SCS_SHCSR_MEMFAULTACT_SIZE 1
#define CORTEX_M3_SCS_SHCSR_BUSFAULTACT_OFFSET 1
#define CORTEX_M3_SCS_SHCSR_BUSFAULTACT_SIZE 1
#define CORTEX_M3_SCS_SHCSR_USGFAULTACT_OFFSET 3
#define CORTEX_M3_SCS_SHCSR_USGFAULTACT_SIZE 1
#define CORTEX_M3_SCS_SHCSR_SVCALLACT_OFFSET 7
#define CORTEX_M3_SCS_SHCSR_SVCALLACT_SIZE 1
#define CORTEX_M3_SCS_SHCSR_MONITORACT_OFFSET 8
#define CORTEX_M3_SCS_SHCSR_MONITORACT_SIZE 1
#define CORTEX_M3_SCS_SHCSR_PENDSVACT_OFFSET 10
#define CORTEX_M3_SCS_SHCSR_PENDSVACT_SIZE 1
#define CORTEX_M3_SCS_SHCSR_SYSTICKACT_OFFSET 11
#define CORTEX_M3_SCS_SHCSR_SYSTICKACT_SIZE 1
#define CORTEX_M3_SCS_SHCSR_USGFAULTPENDED_OFFSET 12
#define CORTEX_M3_SCS_SHCSR_USGFAULTPENDED_SIZE 1
#define CORTEX_M3_SCS_SHCSR_MEMFAULTPENDED_OFFSET 13
#define CORTEX_M3_SCS_SHCSR_MEMFAULTPENDED_SIZE 1
#define CORTEX_M3_SCS_SHCSR_BUSFAULTPENDED_OFFSET 14
#define CORTEX_M3_SCS_SHCSR_BUSFAULTPENDED_SIZE 1
#define CORTEX_M3_SCS_SHCSR_SVCALLPENDED_OFFSET 15
#define CORTEX_M3_SCS_SHCSR_SVCALLPENDED_SIZE 1
#define CORTEX_M3_SCS_SHCSR_MEMFAULTENA_OFFSET 16
#define CORTEX_M3_SCS_SHCSR_MEMFAULTENA_SIZE 1
#define CORTEX_M3_SCS_SHCSR_BUSFAULTENA_OFFSET 17
#define CORTEX_M3_SCS_SHCSR_BUSFAULTENA_SIZE 1
#define CORTEX_M3_SCS_SHCSR_USGFAULTENA_OFFSET 18
#define CORTEX_M3_SCS_SHCSR_USGFAULTENA_SIZE 1

/** bit field defines for cortex_m3_scs_s#cfsr */
#define CORTEX_M3_SCS_CFSR_IACCVIOL_OFFSET 0
#define CORTEX_M3_SCS_CFSR_IACCVIOL_SIZE 1
#define CORTEX_M3_SCS_CFSR_DACCVIOL_OFFSET 1
#define CORTEX_M3_SCS_CFSR_DACCVIOL_SIZE 1
#define CORTEX_M3_SCS_CFSR_MUNSTKERR_OFFSET 3
#define CORTEX_M3_SCS_CFSR_MUNSTKERR_SIZE 1
#define CORTEX_M3_SCS_CFSR_MSTKERR_OFFSET 4
#define CORTEX_M3_SCS_CFSR_MSTKERR_SIZE 1
#define CORTEX_M3_SCS_CFSR_MMARVALID_OFFSET 7
#define CORTEX_M3_SCS_CFSR_MMARVALID_SIZE 1
#define CORTEX_M3_SCS_CFSR_IBUSERR_OFFSET 8
#define CORTEX_M3_SCS_CFSR_IBUSERR_SIZE 1
#define CORTEX_M3_SCS_CFSR_PRECISERR_OFFSET 9
#define CORTEX_M3_SCS_CFSR_PRECISERR_SIZE 1
#define CORTEX_M3_SCS_CFSR_IMPRECISERR_OFFSET 10
#define CORTEX_M3_SCS_CFSR_IMPRECISERR_SIZE 1
#define CORTEX_M3_SCS_CFSR_UNSTKERR_OFFSET 11
#define CORTEX_M3_SCS_CFSR_UNSTKERR_SIZE 1
#define CORTEX_M3_SCS_CFSR_STKERR_OFFSET 12
#define CORTEX_M3_SCS_CFSR_STKERR_SIZE 1
#define CORTEX_M3_SCS_CFSR_BFARVALID_OFFSET 15
#define CORTEX_M3_SCS_CFSR_BFARVALID_SIZE 1
#define CORTEX_M3_SCS_CFSR_UNDEFINSTR_OFFSET 16
#define CORTEX_M3_SCS_CFSR_UNDEFINSTR_SIZE 1
#define CORTEX_M3_SCS_CFSR_INVSTATE_OFFSET 17
#define CORTEX_M3_SCS_CFSR_INVSTATE_SIZE 1
#define CORTEX_M3_SCS_CFSR_INVPC_OFFSET 18
#define CORTEX_M3_SCS_CFSR_INVPC_SIZE 1
#define CORTEX_M3_SCS_CFSR_NOCP_OFFSET 19
#define CORTEX_M3_SCS_CFSR_NOCP_SIZE 1
#define CORTEX_M3_SCS_CFSR_UNALIGNED_OFFSET 24
#define CORTEX_M3_SCS_CFSR_UNALIGNED_SIZE 1
#define CORTEX_M3_SCS_CFSR_DIVBYZERO_OFFSET 25
#define CORTEX_M3_SCS_CFSR_DIVBYZERO_SIZE 1

/** bit field defines for cortex_m3_scs_s#hfsr */
#define CORTEX_M3_SCS_HFSR_VECTTBL_OFFSET 1
#define CORTEX_M3_SCS_HFSR_VECTTBL_SIZE 1
#define CORTEX_M3_SCS_HFSR_FORCED_OFFSET 30
#define CORTEX_M3_SCS_HFSR_FORCED_SIZE 1
#define CORTEX_M3_SCS_HFSR_DEBUGEVT_OFFSET 31
#define CORTEX_M3_SCS_HFSR_DEBUGEVT_SIZE 1

/** bit field defines for cortex_m3_scs_s#dfsr */
#define CORTEX_M3_SCS_DFSR_HALTED_OFFSET 0
#define CORTEX_M3_SCS_DFSR_HALTED_SIZE 1
#define CORTEX_M3_SCS_DFSR_BKPT_OFFSET 1
#define CORTEX_M3_SCS_DFSR_BKPT_SIZE 1
#define CORTEX_M3_SCS_DFSR_DWTTRAP_OFFSET 2
#define CORTEX_M3_SCS_DFSR_DWTTRAP_SIZE 1
#define CORTEX_M3_SCS_DFSR_VCATCH_OFFSET 3
#define CORTEX_M3_SCS_DFSR_VCATCH_SIZE 1
#define CORTEX_M3_SCS_DFSR_EXTERNAL_OFFSET 4
#define CORTEX_M3_SCS_DFSR_EXTERNAL_SIZE 1

/** bit field defines for cortex_m3_scs_s#mmfar */
#define CORTEX_M3_SCS_MMFAR_ADDRESS_OFFSET 0
#define CORTEX_M3_SCS_MMFAR_ADDRESS_SIZE 32

/** bit field defines for cortex_m3_scs_s#bfar */
#define CORTEX_M3_SCS_BFAR_ADDRESS_OFFSET 0
#define CORTEX_M3_SCS_BFAR_ADDRESS_SIZE 32

/** bit field defines for cortex_m3_scs_s#afsr */
#define CORTEX_M3_SCS_AFSR_AUXFAULT_OFFSET 0
#define CORTEX_M3_SCS_AFSR_AUXFAULT_SIZE 32

/** bit field defines for cortex_m3_scs_s#id_pfr0 */
#define CORTEX_M3_SCS_ID_PFR0_STATE0_OFFSET 0
#define CORTEX_M3_SCS_ID_PFR0_STATE0_SIZE 4
#define CORTEX_M3_SCS_ID_PFR0_STATE1_OFFSET 4
#define CORTEX_M3_SCS_ID_PFR0_STATE1_SIZE 4

/** bit field defines for cortex_m3_scs_s#id_pfr1 */
#define CORTEX_M3_SCS_ID_PFR1_MICROCONTROLLER_PROGRAMMERS_MODEL_OFFSET 8
#define CORTEX_M3_SCS_ID_PFR1_MICROCONTROLLER_PROGRAMMERS_MODEL_SIZE 4

/** bit field defines for cortex_m3_scs_s#id_dfr0 */
#define CORTEX_M3_SCS_ID_DFR0_MICROCONTROLLER_DEBUG_MODEL_OFFSET 20
#define CORTEX_M3_SCS_ID_DFR0_MICROCONTROLLER_DEBUG_MODEL_SIZE 4

/** bit field defines for cortex_m3_scs_s#id_afr0 */
#define CORTEX_M3_SCS_ID_AFR0_IMPLEMENTATION_DEFINED0_OFFSET 0
#define CORTEX_M3_SCS_ID_AFR0_IMPLEMENTATION_DEFINED0_SIZE 4
#define CORTEX_M3_SCS_ID_AFR0_IMPLEMENTATION_DEFINED1_OFFSET 4
#define CORTEX_M3_SCS_ID_AFR0_IMPLEMENTATION_DEFINED1_SIZE 4
#define CORTEX_M3_SCS_ID_AFR0_IMPLEMENTATION_DEFINED2_OFFSET 8
#define CORTEX_M3_SCS_ID_AFR0_IMPLEMENTATION_DEFINED2_SIZE 4
#define CORTEX_M3_SCS_ID_AFR0_IMPLEMENTATION_DEFINED3_OFFSET 12
#define CORTEX_M3_SCS_ID_AFR0_IMPLEMENTATION_DEFINED3_SIZE 4

/** bit field defines for cortex_m3_scs_s#id_mmfr0 */
#define CORTEX_M3_SCS_ID_MMFR0_PMSA_SUPPORT_OFFSET 4
#define CORTEX_M3_SCS_ID_MMFR0_PMSA_SUPPORT_SIZE 4
#define CORTEX_M3_SCS_ID_MMFR0_CACHE_COHERENCE_SUPPORT_OFFSET 8
#define CORTEX_M3_SCS_ID_MMFR0_CACHE_COHERENCE_SUPPORT_SIZE 4
#define CORTEX_M3_SCS_ID_MMFR0_OUTER_NON_SHARABLE_SUPPORT_OFFSET 12
#define CORTEX_M3_SCS_ID_MMFR0_OUTER_NON_SHARABLE_SUPPORT_SIZE 4
#define CORTEX_M3_SCS_ID_MMFR0_AUXILIARY_REGISTER_SUPPORT_OFFSET 20
#define CORTEX_M3_SCS_ID_MMFR0_AUXILIARY_REGISTER_SUPPORT_SIZE 4

/** bit field defines for cortex_m3_scs_s#id_mmfr1 */
#define CORTEX_M3_SCS_ID_MMFR1_ID_MMFR1_OFFSET 0
#define CORTEX_M3_SCS_ID_MMFR1_ID_MMFR1_SIZE 32

/** bit field defines for cortex_m3_scs_s#id_mmfr2 */
#define CORTEX_M3_SCS_ID_MMFR2_WAIT_FOR_INTERRUPT_STALLING_OFFSET 24
#define CORTEX_M3_SCS_ID_MMFR2_WAIT_FOR_INTERRUPT_STALLING_SIZE 4

/** bit field defines for cortex_m3_scs_s#id_mmfr3 */
#define CORTEX_M3_SCS_ID_MMFR3_ID_MMFR3_OFFSET 0
#define CORTEX_M3_SCS_ID_MMFR3_ID_MMFR3_SIZE 32

/** bit field defines for cortex_m3_scs_s#id_isar0 */
#define CORTEX_M3_SCS_ID_ISAR0_BITCOUNT_INSTRS_OFFSET 4
#define CORTEX_M3_SCS_ID_ISAR0_BITCOUNT_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR0_BITFIELD_INSTRS_OFFSET 8
#define CORTEX_M3_SCS_ID_ISAR0_BITFIELD_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR0_CMPBRANCH_INSTRS_OFFSET 12
#define CORTEX_M3_SCS_ID_ISAR0_CMPBRANCH_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR0_COPROC_INSTRS_OFFSET 16
#define CORTEX_M3_SCS_ID_ISAR0_COPROC_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR0_DEBUG_INSTRS_OFFSET 20
#define CORTEX_M3_SCS_ID_ISAR0_DEBUG_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR0_DIVIDE_INSTRS_OFFSET 24
#define CORTEX_M3_SCS_ID_ISAR0_DIVIDE_INSTRS_SIZE 4

/** bit field defines for cortex_m3_scs_s#id_isar1 */
#define CORTEX_M3_SCS_ID_ISAR1_EXTEND_INSRS_OFFSET 12
#define CORTEX_M3_SCS_ID_ISAR1_EXTEND_INSRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR1_IFTHEN_INSTRS_OFFSET 16
#define CORTEX_M3_SCS_ID_ISAR1_IFTHEN_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR1_IMMEDIATE_INSTRS_OFFSET 20
#define CORTEX_M3_SCS_ID_ISAR1_IMMEDIATE_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR1_INTERWORK_INSTRS_OFFSET 24
#define CORTEX_M3_SCS_ID_ISAR1_INTERWORK_INSTRS_SIZE 4

/** bit field defines for cortex_m3_scs_s#id_isar2 */
#define CORTEX_M3_SCS_ID_ISAR2_LOADSTORE_INSTRS_OFFSET 0
#define CORTEX_M3_SCS_ID_ISAR2_LOADSTORE_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR2_MEMHINT_INSTRS_OFFSET 4
#define CORTEX_M3_SCS_ID_ISAR2_MEMHINT_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR2_MULTIACCESSINT_INSTRS_OFFSET 8
#define CORTEX_M3_SCS_ID_ISAR2_MULTIACCESSINT_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR2_MULT_INSTRS_OFFSET 12
#define CORTEX_M3_SCS_ID_ISAR2_MULT_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR2_MULTS_INSTRS_OFFSET 16
#define CORTEX_M3_SCS_ID_ISAR2_MULTS_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR2_MULTU_INSTRS_OFFSET 20
#define CORTEX_M3_SCS_ID_ISAR2_MULTU_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR2_REVERSAL_INSTRS_OFFSET 28
#define CORTEX_M3_SCS_ID_ISAR2_REVERSAL_INSTRS_SIZE 4

/** bit field defines for cortex_m3_scs_s#id_isar3 */
#define CORTEX_M3_SCS_ID_ISAR3_SATRUATE_INSTRS_OFFSET 0
#define CORTEX_M3_SCS_ID_ISAR3_SATRUATE_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR3_SIMD_INSTRS_OFFSET 4
#define CORTEX_M3_SCS_ID_ISAR3_SIMD_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR3_SVC_INSTRS_OFFSET 8
#define CORTEX_M3_SCS_ID_ISAR3_SVC_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR3_SYNCPRIM_INSTRS_OFFSET 12
#define CORTEX_M3_SCS_ID_ISAR3_SYNCPRIM_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR3_TABBRANCH_INSTRS_OFFSET 16
#define CORTEX_M3_SCS_ID_ISAR3_TABBRANCH_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR3_THUMBCOPY_INSTRS_OFFSET 20
#define CORTEX_M3_SCS_ID_ISAR3_THUMBCOPY_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR3_TRUENOP_INSTRS_OFFSET 24
#define CORTEX_M3_SCS_ID_ISAR3_TRUENOP_INSTRS_SIZE 4

/** bit field defines for cortex_m3_scs_s#id_isar4 */
#define CORTEX_M3_SCS_ID_ISAR4_UNPRIV_INSTRS_OFFSET 0
#define CORTEX_M3_SCS_ID_ISAR4_UNPRIV_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR4_WITHSHIFTS_INSTRS_OFFSET 4
#define CORTEX_M3_SCS_ID_ISAR4_WITHSHIFTS_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR4_WRITEBACK_INSTRS_OFFSET 8
#define CORTEX_M3_SCS_ID_ISAR4_WRITEBACK_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR4_BARRIER_INSTRS_OFFSET 16
#define CORTEX_M3_SCS_ID_ISAR4_BARRIER_INSTRS_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR4_SYNCPRIM_INSTRS_FRAC_OFFSET 20
#define CORTEX_M3_SCS_ID_ISAR4_SYNCPRIM_INSTRS_FRAC_SIZE 4
#define CORTEX_M3_SCS_ID_ISAR4_PSR_M_INSTRS_OFFSET 24
#define CORTEX_M3_SCS_ID_ISAR4_PSR_M_INSTRS_SIZE 4

/** bit field defines for cortex_m3_scs_s#cpacr */
#define CORTEX_M3_SCS_CPACR_CP10_OFFSET 20
#define CORTEX_M3_SCS_CPACR_CP10_SIZE 2
#define CORTEX_M3_SCS_CPACR_CP11_OFFSET 22
#define CORTEX_M3_SCS_CPACR_CP11_SIZE 2

/** bit field defines for cortex_m3_scs_s#mpu_type */
#define CORTEX_M3_SCS_MPU_TYPE_SEPARATE_OFFSET 0
#define CORTEX_M3_SCS_MPU_TYPE_SEPARATE_SIZE 1
#define CORTEX_M3_SCS_MPU_TYPE_DREGION_OFFSET 8
#define CORTEX_M3_SCS_MPU_TYPE_DREGION_SIZE 8
#define CORTEX_M3_SCS_MPU_TYPE_IREGION_OFFSET 16
#define CORTEX_M3_SCS_MPU_TYPE_IREGION_SIZE 8

/** bit field defines for cortex_m3_scs_s#mpu_ctrl */
#define CORTEX_M3_SCS_MPU_CTRL_ENABLE_OFFSET 0
#define CORTEX_M3_SCS_MPU_CTRL_ENABLE_SIZE 1
#define CORTEX_M3_SCS_MPU_CTRL_HFNMIENA_OFFSET 1
#define CORTEX_M3_SCS_MPU_CTRL_HFNMIENA_SIZE 1
#define CORTEX_M3_SCS_MPU_CTRL_PRIVDEFENA_OFFSET 2
#define CORTEX_M3_SCS_MPU_CTRL_PRIVDEFENA_SIZE 1

/** bit field defines for cortex_m3_scs_s#mpu_rnr */
#define CORTEX_M3_SCS_MPU_RNR_REGION_OFFSET 0
#define CORTEX_M3_SCS_MPU_RNR_REGION_SIZE 8

/** bit field defines for cortex_m3_scs_s#mpu_rbar */
#define CORTEX_M3_SCS_MPU_RBAR_REGION_OFFSET 0
#define CORTEX_M3_SCS_MPU_RBAR_REGION_SIZE 4
#define CORTEX_M3_SCS_MPU_RBAR_VALID_OFFSET 4
#define CORTEX_M3_SCS_MPU_RBAR_VALID_SIZE 1
#define CORTEX_M3_SCS_MPU_RBAR_ADDR_OFFSET 5
#define CORTEX_M3_SCS_MPU_RBAR_ADDR_SIZE 27

/** bit field defines for cortex_m3_scs_s#mpu_rasr */
#define CORTEX_M3_SCS_MPU_RASR_ENABLE_OFFSET 0
#define CORTEX_M3_SCS_MPU_RASR_ENABLE_SIZE 1
#define CORTEX_M3_SCS_MPU_RASR_SIZE_OFFSET 1
#define CORTEX_M3_SCS_MPU_RASR_SIZE_SIZE 5
#define CORTEX_M3_SCS_MPU_RASR_SRD_OFFSET 8
#define CORTEX_M3_SCS_MPU_RASR_SRD_SIZE 8
#define CORTEX_M3_SCS_MPU_RASR_B_OFFSET 16
#define CORTEX_M3_SCS_MPU_RASR_B_SIZE 1
#define CORTEX_M3_SCS_MPU_RASR_C_OFFSET 17
#define CORTEX_M3_SCS_MPU_RASR_C_SIZE 1
#define CORTEX_M3_SCS_MPU_RASR_S_OFFSET 18
#define CORTEX_M3_SCS_MPU_RASR_S_SIZE 1
#define CORTEX_M3_SCS_MPU_RASR_TEX_OFFSET 19
#define CORTEX_M3_SCS_MPU_RASR_TEX_SIZE 3
#define CORTEX_M3_SCS_MPU_RASR_AP_OFFSET 24
#define CORTEX_M3_SCS_MPU_RASR_AP_SIZE 3
#define CORTEX_M3_SCS_MPU_RASR_XN_OFFSET 28
#define CORTEX_M3_SCS_MPU_RASR_XN_SIZE 1

/** bit field defines for cortex_m3_scs_s#mpu_rbar_a1 */
#define CORTEX_M3_SCS_MPU_RBAR_A1_MPU_RBAR_A1_OFFSET 0
#define CORTEX_M3_SCS_MPU_RBAR_A1_MPU_RBAR_A1_SIZE 32

/** bit field defines for cortex_m3_scs_s#mpu_rasr_a1 */
#define CORTEX_M3_SCS_MPU_RASR_A1_MPU_RASR_A1_OFFSET 0
#define CORTEX_M3_SCS_MPU_RASR_A1_MPU_RASR_A1_SIZE 32

/** bit field defines for cortex_m3_scs_s#mpu_rbar_a2 */
#define CORTEX_M3_SCS_MPU_RBAR_A2_MPU_RBAR_A2_OFFSET 0
#define CORTEX_M3_SCS_MPU_RBAR_A2_MPU_RBAR_A2_SIZE 32

/** bit field defines for cortex_m3_scs_s#mpu_rasr_a2 */
#define CORTEX_M3_SCS_MPU_RASR_A2_MPU_RASR_A2_OFFSET 0
#define CORTEX_M3_SCS_MPU_RASR_A2_MPU_RASR_A2_SIZE 32

/** bit field defines for cortex_m3_scs_s#mpu_rbar_a3 */
#define CORTEX_M3_SCS_MPU_RBAR_A3_MPU_RBAR_A3_OFFSET 0
#define CORTEX_M3_SCS_MPU_RBAR_A3_MPU_RBAR_A3_SIZE 32

/** bit field defines for cortex_m3_scs_s#mpu_rasr_a3 */
#define CORTEX_M3_SCS_MPU_RASR_A3_MPU_RASR_A3_OFFSET 0
#define CORTEX_M3_SCS_MPU_RASR_A3_MPU_RASR_A3_SIZE 32

/** bit field defines for cortex_m3_scs_s#dhcsr */
#define CORTEX_M3_SCS_DHCSR_C_DEBUGEN_OFFSET 0
#define CORTEX_M3_SCS_DHCSR_C_DEBUGEN_SIZE 1
#define CORTEX_M3_SCS_DHCSR_C_HALT_OFFSET 1
#define CORTEX_M3_SCS_DHCSR_C_HALT_SIZE 1
#define CORTEX_M3_SCS_DHCSR_C_STEP_OFFSET 2
#define CORTEX_M3_SCS_DHCSR_C_STEP_SIZE 1
#define CORTEX_M3_SCS_DHCSR_C_MASKINTS_OFFSET 3
#define CORTEX_M3_SCS_DHCSR_C_MASKINTS_SIZE 1
#define CORTEX_M3_SCS_DHCSR_C_SNAPSTALL_OFFSET 5
#define CORTEX_M3_SCS_DHCSR_C_SNAPSTALL_SIZE 1
#define CORTEX_M3_SCS_DHCSR_S_REGRDY_OFFSET 16
#define CORTEX_M3_SCS_DHCSR_S_REGRDY_SIZE 1
#define CORTEX_M3_SCS_DHCSR_S_HALT_OFFSET 17
#define CORTEX_M3_SCS_DHCSR_S_HALT_SIZE 1
#define CORTEX_M3_SCS_DHCSR_S_SLEEP_OFFSET 18
#define CORTEX_M3_SCS_DHCSR_S_SLEEP_SIZE 1
#define CORTEX_M3_SCS_DHCSR_S_LOCKUP_OFFSET 19
#define CORTEX_M3_SCS_DHCSR_S_LOCKUP_SIZE 1
#define CORTEX_M3_SCS_DHCSR_DEBUG_KEY0_OFFSET 20
#define CORTEX_M3_SCS_DHCSR_DEBUG_KEY0_SIZE 4
#define CORTEX_M3_SCS_DHCSR_S_RETIRE_ST_OFFSET 24
#define CORTEX_M3_SCS_DHCSR_S_RETIRE_ST_SIZE 1
#define CORTEX_M3_SCS_DHCSR_S_RESET_ST_OFFSET 25
#define CORTEX_M3_SCS_DHCSR_S_RESET_ST_SIZE 1
#define CORTEX_M3_SCS_DHCSR_DEBUG_KEY1_OFFSET 26
#define CORTEX_M3_SCS_DHCSR_DEBUG_KEY1_SIZE 6

/** bit field defines for cortex_m3_scs_s#dcrsr */
#define CORTEX_M3_SCS_DCRSR_REGSEL_OFFSET 0
#define CORTEX_M3_SCS_DCRSR_REGSEL_SIZE 5
#define CORTEX_M3_SCS_DCRSR_REGWNR_OFFSET 16
#define CORTEX_M3_SCS_DCRSR_REGWNR_SIZE 1

/** bit field defines for cortex_m3_scs_s#dcrdr */
#define CORTEX_M3_SCS_DCRDR_DCRDR_OFFSET 0
#define CORTEX_M3_SCS_DCRDR_DCRDR_SIZE 32

/** bit field defines for cortex_m3_scs_s#demcr */
#define CORTEX_M3_SCS_DEMCR_VC_CORERESET_OFFSET 0
#define CORTEX_M3_SCS_DEMCR_VC_CORERESET_SIZE 1
#define CORTEX_M3_SCS_DEMCR_VC_MMERR_OFFSET 4
#define CORTEX_M3_SCS_DEMCR_VC_MMERR_SIZE 1
#define CORTEX_M3_SCS_DEMCR_VC_NOCPERR_OFFSET 5
#define CORTEX_M3_SCS_DEMCR_VC_NOCPERR_SIZE 1
#define CORTEX_M3_SCS_DEMCR_VC_CHKERR_OFFSET 6
#define CORTEX_M3_SCS_DEMCR_VC_CHKERR_SIZE 1
#define CORTEX_M3_SCS_DEMCR_VC_STATERR_OFFSET 7
#define CORTEX_M3_SCS_DEMCR_VC_STATERR_SIZE 1
#define CORTEX_M3_SCS_DEMCR_VC_BUSERR_OFFSET 8
#define CORTEX_M3_SCS_DEMCR_VC_BUSERR_SIZE 1
#define CORTEX_M3_SCS_DEMCR_VC_INTERR_OFFSET 9
#define CORTEX_M3_SCS_DEMCR_VC_INTERR_SIZE 1
#define CORTEX_M3_SCS_DEMCR_VC_HARDERR_OFFSET 10
#define CORTEX_M3_SCS_DEMCR_VC_HARDERR_SIZE 1
#define CORTEX_M3_SCS_DEMCR_MON_EN_OFFSET 16
#define CORTEX_M3_SCS_DEMCR_MON_EN_SIZE 1
#define CORTEX_M3_SCS_DEMCR_MON_PEND_OFFSET 17
#define CORTEX_M3_SCS_DEMCR_MON_PEND_SIZE 1
#define CORTEX_M3_SCS_DEMCR_MON_STEP_OFFSET 18
#define CORTEX_M3_SCS_DEMCR_MON_STEP_SIZE 1
#define CORTEX_M3_SCS_DEMCR_MON_REQ_OFFSET 19
#define CORTEX_M3_SCS_DEMCR_MON_REQ_SIZE 1
#define CORTEX_M3_SCS_DEMCR_TRCENA_OFFSET 24
#define CORTEX_M3_SCS_DEMCR_TRCENA_SIZE 1

/** bit field defines for cortex_m3_scs_s#stir */
#define CORTEX_M3_SCS_STIR_INTID_OFFSET 0
#define CORTEX_M3_SCS_STIR_INTID_SIZE 9

/* EOF cortex_m3_scs.h */
#endif
