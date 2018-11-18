/* Copyright (c) 2017-2018 ARM Limited
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

#ifndef PSA_NEG_SERVER_PART1_PARTITION_H
#define PSA_NEG_SERVER_PART1_PARTITION_H

#define NEG_SERVER_PART1_ID 7

#define NEG_SERVER_PART1_ROT_SRV_COUNT (4UL)
#define NEG_SERVER_PART1_EXT_ROT_SRV_COUNT (0UL)

/* NEG_SERVER_PART1 event flags */
#define NEG_SERVER_PART1_RESERVED1_POS (1UL)
#define NEG_SERVER_PART1_RESERVED1_MSK (1UL << NEG_SERVER_PART1_RESERVED1_POS)

#define NEG_SERVER_PART1_RESERVED2_POS (2UL)
#define NEG_SERVER_PART1_RESERVED2_MSK (1UL << NEG_SERVER_PART1_RESERVED2_POS)



#define PART1_REPLY_INVALID_RETVAL_CONNECT_MSK_POS (4UL)
#define PART1_REPLY_INVALID_RETVAL_CONNECT_MSK (1UL << PART1_REPLY_INVALID_RETVAL_CONNECT_MSK_POS)
#define PART1_REPLY_INVALID_RETVAL_CALL_MSK_POS (5UL)
#define PART1_REPLY_INVALID_RETVAL_CALL_MSK (1UL << PART1_REPLY_INVALID_RETVAL_CALL_MSK_POS)
#define PART1_CLEAR_NO_DOORBELL_MSK_POS (6UL)
#define PART1_CLEAR_NO_DOORBELL_MSK (1UL << PART1_CLEAR_NO_DOORBELL_MSK_POS)
#define PART1_WRITE_OUTVEC_IX_SIZE_0_MSK_POS (7UL)
#define PART1_WRITE_OUTVEC_IX_SIZE_0_MSK (1UL << PART1_WRITE_OUTVEC_IX_SIZE_0_MSK_POS)

#define NEG_SERVER_PART1_WAIT_ANY_SID_MSK (\
    PART1_REPLY_INVALID_RETVAL_CONNECT_MSK | \
    PART1_REPLY_INVALID_RETVAL_CALL_MSK | \
    PART1_CLEAR_NO_DOORBELL_MSK | \
    PART1_WRITE_OUTVEC_IX_SIZE_0_MSK)

/*
#define NEG_SERVER_PART1_WAIT_ANY_MSK (\
    NEG_SERVER_PART1_WAIT_ANY_SID_MSK) | \
    PSA_DOORBELL)
*/


#endif // PSA_NEG_SERVER_PART1_PARTITION_H
