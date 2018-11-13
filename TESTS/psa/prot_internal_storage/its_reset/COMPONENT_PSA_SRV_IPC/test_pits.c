#include "spm_client.h"
#include "psa_prot_internal_storage.h"
#include "test_pits.h"
#include "psa_test_its_reset_ifs.h"

psa_its_status_t test_psa_its_reset(void)
{
    psa_handle_t conn = psa_connect(TEST_PSA_ITS_RESET, 1);
    if (conn <= PSA_NULL_HANDLE) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_error_t status = psa_call(conn, NULL, 0, NULL, 0);
    if (status == PSA_DROP_CONNECTION) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_close(conn);
    return status;
}
