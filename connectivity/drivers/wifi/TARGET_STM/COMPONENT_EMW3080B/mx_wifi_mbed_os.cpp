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

#include "EMW3080B_EMAC.h"

#include "mx_wifi_mbed_os.h"

extern "C"
{

#if BASIC_MALLOC == 0

    mx_buf_t   *mx_buf_alloc(uint32_t len)
    {
        emac_mem_buf_t *p = emac3080b_global_memory_manager->alloc_pool(len, 0);
        return (mx_buf_t *) p;
    }


    void            mx_buf_free(mx_buf_t *netb_in)
    {
        emac_mem_buf_t *netb = (emac_mem_buf_t *) netb_in;
        emac3080b_global_memory_manager->free(netb);
    }


    void            mx_buf_hide_header(mx_buf_t *netb_in, int32_t n)
    {
        if (n <= 0) {
            error("Hiding header is not supported\n");
        } else {
            emac_mem_buf_t *netb = (emac_mem_buf_t *) netb_in;
            ssize_t len = emac3080b_global_memory_manager->get_len(netb);
            uint8_t    *pdata = (uint8_t *) emac3080b_global_memory_manager->get_ptr(netb);

            memmove(pdata, pdata + n, len);
        }

    }

    uint8_t    *mx_buf_payload(mx_buf_t *netb_in)
    {
        emac_mem_buf_t *netb = (emac_mem_buf_t *) netb_in;
        return (uint8_t *) emac3080b_global_memory_manager->get_ptr(netb);
    }


    uint32_t  mx_buf_get_size(mx_buf_t *netb_in)
    {
        emac_mem_buf_t *netb = (emac_mem_buf_t *) netb_in;
        return emac3080b_global_memory_manager->get_len(netb);
    }

    void            mx_buf_set_size(mx_buf_t *netb_in, int32_t n)
    {
        emac_mem_buf_t *netb = (emac_mem_buf_t *) netb_in;
        return emac3080b_global_memory_manager->set_len(netb, n);

    }

#endif /* BASIC_ALLOC */


    void        mbed_delay(uint32_t delay)
    {
        ThisThread::sleep_for(std::chrono::duration<uint32_t, std::milli>(delay));
    }


    void       *mbed_mutex_new(void)
    {
        void *m;
        m = new Mutex();
        return (void *) m;
    }

    void mbed_mutex_delete(void *m)
    {
        delete ((Mutex *) m);
    }


    void        mbed_mutex_lock(void *mutexin)
    {
        Mutex  *mutex = (Mutex *) mutexin;
        mutex->lock();
    }

    void        mbed_mutex_unlock(void *mutexin)
    {
        Mutex  *mutex = (Mutex *) mutexin;
        mutex->unlock();
    }

    void       *mbed_sem_new(uint32_t count)
    {
        Semaphore   *sem;
        sem = new Semaphore(count);
        for (uint32_t ii = 0; ii < count ; ii++) {
            sem->acquire();
        }
        return (void *) sem;

    }
    void        mbed_sem_delete(void *insem)
    {
        Semaphore   *sem = (Semaphore *) insem;
        delete sem;
    }
    bool        mbed_sem_signal(void *semin)
    {
        bool    ret;
        Semaphore *sem = (Semaphore *) semin;
        ret = (sem->release() == osOK);
        return ret;
    }
    bool        mbed_sem_wait(void *semin, uint32_t timeout)
    {
        bool    ret;
        Semaphore *sem = (Semaphore *) semin;
        ret = sem->try_acquire_for(std::chrono::duration<uint32_t, std::milli>(timeout));
        return ret;
    }

#define QSIZE   10

    void       *mbed_queue_new(uint32_t count)
    {
        Queue<uint32_t, QSIZE> *queue;
        queue = new Queue<uint32_t, QSIZE> ;
        return (void *) queue;
    }

    void        mbed_queue_delete(void *queuein)
    {
        Queue<uint32_t, QSIZE> *queue = (Queue<uint32_t, QSIZE> *) queuein;
        delete queue;
    }

    void       *mbed_queue_pop(void *queuein, uint32_t timeout)
    {
        uint32_t    *message = NULL;
        Queue<uint32_t, QSIZE> *queue = (Queue<uint32_t, QSIZE> *) queuein;
        while (true) {
            if (queue->try_get_for(std::chrono::duration<uint32_t, std::milli>(timeout), &message)) {
                break;
            }
        }
        return (void *) message;
    }

    bool        mbed_queue_push(void *queuein, void *value, uint32_t timeout)
    {
        Queue<uint32_t, QSIZE> *queue = (Queue<uint32_t, QSIZE> *) queuein;
        return queue->try_put_for(std::chrono::duration<uint32_t, std::milli>(timeout), (uint32_t *)value);
    }



    void   *mbed_thread_new(void (*thread_entry)(void), uint32_t stacksize, uint32_t prio_in)
    {
        osPriority_t    priority;
        priority = osPriorityNormal;
        if (prio_in == OSPRIORITYREALTIME) {
            priority = osPriorityRealtime;
        }
        if (prio_in == OSPRIORITYABOVENORMAL) {
            priority = osPriorityAboveNormal;
        }


        Thread    *thread = new Thread(priority, stacksize);
        if (thread) {
            thread->start(callback(thread_entry));
        }
        return (void *) thread;
    }

    void        mbed_thread_delete(void *threadin)
    {
        Thread *thread = (Thread *) threadin;
        thread->join();
    }

    void        mbed_thread_terminate(void *threadin)
    {
        Thread *thread = (Thread *) threadin;
        thread->terminate();
    }

} /* end of extern "C" */
