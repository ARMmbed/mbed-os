/* Copyright (c) 2018 Arm Limited
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

#ifdef __cplusplus
extern "C" {
#endif

/* UART */

PalUartState_t PalUartGetState(PalUartId_t id)
{
    return PAL_UART_STATE_UNINIT;
}

void PalUartInit(PalUartId_t id, const PalUartConfig_t *pCfg)
{
}

void PalUartReadData(PalUartId_t id, uint8_t *pData, uint16_t len)
{
}

void PalUartWriteData(PalUartId_t id, const uint8_t *pData, uint16_t len)
{
}

/* NVM */

void PalNvmInit(PalNvmCback_t actCback)
{
}

PalNvmState_t PalNvmGetState()
{
    return PAL_NVM_STATE_UNINIT;
}

void PalNvmRead(void *pBuf, uint32_t size, uint32_t srcAddr)
{
}

void PalNvmWrite(void *pBuf, uint32_t size, uint32_t dstAddr)
{
}

void PalNvmEraseSector(uint32_t size, uint32_t startAddr)
{
}

/* LED */

void PalLedOn(uint8_t id)
{
}

void PalLedOff(uint8_t id)
{
}

/* RTC */

void PalRtcInit()
{
}

void PalRtcEnableCompareIrq()
{
}

void PalRtcDisableCompareIrq()
{
}

uint32_t PalRtcCounterGet()
{
    return 0;
}

void PalRtcCompareSet(uint32_t value)
{
}

uint32_t PalRtcCompareGet()
{
    return 0;
}

/* SYS */

bool_t PalSysIsBusy()
{
    return 0;
}

void PalSysAssertTrap()
{
}

void PalSysSleep()
{
}

#ifdef __cplusplus
};
#endif
