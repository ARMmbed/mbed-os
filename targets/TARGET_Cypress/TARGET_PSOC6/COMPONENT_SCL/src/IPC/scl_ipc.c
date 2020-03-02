/*
 * Copyright 2018-2020 Cypress Semiconductor Corporation
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

/** @file
 *  Provides SCL functionality to communicate with Network Processor
 */
#include "scl_ipc.h"
#include "scl_buffer_api.h"
#include "cyabs_rtos.h"
#include "mbed_wait_api.h"
#include "string.h"
#include "nsapi_types.h"
/******************************************************
 **                      Macros
 *******************************************************/
#define SCL_THREAD_STACK_SIZE      (1000)
#define SCL_THREAD_PRIORITY        (CY_RTOS_PRIORITY_HIGH)
#define SCL_INTR_SRC               (cpuss_interrupts_ipc_4_IRQn)
#define SCL_INTR_PRI               (1)
#define SCL_RX_CHANNEL             (4)
#define SCL_CHANNEL_NOTIFY_INTR    ((1 << SCL_RX_CHANNEL) << 16)
#define SCL_NOTIFY                 (1 << SCL_TX_CHANNEL)
#define SCL_LOCK_ACQUIRE_STATUS    (0x80000000)
#define SCL_TX_CHANNEL             (3)
#define SCL_RELEASE                (0)
#define DELAY_TIME                 (1000)
#define SEMAPHORE_MAXCOUNT         (1)
#define SEMAPHORE_INITCOUNT        (0)
/******************************************************
 **               Function Declarations
 *******************************************************/
static void scl_isr(void);
static void scl_config(void);
static void scl_rx_handler(void);
static scl_result_t scl_thread_init(void);
scl_result_t scl_get_nw_parameters(network_params_t *nw_param);
scl_result_t scl_send_data(int index, char *buffer, uint32_t timeout);
scl_result_t scl_end(void);
scl_result_t scl_init(void);
/******************************************************
 *        Variables Definitions
 *****************************************************/
/* Structure of SCL thread info
 *   scl_thread_quit_flag:      flag used to determine if thread is to be quit
 *   scl_inited:                flag used to determine if thread is started
 *   scl_thread:                variable for thread handle
 *   scl_thread_stack_start:    pointer to start of thread stack
 *   scl_thread_stack_size:     size of thread stack
 *   scl_thread_priority:       priority of thread
 *   scl_rx_ready:              semaphore for blocking the thread
 */
struct scl_thread_info_t {
    volatile scl_bool_t scl_inited;
    volatile scl_bool_t scl_thread_quit_flag;
    void *scl_thread_stack_start;
    cy_thread_t scl_thread;
    cy_semaphore_t scl_rx_ready;
    uint32_t scl_thread_stack_size;
    cy_thread_priority_t scl_thread_priority;
};
struct scl_thread_info_t g_scl_thread_info;

/******************************************************
 *               Function Definitions
 ******************************************************/

/** ISR for the IPC receiver channel interrupt
 */
static void scl_isr(void)
{
    IPC_INTR_STRUCT_Type *scl_rx_intr = NULL;
    scl_rx_intr = Cy_IPC_Drv_GetIntrBaseAddr(SCL_RX_CHANNEL);

    if (REG_IPC_INTR_STRUCT_INTR_MASKED(scl_rx_intr) & SCL_CHANNEL_NOTIFY_INTR) {
        REG_IPC_INTR_STRUCT_INTR(scl_rx_intr) |= SCL_CHANNEL_NOTIFY_INTR;
        if (g_scl_thread_info.scl_inited == SCL_TRUE) {
            cy_rtos_set_semaphore(&g_scl_thread_info.scl_rx_ready, true);
        }
    }
}
/** Configures the IPC interrupt channel
 */
static void scl_config(void)
{
    IPC_INTR_STRUCT_Type *scl_rx_intr = NULL;
    cy_stc_sysint_t intrCfg = {
        .intrSrc = SCL_INTR_SRC,
        .intrPriority = SCL_INTR_PRI
    };

    scl_rx_intr = Cy_IPC_Drv_GetIntrBaseAddr(SCL_RX_CHANNEL);
    REG_IPC_INTR_STRUCT_INTR_MASK(scl_rx_intr) |= SCL_CHANNEL_NOTIFY_INTR;
    Cy_SysInt_Init(&intrCfg, &scl_isr);
    NVIC_EnableIRQ(intrCfg.intrSrc);
}
/** Create the SCL thread and initialize the semaphore for handling the events from Network Processor
 *
 *  @return  SCL_SUCCESS on successful initialization of thread or SCL_ERROR on failure
 */
static scl_result_t scl_thread_init(void)
{
    cy_rslt_t retval, tmp = 0;
    memset(&g_scl_thread_info, 0, sizeof(g_scl_thread_info));
    g_scl_thread_info.scl_thread_stack_start = (uint8_t *) malloc(SCL_THREAD_STACK_SIZE);;
    g_scl_thread_info.scl_thread_stack_size = (uint32_t) SCL_THREAD_STACK_SIZE;
    g_scl_thread_info.scl_thread_priority = (cy_thread_priority_t) SCL_THREAD_PRIORITY;

    if (g_scl_thread_info.scl_inited != SCL_TRUE) {
        retval = cy_rtos_init_semaphore(&g_scl_thread_info.scl_rx_ready, SEMAPHORE_MAXCOUNT, SEMAPHORE_INITCOUNT);
        if (retval != SCL_SUCCESS) {
            return SCL_ERROR;
        }
        retval = cy_rtos_create_thread(&g_scl_thread_info.scl_thread, (cy_thread_entry_fn_t) scl_rx_handler,
                                       "SCL_thread", g_scl_thread_info.scl_thread_stack_start,
                                       g_scl_thread_info.scl_thread_stack_size,
                                       g_scl_thread_info.scl_thread_priority, (uint32_t) tmp);
        if (retval != SCL_SUCCESS) {
            return SCL_ERROR;
        }
        g_scl_thread_info.scl_inited = SCL_TRUE;
    } else {
        return SCL_ERROR;
    }
    return SCL_SUCCESS;
}

scl_result_t scl_init(void)
{
    scl_result_t retval = SCL_SUCCESS;
    uint32_t configuration_parameters = 0;
#ifdef MBED_CONF_TARGET_NP_CLOUD_DISABLE
    configuration_parameters = (MBED_CONF_TARGET_NP_CLOUD_DISABLE << 1);
#else
    configuration_parameters = (false << 1);
#endif
#ifdef MBED_CONF_TARGET_NP_WIFI_ENABLE
    configuration_parameters |= MBED_CONF_TARGET_NP_WIFI_ENABLE;
#else
    configuration_parameters |= false;
#endif
    //SCL_LOG("configuration_parameters = %lu\n", configuration_parameters);
    scl_config();
    if (g_scl_thread_info.scl_inited != SCL_TRUE) {
        retval = scl_thread_init();
        if (retval != SCL_SUCCESS) {
            SCL_LOG(("Thread init failed\n"));
            return SCL_ERROR;
        } else {
            retval = scl_send_data(SCL_TX_CONFIG_PARAMETERS, (char *) &configuration_parameters, TIMER_DEFAULT_VALUE);
            return retval;
        }
    }
    return SCL_SUCCESS;
}

scl_result_t scl_send_data(int index, char *buffer, uint32_t timeout)
{
    uint32_t acquire_state;
    IPC_STRUCT_Type *scl_send = NULL;
    uint32_t delay_timeout;

    SCL_LOG(("scl_send_data index = %d\n", index));
    scl_send = Cy_IPC_Drv_GetIpcBaseAddress(SCL_TX_CHANNEL);
    CHECK_BUFFER_NULL(buffer);
    if (!(REG_IPC_STRUCT_LOCK_STATUS(scl_send) & SCL_LOCK_ACQUIRE_STATUS)) {
        acquire_state = REG_IPC_STRUCT_ACQUIRE(scl_send);
        if (!(acquire_state & SCL_LOCK_ACQUIRE_STATUS)) {
            SCL_LOG(("IPC Channel 3 Acquired Failed\r\n"));
            return SCL_ERROR;
        }
        REG_IPC_STRUCT_DATA0(scl_send) = index;
        REG_IPC_STRUCT_DATA1(scl_send) = (uint32_t) buffer;
        REG_IPC_STRUCT_NOTIFY(scl_send) = SCL_NOTIFY;
        delay_timeout = 0;
        while ((REG_IPC_STRUCT_LOCK_STATUS(scl_send) & SCL_LOCK_ACQUIRE_STATUS) && delay_timeout <= timeout) {
            wait_us(DELAY_TIME);
            delay_timeout++;
        }
        if (delay_timeout > timeout) {
            REG_IPC_STRUCT_RELEASE(scl_send) = SCL_RELEASE;
            delay_timeout = 0;
            return SCL_ERROR;
        } else {
            return SCL_SUCCESS;
        }
    } else {
        SCL_LOG(("unable to acquire lock\n"));
        return SCL_ERROR;
    }
}

scl_result_t scl_end(void)
{
    scl_result_t retval = SCL_SUCCESS;
    if (g_scl_thread_info.scl_inited == SCL_TRUE) {
        retval = (scl_result_t) cy_rtos_terminate_thread(&g_scl_thread_info.scl_thread);
        if (retval == SCL_SUCCESS) {
            retval = (scl_result_t) cy_rtos_join_thread(&g_scl_thread_info.scl_thread);
            if (retval == SCL_SUCCESS) {
                retval = (scl_result_t) cy_rtos_deinit_semaphore(&g_scl_thread_info.scl_rx_ready);
                if (retval == SCL_SUCCESS) {
                    g_scl_thread_info.scl_inited = SCL_FALSE;
                }
            }
        }
    }
    return retval;
}

/** Thread to handle the received buffer
 */
static void scl_rx_handler(void)
{
    char *buffer = NULL;
    nsapi_connection_status_t connection_status;
    uint32_t index;
    IPC_STRUCT_Type *scl_receive = NULL;
    scl_buffer_t cp_buffer;
    scl_buffer_t scl_buffer;
    uint32_t rx_ipc_size;
    struct rx_ipc_info {
        uint32_t size;
        int *buf_alloc;
    }*rx_cp = NULL;

    SCL_LOG(("Starting CP Rx thread\r\n"));
    scl_receive = Cy_IPC_Drv_GetIpcBaseAddress(SCL_RX_CHANNEL);

    while (SCL_TRUE) {
        cy_rtos_get_semaphore(&g_scl_thread_info.scl_rx_ready, CY_RTOS_NEVER_TIMEOUT, SCL_FALSE);
        index = (uint32_t)REG_IPC_STRUCT_DATA0(scl_receive);
        SCL_LOG(("scl_rx_handler index = %lu\n", index));
        switch (index) {
            case SCL_RX_DATA: {
                rx_cp = (struct rx_ipc_info *) REG_IPC_STRUCT_DATA1(scl_receive);
                scl_buffer = rx_cp->buf_alloc;
                REG_IPC_STRUCT_RELEASE(scl_receive) = SCL_RELEASE;
                SCL_LOG(("scl_buffer = %p\n", scl_buffer));
                scl_network_process_ethernet_data(scl_buffer);
                break;
            }
            case SCL_RX_TEST_MSG: {
                buffer = (char *) REG_IPC_STRUCT_DATA1(scl_receive);
                SCL_LOG(("%s\r\n", (char *) buffer));
                REG_IPC_STRUCT_RELEASE(scl_receive) = SCL_RELEASE;
                break;
            }
            case SCL_RX_GET_BUFFER: {
                rx_ipc_size = (uint32_t) REG_IPC_STRUCT_DATA1(scl_receive);
                scl_host_buffer_get(&cp_buffer, SCL_NETWORK_RX, rx_ipc_size, SCL_FALSE);
                REG_IPC_STRUCT_DATA1(scl_receive) = (uint32_t)cp_buffer;
                REG_IPC_STRUCT_RELEASE(scl_receive) = SCL_RELEASE;
                break;
            }
            case SCL_RX_GET_CONNECTION_STATUS: {
                connection_status = (nsapi_connection_status_t) REG_IPC_STRUCT_DATA1(scl_receive);
                if (connection_status == NSAPI_STATUS_GLOBAL_UP) {
                    scl_emac_wifi_link_state_changed(true);
                } else {
                    scl_emac_wifi_link_state_changed(false);
                }
                SCL_LOG(("connection status = %d\n", connection_status));
                break;
            }
            default: {
                SCL_LOG(("incorrect IPC from Network Processor\n"));
                REG_IPC_STRUCT_RELEASE(scl_receive) = SCL_RELEASE;
                break;
            }
        }
    }
}

scl_result_t scl_get_nw_parameters(network_params_t *nw_param)
{
    scl_result_t status = SCL_ERROR;
    status = scl_send_data(SCL_TX_WIFI_NW_PARAM, (char *)nw_param, TIMER_DEFAULT_VALUE);
    return status;
}
