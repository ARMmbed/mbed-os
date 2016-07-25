/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
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
#ifndef MULTICAST_API_H_
#define MULTICAST_API_H_
/**
 * \file multicast_api.h
 * \brief Multicast Trickle Forwarding API.
 * \section multi-init Init API:
 * - multicast_set_parameters(), Set trickle parameters.
 * \section multi-cnf Configure API:
 * - multicast_add_address(), Add new address to a multicast group and control trickle forwarding.
 * - multicast_free_address(), Remove supported multicast address from list.
 *
 * \section ZigBeeIP Trickle Setups for Multicast Init
 *
 * | Parameter         | VALUE |
 * | :---------------: | :---: |
 * | imin              | 10    |
 * | imax              | 0     |
 * | k                 | 20    |
 * | timer_expirations | 3     |
 * | window_expiration | 75    |
 *
 */

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Set new parameters for trickle multicast.
 *
 * \param i_min Minimum trickle timer interval in 50ms resolution: Imin = i_min * 50ms.
 * \param i_max Maximum trickle timer interval as a number if doubling the minimum interval.
 * \param k Redundancy constant.
 * \param timer_expirations Number of trickle timer expirations before terminating the trickle process.
 * \param window_expiration The time window for keeping the state after the end of trickle process in 50ms resolution.
 *        NOTE: If window_expiration value is set too small an infinite retransmission loop may occur when using the trickle multicast.
 */
extern void multicast_set_parameters(uint8_t i_min, uint8_t i_max, uint8_t k, uint8_t timer_expirations, uint16_t window_expiration);


/**
 * \brief Add new address to multicast group.
 *
 * \param address_ptr Pointer to a 16-byte array that includes the address to be added.
 * \param use_trickle 0 = no trickle multicast forwarding, all other values = trickle multicast forwarding will be used with this address.
 *
 * \return 0 General error.
 * \return 1 Address updated.
 * \return 2 Address added.
 * \return 255 Link local not allowed when using multicast.
*
 */
extern uint8_t multicast_add_address(const uint8_t *address_ptr, uint8_t use_trickle);



/**
 * \brief Free address from multicast group.
 *
 * \param address_ptr Pointer to a 16-byte array that includes the address to be removed.
 *
 * \return 0 will be returned on successful execution, other values indicate an error on removing the address.
 */
extern uint8_t multicast_free_address(uint8_t *address_ptr);
#ifdef __cplusplus
}
#endif
#endif /* MULTICAST_API_H_ */
