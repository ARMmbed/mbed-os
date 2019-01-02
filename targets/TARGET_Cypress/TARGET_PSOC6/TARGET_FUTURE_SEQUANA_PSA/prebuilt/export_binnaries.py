import fnmatch
import shutil
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.realpath(os.path.join(SCRIPT_DIR, '..', '..', '..', '..', '..'))
DELIVERY_DIR = os.path.join(ROOT_DIR, 'DELIVERY', 'TARGET_FUTURE_SEQUANA_PSA')

for f in fnmatch.filter(os.listdir(DELIVERY_DIR), '*.hex'):
    test_suite_name = os.path.splitext(f)[0]
    test_directory = os.path.join(ROOT_DIR, 'TESTS', 'psa', test_suite_name)

    if os.path.exists(test_directory):
        target_dir = os.path.join(test_directory, 'TARGET_FUTURE_SEQUANA_PSA', f)
    else:
        target_dir = os.path.join(SCRIPT_DIR, f)

    shutil.copyfile(os.path.join(DELIVERY_DIR, f), target_dir)



