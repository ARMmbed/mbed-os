Wifi test plan
========================

This is a test plan for Mbed OS Wifi API.

Target API
----------

Target for this plan is to test [WiFiInterface](https://github.com/ARMmbed/mbed-os/blob/master/features/netsocket/WiFiInterface.h) class.

Tools to be used
----------------

-   Mbed OS
-   Secure WiFi access point
-   Unsecure WiFi access point

This test plan is written in tool agnostic way and do no specify how test cases should be written, build or run.

Test environment
----------------

General test environment consist of DUTs, base stations, network connection and test server.

![Wi-Fi](../../netsocket/wifi_environment.png)

-   All DUTS should be in RAAS
-   Two Wifi APs required
    -   non-secured one. SSID later referred as `<ssid:unsecure>`
    -   secured one: SSID later referred as `<ssid:secure>`, password referred as `<pw:secure>`
-   IPv4 and IPv6 must be routable between Wifi network IP address space and echo server.
-   Firewall must not block test traffic to and from test server **echo.mbedcloudtesting.com**
    -   Echo Protocol, [RFC 862](https://tools.ietf.org/html/rfc862) is enabled in both TCP and UDP. Port 7.
    -   Discard Protocol, [RFC 863](https://tools.ietf.org/html/rfc863) is enabled in both TCP and UDP. Port 9
    -   Character generator protocol, [RFC 864](https://tools.ietf.org/html/rfc864) is enabled in both TCP and UDP. Port 19
    -   Daytime protocol, [RFC 867](https://tools.ietf.org/html/rfc867) in both TCP and UDP. Port 13.
    -   Time protocol, [RFC 868](https://tools.ietf.org/html/rfc868) in both TCP and UDP. Port 37.
-   Channels to be used must be different for both APs. For secure on channel number is later referred as `<ch:secure>` and for unsecure on `<ch:unsecure>`
-   MAC addresses of Wifi APs must be known. Later referred as `<mac:secure>` and `<mac:unsecure>`

**NOTE:** Echo server is referred here as it is a requirement for running Socket API tests. It is not directly used by test cases defined in this document.

Test case priorities
--------------------

Please refer to following table for priorities of test cases. Priorities are labelled as MUST and SHOULD. MUST means this is a requirement and therefore mandatory to pass the test. SHOULD means it is recommended to pass the test if the driver implements the feature in question.

|     | Test case                               |                            | Priority |
|-----|-----------------------------------------|----------------------------|----------|
| 1   | WIFI_CONSTRUCTOR                        |                            | MUST     |
| 2   | WIFI_CONNECT_NOCREDENTIALS              |                            | MUST     |
| 3   | WIFI_SET_CREDENTIAL                     |                            | MUST     |
| 4   | WIFI_SET_CHANNEL                        |                            | SHOULD   |
| 5   | WIFI_GET_RSSI                           |                            | SHOULD   |
| 6   | WIFI_CONNECT_PARAMS_NULL                |                            | MUST     |
| 7   | WIFI_CONNECT_PARAMS_VALID_UNSECURE      |                            | MUST     |
| 8   | WIFI_CONNECT_PARAMS_VALID_SECURE        | With security type:        |          |
|     |                                         | NSAPI_SECURITY_WEP         | SHOULD   |
|     |                                         | NSAPI_SECURITY_WPA         | SHOULD   |
|     |                                         | NSAPI_SECURITY_WPA2        | SHOULD   |
|     |                                         | NSAPI_SECURITY_WPA_WPA2    | MUST     |
| 9   | WIFI_CONNECT_PARAMS_CHANNEL             |                            | SHOULD   |
| 10  | WIFI_CONNECT_PARAMS_CHANNEL_FAIL        |                            | SHOULD   |
| 11  | WIFI_CONNECT                            |                            | MUST     |
| 12  | WIFI_CONNECT_SECURE                     | With security type:        |          |
|     |                                         | NSAPI_SECURITY_WEP         | SHOULD   |
|     |                                         | NSAPI_SECURITY_WPA         | SHOULD   |
|     |                                         | NSAPI_SECURITY_WPA2        | SHOULD   |
|     |                                         | NSAPI_SECURITY_WPA_WPA2    | MUST     |
| 13  | WIFI_CONNECT_SECURE_FAIL                |                            | MUST     |
| 14  | WIFI_CONNECT_DISCONNECT_REPEAT          |                            | MUST     |
| 15  | WIFI_SCAN_NULL                          |                            | SHOULD   |
| 16  | WIFI_SCAN                               |                            | SHOULD   |

Building test binaries
----------------------

For testing the board and driver, test against the Mbed OS
master branch to get the most recent, up-to-date test cases and drivers.

To create a build environment:

```.sh
mbed new wifi_test
cd wifi_test
cd mbed-os
git checkout master
cd ..
```

Now build test binaries:

```.sh
mbed test --compile -t <toolchain> -m <target> -n mbed-os-tests-network-wifi
```

Running tests
-------------

Bear in mind that RAAS user and password have to be set in the shell to access it.

```.sh
mbedgt -g <target>:raas_client:ruka.mbedcloudtesting.com:8000 -n mbed-os-tests-network-wifi -V -v
```

Test cases for WifiInterface class
-------------------------------------

### WIFI_CONSTRUCTOR

**Description:**

 Test that constructor of the driver works.

**Preconditions:**

1.  None

**Test steps:**

1.  Construct the driver by calling the constructor.

**Expected result:**

Constructor returns.

### WIFI_CONNECT_NOCREDENTIALS

**Description:**

 Test `WiFiInterface::connect()` without parameters. Don't set parameters with `set_credentials()`

This must be a first test to run after constructing the driver. No credentials should be given for the driver before this test.

**Preconditions:**

1.  Test enviroment is set up as specified in "Test Environment" chapter.

**Test steps:**

1.  Initialise the driver
2.  Call `WiFiInterface::connect()`
3.  `disconnect()`

**Expected result:**

`connect()` call returns NSAPI_ERROR_PARAMETER or NSAPI_ERROR_NO_SSID

### WIFI_SET_CREDENTIAL

**Description:**

This test case is to test whether the driver accepts valid credentials and reject ones that are not valid.

**Preconditions:**

1.  Driver class is initialised.

**Test steps:**

1.  Call `set_credentials()` with various parameters described in "Expected results"

**Expected result:**

| Parameters | Password                  | Security                         | Expected return code                                |
|------------|---------------------------|----------------------------------|-----------------------------------------------------|
| ssid=NULL  | NULL                      | NSAPI_SECURITY_NONE              | NSAPI_ERROR_PARAMETER                               |
| ssid="OK"  | NULL                      | NSAPI_SECURITY_WPA_WPA2          | NSAPI_ERROR_PARAMETER                               |
| ssid="OK"  | NULL                      | NSAPI_SECURITY_WEP               | NSAPI_ERROR_PARAMETER because password is missing.  |
| ssid="OK"  | NULL                      | NSAPI_SECURITY_NONE              | NSAPI_ERROR_OK                                      |
| ssid="OK"  | [any 64 character string] | NSAPI_SECURITY_WPA2              | NSAPI_ERROR_PARAMETER because password is too long  |
| ssid="OK"  | [any 63 character string] | NSAPI_SECURITY_WPA2              | NSAPI_ERROR_OK                                      |
| ssid="OK"  | ""                        | NSAPI_SECURITY_WPA_WPA2          | NSAPI_ERROR_PARAMETER                               |
| ssid="OK"  | ""                        | NSAPI_SECURITY_WEP               | NSAPI_ERROR_PARAMETER because password is missing.  |
| ssid="OK"  | ""                        | NSAPI_SECURITY_NONE              | NSAPI_ERROR_OK                                      |
| ssid="OK"  | "12345678"                | NSAPI_SECURITY_WPA_WPA2          | NSAPI_ERROR_OK                                      |
| ssid="OK"  | "12345678"                | NSAPI_SECURITY_WPA2              | NSAPI_ERROR_OK                                      |
| ssid="OK"  | "12345678"                | NSAPI_SECURITY_WPA               | NSAPI_ERROR_OK                                      |
| ssid="OK"  | "12345678"                | NSAPI_SECURITY_WEP               | NSAPI_ERROR_OK or NSAPI_ERROR_UNSUPPORTED           |
| ssid=""    | ""                        | NSAPI_SECURITY_NONE              | NSAPI_ERROR_PARAMETER                               |

### WIFI_SET_CHANNEL

**Description:**

Test validity of `WiFiInterface::set_channel()`.

When proper channel number is feed, it should return NSAPI_ERROR_OK. API documentation is unclear what to report on error case, but I'm assuming NSAPI_ERROR_PARAMETER.

If the driver does not support setting channels for scan, it should report NSAPI_ERROR_UNSUPPORTED for all of the channels.

**Preconditions:**

1.  Driver class is initialised

**Test steps:**

1.  Call `set_channel()` with various parameters described in "Expected results"

**Expected result:**

For 2.4 Ghz chips

| channel | Expected return code                                                              |
|---------|-----------------------------------------------------------------------------------|
| 0       | NSAPI_ERROR_OK (0 = any channel)                                                  |
| 1       | NSAPI_ERROR_OK                                                                    |
| 13      | NSAPI_ERROR_OK if supporting European frequencies                                 |                  
|         | NSAPI_ERROR_PARAMETER if configured for North America                             |
| 15      | NSAPI_ERROR_PARAMETER                                                             |  
|         | because not a valid 2.4 Ghz channel                                               |

For 5 Ghz chips

| channel | Expected return code                                                              |
|---------|-----------------------------------------------------------------------------------|
| 3       | NSAPI_ERROR_PARAMETER                                                             |
|         | because not a valid 5 Ghz channel number                                          |
| 36      | NSAPI_ERROR_OK                                                                    |
| 169     | NSAPI_ERROR_PARAMETERs                                                            |                  
|         | should not be allowed in any country                                              |

Drivers not supporting

| channel | Expected return code                                                              |
|---------|-----------------------------------------------------------------------------------|
| <any>   | NSAPI_ERROR_UNSUPPORTED should be returned for ALL channel numbers > 0.           |

### WIFI_GET_RSSI

**Description:**

 Test `WiFiInterface::get_rssi()` API. When connected, it should return valid RSSI value. When unconnected it should return 0.

API is a bit unclear whether 0 can also mean that driver does not support reporting of RSSI.

**Preconditions:**

1.  Test enviroment is set up as specified in "Test Environment" chapter.

**Test steps:**

1.  Initialise the driver.
2.  Call `get_rssi()`, store the result.
3.  Call `connect()` with valid SSID parameters. Wait for connection
4.  Call `get_rssi()`

**Expected result:**

First call should return zero, second call should return negative number between -10 and -100.

### WIFI_CONNECT_PARAMS_NULL

**Description:**

Test `WiFiInterface::connect(ssid, pass, security, channel)` with NULL parameters

**Preconditions:**

1.  Test enviroment is set up as specified in "Test Environment" chapter.

**Test steps:**

1.  Initialise the driver
2.  Call `WiFiInterface::connect(NULL, NULL)`
3.  Call `WiFiInterface::connect("", "")`

**Expected result:**

Both `connect()` calls return NSAPI_ERROR_PARAMETER

### WIFI_CONNECT_PARAMS_VALID_UNSECURE

**Description:**

 Test `WiFiInterface::connect(ssid, pass, security)` with valid parameters for unsecure network

**Preconditions:**

1.  Test enviroment is set up as specified in "Test Environment" chapter.

**Test steps:**

1.  Initialise the driver
2.  Call `WiFiInterface::connect( <ssid:unsecure>, NULL)`
3.  `disconnect()`
4.  Call `WiFiInterface::connect( <ssid:unsecure>, "")`
5.  `disconnect()`

**Expected result:**

`connect()` calls return NSAPI_ERROR_OK

### WIFI_CONNECT_PARAMS_VALID_SECURE

**Description:**

 Test `WiFiInterface::connect(ssid, pass, security)` with valid parameters for secure network

**Preconditions:**

1.  Test enviroment is set up as specified in "Test Environment" chapter.

**Test steps:**

1.  Initialise the driver
2.  Call `WiFiInterface::connect( <ssid:secure>, <pw:secure>, NSAPI_SECURITY_WPA2)`
3.  `disconnect()`

**Expected result:**

`connect()` call returns NSAPI_ERROR_OK

 

### WIFI_CONNECT_PARAMS_CHANNEL

**Description:**

 Test `WiFiInterface::connect(ssid, pass, security, channel)` with valid parameters for secure network using channel specified.

This test only applies to devices which support selecting the channel (passes WIFI-SET-CHANNEL).

**Preconditions:**

1.  Test enviroment is set up as specified in "Test Environment" chapter.

**Test steps:**

1.  Initialise the driver
2.  Call `WiFiInterface::connect( <ssid:secure>, <pw:secure>, NSAPI_SECURITY_WPA2, <ch:secure>)`
3.  `disconnect()`

**Expected result:**

`connect()` call returns NSAPI_ERROR_OK

 

### WIFI_CONNECT_PARAMS_CHANNEL_FAIL

**Description:**

 Test `WiFiInterface::connect(ssid, pass, security, channel)` with valid parameters for secure network using channel specified. Channel specified must be wrong so that connect call should fail.

This test only applies to devices which support selecting the channel (passes WIFI-SET-CHANNEL).

**Preconditions:**

1.  Test enviroment is set up as specified in "Test Environment" chapter.

**Test steps:**

1.  Initialise the driver
2.  Call `WiFiInterface::connect( <ssid:secure>, <pw:secure>, NSAPI_SECURITY_WPA2, <ch:unsecure>)`
3.  `disconnect()`

**Expected result:**

connect() call returns NSAPI_ERROR_CONNECTION_TIMEOUT or NSAPI_ERROR_NO_CONNECTION

### WIFI_CONNECT

**Description:**

 Test `WiFiInterface::connect()` without parameters. Use `set_credentials()` for setting parameters. Checks that driver does not try to retrieve SSID from location it's not controlling.

**Preconditions:**

1.  Test enviroment is set up as specified in "Test Environment" chapter.

**Test steps:**

1.  Initialise the driver
2.  `Call WiFiInterface::set_credentials( <ssid:unsecure>, NULL)`
3.  `Call WiFiInterface::connect()`
4.  `disconnect()`
5.  `Call WiFiInterface::set_credentials( <ssid:unsecure>, "")`
6.  `Call WiFiInterface::connect()`
7.  `disconnect()`
8.  trash the memory storing SSID
9.  `Call WiFiInterface::set_credentials( <ssid:unsecure>, "")`
10. `Call WiFiInterface::connect()`
11. `disconnect()`

**Expected result:**

`connect()` calls return NSAPI_ERROR_OK

### WIFI_CONNECT_SECURE

**Description:**

 Test `WiFiInterface::connect()` without parameters. Use secure settings for `set_credentials`.

**Preconditions:**

1.  Test enviroment is set up as specified in "Test Environment" chapter.

**Test steps:**

1.  Initialise the driver
2.  Call `WiFiInterface::set_credentials( <ssid:secure>, <pw:secure>, NSAPI_SECURITY_WPA2)`
3.  Call `WiFiInterface::connect()`
4.  `disconnect()`

**Expected result:**

`connect()` call returns NSAPI_ERROR_OK

### WIFI_CONNECT_SECURE_FAIL

**Description:**

 Test `WiFiInterface::connect()` failing with wrong password.

**Preconditions:**

1.  Test enviroment is set up as specified in "Test Environment" chapter.

**Test steps:**

1.  Initialise the driver
2.  Call `WiFiInterface::set_credentials( <ssid:secure>, <any 8 character string, not pw:secure>, NSAPI_SECURITY_WPA2)`
3.  Call `WiFiInterface::connect()`
4.  disconnect()

**Expected result:**

`connect()` call returns NSAPI_ERROR_AUTH_FAILUR, NSAPI_ERROR_CONNECTION_TIMEOUT or NSAPI_ERROR_NO_CONNECTION

### WIFI_CONNECT_DISCONNECT_REPEAT

**Description:**

 Test `WiFiInterface::connect()` - `disconnect()` repeatition works.

**Preconditions:**

1.  Test enviroment is set up as specified in "Test Environment" chapter.

**Test steps:**

1.  Initialise the driver
2.  Call `WiFiInterface::set_credentials( <ssid:unsecure>, NULL)`
3.  Repeat 10 times
    1.  Call `WiFiInterface::connect()`
    2.  disconnect()

**Expected result:**

Each `connect()` call returns NSAPI_ERROR_OK

Each `disconnect()` returns NSAPI_ERROR_OK

### WIFI_SCAN_NULL

**Description:**

 Call `WiFiInterface::scan()` with null parameters to get number of networks available.

**Preconditions:**

1.  Test enviroment is set up as specified in "Test Environment" chapter.

**Test steps:**

1.  Initialise the driver
2.  Call `WiFiInterface::scan(NULL, 0)`;

**Expected result:**

`scan()` returns positive number that is higher or equivalent of two. (>=2).

### WIFI_SCAN

**Description:**

 Call `WiFiInterface::scan()` with valid accesspoint list allocated

**Preconditions:**

1.  Test enviroment is set up as specified in "Test Environment" chapter.
2.  Test environment must contain less than 10 WiFi SSID within the listening range of DUT, otherwise adjust the value used in step 2.

**Test steps:**

1.  Initialise the driver
2.  Allocate array of 10 WiFiAccessPoint objects.
3.  Call `WiFiInterface::scan(<array>, 10)`;

**Expected result:**

`scan()` returns positive number that is higher or equivalent of two. (>=2).

Scan elements from array indexes between zero and the number returned.

-   Call `get_ssid()` for each element. `<ssid:secure>` and `<ssid:unsecure>` must be found.
-   Call `get_rssid()` for each element. Value must be between -10 and -100.

For those known networks, test that following return values are found:

| get_ssid()              | get_bssid()            | get_security()        | get_channel()         |
|-------------------------|------------------------|-----------------------|-----------------------|
| `<ssid:unsecure>`       | `<mac:unsecure>`       | NSAPI_SECURITY_NONE   | `<ch:unsecure>`       |
| `<ssid:secure>`         | `<mac:secure>`         | NSAPI_SECURITY_WPA2   | `<ch:secure>`         |