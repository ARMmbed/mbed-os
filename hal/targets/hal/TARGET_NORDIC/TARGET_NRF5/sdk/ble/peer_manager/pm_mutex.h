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


#ifndef MUTEX_H__
#define MUTEX_H__


#include <stdint.h>
#include <stdbool.h>

/**
 * @cond NO_DOXYGEN
 * @defgroup pm_mutex Mutex
 * @ingroup peer_manager
 * @{
 * @brief An internal module of @ref peer_manager. This module provides thread-safe mutexes.
 */


/**@brief Defines the storage size of a specified mutex group.
 *
 * @param number_of_mutexes the number of mutexes in the group.
 */
#define MUTEX_STORAGE_SIZE(number_of_mutexes) ((7 + (number_of_mutexes)) >> 3)


/**@brief Initializes a mutex group.
 *
 * @param[in] p_mutex     Pointer to the mutex group. See @ref MUTEX_STORAGE_SIZE().
 * @param[in] mutex_size  The size of the mutex group in number of mutexes.
 */
void pm_mutex_init(uint8_t * p_mutex, uint16_t mutex_size);


/**@brief Locks the mutex specified by the bit id.
 *
 * @param[inout] p_mutex       Pointer to the mutex group.
 * @param[in]    mutex_bit_id  The bit id of the mutex.
 *
 * @retval true   if it was possible to lock the mutex.
 * @retval false  otherwise.
 */
bool pm_mutex_lock(uint8_t * p_mutex, uint16_t mutex_bit_id);


/**@brief Locks the first unlocked mutex within the mutex group.
 *
 * @param[in, out] p_mutex     Pointer to the mutex group.
 * @param[in]      mutex_size  The size of the mutex group.
 *
 * @return The first unlocked mutex id in the group.
 * @retval group-size  if there was no unlocked mutex available.
 */
uint16_t pm_mutex_lock_first_available(uint8_t * p_mutex, uint16_t mutex_size);


/**@brief Unlocks the mutex specified by the bit id.
 *
 * @param[in, out] p_mutex       Pointer to the mutex group.
 * @param[in]      mutex_bit_id  The bit id of the mutex.
 */
void pm_mutex_unlock(uint8_t * p_mutex, uint16_t mutex_bit_id);


/**@brief Gets the locking status of the specified mutex.
 *
 * @param[in, out] p_mutex      Pointer to the mutex group.
 * @param[in]      mutex_bit_id The bit id of the mutex.
 *
 * @retval true   if the mutex was locked.
 * @retval false  otherwise.
 */
bool pm_mutex_lock_status_get(uint8_t * p_mutex, uint16_t mutex_bit_id);


#endif // MUTEX_H__

/** @} 
 * @endcond  
 */
