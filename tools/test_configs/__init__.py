from os.path import dirname, abspath, join

from tools.utils import json_file_to_dict
from tools.targets import TARGET_MAP

CONFIG_DIR = dirname(abspath(__file__))
CONFIG_MAP = json_file_to_dict(join(CONFIG_DIR, "config_paths.json"))
TARGET_CONFIGS = json_file_to_dict(join(CONFIG_DIR, "target_configs.json"))

def get_valid_configs(target_name):
    if target_name in TARGET_CONFIGS:
        target_config = TARGET_CONFIGS[target_name]
    elif (target_name in TARGET_MAP and 'LWIP' in TARGET_MAP[target_name].features):
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

def get_default_config(target_name):
    if target_name in TARGET_CONFIGS:
        config_name = TARGET_CONFIGS[target_name]['default_test_configuration']
        if config_name == "NONE":
            return None
        return join(CONFIG_DIR, CONFIG_MAP[config_name])
    elif (target_name in TARGET_MAP and 'LWIP' in TARGET_MAP[target_name].features):
        return join(CONFIG_DIR, CONFIG_MAP["ETHERNET"])
    else:
        return None
