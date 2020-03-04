/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>
#include <string.h>

#include "device.h"
#include "mbed_atomic.h"
#include "mbed_error.h"
#include "mbed_interface.h"
#include "mbed_crash_data_offsets.h"

#ifndef MBED_FAULT_HANDLER_DISABLED
#include "mbed_fault_handler.h"

//Functions Prototypes
void print_context_info(void);

#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
//Global for populating the context in exception handler
mbed_fault_context_t *const mbed_fault_context = (mbed_fault_context_t *)(FAULT_CONTEXT_LOCATION);
#else
mbed_fault_context_t fault_context;
mbed_fault_context_t *const mbed_fault_context = &fault_context;
#endif

extern bool mbed_error_in_progress;

//This is a handler function called from Fault handler to print the error information out.
//This runs in fault context and uses special functions(defined in mbed_rtx_fault_handler.c) to print the information without using C-lib support.
void mbed_fault_handler(uint32_t fault_type, const mbed_fault_context_t *mbed_fault_context_in)
{
    mbed_error_status_t faultStatus = MBED_SUCCESS;

    /* Need to set the flag to ensure prints do not trigger a "mutex in ISR" trap
     * if they're first prints since boot and we have to init the I/O system.
     */
    if (!core_util_atomic_exchange_bool(&mbed_error_in_progress, true)) {
        mbed_error_printf("\n++ MbedOS Fault Handler ++\n\nFaultType: ");

        switch (fault_type) {
            case MEMMANAGE_FAULT_EXCEPTION:
                mbed_error_printf("MemManageFault");
                faultStatus = MBED_ERROR_MEMMANAGE_EXCEPTION;
                break;

            case BUS_FAULT_EXCEPTION:
                mbed_error_printf("BusFault");
                faultStatus = MBED_ERROR_BUSFAULT_EXCEPTION;
                break;

            case USAGE_FAULT_EXCEPTION:
                mbed_error_printf("UsageFault");
                faultStatus = MBED_ERROR_USAGEFAULT_EXCEPTION;
                break;

            //There is no way we can hit this code without getting an exception, so we have the default treated like hardfault
            case HARD_FAULT_EXCEPTION:
            default:
                mbed_error_printf("HardFault");
                faultStatus = MBED_ERROR_HARDFAULT_EXCEPTION;
                break;
        }
        mbed_error_printf("\n\nContext:");
        print_context_info();

        mbed_error_printf("\n\n-- MbedOS Fault Handler --\n\n");
        core_util_atomic_store_bool(&mbed_error_in_progress, false);
    }

    //Now call mbed_error, to log the error and halt the system
    mbed_error(faultStatus, "Fault exception", (unsigned int)mbed_fault_context_in, NULL, 0);

}

MBED_NOINLINE void print_context_info(void)
{
    //Context Regs
    for (int i = 0; i < 13; i++) {
        mbed_error_printf("\nR%-4d: %08" PRIX32, i, ((uint32_t *)(mbed_fault_context))[i]);
    }

    mbed_error_printf("\nSP   : %08" PRIX32
                      "\nLR   : %08" PRIX32
                      "\nPC   : %08" PRIX32
                      "\nxPSR : %08" PRIX32
                      "\nPSP  : %08" PRIX32
                      "\nMSP  : %08" PRIX32, mbed_fault_context->SP_reg, mbed_fault_context->LR_reg, mbed_fault_context->PC_reg,
                      mbed_fault_context->xPSR, mbed_fault_context->PSP, mbed_fault_context->MSP);

    //Capture CPUID to get core/cpu info
    mbed_error_printf("\nCPUID: %08" PRIX32, SCB->CPUID);

#if !defined(TARGET_M0) && !defined(TARGET_M0P) && !defined(TARGET_M23)
    //Capture fault information registers to infer the cause of exception
    mbed_error_printf("\nHFSR : %08" PRIX32
                      "\nMMFSR: %08" PRIX32
                      "\nBFSR : %08" PRIX32
                      "\nUFSR : %08" PRIX32
                      "\nDFSR : %08" PRIX32
                      "\nAFSR : %08" PRIX32  ////Split/Capture CFSR into MMFSR, BFSR, UFSR
                      , SCB->HFSR, (0xFF & SCB->CFSR), ((0xFF00 & SCB->CFSR) >> 8), ((0xFFFF0000 & SCB->CFSR) >> 16), SCB->DFSR, SCB->AFSR);

    //Print MMFAR only if its valid as indicated by MMFSR
    if ((0xFF & SCB->CFSR) & 0x80) {
        mbed_error_printf("\nMMFAR: %08" PRIX32, SCB->MMFAR);
    }
    //Print BFAR only if its valid as indicated by BFSR
    if (((0xFF00 & SCB->CFSR) >> 8) & 0x80) {
        mbed_error_printf("\nBFAR : %08" PRIX32, SCB->BFAR);
    }
#endif

    //Print Mode
    if (mbed_fault_context->EXC_RETURN & 0x8) {
        mbed_error_printf("\nMode : Thread");
        //Print Priv level in Thread mode - We capture CONTROL reg which reflects the privilege.
        //Note that the CONTROL register captured still reflects the privilege status of the
        //thread mode eventhough we are in Handler mode by the time we capture it.
        if (mbed_fault_context->CONTROL & 0x1) {
            mbed_error_printf("\nPriv : User");
        } else {
            mbed_error_printf("\nPriv : Privileged");
        }
    } else {
        mbed_error_printf("\nMode : Handler");
        mbed_error_printf("\nPriv : Privileged");
    }
    //Print Return Stack
    if (mbed_fault_context->EXC_RETURN & 0x4) {
        mbed_error_printf("\nStack: PSP");
    } else {
        mbed_error_printf("\nStack: MSP");
    }
}

mbed_error_status_t mbed_get_reboot_fault_context(mbed_fault_context_t *fault_context)
{
    mbed_error_status_t status = MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_ITEM_NOT_FOUND);
#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
    if (fault_context == NULL) {
        return MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_INVALID_ARGUMENT);
    }
    memcpy(fault_context, mbed_fault_context, sizeof(mbed_fault_context_t));
    status = MBED_SUCCESS;
#endif
    return status;
}

#endif //MBED_FAULT_HANDLER_SUPPORT
