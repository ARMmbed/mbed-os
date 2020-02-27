/***************************************************************************//**
 * @file
 * @brief Flash controller (MSC) Peripheral API
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include "em_msc.h"
#if defined(MSC_COUNT) && (MSC_COUNT > 0)

#include "em_assert.h"
#include "em_cmu.h"
#include "em_common.h"
#include "em_core.h"
#include "em_system.h"

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#if defined(__ICCARM__)
/* Suppress warnings originating from use of EFM_ASSERT() with IAR Embedded Workbench */
#pragma diag_suppress=Ta022,Ta023
#endif

#if defined(EM_MSC_RUN_FROM_FLASH) && defined(_EFM32_GECKO_FAMILY)
#error "Running Flash write/erase operations from Flash is not supported on EFM32G."
#endif

/*******************************************************************************
 ******************************      DEFINES      ******************************
 ******************************************************************************/
#if defined(MSC_WRITECTRL_WDOUBLE)
#define WORDS_PER_DATA_PHASE (FLASH_SIZE < (512 * 1024) ? 1 : 2)
#else
#define WORDS_PER_DATA_PHASE (1)
#endif

#if defined(_SILICON_LABS_GECKO_INTERNAL_SDID_80)
/* Fix for errata FLASH_E201 - Potential program failure after Power On */
#define ERRATA_FIX_FLASH_E201_EN
#endif

#define FLASH_PAGE_MASK (~(FLASH_PAGE_SIZE - 1U))

#if defined(_MSC_ECCCTRL_MASK) || defined(_SYSCFG_DMEM0ECCCTRL_MASK)
#if defined(_SILICON_LABS_32B_SERIES_1_CONFIG_1)
/* On Series 1 Config 1 EFM32GG11, ECC is supported for RAM0 and RAM1
   banks (not RAM2). It is necessary to figure out which is biggest to
   calculate the number of DMA descriptors needed. */
#define ECC_RAM_SIZE_MAX   (SL_MAX(RAM0_MEM_SIZE, RAM1_MEM_SIZE))

#define ECC_RAM0_MEM_BASE  (RAM0_MEM_BASE)
#define ECC_RAM0_MEM_SIZE  (RAM0_MEM_SIZE)

#define ECC_RAM1_MEM_BASE  (RAM1_MEM_BASE)
#define ECC_RAM1_MEM_SIZE  (RAM1_MEM_SIZE)

#define ECC_CTRL_REG            (MSC->ECCCTRL)
#define ECC_RAM0_SYNDROMES_INIT (MSC_ECCCTRL_RAMECCEWEN)
#define ECC_RAM0_CORRECTION_EN  (MSC_ECCCTRL_RAMECCCHKEN)
#define ECC_RAM1_SYNDROMES_INIT (MSC_ECCCTRL_RAM1ECCEWEN)
#define ECC_RAM1_CORRECTION_EN  (MSC_ECCCTRL_RAM1ECCCHKEN)

#define ECC_IFC_REG        (MSC->IFC)
#define ECC_IFC_MASK       (MSC_IFC_RAMERR1B | MSC_IFC_RAMERR2B \
                            | MSC_IFC_RAM1ERR1B | MSC_IFC_RAM1ERR2B)

#define ECC_FAULT_CTRL_REG (MSC->CTRL)
#define ECC_FAULT_EN       (MSC_CTRL_RAMECCERRFAULTEN)

#elif defined(_SILICON_LABS_GECKO_INTERNAL_SDID_106)
/* On Series 1 Config 2 EFM32GG12, ECC is supported for RAM0, RAM1 and
   RAM2 banks. All banks are of equal size. */
#define ECC_RAM_SIZE_MAX   (RAM0_MEM_SIZE)

#define ECC_RAM0_MEM_BASE  (RAM0_MEM_BASE)
#define ECC_RAM0_MEM_SIZE  (RAM0_MEM_SIZE)

#define ECC_RAM1_MEM_BASE  (RAM1_MEM_BASE)
#define ECC_RAM1_MEM_SIZE  (RAM1_MEM_SIZE)

#define ECC_RAM2_MEM_BASE  (RAM2_MEM_BASE)
#define ECC_RAM2_MEM_SIZE  (RAM2_MEM_SIZE)

#define ECC_CTRL_REG            (MSC->ECCCTRL)
#define ECC_RAM0_SYNDROMES_INIT (MSC_ECCCTRL_RAMECCEWEN)
#define ECC_RAM0_CORRECTION_EN  (MSC_ECCCTRL_RAMECCCHKEN)
#define ECC_RAM1_SYNDROMES_INIT (MSC_ECCCTRL_RAM1ECCEWEN)
#define ECC_RAM1_CORRECTION_EN  (MSC_ECCCTRL_RAM1ECCCHKEN)
#define ECC_RAM2_SYNDROMES_INIT (MSC_ECCCTRL_RAM2ECCEWEN)
#define ECC_RAM2_CORRECTION_EN  (MSC_ECCCTRL_RAM2ECCCHKEN)

#define ECC_IFC_REG        (MSC->IFC)
#define ECC_IFC_MASK       (MSC_IFC_RAMERR1B | MSC_IFC_RAMERR2B     \
                            | MSC_IFC_RAM1ERR1B | MSC_IFC_RAM1ERR2B \
                            | MSC_IFC_RAM2ERR1B | MSC_IFC_RAM2ERR2B)

#define ECC_FAULT_CTRL_REG (MSC->CTRL)
#define ECC_FAULT_EN       (MSC_CTRL_RAMECCERRFAULTEN)

#elif defined(_SILICON_LABS_32B_SERIES_2)

#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_1)

/* On Series 2 Config 1, aka EFR32XG21, ECC is supported for the
   main DMEM RAM banks which is controlled with one ECC encoder/decoder. */
#define ECC_RAM0_SYNDROMES_INIT (SYSCFG_DMEM0ECCCTRL_RAMECCEWEN)
#define ECC_RAM0_CORRECTION_EN  (SYSCFG_DMEM0ECCCTRL_RAMECCCHKEN)

#elif defined(_SILICON_LABS_32B_SERIES_2_CONFIG_2)

/* On Series 2 Config 2, aka EFR32XG22, ECC is supported for the
   main DMEM RAM banks which is controlled with one ECC encoder/decoder. */
#define ECC_RAM0_SYNDROMES_INIT (SYSCFG_DMEM0ECCCTRL_RAMECCEN)
#define ECC_RAM0_CORRECTION_EN  (SYSCFG_DMEM0ECCCTRL_RAMECCEWEN)

#define ECC_IF_REG         (SYSCFG->IF)
#define ECC_IF_1BIT_ERROR  (SYSCFG_IF_RAMERR1B)

#else

#error "Unknown device"

#endif /* #if defined(if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_1) */

#define ECC_RAM_SIZE_MAX   (RAM_MEM_SIZE)
#define ECC_RAM0_MEM_BASE  (RAM_MEM_BASE)
#define ECC_RAM0_MEM_SIZE  (RAM_MEM_SIZE)

#define ECC_CTRL_REG       (SYSCFG->DMEM0ECCCTRL)
#define ECC_IFC_REG        (SYSCFG->IF_CLR)
#define ECC_IFC_MASK       (SYSCFG_IF_RAMERR1B | SYSCFG_IF_RAMERR2B)
#define ECC_FAULT_CTRL_REG (SYSCFG->CTRL)
#define ECC_FAULT_EN       (SYSCFG_CTRL_RAMECCERRFAULTEN)

#else

#error Unknown device.

#endif

#define ECC_DMA_MAX_XFERCNT (_LDMA_CH_CTRL_XFERCNT_MASK \
                             >> _LDMA_CH_CTRL_XFERCNT_SHIFT)
#define ECC_DMA_DESC_SIZE   ((ECC_DMA_MAX_XFERCNT + 1) * 4)  /* 4 bytes units */

#define ECC_DMA_DESCS       (ECC_RAM_SIZE_MAX / ECC_DMA_DESC_SIZE)

#endif /* #if defined(_MSC_ECCCTRL_MASK) */

/***************************************************************************//**
 * @brief
 *   Get locked status of the MSC registers.
 *
 * @detail
 *   MSC_IS_LOCKED() is implemented as a macro because it's used inside functions
 *   that can be placed either in flash or in RAM.
 ******************************************************************************/
#if defined(_MSC_STATUS_REGLOCK_MASK)
#define MSC_IS_LOCKED()    ((MSC->STATUS & _MSC_STATUS_REGLOCK_MASK) != 0U)
#else
#define MSC_IS_LOCKED()    ((MSC->LOCK & _MSC_LOCK_MASK) != 0U)
#endif

/*******************************************************************************
 ******************************      TYPEDEFS     ******************************
 ******************************************************************************/

#if defined(_MSC_ECCCTRL_MASK) || defined(_SYSCFG_DMEM0ECCCTRL_MASK)
typedef struct {
  uint32_t           initSyndromeEnable;
  uint32_t           correctionEnable;
  uint32_t           base;
  uint32_t           size;
} MSC_EccBank_Typedef;

#endif

/*******************************************************************************
 ******************************      LOCALS      *******************************
 ******************************************************************************/
#if defined(_MSC_ECCCTRL_MASK) || defined(_SYSCFG_DMEM0ECCCTRL_MASK)
static const MSC_EccBank_Typedef eccBankTbl[MSC_ECC_BANKS] =
{
  {
    ECC_RAM0_SYNDROMES_INIT, ECC_RAM0_CORRECTION_EN,
    ECC_RAM0_MEM_BASE, ECC_RAM0_MEM_SIZE
  },
#if MSC_ECC_BANKS > 1
  {
    ECC_RAM1_SYNDROMES_INIT, ECC_RAM1_CORRECTION_EN,
    ECC_RAM1_MEM_BASE, ECC_RAM1_MEM_SIZE
  },
#if MSC_ECC_BANKS > 2
  {
    ECC_RAM2_SYNDROMES_INIT, ECC_RAM2_CORRECTION_EN,
    ECC_RAM2_MEM_BASE, ECC_RAM2_MEM_SIZE
  },
#endif
#endif
};
#endif

/*******************************************************************************
 ******************************     FUNCTIONS     ******************************
 ******************************************************************************/
MSC_RAMFUNC_DECLARATOR MSC_Status_TypeDef
MSC_WriteWordI(uint32_t *address,
               void const *data,
               uint32_t numBytes);

MSC_RAMFUNC_DECLARATOR MSC_Status_TypeDef
MSC_LoadWriteData(uint32_t* data,
                  uint32_t numWords);

MSC_RAMFUNC_DECLARATOR MSC_Status_TypeDef
MSC_LoadVerifyAddress(uint32_t* address);

/** @endcond */

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup MSC
 * @{
 ******************************************************************************/

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

#if defined(_SILICON_LABS_32B_SERIES_2)

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/***************************************************************************//**
 * @brief
 *   Wait for a specified MSC status or timeout.
 *
 * @param[in] mask
 *   MSC->STATUS register mask to apply when testing for specified status.
 * @param[in] value
 *   The value the MSC->STATUS test is waiting to see.
 * @return
 *   Returns the status of a write or erase operation, @ref MSC_Status_TypeDef
 * @verbatim
 *   mscReturnOk - Specified status criterium fulfilled.
 *   mscReturnInvalidAddr - Operation tried to write or erase a non-flash area.
 *   flashReturnLocked - MSC registers are locked or the operation tried to
 *                       write or erase a locked area of the flash.
 *   flashReturnTimeOut - Operation timed out.
 * @endverbatim
 ******************************************************************************/
MSC_RAMFUNC_DEFINITION_BEGIN
msc_Return_TypeDef mscStatusWait(uint32_t mask, uint32_t value)
{
  uint32_t status;
  int timeOut = MSC_PROGRAM_TIMEOUT;

  while (timeOut > 0) {
    // Check if any error flags are set
    if ((status = MSC->STATUS)
        & (MSC_STATUS_LOCKED | MSC_STATUS_REGLOCK | MSC_STATUS_INVADDR)) {
      if (status & (MSC_STATUS_LOCKED | MSC_STATUS_REGLOCK)) {
        return mscReturnLocked;
      }
      return mscReturnInvalidAddr;
    }
    // Test exit criterium
    if ((status & mask) == value) {
      return mscReturnOk;
    }
    timeOut--;
  }
  return mscReturnTimeOut;
}
MSC_RAMFUNC_DEFINITION_END

/***************************************************************************//**
 * @brief
 *   Writes data to flash memory. It is assumed that start address is word
 *   aligned and that numBytes is an integer multiple of four, and that the
 *   write operation does not cross a flash page boundary.
 *
 * @param[in] address
 *   Pointer to the flash word to write to. Must be aligned to words.
 * @param[in] data
 *   Data to write to flash.
 * @param[in] numBytes
 *   Number of bytes to write to flash. NB: Must be divisable by four.
 * @return
 *   Returns the status of the write operation, @ref MSC_Status_TypeDef
 * @verbatim
 *   flashReturnOk - Operation completed successfully.
 *   flashReturnInvalidAddr - Operation tried to write to a non-flash area.
 *   flashReturnLocked - MSC registers are locked or the operation tried to
 *                       program a locked area of the flash.
 *   flashReturnTimeOut - Operation timed out.
 * @endverbatim
 ******************************************************************************/
MSC_RAMFUNC_DEFINITION_BEGIN
msc_Return_TypeDef writeBurst(uint32_t address,
                              const uint32_t *data,
                              uint32_t numBytes)
{
  msc_Return_TypeDef retVal;

  MSC->ADDRB = address;
  if (MSC->STATUS & MSC_STATUS_INVADDR) {
    return mscReturnInvalidAddr;
  }
  MSC->WDATA = *data++;
  numBytes  -= 4;

  while (numBytes) {
    if ((retVal = mscStatusWait(MSC_STATUS_WDATAREADY, MSC_STATUS_WDATAREADY))
        != mscReturnOk) {
      MSC->WRITECMD = MSC_WRITECMD_WRITEEND;
      return retVal;
    }
    MSC->WDATA = *data++;
    numBytes  -= 4;
  }

  MSC->WRITECMD = MSC_WRITECMD_WRITEEND;
  if ((retVal = mscStatusWait((MSC_STATUS_BUSY | MSC_STATUS_PENDING), 0))
      == mscReturnOk) {
    // We need to check twice to be sure
    retVal = mscStatusWait((MSC_STATUS_BUSY | MSC_STATUS_PENDING), 0);
  }
  return retVal;
}
MSC_RAMFUNC_DEFINITION_END

/** @endcond */

/***************************************************************************//**
 * @brief
 *   Initialize MSC module. Puts MSC hw in a known state.
 ******************************************************************************/
void MSC_Init(void)
{
#if defined(_CMU_CLKEN1_MASK)
  CMU->CLKEN1_SET = CMU_CLKEN1_MSC;
#endif
  // Unlock MSC
  MSC->LOCK = MSC_LOCK_LOCKKEY_UNLOCK;
  // Disable flash write
  MSC->WRITECTRL_CLR = MSC_WRITECTRL_WREN;
}

/***************************************************************************//**
 * @brief
 *   Turn off MSC flash write enable and lock MSC registers.
 ******************************************************************************/
void MSC_Deinit(void)
{
  // Unlock MSC
  MSC->LOCK = MSC_LOCK_LOCKKEY_UNLOCK;
  // Disable flash write
  MSC->WRITECTRL_CLR = MSC_WRITECTRL_WREN;
  // Lock MSC
  MSC->LOCK = MSC_LOCK_LOCKKEY_LOCK;
#if defined(_CMU_CLKEN1_MASK)
  CMU->CLKEN1_CLR = CMU_CLKEN1_MSC;
#endif
}

/***************************************************************************//**
 * @brief
 *   Set MSC code execution configuration
 *
 * @param[in] execConfig
 *   Code execution configuration
 ******************************************************************************/
void MSC_ExecConfigSet(MSC_ExecConfig_TypeDef *execConfig)
{
  uint32_t mscReadCtrl;

  mscReadCtrl = MSC->READCTRL & ~MSC_READCTRL_DOUTBUFEN;

  if (execConfig->doutBufEn) {
    mscReadCtrl |= MSC_READCTRL_DOUTBUFEN;
  }

  MSC->READCTRL = mscReadCtrl;
}

/***************************************************************************//**
 * @brief
 *   Erases a page in flash memory.
 *
 *   For IAR Embedded Workbench, Simplicity Studio and GCC this will be achieved
 *   automatically by using attributes in the function proctype. For Keil
 *   uVision you must define a section called "ram_code" and place this manually
 *   in your project's scatter file.
 *
 * @param[in] startAddress
 *   Pointer to the flash page to erase. Must be aligned to beginning of page
 *   boundary.
 * @return
 *   Returns the status of erase operation, @ref MSC_Status_TypeDef
 * @verbatim
 *   mscReturnOk - Operation completed successfully.
 *   mscReturnInvalidAddr - Operation tried to erase a non-flash area.
 *   flashReturnLocked - MSC registers are locked or the operation tried to
 *                       erase a locked area of the flash.
 *   flashReturnTimeOut - Operation timed out.
 * @endverbatim
 ******************************************************************************/
MSC_RAMFUNC_DEFINITION_BEGIN
MSC_Status_TypeDef MSC_ErasePage(uint32_t *startAddress)
{
  MSC_Status_TypeDef retVal;
  bool wasLocked;

  // Address must be aligned to page boundary
  EFM_ASSERT((((uint32_t)startAddress) & (FLASH_PAGE_SIZE - 1U)) == 0);

  wasLocked = MSC_IS_LOCKED();
  MSC->LOCK = MSC_LOCK_LOCKKEY_UNLOCK;

  MSC->WRITECTRL_SET = MSC_WRITECTRL_WREN;
  MSC->ADDRB         = (uint32_t)startAddress;
  MSC->WRITECMD      = MSC_WRITECMD_ERASEPAGE;
  if ((retVal = mscStatusWait((MSC_STATUS_BUSY | MSC_STATUS_PENDING), 0))
      == mscReturnOk) {
    // We need to check twice to be sure
    retVal = mscStatusWait((MSC_STATUS_BUSY | MSC_STATUS_PENDING), 0);
  }
  MSC->WRITECTRL_CLR = MSC_WRITECTRL_WREN;

  if (wasLocked) {
    MSC->LOCK = MSC_LOCK_LOCKKEY_LOCK;
  }

  return retVal;
}
MSC_RAMFUNC_DEFINITION_END

/***************************************************************************//**
 * @brief
 *   Writes data to flash memory. Write data must be aligned to words and
 *   contain a number of bytes that is divisable by four.
 * @note
 *   It is recommended to erase the flash page before performing a write.
 *
 *   For IAR Embedded Workbench, Simplicity Studio and GCC this will be achieved
 *   automatically by using attributes in the function proctype. For Keil
 *   uVision you must define a section called "ram_code" and place this manually
 *   in your project's scatter file.
 *
 * @param[in] address
 *   Pointer to the flash word to write to. Must be aligned to words.
 * @param[in] data
 *   Data to write to flash.
 * @param[in] numBytes
 *   Number of bytes to write to flash. NB: Must be divisable by four.
 * @return
 *   Returns the status of the write operation, @ref MSC_Status_TypeDef
 * @verbatim
 *   flashReturnOk - Operation completed successfully.
 *   flashReturnInvalidAddr - Operation tried to write to a non-flash area.
 *   flashReturnLocked - MSC registers are locked or the operation tried to
 *                       program a locked area of the flash.
 *   flashReturnTimeOut - Operation timed out.
 * @endverbatim
 ******************************************************************************/
MSC_RAMFUNC_DEFINITION_BEGIN
MSC_Status_TypeDef MSC_WriteWord(uint32_t *address,
                                 void const *data,
                                 uint32_t numBytes)
{
  uint32_t addr;
  uint8_t  *pData;
  uint32_t burstLen;
  MSC_Status_TypeDef retVal = mscReturnOk;
  bool wasLocked;

  // Check alignment (must be aligned to words)
  EFM_ASSERT(((uint32_t)address & 0x3U) == 0);
  // Check number of bytes, must be divisable by four
  EFM_ASSERT((numBytes & 0x3U) == 0);

  wasLocked = MSC_IS_LOCKED();
  MSC->LOCK = MSC_LOCK_LOCKKEY_UNLOCK;

  // Enable flash write
  MSC->WRITECTRL_SET = MSC_WRITECTRL_WREN;

  addr  = (uint32_t)address;
  pData = (uint8_t*)data;

  while (numBytes) {
    // Max burst length is up to next flash page boundary
    burstLen = SL_MIN(numBytes,
                      ((addr + FLASH_PAGE_SIZE) & FLASH_PAGE_MASK) - addr);

    if ((retVal = writeBurst(addr, (const uint32_t*)pData, burstLen))
        != mscReturnOk) {
      break;
    }

    addr     += burstLen;
    pData    += burstLen;
    numBytes -= burstLen;
  }

  // Disable flash write
  MSC->WRITECTRL_CLR = MSC_WRITECTRL_WREN;

  if (wasLocked) {
    MSC->LOCK = MSC_LOCK_LOCKKEY_LOCK;
  }

  return retVal;
}
MSC_RAMFUNC_DEFINITION_END

MSC_RAMFUNC_DEFINITION_BEGIN
MSC_Status_TypeDef MSC_MassErase(void)
{
  MSC_Status_TypeDef retVal;

  if (MSC_IS_LOCKED()) {
    return mscReturnLocked;
  }

  MSC->WRITECTRL_SET    = MSC_WRITECTRL_WREN;
  MSC->MISCLOCKWORD_CLR = MSC_MISCLOCKWORD_MELOCKBIT;
  MSC->WRITECMD         = MSC_WRITECMD_ERASEMAIN0;
  retVal                = mscStatusWait(MSC_STATUS_BUSY, 0);
  MSC->MISCLOCKWORD_SET = MSC_MISCLOCKWORD_MELOCKBIT;
  MSC->WRITECTRL_CLR    = MSC_WRITECTRL_WREN;

  return retVal;
}
MSC_RAMFUNC_DEFINITION_END

#else // defined(_SILICON_LABS_32B_SERIES_2)

/***************************************************************************//**
 * @brief
 *   Enables the flash controller for writing.
 * @note
 *   This function must be called before flash operations when
 *   AUXHFRCO clock has been changed from a default band.
 ******************************************************************************/
void MSC_Init(void)
{
#if defined(_MSC_TIMEBASE_MASK)
  uint32_t freq, cycles;
#endif

#if defined(_EMU_STATUS_VSCALE_MASK) && defined(_SILICON_LABS_32B_SERIES_1)
  /* VSCALE must be done. Flash erase and write requires VSCALE2. */
  EFM_ASSERT(!(EMU->STATUS & _EMU_STATUS_VSCALEBUSY_MASK));
  EFM_ASSERT((EMU->STATUS & _EMU_STATUS_VSCALE_MASK) == EMU_STATUS_VSCALE_VSCALE2);
#endif

  /* Unlock the MSC module. */
  MSC->LOCK = MSC_UNLOCK_CODE;
  /* Disable writing to the Flash. */
  MSC->WRITECTRL &= ~MSC_WRITECTRL_WREN;

#if defined(_MSC_TIMEBASE_MASK)
  /* Configure MSC->TIMEBASE according to a selected frequency. */
  freq = CMU_ClockFreqGet(cmuClock_AUX);

  /* Timebase 5us is used for the 1/1.2 MHz band only. Note that the 1 MHz band
     is tuned to 1.2 MHz on newer revisions.  */
  if (freq > 1200000) {
    /* Calculate a number of clock cycles for 1 us as a base period. */
    freq   = (freq * 11) / 10;
    cycles = (freq / 1000000) + 1;

    /* Configure clock cycles for flash timing. */
    MSC->TIMEBASE = (MSC->TIMEBASE & ~(_MSC_TIMEBASE_BASE_MASK
                                       | _MSC_TIMEBASE_PERIOD_MASK))
                    | MSC_TIMEBASE_PERIOD_1US
                    | (cycles << _MSC_TIMEBASE_BASE_SHIFT);
  } else {
    /* Calculate a number of clock cycles for 5 us as a base period. */
    freq   = (freq * 5 * 11) / 10;
    cycles = (freq / 1000000) + 1;

    /* Configure clock cycles for flash timing */
    MSC->TIMEBASE = (MSC->TIMEBASE & ~(_MSC_TIMEBASE_BASE_MASK
                                       | _MSC_TIMEBASE_PERIOD_MASK))
                    | MSC_TIMEBASE_PERIOD_5US
                    | (cycles << _MSC_TIMEBASE_BASE_SHIFT);
  }
#endif
}

/***************************************************************************//**
 * @brief
 *   Disables the flash controller for writing.
 ******************************************************************************/
void MSC_Deinit(void)
{
  /* Disable writing to the Flash. */
  MSC->WRITECTRL &= ~MSC_WRITECTRL_WREN;
  /* Lock the MSC module.*/
  MSC->LOCK = 0;
}

/***************************************************************************//**
 * @brief
 *   Set the MSC code execution configuration.
 *
 * @param[in] execConfig
 *   The code execution configuration.
 ******************************************************************************/
void MSC_ExecConfigSet(MSC_ExecConfig_TypeDef *execConfig)
{
  uint32_t mscReadCtrl;

#if defined(MSC_READCTRL_MODE_WS0SCBTP)
  mscReadCtrl = MSC->READCTRL & _MSC_READCTRL_MODE_MASK;
  if ((mscReadCtrl == MSC_READCTRL_MODE_WS0) && (execConfig->scbtEn)) {
    mscReadCtrl |= MSC_READCTRL_MODE_WS0SCBTP;
  } else if ((mscReadCtrl == MSC_READCTRL_MODE_WS1) && (execConfig->scbtEn)) {
    mscReadCtrl |= MSC_READCTRL_MODE_WS1SCBTP;
  } else if ((mscReadCtrl == MSC_READCTRL_MODE_WS0SCBTP) && (!execConfig->scbtEn)) {
    mscReadCtrl |= MSC_READCTRL_MODE_WS0;
  } else if ((mscReadCtrl == MSC_READCTRL_MODE_WS1SCBTP) && (!execConfig->scbtEn)) {
    mscReadCtrl |= MSC_READCTRL_MODE_WS1;
  } else {
    /* No change needed. */
  }
#endif

  mscReadCtrl = MSC->READCTRL & ~(0
#if defined(MSC_READCTRL_SCBTP)
                                  | MSC_READCTRL_SCBTP
#endif
#if defined(MSC_READCTRL_USEHPROT)
                                  | MSC_READCTRL_USEHPROT
#endif
#if defined(MSC_READCTRL_PREFETCH)
                                  | MSC_READCTRL_PREFETCH
#endif
#if defined(MSC_READCTRL_ICCDIS)
                                  | MSC_READCTRL_ICCDIS
#endif
#if defined(MSC_READCTRL_AIDIS)
                                  | MSC_READCTRL_AIDIS
#endif
#if defined(MSC_READCTRL_IFCDIS)
                                  | MSC_READCTRL_IFCDIS
#endif
                                  );
  mscReadCtrl |= (0
#if defined(MSC_READCTRL_SCBTP)
                  | (execConfig->scbtEn ? MSC_READCTRL_SCBTP : 0)
#endif
#if defined(MSC_READCTRL_USEHPROT)
                  | (execConfig->useHprot ? MSC_READCTRL_USEHPROT : 0)
#endif
#if defined(MSC_READCTRL_PREFETCH)
                  | (execConfig->prefetchEn ? MSC_READCTRL_PREFETCH : 0)
#endif
#if defined(MSC_READCTRL_ICCDIS)
                  | (execConfig->iccDis ? MSC_READCTRL_ICCDIS : 0)
#endif
#if defined(MSC_READCTRL_AIDIS)
                  | (execConfig->aiDis ? MSC_READCTRL_AIDIS : 0)
#endif
#if defined(MSC_READCTRL_IFCDIS)
                  | (execConfig->ifcDis ? MSC_READCTRL_IFCDIS : 0)
#endif
                  );

  MSC->READCTRL = mscReadCtrl;
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/***************************************************************************//**
 * @brief
 *   Perform the address phase of the flash write cycle.
 * @details
 *   This function performs the address phase of a flash write operation by
 *   writing the given flash address to the ADDRB register and issuing the
 *   LADDRIM command to load the address.
 * @param[in] address
 *   An address in flash memory. Must be aligned at a 4 byte boundary.
 * @return
 *   Returns the status of the address load operation, #MSC_Status_TypeDef
 * @verbatim
 *   mscReturnOk - The operation completed successfully.
 *   mscReturnInvalidAddr - The operation tried to erase a non-flash area.
 *   mscReturnLocked - The operation tried to erase a locked area of the Flash.
 * @endverbatim
 ******************************************************************************/
MSC_RAMFUNC_DEFINITION_BEGIN
MSC_Status_TypeDef MSC_LoadVerifyAddress(uint32_t* address)
{
  uint32_t timeOut;

  /* Wait for the MSC to become ready. */
  timeOut = MSC_PROGRAM_TIMEOUT;
  while ((MSC->STATUS & MSC_STATUS_BUSY) && (timeOut != 0)) {
    timeOut--;
  }

  /* Check for timeout. */
  if (timeOut == 0) {
    return mscReturnTimeOut;
  }
  /* Load the address. */
  MSC->ADDRB    = (uint32_t)address;
  MSC->WRITECMD = MSC_WRITECMD_LADDRIM;

  /* Check for an invalid address. */
  if (MSC->STATUS & MSC_STATUS_INVADDR) {
    return mscReturnInvalidAddr;
  }
  return mscReturnOk;
}
MSC_RAMFUNC_DEFINITION_END

/***************************************************************************//**
 * @brief
 *   Perform a flash data write phase.
 * @details
 *   This function performs the data phase of a flash write operation by loading
 *   the given number of 32-bit words to the WDATA register.
 * @param[in] data
 *   A pointer to the first data word to load.
 * @param[in] numWords
 *   A number of data words (32-bit) to load.
 * @return
 *   Returns the status of the data load operation.
 * @verbatim
 *   mscReturnOk - An operation completed successfully.
 *   mscReturnTimeOut - An operation timed out waiting for the flash operation
 *                      to complete.
 * @endverbatim
 ******************************************************************************/
MSC_RAMFUNC_DEFINITION_BEGIN
MSC_Status_TypeDef MSC_LoadWriteData(uint32_t* data,
                                     uint32_t numWords)
{
  uint32_t timeOut;
  uint32_t wordIndex;
  bool useWDouble = false;
  MSC_Status_TypeDef retval = mscReturnOk;

#if defined(_SILICON_LABS_32B_SERIES_0) && defined(_MSC_WRITECTRL_WDOUBLE_MASK)
  /* If the LPWRITE (Low Power Write) is NOT enabled, set WDOUBLE (Write Double word). */
  if (!(MSC->WRITECTRL & MSC_WRITECTRL_LPWRITE)) {
    /* If the number of words to be written is odd, align by writing
       a single word first, before setting the WDOUBLE bit. */
    if (numWords & 0x1) {
      /* Wait for the MSC to become ready for the next word. */
      timeOut = MSC_PROGRAM_TIMEOUT;
      while ((!(MSC->STATUS & MSC_STATUS_WDATAREADY)) && (timeOut != 0)) {
        timeOut--;
      }
      /* Check for timeout. */
      if (timeOut == 0) {
        return mscReturnTimeOut;
      }

      /* Clear the double word option to write the initial single word. */
      MSC->WRITECTRL &= ~MSC_WRITECTRL_WDOUBLE;
      /* Write first data word. */
      MSC->WDATA = *data++;
      MSC->WRITECMD = MSC_WRITECMD_WRITEONCE;

      /* Wait for the operation to finish. It may be required to change the WDOUBLE
         configuration after the initial write. It should not be changed while BUSY. */
      timeOut = MSC_PROGRAM_TIMEOUT;
      while ((MSC->STATUS & MSC_STATUS_BUSY) && (timeOut != 0)) {
        timeOut--;
      }
      /* Check for timeout. */
      if (timeOut == 0) {
        return mscReturnTimeOut;
      }
      /* Check for a write protected flash area. */
      if (MSC->STATUS & MSC_STATUS_LOCKED) {
        return mscReturnLocked;
      }
      /* Subtract this initial odd word for the write loop below. */
      numWords -= 1;
      retval = mscReturnOk;
    }
    /* Set the double word option to write two words per
       data phase. */
    MSC->WRITECTRL |= MSC_WRITECTRL_WDOUBLE;
    useWDouble = true;
  }
#endif /* defined( _MSC_WRITECTRL_LPWRITE_MASK ) && defined( _MSC_WRITECTRL_WDOUBLE_MASK ) */

  /* Write the rest as a double word write if wordsPerDataPhase == 2 */
  if (numWords > 0) {
    /* Requires a system core clock at 1MHz or higher */
    EFM_ASSERT(SystemCoreClock >= 1000000);
    wordIndex = 0;
    while (wordIndex < numWords) {
      if (!useWDouble) {
        MSC->WDATA = *data++;
        wordIndex++;
        MSC->WRITECMD = MSC_WRITECMD_WRITEONCE;
      } else {
        /* Trigger a double write according to flash properties. */
#if defined(_SILICON_LABS_32B_SERIES_0) && defined(_MSC_WRITECTRL_WDOUBLE_MASK)
        MSC->WDATA = *data++;
        while (!(MSC->STATUS & MSC_STATUS_WDATAREADY)) ;
        MSC->WDATA = *data++;
        wordIndex += 2;
        MSC->WRITECMD = MSC_WRITECMD_WRITEONCE;
#endif
      }

      /* Wait for the transaction to finish. */
      timeOut = MSC_PROGRAM_TIMEOUT;
      while ((MSC->STATUS & MSC_STATUS_BUSY) && (timeOut != 0)) {
        timeOut--;
      }
      /* Check for a timeout. */
      if (timeOut == 0) {
        retval = mscReturnTimeOut;
        break;
      }
      /* Check for a write protected flash area. */
      if (MSC->STATUS & MSC_STATUS_LOCKED) {
        retval = mscReturnLocked;
        break;
      }
#if defined(_EFM32_GECKO_FAMILY)
      MSC->ADDRB += 4;
      MSC->WRITECMD = MSC_WRITECMD_LADDRIM;
#endif
    }
  }

#if defined(_MSC_WRITECTRL_WDOUBLE_MASK)
  /* Clear a double word option, which should not be left on when returning. */
  MSC->WRITECTRL &= ~MSC_WRITECTRL_WDOUBLE;
#endif

  return retval;
}
MSC_RAMFUNC_DEFINITION_END

/***************************************************************************//**
 * @brief
 *   An internal flash write function.
 * @param[in] address
 *   A write address.
 * @param[in] data
 *   A pointer to the first data word to load.
 * @param[in] numBytes
 *   A nsumber of data bytes to load, which must be a multiple of 4 bytes.
 * @return
 *   Returns the status of the data load operation.
 ******************************************************************************/
MSC_RAMFUNC_DEFINITION_BEGIN
MSC_Status_TypeDef MSC_WriteWordI(uint32_t *address,
                                  void const *data,
                                  uint32_t numBytes)
{
  uint32_t wordCount;
  uint32_t numWords;
  uint32_t pageWords;
  uint32_t* pData;
  bool wasLocked;
  MSC_Status_TypeDef retval = mscReturnOk;

  wasLocked = MSC_IS_LOCKED();
  MSC->LOCK = MSC_LOCK_LOCKKEY_UNLOCK;

  /* Check alignment (must be aligned to words). */
  EFM_ASSERT(((uint32_t) address & 0x3) == 0);

  /* Check a number of bytes. Must be divisible by four. */
  EFM_ASSERT((numBytes & 0x3) == 0);

#if defined(_EMU_STATUS_VSCALE_MASK) && defined(_SILICON_LABS_32B_SERIES_1)
  /* VSCALE must be done and flash write requires VSCALE2. */
  EFM_ASSERT(!(EMU->STATUS & _EMU_STATUS_VSCALEBUSY_MASK));
  EFM_ASSERT((EMU->STATUS & _EMU_STATUS_VSCALE_MASK) == EMU_STATUS_VSCALE_VSCALE2);
#endif

  /* Enable writing to the MSC module. */
  MSC->WRITECTRL |= MSC_WRITECTRL_WREN;

  /* Convert bytes to words. */
  numWords = numBytes >> 2;
  EFM_ASSERT(numWords > 0);

  /* The following loop splits the data into chunks corresponding to flash pages.
     The address is loaded only once per page because the hardware automatically
     increments the address internally for each data load inside a page. */
  for (wordCount = 0, pData = (uint32_t *)data; wordCount < numWords; ) {
    /* First, the address is loaded. The address is auto-incremented within a page.
       Therefore, the address phase is only needed once for each page. */
    retval = MSC_LoadVerifyAddress(address + wordCount);
    if (mscReturnOk != retval) {
      /* Disable writing to the MSC module. */
      MSC->WRITECTRL &= ~MSC_WRITECTRL_WREN;
      if (wasLocked) {
        MSC->LOCK = MSC_LOCK_LOCKKEY_LOCK;
      }
      return retval;
    }
    /* Compute the number of words to write to the current page. */
    pageWords =
      (FLASH_PAGE_SIZE
       - (((uint32_t) (address + wordCount)) & (FLASH_PAGE_SIZE - 1)))
      / sizeof(uint32_t);
    if (pageWords > numWords - wordCount) {
      pageWords = numWords - wordCount;
    }
    /* Write the data in the current page. */
    retval = MSC_LoadWriteData(pData, pageWords);
    if (mscReturnOk != retval) {
      break;
    }
    wordCount += pageWords;
    pData += pageWords;
  }

#if defined(ERRATA_FIX_FLASH_E201_EN)
  /* Fix for errata FLASH_E201 - Potential program failure after Power On.
   *
   * Check if the first word was programmed correctly. If a failure is detected,
   * retry programming of the first word.
   *
   * A full description of the errata is in the errata document. */
  pData = (uint32_t *) data;
  if (*address != *pData) {
    retval = MSC_LoadVerifyAddress(address);
    if (mscReturnOk == retval) {
      retval = MSC_LoadWriteData(pData, 1);
    }
  }
#endif

  /* Disable writing to the MSC module. */
  MSC->WRITECTRL &= ~MSC_WRITECTRL_WREN;

#if defined(_MSC_WRITECTRL_WDOUBLE_MASK)
#if (WORDS_PER_DATA_PHASE == 2)
  /* Turn off the double word write cycle support. */
  MSC->WRITECTRL &= ~MSC_WRITECTRL_WDOUBLE;
#endif
#endif
  if (wasLocked) {
    MSC->LOCK = MSC_LOCK_LOCKKEY_LOCK;
  }
  return retval;
}
MSC_RAMFUNC_DEFINITION_END

/** @endcond */

/***************************************************************************//**
 * @brief
 *   Erases a page in flash memory.
 * @note
 *   For the Gecko family, it is required to run this function from RAM.
 *
 *   For IAR Embedded Workbench, Simplicity Studio and GCC, this is
 *   achieved automatically by using attributes in the function proctype. For Keil
 *   uVision IDE, define a section called "ram_code" and place this manually in
 *   the project's scatter file.
 *
 * @param[in] startAddress
 *   A pointer to the flash page to erase. Must be aligned to the beginning of the page
 *   boundary.
 * @return
 *   Returns the status of erase operation, #MSC_Status_TypeDef
 * @verbatim
 *   mscReturnOk - The operation completed successfully.
 *   mscReturnInvalidAddr - The operation tried to erase a non-flash area.
 *   mscReturnLocked - The operation tried to erase a locked area of the flash.
 *   mscReturnTimeOut - The operation timed out waiting for the flash operation
 *       to complete.
 * @endverbatim
 ******************************************************************************/
MSC_RAMFUNC_DEFINITION_BEGIN
MSC_Status_TypeDef MSC_ErasePage(uint32_t *startAddress)
{
  uint32_t timeOut = MSC_PROGRAM_TIMEOUT;
  bool wasLocked;

  wasLocked = MSC_IS_LOCKED();
  MSC->LOCK = MSC_LOCK_LOCKKEY_UNLOCK;

  /* An address must be aligned to pages. */
  EFM_ASSERT((((uint32_t) startAddress) & (FLASH_PAGE_SIZE - 1)) == 0);
#if defined(_EMU_STATUS_VSCALE_MASK) && defined(_SILICON_LABS_32B_SERIES_1)
  /* VSCALE must be done and flash erase requires VSCALE2. */
  EFM_ASSERT(!(EMU->STATUS & _EMU_STATUS_VSCALEBUSY_MASK));
  EFM_ASSERT((EMU->STATUS & _EMU_STATUS_VSCALE_MASK) == EMU_STATUS_VSCALE_VSCALE2);
#endif

  /* Enable writing to the MSC module. */
  MSC->WRITECTRL |= MSC_WRITECTRL_WREN;

  /* Load an address. */
  MSC->ADDRB    = (uint32_t)startAddress;
  MSC->WRITECMD = MSC_WRITECMD_LADDRIM;

  /* Check for an invalid address. */
  if (MSC->STATUS & MSC_STATUS_INVADDR) {
    /* Disable writing to the MSC module. */
    MSC->WRITECTRL &= ~MSC_WRITECTRL_WREN;
    if (wasLocked) {
      MSC->LOCK = MSC_LOCK_LOCKKEY_LOCK;
    }
    return mscReturnInvalidAddr;
  }

  /* Send erase page command. */
  MSC->WRITECMD = MSC_WRITECMD_ERASEPAGE;

  /* Wait for the erase to complete. */
  while ((MSC->STATUS & MSC_STATUS_BUSY) && (timeOut != 0)) {
    timeOut--;
  }
  /* Check for write protected page. */
  if (MSC->STATUS & MSC_STATUS_LOCKED) {
    /* Disable writing to the MSC module. */
    MSC->WRITECTRL &= ~MSC_WRITECTRL_WREN;
    if (wasLocked) {
      MSC->LOCK = MSC_LOCK_LOCKKEY_LOCK;
    }
    return mscReturnLocked;
  }
  if (timeOut == 0) {
    /* Disable writing to the MSC module. */
    MSC->WRITECTRL &= ~MSC_WRITECTRL_WREN;
    if (wasLocked) {
      MSC->LOCK = MSC_LOCK_LOCKKEY_LOCK;
    }
    return mscReturnTimeOut;
  }
  /* Disable writing to the MSC module. */
  MSC->WRITECTRL &= ~MSC_WRITECTRL_WREN;
  if (wasLocked) {
    MSC->LOCK = MSC_LOCK_LOCKKEY_LOCK;
  }
  return mscReturnOk;
}
MSC_RAMFUNC_DEFINITION_END

/***************************************************************************//**
 * @brief
 *   Writes data to flash memory. This function is interrupt-safe, but slower than
 *   MSC_WriteWordFast(), which writes to flash with interrupts disabled.
 *   Write data must be aligned to words and contain a number of bytes that is
 *   divisible by four.
 * @note
 *   It is recommended to erase the flash page before performing a write.
 *
 *   For the Gecko family, it is required to run this function from RAM.
 *
 *   For IAR Embedded Workbench, Simplicity Studio and GCC,
 *   this is done automatically by using attributes in the function proctype.
 *   For Keil uVision IDE, define a section called "ram_code" and place it
 *   manually in the project's scatter file.
 *
 *   This function requires a system core clock at 1 MHz or higher.
 *
 * @param[in] address
 *   A pointer to the flash word to write to. Must be aligned to words.
 * @param[in] data
 *   Data to write to flash.
 * @param[in] numBytes
 *   A number of bytes to write from flash. NB: Must be divisible by four.
 * @return
 *   Returns the status of the write operation.
 * @verbatim
 *   flashReturnOk - The operation completed successfully.
 *   flashReturnInvalidAddr - The operation tried to erase a non-flash area.
 *   flashReturnLocked - The operation tried to erase a locked area of the Flash.
 *   flashReturnTimeOut - The operation timed out waiting for the flash operation
 *       to complete, or the MSC module timed out waiting for the software to write
 *       the next word into the DWORD register.
 * @endverbatim
 ******************************************************************************/
MSC_RAMFUNC_DEFINITION_BEGIN
MSC_Status_TypeDef MSC_WriteWord(uint32_t *address,
                                 void const *data,
                                 uint32_t numBytes)
{
  return MSC_WriteWordI(address, data, numBytes);
}
MSC_RAMFUNC_DEFINITION_END

/***************************************************************************//**
 * @brief
 *   Writes data to flash memory. This function is faster than MSC_WriteWord(),
 *   but it disables interrupts. Write data must be aligned to words and contain
 *   a number of bytes that is divisible by four.
 * @warning
 *   This function is only available for certain devices.
 * @note
 *   It is recommended to erase the flash page before performing a write.
 *   It is required to run this function from RAM on parts that include a
 *   flash write buffer.
 *
 *   For IAR Embedded Workbench, Simplicity Studio and GCC,
 *   this is done automatically by using attributes in the function proctype.
 *   For Keil uVision IDE, define a section called "ram_code" and place this manually
 *   in the project's scatter file.
 *
 * @deprecated
 *   This function is deprecated, the functionality is the same as calling
 *   @ref MSC_WriteWord().
 *
 * @param[in] address
 *   A pointer to the flash word to write to. Must be aligned to words.
 * @param[in] data
 *   Data to write to flash.
 * @param[in] numBytes
 *   A number of bytes to write from the Flash. NB: Must be divisible by four.
 * @return
 *   Returns the status of the write operation.
 * @verbatim
 *   flashReturnOk - The operation completed successfully.
 *   flashReturnInvalidAddr - The operation tried to erase a non-flash area.
 *   flashReturnLocked - The operation tried to erase a locked area of the flash.
 *   flashReturnTimeOut - The operation timed out waiting for flash operation
 *       to complete. Or the MSC timed out waiting for the software to write
 *       the next word into the DWORD register.
 * @endverbatim
 ******************************************************************************/
MSC_RAMFUNC_DEFINITION_BEGIN
MSC_Status_TypeDef MSC_WriteWordFast(uint32_t *address,
                                     void const *data,
                                     uint32_t numBytes)
{
  return MSC_WriteWord(address, data, numBytes);
}
MSC_RAMFUNC_DEFINITION_END

#if defined(_MSC_MASSLOCK_MASK)
/***************************************************************************//**
 * @brief
 *   Erase the entire Flash in one operation.
 *
 * @note
 *   This command will erase the entire contents of the device.
 *   Use with care, both a debug session and all contents of the flash will be
 *   lost. The lock bit, MLW will prevent this operation from executing and
 *   might prevent a successful mass erase.
 ******************************************************************************/
SL_RAMFUNC_DEFINITION_BEGIN
MSC_Status_TypeDef MSC_MassErase(void)
{
  bool wasLocked;
  wasLocked = MSC_IS_LOCKED();
  MSC->LOCK = MSC_LOCK_LOCKKEY_UNLOCK;

  /* Enable writing to the MSC module. */
  MSC->WRITECTRL |= MSC_WRITECTRL_WREN;

  /* Unlock the device mass erase. */
  MSC->MASSLOCK = MSC_MASSLOCK_LOCKKEY_UNLOCK;

  /* Erase the first 512 K block. */
  MSC->WRITECMD = MSC_WRITECMD_ERASEMAIN0;

  /* Waiting for erase to complete. */
  while ((MSC->STATUS & MSC_STATUS_BUSY) != 0U) {
  }

#if ((FLASH_SIZE >= (512 * 1024)) && defined(_MSC_WRITECMD_ERASEMAIN1_MASK))
  /* Erase the second 512 K block. */
  MSC->WRITECMD = MSC_WRITECMD_ERASEMAIN1;

  /* Waiting for erase to complete. */
  while ((MSC->STATUS & MSC_STATUS_BUSY) != 0U) {
  }
#endif

  /* Restore the mass erase lock. */
  MSC->MASSLOCK = MSC_MASSLOCK_LOCKKEY_LOCK;

  /* Disable writing to the MSC module. */
  MSC->WRITECTRL &= ~MSC_WRITECTRL_WREN;

  if (wasLocked) {
    MSC->LOCK = MSC_LOCK_LOCKKEY_LOCK;
  }

  /* This will only successfully return if calling function is also in SRAM. */
  return mscReturnOk;
}
SL_RAMFUNC_DEFINITION_END
#endif  // defined(_MSC_MASSLOCK_MASK)

#endif // defined(_SILICON_LABS_32B_SERIES_2)

#if defined(_MSC_ECCCTRL_MASK) || defined(_SYSCFG_DMEM0ECCCTRL_MASK)

#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_2)

/***************************************************************************//**
 * @brief
 *    Read and write existing values in RAM (for ECC initializaion).
 *
 * @details
 *    This function uses core to load and store the existing data
 *    values in the given RAM bank.
 *
 * @param[in] eccBank
 *    Pointer to ECC RAM bank (MSC_EccBank_Typedef)
 ******************************************************************************/
static void mscEccReadWriteExistingPio(const MSC_EccBank_Typedef *eccBank)
{
  volatile uint32_t *ramptr = (volatile uint32_t *) eccBank->base;
  const uint32_t *endptr = (const uint32_t *) (eccBank->base + eccBank->size);
  uint32_t val32;
  uint32_t ctrlReg = ECC_CTRL_REG;

  // Make sure ECC bit error interrupt event bits are cleared.
  ECC_IFC_REG = ECC_IFC_MASK;

  // Loop through all 32-bit words in RAM block.
  for (; ramptr < endptr; ramptr++) {
    // Read value from RAM
    val32 = *ramptr;
    if (ECC_IF_REG & ECC_IF_1BIT_ERROR) {
      /* 1-bit error occurred. The read value is incorrect since the ECC logic
         has modified it. Disable ECC, re-read correct value from RAM,
         re-enable ECC, and finally write value which will also initialize the
         corresponding ECC syndrome. */
      ctrlReg &= ~eccBank->initSyndromeEnable;
      ECC_CTRL_REG = ctrlReg;
      val32 = *ramptr;
      // Re-enable ECC
      ctrlReg |= eccBank->initSyndromeEnable;
      ECC_CTRL_REG = ctrlReg;
      ECC_IFC_REG = ECC_IFC_MASK;
    }
    *ramptr = val32;
  }
}

#else // #if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_2)

/***************************************************************************//**
 * @brief
 *    DMA read and write existing values (for ECC initializaion).
 *
 * @details
 *    This function uses DMA to read and write the existing data values in
 *    the RAM region specified by start and size. The function will use the
 *    2 DMA channels specified by the channels[2] array.
 *
 * @param[in] start
 *    Start address of address range in RAM to read/write.
 *
 * @param[in] size
 *    Size of address range in RAM to read/write.
 *
 * @param[in] channels[2]
 *    Array of 2 DMA channels to use.
 ******************************************************************************/
static void mscEccReadWriteExistingDma(uint32_t start,
                                       uint32_t size,
                                       uint32_t channels[2])
{
  uint32_t descCnt = 0;
  volatile uint32_t dmaDesc[ECC_DMA_DESCS][4];
  uint32_t chMask = (1 << channels[0]) | (1 << channels[1]);
  /* Assert that the 2 DMA channel numbers are different. */
  EFM_ASSERT(channels[0] != channels[1]);

  /* Make sure ECC_RAM_SIZE_MAX is a multiple of ECC_DMA_DESC_SIZE in order
     to match the total xfer size of the descriptor chain with the largest
     ECC RAM bank. */
  EFM_ASSERT((ECC_RAM_SIZE_MAX % ECC_DMA_DESC_SIZE) == 0);

  /* Initialize LDMA descriptor chain. */
  do {
    dmaDesc[descCnt][0] =                 /* DMA desc CTRL word */
                          LDMA_CH_CTRL_STRUCTTYPE_TRANSFER
                          | LDMA_CH_CTRL_STRUCTREQ
                          | _LDMA_CH_CTRL_XFERCNT_MASK
                          | LDMA_CH_CTRL_BLOCKSIZE_ALL
                          | LDMA_CH_CTRL_REQMODE_ALL
                          | LDMA_CH_CTRL_SRCINC_ONE
                          | LDMA_CH_CTRL_SIZE_WORD
                          | LDMA_CH_CTRL_DSTINC_ONE;

    /* source and destination address */
    dmaDesc[descCnt][1] = start;
    dmaDesc[descCnt][2] = start;
    /* link to next descriptor */
    dmaDesc[descCnt][3] = LDMA_CH_LINK_LINK
                          | (((uint32_t) &dmaDesc[descCnt + 1][0])
                             & _LDMA_CH_LINK_LINKADDR_MASK);

    start += ECC_DMA_DESC_SIZE;
    size  -= ECC_DMA_DESC_SIZE;
    descCnt++;
  } while (size);

  /* Make sure descCnt is valid to avoid out-of-bounds access when writing to
     dmaDesc array. */
  if ((descCnt < 2) || (descCnt > ECC_DMA_DESCS)) {
    while (true) {
      EFM_ASSERT(false);
    }
  }

  /* Now, divide the descriptor list in two parts, one for each channel,
     by setting the link bit and address 0 of the descriptor in the middle
     to 0. */
  dmaDesc[(descCnt / 2) - 1][3] = 0;

  /* Set last descriptor link bit and address to 0. */
  dmaDesc[descCnt - 1][3] = 0;

#if !defined(_SILICON_LABS_32B_SERIES_2)
  /* Start the LDMA clock now */
  CMU_ClockEnable(cmuClock_LDMA, true);
#endif

  /* Round robin scheduling for all channels (0 = no fixed priority channels).
   */
  LDMA->CTRL    = 0 << _LDMA_CTRL_NUMFIXED_SHIFT;
#if defined(LDMA_EN_EN)
  LDMA->EN      = LDMA_EN_EN;
#endif
  LDMA->CHEN    = 0;
  LDMA->DBGHALT = 0;
  LDMA->REQDIS  = 0;

  /* Disable LDMA interrupts, and clear interrupt status. */
  LDMA->IEN = 0;
#if defined (LDMA_HAS_SET_CLEAR)
  LDMA->IF_CLR = chMask;
#else
  LDMA->IFC = chMask;
#endif

  /* Disable looping */
  LDMA->CH[channels[0]].LOOP = 0;
  LDMA->CH[channels[1]].LOOP = 0;

  /* Set descriptor address for first channel. */
  LDMA->CH[channels[0]].LINK = ((uint32_t)&dmaDesc[0][0])
                               & _LDMA_CH_LINK_LINKADDR_MASK;
  /* Set descriptor address for second channel. */
  LDMA->CH[channels[1]].LINK = ((uint32_t)&dmaDesc[descCnt / 2][0])
                               & _LDMA_CH_LINK_LINKADDR_MASK;
  /* Clear the channel done flags.  */
  BUS_RegMaskedClear(&LDMA->CHDONE, chMask);

  /* Start transfer by loading descriptors.  */
  LDMA->LINKLOAD = chMask;

  /* Wait until finished. */
  while (!(
#if defined(_LDMA_CHSTATUS_MASK)
           ((LDMA->CHSTATUS & chMask) == 0)
#else
           ((LDMA->CHEN & chMask) == 0)
#endif
           && ((LDMA->CHDONE & chMask) == chMask))) {
  }

#if !defined(_SILICON_LABS_32B_SERIES_2)
  /* Stop the LDMA clock now */
  CMU_ClockEnable(cmuClock_LDMA, false);
#endif
}

#endif // #if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_2)

/***************************************************************************//**
 * @brief
 *    Initialize ECC for a given memory bank.
 *
 * @brief
 *    This function initializes ECC for a given memory bank which is specified
 *    with the MSC_EccBank_Typedef structure input parameter.
 *
 * @param[in] eccBank
 *    ECC memory bank device structure.
 *
 * @param[in] dmaChannels
 *    Array of 2 DMA channels that may be used during ECC initialization.
 *
 ******************************************************************************/
static void mscEccBankInit(const MSC_EccBank_Typedef *eccBank,
                           uint32_t dmaChannels[2])
{
  uint32_t ctrlReg;

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  /* Enable ECC write. Keep ECC checking disabled during initialization. */
  ctrlReg  = ECC_CTRL_REG;
  ctrlReg |= eccBank->initSyndromeEnable;
  ECC_CTRL_REG = ctrlReg;

#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_2)
  (void) dmaChannels;
  /* Initialize ECC syndromes by using core cpu to load and store the existing
     data values in RAM. */
  mscEccReadWriteExistingPio(eccBank);
#else
  /* Initialize ECC syndromes by using DMA to read and write the existing
     data values in RAM. */
  mscEccReadWriteExistingDma(eccBank->base, eccBank->size, dmaChannels);
#endif

  /* Clear any ECC errors that may have been reported before or during
     initialization. */
  ECC_IFC_REG = ECC_IFC_MASK;

  /* Enable ECC decoder to detect and report ECC errors. */
  ctrlReg |= eccBank->correctionEnable;
  ECC_CTRL_REG = ctrlReg;

  CORE_EXIT_CRITICAL();
}

/***************************************************************************//**
 * @brief
 *    Disable ECC for a given memory bank.
 *
 * @brief
 *    This function disables ECC for a given memory bank which is specified
 *    with the MSC_EccBank_Typedef structure input parameter.
 *
 * @param[in] eccBank
 *    ECC memory bank device structure.
 *
 ******************************************************************************/
static void mscEccBankDisable(const MSC_EccBank_Typedef *eccBank)
{
  /* Disable ECC write (encoder) and checking (decoder). */
  ECC_CTRL_REG &= ~(eccBank->initSyndromeEnable | eccBank->correctionEnable);
}

/***************************************************************************//**
 * @brief
 *   Configure Error Correcting Code (ECC)
 *
 * @details
 *   This function configures ECC support according to the configuration
 *   input parameter. If the user requests enabling ECC for a given RAM bank
 *   this function will initialize ECC memory (syndromes) for the bank by
 *   reading and writing the existing values in memory. I.e. all data is
 *   preserved. The initialization process runs in a critical section
 *   disallowing interrupts and thread scheduling, and will consume a
 *   considerable amount of clock cycles. Therefore the user should carefully
 *   assess where to call this function. The user can consider to increase
 *   the clock frequency in order to reduce the execution time.
 *   This function makes use of 2 DMA channels to move data to/from RAM in an
 *   efficient way. The user can select which 2 DMA channels to use in order
 *   to avoid conflicts with the application. However the user must make sure
 *   that no other DMA operations takes place while this function is executing.
 *   If the application has been using the DMA controller prior to calling this
 *   function, the application will need to reinitialize DMA registers after
 *   this function has completed.
 *
 * @note
 *   This function protects the ECC initialization procedure from interrupts
 *   and other threads by using a critical section (defined by em_core.h)
 *   When running on RTOS the user may need to override CORE_EnterCritical
 *   CORE_ExitCritical which are declared as 'SL_WEAK' in em_core.c.
 *
 * @param[in] eccConfig
 *   ECC configuration
 ******************************************************************************/
void MSC_EccConfigSet(MSC_EccConfig_TypeDef *eccConfig)
{
  unsigned int cnt;
#if defined(ECC_FAULT_CTRL_REG)
  uint32_t faultCtrlReg = ECC_FAULT_CTRL_REG;
  /* Disable ECC faults if ecc fault ctrl register is defined. */
  faultCtrlReg &= ~ECC_FAULT_EN;
  ECC_FAULT_CTRL_REG = faultCtrlReg;
#endif

  /* Loop through the ECC banks array, enable or disable according to
     the eccConfig->enableEccBank array. */
  for (cnt = 0; cnt < MSC_ECC_BANKS; cnt++) {
    if (eccConfig->enableEccBank[cnt]) {
      mscEccBankInit(&eccBankTbl[cnt], eccConfig->dmaChannels);
    } else {
      mscEccBankDisable(&eccBankTbl[cnt]);
    }
  }

#if defined(ECC_FAULT_CTRL_REG) && !defined(_SILICON_LABS_32B_SERIES_1_CONFIG_1)
  /* Enable ECC faults if ecc fault ctrl register is set.
     On Series 1 Config 1, aka EFM32GG11, ECC faults should stay disabled. */
  faultCtrlReg |= ECC_FAULT_EN;
  ECC_FAULT_CTRL_REG = faultCtrlReg;
#endif
}

#endif /* #if defined(_MSC_ECCCTRL_MASK) */

/** @} (end addtogroup MSC) */
/** @} (end addtogroup emlib) */
#endif /* defined(MSC_COUNT) && (MSC_COUNT > 0) */
