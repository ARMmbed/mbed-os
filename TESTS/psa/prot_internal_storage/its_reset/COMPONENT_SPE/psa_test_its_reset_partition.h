/* Copyright (c) 2017-2018 ARM Limited
 *
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

/***********************************************************************************************************************
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * THIS FILE IS AN AUTO-GENERATED FILE - DO NOT MODIFY IT.
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 **********************************************************************************************************************/

#ifndef PSA_TEST_ITS_RESET_PARTITION_H
#define PSA_TEST_ITS_RESET_PARTITION_H

#define TEST_ITS_RESET_ID 11

#define TEST_ITS_RESET_ROT_SRV_COUNT (1UL)
#define TEST_ITS_RESET_EXT_ROT_SRV_COUNT (0UL)

/* TEST_ITS_RESET event flags */
#define TEST_ITS_RESET_RESERVED1_POS (1UL)
#define TEST_ITS_RESET_RESERVED1_MSK (1UL << TEST_ITS_RESET_RESERVED1_POS)

#define TEST_ITS_RESET_RESERVED2_POS (2UL)
#define TEST_ITS_RESET_RESERVED2_MSK (1UL << TEST_ITS_RESET_RESERVED2_POS)



#define TEST_PSA_ITS_RESET_MSK_POS (4UL)
#define TEST_PSA_ITS_RESET_MSK (1UL << TEST_PSA_ITS_RESET_MSK_POS)

#define TEST_ITS_RESET_WAIT_ANY_SID_MSK (\
    TEST_PSA_ITS_RESET_MSK)

/*
#define TEST_ITS_RESET_WAIT_ANY_MSK (\
    TEST_ITS_RESET_WAIT_ANY_SID_MSK) | \
    PSA_DOORBELL)
*/


#endif // PSA_TEST_ITS_RESET_PARTITION_H
