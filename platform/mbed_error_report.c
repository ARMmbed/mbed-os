/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "hal/serial_api.h"
#include "hal/itm_api.h"
#include "platform/mbed_error.h"
#include "platform/mbed_error_report.h"

#ifdef DEVICE_SERIAL
extern int stdio_uart_inited;
extern serial_t stdio_uart;
#endif

//Helper macro to get the current SP
#define GET_CURRENT_SP(sp)                                                          \
                        {                                                           \
                            /*If in Handler mode we are always using MSP*/          \
                            if( __get_IPSR() != 0U ) {                              \
                                sp = __get_MSP();                                   \
                            } else {                                                \
                                /*Look into CONTROL.SPSEL value*/                   \
                                if ((__get_CONTROL() & 2U) == 0U) {                 \
                                    sp = __get_MSP();/*Read MSP*/                   \
                                } else {                                            \
                                    sp = __get_PSP();/*Read PSP*/                   \
                                }                                                   \
                            }                                                       \
                        }    

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

/* Converts a uint32 to dec char string */
static void value_to_dec_str(uint32_t value, char *dec_str)
{
    char dec_char_map[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    int i = 0;
    
    //Return without converting if hex_str is not provided
    if(dec_str == NULL) return;
        
    while(value > 0) {
        dec_str[i++] = dec_char_map[value % 10];
        value = value / 10;
    }
}

void mbed_error_init(void)
{
#if DEVICE_SERIAL && (MBED_CONF_ERROR_REPORT_INTERFACE==DEVICE_SERIAL)
    /* Initializes std uart if not init-ed yet */
    if (!stdio_uart_inited) {
        serial_init(&stdio_uart, STDIO_UART_TX, STDIO_UART_RX);
    }
#endif

#if DEVICE_ITM && (MBED_CONF_ERROR_REPORT_INTERFACE==DEVICE_ITM)
    /*Initialize ITM interfaces*/
    mbed_itm_init();
#endif
}

void mbed_error_putc(char ch)
{
#if DEVICE_SERIAL && (MBED_CONF_ERROR_REPORT_INTERFACE==DEVICE_SERIAL)
    serial_putc(&stdio_uart, ch);
#endif

#if DEVICE_ITM && (MBED_CONF_ERROR_REPORT_INTERFACE==DEVICE_ITM)
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
void mbed_error_print(char *fmtstr, uint32_t *values)
{
#if DEVICE_SERIAL || DEVICE_ITM
    int i = 0;
    int idx = 0;
    int vidx = 0;
    char num_str[9]={0};
    char *str=NULL;
    
    //Init error reporting interfaces
    mbed_error_init();
        
    while(fmtstr[i] != '\0') {
        if(fmtstr[i]=='%') {
            i++;
            if(fmtstr[i]=='x' || fmtstr[i]=='d') {
                //print the number in hex format
                value_to_hex_str(values[vidx++],num_str);
                for(idx=7; idx>=0; idx--) {
                    mbed_error_putc(num_str[idx]);
                }
            }
            else if(fmtstr[i]=='d') {
                memset(num_str, '0', sizeof(num_str));
                //print the number in dec format
                value_to_dec_str(values[vidx++],num_str);
                idx=5;//Start from 5 as we dont have big decimal numbers
                while(num_str[idx--]=='0' && idx > 0);//Dont print zeros at front
                for(idx++;idx>=0; idx--) {
                    mbed_error_putc(num_str[idx]);
                }
            }
            else if(fmtstr[i]=='s') {
                //print the string
                str = (char *)((uint32_t)values[vidx++]);
                while(*str != '\0') {
                    mbed_error_putc(*str);
                    str++;
                }
                str = NULL;
            } else {
                //Do not handle any other % formatting and keep going
            }
        } else {
            //handle carriage returns
            if (fmtstr[i] == '\n') {
                mbed_error_putc('\r');
            }
            mbed_error_putc(fmtstr[i]);
        }
        i++;
    }
#endif    
}

#ifdef MBED_CONF_RTOS_PRESENT 
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
    mbed_error_print("\nState: 0x%x Entry: 0x%x Stack Size: 0x%x Mem: 0x%x SP: 0x%x", data);
}
#endif

/* Prints the error information */
void mbed_report_error(mbed_error_ctx *error_ctx, char *error_msg) 
{
    uint32_t error_vals[3] = {0};
    error_vals[0] = error_ctx->error_status;
    error_vals[1] = GET_MBED_ERROR_CODE(error_ctx->error_status);
    error_vals[2] = GET_MBED_ERROR_MODULE(error_ctx->error_status);
    
    mbed_error_print("\n\n++ MbedOS Error Info ++\nError Status: 0x%x Code: %d Entity: %d\nError Message: ", error_vals);
    
    //Report error info based on error code, some errors require different 
    //error_vals[1] contains the error code
    if(error_vals[1] == ERROR_CODE_HARDFAULT_EXCEPTION || 
       error_vals[1] == ERROR_CODE_MEMMANAGE_EXCEPTION || 
       error_vals[1] == ERROR_CODE_BUSFAULT_EXCEPTION || 
       error_vals[1] == ERROR_CODE_USAGEFAULT_EXCEPTION ) {
        mbed_error_print(error_msg, NULL);
        mbed_error_print("\nLocation: 0x%x\n", (uint32_t *)&error_ctx->error_value);
    } else {
        switch (error_vals[1]) {
            //These are errors reported by kernel handled from mbed_rtx_handlers
            case ERROR_CODE_RTOS_EVENT:
                mbed_error_print("Kernel Error: 0x%x, ", (uint32_t *)&error_ctx->error_value);
                break;
            
            case ERROR_CODE_RTOS_THREAD_EVENT:
                mbed_error_print("Thread: 0x%x, ", (uint32_t *)&error_ctx->error_value);
                break;
            
            case ERROR_CODE_RTOS_MUTEX_EVENT:
                mbed_error_print("Mutex: 0x%x, ", (uint32_t *)&error_ctx->error_value);
                break;
            
            case ERROR_CODE_RTOS_SEMAPHORE_EVENT:
                mbed_error_print("Semaphore: 0x%x, ", (uint32_t *)&error_ctx->error_value);
                break;
            
            case ERROR_CODE_RTOS_MEMORY_POOL_EVENT:
                mbed_error_print("MemoryPool: 0x%x, ", (uint32_t *)&error_ctx->error_value);
                break;
            
            case ERROR_CODE_RTOS_EVENT_FLAGS_EVENT:
                mbed_error_print("EventFlags: 0x%x, ", (uint32_t *)&error_ctx->error_value);
                break;
            
            case ERROR_CODE_RTOS_TIMER_EVENT:
                mbed_error_print("Timer: 0x%x, ", (uint32_t *)&error_ctx->error_value);
                break;
            
            case ERROR_CODE_RTOS_MESSAGE_QUEUE_EVENT:    
                mbed_error_print("MessageQueue: 0x%x, ", (uint32_t *)&error_ctx->error_value);
                break;
            
            default:
                //Nothing to do here, just print the error info down
                break;
        }
        mbed_error_print(error_msg, NULL);
        mbed_error_print("\nLocation: 0x%x", (uint32_t *)&error_ctx->error_address);
#ifdef MBED_CONF_ERROR_FILENAME_CAPTURE_ENABLED
        if(NULL != error_ctx->error_filename) {
            //for string, we must pass address of a ptr which has the address of the string 
            uint32_t *file_name = (uint32_t *)&error_ctx->error_filename[0];
            mbed_error_print("\nFile:%s", &file_name);
            mbed_error_print("+0x%x", (uint32_t *)&error_ctx->error_line_number);
        }
#endif 

#ifdef MBED_CONF_RTOS_PRESENT         
        //Capture thread info
        osRtxThread_t *current_thread = osRtxInfo.thread.run.curr;
        error_ctx->thread_id = (uint32_t)current_thread;
        error_ctx->thread_entry_address = (uint32_t)current_thread->thread_addr;
        error_ctx->thread_stack_size = current_thread->stack_size;
        error_ctx->thread_stack_mem = (uint32_t)current_thread->stack_mem;
#ifdef TARGET_CORTEX_M        
        GET_CURRENT_SP(error_ctx->thread_current_sp);
            
        //Take advantage of the fact that the thread info in context struct is consecutively placed
        mbed_error_print("\nError Value: 0x%x\nCurrent Thread: Id: 0x%x Entry: 0x%x StackSize: 0x%x StackMem: 0x%x SP: 0x%x ", 
                            (uint32_t *)&error_ctx->error_value);
#else
        //For Cortex-A targets we dont have support to capture the current SP
        //Take advantage of the fact that the thread info in context struct is consecutively placed
        mbed_error_print("\nError Value: 0x%x\nCurrent Thread: Id: 0x%x Entry: 0x%x StackSize: 0x%x StackMem: 0x%x ", 
                            (uint32_t *)&error_ctx->error_value);
#endif //TARGET_CORTEX_M
   
#endif //MBED_CONF_RTOS_PRESENT
    }
    
    mbed_error_print("\n-- MbedOS Error Info --", NULL);
}
