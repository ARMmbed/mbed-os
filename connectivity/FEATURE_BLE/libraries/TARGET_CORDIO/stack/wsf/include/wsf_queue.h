/*************************************************************************************************/
/*!
 *  \file   wsf_queue.h
 *
 *  \brief  General purpose queue service.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
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
#ifndef WSF_QUEUE_H
#define WSF_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WSF_QUEUE_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Initialize a queue */
#define WSF_QUEUE_INIT(pQueue)          {(pQueue)->pHead = NULL; (pQueue)->pTail = NULL;}

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief Queue structure */
typedef struct
{
  void      *pHead;         /*!< \brief head of queue */
  void      *pTail;         /*!< \brief tail of queue */
} wsfQueue_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Enqueue an element to the tail of a queue.
 *
 *  \param  pQueue    Pointer to queue.
 *  \param  pElem     Pointer to element.
 */
/*************************************************************************************************/
void WsfQueueEnq(wsfQueue_t *pQueue, void *pElem);

/*************************************************************************************************/
/*!
 *  \brief  Dequeue an element from the head of a queue.
 *
 *  \param  pQueue    Pointer to queue.
 *
 *  \return Pointer to element that has been dequeued or NULL if queue is empty.
 */
/*************************************************************************************************/
void *WsfQueueDeq(wsfQueue_t *pQueue);

/*************************************************************************************************/
/*!
 *  \brief  Push an element to the head of a queue.
 *
 *  \param  pQueue    Pointer to queue.
 *  \param  pElem     Pointer to element.
 */
/*************************************************************************************************/
void WsfQueuePush(wsfQueue_t *pQueue, void *pElem);

/*************************************************************************************************/
/*!
 *  \brief  Insert an element into a queue.  This function is typically used when iterating
 *          over a queue.
 *
 *  \param  pQueue    Pointer to queue.
 *  \param  pElem     Pointer to element to be inserted.
 *  \param  pPrev     Pointer to previous element in the queue before element to be inserted.
 *                    Note:  set pPrev to NULL if pElem is first element in queue.
 *  \return None.
 */
/*************************************************************************************************/
void WsfQueueInsert(wsfQueue_t *pQueue, void *pElem, void *pPrev);

/*************************************************************************************************/
/*!
 *  \brief  Remove an element from a queue.  This function is typically used when iterating
 *          over a queue.
 *
 *  \param  pQueue    Pointer to queue.
 *  \param  pElem     Pointer to element to be removed.
 *  \param  pPrev     Pointer to previous element in the queue before element to be removed.
 */
/*************************************************************************************************/
void WsfQueueRemove(wsfQueue_t *pQueue, void *pElem, void *pPrev);

/*************************************************************************************************/
/*!
 *  \brief  Count the number of elements in a queue.
 *
 *  \param  pQueue    Pointer to queue.
 *
 *  \return Number of elements in queue.
 */
/*************************************************************************************************/
uint16_t WsfQueueCount(wsfQueue_t *pQueue);

/*************************************************************************************************/
/*!
 *  \brief  Return TRUE if queue is empty.
 *
 *  \param  pQueue    Pointer to queue.
 *
 *  \return TRUE if queue is empty, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t WsfQueueEmpty(wsfQueue_t *pQueue);

/*************************************************************************************************/
/*!
 *  \brief  Check for a queue depth of 1 element.
 *
 *  \param  pQueue      Queue.
 *
 *  \return TRUE if Queue only has 1 element, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t WsfIsQueueDepthOne(wsfQueue_t *pQueue);

/*! \} */    /* WSF_QUEUE_API */

#ifdef __cplusplus
};
#endif

#endif /* WSF_QUEUE_H */
