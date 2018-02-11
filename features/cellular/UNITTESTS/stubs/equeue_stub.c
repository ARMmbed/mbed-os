
#include "equeue.h"

int equeue_create(equeue_t *queue, size_t size)
{
    return 0;
}

int equeue_create_inplace(equeue_t *queue, size_t size, void *buffer)
{
    return 0;
}

void equeue_destroy(equeue_t *queue)
{

}

void equeue_dispatch(equeue_t *queue, int ms)
{

}

void equeue_break(equeue_t *queue)
{

}

int equeue_call(equeue_t *queue, void (*cb)(void *), void *data)
{
    return 0;
}

int equeue_call_in(equeue_t *queue, int ms, void (*cb)(void *), void *data)
{
    return 0;
}

int equeue_call_every(equeue_t *queue, int ms, void (*cb)(void *), void *data)
{
    return 0;
}

void *equeue_alloc(equeue_t *queue, size_t size)
{
    return NULL;
}

void equeue_dealloc(equeue_t *queue, void *event)
{

}

void equeue_event_delay(void *event, int ms)
{

}

void equeue_event_period(void *event, int ms)
{

}

void equeue_event_dtor(void *event, void (*dtor)(void *))
{

}

int equeue_post(equeue_t *queue, void (*cb)(void *), void *event)
{
    return 0;
}

void equeue_cancel(equeue_t *queue, int id)
{

}

void equeue_background(equeue_t *queue,
        void (*update)(void *timer, int ms), void *timer)
{

}

void equeue_chain(equeue_t *queue, equeue_t *target)
{

}
