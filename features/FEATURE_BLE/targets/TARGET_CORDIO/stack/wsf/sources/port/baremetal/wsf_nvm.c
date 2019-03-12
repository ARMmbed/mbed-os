/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief NVM service.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_nvm.h"
#include "stack/platform/include/pal_nvm.h"
#include "util/crc32.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! NVM data start address. */
#define WSF_NVM_START_ADDR                        0x0000

/*! Reserved filecode. */
#define WSF_NVM_RESERVED_FILECODE                 ((uint32_t)0)

/* Unused (erased) filecode. */
/* TODO: May depend on flash type */
#define WSF_NVM_UNUSED_FILECODE                   ((uint32_t)0xFFFFFFFF)

/*! Align value to word boundary. */
#define WSF_NVM_WORD_ALIGN(x)                     (((x) + (PAL_NVM_WORD_SIZE - 1)) & \
                                                         ~(PAL_NVM_WORD_SIZE - 1))

#define WSF_NVM_CRC_INIT_VALUE                    0xFEDCBA98

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Header. */
typedef struct
{
  uint32_t          id;         /*!< Stored data ID. */
  uint32_t          len;        /*!< Stored data length. */
  uint32_t          headerCrc;  /*!< CRC of this header. */
  uint32_t          dataCrc;    /*!< CRC of subsequent data. */
} WsfNvmHeader_t;

/**************************************************************************************************
  Global Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the WSF NVM.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfNvmInit(void)
{
  PalNvmInit(NULL);
}

/*************************************************************************************************/
/*!
 *  \brief  Read data.
 *
 *  \param  id         Stored data ID.
 *  \param  pData      Buffer to read to.
 *  \param  len        Data length to read.
 *  \param  compCback  Read callback.
 *
 *  \return if Read NVM successfully.
 */
/*************************************************************************************************/
bool_t WsfNvmReadData(uint32_t id, uint8_t *pData, uint16_t len, WsfNvmCompEvent_t compCback)
{
  WsfNvmHeader_t header;
  uint32_t headerCrc, dataCrc;
  uint32_t storageAddr = WSF_NVM_START_ADDR;
  bool_t findId = FALSE;

  WSF_ASSERT(!((id == WSF_NVM_RESERVED_FILECODE) || (id == WSF_NVM_UNUSED_FILECODE)));

  /* Read first header. */
  PalNvmRead(&header, sizeof(header), storageAddr);

  do
  {
    if (header.id == WSF_NVM_UNUSED_FILECODE)
    {
      /* Found unused entry at end of used storage. */
      break;
    }

    /* Iterate through stored data headers, looking for existing matching stored data header. */
    if (header.id != WSF_NVM_RESERVED_FILECODE)
    {
      /* Calculate CRC of header itself. */
      headerCrc = CalcCrc32(WSF_NVM_CRC_INIT_VALUE, sizeof(header.id) + sizeof(header.len),
                            (uint8_t *)&header);

      if (headerCrc != header.headerCrc)
      {
        /* Corrupt header. */
        /* TODO: Catastrophic failure? */
        break;
      }
      else if ((header.id == id) && (header.len == len))
      {
        /* Valid header and matching ID - read data after header. */
        storageAddr += sizeof(header);
        PalNvmRead(pData, header.len, storageAddr);
        dataCrc = CalcCrc32(WSF_NVM_CRC_INIT_VALUE, header.len, pData);
        if (dataCrc == header.dataCrc)
        {
          findId = TRUE;
        }
        break;
      }
    }

    /* Move to next stored data block and read header. */
    storageAddr += WSF_NVM_WORD_ALIGN(header.len) + sizeof(header);
    PalNvmRead(&header, sizeof(header), storageAddr);
  } while(1);

  if (compCback)
  {
    compCback(findId);
  }
  return findId;
}

/*************************************************************************************************/
/*!
 *  \brief  Write data.
 *
 *  \param  id         Stored data ID.
 *  \param  pData      Buffer to write.
 *  \param  len        Data length to write.
 *  \param  compCback  Write callback.
 *
 *  \return if write NVM successfully.
 */
/*************************************************************************************************/
bool_t WsfNvmWriteData(uint32_t id, const uint8_t *pData, uint16_t len, WsfNvmCompEvent_t compCback)
{
  WsfNvmHeader_t header;
  uint32_t headerCrc;
  uint32_t storageAddr = WSF_NVM_START_ADDR;

  WSF_ASSERT(!((id == WSF_NVM_RESERVED_FILECODE) || (id == WSF_NVM_UNUSED_FILECODE)));

  /* Read first header. */
  PalNvmRead(&header, sizeof(header), storageAddr);

  do
  {
    if (header.id == WSF_NVM_UNUSED_FILECODE)
    {
      /* Found unused entry at end of used storage. */
      break;
    }

    /* Iterate through stored data headers, looking for existing matching stored data header. */
    if (header.id != WSF_NVM_RESERVED_FILECODE)
    {
      /* Calculate CRC of header itself. */
      headerCrc = CalcCrc32(WSF_NVM_CRC_INIT_VALUE, sizeof(header.id) + sizeof(header.len),
                            (uint8_t *)&header);

      if (headerCrc != header.headerCrc)
      {
        /* Corrupt header. */
        /* TODO: Catastrophic failure? */
        break;
      }
      else if (header.id == id)
      {
        /* Valid header and matching ID - scratch header out. */
        header.id = WSF_NVM_RESERVED_FILECODE;
        header.headerCrc = 0;
        header.dataCrc = 0;
        PalNvmWrite(&header, sizeof(header), storageAddr);
      }
    }

    /* Move to next stored data block and read header. */
    storageAddr += WSF_NVM_WORD_ALIGN(header.len) + sizeof(header);
    PalNvmRead(&header, sizeof(header), storageAddr);
  } while(1);

  /* After cycling through all headers, create a new stored data header and store data */
  header.id = id;
  header.len = len;
  header.headerCrc = CalcCrc32(WSF_NVM_CRC_INIT_VALUE, sizeof(header.id) + sizeof(header.len),
                               (uint8_t *)&header);
  header.dataCrc = CalcCrc32(WSF_NVM_CRC_INIT_VALUE, len, pData);
  PalNvmWrite(&header, sizeof(header), storageAddr);
  PalNvmWrite((void *)pData, len, storageAddr + sizeof(header));
  if (compCback)
  {
    compCback(TRUE);
  }
  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  Erase data.
 *
 *  \param  id         Erase ID.
 *  \param  compCback  Write callback.
 *
 *  \return if erase NVM successfully.
 */
/*************************************************************************************************/
bool_t WsfNvmEraseData(uint32_t id, WsfNvmCompEvent_t compCback)
{
  WsfNvmHeader_t header;
  uint32_t headerCrc;
  uint32_t storageAddr = WSF_NVM_START_ADDR;
  bool_t erased = FALSE;

  WSF_ASSERT(!((id == WSF_NVM_RESERVED_FILECODE) || (id == WSF_NVM_UNUSED_FILECODE)));

  /* Read first header. */
  PalNvmRead(&header, sizeof(header), storageAddr);

  do
  {
    if (header.id == WSF_NVM_UNUSED_FILECODE)
    {
      /* Found unused entry at end of used storage. */
      break;
    }

    /* Iterate through stored data headers, looking for existing matching stored data header. */
    if (header.id != WSF_NVM_RESERVED_FILECODE)
    {
      headerCrc = CalcCrc32(WSF_NVM_CRC_INIT_VALUE, sizeof(header.id) + sizeof(header.len),
                            (uint8_t *)&header);

      if (headerCrc != header.headerCrc)
      {
        /* Corrupt header. */
        /* TODO: Catastrophic failure? */
        break;
      }
      else if (header.id == id)
      {
        header.id = WSF_NVM_RESERVED_FILECODE;
        header.headerCrc = 0;
        header.dataCrc = 0;
        PalNvmWrite(&header, sizeof(header), storageAddr);
        erased = TRUE;
      }
    }

    /* Move to next stored data block and read header. */
    storageAddr += WSF_NVM_WORD_ALIGN(header.len) + sizeof(header);
    PalNvmRead(&header, sizeof(header), storageAddr);
  } while(1);

  if (compCback)
  {
    compCback(erased);
  }
  return erased;
}

/*************************************************************************************************/
/*!
 *  \brief  Erase sectors.
 *
 *  \param  numOfSectors       Number of sectors to be erased.
 *  \param  compCback          Erase callback.
 *
 *  \return if erase NVM successfully.
 */
/*************************************************************************************************/
void WsfNvmEraseSector(uint32_t numOfSectors, WsfNvmCompEvent_t compCback)
{
  PalNvmEraseSector(numOfSectors * PAL_NVM_SECTOR_SIZE, WSF_NVM_START_ADDR);

  if (compCback)
  {
    compCback(TRUE);
  }
}
