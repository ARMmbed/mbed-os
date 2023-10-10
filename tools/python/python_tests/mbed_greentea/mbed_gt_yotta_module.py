"""
mbed SDK
Copyright (c) 2011-2015 ARM Limited

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

import unittest
from mbed_greentea.mbed_yotta_module_parse import YottaModule


class YOttaConfigurationParse(unittest.TestCase):

    def setUp(self):
        # greentea-client defined in 'dependencies' and 'testDependencies'
        self.YOTTA_MODULE_LONG = {
            "name": "utest",
            "version": "1.9.1",
            "description": "Simple test harness with unity and greentea integration.",
            "keywords": [
                "greentea",
                "testing",
                "unittest",
                "unity",
                "unit",
                "test",
                "asynchronous",
                "async",
                "mbed-official"
                ],
            "author": "Niklas Hauser <niklas.hauser@arm.com>",
            "license": "Apache-2.0",
            "dependencies": {
                "minar": "^1.0.0",
                "core-util": "^1.0.1",
                "compiler-polyfill": "^1.2.0",
                "mbed-drivers": "~0.12.0",
                "greentea-client": "^0.1.2"
                },
            "testDependencies": {
                "unity": "^2.0.1",
                "greentea-client": "^0.1.2"
                }
        }

        # greentea-client defined in 'dependencies'
        self.YOTTA_MODULE_LONG_IN_DEP = {
            "name": "utest",
            "version": "1.9.1",
            "description": "Simple test harness with unity and greentea integration.",
            "keywords": [
                "greentea",
                "testing",
                "unittest",
                "unity",
                "unit",
                "test",
                "asynchronous",
                "async",
                "mbed-official"
                ],
            "author": "Niklas Hauser <niklas.hauser@arm.com>",
            "license": "Apache-2.0",
            "dependencies": {
                "minar": "^1.0.0",
                "core-util": "^1.0.1",
                "compiler-polyfill": "^1.2.0",
                "mbed-drivers": "~0.12.0",
                "greentea-client": "^0.1.2"
                },
            "testDependencies": {
                "unity": "^2.0.1",
                }
        }

        # greentea-client defined in 'testDependencies'
        self.YOTTA_MODULE_LONG_IN_TESTDEP = {
            "name": "utest",
            "version": "1.9.1",
            "description": "Simple test harness with unity and greentea integration.",
            "keywords": [
                "greentea",
                "testing",
                "unittest",
                "unity",
                "unit",
                "test",
                "asynchronous",
                "async",
                "mbed-official"
                ],
            "author": "Niklas Hauser <niklas.hauser@arm.com>",
            "license": "Apache-2.0",
            "dependencies": {
                "minar": "^1.0.0",
                "core-util": "^1.0.1",
                "compiler-polyfill": "^1.2.0",
                "mbed-drivers": "~0.12.0"
                },
            "testDependencies": {
                "unity": "^2.0.1",
                "greentea-client": "^0.1.2"
                }
        }

        # No dependency to greentea-client
        self.YOTTA_MODULE_LONG_NO_DEP = {
            "name": "utest",
            "version": "1.9.1",
            "description": "Simple test harness with unity and greentea integration.",
            "keywords": [
                "greentea",
                "testing",
                "unittest",
                "unity",
                "unit",
                "test",
                "asynchronous",
                "async",
                "mbed-official"
                ],
            "author": "Niklas Hauser <niklas.hauser@arm.com>",
            "license": "Apache-2.0",
            "dependencies": {
                "minar": "^1.0.0",
                "core-util": "^1.0.1",
                "compiler-polyfill": "^1.2.0",
                "mbed-drivers": "~0.12.0"
                },
            "testDependencies": {
                "unity": "^2.0.1"
                }
        }

        # Yotta module itself is 'greentea-client'
        self.GREENTEA_CLIENT_MODULE = {
            "name": "greentea-client",
            "version": "0.1.6",
            "description": "greentea client for mbed devices.",
            "keywords": [
                "greentea",
                "greentea-client",
                "mbedgt"
                ],
            "author": "Przemyslaw.Wirkus <Przemyslaw.Wirkus@arm.com>",
            "homepage": "https://github.com/ARMmbed/greentea-client",
            "repository": {
                "url": "git@github.com:ARMmbed/greentea-client.git",
                "type": "git"
            },
            "license": "Apache-2.0",
            "dependencies": {
            },
            "testDependencies": {
                "utest": "^1.10.0",
                "unity": "^2.0.0"
            }
        }



    def tearDown(self):
        pass

    def test_get_name(self):
        yotta_module = YottaModule()
        # Modules using Greentea >= v0.2.0
        for module_json in [self.YOTTA_MODULE_LONG,
                            self.YOTTA_MODULE_LONG_IN_DEP,
                            self.YOTTA_MODULE_LONG_IN_TESTDEP,
                            self.YOTTA_MODULE_LONG_NO_DEP]:
            yotta_module.set_yotta_module(module_json)
            self.assertEqual('utest', yotta_module.get_name())

        # 'greentea-client' module itself
        yotta_module.set_yotta_module(self.GREENTEA_CLIENT_MODULE)
        self.assertEqual('greentea-client', yotta_module.get_name())

    def test_get_dict_items(self):
        yotta_module = YottaModule()

        yotta_module.set_yotta_module(self.YOTTA_MODULE_LONG)
        self.assertEqual('Simple test harness with unity and greentea integration.', yotta_module.get_data().get('description'))
        self.assertEqual('Apache-2.0', yotta_module.get_data().get('license'))

    def test_check_greentea_client_in_dep(self):
        yotta_module = YottaModule()

        yotta_module.set_yotta_module(self.YOTTA_MODULE_LONG)
        self.assertTrue(yotta_module.check_greentea_client())

        yotta_module.set_yotta_module(self.YOTTA_MODULE_LONG_IN_DEP)
        self.assertTrue(yotta_module.check_greentea_client())

        yotta_module.set_yotta_module(self.GREENTEA_CLIENT_MODULE)
        self.assertTrue(yotta_module.check_greentea_client())

        yotta_module.set_yotta_module(self.YOTTA_MODULE_LONG_NO_DEP)
        self.assertFalse(yotta_module.check_greentea_client())

    def test_check_greentea_client_in_test_dep(self):
        yotta_module = YottaModule()

        yotta_module.set_yotta_module(self.YOTTA_MODULE_LONG)
        self.assertTrue(yotta_module.check_greentea_client())

        yotta_module.set_yotta_module(self.YOTTA_MODULE_LONG_IN_TESTDEP)
        self.assertTrue(yotta_module.check_greentea_client())

        yotta_module.set_yotta_module(self.GREENTEA_CLIENT_MODULE)
        self.assertTrue(yotta_module.check_greentea_client())

        yotta_module.set_yotta_module(self.YOTTA_MODULE_LONG_NO_DEP)
        self.assertFalse(yotta_module.check_greentea_client())


if __name__ == '__main__':
    unittest.main()
