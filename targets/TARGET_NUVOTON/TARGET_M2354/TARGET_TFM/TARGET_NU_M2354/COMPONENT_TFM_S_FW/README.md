# TF-M exported stuff for Mbed integration

This directory contains TF-M exported stuff to integrate with Mbed.

## TF-M exported stuff

-   bl2.bin: [MCUboot](https://github.com/mcu-tools/mcuboot) bootloader binary

-   tfm_s.bin: TF-M secure binary

-   s_veneers.o: TF-M secure gateway library

-   partition/: Flash layout for image signing and concatenating in post-build process

    **NOTE**: On import, `signing_layout_s_ns.o` is renamed to `signing_layout_preprocessed.h` for the following reasons:
    -   Post-build script checks file name with `_s`/`_ns` to resolve `sw_type` as `SPE`/`NSPE` respectively.
        To recognize as `NSPE_SPE`, don't use `_s_ns`/`_ns_s` file name to avoid mis-recognized.
    -   Use `.h` instead of `.c` as file extension name.
        This is to enable custom TF-M build where the locatioin of this directory can change elsewhere.
        In Greentea build process, `.c` file isn't but`.h` file is copied into `BUILD` directory, so that post-build script can still access the file.

-   [signing_key/](signing_key/nuvoton_m2354-root-rsa-3072.md)

### Copy path summary

This section summarizes the copy paths from TF-M to Mbed.

-   trusted-firmware-m/cmake_build/bin/bl2.bin → bl2.bin
-   trusted-firmware-m/cmake_build/install/export/tfm/lib/s_veneers.o → s_veneers.o
-   trusted-firmware-m/cmake_build/bin/tfm_s.bin → tfm_s.bin
-   trusted-firmware-m/platform/ext/target/nuvoton/m2354/partition/flash_layout.h → partition/flash_layout.h
-   trusted-firmware-m/cmake_build/bl2/ext/mcuboot/CMakeFiles/signing_layout_s.dir/signing_layout_s_ns.o → partition/signing_layout_preprocessed.h
-   trusted-firmware-m/bl2/ext/mcuboot/root-RSA-3072.pem → signing_key/nuvoton_m2354-root-rsa-3072.pem

## Partitioning the hardware

In TF-M, the hardware is partitioned as follows:

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
    - **TRNG**: Hardwired to secure. Accessible to Mbed through secure gateway.

## Building TF-M

For Mbed/TF-M 1.2 integration, the mainstream TF-M is patched as follows:
-   Apply Mbed-enabled patch to `nuvoton/m2354` TF-M target.
-   Backport to base on [ARMmbed TF-M mbed-tfm-1.2](https://github.com/ARMmbed/trusted-firmware-m/tree/mbed-tfm-1.2).

To re-build TF-M, run:

```sh
$ cmake -S . \
-B cmake_build \
-DTFM_PLATFORM=nuvoton/m2354 \
-DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake \
-DTFM_PSA_API=ON \
-DTFM_ISOLATION_LEVEL=2 \
-G"Unix Makefiles"
```

```sh
$ cmake --build cmake_build -- install
```

**NOTE**: This patch has not been public yet.
