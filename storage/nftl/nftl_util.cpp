#include <stdlib.h>
#include "nftl_util.h"

// Software CRC implementation
uint32_t nftl_crc32(uint32_t init, const void *buffer, size_t size) {
    uint32_t crc = init;

    static const uint32_t rtable[16] = {
        0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
        0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
        0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
        0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c,
    };

    const uint8_t *data = (uint8_t*)buffer;

    for (size_t i = 0; i < size; i++) {
        crc = (crc >> 4) ^ rtable[(crc ^ (data[i] >> 0)) & 0xf];
        crc = (crc >> 4) ^ rtable[(crc ^ (data[i] >> 4)) & 0xf];
    }

    return crc;
}

/* Global NFTL lock init */
#ifdef NFTL_SIM_DEBUG
#include <pthread.h>

void nftl_lock_create(nftl_lock_t *lock)
{
    pthread_mutex_init(lock, NULL);
}

void nftl_lock_destroy(nftl_lock_t *lock)
{
    pthread_mutex_destroy(lock);
}

void nftl_lock(nftl_lock_t *lock)
{
    int ret;
    ret = pthread_mutex_lock(lock);
    assert(ret == 0);
}

void nftl_unlock(nftl_lock_t *lock)
{
    int ret;
    ret = pthread_mutex_unlock(lock);
    assert(ret == 0);
}

typedef void (*nftl_fn_t)(void *);
typedef struct {
    nftl_fn_t func;
    void *arg;
} thread_helper_t;

static void *pthread_helper(void *arg)
{
    thread_helper_t *helper;

    helper = (thread_helper_t *)arg;
    helper->func(helper->arg);
}

int nftl_task_new(const char *name, nftl_fn_t func, void *arg, int stack_size)
{
    pthread_t tid;
    thread_helper_t *helper = (thread_helper_t *)malloc(sizeof(thread_helper_t));
    if (!helper) return -1;
    helper->func = func;
    helper->arg = arg;
    pthread_create(&tid, NULL, pthread_helper, helper);
    //pthread_join(tid, NULL);
}

void nftl_task_exit(int code)
{
    (void)code;
    /* TODO: free thread_helper_t memory */
    return;
}

#include <unistd.h>
void nftl_msleep(int ms)
{
    usleep(ms * 1000);
}

#ifdef MEASURE_TIME
#include <time.h>
long long nftl_now_ms(void)
{
    return (long long)time(NULL) * 1000;
}
#endif /* MEASURE_TIME */

#else /* NFTL_SIM_DEBUG */

void nftl_lock_create(nftl_lock_t *lock)
{
    //aos_mutex_new(lock);
}

void nftl_lock_destroy(nftl_lock_t *lock)
{
    //aos_mutex_free(lock);
}

void nftl_lock(nftl_lock_t *lock)
{
	lock->lock();
}

void nftl_unlock(nftl_lock_t *lock)
{
	lock->unlock();
}

int nftl_task_new(const char *name, void (*fn)(void *), void *arg, int stack_size)
{
	return 0;
    //return aos_task_new(name, fn, arg, stack_size);
}

void nftl_task_exit(int code)
{
    //aos_task_exit(code);
}

void nftl_msleep(int ms)
{
    //aos_msleep(ms);
}

#ifdef MEASURE_TIME
long long nftl_now_ms(void)
{
    return aos_now_ms();
}
#endif /* MEASURE_TIME */

#endif /* NFTL_SIM_DEBUG */
