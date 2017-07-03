/***************************************************************************//**
* \file cy_scb_spi.c
* \version 1.0
*
* Provides SPI API implementation of the SCB driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_scb_spi.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* Static functions */
static void HandleTransmit(CySCB_Type *base, cy_stc_scb_spi_context_t *context);
static void HandleReceive (CySCB_Type *base, cy_stc_scb_spi_context_t *context);
static void DiscardArrayNoCheck(CySCB_Type const *base, uint32_t size);

/*******************************************************************************
* Function Name: Cy_SCB_SPI_Init
****************************************************************************//**
*
* Initializes the SCB for SPI operation.
*
* \param base
* The pointer to the SPI SCB instance.
*
* \param config
* The pointer to the configuration structure \ref cy_stc_scb_spi_config_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_spi_context_t allocated
* by the user. The structure is used during the SPI operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
* If only SPI functions which do not require context will be used pass NULL
* as pointer to context.
*
* \return
* \ref cy_en_scb_spi_status_t
*
* \note
* Ensure that the SCB block is disabled before calling this function.
*
*******************************************************************************/
cy_en_scb_spi_status_t Cy_SCB_SPI_Init(CySCB_Type *base, cy_stc_scb_spi_config_t const *config, cy_stc_scb_spi_context_t *context)
{
    cy_en_scb_spi_status_t retStatus = CY_SCB_SPI_BAD_PARAM;

    if ((NULL != base) && (NULL != config))
    {
        /* Set SCLK mode for TI - CY_SCB_SPI_CPHA1_CPOL0, NS - CY_SCB_SPI_CPHA0_CPOL0, Motorola - take from config */
        uint32_t locSclkMode = (CY_SCB_SPI_MOTOROLA == config->subMode) ? config->sclkMode :
                                ((CY_SCB_SPI_NATIONAL == config->subMode) ? CY_SCB_SPI_CPHA0_CPOL0 : CY_SCB_SPI_CPHA1_CPOL0);

        /* Configure an SPI interface */
        base->CTRL = (((config->rxDataWidth <= CY_SCB_BYTE_WIDTH) && (config->txDataWidth <= CY_SCB_BYTE_WIDTH)) ?
                          SCB_CTRL_BYTE_MODE_Msk : 0UL)                                |
                         (config->enableWakeFromSleep ? SCB_CTRL_EC_AM_MODE_Msk : 0UL) |
                          _VAL2FLD(SCB_CTRL_OVS, (config->oversample - 1UL))           |
                          _VAL2FLD(SCB_CTRL_MODE, CY_SCB_CTRL_MODE_SPI);

        base->SPI_CTRL = (config->enableTransferSeperation       ? 0UL : SCB_SPI_CTRL_SSEL_CONTINUOUS_Msk)         |
                            ((0UL != (CY_SCB_SPI_TI_PRECEDE & config->subMode)) ? SCB_SPI_CTRL_SELECT_PRECEDE_Msk : 0UL) |
                            (config->enableMisoLateSample           ? SCB_SPI_CTRL_LATE_MISO_SAMPLE_Msk : 0UL)        |
                            (config->enableFreeRunSclk              ? SCB_SPI_CTRL_SCLK_CONTINUOUS_Msk : 0UL)         |
                            ((CY_SCB_SPI_MASTER == config->spiMode)    ? SCB_SPI_CTRL_MASTER_MODE_Msk : 0UL)             |
                            _VAL2FLD(CY_SCB_SPI_CTRL_CLK_MODE, locSclkMode)                                              |
                            _VAL2FLD(CY_SCB_SPI_CTRL_SSEL_POLARITY, config->ssPolarity)                                  |
                            _VAL2FLD(SCB_SPI_CTRL_MODE, config->subMode);

        /* Configure the RX direction */
        base->RX_CTRL = (config->enableMsbFirst    ? SCB_RX_CTRL_MSB_FIRST_Msk : 0UL) |
                           (config->enableInputFilter ? SCB_RX_CTRL_MEDIAN_Msk : 0UL)    |
                            _VAL2FLD(SCB_RX_CTRL_DATA_WIDTH, (config->rxDataWidth - 1UL));

        base->RX_FIFO_CTRL = _VAL2FLD(SCB_RX_FIFO_CTRL_TRIGGER_LEVEL, config->rxFifoTriggerLevel);

        /* Configure the TX direction */
        base->TX_CTRL = (config->enableMsbFirst ? SCB_RX_CTRL_MSB_FIRST_Msk : 0UL) |
                            _VAL2FLD(SCB_TX_CTRL_DATA_WIDTH, (config->txDataWidth - 1UL));

        base->TX_FIFO_CTRL = _VAL2FLD(SCB_TX_FIFO_CTRL_TRIGGER_LEVEL, config->txFifoTriggerLevel);

        /* Set up interrupt sources */
        base->INTR_TX_MASK = config->txFifoIntEnableMask;
        base->INTR_RX_MASK = config->rxFifoIntEnableMask;
        base->INTR_M       = (config->masterSlaveIntEnableMask & SCB_INTR_M_MASK_SPI_DONE_Msk);
        base->INTR_S       = (config->masterSlaveIntEnableMask & SCB_INTR_S_MASK_SPI_BUS_ERROR_Msk);
        base->INTR_SPI_EC_MASK = 0UL;

        /* Initialize the context */
        if (NULL != context)
        {
            context->status    = 0UL;

            context->txBufIdx  = 0UL;
            context->rxBufIdx  = 0UL;

            context->cbEvents = NULL;

        #if !defined(NDEBUG)
            /* Put an initialization key into the initKey variable to verify
            * context initialization in the transfer API.
            */
            context->initKey = CY_SCB_SPI_INIT_KEY;
        #endif /* !(NDEBUG) */
        }

        retStatus = CY_SCB_SPI_SUCCESS;
    }

    return (retStatus);
}


/*******************************************************************************
* Function Name: Cy_SCB_SPI_DeInit
****************************************************************************//**
*
* De-initializes the SCB block, returns the register values to default.
*
* \param base
* The pointer to the SPI SCB instance.
*
* \note
* Ensure that the SCB block is disabled before calling this function.
*
*******************************************************************************/
void Cy_SCB_SPI_DeInit(CySCB_Type *base)
{
    /* SPI interface */
    base->CTRL         = CY_SCB_CTRL_DEF_VAL;
    base->SPI_CTRL     = CY_SCB_SPI_CTRL_DEF_VAL;

    /* RX direction */
    base->RX_CTRL      = CY_SCB_RX_CTRL_DEF_VAL;
    base->RX_FIFO_CTRL = 0UL;

    /* TX direction */
    base->TX_CTRL      = CY_SCB_TX_CTRL_DEF_VAL;
    base->TX_FIFO_CTRL = 0UL;

    /* Disable all interrupt sources */
    base->INTR_SPI_EC_MASK = 0UL;
    base->INTR_I2C_EC_MASK = 0UL;
    base->INTR_RX_MASK     = 0UL;
    base->INTR_TX_MASK     = 0UL;
    base->INTR_M_MASK      = 0UL;
    base->INTR_S_MASK      = 0UL;
}


/*******************************************************************************
* Function Name: Cy_SCB_SPI_Disable
****************************************************************************//**
*
* Disables the SCB block, clears context statuses and disables
* TX and RX interrupt sources.
* Note that after the block is disabled the TX and RX FIFOs and
* hardware statuses are cleared. Also, the hardware stops driving the output
* and ignores the input.
*
* \param base
* The pointer to the SPI SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_spi_context_t allocated
* by the user. The structure is used during the SPI operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
* If only SPI functions which do not require context will be used pass NULL
* as pointer to context.
*
* \note
* Calling this function when the SPI is busy (master preforms data transfer or
* slave communicates with the master) may cause transfer corruption because the
* hardware stops driving the output and ignores the input.
* It is recommenced to ensure that the SPI is not busy before calling this
* function.
*
*******************************************************************************/
void Cy_SCB_SPI_Disable(CySCB_Type *base, cy_stc_scb_spi_context_t *context)
{
    base->CTRL &= (uint32_t) ~SCB_CTRL_ENABLED_Msk;

    if (NULL != context)
    {
        context->status    = 0UL;

        context->rxBufIdx  = 0UL;
        context->txBufIdx  = 0UL;
    }

    /* Disable RX and TX interrupt sources for the slave because
    * RX overflow and TX underflow are kept enabled after the 1st call of
    * Cy_SCB_SPI_Transfer().
    */
    if (!_FLD2BOOL(SCB_SPI_CTRL_MASTER_MODE, base->SPI_CTRL))
    {
        Cy_SCB_SetRxInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);
        Cy_SCB_SetTxInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);
    }
}


/*******************************************************************************
* Function Name: Cy_SCB_SPI_DeepSleep
****************************************************************************//**
*
* Prepares the SPI for deep sleep operation: clears the SPI wake up interrupt
* history and enables it. The wake-up event is the activation of the
* slave select line.
* This function does nothing if the deep sleep option has not been enabled.
*
* \param base
* The pointer to the SPI SCB instance.
*
* \note
* Not all SCBs support the deep sleep operation, consult the device
* datasheet to determine which SCBs support deep sleep operation.
*
*******************************************************************************/
void Cy_SCB_SPI_DeepSleep(CySCB_Type *base)
{
    if (_FLD2BOOL(SCB_CTRL_EC_AM_MODE, base->CTRL))
    {
        /* Clear and enable the SPI wakeup source */
        Cy_SCB_ClearSpiInterrupt  (base, CY_SCB_SPI_INTR_WAKEUP);
        Cy_SCB_SetSpiInterruptMask(base, CY_SCB_SPI_INTR_WAKEUP);
    }
}


/*******************************************************************************
* Function Name: Cy_SCB_SPI_Wakeup
****************************************************************************//**
*
* Disables the wakeup interrupt.
* This function does nothing if the deep sleep option has not been enabled.
*
* \param base
* The pointer to the SPI SCB instance.
*
*******************************************************************************/
void Cy_SCB_SPI_Wakeup(CySCB_Type *base)
{
    if (_FLD2BOOL(SCB_CTRL_EC_AM_MODE, base->CTRL))
    {
        /* Disable the SPI wakeup source */
        Cy_SCB_SetSpiInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);
    }
}


/************************* High-Level Functions ********************************
* The following functions are considered high-level. They provide the layer of
* intelligence to the SCB. These functions require interrupts.
* Low-level and high-level functions should not be mixed because low-level API
* can adversely affect the operation of high-level functions.
*******************************************************************************/


/*******************************************************************************
* Function Name: Cy_SCB_SPI_Transfer
****************************************************************************//**
*
* This function starts an SPI transfer operation.
* It configures transmit and receive buffers for an SPI transfer.
* If the data that will be received is not important, pass NULL as rxBuf.
* If the data that will be transmitted is not important, pass NULL as txBuf
* and then the \ref CY_SCB_SPI_DEFAULT_TX is sent out as each data element.
*
* After the function configures TX and RX interrupt sources, it returns and
* \ref Cy_SCB_SPI_Interrupt manages further data transfer.
*
* * In the master mode, the transfer operation is started after calling this
*   function
* * In the slave mode, the transfer is registered and will be started when
*   the master request arrives.
*
* When the transfer operation is completed (requested number of data elements
* sent and received), the \ref CY_SCB_SPI_TRANSFER_ACTIVE status is cleared
* and the \ref CY_SCB_SPI_TRANSFER_CMPLT_EVENT event is generated.
*
* \param base
* The pointer to the SPI SCB instance.
*
* \param txBuf
* The pointer of the buffer with data to transmit.
* The item size is defined by the data type which depends on the configured
* TX data width.
*
* \param rxBuf
* The pointer to the buffer to store received data.
* The item size is defined by the data type which depends on the configured
* RX data width.
*
* \param size
* The number of data elements to transmit and receive.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_spi_context_t allocated
* by the user. The structure is used during the SPI operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
*
* \return
* \ref cy_en_scb_spi_status_t
*
* \note
* * The buffers must not be modified and stay allocated until the end of the
*   transfer.
* * This function overrides all RX and TX FIFO interrupt sources and changes
*   the RX and TX FIFO level.
*
*******************************************************************************/
cy_en_scb_spi_status_t Cy_SCB_SPI_Transfer(CySCB_Type *base, void *txBuf, void *rxBuf, uint32_t size, cy_stc_scb_spi_context_t *context)
{
    cy_en_scb_spi_status_t retStatus = CY_SCB_SPI_TRANSFER_BUSY;

#if !defined(NDEBUG)
    /* Check that the initialization key was set before using the context */
    CY_ASSERT(NULL != context);
    CY_ASSERT(CY_SCB_SPI_INIT_KEY == context->initKey);
#endif /* !(NDEBUG) */

    /* Check if there are no active transfer requests */
    if (0UL == (CY_SCB_SPI_TRANSFER_ACTIVE & context->status))
    {
        uint32_t fifoSize = Cy_SCB_GetFifoSize(base);

        /* Set up the context */
        context->status    = CY_SCB_SPI_TRANSFER_ACTIVE;

        context->txBuf     = txBuf;
        context->txBufSize = size;
        context->txBufIdx =  0UL;

        context->rxBuf     = rxBuf;
        context->rxBufSize = size;
        context->rxBufIdx  = 0UL;

        /* Set the TX interrupt when half of FIFO was transmitted */
        Cy_SCB_SetTxFifoLevel(base, fifoSize / 2UL);

        if (_FLD2BOOL(SCB_SPI_CTRL_MASTER_MODE, base->SPI_CTRL))
        {
            /* Trigger an RX interrupt:
            * - If the transfer size is equal to or less than FIFO, trigger at the end of the transfer.
            * - If the transfer size is greater than FIFO, trigger 1 byte earlier than the TX interrupt.
            */
            Cy_SCB_SetRxFifoLevel(base, (size > fifoSize) ? ((fifoSize / 2UL) - 2UL) : (size - 1UL));

            Cy_SCB_SetMasterInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);

            /* Enable interrupt sources to perform a transfer */
            Cy_SCB_SetRxInterruptMask(base, CY_SCB_RX_INTR_LEVEL);
            Cy_SCB_SetTxInterruptMask(base, CY_SCB_TX_INTR_LEVEL);
        }
        else
        {
            /* Trigger an RX interrupt:
            * - If the transfer size is equal to or less than half of FIFO, trigger ??at the end of the transfer.
            * - If the transfer size is greater than half of FIFO, trigger 1 byte earlier than a TX interrupt.
            */
            Cy_SCB_SetRxFifoLevel(base, (size > (fifoSize / 2UL)) ? ((fifoSize / 2UL) - 2UL) : (size - 1UL));

            Cy_SCB_SetSlaveInterruptMask(base, CY_SCB_SLAVE_INTR_SPI_BUS_ERROR);

            /* Enable interrupt sources to perform a transfer */
            Cy_SCB_SetRxInterruptMask(base, CY_SCB_RX_INTR_LEVEL | CY_SCB_RX_INTR_OVERFLOW);
            Cy_SCB_SetTxInterruptMask(base, CY_SCB_TX_INTR_LEVEL | CY_SCB_TX_INTR_UNDERFLOW);
        }

        retStatus = CY_SCB_SPI_SUCCESS;
    }

    return (retStatus);
}


/*******************************************************************************
* Function Name: Cy_SCB_SPI_AbortTransfer
****************************************************************************//**
*
* Aborts the current SPI transfer.
* It disables the transmit and RX interrupt sources, clears the TX
* and RX FIFOs and the status.
*
* \param base
* The pointer to the SPI SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_spi_context_t allocated
* by the user. The structure is used during the SPI operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
*
* \note
* In the slave mode and after abort of transfer operation master continue
* sending data it gets into RX FIFO and TX FIFO is underflow as there is
* nothing to send. To drop this data, RX FIFO has to be cleared when
* the transfer is completed. Otherwise, received data will be kept and
* copied to the buffer when \ref Cy_SCB_SPI_Transfer is called.
*
* \sideeffect
* The transmit FIFO clear operation also clears the shift register, so that
* the shifter could be cleared in the middle of a data element transfer,
* corrupting it. The data element corruption means that all bits which has
* not been transmitted are transmitted as "ones" on the bus.
*
*******************************************************************************/
void Cy_SCB_SPI_AbortTransfer(CySCB_Type *base, cy_stc_scb_spi_context_t *context)
{
    /* Disable interrupt sources */
    Cy_SCB_SetSlaveInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);

    if (_FLD2BOOL(SCB_SPI_CTRL_MASTER_MODE, base->SPI_CTRL))
    {
        Cy_SCB_SetRxInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);
        Cy_SCB_SetTxInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);
    }
    else
    {
        Cy_SCB_SetRxInterruptMask(base, CY_SCB_RX_INTR_OVERFLOW);
        Cy_SCB_SetTxInterruptMask(base, CY_SCB_TX_INTR_UNDERFLOW);
    }

    /* Clear FIFOs */
    Cy_SCB_SPI_ClearTxFifo(base);
    Cy_SCB_SPI_ClearRxFifo(base);

    /* Clear the status to allow a new transfer */
    context->status = 0UL;
}


/*******************************************************************************
* Function Name: Cy_SCB_SPI_GetNumTransfered
****************************************************************************//**
*
* Returns the number of data elements transferred since the last call to \ref
* Cy_SCB_SPI_Transfer.
*
* \param base
* The pointer to the SPI SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_spi_context_t allocated
* by the user. The structure is used during the SPI operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
*
* \return
* The number of data elements transferred.
*
*******************************************************************************/
uint32_t Cy_SCB_SPI_GetNumTransfered(CySCB_Type const *base, cy_stc_scb_spi_context_t const *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) base;

    return (context->rxBufIdx);
}


/*******************************************************************************
* Function Name: Cy_SCB_SPI_GetTransferStatus
****************************************************************************//**
*
* Returns the status of the transfer operation started by
* \ref Cy_SCB_SPI_Transfer.
* This status is a bit mask and the value returned may have a multiple-bit set.
*
* \param base
* The pointer to the SPI SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_spi_context_t allocated
* by the user. The structure is used during the SPI operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
*
* \return
* \ref group_scb_spi_macro_xfer_status.
*
* \note
* The status is cleared by calling \ref Cy_SCB_SPI_Transfer or
* \ref Cy_SCB_SPI_AbortTransfer.
*
*******************************************************************************/
uint32_t Cy_SCB_SPI_GetTransferStatus(CySCB_Type const *base, cy_stc_scb_spi_context_t const *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) base;

    return (context->status);
}


/*******************************************************************************
* Function Name: Cy_SCB_SPI_Interrupt
****************************************************************************//**
*
* This is the interrupt function for the SCB configured in the SPI mode.
* This function must be called inside the  user-defined interrupt service
* routine in order for \ref Cy_SCB_SPI_Transfer to work.
*
* \param base
* The pointer to the SPI SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_spi_context_t allocated
* by the user. The structure is used during the SPI operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
*
*******************************************************************************/
void Cy_SCB_SPI_Interrupt(CySCB_Type *base, cy_stc_scb_spi_context_t *context)
{
    uint32 locXferErr = 0UL;

    /* Wake up on the slave select condition */
    if (0UL != (CY_SCB_SPI_INTR_WAKEUP & Cy_SCB_GetSpiInterruptStatusMasked(base)))
    {
        Cy_SCB_ClearSpiInterrupt(base, CY_SCB_SPI_INTR_WAKEUP);
    }

    /* The slave error condition */
    if (0UL != (CY_SCB_SLAVE_INTR_SPI_BUS_ERROR & Cy_SCB_GetSlaveInterruptStatusMasked(base)))
    {
        locXferErr       = CY_SCB_SPI_TRANSFER_ERR;
        context->status |= CY_SCB_SPI_SLAVE_TRANSFER_ERR;

        Cy_SCB_ClearSlaveInterrupt(base, CY_SCB_SLAVE_INTR_SPI_BUS_ERROR);
    }

    /* The RX overflow error condition */
    if (0UL != (CY_SCB_RX_INTR_OVERFLOW & Cy_SCB_GetRxInterruptStatusMasked(base)))
    {
        locXferErr       = CY_SCB_SPI_TRANSFER_ERR;
        context->status |= CY_SCB_SPI_TRANSFER_OVERFLOW;

        Cy_SCB_ClearRxInterrupt(base, CY_SCB_RX_INTR_OVERFLOW);
    }

    /* The TX underflow error condition or slave complete data transfer */
    if (0UL != (CY_SCB_TX_INTR_UNDERFLOW & Cy_SCB_GetTxInterruptStatusMasked(base)))
    {
        locXferErr       = CY_SCB_SPI_TRANSFER_ERR;
        context->status |= CY_SCB_SPI_TRANSFER_UNDERFLOW;

        Cy_SCB_ClearTxInterrupt(base, CY_SCB_TX_INTR_UNDERFLOW);
    }

    /* Report an error, use a callback */
    if (0UL != locXferErr)
    {
        if (NULL != context->cbEvents)
        {
            context->cbEvents(CY_SCB_SPI_TRANSFER_ERR);
        }
    }

    /* RX direction */
    if (0UL != (CY_SCB_RX_INTR_LEVEL & Cy_SCB_GetRxInterruptStatusMasked(base)))
    {
        HandleReceive(base, context);

        Cy_SCB_ClearRxInterrupt(base, CY_SCB_RX_INTR_LEVEL);
    }

    /* TX direction */
    if (0UL != (CY_SCB_TX_INTR_LEVEL & Cy_SCB_GetTxInterruptStatusMasked(base)))
    {
        HandleTransmit(base, context);

        Cy_SCB_ClearTxInterrupt(base, CY_SCB_TX_INTR_LEVEL);
    }

    /* The transfer is complete: all data is loaded in the TX FIFO
    * and all data is read from the RX FIFO
    */
    if ((0UL != (context->status & CY_SCB_SPI_TRANSFER_ACTIVE)) &&
        (0UL == context->rxBufSize) && (0UL == context->txBufSize))
    {
        /* The transfer is complete */
        context->status &= (uint32_t) ~CY_SCB_SPI_TRANSFER_ACTIVE;

        if (NULL != context->cbEvents)
        {
            context->cbEvents(CY_SCB_SPI_TRANSFER_CMPLT_EVENT);
        }
    }
}


/*******************************************************************************
* Function Name: HandleReceive
****************************************************************************//**
*
* Reads data from RX FIFO into the buffer provided by \ref Cy_SCB_SPI_Transfer.
*
* \param base
* The pointer to the SPI SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_spi_context_t allocated
* by the user. The structure is used during the SPI operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
*
*******************************************************************************/
static void HandleReceive(CySCB_Type *base, cy_stc_scb_spi_context_t *context)
{
    /* Get data in RX FIFO */
    uint32_t numToCopy = Cy_SCB_GetNumInRxFifo(base);

    /* Adjust the number to read */
    if (numToCopy > context->rxBufSize)
    {
        numToCopy = context->rxBufSize;
    }

    /* Move the buffer */
    context->rxBufIdx  += numToCopy;
    context->rxBufSize -= numToCopy;

    /* Read data from RX FIFO */
    if (NULL != context->rxBuf)
    {
        uint8_t *buf = (uint8_t *) context->rxBuf;

        Cy_SCB_ReadArrayNoCheck(base, context->rxBuf, numToCopy);

        buf = &buf[(Cy_SCB_IsRxDataWidthByte(base) ? (numToCopy) : (2UL * numToCopy))];
        context->rxBuf = (void *) buf;
    }
    else
    {
        /* Discard read data. */
        DiscardArrayNoCheck(base, numToCopy);
    }

    if (0UL == context->rxBufSize)
    {
        /* Disable the RX level interrupt */
        Cy_SCB_SetRxInterruptMask(base, (Cy_SCB_GetRxInterruptMask(base) & (uint32_t) ~CY_SCB_RX_INTR_LEVEL));
    }
    else
    {
        uint32_t level = (_FLD2BOOL(SCB_SPI_CTRL_MASTER_MODE, base->SPI_CTRL)) ?
                                    Cy_SCB_GetFifoSize(base) : (Cy_SCB_GetFifoSize(base) / 2UL);

        if (context->rxBufSize < level)
        {
            Cy_SCB_SetRxFifoLevel(base, (context->rxBufSize - 1UL));
        }
    }
}


/*******************************************************************************
* Function Name: HandleTransmit
****************************************************************************//**
*
* Loads TX FIFO with data provided by \ref Cy_SCB_SPI_Transfer.
*
* \param base
* The pointer to the SPI SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_spi_context_t allocated
* by the user. The structure is used during the SPI operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
*
*******************************************************************************/
static void HandleTransmit(CySCB_Type *base, cy_stc_scb_spi_context_t *context)
{
    uint32_t numToCopy;
    uint32_t fifoSize = Cy_SCB_GetFifoSize(base);

    numToCopy = fifoSize - Cy_SCB_GetNumInTxFifo(base);

    /* Adjust the number to load */
    if (numToCopy > context->txBufSize)
    {
        numToCopy = context->txBufSize;
    }

    /* Move the buffer */
    context->txBufIdx  += numToCopy;
    context->txBufSize -= numToCopy;

    /* Load TX FIFO with data */
    if (NULL != context->txBuf)
    {
        uint8_t *buf = (uint8_t *) context->txBuf;

        Cy_SCB_WriteArrayNoCheck(base, context->txBuf, numToCopy);

        buf = &buf[(Cy_SCB_IsTxDataWidthByte(base) ? (numToCopy) : (2UL * numToCopy))];
        context->txBuf = (void *) buf;
    }
    else
    {
        Cy_SCB_WriteDefaultArrayNoCheck(base, CY_SCB_SPI_DEFAULT_TX, numToCopy);
    }

    if (0UL == context->txBufSize)
    {
        /* Data is transferred into TX FIFO */
        context->status |= CY_SCB_SPI_TRANSFER_IN_FIFO;

        /* Disable the TX level interrupt */
        Cy_SCB_SetTxInterruptMask(base, (Cy_SCB_GetTxInterruptMask(base) & (uint32_t) ~CY_SCB_TX_INTR_LEVEL));

        if (NULL != context->cbEvents)
        {
            context->cbEvents(CY_SCB_SPI_TRANSFER_IN_FIFO_EVENT);
        }
    }
}


/*******************************************************************************
* Function Name: DiscardArrayNoCheck
****************************************************************************//**
*
* Reads the number of data elements from the SPI RX FIFO. The read data is
* discarded. Before calling this function, make sure that RX FIFO has
* enough data elements to read.
*
* \param base
* The pointer to the SPI SCB instance.
*
* \param size
* The number of data elements to read.
*
*******************************************************************************/
static void DiscardArrayNoCheck(CySCB_Type const *base, uint32_t size)
{
    while (size > 0UL)
    {
        (void) Cy_SCB_SPI_Read(base);
        --size;
    }
}


#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */

