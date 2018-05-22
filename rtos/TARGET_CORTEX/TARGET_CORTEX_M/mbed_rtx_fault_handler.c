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
#include "platform/mbed_error_report.h"

#ifndef MBED_FAULT_HANDLER_DISABLED
#include "mbed_rtx_fault_handler.h"

//Functions Prototypes
void print_context_info(void);

//Global for populating the context in exception handler
mbed_fault_context_t mbed_fault_context;

//This is a handler function called from Fault handler to print the error information out.
//This runs in fault context and uses special functions(defined in mbed_rtx_fault_handler.c) to print the information without using C-lib support.
__NO_RETURN void mbed_fault_handler (uint32_t fault_type, void *mbed_fault_context_in, void *osRtxInfoIn)
{
    mbed_error_status_t faultStatus = MBED_SUCCESS;
    
    mbed_error_print("\n++ MbedOS Fault Handler ++\n\nFaultType: ",NULL);
        
    switch( fault_type ) {
      case HARD_FAULT_EXCEPTION: 
        mbed_error_print("HardFault",NULL);
        faultStatus = MBED_ERROR_HARDFAULT_EXCEPTION;      
        break;
      case MEMMANAGE_FAULT_EXCEPTION: 
        mbed_error_print("MemManageFault",NULL); 
        faultStatus = MBED_ERROR_MEMMANAGE_EXCEPTION;
        break;
      case BUS_FAULT_EXCEPTION: 
        mbed_error_print("BusFault",NULL); 
        faultStatus = MBED_ERROR_BUSFAULT_EXCEPTION;
        break;
      case USAGE_FAULT_EXCEPTION: 
        mbed_error_print("UsageFault",NULL); 
        faultStatus = MBED_ERROR_USAGEFAULT_EXCEPTION;
        break;
      default: 
        mbed_error_print("Unknown Fault",NULL); 
        faultStatus = MBED_ERROR_UNKNOWN;
        break;
    }
    mbed_error_print("\n\nContext:",NULL);
    print_context_info();
        
    mbed_error_print("\n\nThreads Info:\nCurrent:",NULL);
    print_thread(((osRtxInfo_t *)osRtxInfoIn)->thread.run.curr);
  
    mbed_error_print("\nNext:",NULL);
    print_thread(((osRtxInfo_t *)osRtxInfoIn)->thread.run.next);
    
    mbed_error_print("\nWait:",NULL);
    osRtxThread_t *threads = ((osRtxInfo_t *)osRtxInfoIn)->thread.wait_list;
    print_threads_info(threads);
    
    mbed_error_print("\nDelay:",NULL);
    threads = ((osRtxInfo_t *)osRtxInfoIn)->thread.delay_list;
    print_threads_info(threads);
    
    mbed_error_print("\nIdle:",NULL);
    threads = ((osRtxInfo_t *)osRtxInfoIn)->thread.idle;
    print_threads_info(threads);
    
    mbed_error_print("\n\n-- MbedOS Fault Handler --\n\n",NULL);
    
    //Now call mbed_error, to log the error and halt the system
    mbed_error( MAKE_ERROR( MODULE_UNKNOWN, faultStatus ), "System encountered an unrecoverable fault excaption, halting system.", mbed_fault_context.PC_reg, NULL, 0 );
    
    /* In case we return, just spin here, we have already crashed */
    for (;;) { 
        __WFI(); 
    }
}

void print_context_info(void)
{
    //Context Regs
    mbed_error_print("\nR0   : %x" 
                    "\nR1   : %x" 
                    "\nR2   : %x" 
                    "\nR3   : %x" 
                    "\nR4   : %x" 
                    "\nR5   : %x" 
                    "\nR6   : %x" 
                    "\nR7   : %x" 
                    "\nR8   : %x" 
                    "\nR9   : %x" 
                    "\nR10  : %x" 
                    "\nR11  : %x" 
                    "\nR12  : %x" 
                    "\nSP   : %x" 
                    "\nLR   : %x" 
                    "\nPC   : %x" 
                    "\nxPSR : %x" 
                    "\nPSP  : %x" 
                    "\nMSP  : %x", (uint32_t *)&mbed_fault_context);
                       
    //Capture CPUID to get core/cpu info
    mbed_error_print("\nCPUID: %x",(uint32_t *)&SCB->CPUID);
    
#if !defined(TARGET_M0) && !defined(TARGET_M0P)
    //Capture fault information registers to infer the cause of exception
    uint32_t FSR[7] = {0};
    
    FSR[0] = SCB->HFSR;
    //Split/Capture CFSR into MMFSR, BFSR, UFSR
    FSR[1] = 0xFF & SCB->CFSR;//MMFSR
    FSR[2] = (0xFF00 & SCB->CFSR) >> 8;//BFSR
    FSR[3] = (0xFFFF0000 & SCB->CFSR) >> 16;//UFSR
    FSR[4] = SCB->DFSR;
    FSR[5] = SCB->AFSR;
    FSR[6] = SCB->SHCSR;
    mbed_error_print("\nHFSR : %x"
                    "\nMMFSR: %x"
                    "\nBFSR : %x"
                    "\nUFSR : %x"
                    "\nDFSR : %x"
                    "\nAFSR : %x"
                    "\nSHCSR: %x",FSR); 
    
    //Print MMFAR only if its valid as indicated by MMFSR
    if(FSR[1] & 0x80) {
        mbed_error_print("\nMMFAR: %x",(uint32_t *)&SCB->MMFAR); 
    }
    //Print BFAR only if its valid as indicated by BFSR
    if(FSR[2] & 0x80) {
        mbed_error_print("\nBFAR : %x",(uint32_t *)&SCB->BFAR); 
    }
#endif
    //Print Mode
    if(mbed_fault_context.EXC_RETURN & 0x8) {
        mbed_error_print("\nMode : Thread", NULL);
        //Print Priv level in Thread mode - We capture CONTROL reg which reflects the privilege.
        //Note that the CONTROL register captured still reflects the privilege status of the 
        //thread mode eventhough we are in Handler mode by the time we capture it.
        if(mbed_fault_context.CONTROL & 0x1) {
            mbed_error_print("\nPriv : User", NULL); 
        } else {
            mbed_error_print("\nPriv : Privileged", NULL); 
        }        
    } else {
        mbed_error_print("\nMode : Handler", NULL); 
        mbed_error_print("\nPriv : Privileged", NULL); 
    }
    //Print Return Stack
    if(mbed_fault_context.EXC_RETURN & 0x4) {
        mbed_error_print("\nStack: PSP", NULL); 
    } else {
        mbed_error_print("\nStack: MSP", NULL); 
    }
}

#endif //MBED_FAULT_HANDLER_SUPPORT
