/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      UART driver definition.
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

#include "pal_uart.h"
#include "nrfx_uarte.h"

#if defined(BOARD_NRF6832)
#define RX_PIN_NUMBER  26
#define TX_PIN_NUMBER  27
#define CTS_PIN_NUMBER 0xFF
#define RTS_PIN_NUMBER 0xFF
#else
#include "boards.h"
#endif

/**************************************************************************************************
  Type definitions
**************************************************************************************************/

/*! \brief      Control block. */
typedef struct
{
  PalUartState_t  state;    /*!< UART state. */
  PalUartConfig_t config;   /*!< UART configuration. */
  nrfx_uarte_t    inst;     /*!< nRF UART driver instance. */
} palUartCtrlBlk_t;

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

#define PAL_UART_INVALID_INSTANCE_NUM       0xFF

#ifdef DEBUG

/*! \brief      Parameter check. */
#define PAL_UART_PARAM_CHECK(p, expr)       { if (!(expr)) { p->state = PAL_UART_STATE_ERROR; return; } }

#else

/*! \brief      Parameter check (disabled). */
#define PAL_UART_PARAM_CHECK(p, expr)

#endif

/*! \brief      Control block. */
static palUartCtrlBlk_t palUartCb[2];

/**************************************************************************************************
  Local Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Get UART instance number from UART ID.
 *
 *  \param      id      UART ID.
 *
 *  \return     UART instance number.
 */
/*************************************************************************************************/
static palUartCtrlBlk_t *palUartGetContext(PalUartId_t id)
{
  switch (id)
  {
    case PAL_UART_ID_CHCI:
    case PAL_UART_ID_TERMINAL:
      return &palUartCb[0];

    case PAL_UART_ID_USER:
      return (NRFX_UARTE_ENABLED_COUNT > 1) ? &palUartCb[1] : &palUartCb[0];

    default:
      break;
  }

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  UART NRF event handler.
 *
 *  \param  pEvent      UART event.
 *  \param  pContext    Instance context.
 */
/*************************************************************************************************/
static void palUartEventHandler(nrfx_uarte_event_t const *pEvent, void *pInstCtx)
{
  palUartCtrlBlk_t *pCtx = (palUartCtrlBlk_t *)pInstCtx;

  switch (pEvent->type)
  {
    case NRFX_UARTE_EVT_RX_DONE:
      if (pCtx->config.rdCback != NULL)
      {
        pCtx->config.rdCback();
      }
      break;
    case NRFX_UARTE_EVT_TX_DONE:
      pCtx->state = PAL_UART_STATE_READY;
      if (pCtx->config.wrCback != NULL)
      {
        pCtx->config.wrCback();
      }
      break;
    case NRFX_UARTE_EVT_ERROR:
      pCtx->state = PAL_UART_STATE_ERROR;
      break;
    default:
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize UART.
 *
 *  \param      id      UART ID.
 *  \param      pCfg    Peripheral configuration.
 */
/*************************************************************************************************/
void PalUartInit(PalUartId_t id, const PalUartConfig_t *pCfg)
{
  palUartCtrlBlk_t *pCtx = palUartGetContext(id);
  PAL_UART_PARAM_CHECK(pCtx, pCtx != NULL);
  PAL_UART_PARAM_CHECK(pCtx, pCfg != NULL);

  pCtx->config = *pCfg;

  /* Resolve instance. */
  switch (pCtx - palUartCb)
  {
    default:
    case 0:
      pCtx->inst.p_reg = NRF_UARTE0;
      pCtx->inst.drv_inst_idx = 0;
      break;
#if (NRFX_UARTE1_ENABLED)
    case 1:
      pCtx->inst.p_reg = NRF_UARTE1;
      pCtx->inst.drv_inst_idx = 1;
      break;
#endif
  }

  nrfx_uarte_config_t nrfCfg = NRFX_UARTE_DEFAULT_CONFIG;
  nrfCfg.pselrxd = RX_PIN_NUMBER;
  nrfCfg.pseltxd = TX_PIN_NUMBER;
  nrfCfg.pselcts = CTS_PIN_NUMBER;
  nrfCfg.pselrts = RTS_PIN_NUMBER;
  nrfCfg.p_context = pCtx;
  nrfCfg.parity = NRF_UARTE_PARITY_EXCLUDED;
  nrfCfg.interrupt_priority = 0xFF; /* Lowest priority. */
  nrfCfg.hwfc = pCfg->hwFlow ? NRF_UARTE_HWFC_ENABLED : NRF_UARTE_HWFC_DISABLED;

  switch (pCfg->baud)
  {
    default:
    case  115200: nrfCfg.baudrate = NRF_UARTE_BAUDRATE_115200;  break;
    case  230400: nrfCfg.baudrate = NRF_UARTE_BAUDRATE_230400;  break;
    case  460800: nrfCfg.baudrate = NRF_UARTE_BAUDRATE_460800;  break;
    case  921600: nrfCfg.baudrate = NRF_UARTE_BAUDRATE_921600;  break;
    case 1000000: nrfCfg.baudrate = NRF_UARTE_BAUDRATE_1000000; break;
  }

  nrfx_err_t err_code = nrfx_uarte_init(&(pCtx->inst), &nrfCfg, palUartEventHandler);

  if (err_code != NRFX_SUCCESS)
  {
    pCtx->state = PAL_UART_STATE_ERROR;
    return;
  }

  pCtx->state = PAL_UART_STATE_READY;
}

/*************************************************************************************************/
/*!
 *  \brief      De-Initialize UART.
 *
 *  \param      id      UART ID.
 */
/*************************************************************************************************/
void PalUartDeInit(PalUartId_t id)
{
  palUartCtrlBlk_t *pCtx = palUartGetContext(id);
  PAL_UART_PARAM_CHECK(pCtx, pCtx != NULL);
  PAL_UART_PARAM_CHECK(pCtx, pCtx->state == PAL_UART_STATE_READY);

  nrfx_uarte_uninit(&pCtx->inst);

  pCtx->state = PAL_UART_STATE_UNINIT;
}

/*************************************************************************************************/
/*!
 *  \brief      Get the current state.
 *
 *  \param      id      UART id.
 *
 *  \return      Current state.
 */
/*************************************************************************************************/
PalUartState_t PalUartGetState(PalUartId_t id)
{
  palUartCtrlBlk_t *pCtx = palUartGetContext(id);

  if (pCtx == NULL)
  {
    return PAL_UART_STATE_ERROR;
  }

  return pCtx->state;
}

/*************************************************************************************************/
/*!
 *  \brief      Read data from Rx FIFO.
 *
 *  \param      id      UART ID.
 *  \param      pData   Read buffer.
 *  \param      len     Number of bytes to read.
 */
/*************************************************************************************************/
void PalUartReadData(PalUartId_t id, uint8_t *pData, uint16_t len)
{
  palUartCtrlBlk_t *pCtx = palUartGetContext(id);
  PAL_UART_PARAM_CHECK(pCtx, pCtx != NULL);
  PAL_UART_PARAM_CHECK(pCtx, pCtx->state != PAL_UART_STATE_UNINIT);
  PAL_UART_PARAM_CHECK(pCtx, pData != NULL);
  PAL_UART_PARAM_CHECK(pCtx, len > 0);

  nrfx_err_t err = nrfx_uarte_rx(&pCtx->inst, pData, len);
  PAL_UART_PARAM_CHECK(pCtx, err == NRFX_SUCCESS);
  (void)err;
}

/*************************************************************************************************/
/*!
 *  \brief      Write data to Tx FIFO.
 *
 *  \param      id      UART ID.
 *  \param      pData   Write buffer.
 *  \param      len     Number of bytes to write.
 */
/*************************************************************************************************/
void PalUartWriteData(PalUartId_t id, const uint8_t *pData, uint16_t len)
{
  palUartCtrlBlk_t *pCtx = palUartGetContext(id);
  PAL_UART_PARAM_CHECK(pCtx, pCtx != NULL);
  PAL_UART_PARAM_CHECK(pCtx, pCtx->state != PAL_UART_STATE_UNINIT);
  PAL_UART_PARAM_CHECK(pCtx, pData != NULL);
  PAL_UART_PARAM_CHECK(pCtx, len > 0);

  pCtx->state = PAL_UART_STATE_BUSY;
  nrfx_err_t err = nrfx_uarte_tx(&pCtx->inst, pData, len);
  PAL_UART_PARAM_CHECK(pCtx, err == NRFX_SUCCESS);
  (void)err;
}
