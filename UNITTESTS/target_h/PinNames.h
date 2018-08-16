#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

typedef enum {
    NC = (int)0xFFFFFFFF
} PinName;

typedef enum {
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
