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
#ifndef _TEST_S002_ITS_DATA_TESTS_H_
#define _TEST_S002_ITS_DATA_TESTS_H_

#include "val_internal_trusted_storage.h"

#define SST_FUNCTION val->its_function
#define PSA_SST_FLAG_WRITE_ONCE PSA_ITS_FLAG_WRITE_ONCE
#define psa_sst_uid_t psa_its_uid_t

typedef struct {
    enum its_function_code  api;
    psa_its_status_t        status;
} test_data;

static struct psa_its_info_t orig_info;
static struct psa_its_info_t new_info;
static const test_data s002_data[] = {
{
 0, 0 /* This is dummy for index0 */
},
{
 VAL_ITS_SET, PSA_ITS_SUCCESS /* Create a valid storage with create flag value 0 */
},
{
 VAL_ITS_GET_INFO, PSA_ITS_SUCCESS /* Call the get_info API to validate the attributes */
},
{
 0, 0 /* Index not used as check for get info size */
},
{
 0, 0 /* Index not used as check for get info flag */
},
{
 VAL_ITS_GET, PSA_ITS_SUCCESS /* Validate the data using get API */
},
{
 0, 0 /* Index not used */
},
{
 VAL_ITS_SET, PSA_ITS_SUCCESS /* Change the flag to WRITE_ONCE using set API */
},
{
 VAL_ITS_GET_INFO, PSA_ITS_SUCCESS /* Call the get_info API to validate the flag change */
},
{
 0, 0 /* Index not used as check for get info size */
},
{
 0, 0 /* Index not used as check for get info flag */
},
{
 VAL_ITS_GET, PSA_ITS_SUCCESS /* Validate the data using get API after flag change */
},
{
 0, 0 /* Index not used */
},
{
 VAL_ITS_REMOVE, PSA_ITS_ERROR_WRITE_ONCE /* Storage should not be removed after WRITE_ONCE flag */
},
{
 VAL_ITS_SET, PSA_ITS_SUCCESS /* Create a storage with different UID and flag value WRITE_ONCE */
},
{
 VAL_ITS_REMOVE, PSA_ITS_ERROR_WRITE_ONCE /* Storage should not be removed */
},
{
 VAL_ITS_GET, PSA_ITS_SUCCESS /* Validate the data using get API after flag change */
},
{
 0, 0 /* Index not used */
},
{
 VAL_ITS_GET_INFO, PSA_ITS_SUCCESS /* Call the get_info API to validate the flag change */
},
{
 0, 0 /* Index not used as check for get info size */
},
{
 0, 0 /* Index not used as check for get info flag */
},
{
 VAL_ITS_SET, PSA_ITS_ERROR_WRITE_ONCE /* Try to set different size for same UID and flag value */
},
{
 VAL_ITS_REMOVE, PSA_ITS_ERROR_WRITE_ONCE /* Storage should not be removed */
},
{
 VAL_ITS_GET_INFO, PSA_ITS_SUCCESS /* Call the get_info API to validate the flag change */
},
{
 0, 0 /* Index not used as check for get info size */
},
{
 0, 0 /* Index not used as check for get info flag */
},
{
 VAL_ITS_GET, PSA_ITS_SUCCESS  /* Validate the data using get API after flag change */
},
{
 0, 0 /* Index not used */
},
{
 VAL_ITS_SET, PSA_ITS_ERROR_WRITE_ONCE  /* Setting flag to zero for UID should fail */
},
{
 VAL_ITS_REMOVE, PSA_ITS_ERROR_WRITE_ONCE /* Storage should not be removed */
},
{
 VAL_ITS_GET_INFO, PSA_ITS_SUCCESS  /* Check that the WRITE_ONCE flag is preserved */
},
{
 0, 0 /* Index not used as check for get info size */
},
{
 0, 0 /* Index not used as check for get info flag */
},
};
#endif /* _TEST_S002_ITS_DATA_TESTS_H_ */
