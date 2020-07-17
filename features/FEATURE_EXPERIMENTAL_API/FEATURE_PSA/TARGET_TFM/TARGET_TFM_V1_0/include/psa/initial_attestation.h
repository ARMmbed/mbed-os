/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/***************************************************************************/
/*                             DRAFT UNDER REVIEW                          */
/*   These APIs are still evolving and are meant as a prototype for review.*/
/*   The APIs will change depending on feedback and will be firmed up      */
/*   to a stable set of APIs once all the feedback has been considered.    */
/***************************************************************************/

#ifndef __PSA_INITIAL_ATTESTATION_H__
#define __PSA_INITIAL_ATTESTATION_H__

#include <limits.h>
#include <stdint.h>
#include <stddef.h>
#include "psa/crypto.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief PSA INITIAL ATTESTATION API version
 *
 * Initial attestation API version is: 1.0.0
 */
#define PSA_INITIAL_ATTEST_API_VERSION_MAJOR (1)
#define PSA_INITIAL_ATTEST_API_VERSION_MINOR (0)

/**
 * The allowed size of input challenge in bytes: 32, 48, 64
 * Challenge can be a nonce from server
 * or the hash of some combined data : nonce + attested data by caller.
 */
#define PSA_INITIAL_ATTEST_CHALLENGE_SIZE_32  (32u)
#define PSA_INITIAL_ATTEST_CHALLENGE_SIZE_48  (48u)
#define PSA_INITIAL_ATTEST_CHALLENGE_SIZE_64  (64u)

/**
 * The maximum size of an attestation token that can be generated by the
 * attestation service. Used to configure buffers for services that verify the
 * produced tokens.
 */
#define PSA_INITIAL_ATTEST_MAX_TOKEN_SIZE (0x400)

/**
 * The list of fixed claims in the initial attestation token is still evolving,
 * you can expect slight changes in the future.
 *
 * The initial attestation token is planned to be aligned with future version of
 * Entity Attestation Token format:
 * https://tools.ietf.org/html/draft-mandyam-eat-01
 *
 * Current list of claims:
 *  - Challenge:   Input object from caller. Can be a single nonce from server
 *                 or hash of nonce and attested data. It is intended to provide
 *                 freshness to reports and the caller has responsibility to
 *                 arrange this. Allowed length: 32, 48, 64 bytes. The claim is
 *                 modeled to be eventually represented by the EAT standard
 *                 claim nonce. Until such a time as that standard exists,
 *                 the claim will be represented by a custom claim. Value
 *                 is encoded as byte string.
 *
 *  - Instance ID: It represents the unique identifier of the instance. In the
 *                 PSA definition it is a hash of the public attestation key
 *                 of the instance. The claim is modeled to be eventually
 *                 represented by the EAT standard claim UEID of type GUID.
 *                 Until such a time as that standard exists, the claim will be
 *                 represented by a custom claim  Value is encoded as byte
 *                 string.
 *
 *  - Verification service indicator: Optional, recommended claim. It is used by
 *                 a Relying Party to locate a validation service for the token.
 *                 The value is a text string that can be used to locate the
 *                 service or a URL specifying the address of the service. The
 *                 claim is modeled to be eventually represented by the EAT
 *                 standard claim origination. Until such a time as that
 *                 standard exists, the claim will be represented by a custom
 *                 claim. Value is encoded as text string.
 *
 *  - Profile definition: Optional, recommended claim. It contains the name of
 *                 a document that describes the 'profile' of the token, being
 *                 a full description of the claims, their usage, verification
 *                 and token signing. The document name may include versioning.
 *                 Custom claim with a value encoded as text string.
 *
 *  - Implementation ID: It represents the original implementation signer of the
 *                 attestation key and identifies the contract between the
 *                 report and verification. A verification service will use this
 *                 claim to locate the details of the verification process.
 *                 Custom claim with a value encoded as byte string.
 *
 *  - Security lifecycle: It represents the current lifecycle state of the
 *                 instance. Custom claim with a value encoded as integer that
 *                 is divided to convey a major state and a minor state. The
 *                 PSA state and implementation state are encoded as follows:
 *                   - version[15:8] - PSA lifecycle state - major
 *                   - version[7:0]  - IMPLEMENTATION DEFINED state - minor
 *                 Possible PSA lifecycle states:
 *                  - Unknown (0x1000u),
 *                  - PSA_RoT_Provisioning (0x2000u),
 *                  - Secured (0x3000u),
 *                  - Non_PSA_RoT_Debug(0x4000u),
 *                  - Recoverable_PSA_RoT_Debug (0x5000u),
 *                  - Decommissioned (0x6000u)
 *
 *  - Client ID:   The partition ID of that secure partition or non-secure
 *                 thread who called the initial attestation API. Custom claim
 *                 with a value encoded as a *signed* integer. Negative number
 *                 represents non-secure caller, positive numbers represents
 *                 secure callers, zero is invalid.
 *
 *  - HW version:  Optional claim. Globally unique number in EAN-13 format
 *                 identifying the GDSII that went to fabrication, HW and ROM.
 *                 It can be used to reference the security level of the PSA-ROT
 *                 via a certification website. Custom claim with a value is
 *                 encoded as text string.

 *  - Boot seed:   It represents a random value created at system boot time that
 *                 will allow differentiation of reports from different system
 *                 sessions. The size is 32 bytes. Custom claim with a value is
 *                 encoded as byte string.
 *
 *  - Software components: Recommended claim. It represents the software state
 *                 of the system. The value of the claim is an array of CBOR map
 *                 entries, with one entry per software component within the
 *                 device. Each map contains multiple claims that describe
 *                 evidence about the details of the software component.
 *
 *     - Measurement type: Optional claim. It represents the role of the
 *                    software component. Value is encoded as short(!) text
 *                    string.
 *
 *     - Measurement value: It represents a hash of the invariant software
 *                    component in memory at start-up time. The value must be a
 *                    cryptographic hash of 256 bits or stronger.Value is
 *                    encoded as byte string.
 *
 *     - Version:     Optional claim. It represents the issued software version.
 *                    Value is encoded as text string.
 *
 *     - Signer ID:   It represents the hash of a signing authority public key.
 *                    Value is encoded as byte string.
 *
 *     - Measurement description: Optional claim. It represents the way in which
 *                    the measurement value of the software component is
 *                    computed. Value is encoded as text string containing an
 *                    abbreviated description (name) of the measurement method.
 *
 *  - No software measurements: In the event that the implementation does not
 *                 contain any software measurements then the software
 *                 components claim above can be omitted but instead
 *                 it is mandatory to include this claim to indicate this is a
 *                 deliberate state. Custom claim a value is encoded as unsigned
 *                 integer set to 1.
 */

/**
 * \brief Get initial attestation token
 *
 * \param[in]     auth_challenge  Pointer to buffer where challenge input is
 *                                stored. Nonce and / or hash of attested data.
 *                                Must be always
 *                                \ref PSA_INITIAL_ATTEST_TOKEN_SIZE bytes
 *                                long.
 * \param[in]     challenge_size  Size of challenge object in bytes.
 * \param[out]    token_buf       Pointer to the buffer where attestation token
 *                                will be stored.
 * \param[in]     token_buf_size  Size of allocated buffer for token, in bytes.
 * \param[out]    token_size      Size of the token that has been returned, in
 *                                bytes.
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t
psa_initial_attest_get_token(const uint8_t *auth_challenge,
                             size_t         challenge_size,
                             uint8_t       *token_buf,
                             size_t         token_buf_size,
                             size_t        *token_size);

/**
 * \brief Get the exact size of initial attestation token in bytes.
 *
 * It just returns with the size of the IAT token. It can be used if the caller
 * dynamically allocates memory for the token buffer.
 *
 * \param[in]   challenge_size  Size of challenge object in bytes. This must be
 *                              a supported challenge size (as above).
 * \param[out]  token_size      Size of the token in bytes, which is created by
 *                              initial attestation service.
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t
psa_initial_attest_get_token_size(size_t  challenge_size,
                                  size_t *token_size);

/**
 * \brief Get the initial attestation public key.
 *
 * \param[out]  public_key        Pointer to the buffer where the public key
 *                                will be stored.
 * \param[in]   key_buf_size      Size of allocated buffer for key, in bytes.
 * \param[out]  public_key_len    Size of public key in bytes.
 * \param[out]  public_key_curve  Type of the elliptic curve which the key
 *                                belongs to.
 *
 * \note Currently only the ECDSA P-256 over SHA-256 algorithm is supported.
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t
tfm_initial_attest_get_public_key(uint8_t         *public_key,
                                  size_t           public_key_buf_size,
                                  size_t          *public_key_len,
                                  psa_ecc_curve_t *elliptic_curve_type);

#ifdef __cplusplus
}
#endif

#endif /* __PSA_INITIAL_ATTESTATION_H__ */
