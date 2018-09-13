## Running Icetea tests located under `mbed-os`

### Structure

`mbed-os` has a folder called `TEST_APPS`, which contains everything related to Icetea testing.

There are currently three folders:

- `device` - contains all the different test applications you can flash to your board.
- `icetea-plugins` - contains plugins that some of the test cases use, which you need for the test execution.
- `testcases` - contains Icetea testcases written in Python.

The test cases depend on the test applications.

### Preparing your work environment

#### Prerequisities

You need to install Icetea and Mbed CLI, which supports Icetea.

#### Selecting the network interface to use

Depending on a device, there might be a default network interface type defined in the `mbed-os/targets/targets.json`, which you can use to locate a test-config file by default.

If there is not, or you want to use a different interface than the default, you need to provide a relevant test-config file to the mbed test with `--test-config` option.

The test-config file contains the necessary information for the test application, there are some test-config files located under `mbed-os/tools/test_configs`.

Devices whose network drivers reside inside `mbed-os` can use generic test-configs, such as `HeapBlockDeviceAndEthernetInterface.json` and `HeapBlockDeviceAndWifiInterface.json`. Otherwise, you need to use a device specific test-config.

### Running the tests

Now that you have selected the interface, you can run the Icetea tests from the `mbed-os` root on your command-line with:

`>mbed test -m <target> -t <toolchain> --icetea`

This command compiles `mbed-os`, then compiles the test applications, creates a test suite and starts running the tests.

If you want only to run some specific tests, you can use the `-n` -option. You can list multiple tests by separating them by commas (,):

`>mbed test -m <target> -t <toolchain> --icetea -n test1,test2`

#### Running the tests with specifig test-config

Some devices may offer multiple network interfaces to operate with. For example, UBLOX_EVK_ODIN_W2 offers ethernet and Wi-Fi capabilities.

You can run the tests for either one of those using already existing test-config -files.

To run the tests with Wi-Fi interface:

`>mbed test -m UBLOX_EVK_ODIN_W2 -t <toolchain> --icetea --test-config tools/test_configs/HeapBlockDeviceAndWifiInterface.json`

To run the tests with ethernet interface:

`>mbed test -m UBLOX_EVK_ODIN_W2 -t <toolchain> --icetea --test-config tools/test_configs/HeapBlockDeviceAndEthernetInterface.json`

#### Providing Wi-Fi access point information

If you are using a Wi-Fi interface for running the tests, you need to provide information about the used access point. You can provide the information in the test-config file.

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

You can also find the results from the tests in the `mbed-os/log` folder. You probably want to add the log folder to your `.mbedignore` file to prevent issues with build commands becoming too long over the time.
