#! /usr/bin/env python3
#
# -----------------------------------------------------------------------------
# Copyright (c) 2019, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
# -----------------------------------------------------------------------------


import re
import os

expression_re = re.compile(r"[(]?(([(]?(((0x)[0-9a-fA-F]+)|([0-9]+))[)]?)\s*([\+\-]\s*([(]?(((0x)[0-9a-fA-F]+)|([0-9]+))[)]?)\s*)*)[)]?")

# Simple parser that takes a string and evaluates an expression from it.
# The expression might contain additions and subtractions amongst numbers that
# are written in decimal or hexadecimal form.
# The parses can process expressions in which the parentheses does not change
# the sign of the following number or numbers in an expression.
# Thus the parser can process the following expression: (x + y)
# However it will not calculate the correct sum for the expression below:
# (x - (y + z))
def parse_and_sum(text):
    m = expression_re.match(text)
    if m is None:
        msg = "The script was probably invoked manually"
        msg += " with having certain macros nested in flash_layouts.h.\n"
        msg += "Please revisit the flash_layout.h file and hardcode values"
        msg += " for the (NON-)SECURE_IMAGE_OFFSET and"
        msg += " (NON-)SECURE_IMAGE_MAX_SIZE macros"
        raise Exception(msg)

    nums = re.findall(r'(0x[A-Fa-f0-9]+)|[\d]+', m.group(0))
    for i in range(len(nums)):
        nums[i] = int(nums[i], 0)
    ops = re.findall(r'\+|\-', m.group(0))
    sum = nums[0]
    for i in range(len(ops)):
        if ops[i] == '+':
            sum += nums[i+1]
        else:
            sum -= nums[i+1]
    return sum


# Opens a file that contains the macro of interest, then finds the macro with
# a regular expression, parses the expression that is defined for the given
# macro. Lastly it evaluates the expression with the parse_and_sum function
def evaluate_macro(file, regexp, matchGroupKey, matchGroupData, bracketless=False):
    regexp_compiled = re.compile(regexp)

    if os.path.isabs(file):
        configFile = file
    else:
        scriptsDir = os.path.dirname(os.path.abspath(__file__))
        configFile = os.path.join(scriptsDir, file)

    macroValue = {}
    with open(configFile, 'r') as macros_preprocessed_file:
        for line in macros_preprocessed_file:
            if bracketless:
                line=line.replace("(","")
                line=line.replace(")","")
            m = regexp_compiled.match(line)
            if m is not None:
                macroValue[m.group(matchGroupKey)] = \
                parse_and_sum(m.group(matchGroupData))

    if (matchGroupKey == 0 and not macroValue):
        macroValue["None"] = None

    return list(macroValue.values())[0] if (matchGroupKey == 0) else macroValue
