/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
 */

/**
 * \file net_mle_api.h
 * \brief 6LoWPAN MLE options control API
 *
 */
#ifndef _NET_MLE_API_H
#define _NET_MLE_API_H

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  * \brief Set default MLE neighbor lifetime for a router.
  *
  * \param interface_id Interface ID.
  * \param lifetime Lifetime in seconds. 64 to 2560.
  *
  * \return 0, Lifetime update OK.
  * \return < 0, Lifetime update fail.
  *
  */
int8_t arm_nwk_6lowpan_mle_router_lifetime_set(int8_t interface_id, uint16_t lifetime);

/**
  * \brief Set default MLE neighbor lifetime for a host.
  *
  * \param interface_id Interface ID.
  * \param lifetime Lifetime in seconds. 64 to 2560.
  *
  * \return 0, Lifetime update OK.
  * \return < 0, Lifetime update fail.
  *
  */
int8_t arm_nwk_6lowpan_mle_host_lifetime_set(int8_t interface_id, uint16_t lifetime);

/**
  * \brief Set MLE neighbor list limits.
  *
  * The MLE neighbor limits configuration settings limit the number of neighbors
  * added to the MLE neighbor list.
  *
  * If the number of neighbors reaches the lower threshold, MLE starts to limit addition
  * of new neighbors. Multicast MLE messages from unknown neighbors are ignored (ignore probability is randomized).
  * The value must be smaller than the upper threshold and maximum value.
  *
  * If the number of neighbors reaches the upper threshold, MLE stops adding new neighbors
  * based on the multicast MLE messages. Only nodes that select this node for a
  * parent during the bootstrap will be accepted. The value must be smaller than or the same as
  * the maximum value.
  *
  * If the number of neighbors reaches the maximum value, no new neighbors are added.
  *
  * If the MLE neighbor list limits are not used, all values must be set to 0.
  *
  * \param interface_id Interface ID.
  * \param lower_threshold Lower threshold. 5 to 499. 0 limits not used.
  * \param upper_threshold Upper threshold. 5 to 500. 0 limits not used.
  * \param max Maximum number of neighbors. 5 to 500. 0 limits not used.
  *
  * \return 0, Limits update OK.
  * \return < 0, Limits update fail.
  *
  */
int8_t arm_nwk_6lowpan_mle_neighbor_limits_set(int8_t interface_id, uint16_t lower_threshold, uint16_t upper_threshold, uint16_t max);

/**
  * \brief Set MLE message token bucket settings
  *
  * The MLE message token bucket limits the MLE message sending rate. The token bucket size
  * controls the bucket size. The token bucket rate controls the rate in which
  * new tokens are added. The count defines how many tokens at a time are added to the bucket.
  *
  * The rate is entered in multiplies of 0.1 second minimum interval (for example, if rate is 3
  * and count is 4 then 4 new tokens are added to bucket every 0.3 seconds).
  *
  * If the token bucket is not used, all values must be set to 0.
  *
  * \param interface_id Interface ID.
  * \param size Bucket size. 1 to 255. 0 token bucket not used.
  * \param rate Token rate. 1 to 255. 0 token bucket not used.
  * \param count Token count. 1 to 255. 0 token bucket not used.
  *
  * \return 0, Token bucket settings update OK.
  * \return < 0, Token bucket settings update fail.
  *
  */
int8_t arm_nwk_6lowpan_mle_token_bucket_settings_set(int8_t interface_id, uint8_t size, uint8_t rate, uint8_t count);

#ifdef __cplusplus
}
#endif
#endif /* _NET_MLE_API_H */
