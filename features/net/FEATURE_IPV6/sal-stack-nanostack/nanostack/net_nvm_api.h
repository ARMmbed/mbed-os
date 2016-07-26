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
/**
 * \file net_nvm_api.h
 * \brief Library PANA NVM API for Client and Server.
 *
 * \section server-api Server NVM API
 *
 * PANA server security material is crypted always and if you change the border router RF module
 * decrypt does not work properly.
 *
 *  - pana_server_nvm_callback_set(),  Initialize PANA server NVM functionality.
 *  - pana_server_restore_from_nvm(), Load crypted PANA server base and security material from NVM.
 *  - pana_server_nvm_client_session_load(), Load crypted client session from NVM.
 *
 * \section client-api Client NVM API
 *
 * nw_nvm.c use already this API and the application can just use net_nvm_api.h.
 *
 * - pana_client_nvm_callback_set(), Initialize PANA session NVM.
 * - net_read_persistent_data(), Read NWK ID & MAC 16-bit address.
 * - net_nvm_data_load(), Load PANA session, NWK-ID and short address to stack for re-use.
 * - net_pana_client_session_nvm_data_load(), Load saved PANA session with specific address or not.
 *
 */
#ifndef PANA_NVM_API_H_
#define PANA_NVM_API_H_

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PANA_CLIENT_NVM_SESSION_BUF_SIZE 70
#define PANA_CLIENT_NVM_SESSION_BUF_SIZE_WITH_ADDRESS 86
#define NET_NVM_SESSION_BUF_SIZE_WITH_NET_PARAMS_AND_PANA 88

#define PANA_SERVER_CLIENT_NVM_SESSION_UNCRYPTED_DATA_SIZE 20
#define PANA_SERVER_CLIENT_NVM_SESSION_FULL_ENCRYPTED_DATA_SIZE 96
#define PANA_SERVER_CLIENT_NVM_SESSION_SEQUENCY_STATE_ENCRYPTED_DATA_SIZE 33
#define PANA_SERVER_CLIENT_NVM_SESSION_BUF_SIZE (PANA_SERVER_CLIENT_NVM_SESSION_UNCRYPTED_DATA_SIZE + PANA_SERVER_CLIENT_NVM_SESSION_FULL_ENCRYPTED_DATA_SIZE)


#define PANA_SERVER_MATERIAL_BUF_SIZE 90
/*!
 * \enum pana_nvm_update_process_t
 * \brief PANA server NVM update states.
 */
typedef enum pana_nvm_update_process_t {
    PANA_SERVER_MATERIAL_UPDATE,                /**< PANA server security material update. */
    PANA_SERVER_CLIENT_SESSION_UPDATE,          /**< PANA client session update. */
    PANA_SERVER_CLIENT_SESSION_SEQ_UPDATE,      /**< PANA client session sequence number update. */
    PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE,   /**< PANA client session remove. */
} pana_nvm_update_process_t;

/*!
 * \enum pana_client_nvm_update_process_t
 * \brief PANA client NVM update states.
 */
typedef enum pana_client_nvm_update_process_t {
    PANA_CLIENT_SESSION_UPDATE,     /**< PANA session information fully update.  */
    PANA_CLIENT_SESSION_SEQ_UPDATE, /**< PANA key pull or push operation update REQ and RES sequence number. */
} pana_client_nvm_update_process_t;

/* NVM API PART */
/**
 * \brief PANA server NVM functionality initialization.
 *
 * \param passed_fptr A function pointer to NVM update process.
 * \param nvm_static_buffer A pointer to application allocated static memory, minimum size 115 bytes.
 *
 *
 * Reference callback function structure using EEPROM:
 * - nvm_static_buffer Application is allocated a static buffer.
 *
 * \return 0, Init OK.
 * \return -1, Null parameter detect.
 *
 */
extern int8_t pana_server_nvm_callback_set(uint16_t (*passed_fptr)(pana_nvm_update_process_t), uint8_t *nvm_static_buffer);
/**
 * \brief PANA server base restore from NVM.
 *
 * \param nvm_data A pointer to encrypted PANA server base data.
 *
 * \return 0, Restore OK.
 * \return -1, Memory allocation fail.
 *
 */
extern int8_t pana_server_restore_from_nvm(uint8_t *nvm_data, int8_t interface_id);
/**
 * \brief PANA client session load from NVM API.
 *
 * \param nvm_data A pointer to encrypted PANA client session.
 *
 * \return 0, Restore OK.
 * \return -1, Memory allocation fail.
 *
 */
extern int8_t pana_server_nvm_client_session_load(uint8_t *nvm_pointer);

/**
 * \brief PANA client NVM functionality init.
 *
 * \param passed_fptr A function pointer to NVM update process.
 * \param nvm_static_buffer A pointer to application allocated static memory, minimum size 88 bytes.
 *
 * Reference callback function structure using EEPROM:
 * - nvm_static_buffer Application allocated static buffer.
 *
 * \return 0, Init OK.
 * \return -1, Null parameter detect.
 *
 */
extern int8_t pana_client_nvm_callback_set(void (*passed_fptr)(pana_client_nvm_update_process_t), uint8_t *nvm_static_buffer);
/**
 * \brief Read network persistent data.
 *
 * \param data_buffer A pointer to location where the stack saves 18 bytes [NWK-ID 16bytes, Short Address 2bytes].
 *
 *
 * \return 0, Read OK.
 * \return -1, Null parameter detected.
 * \return -2 Bootstrap not ready yet.
 *
 * This function should call when network bootstrap is ready.
 *
 */
extern int8_t net_read_persistent_data(uint8_t *data_buffer, int8_t interface_id);
/**
 * \brief Load ZigBeeIP node persistent data to stack for re-use.
 *
 * \param data_buffer A pointer to data with 88 bytes [NWK-ID 16bytes, Short Address 2bytes, Pana 70 bytes].
 *
 *
 * \return 0, Read OK.
 * \return -1, Null parameter detected.
 * \return -2, Stack is active.
 * \return <-2 Memory allocation fail.
 *
 */
extern int8_t net_nvm_data_load(uint8_t *data_buffer, int8_t interface_id);

/**
 * \brief Load PANA client session data to the stack for re-use.
 *
 * \param data_buffer A pointer to data with encrypted PANA session 70 bytes.
 * \param session_address A pointer to a session-specific address. Give the address if you want to re-use the session with specific parent, otherwise give NULL.
 *
 *
 * \return 0, Read OK.
 * \return -1, Null parameter detected.
 * \return -2, Stack is active.
 * \return <-2 Memory allocation fail.
 *
 */
extern int8_t net_pana_client_session_nvm_data_load(uint8_t *data_buffer, uint8_t *session_address, int8_t interface_id);
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
#ifdef __cplusplus
}
#endif
#endif /* PANA_NVM_API_H_ */
