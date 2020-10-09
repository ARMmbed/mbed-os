/** @file
 * Copyright (c) 2019, Arm Limited or ps affiliates. All rights reserved.
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
#ifndef _TEST_S009_PS_DATA_TESTS_H_
#define _TEST_S009_PS_DATA_TESTS_H_

#include "val_protected_storage.h"

#define SST_FUNCTION val->ps_function
#define psa_sst_uid_t psa_ps_uid_t

typedef struct {
    enum ps_function_code  api;
    psa_ps_status_t        status;
} test_data;

static struct psa_ps_info_t info;
static const test_data s009_data[] = {
{
 0, 0 /* This is dummy for index0 */
},
{
 VAL_PS_SET, PSA_PS_SUCCESS /* Call set API with NULL write buffer and 0 length */
},
{
 VAL_PS_GET_INFO, PSA_PS_SUCCESS /* Verify UID is created */
},
{
 VAL_PS_GET, PSA_PS_SUCCESS /* Call get API with NULL write buffer and 0 length */
},
{
 VAL_PS_REMOVE, PSA_PS_SUCCESS /* Remove the storage entity */
},
{
 VAL_PS_GET_INFO, PSA_PS_ERROR_UID_NOT_FOUND /* Verify UID is removed */
},
{
 VAL_PS_SET, PSA_PS_SUCCESS /* Create storage of zero size and valid write buffer */
},
{
 VAL_PS_GET_INFO, PSA_PS_SUCCESS /* Call get_info API to check data size */
},
{
 0, 0 /* This is dummy for index8 */
},
{
 VAL_PS_GET, PSA_PS_SUCCESS /* Call get API with 0 length and NULL read buffer */
},
{
 VAL_PS_SET, PSA_PS_SUCCESS /* Increase the asset size */
},
{
 VAL_PS_REMOVE, PSA_PS_SUCCESS /* Remove the storage entity */
},
};
#endif /* _TEST_S009_PS_DATA_TESTS_H_ */
