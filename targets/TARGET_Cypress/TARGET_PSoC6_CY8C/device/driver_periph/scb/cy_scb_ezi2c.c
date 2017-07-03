/***************************************************************************//**
* \file cy_scb_ezi2c.c
* \version 1.0
*
* Provides EZI2C API implementation of the SCB driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_scb_ezi2c.h"

#if defined(__cplusplus)
extern "C" {
#endif


/***************************************
*        Function Prototypes
***************************************/

static void HandleErrors      (CySCB_Type *base, cy_stc_scb_ezi2c_context_t *context);
static void HandleAddress     (CySCB_Type *base, cy_stc_scb_ezi2c_context_t *context);
static void UpdateRxFifoLevel (CySCB_Type *base, uint32_t bufSize);
static void HandleDataReceive (CySCB_Type *base, cy_stc_scb_ezi2c_context_t *context);
static void HandleDataTransmit(CySCB_Type *base, cy_stc_scb_ezi2c_context_t *context);
static void HandleStop        (CySCB_Type *base, cy_stc_scb_ezi2c_context_t *context);
static void UpdateAddressMask (CySCB_Type *base, cy_stc_scb_ezi2c_context_t const *context);


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_Init
****************************************************************************//**
*
* Initializes the SCB for the EZI2C operation.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param config
* The pointer to the configuration structure \ref cy_stc_scb_ezi2c_config_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user should not modify anything
* in this structure.
*
* \return
* \ref cy_en_scb_ezi2c_status_t
*
* \note
* Ensure that the SCB block is disabled before calling this function.
*
*******************************************************************************/
cy_en_scb_ezi2c_status_t Cy_SCB_EZI2C_Init(CySCB_Type *base, cy_stc_scb_ezi2c_config_t const *config,
                                           cy_stc_scb_ezi2c_context_t *context)
{
    cy_en_scb_ezi2c_status_t retStatus = CY_SCB_EZI2C_BAD_PARAM;

    if ((NULL != base) && (NULL != config) && (NULL != context))
    {
        /* Configure the EZI2C interface */
        base->CTRL = ((config->numberOfAddresses == CY_SCB_EZI2C_TWO_ADDRESSES) ? SCB_CTRL_ADDR_ACCEPT_Msk : 0UL) |
                      (config->enableWakeFromSleep ? SCB_CTRL_EC_AM_MODE_Msk  : 0UL) |
                       SCB_CTRL_BYTE_MODE_Msk;

        base->I2C_CTRL = CY_SCB_EZI2C_I2C_CTRL;

        /* Configure the RX direction */
        base->RX_CTRL      = CY_SCB_EZI2C_RX_CTRL;
        base->RX_FIFO_CTRL = 0UL;

        /* Set the default address and mask */
        if (config->numberOfAddresses == CY_SCB_EZI2C_ONE_ADDRESS)
        {
            context->address2 = 0U;
            Cy_SCB_EZI2C_SetAddress1(base, config->slaveAddress1, context);
        }
        else
        {
            Cy_SCB_EZI2C_SetAddress1(base, config->slaveAddress1, context);
            Cy_SCB_EZI2C_SetAddress2(base, config->slaveAddress2, context);
        }

        /* Configure the TX direction */
        base->TX_CTRL      = CY_SCB_EZI2C_TX_CTRL;
        base->TX_FIFO_CTRL = CY_SCB_EZI2C_HALF_FIFO_SIZE;

        /* Configure interrupt sources */
        base->INTR_SPI_EC_MASK = 0UL;
        base->INTR_I2C_EC_MASK = 0UL;
        base->INTR_RX_MASK     = 0UL;
        base->INTR_TX_MASK     = 0UL;
        base->INTR_M_MASK      = 0UL;
        base->INTR_S_MASK      = CY_SCB_EZI2C_SLAVE_INTR;

        /* Initialize context */
        context->status = 0UL;
        context->state  = CY_SCB_EZI2C_STATE_IDLE;

        context->subAddrSize = config->subAddressSize;

        context->buf1Size      = 0UL;
        context->buf1rwBondary = 0UL;
        context->baseAddr1     = 0UL;

        context->buf1Size      = 0UL;
        context->buf1rwBondary = 0UL;
        context->baseAddr2     = 0UL;

        retStatus = CY_SCB_EZI2C_SUCCESS;
    }

    return (retStatus);
}


/*******************************************************************************
*  Function Name: Cy_SCB_EZI2C_DeInit
****************************************************************************//**
*
* De-initializes the SCB block, returns register values to default.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \note
* Ensure that the SCB block is disabled before calling this function.
*
*******************************************************************************/
void Cy_SCB_EZI2C_DeInit(CySCB_Type *base)
{
    /* Returns block registers into the default state */
    base->CTRL     = CY_SCB_CTRL_DEF_VAL;
    base->I2C_CTRL = CY_SCB_I2C_CTRL_DEF_VAL;

    base->RX_CTRL      = CY_SCB_RX_CTRL_DEF_VAL;
    base->RX_FIFO_CTRL = 0UL;
    base->RX_MATCH     = 0UL;

    base->TX_CTRL      = CY_SCB_TX_CTRL_DEF_VAL;
    base->TX_FIFO_CTRL = 0UL;

    base->INTR_SPI_EC_MASK = 0UL;
    base->INTR_I2C_EC_MASK = 0UL;
    base->INTR_RX_MASK     = 0UL;
    base->INTR_TX_MASK     = 0UL;
    base->INTR_M_MASK      = 0UL;
    base->INTR_S_MASK      = 0UL;
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_Disable
****************************************************************************//**
*
* Disables the SCB block and clears context statuses.
* Note that after the block was disabled, the TX and RX FIFOs and hardware
* statuses are cleared. Also, the hardware stops driving the output and
* ignores the input.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user should not modify anything
* in this structure.
*
* \note
* Calling this function when EZI2C is busy (slave was address and communicates
* with master) may cause transaction corruption because the hardware stops
* driving the output and ignores the input. It is recommenced to ensure that
* EZI2C slave is not busy before calling this function.
*
*******************************************************************************/
void Cy_SCB_EZI2C_Disable(CySCB_Type *base, cy_stc_scb_ezi2c_context_t *context)
{
    base->CTRL &= (uint32_t) ~SCB_CTRL_ENABLED_Msk;

    /* Set the state to default and clear statuses */
    context->status = 0UL;
    context->state  = CY_SCB_EZI2C_STATE_IDLE;
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_DeepSleep
****************************************************************************//**
*
* Configures the EZI2C slave to be a wake-up source from Deep Sleep on address
* match: enables wake-up interrupt.
* Note that a wake-up from the Deep Sleep option must be enabled during
* initialization. If this was not done, this function does nothing.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \note
* Not all SCB blocks are capable to be a wake-up source from Deep Sleep.
* Consult the device datasheet to determine which SCB supports this option.
*
*******************************************************************************/
void Cy_SCB_EZI2C_DeepSleep(CySCB_Type *base)
{
    if (_FLD2BOOL(SCB_CTRL_EC_AM_MODE, base->CTRL))
    {
        Cy_SCB_SetI2CInterruptMask(base, CY_SCB_I2C_INTR_WAKEUP);
    }
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_Wakeup
****************************************************************************//**
*
* Disables the wake-up interrupt.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
*******************************************************************************/
void Cy_SCB_EZI2C_Wakeup(CySCB_Type *base)
{
    if (_FLD2BOOL(SCB_CTRL_EC_AM_MODE, base->CTRL))
    {
        Cy_SCB_SetI2CInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);
    }
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_GetActivity
****************************************************************************//**
*
* Returns a non-zero value if an I2C read or write cycle has occurred since the
* last time this function was called. All flags are reset to zero at the end of
* this function call, except the \ref CY_SCB_EZI2C_STATUS_BUSY.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user should not modify anything
* in this structure.
*
* \return
* \ref group_scb_ezi2c_macro_get_activity.
*
*******************************************************************************/
uint32_t Cy_SCB_EZI2C_GetActivity(CySCB_Type const *base, cy_stc_scb_ezi2c_context_t *context)
{
    uint32_t intrState;
    uint32_t locStatus;

    /* Suppress a compiler warning about unused variables */
    (void) base;

    intrState = Cy_SysLib_EnterCriticalSection();

    locStatus = context->status;
    context->status &= CY_SCB_EZI2C_STATUS_BUSY;

    Cy_SysLib_ExitCriticalSection(intrState);

    return (locStatus);
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_SetAddress1
****************************************************************************//**
*
* Sets the primary EZI2C slave address.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param addr
* The 7-bit right justified slave address.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user should not modify anything
* in this structure.
*
*******************************************************************************/
void Cy_SCB_EZI2C_SetAddress1(CySCB_Type *base, uint8_t addr, cy_stc_scb_ezi2c_context_t *context)
{
    context->address1 = addr;

    base->RX_MATCH = _CLR_SET_FLD32U(base->RX_MATCH, SCB_RX_MATCH_ADDR, ((uint32_t)((uint32_t) addr << 1UL)));

    UpdateAddressMask(base, context);
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_GetAddress1
****************************************************************************//**
*
* Returns the primary the EZI2C slave address.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* * \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user should not modify anything
* in this structure.
*
* \return
* The 7-bit right justified slave address.
*
*******************************************************************************/
uint32_t Cy_SCB_EZI2C_GetAddress1(CySCB_Type const *base, cy_stc_scb_ezi2c_context_t const *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) base;

    return ((uint32_t) context->address1);
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_SetBuffer1
****************************************************************************//**
*
* Sets up the data buffer to be exposed to the I2C master on the primary slave
* address request.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param buf
* The pointer to data buffer.
*
* \param bufSize
* Size of buffer in bytes.
*
* \param rwBoundary
* Number of data bytes starting from the beginning of the buffer with read and
* write access. Data bytes located at rwBoundary or greater are read only.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user should not modify anything
* in this structure.
*
* \note
* Calling this function in the middle of a transaction intended for the primary
* slave address leads to unexpected behavior.
*
*******************************************************************************/
void Cy_SCB_EZI2C_SetBuffer1(CySCB_Type const *base, uint8_t *buf, uint32_t bufSize, uint32_t rwBoundary,
                             cy_stc_scb_ezi2c_context_t *context)
{
    uint32_t intrState;

    /* Suppress a compiler warning about unused variables */
    (void) base;

    CY_ASSERT( !((NULL == buf) && (0UL == bufSize)) );

    intrState = Cy_SysLib_EnterCriticalSection();

    context->buf1          = buf;
    context->buf1Size      = bufSize;
    context->buf1rwBondary = rwBoundary;

    Cy_SysLib_ExitCriticalSection(intrState);
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_SetAddress2
****************************************************************************//**
*
* Sets the secondary EZI2C slave address.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param addr
* The 7-bit right justified slave address.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user should not modify anything
* in this structure.
*
* \note
* Calling this function when EZI2C slave configured for one address operation
* leads to unexpected behavior because it updates address mask.
*
*******************************************************************************/
void Cy_SCB_EZI2C_SetAddress2(CySCB_Type *base, uint8_t addr, cy_stc_scb_ezi2c_context_t *context)
{
    context->address2 = addr;

    UpdateAddressMask(base, context);
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_GetAddress2
****************************************************************************//**
*
* Returns the secondary the EZI2C slave address.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user should not modify anything
* in this structure.
*
* \return
* The 7-bit right justified slave address.
*
*******************************************************************************/
uint32_t Cy_SCB_EZI2C_GetAddress2(CySCB_Type const *base, cy_stc_scb_ezi2c_context_t const *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) base;

    return ((uint32_t) context->address2);
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_SetBuffer2
****************************************************************************//**
*
* Sets up the data buffer to be exposed to the I2C master on the secondary
* slave address request.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param buf
* The pointer to data buffer.
*
* \param bufSize
* Size of buffer in bytes.
*
* \param rwBoundary
* Number of data bytes starting from the beginning of the buffer with read and
* write access. Data bytes located at rwBoundary or greater are read only.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user should not modify anything
* in this structure.
*
* \note
* Calling this function in the middle of a transaction intended for the
* secondary slave address leads to unexpected behavior.
*
*******************************************************************************/
void Cy_SCB_EZI2C_SetBuffer2(CySCB_Type const *base, uint8_t *buf, uint32_t bufSize, uint32_t rwBoundary,
                             cy_stc_scb_ezi2c_context_t *context)
{
    uint32_t intrState;

    /* Suppress a compiler warning about unused variables */
    (void) base;

    CY_ASSERT( !((NULL == buf) && (0UL == bufSize)) );

    intrState = Cy_SysLib_EnterCriticalSection();

    context->buf2          = buf;
    context->buf2Size      = bufSize;
    context->buf2rwBondary = rwBoundary;

    Cy_SysLib_ExitCriticalSection(intrState);
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_Interrupt
****************************************************************************//**
*
* This is the interrupt function for the SCB configured in the EZI2C mode.
* This function must be called inside the user-defined interrupt service
* routine in order for EZI2C slave to work.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t allocated
* by the user. The structure is used during the EZI2C operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
*
*******************************************************************************/
void Cy_SCB_EZI2C_Interrupt(CySCB_Type *base, cy_stc_scb_ezi2c_context_t *context)
{
    uint32_t slaveIntrStatus;

    /* Handle an I2C wake-up event */
    if (0UL != (CY_SCB_I2C_INTR_WAKEUP & Cy_SCB_GetI2CInterruptStatusMasked(base)))
    {
        Cy_SCB_ClearI2CInterrupt(base, CY_SCB_I2C_INTR_WAKEUP);
    }

    /* Get the slave interrupt sources */
    slaveIntrStatus = Cy_SCB_GetSlaveInterruptStatusMasked(base);

    /* Handle the error conditions */
    if (0UL != (CY_SCB_EZI2C_SLAVE_INTR_ERROR & slaveIntrStatus))
    {
        HandleErrors(base, context);

        Cy_SCB_ClearSlaveInterrupt(base, CY_SCB_EZI2C_SLAVE_INTR_ERROR);

        /* Triggers stop processing to complete transaction */
        slaveIntrStatus |= CY_SCB_SLAVE_INTR_I2C_STOP;
    }
    else
    {
        if ((CY_SCB_EZI2C_STATE_RX_DATA1 == context->state) &&
            (0UL != (CY_SCB_SLAVE_INTR_I2C_STOP & slaveIntrStatus)))
        {
            /* Get data from RX FIFO after Stop is generated */
            Cy_SCB_SetRxInterrupt    (base, CY_SCB_RX_INTR_LEVEL);
            Cy_SCB_SetRxInterruptMask(base, CY_SCB_RX_INTR_LEVEL);
        }
    }

    /* Handle the receive direction (master writes data) */
    if (0UL != (CY_SCB_RX_INTR_LEVEL & Cy_SCB_GetRxInterruptStatusMasked(base)))
    {
        HandleDataReceive(base, context);

        Cy_SCB_ClearRxInterrupt(base, CY_SCB_RX_INTR_LEVEL);
    }

    /* Handle the transaction completion */
    if (0UL != (CY_SCB_SLAVE_INTR_I2C_STOP & slaveIntrStatus))
    {
        HandleStop(base, context);

        Cy_SCB_ClearSlaveInterrupt(base, CY_SCB_SLAVE_INTR_I2C_STOP);

        /* Update the slave interrupt status */
        slaveIntrStatus = Cy_SCB_GetSlaveInterruptStatusMasked(base);
    }

    /* Handle the address byte */
    if (0UL != (CY_SCB_SLAVE_INTR_I2C_ADDR_MATCH & slaveIntrStatus))
    {
        HandleAddress(base, context);

        Cy_SCB_ClearI2CInterrupt  (base, CY_SCB_I2C_INTR_WAKEUP);
        Cy_SCB_ClearSlaveInterrupt(base, CY_SCB_SLAVE_INTR_I2C_ADDR_MATCH);
    }

    /* Handle the transmit direction (master reads data) */
    if (0UL != (CY_SCB_TX_INTR_LEVEL & Cy_SCB_GetTxInterruptStatusMasked(base)))
    {
        HandleDataTransmit(base, context);

        Cy_SCB_ClearTxInterrupt(base, CY_SCB_TX_INTR_LEVEL);
    }
}



/*******************************************************************************
* Function Name: HandleErrors
****************************************************************************//**
*
* Handles an error conditions.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t allocated
* by the user. The structure is used during the EZI2C operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
*
*******************************************************************************/
static void HandleErrors(CySCB_Type *base, cy_stc_scb_ezi2c_context_t *context)
{
    context->status |= CY_SCB_EZI2C_STATUS_ERR;

    /* Drop any data available in RX FIFO */
    Cy_SCB_ClearRxFifo(base);

    /* Stop TX and RX processing */
    Cy_SCB_SetRxInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);
    Cy_SCB_SetTxInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);
}


/*******************************************************************************
* Function Name: HandleAddress
****************************************************************************//**
*
* Prepares the EZI2C slave for the following read or write transfer after the
* matched address was received.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t allocated
* by the user. The structure is used during the EZI2C operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
*
*******************************************************************************/
static void HandleAddress(CySCB_Type *base, cy_stc_scb_ezi2c_context_t *context)
{
    /* Default actions: active is address1, ACK address */
    uint32_t cmd = SCB_I2C_S_CMD_S_ACK_Msk;
    context->addr1Active = true;

    if (0U != context->address2)
    {
        /* Get address from RX FIFO and make it 7-bits */
        uint32_t address = (Cy_SCB_ReadRxFifo(base) >> 1UL);
        Cy_SCB_ClearRxInterrupt(base, CY_SCB_RX_INTR_LEVEL);

        /* Decide if address matches */
        if ((address == context->address1) || (address == context->address2))
        {
            /* ACK address */
            if (address == context->address2)
            {
                context->addr1Active = false;
            }

            /* Clear the stall stop status and enable the stop interrupt source */
            Cy_SCB_ClearSlaveInterrupt  (base, CY_SCB_SLAVE_INTR_I2C_STOP);
            Cy_SCB_SetSlaveInterruptMask(base, CY_SCB_EZI2C_SLAVE_INTR);
        }
        else
        {
            /* NACK address */
            cmd = SCB_I2C_S_CMD_S_NACK_Msk;

            /* Disable the stop interrupt source */
            Cy_SCB_SetI2CInterruptMask(base, CY_SCB_EZI2C_SLAVE_INTR_NO_STOP);
        }
    }

    /* Clean-up TX FIFO before continue transaction */
    Cy_SCB_ClearTxFifo(base);

    /* Set the command to an ACK or NACK address */
    base->I2C_S_CMD = cmd;

    if (cmd == SCB_I2C_S_CMD_S_ACK_Msk)
    {
        context->status |= CY_SCB_EZI2C_STATUS_BUSY;

        /* Prepare for a transaction */
        if (_FLD2BOOL(SCB_I2C_STATUS_S_READ,base->I2C_STATUS))
        {
            /* Master read data from slave */
            context->state = CY_SCB_EZI2C_STATE_TX_DATA;

            /* Prepare buffer to transmit */
            if (context->addr1Active)
            {
                context->curBuf  = &context->buf1[context->baseAddr1];
                context->bufSize = context->buf1Size - context->baseAddr1;
            }
            else
            {
                context->curBuf  = &context->buf2[context->baseAddr2];
                context->bufSize = context->buf2Size - context->baseAddr2;
            }

            Cy_SCB_SetTxInterruptMask(base, CY_SCB_TX_INTR_LEVEL);
        }
        else
        {
            /* Master writes data into slave */
            context->state = CY_SCB_EZI2C_STATE_RX_OFFSET_MSB;

            context->bufSize = ((context->addr1Active) ? context->buf1Size : context->buf2Size);

            Cy_SCB_SetRxFifoLevel    (base, 0UL);
            Cy_SCB_SetRxInterruptMask(base, CY_SCB_RX_INTR_LEVEL);
        }
    }
}


/*******************************************************************************
* Function Name: HandleDataReceive
****************************************************************************//**
*
* Updates RX FIFO level to trigger next read from it. It also manages
* auto data NACK feature.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param bufSize
* Size of buffer in bytes.
*
*******************************************************************************/
static void UpdateRxFifoLevel(CySCB_Type *base, uint32_t bufSize)
{
    uint32_t level;

    if (bufSize > CY_SCB_EZI2C_FIFO_SIZE)
    {
        /* Continue transaction: there is a space in the buffer */
        level = (bufSize - CY_SCB_EZI2C_FIFO_SIZE);
        level = ((level > CY_SCB_EZI2C_FIFO_SIZE) ? CY_SCB_EZI2C_HALF_FIFO_SIZE : level) - 1UL;
    }
    else
    {
        /* Prepare to end transaction: after FIFO becomes full NACK next byte.
        * NACKed byte is dropped by the hardware.
        */
        base->I2C_CTRL |= SCB_I2C_CTRL_S_NOT_READY_DATA_NACK_Msk;

        level = ((bufSize == 0UL) ? (0UL) : (bufSize - 1UL));
        Cy_SCB_SetRxInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);
    }

    Cy_SCB_SetRxFifoLevel(base, level);
}


/*******************************************************************************
* Function Name: HandleDataReceive
****************************************************************************//**
*
* Handles data reading from the RX FIFO.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t allocated
* by the user. The structure is used during the EZI2C operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
*
*******************************************************************************/
static void HandleDataReceive(CySCB_Type *base, cy_stc_scb_ezi2c_context_t *context)
{
    switch(context->state)
    {
        case CY_SCB_EZI2C_STATE_RX_OFFSET_MSB:
        case CY_SCB_EZI2C_STATE_RX_OFFSET_LSB:
        {
            /* Default actions: compare base address and ACK it */
            bool checkBaseAddr = true;

            /* Get base address from RX FIFO */
            uint32_t baseAddr = Cy_SCB_ReadRxFifo(base);

            if (context->subAddrSize == CY_SCB_EZI2C_SUB_ADDR16_BITS)
            {
                if (context->state == CY_SCB_EZI2C_STATE_RX_OFFSET_MSB)
                {
                    /* ACK base address MSB */
                    base->I2C_S_CMD = SCB_I2C_S_CMD_S_ACK_Msk;

                    /* Temporary store base address MSB */
                    context->idx = (uint32_t) (baseAddr << 8UL);

                    /* Do not compare until 16 bits are received */
                    checkBaseAddr = false;
                    context->state = CY_SCB_EZI2C_STATE_RX_OFFSET_LSB;
                }
                else
                {
                    /* Get base address (MSB | LSB) */
                    baseAddr |= context->idx;
                }
            }

            /* Check if received base address is valid */
            if (checkBaseAddr)
            {
                uint32_t cmd = SCB_I2C_S_CMD_S_ACK_Msk;

                /* Decide if base address within buffer range */
                if (baseAddr < context->bufSize)
                {
                    /* Accept new base address */
                    if (context->addr1Active)
                    {
                        context->baseAddr1 = baseAddr;
                    }
                    else
                    {
                        context->baseAddr2 = baseAddr;
                    }

                    /* Store base address to use it later */
                    context->idx = baseAddr;
                }
                else
                {
                    /* Restore valid base address */
                    context->idx = ((context->addr1Active) ? context->baseAddr1 : context->baseAddr2);

                    /* Base address is out of range - NACK it */
                    cmd = SCB_I2C_S_CMD_S_NACK_Msk;
                }

                /* Set the command to an ACK or NACK address */
                base->I2C_S_CMD = cmd;

                if (cmd == SCB_I2C_S_CMD_S_ACK_Msk)
                {
                    /* Prepare buffer for a write */
                    if (context->addr1Active)
                    {
                        context->curBuf  = &context->buf1[context->baseAddr1];
                        context->bufSize = ((context->baseAddr1 < context->buf1rwBondary) ?
                                                    (context->buf1rwBondary - context->baseAddr1) : (0UL));
                    }
                    else
                    {
                        context->curBuf  = &context->buf2[context->baseAddr2];
                        context->bufSize = ((context->baseAddr2 < context->buf2rwBondary) ?
                                                    (context->buf2rwBondary - context->baseAddr2) : (0UL));
                    }

                    /* Choice receive scheme */
                    if ((0U != context->address2) || (context->bufSize < CY_SCB_EZI2C_FIFO_SIZE))
                    {
                        /* Handle each byte separately */
                        context->state = CY_SCB_EZI2C_STATE_RX_DATA0;
                    }
                    else
                    {
                        /* Use RX FIFO and auto ACK/NACK features */
                        base->I2C_CTRL |= SCB_I2C_CTRL_S_READY_DATA_ACK_Msk;
                        UpdateRxFifoLevel(base, context->bufSize);

                        context->state = CY_SCB_EZI2C_STATE_RX_DATA1;
                    }
                }
            }
        }
        break;

        case CY_SCB_EZI2C_STATE_RX_DATA0:
        {
            uint32_t byte = Cy_SCB_ReadRxFifo(base);

            /* Check if there is a space to store byte */
            if (context->bufSize > 0UL)
            {
                /* Continue the transfer: send ACK */
                base->I2C_S_CMD = SCB_I2C_S_CMD_S_ACK_Msk;

                /* Store byte in the buffer */
                context->curBuf[0UL] = (uint8_t) byte;
                --context->bufSize;
                ++context->curBuf;

                /* Update base address to notice that buffer is modified */
                ++context->idx;
            }
            else
            {
                /* Finish a transfer: send a NACK. Drop received byte */
                base->I2C_S_CMD = SCB_I2C_S_CMD_S_NACK_Msk;
                Cy_SCB_SetRxInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);
            }
        }
        break;

        case CY_SCB_EZI2C_STATE_RX_DATA1:
        {
            /* Get the number of bytes to read from RX FIFO */
            uint32_t numToCopy = Cy_SCB_GetRxFifoLevel(base) + 1UL;

            /* Get data from RX FIFO */
            numToCopy = Cy_SCB_ReadArray(base, context->curBuf, numToCopy);
            context->bufSize -= numToCopy;
            context->curBuf  += numToCopy;

            /* Configure next RX FIFO read event */
            UpdateRxFifoLevel(base, context->bufSize);

            /* Update base address to notice that buffer is modified */
            ++context->idx;
        }
        break;

        default:
        break;
    }
}


/*******************************************************************************
* Function Name: HandleDataTransmit
****************************************************************************//**
*
* Loads the TX FIFO with data from the buffer.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t allocated
* by the user. The structure is used during the EZI2C operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
*
*******************************************************************************/
static void HandleDataTransmit(CySCB_Type *base, cy_stc_scb_ezi2c_context_t *context)
{
    if (context->bufSize > 0UL)
    {
        /* Write data into TX FIFO from the buffer */
        uint32_t numToCopy = Cy_SCB_WriteArray(base, context->curBuf, context->bufSize);
        context->bufSize  -= numToCopy;
        context->curBuf   += numToCopy;
    }

    if (0UL == context->bufSize)
    {
        /* Write default bytes into the TX FIFO */
        (void) Cy_SCB_WriteDefaultArray(base, CY_SCB_EZI2C_DEFAULT_TX, CY_SCB_EZI2C_FIFO_SIZE);
    }
}


/*******************************************************************************
* Function Name: HandleStop
****************************************************************************//**
*
* Handles transfer completion.
* It is triggered by a stop or restart condition on the bus.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t allocated
* by the user. The structure is used during the EZI2C operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
*
*******************************************************************************/
static void HandleStop(CySCB_Type *base, cy_stc_scb_ezi2c_context_t *context)
{
    /* Check for errors */
    if (0UL != (CY_SCB_EZI2C_STATUS_ERR & context->status))
    {
        /* Re-enable SCB to get it back into know state: FIFO content and
        * interrupt statuses are cleared.
        */
        Cy_SCB_FwBlockReset(base);
    }

    /* Clean-up hardware to be ready for the next transaction */
    if (CY_SCB_EZI2C_STATE_TX_DATA == context->state)
    {
        Cy_SCB_SetTxInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);
    }
    else
    {
        Cy_SCB_SetRxInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);

        base->I2C_CTRL &= (uint32_t) ~(SCB_I2C_CTRL_S_READY_DATA_ACK_Msk |
                                       SCB_I2C_CTRL_S_NOT_READY_DATA_NACK_Msk);
    }

    /* Update statuses */
    context->status &= (uint32_t) ~CY_SCB_EZI2C_STATUS_BUSY;

    if (context->addr1Active)
    {
        context->status |= ((CY_SCB_EZI2C_STATE_TX_DATA == context->state) ? CY_SCB_EZI2C_STATUS_READ1 :
                             ((context->baseAddr1 != context->idx) ?  CY_SCB_EZI2C_STATUS_WRITE1 : 0UL));
    }
    else
    {
        context->status |= ((CY_SCB_EZI2C_STATE_TX_DATA == context->state) ? CY_SCB_EZI2C_STATUS_READ2 :
                             ((context->baseAddr2 != context->idx) ? CY_SCB_EZI2C_STATUS_WRITE2 : 0UL));
    }

    /* Back to idle state */
    context->state = CY_SCB_EZI2C_STATE_IDLE;
}


/*******************************************************************************
* Function Name: UpdateAddressMask
****************************************************************************//**
*
* Updates the slave address mask to enable the SCB hardware to receive matching
* slave addresses.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t allocated
* by the user. The structure is used during the EZI2C operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
*
*******************************************************************************/
static void UpdateAddressMask(CySCB_Type *base, cy_stc_scb_ezi2c_context_t const *context)
{
    uint32_t addrMask;

    /* Check how many addresses are used: */
    if (0U != context->address2)
    {
        /* If (addr1 and addr2) bit matches - mask bit equals 1, otherwise 0 */
        addrMask  = (uint32_t) ~((uint32_t) context->address1 ^ (uint32_t) context->address2);
    }
    else
    {
        /* All bits have to match for singe address */
        addrMask = CY_SCB_EZI2C_ONE_ADDRESS_MASK;
    }

    /* Update hardware address match */
    base->RX_MATCH = _CLR_SET_FLD32U(base->RX_MATCH, SCB_RX_MATCH_MASK, ((uint32_t) addrMask << 1UL));
}


#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */

