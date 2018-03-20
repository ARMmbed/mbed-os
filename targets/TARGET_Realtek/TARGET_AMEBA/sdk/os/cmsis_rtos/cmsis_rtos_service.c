/* RTX includes */
#include "osdep_service.h"
#include "tcm_heap.h"
//#include <core_cmFunc.h>
//#include <stdlib.h>//malloc(), free()
//#include <string.h>//memcpy(), memcmp(), memset()
#include "platform_stdlib.h"
//#include <rt_HAL_CM.h>
//#include <RTX_CM_lib.h>
/********************* os depended utilities ********************/

#ifndef USE_MUTEX_FOR_SPINLOCK
#define USE_MUTEX_FOR_SPINLOCK 1
#endif

#define USE_HEAP_INFO	0

#define OS_TICK        OS_TICK_FREQ
#define OS_TICK_RATE_MS (1000/OS_TICK)

//-----------------------------------------------------------------------
// Private Variables
//-----------------------------------------------------------------------
static unsigned long CriticalNesting = 0;

//-----------------------------------------------------------------------
// Misc Function
//-----------------------------------------------------------------------
int osdep_print = 0;
#define _func_enter_ do{\
						if(osdep_print)\
							printf("enter %s\r\n", __FUNCTION__);\
					}while(0)
#define _func_exit_ do{\
						if(osdep_print)\
							printf("exit %s\r\n", __FUNCTION__);\
				     }while(0)

void save_and_cli()
{
_func_enter_;
#if defined(__CC_ARM)
	rtw_enter_critical(NULL, NULL);
#else
	__disable_irq();
#endif
_func_exit_;
}

void restore_flags()
{
_func_enter_;
#if defined(__CC_ARM)
	rtw_exit_critical(NULL, NULL);
#else
	__enable_irq();
#endif
_func_exit_;
}

void cli()
{
_func_enter_;
	__disable_irq();
_func_exit_;
}

/* Not needed on 64bit architectures */
static unsigned int __div64_32(u64 *n, unsigned int base)
{
	u64 rem = *n;
	u64 b = base;
	u64 res, d = 1;
	unsigned int high = rem >> 32;
_func_enter_;
	/* Reduce the thing a bit first */
	res = 0;
	if (high >= base) {
		high /= base;
		res = (u64) high << 32;
		rem -= (u64) (high * base) << 32;
	}

	while ((u64)b > 0 && b < rem) {
		b = b+b;
		d = d+d;
	}

	do {
		if (rem >= b) {
			rem -= b;
			res += d;
		}
		b >>= 1;
		d >>= 1;
	} while (d);
_func_exit_;
	*n = res;
	return rem;
}

/********************* os depended service ********************/
#if USE_HEAP_INFO
static uint32_t osFreeBytesRemaining=0x400;
#endif
static void _rtx2_memset(void *pbuf, int c, u32 sz);
u8* _rtx2_malloc(u32 sz)
{
_func_enter_;
	void *p = NULL;
	p = (void *)malloc(sz);
	if(p != NULL){
#if USE_HEAP_INFO
		osFreeBytesRemaining-=sz;
#endif
	}
_func_exit_;
	return p;
}

u8* _rtx2_zmalloc(u32 sz)
{
_func_enter_;
	u8 *pbuf = _rtx2_malloc(sz);

	if (pbuf != NULL){
#if USE_HEAP_INFO
		osFreeBytesRemaining-=sz;
#endif
		_rtx2_memset(pbuf, 0, sz);
	}
_func_exit_;
	return pbuf;	
}

static void (*ext_free)( void *p ) = NULL;
static uint32_t ext_upper = 0;
static uint32_t ext_lower = 0;
void rtw_set_mfree_ext( void (*free)( void *p ), uint32_t upper, uint32_t lower )
{
	ext_free = free;
	ext_upper = upper;
	ext_lower = lower;
}

void _rtx2_mfree(u8 *pbuf, u32 sz)
{
_func_enter_;
	if( ((uint32_t)pbuf >= ext_lower) && ((uint32_t)pbuf < ext_upper) ){
		if(ext_free)
			ext_free(pbuf);
	}else{
		free(pbuf);
	}
#if USE_HEAP_INFO
	osFreeBytesRemaining+=sz;
#endif
}

static void _rtx2_memcpy(void* dst, void* src, u32 sz)
{
_func_enter_;
	memcpy(dst, src, sz);
_func_exit_;
}

static int _rtx2_memcmp(void *dst, void *src, u32 sz)
{
_func_enter_;
//under Linux/GNU/GLibc, the return value of memcmp for two same mem. chunk is 0
	if (!(memcmp(dst, src, sz)))
		return _SUCCESS;
_func_exit_;
	return _FAIL;
}

static void _rtx2_memset(void *pbuf, int c, u32 sz)
{
_func_enter_;
	memset(pbuf, c, sz);
_func_exit_;
}

static void _rtx2_init_sema(_sema *sem, int init_val)
{
_func_enter_;
	rtx_sema_t *p_sem = (rtx_sema_t *)_rtx2_zmalloc(sizeof(rtx_sema_t));
	if(p_sem == NULL){
		goto err_exit;
	}
	*sem = (_sema)p_sem;
        _rtx2_memset(&p_sem->data, 0, sizeof(p_sem->data));
        p_sem->attr.cb_mem = &p_sem->data;
        p_sem->attr.cb_size = sizeof(p_sem->data);
        p_sem->id = osSemaphoreNew(osRtxSemaphoreTokenLimit, (uint32_t)init_val, &p_sem->attr);
        if (p_sem->id == NULL){
                goto err_exit;
        }
_func_exit_;
	return;
err_exit:
	DBG_ERR("error");
	if(p_sem)
		_rtx2_mfree((u8 *)p_sem, sizeof(rtx_sema_t));
	*sem = NULL;
	return;
}

static void _rtx2_free_sema(_sema *sema)
{
_func_enter_;
	if(*sema){
		rtx_sema_t *p_sem = (rtx_sema_t *)(*sema);
		osSemaphoreDelete(p_sem->id);
		if(p_sem)
			_rtx2_mfree((u8 *)p_sem, sizeof(rtx_sema_t));
		*sema = NULL;
	}else
		DBG_ERR("NULL pointer get");
_func_exit_;
}

static void _rtx2_up_sema(_sema *sema)
{
	if(*sema){
		rtx_sema_t *p_sem = (rtx_sema_t *)(*sema);
		osStatus_t status = osSemaphoreRelease(p_sem->id);
		if ( status != osOK){
			DBG_ERR("error %d", status);
		}
	}else
		DBG_ERR("NULL pointer get");
_func_exit_;
}

static void _rtx2_up_sema_from_isr(_sema *sema)
{
_func_enter_;
	if(*sema){
		rtx_sema_t *p_sem = (rtx_sema_t *)*sema;
		osStatus_t status = osSemaphoreRelease(p_sem->id);
		if (status != osOK){
			DBG_ERR("error %d", status);
		}
	}else
		DBG_ERR("NULL pointer get");
_func_exit_;
}

static u32 _rtx2_down_sema(_sema *sema, u32 timeout_ms)
{
	if(*sema){
		rtx_sema_t *p_sem = (rtx_sema_t *)*sema;
		if(timeout_ms == RTW_MAX_DELAY) {
			timeout_ms = osWaitForever;
		} else {
			timeout_ms = rtw_ms_to_systime(timeout_ms);
		}
		osStatus_t status = osSemaphoreAcquire(p_sem->id, timeout_ms);
		if (status == osOK){
			return _TRUE;
		};
	}
	return _FALSE;
}

static void _rtx2_mutex_init(_mutex *mutex)
{
_func_enter_;
	rtx_mutex_t *p_mut = (rtx_mutex_t *)_rtx2_zmalloc(sizeof(rtx_mutex_t));
	if(p_mut == NULL)
		goto err_exit;
        memset(&p_mut->data, 0, sizeof(p_mut->data));
        p_mut->attr.cb_mem = &p_mut->data;
        p_mut->attr.cb_size = sizeof(p_mut->data);
        p_mut->id = osMutexNew(&p_mut->attr);
        if (p_mut->id == NULL)
                goto err_exit;
	*mutex = (_mutex)p_mut;
_func_exit_;
	return;
err_exit:
	DBG_ERR("error");
	if(p_mut)
		_rtx2_mfree((u8 *)p_mut, sizeof(rtx_mutex_t));
	*mutex = NULL;
	return;
}

static void _rtx2_mutex_free(_mutex *pmutex)
{
_func_enter_;
	if(*pmutex){
		rtx_mutex_t *p_mut = (rtx_mutex_t *)(*pmutex);
		osMutexDelete(p_mut->id);
		if(p_mut)
			_rtx2_mfree((u8 *)p_mut, sizeof(rtx_mutex_t));
	}
_func_exit_;
}

static void _rtx2_mutex_get(_mutex *pmutex)
{
_func_enter_;
	if(*pmutex){
		rtx_mutex_t *p_mut = (rtx_mutex_t *)(*pmutex);
		if (osMutexAcquire(p_mut->id, 60 * 1000 / OS_TICK_RATE_MS) != osOK)
			DBG_ERR("%s(%p) failed, retry\n",  __FUNCTION__, p_mut);
	}
_func_exit_;
} 

static int _rtx2_mutex_get_timeout(_mutex *pmutex, u32 timeout_ms)
{
_func_enter_;
	if(*pmutex){
		rtx_mutex_t *p_mut = (rtx_mutex_t *)(*pmutex);
		if(timeout_ms == RTW_MAX_DELAY) {
			timeout_ms = osWaitForever;
		} else {
			timeout_ms = rtw_ms_to_systime(timeout_ms);
		}
		if(osMutexAcquire(p_mut->id, timeout_ms) == osOK){	
			return _SUCCESS;
		}
	}
_func_exit_;
	DBG_ERR("%s(%p) failed, retry\n",  __FUNCTION__, pmutex);
	return _FAIL;
}

static void _rtx2_mutex_put(_mutex *pmutex)
{
_func_enter_;
	if(*pmutex){
		rtx_mutex_t *p_mut = (rtx_mutex_t *)(*pmutex);
		if (osMutexRelease(p_mut->id) != osOK)
			DBG_ERR("\r\ninternal counter of mutex is 0 or calling task is not the owner of the mutex");
	}
_func_exit_;
}

static void _rtx2_enter_critical(_lock *plock, _irqL *pirqL)
{
_func_enter_;
	CriticalNesting++;
	if(CriticalNesting == 1){
		osKernelLock();//tsk_lock & tsk_unlock should not be called nested
	}
_func_exit_;
}

void mbed_die(void){
	DBG_ERR(" %p die here", osThreadGetId());
	__disable_irq();
	while(1);
}

static void _rtx2_exit_critical(_lock *plock, _irqL *pirqL)
{
_func_enter_;
	if(CriticalNesting == 0){
		DBG_ERR("die here");
		HALT();
	}
	CriticalNesting--;
	if(CriticalNesting == 0){
		osKernelUnlock();
	}
_func_exit_;
} 

static void _rtx2_enter_critical_from_isr(_lock *plock, _irqL *pirqL)
{
_func_enter_;
	__disable_irq();
_func_exit_;
}

static void _rtx2_exit_critical_from_isr(_lock *plock, _irqL *pirqL)
{
_func_enter_;
	__enable_irq();
_func_exit_;
}

static int _rtx2_enter_critical_mutex(_mutex *pmutex, _irqL *pirqL)
{
_func_enter_;
	while(_rtx2_mutex_get_timeout(pmutex, 60 * 1000) != _SUCCESS)
		DBG_ERR("\n\r[%p] %s(%p) failed, retry\n", osThreadGetId(), __FUNCTION__, pmutex);
_func_exit_;
	return _SUCCESS;
}

static void _rtx2_exit_critical_mutex(_mutex *pmutex, _irqL *pirqL)
{
_func_enter_;
	_rtx2_mutex_put(pmutex);
_func_exit_;
}

static void _rtx2_spinlock_init(_lock *plock)
{
_func_enter_;
#if USE_MUTEX_FOR_SPINLOCK
	_rtx2_mutex_init(plock);
#endif
_func_exit_;
}

static void _rtx2_spinlock_free(_lock *plock)
{
_func_enter_;
#if USE_MUTEX_FOR_SPINLOCK
	if(plock != NULL){
		_rtx2_mutex_free(plock);
	}
#endif
_func_exit_;
}

static void _rtx2_spinlock(_lock *plock)
{
_func_enter_;
#if USE_MUTEX_FOR_SPINLOCK
	_rtx2_mutex_get(plock);
#endif
_func_exit_;
}

static void _rtx2_spinunlock(_lock *plock)
{
_func_enter_;
#if USE_MUTEX_FOR_SPINLOCK
	_rtx2_mutex_put(plock);
#endif
_func_exit_;
}

static void _rtx2_spinlock_irqsave(_lock *plock, _irqL *irqL)
{
_func_enter_;
	_rtx2_enter_critical(plock, irqL);
#if USE_MUTEX_FOR_SPINLOCK
	_rtx2_spinlock(plock);
#endif
_func_exit_;
}

static void _rtx2_spinunlock_irqsave(_lock *plock, _irqL *irqL)
{
_func_enter_;
#if USE_MUTEX_FOR_SPINLOCK
	_rtx2_spinunlock(plock);
#endif
	_rtx2_exit_critical(plock, irqL);
_func_exit_;
}

static int _rtx2_init_xqueue( _xqueue* queue, const char* name, u32 message_size, u32 number_of_messages )
{
_func_enter_;
	rtx_mbox_t *mbox = (rtx_mbox_t *)_rtx2_zmalloc(sizeof(rtx_mbox_t));
	if (mbox == NULL ){
		goto err_exit;
	}
	mbox->queue_mem = _rtx2_zmalloc(number_of_messages * (message_size + sizeof(os_message_t)));
	if(mbox->queue_mem == NULL)
		goto err_exit;
        mbox->attr.mq_mem = mbox->queue_mem;
        mbox->attr.mq_size = number_of_messages * (message_size + sizeof(os_message_t));
        mbox->attr.cb_mem = &mbox->data;
        mbox->attr.cb_size = sizeof(mbox->data);
	*queue = (_xqueue)mbox;
    	mbox->id = osMessageQueueNew(number_of_messages, message_size, &mbox->attr);
	if(mbox->id == NULL)
		goto err_exit;

_func_exit_;
	return _SUCCESS;
err_exit:
	DBG_ERR("%s error\r\n", __FUNCTION__);
	if(mbox){
		if(mbox->queue_mem)
			_rtx2_mfree(mbox->queue_mem, number_of_messages * (message_size + sizeof(os_message_t)));
		_rtx2_mfree((u8 *)mbox, sizeof(rtx_mbox_t));
		*queue = NULL;
	}
	return _FAIL;
}

static int _rtx2_push_to_xqueue( _xqueue* queue, void* message, u32 timeout_ms )
{
_func_enter_;
	rtx_mbox_t *mbox;
	if(timeout_ms == RTW_MAX_DELAY) {
	      timeout_ms = osWaitForever;
	} else {
	      timeout_ms = rtw_ms_to_systime(timeout_ms);
	}
	
	if (*queue != NULL){
		mbox = (rtx_mbox_t *)(*queue);
		if(osMessageQueuePut(mbox->id, message, NULL, timeout_ms) != osOK ){
			DBG_ERR("%s error\n", __FUNCTION__);
			return _FAIL;
		}
    	}
_func_exit_;
	return _SUCCESS;
}

static int _rtx2_pop_from_xqueue( _xqueue* queue, void* message, u32 timeout_ms )
{
_func_enter_;
	if(timeout_ms == RTW_WAIT_FOREVER) {
		timeout_ms = osWaitForever;
	} else {
		timeout_ms = rtw_ms_to_systime(timeout_ms);
	}
	if (*queue != NULL){
		rtx_mbox_t *mbox = (rtx_mbox_t *)(*queue);
		osStatus_t res = osMessageQueueGet(mbox->id, message, NULL, timeout_ms);
		if (res == osOK) {
_func_exit_;
			return _SUCCESS;
		}
	}

	DBG_ERR("[%p] %s error", osThreadGetId(), __FUNCTION__);
	return _FAIL;
}

static int _rtx2_deinit_xqueue( _xqueue* queue )
{
_func_enter_;
	if(*queue != NULL){
		rtx_mbox_t *mbox = (rtx_mbox_t *)(*queue);
		if(mbox->queue_mem)
			_rtx2_mfree(mbox->queue_mem, mbox->attr.mq_size);
		_rtx2_mfree((u8 *)mbox, sizeof(rtx_mbox_t));
		*queue = NULL;
	}
_func_exit_;
    return 0;
}

static u32 _rtx2_get_current_time(void)
{
	return osKernelGetSysTimerCount();
}

static u32 _rtx2_systime_to_ms(u32 systime)
{
	return systime * OS_TICK_RATE_MS;
}

static u32 _rtx2_systime_to_sec(u32 systime)
{
	return systime / OS_TICK;
}

static u32 _rtx2_ms_to_systime(u32 ms)
{
	return ms / OS_TICK_RATE_MS;
}

static u32 _rtx2_sec_to_systime(u32 sec)
{
	return sec * OS_TICK;
}

static void _rtx2_msleep_os(int ms)
{
_func_enter_;
	osDelay(_rtx2_ms_to_systime(ms));
_func_exit_;
}

static void _rtx2_usleep_os(int us)
{
_func_enter_;
#if defined(STM32F2XX) || defined(STM32F4XX) || defined(STM32F10X_XL)
	// FreeRTOS does not provide us level delay. Use busy wait
	WLAN_BSP_UsLoop(us);
#elif defined(CONFIG_PLATFORM_8195A) || defined(CONFIG_PLATFORM_8711B)
	//DBG_ERR("%s: Please Implement micro-second delay\n", __FUNCTION__);
	HalDelayUs(us);
#else
//	#error "Please implement hardware dependent micro second level sleep here"
#endif
_func_exit_;
}

static void _rtx2_mdelay_os(int ms)
{
_func_enter_;
	osDelay(_rtx2_ms_to_systime(ms));
_func_exit_;
}

static void _rtx2_udelay_os(int us)
{
_func_enter_;
#if defined(STM32F2XX)	|| defined(STM32F4XX) || defined(STM32F10X_XL)
	// FreeRTOS does not provide us level delay. Use busy wait
	WLAN_BSP_UsLoop(us);
#elif defined(CONFIG_PLATFORM_8195A) || defined(CONFIG_PLATFORM_8711B)
	//RtlUdelayOS(us);
	HalDelayUs(us);
#else
//	#error "Please implement hardware dependent micro second level sleep here"
#endif
_func_exit_;
}

static void _rtx2_yield_os(void)
{
_func_enter_;
	osThreadYield();
_func_exit_;
}

static void _rtx2_ATOMIC_SET(ATOMIC_T *v, int i)
{
	atomic_set(v,i);
}

static int _rtx2_ATOMIC_READ(ATOMIC_T *v)
{
	return atomic_read(v);
}

static void _rtx2_ATOMIC_ADD(ATOMIC_T *v, int i)
{
	save_and_cli();
	v->counter += i;
	restore_flags();
}

static void _rtx2_ATOMIC_SUB(ATOMIC_T *v, int i)
{
	save_and_cli();
	v->counter -= i;
	restore_flags();
}

static void _rtx2_ATOMIC_INC(ATOMIC_T *v)
{
	save_and_cli();
	v->counter++;
	restore_flags();
}

static void _rtx2_ATOMIC_DEC(ATOMIC_T *v)
{
	save_and_cli();
	v->counter--;
	restore_flags();
}

static int _rtx2_ATOMIC_ADD_RETURN(ATOMIC_T *v, int i)
{
	int temp;

	save_and_cli();
	temp = v->counter;
	temp += i;
	v->counter = temp;
	restore_flags();

	return temp;
}

static int _rtx2_ATOMIC_SUB_RETURN(ATOMIC_T *v, int i)
{
	int temp;

	save_and_cli();
	temp = v->counter;
	temp -= i;
	v->counter = temp;
	restore_flags();

	return temp;
}

static int _rtx2_ATOMIC_INC_RETURN(ATOMIC_T *v)
{
	return _rtx2_ATOMIC_ADD_RETURN(v, 1);
}

static int _rtx2_ATOMIC_DEC_RETURN(ATOMIC_T *v)
{
	return _rtx2_ATOMIC_SUB_RETURN(v, 1);
}

static u64 _rtx2_modular64(u64 n, u64 base)
{
	unsigned int __base = (base);
	unsigned int __rem;
_func_enter_;
	if (((n) >> 32) == 0) {
		__rem = (unsigned int)(n) % __base;
		(n) = (unsigned int)(n) / __base;
	}
	else
		__rem = __div64_32(&(n), __base);
_func_exit_;
	return __rem;
}

/* Refer to ecos bsd tcpip codes */
static int _rtx2_arc4random(void)
{
_func_enter_;
	u32 res = _rtx2_get_current_time();
	static unsigned long seed = 0xDEADB00B;
	seed = ((seed & 0x007F00FF) << 7) ^
	    ((seed & 0x0F80FF00) >> 8) ^ // be sure to stir those low bits
	    (res << 13) ^ (res >> 9);    // using the clock too!
_func_exit_;
	return (int)seed;
}

static int _rtx2_get_random_bytes(void *buf, u32 len)
{
#if 1 //becuase of 4-byte align, we use the follow code style.
	unsigned int ranbuf;
	unsigned int *lp;
	int i, count;
	count = len / sizeof(unsigned int);
	lp = (unsigned int *) buf;
_func_enter_;
	for(i = 0; i < count; i ++) {
		lp[i] = _rtx2_arc4random();  
		len -= sizeof(unsigned int);
	}

	if(len > 0) {
		ranbuf = _rtx2_arc4random();
		_rtx2_memcpy(&lp[i], &ranbuf, len);
	}
_func_exit_;
	return 0;
#else
	unsigned long ranbuf, *lp;
	lp = (unsigned long *)buf;
	while (len > 0) {
		ranbuf = _rtx2_arc4random();
		*lp++ = ranbuf; //this op need the pointer is 4Byte-align!
		len -= sizeof(ranbuf);
	}
	return 0;
#endif
}

static u32 _rtx2_GetFreeHeapSize(void)
{
#if USE_HEAP_INFO
	return osFreeBytesRemaining;
#else
	return 0;
#endif
}


#if CONFIG_USE_TCM_HEAP
void *tcm_heap_malloc(int size);
#endif
static int _rtx2_create_task(struct task_struct *ptask, const char *name,
	u32  stack_size, u32 priority, thread_func_t func, void *thctx)
{
_func_enter_;
	rtx_thread_data_t *thread_hdl = NULL;
	u32 stacksize = stack_size * 4; //sizeof(DWORD)
	if(!func)
		goto err_exit;
	thread_hdl = (rtx_thread_data_t *)_rtx2_zmalloc(sizeof(rtx_thread_data_t));
	if(thread_hdl == NULL)
		goto err_exit;
	if(priority > osPriorityRealtime){
		DBG_ERR("[%s]priority is higher than osPriorityRealtime", name);
		priority = osPriorityRealtime;
	}
	thread_hdl->attr.name = name;
	thread_hdl->attr.priority = (osPriority_t)priority;
	thread_hdl->attr.cb_size = sizeof(thread_hdl->data);
	thread_hdl->attr.cb_mem = &thread_hdl->data;
	thread_hdl->attr.stack_size = stacksize;
	thread_hdl->attr.stack_mem = (void *)_rtx2_malloc(stacksize); 
	if (thread_hdl->attr.stack_mem == NULL)
		goto err_exit;

	ptask->task = (_thread_hdl_)thread_hdl;
	ptask->task_name = name;
	ptask->blocked = 0;
	ptask->callback_running = 0;

	_rtx2_init_sema(&ptask->wakeup_sema, 0);
	_rtx2_init_sema(&ptask->terminate_sema, 0);
	//rtw_init_queue(&wq->work_queue);

	thread_hdl->id = osThreadNew((osThreadFunc_t)func, thctx, &thread_hdl->attr);
	if(thread_hdl->id == NULL)
		goto err_exit;
	return _SUCCESS;
err_exit:
	if(thread_hdl){
		_rtx2_free_sema(&ptask->wakeup_sema);
		_rtx2_free_sema(&ptask->terminate_sema);
		_rtx2_memset((u8 *)ptask, 0, sizeof(*ptask));
		if(thread_hdl->attr.stack_mem)
			_rtx2_mfree((void *)thread_hdl->attr.stack_mem, thread_hdl->attr.stack_size);
		_rtx2_mfree((u8 *)thread_hdl, sizeof(rtx_thread_data_t));
	}
	DBG_ERR("Create Task \"%s\" Failed! \n", ptask->task_name);
	return _FAIL;
}

static void _rtx2_delete_task(struct task_struct *ptask)
{
_func_enter_;
	rtx_thread_data_t *thread_hdl = (rtx_thread_data_t *)ptask->task;
	if (!thread_hdl){
		DBG_ERR("_rtx2_delete_task(): ptask is NULL!\n");
		return;
	}

	ptask->blocked = 1;

	_rtx2_up_sema(&ptask->wakeup_sema);
	_rtx2_down_sema(&ptask->terminate_sema, TIMER_MAX_DELAY);

	osThreadTerminate(thread_hdl->id);
	if(thread_hdl->attr.stack_mem)
		_rtx2_mfree((void *)thread_hdl->attr.stack_mem, thread_hdl->attr.stack_size);
	_rtx2_mfree((u8 *)thread_hdl, sizeof(rtx_thread_data_t));
	
	//rtw_deinit_queue(&wq->work_queue);
	_rtx2_free_sema(&ptask->wakeup_sema);
	_rtx2_free_sema(&ptask->terminate_sema);

	ptask->task = NULL;

	DBG_TRACE("Delete Task \"%s\"\n", ptask->task_name);
_func_exit_;
}

void _rtx2_wakeup_task(struct task_struct *ptask)
{
_func_enter_;
	if(ptask)
		_rtx2_up_sema(&ptask->wakeup_sema);
_func_exit_;
}

static void _rtx2_thread_enter(char *name)
{
_func_enter_;
	DBG_INFO("\n\rRTKTHREAD %s\n", name);
_func_exit_;
}

static void _rtx2_thread_exit(void)
{
_func_enter_;
	osThreadExit();
_func_exit_;
}

/*****************************************************
************timer data block, defined in rt_CMSIS.c*********

// Timer definitions
#define osTimerInvalid  0
#define osTimerStopped  1
#define osTimerRunning  2

// Timer structures

typedef struct os_timer_cb_ {                   // Timer Control Block
  struct os_timer_cb_ *next;                    // Pointer to next active Timer, (u8 *)data[0:3]
  uint8_t             state;                    // Timer State, (u8 *)data[4]
  uint8_t              type;                    // Timer Type (Periodic/One-shot), (u8 *)data[5]
  uint16_t         reserved;                    // Reserved, (u8 *)data[6:7]
  uint16_t             tcnt;                    // Timer Delay Count, (u8 *)data[8:9]
  uint16_t             icnt;                    // Timer Initial Count, (u8 *)data[10:11]
  void                 *arg;                    // Timer Function Argument, (u8 *)data[12:15]
  osTimerDef_t       *timer;                    // Pointer to Timer definition, (u8 *)data[16:19]
} os_timer_cb;
*****************************************************/
_timerHandle _rtx2_timerCreate( const signed char *pcTimerName, 
							  osdepTickType xTimerPeriodInTicks, 
							  u32 uxAutoReload, 
							  void * pvTimerID, 
							  TIMER_FUN pxCallbackFunction )
{
_func_enter_;
	rtx_tmr_t *tmr = (rtx_tmr_t *)_rtx2_zmalloc(sizeof(rtx_tmr_t));
	osTimerType_t type = (uxAutoReload == _TRUE)?osTimerPeriodic:osTimerOnce;
	if(tmr == NULL)
		goto err_exit;

	tmr->attr.name = pcTimerName;
	tmr->attr.cb_mem = (void *)&tmr->data;
	tmr->attr.cb_size = sizeof(tmr->data);
	if(pvTimerID == NULL)
		pvTimerID = (void *)tmr;
	tmr->id =  osTimerNew(pxCallbackFunction, type, pvTimerID, &tmr->attr);
	if(tmr->id == NULL)
		goto err_exit;

_func_exit_;
	return (_timerHandle)tmr;
err_exit:
	DBG_ERR("error");
	if(tmr)
		_rtx2_mfree((u8 *)tmr, sizeof(rtx_tmr_t));
	return NULL;
}

u32 _rtx2_timerDelete( _timerHandle xTimer, 
							   osdepTickType xBlockTime )
{
_func_enter_;
	rtx_tmr_t *tmr = (rtx_tmr_t *) xTimer;
	osStatus_t status = osTimerDelete(tmr->id);
	_rtx2_mfree((u8 *)tmr, sizeof(rtx_tmr_t));
	if(status != osOK){
		DBG_ERR("error %d", status);
		return _FAIL;
	}
_func_exit_;
	return _SUCCESS;
}

u32 _rtx2_timerIsTimerActive( _timerHandle xTimer )
{
_func_enter_;
	rtx_tmr_t *tmr = (rtx_tmr_t *) xTimer;
	if(osTimerIsRunning(tmr->id))
		return _TRUE;
	return _FALSE;	
}

u32  _rtx2_timerStop( _timerHandle xTimer, 
							   osdepTickType xBlockTime )
{
_func_enter_;
	rtx_tmr_t *tmr = (rtx_tmr_t *) xTimer;
	if(_rtx2_timerIsTimerActive(xTimer) == _TRUE){
		osStatus_t status = osTimerStop(tmr->id);
_func_exit_;
		if(status != osOK){
			DBG_ERR("error %d\n", status);
			return _FAIL;
		}
	}
	return _SUCCESS;
}

u32  _rtx2_timerChangePeriod( _timerHandle xTimer, 
							   osdepTickType xNewPeriod, 
							   osdepTickType xBlockTime )
{
_func_enter_;
	rtx_tmr_t *tmr = (rtx_tmr_t *) xTimer;
	osStatus_t ret;

	if(xNewPeriod == 0)
		xNewPeriod += 1;
	//xNewPeriod = _rtx2_systime_to_ms(xNewPeriod);
	ret = osTimerStart(tmr->id, xNewPeriod);
_func_exit_;
	if(ret == osOK)
		return _SUCCESS;
	
	DBG_ERR("%s error\n", __FUNCTION__);
	return _FAIL;
}

//void _rtx2_acquire_wakelock()
//{
//#if defined(configUSE_WAKELOCK_PMU) && (configUSE_WAKELOCK_PMU == 1)
//    acquire_wakelock(WAKELOCK_WLAN);
//#endif
//}

//void _rtx2_release_wakelock()
//{
//#if defined(configUSE_WAKELOCK_PMU) && (configUSE_WAKELOCK_PMU == 1)
//    release_wakelock(WAKELOCK_WLAN);
//#endif
//}

u8 _rtx2_get_scheduler_state(void)
{
_func_enter_;
	osKernelState_t state = osKernelGetState();
	u8 state_out = OS_SCHEDULER_NOT_STARTED;
	switch(state){
		case osKernelRunning:		state = OS_SCHEDULER_RUNNING;		break;
		case osKernelSuspended:		state = OS_SCHEDULER_SUSPENDED;		break;
		default:	break;
	}
_func_exit_;
	return state_out;
}

const struct osdep_service_ops osdep_service = {
	_rtx2_malloc,		//rtw_vmalloc
	_rtx2_zmalloc,		//rtw_zvmalloc
	_rtx2_mfree,			//rtw_vmfree
	_rtx2_malloc, //rtw_malloc
	_rtx2_zmalloc, //rtw_zmalloc
	_rtx2_mfree, //rtw_mfree
	_rtx2_memcpy, //rtw_memcpy
	_rtx2_memcmp, //rtw_memcmp
	_rtx2_memset, //rtw_memset
	_rtx2_init_sema, //rtw_init_sema
	_rtx2_free_sema, //rtw_free_sema
	_rtx2_up_sema, //rtw_up_sema
	_rtx2_up_sema_from_isr,//rtw_up_sema_from_isr
	_rtx2_down_sema, //rtw_down_sema
	_rtx2_mutex_init, //rtw_mutex_init
	_rtx2_mutex_free, //rtw_mutex_free
	_rtx2_mutex_get, //rtw_mutex_get
	_rtx2_mutex_get_timeout, //rtw_mutex_get_timeout
	_rtx2_mutex_put, //rtw_mutex_put
	_rtx2_enter_critical,		//rtw_enter_critical
	_rtx2_exit_critical,		//rtw_exit_critical
	_rtx2_enter_critical_from_isr,		//rtw_enter_critical_from_isr
	_rtx2_exit_critical_from_isr,		//rtw_exit_critical_from_isr
	NULL,		//rtw_enter_critical_bh
	NULL,		//rtw_exit_critical_bh
	_rtx2_enter_critical_mutex,		//rtw_enter_critical_mutex
	_rtx2_exit_critical_mutex,		//rtw_exit_critical_mutex
	_rtx2_spinlock_init, //rtw_spinlock_init
	_rtx2_spinlock_free, //rtw_spinlock_free
	_rtx2_spinlock, //rtw_spin_lock
	_rtx2_spinunlock, //rtw_spin_unlock
	_rtx2_spinlock_irqsave,		//rtw_spinlock_irqsave
	_rtx2_spinunlock_irqsave,	//rtw_spinunlock_irqsave
	_rtx2_init_xqueue,//rtw_init_xqueue
	_rtx2_push_to_xqueue,//rtw_push_to_xqueue
	_rtx2_pop_from_xqueue,//rtw_pop_from_xqueue
	_rtx2_deinit_xqueue,//rtw_deinit_xqueue
	_rtx2_get_current_time, //rtw_get_current_time
	_rtx2_systime_to_ms, //rtw_systime_to_ms
	_rtx2_systime_to_sec, //rtw_systime_to_sec
	_rtx2_ms_to_systime, //rtw_ms_to_systime
	_rtx2_sec_to_systime, //rtw_sec_to_systime
	_rtx2_msleep_os, //rtw_msleep_os
	_rtx2_usleep_os, //rtw_usleep_os
	_rtx2_mdelay_os, //rtw_mdelay_os
	_rtx2_udelay_os, //rtw_udelay_os
	_rtx2_yield_os, //rtw_yield_os
	
	_rtx2_ATOMIC_SET, //ATOMIC_SET
	_rtx2_ATOMIC_READ, //ATOMIC_READ
	_rtx2_ATOMIC_ADD, //ATOMIC_ADD
	_rtx2_ATOMIC_SUB, //ATOMIC_SUB
	_rtx2_ATOMIC_INC, //ATOMIC_INC
	_rtx2_ATOMIC_DEC, //ATOMIC_DEC
	_rtx2_ATOMIC_ADD_RETURN, //ATOMIC_ADD_RETURN
	_rtx2_ATOMIC_SUB_RETURN, //ATOMIC_SUB_RETURN
	_rtx2_ATOMIC_INC_RETURN, //ATOMIC_INC_RETURN
	_rtx2_ATOMIC_DEC_RETURN, //ATOMIC_DEC_RETURN

	_rtx2_modular64, 		//rtw_modular64
	_rtx2_get_random_bytes,	//rtw_get_random_bytes
	_rtx2_GetFreeHeapSize,		//rtw_getFreeHeapSize

	_rtx2_create_task,		//rtw_create_task
	_rtx2_delete_task,		//rtw_delete_task
	_rtx2_wakeup_task,		//rtw_wakeup_task

	_rtx2_thread_enter,		//rtw_thread_enter
	_rtx2_thread_exit,		//rtw_thread_exit

	_rtx2_timerCreate,           //rtw_timerCreate,       
	_rtx2_timerDelete,           //rtw_timerDelete,       
	_rtx2_timerIsTimerActive,    //rtw_timerIsTimerActive,
	_rtx2_timerStop,             //rtw_timerStop,         
	_rtx2_timerChangePeriod,      //rtw_timerChangePeriod  

	NULL,  // rtw_acquire_wakelock
	NULL,  // rtw_release_wakelock
	NULL,  //rtw_wakelock_timeout

	_rtx2_get_scheduler_state  // rtw_get_scheduler_state
};

