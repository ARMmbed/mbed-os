#include "api/inc/uvisor_semaphore_exports.h"
#include "api/inc/uvisor_exports.h"
#include "api/inc/halt_exports.h"
#include "cmsis_os2.h"
#include "rtx_lib.h"
#include <string.h>

typedef struct uvisor_semaphore_internal {
    osSemaphoreId_t   id;
    osSemaphoreAttr_t attr;
    osRtxSemaphore_t data;
} UVISOR_ALIGN(4) uvisor_semaphore_internal_t;

UVISOR_STATIC_ASSERT(UVISOR_SEMAPHORE_INTERNAL_SIZE >= sizeof(UvisorSemaphore), semaphore_size_too_small);

int __uvisor_semaphore_init(UvisorSemaphore * s, uint32_t max_count, uint32_t initial_count)
{
    uvisor_semaphore_internal_t * semaphore = (uvisor_semaphore_internal_t *) s;

    memset(&semaphore->data, 0, sizeof(semaphore->data));
    memset(&semaphore->attr, 0, sizeof(semaphore->attr));
    semaphore->attr.name = "uvisor_semaphore";
    semaphore->attr.cb_size = sizeof(semaphore->data);
    semaphore->attr.cb_mem = &semaphore->data;
    semaphore->id = osSemaphoreNew(max_count, initial_count, &semaphore->attr);

    /* Error when semaphore->id is NULL */
    return semaphore->id == NULL ? UVISOR_ERROR_OUT_OF_STRUCTURES : 0;
}

int __uvisor_semaphore_pend(UvisorSemaphore * s, uint32_t timeout_ms)
{
    uvisor_semaphore_internal_t * semaphore = (uvisor_semaphore_internal_t *) s;

    osStatus_t status = osSemaphoreAcquire(semaphore->id, timeout_ms);

    if (status == osErrorTimeout) {
        return UVISOR_ERROR_TIMEOUT;
    } else if (status == osErrorParameter) {
        return UVISOR_ERROR_INVALID_PARAMETERS;
    } else if (status != osOK) {
        return -1; /* Other error */
    }

    return 0;
}

int __uvisor_semaphore_post(UvisorSemaphore * s) {
    uvisor_semaphore_internal_t * semaphore = (uvisor_semaphore_internal_t *) s;
    return osSemaphoreRelease(semaphore->id);
}
