/*
 *  Buffer Controller (BUFC) interface definition.
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
#ifndef __EM_BUFC_H
#define __EM_BUFC_H

#include "em_device.h"

#if defined(BUFC_COUNT) && (BUFC_COUNT > 0)

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

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

/** Buffer size options. */
typedef enum
{
  /** Buffer size of 64 bytes. */
  bufcSize64 = 64,

  /** Buffer size of 128 bytes. */
  bufcSize128 = 128,

  /** Buffer size of 256 bytes. */
  bufcSize256 = 256,

  /** Buffer size of 512 bytes. */
  bufcSize512 = 512,

  bufcSize1024 = 1024,
  bufcSize2048 = 2048
} BUFC_Size_TypeDef;


/** Buffer threshold mode options. */
typedef enum
{
  /** Set threshold interrupt flag (BUFn_IF_THRESHOLDIF) if the number of bytes
   *  in the buffer is larger than threshold value. */
  bufcThreshModeLarger = BUFC_BUF_THRESHOLDCTRL_THRESHOLDMODE_LARGER,

  /** Set threshold interrupt flag (BUFn_IF_THRESHOLDIF) if the number of bytes
   *  in the buffer is less or equal than threshold value. */
  bufcThreshModeLessOrEq = BUFC_BUF_THRESHOLDCTRL_THRESHOLDMODE_LESSOREQUAL
} BUFC_ThreshMode_TypeDef;


/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/** Buffer configuration structure. */
typedef struct
{
  /** Buffer size.
   *  Note: make sure the memory range is reserved for the buffer in RAM! */
  BUFC_Size_TypeDef        size;

  /** Buffer address.
   *  Note: make sure the memory range is reserved for the buffer in RAM! */
  uint32_t                 addr;

  /** Buffer threshold (number of bytes). This value is used to trigger the
   *  threshold interrupt flag (BUFn_IF_THRESHOLDIF), the compare mode is
   *  defined by #threshMode. */
  uint32_t                 thresh;

  /** Buffer threshold mode. Defines how to compare the threshold value with the
   *  number of bytes in the buffer.
   *  number of bytes = (WRITEOFFSET - READOFFSET) mod (buffer size) */
  BUFC_ThreshMode_TypeDef  threshMode;
} BUFC_BufferConf_TypeDef;

/** Default configuration for BUFC_BufferConf_TypeDef structure. */
#define BUFC_BUFFER_CONF_DEFAULT                                 \
  {                                                              \
    bufcSize512,          /* Buffer size is 512 bytes. */        \
    0x0000U,              /* Buffer start address is 0x00000. */ \
    256U,                 /* Threshold set to 256 bytes. */      \
    bufcThreshModeLarger  /* Threshold mode set to "larger". */  \
  }


/** Buffer controller (BUFC) initialization structure. */
typedef struct
{
  /** Configuration structures for all buffers instances. */
  BUFC_BufferConf_TypeDef  buffer[4U];
} BUFC_Init_TypeDef;

/** Default configuration for BUFC_Init_TypeDef structure. */
#define BUFC_INIT_DEFAULT                                             \
  {                                                                   \
    {                                                                 \
      BUFC_BUFFER_CONF_DEFAULT, /* Buffer 0 default configuration. */ \
      BUFC_BUFFER_CONF_DEFAULT, /* Buffer 1 default configuration. */ \
      BUFC_BUFFER_CONF_DEFAULT, /* Buffer 2 default configuration. */ \
      BUFC_BUFFER_CONF_DEFAULT  /* Buffer 3 default configuration. */ \
    }                                                                 \
  }


/*******************************************************************************
 ******************************   PROTOTYPES   *********************************
 ******************************************************************************/

void BUFC_Init(BUFC_Init_TypeDef const *init);
void BUFC_Reset(void);
void BUFC_BufferConfig(uint32_t bufId,
                       BUFC_BufferConf_TypeDef const *confPtr);

/* Note:
 * Consider if these should be really offered as API functions as reconfiguring
 * only one buffer can mess up the others! */
void BUFC_AddrSet(uint32_t bufId, uint32_t addr);
void BUFC_SizeSet(uint32_t bufId, BUFC_Size_TypeDef size);
void BUFC_ThresholdSet(uint32_t bufId,
                       uint32_t thresh,
                       BUFC_ThreshMode_TypeDef threshMode);

void BUFC_Clear(uint32_t bufId);
void BUFC_Prefetch(uint32_t bufId);

#if defined(EFR4DFULL)
void BUFC_WriteStartUpdate(uint32_t bufId);
void BUFC_WriteOffsetRestore(uint32_t bufId);
#endif

uint32_t BUFC_NumOfBytesGet(uint32_t bufId);
int32_t BUFC_WriteOffsetGet(uint32_t bufId);
int32_t BUFC_ReadOffsetGet(uint32_t bufId);

static __INLINE void BUFC_XORDataWrite(uint32_t bufId, uint8_t data);
static __INLINE void BUFC_DataWrite(uint32_t bufId, uint8_t data);
static __INLINE uint8_t BUFC_DataRead(uint32_t bufId);

static __INLINE void BUFC_IntEnable(uint32_t flags);
                                        /*, INT_IntLocation_TypeDef loc) */
static __INLINE void BUFC_IntDisable(uint32_t flags);
                                        /*, INT_IntLocation_TypeDef loc) */

static __INLINE void BUFC_IntClear(uint32_t flags);
static __INLINE void BUFC_IntSet(uint32_t flags);

static __INLINE uint32_t BUFC_IntGet(void); /* INT_IntLocation_TypeDef loc); */
static __INLINE uint32_t BUFC_IntGetEnabled(void); /* INT_IntLocation_TypeDef loc); */

/***************************************************************************//**
 * @brief
 *   Writes data to the selected buffer with XOR.
 *
 * @details
 *   Using this function, the data is written to the buffer through
 *   BUFn_XWRITE resulting in a XOR write that means the data to be written
 *   XOR'ed with the data already in the buffer.
 *   When writing data to the buffer, the WRITEOFFSET is being incremented by 1
 *   for each write. Refer to the Buffer Controller Chapter in EFR Reference
 *   Manual for more details.
 *
 * @note
 *   Internal note: 32bit value on SoC.
 *   +DataWriteOffsetGet()? - (would save one switch or if)
 *
 * @param[in] bufId
 *   Buffer ID, possible values: 0-3.
 *
 * @param[in] data
 *   Data to write to the selected buffer.
 ******************************************************************************/
static __INLINE void BUFC_XORDataWrite(uint32_t bufId, uint8_t data)
{
  BUFC->BUF[bufId].XWRITE = (uint8_t)data;
}

/***************************************************************************//**
 * @brief
 *   Writes data to the selected buffer.
 *
 * @details
 *   When writing data to the buffer, the WRITEOFFSET is being incremented by 1
 *   for each write. Refer to the Buffer Controller Chapter in EFR Reference
 *   Manual for more details.
 *
 * @note
 *   Internal note:
 *   +DataWriteOffsetGet()? - (would save one switch or if)
 *
 * @param[in] bufId
 *   Buffer ID, possible values: 0-3.
 *
 * @param[in] data
 *   Data to write to the selected buffer.
 ******************************************************************************/
static __INLINE void BUFC_DataWrite(uint32_t bufId, uint8_t data)
{
  BUFC->BUF[bufId].WRITEDATA = (uint8_t)data;
}


/***************************************************************************//**
 * @brief
 *   Reads from the selected buffer.
 *
 * @note
 *   ...
 *
 * @param[in] bufId
 *   Buffer ID, possible values: 0-3.
 *
 * @return
 *
 ******************************************************************************/
static __INLINE uint8_t BUFC_DataRead(uint32_t bufId)
{
  return (uint8_t)BUFC->BUF[bufId].READDATA;
}

/***************************************************************************//**
 * @brief
 *   Enable one or more BUFC interrupts.
 *
 * @param[in] flags
 *   BUFC interrupt sources to enable. Use a set of interrupt flags OR-ed
 *   together to enable multiple interrupt sources of the BUFC module
 *   (IRH_BUFC_nIEN_nnn).
 *   Internal note: consider different scenarios, host vs M3, asserts
 *
 * @param[in] loc
 *   Interrupt flag location, possible values:
 *   (list of enum values of INT_IntLocation_TypeDef)
 *   (list of enum values of INT_IntLocation_TypeDef)
 ******************************************************************************/
static __INLINE void BUFC_IntEnable(uint32_t flags)
                                        /*, INT_IntLocation_TypeDef loc) */
{
  BUFC->IEN |= flags;
}


/***************************************************************************//**
 * @brief
 *   Disable one or more BUFC interrupts.
 *
 * @param[in] flags
 *   BUFC interrupt sources to disable. Use a set of interrupt flags OR-ed
 *   together to disable multiple interrupt sources of the BUFC module
 *   (IRH_BUFC_nIEN_nnn).
 *   Internal note: consider different scenarios, host vs M3, asserts
 *
 * @param[in] loc
 *   Interrupt flag location, possible values:
 *   (list of enum values of INT_IntLocation_TypeDef)
 *   (list of enum values of INT_IntLocation_TypeDef)
 ******************************************************************************/
static __INLINE void BUFC_IntDisable(uint32_t flags)
                                        /*, INT_IntLocation_TypeDef loc) */
{
  BUFC->IEN &= ~(flags);
}


/***************************************************************************//**
 * @brief
 *   Clear one or more pending BUFC interrupts.
 *
 * @param[in] flags
 *   BUFC interrupt sources to clear. Use a set of interrupt flags OR-ed together
 *   to clear multiple interrupt sources of the BUFC module (IRH_BUFC_FLAGCLEAR_nnn).
 ******************************************************************************/
static __INLINE void BUFC_IntClear(uint32_t flags)
{
  BUFC->IFC = flags;
}


/***************************************************************************//**
 * @brief
 *   Set one or more pending BUFC interrupts from SW.
 *
 * @param[in] flags
 *   BUFC interrupt sources to set to pending. Use a set of interrupt
 *   flags OR-ed together to set multiple interrupt sources of the BUFC module
 *   (IRH_BUFC_FLAGSET_nnn).
 ******************************************************************************/
static __INLINE void BUFC_IntSet(uint32_t flags)
{
  BUFC->IFC = flags;
}


/***************************************************************************//**
 * @brief
 *   Get pending BUFC interrupt flags.
 *
 * @note
 *   The effect of reading the interrupt flags is dependent on the configuration
 *   of the Interrupt Handler module (IRH) as reading the interrupt flags from
 *   an external host can clear the interrupt if the EIREADCLEAR bit is set
 *   in IRH_CTRL. Refer to EFR Reference Manual Chapter 13 and _irh.c for
 *   more details.
 *   Internal note: consider different scenarios, host vs M3, asserts
 *
 * @param[in] loc
 *   Interrupt flag location, possible values:
 *   (list of enum values of INT_IntLocation_TypeDef)
 *   (list of enum values of INT_IntLocation_TypeDef)
 *   (both should not be supported)
 *
 * @return
 *   Pending BUFC interrupt sources. The OR combination of valid interrupt flags
 *   of the BUFC module (IRH_BUFC_FLAG_nnn).
 ******************************************************************************/
static __INLINE uint32_t BUFC_IntGet(void) /* INT_IntLocation_TypeDef loc) */
{
  return BUFC->IF;
}


/***************************************************************************//**
 * @brief
 *   Get enabled and pending BUFC interrupt flags.
 *
 * @details
 *   Useful for handling more interrupt sources in the same interrupt handler.
 *
 * @note
 *   The effect of reading the interrupt flags is dependent on the configuration
 *   of the Interrupt Handler module (IRH) as reading the interrupt flags from
 *   an external host can clear the interrupt if the EIREADCLEAR bit is set
 *   in IRH_CTRL. Refer to EFR Reference Manual Chapter 13 and _irh.c for
 *   more details.
 *   Internal note: consider different scenarios, host vs M3, asserts
 *
 * @param[in] loc
 *   Interrupt flag location, possible values:
 *   (list of enum values of INT_IntLocation_TypeDef)
 *   (for external its the same as the simple xxxIntGet() as the external flag register only shows the enabled flags)
 *   (both should not be supported)
 *
 * @return
 *   Pending BUFC interrupt sources.
 *   The return value is the bitwise AND combination of
 *   - the OR combination of enabled interrupt sources in IRH_BUFC_xIEN register
 *   (IRH_BUFC_nIEN_nnn) and
 *   - the OR combination of valid interrupt flags of the BUFC module
 *   (IRH_BUFC_FLAG_nnn).
 ******************************************************************************/
static __INLINE uint32_t BUFC_IntGetEnabled(void) /* INT_IntLocation_TypeDef loc) */
{
  uint32_t tmp = 0U;


  /* Store BUFC->IEN in temporary variable in order to define explicit order
   * of volatile accesses. */
  tmp = BUFC->IEN;

  /* Bitwise AND of pending and enabled interrupt flags. */
  return BUFC->IF & tmp;
}


/** @} (end addtogroup BUFC) */
/** @} (end addtogroup EM_Library) */

#ifdef __cplusplus
}
#endif

#endif /* defined(BUFC_COUNT) && (BUFC_COUNT > 0) */

#endif /* __EM_BUFC_H */
