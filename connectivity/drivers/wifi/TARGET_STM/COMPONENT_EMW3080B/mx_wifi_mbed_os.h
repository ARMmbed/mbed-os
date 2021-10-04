/**
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * SPDX-License-Identifier: BSD-3-Clause
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_WIFI_CMSIS_OS_H
#define MX_WIFI_CMSIS_OS_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "mbed_assert.h"

#ifndef MX_WIFI_MALLOC
#define MX_WIFI_MALLOC malloc
#endif /* MX_WIFI_MALLOC */

#ifndef MX_WIFI_FREE
#define MX_WIFI_FREE free
#endif /* MX_WIFI_FREE */

#define BASIC_MALLOC    1
/* Assume that OS is not used when bypass is disabled */

#if BASIC_MALLOC
typedef struct mx_buf {
    uint32_t len;
    uint32_t header_len;
    uint8_t  data[1];
} mx_buf_t;

static inline mx_buf_t *mx_buf_alloc(uint32_t len)
{
    mx_buf_t *p = (mx_buf_t *) MX_WIFI_MALLOC(len + sizeof(mx_buf_t) -1U);
    if (NULL != p) {
        p->len = len;
        p->header_len = 0;
    }
    return p;

}

#define MX_NET_BUFFER_ALLOC(len)                  mx_buf_alloc(len)
#define MX_NET_BUFFER_FREE(p)                     MX_WIFI_FREE(p)
#define MX_NET_BUFFER_HIDE_HEADER(p, n)           (p)->header_len += (n)
#define MX_NET_BUFFER_PAYLOAD(p)                  &(p)->data[(p)->header_len]
#define MX_NET_BUFFER_SET_PAYLOAD_SIZE(p, size)   (p)->len = (size)
#define MX_NET_BUFFER_GET_PAYLOAD_SIZE(p)         (p)->len

#else /* BASIC_ALLOC */

#define MX_NET_BUFFER_ALLOC(len)                  mx_buf_alloc((len))
#define MX_NET_BUFFER_FREE(p)                     mx_buf_free((p))
#define MX_NET_BUFFER_HIDE_HEADER(p, n)           mx_buf_hide_header((p),(n))
#define MX_NET_BUFFER_PAYLOAD(p)                  mx_buf_payload((p))
#define MX_NET_BUFFER_SET_PAYLOAD_SIZE(p, size)   mx_buf_set_size((p),(size))
#define MX_NET_BUFFER_GET_PAYLOAD_SIZE(p)         mx_buf_get_size((p))


typedef void        mx_buf_t;

mx_buf_t   *mx_buf_alloc(uint32_t len);
void        mx_buf_free(mx_buf_t *p);
void        mx_buf_hide_header(mx_buf_t *p, int32_t n);
uint8_t    *mx_buf_payload(mx_buf_t *p);
uint32_t    mx_buf_get_size(mx_buf_t *p);
void        mx_buf_set_size(mx_buf_t *p, int32_t n);

#endif /* BASIC_ALLOC */


void        mbed_delay(uint32_t delay);
void       *mbed_mutex_new(void);
void        mbed_mutex_delete(void *);
void        mbed_mutex_lock(void *);
void        mbed_mutex_unlock(void *);


void       *mbed_sem_new(uint32_t count);
void        mbed_sem_delete(void *sem);
bool        mbed_sem_signal(void *sem);
bool        mbed_sem_wait(void *sem, uint32_t timeout);

void       *mbed_queue_new(uint32_t count);
void        mbed_queue_delete(void *sem);
void       *mbed_queue_pop(void *sem, uint32_t timeout);


bool        mbed_queue_push(void *sem, void *value, uint32_t timeout);

void       *mbed_thread_new(void (*thread_entry)(void), uint32_t stacksize, uint32_t priority);
void        mbed_thread_delete(void *thread);
void        mbed_thread_terminate(void *thread);


#define MX_ASSERT(a)                              MBED_ASSERT(a)

#define LOCK_DECLARE(A)                           void* A
#define LOCK_INIT(A)                              A = mbed_mutex_new()
#define LOCK_DEINIT(A)                            mbed_mutex_delete(A)
#define LOCK(A)                                   mbed_mutex_lock(A)
#define UNLOCK(A)                                 mbed_mutex_unlock(A)

#define SEM_DECLARE(A)                            void *A
#define SEM_INIT(A,COUNT)                         A = mbed_sem_new(COUNT);
#define SEM_DEINIT(A)                             mbed_sem_delete(A)
#define SEM_SIGNAL(A)                             mbed_sem_signal(A)
#define SEM_WAIT(A,TIMEOUT,IDLE_FUNC)             mbed_sem_wait(A,TIMEOUT)

#define THREAD_DECLARE(A)                         void    *A
#define THREAD_INIT(A, THREAD_FUNC, THREAD_CONTEXT, STACKSIZE, PRIORITY)    (((A = mbed_thread_new((THREAD_CONTEXT_FUNC_TYPE)THREAD_FUNC,STACKSIZE, PRIORITY))!= NULL)?true:false)
#define THREAD_DEINIT(A)                          mbed_thread_delete(A)
#define THREAD_TERMINATE(A)                       mbed_thread_terminate(A)
#define THREAD_CONTEXT_TYPE                       void
#define THREAD_CONTEXT_FUNC_TYPE                  void (*)(void)

#define FIFO_DECLARE(QUEUE)                       void* QUEUE
#define FIFO_PUSH(QUEUE, VALUE, TIMEOUT, IDLE_FUNC)  mbed_queue_push(QUEUE, VALUE, TIMEOUT)
#define FIFO_POP(QUEUE, TIMEOUT, IDLE_FUNC)       mbed_queue_pop(QUEUE, TIMEOUT)
#define FIFO_INIT(QUEUE,QSIZE)                    QUEUE = mbed_queue_new(QSIZE)
#define FIFO_DEINIT(QUEUE)                        mbed_queue_delete(QUEUE)


#define WAIT_FOREVER                              0xffffffff
#define SEM_OK                                    true
#define THREAD_OK                                 true
#define QUEUE_OK                                  true
#define FIFO_OK                                   true
#define OSPRIORITYNORMAL                          1
#define OSPRIORITYREALTIME                        2
#define OSPRIORITYABOVENORMAL                     3
#define DELAYms(n)                                mbed_delay(n)

#ifdef __cplusplus
}
#endif


#endif /* MX_WIFI_MBED_OS_H */
