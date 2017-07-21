from os.path import dirname, abspath, join

from tools.utils import json_file_to_dict
from tools.targets import TARGET_MAP

class TestConfig:
    CONFIG_DIR = dirname(abspath(__file__))
    CONFIG_MAP = json_file_to_dict(join(CONFIG_DIR, "config_paths.json"))

    @classmethod
    def get_valid_configs(cls, target_name):
        target = TARGET_MAP[target_name]
        config_dict = {}
        for attr in cls.CONFIG_MAP:
            if attr in target.device_has:
                config_dict[attr] = cls.CONFIG_MAP[attr]
        return config_dict

    @classmethod
    def get_config_path(cls, conf_name, target_name):
        configs = cls.get_valid_configs(target_name)
        if configs and conf_name.upper() in configs:
            return join(cls.CONFIG_DIR, configs[conf_name.upper()])
        else:
            return None
