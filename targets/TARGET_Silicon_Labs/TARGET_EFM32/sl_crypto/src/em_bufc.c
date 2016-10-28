/*
 *  Buffer Controller (BUFC) interface implementation.
 *
 *  Copyright (C) 2016 Silicon Labs, http://www.silabs.com
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#include "em_device.h"

#if defined(BUFC_COUNT) && (BUFC_COUNT > 0)

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "em_bufc.h"
#include "em_assert.h"

/***************************************************************************//**
 * @addtogroup EM_Library
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup BUFC
 * @{
 ******************************************************************************/

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/


/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/


/*******************************************************************************
 ***************************   GLOBAL FUNCTIONS   ******************************
 ******************************************************************************/


/***************************************************************************//**
 * @brief
 *   Initialize the Buffer Controller (BUFC) of EFR.
 *
 * @details
 *   Use this function to initialize the 4 buffers controlled by the Buffer
 *   Controller (BUFC).
 *   Refer to EFR Reference Manual Chapter 5.8 and the configuration structure
 *   BUFC_Init_TypeDef for more details.
 *
 * @note
 *   Internal notes:
 *   Init should map all the buffers (/w starter address), set threshold mode, etc
 *   Init should use the BufferConfig()
 *   It would be important to enforce the use of Init() as it would be required
 *   (and safe) to initialize all the buffers in each RESET or WAKEUP cycle.
 *   => this shall be handled by higher layers (platform)
 *   Note: These kinda cases are not optimal for SPI access, (starter addr mixed
 *   with thrsh mode ("fragmentation")
 *
 *   - BUFC_ThresholdOnPinSet() should be part of init?
 *
 * @param[in] init
 *   Pointer to initialization structure used to configure the buffers.
 ******************************************************************************/
void BUFC_Init(BUFC_Init_TypeDef const *init)
{
  uint32_t i;


  /* Iterate through all the buffers. */
  for (i = 0U; i < 4U; i++)
  {
    BUFC_BufferConfig(i, &(init->buffer[i]));
    /* BUFC_ThresholdOnPinSet() ? */
  }
}


/***************************************************************************//**
 * @brief
 *   Reset BUFC registers to the hardware reset state.
 *
 * @details
 *   This function resets all the buffer configurations (incl. address, size,
 *   buffer pointers, etc.).
 ******************************************************************************/
void BUFC_Reset(void)
{
  uint32_t i;

  /* Reset all buffers (0-3). */
  for (i = 0U; i < 4U; i++)
  {
    /* Clear the buffer. */
    BUFC_Clear(i);

    /* Reset BUF[i] registers to their default values. */
    BUFC->BUF[i].CTRL = _BUFC_BUF_CTRL_RESETVALUE;
    BUFC->BUF[i].ADDR = _BUFC_BUF_ADDR_RESETVALUE;
    BUFC->BUF[i].WRITEOFFSET = _BUFC_BUF_WRITEOFFSET_RESETVALUE;
    BUFC->BUF[i].READOFFSET = _BUFC_BUF_READOFFSET_RESETVALUE;
    BUFC->BUF[i].WRITEDATA = _BUFC_BUF_WRITEDATA_RESETVALUE;
    BUFC->BUF[i].THRESHOLDCTRL = _BUFC_BUF_THRESHOLDCTRL_RESETVALUE;
  }
}


/***************************************************************************//**
 * @brief
 *   Configures all parameters of the selected buffer.
 *
 * @details
 *   Use this function to initialize the 4 buffers controlled by the Buffer
 *   Controller (BUFC).
 *   Refer to EFR Reference Manual Chapter 5.8 and the configuration structure
 *   BUFC_Init_TypeDef for more details.
 *
 * @note
 *   Internal notes:
 *   This should not use the INLINEs provided below.
 *   Think with a MANAGEMENT sublayer in mind when designing this API.
 *   eg. single buffer manipulation could possibly be done by higher level
 *   using this function. Think of run-time services.
 *   - buffer size enums are used because its more consistent for higher level
 *   layers to handle allocated RAM size and BUFC buffer size with them
 *
 * @param[in] confPtr
 *   Pointer to configuration structure used to configure the buffers.
 ******************************************************************************/
void BUFC_BufferConfig(uint32_t bufId,
                       BUFC_BufferConf_TypeDef const *confPtr)
{
  uint32_t tmp;


  /* Sanity check of bufId. */
  EFM_ASSERT(bufId < 4U);
  EFM_ASSERT((confPtr->size == bufcSize64) ||
             (confPtr->size == bufcSize128) ||
             (confPtr->size == bufcSize256) ||
             (confPtr->size == bufcSize1024) ||
             (confPtr->size == bufcSize2048) ||
             (confPtr->size == bufcSize512));


  /* Get buffer size from configuration. */
  switch (confPtr->size)
  {
    case bufcSize64:
    {
      tmp = BUFC_BUF_CTRL_SIZE_SIZE64;
    } break;

    case bufcSize128:
    {
      tmp = BUFC_BUF_CTRL_SIZE_SIZE128;
    } break;

    case bufcSize256:
    {
      tmp = BUFC_BUF_CTRL_SIZE_SIZE256;
    } break;

    case bufcSize512:
    {
      tmp = BUFC_BUF_CTRL_SIZE_SIZE512;
    } break;

    case bufcSize1024:
    {
      tmp = BUFC_BUF_CTRL_SIZE_SIZE1024;
    } break;

    case bufcSize2048:
    {
      tmp = BUFC_BUF_CTRL_SIZE_SIZE2048;
    } break;

    default:
    {
      EFM_ASSERT(0);
      return; /* Early return on error. */
    }
    /* Note: intentionally missing "break;" from the end of the branch as it
     * would be an unreachable statement due to the early return! */
  }

  /* Set buffer size. */
  BUFC->BUF[bufId].CTRL = tmp;

  /* Set the start address of the buffer. */
  BUFC->BUF[bufId].ADDR = (uint32_t)confPtr->addr;

  /* Set buffer threshold and threshold mode. */
  BUFC->BUF[bufId].THRESHOLDCTRL = (uint32_t)confPtr->threshMode |
                                  ((uint32_t)confPtr->thresh <<
                                   _BUFC_BUF_THRESHOLDCTRL_THRESHOLD_SHIFT);
}


/***************************************************************************//**
 * @brief
 *   Sets the address of the selected buffer.
 *
 * @details
 *
 * @note
 *   Internal notes: buffer address will be 32bit on the SoC.
 *
 * @param[in] bufId
 *   Buffer ID, possible values: 0-3.
 *
 * @param[in] addr
 *   Address to be set.
 ******************************************************************************/
void BUFC_AddrSet(uint32_t bufId, uint32_t addr)
{
  /* Set the start address of the buffer. */
  BUFC->BUF[bufId].ADDR = (uint32_t)addr;
}


/***************************************************************************//**
 * @brief
 *   Sets the size of the selected buffer.
 *
 * @details
 *
 * @param[in] bufId
 *   Buffer ID, possible values: 0-3.
 *
 * @param[in] size
 *   Size to be set.
 *   @li bufcSize64 - Buffer size 64 bytes.
 *   @li bufcSize128 - Buffer size 128 bytes.
 *   @li bufcSize256 - Buffer size 256 bytes.
 *   @li bufcSize512 - Buffer size 512 bytes.
 ******************************************************************************/
void BUFC_SizeSet(uint32_t bufId, BUFC_Size_TypeDef size)
{
  /* Set buffer size. */
  BUFC->BUF[bufId].CTRL = (uint32_t)size;
}


/***************************************************************************//**
 * @brief
 *   Sets the threshold options of the selected buffer.
 *
 * @details
 *
 * @param[in] bufId
 *   Buffer ID, possible values: 0-3.
 *
 * @param[in] thresh
 *   Threshold to be set.
 *
 * @param[in] threshMode
 *   Threshold mode to be set.
 *   @li false: THRESHOLDIF will be set if BYTES is larger than THRESHOLD
 *   @li true: THRESHOLDIF will be set if BYTES is less than or equal to THRESHOLD
 ******************************************************************************/
void BUFC_ThresholdSet(uint32_t bufId,
                       uint32_t thresh,
                       BUFC_ThreshMode_TypeDef threshMode)
{
  /* Set buffer threshold and threshold mode. */
  BUFC->BUF[bufId].THRESHOLDCTRL = (uint32_t)threshMode |
                                   (thresh << _BUFC_BUF_THRESHOLDCTRL_THRESHOLD_SHIFT);
}

/***************************************************************************//**
 * @brief
 *   Clears the selected buffer, resets the offset pointers.
 *
 * @details
 *   ...
 *
 * @note
 *   Internal note: CMD register
 *
 * @param[in] bufId
 *   Buffer ID, possible values: 0-3.
 ******************************************************************************/
void BUFC_Clear(uint32_t bufId)
{
  BUFC->BUF[bufId].CMD = BUFC_BUF_CMD_CLEAR;
}


/***************************************************************************//**
 * @brief
 *   Prefetch read data if the buffer was written outside of the buffer
 *   controller (BUFC).
 *
 * @details
 *   If a write to the location indicated by the READOFFSET is performed outside
 *   the BUFC, the prefetched value will not be updated. In this case use this
 *   function to update the prefetched value.
 *
 * @note
 *   Internal note: consider if its really necessary as an API function!
 *
 * @param[in] bufId
 *   Buffer ID, possible values: 0-3.
 ******************************************************************************/
void BUFC_Prefetch(uint32_t bufId)
{
  BUFC->BUF[bufId].CMD = BUFC_BUF_CMD_PREFETCH;
}

#if defined(EFR4DFULL)
/***************************************************************************//**
 * @brief
 *   Update the write start value to the value of the write offset.
 *
 * @details
 *   Use this function to set the write start value (WRITESTART) to the value of
 *   the write offset (WRITEOFFSET). This is normally done before receiving a
 *   new frame. The write start value is used to store the initial value of the
 *   write offset prior to receiving a frame. This value is basically used to
 *   remove a partly received frame which is to be removed.
 *
 * @param[in] bufId
 *   Buffer ID, possible values: 0-3.
 ******************************************************************************/
void BUFC_WriteStartUpdate(uint32_t bufId)
{
  BUFC->BUF[bufId].CMD = BUFC_BUF_CMD_UPDATEWRITESTART;
}


/***************************************************************************//**
 * @brief
 *   Restore the write offset value to the value of the write start.
 *
 * @details
 *   Use this function to set the write offset value (WRITEOFFSET) to the value
 *   of the write start (WRITESTART). This is normally done before discarding a
 *   partly received frame.
 *
 * @param[in] bufId
 *   Buffer ID, possible values: 0-3.
 ******************************************************************************/
void BUFC_WriteOffsetRestore(uint32_t bufId)
{
  BUFC->BUF[bufId].CMD = BUFC_BUF_CMD_RESTOREWRITEOFFSET;
}
#endif

/***************************************************************************//**
 * @brief
 *   Gets the current number of bytes in the selected buffer.
 *
 * @details
 *   BYTES = (WRITEOFFSET - READOFFSET)modulo(SIZE)
 *
 * @note
 *   Internal note: BUFn_STATUS
 *
 * @param[in] bufId
 *   Buffer ID, possible values: 0-3.
 *
 * @return
 *   Number of bytes in the selected buffer.
 ******************************************************************************/
uint32_t BUFC_NumOfBytesGet(uint32_t bufId)
{
  return (BUFC->BUF[bufId].STATUS & _BUFC_BUF_STATUS_BYTES_MASK);
}


/***************************************************************************//**
 * @brief
 *   Gets the current WriteOffset value of the selected buffer.
 *
 * @note
 *   Internal note: consider int16_t instead of int
 *
 * @param[in] bufId
 *   Buffer ID, possible values: 0-3.
 *
 * @return
 *   Current WriteOffset value of the selected buffer.
 ******************************************************************************/
int32_t BUFC_WriteOffsetGet(uint32_t bufId)
{
  /* Casting 2's complement offset value to int32_t. */
  return (int32_t)BUFC->BUF[bufId].WRITEOFFSET;
}


/***************************************************************************//**
 * @brief
 *   Gets the current ReadOffset value of the selected buffer.
 *
 * @note
 *   Internal note: consider int16_t instead of int
 *
 * @param[in] bufId
 *   Buffer ID, possible values: 0-3.
 *
 * @return
 *   Current ReadOffset value of the selected buffer.
 ******************************************************************************/
int32_t BUFC_ReadOffsetGet(uint32_t bufId)
{
  /* Casting 2's complement offset value to int32_t. */
  return (int32_t)BUFC->BUF[bufId].READOFFSET;
}


/** @} (end addtogroup BUFC) */
/** @} (end addtogroup EM_Library) */

#endif /* defined(BUFC_COUNT) && (BUFC_COUNT > 0) */
