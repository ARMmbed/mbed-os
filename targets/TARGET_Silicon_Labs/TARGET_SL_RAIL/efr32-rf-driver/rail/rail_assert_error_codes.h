/***************************************************************************//**
 * @file rail_assert_error_codes.h
 * @brief Definition of error codes that occur in rail for use in
      RAILCb_AssertFailed. This file is purely informational and optional -
      it need not be included even if rail_assert libraries are included.

 * @copyright Copyright 2017 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#include "rail_types.h"

/**
 * @addtogroup Assertions
 * @{
 */

#ifndef _RAIL_ASSERT_ERROR_CODES_

#define _RAIL_ASSERT_ERROR_CODES_

/**
 * Enumeration of all possible error codes from RAIL_ASSERT
 */
RAIL_ENUM(RAIL_AssertErrorCodes_t)
{
  RAIL_ASSERT_FAILED_APPENDED_INFO_MISSING,
  RAIL_ASSERT_FAILED_RX_FIFO_BYTES,
  RAIL_ASSERT_FAILED_RX_FIFO_ZERO_BYTES_READ,
  RAIL_ASSERT_FAILED_ILLEGAL_RXLEN_ENTRY_STATUS,
  RAIL_ASSERT_FAILED_BAD_PACKET_LENGTH,
  RAIL_ASSERT_FAILED_SYNTH_DIVCTRL_ENUM_CONVERSION_ERROR, //5
  RAIL_ASSERT_FAILED_UNEXPECTED_STATE_RX_FIFO,
  RAIL_ASSERT_FAILED_UNEXPECTED_STATE_RXLEN_FIFO,
  RAIL_ASSERT_FAILED_UNEXPECTED_STATE_TX_FIFO,
  RAIL_ASSERT_FAILED_UNEXPECTED_STATE_TXACK_FIFO,
  RAIL_ASSERT_FAILED_PBUFFER_NOT_DEFINED, //10
  RAIL_ASSERT_FAILED_INSUFFICIENT_BYTES_IN_RX_PACKET,
  RAIL_ASSERT_FAILED_CLOCK_PRESCALER,
  RAIL_ASSERT_FAILED_RTCC_POST_WAKEUP,
  RAIL_ASSERT_FAILED_SYNTH_VCO_FREQUENCY,
  RAIL_ASSERT_FAILED_RAC_STATE, //15
  RAIL_ASSERT_FAILED_RETIME_LIMIT,
  RAIL_ASSERT_FAILED_NESTED_SEQUENCER_LOCK,
  RAIL_ASSERT_FAILED_RSSI_AVERAGE_DONE,
  RAIL_ASSERT_FAILED_DFL_BITS_SIZE,
  RAIL_ASSERT_FAILED_PROTIMER_RANDOM_SEED, //20
  RAIL_ASSERT_FAILED_EFR32XG1_REGISTER_SIZE,
  RAIL_ASSERT_FAILED_PROTIMER_CHANNEL,
  RAIL_ASSERT_FAILED_TIMER_REQUIRES_WRAP,
  RAIL_ASSERT_FAILED_BASECNTTOP,
  RAIL_ASSERT_FAILED_DEPRECATED_LBTRETRY, //25
  RAIL_ASSERT_FAILED_RTCC_SYNC_MISSED,
  RAIL_ASSERT_FAILED_CLOCK_SOURCE_NOT_READY,
  RAIL_ASSERT_FAILED_TIMINGS_INVALID,
  RAIL_ASSERT_NULL_HANDLE,
  RAIL_ASSERT_FAILED_TMRDRV_SCHED_TIMER_NOT_RUNNING, //30
  RAIL_ASSERT_FAILED_NO_ACTIVE_CONFIG,
  RAIL_ASSERT_FAILED_NO_ACTIVE_HANDLE_SWITCH,
  RAIL_ASSERT_FAILED_RFINIT,
  RAIL_ASSERT_FAILED_NO_ACTIVE_HANDLE_SCHEDRX,
  RAIL_ASSERT_FAILED_INVALID_HANDLE_SCHEDTX, //35
  RAIL_ASSERT_FAILED_INACTIVE_HANDLE_SCHEDTX,
  RAIL_ASSERT_FAILED_CONFIG_INDEX_INVALID,
  RAIL_ASSERT_FAILED_NO_ACTIVE_HANDLE_SINGLEPROTOCOL,
  RAIL_ASSERT_DEPRECATED_FUNCTION,
  RAIL_ASSERT_MULTIPROTOCOL_NO_EVENT, //40
  RAIL_ASSERT_FAILED_INVALID_INTERRUPT_ENABLED,
  RAIL_ASSERT_CONVERSION_CURVES_NOT_INITIALIZED,
};

/**
 * Use this define to create an array of error messages that map to the codes
 * in \ref RAIL_AssertErrorCodes_t. You can use these to print slightly more
 * detailed error strings related to a particular assert error code if desired.
 * For example, you could implement your assert failed callback as follows to
 * make use of this.
 *
 * @code{.c}
 * void RAILCb_AssertFailed(RAIL_Handle_t railHandle, uint32_t errorCode)
 * {
 *   static const char* railErrorMessages[] = RAIL_ASSERT_ERROR_MESSAGES;
 *   const char *errorMessage = "Unknown";
 *
 *   // If this error code is within the range of known error messages then use
 *   // the appropriate error message.
 *   if (errorCode < (sizeof(railErrorMessages) / sizeof(char*))) {
 *     errorMessage = railErrorMessages[errorCode];
 *   }
 *   printf(errorMessage);
 *
 *   // Reset the chip since an assert is a fatal error
 *   NVIC_SystemReset();
 * }
 * @endcode
 */
#define RAIL_ASSERT_ERROR_MESSAGES {                                    \
    "Appended info missing from Rx packet",                             \
    "Payload bytes missing from Rx packet",                             \
    "Error reading back packet payload",                                \
    "Receive fifo entry has invalid status",                            \
    "Receive fifo entry bad packet length",                             \
    "Unable to configure radio for IR calibration",                     \
    "Reached unexpected state while handling Rx fifo events",           \
    "Reached unexpected state while handling RXLEN fifo events",        \
    "Reached unexpected state while handling Tx fifo events",           \
    "Reached unexpected state while handling Tx ACK fifo events",       \
    "No memory to store receive packet",                                \
    "Packet length longer than the receive FIFO size",                  \
    "Invalid radio clock prescaler",                                    \
    "Error synchronizing the RAIL timebase after sleep",                \
    "VCO frequency outside supported range",                            \
    "Radio active while changing channels",                             \
    "Unable to configure DCDC retiming",                                \
    "Nested attempt to lock the sequencer",                             \
    "RSSI averaging enabled without a valid callback",                  \
    "Invalid dynamic frame length setting provided (dflBits)",          \
    "Unable to seed radio pseudo random number generator",              \
    "Timeout exceeds EFR32XG1 register size",                           \
    "Invalid timer channel specified",                                  \
    "Timer value larger than RAIL timebase",                            \
    "LBT config exceeds EFR32XG1 register size",                        \
    "Deprecated CSMA/LBT retry callback unexpectedly called",           \
    "Could not synchronize RAIL timebase with the RTC",                 \
    "Clock source not ready",                                           \
    "Attempted to set RAIL timings to invalid value",                   \
    "NULL was supplied as a RAIL_Handle_t argument",                    \
    "Scheduled timer not running",                                      \
    "No active config to switch from",                                  \
    "No active handle after switch",                                    \
    "RfInit failed to configure active state",                          \
    "No active handle for scheduled rx",                                \
    "Invalid handle for scheduled tx",                                  \
    "Inactive handle for scheduled tx",                                 \
    "Invalid config index to switch to",                                \
    "No active handle for single protocol",                             \
    "This function is deprecated and must not be called",               \
    "Multiprotocol task started with no event to run",                  \
    "Invalid interrupt enabled",                                        \
    "Power conversion functions called before curves were initialized", \
}

#endif
/**
 * @}
 */
