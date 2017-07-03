/***************************************************************************//**
* \file
* \version 1.0
*
*  Description:
*   IPC Pipe Driver - This source file includes code for the Pipe layer on top
*   of the IPC driver.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_ipc_pipe.h"

/* Define a pointer to array of endPoints. */
cy_stc_ipc_pipe_ep_t * cy_ipc_pipe_epArray;

/*******************************************************************************
* Function Name: Cy_IPC_PIPE_Config
****************************************************************************//**
*
* This function stores a copy of a pointer to the array of endpoints.  All
* access to endpoints will be via the index of the endpoint in this array.
*
* \param theEpArray
* This is the pointer to an array of endpoint structures that the designer
* created and will be used to reference all endpoints.
*
*
*  \return
*   void
*
*******************************************************************************/
void Cy_IPC_PIPE_Config(cy_stc_ipc_pipe_ep_t * theEpArray)
{
    /* Keep copy of this endpoint */
    cy_ipc_pipe_epArray = theEpArray;
}

/*******************************************************************************
* Function Name: Cy_IPC_PIPE_Init
****************************************************************************//**
*
* This function initializes the endpoint of a pipe for the current CPU. An
* endpoint of a pipe is IPC channel that receives a message for the current
* CPU.  The current CPU is the CPU that is executing the code.
* After this function is called, the callbackArray needs to be populated
* with the callback functions for that endpoint using the Cy_IPC_PIPE_RegisterCallback()
* function.
*
* \param epAddr
* This parameter is the address (or index in the array of endpoint structure)
* that designates the endpoint in which you want to initialize.
*
* \param cbArray
* This is a pointer to the callback function array.  Based on the client ID, one
* of the functions in this array is called to process the message.
*
* \param cbCnt
* This is the size of the callback array, or the number of defined clients.
*
* \param epConfig
* This value defines the IPC channel, IPC interrupt number, and the interrupt
* mask for the entire pipe.
* The format of the endPoint configuration
*    Bits[31:16] Interrupt Mask
*    Bits[15:8 ] IPC interrupt
*    Bits[ 7:0 ] IPC channel
*
*  \return
*   void
*
*******************************************************************************/
void Cy_IPC_PIPE_Init(uint32_t epAddr, cy_ipc_pipe_callback_array_ptr_t cbArray, uint32_t cbCnt, uint32_t epConfig)
{
    cy_stc_ipc_pipe_ep_t * endPoint;

    endPoint = &cy_ipc_pipe_epArray[epAddr];

    /* Extract the channel, interrupt and interrupt mask */
    endPoint->ipcChan     = (epConfig & CY_IPC_PIPE_CFG_CHAN_MASK)  >> CY_IPC_PIPE_CFG_CHAN_SHIFT ;
    endPoint->intrChan    = (epConfig & CY_IPC_PIPE_CFG_INTR_MASK)  >> CY_IPC_PIPE_CFG_INTR_SHIFT;
    endPoint->pipeIntMask = (epConfig & CY_IPC_PIPE_CFG_IMASK_MASK) >> CY_IPC_PIPE_CFG_IMASK_SHIFT;

    /* Assign IPC channel to this endpoint */
    endPoint->ipcPtr   = Cy_IPC_DRV_GetIpcBaseAddress (endPoint->ipcChan);

    /* Assign interrupt structure to endpoint and Initialize the interrupt mask for this endpoint */
    endPoint->ipcIntrPtr = Cy_IPC_DRV_GetIntrBaseAddr(endPoint->intrChan);
    endPoint->ipcIntrPtr->INTR_MASK = endPoint->pipeIntMask | (endPoint->pipeIntMask << 16); /* Only allow notify and release interrupts */
                                                                                              /* from endpoints in this pipe.         */
    /* Save the Client count and the callback array pointer */
    endPoint->clientCount   = cbCnt;
    endPoint->callbackArray = cbArray;
    endPoint->busy = CY_IPC_PIPE_ENDPOINT_NOTBUSY;
}


/*******************************************************************************
* Function Name: Cy_IPC_PIPE_SendMessage
****************************************************************************//**
*
* This function is used to send a message from one endpoint to another.  It
* generates an interrupt on the endpoint that receives the message and a
* release interrupt to the sender to acknowledge the message has been processed.
*
* \param toAddr
* This parameter is the address (or index in the array of endpoint structures)
* of the endpoint to which you are sending the message.
*
* \param fromAddr
* This parameter is the address (or index in the array of endpoint structures)
* of the endpoint to which the message is being sent.
*
* \param msgPtr
* Pointer to the message structure to be sent.
*
* \param callBackPtr
* Pointer to the Release callback function.
*
*
*  \return
*    CY_IPC_PIPE_SUCCESS:     Message was sent to the other end of the pipe
*    CY_IPC_PIPE_BAD_HANDLE:  The handle provided for the pipe was not valid
*    CY_IPC_PIPE_SEND_BUSY:   The pipe is already busy sending a message
*    CY_IPC_PIPE_DIR_ERROR:   Tried to send on the "to" end of a unidirectional pipe
*******************************************************************************/
cy_en_ipc_pipe_status_t Cy_IPC_PIPE_SendMessage(uint32_t toAddr, uint32_t fromAddr, void * msgPtr, cy_ipc_pipe_relcallback_ptr_t callBackPtr)
{
    cy_en_ipc_pipe_status_t  returnStatus;
    uint32_t releaseMask;
    uint32_t notifyMask;

    cy_stc_ipc_pipe_ep_t * fromEp;
    cy_stc_ipc_pipe_ep_t * toEp;

    toEp   = &(cy_ipc_pipe_epArray[toAddr]);
    fromEp = &cy_ipc_pipe_epArray[fromAddr];

    /* Check if IPC channel valid */
    if( toEp->ipcPtr != (void *)0u)
    {
        if(fromEp->busy == CY_IPC_PIPE_ENDPOINT_NOTBUSY)
        {
            /* Attempt to acquire the channel */
            if( CY_IPC_DRV_SUCCESS == Cy_IPC_DRV_LockAcquire(toEp->ipcPtr) )
            {
                /* Create the release mask for the "fromAddr" channel's interrupt channel */
                releaseMask =  (0x0001 << fromEp->intrChan);

                * (uint32_t *) msgPtr &= ~(CY_IPC_PIPE_MSG_RELEASE_MASK);                /* Mask out the release mask area */
                releaseMask = ((releaseMask << CY_IPC_PIPE_MSG_RELEASE_SHIFT) & CY_IPC_PIPE_MSG_RELEASE_MASK);   /* shift into position */
                * (uint32_t *) msgPtr |= releaseMask;    /* OR in the release mask */

                 /* If the channel was acquired, write the message.   */
                toEp->ipcPtr->DATA = (uint32_t) msgPtr;

                /* The last thing to do is create the notify event that causes the interrupt */
                /* Create the notify mask for the "toAddr" channel's interrupt channel */
                notifyMask =  (0x0001 << toEp->intrChan);

                fromEp->busy = CY_IPC_PIPE_ENDPOINT_BUSY;  /* Set the busy flag.  The ISR clears this after the release */

                /* Setup release callback function */
                fromEp->releaseCallbackPtr = callBackPtr;               /* Set the callback function  */

                /* Cause notify event/interrupt */
                toEp->ipcPtr->NOTIFY = (IPC_STRUCT_NOTIFY_INTR_NOTIFY_Msk & notifyMask);
                returnStatus = CY_IPC_PIPE_SUCCESS;
            }
            else
            {
                /* Channel was already acquired, return Error */
                returnStatus = CY_IPC_PIPE_ERROR_SEND_BUSY;
            }
        }
        else
        {
            /* Channel may not be acquired, but the release interrupt has not executed yet */
            returnStatus = CY_IPC_PIPE_ERROR_SEND_BUSY;
        }
    }
    else
    {
        /* Null pipe handle. */
        returnStatus = CY_IPC_PIPE_ERROR_BAD_HANDLE;
    }
    return(returnStatus);
}



/*******************************************************************************
* Function Name: Cy_IPC_PIPE_RegisterCallback
****************************************************************************//**
*
* This function registers a callback when a message is received on a pipe.
* The client_ID is the same as the index of the callback function array.  The callback
* may be a real function pointer or NULL if no callback is required.
*
* \param epAddr
* This parameter is the address (or index in the array of endpoint structures)
* that designates the endpoint to which you want to add callback functions.
*
* \param callBackPtr
* Pointer to the callback function called when the endpoint has received a message.
*
* \param clientId
* The index in the callback array (Client ID) where the function pointer is saved.
*
*
*  \return
*    CY_IPC_PIPE_SUCCESS:           Callback registered successfully
*    CY_IPC_PIPE_ERROR_BAD_CLIENT:  Client ID out of range, callback not registered.
*******************************************************************************/
cy_en_ipc_pipe_status_t Cy_IPC_PIPE_RegisterCallback(uint32_t epAddr, cy_ipc_pipe_callback_ptr_t callBackPtr,  uint32_t clientId)
{
    cy_en_ipc_pipe_status_t returnStatus;   /* Return Status */
    cy_stc_ipc_pipe_ep_t * thisEp;

    thisEp = &cy_ipc_pipe_epArray[epAddr];

    /* Check if clientId is between 0 and less than client count */
    if(clientId <= thisEp->clientCount)
    {
        /* Copy callback function into callback function pointer array */
        thisEp->callbackArray[clientId] = callBackPtr;
        returnStatus = CY_IPC_PIPE_SUCCESS;
    }
    else
    {
        returnStatus = CY_IPC_PIPE_ERROR_BAD_CLIENT;
    }
    return(returnStatus);
}

/*******************************************************************************
* Function Name: Cy_IPC_PIPE_RegisterRecvCallbackRel
****************************************************************************//**
*
* This function registers a default callback if a release interrupt
* is generated but the current release callback function is null.
*
*
* \param epAddr
* This parameter is the address (or index in the array of endpoint structures)
* that designates the endpoint to which you want to add a release callback function.
*
* \param callBackPtr
* Pointer to the callback executed when the endpoint has received a message.
*
*  \return
*    None
*******************************************************************************/
void Cy_IPC_PIPE_RegisterCallbackRel(uint32_t epAddr, cy_ipc_pipe_relcallback_ptr_t callBackPtr)
{
    cy_stc_ipc_pipe_ep_t * endPoint;

    endPoint = &cy_ipc_pipe_epArray[epAddr];
    /* Copy callback function into callback function pointer array */
    endPoint->defaultReleaseCbPtr = callBackPtr;
}

/*******************************************************************************
* Function Name: Cy_IPC_PIPE_ExecCallback
****************************************************************************//**
*
* This function is called by the ISR for a given pipe endpoint to dispatch
* the appropriate callback function based on the client ID for that endpoint.
*
* \param endPoint
* Pointer to endpoint structure.
*
*  \return
*  None
*******************************************************************************/
void Cy_IPC_PIPE_ExecCallback(cy_stc_ipc_pipe_ep_t * endPoint)
{
    uint32_t * msgPtr = (void *)0uL;
    uint32_t clientID;
    uint32_t shadowIntr;
    uint32_t releaseMask = (uint32_t)0;
    cy_ipc_pipe_callback_ptr_t callbackPtr;

    /* Check to make sure the interrupt was a notify interrupt */
    shadowIntr = endPoint->ipcIntrPtr->INTR_MASKED;
    if( 0 != (shadowIntr & IPC_INTR_STRUCT_INTR_MASK_NOTIFY_Msk))
    {
        /* Clear the notify interrupt.  */
        endPoint->ipcIntrPtr->INTR = (shadowIntr & IPC_INTR_STRUCT_INTR_MASK_NOTIFY_Msk);

        if( CY_IPC_DRV_LOCKED == Cy_IPC_DRV_GetLockStatus (endPoint->ipcPtr) )
        {
            /* Extract Client ID  */
            if( CY_IPC_DRV_SUCCESS == Cy_IPC_DRV_ReadMsgPtr (endPoint->ipcPtr, (void **)&msgPtr))
            {
                /* Get release mask */
                releaseMask = (uint32_t)((*msgPtr & CY_IPC_PIPE_MSG_RELEASE_MASK) >> CY_IPC_PIPE_MSG_RELEASE_SHIFT);

                clientID = *msgPtr & CY_IPC_PIPE_MSG_CLIENT_MASK;
                /* Make sure client ID is within valid range */
                if(endPoint->clientCount > clientID)
                {
                    callbackPtr = endPoint->callbackArray[clientID];  /* Get the callback function */
                    if(callbackPtr != (void *)0)
                    {
                      callbackPtr(msgPtr);   /* Call the function pointer for "clientID" */
                    }
                }
            }
            /* Must always release the IPC channel */
            Cy_IPC_DRV_Release (endPoint->ipcPtr, releaseMask);
        }
    }
    /* Check to make sure the interrupt was a release interrupt */
    if( 0 != (shadowIntr & IPC_INTR_STRUCT_INTR_MASK_RELEASE_Msk))  /* Check for a Release interrupt */
    {
        /* Clear the release callback function  */
        endPoint->ipcIntrPtr->INTR = (shadowIntr & IPC_INTR_STRUCT_INTR_MASK_RELEASE_Msk);

        if(endPoint->releaseCallbackPtr != (void *)0)
        {
            endPoint->releaseCallbackPtr();

            /* Clear the pointer after it was called */
            endPoint->releaseCallbackPtr = (void *)0;
        }
        else if( endPoint->defaultReleaseCbPtr != (void *)0)
        {
            endPoint->defaultReleaseCbPtr();
        }
        /* Clear the busy flag when release is detected */
        endPoint->busy = CY_IPC_PIPE_ENDPOINT_NOTBUSY;
    }

    (void)endPoint->ipcIntrPtr->INTR;
}



/* [] END OF FILE */

