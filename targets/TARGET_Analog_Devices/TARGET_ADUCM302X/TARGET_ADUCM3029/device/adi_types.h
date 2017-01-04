#ifndef __ADI_TYPES_H__
#define __ADI_TYPES_H__

/* obtain integer types ... */
#include <stdint.h>

/* obtain boolean types ... */
#include <stdbool.h>

/* define required types that are not provided by stdint.h or stdbool.h ... */
typedef bool                bool_t;
typedef char                char_t;
typedef float               float32_t;
#if !defined(__NO_FLOAT64)
typedef long double         float64_t;
#endif

#endif /* __ADI_TYPES_H__ */
