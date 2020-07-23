## Configuring Ublox Cellular Modems for custom targets

In order to use cellular APIs with custom targets, user needs to define ublox modem macros in `mbed_app.json` configuration file:
```
"target_overrides": {
    "*": {
        "mbed-trace.enable": true,
        "platform.stdio-convert-newlines": true,
        "platform.stdio-baud-rate": 115200,
        "platform.default-serial-baud-rate": 115200
    },
    "K64F": {
        "target.macros_add": ["UBX_MDM_SARA_R41XM", "UBX_MDM_SARA_R412M"]
    }
}
```


List of supported ublox modems and their respective macros are as follow:

| Ublox mbed board  | Modem Name    | Family Macro         | Modem Macro          |
| -----------       | :-----------: | :-------------:      | :-------------:      |
| UBLOX_C027        | SARA-G350     | `UBX_MDM_SARA_G3XX`  | `UBX_MDM_SARA_G350`  |
| UBLOX_C030_N211   | SARA-N211     | `UBX_MDM_SARA_N2XX`  | `UBX_MDM_SARA_N211`  |
| UBLOX_C030_U201   | SARA-U201     | `UBX_MDM_SARA_U2XX`  | `UBX_MDM_SARA_U201`  |
| UBLOX_C030_R410M  | SARA-R410M    | `UBX_MDM_SARA_R41XM` | `UBX_MDM_SARA_R410M` |
| UBLOX_C030_R412M  | SARA-R412M    | `UBX_MDM_SARA_R41XM` | `UBX_MDM_SARA_R412M` |
