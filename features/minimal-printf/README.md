# Minimal printf and snprintf

Library supports both printf and snprintf in 1252 bytes of flash.

Prints directly to stdio/UART without using malloc. All flags and precision modifiers are ignored. Floating point is disabled by default.

Supports:
* %d: signed integer [h, hh, (none), l, ll, z, j, t].
* %i: signed integer [h, hh, (none), l, ll, z, j, t].
* %u: unsigned integer [h, hh, (none), l, ll, z, j, t].
* %x: unsigned integer [h, hh, (none), l, ll, z, j, t], printed as hexadecimal number (e.g., FF).
* %X: unsigned integer [h, hh, (none), l, ll, z, j, t], printed as hexadecimal number (e.g., FF).
* %f: floating point (disabled by default).
* %F: floating point (disabled by default).
* %g: floating point (disabled by default).
* %G: floating point (disabled by default).
* %c: character.
* %s: string.
* %p: pointer (e.g. 0x00123456).

## Enabling floating point and setting baud rate

In mbed_app.json:

```
    "target_overrides": {
        "*": {
            "platform.stdio-baud-rate": 115200,
            "minimal-printf.floating-point": true,
            "minimal-printf.floating-point-max-decimals": 10
        }
    }
```


## Size comparison

Example application compiled with minimal mbed OS (no RTOS) using this .mbedignore:

```
mbed-os/rtos/*
mbed-os/features/FEATURE_LWIP/*
mbed-os/features/FEATURE_CLIENT/*
mbed-os/features/FEATURE_COMMON_PAL/*
mbed-os/features/FEATURE_UVISOR/*
mbed-os/features/frameworks/*
mbed-os/features/net/*
mbed-os/features/netsocket/*
mbed-os/features/storage/*
mbed-os/events/*
```

### Example application
```
#include "mbed.h"
#include "mbed_printf.h"
#include <limits.h>

int main()
{
    char buffer[1000];
    int result;

    double pi = 3.14159265359;

#if 0
    result = printf("hello world %d %u %X %p %s %2.5f %% %\r\n", LONG_MAX, ULONG_MAX, UCHAR_MAX, buffer, "muh", pi);
    printf("results: %d\r\n", result);

    result = snprintf(buffer, 1000, "hello world %d %u %X %p %s %2.5f %% %\r\n", LONG_MIN, 0, 0, buffer, "muh", -1*pi);
    printf("%s\r\n", buffer);

    printf("results: %d\r\n", result);

#else
    result = mbed_printf("hello world %ld %llu %02X %p %s %2.5f %% %\r\n", LONG_MAX, ULONG_MAX, UCHAR_MAX, buffer, "muh", pi);
    mbed_printf("results: %d\r\n", result);

    result = mbed_snprintf(buffer, 1000, "hello world %d %u %X %p %s %2.5f %% %\r\n", LONG_MIN, 0, 0, buffer, "muh", -1*pi);
    mbed_printf("%s\r\n", buffer);

    mbed_printf("results: %d\r\n", result);
#endif
}
```


### K64F GCC minimal printf without floating point

```
+------------------------------------------+-------+-------+------+
| Module                                   | .text | .data | .bss |
+------------------------------------------+-------+-------+------+
| [fill]                                   |    16 |     3 | 2102 |
| [lib]/c.a/lib_a-init.o                   |    80 |     0 |    0 |
| [lib]/c.a/lib_a-memcpy.o                 |   308 |     0 |    0 |
| [lib]/c.a/lib_a-memset.o                 |   156 |     0 |    0 |
| [lib]/misc/                              |   248 |     4 |   28 |
| main.o                                   |   256 |     0 |    0 |
| mbed-os/hal/mbed_gpio.o                  |    96 |     0 |    0 |
| mbed-os/hal/mbed_pinmap_common.o         |   212 |     0 |    0 |
| mbed-os/hal/mbed_ticker_api.o            |   214 |     0 |    0 |
| mbed-os/hal/mbed_us_ticker_api.o         |    44 |     0 |   24 |
| mbed-os/platform/mbed_board.o            |    90 |     0 |    0 |
| mbed-os/platform/mbed_critical.o         |    56 |     0 |    5 |
| mbed-os/platform/mbed_error.o            |    32 |     0 |    1 |
| mbed-os/platform/mbed_retarget.o         |    20 |     0 |    0 |
| mbed-os/platform/mbed_sdk_boot.o         |    74 |     0 |    0 |
| mbed-os/platform/mbed_wait_api_no_rtos.o |    32 |     0 |    0 |
| mbed-os/targets/TARGET_Freescale         |  4758 |    12 |  384 |
| mbed-printf/mbed_printf.o                |   960 |     1 |  188 |
| Subtotals                                |  7652 |    20 | 2732 |
+------------------------------------------+-------+-------+------+
Total Static RAM memory (data + bss): 2752 bytes
Total Flash memory (text + data): 7672 bytes
```


### K64F GCC minimal printf with floating point

```
+------------------------------------------+-------+-------+------+
| Module                                   | .text | .data | .bss |
+------------------------------------------+-------+-------+------+
| [fill]                                   |    24 |     3 | 2102 |
| [lib]/c.a/lib_a-init.o                   |    80 |     0 |    0 |
| [lib]/c.a/lib_a-memcpy.o                 |   308 |     0 |    0 |
| [lib]/c.a/lib_a-memset.o                 |   156 |     0 |    0 |
| [lib]/gcc.a/_arm_addsubdf3.o             |   880 |     0 |    0 |
| [lib]/gcc.a/_arm_fixdfsi.o               |    80 |     0 |    0 |
| [lib]/gcc.a/_arm_muldivdf3.o             |  1060 |     0 |    0 |
| [lib]/misc/                              |   248 |     4 |   28 |
| main.o                                   |   256 |     0 |    0 |
| mbed-os/hal/mbed_gpio.o                  |    96 |     0 |    0 |
| mbed-os/hal/mbed_pinmap_common.o         |   212 |     0 |    0 |
| mbed-os/hal/mbed_ticker_api.o            |   214 |     0 |    0 |
| mbed-os/hal/mbed_us_ticker_api.o         |    44 |     0 |   24 |
| mbed-os/platform/mbed_board.o            |    90 |     0 |    0 |
| mbed-os/platform/mbed_critical.o         |    56 |     0 |    5 |
| mbed-os/platform/mbed_error.o            |    32 |     0 |    1 |
| mbed-os/platform/mbed_retarget.o         |    20 |     0 |    0 |
| mbed-os/platform/mbed_sdk_boot.o         |    74 |     0 |    0 |
| mbed-os/platform/mbed_wait_api_no_rtos.o |    32 |     0 |    0 |
| mbed-os/targets/TARGET_Freescale         |  4758 |    12 |  384 |
| mbed-printf/mbed_printf.o                |  1160 |     1 |  188 |
| Subtotals                                |  9880 |    20 | 2732 |
+------------------------------------------+-------+-------+------+
Total Static RAM memory (data + bss): 2752 bytes
Total Flash memory (text + data): 9900 bytes
```


### K64F GCC standard printf

```
+------------------------------------------+-------+-------+------+
| Module                                   | .text | .data | .bss |
+------------------------------------------+-------+-------+------+
| [fill]                                   |    62 |     4 | 2071 |
| [lib]/c.a/lib_a-callocr.o                |    96 |     0 |    0 |
| [lib]/c.a/lib_a-closer.o                 |    36 |     0 |    0 |
| [lib]/c.a/lib_a-ctype_.o                 |   257 |     0 |    0 |
| [lib]/c.a/lib_a-dtoa.o                   |  3948 |     0 |    0 |
| [lib]/c.a/lib_a-errno.o                  |    12 |     0 |    0 |
| [lib]/c.a/lib_a-fclose.o                 |   196 |     0 |    0 |
| [lib]/c.a/lib_a-fflush.o                 |   404 |     0 |    0 |
| [lib]/c.a/lib_a-findfp.o                 |   204 |     0 |    0 |
| [lib]/c.a/lib_a-fputwc.o                 |   264 |     0 |    0 |
| [lib]/c.a/lib_a-freer.o                  |   620 |     0 |    0 |
| [lib]/c.a/lib_a-fstatr.o                 |    40 |     0 |    0 |
| [lib]/c.a/lib_a-fvwrite.o                |   752 |     0 |    0 |
| [lib]/c.a/lib_a-fwalk.o                  |    84 |     0 |    0 |
| [lib]/c.a/lib_a-impure.o                 |     0 |  1068 |    0 |
| [lib]/c.a/lib_a-init.o                   |    80 |     0 |    0 |
| [lib]/c.a/lib_a-isattyr.o                |    36 |     0 |    0 |
| [lib]/c.a/lib_a-locale.o                 |    42 |   364 |    0 |
| [lib]/c.a/lib_a-localeconv.o             |    28 |     0 |    0 |
| [lib]/c.a/lib_a-lock.o                   |    16 |     0 |   33 |
| [lib]/c.a/lib_a-lseekr.o                 |    44 |     0 |    0 |
| [lib]/c.a/lib_a-makebuf.o                |   240 |     0 |    0 |
| [lib]/c.a/lib_a-mallocr.o                |  1376 |  1040 |   52 |
| [lib]/c.a/lib_a-mbtowc_r.o               |    44 |     0 |    0 |
| [lib]/c.a/lib_a-memchr.o                 |   160 |     0 |    0 |
| [lib]/c.a/lib_a-memcpy.o                 |   308 |     0 |    0 |
| [lib]/c.a/lib_a-memmove.o                |   200 |     0 |    0 |
| [lib]/c.a/lib_a-memset.o                 |   156 |     0 |    0 |
| [lib]/c.a/lib_a-mprec.o                  |  1776 |     0 |    0 |
| [lib]/c.a/lib_a-printf.o                 |    40 |     0 |    0 |
| [lib]/c.a/lib_a-readr.o                  |    44 |     0 |    0 |
| [lib]/c.a/lib_a-reallocr.o               |   844 |     0 |    0 |
| [lib]/c.a/lib_a-reent.o                  |     0 |     0 |    4 |
| [lib]/c.a/lib_a-sbrkr.o                  |    36 |     0 |    0 |
| [lib]/c.a/lib_a-snprintf.o               |   152 |     0 |    0 |
| [lib]/c.a/lib_a-stdio.o                  |   136 |     0 |    0 |
| [lib]/c.a/lib_a-strlen.o                 |   220 |     0 |    0 |
| [lib]/c.a/lib_a-svfiprintf.o             |   252 |     0 |    0 |
| [lib]/c.a/lib_a-svfprintf.o              |  5370 |     0 |    0 |
| [lib]/c.a/lib_a-vfiprintf.o              |   148 |     0 |    0 |
| [lib]/c.a/lib_a-vfprintf.o               |  5754 |     0 |    0 |
| [lib]/c.a/lib_a-wbuf.o                   |   176 |     0 |    0 |
| [lib]/c.a/lib_a-wcrtomb.o                |    84 |     0 |    0 |
| [lib]/c.a/lib_a-wctomb_r.o               |    28 |     0 |    0 |
| [lib]/c.a/lib_a-writer.o                 |    44 |     0 |    0 |
| [lib]/c.a/lib_a-wsetup.o                 |   200 |     0 |    0 |
| [lib]/gcc.a/_aeabi_uldivmod.o            |    48 |     0 |    0 |
| [lib]/gcc.a/_arm_addsubdf3.o             |   880 |     0 |    0 |
| [lib]/gcc.a/_arm_cmpdf2.o                |   272 |     0 |    0 |
| [lib]/gcc.a/_arm_fixdfsi.o               |    80 |     0 |    0 |
| [lib]/gcc.a/_arm_muldivdf3.o             |  1060 |     0 |    0 |
| [lib]/gcc.a/_arm_unorddf2.o              |    44 |     0 |    0 |
| [lib]/gcc.a/_dvmd_tls.o                  |     4 |     0 |    0 |
| [lib]/gcc.a/_udivmoddi4.o                |   732 |     0 |    0 |
| [lib]/misc/                              |   248 |     4 |   28 |
| main.o                                   |   215 |     0 |    0 |
| mbed-os/hal/mbed_gpio.o                  |    96 |     0 |    0 |
| mbed-os/hal/mbed_pinmap_common.o         |   212 |     0 |    0 |
| mbed-os/hal/mbed_ticker_api.o            |   214 |     0 |    0 |
| mbed-os/hal/mbed_us_ticker_api.o         |    44 |     0 |   24 |
| mbed-os/platform/mbed_alloc_wrappers.o   |    16 |     0 |    0 |
| mbed-os/platform/mbed_board.o            |    90 |     0 |    0 |
| mbed-os/platform/mbed_critical.o         |    56 |     0 |    5 |
| mbed-os/platform/mbed_error.o            |    32 |     0 |    1 |
| mbed-os/platform/mbed_retarget.o         |   646 |     4 |  266 |
| mbed-os/platform/mbed_sdk_boot.o         |    74 |     0 |    0 |
| mbed-os/platform/mbed_wait_api_no_rtos.o |    32 |     0 |    0 |
| mbed-os/targets/TARGET_Freescale         |  4834 |    12 |  384 |
| Subtotals                                | 34938 |  2496 | 2868 |
+------------------------------------------+-------+-------+------+
Total Static RAM memory (data + bss): 5364 bytes
Total Flash memory (text + data): 37434 bytes
```
