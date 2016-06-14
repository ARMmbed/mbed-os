/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef PEER_DATA_H__
#define PEER_DATA_H__

#include <stdint.h>
#include "peer_manager_types.h"
#include "fds.h"


/**
 * @defgroup peer_data Peer Data
 * @ingroup peer_manager
 * @{
 * @brief An internal module of @ref peer_manager. This module defines the structure of the data
 *        that is managed by the @ref peer_manager. It also provides functions for parsing the data.
 */


/**@brief Function for enumerating the separate (non-contiguous) parts of the peer data.
 *
 * @param[in]  p_peer_data  The peer data to enumerate.
 * @param[out] p_chunks      The resulting chunks. This must be an array of at least 2 elements.
 * @param[out] p_n_chunks    The number of chunks. If this is 0, something went wrong.
 */
void peer_data_parts_get(pm_peer_data_const_t const * p_peer_data, fds_record_chunk_t * p_chunks, uint16_t * p_n_chunks);


/**@brief Function for converting @ref pm_peer_data_flash_t into @ref pm_peer_data_t.
 *
 * @param[in]  p_in_data   The source data.
 * @param[out] p_out_data  The target data structure.
 *
 * @retval NRF_SUCCESS       Successful conversion.
 * @retval NRF_ERROR_NULL    A parameter was NULL.
 * @retval NRF_ERROR_NO_MEM  A buffer was not large enough.
 */
ret_code_t peer_data_deserialize(pm_peer_data_flash_t const * p_in_data, pm_peer_data_t * p_out_data);

/** @} */

#endif /* PEER_DATA_H__ */
