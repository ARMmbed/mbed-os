/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *                                        
 ******************************************************************************/

#include <osdep_service.h>
#define OSDEP_DBG(x, ...) do {} while(0)

extern struct osdep_service_ops osdep_service;

#ifdef CONFIG_LITTLE_ENDIAN
u16
_htons(u16 n)
{
  return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

u16
_ntohs(u16 n)
{
  return _htons(n);
}

u32
_htonl(u32 n)
{
  return ((n & 0xff) << 24) |
    ((n & 0xff00) << 8) |
    ((n & 0xff0000UL) >> 8) |
    ((n & 0xff000000UL) >> 24);
}

u32
_ntohl(u32 n)
{
  return _htonl(n);
}

#endif /* CONFIG_LITTLE_ENDIAN */
/*
* Translate the OS dependent @param error_code to OS independent RTW_STATUS_CODE
* @return: one of RTW_STATUS_CODE
*/
int RTW_STATUS_CODE(int error_code)
{
	if(error_code >= 0)
		return _SUCCESS;

	return _FAIL;
}

u32 rtw_atoi(u8* s)
{
	int num=0,flag=0;
	int i;

	for(i=0;i<=strlen((char *)s);i++)
	{
		if(s[i] >= '0' && s[i] <= '9')
			num = num * 10 + s[i] -'0';
		else if(s[0] == '-' && i==0) 
			flag =1;
		else 
			break;
	}

	if(flag == 1)
		num = num * -1;

	return(num); 
}
void *tcm_heap_malloc(int size);
void *tcm_heap_calloc(int size);
u8* _rtw_vmalloc(u32 sz)
{
	u8 *pbuf = NULL;	
#if CONFIG_USE_TCM_HEAP
	pbuf = tcm_heap_malloc(sz);
#endif
	if(pbuf==NULL){
		if(osdep_service.rtw_vmalloc) {
			pbuf = osdep_service.rtw_vmalloc(sz);
		} else
			OSDEP_DBG("Not implement osdep service: rtw_vmalloc");	
	}
	return pbuf;
}

u8* _rtw_zvmalloc(u32 sz)
{
	u8 *pbuf = NULL;	
#if CONFIG_USE_TCM_HEAP
	pbuf = tcm_heap_calloc(sz);
#endif
	if(pbuf==NULL){
		if(osdep_service.rtw_zvmalloc) {
			pbuf = osdep_service.rtw_zvmalloc(sz);
		} else
			OSDEP_DBG("Not implement osdep service: rtw_zvmalloc");	
	}
	return pbuf;
}

void _rtw_vmfree(u8 *pbuf, u32 sz)
{
	
#if CONFIG_USE_TCM_HEAP
	if( (u32)pbuf > 0x1FFF0000 && (u32)pbuf < 0x20000000 )
		tcm_heap_free(pbuf);
	else
#endif
	{
	if(osdep_service.rtw_vmfree) {
		osdep_service.rtw_vmfree(pbuf, sz);
	} else
		OSDEP_DBG("Not implement osdep service: rtw_vmfree");	
	}
}

u8* _rtw_malloc(u32 sz)
{
	if(osdep_service.rtw_malloc) {
		u8 *pbuf = osdep_service.rtw_malloc(sz);
		return pbuf;
	} else
		OSDEP_DBG("Not implement osdep service: rtw_malloc");	

	return NULL;
}

u8* _rtw_zmalloc(u32 sz)
{
	if(osdep_service.rtw_zmalloc) {
		u8 *pbuf = osdep_service.rtw_zmalloc(sz);
		return pbuf;
	} else
		OSDEP_DBG("Not implement osdep service: rtw_zmalloc");	

	return NULL;
}

void _rtw_mfree(u8 *pbuf, u32 sz)
{
	if(osdep_service.rtw_mfree) {
		osdep_service.rtw_mfree(pbuf, sz);
	} else
		OSDEP_DBG("Not implement osdep service: rtw_mfree");
}

#ifdef CONFIG_MEM_MONITOR
#if CONFIG_MEM_MONITOR & MEM_MONITOR_LEAK
_list mem_table;
int mem_used_num;
#endif
int min_free_heap_size;

void init_mem_monitor(_list *pmem_table, int *used_num)
{
#if CONFIG_MEM_MONITOR & MEM_MONITOR_LEAK
	rtw_init_listhead(pmem_table);
	*used_num = 0;
#endif
	min_free_heap_size = rtw_getFreeHeapSize();
}

void deinit_mem_monitor(_list *pmem_table, int *used_num)
{
#if CONFIG_MEM_MONITOR & MEM_MONITOR_LEAK
	_list *plist;
	struct mem_entry *mem_entry;
	
	if(*used_num > 0)
		DBG_ERR("Have %d mem_entry kept in monitor", *used_num);
	else
		DBG_INFO("No mem_entry kept in monitor");
	
	save_and_cli();
	
	while (rtw_end_of_queue_search(pmem_table, get_next(pmem_table)) == _FALSE)	{		
		plist = get_next(pmem_table);
		mem_entry = LIST_CONTAINOR(plist, struct mem_entry, list);
		
		DBG_ERR("Not release memory at %p with size of %d", mem_entry->ptr, mem_entry->size);
		
		rtw_list_delete(plist);
		_rtw_mfree((u8 *) mem_entry, sizeof(struct mem_entry));
	}
	
	restore_flags();
#endif
}

void add_mem_usage(_list *pmem_table, void *ptr, int size, int *used_num, int flag)
{
	int free_heap_size = rtw_getFreeHeapSize();
#if CONFIG_MEM_MONITOR & MEM_MONITOR_LEAK
	struct mem_entry *mem_entry;
#endif
	if(ptr == NULL) {
		DBG_ERR("Catch a mem alloc fail with size of %d, current heap free size = %d", size, free_heap_size);
		return;
	}
	else{
		if(flag == MEM_MONITOR_FLAG_WPAS)
			DBG_INFO("Alloc memory at %p with size of %d", ptr, size);
		else
			DBG_INFO("Alloc memory at %p with size of %d", ptr, size);
	}
#if CONFIG_MEM_MONITOR & MEM_MONITOR_LEAK
	mem_entry = (struct mem_entry *) _rtw_malloc(sizeof(struct mem_entry));

	if(mem_entry == NULL) {
		DBG_ERR("Fail to alloc mem_entry");
		return;
	}

	memset(mem_entry, 0, sizeof(struct mem_entry));
	mem_entry->ptr = ptr;
	mem_entry->size = size;
	
	save_and_cli();
	rtw_list_insert_head(&mem_entry->list, pmem_table);
	restore_flags();

	*used_num ++;
#endif
	if(min_free_heap_size > free_heap_size)
		min_free_heap_size = free_heap_size;
}

void del_mem_usage(_list *pmem_table, void *ptr, int *used_num, int flag)
{
#if CONFIG_MEM_MONITOR & MEM_MONITOR_LEAK
	_list *plist;
	struct mem_entry *mem_entry = NULL;

	if(ptr == NULL)
		return;
	
	if(flag == MEM_MONITOR_FLAG_WPAS)
		DBG_INFO("Free memory at %p", ptr);
	else
		DBG_INFO("Free memory at %p", ptr);	

	save_and_cli();

	plist = get_next(pmem_table);
	while ((rtw_end_of_queue_search(pmem_table, plist)) == _FALSE)	
	{		
		mem_entry = LIST_CONTAINOR(plist, struct mem_entry, list);
		if(mem_entry->ptr == ptr) {
			rtw_list_delete(plist);
			break;
		}
		plist = get_next(plist);
	}

	restore_flags();

	if(plist == pmem_table)
		DBG_ERR("Fail to find the mem_entry in mem table");
	else {
		*used_num --;
		_rtw_mfree((u8 *) mem_entry, sizeof(struct mem_entry));
	}
#endif
}

#if CONFIG_MEM_MONITOR & MEM_MONITOR_LEAK
int get_mem_usage(_list *pmem_table)
{
	_list *plist;
	struct mem_entry *mem_entry;
	int mem_usage = 0;
	int entry_num = 0;

	save_and_cli();

	if((plist = get_next(pmem_table)) == NULL) {
		DBG_ERR("No mem table available\n");
		restore_flags();
		return 0;
	}

	while (rtw_end_of_queue_search(pmem_table, plist) == _FALSE) {
		entry_num ++;
		mem_entry = LIST_CONTAINOR(plist, struct mem_entry, list);
		mem_usage += mem_entry->size;

		DBG_INFO("size of mem_entry(%d)=%d\n", entry_num, mem_entry->size);
		plist = get_next(plist);
	}

	restore_flags();

	DBG_INFO("Get %d mem_entry\n", entry_num);

	return mem_usage;
}
#endif


u8* rtw_vmalloc(u32 sz)
{
	u8 *pbuf = _rtw_vmalloc(sz);
#if CONFIG_MEM_MONITOR & MEM_MONITOR_LEAK
	add_mem_usage(&mem_table, pbuf, sz, &mem_used_num, MEM_MONITOR_FLAG_WIFI_DRV);
#else
	add_mem_usage(NULL, pbuf, sz, NULL, MEM_MONITOR_FLAG_WIFI_DRV);
#endif
	return pbuf;
}

u8* rtw_zvmalloc(u32 sz)
{
	u8 *pbuf = _rtw_zvmalloc(sz);
#if CONFIG_MEM_MONITOR & MEM_MONITOR_LEAK
	add_mem_usage(&mem_table, pbuf, sz, &mem_used_num, MEM_MONITOR_FLAG_WIFI_DRV);
#else
	add_mem_usage(NULL, pbuf, sz, NULL, MEM_MONITOR_FLAG_WIFI_DRV);
#endif
	return pbuf;
}

void rtw_vmfree(u8 *pbuf, u32 sz)
{
	_rtw_vmfree(pbuf, sz);
#if CONFIG_MEM_MONITOR & MEM_MONITOR_LEAK
	del_mem_usage(&mem_table, pbuf, &mem_used_num, MEM_MONITOR_FLAG_WIFI_DRV);	
#else
	del_mem_usage(NULL, pbuf, NULL, MEM_MONITOR_FLAG_WIFI_DRV);
#endif
}

u8* rtw_malloc(u32 sz)
{
	u8 *pbuf = _rtw_malloc(sz);
#if CONFIG_MEM_MONITOR & MEM_MONITOR_LEAK
	add_mem_usage(&mem_table, pbuf, sz, &mem_used_num, MEM_MONITOR_FLAG_WIFI_DRV);
#else
	add_mem_usage(NULL, pbuf, sz, NULL, MEM_MONITOR_FLAG_WIFI_DRV);
#endif
	return pbuf;
}

u8* rtw_zmalloc(u32 sz)
{
	u8 *pbuf = _rtw_zmalloc(sz);
#if CONFIG_MEM_MONITOR & MEM_MONITOR_LEAK
	add_mem_usage(&mem_table, pbuf, sz, &mem_used_num, MEM_MONITOR_FLAG_WIFI_DRV);
#else
	add_mem_usage(NULL, pbuf, sz, NULL, MEM_MONITOR_FLAG_WIFI_DRV);
#endif
	return pbuf;
}

void rtw_mfree(u8 *pbuf, u32 sz)
{
	_rtw_mfree(pbuf, sz);
#if CONFIG_MEM_MONITOR & MEM_MONITOR_LEAK
	del_mem_usage(&mem_table, pbuf, &mem_used_num, MEM_MONITOR_FLAG_WIFI_DRV);	
#else
	del_mem_usage(NULL, pbuf, NULL, MEM_MONITOR_FLAG_WIFI_DRV);
#endif
}
#endif

void* rtw_malloc2d(int h, int w, int size)
{
	int j;

	void **a = (void **) rtw_zmalloc( h*sizeof(void *) + h*w*size );
	if(a == NULL)
	{
		OSDEP_DBG("%s: alloc memory fail!\n", __FUNCTION__);
		return NULL;
	}

	for( j=0; j<h; j++ )
		a[j] = ((char *)(a+h)) + j*w*size;

	return a;
}

void rtw_mfree2d(void *pbuf, int h, int w, int size)
{
	rtw_mfree((u8 *)pbuf, h*sizeof(void*) + w*h*size);
}

void rtw_memcpy(void* dst, void* src, u32 sz)
{
	if(osdep_service.rtw_memcpy)
		osdep_service.rtw_memcpy(dst, src, sz);
	else
		OSDEP_DBG("Not implement osdep service: rtw_memcpy");
}

int rtw_memcmp(void *dst, void *src, u32 sz)
{
	if(osdep_service.rtw_memcmp)
		return osdep_service.rtw_memcmp(dst, src, sz);
	else
		OSDEP_DBG("Not implement osdep service: rtw_memcmp");

	return _FALSE;
}

void rtw_memset(void *pbuf, int c, u32 sz)
{
	if(osdep_service.rtw_memset)
		osdep_service.rtw_memset(pbuf, c, sz);
	else
		OSDEP_DBG("Not implement osdep service: rtw_memset");
}

void rtw_init_listhead(_list *list)
{
	INIT_LIST_HEAD(list);
}

/*
For the following list_xxx operations, 
caller must guarantee the atomic context.
Otherwise, there will be racing condition.
*/
u32 rtw_is_list_empty(_list *phead)
{
	if(list_empty(phead))
		return _TRUE;

	return _FALSE;
}

void rtw_list_insert_head(_list *plist, _list *phead)
{
	list_add(plist, phead);
}

void rtw_list_insert_tail(_list *plist, _list *phead)
{
	list_add_tail(plist, phead);
}

/*

Caller must check if the list is empty before calling rtw_list_delete

*/
void rtw_list_delete(_list *plist)
{
	list_del_init(plist);
}



void rtw_init_sema(_sema *sema, int init_val)
{
	if(osdep_service.rtw_init_sema)
		osdep_service.rtw_init_sema(sema, init_val);
	else
		OSDEP_DBG("Not implement osdep service: rtw_init_sema");
}

void rtw_free_sema(_sema *sema)
{
	if(osdep_service.rtw_free_sema)
		osdep_service.rtw_free_sema(sema);
	else
		OSDEP_DBG("Not implement osdep service: rtw_free_sema");
}

void rtw_up_sema(_sema *sema)
{
	if(osdep_service.rtw_up_sema)
		osdep_service.rtw_up_sema(sema);
	else
		OSDEP_DBG("Not implement osdep service: rtw_up_sema");
}

void rtw_up_sema_from_isr(_sema *sema)
{
	if(osdep_service.rtw_up_sema_from_isr)
		osdep_service.rtw_up_sema_from_isr(sema);
	else
		OSDEP_DBG("Not implement osdep service: rtw_up_sema_from_isr");
}

u32	rtw_down_timeout_sema(_sema *sema, u32 timeout)
{
	if(osdep_service.rtw_down_timeout_sema)
		return osdep_service.rtw_down_timeout_sema(sema, timeout);
	else
		OSDEP_DBG("Not implement osdep service: rtw_down_timeout_sema");

	return _FAIL;
}

u32 rtw_down_sema(_sema *sema)
{
	while(rtw_down_timeout_sema(sema, RTW_MAX_DELAY) != _TRUE)
//		rom_e_rtw_msg_871X_LEVEL(DOWN_SEMA_1, _drv_always_, "%s(%p) failed, retry\n",  __FUNCTION__, sema);
		//OSDEP_DBG_LEVEL(_drv_always_, "%s(%p) failed, retry\n",  __FUNCTION__, sema);
		OSDEP_DBG("%s(%p) failed, retry\n",  __FUNCTION__, sema);
	return _TRUE;
}

void rtw_mutex_init(_mutex *pmutex)
{
	if(osdep_service.rtw_mutex_init)
		osdep_service.rtw_mutex_init(pmutex);
	else
		OSDEP_DBG("Not implement osdep service: rtw_mutex_init");
}

void rtw_mutex_free(_mutex *pmutex)
{
	if(osdep_service.rtw_mutex_free)
		osdep_service.rtw_mutex_free(pmutex);
	else
		OSDEP_DBG("Not implement osdep service: rtw_mutex_free");
}

void rtw_mutex_put(_mutex *pmutex)
{
	if(osdep_service.rtw_mutex_put)
		osdep_service.rtw_mutex_put(pmutex);
	else
		OSDEP_DBG("Not implement osdep service: rtw_mutex_put");
}

void rtw_mutex_get(_mutex *pmutex)
{
	if(osdep_service.rtw_mutex_get)
		osdep_service.rtw_mutex_get(pmutex);
	else
		OSDEP_DBG("Not implement osdep service: rtw_mutex_get");
}

int rtw_mutex_get_timeout(_mutex *pmutex, u32 timeout_ms)
{
	if(osdep_service.rtw_mutex_get_timeout)
		return osdep_service.rtw_mutex_get_timeout(pmutex, timeout_ms);
	else{
		OSDEP_DBG("Not implement osdep service: rtw_mutex_get_timeout");
		return -1;
	}
}

void rtw_enter_critical(_lock *plock, _irqL *pirqL)
{
	if(osdep_service.rtw_enter_critical)
		osdep_service.rtw_enter_critical(plock, pirqL);
	else
		OSDEP_DBG("Not implement osdep service: rtw_enter_critical");
}

void rtw_exit_critical(_lock *plock, _irqL *pirqL)
{
	if(osdep_service.rtw_exit_critical)
		osdep_service.rtw_exit_critical(plock, pirqL);
	else
		OSDEP_DBG("Not implement osdep service: rtw_exit_critical");
}

void rtw_enter_critical_from_isr(_lock *plock, _irqL *pirqL)
{
	if(osdep_service.rtw_enter_critical)
		osdep_service.rtw_enter_critical(plock, pirqL);
	else
		OSDEP_DBG("Not implement osdep service: rtw_enter_critical_from_isr");
}

void rtw_exit_critical_from_isr(_lock *plock, _irqL *pirqL)
{
	if(osdep_service.rtw_exit_critical)
		osdep_service.rtw_exit_critical(plock, pirqL);
	else
		OSDEP_DBG("Not implement osdep service: rtw_exit_critical_from_isr");
}

void rtw_enter_critical_bh(_lock *plock, _irqL *pirqL)
{
	rtw_spin_lock(plock);
}

void rtw_exit_critical_bh(_lock *plock, _irqL *pirqL)
{
	rtw_spin_unlock(plock);
}

int rtw_enter_critical_mutex(_mutex *pmutex, _irqL *pirqL)
{
	if(osdep_service.rtw_enter_critical_mutex)
		return osdep_service.rtw_enter_critical_mutex(pmutex, pirqL);
	else
		OSDEP_DBG("Not implement osdep service: rtw_enter_critical_mutex");

	return 0;
}

void rtw_exit_critical_mutex(_mutex *pmutex, _irqL *pirqL)
{
	if(osdep_service.rtw_exit_critical_mutex)
		osdep_service.rtw_exit_critical_mutex(pmutex, pirqL);
	else
		OSDEP_DBG("Not implement osdep service: rtw_exit_critical_mutex");
}

void	rtw_init_queue(_queue	*pqueue)
{
	rtw_init_listhead(&(pqueue->queue));
	rtw_spinlock_init(&(pqueue->lock));
}

u32	  rtw_queue_empty(_queue	*pqueue)
{
	return (rtw_is_list_empty(&(pqueue->queue)));
}


u32 rtw_end_of_queue_search(_list *head, _list *plist)
{
	if (head == plist)
		return _TRUE;
	else
		return _FALSE;
}

#if 1
void rtw_spinlock_init(_lock *plock)
{
	if(osdep_service.rtw_spinlock_init)
		osdep_service.rtw_spinlock_init(plock);
	else
		OSDEP_DBG("Not implement osdep service: rtw_spinlock_init");
}

void rtw_spinlock_free(_lock *plock)
{
	if(osdep_service.rtw_spinlock_free)
		osdep_service.rtw_spinlock_free(plock);
	else
		OSDEP_DBG("Not implement osdep service: rtw_spinlock_free");
}

void rtw_spin_lock(_lock *plock)
{
	if(osdep_service.rtw_spin_lock)
		osdep_service.rtw_spin_lock(plock);
	else
		OSDEP_DBG("Not implement osdep service: rtw_spin_lock");
}

void rtw_spin_unlock(_lock *plock)
{
	if(osdep_service.rtw_spin_unlock)
		osdep_service.rtw_spin_unlock(plock);
	else
		OSDEP_DBG("Not implement osdep service: rtw_spin_unlock");
}

void rtw_spinlock_irqsave(_lock *plock, _irqL *irqL)
{
	if(osdep_service.rtw_spinlock_irqsave)
		osdep_service.rtw_spinlock_irqsave(plock, irqL);
	else
		OSDEP_DBG("Not implement osdep service: rtw_spinlock_irqsave");
}

void rtw_spinunlock_irqsave(_lock *plock, _irqL *irqL)
{
	if(osdep_service.rtw_spinunlock_irqsave)
		osdep_service.rtw_spinunlock_irqsave(plock, irqL);
	else
		OSDEP_DBG("Not implement osdep service: rtw_spinunlock_irqsave");
}
#endif

int rtw_init_xqueue( _xqueue* queue, const char* name, u32 message_size, u32 number_of_messages )
{
	if(osdep_service.rtw_init_xqueue)
		return (int)osdep_service.rtw_init_xqueue(queue, name, message_size, number_of_messages);
	else
		OSDEP_DBG("Not implement osdep service: rtw_init_xqueue");

	return FAIL;
}

int rtw_push_to_xqueue( _xqueue* queue, void* message, u32 timeout_ms )
{
	if(osdep_service.rtw_push_to_xqueue)
		return (int)osdep_service.rtw_push_to_xqueue(queue, message, timeout_ms);
	else
		OSDEP_DBG("Not implement osdep service: rtw_push_to_xqueue");

	return FAIL;
}

int rtw_pop_from_xqueue( _xqueue* queue, void* message, u32 timeout_ms )
{
	if(osdep_service.rtw_pop_from_xqueue)
		return (int)osdep_service.rtw_pop_from_xqueue(queue, message, timeout_ms);
	else
		OSDEP_DBG("Not implement osdep service: rtw_pop_from_xqueue");

	return FAIL;
}

int rtw_deinit_xqueue( _xqueue* queue )
{
	if(osdep_service.rtw_deinit_xqueue)
		return (int)osdep_service.rtw_deinit_xqueue(queue);
	else
		OSDEP_DBG("Not implement osdep service: rtw_deinit_xqueue");

	return FAIL;
}

#if 0
void rtw_init_queue(_queue *pqueue)
{
	rtw_init_listhead(&(pqueue->queue));
	rtw_mutex_init(&(pqueue->lock));
}

void	rtw_deinit_queue(_queue *pqueue)
{
	rtw_mutex_free(&(pqueue->lock));
}

u32 rtw_is_queue_empty(_queue *pqueue)
{
	return (rtw_is_list_empty(&(pqueue->queue)));
}

u32 rtw_end_of_queue_search(_list *head, _list *plist)
{
	if (head == plist)
		return _TRUE;

	return _FALSE;
}

_list	*rtw_get_queue_head(_queue	*queue)
{
	return (&(queue->queue));
}
#endif

u32 rtw_get_current_time(void)
{
	if(osdep_service.rtw_get_current_time)
		return osdep_service.rtw_get_current_time();
	else
		OSDEP_DBG("Not implement osdep service: rtw_get_current_time");

	return 0;
}

u32 rtw_systime_to_ms(u32 systime)
{
	if(osdep_service.rtw_systime_to_ms)
		return osdep_service.rtw_systime_to_ms(systime);
	else
		OSDEP_DBG("Not implement osdep service: rtw_systime_to_ms");

	return 0;
}

u32 rtw_systime_to_sec(u32 systime)
{
	if(osdep_service.rtw_systime_to_sec)
		return osdep_service.rtw_systime_to_sec(systime);
	else
		OSDEP_DBG("Not implement osdep service: rtw_systime_to_sec");

	return 0;
}

u32 rtw_ms_to_systime(u32 ms)
{
	if(osdep_service.rtw_ms_to_systime)
		return osdep_service.rtw_ms_to_systime(ms);
	else
		OSDEP_DBG("Not implement osdep service: rtw_ms_to_systime");

	return 0;
}

u32 rtw_sec_to_systime(u32 sec)
{
	if(osdep_service.rtw_sec_to_systime)
		return osdep_service.rtw_sec_to_systime(sec);
	else
		OSDEP_DBG("Not implement osdep service: rtw_sec_to_systime");

	return 0;
}

// the input parameter start use the same unit as returned by rtw_get_current_time
s32 rtw_get_passing_time_ms(u32 start)
{
	return rtw_systime_to_ms(rtw_get_current_time() - start);
}

s32 rtw_get_time_interval_ms(u32 start, u32 end)
{
	return rtw_systime_to_ms(end - start);
}
	
void rtw_msleep_os(int ms)
{
	if(osdep_service.rtw_msleep_os)
		osdep_service.rtw_msleep_os(ms);
	else
		OSDEP_DBG("Not implement osdep service: rtw_msleep_os");
}

void rtw_usleep_os(int us)
{
	if(osdep_service.rtw_usleep_os)
		osdep_service.rtw_usleep_os(us);
	else
		OSDEP_DBG("Not implement osdep service: rtw_usleep_os");	
}

void rtw_mdelay_os(int ms)
{
	if(osdep_service.rtw_mdelay_os)
		osdep_service.rtw_mdelay_os(ms);
	else
		OSDEP_DBG("Not implement osdep service: rtw_mdelay_os");
}

void rtw_udelay_os(int us)
{
	if(osdep_service.rtw_udelay_os)
		osdep_service.rtw_udelay_os(us);
	else
		OSDEP_DBG("Not implement osdep service: rtw_udelay_os");
}

void rtw_yield_os(void)
{
	if(osdep_service.rtw_yield_os)
		osdep_service.rtw_yield_os();
	else
		OSDEP_DBG("Not implement osdep service: rtw_yield_os");
}

void ATOMIC_SET(ATOMIC_T *v, int i)
{
	if(osdep_service.ATOMIC_SET)
		osdep_service.ATOMIC_SET(v, i);
	else
		OSDEP_DBG("Not implement osdep service: ATOMIC_SET");
}

int ATOMIC_READ(ATOMIC_T *v)
{
	if(osdep_service.ATOMIC_READ)
		return osdep_service.ATOMIC_READ(v);
	else
		OSDEP_DBG("Not implement osdep service: ATOMIC_READ");

	return 0;
}

void ATOMIC_ADD(ATOMIC_T *v, int i)
{
	if(osdep_service.ATOMIC_ADD)
		osdep_service.ATOMIC_ADD(v, i);
	else
		OSDEP_DBG("Not implement osdep service: ATOMIC_ADD");
}

void ATOMIC_SUB(ATOMIC_T *v, int i)
{
	if(osdep_service.ATOMIC_SUB)
		osdep_service.ATOMIC_SUB(v, i);
	else
		OSDEP_DBG("Not implement osdep service: ATOMIC_SUB");
}

void ATOMIC_INC(ATOMIC_T *v)
{
	if(osdep_service.ATOMIC_INC)
		osdep_service.ATOMIC_INC(v);
	else
		OSDEP_DBG("Not implement osdep service: ATOMIC_INC");
}

void ATOMIC_DEC(ATOMIC_T *v)
{
	if(osdep_service.ATOMIC_DEC)
		osdep_service.ATOMIC_DEC(v);
	else
		OSDEP_DBG("Not implement osdep service: ATOMIC_DEC");
}

int ATOMIC_ADD_RETURN(ATOMIC_T *v, int i)
{
	if(osdep_service.ATOMIC_ADD_RETURN)
		return osdep_service.ATOMIC_ADD_RETURN(v, i);
	else
		OSDEP_DBG("Not implement osdep service: ATOMIC_ADD_RETURN");

	return 0;
}

int ATOMIC_SUB_RETURN(ATOMIC_T *v, int i)
{
	if(osdep_service.ATOMIC_SUB_RETURN)
		return osdep_service.ATOMIC_SUB_RETURN(v, i);
	else
		OSDEP_DBG("Not implement osdep service: ATOMIC_SUB_RETURN");

	return 0;
}

int ATOMIC_INC_RETURN(ATOMIC_T *v)
{
	if(osdep_service.ATOMIC_INC_RETURN)
		return osdep_service.ATOMIC_INC_RETURN(v);
	else
		OSDEP_DBG("Not implement osdep service: ATOMIC_INC_RETURN");

	return 0;
}

int ATOMIC_DEC_RETURN(ATOMIC_T *v)
{
	if(osdep_service.ATOMIC_DEC_RETURN)
		return osdep_service.ATOMIC_DEC_RETURN(v);
	else
		OSDEP_DBG("Not implement osdep service: ATOMIC_DEC_RETURN");

	return 0;
}

int ATOMIC_DEC_AND_TEST(ATOMIC_T *v)
{
	return ATOMIC_DEC_RETURN(v) == 0;
}

u64 rtw_modular64(u64 x, u64 y)
{
	if(osdep_service.rtw_modular64)
		return osdep_service.rtw_modular64(x, y);
	else
		OSDEP_DBG("Not implement osdep service: rtw_modular64");

	return 0;
}

int rtw_get_random_bytes(void* dst, u32 size)
{
	if(osdep_service.rtw_get_random_bytes)
		return osdep_service.rtw_get_random_bytes(dst, size);
	else
		OSDEP_DBG("Not implement osdep service: rtw_get_random_bytes");

	return 0;
}

u32 rtw_getFreeHeapSize(void)
{
	if(osdep_service.rtw_getFreeHeapSize)
		return osdep_service.rtw_getFreeHeapSize();
	else
		OSDEP_DBG("Not implement osdep service: rtw_getFreeHeapSize");

	return 0;
}

int rtw_netif_queue_stopped(void *pnetdev)
{
	return 0;
}

void rtw_netif_wake_queue(void *pnetdev)
{
}

void rtw_netif_start_queue(void *pnetdev)
{
}

void rtw_netif_stop_queue(void *pnetdev)
{
}

void flush_signals_thread(void) 
{
}

void rtw_acquire_wakelock(void)
{
	if (osdep_service.rtw_acquire_wakelock)
		osdep_service.rtw_acquire_wakelock();
	else
		OSDEP_DBG("Not implement osdep service: rtw_acquire_wakelock");
}

void rtw_release_wakelock(void)
{
	if (osdep_service.rtw_release_wakelock)
		osdep_service.rtw_release_wakelock();
	else
		OSDEP_DBG("Not implement osdep service: rtw_release_wakelock");
}

void rtw_wakelock_timeout(u32 timeoutms)
{
	if (osdep_service.rtw_wakelock_timeout)
		osdep_service.rtw_wakelock_timeout(timeoutms);
	else
		OSDEP_DBG("Not implement osdep service: rtw_wakelock_timeout");
}

int rtw_create_task(struct task_struct *task, const char *name,
	u32 stack_size, u32 priority, thread_func_t func, void *thctx)
{
	if(osdep_service.rtw_create_task)
		return osdep_service.rtw_create_task(task, name, stack_size, priority, func, thctx);
	else
		OSDEP_DBG("Not implement osdep service: rtw_create_task");
	return 1;
}
void rtw_delete_task(struct task_struct *task)
{
	if(osdep_service.rtw_delete_task)
		osdep_service.rtw_delete_task(task);
	else
		OSDEP_DBG("Not implement osdep service: rtw_delete_task");

	return;	
}
void rtw_wakeup_task(struct task_struct *task)
{
	if(osdep_service.rtw_wakeup_task)
		osdep_service.rtw_wakeup_task(task);
	else
		OSDEP_DBG("Not implement osdep service: rtw_wakeup_task");

	return;	
}

static void worker_thread_main( void *arg )
{
	rtw_worker_thread_t* worker_thread = (rtw_worker_thread_t*) arg;

	while ( 1 )
	{
		rtw_event_message_t message;

		if ( rtw_pop_from_xqueue( &worker_thread->event_queue, &message, RTW_WAIT_FOREVER ) == SUCCESS )
		{
			message.function(message.buf, message.buf_len, message.flags, message.user_data);
			if(message.buf){
				//printf("\n!!!!!Free %p(%d)\n", message.buf, message.buf_len);
				_rtw_mfree(message.buf, message.buf_len);
			}
		}
	}
}

int rtw_create_worker_thread( rtw_worker_thread_t* worker_thread, u8 priority, u32 stack_size, u32 event_queue_size )
{
	if(NULL == worker_thread)
		return FAIL;

	memset( worker_thread, 0, sizeof( *worker_thread ) );

	if ( rtw_init_xqueue( &worker_thread->event_queue, "worker queue", sizeof(rtw_event_message_t), event_queue_size ) != SUCCESS )
	{
		return FAIL;
	}

	if ( !rtw_create_task( &worker_thread->thread, "worker thread", stack_size, priority, worker_thread_main, (void*) worker_thread ) )
	{
		rtw_deinit_xqueue( &worker_thread->event_queue );
		return FAIL;
	}

	return SUCCESS;
}

int rtw_delete_worker_thread( rtw_worker_thread_t* worker_thread )
{
	if(NULL == worker_thread)
		return FAIL;

	rtw_deinit_xqueue( &worker_thread->event_queue );

	rtw_delete_task(&worker_thread->thread);

	return SUCCESS;
}

_timerHandle rtw_timerCreate( const signed char *pcTimerName, 
							  osdepTickType xTimerPeriodInTicks, 
							  u32 uxAutoReload, 
							  void * pvTimerID, 
							  TIMER_FUN pxCallbackFunction )
{
	if(osdep_service.rtw_timerCreate)
		return osdep_service.rtw_timerCreate(pcTimerName, xTimerPeriodInTicks, uxAutoReload, 
											 pvTimerID, pxCallbackFunction);
	else
		OSDEP_DBG("Not implement osdep service: rtw_timerCreate");

	return 0;	
}

u32 rtw_timerDelete( _timerHandle xTimer, 
							   osdepTickType xBlockTime )
{
	if(osdep_service.rtw_timerDelete)
		return osdep_service.rtw_timerDelete( xTimer, xBlockTime );
	else
		OSDEP_DBG("Not implement osdep service: rtw_timerDelete");

	return 0;	
}

u32 rtw_timerIsTimerActive( _timerHandle xTimer )
{
	if(osdep_service.rtw_timerIsTimerActive)
		return osdep_service.rtw_timerIsTimerActive(xTimer);
	else
		OSDEP_DBG("Not implement osdep service: rtw_timerIsTimerActive");

	return 0;	
}

u32  rtw_timerStop( _timerHandle xTimer, 
							   osdepTickType xBlockTime )
{
	if(osdep_service.rtw_timerStop)
		return osdep_service.rtw_timerStop(xTimer, xBlockTime);
	else
		OSDEP_DBG("Not implement osdep service: rtw_timerStop");

	return 0;	
}

u32  rtw_timerChangePeriod( _timerHandle xTimer, 
							   osdepTickType xNewPeriod, 
							   osdepTickType xBlockTime )
{
	if(osdep_service.rtw_timerChangePeriod)
		return osdep_service.rtw_timerChangePeriod(xTimer, xNewPeriod, xBlockTime);
	else
		OSDEP_DBG("Not implement osdep service: rtw_timerChangePeriod");

	return 0;	
}

#if 0 //TODO
void rtw_init_delayed_work(struct delayed_work *dwork, work_func_t func, const char *name)
{
	if(osdep_service.rtw_init_delayed_work)
		osdep_service.rtw_init_delayed_work(dwork, func, name);
	else
		OSDEP_DBG("Not implement osdep service: rtw_init_delayed_work");

	return;	
}

void rtw_deinit_delayed_work(struct delayed_work *dwork)
{
	if(osdep_service.rtw_deinit_delayed_work)
		osdep_service.rtw_deinit_delayed_work(dwork);
	else
		OSDEP_DBG("Not implement osdep service: rtw_deinit_delayed_work");

	return;	
}

int rtw_queue_delayed_work(struct workqueue_struct *wq,
			struct delayed_work *dwork, u32 delay, void* context)
{
	if(osdep_service.rtw_queue_delayed_work)
		osdep_service.rtw_queue_delayed_work(wq, dwork, delay, context);
	else
		OSDEP_DBG("Not implement osdep service: rtw_queue_delayed_work");

	return;	
}

BOOLEAN rtw_cancel_delayed_work(struct delayed_work *dwork)
{
	if(osdep_service.rtw_cancel_delayed_work)
		osdep_service.rtw_cancel_delayed_work(dwork);
	else
		OSDEP_DBG("Not implement osdep service: rtw_cancel_delayed_work");

	return;	
}
#endif
void rtw_thread_enter(char *name)
{
	if(osdep_service.rtw_thread_enter)
		osdep_service.rtw_thread_enter(name);
	else
		OSDEP_DBG("Not implement osdep service: rtw_thread_enter");
}

void rtw_thread_exit()
{
	if(osdep_service.rtw_thread_exit)
		osdep_service.rtw_thread_exit();
	else
		OSDEP_DBG("Not implement osdep service: rtw_thread_exit");
}

u8 rtw_get_scheduler_state()
{	
	// OS_SCHEDULER_NOT_STARTED	0
	// OS_SCHEDULER_RUNNING		1
	// OS_SCHEDULER_SUSPENDED	2
	// OS_SCHEDULER_UNREACHABLE	3
	if(osdep_service.rtw_get_scheduler_state)
		return osdep_service.rtw_get_scheduler_state();
	else{
		OSDEP_DBG("Not implement osdep service: rtw_get_scheduler_state");
		return 3;
	}
}
