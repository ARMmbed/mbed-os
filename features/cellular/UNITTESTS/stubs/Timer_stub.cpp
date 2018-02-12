
#include "Timer.h"
#include "Timer_stub.h"

namespace mbed {

Timer::Timer()  {
}

Timer::Timer(const ticker_data_t *data) {
}

Timer::~Timer() {
}

void Timer::start() {
}

void Timer::stop() {;
}

int Timer::read_us() {
    return 0;
}

float Timer::read() {
    return 0;
}

int Timer::read_ms() {
    timer_stub_value += timer_stub_step;
    return timer_stub_value;
}

us_timestamp_t Timer::read_high_resolution_us() {
    return 0;
}

void Timer::reset()
{
}

Timer::operator float() {
    return 0;
}

} // namespace mbed
