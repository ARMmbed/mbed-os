# Mbed Cellular Connectivity

This is the Github repo for Mbed cellular connectivity:

    easy_cellular/
        EasyCellularConnection  Simplified cellular usage based on CellularBase.h
        CellularConnectionUtil  An utility class for cellular connection

    framework/
        API         Application Programming Interface for cellular connectivity
        AT          AT implementation based on 3GPP TS 27.007 specification
        common      Common and utility sources
        mux         MUX implementation based on 3GPP TS 27.010 specification
        targets     Vendor specific cellular module adaptations

## Known Limitations

**Please note that as a first release framework is subject to change and may break code that uses it.**

Only UDP is supported when using AT commands to control sockets in an IP stack built into the cellular modem. If TCP is required use PPP/LWIP stack.

## Supported Modules

Currently supported cellular modules can be found in the framework/targets/ folder, where also support for new cellular modules are added.

## Cellular Configuration

Cellular defaults can be changed in mbed_app.json configuration file:

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

## Debug Traces

Debug tracing level can be defined in mbed_app.json configuration file:

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
