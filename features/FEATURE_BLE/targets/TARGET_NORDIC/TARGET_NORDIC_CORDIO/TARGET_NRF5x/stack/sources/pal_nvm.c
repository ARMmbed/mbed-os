/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  PAL NVM driver.
 *
 *  Copyright (c) 2018-2019 Arm Ltd. All Rights Reserved.
 *  Arm Ltd. confidential and proprietary.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#include <string.h>
#include "stack/platform/include/pal_nvm.h"
#include "sdk_config.h"
#if defined(NRF52840_XXAA)
#include "nrfx_qspi.h"
#include "boards.h"
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! NVM block size. */
#define PAL_NVM_SECTOR64K_SIZE                  0x10000

/*! NVM internal cache buffer size. Note: should be at least 2. */
#define PAL_NVM_CACHE_BUF_SIZE                  11

/*! Aligns a value to word size. */
#define PAL_NVM_WORD_ALIGN(value)               (((value) + (PAL_NVM_WORD_SIZE - 1)) & \
                                                        ~(PAL_NVM_WORD_SIZE - 1))
/*! Validates if a value is aligned to word. */
#define PAL_NVM_IS_WORD_ALIGNED(value)          (((uint32_t)(value) & \
                                                        (PAL_NVM_WORD_SIZE - 1)) == 0)

/*! QSPI flash commands. */
#define QSPI_STD_CMD_WRSR   0x01
#define QSPI_STD_CMD_RSTEN  0x66
#define QSPI_STD_CMD_RST    0x99

#ifdef DEBUG

/*! \brief      Parameter check. */
#define PAL_NVM_PARAM_CHECK(expr)               { \
                                                  if (!(expr)) \
                                                  { \
                                                     palNvmCb.state = PAL_NVM_STATE_ERROR; \
                                                     return; \
                                                  } \
                                                }

#else

/*! \brief      Parameter check (disabled). */
#define PAL_NVM_PARAM_CHECK(expr)

#endif

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! NVM cache buffer. */
static uint32_t palNvmCacheBuf[PAL_NVM_CACHE_BUF_SIZE];

/*! \brief      Control block. */
struct
{
  PalNvmState_t  state;                                   /*!< State. */
  uint32_t       writeAddr;                               /*!< Write address. */
} palNvmCb;

/**************************************************************************************************
  Global Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the platform NVM.
 *
 *  \param[in] actCback     Callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalNvmInit(PalNvmCback_t actCback)
{
#if defined(NRF52840_XXAA)

  uint32_t status;
  uint8_t  temp = 0x40;

  nrfx_qspi_config_t config =
  {                                                                       \
      .xip_offset  = NRFX_QSPI_CONFIG_XIP_OFFSET,                         \
      .pins = {                                                           \
         .sck_pin     = BSP_QSPI_SCK_PIN,                                 \
         .csn_pin     = BSP_QSPI_CSN_PIN,                                 \
         .io0_pin     = BSP_QSPI_IO0_PIN,                                 \
         .io1_pin     = BSP_QSPI_IO1_PIN,                                 \
         .io2_pin     = BSP_QSPI_IO2_PIN,                                 \
         .io3_pin     = BSP_QSPI_IO3_PIN,                                 \
      },                                                                  \
      .irq_priority   = (uint8_t)NRFX_QSPI_CONFIG_IRQ_PRIORITY,           \
      .prot_if = {                                                        \
          .readoc     = (nrf_qspi_readoc_t)NRFX_QSPI_CONFIG_READOC,       \
          .writeoc    = (nrf_qspi_writeoc_t)NRFX_QSPI_CONFIG_WRITEOC,     \
          .addrmode   = (nrf_qspi_addrmode_t)NRFX_QSPI_CONFIG_ADDRMODE,   \
          .dpmconfig  = false,                                            \
      },                                                                  \
      .phy_if = {                                                         \
          .sck_freq   = (nrf_qspi_frequency_t)NRFX_QSPI_CONFIG_FREQUENCY, \
          .sck_delay  = (uint8_t)NRFX_QSPI_CONFIG_SCK_DELAY,              \
          .spi_mode   = (nrf_qspi_spi_mode_t)NRFX_QSPI_CONFIG_MODE,       \
          .dpmen      = false                                             \
      },                                                                  \
  }
  ;

  /* Verify palNvmCacheBuf size is at least 2. */
  PAL_NVM_PARAM_CHECK(PAL_NVM_CACHE_BUF_SIZE >= 2);

  status = nrfx_qspi_init(&config, NULL, NULL);

  PAL_NVM_PARAM_CHECK(status == NRFX_SUCCESS);

  nrf_qspi_cinstr_conf_t cinstr_cfg = {
      .opcode    = QSPI_STD_CMD_RSTEN,
      .length    = NRF_QSPI_CINSTR_LEN_1B,
      .io2_level = 1,
      .io3_level = 1,
      .wipwait   = 1,
      .wren      = 1
  };

  /* Send reset enable. */
  status = nrfx_qspi_cinstr_xfer(&cinstr_cfg, NULL, NULL);

  /* Send reset command */
  cinstr_cfg.opcode = QSPI_STD_CMD_RST;

  status = nrfx_qspi_cinstr_xfer(&cinstr_cfg, NULL, NULL);

  PAL_NVM_PARAM_CHECK(status == NRFX_SUCCESS);

  /* Switch to qspi mode */
  cinstr_cfg.opcode = QSPI_STD_CMD_WRSR;
  cinstr_cfg.length = NRF_QSPI_CINSTR_LEN_2B;

  status = nrfx_qspi_cinstr_xfer(&cinstr_cfg, &temp, NULL);

  PAL_NVM_PARAM_CHECK(status == NRFX_SUCCESS);

  memset(&palNvmCb, 0, sizeof(palNvmCb));

  palNvmCb.state = PAL_NVM_STATE_READY;

  (void)status;
#else
  (void)palNvmCacheBuf;
#endif
}

/*************************************************************************************************/
/*!
 *  \brief  De-initialize the platform NVM.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalNvmDeInit(void)
{
#if defined(NRF52840_XXAA)
  nrfx_qspi_uninit();
#else
#endif
}

/*************************************************************************************************/
/*!
 *  \brief     Reads data from NVM storage.
 *
 *  \param[in] pBuf     Pointer to memory buffer where data will be stored.
 *  \param[in] size     Data size in bytes to be read.
 *  \param[in] srcAddr  Word aligned address from where data is read.
 *
 *  \return    None.
 */
/*************************************************************************************************/
void PalNvmRead(void *pBuf, uint32_t size, uint32_t srcAddr)
{
#if defined(NRF52840_XXAA)
  uint32_t readSize = PAL_NVM_WORD_ALIGN(size);
  uint32_t actualSize = size;
  uint32_t status;
  uint16_t addrOffset = 0;

  PAL_NVM_PARAM_CHECK(palNvmCb.state == PAL_NVM_STATE_READY);
  PAL_NVM_PARAM_CHECK(pBuf != NULL);
  PAL_NVM_PARAM_CHECK(size != 0);

  do
  {
    if (readSize <= sizeof(palNvmCacheBuf))
    {
      /* Read data. */
      status = nrfx_qspi_read(palNvmCacheBuf, readSize, srcAddr + addrOffset);

      PAL_NVM_PARAM_CHECK(status == NRFX_SUCCESS);

      memcpy((uint8_t*)pBuf + addrOffset, palNvmCacheBuf, actualSize);

      readSize = 0;
    }
    else
    {
      /* Read data. */
      status = nrfx_qspi_read(palNvmCacheBuf, sizeof(palNvmCacheBuf), srcAddr + addrOffset);

      PAL_NVM_PARAM_CHECK (status == NRFX_SUCCESS);

      memcpy((uint8_t*)pBuf + addrOffset, palNvmCacheBuf, sizeof(palNvmCacheBuf));

      addrOffset += sizeof(palNvmCacheBuf);
      readSize -= sizeof(palNvmCacheBuf);
      actualSize -= sizeof(palNvmCacheBuf);
    }
  } while (readSize != 0);
  (void)status;
#else
  memset(pBuf, 0xFF, size);
#endif
}

/*************************************************************************************************/
/*!
 *  \brief     Writes data to NVM storage.
 *
 *  \param[in] pBuf     Pointer to memory buffer from where data will be written.
 *  \param[in] size     Data size in bytes to be written.
 *  \param[in] dstAddr  Word aligned address to write data.
 *
 *  \return    None.
 */
/*************************************************************************************************/
void PalNvmWrite(void *pBuf, uint32_t size, uint32_t dstAddr)
{
#if defined(NRF52840_XXAA)
  uint32_t writeSize = PAL_NVM_WORD_ALIGN(size);
  uint32_t actualSize = size;
  uint32_t status;
  uint16_t addrOffset = 0;

  PAL_NVM_PARAM_CHECK(palNvmCb.state == PAL_NVM_STATE_READY);
  PAL_NVM_PARAM_CHECK(pBuf != NULL);
  PAL_NVM_PARAM_CHECK(size != 0);

  do
  {
    if (writeSize <= sizeof(palNvmCacheBuf))
    {
      memcpy(palNvmCacheBuf, (uint8_t*)pBuf + addrOffset, actualSize);
      memset((uint8_t*)palNvmCacheBuf + actualSize, 0xFF, sizeof(palNvmCacheBuf) - actualSize);

      /* Write data. */
      status = nrfx_qspi_write(palNvmCacheBuf, writeSize, dstAddr + addrOffset);

      PAL_NVM_PARAM_CHECK(status == NRFX_SUCCESS);

      writeSize = 0;
    }
    else
    {
      memcpy(palNvmCacheBuf, (uint8_t*)pBuf + addrOffset, sizeof(palNvmCacheBuf));

      /* Write data. */
      status = nrfx_qspi_write(palNvmCacheBuf, sizeof(palNvmCacheBuf), dstAddr + addrOffset);

      PAL_NVM_PARAM_CHECK(status == NRFX_SUCCESS);

      addrOffset += sizeof(palNvmCacheBuf);
      writeSize -= sizeof(palNvmCacheBuf);
      actualSize -= sizeof(palNvmCacheBuf);
    }
  } while (writeSize != 0);
  (void)status;
#else
#endif
}

/*************************************************************************************************/
/*!
 *  \brief  Erase sector.
 *
 *  \param[in] size       Data size in bytes to be erased.
 *  \param[in] startAddr  Word aligned address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalNvmEraseSector(uint32_t size, uint32_t startAddr)
{
#if defined(NRF52840_XXAA)
  nrf_qspi_erase_len_t eSize = QSPI_ERASE_LEN_LEN_4KB;

  if (size > PAL_NVM_SECTOR_SIZE)
  {
    eSize = QSPI_ERASE_LEN_LEN_64KB;
  }

  if (size > PAL_NVM_SECTOR64K_SIZE)
  {
    eSize = QSPI_ERASE_LEN_LEN_All;
  }

  nrfx_qspi_erase(eSize, startAddr);
#endif
}

/*************************************************************************************************/
/*!
 *  \brief  Erase chip. It is not recommended to use since it takes up to 240s.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalNvmEraseChip(void)
{
#if defined(NRF52840_XXAA)
  nrfx_qspi_chip_erase();
#endif
}

/*************************************************************************************************/
/*!
 *  \brief     Get NVM state.
 *
 *  \return    NVM state.
 */
/*************************************************************************************************/
PalNvmState_t PalNvmGetState(void)
{
  return palNvmCb.state;
}
