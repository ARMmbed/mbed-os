/* Copyright (C) 2012 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef __ARCH_SYS_ARCH_H__
#define __ARCH_SYS_ARCH_H__

#include "lwip/opt.h"
#include "rtx_os.h"

extern u8_t lwip_ram_heap[];

#if NO_SYS == 0
#include "cmsis_os2.h"

// === SEMAPHORE ===
typedef struct {
    osSemaphoreId_t   id;
    osSemaphoreAttr_t attr;
#ifdef CMSIS_OS_RTX
    char         data[sizeof(os_semaphore_t)];
#endif
} sys_sem_t;

#define sys_sem_valid(x)        (((*x).id == NULL) ? 0 : 1)
#define sys_sem_set_invalid(x)  ( (*x).id = NULL)

// === MUTEX ===
typedef struct {
    osMutexId_t    id;
    osMutexAttr_t  attr;
#ifdef CMSIS_OS_RTX
#if defined(__MBED_CMSIS_RTOS_CA9) || defined(__MBED_CMSIS_RTOS_CM)
    char       data[sizeof(os_mutex_t)];
#else
    int32_t    data[3];
#endif
#endif
} sys_mutex_t;

// === MAIL BOX ===
#define MB_SIZE      8

typedef struct {
    osMessageQueueId_t   id;
    osMessageQueueAttr_t attr;
#ifdef CMSIS_OS_RTX
    char        queue[MB_SIZE * (sizeof(void*) + sizeof(os_message_t))];
    char        obj[sizeof(os_message_queue_t)];
#endif
} sys_mbox_t;

#define SYS_MBOX_NULL               ((uint32_t) NULL)
#define sys_mbox_valid(x)           (((*x).id == NULL) ? 0 : 1 )
#define sys_mbox_set_invalid(x)     ( (*x).id = NULL )

#if ((DEFAULT_RAW_RECVMBOX_SIZE) > (MB_SIZE)) || \
    ((DEFAULT_UDP_RECVMBOX_SIZE) > (MB_SIZE)) || \
    ((DEFAULT_TCP_RECVMBOX_SIZE) > (MB_SIZE)) || \
    ((DEFAULT_ACCEPTMBOX_SIZE)   > (MB_SIZE)) || \
    ((TCPIP_MBOX_SIZE)           > (MB_SIZE))
#   error Mailbox size not supported
#endif

// === THREAD ===
typedef struct {
    osThreadId_t   id;
    osThreadAttr_t attr;
} sys_thread_data_t;
typedef sys_thread_data_t* sys_thread_t;

#define SYS_THREAD_POOL_N                   6
#define SYS_DEFAULT_THREAD_STACK_DEPTH      DEFAULT_STACK_SIZE

// === PROTECTION ===
typedef int sys_prot_t;

#else
#ifdef  __cplusplus
extern "C" {
#endif

/** \brief  Init systick to 1ms rate
 *
 *  This init the systick to 1ms rate. This function is only used in standalone
 *  systems.
 */
void SysTick_Init(void);


/** \brief  Get the current systick time in milliSeconds
 *
 *  Returns the current systick time in milliSeconds. This function is only
 *  used in standalone systems.
 *
 *  /returns current systick time in milliSeconds
 */
uint32_t SysTick_GetMS(void);

/** \brief  Delay for the specified number of milliSeconds
 *
 *  For standalone systems. This function will block for the specified
 *  number of milliSconds. For RTOS based systems, this function will delay
 *  the task by the specified number of milliSeconds.
 *
 *  \param[in]  ms Time in milliSeconds to delay
 */
void osDelay(uint32_t ms);

#ifdef  __cplusplus
}
#endif
#endif

#endif /* __ARCH_SYS_ARCH_H__ */
