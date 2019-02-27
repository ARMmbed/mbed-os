# RAM memory model update - Mbed OS

# Table of contents

1. [RAM memory model update - Mbed OS](#mbed-os-ram-memory-model).
1. [Table of contents](#table-of-contents).
    1. [Revision history](#revision-history).
1. [Introduction](#introduction).
    1. [Current RAM memory model](#current-ram-memory-model).
    1. [Proposed RAM memory model](#proposed-ram-memory-model).
1. [Phases](#phases).
1. Detailed Design (#detailed-design).
1. [Tools and configuration changes](#tools-and-configuration-changes).

### Revision history

1.0 - A brief description of this version. For example, Initial revision - Author name - Date. 
**NOTE: You may also specify the Mbed OS version this revision of design document applies to.**
1.1 - Added new section - Author name - Date.

# Introduction

### Current RAM memory model

Single memory space is shared between stack and heap memory, start address is fixed but the size of both regions varies based on application and usage runtime.
Heap starts at the first address after the end of ZI growing up into higher memory address and stack starts at the last memory address of RAM growing downward into lower addresses.

                +----------------------+ Stack Start (Last address of RAM)
                | ISR stack            |
                | Main Stack(No RTOS)  |
                |          |           |
                |          V           |
                +----------------------+
                |          ^           |
                |          |           |
                |        Heap          |
                +----------------------+ HEAP Start
                |         ZI           |
                |(Idle, Timer and Main |
                | stack is in ZI for   |
                | RTOS)                |
                +----------------------+
                |                      |
                +----------------------+ First address of RAM

#### Drawbacks:
1. Collisions between stack and heap are hard to detect and result in hardfault.
1. Cannot check stack limit - In case of new ARM architecture stack limit registers are available to verify stack boundaries, but this feature cannot be used with dynamic stack size.
1. Stack size unification cannot be achieved across various targets.
1. GCC ARM: Memory allocator request memory at 4K boundary end of HEAP memory should be 4K aligned. Placing ISR stack (1K) after HEAP memory in case of RTOS, results in loss of 3K RAM memory
1. Memory allocators do not support HEAP split into multiple banks, hence with single region memory model HEAP is used only till end of first bank.

### Proposed RAM memory model

2-region memory model for heap and stack. Defined boundaries for ISR stack memory. Heap memory can be dynamic (starts at end of ZI and ends at last RAM address) or with fix boundaries in separate RAM bank.

                +----------------------+ Heap Ends (Last address of RAM)
                |          ^           |
                |          |           |
                |        Heap          |
                +----------------------+ HEAP Start
                |         ZI           |
                |(Idle, Timer and Main |
                | stack is in ZI for   |
                | RTOS)                |
                +----------------------+Stack Ends 
                | ISR stack            |
                | Main Stack(No RTOS)  |
                |          |           |
                |          V           |
                +----------------------+Stack Start
                |                      |
                +----------------------+ First address of RAM

#### Drawbacks:
1. ISR Stack is not dynamic - This drawback is mainly for bare metal implementation (RTOS-less) where ISR and Main stack is same. With this limitation application writer should know if stack or heap will be usued more and tweaks the values accordingly.

# Phases:
This feature will be implemented in different phases as follow:

Phase 1 (5.12 Release):
1. Adopt 2-region memory model for Stack and Heap memory.
1. Unify the stack size accross all targets (RTOS: ISR stack - 1K Main thread Stack - 4K; Bare Metal(No RTOS) ISR/Main Stack - 4K)

Phase 2:
1. Heap memory to be dynamic and starts at the end of ZI growing up till end of RAM memory (In case of single RAM bank)
   Heap memory to be dynamic and assigned partial or full RAM bank in case of multiple RAM banks, based on calculation of other RAM regions.
1. ISR Stack to be placed after vectors or before ZI memory section.

Note: Heap split support across multiple RAM banks, can also be achieved post this change. 

# Detailed Design
1. Update tools to set define `MBED_BOOT_STACK_SIZE` from target config option `target.boot-stack-size`
1. Linker Scripts - Update linker scripts for ARM, IAR and GCC toolchain to use MBED_BOOT_STACK_SIZE define for standardizing size of ISR stack.
1. Update __user_setup_stackheap() implementation to adopt 2-region RAM memory model.
__user_setup_stackheap() works with systems where the application starts with a value of sp (r13) that is already correct. To make use of sp(stack base), implement __user_setup_stackheap() to set up r0 (heap base), r2 (heap limit), and r3 (stack limit) (for a two-region model) and return.
Reference http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.kui0099a/armlib_cjagaaha.htm http://www.keil.com/support/man/docs/armlib/armlib_chr1359122863069.htm
1. Modify _sbrk() implementation for GCC to use 2-region memory model

# Tools and configuration changes

1. Target config option "target.boot-stack-size" which is passed to the linker as the define "MBED_BOOT_STACK_SIZE" so the linker can adjust the stack accordingly.
   Boot stack size - the size of ISR and main stack will be 4K as default in targets.json for bare metal (non-RTOS) builds. 
   Boot stack size - the size of ISR stack will be over-written as 1K in `rtos/mbed_lib.json` for RTOS builds.

