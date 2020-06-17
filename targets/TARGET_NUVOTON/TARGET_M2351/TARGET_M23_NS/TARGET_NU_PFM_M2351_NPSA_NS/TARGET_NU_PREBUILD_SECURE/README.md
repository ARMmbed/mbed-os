The M2351 is a TrustZone target that requires two programs: secure and nonsecure.
Secure program runs first to set up the secure environment and then brings up the nonsecure program. `NuMaker-mbed-TZ-secure-example.hex` is the pre-built secure image, and `cmse_lib.o` is its accompanying secure gateway library, which exports secure functions to nonsecure program.
It is built from the [non-PSA secure program template](https://github.com/OpenNuvoton/NuMaker-mbed-TZ-secure-example). Because secure program and nonsecure program may start at an address other than `0x0`, we have chosen `.hex` rather than `.bin` as the output format.

## Hardware partition

On TrustZone targets, you need to partition the hardware first for secure program to run on secure world and nonsecure program to run on nonsecure world.

The pre-built secure program will set up the following hardware partition:

- Flash (512KiB in total): 64KiB for secure and 448KiB for nonsecure.
- SRAM (96KiB in total): 8KiB for secure and 88KiB for nonsecure.
- Peripherals: Most are configured to nonsecure except the following, which are hardwired or reserved:
    - **SYS/CLK** hardwired to secure. Accessible to nonsecure through a secure gateway.
    - **FMC** hardwired to secure. Accessible to nonsecure through a secure gateway.
    - **WDT** hardwired to secure. Accessible to nonsecure through a secure gateway.
    - **RTC** configured to secure. Accessible to nonsecure through a secure gateway.
    - **TMR0/1** hardwired to secure. TMR0 implements secure `us_ticker`, and TMR1 implements secure `lp_ticker`.
    - **TMR2/3** configured to nonsecure. TMR2 implements nonsecure `us_ticker`, and TMR3 implements nonsecure `lp_ticker`.
    - **PDMA0** hardwired to secure. Implements secure asynchronous transfer.
    - **PDMA1** configured to nonsecure. Implements nonsecure asynchronous transfer.
    - **TRNG** configured to secure. Accessible to nonsecure through a secure gateway.

In this memory partition, secure program is the most simplified.
Then non-secure program can make use of the most memory available for its large application like Pelion.

To regenerate the default secure program, in [non-PSA secure program template](https://github.com/OpenNuvoton/NuMaker-mbed-TZ-secure-example), run:

```sh
$ mbed compile -m NU_PFM_M2351_NPSA_S -t ARMC6 --profile release
```

## Pre-built secure program files

-   NuMaker-mbed-TZ-secure-example.hex
    Pre-built secure image generated in non-PSA secure target build and to combine with non-secure image in non-PSA non-secure target post-build
-   cmse_lib.o
    Pre-built secure gateway library generated in non-PSA secure target build and to link with non-secure program in non-PSA non-secure target build

## Reference

Please refer to the [non-PSA secure program template](https://github.com/OpenNuvoton/NuMaker-mbed-TZ-secure-example) for details.
