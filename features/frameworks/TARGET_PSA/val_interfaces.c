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


#include "val_framework.h"
#include "val_interfaces.h"
#include "val_peripherals.h"
#include "val_target.h"
#include "val_crypto.h"
#include "val_internal_trusted_storage.h"
#include "val_protected_storage.h"
#include "val_attestation.h"

/*VAL APIs to be used by test */
const val_api_t val_api = {
    .print                     = val_print,
    .set_status                = val_set_status,
    .get_status                = val_get_status,
    .test_init                 = val_test_init,
    .test_exit                 = val_test_exit,
    .err_check_set             = val_err_check_set,
    .target_get_config         = val_target_get_config,
    .execute_non_secure_tests  = val_execute_non_secure_tests,
    .switch_to_secure_client   = val_switch_to_secure_client,
    .execute_secure_test_func  = val_execute_secure_test_func,
    .get_secure_test_result    = val_get_secure_test_result,
    .ipc_connect               = val_ipc_connect,
    .ipc_call                  = val_ipc_call,
    .ipc_close                 = val_ipc_close,
    .nvmem_read                = val_nvmem_read,
    .nvmem_write               = val_nvmem_write,
    .wd_timer_init             = val_wd_timer_init,
    .wd_timer_enable           = val_wd_timer_enable,
    .wd_timer_disable          = val_wd_timer_disable,
    .wd_reprogram_timer        = val_wd_reprogram_timer,
    .set_boot_flag             = val_set_boot_flag,
    .get_boot_flag             = val_get_boot_flag,
    .crypto_function           = val_crypto_function,
    .its_function              = val_its_function,
    .ps_function               = val_ps_function,
    .attestation_function      = val_attestation_function,
};

const psa_api_t psa_api = {
    .framework_version     = pal_ipc_framework_version,
    .version               = pal_ipc_version,
    .connect               = pal_ipc_connect,
    .call                  = pal_ipc_call,
    .close                 = pal_ipc_close,
};
