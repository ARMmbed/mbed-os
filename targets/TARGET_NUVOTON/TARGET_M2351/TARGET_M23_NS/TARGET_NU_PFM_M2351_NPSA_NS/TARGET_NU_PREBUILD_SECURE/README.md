The M2351 is a TrustZone target that requires two codes: secure and nonsecure. Secure code runs first to set up the secure environment and then brings up the nonsecure code. `NuMaker-mbed-TZ-secure-example.hex` is the pre-built secure image, and `cmse_lib.o` is its accompanying
secure gateway library, which exports secure functions to nonsecure code. It is built from the [non-PSA secure code template](https://github.com/OpenNuvoton/NuMaker-mbed-TZ-secure-example). Because secure and nonsecure code may start at an address other than `0x0`, we have chosen `.hex` rather than `.bin` as the output format.

## Hardware partition

On TrustZone targets, you need to partition the hardware first for secure code to run on secure world and nonsecure code to run on nonsecure world.

The pre-built secure code has the following hardware partition:

- Flash (512KiB in total): 256KiB for secure and 256KiB for nonsecure.
- SRAM (96KiB in total): 32KiB for secure and 64KiB for nonsecure.
- Peripherals: Most are configured to nonsecure except the following, which are hardwired or reserved:
    - **SYS/CLK** hardwired to secure. Accessible to nonsecure through a secure gateway.
    - **FMC** hardwired to secure. Accessible to nonsecure through a secure gateway.
    - **WDT** hardwired to secure. Accessible to nonsecure through a secure gateway.
    - **RTC** configured to secure. Accessible to nonsecure through a secure gateway.
    - **TMR0/1** hardwired to secure. TMR0 implements secure `us_ticker`, and TMR1 implements secure `lp_ticker`.
    - **TMR2/3** configured to nonsecure. TMR2 implements nonsecure `us_ticker`, and TMR3 implements nonsecure `lp_ticker`.
    - **PDMA0** hardwired to secure. Implements secure asynchronous transfer.
    - **PDMA1** configured to nonsecure. Implements nonsecure asynchronous transfer.

## Pre-built secure code files

-   NuMaker-mbed-TZ-secure-example.hex
    Pre-built secure image generated in non-PSA secure target build and to combine with non-secure image in non-PSA non-secure target post-build
-   cmse_lib.o
    Pre-built secure gateway library generated in non-PSA secure target build and to link with non-secure code in non-PSA non-secure target build

## Reference

Please refer to the [non-PSA secure code template](https://github.com/OpenNuvoton/NuMaker-mbed-TZ-secure-example) for details.
