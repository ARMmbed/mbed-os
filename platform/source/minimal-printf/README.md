# Minimal printf and snprintf


Library supports both printf and snprintf in 1252 bytes of flash.

Prints directly to stdio/UART without using malloc. All flags and precision modifiers are ignored.

Supports:
* %d: signed integer [h, hh, (none), l, ll, z, j, t].
* %i: signed integer [h, hh, (none), l, ll, z, j, t].
* %u: unsigned integer [h, hh, (none), l, ll, z, j, t].
* %x: unsigned integer [h, hh, (none), l, ll, z, j, t], printed as hexadecimal number (e.g., ff).
* %X: unsigned integer [h, hh, (none), l, ll, z, j, t], printed as hexadecimal number (e.g., FF).
* %f: floating point (enabled by default).
* %F: floating point (enabled by default, treated as %f).
* %g: floating point (enabled by default, treated as %f).
* %G: floating point (enabled by default, treated as %f).
* %c: character.
* %s: string.
* %p: pointer (e.g. 0x00123456).

Unrecognized format specifiers are treated as ordinary characters.

Floating point limitations:
* All floating points are treated as %f.
* No support for inf, infinity or nan

## Configuration


Minimal printf is configured by the following parameters defined in `platform/mbed_lib.json`:

```
{
    "name": "platform",
    "config": {
       "minimal-printf-console-output": {
            "help": "Console output when using minimal-printf profile. Options: UART, SWO",
            "value": "UART"
        },
        "minimal-printf-enable-64-bit": {
            "help": "Enable printing 64 bit integers when using minimal-printf profile",
            "value": true
        },
        "minimal-printf-enable-file-stream": {
            "help": "Enable printing to a FILE stream when using minimal-printf profile",
            "value": true
        },
        "minimal-printf-enable-floating-point": {
            "help": "Enable floating point printing when using minimal-printf profile",
            "value": true
        },
        "minimal-printf-set-floating-point-max-decimals": {
            "help": "Maximum number of decimals to be printed",
            "value": 6
        }
   }
}
```

By default, 64 bit integers, floating point  and FILE stream printing are enabled.

If your target does not require some options then you can override the default configuration in your application `mbed_app.json` and achieve further memory optimisation (see next section for size comparison numbers).

In mbed_app.json:

```
    "target_overrides": {
        "*": {
            "platform.minimal-printf-enable-file-stream": false,
            "platform.minimal-printf-enable-floating-point": false,
            "platform.minimal-printf-set-floating-point-max-decimals": 6,
            "platform.minimal-printf-enable-64-bit": false
        }
    }
```

## Usage


To replace the standard implementation of the printf functions with the ones in this library:

Compile with mbed-cli using the custom `minimal-printf` profile. For example, to compile in release mode:

```
$ mbed compile -t <toolchain> -m <target> --profile release --profile mbed-os/tools/profiles/extensions/minimal-printf.json
```

## Size comparison


### Blinky application

https://github.com/ARMmbed/mbed-os-example-blinky application compiled with the different toolchains.

Blinky application size on K64F/GCC_ARM

|             | File stream | Floating point | 64 bit integers | Flash  | RAM    |
| -           | -           | -              | -               | -      | -      |
| mbed-printf |             |                |                 | 30,944 | 12,096 |
| mbed-printf |             |                | X               | 31,084 | 12,096 |
| mbed-printf |             | X              | X               | 33,824 | 12,096 |
| mbed-printf | X           | X              | X               | 34,304 | 12,096 |
| std printf  | X           | X              | X               | 54,158 | 12,112 |

Blinky application size on K64F/ARMC6

|             | File stream | Floating point | 64 bit integers | Flash  | RAM   |
| -           | -           | -              | -               | -      | -     |
| mbed-printf |             |                |                 | 31,543 | xxxxx |
| mbed-printf |             |                | X               | 31,691 | xxxxx |
| mbed-printf |             | X              | X               | 34,515 | xxxxx |
| mbed-printf | X           | X              | X               | 34,647 | xxxxx |
| std printf  | X           | X              | X               | 37,458 | xxxxx |

Blinky application size on K64F/IAR

|             | File stream | Floating point | 64 bit integers | Flash  | RAM    |
| -           | -           | -              | -               | -      | -      |
| mbed-printf |             |                |                 | 28,713 | 8,546  |
| mbed-printf |             |                | X               | 28,853 | 8,546  |
| mbed-printf |             | X              | X               | 30,661 | 8,546  |
| mbed-printf | X           | X              | X               | 32,047 | 8,594  |
| std printf  | X           | X              | X               | 35,055 | 8,462  |

### Blinky bare metal application

https://github.com/ARMmbed/mbed-os-example-blinky-baremetal application compiled with the different toolchains.

Blinky application size on K64F/GCC_ARM

|             | File stream | Floating point | 64 bit integers | Flash  | RAM   |
| -           | -           | -              | -               | -      | -     |
| mbed-printf |             |                |                 | 17,168 | 5,880 |
| mbed-printf |             |                | X               | 17,308 | 5,880 |
| mbed-printf |             | X              | X               | 20,044 | 5,880 |
| mbed-printf | X           | X              | X               | 22,560 | 5,880 |
| std printf  | X           | X              | X               | 35,244 | 5,864 |

Blinky application size on K64F/ARMC6

|             | File stream | Floating point | 64 bit integers | Flash  | RAM   |
| -           | -           | -              | -               | -      | -     |
| mbed-printf |             |                |                 | 19,496 | xxxxx |
| mbed-printf |             |                | X               | 19,644 | xxxxx |
| mbed-printf |             | X              | X               | 22,468 | xxxxx |
| mbed-printf | X           | X              | X               | 23,060 | xxxxx |
| std printf  | X           | X              | X               | 25,393 | xxxxx |

Blinky application size on K64F/IAR

|             | File stream | Floating point | 64 bit integers | Flash  | RAM    |
| -           | -           | -              | -               | -      | -      |
| mbed-printf |             |                |                 | 13,007 | 1,413  |
| mbed-printf |             |                | X               | 13,147 | 1,413  |
| mbed-printf |             | X              | X               | 14,955 | 1,413  |
| mbed-printf | X           | X              | X               | 21,757 | 1,929  |
| std printf  | X           | X              | X               | 17,991 | 1,281  |
