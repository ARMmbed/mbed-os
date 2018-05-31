# Description

This document describes how to run LoRaRadio API tests.

## Configuring target HW

Before starting to test, the target HW must be configured for the test. This can be done by either modifying the application configuration `json` file (if using currently supported Semtech SX1272 or SX1276 radios) or implementing driver construction into test source.

The default mbed_app.json file provides configuration for some already supported HWs.

## Running tests

You can use the following command to run tests:

`mbed test -n mbed-os-tests-lorawan-loraradio -m TARGET -t GCC_ARM --app-config mbed-os/TESTS/lorawan/loraradio/template_mbed_app.txt`

Replace TARGET with the target device.
