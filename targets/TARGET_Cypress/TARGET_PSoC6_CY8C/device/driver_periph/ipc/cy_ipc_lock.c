/***************************************************************************//**
* \file
* \version 1.0
*
*  Description:
*   IPC Lock Driver - This source file contains the source code for the lock
*   level APIs for the IPC interface.
*
********************************************************************************
* Copyright2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_ipc_drv.h"
#include "cy_ipc_lock.h"


/* Assign the IPC structure used for Lock functions */
IPC_STRUCT_Type* CY_IPC_LOCK_Ptr = (IPC_STRUCT_Type*) &IPC->STRUCT[CY_IPC_CHAN_LOCK];       /**< Pointer to IPC structure used for locks */

struct CY_IPC_LOCK_STRUCT {
    uint32_t maxLocks;     /* Maximum locks in system */
    uint32_t *arrayPtr;    /* Pointer to lock array  */
} CY_IPC_LOCK_data;
typedef struct CY_IPC_LOCK_STRUCT CY_IPC_LOCK_STRUCT;


/*******************************************************************************
* Function Name: Cy_IPC_LOCK_Init
****************************************************************************//**
*
* This function initializes the lock subsystem.  The user must create an array of
* unsigned 32-bit words to hold the lock bits.  The amount of locks will be
* the size of the array * 32. The total lock count will always be a multiple of 32.
* Note that only one of the CPUs in a multi-CPU system should call this
* init function and provide a pointer to SRAM that can be shared between all
* the CPUs in the system that will use locks.
*
* \param count
*  The maximum number of locks to be supported (multiple of 32).
*
* \param memPtr
*  This points to the array of (count/32) words that contain the lock data.
*
* \return
*    CY_IPC_LOCK_SUCCESS: Sucessfully initialized
*    CY_IPC_LOCK_INIT_ERROR:  Init error, count value was zero, or greater than 31 and not a multiple of 32.
*
*
*******************************************************************************/
cy_en_ipclock_status_t Cy_IPC_LOCK_Init(uint32_t count, uint32_t memPtr[])
{
    extern   IPC_STRUCT_Type* CY_IPC_LOCK_Ptr;
    cy_en_ipclock_status_t retStatus = CY_IPC_LOCK_INIT_ERROR;
    uint32_t cnt;

    CY_IPC_LOCK_data.maxLocks = count;
    CY_IPC_LOCK_data.arrayPtr = memPtr;

    /* Initialize all locks to released */
    for(cnt= 0u ; cnt < (count/CY_IPC_LOCKS_PER_WORD); cnt++)
    {
         CY_IPC_LOCK_data.arrayPtr[cnt] = (uint32_t)0x00000000ul;
    }

    /* Make sure locks start out released  */
    (void) Cy_IPC_DRV_Release (CY_IPC_LOCK_Ptr, CY_IPC_NO_NOTIFIFICATION);

    /* Set the IPC Data with the pointer to the array. */

    if( CY_IPC_DRV_SUCCESS == Cy_IPC_DRV_SendMsgPtr (CY_IPC_LOCK_Ptr, CY_IPC_NO_NOTIFIFICATION, &CY_IPC_LOCK_data))
    {
        if(CY_IPC_DRV_SUCCESS == Cy_IPC_DRV_Release (CY_IPC_LOCK_Ptr, CY_IPC_NO_NOTIFIFICATION))
        {
            retStatus = CY_IPC_LOCK_SUCCESS;
        }
    }

    return(retStatus);
}

/*******************************************************************************
* Function Name: Cy_IPC_LOCK_Acquire
****************************************************************************//**
*
* This functions tries to acquire a lock.  If the lock is available, this
* function returns immediately with CY_IPC_LOCK_ERROR_NONE.  If the lock is not
* available, this function returns immediately with CY_IPC_LOCK_ERROR_LOCKED.
*
* \param lockNumber
*  The lock number to acquire.
*
* \return
*    CY_IPC_LOCK_SUCCESS:      The lock was acquired
*    CY_IPC_LOCK_LOCKED:       The lock channel is busy or locked by another process
*    CY_IPC_LOCK_NOT_ACQUIRED: Lock was already acquired
*    CY_IPC_LOCK_OUT_OF_RANGE: The lock number is not valid
*
*
*******************************************************************************/
cy_en_ipclock_status_t Cy_IPC_LOCK_Acquire(uint32_t lockNumber)
{
    uint32_t  lockIndex, lockMask;
    cy_en_ipclock_status_t   retStatus = CY_IPC_LOCK_LOCKED;
    CY_IPC_LOCK_STRUCT  *lockStructPtr;

    /* Check to make sure the Lock channel is released */
    /* If so, check is specific channel can be locked. */
    if(CY_IPC_DRV_SUCCESS == Cy_IPC_DRV_LockAcquire (CY_IPC_LOCK_Ptr))
    {
        /* Get the index into the lock array and calculate the mask */
        lockIndex = lockNumber / CY_IPC_LOCKS_PER_WORD;
        lockMask = (0x00000001ul) << (lockNumber - (lockIndex * CY_IPC_LOCKS_PER_WORD) );

        /* Get pointer to structure */
        lockStructPtr = (CY_IPC_LOCK_STRUCT *)CY_IPC_LOCK_Ptr->DATA;
        if(lockNumber < lockStructPtr->maxLocks)
        {
            if((lockStructPtr->arrayPtr[lockIndex] & lockMask) == (uint32_t)0)
            {
                lockStructPtr->arrayPtr[lockIndex] |= lockMask;
                retStatus = CY_IPC_LOCK_SUCCESS;
            }
            else
            {
                retStatus = CY_IPC_LOCK_NOT_ACQUIRED;
            }
        }
        else
        {
            retStatus = CY_IPC_LOCK_OUT_OF_RANGE;
        }
        /* Do not send an event on a lock acquire when the lock channel */
        /* is released, since the lock was just acquired.               */
        (void) Cy_IPC_DRV_Release (CY_IPC_LOCK_Ptr, CY_IPC_NO_NOTIFIFICATION);
    }

    return(retStatus);
}

/*******************************************************************************
* Function Name: Cy_IPC_LOCK_Status
****************************************************************************//**
*
* This function returns the status of any given lock.
*
* \param lockNumber
*  The index of the lock to return status.
*
* \return
*     CY_IPC_LOCK_STATUS_LOCKED:    The lock is in the acquired state. (return a 1)
*     CY_IPC_LOCK_STATUS_UNLOCKED:  The lock is in the released state. ( return 0)
*     CY_IPC_LOCK_OUT_OF_RANGE:     The lock number is not valid
*
*
*******************************************************************************/
cy_en_ipclock_status_t Cy_IPC_LOCK_Status(uint32_t lockNumber)
{
    cy_en_ipclock_status_t   retStatus;
    uint32_t  lockIndex, lockMask;
    CY_IPC_LOCK_STRUCT  *lockStructPtr;

    lockStructPtr = (CY_IPC_LOCK_STRUCT *)CY_IPC_LOCK_Ptr->DATA;
    if(lockNumber < lockStructPtr->maxLocks)
    {
        /* Get the index into the lock array and calculate the mask */
        lockIndex = lockNumber / CY_IPC_LOCKS_PER_WORD;
        lockMask = (0x00000001ul) << (lockNumber - (lockIndex * CY_IPC_LOCKS_PER_WORD) );

        if((lockStructPtr->arrayPtr[lockIndex] & lockMask) != (uint32_t)0)
        {
            retStatus =  CY_IPC_LOCK_STATUS_LOCKED;
        }
        else
        {
            retStatus =  CY_IPC_LOCK_STATUS_UNLOCKED;
        }
    }
    else
    {
        retStatus = CY_IPC_LOCK_OUT_OF_RANGE;
    }
    return(retStatus);
}


/*******************************************************************************
* Function Name: Cy_IPC_LOCK_Release
****************************************************************************//**
*
* This functions tries to releases a lock.
*
* \param lockNumber
*  The index of the lock in which to release.
*
* \return
*    CY_IPC_LOCK_SUCCESS:         The lock was released
*    CY_IPC_LOCK_NOT_ACQUIRED:    The lock was already released
*    CY_IPC_LOCK_LOCKED:          The lock channel was locked or busy
*    CY_IPC_LOCK_OUT_OF_RANGE:    The lock number is not valid
*
*
*******************************************************************************/
cy_en_ipclock_status_t Cy_IPC_LOCK_Release(uint32_t lockNumber)
{
    uint32_t  lockIndex, lockMask;
    cy_en_ipclock_status_t  retStatus = CY_IPC_LOCK_LOCKED;
    CY_IPC_LOCK_STRUCT  *lockStructPtr;

    /* Check to make sure the Lock channel is released */
    /* If so, check is specific channel can be locked. */
    if(CY_IPC_DRV_SUCCESS == Cy_IPC_DRV_LockAcquire (CY_IPC_LOCK_Ptr))
    {
        lockIndex = lockNumber / CY_IPC_LOCKS_PER_WORD;
        lockMask = (0x00000001ul) << (lockNumber - (lockIndex * CY_IPC_LOCKS_PER_WORD) );

        /* Get pointer to structure */
        lockStructPtr = (CY_IPC_LOCK_STRUCT *)CY_IPC_LOCK_Ptr->DATA;
        if(lockNumber < lockStructPtr->maxLocks)
        {
            if((lockStructPtr->arrayPtr[lockIndex] & lockMask) != (uint32_t)0u)
            {
                lockStructPtr->arrayPtr[lockIndex] &= ~lockMask;
                retStatus = CY_IPC_LOCK_SUCCESS;
            }
            else
            {
                retStatus = CY_IPC_LOCK_NOT_ACQUIRED;
            }
        }
        else
        {
            retStatus = CY_IPC_LOCK_OUT_OF_RANGE;
        }
        /* Release, but do not trigger a release event */
        (void) Cy_IPC_DRV_Release (CY_IPC_LOCK_Ptr, CY_IPC_NO_NOTIFIFICATION);
    }
    return(retStatus);
}

/*******************************************************************************
* Function Name: Cy_IPC_LOCK_GetMaxLocks
****************************************************************************//**
*
* This function returns the maximum lock count.
*
* \param
*
* \return
*     Returns the maximum lock count as configured.
*
*
*******************************************************************************/
uint32_t Cy_IPC_LOCK_GetMaxLocks(void)
{

    CY_IPC_LOCK_STRUCT  *lockStructPtr;

    lockStructPtr = (CY_IPC_LOCK_STRUCT *)CY_IPC_LOCK_Ptr->DATA;

    return(lockStructPtr->maxLocks);
}



/* [] END OF FILE */
