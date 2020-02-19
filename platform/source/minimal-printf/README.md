# Minimal printf and snprintf


Library supports both printf and snprintf in around 1300 bytes of flash.

Prints directly to stdio/UART without using malloc. All flags and precision modifiers are ignored.
There is no error handling if a writing error occurs.

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

Note that support for:
* 64b modifiers is only present when `minimal-printf-enable-64-bit` config is set to `true` (default).
* Floating point parameters is only present when `minimal-printf-enable-floating-point` config is set to `true` (disabled by default).

Unrecognized format specifiers are treated as ordinary characters.

Floating point limitations:
* All floating points are treated as %f.
* No support for inf, infinity or nan

## Usage

As of Mbed OS 6.0 this is enabled by default. To replace the standard implementation of the printf functions with the ones in this library for older versions of Mbed:

Modify your application configuration file to override the parameter `target.printf_lib` with the value `minimal-printf` as shown below:

```json
{
    "target_overrides": {
        "*": {
            "target.printf_lib": "minimal-printf"
        }
    }
}
```

If your application requires more advanced functionality, you'll need to revert to using standard version of printf/snprintf. Please note that it will result in significant ROM usage increase. In case you are using minimal version of standard C library advanced functionality may not be present.

Modify your application configuration in `mbed_app.json` file to override the parameter `target.printf_lib` with the value `std` as shown below:

```json
    "target_overrides": {
        "*": {
            "target.printf_lib": "std"
        }
    }
```

## Configuration


Minimal printf is configured by the following parameters defined in `platform/mbed_lib.json`:

```json
{
    "name": "platform",
    "config": {
        "minimal-printf-enable-64-bit": {
            "help": "Enable printing 64 bit integers when using minimal printf library",
            "value": true
        },
        "minimal-printf-enable-floating-point": {
            "help": "Enable floating point printing when using minimal printf library",
            "value": false
        },
        "minimal-printf-set-floating-point-max-decimals": {
            "help": "Maximum number of decimals to be printed when using minimal printf library",
            "value": 6
        }
    }
}
```

By default, 64 bit integers support is enabled, but floating point support is disabled to increase memory savings.

If your application needs to override the default configuration add following section to your `mbed_app.json`:
```json
    "target_overrides": {
        "*": {
            "target.printf_lib": "minimal-printf",
            "platform.minimal-printf-enable-floating-point": false,
            "platform.minimal-printf-set-floating-point-max-decimals": 6,
            "platform.minimal-printf-enable-64-bit": false
        }
    }
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
