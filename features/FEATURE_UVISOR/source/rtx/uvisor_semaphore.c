#include "api/inc/uvisor_semaphore_exports.h"
#include "api/inc/uvisor_exports.h"
#include "api/inc/halt_exports.h"
#include "cmsis_os.h"
#include <string.h>

typedef struct uvisor_semaphore_internal {
    osSemaphoreId id;
    osSemaphoreDef_t def;
    uint32_t data[2]; /* RTX expects this is 4-byte aligned */
} UVISOR_ALIGN(4) uvisor_semaphore_internal_t;

UVISOR_STATIC_ASSERT(UVISOR_SEMAPHORE_INTERNAL_SIZE >= sizeof(UvisorSemaphore), semaphore_size_too_small);

int __uvisor_semaphore_init(UvisorSemaphore * s, int32_t count)
{
    uvisor_semaphore_internal_t * semaphore = (uvisor_semaphore_internal_t *) s;

    memset(semaphore->data, 0, sizeof(semaphore->data));
    semaphore->def.semaphore = semaphore->data;
    semaphore->id = osSemaphoreCreate(&semaphore->def, count);

    /* Error when semaphore->id is NULL */
    return semaphore->id == NULL ? UVISOR_ERROR_OUT_OF_STRUCTURES : 0;
}

int __uvisor_semaphore_pend(UvisorSemaphore * s, uint32_t timeout_ms)
{
    uvisor_semaphore_internal_t * semaphore = (uvisor_semaphore_internal_t *) s;

    int32_t num_available_tokens = osSemaphoreWait(semaphore->id, timeout_ms);

    if (num_available_tokens == -1) {
        return UVISOR_ERROR_INVALID_PARAMETERS;
    }

    if (num_available_tokens == 0) {
        return UVISOR_ERROR_TIMEOUT;
    }

    return 0;
}

int __uvisor_semaphore_post(UvisorSemaphore * s) {
    uvisor_semaphore_internal_t * semaphore = (uvisor_semaphore_internal_t *) s;
    return osSemaphoreRelease(semaphore->id);
}
