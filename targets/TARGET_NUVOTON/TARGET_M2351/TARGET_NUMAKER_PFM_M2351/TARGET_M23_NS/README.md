# Default secure code for M2351

M2351 is a TrustZone target which requires two codes: secure and non-secure.
Secure code would run first to set up secure environment and then bring up non-secure code.
`NuMaker-mbed-TZ-secure-example.hex` is the default secure code and `cmse_lib.o` is its accompanying
secure gateway library which exports secure functions to non-secure code. It is built from
[secure code template](https://github.com/OpenNuvoton/NuMaker-mbed-TZ-secure-example).
With secure/non-secure code possibly not starting at address `0x0`, `.hex` rather than `.bin` is
chosen as output format.

## Hardware partition

On TrustZone targets, it is necessary to partition the hardware first for secure/non-secure code
to run on secure/non-secure worlds respectively.

The default secure code has the following hardware partition:
- Flash (512KiB in total): 256KiB/256KiB to secure/non-secure respectively
- SRAM (96KiB in total): 32KiB/64KiB to secure/non-secure respectively
- Peripherals: most are configured to non-secure except the following which are hardwired or reserved:
    - **SYS/CLK** hardwired to secure. Accessible to non-secure through secure gateway
    - **FMC** hardwired to secure. Accessible to non-secure through secure gateway
    - **WDT** hardwired to secure. Accessible to non-secure through secure gateway
    - **RTC** configured to secure. Accessible to non-secure through secure gateway
    - **TMR0/1** hardwired to secure. Implement secure `us_ticker`/`lp_ticker` respectively
    - **TMR2/3** configured to non-secure. Implement non-secure `us_ticker`/`lp_ticker` respectively
    - **PDMA0** hardwired to secure. Implement secure asynchronous transfer
    - **PDMA1** configured to non-secure. Implement non-secure asynchronous transfer

## Flash secure/non-secure code

To flash secure/non-secure code on M2351, drag-n-drop `NuMaker-mbed-TZ-secure-example.hex`
first and then built user program e.g. [mbed-os-example-blinky.hex](https://github.com/ARMmbed/mbed-os-example-blinky).

## Reference

Refer to [secure code template](https://github.com/OpenNuvoton/NuMaker-mbed-TZ-secure-example) for details.