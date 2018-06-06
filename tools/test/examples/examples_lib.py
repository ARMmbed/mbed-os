""" Import and bulid a bunch of example programs

    This library includes functions that are shared between the examples.py and
    the update.py modules.

 """
import os
from os.path import dirname, abspath, basename
import os.path
import sys
import subprocess
from shutil import rmtree

ROOT = abspath(dirname(dirname(dirname(dirname(__file__)))))
sys.path.insert(0, ROOT)

from tools.build_api import get_mbed_official_release
from tools.targets import TARGET_MAP
from tools.export import EXPORTERS
from tools.toolchains import TOOLCHAINS

SUPPORTED_TOOLCHAINS = list(TOOLCHAINS - set(u'uARM'))
SUPPORTED_IDES = [exp for exp in EXPORTERS.keys() if exp != "cmsis" and exp != "zip"]


def print_list(lst):
    """Prints to screen the contents of a list

    Args:
    lst - a list of any type, to be displayed

    """
    if lst:
        for thing in lst:
            print("# %s" % thing)


def print_category(results, index, message):
    summary = [example for key, summ in results.items()
               for example in summ[index]]
    if all(len(s) == 0 for s in summary):
        return
    print("#")
    print("#" * 80)
    print("# %s" % message)
    print("#" * 80)
    split_summ = [s.rsplit(" ", 1) for s in summary]

    print_list(summary)


def print_summary(results, export=False):
    """Prints to screen the results of compiling/exporting combinations of example programs,
       targets and compile toolchains/IDEs.

    Args:
    results - results of the compilation stage. See compile_repos() and export_repos()
              for details of the format.

    """

    print("#"*80)
    print("# Examples compilation summary")
    print("#"*80)

    print_category(results, 2, "Passed example combinations")

    second_result = "Failed example combinations" if not export else \
        "Failed export example combinations"

    print_category(results, 3, second_result)

    if export:
        print_category(results, 4, "Failed build combinations")
        print_category(results, 5, "Skipped build combinations")

    print("#")
    print("#"*80)

def valid_choices(allowed_choices, all_choices):
    if len(allowed_choices) > 0:
        return [t for t in all_choices if t in allowed_choices]
    else:
        return all_choices


def target_cross_toolchain(allowed_targets, allowed_toolchains, features=[]):
    """Generate pairs of target and toolchains

    Args:
    allowed_targets - a list of all possible targets
    allowed_toolchains - a list of all possible toolchains

    Kwargs:
    features - the features that must be in the features array of a
               target
    """
    for target in allowed_targets:
        for toolchain in allowed_toolchains:
            if all(feature in TARGET_MAP[target].features
                    for feature in features):
                yield target, toolchain


def target_cross_ide(allowed_targets, allowed_ides, features=[], toolchains=[]):
    """Generate pairs of target and ides

    Args:
    allowed_targets - a list of all possible targets
    allowed_ides - a list of all possible IDEs

    Kwargs:
    features - the features that must be in the features array of a
               target
    """
    for target in allowed_targets:
        for ide in allowed_ides:
            if (EXPORTERS[ide].is_target_supported(target) and
                (not toolchains or EXPORTERS[ide].TOOLCHAIN in toolchains) and
                all(feature in TARGET_MAP[target].features
                    for feature in features)):
                yield target, ide


def get_repo_list(example):
    """ Returns a list of all the repos and their types associated with the
        specific example in the json config file.
        If the key 'test-repo-source' is set to 'mbed', then it will return the
        mbed section as a list. Otherwise, it will return the single github repo.
        NOTE: This does not currently deal with multiple examples underneath a github
        sourced exampe repo.

    Args:
    example - Example for which the repo list is requested

    """
    repos = []
    if example['test-repo-source'] == 'mbed':
        for repo in example['mbed']:
            repos.append({
                'repo': repo,
                'type': 'hg'
            })
    else:
        repos.append({
            'repo': example['github'],
            'type': 'git'
        })
    return repos


def source_repos(config, examples):
    """ Imports each of the repos and its dependencies (.lib files) associated
        with the specific examples name from the json config file. Note if
        there is already a clone of the repo then it will first be removed to
        ensure a clean, up to date cloning.
    Args:
    config - the json object imported from the file.

    """
    print("\nImporting example repos....\n")
    for example in config['examples']:
        for repo_info in get_repo_list(example):
            name = basename(repo_info['repo'])
            if name in examples:
                if os.path.exists(name):
                    print("'%s' example directory already exists. Deleting..." % name)
                    rmtree(name)

                subprocess.call(["mbed-cli", "import", repo_info['repo']])

def clone_repos(config, examples , retry = 3):
    """ Clones each of the repos associated with the specific examples name from the
        json config file. Note if there is already a clone of the repo then it will first
        be removed to ensure a clean, up to date cloning.
    Args:
    config - the json object imported from the file.

    """
    print("\nCloning example repos....\n")
    for example in config['examples']:
        for repo_info in get_repo_list(example):
            name = basename(repo_info['repo'])
            if name in examples:
                if os.path.exists(name):
                    print("'%s' example directory already exists. Deleting..." % name)
                    rmtree(name)
                for i in range(0, retry):
                    if subprocess.call([repo_info['type'], "clone", repo_info['repo']]) == 0:
                        break
                else:
                    print("ERROR : unable to clone the repo {}".format(name))

def deploy_repos(config, examples):
    """ If the example directory exists as provided by the json config file,
        pull in the examples dependencies by using `mbed-cli deploy`.
    Args:
    config - the json object imported from the file.

    """
    print("\nDeploying example repos....\n")
    for example in config['examples']:
        for repo_info in get_repo_list(example):
            name = basename(repo_info['repo'])
            if name in examples:
                if os.path.exists(name):
                    os.chdir(name)
                    subprocess.call(["mbed-cli", "deploy"])
                    os.chdir("..")
                else:
                    print("'%s' example directory doesn't exist. Skipping..." % name)


def get_num_failures(results, export=False):
    """ Returns the number of failed compilations from the results summary
    Args:
    results - results summary of the compilation stage. See compile_repos() for
              details of the format.
    num_failures

    """
    num_failures = 0

    for key, val in results.items():
        num_failures = num_failures + len(val[3])
        if export:
            num_failures += len(val[4])

    return num_failures

def export_repos(config, ides, targets, examples):
    """Exports and builds combinations of example programs, targets and IDEs.

        The results are returned in a [key: value] dictionary format:
            Where key = The example name from the json config file
            value = a list containing: pass_status, successes, export failures, build_failures,
            and build_skips

            where pass_status = The overall pass status for the export of the full
            set of example programs comprising the example suite.
            IE they must build and export) True if all examples pass, false otherwise
            successes = list of examples that exported and built (if possible)
            If the exporter has no build functionality, then it is a pass
            if exported
            export_failures = list of examples that failed to export.
            build_failures = list of examples that failed to build
            build_skips = list of examples that cannot build

            Both successes and failures contain the example name, target and IDE

            Args:
            config - the json object imported from the file.
            ides - List of IDES to export to
    """
    results = {}
    valid_examples = set(examples)
    print("\nExporting example repos....\n")
    for example in config['examples']:
        example_names = [basename(x['repo']) for x in get_repo_list(example)]
        common_examples = valid_examples.intersection(set(example_names))
        if not common_examples:
            continue
        export_failures = []
        build_failures = []
        build_skips = []
        successes = []
        exported = True
        pass_status = True
        if example['export']:
            for repo_info in get_repo_list(example):
                example_project_name = basename(repo_info['repo'])
                os.chdir(example_project_name)
                # Check that the target, IDE, and features combinations are valid and return a
                # list of valid combinations to work through
                for target, ide in target_cross_ide(valid_choices(example['targets'], targets),
                                                    valid_choices(example['exporters'], ides),
                                                    example['features'], example['toolchains']):
                    example_name = "{} {} {}".format(example_project_name, target,
                                                     ide)
                    def status(message):
                        print(message + " %s" % example_name)
                        sys.stdout.flush()

                    status("Exporting")
                    proc = subprocess.Popen(["mbed-cli", "export", "-i", ide,
                                             "-m", target])
                    proc.wait()
                    if proc.returncode:
                        export_failures.append(example_name)
                        status("FAILURE exporting")
                    else:
                        status("SUCCESS exporting")
                        status("Building")
                        try:
                            if EXPORTERS[ide].build(example_project_name, cleanup=False):
                                status("FAILURE building")
                                build_failures.append(example_name)
                            else:
                                status("SUCCESS building")
                                successes.append(example_name)
                        except TypeError:
                            successes.append(example_name)
                            build_skips.append(example_name)
                os.chdir("..")

                if len(build_failures+export_failures) > 0:
                    pass_status= False
        else:
            exported = False

        results[example['name']] = [exported, pass_status, successes,
                                    export_failures, build_failures, build_skips]

    return results


def compile_repos(config, toolchains, targets, profile, examples):
    """Compiles combinations of example programs, targets and compile chains.

       The results are returned in a [key: value] dictionary format:
       Where key = The example name from the json config file
             value = a list containing: pass_status, successes, and failures

             where pass_status = The overall pass status for the compilation of the full
                                 set of example programs comprising the example suite.
                                 True if all examples pass, false otherwise
                   successes = list of passing examples.
                   failures = list of failing examples.

                   Both successes and failures contain the example name, target and compile chain

    Args:
    config - the json object imported from the file.
    toolchains - List of toolchains to compile for.
    results - results of the compilation stage.

    """
    results = {}
    valid_examples = set(examples)
    print("\nCompiling example repos....\n")
    for example in config['examples']:
        example_names = [basename(x['repo']) for x in get_repo_list(example)]
        common_examples = valid_examples.intersection(set(example_names))
        if not common_examples:
            continue
        failures = []
        successes = []
        compiled = True
        pass_status = True
        if example['compile']:
            for repo_info in get_repo_list(example):
                name = basename(repo_info['repo'])
                os.chdir(name)

                # Check that the target, toolchain and features combinations are valid and return a
                # list of valid combinations to work through
                for target, toolchain in target_cross_toolchain(valid_choices(example['targets'], targets),
                                                                valid_choices(example['toolchains'], toolchains),
                                                                example['features']):
                    print("Compiling %s for %s, %s" % (name, target, toolchain))
                    build_command = ["mbed-cli", "compile", "-t", toolchain, "-m", target, "-v"]

                    if profile:
                        build_command.append("--profile")
                        build_command.append(profile)

                    proc = subprocess.Popen(build_command)

                    proc.wait()
                    example_summary = "{} {} {}".format(name, target, toolchain)
                    if proc.returncode:
                        failures.append(example_summary)
                    else:
                        successes.append(example_summary)
                os.chdir("..")

            # If there are any compilation failures for the example 'set' then the overall status is fail.
            if len(failures) > 0:
                pass_status = False
        else:
            compiled = False

        results[example['name']] = [compiled, pass_status, successes, failures]

    return results


def update_mbedos_version(config, tag, examples):
    """ For each example repo identified in the config json object, update the version of
        mbed-os to that specified by the supplied GitHub tag. This function assumes that each
        example repo has already been cloned.

    Args:
    config - the json object imported from the file.
    tag - GitHub tag corresponding to a version of mbed-os to upgrade to.

    """
    print("Updating mbed-os in examples to version %s\n" % tag)
    for example in config['examples']:
        if example['name'] not in examples:
            continue
        for repo_info in get_repo_list(example):
            update_dir =  basename(repo_info['repo']) + "/mbed-os"
            print("\nChanging dir to %s\n" % update_dir)
            os.chdir(update_dir)
            subprocess.call(["mbed-cli", "update", tag, "--clean"])
            os.chdir("../..")
