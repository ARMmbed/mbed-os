/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
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

#ifndef __PSA_INITIAL_ATTESTATION_IMPL_H__
#define __PSA_INITIAL_ATTESTATION_IMPL_H__

#include "crypto.h"
#include <stdint.h>
#include <string.h>

#ifdef   __cplusplus
extern "C"
{
#endif

psa_status_t
psa_attestation_inject_key_impl(const uint8_t *key_data,
                                size_t key_data_length,
                                psa_key_type_t type,
                                uint8_t *public_key_data,
                                size_t public_key_data_size,
                                size_t *public_key_data_length);

#ifdef   __cplusplus
}
#endif

#endif /* __PSA_INITIAL_ATTESTATION_IMPL_H__ */
