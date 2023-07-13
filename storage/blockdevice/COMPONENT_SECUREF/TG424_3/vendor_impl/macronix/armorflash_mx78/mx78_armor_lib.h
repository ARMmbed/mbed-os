/*
 * Copyright (c) 2022-2023 Macronix International Co. LTD. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _MX78_ARMOR_LIB_H_
#define _MX78_ARMOR_LIB_H_

#include <stdint.h>
#include <stdbool.h>
#include "../../../JEDEC_security_HAL/crypto_wrapper.h"
#include "../../../JEDEC_security_HAL/include/jedec_defs.h"
#include "mx78_armor.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Check whether input session_key_id is valid.
 *
 * \param[in] session_key_id           Input session_key_id
 * \param[out] session_key_valid_flag  Session key validity flag
 * \return NULL
 */
void __mx78_armor_check_session_key_id_validity(uint32_t session_key_id,
                                                bool *session_key_valid_flag);
/**
 * \brief Get security item's access address and size.
 *
 * \param[in] item   Security item type
 * \param[out] addr  Pointer to address
 * \param[out] size  Pointer to size
 * \return JEDEC_ERROR_NONE on success, otherwise JEDEC_ERROR_XX
 */
int32_t __get_target_addr_and_size(mx78_armor_security_item_t item,
                                   uint32_t *addr, uint32_t *size);
/**
 * \brief Get the minimum size of secure erase operation.
 *
 * \return The minimum size of secure erase operation.
 */
uint32_t __secure_erase_min_size(void);
/**
 * \brief Get security operation's Nonce size.
 *
 * \param[in] params  Security operation parameters
 *
 * \return Size of the nonce
 */
uint8_t __get_nonce_size(mx78_armor_security_operation_t *oper_ctx);
/**
 * \brief Get security operation's linked monotonic counter id.
 *
 * \param[in] params  Security operation parameters
 *
 * \return Monotonic counter id
 */
int32_t __get_linked_mc_id(mx78_armor_security_operation_t *oper_ctx);

/**
 * \brief Get security operation's to-be-received packet size.
 *
 * \param[in] params          Security operation parameters
 * \param[out] rd_packet_len  The size of to-be-received packet from secure Flash
 * \return NULL
 */
void __get_read_packet_size(mx78_armor_security_operation_t *oper_ctx,
                            uint32_t *rd_packet_len);

/**
 * \brief Get the messages needed by AEAD cryptographic algorithm.
 *
 * \param[in] params      Security operation parameters
 * \param[out] indicator  The cryptographic indicator of AEAD cryptographic algorithm
 *
 * \return JEDEC_ERROR_XX
 */
int32_t __get_aead_msg(mx78_armor_security_operation_t *oper_ctx,
                       crypto_indicator_t *indicator);
/**
 * \brief Get the messages needed by key derivation function.
 *
 * \param[in] root_key_id  The id of kdf's input root key
 * \param[in] mc           Pointer to the monotonic counter involved in key derivation
 * \param[in] mc_size      The size of monotonic counter
 * \param[out] indicator   The cryptographic indicator of key derivation operation
 * \return JEDEC_ERROR_XX
 */
int32_t __get_kdf_msg(uint32_t root_key_id,
                      uint8_t *mc, uint32_t mc_size,
                      crypto_indicator_t *indicator);
/**
 * \brief Prepare write packet to be sent to secure Flash.
 *
 * \param[in] params          Structure containing security operation's parameters
 * \param[in] buf             Buffer containing ciphertext data
 * \param[in] buf_size        The size of ciphertext data i bytes
 * \param[in] mac             Buffer containing authentication code
 * \param[in] mac_size        The size of authentication code
 * \param[out] wr_packet      Pointer to write packet
 * \param[out] wr_packet_len  The size of write packet in bytes
 * \param[out] rd_packet_len  The size of read packet in bytes
 *
 * \return JEDEC_ERROR_NONE if successful,
 *         or a specific JEDEC_ERROR_XX error code
 */
int32_t __prepare_write_packet(mx78_armor_security_operation_t *oper_ctx,
                               uint8_t *buf, uint32_t buf_size,
                               uint8_t *mac, uint8_t mac_size,
                               uint8_t *wr_packet, uint32_t *wr_packet_len,
                               uint32_t *rd_packet_len);

/**
 * \brief Pack the command packet to secure Flash.
 *
 * \param[in] ctx             Secure flash context
 * \param[in] type            Command packet type
 * \param[in] data            Buffer holding command data
 * \param[in] data_size       The buffer size
 * \param[out] tx_buffer      Buffer to hold command packet
 * \param[out] tx_buffer_len  The actual command packet size in byte
 *
 * \return JEDEC_ERROR_NONE if successful,
 *         or a specific JEDEC_ERROR_XX error code
 */

int32_t __packet_assemble(mx78_armor_context *ctx, uint8_t type,
                          uint8_t *data, uint32_t data_size,
                          uint8_t *tx_buffer, uint32_t *tx_buffer_len);
/**
 * \brief Pack the to-be-authenticated response from secure Flash.
 *
 * \param[in] params            Security operation's parameters
 * \param[in] resp_buf          Buffer to hold the response
 * \param[in] resp_buf_size     The size of resp_buf
 * \param[out] actual_resp_len  The actual packed response size in byte
 *
 * \return JEDEC_ERROR_NONE if successful,
 *         or a specific JEDEC_ERROR_XX error code
 */
int32_t __pack_response(mx78_armor_security_operation_t *oper_ctx,
                        uint8_t *resp_buf, uint32_t resp_buf_size,
                        uint32_t *actual_resp_len);
/**
 * \brief Parse secure Flash security configuration.
 *
 * \param[in] data_buf   Buffer holding secure Flash security configuration
 * \param[in] data_size  The size of secure Flash security configuration
 *
 * \return JEDEC_ERROR_NONE if successful,
 *         or a specific JEDEC_ERROR_XX error code
 */
int32_t __parse_security_configuration(uint8_t *data_buf, uint32_t data_size);
/**
 * \brief Parse read packet received from secure Flash.
 *
 * \param[in] params          Structure containing security operation's parameters
 * \param[out] buf            Buffer to hold data parsed from read packet
 * \param[in] buf_size        The size of data buf
 * \param[out] mac_buf        Buffer to hold mac
 * \param[in] mac_size        The size of mac
 * \param[in] rd_packet       Pointer to read packet
 * \param[in] rd_packet_len   The size of read packet in bytes
 *
 * \return JEDEC_ERROR_NONE if successful,
 *         or a specific JEDEC_ERROR_XX error code
 */
int32_t __parse_read_packet(mx78_armor_security_operation_t *oper_ctx,
                            uint8_t *buf, uint32_t buf_size,
                            uint8_t *mac_buf, uint8_t mac_size,
                            uint8_t *rd_packet, uint32_t rd_packet_len);
/**
 * \brief Parse region security description based on region configure.
 *
 * \param[in] oper_ctx         Security operation's context
 * \param[out] region_descr_p  Pointer to region description link list node
 *
 * \return JEDEC_ERROR_NONE if successful,
 *         or a specific JEDEC_ERROR_XX error code
 */
int32_t __parse_region_config(mx78_armor_security_operation_t *oper_ctx,
                              region_ll_node_t *region_descr_p);
/**
 * \brief Parse secure program response from secure Flash.
 *
 * \param[in] response_buffer       Buffer containing response
 * \param[in] response_buffer_size  The size of response buffer
 * \param[out] bytes_programmed     The size of programmed data in byte
 *
 * \return JEDEC_ERROR_NONE if successful,
 *         or a specific JEDEC_ERROR_XX error code
 */
int32_t __parse_secure_program_response(uint8_t *response_buffer,
                                        uint16_t response_buffer_size,
                                        uint32_t *bytes_programmed);
/**
 * \brief Parse secure erase response from secure Flash.
 *
 * \param[in] response_buffer       Buffer containing response
 * \param[in] response_buffer_size  The size of response buffer
 *
 * \return JEDEC_ERROR_NONE if successful,
 *         or a specific JEDEC_ERROR_XX error code
 */
int32_t __parse_secure_erase_response(uint8_t *response_buffer,
                                      uint32_t response_buffer_size);
/**
 * \brief Parse secure read response from secure Flash.
 *
 * \param[in] response_buffer        Buffer holding response
 * \param[in] response_buffer_size   The size of response buffer
 * \param[out] data                  Buffer to hold data
 * \param[out] data_len              Buffer length
 * \param[out] bytes_read            The actual size of read data in byte
 *
 * \return JEDEC_ERROR_NONE if successful,
 *         or a specific JEDEC_ERROR_XX error code
 */
int32_t __parse_secure_read_response(uint8_t *response_buffer,
                                     uint16_t response_buffer_size,
                                     uint8_t *data, uint32_t data_len,
                                     uint32_t *bytes_read);
/**
 * \brief Parse app info.
 *
 * \param[in] app_data               Buffer holding app data
 * \param[in] info_size              The size of app data
 * \param[in] app_data_num           App data item number
 *
 * \return JEDEC_ERROR_NONE if successful,
 *         or a specific JEDEC_ERROR_XX error code
 */
int32_t __parse_app_info(uint8_t *app_data, uint32_t info_size, uint8_t app_data_num);

#ifdef __cplusplus
}
#endif

#endif /* _MX78_ARMOR_LIB_H_ */
