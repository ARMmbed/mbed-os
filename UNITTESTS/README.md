## Stubs, mocks and fakes in Mbed OS

To make unit testing easier, Mbed OS provides some ready-made CMake libraries you can add in your `target_link_libraries`. 
Libraries can be object files that contain stubs, mocks and fakes but also sets of include paths - this way you can gain access to normal headers from Mbed OS.

All the libraries available are in the `CMakeLists.txt` files in `mbed-os/UNITTESTS/stubs` and `mbed-os/UNITTESTS/fakes`.

The most common ones you might need are `mbed-os-fakes-ble` and `mbed-os-fakes-event-queue`.

### Fakes

While stubs are self explanatory and don't offer anything beyond empty implementations, fakes allow for more complex unittests that can simulate whole subsystems.

#### mbed-os-fakes-ble

This library provides a fake BLE implementation that uses mocks instead of real BLE components for `Gap`, `GattServer`, `GattClient`, `SecurityManager`.

There is no need to initialise a fake BLE instance; it is ready to go and can be used just like a normal BLE instance:

```c++
BLE *ble = &BLE::Instance();
```

This call also initialises mocks. 
Do no cache the BLE instance pointer, or pointer to `GAP`, `GattServer` etc. between tests. 
You must get the instance fresh at the start of the test.

You can retrieve all the BLE APIs from the instance just like with a normal one:

```c++
Gap &gap = ble->gap();
GattClient &client = ble->gattClient();
GattServer &server = ble->gattServer();
SecurityManager &sm = ble->securityManager();
```

Whenever an API call is made, the implementation will be called. 
These are replaced in the fake BLE with google mocks.
This means you can set expectations on them. 

```c++
EXPECT_CALL(ble::gap_mock(), reset());
```

This will set up an expectation that at some point during the test the `Gap::reset` method will be called.

The way GoogleTest works means that if you set expectations on your mocks they must be destroyed at the end of each test. 
This is done through the fake BLE instance special method:

```c++
ble::delete_mocks();
```

#### mbed-os-fakes-event-queue

This is a fake event queue that doesn't bring in any dependencies from mbed-os. 
Its API is simplified and it only offers limited functionality.

If you choose to use it you must not also include a library that brings in real headers for the event queue as they would conflict.

The API calls supported are for simple calls `call`, `call_in` and the `cancel` method.

The event queue is not run in real time and must be progressed manually. 
You may use `dispatch(int milliseconds)` and `dispatch_forever()` to process events in the queue. 
This way you can simulate the passage of time in your test.
