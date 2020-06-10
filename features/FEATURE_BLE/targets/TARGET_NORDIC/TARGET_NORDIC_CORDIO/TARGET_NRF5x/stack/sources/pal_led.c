/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      LED driver implementation.
 *
 *  Copyright (c) 2018-2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
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

#include "pal_led.h"

#if defined(BOARD_PCA10056)
#include "boards.h"
#include "nrfx_gpiote.h"
#endif

#if defined(BOARD_NRF6832)
#include "lp5562.h"
#endif

/**************************************************************************************************
  Functions: Initialization
**************************************************************************************************/

#if defined(BOARD_NRF6832)
/*************************************************************************************************/
/*!
 *  \brief      Map LED ID to physical LED.
 *
 *  \param      ledId           LED ID.
 *
 *  \return     Mapped physical LED.
 */
/*************************************************************************************************/
static int palLedMapId(uint8_t ledId)
{
  switch (ledId)
  {
    /* Predefined */
    case PAL_LED_ID_ERROR:
      return LP5562_LED_W;     /* bottom */
      break;

    /* Application defined */
    case 0:
      return LP5562_LED_R;     /* top/left */
    case 1:
      return LP5562_LED_G;     /* top/middle */
    case 2:
      return LP5562_LED_B;     /* top/right */

    /* Ignore */
    case PAL_LED_ID_CPU_ACTIVE:
    default:
      break;
  }

  return -1;
}
#endif

/*************************************************************************************************/
/*!
 *  \brief      Initialize LEDs.
 */
/*************************************************************************************************/
void PalLedInit(void)
{
#if defined(BOARD_PCA10056)
  nrfx_err_t err;

  if (!nrfx_gpiote_is_init())
  {
    err = nrfx_gpiote_init();

    if (err != NRFX_SUCCESS)
    {
      return;
    }
  }

  nrfx_gpiote_out_config_t cfg = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(true);

  nrfx_gpiote_out_init(LED_1, &cfg);
  nrfx_gpiote_out_init(LED_2, &cfg);
  nrfx_gpiote_out_init(LED_3, &cfg);
  nrfx_gpiote_out_init(LED_4, &cfg);
#endif

#if AUDIO_CAPE
  nrfx_gpiote_out_init(NRF_GPIO_PIN_MAP(1, 1), &cfg);
  nrfx_gpiote_out_init(NRF_GPIO_PIN_MAP(1, 2), &cfg);
  nrfx_gpiote_out_init(NRF_GPIO_PIN_MAP(1, 3), &cfg);
  nrfx_gpiote_out_init(NRF_GPIO_PIN_MAP(1, 4), &cfg);
  nrfx_gpiote_out_init(NRF_GPIO_PIN_MAP(1, 5), &cfg);
  nrfx_gpiote_out_init(NRF_GPIO_PIN_MAP(1, 6), &cfg);
#endif

#if defined(BOARD_NRF6832)
  lp5562_LedInit();
#endif
}

/*************************************************************************************************/
/*!
 *  \brief      De-initialize LEDs.
 */
/*************************************************************************************************/
void PalLedDeInit(void)
{
#if defined(BOARD_PCA10056)
  nrfx_gpiote_out_uninit(LED_1);
  nrfx_gpiote_out_uninit(LED_2);
  nrfx_gpiote_out_uninit(LED_3);
  nrfx_gpiote_out_uninit(LED_4);
#endif

#if defined(BOARD_NRF6832)
  lp5562_LedDeInit();
#endif

#if AUDIO_CAPE
  nrfx_gpiote_out_uninit(NRF_GPIO_PIN_MAP(1, 1));
  nrfx_gpiote_out_uninit(NRF_GPIO_PIN_MAP(1, 2));
  nrfx_gpiote_out_uninit(NRF_GPIO_PIN_MAP(1, 3));
  nrfx_gpiote_out_uninit(NRF_GPIO_PIN_MAP(1, 4));
  nrfx_gpiote_out_uninit(NRF_GPIO_PIN_MAP(1, 5));
  nrfx_gpiote_out_uninit(NRF_GPIO_PIN_MAP(1, 6));
#endif
}

/*************************************************************************************************/
/*!
 *  \brief      Set LED on.
 *
 *  \param      ledId           LED ID.
 */
/*************************************************************************************************/
void PalLedOn(uint8_t ledId)
{
#if defined(BOARD_PCA10056)
  switch (ledId)
  {
  case PAL_LED_ID_CPU_ACTIVE:
    nrfx_gpiote_out_clear(LED_2);
    break;
  case PAL_LED_ID_ERROR:
    nrfx_gpiote_out_clear(LED_4);
    break;
  case 0:
    nrfx_gpiote_out_clear(LED_1);
    break;
  case 1:
    nrfx_gpiote_out_clear(LED_3);
    break;
  default:
    break;
  }
#endif

#if defined(BOARD_NRF6832)
  int ledPin = palLedMapId(ledId);
  if (ledPin >= 0)
  {
    lp5562_LedOn(ledPin);
  }
#endif

#if AUDIO_CAPE
  switch (ledId)
  {
  case 2:
    nrfx_gpiote_out_clear(NRF_GPIO_PIN_MAP(1, 1));
    break;
  case 3:
    nrfx_gpiote_out_clear(NRF_GPIO_PIN_MAP(1, 2));
    break;
  case 4:
    nrfx_gpiote_out_clear(NRF_GPIO_PIN_MAP(1, 3));
    break;
  case 5:
    nrfx_gpiote_out_clear(NRF_GPIO_PIN_MAP(1, 4));
    break;
  case 6:
    nrfx_gpiote_out_clear(NRF_GPIO_PIN_MAP(1, 5));
    break;
  case 7:
    nrfx_gpiote_out_clear(NRF_GPIO_PIN_MAP(1, 6));
    break;
  default:
    break;
  }
#endif
}

/*************************************************************************************************/
/*!
 *  \brief      Set LED off.
 *
 *  \param      ledId           LED ID.
 */
/*************************************************************************************************/
void PalLedOff(uint8_t ledId)
{
#if defined(BOARD_PCA10056)
  switch (ledId)
  {
  case PAL_LED_ID_CPU_ACTIVE:
    nrfx_gpiote_out_set(LED_2);
    break;
  case PAL_LED_ID_ERROR:
    nrfx_gpiote_out_set(LED_4);
    break;
  case 0:
    nrfx_gpiote_out_set(LED_1);
    break;
  case 1:
    nrfx_gpiote_out_set(LED_3);
    break;
  default:
    break;
  }
#endif

#if defined(BOARD_NRF6832)
  int ledPin = palLedMapId(ledId);
  if (ledPin >= 0)
  {
    lp5562_LedOff(ledPin);
  }
#endif

#if AUDIO_CAPE
  switch (ledId)
  {
  case 2:
    nrfx_gpiote_out_set(NRF_GPIO_PIN_MAP(1, 1));
    break;
  case 3:
    nrfx_gpiote_out_set(NRF_GPIO_PIN_MAP(1, 2));
    break;
  case 4:
    nrfx_gpiote_out_set(NRF_GPIO_PIN_MAP(1, 3));
    break;
  case 5:
    nrfx_gpiote_out_set(NRF_GPIO_PIN_MAP(1, 4));
    break;
  case 6:
    nrfx_gpiote_out_set(NRF_GPIO_PIN_MAP(1, 5));
    break;
  case 7:
    nrfx_gpiote_out_set(NRF_GPIO_PIN_MAP(1, 6));
    break;
  default:
    break;
  }
#endif
}
