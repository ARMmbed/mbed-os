/**
 * \file
 *
 * \brief Real-Time Clock (RTC) driver for SAM.
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "rtc.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

    /**
     * \defgroup sam_drivers_rtc_group Real-Time Clock (RTC)
     *
     * See \ref sam_rtc_quickstart.
     *
     * The RTC provides a full binary-coded decimal (BCD) clock that includes
     * century (19/20), year (with leap years), month, date, day, hour, minute
     * and second.
     *
     * @{
     */

    /* RTC Write Protect Key "RTC" in ASCII */
#define RTC_WP_KEY     (0x525443)

    /* The BCD code shift value */
#define BCD_SHIFT      4

    /* The BCD code mask value */
#define BCD_MASK       0xfu

    /* The BCD mul/div factor value */
#define BCD_FACTOR     10

    /**
     * \brief Set the RTC hour mode.
     *
     * \param p_rtc Pointer to an RTC instance.
     * \param ul_mode 1 for 12-hour mode, 0 for 24-hour mode.
     */
    void rtc_set_hour_mode(Rtc *p_rtc, uint32_t ul_mode)
{
    if (ul_mode) {
        p_rtc->RTC_MR |= RTC_MR_HRMOD;
    } else {
        p_rtc->RTC_MR &= (~RTC_MR_HRMOD);
    }
}

/**
 * \brief Get the RTC hour mode.
 *
 * \param p_rtc Pointer to an RTC instance.
 *
 * \return 1 for 12-hour mode, 0 for 24-hour mode.
 */
uint32_t rtc_get_hour_mode(Rtc *p_rtc)
{
    uint32_t ul_temp = p_rtc->RTC_MR;

    if (ul_temp & RTC_MR_HRMOD) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * \brief Enable RTC interrupts.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param ul_sources Interrupts to be enabled.
 */
void rtc_enable_interrupt(Rtc *p_rtc, uint32_t ul_sources)
{
    p_rtc->RTC_IER = ul_sources;
}

/**
 * \brief Disable RTC interrupts.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param ul_sources Interrupts to be disabled.
 */
void rtc_disable_interrupt(Rtc *p_rtc, uint32_t ul_sources)
{
    p_rtc->RTC_IDR = ul_sources;
}

/**
 * \brief Read RTC interrupt mask.
 *
 * \param p_rtc Pointer to an RTC instance.
 *
 * \return The interrupt mask value.
 */
uint32_t rtc_get_interrupt_mask(Rtc *p_rtc)
{
    return p_rtc->RTC_IMR;
}

/**
 * \brief Get the RTC time value.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param pul_hour Current hour, 24-hour mode.
 * \param pul_minute Current minute.
 * \param pul_second Current second.
 */
void rtc_get_time(Rtc *p_rtc, uint32_t *pul_hour, uint32_t *pul_minute,
                  uint32_t *pul_second)
{
    uint32_t ul_time;
    uint32_t ul_temp;

    /* Get the current RTC time (multiple reads are necessary to insure a stable value). */
    ul_time = p_rtc->RTC_TIMR;
    while (ul_time != p_rtc->RTC_TIMR) {
        ul_time = p_rtc->RTC_TIMR;
    }

    /* Hour */
    if (pul_hour) {
        ul_temp = (ul_time & RTC_TIMR_HOUR_Msk) >> RTC_TIMR_HOUR_Pos;
        *pul_hour = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);

        if ((ul_time & RTC_TIMR_AMPM) == RTC_TIMR_AMPM) {
            *pul_hour += 12;
        }
    }

    /* Minute */
    if (pul_minute) {
        ul_temp = (ul_time & RTC_TIMR_MIN_Msk) >> RTC_TIMR_MIN_Pos;
        *pul_minute = (ul_temp >> BCD_SHIFT) * BCD_FACTOR +  (ul_temp & BCD_MASK);
    }

    /* Second */
    if (pul_second) {
        ul_temp = (ul_time & RTC_TIMR_SEC_Msk) >> RTC_TIMR_SEC_Pos;
        *pul_second = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);
    }
}

/**
 * \brief Set the RTC time value.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param ul_hour Current hour, 24-hour mode.
 * \param ul_minute Current minute.
 * \param ul_second Current second.
 *
 * \return 0 for OK, else invalid setting.
 */
uint32_t rtc_set_time(Rtc *p_rtc, uint32_t ul_hour, uint32_t ul_minute,
                      uint32_t ul_second)
{
    uint32_t ul_time = 0;

    /* If 12-hour mode, set AMPM bit */
    if ((p_rtc->RTC_MR & RTC_MR_HRMOD) == RTC_MR_HRMOD) {
        if (ul_hour > 12) {
            ul_hour -= 12;
            ul_time |= RTC_TIMR_AMPM;
        }
    }

    /* Hour */
    ul_time |= ((ul_hour / BCD_FACTOR) << (RTC_TIMR_HOUR_Pos + BCD_SHIFT)) |
               ((ul_hour % BCD_FACTOR) << RTC_TIMR_HOUR_Pos);

    /* Minute */
    ul_time |= ((ul_minute / BCD_FACTOR) << (RTC_TIMR_MIN_Pos + BCD_SHIFT)) |
               ((ul_minute % BCD_FACTOR) << RTC_TIMR_MIN_Pos);

    /* Second */
    ul_time |= ((ul_second / BCD_FACTOR) << (RTC_TIMR_SEC_Pos + BCD_SHIFT)) |
               ((ul_second % BCD_FACTOR) << RTC_TIMR_SEC_Pos);

    /* Update time register. Check the spec for the flow. */
    p_rtc->RTC_CR |= RTC_CR_UPDTIM;
    while ((p_rtc->RTC_SR & RTC_SR_ACKUPD) != RTC_SR_ACKUPD);
    p_rtc->RTC_SCCR = RTC_SCCR_ACKCLR;
    p_rtc->RTC_TIMR = ul_time;
    p_rtc->RTC_CR &= (~RTC_CR_UPDTIM);
    p_rtc->RTC_SCCR |= RTC_SCCR_SECCLR;

    return (p_rtc->RTC_VER & RTC_VER_NVTIM);
}

/**
 * \brief Set the RTC alarm time value.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param ul_hour_flag 1 for setting, 0 for not setting.
 * \param ul_hour Alarm hour value, 24-hour mode.
 * \param ul_minute_flag 1 for setting, 0 for not setting.
 * \param ul_minute Alarm minute value.
 * \param ul_second_flag 1 for setting, 0 for not setting.
 * \param ul_second Alarm second value.
 *
 * \return 0 for OK, else invalid setting.
 */
uint32_t rtc_set_time_alarm(Rtc *p_rtc,
                            uint32_t ul_hour_flag, uint32_t ul_hour,
                            uint32_t ul_minute_flag, uint32_t ul_minute,
                            uint32_t ul_second_flag, uint32_t ul_second)
{
    uint32_t ul_alarm = 0;

    /* Hour alarm setting */
    if (ul_hour_flag) {
        /* If 12-hour mode, set AMPM bit */
        if ((p_rtc->RTC_MR & RTC_MR_HRMOD) == RTC_MR_HRMOD) {
            if (ul_hour > 12) {
                ul_hour -= 12;
                ul_alarm |= RTC_TIMR_AMPM;
            }
        }

        ul_alarm |= ((ul_hour / BCD_FACTOR) << (RTC_TIMR_HOUR_Pos + BCD_SHIFT)) |
                    ((ul_hour % BCD_FACTOR) << RTC_TIMR_HOUR_Pos);
    }

    /* Minute alarm setting */
    if (ul_minute_flag) {
        ul_alarm |= ((ul_minute / BCD_FACTOR) << (RTC_TIMR_MIN_Pos + BCD_SHIFT)) |
                    ((ul_minute % BCD_FACTOR) << RTC_TIMR_MIN_Pos);
    }

    /* Second alarm setting */
    if (ul_second_flag) {
        ul_alarm |= ((ul_second / BCD_FACTOR) << (RTC_TIMR_SEC_Pos + BCD_SHIFT)) |
                    ((ul_second % BCD_FACTOR) << RTC_TIMR_SEC_Pos);
    }

    p_rtc->RTC_TIMALR &= ~(RTC_TIMALR_SECEN | RTC_TIMALR_MINEN | RTC_TIMALR_HOUREN);
    p_rtc->RTC_TIMALR = ul_alarm;
    p_rtc->RTC_TIMALR |= (RTC_TIMALR_SECEN | RTC_TIMALR_MINEN | RTC_TIMALR_HOUREN);

    return (p_rtc->RTC_VER & RTC_VER_NVTIMALR);
}

/**
 * \brief Get the RTC date value.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param pul_year Current year.
 * \param pul_month Current month.
 * \param pul_day Current day.
 * \param pul_week Current day in current week.
 */
void rtc_get_date(Rtc *p_rtc, uint32_t *pul_year, uint32_t *pul_month,
                  uint32_t *pul_day, uint32_t *pul_week)
{
    uint32_t ul_date;
    uint32_t ul_cent;
    uint32_t ul_temp;

    /* Get the current date (multiple reads are necessary to insure a stable value). */
    ul_date = p_rtc->RTC_CALR;
    while (ul_date != p_rtc->RTC_CALR) {
        ul_date = p_rtc->RTC_CALR;
    }

    /* Retrieve year */
    if (pul_year) {
        ul_temp = (ul_date & RTC_CALR_CENT_Msk) >> RTC_CALR_CENT_Pos;
        ul_cent = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);
        ul_temp = (ul_date & RTC_CALR_YEAR_Msk) >> RTC_CALR_YEAR_Pos;
        *pul_year = (ul_cent * BCD_FACTOR * BCD_FACTOR) +
                    (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);
    }

    /* Retrieve month */
    if (pul_month) {
        ul_temp = (ul_date & RTC_CALR_MONTH_Msk) >> RTC_CALR_MONTH_Pos;
        *pul_month = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);
    }

    /* Retrieve day */
    if (pul_day) {
        ul_temp = (ul_date & RTC_CALR_DATE_Msk) >> RTC_CALR_DATE_Pos;
        *pul_day = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);
    }

    /* Retrieve week */
    if (pul_week) {
        *pul_week = ((ul_date & RTC_CALR_DAY_Msk) >> RTC_CALR_DAY_Pos);
    }
}

/**
 * \brief Set the RTC date.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param ul_year Current year.
 * \param ul_month Current month.
 * \param ul_day Current day.
 * \param ul_week Current day in current week.
 *
 * \return 0 for OK, else invalid setting.
 */
uint32_t rtc_set_date(Rtc *p_rtc, uint32_t ul_year, uint32_t ul_month,
                      uint32_t ul_day, uint32_t ul_week)
{
    uint32_t ul_date = 0;

    /* Cent */
    ul_date |= ((ul_year / BCD_FACTOR / BCD_FACTOR / BCD_FACTOR) <<
                (RTC_CALR_CENT_Pos + BCD_SHIFT) |
                ((ul_year / BCD_FACTOR / BCD_FACTOR) % BCD_FACTOR) <<  RTC_CALR_CENT_Pos);

    /* Year */
    ul_date |= (((ul_year / BCD_FACTOR) % BCD_FACTOR) <<
                (RTC_CALR_YEAR_Pos + BCD_SHIFT)) |
               ((ul_year % BCD_FACTOR) << RTC_CALR_YEAR_Pos);

    /* Month */
    ul_date |= ((ul_month / BCD_FACTOR) << (RTC_CALR_MONTH_Pos + BCD_SHIFT)) |
               ((ul_month % BCD_FACTOR) << RTC_CALR_MONTH_Pos);

    /* Week */
    ul_date |= (ul_week << RTC_CALR_DAY_Pos);

    /* Day */
    ul_date |= ((ul_day / BCD_FACTOR) << (RTC_CALR_DATE_Pos + BCD_SHIFT)) |
               ((ul_day % BCD_FACTOR) << RTC_CALR_DATE_Pos);

    /* Update calendar register. Check the spec for the flow. */
    p_rtc->RTC_CR |= RTC_CR_UPDCAL;
    while ((p_rtc->RTC_SR & RTC_SR_ACKUPD) != RTC_SR_ACKUPD);

    p_rtc->RTC_SCCR = RTC_SCCR_ACKCLR;
    p_rtc->RTC_CALR = ul_date;
    p_rtc->RTC_CR &= (~RTC_CR_UPDCAL);
    /* Clear SECENV in SCCR */
    p_rtc->RTC_SCCR |= RTC_SCCR_SECCLR;

    return (p_rtc->RTC_VER & RTC_VER_NVCAL);
}

/**
 * \brief Set the RTC alarm date value.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param ul_month_flag 1 for setting, 0 for not setting.
 * \param ul_month Alarm month value.
 * \param ul_day_flag 1 for setting, 0 for not setting.
 * \param ul_day Alarm day value.
 *
 * \return 0 for OK, else invalid setting.
 */
uint32_t rtc_set_date_alarm(Rtc *p_rtc,
                            uint32_t ul_month_flag, uint32_t ul_month,
                            uint32_t ul_day_flag, uint32_t ul_day)
{
    uint32_t ul_alarm = 0;

    /* Month alarm setting */
    if (ul_month_flag) {
        ul_alarm |= ((ul_month / BCD_FACTOR) << (RTC_CALR_MONTH_Pos + BCD_SHIFT)) |
                    ((ul_month % BCD_FACTOR) << RTC_CALR_MONTH_Pos);
    }

    /* Day alarm setting */
    if (ul_day_flag) {
        ul_alarm |= ((ul_day / BCD_FACTOR) << (RTC_CALR_DATE_Pos + BCD_SHIFT)) |
                    ((ul_day % BCD_FACTOR) << RTC_CALR_DATE_Pos);
    }

    /* Set alarm */
    p_rtc->RTC_CALALR &= ~(RTC_CALALR_MTHEN | RTC_CALALR_DATEEN);
    p_rtc->RTC_CALALR = ul_alarm;
    p_rtc->RTC_CALALR |= (RTC_CALALR_MTHEN | RTC_CALALR_DATEEN);

    return (p_rtc->RTC_VER & RTC_VER_NVCALALR);
}

/**
 * \brief Clear the RTC time alarm setting.
 *
 * \param p_rtc Pointer to an RTC instance.
 */
void rtc_clear_time_alarm(Rtc *p_rtc)
{
    p_rtc->RTC_TIMALR = 0;
}

/**
 * \brief Clear the RTC date alarm setting.
 *
 * \param p_rtc Pointer to an RTC instance.
 */
void rtc_clear_date_alarm(Rtc *p_rtc)
{
    /* Need a valid value without enabling */
    p_rtc->RTC_CALALR = RTC_CALALR_MONTH(0x01) | RTC_CALALR_DATE(0x01);
}

/**
 * \brief Get the RTC status.
 *
 * \param p_rtc Pointer to an RTC instance.
 *
 * \return Status of the RTC.
 */
uint32_t rtc_get_status(Rtc *p_rtc)
{
    return (p_rtc->RTC_SR);
}

/**
 * \brief Set the RTC SCCR to clear status bits.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param ul_clear Some flag bits which will be cleared.
 */
void rtc_clear_status(Rtc *p_rtc, uint32_t ul_clear)
{
    p_rtc->RTC_SCCR = ul_clear;
}

/**
 * \brief Get the RTC valid entry.
 *
 * \param p_rtc Pointer to an RTC instance.
 *
 * \return 0 for no invalid data, else has contained invalid data.
 */
uint32_t rtc_get_valid_entry(Rtc *p_rtc)
{
    return (p_rtc->RTC_VER);
}

/**
 * \brief Set the RTC time event selection.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param ul_selection Time event selection to be enabled.
 */
void rtc_set_time_event(Rtc *p_rtc, uint32_t ul_selection)
{
    p_rtc->RTC_CR &= ~RTC_CR_TIMEVSEL_Msk;
    p_rtc->RTC_CR |= (ul_selection << RTC_CR_TIMEVSEL_Pos) & RTC_CR_TIMEVSEL_Msk;
}

/**
 * \brief Set the RTC calendar event selection.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param ul_selection Calendar event selection to be enabled..
 */
void rtc_set_calendar_event(Rtc *p_rtc, uint32_t ul_selection)
{
    p_rtc->RTC_CR &= ~RTC_CR_CALEVSEL_Msk;
    p_rtc->RTC_CR |= (ul_selection << RTC_CR_CALEVSEL_Pos) & RTC_CR_CALEVSEL_Msk;
}

#if ((SAM3S8) || (SAM3SD8) || (SAM4S) || (SAM4N) || (SAM4C) || (SAMG) || (SAM4CP) || (SAM4CM))
/**
 * \brief Set the RTC calendar mode.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param ul_mode 1 for Persian mode,0 for Gregorian mode.
 */
void rtc_set_calendar_mode(Rtc *p_rtc, uint32_t ul_mode)
{
    if (ul_mode) {
        p_rtc->RTC_MR |= RTC_MR_PERSIAN;
    } else {
        p_rtc->RTC_MR &= (~RTC_MR_PERSIAN);
    }
}

/**
 * \brief Get the RTC calendar mode.
 *
 * \param p_rtc Pointer to an RTC instance.
 *
 * \return 1 for Persian calendar, 0 for Gregorian calendar.
 */
uint32_t rtc_get_calendar_mode(Rtc *p_rtc)
{
    uint32_t ul_temp = p_rtc->RTC_MR;

    if (ul_temp & RTC_MR_PERSIAN) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * \brief Set the RTC calibration.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param ul_direction_ppm Positive/negative correction.
 * \param ul_correction Correction value.
 * \param ul_range_ppm Low/high range correction.
 */
void rtc_set_calibration(Rtc *p_rtc, uint32_t ul_direction_ppm,
                         uint32_t ul_correction, uint32_t ul_range_ppm)
{
    uint32_t ul_temp;

    ul_temp = p_rtc->RTC_MR;

    if (ul_direction_ppm) {
        ul_temp |= RTC_MR_NEGPPM;
    } else {
        ul_temp &= (~RTC_MR_NEGPPM);
    }

    ul_temp |= RTC_MR_CORRECTION(ul_correction);

    if (ul_range_ppm) {
        ul_temp |= RTC_MR_HIGHPPM;
    } else {
        ul_temp &= (~RTC_MR_HIGHPPM);
    }

    p_rtc->RTC_MR = ul_temp;
}
#endif

#if ((SAM3S8) || (SAM3SD8) || (SAM4S) || (SAM4C) || (SAMG) || (SAM4CP) || (SAM4CM) || SAMV71 || SAMV70 || SAME70 || SAMS70)
/**
 * \brief Set the RTC output waveform.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param ul_channel Output channel selection.
 * \param ul_value Output source selection value.
 */
void rtc_set_waveform(Rtc *p_rtc, uint32_t ul_channel, uint32_t ul_value)
{
    if (ul_channel == 0) {
        switch (ul_value) {
            case 0:
                p_rtc->RTC_MR &= ~RTC_MR_OUT0_Msk;
                p_rtc->RTC_MR |= RTC_MR_OUT0_NO_WAVE;
                break;

            case 1:
                p_rtc->RTC_MR &= ~RTC_MR_OUT0_Msk;
                p_rtc->RTC_MR |= RTC_MR_OUT0_FREQ1HZ;
                break;

            case 2:
                p_rtc->RTC_MR &= ~RTC_MR_OUT0_Msk;
                p_rtc->RTC_MR |= RTC_MR_OUT0_FREQ32HZ;
                break;

            case 3:
                p_rtc->RTC_MR &= ~RTC_MR_OUT0_Msk;
                p_rtc->RTC_MR |= RTC_MR_OUT0_FREQ64HZ;
                break;

            case 4:
                p_rtc->RTC_MR &= ~RTC_MR_OUT0_Msk;
                p_rtc->RTC_MR |= RTC_MR_OUT0_FREQ512HZ;
                break;

#if (!SAMG)
            case 5:
                p_rtc->RTC_MR &= ~RTC_MR_OUT0_Msk;
                p_rtc->RTC_MR |= RTC_MR_OUT0_ALARM_TOGGLE;
                break;
#endif

            case 6:
                p_rtc->RTC_MR &= ~RTC_MR_OUT0_Msk;
                p_rtc->RTC_MR |= RTC_MR_OUT0_ALARM_FLAG;
                break;

#if (!SAMG)
            case 7:
                p_rtc->RTC_MR &= ~RTC_MR_OUT0_Msk;
                p_rtc->RTC_MR |= RTC_MR_OUT0_PROG_PULSE;
                break;
#endif

            default:
                break;
        }
    } else {
#if (!SAM4C && !SAM4CP && !SAM4CM)
        switch (ul_value) {
            case 0:
                p_rtc->RTC_MR &= ~RTC_MR_OUT1_Msk;
                p_rtc->RTC_MR |= RTC_MR_OUT1_NO_WAVE;
                break;

            case 1:
                p_rtc->RTC_MR &= ~RTC_MR_OUT1_Msk;
                p_rtc->RTC_MR |= RTC_MR_OUT1_FREQ1HZ;
                break;

            case 2:
                p_rtc->RTC_MR &= ~RTC_MR_OUT1_Msk;
                p_rtc->RTC_MR |= RTC_MR_OUT1_FREQ32HZ;
                break;

            case 3:
                p_rtc->RTC_MR &= ~RTC_MR_OUT1_Msk;
                p_rtc->RTC_MR |= RTC_MR_OUT1_FREQ64HZ;
                break;

            case 4:
                p_rtc->RTC_MR &= ~RTC_MR_OUT1_Msk;
                p_rtc->RTC_MR |= RTC_MR_OUT1_FREQ512HZ;
                break;

#if (!SAMG)
            case 5:
                p_rtc->RTC_MR &= ~RTC_MR_OUT1_Msk;
                p_rtc->RTC_MR |= RTC_MR_OUT1_ALARM_TOGGLE;
                break;
#endif

            case 6:
                p_rtc->RTC_MR &= ~RTC_MR_OUT1_Msk;
                p_rtc->RTC_MR |= RTC_MR_OUT1_ALARM_FLAG;
                break;

#if (!SAMG)
            case 7:
                p_rtc->RTC_MR &= ~RTC_MR_OUT1_Msk;
                p_rtc->RTC_MR |= RTC_MR_OUT1_PROG_PULSE;
                break;
#endif

            default:
                break;
        }
#endif
    }
}

#if ((SAM3S8) || (SAM3SD8) || (SAM4S) || (SAM4C) || SAMV71 || SAMV70 || SAME70 || SAMS70)
/**
 * \brief Set the pulse output waveform parameters.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param ul_time_high High duration of the output pulse.
 * \param ul_period Period of the output pulse.
 */
void rtc_set_pulse_parameter(Rtc *p_rtc, uint32_t ul_time_high,
                             uint32_t ul_period)
{
    uint32_t ul_temp;

    ul_temp = p_rtc->RTC_MR;

    ul_temp |= (RTC_MR_THIGH_Msk & ((ul_time_high) << RTC_MR_THIGH_Pos));
    ul_temp |= (RTC_MR_TPERIOD_Msk & ((ul_period) << RTC_MR_TPERIOD_Pos));

    p_rtc->RTC_MR = ul_temp;
}
#endif
#endif


#if ((SAM3N) || (SAM3U) || (SAM3XA))
/**
 * \brief Enable or disable write protection of RTC registers.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param ul_enable 1 to enable, 0 to disable.
 */
void rtc_set_writeprotect(Rtc *p_rtc, uint32_t ul_enable)
{
    if (ul_enable) {
        p_rtc->RTC_WPMR = RTC_WPMR_WPKEY(RTC_WP_KEY) | RTC_WPMR_WPEN;
    } else {
        p_rtc->RTC_WPMR = RTC_WPMR_WPKEY(RTC_WP_KEY);
    }
}
#endif /* ((SAM3N) || (SAM3U) || (SAM3XA)) */

#if SAM4C || SAM4CP || SAM4CM
/**
 * \brief Get the RTC tamper time value.
 *
 * \note This function should be called before rtc_get_tamper_source()
 *       function call, Otherwise the tamper time will be cleared.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param pul_hour Current hour, 24-hour mode.
 * \param pul_minute Current minute.
 * \param pul_second Current second.
 * \param reg_num Current tamper register set number.
 */
void rtc_get_tamper_time(Rtc *p_rtc, uint32_t *pul_hour, uint32_t *pul_minute,
                         uint32_t *pul_second, uint8_t reg_num)
{
    uint32_t ul_time;
    uint32_t ul_temp;

    /* Get the current RTC time (multiple reads are to insure a stable value). */
    ul_time = p_rtc->RTC_TS[reg_num].RTC_TSTR;
    while (ul_time != p_rtc->RTC_TS[reg_num].RTC_TSTR) {
        ul_time = p_rtc->RTC_TS[reg_num].RTC_TSTR;
    }

    /* Hour */
    if (pul_hour) {
        ul_temp = (ul_time & RTC_TSTR_HOUR_Msk) >> RTC_TSTR_HOUR_Pos;
        *pul_hour = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);

        if ((ul_time & RTC_TSTR_AMPM) == RTC_TSTR_AMPM) {
            *pul_hour += 12;
        }
    }

    /* Minute */
    if (pul_minute) {
        ul_temp = (ul_time & RTC_TSTR_MIN_Msk) >> RTC_TSTR_MIN_Pos;
        *pul_minute = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);
    }

    /* Second */
    if (pul_second) {
        ul_temp = (ul_time & RTC_TSTR_SEC_Msk) >> RTC_TSTR_SEC_Pos;
        *pul_second = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);
    }
}

/**
 * \brief Get the RTC tamper date.
 *
 * \note This function should be called before rtc_get_tamper_source()
 *       function call, Otherwise the tamper date will be cleared.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param pul_year Current year.
 * \param pul_month Current month.
 * \param pul_day Current day.
 * \param pul_week Current day in current week.
 * \param reg_num Current tamper register set number.
 */
void rtc_get_tamper_date(Rtc *p_rtc, uint32_t *pul_year, uint32_t *pul_month,
                         uint32_t *pul_day, uint32_t *pul_week, uint8_t reg_num)
{
    uint32_t ul_date;
    uint32_t ul_cent;
    uint32_t ul_temp;

    /* Get the current date (multiple reads are to insure a stable value). */
    ul_date = p_rtc->RTC_TS[reg_num].RTC_TSDR;
    while (ul_date != p_rtc->RTC_TS[reg_num].RTC_TSDR) {
        ul_date = p_rtc->RTC_TS[reg_num].RTC_TSDR;
    }

    /* Retrieve year */
    if (pul_year) {
        ul_temp = (ul_date & RTC_TSDR_CENT_Msk) >> RTC_TSDR_CENT_Pos;
        ul_cent = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);
        ul_temp = (ul_date & RTC_TSDR_YEAR_Msk) >> RTC_TSDR_YEAR_Pos;
        *pul_year = (ul_cent * BCD_FACTOR * BCD_FACTOR) +
                    (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);
    }

    /* Retrieve month */
    if (pul_month) {
        ul_temp = (ul_date & RTC_TSDR_MONTH_Msk) >> RTC_TSDR_MONTH_Pos;
        *pul_month = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);
    }

    /* Retrieve day */
    if (pul_day) {
        ul_temp = (ul_date & RTC_TSDR_DATE_Msk) >> RTC_TSDR_DATE_Pos;
        *pul_day = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);
    }

    /* Retrieve week */
    if (pul_week) {
        *pul_week = ((ul_date & RTC_TSDR_DAY_Msk) >> RTC_TSDR_DAY_Pos);
    }
}

/**
 * \brief Get the RTC tamper source.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param reg_num Current tamper register set number.
 *
 * \return Tamper source.
 */
uint32_t rtc_get_tamper_source(Rtc *p_rtc, uint8_t reg_num)
{
    return (p_rtc->RTC_TS[reg_num].RTC_TSSR & RTC_TSSR_TSRC_Msk) >>
           RTC_TSSR_TSRC_Pos;
}

/**
 * \brief Get the RTC tamper event counter.
 *
 * \note This function should be called before rtc_get_tamper_source()
 *       function call, Otherwise the tamper event counter will be cleared.
 *
 * \param p_rtc Pointer to an RTC instance.
 *
 * \return Tamper event counter
 */
uint32_t rtc_get_tamper_event_counter(Rtc *p_rtc)
{
    return (p_rtc->RTC_TS[0].RTC_TSTR & RTC_TSTR_TEVCNT_Msk) >>
           RTC_TSTR_TEVCNT_Pos;
}

/**
 * \brief Check the system is in backup mode when RTC tamper event happen.
 *
 * \note This function should be called before rtc_get_tamper_source()
 *       function call, Otherwise the flag indicates tamper occur in backup
 *       mode will be cleared.
 *
 * \param p_rtc Pointer to an RTC instance.
 * \param reg_num Current tamper register set number.
 *
 * \return True - The system is in backup mode when the tamper event occurs.
 *         Flase - The system is different from backup mode.
 */
bool rtc_is_tamper_occur_in_backup_mode(Rtc *p_rtc, uint8_t reg_num)
{
    if(p_rtc->RTC_TS[reg_num].RTC_TSTR & RTC_TSTR_BACKUP) {
        return true;
    } else {
        return false;
    }
}
#endif

#if (SAMG55)
/**
 * \brief Get the RTC milliseconds value.
 *
 * \param p_rtc Pointer to an RTC instance.
 *
 * \return Number of 1/1024 seconds elapsed within one second.
 */
uint32_t rtc_get_milliseconds(Rtc *p_rtc)
{
    return (p_rtc->RTC_MSR) & RTC_MSR_MS_Msk;
}
#endif


//@}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
