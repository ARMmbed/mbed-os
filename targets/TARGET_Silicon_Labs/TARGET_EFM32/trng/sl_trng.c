/*
 *  True Random Number Generator (TRNG) driver for Silicon Labs devices
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
#include "sl_trng.h"

#if defined(TRNG_PRESENT)
#include "em_cmu.h"
#include "em_common.h"
#include <string.h>

#define FIFO_LEVEL_RETRY   (1000)
#define TEST_WORDS_MIN      (257)

#define TEST_VECTOR_CONDITIONING_KEY_SIZE  (4)
static const uint32_t
test_vector_conditioning_key[TEST_VECTOR_CONDITIONING_KEY_SIZE] =
  {0x16157E2B, 0xA6D2AE28, 0x8815F7AB, 0x3C4FCF09};

#define TEST_VECTOR_CONDITIONING_INPUT_SIZE (16)
static const uint32_t
test_vector_conditioning_input[TEST_VECTOR_CONDITIONING_INPUT_SIZE] =
  {0xE1BCC06B, 0x9199452A, 0x1A7434E1, 0x25199E7F,
   0x578A2DAE, 0x9CAC031E, 0xAC6FB79E, 0x518EAF45,
   0x461CC830, 0x11E45CA3, 0x19C1FBE5, 0xEF520A1A,
   0x45249FF6, 0x179B4FDF, 0x7B412BAD, 0x10376CE6};

#define TEST_VECTOR_CONDITIONING_OUTPUT_SIZE (4)
static const uint32_t
test_vector_conditioning_output[TEST_VECTOR_CONDITIONING_OUTPUT_SIZE] =
  {0xA1CAF13F, 0x09AC1F68, 0x30CA0E12, 0xA7E18675};

#define TRNG_STARTUP_TEST_WAIT_RETRY  (10000)

typedef struct {
  TRNG_TypeDef *instance;
  CMU_Clock_TypeDef clock;
} sl_trng_device_t;

static const sl_trng_device_t sl_trng_devices[TRNG_COUNT] = 
{
#if defined(TRNG0)
{
  TRNG0,
  cmuClock_TRNG0
},
#endif
};

static CMU_Clock_TypeDef sl_trng_get_clock( TRNG_TypeDef *device )
{
  for(int i = 0; i < TRNG_COUNT; i++) {
    if(sl_trng_devices[i].instance == device) {
      return sl_trng_devices[i].clock;
    }
  }
  return cmuClock_TRNG0;
}

void sl_trng_init( TRNG_TypeDef *device )
{
    int i;

    /* Enable the TRNG's clock. */
    CMU_ClockEnable( sl_trng_get_clock(device), true );

    device->CONTROL =
        TRNG_CONTROL_ENABLE |
        TRNG_CONTROL_REPCOUNTIEN |
        TRNG_CONTROL_APT64IEN |
        TRNG_CONTROL_APT4096IEN |
        TRNG_CONTROL_PREIEN |
        TRNG_CONTROL_ALMIEN;

    /* Apply software reset */
    sl_trng_soft_reset(device);

    /* Wait for TRNG to complete startup tests and start filling the FIFO. */
    for (i=0; (device->FIFOLEVEL == 0) && (i<TRNG_STARTUP_TEST_WAIT_RETRY); i++);
    EFM_ASSERT(i<TRNG_STARTUP_TEST_WAIT_RETRY);
}

void sl_trng_free( TRNG_TypeDef *device )
{
    /* Disable TRNG. */
    device->CONTROL = 0;

    /* Disable the TRNG clock. */
    CMU_ClockEnable( sl_trng_get_clock(device), false );
}

void sl_trng_soft_reset( TRNG_TypeDef *device )
{
    uint32_t ctrl = device->CONTROL;

    ctrl |= TRNG_CONTROL_SOFTRESET;

    device->CONTROL = ctrl;

    ctrl &= ~TRNG_CONTROL_SOFTRESET;
    device->CONTROL = ctrl;
}

static void sl_trng_clear_fifo( TRNG_TypeDef *device )
{
  volatile uint32_t val32;

  /* Empty FIFO */
  while ( device->FIFOLEVEL )
  {
      val32 = device->FIFO;
      (void)val32;
  }
}

int sl_trng_set_key( TRNG_TypeDef *device, const unsigned char *key )
{
    uint32_t *_key = (uint32_t*) key;

    sl_trng_clear_fifo(device);

    /* Program key in KEY registers of the TRNG. */
    device->KEY0 = *_key++;
    device->KEY1 = *_key++;
    device->KEY2 = *_key++;
    device->KEY3 = *_key++;

    return 0;
}

static int sl_trng_check_status( TRNG_TypeDef *device )
{
    uint32_t status = device->STATUS;

    if ( (status & (TRNG_STATUS_PREIF
                    | TRNG_STATUS_REPCOUNTIF
                    | TRNG_STATUS_APT64IF
                    | TRNG_STATUS_APT4096IF
                    | TRNG_STATUS_ALMIF)) == 0 )
    {
        /* No errors */
        return 0;
    }

    if ( status & TRNG_STATUS_PREIF )
    {
        /* On a preliminary noise alarm we clear the FIFO and clear
         * the alarm. The preliminary noise alarm is not critical. */
        status &= ~TRNG_STATUS_PREIF;
        device->STATUS = status;
        sl_trng_clear_fifo(device);
        return SL_TRNG_ERR_PRELIMINARY_NOISE_ALARM;
    }
    else
    {
        /* Clear alarm conditions by doing a TRNG soft reset. */
        sl_trng_soft_reset( device );
        if ( status & TRNG_STATUS_REPCOUNTIF )
        {
            return SL_TRNG_ERR_REPETITION_COUNT_TEST_FAILED;
        }
        if ( status & TRNG_STATUS_APT64IF )
        {
            return SL_TRNG_ERR_ADAPTIVE_PROPORTION_TEST_64_FAILED;
        }
        if ( status & TRNG_STATUS_APT4096IF )
        {
            return SL_TRNG_ERR_ADAPTIVE_PROPORTION_TEST_4096_FAILED;
        }
        if ( status & TRNG_STATUS_ALMIF )
        {
            return SL_TRNG_ERR_NOISE_ALARM;
        }
    }

    return 0;
}

static void sl_trng_read_chunk( TRNG_TypeDef *device,
                                unsigned char *output,
                                size_t len )
{
    uint32_t * out32 = (uint32_t *) output;
    uint32_t tmp;

    /* Read known good available data. */
    while ( len >= 4)
    {
        *out32++ = device->FIFO;
        len -= 4;
    }

    /* Handle the case where len is not a multiple of 4. */
    if ( len < 4 )
    {
        tmp = device->FIFO;
        memcpy((uint8_t *)out32, (const uint8_t *) &tmp, len);
    }
}

int sl_trng_poll( TRNG_TypeDef *device,
                  unsigned char *output,
                  size_t len,
                  size_t *olen )
{
    size_t output_len = 0;
    size_t chunk_len = 0;
    size_t available;
    int ret = 0;

    while (output_len < len)
    {
        available = device->FIFOLEVEL * 4;
        if (available == 0)
        {
            break;
        }

#if !defined(SL_TRNG_IGNORE_ALL_ALARMS)
        /* Check status for current data in FIFO
         * and handle any error conditions. */
        ret = sl_trng_check_status( device );
#if defined(SL_TRNG_IGNORE_NOISE_ALARMS)
        /* Ignore noise alarms by returning 0 (OK) if they occur and
         * keeping the already generated random data. */
        if ( (ret == SL_TRNG_ERR_PRELIMINARY_NOISE_ALARM) ||
             (ret == SL_TRNG_ERR_NOISE_ALARM) )
        {
            ret = 0;
            continue;
        }
#else
        /* Noise alarms trigger a FIFO clearing, and we need to throw
         * away the collected entropy. */
        if ( (ret == SL_TRNG_ERR_PRELIMINARY_NOISE_ALARM) ||
             (ret == SL_TRNG_ERR_NOISE_ALARM) )
        {
            ret = 0;
            output_len = 0;
            continue;
        }
#endif
        /* Alarm has been signaled so we throw the generated data away. */
        if (ret != 0)
        {
            output_len = 0;
            break;
        }
#endif

        chunk_len = SL_MIN(len - output_len, available);
        sl_trng_read_chunk(device, output + output_len, chunk_len);
        output_len += chunk_len;
    }

    *olen = output_len;
    return ret;
}

#endif /* TRNG_PRESENT */
