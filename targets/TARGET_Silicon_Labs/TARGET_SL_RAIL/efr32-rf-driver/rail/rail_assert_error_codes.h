/***************************************************************************//**
 * @file rail_assert_error_codes.h
 * @brief Definition of error codes that occur in rail for use in
 *   RAILCb_AssertFailed. This file is purely informational and optional -
 *   it need not be included even if rail_assert libraries are included.
 * @copyright Copyright 2017 Silicon Laboratories, Inc. www.silabs.com
 ******************************************************************************/
#ifndef __RAIL_ASSERT_ERROR_CODES_H__
#define __RAIL_ASSERT_ERROR_CODES_H__

#include "rail_types.h"

/**
 * @addtogroup Assertions
 * @{
 */

/**
 * Enumeration of all possible error codes from RAIL_ASSERT
 */
RAIL_ENUM_GENERIC(RAIL_AssertErrorCodes_t, uint32_t)
{
  RAIL_ASSERT_FAILED_APPENDED_INFO_MISSING = 0,
  RAIL_ASSERT_FAILED_RX_FIFO_BYTES = 1,
  RAIL_ASSERT_FAILED_RX_FIFO_ZERO_BYTES_READ = 2,
  RAIL_ASSERT_FAILED_ILLEGAL_RXLEN_ENTRY_STATUS = 3,
  RAIL_ASSERT_FAILED_BAD_PACKET_LENGTH = 4,
  RAIL_ASSERT_FAILED_SYNTH_DIVCTRL_ENUM_CONVERSION_ERROR = 5,
  RAIL_ASSERT_FAILED_UNEXPECTED_STATE_RX_FIFO = 6,
  RAIL_ASSERT_FAILED_UNEXPECTED_STATE_RXLEN_FIFO = 7,
  RAIL_ASSERT_FAILED_UNEXPECTED_STATE_TX_FIFO = 8,
  RAIL_ASSERT_FAILED_UNEXPECTED_STATE_TXACK_FIFO = 9,
  RAIL_ASSERT_FAILED_PBUFFER_NOT_DEFINED = 10,
  RAIL_ASSERT_FAILED_INSUFFICIENT_BYTES_IN_RX_PACKET = 11,
  RAIL_ASSERT_FAILED_CLOCK_PRESCALER = 12,
  RAIL_ASSERT_FAILED_RTCC_POST_WAKEUP = 13,
  RAIL_ASSERT_FAILED_SYNTH_VCO_FREQUENCY = 14,
  RAIL_ASSERT_FAILED_RAC_STATE = 15,
  RAIL_ASSERT_FAILED_RETIME_LIMIT = 16,
  RAIL_ASSERT_FAILED_NESTED_SEQUENCER_LOCK = 17,
  RAIL_ASSERT_FAILED_RSSI_AVERAGE_DONE = 18,
  RAIL_ASSERT_FAILED_DFL_BITS_SIZE = 19,
  RAIL_ASSERT_FAILED_PROTIMER_RANDOM_SEED = 20,
  RAIL_ASSERT_FAILED_EFR32XG1_REGISTER_SIZE = 21,
  RAIL_ASSERT_FAILED_PROTIMER_CHANNEL = 22,
  RAIL_ASSERT_FAILED_TIMER_REQUIRES_WRAP = 23,
  RAIL_ASSERT_FAILED_BASECNTTOP = 24,
  RAIL_ASSERT_FAILED_DEPRECATED_LBTRETRY = 25,
  RAIL_ASSERT_FAILED_RTCC_SYNC_MISSED = 26,
  RAIL_ASSERT_FAILED_CLOCK_SOURCE_NOT_READY = 27,
  RAIL_ASSERT_FAILED_TIMINGS_INVALID = 28,
  RAIL_ASSERT_NULL_HANDLE = 29,
  RAIL_ASSERT_FAILED_SCHED_TIMER_NOT_RUNNING = 30,
  RAIL_ASSERT_FAILED_NO_ACTIVE_CONFIG = 31,
  RAIL_ASSERT_FAILED_NO_ACTIVE_HANDLE_SWITCH = 32,
  RAIL_ASSERT_FAILED_RFINIT = 33,
  RAIL_ASSERT_FAILED_NO_ACTIVE_HANDLE_SCHEDRX = 34,
  RAIL_ASSERT_FAILED_INVALID_HANDLE_SCHEDTX = 35,
  RAIL_ASSERT_FAILED_INACTIVE_HANDLE_SCHEDTX = 36,
  RAIL_ASSERT_FAILED_CONFIG_INDEX_INVALID = 37,
  RAIL_ASSERT_FAILED_NO_ACTIVE_HANDLE_SINGLEPROTOCOL = 38,
  RAIL_ASSERT_DEPRECATED_FUNCTION = 39,
  RAIL_ASSERT_MULTIPROTOCOL_NO_EVENT = 40,
  RAIL_ASSERT_FAILED_INVALID_INTERRUPT_ENABLED = 41,
  RAIL_ASSERT_CONVERSION_CURVES_NOT_INITIALIZED = 42,
  RAIL_ASSERT_DIVISION_BY_ZERO = 43,
  RAIL_ASSERT_CANT_USE_HARDWARE = 44,
};

/// Use this define to create an array of error messages that map to the codes
/// in \ref RAIL_AssertErrorCodes_t. You can use these to print slightly more
/// detailed error strings related to a particular assert error code if desired.
/// For example, you could implement your assert failed callback as follows to
/// make use of this.
///
/// @code{.c}
/// void RAILCb_AssertFailed(RAIL_Handle_t railHandle, uint32_t errorCode)
/// {
///   static const char* railErrorMessages[] = RAIL_ASSERT_ERROR_MESSAGES;
///   const char *errorMessage = "Unknown";
///
///   // If this error code is within the range of known error messages then use
///   // the appropriate error message.
///   if (errorCode < (sizeof(railErrorMessages) / sizeof(char*))) {
///     errorMessage = railErrorMessages[errorCode];
///   }
///   printf(errorMessage);
///
///   // Reset the chip since an assert is a fatal error
///   NVIC_SystemReset();
/// }
/// @endcode
///
#define RAIL_ASSERT_ERROR_MESSAGES {                                           \
    /* 0*/ "Appended info missing from Rx packet",                             \
    /* 1*/ "Payload bytes missing from Rx packet",                             \
    /* 2*/ "Error reading back packet payload",                                \
    /* 3*/ "Receive fifo entry has invalid status",                            \
    /* 4*/ "Receive fifo entry bad packet length",                             \
    /* 5*/ "Unable to configure radio for IR calibration",                     \
    /* 6*/ "Reached unexpected state while handling Rx fifo events",           \
    /* 7*/ "Reached unexpected state while handling RXLEN fifo events",        \
    /* 8*/ "Reached unexpected state while handling Tx fifo events",           \
    /* 9*/ "Reached unexpected state while handling Tx ACK fifo events",       \
    /*10*/ "No memory to store receive packet",                                \
    /*11*/ "Packet length longer than the receive FIFO size",                  \
    /*12*/ "Invalid radio clock prescaler",                                    \
    /*13*/ "Error synchronizing the RAIL timebase after sleep",                \
    /*14*/ "VCO frequency outside supported range",                            \
    /*15*/ "Radio active while changing channels",                             \
    /*16*/ "Unable to configure DCDC retiming",                                \
    /*17*/ "Nested attempt to lock the sequencer",                             \
    /*18*/ "RSSI averaging enabled without a valid callback",                  \
    /*19*/ "Invalid dynamic frame length setting provided (dflBits)",          \
    /*20*/ "Unable to seed radio pseudo random number generator",              \
    /*21*/ "Timeout exceeds EFR32XG1 register size",                           \
    /*22*/ "Invalid timer channel specified",                                  \
    /*23*/ "Timer value larger than RAIL timebase",                            \
    /*24*/ "LBT config exceeds EFR32XG1 register size",                        \
    /*25*/ "Deprecated CSMA/LBT retry callback unexpectedly called",           \
    /*26*/ "Could not synchronize RAIL timebase with the RTC",                 \
    /*27*/ "Clock source not ready",                                           \
    /*28*/ "Attempted to set RAIL timings to invalid value",                   \
    /*29*/ "NULL was supplied as a RAIL_Handle_t argument",                    \
    /*30*/ "Scheduled timer not running",                                      \
    /*31*/ "No active config to switch from",                                  \
    /*32*/ "No active handle after switch",                                    \
    /*33*/ "RfInit failed to configure active state",                          \
    /*34*/ "No active handle for scheduled rx",                                \
    /*35*/ "Invalid handle for scheduled tx",                                  \
    /*36*/ "Inactive handle for scheduled tx",                                 \
    /*37*/ "Invalid config index to switch to",                                \
    /*38*/ "No active handle for single protocol",                             \
    /*39*/ "This function is deprecated and must not be called",               \
    /*40*/ "Multiprotocol task started with no event to run",                  \
    /*41*/ "Invalid interrupt enabled",                                        \
    /*42*/ "Power conversion functions called before curves were initialized", \
    /*43*/ "Division by zero",                                                 \
    /*44*/ "Function cannot be called without access to the hardware",         \
}

/**
 * @}
 */

#endif  // __RAIL_ASSERT_ERROR_CODES_H__
