#include "analogin_api.h"
#include "emscripten.h"
#include "objects.h"

void analogin_init(analogin_t *obj, PinName pin) {
    obj->pin = pin;

    EM_ASM_({
        MbedJSHal.gpio.init_analogin($0, $1);
    }, obj, obj->pin);
}

uint16_t analogin_read_u16(analogin_t *obj) {
    return EM_ASM_INT({
        return MbedJSHal.gpio.read($0);
    }, obj->pin);
}

float analogin_read(analogin_t *obj) {
    int v = EM_ASM_INT({
        return MbedJSHal.gpio.read($0);
    }, obj->pin);

    return ((float)v) / 1024.0f;
}
