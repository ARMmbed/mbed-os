/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"){}
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

#include "mbed.h"
#include "pal_types.h"
#include "pal_uart.h"
#include "pal_nvm.h"
#include "hal/ticker_api.h"

#ifdef __cplusplus
extern "C" {
#endif

static const mbed_error_status_t function_not_implemented = MBED_MAKE_ERROR(MBED_MODULE_BLE, ENOSYS);
#define NOT_IMPLEMENTED(name) MBED_ERROR(function_not_implemented, "Provide implementation of "name".");

/* UART */

MBED_WEAK PalUartState_t PalUartGetState(PalUartId_t id)
{
    NOT_IMPLEMENTED("PalUartGetState");

    return PAL_UART_STATE_UNINIT;
}

MBED_WEAK void PalUartInit(PalUartId_t id, const PalUartConfig_t *pCfg)
{
    NOT_IMPLEMENTED("PalUartInit");
}

MBED_WEAK void PalUartReadData(PalUartId_t id, uint8_t *pData, uint16_t len)
{
    NOT_IMPLEMENTED("PalUartReadData");
}

MBED_WEAK void PalUartWriteData(PalUartId_t id, const uint8_t *pData, uint16_t len)
{
    NOT_IMPLEMENTED("PalUartWriteData");
}

/* NVM */

MBED_WEAK void PalNvmInit(PalNvmCback_t actCback)
{
    NOT_IMPLEMENTED("PalNvmInit");
}

MBED_WEAK PalNvmState_t PalNvmGetState()
{
    NOT_IMPLEMENTED("PalNvmGetState");

    return PAL_NVM_STATE_UNINIT;
}

MBED_WEAK void PalNvmRead(MBED_WEAK void *pBuf, uint32_t size, uint32_t srcAddr)
{
    NOT_IMPLEMENTED("PalNvmRead");
}

MBED_WEAK void PalNvmWrite(MBED_WEAK void *pBuf, uint32_t size, uint32_t dstAddr)
{
    NOT_IMPLEMENTED("PalNvmWrite");
}

MBED_WEAK void PalNvmEraseSector(uint32_t size, uint32_t startAddr)
{
    NOT_IMPLEMENTED("PalNvmEraseSector");
}

/* LED */

MBED_WEAK void PalLedOn(uint8_t id)
{
    NOT_IMPLEMENTED("PalLedOn");
}

MBED_WEAK void PalLedOff(uint8_t id)
{
    NOT_IMPLEMENTED("PalLedOff");
}

/* RTC */

MBED_WEAK void PalRtcInit()
{
    NOT_IMPLEMENTED("PalRtcInit");
}

MBED_WEAK void PalRtcEnableCompareIrq()
{
    NOT_IMPLEMENTED("PalRtcEnableCompareIrq");
}

MBED_WEAK void PalRtcDisableCompareIrq()
{
    NOT_IMPLEMENTED("PalRtcDisableCompareIrq");
}

MBED_WEAK uint32_t PalRtcCounterGet()
{
    return osKernelGetTickCount();
}

MBED_WEAK void PalRtcCompareSet(uint32_t value)
{
    NOT_IMPLEMENTED("PalRtcCompareSet");
}

MBED_WEAK uint32_t PalRtcCompareGet()
{
    NOT_IMPLEMENTED("PalRtcCompareGet");

    return 0;
}

/* SYS */

MBED_WEAK bool_t PalSysIsBusy()
{
    NOT_IMPLEMENTED("PalSysIsBusy");

    return 0;
}

MBED_WEAK void PalSysAssertTrap()
{
    NOT_IMPLEMENTED("PalSysAssertTrap");
}

MBED_WEAK void PalSysSleep()
{
    NOT_IMPLEMENTED("PalSysSleep");
}

#ifdef __cplusplus
};
#endif
