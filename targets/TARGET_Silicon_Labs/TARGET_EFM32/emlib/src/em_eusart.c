/***************************************************************************//**
 * @file
 * @brief Universal asynchronous receiver/transmitter (EUSART) peripheral API
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "em_eusart.h"
#if defined(EUART_COUNT) && (EUART_COUNT > 0)
#include "em_cmu.h"
#include <stddef.h>

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup EUSART
 * @{
 ******************************************************************************/

/*******************************************************************************
 *********************************   DEFINES   *********************************
 ******************************************************************************/

#if defined(EUART_PRESENT)
  #define EUSART_REF_VALID(ref)    ((ref) == EUART0)
  #define EUSART_EM2_CAPABLE(ref)  (true)
  #define EUSART_RX_FIFO_SIZE  4u
#endif

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/*******************************************************************************
 **************************   LOCAL FUNCTIONS   ********************************
 ******************************************************************************/

static void EUSART_AsyncInitCommon(EUSART_TypeDef *eusart,
                                   const EUSART_UartInit_TypeDef *init,
                                   const EUSART_IrDAInit_TypeDef *irdaInit);

/***************************************************************************//**
 * Wait for ongoing sync of register(s) to the low-frequency domain to complete.
 *
 * @param eusart Pointer to the EUSART peripheral register block.
 * @param mask A bitmask corresponding to SYNCBUSY register defined bits,
 *             indicating registers that must complete any ongoing
 *             synchronization.
 ******************************************************************************/
__STATIC_INLINE void eusart_sync(EUSART_TypeDef *eusart, uint32_t mask)
{
  // Wait for any pending previous write operation to have been completed
  // in the low-frequency domain.
  while ((eusart->SYNCBUSY & mask) != 0U) {
  }
}

/***************************************************************************//**
 *   Calculate baudrate for a given reference frequency, clock division,
 *   and oversampling rate.
 ******************************************************************************/
__STATIC_INLINE uint32_t EUSART_BaudrateCalc(uint32_t refFreq,
                                             uint32_t clkdiv,
                                             EUSART_OVS_TypeDef ovs);

/** @endcond */

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * Initializes the EUSART when used with the high frequency clock.
 ******************************************************************************/
void EUSART_UartInitHf(EUSART_TypeDef *eusart, const EUSART_UartInit_TypeDef *init)
{
  // Make sure the module exists on the selected chip.
  EFM_ASSERT(EUSART_REF_VALID(eusart));
  // Init structure must be provided.
  EFM_ASSERT(init);

  // Assert features specific to HF.
  // The oversampling must not be disabled when using a high frequency clock.
  EFM_ASSERT(init->oversampling != eusartOVS0);

  // Initialize EUSART with common features to HF and LF.
  EUSART_AsyncInitCommon(eusart, init, NULL);
}

/***************************************************************************//**
 * Initializes the EUSART when used with the low frequency clock.
 *
 * @note (1) When EUSART oversampling is set to eusartOVS0 (Disable), the peripheral
 *           clock frequency must be at least three times higher than the
 *           chosen baud rate. In LF, max input clock is 32768 (LFXO or LFRCO),
 *           thus 32768 / 3 ~ 9600 baudrate.
 ******************************************************************************/
void EUSART_UartInitLf(EUSART_TypeDef *eusart, const EUSART_UartInit_TypeDef *init)
{
  // Make sure the module exists and is Low frequency capable.
  EFM_ASSERT(EUSART_REF_VALID(eusart) && EUSART_EM2_CAPABLE(EUSART_NUM(eusart)));
  // Init structure must be provided.
  EFM_ASSERT(init);

  // Assert features specific to LF.
  // LFXO, LFRCO, ULFRCO can be a clock source in LF.
#if defined(DEBUG_EFM) || defined(DEBUG_EFM_USER)
  {
    CMU_Select_TypeDef clock_source;
    clock_source = CMU_ClockSelectGet(cmuClock_EUART0);
    EFM_ASSERT((clock_source == cmuSelect_LFRCO) || (clock_source == cmuSelect_ULFRCO) || (clock_source == cmuSelect_LFXO));
  }
#endif
  // The oversampling must be disabled when using a low frequency clock.
  EFM_ASSERT(init->oversampling == eusartOVS0);
  // The Mojority Vote must be disabled when using a low frequency clock.
  EFM_ASSERT(init->majorityVote == eusartMajorityVoteDisable);
  // Number of stop bits can only be 1 or 2 in LF.
  EFM_ASSERT((init->stopbits == eusartStopbits1) || (init->stopbits == eusartStopbits2));
  // In LF, max baudrate is 9600. See Note #1.
  EFM_ASSERT(init->baudrate <= 9600 && init->baudrate != 0);

  // Initialize EUSART with common features to HF and LF.
  EUSART_AsyncInitCommon(eusart, init, NULL);
}

/***************************************************************************//**
 * Initializes the EUSART when used in IrDA mode with the high or low
 * frequency clock.
 ******************************************************************************/
void EUSART_IrDAInit(EUSART_TypeDef *eusart,
                     const EUSART_IrDAInit_TypeDef *irdaInit)
{
  // Make sure the module exists on the selected chip.
  EFM_ASSERT(EUSART_REF_VALID(eusart));
  // Init structure must be provided.
  EFM_ASSERT(irdaInit);

  if (irdaInit->irDALowFrequencyEnable) {
    // Validate the low frequency capability of the EUSART instance.
    EFM_ASSERT(EUSART_EM2_CAPABLE(EUSART_NUM(eusart)));
    // The oversampling must be disabled when using a low frequency clock.
    EFM_ASSERT(irdaInit->init.oversampling == eusartOVS0);
    // Number of stop bits can only be 1 or 2 in LF.
    EFM_ASSERT((irdaInit->init.stopbits == eusartStopbits1) || (irdaInit->init.stopbits == eusartStopbits2));
    // In LF, max baudrate is 9600. See Note #1.
    EFM_ASSERT(irdaInit->init.baudrate <= 9600);
    EFM_ASSERT(irdaInit->init.enable == eusartEnableRx || irdaInit->init.enable == eusartDisable);
  } else {
    EFM_ASSERT(irdaInit->init.oversampling != eusartOVS0);
    // In HF, 2.4 kbps <= baudrate <= 1.152 Mbps.
    EFM_ASSERT(irdaInit->init.baudrate >= 2400 && irdaInit->init.baudrate <= 1152000);
  }

  // Initialize EUSART with common features to HF and LF.
  EUSART_AsyncInitCommon(eusart, &irdaInit->init, irdaInit);
}

/***************************************************************************//**
 * Configure the EUSART to its reset state.
 ******************************************************************************/
void EUSART_Reset(EUSART_TypeDef *eusart)
{
  uint8_t i = 0;
  // Make sure the module exists on the selected chip.
  EFM_ASSERT(EUSART_REF_VALID(eusart));

  // Enable peripheral to reset internal state.
  eusart->EN_SET = EUSART_EN_EN;
  eusart_sync(eusart, (EUSART_SYNCBUSY_RXDIS | EUSART_SYNCBUSY_TXDIS | EUSART_SYNCBUSY_TXTRIDIS | EUSART_SYNCBUSY_RXBLOCKDIS));
  eusart->CMD = EUSART_CMD_RXDIS | EUSART_CMD_TXDIS | EUSART_CMD_TXTRIDIS | EUSART_CMD_CLEARTX | EUSART_CMD_RXBLOCKDIS;
  eusart_sync(eusart, (EUSART_SYNCBUSY_RXDIS | EUSART_SYNCBUSY_TXDIS | EUSART_SYNCBUSY_TXTRIDIS | EUSART_SYNCBUSY_RXBLOCKDIS));
  eusart->CLKDIV = _EUSART_CLKDIV_RESETVALUE;
  eusart_sync(eusart, _EUSART_SYNCBUSY_DIV_MASK);
  // Clear Rx FIFO.
  for (i = 0; i < EUSART_RX_FIFO_SIZE; i++) {
    eusart->RXDATA;
  }

  // Ensure that the peripheral is disabled while configuring.
  eusart->EN_CLR = EUSART_EN_EN;

  // Set all configurable register to its reset value.
  eusart->CFG0 = _EUSART_CFG0_RESETVALUE;
  eusart->CFG1 = _EUSART_CFG1_RESETVALUE;
  eusart->FRAMECFG = _EUSART_FRAMECFG_RESETVALUE;
  eusart->IRHFCFG = _EUSART_IRHFCFG_RESETVALUE;
  eusart->IRLFCFG = _EUSART_IRLFCFG_RESETVALUE;
  eusart->TIMINGCFG = _EUSART_TIMINGCFG_RESETVALUE;
  eusart->STARTFRAMECFG = _EUSART_STARTFRAMECFG_RESETVALUE;
  eusart->SIGFRAMECFG = _EUSART_SIGFRAMECFG_RESETVALUE;
  eusart->TRIGCTRL = _EUSART_TRIGCTRL_RESETVALUE;
  eusart->IEN = _EUSART_IEN_RESETVALUE;
  eusart->IF_CLR = _EUSART_IF_MASK;;
}

/***************************************************************************//**
 * Enables/disables the EUSART receiver and/or transmitter.
 ******************************************************************************/
void EUSART_Enable(EUSART_TypeDef *eusart, EUSART_Enable_TypeDef enable)
{
  uint32_t tmp = 0;;

  // Make sure that the module exists on the selected chip.
  EFM_ASSERT(EUSART_REF_VALID(eusart));

  if (enable == eusartDisable) {
    eusart->EN_CLR = EUSART_EN_EN;
  } else {
    eusart->EN_SET = EUSART_EN_EN;

    tmp = (enable)
          & (_EUSART_CMD_RXEN_MASK | _EUSART_CMD_TXEN_MASK
             | _EUSART_CMD_RXDIS_MASK | _EUSART_CMD_TXDIS_MASK);

    eusart_sync(eusart, _EUSART_SYNCBUSY_MASK); // Wait for low frequency register synchronization.
    eusart->CMD = tmp;
    eusart_sync(eusart,
                EUSART_SYNCBUSY_RXEN | EUSART_SYNCBUSY_TXEN
                | EUSART_SYNCBUSY_RXDIS | EUSART_SYNCBUSY_TXDIS); // Wait for low frequency register synchronization.

    tmp = 0;
    if (_EUSART_CMD_RXEN_MASK & enable) {
      tmp |= EUSART_STATUS_RXENS;
    }
    if (_EUSART_CMD_TXEN_MASK & enable) {
      tmp |= EUSART_STATUS_TXENS;
    }
    while ((eusart->STATUS & (_EUSART_STATUS_TXENS_MASK | _EUSART_STATUS_RXENS_MASK)) != tmp) {
    } // Wait for the status register to be updated.
  }
}

/***************************************************************************//**
 * Receives one 8 bit frame, (or part of 9 bit frame).
 ******************************************************************************/
uint8_t EUSART_Rx(EUSART_TypeDef *eusart)
{
  while (!(eusart->STATUS & EUSART_STATUS_RXFL)) {
  } // Wait for incoming data.

  return (uint8_t)eusart->RXDATA;
}
/***************************************************************************//**
 * Receives one 8-9 bit frame with extended information.
 ******************************************************************************/
uint16_t EUSART_RxExt(EUSART_TypeDef *eusart)
{
  while (!(eusart->STATUS & EUSART_STATUS_RXFL)) {
  } // Wait for incoming data.

  return (uint16_t)eusart->RXDATA;
}

/***************************************************************************//**
 * Transmits one frame.
 ******************************************************************************/
void EUSART_Tx(EUSART_TypeDef *eusart, uint8_t data)
{
  // Check that transmit FIFO is not full.
  while (!(eusart->STATUS & EUSART_STATUS_TXFL)) {
  }

  eusart->TXDATA = (uint32_t)data;
}

/***************************************************************************//**
 * Transmits one 8-9 bit frame with extended control.
 ******************************************************************************/
void EUSART_TxExt(EUSART_TypeDef *eusart, uint16_t data)
{
  // Check that transmit FIFO is not full.
  while (!(eusart->STATUS & EUSART_STATUS_TXFL)) {
  }

  eusart->TXDATA = (uint32_t)data;
}

/***************************************************************************//**
 * Configures the baudrate (or as close as possible to a specified baudrate).
 *
 * @note (1) When the oversampling is disabled, the peripheral clock frequency
 *           must be at least three times higher than the chosen baud rate.
 ******************************************************************************/
void EUSART_BaudrateSet(EUSART_TypeDef *eusart,
                        uint32_t refFreq,
                        uint32_t baudrate)
{
  uint32_t          clkdiv;
  CMU_Clock_TypeDef clock;
  uint8_t           oversample;

  // Prevent dividing by 0.
  EFM_ASSERT(baudrate);

  // Make sure the module exists on the selected chip.
  EFM_ASSERT(EUSART_REF_VALID(eusart));

  // The peripheral must be enabled to configure the baud rate.
  EFM_ASSERT(eusart->EN == EUSART_EN_EN);

  /*
   * Use integer division to avoid forcing in float division
   * utils, and yet keep rounding effect errors to a minimum.
   *
   * CLKDIV is given by:
   *
   * CLKDIV = 256 * (fUARTn/(oversample * br) - 1)
   * or
   * CLKDIV = (256 * fUARTn)/(oversample * br) - 256
   *
   * Since fUARTn may be derived from HFCORECLK, consider the overflow when
   * using integer arithmetic.
   *
   * The basic problem with integer division in the above formula is that
   * the dividend (256 * fUARTn) may become higher than the maximum 32 bit
   * integer. Yet, the dividend should be evaluated first before dividing
   * to get as small rounding effects as possible.
   * Also, harsh restrictions on the maximum fUARTn value should not be made.
   *
   * Since the last 3 bits of CLKDIV are don't care, base the
   * integer arithmetic on the below formula:
   *
   * CLKDIV/8 = ((32*fUARTn)/(br * Oversample)) - 32
   *
   * and calculate 1/8 of CLKDIV first. This allows for fUARTn
   * up to 128 MHz without overflowing a 32 bit value.
   */

  // Get the current frequency.
  if (!refFreq) {
    if (eusart == EUART0) {
      clock = cmuClock_EUART0;
    } else {
      EFM_ASSERT(0);
      return;
    }

    refFreq = CMU_ClockFreqGet(clock);
  }

  // Map oversampling.
  switch (eusart->CFG0 & _EUSART_CFG0_OVS_MASK) {
    case eusartOVS16:
      EFM_ASSERT(baudrate <= (refFreq / 16));
      oversample = 16;
      break;

    case eusartOVS8:
      EFM_ASSERT(baudrate <= (refFreq / 8));
      oversample = 8;
      break;

    case eusartOVS6:
      EFM_ASSERT(baudrate <= (refFreq / 6));
      oversample = 6;
      break;

    case eusartOVS4:
      EFM_ASSERT(baudrate <= (refFreq / 4));
      oversample = 4;
      break;

    case eusartOVS0:
      EFM_ASSERT(refFreq >= (3 * baudrate)); // See Note #1.
      oversample = 1;
      break;

    default:
      // Invalid input
      EFM_ASSERT(0);
      return;
  }

  // Calculate and set the CLKDIV with fractional bits.
  clkdiv  = (32 * refFreq) / (baudrate * oversample);
  clkdiv -= 32;
  clkdiv *= 8;

  // Verify that the resulting clock divider is within limits.
  EFM_ASSERT(clkdiv <= _EUSART_CLKDIV_MASK);

  // If the EFM_ASSERT is not enabled, make sure not to write to reserved bits.
  clkdiv &= _EUSART_CLKDIV_MASK;

  eusart_sync(eusart, _EUSART_SYNCBUSY_DIV_MASK); // Wait for low frequency register synchronization.
  eusart->CLKDIV = clkdiv;
  eusart_sync(eusart, _EUSART_SYNCBUSY_DIV_MASK); // Wait for low frequency register synchronization.
}

/***************************************************************************//**
 * Gets the current baudrate.
 ******************************************************************************/
uint32_t EUSART_BaudrateGet(EUSART_TypeDef *eusart)
{
  uint32_t freq;
  CMU_Clock_TypeDef clock;
  EUSART_OVS_TypeDef ovs;

  // Make sure the module exists on the selected chip.
  EFM_ASSERT(EUSART_REF_VALID(eusart));

  if (eusart == EUART0) {
    clock = cmuClock_EUART0;
  } else {
    EFM_ASSERT(0);
    return 0u;
  }
  ovs = (EUSART_OVS_TypeDef)(eusart->CFG0 & _EUSART_CFG0_OVS_MASK);
  freq = CMU_ClockFreqGet(clock);

  return EUSART_BaudrateCalc(freq, eusart->CLKDIV, ovs);
}

/***************************************************************************//**
 * Enable/Disable reception operations until the configured start frame is
 * received.
 ******************************************************************************/
void  EUSART_RxBlock(EUSART_TypeDef *eusart, EUSART_BlockRx_TypeDef enable)
{
  uint32_t tmp;

  // Make sure that the module exists on the selected chip.
  EFM_ASSERT(EUSART_REF_VALID(eusart));

  tmp   = ((uint32_t)(enable));
  tmp  &= (_EUSART_CMD_RXBLOCKEN_MASK | _EUSART_CMD_RXBLOCKDIS_MASK);

  eusart_sync(eusart, EUSART_SYNCBUSY_RXBLOCKEN | EUSART_SYNCBUSY_RXBLOCKDIS); // Wait for low frequency register synchronization.
  eusart->CMD = tmp;
  eusart_sync(eusart, EUSART_SYNCBUSY_RXBLOCKEN | EUSART_SYNCBUSY_RXBLOCKDIS); // Wait for low frequency register synchronization.

  tmp = 0u;
  if ((_EUSART_CMD_RXBLOCKEN_MASK & enable) != 0u) {
    tmp |= EUSART_STATUS_RXBLOCK;
  }
  while ((eusart->STATUS & _EUSART_STATUS_RXBLOCK_MASK) != tmp) {
  } // Wait for the status register to be updated.
}

/***************************************************************************//**
 * Enables/Disables the tristating of the transmitter output.
 ******************************************************************************/
void  EUSART_TxTristateSet(EUSART_TypeDef *eusart,
                           EUSART_TristateTx_TypeDef enable)
{
  uint32_t tmp;

  // Make sure that the module exists on the selected chip.
  EFM_ASSERT(EUSART_REF_VALID(eusart));

  tmp   = ((uint32_t)(enable));
  tmp  &= (_EUSART_CMD_TXTRIEN_MASK | _EUSART_CMD_TXTRIDIS_MASK);

  eusart_sync(eusart, EUSART_SYNCBUSY_TXTRIEN | EUSART_SYNCBUSY_TXTRIDIS); // Wait for low frequency register synchronization.
  eusart->CMD = tmp;
  eusart_sync(eusart, EUSART_SYNCBUSY_TXTRIEN | EUSART_SYNCBUSY_TXTRIDIS); // Wait for low frequency register synchronization.

  tmp = 0u;
  if ((_EUSART_CMD_TXTRIEN_MASK & enable) != 0u) {
    tmp |= EUSART_STATUS_TXTRI;
  }
  while ((eusart->STATUS & _EUSART_STATUS_TXTRI_MASK) != tmp) {
  } // Wait for the status register to be updated.
}

/***************************************************************************//**
 * Initializes the automatic enabling of transmissions and/or reception using
 * the PRS as a trigger.
 ******************************************************************************/
void EUSART_PrsTriggerEnable(EUSART_TypeDef *eusart,
                             const EUSART_PrsTriggerInit_TypeDef *init)
{
  uint32_t tmp;

  // Make sure that the module exists on the selected chip.
  EFM_ASSERT(EUSART_REF_VALID(eusart));

  // The peripheral must be enabled to configure the PRS trigger.
  EFM_ASSERT(eusart->EN == EUSART_EN_EN);

  PRS->CONSUMER_EUART0_TRIGGER_SET = (init->prs_trigger_channel & _PRS_CONSUMER_EUART0_TRIGGER_MASK);

  tmp   = ((uint32_t)(init->prs_trigger_enable));
  tmp  &= (_EUSART_TRIGCTRL_RXTEN_MASK | _EUSART_TRIGCTRL_TXTEN_MASK);

  eusart->TRIGCTRL_SET = tmp;
  eusart_sync(eusart, EUSART_SYNCBUSY_RXTEN | EUSART_SYNCBUSY_TXTEN); // Wait for low frequency register synchronization.

  tmp   = ~((uint32_t)(init->prs_trigger_enable));
  tmp  &= (_EUSART_TRIGCTRL_RXTEN_MASK | _EUSART_TRIGCTRL_TXTEN_MASK);
  eusart->TRIGCTRL_CLR = tmp;
  eusart_sync(eusart, EUSART_SYNCBUSY_RXTEN | EUSART_SYNCBUSY_TXTEN); // Wait for low frequency register synchronization.
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/***************************************************************************//**
 * Calculate baudrate for a given reference frequency, clock division,
 * and oversampling rate.
 *
 * @param refFreq The EUSART reference clock frequency in Hz that will be used.
 * @param clkdiv Clock division factor to be used.
 * @param ovs Oversampling to be used.
 *
 * @return Computed baudrate from given settings.
 ******************************************************************************/
__STATIC_INLINE uint32_t EUSART_BaudrateCalc(uint32_t refFreq,
                                             uint32_t clkdiv,
                                             EUSART_OVS_TypeDef ovs)
{
  uint32_t oversample;
  uint64_t divisor;
  uint64_t factor;
  uint64_t remainder;
  uint64_t quotient;
  uint32_t br;

  // Out of bound clkdiv.
  EFM_ASSERT(clkdiv <= _EUSART_CLKDIV_MASK);

  // Mask out unused bits
  clkdiv &= _EUSART_CLKDIV_MASK;

  /* Use integer division to avoid forcing in float division
   * utils and yet keep rounding effect errors to a minimum.
   *
   * Baudrate in is given by:
   *
   * br = fUARTn/(oversample * (1 + (CLKDIV / 256)))
   * or
   * br = (256 * fUARTn)/(oversample * (256 + CLKDIV))
   *
   * 256 factor of the dividend is reduced with a
   * (part of) oversample part of the divisor.
   */

  switch (ovs) {
    case eusartOVS16:
      oversample = 1;
      factor = 256 / 16;
      break;

    case eusartOVS8:
      oversample = 1;
      factor = 256 / 8;
      break;

    case eusartOVS6:
      oversample = 3;
      factor = 256 / 2;
      break;

    case eusartOVS4:
      oversample = 1;
      factor = 256 / 4;
      break;

    case eusartOVS0:
      oversample = 1;
      factor = 256;
      break;

    default:
      return 0u;
      break;
  }

  /*
   * The basic problem with integer division in the above formula is that
   * the dividend (factor * fUARTn) may become larger than a 32 bit
   * integer. Yet we want to evaluate the dividend first before dividing
   * to get as small rounding effects as possible. Too harsh restrictions
   * should not be made on the maximum fUARTn value either.
   *
   * For division a/b,
   *
   * a = qb + r
   *
   * where q is the quotient and r is the remainder, both integers.
   *
   * The original baudrate formula can be rewritten as
   *
   * br = xa / b = x(qb + r)/b = xq + xr/b
   *
   * where x is 'factor', a is 'refFreq' and b is 'divisor', referring to
   * variable names.
   */

  /*
   * The divisor will never exceed max 32 bit value since
   * clkdiv <= _EUSART_CLKDIV_MASK (currently 0x7FFFF8)
   * and 'oversample' has been reduced to <= 3.
   */
  divisor = oversample * (256 + clkdiv);

  quotient = refFreq / divisor;
  remainder = refFreq % divisor;

  // The factor <= 128 and since divisor >= 256, the below cannot exceed the maximum
  // 32 bit value. However, factor * remainder can become larger than 32-bit
  // because of the size of _EUSART_CLKDIV_DIV_MASK on some families.
  br = (uint32_t) (factor * quotient);

  /*
   * The factor <= 128 and remainder < (oversample*(256 + clkdiv)), which
   * means dividend (factor * remainder) worst case is
   * 128 * (3 * (256 + _EUSART_CLKDIV_MASK)) = 0xC001_7400.
   */
  br += (uint32_t) ((factor * remainder) / divisor);

  return br;
}

/***************************************************************************//**
 * Initializes the EUSART with asynchronous common settings to high
 * and low frequency clock.
 *
 * @param eusart Pointer to the EUSART peripheral register block.
 * @param init A pointer to the initialization structure.
 * @param irdaInit Pointer to IrDA initialization structure.
 ******************************************************************************/
static void EUSART_AsyncInitCommon(EUSART_TypeDef *eusart,
                                   const EUSART_UartInit_TypeDef  *init,
                                   const EUSART_IrDAInit_TypeDef  *irdaInit)
{
  // LF register about to be modified requires sync busy check.
  if (eusart->EN) {
    eusart_sync(eusart, _EUSART_SYNCBUSY_MASK);
  }
  // Initialize EUSART registers to hardware reset state.
  EUSART_Reset(eusart);

  // Configure frame format
  eusart->FRAMECFG = (eusart->FRAMECFG & ~(_EUSART_FRAMECFG_DATABITS_MASK
                                           | _EUSART_FRAMECFG_STOPBITS_MASK
                                           | _EUSART_FRAMECFG_PARITY_MASK))
                     | (uint32_t)(init->databits)
                     | (uint32_t)(init->parity)
                     | (uint32_t)(init->stopbits);

  // Configure global configuration register 0.
  eusart->CFG0 = (eusart->CFG0 & ~(_EUSART_CFG0_OVS_MASK
                                   | _EUSART_CFG0_LOOPBK_MASK
                                   | _EUSART_CFG0_MVDIS_MASK))
                 | (uint32_t)(init->oversampling)
                 | (uint32_t)(init->loopbackEnable)
                 | (uint32_t)(init->majorityVote);

  if (init->baudrate == 0) {
    eusart->CFG0 |= EUSART_CFG0_AUTOBAUDEN;
  }

  if (init->advancedSettings) {
    eusart->CFG0 |= (uint32_t)init->advancedSettings->dmaHaltOnError << _EUSART_CFG0_ERRSDMA_SHIFT;
    eusart->CFG0 |= (uint32_t)init->advancedSettings->txAutoTristate << _EUSART_CFG0_AUTOTRI_SHIFT;
    eusart->CFG0 |= (uint32_t)init->advancedSettings->invertIO & (_EUSART_CFG0_RXINV_MASK | _EUSART_CFG0_TXINV_MASK);
    eusart->CFG0 |= (uint32_t)init->advancedSettings->collisionDetectEnable << _EUSART_CFG0_CCEN_SHIFT;
    eusart->CFG0 |= (uint32_t)init->advancedSettings->multiProcessorEnable << _EUSART_CFG0_MPM_SHIFT;
    eusart->CFG0 |= (uint32_t)init->advancedSettings->multiProcessorAddressBitHigh << _EUSART_CFG0_MPAB_SHIFT;
    eusart->CFG0 |= (uint32_t)init->advancedSettings->msbFirst << _EUSART_CFG0_MSBF_SHIFT;

    // Configure global configuration register 1.
    eusart->CFG1 = (uint32_t)init->advancedSettings->dmaWakeUpOnRx << _EUSART_CFG1_RXDMAWU_SHIFT
                   | (uint32_t)init->advancedSettings->dmaWakeUpOnTx << _EUSART_CFG1_TXDMAWU_SHIFT;

    if (init->advancedSettings->hwFlowControl == eusartHwFlowControlCts
        || init->advancedSettings->hwFlowControl == eusartHwFlowControlCtsAndRts) {
      eusart->CFG1 |= EUSART_CFG1_CTSEN;
    }
    // Enable RTS route pin if necessary. CTS is an input so it is enabled by default.
    if ((init->advancedSettings->hwFlowControl == eusartHwFlowControlRts)
        || (init->advancedSettings->hwFlowControl == eusartHwFlowControlCtsAndRts)) {
      GPIO->EUARTROUTE_SET->ROUTEEN = GPIO_EUART_ROUTEEN_RTSPEN;
    } else {
      GPIO->EUARTROUTE_CLR->ROUTEEN = GPIO_EUART_ROUTEEN_RTSPEN;
    }
    eusart->STARTFRAMECFG_SET = (uint32_t)init->advancedSettings->startFrame;
    if (init->advancedSettings->startFrame) {
      eusart->CFG1 |= EUSART_CFG1_SFUBRX;
    }
    if (init->advancedSettings->prsRxEnable) {
      eusart->CFG1 |= EUSART_CFG1_RXPRSEN;
      //Configure PRS channel as input data line for EUSART.
      PRS->CONSUMER_EUART0_RX_SET = (init->advancedSettings->prsRxChannel & _PRS_CONSUMER_EUART0_RX_MASK);
    }
  }

  if (irdaInit) {
    if (irdaInit->irDALowFrequencyEnable) {
      eusart->IRLFCFG_SET = (uint32_t)(EUSART_IRLFCFG_IRLFEN);
    } else {
      // Configure IrDA HF configuration register.
      eusart->IRHFCFG_SET = (eusart->IRHFCFG & ~(_EUSART_IRHFCFG_IRHFEN_MASK
                                                 | _EUSART_IRHFCFG_IRHFEN_MASK
                                                 | _EUSART_IRHFCFG_IRHFFILT_MASK))
                            | (uint32_t)(EUSART_IRHFCFG_IRHFEN)
                            | (uint32_t)(irdaInit->irDAPulseWidth)
                            | (uint32_t)(irdaInit->irDARxFilterEnable);
    }
  }

  // Enable EUSART IP.
  eusart->EN_SET = EUSART_EN_EN;

  // Configure the baudrate if auto baud detection is not used.
  if (init->baudrate) {
    EUSART_BaudrateSet(eusart, init->refFreq, init->baudrate);
  }

  // Finally enable the Rx and/or Tx channel (as specified).
  EUSART_Enable(eusart, init->enable);
  while (~EUSART_StatusGet(eusart) & (_EUSART_STATUS_RXIDLE_MASK | _EUSART_STATUS_TXIDLE_MASK)) {
  }
}
/** @endcond */

/** @} (end addtogroup EUSART) */
/** @} (end addtogroup emlib) */
#endif /* defined(EUART_COUNT) && (EUART_COUNT > 0) */
