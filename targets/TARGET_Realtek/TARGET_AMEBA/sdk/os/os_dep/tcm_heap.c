//#include <autoconf.h>
#include "tcm_heap.h"

#include <string.h>    // memset()

#include <osdep_service.h>

//#define _DEBUG

#if CONFIG_USE_TCM_HEAP
#define FREE_FILL_CODE     0xDEAD
#define ALLOC_FILL_CODE    0xBEEF

#define ROUND_UP2(x, pad) (((x) + ((pad) - 1)) & ~((pad) - 1))

#define TCM_HEAP_SIZE  (40*1024)

static struct Heap g_tcm_heap;

#if defined (__ICCARM__)
#pragma location=".tcm.heap"
#else
__attribute__((section(".tcm.heap")))
#endif
HEAP_DEFINE_BUF(tcm_heap, TCM_HEAP_SIZE);
//unsigned char tcm_heap[TCM_HEAP_SIZE];

static int g_heap_inited=0;
static	_lock	tcm_lock;
#ifdef PLATFORM_FREERTOS
extern void vPortSetExtFree( void (*free)( void *p ), uint32_t upper, uint32_t lower );
#else
extern void rtw_set_mfree_ext( void (*free)( void *p ), uint32_t upper, uint32_t lower );
#endif
void tcm_heap_init(void)
{
	//#ifdef _DEBUG
	//memset(memory, FREE_FILL_CODE, size);
	//#endif

	//ASSERT2(((int)memory % alignof(heap_buf_t)) == 0,
	//"memory buffer is unaligned, please use the HEAP_DEFINE_BUF() macro to declare heap buffers!\n");
	
	/* Initialize heap with a single big chunk */
	g_tcm_heap.FreeList = (MemChunk *)&tcm_heap;
	g_tcm_heap.FreeList->next = NULL;
	g_tcm_heap.FreeList->size = sizeof(tcm_heap);
	
	g_heap_inited = 1;
	rtw_spinlock_init(&tcm_lock);
	
#if defined(PLATFORM_FREERTOS) 
	// let RTOS know how to free memory if using as task stack
	vPortSetExtFree(tcm_heap_free, 0x20000000, 0x1fff0000);
#elif defined (PLATFORM_CMSIS_RTOS)	
	rtw_set_mfree_ext(tcm_heap_free, 0x20000000, 0x1fff0000);
#endif
}

void tcm_heap_dump(void)
{
	MemChunk *chunk, *prev;
	struct Heap* h = &g_tcm_heap;
	
	printf("---Free List--\n\r");
	for (prev = (MemChunk *)&h->FreeList, chunk = h->FreeList;
		chunk;
		prev = chunk, chunk = chunk->next)
	{
		printf(" prev %x, chunk %x, size %d \n\r", prev, chunk, chunk->size);
	}
	printf("--------------\n\r");
}

void *tcm_heap_allocmem(int size)
{
	MemChunk *chunk, *prev;
	struct Heap* h = &g_tcm_heap;
	_irqL 	irqL;

	rtw_enter_critical(&tcm_lock, &irqL);
	
	if(!g_heap_inited)	tcm_heap_init();

	/* Round size up to the allocation granularity */
	size = ROUND_UP2(size, sizeof(MemChunk));

	/* Handle allocations of 0 bytes */
	if (!size)
		size = sizeof(MemChunk);

	/* Walk on the free list looking for any chunk big enough to
	 * fit the requested block size.
	 */
	for (prev = (MemChunk *)&h->FreeList, chunk = h->FreeList;
		chunk;
		prev = chunk, chunk = chunk->next)
	{
		if (chunk->size >= size)
		{
			if (chunk->size == size)
			{
				/* Just remove this chunk from the free list */
				prev->next = chunk->next;
				#ifdef _DEBUG
					memset(chunk, ALLOC_FILL_CODE, size);
				#endif
				
				rtw_exit_critical(&tcm_lock, &irqL);
				//printf("----ALLOC1-----\n\r");
				//tcm_heap_dump();
				//printf("--------------\n\r");
				return (void *)chunk;
			}
			else
			{
				/* Allocate from the END of an existing chunk */
				chunk->size -= size;
				#ifdef _DEBUG
					memset((uint8_t *)chunk + chunk->size, ALLOC_FILL_CODE, size);
				#endif
				rtw_exit_critical(&tcm_lock, &irqL);
				//printf("----ALLOC2-----\n\r");
				//tcm_heap_dump();
				//printf("--------------\n\r");
				
				return (void *)((uint8_t *)chunk + chunk->size);
			}
		}
	}
	
	rtw_exit_critical(&tcm_lock, &irqL);
	//printf("----ALLOC3-----\n\r");
	//tcm_heap_dump();
	//printf("--------------\n\r");
	return NULL; /* fail */
}


void tcm_heap_freemem(void *mem, int size)
{
	MemChunk *prev;
	//ASSERT(mem);
	struct Heap* h = &g_tcm_heap;
	_irqL 	irqL;

	rtw_enter_critical(&tcm_lock, &irqL);	
	
	if(!g_heap_inited)	tcm_heap_init();

#ifdef _DEBUG
	memset(mem, FREE_FILL_CODE, size);
#endif

	/* Round size up to the allocation granularity */
	size = ROUND_UP2(size, sizeof(MemChunk));

	/* Handle allocations of 0 bytes */
	if (!size)
		size = sizeof(MemChunk);

	/* Special cases: first chunk in the free list or memory completely full */
	//ASSERT((uint8_t*)mem != (uint8_t*)h->FreeList);
	if (((uint8_t *)mem) < ((uint8_t *)h->FreeList) || !h->FreeList)
	{
		/* Insert memory block before the current free list head */
		prev = (MemChunk *)mem;
		prev->next = h->FreeList;
		prev->size = size;
		h->FreeList = prev;
	}
	else /* Normal case: not the first chunk in the free list */
	{
		/*
		 * Walk on the free list. Stop at the insertion point (when mem
		 * is between prev and prev->next)
		 */
		prev = h->FreeList;
		while (prev->next < (MemChunk *)mem && prev->next)
			prev = prev->next;

		/* Make sure mem is not *within* prev */
		//ASSERT((uint8_t*)mem >= (uint8_t*)prev + prev->size);

		/* Should it be merged with previous block? */
		if (((uint8_t *)prev) + prev->size == ((uint8_t *)mem))
		{
			/* Yes */
			prev->size += size;
		}
		else /* not merged with previous chunk */
		{
			MemChunk *curr = (MemChunk*)mem;

			/* insert it after the previous node
			 * and move the 'prev' pointer forward
			 * for the following operations
			 */
			curr->next = prev->next;
			curr->size = size;
			prev->next = curr;

			/* Adjust for the following test */
			prev = curr;
		}
	}

	/* Also merge with next chunk? */
	if (((uint8_t *)prev) + prev->size == ((uint8_t *)prev->next))
	{
		prev->size += prev->next->size;
		prev->next = prev->next->next;

		/* There should be only one merge opportunity, becuase we always merge on free */
		//ASSERT((uint8_t*)prev + prev->size != (uint8_t*)prev->next);
	}
	
	rtw_exit_critical(&tcm_lock, &irqL);	
	//printf("---FREE %x--\n\r", mem);
	//tcm_heap_dump();
	//printf("--------------\n\r");
	
}

int tcm_heap_freeSpace(void)
{
	int free_mem = 0;
	struct Heap* h = &g_tcm_heap;
	_irqL 	irqL;
	MemChunk *chunk;

	rtw_enter_critical(&tcm_lock, &irqL);
	
	if(!g_heap_inited)	tcm_heap_init();
	
	for (chunk = h->FreeList; chunk; chunk = chunk->next)
		free_mem += chunk->size;

	rtw_exit_critical(&tcm_lock, &irqL);
	return free_mem;
}


/**
 * Standard malloc interface
 */
void *tcm_heap_malloc(int size)
{
	int *mem;

	size += sizeof(int);
	if ((mem = (int*)tcm_heap_allocmem(size))){
		*mem++ = size;
	}

	return mem;
}

/**
 * Standard calloc interface
 */
void *tcm_heap_calloc(int size)
{
	void *mem;

	if ((mem = tcm_heap_malloc(size)))
		memset(mem, 0, size);

	return mem;
}

/**
 * Free a block of memory, determining its size automatically.
 *
 * \param h    Heap from which the block was allocated.
 * \param mem  Pointer to a block of memory previously allocated with
 *             either heap_malloc() or heap_calloc().
 *
 * \note If \a mem is a NULL pointer, no operation is performed.
 *
 * \note Freeing the same memory block twice has undefined behavior.
 *
 * \note This function works like the ANSI C free().
 */
void tcm_heap_free(void *mem)
{
	int *_mem = (int *)mem;

	if (_mem)
	{
		--_mem;
		tcm_heap_freemem(_mem, *_mem);
	}
}


static void alloc_test(int size, int test_len)
{
	//Simple test
	uint8_t *a[100];
	int i, j;
	
	for (i = 0; i < test_len; i++)
	{
		a[i] = tcm_heap_allocmem(size);
		//ASSERT(a[i]);
		for (j = 0; j < size; j++)
			a[i][j] = i;
	}

	//ASSERT(heap_freeSpace(&h) == HEAP_SIZE - test_len * ROUND_UP2(size, sizeof(MemChunk)));

	for (i = 0; i < test_len; i++)
	{
		for (j = 0; j < size; j++)
		{
			printf("a[%d][%d] = %d\n", i, j, a[i][j]);
			//ASSERT(a[i][j] == i);
		}
		tcm_heap_freemem(a[i], size);
	}
	//ASSERT(heap_freeSpace(&h) == HEAP_SIZE);
}

#define ALLOC_SIZE 256
#define ALLOC_SIZE2 1024
#define TEST_LEN 20
#define TEST_LEN2 10
#define HEAP_SIZE 59*1024
int tcm_heap_testRun(void)
{
	alloc_test(ALLOC_SIZE, TEST_LEN);
	alloc_test(ALLOC_SIZE2, TEST_LEN2);
	/* Try to allocate the whole heap */
	uint8_t *b = tcm_heap_allocmem(HEAP_SIZE);
	int i, j;
	//ASSERT(b);
	//ASSERT(heap_freeSpace(&h) == 0);

	//ASSERT(!heap_allocmem(&h, HEAP_SIZE));

	for (j = 0; j < HEAP_SIZE; j++)
		b[j] = j;
	
	for (j = 0; j < HEAP_SIZE; j++)
	{
		printf("b[%d] = %d\n", j, j);
		//ASSERT(b[j] == (j & 0xff));
	}
	tcm_heap_freemem(b, HEAP_SIZE);
	//ASSERT(heap_freeSpace(&h) == HEAP_SIZE);

	return 0;
}

#endif
