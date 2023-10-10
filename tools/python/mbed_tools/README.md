# Mbed Tools

![Package](https://badgen.net/badge/Package/mbed-tools/grey)
[![Documentation](https://badgen.net/badge/Documentation/GitHub%20Pages/blue?icon=github)](https://armmbed.github.io/mbed-tools/api/)
[![PyPI](https://badgen.net/pypi/v/mbed-tools)](https://pypi.org/project/mbed-tools/)
[![PyPI - Status](https://img.shields.io/pypi/status/mbed-tools)](https://pypi.org/project/mbed-tools/)
[![PyPI - Python Version](https://img.shields.io/pypi/pyversions/mbed-tools)](https://pypi.org/project/mbed-tools/)

[![License](https://badgen.net/pypi/license/mbed-tools)](https://github.com/ARMmbed/mbed-tools/blob/master/LICENSE)

[![Build Status](https://dev.azure.com/mbed-tools/mbed-tools/_apis/build/status/Build%20and%20Release?branchName=master&stageName=CI%20Checkpoint)](https://dev.azure.com/mbed-tools/mbed-tools/_build/latest?definitionId=10&branchName=master)
[![Test Coverage](https://codecov.io/gh/ARMmbed/mbed-tools/branch/master/graph/badge.svg)](https://codecov.io/gh/ARMmbed/mbed-tools)
[![Maintainability](https://api.codeclimate.com/v1/badges/b9fca0e16f7a85da7674/maintainability)](https://codeclimate.com/github/ARMmbed/mbed-tools/maintainability)

## Overview

This is the **future** command line tool for Mbed OS. It provides the ability to detect Mbed Enabled devices connected
by USB, checkout Mbed projects and perform builds amongst other operations.

> :warning: While this package is generally available it is not complete. The available functionality can be viewed with
> the `--help` option once installed. Please note that the current tools for Mbed OS 5.x and above can be found at
> https://github.com/ARMmbed/mbed-cli.

## Releases

For release notes and a history of changes of all **production** releases, please see the following:

- [Changelog](https://github.com/ARMmbed/mbed-tools/blob/master/CHANGELOG.md)

For a the list of all available versions please, please see the:

- [PyPI Release History](https://pypi.org/project/mbed-tools/#history)

## Versioning

The version scheme used follows [PEP440](https://www.python.org/dev/peps/pep-0440/) and
[Semantic Versioning](https://semver.org/). For production quality releases the version will look as follows:

- `<major>.<minor>.<patch>`

Pre releases are used to give early access to new functionality, for testing and to get feedback on experimental
features. As such these releases may not be stable and should not be used for production. Additionally any interfaces
introduced in a pre release may be removed or changed without notice. For pre releases the version will look as
follows:

- `<major>.<minor>.<patch>.dev<pre-release-number>`

## Installation

`mbed-tools` relies on the Ninja build system and CMake.
- CMake. [Install version 3.19.0 or newer for all operating systems](https://cmake.org/install/).
- Ninja. [Install version 1.0 or newer for all operating systems](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages).

We recommend installing `mbed-tools` in a Python virtual environment to avoid dependency conflicts.

To install the most recent production quality release use:

```
pip install mbed-tools
```

To install a specific release:

```
pip install mbed-tools==<version>
```

## Usage

Interface definition and usage documentation (for developers of Mbed OS tooling) is available for the most recent
production release here:

- [GitHub Pages](https://armmbed.github.io/mbed-tools/api/)

## Project Structure

The follow described the major aspects of the project structure:

- `azure-pipelines/` - CI configuration files for Azure Pipelines.
- `src/mbed_tools/` - Python source files.
- `news/` - Collection of news files for unreleased changes.
- `tests/` - Unit and integration tests.

## Getting Help

- For interface definition and usage documentation, please see [GitHub Pages](https://armmbed.github.io/mbed-tools/api/).
- For a list of known issues and possible work arounds, please see [Known Issues](KNOWN_ISSUES.md).
- To raise a defect or enhancement please use [GitHub Issues](https://github.com/ARMmbed/mbed-tools/issues).
- To ask a question please use the [Mbed Forum](https://forums.mbed.com/).

## Contributing

- Mbed OS is an open source project and we are committed to fostering a welcoming community, please see our
  [Code of Conduct](https://github.com/ARMmbed/mbed-tools/blob/master/CODE_OF_CONDUCT.md) for more information.
- For how to contribute to the project, including how to develop the project,
  please see the [Contributions Guidelines](https://github.com/ARMmbed/mbed-tools/blob/master/CONTRIBUTING.md)
