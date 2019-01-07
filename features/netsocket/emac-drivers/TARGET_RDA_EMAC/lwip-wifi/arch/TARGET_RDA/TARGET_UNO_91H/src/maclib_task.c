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

/******************************************************************************
 * @file   : maclib_task.c
 * @brief  : WiFi MACLib task source file
 * @version: V1.0
 * @date   : 6. May 2017
 *
 * @note
 *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mbed_interface.h"
#include "cmsis_os.h"
#include "csl_mbed.h"
#include "maclib_task.h"
#include "rda5991h_wland.h"

#include "lwip/pbuf.h"
#include "lwip/sys.h"

/**
 * Macros
 */
#define MACLIB_TASK_DEBUG   (0)

/**
 * Variables
 */
static int maclib_task_run = 0;
static sys_mbox_t maclib_mbox;
static int g_event_num = 0;
static int g_event_proc_done = 1;
static sys_sem_t g_maclib_sem_sleep;
static int g_maclib_sleep_flag = 0;

extern maclib_func_t *maclib_func_p;

extern void rda_critical_sec_start(void);
extern void rda_critical_sec_end(void);
extern void wland_set_sta_sleep(unsigned char is_sleep);

#define MAX_MSG_POOL_NUM    (64)
maclib_msg_t msg_str_pool[MAX_MSG_POOL_NUM];
int msg_str_pool_inited = 0;

void init_msg_str_pool(void)
{
    int idx;
    for(idx = 0; idx < MAX_MSG_POOL_NUM; idx++) {
        msg_str_pool[idx].is_free = 1;
    }
}

maclib_msg_t *alloc_msg_str(void)
{
    int idx;
    maclib_msg_t *ret = NULL;
    rda_critical_sec_start();
    if (0 == msg_str_pool_inited) {
        init_msg_str_pool();
        msg_str_pool_inited = 1;
    }
    rda_critical_sec_end();
    for (idx = 0; idx < MAX_MSG_POOL_NUM; idx++) {
        rda_critical_sec_start();
        ret = &msg_str_pool[idx];
        if (1 == ret->is_free) {
            ret->is_free = 0;
            rda_critical_sec_end();
            break;
        }
        rda_critical_sec_end();
    }
    return ret;
}

void free_msg_str(maclib_msg_t *p_msg)
{
    rda_critical_sec_start();
    p_msg->is_free = 1;
    rda_critical_sec_end();
}

/**
 * Functions
 */
/* maybe called in isr, should not use "printf", "malloc" */
void mbed_event_handle_cb(unsigned int event)
{
    MACLIB_EVENT_HANDLE_T type = (MACLIB_EVENT_HANDLE_T)event;
    if ((maclib_task_run == 0) && (MACLIB_EVENT_CLEANUP != type)) {
        mbed_error_printf("evntHndlCb_nulldata\r\n");
        return;
    }
    switch(type) {
        case MACLIB_EVENT_PEND:
            rda_critical_sec_start();
            g_event_num++;
            if((1 == g_event_num) && (1 == g_event_proc_done)) {
                maclib_msg_t *msg;
#if MACLIB_TASK_DEBUG
                mbed_error_printf("#1-1,%d(%08X)\r\n", g_event_num, __get_xPSR());
#endif
                msg = alloc_msg_str();
                if(NULL == msg) {
                    mbed_error_printf("malloc err\r\n");
                    return;
                }
                msg->type = MACLIB_MSG_EVNT_HNDL;
                msg->msg = NULL;
                sys_mbox_trypost(&(maclib_mbox), msg);
#if MACLIB_TASK_DEBUG
                mbed_error_printf("#1-2\r\n");
#endif
            }
            rda_critical_sec_end();
            break;
        case MACLIB_EVENT_PROCESS:
#if 1
            rda_critical_sec_start();
            g_event_num--;
            if(0 > g_event_num) {
                mbed_error_printf("event handle err\r\n");
                g_event_num = 0;
            }
            rda_critical_sec_end();
#if MACLIB_TASK_DEBUG
            mbed_error_printf("#3,%d\r\n",g_event_num);
#endif
#endif
            break;
        case MACLIB_EVENT_CLEANUP:
#if MACLIB_TASK_DEBUG
            mbed_error_printf("event cleanup\r\n");
#endif
            rda_critical_sec_start();
            g_event_num = 0;
            rda_critical_sec_end();
            break;
        default:
            break;
    }
}

void mbed_mltask_sleep_cb(void)
{
    g_maclib_sleep_flag = 1;
    sys_arch_sem_wait(&g_maclib_sem_sleep, 0);
}

void mbed_mltask_wakeup_cb(void)
{
    rda_critical_sec_start();
    if (1 == g_maclib_sleep_flag) {
        g_maclib_sleep_flag = 0;
        sys_sem_signal(&g_maclib_sem_sleep);
    }
    rda_critical_sec_end();
}

void maclib_check_status(void)
{
    rda_critical_sec_start();
    if (1 == g_maclib_sleep_flag) {
        if(*((unsigned int *)0x40020580U) & (0x01UL << 2)) {
            mbed_mltask_wakeup_cb();
        }
    }
    rda_critical_sec_end();
}

int maclib_is_sleep_allowed(void)
{
    return g_maclib_sleep_flag;
}

void maclib_task(void *pvParameters)
{
    int ret;
#if 0 
    sleep_entry_t maclib_sleep_entry = {
        wland_set_sta_sleep,
        maclib_is_sleep_allowed,
        maclib_check_status
    };
#endif
    sys_sem_new(&g_maclib_sem_sleep, 0);
    //sleep_entry_register(&maclib_sleep_entry);

    ret = sys_mbox_new(&(maclib_mbox), 8);
    if(0 != ret) {
        LWIP_DEBUGF(NETIF_DEBUG,"msgbox init err!\r\n");
        goto mac_lib_err;
    }
#if MACLIB_TASK_DEBUG
    LWIP_DEBUGF(NETIF_DEBUG,"#mbox new\r\n");
#endif
    maclib_task_run = 1;
    while(1) {
        int mem_free = 1;
        maclib_msg_t *msg = NULL;
        unsigned int time = sys_arch_mbox_fetch(&(maclib_mbox), (void **)&msg, 0);
        if ((SYS_ARCH_TIMEOUT == time) || (NULL == msg)) {
            LWIP_DEBUGF(NETIF_DEBUG, "ml_task: invalid msg\r\n");
            continue;
        }
        switch(msg->type) {
            case MACLIB_MSG_EVNT_HNDL: {
                rda_critical_sec_start();
                g_event_proc_done = 0;
                rda_critical_sec_end();
#if MACLIB_TASK_DEBUG
                mbed_error_printf("#get event %d\r\n", g_event_num);
#endif
                maclib_func_p->ml_tasklet();
#if MACLIB_TASK_DEBUG
                mbed_error_printf("#5\r\n");
#endif
                rda_critical_sec_start();
#if 0
                g_event_num--;
                if(0 > g_event_num) {
                    mbed_error_printf("event handle err\r\n");
                } else
#endif
                g_event_proc_done = 1;
                if(0 < g_event_num) {
#if MACLIB_TASK_DEBUG
                    mbed_error_printf("#2-1\r\n");
#endif
                    sys_mbox_trypost(&(maclib_mbox), msg);
#if MACLIB_TASK_DEBUG
                    mbed_error_printf("#2-2\r\n");
#endif
                    mem_free = 0;
                }
                rda_critical_sec_end();
#if MACLIB_TASK_DEBUG
                mbed_error_printf("#pDone\r\n");
#endif
                break;
            }
            default:
                break;
        }
        if (mem_free) {
            free_msg_str(msg);
#if MACLIB_TASK_DEBUG
            mbed_error_printf("#4\r\n");
#endif
        }
    }

mac_lib_err:
    LWIP_DEBUGF(NETIF_DEBUG,"MACLib exit!\r\n");
    osDelay(osWaitForever);
}
