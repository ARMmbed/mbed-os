/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "mbed_toolchain.h"
#include <stdlib.h>
#include <stdint.h>
#include "cmsis.h"

/* This startup is for mbed 2 baremetal. There is no config for RTOS for mbed 2,
 * therefore we protect this file with MBED_CONF_RTOS_PRESENT
 * Note: The new consolidated started for mbed OS is in rtos/mbed_boot code file.
 */
#if !defined(MBED_CONF_RTOS_PRESENT)

/* mbed_main is a function that is called before main()
 * mbed_sdk_init() is also a function that is called before main(), but unlike
 * mbed_main(), it is not meant for user code, but for the SDK itself to perform
 * initializations before main() is called.
 */
MBED_WEAK void mbed_main(void) 
{

}

/* This function can be implemented by the target to perform higher level target initialization
 */
MBED_WEAK void mbed_sdk_init(void) 
{

}

MBED_WEAK void software_init_hook_rtos()
{
    // Nothing by default
}

void mbed_copy_nvic(void)
{
    /* If vector address in RAM is defined, copy and switch to dynamic vectors. Exceptions for M0 which doesn't have
    VTOR register and for A9 for which CMSIS doesn't define NVIC_SetVector; in both cases target code is
    responsible for correctly handling the vectors.
    */
#if !defined(__CORTEX_M0) && !defined(__CORTEX_A9)
#ifdef NVIC_RAM_VECTOR_ADDRESS
    uint32_t *old_vectors = (uint32_t *)SCB->VTOR;
    uint32_t *vectors = (uint32_t*)NVIC_RAM_VECTOR_ADDRESS;
    for (int i = 0; i < NVIC_NUM_VECTORS; i++) {
        vectors[i] = old_vectors[i];
    }
    SCB->VTOR = (uint32_t)NVIC_RAM_VECTOR_ADDRESS;
#endif /* NVIC_RAM_VECTOR_ADDRESS */
#endif /* !defined(__CORTEX_M0) && !defined(__CORTEX_A9) */
}

/* Toolchain specific main code */

#if defined (__CC_ARM)

int $Super$$main(void);

int $Sub$$main(void) 
{
    mbed_main();
    return $Super$$main();
}

void _platform_post_stackheap_init(void) 
{
    mbed_copy_nvic();
    mbed_sdk_init();
}

#elif defined (__GNUC__) 

extern int __real_main(void);

void software_init_hook(void)
{
    mbed_copy_nvic();
    mbed_sdk_init();
    software_init_hook_rtos();
}


int __wrap_main(void) 
{
    mbed_main();
    return __real_main();
}

#elif defined (__ICCARM__)

int __low_level_init(void)
{
  mbed_copy_nvic();
  return 1;
}

#endif

#endif
