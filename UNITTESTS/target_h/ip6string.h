
#ifndef IP6STRING_H
#define IP6STRING_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint_fast8_t ip6tos(const void *ip6addr, char *p)
{
    return 0;
}

void stoip6(const char *ip6addr, size_t len, void *dest) {}

#ifdef __cplusplus
}
#endif
#endif
