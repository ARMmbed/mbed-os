/** @file
 * Copyright (c) 2019, Arm Limited or its affiliates. All rights reserved.
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
#ifndef _TEST_S001_ITS_DATA_TESTS_H_
#define _TEST_S001_ITS_DATA_TESTS_H_

#include "val_internal_trusted_storage.h"

#define SST_FUNCTION val->its_function
#define psa_sst_uid_t psa_its_uid_t

typedef struct {
    enum its_function_code  api;
    psa_its_status_t        status;
} test_data;

static struct psa_its_info_t info;
static const test_data s001_data[] = {
{
 0, 0 /* Unused Index0 */
},
{
 VAL_ITS_GET, PSA_ITS_ERROR_UID_NOT_FOUND /* Call the get API when no UID is set */
},
{
 VAL_ITS_GET_INFO, PSA_ITS_ERROR_UID_NOT_FOUND /* Call the get_info API when no UID is set */
},
{
 VAL_ITS_REMOVE, PSA_ITS_ERROR_UID_NOT_FOUND /* Call the remove API when no UID is set */
},
{
 VAL_ITS_SET, PSA_ITS_SUCCESS /* Create a valid storage entity with UID1 */
},
{
 VAL_ITS_SET, PSA_ITS_SUCCESS /* Create a valid storage entity with UID2 */
},
{
 VAL_ITS_REMOVE, PSA_ITS_SUCCESS /* Remove UID1 */
},
{
 VAL_ITS_GET, PSA_ITS_ERROR_UID_NOT_FOUND /* Call get API for UID1 */
},
{
 VAL_ITS_GET_INFO, PSA_ITS_ERROR_UID_NOT_FOUND /* Call get_info API for UID1 */
},
{
 VAL_ITS_REMOVE, PSA_ITS_ERROR_UID_NOT_FOUND /* Call remove API for UID1 */
},
{
 VAL_ITS_SET, PSA_ITS_SUCCESS /* Create a valid storage entity again with UID1 */
},
{
 VAL_ITS_GET, PSA_ITS_ERROR_UID_NOT_FOUND /* Call get API for UID not same as UID1 or UID2 */
},
{
 VAL_ITS_GET_INFO, PSA_ITS_ERROR_UID_NOT_FOUND /* Call get_info for UID not same as UID1 or UID2 */
},
{
 VAL_ITS_REMOVE, PSA_ITS_ERROR_UID_NOT_FOUND /* Call remove API for UID not same as UID1 or UID2 */
},
{
 VAL_ITS_REMOVE, PSA_ITS_SUCCESS /* Remove UID1 */
},
{
 VAL_ITS_REMOVE, PSA_ITS_SUCCESS /* Remove UID2 */
},
};
#endif /* _TEST_S001_ITS_DATA_TESTS_H_ */
