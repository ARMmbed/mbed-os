## Running IceTea tests located under mbed-os

### Structure

mbed-os has a folder called TEST_APPS that contains everything related to IceTea -testing.
There are currently 3 folders

- device - contains all the different test applications you can flash to your board
- icetea-plugins - contains plugins that are being used by some of the testcases, needed for the test execution
- testcases - contains IceTea testcases written in Python

The testcases and test applications have a dependency

### Preparing your work environment

#### Prerequisities

You should have IceTea and forked mbed-cli that supports IceTea, installed.

#### Selecting the network interface to use

Depending on a device, there might be a default network interface type defined in the mbed-os/targets/targets.json, which is used to locate a test-config file by default. 
If there is not, or you want to use a different interface than the default, you need to provide a relevant test-config -file to the mbed test with --test-config option.
The test-config file contains the necessary information for the test application, there are some test-config files located under mbed-os/tools/test-configs.

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
`>mbed test -m UBLOX_EVK_ODIN_W2 -t <toolchain> --icetea --test-config tools/test-configs/OdinInterface.json`

To run the tests with ethernet interface:
`>mbed test -m UBLOX_EVK_ODIN_W2 -t <toolchain> --icetea --test-config tools/test-configs/Odin_EthernetInterface.json`

#### Providing Wi-Fi access point information

If you are using Wi-Fi interface for running the tests, you need to provide also information about the used access point.
The information can be provided in the used test-config -file. Depending on the used interface you might need to provide ssid, password and security.
You can either provide separate WIFI_SSID, WIFI_PASSWORD and WIFI_SECURITY macros, or provide the SSID, password and security in the connect statement provided in the test-config -file.

Example of separate macros:
```
        "wifi-ssid": {
            "help": "WiFi SSID",
            "value": "\"ssid\"",
			"macro_name": "WIFI_SSID"
        },
        "wifi-password": {
            "help": "WiFi Password",
            "value": "\"password\"",
			"macro_name": "WIFI_PASSWORD"
        },
        "wifi-security": {
            "help": "WiFi Security, values from nsapi_security from features/netsocket/nsapi_types.h"
            "value": "NSAPI_SECURITY_WPA_WPA2"
            "macro_name": "WIFI_SECURITY"
```

Example of modifying the connect statement
Original:
```
        "connect-statement" : {
            "help" : "Must use 'net' variable name",
            "value" : "net->wifiInterface()->connect(WIFI_SSID, WIFI_PASSWORD, WIFI_SECURITY)"
        },
```
Modified:
```
        "connect-statement" : {
            "help" : "Must use 'net' variable name",
            "value" : "net->wifiInterface()->connect(\"ssid\", \"password\", NSAPI_SECURITY_WPA_WPA2)"
        },
```

### Test results

IceTea prints the results from the test run to the command line, and the final result looks similar to this.

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

The results from the tests can also be found from mbed-os/log -folder.
You probably want to add the log -folder to your .mbedignore -file to prevent issues with build commands becoming too long over the time.
