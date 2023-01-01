# Nuvoton M460 series

## HyperRAM

HyperRAM, via Hyper Bus Interface Controller (HBI), is mapped to two regions: `0x0A00_0000`–`0x0BFF_FFFF` and `0x8000_0000`–`0x81FF_FFFF`, through which CPU can direct access.

### Enable HBI for HyperRAM

To enable HBI for HyperRAM, create one Mbed OS application configuration file named `mbed_app.json` under root directory of your sample program.
The file should have the following content which will enable HBI and configure its multi-function pins:

**mbed_app.json**:

```json
{
    ......
    "target_overrides": {
        ......
        "NUMAKER_IOT_M467": {
            "target.hbi-enable": true,
            "target.hbi-mfp-reg-list": "0x40000534, 0x4000057C, 0x40000590, 0x40000594",
            "target.hbi-mfp-reg-msk-list": "0xFFFFFF00, 0xFFFFFFFF, 0xFFFF0000, 0xFFFFFFFF",
            "target.hbi-mfp-reg-val-list": "0x10101000, 0x10101010, 0x10100000, 0x10101010"
        }
    }
}
```

Where:

-   `hbi-mfp-reg-list`: Comma-separated list of relevant multi-function pin registers
-   `hbi-mfp-reg-msk-list`: Comma-separated list of relevant multi-function pin register masks
-   `hbi-mfp-reg-val-list`: Comma-separated list of relevant multi-function pin register values

> **_NOTE:_** Make sure your HyperRAM hardware is ready.

> **_NOTE:_** Change the multi-function pins to align with your hardware.

### Locate data at HyperRAM

In the port, the region `0x8000_0000`–`0x81FF_FFFF` is chosen for the goal because on Mbed OS, MPU is enabled and the region is configured to be write-able, which naturally fits HyperRAM.
To get to the goal, data must place in specified sections:

**Locate initialized data**

Initialized data are just like `RW` sections for ARM, `.data` sectionis for GCC_ARM, and `readwrite` sections for IAR.
To locate them at HyperRAM, place in specified sections named `.data.nu.hyperram`.

```c++
__attribute__((section(".data.nu.hyperram")))
uint32_t my_data = 100;
```

**Locate zero-initialized data**

Zero-initialized data are just like `ZI` sections for ARM, `.bss` sections for GCC_ARM, and `zeroinit` sections for IAR.
To locate them at HyperRAM, place in specified sections named`.bss.nu.hyperram`.

```c++
__attribute__((section(".bss.nu.hyperram")))
uint32_t my_bss;
```

> **_NOTE:_** Don't mis-place in sections,
or behavior is undefined because C runtime is not correctly initialized for these sections.

> **_NOTE:_** Check linker-generated .map file to make sure your data are actually located at HyperRAM.
