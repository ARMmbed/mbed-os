/*************************************************************************************************/
/*!
 *  \file   wsf_queue.h
 *        
 *  \brief  General purpose queue service.
 *
 *          $Date: 2011-10-14 21:35:03 -0700 (Fri, 14 Oct 2011) $
 *          $Revision: 191 $
 *  
 *  Copyright (c) 2009 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/
#ifndef WSF_QUEUE_H
#define WSF_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Initialize a queue */
#define WSF_QUEUE_INIT(pQueue)          {(pQueue)->pHead = NULL; (pQueue)->pTail = NULL;}

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Queue structure */
typedef struct
{
  void      *pHead;         /*! head of queue */
  void      *pTail;         /*! tail of queue */
} wsfQueue_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     WsfQueueEnq
 *        
 *  \brief  Enqueue an element to the tail of a queue.
 *
 *  \param  pQueue    Pointer to queue.
 *  \param  pElem     Pointer to element.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfQueueEnq(wsfQueue_t *pQueue, void *pElem);

/*************************************************************************************************/
/*!
 *  \fn     WsfQueueDeq
 *        
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
 *  \fn     WsfQueuePush
 *        
 *  \brief  Push an element to the head of a queue.
 *
 *  \param  pQueue    Pointer to queue.
 *  \param  pElem     Pointer to element.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfQueuePush(wsfQueue_t *pQueue, void *pElem);

/*************************************************************************************************/
/*!
 *  \fn     WsfQueueInsert
 *        
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
 *  \fn     WsfQueueRemove
 *        
 *  \brief  Remove an element from a queue.  This function is typically used when iterating
 *          over a queue.
 *
 *  \param  pQueue    Pointer to queue.
 *  \param  pElem     Pointer to element to be removed.
 *  \param  pPrev     Pointer to previous element in the queue before element to be removed.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfQueueRemove(wsfQueue_t *pQueue, void *pElem, void *pPrev);

/*************************************************************************************************/
/*!
 *  \fn     WsfQueueCount
 *        
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
 *  \fn     WsfQueueEmpty
 *        
 *  \brief  Return TRUE if queue is empty.
 *
 *  \param  pQueue    Pointer to queue.
 *
 *  \return TRUE if queue is empty, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t WsfQueueEmpty(wsfQueue_t *pQueue);


#ifdef __cplusplus
};
#endif

#endif /* WSF_QUEUE_H */
