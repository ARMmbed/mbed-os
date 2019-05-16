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
 *  \brief Radio driver interface file.
 */
/*************************************************************************************************/

#ifndef RADIO_DRV_H
#define RADIO_DRV_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Operation types. */
enum
{
  RADIO_DRV_BLE_OP_TEST_TX,                     /*!< Continuous Tx test mode. */
  RADIO_DRV_BLE_OP_TEST_RX,                     /*!< Continuous Rx test mode. */
  RADIO_DRV_BLE_OP_MST_ADV_EVENT,               /*!< Master advertising event. */
  RADIO_DRV_BLE_OP_SLV_ADV_EVENT,               /*!< Slave advertising event. */
  RADIO_DRV_BLE_OP_MST_CONN_EVENT,              /*!< Master connection event. */
  RADIO_DRV_BLE_OP_SLV_CONN_EVENT,              /*!< Slave connection event. */
  RADIO_DRV_15P4_EVENT                          /*!< 15P4 event. */
};

/*! \brief      Radio timing. */
typedef struct
{
  uint32_t txOnLatency;                         /*!< Latency between radio on signal and transmit. */
  uint32_t rxOnLatency;                         /*!< Latency between radio on signal and receive. */
  uint32_t txDataPathLatency;                   /*!< Transmit data path latency. */
  uint32_t rxDataPathLatency;                   /*!< Receive data path latency. */
} RadioDrvTiming_t;

/*! \brief      Abort callback. */
typedef void (*RadioDrvAbortCback_t)(void);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Handle radio configuration.
 *
 *  \param      len       Length of configuration data, in octets.
 *  \param      pCfg      Configuration data.
 *
 *  \return     TRUE if radio configuration was handled.
 *
 *  The data block pCfg is only valid during the execution of this function, so configuration
 *  data must be stored or copied.
 */
/*************************************************************************************************/
bool_t RadioDrvCfgHandler(uint16_t len, const uint8_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief      Set radio configuration values.
 *
 *  \param      len       Length of configuration data, in octets.
 *  \param      pCfg      Configuration data.
 *
 *  \return     TRUE if radio configuration was handled.
 *
 *  The data block pCfg is only valid during the execution of this function, so configuration
 *  data must be stored or copied.
 */
/*************************************************************************************************/
bool_t RadioDrvSetConfiguration(uint16_t len, const uint8_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief      Initialize the BB radio.
 *
 *  \return     None.
 *
 *  Initialization occurs once upon startup of MAC-layer software to load trim, calibrate clocks,
 *  or perform any other one-time operations.
 */
/*************************************************************************************************/
void RadioDrvInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Get timing parameters for radio.
 *
 *  \param      pTiming         Storage for timing parameters.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void RadioDrvGetTiming(RadioDrvTiming_t *pTiming);

/*************************************************************************************************/
/*!
 *  \brief      Get supported transmit power levels.
 *
 *  \param      pMinTxPwr       Storage for minimum transmit power (expressed in 1dBm units).
 *  \param      pMaxTxPwr       Storage for maximum transmit power (expressed in 1dBm units).
 *
 *  \return     None.
 */
/*************************************************************************************************/
void RadioDrvGetSupTxPower(int8_t *pMinTxPwr, int8_t *pMaxTxPwr);

/*************************************************************************************************/
/*!
 *  \brief      Get the actual Tx power at the antenna (expressed in 1dBm units).
 *
 *  \param      txPwr     Tx power provided by the host (expressed in 1dBm units).
 *
 *  \return     Actual Tx power at the antenna (expressed in 1dBm units).
 */
/*************************************************************************************************/
int8_t RadioDrvGetActualTxPower(int8_t txPwr);

/*************************************************************************************************/
/*!
 *  \brief      Get the radio version.
 *
 *  \param      pVerCode        Version code return value.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void RadioDrvGetVersion(uint32_t *pVerCode);

/*************************************************************************************************/
/*!
 *  \brief      Set abort callback.
 *
 *  \param      cback           Abort callback.
 *
 *  \return     None.
 *
 *  If the abort callback is called after RadioDrvStartTx() or RadioDrvStartRx() but before
 *  RadioDrvStop(), the BB will abort the current operation.  Otherwise, the function is ignored.
 *
 *  The BB will set the callback to NULL to clear the callback.
 */
/*************************************************************************************************/
void RadioDrvSetAbortCback(RadioDrvAbortCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief      Enable the BB radio.
 *
 *  \return     None.
 *
 *  The radio should be enabled, possibly after leaving sleep.  The XTAL warmup must be started, but
 *  no radio operation will be attempted for xtalWarmup time, when the XTAL must be ready.
 */
/*************************************************************************************************/
void RadioDrvEnable(void);

/*************************************************************************************************/
/*!
 *  \brief      Disable the BB radio.
 *
 *  \return     None.
 *
 *  The radio should be disabled, possibly before entering sleep.  Any ongoing transmit or receive
 *  should be stopped.  The XTAL may be disabled.
 */
/*************************************************************************************************/
void RadioDrvDisable(void);

/*************************************************************************************************/
/*!
 *  \brief      Set RF Debug Mode.
 *
 *  \param      ddmSetting             DDM mode for different modules.
 *  \param      ddmDir                 DDM mode direction.
 *  \param      peripheralSetting      RF setting for debug mode.
 *  \param      peripheralDir          RX or TX.
 *
 *  \return     bool_t                 TRUE if handled here.
 */
/*************************************************************************************************/
bool_t RadioDrvSetDdm(uint32_t ddmSetting, uint32_t ddmDir, uint32_t peripheralSetting, uint32_t peripheralDir);

/*************************************************************************************************/
/*!
 *  \brief      Wait until radio is in idle state.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void RadioDrvWaitForIdle(void);

/*************************************************************************************************/
/*!
 *  \brief      Set radio channel parameters.
 *
 *  \param      opType          Operation type.
 *  \param      rfFreq          RF frequency in MHz.
 *  \param      txPhy           Transmitter PHY.
 *  \param      rxPhy           Receiver PHY.
 *  \param      phyOptions      PHY options.
 *  \param      txPower         Transmit power in dBm.
 *
 *  \return     None.
 *
 *  The channel parameters remain active until new parameters are set, the radio is disabled, or a
 *  radio operation is stopped.
 */
/*************************************************************************************************/
void RadioDrvSetChannelParam(uint8_t opType, uint16_t rfFreq, uint8_t txPhy, uint8_t rxPhy, uint8_t phyOptions, int8_t txPower);

/*************************************************************************************************/
/*!
 *  \brief      Start transmitter.
 *
 *  \return     None.
 *
 *  Prepare the transmitter, so that the warmup will begin at the radio request, with the
 *  modulator producing the first bit after txOnLatency.  The transmitter should automatically stop
 *  when the transmit ends so that another transmit or a receive can be started.
 */
/*************************************************************************************************/
void RadioDrvStartTx(void);

/*************************************************************************************************/
/*!
 *  \brief      Start receiver.
 *
 *  \return     None.
 *
 *  Prepare the receiver, so that warmup will begin at the radio request, with the demodulator
 *  expecting the first bit after rxOnLatency.  The receiver should automatically stop when the
 *  receive ends so that another recieve or a transmit can be started.
 */
/*************************************************************************************************/
void RadioDrvStartRx(void);

/*************************************************************************************************/
/*!
 *  \brief      Start transmitter in continuous mode.
 *
 *  \return     None.
 *
 *  Start the transmitter immediately and stay on indefinitely.
 */
/*************************************************************************************************/
void RadioDrvStartContinuousTx(void);

/*************************************************************************************************/
/*!
 *  \brief      Start receiver in continuous mode.
 *
 *  \return     None.
 *
 *  Start the receiver immediately and stay on indefinitely.
 */
/*************************************************************************************************/
void RadioDrvStartContinuousRx(void);

/*************************************************************************************************/
/*!
 *  \brief      Stop transmitter or receiver.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void RadioDrvStop(void);

/*************************************************************************************************/
/*!
 *  \brief      Radio driver set PMU and clk.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void RadioDrvSetPMUClk(void);

/*************************************************************************************************/
/*!
 *  \brief      Fill the buffer with random bytes
 *
 *  \param      pBufferRandom   Pointer to buffer
 *  \param      numRandomBytes  Number of bytes to write
 *
 *  \return     None.
 *
 *  \note       Because this function takes manual control of the radio it cannot be used when
 *              the radio is, or might become active. Typically this function will only be used
 *              during boot time to provide random numbers that are used for initialising other
 *              parts of the system.
 */
/*************************************************************************************************/
void RadioDrvGetRandomBytes(uint8_t *pBufferRandom, uint8_t numRandomBytes);

#ifdef __cplusplus
};
#endif

#endif /* RADIO_DRV_H */
