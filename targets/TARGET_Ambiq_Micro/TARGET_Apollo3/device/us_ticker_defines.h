// Comment at top

#ifndef MBED_US_TICKER_DEFINES_H
#define MBED_US_TICKER_DEFINES_H

//  * US_TICKER_PERIOD_NUM, US_TICKER_PERIOD_DEN: These denote the ratio (numerator, denominator)
//  * of the ticker period to a microsecond. For example, an 8MHz ticker would have NUM = 1, DEN = 8;
//  * a 1MHz ticker would have NUM = 1, DEN = 1; a 250kHz ticker would have NUM = 4, DEN = 1.
//  * Both numerator and denominator must be 16 bits or less.
#define US_TICKER_FREQ AM_HAL_STIMER_HFRC_3MHZ
#define US_TICKER_PERIOD_NUM 1
#define US_TICKER_PERIOD_DEN 3

//  *
//  * US_TICKER_MASK: The value mask for the ticker - eg 0x07FFFFFF for a 27-bit ticker.
//  *
#define US_TICKER_MASK 0xFFFFFFFF

#define US_TICKER_COMPARE_INSTANCE A
#define US_TICKER_STIMER_INT_COMPARE_CONCATENATOR(a, b) a##b
#define US_TICKER_STIMER_INT_COMPARE_EVALUATOR(a, b) US_TICKER_STIMER_INT_COMPARE_CONCATENATOR(a, b)
#define US_TICKER_STIMER_INT_COMPARE US_TICKER_STIMER_INT_COMPARE_EVALUATOR(AM_HAL_STIMER_INT_COMPARE, US_TICKER_COMPARE_INSTANCE)

#endif