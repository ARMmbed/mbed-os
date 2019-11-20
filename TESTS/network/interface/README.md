Network interface test plan
========================

Target API
----------

The goal of this plan is to test [NetworkInterface.h](https://github.com/ARMmbed/mbed-os/blob/master/features/netsocket/NetworkInterface.h)

Tools needed
----------------

-   Mbed OS.
-   Standard Mbed OS development tools as described in the [Arm Mbed tools overview](https://os.mbed.com/docs/latest/tools/index.html).

Test environment
----------------

[Greentea](https://os.mbed.com/docs/mbed-os/latest/tools/greentea-testing-applications.html).

Test case priorities
--------------------

Please refer to the following table for priorities of test cases. Priorities
are labeled as MUST and SHOULD. MUST means this is a requirement and
therefore mandatory to pass the test. SHOULD means it is recommended to
pass the test if the driver implements the feature in question.

|     | Test case                               | Priority |
|-----|-----------------------------------------|----------|
| 1   | NETWORKINTERFACE_CONN_DISC_REPEAT       | MUST     |
| 2   | NETWORKINTERFACE_STATUS                 | MUST     |
| 3   | NETWORKINTERFACE_STATUS_NONBLOCK        | MUST     |
| 4   | NETWORKINTERFACE_STATUS_GET             | MUST     |

Building test binaries
----------------------

For testing the board and driver, test against the Mbed OS
master branch to get the most recent, up-to-date test cases and drivers.

To create a build environment:

```.sh
mbed new network_interface_test
cd network_interface_test
cd mbed-os
git checkout master
cd ..
```

Now build test binaries:

```.sh
mbed test --compile -t <toolchain> -m <target> -n mbed-os-tests-network-interface
```

Running tests
-------------

When device is connected to network, or if a wireless device is near
the access point:

```.sh
mbed test -n mbed-os-tests-network-interface
```

Test cases for NetworkInterface class
-------------------------------------

### NETWORKINTERFACE_CONN_DISC_REPEAT

**Description:**

Test `NetworkInterface::connect()` and `NetworkInterface::disconnect()`

**Precondition:**

Network interface is initialized.

**Test steps:**

1.  Connect interface.
2.  Disconnect interface.
3.  Repeat connect and disconnect steps 1 to 2 four times.

**Expected result:**

`Connect()` and `disconnect()` calls return `NSAPI_ERROR_OK`.

### NETWORKINTERFACE_STATUS

**Description:**

Test `NetworkInterface::attach()` and status indications.

**Preconditions:**

1.  Network interface is initialized.
2.  Status callback is attached to the network interface.
3.  Network interface is on blocking mode.

**Test steps:**

1.  Connect interface.
2.  Check that interface indicates status `NSAPI_STATUS_CONNECTING`.
3.  Optional: Check that interface indicates status `NSAPI_STATUS_LOCAL_UP` (whether this is indicated depends on network interface type).
4.  Check that interface indicates status `NSAPI_STATUS_GLOBAL_UP`.
5.  Disconnect interface.
6.  Check that interface indicates status `NSAPI_STATUS_DISCONNECTED`.
7.  Repeat connect and disconnect steps 1 to 6 four times.

**Expected result:**

Callback statuses are correct. `Connect()` and `disconnect()` calls return `NSAPI_ERROR_OK`. Note: The network interface may indicate the same status several times in a row, depending on the network interface type. This will not fail the test.

### NETWORKINTERFACE_STATUS_NONBLOCK

**Description:**

Test `NetworkInterface::attach()` and status indications on nonblocking mode.

**Preconditions:**

1.  Network interface is initialized.
2.  Status callback is attached to the network interface.
3.  Network interface is on nonblocking mode.

**Test steps:**

1.  Connect interface.
2.  Check that interface indicates status `NSAPI_STATUS_CONNECTING`.
3.  Optional: Check that interface indicates status `NSAPI_STATUS_LOCAL_UP` (whether this is indicated depends on network interface type).
4.  Check that interface indicates status `NSAPI_STATUS_GLOBAL_UP`.
5.  Disconnect interface.
6.  Check that interface indicates status `NSAPI_STATUS_DISCONNECTED`.
7.  Repeat connect and disconnect steps 1 to 6 four times.

**Expected result:**

Callback statuses are correct. `Connect()` and `disconnect()` calls return `NSAPI_ERROR_OK`. Note: The network interface may indicate the same status several times in a row, depending on the network interface type. This will not fail the test.

### NETWORKINTERFACE_STATUS_GET

**Description:**

Test `NetworkInterface::get_connection_status()`.

**Preconditions:**

1.  Network interface is initialized.
2.  Network interface is on blocking mode.

**Test steps:**

1.  Check that `get_connection_status()` returns status `NSAPI_STATUS_DISCONNECTED`.
2.  Connect interface.
3.  Poll the `get_connection_status()` until it returns status `NSAPI_STATUS_GLOBAL_UP`.
4.  (IPv6 only) Get IPv6 link local address using `get_ipv6_link_local_address` API.
5.  (IPv6 only) Check that `get_ipv6_link_local_address` returned status `NSAPI_ERROR_OK`.
6.  (IPv6 only) Check that the IP address associated with the Socket Address is not `NULL`.
7.  (IPv6 only) Check that the IP version of the IPv6 link local address is `NSAPI_IPv6`.
8.  Disconnect interface.
9.  Check that `get_connection_status()` returns status `NSAPI_STATUS_DISCONNECTED`.
10. Repeat connect and disconnect steps 2 to 5 four times.

**Expected result:**

`Connect()`, `get_ipv6_link_local_address` and `disconnect()` calls return `NSAPI_ERROR_OK`. The right status is returned by `get_connection_status()`. And the right IPv6 link local address is returned by `get_ipv6_link_local_address`.
