/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Dynamic ATT services and attributes.
 *
 *  Copyright (c) 2017-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "wsf_math.h"
#include "util/bstream.h"
#include "att_api.h"
#include "att_main.h"
#include "dm_api.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Configurable number of bytes in the Dynamic Service and Attribute Heap */
#ifndef ATTS_DYN_HEAP_SIZE
#define ATTS_DYN_HEAP_SIZE                  1280
#endif

/* Configurable memory byte alignment Dynamic Service and Attribute Heap */
#ifndef ATTS_DYN_ALIGNMENT
#define ATTS_DYN_ALIGNMENT                  4
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Dynamic service and attributes control block */
typedef struct
{
  uint8_t numServices;
  uint8_t *pNextBuffer;
} attsDynCb_t;

/* Dynamic service group control block */
typedef struct
{
  attsGroup_t group;
  uint16_t currentAttr;
} attsDynGroupCb_t;

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/* Dynamic Service and Attribute Heap */
static uint8_t attsDynHeap[ATTS_DYN_HEAP_SIZE];

/* Dynamic Service and Attribute control block */
static attsDynCb_t attsDynCb;

/*************************************************************************************************/
/*!
 *  \brief  Allocate memory from the Dynamic Service and Attribute Heap.
 *
 *  \param  size    Size of buffer to allocate in bytes
 *
 *  \return Allocated buffer or NULL if failed to allocate a buffer.
 */
/*************************************************************************************************/
static void *attsDynAlloc(uint16_t size)
{
  uint8_t *pMem = attsDynCb.pNextBuffer;

  /* Verify enough space in heap for buffer */
  if (pMem + size <= attsDynHeap + ATTS_DYN_HEAP_SIZE)
  {
#if ATTS_DYN_ALIGNMENT > 1
    /* Increase size if size not a multiple of the memory alignment */
    if (size % ATTS_DYN_ALIGNMENT)
    {
      size += ATTS_DYN_ALIGNMENT - (size % ATTS_DYN_ALIGNMENT);
    }
#endif

    /* Set the next buffer location */
    attsDynCb.pNextBuffer += size;

    return pMem;
  }

  /* Out of heap */
  WSF_ASSERT(FALSE);
  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize the Dynamic ATT Service subsystem.
 *
 *  \param  None
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsDynInit()
{
  attsDynCb.numServices = 0;
  attsDynCb.pNextBuffer = attsDynHeap;
}

/*************************************************************************************************/
/*!
 *  \brief  Dynamically create an ATT Service at runtime.
 *
 *  \param  startHandle       Starting attribute handle in the service
 *  \param  endHandle         Last attribute handle in the service
 *
 *  \return Service Handle.
 *
 *  \note   It is recommended this function only be used when no connections are open and the
 *          device is not in a connectable mode.
 */
/*************************************************************************************************/
void *AttsDynCreateGroup(uint16_t startHandle, uint16_t endHandle)
{
  attsDynGroupCb_t *pGroup = NULL;

  /* Allocate memory for the service control block */
  pGroup = attsDynAlloc(sizeof(attsDynGroupCb_t));
  WSF_ASSERT(pGroup);

  if (pGroup != NULL)
  {

    /* Initialize the service group */
    pGroup->group.startHandle = startHandle;
    pGroup->group.endHandle = endHandle;
    pGroup->group.readCback = NULL;
    pGroup->group.writeCback = NULL;

    /* Allocate memory for the attributes */
    pGroup->group.pAttr = attsDynAlloc(sizeof(attsAttr_t) * (endHandle - startHandle + 1));
    WSF_ASSERT(pGroup->group.pAttr);

    pGroup->currentAttr = 0;
    attsDynCb.numServices++;
  }

  return pGroup;
}

/*************************************************************************************************/
/*!
 *  \brief  Dynamically delete an ATT Service at runtime.
 *
 *  \param  pSvcHandle      Service handle returned by AttsDynCreateGroup
 *
 *  \return None.
 *
 *  \note   It is recommended this function only be used when no connections are open and the
 *          device is not in a connectable mode.
 */
/*************************************************************************************************/
void AttsDynDeleteGroup(void *pSvcHandle)
{
  attsDynGroupCb_t *pGroup = pSvcHandle;

  WSF_ASSERT(attsDynCb.numServices);
  WSF_ASSERT(pGroup);

  /* Remove the group */
  AttsRemoveGroup(pGroup->group.startHandle);

  attsDynCb.numServices--;

  /* If there are no more dynamic groups, initialize the subsystem to free the memory in the heap */
  if (attsDynCb.numServices == 0)
  {
    AttsDynInit();
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Register callback functions for a dynamic ATT Service at runtime.
 *
 *  \param  pSvcHandle    Service handle returned by AttsDynCreateGroup
 *  \param  readCback     Read callback function.
 *  \param  writeCback    Write callback function.
 *
 *  \return None.
 *
 *  \note   It is recommended this function only be used when no connections are open and the
 *          device is not in a connectable mode.
 */
/*************************************************************************************************/
void AttsDynRegister(void *pSvcHandle, attsReadCback_t readCback, attsWriteCback_t writeCback)
{
  attsDynGroupCb_t *pGroup = pSvcHandle;

  WSF_ASSERT(pGroup);

  pGroup->group.readCback = readCback;
  pGroup->group.writeCback = writeCback;
}

/*************************************************************************************************/
/*!
 *  \brief  Dynamically add an attribute to a dynamic ATT Services at app initialization.
 *
 *  \param  pSvcHandle    Service handle returned by AttsDynCreateGroup
 *  \param  pUuid         Constant UUID
 *  \param  pValue        Initial value of attribute (copied into attribute memory)
 *  \param  len           Length of pValue in bytes
 *  \param  maxLen        Maximum length of the attribute in bytes
 *  \param  settings      Attribute settings
 *  \param  permissions   Attribute permissions
 *
 *  \return None.
 *
 *  \note   It is recommended this function only be used when no connections are open and the
 *          device is not in a connectable mode.
 */
/*************************************************************************************************/
void AttsDynAddAttr(void *pSvcHandle, const uint8_t *pUuid, const uint8_t *pValue, uint16_t len,
                    const uint16_t maxLen, uint8_t settings, uint8_t permissions)
{
  attsAttr_t *pAttr;
  attsDynGroupCb_t *pGroup = pSvcHandle;
  uint16_t handle = pGroup->group.startHandle + pGroup->currentAttr++;

  /* Verify inputs */
  WSF_ASSERT(handle <= pGroup->group.endHandle);
  WSF_ASSERT(pUuid);
  WSF_ASSERT(len <= maxLen);

  pAttr = pGroup->group.pAttr + (handle - pGroup->group.startHandle);


  /* Allocate a buffer for the length of the attribute */
  pAttr->pLen = attsDynAlloc(sizeof(uint16_t));
  WSF_ASSERT(pAttr->pLen);

  if (pAttr->pLen == NULL)
  {
    return;
  }

  /* Allocate a buffer for the value of the attribute */
  pAttr->pValue = attsDynAlloc(maxLen);
  WSF_ASSERT(pAttr->pValue);

  if (pAttr->pValue == NULL)
  {
    return;
  }

  /* Set the attribute values */
  pAttr->pUuid = pUuid;
  pAttr->maxLen = maxLen;
  pAttr->settings = settings;
  pAttr->permissions = permissions;



  if (pValue)
  {
    /* Copy the initial value */
    memcpy(pAttr->pValue, pValue, len);
    *pAttr->pLen = len;
  }
  else
  {
    /* No initial value, zero value and length */
    memset(pAttr->pValue, 0, maxLen);
    *pAttr->pLen = 0;
  }

  /* Add the service when the last attribute has been added */
  if (handle == pGroup->group.endHandle)
  {
    AttsAddGroup(&pGroup->group);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Dynamically add an attribute with a constant value to a dynamic ATT Services.
 *
 *  \param  pSvcHandle    Service handle returned by AttsDynCreateGroup
 *  \param  pUuid         Constant UUID
 *  \param  pValue        Pointer to constant attribute memory
 *  \param  len           Length of pValue in bytes
 *  \param  settings      Attribute settings
 *  \param  permissions   Attribute permissions
 *
 *  \return None.
 *
 *  \note   It is recommended this function only be used when no connections are open and the
 *          device is not in a connectable mode.
 */
/*************************************************************************************************/
void AttsDynAddAttrConst(void *pSvcHandle, const uint8_t *pUuid, const uint8_t *pValue,
                         const uint16_t len, uint8_t settings, uint8_t permissions)
{
  attsAttr_t *pAttr;
  attsDynGroupCb_t *pGroup = pSvcHandle;
  uint16_t handle = pGroup->group.startHandle + pGroup->currentAttr++;

  /* Verify inputs */
  WSF_ASSERT(handle <= pGroup->group.endHandle);
  WSF_ASSERT(pValue);
  WSF_ASSERT(pUuid);

  pAttr = pGroup->group.pAttr + (handle - pGroup->group.startHandle);

  /* Allocate a buffer for the length of the attribute */
  pAttr->pLen = attsDynAlloc(sizeof(uint16_t));
  WSF_ASSERT(pAttr->pLen);

  if (pAttr->pLen == NULL)
  {
    return;
  }

  /* Set the attribute values */
  *pAttr->pLen = len;
  pAttr->pUuid = pUuid;
  pAttr->pValue = (uint8_t *) pValue;
  pAttr->maxLen = len;
  pAttr->settings = settings;
  pAttr->permissions = permissions;

  /* Add the service when the last attribute has been added */
  if (handle == pGroup->group.endHandle)
  {
    AttsAddGroup(&pGroup->group);
  }
}
