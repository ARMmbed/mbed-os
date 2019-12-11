#!/usr/bin/env python

"""
Copyright (c) 2017-2019 ARM Limited. All rights reserved.

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
import os
from os.path import dirname, abspath, basename, join, normpath
import os.path
import sys
import copy
import stat
import subprocess
from shutil import rmtree
import json
import logging

logging.basicConfig(level=logging.DEBUG, format='[EXAMPLES]> %(levelname)-8s %(message)s')

""" Import and bulid a bunch of example programs

    This library includes functions that are shared between the examples.py and
    the update.py modules.

 """

MBED_OS_ROOT = abspath(dirname(dirname(dirname(dirname(__file__)))))
CWD = os.getcwd()
sys.path.insert(0, MBED_OS_ROOT)

from tools.build_api import get_mbed_official_release
from tools.targets import TARGET_MAP
from tools.export import EXPORTERS
from tools.project import EXPORTER_ALIASES
from tools.toolchains import TOOLCHAINS
from tools.utils import write_json_to_file
from prettytable import PrettyTable

SUPPORTED_TOOLCHAINS = list(TOOLCHAINS - set(u'uARM'))
SUPPORTED_IDES = [exp for exp in list(EXPORTERS) + list(EXPORTER_ALIASES)
                  if exp != "cmsis" and exp != "zip"]


def get_build_summary(results):
    """Prints to screen the complication results of example programs.

    Args:
    results - results of the compilation stage. which is the output of compile_repos()
    
    Returns: Numbers of failed results
    """
    pass_table = PrettyTable()
    pass_table.field_names = ["EXAMPLE NAME", "TARGET", "TOOLCHAIN", "TEST GEN", "BUILD RESULT"]
    pass_table.align["EXAMPLE NAME"] = "l"
    fail_table = copy.deepcopy(pass_table)
    failure_counter = 0

    for exp, status in list(results.items()):
        for summary in status[2]:
            pass_table.add_row([summary["name"], summary["target"], summary["toolchain"], summary["test"], "PASSED"])
        for summary in status[3]:
            fail_table.add_row([summary["name"], summary["target"], summary["toolchain"], summary["test"], "FAILED"])
            failure_counter+=1
    print("\n\nPassed Example Compilation:")
    print(pass_table)
    if (failure_counter > 0):
        print("\n\nFailed Example Compilation:")
        print(fail_table)
    print("Number of failures = %d" % failure_counter)
    return failure_counter

def get_export_summary(results):
    """Prints to screen the exporting results of example programs.

    Args:
    results -  results of the compilation stage. which is the output of and export_repos()
    
    Returns: Numbers of failed results
    """
    pass_table = PrettyTable()
    pass_table.field_names = ["EXAMPLE NAME", "TARGET", "IDE", "EXPORT RESULT", "BUILD RESULT"]
    pass_table.align["EXAMPLE NAME"] = "l"
    fail_table = copy.deepcopy(pass_table)
    
    failure_counter = 0
    for exp, status in list(results.items()):
        for summary in status[2]:
            pass_table.add_row([summary["name"], summary["target"], summary["ide"], "PASSED", "PASSED"])
        for summary in status[3]:
            fail_table.add_row([summary["name"], summary["target"], summary["ide"], "FAILED", ""])
            failure_counter+=1
        for summary in status[4]:
            fail_table.add_row([summary["name"], summary["target"], summary["ide"], "PASSED", "FAILED"])
            failure_counter+=1
        for summary in status[5]:
            pass_table.add_row([summary["name"], summary["target"], summary["ide"], "PASSED", "SKIPPED"])
    
    print("\n\nPassed Example Exporting:")
    print(pass_table)
    if (failure_counter > 0):
        print("\n\nFailed Example Exporting:")
        print(fail_table)
    print("Number of failures = %d" % failure_counter)
    return failure_counter

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


def get_sub_examples_list(example):
    """ Get the names of sub examples. if no sub examples, return the name of main example"""
    sub_examples = []
    if example['sub-repo-example']:
        for sub in example['subs']:
            sub_examples.append("%s/%s" % (example["name"], sub))
    else:
        sub_examples.append(example["name"])
    return sub_examples

def source_repos(config, exp_filter):
    """ Imports each of the repos and its dependencies (.lib files) associated
        with the specific examples name from the json config file. Note if
        there is already a clone of the repo then it will first be removed to
        ensure a clean, up to date cloning.
    Args:
    config - the json object imported from the file.

    """
    print("\nImporting example repos....\n")
    for example in config['examples']:
        name = example['name']
        if name in exp_filter:
            if os.path.exists(name):
                logging.warning("'%s' example directory already exists. Deleting..." % name)
                rmtree(name)
            
            cmd = "mbed-cli import %s" % example['github']
            logging.info("Executing command '%s'..." % cmd)
            result = subprocess.call(cmd, shell=True)
            if result:
                return result
    return 0                

def clone_repos(config, exp_filter , retry = 3):
    """ Clones each of the repos associated with the specific examples name from the
        json config file. Note if there is already a clone of the repo then it will first
        be removed to ensure a clean, up to date cloning.
    Args:
    config - the json object imported from the file.

    """
    print("\nCloning example repos....\n")
    for example in config['examples']:
        name = example['name']
        if name in exp_filter:
            if os.path.exists(name):
                logging.warning("'%s' example directory already exists. Deleting..." % name)
                rmtree(name, onerror=remove_readonly)

            cmd = "git clone %s" % example['github']
            for i in range(0, retry):
                logging.info("Executing command '%s'..." % cmd)
                if not subprocess.call(cmd, shell=True):                        
                    break
            else:
                logging.error("unable to clone the repo '%s'" % name)
                return 1
    return 0

def deploy_repos(config, exp_filter):
    """ If the example directory exists as provided by the json config file,
        pull in the examples dependencies by using `mbed-cli deploy`.
    Args:
    config - the json object imported from the file.

    """
    print("\nDeploying example repos....\n")
    for example in config['examples']:
        name = example['name']
        if name in exp_filter:
            if os.path.exists(name):
                os.chdir(name)
                logging.info("In folder '%s'" % name)
                cmd = "mbed-cli deploy"
                logging.info("Executing command '%s'..." % cmd)
                result = subprocess.call(cmd, shell=True)
                os.chdir(CWD)
                if result:
                    logging.error("mbed-cli deploy command failed for '%s'" % name)
                    return result                
            else:
                logging.info("'%s' example directory doesn't exist. Skipping..." % name)
                return 1
    return  0

def export_repos(config, ides, targets, exp_filter):
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

    print("\nExporting example repos....\n")
    for example in config['examples']:
        if example['name'] not in exp_filter:
            continue
        export_failures = []
        build_failures = []
        build_skips = []
        successes = []
        exported = True
        pass_status = True
        if example['export']:
            for name in get_sub_examples_list(example):              
                os.chdir(name)
                logging.info("In folder '%s'" % name)
                # Check that the target, IDE, and features combinations are valid and return a
                # list of valid combinations to work through
                for target, ide in target_cross_ide(valid_choices(example['targets'], targets),
                                                    valid_choices(example['exporters'], ides),
                                                    example['features'], example['toolchains']):
                    example_summary = {"name" : name, "target" : target, "ide" : ide }
                    summary_string = "%s %s %s" % (name, target, ide)
                    logging.info("Exporting %s"  % summary_string)
                    
                    cmd = ["mbed-cli", "export", "-i", ide, "-m", target]
                    logging.info("Executing command '%s'..." % " ".join(cmd))
                    proc = subprocess.Popen(cmd)
                    proc.wait()
                    if proc.returncode:
                        export_failures.append(example_summary)
                        logging.error("FAILURE exporting %s"  % summary_string)
                    else:
                        logging.info("SUCCESS exporting %s"  % summary_string)
                        logging.info("Building %s"  % summary_string)
                        try:
                            if EXPORTERS[ide].build(name, cleanup=False):
                                logging.error("FAILURE building %s"  % summary_string)
                                build_failures.append(example_summary)
                            else:
                                logging.info("SUCCESS building %s"  % summary_string)
                                successes.append(example_summary)
                        except TypeError:
                            successes.append(example_summary)
                            build_skips.append(example_summary)
                os.chdir(CWD)

                if len(build_failures+export_failures) > 0:
                    pass_status= False
        else:
            exported = False

        results[example['name']] = [exported, pass_status, successes,
                                    export_failures, build_failures, build_skips]

    return results


def compile_repos(config, toolchains, targets, profiles, verbose, exp_filter, jobs=0):
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
    targets - list of target names
    profile - build profile path or name if in default place
    verbose - enabling verbose
    exp_filter - List of exp_filter to be build
    jobs - Number of compile jobs

    """
    results = {}
    test_json = {"builds":{}}
    base_path = os.getcwd()
    print("\nCompiling example repos....\n")
    for example in config['examples']:
        if example['name'] not in exp_filter:
            continue
        failures = []
        successes = []
        compiled = True
        pass_status = True
        if example['test']:
            if not ('baud_rate' in example and 'compare_log'in example):
                logging.warning("'baud_rate' or 'compare_log' keys are missing from config json file")
                example['test'] = False
        if example['compile']:
            for name in get_sub_examples_list(example):
                os.chdir(name)
                logging.info("In folder '%s'" % name)
                # Check that the target, toolchain and features combinations are valid and return a
                # list of valid combinations to work through
                for target, toolchain in target_cross_toolchain(valid_choices(example['targets'], targets),
                                                                valid_choices(example['toolchains'], toolchains),
                                                                example['features']):
                    example_summary = {"name" : name, "target" : target, "toolchain" : toolchain, "test": "UNSET"}
                    summary_string = "%s %s %s" % (name, target, toolchain)
                    logging.info("Compiling %s" % summary_string)

                    build_command = ["mbed-cli", "compile", "-t", toolchain, "-m", target, "-j", str(jobs)] + (['-vv'] if verbose else [])
                    if profiles:
                        for profile in profiles:
                            build_command.extend(["--profile", profile])

                    logging.info("Executing command '%s'..." % " ".join(build_command))
                    proc = subprocess.Popen(build_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

                    std_out, std_err = proc.communicate()
                    std_out = std_out.decode()
                    std_err = std_err.decode()
                    print ("\n#### STDOUT ####\n%s\n#### STDERR ####\n%s\n#### End of STDOUT/STDERR ####\n" % (std_out,std_err))

                    if proc.returncode:
                        failures.append(example_summary)
                    else:
                        if example['test']:
                            log = example['compare_log'].pop(0)
                            # example['compare_log'] is a list of log file/files, which matches each examples/sub-examples from same repo. 
                            # pop the log file out of list regardless the compilation for each example pass of fail
                            image = fetch_output_image(std_out)
                            if image:
                                image_info = [{"binary_type": "bootable","path": normpath(join(name,image)),"compare_log":log}]
                                test_group = "{}-{}-{}".format(target, toolchain, example['baud_rate'])
                                if not test_group in test_json['builds']:
                                    test_json['builds'][test_group] = {
                                        "platform":target ,
                                        "toolchain": toolchain ,
                                        "base_path": base_path ,
                                        "baud_rate": int(example['baud_rate']), 
                                        "tests":{} }
                                test_json['builds'][test_group]['tests'][name]={"binaries":image_info}
                                example_summary["test"] = "TEST_ON"
                                
                            else:
                                logging.warning("could not find built image for example %s" % name)
                                example_summary["test"]  = "NO_IMAGE"
                        else:
                            logging.warning("Test for %s will not be generated." % name)
                            example_summary["test"]  = "TEST_OFF"
                        successes.append(example_summary)

                os.chdir(CWD)

            # If there are any compilation failures for the example 'set' then the overall status is fail.
            if len(failures) > 0:
                pass_status = False
        else:
            compiled = False

        results[example['name']] = [compiled, pass_status, successes, failures]

    write_json_to_file(test_json, "test_spec.json")
    return results


def update_mbedos_version(config, tag, exp_filter):
    """ For each example repo identified in the config json object, update the version of
        mbed-os to that specified by the supplied GitHub tag. This function assumes that each
        example repo has already been cloned.

    Args:
    config - the json object imported from the file.
    tag - GitHub tag corresponding to a version of mbed-os to upgrade to.

    """
    print("\nUpdating mbed-os in examples to version '%s'\n" % tag)
    for example in config['examples']:
        if example['name'] not in exp_filter:
            continue
        for name in get_sub_examples_list(example):
            update_dir =  name + "/mbed-os"
            os.chdir(update_dir)
            logging.info("In folder '%s'" % name)
            cmd = "mbed-cli update %s --clean" %tag
            logging.info("Executing command '%s'..." % cmd)
            result = subprocess.call(cmd, shell=True)
            os.chdir(CWD)
            if result:
                return result
    return 0

def symlink_mbedos(config, path, exp_filter):
    """ Create a symbolic link in each example folder to given path 
        If a mbed-os.lib can be found in the folder, it will be removed
    """
    print("\nCreating mbed-os Symbolic link to '%s'\n" % path)
    for example in config['examples']:
        if example['name'] not in exp_filter:
            continue
        for name in get_sub_examples_list(example):
            os.chdir(name)
            logging.info("In folder '%s'" % name)
            if os.path.exists("mbed-os.lib"):
                logging.info("Removing 'mbed-os.lib' in '%s'" % name)
                os.remove("mbed-os.lib")
            else:
                logging.warning("No 'mbed-os.lib' found in '%s'" % name)
            if os.path.exists("mbed-os"):
                logging.warning("'mbed-os' already existed in '%s'" % name)
            else:
                logging.info("Creating Symbolic link '%s'->'mbed-os'" % path)
                os.symlink(path, "mbed-os")
            os.chdir(CWD)
    return 0

def fetch_output_image(output):
    """Find the build image from the last 30 lines of a given log"""
    lines = output.splitlines()
    last_index = -31 if len(lines)>29 else (-1 - len(lines))
    for index in range(-1,last_index,-1):
        if lines[index].startswith("Image:"):
            image = lines[index][7:]
            if os.path.isfile(image):
                return image
    return False

