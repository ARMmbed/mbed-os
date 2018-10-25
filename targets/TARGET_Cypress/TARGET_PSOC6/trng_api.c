/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#include "device.h"
#include "mbed_critical.h"
#include "mbed_error.h"
#include "trng_api.h"
#include "drivers/peripheral/ipc/cy_ipc_drv.h"
#include "drivers/peripheral/crypto/cy_crypto_config.h"
#include "drivers/peripheral/crypto/cy_crypto.h"
#include "drivers/peripheral/crypto/cy_crypto_server.h"


#if DEVICE_TRNG

/* Polynomial configuration for the programmable Galois and Fibonacci ring oscillators */
#define CRYPTO_TRNG_GARO_POL             (0x42000000)
#define CRYPTO_TRNG_FIRO_POL             (0x43000000)


static uint32_t trng_initialized = 0;

/** The Crypto configuration structure. */
static const cy_stc_crypto_config_t crypto_config = {
    /* .ipcChannel             */ CY_IPC_CHAN_CRYPTO,
    /* .acquireNotifierChannel */ CY_CRYPTO_IPC_INTR_NOTIFY_NUM,
    /* .releaseNotifierChannel */ CY_CRYPTO_IPC_INTR_RELEASE_NUM,

    /* .releaseNotifierConfig */ {
#if (CY_CPU_CORTEX_M0P)
        /* .intrSrc            */ CY_CRYPTO_CM0_RELEASE_INTR_NR,
        /* .cm0pSrc            */ (cy_en_intr_t)CY_IPC_INTR_NUM_TO_VECT((int32_t)CY_CRYPTO_IPC_INTR_RELEASE_NUM),
#else
        /* .intrSrc            */ (IRQn_Type)CY_IPC_INTR_NUM_TO_VECT((int32_t)CY_CRYPTO_IPC_INTR_RELEASE_NUM),
#endif
        /* .intrPriority       */ CY_CRYPTO_RELEASE_INTR_PR,
    },
    /* .userCompleteCallback   */ NULL

#if (CY_CRYPTO_CORE_ENABLE)
    ,
    /* .userGetDataHandler     */ NULL,
    /* .userErrorHandler       */ NULL,

    /* .acquireNotifierConfig */ {
#if (CY_CPU_CORTEX_M0P)
        /* .intrSrc            */ CY_CRYPTO_CM0_NOTIFY_INTR_NR,
        /* .cm0pSrc            */ (cy_en_intr_t)CY_IPC_INTR_NUM_TO_VECT((int32_t)CY_CRYPTO_IPC_INTR_NOTIFY_NUM),
#else
        /* .intrSrc            */ (IRQn_Type)CY_IPC_INTR_NUM_TO_VECT((int32_t)CY_CRYPTO_IPC_INTR_NOTIFY_NUM),
#endif
        /* .intrPriority       */ CY_CRYPTO_NOTIFY_INTR_PR,
    },
    /* .cryptoErrorIntrConfig */ {
#if (CY_CPU_CORTEX_M0P)
        /* .intrSrc            */ CY_CRYPTO_CM0_ERROR_INTR_NR,
        /* .cm0pSrc            */ cpuss_interrupt_crypto_IRQn,
#else
        /* .intrSrc            */ cpuss_interrupt_crypto_IRQn,
#endif
        /* .intrPriority       */ CY_CRYPTO_ERROR_INTR_PR,
    }
#endif
};

#if defined(TARGET_MCU_PSOC6_M0)
static cy_stc_crypto_server_context_t crypto_server_context;
#endif
static cy_stc_crypto_context_t crypto_scratch;
static cy_stc_crypto_context_trng_t trng_context;

void trng_init(trng_t *obj)
{
    (void) obj;

    if (core_util_atomic_incr_u32(&trng_initialized, 1) > 1) {
        error("Only single instance of TRNG is supported.");
    }

    /* Start the Crypto Server, only on Cm0 core */
#if defined(TARGET_MCU_PSOC6_M0)
    Cy_Crypto_Server_Start(&crypto_config, &crypto_server_context);
    Cy_Crypto_Sync(CY_CRYPTO_SYNC_BLOCKING);
#endif

    /* Initialize the Crypto Driver */
    Cy_Crypto_Init(&crypto_config, &crypto_scratch);
    Cy_Crypto_Sync(CY_CRYPTO_SYNC_BLOCKING);

    /* Enable Crypto Engine */
    Cy_Crypto_Enable();
    Cy_Crypto_Sync(CY_CRYPTO_SYNC_BLOCKING);

}

void trng_free(trng_t *obj)
{
    (void) obj;

    Cy_Crypto_Enable();
    Cy_Crypto_Sync(CY_CRYPTO_SYNC_BLOCKING);
    Cy_Crypto_DeInit();

#if defined(TARGET_MCU_PSOC6_M0)
    Cy_Crypto_Server_Stop();
#endif
    trng_initialized = 0;
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    uint8_t     random_val[4];
    uint32_t    i;

    (void) obj;

    *output_length = length;
    while (length > 0) {
        if (Cy_Crypto_Trng_Generate(CRYPTO_TRNG_GARO_POL,
                                    CRYPTO_TRNG_FIRO_POL,
                                    32,
                                    (uint32_t *)random_val,
                                    &trng_context) != CY_CRYPTO_SUCCESS) {
            *output_length = 0;
            return (-1);
        }

        Cy_Crypto_Sync(CY_CRYPTO_SYNC_BLOCKING);

        for (i = 0; (i < 4) && (length > 0); ++i) {
            *output++ = random_val[i];
            --length;
        }
    }

    return 0;
}

#endif // DEVICE_TRNG
