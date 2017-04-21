# Script to check a new mbed 2 release by compiling a set of specified test apps 
# for all currently supported platforms. Each test app must include an mbed library.
# This can either be the pre-compiled version 'mbed' or the source version 'mbed-dev'.
#
# Setup:
# 1. Set up your global .hgrc file
# 
#    If you don't already have a .hgrc file in your $HOME directory, create one there.
#    Then add the following section:
# 
#    [auth]
#    x.prefix = *
#    x.username = <put your mbed org username here>
#    x.password = <put your mbed org password here>
#
#    This has 2 purposes, the first means you don't get prompted for your password 
#    whenever you run hg commands on the commandline. The second is that this script
#    reads these details in order to fully automate the Mercurial commands.
# 
# Edit "check_release.json". This has the following structure:
#{
#  "config" : {
#    "mbed_repo_path" : "C:/Users/annbri01/Work/Mercurial"
#  },
#  "test_list" : [
#    {
#        "name" : "test_compile_mbed_lib",
#        "lib" : "mbed"
#    },
#    {
#        "name" : "test_compile_mbed_dev",
#        "lib" : "mbed-dev"
#    }
#  ],
#  "target_list" : []
#}
#
# The mbed_repo_path field should be changed to point to where your local
# working directory is for Mercurial repositories.
# For each test app you wish to run, add an entry to the test list. The example
# above has 2 test apps  
#     "test_compile_mbed_lib" and "test_compile_mbed_dev"
# The lib field in each says which type of mbed 2 library the app contains.
# These test apps MUST be available as repos in the user's online Mercurial area.
# The target_list allows the user to override the set of targets/platforms used 
# for the compilation.
# E.g to just compile for 2 targets, K64F and K22F :
# "target_list" : ["K64F", "K22F"]
#
# Run the script from the mbed-os directory as follows:
# > python tools/check_release.py 
#
# It will look for local clones of the test app repos. If they don't exist
# it will clone them. It will then read the latest versions of mbed and mbed-dev
# (an assumption is made that both of these are already cloned in your Mercurial area).
# The lib files within the test apps are then updated to the corresponding version in 
# the associated lib itself. The test apps are then committed and pushed back to the users
# fork.
# The test apps will then be compiled for all supported targets and a % result output at 
# the end.
#
# Uses the online compiler API at https://mbed.org/handbook/Compile-API
# Based on the example from https://mbed.org/teams/mbed/code/mbed-API-helper/
 
 
import os, getpass, sys, json, time, requests, logging
from os.path import dirname, abspath, basename, join
import argparse
import subprocess
import re
import hglib
import argparse

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from tools.build_api import get_mbed_official_release

OFFICIAL_MBED_LIBRARY_BUILD = get_mbed_official_release('2')

def get_compilation_failure(messages):
    """ Reads the json formatted 'messages' and checks for compilation errors.
        If there is a genuine compilation error then there should be a new 
        message containing a severity field = Error and an accompanying message 
        with the compile error text. Any other combination is considered an 
        internal compile engine failure 
    Args:
    messages - json formatted text returned by the online compiler API.
    
    Returns:
    Either "Error" or "Internal" to indicate an actual compilation error or an 
    internal IDE API fault.

    """
    for m in messages:
        # Get message text if it exists
        try:
            message = m['message']
            message = message + "\n"
        except KeyError:
            # Skip this message as it has no 'message' field
            continue
                 
        # Get type of message text
        try:
            msg_type = m['type']
        except KeyError:
            # Skip this message as it has no 'type' field
            continue
        
        if msg_type == 'error' or msg_type == 'tool_error':
            rel_log.error(message)
            return "Error"
        else: 
            rel_log.debug(message)

    return "Internal"
                 
def invoke_api(payload, url, auth, polls, begin="start/"):
    """ Sends an API command request to the online IDE. Waits for a task completed 
        response before returning the results.

    Args:
    payload - Configuration parameters to be passed to the API
    url - THe URL for the online compiler API
    auth - Tuple containing authentication credentials
    polls - Number of times to poll for results
    begin - Default value = "start/", start command to be appended to URL
    
    Returns:
    result - True/False indicating the success/failure of the compilation
    fail_type - the failure text if the compilation failed, else None
    """

    # send task to api
    rel_log.debug(url + begin + "| data: " + str(payload))
    r = requests.post(url + begin, data=payload, auth=auth)
    rel_log.debug(r.request.body)
    
    if r.status_code != 200:
        rel_log.error("HTTP code %d reported.", r.status_code)
        return False, "Internal"

    response = r.json()
    rel_log.debug(response)
    uuid = response['result']['data']['task_id']
    rel_log.debug("Task accepted and given ID: %s", uuid)
    result = False
    fail_type = None
    
    # It currently seems to take the onlide IDE API ~30s to process the compile
    # request and provide a response. Set the poll time to half that in case it 
    # does manage to compile quicker.
    poll_delay = 15
    rel_log.debug("Running with a poll for response delay of: %ss", poll_delay)

    # poll for output
    for check in range(polls):
        time.sleep(poll_delay)
        
        try:
            r = requests.get(url + "output/%s" % uuid, auth=auth)
            
        except ConnectionError:
            return "Internal"

        response = r.json()

        data = response['result']['data']
        if data['task_complete']:
            # Task completed. Now determine the result. Should be one of :
            # 1) Successful compilation
            # 2) Failed compilation with an error message
            # 3) Internal failure of the online compiler            
            result = bool(data['compilation_success'])
            if result:
                rel_log.info("COMPILATION SUCCESSFUL\n")
            else:
                # Did this fail due to a genuine compilation error or a failue of 
                # the api itself ?
                rel_log.info("COMPILATION FAILURE\n")
                fail_type = get_compilation_failure(data['new_messages'])
            break
    else:
        rel_log.info("COMPILATION FAILURE\n")
        
    if not result and fail_type == None:
        fail_type = "Internal"
        
    return result, fail_type


def build_repo(target, program, user, pw, polls=25, 
               url="https://developer.mbed.org/api/v2/tasks/compiler/"):
    """ Wrapper for sending an API command request to the online IDE. Sends a 
        build request.

    Args:
    target - Target to be built
    program - Test program to build
    user - mbed username
    pw - mbed password
    polls - Number of times to poll for results
    url - THe URL for the online compiler API
    
    Returns:
    result - True/False indicating the success/failure of the compilation
    fail_type - the failure text if the compilation failed, else None
    """
    payload = {'clean':True, 'target':target, 'program':program}
    auth = (user, pw)
    return invoke_api(payload, url, auth, polls)

def run_cmd(command, exit_on_failure=False):
    """ Passes a command to the system and returns a True/False result once the 
        command has been executed, indicating success/failure. Commands are passed
        as a list of tokens. 
        E.g. The command 'git remote -v' would be passed in as ['git', 'remote', '-v']

    Args:
    command - system command as a list of tokens
    exit_on_failure - If True exit the program on failure (default = False)
    
    Returns:
    result - True/False indicating the success/failure of the command
    """
    rel_log.debug('[Exec] %s', ' '.join(command))
    return_code = subprocess.call(command, shell=True)
    
    if return_code:
        rel_log.warning("The command '%s' failed with return code: %s",  
                        (' '.join(command), return_code))
        if exit_on_failure:
            sys.exit(1)
    
    return return_code

def run_cmd_with_output(command, exit_on_failure=False):
    """ Passes a command to the system and returns a True/False result once the 
        command has been executed, indicating success/failure. If the command was 
        successful then the output from the command is returned to the caller.
        Commands are passed as a list of tokens. 
        E.g. The command 'git remote -v' would be passed in as ['git', 'remote', '-v']

    Args:
    command - system command as a list of tokens
    exit_on_failure - If True exit the program on failure (default = False)
    
    Returns:
    result - True/False indicating the success/failure of the command
    output - The output of the command if it was successful, else empty string
    """
    rel_log.debug('[Exec] %s', ' '.join(command))
    returncode = 0
    output = ""
    try:
        output = subprocess.check_output(command, shell=True)
    except subprocess.CalledProcessError as e:
        rel_log.warning("The command '%s' failed with return code: %s", 
                        (' '.join(command), e.returncode))
        returncode = e.returncode
        if exit_on_failure:
            sys.exit(1)
    return returncode, output

def upgrade_test_repo(test, user, library, ref, repo_path):
    """ Upgrades a local version of a test repo to the latest version of its 
        embedded library.
        If the test repo is not present in the user area specified in the json 
        config file, then it will first be cloned. 
    Args:
    test - Mercurial test repo name
    user - Mercurial user name
    library - library name
    ref - SHA corresponding to the latest version of the library
    repo_path - path to the user's repo area
    
    Returns:
    updated - True if library was updated, False otherwise
    """
    rel_log.info("Updating test repo: '%s' to SHA: %s", test, ref)
    cwd = os.getcwd()

    repo = "https://" + user + '@developer.mbed.org/users/' + user + '/code/' + test 

    # Clone the repo if it doesn't already exist
    path = abspath(repo_path + '/' + test)
    if not os.path.exists(path):
        rel_log.info("Test repo doesn't exist, cloning...")
        os.chdir(abspath(repo_path))        
        clone_cmd = ['hg', 'clone', repo]
        run_cmd(clone_cmd, exit_on_failure=True)
    
    os.chdir(path)

    client = hglib.open(path)        

    lib_file = library + '.lib'    
    if os.path.isfile(lib_file):
        # Rename command will fail on some OS's if the target file already exist,
        # so ensure if it does, it is deleted first.
        bak_file = library + '_bak' 
        if os.path.isfile(bak_file):
            os.remove(bak_file)
        
        os.rename(lib_file, bak_file)
    else:
        rel_log.error("Failure to backup lib file prior to updating.")
        return False
    
    # mbed 2 style lib file contains one line with the following format
    # e.g. https://developer.mbed.org/users/<user>/code/mbed-dev/#156823d33999
    exp = 'https://developer.mbed.org/users/' + user + '/code/' + library + '/#[A-Za-z0-9]+'
    lib_re = re.compile(exp)
    updated = False

    # Scan through mbed-os.lib line by line, looking for lib version and update 
    # it if found
    with open(bak_file, 'r') as ip, open(lib_file, 'w') as op:
        for line in ip:

            opline = line
            
            regexp = lib_re.match(line)
            if regexp:
                opline = 'https://developer.mbed.org/users/' + user + '/code/' + library + '/#' + ref
                updated = True
    
            op.write(opline)

    if updated:

        # Setup the default commit message
        commit_message = '"Updating ' + library + ' to ' + ref + '"' 
 
        # Setup and run the commit command. Need to use the rawcommand in the hglib
        # for this in order to pass the string value to the -m option. run_cmd using 
        # subprocess does not like this syntax.
        try:
            client.rawcommand(['commit','-m '+commit_message, lib_file])

            cmd = ['hg', 'push', '-f', repo]
            run_cmd(cmd, exit_on_failure=True)
            
        except:
            rel_log.info("Lib file already up to date and thus nothing to commit")
                            
    os.chdir(cwd)
    return updated

def get_sha(repo_path, library):
    """ Gets the latest SHA for the library specified. The library is assumed to be
        located at the repo_path. If a SHA cannot be obtained this script will exit.

    Args:
    library - library name
    repo_path - path to the user's repo area
    
    Returns:
    sha - last commit SHA
    """
    cwd = os.getcwd()
    sha = None
    os.chdir(abspath(repo_path + '/' + library))
    
    cmd = ['hg', 'log', '-l', '1']
    ret, output = run_cmd_with_output(cmd, exit_on_failure=True)
    
    # Output should contain a 4 line string of the form:
    # changeset:   135:176b8275d35d
    # tag:         tip
    # user:        <>
    # date:        Thu Feb 02 16:02:30 2017 +0000
    # summary:     Release 135 of the mbed library
    # All we want is the changeset string after version number
    
    lines = output.split('\n')
    fields = lines[0].split(':')
    sha = fields[2]
    
    os.chdir(cwd)
    return sha

def get_latest_library_versions(repo_path):
    """ Returns the latest library versions (SHAs) for 'mbed' and 'mbed-dev'. 
    If the SHAs cannot be obtained this script will exit.

    Args:
    repo_path - path to the user's repo area
    
    Returns:
    mbed - last commit SHA for mbed library
    mbed_dev - last commit SHA for mbed_dev library
    
    """

    mbed = get_sha(repo_path, 'mbed')
    mbed_dev = get_sha(repo_path, 'mbed-dev')

    return mbed, mbed_dev

def log_results(lst, title):
    if len(lst) == 0:
        rel_log.info("%s - None", title)
    else:        
        for entry in lst:
            rel_log.info("%s - Test: %s, Target: %s", title, entry[0], entry[1])                


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('-l', '--log-level', 
                        help="Level for providing logging output", 
                        default='INFO')
    args = parser.parse_args()

    default = getattr(logging, 'INFO')
    level = getattr(logging, args.log_level.upper(), default)
        
    # Set logging level
    logging.basicConfig(level=level)
    rel_log = logging.getLogger("check-release")
    
    # Read configuration data
    with open(os.path.join(os.path.dirname(__file__), "check_release.json")) as config:
        json_data = json.load(config)
     
    supported_targets = []

    if len(json_data["target_list"]) > 0:
        # Compile user supplied subset of targets
        supported_targets = json_data["target_list"]
    else:        
        # Get a list of the officially supported mbed-os 2 targets
        for tgt in OFFICIAL_MBED_LIBRARY_BUILD:
            supported_targets.append(tgt[0])

    ignore_list = []

    if len(json_data["ignore_list"]) > 0:
        # List of tuples of (test, target) to be ignored in this test
        ignore_list = json_data["ignore_list"]

    config = json_data["config"]
    test_list = json_data["test_list"]
    repo_path = config["mbed_repo_path"]
    tests = []

    # get username
    cmd = ['hg', 'config', 'auth.x.username']
    ret, output = run_cmd_with_output(cmd, exit_on_failure=True)
    output = output.split('\n')
    user = output[0]
    
    # get password
    cmd = ['hg', 'config', 'auth.x.password']
    ret, output = run_cmd_with_output(cmd, exit_on_failure=True)
    output = output.split('\n')
    password = output[0]
    
    mbed, mbed_dev = get_latest_library_versions(repo_path)

    if not mbed or not mbed_dev:
        rel_log.error("Could not obtain latest versions of library files!!")
        exit(1)
        
    rel_log.info("Latest mbed lib version = %s", mbed)
    rel_log.info("Latest mbed-dev lib version = %s", mbed_dev)    
  
    # First update test repos to latest versions of their embedded libraries
    for test in test_list:
        tests.append(test['name'])
        upgrade_test_repo(test['name'], user, test['lib'], 
                          mbed if test['lib'] == "mbed" else mbed_dev, 
                          repo_path)

    total = len(supported_targets) * len(tests)
    current = 0
    retries = 10
    passes = 0
    failures = []
    skipped = []
    
    # Compile each test for each supported target
    for test in tests:
        for target in supported_targets:
            
            combo = [test, target]
            
            if combo in ignore_list:
                rel_log.info("SKIPPING TEST: %s, TARGET: %s", test, target)
                total -= 1   
                skipped.append(combo)
                continue
                
            current += 1
            for retry in range(0, retries):
                rel_log.info("COMPILING (%d/%d): TEST %s, TARGET: %s , attempt %u\n", current, total, test, target,  retry)
                result, mesg = build_repo(target, test, user, password)
                if not result:
                    if mesg == 'Internal':
                        # Internal compiler error thus retry
                        continue
                    else:
                        # Actual error thus move on to next compilation
                        failures.append(combo)
                        break
                                    
                passes += (int)(result)
                break
            else:
                rel_log.error("Compilation failed due to internal errors.")
                rel_log.error("Skipping test/target combination.")
                total -= 1   
                skipped.append(combo)
                
    rel_log.info(" SUMMARY OF COMPILATION RESULTS")                
    rel_log.info(" ------------------------------")                
    rel_log.info(" NUMBER OF TEST APPS: %d, NUMBER OF TARGETS: %d", 
                 len(tests), len(supported_targets))   
    log_results(failures, " FAILED")
    log_results(skipped, " SKIPPED")

    # Output a % pass rate, indicate a failure if not 100% successful
    pass_rate = (float(passes) / float(total)) * 100.0
    rel_log.info(" PASS RATE %.1f %%\n", pass_rate)
    sys.exit(not (pass_rate == 100))
