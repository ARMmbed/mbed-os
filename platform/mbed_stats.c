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
#endif
}

size_t mbed_stats_stack_get_each(mbed_stats_stack_t *stats, size_t count)
{
    memset(stats, 0, count*sizeof(mbed_stats_stack_t));
    size_t i = 0;

#if MBED_STACK_STATS_ENABLED && MBED_CONF_RTOS_PRESENT
    osThreadEnumId enumid = _osThreadsEnumStart();
    osThreadId threadid;

    while ((threadid = _osThreadEnumNext(enumid)) && i < count) {
        osEvent e;

        e = _osThreadGetInfo(threadid, osThreadInfoStackMax);
        if (e.status == osOK) {
           stats[i].max_size = (uint32_t)e.value.p;
        }

        e = _osThreadGetInfo(threadid, osThreadInfoStackSize);
        if (e.status == osOK) {
           stats[i].reserved_size = (uint32_t)e.value.p;
        }

        stats[i].thread_id = (uint32_t)threadid;
        stats[i].stack_cnt = 1;
        i += 1;
    }
#endif

    return i;
}

#if MBED_STACK_STATS_ENABLED && !MBED_CONF_RTOS_PRESENT
#warning Stack statistics are currently not supported without the rtos.
#endif
