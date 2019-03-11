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
#include "stack/platform/include/pal_types.h"
#include "stack/platform/include/pal_uart.h"
#include "stack/platform/include/pal_nvm.h"
#include "hal/ticker_api.h"

#ifdef __cplusplus
extern "C" {
#endif

static const mbed_error_status_t function_not_implemented = MBED_MAKE_ERROR(MBED_MODULE_BLE, ENOSYS);

/* UART */

MBED_WEAK PalUartState_t PalUartGetState(PalUartId_t id)
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalUartGetState");
    return PAL_UART_STATE_UNINIT;
}

MBED_WEAK void PalUartInit(PalUartId_t id, const PalUartConfig_t *pCfg)
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalUartInit");
}

MBED_WEAK void PalUartReadData(PalUartId_t id, uint8_t *pData, uint16_t len)
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalUartReadData");
}

MBED_WEAK void PalUartWriteData(PalUartId_t id, const uint8_t *pData, uint16_t len)
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalUartWriteData");
}

/* NVM */

MBED_WEAK void PalNvmInit(PalNvmCback_t actCback)
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalNvmInit");
}

MBED_WEAK PalNvmState_t PalNvmGetState()
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalNvmGetState");
    return PAL_NVM_STATE_UNINIT;
}

MBED_WEAK void PalNvmRead(void *pBuf, uint32_t size, uint32_t srcAddr)
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalNvmRead");
}

MBED_WEAK void PalNvmWrite(void *pBuf, uint32_t size, uint32_t dstAddr)
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalNvmWrite");
}

MBED_WEAK void PalNvmEraseSector(uint32_t size, uint32_t startAddr)
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalNvmEraseSector");
}

/* LED */

MBED_WEAK void PalLedOn(uint8_t id)
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalLedOn");
}

MBED_WEAK void PalLedOff(uint8_t id)
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalLedOff");
}

/* RTC */

MBED_WEAK void PalRtcInit()
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalRtcInit");
}

MBED_WEAK void PalRtcEnableCompareIrq()
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalRtcEnableCompareIrq");
}

MBED_WEAK void PalRtcDisableCompareIrq()
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalRtcDisableCompareIrq");
}

MBED_WEAK uint32_t PalRtcCounterGet()
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalRtcDisableCompareIrq");
    return 0;
}

MBED_WEAK void PalRtcCompareSet(uint32_t value)
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalRtcCompareSet");
}

MBED_WEAK uint32_t PalRtcCompareGet()
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalRtcCompareGet");
    return 0;
}

/* SYS */

MBED_WEAK bool_t PalSysIsBusy()
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalSysIsBusy");
    return 0;
}

MBED_WEAK void PalSysAssertTrap()
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalSysAssertTrap");
}

MBED_WEAK void PalSysSleep()
{
    MBED_ERROR(function_not_implemented, "Provide implementation of PalSysSleep");
}

#ifdef __cplusplus
};
#endif
