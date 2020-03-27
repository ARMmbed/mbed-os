/* Copyright (c) 2017-2018 ARM Limited
 *
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

#if !defined(COMPONENT_PSA_SRV_IPC)
#error [NOT_SUPPORTED] Test supported only on PSA targets
#else

#if (defined(__ARMCC_VERSION) || defined( __ICCARM__ ))
#error [NOT_SUPPORTED] this test is supported on GCC only
#else

#if DOMAIN_NS == 1
#error [NOT_SUPPORTED] Cannot run on M23/M33 core as SecureFault is implemented in secure-side and cant be remapped
#else

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "cmsis.h"
#include "spm_api.h"
#include <stdlib.h>
#include "fault_functions.h"

using namespace utest::v1;


#define HARDFAULT_IRQn   ((IRQn_Type)-13)
#define EXC_RETURN_RETURN_STACK_MSK ((uint32_t)(0x00000004))
#define PC_INDEX_IN_STACK_FRAME 6

volatile uint32_t fault_occurred;
uint32_t real_hard_fault_handler;

__STATIC_FORCEINLINE uint32_t __get_LR(void)
{
    uint32_t result;

    __ASM volatile("MOV %0, lr" : "=r"(result));
    return (result);
}

void do_nothing(void)
{
    __NOP();
}

static void hard_fault_handler_test()
{
    fault_occurred++;
    // LR is set EXC_RETURN
    // lowest bits identify PSP vs MSP stack used for stacking
    uint32_t lr = __get_LR();
    uint32_t sp;

    if (lr & EXC_RETURN_RETURN_STACK_MSK) {
        sp = __get_PSP();
    } else {
        sp = __get_MSP();
    }

    // Overwrite return address.
    // Fake return to a our special function since current
    // instruction under test will always fail due to memory protection
    ((uint32_t *)sp)[PC_INDEX_IN_STACK_FRAME] = (uint32_t)do_nothing;
}

__attribute__((naked)) void call_mem(uint32_t addr)
{
    // Only first instruction will be executed in positive flow,
    // since exception will be generated for invalid memory access.
    // Other instructions are for calling do_nothing function according to AAPCS.
    __ASM(
        "LDR     r3, [r0]\n"
        "BX      lr\n"
    );
}

static void test_memory(uint32_t addr, uint32_t expected_fatal_count)
{
    call_mem(addr);
    // Although call_mem is a "naked" function, it is called using AAPCS.
    // Thus we can assume LR will point to next instruction, and caller save registers are backed up
    TEST_ASSERT_EQUAL(expected_fatal_count, fault_occurred);
}

static void secure_ram_fault_test(void)
{
    test_memory(PSA_SECURE_RAM_START, 1);
}

static void secure_flash_fault_test(void)
{
    test_memory(PSA_SECURE_ROM_START, 1);
}

static void non_secure_ram_fault_test(void)
{
    test_memory(PSA_NON_SECURE_RAM_START, 0);
}

static void non_secure_flash_fault_test(void)
{
    test_memory(PSA_NON_SECURE_ROM_START, 0);
}

utest::v1::status_t fault_override_setup(const Case *const source, const size_t index_of_case)
{
    // Save old hard fault handler and replace it with a new one
    // NOTE: only works when VTOR is set to RAM
    real_hard_fault_handler = NVIC_GetVector(HARDFAULT_IRQn);
    NVIC_SetVector(HARDFAULT_IRQn, (uint32_t)&hard_fault_handler_test);
    fault_occurred = 0;

    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t fault_override_teardown(const Case *const source, const size_t passed, const size_t failed,
                                            const failure_t reason)
{
    // Restore real hard fault handler
    NVIC_SetVector(HARDFAULT_IRQn, real_hard_fault_handler);

    return greentea_case_teardown_handler(source, passed, failed, reason);
}

Case cases[] = {
    Case("SPM - Access non-secure RAM",   fault_override_setup, non_secure_ram_fault_test, fault_override_teardown),
    Case("SPM - Access non-secure Flash", fault_override_setup, non_secure_flash_fault_test, fault_override_teardown),
    Case("SPM - Access secure RAM",       fault_override_setup, secure_ram_fault_test, fault_override_teardown),
    Case("SPM - Access secure Flash",     fault_override_setup, secure_flash_fault_test, fault_override_teardown)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif // DOMAIN_NS == 1
#endif // (defined(__ARMCC_VERSION) || defined( __ICCARM__ ))
#endif // !defined(COMPONENT_PSA_SRV_IPC)
