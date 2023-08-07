#if DEVICE_RTC

#include "mbed_critical.h"
#include "rtc_api.h"
#include "hardware/rtc.h"
#include "hardware/structs/rtc.h"
#include "hardware/clocks.h"
#include "mbed_mktime.h"
#include "mbed_wait_api.h"

void rtc_init(void)
{
    core_util_critical_section_enter();

    // Calling _rtc_init() resets the current time.
    // So, if the RTC has already been initialized, we don't want to initialize it again.
    static bool rtc_initted = false;
    if(!rtc_initted)
    {
        pico_sdk_rtc_init();
        rtc_initted = true;
    }

    core_util_critical_section_exit();
}

void rtc_free(void)
{
   /* RTC clock can not be reset */
}

int rtc_isenabled(void)
{
    return rtc_running();
}

time_t rtc_read(void)
{
    struct tm timeinfo;
    time_t t;
    datetime_t date;
    
    if (!rtc_get_datetime(&date)) {
        return 0;
    } 

    /* Setup a tm structure based on the RTC
    struct tm :
        tm_sec      seconds after the minute 0-61
        tm_min      minutes after the hour 0-59
        tm_hour     hours since midnight 0-23
        tm_mday     day of the month 1-31
        tm_mon      months since January 0-11
        tm_year     years since 1900
        tm_yday     information is ignored by _rtc_maketime
        tm_wday     information is ignored by _rtc_maketime
        tm_isdst    information is ignored by _rtc_maketime
    */
    timeinfo.tm_year = date.year - 1900;
    timeinfo.tm_mon = date.month - 1;
    timeinfo.tm_mday = date.day;
    timeinfo.tm_wday = date.dotw;
    timeinfo.tm_hour = date.hour;
    timeinfo.tm_min = date.min;
    timeinfo.tm_sec = date.sec;

    if (_rtc_maketime(&timeinfo, &t, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
        return 0;
    }

    return t;
}

void rtc_write(time_t t)
{
    struct tm timeinfo;
    datetime_t date;

    // JS: After some testing, it seems that whatever value is written to the RTC always ends up being
    // 1 second higher after it's read back.  This means that writing the RTC and then reading it again
    // won't produce the expected result.  I wasn't able to find any errata or anything documenting
    // this behavior, but it's very consistent, and the RTC tests fail if the below block is removed.
    // To fix the error, we just decrease the time by 1 second before writing it to the registers.
    if (t >= 1) {
        t -= 1;
    }

    if (_rtc_localtime(t, &timeinfo, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
        return;
    }
    
    /* Setup a datetime_t structure based on the RTC
     struct datetime_t
        year;     0..4095
        month;    1..12, 1 is January
        day;      1..28,29,30,31 depending on month
        dotw;     0..6, 0 is Sunday
        hour;     0..23
        min;      0..59
        sec;      0..59
    */
    date.year = timeinfo.tm_year + 1900;
    date.month = timeinfo.tm_mon + 1;
    date.day = timeinfo.tm_mday;
    date.dotw = timeinfo.tm_wday;
    date.hour = timeinfo.tm_hour;
    date.min = timeinfo.tm_min;
    date.sec = timeinfo.tm_sec;


    core_util_critical_section_enter();
    rtc_set_datetime(&date);
    core_util_critical_section_exit();

    // Per the datasheet section 4.8.4, writes to the RTC take 2 RTC clocks (~50us at our frequency)
    // to go through.  However, the Mbed test suite expects the new time to be available immediately.
    // So, don't return from this function until the write is complete.
    // Instead, wait 2*(1/clk_rtc) microseconds.
    const int writeWaitTime = (2 * 1000000) / clock_get_hz(clk_rtc);
    wait_us(writeWaitTime);

    return;
}

#endif // DEVICE_RTC