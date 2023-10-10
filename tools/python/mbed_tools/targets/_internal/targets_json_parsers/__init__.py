#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Package for parsing Mbed OS library's targets.json.

targets.json contains the attribute data for all supported targets.

Targets are defined using inheritance. Multiple inheritance is allowed (though not encouraged).

Attributes come in two types - overriding and accumulating. These
use two completely different ways of inheriting so each has its own parser.

The combined results of these two parsers makes a complete set of attributes for
a target as defined in targets.json.

For more information about targets.json structure see
https://os.mbed.com/docs/mbed-os/latest/reference/adding-and-configuring-targets.html
"""
