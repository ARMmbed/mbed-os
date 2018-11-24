/*
 * Copyright (c) 2018 Arm Limited
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

/**
 * \file rtc_pl031_drv.c
 * \brief Implementation of the PL031 Real Time Clock (RTC) native driver.
 *
 * \note PL031 device specific definitions based on
 *       real_time_clock_pl031_r1p3_technical_reference_manual.pdf
 *       which is available from http://infocenter.arm.com.
 */

#include <stddef.h>
#include "rtc_pl031_drv.h"

/**
 * \brief Structure to access the memory mapped registers of the PL031.
 */
struct rtc_pl031_dev_reg_map_t {
    volatile uint32_t rtcdr;          /*!< Data Register */
    volatile uint32_t rtcmr;          /*!< Match Register */
    volatile uint32_t rtclr;          /*!< Load Register */
    volatile uint32_t rtccr;          /*!< Control Register */
    volatile uint32_t rtcimsc;
                                    /*!< Interrupt Mask Set or Clear Register */
    volatile uint32_t rtcris;         /*!< Raw Interrupt Status Register */
    volatile uint32_t rtcmis;         /*!< Masked Interrupt Status Register */
    volatile uint32_t rtcicr;         /*!< Interrupt Clear Register */
    volatile uint32_t reserved[1008]; /*!< Reserved from Offset 0x20-0xFDC */
    volatile uint32_t rtcperiphid0;   /*!< Peripheral ID0 Register */
    volatile uint32_t rtcperiphid1;   /*!< Peripheral ID1 Register */
    volatile uint32_t rtcperiphid2;   /*!< Peripheral ID2 Register */
    volatile uint32_t rtcperiphid3;   /*!< Peripheral ID3 Register */
    volatile uint32_t rtcpcellid0;    /*!< Primary Cell ID0 Register */
    volatile uint32_t rtcpcellid1;    /*!< Primary Cell ID1 Register */
    volatile uint32_t rtcpcellid2;    /*!< Primary Cell ID2 Register */
    volatile uint32_t rtcpcellid3;    /*!< Primary Cell ID3 Register */
};

/* RTC Control Register */
#define RTC_PL031_RTCCR_ENABLE_POS         0x0U
#define RTC_PL031_RTCCR_ENABLE_MSK         (0x1U << RTC_PL031_RTCCR_ENABLE_POS)

/* RTC Interrupt Mask Set or Clear Register */
#define RTC_PL031_RTCIMSC_SET_CLEAR_POS    0x0U
#define RTC_PL031_RTCIMSC_SET_CLEAR_MSK    (0x1U << \
                                           RTC_PL031_RTCIMSC_SET_CLEAR_POS)

/* RTC RAW Interrupt Status Register */
#define RTC_PL031_RTCRIS_STATUS_POS        0x0U
#define RTC_PL031_RTCRIS_STATUS_MSK        (0x1U << RTC_PL031_RTCRIS_STATUS_POS)

/* RTC Masked Interrupt Status Register */
#define RTC_PL031_RTCMIS_STATUS_POS        0x0U
#define RTC_PL031_RTCMIS_STATUS_MSK        (0x1U << RTC_PL031_RTCMIS_STATUS_POS)

/* RTC Interrupt Clear Register */
#define RTC_PL031_RTCICR_CLEAR_POS         0x0U
#define RTC_PL031_RTCICR_CLEAR_MSK         (0x1U << RTC_PL031_RTCICR_CLEAR_POS)

bool rtc_pl031_init(struct rtc_pl031_dev_t* dev)
{
    struct rtc_pl031_dev_reg_map_t* p_rtc;

    if (dev == NULL) {
        return false;
    }

    p_rtc = (struct rtc_pl031_dev_reg_map_t*) dev->cfg->base;
    p_rtc->rtcmr = 0U;
    p_rtc->rtcicr = RTC_PL031_RTCICR_CLEAR_MSK;
    return true;
}

bool rtc_pl031_dev_enable(struct rtc_pl031_dev_t* dev)
{
    struct rtc_pl031_dev_reg_map_t* p_rtc;

    if (dev == NULL) {
        return false;
    }

    p_rtc = (struct rtc_pl031_dev_reg_map_t*) dev->cfg->base;
    p_rtc->rtccr = RTC_PL031_RTCCR_ENABLE_MSK;
    return true;
}

bool rtc_pl031_dev_disable(struct rtc_pl031_dev_t* dev)
{
    struct rtc_pl031_dev_reg_map_t* p_rtc;

    if (dev == NULL) {
        return false;
    }

    p_rtc = (struct rtc_pl031_dev_reg_map_t*) dev->cfg->base;
    p_rtc->rtccr = 0U;
    return true;
}

bool rtc_pl031_read_current_time(struct rtc_pl031_dev_t* dev, uint32_t *seconds)
{
    struct rtc_pl031_dev_reg_map_t* p_rtc;

    if (dev == NULL || seconds == NULL) {
        return false;
    }

    p_rtc = (struct rtc_pl031_dev_reg_map_t*) dev->cfg->base;
    *seconds = (uint32_t)p_rtc->rtcdr;
    return true;
}

bool rtc_pl031_write_current_time(struct rtc_pl031_dev_t* dev, uint32_t seconds)
{
    struct rtc_pl031_dev_reg_map_t* p_rtc;

    if (dev == NULL) {
        return false;
    }

    p_rtc = (struct rtc_pl031_dev_reg_map_t*) dev->cfg->base;
    p_rtc->rtclr = (uint32_t)seconds;
    return true;
}

bool rtc_pl031_enable_interrupt(struct rtc_pl031_dev_t* dev)
{
    struct rtc_pl031_dev_reg_map_t* p_rtc;

    if (dev == NULL) {
        return false;
    }

    p_rtc = (struct rtc_pl031_dev_reg_map_t*) dev->cfg->base;
    p_rtc->rtcimsc = 0U;
    return true;
}

bool rtc_pl031_disable_interrupt(struct rtc_pl031_dev_t* dev)
{
    struct rtc_pl031_dev_reg_map_t* p_rtc;

    if (dev == NULL) {
        return false;
    }

    p_rtc = (struct rtc_pl031_dev_reg_map_t*) dev->cfg->base;
    p_rtc->rtcimsc = RTC_PL031_RTCIMSC_SET_CLEAR_MSK;
    return true;
}

bool rtc_pl031_is_interrupt_masked(struct rtc_pl031_dev_t* dev)
{
    struct rtc_pl031_dev_reg_map_t* p_rtc =
                              (struct rtc_pl031_dev_reg_map_t*) dev->cfg->base;

    if (p_rtc->rtcimsc & RTC_PL031_RTCIMSC_SET_CLEAR_MSK){
            return true;
    } else {
            return false;
    }
}

bool rtc_pl031_is_raw_interrupt_pending(struct rtc_pl031_dev_t* dev)
{
    struct rtc_pl031_dev_reg_map_t* p_rtc =
                              (struct rtc_pl031_dev_reg_map_t*) dev->cfg->base;

    if (p_rtc->rtcris & RTC_PL031_RTCRIS_STATUS_MSK) {
            return true;
    } else {
            return false;
    }
}

bool rtc_pl031_is_masked_interrupt_pending(struct rtc_pl031_dev_t* dev)
{
    struct rtc_pl031_dev_reg_map_t* p_rtc =
                              (struct rtc_pl031_dev_reg_map_t*) dev->cfg->base;

    if (p_rtc->rtcmis & RTC_PL031_RTCMIS_STATUS_MSK) {
            return true;
    } else {
            return false;
    }
}

bool rtc_pl031_write_match_value(struct rtc_pl031_dev_t* dev, uint32_t seconds)
{
    struct rtc_pl031_dev_reg_map_t* p_rtc;

    if (dev == NULL) {
        return false;
    }

    p_rtc = (struct rtc_pl031_dev_reg_map_t*) dev->cfg->base;
    p_rtc->rtcmr = (uint32_t)seconds;
    return true;
}

bool rtc_pl031_clear_interrupt(struct rtc_pl031_dev_t* dev)
{
    struct rtc_pl031_dev_reg_map_t* p_rtc;

    if (dev == NULL) {
        return false;
    }

    p_rtc = (struct rtc_pl031_dev_reg_map_t*) dev->cfg->base;
    p_rtc->rtcicr = RTC_PL031_RTCICR_CLEAR_MSK;
    return true;
}
