/*
 * Implementation for the mbed library
 * https://github.com/mbedmicro/mbed
 *
 * Copyright (c) 2016 Christopher Haster
 * Distributed under the MIT license
 */
#include "equeue_platform.h"

#if defined(EQUEUE_PLATFORM_FREERTOS)

#include "task.h"


// Ticker operations
unsigned equeue_tick(void) {
    return xTaskGetTickCountFromISR() * portTICK_PERIOD_MS;
}


// Mutex operations
int equeue_mutex_create(equeue_mutex_t *m) { return 0; }
void equeue_mutex_destroy(equeue_mutex_t *m) { }

void equeue_mutex_lock(equeue_mutex_t *m) {
    *m = taskENTER_CRITICAL_FROM_ISR();
}

void equeue_mutex_unlock(equeue_mutex_t *m) {
    taskEXIT_CRITICAL_FROM_ISR(*m);
}


// Semaphore operations
int equeue_sema_create(equeue_sema_t *s) {
    s->handle = xSemaphoreCreateBinaryStatic(&s->buffer);
    return s->handle ? 0 : -1;
}

void equeue_sema_destroy(equeue_sema_t *s) {
    vSemaphoreDelete(s->handle);
}

void equeue_sema_signal(equeue_sema_t *s) {
    xSemaphoreGiveFromISR(s->handle, NULL);
}

bool equeue_sema_wait(equeue_sema_t *s, int ms) {
    if (ms < 0) {
        ms = portMAX_DELAY;
    } else {
        ms = ms / portTICK_PERIOD_MS;
    }

    return xSemaphoreTake(s->handle, ms);
}


#endif
