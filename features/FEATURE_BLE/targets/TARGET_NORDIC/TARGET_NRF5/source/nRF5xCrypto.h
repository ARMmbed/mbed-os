/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#ifndef NRF5X_CRYPTO_
#define NRF5X_CRYPTO_

#include <algorithm>

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif
#include <stdio.h>
#include <string.h>


#include "mbedtls/platform.h"
#include "mbedtls/ecdh.h"
#include "mbedtls/memory_buffer_alloc.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ecp.h"

#include "platform/NonCopyable.h"
#include "platform/CriticalSectionLock.h"
#include "ble/BLETypes.h"
#include "cmsis.h"
#include "ble/ArrayView.h"

namespace ble {
namespace pal {
namespace vendor {
namespace nordic {

class CryptoToolbox : mbed::NonCopyable<CryptoToolbox> {

public:
    static const ptrdiff_t lesc_key_size_ = public_key_coord_t::size_;


    CryptoToolbox();

    ~CryptoToolbox();

    bool generate_keys(
        ArrayView<uint8_t, lesc_key_size_> X,
        ArrayView<uint8_t, lesc_key_size_> Y,
        ArrayView<uint8_t, lesc_key_size_> secret
    );

    bool generate_shared_secret(
        const ArrayView<const uint8_t, lesc_key_size_>& peer_X,
        const ArrayView<const uint8_t, lesc_key_size_>& peer_Y,
        const ArrayView<const uint8_t, lesc_key_size_>& own_secret,
        ArrayView<uint8_t, lesc_key_size_> shared_secret
    );

private:

    void load_mpi(mbedtls_mpi& dest, const ArrayView<const uint8_t, lesc_key_size_>& src);

    void store_mpi(ArrayView<uint8_t, lesc_key_size_>& dest, const mbedtls_mpi& src);

    void swap_endian(uint8_t* buf, size_t len);

    bool _initialized;
    mbedtls_entropy_context _entropy_context;
    mbedtls_ecp_group _group;
};

} // nordic
} // vendor
} // pal
} // ble

#endif // NRF5X_CRYPTO_
