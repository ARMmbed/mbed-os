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

class LescCrypto : mbed::NonCopyable<LescCrypto> {

public:
    LescCrypto();

    ~LescCrypto();

    bool generate_keys(
        ArrayView<uint8_t> X,
        ArrayView<uint8_t> Y,
        ArrayView<uint8_t> secret
    );

    bool generate_shared_secret(
        const ArrayView<const uint8_t>& peer_X,
        const ArrayView<const uint8_t>& peer_Y,
        const ArrayView<const uint8_t>& own_secret,
        ArrayView<uint8_t> shared_secret
    );

private:

    void load_mpi(mbedtls_mpi& dest, const ArrayView<const uint8_t>& src);

    void store_mpi(ArrayView<uint8_t>& dest, const mbedtls_mpi& src);

    void swap_endian(ArrayView<uint8_t>& to_swap);

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
