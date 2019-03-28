# Mbed OS Build Tools

This directory contains the python tools used for building Mbed OS and
Mbed 2. 

Quick navigation:

| file/directory        | Purpose/function                               |
|-----------------------|------------------------------------------------|
| `build.py`            | implementation of `mbed compile --library`     |
| `build_api.py`        | full-project build operations                  |
| `build_release.py`    | CLI for making an mbed 2 release               |
| `config`              | implementation of Config System                |
| `debug_tools`         | Crash log parsing                              |
| `default_settings.py` | Default version of project local `settings.py` |
| `detect_targets.py`   | implementation of `mbed detect`                |
| `device_management.py`| implementation of `mbed device-management`     |
| `export`              | Export plugins and API for woking with them    | 
| `flash_algo`          | CMSIS flash algorithm parser                   |
| `get_config.py`       | implementation of `mbed compile --config`      |
| `host_tests`          | location of pre-htrun host tests               |
| `importer`            | code importer for use with CMSIS, TFM/PSA etc. |
| `libraries.py`        | constants for building mbed 2 libraries        |
| `make.py`             | implementation of `mbed compile`               |
| `memap.py`            | map file parser and sumary generator           |
| `notifier`            | API for seting compile status to a frontend    |
| `options.py`          | Default option parser and option utilities     |
| `paths.py`            | constants for many paths used                  |
| `profiles`            | location of the default build profiles         |
| `project.py`          | implementation of `mbed export`                |
| `psa`                 | PSA                                            |
| `regions.py`          | merging from managed bootloader mode           |
| `resources`           | scans directories for files used in a project  |
| `run_icetea.py`       | implementation of `mbed test --icetea`         |
| `settings.py`         | project specific settings from env vars        |
| `singletest.py`       | location of pre-greentea greentea              |
| `targets`             | target description reader and post-build steps |
| `test`                | unit tests for tools                           |
| `test_api.py`         | part of pre-greentea greentea                  |
| `test_configs`        | configuration files used by `mbed test`        |
| `test_exporters.py`   | part of pre-greentea greentea                  |
| `tests.py`            | implementation of `mbed test --greentea`       |
| `toolchains`          | API for calling the selected compiler          |
| `utils.py`            | General purpose utilities like file moving     |

