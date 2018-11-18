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

#ifndef PSA_CLIENT_TESTS_PART1_PARTITION_H
#define PSA_CLIENT_TESTS_PART1_PARTITION_H

#define CLIENT_TESTS_PART1_ID 1

#define CLIENT_TESTS_PART1_ROT_SRV_COUNT (3UL)
#define CLIENT_TESTS_PART1_EXT_ROT_SRV_COUNT (0UL)

/* CLIENT_TESTS_PART1 event flags */
#define CLIENT_TESTS_PART1_RESERVED1_POS (1UL)
#define CLIENT_TESTS_PART1_RESERVED1_MSK (1UL << CLIENT_TESTS_PART1_RESERVED1_POS)

#define CLIENT_TESTS_PART1_RESERVED2_POS (2UL)
#define CLIENT_TESTS_PART1_RESERVED2_MSK (1UL << CLIENT_TESTS_PART1_RESERVED2_POS)



#define PART1_ROT_SRV1_MSK_POS (4UL)
#define PART1_ROT_SRV1_MSK (1UL << PART1_ROT_SRV1_MSK_POS)
#define DROP_CONN_MSK_POS (5UL)
#define DROP_CONN_MSK (1UL << DROP_CONN_MSK_POS)
#define SECURE_CLIENTS_ONLY_MSK_POS (6UL)
#define SECURE_CLIENTS_ONLY_MSK (1UL << SECURE_CLIENTS_ONLY_MSK_POS)

#define CLIENT_TESTS_PART1_WAIT_ANY_SID_MSK (\
    PART1_ROT_SRV1_MSK | \
    DROP_CONN_MSK | \
    SECURE_CLIENTS_ONLY_MSK)

/*
#define CLIENT_TESTS_PART1_WAIT_ANY_MSK (\
    CLIENT_TESTS_PART1_WAIT_ANY_SID_MSK) | \
    PSA_DOORBELL)
*/


#endif // PSA_CLIENT_TESTS_PART1_PARTITION_H
