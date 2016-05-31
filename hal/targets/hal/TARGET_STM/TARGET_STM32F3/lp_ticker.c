#include "device.h"

#if DEVICE_LOWPOWERTIMER

#include "ticker_api.h"
#include "lp_ticker_api.h"
#include "rtc_api.h"
#include "rtc_api_hal.h"

static uint8_t lp_ticker_inited = 0;
static uint32_t m_timestamp = 0;
static uint32_t m_offset = 0;

void lp_ticker_init() {
    /*
     * At this stage we don't have the users requested time.
     * This is only to cover the API implementation.
     */
}

uint32_t lp_ticker_read() {
    /*
     * Becuase our RTC has a periodic wake-up timer we just want to simply
     * return the desired timestamp value to configure RTC exactly with the value
     * provided by the user and to fulfill the mBed ticker architecture.
     */
    return m_timestamp;
}

void lp_ticker_set_interrupt(timestamp_t timestamp) {
    m_offset = timestamp;

    if (lp_ticker_inited) return;
    lp_ticker_inited = 1;

    rtc_periodic_ticker_init(timestamp);
    rtc_set_irq_handler((uint32_t) lp_ticker_irq_handler);
}

void lp_ticker_disable_interrupt() {
    lp_ticker_inited = 0;
    rtc_periodic_ticker_disable_irq();
}

void lp_ticker_clear_interrupt() {
    m_timestamp = m_offset;
}

#endif
