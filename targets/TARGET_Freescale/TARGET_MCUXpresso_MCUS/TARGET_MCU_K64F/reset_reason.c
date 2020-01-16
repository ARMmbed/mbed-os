/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "reset_reason_api.h"

#if DEVICE_RESET_REASON

#include "fsl_rcm.h"

reset_reason_t hal_reset_reason_get(void)
{
    const uint32_t reset_sources =
        RCM_GetPreviousResetSources(RCM) & kRCM_SourceAll;

    // Low power mode is exited via the RESET pin. Therefore, when this reset is
    // triggered both the PIN and WAKEUP will have bits set, so check this flag
    // first.
#if (defined(FSL_FEATURE_RCM_HAS_WAKEUP) && FSL_FEATURE_RCM_HAS_WAKEUP)
    if ((reset_sources & kRCM_SourceWakeup) != 0) {
        return RESET_REASON_PLATFORM;
    }
#endif

    // Check POR flag first. During a POR reset there will be two reset sources
    // set: POR and LVD. As during the power on phase the low voltage detector
    // circuit will detect a low voltage while the voltage is initially ramping
    // up and set the BROWN_OUT flag. Therefore, if LVD is set we must check the
    // POR to determine what the actual cause was.
    if ((reset_sources & kRCM_SourcePor) != 0) {
        return RESET_REASON_POWER_ON;
    }

    if ((reset_sources & kRCM_SourceLvd) != 0) {
        return RESET_REASON_BROWN_OUT;
    }

    if ((reset_sources & kRCM_SourceWdog) != 0) {
        return RESET_REASON_WATCHDOG;
    }

    if ((reset_sources & kRCM_SourcePin) != 0) {
        return RESET_REASON_PIN_RESET;
    }

    if ((reset_sources & kRCM_SourceSw) != 0) {
        return RESET_REASON_SOFTWARE;
    }

#if (defined(FSL_FEATURE_RCM_HAS_LOC) && FSL_FEATURE_RCM_HAS_LOC)
    if ((reset_sources & kRCM_SourceLoc) != 0) {
        return RESET_REASON_PLATFORM;
    }
#endif

#if (defined(FSL_FEATURE_RCM_HAS_LOL) && FSL_FEATURE_RCM_HAS_LOL)
    if ((reset_sources & kRCM_SourceLol) != 0) {
        return RESET_REASON_PLATFORM;
    }
#endif

#if (defined(FSL_FEATURE_RCM_HAS_JTAG) && FSL_FEATURE_RCM_HAS_JTAG)
    if ((reset_sources & kRCM_SourceJtag) != 0) {
        return RESET_REASON_PLATFORM;
    }
#endif

#if (defined(FSL_FEATURE_RCM_HAS_MDM_AP) && FSL_FEATURE_RCM_HAS_MDM_AP)
    if ((reset_sources & kRCM_SourceMdmap) != 0) {
        return RESET_REASON_PLATFORM;
    }
#endif

#if (defined(FSL_FEATURE_RCM_HAS_EZPORT) && FSL_FEATURE_RCM_HAS_EZPORT)
    if ((reset_sources & kRCM_SourceEzpt) != 0) {
        return RESET_REASON_PLATFORM;
    }
#endif

    return RESET_REASON_UNKNOWN;
}


uint32_t hal_reset_reason_get_raw(void)
{
    return (RCM_GetPreviousResetSources(RCM) & kRCM_SourceAll);
}


void hal_reset_reason_clear(void)
{
#if (defined(FSL_FEATURE_RCM_HAS_SSRS) && FSL_FEATURE_RCM_HAS_SSRS)
    RCM_ClearStickyResetSources(RCM, kRCM_SourceAll);
#endif
}

void hal_reset_reason_get_capabilities(reset_reason_capabilities_t *cap)
{
    cap->reasons = 1 << RESET_REASON_UNKNOWN;
    cap->reasons |= 1 << RESET_REASON_POWER_ON;
    cap->reasons |= 1 << RESET_REASON_BROWN_OUT;
    cap->reasons |= 1 << RESET_REASON_WATCHDOG;
    cap->reasons |= 1 << RESET_REASON_PIN_RESET;
    cap->reasons |= 1 << RESET_REASON_SOFTWARE;

#if (defined(FSL_FEATURE_RCM_HAS_WAKEUP) && FSL_FEATURE_RCM_HAS_WAKEUP) || \
    (defined(FSL_FEATURE_RCM_HAS_LOC) && FSL_FEATURE_RCM_HAS_LOC) || \
    (defined(FSL_FEATURE_RCM_HAS_LOL) && FSL_FEATURE_RCM_HAS_LOL) || \
    (defined(FSL_FEATURE_RCM_HAS_JTAG) && FSL_FEATURE_RCM_HAS_JTAG) || \
    (defined(FSL_FEATURE_RCM_HAS_MDM_AP) && FSL_FEATURE_RCM_HAS_MDM_AP) || \
    (defined(FSL_FEATURE_RCM_HAS_EZPORT) && FSL_FEATURE_RCM_HAS_EZPORT)
    cap->reasons |= 1 << RESET_REASON_PLATFORM;
#endif
}

#endif // DEVICE_RESET_REASON
