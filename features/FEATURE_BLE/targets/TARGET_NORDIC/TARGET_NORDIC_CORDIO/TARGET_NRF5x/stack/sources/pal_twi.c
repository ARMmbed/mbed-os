/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      TWI driver implementation.
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
#include "stack/platform/include/pal_twi.h"
#include "nrfx_twim.h"
#include "nrf_twim.h"
#include "app_util_platform.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Allow compiler directive override. */
#ifndef PAL_TWI_MAX_DEVICE

/*! \brief      Maximum device count (must be an even multiple of 2^N). */
#define PAL_TWI_MAX_DEVICE           4
#endif

/*! \brief      Get next handle value, includes wrap around. */
#define PAL_TWI_GET_NEXT_HANDLE(h)  (((h) + 1) & (PAL_TWI_MAX_DEVICE - 1))

#ifdef DEBUG

/*! \brief      Parameter check. */
#define PAL_TWI_PARAM_CHECK(expr)           { if (!(expr)) { twiDevCb.drvState = PAL_TWI_STATE_ERROR; return; } }

/*! \brief      Parameter check, with return value. */
#define PAL_TWI_PARAM_CHECK_RET(expr, rv)   { if (!(expr)) { twiDevCb.drvState = PAL_TWI_STATE_ERROR; return (rv); } }

#else

/*! \brief      Parameter check (disabled). */
#define PAL_TWI_PARAM_CHECK(expr)

/*! \brief      Parameter check, with return value (disabled). */
#define PAL_TWI_PARAM_CHECK_RET(expr, rv)

#endif

/*! \brief TWI instance ID. */
#define PAL_TWI_INSTANCE_ID     0

/*! \brief      Pin number for SCL. */
#define PAL_TWI_CLOCK_PIN_NUMBER (27U)

/*! \brief      Pin number for SDA. */
#define PAL_TWI_DATA_PIN_NUMBER (26U)


/**************************************************************************************************
  Type Definitions
**************************************************************************************************/

/*! \brief      Commands state. */
typedef enum
{
  PAL_TWI_CMD_IDLE,         /*!< Idle state. */
  PAL_TWI_CMD_TX_DATA,      /*!< Write data state. */
  PAL_TWI_CMD_RX_DATA       /*!< Read data state. */
} PalTwiCmdState_t;

/*! \brief      Device configuration. */
typedef struct
{
  bool_t opPending;             /*!< Operation pending flag. */
  PalTwiDevConfig_t devCfg;     /*!< Device configuration. */
} PalTwiDevCtx_t;

#if NRFX_CHECK(NRFX_TWIM0_ENABLED)

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Driver control block. */
static struct
{
  uint8_t curHandle;            /*!< Current device handle. */
  PalTwiCmdState_t cmdState;       /*!< Command transaction state, Tx or Rx. */
  PalTwiState_t drvState;          /*!< Current state. */
  bool_t firstCmd;              /*!< First command after operation start flag. */
} twiDevCb;

/*! \brief      Device context table. */
static PalTwiDevCtx_t twiDevCtx[PAL_TWI_MAX_DEVICE];

/*! \brief      TWI instance. */
static const nrfx_twim_t twiId = NRFX_TWIM_INSTANCE(PAL_TWI_INSTANCE_ID);

/*! \brief      TWI config. */
const nrfx_twim_config_t twiConfig =
{
   .frequency          = NRF_TWIM_FREQ_400K,
   .scl                = PAL_TWI_CLOCK_PIN_NUMBER,
   .sda                = PAL_TWI_DATA_PIN_NUMBER,
   .interrupt_priority = APP_IRQ_PRIORITY_LOWEST,
   .hold_bus_uninit     = false
};

/**************************************************************************************************
  Functions: Initialization
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Control callback.
 *
 *  \param      event     Event parameters.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void palTwiCallback(nrfx_twim_evt_t *event, void *pContext)
{
  /* Pre-resolve device configuration. */
  PalTwiDevConfig_t *pCfg = &twiDevCtx[twiDevCb.curHandle].devCfg;
  bool_t success = FALSE;

  twiDevCb.firstCmd = FALSE;

  if ((event->type) == NRFX_TWIM_EVT_DONE)
  {
    success = TRUE;
  }

  if (twiDevCb.cmdState == PAL_TWI_CMD_TX_DATA)
  {
    twiDevCb.cmdState = PAL_TWI_CMD_IDLE;
    if (pCfg->wrCback)
    {
      pCfg->wrCback(twiDevCb.curHandle, success);
    }
  }
  else if (twiDevCb.cmdState == PAL_TWI_CMD_RX_DATA)
  {
    twiDevCb.cmdState = PAL_TWI_CMD_IDLE;
    if (pCfg->rdCback)
    {
      pCfg->rdCback(twiDevCb.curHandle, success);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize TWI resources.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalTwiInit(void)
{
  if (twiDevCb.drvState != PAL_TWI_STATE_UNINIT)
  {
    return;
  }

  memset(&twiDevCb, 0, sizeof(twiDevCb));

  for (unsigned int handle = 0; handle < PAL_TWI_MAX_DEVICE; handle++)
  {
    twiDevCtx[handle].devCfg.devAddr      = PAL_TWI_INVALID_ID;
    twiDevCtx[handle].devCfg.opReadyCback = NULL;
    twiDevCtx[handle].devCfg.rdCback      = NULL;
    twiDevCtx[handle].devCfg.wrCback      = NULL;
  }

  twiDevCb.drvState = PAL_TWI_STATE_READY;

  nrfx_twim_init(&twiId, &twiConfig, (nrfx_twim_evt_handler_t)palTwiCallback, NULL);
  nrfx_twim_enable(&twiId);
  NVIC_ClearPendingIRQ(SPI0_TWI0_IRQn);
  NVIC_EnableIRQ(SPI0_TWI0_IRQn);
}

/*************************************************************************************************/
/*!
 *  \brief      De-Initialize the TWI resources.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalTwiDeInit(void)
{
  PAL_TWI_PARAM_CHECK(twiDevCb.drvState != PAL_TWI_STATE_BUSY);

  for (unsigned int handle = 0; handle < PAL_TWI_MAX_DEVICE; handle++)
  {
    twiDevCtx[handle].devCfg.devAddr      = PAL_TWI_INVALID_ID;
    twiDevCtx[handle].devCfg.opReadyCback = NULL;
    twiDevCtx[handle].devCfg.rdCback      = NULL;
    twiDevCtx[handle].devCfg.wrCback      = NULL;
  }

  nrfx_twim_uninit(&twiId);
  nrfx_twim_disable(&twiId);

  twiDevCb.drvState = PAL_TWI_STATE_UNINIT;
}

/*************************************************************************************************/
/*!
 *  \brief      Register a device on the TWI bus.
 *
 *  \param      pDevCfg     Device configuration.
 *
 *  \return     Device handle.
 */
/*************************************************************************************************/
uint8_t PalTwiRegisterDevice(PalTwiDevConfig_t *pDevCfg)
{
  PAL_TWI_PARAM_CHECK_RET(pDevCfg != NULL, PAL_TWI_INVALID_ID);
  PAL_TWI_PARAM_CHECK_RET(pDevCfg->opReadyCback, PAL_TWI_INVALID_ID);

  uint8_t retValue = PAL_TWI_INVALID_ID;

  for (unsigned int handle = 0; handle < PAL_TWI_MAX_DEVICE; handle++)
  {
    if ((twiDevCtx[handle].devCfg.devAddr == PAL_TWI_INVALID_ID) && (pDevCfg->devAddr != PAL_TWI_INVALID_ID))
    {
      twiDevCtx[handle].devCfg = *pDevCfg;
      return (uint8_t)handle;
    }
  }

  return retValue;
}

/**************************************************************************************************
  Functions: Control and Status
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Get the current state.
 *
 *  \return     Current state.
 *
 *  Return the current state of the TWI.
 */
/*************************************************************************************************/
PalTwiState_t PalTwiGetState(void)
{
  return twiDevCb.drvState;
}

/**************************************************************************************************
  Functions: Data Transfer
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Always start an operation before reading or writing on TWI interface.
 *
 *  \param      handle      Device handle.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalTwiStartOperation(uint8_t handle)
{
  PAL_TWI_PARAM_CHECK(handle < PAL_TWI_MAX_DEVICE);

  if (twiDevCb.drvState != PAL_TWI_STATE_READY)
  {
    PAL_TWI_PARAM_CHECK(handle != twiDevCb.curHandle);          /* Client operation already in progress. */
    PAL_TWI_PARAM_CHECK(twiDevCtx[handle].opPending == FALSE);  /* Client already pended an operation. */
  }

  __disable_irq();

  if (twiDevCb.drvState == PAL_TWI_STATE_READY)
  {
    __enable_irq();
    twiDevCb.drvState = PAL_TWI_STATE_BUSY;
    twiDevCb.firstCmd = TRUE;
    twiDevCb.curHandle = handle;
    if (twiDevCtx[handle].devCfg.opReadyCback)
    {
      twiDevCtx[handle].devCfg.opReadyCback(handle);
    }
  }
  else
  {
    /* Pend the operation until current operation completes. */
    twiDevCtx[handle].opPending = TRUE;
    __enable_irq();
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Always stop an operation after reading or writing on TWI interface.
 *
 *  \param      handle      Device handle.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalTwiStopOperation(uint8_t handle)
{
  PAL_TWI_PARAM_CHECK(handle < PAL_TWI_MAX_DEVICE);
  PAL_TWI_PARAM_CHECK(handle == twiDevCb.curHandle);
  PAL_TWI_PARAM_CHECK(twiDevCb.cmdState == PAL_TWI_CMD_IDLE);
  PAL_TWI_PARAM_CHECK(twiDevCb.drvState == PAL_TWI_STATE_BUSY);

  unsigned int nextHandle = PAL_TWI_GET_NEXT_HANDLE(handle);

  twiDevCb.curHandle = PAL_TWI_INVALID_ID;

  /* Only when address or command is sent, then issue stop. */
  if (!twiDevCb.firstCmd)
  {
    nrf_twim_event_clear(twiId.p_twim, NRF_TWIM_EVENT_STOPPED);
    nrf_twim_task_trigger(twiId.p_twim, NRF_TWIM_TASK_RESUME);
    nrf_twim_task_trigger(twiId.p_twim, NRF_TWIM_TASK_STOP);
    nrf_twim_int_disable(twiId.p_twim, 0xFFFFFFFF);
  }

  while (nextHandle != handle)
  {
    if (twiDevCtx[nextHandle].opPending)
    {
      /* Set the operation pending to FALSE first in case of race condition. */
      twiDevCtx[nextHandle].opPending = FALSE;

      twiDevCb.firstCmd = TRUE;
      twiDevCb.curHandle = nextHandle;
      if (twiDevCtx[nextHandle].devCfg.opReadyCback)
      {
        twiDevCtx[nextHandle].devCfg.opReadyCback(nextHandle);
      }

      return;
    }

    nextHandle = PAL_TWI_GET_NEXT_HANDLE(nextHandle);
  }

  /* No pending operations. */
  twiDevCb.drvState = PAL_TWI_STATE_READY;
}

/*************************************************************************************************/
/*!
 *  \brief      Write data to TWI interface.
 *
 *  \param      handle      Device handle.
 *  \param      pData       Write buffer.
 *  \param      len         Number of bytes to write.
 *
 *  \return     None.
 *
 *  Transfer \a len bytes from \a pData to the TWI device.
 */
/*************************************************************************************************/
void PalTwiWriteData(uint8_t handle, const uint8_t *pData, uint8_t len)
{
  PAL_TWI_PARAM_CHECK(len != 0);
  PAL_TWI_PARAM_CHECK(pData != NULL);
  PAL_TWI_PARAM_CHECK(handle < PAL_TWI_MAX_DEVICE);
  PAL_TWI_PARAM_CHECK(handle == twiDevCb.curHandle);
  PAL_TWI_PARAM_CHECK(twiDevCb.drvState == PAL_TWI_STATE_BUSY);
  PAL_TWI_PARAM_CHECK(twiDevCb.cmdState == PAL_TWI_CMD_IDLE);

  twiDevCb.curHandle = handle;
  twiDevCb.cmdState = PAL_TWI_CMD_TX_DATA;

  nrfx_twim_tx(&twiId, twiDevCtx[handle].devCfg.devAddr, pData, len, NRFX_TWIM_FLAG_TX_NO_STOP);
}

/*************************************************************************************************/
/*!
 *  \brief      Read data from TWI interface.
 *
 *  \param      handle      Device handle.
 *  \param      pData       Write buffer.
 *  \param      len         Number of bytes to write.
 *
 *  \return     None.
 *
 *  Read \a len bytes from \a pData to the TWI device.
 */
/*************************************************************************************************/
void PalTwiReadData(uint8_t handle, uint8_t *pData, uint8_t len)
{
  PAL_TWI_PARAM_CHECK(len != 0);
  PAL_TWI_PARAM_CHECK(pData != NULL);
  PAL_TWI_PARAM_CHECK(handle < PAL_TWI_MAX_DEVICE);
  PAL_TWI_PARAM_CHECK(handle == twiDevCb.curHandle);
  PAL_TWI_PARAM_CHECK(twiDevCb.drvState == PAL_TWI_STATE_BUSY);
  PAL_TWI_PARAM_CHECK(twiDevCb.cmdState == PAL_TWI_CMD_IDLE);

  twiDevCb.curHandle =handle;
  twiDevCb.cmdState = PAL_TWI_CMD_RX_DATA;

  nrfx_twim_rx(&twiId, twiDevCtx[handle].devCfg.devAddr, pData, len);
}

#endif // NRFX_CHECK(NRFX_TWIM0_ENABLED)

