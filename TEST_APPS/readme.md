## Running Icetea tests located under mbed-os

### Structure

Mbed OS has a folder called `TEST_APPS` that contains everything related to Icetea testing.

There are currently three folders:

- `device` - contains all the different test applications you can flash to your board.
- `icetea_plugins` - contains plugins that are being used by some of the testcases, needed for the test execution.
- `testcases` - contains Icetea testcases written in Python.

The testcases depend on test applications.

### Preparing your work environment

#### Prerequisites

You need Icetea and version 1.8.0 or higher of Mbed CLI installed.

#### Selecting the network interface to use

Depending on the device, there might be a default network interface type defined in `mbed-os/targets/targets.json`, which you can use to locate a default test-config file.

If the default network interface type is not defined or you want to use a different interface than the default, you need to provide a test-config file to the mbed test with `--test-config` option.

The test-config file contains the necessary information for the test application. There are some test-config files located under `mbed-os/tools/test_configs`.

Devices that have their network drivers residing inside `mbed-os` can use generic `test_configs`, such as `HeapBlockDeviceAndEthernetInterface.json` and `HeapBlockDeviceAndWifiInterface.json`. Otherwise, you need to use a device-specific test-config.

### Running the tests

Now that you have selected the interface, you can run the Icetea tests from the `mbed-os` root on your command-line by running the following command:

`mbed test -m <target> -t <toolchain> --icetea`

This command compiles the OS, compiles the test applications, creates a test suite and then starts running the tests.

If you only want to run some specific tests, you can use the `-n` option. You can choose multiple tests by separating them with a comma (`,`):

`mbed test -m <target> -t <toolchain> --icetea -n test1,test2`

#### Running the tests with a specific test-config

Some devices may offer multiple network interfaces to operate with. The tests can be run for either one of those using already existing test-config -files.

To run the tests with the Wi-Fi interface:

`mbed test -m <target> -t <toolchain> --icetea --test-config tools/test_configs/HeapBlockDeviceAndWifiInterface.json`

To run the tests with the ethernet interface:

`mbed test -m <target> -t <toolchain> --icetea --test-config tools/test_configs/HeapBlockDeviceAndEthernetInterface.json`

#### Providing Wi-Fi access point information

If you are using the Wi-Fi interface for running the tests, you need to also provide information about the used access point.

The information can be provided in the used test-config file.

Example of access point information:

```
    "target_overrides": {
        "*": {
            "target.network-default-interface-type": "WIFI",
            "nsapi.default-wifi-ssid": "\"ssid\"",
            "nsapi.default-wifi-password": "\"password\"",
            "nsapi.default-wifi-security": "WPA_WPA2"
        }
    }
```

### Test results

Icetea prints the results from the test run to the command-line, and the final result looks similar to this:

```
+--------------------------------+---------+-------------+-------------+-----------+----------+
| Testcase                       | Verdict | Fail Reason | Skip Reason | platforms | duration |
+--------------------------------+---------+-------------+-------------+-----------+----------+
| test_cmdline                   |   pass  |             |             |    K64F   |  8.555   |
| UDPSOCKET_BIND_PORT            |   pass  |             |             |    K64F   |  19.614  |
| TCPSOCKET_BIND_PORT            |   pass  |             |             |    K64F   |  15.852  |
| TCPSERVER_ACCEPT               |   pass  |             |             |    K64F   |  41.629  |
| TCPSOCKET_ECHOTEST_BURST_SHORT |   pass  |             |             |    K64F   |  19.926  |
+--------------------------------+---------+-------------+-------------+-----------+----------+
+---------------+----------------+
|    Summary    |                |
+---------------+----------------+
| Final Verdict |      PASS      |
|     count     |       5        |
|    passrate   |    100.00 %    |
|      pass     |       5        |
|    Duration   | 0:01:45.576000 |
+---------------+----------------+
```

You can also find the results from the tests in the `mbed-os/log` folder.

You probably want to add the log folder to your `.mbedignore` file to prevent issues with build commands becoming too long over the time.
