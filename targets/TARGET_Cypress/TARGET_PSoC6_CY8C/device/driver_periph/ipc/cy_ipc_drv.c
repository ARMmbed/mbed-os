/***************************************************************************//**
* \file
* \version 1.0
*
*  \breif
*   IPC Driver - This source file contains the low level driver code for
*   the IPC hardware.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_ipc_drv.h"


/*******************************************************************************
* Function Name: Cy_IPC_DRV_LockAcquire
****************************************************************************//**
*
* This function is used to acquire the IPC lock. The function acquires the lock corresponding to the ipcPtr passed.
*
* \param ipcPtr
* This parameter is a handle which represents the base address of the registers of the IPC channel.
* The parameter is generally returned from a call to the \ref Cy_IPC_DRV_GetIpcBaseAddress
*
* \return
*   Status for the function:
*   CY_IPC_DRV_SUCCESS: The IPC was successfully acquired
*   CY_IPC_DRV_ERROR: The IPC was not acquired since it was already acquired by another master
*
*******************************************************************************/
cy_en_ipcdrv_status_t Cy_IPC_DRV_LockAcquire (IPC_STRUCT_Type const * ipcPtr)
{
    cy_en_ipcdrv_status_t retStatus;

    if( 0ul != (IPC_STRUCT_ACQUIRE_SUCCESS_Msk & ipcPtr->ACQUIRE))
    {
        retStatus = CY_IPC_DRV_SUCCESS;
    }
    else
    {
        retStatus = CY_IPC_DRV_ERROR;
    }
    return(retStatus);
}

/*******************************************************************************
* Function Name: Cy_IPC_DRV_GetLockStatus
****************************************************************************//**
*
* The function is used to read the lock status of an IPC channel. The function tells the reader if the IPC was in the locked or released state.
*
* \param ipcPtr
* This parameter is a handle which represents the base address of the registers of the IPC channel.
* The parameter is generally returned from a call to the \ref Cy_IPC_DRV_GetIpcBaseAddress
*
* \return
*   Status for the function:
*   CY_IPC_DRV_LOCKED: The IPC is in the Locked state
*   CY_IPC_DRV_RELEASED: The IPC is in the Released state
*
*******************************************************************************/
cy_en_ipcdrv_status_t  Cy_IPC_DRV_GetLockStatus (IPC_STRUCT_Type const * ipcPtr)
{
    cy_en_ipcdrv_status_t retStatus;

    if( IPC_STRUCT_ACQUIRE_SUCCESS_Msk == (IPC_STRUCT_ACQUIRE_SUCCESS_Msk & ipcPtr->LOCK_STATUS) )
    {
        retStatus = CY_IPC_DRV_LOCKED;
    }
    else
    {
        retStatus = CY_IPC_DRV_RELEASED;
    }
    return(retStatus);
}


/*******************************************************************************
* Function Name: Cy_IPC_DRV_Release
****************************************************************************//**
*
* The function is used to release an IPC from the locked state.
* The function also has a way to specify through a parameter, which IPC interrupts need to be notified during the release event.
*
* \param ipcPtr
* This parameter is a handle which represents the base address of the registers of the IPC channel.
* The parameter is generally returned from a call to the \ref Cy_IPC_DRV_GetIpcBaseAddress
*
* \param releaseEventIntr
* Bit encoded list of IPC interrupt lines that are triggered by a release event
*
* \return
*   Status for the function:
*   CY_IPC_DRV_SUCCESS: The function executed successfully and the IPC was released
*   CY_IPC_DRV_NOT_ACQUIRED: The IPC channel was not acquired before the function call.
*
*******************************************************************************/
cy_en_ipcdrv_status_t  Cy_IPC_DRV_Release (IPC_STRUCT_Type* ipcPtr, uint32_t releaseEventIntr)
{
    cy_en_ipcdrv_status_t retStatus;

   /* Check to make sure the IPC is Acquired */
   if( IPC_STRUCT_ACQUIRE_SUCCESS_Msk == ( IPC_STRUCT_ACQUIRE_SUCCESS_Msk & ipcPtr->LOCK_STATUS) )
   {
        /* The IPC was acquired, release the IPC channel */
        ipcPtr->RELEASE = (IPC_INTR_STRUCT_INTR_RELEASE_Msk & releaseEventIntr);
        retStatus = CY_IPC_DRV_SUCCESS;
    }
    else   /* The IPC channel was already released (not acquired) */
    {
       retStatus = CY_IPC_DRV_NOT_ACQUIRED;
    }

    return(retStatus);
}



/*******************************************************************************
* Function Name: Cy_IPC_DRV_Notify
****************************************************************************//**
*
* The function generates a notify event to an IPC interrupt structure.
*
* \param ipcPtr
* This parameter is a handle which represents the base address of the registers of the IPC channel.
* The parameter is generally returned from a call to the \ref Cy_IPC_DRV_GetIpcBaseAddress
*
* \param notifyEventIntr
* Bit encoded list of IPC interrupt lines that are triggered by a notification
*
* \return
*  void
*
*******************************************************************************/
void  Cy_IPC_DRV_Notify (IPC_STRUCT_Type* ipcPtr, uint32_t notifyEventIntr)
{
      ipcPtr->NOTIFY = (IPC_STRUCT_NOTIFY_INTR_NOTIFY_Msk & notifyEventIntr);
}

/*******************************************************************************
* Function Name: Cy_IPC_DRV_SendMsgWord
****************************************************************************//**
*
* This function is used to send a 32 bit word message through an IPC channel.
* The function also has an associated notification field that will let the message notify one or multiple IPC interrupts.
* The IPC channel is locked and remains locked after the function returns.  The receiver
* of the message should release the channel.
*
* \param ipcPtr
* This parameter is a handle which represents the base address of the registers of the IPC channel.
* The parameter is generally returned from a call to the \ref Cy_IPC_DRV_GetIpcBaseAddress.
*
* \param notifyEventIntr
* Bit encoded list of IPC interrupt lines that are triggered by a notification.
*
* \param message
* The message word that is the data placed in the IPC data register.
*
* \return
*   Status for the function:
*   CY_IPC_DRV_SUCCESS: The send operation was successful
*   CY_IPC_DRV_ERROR: The IPC channel is unavailable since it is already locked.
*
*******************************************************************************/
cy_en_ipcdrv_status_t  Cy_IPC_DRV_SendMsgWord (IPC_STRUCT_Type* ipcPtr, uint32_t notifyEventIntr, uint32_t message)
{
    cy_en_ipcdrv_status_t retStatus;

    if( CY_IPC_DRV_SUCCESS == Cy_IPC_DRV_LockAcquire(ipcPtr) )
    {
        /* If the channel was acquired, send the message.   */
        ipcPtr->DATA = message;
        ipcPtr->NOTIFY = (IPC_STRUCT_NOTIFY_INTR_NOTIFY_Msk & notifyEventIntr);
        retStatus = CY_IPC_DRV_SUCCESS;
    }
    else
    {
        /* Channel was already acquired, return Error */
        retStatus = CY_IPC_DRV_ERROR;
    }
    return(retStatus);
}


/*******************************************************************************
* Function Name: Cy_IPC_DRV_ReadMsgWord
****************************************************************************//**
*
* This function is used to read a 32 bit word message through an IPC channel.
* This function assumes that the channel is locked (in order for a valid message).  If
* the channel is not locked the message is invalid.  The user needs to call
* Cy_IPC_DRV_Release() function after reading the message, to release the lock.
*
* \param ipcPtr
* This parameter is a handle which represents the base address of the registers of the IPC channel.
* The parameter is generally returned from a call to the \ref Cy_IPC_DRV_GetIpcBaseAddress.
*
* \param message
*  A variable where the read data is copied.
*
* \return
*  Status for the function
*   CY_IPC_DRV_SUCCESS: The function executed successfully and the IPC was released.
*   CY_IPC_DRV_ERROR: The function encountered an error since the IPC channel was already in a released state meaning the data may be invalid
*
*******************************************************************************/
cy_en_ipcdrv_status_t  Cy_IPC_DRV_ReadMsgWord (IPC_STRUCT_Type* ipcPtr, uint32_t * message)
{
    cy_en_ipcdrv_status_t retStatus;

    if( CY_IPC_DRV_LOCKED == Cy_IPC_DRV_GetLockStatus(ipcPtr) )
    {
        /* The channel is locked, message is valid. */
        *message = ipcPtr->DATA;
        retStatus = CY_IPC_DRV_SUCCESS;
    }
    else
    {
        /* The channel is not locked so channel is invalid. */
        retStatus = CY_IPC_DRV_ERROR;
    }
    return(retStatus);
}

/*******************************************************************************
* Function Name: Cy_IPC_DRV_SendMsgPtr
****************************************************************************//**
*
* This function is used to send a message pointer through an IPC channel.
* The message structure may hold a generic pointer which may contain the address of
* any user data type or structure.  This parameter could be a pointer to a 32 bit integer, an
* array or even a data structure defined in the user code. This function acts
* as a transfer engine for sending the pointer. Any memory management of the pointer
* allocation and deallocation is up to the application code.
* The function also has an associated notification field that will let the message
* notify one or multiple interrupts.
*
* \param ipcPtr
* This parameter is a handle which represents the base address of the registers of the IPC channel.
* The parameter is generally returned from a call to the \ref Cy_IPC_DRV_GetIpcBaseAddress
*
* \param notifyEventIntr
* Bit encoded list of IPC interrupt lines that are triggered during the release action.
*
* \param msgPtr
* The message pointer that is being sent over the IPC channel
*
* \return
*   Status for the function:
*   CY_IPC_DRV_SUCCESS: The send operation was successful
*   CY_IPC_DRV_ERROR: The IPC channel is unavailable since it is already locked
*
*******************************************************************************/
cy_en_ipcdrv_status_t  Cy_IPC_DRV_SendMsgPtr(IPC_STRUCT_Type* ipcPtr, uint32_t notifyEventIntr, void const * msgPtr)
{
    cy_en_ipcdrv_status_t retStatus;

    if( CY_IPC_DRV_SUCCESS == Cy_IPC_DRV_LockAcquire(ipcPtr) )
    {
        /* If the channel was acquired, send the message. */
        ipcPtr->DATA = (uint32_t)msgPtr;
        ipcPtr->NOTIFY = ((uint32_t)IPC_STRUCT_NOTIFY_INTR_NOTIFY_Msk & notifyEventIntr);
        retStatus = CY_IPC_DRV_SUCCESS;
    }
    else
    {
        /* Chaneel was already acquired, Error */
        retStatus = CY_IPC_DRV_ERROR;
    }
    return(retStatus);
}

/*******************************************************************************
* Function Name: Cy_IPC_DRV_ReadMsgPtr
****************************************************************************//**
*
* This function is used to read a 32 bit pointer message through an IPC channel.
*
* \param ipcPtr
* This parameter is a handle which represents the base address of the registers of the IPC channel.
* The parameter is generally returned from a call to the \ref Cy_IPC_DRV_GetIpcBaseAddress
*
* \param msgPtr
* Pointer variable to hold the data pointer that is being read from the IPC channel
*
*
* \return
*  Status for the function
*   CY_IPC_DRV_SUCCESS: The function executed successfully and the IPC was released
*   CY_IPC_DRV_ERROR: The function encountered an error since the IPC channel was already in a released state meaning the data in it is invalid
*
*******************************************************************************/
cy_en_ipcdrv_status_t  Cy_IPC_DRV_ReadMsgPtr (IPC_STRUCT_Type* ipcPtr, void ** msgPtr)
{
    cy_en_ipcdrv_status_t retStatus;

    if( CY_IPC_DRV_LOCKED == Cy_IPC_DRV_GetLockStatus(ipcPtr) )
    {
        /* The channel is locked, message is valid. */
        *msgPtr = (void *)(ipcPtr->DATA);
        retStatus = CY_IPC_DRV_SUCCESS;
    }
    else
    {
        /* The channel is not locked so channel is invalid. */
        retStatus = CY_IPC_DRV_ERROR;
    }
    return(retStatus);
}


/*******************************************************************************
* Function Name: Cy_IPC_DRV_SetIntrMask
****************************************************************************//**
*
* This function is used to set the interrupt mask for an IPC Interrupt.
* The mask can be set for release or notification events of all available IPC ports.
*
* \param ipcIntrPtr
* This is a handle to the IPC interrupt. This handle can be calculated from the IPC interrupt number using \ref Cy_IPC_DRV_GetIntrBaseAddr
*
* \param ipcReleaseMask
* An encoded list of all IPC structures that can trigger the interrupt on a release event
*
* \param ipcNotifyMask
* An encoded list of all IPC structures that can trigger the interrupt on a notify event.
*
* \return
*   void
*
*******************************************************************************/
void  Cy_IPC_DRV_SetIntrMask (IPC_INTR_STRUCT_Type* ipcIntrPtr, uint32_t ipcReleaseMask, uint32_t ipcNotifyMask)
{
    ipcIntrPtr->INTR_MASK = ( ipcNotifyMask << IPC_INTR_STRUCT_INTR_NOTIFY_Pos) | ( ipcReleaseMask & IPC_INTR_STRUCT_INTR_MASK_RELEASE_Msk);
}


/*******************************************************************************
* Function Name: Cy_IPC_DRV_GetIntrMask
****************************************************************************//**
*
* This function is used to read the interrupt mask.

* \param ipcIntrPtr
* This is a handle to the IPC interrupt. This handle can be calculated from the IPC interrupt number using \ref Cy_IPC_DRV_GetIntrBaseAddr
*
* \return
*   The return value is encoded as follows
*   <table>
*   <tr><th>Interrupt sources   <th>Value
*   <tr><td>Ipc_PORTX_RELEASE   <td>Xth bit set
*   <tr><td>Ipc_PORTX_NOTIFY    <td>X+16 th bit set
*   </table>
*
*******************************************************************************/
uint32_t Cy_IPC_DRV_GetIntrMask(IPC_INTR_STRUCT_Type const * ipcIntrPtr)
{
    return(ipcIntrPtr->INTR_MASK);
}

/*******************************************************************************
* Function Name: Cy_IPC_DRV_GetIntrStatusMasked
****************************************************************************//**
*
* This function is used to read the active unmasked interrupt. This function can be used in the interrupt service routine to
* find which source triggered the interrupt.
*
* \param ipcIntrPtr
* This is a handle to the IPC interrupt. This handle can be calculated from the IPC interrupt number using \ref Cy_IPC_DRV_GetIntrBaseAddr
*
* \return
*   The return value is encoded as follows
*   <table>
*   <tr><th>Interrupt sources   <th>Value
*   <tr><td>Ipc_PORTX_RELEASE   <td>Xth bit set
*   <tr><td>Ipc_PORTX_NOTIFY    <td>X+16 th bit set
*   </table>
*
*******************************************************************************/
uint32_t Cy_IPC_DRV_GetIntrStatusMasked (IPC_INTR_STRUCT_Type const * ipcIntrPtr)
{
    return(ipcIntrPtr->INTR_MASKED);
}

/*******************************************************************************
* Function Name: Cy_IPC_DRV_GetIntrStatus
****************************************************************************//**
*
* This function is used to read the pending interrupts. Note that this read is an unmasked read of the interrupt status
* Interrupt sources read as active by this function would generate interrupts only if they were not masked

* \param ipcIntrPtr
* This is a handle to the IPC interrupt. This handle can be calculated from the IPC interrupt number using \ref Cy_IPC_DRV_GetIntrBaseAddr
*
* \return
*   The return value is encoded as follows
*   <table>
*   <tr><th>Interrupt sources   <th>Value
*   <tr><td>Ipc_PORTX_RELEASE   <td>Xth bit set
*   <tr><td>Ipc_PORTX_NOTIFY    <td>X+16 th bit set
*   </table>
*
*******************************************************************************/
uint32_t Cy_IPC_DRV_GetIntrStatus(IPC_INTR_STRUCT_Type const * ipcIntrPtr)
{
    return(ipcIntrPtr->INTR);
}

/*******************************************************************************
* Function Name: Cy_IPC_DRV_SetIntr
****************************************************************************//**
*
* This function is used to set the interrupt source. This function can be used to activate interrupts through software.
* Note that interrupt sources set using this interrupt would generate interrupts only if they are not masked
*
* \param ipcIntrPtr
* This is a handle to the IPC interrupt. This handle can be calculated from the IPC interrupt number using \ref Cy_IPC_DRV_GetIntrBaseAddr
*
* \param ipcReleaseMask
* An encoded list of all IPC structures that can trigger the interrupt on a release event
*
* \param ipcNotifyMask
* An encoded list of all IPC structures that can trigger the interrupt on a notify event.
*
* \return
*   void
*
*******************************************************************************/
void  Cy_IPC_DRV_SetIntr(IPC_INTR_STRUCT_Type* ipcIntrPtr, uint32_t ipcReleaseMask, uint32_t ipcNotifyMask)
{
    ipcIntrPtr->INTR_SET =  (ipcNotifyMask << IPC_INTR_STRUCT_INTR_NOTIFY_Pos)  | ( ipcReleaseMask & IPC_INTR_STRUCT_INTR_MASK_RELEASE_Msk)  ;
}

/*******************************************************************************
* Function Name: Cy_IPC_DRV_ClearIntr
****************************************************************************//**
*
* This function is used to clear the interrupt source. Use this function to clear a pending interrupt source in the interrupt status
*
* \param ipcIntrPtr
* This is a handle to the IPC interrupt. This handle can be calculated from the IPC interrupt number using \ref Cy_IPC_DRV_GetIntrBaseAddr
*
* \param ipcReleaseMask
* An encoded list of all IPC structures that can trigger the interrupt on a release event
*
* \param ipcNotifyMask
* An encoded list of all IPC structures that can trigger the interrupt on a notify event.
*
* \return
*   void
*
*******************************************************************************/
void  Cy_IPC_DRV_ClearIntr(IPC_INTR_STRUCT_Type* ipcIntrPtr, uint32_t ipcReleaseMask, uint32_t ipcNotifyMask)
{
     ipcIntrPtr->INTR =  (ipcNotifyMask << IPC_INTR_STRUCT_INTR_NOTIFY_Pos) | ( ipcReleaseMask & IPC_INTR_STRUCT_INTR_MASK_RELEASE_Msk)  ;
     (void)ipcIntrPtr->INTR;  /* Read the register to flush the cache */
}

/* [] END OF FILE */
