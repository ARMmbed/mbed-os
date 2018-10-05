# Mbed cellular connectivity

This is the Github repo for Mbed cellular connectivity:

    easy_cellular/
        EasyCellularConnection  Simplified cellular usage based on `CellularBase.h`
        CellularConnectionUtil  A utility class for cellular connection

    framework/
        API         Application Programming Interface for cellular connectivity
        AT          AT implementation based on 3GPP TS 27.007 specification
        common      Common and utility sources
        targets     Vendor specific cellular module adaptations

    TESTS           Cellular Greentea test

    UNITTESTS       Cellular unit test

## Known limitations

**Please note that this is a first release of Cellular framework and is subject to further development in future.**

Only UDP is supported when using AT commands to control sockets in an IP stack built into the cellular modem. If TCP is required, use the PPP/LWIP stack.

## Supported modules

You can find currently supported cellular modules in the `framework/targets/` folder, where we also add support for new cellular modules.

## Cellular configuration

You can change cellular defaults in the `mbed_lib.json` configuration file.

You can also override cellular defaults in the `mbed_app.json` configuration file:

    "config": {
        "cellular_plmn": {
            "help": "PLMN selection, 0=auto",
            "value": 0
        },
        "apn": {
            "help": "Access point name, e.g. internet",
            "value": "\"internet\""
        },
        "cellular_sim_pin": {
            "help": "PIN code",
            "value": "\"1234\""
        }
    }

## Debug traces

You can define the debug tracing level in the `mbed_app.json` configuration file:

    "target_overrides": {
        "*": {
            "target.features_add": ["COMMON_PAL"],
            "mbed-trace.enable": true,
            "platform.stdio-convert-newlines": true,
            "platform.stdio-baud-rate": 115200,
            "platform.default-serial-baud-rate": 115200
        }
    },
    "config": {
        "trace-level": {
            "help": "Options are TRACE_LEVEL_ERROR,TRACE_LEVEL_WARN,TRACE_LEVEL_INFO,TRACE_LEVEL_DEBUG",
            "macro_name": "MBED_TRACE_MAX_LEVEL",
            "value": "TRACE_LEVEL_INFO"
        }
    }

## Greentea tests

The `TESTS` folder contains Greentea tests for cellular specific classes. You need to give relevant configuration file with `--app-config` parameter, e.g.:

    mbed test -n features-cellular-tests-* --app-config features\cellular\TESTS\socket\udp\template_mbed_app.json.txt -v

    Note that Greentea tests use SIM PIN so you need to change that or your SIM card may get locked.

## Unit tests

The `UNITTESTS` folder contains unit tests for cellular specific classes. Unit tests are based on the stubbing method.

You can run those tests locally by running `./run_tests` script under the `UNITTESTS/` folder.

You need the following applications: `cpputest`, `gcov` and `lcov` (genhtml) for running the tests.

After you have run the `run_tests` script, you can find test results under `UNITTESTS/results` folder and line and function coverages under the `UNITTESTS/coverages` folder.
