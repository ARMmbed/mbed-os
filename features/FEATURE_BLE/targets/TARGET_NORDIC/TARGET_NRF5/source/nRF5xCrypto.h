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

namespace ble {
namespace pal {
namespace vendor {
namespace nordic {

class LescCrypto : mbed::NonCopyable<LescCrypto> {

public:
    LescCrypto();

    ~LescCrypto();

    bool generate_keys(
        ble::public_key_coord_t& X,
        ble::public_key_coord_t& Y,
        ble::public_key_coord_t& secret
    );

    bool generate_shared_secret(
        const ble::public_key_coord_t& peer_X,
        const ble::public_key_coord_t& peer_Y,
        const ble::public_key_coord_t& own_secret,
        ble::public_key_coord_t& shared_secret
    );

private:

    void load_mpi(mbedtls_mpi& dest, const ble::public_key_coord_t& src);

    void store_mpi(ble::public_key_coord_t& dest, const mbedtls_mpi& src);

    void swap_endian(ble::public_key_coord_t& to_swap);

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
