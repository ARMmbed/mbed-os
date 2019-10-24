/* 
 * Copyright (c) 2019-2020 Nuvoton Technology Corporation
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


#include "cmsis.h"
#include "mbed_assert.h"
#include "mbed_critical.h"
#include "mbed_error.h"
#include <limits.h>
#include "nu_modutil.h"
#include "nu_bitutil.h"
#include "crypto-misc.h"
#include "platform/SingletonPtr.h"
#include "platform/PlatformMutex.h"

#if defined(MBEDTLS_CONFIG_HW_SUPPORT)

/* Consideration for choosing proper synchronization mechanism
 *
 * 1. We choose mutex to synchronize access to crypto non-SHA AC. We can guarantee:
 *    (1) No deadlock
 *        We just lock mutex for a short sequence of operations rather than the whole lifetime
 *        of crypto context.
 *    (2) No priority inversion
 *        Mutex supports priority inheritance and it is enabled.
 * 2. We choose atomic flag to synchronize access to crypto SHA AC. We can guarantee:
 *    (1) No deadlock
 *        With SHA AC not supporting context save & restore, we provide SHA S/W fallback when
 *        SHA AC is not available.
 *    (2) No biting CPU
 *        Same reason as above.
 */

/* Mutex for crypto AES AC management */
static SingletonPtr<PlatformMutex> crypto_aes_mutex;

/* Mutex for crypto DES AC management */
static SingletonPtr<PlatformMutex> crypto_des_mutex;

/* Mutex for crypto ECC AC management */
static SingletonPtr<PlatformMutex> crypto_ecc_mutex;

/* Atomic flag for crypto SHA AC management */
static core_util_atomic_flag crypto_sha_atomic_flag = CORE_UTIL_ATOMIC_FLAG_INIT;


/* Crypto (AES, DES, SHA, etc.) init counter. Crypto's keeps active as it is non-zero. */
static uint16_t crypto_init_counter = 0U;

/* Crypto done flags */
#define CRYPTO_DONE_OK              BIT0    /* Done with OK */
#define CRYPTO_DONE_ERR             BIT1    /* Done with error */

/* Track if PRNG H/W operation is done */
static volatile uint16_t crypto_prng_done;
/* Track if AES H/W operation is done */
static volatile uint16_t crypto_aes_done;
/* Track if DES H/W operation is done */
static volatile uint16_t crypto_des_done;
/* Track if ECC H/W operation is done */
static volatile uint16_t crypto_ecc_done;

static void crypto_submodule_prestart(volatile uint16_t *submodule_done);
static bool crypto_submodule_wait(volatile uint16_t *submodule_done);

/* As crypto init counter changes from 0 to 1:
 *
 * 1. Enable crypto clock
 * 2. Enable crypto interrupt
 */
void crypto_init(void)
{
    core_util_critical_section_enter();
    if (crypto_init_counter == USHRT_MAX) {
        core_util_critical_section_exit();
        error("Crypto clock enable counter would overflow (> USHRT_MAX)");
    }
    core_util_atomic_incr_u16(&crypto_init_counter, 1);
    if (crypto_init_counter == 1) {
        // Enable IP clock
        SYS_UnlockReg();    // Unlock protected register
        CLK_EnableModuleClock(CRPT_MODULE);
        SYS_LockReg();      // Lock protected register
        
        NVIC_EnableIRQ(CRPT_IRQn);
    }
    core_util_critical_section_exit();
}

/* As crypto init counter changes from 1 to 0:
 *
 * 1. Disable crypto interrupt 
 * 2. Disable crypto clock
 */
void crypto_uninit(void)
{
    core_util_critical_section_enter();
    if (crypto_init_counter == 0) {
        core_util_critical_section_exit();
        error("Crypto clock enable counter would underflow (< 0)");
    }
    core_util_atomic_decr_u16(&crypto_init_counter, 1);
    if (crypto_init_counter == 0) {
        NVIC_DisableIRQ(CRPT_IRQn);
        
        SYS_UnlockReg();    // Unlock protected register
        CLK_DisableModuleClock(CRPT_MODULE);
        SYS_LockReg();      // Lock protected register
    }
    core_util_critical_section_exit();
}

/* Implementation that should never be optimized out by the compiler */
void crypto_zeroize(void *v, size_t n)
{
    volatile unsigned char *p = (volatile unsigned char*) v;
    while (n--) {
        *p++ = 0;
    }
}

/* Implementation that should never be optimized out by the compiler */
void crypto_zeroize32(uint32_t *v, size_t n)
{
    volatile uint32_t *p = (volatile uint32_t*) v;
    while (n--) {
        *p++ = 0;
    }
}

void crypto_aes_acquire(void)
{
    /* Don't check return code of Mutex::lock(void)
     *
     * This function treats RTOS errors as fatal system errors, so it can only return osOK.
     * Use of the return value is deprecated, as the return is expected to become void in
     * the future.
     */
    crypto_aes_mutex->lock();
}

void crypto_aes_release(void)
{
    crypto_aes_mutex->unlock();
}

void crypto_des_acquire(void)
{
    /* Don't check return code of Mutex::lock(void) */
    crypto_des_mutex->lock();
}

void crypto_des_release(void)
{
    crypto_des_mutex->unlock();
}

void crypto_ecc_acquire(void)
{
    /* Don't check return code of Mutex::lock(void) */
    crypto_ecc_mutex->lock();
}

void crypto_ecc_release(void)
{
    crypto_ecc_mutex->unlock();
}

bool crypto_sha_try_acquire(void)
{
    return !core_util_atomic_flag_test_and_set(&crypto_sha_atomic_flag);
}

void crypto_sha_release(void)
{
    core_util_atomic_flag_clear(&crypto_sha_atomic_flag);
}

void crypto_prng_prestart(void)
{
    crypto_submodule_prestart(&crypto_prng_done);
}

bool crypto_prng_wait(void)
{
    return crypto_submodule_wait(&crypto_prng_done);
}

void crypto_aes_prestart(void)
{
    crypto_submodule_prestart(&crypto_aes_done);
}

bool crypto_aes_wait(void)
{
    return crypto_submodule_wait(&crypto_aes_done);
}

void crypto_des_prestart(void)
{
    crypto_submodule_prestart(&crypto_des_done);
}

bool crypto_des_wait(void)
{
    return crypto_submodule_wait(&crypto_des_done);
}

void crypto_ecc_prestart(void)
{
    crypto_submodule_prestart(&crypto_ecc_done);
}

bool crypto_ecc_wait(void)
{
    return crypto_submodule_wait(&crypto_ecc_done);
}

bool crypto_dma_buff_compat(const void *buff, size_t buff_size, size_t size_aligned_to)
{
    uint32_t buff_ = (uint32_t) buff;

    return (((buff_ & 0x03) == 0) &&                                        /* Word-aligned buffer base address */
        ((buff_size & (size_aligned_to - 1)) == 0) &&                       /* Crypto submodule dependent buffer size alignment */
        (((buff_ >> 28) == 0x2) && (buff_size <= (0x30000000 - buff_))));   /* 0x20000000-0x2FFFFFFF */
}

/* Overlap cases
 *
 * 1. in_buff in front of out_buff:
 *
 * in             in_end
 * |              |
 * ||||||||||||||||
 *     ||||||||||||||||
 *     |              |
 *     out            out_end
 *
 * 2. out_buff in front of in_buff:
 *
 *     in             in_end
 *     |              |
 *     ||||||||||||||||
 * ||||||||||||||||
 * |              |
 * out            out_end
 */
bool crypto_dma_buffs_overlap(const void *in_buff, size_t in_buff_size, const void *out_buff, size_t out_buff_size)
{
    uint32_t in = (uint32_t) in_buff;
    uint32_t in_end = in + in_buff_size;
    uint32_t out = (uint32_t) out_buff;
    uint32_t out_end = out + out_buff_size;

    bool overlap = (in <= out && out < in_end) || (out <= in && in < out_end);
    
    return overlap;
}

static void crypto_submodule_prestart(volatile uint16_t *submodule_done)
{
    *submodule_done = 0;
    
    /* Ensure memory accesses above are completed before DMA is started
     *
     * Replacing __DSB() with __DMB() is also OK in this case.
     *
     * Refer to "multi-master systems" section with DMA in:
     * https://static.docs.arm.com/dai0321/a/DAI0321A_programming_guide_memory_barriers_for_m_profile.pdf
     */
    __DSB();
}

static bool crypto_submodule_wait(volatile uint16_t *submodule_done)
{
    while (! *submodule_done);

    /* Ensure while loop above and subsequent code are not reordered */
    __DSB();

    if ((*submodule_done & CRYPTO_DONE_OK)) {
        /* Done with OK */
        return true;
    } else if ((*submodule_done & CRYPTO_DONE_ERR)) {
        /* Done with error */
        return false;
    }

    return false;
}

/* Crypto interrupt handler
 * 
 * There's inconsistency in cryptography related naming, Crpt or Crypto. For example,
 * cryptography IRQ handler could be CRPT_IRQHandler or CRYPTO_IRQHandler. To override
 * default cryptography IRQ handler, see device/startup_{CHIP}.c for its correct name
 * or call NVIC_SetVector() in crypto_init() regardless of its name. */
extern "C" void CRPT_IRQHandler()
{
    uint32_t intsts;
    
    if ((intsts = PRNG_GET_INT_FLAG(CRYPTO_MODBASE())) != 0) {
        /* Done with OK */
        crypto_prng_done |= CRYPTO_DONE_OK;
        /* Clear interrupt flag */
        PRNG_CLR_INT_FLAG(CRYPTO_MODBASE());
    }  else if ((intsts = AES_GET_INT_FLAG(CRYPTO_MODBASE())) != 0) {
        /* Done with OK */
        crypto_aes_done |= CRYPTO_DONE_OK;
        /* Clear interrupt flag */
        AES_CLR_INT_FLAG(CRYPTO_MODBASE());
    } else if ((intsts = TDES_GET_INT_FLAG(CRYPTO_MODBASE())) != 0) {
        /* Done with OK */
        crypto_des_done |= CRYPTO_DONE_OK;
        /* Clear interrupt flag */
        TDES_CLR_INT_FLAG(CRYPTO_MODBASE());
    } else if ((intsts = ECC_GET_INT_FLAG(CRYPTO_MODBASE())) != 0) {
        /* Check interrupt flags */
        if (intsts & CRPT_INTSTS_ECCIF_Msk) {
            /* Done with OK */
            crypto_ecc_done |= CRYPTO_DONE_OK;
        } else if (intsts & CRPT_INTSTS_ECCEIF_Msk) {
            /* Done with error */
            crypto_ecc_done |= CRYPTO_DONE_ERR;
        }
        /* Clear interrupt flag */
        ECC_CLR_INT_FLAG(CRYPTO_MODBASE());
    }
}

#endif /* #if defined(MBEDTLS_CONFIG_HW_SUPPORT) */
