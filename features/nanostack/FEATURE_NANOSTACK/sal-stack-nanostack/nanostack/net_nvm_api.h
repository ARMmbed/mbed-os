/*
 * Copyright (c) 2013-2017 ARM Limited. All rights reserved.
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
 * \file net_nvm_api.h
 * \brief Library PANA NVM API for Client and Server.
 *
 * \section server-api Server NVM API
 *
 *  - pana_server_nvm_callback_set(),  Initialize PANA server NVM functionality.
 *  - pana_server_restore_from_nvm(), Load PANA server base and security material from NVM.
 *  - pana_server_nvm_client_session_load(), Load client session from NVM.
 *
 * \section client-api Client NVM API
 *
 * nw_nvm.c use already this API and the application can just use net_nvm_api.h.
 *
 * - pana_client_nvm_callback_set(), Initialize PANA session NVM.
 *
 */
#ifndef PANA_NVM_API_H_
#define PANA_NVM_API_H_

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief Pana client and common data structure for get / Update callback's
 *
 * Client: [session address 16-bytes] + [session data 70-bytes] , Total 86 bytes
 *
 * Server: [offset 2-bytes] + [session address 16-bytes] + [session port 2-bytes] + [session id 4-bytes] + [session data 92-bytes], Total 116 bytes
 */

/**
 * Size of Pana client session data without session address
 */
#define PANA_CLIENT_NVM_SESSION_BUF_SIZE 70
/**
 * Size of Pana client session data with session address
 */
#define PANA_CLIENT_NVM_SESSION_BUF_SIZE_WITH_ADDRESS 86

/**
 * Size of data for Server client Uodate and get callback
 *
 */
#define PANA_SERVER_CLIENT_NVM_SESSION_BUF_SIZE 116

/**
 * Size of data for Server material update and restore operation
 *
 */
#define PANA_SERVER_MATERIAL_BUF_SIZE 90
/*!
 * \enum pana_nvm_update_process_t
 * \brief PANA server NVM update states.
 */
typedef enum pana_nvm_update_process_t {
    PANA_SERVER_MATERIAL_UPDATE,                /**< PANA server security material update. */
    PANA_SERVER_CLIENT_SESSION_UPDATE,          /**< PANA client session update. */
    PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE,   /**< PANA client session remove. */
} pana_nvm_update_process_t;

/*!
 * \enum pana_client_nvm_update_process_t
 * \brief PANA client NVM update states.
 */
typedef enum pana_client_nvm_update_process_t {
    PANA_CLIENT_SESSION_UPDATE,          /**< PANA client session update. */
    PANA_CLIENT_SESSION_REMOVE,   /**< PANA client session remove. */
} pana_client_nvm_update_process_t;

/*!
 * \struct wpan_nvm_params_t
 * \brief Network nvm parameters.
 */
typedef struct wpan_nvm_params {
    uint16_t pan_id;     /**< WPAN Pan-id.  */
    uint32_t mac_security_frame_counter; /**< Mac security counter. */
    uint32_t mle_securit_counter; /**< MLE security counter. */
} wpan_nvm_params_t;

/**
 * \brief NVM memory user callback for updated parameters
 *
 * \param parameters Updated wpan parameters
 */
typedef void wpan_params_updated(wpan_nvm_params_t *parameters);

/**
 * \brief NVM memory user for get parameter->pan_id network setup
 *
 * \param parameters Pointer for request parameters parameter->pan_id is configured for proper setup
 *
 * \return true when configure is loaded to buffer
 * \return false when there is no proper data for current wpan
 */
typedef bool wpan_params_get(wpan_nvm_params_t *parameters);

/**
 * \brief Pana client session update callback
 *
 * \param pan_id define which pan session is
 * \param process PANA_CLIENT_SESSION_UPDATE or PANA_CLIENT_SESSION_REMOVE
 */
typedef void pana_client_session_update_cb(uint16_t pan_id, pana_client_nvm_update_process_t process);

/**
 * \brief Pana client discover session from NVM user based on pan id
 *
 * \param pan_id define which pan session is
 *
 * \return true When Session is stored to behind given session buffer
 * \return false NVM can't detect session for given pan id
 */
typedef bool pana_client_session_get_cb(uint16_t pan_id);

/**
 * \brief Pana server call this when it create /Update / delete client session or update Server material
 *
 * \param operation Pana server NVM update
 *
 * \return session offset . Requirement for operation PANA_SERVER_CLIENT_SESSION_UPDATE
 */
typedef uint16_t pana_server_update_cb(pana_nvm_update_process_t operation);

/**
 * \brief Pana server call this when discover client session from NVM for given address
 *
 * \param linklocal_address Link local address for sesion
 *
 * \return true When Session is stored to behind given session buffer
 * \return false NVM can't detect session for given address
 */
typedef bool pana_server_session_get_cb(uint8_t *linklocal_address);

/**
 * \brief Pana server call this when discover client session from NVM for pana session
 *
 * \param session_id Pana session Id
 *
 * \return true When Session is stored to behind given session buffer
 * \return false NVM can't detect session for given session id
 */
typedef bool pana_server_session_get_by_id_cb(uint32_t session_id);

/* NVM API PART */
/**
 * \brief PANA server NVM functionality initialization.
 *
 * \param update_cb A function pointer to NVM update process.
 * \param nvm_get A function pointer for discover session data from NVM by link local address
 * \param nvm_session_get A function pointer for discover session data from NVM by pana session id
 * \param nvm_static_buffer A pointer to application allocated static memory, minimum size PANA_SERVER_CLIENT_NVM_SESSION_BUF_SIZE (116 bytes).
 *
 *
 * \return 0, Init OK.
 * \return -1, Null parameter detect.
 *
 */
extern int8_t pana_server_nvm_callback_set(pana_server_update_cb *update_cb, pana_server_session_get_cb *nvm_get, pana_server_session_get_by_id_cb *nvm_session_get, uint8_t *nvm_static_buffer);
/**
 * \brief PANA server base restore from NVM.
 *
 * \param nvm_data A pointer to PANA server base data.
 * \param interface_id Interface ID.
 *
 * \return 0, Restore OK.
 * \return -1, Memory allocation fail.
 *
 */
extern int8_t pana_server_restore_from_nvm(uint8_t *nvm_data, int8_t interface_id);
/**
 * \brief PANA client session load from NVM API.
 *
 * \param nvm_pointer A pointer  PANA client session.
 *
 * \return 0, Restore OK.
 * \return -1, Memory allocation fail.
 *
 */
extern int8_t pana_server_nvm_client_session_load(uint8_t *nvm_pointer);

/**
 * \brief PANA client NVM functionality init.
 *
 * \param nvm_update A function pointer to NVM update process.
 * \param nvm_get A function pointer for discover session from NVM for given Pan-id
 * \param nvm_static_buffer A pointer to application allocated static memory, minimum size for session 86 bytes + 16-bit pan-id.
 *
 *
 * \return 0, Init OK.
 * \return -1, Null parameter detect.
 *
 */
extern int8_t pana_client_nvm_callback_set(pana_client_session_update_cb *nvm_update, pana_client_session_get_cb *nvm_get,uint8_t *nvm_static_buffer);

/**
 * \brief Clean node persistent data and all PANA client sessions from the stack.
 *
 * This function disables the network ID filter, sets EUID-16 to 0xffff and removes PANA client sessions. It is only for client purposes.
 *
 *
 * \return 0, Clean OK.
 * \return -1, Stack is active.
 *
 */
extern int8_t net_nvm_data_clean(int8_t interface_id);

/**
 * \brief Enable and init network NVM parameter automatic update.
 *
 * This function enables MAC and MLE protocol critical components update process to NVM memory user.
 *
 * \param interface_id Interface ID.
 * \param nvm_update_cb Function pointer for update NVM
 * \param nvm_get_cb Function for stack to request setup from NVM.
 *
 *
 * \return 0, Init OK.
 * \return -1, Unknown Interface.
 * \return -2, Memory allocation fail.
 *
 */
extern int8_t net_nvm_wpan_params_storage_enable(int8_t interface_id, wpan_params_updated *nvm_update_cb, wpan_params_get *nvm_get_cb);

/**
 * \brief Reset stored WPAN parameter's at interface .
 *
 * This function clean state at WPAN params at stack. Force Request from NVM
 *
 *\param interface_id Interface ID.
 *
 * \return 0, reset OK.
 * \return -1, Unknown Interface.
 *
 */
extern int8_t net_nvm_wpan_params_storage_reset(int8_t interface_id);


/**
 * \brief Disable stored WPAN parameter's to interface .
 *
 *
 *\param interface_id Interface ID.
 *
 * \return 0, Disable OK.
 * \return -1, Unknown Interface.
 *
 */
extern int8_t net_nvm_wpan_params_storage_disable(int8_t interface_id);


#ifdef __cplusplus
}
#endif
#endif /* PANA_NVM_API_H_ */
