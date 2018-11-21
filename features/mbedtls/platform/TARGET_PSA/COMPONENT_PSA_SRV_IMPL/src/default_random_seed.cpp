#include "mbed.h"
#include "crypto.h"
#include "default_random_seed.h"
#include "psa_prot_internal_storage.h"

int mbed_default_seed_read(unsigned char *buf, size_t buf_len)
{
    struct psa_its_info_t info = {0, 0};
    size_t actual_size = buf_len;
    psa_its_get_info(PSA_CRYPTO_ITS_RANDOM_SEED_UID, &info);
    if (info.size < buf_len)
    {
        actual_size = info.size;
    }
    psa_its_status_t rc = psa_its_get(PSA_CRYPTO_ITS_RANDOM_SEED_UID, 0, actual_size, buf);
    /* Make sure that in case of an error the value will be negative
     * Mbed TLS errors are negative values */
    rc  = rc < 0 ? rc : (-1 * rc);
    return (rc);
}

int mbed_default_seed_write(unsigned char *buf, size_t buf_len)
{
    psa_its_status_t rc = psa_its_set(PSA_CRYPTO_ITS_RANDOM_SEED_UID, buf_len, buf, 0);
    /* Make sure that in case of an error the value will be negative
     * Mbed TLS errors are negative values */
    rc  = rc < 0 ? rc : (-1 * rc);
    return (rc);
}

