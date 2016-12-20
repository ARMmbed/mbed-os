/***************************************************************************//**
 * @file buffer_pool_allocator.c
 * @brief The source for a simple memory allocator that statically creates pools
 *        of fixed size buffers to allocate from.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#include <stdlib.h>

#include "buffer_pool_allocator.h"

#include "em_int.h"

#ifdef CONFIGURATION_HEADER
#include CONFIGURATION_HEADER
#endif

// -----------------------------------------------------------------------------
// Configuration Macros
// -----------------------------------------------------------------------------

// Default to a ping-pong buffer pool with a size of 128 (127 MTU + 1 length) bytes per buffer
#ifndef BUFFER_POOL_SIZE
#define BUFFER_POOL_SIZE 2
#endif
#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE 128
#endif

#define INVALID_BUFFER_OBJ ((void*)0xFFFFFFFF)

typedef struct {
  uint8_t refCount;
  uint8_t data[MAX_BUFFER_SIZE];
} BufferPoolObj_t;

static BufferPoolObj_t memoryObjs[BUFFER_POOL_SIZE];

void* memoryAllocate(uint32_t size)
{
  uint32_t i = 0;
  void *handle = INVALID_BUFFER_OBJ;

  // We can't support sizes greater than the maximum heap buffer size
  if(size > MAX_BUFFER_SIZE) {
    return INVALID_BUFFER_OBJ;
  }

  INT_Disable();
  for(i = 0; i < BUFFER_POOL_SIZE; i++)
  {
    if(memoryObjs[i].refCount == 0)
    {
      memoryObjs[i].refCount = 1;
      handle = (void*)i;
      break;
    }
  }
  INT_Enable();

  return handle;
}

void *memoryPtrFromHandle(void *handle)
{
  void *ptr = NULL;

  // Make sure we were given a valid handle
  if((handle == INVALID_BUFFER_OBJ) || ((uint32_t)handle > BUFFER_POOL_SIZE))
  {
    return NULL;
  }

  INT_Disable();
  if(memoryObjs[(uint32_t)handle].refCount > 0)
  {
    ptr = memoryObjs[(uint32_t)handle].data;
  }
  INT_Enable();

  return ptr;
}

void memoryFree(void *handle)
{
  INT_Disable();
  if(memoryPtrFromHandle(handle) != NULL)
  {
    memoryObjs[(uint32_t)handle].refCount--;
  }
  INT_Enable();
}

void memoryTakeReference(void *handle)
{
  INT_Disable();
  if(memoryPtrFromHandle(handle) != NULL)
  {
    memoryObjs[(uint32_t)handle].refCount++;
  }
  INT_Enable();
}
