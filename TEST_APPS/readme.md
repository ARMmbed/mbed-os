## Running Icetea tests located under mbed-os

### Structure

mbed-os has a folder called TEST_APPS that contains everything related to Icetea testing.
There are currently 3 folders:

- device - contains all the different test applications you can flash to your board
- icetea-plugins - contains plugins that are being used by some of the testcases, needed for the test execution
- testcases - contains Icetea testcases written in Python

The testcases depends on test applications

### Preparing your work environment

#### Prerequisities

You need Icetea and mbed-cli that supports Icetea, installed.

#### Selecting the network interface to use

Depending on a device, there might be a default network interface type defined in the mbed-os/targets/targets.json, which is used to locate a test-config file by default. 
If there is not, or you want to use a different interface than the default, you need to provide a relevant test-config -file to the mbed test with --test-config option.
The test-config file contains the necessary information for the test application, there are some test-config files located under mbed-os/tools/test_configs.
Devices which have their network drivers residing inside mbed-os can use generic test_configs like HeapBlockDeviceAndEthernetInterface.json and HeapBlockDeviceAndWifiInterface.json. Otherwise you need to use a device specific test-config.

### Running the tests

Now that the interface has been selected you can run the icetea tests from the mbed-os root on your command line by

`>mbed test -m <target> -t <toolchain> --icetea`

This command will compile the mbed-os, then compiles the test applications, creates a test suite and then starts running the tests.

If you want only to run some specific tests, you can use the -n -option. You can list multiple tests by separating them by comma (,).

`>mbed test -m <target> -t <toolchain> --icetea -n test1,test2`

#### Running the tests with specifig test-config

Some devices may offer multiple network interfaces to operate with. For example, UBLOX_EVK_ODIN_W2 offers ethernet and Wi-Fi capabilities.
The tests can be run for either one of those using already existing test-config -files.

To run the tests with Wi-Fi interface:
`>mbed test -m UBLOX_EVK_ODIN_W2 -t <toolchain> --icetea --test-config tools/test_configs/HeapBlockDeviceAndWifiInterface.json`

To run the tests with ethernet interface:
`>mbed test -m UBLOX_EVK_ODIN_W2 -t <toolchain> --icetea --test-config tools/test_configs/HeapBlockDeviceAndEthernetInterface.json`

#### Providing Wi-Fi access point information

If you are using Wi-Fi interface for running the tests, you need to provide also information about the used access point.
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

Icetea prints the results from the test run to the command line, and the final result looks similar to this.

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

The results from the tests can also be found in the mbed-os/log folder.
You probably want to add the log folder to your .mbedignore file to prevent issues with build commands becoming too long over the time.
