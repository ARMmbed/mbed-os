//
//  t_cose_util.h
/*
 *  t_cose_common.h
 *
 * Copyright 2019, Laurence Lundblade
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.mdE.
 */


#ifndef t_cose_util_h
#define t_cose_util_h

#include <stdint.h>
#include "useful_buf.h"
#include "t_cose_common.h"

int32_t hash_alg_id_from_sig_alg_id(int32_t cose_sig_alg_id);

enum t_cose_err_t create_tbs_hash(int32_t cose_alg_id,
                                  struct useful_buf buffer_for_hash,
                                  struct useful_buf_c *hash,
                                  struct useful_buf_c protected_headers,
                                  struct useful_buf_c payload);

#endif /* t_cose_util_h */
