#!/usr/bin/env python

import re
import sys
import subprocess
import os
import yaml

def generate(test):
    with open('replacements.yml') as file:
        replacements = yaml.load(file)

    lines = []
    for line in re.split('(?<=[;{}])\n', test.read()):
        for pattern, replacement in replacements:
            line = re.sub(pattern, replacement, line, 0, re.DOTALL | re.MULTILINE)

        match = re.match('(?: *\n)*( *)(.*)=>(.*);', line, re.DOTALL | re.MULTILINE)
        if match:
            tab, test, expect = match.groups()
            lines.append(tab+'res = {test};'.format(test=test.strip()))
            lines.append(tab+'TEST_ASSERT_EQUAL({expect}, res);'.format(
                    name=re.match('\w*', test.strip()).group(),
                    expect=expect.strip()))
        else:
            lines.append(line)

    lines = lines[:-1]

    with open('template_subunit.fmt') as file:
        template = file.read()

    with open('main.cpp', 'a') as file:
        file.write(template.format(
                test=('\n'.join(
                    4*' '+line.replace('\n', '\n'+4*' ')
                    for line in lines))))

def main(test=None):
    if test and not test.startswith('-'):
        with open(test) as file:
            generate(file)
    else:
        generate(sys.stdin)


if __name__ == "__main__":
    main(*sys.argv[1:])
