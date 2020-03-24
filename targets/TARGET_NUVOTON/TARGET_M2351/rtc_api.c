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

#include "rtc_api.h"

#if DEVICE_RTC

#include "mbed_wait_api.h"
#include "mbed_error.h"
#include "nu_modutil.h"
#include "nu_miscutil.h"
#include "mbed_mktime.h"
#include "partition_M2351.h"
#include "hal_secure.h"
#if defined(DOMAIN_NS) && (DOMAIN_NS == 1L) && (TFM_LVL > 0)
#include "tfm_ns_lock.h"
#endif

/* Secure attribution of RTC
 *
 * We need RTC to be secure for security concern.
 *
 * On M2351, configured to secure
 * On M2354, hard-wired to secure
 */
#if defined(SCU_INIT_PNSSET2_VAL) && (SCU_INIT_PNSSET2_VAL & (1 << 1))
#error("Limited by BSP/RTC, we can only support secure RTC.")
#endif

void rtc_init(void)
{
    rtc_init_s();
}

void rtc_free(void)
{
    rtc_free_s();
}

int rtc_isenabled(void)
{
    return rtc_isenabled_s();
}

time_t rtc_read(void)
{
    return rtc_read_s();
}

void rtc_write(time_t t)
{
    rtc_write_s(t);
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)

/* Micro seconds per second */
#define NU_US_PER_SEC               1000000
/* Timer clock per second
 *
 * NOTE: This dependents on real hardware.
 */
#define NU_RTCCLK_PER_SEC           (__LXT)

/* Strategy for implementation of RTC HAL
 *
 * H/W RTC just supports year range 2000~2099, which cannot fully cover POSIX time (starting since 2970)
 * and date time of struct TM (starting since 1900).
 *
 * To conquer the difficulty, we don't use H/W RTC to keep real date time. Instead, we use it to keep
 * elapsed time in seconds since one reference time point. The strategy would be:
 *
 * 1. Choose DATETIME_HWRTC_ORIGIN (00:00:00 UTC, Saturday, 1 January 2000) as reference time point of H/W RTC.
 * 2. t_hwrtc_origin = DATETIME_HWRTC_ORIGIN in POSIX time
 * 3. t_hwrtc_elapsed = t_hwrtc_origin + elapsed time since t_hwrtc_origin
 * 4. t_write = POSIX time set by rtc_write().
 * 5. t_present = rtc_read() = t_write + (t_hwrtc_elapsed - t_hwrtc_origin)
 *
 * 1900
 * |---------------------------------------------------------------------------------|
 *           1970    t_write           t_present
 * |---------|-------|-----------------|---------------------------------------------|
 * 
 * 2000 
 * |-----------------|---------------------------------------------------------------|
 * t_hwrtc_origin    t_hwrtc_elapsed
 *
 */
/* Start year of struct TM*/
#define NU_TM_YEAR0         1900
/* Start year of POSIX time (set_time()/time()) */
#define NU_POSIX_YEAR0      1970
/* Start year of H/W RTC */
#define NU_HWRTC_YEAR0      2000

/* RTC H/W origin time: 00:00:00 UTC, Saturday, 1 January 2000 */
static const S_RTC_TIME_DATA_T DATETIME_HWRTC_ORIGIN = {
    2000,           /* Year value, range between 2000 ~ 2099 */
    1,              /* Month value, range between 1 ~ 12 */
    1,              /* Day value, range between 1 ~ 31 */
    RTC_SATURDAY,   /* Day of the week */
    0,              /* Hour value, range between 0 ~ 23 */
    0,              /* Minute value, range between 0 ~ 59 */
    0,              /* Second value, range between 0 ~ 59 */
    RTC_CLOCK_24,   /* 12-Hour (RTC_CLOCK_12) / 24-Hour (RTC_CLOCK_24) */
    0               /* RTC_AM / RTC_PM (used only for 12-Hour) */
};
/* t_hwrtc_origin initialized or not? */
static bool t_hwrtc_origin_inited = 0;
/* POSIX time of DATETIME_HWRTC_ORIGIN (since 00:00:00 UTC, Thursday, 1 January 1970) */
static time_t t_hwrtc_origin = 0;
/* POSIX time set by rtc_write() */
static time_t t_write = 0;
/* Convert date time from H/W RTC to struct TM */
static void rtc_convert_datetime_hwrtc_to_tm(struct tm *datetime_tm, const S_RTC_TIME_DATA_T *datetime_hwrtc);

static const struct nu_modinit_s rtc_modinit = {RTC_0, RTC_MODULE, CLK_CLKSEL3_RTCSEL_LXT, 0, 0, RTC_IRQn, NULL};

static void rtc_init_impl(void);
static void rtc_free_impl(void);
static int32_t rtc_isenabled_impl(void);
static int64_t rtc_read_impl(void);
static void rtc_write_impl(int64_t t);

static void rtc_init_impl(void)
{
    if (rtc_isenabled_impl()) {
        return;
    }

    RTC_Open(NULL);

    /* POSIX time origin (00:00:00 UTC, Thursday, 1 January 1970) */
    rtc_write_impl(0);
}

static void rtc_free_impl(void)
{
    CLK_DisableModuleClock_S(rtc_modinit.clkidx);
}

static int32_t rtc_isenabled_impl(void)
{
    // To access (RTC) registers, clock must be enabled first.
    // For TZ, with RTC being secure, we needn't call the secure gateway versions.
    CLK_EnableModuleClock(rtc_modinit.clkidx);
    CLK_SetModuleClock(rtc_modinit.clkidx, rtc_modinit.clksrc, rtc_modinit.clkdiv);

    RTC_T *rtc_base = (RTC_T *) NU_MODBASE(rtc_modinit.modname);
    
    // NOTE: Check RTC Init Active flag to support crossing reset cycle.
    return !! (rtc_base->INIT & RTC_INIT_ACTIVE_Msk);
}

static int64_t rtc_read_impl(void)
{
    /* NOTE: After boot, RTC time registers are not synced immediately, about 1 sec latency.
     *       RTC time got (through RTC_GetDateAndTime()) in this sec would be last-synced and incorrect.
     *       NUC472/M453: Known issue
     *       M487: Fixed
     */
    if (! rtc_isenabled_impl()) {
        rtc_init_impl();
    }

    /* Used for intermediary between date time of H/W RTC and POSIX time */
    struct tm datetime_tm;

    if (! t_hwrtc_origin_inited) {
        t_hwrtc_origin_inited = 1;
        
        /* Convert date time from H/W RTC to struct TM */     
        rtc_convert_datetime_hwrtc_to_tm(&datetime_tm, &DATETIME_HWRTC_ORIGIN);
        /* Convert date time of struct TM to POSIX time */
        if (! _rtc_maketime(&datetime_tm, &t_hwrtc_origin, RTC_FULL_LEAP_YEAR_SUPPORT)) {
            return 0;
        }

        /* Load t_write from RTC spare register to cross reset cycle */
        RTC_T *rtc_base = (RTC_T *) NU_MODBASE(rtc_modinit.modname);
        RTC_WaitAccessEnable();
        RTC_EnableSpareAccess();
        RTC_WaitAccessEnable();
        t_write = RTC_READ_SPARE_REGISTER(rtc_base, 0);
    }

    S_RTC_TIME_DATA_T hwrtc_datetime_2K_present;
    RTC_WaitAccessEnable();
    RTC_GetDateAndTime(&hwrtc_datetime_2K_present);
    /* Convert date time from H/W RTC to struct TM */
    rtc_convert_datetime_hwrtc_to_tm(&datetime_tm, &hwrtc_datetime_2K_present);
    /* Convert date time of struct TM to POSIX time */
    time_t t_hwrtc_elapsed;
    if (! _rtc_maketime(&datetime_tm, &t_hwrtc_elapsed, RTC_FULL_LEAP_YEAR_SUPPORT)) {
        return 0;
    }

    /* Present time in POSIX time */
    time_t t_present = t_write + (t_hwrtc_elapsed - t_hwrtc_origin);
    return t_present;
}

static void rtc_write_impl(int64_t t)
{
    if (! rtc_isenabled_impl()) {
        rtc_init_impl();
    }

    t_write = t;

    /* Store t_write to RTC spare register to cross reset cycle */
    RTC_T *rtc_base = (RTC_T *) NU_MODBASE(rtc_modinit.modname);
    RTC_WaitAccessEnable();
    RTC_EnableSpareAccess();
    RTC_WaitAccessEnable();
    RTC_WRITE_SPARE_REGISTER(rtc_base, 0, t_write);

    RTC_WaitAccessEnable();
    RTC_SetDateAndTime((S_RTC_TIME_DATA_T *) &DATETIME_HWRTC_ORIGIN);
    /* NOTE: When engine is clocked by low power clock source (LXT/LIRC), we need to wait for 3 engine clocks. */
    wait_us((NU_US_PER_SEC / NU_RTCCLK_PER_SEC) * 3);
}

/*
 struct tm
   tm_sec      seconds after the minute 0-61
   tm_min      minutes after the hour 0-59
   tm_hour     hours since midnight 0-23
   tm_mday     day of the month 1-31
   tm_mon      months since January 0-11
   tm_year     years since 1900
   tm_wday     days since Sunday 0-6
   tm_yday     days since January 1 0-365
   tm_isdst    Daylight Saving Time flag
*/
static void rtc_convert_datetime_hwrtc_to_tm(struct tm *datetime_tm, const S_RTC_TIME_DATA_T *datetime_hwrtc)
{
    datetime_tm->tm_year = datetime_hwrtc->u32Year - NU_TM_YEAR0;
    datetime_tm->tm_mon  = datetime_hwrtc->u32Month - 1;
    datetime_tm->tm_mday = datetime_hwrtc->u32Day;
    datetime_tm->tm_wday = datetime_hwrtc->u32DayOfWeek;
    datetime_tm->tm_hour = datetime_hwrtc->u32Hour;
    if (datetime_hwrtc->u32TimeScale == RTC_CLOCK_12 && datetime_hwrtc->u32AmPm == RTC_PM) {
        datetime_tm->tm_hour += 12;
    }
    datetime_tm->tm_min  = datetime_hwrtc->u32Minute;
    datetime_tm->tm_sec  = datetime_hwrtc->u32Second;
}


#if (TFM_LVL > 0)
    
__NONSECURE_ENTRY
int32_t rtc_init_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
    rtc_init_impl();
    return 0;
}

__NONSECURE_ENTRY
int32_t rtc_free_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
    rtc_free_impl();
    return 0;
}

__NONSECURE_ENTRY
int32_t rtc_isenabled_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
    int32_t *enabled = (int32_t *) arg0;
    *enabled = rtc_isenabled_impl();
    return 0;
}

__NONSECURE_ENTRY
int32_t rtc_read_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
    int64_t *t = (int64_t *) arg0;
    *t = rtc_read_impl();
    return 0;
}

__NONSECURE_ENTRY
int32_t rtc_write_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
    int64_t *t = (int64_t *) arg0;
    rtc_write_impl(*t);
    return 0;
}

#endif

#endif

#if defined(DOMAIN_NS) && (DOMAIN_NS == 1) && (TFM_LVL > 0)

void rtc_init_s(void)
{
    tfm_ns_lock_dispatch(rtc_init_veneer, 0, 0, 0, 0);
}

void rtc_free_s(void)
{
    tfm_ns_lock_dispatch(rtc_free_veneer, 0, 0, 0, 0);
}

int32_t rtc_isenabled_s(void)
{
    int32_t enabled = 0;
    tfm_ns_lock_dispatch(rtc_isenabled_veneer, (uint32_t) &enabled, 0, 0, 0);
    return enabled;
}

int64_t rtc_read_s(void)
{
    int64_t t = 0;
    tfm_ns_lock_dispatch(rtc_read_veneer, (uint32_t) &t, 0, 0, 0);
    return t;
}

void rtc_write_s(int64_t t)
{
    tfm_ns_lock_dispatch(rtc_write_veneer, (uint32_t) &t, 0, 0, 0);
}

#elif defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)

#if (TFM_LVL == 0)
__NONSECURE_ENTRY
#endif
void rtc_init_s(void)
{
    rtc_init_impl();
}

#if (TFM_LVL == 0)
__NONSECURE_ENTRY
#endif
void rtc_free_s(void)
{
    rtc_free_impl();
}

#if (TFM_LVL == 0)
__NONSECURE_ENTRY
#endif
int32_t rtc_isenabled_s(void)
{
    return rtc_isenabled_impl();
}

#if (TFM_LVL == 0)
__NONSECURE_ENTRY
#endif
int64_t rtc_read_s(void)
{
    return rtc_read_impl();
}

#if (TFM_LVL == 0)
__NONSECURE_ENTRY
#endif
void rtc_write_s(int64_t t)
{
    rtc_write_impl(t);
}

#endif

#endif
