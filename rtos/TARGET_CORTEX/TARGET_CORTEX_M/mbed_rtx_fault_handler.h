/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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

//Fault context struct
//WARNING: DO NOT CHANGE THIS STRUCT WITHOUT MAKING CORRESPONDING CHANGES in except.S files.
//Offset of these registers are used by fault handler in except.S
typedef struct {
  uint32_t R0;
  uint32_t R1;
  uint32_t R2;
  uint32_t R3;
  uint32_t R4;
  uint32_t R5;
  uint32_t R6;
  uint32_t R7;
  uint32_t R8;
  uint32_t R9;
  uint32_t R10;
  uint32_t R11;
  uint32_t R12;
  uint32_t SP;
  uint32_t LR;
  uint32_t PC;
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
//This runs in fault context and uses special functions(defined in mbed_rtx_fault_handler.c) to print the information without using C-lib support.
__NO_RETURN void mbed_fault_handler (uint32_t fault_type, void *mbed_fault_context_in, void *osRtxInfoIn);

