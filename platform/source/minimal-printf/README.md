# Minimal printf and snprintf


Library supports both printf and snprintf in 1252 bytes of flash.

Prints directly to stdio/UART without using malloc. All flags and precision modifiers are ignored.
There is no error handling if a writing error occurs.

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

```json
{
    "name": "platform",
    "config": {
        "minimal-printf-enable-64-bit": {
            "help": "Enable printing 64 bit integers when using minimal-printf profile",
            "value": true
        },
        "minimal-printf-enable-floating-point": {
            "help": "Enable floating point printing when using minimal-printf profile",
            "value": false
        },
        "minimal-printf-set-floating-point-max-decimals": {
            "help": "Maximum number of decimals to be printed",
            "value": 6
        }
   }
}
```

By default, 64 bit integers support is enabled.

If your target does not require some options then you can override the default configuration in your application `mbed_app.json` and achieve further memory optimisation (see next section for size comparison numbers).

In mbed_app.json:

```json
    "target_overrides": {
        "*": {
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

|             | Floating point | 64 bit integers | Flash  | RAM    |
| -           | -              | -               | -      | -      |
| mbed-printf |                |                 | 32,972 | 11,608 |
| mbed-printf |                | X               | 33,116 | 11,608 |
| mbed-printf | X              | X               | 35,856 | 11,608 |
| std printf  | X              | X               | 55,766 | 12,104 |

Blinky application size on K64F/ARMC6

|             | Floating point | 64 bit integers | Flash  | RAM   |
| -           | -              | -               | -      | -     |
| mbed-printf |                |                 | 33,585 | xxxxx |
| mbed-printf |                | X               | 33,679 | xxxxx |
| mbed-printf | X              | X               | 36,525 | xxxxx |
| std printf  | X              | X               | 39,128 | xxxxx |

Blinky application size on K64F/IAR

|             | Floating point | 64 bit integers | Flash  | RAM    |
| -           | -              | -               | -      | -      |
| mbed-printf |                |                 | 31,439 | 8,493  |
| mbed-printf |                | X               | 31,579 | 8,493  |
| mbed-printf | X              | X               | 33,387 | 8,493  |
| std printf  | X              | X               | 36,643 | 8,553  |

### Blinky bare metal application

https://github.com/ARMmbed/mbed-os-example-blinky-baremetal application compiled with the different toolchains.

Blinky application size on K64F/GCC_ARM

|             | Floating point | 64 bit integers | Flash  | RAM   |
| -           | -              | -               | -      | -     |
| mbed-printf |                |                 | 19,660 | 5,368 |
| mbed-printf |                | X               | 19,804 | 5,368 |
| mbed-printf | X              | X               | 22,548 | 5,368 |
| std printf  | X              | X               | 35,292 | 5,864 |

Blinky application size on K64F/ARMC6

|             | Floating point | 64 bit integers | Flash  | RAM   |
| -           | -              | -               | -      | -     |
| mbed-printf |                |                 | 18,764 | xxxxx |
| mbed-printf |                | X               | 18,764 | xxxxx |
| mbed-printf | X              | X               | 18,764 | xxxxx |
| std printf  | X              | X               | 25,403 | xxxxx |

Blinky application size on K64F/IAR

|             | Floating point | 64 bit integers | Flash  | RAM    |
| -           | -              | -               | -      | -      |
| mbed-printf |                |                 | 19,623 | 1,737  |
| mbed-printf |                | X               | 19,763 | 1,737  |
| mbed-printf | X              | X               | 21,571 | 1,737  |
| std printf  | X              | X               | 18,059 | 1,281  |
