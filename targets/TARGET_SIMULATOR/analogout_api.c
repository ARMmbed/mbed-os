#include <math.h>
#include "analogout_api.h"
#include "objects.h"
#include "emscripten.h"

void analogout_init(dac_t *obj, PinName pin) {
    obj->pin = pin;

    EM_ASM_({
        MbedJSHal.gpio.init_analogout($0, $1);
    }, obj, obj->pin);
}

void analogout_free(dac_t *obj) {

}

void analogout_write(dac_t *obj, float value) {
    EM_ASM_({
        MbedJSHal.gpio.write($0, $1);
    }, obj->pin, (lroundf(value * 1024.0f)));
}

void analogout_write_u16(dac_t *obj, uint16_t value) {
    EM_ASM_({
        MbedJSHal.gpio.write($0, $1);
    }, obj->pin, value);
}

float analogout_read(dac_t *obj) {
    int v = EM_ASM_INT({
        return MbedJSHal.gpio.read($0);
    }, obj->pin);

    return ((float)v) / 1024.0f;
}

uint16_t analogout_read_u16(dac_t *obj) {
    return EM_ASM_INT({
        return MbedJSHal.gpio.read($0);
    }, obj->pin);
}
