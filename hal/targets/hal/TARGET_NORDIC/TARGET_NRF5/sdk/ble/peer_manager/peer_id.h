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


#ifndef PEER_ID_H__
#define PEER_ID_H__


#include <stdint.h>
#include "sdk_errors.h"
#include "ble_gap.h"
#include "peer_manager_types.h"


/**
 * @cond NO_DOXYGEN
 * @defgroup peer_id Peer IDs
 * @ingroup peer_manager
 * @{
 * @brief An internal module of @ref peer_manager. This module keeps track of which peer IDs are in
 *        use and which are free.
 */


/**@brief Function for initializing the module.
 */
void peer_id_init(void);


/**@brief Function for claiming an unused peer ID.
 *
 * @param peer_id  The peer ID to allocate. If this is @ref PM_PEER_ID_INVALID, the first available
 *                 will be allocated.
 *
 * @return  The allocated peer ID.
 * @retval  PM_PEER_ID_INVALID  If no peer ID could be allocated or module is not initialized.
 */
pm_peer_id_t peer_id_allocate(pm_peer_id_t peer_id);


/**@brief Function for marking a peer ID for deletion.
 *
 * @param peer_id  The peer ID to delete.
 *
 * @retval  true   Deletion was successful.
 * @retval  false  Peer ID already marked for deletion, peer_id was PM_PEER_ID_INVALID, or module is
 *                 not initialized.
 */
bool peer_id_delete(pm_peer_id_t peer_id);


/**@brief Function for freeing a peer ID and clearing all data associated with it in persistent
 *        storage.
 *
 * @param[in]  peer_id  Peer ID to free.
 */
void peer_id_free(pm_peer_id_t peer_id);


/**@brief Function for finding out whether a peer ID is marked for deletion.
 *
 * @param[in]  peer_id  The peer ID to inquire about.
 *
 * @retval  true   peer_id is in marked for deletion.
 * @retval  false  peer_id is not marked for deletion, or the module is not initialized.
 */
bool peer_id_is_deleted(pm_peer_id_t peer_id);


/**@brief Function for finding out whether a peer ID is in use.
 *
 * @param[in]  peer_id  The peer ID to inquire about.
 *
 * @retval  true   peer_id is in use.
 * @retval  false  peer_id is free, or the module is not initialized.
 */
bool peer_id_is_allocated(pm_peer_id_t peer_id);


/**@brief Function for getting the next peer ID in the sequence of all used peer IDs. Can be
 *        used to loop through all used peer IDs.
 *
 * @note @ref PM_PEER_ID_INVALID is considered to be before the first and after the last ordinary
 *       peer ID.
 *
 * @param[in]  prev_peer_id  The previous peer ID.
 *
 * @return  The next peer ID.
 * @return  The first used peer ID  if prev_peer_id was @ref PM_PEER_ID_INVALID.
 * @retval  PM_PEER_ID_INVALID      if prev_peer_id was the last ordinary peer ID or the module is
 *                                  not initialized.
 */
pm_peer_id_t peer_id_get_next_used(pm_peer_id_t prev_peer_id);


/**@brief Function for getting the next peer ID in the sequence of all peer IDs marked for deletion.
 *        Can be used to loop through all peer IDs marked for deletion.
 *
 * @note @ref PM_PEER_ID_INVALID is considered to be before the first and after the last ordinary
 *       peer ID.
 *
 * @param[in]  prev_peer_id  The previous peer ID.
 *
 * @return  The next peer ID.
 * @return  The first used peer ID  if prev_peer_id was @ref PM_PEER_ID_INVALID.
 * @retval  PM_PEER_ID_INVALID      if prev_peer_id was the last ordinary peer ID or the module is
 *                                  not initialized.
 */
pm_peer_id_t peer_id_get_next_deleted(pm_peer_id_t prev_peer_id);


/**@brief Function for querying the number of valid peer IDs available. I.E the number of peers
 *        in persistent storage.
 *
 * @return  The number of valid peer IDs, or 0 if module is not initialized.
 */
uint32_t peer_id_n_ids(void);

/** @} 
 * @endcond
 */

#endif /* PEER_ID_H__ */
