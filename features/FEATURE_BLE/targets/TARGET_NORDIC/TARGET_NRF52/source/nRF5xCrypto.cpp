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

#include <algorithm>

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif
#include <stdio.h>
#include <string.h>

#if defined(MBEDTLS_ECDH_C)

#include "mbedtls/platform.h"
#include "mbedtls/ecdh.h"
#include "mbedtls/memory_buffer_alloc.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ecp.h"

#include "platform/NonCopyable.h"
#include "platform/CriticalSectionLock.h"
#include "ble/BLETypes.h"
#include "cmsis.h"
#include "nRF5xCrypto.h"
#include "platform/mbed_assert.h"
#include "nrf_soc.h"


namespace ble {
namespace pal {
namespace vendor {
namespace nordic {

CryptoToolbox::CryptoToolbox() : _initialized(false) {
    mbedtls_platform_setup(&_platform_context);
    mbedtls_entropy_init(&_entropy_context);
    mbedtls_ecp_group_init(&_group);
    int err = mbedtls_ecp_group_load(
        &_group,
        MBEDTLS_ECP_DP_SECP256R1
    );
    _initialized = err ? false : true;
}

CryptoToolbox::~CryptoToolbox() {
    mbedtls_ecp_group_free(&_group);
    mbedtls_entropy_free(&_entropy_context);
    mbedtls_platform_teardown(&_platform_context);
}

bool CryptoToolbox::generate_keys(
    ArrayView<uint8_t, lesc_key_size_> X,
    ArrayView<uint8_t, lesc_key_size_> Y,
    ArrayView<uint8_t, lesc_key_size_> secret
) {
    mbedtls_mpi secret_key;
    mbedtls_ecp_point public_keys;

    mbedtls_mpi_init(&secret_key);
    mbedtls_ecp_point_init(&public_keys);

    int err = mbedtls_ecp_gen_keypair(
        &_group,
        &secret_key,
        &public_keys,
        mbedtls_entropy_func,
        &_entropy_context
    );

    if (!err) {
        store_mpi(secret, secret_key);
        store_mpi(X, public_keys.X);
        store_mpi(Y, public_keys.Y);
    }

    mbedtls_ecp_point_free(&public_keys);
    mbedtls_mpi_free(&secret_key);

    return err ? false : true;
}

bool CryptoToolbox::generate_shared_secret(
    const ArrayView<const uint8_t, lesc_key_size_>& peer_X,
    const ArrayView<const uint8_t, lesc_key_size_>& peer_Y,
    const ArrayView<const uint8_t, lesc_key_size_>& own_secret,
    ArrayView<uint8_t, lesc_key_size_> shared_secret
) {
    mbedtls_mpi result;
    mbedtls_mpi secret_key;
    mbedtls_ecp_point public_keys;

    mbedtls_mpi_init(&result);
    mbedtls_mpi_init(&secret_key);
    mbedtls_ecp_point_init(&public_keys);

    load_mpi(secret_key, own_secret);
    load_mpi(public_keys.X, peer_X);
    load_mpi(public_keys.Y, peer_Y);
    mbedtls_mpi_lset( &public_keys.Z, 1 );

    int err = mbedtls_ecdh_compute_shared(
        &_group,
        &result,
        &public_keys,
        &secret_key,
        /* rng function; optional */ NULL,
        /* rng param */ NULL
    );

    if (!err) {
        store_mpi(shared_secret, result);
    }

    mbedtls_ecp_point_free(&public_keys);
    mbedtls_mpi_free(&secret_key);
    mbedtls_mpi_free(&result);

    return err ? false : true;
}

bool CryptoToolbox::ah(
    const ArrayView<const uint8_t, irk_size_>& irk,
    const ArrayView<const uint8_t, prand_size_>& prand,
    ArrayView<uint8_t, hash_size_> hash
) {
    // Note copy then swap operation can be optimized.

    // Note: the encryption block works in big endian; go figure.
    nrf_ecb_hal_data_t ecb_hal_data;

    memcpy(ecb_hal_data.key, irk.data(), irk.size());
    swap_endian(ecb_hal_data.key, sizeof(ecb_hal_data.key));

    memcpy(ecb_hal_data.cleartext, prand.data(), prand.size());
    memset(ecb_hal_data.cleartext + prand.size(), 0, sizeof(ecb_hal_data.cleartext) - prand.size());
    swap_endian(ecb_hal_data.cleartext, sizeof(ecb_hal_data.cleartext));

    uint32_t err = sd_ecb_block_encrypt(&ecb_hal_data);

    if (err) {
        return false;
    }

    swap_endian(ecb_hal_data.ciphertext, sizeof(ecb_hal_data.ciphertext));

    memcpy(hash.data(), ecb_hal_data.ciphertext, hash.size());

    return true;
}


void CryptoToolbox::load_mpi(mbedtls_mpi& dest, const ArrayView<const uint8_t, lesc_key_size_>& src) {
    ble::public_key_coord_t src_be = src.data();
    swap_endian(src_be.data(), src_be.size());
    mbedtls_mpi_read_binary(&dest, src_be.data(), src_be.size());
}

void CryptoToolbox::store_mpi(ArrayView<uint8_t, lesc_key_size_>& dest, const mbedtls_mpi& src) {
    mbedtls_mpi_write_binary(&src, dest.data(), dest.size());
    swap_endian(dest.data(), dest.size());
}

void CryptoToolbox::swap_endian(uint8_t* buf, size_t len) {
    for(size_t low = 0, high = (len - 1); high > low; --high, ++low) {
        std::swap(buf[low], buf[high]);
    }
}

} // nordic
} // vendor
} // pal
} // ble

#endif //defined(MBEDTLS_ECDH_C)

