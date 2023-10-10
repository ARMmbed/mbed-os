#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#

import tempfile
import pathlib
import json

from unittest import TestCase

from click.testing import CliRunner

from mbed_tools.cli.configure import configure

target_json = json.dumps(
    {
        "TARGET": {
            "core": None,
            "trustzone": False,
            "default_toolchain": "ARM",
            "supported_toolchains": None,
            "extra_labels": [],
            "supported_form_factors": [],
            "components": [],
            "is_disk_virtual": False,
            "macros": [],
            "device_has": [],
            "features": [],
            "detect_code": [],
            "c_lib": "std",
            "bootloader_supported": False,
            "static_memory_defines": True,
            "printf_lib": "minimal-printf",
            "supported_c_libs": {"arm": ["std"], "gcc_arm": ["std", "small"], "iar": ["std"]},
            "supported_application_profiles": ["full"],
        }
    }
)

mbed_app_json = json.dumps(
    {"target_overrides": {"*": {"target.c_lib": "small", "target.printf_lib": "minimal-printf"}}}
)


class TestConfigureRegression(TestCase):
    def test_generate_config_called_with_correct_arguments(self):
        with tempfile.TemporaryDirectory() as tmpDir:
            tmpDirPath = pathlib.Path(tmpDir)
            pathlib.Path(tmpDirPath / "mbed-os.lib").write_text("https://github.com/ARMmbed/mbed-os")
            pathlib.Path(tmpDirPath / "mbed_app.json").write_text(mbed_app_json)
            pathlib.Path(tmpDirPath / "mbed-os").mkdir()
            pathlib.Path(tmpDirPath / "mbed-os" / "targets").mkdir()
            pathlib.Path(tmpDirPath / "mbed-os" / "targets" / "targets.json").write_text(target_json)

            result = CliRunner().invoke(
                configure, ["-m", "Target", "-t", "gcc_arm", "-p", tmpDir], catch_exceptions=False
            )
            self.assertIn("mbed_config.cmake has been generated and written to", result.output)
