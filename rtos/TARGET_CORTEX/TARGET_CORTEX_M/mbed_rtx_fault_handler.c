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

#include "rtx_os.h"
#include "device.h"
#include "platform/mbed_error.h"
#include "platform/mbed_interface.h"
#include "hal/serial_api.h"

#ifndef MBED_FAULT_HANDLER_DISABLED
#include "mbed_rtx_fault_handler.h"

//Functions Prototypes
void print_context_info(void);

//Global for populating the context in exception handler
mbed_fault_context_t mbed_fault_context;

//This is a handler function called from Fault handler to print the error information out.
//This runs in fault context and uses special functions(defined in mbed_rtx_fault_handler.c) to print the information without using C-lib support.
void mbed_fault_handler (uint32_t fault_type, void *mbed_fault_context_in, void *osRtxInfoIn)
{
    mbed_error_status_t faultStatus = MBED_SUCCESS;
    
    mbed_error_printf("\n++ MbedOS Fault Handler ++\n\nFaultType: ");
        
    switch( fault_type ) {
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
    
    //Now call mbed_error, to log the error and halt the system
    mbed_error( faultStatus, "Unrecoverable fault excaption.", mbed_fault_context.PC_reg, NULL, 0 );
    
}

MBED_NOINLINE void print_context_info(void)
{
    //Context Regs
    for(int i=0;i<13;i++) {
        mbed_error_printf("\nR%-4d: %08X", i, ((uint32_t *)&mbed_fault_context)[i]);  
    }
        
    mbed_error_printf("\nSP   : %08X" 
                      "\nLR   : %08X" 
                      "\nPC   : %08X" 
                      "\nxPSR : %08X" 
                      "\nPSP  : %08X" 
                      "\nMSP  : %08X", mbed_fault_context.SP_reg, mbed_fault_context.LR_reg, mbed_fault_context.PC_reg, 
                                     mbed_fault_context.xPSR, mbed_fault_context.PSP, mbed_fault_context.MSP );
                       
    //Capture CPUID to get core/cpu info
    mbed_error_printf("\nCPUID: %08X", SCB->CPUID);
    
#if !defined(TARGET_M0) && !defined(TARGET_M0P)
    //Capture fault information registers to infer the cause of exception
    mbed_error_printf("\nHFSR : %08X"
                    "\nMMFSR: %08X"
                    "\nBFSR : %08X"
                    "\nUFSR : %08X"
                    "\nDFSR : %08X"
                    "\nAFSR : %08X"  ////Split/Capture CFSR into MMFSR, BFSR, UFSR
                    ,SCB->HFSR, (0xFF & SCB->CFSR), ((0xFF00 & SCB->CFSR) >> 8), ((0xFFFF0000 & SCB->CFSR) >> 16), SCB->DFSR, SCB->AFSR ); 
    
    //Print MMFAR only if its valid as indicated by MMFSR
    if ((0xFF & SCB->CFSR) & 0x80) {
        mbed_error_printf("\nMMFAR: %08X",SCB->MMFAR); 
    }
    //Print BFAR only if its valid as indicated by BFSR
    if (((0xFF00 & SCB->CFSR) >> 8) & 0x80) {
        mbed_error_printf("\nBFAR : %08X",SCB->BFAR); 
    }
#endif
    
    //Print Mode
    if (mbed_fault_context.EXC_RETURN & 0x8) {
        mbed_error_printf("\nMode : Thread");
        //Print Priv level in Thread mode - We capture CONTROL reg which reflects the privilege.
        //Note that the CONTROL register captured still reflects the privilege status of the 
        //thread mode eventhough we are in Handler mode by the time we capture it.
        if(mbed_fault_context.CONTROL & 0x1) {
            mbed_error_printf("\nPriv : User"); 
        } else {
            mbed_error_printf("\nPriv : Privileged"); 
        }        
    } else {
        mbed_error_printf("\nMode : Handler"); 
        mbed_error_printf("\nPriv : Privileged"); 
    }
    //Print Return Stack
    if (mbed_fault_context.EXC_RETURN & 0x4) {
        mbed_error_printf("\nStack: PSP"); 
    } else {
        mbed_error_printf("\nStack: MSP"); 
    }
}

#endif //MBED_FAULT_HANDLER_SUPPORT
