/*
 *  Silicon Labs CRYPTO device management interface.
 *
 *  Copyright (C) 2016, Silicon Labs, http://www.silabs.com
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "crypto_management.h"
#include "em_core.h"
#include "em_bus.h"

#if defined( CRYPTO_PRESENT )

/* Conversion macro for compatibility with the 5.3.x release of the Gecko SDK */
#if defined( MBEDTLS_CRYPTO_DEVICE_PREEMPTION )
#warning "MBEDTLS_CRYPTO_DEVICE_PREEMPTION is deprecated, please define " \
         "CRYPTO_DEVICE_PREEMPTION instead."
#endif

#if defined( MBEDTLS_THREADING_C )
#include "mbedtls/threading.h"
static mbedtls_threading_mutex_t    crypto_locks[CRYPTO_COUNT];
static volatile bool                crypto_locks_initialized = false;
static unsigned int                 acquire_count = 0U;
#endif /* MBEDTLS_THREADING_C */

#if defined( CRYPTO_DEVICE_PREEMPTION )
/** Preemptable context of CRYPTO hardware module. */
typedef struct
{
    uint32_t CTRL;          /*!< Control Register  */
    uint32_t WAC;           /*!< Wide Arithmetic Configuration  */
    uint32_t SEQCTRL;       /*!< Sequence Control  */
    uint32_t SEQCTRLB;      /*!< Sequence Control B  */
    uint32_t IEN;           /*!< Interrupt Enable Register  */
    uint32_t SEQ[5];        /*!< Instruction Sequence registers */
    CRYPTO_Data260_TypeDef DDATA[5]; /*!< DDATA registers. Covers all data
                                        registers
                                        of CRYPTO, including DATA(128 bit),
                                        DDATA (256bit/260bit),
                                        QDATA (512bit) registers. */
    uint32_t regmask;       /*!< Bitmask for which registers to save */
    uint32_t operands;      /*!< Saving the currently selected operands */
    bool carry;             /*!< Saving the status of the carry flag */
} crypto_context_t;

static crypto_context_t preemption_context;
static bool             is_preempted = false;
static CORE_DECLARE_IRQ_STATE;
#endif /* CRYPTO_DEVICE_PREEMPTION */

typedef enum
{
#if defined( CRYPTO0 )
    CRYPTO0_ID = 0,
#elif defined( CRYPTO )
    CRYPTO_ID = 0,
#endif
#if defined( CRYPTO1 )
    CRYPTO1_ID = 1,
#endif
} crypto_instance_number_t;

typedef struct {
    CRYPTO_TypeDef *device;
    uint32_t clockMask;
} crypto_device_t;

static const crypto_device_t crypto_devices[CRYPTO_COUNT] =
{
#if defined( CRYPTO0 )
    {
        CRYPTO0,
        _CMU_HFBUSCLKEN0_CRYPTO0_SHIFT
    },
#elif defined( CRYPTO )
    {
        CRYPTO,
        _CMU_HFBUSCLKEN0_CRYPTO_SHIFT
    },
#endif
#if defined( CRYPTO1 )
    {
        CRYPTO1,
        _CMU_HFBUSCLKEN0_CRYPTO1_SHIFT
    },
#endif
};

static inline int crypto_management_index_by_device( CRYPTO_TypeDef *device )
{
#if defined( CRYPTO0 )
    if ( device == CRYPTO0 ) return 0;
#elif defined( CRYPTO )
    if ( device == CRYPTO ) return 0;
#endif
#if defined( CRYPTO1 )
    if ( device == CRYPTO1 ) return 1;
#endif
    return -1;
}

/* Use bitband for clock enable/disable operations, such that they are atomic */
#define CRYPTO_CLOCK_ENABLE(clk)  BUS_RegBitWrite(&(CMU->HFBUSCLKEN0), (clk), 1)
#define CRYPTO_CLOCK_DISABLE(clk) BUS_RegBitWrite(&(CMU->HFBUSCLKEN0), (clk), 0)
#define CRYPTO_CLOCK_ENABLED(clk) BUS_RegBitRead(&(CMU->HFBUSCLKEN0), (clk))

/* Get ownership of an available crypto device */
CRYPTO_TypeDef *crypto_management_acquire( void )
{
    CRYPTO_TypeDef *device = NULL;

#if defined( MBEDTLS_THREADING_C )
    /* Initialize mutexes if that hasn't happened yet */
    CORE_DECLARE_IRQ_STATE;

    if ( !crypto_locks_initialized ) {
        CORE_ENTER_CRITICAL();
        if ( !crypto_locks_initialized ) {
            for ( int i = 0; i < CRYPTO_COUNT; i++ ) {
                mbedtls_mutex_init(&crypto_locks[i]);
            }
            crypto_locks_initialized = true;
        }
        CORE_EXIT_CRITICAL();
    }

/* Wrapping this in SL_THREADING_ALT pending non-blocking mutex in official
 * threading API. */
#if defined( SL_THREADING_ALT )
    /* Try to take an available crypto instance */
    unsigned int devno = 0;
    for ( ; devno < CRYPTO_COUNT; devno++ ) {
        if ( 0 == THREADING_TakeMutexNonBlocking(&crypto_locks[devno]) ) {
            device = crypto_devices[devno].device;
            break;
        }
    }
#endif // SL_THREADING_ALT

    /* If no device immediately available, do naieve round-robin */
    if ( device == NULL ) {
        devno = acquire_count % CRYPTO_COUNT;
        mbedtls_mutex_lock( &crypto_locks[devno] );
        device = crypto_devices[devno].device;
    }

    /* Doing this outside of critical section is safe, since we own the lock
     * and are using bitband to poke the clock enable bit */
    CRYPTO_CLOCK_ENABLE( crypto_devices[devno].clockMask );

    acquire_count++;
#else // !MBEDTLS_THREADING_C
    device = crypto_devices[0].device;
    CRYPTO_CLOCK_ENABLE( crypto_devices[0].clockMask );
#endif // MBEDTLS_THREADING_C

    return device;
}

/* Get ownership of the default crypto device (CRYPTO0/CRYPTO) */
CRYPTO_TypeDef *crypto_management_acquire_default( void )
{
    CRYPTO_TypeDef *device = NULL;

#if defined( MBEDTLS_THREADING_C )
    /* Initialize mutexes if that hasn't happened yet */
    CORE_DECLARE_IRQ_STATE;

    if ( !crypto_locks_initialized ) {
        CORE_ENTER_CRITICAL();
        if ( !crypto_locks_initialized ) {
            for ( int i = 0; i < CRYPTO_COUNT; i++ ) {
                mbedtls_mutex_init(&crypto_locks[i]);
            }
            crypto_locks_initialized = true;
        }
        CORE_EXIT_CRITICAL();
    }

    mbedtls_mutex_lock( &crypto_locks[0] );
    device = crypto_devices[0].device;

    /* Doing this outside of critical section is safe, since we own the lock
     * and are using bitband to poke the clock enable bit */
    CRYPTO_CLOCK_ENABLE( crypto_devices[0].clockMask );
#else // !MBEDTLS_THREADING_C
    device = crypto_devices[0].device;
    CRYPTO_CLOCK_ENABLE( crypto_devices[0].clockMask );
#endif // MBEDTLS_THREADING_C

    return device;
}

/* Release ownership of an available crypto device */
void crypto_management_release( CRYPTO_TypeDef *device )
{
    int devno = crypto_management_index_by_device( device );
    if ( devno < 0 ) {
        return;
    }

    /* Doing this outside of critical section is safe, since we still own the lock
     * and are using bitband to poke the clock enable bit */
    CRYPTO_CLOCK_DISABLE( crypto_devices[devno].clockMask );

#if defined ( MBEDTLS_THREADING_C )
    mbedtls_mutex_unlock( &crypto_locks[devno] );
#endif
}

/* Acquire a device with preemption. NOT thread-safe! */
CRYPTO_TypeDef *crypto_management_acquire_preemption( uint32_t regmask )
{
#if defined( CRYPTO_DEVICE_PREEMPTION )
    CRYPTO_TypeDef *device = NULL;
    /* Turn off interrupts */
    CORE_ENTER_CRITICAL();

    /* Check if there is an unused CRYPTO instance */
    for ( int i = 0; i < CRYPTO_COUNT; i++ ) {
        if ( !CRYPTO_CLOCK_ENABLED( crypto_devices[i].clockMask ) ) {
            /* Found an unused device */
            CRYPTO_CLOCK_ENABLE( crypto_devices[i].clockMask );
            device = crypto_devices[i].device;
            break;
        }
    }

    /* If there is no unused instance, preempt the last one */
    if ( device == NULL ) {
        is_preempted = true;
        device = crypto_devices[CRYPTO_COUNT - 1].device;

        /* In case this instance is still working on anything */
        CRYPTO_InstructionSequenceWait(device);

        /* Store operational context */
        preemption_context.regmask  = regmask;
        preemption_context.WAC      = device->WAC;
        preemption_context.CTRL     = device->CTRL;
        preemption_context.SEQCTRL  = device->SEQCTRL;
        preemption_context.SEQCTRLB = device->SEQCTRLB;
        preemption_context.IEN      = device->IEN;
        preemption_context.operands = device->CSTATUS;
        preemption_context.carry    = (device->DSTATUS & CRYPTO_DSTATUS_CARRY) != 0;

        if ( (preemption_context.WAC & _CRYPTO_WAC_RESULTWIDTH_MASK) == CRYPTO_WAC_RESULTWIDTH_260BIT)
        {
            CRYPTO_DData0Read260(device, preemption_context.DDATA[0]); /* Always save DDATA0 because it'll get clobbered in 260-bit mode*/

            if ( (regmask & CRYPTO_MANAGEMENT_SAVE_DDATA1) != 0 ) {
                device->CMD = CRYPTO_CMD_INSTR_DDATA1TODDATA0; /* Move DDATA1 to DDATA0
                                                                  in order to read. */
                CRYPTO_DData0Read260(device, preemption_context.DDATA[1]);
            }
            if ( (regmask & CRYPTO_MANAGEMENT_SAVE_DDATA2) != 0 ) {
                device->CMD = CRYPTO_CMD_INSTR_DDATA2TODDATA0; /* Move DDATA2 to DDATA0
                                                                  in order to read. */
                CRYPTO_DData0Read260(device, preemption_context.DDATA[2]);
            }
            if ( (regmask & CRYPTO_MANAGEMENT_SAVE_DDATA3) != 0 ) {
                device->CMD = CRYPTO_CMD_INSTR_DDATA3TODDATA0; /* Move DDATA3 to DDATA0
                                                                  in order to read. */
                CRYPTO_DData0Read260(device, preemption_context.DDATA[3]);
            }
            if ( (regmask & CRYPTO_MANAGEMENT_SAVE_DDATA4) != 0 ) {
                device->CMD = CRYPTO_CMD_INSTR_DDATA4TODDATA0; /* Move DDATA4 to DDATA0
                                                                  in order to read. */
                CRYPTO_DData0Read260(device, preemption_context.DDATA[4]);
            }
        }
        else
        {
            if ( (regmask & CRYPTO_MANAGEMENT_SAVE_DDATA0) != 0 )
                CRYPTO_DDataRead(&device->DDATA0, preemption_context.DDATA[0]);
            if ( (regmask & CRYPTO_MANAGEMENT_SAVE_DDATA1) != 0 )
                CRYPTO_DDataRead(&device->DDATA1, preemption_context.DDATA[1]);
            if ( (regmask & CRYPTO_MANAGEMENT_SAVE_DDATA2) != 0 )
                CRYPTO_DDataRead(&device->DDATA2, preemption_context.DDATA[2]);
            if ( (regmask & CRYPTO_MANAGEMENT_SAVE_DDATA3) != 0 )
                CRYPTO_DDataRead(&device->DDATA3, preemption_context.DDATA[3]);
            if ( (regmask & CRYPTO_MANAGEMENT_SAVE_DDATA4) != 0 )
                CRYPTO_DDataRead(&device->DDATA4, preemption_context.DDATA[4]);
        }

        /* Search for possible EXEC commands and replace with END. */
        for ( size_t j = 0; j < (regmask & 0x7U)*sizeof(uint32_t); j++ ) {
            if ( (j & 0x03) == 0 ) {
                preemption_context.SEQ[j / sizeof(uint32_t)] = *((&device->SEQ0) + (j / sizeof(uint32_t)));
            }
            if ( ((uint8_t*)preemption_context.SEQ)[j] == CRYPTO_CMD_INSTR_EXEC ) {
                ((uint8_t*)preemption_context.SEQ)[j] = CRYPTO_CMD_INSTR_END;
            }
        }
    }

    return device;
#else
    (void) regmask;
    return crypto_management_acquire();
#endif
}

/* Release a device from preemption */
void crypto_management_release_preemption( CRYPTO_TypeDef *device )
{
    if ( crypto_management_index_by_device( device ) < 0 ) {
        return;
    }
#if defined( CRYPTO_DEVICE_PREEMPTION )

    if ( is_preempted ) {
        /* If we preempted something, put their context back */
        device->WAC      = preemption_context.WAC;
        device->CTRL     = preemption_context.CTRL;
        device->SEQCTRL  = preemption_context.SEQCTRL;
        device->SEQCTRLB = preemption_context.SEQCTRLB;
        device->IEN      = preemption_context.IEN;

        if ( (preemption_context.WAC & _CRYPTO_WAC_RESULTWIDTH_MASK) == CRYPTO_WAC_RESULTWIDTH_260BIT)
        {
            /* Start by writing the DDATA1 value to DDATA0 and move to DDATA1. */
            if ( (preemption_context.regmask & CRYPTO_MANAGEMENT_SAVE_DDATA1) != 0 ) {
                CRYPTO_DData0Write260(device, preemption_context.DDATA[1]);
                device->CMD = CRYPTO_CMD_INSTR_DDATA0TODDATA1;
            }

            /* Write the DDATA2 value to DDATA0 and move to DDATA2. */
            if ( (preemption_context.regmask & CRYPTO_MANAGEMENT_SAVE_DDATA2) != 0 ) {
                CRYPTO_DData0Write260(device, preemption_context.DDATA[2]);
                device->CMD = CRYPTO_CMD_INSTR_DDATA0TODDATA2;
            }

            /* Write the DDATA3 value to DDATA0 and move to DDATA3. */
            if ( (preemption_context.regmask & CRYPTO_MANAGEMENT_SAVE_DDATA3) != 0 ) {
                CRYPTO_DData0Write260(device, preemption_context.DDATA[3]);
                device->CMD = CRYPTO_CMD_INSTR_DDATA0TODDATA3;
            }

            /* Write the DDATA4 value to DDATA0 and move to DDATA4. */
            if ( (preemption_context.regmask & CRYPTO_MANAGEMENT_SAVE_DDATA4) != 0 ) {
                CRYPTO_DData0Write260(device, preemption_context.DDATA[4]);
                device->CMD = CRYPTO_CMD_INSTR_DDATA0TODDATA4;
            }

            /* Finally write DDATA0 */
            CRYPTO_DData0Write260(device, preemption_context.DDATA[0]);
        }
        else
        {
            if ( (preemption_context.regmask & CRYPTO_MANAGEMENT_SAVE_DDATA0) != 0 )
                CRYPTO_DDataWrite(&device->DDATA0, preemption_context.DDATA[0]);
            if ( (preemption_context.regmask & CRYPTO_MANAGEMENT_SAVE_DDATA1) != 0 )
                CRYPTO_DDataWrite(&device->DDATA1, preemption_context.DDATA[1]);
            if ( (preemption_context.regmask & CRYPTO_MANAGEMENT_SAVE_DDATA2) != 0 )
                CRYPTO_DDataWrite(&device->DDATA2, preemption_context.DDATA[2]);
            if ( (preemption_context.regmask & CRYPTO_MANAGEMENT_SAVE_DDATA3) != 0 )
                CRYPTO_DDataWrite(&device->DDATA3, preemption_context.DDATA[3]);
            if ( (preemption_context.regmask & CRYPTO_MANAGEMENT_SAVE_DDATA4) != 0 )
                CRYPTO_DDataWrite(&device->DDATA4, preemption_context.DDATA[4]);
        }

        if (preemption_context.carry) {
            device->CMD = CRYPTO_CMD_INSTR_CSET;
        } else {
            device->CMD = CRYPTO_CMD_INSTR_CCLR;
        }

        device->CMD = (preemption_context.operands & 0x7U) |
                      (((preemption_context.operands >> 8) & 0x7U) << 3) |
                      0xC0;

        for (size_t i = 0; i < (preemption_context.regmask & 0x7U); i++ ) {
            *((&device->SEQ0) + i) = preemption_context.SEQ[i];
        }

        is_preempted = false;
    } else {
        /* If we didn't preempt anything, turn crypto clock back off */
        CRYPTO_CLOCK_DISABLE( crypto_devices[crypto_management_index_by_device( device )].clockMask );
    }

    /* Turn interrupts back on */
    CORE_EXIT_CRITICAL();
#else
    crypto_management_release(device);
#endif
}

#endif /* CRYPTO_PRESENT */
