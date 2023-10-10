#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import os
from unittest import TestCase, mock

from mbed_tools.targets.env import env


class TestMbedApiAuthToken(TestCase):
    @mock.patch.dict(os.environ, {"MBED_API_AUTH_TOKEN": "sometoken"})
    def test_returns_api_token_set_in_env(self):
        self.assertEqual(env.MBED_API_AUTH_TOKEN, "sometoken")


class TestDatabaseMode(TestCase):
    @mock.patch.dict(os.environ, {"MBED_DATABASE_MODE": "ONLINE"})
    def test_returns_database_mode_set_in_env(self):
        self.assertEqual(env.MBED_DATABASE_MODE, "ONLINE")

    def test_returns_default_database_mode_if_not_set_in_env(self):
        self.assertEqual(env.MBED_DATABASE_MODE, "AUTO")
