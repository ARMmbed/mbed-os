# Mbed cellular connectivity

This is the Github repo for Mbed cellular connectivity:

    framework/
        API         Application Programming Interface for cellular connectivity
        AT          AT implementation based on 3GPP TS 27.007 specification
        common      Common and utility sources
        device      Implementation of cellular device and state machine
        targets     Vendor specific cellular module adaptations

    TESTS           Cellular Greentea test

**Note:** Application developers should use only the `API` folder.

## Known limitations

**Please note that this is a first release of Cellular framework and is subject to further development in future.**

## Supported modules

You can find currently supported cellular modules in the `framework/targets/` folder, where we also add support for new cellular modules.

## Cellular configuration

You can change cellular defaults in the `mbed_lib.json` configuration file.

## Debug traces

You can define the debug tracing level in the `mbed_app.json` configuration file:
```
"target_overrides": {
    "*": {
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
```

## Greentea tests

The `TESTS` folder contains Greentea tests for cellular specific classes. You need to give relevant configuration file with `--app-config` parameter, such as:

```
mbed test -n features-cellular-tests-* --app-config features\cellular\TESTS\socket\udp\template_mbed_app.json.txt -v
```

**Note:** Greentea tests use SIM PIN, so you need to change that or your SIM card may get locked.

## Unit tests

Cellular unit tests are in Mbed OS root `UNITTESTS`. Unit tests are based on the stubbing method.

You need the following applications: `cpputest`, `gcov` and `lcov` (genhtml) for running the tests.

After you run the `run_tests` script, you can find test results in `UNITTESTS/results`, and line and function coverages in `UNITTESTS/coverages`.
