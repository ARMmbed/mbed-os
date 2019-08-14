/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      LED driver implementation.
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

#include "stack/platform/include/pal_led.h"
#include "boards.h"
#include "stack/platform/include/pal_types.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define PAL_LED0_MASK                      0x00002000
#define PAL_LED1_MASK                      0x00004000
#define PAL_LED2_MASK                      0x00008000
#define PAL_LED3_MASK                      0x00010000

/*! \brief      LED masks. */
#if (AUDIO_CAPE == 1)

#define PAL_LED_AUDIO_LED1_MASK                0x00000002
#define PAL_LED_AUDIO_LED2_MASK                0x00000004
#define PAL_LED_AUDIO_LED3_MASK                0x00000008
#define PAL_LED_AUDIO_LED4_MASK                0x00000010
#define PAL_LED_AUDIO_LED5_MASK                0x00000020
#define PAL_LED_AUDIO_LED6_MASK                0x00000040

#define PAL_LED_P0_MASK                    0x01E000
#define PAL_LED_P1_MASK                    0x00007E

#else
#define PAL_LED_P0_MASK                    0x01E000
#define PAL_LED_P1_MASK                    0x000000

#endif

/*! \brief      Invalid LED mask. */
#define PAL_LED_INVALID_MASK            0xFF

/*! \brief      LED count using GPIO P0. */
#define PAL_LED_COUNT_P0                0x04

/*! \brief I/O Expander definitions */
enum
{
  PAL_LED_IO_EXP_SUB_ADDR             = 0x06       /*!< Lower 3 bit of I/O expander address connected with LEDs. */
};

/*! \brief I/O Expander accessories */
enum
{
  PAL_LED_IO_EXP_CONFIG               = 0x00       /*!< LED3 to LED10 are defined as outputs.*/
};

#ifdef DEBUG

/*! \brief      Parameter check. */
#define PAL_LED_PARAM_CHECK(expr)           { if (!(expr)) { return; } }

/*! \brief      Parameter check, with return value. */
#define PAL_LED_PARAM_CHECK_RET(expr, rv)   { if (!(expr)) { return (rv); } }

#else

/*! \brief      Parameter check (disabled). */
#define PAL_LED_PARAM_CHECK(expr)

/*! \brief      Parameter check, with return value (disabled). */
#define PAL_LED_PARAM_CHECK_RET(expr, rv)

#endif

/**************************************************************************************************
  Functions: Initialization
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Get LED pin number from LED ID.
 *
 *  \param      ledId           LED ID.
 *
 *  \return     LED pin mask.
 */
/*************************************************************************************************/
static uint32_t palLedGetPinMask(uint8_t ledId)
{
  uint32_t ledMask = PAL_LED_INVALID_MASK;

  switch (ledId)
  {
    case PAL_LED_ID_CPU_ACTIVE:
      ledMask = PAL_LED1_MASK;
      break;

    case PAL_LED_ID_ERROR:
      ledMask = PAL_LED3_MASK;
      break;

    case 0:
      ledMask = PAL_LED0_MASK;
      break;
    case 1:
      ledMask = PAL_LED1_MASK;
      break;
    case 2:
      ledMask = PAL_LED2_MASK;
      break;
    case 3:
      ledMask = PAL_LED3_MASK;
      break;

#if (AUDIO_CAPE == 1)
    case 4:
      ledMask = PAL_LED_AUDIO_LED1_MASK;
      break;
    case 5:
      ledMask = PAL_LED_AUDIO_LED2_MASK;
      break;
    case 6:
      ledMask = PAL_LED_AUDIO_LED3_MASK;
      break;
    case 7:
      ledMask = PAL_LED_AUDIO_LED4_MASK;
      break;
    case 8:
      ledMask = PAL_LED_AUDIO_LED5_MASK;
      break;
    case 9:
      ledMask = PAL_LED_AUDIO_LED6_MASK;
      break;
#endif

    default:
      ledMask = PAL_LED_INVALID_MASK;
      break;
  }

  return ledMask;
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize LEDs.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalLedInit(void)
{
  nrf_gpio_port_dir_output_set(NRF_P0, PAL_LED_P0_MASK);
  nrf_gpio_port_out_set(NRF_P0, PAL_LED_P0_MASK);
#if (GPIO_COUNT > 1)
  nrf_gpio_port_dir_output_set(NRF_P1, PAL_LED_P1_MASK);
  nrf_gpio_port_out_set(NRF_P1, PAL_LED_P1_MASK);
#endif
}

/*************************************************************************************************/
/*!
 *  \brief      De-initialize LEDs.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalLedDeInit(void)
{
  nrf_gpio_port_dir_input_set(NRF_P0, PAL_LED_P0_MASK);
#if (GPIO_COUNT > 1)
  nrf_gpio_port_dir_input_set(NRF_P1, PAL_LED_P1_MASK);
#endif
}

/*************************************************************************************************/
/*!
 *  \brief      Set multiple LEDs on.
 *
 *  \param      mask           LED mask.
 *
 *  \return     None.
 *
 */
/*************************************************************************************************/
void PalLedOnGroup(uint32_t mask)
{
  PAL_LED_PARAM_CHECK(mask);

  nrf_gpio_port_out_clear(NRF_P0, mask);
}

/*************************************************************************************************/
/*!
 *  \brief      Set multiple LEDs off.
 *
 *  \param      mask           LED mask.
 *
 *  \return     None.
 *
 */
/*************************************************************************************************/
void PalLedOffGroup(uint32_t mask)
{
  PAL_LED_PARAM_CHECK(mask);

  nrf_gpio_port_out_set(NRF_P0, mask);
}

/*************************************************************************************************/
/*!
 *  \brief      Set LED on.
 *
 *  \param      ledId           LED ID.
 *
 *  \return     None.
 *
 */
/*************************************************************************************************/
void PalLedOn(uint8_t ledId)
{
  uint32_t ledMask = palLedGetPinMask(ledId);
  PAL_LED_PARAM_CHECK(ledMask != PAL_LED_INVALID_MASK);

  if ((ledId < PAL_LED_COUNT_P0) || (ledId >= PAL_LED_ID_CPU_ACTIVE))
  {
    nrf_gpio_port_out_clear(NRF_P0, ledMask);
  }
#if (GPIO_COUNT > 1)
  else
  {
    nrf_gpio_port_out_clear(NRF_P1, ledMask);
  }
#endif
}

/*************************************************************************************************/
/*!
 *  \brief      Set LED off.
 *
 *  \param      ledId           LED ID.
 *
 *  \return     None.
 *
 */
/*************************************************************************************************/
void PalLedOff(uint8_t ledId)
{
  uint32_t ledMask = palLedGetPinMask(ledId);
  PAL_LED_PARAM_CHECK(ledMask != PAL_LED_INVALID_MASK);

  if ((ledId < PAL_LED_COUNT_P0) || (ledId >= PAL_LED_ID_CPU_ACTIVE))
  {
    nrf_gpio_port_out_set(NRF_P0, ledMask);
  }
#if (GPIO_COUNT > 1)
  else
  {
    nrf_gpio_port_out_set(NRF_P1, ledMask);
  }
#endif
}
