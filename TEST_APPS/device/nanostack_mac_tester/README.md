# Nanostack MAC test application

You can use this application to test the Nanostack RF driver implementations that follow the [Nanostack RF driver porting instructions](https://os.mbed.com/docs/v5.6/reference/contributing-connectivity.html#porting-new-rf-driver-for-6lowpan-stack). The application has a command-line interface that is used to send commands to Nanostack's MAC layer, for example starting PANs, scanning or sending data. The provided tests do not test performance or stability and only indirectly test RF functionalities.

## Table of contents

* [Prerequisites](#prerequisites)
* [Setting up the application](#setting-up-the-application)
* [Application usage](#application-usage)
    * [Interactive mode](#interactive-mode)
    * [Automated mode](#automated-mode)
		* [Testcases](#testcases)
    * [Considerations](#considerations)

## Prerequisites

* [Mbed CLI](https://github.com/ARMmbed/mbed-cli).
* Mbed OS target board with build in radio, OR RF shield with Mbed OS driver

## Setting up the application

### Import the application

To import the application:

```
mbed import https://github.com/ARMmbed/nanostack-mac-tester.git
cd nanostack-mac-tester
```
### Add your RF driver

To add your RF driver:

```
mbed add https://www.github.com/me/my-rf-driver.git
```

When using RF shield, you need to configure it to be a default RF driver and instruct Mbed OS that RF driver is present. For example, configuring Atmel RF driver to provide default driver:

```
    "target_overrides": {
        "*": {
            "target.device_has_add": ["802_15_4_RF_PHY"],
            "atmel-rf.provide-default": true
```


### Compile your binary

Check that your choice of platform is supported with your choice of toolchain:

```
mbed compile --supported
```

<span class="notes">**Note:** Targets are often abbreviated from the full model names. In the case of `FRDM-K64F` the target is `K64F`.</span>

#### Set your target

To set your target HW:

```
mbed target TARGET
```

#### Set your toolchain

To set the toolchain:

```
mbed toolchain TOOLCHAIN
```

Currently, the supported toolchains are `GCC_ARM`, `ARM` and `IAR`.

#### Compile

To compile the application:

```
mbed compile
```

Alternately, you can put everything in a single line:

```
mbed compile -m TARGET -t TOOLCHAIN
```

## Application usage

You can use this application in interactive or automated mode.

### Interactive mode

To set the application to interactive mode:

1. Connect your board and copy the compiled application binary from the `BUILD/TARGET/TOOLCHAIN` folder to the board.
2. Open a serial connection with a program such as PuTTY, screen or minicom. The default baudrate is 115200.
3. Press the reset button on the board. The Arm Mbed logo and trace appears in the terminal window.

If the driver registration and SW MAC creation was successful, the application is ready to receive commands.

To start off, type `help` to list all commands available and furthermore `help <command>` to print detailed information about a specific command.

### Automated mode

To use the automated testcases, you first need to install the [Icetea test framework](https://github.com/ARMmbed/icetea).

After the framework is installed, you can run the test set in the root directory of this repository using the command `icetea`.
The following arguments can be used:
* `--tcdir testcases/` sets the target directory to search for testcases
* `--list` lists all testcases found in path, then exits
* `--tc all` specifies that all testcases should be run  
  `--tc test_name` or `--tc test_name1,test_name2` specify which testcases should be run
* `--repeat N` number of iterations on given set of testcases
* `--bin <path to binary>` the path to the binary to flash to the devices. Optional once the devices have been flashed once.
* `--reset` reset the devices before the next testcase(highly recommended)

Many of the provided testcases have a `self.channel` variable in the `setUp()` function for setting the RF channel. The default channel is 11. If you wish to run a test on another channel, you will need to change it manually.

Some testcases also use a secondary channel for transmitting and will use the next higher channel for that purpose. If the given channel is 26, the secondary channel will default to 25.

### Test cases

The automated test set runs the following testcases included in the repository under `testcases/`.
* [Create and join PAN](https://github.com/ARMmbed/nanostack-mac-tester/blob/master/testcases/create_and_join_PAN.py)
* [Direct data transmission](https://github.com/ARMmbed/nanostack-mac-tester/blob/master/testcases/send_data.py)
* [Indirect data transmission](https://github.com/ARMmbed/nanostack-mac-tester/blob/master/testcases/send_data_indirect.py)
* [ED scan](https://github.com/ARMmbed/nanostack-mac-tester/blob/master/testcases/ED_scan.py)
* [Address read and write](https://github.com/ARMmbed/nanostack-mac-tester/blob/master/testcases/address_write.py)
* [Large data transmission](https://github.com/ARMmbed/nanostack-mac-tester/blob/master/testcases/send_large_payloads.py)

### Considerations

* Devices need to be power cycled if they are unresponsive to test framework commands even after resetting.
* Devices can be enclosed in an RF isolation box to improve the consistency of test results.
* Some boards and radio modules come with a PCB trace antenna, instead of an SMD antenna, and need to be spaced further apart to decrease interference.