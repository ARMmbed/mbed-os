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
#include <string.h>

/* mbed includes */
#include "mbed_error.h"
#include "mbed_interface.h"
#include "mbed_rtos_storage.h"

/* lwIP includes. */
#include "lwip/opt.h"
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"

/* Define the heap ourselves to give us section placement control */
#ifndef ETHMEM_SECTION
#if defined(TARGET_LPC4088) || defined(TARGET_LPC4088_DM)
#  if defined (__ICCARM__)
#     define ETHMEM_SECTION
#  elif defined(TOOLCHAIN_GCC_CR)
#     define ETHMEM_SECTION __attribute__((section(".data.$RamPeriph32")))
#  else
#     define ETHMEM_SECTION __attribute__((section("AHBSRAM1"),aligned))
#  endif
#elif defined(TARGET_LPC1768)
#  if defined (__ICCARM__)
#     define ETHMEM_SECTION
#  elif defined(TOOLCHAIN_GCC_CR)
#     define ETHMEM_SECTION __attribute__((section(".data.$RamPeriph32")))
#  else
#     define ETHMEM_SECTION __attribute__((section("AHBSRAM0"),aligned))
#  endif
#else
#define ETHMEM_SECTION
#endif
#endif

/* LWIP's mem.c doesn't give visibility of its overhead; memory area has to be big
 * enough to hold "MEM_SIZE" (which we specify) plus mem.c's overhead. Have to work
 * it all out here, copying code from mem.c */
struct mem {
  /** index (-> ram[next]) of the next struct */
  mem_size_t next;
  /** index (-> ram[prev]) of the previous struct */
  mem_size_t prev;
  /** 1: this area is used; 0: this area is unused */
  u8_t used;
};

#define SIZEOF_STRUCT_MEM    LWIP_MEM_ALIGN_SIZE(sizeof(struct mem))
#define MEM_SIZE_ALIGNED     LWIP_MEM_ALIGN_SIZE(MEM_SIZE)

#if defined (__ICCARM__)
#pragma location = ".ethusbram"
#endif
LWIP_DECLARE_MEMORY_ALIGNED(lwip_ram_heap, MEM_SIZE_ALIGNED + (2U*SIZEOF_STRUCT_MEM)) ETHMEM_SECTION;

 #if NO_SYS==1
#include "cmsis.h"

/* Saved total time in ms since timer was enabled */
static volatile u32_t systick_timems;

/* Enable systick rate and interrupt */
void SysTick_Init(void) {
    if (SysTick_Config(SystemCoreClock / 1000)) {
        while (1);     /* Capture error */
    }
}

/** \brief  SysTick IRQ handler and timebase management
 *
 *  This function keeps a timebase for the sysTick that can be
 * used for other functions. It also calls an external function
 * (SysTick_User) that must be defined outside this handler.
 */
void SysTick_Handler(void) {
    systick_timems++;
}

/* Delay for the specified number of milliSeconds */
void osDelay(uint32_t ms) {
    uint32_t to = ms + systick_timems;
    while (to > systick_timems);
}

/* Returns the current time in mS. This is needed for the LWIP timers */
u32_t sys_now(void) {
  return (u32_t) systick_timems;
}

#else
/* CMSIS-RTOS implementation of the lwip operating system abstraction */
#include "arch/sys_arch.h"

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates a new mailbox
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      int queue_sz            -- Size of elements in the mailbox
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *---------------------------------------------------------------------------*/
err_t sys_mbox_new(sys_mbox_t *mbox, int queue_sz) {
    if (queue_sz > MB_SIZE)
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_INVALID_SIZE), "sys_mbox_new size error\n", queue_sz);

    memset(mbox, 0, sizeof(*mbox));

    mbox->attr.cb_mem = &mbox->data;
    mbox->attr.cb_size = sizeof(mbox->data);
    mbox->id = osEventFlagsNew(&mbox->attr);
    if (mbox->id == NULL)
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_FAILED_OPERATION), "sys_mbox_new create error\n", (u32_t)mbox);

    osEventFlagsSet(mbox->id, SYS_MBOX_POST_EVENT);

    return ERR_OK;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_free
 *---------------------------------------------------------------------------*
 * Description:
 *      Deallocates a mailbox. If there are messages still present in the
 *      mailbox when the mailbox is deallocated, it is an indication of a
 *      programming error in lwIP and the developer should be notified.
 * Inputs:
 *      sys_mbox_t *mbox         -- Handle of mailbox
 *---------------------------------------------------------------------------*/
void sys_mbox_free(sys_mbox_t *mbox) {
    if (mbox->post_idx != mbox->fetch_idx)
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_INVALID_INDEX), "sys_mbox_free error\n", (u32_t)mbox->fetch_idx);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_post
 *---------------------------------------------------------------------------*
 * Description:
 *      Post the "msg" to the mailbox.
 * Inputs:
 *      sys_mbox_t mbox        -- Handle of mailbox
 *      void *msg              -- Pointer to data to post
 *---------------------------------------------------------------------------*/
void sys_mbox_post(sys_mbox_t *mbox, void *msg) {
    osEventFlagsWait(mbox->id, SYS_MBOX_POST_EVENT,
            osFlagsWaitAny | osFlagsNoClear, osWaitForever);

    int state = osKernelLock();

    mbox->queue[mbox->post_idx % MB_SIZE] = msg;
    mbox->post_idx += 1;

    osEventFlagsSet(mbox->id, SYS_MBOX_FETCH_EVENT);
    if (mbox->post_idx - mbox->fetch_idx == MB_SIZE-1)
        osEventFlagsClear(mbox->id, SYS_MBOX_POST_EVENT);

    osKernelRestoreLock(state);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_trypost
 *---------------------------------------------------------------------------*
 * Description:
 *      Try to post the "msg" to the mailbox.  Returns immediately with
 *      error if cannot.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void *msg               -- Pointer to data to post
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *                                  if not.
 *---------------------------------------------------------------------------*/
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg) {
    uint32_t flags = osEventFlagsWait(mbox->id, SYS_MBOX_POST_EVENT,
            osFlagsWaitAny | osFlagsNoClear, 0);
    if ((flags & osFlagsError) || !(flags & SYS_MBOX_POST_EVENT)) {
        MBED_WARNING1(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_FAILED_OPERATION), "sys_mbox_trypost error\n", flags);
        return ERR_MEM;
    }

    int state = osKernelLock();

    mbox->queue[mbox->post_idx % MB_SIZE] = msg;
    mbox->post_idx += 1;

    osEventFlagsSet(mbox->id, SYS_MBOX_FETCH_EVENT);
    if (mbox->post_idx - mbox->fetch_idx == MB_SIZE-1)
        osEventFlagsClear(mbox->id, SYS_MBOX_POST_EVENT);

    osKernelRestoreLock(state);
    return ERR_OK;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_mbox_fetch
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread until a message arrives in the mailbox, but does
 *      not block the thread longer than "timeout" milliseconds (similar to
 *      the sys_arch_sem_wait() function). The "msg" argument is a result
 *      parameter that is set by the function (i.e., by doing "*msg =
 *      ptr"). The "msg" parameter maybe NULL to indicate that the message
 *      should be dropped.
 *
 *      The return values are the same as for the sys_arch_sem_wait() function:
 *      Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
 *      timeout.
 *
 *      Note that a function with a similar name, sys_mbox_fetch(), is
 *      implemented by lwIP.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void **msg              -- Pointer to pointer to msg received
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- SYS_ARCH_TIMEOUT if timeout, else number
 *                                  of milliseconds until received.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout) {
    uint32_t start = osKernelGetTickCount();
    uint32_t flags = osEventFlagsWait(mbox->id, SYS_MBOX_FETCH_EVENT,
            osFlagsWaitAny | osFlagsNoClear, (timeout ? timeout : osWaitForever));
    if ((flags & osFlagsError) || !(flags & SYS_MBOX_FETCH_EVENT)) {
        MBED_WARNING1(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_TIME_OUT), "sys_arch_mbox_fetch time-out\n", flags);
        return SYS_ARCH_TIMEOUT;
    }

    int state = osKernelLock();

    if (msg)
        *msg = mbox->queue[mbox->fetch_idx % MB_SIZE];
    mbox->fetch_idx += 1;

    osEventFlagsSet(mbox->id, SYS_MBOX_POST_EVENT);
    if (mbox->post_idx == mbox->fetch_idx)
        osEventFlagsClear(mbox->id, SYS_MBOX_FETCH_EVENT);

    osKernelRestoreLock(state);
    return osKernelGetTickCount() - start;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_mbox_tryfetch
 *---------------------------------------------------------------------------*
 * Description:
 *      Similar to sys_arch_mbox_fetch, but if message is not ready
 *      immediately, we'll return with SYS_MBOX_EMPTY.  On success, 0 is
 *      returned.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void **msg              -- Pointer to pointer to msg received
 * Outputs:
 *      u32_t                   -- SYS_MBOX_EMPTY if no messages.  Otherwise,
 *                                  return ERR_OK.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg) {
    uint32_t flags = osEventFlagsWait(mbox->id, SYS_MBOX_FETCH_EVENT,
            osFlagsWaitAny | osFlagsNoClear, 0);
    if ((flags & osFlagsError) || !(flags & SYS_MBOX_FETCH_EVENT)) {
        MBED_WARNING1(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_FAILED_OPERATION), "sys_arch_mbox_tryfetch empty\n", flags);
        return SYS_MBOX_EMPTY;
    }

    int state = osKernelLock();

    if (msg)
        *msg = mbox->queue[mbox->fetch_idx % MB_SIZE];
    mbox->fetch_idx += 1;

    osEventFlagsSet(mbox->id, SYS_MBOX_POST_EVENT);
    if (mbox->post_idx == mbox->fetch_idx)
        osEventFlagsClear(mbox->id, SYS_MBOX_FETCH_EVENT);

    osKernelRestoreLock(state);
    return ERR_OK;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates and returns a new semaphore. The "ucCount" argument specifies
 *      the initial state of the semaphore.
 *      NOTE: Currently this routine only creates counts of 1 or 0
 * Inputs:
 *      sys_sem_t sem         -- Handle of semaphore
 *      u8_t count            -- Initial count of semaphore
 * Outputs:
 *      err_t                 -- ERR_OK if semaphore created
 *---------------------------------------------------------------------------*/
err_t sys_sem_new(sys_sem_t *sem, u8_t count) {
    memset(sem, 0, sizeof(*sem));
    sem->attr.cb_mem = &sem->data;
    sem->attr.cb_size = sizeof(sem->data);
    sem->id = osSemaphoreNew(UINT16_MAX, count, &sem->attr);
    if (sem->id == NULL)
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_FAILED_OPERATION), "sys_sem_new create error\n", (u32_t)sem);
    
    return ERR_OK;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_sem_wait
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread while waiting for the semaphore to be
 *      signaled. If the "timeout" argument is non-zero, the thread should
 *      only be blocked for the specified time (measured in
 *      milliseconds).
 *
 *      If the timeout argument is non-zero, the return value is the number of
 *      milliseconds spent waiting for the semaphore to be signaled. If the
 *      semaphore wasn't signaled within the specified time, the return value is
 *      SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
 *      (i.e., it was already signaled), the function may return zero.
 *
 *      Notice that lwIP implements a function with a similar name,
 *      sys_sem_wait(), that uses the sys_arch_sem_wait() function.
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to wait on
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- Time elapsed or SYS_ARCH_TIMEOUT.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout) {
    u32_t start = osKernelGetTickCount();
    
    if (osSemaphoreAcquire(sem->id, (timeout != 0)?(timeout):(osWaitForever)) != osOK) {
        MBED_WARNING1(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_TIME_OUT), "sys_arch_sem_wait time out\n", (u32_t)sem);
        return SYS_ARCH_TIMEOUT;
    }
    
    return osKernelGetTickCount() - start;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_signal
 *---------------------------------------------------------------------------*
 * Description:
 *      Signals (releases) a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to signal
 *---------------------------------------------------------------------------*/
void sys_sem_signal(sys_sem_t *data) {
    if (osSemaphoreRelease(data->id) != osOK)
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_SEMAPHORE_UNLOCK_FAILED), "sys_sem_signal error\n", (u32_t)data->id);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_free
 *---------------------------------------------------------------------------*
 * Description:
 *      Deallocates a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to free
 *---------------------------------------------------------------------------*/
void sys_sem_free(sys_sem_t *sem) {}

/** Create a new mutex
 * @param mutex pointer to the mutex to create
 * @return a new mutex */
err_t sys_mutex_new(sys_mutex_t *mutex) {
    memset(mutex, 0, sizeof(*mutex));
    mutex->attr.name = "lwip_mutex";
    mutex->attr.cb_mem = &mutex->data;
    mutex->attr.cb_size = sizeof(mutex->data);
    mutex->id = osMutexNew(&mutex->attr);
    if (mutex->id == NULL) {
        MBED_WARNING1(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_FAILED_OPERATION), "sys_mutex_new error\n", (u32_t)mutex);
        return ERR_MEM;
    }
    
    return ERR_OK;
}

/** Lock a mutex
 * @param mutex the mutex to lock */
void sys_mutex_lock(sys_mutex_t *mutex) {
    if (osMutexAcquire(mutex->id, osWaitForever) != osOK)
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_MUTEX_LOCK_FAILED), "sys_mutex_lock error\n", (u32_t)mutex);
}

/** Unlock a mutex
 * @param mutex the mutex to unlock */
void sys_mutex_unlock(sys_mutex_t *mutex) {
    if (osMutexRelease(mutex->id) != osOK)
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_MUTEX_UNLOCK_FAILED), "sys_mutex_unlock error\n", (u32_t)mutex);
}

/** Delete a mutex
 * @param mutex the mutex to delete */
void sys_mutex_free(sys_mutex_t *mutex) {}

/*---------------------------------------------------------------------------*
 * Routine:  sys_init
 *---------------------------------------------------------------------------*
 * Description:
 *      Initialize sys arch
 *---------------------------------------------------------------------------*/
osMutexId_t lwip_sys_mutex;
osMutexAttr_t lwip_sys_mutex_attr;
mbed_rtos_storage_mutex_t lwip_sys_mutex_data;

void sys_init(void) {
    lwip_sys_mutex_attr.name = "lwip_sys_mutex";
    lwip_sys_mutex_attr.cb_mem = &lwip_sys_mutex_data;
    lwip_sys_mutex_attr.cb_size = sizeof(lwip_sys_mutex_data);
    lwip_sys_mutex_attr.attr_bits = osMutexRecursive;
    lwip_sys_mutex = osMutexNew(&lwip_sys_mutex_attr);
    if (lwip_sys_mutex == NULL)
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_INITIALIZATION_FAILED), "sys_init error, mutex initialization failed\n");
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_jiffies
 *---------------------------------------------------------------------------*
 * Description:
 *      Used by PPP as a timestamp-ish value
 *---------------------------------------------------------------------------*/
u32_t sys_jiffies(void) {
    return osKernelGetTickCount();
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_protect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" critical region protection and
 *      returns the previous protection level. This function is only called
 *      during very short critical regions. An embedded system which supports
 *      ISR-based drivers might want to implement this function by disabling
 *      interrupts. Task-based systems might want to implement this by using
 *      a mutex or disabling tasking. This function should support recursive
 *      calls from the same task or interrupt. In other words,
 *      sys_arch_protect() could be called while already protected. In
 *      that case the return value indicates that it is already protected.
 *
 *      sys_arch_protect() is only required if your port is supporting an
 *      operating system.
 * Outputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
sys_prot_t sys_arch_protect(void) {
    if (osMutexAcquire(lwip_sys_mutex, osWaitForever) != osOK)
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_MUTEX_LOCK_FAILED), "sys_arch_protect error\n");
    return (sys_prot_t) 1;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_unprotect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" set of critical region
 *      protection to the value specified by pval. See the documentation for
 *      sys_arch_protect() for more information. This function is only
 *      required if your port is supporting an operating system.
 * Inputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
void sys_arch_unprotect(sys_prot_t p) {
    if (osMutexRelease(lwip_sys_mutex) != osOK)
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_MUTEX_UNLOCK_FAILED), "sys_arch_unprotect error\n");
}

u32_t sys_now(void) {
    return osKernelGetTickCount();
}

void sys_msleep(u32_t ms) {
    osDelay(ms);
}

osThreadId_t lwip_tcpip_thread_id = 0;

void sys_tcpip_thread_set(void)
{
    lwip_tcpip_thread_id = osThreadGetId();
}

bool sys_tcpip_thread_check(void)
{
    return osThreadGetId() == lwip_tcpip_thread_id;
}

// Keep a pool of thread structures
static int thread_pool_index = 0;
static sys_thread_data_t thread_pool[SYS_THREAD_POOL_N];

/*---------------------------------------------------------------------------*
 * Routine:  sys_thread_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Starts a new thread with priority "prio" that will begin its
 *      execution in the function "thread()". The "arg" argument will be
 *      passed as an argument to the thread() function. The id of the new
 *      thread is returned. Both the id and the priority are system
 *      dependent.
 * Inputs:
 *      char *name                -- Name of thread
 *      void (*thread)(void *arg) -- Pointer to function to run.
 *      void *arg                 -- Argument passed into function
 *      int stacksize             -- Required stack amount in bytes
 *      int priority              -- Thread priority
 * Outputs:
 *      sys_thread_t              -- Pointer to thread handle.
 *---------------------------------------------------------------------------*/
#ifndef MBED_TZ_DEFAULT_ACCESS
#define MBED_TZ_DEFAULT_ACCESS   0
#endif    

 sys_thread_t sys_thread_new(const char *pcName,
                            void (*thread)(void *arg),
                            void *arg, int stacksize, int priority) {
    LWIP_DEBUGF(SYS_DEBUG, ("New Thread: %s\n", pcName));

    if (thread_pool_index >= SYS_THREAD_POOL_N)
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_THREAD_CREATE_FAILED), "sys_thread_new number error\n", thread_pool_index);
    sys_thread_t t = (sys_thread_t)&thread_pool[thread_pool_index];
    thread_pool_index++;

    memset(t, 0, sizeof(*t));
    t->attr.name = pcName ? pcName : "lwip_unnamed_thread";
    t->attr.priority = (osPriority_t)priority;
    t->attr.cb_size = sizeof(t->data);
    t->attr.cb_mem = &t->data;
    t->attr.stack_size = stacksize;
    t->attr.stack_mem = malloc(stacksize);
    t->attr.tz_module = MBED_TZ_DEFAULT_ACCESS;
    if (t->attr.stack_mem == NULL) {
      MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_OUT_OF_MEMORY), "unable to allocate thread stack\n", stacksize);
    }
    t->id = osThreadNew((osThreadFunc_t)thread, arg, &t->attr);
    if (t->id == NULL)
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_THREAD_CREATE_FAILED), "sys_thread_new create error\n");
    
    return t;
}

#endif

#ifdef LWIP_DEBUG

#include <stdbool.h>

#if MBED_CONF_LWIP_USE_MBED_TRACE
#include "mbed-trace/mbed_trace.h"

void lwip_mbed_tracef_debug(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    mbed_vtracef(TRACE_LEVEL_DEBUG, "lwIP", fmt, ap);
    va_end(ap);
}

void lwip_mbed_tracef_warn(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    mbed_vtracef(TRACE_LEVEL_WARN, "lwIP", fmt, ap);
    va_end(ap);
}

void lwip_mbed_tracef_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    mbed_vtracef(TRACE_LEVEL_ERROR, "lwIP", fmt, ap);
    va_end(ap);
}

MBED_NORETURN void lwip_mbed_assert_fail(const char *msg, const char *func, const char *file, unsigned int line)
{
    mbed_tracef(TRACE_LEVEL_ERROR, "lwIP", "Assertion failed: %s, function %s, file %s, line %u.", msg, func, file, line);
    exit(EXIT_FAILURE); // XXX how about abort? mbed_assert uses exit, so follow suit
}
#else // MBED_CONF_LWIP_USE_MBED_TRACE

/** \brief  Displays an error message on assertion

    This function will display an error message on an assertion
    to the debug output.

    \param[in]    msg   Error message to display
    \param[in]    line  Line number in file with error
    \param[in]    file  Filename with error
 */
MBED_NORETURN void assert_printf(char *msg, int line, char *file) {
    if (msg)
        error("%s:%d in file %s\n", msg, line, file);
    else
        error("LWIP ASSERT\n");
}
#endif // MBED_CONF_LWIP_USE_MBED_TRACE

#if TRACE_TO_ASCII_HEX_DUMP
void trace_to_ascii_hex_dump(char *prefix, int len, char *data)
{
    int line_len = 0;

    for (int i = 0; i < len; i++) {
       if ((line_len % 16) == 0) {
           if (line_len != 0) {
               LWIP_PLATFORM_DIAG(("\n"));
           }
           LWIP_PLATFORM_DIAG(("%s %06x", prefix, line_len));
       }
       line_len++;
       LWIP_PLATFORM_DIAG((" %02x", data[i]));
    }
    LWIP_PLATFORM_DIAG(("\n"));
}
#endif

#endif /* LWIP_DEBUG */
