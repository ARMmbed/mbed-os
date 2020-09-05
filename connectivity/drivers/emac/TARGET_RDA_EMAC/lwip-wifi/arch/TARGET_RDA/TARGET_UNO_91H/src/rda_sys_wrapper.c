/* Copyright (c) 2019 Unisoc Communications Inc.
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

#if 1
#include "sys_arch.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "lwip/sys.h"
#include "rtx_lib.h"

#include <stdio.h>
#include <string.h>
#include "mbed_assert.h"
#define NO_ERR  0
#define ERR     -1

//#define RDA_SYS_DEBUG
#ifdef RDA_SYS_DEBUG
#define RDA_SYS_PRINT(fmt, ...) do {\
            printf(fmt, ##__VA_ARGS__);\
    } while (0)
#else
#define RDA_SYS_PRINT(fmt, ...)
#endif


/* Alarm */
// Timer definitions
#define osTimerInvalid  0
#define osTimerStopped  1
#define osTimerRunning  2
#if 0
// Timer structures, same as os_timer_cb(rt_CMSIS.c)
typedef struct rda_ostmr_cb {
    struct rda_ostmr_cb *next;                    // Pointer to next active Timer
    uint8_t             state;                    // Timer State
    uint8_t              type;                    // Timer Type (Periodic/One-shot)
    uint16_t         reserved;                    // Reserved
    uint32_t             tcnt;                    // Timer Delay Count
    uint32_t             icnt;                    // Timer Initial Count
    void                 *arg;                    // Timer Function Argument
    const osTimerDef_t *timer;                    // Pointer to Timer definition
} rda_ostmr_cb_t;
#endif
//#define USING_STDLIB

#if !defined(USING_STDLIB)
typedef struct rda_tmr_node {
    mbed_rtos_storage_timer_t _obj_mem;
} rda_tmr_node_t;

typedef struct rda_tmr_ctrl {
    unsigned char *buff;
    unsigned char *state;
    unsigned char max_node_num;
    unsigned char node_size;
    unsigned char node_cntr;
    unsigned char last_freed_node_idx;
} rda_tmr_ctrl_t;

#define MAX_ALARM_NUM       (28)
#define WORD_ALIGN(n)       (((n) + 0x03UL) & ~0x03UL)
#define MAX_ALARM_STAT_SIZE WORD_ALIGN(MAX_ALARM_NUM >> 3)
#define MAX_ALARM_MEM_SIZE  (MAX_ALARM_NUM * WORD_ALIGN(sizeof(mbed_rtos_storage_timer_t)) + MAX_ALARM_STAT_SIZE)

unsigned long g_alarm_buf[WORD_ALIGN(MAX_ALARM_MEM_SIZE) >> 2] = {0};
rda_tmr_ctrl_t g_alarm_ctrl = {
    (unsigned char *)g_alarm_buf + MAX_ALARM_STAT_SIZE,
    (unsigned char *)g_alarm_buf,
    MAX_ALARM_NUM,
    WORD_ALIGN(sizeof(mbed_rtos_storage_timer_t)),
    0U,
    0U
};

__STATIC_INLINE unsigned char get_node_state(unsigned char *buf, unsigned char idx)
{
    unsigned char state, ofst;
    ofst = (idx & 0x07U);
    buf += (idx >> 3);
    state = (*buf >> ofst) & 0x01U;
    return state;
}

__STATIC_INLINE void set_node_state(unsigned char *buf, unsigned char idx, unsigned char state)
{
    unsigned char ofst, tmp;
    ofst = (idx & 0x07U);
    buf += (idx >> 3);
    tmp = *buf & (~(0x01U << ofst));
    *buf = tmp | (((state & 0x01U) << ofst));
}

static rda_tmr_node_t *get_tmr_node(void)
{
    rda_tmr_node_t *node = NULL;
    unsigned char idx = g_alarm_ctrl.last_freed_node_idx;
    if ((idx < g_alarm_ctrl.max_node_num) && (0U == get_node_state(g_alarm_ctrl.state, idx))) {
        set_node_state(g_alarm_ctrl.state, idx, 1U);
        node = (rda_tmr_node_t *)(g_alarm_ctrl.buff + idx * g_alarm_ctrl.node_size);
        g_alarm_ctrl.node_cntr++;
    } else {
        for (idx = 0U; idx < g_alarm_ctrl.max_node_num; idx++) {
            if(0U == get_node_state(g_alarm_ctrl.state, idx)) {
                set_node_state(g_alarm_ctrl.state, idx, 1U);
                node = (rda_tmr_node_t *)(g_alarm_ctrl.buff + idx * g_alarm_ctrl.node_size);
                g_alarm_ctrl.node_cntr++;
                break;
            }
        }
    }
    return node;
}

static void put_tmr_node(rda_tmr_node_t *node)
{
    unsigned char *node_buf = (unsigned char *)node;
    unsigned char idx = (node_buf - g_alarm_ctrl.buff) / g_alarm_ctrl.node_size;
    if ((node_buf > g_alarm_ctrl.buff) && (idx < g_alarm_ctrl.max_node_num) &&
        (1U == get_node_state(g_alarm_ctrl.state, idx))) {
        set_node_state(g_alarm_ctrl.state, idx, 0U);
        g_alarm_ctrl.node_cntr--;
        g_alarm_ctrl.last_freed_node_idx = idx;
    }
}
#endif /* !USING_STDLIB */

/**
 * @brief     : get current time in units of micro second
 * @param[in] :
 * @param[out]:
 * @return    : return time value with uint32 type
 */
unsigned long rda_get_cur_time_ms(void)
{
    return sys_now();
}

/**
 * @brief     : create an alarm with given function, return timer handle
 * @param[in] : func(callback)/data(pass to func)/mode(once or periodic)
 * @param[out]:
 * @return    : return timer handle, a pointer to the timer structure, non-zero is valid
 */
void * rda_alarm_create_v2(void *func, unsigned int data, unsigned int mode)
{
    osTimerId_t handle;
    mbed_rtos_storage_timer_t* _obj_mem = NULL;
    MBED_ASSERT(func != NULL);
    osTimerAttr_t attr = { 0 };
#if defined(USING_STDLIB)
    _obj_mem = (mbed_rtos_storage_timer_t*)malloc(sizeof(mbed_rtos_storage_timer_t));
#else  /* USING_STDLIB */
    _obj_mem = (mbed_rtos_storage_timer_t*)get_tmr_node();
#endif /* USING_STDLIB */
    MBED_ASSERT(_obj_mem != NULL);
    memset(_obj_mem, 0, sizeof(_obj_mem));
    attr.cb_mem = _obj_mem;
    attr.cb_size = sizeof(mbed_rtos_storage_timer_t);
    //printf("hehehehe fun %p\r\n", func);
    handle = osTimerNew((osTimerFunc_t)func, mode, (void *)data, &attr);
    //printf("time cb %p handle %p\r\n", _obj_mem, handle);
    MBED_ASSERT(handle != NULL);
    return handle;
}

void * rda_alarm_create(void *func, unsigned int data)
{
    return rda_alarm_create_v2(func, data, osTimerOnce);
}

/**
 * @brief     : delete an alarm with given handle, then reset the handle
 * @param[in] : *handle(pointer to the timer structure)
 * @param[out]: **handle(address of the handle variable)
 * @return    :
 */
int rda_alarm_delete(void **handle)
{
    if (NULL != *handle) {
        osTimerId timer_id = (osTimerId)(*handle);
        osStatus retval = osTimerDelete(timer_id);
        if (osOK == retval) {
#if defined(USING_STDLIB)
            free(timer_id);
#else  /* USING_STDLIB */
            put_tmr_node((rda_tmr_node_t *)timer_id);
#endif /* USING_STDLIB */
            *handle = NULL;
        } else {
            RDA_SYS_PRINT("Delete alarm error: %d\r\n", retval);
            return ERR;
        }
        return NO_ERR;
    }
    return ERR;
}

/**
 * @brief     : start an alarm, raise a function call after given timeout delay
 * @param[in] : handle(pointer to the timer structure)/timeout(micro second)
 * @param[out]:
 * @return    :
 */
int rda_alarm_start(void *handle, unsigned int timeout_ms)
{
    if (NULL != handle) {
        osTimerId timer_id = (osTimerId)handle;
        osStatus retval = osTimerStart(timer_id, (uint32_t)timeout_ms);
        if (osOK != retval) {
            RDA_SYS_PRINT("Start alarm error: %d\r\n", retval);
            return ERR;
        }
        return NO_ERR;
    }
    return ERR;
}

/**
 * @brief     : stop an alarm, will not raise a function call any more
 * @param[in] : handle(pointer to the timer structure)
 * @param[out]:
 * @return    :
 */
int rda_alarm_stop(void *handle)
{
    if (NULL != handle) {
        osTimerId timer_id = (osTimerId)handle;
        os_timer_t *timer = osRtxTimerId(timer_id);
        if(timer->state == osRtxTimerRunning){
            osStatus retval = osTimerStop(timer_id);
            if(osOK != retval) {
                RDA_SYS_PRINT("Stop alarm error: %d\r\n", retval);
                return ERR;
            }
        }
        return NO_ERR;
    }
    return ERR;
}



/* Semaphore */
void* rda_sem_create(unsigned int count)
{
    osSemaphoreId_t sem;
    mbed_rtos_storage_semaphore_t *_obj_mem = (mbed_rtos_storage_semaphore_t*)malloc(sizeof(mbed_rtos_storage_semaphore_t));
    osSemaphoreAttr_t attr = { 0 };
    attr.name = "rda_unnamed_sem";
    attr.cb_mem = _obj_mem;
    attr.cb_size = sizeof(mbed_rtos_storage_semaphore_t);
    sem = osSemaphoreNew(1, count, &attr);
    MBED_ASSERT(sem != NULL);

    return (void*)sem;
}

int rda_sem_wait(void* sem, unsigned int millisec)
{
    int res;

    res = osSemaphoreWait(sem, millisec);
    if (res > 0) {
        return NO_ERR;
    } else {
        RDA_SYS_PRINT("rda_sem_wait error %d\r\n", res);
        return ERR;
    }
}

int rda_sem_release(void *sem)
{
    int res;

    res = osSemaphoreRelease(sem);
    if (res == 0) {
        return NO_ERR;
    } else {
        RDA_SYS_PRINT("rda_sem_release error %d\r\n", res);
        return ERR;
    }
}

int rda_sem_delete(void *sem)
{
    int res;

    res = osSemaphoreDelete(sem);
    free(sem);
    if (res == 0) {
        return NO_ERR;
    } else {
        RDA_SYS_PRINT("rda_sem_delete error %d\r\n", res);
        return ERR;
    }
}

/* Mail */
void* rda_mail_create(unsigned int msgcnt, unsigned int msgsize)
{
    unsigned int mq_size = msgcnt * (msgsize + sizeof(os_message_t));
    osMessageQueueId_t msgq;
    mbed_rtos_storage_msg_queue_t *_obj_mem = (mbed_rtos_storage_msg_queue_t*)malloc(sizeof(mbed_rtos_storage_msg_queue_t));
    MBED_ASSERT(_obj_mem != NULL);
    memset(_obj_mem, 0, sizeof(mbed_rtos_storage_msg_queue_t));
    void *_mq_mem = (void *)malloc(mq_size);
    MBED_ASSERT(_mq_mem != NULL);
    memset(_mq_mem, 0, mq_size);
    osMessageQueueAttr_t attr = { 0 };
    attr.name = "rda_unnamed_message_queue";
    attr.cb_mem = _obj_mem;
    attr.cb_size = sizeof(mbed_rtos_storage_msg_queue_t);
    attr.mq_mem = _mq_mem;
    attr.mq_size = mq_size;
    msgq = osMessageQueueNew(msgcnt, msgsize, &attr);
    MBED_ASSERT(msgq != NULL);

    return (void*)msgq;
}

int rda_mail_get(void *msgq, void *msg, unsigned int wait)
{
    int ret;
    ret = osMessageQueueGet(msgq, msg, 0, wait);
    return ret;
}

int rda_mail_put(void *msgq, void *msg, unsigned int wait)
{
    int ret;
    ret = osMessageQueuePut(msgq, msg, 0, wait);
    return ret;
}

/* Mutex */
void* rda_mutex_create(void)
{
    osMutexId_t rdamutex;
    osMutexAttr_t attr = { 0 };
    mbed_rtos_storage_mutex_t *_obj_mem = (mbed_rtos_storage_mutex_t *)malloc(sizeof(mbed_rtos_storage_mutex_t));
    memset(_obj_mem, 0, sizeof(mbed_rtos_storage_mutex_t));
    attr.name = "rda_unnamed_mutex";
    attr.cb_mem = _obj_mem;
    attr.cb_size = sizeof(mbed_rtos_storage_mutex_t);
    attr.attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust;
    rdamutex = osMutexNew(&attr);
    MBED_ASSERT(rdamutex != NULL);
    return (void *)rdamutex;
}

int rda_mutex_wait(void *rdamutex, unsigned int millisec)
{
    osMutexId_t mutex = (osMutexId_t)rdamutex;
    osStatus res;
    res = osMutexWait(mutex, millisec);
    if (res == osOK) {
        return NO_ERR;
    } else {
        return ERR;
    }
}

int rda_mutex_realease(void *rdamutex)
{
    osMutexId_t mutex = (osMutexId_t)rdamutex;
    osStatus res;
    res = osMutexRelease(mutex);
    if(res == osOK) {
        return NO_ERR;
    } else {
        return ERR;
    }
}

int rda_mutex_delete(void *rdamutex)
{
    osMutexId_t mutex = (osMutexId_t)rdamutex;
    osStatus res;
    res = osMutexDelete(mutex);
    free(mutex);
    if (res == osOK) {
        return NO_ERR;
    } else {
        return ERR;
    }
}

/* Thread */
void* rda_thread_new(const char *pcName,
                            void (*thread)(void *arg),
                            void *arg, int stacksize, int priority)
{
    osThreadId_t     id;
    osThreadAttr_t   _attr = { 0 };
    mbed_rtos_storage_thread_t *_obj_mem = (mbed_rtos_storage_thread_t *)malloc(sizeof(mbed_rtos_storage_thread_t));
    MBED_ASSERT(_obj_mem != NULL);
    memset(_obj_mem, 0, sizeof(mbed_rtos_storage_thread_t));
    _attr.priority = priority;
    _attr.stack_size = stacksize;
    _attr.name = pcName ? pcName : "rda_unnamed_thread";
    _attr.stack_mem = malloc(stacksize);
    MBED_ASSERT(_attr.stack_mem != NULL);
    _attr.cb_size = sizeof(mbed_rtos_storage_thread_t);
    _attr.cb_mem = _obj_mem;
    _attr.tz_module = 0;
    
    //Fill the stack with a magic word for maximum usage checking
    for (uint32_t i = 0; i < (_attr.stack_size / sizeof(uint32_t)); i++) {
        ((uint32_t *)_attr.stack_mem)[i] = 0xE25A2EA5;
    }
    id = osThreadNew(thread, arg, &_attr);
    if (id == NULL){
        free(_attr.stack_mem);
        free(_attr.cb_mem);
        RDA_SYS_PRINT("sys_thread_new create error\n");
        return NULL;
    }
    return (void *)id;
}

int rda_thread_delete(void* id)
{
    osStatus ret;
    unsigned int *stk = ((osRtxThread_t *)id)->stack_mem;
    ret = osThreadTerminate(id);
    free(id);
    free(stk);
    if (ret != osOK) {
        return ERR;
    }
    return NO_ERR;
}

void* rda_thread_get_id(void)
{
    osThreadId id = osThreadGetId();
    return (void*)id;
}
#endif
