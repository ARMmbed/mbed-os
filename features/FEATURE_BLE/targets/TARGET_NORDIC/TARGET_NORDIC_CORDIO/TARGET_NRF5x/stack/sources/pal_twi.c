/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      TWI driver implementation.
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

#include "pal_twi.h"
#include "nrfx_twim.h"
#include "nrf_twim.h"
#include "app_util_platform.h"
#include <string.h>

#if defined(BOARD_PCA10056) | defined(BOARD_PCA10040)
#include "boards.h"
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Allow compiler directive override. */
#ifndef PAL_TWI_MAX_DEVICE

/*! \brief      Maximum device count (must be an even multiple of 2^N). */
#define PAL_TWI_MAX_DEVICE      4
#endif

/*! \brief      Get next handle value, includes wrap around. */
#define PAL_TWI_GET_NEXT_HANDLE(h)  (((h) + 1) & (PAL_TWI_MAX_DEVICE - 1))

#ifdef DEBUG

/*! \brief      Parameter check. */
#define PAL_TWI_PARAM_CHECK(expr)           { if (!(expr)) { palTwiCb.drvState = PAL_TWI_STATE_ERROR; return; } }

/*! \brief      Parameter check, with return value. */
#define PAL_TWI_PARAM_CHECK_RET(expr, rv)   { if (!(expr)) { palTwiCb.drvState = PAL_TWI_STATE_ERROR; return (rv); } }

#else

/*! \brief      Parameter check (disabled). */
#define PAL_TWI_PARAM_CHECK(expr)

/*! \brief      Parameter check, with return value (disabled). */
#define PAL_TWI_PARAM_CHECK_RET(expr, rv)

#endif

/*! \brief TWI instance ID. */
#define PAL_TWI_INSTANCE_ID     0

#if defined(BOARD_PCA10056) | defined(BOARD_PCA10040)
#define PAL_TWI_SCL_PIN ARDUINO_SCL_PIN   /*!< Pin number for SCL. */
#define PAL_TWI_SDA_PIN ARDUINO_SDA_PIN   /*!< Pin number for SDA. */
#endif

#if defined(BOARD_NRF6832)
#define PAL_TWI_SCL_PIN (6U)    /*!< Pin number for SCL. */
#define PAL_TWI_SDA_PIN (7U)    /*!< Pin number for SDA. */
#endif

/**************************************************************************************************
  Type Definitions
**************************************************************************************************/

/*! \brief      Commands state. */
typedef enum
{
  PAL_TWI_CMD_IDLE,             /*!< Idle state. */
  PAL_TWI_CMD_TX_DATA,          /*!< Write data state. */
  PAL_TWI_CMD_RX_DATA           /*!< Read data state. */
} PalTwiCmdState_t;

/*! \brief      Device configuration. */
typedef struct
{
  bool_t opPending;             /*!< Operation pending flag. */
  PalTwiDevConfig_t devCfg;     /*!< Device configuration. */
} PalTwiDevCtx_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Driver control block. */
static struct
{
  uint8_t curHandle;            /*!< Current device handle. */
  PalTwiCmdState_t cmdState;    /*!< Command transaction state, Tx or Rx. */
  PalTwiState_t drvState;       /*!< Current state. */
} palTwiCb = { 0 };

/*! \brief      Device context table. */
static PalTwiDevCtx_t twiDevCtx[PAL_TWI_MAX_DEVICE];

/*! \brief      TWI instance. */
static const nrfx_twim_t twiId = NRFX_TWIM_INSTANCE(PAL_TWI_INSTANCE_ID);

/*! \brief      TWI config. */
const nrfx_twim_config_t twiConfig =
{
   .frequency          = NRF_TWIM_FREQ_400K,
   .scl                = PAL_TWI_SCL_PIN,
   .sda                = PAL_TWI_SDA_PIN,
   .interrupt_priority = APP_IRQ_PRIORITY_LOWEST,   // TODO define in common platform BSP
   .hold_bus_uninit    = FALSE
};

/**************************************************************************************************
  Functions: Initialization
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Operation complete callback.
 *
 *  \param      pEvt        Event parameters.
 *  \param      pContext    Unused  context.
 */
/*************************************************************************************************/
static void palTwiCallback(nrfx_twim_evt_t *pEvt, void *pCtx)
{
  (void)pCtx;

  /* Pre-resolve device configuration. */
  PalTwiDevConfig_t *pCfg = &twiDevCtx[palTwiCb.curHandle].devCfg;
  bool_t success = FALSE;

  if ((pEvt->type) == NRFX_TWIM_EVT_DONE)
  {
    success = TRUE;
  }

  if (palTwiCb.cmdState == PAL_TWI_CMD_TX_DATA)
  {
    palTwiCb.cmdState = PAL_TWI_CMD_IDLE;
    if (pCfg->wrCback)
    {
      pCfg->wrCback(palTwiCb.curHandle, success);
    }
  }
  else if (palTwiCb.cmdState == PAL_TWI_CMD_RX_DATA)
  {
    palTwiCb.cmdState = PAL_TWI_CMD_IDLE;
    if (pCfg->rdCback)
    {
      pCfg->rdCback(palTwiCb.curHandle, success);
    }
  }

  palTwiCb.drvState = PAL_TWI_STATE_BUSY_DATA_PEND;
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize TWI resources.
 */
/*************************************************************************************************/
void PalTwiInit(void)
{
  if (palTwiCb.drvState != PAL_TWI_STATE_UNINIT)
  {
    /* Only initialize once. */
    return;
  }

  memset(&palTwiCb, 0, sizeof(palTwiCb));

  for (unsigned int handle = 0; handle < PAL_TWI_MAX_DEVICE; handle++)
  {
    twiDevCtx[handle].devCfg.devAddr      = PAL_TWI_INVALID_ID;
    twiDevCtx[handle].devCfg.opReadyCback = NULL;
    twiDevCtx[handle].devCfg.rdCback      = NULL;
    twiDevCtx[handle].devCfg.wrCback      = NULL;
  }

  palTwiCb.drvState = PAL_TWI_STATE_READY;

  nrfx_twim_init(&twiId, &twiConfig, (nrfx_twim_evt_handler_t)palTwiCallback, NULL);
  nrfx_twim_enable(&twiId);
}

/*************************************************************************************************/
/*!
 *  \brief      De-Initialize the TWI resources.
 */
/*************************************************************************************************/
void PalTwiDeInit(void)
{
  PAL_TWI_PARAM_CHECK(palTwiCb.drvState == PAL_TWI_STATE_READY);

  for (unsigned int handle = 0; handle < PAL_TWI_MAX_DEVICE; handle++)
  {
    twiDevCtx[handle].devCfg.devAddr      = PAL_TWI_INVALID_ID;
    twiDevCtx[handle].devCfg.opReadyCback = NULL;
    twiDevCtx[handle].devCfg.rdCback      = NULL;
    twiDevCtx[handle].devCfg.wrCback      = NULL;
  }

  nrfx_twim_disable(&twiId);
  nrfx_twim_uninit(&twiId);

  palTwiCb.drvState = PAL_TWI_STATE_UNINIT;
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
  return palTwiCb.drvState;
}

/**************************************************************************************************
  Functions: Data Transfer
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Always start an operation before reading or writing on TWI interface.
 *
 *  \param      handle      Device handle.
 */
/*************************************************************************************************/
void PalTwiStartOperation(uint8_t handle)
{
  PAL_TWI_PARAM_CHECK(handle < PAL_TWI_MAX_DEVICE);

  if (palTwiCb.drvState == PAL_TWI_STATE_READY)
  {
    palTwiCb.drvState = PAL_TWI_STATE_BUSY_DATA_PEND;
    palTwiCb.curHandle = handle;
    if (twiDevCtx[handle].devCfg.opReadyCback)
    {
      twiDevCtx[handle].devCfg.opReadyCback(handle);
    }
  }
  else
  {
    PAL_TWI_PARAM_CHECK(handle != palTwiCb.curHandle);          /* Client operation already in progress. */
    PAL_TWI_PARAM_CHECK(twiDevCtx[handle].opPending == FALSE);  /* Client already pended an operation. */

    /* Pend the operation until current operation completes. */
    twiDevCtx[handle].opPending = TRUE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Always stop an operation after reading or writing on TWI interface.
 *
 *  \param      handle      Device handle.
 */
/*************************************************************************************************/
void PalTwiStopOperation(uint8_t handle)
{
  PAL_TWI_PARAM_CHECK(handle < PAL_TWI_MAX_DEVICE);
  PAL_TWI_PARAM_CHECK(handle == palTwiCb.curHandle);
  PAL_TWI_PARAM_CHECK(palTwiCb.cmdState == PAL_TWI_CMD_IDLE);
  PAL_TWI_PARAM_CHECK(palTwiCb.drvState == PAL_TWI_STATE_BUSY_DATA_PEND);

  unsigned int nextHandle = PAL_TWI_GET_NEXT_HANDLE(handle);

  palTwiCb.curHandle = PAL_TWI_INVALID_ID;

  while (nextHandle != handle)
  {
    if (twiDevCtx[nextHandle].opPending)
    {
      /* Set the operation pending to FALSE first in case of race condition. */
      twiDevCtx[nextHandle].opPending = FALSE;

      palTwiCb.curHandle = nextHandle;
      if (twiDevCtx[nextHandle].devCfg.opReadyCback)
      {
        twiDevCtx[nextHandle].devCfg.opReadyCback(nextHandle);
      }

      /* Remain in BUSY state. */
      return;
    }

    nextHandle = PAL_TWI_GET_NEXT_HANDLE(nextHandle);
  }

  /* No pending operations. */
  palTwiCb.drvState = PAL_TWI_STATE_READY;
}

/*************************************************************************************************/
/*!
 *  \brief      Read data from TWI interface.
 *
 *  \param      handle      Device handle.
 *  \param      pData       Read buffer.
 *  \param      len         Number of bytes to write.
 *
 *  Read \a len bytes from \a pData to the TWI device.
 */
/*************************************************************************************************/
void PalTwiReadData(uint8_t handle, uint8_t *pData, uint8_t len)
{
  PAL_TWI_PARAM_CHECK(len != 0);
  PAL_TWI_PARAM_CHECK(pData != NULL);
  PAL_TWI_PARAM_CHECK(handle < PAL_TWI_MAX_DEVICE);
  PAL_TWI_PARAM_CHECK(handle == palTwiCb.curHandle);

  palTwiCb.curHandle =handle;
  palTwiCb.cmdState = PAL_TWI_CMD_RX_DATA;
  palTwiCb.drvState = PAL_TWI_STATE_BUSY_DATA_XFER;

  nrfx_twim_rx(&twiId, twiDevCtx[handle].devCfg.devAddr, pData, len);
}

/*************************************************************************************************/
/*!
 *  \brief      Write data to TWI interface.
 *
 *  \param      handle      Device handle.
 *  \param      pData       Write buffer.
 *  \param      len         Number of bytes to write.
 *
 *  Transfer \a len bytes from \a pData to the TWI device.
 */
/*************************************************************************************************/
void PalTwiWriteData(uint8_t handle, const uint8_t *pData, uint8_t len)
{
  PAL_TWI_PARAM_CHECK(len != 0);
  PAL_TWI_PARAM_CHECK(pData != NULL);
  PAL_TWI_PARAM_CHECK(handle < PAL_TWI_MAX_DEVICE);
  PAL_TWI_PARAM_CHECK(handle == palTwiCb.curHandle);

  palTwiCb.curHandle = handle;
  palTwiCb.cmdState = PAL_TWI_CMD_TX_DATA;
  palTwiCb.drvState = PAL_TWI_STATE_BUSY_DATA_XFER;

  nrfx_err_t err = nrfx_twim_tx(&twiId, twiDevCtx[handle].devCfg.devAddr, pData, len, NRFX_TWIM_FLAG_TX_NO_STOP);
  PAL_TWI_PARAM_CHECK(err == NRFX_SUCCESS);
  #ifndef DEBUG
    (void)err;
  #endif
}
