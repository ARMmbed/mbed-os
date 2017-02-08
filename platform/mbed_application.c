/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#if MBED_APPLICATION_SUPPORT

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

    sp = *((void**)address + 0);
    pc = *((void**)address + 1);
    start_new_application(sp, pc);
}

static void powerdown_nvic()
{
    int isr_count;
    int i;
    int j;

    isr_count = (SCnSCB->ICTR & SCnSCB_ICTR_INTLINESNUM_Msk) >> SCnSCB_ICTR_INTLINESNUM_Pos;
    for (i = 0; i < isr_count; i++) {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
        for (j = 0; j < 8; j++) {
            NVIC->IP[i * 8 + j] = 0x00000000;
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
    for (i = 0; i < 12; i++) {
#if defined(__CORTEX_M7)
        SCB->SHPR[i] = 0x00;
#else
        SCB->SHP[i] = 0x00;
#endif
    }
    SCB->SHCSR = 0x00000000;
    SCB->CFSR = 0xFFFFFFFF;
    SCB->HFSR = SCB_HFSR_DEBUGEVT_Msk | SCB_HFSR_FORCED_Msk | SCB_HFSR_VECTTBL_Msk;
    SCB->DFSR = SCB_DFSR_EXTERNAL_Msk | SCB_DFSR_VCATCH_Msk |
                SCB_DFSR_DWTTRAP_Msk | SCB_DFSR_BKPT_Msk | SCB_DFSR_HALTED_Msk;
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
    MOV R2, #0
    MSR CONTROL, R2         // Switch to main stack
    MOV SP, R0
    MSR PRIMASK, R2         // Enable interrupts
    BX R1
}

#elif defined (__GNUC__) || defined (__ICCARM__)

void start_new_application(void *sp, void *pc)
{
    __asm volatile (
        "mov    r2, #0      \n"
        "msr    control, r2 \n" // Switch to main stack
        "mov    sp, %0      \n"
        "msr    primask, r2 \n" // Enable interrupts
        "bx     %1          \n"
        :
        : "l" (sp), "l" (pc)
        : "r2", "cc", "memory"
    );
}

#else

#error "Unsupported toolchain"

#endif

#endif /* MBED_APPLICATION_SUPPORT */
