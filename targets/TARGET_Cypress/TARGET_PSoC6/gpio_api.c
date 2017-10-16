
#include "cmsis.h"
#include "device.h"
#include "mbed_assert.h"
#include "PeripheralNames.h"
#include "PinNames.h"
#include "PortNames.h"
#include "drivers/peripheral/gpio/cy_gpio.h"

void _gpio_set_dir_mode(gpio_t *obj);
uint32_t _gpio_get_cy_drive_mode(PinDirection dir, PinMode mode);


void gpio_init(gpio_t *obj, PinName pin)
{
    obj->pin = pin;

    MBED_ASSERT(obj->pin != NC);

    GPIO_PRT_Type *port = Cy_GPIO_PortToAddr(CYPRESS_PORT(obj->pin));
    uint32_t ppin  = CYPRESS_PIN(obj->pin);

    const cy_stc_gpio_pin_config_t config = {
        .outVal    = 0x01,  // must be 1 for DigitalIn to work. Does not seems to influence DigitalOut. Tested with PullDefault = PullUp
        .driveMode = _gpio_get_cy_drive_mode(obj->dir, obj->mode),
        .hsiom     = HSIOM_SEL_GPIO,
    };
    Cy_GPIO_Pin_Init(port, ppin, &config);
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    MBED_ASSERT(obj->pin != NC);

    obj->mode = mode;
    _gpio_set_dir_mode(obj);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    MBED_ASSERT(obj->pin != NC);

    obj->dir = direction;
    _gpio_set_dir_mode(obj);
}

void gpio_write(gpio_t *obj, int value)
{
    MBED_ASSERT(obj->pin != NC);

    GPIO_PRT_Type *port = Cy_GPIO_PortToAddr(CYPRESS_PORT(obj->pin));
    uint32_t pin  = CYPRESS_PIN(obj->pin);

    Cy_GPIO_Write(port, pin, value);
}

/*
    Return an integer representing the output setting of the pin if it is an
    output, or read the input if set as an input.
    (From DigitalOut::read)
*/
int gpio_read(gpio_t *obj)
{
    int ret = 0;

    MBED_ASSERT(obj->pin != NC);

    GPIO_PRT_Type *port = Cy_GPIO_PortToAddr(CYPRESS_PORT(obj->pin));
    uint32_t pin  = CYPRESS_PIN(obj->pin);

    switch (obj->dir) {
        case PIN_INPUT:
            ret = Cy_GPIO_Read(port, pin);
            break;

        case PIN_OUTPUT:
            ret = Cy_GPIO_ReadOut(port, pin);
            break;
    }

    return ret;
}

int gpio_is_connected(const gpio_t *obj)
{
    return obj->pin != NC;
}

void _gpio_set_dir_mode(gpio_t *obj)
{
    MBED_ASSERT(obj->pin != NC);

    GPIO_PRT_Type *port = Cy_GPIO_PortToAddr(CYPRESS_PORT(obj->pin));
    uint32_t pin  = CYPRESS_PIN(obj->pin);
    uint32_t cymode = _gpio_get_cy_drive_mode(obj->dir, obj->mode);

    Cy_GPIO_SetDrivemode(port, pin, cymode);
}

uint32_t _gpio_get_cy_drive_mode(PinDirection dir, PinMode mode)
{
    uint32_t cymode = 0;

    switch (dir) {
        case PIN_INPUT:
            switch (mode) {
                case PullNone:
                    cymode = CY_GPIO_DM_STRONG;
                    break;
                case PullUp:
                    cymode = CY_GPIO_DM_PULLUP;
                    break;
                case PullDown:
                    cymode = CY_GPIO_DM_PULLDOWN;
                    break;
                case OpenDrainPullUp:
                    cymode = CY_GPIO_DM_OD_DRIVESHIGH;
                    break;
                case OpenDrainNoPull:
                    cymode = CY_GPIO_DM_STRONG;
                    break;
                case OpenDrainPullDown:
                    cymode = CY_GPIO_DM_OD_DRIVESLOW;
                    break;
            }
            break;

        case PIN_OUTPUT:
            switch (mode) {
                case PullNone:
                    cymode = CY_GPIO_DM_STRONG_IN_OFF;
                    break;
                case PullUp:
                    cymode = CY_GPIO_DM_PULLUP_IN_OFF;
                    break;
                case PullDown:
                    cymode = CY_GPIO_DM_PULLDOWN_IN_OFF;
                    break;
                case OpenDrainPullUp:
                    cymode = CY_GPIO_DM_OD_DRIVESHIGH_IN_OFF;
                    break;
                case OpenDrainNoPull:
                    cymode = CY_GPIO_DM_STRONG_IN_OFF;
                    break;
                case OpenDrainPullDown:
                    cymode = CY_GPIO_DM_OD_DRIVESLOW_IN_OFF;
                    break;
            }
            break;
    }

    return cymode;
}
