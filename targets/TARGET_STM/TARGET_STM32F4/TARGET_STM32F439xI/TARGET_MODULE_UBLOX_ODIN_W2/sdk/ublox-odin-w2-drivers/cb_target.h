/**
 *---------------------------------------------------------------------------
 * Copyright (c) 2016, u-blox Malmö, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Component   : WLAN driver
 * File        : cb_target.h
 *
 * Description : OWL222a Wireless LAN module representation
 *-------------------------------------------------------------------------*/

/**
 * @file cb_target.h Handles the lowest layer of communication to the target 
 * before the SDIO stack.
 * 
 * @note Create and Destroy methods are not specified here as these are not part
 * of the interface used by the driver. The application should specify these
 * as needed and simply supply the driver with a target handle.
 *
 * @ingroup port
 */

#ifndef _CB_TARGET_H_
#define _CB_TARGET_H_

#include "cb_wlan_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * DEFINES
 *=========================================================================*/
#define cbTARGET_SDIO_BLOCK_SIZE        (256)

/*===========================================================================
 * TYPES
 *=========================================================================*/
typedef struct cbTARGET_Handle cbTARGET_Handle;

typedef struct cbTARGET_FunctionHandle cbTARGET_FunctionHandle;

typedef void(*cbTARGET_ISRHandler)(void* hCallback);


typedef enum {
    cbTARGET_CBTYPE_IRQ,
    cbTARGET_CBTYPE_START,
    cbTARGET_CBTYPE_STOP,
} cbTARGET_CallbackType;

typedef enum cbTARGET_SDIO_Function {
    cbTARGET_SDIO_FUNCTION_0,
    cbTARGET_SDIO_FUNCTION_1,
    cbTARGET_SDIO_FUNCTION_2,
    cbTARGET_SDIO_FUNCTION_3,
    cbTARGET_SDIO_FUNCTION_4,
    cbTARGET_SDIO_FUNCTION_5,
    cbTARGET_SDIO_FUNCTION_6,
    cbTARGET_SDIO_FUNCTION_7,
} cbTARGET_SDIO_Function;

/*===========================================================================
 * FUNCTIONS
 *=========================================================================*/

/**
 * Map/resolve target handle from port specific identifier.
 *
 * @param targetId Port specific target identifier.
 * @return Target handle matching target identifier.
 */
cbTARGET_Handle *cbTARGET_targetResolve(cb_int32 targetId);

/**
 * Perform a hardware reset of the TARGET.
 */
void cbTARGET_reset(cbTARGET_Handle *hTarget);

/**
 * Register a interrupt handler with the TARGET.
 */
void cbTARGET_registerISRHandler(cbTARGET_Handle *hTarget, cbTARGET_ISRHandler handler, void* hContext);


/**
 * Initiate a SDIO function and return a controlling object.
 *
 * @param hTarget Handle to target object.
 * @param func    SDIO function to initiate.
 * @return Handle to a SDIO function object.
 */
cbTARGET_FunctionHandle* cbTARGET_initFunction(cbTARGET_Handle *hTarget, cbTARGET_SDIO_Function func);

/**
 * Write a buffer to the target using SDIO FIFO access.
 *
 * @param hFunction Handle to a SDIO target function object.
 * @param address SDIO address.
 * @param data Send buffer.
 * @param dataSize Number of valid data bytes in transfer.
 * @return True if successful, otherwise FALSE.
 */
cb_boolean cbTARGET_FIFOWrite(cbTARGET_FunctionHandle* hFunction, cb_uint32 address, cb_uint8* data, cb_uint32 dataSize);

/**
 * Read from the target into a buffer using SDIO FIFO access.
 *
 * @param hFunction Handle to a SDIO target function object.
 * @param address SDIO address.
 * @param data Receive buffer.
 * @param dataSize Number of valid data bytes in transfer.
 * @return TRUE if successful, otherwise FALSE.
 */
cb_boolean cbTARGET_FIFORead(cbTARGET_FunctionHandle* hFunction, cb_uint32 address, cb_uint8* data, cb_uint32 dataSize);

/**
 * Write a buffer to the target.
 *
 * @param hFunction Handle to a SDIO target function object.
 * @param address SDIO address.
 * @param data Send buffer.
 * @param dataSize Number of valid data bytes in transfer.
 * @return True if successful, otherwise FALSE.
 */
cb_boolean cbTARGET_write(cbTARGET_FunctionHandle* hFunction, cb_uint32 address, cb_uint8* data, cb_uint32 dataSize);

/**
 * Read from the target into a buffer.
 *
 * @param hFunction Handle to a SDIO target function object.
 * @param address SDIO address.
 * @param data Receive buffer.
 * @param dataSize Number of valid data bytes in transfer.
 * @return TRUE if successful, otherwise FALSE.
 */
cb_boolean cbTARGET_read(cbTARGET_FunctionHandle* hFunction, cb_uint32 address, cb_uint8* data, cb_uint32 dataSize);

/**
 * Write a single byte to the function 0 of the SDIO target.
 *
 * @param hTarget Handle to target object.
 * @param address SDIO address.
 * @param data Send buffer.
 * @return True if successful, otherwise FALSE.
 */
cb_boolean cbTARGET_f0_writeByte(cbTARGET_Handle *hTarget, cb_uint32 address, cb_uint8 data);

/**
 * Read a single byte from the function 0 of the SDIO target.
 *
 * @param hTarget Handle to target object.
 * @param address SDIO address.
 * @param data Receive buffer.
 * @return TRUE if successful, otherwise FALSE.
 */
cb_boolean cbTARGET_f0_readByte(cbTARGET_Handle *hTarget, cb_uint32 address, cb_uint8 *data);


cb_uint16 cbTARGET_getBlockSize(cbTARGET_Handle *hTarget);

#ifdef __cplusplus
}
#endif

#endif
