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
#include "hal/serial_api.h"
#include "hal/itm_api.h"

#ifndef MBED_FAULT_HANDLER_DISABLED
#include "mbed_rtx_fault_handler.h"

#ifdef DEVICE_SERIAL
extern int stdio_uart_inited;
extern serial_t stdio_uart;
#endif

//Functions Prototypes
void print_context_info(void);

//Global for populating the context in exception handler
mbed_fault_context_t mbed_fault_context;

/* Converts a uint32 to hex char string */
static void value_to_hex_str(uint32_t value, char *hex_str)
{
    char hex_char_map[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    int i = 0;
    
    //Return without converting if hex_str is not provided
    if(hex_str == NULL) return;
        
    for(i=7; i>=0; i--) {
        hex_str[i] = hex_char_map[(value & (0xf << (i * 4))) >> (i * 4)];
    }
}

static void fault_print_init(void)
{
#if DEVICE_SERIAL
    /* Initializes std uart if not init-ed yet */
    if (!stdio_uart_inited) {
        serial_init(&stdio_uart, STDIO_UART_TX, STDIO_UART_RX);
    }
#endif

#if DEVICE_ITM
    /*Initialize ITM interfaces*/
    mbed_itm_init();
#endif
}

static void fault_putc(char ch)
{
#if DEVICE_SERIAL
    serial_putc(&stdio_uart, ch);
#endif

#if DEVICE_ITM
    /*Initialize ITM interfaces*/
    mbed_itm_send(ITM_PORT_SWO, ch);
#endif
}

/* Limited print functionality which prints the string out to 
stdout/uart without using stdlib by directly calling serial-api 
and also uses less resources 
The fmtstr contains the format string for printing and for every %
found in that it fetches a uint32 value from values buffer
and prints it in hex format.
*/
static void fault_print(char *fmtstr, uint32_t *values)
{
#if DEVICE_SERIAL || DEVICE_ITM
    int i = 0;
    int idx = 0;
    int vidx = 0;
    char num_str[9]={0};
    char *str=NULL;
    
    //Init error reporting interfaces
    fault_print_init();
        
    while(fmtstr[i] != '\0') {
        if(fmtstr[i]=='%') {
            i++;
            if(fmtstr[i]=='x') {
                //print the number in hex format
                value_to_hex_str(values[vidx++],num_str);
                for(idx=7; idx>=0; idx--) {
                    fault_putc(num_str[idx]);
                }
            }
            else if(fmtstr[i]=='s') {
                //print the string
                str = (char *)((uint32_t)values[vidx++]);
                while(*str != '\0') {
                    fault_putc(*str);
                    str++;
                }
                str = NULL;
            } else {
                //Do not handle any other % formatting and keep going
            }
        } else {
            //handle carriage returns
            if (fmtstr[i] == '\n') {
                fault_putc('\r');
            }
            fault_putc(fmtstr[i]);
        }
        i++;
    }
#endif    
}

#ifdef MBED_CONF_RTOS_PRESENT 
/* Prints info of a thread(using osRtxThread_t struct)*/
static void print_thread(osRtxThread_t *thread)
{
    uint32_t data[5];

    data[0]=thread->state;
    data[1]=thread->thread_addr;
    data[2]=thread->stack_size;
    data[3]=(uint32_t)thread->stack_mem;
    data[4]=thread->sp;
    fault_print("\nState: 0x%x Entry: 0x%x Stack Size: 0x%x Mem: 0x%x SP: 0x%x", data);
}

/* Prints thread info from a list */
static void print_threads_info(osRtxThread_t *threads)
{
    while(threads != NULL) {
        print_thread( threads );
        threads = threads->thread_next;
    }
}

#endif

//This is a handler function called from Fault handler to print the error information out.
//This runs in fault context and uses special functions(defined in mbed_rtx_fault_handler.c) to print the information without using C-lib support.
__NO_RETURN void mbed_fault_handler (uint32_t fault_type, void *mbed_fault_context_in, void *osRtxInfoIn)
{
    mbed_error_status_t faultStatus = MBED_SUCCESS;
    
    fault_print("\n++ MbedOS Fault Handler ++\n\nFaultType: ",NULL);
        
    switch( fault_type ) {
      case HARD_FAULT_EXCEPTION: 
        fault_print("HardFault",NULL);
        faultStatus = MBED_ERROR_HARDFAULT_EXCEPTION;      
        break;
      case MEMMANAGE_FAULT_EXCEPTION: 
        fault_print("MemManageFault",NULL); 
        faultStatus = MBED_ERROR_MEMMANAGE_EXCEPTION;
        break;
      case BUS_FAULT_EXCEPTION: 
        fault_print("BusFault",NULL); 
        faultStatus = MBED_ERROR_BUSFAULT_EXCEPTION;
        break;
      case USAGE_FAULT_EXCEPTION: 
        fault_print("UsageFault",NULL); 
        faultStatus = MBED_ERROR_USAGEFAULT_EXCEPTION;
        break;
      default: 
        fault_print("Unknown Fault",NULL); 
        faultStatus = MBED_ERROR_UNKNOWN;
        break;
    }
    fault_print("\n\nContext:",NULL);
    print_context_info();
        
    fault_print("\n\nThreads Info:\nCurrent:",NULL);
    print_thread(((osRtxInfo_t *)osRtxInfoIn)->thread.run.curr);
  
    fault_print("\nNext:",NULL);
    print_thread(((osRtxInfo_t *)osRtxInfoIn)->thread.run.next);
    
    fault_print("\nWait:",NULL);
    osRtxThread_t *threads = ((osRtxInfo_t *)osRtxInfoIn)->thread.wait_list;
    print_threads_info(threads);
    
    fault_print("\nDelay:",NULL);
    threads = ((osRtxInfo_t *)osRtxInfoIn)->thread.delay_list;
    print_threads_info(threads);
    
    fault_print("\nIdle:",NULL);
    threads = ((osRtxInfo_t *)osRtxInfoIn)->thread.idle;
    print_threads_info(threads);
    
    fault_print("\n\n-- MbedOS Fault Handler --\n\n",NULL);
    
    //Now call mbed_error, to log the error and halt the system
    mbed_error( MBED_MAKE_ERROR( MBED_MODULE_UNKNOWN, faultStatus ), "System encountered an unrecoverable fault exception, halting system.", mbed_fault_context.PC_reg, NULL, 0 );
    
    /* In case we return, just spin here, we have already crashed */
    for (;;) { 
        __WFI(); 
    }
}

void print_context_info(void)
{
    //Context Regs
    fault_print("\nR0   : %x" 
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
    fault_print("\nCPUID: %x",(uint32_t *)&SCB->CPUID);
    
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
    fault_print("\nHFSR : %x"
                    "\nMMFSR: %x"
                    "\nBFSR : %x"
                    "\nUFSR : %x"
                    "\nDFSR : %x"
                    "\nAFSR : %x"
                    "\nSHCSR: %x",FSR); 
    
    //Print MMFAR only if its valid as indicated by MMFSR
    if(FSR[1] & 0x80) {
        fault_print("\nMMFAR: %x",(uint32_t *)&SCB->MMFAR); 
    }
    //Print BFAR only if its valid as indicated by BFSR
    if(FSR[2] & 0x80) {
        fault_print("\nBFAR : %x",(uint32_t *)&SCB->BFAR); 
    }
#endif
    //Print Mode
    if(mbed_fault_context.EXC_RETURN & 0x8) {
        fault_print("\nMode : Thread", NULL);
        //Print Priv level in Thread mode - We capture CONTROL reg which reflects the privilege.
        //Note that the CONTROL register captured still reflects the privilege status of the 
        //thread mode eventhough we are in Handler mode by the time we capture it.
        if(mbed_fault_context.CONTROL & 0x1) {
            fault_print("\nPriv : User", NULL); 
        } else {
            fault_print("\nPriv : Privileged", NULL); 
        }        
    } else {
        fault_print("\nMode : Handler", NULL); 
        fault_print("\nPriv : Privileged", NULL); 
    }
    //Print Return Stack
    if(mbed_fault_context.EXC_RETURN & 0x4) {
        fault_print("\nStack: PSP", NULL); 
    } else {
        fault_print("\nStack: MSP", NULL); 
    }
}

#endif //MBED_FAULT_HANDLER_SUPPORT
