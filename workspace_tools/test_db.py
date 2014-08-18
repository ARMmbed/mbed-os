"""
mbed SDK
Copyright (c) 2011-2014 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: Przemyslaw Wirkus <Przemyslaw.wirkus@arm.com>
"""

import re


class BaseDBAccess():
    """ Class used to connect with test database and store test results
    """
    def __init__(self):
        self.db_object = None
        self.db_type = None
        # Test Suite DB scheme (table names)
        self.TABLE_BUILD_ID = 'mtest_build_id'
        self.TABLE_BUILD_ID_STATUS = 'mtest_build_id_status'
        self.TABLE_TARGET = 'mtest_target'
        self.TABLE_TEST_ENTRY = 'mtest_test_entry'
        self.TABLE_TEST_ID = 'mtest_test_id'
        self.TABLE_TEST_RESULT = 'mtest_test_result'
        self.TABLE_TEST_TYPE = 'mtest_test_type'
        self.TABLE_TOOLCHAIN = 'mtest_toolchain'
        # Build ID status PKs
        self.BUILD_ID_STATUS_STARTED = 1 # Started
        self.BUILD_ID_STATUS_IN_PROGRESS = 2 # In Progress
        self.BUILD_ID_STATUS_COMPLETED = 3 #Completed
        self.BUILD_ID_STATUS_FAILED = 4 # Failed

    def get_hostname(self):
        """ Useful when creating build_id in database
            Function returns (hostname, uname) which can be used as (build_id_name, build_id_desc)
        """
        # Get hostname from socket
        import socket
        hostname = socket.gethostbyaddr(socket.gethostname())[0]
        # Get uname from platform resources
        import platform
        uname = json.dumps(platform.uname())
        return (hostname, uname)

    def get_db_type(self):
        """ Returns database type. E.g. 'mysql', 'sqlLite' etc.
        """
        return self.db_type

    def detect_database(self, verbose=False):
        """ detect database and return VERION data structure or string (verbose=True)
        """
        return None

    def parse_db_connection_string(self, str):
        """ Parsing SQL DB connection string. String should contain:
            - DB Name, user name, password, URL (DB host), name
            Function should return tuple with parsed (db_type, username, password, host, db_name) or None if error
            E.g. connection string: 'mysql://username:password@127.0.0.1/db_name'
        """
        PATTERN = '^([\w]+)://([\w]+):([\w]*)@(.*)/([\w]+)'
        result = re.match(PATTERN, str)
        if result is not None:
            result = result.groups()    # Tuple (db_name, host, user, passwd, db)
        return result # (db_type, username, password, host, db_name)

    def is_connected(self):
        """ Returns True if we are connected to database
        """
        pass

    def connect(self, host, user, passwd, db):
        """ Connects to DB and returns DB object
        """
        pass

    def disconnect(self):
        """ Close DB connection
        """
        pass

    def escape_string(self, str):
        """ Escapes string so it can be put in SQL query between quotes
        """
        pass

    def select_all(self, query):
        """ Execute SELECT query and get all results
        """
        pass

    def insert(self, query, commit=True):
        """ Execute INSERT query, define if you want to commit
        """
        pass

    def get_next_build_id(self, name, desc=''):
        """ Insert new build_id (DB unique build like ID number to send all test results)
        """
        pass

    def get_table_entry_pk(self, table, column, value, update_db=True):
        """ Checks for entries in tables with two columns (<TABLE_NAME>_pk, <column>)
            If update_db is True updates table entry if value in specified column doesn't exist
        """
        pass

    def update_table_entry(self, table, column, value):
        """ Updates table entry if value in specified column doesn't exist
            Locks table to perform atomic read + update
        """
        pass

    def insert_test_entry(self, next_build_id, target, toolchain, test_type, test_id, test_result, test_time, test_timeout, test_loop, test_extra=''):
        """ Inserts test result entry to database. All checks regarding existing
            toolchain names in DB are performed.
            If some data is missing DB will be updated
        """
        pass
