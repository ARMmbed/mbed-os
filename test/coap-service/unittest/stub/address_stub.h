#ifndef __ADDRESS_STUB_H__
#define __ADDRESS_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t expectedUint8;
    uint16_t expectedUint16;
    uint8_t *expectedUint8ptr;
    uint8_t returnUint8;
} address_stub_def;

extern address_stub_def address_stub;

#ifdef __cplusplus
}
#endif

#endif // __ADDRESS_STUB_H__
