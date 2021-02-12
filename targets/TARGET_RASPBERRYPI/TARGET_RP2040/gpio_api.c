#include "mbed_assert.h"
#include "hal/gpio_api.h"
#include "pinmap.h"
#include "mbed_error.h"

void gpio_write(gpio_t *obj, int value)
{
    gpio_put(obj->pin, value);
}

int gpio_read(gpio_t *obj)
{
    return gpio_get(obj->pin);
}

void gpio_init(gpio_t *obj, PinName pin)
{
    obj->pin = pin;
    _gpio_init(obj->pin);
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    gpio_set_pulls(obj->pin, mode == PullUp, mode == PullDown);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    obj->direction = direction;
    if (direction == PIN_OUTPUT) {
        gpio_set_dir(obj->pin, GPIO_OUT);
    }
    if (direction == PIN_INPUT) {
        gpio_set_dir(obj->pin, GPIO_IN);
    }
}

