
/** \addtogroup hal */
/** @{*/
/*
 * Copyright (c) 2006-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __DRIVER_STORAGE_H
#define __DRIVER_STORAGE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Driver_Common.h"

#define ARM_STORAGE_API_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,00)  /* API version */


#define _ARM_Driver_Storage_(n)      Driver_Storage##n
#define  ARM_Driver_Storage_(n) _ARM_Driver_Storage_(n)

#define ARM_STORAGE_INVALID_OFFSET  (0xFFFFFFFFFFFFFFFFULL) ///< Invalid address (relative to a storage controller's
                                                            ///< address space). A storage block may never start at this address.

#define ARM_STORAGE_INVALID_ADDRESS (0xFFFFFFFFUL)          ///< Invalid address within the processor's memory address space.
                                                            ///< Refer to memory-mapped storage, i.e. < \ref ARM_DRIVER_STORAGE::ResolveAddress().

/****** Storage specific error codes *****/
#define ARM_STORAGE_ERROR_NOT_ERASABLE      (ARM_DRIVER_ERROR_SPECIFIC - 1) ///< Part (or all) of the range provided to Erase() isn't erasable.
#define ARM_STORAGE_ERROR_NOT_PROGRAMMABLE  (ARM_DRIVER_ERROR_SPECIFIC - 2) ///< Part (or all) of the range provided to ProgramData() isn't programmable.
#define ARM_STORAGE_ERROR_PROTECTED         (ARM_DRIVER_ERROR_SPECIFIC - 3) ///< Part (or all) of the range to Erase() or ProgramData() is protected.
#define ARM_STORAGE_ERROR_RUNTIME_OR_INTEGRITY_FAILURE (ARM_DRIVER_ERROR_SPECIFIC - 4) ///< Runtime or sanity-check failure.

/**
 * \brief Attributes of the storage range within a storage block.
 */
typedef struct _ARM_STORAGE_BLOCK_ATTRIBUTES {
  uint32_t erasable      :  1;   ///< Erasing blocks is permitted with a minimum granularity of 'erase_unit'.
                                 ///<   @note: if 'erasable' is 0--i.e. the 'erase' operation isn't available--then
                                 ///<   'erase_unit' (see below) is immaterial and should be 0.
  uint32_t programmable  :  1;   ///< Writing to ranges is permitted with a minimum granularity of 'program_unit'.
                                 ///<   Writes are typically achieved through the ProgramData operation (following an erase);
                                 ///<   if storage isn't erasable (see 'erasable' above) but is memory-mapped
                                 ///<   (i.e. 'memory_mapped'), it can be written directly using memory-store operations.
  uint32_t executable    :  1;   ///< This storage block can hold program data; the processor can fetch and execute code
                                 ///<   sourced from it. Often this is accompanied with the device being 'memory_mapped' (see \ref ARM_STORAGE_INFO).
  uint32_t protectable   :  1;   ///< The entire block can be protected from program and erase operations. Once protection
                                 ///<   is enabled for a block, its 'erasable' and 'programmable' bits are turned off.
  uint32_t reserved      : 28;
  uint32_t erase_unit;           ///< Minimum erase size in bytes.
                                 ///<   The offset of the start of the erase-range should also be aligned with this value.
                                 ///<   Applicable if the 'erasable' attribute is set for the block.
                                 ///<   @note: if 'erasable' (see above) is 0--i.e. the 'erase' operation isn't available--then
                                 ///<   'erase_unit' is immaterial and should be 0.
  uint32_t protection_unit;      ///< Minimum protectable size in bytes. Applicable if the 'protectable'
                                 ///<   attribute is set for the block. This should be a divisor of the block's size. A
                                 ///<   block can be considered to be made up of consecutive, individually-protectable fragments.
} ARM_STORAGE_BLOCK_ATTRIBUTES;

/**
 * \brief A storage block is a range of memory with uniform attributes. Storage blocks
 * combine to make up the address map of a storage controller.
 */
typedef struct _ARM_STORAGE_BLOCK {
  uint64_t                     addr;       ///< This is the start address of the storage block. It is
                                           ///<   expressed as an offset from the start of the storage map
                                           ///<   maintained by the owning storage controller.
  uint64_t                     size;       ///< This is the size of the storage block, in units of bytes.
                                           ///<   Together with addr, it describes a range [addr, addr+size).
  ARM_STORAGE_BLOCK_ATTRIBUTES attributes; ///< Attributes for this block.
} ARM_STORAGE_BLOCK;

/**
 * The check for a valid ARM_STORAGE_BLOCK.
 */
#define ARM_STORAGE_VALID_BLOCK(BLK) (((BLK)->addr != ARM_STORAGE_INVALID_OFFSET) && ((BLK)->size != 0))

/**
 * \brief Values for encoding storage memory-types with respect to programmability.
 *
 * Please ensure that the maximum of the following memory types doesn't exceed 16; we
 * encode this in a 4-bit field within ARM_STORAGE_INFO::programmability.
 */
#define ARM_STORAGE_PROGRAMMABILITY_RAM       (0x0)
#define ARM_STORAGE_PROGRAMMABILITY_ROM       (0x1) ///< Read-only memory.
#define ARM_STORAGE_PROGRAMMABILITY_WORM      (0x2) ///< write-once-read-only-memory (WORM).
#define ARM_STORAGE_PROGRAMMABILITY_ERASABLE  (0x3) ///< re-programmable based on erase. Supports multiple writes.

/**
 * Values for encoding data-retention levels for storage blocks.
 *
 * Please ensure that the maximum of the following retention types doesn't exceed 16; we
 * encode this in a 4-bit field within ARM_STORAGE_INFO::retention_level.
 */
#define ARM_RETENTION_WHILE_DEVICE_ACTIVE     (0x0) ///< Data is retained only during device activity.
#define ARM_RETENTION_ACROSS_SLEEP            (0x1) ///< Data is retained across processor sleep.
#define ARM_RETENTION_ACROSS_DEEP_SLEEP       (0x2) ///< Data is retained across processor deep-sleep.
#define ARM_RETENTION_BATTERY_BACKED          (0x3) ///< Data is battery-backed. Device can be powered off.
#define ARM_RETENTION_NVM                     (0x4) ///< Data is retained in non-volatile memory.

/**
 * Device Data Security Protection Features. Applicable mostly to EXTERNAL_NVM.
 */
typedef struct _ARM_STORAGE_SECURITY_FEATURES {
  uint32_t acls                :  1; ///< Protection against internal software attacks using ACLs.
  uint32_t rollback_protection :  1; ///< Roll-back protection. Set to true if the creator of the storage
                                     ///<   can ensure that an external attacker can't force an
                                     ///<   older firmware to run or to revert back to a previous state.
  uint32_t tamper_proof        :  1; ///< Tamper-proof memory (will be deleted on tamper-attempts using board level or chip level sensors).
  uint32_t internal_flash      :  1; ///< Internal flash.
  uint32_t reserved1           : 12;

  /**
   * Encode support for hardening against various classes of attacks.
   */
  uint32_t software_attacks     :  1; ///< device software (malware running on the device).
  uint32_t board_level_attacks  :  1; ///< board level attacks (debug probes, copy protection fuses.)
  uint32_t chip_level_attacks   :  1; ///< chip level attacks (tamper-protection).
  uint32_t side_channel_attacks :  1; ///< side channel attacks.
  uint32_t reserved2            : 12;
} ARM_STORAGE_SECURITY_FEATURES;

#define ARM_STORAGE_PROGRAM_CYCLES_INFINITE (0UL) /**< Infinite or unknown endurance for reprogramming. */

/**
 * \brief Storage information. This contains device-metadata. It is the return
 *     value from calling GetInfo() on the storage driver.
 *
 * \details These fields serve a different purpose than the ones contained in
 *     \ref ARM_STORAGE_CAPABILITIES, which is another structure containing
 *     device-level metadata. ARM_STORAGE_CAPABILITIES describes the API
 *     capabilities, whereas ARM_STORAGE_INFO describes the device. Furthermore
 *     ARM_STORAGE_CAPABILITIES fits within a single word, and is designed to be
 *     passed around by value; ARM_STORAGE_INFO, on the other hand, contains
 *     metadata which doesn't fit into a single word and requires the use of
 *     pointers to be moved around.
 */
typedef struct _ARM_STORAGE_INFO {
  uint64_t                      total_storage;        ///< Total available storage, in bytes.
  uint32_t                      program_unit;         ///< Minimum programming size in bytes.
                                                      ///<   The offset of the start of the program-range should also be aligned with this value.
                                                      ///<   Applicable only if the 'programmable' attribute is set for a block.
                                                      ///<   @note: setting program_unit to 0 has the effect of disabling the size and alignment
                                                      ///<   restrictions (setting it to 1 also has the same effect).
  uint32_t                      optimal_program_unit; ///< Optimal programming page-size in bytes. Some storage controllers
                                                      ///<   have internal buffers into which to receive data. Writing in chunks of
                                                      ///<   'optimal_program_unit' would achieve maximum programming speed.
                                                      ///<   Applicable only if the 'programmable' attribute is set for the underlying block(s).
  uint32_t                      program_cycles;       ///< A measure of endurance for reprogramming.
                                                      ///<   Use ARM_STORAGE_PROGRAM_CYCLES_INFINITE for infinite or unknown endurance.
  uint32_t                      erased_value    :  1; ///< Contents of erased memory (usually 1 to indicate erased bytes with state 0xFF).
  uint32_t                      memory_mapped   :  1; ///< This storage device has a mapping onto the processor's memory address space.
                                                      ///<   @note: For a memory-mapped block which isn't erasable but is programmable (i.e. if
                                                      ///<   'erasable' is set to 0, but 'programmable' is 1), writes should be possible directly to
                                                      ///<   the memory-mapped storage without going through the ProgramData operation.
  uint32_t                      programmability :  4; ///< A value to indicate storage programmability.
  uint32_t                      retention_level :  4;
  uint32_t                      reserved        : 22;
  ARM_STORAGE_SECURITY_FEATURES security;             ///< \ref ARM_STORAGE_SECURITY_FEATURES
} ARM_STORAGE_INFO;

/**
\brief Operating status of the storage controller.
*/
typedef struct _ARM_STORAGE_STATUS {
  uint32_t busy  : 1;                   ///< Controller busy flag
  uint32_t error : 1;                   ///< Read/Program/Erase error flag (cleared on start of next operation)
} ARM_STORAGE_STATUS;

/**
 * \brief Storage Driver API Capabilities.
 *
 * This data structure is designed to fit within a single word so that it can be
 * fetched cheaply using a call to driver->GetCapabilities().
 */
typedef struct _ARM_STORAGE_CAPABILITIES {
  uint32_t asynchronous_ops :  1; ///< Used to indicate if APIs like initialize,
                                  ///<   read, erase, program, etc. can operate in asynchronous mode.
                                  ///<   Setting this bit to 1 means that the driver is capable
                                  ///<   of launching asynchronous operations; command completion is
                                  ///<   signaled by the invocation of a completion callback. If
                                  ///<   set to 1, drivers may still complete asynchronous
                                  ///<   operations synchronously as necessary--in which case they
                                  ///<   return a positive error code to indicate synchronous completion.
  uint32_t erase_all        :  1; ///< Supports EraseAll operation.
  uint32_t reserved         : 30;
} ARM_STORAGE_CAPABILITIES;

/**
 * Command opcodes for Storage. Completion callbacks use these codes to refer to
 * completing commands. Refer to \ref ARM_Storage_Callback_t.
 */
typedef enum _ARM_STORAGE_OPERATION {
  ARM_STORAGE_OPERATION_GET_VERSION,
  ARM_STORAGE_OPERATION_GET_CAPABILITIES,
  ARM_STORAGE_OPERATION_INITIALIZE,
  ARM_STORAGE_OPERATION_UNINITIALIZE,
  ARM_STORAGE_OPERATION_POWER_CONTROL,
  ARM_STORAGE_OPERATION_READ_DATA,
  ARM_STORAGE_OPERATION_PROGRAM_DATA,
  ARM_STORAGE_OPERATION_ERASE,
  ARM_STORAGE_OPERATION_ERASE_ALL,
  ARM_STORAGE_OPERATION_GET_STATUS,
  ARM_STORAGE_OPERATION_GET_INFO,
  ARM_STORAGE_OPERATION_RESOLVE_ADDRESS,
  ARM_STORAGE_OPERATION_GET_NEXT_BLOCK,
  ARM_STORAGE_OPERATION_GET_BLOCK
} ARM_STORAGE_OPERATION;

/**
 * Declaration of the callback-type for command completion.
 *
 * @param [in] status
 *               A code to indicate the status of the completed operation. For data
 *               transfer operations, the status field is overloaded in case of
 *               success to return the count of items successfully transferred; this
 *               can be done safely because error codes are negative values.
 *
 * @param [in] operation
 *               The command op-code. This value isn't essential for the callback in
 *               the presence of the command instance-id, but it is expected that
 *               this information could be a quick and useful filter.
 */
typedef void (*ARM_Storage_Callback_t)(int32_t status, ARM_STORAGE_OPERATION operation);

/**
 * This is the set of operations constituting the Storage driver. Their
 * implementation is platform-specific, and needs to be supplied by the
 * porting effort.
 *
 * Some APIs within `ARM_DRIVER_STORAGE` will always operate synchronously:
 * GetVersion, GetCapabilities, GetStatus, GetInfo, ResolveAddress,
 * GetNextBlock, and GetBlock. This means that control returns to the caller
 * with a relevant status code only after the completion of the operation (or
 * the discovery of a failure condition).
 *
 * The remainder of the APIs: Initialize, Uninitialize, PowerControl, ReadData,
 * ProgramData, Erase, EraseAll, can function asynchronously if the underlying
 * controller supports it--i.e. if ARM_STORAGE_CAPABILITIES::asynchronous_ops is
 * set. In the case of asynchronous operation, the invocation returns early
 * (with ARM_DRIVER_OK) and results in a completion callback later. If
 * ARM_STORAGE_CAPABILITIES::asynchronous_ops is not set, then all such APIs
 * execute synchronously, and control returns to the caller with a status code
 * only after the completion of the operation (or the discovery of a failure
 * condition).
 *
 * If ARM_STORAGE_CAPABILITIES::asynchronous_ops is set, a storage driver may
 * still choose to execute asynchronous operations in a synchronous manner. If
 * so, the driver returns a positive value to indicate successful synchronous
 * completion (or an error code in case of failure) and no further invocation of
 * completion callback should be expected. The expected return value for
 * synchronous completion of such asynchronous operations varies depending on
 * the operation. For operations involving data access, it often equals the
 * amount of data transferred or affected. For non data-transfer operations,
 * such as EraseAll or Initialize, it is usually 1.
 *
 * Here's a code snippet to suggest how asynchronous APIs might be used by
 * callers to handle both synchronous and asynchronous execution by the
 * underlying storage driver:
 * \code
 *     ASSERT(ARM_DRIVER_OK == 0); // this is a precondition; it doesn't need to be put in code
 *     int32_t returnValue = drv->asynchronousAPI(...);
 *     if (returnValue < ARM_DRIVER_OK) {
 *         // handle error.
 *     } else if (returnValue == ARM_DRIVER_OK) {
 *         ASSERT(drv->GetCapabilities().asynchronous_ops == 1);
 *         // handle early return from asynchronous execution; remainder of the work is done in the callback handler.
 *     } else {
 *         ASSERT(returnValue == EXPECTED_RETURN_VALUE_FOR_SYNCHRONOUS_COMPLETION);
 *         // handle synchronous completion.
 *     }
 * \endcode
 */
typedef struct _ARM_DRIVER_STORAGE {
  /**
   * \brief Get driver version.
   *
   * The function GetVersion() returns version information of the driver implementation in ARM_DRIVER_VERSION.
   *
   *    - API version is the version of the CMSIS-Driver specification used to implement this driver.
   *    - Driver version is source code version of the actual driver implementation.
   *
   * Example:
   * \code
   *     extern ARM_DRIVER_STORAGE *drv_info;
   *
   *     void read_version (void)  {
   *       ARM_DRIVER_VERSION  version;
   *
   *       version = drv_info->GetVersion ();
   *       if (version.api < 0x10A)   {      // requires at minimum API version 1.10 or higher
   *         // error handling
   *         return;
   *       }
   *     }
   * \endcode
   *
   * @return \ref ARM_DRIVER_VERSION.
   *
   * @note This API returns synchronously--it does not result in an invocation
   *     of a completion callback.
   *
   * @note The function GetVersion() can be called any time to obtain the
   *     required information from the driver (even before initialization). It
   *     always returns the same information.
   */
  ARM_DRIVER_VERSION (*GetVersion)(void);

  /**
   * \brief Get driver capabilities.
   *
   * \details The function GetCapabilities() returns information about
   * capabilities in this driver implementation. The data fields of the struct
   * ARM_STORAGE_CAPABILITIES encode various capabilities, for example if the device
   * is able to execute operations asynchronously.
   *
   * Example:
   * \code
   *     extern ARM_DRIVER_STORAGE *drv_info;
   *
   *     void read_capabilities (void)  {
   *       ARM_STORAGE_CAPABILITIES drv_capabilities;
   *
   *       drv_capabilities = drv_info->GetCapabilities ();
   *       // interrogate capabilities
   *
   *     }
   * \endcode
   *
   * @return \ref ARM_STORAGE_CAPABILITIES.
   *
   * @note This API returns synchronously--it does not result in an invocation
   *     of a completion callback.
   *
   * @note The function GetCapabilities() can be called any time to obtain the
   *     required information from the driver (even before initialization). It
   *     always returns the same information.
   */
  ARM_STORAGE_CAPABILITIES (*GetCapabilities)(void);

  /**
   * \brief Initialize the Storage Interface.
   *
   * The function Initialize is called when the middleware component starts
   * operation. In addition to bringing the controller to a ready state,
   * Initialize() receives a callback handler to be invoked upon completion of
   * asynchronous operations.
   *
   * Initialize() needs to be called explicitly before
   * powering the peripheral using PowerControl(), and before initiating other
   * accesses to the storage controller.
   *
   * The function performs the following operations:
   *   - Initializes the resources needed for the Storage interface.
   *   - Registers the \ref ARM_Storage_Callback_t callback function.
   *
   * To start working with a peripheral the functions Initialize and PowerControl need to be called in this order:
   *     drv->Initialize (...); // Allocate I/O pins
   *     drv->PowerControl (ARM_POWER_FULL);        // Power up peripheral, setup IRQ/DMA
   *
   * - Initialize() typically allocates the I/O resources (pins) for the
   *   peripheral. The function can be called multiple times; if the I/O resources
   *   are already initialized it performs no operation and just returns with
   *   ARM_DRIVER_OK.
   *
   * - PowerControl (ARM_POWER_FULL) sets the peripheral registers including
   *   interrupt (NVIC) and optionally DMA. The function can be called multiple
   *   times; if the registers are already set it performs no operation and just
   *   returns with ARM_DRIVER_OK.
   *
   * To stop working with a peripheral the functions PowerControl and Uninitialize need to be called in this order:
   *     drv->PowerControl (ARM_POWER_OFF);     // Terminate any pending transfers, reset IRQ/DMA, power off peripheral
   *     drv->Uninitialize (...);               // Release I/O pins
   *
   * The functions PowerControl and Uninitialize always execute and can be used
   * to put the peripheral into a Safe State, for example after any data
   * transmission errors. To restart the peripheral in an error condition,
   * you should first execute the Stop Sequence and then the Start Sequence.
   *
   * @param [in] callback
   *               Caller-defined callback to be invoked upon command completion
   *               for asynchronous APIs (including the completion of
   *               initialization). Use a NULL pointer when no callback
   *               signals are required.
   *
   * @note This API may execute asynchronously if
   *     ARM_STORAGE_CAPABILITIES::asynchronous_ops is set. Asynchronous
   *     execution is optional even if 'asynchronous_ops' is set.
   *
   * @return If asynchronous activity is launched, an invocation returns
   *     ARM_DRIVER_OK, and the caller can expect to receive a callback in the
   *     future with a status value of ARM_DRIVER_OK or an error-code. In the
   *     case of synchronous execution, control returns after completion with a
   *     value of 1. Return values less than ARM_DRIVER_OK (0) signify errors.
   */
  int32_t (*Initialize)(ARM_Storage_Callback_t callback);

  /**
   * \brief De-initialize the Storage Interface.
   *
   * The function Uninitialize() de-initializes the resources of Storage interface.
   *
   * It is called when the middleware component stops operation, and wishes to
   * release the software resources used by the interface.
   *
   * @note This API may execute asynchronously if
   *     ARM_STORAGE_CAPABILITIES::asynchronous_ops is set. Asynchronous
   *     execution is optional even if 'asynchronous_ops' is set.
   *
   * @return If asynchronous activity is launched, an invocation returns
   *     ARM_DRIVER_OK, and the caller can expect to receive a callback in the
   *     future with a status value of ARM_DRIVER_OK or an error-code. In the
   *     case of synchronous execution, control returns after completion with a
   *     value of 1. Return values less than ARM_DRIVER_OK (0) signify errors.
   */
  int32_t (*Uninitialize)(void);

  /**
   * \brief Control the Storage interface power.
   *
   * The function \b ARM_Storage_PowerControl operates the power modes of the Storage interface.
   *
   * To start working with a peripheral the functions Initialize and PowerControl need to be called in this order:
   *     drv->Initialize (...);                 // Allocate I/O pins
   *     drv->PowerControl (ARM_POWER_FULL);    // Power up peripheral, setup IRQ/DMA
   *
   * - Initialize() typically allocates the I/O resources (pins) for the
   *   peripheral. The function can be called multiple times; if the I/O resources
   *   are already initialized it performs no operation and just returns with
   *   ARM_DRIVER_OK.
   *
   * - PowerControl (ARM_POWER_FULL) sets the peripheral registers including
   *   interrupt (NVIC) and optionally DMA. The function can be called multiple
   *   times; if the registers are already set it performs no operation and just
   *   returns with ARM_DRIVER_OK.
   *
   * To stop working with a peripheral the functions PowerControl and Uninitialize need to be called in this order:
   *
   *     drv->PowerControl (ARM_POWER_OFF);     // Terminate any pending transfers, reset IRQ/DMA, power off peripheral
   *     drv->Uninitialize (...);               // Release I/O pins
   *
   * The functions PowerControl and Uninitialize always execute and can be used
   * to put the peripheral into a Safe State, for example after any data
   * transmission errors. To restart the peripheral in an error condition,
   * you should first execute the Stop Sequence and then the Start Sequence.
   *
   * @param state
   *          \ref ARM_POWER_STATE. The target power-state for the storage controller.
   *          The parameter state can have the following values:
   *              - ARM_POWER_FULL : set-up peripheral for data transfers, enable interrupts
   *                                 (NVIC) and optionally DMA. Can be called multiple times. If the peripheral
   *                                 is already in this mode, then the function performs no operation and returns
   *                                 with ARM_DRIVER_OK.
   *              - ARM_POWER_LOW : may use power saving. Returns ARM_DRIVER_ERROR_UNSUPPORTED when not implemented.
   *              - ARM_POWER_OFF : terminates any pending data transfers, disables peripheral, disables related interrupts and DMA.
   *
   * @note This API may execute asynchronously if
   *     ARM_STORAGE_CAPABILITIES::asynchronous_ops is set. Asynchronous
   *     execution is optional even if 'asynchronous_ops' is set.
   *
   * @return If asynchronous activity is launched, an invocation returns
   *     ARM_DRIVER_OK, and the caller can expect to receive a callback in the
   *     future with a status value of ARM_DRIVER_OK or an error-code. In the
   *     case of synchronous execution, control returns after completion with a
   *     value of 1. Return values less than ARM_DRIVER_OK (0) signify errors.
   */
  int32_t (*PowerControl)(ARM_POWER_STATE state);

  /**
   * \brief read the contents of a given address range from the storage device.
   *
   * \details Read the contents of a range of storage memory into a buffer
   *   supplied by the caller. The buffer is owned by the caller and should
   *   remain accessible for the lifetime of this command.
   *
   * @param  [in] addr
   *                This specifies the address from where to read data.
   *
   * @param [out] data
   *                The destination of the read operation. The buffer
   *                is owned by the caller and should remain accessible for the
   *                lifetime of this command.
   *
   * @param  [in] size
   *                The number of bytes requested to read. The data buffer
   *                should be at least as large as this size.
   *
   * @note This API may execute asynchronously if
   *     ARM_STORAGE_CAPABILITIES::asynchronous_ops is set. Asynchronous
   *     execution is optional even if 'asynchronous_ops' is set.
   *
   * @return If asynchronous activity is launched, an invocation returns
   *     ARM_DRIVER_OK, and the caller can expect to receive a callback in the
   *     future with the number of successfully transferred bytes passed in as
   *     the 'status' parameter. In the case of synchronous execution, control
   *     returns after completion with a positive transfer-count. Return values
   *     less than ARM_DRIVER_OK (0) signify errors.
   */
  int32_t (*ReadData)(uint64_t addr, void *data, uint32_t size);

  /**
   * \brief program (write into) the contents of a given address range of the storage device.
   *
   * \details Write the contents of a given memory buffer into a range of
   *   storage memory. In the case of flash memory, the destination range in
   *   storage memory typically has its contents in an erased state from a
   *   preceding erase operation. The source memory buffer is owned by the
   *   caller and should remain accessible for the lifetime of this command.
   *
   * @param [in] addr
   *               This is the start address of the range to be written into. It
   *               needs to be aligned to the device's \em program_unit
   *               specified in \ref ARM_STORAGE_INFO.
   *
   * @param [in] data
   *               The source of the write operation. The buffer is owned by the
   *               caller and should remain accessible for the lifetime of this
   *               command.
   *
   * @param [in] size
   *               The number of bytes requested to be written. The buffer
   *               should be at least as large as this size. \note 'size' should
   *               be a multiple of the device's 'program_unit' (see \ref
   *               ARM_STORAGE_INFO).
   *
   * @note It is best for the middleware to write in units of
   *     'optimal_program_unit' (\ref ARM_STORAGE_INFO) of the device.
   *
   * @note This API may execute asynchronously if
   *     ARM_STORAGE_CAPABILITIES::asynchronous_ops is set. Asynchronous
   *     execution is optional even if 'asynchronous_ops' is set.
   *
   * @return If asynchronous activity is launched, an invocation returns
   *     ARM_DRIVER_OK, and the caller can expect to receive a callback in the
   *     future with the number of successfully transferred bytes passed in as
   *     the 'status' parameter. In the case of synchronous execution, control
   *     returns after completion with a positive transfer-count. Return values
   *     less than ARM_DRIVER_OK (0) signify errors.
   */
  int32_t (*ProgramData)(uint64_t addr, const void *data, uint32_t size);

  /**
   * @brief Erase Storage range.
   *
   * @details This function erases a range of storage specified by [addr, addr +
   *     size). Both 'addr' and 'addr + size' should align with the
   *     'erase_unit'(s) of the respective owning storage block(s) (see \ref
   *     ARM_STORAGE_BLOCK and \ref ARM_STORAGE_BLOCK_ATTRIBUTES). The range to
   *     be erased will have its contents returned to the un-programmed state--
   *     i.e. to 'erased_value' (see \ref ARM_STORAGE_BLOCK_ATTRIBUTES), which
   *     is usually 1 to indicate the pattern of all ones: 0xFF.
   *
   * @param [in] addr
   *               This is the start-address of the range to be erased. It must
   *               start at an 'erase_unit' boundary of the underlying block.
   *
   * @param [in] size
   *               Size (in bytes) of the range to be erased. 'addr + size'
   *               must be aligned with the 'erase_unit' of the underlying
   *               block.
   *
   * @note This API may execute asynchronously if
   *     ARM_STORAGE_CAPABILITIES::asynchronous_ops is set. Asynchronous
   *     execution is optional even if 'asynchronous_ops' is set.
   *
   * @return
   *   If the range to be erased doesn't align with the erase_units of the
   *   respective start and end blocks, ARM_DRIVER_ERROR_PARAMETER is returned.
   *   If any part of the range is protected, ARM_STORAGE_ERROR_PROTECTED is
   *   returned. If any part of the range is not erasable,
   *   ARM_STORAGE_ERROR_NOT_ERASABLE is returned. All such sanity-check
   *   failures result in the error code being returned synchronously and the
   *   storage bytes within the range remain unaffected.
   *   Otherwise the function executes in the following ways:
   *     If asynchronous activity is launched, an invocation returns
   *     ARM_DRIVER_OK, and the caller can expect to receive a callback in the
   *     future with the number of successfully erased bytes passed in as
   *     the 'status' parameter. In the case of synchronous execution, control
   *     returns after completion with a positive erase-count. Return values
   *     less than ARM_DRIVER_OK (0) signify errors.
   *
   * @note Erase() may return a smaller (positive) value than the size of the
   *     requested range. The returned value indicates the actual number of bytes
   *     erased. It is the caller's responsibility to follow up with an appropriate
   *     request to complete the operation.
   *
   * @note in the case of a failed erase (except when
   *     ARM_DRIVER_ERROR_PARAMETER, ARM_STORAGE_ERROR_PROTECTED, or
   *     ARM_STORAGE_ERROR_NOT_ERASABLE is returned synchronously), the
   *     requested range should be assumed to be in an unknown state. The
   *     previous contents may not be retained.
   */
  int32_t (*Erase)(uint64_t addr, uint32_t size);

  /**
   * @brief Erase complete storage. Optional function for faster erase of the complete device.
   *
   * This optional function erases the complete device. If the device does not
   *    support global erase then the function returns the error value \ref
   *    ARM_DRIVER_ERROR_UNSUPPORTED. The data field \em 'erase_all' =
   *    \token{1} of the structure \ref ARM_STORAGE_CAPABILITIES encodes that
   *    \ref ARM_STORAGE_EraseAll is supported.
   *
   * @note This API may execute asynchronously if
   *     ARM_STORAGE_CAPABILITIES::asynchronous_ops is set. Asynchronous
   *     execution is optional even if 'asynchronous_ops' is set.
   *
   * @return
   *   If any part of the storage range is protected,
   *   ARM_STORAGE_ERROR_PROTECTED is returned. If any part of the storage
   *   range is not erasable, ARM_STORAGE_ERROR_NOT_ERASABLE is returned. All
   *   such sanity-check failures result in the error code being returned
   *   synchronously and the storage bytes within the range remain unaffected.
   *   Otherwise the function executes in the following ways:
   *     If asynchronous activity is launched, an invocation returns
   *     ARM_DRIVER_OK, and the caller can expect to receive a callback in the
   *     future with ARM_DRIVER_OK passed in as the 'status' parameter. In the
   *     case of synchronous execution, control returns after completion with a
   *     value of 1. Return values less than ARM_DRIVER_OK (0) signify errors.
   */
  int32_t (*EraseAll)(void);

  /**
   * @brief Get the status of the current (or previous) command executed by the
   *     storage controller; stored in the structure \ref ARM_STORAGE_STATUS.
   *
   * @return
   *          The status of the underlying controller.
   *
   * @note This API returns synchronously--it does not result in an invocation
   *     of a completion callback.
   */
  ARM_STORAGE_STATUS (*GetStatus)(void);

  /**
   * @brief Get information about the Storage device; stored in the structure \ref ARM_STORAGE_INFO.
   *
   * @param [out] info
   *                A caller-supplied buffer capable of being filled in with an
   *                \ref ARM_STORAGE_INFO.
   *
   * @return ARM_DRIVER_OK if a ARM_STORAGE_INFO structure containing top level
   *         metadata about the storage controller is filled into the supplied
   *         buffer, else an appropriate error value.
   *
   * @note It is the caller's responsibility to ensure that the buffer passed in
   *         is able to be initialized with a \ref ARM_STORAGE_INFO.
   *
   * @note This API returns synchronously--it does not result in an invocation
   *     of a completion callback.
   */
  int32_t (*GetInfo)(ARM_STORAGE_INFO *info);

  /**
   * \brief For memory-mapped storage, resolve an address relative to
   *     the storage controller into a memory address.
   *
   * @param addr
   *          This is the address for which we want a resolution to the
   *          processor's physical address space. It is an offset from the
   *          start of the storage map maintained by the owning storage
   *          controller.
   *
   * @return
   *          The resolved address in the processor's address space; else
   *          ARM_STORAGE_INVALID_ADDRESS, if no resolution is possible.
   *
   * @note This API returns synchronously. The invocation should return quickly,
   *     and result in a resolved address.
   */
  uint32_t (*ResolveAddress)(uint64_t addr);

  /**
   * @brief Advance to the successor of the current block (iterator), or fetch
   *     the first block (if 'prev_block' is passed in as NULL).
   *
   * @details This helper function fetches (an iterator to) the next block (or
   *     the first block if 'prev_block' is passed in as NULL). In the failure
   *     case, a terminating, invalid block iterator is filled into the out
   *     parameter: 'next_block'. In combination with \ref
   *     ARM_STORAGE_VALID_BLOCK(), it can be used to iterate over the sequence
   *     of blocks within the storage map:
   *
   * \code
   *   ARM_STORAGE_BLOCK block;
   *   for (drv->GetNextBlock(NULL, &block); ARM_STORAGE_VALID_BLOCK(&block); drv->GetNextBlock(&block, &block)) {
   *       // make use of block
   *   }
   * \endcode
   *
   * @param[in]  prev_block
   *               An existing block (iterator) within the same storage
   *               controller. The memory buffer holding this block is owned
   *               by the caller. This pointer may be NULL; if so, the
   *               invocation fills in the first block into the out parameter:
   *               'next_block'.
   *
   * @param[out] next_block
   *               A caller-owned buffer large enough to be filled in with
   *               the following ARM_STORAGE_BLOCK. It is legal to provide the
   *               same buffer using 'next_block' as was passed in with 'prev_block'. It
   *               is also legal to pass a NULL into this parameter if the
   *               caller isn't interested in populating a buffer with the next
   *               block--i.e. if the caller only wishes to establish the
   *               presence of a next block.
   *
   * @return ARM_DRIVER_OK if a valid next block is found (or first block, if
   *     prev_block is passed as NULL); upon successful operation, the contents
   *     of the next (or first) block are filled into the buffer pointed to by
   *     the parameter 'next_block' and ARM_STORAGE_VALID_BLOCK(next_block) is
   *     guaranteed to be true. Upon reaching the end of the sequence of blocks
   *     (iterators), or in case the driver is unable to fetch information about
   *     the next (or first) block, an error (negative) value is returned and an
   *     invalid StorageBlock is populated into the supplied buffer. If
   *     prev_block is NULL, the first block is returned.
   *
   * @note This API returns synchronously--it does not result in an invocation
   *     of a completion callback.
   */
   int32_t (*GetNextBlock)(const ARM_STORAGE_BLOCK* prev_block, ARM_STORAGE_BLOCK *next_block);

  /**
   * @brief Find the storage block (iterator) encompassing a given storage address.
   *
   * @param[in]  addr
   *               Storage address in bytes.
   *
   * @param[out] block
   *               A caller-owned buffer large enough to be filled in with the
   *               ARM_STORAGE_BLOCK encapsulating the given address. This value
   *               can also be passed in as NULL if the caller isn't interested
   *               in populating a buffer with the block--if the caller only
   *               wishes to establish the presence of a containing storage
   *               block.
   *
   * @return ARM_DRIVER_OK if a containing storage-block is found. In this case,
   *     if block is non-NULL, the buffer pointed to by it is populated with
   *     the contents of the storage block--i.e. if block is valid and a block is
   *     found, ARM_STORAGE_VALID_BLOCK(block) would return true following this
   *     call. If there is no storage block containing the given offset, or in
   *     case the driver is unable to resolve an address to a storage-block, an
   *     error (negative) value is returned and an invalid StorageBlock is
   *     populated into the supplied buffer.
   *
   * @note This API returns synchronously--it does not result in an invocation
   *     of a completion callback.
   */
  int32_t (*GetBlock)(uint64_t addr, ARM_STORAGE_BLOCK *block);
} const ARM_DRIVER_STORAGE;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __DRIVER_STORAGE_H */

/** @}*/
