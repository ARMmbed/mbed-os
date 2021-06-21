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

#include "qcbor.h"
#include "pal_common.h"

#define PAL_ATTEST_MIN_ERROR              30

#define COSE_HEADER_PARAM_ALG             1
#define COSE_HEADER_PARAM_KID             4

#define MANDATORY_CLAIM_WITH_SW_COMP      862
#define MANDATORY_CLAIM_NO_SW_COMP        926
#define MANDATORY_SW_COMP                 36
#define CBOR_ARM_TOTAL_CLAIM_INSTANCE     10

/*
 CBOR Label for proprietary header indicating short-circuit
 signing was used. Just a random number in the proprietary
 label space */
#define T_COSE_SHORT_CIRCUIT_LABEL              (-8675309)

#define EAT_CBOR_ARM_RANGE_BASE                 (-75000)
#define EAT_CBOR_ARM_LABEL_PROFILE_DEFINITION   (EAT_CBOR_ARM_RANGE_BASE - 0)
#define EAT_CBOR_ARM_LABEL_CLIENT_ID            (EAT_CBOR_ARM_RANGE_BASE - 1)
#define EAT_CBOR_ARM_LABEL_SECURITY_LIFECYCLE   (EAT_CBOR_ARM_RANGE_BASE - 2)
#define EAT_CBOR_ARM_LABEL_IMPLEMENTATION_ID    (EAT_CBOR_ARM_RANGE_BASE - 3)
#define EAT_CBOR_ARM_LABEL_BOOT_SEED            (EAT_CBOR_ARM_RANGE_BASE - 4)
#define EAT_CBOR_ARM_LABEL_HW_VERSION           (EAT_CBOR_ARM_RANGE_BASE - 5)
#define EAT_CBOR_ARM_LABEL_SW_COMPONENTS        (EAT_CBOR_ARM_RANGE_BASE - 6)
#define EAT_CBOR_ARM_LABEL_NO_SW_COMPONENTS     (EAT_CBOR_ARM_RANGE_BASE - 7)
#define EAT_CBOR_ARM_LABEL_NONCE                (EAT_CBOR_ARM_RANGE_BASE - 8)
#define EAT_CBOR_ARM_LABEL_UEID                 (EAT_CBOR_ARM_RANGE_BASE - 9)
#define EAT_CBOR_ARM_LABEL_ORIGINATION          (EAT_CBOR_ARM_RANGE_BASE - 10)

#define EAT_CBOR_SW_COMPONENT_TYPE              (1u)
#define EAT_CBOR_SW_COMPONENT_MEASUREMENT       (2u)
#define EAT_CBOR_SW_COMPONENT_EPOCH             (3u)
#define EAT_CBOR_SW_COMPONENT_VERSION           (4u)
#define EAT_CBOR_SW_COMPONENT_SIGNER_ID         (5u)
#define EAT_CBOR_SW_COMPONENT_MEASUREMENT_DESC  (6u)


enum attestation_error_code {
    PAL_ATTEST_SUCCESS = 0,
    PAL_ATTEST_TOKEN_ERR_CBOR_FORMATTING = PAL_ATTEST_MIN_ERROR,
    PAL_ATTEST_TOKEN_CHALLENGE_MISMATCH,
    PAL_ATTEST_TOKEN_NOT_SUPPORTED,
    PAL_ATTEST_TOKEN_NOT_ALL_MANDATORY_CLAIMS,
    PAL_ATTEST_ERROR,
};

struct items_to_get_t {
    int64_t label;
    QCBORItem item;
};

int32_t pal_initial_attest_verify_token(uint8_t *challenge, uint32_t challenge_size,
                                        uint8_t *token, uint32_t token_size);
