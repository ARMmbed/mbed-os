/***************************************************************************//**
 * @file resetreason_api.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2018 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "device.h"
#include "reset_reason_api.h"

#if DEVICE_RESET_REASON

#include "em_rmu.h"

reset_reason_t hal_reset_reason_get(void)
{
	uint32_t reasonmask = RMU_ResetCauseGet();

	/* Only care about the upper bit, since that is 'most significant' reset reason */
	reasonmask = 1 << (31 - __CLZ(reasonmask));

	if (reasonmask & RMU_RSTCAUSE_PORST) {
		return RESET_REASON_POWER_ON;
	}
#if defined(RMU_RSTCAUSE_BODUNREGRST)
	if (reasonmask & RMU_RSTCAUSE_BODUNREGRST) {
		return RESET_REASON_BROWN_OUT;
	}
#endif
#if defined(RMU_RSTCAUSE_BODREGRST)
	if (reasonmask & RMU_RSTCAUSE_BODREGRST) {
		return RESET_REASON_BROWN_OUT;
	}
#endif
#if defined(RMU_RSTCAUSE_AVDDBOD)
	if (reasonmask & RMU_RSTCAUSE_AVDDBOD) {
		return RESET_REASON_BROWN_OUT;
	}
#endif
#if defined(RMU_RSTCAUSE_DVDDBOD)
	if (reasonmask & RMU_RSTCAUSE_DVDDBOD) {
		return RESET_REASON_BROWN_OUT;
	}
#endif
#if defined(RMU_RSTCAUSE_DECBOD)
	if (reasonmask & RMU_RSTCAUSE_DECBOD) {
		return RESET_REASON_BROWN_OUT;
	}
#endif
	if (reasonmask & RMU_RSTCAUSE_EXTRST) {
		return RESET_REASON_PIN_RESET;
	}
	if (reasonmask & RMU_RSTCAUSE_WDOGRST) {
		return RESET_REASON_WATCHDOG;
	}
	if (reasonmask & RMU_RSTCAUSE_LOCKUPRST) {
		return RESET_REASON_LOCKUP;
	}
	if (reasonmask & RMU_RSTCAUSE_SYSREQRST) {
		return RESET_REASON_SOFTWARE;
	}
#if defined(RMU_RSTCAUSE_EM4RST)
	if (reasonmask & RMU_RSTCAUSE_EM4RST) {
		return RESET_REASON_WAKE_LOW_POWER;
	}
#endif
#if defined(RMU_RSTCAUSE_EM4WURST)
	if (reasonmask & RMU_RSTCAUSE_EM4WURST) {
		return RESET_REASON_WAKE_LOW_POWER;
	}
#endif
#if defined(RMU_RSTCAUSE_BODAVDD0)
	if (reasonmask & RMU_RSTCAUSE_BODAVDD0) {
		return RESET_REASON_BROWN_OUT;
	}
#endif
#if defined(RMU_RSTCAUSE_BODAVDD1)
	if (reasonmask & RMU_RSTCAUSE_BODAVDD1) {
		return RESET_REASON_BROWN_OUT;
	}
#endif
#if defined(BU_PRESENT) && defined(_SILICON_LABS_32B_SERIES_0)
	if (reasonmask & RMU_RSTCAUSE_BUBODVDDDREG) {
		return RESET_REASON_BROWN_OUT;
	}
	if (reasonmask & RMU_RSTCAUSE_BUBODBUVIN) {
		return RESET_REASON_BROWN_OUT;
	}
	if (reasonmask & RMU_RSTCAUSE_BUBODUNREG) {
		return RESET_REASON_BROWN_OUT;
	}
	if (reasonmask & RMU_RSTCAUSE_BUBODREG) {
		return RESET_REASON_BROWN_OUT;
	}
	if (reasonmask & RMU_RSTCAUSE_BUMODERST) {
		return RESET_REASON_PLATFORM;
	}
#elif defined(RMU_RSTCAUSE_BUMODERST)
	if (reasonmask & RMU_RSTCAUSE_BUMODERST) {
		return RESET_REASON_PLATFORM;
	}
#endif
	return RESET_REASON_UNKNOWN;
}

uint32_t hal_reset_reason_get_raw(void)
{
	return RMU->RSTCAUSE;
}

void hal_reset_reason_clear(void)
{
	RMU_ResetCauseClear();
}

#endif /* DEVICE_RESET_REASON */
