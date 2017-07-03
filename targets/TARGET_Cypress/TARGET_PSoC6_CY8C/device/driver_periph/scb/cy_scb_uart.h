/***************************************************************************//**
* \file cy_scb_uart.h
* \version 1.0
*
* Provides UART API declarations of the SCB driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/**
* \addtogroup group_scb_uart
* \{
* Driver API for UART
*
* UART - Universal Synchronous/Asynchronous Receiver/Transmitter,
* commonly referred to as RS-232.
*
* Three different UART-like serial interface protocols are supported:
* * UART - the standard mode with an optional UART Hardware flow control.
* * SmartCard - the transfer is similar to the UART transfer,
*   but a NACK (negative acknowledgment) may be sent from the
*   receiver to the transmitter. Both transmitter and receiver drive the same
*   line, although never at the same time.
* * IrDA - the Infra-red Data Association protocol adds a modulation
*   scheme to the UART signaling. At the transmitter, bits are modulated.
*   At the receiver, bits are demodulated. The modulation scheme uses the
*   Return-to-Zero-Inverted (RZI) format. Bit value "0" is signaled by a
*   short "1" pulse on the line and bit value "1" is signaled by holding
*   the line to "0".
*
* \section group_scb_uart_configuration Configuration Considerations
*
* To set up a UART, provide configuration parameters in the
* \ref cy_stc_scb_uart_config_t structure. For example: provide uartMode,
* overSample, dataWidth, enableMsbFirst, parity, and stopBits. The other
* parameters are optional for operation.
* To initialize the driver, call the \ref Cy_SCB_UART_Init function providing
* filled \ref cy_stc_scb_uart_config_t structure and allocated
* \ref cy_stc_scb_uart_context_t.
* The clock source connected to SCB UART must be configured to provide
* the sufficient frequency and set the appropriate oversampling to reach the
* desired data.
* Call \ref Cy_SCB_UART_Enable to start the UART operation after configuration
* is completed.
*
* The UART API is divided into two categories: low-level and high-level.
* Do not mix high-level and low-level API because low-level APIs can adversely
* affect the operation of higher level API.
*
* The low-level API's functions allow interacting directly with the hardware
* and do not use interrupts. These functions do not require context for
* operation, thus NULL can be passed in \ref Cy_SCB_UART_Init and
* \ref Cy_SCB_UART_Disable instead of a pointer to the context structure.
*
* * To write data into the TX FIFO, use one of the provided functions:
*   \ref Cy_SCB_UART_Put, \ref Cy_SCB_UART_PutArray,
*   \ref Cy_SCB_UART_PutArrayBlocking or \ref Cy_SCB_UART_PutString.
*   Note that putting data into the TX FIFO starts data transfer.
*
* * To read data from the RX FIFO, use one of the provided functions:
*   \ref Cy_SCB_UART_Get, \ref Cy_SCB_UART_GetArray or
*   \ref Cy_SCB_UART_GetArrayBlocking.
*
* * The statuses can be polled by using: \ref Cy_SCB_UART_GetRxFifoStatus and
*   \ref Cy_SCB_UART_GetTxFifoStatus.
*   The statuses are W1C (Write 1 to Clear) and after status is set it must
*   be cleared. Note that there are statuses that are evaluated as level. These
*   statuses remain set when an event is true. Therefore after clear operation
*   the status is cleared but then restored (if event is true).
*   For example: the TX FIFO empty can be cleared when the TX FIFO is
*   NOT empty. You have to put at least (two data elements, one goes to shifter
*   and next in FIFO) before clearing this status.
*
* The High Level API uses an interrupt to execute transfer. Call
* \ref Cy_SCB_UART_Transmit to start transmission. Call \ref Cy_SCB_UART_Receive
* to start receive operation. After the operation is started the
* \ref Cy_SCB_UART_Interrupt handles the data transfer until its
* completion. Therefore \ref Cy_SCB_UART_Interrupt must be called inside the
* user interrupt handler to make the high level API work. To monitor status
* of transmit operation, use \ref Cy_SCB_UART_GetTransmitStatus and
* \ref Cy_SCB_UART_GetReceiveStatus to monitor receive status appropriately.
* Alternatively use \ref Cy_SCB_UART_RegisterCallback to register callback
* function to be notified about \ref group_scb_uart_macro_callback_events.
*
* There is also capability to insert a receive ring buffer that operates between
* the RX FIFO and the user buffer. The received data is copied into the ring
* buffer from the RX FIFO. This process runs in the background after the ring
* buffer operation is started by \ref Cy_SCB_UART_StartRingBuffer.
* When \ref Cy_SCB_UART_Receive is called, it first reads data from the ring
* buffer and then sets up an interrupt to receive more data if the required
* amount has not been read yet.
*
* \section group_scb_uart_more_information More Information
*
* For more information on the SCB peripheral, refer to the technical reference
* manual (TRM).
*
* \section group_scb_uart_MISRA MISRA-C Compliance
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>11.4</td>
*     <td>A</td>
*     <td>A cast should not be performed between a pointer to object type and
*         a different pointer to object type.</td>
*     <td>The pointer to the buffer memory is void to allow handling different
*         different data types: uint8_t (4-8 bits) or uint16_t (9-16 bits).
*         </td>
*   </tr>
*   <tr>
*     <td>14.2</td>
*     <td>R</td>
*     <td>All non-null statements shall either: a) have at least one side-effect
*         however executed, or b) cause control flow to change.</td>
*     <td>The unused function parameters are cast to void. This statement
*         has no side-effect and is used to suppress a compiler warning.</td>
*   </tr>
*   <tr>
*     <td>20.3</td>
*     <td>R</td>
*     <td>The validity of values passed to library functions shall be checked.
*     </td>
*     <td>The CY_ASSERT() macro is used for verification of pointer to context
*         parameter against NULL. This macro halts code execution if
*         verification fails what ensures that NULL pointer is not dereference.
*     </td>
*   </tr>
* </table>
*
* \section group_scb_uart_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_scb_uart_macro Macro
* \defgroup group_scb_uart_functions Functions
* \defgroup group_scb_uart_data_structures Data Structures
* \defgroup group_scb_uart_enums Enumerated Types
*/

#if !defined(CY_SCB_UART_H)
#define CY_SCB_UART_H

#include "cy_scb_common.h"

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*          Enumerated Types
***************************************/

/**
* \addtogroup group_scb_uart_enums
* \{
*/

/** UART status codes */
typedef enum
{
    /** Operation completed successfully */
    CY_SCB_UART_SUCCESS = 0x00U,

    /** One or more of input parameters are invalid */
    CY_SCB_UART_BAD_PARAM = CY_SCB_ID | CY_PDL_STATUS_ERROR | CY_SCB_UART_ID | 1UL,

    /**
    * The UART is busy processing a transmit operation. Call this function
    * again once that operation is completed or aborted.
    */
    CY_SCB_UART_RECEIVE_BUSY,

    /**
    * The UART is busy processing a receive operation. Call this function
    * again once that operation is completed or aborted.
    */
    CY_SCB_UART_TRANSMIT_BUSY
} cy_en_scb_uart_status_t;
/** \} group_scb_uart_enums */


/***************************************
*       Type Definitions
***************************************/

/**
* \addtogroup group_scb_uart_data_structures
* \{
*/

/**
* Provides the typedef for the callback function called in the
* \ref Cy_SCB_UART_Interrupt to notify the user about occurrences of
* \ref group_scb_uart_macro_callback_events.
*/
typedef void (* scb_uart_handle_events_t)(uint32_t event);

/** UART configuration structure */
typedef struct stc_scb_uart_config
{
    /** Specifies the UART's mode of operation
    * (See \ref group_scb_uart_macro_modes for the set of constants)
    */
    uint32_t    uartMode;

    /**
    * Oversample factor for UART.
    * * The UART baud rate is the SCB Clock frequency / oversample
    *  (valid range is 8-16).
    * * For IrDA, the oversample is always 16, unless
    * \ref irdaEnableLowPowerReceiver is enabled. Then the oversample is
    * reduced to the \ref group_scb_uart_macro_irda_lp_ovs set.
    */
    uint32_t    oversample;

    /** The width of UART data (valid range is 5 to 9) */
    uint32_t    dataWidth;

    /**
    * Enables the hardware to shift out data element MSB first, otherwise,
    * LSB first
    */
    bool        enableMsbFirst;

    /**
    * Specifies the number of stop bits in the UART transaction, in half-bit
    * increments (See \ref group_scb_uart_macro_stop_bits for the set of
    * constants.)
    */
    uint32_t    stopBits;

    /** Configures the UART parity
    * (See \ref group_scb_uart_macro_parity for the set of constants.)
    */
    uint32_t    parity;

    /**
    * Enables a digital 3-tap median filter to be applied to the input
    * of the RX FIFO to filter glitches on the line
    */
    bool        enableInputFilter;

    /**
    * Enables the hardware to drop data in the RX FIFO when a parity error is
    * detected
    */
    bool        dropOnParityError;

    /**
    * Enables the hardware to drop data in the RX FIFO when a frame error is
    * detected
    */
    bool        dropOnFrameError;

    /**
    * Enables the UART operation in Multi-Processor mode which requires
    * dataWidth to be 9 bits (the 9th bit is used to indicate address byte)
    */
    bool        enableMutliProcessorMode;

    /**
    * If Multi Processor mode is enabled, this is the address of the RX
    * FIFO. If the address matches, data is accepted into the FIFO. If
    * it does not match, the data is ignored.
    */
    uint32_t    receiverAddress;

    /** This is the address mask for the Multi Processor address. 1 indicates
    * that the incoming address must match the corresponding bit in the slave
    * address. A 0 in the mask indicates that the incoming address does
    * not need to match.
    */
    uint32_t    receiverAddressMask;

    /**
    * Enables the hardware to accept the matching address in the RX FIFO.
    * This is useful when the device supports more than one address.
    */
    bool        acceptAddrInFifo;

    /** Inverts the IrDA RX input */
    bool        irdaInvertRx;

    /**
    * Enables the low-power receive for IrDA mode.
    * Note that the transmission must be disabled if this mode is enabled.
    */
    bool        irdaEnableLowPowerReceiver;

    /**
    * Enables retransmission of the frame placed in the TX FIFO when
    * NACK is received in SmartCard mode
    */
    bool        smartCardRetryOnNack;

    /**
    * Enables the usage of the CTS input signal for the transmitter. The
    * transmitter waits for CTS to be active before sending data
    */
    bool        enableCts;

    /** Sets the CTS Polarity
    * (See \ref group_scb_uart_macro_polarity for the set of constants.)
    */
    uint32_t    ctsPolarity;

    /**
    * When the RX FIFO has fewer entries than rtsRxFifoLevel, the
    * RTS signal is active (note to disable RTS, set this field to zero)
    */
    uint32_t    rtsRxFifoLevel;

    /** Sets the RTS Polarity
    * (See \ref group_scb_uart_macro_polarity for the set of constants.)
    */
    uint32_t    rtsPolarity;

    /** Specifies the number of bits to detect a break condition */
    uint32_t    breakWidth;

    /**
    * When there are more entries in the RX FIFO than this level
    * the RX trigger output goes high. This output can be connected
    * to a DMA channel through a trigger mux.
    * Also, it controls the \ref CY_SCB_UART_RX_TRIGGER interrupt source.
    */
    uint32_t    rxFifoTriggerLevel;

    /**
    * The bits set in this mask allow the event to cause an interrupt
    * (See \ref group_scb_uart_macro_rx_fifo_status for the set of constants.)
    */
    uint32_t    rxFifoIntEnableMask;

    /**
    * When there are fewer entries in the TX FIFO then this level
    * the TX trigger output goes high. This output can be connected
    * to a DMA channel through a trigger mux.
    * Also it controls \ref CY_SCB_UART_TX_TRIGGER interrupt source.
    */
    uint32_t    txFifoTriggerLevel;

    /**
    * Bits set in this mask allows the event to cause an interrupt
    * (See \ref group_scb_uart_macro_tx_fifo_status for the set of constants)
    */
    uint32_t    txFifoIntEnableMask;


} cy_stc_scb_uart_config_t;

/** UART internal context structure */
typedef struct cy_stc_scb_uart_context
{
    uint32_t volatile txStatus;         /**< Transmit status */
    uint32_t volatile rxStatus;         /**< Receive status */

    void *rxRingBuf;                    /**< Pointer to ring buffer */
    uint32_t volatile rxRingBufSize;    /**< Ring buffer size */
    uint32_t volatile rxRingBufHead;    /**< Ring buffer head index */
    uint32_t volatile rxRingBufTail;    /**< Ring buffer tail index */

    void *rxBuf;                        /**< Pointer to receive buffer */
    uint32_t volatile rxBufSize;        /**< Receive buffer size */
    uint32_t volatile rxBufIdx;         /**< Index within receive buffer */

    void *txBuf;                        /**< Pointer to transmit buffer */
    uint32_t volatile txBufSize;        /**< Transmit buffer size */
    uint32_t volatile txLeftToTransmit; /**< Number of data elements left to be transmitted */

    /**
    * The pointer to an event callback which is called when a UART event occurs
    */
    scb_uart_handle_events_t cbEvents;

#if !defined(NDEBUG)
    uint32_t initKey;               /**< Tracks context initialization */
#endif /* !(NDEBUG) */
} cy_stc_scb_uart_context_t;
/** \} group_scb_uart_data_structures */


/***************************************
*        Function Prototypes
***************************************/

/**
* \addtogroup group_scb_uart_functions
* \{
*/

/* Basic functions */
cy_en_scb_uart_status_t Cy_SCB_UART_Init(CySCB_Type *base, cy_stc_scb_uart_config_t const *config,
                                         cy_stc_scb_uart_context_t *context);
void     Cy_SCB_UART_DeInit (CySCB_Type *base);
__STATIC_INLINE void Cy_SCB_UART_Enable(CySCB_Type *base);
void     Cy_SCB_UART_Disable(CySCB_Type *base, cy_stc_scb_uart_context_t *context);

/* Configuration functions */
__STATIC_INLINE void     Cy_SCB_UART_EnableCts      (CySCB_Type *base);
__STATIC_INLINE void     Cy_SCB_UART_DisableCts     (CySCB_Type *base);
__STATIC_INLINE void     Cy_SCB_UART_SetRtsFifoLevel(CySCB_Type *base, uint32_t rxFifoLevel);
__STATIC_INLINE uint32_t Cy_SCB_UART_GetRtsFifoLevel(CySCB_Type const *base);

__STATIC_INLINE void     Cy_SCB_UART_EnableSkipStart (CySCB_Type *base);
__STATIC_INLINE void     Cy_SCB_UART_DisableSkipStart(CySCB_Type *base);

/* Low-level: RX direction functions */
__STATIC_INLINE uint32_t Cy_SCB_UART_Get             (CySCB_Type const *base);
__STATIC_INLINE uint32_t Cy_SCB_UART_GetArray        (CySCB_Type const *base, void *rxBuf, uint32_t size);
__STATIC_INLINE void     Cy_SCB_UART_GetArrayBlocking(CySCB_Type const *base, void *rxBuf, uint32_t size);

__STATIC_INLINE uint32_t Cy_SCB_UART_GetRxFifoStatus  (CySCB_Type const *base);
__STATIC_INLINE void     Cy_SCB_UART_ClearRxFifoStatus(CySCB_Type *base, uint32_t clearMask);
__STATIC_INLINE uint32_t Cy_SCB_UART_GetNumInRxFifo   (CySCB_Type const *base);
__STATIC_INLINE void     Cy_SCB_UART_ClearRxFifo      (CySCB_Type *base);

/* Low-level: TX direction functions */
__STATIC_INLINE uint32_t Cy_SCB_UART_Put             (CySCB_Type *base, uint32_t data);
__STATIC_INLINE uint32_t Cy_SCB_UART_PutArray        (CySCB_Type *base, void *txBuf, uint32_t size);
__STATIC_INLINE void     Cy_SCB_UART_PutArrayBlocking(CySCB_Type *base, void *txBuf, uint32_t size);
__STATIC_INLINE void     Cy_SCB_UART_PutString       (CySCB_Type *base, char_t const string[]);

__STATIC_INLINE uint32_t Cy_SCB_UART_GetTxFifoStatus  (CySCB_Type const *base);
__STATIC_INLINE void     Cy_SCB_UART_ClearTxFifoStatus(CySCB_Type *base, uint32_t clearMask);
__STATIC_INLINE uint32_t Cy_SCB_UART_GetNumInTxFifo   (CySCB_Type const *base);
__STATIC_INLINE bool     Cy_SCB_UART_IsTxComplete     (CySCB_Type const *base);
__STATIC_INLINE void     Cy_SCB_UART_ClearTxFifo      (CySCB_Type *base);

/* High-level: Callback */
__STATIC_INLINE void Cy_SCB_UART_RegisterCallback(CySCB_Type const *base, scb_uart_handle_events_t callback,
                                                  cy_stc_scb_uart_context_t *context);

/* High-level: Ring buffer functions */
void     Cy_SCB_UART_StartRingBuffer   (CySCB_Type *base, void* ringBuffer, uint32_t size,
                                        cy_stc_scb_uart_context_t *context);
void     Cy_SCB_UART_StopRingBuffer    (CySCB_Type *base, cy_stc_scb_uart_context_t *context);
uint32_t Cy_SCB_UART_GetNumInRingBuffer(CySCB_Type const *base, cy_stc_scb_uart_context_t const *context);
void     Cy_SCB_UART_ClearRingBuffer   (CySCB_Type const *base, cy_stc_scb_uart_context_t *context);

/* High-level: RX direction functions */
cy_en_scb_uart_status_t Cy_SCB_UART_Receive(CySCB_Type *base, void *rxBuf, uint32_t size, cy_stc_scb_uart_context_t *context);
void     Cy_SCB_UART_AbortReceive    (CySCB_Type *base, cy_stc_scb_uart_context_t *context);
uint32_t Cy_SCB_UART_GetNumReceived  (CySCB_Type const *base, cy_stc_scb_uart_context_t const *context);
uint32_t Cy_SCB_UART_GetReceiveStatus(CySCB_Type const *base, cy_stc_scb_uart_context_t const *context);

/* High-level: TX direction functions */
cy_en_scb_uart_status_t Cy_SCB_UART_Transmit(CySCB_Type *base, void *txBuf, uint32_t size, cy_stc_scb_uart_context_t *context);
void     Cy_SCB_UART_AbortTransmit       (CySCB_Type *base, cy_stc_scb_uart_context_t *context);
uint32_t Cy_SCB_UART_GetNumLeftToTransmit(CySCB_Type const *base, cy_stc_scb_uart_context_t const *context);
uint32_t Cy_SCB_UART_GetTransmitStatus   (CySCB_Type const *base, cy_stc_scb_uart_context_t const *context);

/* High-level: Interrupt handler */
void Cy_SCB_UART_Interrupt(CySCB_Type *base, cy_stc_scb_uart_context_t *context);
/** \} group_scb_uart_functions */


/***************************************
*            API Constants
***************************************/

/**
* \addtogroup group_scb_uart_macro
* \{
*/

/**
* \defgroup group_scb_uart_macro_modes UART Mode Defines
* \{
*/
/** Configures the SCB for Standard UART operation */
#define CY_SCB_UART_STANDARD   (0UL)

/** Configures the SCB for SmartCard operation */
#define CY_SCB_UART_SMARTCARD  (1UL)

/** Configures the SCB for IrDA operation */
#define CY_SCB_UART_IRDA       (2UL)
/** \} group_scb_uart_macro_modes */

/**
* \defgroup group_scb_uart_macro_stop_bits UART Stop Bit Defines
* \{
*/
#define CY_SCB_UART_STOP_BITS_1        (2UL)    /**< UART looks for 1 Stop Bit    */
#define CY_SCB_UART_STOP_BITS_1_5      (3UL)    /**< UART looks for 1.5 Stop Bits */
#define CY_SCB_UART_STOP_BITS_2        (4UL)    /**< UART looks for 2 Stop Bits   */
#define CY_SCB_UART_STOP_BITS_2_5      (5UL)    /**< UART looks for 2.5 Stop Bits */
#define CY_SCB_UART_STOP_BITS_3        (6UL)    /**< UART looks for 3 Stop Bits   */
#define CY_SCB_UART_STOP_BITS_3_5      (7UL)    /**< UART looks for 3.5 Stop Bits */
#define CY_SCB_UART_STOP_BITS_4        (8UL)    /**< UART looks for 4 Stop Bits   */
/** \} group_scb_uart_macro_stop_bits */

/**
* \defgroup group_scb_uart_macro_parity UART Parity Defines
* \{
*/
#define CY_SCB_UART_PARITY_NONE        (0UL)    /**< UART has no parity check   */
#define CY_SCB_UART_PARITY_EVEN        (2UL)    /**< UART has even parity check */
#define CY_SCB_UART_PARITY_ODD         (3UL)    /**< UART has odd parity check  */
/** \} group_scb_uart_macro_parity */

/**
* \defgroup group_scb_uart_macro_irda_lp_ovs UART IRDA Low Power Oversample factors
* \{
*/
#define CY_SCB_UART_IRDA_LP_OVS16      (1UL)   /**< IrDA in low-power mode oversampled by 16   */
#define CY_SCB_UART_IRDA_LP_OVS32      (2UL)   /**< IrDA in low-power mode oversampled by 32   */
#define CY_SCB_UART_IRDA_LP_OVS48      (3UL)   /**< IrDA in low-power mode oversampled by 48   */
#define CY_SCB_UART_IRDA_LP_OVS96      (4UL)   /**< IrDA in low-power mode oversampled by 96   */
#define CY_SCB_UART_IRDA_LP_OVS192     (5UL)   /**< IrDA in low-power mode oversampled by 192  */
#define CY_SCB_UART_IRDA_LP_OVS768     (6UL)   /**< IrDA in low-power mode oversampled by 768  */
#define CY_SCB_UART_IRDA_LP_OVS1536    (7UL)   /**< IrDA in low-power mode oversampled by 1536 */
/** \} group_scb_uart_macro_irda_lp_ovs */

/**
* \defgroup group_scb_uart_macro_polarity UART Polarity Defines
* \{
*/
#define CY_SCB_UART_ACTIVE_LOW         (0UL)   /**< Signal is active low */
#define CY_SCB_UART_ACTIVE_HIGH        (1UL)   /**< Signal is active high */
/** \} group_scb_uart_macro_polarity */

/**
* \defgroup group_scb_uart_macro_rx_fifo_status UART Receive FIFO status.
* \{
*/
/** The number of entries in the RX FIFO is more than the RX FIFO trigger level
* value
*/
#define CY_SCB_UART_RX_TRIGGER         (SCB_INTR_RX_TRIGGER_Msk)

/** The RX FIFO is not empty, there is data to read */
#define CY_SCB_UART_RX_NOT_EMPTY       (SCB_INTR_RX_NOT_EMPTY_Msk)

/**
* The RX FIFO is full, there is no more space for additional data,
* any additional data will be dropped
*/
#define CY_SCB_UART_RX_FULL            (SCB_INTR_RX_FULL_Msk)

/**
* The RX FIFO was full and there was an attempt to write to it.
* That additional data was dropped.
*/
#define CY_SCB_UART_RX_OVERFLOW        (SCB_INTR_RX_OVERFLOW_Msk)

/** An attempt to read from an empty RX FIFO */
#define CY_SCB_UART_RX_UNDERFLOW       (SCB_INTR_RX_UNDERFLOW_Msk)

/** The RX FIFO detected a frame error, either a stop or stop-bit error */
#define CY_SCB_UART_RX_ERR_FRAME       (SCB_INTR_RX_FRAME_ERROR_Msk)

/** The RX FIFO detected a parity error */
#define CY_SCB_UART_RX_ERR_PARITY      (SCB_INTR_RX_PARITY_ERROR_Msk)

/** The RX FIFO detected a break transmission from the transmitter */
#define CY_SCB_UART_RX_BREAK_DETECT    (SCB_INTR_RX_BREAK_DETECT_Msk)
/** \} group_scb_uart_macro_rx_fifo_status */

/**
* \defgroup group_scb_uart_macro_tx_fifo_status UART TX FIFO Statuses
* \{
*/
/** The number of entries in the TX FIFO is less than the TX FIFO trigger level
* value
*/
#define CY_SCB_UART_TX_TRIGGER     (SCB_INTR_TX_TRIGGER_Msk)

/** The TX FIFO is not full, there is a space for more data */
#define CY_SCB_UART_TX_NOT_FULL    (SCB_INTR_TX_NOT_FULL_Msk)

/** The TX FIFO is empty, note there may still be data in the shift register.*/
#define CY_SCB_UART_TX_EMPTY       (SCB_INTR_TX_EMPTY_Msk)

/** An attempt to write to the full TX FIFO */
#define CY_SCB_UART_TX_OVERFLOW    (SCB_INTR_TX_OVERFLOW_Msk)

/** An attempt to read from an empty transmitter FIFO (hardware reads). */
#define CY_SCB_UART_TX_UNDERFLOW (SCB_INTR_TX_UNDERFLOW_Msk)

/** All data has been transmitted out of the FIFO, including shifter */
#define CY_SCB_UART_TX_DONE        (SCB_INTR_TX_UART_DONE_Msk)

/** SmartCard only: the transmitter received a NACK */
#define CY_SCB_UART_TX_NACK        (SCB_INTR_TX_UART_NACK_Msk)

/** SmartCard only: the transmitter lost arbitration */
#define CY_SCB_UART_TX_ARB_LOST    (SCB_INTR_TX_UART_ARB_LOST_Msk)
/** \} group_scb_uart_macro_tx_fifo_status */

/**
* \defgroup group_scb_uart_macro_receive_status UART Receive Statuses
* \{
*/
/** The receive operation triggered by \ref Cy_SCB_UART_Receive is in progress */
#define CY_SCB_UART_RECEIVE_ACTIVE         (0x01UL)

/**
* The hardware RX FIFO was full and there was an attempt to write to it.
* That additional data was dropped.
*/
#define CY_SCB_UART_RECEIVE_OVERFLOW       (SCB_INTR_RX_OVERFLOW_Msk)

/** The receive hardware detected a frame error, either a start or
* stop bit error
*/
#define CY_SCB_UART_RECEIVE_ERR_FRAME      (SCB_INTR_RX_FRAME_ERROR_Msk)

/** The receive hardware detected a parity error */
#define CY_SCB_UART_RECEIVE_ERR_PARITY     (SCB_INTR_RX_PARITY_ERROR_Msk)

/** The receive hardware detected a break transmission from transmitter */
#define CY_SCB_UART_RECEIVE_BREAK_DETECT   (SCB_INTR_RX_BREAK_DETECT_Msk)
/** \} group_scb_uart_macro_receive_status */

/**
* \defgroup group_scb_uart_macro_transmit_status UART Transmit Status
* \{
*/
/** The transmit operation triggered by \ref Cy_SCB_UART_Transmit is in progress */
#define CY_SCB_UART_TRANSMIT_ACTIVE    (0x01UL)

/** All data elements specified by \ref Cy_SCB_UART_Transmit are in the TX FIFO */
#define CY_SCB_UART_TRANSMIT_IN_FIFO   (0x02UL)

/** SmartCard only: the transmitter received a NACK */
#define CY_SCB_UART_TRANSMIT_NACK      (SCB_INTR_TX_UART_NACK_Msk)

/** SmartCard only: the transmitter lost arbitration */
#define CY_SCB_UART_TRANSMIT_ARB_LOST  (SCB_INTR_TX_UART_ARB_LOST_Msk)
/** \} group_scb_uart_macro_transmit_status */

/**
* \defgroup group_scb_uart_macro_callback_events UART Callback Events
* \{
* Only single event is notified by the callback.
*/
/** All data elements specified by \ref Cy_SCB_UART_Transmit are in the TX FIFO */
#define CY_SCB_UART_TRANSMIT_IN_FIFO_EVENT (0x01UL)

/**
* All data elements specified by \ref Cy_SCB_UART_Transmit have been transmitted.
*/
#define CY_SCB_UART_TRANSMIT_DONE_EVENT    (0x02UL)

/** All data elements specified by \ref Cy_SCB_UART_Receive have been received */
#define CY_SCB_UART_RECEIVE_DONE_EVENT     (0x04UL)

/**
* The ring buffer is full, there is no more space for additional data.
* Additional data is stored in the RX FIFO until it becomes full, at which point
* data is dropped.
*/
#define CY_SCB_UART_RB_FULL_EVENT          (0x08UL)

/**
* An error was detected during the receive operation. This includes overflow,
* frame error, or parity error. Check \ref Cy_SCB_UART_GetReceiveStatus to
* determine the source of the error.
*/
#define CY_SCB_UART_RECEIVE_ERR_EVENT      (0x10UL)

/**
* An error was detected during the transmit operation. This includes a NACK
* or lost arbitration. Check \ref Cy_SCB_UART_GetTransmitStatus to determine
* the source of the error
*/
#define CY_SCB_UART_TRANSMIT_ERR_EVENT     (0x20UL)
/** \} group_scb_uart_macro_callback_events */

/** Data returned by the hardware when an empty RX FIFO is read */
#define CY_SCB_UART_RX_NO_DATA         (0xFFFFFFFFUL)


/***************************************
*         Internal Constants
***************************************/

/** \cond INTERNAL */
#define CY_SCB_UART_TX_FIFO_STATUS (CY_SCB_UART_TX_TRIGGER  | CY_SCB_UART_TX_NOT_FULL  | CY_SCB_UART_TX_EMPTY | \
                                    CY_SCB_UART_TX_OVERFLOW | CY_SCB_UART_TX_UNDERFLOW | CY_SCB_UART_TX_DONE  | \
                                    CY_SCB_UART_TX_NACK     | CY_SCB_UART_TX_ARB_LOST)

#define CY_SCB_UART_RX_FIFO_STATUS (CY_SCB_UART_RX_TRIGGER    | CY_SCB_UART_RX_NOT_EMPTY | CY_SCB_UART_RX_FULL      | \
                                    CY_SCB_UART_RX_OVERFLOW   | CY_SCB_UART_RX_UNDERFLOW | CY_SCB_UART_RX_ERR_FRAME | \
                                    CY_SCB_UART_RX_ERR_PARITY | CY_SCB_UART_RX_BREAK_DETECT)

#define CY_SCB_UART_TX_INTR        (CY_SCB_TX_INTR_LEVEL | CY_SCB_TX_INTR_UART_NACK | CY_SCB_TX_INTR_UART_ARB_LOST)

#define CY_SCB_UART_RX_INTR        (CY_SCB_RX_INTR_LEVEL | CY_SCB_RX_INTR_OVERFLOW | CY_SCB_RX_INTR_UART_FRAME_ERROR | \
                                    CY_SCB_RX_INTR_UART_PARITY_ERROR | CY_SCB_RX_INTR_UART_BREAK_DETECT)

#define CY_SCB_UART_RECEIVE_ERR    (CY_SCB_RX_INTR_OVERFLOW          | CY_SCB_RX_INTR_UART_FRAME_ERROR | \
                                    CY_SCB_RX_INTR_UART_PARITY_ERROR | CY_SCB_RX_INTR_UART_BREAK_DETECT)

#define CY_SCB_UART_TRANSMIT_ERR   (CY_SCB_TX_INTR_UART_NACK | CY_SCB_TX_INTR_UART_ARB_LOST)

#define CY_SCB_UART_INIT_KEY       (0x00ABCDEFUL)
/** \endcond */

/** \} group_scb_uart_macro */


/***************************************
*    In-line Function Implementation
***************************************/

/**
* \addtogroup group_scb_uart_functions
* \{
*/

/*******************************************************************************
* Function Name: Cy_SCB_UART_Enable
****************************************************************************//**
*
* Enables the SCB block for the UART operation.
*
* \param base
* The pointer to the UART SCB instance.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SCB_UART_Enable(CySCB_Type *base)
{
    base->CTRL |= SCB_CTRL_ENABLED_Msk;
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_EnableCts
****************************************************************************//**
*
* Enables the Clear to Send (CTS) input for the UART. The UART will not transmit
* data while this signal is inactive.
*
* \param base
* The pointer to the UART SCB instance.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SCB_UART_EnableCts(CySCB_Type *base)
{
    base->UART_FLOW_CTRL |= SCB_UART_FLOW_CTRL_CTS_ENABLED_Msk;
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_DisableCts
****************************************************************************//**
*
* Disables the Clear to Send (CTS) input for the UART.
* See \ref Cy_SCB_UART_EnableCts for the details.
*
* \param base
* The pointer to the UART SCB instance.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SCB_UART_DisableCts(CySCB_Type *base)
{
    base->UART_FLOW_CTRL &= (uint32_t) ~SCB_UART_FLOW_CTRL_CTS_ENABLED_Msk;
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_SetRtsFifoLevel
****************************************************************************//**
*
* Sets a level for the Ready To Send (RTS) signal activation.
* When the number of data elements in the receive FIFO is below this level,
* then the RTS output is active. Otherwise, the RTS signal is inactive.
* To disable the RTS signal generation, set this level to zero.
*
* \param base
* The pointer to the UART SCB instance.
*
* \param rxFifoLevel
* The level in the RX FIFO for RTS signal activation.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SCB_UART_SetRtsFifoLevel(CySCB_Type *base, uint32_t rxFifoLevel)
{
    base->UART_FLOW_CTRL = _CLR_SET_FLD32U(base->UART_FLOW_CTRL, SCB_UART_FLOW_CTRL_TRIGGER_LEVEL, rxFifoLevel);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_GetRtsFifoLevel
****************************************************************************//**
*
* Returns the level in the RX FIFO for the RTS signal activation.
*
* \param base
* The pointer to the UART SCB instance.
*
* \return
* The level in the RX FIFO for RTS signal activation.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SCB_UART_GetRtsFifoLevel(CySCB_Type const *base)
{
    return _FLD2VAL(SCB_UART_FLOW_CTRL_TRIGGER_LEVEL, base->UART_FLOW_CTRL);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_EnableSkipStart
****************************************************************************//**
*
* Enables the skip start-bit functionality.
* The UART hardware does not synchronize to a start but synchronizes to
* the first rising edge. To create a rising edge, the first data bit must
* be a 1. This feature is useful when the Start edge is used to wake the
* device through a GPIO interrupt.
*
* \param base
* The pointer to the UART SCB instance.
*
* \note
* The skip start-bit feature is applied whenever the UART is disabled due
* to entrance into DeepSleep or after calling \ref Cy_SCB_UART_Disable.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SCB_UART_EnableSkipStart(CySCB_Type *base)
{
    base->UART_RX_CTRL |= SCB_UART_RX_CTRL_SKIP_START_Msk;
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_DisableSkipStart
****************************************************************************//**
*
* Disable the skip start-bit functionality.
* See \ref Cy_SCB_UART_EnableSkipStart for the details.
*
* \param base
* The pointer to the UART SCB instance.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SCB_UART_DisableSkipStart(CySCB_Type *base)
{
    base->UART_RX_CTRL &= (uint32_t) ~SCB_UART_RX_CTRL_SKIP_START_Msk;
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_Get
****************************************************************************//**
*
* Reads a single data element from the UART RX FIFO.
* This function does not check whether the RX FIFO has data before reading it.
* If the RX FIFO is empty, the function returns \ref CY_SCB_UART_RX_NO_DATA.
*
* \param base
* The pointer to the UART SCB instance.
*
* \return
* Data from the RX FIFO.
* The data element size is defined by the configured data width.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SCB_UART_Get(CySCB_Type const *base)
{
    return Cy_SCB_ReadRxFifo(base);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_GetArray
****************************************************************************//**
*
* Reads an array of data out of the UART RX FIFO.
* This function does not block. It returns how many data elements were read
* from the RX FIFO.
*
* \param base
* The pointer to the UART SCB instance.
*
* \param rxBuf
* The pointer to the location to place the data read from the RX FIFO.
* The item size is defined by the data type, which depends on the configured
* data width.
*
* \param size
* The number of data elements to read from the RX FIFO.
*
* \return
* The number of data elements read from the RX FIFO.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SCB_UART_GetArray(CySCB_Type const *base, void *rxBuf, uint32_t size)
{
    return Cy_SCB_ReadArray(base, rxBuf, size);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_GetArrayBlocking
****************************************************************************//**
*
* Reads an array of data out of the UART RX FIFO.
* This function blocks until the number of data elements specified by the
* size has been read from the RX FIFO.
*
* \param base
* The pointer to the UART SCB instance.
*
* \param rxBuf
* The pointer to the location to place the data read from the RX FIFO.
* The item size is defined by the data type which depends on the configured
* data width.
*
* \param size
* The number of data elements to read from the RX FIFO.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SCB_UART_GetArrayBlocking(CySCB_Type const *base, void *rxBuf, uint32_t size)
{
    Cy_SCB_ReadArrayBlocking(base, rxBuf, size);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_GetRxFifoStatus
****************************************************************************//**
*
* Returns the current status of the RX FIFO.
* Clear the active statuses to let the SCB hardware update them.
*
* \param base
* The pointer to the UART SCB instance.
*
* \return
* \ref group_scb_uart_macro_rx_fifo_status
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SCB_UART_GetRxFifoStatus(CySCB_Type const *base)
{
    return (Cy_SCB_GetRxInterruptStatus(base) & CY_SCB_UART_RX_FIFO_STATUS);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_ClearRxFifoStatus
****************************************************************************//**
*
* Clears the selected statuses of the RX FIFO.
*
* \param base
* The pointer to the UART SCB instance.
*
* \param clearMask
* The mask whose statuses to clear.
* See \ref group_scb_uart_macro_rx_fifo_status for the set of constants.
*
* \note
* * This status is also used for interrupt generation, so clearing it also
*   clears the interrupt sources.
* * Level-sensitive statuses such as \ref CY_SCB_UART_RX_TRIGGER,
*   \ref CY_SCB_UART_RX_NOT_EMPTY and \ref CY_SCB_UART_RX_FULL set high again after
*   being cleared if the condition remains true.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SCB_UART_ClearRxFifoStatus(CySCB_Type *base, uint32_t clearMask)
{
    Cy_SCB_ClearRxInterrupt(base, clearMask);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_GetNumInRxFifo
****************************************************************************//**
*
* Returns the number of data elements in the UART RX FIFO.
*
* \param base
* The pointer to the UART SCB instance.
*
* \return
* The number of data elements in the RX FIFO.
* The size of date element defined by the configured data width.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SCB_UART_GetNumInRxFifo(CySCB_Type const *base)
{
    return Cy_SCB_GetNumInRxFifo(base);
}

/*******************************************************************************
* Function Name: Cy_SCB_UART_ClearRxFifo
****************************************************************************//**
*
* Clears all data out of the UART RX FIFO.
*
* \param base
* The pointer to the UART SCB instance.
*
* \sideeffect
* Any data currently in the shifter is cleared and lost.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SCB_UART_ClearRxFifo(CySCB_Type *base)
{
    Cy_SCB_ClearRxFifo(base);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_Put
****************************************************************************//**
*
* Places a single data element in the UART TX FIFO.
* This function does not block and returns how many data elements were placed
* in the TX FIFO.
*
* \param base
* The pointer to the UART SCB instance.
*
* \param data
* Data to put in the TX FIFO.
* The item size is defined by the data type which depends on the configured
* data width.
*
* \return
* The number of data elements placed in the TX FIFO: 0 or 1.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SCB_UART_Put(CySCB_Type *base, uint32_t data)
{
    return Cy_SCB_Write(base, data);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_PutArray
****************************************************************************//**
*
* Places an array of data in the UART TX FIFO.
* This function does not block and it returns how many data elements were
* placed in the TX FIFO.
*
* \param base
* The pointer to the UART SCB instance.
*
* \param txBuf
* The pointer to data to place in the TX FIFO.
* The item size is defined by the data type which depends on the configured
* TX data width.
*
* \param size
* The number of data elements to TX.
*
* \return
* The number of data elements placed in the TX FIFO.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SCB_UART_PutArray(CySCB_Type *base, void *txBuf, uint32_t size)
{
    return Cy_SCB_WriteArray(base, txBuf, size);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_PutArrayBlocking
****************************************************************************//**
*
* Places an array of data in the UART TX FIFO.
* This function blocks until the number of data elements specified by the size
* is placed in the TX FIFO.
*
* \param base
* The pointer to the UART SCB instance.
*
* \param txBuf
* The pointer to data to place in the TX FIFO.
* The item size is defined by the data type which depends on the configured
* data width.
*
* \param size
* The number of data elements to write into the TX FIFO.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SCB_UART_PutArrayBlocking(CySCB_Type *base, void *txBuf, uint32_t size)
{
    Cy_SCB_WriteArrayBlocking(base, txBuf, size);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_PutString
****************************************************************************//**
*
* Places a NULL terminated string in the UART TX FIFO.
* This function blocks until the entire string is placed in the TX FIFO.
*
* \param base
* The pointer to the UART SCB instance.
*
* \param string
* The pointer to the null terminated string array.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SCB_UART_PutString(CySCB_Type *base, char_t const string[])
{
    Cy_SCB_WriteString(base, string);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_GetTxFifoStatus
****************************************************************************//**
*
* Returns the current status of the TX FIFO.
* Clear the active statuses to let the SCB hardware update them.
*
* \param base
* The pointer to the UART SCB instance.
*
* \return
* \ref group_scb_uart_macro_tx_fifo_status
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SCB_UART_GetTxFifoStatus(CySCB_Type const *base)
{
    return (Cy_SCB_GetTxInterruptStatus(base) & CY_SCB_UART_TX_FIFO_STATUS);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_ClearTxFifoStatus
****************************************************************************//**
*
* Clears the selected statuses of the TX FIFO.
*
* \param base
* The pointer to the UART SCB instance.
*
* \param clearMask
* The mask whose statuses to clear.
* See \ref group_scb_uart_macro_tx_fifo_status for the set of constants.
*
* \note
* * The status is also used for interrupt generation, so clearing it also
*   clears the interrupt sources.
* * Level-sensitive statuses such as \ref CY_SCB_UART_TX_TRIGGER,
*   \ref CY_SCB_UART_TX_EMPTY and \ref CY_SCB_UART_TX_NOT_FULL set high again after
*   being cleared if the condition remains true.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SCB_UART_ClearTxFifoStatus(CySCB_Type *base, uint32_t clearMask)
{
    Cy_SCB_ClearTxInterrupt(base, clearMask);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_GetNumInTxFifo
****************************************************************************//**
*
* Returns the number of data elements in the UART TX FIFO.
*
* \param base
* The pointer to the UART SCB instance.
*
* \return
* The number of data elements in the TX FIFO.
* The size of date element defined by the configured data width.
*
* \note
* This number does not include any data currently in the TX shifter.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SCB_UART_GetNumInTxFifo(CySCB_Type const *base)
{
    return Cy_SCB_GetNumInTxFifo(base);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_IsTxComplete
****************************************************************************//**
*
* Checks if the TX FIFO and Shifter are empty and there is no more data to send
*
* \param base
* Pointer to the UART SCB instance.
*
* \return
* If true, transmission complete. If false, transmission is not complete.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SCB_UART_IsTxComplete(CySCB_Type const *base)
{
    return Cy_SCB_IsTxComplete(base);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_ClearTxFifo
****************************************************************************//**
*
* Clears all data out of the UART TX FIFO.
*
* \param base
* The pointer to the UART SCB instance.
*
* \sideeffect
* The TX FIFO clear operation also clears the shift register, so that
* the shifter could be cleared in the middle of a data element transfer,
* corrupting it. The data element corruption means that all bits that have
* not been transmitted are transmitted as 1s on the bus.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SCB_UART_ClearTxFifo(CySCB_Type *base)
{
    Cy_SCB_ClearTxFifo(base);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_RegisterCallback
****************************************************************************//**
*
* Registers a callback function that notifies that
* \ref group_scb_uart_macro_callback_events occurred in the
* \ref Cy_SCB_UART_Interrupt.
*
* \param base
* The pointer to the UART SCB instance.
*
* \param callback
* The pointer to the callback function.
* See \ref scb_uart_handle_events_t for the function prototype.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_uart_context_t allocated
* by the user. The structure is used during the UART operation for internal
* configuration and data keeping. The user should not modify anything
* in this structure.
*
* \note
* To remove the callback, pass NULL as the pointer to the callback function.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SCB_UART_RegisterCallback(CySCB_Type const *base,
          scb_uart_handle_events_t callback, cy_stc_scb_uart_context_t *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) base;

    context->cbEvents = callback;
}

/** \} group_scb_uart_functions */

#if defined(__cplusplus)
}
#endif

/** \} group_scb_uart */

#endif /* (CY_SCB_UART_H) */


/* [] END OF FILE */

