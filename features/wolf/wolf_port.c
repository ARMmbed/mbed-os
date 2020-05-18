/* wolf_port.c 
 * Used for wolf specific porting functions.
 */

#include "hal/trng_api.h"
#include "hal/rtc_api.h"
#include "mbed_mktime.h"

#ifdef USE_WOLFSSL_LIB
#include "wolfssl/wolfcrypt/settings.h"
#include "wolfssl/wolfcrypt/wc_port.h"

/* RNG CODE */
/* Return 0 to indicate success */
int wolf_rand_gen_seed(unsigned char* output, unsigned int sz)
{
    int ret;
    size_t outLen = 0;
    trng_t trng;
    trng_init(&trng);
    trng_get_bytes(&trng, output, sz, &outLen);
    trng_free(&trng);
    if (ret == 0 && sz == outLen)
        return 0;
    return -1;
}

/* RTC Time */
static struct tm rtc_time;
/* return UTC seconds */
time_t XTIME(time_t* timer)
{
    time_t seconds;
    rtc_init();
    seconds = rtc_read();
    rtc_free();
    (void)timer; /* unused */
    return seconds;
}
/* return struct tm (RTC) time */
struct tm* XGMTIME(const time_t* timer, struct tm* tmp)
{
    time_t seconds;

    rtc_init();
    seconds = rtc_read();
    _rtc_localtime(seconds, &rtc_time, RTC_4_YEAR_LEAP_YEAR_SUPPORT);
    rtc_free();
    (void)timer; /* unused */
    (void)tmp; /* unused */
    return &rtc_time;
}
#endif /* USE_WOLFSSL_LIB */
