/*
 * Copyright (c) 2020-2023 Macronix International Co. LTD. All rights reserved.
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

#ifndef _VENDOR_PROVISIONING_IMPL_H_
#define _VENDOR_PROVISIONING_IMPL_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* secure Flash provisioning error definition */
#define JEDEC_ERROR_PROVISION              -0x2001
#define JEDEC_ERROR_GET_PROVISION_INFO     -0x2002
#define JEDEC_ERROR_PROVISION_EXCHANGE_KEY -0x2003
#define JEDEC_ERROR_PROVISION_SYNC_SALT    -0x2004
#define JEDEC_ERROR_PROVISION_GEN_ROOT_KEY -0x2005

typedef enum {
    ITEM_APP_INFO,
    ITEM_LKD_INFO,
    LAST_ITEM_TYPE
}provision_item_type_e;

/**
 * \brief vendor specific provisioning operations
 *
 */
typedef struct {
    /**
      * \brief Perform secure Flash provisioning and verify.
      *
      * \param[in] vendor_ctx      The vendor specific secure Flash context
      * \param[in] provision_data  Input provisioning data blob
      * \param[in] data_length     The size of input provisioning data blob in bytes
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*perform_and_verify)(void *vendor_ctx,
                                  uint8_t *provision_data,
                                  uint32_t data_length);
    /**
      * \brief Get secure Flash provisioning item data.
      *
      * \param[in] vendor_ctx                The vendor specific secure Flash context
      * \param[in] item_type                 Provisioning item type
      * \param[in] provision_data_buf        Buffer to store provisioning item data
      * \param[in] provision_data_size       Buffer size
      * \param[out] item_data_num            The number of item entries
      * \param[out] provision_data_act_size  Actual size of provisioning item data
      * \return JEDEC_ERROR_NONE if successful,
      *         or a specific JEDEC_ERROR_XX error code
      */
    int32_t (*provision_item_get_data)(void *vendor_ctx,
                                       provision_item_type_e item_type,
                                       uint8_t *provision_data_buf,
                                       uint32_t provision_data_size,
                                       uint8_t *item_data_num,
                                       uint32_t *provision_data_act_size);
} vendor_provisioning_op_t;

#ifdef __cplusplus
}
#endif

#endif /* _VENDOR_PROVISIONING_IMPL_H_ */
