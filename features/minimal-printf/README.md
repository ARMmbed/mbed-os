# Minimal printf and snprintf

Library supports both printf and snprintf in 1252 bytes of flash.

Prints directly to stdio/UART without using malloc. All flags and precision modifiers are ignored.
Floating point is enabled by default.
Printing to a FILE stream is enabled by default.

Supports:
* %d: signed integer [h, hh, (none), l, ll, z, j, t].
* %i: signed integer [h, hh, (none), l, ll, z, j, t].
* %u: unsigned integer [h, hh, (none), l, ll, z, j, t].
* %x: unsigned integer [h, hh, (none), l, ll, z, j, t], printed as hexadecimal number (e.g., ff).
* %X: unsigned integer [h, hh, (none), l, ll, z, j, t], printed as hexadecimal number (e.g., FF).
* %f: floating point (disabled by default).
* %F: floating point (disabled by default, treated as %f).
* %g: floating point (disabled by default, treated as %f).
* %G: floating point (disabled by default, treated as %f).
* %c: character.
* %s: string.
* %p: pointer (e.g. 0x00123456).

Unrecognized format specifiers are treated as ordinary characters.

Floating point support:
* Floating point is enabled by default.
* All floating points are treated as %f.
* No support for inf, infinity or nan

## Usage

To replace the standard implementation of the printf functions with the ones in this library:

Compile with mbed-cli using the custom `mprintf` profile. For example, to compile in release mode:

```
$ mbed compile -t <toolchain> -m <target> --profile release --profile mprintf
```

## Enabling FILE stream, floating point and 64 bit integers

In mbed_app.json:

```
    "target_overrides": {
        "*": {
            "minimal-printf.enable-file-stream": true,
            "minimal-printf.enable-floating-point": true,
            "minimal-printf.set-floating-point-max-decimals": 6,
            "minimal-printf.enable-64-bit": true
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

### Full application size on K64F/GCC

|             | Floating point | 64 bit integers | Flash | RAM  |
| -           | -              | -               | -     | -    |
| mbed-printf |                |                 | 7772  | 2752 |
| mbed-printf |                | X               | 8708  | 2752 |
| mbed-printf | X              |                 | 10368 | 2752 |
| mbed-printf | X              | X               | 11360 | 2752 |
| std printf  | X              | X               | 37354 | 5364 |
