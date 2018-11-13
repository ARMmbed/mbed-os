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

#ifndef PSA_NEG_DUAL_PART2_PARTITION_H
#define PSA_NEG_DUAL_PART2_PARTITION_H

#define NEG_DUAL_PART2_ID 7

#define NEG_DUAL_PART2_ROT_SRV_COUNT (2UL)
#define NEG_DUAL_PART2_EXT_ROT_SRV_COUNT (1UL)

/* NEG_DUAL_PART2 event flags */
#define NEG_DUAL_PART2_RESERVED1_POS (1UL)
#define NEG_DUAL_PART2_RESERVED1_MSK (1UL << NEG_DUAL_PART2_RESERVED1_POS)

#define NEG_DUAL_PART2_RESERVED2_POS (2UL)
#define NEG_DUAL_PART2_RESERVED2_MSK (1UL << NEG_DUAL_PART2_RESERVED2_POS)



#define DUMMY_MSK_POS (4UL)
#define DUMMY_MSK (1UL << DUMMY_MSK_POS)
#define PART2_CALL_INSIDE_PARTITION_MSK_POS (5UL)
#define PART2_CALL_INSIDE_PARTITION_MSK (1UL << PART2_CALL_INSIDE_PARTITION_MSK_POS)

#define NEG_DUAL_PART2_WAIT_ANY_SID_MSK (\
    DUMMY_MSK | \
    PART2_CALL_INSIDE_PARTITION_MSK)

/*
#define NEG_DUAL_PART2_WAIT_ANY_MSK (\
    NEG_DUAL_PART2_WAIT_ANY_SID_MSK) | \
    PSA_DOORBELL)
*/


#endif // PSA_NEG_DUAL_PART2_PARTITION_H
