/*
 * Copyright (c) 2016-2019, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include <string.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "mbedtls/md.h"
#include "Service_Libs/hmac/hmac_sha1.h"

#define TRACE_GROUP "hmac"

int8_t hmac_sha1_calc(const uint8_t *key, uint16_t key_len, const uint8_t *data, uint16_t data_len, uint8_t *result, uint8_t result_len)
{
#ifdef EXTRA_DEBUG_INFO
    // Extensive debug for now, to be disabled later
    tr_debug("hmac_sha_1 key %s\n", trace_array(key, key_len));

    const uint8_t *print_data = data;
    uint16_t print_data_len = data_len;
    while (true) {
        tr_debug("hmac_sha_1 data %s\n", trace_array(print_data, print_data_len > 32 ? 32 : print_data_len));
        if (print_data_len > 32) {
            print_data_len -= 32;
            print_data += 32;
        } else {
            break;
        }
    }
#endif

    const mbedtls_md_type_t md_type = MBEDTLS_MD_SHA1;
    const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(md_type);
    if (md_info == NULL) {
        return -1;
    }

    mbedtls_md_context_t ctx;

    mbedtls_md_init(&ctx);
    if (mbedtls_md_setup(&ctx, md_info, 1) != 0) {
        return -1;
    }
    if (mbedtls_md_hmac_starts(&ctx, (const unsigned char *) key, key_len) != 0) {
        goto error;
    }
    if (mbedtls_md_hmac_update(&ctx, (const unsigned char *) data, data_len) != 0) {
        goto error;
    }

    uint8_t result_value[20];
    if (mbedtls_md_hmac_finish(&ctx, result_value) != 0) {
        goto error;
    }
    mbedtls_md_free(&ctx);

    if (result_len > 20) {
        result_len = 20;
    }

    memcpy(result, result_value, result_len);

#ifdef EXTRA_DEBUG_INFO
    tr_debug("hmac_sha_1 result %s\n", trace_array(result_value, 20));
#endif
    return 0;

error:
    mbedtls_md_free(&ctx);
    return -1;
}
