# Rebuild TF-M and integrate with Mbed for M2354

This document guides how to rebuild TF-M and integrate with Mbed for M2354.

### Downloading TF-M source

The M2354 port in TF-M must patch to enable TF-M integration with Mbed.
For TF-M 1.4/Mbed integration for M2354, the [mainstream TF-M](https://git.trustedfirmware.org/TF-M/trusted-firmware-m.git) is patched as follows:
-   Apply Mbed-enabled patch to `nuvoton/m2354` TF-M target.

Run the following command to fetch and switch to the intended version:
```
$ git clone https://github.com/OpenNuvoton/trusted-firmware-m -b nuvoton_mbed_m2354_tfm-1.4
```

## Customizing TF-M

TF-M is out of scope of Mbed.
We only provide limited support for customizing M2354 port in TF-M.

### Default hardware partition

In TF-M, by default, the M2354 hardware is partitioned as follows:

- Flash (1024KiB in total): 448KiB for secure and 576KiB for nonsecure.
- SRAM (256KiB in total): 80KiB for secure and 176KiB for nonsecure.
- Peripherals: Most are configured to nonsecure except the following, which are hardwired or reserved:
    - **SYS/CLK**: Hardwired to secure. Accessible to Mbed through secure gateway.
    - **FMC**: Hardwired to secure. Inaccessible to Mbed.
    - **WDT**: Hardwired to secure. Inaccessible to Mbed.
    - **RTC**: Configured to secure. Accessible to Mbed through secure gateway.
    - **TMR0/1**: Hardwired to secure. Inaccessible to Mbed.
    - **TMR2/3**: Configured to nonsecure. TMR2 is used for TF-M NSPE test when enabled.
    - **TMR4/5**: Configured to nonsecure for Mbed `us_ticker`/`lp_ticker`.
    - **PDMA0**: Hardwired to secure. Inaccessible to Mbed.
    - **PDMA1**: Configured to nonsecure for Mbed asynchronous transfer.
    - **CRYPTO**: Configured to secure. Inaccessible to Mbed.
    - **TRNG**: Hardwired to secure. Accessible to Mbed indirectly through PSA Cryptography API.
    - **SDH**: Configured to secure to enable PSA Firmware Update. Inaccessible to Mbed.

### Defining Flash for TF-M/Mbed

To define memory spec of Flash for TF-M/Mbed, search/change the line:

**trusted-firmware-m/platform/ext/target/nuvoton/m2354/partition/flash_layout.h**:
```C
/* Max Flash size for TF-M + bootloader information */
#define FLASH_S_PARTITION_SIZE          (0x50000)
/* Max Flash size for Mbed + bootloader information */
#define FLASH_NS_PARTITION_SIZE         (0x90000)
```

M2354 has 1024KiB Flash in total, among which 128KiB have been allocated for bootloader code and ITS/PS storage.
896KiB are left for `FLASH_S_PARTITION_SIZE` and `FLASH_NS_PARTITION_SIZE`.

**NOTE**: `FLASH_S_PARTITION_SIZE` and `FLASH_NS_PARTITION_SIZE` must be sector size (2KiB)-aligned.

### Defining SRAM for TF-M/Mbed

To define memory spec of SRAM for TF-M/Mbed, search/change the line:

**trusted-firmware-m/platform/ext/target/nuvoton/m2354/partition/region_defs.h**:
```C
/* Max SRAM size for TF-M */
#define S_DATA_SIZE     (80 * 1024)
/* Max SRAM size for Mbed = Total - Max SRAM size for TF-M */
#define NS_DATA_SIZE    (TOTAL_RAM_SIZE - S_DATA_SIZE)
```

**NOTE**: `S_DATA_SIZE` and `NS_DATA_SIZE` must be 16KiB-aligned required by M2354 Security Configuration Unit (SCU).

## Rebuilding TF-M

### Environment

Navigate [TF-M](https://www.trustedfirmware.org/projects/tf-m/).
Then go through **DOCS** → **Getting Started Guides** → **Software requirements** for TF-M build environment setup.

**NOTE**: **GNU Arm Embedded Toolchain 9 2020-q2-update** and earlier built code **FAILS** to run with `-Os`. Avoid these toolchain versions. Check [their bug report](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=95646).

**NOTE**: **GNU Arm Embedded Toolchain 10-2020-q4-major** built code **FAILS** to run. Avoid this toolchain version. Check [its bug report](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=99157).

### Default relevant build configurations

-   Enabled BL2 (MCUboot)
-   Enabled multiple image boot
-   Enabled SDH (SD card) as update staging area for PSA Firmware Update

### Compiling

To compile TF-M for M2354, run:

```
$ cmake -S . \
-B cmake_build \
-DTFM_PLATFORM=nuvoton/m2354 \
-DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake \
-DTFM_PSA_API=ON \
-DCMAKE_BUILD_TYPE=Release \
-G"Unix Makefiles"
```

Then:

```
$ cmake --build cmake_build -- install
```

## Integrating with Mbed

### Updating TF-M exported stuff

The following TF-M exported stuffs must update into Mbed:

-   bl2.bin: [MCUboot](https://github.com/mcu-tools/mcuboot) bootloader binary

-   tfm_s.bin: TF-M secure binary

-   s_veneers.o: TF-M secure gateway library

-   partition/: Flash layout for image signing and concatenating in post-build process

    **NOTE**: On import, `signing_layout_*.o` are renamed to `signing_layout_*.h`.
    This is to use `.h` instead of `.c`/`.o` as file extension name and avoid adding into compile/link list.

    **NOTE**: On import for single image boot, `signing_layout_s_ns.o` is renamed to `signing_layout_preprocessed.h`.
    Post-build script checks file name with `_s`/`_ns` to resolve `sw_type` as `SPE`/`NSPE` respectively.
    To recognize as `NSPE_SPE`, don't use `_s_ns`/`_ns_s` file name to avoid mis-recognized.

-   [signing_key/](signing_key/nuvoton_m2354-root-rsa-3072.md)

Below summarize the copy paths from TF-M into Mbed:

-   trusted-firmware-m/cmake_build/install/outputs/NUVOTON/M2354/bl2.bin → bl2.bin
-   trusted-firmware-m/cmake_build/install/outputs/NUVOTON/M2354/tfm_s.axf → tfm_s.axf
-   trusted-firmware-m/cmake_build/install/outputs/NUVOTON/M2354/tfm_s.bin → tfm_s.bin
-   trusted-firmware-m/cmake_build/install/interface/lib/s_veneers.o → s_veneers.o
-   trusted-firmware-m/platform/ext/target/nuvoton/m2354/partition/flash_layout.h → partition/flash_layout.h
-   trusted-firmware-m/platform/ext/target/nuvoton/m2354/partition/region_defs.h → partition/region_defs.h
-   trusted-firmware-m/cmake_build/install/image_signing/layout_files/signing_layout_s.o → partition/signing_layout_s_preprocessed.h
-   trusted-firmware-m/cmake_build/install/image_signing/layout_files/signing_layout_ns.o → partition/signing_layout_ns_preprocessed.h
-   trusted-firmware-m/cmake_build/install/image_signing/keys/root-RSA-3072.pem → signing_key/nuvoton_m2354-root-rsa-3072.pem
-   trusted-firmware-m/cmake_build/install/image_signing/keys/root-RSA-3072_1.pem → signing_key/nuvoton_m2354-root-rsa-3072_1.pem

**NOTE**: `trusted-firmware-m/cmake_build/install/image_signing/keys/root-RSA-3072.pem` can be missing due to TF-M build tool issue.
Try to get it from `trusted-firmware-m/bl2/ext/mcuboot/root-RSA-3072.pem` instead if it is just the original source.

## PSA Firmware Update

### Requirement

-   SD card: Used for update staging area according to above TF-M build configurations.

### Update binary files

After finishing Mbed build, you will find the following files under build directory.
They are to write to update staging area through PSA Firmware Update API for firmware upgrade.

-   `tfm_s_update.bin`: TF-M secure binary file for update separately when multiple image boot is enabled
-   `<application>_update.bin`: Mbed non-secure binary file for update separately when multiple image boot is enabled or
    combined TF-M secure+Mbed non-secure binary file for update together when single image boot is enabled
