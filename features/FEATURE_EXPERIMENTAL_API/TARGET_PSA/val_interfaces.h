/** @file
 * Copyright (c) 2018-2019, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#ifndef _VAL_INTERFACES_H_
#define _VAL_INTERFACES_H_

#include "val.h"
#include "val_client_defs.h"
#include "pal_interfaces_ns.h"

/* typedef's */
typedef struct {
    val_status_t (*print)(print_verbosity_t verbosity,
                          const char *string, uint32_t data);
    val_status_t (*set_status)(uint32_t status);
    uint32_t (*get_status)(void);
    void (*test_init)(uint32_t test_num,  char8_t *desc,
                      uint32_t test_bitfield);
    void (*test_exit)(void);
    val_status_t (*err_check_set)(uint32_t checkpoint, val_status_t status);
    val_status_t (*target_get_config)(cfg_id_t cfg_id, uint8_t **data, uint32_t *size);
    val_status_t (*execute_non_secure_tests)(uint32_t test_num, client_test_t *tests_list,
                                             bool_t server_hs);
    val_status_t (*switch_to_secure_client)(uint32_t test_num);
    val_status_t (*execute_secure_test_func)(psa_handle_t *handle, test_info_t test_info,
                                             uint32_t sid);
    val_status_t (*ipc_connect)(uint32_t sid, uint32_t minor_version,
                                psa_handle_t *handle);
    val_status_t (*ipc_call)(psa_handle_t handle, psa_invec *in_vec,
                             size_t in_len,  psa_outvec *out_vec,
                             size_t out_len);
    void (*ipc_close)(psa_handle_t handle);
    val_status_t (*get_secure_test_result)(psa_handle_t *handle);
    val_status_t (*nvmem_read)(uint32_t offset, void *buffer, int size);
    val_status_t (*nvmem_write)(uint32_t offset, void *buffer, int size);
    val_status_t (*wd_timer_init)(wd_timeout_type_t timeout_type);
    val_status_t (*wd_timer_enable)(void);
    val_status_t (*wd_timer_disable)(void);
    val_status_t (*wd_reprogram_timer)(wd_timeout_type_t timeout_type);
    val_status_t (*set_boot_flag)(boot_state_t state);
    val_status_t (*get_boot_flag)(boot_state_t *state);
    int32_t (*crypto_function)(int type, ...);
    uint32_t (*its_function)(int type, ...);
    uint32_t (*ps_function)(int type, ...);
    int32_t (*attestation_function)(int type, ...);
} val_api_t;

typedef struct {
    uint32_t (*framework_version)(void);
    uint32_t (*version)(uint32_t sid);
    psa_handle_t (*connect)(uint32_t sid, uint32_t minor_version);
    psa_status_t (*call)(psa_handle_t handle,
                         const psa_invec *in_vec,
                         size_t in_len,
                         psa_outvec *out_vec,
                         size_t out_len
                        );
    void (*close)(psa_handle_t handle);
} psa_api_t;

typedef void (*test_fptr_t)(val_api_t *val, psa_api_t *psa);

typedef struct {
    test_id_t     test_id;
    test_fptr_t   entry_addr;
} val_test_info_t;

void test_entry(val_api_t *val, psa_api_t *psa);
#endif
