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
#ifndef _TEST_S004_PS_DATA_TESTS_H_
#define _TEST_S004_PS_DATA_TESTS_H_

#include "val_protected_storage.h"

#define SST_FUNCTION val->ps_function
#define psa_sst_uid_t psa_ps_uid_t

typedef struct {
    enum ps_function_code  api;
    psa_ps_status_t        status;
} test_data;

static const test_data s004_data[] = {
{
 0, 0 /* This is dummy for index0 */
},
{
 VAL_PS_SET, PSA_PS_SUCCESS /* Create a valid storage entity */
},
{
 VAL_PS_GET, PSA_PS_SUCCESS /* Validate the data using get API after set API failure */
},
{
 0, 0 /* Index not used */
},
{
 VAL_PS_SET, PSA_PS_SUCCESS /* For same UID set the length as half of previous */
},
{
 VAL_PS_GET, PSA_PS_ERROR_INCORRECT_SIZE /* Call get with incorrect length */
},
{
 0, 0 /* No data should be returned */
},
{
 VAL_PS_GET, PSA_PS_SUCCESS /* Call get API with correct length */
},
{
 0, 0 /* No data should be returned */
},
{
 0, 0 /* Check that we should not be able to access the old data */
},
{
 VAL_PS_REMOVE, PSA_PS_SUCCESS /* Remove the valid storage entity */
},
};
#endif /* _TEST_S004_PS_DATA_TESTS_H_ */
