/**
 * @file xmc_common.c
 * @date 2017-02-25
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-20:
 *     - Initial <br>
 *      
 * 2017-02-25:
 *     - Remove the need to define XMC_USER_ASSERT_FUNCTION
 *     - XMC_AssertHandler fixed compilation warnings
 *
 * @endcond 
 *
 */

#include "xmc_common.h"

/*******************************************************************************
 * DATA STRUCTURES
 *******************************************************************************/
struct list
{
  struct list *next;
};

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/
#if defined(XMC_ASSERT_ENABLE)
__WEAK void XMC_AssertHandler(const char *const msg, const char *const file, uint32_t line)
{
  XMC_UNUSED_ARG(msg);
  XMC_UNUSED_ARG(file);
  XMC_UNUSED_ARG(line);

  while(1)
  {
    /* Endless loop */
  }
}
#endif

void XMC_LIST_Init(XMC_LIST_t *list)
{
  *list = NULL;
}

void *XMC_LIST_GetHead(XMC_LIST_t *list)
{
  return *list;
}

void *XMC_LIST_GetTail(XMC_LIST_t *list)
{
  struct list *tail;
  
  if (*list == NULL) 
  {
    tail = NULL;
  }
  else
  {
    for (tail = (struct list *)*list; tail->next != NULL; tail = tail->next)
    {
      /* Loop through the list */
    }  
  }
  
  return tail;    
}

void XMC_LIST_Add(XMC_LIST_t *list, void *item)
{
  struct list *tail;
  
  ((struct list *)item)->next = NULL;
  tail = (struct list *)XMC_LIST_GetTail(list);
  
  if (tail == NULL) 
  {
    *list = item;
  } 
  else 
  {
    tail->next = (struct list *)item;
  }
}

void XMC_LIST_Remove(XMC_LIST_t *list, void *item)
{
  struct list *right, *left;
  
  if (*list != NULL) 
  {
    left = NULL;
    for(right = (struct list *)*list; right != NULL; right = right->next)
    {
      if(right == item) 
      {
        if(left == NULL) 
        {
          /* First on list */
          *list = right->next;
        } 
        else 
        {
          /* Not first on list */
          left->next = right->next;
        }
        right->next = NULL;
        break;
      }
      left = right;
    }
  }
}

void XMC_LIST_Insert(XMC_LIST_t *list, void *prev_item, void *new_item)
{
  if (prev_item == NULL) 
  {
	((struct list *)new_item)->next = (struct list *)*list;
	*list = new_item;
  } 
  else 
  {
    ((struct list *)new_item)->next = ((struct list *)prev_item)->next;
    ((struct list *)prev_item)->next = (struct list *)new_item;
  }
}

void XMC_PRIOARRAY_Init(XMC_PRIOARRAY_t *prioarray)
{
  XMC_ASSERT("XMC_PRIOARRAY_Init: NULL pointer", prioarray != NULL);

  /* Initialize head, next points to tail, previous to NULL and the priority is MININT */
  prioarray->items[prioarray->size].next = prioarray->size + 1;
  prioarray->items[prioarray->size].previous = -1;
  prioarray->items[prioarray->size].priority = INT32_MAX;

  /* Initialize tail, next points to NULL, previous is the head and the priority is MAXINT */
  prioarray->items[prioarray->size + 1].next = -1;
  prioarray->items[prioarray->size + 1].previous = prioarray->size;
  prioarray->items[prioarray->size + 1].priority = INT32_MIN;

}

void XMC_PRIOARRAY_Add(XMC_PRIOARRAY_t *prioarray, int32_t item, int32_t priority)
{
  int32_t next;
  int32_t previous;

  XMC_ASSERT("XMC_PRIOARRAY_Add: item out of range", (item >= 0) && (item < prioarray->size));
  
  next = XMC_PRIOARRAY_GetHead(prioarray);
  while (XMC_PRIOARRAY_GetItemPriority(prioarray, next) > priority)
  {
    next = XMC_PRIOARRAY_GetItemNext(prioarray, next);
  }

  previous = prioarray->items[next].previous;

  prioarray->items[item].next = next;
  prioarray->items[item].previous = previous;
  prioarray->items[item].priority = priority;

  prioarray->items[previous].next = item;
  prioarray->items[next].previous = item;
}

void XMC_PRIOARRAY_Remove(XMC_PRIOARRAY_t *prioarray, int32_t item)
{
  int32_t next;
  int32_t previous;

  XMC_ASSERT("XMC_PRIOARRAY_Add: item out of range", (item >= 0) && (item < prioarray->size));
  
  next = prioarray->items[item].next;
  previous = prioarray->items[item].previous;

  prioarray->items[previous].next = next;
  prioarray->items[next].previous = previous;
}


