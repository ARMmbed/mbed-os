#ifndef __NS_TIMER_STUB_H__
#define __NS_TIMER_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int8_t int8_value;
    void (*cb)(int8_t, uint16_t);
} ns_timer_stub_def;

extern ns_timer_stub_def ns_timer_stub;

#ifdef __cplusplus
}
#endif

#endif // __NS_TIMER_STUB_H__
