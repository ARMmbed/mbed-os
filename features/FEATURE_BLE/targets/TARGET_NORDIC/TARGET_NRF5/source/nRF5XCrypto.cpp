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
#include "nRF5xCrypto.h"

namespace ble {
namespace pal {
namespace vendor {
namespace nordic {

LescCrypto::LescCrypto() : _initialized(false) {
    mbedtls_entropy_init(&_entropy_context);
    mbedtls_ecp_group_init(&_group);
    int err = mbedtls_ecp_group_load(
        &_group,
        MBEDTLS_ECP_DP_SECP256R1
    );
    _initialized = err ? false : true;
}

LescCrypto::~LescCrypto() {
    mbedtls_ecp_group_free(&_group);
    mbedtls_entropy_free(&_entropy_context);
}

bool LescCrypto::generate_keys(
    ble::public_key_coord_t& X,
    ble::public_key_coord_t& Y,
    ble::public_key_coord_t& secret
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

bool LescCrypto::generate_shared_secret(
    const ble::public_key_coord_t& peer_X,
    const ble::public_key_coord_t& peer_Y,
    const ble::public_key_coord_t& own_secret,
    ble::public_key_coord_t& shared_secret
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


void LescCrypto::load_mpi(mbedtls_mpi& dest, const ble::public_key_coord_t& src) {
    ble::public_key_coord_t src_be = src;
    swap_endian(src_be);
    mbedtls_mpi_read_binary(&dest, src_be.buffer(), src_be.size());
}

void LescCrypto::store_mpi(ble::public_key_coord_t& dest, const mbedtls_mpi& src) {
    mbedtls_mpi_write_binary(&src, dest.buffer(), dest.size());
    swap_endian(dest);
}

void LescCrypto::swap_endian(ble::public_key_coord_t& to_swap) {
    swap_endian(to_swap.buffer(), to_swap.size());
}

void LescCrypto::swap_endian(uint8_t* buf, size_t len) {
    for(size_t low = 0, high = (len - 1); high > low; --high, ++low) {
        std::swap(buf[low], buf[high]);
    }
}

} // nordic
} // vendor
} // pal
} // ble

