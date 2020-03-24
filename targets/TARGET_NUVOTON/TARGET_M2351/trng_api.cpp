/*
 * Copyright (c) 2017-2018, Nuvoton Technology Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
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

#if DEVICE_TRNG

#include "cmsis.h"
#include <limits.h>
#include "crypto-misc.h"
#include "hal/trng_api.h"
#include "platform/mbed_toolchain.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_error.h"
#include "nu_modutil.h"
#include "hal_secure.h"
#if defined(DOMAIN_NS) && (DOMAIN_NS == 1L) && (TFM_LVL > 0)
#include "tfm_ns_lock.h"
#endif
#include "partition_M2351.h"

#if defined(SCU_INIT_PNSSET5_VAL) && (SCU_INIT_PNSSET5_VAL & (1 << 25))
#error("We just support secure TRNG")
#endif

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3L)

/* Module init definition: modname, clkidx, clksrc, clkdiv, rstidx, irqnum, misc */
static const struct nu_modinit_s trng_modinit = {TRNG_0, TRNG_MODULE, 0, 0, TRNG_RST, TRNG_IRQn, NULL};

/* TRNG init counter. TRNG is kept active as it is non-zero. */
static uint16_t trng_init_counter = 0U;

#endif

void trng_init(trng_t *obj)
{
    trng_init_s(obj);
}

void trng_free(trng_t *obj)
{
    trng_free_s(obj);
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    uint32_t output_length_;
    int32_t rc = trng_get_bytes_s(obj, output, (uint32_t) length, &output_length_);
    if (output_length) {
        *output_length = output_length_;
    }
    return rc;
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)

static void trng_init_impl(void *obj);
static void trng_free_impl(void *obj);
static int32_t trng_get_bytes_impl(void *obj, uint8_t *output, uint32_t length, uint32_t *output_length);

static void trng_init_impl(MBED_UNUSED void *obj)
{
    core_util_critical_section_enter();
    if (trng_init_counter == USHRT_MAX) {
        core_util_critical_section_exit();
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_HAL, MBED_ERROR_CODE_OVERFLOW), \
                   "TRNG initialization counter would overflow");
    }
    ++ trng_init_counter;
    if (trng_init_counter == 1) {
        /* Enable IP clock (secure version) */
        CLK_EnableModuleClock_S(trng_modinit.clkidx);

        /* Reset IP (secure version) */
        SYS_ResetModule_S(trng_modinit.rsetidx);

        TRNG_T *trng_base = (TRNG_T *) NU_MODBASE(trng_modinit.modname);

        trng_base->ACT |= TRNG_ACT_ACT_Msk;
        while (!(trng_base->CTL & TRNG_CTL_READY_Msk));
    }
    core_util_critical_section_exit();
}

static void trng_free_impl(MBED_UNUSED void *obj)
{
    core_util_critical_section_enter();
    if (trng_init_counter == 0) {
        core_util_critical_section_exit();
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_HAL, MBED_ERROR_CODE_UNDERFLOW), \
                   "TRNG initialization counter would underflow");
    }
    -- trng_init_counter;
    if (trng_init_counter == 0) {
        TRNG_T *trng_base = (TRNG_T *) NU_MODBASE(trng_modinit.modname);

        trng_base->ACT &= ~TRNG_ACT_ACT_Msk;

        /* Disable IP clock (secure version) */
        CLK_DisableModuleClock_S(trng_modinit.clkidx);
    }
    core_util_critical_section_exit();
}

static int32_t trng_get_bytes_impl(MBED_UNUSED void *obj, uint8_t *output, uint32_t length, uint32_t *output_length)
{
    /* Check augument validity */
    if (!output && length) {
        return -1;
    }

    uint8_t *output_ind = output;
    uint8_t *output_end = output + length;

    TRNG_T *trng_base = (TRNG_T *) NU_MODBASE(trng_modinit.modname);

    for (; output_ind != output_end; output_ind ++) {
        trng_base->CTL |= TRNG_CTL_TRNGEN_Msk;
        while (!(trng_base->CTL & TRNG_CTL_DVIF_Msk));
        *output_ind = trng_base->DATA & 0xff;
    }

    if (output_length) {
        *output_length = length;
    }

    return 0;
}

#if (TFM_LVL > 0)

/* Add 'extern "C"' here to get around compile error on ARMC6 */

extern "C"
__NONSECURE_ENTRY
int32_t trng_init_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
    void *obj = (void *) arg0;
    trng_init_impl(obj);
    return 0;
}

extern "C"
__NONSECURE_ENTRY
int32_t trng_free_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
    void *obj = (void *) arg0;
    trng_free_impl(obj);
    return 0;
}

extern "C"
__NONSECURE_ENTRY
int32_t trng_get_bytes_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
    void *obj = (void *) arg0;
    uint8_t *output = (uint8_t *) arg1;
    uint32_t *length_bidi = (uint32_t *) arg2;
    int32_t *status = (int32_t *) arg3;
    *status = trng_get_bytes_impl(obj, output, *length_bidi, length_bidi);

    return 0;
}

#endif

#endif

#if defined(DOMAIN_NS) && (DOMAIN_NS == 1) && (TFM_LVL > 0)

void trng_init_s(void *obj)
{
    tfm_ns_lock_dispatch(trng_init_veneer, (uint32_t) obj, 0, 0, 0);
}

void trng_free_s(void *obj)
{
    tfm_ns_lock_dispatch(trng_free_veneer, (uint32_t) obj, 0, 0, 0);
}

int32_t trng_get_bytes_s(void *obj, uint8_t *output, uint32_t length, uint32_t *output_length)
{
    uint32_t length_bidi = length;
    int32_t status;

    tfm_ns_lock_dispatch(trng_get_bytes_veneer, (uint32_t) obj, (uint32_t) output, (uint32_t) &length_bidi, (uint32_t) &status);

    if (output_length) {
        *output_length = length_bidi;
    }
    
    return status;
}

#elif defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)

/* Add 'extern "C"' here to get around compile error on ARMC6 */

#if (TFM_LVL == 0)
extern "C"
__NONSECURE_ENTRY
#endif
void trng_init_s(void *obj)
{
    trng_init_impl(obj);
}

#if (TFM_LVL == 0)
extern "C"
__NONSECURE_ENTRY
#endif
void trng_free_s(void *obj)
{
    trng_free_impl(obj);
}

#if (TFM_LVL == 0)
extern "C"
__NONSECURE_ENTRY
#endif
int32_t trng_get_bytes_s(void *obj, uint8_t *output, uint32_t length, uint32_t *output_length)
{
    return trng_get_bytes_impl(obj, output, length, output_length);
}

#endif

#endif
