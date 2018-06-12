#include "rtos/EventFlags.h"

rtos::EventFlags::EventFlags()                      {}
rtos::EventFlags::~EventFlags()                     {}
uint32_t rtos::EventFlags::set(uint32_t flags)
{
    return 0;
}
uint32_t rtos::EventFlags::clear(uint32_t flags)
{
    return 0;
}
uint32_t rtos::EventFlags::get() const
{
    return 0;
}
uint32_t rtos::EventFlags::wait_all(uint32_t flags, uint32_t timeout, bool clear)
{
    return 0;
}
uint32_t rtos::EventFlags::wait_any(uint32_t flags, uint32_t timeout, bool clear)
{
    return 0;
}
