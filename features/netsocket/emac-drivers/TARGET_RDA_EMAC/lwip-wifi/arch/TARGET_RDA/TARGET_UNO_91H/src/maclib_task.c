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
static void* maclib_mbox;
static int g_event_num = 0;
static int g_event_proc_done = 1;
static sys_sem_t g_maclib_sem_sleep;
static int g_maclib_sleep_flag = 0;

extern maclib_func_t *maclib_func_p;

extern void rda_critical_sec_start(void);
extern void rda_critical_sec_end(void);
extern void wland_set_sta_sleep(unsigned char is_sleep);
/**
 * Functions
 */
/* maybe called in isr, should not use "printf", "malloc" */
void mbed_event_handle_cb(unsigned int event)
{
    static unsigned int sec_cnt = 0;
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
                maclib_msg_t msg;
#if MACLIB_TASK_DEBUG
                mbed_error_printf("#1-1,%d(%08X)\r\n", g_event_num, __get_xPSR());
#endif
                msg.type = MACLIB_MSG_EVNT_HNDL;
                msg.msg = NULL;
                rda_mail_put(maclib_mbox, (void*)&msg, 0);
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

    maclib_mbox = (void *)rda_mail_create(8, sizeof(maclib_msg_t));//ret = sys_mbox_new(&(maclib_mbox), 8);
    if(NULL == maclib_mbox) {
        mbed_error_printf("msgbox init err!\r\n");
        goto mac_lib_err;
    }
#if MACLIB_TASK_DEBUG
    LWIP_DEBUGF(NETIF_DEBUG,"#mbox new\r\n");
#endif
    maclib_task_run = 1;
    while(1) {
        maclib_msg_t msg;
        osStatus_t status = rda_mail_get(maclib_mbox, (void *)&msg, osWaitForever);
        if(osOK != status) {
            mbed_error_printf("ml_task: invalid msg ret=%08X\r\n", status);
            continue;
        }
        switch(msg.type) {
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
                    rda_mail_put(maclib_mbox, (void*)&msg, 0);
#if MACLIB_TASK_DEBUG
                    mbed_error_printf("#2-2\r\n");
#endif
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
#if MACLIB_TASK_DEBUG
            mbed_error_printf("#4\r\n");
#endif
    }

mac_lib_err:
    LWIP_DEBUGF(NETIF_DEBUG,"MACLib exit!\r\n");
    osDelay(osWaitForever);
}
