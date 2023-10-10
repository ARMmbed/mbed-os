# Development moved

The development of Htrun has been moved into the [mbed-os-tools](../../src/mbed_os_tools) package. You can continue to use this module for legacy reasons, however all further development should be continued in the new package.

-------------

[![PyPI version](https://badge.fury.io/py/mbed-host-tests.svg)](https://badge.fury.io/py/mbed-host-tests)

# Htrun (mbed-host-tests)

`htrun` has extensive command line. In most cases `htrun` (or its command line avatar `mbedhtrun`) will be run in background:
* driving test binary flashing,
* device reset and
* test execution.

Default binary flashing method is one supported by [mbed-enabled](https://www.mbed.com/en/about-mbed/mbed-enabled/) devices: binary file is copied on mbed-enabled DUT (Device Under Test) mounted drive (MSD). This procedure will automatically flash device with given binary file content.

Default DUT reset method is one supported by [mbed-enabled](https://www.mbed.com/en/about-mbed/mbed-enabled/) devices: serial port (CDC) "*sendBreak*" command resets target MCU on mbed-enabled platform.

Test execution phase will consist of:
* Opening connection between host computer and DUT,
* DUT will send to host preamble with test runner information such as:
  * test environment version,
  * test timeout,
  * preferred host test script (Python script which is used to supervise/instrument test execution),
* Host will spawn host test script and test execution will be instrumented
* Exchange data (in most cases text) between host and DUT,


## Command line overview

This chapter will present few examples of how you can use `mbedhtrun` command line to execute tests. In most cases test automation tools such as [Greentea](https://github.com/ARMmbed/greentea) will execute `mbedhtrun` implicitly. There are cases when we want to execute `mbedhtrun` independently. Mostly in situation when we want to:
* debug our code and have binary + host test instrumentation on,
* prototype or
* just want to replace test runner in another OS with one compatible with mbed-enabled devices.

All `mbedhtrun` needs is name of the binary you want to flash and method of flashing!

## Useful command line end-to-end examples

### Flashing phase operations

Flash binary file `/path/to/file/binary.bin` using mount point `D:`. Use serial port `COM4` to communicate with DUT:

```
$ mbedhtrun -f /path/to/file/binary.bin -d D: -p COM4
```

Flash (use shell command `copy`) binary file `/path/to/file/binary.bin` using mount point `D:`. Use serial port `COM4` to communicate with DUT:

```
$ mbedhtrun -f /path/to/file/binary.bin -d D: -p COM4 -c copy
```

Skip flashing phase (e.g. you've already flashed this device with `/path/to/file/binary.bin` binary). Use serial port `COM4` to communicate with DUT:

```
$ mbedhtrun -f /path/to/file/binary.bin -d D: -p COM4 --skip-flashing
```

### DUT-host communication and reset phase

Flash binary file `/path/to/file/binary.bin` using mount point `D:`. Use serial port `COM4` with baudrate `115200` to communicate with DUT:

```
$ mbedhtrun -f /path/to/file/binary.bin -d D: -p COM4:115200
```

As above but we will skip reset phase (non so common but in some cases can be used to suppress reset phase for some reasons):

```
$ mbedhtrun -f /path/to/file/binary.bin -d D: -p COM4:115200 --skip-reset
```

Flash binary file `/path/to/file/binary.bin` using mount point `D:`. Use serial port `COM4` with default baudrate to communicate with DUT. Do not send `__sync` key-value protocol synchronization packet to DUT before preamble read:

```
$ mbedhtrun -f /path/to/file/binary.bin -d D: -p COM4 --sync=0
```

**Note**: Sync packet management allows you to manipulate the way `htrun` sends `__sync` packet(s) to DUT. With current settings we can force on `htrun` to send `__sync` packets in this manner:
* `--sync=0` - No sync packets will be sent to DUT.
* `--sync=-1`- `__sync` packets will be sent unless we will reach timeout or proper response is sent from DUT.
* `--sync=N` - Where N is integer > 0. Send up to N `__sync` packets to target platform. Response is sent unless we get response from target platform or timeout occurs.

### Global Resource Manager connection

Flash local file `/path/to/file/binary.bin` to remote device resource (platform `K64F`) provided by `remote_client` GRM service available on IP address `10.2.203.31` and port: `8000`. Force serial port connection to remote device `9600` with baudrate:

```
$ mbedhtrun -p :9600 -f /path/to/file/binary.bin -m K64F --grm remote_client:10.2.203.31:8000
```

Command line switch `--grm` has format: `<module_name>:<IP_address>:<port_number>`.
  * `<module_name>` - name of Python module to load as remote resource manager.
  * `<IP_address>` and `<port_number>` - IP address and port of remote resource manager.

**Note**: Switch -m <platform_name> is required to tell Global Resource Management which platform to request.
**Note**: Command line switch `--grm` implicitly forces `--skip-flashing` and `--skip-reset` because both flags are used for locally available DUTs.

### Fast Model connection

This option is designed for htrun to use Arm Fast Models.

The "--fm" option only available when [mbed-fastmodel-agent](https://github.com/ARMmbed/mbed-fastmodel-agent) module is installed :

Load local file `/path/to/file/binary.elf` to onto fastmodel FVP_MPS2_m3 simulators:

```
$ mbedhtrun -f /path/to/file/binary.elf -m FVP_MPS2_M3 --fm DEFAULT
```

Command line switch format `--fm <config_name>`.
  * `<config_name>` - ether pre-defined CONFIG_NAME from mbedfm or a local config file for the Fast Models.

**Note**: Switch -m <platform_name> is required to tell this fastmodel connection which Fastmodel to request.
**Note**: Command line switch `--fm` implicitly forces `--skip-flashing` and `--skip-reset` because both flags are used for locally available DUTs.


### Miscellaneous

List available host tests names, class names and origin:

```
$ mbedhtrun --list
```

List available host tests names, class names and origin. Load additional host tests from `/path/to/host_tests` directory:

```
$ mbedhtrun --list -e /path/to/host_tests
```

List available reset and flashing plugins:

```
$ mbedhtrun --plugins
```

Flash binary file `/path/to/file/binary.bin` using plugin `stlink`. Use serial port `COM4` with baudrate `115200` to communicate with DUT:

```
mbedhtrun -c stlink -f /path/to/file/binary.bin -p COM4:115200
```

# Installation

`htrun` is redistributed with sources, as Python 2.7 compatible module called `mbed-host-tests` and command line tool called `mbedhtrun`.

## Installation from PyPI (Python Package Index)
`mbed-host-tests` module is redistributed via PyPI. We recommend you use the [application pip](https://pip.pypa.io/en/latest/installing.html#install-pip).

**Note:** Python 2.7.9 onwards include ```pip``` by default, so you may have ```pip``` already.
**Note:** `mbed-host-tests` module is redistributed with `mbed-greentea` module as a dependency. So if you've already installed Greentea `mbed-host-tests` should be there!

To install mbed-ls from [PyPI](https://pypi.python.org/pypi/mbed-host-tests) use command:

```
$ pip install mbed-host-tests --upgrade
```

## Installation from Python sources
To install the mbed test suite, first clone the `mbed-os-tools` repository:

```
$ git clone https://github.com/ARMmbed/mbed-os-tools.git
```

Change the directory to the `mbed-os-tools/packages/mbed-host-tests` directory:

```
$ cd mbed-os-tools/packages/mbed-host-tests
```

Now you are ready to install `htrun`:

```
$ python setup.py install
```

### Checking installation
To check whether the installation was successful try running the ```mbedgt --help``` command and check that it returns information (you may need to restart your terminal first):

```
$ mbedhtrun --help
Usage: mbedgt-script.py [options]

Flash, reset and perform host supervised tests on mbed platforms

Options:
  -h, --help            show this help message and exit
```

# mbed-host-tests

mbed's test suite (codenamed ```Greentea```) supports the *test supervisor* concept. This concept is realized by this module. ```mbed-host-tests``` is a collection of host tests. Host test is script written in Python, which is executed in parallel with the test suite runner (a binary running on the target hardware / device under test) to monitor the test execution's progress or to control the test flow (interaction with the mbed device under test - DUT). The host test is also responsible for grabbing the test result, or deducing it from the test runner's behavior.

Key-value protocol was developed and is used to provide communication layer between DUT (device under test) and host computer. Key-value protocol defined host computer as master and DUT as slave.
* Slave side APIs and key-value protocol implementation is encapsulated in [greentea-client](https://github.com/ARMmbed/mbed-os/tree/master/features/frameworks/greentea-client) module.
* Master side APIs and key-value protocol is encapsulated in  ```mbed-host-tests```.

```mbed-host-tests``` responsibilities are:
* Flash mbed device with given binary.
* Reset mbed device after flashing to start test suite execution.
* Use key-value protocol to handshake with device and make sure correct host test script is executed to supervise test suite execution.
* Run key-value protocol state machine and execute event callbacks.
* Monitor serial port traffic to parse valid key-value protocol events.
* Make decision if test test suite passed / failed / returned error.
* Provide command line tool interface, command: ```mbedhtrun``` after module installation (on host).
* Provide few basic host test implementations which can be used out of the box for test development. For example the basic host test (called ```default``` or ```default_auto```) just parses events from DUT and finished host test execution when ```end``` event is received. Other included in this module host tests can help you to test timers or RTC.

## Key-value protocol overview

* Text based protocol, format ```{{KEY;VALUE}}}```.
* Master-slave mode where host is master and DUT is slave.

## Design draft
* Simple key-value protocol is introduced. It is used to communicate between DUT and host. Protocol main features:
* Protocol introduced is master-slave protocol, where master is host and slave is device under test.
* Transport layer consist of simple ```{{ KEY ; VALUE }} \n``` text messages sent by slave (DUT). Both key and value are strings with allowed character set limitations (to simplify parsing and protocol parser itself). Message ends with required by DUT K-V parser `\n` character.
* DUT always (except for handshake phase) initializes communication by sending key-value message to host.
* To avoid miscommunication between master and slave simple handshake protocol is introduces:
    * Master (host) sends sync packet: ```{{__sync;UUID-STRING}}}``` with message value containing random UUID string.
    * DUT waits for ```{{__sync;...}}``` message in input stream and replies with the same packer ```{{__sync;...}}```.
    * After correct sync packet is received by master, messages ```{{__timeout;%d}}``` and ```{{__host_test_name}}``` are expected.
  * Host parses DUTs tx stream and generates events sent to host test.
  * Each event is a tuple of ```(key, value, timestamp)```, where key and value are extracted from message and
* Host tests are now driven by simple async feature. Event state machine on master side is used to process events from DUT. Each host test is capable of registering callbacks, functions which will be executed when event occur. Event name is identical with KEY in key-value pair send as event from/to DUT.
* DUT slave side uses simple parser to parse key-value pairs from stream. All non key-value data will be ignored. Blocking wait for an event API is provided: This implies usage of master-slave exchange between DUT and host where DUT uses non-blocking send event API to send to host (master) event and can wait for response. Master implements corresponding response after receiving event and processing data.
  * Message parsing transforms key-value string message to Python event in this order:
    * ```{{key;value}}``` string captured on DUT output.
   * key-value data becomes a recognizable message with key (string) and value (string) payload.
   * Event is formed in host test, a tuple of ```key``` (string), ```value``` (string), ```timestamp``` where ```timestamp``` is time of message reception in Python [time.time()](https://docs.python.org/2/library/time.html#time.time) format (float, time in seconds since the epoch as a floating point number.).
* Each host test registers callbacks for available events.
* Few keys' names in key-value messaging protocol are promoted to be considered "system events". Their names are used by event loop mechanism to communicate between DUT, host and various internal components. Please do not use restricted even names for your own private events. What's more:
    * User can't register callbacks to "system events" with few exceptions.
    * Reserved event/message keys have leading ```__``` in name:
      * ```__sync``` - sync message, used by master and DUT to handshake.
      * ```__notify_sync_failed``` - sent by host when sync response not received from DUT.
      * ```__timeout``` - timeout in sec, sent by DUT after ```{{sync;UUID}}``` is received.
      * ```__version``` - ```greentea-client``` version send from DUT to host.
      * ```__host_test_name``` - host test name, sent by DUT after ```{{sync;UUID}}``` is received.
      * ```__notify_prn``` - sent by host test to print log message.
      * ```__notify_conn_lost``` - sent by host test's connection process to notify serial port connection lost.
      * ```__notify_complete``` - sent by DUT, async notificaion about test case result (true, false, none).
      * ```__coverage_start``` - sent by DUT, coverage data.
      * ```__testcase_start``` - sent by DUT, test case start data.
      * ```__testcase_finish``` - sent by DUT, test case result.
      * ```__exit``` - sent by DUT, test suite execution finished.
      * ```__exit_event_queue``` - sent by host test, indicating no more events expected.
  * Non-Reserved event/message keys have leading ```__``` in name:
    * ```__rxd_line``` - Event triggered when ```\n``` was found on DUT RXD channel. It can be overridden (```self.register_callback('__rxd_line', <callback_function>)```) and used by user. Event is sent by host test to notify a new line of text was received on RXD channel. ```__rxd_line``` event payload (value) in a line of text received from DUT over RXD.
* Each host test (master side) has four functions used by async framework:
  * ```setup()``` used to initialize host test and register callbacks.
  * ```result()``` used to return test case result when ```notify_complete()``` is not called.
  * ```teardown()``` used to finalize and resource freeing. It is guaranteed that ```teardown()``` will be always called after timeout or async test completion().
  * ```notify_complete(result : bool)``` used by host test to notify test case result. This result will be read after test suite ```TIMEOUT```s or after DUT send ```__exit``` message (test suite execution finished event).
  * ```self.send_kv(key : string, value : string)``` - send key-value message to DUT.
  * ```self.log(text : string)``` - send event ```__notify_prn``` with text as payload (value). Your message will be printed in log.
* Result returned from host test is a test suite result. Test cases results are reported by DUT, usually using modified ```utest``` framework.

# Greentea client API

DUT test API was first introduced in ```mbedmicro/mbed``` project [here](https://github.com/mbedmicro/mbed/tree/master/libraries/tests/mbed/env). After refactoring this functionality was copied and improved in [greentea-client](https://github.com/ARMmbed/mbed-os/tree/master/features/frameworks/greentea-client) module.

* Slave side key-value protocol API, see [here](https://github.com/ARMmbed/mbed-os/blob/master/features/frameworks/greentea-client/greentea-client/test_env.h) for details.

```c++
// Send key-value pairs from slave to master
void greentea_send_kv(const char *, const char *);
void greentea_send_kv(const char *, const int);
void greentea_send_kv(const char *, const int, const int);
void greentea_send_kv(const char *, const char *, const int);
void greentea_send_kv(const char *, const char *, const int, const int);

// Blocking, receive key-value message from master
int greentea_parse_kv(char *, char *, const int, const int);
```
Functions are used to send key-string or key-integer value messages to master. This functions should replace typical ```printf()``` calls with payload/control data to host.

* **Blocking** wait for key-value pair message in input stream:

```c++
int greentea_parse_kv(char *out_key, char *out_value, const int out_key_len, const int out_value_len);
```

This function should replace ```scanf()``` used to check for incoming messages from master.
Function parses input and if key-value message is found load to ```out_key```, ```out_value``` key-value pair. Use ```out_key_size``` and ```out_value_size``` to define out buffers max size (including trailing zero).

# Key-value transport protocol sequence

Key-value protocol has few parts:
* **Handshake** - synchronize master and slave.
* **Preamble exchange** - DUT informs host about test parameters such as client version, test suite timeout, requested host test name etc. After this part is finished master will create requested host test and attach callbacks to user events.
* **Event exchange** - key-value event exchange between slave and master. In this exchange in general slave (DUT) will initialize communication. This part may end with ending pair of events ```end``` and ```__exit``` where ```end``` event carries test suite result returned by DUT and ```__exit``` event marks test suite ended and exited. After ```__exit``` event is received there will be no more communication between DUT and host test.

## Handshake
Hanshake between DUT and host is a sequence of ```__sync``` events send between host (master) and DUT (slave). This is currently only situation when master initiates communication first. Handshake should provide synchronization point where master and slave are starting the same session.

After reset:
* DUT calls function ```GREENTEA_SETUP(timeout, "host test name");``` which
* calls immediately ```greentea_parse_kv``` (blocking parse of input serial port for event ```{{__sync;UUID}}```).
* When ```__sync``` packet is parsed in the stream DUT sends back (echoes) ```__sync``` event with the same [UUID](https://en.wikipedia.org/wiki/Universally_unique_identifier#Version_3_.28MD5_hash_.26_namespace.29) as payload. UUID is a random value e.g.  ```5f8dbbd2-199a-449c-b286-343a57da7a37```.

```
                           DUT (slave)        host (master)
                             -----               -----
                               |                   |
                DUT reset ---> |                   |
                               |                   |
greentea_parse_kv(key,value)   |                   |
-------[ blocking ]----------->|                   |
                               |                   |
                               .                   .
                               .                   .
                               |                   |  self.send_kv("__sync", UUID)
                               |  {{__sync;UUID}}  |<-----------------------------
                               |<------------------|
                               |                   |
                               |                   |
greentea_parse_kv              |  {{__sync;UUID}}  |
echoes __sync event with       |------------------>|
the same UUID to master        |                   |
                               |                   |
```

Example of handshake from ```htrun``` log:

DUT code:

```c
// GREENTEA_SETUP pseudo-code
void GREENTEA_SETUP(const int timeout, const char *host_test_name) {
    // Wait for SYNC and echo it back
    char _key[8] = {0};
    char _value[48] = {0};
    while (1) {
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        if (strcmp(_key, GREENTEA_TEST_ENV_SYNC) == 0) {
            // Found correct __sunc message
            greentea_send_kv(_key, _value);
            break;
        }
    }

    // Send PREAMBLE: client version, test suite timeout and requested host test
    greentea_send_kv(GREENTEA_TEST_ENV_HOST_TEST_VERSION, "0.1.8");
    greentea_send_kv(GREENTEA_TEST_ENV_TIMEOUT, timeout);
    greentea_send_kv(GREENTEA_TEST_ENV_HOST_TEST_NAME, host_test_name);
}
```

Corresponding log:

```
[1458565465.35][SERI][INF] reset device using 'default' plugin...
[1458565465.60][SERI][INF] wait for it...
[1458565466.60][CONN][INF] sending preamble '2f554b1c-bbbf-4b1b-b1f0-f45493282f2c'
[1458565466.60][SERI][TXD] mbedmbedmbedmbedmbedmbedmbedmbedmbedmbed
[1458565466.60][SERI][TXD] {{__sync;2f554b1c-bbbf-4b1b-b1f0-f45493282f2c}}
[1458565466.74][CONN][INF] found SYNC in stream: {{__sync;2f554b1c-bbbf-4b1b-b1f0-f45493282f2c}}, queued...
[1458565466.74][HTST][INF] sync KV found, uuid=2f554b1c-bbbf-4b1b-b1f0-f45493282f2c, timestamp=1458565466.743000
[1458565466.74][CONN][RXD] {{__sync;2f554b1c-bbbf-4b1b-b1f0-f45493282f2c}}
```

## Preamble exchange

This phase comes just after handshake phase. DUT informs host about test parameters such as client version, timeout, requested host test name etc. After this part is finished master will create requested host test and attach callbacks to user events.
This phase is ended with ```__host_test_name``` being received by host. After ```__host_test_name``` event is received

```
DUT (slave)              host (master)
  -----                     -----
    |                         |
    |    {{__version;%s}}     |
    |------------------------>|
    |                         |
    |    {{__timeout;%d}}     |
    |------------------------>|
    |                         |
    | {{__host_test_name;%s}} |
    |------------------------>|
    |                         |
```

Example of handshake from ```htrun``` log:

* DUT code:

```c
void main() {
    GREENTEA_CLIENT(5, "default_auto");
    // ...
}
```

* Corresponding log:

```
[1458565466.76][CONN][INF] found KV pair in stream: {{__version;0.1.8}}, queued...
[1458565466.76][CONN][RXD] {{__version;0.1.8}}
[1458565466.76][HTST][INF] DUT greentea-client version: 0.1.8
[1458565466.77][CONN][INF] found KV pair in stream: {{__timeout;5}}, queued...
[1458565466.77][HTST][INF] setting timeout to: 5 sec
[1458565466.78][CONN][RXD] {{__timeout;5}}
[1458565466.81][CONN][INF] found KV pair in stream: {{__host_test_name;default_auto}}, queued...
[1458565466.81][HTST][INF] host test setup() call...
[1458565466.81][HTST][INF] CALLBACKs updated
[1458565466.81][HTST][INF] host test detected: default_auto
[1458565466.81][CONN][RXD] {{__host_test_name;default_auto}}
```

## Event exchange

In this phase DUT and host exchange events and host side is calling callbacks registered to each of the events sent from DUT. DUT can use function ```greentea_parse_kv``` to parse input stream for next incoming key-value event.
After ```__host_test_name``` event is received and before any event is consumed during this stage:
* Host state machine loads host test object by name provided in payload of ```__host_test_name``` event.E.g. event ```{{____host_test_name;default_auto}} will load host test named "*default_auto*".
* Host state machine loads callbacks registered by user in host test setup phase and hooks them to event machine.
Now host is ready to handle test suite test execution. From this moment each event sent from DUT will be handled by corresponding callback registered by user in host test setup. Unknown events will not be handled and warning will be printed in log.

```

DUT (slave)      host (master)
  -----             -----
    |                 |
    |                 |        Host Test
    |                 |         -----
    |                 |  create   |
    |                 |---------->|
    |                 |           |
    |                 |           |
    | {{key1;value}}  |           |
    |---------------->|           |          ht.setup()
    |       .         |           |<---[ user register callbacks ]---
    |       .         |           |
    |       .         |           |  host.callbacks.update(ht.get_callbacks())
    |       .         |           |<---[ host state machine ]------------------
    | {{key2;value}}  |           |
    |---------------->|           |
    |                 |           |
    |                 |           |
    |                 |           | ht.callbacks[key1](key, value, timestamp)
    |                 |           |<------------------------------------------
    |                 |           | ht.callbacks[key2](key, value, timestamp)
    |                 |           |<------------------------------------------
    |                 |           |
    |                 |           |
    -  - - - - - - - -  - - - -     - -
          TEST CASE FLOW CONTINUES
    -  - - - - - - - -  - - - -     - -
    |                 |           |
    |                 |           | ht.notify_complete(true)
    |                 |           | (sets test suite 'result' to true
    |                 |           |<----------------
    |                 |           |
    |                 |           |
    | {{end;success}} |           |
    |---------------->|           |
    |                 |           |
    | {{__exit;%d}}   |           |
    |---------------->|           |
    |                 |           |
    |                 |           | result = ht.result()
    |                 |           |<----------------
    |                 |           |
    |                 |           | ht.teardown()
    |                 |           |<----------------
    |                 |           |
    |                 |           |

```

* After DUT send ```__exit``` or after timeout it is guaranteed that host test ```teardown()``` function will be called. This call is blocking, please make sure your tear down function finishes.

# DUT - host test case workflow
## DUT implementation

```c++
int main() {
    // 1. Handshake between DUT and host and
    // 2. Send test case related data
    GREENTEA_SETUP(15, "gimme_auto");  // __timeout, __host_test_name

    // ...
    // Send to master {{gimme_something; some_stuff}}
    greentea_send_kv("gimme_something", "some_stuff");

    char key[16] = {0};
    char value[32] = {0};
    // Blocking wait for master response for {{gimme_something; some_stuff}}
    greentea_parse_kv(key, value, sizeof(key), sizeof(value));
    // ...
    fprintf(stderr, "Received from master %s, %s", key, value);
    // ...

    GREENTEA_TESTSUITE_RESULT(true);    // __exit
}
```

## Example of corresponding host test

```python
class GimmeAuto(BaseHostTest):
    """ Simple, basic host test's test runner waiting for serial port
        output from MUT, no supervision over test running in MUT is executed.
    """

    __result = None
    name = "gimme_auto"

    def _callback_gimme_something(self, key, value, timestamp):
        # You've received {{gimme_something;*}}

        # We will send DUT some data back...
        # And now decide about test case result
        if value == 'some_stuff':
            # Message payload/value was 'some_stuff'
            # We can for example return true from test
            self.send_kv("print_this", "This is what I wanted %s"% value)
            self.notify_complete(True)
        else:
            self.send_kv("print_this", "This not what I wanted :(")
            self.notify_complete(False)

    def setup(self):
        # Register callback for message 'gimme_something' from DUT
        self.register_callback("gimme_something", self._callback_gimme_something)

        # Initialize your host test here
        # ...

    def result(self):
        # Define your test result here
        # Or use self.notify_complete(bool) to pass result anytime!
        return self.__result

    def teardown(self):
        # Release resources here after test is completed
        pass
```

Log:

```
[1454926794.22][HTST][INF] copy image onto target...
        1 file(s) copied.
[1454926801.48][HTST][INF] starting host test process...
[1454926802.01][CONN][INF] starting connection process...
[1454926802.01][CONN][INF] initializing serial port listener...
[1454926802.01][SERI][INF] serial(port=COM188, baudrate=9600)
[1454926802.02][SERI][INF] reset device using 'default' plugin...
[1454926802.27][SERI][INF] wait for it...
[1454926803.27][CONN][INF] sending preamble '9caa42a0-28a0-4b80-ba1d-befb4e43a4c1'...
[1454926803.27][SERI][TXD] mbedmbedmbedmbedmbedmbedmbedmbedmbedmbed
[1454926803.27][SERI][TXD] {{__sync;9caa42a0-28a0-4b80-ba1d-befb4e43a4c1}}
[1454926803.40][CONN][RXD] {{__sync;9caa42a0-28a0-4b80-ba1d-befb4e43a4c1}}
[1454926803.40][CONN][INF] found SYNC in stream: {{__sync;9caa42a0-28a0-4b80-ba1d-befb4e43a4c1}}, queued...
[1454926803.40][HTST][INF] sync KV found, uuid=9caa42a0-28a0-4b80-ba1d-befb4e43a4c1, timestamp=1454926803.405000
[1454926803.42][CONN][RXD] {{__timeout;15}}
[1454926803.42][CONN][INF] found KV pair in stream: {{__timeout;15}}, queued...
[1454926803.42][HTST][INF] setting timeout to: 15 sec
[1454926803.45][CONN][RXD] {{__host_test_name;gimme_auto}}
[1454926803.45][CONN][INF] found KV pair in stream: {{__host_test_name;gimme_auto}}, queued...
[1454926803.45][HTST][INF] host test setup() call...
[1454926803.45][HTST][INF] CALLBACKs updated
[1454926803.45][HTST][INF] host test detected: gimme_auto
[1454926803.48][CONN][RXD] {{gimme_something;some_stuff}}
[1454926803.48][CONN][INF] found KV pair in stream: {{gimme_something;some_stuff}}, queued...
[1454926803.48][SERI][TXD] {{print_this;This is what I wanted some_stuff}}
[1454926803.48][HTST][INF] __notify_complete(True)
[1454926803.62][CONN][RXD] Received from master print_this, This is what I wanted some_stuf
[1454926803.62][CONN][RXD] {{end;success}}
[1454926803.62][CONN][INF] found KV pair in stream: {{end;success}}, queued...
[1454926803.62][HTST][ERR] orphan event in main phase: {{end;success}}, timestamp=1454926803.625000
[1454926803.63][CONN][RXD] {{__exit;0}}
[1454926803.63][CONN][INF] found KV pair in stream: {{__exit;0}}, queued...
[1454926803.63][HTST][INF] __exit(0)
[1454926803.63][HTST][INF] test suite run finished after 0.21 sec...
[1454926803.63][HTST][INF] exited with code: None
[1454926803.63][HTST][INF] 0 events in queue
[1454926803.63][HTST][INF] stopped consuming events
[1454926803.63][HTST][INF] host test result() skipped, received: True
[1454926803.63][HTST][INF] calling blocking teardown()
[1454926803.63][HTST][INF] teardown() finished
[1454926803.63][HTST][INF] {{result;success}}
mbedgt: mbed-host-test-runner: stopped
mbedgt: mbed-host-test-runner: returned 'OK'
mbedgt: test on hardware with target id: 02400226d94b0e770000000000000000000000002492f3cf
mbedgt: test suite 'mbed-drivers-test-gimme' ......................................................... OK in 10.02 sec
mbedgt: shuffle seed: 0.3631708941
mbedgt: test suite report:
+---------------+---------------+-------------------------+--------+--------------------+-------------+
| target        | platform_name | test suite              | result | elapsed_time (sec) | copy_method |
+---------------+---------------+-------------------------+--------+--------------------+-------------+
| frdm-k64f-gcc | K64F          | mbed-drivers-test-gimme | OK     | 10.02              | shell       |
+---------------+---------------+-------------------------+--------+--------------------+-------------+
mbedgt: test suite results: 1 OK
```

# Host test examples
## Return result after __exit

```python
class GimmeAuto(BaseHostTest):
    """ Simple, basic host test's test runner waiting for serial port
        output from MUT, no supervision over test running in MUT is executed.
    """

    __result = None
    name = "gimme_auto"

    def _callback_gimme_something(self, key, value, timestamp):
        # You've received {{gimme_something;*}}

        # We will send DUT some data back...
        # And now decide about test case result
        if value == 'some_stuff':
            # Message payload/value was 'some_stuff'
            # We can for example return true from test
            self.send_kv("print_this", "This is what I wanted %s"% value)
            self.__result = True
        else:
            self.send_kv("print_this", "This not what I wanted :(")
            self.__result = False

    def setup(self):
        # Register callback for message 'gimme_something' from DUT
        self.register_callback("gimme_something", self._callback_gimme_something)

        # Initialize your host test here
        # ...

    def result(self):
        # Define your test result here
        # Or use self.notify_complete(bool) to pass result anytime!
        return self.__result

    def teardown(self):
        # Release resources here after test is completed
        pass
```

Corresponding log:

```
[1454926627.11][HTST][INF] copy image onto target...
        1 file(s) copied.
[1454926634.38][HTST][INF] starting host test process...
[1454926634.93][CONN][INF] starting connection process...
[1454926634.93][CONN][INF] initializing serial port listener...
[1454926634.93][SERI][INF] serial(port=COM188, baudrate=9600)
[1454926634.94][SERI][INF] reset device using 'default' plugin...
[1454926635.19][SERI][INF] wait for it...
[1454926636.19][CONN][INF] sending preamble '9a743ff3-45e6-44cf-9e2a-9a83e6205184'...
[1454926636.19][SERI][TXD] mbedmbedmbedmbedmbedmbedmbedmbedmbedmbed
[1454926636.19][SERI][TXD] {{__sync;9a743ff3-45e6-44cf-9e2a-9a83e6205184}}
[1454926636.33][CONN][RXD] {{__sync;9a743ff3-45e6-44cf-9e2a-9a83e6205184}}
[1454926636.33][CONN][INF] found SYNC in stream: {{__sync;9a743ff3-45e6-44cf-9e2a-9a83e6205184}}, queued...
[1454926636.33][HTST][INF] sync KV found, uuid=9a743ff3-45e6-44cf-9e2a-9a83e6205184, timestamp=1454926636.331000
[1454926636.34][CONN][RXD] {{__timeout;15}}
[1454926636.34][CONN][INF] found KV pair in stream: {{__timeout;15}}, queued...
[1454926636.34][HTST][INF] setting timeout to: 15 sec
[1454926636.38][CONN][RXD] {{__host_test_name;gimme_auto}}
[1454926636.38][CONN][INF] found KV pair in stream: {{__host_test_name;gimme_auto}}, queued...
[1454926636.38][HTST][INF] host test setup() call...
[1454926636.38][HTST][INF] CALLBACKs updated
[1454926636.38][HTST][INF] host test detected: gimme_auto
[1454926636.41][CONN][RXD] {{gimme_something;some_stuff}}
[1454926636.41][CONN][INF] found KV pair in stream: {{gimme_something;some_stuff}}, queued...
[1454926636.41][SERI][TXD] {{print_this;This is what I wanted some_stuff}}
[1454926636.54][CONN][RXD] Received from master print_this, This is what I wanted some_stuf
[1454926636.54][CONN][RXD] {{end;success}}
[1454926636.54][CONN][INF] found KV pair in stream: {{end;success}}, queued...
[1454926636.55][HTST][ERR] orphan event in main phase: {{end;success}}, timestamp=1454926636.541000
[1454926636.56][CONN][RXD] {{__exit;0}}
[1454926636.56][CONN][INF] found KV pair in stream: {{__exit;0}}, queued...
[1454926636.56][HTST][INF] __exit(0)
[1454926636.56][HTST][INF] test suite run finished after 0.22 sec...
[1454926636.56][HTST][INF] exited with code: None
[1454926636.56][HTST][INF] 0 events in queue
[1454926636.56][HTST][INF] stopped consuming events
[1454926636.56][HTST][INF] host test result(): True
[1454926636.56][HTST][INF] calling blocking teardown()
[1454926636.56][HTST][INF] teardown() finished
[1454926636.56][HTST][INF] {{result;success}}
mbedgt: mbed-host-test-runner: stopped
mbedgt: mbed-host-test-runner: returned 'OK'
mbedgt: test on hardware with target id: 02400226d94b0e770000000000000000000000002492f3cf
mbedgt: test suite 'mbed-drivers-test-gimme' ......................................................... OK in 10.04 sec
mbedgt: shuffle seed: 0.3866075474
mbedgt: test suite report:
+---------------+---------------+-------------------------+--------+--------------------+-------------+
| target        | platform_name | test suite              | result | elapsed_time (sec) | copy_method |
+---------------+---------------+-------------------------+--------+--------------------+-------------+
| frdm-k64f-gcc | K64F          | mbed-drivers-test-gimme | OK     | 10.04              | shell       |
+---------------+---------------+-------------------------+--------+--------------------+-------------+
mbedgt: test suite results: 1 OK
```

# Writing DUT test suite (slave side)

## DUT test suite with single test case

We can use few methods to structure out test suite and test cases. Simpliest would be to use ```greentea-client``` API and wrap one test case inside out test suite. This way of creating test suite is useful when you want to:
* write only one test case inside test suite,
* make example application (example as a test) or
* when your test suite is calling blocking forever function. For example all types of UDP/TCP servers which run in forever loop are in this category. In this case we do not expect from DUT ```__exit``` event at all and host test should be designed in such a way that it always return result.

### DUT always finishes execution

In this example DUT code uses ```greentea-client``` to sync (```GREENTEA_SETUP```) and pass result (```GREENTEA_TESTSUITE_RESULT```) to ```Greentea```. This is very simple example of how you can write tests. Note that in this example test suite only implements one test case. Actually test suite is test case at the same time. Result passed to ```GREENTEA_TESTSUITE_RESULT``` will be at the same time test case result.

* DUT implementation:

```c++
#include "greentea-client/test_env.h"
#include "unity/unity.h"    // Optional: unity ASSERTs

int app_start(int, char*[]) {

    bool result = true;
    GREENTEA_SETUP(15, "default_auto");

    // test case execution and assertions

    GREENTEA_TESTSUITE_RESULT(result);
    return 0;
}
```

### DUT test suite never finishes execution

Test suite is implemented so that it will never exit / finish its execution. For example ```main()``` or ```app_start()``` functions are implemented using infinite (endless) loop. This property have for example UDP/TCP servers (listening forever), all sorts of echo servers etc.

In this example DUT code uses ```greentea-client``` to sync (```GREENTEA_SETUP```) with ```Greentea```. We are not calling ```GREENTEA_TESTSUITE_RESULT(result)``` at any time. In this example host test is responsible for providing test suite result using ```self.notify_complete()``` API or ```self.result()``` function.

You need to write and specify by name your custom host test:
* DUT side uses second argument of ```GREENTEA_SETUP(timeout, host_test_name)``` function:

```c++
GREENTEA_SETUP(15, "wait_us_auto");
```

* You need to place your custom host test in ```<module>/test/host_tests``` directory.
  * Do not forget to name host test accordingly. See below example host test ```name``` class member.

* DUT implementation using ```my_host_test``` custom host test:

```c++
#include "greentea-client/test_env.h"
#include "unity/unity.h"

void recv() {
    // receive from client
}

int app_start(int, char*[]) {

    Ethernet eth(TCP_SERVER, PORT, recv);
    GREENTEA_SETUP(15, "my_host_test");

    eth.listen();   // Blocking forever

    return 0;
}
```

* Example host test template:

```python
from mbed_host_tests import BaseHostTest

class YourCustomHostTest(BaseHostTest):

    name = "my_host_test"   # Host test names used by GREENTEA_CLIENT(..., host_test_name)

    __result = False    # Result in case of timeout!

    def _callback_for_event(self, key, value, timestamp):
        #
        # Host test API:
        #
        # self.notify_complete(result : bool)
        #
        # """! Notify main even loop that host test finished processing
        #      @param result True for success, False failure. If None - no action in main even loop
        # """
        #
        # self.send_kv(key : string, value : string)
        #
        # """! Send Key-Value data to DUT
        #      @param key Event key
        #      @param value Event payload
        # """
        #
        # self.log(text : string)
        #
        # """! Send log message to main event loop
        #      @param text log message
        # """
        pass

    def setup(self):
        # TODO:
        # * Initialize your resources
        # * Register callbacks:
        #
        # Host test API:
        #
        # self.register_callback(event_name, callable, force=False)
        #
        # """! Register callback for a specific event (key: event name)
        #     @param key String with name of the event
        #     @param callback Callable which will be registered for event "key"
        #     @param force God mode, if set to True you can add callback on any system event
        # """
        pass

    def teardown(self):
        # Destroy all resources used by host test.
        # For example open sockets, open files, auxiliary threads and processes.
        pass

    def result(self):
        # Returns host test result (True, False or None)
        # This function will be called when test suite ends (also timeout).
        # Use when you want to pass result after host state machine stops.
        return __result
```

### DUT test suite with ```utest``` harness

```utest``` harness allows you to define multiple test cases inside your test suite. This feature is supported by ```Greentea``` test tools.

* DUT implementation:

```c++
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"

status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    // Continue with next test case if it fails
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

void test_uninitialised_array() {
    // TEst case code...
}

void test_repeated_init() {
    // TEst case code...
}

void test_data_types() {
    // TEst case code...
}

const Case cases[] = {
    Case("Test uninitialised array", test_uninitialised_array, greentea_failure_handler),
    Case("Test repeated array initialisation", test_repeated_init, greentea_failure_handler),
    Case("Test basic data type arrays", test_data_types, greentea_failure_handler)
    // ...
};

status_t greentea_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(5, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

int app_start(int, char*[]) {

   // Run the test cases
    Harness::run(specification);
}
```

# Writing host tests (master side)
When writing a new host test for your module please bear in mind that:
* You own the host test and you should write it the way so it can coexist with the same host tests ran by other processes such as Continuous Integration systems or other host users.
  * Note: If you work in isolation and your test environment if fully controlled by you (for example you queue all tasks calling host tests, or use global host unique socket port numbers) this rule doesn’t apply to you.
* When writing host test using OS resources such as sockets, files, serial ports, peripheral devices like for example multi-meters / scopes. remember that those resources are indivisible!
  * For example if you hardcode in your host test UDP port 32123 and use it for UDP server implementation  of your host test bear in mind that this port may be already used. It is your responsibility to react for this event and implement means to overcome it (if possible).

## Callbacks
You can register callbacks in ```setup()``` phase or decorate callback functions using ```@event_callback``` decorator.

### Callback registration in setup() method

```python
from mbed_host_tests import BaseHostTest

class DetectRuntimeError(BaseHostTest):

    __result = False

    def callback_some_event(self, key, value, timeout):
        # Do something with 'some_event'
        pass

    def setup(self):
        # Reagister call back for 'some_event' event
        self.register_callback('some_event', self.callback_some_event)

    def result(self):
        # Do some return calculations
        return self.__result
```

Below the same callback registered using decorator:

### Callback decorator definition

```python
from mbed_host_tests.host_tests import BaseHostTest, event_callback

class DetectRuntimeError(BaseHostTest):

    __result = False

    @event_callback('some_event')
    def callback_some_event(self, key, value, timeout):
        # Do something with 'some_event'
        pass

    def setup(self):
        # Do some extra setup if required
        # You can also register here callbacks using self.register_callback(...) method
        pass

    def result(self):
        # Do some return calculations
        return self.__result
```

### Parsing text received from DUT (line by line)
Example of host test expecting ```Runtime error ... CallbackNode ... ``` string in DUT output.
We will use allowed to override ```__rxd_line``` event to hook to DUT RXD channel lines of text.

#### Before Greentea v0.2.0

```python
from sys import stdout
from mbed_host_tests import BaseHostTest

class DetectRuntimeError(BaseHostTest):

    name = 'detect_runtime_error'

    def test(self, selftest):
        result = selftest.RESULT_FAILURE
        try:
            while True:
                line = selftest.mbed.serial_readline()

                if line is None:
                    return selftest.RESULT_IO_SERIAL

                stdout.write(line)
                stdout.flush()

                line = line.strip()

                if line.startswith("Runtime error") and line.find("CallbackNode") != -1:
                    result = selftest.RESULT_SUCCESS
                    break

        except KeyboardInterrupt, _:
            selftest.notify("\r\n[CTRL+C] exit")
            result = selftest.RESULT_ERROR

        return result
```

#### Using __rdx_line event

```python
from mbed_host_tests import BaseHostTest

class DetectRuntimeError(BaseHostTest):
    """! We _expect_ to detect 'Runtime error' """

    __result = False

    def callback__rxd_line(self, key, value, timeout):
        #
        # Parse line of text received over e.g. serial from DUT
        #
        line = value.strip()
        if line.startswith("Runtime error") and "CallbackNode" in line:
            # We've found exepcted "Runtime error" string in DUTs output stream
            self.notify_complete(True)

    def setup(self):
        # Force, we force callback registration even it is a restricted one (starts with '__')
        self.register_callback('__rxd_line', self.callback__rxd_line, force=True)

    def result(self):
        # We will return here (False) when we reach timeout of the test
        return self.__result

    def teardown(self):
        pass
```

##  ```htrun``` new log format:
  * ```[timestamp][source][level]``` - new log format, where:
    * ```timestamp``` - returned by Python's ```time.time()```.
    * ```source``` - log source.
      * ```CONN``` - connection process (pooling for connection source e.g. serial port),
      * ```SERI``` - serial port wrapper with standard read, write, flush interface,
      * ```HTST``` - host test object, HostTestBase derived object,
      * ```PLGN``` - host test plugins, type `BasePlugin` of the plugin,
      * ```COPY``` - host test plugins, type `CopyMethod` of the plugin,
      * ```REST``` - host test plugins, type `ResetMethod` of the plugin,
    * ```level``` - logging level:
      * ```INF``` (info),
      * ```WRN``` (warning),
      * ```ERR``` (error).
      * ```TXD``` (host's TX channel, to DUT).
      * ```RXD``` (host's RX channel, from DUT).

### Log example
* ```[1455218713.87][CONN][RXD] {{__sync;a7ace3a2-4025-4950-b9fc-a3671103387a}}```:
* Logged from ```CONN``` (connection process).
* ```RXD``` channel emitted ```{{__sync;a7ace3a2-4025-4950-b9fc-a3671103387a}}```.
* Time stamp: ```2016-02-11 19:53:27```, see below:

# Plugins

In order to work with platforms for which the hardware is still under development, and hence may not have an mbed interface chip, some "hook" files are required.  Operation with these platforms is a matter for the platform development teams involved and is not, in general, supported by ARM.

## SARA NBIOT EVK
The SARA NBIOT EVK board must be connected to a Windows PC using a Segger JLink box, which is used for downloading code and resetting the board. The USB port on the EVK must also be connected to the same PC.  To make use of these hooks you will also require access to some proprietary tools that can be requested from u-blox.


# Testing mbed-os examples

mbed-os examples are essentially sample apps written as inspirational code for developers to understand the mbed-os APIs and coding paradigms. Before every mbed-os release all examples are tested across all supported configs and platforms. There is already a large set examples available and as they grow it is important to automate them. Hence automating examples make sense. Although it is important not to pollute them with test like instrumentation. As that will defeat the purpose of examples being simple and specific.

Hence the strategy for testing examples is based on observation instead of interaction. An example's serial logging is captured and converted into a templated log. All successive executions of this example should match this log.

Templated log simply means a log with text that does not change or regular expressions replacing original text. Below is an example of the templated log:

```

							      >	Using Ethernet LWIP

							      >	Client IP Address is 10.2.203.139

							      >	Connecting with developer.mbed.org

Starting the TLS handshake...								Starting the TLS handshake...

							      >	TLS connection to developer.mbed.org established

Server certificate:								Server certificate:

							      >
								    cert. version     : 3
							      >
								    serial number     : 11:21:B8:47:9B:21:6C:B1:C6:AF:BC:5D:0
							      >
								    issuer name       : C=BE, O=GlobalSign nv-sa, CN=GlobalSi
							      >
								    subject name      : C=GB, ST=Cambridgeshire, L=Cambridge,
							      >
								    issued  on        : 2016-03-03 12:26:08
							      >
								    expires on        : 2017-04-05 10:31:02
							      >
								    signed using      : RSA with SHA-256
							      >
								    RSA key size      : 2048 bits
							      >
								    basic constraints : CA=false
							      >
								    subject alt name  : *.mbed.com, mbed.org, *.mbed.org, mbe
							      >
								    key usage         : Digital Signature, Key Encipherment
							      >
								    ext key usage     : TLS Web Server Authentication, TLS We

Certificate verification passed								Certificate verification passed




							      >	HTTPS: Received 439 chars from server

							      >	HTTPS: Received 200 OK status ... [OK]

HTTPS: Received 'Hello world!' status ... [OK]								HTTPS: Received 'Hello world!' status ... [OK]

HTTPS: Received message:								HTTPS: Received message:




							      >	HTTP/1.1 200 OK

							      >	Server: nginx/1.7.10

							      >	Date: Thu, 01 Dec 2016 13:56:32 GMT

							      >	Content-Type: text/plain

							      >	Content-Length: 14

							      >	Connection: keep-alive

							      >	Last-Modified: Fri, 27 Jul 2012 13:30:34 GMT

							      >	Accept-Ranges: bytes

							      >	Cache-Control: max-age=36000

							      >	Expires: Thu, 01 Dec 2016 23:56:32 GMT

							      >	X-Upstream-L3: 172.17.0.3:80

							      >	X-Upstream-L2: developer-sjc-indigo-2-nginx

							      >	Strict-Transport-Security: max-age=31536000; includeSubdomain




Hello world!								Hello world!

```

Please observe above that all the lines that have data that changes from execution to execution (on right) have been removed. It makes it possible htrun to compare these logs. htrun matches lines from the compare log (on left) one by one. It keeps on looking for a line until it matches. Once matched it moves on to match the next line. If it finds all lines from the compare log in the target serial output stream. Then it halts and passes the examples.

Another example with regular examples is shown below:

```

  SHA-256                  :\s*\d+ Kb/s,\s*\d+ cycles/byte							      |	  SHA-256                  :       1922 Kb/s,         61 cycl

  SHA-512                  :\s*\d+ Kb/s,\s*\d+ cycles/byte							      |	  SHA-512                  :        614 Kb/s,        191 cycl

  AES-CBC-128              :\s*\d+ Kb/s,\s*\d+ cycles/byte							      |	  AES-CBC-128              :       1401 Kb/s,         83 cycl

  AES-CBC-192              :\s*\d+ Kb/s,\s*\d+ cycles/byte							      |	  AES-CBC-192              :       1231 Kb/s,         95 cycl

  AES-CBC-256              :\s*\d+ Kb/s,\s*\d+ cycles/byte							      |	  AES-CBC-256              :       1097 Kb/s,        106 cycl

  AES-GCM-128              :\s*\d+ Kb/s,\s*\d+ cycles/byte							      |	  AES-GCM-128              :        429 Kb/s,        273 cycl

  AES-GCM-192              :\s*\d+ Kb/s,\s*\d+ cycles/byte							      |	  AES-GCM-192              :        412 Kb/s,        285 cycl

  AES-GCM-256              :\s*\d+ Kb/s,\s*\d+ cycles/byte							      |	  AES-GCM-256              :        395 Kb/s,        297 cycl

  AES-CCM-128              :\s*\d+ Kb/s,\s*\d+ cycles/byte							      |	  AES-CCM-128              :        604 Kb/s,        194 cycl

  AES-CCM-192              :\s*\d+ Kb/s,\s*\d+ cycles/byte							      |	  AES-CCM-192              :        539 Kb/s,        217 cycl

  AES-CCM-256              :\s*\d+ Kb/s,\s*\d+ cycles/byte							      |	  AES-CCM-256              :        487 Kb/s,        241 cycl

  CTR_DRBG \(NOPR\)          :\s*\d+ Kb/s,\s*\d+ cycles/byte							      |	  CTR_DRBG (NOPR)          :       1145 Kb/s,        102 cycl

  CTR_DRBG \(PR\)            :\s*\d+ Kb/s,\s*\d+ cycles/byte							      |	  CTR_DRBG (PR)            :        821 Kb/s,        142 cycl

  HMAC_DRBG SHA-256 \(NOPR\) :\s*\d+ Kb/s,\s*\d+ cycles/byte							      |	  HMAC_DRBG SHA-256 (NOPR) :        219 Kb/s,        537 cycl

  HMAC_DRBG SHA-256 \(PR\)   :\s*\d+ Kb/s,\s*\d+ cycles/byte							      |	  HMAC_DRBG SHA-256 (PR)   :        193 Kb/s,        612 cycl

  RSA-2048                 :\s*\d+ ms/ public							      |	  RSA-2048                 :      30 ms/ public

  RSA-2048                 :\s*\d+ ms/private							      |	  RSA-2048                 :    1054 ms/private

  RSA-4096                 :\s*\d+ ms/ public							      |	  RSA-4096                 :     101 ms/ public

  RSA-4096                 :\s*\d+ ms/private							      |	  RSA-4096                 :    5790 ms/private

  ECDHE-secp384r1          :\s*\d+ ms/handshake							      |	  ECDHE-secp384r1          :    1023 ms/handshake

  ECDHE-secp256r1          :\s*\d+ ms/handshake							      |	  ECDHE-secp256r1          :     678 ms/handshake

  ECDHE-Curve25519         :\s*\d+ ms/handshake							      |	  ECDHE-Curve25519         :     580 ms/handshake

  ECDH-secp384r1           :\s*\d+ ms/handshake							      |	  ECDH-secp384r1           :     503 ms/handshake

  ECDH-secp256r1           :\s*\d+ ms/handshake							      |	  ECDH-secp256r1           :     336 ms/handshake

  ECDH-Curve25519          :\s*\d+ ms/handshake							      |	  ECDH-Curve25519          :     300 ms/handshake

```

To capture a log use following option:

```
mbedhtrun -d D: -p COM46 -m K64F -f .\BUILD\K64F\GCC_ARM\benchmark.bin --serial-output-file compare.log
```

Option ```--serial-output-file``` takes file name as argument and writes the target serial output to the file. Edit the file to remove lines that will change in successive executions. Put regular expressions if needed at places like benchmark numbers in above log. With these edits you are left with a template good for comparison.

Use following command to test the example and the comparison log:

```
mbedhtrun -d D: -p COM46 -m K64F -f .\BUILD\K64F\GCC_ARM\benchmark.bin --compare-log compare.log
```

In case an application requires more time to process data and generate results, you can use the option ```--polling-timeout``` to override the default timeout setting.

A tested comparison log can be checked into GitHub with the examples and can be used in the CI for example verification.
