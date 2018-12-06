#include "mbed.h"
#include "crypto.h"
#include "default_random_seed.h"
#include "psa_prot_internal_storage.h"

int mbed_default_seed_read(unsigned char *buf, size_t buf_len)
{
    /* Make sure that in case of an error the value will be negative
     * return (-1 * rc);
     * Mbed TLS errors are negative values
     */
    psa_its_status_t rc = psa_its_get(PSA_CRYPTO_ITS_RANDOM_SEED_UID, 0, buf_len, buf);
    return ( -1 * rc );
}

int mbed_default_seed_write(unsigned char *buf, size_t buf_len)
{
    psa_its_status_t rc = psa_its_set(PSA_CRYPTO_ITS_RANDOM_SEED_UID, buf_len, buf, 0);
    /* Make sure that in case of an error the value will be negative
     * return (-1 * rc);
     * Mbed TLS errors are negative values
     */
    return ( -1 * rc );
}

