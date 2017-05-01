/***************************************************************************//**
 * @file rail.h
 * @brief The main header file for the RAIL library. It describes the external
 *        APIs available to a RAIL user
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef __RAIL_H__
#define __RAIL_H__

// Get the standard include types
#include <stdint.h>
#include <stdbool.h>

// Get the RAIL specific structures and types
#include "rail_types.h"

/**
 * @addtogroup RAIL_API
 * @brief This is the primary API layer for the Radio Abstraction Interface
 * Layer (RAIL)
 * @{
 */

/******************************************************************************
 * General Radio Operation
 *****************************************************************************/
/**
 * @addtogroup General
 * @brief Basic APIs for setting up and interacting with the RAIL library
 * @{
 */

/**
 * Get the version information for the compiled RAIL library.
 *
 * @param[in] version Pointer to \ref RAIL_Version_t struct to populate with version
 *   information.
 * @param[in] verbose Populate \ref RAIL_Version_t struct with verbose information
 *
 * Version information contains a major version number, a minor version number,
 * and a rev (revision) number.
 */
void RAIL_VersionGet(RAIL_Version_t * version, bool verbose);

/**
 * Initialize RAIL
 *
 * @param[in] railInit The initialization structure to be used for setting up
 *   the library. This will contain memory and other options needed by RAIL.
 * @return Returns zero on success and an error code on error.
 *
 * RF initialization sets the overall maximum packet length, the xtal frequency
 * of the radio, and the calibrations to perform.
 */
uint8_t RAIL_RfInit(const RAIL_Init_t *railInit);

/**
 * Set protocol that RAIL outputs on PTI
 *
 * @param protocol The enum representing which protocol the node is using
 * @return Returns zero on success and an error code on error.
 *
 * The protocol is output via the Packet Trace Interface (PTI) for each packet.
 * Before any protocol is set, the default value is \ref
 * RAIL_PTI_PROTOCOL_CUSTOM. One of the enum values should be used in order for
 * Network Analyzer to be able to decode the packet.
 */
RAIL_Status_t RAIL_SetPtiProtocol(RAIL_PtiProtocol_t protocol);

/**
 * Callback for when the radio is finished initializing from \ref RAIL_RfInit
 * and is ready to be configured
 *
 * @return void
 *
 * Callback that notifies the application when the radio is finished
 * initializing and is ready for further configuration. This callback is
 * useful for potential transceiver products that require a power up sequence
 * before further configuration is available.  After this callback fires, the
 * radio is ready for additional configuration before transmit and receive
 * operations.
 */
void RAILCb_RfReady(void);

/**
 * Get the current radio state
 *
 * @return An enumeration for current radio state
 *
 * Returns the state of the radio as either TX, RX, or IDLE. There are
 * intermediate states that the radio can transistion through which are not
 * reported, but are instead bucketed into the state being transitioned
 * into. (Example: When the transmitter is in the process of shutting down,
 * this function will return TX, as if the shutdown process hadn't started yet)
 */
RAIL_RadioState_t RAIL_RfStateGet(void);

/**
 * Configure RAIL automatic state transitions after RX
 *
 * @param[in] success The next radio state to enter after a successful packet
 * reception.
 * @param[in] error The next radio state to enter after an error during packet
 * reception.
 * @param[in] ignoreErrors Define errors during packet handling to be ignored
 * @return Returns zero on success and an error code on error.
 *
 * This function fails if unsupported transitions are passed in, or if the
 * radio is currently in the RX state. Success can transition to TX, RX, or
 * IDLE, while error can transition to RX or IDLE. The full list of options for
 * the ignoreErrors parameter is any define that starts with
 * \link RAIL_IGNORE_NO_ERRORS RAIL_IGNORE_\endlink.
 */
RAIL_Status_t RAIL_SetRxTransitions(RAIL_RadioState_t success,
                                    RAIL_RadioState_t error,
                                    uint8_t ignoreErrors);

/**
 * Configure RAIL automatic state transitions after TX
 *
 * @param[in] success The next radio state to enter after a successful packet
 * transmission.
 * @param[in] error The next radio state to enter after an error during packet
 * transmission.
 * @return Returns zero on success and an error code on error.
 *
 * This function fails if unsupported transitions are passed in, or if the
 * radio is currently the TX state. Success and error can each transition to RX
 * or IDLE.
 */
RAIL_Status_t RAIL_SetTxTransitions(RAIL_RadioState_t success,
                                    RAIL_RadioState_t error);

/**
 * Configure RAIL automatic state transition timing
 *
 * @param[in] timings The timings used to configure the RAIL state machine. This
 * structure will be overwritten with the actual times that were set, in the
 * case of an input timing that is invalid.
 * @return Returns zero on success and an error code on error.
 *
 * The timings given will be close to the actual transition time, but there is
 * some software overhead that is not yet characterized. Also, timings are not
 * always adhered to when using an automatic transition after an error, due to
 * the cleanup required to recover from the error.
 */
RAIL_Status_t RAIL_SetStateTiming(RAIL_StateTiming_t *timings);

/**
 * Place the radio into an idle state
 *
 * @return void
 *
 * This function is used to remove the radio from TX and RX states.
 */
void RAIL_RfIdle(void);

/**
 * Extended radio idle API
 *
 * @param[in] mode The method to use for shutting down the radio.
 * @param[in] wait Whether this function should wait for the radio to reach idle
 * before returning.
 *
 * This is an extended version of the simple RAIL_RfIdle() API which lets the
 * application specify how it reaches idle state and if the function should
 * busy wait.
 */
void RAIL_RfIdleExt(RAIL_RfIdleMode_t mode, bool wait);

/**
 * Start/Stop RF Sense functionality for use during low-energy sleep modes.
 *
 * @param[in] band The frequency band(s) on which to sense RF energy.
 * To stop Rf Sense, specify \ref RAIL_RFSENSE_OFF.
 * @param[in] senseTime The time (in microseconds) RF energy must be
 * continually detected to be considered "sensed".
 * @param[in] enableCb Set true to enable \ref RAILCb_RxRadioStatus() callback
 * with status \ref RAIL_RX_CONFIG_RF_SENSED when Rf is sensed.  Set false if
 * prefer to poll via \ref RAIL_RfSensed().
 *
 * @return The actual senseTime utilized, which may be different than
 * requested due to limitations of the hardware.  If 0, RF sense was
 * disabled or it could not be enabled (no callback will be issued).
 *
 * The EFR32 has the ability to sense the presence of RF Energy above -20 dBm
 * within either or both the 2.4 GHz and Sub-GHz bands, and trigger an event
 * if that energy is continuously present for certain durations of time.
 *
 * @note After RF energy has been sensed, RF Sense is automatically disabled,
 * and RAIL_RfSense() must be called again to reactivate it.
 *
 * @warning RF Sense functionality is only guaranteed from 0 to
 *          85 degrees Celsius. RF Sense should be disabled
 *          outside this Temperature range.
 */
uint32_t RAIL_RfSense(RAIL_RfSenseBand_t band, uint32_t senseTime, bool enableCb);

/**
 * Check if RF was sensed.
 *
 * @return true if RF was sensed since last call to \ref RAIL_RfSense; false
 * otherwise.
 *
 * This function is useful if \ref RAIL_RfSense has been called with enableCb
 * set to false. It is generally used after EM4 reboot, but can be used any
 * time.
 */
bool RAIL_RfSensed(void);

/**
 * Modify the currently configured fixed length
 *
 * @param[in] length Expected fixed length; 0 is infinite
 * @return Length configured; 0xFFFF if not in fixed length, 0 if in infinite
 *
 * Set the fixed length configuration for transmit and receive. Users should
 * be careful when using this function in receive and transmit. This function
 * returns \ref RAIL_SETFIXEDLENGTH_INVALID if the radio is not in fixed length
 * mode. The function returns 0 if in infinite length mode. Otherwise it will
 * return the length configured into the hardware.
 */
uint16_t RAIL_SetFixedLength(uint16_t length);

/***************************************************************************//**
 * Collect entropy from the radio if available.
 *
 * @param buffer The buffer to write the collected entropy to.
 * @param bytes The number of bytes to fill in in the input buffer
 * @return Returns the number of bytes of entropy we were able to collect. For
 * chips that don't support entropy collection this will return 0. Values less
 * than the requested amount may also be returned on platforms that use entropy
 * pools to collect random data periodically.
 *
 * Attempts to fill up the provided buffer with the requested number of bytes of
 * entropy. If we cannot provide as many bytes as requested then we will fill in
 * whatever we can and return the number of bytes we were able to get. For chips
 * that do not support this function we will always return 0 bytes. For
 * information about the specific mechanism for gathering entropy consult the
 * documentation for the chip family you're using.
 ******************************************************************************/
uint16_t RAIL_GetRadioEntropy(uint8_t *buffer, uint16_t bytes);

/**
 * @}
 */

/**
 * @addtogroup Memory_Management
 * @brief Application callbacks to provide memory for RAIL actions.
 *
 * The RAIL library does not want to dictate how upper layers handle memory
 * allocation for packet receive data. At the same time we need to put the
 * packets somewhere to give them to the upper layers. To abstract this we
 * require the user application to implement the RAILCb_AllocateMemory(),
 * RAILCb_FreeMemory(), RAILCb_BeginWriteMemory(), and RAILCb_EndWriteMemory()
 * callbacks. These callbacks will be called from interrupt context to interact
 * with whatever memory allocation system your application uses.
 *
 * Memory will be allocated for receiving a packet whenever we think we need
 * it.  This depends on the chip you're using and possibly the size of your
 * maximum packet. We will never ask for more memory than `MAX_PACKET_SIZE +
 * sizeof(\ref RAIL_RxPacketInfo_t)` where MAX_PACKET_SIZE is the maximum
 * packet your PHY is configured to receive over the air. Once you give us the
 * handle to this memory it must stay valid until we tell you we are done with
 * it using the RAILCb_FreeMemory() callback. Generally this will happen
 * immediately after we call the RAILCb_RxPacketReceived() function with that
 * handle. RAIL has no concept of an invalid handle so we will attempt to use
 * whatever you pass to us. This means that you will still receive all
 * callbacks for invalid handles even if we are forced to drop receive packet
 * bytes because they don't fit anywhere.
 *
 * If the handle is invalid you must make sure your callbacks do not
 * crash and that RAILCb_BeginWriteMemory() returns a NULL pointer or 0 bytes
 * available so that we do not try to write to this memory. In this case, the
 * packet data will be dropped.
 *
 * To actually write data to the handle you provide us we need to convert it
 * into an actual memory pointer. We will do this each time we need to access
 * the memory by calling RAILCb_BeginWriteMemory(). This function must return
 * a pointer to the requested offset in the memory buffer allocated. If you are
 * using non-contiguous memory buffers you can also return the number of bytes
 * available before we need to re-request a pointer with a new offset. Once the
 * access is complete we will call RAILCb_EndWriteMemory() with information
 * about exactly how many bytes were written at the specified offset. After this
 * call we will always call RAILCb_BeginWriteMemory() again before trying to
 * write any more data. In the event that you receive an invalid handle these
 * APIs must return NULL or set available bytes to 0 so that we do not attempt
 * to write packet data to the buffer.
 *
 * This system is fairly flexible and can tie into many higher level memory
 * allocation APIs. A simple example using one static buffer for memory
 * allocation is shown below. You will probably want a more advanced system
 * that can handle receiving multiple packets simultaneously.
 *
 * @code{.c}
 * static uint8_t buffer[MAX_PACKET_SIZE + sizeof(RAIL_RxPacketInfo_t)];
 * static bool isAllocated = false;
 *
 * void *RAILCb_AllocateMemory(uint32_t size)
 * {
 *   int i = 0;
 *   void *ptr = NULL;
 *   CORE_DECLARE_IRQ_STATE;
 *
 *   // We can't support sizes greater than the maximum buffer size
 *   if(size > (MAX_PACKET_SIZE + sizeof(RAIL_RxPacketInfo_t))) {
 *     return NULL;
 *   }
 *
 *   // Disable interrupts and attempt to grab the buffer
 *   CORE_ENTER_CRITICAL();
 *   if (isAllocated) {
 *     ptr = NULL;
 *   } else {
 *     isAllocated = true;
 *     ptr = buffer;
 *   }
 *   CORE_EXIT_CRITICAL();
 *
 *   return ptr;
 * }
 *
 * void RAILCb_FreeMemory(void *ptr)
 * {
 *   CORE_CRITICAL_SECTION(
 *     isAllocated = false;
 *   );
 * }
 *
 * void *RAILCb_BeginWriteMemory(void *handle,
 *                               uint32_t offset,
 *                               uint32_t *available)
 * {
 *   return ((uint8_t*)handle) + offset;
 * }
 *
 * void RAILCb_EndWriteMemory(void *handle, uint32_t offset, uint32_t size)
 * {
 *   // Do nothing
 * }
 * @endcode
 *
 * @{
 */

/**
 * Callback function used by RAIL to request memory.
 *
 * @param[in] size The amount of memory in bytes that we need for this packet
 * @return A handle to memory in your storage system.
 *
 * This is used to allocate memory for receive packets and must be implemented
 * by the application.
 */
void *RAILCb_AllocateMemory(uint32_t size);

/**
 * Callback function used by RAIL to free memory.
 *
 * @param[in] handle A handle to a memory block allocated with the
 *   RAILCb_AllocateMemory() API above.
 *
 * This is used to free memory that was allocated with the
 * RAILCb_AllocateMemory() function when RAIL is done using it.
 */
void RAILCb_FreeMemory(void *handle);

/**
 * Called to begin copying received data into the current memory handle.
 *
 * @param[in] handle A handle to the current memory block for packet data.
 * @param[in] offset The offset in bytes from the start of the handle that we
 * need a pointer for.
 * @param[out] available The number of bytes available to be written to this
 * return pointer. If this is zero the receive will terminate. This parameter
 * will default to all spaces allocated to handle contiguous allocators. If your
 * allocator is different you *must* set this appropriately.
 * @return A pointer to the address to write data for this handle.
 *
 * This function is called before every memory write to a handle so that we can
 * get the actual address this handle references in the system. When we're done
 * writing there will be a corresponding call to RAILCb_EndWriteMemory().
 *
 * @note You must have at least `sizeof(RAIL_RxPacketInfo_t)` contiguous bytes at
 * offset 0 or the appended info will not be written.
 */
void *RAILCb_BeginWriteMemory(void *handle,
                              uint32_t offset,
                              uint32_t *available);

/**
 * Called to complete the write memory transaction.
 *
 * @param handle The handle to the current memory block we're modifying.
 * @param offset The offset in bytes from the start of the handle that this data
 * was written to.
 * @param size The number of bytes that were written.
 *
 * This callback indicates the completeion of a write memory transaction. It
 * can be used to store information about how many bytes were written or
 * anything else needed. Once this is called the pointer returned by
 * RAILCb_BeginWriteMemory() will no longer be assumed to be valid and we will
 * call that function again for any future writes.
 */
void RAILCb_EndWriteMemory(void *handle, uint32_t offset, uint32_t size);

/**
 * @}
 */

/******************************************************************************
 * Data Management
 *****************************************************************************/
/**
 * @addtogroup Data_Management
 * @brief Data management functions
 *
 * These functions allow the application to choose how data is presented to the
 * application. There are two methods for RAIL to provide data, in a packet
 * based method leveraging \ref Memory_Management callbacks or in a FIFO based
 * method which gives the application more granularity and responsibility in
 * managing transmit and receive data.
 *
 * The application can configure RAIL data mangement through RAIL_DataConfig();
 * this function allows the application to specify the type of radio data (\ref
 * RAIL_TxDataSource_t and \ref RAIL_RxDataSource_t) and the method of
 * interacting with this data (\ref RAIL_DataMethod_t). By default, RAIL
 * configures Tx and Rx both with packet data source and packet mode.
 *
 * In packet based data management:
 *   - Load transmit data with RAIL_TxDataLoad()
 *   - Received data is returned in RAILCb_RxPacketReceived()
 *   - Packet lengths are determined from the Radio Configurator configuration
 *   - \ref Memory_Management callbacks will fire to ask for pointers to store
 *     data
 *
 * In FIFO based data management:
 *   - Load transmit data with RAIL_WriteTxFifo()
 *   - Received data is retrieved through RAIL_ReadRxFifo()
 *   - Packet Lengths are determined from the Radio Configurator configuration
 *   - Set fifo thresholds through RAIL_SetTxFifoThreshold() and
 *     RAIL_SetRxFifoThreshold() which fires RAILCb_RxFifoAlmostFull() and
 *     RAILCb_TxFifoAlmostEmpty().
 *   - Get fifo count information through RAIL_GetRxFifoBytesAvailable()
 *     and RAIL_GetTxFifoSpaceAvailable()
 *   - Reset fifos with RAIL_ResetFifo()
 *   - CRC Error acceptance is on by default
 *
 * Both transmit and receive fifos are the same size; when trying to determine
 * an appropriate threshold, the application can use
 * RAIL_GetTxFifoSpaceAvailable() to query the size of the fifo if it is empty
 * and use that as the size of the receive fifo as well.  The transmit fifo is edge
 * based where it only provides an interrupt once when the threshold is
 * crossed. The receive fifo is level based where the interrupt will constantly
 * pend if the threshold is exceeded. This normally means that inside
 * RAILCb_RxFifoAlmostFull(), the application should empty enough of the fifo
 * to go under the threshold. If the application wishes to defer reading the
 * fifo to main, it can disable the receive fifo threshold interrupt via
 * RAIL_DisableRxFifoThreshold(). The application can reenable the interrupt
 * via RAIL_EnableRxFifoThreshold().
 *
 * In fifo mode, the fifos can store multiple packets. Depending on traffic,
 * RAIL can receive multiple packets into the receive fifo before the
 * application gets around to reading out the received data from the fifo. If
 * appended info is enabled, make sure to read out the appended info with
 * RAIL_ReadRxFifoAppendedInfo() before attempting to read out the next packet.
 * If the application aborts during packet reception, appended info will not be
 * present in the receive fifo. If a frame error occurs in fifo mode, the
 * contents of the receive fifo is unreliable and should be flushed.
 *
 * When calling RAIL_DataConfig() for fifo mode, RAIL will set \ref
 * RAIL_IGNORE_CRC_ERRORS. Otherwise for packet mode, RAIL will set \ref
 * RAIL_IGNORE_NO_ERRORS. It is highly suggested that the application maintains
 * \ref RAIL_IGNORE_CRC_ERRORS in fifo mode if using hardware crc checking.
 *
 * While RAIL defaults to packet mode, the application can explicitly
 * initialize RAIL for packet mode in the following manner:
 * @code{.c}
 * static const RAIL_DataConfig_t railDataConfig = {
 *   .txSource = TX_PACKET_DATA,
 *   .rxSource = RX_PACKET_DATA,
 *   .txMethod = PACKET_MODE,
 *   .rxMethod = PACKET_MODE,
 * };
 *
 * status = RAIL_DataConfig(&railDataConfig);
 *
 * // Callbacks that occur in Packet Mode
 * void RAILCb_TxPacketSent(RAIL_TxPacketInfo_t *txPacketInfo);
 * void RAILCb_RxPacketReceived(void *rxPacketHandle);
 * void *RAILCb_AllocateMemory(uint32_t size);
 * void RAILCb_FreeMemory(void *handle);
 * void *RAILCb_BeginWriteMemory(void *handle,
 *                          uint32_t offset,
 *                          uint32_t *available);
 * void RAILCb_EndWriteMemory(void *handle, uint32_t offset, uint32_t size);
 * @endcode
 *
 * Initializing RAIL for Fifo Mode requires a few more function calls:
 * @code{.c}
 * static const RAIL_DataConfig_t railDataConfig = {
 *   .txSource = TX_PACKET_DATA,
 *   .rxSource = RX_PACKET_DATA,
 *   .txMethod = FIFO_MODE,
 *   .rxMethod = FIFO_MODE,
 * };
 *
 * status = RAIL_DataConfig(&railDataConfig);
 *
 * // Get the size of the fifos
 * // The transmit and receive fifos are the same size
 * uint16_t fifoSize = RAIL_GetTxFifoSpaceAvailable();
 *
 * // Set the transmit and receive fifo thresholds
 * // For this example, set the threshold in the middle of each fifo
 * RAIL_SetRxFifoThreshold(fifoSize / 2);
 * RAIL_SetTxFifoThreshold(fifoSize / 2);
 *
 * //Callbacks that occur in Fifo mode
 * void RAILCb_TxPacketSent(RAIL_TxPacketInfo_t *txPacketInfo);
 * void RAILCb_RxPacketReceived(void *rxPacketHandle);
 * void RAILCb_TxFifoAlmostEmpty(uint16_t spaceAvailable);
 * void RAILCb_RxFifoAlmostFull(uint16_t bytesAvailable);
 * @endcode
 *
 * On receive, there are multiple data sources that an application can use that
 * are only compatible with the fifo method of data delivery. All that differs
 * from the fifo mode example above is the RAIL_DataConfig_t::rxSource setting.
 * IQ data samples are taken at the hardware's oversample rate and the amount
 * of data can easily overwhelm CPU processing time. The sample rate depends on
 * the chosen PHY as is determined by the data rate as well as the decimation
 * chain. It is <b>not</b> recommended to use the IQ data source with sample
 * rates above 300k samples/second as the CPU might not be able to keep up with
 * the data.  Depending on the application and needed CPU bandwidth, slower
 * data rates may be required.
 * @code{.c}
 * // IQ data is provided into the receive fifo
 * static const RAIL_DataConfig_t railDataConfig = {
 *   .txSource = TX_PACKET_DATA,
 *   .rxSource = RX_IQDATA_FILTLSB,
 *   .txMethod = FIFO_MODE,
 *   .rxMethod = FIFO_MODE,
 * };
 *
 * // When reading IQ data out of the fifo, it comes in the following format:
 * //------------------------------------
 * // I[LSB] | I[MSB] | Q[LSB] | Q[MSB] |
 * //------------------------------------
 * @endcode
 *
 * @note \ref RAIL_DataConfig_t.txMethod and \ref RAIL_DataConfig_t.rxMethod
 * must have the same \ref RAIL_DataMethod_t configuration.
 *
 * @warning Do not call RAIL fifo functions while in \ref
 * RAIL_DataMethod_t::PACKET_MODE.
 * @{
 */

/**
 * RAIL data management configuration
 *
 * @param[in] dataConfig RAIL data configuration structure
 * @return RAIL Status of configuration
 *
 * This function configures how RAIL manages data. The application can
 * configure RAIL to receive data in a packet based or FIFO based format. When
 * configuring tx or rx for fifo mode, this function will reset the configured
 * fifos.
 *
 * If \ref RAIL_DataConfig_t.rxMethod is set to \ref
 * RAIL_DataMethod_t.PACKET_MODE, the radio will filter packets with invalid
 * CRCs by default. This is similar to setting the <b>ignoreErrors</b>
 * parameter in RAIL_SetRxTransitions() to \ref RAIL_IGNORE_NO_ERRORS.
 *
 * If \ref RAIL_DataConfig_t.rxMethod is set to \ref
 * RAIL_DataMethod_t.FIFO_MODE, the radio will accept packets with CRCs as
 * 'valid' packets by default. This is meant to treat 'fully received' packets
 * the same way regardless if CRC passes or fails. The application can parse
 * CRC errors via appended info obtained from RAIL_ReadRxFifoAppendedInfo().
 * This is similar to setting the <b>ignoreErrors</b> parameter in
 * RAIL_SetRxTransitions() to \ref RAIL_IGNORE_CRC_ERRORS.
 *
 * In either situation, the application can set <b>ignoreErrors</b> as needed;
 * in fifo mode, appended info will not be present for frame errors. The
 * defaults defined above are the recommended setting.
 */
RAIL_Status_t RAIL_DataConfig(RAIL_DataConfig_t *dataConfig);

/**
 * Write data to the transmit fifo
 *
 * @param[in] dataPtr Application provided pointer to transmit data
 * @param[in] writeLength Number of bytes to write to the transmit fifo
 *
 * @return The number of bytes written to the transmit fifo
 *
 * This function reads data from the provided dataPtr and writes it to the TX
 * Fifo.  If the requested writeLength exceeds the current number of bytes open
 * in the transmit fifo, the function will only write until the transmit fifo
 * is full. The function returns the number of bytes written to the transmit
 * fifo.
 *
 * @note This function does not create a critical section but depending on the
 * application a critical section could be appropriate.
 */
uint16_t RAIL_WriteTxFifo(uint8_t *dataPtr, uint16_t writeLength);

/**
 * Read data from the receive fifo
 *
 * @param[out] dataPtr Application provided pointer to store data
 * @param[in] readLength Number of bytes to read from the fifo
 *
 * @return The number of bytes read from the receive fifo
 *
 * This function reads data from the receive fifo and writes it to the provided
 * dataPtr. If the requested readLength exceeds the current number of bytes in
 * the receive fifo, the function will only read the current amount of bytes
 * available.
 *
 * This function does not have a critical section, so either use it only in one
 * context or make sure function calls are protected to prevent buffer
 * corruption.
 */
uint16_t RAIL_ReadRxFifo(uint8_t *dataPtr, uint16_t readLength);

/**
 * Read appended info from the receive fifo
 *
 * @param[out] appendedInfo Application provided pointer to store RAIL_AppendedInfo_t
 * @return void
 *
 * This function reads appended info from the receive fifo and writes it to the
 * provided pointer; appended info is added to the receive fifo once a packet is
 * received.  Using this function while not at the end of a packet can corrupt
 * your buffer by processing receive data as appended info.
 *
 * @note The following fields in appended info are not implemented in fifo mode and
 * do not contain valid info:
 * - RAIL_AppendedInfo_t.isAck
 * - RAIL_AppendedInfo_t.lqi
 * - RAIL_AppendedInfo_t.frameCodingStatus (will reflect the last received packet)
 */
void RAIL_ReadRxFifoAppendedInfo(RAIL_AppendedInfo_t *appendedInfo);

/**
 * Configure the RAIL transmit fifo almost empty threshold
 *
 * @param[in] txThreshold Threshold once fallen under
 * will fire RAILCb_TxFifoAlmostEmpty()
 * @return Configured transmit fifo threshold value
 *
 * This function configures the threshold for the transmit fifo. When the count
 * of the transmit fifo is less than the configured threshold,
 * RAILCb_TxFifoAlmostEmpty() will fire. A value of 0 is invalid and will not
 * change the current configuration.
 */
uint16_t RAIL_SetTxFifoThreshold(uint16_t txThreshold);

/**
 * Configure the RAIL receive fifo almost full threshold
 *
 * @param[in] rxThreshold Threshold once exceeded will fire
 * RAILCb_RxFifoAlmostFull()
 * @return Configured receive fifo threshold value
 *
 * This function configures the threshold for the transmit fifo. When the count
 * of the receive fifo is greater than the configured threshold,
 * RAILCb_RxFifoAlmostFull() will fire. A value of 0xFFFF is invalid and will
 * not change the current configuration. Depending on the hardware the maximum
 * value can vary. If the rxThreshold value exceeds the capability of the
 * hardware, the rx threshold will be configured so that it fires only when the
 * FIFO is one byte away from being full.
 *
 */
uint16_t RAIL_SetRxFifoThreshold(uint16_t rxThreshold);

/**
 * Get the RAIL transmit fifo almost empty threshold value
 *
 * @return Configured Tx Threshold value
 *
 * Retrieve the configured tx threshold value
 */
uint16_t RAIL_GetTxFifoThreshold(void);

/**
 * Get the RAIL receive fifo almost full threshold value
 *
 * @return Configured Rx Threshold value
 *
 * Retrieve the configured rx threshold value
 */
uint16_t RAIL_GetRxFifoThreshold(void);

/**
 * Enable the RAIL receive fifo threshold interrupt
 *
 * @return void
 *
 * Enable the RAIL receive fifo threshold interrupt.
 */
void RAIL_EnableRxFifoThreshold(void);

/**
 * Disable the RAIL receive fifo threshold interrupt
 *
 * @return void
 *
 * Disable the RAIL receive fifo threshold interrupt. This is useful if the
 * application wishes to defer reading the receive fifo into another context.
 */
void RAIL_DisableRxFifoThreshold(void);

/**
 * Reset the RAIL Fifos
 *
 * @param[in] txFifo If true, reset the transmit fifo
 * @param[in] rxFifo If true, reset the receive fifo
 * @return void
 *
 * This function can reset each fifo. The application should not reset the Rx
 * Fifo while receiving a frame.
 */
//@TODO interrupt protect when clearing; need to check race conditions with hw team
void RAIL_ResetFifo(bool txFifo, bool rxFifo);

/**
 * Get the number of bytes in the receive fifo
 *
 * @return Number of bytes in the receive fifo
 *
 * Get the number of bytes in the receive fifo
 */
uint16_t  RAIL_GetRxFifoBytesAvailable(void);

/**
 * Get the number of bytes open in the transmit fifo
 *
 * @return Number of bytes open in the transmit fifo
 *
 * Get the number of bytes open in the transmit fifo
 */
uint16_t  RAIL_GetTxFifoSpaceAvailable(void);

/**
 * Callback that fires when the receive fifo exceeds the configured threshold
 * value
 *
 * @param[in] bytesAvailable Number of bytes available in the receive fifo at
 * the time of the callback dispatch
 *
 * @return void
 * @warning You must implement a stub for this in your RAIL application.
 *
 * Callback that fires when the receive fifo exceeds the configured threshold
 * value.  Provides the number of bytes available in the receive fifo at the
 * time of the callback dispatch.
 */
void RAILCb_RxFifoAlmostFull(uint16_t bytesAvailable);

/**
 * Callback that fires when the transmit fifo falls under the configured
 * threshold value
 *
 * @param[in] spaceAvailable Number of bytes open in the transmit fifo at the
 * time of the callback dispatch
 *
 * @return void
 * @warning You must implement a stub for this in your RAIL application.
 *
 * Callback that fires when the transmit fifo falls under the configured
 * threshold value. It only fires if a rising edge occurs across this
 * threshold. This callback will not fire on initailization nor after resetting
 * the transmit fifo with RAIL_ResetFifo().
 *
 * Provides the number of bytes open in the transmit fifo at the time of the
 * callback dispatch.
 */
void RAILCb_TxFifoAlmostEmpty(uint16_t spaceAvailable);
/**
 * @}
 */

/******************************************************************************
 * Timing Information
 *****************************************************************************/
/**
 * @addtogroup System_Timing
 * @brief Functionality related to the RAIL timer and general system time.
 *
 * These functions can be used to get information about the current system time
 * or to manipulate the RAIL timer.
 *
 * The system time returned by RAIL_GetTime() is in the same timebase that is
 * used throughout RAIL. Any callbacks that return a timestamp (like
 * RAILCb_RxPacketReceived()) will use this same timebase as will any APIs that
 * accept an absolute time for scheduling their action. Throughout this
 * documentation the timebase used for this will be referred to as the RAIL
 * timebase. This is currently a value in microseconds from chip boot time. This
 * means that it will wrap every 1.19 hours
 * (`(2^32 - 1) / (3600 sec/hr * 1000000 us/sec)`).
 *
 * The provided timer is hardware backed and interrupt driven. It can be used
 * for timing any event in your system, but will be especially helpful for
 * timing protocol based state machines and other systems that interact with
 * the radio. If you do not want to process the expiration in interrupt context
 * you can leave the RAILCb_TimerExpired() callback empty and poll for
 * expiration with the RAIL_TimerExpired() function.
 *
 * @{
 */

/**
 * Get the current RAIL time
 *
 * @return Returns the RAIL timebase in microseconds. Note that this wraps after
 * around 1.19 hours since it's stored in a 32bit value.
 *
 * Return the current time in the RAIL timebase (microseconds). This can be
 * used to compare with packet timestamps or to schedule transmits.
 */
uint32_t RAIL_GetTime(void);

/**
 * Set the current RAIL time
 *
 * @param[in] time Set the RAIL timebase to this value in microseconds.
 * @return Returns RAIL_STATUS_NO_ERROR on success and
 * RAIL_STATUS_INVALID_STATE if the time could not be set.
 *
 * Set the current time in the RAIL timebase in microseconds.
 */
RAIL_Status_t RAIL_SetTime(uint32_t time);

/**
 * Set a timer via the RAIL timebase
 *
 * @param[in] time The time to delay for in the RAIL timebase.
 * @param[in] mode The timer mode can be relative to now or an absolute time.
 * @return Returns RAIL_STATUS_NO_ERROR on success and
 * RAIL_STATUS_INVALID_PARAMETER if the timer could not be scheduled.
 *
 * Configure a timer to fire after some period in the RAIL timebase. This timer
 * can be used to implement low level protocol features.
 */
RAIL_Status_t RAIL_TimerSet(uint32_t time, RAIL_TimeMode_t mode);

/**
 * Return the absolute time that the RAIL timer is configured to fire at.
 *
 * @return The absolute time that this timer is set to go off at.
 *
 * This will give the absolute time regardless of the \ref RAIL_TimeMode_t that
 * was passed into \ref RAIL_TimerSet. The return value is undefined if the
 * timer was never set.
 */
uint32_t RAIL_TimerGet(void);

/**
 * Stop the currently scheduled RAIL timer.
 *
 * @return void
 *
 * Cancels the timer. If this is called before the timer fires, then
 * RAILCb_TimerExpired will never be called.
 */
void RAIL_TimerCancel(void);

/**
 * Check to see if the RAIL timer has expired
 *
 * @return True if the previously scheduled timer has fired and false otherwise.
 *
 * This is cleared on RAIL_TimerSet() and will be set when the delay expires.
 * This function can be used as an alternative to RAILCb_TimerExpired using
 * polling. If this is the case, implement RAILCb_TimerExpired as a stub.
 */
bool RAIL_TimerExpired(void);

/**
 * See if the RAIL timer is currently running
 *
 * @return Returns true if the timer is running and false otherwise
 *
 * Will return false if the timer was never set or has expired.
 */
bool RAIL_TimerIsRunning(void);

/**
 * This function is called when the RAIL timer expires
 *
 * @return void
 *
 * You must implement a stub for this in your RAIL application even if you
 * don't use the timer. You can use this callback for low-level protocol
 * features.
 */
void RAILCb_TimerExpired(void);

/**
 * @}
 */

/******************************************************************************
 * Radio Configuration
 *****************************************************************************/
/**
 * @addtogroup Radio_Configuration
 * @brief Routines for setting up and querying radio configuration information.
 *
 * All of these routines allow for runtime flexibility in your radio
 * configuration. Some of the parameters, however, are meant to be generated
 * from the radio calculator in Simplicity Studio. The basic code to configure
 * the radio from this calculator output looks like the example below.
 *
 * @code{.c}
 *  // Apply the selected RADIO configuration
 *  if (RAIL_RadioConfig((void*)configList[0])) {
 *    // Error: Could not apply the radio configuration
 *    while(1);
 *  }
 *
 *  // Configure the packet configuration for this PHY
 *  RAIL_PacketLengthConfigFrameType(frameTypeConfigList[0]);
 *
 *  // Set up the channel configuration for this PHY
 *  RAIL_ChannelConfig(channelConfigs[0]);
 * @endcode
 *
 * For more information about the types of parameters that can be changed in
 * the other functions and how to use them see their individual documentation.
 *
 * @{
 */

/**
 * Load a static radio configuration
 *
 * @param[in] radioConfig Pointer to a radio configuration array
 * @return A non-zero value on failure and zero on success
 *
 * The radioConfig passed into this function should be generated for you, and
 * not created or edited by hand.
 */
uint8_t RAIL_RadioConfig(void *radioConfig);

/**
 * Configure the length to use for received packets to be variable based on an
 * implicit length field in payload bytes
 *
 * @param[in] frameType Frame type configuration structure.
 *
 * Currently the frame type passed in only handles packet length decoding. If
 * NULL is passed into this function, it will clear any currently configured
 * frame type settings.
 */
void RAIL_PacketLengthConfigFrameType(const RAIL_FrameType_t *frameType);

/**
 * Configure the channels supported by this device
 *
 * @param[in] config A pointer to the channel configuration for your device.
 *   This pointer will be cached in the library so it must be something that
 *   will exist for the runtime of the application. Typically this should be
 *   what is stored in Flash by the configuration tool.
 * @return Returns first available channel in config.
 *
 * When configuring channels on the EFR32, the Synth will be reconfigured based
 * on the frequency and channel spacing in config.
*/
uint8_t RAIL_ChannelConfig(const RAIL_ChannelConfig_t * config);

/**
 * Check to see if the channel exists in RAIL
 *
 * @param[in] channel Channel number to check
 * @return Returns 1 on failure, returns 0 on channel exists
 *
 * Will return 1 if the given channel does not exist in the channel config
 * currently being used, and 0 if the channel is valid.
 */
RAIL_Status_t RAIL_ChannelExists(uint8_t channel);

/**
 * Return the symbol rate for the current PHY
 *
 * @return The symbol rate in symbols per second
 *
 * The symbol rate is the rate of symbol changes over the air. For non-DSSS
 * PHYs this is the same as the baudrate. For DSSS PHYs it is the baudrate
 * divided by the length of a chipping sequence. For more information on this
 * consult the modem calculator documentation.
 */
uint32_t RAIL_SymbolRateGet(void);

/**
 * Return the bit rate for the current PHY
 *
 * @return The bit rate in bits per second
 *
 * The bit rate is the effective over the air data rate. It does not account
 * for extra spreading you may do for things like forward error correction, but
 * will account for modulation schemes, DSSS, and other configurations. For more
 * information on this consult the modem calculator documentation.
 */
uint32_t RAIL_BitRateGet(void);

/**
 * Set the PA capacitor tune value for transmit and receive
 *
 * @param[in] txPaCtuneValue PA Ctune value for TX mode
 * @param[in] rxPaCtuneValue PA Ctune value for RX mode
 *
 * @return returns RAIL_STATUS_NO_ERROR if successful
 *
 * Provides the ability to tune the impedance of the transmit
 * and receive modes by changing the amount of capacitance at
 * the PA output.
 */
RAIL_Status_t RAIL_PaCtuneSet(uint8_t txPaCtuneValue, uint8_t rxPaCtuneValue);

/**
 * @}
 */

/******************************************************************************
 * Transmit
 *****************************************************************************/
/**
 * @addtogroup Transmit
 * @brief APIs related to transmitting data packets
 * @{
 */

/**
 * Set the radio transmit power level
 *
 * @param[in] powerLevel TX Power Level defined in deci dBm (10 * dBm)
 * @return TX Power Level in deci dBm (10 * dBm)
 *
 * Not all values of powerLevel are achievable, but this function will set the
 * power output to be close to the given powerLevel, and return the value that
 * was set as the power.
 */
int32_t RAIL_TxPowerSet(int32_t powerLevel);

/**
 * Get the radio transmit power level
 *
 * @return TX Power Level defined in deci dBm (10 * dBm)
 *
 * This will return what the power output was actually set to, not just the
 * value passed into RAIL_TxPowerSet.
 */
int32_t RAIL_TxPowerGet(void);

/**
 * Configure which radio transmit actions trigger callbacks
 *
 * @param[in] cbToEnable Define which callbacks to trigger for transmit events.
 *  The full list of available callabcks can be found by looking at the
 *  RAIL_TX_CONFIG_* set of defines.
 * @return Return 0 for success or an error code
 *
 * Setup which receive interrupts will generate a RAILCb_TxRadioStatus()
 * callback. The full list of options is any define that starts with
 * RAIL_TX_CONFIG_. Before this function is called, the actions which will
 * generate callbacks are:
 * - \ref RAIL_TX_CONFIG_BUFFER_UNDERFLOW
 * - \ref RAIL_TX_CONFIG_CHANNEL_BUSY
 * - \ref RAIL_TX_CONFIG_TX_ABORTED
 * - \ref RAIL_TX_CONFIG_TX_BLOCKED
 */
RAIL_Status_t RAIL_TxConfig(uint32_t cbToEnable);

/**
 * Load payload to transmit.
 *
 * @param[in] txData Pointer to a RAIL_TxData_t structure which defines the
 * payload bytes and the number of bytes to write into the transmit buffer.
 * @return Returns 0 on success and an error code on fail.
 *
 * This function will overwrite current TX data held by RAIL, and will return
 * an error if called during transmit operations. RAIL_TxData_t.dataLength
 * defines the number of bytes to load into the transmit buffer from
 * RAIL_TxData_t.dataPtr while the number of bytes transmitted is determined by
 * the packet configuration defined in the radio configuration.
 *
 * @note This function creates a critical section while writing to the transmit
 * buffer.
 */
uint8_t RAIL_TxDataLoad(RAIL_TxData_t *txData);

/**
 * Non-blocking Transmit
 *
 * @param[in] channel Define the channel to transmit on.
 * @param[in] preTxOp Function to use for any pre-transmit operation (e.g. for
 * scheduled transmit, CSMA, LBT, etc.), or NULL for an immediate transmit.
 * @param[in] preTxOpParams Pointer to the pre-transmit operation's
 * configuration parameters, or NULL if none.
 * @return Returns 0 on successfully initiating the transmit process, or an
 * error code on failure. If successfully initiated, transmit completion
 * or failure will be reported by later callbacks RAILCb_TxPacketSent()
 * (success) or RAILCb_TxRadioStatus() (failure).
 *
 * Begins transmission of the payload previously loaded via RAIL_TxDataLoad().
 * Will begin transmitting after a received packet if currently receiving a
 * packet. Returns error if the radio is active and the channel needs to be
 * changed.
 */
uint8_t RAIL_TxStart(uint8_t channel,
                     RAIL_PreTxOp_t preTxOp,
                     void *preTxOpParams);

/**
 * Non-blocking Transmit with options
 *
 * @param[in] channel Define the channel to transmit on.
 * @param[in] options Defines options that apply for this transmit
 * @param[in] preTxOp Function to use for any pre-transmit operation (e.g. for
 * scheduled transmit, CSMA, LBT, etc.), or NULL for an immediate transmit.
 * @param[in] preTxOpParams Pointer to the pre-transmit operation's
 * configuration parameters, or NULL if none.
 * @return Returns 0 on successfully initiating the transmit process, or an
 * error code on failure.  If successfully initiated, transmit completion
 * or failure will be reported by later callbacks RAILCb_TxPacketSent()
 * (success) or RAILCb_TxRadioStatus() (failure).
 *
 * This is an extension of RAIL_TxStart where the transmit is modified by the
 * options defined in RAIL_TxOptions_t. If using a pre-tx operation, the
 * transmit options will only be configured if the preTxOp is successful.
 *
 * Begins transmission of the payload previously loaded via RAIL_TxDataLoad().
 * Will begin transmitting after a received packet if currently receiving a
 * packet. Returns error if the radio is active and the channel needs to be
 * changed.
 */
uint8_t RAIL_TxStartWithOptions(uint8_t channel,
                                RAIL_TxOptions_t *options,
                                RAIL_PreTxOp_t preTxOp,
                                void *preTxOpParams);

/**
 * Interrupt level callback to signify when the packet was sent
 *
 * @param txPacketInfo Information about the packet that was transmitted.
 * @note that this structure is only valid during the timeframe of the
 * callback.
 *
 * Currently the RAIL_TxPacketInfo_t only contains the time when the packet
 * was transmitted.
 */
void RAILCb_TxPacketSent(RAIL_TxPacketInfo_t *txPacketInfo);

/**
 * Callback to indicate an error with a transmission
 *
 * @param[in] status A bit field that defines what event caused the callback
 *
 * This interrupt level callback allows the user finer granularity in handling
 * TX radio errors.
 *
 * Radio Statuses:
 * - \ref RAIL_TX_CONFIG_BUFFER_UNDERFLOW
 * - \ref RAIL_TX_CONFIG_CHANNEL_BUSY
 * - \ref RAIL_TX_CONFIG_TX_ABORTED
 * - \ref RAIL_TX_CONFIG_TX_BLOCKED
 * - \ref RAIL_TX_CONFIG_CHANNEL_CLEAR
 * - \ref RAIL_TX_CONFIG_CCA_RETRY
 * - \ref RAIL_TX_CONFIG_START_CCA
 */
void RAILCb_TxRadioStatus(uint8_t status);


/******************************************************************************
 * Pre-Transmit Operations
 *****************************************************************************/
/**
 * @addtogroup Pre-Transmit
 * @brief APIs for pre-transmit operations (Scheduling, CSMA, LBT, ...)
 *
 * There are many operation that you can want to happen before a transmit. In
 * RAIL these are configurable via Pre-Transmit hooks. You are free to use your
 * own custom hooks, but there are several provided hooks to help with common
 * use cases. The provided hooks will use the hardware as efficiently as
 * possible which typically means that they do not introduce any software
 * time overhead.
 *
 * Here's a simple example of how to use a scheduled transmit to send a packet
 * 1 ms after right now.
 *
 * @code{.c}
 * RAIL_ScheduleTxConfig_t nextPacketTxTime = { 1000, RAIL_TIME_DELAY };
 * txStatus = RAIL_TxStart(channel, &RAIL_ScheduleTx, &nextPacketTxTime);
 * @endcode
 *
 * @{
 */

/**
 * Send a packet on a schedule, instead of immediately
 *
 * @param[in] params A pointer to the RAIL_ScheduleTxConfig_t
 * structure containing when the transmit should occur.
 * @return - Returns 0 on success and anything else on error.
 *
 * A RAIL_PreTxOp_t function that schedules the transmit to occur at the
 * specified absolute or relative time within a RAIL_TxStart() transmit
 * operation. If RAIL is receiving a packet at the scheduled time, the transmit
 * will be delayed until after the packet is received. To guarantee the time of
 * the outgoing transmit, only call this function while the radio is idle.
 */
uint8_t RAIL_ScheduleTx(void *params);

/**
 * Use CSMA instead of ignoring current usage of the channel
 *
 * @param[in] params A pointer to the RAIL_CsmaConfig_t structure containing
 * the CSMA parameters to use.
 * @return - Returns 0 on success and anything else on error.
 *
 * A RAIL_PreTxOp_t function that performs the CSMA algorithm when specified
 * within a RAIL_TxStart() transmit operation. Packets can be received during
 * CSMA backoff periods if receive is active throughout the CSMA process. This
 * will happen either by starting the CSMA process while receive is already
 * active, or if the ccaBackoff time in the RAIL_CsmaConfig_t is less than the
 * idleToRx time (set by RAIL_SetStateTimings). If the ccaBackoff time is
 * greater than the idleToRx time, then receive will only be active during the
 * clear channel assessments.
 */

uint8_t RAIL_CcaCsma(void *params);

/**
 * Listen to the channel before sending a message
 *
 * @param[in] params A pointer to the RAIL_LbtConfig_t structure containing
 * the LBT parameters to use.
 * @return Returns 0 on success and anything else on error.
 *
 * A RAIL_PreTxOp_t function that performs the LBT algorithm when specified
 * within a RAIL_TxStart() transmit operation. Packets can be received during
 * CSMA backoff periods if receive is active throughout the LBT process. This
 * will happen either by starting the LBT process while receive is already
 * active, or if the lbtBackoff time in the RAIL_LbtConfig_t is less than the
 * idleToRx time (set by RAIL_SetStateTimings). If the lbtBackoff time is
 * greater than the idleToRx time, then receive will only be active during the
 * clear channel assessments.
 */
uint8_t RAIL_CcaLbt(void *params);

/**
 * Sets the CCA threshold in dBm
 *
 * @param[in] ccaThresholdDbm CCA threshold in dBm.
 * @return \ref RAIL_STATUS_NO_ERROR on success.
 *
 * A RAIL_PreTxOp_t function will normally set CCA threshold, assuming it is
 * enabled either in LBT or CSMA mode. Unlike RAIL_CcaCsma and RAIL_CcaLbt,
 * which are called as RAIL_PreTxOp_t functions, this function only modifies
 * CCA threshold. A possible usecase for this function is to set CCA threshold
 * to invalid RSSI of -128 which disables transmission by canceling
 * the current CCA check.
 *
 */
RAIL_Status_t RAIL_SetCcaThreshold(int8_t ccaThresholdDbm);

/**
 * end of group Pre-Transmit
 * @}
 */

/**
 * end of group Transmit
 * @}
 */

/******************************************************************************
 * Receive
 *****************************************************************************/
/**
 * @addtogroup Receive
 * @brief APIs related to packet receive
 * @{
 */

/**
 * Configure radio receive actions
 *
 * @param[in] cbToEnable Define which callbacks to trigger for receive events.
 *  The full list of available callabcks can be found by looking at the
 *  RAIL_RX_CONFIG_* set of defines.
 * @param[in] appendedInfoEnable Enable/Disable appended info (not implemented)
 * @return Return 0 for success or an error code
 *
 * Setup which receive interrupts will generate a RAILCb_RxRadioStatus()
 * callback. The full list of options is any define that starts with
 * RAIL_RX_CONFIG_.
 */
uint8_t RAIL_RxConfig(uint32_t cbToEnable, bool appendedInfoEnable);

/**
 * Configure receive options
 *
 * @param[in] options Bitfield of options which affect recieve. The available
 *  options begin with RAIL_RX_OPTION.
 * @return Return 0 for success or an error code
 *
 * Configure the radio receive flow, based on the list of available options.
 * This will fail with RAIL_STATUS_INVALID_STATE if a packet is being received
 * during this configuration.
 */
RAIL_Status_t RAIL_SetRxOptions(uint32_t options);

/**
 * Listen on a channel for a packet
 *
 * @param[in] channel Channel to listen on
 * @return Return 0 for success or an error code
 *
 * This is a non-blocking function. RAILCb_RxPacketReceived() will be called
 * when a packet has been received. Returns an error if currently transmitting
 * or receiving.
 */
uint8_t RAIL_RxStart(uint8_t channel);

/**
 * Schedule a receive window for some time in the future.
 *
 * @param[in] channel Channel to listen on
 * @param[in] cfg The configuation struct to define the receive window.
 * @return Return 0 on success or an error code
 *
 * This API will immediately change your channel and schedule receive to start
 * at the specified time and end at the given end time. If you do not specify an
 * end time then you may call this API later with an end time as long as you set
 * the start time to disabled. You can also terminate the whole receive
 * operation immediately using the RAIL_RfIdle() function. Note that relative
 * end times are always relative to the start unless there is no start time
 * specified.
 */
uint8_t RAIL_ScheduleRx(uint8_t channel, RAIL_ScheduleRxConfig_t *cfg);

/**
 * Return the current raw RSSI
 *
 * @return Return \ref RAIL_RSSI_INVALID if the receiver is disabled and we are
 * unable to get an RSSI value, otherwise, return the RSSI in quarter dBm,
 * dbm*4.
 *
 * Get the current RSSI value. This value represents the current energy of the
 * channel, so it can change rapidly, and will be low if there is no RF energy
 * in your current channel. The function from the value reported to dBm is an
 * offset dependent on the PHY and the PCB layout. Users should characterize the
 * RSSI received on their hardware and apply an offset in the application to
 * account for board and PHY parameters.
 */
int16_t RAIL_RxGetRSSI(void);

/**
 * Compute the average RSSI over a specified time in us
 *
 * @param[in] averageTimeUs Averaging time in microseconds.
 * @return Return \ref RAIL_RSSI_INVALID if the receiver is disabled and we are
 * unable to get an RSSI value, otherwise, return the RSSI in quarter dBm,
 * dbm*4.
 *
 * This blocking function will poll the hardware for RSSI values and compute
 * the average RSSI over the requested time period. If no valid readings have
 * been made function will return \ref RAIL_RSSI_INVALID reading. Receiving a
 * packet during the averaging will cause invalid reading(s). However, invalid
 * readings during the averaging will not be included in the average. Number of
 * RSSI readings per baud depends on the phy.
 */
int16_t RAIL_PollAverageRSSI(uint32_t averageTimeUs);

/**
 * Start the RSSI averaging over specified time in us
 *
 * @param[in] channel The physical channel to set
 * @param[in] averagingTimeUs Averaging time in microseconds.
 * @return Returns 0 on success, error code on error.
 *
 * Start a non-blocking hardware based RSSI averaging mechanism. Only a single
 * instance of RSSI averaging can be run at any time and the radio must be idle
 * to start.
 */
RAIL_Status_t RAIL_StartAverageRSSI(uint8_t channel, uint32_t averagingTimeUs);

/**
 * Queries whether the RSSI averaging is done
 *
 * @return Returns true if done and false otherwise.
 *
 * This function can be used to poll for completion of the RSSI averaging so
 * that you do not have to rely on an interrupt based callback.
 */
bool RAIL_AverageRSSIReady(void);

/**
 * Get the RSSI averaged over specified time in us
 *
 * @return Return \ref RAIL_RSSI_INVALID if the receiver is disabled and we are
 * unable to get an RSSI value, otherwise, return the RSSI in quarter dBm,
 * dbm*4.
 *
 * Get the hardware RSSI average after issuing RAIL_StartAverageRSSI. Should be
 * used after RAIL_StartAverageRSSI.
 */
int16_t RAIL_GetAverageRSSI(void);

/**
 * Callback for when AGC averaged RSSI is done
 *
 * @param avgRssi Contains the the RSSI in quarter dBm (dbm*4) on success and
 * returns \ref RAIL_RSSI_INVALID if there was a problem computing the result.
 *
 * Called in response to RAIL_StartAverageRSSI() to indicate that the hardware
 * has completed averaging. If you would like you can instead use the
 * RAIL_AverageRSSIReady() to wait for completion and RAIL_GetAverageRSSI() to
 * get the result.
 */
void RAILCb_RssiAverageDone(int16_t avgRssi);

/**
 * Receive packet callback.
 *
 * @param[in] rxPacketHandle Contains a handle that points to the memory that
 *   the packet was stored in. This handle will be the same as something
 *   returned by the RAILCb_AllocateMemory() API. This handle will hold a
 *   RAIL_RxPacketInfo_t structure starting at offset 0 in the buffer.
 *
 * This function is called whenever a packet is received and returns to you the
 * memory handle for where this received packet and its appended information was
 * stored. After this callback is done we will release the memory handle so you
 * must somehow increment a reference count or copy the data out within this
 * function.
 *
 * If \ref RAIL_IGNORE_CRC_ERRORS is set, this callback will fire for packets
 * with crc errors as well.
 */
void RAILCb_RxPacketReceived(void *rxPacketHandle);

/**
 * Called whenever an enabled radio status event occurs
 *
 * @param[in] status The event that triggered this callback
 *
 * The triggers that cause this function to be called can be enabled using the
 * RAIL_RxConfig() function.
 *
 * @note This function will return only the first 8 of all possible triggers.
 * For accessing all triggers see the new RAILCb_RxRadioStatusExt() API. If you
 * implement RAILCb_RxRadioStatusExt() this callback will no longer be used by
 * the RAIL library. In RAIL 2.0 this API will be merged with the
 * RAILCb_RxRadioStatusExt() for one clean interface.
 *
 * Triggers:
 *  - \ref RAIL_RX_CONFIG_PREAMBLE_DETECT
 *  - \ref RAIL_RX_CONFIG_SYNC1_DETECT
 *  - \ref RAIL_RX_CONFIG_SYNC2_DETECT
 *  - \ref RAIL_RX_CONFIG_FRAME_ERROR
 *  - \ref RAIL_RX_CONFIG_BUFFER_OVERFLOW
 *  - \ref RAIL_RX_CONFIG_ADDRESS_FILTERED
 *  - \ref RAIL_RX_CONFIG_RF_SENSED
 */
void RAILCb_RxRadioStatus(uint8_t status);

/**
 * Called whenever an enabled radio status event occurs
 *
 * @param[in] status The event or events that triggered this callback
 *
 * The triggers that cause this function to be called can be enabled using the
 * RAIL_RxConfig() function. This function is the same as RAILCb_RxRadioStatus()
 * with an extended set of triggers. For backwards compatibility this function
 * is weakly defined in the RAIL library to call RAILCb_RxRadioStatus() with the
 * subset of valid events. If you need more events you must implement this
 * version which will stop the old one from being called.
 *
 * @note In RAIL 2.0 this API will be merged with the RAILCb_RxRadioStatus() for
 * one clean interface.
 *
 * Triggers:
 *  - \ref RAIL_RX_CONFIG_PREAMBLE_DETECT
 *  - \ref RAIL_RX_CONFIG_SYNC1_DETECT
 *  - \ref RAIL_RX_CONFIG_SYNC2_DETECT
 *  - \ref RAIL_RX_CONFIG_FRAME_ERROR
 *  - \ref RAIL_RX_CONFIG_BUFFER_OVERFLOW
 *  - \ref RAIL_RX_CONFIG_ADDRESS_FILTERED
 *  - \ref RAIL_RX_CONFIG_RF_SENSED
 *  - \ref RAIL_RX_CONFIG_TIMEOUT
 *  - \ref RAIL_RX_CONFIG_SCHEDULED_RX_END
 *  - \ref RAIL_RX_CONFIG_PACKET_ABORTED
 */
void RAILCb_RxRadioStatusExt(uint32_t status);

/******************************************************************************
 * Address Filtering (Rx)
 *****************************************************************************/
/**
 * @addtogroup Address_Filtering
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
 * reference the specific sections in the packet that will each be interpreted
 * as an address during address filtering. The application may submit up to four
 * addresses to attempt to match each field segment and each address may have a
 * size of up to 8 bytes. To setup
 * address filtering you must first configure where the addresses are in your
 * packet and how long they are. Next, you need to configure what combinations
 * of matches in Field0 and Field1 should constitute an address match. Lastly,
 * you need to enter addresses into the tables for each field and enable them.
 * The first two of these are part of the RAIL_AddrConfig_t structure while the
 * second part is configured at runtime using the RAIL_AddressFilterSetAddress()
 * API. A brief description of each of these configurations is listed below.
 *
 * For the first piece of configuration, the offsets and sizes of the fields are
 * assumed to be fixed for the RAIL address filter. To set them you must specify
 * arrays for these values in the sizes and offsets entries in the
 * RAIL_AddrConfig_t struct. A size of zero will indicate that a field is
 * disabled.  The start offset for a field is relative to the previous start
 * offset and if you're using FrameType decoding the first start offset is
 * relative to the end of the byte containing the frame type.
 *
 * Configuring which combinations of Field0 and Field1 constitute a match is the
 * most complex portion of the address filter. The easiest way to think about
 * this is with a truth table. If you consider each of the four possible address
 * entries in a field then you can have a match on any one of those or a match
 * for none of them. We can represent this as a 4 bit mask where a 1 indicates a
 * match and a 0 indicates no match. If we then show the Field0 match options as
 * rows and the Field1 options as columns we get a truth table like the one
 * shown below.
 *
 * |          | 0000 | 0001 | 0010 | 0100 | 1000 |
 * |----------|------|------|------|------|------|
 * | __0000__ | bit0 | bit1 | bit2 | bit3 | bit4 |
 * | __0001__ | bit5 | bit6 | bit7 | bit8 | bit9 |
 * | __0010__ | bit10| bit11| bit12| bit13| bit14|
 * | __0100__ | bit15| bit16| bit17| bit18| bit19|
 * | __1000__ | bit20| bit21| bit22| bit23| bit24|
 *
 * Since this is only 25 bits it can be represented in one 32bit integer where a
 * 1 indicates filter pass and a 0 indicates filter fail. This is the matchTable
 * parameter in the configuration struct and it is what's used during filtering.
 * For common simple configurations we provide two defines, the truth tables for
 * which are shown below. The first is \ref
 * ADDRCONFIG_MATCH_TABLE_SINGLE_FIELD and it can be used if you're only using
 * one address field (either field). If you're using two fields and want to
 * force in the same address entry in each field you can use second define: \ref
 * ADDRCONFIG_MATCH_TABLE_DOUBLE_FIELD. For more complex systems you'll have to
 * create a valid table on your own.
 *
 * @note Address filtering does not function properly with PHYs that use a data
 * rate greater than 500kbps. If you require this you must filter in software
 * for the time being.
 *
 * @{
 */

/**
 * Configure address filtering.
 *
 * @param addrConfig The configuration structure which defines how addresses
 * are setup in your packets.
 * @return True if we were able to configure address filtering and false
 * otherwise.
 *
 * This function must be called to setup address filtering. You may call this
 * multiple times, but all previous information is wiped out each time you call
 * it so any configured addresses must be reset.
 */
bool RAIL_AddressFilterConfig(RAIL_AddrConfig_t *addrConfig);

/**
 * Enable address filtering.
 *
 * @return True if address filtering was enabled to start with and false
 * otherwise
 *
 * Only allow packets through that pass the current address filtering
 * configuration. This will not reset or change the configuration so you can
 * set that up before turning this feature on.
 */
bool RAIL_AddressFilterEnable(void);

/**
 * Disable address filtering.
 *
 * @return True if address filtering was enabled to start with and false
 * otherwise
 *
 * Allow all packets through regardless of addressing information. This will not
 * reset or change the current configuration.
 */
bool RAIL_AddressFilterDisable(void);

/**
 * Return whether address filtering is currently enabled.
 *
 * @return True if address filtering is enabled and false otherwise
 */
bool RAIL_AddressFilterIsEnabled(void);

/**
 * Reset the address filtering configuration.
 *
 * Reset all structures related to address filtering. This will not disable
 * address fitlering. It will leave the radio in a state where no packets will
 * pass filtering.
 */
void RAIL_AddressFilterReset(void);

/**
 * Set an address for filtering in hardware.
 *
 * @param field Which address field you want to use for this address
 * @param index Which match entry you want to place this address in for the
 * given field.
 * @param value A pointer to the address data. This must be at least as long
 * as the size specified in RAIL_AddressFilterConfig().
 * @param enable A boolean to indicate whether this address should be enabled
 * immediately.
 * @return True if we were able to set this address and false otherwise.
 *
 * This function will load the given address into hardware for filtering and
 * start filtering on it if you set the enable parameter to true. Otherwise,
 * you must call RAIL_AddressFilterEnableAddress() to turn it on later.
 */
bool RAIL_AddressFilterSetAddress(uint8_t field,
                                  uint8_t index,
                                  uint8_t *value,
                                  bool enable);

/**
 * Enable address filtering for the specified address
 *
 * @param field Which address field you want to enable the address within
 * @param index Which match entry in the given field you want to enable
 * @return True if we were able to enable filtering for this address and false
 * otherwise.
 */
bool RAIL_AddressFilterEnableAddress(uint8_t field, uint8_t index);

/**
 * Disable address filtering for the specified address
 *
 * @param field Which address field you want to disable the address within
 * @param index Which match entry in the given field you want to disable
 * @return True if this address disabled successfully and false otherwise.
 *
 * This will clear the matchMask set in the RAIL_AddressFilterEnableAddress()
 * function and make sure that this address is marked as valid. To use it in
 * filtering again you must enable this address again.
 */
bool RAIL_AddressFilterDisableAddress(uint8_t field, uint8_t index);

/**
 * Configure address filtering based on frame type
 *
 * @param validFrames The frames on which to enable address filtering. Each bit
 * corresponds to a frame, where a 1 means to enable address filtering during
 * that frame, and a 0 means to ignore addresses during that frame. The least
 * significant bit corresponds to frame 0, and the most significant bit to
 * frame 7.
 * @return True if configuration was set properly, false otherwise
 *
 * This function only takes effect if frame type length decoding and address
 * filtering are both being used. In that case, this function gives the ability
 * to only enable address filtering on certain types of frames.
 *
 * @note This function must be called after RAIL_AddressFilterConfig for it to
 * take effect.
 */
bool RAIL_AddressFilterByFrameType(uint8_t validFrames);

/**
 * end of group Address_Filtering
 * @}
 */

/**
 * end of group Receive
 * @}
 */

/******************************************************************************
 * Auto Acking
 *****************************************************************************/
/**
 * @addtogroup Auto_Ack
 * @brief APIs for configuring Auto-Ack functionality
 *
 * These APIs are used to configure the radio for auto acknowledgement
 * features.  Auto ack inherently changes how the underlying state machine
 * behaves so users should not modify RAIL_SetRxTransitions() and
 * RAIL_SetTxTransitions() while using auto ack features.
 *
 * @code{.c}
 * // Go to RX after ack operation, 100 us idle->rx/tx,
 * // 192 us rx->tx/tx->rx, 1000 us ack timeout
 * RAIL_AutoAckConfig_t autoAckConfig = { RAIL_RF_STATE_RX, 100, 192, 1000};
 *
 * RAIL_Status_t status = RAIL_AutoAckConfig(&autoAckConfig);
 *
 * uint8_t ackPayload[] = {0x05, 0x02, 0x10, 0x00};
 * RAIL_AutoAckData_t ackData = {ackPayload, sizeof(ackPayload)};
 *
 * RAIL_Status_t status = RAIL_AutoAckLoadBuffer(&ackData);
 * @endcode
 *
 * The acknowledgement will transmit based on the frame format configured via
 * the Radio Configurator. For example, if the frame format is using a variable
 * length scheme, the ack will be sent according to that scheme. If a 10 byte
 * packet is loaded into the ack, but the variable length field of the ack
 * payload specifies a length of 5, only 5 bytes will transmit for the ack.
 * The converse is also true, if the frame length is configured to be a fixed
 * 10 byte packet but only 5 bytes are loaded into the ack buffer then a TX
 * underflow will occur during the ack transmit.
 *
 * When auto ack is enabled, the default operation is to transmit the ack after
 * a receive and wait for an ack after a transmit. After the ack operation
 * completes, the radio will transition to the configured defaultState. If
 * there is a desire to not auto acknowledge a series of packets after transmit
 * or receive, call RAIL_AutoAckTxPause() and RAIL_AutoAckRxPause(). When
 * auto acking is paused, after successfully receiving or transmitting a
 * packet, the radio will transition to the defaultState. To get out of a
 * paused state and resume auto acking, call RAIL_AutoAckTxResume() or
 * RAIL_AutoAckRxResume().
 *
 * Applications can cancel the transmission of an ack with
 * RAIL_AutoAckCancelAck(). Conversly, applications can control if a transmit
 * operation should wait for an ack after transmitting by using
 * RAIL_TxStartWithOptions() and populating the waitForAck field in
 * \ref RAIL_TxOptions_t.
 *
 * @code{.c}
 * void RAILCb_RxPacketReceived(void *rxPacketHandle)
 * {
 *   RAIL_RxPacketInfo_t rxPacketInfo = (RAIL_RxPacketInfo_t)rxPacketHandle;
 *
 *   // If we have just received an ACK, don't respond with an ACK
 *   if (rxPacketInfo->dataPtr[2] == 0xF1)
 *   {
 *     RAIL_AutoAckCancelAck();
 *   }
 * }
 *
 * void transmitAndWaitForAck (void)
 * {
 *   RAIL_TxOptions_t txOption;
 *   txOption.waitForAck = true;
 *   RAIL_Status_t status = RAIL_TxStartWithOptions(0, &txOption, NULL, NULL);
 * }
 * @endcode
 *
 * If the ack payload is dynamic, the application must call
 * RAIL_AutoAckLoadBuffer() with the appropriate ack payload after the
 * application processes the receive. RAIL can auto ack from the normal
 * transmit buffer if RAIL_AutoAckUseTxBuffer() is called before the radio
 * transmits the ack.  Make sure the transmit buffer contains data loaded by
 * RAIL_TxDataLoad().
 *
 * Standards based protocols that contain auto ack functionality are normally
 * configured in the protocol specific config function. For example,
 * RAIL_IEEE802154_Init() provides auto ack configuration parameters in \ref
 * RAIL_IEEE802154_Config_t and should only be configured through that
 * function. It is unadvised to call both RAIL_IEEE802154_Init() and
 * RAIL_AutoAckConfig(). However, ack modification functions are still valid to
 * use with protocol specific acks. To cancel a IEEE 802.15.4 ack transmit, use
 * RAIL_AutoAckCancelAck().
 *
 * @{
 */

/**
 * Disable Automatic Acknowledgement
 *
 * @return if function successfully disabled auto acking
 *
 * Disable auto ack functionality. All state transitions are reverted to IDLE,
 * IDLE.
 */
RAIL_Status_t RAIL_AutoAckDisable(void);

/**
 * Return the enable status of the auto ack feature
 *
 * @return true if Auto Ack is enabled, false if disabled
 */
bool RAIL_AutoAckIsEnabled(void);

/**
 * Configure and enable Auto Acknowledgement
 *
 * @param[in] config Auto ack config structure
 * @return If autoack is successfully enabled
 *
 * Configures the RAIL state machine to for hardware accelerated auto
 * acknowledgement. Ack timing parameters are defined in the configuration
 * structure.
 *
 * While auto acking is enabled do not call the following RAIL functions:
 *   - RAIL_SetRxTransitions()
 *   - RAIL_SetTxTransitions()
 *   - RAIL_SetStateTiming()
 */
RAIL_Status_t RAIL_AutoAckConfig(RAIL_AutoAckConfig_t *config);

/**
 * Load Auto Ack buffer with ack data
 *
 * @param[in] ackData Pointer to ack data to transmit
 * @return \ref RAIL_STATUS_INVALID_CALL if called while ACK buffer is being
 * used by the radio
 *
 * If the ack buffer is available to be updated, load the ack buffer with data.
 */
RAIL_Status_t RAIL_AutoAckLoadBuffer(RAIL_AutoAckData_t *ackData);

/**
 * Pause RX Auto Ack functionality.
 *
 * @return void
 *
 * When RX Auto Acking is paused, the radio will transition to the defaultState
 * after receiving a packet and will not transmit an ack.
 *
 */
void RAIL_AutoAckRxPause(void);

/**
 * Resume Rx Auto Ack functionality.
 *
 * @return void
 *
 * When Rx Auto Ack is resumed, the radio will resume automatically acking
 * every successfully received packet.
 */
void RAIL_AutoAckRxResume(void);

/**
 * Return if Rx Auto Ack is paused
 *
 * @return true if Rx Auto Ack is paused, false if not paused
 */
bool RAIL_AutoAckRxIsPaused(void);

/**
 * Resume Tx Auto Ack functionality.
 *
 * @return void
 *
 * When Tx Auto Ack is resumed, the radio will resume automatically waiting for
 * an ack after a successful transmit.
 */
void RAIL_AutoAckTxResume(void);

/**
 * Pause TX Auto Ack functionality.
 *
 * @return void
 *
 * When TX Auto Acking is paused, the radio will transition to the defaultState
 * after transmitting a packet and will not wait for an ack.
 *
 */
void RAIL_AutoAckTxPause(void);

/**
 * Return if Tx Auto Ack is paused
 *
 * @return true if Tx Auto Ack is paused, false if not paused
 */
bool RAIL_AutoAckTxIsPaused(void);

/**
 * Modify the upcoming ack to use the TX Buffer
 *
 * @return True if the ack is modified to send from TX buffer, false if it is
 * too late to switch to tx buffer or if the function call is not valid
 *
 * This function allows the application to use the normal TX buffer as the data
 * source for the upcoming ack. The ack modification to use the TX buffer only
 * applies to one ack transmission.
 *
 * This function will only return true if the following conditions are met:
 *   - Radio has not already decided to use the ack buffer AND
 *   - Radio is either looking for sync, receiving the packet after sync or in
 *     the Rx2Tx turnaround before the ack is sent.
 */
bool RAIL_AutoAckUseTxBuffer(void);

/**
 * Cancel the upcoming ack
 *
 * @return True if the ack is successfully cancelled, false if it is
 * too late to cancel the ack or if the function call is not valid
 *
 * This function allows the application to use cancel the upcoming automatic
 * acknowledgement.
 *
 * This function will only return true if the following conditions are met:
 *   - Radio has not already decided to transmit the ack AND
 *   - Radio is either looking for sync, receiving the packet after sync or in
 *     the Rx2Tx turnaround before the ack is sent.
 */
bool RAIL_AutoAckCancelAck(void);

/**
 * Return if the radio is currently waiting for an ack
 *
 * @return True if radio is waiting for ack, False if radio is not waiting for
 * an ack
 *
 * This function allows the application to query if the radio is currently
 * waiting for an ack after a transmit operation.
 */
bool RAIL_AutoAckWaitingForAck(void);

/**
 * Callback that notifies the application when searching for an ACK has timed
 * out.
 *
 * @return void
 *
 * This callback function is called whenever the timeout for searching for an
 * ack is exceeded.
 */
void RAILCb_RxAckTimeout(void);

/**
 * @} endof Auto_Acking
 */

/******************************************************************************
 * Calibration
 *****************************************************************************/
/**
 * @addtogroup Calibration
 * @brief APIs for calibrating the radio
 * @{
 *
 * These APIs can be used to calibrate the radio. The RAIL library will
 * determine what calibrations are necessary to be performed. Calibrations can
 * be enabled/disabled in RAIL_Init_t.calEnable.
 *
 * Some calibrations produce values that can be saved and reapplied to
 * save repetition of the calibration process. RAIL_CalValues_t is the
 * structure to communicate this value between RAIL and the application.
 */

/**
 * Initialize RAIL Calibration
 *
 * @param[in] railCalInit The initialization structure to be used for setting
 *   up calibration procedures.
 * @return Returns zero on success and an error code on error.
 *
 * Calibration initialization provides the calibration settings that
 * correspond to the current radio configuration.
 */
uint8_t RAIL_CalInit(const RAIL_CalInit_t *railCalInit);

/**
 * Start the calibration process
 *
 * @param[in] calValues Calibration Values to apply. To force the calibration
 * algorithm to run set the value to \ref RAIL_CAL_INVALID_VALUE.
 * @param[in] calForce  Mask to force certain calibration(s) to execute. These
 * will run even if not enabled during initialization. If specified, only forced
 * calibrations will be run.
 * @param[in] calSave If true, we will update any invalid values in calValues
 * with their computed value. You can use this to save calibrations across runs.
 *
 * This function begins the calibration process while determining which
 * calibrations should be performed. The possible list of calibration options
 * are configured in RAIL_Init_t.calEnable parameter.
 *
 * If the calibration was performed previously and the application saves off
 * the calibration value, it can be passed into function and applied to the
 * chip. If the calibration value provided is \ref RAIL_CAL_INVALID_VALUE then
 * the calibration will be performed to set this value. If calSave is set, the
 * calibration output will update  the pointer's value. If a NULL pointer is
 * passed in all calibrations requested/required will be performed and the
 * results will not be saved regardless of the calSave parameter.
 *
 * @note Some calibrations should only be executed when the radio is IDLE. See
 * chip-specific documentation for more detail.
 */
void RAIL_CalStart(RAIL_CalValues_t *calValues, RAIL_CalMask_t calForce, bool calSave);

/**
 * Returns the current set of pending calibrations
 *
 * @return A mask of all pending calibrations that the user has been asked to
 * perform.
 *
 * This function will return a full set of pending calibrations. The only way
 * to clear pending calibrations is to perform them using the \ref RAIL_CalStart()
 * API with the appropriate list of calibrations.
 */
RAIL_CalMask_t RAIL_CalPendingGet(void);

/**
 * Callback that notifies the application that a calibration is needed.
 *
 * @return void
 *
 * This callback function is called whenever the RAIL library detects that a
 * calibration is needed. It is up to the application to determine a valid
 * window to call \ref RAIL_CalStart().
 */
void RAILCb_CalNeeded(void);

/**
 * @}
 */

/******************************************************************************
 * Diagnostic
 *****************************************************************************/
/**
 * @addtogroup Diagnostic
 * @brief APIs for diagnostic and test chip modes
 * @{
 */

/**
 * Enable or disable direct mode for RAIL.
 *
 * @param[in] enable Whether to turn direct mode on or off. At some point this
 *  will include a configuration structure.
 * @warning This API configures fixed pins for tx data in, rx data out, rx clock
 *  out. There should be more control over these pins in the future but they are
 *  currently fixed.
 *
 * In this mode packets will be output and input directly to the radio via GPIO
 * and RAIL packet handling will be ignored. On the EFR32, the DIN pin in TX is
 * EFR32_PC10, which corresponds to EXP_HEADER15/WSTKP12, and the DOUT pin in
 * RX is EFR32_PC11, which corresponds to EXP_HEADER16/WSTKP13.
 */
void RAIL_DirectModeConfig(bool enable);

/**
 * Set the crystal tuning
 *
 * @param[in] tune Chip dependent crystal capacitor bank tuning parameter
 *
 * Tune the crystal that the radio depends on, to change the location of the
 * center frequency for transmitting and receiving.
 */
void RAIL_SetTune(uint32_t tune);

/**
 * Get the crystal tuning
 *
 * @return Chip dependent crystal capacitor bank tuning parameter
 *
 * Retrieve the current tuning value used by the crystal that the radio
 * depends on.
 */
uint32_t RAIL_GetTune(void);

/**
 * Starts transmitting a tone on a certain channel
 *
 * @param[in] channel Define the channel to emit a tone
 * @return Returns 0 on success and 1 on error
 *
 * Transmits a continuous wave, or tone, at the given channel, as defined by
 * the channel configuration passed to RAIL_ChannelConfig().
 */
uint8_t RAIL_TxToneStart(uint8_t channel);

/**
 * Stop tone transmission
 *
 * @return Returns 0 on success and 1 on error
 *
 * Halt the transmission started by RAIL_TxToneStart().
 */
uint8_t RAIL_TxToneStop(void);

/**
 * Start transmitting a stream on a certain channel
 *
 * @param[in] channel Channel on which to emit a stream
 * @param[in] mode Choose the stream mode (PN9, etc)
 * @return Returns 0 on success and 1 on error
 *
 * Emits an encoded stream of bits on the given channel, from either a PN9 or
 * pseudo-random source.
 */
uint8_t RAIL_TxStreamStart(uint8_t channel, RAIL_StreamMode_t mode);

/**
 * Stop stream transmission
 *
 * @return Returns 0 on success and 1 on error
 *
 * Halt the transmission started by RAIL_TxStreamStart().
 */
uint8_t RAIL_TxStreamStop(void);

/**
 * Configure BER test
 *
 * @param[in] berConfig BER test parameters to apply.
 *
 * Configure settings specific to bit error rate (BER) testing.
 * During BER test mode, this device will expect to receive a standard PN9
 * signal (x^9 + x^5 + 1). In order to use this BER test, the selection
 * for BER mode should be enabled from the radio configurator.
 * This function has been deprecated.
 */
void RAIL_BerConfigSet(RAIL_BerConfig_t *berConfig);

/**
 * Start BER test
 *
 * @return void
 *
 * Enter BER receive with the settings specified by RAIL_BerConfigSet().
 * This also resets the BER status.
 * This function has been deprecated.
 */
void RAIL_BerRxStart(void);

/**
 * Stop BER test
 *
 * @return void
 *
 * Halt a test early, or exit infinite BER receive mode.
 * This function has been deprecated.
 */
void RAIL_BerRxStop(void);

/**
 * Get BER test status
 *
 * @param[out] status Statistics pertaining to the latest BER test.
 * @return void
 *
 * Get status of latest BER test.
 * This function has been deprecated.
 */
void RAIL_BerStatusGet(RAIL_BerStatus_t *status);

/**
 * @}
 */


/******************************************************************************
 * Debug
 *****************************************************************************/
/**
 * @addtogroup Debug
 * @brief APIs for debugging
 * @{
 */

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/**
 * Configure Debug callbacks (all are optional)
 *
 * @param[in] cbToEnable Define statuses that force TxRadioStatus callback
 */
void RAIL_DebugCbConfig(uint32_t cbToEnable);

/**
 * Configure the debug mode for the radio library. Do not use this function
 * unless instructed to by Silicon Labs.
 * @param debugMode The debug mode to enter
 * @return Whether this command ran successfully or not.
 */
RAIL_Status_t RAIL_DebugModeSet(uint32_t debugMode);

uint32_t RAIL_DebugModeGet(void);

/**
 * Override the radio base frequency
 *
 * @param[in] freq Desired frequency in Hz
 *
 * Sets the radio to transmit at the frequency given. This function can only
 * be used while in RAIL_DEBUG_MODE_FREQ_OVERRIDE. The given frequency needs
 * to be close to the base frequency of the current PHY.
 */
RAIL_Status_t RAIL_DebugFrequencyOverride(uint32_t freq);
#endif

/**
 * Callback function to signify when the radio changes state.
 *
 * @param[in] state Current state of the radio. Exact values are for internal
 * use only.
 *
 * This is for debug and __NOT__ for application use. It is not called by
 * default but is required for the linking process.
 *
 * Create an empty function for this callback as shown below.
 *
 * @code{.c}
 * RAILCb_RadioStateChanged(uint8_t state) {
 * }
 * @endcode
 */
void RAILCb_RadioStateChanged(uint8_t state);

/**
 * @}
 */

/**
 * end of RAIL_API
 * @}
 */

#endif // __RAIL_H__
