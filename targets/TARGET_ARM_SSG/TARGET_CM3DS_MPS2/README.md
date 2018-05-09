# Cortex-M3 Design Start Eval package example on MPS2+ board

This folder includes the port of Mbed OS on the example system of the Cortex-M3
Design Start Eval package. This example is implemented in FPGA on the MPS2+
board. Please see this target's [Mbed page](https://os.mbed.com/platforms/ARM-CM3DS/)
for more information.
For convenience, this target is called **CM3DS**.

## Compiling

The target name is `ARM_CM3DS_MPS2`. You can compile Mbed OS
projects for CM3DS with:

```bash
mbed compile -t COMPILER -m ARM_CM3DS_MPS2
```

Mbed OS supports the following compilers (replace `COMPILER` with):

* `ARM` for Arm Compiler version 5.
* `GCC_ARM` for GNU Compiler for Arm.
* `IAR` for IAR compiler.

## Running

Because of the new memory configuration introduced in commit `CM3DS: switch to
larger memories for code and data`, it
has become easier (and portable among all compilers) to use `.elf` files
instead of `.bin`. `.elf` files are now the default for CM3DS projects, and compilation
generates only them.
For `.elf` files to work, you need **at least version 2.2.5** of the MPS2+
firmware. For more information, please see the [firmware version 2.2.6 and instructions on how to put it
in the MPS2+ board](https://community.arm.com/processors/designstart/f/discussions/9727/mps2-firmware-for-mbed).

## Testing

If you want to execute the Mbed OS greentea tests on CM3DS, you need
**at least firmware version 2.2.6**.

* `mbedls` does not automatically recognize which serial port is linked to the
board. Check it manually, and create a file named `mbedls.json` containing
(at the same level than where you execute all commands):

```bash
{
    "50040200074D652F3828F333": {
        "serial_port": "/dev/ttyUSB0"
    }
}
```

Replace `/dev/ttyUSB0` with your correct serial port
(something like `COM6` on Windows).

* `mbedls` does not link CM3DS target ID with its name, so execute the command:

```bash
mbedls --mock 5004:ARM_CM3DS_MPS2
```

* You can now compile and run the tests:

```bash
mbed test -m ARM_CM3DS_MPS2 -t COMPILER
```
