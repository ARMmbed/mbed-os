/**************************************************************************//**
 * @file rail.h
 * @brief The main header file for the RAIL library. It describes the external
 *        APIs available to a RAIL user
 * @copyright Copyright 2015 Silicon Laboratories, Inc. www.silabs.com
 *****************************************************************************/

#ifndef __RAIL_H__
#define __RAIL_H__

// Get the standard include types
#include <stdint.h>
#include <stdbool.h>
#include <string.h> // For memcpy()

// Get the RAIL-specific structures and types
#include "rail_chip_specific.h"
#include "rail_types.h"
#include "rail_assert_error_codes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup RAIL_API RAIL API
 * @brief This is the primary API layer for the Radio Abstraction Interface
 *   Layer (RAIL)
 * @{
 */

/**
 * @defgroup Protocol_Specific Protocol Specific
 * @brief Protocol specific RAIL APIs
 */

/******************************************************************************
 * General Radio Operation
 *****************************************************************************/
/**
 * @addtogroup General
 * @brief Basic APIs to set up and interact with the RAIL library
 * @{
 */

/**
 * Gets the version information for the compiled RAIL library.
 *
 * @param[out] version A pointer to \ref RAIL_Version_t structure to
 *   populate with version information.
 * @param[in] verbose Populate \ref RAIL_Version_t struct with verbose
 *   information.
 * @return void.
 *
 * The version information contains a major version number, a minor version
 * number, and a rev (revision) number.
 */
void RAIL_GetVersion(RAIL_Version_t *version, bool verbose);

/**
 * Initializes RAIL.
 *
 * @param[in,out] railCfg The configuration and state structure for setting up
 *   the library, which contains memory and other options needed by RAIL.
 *   This structure must be allocated in application global read-write
 *   memory. RAIL may modify fields within or referenced by this structure
 *   during its operation.
 * @param[in] cb A callback that notifies the application when the radio is
 *   finished initializing and is ready for further configuration. This
 *   callback is useful for potential transceiver products that require a
 *   power up sequence before further configuration is available. After the
 *   callback fires, the radio is ready for additional configuration before
 *   transmit and receive operations.
 * @return Handle for initialized rail instance or NULL if an
 *   invalid value was passed in the railCfg.
 */
RAIL_Handle_t RAIL_Init(RAIL_Config_t *railCfg,
                        RAIL_InitCompleteCallbackPtr_t cb);

/**
 * Collects entropy from the radio if available.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[out] buffer The buffer to write the collected entropy.
 * @param[in] bytes The number of bytes to fill in in the input buffer.
 * @return Returns the number of bytes of entropy collected. For
 *   chips that don't support entropy collection, the function returns 0.
 *   Values less than the requested amount may also be returned on platforms
 *   that use entropy pools to collect random data periodically.
 *
 * Attempts to fill the provided buffer with the requested number of bytes of
 * entropy. If the requested number of bytes can't be provided, as many
 * bytes as possible will be filled and returned. For chips
 * that do not support this function, 0 bytes will always be returned. For
 * information about the specific mechanism for gathering entropy, see
 * documentation for the chip family.
 */
uint16_t RAIL_GetRadioEntropy(RAIL_Handle_t railHandle,
                              uint8_t *buffer,
                              uint16_t bytes);

/** @} */ // end of group General

/******************************************************************************
 * PTI
 *****************************************************************************/
/**
 * @addtogroup PTI Packet Trace (PTI)
 * @brief Basic APIs to set up and interact with PTI settings
 * @{
 */

/**
 * Configures PTI pin locations, serial protocols, and baud rates.
 *
 * @param[in] railHandle A RAIL instance handle (currently not used).
 * @param[in] ptiConfig A configuration structure applied to the
 *   relevant PTI registers.
 * @return Status code indicating success of the function call.
 *
 * This method must be called before RAIL_EnablePti() is called.
 * Although we do take a RAIL handle for potential future
 * expansion of this function, it is currently not used. That is,
 * there is only one PTI configuration that can be active on a
 * chip, regardless of the number of protocols (unless the application
 * takes responsibility to update the configuration upon a protocol switch),
 * and the configuration is not saved in your RAIL instance. For optimal
 * future compatibility, pass in a chip specific handle, such as
 * \ref RAIL_EFR32_HANDLE for now.
 */
RAIL_Status_t RAIL_ConfigPti(RAIL_Handle_t railHandle,
                             const RAIL_PtiConfig_t *ptiConfig);

/**
 * Gets the currently active PTI configuration.
 *
 * @param[in] railHandle A RAIL instance handle (currently not used).
 * @param[out] ptiConfig A configuration structure filled with the active
 *   PTI configuration.
 * @return RAIL status indicating success of the function call.
 *
 * Although most combinations of configurations can be set, it is safest
 * to call this method after configuration to confirm which values were
 * actually set. As in RAIL_ConfigPti, railHandle is not used. This function
 * will always return the single active PTI configuration regardless of the
 * active protocol. For optimal future compatibility, pass in a chip
 * specific handle, such as \ref RAIL_EFR32_HANDLE for now.
 */
RAIL_Status_t RAIL_GetPtiConfig(RAIL_Handle_t railHandle,
                                RAIL_PtiConfig_t *ptiConfig);

/**
 * Enables the PTI output of the packet data.
 *
 * @param[in] railHandle A RAIL instance handle (currently not used).
 * @param[in] enable PTI is enabled if true; disable if false.
 * @return RAIL status indicating success of the function call.
 *
 * Similarly to how there is only one PTI configuration per chip,
 * PTI can only be enabled or disabled for all protocols. It cannot
 * be individually set to enabled and disabled per protocol
 * (unless the application takes the responsibility of switching it when
 * the protocol switches), and enable/disable is not saved as part of your
 * RAIL instance. For optimal future compatibility, pass in a chip
 * specific handle, such as \ref RAIL_EFR32_HANDLE for now.
 */
RAIL_Status_t RAIL_EnablePti(RAIL_Handle_t railHandle,
                             bool enable);

/**
 * Sets a protocol that RAIL outputs on PTI.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] protocol The enum representing which protocol the node is using.
 * @return Status code indicating success of the function call.
 *
 * The protocol is output via the Packet Trace Interface (PTI) for each packet.
 * Before any protocol is set, the default value is \ref
 * RAIL_PTI_PROTOCOL_CUSTOM. One of the enum values should be used so that
 * the Network Analyzer can decode the packet.
 */
RAIL_Status_t RAIL_SetPtiProtocol(RAIL_Handle_t railHandle,
                                  RAIL_PtiProtocol_t protocol);

/** @} */ // end of group PTI

/******************************************************************************
 * Antenna Control
 *****************************************************************************/
/**
 * @addtogroup Antenna_Control
 * @brief Basic APIs to control the Antenna functionality
 * @{
 */

/**
 * Configures Antenna pin locations
 * @param[in] railHandle a RAIL instance handle.
 * @param[in] config A configuration structure applied to the relevant Antenna
 *   Configuration registers.
 * @return Status code indicating success of the function call.
 *
 * Although we do take a RAIL handle for potential future expansion, it is
 * currently not used.
 *
 */
RAIL_Status_t RAIL_ConfigAntenna(RAIL_Handle_t railHandle,
                                 const RAIL_AntennaConfig_t *config);

/** @} */ // end of group Antenna_Control

/******************************************************************************
 * Radio Configuration
 *****************************************************************************/
/// @addtogroup Radio_Configuration Radio Configuration
/// @brief Routines for setting up and querying radio configuration information.
///
/// These routines allow for runtime flexibility in the radio
/// configuration. Some of the parameters, however, are meant to be generated
/// from the radio calculator in Simplicity Studio. The basic code to configure
/// the radio from this calculator output looks like the example below.
///
/// @code{.c}
/// // Associate a specific channel config with a particular RAIL instance, and
/// // load the settings that correspond to the first usable channel.
/// RAIL_ConfigChannels(railHandle, channelConfigs[0]);
/// @endcode
///
/// For more information about the types of parameters that can be changed in
/// the other functions and how to use them, see their individual documentation.
///
/// @{

/**
 * Loads a static radio configuration.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] config A pointer to a radio configuration.
 * @return Status code indicating success of the function call.
 *
 * The configuration passed into this function should be generated for you
 * and not manually created or edited. By default this function should not be
 * called in RAIL 2.x and later unless a non-default radio configuration needs
 * to be applied. In RAIL 2.x and later, the RAIL_ConfigChannels function
 * applies the default radio configuration automatically.
 */
RAIL_Status_t RAIL_ConfigRadio(RAIL_Handle_t railHandle,
                               RAIL_RadioConfig_t config);

/**
 * Modifies the currently configured fixed frame length in bytes.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] length The expected fixed frame length. A value of 0 is infinite.
 *   A value of RAIL_SETFIXEDLENGTH_INVALID restores the frame's length back to
 *   that length specified by the default frame type configuration.
 * @return Length configured; The new frame length configured into the hardware
 *   for use. 0 if in infinite mode, or RAIL_SETFIXEDLENGTH_INVALID if the frame
 *   length has not yet been overridden by a valid value.
 *
 * Sets the fixed-length configuration for transmit and receive.
 * Be careful when using this function in receive and transmit as this
 * function changes the default frame configuration and remains in force until
 * it is called again with an input value of RAIL_SETFIXEDLENGTH_INVALID. This
 * function will override any fixed or variable length settings from a radio
 * configuration.
 */
uint16_t RAIL_SetFixedLength(RAIL_Handle_t railHandle, uint16_t length);

/**
 * Configures the channels supported by this device.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] config A pointer to the channel configuration for your device.
 *   This pointer will be cached in the library so it must be something that
 *   will exist for the runtime of the application. Typically, this should be
 *   what is stored in Flash by the configuration tool.
 * @param[in] cb Function called whenever a radio configuration change occurs.
 * @return Returns the first available channel in the configuration.
 *
 * When configuring channels on the EFR32, the radio tuner is reconfigured
 * based on the frequency and channel spacing in the channel configuration.
 */
uint16_t RAIL_ConfigChannels(RAIL_Handle_t railHandle,
                             const RAIL_ChannelConfig_t *config,
                             RAIL_RadioConfigChangedCallback_t cb);

/**
 * Checks to see if the channel exists in RAIL.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] channel A channel number to check.
 * @return Returns RAIL_STATUS_NO_ERROR if channel exists
 *
 * Returns RAIL_STATUS_INVALID_PARAMETER if the given channel does not exist
 * in the channel configuration currently used, and RAIL_STATUS_NO_ERROR if the
 * channel is valid.
 */
RAIL_Status_t RAIL_IsValidChannel(RAIL_Handle_t railHandle,
                                  uint16_t channel);

/**
 * Returns the symbol rate for the current PHY.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return The symbol rate in symbols per second or 0.
 *
 * The symbol rate is the rate of symbol changes over the air. For non-DSSS
 * PHYs, this is the same as the baudrate. For DSSS PHYs, it is the baudrate
 * divided by the length of a chipping sequence. For more information,
 * see the modem calculator documentation. If the rate is unable to be
 * calculated, this function will return 0.
 */
uint32_t RAIL_GetSymbolRate(RAIL_Handle_t railHandle);

/**
 * Returns the bit rate for the current PHY.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return The bit rate in bits per second or 0.
 *
 * The bit rate is the effective over-the-air data rate. It does not account
 * for extra spreading for forward error correction, and so on, but
 * accounts for modulation schemes, DSSS, and other configurations. For more
 * information, see the modem calculator documentation. If the rate is unable
 * to be calculated, this function will return 0.
 */
uint32_t RAIL_GetBitRate(RAIL_Handle_t railHandle);

/**
 * Sets the PA capacitor tune value for transmit and receive.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] txPaCtuneValue PA Ctune value for TX mode.
 * @param[in] rxPaCtuneValue PA Ctune value for RX mode.
 * @return Status code indicating success of the function call.
 *
 * Provides the ability to tune the impedance of the transmit
 * and receive modes by changing the amount of capacitance at
 * the PA output.
 */
RAIL_Status_t RAIL_SetPaCTune(RAIL_Handle_t railHandle,
                              uint8_t txPaCtuneValue,
                              uint8_t rxPaCtuneValue);

/** @} */ // end of group Radio_Configuration

/******************************************************************************
 * Timing Information
 *****************************************************************************/
/**
 * @addtogroup System_Timing System Timing
 * @brief Functionality related to the RAIL timer and general system time.
 *
 * These functions can be used to get information about the current system time
 * or to manipulate the RAIL timer.
 *
 * The system time returned by RAIL_GetTime() is in the same timebase that is
 * used throughout RAIL. Any callbacks that return a timestamp, such as
 * rxPacketReceived() callback, will use this same timebase as will any APIs
 * that accept an absolute time for scheduling their action. Throughout this
 * documentation the timebase used for this will be referred to as the RAIL
 * timebase. This is currently a value in microseconds from chip boot time.
 * This means that it will wrap every 1.19 hours.
 * (`(2^32 - 1) / (3600 sec/hr * 1000000 us/sec)`).
 *
 * The provided timer is hardware backed and interrupt driven. It can be used
 * for timing any event in your system, but will be especially helpful for
 * timing protocol based state machines and other systems that interact with
 * the radio. If you do not want to process the expiration in interrupt
 * context, leave the timerExpired() callback empty and poll for expiration
 * with the RAIL_IsTimerExpired() function.
 *
 * @{
 */

/**
 * Configure the RAIL timer plugin.
 *
 * @param[in] enable Enables/Disables the RAIL multitimer.
 * @return True if the multitimer was successfully enabled/disabled, false
 *   otherwise.
 *
 * @note This function must be called before calling \ref RAIL_SetMultiTimer.
 *       This function must be called to use the timer driver plugin
 *       with the RAIL single protocol library.
 *       This function should not be called while the RAIL timer is running.
 *       Call \ref RAIL_IsTimerRunning before enabling/disabling the multitimer.
 *       If the multitimer is not needed, do not call this function.
 *       This will allow the multitimer code to be dead stripped.
 */
bool RAIL_ConfigMultiTimer(bool enable);

/**
 * Gets the current RAIL time.
 *
 * @return Returns the RAIL timebase in microseconds. Note that this wraps
 *   after around 1.19 hours since it's stored in a 32bit value.
 *
 * Returns the current time in the RAIL timebase (microseconds). It can be
 * used to compare with packet timestamps or to schedule transmits.
 */
RAIL_Time_t RAIL_GetTime(void);

/**
 * Sets the current RAIL time.
 *
 * @param[in] time Set the RAIL timebase to this value in microseconds.
 * @return Status code indicating success of the function call.
 *
 * Sets the current time in the RAIL timebase in microseconds.
 */
RAIL_Status_t RAIL_SetTime(RAIL_Time_t time);

/**
 * Schedules a timer to expire using the RAIL timebase.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] time The timer's expiration time in the RAIL timebase.
 * @param[in] mode Indicates whether the time argument is an absolute
 *   RAIL time or relative to the current RAIL time. Specifying mode
 *   \ref RAIL_TIME_DISABLED is the same as calling RAIL_CancelTimer().
 * @param[in] cb The callback for RAIL to call when the timer expires.
 * @return RAIL_STATUS_NO_ERROR on success and
 *   RAIL_STATUS_INVALID_PARAMETER if the timer could not be scheduled.
 *
 * Configures a timer to expire after some period in the RAIL timebase.
 * This timer can be used to implement low level protocol features.
 *
 * @warning It is an error to attempt to schedule the timer when it is
 *   still running from a previous request -- unless the cb callback is
 *   identical to that used in the previous request, in which case the
 *   timer is rescheduled to the new time. Note that in this case if
 *   the original timer expires as it is being rescheduled, the callback
 *   may or may not occur. It is generally good practice to cancel a
 *   running timer before rescheduling it to minimize such ambiguity.
 */
RAIL_Status_t RAIL_SetTimer(RAIL_Handle_t railHandle,
                            RAIL_Time_t time,
                            RAIL_TimeMode_t mode,
                            RAIL_TimerCallback_t cb);

/**
 * Returns the absolute time that the RAIL timer was configured to expire.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return The absolute time that this timer was set to expire.
 *
 * It will give the absolute time regardless of the \ref RAIL_TimeMode_t that
 * was passed into \ref RAIL_SetTimer. Note that this time might be in the
 * past if the timer already expired. The return value is undefined if the
 * timer was never set.
 */
RAIL_Time_t RAIL_GetTimer(RAIL_Handle_t railHandle);

/**
 * Stops the currently scheduled RAIL timer.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return void.
 *
 * Cancels the timer. If this function is called before the timer expires,
 * the cb callback specified in the earlier RAIL_SetTimer() call will never
 * be called.
 */
void RAIL_CancelTimer(RAIL_Handle_t railHandle);

/**
 * Checks whether the RAIL timer has expired.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return True if the previously scheduled timer has expired and false
 *   otherwise.
 *
 * Polling via this function can be used as an alternative to the callback.
 */
bool RAIL_IsTimerExpired(RAIL_Handle_t railHandle);

/**
 * Checks whether the RAIL timer is currently running.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return Returns true if the timer is running and false if
 *   the timer has expired or was never set.
 */
bool RAIL_IsTimerRunning(RAIL_Handle_t railHandle);

/**
 * Starts a multitimer instance.
 *
 * @note
 *    It is legal to start an already running timer. If this is done, the timer
 *    will first be stopped before the new configurations are applied.
 *    If expirationTime is 0, the callback will be called
 *    immediately.
 *
 * @param[in,out] tmr A pointer to the timer instance to start.
 * @param[in] expirationTime When the timer is to expire.
 * @param[in] expirationMode Select mode of expirationTime. See \ref
 *            RAIL_TimeMode_t.
 * @param[in] callback Function to call on timer expiry. See \ref
 *            RAIL_MultiTimerCallback_t. NULL is a legal value.
 * @param[in] cbArg Extra callback function parameter for user application.
 *
 * @return
 *    \ref RAIL_STATUS_NO_ERROR on success.@n
 *    \ref RAIL_STATUS_INVALID_PARAMETER if tmr has an illegal value or if
 *         timeout is in the past.
 */
RAIL_Status_t RAIL_SetMultiTimer(RAIL_MultiTimer_t *tmr,
                                 RAIL_Time_t expirationTime,
                                 RAIL_TimeMode_t expirationMode,
                                 RAIL_MultiTimerCallback_t callback,
                                 void *cbArg);

/**
 * Stops the currently scheduled RAIL multi timer.
 *
 * @param[in,out] tmr A RAIL timer instance handle.
 *
 * @return
 *    true if timer was successfully cancelled.
 *    false if timer was not running.
 *
 * Cancels the timer. If this function is called before the timer expires,
 * the cb callback specified in the earlier RAIL_SetTimer() call will never
 * be called.
 */
bool RAIL_CancelMultiTimer(RAIL_MultiTimer_t *tmr);

/**
 * Check if a given timer is running.
 *
 * @param[in] tmr A pointer to the timer structure to query.
 *
 * @return
 *    true if timer is running.
 *    false if timer is not running.
 */
bool RAIL_IsMultiTimerRunning(RAIL_MultiTimer_t *tmr);

/**
 * Check if a given timer has expired.
 *
 * @param[in] tmr A pointer to the timer structure to query.
 *
 * @return
 *    true if timer is expired.
 *    false if timer is running.
 */
bool RAIL_IsMultiTimerExpired(RAIL_MultiTimer_t *tmr);

/**
 * Get time left before a given timer instance expires.
 *
 * @param[in] tmr A pointer to the timer structure to query.
 * @param[in] timeMode An indication as to how the function provides the time
 *            remaining. By choosing \ref
 *            RAIL_TimeMode_t::RAIL_TIME_ABSOLUTE, the function returns the
 *            absolute expiration time, and by choosing \ref
 *            RAIL_TimeMode_t::RAIL_TIME_DELAY, the function returns the
 *            amount of time remaining before the timer's expiration.
 *
 * @return
 *    Time left expressed in RAIL's time units.
 *    0 if the soft timer is not running or has already expired.
 */
RAIL_Time_t RAIL_GetMultiTimer(RAIL_MultiTimer_t *tmr,
                               RAIL_TimeMode_t timeMode);

/**
 * Initialize RAIL timer synchronization.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] sleepConfig A sleep configuration.
 *
 * @return Status code indicating success of the function call.
 */
RAIL_Status_t RAIL_ConfigSleep(RAIL_Handle_t railHandle,
                               RAIL_SleepConfig_t sleepConfig);

/**
 * Stop the RAIL timer and prepare RAIL for sleep.
 *
 * @param[in] wakeupProcessTime Time in microseconds that the application and
 *                              hardware need to recover from sleep state.
 * @param[out] deepSleepAllowed
 *   true - system can go to deep sleep.
 *   false - system should not go to deep sleep. Deep sleep should be blocked
 *           in this case.
 *
 * @return Status code indicating success of the function call.
 *
 * @warning The active RAIL configuration must be idle for enabling sleep.
 */
RAIL_Status_t RAIL_Sleep(uint16_t wakeupProcessTime, bool *deepSleepAllowed);

/**
 * Wake RAIL from sleep and restart the RAIL timer.
 *
 * @param[in] elapsedTime Add the sleep duration to the RAIL timer
 *   before restarting the RAIL timer.
 *
 * @return Status code indicating success of the function call.
 *
 * If timer sync was enabled by \ref RAIL_ConfigSleep, synchronize the RAIL
 * timer using an alternate timer. Otherwise, add elapsedTime to the RAIL
 * timer.
 */
RAIL_Status_t RAIL_Wake(RAIL_Time_t elapsedTime);

/** @} */ // end of group System_Timing

/******************************************************************************
 * Events
 *****************************************************************************/
/**
 * @addtogroup Events
 * @brief APIs related to events
 * @{
 */

/**
 * Configures radio events.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] mask Bitmask containing which events should be modified.
 * @param[in] events Define which events should trigger \ref RAIL_Config_t::eventsCallback
 *   The full list of available callbacks can be found by looking at the
 *   RAIL_EVENT_* set of defines.
 * @return Status code indicating success of the function call.
 *
 * Sets up which radio interrupts generate a RAIL event. The full list of
 * options is in \ref RAIL_Events_t.
 */
RAIL_Status_t RAIL_ConfigEvents(RAIL_Handle_t railHandle,
                                RAIL_Events_t mask,
                                RAIL_Events_t events);

/** @} */ // end of group Events

/******************************************************************************
 * Data Management
 *****************************************************************************/
/// @addtogroup Data_Management Data Management
/// @brief Data management functions
///
/// These functions allow the application to choose how data is presented to the
/// application. RAIL provides data in a packet-based method or in a FIFO-based
/// method which gives the application more granularity and responsibility in
/// managing transmit and receive data, and allows packet sizes larger than
/// the RX or TX FIFO buffers.
///
/// The application can configure RAIL data management through
/// RAIL_ConfigData().
/// This function allows the application to specify the type of radio data
/// (\ref RAIL_TxDataSource_t and \ref RAIL_RxDataSource_t) and the method of
/// interacting with data (\ref RAIL_DataMethod_t). By default, RAIL
/// configures TX and RX both with packet data source and packet mode.
///
/// In packet based data management:
///   - Packet lengths are determined from the Radio Configurator configuration
///     or after receive packet completion using RAIL_GetRxPacketInfo().
///   - Load transmit data with RAIL_WriteTxFifo().
///   - Received packet data is made available on successful packet completion
///     via \ref RAIL_Config_t::eventsCallback with \ref RAIL_EVENT_RX_PACKET_RECEIVED
///     which can then use RAIL_GetRxPacketInfo() and RAIL_GetRxPacketDetails() to
///     access packet information, and RAIL_PeekRxPacket() to access packet
///     data.
///   - Filtered, Aborted, or FrameError received packet data is automatically
///     dropped without the application needing to worry about consuming it.
///     The application can choose to not even be bothered with the events
///     related to such packets: \ref RAIL_EVENT_RX_ADDRESS_FILTERED,
///     \ref RAIL_EVENT_RX_PACKET_ABORTED, or \ref RAIL_EVENT_RX_FRAME_ERROR.
///
/// In FIFO based data management:
///   - Packet Lengths are determined from the Radio Configurator configuration
///     or by application knowledge of packet payload structure.
///   - Load transmit data with RAIL_WriteTxFifo() with reset set to false.
///   - Received data can be retrieved prior to packet completion through
///     RAIL_ReadRxFifo(), and is never dropped on Filtered, Aborted, or
///     FrameError packets. The application should enable and handle these
///     events so it can flush any packet data it's already retrieved.
///   - After packet completion, remaining packet data for Filtered, Aborted,
///     or FrameError packets can be either flushed automatically by RAIL
///     or consumed by the application just like a successfully received packet,
///     as determined from RAIL_GetRxPacketInfo(). RAIL_GetRxPacketDetails()
///     provides packet detailed information only for successfully received
///     packets.
///   - Set the TX FIFO threshold through RAIL_SetTxFifoThreshold(). The
///     \ref RAIL_Config_t::eventsCallback with \ref RAIL_EVENT_TX_FIFO_ALMOST_EMPTY
///     will occur telling the application to load more TX packet data, if needed
///     lest a \ref RAIL_EVENT_TX_UNDERFLOW event occurs.
///   - Set the RX FIFO threshold through RAIL_SetRxFifoThreshold(). The
///     \ref RAIL_Config_t::eventsCallback with \ref RAIL_EVENT_RX_FIFO_ALMOST_FULL
///     will occur telling the application to consume some RX packet data lest a
///     \ref RAIL_EVENT_RX_FIFO_OVERFLOW event occurs.
///   - Get RX FIFO count information through
///     RAIL_GetRxPacketInfo(\ref RAIL_RX_PACKET_HANDLE_NEWEST)
///     (or RAIL_GetRxFifoBytesAvailable()).
///   - Get TX FIFO count information through RAIL_GetTxFifoSpaceAvailable().
///   - Reset RX and/or TX FIFOs with RAIL_ResetFifo().
///
/// When trying to determine an appropriate threshold, the application needs
/// to know each FIFO's size. The receive FIFO is internal to RAIL and its
/// size is 512 bytes. The receive FIFO is level-based in that the \ref
/// RAIL_EVENT_RX_FIFO_ALMOST_FULL event will constantly pend if the threshold
/// is exceeded. This normally means that inside this event's callback, the
/// application should
/// empty enough of the FIFO to go under the threshold. To defer reading the
/// FIFO to main context, the application can disable or re-enable the receive
/// FIFO threshold event using RAIL_ConfigEvents() with the mask
/// \ref RAIL_EVENT_RX_FIFO_ALMOST_FULL.
///
/// The transmit FIFO is specified by the application and its actual size is
/// the value returned from the most recent call to RAIL_SetTxFifo(),
/// The transmit FIFO is edge-based in that it only provides the \ref
/// RAIL_EVENT_TX_FIFO_ALMOST_EMPTY event once when the threshold is crossed
/// in the emptying direction.
///
/// In FIFO mode, the FIFOs can store multiple packets. Depending on the
/// traffic, RAIL can receive multiple packets into the receive FIFO before the
/// application gets around to reading out the received data from the FIFO.
/// RAIL_ReadRxFifo() won't allow reading beyond a packet boundary so
/// process packet completion events promptly. Keep in mind that in FIFO mode,
/// packet data already read from packets that are subsequently aborted,
/// frameerror, or filtered should be flushed.
///
/// While RAIL defaults to packet mode, the application can explicitly
/// initialize RAIL for packet mode in the following manner:
/// @code{.c}
/// static const RAIL_DataConfig_t railDataConfig = {
///   .txSource = TX_PACKET_DATA,
///   .rxSource = RX_PACKET_DATA,
///   .txMethod = PACKET_MODE,
///   .rxMethod = PACKET_MODE,
/// };
///
/// status = RAIL_ConfigData(&railDataConfig);
///
/// // Events that can occur in Packet Mode:
///    RAIL_EVENT_TX_PACKET_SENT
///    RAIL_EVENT_RX_PACKET_RECEIVED
/// and optionally (packet data automatically dropped):
///    RAIL_EVENT_RX_ADDRESS_FILTERED
///    RAIL_EVENT_RX_PACKET_ABORTED
///    RAIL_EVENT_RX_FRAME_ERROR
/// @endcode
///
/// Initializing RAIL for FIFO Mode requires a few more function calls:
/// @code{.c}
/// static const RAIL_DataConfig_t railDataConfig = {
///   .txSource = TX_PACKET_DATA,
///   .rxSource = RX_PACKET_DATA,
///   .txMethod = FIFO_MODE,
///   .rxMethod = FIFO_MODE,
/// };
///
/// status = RAIL_ConfigData(&railDataConfig);
///
/// // Gets the size of the FIFOs.
/// // The transmit and receive FIFOs are the same size
/// uint16_t fifoSize = RAIL_GetTxFifoSpaceAvailable();
///
/// // Sets the transmit and receive FIFO thresholds.
/// // For this example, set the threshold in the middle of each FIFO
/// RAIL_SetRxFifoThreshold(fifoSize / 2);
/// RAIL_SetTxFifoThreshold(fifoSize / 2);
///
/// // Events that can occur in FIFO mode:
///    RAIL_EVENT_TX_FIFO_ALMOST_EMPTY
///    RAIL_EVENT_TX_UNDERFLOW
///    RAIL_EVENT_TXACK_UNDERFLOW
///    RAIL_EVENT_TX_PACKET_SENT
///    RAIL_EVENT_RX_FIFO_ALMOST_FULL
///    RAIL_EVENT_RX_FIFO_OVERFLOW
///    RAIL_EVENT_RX_ADDRESS_FILTERED
///    RAIL_EVENT_RX_PACKET_ABORTED
///    RAIL_EVENT_RX_FRAME_ERROR
///    RAIL_EVENT_RX_PACKET_RECEIVED
/// @endcode
///
/// On receive, an application can use multiple data sources that
/// are only compatible with the FIFO method of data delivery. All that differs
/// from the FIFO mode example above is the RAIL_DataConfig_t::rxSource setting.
/// IQ data samples are taken at the hardware's oversample rate and the amount
/// of data can easily overwhelm the CPU processing time. The sample rate
/// depends on the chosen PHY, as determined by the data rate and the decimation
/// chain. It is <b>not</b> recommended to use the IQ data source with sample
/// rates above 300 k samples/second as the CPU might not be able to keep up
/// with the data. Depending on the application and needed CPU bandwidth, slower
/// data rates may be required.
/// @code{.c}
/// // IQ data is provided into the receive FIFO
/// static const RAIL_DataConfig_t railDataConfig = {
///   .txSource = TX_PACKET_DATA,
///   .rxSource = RX_IQDATA_FILTLSB,
///   .txMethod = FIFO_MODE,
///   .rxMethod = FIFO_MODE,
/// };
///
/// // When reading IQ data out of the FIFO, it comes in the following format:
/// //------------------------------------
/// // I[LSB] | I[MSB] | Q[LSB] | Q[MSB] |
/// //------------------------------------
/// @endcode
///
/// @note \ref RAIL_DataConfig_t.txMethod and \ref RAIL_DataConfig_t.rxMethod
///   must have the same \ref RAIL_DataMethod_t configuration.
///
/// @warning Do not call RAIL_ReadRxFifo() function while in
/// \ref RAIL_DataMethod_t::PACKET_MODE.
/// @{

/**
 * RAIL data management configuration
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] dataConfig RAIL data configuration structure.
 * @return Status code indicating success of the function call.
 *
 * This function configures how RAIL manages data. The application can
 * configure RAIL to receive data in a packet-based or FIFO-based format.
 * FIFO mode is necessary to support packets larger than the radio's
 * FIFO buffers.
 *
 * With FIFO mode, the application sets appropriate FIFO thresholds via
 * RAIL_SetTxFifoThreshold() and RAIL_SetRxFifoThreshold(), and then
 * enables and handles the \ref RAIL_EVENT_TX_FIFO_ALMOST_EMPTY event
 * callback (to feed more packet data via RAIL_WriteTxFifo() before the
 * FIFO underflows), and the \ref RAIL_EVENT_RX_FIFO_ALMOST_FULL event
 * callback (to consume packet data via RAIL_ReadRxFifo() before the
 * RX FIFO overflows).
 *
 * When configuring TX or RX for FIFO mode, this function resets the configured
 * FIFOs. When configuring TX or RX for Packet mode, this function will reset
 * the corresponding FIFO thresholds such that they won't trigger the
 * \ref RAIL_EVENT_RX_FIFO_ALMOST_FULL or \ref RAIL_EVENT_TX_FIFO_ALMOST_EMPTY
 * events.
 *
 * When \ref RAIL_DataConfig_t.rxMethod is set to \ref
 * RAIL_DataMethod_t.FIFO_MODE, the radio won't drop packet data of
 * aborted or CRC error packets, but will present it to the application
 * to deal with accordingly. On completion of such erroneous packets, the
 * \ref RAIL_Config_t::eventsCallback with \ref RAIL_EVENT_RX_PACKET_ABORTED,
 * \ref RAIL_EVENT_RX_FRAME_ERROR, or \ref RAIL_EVENT_RX_ADDRESS_FILTERED will
 * tell the application it can drop any data it read via RAIL_ReadRxFifo() during reception.
 * For CRC error packets when the \ref RAIL_RX_OPTION_IGNORE_CRC_ERRORS
 * RX option is in effect, the application would check for that from the
 * \ref RAIL_RxPacketStatus_t obtained by calling RAIL_GetRxPacketInfo().
 * RAIL will automatically flush any remaining packet data after reporting
 * one of these packet completion events, or the application can explicitly
 * flush it by calling RAIL_ReleaseRxPacket().
 *
 * When \ref RAIL_DataConfig_t.rxMethod is set to \ref
 * RAIL_DataMethod_t.PACKET_MODE, the radio will drop all packet data
 * associated with aborted packets including those with CRC errors (unless
 * configured to ignore CRC errors via the
 * \ref RAIL_RX_OPTION_IGNORE_CRC_ERRORS RX option). The application will
 * never be bothered to deal with packet data from such packets.
 *
 * In either mode, the application can set RX options as needed, and
 * packet details are not available for aborted packets.
 */
RAIL_Status_t RAIL_ConfigData(RAIL_Handle_t railHandle,
                              const RAIL_DataConfig_t *dataConfig);

/**
 * Writes data to the transmit FIFO.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] dataPtr Application provided pointer to transmit data
 * @param[in] writeLength Number of bytes to write to the transmit FIFO
 * @param[in] reset If true resets TX FIFO before writing the data.
 * @return The number of bytes written to the transmit FIFO.
 *
 * This function reads data from the provided dataPtr and writes it to the TX
 * FIFO. If the requested writeLength exceeds the current number of bytes open
 * in the transmit FIFO, the function only writes until the transmit FIFO
 * is full. The function returns the number of bytes written to the transmit
 * FIFO, and returns zero if railHandle is NULL or if the TX FIFO is full.
 *
 * @note It is the protocol's packet configuration, as set up by the radio
 *   configurator or via RAIL_SetFixedLength(), that determines how many
 *   bytes of data are consumed from the TX FIFO for a successful transmit
 *   operation, not the writeLength value passed in. If not enough data has
 *   been put into the TX FIFO, a \ref RAIL_EVENT_TX_UNDERFLOW event will
 *   occur. If too much data, the extra data will either become the first bytes
 *   sent in a subsequent packet, or will be thrown away if the FIFO gets
 *   reset prior to the next transmit. In general, the proper number of
 *   packet bytes to put into the TX FIFO are all payload bytes except for
 *   any CRC bytes which the packet configuration would cause to be sent
 *   automatically.
 *
 * @note This function does not create a critical section but, depending on the
 *   application, a critical section could be appropriate.
 */
uint16_t RAIL_WriteTxFifo(RAIL_Handle_t railHandle,
                          const uint8_t *dataPtr,
                          uint16_t writeLength,
                          bool reset);

/**
 * Set the address of the TX FIFO, a circular buffer used for TX data
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in,out] addr Pointer to a read-write memory location in RAM
 *   used as the TX FIFO. This memory must persist until the next call to
 *   this function.
 * @param[in] initLength Number of initial bytes already in the TX FIFO.
 * @param[in] size Desired size of the TX FIFO in bytes.
 * @return Returns the FIFO size in bytes that has been set.
 *
 * This function is used to set the memory location for the TX FIFO. This
 * function must be called at least once before any transmit operations occur.
 *
 * The FIFO size can be determined by the return value of this function. The
 * chosen size is determined based on the available FIFO sizes supported by
 * the hardware. For more on supported FIFO sizes see chip-specific
 * documentation, such as \ref efr32_main. The returned FIFO size will be the
 * closest allowed size less than or equal to the passed in size parameter,
 * unless the size parameter is smaller than the minimum FIFO size. If the
 * initLength parameter is larger than the returned size, than the FIFO will be
 * full up to its size.
 *
 * User may write to the custom memory location directly before calling this
 * function, or use \ref RAIL_WriteTxFifo to write to the memory location after
 * calling this function. For previously-written memory to be set in the TX
 * FIFO, user must specify its initLength.
 *
 * This function reserves the block of RAM starting at txBufPtr with a length
 * of the returned FIFO size. That RAM block is used internally as a circular
 * buffer for the transmit FIFO. The FIFO must be able to hold the entire FIFO
 * size. The caller must guarantee the custom FIFO remains intact and unchanged
 * (except via calls to \ref RAIL_WriteTxFifo) until the next call to this
 * function.
 *
 * @note It is the protocol's packet configuration, as set up by the radio
 *   configurator or via RAIL_SetFixedLength(), that determines how many
 *   bytes of data are consumed from the TX FIFO for a successful transmit
 *   operation, not the initLength value passed in. If not enough data has
 *   been put into the TX FIFO, a \ref RAIL_EVENT_TX_UNDERFLOW event will
 *   occur. If too much data, the extra data will either become the first bytes
 *   sent in a subsequent packet, or will be thrown away if the FIFO gets
 *   reset prior to the next transmit. In general, the proper number of
 *   packet bytes to put into the TX FIFO are all payload bytes except for
 *   any CRC bytes which the packet configuration would cause to be sent
 *   automatically.
 */
uint16_t RAIL_SetTxFifo(RAIL_Handle_t railHandle,
                        uint8_t *addr,
                        uint16_t initLength,
                        uint16_t size);

/**
 * Reads packet data from RAIL's internal receive FIFO buffer.
 * This function can be used in any RX mode, though in Packet
 * mode it can only be used on the oldest unreleased packet whose
 * RAIL_RxPacketStatus_t is among the RAIL_RX_PACKET_READY_ set.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[out] dataPtr An application-provided pointer to store data.
 *   If NULL, the data is thrown away rather than copied out.
 * @param[in] readLength A number of packet bytes to read from the FIFO.
 * @return The number of packet bytes read from the receive FIFO.
 *
 * This function reads packet data from the head of receive FIFO and
 * writes it to the provided dataPtr. It does not permit reading more
 * data than is available in the FIFO, nor does it permit reading more
 * data than remains in the oldest unreleased packet.
 *
 * Because this function does not have a critical section, either use it
 * only in one context or make sure function calls are protected to prevent
 * buffer corruption.
 *
 * @note When reading data from an arriving packet that is not yet complete
 *   keep in mind its data is highly suspect because it has not yet passed
 *   any CRC integrity checking. Also note the packet could be aborted,
 *   cancelled, or fail momentarily, invalidating its data in Packet mode.
 *   Furthermore, there is a small chance towards the end of packet reception
 *   that the RX FIFO could include not only packet data received so far,
 *   but also some raw radio-appended info detail bytes that RAIL's
 *   packet-completion processing will subsequently deal with. It's up to the
 *   application to know its packet format well enough to avoid reading this
 *   info as it will corrupt the packet's details and possibly corrupt the
 *   RX FIFO buffer.
 */
uint16_t RAIL_ReadRxFifo(RAIL_Handle_t railHandle,
                         uint8_t *dataPtr,
                         uint16_t readLength);

/**
 * Configures the RAIL transmit FIFO almost empty threshold.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] txThreshold The threshold once fallen under will fire \ref RAIL_Config_t::eventsCallback
 *   with \ref RAIL_EVENT_TX_FIFO_ALMOST_EMPTY set.
 * @return Configured transmit FIFO threshold value.
 *
 * This function configures the threshold for the transmit FIFO. When the count
 * of the transmit FIFO is less than the configured threshold, \ref RAIL_Config_t::eventsCallback
 * will fire with \ref RAIL_EVENT_TX_FIFO_ALMOST_EMPTY set. A value of
 * 0 is invalid and will not change the current configuration.
 */
uint16_t RAIL_SetTxFifoThreshold(RAIL_Handle_t railHandle,
                                 uint16_t txThreshold);

/**
 * Configures the RAIL receive FIFO almost full threshold.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] rxThreshold The threshold once exceeded will fire \ref RAIL_Config_t::eventsCallback
 *   with \ref RAIL_EVENT_RX_FIFO_ALMOST_FULL set.
 * @return Configured receive FIFO threshold value.
 *
 * This function configures the threshold for the receive FIFO. When the count
 * of the receive FIFO is greater than the configured threshold,
 * \ref RAIL_Config_t::eventsCallback will fire with
 * \ref RAIL_EVENT_RX_FIFO_ALMOST_FULL set. A value of 0xFFFF is invalid and
 * will not change the current configuration. Depending on the size of the
 * receive FIFO hardware, the maximum value can vary. If the rxThreshold value
 * exceeds the capability of the hardware, the RX threshold will be configured
 * so that it fires only when the FIFO is one byte away from being full.
 */
uint16_t RAIL_SetRxFifoThreshold(RAIL_Handle_t railHandle,
                                 uint16_t rxThreshold);

/**
 * Gets the RAIL transmit FIFO almost empty threshold value.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return Configured TX Threshold value.
 *
 * Retrieves the configured TX threshold value.
 */
uint16_t RAIL_GetTxFifoThreshold(RAIL_Handle_t railHandle);

/**
 * Gets the RAIL receive FIFO almost full threshold value.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return Configured RX Threshold value.
 *
 * Retrieves the configured RX threshold value.
 */
uint16_t RAIL_GetRxFifoThreshold(RAIL_Handle_t railHandle);

/**
 * Resets the RAIL FIFOs.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] txFifo If true, reset the transmit FIFO.
 * @param[in] rxFifo If true, reset the receive FIFO.
 * @return void.
 *
 * This function can reset each FIFO. The application should not reset the RX
 * FIFO while receiving a frame.
 */
void RAIL_ResetFifo(RAIL_Handle_t railHandle, bool txFifo, bool rxFifo);

/**
 * Get the number of bytes available in the receive FIFO.
 * This function should only be used in RX FIFO mode; apps should
 * probably be using RAIL_GetRxPacketInfo() instead.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return Number of raw bytes in the receive FIFO.
 *
 * @note The number of bytes returned may not just reflect the current
 *   packet's data but could also include raw appended info bytes added
 *   after successful packet reception and bytes from subsequently received
 *   packets. It is up to the app to never try to consume more than the
 *   packet's actual data when using the value returned here in a subsequent
 *   call to RAIL_ReadRxFifo(), otherwise the Rx buffer will be corrupted.
 */
uint16_t RAIL_GetRxFifoBytesAvailable(RAIL_Handle_t railHandle);

/**
 * Gets the number of bytes open in the transmit FIFO.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return Number of bytes open in the transmit FIFO.
 *
 * Gets the number of bytes open in the transmit FIFO.
 */
uint16_t RAIL_GetTxFifoSpaceAvailable(RAIL_Handle_t railHandle);

/** @} */ // end of group Data_Management

/******************************************************************************
 * State Transitions
 *****************************************************************************/
/**
 * @addtogroup State_Transitions State Transitions
 * @{
 */

/**
 * Configures RAIL automatic state transitions after RX.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] transitions The state transitions to apply after reception.
 * @return Status code indicating success of the function call.
 *
 * This function fails if unsupported transitions are passed in or if the
 * radio is currently in the RX state. Success can transition to TX, RX, or
 * IDLE, while error can transition to RX or IDLE.
 */
RAIL_Status_t RAIL_SetRxTransitions(RAIL_Handle_t railHandle,
                                    const RAIL_StateTransitions_t *transitions);

/**
 * Configures RAIL automatic state transitions after TX.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] transitions The state transitions to apply after transmission.
 * @return Status code indicating a success of the function call.
 *
 * This function fails if unsupported transitions are passed in or if the
 * radio is currently in the TX state. Success and error can each transition
 * to RX or IDLE.
 */
RAIL_Status_t RAIL_SetTxTransitions(RAIL_Handle_t railHandle,
                                    const RAIL_StateTransitions_t *transitions);

/**
 * Configures RAIL automatic state transition timing.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in,out] timings The timings used to configure the RAIL state
 *   machine. This structure is overwritten with the actual times that were
 *   set, if an input timing is invalid.
 * @return Status code indicating a success of the function call.
 *
 * The timings given are close to the actual transition time. However,
 * a still uncharacterized software overhead occurs. Also, timings are not
 * always adhered to when using an automatic transition after an error, due to
 * the cleanup required to recover from the error.
 */
RAIL_Status_t RAIL_SetStateTiming(RAIL_Handle_t railHandle,
                                  RAIL_StateTiming_t *timings);

/**
 * Places the radio into an idle state.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] mode The method for shutting down the radio.
 * @param[in] wait Whether this function should wait for the radio to reach
 *   idle before returning.
 * @return void.
 *
 * This function is used to remove the radio from TX and RX states. How these
 * states are left is defined by the mode parameter.
 *
 * In multiprotocol, this API will also cause the radio to be yielded so that
 * other tasks can be run. See \ref rail_radio_scheduler_yield for more details.
 */
void RAIL_Idle(RAIL_Handle_t railHandle,
               RAIL_IdleMode_t mode,
               bool wait);

/**
 * Gets the current radio state.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return An enumeration for the current radio state.
 *
 * Returns the state of the radio as a bitmask containing:
 * \ref RAIL_RF_STATE_IDLE, \ref RAIL_RF_STATE_RX, \ref RAIL_RF_STATE_TX,
 * and \ref RAIL_RF_STATE_ACTIVE. \ref RAIL_RF_STATE_IDLE, \ref
 * RAIL_RF_STATE_RX, and \ref RAIL_RF_STATE_TX bits are mutually exclusive.
 * The radio can transition through intermediate states,
 * which are not reported but are instead bucketed into the state
 * being transitioned into. For example, when the transmitter is in the
 * process of shutting down, this function will return TX, as if the
 * shutdown process hadn't started yet.
 */
RAIL_RadioState_t RAIL_GetRadioState(RAIL_Handle_t railHandle);

/** @} */ // end of group State_Transitions

/******************************************************************************
 * Transmit
 *****************************************************************************/
/**
 * @addtogroup Transmit
 * @brief APIs related to transmitting data packets
 * @{
 */

/// @addtogroup PA Power Amplifier (PA)
/// @brief APIs for interacting with one of the on chip PAs.
///
/// These APIs let you configure the on chip PA to get the appropriate output
/// power.
///
/// There a few types of functions that are found here
///   1) Configuration functions: These functions set and get configuration
///      for the PA. In this case, "configuration" refers to a) indicating
///      which PA to use, b) the voltage supplied by your board to the PA,
///      and c) the ramp time over which to ramp the PA up to its full
///      power.
///   2) Power-setting functions: These functions consume the actual
///      values written to the PA registers, and write them appropriately.
///      These values are referred to as "(raw) power levels". The range of
///      acceptable values for these functions depends on which PA is
///      currently active. The higher the power level set, the higher
///      the dBm power actually output by the chip. However, the mapping
///      between dBm and these power levels can vary greatly between
///      modules/boards.
///   3) Conversion functions: These functions do the work of converting
///      between the "power levels" discussed previously and the actual
///      dBm values output by the chip. Continue reading for more details
///      on how to handle unit conversion.
///
/// The accuracy of the chip output power in dBm will vary from application to
/// to application. For some protocols or channels the protocol itself or
/// legal limitations will require applications to know exactly what power
/// they're transmitting at, in dBm. Other applications will not have
/// these restrictions, and users will simply find some power level(s)
/// that fit their criteria for the trade-off between radio range and
/// power savings, regardless of what dBm power that maps to.
///
/// In order to provide a solution that fits all these applications,
/// Silicon Labs has provided a great deal of flexibility in
/// \ref RAIL_ConvertRawToDbm and \ref RAIL_ConvertDbmToRaw, the two functions
/// that do the conversion between the dBm power and the raw power levels.
/// Those levels of customizability are outlined below
///  1) No customizability needed: for a given dBm value, the result
///     of RAIL_ConvertDbmToRaw provides an appropriate
///     raw power level that, when written to the registers via
///     RAIL_SetPowerLevel, causes the chip to actually output at that
///     dBm power. In this case, no action is needed by the user,
///     the WEAK versions of the conversion functions can be used,
///     and the default include paths in pa_conversions_efr32.h can
///     be used.
///  2) The mapping of power level to dBm is not good, but the
///     level of precision is sufficient: In pa_conversions_efr32.c
///     the WEAK versions of the conversion functions work by using
///     8-segment piecewise linear curves to convert between dBm
///     and power levels for PA's with hundreds of power levels
///     and simple mapping tables for use with PA's with only a few
///     levels. If this method is sufficiently precise, but the mapping
///     between power levels and dBm is wrong, Silicon Labs recommends
///     copying pa_curves_efr32.h into a new file, updating the segments
///     to form a better fit (_DCDC_CURVES or _VBAT_CURVES defines), and
///     then adding the RAIL_PA_CURVES define to your build with the path
///     to the new file.
///  3) A different level of precision is needed and the fit is bad:
///     If the piecewise-linear line segment fit is not appropriate for
///     your solution, the functions in pa_conversions_efr32.c can be
///     totally rewritten, as long as RAIL_ConvertDbmToRaw and
///     RAIL_ConvertRawToDbm have the same signatures. It is completely
///     acceptable to re-write these in a way that makes the
///     pa_curves_efr32.h and pa_curve_types_efr32.h files referenced in
///     pa_conversions_efr32.h unnecessary. Those files are needed solely
///     for the conversion methods that Silicon Labs provides.
///  4) dBm values are not necessary: If your application does not require
///     dBm values at all, Silicon Labs recommends overwriting
///     RAIL_ConvertDbmToRaw and RAIL_ConvertRawToDbm with smaller functions
///     (i.e. return 0 or whatever was input). These functions are called
///     from within the RAIL library, so they can never be deadstripped,
///     but making them as small as possible is the best way to reduce code
///     size. From there, you can simply call RAIL_SetTxPower, without
///     converting from a dBm value. If you never want the library to coerce the
///     power based on channels, RAIL_ConvertRawToDbm should be overwritten
///     to always return 0 and RAIL_ConvertDbmToRaw should be overwritten to
///     always return 255.
///
/// The following is example code on how to initialize your PA
/// @code{.c}
///
/// #include "pa_conversions_efr32.h"
///
/// // Helper macro to declare all the curve structures used by the Silicon Labs-provided
/// // conversion functions
/// RAIL_DECLARE_TX_POWER_VBAT_CURVES(piecewiseSegments, curvesSg, curves24Hp, curves24Lp);
///
/// // Put the variables declared above into the appropriate structure
/// RAIL_TxPowerCurvesConfig_t txPowerCurvesConfig = { curves24Hp, curvesSg, curves24Lp, piecewiseSegments };
///
/// // In the Silicon Labs implementation, the user is required to save those curves into
/// // to be referenced when the conversion functions are called
/// RAIL_InitTxPowerCurves(&txPowerCurvesConfig);
///
/// // Declare the structure used to configure the PA
/// RAIL_TxPowerConfig_t txPowerConfig = { RAIL_TX_POWER_MODE_2P4_HP, 3300, 10 };
///
/// // And then init the PA. Here, it is assumed that 'railHandle' is a valid RAIL_Handle_t
/// // that has already been initialized.
/// RAIL_ConfigTxPower(railHandle, &txPowerConfig);
///
/// // Pick a dBm power to use: 100 deci-dBm = 10 dBm. See docs on RAIL_TxPower_t
/// RAIL_TxPower_t power = 100;
///
/// // Get the config written by RAIL_ConfigTxPower to confirm what was actually set
/// RAIL_GetTxPowerConfig(railHandle, &txPowerConfig);
///
/// // RAIL_ConvertDbmToRaw will be the weak version provided by Silicon Labs
/// // by default, or the customer version, if overwritten.
/// RAIL_TxPowerLevel_t powerLevel = RAIL_ConvertDbmToRaw(railHandle,
///                                                       txPowerConfig.mode,
///                                                       power);
///
/// // Write the result of the conversion to the PA power registers in terms
/// // of raw power levels
/// RAIL_SetTxPower(railHandle, powerLevel);
/// @endcode
///
/// @note: all the lines following "RAIL_TxPower_t power = 100;" can be
/// replaced with the provided utility function, \ref RAIL_SetTxPowerDbm.
/// However, the full example here was provided for clarity. See the
/// documentation on \ref RAIL_SetTxPowerDbm for more details.
///
/// @{

/**
 * Initialize TxPower Settings
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] config Instance which contains desired initial settings
 *   for the TX amplifier.
 * @return RAIL_Status_t indicating success or an error.
 *
 * These settings include the selection between the multiple TX amplifiers,
 * voltage supplied to the TX power amplifier, and ramp times. This must
 * be called before any transmit occurs, or \ref RAIL_SetTxPower is called.
 * While we recommend always calling this function during initialization,
 * it can also be called anytime if these settings need to change to adapt
 * to a different application/protocol. This API will also reset TX Power to
 * its minimum value, so \ref RAIL_SetTxPower must be called after calling this.
 *
 * At times, certain combinations of configurations cannot be achieved.
 * This API attempts to get as close as possible to the requested settings. The
 * following "RAIL_Get..." API can be used to determine what values were set.
 */
RAIL_Status_t RAIL_ConfigTxPower(RAIL_Handle_t railHandle,
                                 const RAIL_TxPowerConfig_t *config);

/**
 * Get the TX power settings currently used in the amplifier
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[out] config Pointer to memory allocated to hold current TxPower
 *   configuration structure.
 * @return RAIL_TxPowerConfig_t RAIL status variable indicating whether
 *   or not the get was successful.
 *
 * Note, this API does not return the current TX power - that is separately
 * managed by the \ref RAIL_GetTxPower/\ref RAIL_SetTxPower API's. This API
 * should be used to know exactly which values were set as a result of
 * \ref RAIL_ConfigTxPower.
 */
RAIL_Status_t RAIL_GetTxPowerConfig(RAIL_Handle_t railHandle,
                                    RAIL_TxPowerConfig_t *config);

/**
 * Set the TX power in units of raw units (see \ref rail_chip_specific.h for
 * value ranges).
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] powerLevel Power in chip specific \ref RAIL_TxPowerLevel_t units.
 * @return RAIL_Status_t indicating success or an error.
 *
 * In order to convert between decibels and the integer values that the
 * registers take, call \ref RAIL_ConvertDbmToRaw. Silicon Labs provides
 * a weak version of this function which works well with our boards. However
 * a customer using his/her own custom board will want to characterize
 * chip operation on that board and override the function to do the conversion
 * appropriately from the desired dB values to raw integer values.
 *
 * Depending on the configuration used in \ref RAIL_ConfigTxPower, not all
 * power levels are achievable. This API will get as close as possible to
 * the desired power without exceeding it, and calling \ref RAIL_GetTxPower is
 * the only way to know the exact value written.
 *
 * Calling this function before configuring the PA (i.e. before a successful
 * call to \ref RAIL_ConfigTxPower) will cause an error to be returned.
 */
RAIL_Status_t RAIL_SetTxPower(RAIL_Handle_t railHandle,
                              RAIL_TxPowerLevel_t powerLevel);

/**
 * Returns the current power setting of the PA.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return The chip specific \ref RAIL_TxPowerLevel_t value of the current
 * transmit power.
 *
 * This API returns the raw value that was actually set by \ref RAIL_SetTxPower.
 * Silicon Labs provides a weak version of \ref RAIL_ConvertRawToDbm that works
 * with our boards to convert these raw values into actual output dBm values.
 * However, if a customer is using a custom board, we recommend that he/she re-
 * characterizes the relationship between raw and decibel values, and overrides
 * the provided function with one more that more accurately reflects the actual
 * relationship.
 *
 * Calling this function before configuring the PA (i.e. before a successful
 * call to \ref RAIL_ConfigTxPower) will cause an error to be returned
 * (RAIL_TX_POWER_LEVEL_INVALID).
 */
RAIL_TxPowerLevel_t RAIL_GetTxPower(RAIL_Handle_t railHandle);

/**
 * Converts raw values written to registers to decibel value (in units of
 * deci-dBm).
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] mode PA mode for which to do the conversion.
 * @param[in] powerLevel Raw amplifier register value to be converted to
 *   deci-dBm.
 * @return raw amplifier values converted to units of deci-dBm.
 *
 * A weak version of this function is provided by Silicon Labs that is tuned
 * to provide accurate values for our boards. If the customer intends to use
 * a custom board, the relationship between what is written to the Tx amplifier
 * and the actual output power should be re-characterized and implemented in an
 * overriding version of \ref RAIL_ConvertRawToDbm. For minimum code size and
 * best speed use only raw values with the TxPower API and override this
 * function with a smaller function. In the weak version provided with the RAIL
 * library, railHandle is only used to indicate to the user from where the
 * function was called, so it is OK to use either a real protocol handle, or one
 * of the chip specific ones, such as \ref RAIL_EFR32_HANDLE.
 *
 * Although the definitions of this function may change, the signature
 * must be as declared here.
 */
RAIL_TxPower_t RAIL_ConvertRawToDbm(RAIL_Handle_t railHandle,
                                    RAIL_TxPowerMode_t mode,
                                    RAIL_TxPowerLevel_t powerLevel);

/**
 * Converts the desired decibel value (in units of deci-dBm)
 * to raw integer values used by the TX amplifier registers.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] mode PA mode for which to do the conversion.
 * @param[in] power Desired dBm values in units of deci-dBm.
 * @return deci-dBm value converted to a raw
 *   integer value that can be used directly with \ref RAIL_SetTxPower.
 *
 * A weak version of this function is provided by Silicon Labs that is tuned
 * to provide accurate values for our boards. If the customer intends to use
 * a custom board, the relationship between what is written to the TX amplifier
 * and the actual output power should be characterized and implemented in an
 * overriding version of \ref RAIL_ConvertDbmToRaw. For minimum code size and
 * best speed use only raw values with the TxPower API and override this
 * function with a smaller function. In the weak version provided with the RAIL
 * library, railHandle is only used to indicate to the user from where the
 * function was called, so it is OK to use either a real protocol handle, or one
 * of the chip specific ones, such as \ref RAIL_EFR32_HANDLE.
 *
 * Although the definitions of this function may change, the signature
 * must be as declared here.
 *
 * @note This function is called from within the RAIL library for
 *   comparison between channel limitations and current power. It will
 *   throw an assert if you haven't called RAIL_InitTxPowerCurves
 *   which initializes the mappings between raw power levels and
 *   actual dBm powers. To avoid this assert, ensure that the
 *   maxPower of all channel config entries is \ref RAIL_TX_POWER_MAX
 *   or above, or override this function to always return 255.
 */
RAIL_TxPowerLevel_t RAIL_ConvertDbmToRaw(RAIL_Handle_t railHandle,
                                         RAIL_TxPowerMode_t mode,
                                         RAIL_TxPower_t power);

/** @} */ // end of group PA

/// Sets the TX power in terms of deci-dBm instead of raw power level.
///
/// @param[in] railHandle A RAIL instance handle.
/// @param[in] power Desired deci-dBm power to be set.
/// @return RAIL Status variable indicate whether setting the
///   power was successful.
///
/// This is a utility function crafted for user convenience. Normally, to set TX
/// power in dBm, the user would have to do the following:
///
/// @code{.c}
/// RAIL_TxPower_t power = 100; // 100 deci-dBm, 10 dBm
/// RAIL_TxPowerConfig_t txPowerConfig;
/// RAIL_GetTxPowerConfig(railHandle, &txPowerConfig);
/// // RAIL_ConvertDbmToRaw will be the weak version provided by Silicon Labs
/// // by default, or the customer version, if overwritten.
/// RAIL_TxPowerLevel_t powerLevel = RAIL_ConvertDbmToRaw(railHandle,
///                                                       txPowerConfig.mode,
///                                                       power);
/// RAIL_SetTxPower(railHandle, powerLevel);
/// @endcode
///
/// This function wraps all those calls in a single function with power passed in
/// as a parameter.
///
RAIL_Status_t RAIL_SetTxPowerDbm(RAIL_Handle_t railHandle,
                                 RAIL_TxPower_t power);

/// Gets the TX power in terms of deci-dBm instead of raw power level.
///
/// @param[in] railHandle A RAIL instance handle.
/// @return The current output power in deci-dBm
///
/// This is a utility function crafted for user convenience. Normally, to get TX
/// power in dBm, the user would have to do the following:
///
/// @code{.c}
/// RAIL_TxPowerLevel_t powerLevel = RAIL_GetTxPower(railHandle);
/// RAIL_TxPowerConfig_t txPowerConfig;
/// RAIL_GetTxPowerConfig(railHandle, &txPowerConfig);
/// // RAIL_ConvertRawToDbm will be the weak version provided by Silicon Labs
/// // by default, or the customer version, if overwritten.
/// RAIL_TxPower_t power = RAIL_ConvertRawToDbm(railHandle,
///                                             txPowerConfig.mode,
///                                             power);
/// return power;
/// @endcode
///
/// This function wraps all those calls in a single function with power returned
/// as the result.
///
RAIL_TxPower_t RAIL_GetTxPowerDbm(RAIL_Handle_t railHandle);

/**
 * Start a non-blocking transmit
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] channel Define the channel to transmit on.
 * @param[in] options TX options to be applied to this transmit only.
 * @param[in] schedulerInfo Information to allow the radio scheduler to place
 *   this transmit appropriately. This is only used in multiprotocol version of
 *   RAIL and may be set to NULL in all other versions.
 * @return Status code indicating success of the function call. If successfully
 *   initiated, transmit completion or failure will be reported by a later
 *   \ref RAIL_Config_t::eventsCallback with the appropriate \ref RAIL_Events_t.
 *
 * Will begin transmission of the payload previously loaded via
 * \ref RAIL_WriteTxFifo() immediately, or right after a packet currently being
 * received is completed.
 *
 * Returns an error if a previous transmit is still in progress.
 * If changing channels, any ongoing packet reception is aborted.
 *
 * In multiprotocol you must ensure that you properly yield the radio after this
 * operation completes. See \ref rail_radio_scheduler_yield for more details.
 */
RAIL_Status_t RAIL_StartTx(RAIL_Handle_t railHandle,
                           uint16_t channel,
                           RAIL_TxOptions_t options,
                           const RAIL_SchedulerInfo_t *schedulerInfo);

/**
 * Send a packet on a schedule, instead of immediately
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] channel Define the channel to transmit on.
 * @param[in] options TX options to be applied to this transmit only.
 * @param[in] config A pointer to the \ref RAIL_ScheduleTxConfig_t
 *   structure containing when the transmit should occur.
 * @param[in] schedulerInfo Information to allow the radio scheduler to place
 *   this transmit appropriately. This is only used in multiprotocol version of
 *   RAIL and may be set to NULL in all other versions.
 * @return Status code indicating success of the function call. If successfully
 *   initiated, a transmit completion or failure will be reported by a later
 *   \ref RAIL_Config_t::eventsCallback with the appropriate \ref RAIL_Events_t.
 *
 * Will begin transmission of the payload previously loaded via
 * \ref RAIL_WriteTxFifo() at the scheduled time.
 * The time (in microseconds) as well as whether that time is absolute or
 * relative, is specified using the \ref RAIL_ScheduleTxConfig_t structure.
 * Also specified in this structure is what to do if a scheduled transmit
 * fires in the midst of receiving a packet.
 *
 * Returns an error if a previous transmit is still in progress.
 * If changing channels, any ongoing packet reception is aborted.
 *
 * In multiprotocol you must ensure that you properly yield the radio after this
 * operation completes. See \ref rail_radio_scheduler_yield for more details.
 */
RAIL_Status_t RAIL_StartScheduledTx(RAIL_Handle_t railHandle,
                                    uint16_t channel,
                                    RAIL_TxOptions_t options,
                                    const RAIL_ScheduleTxConfig_t *config,
                                    const RAIL_SchedulerInfo_t *schedulerInfo);

/**
 * Start a non-blocking Transmit using CSMA
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] channel Define the channel to transmit on.
 * @param[in] options TX options to be applied to this transmit only.
 * @param[in] csmaConfig A pointer to the RAIL_CsmaConfig_t structure
 *   describing the CSMA parameters to use for this transmit.
 * @param[in] schedulerInfo Information to allow the radio scheduler to place
 *   this transmit appropriately. This is only used in multiprotocol version of
 *   RAIL and may be set to NULL in all other versions.
 * @return Status code indicating success of the function call. If successfully
 *   initiated, a transmit completion or failure will be reported by a later
 *   \ref RAIL_Config_t::eventsCallback with the appropriate \ref RAIL_Events_t.
 *
 * First performs the Carrier Sense Multiple Access (CSMA) algorithm and if
 * the channel is deemed clear (RSSI below the specified threshold) it will
 * commence transmission of the payload previously loaded via
 * RAIL_WriteTxFifo().
 * Packets can be received during CSMA backoff periods if receive is active
 * throughout the CSMA process. This will happen either by starting the CSMA
 * process while receive is already active, or if the csmaBackoff time in
 * the \ref RAIL_CsmaConfig_t is less than the idleToRx time (set by
 * RAIL_SetStateTiming()). If the csmaBackoff time is greater than the
 * idleToRx time, then receive will only be active during CSMA's clear channel
 * assessments.
 *
 * If the CSMA algorithm deems the channel busy, the \ref RAIL_Config_t::eventsCallback
 * occurs with \ref RAIL_EVENT_TX_CHANNEL_BUSY, and the contents
 * of the TX FIFO remain intact, untouched.
 *
 * Returns an error if a previous transmit is still in progress.
 * If changing channels, any ongoing packet reception is aborted.
 *
 * In multiprotocol you must ensure that you properly yield the radio after this
 * operation completes. See \ref rail_radio_scheduler_yield for more details.
 */
RAIL_Status_t RAIL_StartCcaCsmaTx(RAIL_Handle_t railHandle,
                                  uint16_t channel,
                                  RAIL_TxOptions_t options,
                                  const RAIL_CsmaConfig_t *csmaConfig,
                                  const RAIL_SchedulerInfo_t *schedulerInfo);

/**
 * Start a non-blocking Transmit using LBT
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] channel Define the channel to transmit on.
 * @param[in] options TX options to be applied to this transmit only.
 * @param[in] lbtConfig A pointer to the RAIL_LbtConfig_t structure
 *   describing the LBT parameters to use for this transmit.
 * @param[in] schedulerInfo Information to allow the radio scheduler to place
 *   this transmit appropriately. This is only used in multiprotocol version of
 *   RAIL and may be set to NULL in all other versions.
 * @return Status code indicating success of the function call. If successfully
 *   initiated, a transmit completion or failure will be reported by a later
 *   \ref RAIL_Config_t::eventsCallback with the appropriate \ref RAIL_Events_t.
 *
 * First performs the Listen Before Talk (LBT) algorithm and if the channel
 * is deemed clear (RSSI below the specified threshold) it will commence
 * transmission of the payload previously loaded via RAIL_WriteTxFifo().
 * Packets can be received during LBT backoff periods if receive is active
 * throughout the LBT process. This will happen either by starting the LBT
 * process while receive is already active, or if the lbtBackoff time in
 * the \ref RAIL_LbtConfig_t is less than the idleToRx time (set by
 * RAIL_SetStateTiming()). If the lbtBackoff time is greater than the
 * idleToRx time, then receive will only be active during LBT's clear channel
 * assessments.
 *
 * If the LBT algorithm deems the channel busy, the \ref RAIL_Config_t::eventsCallback occurs with
 * \ref RAIL_EVENT_TX_CHANNEL_BUSY, and the contents
 * of the TX FIFO remain intact, untouched.
 *
 * Returns an error if a previous transmit is still in progress.
 * If changing channels, any ongoing packet reception is aborted.
 *
 * In multiprotocol you must ensure that you properly yield the radio after this
 * operation completes. See \ref rail_radio_scheduler_yield for more details.
 */
RAIL_Status_t RAIL_StartCcaLbtTx(RAIL_Handle_t railHandle,
                                 uint16_t channel,
                                 RAIL_TxOptions_t options,
                                 const RAIL_LbtConfig_t *lbtConfig,
                                 const RAIL_SchedulerInfo_t *schedulerInfo);

/**
 * Sets the CCA threshold in dBm
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] ccaThresholdDbm The CCA threshold in dBm.
 * @return Status code indicating success of the function call.
 *
 * Unlike RAIL_StartCcaCsmaTx() or RAIL_StartCcaLbtTx(), which can cause a
 * transmit, this function only modifies the CCA threshold. A possible
 * use case for this function is to set the CCA threshold to invalid RSSI
 * of -128 which blocks transmission by preventing clear channel assessments
 * from succeeding.
 */
RAIL_Status_t RAIL_SetCcaThreshold(RAIL_Handle_t railHandle,
                                   int8_t ccaThresholdDbm);

/**
 * Gets detailed information about the last packet transmitted.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in,out] pPacketDetails An application-provided pointer to store
 *   RAIL_TxPacketDetails_t corresponding to the transmit event.
 *   The isAck and timeSent fields totalPacketBytes and timePosition
 *   must be initialized prior to each call:
 *   - isAck true to obtain details about the most recent ACK transmit,
 *     false to obtain details about the most recent app-initiated transmit.
 *   - totalPacketBytes with the total number of bytes of the transmitted
 *     packet for RAIL to use when calculating the specified timestamp.
 *     This should account for all bytes sent over the air after the
 *     Preamble and Sync word(s), including CRC bytes.
 *   - timePosition with a \ref RAIL_PacketTimePosition_t value specifying
 *     the packet position to put in the timeSent field on return.
 *     This field will also be updated with the actual position corresponding
 *     to the timeSent value filled in.
 * @return \ref RAIL_STATUS_NO_ERROR if pPacketDetails was filled in,
 *   or an appropriate error code otherwise.
 *
 * This function can only be called from callback context for either
 * \ref RAIL_EVENT_TX_PACKET_SENT or \ref RAIL_EVENT_TXACK_PACKET_SENT
 * events.
 */
RAIL_Status_t RAIL_GetTxPacketDetails(RAIL_Handle_t railHandle,
                                      RAIL_TxPacketDetails_t *pPacketDetails);
/**
 * Prevent the radio from starting a transmit.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] enable Enable/Disable TX hold off.
 * @return void.
 *
 * Enable TX hold off to prevent the radio from starting any transmits.
 * Disable TX hold off to allow the radio to transmit again.
 * Attempting to transmit with the TX hold off enabled will result in
 * \ref RAIL_EVENT_TX_BLOCKED and/or \ref RAIL_EVENT_TXACK_BLOCKED
 * events.
 *
 * @note This function does not affect a transmit that has already started.
 *   To stop an already-started transmission, use RAIL_Idle() with
 *   \ref RAIL_IDLE_ABORT.
 */
void RAIL_EnableTxHoldOff(RAIL_Handle_t railHandle, bool enable);

/**
 * Check whether or not TX hold off is enabled.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return Returns true if TX hold off is enabled, false otherwise.
 *
 * TX hold off can be enabled/disabled using \ref RAIL_EnableTxHoldOff.
 * Attempting to transmit with the TX hold off enabled will block the
 * transmission and result in \ref RAIL_EVENT_TX_BLOCKED
 * and/or \ref RAIL_EVENT_TXACK_BLOCKED events.
 */
bool RAIL_IsTxHoldOffEnabled(RAIL_Handle_t railHandle);

/** @} */ // end of group Transmit

/******************************************************************************
 * Receive
 *****************************************************************************/
/**
 * @addtogroup Receive
 * @brief APIs related to packet receive
 * @{
 */

/**
 * Configures receive options.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] mask A bitmask containing which options should be modified.
 * @param[in] options A bitmask containing desired configuration settings.
 *   Bit positions for each option are found in the \ref RAIL_RxOptions_t.
 * @return Status code indicating success of the function call.
 *
 * Configures the radio receive flow based on the list of available options.
 * Only the options indicated by the mask parameter will be affected. Pass
 * \ref RAIL_RX_OPTIONS_ALL to set all parameters.
 * The previous settings may affect the current frame if a packet is
 * received during this configuration.
 */
RAIL_Status_t RAIL_ConfigRxOptions(RAIL_Handle_t railHandle,
                                   RAIL_RxOptions_t mask,
                                   RAIL_RxOptions_t options);

/**
 * Start the receiver on a specific channel.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] channel The channel to listen on.
 * @param[in] schedulerInfo Information to allow the radio scheduler to place
 *   this receive appropriately. This is only used in multiprotocol version of
 *   RAIL and may be set to NULL in all other versions.
 * @return Status code indicating success of the function call.
 *
 * This is a non-blocking function. Whenever a packet is received \ref RAIL_Config_t::eventsCallback
 * will fire with \ref RAIL_EVENT_RX_PACKET_RECEIVED set. If you call
 * this while not idle but with a different channel we will abort any ongoing
 * receive or transmit operation.
 */
RAIL_Status_t RAIL_StartRx(RAIL_Handle_t railHandle,
                           uint16_t channel,
                           const RAIL_SchedulerInfo_t *schedulerInfo);

/**
 * Schedules a receive window for some future time.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] channel A channel to listen on.
 * @param[in] cfg The configuration structure to define the receive window.
 * @param[in] schedulerInfo Information to allow the radio scheduler to place
 *   this receive appropriately. This is only used in multiprotocol version of
 *   RAIL and may be set to NULL in all other versions.
 * @return Status code indicating success of the function call.
 *
 * This API immediately changes the channel and schedules receive to start
 * at the specified time and end at the given end time. If you do not specify
 * an end time, you may call this API later with an end time as long as you set
 * the start time to disabled. You can also terminate the receive
 * operation immediately using the RAIL_Idle() function. Note that relative
 * end times are always relative to the start unless no start time is
 * specified. If changing channels, aborts any ongoing packet transmission or
 * reception.
 *
 * In multiprotocol you must ensure that you properly yield the radio after this
 * call. See \ref rail_radio_scheduler_yield for more details.
 */
RAIL_Status_t RAIL_ScheduleRx(RAIL_Handle_t railHandle,
                              uint16_t channel,
                              const RAIL_ScheduleRxConfig_t *cfg,
                              const RAIL_SchedulerInfo_t *schedulerInfo);

/**
 * Get basic information about a pending or received packet.
 * This function can be used in any RX mode; it does not free up any
 * internal resources.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] packetHandle A packet handle for the unreleased packet as
 *   returned from a previous call, or sentinel values
 *   \ref RAIL_RX_PACKET_HANDLE_OLDEST or \ref RAIL_RX_PACKET_HANDLE_NEWEST.
 * @param[out] pPacketInfo An application-provided pointer to store
 *   \ref RAIL_RxPacketInfo_t for the requested packet.
 * @return The packet handle for the requested packet:
 *   if packetHandle was one of the sentinel values, returns the actual
 *   packet handle for that packet, otherwise returns packetHandle.
 *   It may return \ref RAIL_RX_PACKET_HANDLE_INVALID to indicate an error.
 *
 * @note When getting info about an arriving packet that is not yet complete
 *   (i.e. pPacketInfo->packetStatus == \ref RAIL_RX_PACKET_RECEIVING), keep
 *   in mind its data is highly suspect because it has not yet passed any CRC
 *   integrity checking. Also note the packet could be aborted, cancelled, or
 *   fail momentarily, invalidating its data in Packet mode. Furthermore, there
 *   is a small chance towards the end of packet reception that the filled-in
 *   RAIL_RxPacketInfo_t could include not only packet data received so far,
 *   but also some raw radio-appended info detail bytes that RAIL's
 *   packet-completion processing will subsequently deal with. It's up to the
 *   application to know its packet format well enough to avoid confusing such
 *   info as packet data.
 */
RAIL_RxPacketHandle_t RAIL_GetRxPacketInfo(RAIL_Handle_t railHandle,
                                           RAIL_RxPacketHandle_t packetHandle,
                                           RAIL_RxPacketInfo_t *pPacketInfo);

/**
 * Convenience helper function to copy a full packet to a user-specified
 * contiguous buffer.
 *
 * @param[out] pDest An application-provided pointer to a buffer of at
 *   least pPacketInfo->packetBytes in size to store the packet data
 *   contiguously. This buffer must never overlay RAIL's Rx FIFO buffer.
 *   Exactly pPacketInfo->packetBytes of packet data will be written into it.
 * @param[out] pPacketInfo
 *   \ref RAIL_RxPacketInfo_t for the requested packet.
 * @return void.
 *
 * @note This helper is intended to be expedient -- it does not check the
 *   validity of its arguments, so don't pass either as NULL, and don't
 *   pass a pDest pointer to a buffer that's too small for the packet's data.
 * @note If only a portion of the packet is needed, use RAIL_PeekRxPacket()
 *   instead.
 */
static inline
void RAIL_CopyRxPacket(uint8_t *pDest,
                       const RAIL_RxPacketInfo_t *pPacketInfo)
{
  memcpy(pDest, pPacketInfo->firstPortionData, pPacketInfo->firstPortionBytes);
  if (pPacketInfo->lastPortionData != NULL) {
    memcpy(pDest + pPacketInfo->firstPortionBytes,
           pPacketInfo->lastPortionData,
           pPacketInfo->packetBytes - pPacketInfo->firstPortionBytes);
  }
}

/**
 * Get detailed information about a ready packet received (one whose
 * \ref RAIL_RxPacketStatus_t is among the RAIL_RX_PACKET_READY_ set).
 * This function can be used in any RX mode; it does not free up any
 * internal resources.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] packetHandle A packet handle for the unreleased packet as
 *   returned from a previous call to RAIL_GetRxPacketInfo() or
 *   RAIL_HoldRxPacket(), or sentinel values \ref RAIL_RX_PACKET_HANDLE_OLDEST
 *   or \ref RAIL_RX_PACKET_HANDLE_NEWEST.
 * @param[in,out] pPacketDetails An application-provided pointer to store
 *   \ref RAIL_RxPacketDetails_t for the requested packet.
 *   The timeReceived fields totalPacketBytes and timePosition must be
 *   initialized prior to each call:
 *   - totalPacketBytes with the total number of bytes of the received
 *     packet for RAIL to use when calculating the specified timestamp.
 *     This should account for all bytes received over the air after the
 *     Preamble and Sync word(s), including CRC bytes.
 *   - timePosition with a \ref RAIL_PacketTimePosition_t value specifying
 *     the packet position to put in the timeReceived field on return.
 *     This field will also be updated with the actual position corresponding
 *     to the timeReceived value filled in.
 * @return \ref RAIL_STATUS_NO_ERROR if pPacketDetails was filled in,
 *   or an appropriate error code otherwise.
 */
RAIL_Status_t RAIL_GetRxPacketDetails(RAIL_Handle_t railHandle,
                                      RAIL_RxPacketHandle_t packetHandle,
                                      RAIL_RxPacketDetails_t *pPacketDetails);

/**
 * Place a temporary hold on this packet's data and information resources
 * within RAIL.
 * This shall only be called from within RAIL callback context.
 * This function can be used in any RX mode.
 *
 * Normally when RAIL issues its callback indicating a packet is ready
 * or aborted, it expects the application's callback to retrieve and
 * copy (or discard) the packet's information and data, and will free up
 * its internal packet data after the callback returns. This function
 * tells RAIL to hold onto those resources after the callback returns in
 * case the application wants to defer processing the packet to a later
 * time, e.g. outside of callback context.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return The packet handle for the packet associated with the callback,
 *   or \ref RAIL_RX_PACKET_HANDLE_INVALID if no such packet yet exists.
 */
RAIL_RxPacketHandle_t RAIL_HoldRxPacket(RAIL_Handle_t railHandle);

/**
 * Copies 'len' bytes of packet data starting from 'offset' from the
 * receive FIFO. Those bytes remain valid for re-peeking.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] packetHandle A packet handle as returned from a previous
 *   RAIL_GetRxPacketInfo() or RAIL_HoldRxPacket() call, or
 *   sentinel values \ref RAIL_RX_PACKET_HANDLE_OLDEST
 *   or \ref RAIL_RX_PACKET_HANDLE_NEWEST.
 * @param[out] pDst A pointer to the location where the received bytes will
 *   be copied. If NULL, no copying occurs.
 * @param[in] len A number of packet data bytes to copy.
 * @param[in] offset A byte offset within remaining packet data from which
 *   to copy.
 * @return Number of packet bytes copied.
 *
 * @note Peek does not permit peeking beyond the requested packet's
 *   available packet data (though there is a small chance it might
 *   for a \ref RAIL_RX_PACKET_HANDLE_NEWEST packet at the very end of
 *   still being received). Nor can one peek into already-consumed data read
 *   by RAIL_ReadRxFifo(). len and offset are relative to the remaining data
 *   available in the packet, if any was already consumed by RAIL_ReadRxFifo().
 */
uint16_t RAIL_PeekRxPacket(RAIL_Handle_t railHandle,
                           RAIL_RxPacketHandle_t packetHandle,
                           uint8_t *pDst,
                           uint16_t len,
                           uint16_t offset);

/**
 * Release RAIL's internal resources for the packet.
 * This must be called for any packet previously held via
 * RAIL_HoldRxPacket(), and may optionally be called within
 * callback context to release RAIL resources sooner than at
 * callback completion time when not holding the packet.
 * This function can be used in any RX mode.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] packetHandle A packet handle as returned from a previous
 *   RAIL_HoldRxPacket() call, or sentinel values
 *   \ref RAIL_RX_PACKET_HANDLE_OLDEST or \ref RAIL_RX_PACKET_HANDLE_NEWEST.
 *   The latter might be used within RAIL callback context to explicitly
 *   release the packet associated with the callback early, before it would
 *   be released automatically by RAIL on callback return (unless explicitly
 *   held).
 * @return \ref RAIL_STATUS_NO_ERROR if the held packet was released
 *   or an appropriate error code otherwise.
 */
RAIL_Status_t RAIL_ReleaseRxPacket(RAIL_Handle_t railHandle,
                                   RAIL_RxPacketHandle_t packetHandle);

/**
 * Returns the current raw RSSI.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] wait if false returns instant RSSI with no checks.
 * @return \ref RAIL_RSSI_INVALID if the receiver is disabled and an RSSI
 *   value can't be obtained. Otherwise, return the RSSI in quarter dBm, dbm*4.
 *
 * Gets the current RSSI value. This value represents the current energy of the
 * channel, it can change rapidly, and will be low if no RF energy is
 * in the current channel. The function from the value reported to dBm is an
 * offset dependent on the PHY and the PCB layout. Users should characterize the
 * RSSI received on their hardware and apply an offset in the application to
 * account for board and PHY parameters. 'Wait' argument doesn't guarantee
 * a valid RSSI reading.'Wait' being true gives assurance that that the RSSI is
 * current and not stale value from previous radio state. If GetRssi is called
 * during RX-to-RX, RX-to-IDLE or RX-to-TX transition the RSSI is considered
 * stale and \ref RAIL_RSSI_INVALID is returned if wait=true. 'Wait' being false
 * will return either current RSSI or stale RSSI measurement (if called during
 * RX-to-RX, RX-to-IDLE or RX-to-TX transition). \ref RAIL_RSSI_INVALID is
 * returned if radio hasn't been in RX longer than 'idleToRx' time
 * (see \ref RAIL_StateTiming_t), regardless of wait argument.
 *
 * In multiprotocol, this function returns \ref RAIL_RSSI_INVALID
 * immediately if railHandle is not the current active \ref RAIL_Handle_t.
 * Additionally 'wait' should never be set 'true' in multiprotocol
 * as the wait time is not consistent, so scheduling a scheduler
 * slot cannot be done accurately.
 */
int16_t RAIL_GetRssi(RAIL_Handle_t railHandle, bool wait);

/**
 * Starts the RSSI averaging over a specified time in us.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] channel The physical channel to set.
 * @param[in] averagingTimeUs Averaging time in microseconds.
 * @param[in] schedulerInfo Information to allow the radio scheduler to place
 *   this operation appropriately. This is only used in multiprotocol version of
 *   RAIL and may be set to NULL in all other versions.
 * @return Status code indicating success of the function call.
 *
 * Starts a non-blocking hardware-based RSSI averaging mechanism. Only a single
 * instance of RSSI averaging can be run at any time and the radio must be idle
 * to start.
 *
 * In multiprotocol, this is a scheduled event. It will start when railHandle
 * becomes active, and railHandle will need to stay active until the averaging
 * completes. If the averaging is interrupted, calls to
 * \ref RAIL_GetAverageRssi will return \ref RAIL_RSSI_INVALID.
 *
 * Also in multiprotocol, the user is required to call \ref RAIL_YieldRadio
 * after this event completes (i.e. when \ref RAIL_EVENT_RSSI_AVERAGE_DONE
 * occurs).
 */
RAIL_Status_t RAIL_StartAverageRssi(RAIL_Handle_t railHandle,
                                    uint16_t channel,
                                    RAIL_Time_t averagingTimeUs,
                                    const RAIL_SchedulerInfo_t *schedulerInfo);

/**
 * Queries whether the RSSI averaging is done.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return Returns true if done and false otherwise.
 *
 * This function can be used to poll for completion of the RSSI averaging
 * to avoid relying on an interrupt-based callback.
 */
bool RAIL_IsAverageRssiReady(RAIL_Handle_t railHandle);

/**
 * Gets the RSSI averaged over specified time in us.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return Return \ref RAIL_RSSI_INVALID if the receiver is disabled
 *   an RSSI value can't be obtained. Otherwise, return the RSSI in
 *   quarter dBm,dbm*4.
 *
 * Gets the hardware RSSI average after issuing RAIL_StartAverageRssi.
 * It should be used after \ref RAIL_StartAverageRssi.
 */
int16_t RAIL_GetAverageRssi(RAIL_Handle_t railHandle);

/******************************************************************************
 * Address Filtering (RX)
 *****************************************************************************/
/**
 * @addtogroup Address_Filtering Address Filtering
 * @brief Configuration APIs for receive packet address filtering.
 *
 * The address filtering code examines the packet as follows.
 *
 * | `Bytes: 0 - 255` | `0 - 8`  | `0 - 255` | `0 - 8`  | `Variable` |
 * |:----------------:|---------:|----------:|---------:|:----------:|
 * | `Data0`          | `Field0` | `Data1`   | `Field1` | `Data2`    |
 *
 * In the above structure, anything listed as DataN is an optional section of
 * bytes that RAIL will not process for address filtering. The FieldN segments
 * reference-specific sections in the packet that will each be interpreted
 * as an address during address filtering. The application may submit up to
 * four addresses to attempt to match each field segment and each address may
 * have a size of up to 8 bytes. To set up address filtering, first configure
 * the locations and length of the addresses in the packet. Next, configure
 * which combinations of matches in Field0 and Field1 should constitute an
 * address match. Lastly, enter addresses into tables for each field and
 * enable them. The first two of these are part of the \ref RAIL_AddrConfig_t
 * structure while the second part is configured at runtime using the
 * RAIL_SetAddressFilterAddress() API. A brief description of each
 * configuration is listed below.
 *
 * For the first piece of configuration, the offsets and sizes of the fields
 * are assumed fixed for the RAIL address filter. To set them, specify
 * arrays for these values in the sizes and offsets entries in the
 * \ref RAIL_AddrConfig_t structure. A size of zero indicates that a field is
 * disabled. The start offset for a field is relative to the previous start
 * offset and, if you're using FrameType decoding, the first start offset is
 * relative to the end of the byte containing the frame type.
 *
 * Configuring which combinations of Field0 and Field1 constitute a match is
 * the most complex portion of the address filter. The easiest way to think
 * about this is with a truth table. If you consider each of the four possible
 * address entries in a field, you can have a match on any one of those or a
 * match for none of them. This can be represented as a 4-bit mask where 1
 * indicates a match and 0 indicates no match. Representing the Field0 match
 * options as rows and the Field1 options as columns results in a truth table
 * as shown below.
 *
 * |          | 0000 | 0001 | 0010 | 0100 | 1000 |
 * |----------|------|------|------|------|------|
 * | __0000__ | bit0 | bit1 | bit2 | bit3 | bit4 |
 * | __0001__ | bit5 | bit6 | bit7 | bit8 | bit9 |
 * | __0010__ | bit10| bit11| bit12| bit13| bit14|
 * | __0100__ | bit15| bit16| bit17| bit18| bit19|
 * | __1000__ | bit20| bit21| bit22| bit23| bit24|
 *
 * Because this is only 25 bits, it can be represented in one 32-bit integer
 * where 1 indicates a filter pass and 0 indicates a filter fail. This is the
 * matchTable parameter in the configuration struct and is used during
 * filtering. For common simple configurations two defines are provided with
 * the truth tables as shown below. The first is \ref
 * ADDRCONFIG_MATCH_TABLE_SINGLE_FIELD, which can be used if only using
 * one address field (either field). If using two fields and want to
 * force in the same address entry in each field, use the second define: \ref
 * ADDRCONFIG_MATCH_TABLE_DOUBLE_FIELD. For more complex systems,
 * create a valid custom table.
 *
 * @note Address filtering does not function reliably with PHYs that use a data
 *   rate greater than 500 kbps. If this is a requirement, filter in software
 *   for the time being.
 *
 * @{
 */

/**
 * Configures address filtering.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] addrConfig The configuration structure, which defines how
 *   addresses are setup in your packets.
 * @return Status code indicating success of the function call.
 *
 * This function must be called to set up address filtering. You may call it
 * multiple times but all previous information is wiped out each time you call
 * and any configured addresses must be reset.
 */
RAIL_Status_t RAIL_ConfigAddressFilter(RAIL_Handle_t railHandle,
                                       const RAIL_AddrConfig_t *addrConfig);

/**
 * Enables address filtering.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] enable An argument to indicate whether or not to enable address
 *   filtering.
 * @return True if address filtering was enabled to start with and false
 *   otherwise.
 *
 * Only allow packets through that pass the current address filtering
 * configuration. This does not reset or change the configuration so you can
 * set that up before turning on this feature.
 */
bool RAIL_EnableAddressFilter(RAIL_Handle_t railHandle, bool enable);

/**
 * Returns whether address filtering is currently enabled.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return True if address filtering is enabled and false otherwise.
 */
bool RAIL_IsAddressFilterEnabled(RAIL_Handle_t railHandle);

/**
 * Resets the address filtering configuration.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return void.
 *
 * Resets all structures related to address filtering. This does not disable
 * address filtering. It leaves the radio in a state where no packets
 * pass filtering.
 */
void RAIL_ResetAddressFilter(RAIL_Handle_t railHandle);

/**
 * Sets an address for filtering in hardware.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] field Which address field you want to use for this address.
 * @param[in] index Which match entry you want to place this address in for a
 *   given field.
 * @param[in] value A pointer to the address data. This must be at least as
 *   long as the size specified in RAIL_ConfigAddressFilter().
 * @param[in] enable A boolean to indicate whether this address should be
 *   enabled immediately.
 * @return Status code indicating success of the function call.
 *
 * This function loads the given address into hardware for filtering and
 * starts filtering if you set the enable parameter to true. Otherwise,
 * call RAIL_EnableAddressFilterAddress() to turn it on later.
 */
RAIL_Status_t RAIL_SetAddressFilterAddress(RAIL_Handle_t railHandle,
                                           uint8_t field,
                                           uint8_t index,
                                           const uint8_t *value,
                                           bool enable);

/**
 * Enables address filtering for the specified address.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] enable An argument to indicate whether or not to enable address
 *   filtering.
 * @param[in] field Which address field you want to enable the address in.
 * @param[in] index Which match entry in the given field you want to enable.
 * @return Status code indicating success of the function call.
 */
RAIL_Status_t RAIL_EnableAddressFilterAddress(RAIL_Handle_t railHandle,
                                              bool enable,
                                              uint8_t field,
                                              uint8_t index);

/** @} */ // end of group Address_Filtering

/** @} */ // end of group Receive

/******************************************************************************
 * Auto Acking
 *****************************************************************************/
/// @addtogroup Auto_Ack Auto ACK
/// @brief APIs for configuring auto ACK functionality
///
/// These APIs are used to configure the radio for auto acknowledgment
/// features. Auto ACK inherently changes how the underlying state machine
/// behaves so users should not modify RAIL_SetRxTransitions() and
/// RAIL_SetTxTransitions() while using auto ACK features.
///
/// @code{.c}
/// // Go to RX after ACK operation
/// RAIL_AutoAckConfig_t autoAckConfig = {
///   .enable = true,
///   .ackTimeout = 1000,
///   // "error" param ignored
///   .rxTransitions = { RAIL_RF_STATE_RX, RAIL_RF_STATE_RX},
///   // "error" param ignored
///   .txTransitions = { RAIL_RF_STATE_RX, RAIL_RF_STATE_RX}
/// };
///
/// RAIL_Status_t status = RAIL_ConfigAutoAck(railHandle, &autoAckConfig);
///
/// uint8_t ackData[] = {0x05, 0x02, 0x10, 0x00};
///
/// RAIL_Status_t status = RAIL_WriteAutoAckFifo(ackData, sizeof(ackData));
/// @endcode
///
/// The acknowledgment transmits based on the frame format configured via
/// the Radio Configurator. For example, if the frame format is using a variable
/// length scheme, the ACK will be sent according to that scheme. If a 10-byte
/// packet is loaded into the ACK, but the variable length field of the ACK
/// payload specifies a length of 5, only 5 bytes will transmit for the ACK.
/// The converse is also true, if the frame length is configured to be a fixed
/// 10-byte packet but only 5 bytes are loaded into the ACK buffer, a TX
/// underflow occurs during the ACK transmit.
///
/// Unlike in non-ACK mode, ACK mode will always return to a single
/// state after all ACK sequences complete, regardless of whether
/// the ACK was successfully received/sent or not. Read the documentation
/// of RAIL_ConfigAutoAck for more detail on how that is configured. To
/// not auto acknowledge a series of packets after transmit
/// or receive, call RAIL_PauseTxAutoAck(true) or RAIL_PauseRxAutoAck(true).
/// When auto acking is paused, after receiving or transmitting (also
/// regardless of success) a packet, the radio transitions to the same single
/// state it always defaults to while acking. To return to
/// normal state transition logic outside of acking, you must call
/// RAIL_ConfigAutoAck with the "enable" field false, and specify the
/// desired transitions in the rxTransitions and txTransitions fields.
/// To simply get out of a paused state and resume auto acking, call
/// RAIL_PauseTxAutoAck(false) or RAIL_PauseRxAutoAck(false).
///
/// Applications can cancel the transmission of an ACK with
/// RAIL_CancelAutoAck(). Conversely, applications can control if a transmit
/// operation should wait for an ACK after transmitting by using
/// the \ref RAIL_TX_OPTION_WAIT_FOR_ACK bit.
///
/// If the ACK payload is dynamic, the application must call
/// RAIL_WriteAutoAckFifo() with the appropriate ACK payload after the
/// application processes the receive. RAIL can auto ACK from the normal
/// transmit buffer if RAIL_UseTxFifoForAutoAck() is called before the radio
/// transmits the ACK. Ensure the transmit buffer contains data loaded by
/// RAIL_WriteTxFifo().
///
/// Standard-based protocols that contain auto ACK functionality are normally
/// configured in the protocol-specific configuration function. For example,
/// RAIL_IEEE802154_Init() provides auto ACK configuration parameters in \ref
/// RAIL_IEEE802154_Config_t and should only be configured through that
/// function. It is not advisable to call both RAIL_IEEE802154_Init() and
/// RAIL_ConfigAutoAck(). However, ACK modification functions are still valid to
/// use with protocol-specific ACKs. To cancel a IEEE 802.15.4 ACK transmit, use
/// RAIL_CancelAutoAck().
///
/// @{

/// Configures and enable auto acknowledgment.
///
/// @param[in] railHandle A RAIL instance handle.
/// @param[in] config Auto ACK configuration structure.
/// @return Status code indicating success of the function call.
///
/// Configures the RAIL state machine to for hardware-accelerated auto
/// acknowledgment. ACK timing parameters are defined in the configuration
/// structure.
///
/// While auto acking is enabled, do not call the following RAIL functions:
///   - RAIL_SetRxTransitions()
///   - RAIL_SetTxTransitions()
///
/// Note, that if you are enabling auto ACK (i.e. "enable" field is true)
/// The "error" fields of rxTransitions and txTransitions are ignored.
/// After all ACK sequences, (success or fail) the state machine will return
/// the radio to the "success" state. If you need information about the
/// actual success of the ACK sequence, you can use RAIL events such as
/// \ref RAIL_EVENT_TXACK_PACKET_SENT to make sure an ACK was sent, or
/// \ref RAIL_EVENT_RX_ACK_TIMEOUT to make sure that an ACK was received
/// within the specified timeout.
///
/// If you wish to set a certain turnaround time (i.e. txToRx and rxToTx
/// in \ref RAIL_StateTiming_t), we recommend that you make txToRx lower than
/// desired, in order to ensure you get to RX in time to receive the ACK.
/// Silicon Labs recommends setting 10us lower than desired:
///
/// @code{.c}
/// void setAutoAckStateTimings()
/// {
///   RAIL_StateTiming_t timings;
///
///   // User is already in auto ACK and wants a turnaround of 192us
///   timings.rxToTx = 192;
///   timings.txToRx = 192 - 10;
///
///   // Set other fields of timings...
///   timings.idleToRx = 100;
///   timings.idleToTx = 100;
///   timings.rxSearchTimeout = 0;
///   timings.txToRxSearchTimeout = 0;
///
///   RAIL_SetStateTiming(railHandle, &timings);
/// }
/// @endcode
///
/// As opposed to an explicit "Disable" API, simply set the "enable"
/// field of the RAIL_AutoAckConfig_t to false. Then, auto ACK will be
/// disabled and state transitions will be returned to the values set
/// in \ref RAIL_AutoAckConfig_t. During this disable, the "ackTimeout" field
/// isn't used.
///
RAIL_Status_t RAIL_ConfigAutoAck(RAIL_Handle_t railHandle,
                                 const RAIL_AutoAckConfig_t *config);

/**
 * Returns the enable status of the auto ACK feature.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return true if auto ACK is enabled, false if disabled.
 */
bool RAIL_IsAutoAckEnabled(RAIL_Handle_t railHandle);

/**
 * Loads the auto ACK buffer with ACK data.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] ackData A pointer to ACK data to transmit.
 * @param[in] ackDataLen Number of bytes of ACK data.
 * @return Status code indicating success of the function call.
 *
 * If the ACK buffer is available for updates, load the ACK buffer with data.
 */
RAIL_Status_t RAIL_WriteAutoAckFifo(RAIL_Handle_t railHandle,
                                    const uint8_t *ackData,
                                    uint8_t ackDataLen);

/**
 * Pauses/resumes RX auto ACK functionality.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] pause Pause or resume RX auto acking.
 * @return void.
 *
 * When RX auto acking is paused, the radio transitions to default
 * state after receiving a packet and does not transmit an ACK.
 * When RX auto ACK is resumed, the radio resumes automatically acking
 * every successfully received packet.
 */
void RAIL_PauseRxAutoAck(RAIL_Handle_t railHandle,
                         bool pause);

/**
 * Returns whether the RX auto ACK is paused.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return true if RX auto ACK is paused, false if not paused.
 */
bool RAIL_IsRxAutoAckPaused(RAIL_Handle_t railHandle);

/**
 * Pauses/resumes TX auto ACK functionality.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] pause Pause or resume TX auto acking.
 * @return void.
 *
 * When TX auto acking is paused, the radio transitions to a default
 * state after transmitting a packet and does not wait for an ACK. When TX
 * auto ACK is resumed, the radio resumes automatically waiting for
 * an ACK after a successful transmit.
 */
void RAIL_PauseTxAutoAck(RAIL_Handle_t railHandle, bool pause);

/**
 * Returns whether the TX auto ACK is paused.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return true if TX auto ACK is paused, false if not paused.
 */
bool RAIL_IsTxAutoAckPaused(RAIL_Handle_t railHandle);

/**
 * Modifies the upcoming ACK to use the TX Buffer.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return Status code indicating success of the function call. This call will
 *   fail if it is too late to modify the outgoing ACK.
 *
 * This function allows the application to use the normal TX buffer as the data
 * source for the upcoming ACK. The ACK modification to use the TX buffer only
 * applies to one ACK transmission.
 *
 * This function only returns true if the following conditions are met:
 *   - Radio has not already decided to use the ACK buffer AND
 *   - Radio is either looking for sync, receiving the packet after sync, or in
 *     the Rx2Tx turnaround before the ACK is sent.
 */
RAIL_Status_t RAIL_UseTxFifoForAutoAck(RAIL_Handle_t railHandle);

/**
 * Cancels the upcoming ACK.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return Status code indicating success of the function call. This call will
 *   fail if it is too late to modify the outgoing ACK.
 *
 * This function allows the application to cancel the upcoming automatic
 * acknowledgment.
 *
 * This function only returns true if the following conditions are met:
 *   - Radio has not already decided to transmit the ACK AND
 *   - Radio is either looking for sync, receiving the packet after sync or in
 *     the Rx2Tx turnaround before the ACK is sent.
 */
RAIL_Status_t RAIL_CancelAutoAck(RAIL_Handle_t railHandle);

/**
 * Returns whether the radio is currently waiting for an ACK.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return True if radio is waiting for ACK, false if radio is not waiting for
 *   an ACK.
 *
 * This function allows the application to query whether the radio is currently
 * waiting for an ACK after a transmit operation.
 */
bool RAIL_IsAutoAckWaitingForAck(RAIL_Handle_t railHandle);

/** @} */ // end of group Auto_Ack

/******************************************************************************
 * Calibration
 *****************************************************************************/
/// @addtogroup Calibration
/// @brief APIs for calibrating the radio
/// @{
///
/// These APIs can be used to calibrate the radio. The RAIL library
/// determines which calibrations are necessary. Calibrations can
/// be enabled/disabled with the RAIL_CalMask_t parameter.
///
/// Some calibrations produce values that can be saved and reapplied to
/// save repetition of the calibration process.
///
/// Calibrations can either be run with \ref RAIL_Calibrate, or with the
/// individual chip-specific calibration routines. An example for running code
/// with \ref RAIL_Calibrate looks like:
///
/// @code{.c}
/// static RAIL_CalValues_t calValues = RAIL_CALVALUES_UNINIT;
///
/// void RAILCb_Event(RAIL_Handle_t railHandle, RAIL_Events_t events) {
///   // Omitting other event handlers
///   if (events & RAIL_EVENT_CAL_NEEDED) {
///     // Run all pending calibrations, and save the results
///     RAIL_Calibrate(railHandle, &calValues, RAIL_CAL_ALL_PENDING);
///   }
/// }
/// @endcode
///
/// Alternatively, if the image rejection calibration for your chip can be
/// determined ahead of time, such as by running the calibration on a separate
/// firmware image on each chip, then the following calibration process will
/// result in smaller code.
///
/// @code{.c}
/// static uint32_t imageRejection = IRCAL_VALUE;
///
/// void RAILCb_Event(RAIL_Handle_t railHandle, RAIL_Events_t events) {
///   // Omitting other event handlers
///   if (events & RAIL_EVENT_CAL_NEEDED) {
///     RAIL_CalMask_t pendingCals = RAIL_GetPendingCal(railHandle);
///     // Disable the radio if we have to do an offline calibration
///     if (pendingCals & RAIL_CAL_TEMP_VC0) {
///       RAIL_CalibrateTemp(railHandle);
///     }
///     if (pendingCals & RAIL_CAL_ONETIME_IRCAL) {
///       RAIL_ApplyIrCalibration(railHandle, imageRejection);
///     }
///   }
/// }
/// @endcode

/**
 * Initialize RAIL Calibration
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] calEnable A bitmask of which calibrations to enable for callback
 *   notification. The exact meaning of these bits is chip specific.
 * @return Status code indicating success of the function call.
 *
 * Calibration initialization provides the calibration settings that
 * correspond to the current radio configuration.
 */
RAIL_Status_t RAIL_ConfigCal(RAIL_Handle_t railHandle,
                             RAIL_CalMask_t calEnable);

/**
 * Starts the calibration process.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in,out] calValues A structure of calibration values to apply.
 *   If a valid calibration values structure is provided and the structure
 *   contains valid calibration values, those values will be applied to the
 *   hardware, and the RAIL library will cache those values for use again later.
 *   If a valid calibration values structure is provided and the structure
 *   contains a calibration value of \ref RAIL_CAL_INVALID_VALUE for the
 *   desired calibration, the desired calibration will run, the calibration
 *   values structure will be updated with a valid calibration value, and the
 *   RAIL library will cache that value for use again later.
 *   If a NULL pointer is provided, the desired calibration will run,
 *   and the RAIL library will cache that value for use again later; however,
 *   the valid calibration value will not be returned to the application.
 * @param[in] calForce A mask to force specific calibration(s) to execute.
 *   To run all pending calibrations, use the value \ref RAIL_CAL_ALL_PENDING.
 *   Only the calibrations specified will run, even if not enabled during
 *   initialization.
 * @return Status code indicating success of the function call.
 *
 * If calibrations were performed previously and the application saves the
 * calibration values (i.e. call this function with a calibration values
 * structure containing calibration values of \ref RAIL_CAL_INVALID_VALUE
 * before a reset), the application can later bypass the time it would normally
 * take to recalibrate hardware by reusing previous calibration values (i.e.
 * call this function with a calibration values structure containing valid
 * calibration values after a reset).
 *
 * If multiple protocols are used, this function will return
 * \ref RAIL_STATUS_INVALID_STATE if it is called and the given railHandle is
 * not active. The caller must attempt to re-call this function later, in that
 * case.
 *
 * @note Instead of this function, consider using the individual chip-specific
 *   functions. Using the individual functions will allow for better
 *   dead-stripping if not all calibrations are run.
 * @note Some calibrations should only be executed when the radio is IDLE. See
 *   chip-specific documentation for more details.
 */
RAIL_Status_t RAIL_Calibrate(RAIL_Handle_t railHandle,
                             RAIL_CalValues_t *calValues,
                             RAIL_CalMask_t calForce);

/**
 * Returns the current set of pending calibrations.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return A mask of all pending calibrations that the user has been asked to
 *   perform.
 *
 * This function returns a full set of pending calibrations. The only way
 * to clear pending calibrations is to perform them using the \ref
 * RAIL_Calibrate() API with the appropriate list of calibrations.
 */
RAIL_CalMask_t RAIL_GetPendingCal(RAIL_Handle_t railHandle);

/**
 * Enable/Disable PA calibration
 *
 * @param[in] enable Enables/Disables PA calibration
 * @return void.
 *
 * Enabling this will ensure that the PA power remains constant chip to chip.
 * By default this feature is disabled after reset.
 *
 * @note this function should be called before \ref RAIL_ConfigTxPower() if this
 *   feature is desired.
 */
void RAIL_EnablePaCal(bool enable);

/** @} */ // end of group Calibration

/******************************************************************************
 * RF Sense Structures
 *****************************************************************************/
/**
 * @addtogroup Rf_Sense RF Sense
 * @{
 */

/**
 * Starts/stops RF Sense functionality for use during low-energy sleep modes.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] band The frequency band(s) on which to sense the RF energy.
 *   To stop RF Sense, specify \ref RAIL_RFSENSE_OFF.
 * @param[in] senseTime The time (in microseconds) the RF energy must be
 *   continually detected to be considered "sensed".
 * @param[in] cb \ref RAIL_RfSense_CallbackPtr_t is called when the RF is
 *   sensed. Set null if polling via \ref RAIL_IsRfSensed().
 * @return The actual senseTime used, which may be different than
 *   requested due to limitations of the hardware. If 0, RF sense was
 *   disabled or could not be enabled (no callback will be issued).
 *
 * The EFR32 has the ability to sense the presence of RF Energy above -20 dBm
 * within either or both the 2.4 GHz and Sub-GHz bands and trigger an event
 * if that energy is continuously present for certain durations of time.
 *
 * @note After RF energy has been sensed, the RF Sense is automatically
 *   disabled. RAIL_StartRfSense() must be called again to reactivate it.
 *
 * @note Packet reception is not guaranteed to work correctly once RF Sense is
 *   enabled. To be safe, an application should turn this on only after idling
 *   the radio to stop receive and turn it off before attempting to restart
 *   receive. Since EM4 sleep causes the chip to come up through the reset
 *   vector any wake from EM4 must also shut off RF Sense to ensure proper
 *   receive functionality.
 *
 * @warning RF Sense functionality is only guaranteed from 0 to 85 degrees
 *   Celsius. RF Sense should be disabled outside of this temperature range.
 */
RAIL_Time_t RAIL_StartRfSense(RAIL_Handle_t railHandle,
                              RAIL_RfSenseBand_t band,
                              RAIL_Time_t senseTime,
                              RAIL_RfSense_CallbackPtr_t cb);

/**
 * Checks if the RF was sensed.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return true if RF was sensed since the last call to \ref RAIL_StartRfSense.
 *   False otherwise.
 *
 * This function is useful if \ref RAIL_StartRfSense is called with a null
 * callback. It is generally used after EM4 reboot but can be used any time.
 */
bool RAIL_IsRfSensed(RAIL_Handle_t railHandle);

/** @} */ // end of group Rf_Sense

/******************************************************************************
 * Multiprotocol Structures
 *****************************************************************************/
/**
 * @addtogroup Multiprotocol
 * @brief Multiprotocol scheduler APIs to support multiple time-sliced PHYs.
 * @{
 */

/**
 * Yields the radio to other configurations
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return void.
 *
 * This function is used to indicate that the previous transmit or scheduled
 * receive operation has completed. It must be used in multiprotocol RAIL since
 * the scheduler assumes that any transmit or receive operation that is started
 * by you can go on infinitely based on state transitions and your protocol.
 * RAIL will not allow a lower priority tasks to run until this is called so it
 * can negatively impact performance of those protocols if this is omitted or
 * delayed. It is also possible to simply call the \ref RAIL_Idle() API to
 * to both terminate the operation and idle the radio. In single protocol RAIL
 * this API does nothing.
 *
 * See \ref rail_radio_scheduler_yield for more details.
 */
void RAIL_YieldRadio(RAIL_Handle_t railHandle);

/**
 * Get the status of the RAIL scheduler.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return \ref RAIL_SchedulerStatus_t status.
 *
 * This function can only be called from callback context after the
 * \ref RAIL_EVENT_SCHEDULER_STATUS event occurs.
 */
RAIL_SchedulerStatus_t RAIL_GetSchedulerStatus(RAIL_Handle_t railHandle);

/** @} */ // end of group Multiprotocol

/******************************************************************************
 * Diagnostic
 *****************************************************************************/
/**
 * @addtogroup Diagnostic
 * @brief APIs for diagnostic and test chip modes
 * @{
 */

/**
 * Enables or disables direct mode for RAIL.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] enable Whether or not to enable direct mode.
 * @return \ref RAIL_STATUS_NO_ERROR on success and an error code on failure.
 *
 * @warning This API configures fixed pins for TX data in, RX data out,
 *   RX clock out. There should be more control over these pins in the
 *   future but they are currently fixed. Also, this API is not safe to
 *   use in a true multiprotocol app.
 *
 * In this mode packets are output and input directly to the radio via GPIO
 * and RAIL packet handling is ignored. On the EFR32, the DIN pin in TX is
 * EFR32_PC10, which corresponds to EXP_HEADER15/WSTKP12, and the DOUT pin in
 * RX is EFR32_PC11, which corresponds to EXP_HEADER16/WSTKP13.
 */
RAIL_Status_t RAIL_EnableDirectMode(RAIL_Handle_t railHandle,
                                    bool enable);

/**
 * Sets the crystal tuning.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] tune A chip-dependent crystal capacitor bank tuning parameter.
 * @return Status code indicating success of the function call.
 *
 * Tunes the crystal that the radio depends on to change the location of the
 * center frequency for transmitting and receiving. This function will only
 * succeed if the radio is idle at the time of the call.
 *
 * @note This function proportionally affects the entire chip's timing
 *   across all its peripherals, including radio tuning and channel spacing.
 *   A separate function, \ref RAIL_SetFreqOffset(), can be used to adjust
 *   just the radio tuner without disturbing channel spacing or other chip
 *   peripheral timing.
 */
RAIL_Status_t RAIL_SetTune(RAIL_Handle_t railHandle, uint32_t tune);

/**
 * Gets the crystal tuning.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return A chip-dependent crystal capacitor bank tuning parameter.
 *
 * Retrieves the current tuning value used by the crystal that the radio
 * depends on.
 */
uint32_t RAIL_GetTune(RAIL_Handle_t railHandle);

/**
 * Gets the frequency offset.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return Returns the measured frequency offset on a received packet.
 *   The units are described in the \ref RAIL_FrequencyOffset_t
 *   documentation. If this returns \ref RAIL_FREQUENCY_OFFSET_INVALID
 *   it was called while the radio wasn't active and there is no way
 *   to get the frequency offset.
 *
 * Retrieves the measured frequency offset used during the previous
 * received packet, which includes the current radio frequency offset
 * (see \ref RAIL_SetFreqOffset()). If the chip has not been in RX,
 * it returns the nominal radio frequency offset.
 */
RAIL_FrequencyOffset_t RAIL_GetRxFreqOffset(RAIL_Handle_t railHandle);

/**
 * Sets the nominal radio frequency offset.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] freqOffset \ref RAIL_FrequencyOffset_t parameter (signed, 2's
 *   complement).
 * @return Status code indicating success of the function call.
 *
 * This is used to adjust the radio's tuning frequency slightly up or down.
 * It might be used in conjunction with \ref RAIL_GetRxFreqOffset() after
 * receiving a packet from a peer to adjust the tuner to better match the
 * peer's tuned frequency.
 *
 * @note Unlike \ref RAIL_SetTune(), which affects the entire chip's
 *   timing including radio tuning and channel spacing, this function
 *   only affects radio tuning without disturbing channel spacing or
 *   other chip peripheral timing.
 */
RAIL_Status_t RAIL_SetFreqOffset(RAIL_Handle_t railHandle,
                                 RAIL_FrequencyOffset_t freqOffset);

/**
 * Starts transmitting a stream on a certain channel.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] channel A channel on which to emit a stream.
 * @param[in] mode Choose the stream mode (PN9, and so on).
 * @return Status code indicating success of the function call.
 *
 * Begins streaming onto the given channel. The sources can either be an
 * unmodulated carrier wave, or an encoded stream of bits from a PN9 source.
 * All ongoing radio operations will be stopped before transmission begins.
 */
RAIL_Status_t RAIL_StartTxStream(RAIL_Handle_t railHandle,
                                 uint16_t channel,
                                 RAIL_StreamMode_t mode);

/**
 * Stops stream transmission.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return Status code indicating success of the function call.
 *
 * Halts the transmission started by RAIL_StartTxStream().
 */
RAIL_Status_t RAIL_StopTxStream(RAIL_Handle_t railHandle);

/**
 * Configures the verification of radio memory contents.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in,out] configVerify A configuration structure made available to
 *   RAIL in order to perform radio state verification. This structure must be
 *   allocated in application global read-write memory. RAIL may modify
 *   fields within or referenced by this structure during its operation.
 * @param[in] radioConfig A ptr to a radioConfig that is to be used as a
 *   white list for verifying memory contents.
 * @param[in] cb A callback that notifies the application of a mismatch in
 *   expected vs actual memory contents. A NULL parameter may be passed in
 *   if a callback is not provided by the application.
 * @return \ref RAIL_STATUS_NO_ERROR if setup of the verification feature
 *   successfully occurred.
 *   \ref RAIL_STATUS_INVALID_PARAMETER is returned if the provided railHandle
 *   or configVerify structures are invalid.
 */
RAIL_Status_t RAIL_ConfigVerification(RAIL_Handle_t railHandle,
                                      RAIL_VerifyConfig_t *configVerify,
                                      const uint32_t *radioConfig,
                                      RAIL_VerifyCallbackPtr_t cb);

/**
 * Verifies radio memory contents.
 *
 * @param[in,out] configVerify A configuration structure made available to
 *   RAIL in order to perform radio state verification. This structure must be
 *   allocated in application global read-write memory. RAIL may modify
 *   fields within or referenced by this structure during its operation.
 * @param[in] durationUs The duration (in microseconds) for how long memory
 *   verification should occur before returning to the application. A value of
 *   RAIL_VERIFY_DURATION_MAX indicates that all memory contents should be
 *   verified before returning to the application.
 * @param[in] restart This flag only has meaning if a previous call of this
 *   function returned \ref RAIL_STATUS_SUSPENDED. By restarting (true), the
 *   verification process starts over from the beginning, or by resuming
 *   where verification left off after being suspended (false), verification
 *   can proceed towards completion.
 * @return \ref RAIL_STATUS_NO_ERROR if the contents of all applicable
 *   memory locations have been verified.
 *   \ref RAIL_STATUS_SUSPENDED is returned if the provided test duration
 *   expired but the time was not sufficient to verify all memory contents.
 *   By calling \ref RAIL_Verify again, further verification will commence.
 *   \ref RAIL_STATUS_INVALID_PARAMETER is returned if the provided
 *   verifyConfig structure pointer is not configured for use by the active
 *   RAIL handle.
 *   \ref RAIL_STATUS_INVALID_STATE is returned if any of the verified
 *   memory contents are different from their reference values.
 */
RAIL_Status_t RAIL_Verify(RAIL_VerifyConfig_t *configVerify,
                          uint32_t durationUs,
                          bool restart);

/** @} */ // end of group Diagnostic

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/******************************************************************************
 * Debug
 *****************************************************************************/
/**
 * @addtogroup Debug
 * @brief APIs for debugging
 * @{
 */

/**
 * Configures the debug mode for the radio library. Do not use this function
 * unless instructed by Silicon Labs.
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] debugMode Debug mode to enter.
 * @return Status code indicating success of the function call.
 */
RAIL_Status_t RAIL_SetDebugMode(RAIL_Handle_t railHandle, uint32_t debugMode);

/**
 * Returns the debug mode for the radio library. Do not use this function
 * unless instructed by Silicon Labs.
 * @param[in] railHandle A RAIL instance handle.
 * @return Debug mode for the radio library.
 */
uint32_t RAIL_GetDebugMode(RAIL_Handle_t railHandle);

/**
 * Overrides the radio base frequency.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] freq A desired frequency in Hz.
 * @return Status code indicating success of the function call.
 *
 * Sets the radio to transmit at the frequency given. This function can only
 * be used while in \ref RAIL_DEBUG_MODE_FREQ_OVERRIDE. The given frequency
 * needs to be close to the base frequency of the current PHY.
 */
RAIL_Status_t RAIL_OverrideDebugFrequency(RAIL_Handle_t railHandle,
                                          uint32_t freq);

/** @} */ // end of group Debug

#endif//DOXYGEN_SHOULD_SKIP_THIS

/******************************************************************************
 * Assertion Callback
 *****************************************************************************/
/**
 * @addtogroup Assertions
 * @brief Callbacks called by assertions
 *
 * This assertion framework was implemented for the purpose of not only being
 * able to assert that certain conditions be true in a block of code, but also
 * to be able to handle them more appropriately. In previous implementations,
 * the behavior upon a failed assert would be to hang in a while(1) loop.
 * However, with the callback, each assert is given a unique error code so that
 * they can be handled on a more case-by-case basis. For documentation on each
 * of the errors, please see the rail_assert_error_codes.h file.
 * RAIL_ASSERT_ERROR_MESSAGES[errorCode] gives the explanation of the error.
 * With asserts built into the library, customers can choose how to handle each
 * error inside the callback.
 *
 * @{
 */

/**
 * Callback called upon failed assertion.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] errorCode Value passed in by the calling assertion API indicating
 *   the RAIL error that is indicated by the failing assertion.
 * @return void.
 */
void RAILCb_AssertFailed(RAIL_Handle_t railHandle,
                         RAIL_AssertErrorCodes_t errorCode);

/** @} */ // end of group Assertions

/** @} */ // end of group RAIL_API

#ifdef __cplusplus
}
#endif

#endif // __RAIL_H__
