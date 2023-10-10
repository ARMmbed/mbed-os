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
import re
import json
from subprocess import Popen, PIPE
from .mbed_greentea_log import gt_logger

"""
List of available hooks:
"""


class GreenteaTestHook(object):
    """! Class used to define
    """
    name = None

    def __init__(self, name):
        self.name = name

    def run(self, format=None):
        pass

class GreenteaCliTestHook(GreenteaTestHook):
    """! Class used to define a hook which will call command line program
    """
    cmd = None

    def __init__(self, name, cmd):
        GreenteaTestHook.__init__(self, name)
        self.cmd = cmd

    def run_cli_process(self, cmd):
        """! Runs command as a process and return stdout, stderr and ret code
        @param cmd Command to execute
        @return Tuple of (stdout, stderr, returncode)
        """
        _stdout, _stderr, ret = None, None, -1
        try:
            p = Popen(cmd, stdout=PIPE, stderr=PIPE, shell=True)
            _stdout, _stderr = p.communicate()
            ret = p.returncode
        except OSError as e:
            gt_logger.gt_log_err(str(e))
            ret = -1
        return _stdout, _stderr, ret

    def run(self, format=None):
        """! Runs hook after command is formated with in-place {tags}
        @format Pass format dictionary to replace hook {tags} with real values
        @param format Used to format string with cmd, notation used is e.g: {build_name}
        """
        gt_logger.gt_log("hook '%s' execution"% self.name)
        cmd = self.format_before_run(self.cmd, format)
        gt_logger.gt_log_tab("hook command: %s"% cmd)
        (_stdout, _stderr, ret) = self.run_cli_process(cmd)
        if _stdout:
            print(_stdout)
        if ret:
            gt_logger.gt_log_err("hook exited with error: %d, dumping stderr..."% ret)
            print(_stderr)
        return ret

    @staticmethod
    def format_before_run(cmd, format, verbose=False):
        if format:
            # We will expand first
            cmd_expand = GreenteaCliTestHook.expand_parameters(cmd, format)
            if cmd_expand:
                cmd = cmd_expand
                if verbose:
                    gt_logger.gt_log_tab("hook expanded: %s"% cmd)

            cmd = cmd.format(**format)
            if verbose:
                gt_logger.gt_log_tab("hook formated: %s"% cmd)
        return cmd

    @staticmethod
    def expand_parameters(expr, expandables, delimiter=' '):
        """! Expands lists for multiple parameters in hook command
        @param expr Expression to expand
        @param expandables Dictionary of token: list_to_expand See details for more info
        @param delimiter Delimiter used to combine expanded strings, space by default
        @details
        test_name_list = ['mbed-drivers-test-basic', 'mbed-drivers-test-hello', 'mbed-drivers-test-time_us']
        build_path_list = ['./build/frdm-k64f-gcc', './build/frdm-k64f-armcc']
        expandables = {
            "{test_name_list}": test_name_list,
            "{build_path_list}": build_path_list
        }
        expr = "lcov --gcov-tool arm-none-eabi-gcov [-a {build_path_list}/test/{test_name_list}.info] --output-file result.info"
        'expr' expression [-a {build_path_list}/test/{test_name_list}.info] will expand to:
        [
          "-a ./build/frdm-k64f-gcc/test/mbed-drivers-test-basic.info",
          "-a ./build/frdm-k64f-armcc/test/mbed-drivers-test-basic.info",
          "-a ./build/frdm-k64f-gcc/test/mbed-drivers-test-hello.info",
          "-a ./build/frdm-k64f-armcc/test/mbed-drivers-test-hello.info",
          "-a ./build/frdm-k64f-gcc/test/mbed-drivers-test-time_us.info",
          "-a ./build/frdm-k64f-armcc/test/mbed-drivers-test-time_us.info"
        ]
        """
        result = None
        if expandables:
            expansion_result = []
            m = re.search('\[.*?\]', expr)
            if m:
                expr_str_orig = m.group(0)
                expr_str_base = m.group(0)[1:-1]
                expr_str_list = [expr_str_base]
                for token in expandables:
                    # We will expand only values which are lists (of strings)
                    if type(expandables[token]) is list:
                        # Use tokens with curly braces (Python string format like)
                        format_token = '{' + token + '}'
                        for expr_str in expr_str_list:
                            if format_token in expr_str:
                                patterns = expandables[token]
                                for pattern in patterns:
                                    s = expr_str
                                    s = s.replace(format_token, pattern)
                                    expr_str_list.append(s)
                                    # Nothing to extend/change in this string
                                    if not any('{' + p + '}' in s for p in expandables.keys() if type(expandables[p]) is list):
                                        expansion_result.append(s)
                expansion_result.sort()
                result = expr.replace(expr_str_orig, delimiter.join(expansion_result))
        return result

class LcovHook(GreenteaCliTestHook):
    """! Class used to define a LCOV hook
    """
    lcov_hooks = {
        "hooks": {
            "hook_test_end": "$lcov --gcov-tool gcov  --capture --directory ./build --output-file {build_path}/{test_name}.info",
            "hook_post_all_test_end": "$lcov --gcov-tool gcov [(-a << {build_path}/{test_name_list}.info>>)] --output-file result.info"
        }
    }

    def __init__(self, name, cmd):
        GreenteaCliTestHook.__init__(self, name, cmd)

    @staticmethod
    def format_before_run(cmd, format, verbose=False):
        if format:
            # We will expand first
            cmd_expand = GreenteaCliTestHook.expand_parameters(cmd, format)
            if cmd_expand:
                cmd = cmd_expand
                if verbose:
                    gt_logger.gt_log_tab("hook expanded: %s"% cmd)

            cmd = cmd.format(**format)
            cmd = LcovHook.check_if_file_exists_or_is_empty(cmd)
            if verbose:
                gt_logger.gt_log_tab("hook formated: %s"% cmd)
        return cmd

    @staticmethod
    def check_if_file_exists_or_is_empty(expr):
        """! Check Expression for specific characters in hook command
        @param expr Expression to check
        @details
        expr = "lcov --gcov-tool gcov (-a <<{build_path}/test/{test_name}.info>>) --output-file result.info"
            where:
            (...) -> specify part to check
            <<...>> -> specify part which is a path to a file
        'expr' expression (-a <<{build_path}/test/{test_name}.info>>) will be either:
            "-a ./build/frdm-k64f-gcc/test/test_name.info"
            or will be removed from command
        It is also possible to use it in combination with expand_parameters:
        expr = "lcov --gcov-tool gcov [(-a <<./build/{yotta_target_name}/{test_name_list}.info>>)] --output-file result.info"
        """
        result = expr
        expr_strs_orig = re.findall('\(.*?\)', expr)
        for expr_str_orig in expr_strs_orig:
            expr_str_base = expr_str_orig[1:-1]
            result = result.replace(expr_str_orig, expr_str_base)
            m = re.search('\<<.*?\>>', expr_str_base)
            if m:
                expr_str_path = m.group(0)[2:-2]
                # Remove option if file not exists OR if file exists but empty
                if not os.path.exists(expr_str_path):
                    result = result.replace(expr_str_base, '')
                elif os.path.getsize(expr_str_path) == 0:
                    result = result.replace(expr_str_base, '')

        # Remove path limiter
        result = result.replace('<<', '')
        result = result.replace('>>', '')
        return result

class GreenteaHooks(object):
    """! Class used to store all hooks
    @details Hooks command starts with '$' dollar sign
    """
    HOOKS = {}
    def __init__(self, path_to_hooks):
        """! Opens JSON file with
        """
        try:
            if path_to_hooks == 'lcov':
                hooks = LcovHook.lcov_hooks
                for hook in hooks['hooks']:
                    hook_name = hook
                    hook_expression = hooks['hooks'][hook]
                    self.HOOKS[hook_name] = LcovHook(hook_name, hook_expression[1:])
            else:
                with open(path_to_hooks, 'r') as data_file:
                    hooks = json.load(data_file)
                    if 'hooks' in hooks:
                        for hook in hooks['hooks']:
                            hook_name = hook
                            hook_expression = hooks['hooks'][hook]
                            # This is a command line hook
                            if hook_expression.startswith('$'):
                                self.HOOKS[hook_name] = GreenteaCliTestHook(hook_name, hook_expression[1:])
        except IOError as e:
            print(str(e))
            self.HOOKS = None

    def is_hooked_to(self, hook_name):
        return hook_name in self.HOOKS

    def run_hook(self, hook_name, format):
        if hook_name in self.HOOKS:
            return self.HOOKS[hook_name].run(format)

    def run_hook_ext(self, hook_name, format):
        if self.is_hooked_to(hook_name):
            # We can execute this test hook just after all tests are finished ('hook_post_test_end')
            self.run_hook(hook_name, format)
