"""
Copyright 2018 ARM Limited
SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import os
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..",
                                    ".."))
sys.path.insert(0, ROOT)

from tools.run_icetea import find_build_from_build_data, filter_test_by_build_data, filter_test_by_name, \
    get_application_list, set_allowed_platform

"""
Unit tests for run_icetea.py
"""

test_build_data = {
    'builds': [
        {
            "id": "TEST_APPS-DEVICE-SOCKET_APP",
            "target_name": "K64F",
            "toolchain_name": "GCC_ARM",
            "result": "OK"
        }
    ]
}


def test_find_build_from_build_data_empty():
    assert find_build_from_build_data(build_data={'builds': []}, id="something", target="K64F",
                                      toolchain="GCC_ARM") is None


def test_find_build_from_build_data_wrong_target():
    assert find_build_from_build_data(build_data=test_build_data, id="TEST_APPS-DEVICE-SOCKET_APP", target="AAAA",
                                      toolchain="GCC_ARM") is None


def test_find_build_from_build_data():
    assert find_build_from_build_data(build_data=test_build_data, id="TEST_APPS-DEVICE-SOCKET_APP", target="K64F",
                                      toolchain="GCC_ARM") is not None


icetea_json_output = [
    {
        "status": "released",
        "requirements": {
            "duts": {
                "1": {
                    "nick": "dut1"
                },
                "*": {
                    "count": 1,
                    "application": {
                        "bin": None,
                        "name": "TEST_APPS-device-socket_app"
                    },
                    "type": "hardware"
                }
            },
            "external": {
                "apps": []
            }
        },
        "name": "UDPSOCKET_BIND_PORT",
        "filepath": "/Users/test/mbed-os/TEST_APPS/testcases/SOCKET_BIND_PORT.py",
        "title": "udpsocket open and bind port",
        "component": [
            "mbed-os",
            "netsocket"
        ],
        "compatible": {
            "framework": {
                "version": ">=1.0.0",
                "name": "Icetea"
            },
            "hw": {
                "value": True
            },
            "automation": {
                "value": True
            }
        },
        "subtype": "socket",
        "purpose": "Verify UDPSocket can be created, opened and port binded",
        "type": "smoke",
        "sub_type": None
    }
]


def test_filter_test_by_build_data_when_data_is_empty():
    assert filter_test_by_build_data(
        icetea_json_output=icetea_json_output,
        build_data=None,
        target="K64F",
        toolchain="GCC_ARM"
    ) == icetea_json_output


def test_filter_test_by_build_data():
    temp = filter_test_by_build_data(
        icetea_json_output=icetea_json_output,
        build_data=test_build_data,
        target="K64F",
        toolchain="GCC_ARM"
    )
    assert len(temp) > 0


def test_filter_test_by_name():
    assert len(filter_test_by_name(icetea_json_output, ['UDPSOCKET_BIND_PORT'])) > 0


def test_filter_test_by_name_when_not_found():
    assert filter_test_by_name(icetea_json_output, ['AAA']) == list()


def test_filter_test_by_name_when_name_is_empty():
    assert filter_test_by_name(icetea_json_output, None) == icetea_json_output


def test_get_application_list():
    assert 'TEST_APPS-device-socket_app' in get_application_list(icetea_json_output, ['UDPSOCKET_BIND_PORT'])


def test_get_application_list_not_found():
    assert 'TEST_APPS-device-socket_app' not in get_application_list(icetea_json_output, ['SOMETHING_ELSE'])


def test_get_application_list_none():
    assert 'TEST_APPS-device-socket_app' in get_application_list(icetea_json_output, None)


def test_set_allowed_platform_simple():
    ret = set_allowed_platform({"duts": {}}, "K66F")
    assert ret['duts']['*']['allowed_platforms'] == ["K66F"]


def test_set_allowed_platform_normal():
    ret = set_allowed_platform({
        "duts": {
            "*": {
                "count": 3,
                "allowed_platforms": ["K64F"],
                "application": {"bin": "hex.bin"}
            },
            1: {"application": {"bin": "my_hex.bin"}},
            2: {"application": {"bin": "my_hex2.bin"}}
        }
    }, "K66F")
    assert ret['duts']['*']['allowed_platforms'] == ["K66F"]


def test_set_allowed_platform_no_changes():
    temp = {
        "duts": {
            "*": {
                "count": 3,
                "allowed_platforms": ["K64F"],
                "application": {"bin": "hex.bin"}
            },
        }
    }
    assert temp == set_allowed_platform(temp, "K64F")
