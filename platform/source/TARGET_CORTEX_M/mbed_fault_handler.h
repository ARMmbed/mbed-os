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

#ifndef MBED_FAULT_HANDLER_H
#define MBED_FAULT_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

//Fault context struct
//WARNING: DO NOT CHANGE THIS STRUCT WITHOUT MAKING CORRESPONDING CHANGES in except.S files.
//Offset of these registers are used by fault handler in except.S
typedef struct {
    uint32_t R0_reg;
    uint32_t R1_reg;
    uint32_t R2_reg;
    uint32_t R3_reg;
    uint32_t R4_reg;
    uint32_t R5_reg;
    uint32_t R6_reg;
    uint32_t R7_reg;
    uint32_t R8_reg;
    uint32_t R9_reg;
    uint32_t R10_reg;
    uint32_t R11_reg;
    uint32_t R12_reg;
    uint32_t SP_reg;
    uint32_t LR_reg;
    uint32_t PC_reg;
    uint32_t xPSR;
    uint32_t PSP;
    uint32_t MSP;
    uint32_t EXC_RETURN;
    uint32_t CONTROL;
} mbed_fault_context_t;

//Fault type definitions
//WARNING: DO NOT CHANGE THESE VALUES WITHOUT MAKING CORRESPONDING CHANGES in except.S files.
#define HARD_FAULT_EXCEPTION       (0x10) //Keep some gap between values for any future insertion/expansion
#define MEMMANAGE_FAULT_EXCEPTION  (0x20)
#define BUS_FAULT_EXCEPTION        (0x30)
#define USAGE_FAULT_EXCEPTION      (0x40)

//This is a handler function called from Fault handler to print the error information out.
//This runs in fault context and uses special functions(defined in mbed_fault_handler.c) to print the information without using C-lib support.
void mbed_fault_handler(uint32_t fault_type, const mbed_fault_context_t *mbed_fault_context_in);

/**
 * Call this function to retrieve the fault context after a fatal exception which triggered a system reboot. The function retrieves the fault context stored in crash-report ram area which is preserved over reboot.
 * @param  fault_context        Pointer to mbed_fault_context_t struct allocated by the caller. This is the mbed_fault_context_t info captured as part of the fatal exception which triggered the reboot.
 * @return                      0 or MBED_SUCCESS on success.
 *                              MBED_ERROR_INVALID_ARGUMENT in case of invalid error_info pointer
 *                              MBED_ERROR_ITEM_NOT_FOUND if no reboot context is currently captured by teh system
 *
 */
mbed_error_status_t mbed_get_reboot_fault_context(mbed_fault_context_t *fault_context);

#ifdef __cplusplus
}
#endif

#endif
