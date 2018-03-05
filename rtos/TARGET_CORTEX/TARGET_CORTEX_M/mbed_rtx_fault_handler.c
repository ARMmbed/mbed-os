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
#include "mbed_rtx.h"
#include "mbed_rtx_fault_handler.h"
#include "hal/serial_api.h"

#ifndef MBED_FAULT_HANDLER_DISABLED
//Global for populating the context in exception handler
mbed_fault_context_t mbed_fault_context;

//Structure to capture the context
void fault_print_init(void);
void fault_print_str(char *fmtstr, uint32_t *values);
void hex_to_str(uint32_t value, char *hex_star);
void print_context_info(void);
void print_threads_info(osRtxThread_t *);
void print_thread(osRtxThread_t *thread);
void print_register(char *regtag, uint32_t regval);

#if DEVICE_SERIAL
extern int stdio_uart_inited;
extern serial_t stdio_uart;
#endif

//This is a handler function called from Fault handler to print the error information out.
//This runs in fault context and uses special functions(defined in mbed_rtx_fault_handler.c) to print the information without using C-lib support.
__NO_RETURN void mbed_fault_handler (uint32_t fault_type, void *mbed_fault_context_in, void *osRtxInfoIn)
{
    fault_print_init();
    fault_print_str("\n++ MbedOS Fault Handler ++\n\nFaultType: ",NULL);
        
    switch( fault_type ) {
      case HARD_FAULT_EXCEPTION: 
        fault_print_str("HardFault",NULL); 
        break;
      case MEMMANAGE_FAULT_EXCEPTION: 
        fault_print_str("MemManageFault",NULL); 
        break;
      case BUS_FAULT_EXCEPTION: 
        fault_print_str("BusFault",NULL); 
        break;
      case USAGE_FAULT_EXCEPTION: 
        fault_print_str("UsageFault",NULL); 
        break;
      default: 
        fault_print_str("Unknown Fault",NULL); 
        break;
    }
    fault_print_str("\n\nContext:",NULL);
    print_context_info();
        
    fault_print_str("\n\nThread Info:\nCurrent:",NULL);
    print_thread(((osRtxInfo_t *)osRtxInfoIn)->thread.run.curr);
  
    fault_print_str("\nNext:",NULL);
    print_thread(((osRtxInfo_t *)osRtxInfoIn)->thread.run.next);
    
    fault_print_str("\nWait Threads:",NULL);
    osRtxThread_t *threads = ((osRtxInfo_t *)osRtxInfoIn)->thread.wait_list;
    print_threads_info(threads);
    
    fault_print_str("\nDelay Threads:",NULL);
    threads = ((osRtxInfo_t *)osRtxInfoIn)->thread.delay_list;
    print_threads_info(threads);
    
    fault_print_str("\nIdle Thread:",NULL);
    threads = ((osRtxInfo_t *)osRtxInfoIn)->thread.idle;
    print_threads_info(threads);
    
    fault_print_str("\n\n-- MbedOS Fault Handler --\n\n",NULL);
        
    /* Just spin here, we have already crashed */
    for (;;) {}
}

void print_context_info()
{
    //Context Regs
    fault_print_str( "\nR0   : %" 
                    "\nR1   : %" 
                    "\nR2   : %" 
                    "\nR3   : %" 
                    "\nR4   : %" 
                    "\nR5   : %" 
                    "\nR6   : %" 
                    "\nR7   : %" 
                    "\nR8   : %" 
                    "\nR9   : %" 
                    "\nR10  : %" 
                    "\nR11  : %" 
                    "\nR12  : %" 
                    "\nSP   : %" 
                    "\nLR   : %" 
                    "\nPC   : %" 
                    "\nxPSR : %" 
                    "\nPSP  : %" 
                    "\nMSP  : %", (uint32_t *)&mbed_fault_context);
                       
    //Capture CPUID to get core/cpu info
    fault_print_str("\nCPUID: %",(uint32_t *)&SCB->CPUID);
    
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
    fault_print_str("\nHFSR : %"
                    "\nMMFSR: %"
                    "\nBFSR : %"
                    "\nUFSR : %"
                    "\nDFSR : %"
                    "\nAFSR : %"
                    "\nSHCSR: %",FSR); 
    
    //Print MMFAR only if its valid as indicated by MMFSR
    if(FSR[1] & 0x80) {
        fault_print_str("\nMMFAR: %",(uint32_t *)&SCB->MMFAR); 
    }
    //Print BFAR only if its valid as indicated by BFSR
    if(FSR[2] & 0x80) {
        fault_print_str("\nBFAR : %",(uint32_t *)&SCB->BFAR); 
    }
#endif
    //Print Mode
    if(mbed_fault_context.EXC_RETURN & 0x8) {
        fault_print_str("\nMode : Thread", NULL);
        //Print Priv level in Thread mode - We capture CONTROL reg which reflects the privilege.
        //Note that the CONTROL register captured still reflects the privilege status of the 
        //thread mode eventhough we are in Handler mode by the time we capture it.
        if(mbed_fault_context.CONTROL & 0x1) {
            fault_print_str("\nPriv : User", NULL); 
        } else {
            fault_print_str("\nPriv : Privileged", NULL); 
        }        
    } else {
        fault_print_str("\nMode : Handler", NULL); 
        fault_print_str("\nPriv : Privileged", NULL); 
    }
    //Print Return Stack
    if(mbed_fault_context.EXC_RETURN & 0x4) {
        fault_print_str("\nStack: PSP", NULL); 
    } else {
        fault_print_str("\nStack: MSP", NULL); 
    }
}

/* Prints thread info from a list */
void print_threads_info(osRtxThread_t *threads)
{
    while(threads != NULL) {
        print_thread( threads );
        threads = threads->thread_next;
    }
}

/* Prints info of a thread(using osRtxThread_t struct)*/
void print_thread(osRtxThread_t *thread)
{
    uint32_t data[5];

    data[0]=thread->state;
    data[1]=thread->thread_addr;
    data[2]=thread->stack_size;
    data[3]=(uint32_t)thread->stack_mem;
    data[4]=thread->sp;
    fault_print_str("\nState: % EntryFn: % Stack Size: % Mem: % SP: %", data);
}

/* Initializes std uart for spitting the info out */
void fault_print_init()
{
#if DEVICE_SERIAL
    if (!stdio_uart_inited) {
        serial_init(&stdio_uart, STDIO_UART_TX, STDIO_UART_RX);
    }
#endif    
}

/* Limited print functionality which prints the string out to 
stdout/uart without using stdlib by directly calling serial-api 
and also uses less resources 
The fmtstr contains the format string for printing and for every %
found in that it fetches a uint32 value from values buffer
and prints it in hex format.
*/
void fault_print_str(char *fmtstr, uint32_t *values)
{
#if DEVICE_SERIAL
    int i = 0;
    int idx = 0;
    int vidx = 0;
    char hex_str[9]={0};
        
    while(fmtstr[i] != '\0') {
        if(fmtstr[i]=='%') {
            hex_to_str(values[vidx++],hex_str);
            for(idx=7; idx>=0; idx--) {
                serial_putc(&stdio_uart, hex_str[idx]);
            }
        } else {
            serial_putc(&stdio_uart, fmtstr[i]);
        }
        i++;
    }
#endif    
}

/* Converts a uint32 to hex char string */
void hex_to_str(uint32_t value, char *hex_str)
{
    char hex_char_map[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    int i = 0;
    
    //Return without converting if hex_str is not provided
    if(hex_str == NULL) return;
        
    for(i=7; i>=0; i--) {
        hex_str[i] = hex_char_map[(value & (0xf << (i * 4))) >> (i * 4)];
    }
}

#endif //MBED_FAULT_HANDLER_SUPPORT
