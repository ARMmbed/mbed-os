# Adafruit Feather nRF52840 Express
This is a beta release of support for the Adafruit Feather nRF52840 Express (https://www.adafruit.com/product/4062)

The following features have been tested:
- LED's
- SPI and SPIM
- Console output

Features not specifically tested:
- I2C
- QSPI
- QSPI Flash
- anything else I have missed

## Building under Visual Studio Code

**prerequisites:**

- mbed-os CLI installed
- python3

**Installation (Windows only):**

- Download and install arm gcc compiler 9-2020-q2-update (or later)
  
https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-win32.exe?revision=50c95fb2-67ca-4df7-929b-55396266b4a1&la=en&hash=DE1CD6E7A15046FD1ADAF828EA4FA82228E682E2

- Install adafruit-nrfutil
```text
pip3 install --user adafruit-nrfutil
```
- Locate the adafruit-nrfutil.exe and add to path

- Create empty Mbed-os project.

- Install latest mbed-os from VS Code terminal
```text
mbed add https://github.com/ARMmbed/mbed-os.git
```

- update VS Code tasks.json to include following configuration inside the **tasks** array:
```json
    "tasks": [
        {
            "label": "compile release",
            "group": "build",
            "command": "mbed",
            "args": [
                "compile",
                "--profile=release",
                "-v",
                "-t",
                "GCC_ARM",
                "-m",
                "ADAFRUIT_NRF52840_FEATHER"
            ]
        },
        {
            "label": "ada-nrfutil app hex to DFU zip",
            "group": "build",
            "command": "adafruit-nrfutil",
            "args": [
                "dfu",
                "genpkg",
                "--dev-type",
                "0x0052",
                "--sd-req",
                "0x00B6",
                "--application",
                "${workspaceFolder}\\BUILD\\ADAFRUIT_NRF52840_FEATHER\\GCC_ARM-RELEASE\\${workspaceFolderBasename}_application.hex",
                "${workspaceFolder}\\BUILD\\ADAFRUIT_NRF52840_FEATHER\\GCC_ARM-RELEASE\\${workspaceFolderBasename}_application.hex.zip"
            ],
            "presentation": {
                "echo": true,
                "reveal": "always",
                "focus": false,
                "panel": "shared",
                "showReuseMessage": true,
                "clear": false
            },
        },
        {
            "label": "ada-nrfutil flash DFU pkg",
            "group": "build",
            "command": "adafruit-nrfutil",
            "args": [
                "--verbose",
                "dfu",
                "serial",
                "-pkg",
                "${workspaceFolder}\\BUILD\\ADAFRUIT_NRF52840_FEATHER\\GCC_ARM-RELEASE\\${workspaceFolderBasename}_application.hex.zip",
                "-p",
                "COM7",
                "-b",
                "115200",
                "--singlebank"
            ]
        },
        {
            "label": "Build Release",
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "dependsOrder": "sequence",
            "dependsOn": ["compile release", "ada-nrfutil app hex to DFU zip", "ada-nrfutil flash DFU pkg"]
        }
    ]
``` 

# Compiling and Building

- Connect Adafruit board to computer via USB cable.
- Check the virtual COM port it opened (on my laptop was COM7) but may be different on yours. If it is not COM7 then make sure to change the tasks.json file **"ada-nrfutil flash DFU pkg"** section port to match.
- In Visual Studio Code execute a build. (SHIFT + CTRL + B).
- Whilst waiting for build press the reset button on Adafruit twice quickly to enter DFU programming mode.  Neopixel LED will be green.
- Watch Console output for compile, hex to DFU zip and eventual upload to board.



