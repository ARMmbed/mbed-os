/***************************************************************************//**
* @file resetreason_api.c
*******************************************************************************
* @section License
* <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
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

#include "pic32cx.h"
#include "rstc.h"

reset_reason_t hal_reset_reason_get(void)
{
  uint32_t reason_cause = rstc_get_reset_cause(RSTC);
  
  if (reason_cause == RSTC_SR_RSTTYP_GENERAL_RST) {
    return RESET_REASON_POWER_ON;
  }
  
  if (reason_cause == RSTC_SR_RSTTYP_BACKUP_RST) {
    return RESET_REASON_WAKE_LOW_POWER;
  }
  
  if ((reason_cause == RSTC_SR_RSTTYP_WDT0_RST) ||  (reason_cause == RSTC_SR_RSTTYP_WDT1_RST)) {
    return RESET_REASON_WATCHDOG;
  }
  
  if (reason_cause == RSTC_SR_RSTTYP_SOFT_RST) {
    return RESET_REASON_SOFTWARE;
  }
  
  if (reason_cause == RSTC_SR_RSTTYP_USER_RST) {
    return RESET_REASON_PIN_RESET;
  }
  
  if (reason_cause == RSTC_SR_RSTTYP_CORE_SM_RST) {
    return RESET_REASON_BROWN_OUT;
  }
  
  if (reason_cause == RSTC_SR_RSTTYP_CPU_FAIL_RST) {
    return RESET_REASON_PLATFORM;
  }
  
  if (reason_cause == RSTC_SR_RSTTYP_SLCK_XTAL_RST) {
    return RESET_REASON_PLATFORM;
  }
  
  if (reason_cause == RSTC_SR_RSTTYP_PORIO_RST) {
    return RESET_REASON_BROWN_OUT;
  }
  
  return RESET_REASON_UNKNOWN;
}

uint32_t hal_reset_reason_get_raw(void)
{
  return (rstc_get_reset_cause(RSTC) >> RSTC_SR_RSTTYP_Pos);
}

void hal_reset_reason_clear(void)
{
  /* The RSTC_SR register is read-only */
}

void hal_reset_reason_get_capabilities(reset_reason_capabilities_t *cap)
{
  cap->reasons = 1 << RESET_REASON_UNKNOWN;
  cap->reasons |= 1 << RESET_REASON_POWER_ON;
  cap->reasons |= 1 << RESET_REASON_WAKE_LOW_POWER;
  cap->reasons |= 1 << RESET_REASON_WATCHDOG;
  cap->reasons |= 1 << RESET_REASON_SOFTWARE;
  cap->reasons |= 1 << RESET_REASON_PIN_RESET;
  cap->reasons |= 1 << RESET_REASON_BROWN_OUT;
  cap->reasons |= 1 << RESET_REASON_PLATFORM;
}

#endif /* DEVICE_RESET_REASON */
