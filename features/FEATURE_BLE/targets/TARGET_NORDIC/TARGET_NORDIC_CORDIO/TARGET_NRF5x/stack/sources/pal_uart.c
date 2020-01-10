/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      UART driver definition.
 *
 *  Copyright (c) 2018-2019 ARM Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
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

#include "stack/platform/include/pal_uart.h"
#include "sdk_config.h"
#include "nrfx_uarte.h"
#include "boards.h"

#if NRFX_CHECK(NRFX_UARTE0_ENABLED)

/**************************************************************************************************
  Local Variables
**************************************************************************************************/
#define PAL_UART_INVALID_INSTANCE_NUM          0xFF

#ifdef DEBUG

/*! \brief      Parameter check. */
#define PAL_UART_PARAM_CHECK(num, expr)           { if (!(expr)) { palUartCb[num].state = PAL_UART_STATE_ERROR; while(1){}; } }

#else

/*! \brief      Parameter check (disabled). */
#define PAL_UART_PARAM_CHECK(num, expr)

#endif

/*! \brief      Control block. */
static struct
{
  PalUartId_t           id;             /*!< PAL UART id. */
  PalUartConfig_t       config;         /*!< PAL UART configuration. */
  PalUartState_t        state;          /*!< PAL UART state. */
  nrfx_uarte_t          uart;           /*!< NRF UART driver instance */
} palUartCb[NRFX_UARTE0_ENABLED + NRFX_UARTE1_ENABLED] = {{0, {0}, 0, {NRF_UARTE0, NRFX_UARTE0_INST_IDX}}};

/**************************************************************************************************
  Local Functions
**************************************************************************************************/
/*************************************************************************************************/
/*!
 *  \brief      Get UART instance number from UART ID.
 *
 *  \param      uartId           UART ID.
 *
 *  \return     UART instance number.
 */
/*************************************************************************************************/
static uint8_t palUartGetNum(PalUartId_t uartId)
{
  uint8_t uartNum;
  /* Nordic platform has total two UART instances.
   * By default, only UART0 is enabled.
   */
  switch (uartId)
  {
    case PAL_UART_ID_CHCI:
      uartNum = 0;
      break;
    case PAL_UART_ID_TERMINAL:
      uartNum = 0;
      break;
    case PAL_UART_ID_USER:
      uartNum = 0;
      break;

    default:
      uartNum = PAL_UART_INVALID_INSTANCE_NUM;
      break;
  }

  return uartNum;
}

/*************************************************************************************************/
/*!
 *  \brief  Set UART baud rate.
 *
 *  \param  baud        Baud rate.
 *
 *  \return nrf baud rate.
 */
/*************************************************************************************************/
static nrf_uarte_baudrate_t palUartSetBaud(uint32_t baud)
{
  nrf_uarte_baudrate_t baudRate;

  switch (baud)
  {
    default:
    case 115200:
      baudRate = NRF_UARTE_BAUDRATE_115200;
      break;
    case 230400:
      baudRate = NRF_UARTE_BAUDRATE_230400;
      break;
    case 460800:
      baudRate = NRF_UARTE_BAUDRATE_460800;
      break;
    case 921600:
      baudRate = NRF_UARTE_BAUDRATE_921600;
      break;
    case 1000000:
      baudRate = NRF_UARTE_BAUDRATE_1000000;
      break;
  }

  return baudRate;
}

/*************************************************************************************************/
/*!
 *  \brief  UART NRF event handler.
 *
 *  \param  pEvent   Pointer to event.
 *  \param  pContext Pointer to event context.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void palUartEventHandler(nrfx_uarte_event_t *pEvent, void *pContext)
{
  uint8_t uartNum = palUartGetNum(*(PalUartId_t *)pContext);

  switch (pEvent->type)
  {
    case NRFX_UARTE_EVT_RX_DONE:
      if (palUartCb[uartNum].config.rdCback != NULL)
      {
        palUartCb[uartNum].config.rdCback();
      }
      break;
    case NRFX_UARTE_EVT_TX_DONE:
      palUartCb[uartNum].state = PAL_UART_STATE_READY;
      if (palUartCb[uartNum].config.wrCback != NULL)
      {
        palUartCb[uartNum].config.wrCback();
      }
      break;
    case NRFX_UARTE_EVT_ERROR:
      palUartCb[uartNum].state = PAL_UART_STATE_ERROR;
      break;
    default:
      break;
  }
}

/**************************************************************************************************
  Global Functions
**************************************************************************************************/
/*************************************************************************************************/
/*!
 *  \brief      Initialize UART.
 *
 *  \param      id          UART Id.
 *  \param      pCfg        Peripheral configuration.
 *
 *  \return     None.
 *
 *  Initialize UART peripheral with \a pCfg values.
 */
/*************************************************************************************************/

void PalUartInit(PalUartId_t id, const PalUartConfig_t *pCfg)
{
  uint8_t uartNum = palUartGetNum(id);

  if (uartNum == PAL_UART_INVALID_INSTANCE_NUM)
  {
    return;
  }

  palUartCb[uartNum].id = id;

  PAL_UART_PARAM_CHECK(uartNum, pCfg != NULL);

  palUartCb[uartNum].config = *pCfg;

  /* Configure UART. */
  nrfx_uarte_config_t nrfConfig = NRFX_UARTE_DEFAULT_CONFIG;
  nrfConfig.pselrxd = RX_PIN_NUMBER;
  nrfConfig.pseltxd = TX_PIN_NUMBER;
  nrfConfig.pselcts = CTS_PIN_NUMBER;
  nrfConfig.pselrts = RTS_PIN_NUMBER;
  nrfConfig.p_context = &(palUartCb[uartNum].id);
  nrfConfig.parity = NRF_UARTE_PARITY_EXCLUDED;
  nrfConfig.interrupt_priority = 0xFF; /* Lowest priority. */

  nrfConfig.baudrate = palUartSetBaud(pCfg->baud);
  nrfConfig.hwfc = pCfg->hwFlow ? NRF_UARTE_HWFC_ENABLED : NRF_UARTE_HWFC_DISABLED;

  /* Make sure UART is uninitialized. */
  nrfx_uarte_uninit(&(palUartCb[uartNum].uart));

  /* Initialize UART. */
  nrfx_err_t err_code = nrfx_uarte_init(&(palUartCb[uartNum].uart), &nrfConfig, (nrfx_uarte_event_handler_t)palUartEventHandler);

  //this is for uart only not for uarte
  //nrfx_uart_rx_enable(&palUartCb[uartNum].uart);

  if (err_code != NRFX_SUCCESS)
  {
    palUartCb[uartNum].state = PAL_UART_STATE_ERROR;
    return;
  }

  palUartCb[uartNum].state = PAL_UART_STATE_READY;

  NVIC_EnableIRQ(UART0_IRQn);
}

/*************************************************************************************************/
/*!
 *  \brief      De-Initialize UART.
 *
 *  \param      id      UART id.
 *
 *  \return     None.
 *
 *  De-Initialize UART.
 */
/*************************************************************************************************/
void PalUartDeInit(PalUartId_t id)
{
  uint8_t uartNum = palUartGetNum(id);

  if (uartNum == PAL_UART_INVALID_INSTANCE_NUM)
  {
    return;
  }

  nrfx_uarte_uninit(&(palUartCb[uartNum].uart));

  palUartCb[uartNum].state = PAL_UART_STATE_UNINIT;
}

/*************************************************************************************************/
/*!
 *  \brief      Get the current state.
 *
 *  \param      id      UART id.
 *
*  \return      Current state.
 *
 *  Return the current state.
 */
/*************************************************************************************************/
PalUartState_t PalUartGetState(PalUartId_t id)
{
  uint8_t uartNum = palUartGetNum(id);

  if (uartNum == PAL_UART_INVALID_INSTANCE_NUM)
  {
    return PAL_UART_STATE_ERROR;
  }

  return palUartCb[uartNum].state;
}

/*************************************************************************************************/
/*!
 *  \brief      Read data from Rx FIFO.
 *
 *  \param      id          UART id.
 *  \param      pData       Read buffer.
 *  \param      len         Number of bytes to read.
 *
 *  \return     None.
 *
 *  Store \a len received bytes in \a pData. After \a len is transferred, call
 *  \a UartInitInfo_t::rdCback to signal read completion. Alway call this function to setup buffer
 *  when boot up or after a reading is done
 */
/*************************************************************************************************/
void PalUartReadData(PalUartId_t id, uint8_t *pData, uint16_t len)
{
  uint8_t uartNum = palUartGetNum(id);

  PAL_UART_PARAM_CHECK(uartNum, pData != NULL);
  PAL_UART_PARAM_CHECK(uartNum, len > 0);
  PAL_UART_PARAM_CHECK(uartNum, palUartCb[uartNum].state != PAL_UART_STATE_UNINIT);

  if (uartNum == PAL_UART_INVALID_INSTANCE_NUM)
  {
    return;
  }
  nrfx_uarte_rx(&(palUartCb[uartNum].uart), pData, (uint16_t)len);
}

/*************************************************************************************************/
/*!
 *  \brief      Write data to Tx FIFO.
 *
 *  \param      id          UART id.
 *  \param      pData       Write buffer.
 *  \param      len         Number of bytes to write.
 *
 *  \return     None.
 *
 *  Assign buffer and length and transmit data.
 */
/*************************************************************************************************/
void PalUartWriteData(PalUartId_t id, const uint8_t *pData, uint16_t len)
{
  uint8_t uartNum = palUartGetNum(id);

  PAL_UART_PARAM_CHECK(uartNum, pData != NULL);
  PAL_UART_PARAM_CHECK(uartNum, len > 0);
  PAL_UART_PARAM_CHECK(uartNum, palUartCb[uartNum].state == PAL_UART_STATE_READY);

  if (uartNum == PAL_UART_INVALID_INSTANCE_NUM)
  {
    return;
  }

  nrfx_uarte_tx(&(palUartCb[uartNum].uart), pData, (uint16_t)len);
  palUartCb[uartNum].state = PAL_UART_STATE_BUSY;
}

#endif // NRFX_CHECK(NRFX_UARTE0_ENABLED)
