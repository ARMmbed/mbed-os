#include "mbed_stats.h"
#include <string.h>

#if MBED_CONF_RTOS_PRESENT
#include "cmsis_os.h"
#endif

// note: mbed_stats_heap_get defined in mbed_alloc_wrappers.cpp

void mbed_stats_stack_get(mbed_stats_stack_t *stats)
{
    memset(stats, 0, sizeof(mbed_stats_stack_t));

#if MBED_STACK_STATS_ENABLED && MBED_CONF_RTOS_PRESENT
    osThreadEnumId enumid = _osThreadsEnumStart();
    osThreadId threadid;

    while ((threadid = _osThreadEnumNext(enumid))) {
        osEvent e;

        e = _osThreadGetInfo(threadid, osThreadInfoStackMax);
        if (e.status == osOK) {
           stats->max_size += (uint32_t)e.value.p;
        }

        e = _osThreadGetInfo(threadid, osThreadInfoStackSize);
        if (e.status == osOK) {
           stats->reserved_size += (uint32_t)e.value.p;
        }

        stats->stack_cnt += 1;
    }
#elif MBED_STACK_STATS_ENABLED
#warning Stack statistics are not supported without the rtos.
#endif
}

