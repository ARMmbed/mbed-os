"""
mbed SDK
Copyright (c) 2011-2018 ARM Limited

Licensed under the Apache License, Version 2.0 (the 'License');
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an 'AS IS' BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import unittest
import os
import errno
import logging
import tempfile
import json
from mock import patch, MagicMock, DEFAULT
from io import StringIO

from mbed_lstools.platform_database import PlatformDatabase, DEFAULT_PLATFORM_DB,\
    LOCAL_PLATFORM_DATABASE

try:
    unicode
except NameError:
    unicode = str

class EmptyPlatformDatabaseTests(unittest.TestCase):
    """ Basic test cases with an empty database
    """

    def setUp(self):
        self.base_db_path = os.path.join(tempfile.mkdtemp(), 'base')
        self.base_db = open(self.base_db_path, 'w+b')
        self.base_db.write(b'{}')
        self.base_db.seek(0)
        self.pdb = PlatformDatabase([self.base_db_path])

    def tearDown(self):
        self.base_db.close()

    def test_broken_database_io(self):
        """Verify that the platform database still works without a
        working backing file
        """
        with patch("mbed_os_tools.detect.platform_database.open") as _open:
            _open.side_effect = IOError("Bogus")
            self.pdb = PlatformDatabase([self.base_db_path])
            self.pdb.add("1234", "MYTARGET")
            self.assertEqual(self.pdb.get("1234"), "MYTARGET")

    def test_broken_database_bad_json(self):
        """Verify that the platform database still works without a
        working backing file
        """
        self.base_db.write(b'{}')
        self.base_db.seek(0)
        self.pdb = PlatformDatabase([self.base_db_path])
        self.pdb.add("1234", "MYTARGET")
        self.assertEqual(self.pdb.get("1234"), "MYTARGET")

    def test_broken_database(self):
        """Verify that the platform database correctly reset's its database
        """
        with patch("mbed_os_tools.detect.platform_database.open") as _open,\
             patch("mbed_os_tools.detect.platform_database._older_than_me") as _older:
            _older.return_value = False
            stringio = MagicMock()
            _open.side_effect = (IOError("Bogus"), stringio)
            self.pdb = PlatformDatabase([LOCAL_PLATFORM_DATABASE])
            stringio.__enter__.return_value.write.assert_called_with(
                unicode(json.dumps(DEFAULT_PLATFORM_DB)))
            self.pdb.add("1234", "MYTARGET")
            self.assertEqual(self.pdb.get("1234"), "MYTARGET")

    def test_extra_broken_database(self):
        """Verify that the platform database falls back to the built in database
        even when it can't write to disk
        """
        with patch("mbed_os_tools.detect.platform_database.open") as _open:
            _open.side_effect = IOError("Bogus")
            self.pdb = PlatformDatabase([LOCAL_PLATFORM_DATABASE])
            self.pdb.add("1234", "MYTARGET")
            self.assertEqual(self.pdb.get("1234"), "MYTARGET")

    def test_old_database(self):
        """Verify that the platform database correctly updates's its database
        """
        with patch("mbed_os_tools.detect.platform_database.open") as _open,\
             patch("mbed_os_tools.detect.platform_database.getmtime") as _getmtime:
            file_mock = MagicMock()
            file_mock.read.return_value = ''
            _open.return_value.__enter__.return_value = file_mock
            _getmtime.side_effect = (0, 1000000)
            self.pdb = PlatformDatabase([LOCAL_PLATFORM_DATABASE])
            file_mock.write.assert_called_with(
                unicode(json.dumps(DEFAULT_PLATFORM_DB)))

    def test_bogus_database(self):
        """Basic empty database test
        """
        self.assertEqual(list(self.pdb.items()), [])
        self.assertEqual(list(self.pdb.all_ids()), [])
        self.assertEqual(self.pdb.get('Also_Junk', None), None)

    def test_add(self):
        """Test that what was added can later be queried
        """
        self.assertEqual(self.pdb.get('4753', None), None)
        self.pdb.add('4753', 'Test_Platform', permanent=False)
        self.assertEqual(self.pdb.get('4753', None), 'Test_Platform')

    def test_remove(self):
        """Test that once something is removed it no longer shows up when queried
        """
        self.assertEqual(self.pdb.get('4753', None), None)
        self.pdb.add('4753', 'Test_Platform', permanent=False)
        self.assertEqual(self.pdb.get('4753', None), 'Test_Platform')
        self.assertEqual(self.pdb.remove('4753', permanent=False), 'Test_Platform')
        self.assertEqual(self.pdb.get('4753', None), None)

    def test_bogus_add(self):
        """Test that add requires properly formatted platform ids
        """
        self.assertEqual(self.pdb.get('NOTVALID', None), None)
        with self.assertRaises(ValueError):
            self.pdb.add('NOTVALID', 'Test_Platform', permanent=False)

    def test_bogus_remove(self):
        """Test that removing a not present platform does nothing
        """
        self.assertEqual(self.pdb.get('NOTVALID', None), None)
        self.assertEqual(self.pdb.remove('NOTVALID', permanent=False), None)

    def test_simplify_verbose_data(self):
        """Test that fetching a verbose entry without verbose data correctly
        returns just the 'platform_name'
        """
        platform_data = {
            'platform_name': 'VALID',
            'other_data': 'data'
        }
        self.pdb.add('1337', platform_data, permanent=False)
        self.assertEqual(self.pdb.get('1337', verbose_data=True), platform_data)
        self.assertEqual(self.pdb.get('1337'), platform_data['platform_name'])

class OverriddenPlatformDatabaseTests(unittest.TestCase):
    """ Test that for one database overriding another
    """

    def setUp(self):
        self.temp_dir = tempfile.mkdtemp()
        self.base_db_path = os.path.join(self.temp_dir, 'base')
        self.base_db = open(self.base_db_path, 'w+b')
        self.base_db.write(json.dumps(dict([('0123', 'Base_Platform')])).
                           encode('utf-8'))
        self.base_db.seek(0)
        self.overriding_db_path = os.path.join(self.temp_dir, 'overriding')
        self.overriding_db = open(self.overriding_db_path, 'w+b')
        self.overriding_db.write(b'{}')
        self.overriding_db.seek(0)
        self.pdb = PlatformDatabase([self.overriding_db_path, self.base_db_path],
                                    primary_database=self.overriding_db_path)
        self.base_db.seek(0)
        self.overriding_db.seek(0)

    def tearDown(self):
        self.base_db.close()
        self.overriding_db.close()

    def assertBaseUnchanged(self):
        """Assert that the base database has not changed
        """
        self.base_db.seek(0)
        self.assertEqual(self.base_db.read(),
                         json.dumps(dict([('0123', 'Base_Platform')]))
                         .encode('utf-8'))

    def assertOverrideUnchanged(self):
        """Assert that the override database has not changed
        """
        self.overriding_db.seek(0)
        self.assertEqual(self.overriding_db.read(), b'{}')

    def test_basline(self):
        """Sanity check that the base database does what we expect
        """
        self.assertEqual(list(self.pdb.items()), [('0123', 'Base_Platform')])
        self.assertEqual(list(self.pdb.all_ids()), ['0123'])

    def test_add_non_override(self):
        """Check that adding keys goes to the Override database
        """
        self.pdb.add('1234', 'Another_Platform')
        self.assertEqual(list(self.pdb.items()), [('1234', 'Another_Platform'), ('0123', 'Base_Platform')])
        self.assertEqual(set(self.pdb.all_ids()), set(['0123', '1234']))
        self.assertBaseUnchanged()

    def test_load_override(self):
        """Check that adding a platform goes to the Override database and
        you can no longer query for the base database definition and
        that the override database was not written to disk
        """
        self.overriding_db.write(json.dumps(dict([('0123', 'Overriding_Platform')])).
                                 encode('utf-8'))
        self.overriding_db.seek(0)
        self.pdb = PlatformDatabase([self.overriding_db_path, self.base_db_path],
                                    primary_database=self.overriding_db_path)
        self.assertIn(('0123', 'Overriding_Platform'), list(self.pdb.items()))
        self.assertEqual(set(self.pdb.all_ids()), set(['0123']))
        self.assertEqual(self.pdb.get('0123'), 'Overriding_Platform')
        self.assertBaseUnchanged()

    def test_add_override_permanent(self):
        """Check that adding a platform goes to the Override database and
        you can no longer query for the base database definition and
        that the override database was written to disk
        """
        self.pdb.add('0123', 'Overriding_Platform', permanent=True)
        self.assertIn(('0123', 'Overriding_Platform'), list(self.pdb.items()))
        self.assertEqual(set(self.pdb.all_ids()), set(['0123']))
        self.assertEqual(self.pdb.get('0123'), 'Overriding_Platform')
        self.overriding_db.seek(0)
        self.assertEqual(self.overriding_db.read(),
                         json.dumps(dict([('daplink', dict([('0123', 'Overriding_Platform')]))]))
                         .encode('utf-8'))
        self.assertBaseUnchanged()

    def test_remove_override(self):
        """Check that removing a platform from the Override database allows you to query
        the original base database definition and that
        that the override database was not written to disk
        """
        self.pdb.add('0123', 'Overriding_Platform')
        self.assertIn(('0123', 'Overriding_Platform'), list(self.pdb.items()))
        self.assertEqual(set(self.pdb.all_ids()), set(['0123']))
        self.assertEqual(self.pdb.get('0123'), 'Overriding_Platform')
        self.assertEqual(self.pdb.remove('0123'), 'Overriding_Platform')
        self.assertEqual(self.pdb.get('0123'), 'Base_Platform')
        self.assertOverrideUnchanged()
        self.assertBaseUnchanged()

    def test_remove_from_base(self):
        """Check that removing a platform from the base database no longer allows you to query
        the original base database definition and that that the base database
        was not written to disk
        """
        self.assertEqual(self.pdb.remove('0123'), 'Base_Platform')
        self.assertEqual(self.pdb.get('0123'), None)
        self.assertOverrideUnchanged()
        self.assertBaseUnchanged()

    def test_remove_from_base_permanent(self):
        """Check that removing a platform from the base database no longer allows you to query
        the original base database definition and that that the base database
        was not modified on disk
        """
        self.assertEqual(self.pdb.remove('0123', permanent=True), 'Base_Platform')
        self.assertEqual(self.pdb.get('0123'), None)
        self.assertBaseUnchanged()

class InternalLockingChecks(unittest.TestCase):

    def setUp(self):
        self.mocked_lock = patch('mbed_os_tools.detect.platform_database.InterProcessLock', spec=True).start()
        self.acquire = self.mocked_lock.return_value.acquire
        self.release = self.mocked_lock.return_value.release

        self.base_db_path = os.path.join(tempfile.mkdtemp(), 'base')
        self.base_db = open(self.base_db_path, 'w+b')
        self.base_db.write(b'{}')
        self.base_db.seek(0)
        self.pdb = PlatformDatabase([self.base_db_path])
        self.addCleanup(patch.stopall)

    def tearDown(self):
        self.base_db.close()

    def test_no_update(self):
        """Test that no locks are used when no modifications are specified
        """
        self.pdb.add('7155', 'Junk')
        self.acquire.assert_not_called()
        self.release.assert_not_called()

    def test_update(self):
        """Test that locks are used when modifications are specified
        """
        self.pdb.add('7155', 'Junk', permanent=True)
        assert self.acquire.called, 'Lock acquire should have been called'
        assert self.release.called

    def test_update_fail_acquire(self):
        """Test that the backing file is not updated when lock acquisition fails
        """
        self.acquire.return_value = False
        self.pdb.add('7155', 'Junk', permanent=True)
        assert self.acquire.called, 'Lock acquire should have been called'
        self.base_db.seek(0)
        self.assertEqual(self.base_db.read(), b'{}')

    def test_update_ambiguous(self):
        """Test that the backing file is not updated when lock acquisition fails
        """
        self.pdb._prim_db = None
        self.pdb.add('7155', 'Junk', permanent=True)
        self.acquire.assert_not_called()
        self.release.assert_not_called()
        self.assertEqual(self.base_db.read(), b'{}')
