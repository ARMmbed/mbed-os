"""
Copyright (c) 2018-2019 ARM Limited. All rights reserved.

SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations 
"""

from os.path import dirname, abspath, join, exists

from tools.utils import json_file_to_dict
from tools.targets import TARGET_MAP
from tools.config import Config

CONFIG_DIR = dirname(abspath(__file__))
CONFIG_MAP = json_file_to_dict(join(CONFIG_DIR, "config_paths.json"))
TARGET_CONFIGS = json_file_to_dict(join(CONFIG_DIR, "target_configs.json"))

def get_valid_configs(target_name):
    if target_name in TARGET_CONFIGS:
        target_config = TARGET_CONFIGS[target_name]
    elif (target_name in TARGET_MAP and 'EMAC' in TARGET_MAP[target_name].device_has):
        target_config = { "default_test_configuration": "ETHERNET", "test_configurations": ["ETHERNET"] }
    else:
        return {}

    config_dict = {}
    for attr in CONFIG_MAP:
        if attr in target_config['test_configurations']:
            config_dict[attr] = CONFIG_MAP[attr]
    return config_dict

def get_config_path(conf_name, target_name):
    configs = get_valid_configs(target_name)
    if configs and conf_name.upper() in configs:
        return join(CONFIG_DIR, configs[conf_name.upper()])
    else:
        return None

def get_default_config(source_dir, target_name):
    if target_name in TARGET_CONFIGS:
        config_name = TARGET_CONFIGS[target_name]['default_test_configuration']
        if config_name == "NONE":
            return None
        return join(CONFIG_DIR, CONFIG_MAP[config_name])
    elif Config.find_app_config(source_dir):
        return None
    elif (target_name in TARGET_MAP and 'EMAC' in TARGET_MAP[target_name].device_has):
        return join(CONFIG_DIR, CONFIG_MAP["ETHERNET"])
    else:
        return None
