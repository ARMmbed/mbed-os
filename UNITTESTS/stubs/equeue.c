#include "equeue/equeue.h"

void *equeue_alloc(equeue_t *q, size_t size)
{
}

int equeue_post(equeue_t *q, void (*cb)(void *), void *p)
{
    return 0;
}

void equeue_event_delay(void *p, int ms)
{
}

void equeue_event_period(void *p, int ms)
{
}

void equeue_event_dtor(void *p, void (*dtor)(void *))
{
}
