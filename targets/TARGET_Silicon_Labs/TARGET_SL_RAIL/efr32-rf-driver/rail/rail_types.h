/**************************************************************************//**
 * @file rail_types.h
 * @brief This file contains the type definitions for RAIL structures, enums,
 *        and other types.
 * @copyright Copyright 2017 Silicon Laboratories, Inc. www.silabs.com
 *****************************************************************************/

#ifndef __RAIL_TYPES_H__
#define __RAIL_TYPES_H__

// Include standard type headers to help define structures
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/// The RAIL library does not use enums because the ARM EABI leaves their
/// size ambiguous. This ambiguity causes problems if the application is built
/// with different flags than the library. Instead, uint8_t typedefs
/// are used in compiled code for all enums. For documentation purposes this is
/// converted to an actual enum since it's much easier to read in Doxygen.
#define RAIL_ENUM(name) enum name
/// This macro is a more generic version of the \ref RAIL_ENUM() macro that
/// allows the size of the type to be overridden instead of forcing the use of
/// a uint8_t. See \ref RAIL_ENUM() for more information about why RAIL enums
/// are defined using these.
#define RAIL_ENUM_GENERIC(name, type) enum name
#else
/// Define used for the RAIL library, which sets each enum to a uint8_t
/// typedef and creates a named enum structure for the enumeration values.
#define RAIL_ENUM(name) typedef uint8_t name; enum name##_enum
#define RAIL_ENUM_GENERIC(name, type) typedef type name; enum name##_enum
// For debugging use the following define to turn this back into a proper enum
// #define RAIL_ENUM(name) typedef enum name##_enum name; enum name##_enum
#endif

/**
 * @addtogroup RAIL_API
 * @{
 */

/******************************************************************************
 * Multiprotocol Structures
 *****************************************************************************/
/**
 * @addtogroup Multiprotocol
 * @{
 */

/**
 * @typedef RAIL_Time_t
 * @brief Time in microseconds
 */
typedef uint32_t RAIL_Time_t;

/**
 * @def RAIL_SCHEDULER_STATE_UINT32_BUFFER_SIZE
 * @brief The size in 32-bit words of RAIL_SchedulerStateBuffer_t to store
 *   RAIL multiprotocol internal state.
 */
#define RAIL_SCHEDULER_STATE_UINT32_BUFFER_SIZE 25

/**
 * @typedef RAIL_SchedulerStateBuffer_t
 * @brief Buffer used to store multiprotocol scheduler internal state.
 *
 * This buffer must be allocated in application global read-write memory
 * that persists for the duration of RAIL usage. It cannot be allocated
 * in read-only memory or on the call stack.
 */
typedef uint32_t RAIL_SchedulerStateBuffer_t[RAIL_SCHEDULER_STATE_UINT32_BUFFER_SIZE];

/**
 * @struct RAILSched_Config_t
 * @brief Multiprotocol scheduler configuration and internal state.
 *
 * This buffer must be allocated in application global read-write memory
 * that persists for the duration of RAIL usage. It cannot be allocated
 * in read-only memory or on the call stack.
 */
typedef struct RAILSched_Config {
  RAIL_SchedulerStateBuffer_t buffer; /**< Internal state buffer. */
} RAILSched_Config_t;

/**
 * @struct RAIL_SchedulerInfo_t
 * @brief A structure to hold information used by the scheduler.
 *
 * For multiprotocol versions of RAIL this can be used to control how a receive
 * or transmit operation is run. In single protocol applications it's not
 * necessary.
 */
typedef struct RAIL_SchedulerInfo {
  /**
   * The priority for this operation in the scheduler. This priority is used to
   * preempt a long running lower priority task to ensure higher priority
   * operations complete in time. A lower numerical value represents a higher
   * logical priority meaning 0 is the highest priority and 255 is the lowest.
   */
  uint8_t priority;
  /**
   * The amount of time in us that this operation can slip by into the future
   * and still be run. This time is relative to the start time which may be
   * the current time for relative transmits. If we cannot start by this time
   * the operation will be considered a failure.
   */
  RAIL_Time_t slipTime;
  /**
   * The transaction time in us for this operation. Since transaction times may
   * not be known exactly you will likely want to use a minimum or expected
   * guess for this time. The scheduler will use the value entered here to look
   * for overlaps between low priority and high priority tasks and attempt to
   * find a schedule where all tasks get to run.
   */
  RAIL_Time_t transactionTime;
} RAIL_SchedulerInfo_t;

/**
 * @enum RAIL_SchedulerStatus_t
 * @brief Multiprotocol scheduler status returned by RAIL_GetSchedulerStatus().
 */
RAIL_ENUM(RAIL_SchedulerStatus_t) {
  /** Multiprotocol scheduler reports no error */
  RAIL_SCHEDULER_STATUS_NO_ERROR,
  /**
   * The scheduler is disabled or the requested scheduler operation is
   * unsupported.
   */
  RAIL_SCHEDULER_STATUS_UNSUPPORTED,
  /**
   * The scheduled event was started but was interrupted by a higher priority
   * event before it could be completed.
   */
  RAIL_SCHEDULER_STATUS_EVENT_INTERRUPTED,
  /**
   * This task could not be scheduled given its priority and the other tasks
   * running on the system.
   */
  RAIL_SCHEDULER_STATUS_SCHEDULE_FAIL,
  /**
   * Calling the scheduled transmit function returned an error code. See
   * RAIL_StartScheduledTx() for more information about possible errors.
   */
  RAIL_SCHEDULER_STATUS_SCHEDULED_TX_FAIL,
  /**
   * Calling the start transmit function returned an error code. See
   * RAIL_StartTx() for more information about possible errors.
   */
  RAIL_SCHEDULER_STATUS_SINGLE_TX_FAIL,
  /**
   * Calling the CSMA transmit function returned an error code. See
   * RAIL_StartCcaCsmaTx() for more information about possible errors.
   */
  RAIL_SCHEDULER_STATUS_CCA_CSMA_TX_FAIL,
  /**
   * Calling the LBT transmit function returned an error code. See
   * RAIL_StartCcaLbtTx() for more information about possible errors.
   */
  RAIL_SCHEDULER_STATUS_CCA_LBT_TX_FAIL,
  /**
   * Calling the scheduled receive function returned an error code. This likely
   * means that we were a little too late and could not setup the hardware in
   * time to do this receive.
   */
  RAIL_SCHEDULER_STATUS_SCHEDULED_RX_FAIL,
  /**
   * Calling the stream transmit function returned an error code. See
   * RAIL_StartTxStream() for more information about possible errors.
   */
  RAIL_SCHEDULER_STATUS_TX_STREAM_FAIL,
  /**
   * RSSI averaging failed. If this scheduler status occurs
   * RAIL_GetAverageRssi() will return \ref RAIL_RSSI_INVALID until
   * a RAIL_StartAverageRssi() completes successfully.
   */
  RAIL_SCHEDULER_STATUS_AVERAGE_RSSI_FAIL,
  /**
   * There was an internal error in the scheduler data structures. This should
   * not happen and indicates a problem if seen.
   */
  RAIL_SCHEDULER_STATUS_INTERNAL_ERROR,
};

/** @} */ // end of group Multiprotocol

/******************************************************************************
 * Event Structures
 *****************************************************************************/
/**
 * @addtogroup Events
 * @{
 */

/**
 * @enum RAIL_Events_t
 * @brief RAIL events passed to event callback. More than one event may be
 *   indicated due to interrupt latency.
 */
RAIL_ENUM_GENERIC(RAIL_Events_t, uint64_t) {
  // RX Event Bit Shifts

  /** Shift position of \ref RAIL_EVENT_RSSI_AVERAGE_DONE bit */
  RAIL_EVENT_RSSI_AVERAGE_DONE_SHIFT = 0,
  /** Shift position of \ref RAIL_EVENT_RX_ACK_TIMEOUT bit */
  RAIL_EVENT_RX_ACK_TIMEOUT_SHIFT,
  /** Shift position of \ref RAIL_EVENT_RX_FIFO_ALMOST_FULL bit */
  RAIL_EVENT_RX_FIFO_ALMOST_FULL_SHIFT,
  /** Shift position of \ref RAIL_EVENT_RX_PACKET_RECEIVED bit */
  RAIL_EVENT_RX_PACKET_RECEIVED_SHIFT,
  /** Shift position of \ref RAIL_EVENT_RX_PREAMBLE_LOST bit */
  RAIL_EVENT_RX_PREAMBLE_LOST_SHIFT,
  /** Shift position of \ref RAIL_EVENT_RX_PREAMBLE_DETECT bit */
  RAIL_EVENT_RX_PREAMBLE_DETECT_SHIFT,
  /** Shift position of \ref RAIL_EVENT_RX_SYNC1_DETECT bit */
  RAIL_EVENT_RX_SYNC1_DETECT_SHIFT,
  /** Shift position of \ref RAIL_EVENT_RX_SYNC2_DETECT bit */
  RAIL_EVENT_RX_SYNC2_DETECT_SHIFT,
  /** Shift position of \ref RAIL_EVENT_RX_FRAME_ERROR bit */
  RAIL_EVENT_RX_FRAME_ERROR_SHIFT,
  /** Shift position of \ref RAIL_EVENT_RX_FIFO_OVERFLOW bit */
  RAIL_EVENT_RX_FIFO_OVERFLOW_SHIFT,
  /** Shift position of \ref RAIL_EVENT_RX_ADDRESS_FILTERED bit */
  RAIL_EVENT_RX_ADDRESS_FILTERED_SHIFT,
  /** Shift position of \ref RAIL_EVENT_RX_TIMEOUT bit */
  RAIL_EVENT_RX_TIMEOUT_SHIFT,
  /** Shift position of \ref  RAIL_EVENT_RX_SCHEDULED_RX_END bit */
  RAIL_EVENT_RX_SCHEDULED_RX_END_SHIFT,
  /** Shift position of \ref RAIL_EVENT_RX_PACKET_ABORTED bit */
  RAIL_EVENT_RX_PACKET_ABORTED_SHIFT,
  /** Shift position of \ref RAIL_EVENT_RX_FILTER_PASSED bit */
  RAIL_EVENT_RX_FILTER_PASSED_SHIFT,
  /** Shift position of \ref RAIL_EVENT_RX_TIMING_LOST bit */
  RAIL_EVENT_RX_TIMING_LOST_SHIFT,
  /** Shift position of \ref RAIL_EVENT_RX_TIMING_DETECT bit */
  RAIL_EVENT_RX_TIMING_DETECT_SHIFT,
  /** Shift position of \ref RAIL_EVENT_IEEE802154_DATA_REQUEST_COMMAND bit */
  RAIL_EVENT_IEEE802154_DATA_REQUEST_COMMAND_SHIFT,

  // TX Event Bit Shifts

  /** Shift position of \ref RAIL_EVENT_TX_FIFO_ALMOST_EMPTY bit */
  RAIL_EVENT_TX_FIFO_ALMOST_EMPTY_SHIFT,
  /** Shift position of \ref RAIL_EVENT_TX_PACKET_SENT bit */
  RAIL_EVENT_TX_PACKET_SENT_SHIFT,
  /** Shift position of \ref RAIL_EVENT_TXACK_PACKET_SENT bit */
  RAIL_EVENT_TXACK_PACKET_SENT_SHIFT,
  /** Shift position of \ref RAIL_EVENT_TX_ABORTED bit */
  RAIL_EVENT_TX_ABORTED_SHIFT,
  /** Shift position of \ref RAIL_EVENT_TXACK_ABORTED bit */
  RAIL_EVENT_TXACK_ABORTED_SHIFT,
  /** Shift position of \ref RAIL_EVENT_TX_BLOCKED bit */
  RAIL_EVENT_TX_BLOCKED_SHIFT,
  /** Shift position of \ref RAIL_EVENT_TXACK_BLOCKED bit */
  RAIL_EVENT_TXACK_BLOCKED_SHIFT,
  /** Shift position of \ref RAIL_EVENT_TX_UNDERFLOW bit */
  RAIL_EVENT_TX_UNDERFLOW_SHIFT,
  /** Shift position of \ref RAIL_EVENT_TXACK_UNDERFLOW bit */
  RAIL_EVENT_TXACK_UNDERFLOW_SHIFT,
  /** Shift position of \ref RAIL_EVENT_TX_CHANNEL_CLEAR bit */
  RAIL_EVENT_TX_CHANNEL_CLEAR_SHIFT,
  /** Shift position of \ref RAIL_EVENT_TX_CHANNEL_BUSY bit */
  RAIL_EVENT_TX_CHANNEL_BUSY_SHIFT,
  /** Shift position of \ref RAIL_EVENT_TX_CCA_RETRY bit */
  RAIL_EVENT_TX_CCA_RETRY_SHIFT,
  /** Shift position of \ref RAIL_EVENT_TX_START_CCA bit */
  RAIL_EVENT_TX_START_CCA_SHIFT,

  // Scheduler Event Bit Shifts

  /** Shift position of \ref RAIL_EVENT_CONFIG_UNSCHEDULED bit */
  RAIL_EVENT_CONFIG_UNSCHEDULED_SHIFT,
  /** Shift position of \ref RAIL_EVENT_CONFIG_SCHEDULED bit */
  RAIL_EVENT_CONFIG_SCHEDULED_SHIFT,
  /** Shift position of \ref RAIL_EVENT_SCHEDULER_STATUS bit */
  RAIL_EVENT_SCHEDULER_STATUS_SHIFT,

  // Other Event Bit Shifts

  /** Shift position of \ref RAIL_EVENT_CAL_NEEDED bit */
  RAIL_EVENT_CAL_NEEDED_SHIFT,
};

// RAIL_Event_t bitmasks

/** Value representing no events */
#define RAIL_EVENTS_NONE 0ULL

/**
 * Occurs when the hardware-averaged RSSI is done. It only occurs in response to
 * RAIL_StartAverageRssi() to indicate that the hardware has completed
 * averaging. Call RAIL_GetAverageRssi() to get the result.
 */
#define RAIL_EVENT_RSSI_AVERAGE_DONE (1ULL << RAIL_EVENT_RSSI_AVERAGE_DONE_SHIFT)

/**
 * Notifies the application when searching for an ack packet has timed out.
 * This event occurs whenever the timeout for searching for an ack packet is
 * exceeded. This will only occur after calling RAIL_ConfigAutoAck() and
 * transmitting a packet.
 */
#define RAIL_EVENT_RX_ACK_TIMEOUT (1ULL << RAIL_EVENT_RX_ACK_TIMEOUT_SHIFT)

/**
 * Occurs when the receive FIFO exceeds the configured threshold value. Call
 * RAIL_GetRxFifoBytesAvailable() to get the number of bytes available. When
 * using this event, the threshold should be set via RAIL_SetRxFifoThreshold().
 */
#define RAIL_EVENT_RX_FIFO_ALMOST_FULL (1ULL << RAIL_EVENT_RX_FIFO_ALMOST_FULL_SHIFT)

/**
 * Occurs whenever a packet is received. Call RAIL_GetRxPacketInfo() to get
 * basic packet information along with a handle to this packet for subsequent
 * use with RAIL_PeekRxPacket(), RAIL_GetRxPacketDetails(), RAIL_HoldRxPacket(),
 * and RAIL_ReleaseRxPacket() as needed.
 *
 * If \ref RAIL_RX_OPTION_IGNORE_CRC_ERRORS is set, this event also occurs
 * for packets with CRC errors.
 */
#define RAIL_EVENT_RX_PACKET_RECEIVED (1ULL << RAIL_EVENT_RX_PACKET_RECEIVED_SHIFT)

/**
 * Occurs when the radio has lost a preamble. This event can occur multiple
 * times while searching for a packet, and is generally used for diagnostic
 * purposes.  This event can only occur after a
 * \ref RAIL_EVENT_RX_PREAMBLE_DETECT event has already occurred.
 */
#define RAIL_EVENT_RX_PREAMBLE_LOST (1ULL << RAIL_EVENT_RX_PREAMBLE_LOST_SHIFT)

/**
 * Occurs when the radio has detected a preamble. This event can occur multiple
 * times while searching for a packet, and is generally used for diagnostic
 * purposes. This event can only occur after a \ref RAIL_EVENT_RX_TIMING_DETECT
 * event has already occurred.
 */
#define RAIL_EVENT_RX_PREAMBLE_DETECT (1ULL << RAIL_EVENT_RX_PREAMBLE_DETECT_SHIFT)

/**
 * Occurs when the first sync word is detected. After this event occurs, one of
 * the events in the \ref RAIL_EVENTS_RX_COMPLETION mask will occur.
 */
#define RAIL_EVENT_RX_SYNC1_DETECT (1ULL << RAIL_EVENT_RX_SYNC1_DETECT_SHIFT)

/**
 * Occurs when the second sync word is detected. After this event occurs, one of
 * the events in the \ref RAIL_EVENTS_RX_COMPLETION mask will occur.
 */
#define RAIL_EVENT_RX_SYNC2_DETECT (1ULL << RAIL_EVENT_RX_SYNC2_DETECT_SHIFT)

/**
 * Occurs when a packet being received has a frame error.
 *
 * For efr32xg1x parts, frame errors include violations of variable length
 * minimum/maximum limits and CRC errors. If
 * \ref RAIL_RX_OPTION_IGNORE_CRC_ERRORS is set, \ref RAIL_EVENT_RX_FRAME_ERROR
 * will not occur for CRC errors.
 */
#define RAIL_EVENT_RX_FRAME_ERROR (1ULL << RAIL_EVENT_RX_FRAME_ERROR_SHIFT)

/**
 * Occurs when RX buffer is full. This will cause the radio to stop receiving
 * packets until the packet causing the overflow is processed.
 * */
#define RAIL_EVENT_RX_FIFO_OVERFLOW (1ULL << RAIL_EVENT_RX_FIFO_OVERFLOW_SHIFT)

/**
 * Occurs when a packet's address does not match the filtering settings. After
 * this event, the packet will not be received, and no other events should occur
 * for the packet. This event can only occur after calling
 * RAIL_EnableAddressFilter().
 */
#define RAIL_EVENT_RX_ADDRESS_FILTERED (1ULL << RAIL_EVENT_RX_ADDRESS_FILTERED_SHIFT)

/**
 * Occurs when an RX event times out. This event can only occur if the
 * RAIL_StateTiming_t::rxSearchTimeout passed to RAIL_SetStateTiming() is
 * nonzero.
 */
#define RAIL_EVENT_RX_TIMEOUT (1ULL << RAIL_EVENT_RX_TIMEOUT_SHIFT)

/**
 * Occurs when the scheduled RX window ends. This event only occurs in response
 * to a scheduled receive timeout after calling RAIL_ScheduleRx(). If
 * RAIL_ScheduleRxConfig_t::rxTransitionEndSchedule was passed as false, then
 * this event will occur unless the receive is aborted (due to a call to
 * RAIL_Idle() or a scheduler pre-emption, for instance). If
 * RAIL_ScheduleRxConfig_t::rxTransitionEndSchedule was passed as true, then
 * any of the \ref RAIL_EVENTS_RX_COMPLETION events occurring will also cause
 * this event to not occur, since the scheduled reeive will end with the
 * transition at the end of the packet.
 */
#define RAIL_EVENT_RX_SCHEDULED_RX_END (1ULL << RAIL_EVENT_RX_SCHEDULED_RX_END_SHIFT)

/**
 * Occurs when a packet is aborted, but a more specific reason (such as
 * \ref RAIL_EVENT_RX_ADDRESS_FILTERED) isn't known.
 */
#define RAIL_EVENT_RX_PACKET_ABORTED (1ULL << RAIL_EVENT_RX_PACKET_ABORTED_SHIFT)

/**
 * Occurs when the packet has passed any configured address and frame
 * filtering options. This event will only occur between the start of the
 * packet, indicated by \ref RAIL_EVENT_RX_SYNC1_DETECT or
 * \ref RAIL_EVENT_RX_SYNC2_DETECT, and one of the events in the
 * \ref RAIL_EVENTS_RX_COMPLETION mask. This event will always occur before or
 * concurrently with \ref RAIL_EVENT_RX_PACKET_RECEIVED. If IEEE 802.15.4 frame
 * and address filtering are enabled, this event will occur immediately after
 * destination address filtering.
 */
#define RAIL_EVENT_RX_FILTER_PASSED (1ULL << RAIL_EVENT_RX_FILTER_PASSED_SHIFT)

/**
 * Occurs when the modem timing is lost. This event can occur multiple times
 * while searching for a packet, and is generally used for diagnostic purposes.
 * This event can only occur after a \ref RAIL_EVENT_RX_TIMING_DETECT event has
 * already occurred.
 */
#define RAIL_EVENT_RX_TIMING_LOST (1ULL << RAIL_EVENT_RX_TIMING_LOST_SHIFT)

/**
 * Occurs when the modem timing is detected. This event can occur multiple times
 * while searching for a packet, and is generally used for diagnostic purposes.
 */
#define RAIL_EVENT_RX_TIMING_DETECT (1ULL << RAIL_EVENT_RX_TIMING_DETECT_SHIFT)

/**
 * Indicates a Data Request is being received when using IEEE 802.15.4
 * functionality. This occurs when the command byte of an incoming frame is
 * for a data request, which requests an ACK. This callback is called before
 * the packet is fully received to allow the node to have more time to decide
 * whether to set the frame pending in the outgoing ACK. This event only ever
 * occurs if the RAIL IEEE 802.15.4 functionality is enabled, but will never
 * occur if promiscuous mode is enabled via
 * RAIL_IEEE802154_SetPromiscuousMode().
 *
 * Call RAIL_IEEE802154_GetAddress() to get the source address of the packet.
 */
#define RAIL_EVENT_IEEE802154_DATA_REQUEST_COMMAND (1ULL << RAIL_EVENT_IEEE802154_DATA_REQUEST_COMMAND_SHIFT)

/**
 * This mask indicates all of the events that determine the end of a received
 * packet. After a packet has begun to be received, which is indicated by a
 * \ref RAIL_EVENT_RX_SYNC1_DETECT or a \ref RAIL_EVENT_RX_SYNC2_DETECT,
 * exactly one of the following events will occur. When one of these events
 * occurs a state transition will take place, based on the parameter passed to
 * RAIL_SetRxTransitions(). The RAIL_StateTransitions_t::success transition
 * will be followed only if the \ref RAIL_EVENT_RX_PACKET_RECEIVED event occurs.
 * Any of the other events will trigger the RAIL_StateTransitions_t::error
 * transition.
 */
#define RAIL_EVENTS_RX_COMPLETION (RAIL_EVENT_RX_PACKET_RECEIVED  \
                                   | RAIL_EVENT_RX_PACKET_ABORTED \
                                   | RAIL_EVENT_RX_FRAME_ERROR    \
                                   | RAIL_EVENT_RX_FIFO_OVERFLOW  \
                                   | RAIL_EVENT_RX_ADDRESS_FILTERED)

// TX Event Bitmasks

/**
 * Occurs when the number of bytes in the transmit FIFO was above the
 * configured threshold, and falls to a value at or below the threshold value.
 * This event does not occur on initailization or after resetting the transmit
 * FIFO with RAIL_ResetFifo(). Call RAIL_GetTxFifoSpaceAvailable() to get the
 * number of bytes available in the transmit FIFO at the time of the callback
 * dispatch. When using this event, the threshold should be set via
 * RAIL_SetTxFifoThreshold().
 */
#define RAIL_EVENT_TX_FIFO_ALMOST_EMPTY (1ULL << RAIL_EVENT_TX_FIFO_ALMOST_EMPTY_SHIFT)

/**
 * Occurs when a packet was sent. Call RAIL_GetTxPacketDetails() to get
 * information about the packet that was transmitted.
 * @note RAIL_GetTxPacketDetails() is only valid to call during the timeframe
 *   of the RAIL_Config_t::eventsCallback.
 */
#define RAIL_EVENT_TX_PACKET_SENT (1ULL << RAIL_EVENT_TX_PACKET_SENT_SHIFT)

/**
 * Occurs when an ack packet was sent. Call RAIL_GetTxPacketDetails() to get
 * information about the packet that was transmitted. This event can only occur
 * after calling RAIL_ConfigAutoAck().
 * @note RAIL_GetTxPacketDetails() is only valid to call during the timeframe
 *   of the RAIL_Config_t::eventsCallback.
 */
#define RAIL_EVENT_TXACK_PACKET_SENT (1ULL << RAIL_EVENT_TXACK_PACKET_SENT_SHIFT)

/**
 * Occurs when a transmit is aborted by the user. This can occur due to calling
 * RAIL_Idle() or due to a scheduler pre-emption.
 */
#define RAIL_EVENT_TX_ABORTED (1ULL << RAIL_EVENT_TX_ABORTED_SHIFT)

/**
 * Occurs when an ack transmit is aborted by the user. This event can only
 * occur after calling RAIL_ConfigAutoAck(). This can occur due to calling
 * RAIL_Idle() or due to a scheduler pre-emption.
 */
#define RAIL_EVENT_TXACK_ABORTED (1ULL << RAIL_EVENT_TXACK_ABORTED_SHIFT)

/**
 * Occurs when a transmit is blocked from occurring due to having called
 * RAIL_EnableTxHoldOff().
 */
#define RAIL_EVENT_TX_BLOCKED (1ULL << RAIL_EVENT_TX_BLOCKED_SHIFT)

/**
 * Occurs when an ack transmit is blocked from occurring due to having called
 * RAIL_EnableTxHoldOff(). This event can only occur after calling
 * RAIL_ConfigAutoAck().
 */
#define RAIL_EVENT_TXACK_BLOCKED (1ULL << RAIL_EVENT_TXACK_BLOCKED_SHIFT)

/**
 * Occurs when the transmit buffer underflows. This can happen due to the
 * transmitted packet specifying an unintended length based on the current
 * radio configuration, or due to RAIL_WriteTxFifo() calls not keeping up with
 * the transmit rate, if the entire packet isn't loaded at once.
 */
#define RAIL_EVENT_TX_UNDERFLOW (1ULL << RAIL_EVENT_TX_UNDERFLOW_SHIFT)

/**
 * Occurs when the ack transmit buffer underflows. This can happen due to the
 * transmitted packet specifying an unintended length based on the current
 * radio configuration, or due to RAIL_WriteAutoAckFifo() not being called at
 * all before an ack transmit. This event can only occur after calling
 * RAIL_ConfigAutoAck().
 */
#define RAIL_EVENT_TXACK_UNDERFLOW (1ULL << RAIL_EVENT_TXACK_UNDERFLOW_SHIFT)

/**
 * Occurs when Carrier Sense Mulitple Access (CSMA) or Listen Before Talk (LBT)
 * succeeds. This event can only happen after calling RAIL_StartCcaCsmaTx() or
 * RAIL_StartCsmaLbtTx().
 */
#define RAIL_EVENT_TX_CHANNEL_CLEAR (1ULL << RAIL_EVENT_TX_CHANNEL_CLEAR_SHIFT)

/**
 * Occurs when Carrier Sense Mulitple Access (CSMA) or Listen Before Talk (LBT)
 * fails. This event can only happen after calling RAIL_StartCcaCsmaTx() or
 * RAIL_StartCsmaLbtTx().
 */
#define RAIL_EVENT_TX_CHANNEL_BUSY (1ULL << RAIL_EVENT_TX_CHANNEL_BUSY_SHIFT)

/**
 * Occurs during CSMA or LBT when an individual Clear Channel Assessment (CCA)
 * check failed, but there are more tries needed before the overall operation
 * completes. This event can occur multiple times based on the configuration
 * of the ongoing CSMA or LBT tranmission. This event can only happen after
 * calling RAIL_StartCcaCsmaTx() or RAIL_StartCsmaLbtTx().
 *
 */
#define RAIL_EVENT_TX_CCA_RETRY (1ULL << RAIL_EVENT_TX_CCA_RETRY_SHIFT)

/**
 * Occurs when a Clear Channel Assessment (CCA) check begins. This event can
 * occur multiple times based on the configuration of the ongoing CSMA or LBT
 * transmission. This event can only happen after calling RAIL_StartCcaCsmaTx()
 * or RAIL_StartCsmaLbtTx().
 */
#define RAIL_EVENT_TX_START_CCA (1ULL << RAIL_EVENT_TX_START_CCA_SHIFT)

/**
 * This mask indicates all of the events that determine the end of a transmitted
 * packet. After a packet has begun to be transmitted, which is indicated by
 * a \ref RAIL_STATUS_NO_ERROR return value from one of the transmit functions,
 * exactly one of the following events will occur. When one of these events
 * occurs a state transition will take place based on the parameter
 * passed to RAIL_SetTxTransitions(). The RAIL_StateTransitions_t::success
 * transition will be followed only if the \ref RAIL_EVENT_TX_PACKET_SENT
 * event occurs. Any of the other events will trigger the
 * RAIL_StateTransitions_t::error transition.
 */
#define RAIL_EVENTS_TX_COMPLETION (RAIL_EVENT_TX_PACKET_SENT \
                                   | RAIL_EVENT_TX_ABORTED   \
                                   | RAIL_EVENT_TX_BLOCKED   \
                                   | RAIL_EVENT_TX_UNDERFLOW \
                                   | RAIL_EVENT_TX_CHANNEL_BUSY)

/**
 * This mask indicates all of the events that determine the end of a transmitted
 * ack packet. After an ack packet has begun to be transmitted, which occurs
 * after an ack-requesting receive, exactly one of the following events will
 * occur. When one of these events occurs a state transition will take place
 * based on the RAIL_AutoAckConfig_t::rxTransitions passed to
 * RAIL_ConfigAutoAck(). The receive transitions are used because the
 * transmitted ack packet is considered a part of the ack-requesting received
 * packet. The RAIL_StateTransitions_t::success transition will be followed
 * only if the \ref RAIL_EVENT_TXACK_PACKET_SENT event occurs. Any of the other
 * events will trigger the RAIL_StateTransitions_t::error transition.
 */
#define RAIL_EVENTS_TXACK_COMPLETION (RAIL_EVENT_TXACK_PACKET_SENT \
                                      | RAIL_EVENT_TXACK_ABORTED   \
                                      | RAIL_EVENT_TXACK_BLOCKED   \
                                      | RAIL_EVENT_TXACK_UNDERFLOW)

// Scheduler Event Bitmasks

/**
 * Occurs when the scheduler switches away from this configuration. This event
 * can occur often in dynamic multiprotocol scenarios, and can give the
 * current stack an opportunity to release hardware resources.
 */
#define RAIL_EVENT_CONFIG_UNSCHEDULED (1ULL << RAIL_EVENT_CONFIG_UNSCHEDULED_SHIFT)

/**
 * Occurs when the scheduler switches to this configuration. This event can
 * occur often in dynamic multiprotocol scenarios, and can be used to influence
 * the stack's operation.
 */
#define RAIL_EVENT_CONFIG_SCHEDULED (1ULL << RAIL_EVENT_CONFIG_SCHEDULED_SHIFT)

/**
 * Occurs when the scheduler has a status to report. The exact status can be
 * found with RAIL_GetSchedulerStatus(). See \ref RAIL_SchedulerStatus_t for
 * more details.
 * @note RAIL_GetSchedulerStatus() is only valid to call during the timeframe
 *   of the RAIL_Config_t::eventsCallback.
 */
#define RAIL_EVENT_SCHEDULER_STATUS (1ULL << RAIL_EVENT_SCHEDULER_STATUS_SHIFT)

// Other Event Bitmasks

/**
 * Occurs when the application needs to run a calibration.
 * The RAIL library detects when a calibration is needed. The application
 * determines a valid window to call RAIL_Calibrate().
 */
#define RAIL_EVENT_CAL_NEEDED (1ULL << RAIL_EVENT_CAL_NEEDED_SHIFT)

/** Value representing all possible events */
#define RAIL_EVENTS_ALL 0xFFFFFFFFFFFFFFFFULL

/** @} */ // end of group Events

/**
 * @addtogroup PA Power Amplifier (PA)
 * @ingroup Transmit
 * @{
 */

/**
 * The transmit power in deci-dBm units (e.g. 4.5dBm -> 45 deci-dBm). These
 * values are used by the conversion functions to convert a \ref
 * RAIL_TxPowerLevel_t to deci-dBm for application consumption. On the EFR32
 * they can range from \ref RAIL_TX_POWER_MIN to \ref RAIL_TX_POWER_MAX.
 */
typedef int16_t RAIL_TxPower_t;
/** The maximum valid value for a \ref RAIL_TxPower_t. */
#define RAIL_TX_POWER_MAX ((RAIL_TxPower_t)0x7FFF)
/** The minimum valid value for a \ref RAIL_TxPower_t. */
#define RAIL_TX_POWER_MIN ((RAIL_TxPower_t)0x8000)

/// mV are used for all TX power voltage values
/// TX power voltages take and return voltages multiplied by this factor
#define RAIL_TX_POWER_VOLTAGE_SCALING_FACTOR 1000

/// deci-dBm are used for all TX power dBm values
/// All dBm inputs to TX power functions take dBm power times this factor
#define RAIL_TX_POWER_DBM_SCALING_FACTOR 10

/** @} */ // PA Power Amplifier (PA)

/******************************************************************************
 * General Structures
 *****************************************************************************/
/**
 * @addtogroup General
 * @{
 */

/**
 * @struct RAIL_Version_t
 * @brief Contains RAIL Library Version Information.
 *   It is filled in by RAIL_GetVersion().
 */
typedef struct RAIL_Version {
  uint32_t hash;      /**< Git hash */
  uint8_t  major;     /**< Major number    */
  uint8_t  minor;     /**< Minor number    */
  uint8_t  rev;       /**< Revision number */
  uint8_t  build;     /**< Build number */
  uint8_t  flags;     /**< Build flags */
  /** Boolean to indicate whether this is a multiprotocol library or not. */
  bool     multiprotocol;
} RAIL_Version_t;

/**
 * @typedef RAIL_Handle_t
 * @brief Handle of a RAIL instance, as returned from RAIL_Init().
 */
typedef void *RAIL_Handle_t;

/**
 * @enum RAIL_Status_t
 * @brief Status returned by many RAIL API calls indicating their success or
 *   failure.
 */
RAIL_ENUM(RAIL_Status_t) {
  RAIL_STATUS_NO_ERROR, /**< RAIL function reports no error */
  RAIL_STATUS_INVALID_PARAMETER, /**< Call to RAIL function threw an error
                                      because of an invalid parameter */
  RAIL_STATUS_INVALID_STATE, /**< Call to RAIL function threw an error
                                  because it was called during an invalid
                                  radio state */
  RAIL_STATUS_INVALID_CALL, /**< RAIL function is called in an invalid order */
  RAIL_STATUS_SUSPENDED, /**< RAIL function did not finish in the allotted
                              time */
};

/**
 * @def RAIL_STATE_UINT32_BUFFER_SIZE
 * @brief The size, in 32-bit words, of RAIL_StateBuffer_t to store RAIL
 *   internal state.
 */
#define RAIL_STATE_UINT32_BUFFER_SIZE 70

/**
 * @typedef RAIL_StateBuffer_t
 * @brief A buffer to store RAIL internal state.
 */
typedef uint32_t RAIL_StateBuffer_t[RAIL_STATE_UINT32_BUFFER_SIZE];

/**
 * @struct RAIL_Config_t
 * @brief RAIL configuration and internal state structure.
 *
 * This structure must be allocated in application global read-write memory
 * that persists for the duration of RAIL usage. It cannot be allocated
 * in read-only memory or on the call stack.
 */
typedef struct RAIL_Config {
  /**
   * Pointer to function called whenever a RAIL event occurs.
   *
   * @param[in] railHandle A handle for RAIL instance.
   * @param[in] events A bit mask of RAIL events.
   * @return void.
   *
   * See the \ref RAIL_Events_t documentation for the list of RAIL events.
   */
  void (*eventsCallback)(RAIL_Handle_t railHandle, RAIL_Events_t events);
  /**
   * Pointer to a protocol-specific state structure allocated in global
   * read-write memory and initialized to all zeros.
   * For the BLE protocol this should point to a RAIL_BLE_State_t
   * structure. For IEEE802154 this should be NULL.
   */
  void *protocol;
  /**
   * Pointer to a RAIL scheduler state object allocated in global read-write
   * memory and initialized to all zeros. When not using a multiprotocol
   * scheduler, this should be NULL.
   */
  RAILSched_Config_t *scheduler;
  /**
   * A structure for RAIL to maintain its internal state, which must be
   * initialized to all zeros.
   */
  RAIL_StateBuffer_t buffer;
} RAIL_Config_t;

/**
 * A pointer to init complete callback function
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @return void.
 *
 */
typedef void (*RAIL_InitCompleteCallbackPtr_t)(RAIL_Handle_t railHandle);

/** @} */ // end of group General

/******************************************************************************
 * Radio Configuration Structures
 *****************************************************************************/
/**
 * @addtogroup Radio_Configuration
 * @{
 */

/**
 * @struct RAIL_FrameType_t
 * @brief Configure if there is a frame type in your frame and the lengths of
 * each frame. The number of bits set in the mask determines the number of
 * elements in frameLen. A maximum of 8 different frame types may be specified.
 */
typedef struct RAIL_FrameType {
  /**
   * Pointer to array of frame lengths for each frame type. The length of this
   * array should be equal to the number of frame types. The array that
   * frameLen points to should not change location or be modified.
   */
  uint16_t *frameLen;
  /**
   * Zero-indexed location of the byte containing the frame type field.
   */
  uint8_t offset;
  /**
   * Bit mask of the frame type field. Determines number of frames expected
   * based on the number of bits set. No more than 3 bits can be set in mask.
   * Must be contiguous ones. For example, if the highest three bits of the byte
   * specified by offset constitute the frame type, then mask should be 0xE0,
   * which has 3 bits set, indicating 8 possible frame types.
   */
  uint8_t mask;
  /**
   * Bitmask that marks if each frame is valid or should be filtered. Frame type
   * 0 corresponds to the lowest bit in isValid. If the frame is filtered, a
   * RAIL_EVENT_RX_PACKET_ABORTED will be raised.
   */
  uint8_t isValid;
  /**
   * Bitmask that marks if each frame should have the address filter applied.
   * Frame type 0 corresponds to the least significant bit in addressFilter.
   */
  uint8_t addressFilter;
} RAIL_FrameType_t;

/**
 * @def RAIL_SETFIXEDLENGTH_INVALID
 * @brief Invalid return value when calling RAIL_SetFixedLength()
 *
 * Invalid return value when calling RAIL_SetFixedLength() while the radio is
 * not in fixed-length mode.
 */
#define RAIL_SETFIXEDLENGTH_INVALID (0xFFFF)

/**
 * @struct RAIL_ChannelConfigEntryAttr_t
 * @brief Channel configuration entry attribute structure. Items listed
 *  here are designed to be altered and updated during run-time.
 */
typedef struct RAIL_ChannelConfigEntryAttr {
  uint32_t calValues[1]; /**< Attributes specific to each channel config
                              entry. */
} RAIL_ChannelConfigEntryAttr_t;

/**
 * @struct RAIL_ChannelConfigEntry_t
 * @brief Channel configuration entry structure, which defines a channel range
 *   and parameters across which a corresponding radio configuration is valid.
 *
 * operating frequency = baseFrequency
 *   + channelSpacing * (channel - physicalChannelOffset);
 */
typedef struct RAIL_ChannelConfigEntry {
  const uint32_t *phyConfigDeltaAdd; /**< Minimum radio config to apply to base
                                          config for this channel set. */
  uint32_t baseFrequency; /**< Base frequency in Hz of this channel set. */
  uint32_t channelSpacing; /**< Channel spacing in Hz of this channel set. */
  uint16_t physicalChannelOffset; /**< The offset to subtract from the logical
                                       channel to align them with the zero
                                       based physical channels which are
                                       relative to baseFrequency.
                                       (i.e. By default ch 0 = base freq, but
                                       if offset = 11, ch 11 = base freq.) */
  uint16_t channelNumberStart; /**< The first valid RAIL channel number for this
                                    channel set. */
  uint16_t channelNumberEnd; /**< The last valid RAIL channel number for this
                                  channel set. */
  RAIL_TxPower_t maxPower; /**< The max power allowed in this channel set. */
  RAIL_ChannelConfigEntryAttr_t *attr; /**< A pointer to a structure containing
                                            attributes specific to this
                                            channel set. */
} RAIL_ChannelConfigEntry_t;

/// @struct RAIL_ChannelConfig_t
/// @brief Channel configuration structure, which defines the channel meaning
///   when a channel number is passed into a RAIL function, e.g., RAIL_StartTx()
///   and RAIL_StartRx().
///
/// A RAIL_ChannelConfig_t structure defines the channel scheme that an
/// application uses when registered in RAIL_ConfigChannels().
///
/// A few examples of different channel configurations:
/// @code{.c}
/// // 21 channels starting at 2.45GHz with channel spacing of 1MHz
/// // ... generated by Simplicity Studio (i.e. rail_config.c) ...
/// const uint32_t generated[] = { ... };
/// RAIL_ChannelConfigEntryAttr_t generated_entryAttr = { ... };
/// const RAIL_ChannelConfigEntry_t generated_channels[] = {
///   {
///     .phyConfigDeltaAdd = NULL, // Add this to default config for this entry
///     .baseFrequency = 2450000000,
///     .channelSpacing = 1000000,
///     .physicalChannelOffset = 0,
///     .channelNumberStart = 0,
///     .channelNumberEnd = 20,
///     .maxPower = RAIL_TX_POWER_MAX,
///     .attr = &generated_entryAttr
///   },
/// };
/// const RAIL_ChannelConfig_t generated_channelConfig = {
///   .phyConfigBase = generated, // Default radio config for all entries
///   .phyConfigDeltaSubtract = NULL, // Subtract this to restore default config
///   .configs = generated_channels,
///   .length = 1 // There are this many channel config entries
/// };
/// const RAIL_ChannelConfig_t *channelConfigs[] = {
///   &generated_channelConfig,
///   NULL
/// };
/// // ... in main code ...
/// // Associate a specific channel config with a particular rail instance.
/// RAIL_ConfigChannels(railHandle, channelConfigs[0]);
///
/// // 4 nonlinear channels
/// // ... in rail_config.c ...
/// const uint32_t generated[] = { ... };
/// RAIL_ChannelConfigEntryAttr_t generated_entryAttr = { ... };
/// const RAIL_ChannelConfigEntry_t generated_channels[] = {
///   {
///     .phyConfigDeltaAdd = NULL, // Add this to default config for this entry
///     .baseFrequency = 910123456,
///     .channelSpacing = 0,
///     .physicalChannelOffset = 0,
///     .channelNumberStart = 0,
///     .channelNumberEnd = 0,
///     .maxPower = RAIL_TX_POWER_MAX,
///     .attr = &generated_entryAttr
///   },
///   {
///     .phyConfigDeltaAdd = NULL,
///     .baseFrequency = 911654789,
///     .channelSpacing = 0,
///     .physicalChannelOffset = 0, // Since ch spacing = 0, offset can be 0
///     .channelNumberStart = 1,
///     .channelNumberEnd = 1,
///     .maxPower = RAIL_TX_POWER_MAX,
///     .attr = &generated_entryAttr
///   },
///   {
///     .phyConfigDeltaAdd = NULL,
///     .baseFrequency = 912321456,
///     .channelSpacing = 100000,
///     .physicalChannelOffset = 2, // Since ch spacing != 0, offset = 2
///     .channelNumberStart = 2, // We want ch 2 = baseFrequency
///     .channelNumberEnd = 2,
///     .maxPower = RAIL_TX_POWER_MAX,
///     .attr = &generated_entryAttr
///   },
///   {
///     .phyConfigDeltaAdd = NULL,
///     .baseFrequency = 913147852,
///     .channelSpacing = 0,
///     .physicalChannelOffset = 0,
///     .channelNumberStart = 3,
///     .channelNumberEnd = 3,
///     .maxPower = RAIL_TX_POWER_MAX,
///     .attr = &generated_entryAttr
///   },
/// };
/// const RAIL_ChannelConfig_t generated_channelConfig = {
///   .phyConfigBase = generated, // Default radio config for all entries
///   .phyConfigDeltaSubtract = NULL, // Subtract this to restore default config
///   .configs = generated_channels,
///   .length = 4 // There are this many channel config entries
/// };
/// const RAIL_ChannelConfig_t *channelConfigs[] = {
///   &generated_channelConfig,
///   NULL
/// };
/// // ... in main code ...
/// // Associate a specific channel config with a particular rail instance.
/// RAIL_ConfigChannels(railHandle, channelConfigs[0]);
///
/// // Multiple radio configurations
/// // ... in rail_config.c ...
/// const uint32_t generated0[] = { ... };
/// RAIL_ChannelConfigEntryAttr_t generated0_entryAttr = { ... };
/// const RAIL_ChannelConfigEntry_t generated0_channels[] = {
///   {
///     .phyConfigDeltaAdd = NULL, // Add this to default config for this entry
///     .baseFrequency = 2450000000,
///     .channelSpacing = 1000000,
///     .physicalChannelOffset = 0,
///     .channelNumberStart = 0,
///     .channelNumberEnd = 20,
///     .maxPower = RAIL_TX_POWER_MAX,
///     .attr = &generated0_entryAttr
///   },
/// };
/// const RAIL_ChannelConfig_t generated0_channelConfig = {
///   .phyConfigBase = generated0, // Default radio config for all entries
///   .phyConfigDeltaSubtract = NULL, // Subtract this to restore default config
///   .configs = generated0_channels,
///   .length = 1 // There are this many channel config entries
/// };
/// const uint32_t generated1[] = { ... };
/// RAIL_ChannelConfigEntryAttr_t generated1_entryAttr = { ... };
/// const RAIL_ChannelConfigEntry_t generated1_channels[] = {
///   {
///     .phyConfigDeltaAdd = NULL,
///     .baseFrequency = 2450000000,
///     .channelSpacing = 1000000,
///     .physicalChannelOffset = 0,
///     .channelNumberStart = 0,
///     .channelNumberEnd = 20,
///     .maxPower = -100, // Use this entry when TX power <= -10dBm
///     .attr = &generated1_entryAttr
///   },
///   {
///     .phyConfigDeltaAdd = NULL,
///     .baseFrequency = 2450000000,
///     .channelSpacing = 1000000,
///     .physicalChannelOffset = 0,
///     .channelNumberStart = 0,
///     .channelNumberEnd = 20,
///     .maxPower = 15, // Use this entry when TX power > -10dBm
///                     // and TX power <= 1.5dBm
///     .attr = &generated1_entryAttr
///   },
///   {
///     .phyConfigDeltaAdd = NULL,
///     .baseFrequency = 2450000000,
///     .channelSpacing = 1000000,
///     .physicalChannelOffset = 0,
///     .channelNumberStart = 0,
///     .channelNumberEnd = 20,
///     .maxPower = RAIL_TX_POWER_MAX, // Use this entry when TX power > 1.5dBm
///     .attr = &generated1_entryAttr
///   },
/// };
/// const RAIL_ChannelConfig_t generated1_channelConfig = {
///   .phyConfigBase = generated1,
///   .phyConfigDeltaSubtract = NULL,
///   .configs = generated1_channels,
///   .length = 3
/// };
/// const uint32_t generated2[] = { ... };
/// RAIL_ChannelConfigEntryAttr_t generated2_entryAttr = { ... };
/// const RAIL_ChannelConfigEntry_t generated2_channels[] = {
///   {
///     .phyConfigDeltaAdd = NULL,
///     .baseFrequency = 2450000000,
///     .channelSpacing = 1000000,
///     .physicalChannelOffset = 0,
///     .channelNumberStart = 0,
///     .channelNumberEnd = 20,
///     .maxPower = RAIL_TX_POWER_MAX,
///     .attr = &generated2_entryAttr
///   },
/// };
/// const RAIL_ChannelConfig_t generated2_channelConfig = {
///   .phyConfigBase = generated2,
///   .phyConfigDeltaSubtract = NULL,
///   .configs = generated2_channels,
///   .length = 1
/// };
/// const RAIL_ChannelConfig_t *channelConfigs[] = {
///   &generated0_channelConfig,
///   &generated1_channelConfig,
///   &generated2_channelConfig,
///   NULL
/// };
/// // ... in main code ...
/// // Create a unique RAIL handle for each unique channel config.
/// railHandle0 = RAIL_Init(&railCfg0, &RAILCb_RfReady0);
/// railHandle1 = RAIL_Init(&railCfg1, &RAILCb_RfReady1);
/// railHandle2 = RAIL_Init(&railCfg2, &RAILCb_RfReady2);
/// // Associate each channel config with its corresponding RAIL handle.
/// RAIL_ConfigChannels(railHandle0, channelConfigs[0]);
/// RAIL_ConfigChannels(railHandle1, channelConfigs[1]);
/// RAIL_ConfigChannels(railHandle2, channelConfigs[2]);
/// // Use a RAIL handle and channel to access the desired channel config entry.
/// RAIL_SetTxPowerDbm(railHandle1, 100); // set 10.0 dBm TX power
/// RAIL_StartRx(railHandle1, 0, &schedInfo); // RX using generated1_channels[2]
/// RAIL_SetTxPowerDbm(railHandle1, 0); // set 0 dBm TX power
/// RAIL_StartRx(railHandle1, 0, &schedInfo); // RX using generated1_channels[1]
/// RAIL_StartRx(railHandle2, 0, &schedInfo); // RX using generated2_channels[0]
/// @endcode

typedef struct RAIL_ChannelConfig {
  const uint32_t *phyConfigBase; /**< Base radio config for the corresponding
                                      channel config entries. */
  const uint32_t *phyConfigDeltaSubtract; /**< Minimum radio config to restore
                                               channel entries back to base
                                               config. */
  const RAIL_ChannelConfigEntry_t *configs; /**< Pointer to an array of
                                                 RAIL_ChannelConfigEntry_t
                                                 entries. */
  uint32_t length; /**< Number of RAIL_ChannelConfigEntry_t entries. */
} RAIL_ChannelConfig_t;

/**
 * @typedef RAIL_RadioConfigChangedCallback_t
 * @brief Pointer to function called whenever a radio configuration change occurs.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @param[in] entry The radio configuration entry being changed to.
 */
typedef void (*RAIL_RadioConfigChangedCallback_t)(RAIL_Handle_t railHandle,
                                                  const RAIL_ChannelConfigEntry_t *entry);
/** @} */ // end of group Radio_Configuration

/******************************************************************************
 * Packet Trace Interface (PTI) Structures
 *****************************************************************************/
/**
 * @addtogroup PTI
 * @{
 */

/**
 * @enum RAIL_PtiProtocol_t
 * @brief The protocol that RAIL outputs via the Packet Trace Interface (PTI).
 */
RAIL_ENUM(RAIL_PtiProtocol_t) {
  RAIL_PTI_PROTOCOL_CUSTOM = 0, /**< PTI output for a custom protocol */
  RAIL_PTI_PROTOCOL_THREAD = 2, /**< PTI output for the Thread protocol */
  RAIL_PTI_PROTOCOL_BLE = 3, /**< PTI output for the Bluetooth Smart protocol */
  RAIL_PTI_PROTOCOL_CONNECT = 4, /**< PTI output for the Connect protocol */
  RAIL_PTI_PROTOCOL_ZIGBEE = 5, /**< PTI output for the zigbee protocol */
};

/** @} */ // end of group PTI

/******************************************************************************
 * System Timing Structures
 *****************************************************************************/
/**
 * @addtogroup System_Timing
 * @{
 */

/**
 * Pointer to callback called when the RAIL timer expires.
 *
 * @param[in] cbArg The argument passed to the callback.
 * @return void.
 */
typedef void (*RAIL_TimerCallback_t)(RAIL_Handle_t cbArg);

/**
 * @enum RAIL_TimeMode_t
 * @brief Specifies a time offset in RAIL APIs.
 *
 * Different APIs use the same constants and may provide more specifics about
 * how they're used but the general use for each is described below.
 */
RAIL_ENUM(RAIL_TimeMode_t) {
  /**
   * The time specified is an exact time in the RAIL timebase. The given
   * event should happen at exactly that time. If this time is already in the
   * past, an error is returned. Because the RAIL timebase wraps at 32
   * bits, there is no real 'past'. Instead, any event greater than
   * 3/4 of the way into the future is considered to be in the past.
   */
  RAIL_TIME_ABSOLUTE,
  /**
   * The time specified is relative to the current time. The event should occur
   * that many ticks in the future. Delays are only guaranteed at least as long
   * as the value specified. An overhead may occur between the time when the
   * API is called and when the delay starts. As a result, using this for
   * operations that must happen at an exact given time is not recommended.
   * For that, you must use \ref RAIL_TIME_ABSOLUTE delays.
   *
   * Note that, if you specify a delay 0, that event is triggered as soon as
   * possible. This is different than specifying an absolute time of now which
   * would return an error unless it was possible.
   */
  RAIL_TIME_DELAY,
  /**
   * The specified time is invalid and should be ignored. For some APIs this
   * can also indicate that any previously stored delay should be invalidated
   * and disabled.
   */
  RAIL_TIME_DISABLED
};

/// Forward declaration of RAIL_MultiTimer
struct RAIL_MultiTimer;

/**
 * @typedef RAIL_MultiTimerCallback_t
 * @brief Callback fired when timer expires
 *
 * @param[in] tmr pointer to expired timer
 * @param[in] expectedTimeOfEvent Absolute time event fired
 * @param[in] cbArg User supplied callback argument
 */
typedef void (*RAIL_MultiTimerCallback_t)(struct RAIL_MultiTimer *tmr,
                                          RAIL_Time_t expectedTimeOfEvent,
                                          void *cbArg);

/**
 * @struct RAIL_MultiTimer_t
 * @brief RAIL timer state structure
 *
 * This structure is filled out and maintained internally only.
 * The user/application should not alter any elements of this structure.
 */
typedef struct RAIL_MultiTimer {
  RAIL_Time_t       absOffset;        /**< absolute time before next event */
  RAIL_Time_t       relPeriodic;      /**< relative, periodic time between events; 0 = timer is oneshot */
  RAIL_MultiTimerCallback_t callback; /**< user callback */
  void                *cbArg;          /**< user callback argument */
  struct RAIL_MultiTimer   *next;          /**< pointer to next soft timer structure */
  uint8_t             priority;       /**< priority of callback; 0 = highest priority; 255 = lowest */
  bool                isRunning;      /**< soft timer currently running? */
  bool                doCallback;     /**< callback needs to run? */
} RAIL_MultiTimer_t;

/**
 * @enum RAIL_SleepConfig_t
 * @brief The config
 */
RAIL_ENUM(RAIL_SleepConfig_t) {
  RAIL_SLEEP_CONFIG_TIMERSYNC_DISABLED, /**< Disable timer sync before and after sleep */
  RAIL_SLEEP_CONFIG_TIMERSYNC_ENABLED, /**< Enable timer sync before and after sleep */
};

/**
 * @enum RAIL_PacketTimePosition_t
 * @brief The available packet timestamp position choices
 */
RAIL_ENUM(RAIL_PacketTimePosition_t) {
  /**
   * Indicates timestamp is not to be, or was not, provided.
   * Useful if the application doesn't care about packet timestamps
   * and doesn't want RAIL to spend time calculating one.
   */
  RAIL_PACKET_TIME_INVALID = 0,
  /**
   * Requests the choice most expedient for RAIL to calculate,
   * which may depend on the radio and/or its configuration.
   * The actual choice would always be reflected in the timePosition
   * field of \ref RAIL_RxPacketDetails_t or \ref RAIL_TxPacketDetails_t
   * returned, and would never be one of the _USED_TOTAL values.
   */
  RAIL_PACKET_TIME_DEFAULT = 1,
  /**
   * Requests the timestamp corresponding to the first preamble bit
   * sent or received.
   * Indicates that timestamp did not require using totalPacketBytes.
   */
  RAIL_PACKET_TIME_AT_PREAMBLE_START = 2,
  /**
   * Requests the timestamp corresponding to the first preamble bit
   * sent or received.
   * Indicates that timestamp did require using totalPacketBytes.
   */
  RAIL_PACKET_TIME_AT_PREAMBLE_START_USED_TOTAL = 3,
  /**
   * Requests the timestamp corresponding to right after its last
   * SYNC word bit has been sent or received.
   * Indicates that timestamp did not require using totalPacketBytes.
   */
  RAIL_PACKET_TIME_AT_SYNC_END = 4,
  /**
   * Requests the timestamp corresponding to right after its last
   * SYNC word bit has been sent or received.
   * Indicates that timestamp did require using totalPacketBytes.
   */
  RAIL_PACKET_TIME_AT_SYNC_END_USED_TOTAL = 5,
  /**
   * Reqeusts the timestamp corresponding to right after its last
   * bit has been sent or received.
   * Indicates that timestamp did not require using totalPacketBytes.
   */
  RAIL_PACKET_TIME_AT_PACKET_END = 6,
  /**
   * Reqeusts the timestamp corresponding to right after its last
   * bit has been sent or received.
   * Indicates that timestamp did require using totalPacketBytes.
   */
  RAIL_PACKET_TIME_AT_PACKET_END_USED_TOTAL = 7,
  RAIL_PACKET_TIME_COUNT /**< Count of the choices in this enum */
};

/**
 * @struct RAIL_PacketTimeStamp_t
 * @brief Information needed to calculate and represent a packet timestamp.
 */
typedef struct RAIL_PacketTimeStamp {
  /**
   * Timestamp of the packet in the RAIL timebase.
   */
  RAIL_Time_t packetTime;
  /**
   * A value specifying the total length in bytes of the packet for
   * use when calculating the packetTime requested by the timePosition
   * field. This should account for all bytes sent over the air after
   * the Preamble and Sync word(s), including any CRC bytes.
   */
  uint32_t totalPacketBytes;
  /**
   * A RAIL_PacketTimePosition_t value specifying the packet position
   * to return in the packetTime field.
   * If this is \ref RAIL_PACKET_TIME_DEFAULT then this field will be
   * updated with the actual position corresponding to the packetTime
   * value filled in by a call using this structure.
   */
  RAIL_PacketTimePosition_t timePosition;
} RAIL_PacketTimeStamp_t;

/** @} */ // end of group System_Timing

/******************************************************************************
 * Data Management Structures
 *****************************************************************************/
/**
 * @addtogroup Data_Management
 * @{
 */

/**
 * @enum RAIL_TxDataSource_t
 * @brief Transmit data sources supported by RAIL.
 */
RAIL_ENUM(RAIL_TxDataSource_t) {
  TX_PACKET_DATA, /**< Uses the frame hardware to packetize data */
};

/**
 * @enum RAIL_RxDataSource_t
 * @brief Receive data sources supported by RAIL.
 */
RAIL_ENUM(RAIL_RxDataSource_t) {
  RX_PACKET_DATA, /**< Uses the frame hardware to packetize data */
  RX_DEMOD_DATA, /**< Gets 8-bit data output from the demodulator */
  RX_IQDATA_FILTLSB, /**< Gets lower 16 bits of I/Q data provided to the
                          demodulator */
  RX_IQDATA_FILTMSB /**< Gets highest 16 bits of I/Q data provided to the
                         demodulator */
};

/**
 * @enum RAIL_DataMethod_t
 * @brief Methods for the application to provide and retrieve data from RAIL.
 */
RAIL_ENUM(RAIL_DataMethod_t) {
  PACKET_MODE, /**< Packet-based data method */
  FIFO_MODE, /**< FIFO-based data method */
};

/**
 * @struct RAIL_DataConfig_t
 * @brief RAIL data configuration structure
 *
 * Selects the transmit/receive data sources and the
 * method the application uses to provide/retrieve data from RAIL.
 */
typedef struct {
  RAIL_TxDataSource_t txSource; /**< Source of TX Data */
  RAIL_RxDataSource_t rxSource; /**< Source of RX Data */
  RAIL_DataMethod_t txMethod; /**< Method of providing transmit data */
  RAIL_DataMethod_t rxMethod; /**< Method of retrieving receive data */
} RAIL_DataConfig_t;

/** @} */ // end of group Data Management

/******************************************************************************
 * State Transitions
 *****************************************************************************/
/**
 * @addtogroup State_Transitions
 * @{
 */

/**
 * @enum RAIL_RadioState_t
 * @brief The state of the radio.
 */
RAIL_ENUM(RAIL_RadioState_t) {
  RAIL_RF_STATE_INACTIVE = 0u,       /**< Phy is inactive */
  RAIL_RF_STATE_ACTIVE = (1u << 0),  /**< Radio is idle, receiving, or
                                          transmitting a frame */
  RAIL_RF_STATE_RX = (1u << 1),      /**< Radio is in receive */
  RAIL_RF_STATE_TX = (1u << 2),      /**< Radio is in transmit */
  RAIL_RF_STATE_IDLE = (RAIL_RF_STATE_ACTIVE),  /**< Radio is idle */
  /** Radio is actively receiving a frame */
  RAIL_RF_STATE_RX_ACTIVE = (RAIL_RF_STATE_RX | RAIL_RF_STATE_ACTIVE),
  /** Radio is actively transmitting a frame */
  RAIL_RF_STATE_TX_ACTIVE = (RAIL_RF_STATE_TX | RAIL_RF_STATE_ACTIVE)
};

/**
 * @struct RAIL_StateTransitions_t
 * @brief Used to specify radio states to transition to on success or failure.
 */
typedef struct RAIL_StateTransitions {
  /**
   * Indicates the state the radio should return to after a successful action.
   */
  RAIL_RadioState_t success;
  /**
   * Indicates the state the radio should return to after an error.
   */
  RAIL_RadioState_t error;
} RAIL_StateTransitions_t;

/**
 * @struct RAIL_StateTiming_t
 * @brief Timing configuration structure for the RAIL State Machine.
 *
 * Configures the timings of the radio state transitions for common situations.
 * All of the listed timings are in us. Timing values cannot exceed 13 ms. A
 * value of 0 for the transition time means that the specified transition
 * should happen as fast as possible, even if the timing cannot be as
 * consistent. Any other timing value will be limited, so that the time can be
 * hit consistently. Transitions to IDLE always happen as fast as possible.
 */
typedef struct RAIL_StateTiming {
  uint16_t idleToRx; /**< Transition time from IDLE to RX */
  uint16_t txToRx; /**< Transition time from TX to RX */
  uint16_t idleToTx; /**< Transition time from IDLE to RX */
  uint16_t rxToTx; /**< Transition time from RX to TX */
  uint16_t rxSearchTimeout; /**< Time radio looks for packets when coming from
                                 idle, value of zero disables this feature */
  uint16_t txToRxSearchTimeout; /**< Time to look for packets when coming from
                                     tx, value of zero disables this feature */
} RAIL_StateTiming_t;

/**
 * @enum RAIL_IdleMode_t
 * @brief Enumeration for the different types of supported idle modes. These
 *   vary how quickly and destructively they put the radio into idle.
 */
RAIL_ENUM(RAIL_IdleMode_t) {
  /**
   * Idles the radio by turning off receive and canceling any future scheduled
   * receive or transmit operations. It does not abort a receive or
   * transmit in progress.
   */
  RAIL_IDLE,
  /**
   * Idles the radio by turning off receive and any scheduled events. It
   * also aborts any receive, transmit, or scheduled events in progress.
   */
  RAIL_IDLE_ABORT,
  /**
   * Forces the radio into a shutdown mode as quickly as possible. It
   * aborts all current operations and cancels any pending scheduled
   * operations.
   * It may also corrupt receive or transmit buffers and end up clearing them.
   */
  RAIL_IDLE_FORCE_SHUTDOWN,
  /**
   * Similar to the \ref RAIL_IDLE_FORCE_SHUTDOWN command, it quickly
   * puts the radio into idle state. Additionally, it clears any
   * pending receive or transmit callbacks and clear both the receive and
   * transmit storage.
   */
  RAIL_IDLE_FORCE_SHUTDOWN_CLEAR_FLAGS
};

/** @} */ // end of group State_Transitions

/******************************************************************************
 * TX/RX Configuration Structures
 *****************************************************************************/
/**
 * @addtogroup Transmit
 * @{
 */

/**
 * @enum RAIL_TxOptions_t
 * @brief Transmit options, in reality a bitmask.
 */
RAIL_ENUM_GENERIC(RAIL_TxOptions_t, uint32_t) {
  /** Shift position of \ref RAIL_TX_OPTION_WAIT_FOR_ACK bit */
  RAIL_TX_OPTION_WAIT_FOR_ACK_SHIFT = 0,
  /** Shift position of \ref RAIL_TX_OPTION_REMOVE_CRC bit */
  RAIL_TX_OPTION_REMOVE_CRC_SHIFT,
  /** Shift position of \ref RAIL_TX_OPTION_SYNC_WORD_ID bit */
  RAIL_TX_OPTION_SYNC_WORD_ID_SHIFT,
  /** Shift position of \ref RAIL_TX_OPTION_ANTENNA0 bit */
  RAIL_TX_OPTION_ANTENNA0_SHIFT,
  /** Shift position of \ref RAIL_TX_OPTION_ANTENNA1 bit */
  RAIL_TX_OPTION_ANTENNA1_SHIFT,
};

/** Value representing no options enabled. */
#define RAIL_TX_OPTIONS_NONE 0UL
/** Default is all options disabled. */
#define RAIL_TX_OPTIONS_DEFAULT RAIL_TX_OPTIONS_NONE
/**
 * Option to configure whether or not the TX'ing node will listen for an ACK.
 * If this is false, the isAck flag in RAIL_RxPacketDetails_t of a received
 * packet will always be false.
 */
#define RAIL_TX_OPTION_WAIT_FOR_ACK (1UL << RAIL_TX_OPTION_WAIT_FOR_ACK_SHIFT)
/**
 * Option to remove crc bytes from tx packets. If you want to be able to
 * receive packets with this option set as true, you'll need to set
 * the IGNORE_CRC_ERRORS option on the receive side.
 */
#define RAIL_TX_OPTION_REMOVE_CRC (1UL << RAIL_TX_OPTION_REMOVE_CRC_SHIFT)
/**
 * Option to select which sync word to send (0 or 1). Note that this does
 * not set the actual sync words, it just picks which of the two will be
 * sent with the packet.
 */
#define RAIL_TX_OPTION_SYNC_WORD_ID (1UL << RAIL_TX_OPTION_SYNC_WORD_ID_SHIFT)
/**
 * Option to select antenna 0 for transmission. If not antenna selection
 * option is set or if both antenna options are set, then the TX will
 * take place in either antenna depending on the last RX or TX
 * configuration. This option is only valid on platforms that support
 * antenna selection.
 */
#define RAIL_TX_OPTION_ANTENNA0 (1UL << RAIL_TX_OPTION_ANTENNA0_SHIFT)
/**
 * Option to select antenna 1 for transmission. If not antenna selection
 * option is set or if both antenna options are set, then the TX will
 * take place in either antenna depending on the last RX or TX
 * configuration. This option is only valid on platforms that support
 * antenna selection.
 */
#define RAIL_TX_OPTION_ANTENNA1 (1UL << RAIL_TX_OPTION_ANTENNA1_SHIFT)

/** Value representing all possible options */
#define RAIL_TX_OPTIONS_ALL 0xFFFFFFFFUL

/**
 * @struct RAIL_TxPacketDetails_t
 * @brief Detailed information requested and about the packet that was just
 *   transmitted.
 */
typedef struct RAIL_TxPacketDetails {
  /**
   * Timestamp of the transmitted packet in the RAIL timebase,
   * filled in by RAIL_GetTxPacketDetails().
   */
  RAIL_PacketTimeStamp_t timeSent;
  /**
   * Indicates whether the transmitted packet was an ACK. An 'ACK' is defined
   * as a packet sent in response to a received ACK-requesting frame when
   * auto ACK is enabled. Set to false for not an ACK, and true for an ACK.
   * It should always be set false if auto ACK is not enabled.
   */
  bool isAck;
} RAIL_TxPacketDetails_t;

/**
 * @enum RAIL_ScheduledTxDuringRx_t
 * @brief Enumerates the possible outcomes of what will occur if a
 *   scheduled TX ends up firing during RX. Because RX and TX can't
 *   happen at the same time, it is up to the user how the TX should be
 *   handled. This enumeration is passed into RAIL_StartScheduledTx()
 *   as part of \ref RAIL_ScheduleTxConfig_t.
 */
RAIL_ENUM(RAIL_ScheduledTxDuringRx_t) {
  /**
   * The scheduled TX will be postponed until RX completes and then sent.
   */
  RAIL_SCHEDULED_TX_DURING_RX_POSTPONE_TX,
  /**
   * The scheduled TX will be aborted and a TX aborted event will fire.
   */
  RAIL_SCHEDULED_TX_DURING_RX_ABORT_TX
};

/**
 * @struct RAIL_ScheduleTxConfig_t
 * @brief Configuration structure for a scheduled transmit.
 */
typedef struct RAIL_ScheduleTxConfig {
  /**
   * The time when to transmit this packet. The exact interpretation of
   * this value depends on the mode specified below.
   */
  RAIL_Time_t when;
  /**
   * The type of delay. See the \ref RAIL_TimeMode_t documentation for
   * more information. Be sure to use \ref RAIL_TIME_ABSOLUTE delays for
   * time-critical protocols.
   */
  RAIL_TimeMode_t mode;
  /**
   * Which action to take with a scheduled TX if it occurs during RX.
   * See \ref RAIL_ScheduledTxDuringRx_t structure for more information on
   * potential options.
   */
  RAIL_ScheduledTxDuringRx_t txDuringRx;
} RAIL_ScheduleTxConfig_t;

/**
 * @def RAIL_MAX_LBT_TRIES
 * @brief The maximum number of LBT/CSMA retries supported
 */
#define RAIL_MAX_LBT_TRIES 15

/**
 * @struct RAIL_CsmaConfig_t
 * @brief Configuration structure for the CSMA transmit algorithm.
 */
typedef struct RAIL_CsmaConfig {
  /**
   * Minimum (starting) exponent for CSMA backoff (2^exp - 1)
   */
  uint8_t  csmaMinBoExp;
  /**
   * Maximum exponent for CSMA backoff
   */
  uint8_t  csmaMaxBoExp;
  /**
   * A number of CCA failures before report CCA_FAIL with a maximum value
   * defined in @ref RAIL_MAX_LBT_TRIES). A value 0 performs no CCA assessments
   * and always transmits immediately.
   */
  uint8_t  csmaTries;
  /**
   * The CCA RSSI threshold, in dBm, above which the channel is
   * considered 'busy'.
   */
  int8_t   ccaThreshold;
  /**
   * The backoff unit period in RAIL's microsecond time base. It is
   * multiplied by the random backoff exponential controlled by @ref
   * csmaMinBoExp and @ref csmaMaxBoExp to determine the overall backoff
   * period. For random backoffs, any value above 511 microseconds will
   * be truncated. For fixed backoffs it can go up to 65535 microseconds.
   */
  uint16_t ccaBackoff;
  /**
   * CCA check duration in microseconds.
   */
  uint16_t ccaDuration;
  /**
   * An overall timeout, in RAIL's microsecond time base, for the operation.
   * If the transmission doesn't start before this timeout expires, the
   * transmission will fail. A value 0 means no timeout is imposed.
   */
  RAIL_Time_t csmaTimeout;
} RAIL_CsmaConfig_t;

/**
 * @def RAIL_CSMA_CONFIG_802_15_4_2003_2p4_GHz_OQPSK_CSMA
 * @brief RAIL_CsmaConfig_t initializer configuring CSMA per IEEE 802.15.4-2003
 *   on 2.4 GHz OSPSK, commonly used by ZigBee.
 */
#define RAIL_CSMA_CONFIG_802_15_4_2003_2p4_GHz_OQPSK_CSMA {                    \
    /* CSMA per 802.15.4-2003 on 2.4 GHz OSPSK, commonly used by ZigBee     */ \
    /* csmaMinBoExp */ 3,   /* 2^3-1 for 0..7 backoffs on 1st try           */ \
    /* csmaMaxBoExp */ 5,   /* 2^5-1 for 0..31 backoffs on 3rd+ tries       */ \
    /* csmaTries    */ 5,   /* 5 tries overall (4 re-tries)                 */ \
    /* ccaThreshold */ -75, /* 10 dB above sensitivity                      */ \
    /* ccaBackoff   */ 320, /* 20 symbols at 16 us/symbol                   */ \
    /* ccaDuration  */ 128, /* 8 symbols at 16 us/symbol                    */ \
    /* csmaTimeout  */ 0,   /* no timeout                                   */ \
}

/**
 * @def RAIL_CSMA_CONFIG_SINGLE_CCA
 * @brief RAIL_CsmaConfig_t initializer configuring a single CCA prior to TX.
 *   It can be used to as a basis for implementing other channel access schemes
 *   with custom backoff delays. User can override ccaBackoff with a fixed
 *   delay on each use.
 */
#define RAIL_CSMA_CONFIG_SINGLE_CCA {                                          \
    /* Perform a single CCA after 'fixed' delay                             */ \
    /* csmaMinBoExp */ 0,   /* Used for fixed backoff                       */ \
    /* csmaMaxBoExp */ 0,   /* Used for fixed backoff                       */ \
    /* csmaTries    */ 1,   /* Single try                                   */ \
    /* ccaThreshold */ -75, /* Override if not desired choice               */ \
    /* ccaBackoff   */ 0,   /* No backoff (override with fixed value)       */ \
    /* ccaDuration  */ 128, /* Override if not desired length               */ \
    /* csmaTimeout  */ 0,   /* no timeout                                   */ \
}

/**
 * @struct RAIL_LbtConfig_t
 * @brief Configuration structure for the LBT transmit algorithm.
 */
typedef struct RAIL_LbtConfig {
  /**
   * Maximum backoff random multiplier
   */
  uint8_t  lbtMinBoRand;
  /**
   * Maximum backoff random multiplier
   */
  uint8_t  lbtMaxBoRand;
  /**
   * The number of CCA failures before reporting CCA_FAIL. The maximum
   * supported value for this field is defined in \ref RAIL_MAX_LBT_TRIES.
   * A value 0 performs no CCA assessments and always transmits immediately.
   */
  uint8_t  lbtTries;
  /**
   * The CCA RSSI threshold, in dBm, above which the channel is
   * considered 'busy'.
   */
  int8_t   lbtThreshold;
  /**
   * The backoff unit period, in RAIL's microsecond time base. It is
   * multiplied by the random backoff multiplier controlled by @ref
   * lbtMinBoRand and @ref lbtMaxBoRand to determine the overall backoff
   * period. For random backoffs, any value above 511 microseconds will
   * be truncated. For fixed backoffs, it can go up to 65535 microseconds.
   */
  uint16_t lbtBackoff;
  /**
   * LBT check duration in microseconds.
   */
  uint16_t lbtDuration;
  /**
   * An overall timeout, in RAIL's microsecond time base, for the
   * operation. If transmission doesn't start before this timeout expires, the
   * transmission will fail. This is important for limiting LBT due to LBT's
   * unbounded requirement that if the channel is busy, the next try must wait
   * for the channel to clear. A value 0 means no timeout is imposed.
   */
  RAIL_Time_t lbtTimeout;
} RAIL_LbtConfig_t;

/**
 * @def RAIL_LBT_CONFIG_ETSI_EN_300_220_1_V2_4_1
 * @brief RAIL_LbtConfig_t initializer configuring LBT per ETSI 300 220-1
 *   V2.4.1 for a typical Sub-GHz band. To be practical, users should override
 *   lbtTries and/or lbtTimeout so channel access failure will be reported in a
 *   reasonable time frame rather than the unbounded time frame ETSI defined.
 */
#define RAIL_LBT_CONFIG_ETSI_EN_300_220_1_V2_4_1 {                             \
    /* LBT per ETSI 300 220-1 V2.4.1                                        */ \
    /* LBT time = random backoff of 0-5ms in .5ms increments plus 5ms fixed */ \
    /* lbtMinBoRand */ 0,    /*                                             */ \
    /* lbtMaxBoRand */ 10,   /*                                             */ \
    /* lbtTries     */ RAIL_MAX_LBT_TRIES, /* the maximum supported         */ \
    /* lbtThreshold */ -87,  /*                                             */ \
    /* lbtBackoff   */ 500,  /* 0.5 ms                                      */ \
    /* lbtDuration  */ 5000, /* 5 ms                                        */ \
    /* lbtTimeout   */ 0,    /* no timeout (recommend user override)        */ \
}

/** @} */ // end of group Transmit

/**
 * @addtogroup Receive
 * @{
 */

/**
 * @enum RAIL_RxOptions_t
 * @brief Receive options, in reality a bitmask.
 */
RAIL_ENUM_GENERIC(RAIL_RxOptions_t, uint32_t) {
  /** Shift position of \ref RAIL_RX_OPTION_STORE_CRC bit */
  RAIL_RX_OPTION_STORE_CRC_SHIFT = 0,
  /** Shift position of \ref RAIL_RX_OPTION_IGNORE_CRC_ERRORS bit */
  RAIL_RX_OPTION_IGNORE_CRC_ERRORS_SHIFT,
  /** Shift position of \ref RAIL_RX_OPTION_ENABLE_DUALSYNC bit */
  RAIL_RX_OPTION_ENABLE_DUALSYNC_SHIFT,
  /** Shift position of \ref RAIL_RX_OPTION_TRACK_ABORTED_FRAMES bit */
  RAIL_RX_OPTION_TRACK_ABORTED_FRAMES_SHIFT,
  /** Shift position of \ref RAIL_RX_OPTION_REMOVE_APPENDED_INFO bit */
  RAIL_RX_OPTION_REMOVE_APPENDED_INFO_SHIFT,
  /** Shift position of \ref RAIL_RX_OPTION_ANTENNA0 bit */
  RAIL_RX_OPTION_ANTENNA0_SHIFT,
  /** Shift position of \ref RAIL_RX_OPTION_ANTENNA1 bit */
  RAIL_RX_OPTION_ANTENNA1_SHIFT,
};

/** Value representing no options enabled. */
#define RAIL_RX_OPTIONS_NONE 0
/** Default is all options disabled. */
#define RAIL_RX_OPTIONS_DEFAULT RAIL_RX_OPTIONS_NONE

/**
 * Option to configure whether the CRC portion of the packet is included in
 * the packet payload exposed to the app on packet reception.
 * Defaults to false.
 */
#define RAIL_RX_OPTION_STORE_CRC (1UL << RAIL_RX_OPTION_STORE_CRC_SHIFT)
/**
 * Option to configure whether CRC errors will be ignored
 * if this is set, RX will still be successful, even if
 * the CRC does not pass the check. Defaults to false.
 */
#define RAIL_RX_OPTION_IGNORE_CRC_ERRORS (1UL << RAIL_RX_OPTION_IGNORE_CRC_ERRORS_SHIFT)

/**
 * Option to control which sync words will be accepted. Setting it to
 * 0 (default) will cause the receiver to listen for SYNC0 only, 1 causes
 * the receiver to listen for either SYNC0 or SYNC1. RX appended info will
 * contain which sync word was detected. Note, this only affects which
 * sync word(s) are received, but not what each of the sync words actually are.
 * This feature may not be available on some combinations of chips, phys, and
 * protocols. Consult the data sheet or support team for more details.
 */
#define RAIL_RX_OPTION_ENABLE_DUALSYNC (1UL << RAIL_RX_OPTION_ENABLE_DUALSYNC_SHIFT)

/**
 * Option to configure whether frames which are aborted during reception
 * should continue to be tracked. Setting this option allows viewing Packet
 * Trace information for frames which get discarded. Defaults to false.
 */
#define RAIL_RX_OPTION_TRACK_ABORTED_FRAMES (1UL << RAIL_RX_OPTION_TRACK_ABORTED_FRAMES_SHIFT)

/**
 * Option to configure whether appended info is included after received
 * frames. Default is false.
 */
#define RAIL_RX_OPTION_REMOVE_APPENDED_INFO (1UL << RAIL_RX_OPTION_REMOVE_APPENDED_INFO_SHIFT)

/**
 * Option to select the use of antenna 0 when doing RX. If no antenna option
 * is selected, the RX may take place on either antenna depending on the last
 * RX or TX configuration. Defaults to false.
 * This option is only valid on platforms that support antenna selection.
 */
#define RAIL_RX_OPTION_ANTENNA0 (1UL << RAIL_RX_OPTION_ANTENNA0_SHIFT)

/**
 * Option to select the use of antenna 1 when doing RX. If no antenna option
 * is selected, the RX may take place on either antenna depending on the last
 * RX or TX configuration. Defaults to false.
 * This option is only valid on platforms that support antenna selection.
 */
#define RAIL_RX_OPTION_ANTENNA1 (1UL << RAIL_RX_OPTION_ANTENNA1_SHIFT)

/**
 * Option to automatically choose an antenna. If both antenna 0 and
 * antenna 1 options are set the chip will switch between antennas and
 * will automatically choose one.
 */
#define RAIL_RX_OPTION_ANTENNA_AUTO (RAIL_RX_OPTION_ANTENNA0 | RAIL_RX_OPTION_ANTENNA1)

/** Value representing all possible options */
#define RAIL_RX_OPTIONS_ALL 0xFFFFFFFFUL

/** The value returned by RAIL for an invalid RSSI, in dBm */
#define RAIL_RSSI_INVALID_DBM     (-128)
/** The value returned by RAIL for an invalid RSSI: in quarter dBm */
#define RAIL_RSSI_INVALID         ((int16_t)(RAIL_RSSI_INVALID_DBM * 4))

/**
 * @struct RAIL_ScheduleRxConfig_t
 * @brief Configures the scheduled RX algorithm.
 *
 * Defines the start and end times of the receive window created
 * for a scheduled receive. If either start or end times are disabled, they
 * will be ignored.
 */
typedef struct RAIL_ScheduleRxConfig {
  /**
   * The time to start receive. See startMode for more information about they
   * types of start times that you can specify.
   */
  RAIL_Time_t start;
  /**
   * How to interpret the time value specified in the start parameter. See the
   * \ref RAIL_TimeMode_t documentation for more information. Use
   * \ref RAIL_TIME_ABSOLUTE for absolute times, \ref RAIL_TIME_DELAY for times
   * relative to the current time, and \ref RAIL_TIME_DISABLED to ignore the
   * start time.
   */
  RAIL_TimeMode_t startMode;
  /**
   * The time to end receive. See endMode for more information about the types
   * of end times you can specify.
   */
  RAIL_Time_t end;
  /**
   * How to interpret the time value specified in the end parameter. See the
   * \ref RAIL_TimeMode_t documentation for more information. Note that, in
   * this API, if you specify a \ref RAIL_TIME_DELAY, it is relative to the
   * start time if given and relative to now if none is specified. Also, using
   * \ref RAIL_TIME_DISABLED means that this window will not end unless you
   * explicitly call RAIL_RfIdle() or add an end event through a future update
   * to this configuration.
   */
  RAIL_TimeMode_t endMode;
  /**
   * While in scheduled RX, you can still control the radio state via
   * state transitions. This option configures whether a transition
   * to RX goes back to scheduled RX or to the normal RX state. Once in the
   * normal RX state, you will effectively end the scheduled RX window and can
   * continue to receive indefinitely depending on the state transitions. Set
   * to 1 to transition to normal RX and 0 to stay in the scheduled RX.
   */
  uint8_t rxTransitionEndSchedule;
  /**
   * If set to 0, this will allow any packets, which are received when the
   * window end event occurs, to complete. If set to anything else, an abort
   * of any packets received when the window end occurs is forced.
   */
  uint8_t hardWindowEnd;
} RAIL_ScheduleRxConfig_t;

/**
 * @enum RAIL_RxPacketStatus_t
 * @brief The packet status code associated with a packet received or
 *   currently being received.
 *
 * @note RECEIVING implies some packet data may be available, but
 *   is untrustworthy (not CRC-verified) and might disappear if the packet
 *   is rolled back on error.
 * @note In RX FIFO mode, ABORT statuses imply some packet data may be
 *   available, but it's incomplete and not trustworthy.
 * @note READY statuses indicate all packet data is available, including
 *   packet details (if enabled), but only \ref RAIL_RX_PACKET_READY_SUCCESS
 *   indicates the packet data is trustworthy.
 */
RAIL_ENUM(RAIL_RxPacketStatus_t) {
  RAIL_RX_PACKET_NONE = 0,        /**< Radio is idle or searching for a packet. */
  RAIL_RX_PACKET_ABORT_FORMAT,    /**< Format/Length error. */
  RAIL_RX_PACKET_ABORT_FILTERED,  /**< Filtering error (address). */
  RAIL_RX_PACKET_ABORT_ABORTED,   /**< Aborted error. */
  RAIL_RX_PACKET_ABORT_OVERFLOW,  /**< Receive overflowed buffer. */
  RAIL_RX_PACKET_ABORT_CRC_ERROR, /**< CRC error aborted. */
  RAIL_RX_PACKET_READY_CRC_ERROR, /**< CRC error accepted (details available). */
  RAIL_RX_PACKET_READY_SUCCESS,   /**< Success (details available). */
  RAIL_RX_PACKET_RECEIVING,       /**< Receiving in progress. */
};

/**
 * @typedef RAIL_RxPacketHandle_t
 * @brief A handle used to reference a packet during reception processing.
 *   There are several sentinel handle values that pertain to certain
 *   circumstances: \ref RAIL_RX_PACKET_HANDLE_INVALID, \ref
 *   RAIL_RX_PACKET_HANDLE_OLDEST and \ref RAIL_RX_PACKET_HANDLE_NEWEST.
 */
typedef const void *RAIL_RxPacketHandle_t;

/** Invalid RX packet handle value */
#define RAIL_RX_PACKET_HANDLE_INVALID  (NULL)
/** Special RX packet handle to refer to the oldest unreleased packet */
#define RAIL_RX_PACKET_HANDLE_OLDEST   ((RAIL_RxPacketHandle_t) 1)
/** Special RX packet handle to refer to the newest unreleased packet
 *  when in callback context. For a callback involving a completed
 *  receive event, this refers to the packet just completed. For
 *  other callback events, this refers to the next packet to be
 *  completed, which might be in-progress or might not have even
 *  started yet.
 */
#define RAIL_RX_PACKET_HANDLE_NEWEST   ((RAIL_RxPacketHandle_t) 2)

/**
 * @struct RAIL_RxPacketInfo_t
 * @brief Basic information about a packet being received or already
 *   completed and awaiting processing, including memory pointers to
 *   its data in the circular RX FIFO buffer. This packet information
 *   refers to remaining packet data that has not already been consumed
 *   by RAIL_ReadRxFifo().
 * @note Because the RX FIFO buffer is circular, a packet might start
 *   near the end of the buffer and wrap around to the beginning of
 *   the buffer to finish, hence the distinction between the first
 *   and last portions. Packets that fit without wrapping only have
 *   a first portion (firstPortionBytes == packetBytes and lastPortionData
 *   will be NULL).
 */
typedef struct RAIL_RxPacketInfo {
  RAIL_RxPacketStatus_t packetStatus; /**< Packet status of this packet. */
  uint16_t packetBytes;               /**< Number of packet data bytes
                                           available to read in this packet. */
  uint16_t firstPortionBytes;         /**< Number of bytes in first portion. */
  uint8_t *firstPortionData;          /**< Pointer to first portion of packet
                                           data containing firstPortionBytes
                                           number of bytes. */
  uint8_t *lastPortionData;           /**< Pointer to last portion of
                                           packet, if any; NULL otherwise. The
                                           number of bytes in this portion is
                                           packetBytes - firstPortionBytes. */
} RAIL_RxPacketInfo_t;

/**
 * @struct RAIL_RxPacketDetails_t
 * @brief Successfully received packet details obtained via
 *   RAIL_GetRxPacketDetails().
 *
 */
typedef struct RAIL_RxPacketDetails {
  /**
   * Timestamp of the received packet in the RAIL timebase. The
   * time is the end of the sync word of the received packet.
   */
  RAIL_PacketTimeStamp_t timeReceived;
  /**
   * Indicates whether the CRC passed or failed for the receive packet. It
   * is set to 0 for fail and 1 for pass.
   */
  bool crcPassed;
  /**
   * Indicates whether the received packet is an ACK. An 'ACK' is defined as a
   * packet received during the RX ACK window when auto ACK is enabled.
   * Set to 0 if not an ACK, and 1 for an ACK. It is always 0 if
   * auto ACK is not enabled.
   */
  bool isAck;
  /**
   * RSSI of the received packet in integer dBm. It is latched when the sync
   * word is detected for the packet.
   */
  int8_t rssi;
  /**
   * Link quality indicator of the received packet. It is calculated as the
   * average correlation for the first 8 symbols in a frame.
   */
  uint8_t lqi;
  /**
   * For radios and PHY configurations that support multiple sync words, this
   * number is the ID of the sync word that was used for this packet.
   */
  uint8_t syncWordId;
  /**
   * In configurations where the radio has the option of receiving a given
   * packet in multiple ways, indicates which of the sub-PHY options
   * was used to receive the packet. Most radio configurations do not have
   * this ability, and the subPhy is set to 0 in those cases.
   */
  uint8_t subPhyId;
  /**
   * For configurations where the device has multiple antennas, indicates
   * which antenna received the packets. For hardware configurations
   * with only one antenna, this will be set to the default of 0.
   */
  uint8_t antennaId;
} RAIL_RxPacketDetails_t;

/**
 * @addtogroup Address_Filtering
 * @{
 */

/// Default address filtering match table for configurations that use only one
/// address field. The truth table for address matching is shown below.
///
/// |          | 0000 | 0001 | 0010 | 0100 | 1000 |
/// |----------|------|------|------|------|------|
/// | __0000__ |    0 |    1 |    1 |    1 |    1 |
/// | __0001__ |    1 |    1 |    1 |    1 |    1 |
/// | __0010__ |    1 |    1 |    1 |    1 |    1 |
/// | __0100__ |    1 |    1 |    1 |    1 |    1 |
/// | __1000__ |    1 |    1 |    1 |    1 |    1 |
///
#define ADDRCONFIG_MATCH_TABLE_SINGLE_FIELD (0x1fffffe)
/// Default address filtering match table for configurations that use two
/// address fields and want to match the same index in each. The truth
/// table for address matching is shown below.
///
/// |          | 0000 | 0001 | 0010 | 0100 | 1000 |
/// |----------|------|------|------|------|------|
/// | __0000__ |    0 |    0 |    0 |    0 |    0 |
/// | __0001__ |    0 |    1 |    0 |    0 |    0 |
/// | __0010__ |    0 |    0 |    1 |    0 |    0 |
/// | __0100__ |    0 |    0 |    0 |    1 |    0 |
/// | __1000__ |    0 |    0 |    0 |    0 |    1 |
#define ADDRCONFIG_MATCH_TABLE_DOUBLE_FIELD (0x1041040)

/// The maximum number of address fields that can be used by the address
/// filtering logic.
#define ADDRCONFIG_MAX_ADDRESS_FIELDS (2)

/**
 * @struct RAIL_AddrConfig_t
 * @brief A structure to configure the address filtering functionality in RAIL.
 */
typedef struct RAIL_AddrConfig {
  /**
   * A list of the start offsets for each field.
   *
   * These offsets are specified relative to the previous field's end. In the
   * case of the first field it's relative to either the beginning of the packet
   * or the end of the frame type byte if frame type decoding is enabled. If a
   * field is unused, it's offset should be set to 0.
   */
  uint8_t offsets[ADDRCONFIG_MAX_ADDRESS_FIELDS];

  /**
   * A list of the address field sizes.
   *
   * These sizes are specified in bytes from 0 to 8. If you choose a
   * size of 0, this field is effectively disabled.
   */
  uint8_t sizes[ADDRCONFIG_MAX_ADDRESS_FIELDS];

  /**
   * The truth table to determine how the two fields combine to create a match.
   *
   * For detailed information about how this truth table is formed see the
   * detailed description of @ref Address_Filtering.
   *
   * For simple predefined configurations you can use the following defines.
   *  - ADDRCONFIG_MATCH_TABLE_SINGLE_FIELD
   *    - For filtering that only uses a single address field
   *  - ADDRCONFIG_MATCH_TABLE_DOUBLE_FIELD for two field filtering where you
   *    - For filtering that uses two address fields in a configurations where
   *      you want the following logic `((Field0, Index0) && (Field1, Index0))
   *      || ((Field0, Index1) && (Field1, Index1)) || ...`
   */
  uint32_t matchTable;
} RAIL_AddrConfig_t;

/** @} */ // end of group Address_Filtering

/** @} */ // end of group Receive

/**
 * @addtogroup Auto_Ack
 * @{
 */
/**
 * @struct RAIL_AutoAckConfig_t
 * @brief Enables/disables the auto ACK algorithm, based on "enable". The
 *   structure provides a default state (the "success" of tx/rxTransitions
 *   when acking is being enabled) for the radio to return to after an ACK
 *   operation occurs (transmitting or attempting to receive an ACK), or normal
 *   state transitions to return to in the case that acking is being
 *   disabled. Regardless whether the ACK operation was successful, the
 *   radio returns to the specified success state.
 *
 * ackTimeout specifies how long to stay in receive and wait for an ACK
 * before issuing a RAIL_EVENT_RX_ACK_TIMEOUT event and return to the
 * default state.
 */
typedef struct RAIL_AutoAckConfig {
  /**
   * Indicate whether auto-acking should be enabled or disabled.
   */
  bool enable;
  /**
   * Define the RX ACK timeout duration in µs. Limited to a maximum 65.535 ms.
   * Only applied when auto acking is enabled. Triggers a
   * RAIL_EVENT_RX_ACK_TIMEOUT event if this threshold is exceeded.
   */
  uint16_t ackTimeout;
  /**
   * State transitions to do after receiving a packet. When auto acking is
   * enabled, the "error" transition is always ignored and the radio will
   * return to the "success" state after any acking sequence. See
   * \ref RAIL_ConfigAutoAck for more details on this.
   */
  RAIL_StateTransitions_t rxTransitions;
  /**
   * State transitions to do after transmitting a packet. When auto acking is
   * enabled, the "error" transition is always ignored and the radio will
   * return to the "success" state after any acking sequence. See
   * \ref RAIL_ConfigAutoAck for more details on this.
   */
  RAIL_StateTransitions_t txTransitions;
} RAIL_AutoAckConfig_t;

/// Acknowledgment packets cannot be longer than 64 bytes.
#define RAIL_AUTOACK_MAX_LENGTH 64

/** @} */ // end of group Auto_Ack

/******************************************************************************
 * Calibration Structures
 *****************************************************************************/
/**
 * @addtogroup Calibration
 * @{
 */

/**
 * @typedef RAIL_CalMask_t
 * @brief Calibration mask type
 *
 * This type is a bitmask of different RAIL calibration values. The exact
 * meaning of these bits depends on what a particular chip supports.
 */
typedef uint32_t RAIL_CalMask_t;

/** @} */ // end of group Calibration

/******************************************************************************
 * RF Sense Structures
 *****************************************************************************/
/**
 * @addtogroup Rf_Sense
 * @{
 */

/**
 * A pointer to an rfsense callback function.
 */
typedef void (*RAIL_RfSense_CallbackPtr_t)(void);

/**
 * @enum RAIL_RfSenseBand_t
 * @brief Enumeration for specifying Rf Sense frequency band.
 */
RAIL_ENUM(RAIL_RfSenseBand_t) {
  RAIL_RFSENSE_OFF,    /**< RFSense is disabled */
  RAIL_RFSENSE_2_4GHZ, /**< RFSense is in 2.4 G band */
  RAIL_RFSENSE_SUBGHZ, /**< RFSense is in subgig band */
  RAIL_RFSENSE_ANY,    /**< RfSense is in both bands */
  RAIL_RFSENSE_MAX     // Must be last
};

/** @} */ // end of group Rf_Sense

/******************************************************************************
 * Diagnositc Structures
 *****************************************************************************/
/**
 * @addtogroup Diagnostic
 * @{
 */

/**
 * @enum RAIL_StreamMode_t
 * @brief Possible stream output modes.
 */
RAIL_ENUM(RAIL_StreamMode_t) {
  RAIL_STREAM_CARRIER_WAVE = 0, /**< Unmodulated carrier wave */
  RAIL_STREAM_PN9_STREAM = 1,   /**< PN9 byte sequence */
};

/**
 * @def RAIL_VERIFY_DURATION_MAX
 * @brief This radio state verification duration indicates to RAIL that
 *   all memory contents should be verified by RAIL before returning to the
 *   application.
 */
#define RAIL_VERIFY_DURATION_MAX 0xFFFFFFFFUL

/**
 * A pointer to a verification callback function. This will be called by the
 * radio state verification feature built into RAIL when a verified memory
 * value is different from its reference value.
 *
 * @param[in] address The address of the data in question.
 * @param[in] expectedValue The expected value of the data in question.
 * @param[in] actualValue The actual value of the data in question.
 * @return bool True indicates a data value difference is acceptable, and False
 *   indicates a data value difference in unacceptable.
 *
 * @note This callback will be issued when an address' value is different from
 *   its reference value and either of the following conditions are met:
 *   1) The default radio config provided by the radio configurator is used
 *      for verification purposes (i.e. a custom radio config is not supplied
 *      as an input to \ref RAIL_ConfigVerification()), and the radio
 *      configurator has flagged the address under question as being verifiable.
 *   2) A custom radio config is provided to the verification API (i.e.
 *      a custom radio config is supplied as an input to \ref
 *      RAIL_ConfigVerification()). When providing a custom radio config for
 *      verification purposes, all addresses in that config will be verified,
 *      regardless of whether or not the addresses are flagged as verifiable.
 */
typedef bool (*RAIL_VerifyCallbackPtr_t)(uint32_t address,
                                         uint32_t expectedValue,
                                         uint32_t actualValue);

/**
 * @struct RAIL_VerifyConfig_t
 * @brief The configuration array provided to RAIL for use by the radio state
 *   verification feature. This structure will be populated with appropriate
 *   values by calling \ref RAIL_ConfigVerification(). The application should
 *   not set or alter any of these structure elements.
 */
typedef struct RAIL_VerifyConfig {
  /** Internal verification tracking information */
  RAIL_Handle_t correspondingHandle;
  /** Internal verification tracking information */
  uint32_t nextIndexToVerify;
  /** Internal verification tracking information */
  const uint32_t *override;
  /** Internal verification tracking information */
  RAIL_VerifyCallbackPtr_t cb;
} RAIL_VerifyConfig_t;

#ifndef DOXYGEN_SHOULD_SKIP_THIS

typedef struct RAIL_DirectModeConfig {
  bool enable; /**< Indicates whether to enable direct mode. */
} RAIL_DirectModeConfig_t;

#endif//DOXYGEN_SHOULD_SKIP_THIS

/** @} */ // end of group Diagnostic

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/******************************************************************************
 * Debug Structures
 *****************************************************************************/
/**
 * @addtogroup Debug
 * @{
 */

/**
 * @def RAIL_DEBUG_MODE_FREQ_OVERRIDE
 * @brief A bitmask to enable the frequency override debug mode to
 *   manually tune to a specified frequency. Note that this should only be used
 *   for testing and is not as tuned as frequencies from the calculator.
 */
#define RAIL_DEBUG_MODE_FREQ_OVERRIDE  0x00000001UL

/**
 * @def RAIL_DEBUG_MODE_VALID_MASK
 * @brief Any debug mode bits outside of this mask are invalid and ignored.
 */
#define RAIL_DEBUG_MODE_VALID_MASK     (!(RAIL_DEBUG_MODE_FREQ_OVERRIDE))

/** @} */ // end of group Debug

#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @}
 * end of RAIL_API
 */

#ifdef __cplusplus
}
#endif

#endif  // __RAIL_TYPES_H__
