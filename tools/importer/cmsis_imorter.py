import os, json, stat, sys, shutil, errno, subprocess
from os.path import dirname, abspath, basename, join

ROOT = abspath(join(dirname(__file__), "../.."))
CMSIS_REPO = "CMSIS_Repo"
CMSIS_PATH = abspath(join(dirname(__file__), CMSIS_REPO))
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
    
    # Read configuration data
    with open(os.path.join(os.path.dirname(__file__), "cmsis_importer.json"), 'r') as config:        
        json_data = json.load(config)
    config = json_data["config"]
    cmsis_repo = config['cmsis_repo']
    cmsis_branch = config['cmsis_branch']    
    data_files = json_data["files"]
    data_folders = json_data["folders"]
    
    print "Fetching git repo"
    get_repo(cmsis_repo, cmsis_branch, CMSIS_REPO)
    
    ## Remove all files listed in .json from mbed-os repo to avoid duplications
    print "Cleaning up:"
    for file in data_files:
        cmsis_file =  file['cmsis_file']
        del_file(os.path.basename(cmsis_file))
    
    for folder in data_folders:
        cmsis_folder =  os.path.join(CMSIS_PATH, folder['cmsis_folder'])
        files = os.listdir(cmsis_folder)        
        for file in files:            
            del_file(os.path.basename(file))

    ## Copy all the CMSIS files listed in json file to mbed-os    
    print "Files Copied:"    
    for file in data_files:        
        cmsis_file =  os.path.join(CMSIS_PATH, file['cmsis_file'])
        mbed_path =  os.path.join(ROOT, file['mbed_file'])
        copy_file(cmsis_file, mbed_path)

    for folder in data_folders:
        cmsis_folder =  os.path.join(CMSIS_PATH, folder['cmsis_folder'])
        mbed_path =  os.path.join(ROOT, folder['mbed_folder'])        
        copy_folder(cmsis_folder, mbed_path)

    #Remove CMSIS Repo
    remove_repo(CMSIS_REPO)

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
        

    
    

    

