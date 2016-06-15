/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */


#ifndef PEER_DATA_H__
#define PEER_DATA_H__

#include <stdint.h>
#include "peer_manager_types.h"
#include "peer_manager_internal.h"
#include "fds.h"


/**
 * @cond NO_DOXYGEN
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

/** @} 
 * @endcond
 */

#endif /* PEER_DATA_H__ */
