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
#ifndef _VENDOR_IMPL_H_
#define _VENDOR_IMPL_H_

#include <stdint.h>
#include <stddef.h>
#include "include/jedec_defs.h"
#include "queue.h"
#include "crypto_wrapper.h"

#ifdef __cplusplus
    extern "C" {
#endif

/**
 * \brief vendor specific security operations
 *
 */
typedef struct {
    /**
      * \brief Vendor id.
      */
    uint8_t vendor_id;
    /**
      * \brief Pre create session.
      *
      * \param[in]  vendor_ctx  The vendor specific secure Flash context
      * \param[in]  nonce       Host input nonce
      * \param[in]  nonce_len   Host nonce length
      * \param[out] resp_queue  Pointer to response queue holding responses from flash
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*pre_create_session)(void *vendor_ctx, uint8_t *nonce,
                                  uint32_t nonce_len, jqueue_t *resp_queue);
    /**
      * \brief Pack create session packet.
      *
      * \param[in]  vendor_ctx   The vendor specific secure Flash context
      * \param[in]  root_key_id  Associated root key id
      * \param[in]  packet       Pointer to packet
      * \param[in]  packet_len   Actual packet length
      * \param[out] indicator    Indicator holding crypto algorithm parameters
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*create_session_packet)(void *vendor_ctx, uint32_t root_key_id,
                                     uint8_t *packet, uint32_t *packet_len,
                                     crypto_indicator_t *indicator);
    /**
      * \brief Post create session.
      *
      * \param[in]  vendor_ctx      The vendor specific secure Flash context
      * \param[in]  root_key_id     Associated root key id
      * \param[in]  session_key_id  created session key id
      * \param[in]  nonce           Nonce
      * \param[in]  nonce_len       Nonce length
      * \param[out] resp_queue      Pointer to response queue holding responses from flash
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*post_create_session)(void *vendor_ctx, uint32_t root_key_id, 
                                   uint32_t session_key_id,
                                   uint8_t *nonce, uint32_t nonce_len,
                                   jqueue_t *resp_queue);
#if defined(SESSION_CONFIRMATION)
    int32_t (*pre_confirm_session)();
    int32_t (*confirm_session_packet)();
    int32_t (*post_confirm_session)();
#endif
    /**
      * \brief Pre terminate session.
      *
      * \param[in]  vendor_ctx  The vendor specific secure Flash context
      * \param[in]  nonce       Host input nonce
      * \param[in]  nonce_len   Host nonce length
      * \param[out] resp_queue  Pointer to response queue holding responses from flash
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*pre_terminate_session)(void *vendor_ctx, uint8_t *nonce,
                                     uint32_t nonce_len, jqueue_t *resp_queue);
    /**
      * \brief Pack terminate session packet.
      *
      * \param[in]  vendor_ctx      The vendor specific secure Flash context
      * \param[in]  session_key_id  Associated session key id
      * \param[in]  packet          Pointer to packet
      * \param[in]  packet_len      Actual packet length
      * \param[out] indicator       Indicator holding crypto algorithm parameters
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*terminate_session_packet)(void *vendor_ctx,
                                        uint32_t session_key_id,
                                        uint8_t *packet, uint32_t *packet_len,
                                        crypto_indicator_t *indicator);
    /**
      * \brief Post terminate session.
      *
      * \param[in]  vendor_ctx  The vendor specific secure Flash context
      * \param[in]  session_key_id  Associated session key id
      * \param[out] resp_queue  Pointer to response queue holding responses from flash
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*post_terminate_session)(void *vendor_ctx, 
                                      uint32_t session_key_id, 
                                      jqueue_t *resp_queue);
    /**
       * \brief Pre secure init.
       *
       * \param[in]  vendor_ctx  The vendor specific secure Flash context
       * \param[in]  nonce       Host input nonce
       * \param[in]  nonce_len   Host nonce length
       * \param[out] resp_queue  Pointer to response queue holding responses from flash
       * \return JEDEC_ERROR_NONE if successful,
       *         or a specific JEDEC_ERROR_XX error code
       */
    int32_t (*pre_secure_init)(void *vendor_ctx, uint8_t *nonce,
                               uint32_t nonce_len, jqueue_t *resp_queue);
    /**
      * \brief Pack secure init packet.
      *
      * \param[in]  vendor_ctx   The vendor specific secure Flash context
      * \param[in]  root_key_id  Associated root key id
      * \param[in]  packet       Pointer to packet
      * \param[in]  packet_len   Actual packet length
      * \param[out] indicator    Indicator holding crypto algorithm parameters
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*secure_init_packet)(void *vendor_ctx, uint32_t root_key_id,
                                  uint8_t *packet, uint32_t *packet_len,
                                  crypto_indicator_t *indicator);
    /**
      * \brief Post secure init.
      *
      * \param[in]  vendor_ctx  The vendor specific secure Flash context
      * \param[out] resp_queue  Pointer to response queue holding responses from flash
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*post_secure_init)(void *vendor_ctx, jqueue_t *resp_queue);
    /**
       * \brief Pre secure uninit.
       *
       * \param[in]  vendor_ctx  The vendor specific secure Flash context
       * \param[in]  nonce       Host input nonce
       * \param[in]  nonce_len   Host nonce length
       * \param[out] resp_queue  Pointer to response queue holding responses from flash
       * \return JEDEC_ERROR_NONE if successful,
       *         or a specific JEDEC_ERROR_XX error code
       */
    int32_t (*pre_secure_uninit)(void *vendor_ctx, uint8_t *nonce,
                                 uint32_t nonce_len, jqueue_t *resp_queue);
    /**
      * \brief Pack secure uninit packet.
      *
      * \param[in]  vendor_ctx   The vendor specific secure Flash context
      * \param[in]  root_key_id  Associated root key id
      * \param[in]  packet       Pointer to packet
      * \param[in]  packet_len   Actual packet length
      * \param[out] indicator    Indicator holding crypto algorithm parameters
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*secure_uninit_packet)(void *vendor_ctx, uint32_t root_key_id,
                                    uint8_t *packet, uint32_t *packet_len,
                                    crypto_indicator_t *indicator);
    /**
      * \brief Post secure uninit.
      *
      * \param[in]  vendor_ctx  The vendor specific secure Flash context
      * \param[out] resp_queue  Pointer to response queue holding responses from flash
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*post_secure_uninit)(void *vendor_ctx, jqueue_t *resp_queue);
    /**
       * \brief Pre secure program.
       *
       * \param[in]  vendor_ctx      The vendor specific secure Flash context
       * \param[in]  addr            Destination address
       * \param[in]  session_key_id  Associated session key id
       * \param[out] resp_queue      Pointer to response queue holding responses from flash
       * \return JEDEC_ERROR_NONE if successful,
       *         or a specific JEDEC_ERROR_XX error code
       */
    int32_t (*pre_secure_program)(void *vendor_ctx, uint32_t addr,
                                  uint32_t session_key_id, jqueue_t *resp_queue);
    /**
      * \brief Pack secure program packet.
      *
      * \param[in]  vendor_ctx      The vendor specific secure Flash context
      * \param[in]  addr            Destination address
      * \param[in]  data            Pointer to source data
      * \param[in]  len             Data length
      * \param[in]  session_key_id  Associated session key id
      * \param[in]  packet          Pointer to packet
      * \param[in]  packet_len      Actual packet length
      * \param[out] indicator       Indicator holding crypto algorithm parameters
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*secure_program_packet)(void *vendor_ctx, uint32_t addr,
                                     const uint8_t *data, uint32_t len,
                                     uint32_t session_key_id,
                                     uint8_t *packet, uint32_t *packet_len,
                                     crypto_indicator_t *indicator);
    /**
      * \brief Post secure program.
      *
      * \param[in]  vendor_ctx               The vendor specific secure Flash context
      * \param[in]  session_key_id           Associated session key id
      * \param[in]  resp_queue               Pointer to response queue holding responses from flash
      * \param[in]  resp_packet_buffer       Buffer to hold secure program response from flash
      * \param[out] resp_packet_buffer_size  Actual secure program response size
      * \param[out] indicator                Indicator holding crypto algorithm parameters
      *                                      (if secure program response also needs crypto operations)
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*post_secure_program)(void *vendor_ctx, jqueue_t *resp_queue,
                                   uint32_t session_key_id, 
                                   uint8_t *resp_packet_buffer,
                                   uint32_t *resp_packet_buffer_size,
                                   crypto_indicator_t *indicator);
    /**
      * \brief Parse secure program response.
      *
      * \param[in]  vendor_ctx        The vendor specific secure Flash context
      * \param[in]  indicator         Indicator holding crypto algorithm parameters
      * \param[out] bytes_programmed  Actual programmed bytes
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*parse_secure_program_response)(void *vendor_ctx,
                                             crypto_indicator_t *indicator,
                                             uint32_t *bytes_programmed);
    /**
       * \brief Pre secure erase.
       *
       * \param[in]  vendor_ctx      The vendor specific secure Flash context
       * \param[in]  addr            Destination address
       * \param[in]  len             Erase size
       * \param[in]  session_key_id  Associated session key id
       * \param[out] resp_queue      Pointer to response queue holding responses from flash
       * \return JEDEC_ERROR_NONE if successful,
       *         or a specific JEDEC_ERROR_XX error code
       */
    int32_t (*pre_secure_erase)(void *vendor_ctx, uint32_t addr, uint32_t len,
                                uint32_t session_key_id, jqueue_t *resp_queue);
    /**
      * \brief Pack secure erase packet.
      *
      * \param[in]  vendor_ctx      The vendor specific secure Flash context
      * \param[in]  addr            Destination address
      * \param[in]  len             Erase size
      * \param[in]  session_key_id  Associated session key id
      * \param[in]  packet          Pointer to packet
      * \param[in]  packet_len      Actual packet length
      * \param[out] indicator       Indicator holding crypto algorithm parameters
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*secure_erase_packet)(void *vendor_ctx, uint32_t addr, uint32_t len,
                                   uint32_t session_key_id,
                                   uint8_t *packet, uint32_t *packet_len,
                                   crypto_indicator_t *indicator);
    /**
      * \brief Post secure erase.
      *
      * \param[in]  vendor_ctx               The vendor specific secure Flash context
      * \param[in]  session_key_id           Associated session key id
      * \param[in]  resp_queue               Pointer to response queue holding responses from flash
      * \param[in]  resp_packet_buffer       Buffer to hold secure erase response from flash
      * \param[out] resp_packet_buffer_size  Actual secure erase response size
      * \param[out] indicator                Indicator holding crypto algorithm parameters
      *                                      (if secure erase response also needs crypto operations)
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*post_secure_erase)(void *vendor_ctx,
                                 uint32_t session_key_id,
                                 uint8_t *resp_packet_buffer,
                                 uint32_t *resp_packet_buffer_size,
                                 crypto_indicator_t *indicator,
                                 jqueue_t *resp_queue);
    /**
      * \brief Parse secure erase response.
      *
      * \param[in] vendor_ctx  The vendor specific secure Flash context
      * \param[in] indicator   Indicator holding crypto algorithm parameters
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*parse_secure_erase_response)(void *vendor_ctx,
                                           crypto_indicator_t *indicator);
#if defined(SECURE_COPY)
    int32_t (*pre_secure_copy)();
    int32_t (*secure_copy_packet)();
    int32_t (*post_secure_copy)();
    int32_t (*parse_secure_copy_response)();
#endif
#if defined(SECURE_READ)
    /**
       * \brief Pre secure read.
       *
       * \param[in]  vendor_ctx      The vendor specific secure Flash context
       * \param[in]  addr            Target address
       * \param[in]  session_key_id  Associated session key id
       * \param[out] resp_queue      Pointer to response queue holding responses from flash
       * \return JEDEC_ERROR_NONE if successful,
       *         or a specific JEDEC_ERROR_XX error code
       */
    int32_t (*pre_secure_read)(void *vendor_ctx, uint32_t addr,
                               uint32_t session_key_id, jqueue_t *resp_queue);
    /**
      * \brief Pack secure read packet.
      *
      * \param[in]  vendor_ctx      The vendor specific secure Flash context
      * \param[in]  addr            Target address
      * \param[in]  len             Data length
      * \param[in]  session_key_id  Associated session key id
      * \param[in]  packet          Pointer to packet
      * \param[in]  packet_len      Actual packet length
      * \param[out] indicator       Indicator holding crypto algorithm parameters
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*secure_read_packet)(void *vendor_ctx, uint32_t addr,
                                  uint32_t len, uint32_t session_key_id,
                                  uint8_t *packet, uint32_t *packet_len,
                                  crypto_indicator_t *indicator);
    /**
      * \brief Post secure read.
      *
      * \param[in]  vendor_ctx               The vendor specific secure Flash context
      * \param[in]  session_key_id           Associated session key id
      * \param[in]  resp_queue               Pointer to response queue holding responses from flash
      * \param[in]  resp_packet_buffer       Buffer to hold secure read response from flash
      * \param[out] resp_packet_buffer_size  Actual secure read response size
      * \param[out] indicator                Indicator holding crypto algorithm parameters
      *                                      (if secure read response also needs crypto operations)
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*post_secure_read)(void *vendor_ctx, 
                                uint32_t session_key_id,
                                uint8_t *resp_packet_buffer,
                                uint32_t *resp_packet_buffer_size,
                                crypto_indicator_t *indicator,
                                jqueue_t *resp_queue);
    /**
      * \brief Parse secure read response.
      *
      * \param[in]  vendor_ctx  The vendor specific secure Flash context
      * \param[in]  indicator   Indicator holding crypto algorithm parameters
      * \param[out] data        Buffer to hold read data
      * \param[in]  data_len    Read data length
      * \param[out] bytes_read  Actual read bytes
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*parse_secure_read_response)(void *vendor_ctx,
                                          crypto_indicator_t *indicator,
                                          uint8_t *data, uint32_t data_len,
                                          uint32_t *bytes_read);
#endif
    /**
       * \brief Pre get regions' information in security.
       *
       * \param[in]  vendor_ctx              The vendor specific secure Flash context
       * \param[in]  session_key_id          Associated session key id
       * \param[out] session_key_valid_flag  Session key valid flag
       * \param[out] resp_queue              Pointer to response queue holding responses from flash
       * \return JEDEC_ERROR_NONE if successful,
       *         or a specific JEDEC_ERROR_XX error code
       */
    int32_t (*pre_secure_get_regions_info)(void *vendor_ctx,
                                           uint32_t session_key_id,
                                           bool *session_key_valid_flag,
                                           jqueue_t *resp_queue);
    /**
      * \brief Pack get regions' information in security packet.
      *
      * \param[in]  vendor_ctx      The vendor specific secure Flash context
      * \param[in]  session_key_id  Associated session key id
      * \param[in]  region_index    Region index
      * \param[in]  nonce           Nonce
      * \param[in]  nonce_len       Nonce length
      * \param[in]  packet          Pointer to packet
      * \param[in]  packet_len      Actual packet length
      * \param[out] indicator       Indicator holding crypto algorithm parameters
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*secure_get_regions_info_packet)(void *vendor_ctx,
                                              uint32_t session_key_id,
                                              int8_t region_index,
                                              uint8_t *nonce, uint32_t nonce_len,
                                              uint8_t *packet, uint32_t *packet_len,
                                              crypto_indicator_t *indicator);
    /**
      * \brief Post get regions' information in security.
      *
      * \param[in]  vendor_ctx      The vendor specific secure Flash context
      * \param[in]  region_descr_p  Pointer to the link list holding region description
      * \param[out] resp_queue      Pointer to response queue holding responses from flash
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*post_secure_get_regions_info)(void *vendor_ctx,
                                            region_ll_node_t *region_descr_p,
                                            jqueue_t *resp_queue);
#if defined(SECURE_REGION_MANAGE)
    int32_t (*pre_secure_manage_region)(void *vendor_ctx);
    int32_t (*secure_manage_region_packet)(void *vendor_ctx);
    int32_t (*post_secure_manage_region)(void *vendor_ctx);
#endif
    /**
      * \brief Send security command packet to secure flash.
      *
      * \param[in]     vendor_ctx       The vendor specific secure Flash context
      * \param[in/out] packet_out       Pointer to command packet
      * \param[in]     packet_len       Current packet length without cipher and mac
      * \param[in]     cipher_text      Pointer to cipher text
      * \param[in]     cipher_text_len  Cipher text length
      * \param[in]     mac              Pointer to mac
      * \param[in]     mac_len          Mac length
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*packet_send)(void *vendor_ctx,
                           uint8_t *packet_out, uint16_t packet_len,
                           uint8_t *cipher_text, uint16_t cipher_text_len,
                           uint8_t *mac, uint16_t mac_len);
    /**
      * \brief Receive security command response packet from secure flash.
      *
      * \param[in] vendor_ctx  The vendor specific secure Flash context
      * \param[in] packet_in   Pointer to response packet
      * \param[in] packet_len  Response packet length
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*packet_receive)(void *vendor_ctx,
                              uint8_t *packet_in, uint16_t packet_len);
} vendor_security_op_t;

#ifdef __cplusplus
}
#endif

#endif /* _VENDOR_IMPL_H_ */
