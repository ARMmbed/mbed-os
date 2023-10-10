# Development moved

The development of Mbed LS has been moved into the [mbed-os-tools](../../src/mbed_os_tools) package. You can continue to use this module for legacy reasons, however all further development should be continued in the new package.

-------------

[![PyPI version](https://badge.fury.io/py/mbed-ls.svg)](https://badge.fury.io/py/mbed-ls)

# Mbed LS

Mbed LS is a Python (2 and 3) module that detects and lists Mbed Enabled devices connected to the host computer. The Mbed OS team publishes Mbed LS on PyPI. It works on all major operating systems (Windows, Linux and Mac OS X).

It provides the following information for all connected boards in a console (terminal) output:

- Mbed OS platform name.
- Mount point (MSD or disk).
- Serial port.

# Installation

## Installation from PyPI (Python Package Index)

To install Mbed LS from [PyPI](https://pypi.python.org/pypi/mbed-ls), run the following command:

```bash
$ pip install mbed-ls --upgrade
```

## Installation from Python sources

**Prerequisites:** You need to have [Python 2.7.x](https://www.python.org/download/releases/2.7/) or [Python 3.6.x](https://www.python.org/downloads/release/python-362/) installed on your system.

**Note:** If your OS is Windows, please follow the installation instructions [for the serial port driver](https://os.mbed.com/docs/latest/tutorials/windows-serial-driver.html).

Install Mbed LS from sources with the following commands:

```bash
$ git clone https://github.com/ARMmbed/mbed-os-tools.git
$ cd mbed-os-tools/packages/mbed-ls
$ python setup.py install
```

# Command-line

The command-line tool is available with the command `mbedls`.

**Note:** [Mbed CLI](https://github.com/armmbed/mbed-cli) has a similarly-named command `mbed ls`; however, the commands are different. Be sure to omit the space when using the Mbed LS command-line tool.

```bash
$ mbedls
+---------------+----------------------+-------------+-------------+--------------------------------------------------+-----------------+
| platform_name | platform_name_unique | mount_point | serial_port | target_id                                        | daplink_version |
+---------------+----------------------+-------------+-------------+--------------------------------------------------+-----------------+
| K64F          | K64F[0]              | D:          | COM18       | 0240000032044e4500257009997b00386781000097969900 | 0244            |
+---------------+----------------------+-------------+-------------+--------------------------------------------------+-----------------+
```

## Result formats

The Mbed LS command-line accepts a few arguments to change the format of the results. The default format is a table. You may pass `--simple` to simplify this table format, and `--json` to print the table as a json list of the rows.

### Simple (no table formatting)

```
$ mbedls --simple
 K64F  K64F[0]  D:  COM18  0240000032044e4500257009997b00386781000097969900  0244
```

### JSON

```bash
$ mbedls --json
[
    {
        "daplink_auto_reset": "0",
        "daplink_automation_allowed": "1",
        "daplink_bootloader_crc": "0xa65218eb",
        "daplink_bootloader_version": "0242",
        "daplink_daplink_mode": "Interface",
        "daplink_git_sha": "67f8727a030bcc585e982d899fb6382db56d673b",
        "daplink_hic_id": "97969900",
        "daplink_interface_crc": "0xe4422294",
        "daplink_interface_version": "0244",
        "daplink_local_mods": "0",
        "daplink_overflow_detection": "1",
        "daplink_remount_count": "0",
        "daplink_unique_id": "0240000032044e4500257009997b00386781000097969900",
        "daplink_usb_interfaces": "MSD, CDC, HID",
        "daplink_version": "0244",
        "mount_point": "D:",
        "platform_name": "K64F",
        "platform_name_unique": "K64F[0]",
        "product_id": "0204",
        "serial_port": "COM18",
        "target_id": "0240000032044e4500257009997b00386781000097969900",
        "target_id_mbed_htm": "0240000032044e4500257009997b00386781000097969900",
        "target_id_usb_id": "0240000032044e4500257009997b00386781000097969900",
        "vendor_id": "0d28"
    }
]
```

## Mocking (renaming) platforms

Override a platform's name using the `--mock` parameter:

```
$ mbedls --mock 0240:MY_NEW_PLATFORM
$ mbedls
+-----------------+----------------------+-------------+-------------+--------------------------------------------------+-----------------+
| platform_name   | platform_name_unique | mount_point | serial_port | target_id                                        | daplink_version |
+-----------------+----------------------+-------------+-------------+--------------------------------------------------+-----------------+
| MY_NEW_PLATFORM | MY_NEW_PLATFORM[0]   | D:          | COM18       | 0240000032044e4500257009997b00386781000097969900 | 0244            |
+-----------------+----------------------+-------------+-------------+--------------------------------------------------+-----------------+
```

The `--mock` parameter accepts a platform ID and a platform name, separated by the `:` character. The platform ID is the first 4 characters of the `target_id`. The platform name is the name you are temporarily assigning to this platform.

To remove a mocked platform, use the `--mock` parameter again. Continuing from the previous example, use `-<platform id>` as the value:

```
$ mbedls --mock -0240
$ mbedls
+---------------+----------------------+-------------+-------------+--------------------------------------------------+-----------------+
| platform_name | platform_name_unique | mount_point | serial_port | target_id                                        | daplink_version |
+---------------+----------------------+-------------+-------------+--------------------------------------------------+-----------------+
| K64F          | K64F[0]              | D:          | COM18       | 0240000032044e4500257009997b00386781000097969900 | 0244            |
+---------------+----------------------+-------------+-------------+--------------------------------------------------+-----------------+
```

You can also remove all mocked platforms by supplying `*` as the `target_id`:

```
$ mbedls --mock="-*"
```

**NOTE:** Due to a quirk in the parameter formatting, the command-line can interpret `-*` as another parameter instead of a value. It is necessary to use the complete `--mock="-*"` syntax, so the command-line interprets each part of the command correctly.

## Retargeting platforms

It is possible to change the returned results for certain platforms depending on the current directory. This is especially useful when developing new platforms.

The command-line tool and Python API check the current directory for a file named `mbedls.json`. When it is present, it overrides the returned values. The format of the `mbedls.json` file is:

```json
{
    "<target_id>": {
        "<key>": "<value>"
    }
}
```

For example, to change the `serial_port` of the K64F with a `target_id` of `0240000032044e4500257009997b00386781000097969900`, the `mbedls.json` file contains the following:

```json
{
    "0240000032044e4500257009997b00386781000097969900": {
        "serial_port": "COM99"
    }
}
```

This results in the following output from the command-line tool:

```bash
$ mbedls
+---------------+----------------------+-------------+-------------+--------------------------------------------------+-----------------+
| platform_name | platform_name_unique | mount_point | serial_port | target_id                                        | daplink_version |
+---------------+----------------------+-------------+-------------+--------------------------------------------------+-----------------+
| K64F          | K64F[0]              | D:          | COM99       | 0240000032044e4500257009997b00386781000097969900 | 0244            |
+---------------+----------------------+-------------+-------------+--------------------------------------------------+-----------------+
```

Note how the `serial_port` value changed from `COM18` to `COM99`. Deleting the `mbedls.json` or using the `--skip-retarget` parameter removes these changes.

# Python API

The Python API is available through the `mbed_lstools` module.

## `mbed_lstools.create(...)`

```python
>>> import mbed_lstools
>>> mbeds = mbed_lstools.create()
>>> mbeds
<mbed_lstools.lstools_win7.MbedLsToolsWin7 instance at 0x02F542B0>
```

This returns an instance that provides access to the rest of the API.

### Arguments

#### `skip_retarget`

**Default:** `False`

When set to `True`, this skips the retargetting step, and the results are unmodified. This enables the same behavior as the `--skip-retarget` command-line flag.

#### `list_unmounted`

**Default:** `False`

When set to `True`, this includes unmounted platforms in the results. This enables the same behavior as the `-u` command-line flag.

## `mbeds.list_mbeds(...)`

```python
>>> import mbed_lstools
>>> mbeds = mbed_lstools.create()
>>> mbeds.list_mbeds(fs_interaction=FSInteraction.BeforeFilter,
                                    filter_function=None,
                                    unique_names=False,
                                    read_details_txt=False)
[{'target_id_mbed_htm': u'0240000032044e4500257009997b00386781000097969900', 'mount_point': 'D:', 'target_id': u'0240000032044e4500257009997b00386781000097969900', 'serial_port': u'COM18', 'target_id_usb_id': u'0240000032044e4500257009997b00386781000097969900', 'platform_name': u'K64F'}]
```

### Arguments

#### `filter_function`

**Default:** `None`

This function allows you to filter results based on platform data. This can hasten the execution of the `list_mbeds` function.

As a normal function definition:

```python
def filter_func(mbed):
    return m['platform_name'] == 'K64F'

mbeds.list_mbeds(filter_function=filter_func)
```

As a lambda function:

```python
platforms = mbeds.list_mbeds(filter_function=lambda m: m['platform_name'] == 'K64F')
```

#### `fs_interaction`

**Default:** `FSInteraction.BeforeFilter`

This argument controls the accuracy and speed of this function. There are three choices (in ascending order of accuracy and decreasing order of speed):

- `FSInteraction.NEVER` - This is the fastest option but also potentially the least accurate. It never touches the file system of the devices. It uses only the information available through the USB descriptors. This is appropriate for use in a highly controlled environment (such as an automated Continuous Integration setup). **This has the potential to provide incorrect names and data. It may also lead to devices not being detected at all.**
- `FSInterfaction.AfterFilter` - This accesses the file system but only after application of the `filter_function`. This can lead to speed increases but at the risk of filtering on inaccurate information.
- `FSInteraction.BeforeFilter` - This accesses the file system before doing any filtering. It is the most accurate option and is recommended for most uses. This is the default behavior of the command-line tool and the API.

#### `unique_names`

**Default:** `False`.

Mbed LS assigns a unique name to each platform when this is set to `True`. The unique name takes the form of `K64F[0]`, where the number between the brackets is an incrementing value. This name is accessible through the dictionary member `platform_unique_name` in the returned platform data.

#### `read_details_txt`

**Default:** `False`

Mbed LS reads more data from the file system on each device when this is set to `True`. It can provide useful management data but also takes more time to execute.

## `mbeds.mock_manufacture_id(...)`

```python
>>> import mbed_lstools
>>> mbeds = mbed_lstools.create()
>>> mbeds.mock_manufacture_id('0240', 'CUSTOM_PLATFORM', oper='+')
>>> mbeds.list_mbeds()
[{'target_id': u'0240000032044e4500257009997b00386781000097969900', ... 'platform_name': u'CUSTOM_PLATFORM'}]
>>> mbeds.mock_manufacture_id('0240', '', oper='-')
>>> mbeds.list_mbeds()
[{'target_id': u'0240000032044e4500257009997b00386781000097969900', ... 'platform_name': u'K64F'}]
```

### Arguments

#### `mid`

**Required**

The first four characters of the `target_id` that you want to mock.

#### `platform_name`

**Required**

Overrides the `platform_name` for any platform with a `target_id` that starts with `mid`.

#### `oper`

**Default:** `'+'`

If set to `'+'`, the mocked platform is enabled. If `'-'`, the mocked platform is disabled.


## `mbeds.get_supported_platforms(...)`

```python
>>> import mbed_lstools
>>> mbeds = mbed_lstools.create()
>>> mbeds.get_supported_platforms(device_type='daplink')
{'0240': 'K64F', '0311': 'K66F'}
```

### Arguments

#### `device_type`

**Default:** `'daplink'`

Chooses which device type entries are retrieved from the platform database.

## Logging

Mbed LS uses the Python `logging` module for all of its logging needs. Mbed LS uses the logger `"mbedls"` as its root, and all other loggers start with `"mbedls."`. Configuring the Python root logger automatically redirects all of the Mbed LS logs to the configured endpoint. When using the Python API, configure logging, such as by calling `logging.basicConfig()`.

# Testing

The `/test` directory contains all tests. You can run the tests with the following command:

```
$ python setup.py test
```

## Code coverage

The `coverage` Python package measures code coverage. You can install it with following command:

```
$ pip install coverage --upgrade
```

To run the tests while measuring code coverage, use the following command:

```
$ coverage run setup.py test
```

You can then generate a report:

```
$ coverage report
Name                                Stmts   Miss  Cover
-------------------------------------------------------
mbed_lstools\__init__.py                2      0   100%
mbed_lstools\darwin.py                 85      7    92%
mbed_lstools\linux.py                  45      3    93%
mbed_lstools\lstools_base.py          299    124    59%
mbed_lstools\main.py                  134     44    67%
mbed_lstools\platform_database.py     114      4    96%
mbed_lstools\windows.py                98     21    79%
-------------------------------------------------------
TOTAL                                 777    203    74%
```

# OS-specific behavior

## Windows

The Mbed serial port works by default on Mac and Linux, but Windows needs a driver. Check [here](https://os.mbed.com/docs/latest/tutorials/windows-serial-driver.html) for more details.

## Linux

Mbed LS requires you to mount a platform before it shows up in the results. Many Linux systems do not automatically mount USB devices. We recommend you use an automounter to manage this for you.

There are many automounters available, and it is ultimately up to you to determine which is the best one for your use case. However, the `usbmount` package on Ubuntu makes it easy to start. If you need more control over your automounter, you can build and run an open source project called [ldm](https://github.com/LemonBoy/ldm).

# Mbed Enabled technical requirements overview

This tool relies on board interfaces conforming to certain standards, so it can detect platforms properly. The [Mbed Enabled](https://www.mbed.com/en/about-mbed/mbed-enabled/) program sets these standards. Please see the [Technical Requirements](https://www.mbed.com/en/about-mbed/mbed-enabled/mbed-enabled-program-requirements/) for more information.

## Device unique identifier

Each device must have a unique identifier. This identifier has two parts: a **platform ID** and a **platform unique string**.

The **platform ID** contains four ASCII characters containing only hexadecimal values (A-F and 0-9). This platform ID is the same for all platforms of the same type. For example, all `K64F` platforms have a platform ID of `0240`. `mbedls` uses this to identify the platform.

The **platform unique string** can be any length of characters (a-z, A-Z and 0-9) that you can use to uniquely identify platforms of the same type on the same machine. For example, two FRDM-K64F platforms attached to the same machine could have the following attributes:

```
$ mbedls
+---------------+----------------------+-------------+-------------+--------------------------------------------------+-----------------+
| platform_name | platform_name_unique | mount_point | serial_port | target_id                                        | daplink_version |
+---------------+----------------------+-------------+-------------+--------------------------------------------------+-----------------+
| K64F          | K64F[0]              | D:          | COM18       | 0240000032044e4500257009997b00386781000097969900 | 0244            |
| K64F          | K64F[1]              | E:          | COM19       | 0240000032044e4500257009997b00386781000097840023 | 0244            |
+---------------+----------------------+-------------+-------------+--------------------------------------------------+-----------------+
```

Note how both platforms share the same platform ID (`0240`) but have a unique ending string.

# Adding platform support

If a platform meets the Mbed Enabled technical requirements (stated above), it can be added to Mbed LS.

## Adding a new platform with a supported debugger

Mbed LS currently supports the following types of debuggers:

- [DAPLink](https://github.com/ARMmbed/DAPLink)
   - As well as the related but legacy [CMSIS-DAP](https://github.com/mbedmicro/CMSIS-DAP) firmware
- ST-LINK
- J-Link

### Adding support for DAPLink-compatible platforms (DAPLink, ST-LINK, and CMSIS-DAP)

Add an entry to the `daplink` section of the [`DEFAULT_PLATFORM_DB`](../../src/mbed_os_tools/detect/platform_database.py).

If your platform's name is `NEW_PLATFORM` and it has platform ID of `9999`, the new entry should be:

```
DEFAULT_PLATFORM_DB = {
    u'daplink': {
        ...
        u'9999': u'NEW_PLATFORM',
        ...
    }
}
```

Please order the entries by the platform ID when adding new platforms.

### Adding support for J-Link platforms

J-Link detection works differently due to the information present on the platform's filesystem. All new entries should be added to the `jlink` section of the [`DEFAULT_PLATFORM_DB`](../../src/mbed_os_tools/detect/platform_database.py).

The following is an example `jlink` platform entry:

```
DEFAULT_PLATFORM_DB = {
    ...
    u'jlink': {
        u'X729475D28G': {
            u'platform_name': u'NRF51_DK',
            u'jlink_device_name': u'nRF51422_xxAC'
        },
        ...
    }
}
```

Instead of a platform ID, there is a target-unique string (`X729475D28G` in this case). This should correspond with the unique part of the link present in the `Board.html` or `User Guide.html`. This seems to vary among the platforms. In general, try following the links in each file. You want to use the url that links to a product page that references the platform. The J-Link logic in Mbed LS assumes that the url has the target-unique string on the end (after the last `/` character). In the above example, the expected url structure would be `http://www.nordicsemi.com/X729475D28G`.

If your J-Link platform does not follow this convention, please raise an issue with the following information:

- The name of the platform
- The file **names and contents** present on the platform's filesystem
- A link to the J-Link firmware binary if possible

## Adding a new type of debugger

The type of debugger present on the platform affects how it is detected. The USB Vendor ID is used to detect which type of debugger is present on the platform.

If a new type of debugger is being introduced to Mbed LS with the platform, you will need to add the Vendor ID to the [identification map](../../src/mbed_os_tools/detect/lstools_base.py). You will also need to assign the correct "update from the filesystem" logic [here](../../src/mbed_os_tools/detect/lstools_base.py). If the debugger is compatible with the files presented by DAPLink, you may reuse that implementation when updating the device information from the filesystem. If it is not, you may need to write your own update logic. If you need guidance on this, please ask for it when you submit an issue or a pull request.
