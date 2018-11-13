/* Copyright (c) 2017 ARM Limited
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

#ifndef PSA_SERVER_TEST_PART1_PARTITION_H
#define PSA_SERVER_TEST_PART1_PARTITION_H

#define SERVER_TEST_PART1_ID 2

#define SERVER_TEST_PART1_ROT_SRV_COUNT (2UL)
#define SERVER_TEST_PART1_EXT_ROT_SRV_COUNT (2UL)

/* SERVER_TEST_PART1 event flags */
#define SERVER_TEST_PART1_RESERVED1_POS (1UL)
#define SERVER_TEST_PART1_RESERVED1_MSK (1UL << SERVER_TEST_PART1_RESERVED1_POS)

#define SERVER_TEST_PART1_RESERVED2_POS (2UL)
#define SERVER_TEST_PART1_RESERVED2_MSK (1UL << SERVER_TEST_PART1_RESERVED2_POS)



#define CONTROL_MSK_POS (4UL)
#define CONTROL_MSK (1UL << CONTROL_MSK_POS)
#define TEST_MSK_POS (5UL)
#define TEST_MSK (1UL << TEST_MSK_POS)

#define SERVER_TEST_PART1_WAIT_ANY_SID_MSK (\
    CONTROL_MSK | \
    TEST_MSK)

/*
#define SERVER_TEST_PART1_WAIT_ANY_MSK (\
    SERVER_TEST_PART1_WAIT_ANY_SID_MSK) | \
    PSA_DOORBELL)
*/


#endif // PSA_SERVER_TEST_PART1_PARTITION_H
