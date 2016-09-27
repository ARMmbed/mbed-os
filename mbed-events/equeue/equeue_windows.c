/*
 * Implementation for Windows
 *
 * Copyright (c) 2016 Christopher Haster
 * Distributed under the MIT license
 */
#include "equeue_platform.h"

#if defined(EQUEUE_PLATFORM_WINDOWS)

#include <windows.h>


// Tick operations
unsigned equeue_tick(void) {
    return GetTickCount();
}


// Mutex operations
int equeue_mutex_create(equeue_mutex_t *m) {
    InitializeCriticalSection(m);
    return 0;
}

void equeue_mutex_destroy(equeue_mutex_t *m) {
    DeleteCriticalSection(m);
}

void equeue_mutex_lock(equeue_mutex_t *m) {
    EnterCriticalSection(m);
}

void equeue_mutex_unlock(equeue_mutex_t *m) {
    LeaveCriticalSection(m);
}


// Semaphore operations
int equeue_sema_create(equeue_sema_t *s) {
    *s = CreateSemaphore(NULL, 0, 1, NULL);
    return *s ? 0 : -1;
}

void equeue_sema_destroy(equeue_sema_t *s) {
    CloseHandle(*s);
}

void equeue_sema_signal(equeue_sema_t *s) {
    ReleaseSemaphore(*s, 1, NULL);
}

bool equeue_sema_wait(equeue_sema_t *s, int ms) {
    if (ms < 0) {
        ms = INFINITE;
    }

    return WaitForSingleObject(*s, ms) == WAIT_OBJECT_0;
}


#endif
