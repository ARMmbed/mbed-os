/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_nspm_api.h"

#include <string.h>
#include <stdint.h>

#include "cmsis_os2.h"
#include "tfm_ns_svc.h"

/* Translation table pair between OS threads and client IDs */
struct thread_test_clientid_pair {
    const char* t_name;     /*!< Task/Thread name */
    int32_t     client_id;  /*!< Client ID used in assets definition */
};

static struct thread_test_clientid_pair test_ns_policy_table[] =
{
    {"Thread_A", -1},
    {"Thread_B", -2},
    {"Thread_C", -3},
    {"Thread_D", -4},
    {"seq_task", -5},
    {"mid_task", -6},
    {"pri_task", -7},
};

/**
 * \brief Gets the client ID based on the thread name
 *
 * \return If the thread name is in the test_ns_policy_table, it returns the
 *        client ID. Otherwise, it returns 0 as an error code.
 */
static uint32_t get_client_id(void)
{
    uint32_t i;
    static uint32_t test_table_size = (sizeof(test_ns_policy_table) /
                                       sizeof(test_ns_policy_table[0]));
    const char*  p_thread_name;
    osThreadId_t thread_id;

    /* Get thread name */
    thread_id = osThreadGetId();
    p_thread_name = osThreadGetName(thread_id);

    for (i = 0; i < test_table_size; i++) {
        if (strcmp(test_ns_policy_table[i].t_name, p_thread_name) == 0) {
            return test_ns_policy_table[i].client_id;
        }
    }

    return 0;
}

__attribute__ ((naked))
static uint32_t tfm_nspm_svc_register_client(uint32_t client_id)
{
    SVC(SVC_TFM_NSPM_REGISTER_CLIENT_ID);
    __ASM("BX LR");
}

uint32_t tfm_nspm_register_client_id(void)
{
    uint32_t client_id;

    client_id = get_client_id();
    if (client_id == 0) {
        return 0;
    }

    return tfm_nspm_svc_register_client(client_id);
}


