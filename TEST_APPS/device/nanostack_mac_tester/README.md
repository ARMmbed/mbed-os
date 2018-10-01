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

### Add your RF driver

When using RF shield, you need to configure it to be a default RF driver and instruct Mbed OS that RF driver is present. For example, configuring Atmel RF driver to provide default driver:

```
    "target_overrides": {
        "*": {
            "target.device_has_add": ["802_15_4_RF_PHY"],
            "atmel-rf.provide-default": true
```

### Checking platform support

Check that your choice of platform is supported by your choice of toolchain:

```
mbed compile --supported
```

<span class="notes">**Note:** Targets are often abbreviated from the full model names. In the case of `FRDM-K64F` the target is `K64F`.</span>

## Run tests

You can use this application in interactive or automated mode.

### Interactive mode

To set the application to interactive mode:

1. Build the application.
```
mbed test --compile --icetea -t TOOLCHAIN -m TARGET_PLATFORM -DICETEA_MAC_TESTER_ENABLED --test-config NANOSTACK_MAC_TESTER -n address_read_and_write,send_data,send_data_indirect,send_large_payloads,create_and_join_PAN,ED_scan
```
2. Connect your board and copy the compiled application binary from the `BUILD/tests/TARGET_PLATFORM/TOOLCHAIN/TEST_APPS/device/nanostack_mac_tester/` folder to the board.
3. Wait for the device to flash the binary.
4. Open a serial connection with a program such as PuTTY, screen or minicom. The default baudrate is 115200.
5. Press the reset button on the board. The Arm Mbed logo and trace appears in the terminal window.

If the driver registration and SW MAC creation was successful, the application is ready to receive commands.

To start off, type `help` to list all commands available and furthermore `help <command>` to print detailed information about a specific command.

### Automated mode

```
mbed test --clean --compile --run --icetea -t TOOLCHAIN -m TARGET_PLATFORM -DICETEA_MAC_TESTER_ENABLED --test-config NANOSTACK_MAC_TESTER -n address_read_and_write,send_data,send_data_indirect,send_large_payloads,create_and_join_PAN,ED_scan
```

Many of the provided testcases have a `self.channel` variable in the `setUp()` function for setting the RF channel. The default channel is 11. If you wish to run a test on another channel, you will need to change it manually in TEST_APPS/testcases/nanostack_mac_tester/.

Some testcases also use a secondary channel for transmitting and will use the next higher channel for that purpose. If the given channel is 26, the secondary channel will default to 25.

### Test cases

The automated test set runs the following testcases included in the repository under `TEST_APPS/testcases/nanostack_mac_tester/`.
* Create and join PAN
* Direct data transmission(Transmission between two devices)
* Indirect data transmission(Transmission between two devices with one device acting as intermediary)
* ED scan(Energy Density scanning)
* Address read and write
* Large data transmission

### Considerations

* Devices need to be power cycled if they are unresponsive to test framework commands even after resetting.
* Devices can be enclosed in an RF isolation box to improve the consistency of test results.
* Some boards and radio modules come with a PCB trace antenna, instead of an SMD antenna, and need to be spaced further apart to decrease interference.