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

/*****************************************************************************/
/*                                                                           */
/*  File Name         : csl_mbed.h                                           */
/*                                                                           */
/*  Description       : This file contains all declarations and functions    */
/*                      related to the chip support library.                 */
/*                                                                           */
/*  MbedOS Usage      : Call maclib_get_funcs_struct() to get MACLib funcs;  */
/*                      Define mbed_reg_func_t var, Register it by ml_init;  */
/*                                                                           */
/*****************************************************************************/

#ifndef CSL_MBED_H
#define CSL_MBED_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/* Constants                                                                 */
/*****************************************************************************/
#define RDA_EXT_INT_MAC_HW_INDEX        8
#define RDA_EXT_INT_MAC_HW_PRI          0x80

/*****************************************************************************/
/* Enums                                                                     */
/*****************************************************************************/
typedef enum {
    MACLIB_EVENT_PEND       = 0,
    MACLIB_EVENT_PROCESS    = 1,
    MACLIB_EVENT_CLEANUP    = 2
} MACLIB_EVENT_HANDLE_T;

/*****************************************************************************/
/* Data Types                                                                */
/*****************************************************************************/
typedef struct
{
    unsigned int   ml_id;      /* Buffer identification */
    unsigned char* ml_data;   /* Pkt start address     */
    unsigned short ml_len;     /* Pkt length            */
} maclib_buf_t;

/* Structure that contains functions provided by MACLib */
typedef struct {
    /* Initialize MAC Library, input param: mbed_reg_func_t *reg_funcs */
    int (*ml_init)(void *reg_funcs);

    /* As a peroid task to process MAC Library background event */
    void (*ml_tasklet)(void);

    /* Get a new packet buffer, output param: maclib_buf_t *buf */
    void (*ml_get_pkt_buf)(void *buf);

    /* Mbed stack send packet to MAC Library, input param: maclib_buf_t *buf*/
    int (*ml_xmit_pkt)(void *buf);

    /* Mbed receive and processing packet done, input param: unsigned int buf_id */
    void (*ml_recv_pkt_comp)(unsigned int buf_id);
} maclib_func_t;

/* Structure that contains functions provided by MbedOS */
typedef struct {
    /* MAC Library send packet to mbed stack, input param: maclib_buf_t *buf */
    void (*mbed_recv_pkt)(void *buf);

    /* Critical section start realized in mbed */
    void (*mbed_critical_sec_start)(void);

    /* Critical section end realized in mbed */
    void (*mbed_critical_sec_end)(void);

    /* Create interrupt in mbed, input param: vector/priority/isr(function), */
    unsigned int (*mbed_critical_sec_counter_get)(void);
    /* return: interrupt handle, non-zero is valid                           */
    void * (*mbed_create_interrupt)(unsigned int vec, unsigned int pri, void *isr);

    /* Delete interrupt in mbed, input param: vector */
    void (*mbed_delete_interrupt)(unsigned int vec);

    /* Enable interrupt in mbed, input param: vector */
    void (*mbed_enable_interrupt)(unsigned int vec);

    /* Disable interrupt in mbed, input param: vector */
    void (*mbed_disable_interrupt)(unsigned int vec);

    /* Get current time realized in mbed, return time in units of micro second */
    unsigned long (*mbed_get_cur_time_ms)(void);

    /* Create alarm in mbed, input param: func(callback)/data(pass to func), */
    /* return: alarm handle, non-zero is valid                               */
    void * (*mbed_create_alarm)(void *func, unsigned int data);

    /* Delete alarm in mbed, input param: alarm handle */
    void (*mbed_delete_alarm)(void **handle);

    /* Start alarm in mbed, input param: alarm handle/timeout(micro second)  */
    void (*mbed_start_alarm)(void *handle, unsigned int timeout_ms);

    /* Stop alarm in mbed, input param: alarm handle */
    void (*mbed_stop_alarm)(void *handle);

#if defined(MBED_MUTEX_INTERFACE)
    /* Create mutex */
    void (*mbed_mutex_create)(void);

    /* Delete mutex */
    unsigned int (*mbed_mutex_delete)(void *rdamutex);

    /* Wait mutex, timer unit : millisec */
    unsigned int (*mbed_mutex_wait)(void *rdamutex, unsigned int millisec);

    /* Release mutex */
    unsigned int (*mbed_mutex_release)(void *rdamutex);
#endif /* MBED_MUTEX_INTERFACE */

    /* Event post/get callback function, input param: event_type */
    void (*mbed_event_hdl_cb)(unsigned int event);

    /* maclib task sleep callback function */
    void (*mbed_task_sleep_cb)(void);

    /* maclib task wakeup callback function */
    void (*mbed_task_wakeup_cb)(void);
} maclib_import_func_t;

/*****************************************************************************/
/* Extern Function Declarations                                              */
/*****************************************************************************/
extern void maclib_get_funcs_struct(maclib_func_t *func_str);

#ifdef __cplusplus
}
#endif

#endif /* CSL_MBED_H */
