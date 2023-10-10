# Copyright (c) 2018, Arm Limited and affiliates.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import json
import uuid
import lockfile
from os.path import expanduser

HOME_DIR = expanduser("~")
GREENTEA_HOME_DIR = ".mbed-greentea"
GREENTEA_GLOBAL_LOCK = "glock.lock"
GREENTEA_KETTLE = "kettle.json" # active Greentea instances
GREENTEA_KETTLE_PATH = os.path.join(HOME_DIR, GREENTEA_HOME_DIR, GREENTEA_KETTLE)


def greentea_home_dir_init():
    """ Initialize data in home directory for locking features
    """
    if not os.path.isdir(os.path.join(HOME_DIR, GREENTEA_HOME_DIR)):
        os.mkdir(os.path.join(HOME_DIR, GREENTEA_HOME_DIR))

def greentea_get_app_sem():
    """ Obtain locking mechanism info
    """
    greentea_home_dir_init()
    gt_instance_uuid = str(uuid.uuid4())   # String version
    gt_file_sem_name = os.path.join(HOME_DIR, GREENTEA_HOME_DIR, gt_instance_uuid)
    gt_file_sem = lockfile.LockFile(gt_file_sem_name)
    return gt_file_sem, gt_file_sem_name, gt_instance_uuid

def greentea_get_target_lock(target_id):
    greentea_home_dir_init()
    file_path = os.path.join(HOME_DIR, GREENTEA_HOME_DIR, target_id)
    lock = lockfile.LockFile(file_path)
    return lock

def greentea_get_global_lock():
    greentea_home_dir_init()
    file_path = os.path.join(HOME_DIR, GREENTEA_HOME_DIR, GREENTEA_GLOBAL_LOCK)
    lock = lockfile.LockFile(file_path)
    return lock

def greentea_update_kettle(greentea_uuid):
    from time import gmtime, strftime

    with greentea_get_global_lock():
        current_brew = get_json_data_from_file(GREENTEA_KETTLE_PATH)
        if not current_brew:
            current_brew = {}
        current_brew[greentea_uuid] = {
            "start_time" : strftime("%Y-%m-%d %H:%M:%S", gmtime()),
            "cwd" : os.getcwd(),
            "locks" : []
        }
        with open(GREENTEA_KETTLE_PATH, 'w') as kettle_file:
            json.dump(current_brew, kettle_file, indent=4)

def greentea_clean_kettle(greentea_uuid):
    """ Clean info in local file system config file
    """
    with greentea_get_global_lock():
        current_brew = get_json_data_from_file(GREENTEA_KETTLE_PATH)
        if not current_brew:
            current_brew = {}
        current_brew.pop(greentea_uuid, None)
        with open(GREENTEA_KETTLE_PATH, 'w') as kettle_file:
            json.dump(current_brew, kettle_file, indent=4)

def greentea_acquire_target_id(target_id, gt_instance_uuid):
    """ Acquire lock on target_id for given greentea UUID
    """
    with greentea_get_global_lock():
        current_brew = get_json_data_from_file(GREENTEA_KETTLE_PATH)
        if current_brew:
            current_brew[gt_instance_uuid]['locks'].append(target_id)
            with open(GREENTEA_KETTLE_PATH, 'w') as kettle_file:
                json.dump(current_brew, kettle_file, indent=4)

def greentea_acquire_target_id_from_list(possible_target_ids, gt_instance_uuid):
    """ Acquire lock on target_id from list of possible target_ids for given greentea UUID
    """
    target_id = None
    already_locked_target_ids = []
    with greentea_get_global_lock():
        current_brew = get_json_data_from_file(GREENTEA_KETTLE_PATH)
        # Get all already locked target_id
        for cb in current_brew:
            locks_list = current_brew[cb]['locks']
            already_locked_target_ids.extend(locks_list)

        # Remove from possible_target_ids elements from already_locked_target_ids
        available_target_ids = [item for item in possible_target_ids if item not in already_locked_target_ids]

        if available_target_ids:
            target_id = available_target_ids[0]
            current_brew[gt_instance_uuid]['locks'].append(target_id)
            with open(GREENTEA_KETTLE_PATH, 'w') as kettle_file:
                json.dump(current_brew, kettle_file, indent=4)
    return target_id

def greentea_release_target_id(target_id, gt_instance_uuid):
    """ Release target_id for given greentea UUID
    """
    with greentea_get_global_lock():
        current_brew = get_json_data_from_file(GREENTEA_KETTLE_PATH)
        if current_brew:
            current_brew[gt_instance_uuid]['locks'].remove(target_id)
            with open(GREENTEA_KETTLE_PATH, 'w') as kettle_file:
                json.dump(current_brew, kettle_file, indent=4)

def get_json_data_from_file(json_spec_filename):
    """ Loads from file JSON formatted string to data structure
    """
    result = None
    try:
        with open(json_spec_filename, 'r') as data_file:
            try:
                result = json.load(data_file)
            except ValueError:
                result = None
    except IOError:
        result = None
    return result

def greentea_kettle_info():
    """ generates human friendly info about current kettle state

    @details
    {
        "475a46d0-41fe-41dc-b5e6-5197a2fcbb28": {
            "locks": [],
            "start_time": "2015-10-23 09:29:54",
            "cwd": "c:\\Work\\mbed-drivers"
        }
    }
    """
    from prettytable import PrettyTable
    with greentea_get_global_lock():
        current_brew = get_json_data_from_file(GREENTEA_KETTLE_PATH)
        cols = ['greentea_uuid', 'start_time', 'cwd', 'locks']
        pt = PrettyTable(cols)

        for col in cols:
            pt.align[col] = "l"
        pt.padding_width = 1 # One space between column edges and contents (default)

        row = []
        for greentea_uuid in current_brew:
            kettle = current_brew[greentea_uuid]
            row.append(greentea_uuid)
            row.append(kettle['start_time'])
            row.append(kettle['cwd'])
            row.append('\n'.join(kettle['locks']))
            pt.add_row(row)
            row = []
    return pt.get_string()
