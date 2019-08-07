/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <stdarg.h>
#include "device.h"
#include "platform/mbed_application.h"
#include "platform/mbed_mpu_mgmt.h"

#if MBED_APPLICATION_SUPPORT

#if defined(__CORTEX_A9)

static void powerdown_gic(void);

void mbed_start_application(uintptr_t address)
{
    __disable_irq();
    powerdown_gic();
    __enable_irq();
    ((void(*)())address)();
}

static void powerdown_gic()
{
    int i;
    int j;

    for (i = 0; i < 32; i++) {
        GICDistributor->ICENABLER[i] = 0xFFFFFFFF;
        GICDistributor->ICPENDR[i] = 0xFFFFFFFF;
        if (i < 4) {
            GICDistributor->CPENDSGIR[i] = 0xFFFFFFFF;
        }
        for (j = 0; j < 8; j++) {
            GICDistributor->IPRIORITYR[i * 8 + j] = 0x00000000;
        }
    }
}

#else

static void powerdown_nvic(void);
static void powerdown_scb(uint32_t vtor);
static void start_new_application(void *sp, void *pc);

void mbed_start_application(uintptr_t address)
{
    void *sp;
    void *pc;

    // Interrupts are re-enabled in start_new_application
    __disable_irq();

    SysTick->CTRL = 0x00000000;
    powerdown_nvic();
    powerdown_scb(address);
    mbed_mpu_manager_deinit();

#ifdef MBED_DEBUG
    // Configs to make debugging easier
#ifdef SCnSCB_ACTLR_DISDEFWBUF_Msk
    // Disable write buffer to make BusFaults (eg write to ROM via NULL pointer) precise.
    // Possible on Cortex-M3 and M4, not on M0, M7 or M33.
    // Would be less necessary if ROM was write-protected in MPU to give a
    // precise MemManage exception.
    SCnSCB->ACTLR |= SCnSCB_ACTLR_DISDEFWBUF_Msk;
#endif
#endif

    sp = *((void **)address + 0);
    pc = *((void **)address + 1);
    start_new_application(sp, pc);
}

static void powerdown_nvic()
{
    int i;
    int j;
    int isr_groups_32;

#if defined(__CORTEX_M23)
    // M23 doesn't support ICTR and supports up to 240 external interrupts.
    isr_groups_32 = 8;
#elif defined(__CORTEX_M0PLUS)
    isr_groups_32 = 1;
#else
    isr_groups_32 = ((SCnSCB->ICTR & SCnSCB_ICTR_INTLINESNUM_Msk) >> SCnSCB_ICTR_INTLINESNUM_Pos) + 1;
#endif

    for (i = 0; i < isr_groups_32; i++) {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
        for (j = 0; j < 8; j++) {
#if defined(__CORTEX_M23) || defined(__CORTEX_M33)
            NVIC->IPR[i * 8 + j] = 0x00000000;
#else
            NVIC->IP[i * 8 + j] = 0x00000000;
#endif
        }
    }
}

static void powerdown_scb(uint32_t vtor)
{
    int i;

    // SCB->CPUID   - Read only CPU ID register
    SCB->ICSR = SCB_ICSR_PENDSVCLR_Msk | SCB_ICSR_PENDSTCLR_Msk;
    SCB->VTOR = vtor;
    SCB->AIRCR = 0x05FA | 0x0000;
    SCB->SCR = 0x00000000;
    // SCB->CCR     - Implementation defined value
    int num_pri_reg; // Number of priority registers
#if defined(__CORTEX_M0PLUS) || defined(__CORTEX_M23)
    num_pri_reg = 2;
#else
    num_pri_reg = 12;
#endif
    for (i = 0; i < num_pri_reg; i++) {
#if defined(__CORTEX_M7) || defined(__CORTEX_M23) || defined(__CORTEX_M33)
        SCB->SHPR[i] = 0x00;
#else
        SCB->SHP[i] = 0x00;
#endif
    }
    SCB->SHCSR = 0x00000000;
#if defined(__CORTEX_M23) || defined(__CORTEX_M0PLUS)
#else
    SCB->CFSR = 0xFFFFFFFF;
    SCB->HFSR = SCB_HFSR_DEBUGEVT_Msk | SCB_HFSR_FORCED_Msk | SCB_HFSR_VECTTBL_Msk;
    SCB->DFSR = SCB_DFSR_EXTERNAL_Msk | SCB_DFSR_VCATCH_Msk |
                SCB_DFSR_DWTTRAP_Msk | SCB_DFSR_BKPT_Msk | SCB_DFSR_HALTED_Msk;
#endif
    // SCB->MMFAR   - Implementation defined value
    // SCB->BFAR    - Implementation defined value
    // SCB->AFSR    - Implementation defined value
    // SCB->PFR     - Read only processor feature register
    // SCB->DFR     - Read only debug feature registers
    // SCB->ADR     - Read only auxiliary feature registers
    // SCB->MMFR    - Read only memory model feature registers
    // SCB->ISAR    - Read only instruction set attribute registers
    // SCB->CPACR   - Implementation defined value
}

#if defined (__CC_ARM)

__asm static void start_new_application(void *sp, void *pc)
{
    MOVS R2, #0
    MSR CONTROL, R2         // Switch to main stack
    MOV SP, R0
    MSR PRIMASK, R2         // Enable interrupts
    BX R1
}

#elif defined (__GNUC__) || defined (__ICCARM__)

void start_new_application(void *sp, void *pc)
{
    __asm volatile(
        "movs   r2, #0      \n"
        "msr    control, r2 \n" // Switch to main stack
        "mov    sp, %0      \n"
        "msr    primask, r2 \n" // Enable interrupts
        "bx     %1          \n"
        :
        : "l"(sp), "l"(pc)
        : "r2", "cc", "memory"
    );
}

#else

#error "Unsupported toolchain"

#endif

#endif

#endif /* MBED_APPLICATION_SUPPORT */
