/*
 * Copyright (c) , Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
