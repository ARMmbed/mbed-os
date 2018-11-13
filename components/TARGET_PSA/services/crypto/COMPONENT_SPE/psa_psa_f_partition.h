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

#ifndef PSA_PSA_F_PARTITION_H
#define PSA_PSA_F_PARTITION_H

#define PSA_F_ID 35

#define PSA_F_ROT_SRV_COUNT (10UL)
#define PSA_F_EXT_ROT_SRV_COUNT (0UL)

/* PSA_F event flags */
#define PSA_F_RESERVED1_POS (1UL)
#define PSA_F_RESERVED1_MSK (1UL << PSA_F_RESERVED1_POS)

#define PSA_F_RESERVED2_POS (2UL)
#define PSA_F_RESERVED2_MSK (1UL << PSA_F_RESERVED2_POS)



#define PSA_CRYPTO_INIT_POS (4UL)
#define PSA_CRYPTO_INIT (1UL << PSA_CRYPTO_INIT_POS)
#define PSA_MAC_POS (5UL)
#define PSA_MAC (1UL << PSA_MAC_POS)
#define PSA_HASH_POS (6UL)
#define PSA_HASH (1UL << PSA_HASH_POS)
#define PSA_ASYMMETRIC_POS (7UL)
#define PSA_ASYMMETRIC (1UL << PSA_ASYMMETRIC_POS)
#define PSA_SYMMETRIC_POS (8UL)
#define PSA_SYMMETRIC (1UL << PSA_SYMMETRIC_POS)
#define PSA_AEAD_POS (9UL)
#define PSA_AEAD (1UL << PSA_AEAD_POS)
#define PSA_KEY_MNG_POS (10UL)
#define PSA_KEY_MNG (1UL << PSA_KEY_MNG_POS)
#define PSA_RNG_POS (11UL)
#define PSA_RNG (1UL << PSA_RNG_POS)
#define PSA_CRYPTO_FREE_POS (12UL)
#define PSA_CRYPTO_FREE (1UL << PSA_CRYPTO_FREE_POS)
#define PSA_GENERATOR_POS (13UL)
#define PSA_GENERATOR (1UL << PSA_GENERATOR_POS)

#define PSA_F_WAIT_ANY_SID_MSK (\
    PSA_CRYPTO_INIT | \
    PSA_MAC | \
    PSA_HASH | \
    PSA_ASYMMETRIC | \
    PSA_SYMMETRIC | \
    PSA_AEAD | \
    PSA_KEY_MNG | \
    PSA_RNG | \
    PSA_CRYPTO_FREE | \
    PSA_GENERATOR)

/*
#define PSA_F_WAIT_ANY_MSK (\
    PSA_F_WAIT_ANY_SID_MSK) | \
    PSA_DOORBELL)
*/


#endif // PSA_PSA_F_PARTITION_H
