import os, json, stat, sys, shutil, errno, subprocess, logging
from os.path import dirname, abspath, basename, join
import argparse

ROOT = abspath(join(dirname(__file__), "../.."))
RTOS_UPDATE_BRANCH = "rtos_update"

def del_file(name):
    result = []
    search_path = [join(ROOT, 'rtos'), join(ROOT, 'cmsis')]
    for path in search_path:
        for root, dirs, files in os.walk(path):
            if name in files:
                result.append(os.path.join(root, name))
    for file in result:
        print os.path.relpath(file, ROOT)
        os.remove(file)

def rmtree(top):
    for root, dirs, files in os.walk(top, topdown=False):
        for name in files:
            filename = os.path.join(root, name)
            os.chmod(filename, stat.S_IWUSR)
            os.remove(filename)
        for name in dirs:
            os.rmdir(os.path.join(root, name))
    os.rmdir(top)

def copy_file(file, path):
    try:
        shutil.copy(file, path)
    except IOError as e:
        if e.errno != errno.ENOENT:
            raise
        ## Create directories
        os.makedirs(os.path.dirname(path))
        shutil.copy(file, path)
    print os.path.relpath(path, ROOT)

def copy_folder(folder, path):
    files = os.listdir(folder)
    for file in files:
        abs_src_file = os.path.join(folder, file)
        if os.path.isfile(abs_src_file):
            abs_dst_file = os.path.join(path, file)
            copy_file(abs_src_file, abs_dst_file)

def run_cmd(command, exit_on_failure=False):
    """ Passes a command to the system and returns a True/False result once the
        command has been executed, indicating success/failure. Commands are passed
        as a list of tokens.
        E.g. The command 'git remote -v' would be passed in as ['git', 'remote', '-v']
    """
    return_code = subprocess.call(command, shell=True)

    if return_code:
        print("The command %s failed with return code: %s"
                        %(' '.join(command), return_code))
        if exit_on_failure:
            sys.exit(1)

    return return_code

def remove_repo(folder):
    os.chdir(abspath(dirname(__file__)))
    if os.path.exists(folder):
        rmtree(folder)

def get_repo(repo, branch, folder):
    """ Get the Repository files from git, at depth level 1
        repo - Git repository link
        branch - repository branch
        folder - folder at which repo will be cloned
    """
    remove_repo(folder)
    clone_cmd = ['git', 'clone', repo, "-b", branch, "--depth", '1', folder]
    run_cmd(clone_cmd, exit_on_failure=True)

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('-l', '--log-level',
                        help="Level for providing logging output",
                        default='INFO')
    parser.add_argument('-r',   '--repo-path',
                        help="Git Repository to be imported",
                        default=None)
    parser.add_argument('-c', '--config-file',
                        help="Configuration file",
                        default=None)
    args = parser.parse_args()

    default = getattr(logging, 'INFO')
    level = getattr(logging, args.log_level.upper(), default)

    # Set logging level
    logging.basicConfig(level=level)
    rel_log = logging.getLogger("Importer")

    if (args.repo_path is None) or (args.config_file is None) :
        rel_log.error("Repository path and config file required as input. Use \"--help\" for more info.")
        exit(1)

    repo = os.path.abspath(args.repo_path)
    if not os.path.exists(repo):
        rel_log.error("%s not found.", args.repo_path)
        exit(1)

    json_file = os.path.abspath(args.config_file)
    if not os.path.isfile(json_file):
        rel_log.error("%s not found." , args.config_file)
        exit(1)

    # Read configuration data
    with open(json_file, 'r') as config:
        json_data = json.load(config)
    data_files = json_data["files"]
    data_folders = json_data["folders"]

    ## Remove all files listed in .json from mbed-os repo to avoid duplications
    print "Cleaning up:"
    for file in data_files:
        cmsis_file =  file['cmsis_file']
        del_file(os.path.basename(cmsis_file))

    for folder in data_folders:
        files = os.listdir(repo_path)
        for file in files:
            del_file(os.path.basename(file))

    ## Copy all the CMSIS files listed in json file to mbed-os
    print "Files Copied:"
    for file in data_files:
        repo_file =  os.path.join(repo_path, file['cmsis_file'])
        mbed_path =  os.path.join(ROOT, file['mbed_file'])
        copy_file(repo_file, mbed_path)

    for folder in data_folders:
        repo_folder =  os.path.join(repo_path, folder['cmsis_folder'])
        mbed_path =  os.path.join(ROOT, folder['mbed_folder'])
        copy_folder(repo_folder, mbed_path)

    ## Create new branch with all changes
    create_branch = ['git', 'checkout', '-b', RTOS_UPDATE_BRANCH]
    run_cmd(create_branch, exit_on_failure=True)

    add_files = ['git', 'add', '-A']
    run_cmd(add_files, exit_on_failure=True)

    commit_branch = ['git', 'commit', '-m', "CMSIS/RTX: Update CMSIS/RTX"]
    run_cmd(commit_branch, exit_on_failure=True)

    ## Apply commits specific to mbed-os changes
    mbed_sha = json_data["Mbed_sha"]

    for sha in mbed_sha:
        cherry_pick_sha = ['git', 'cherry-pick', sha]
        run_cmd(cherry_pick_sha, exit_on_failure=True)
