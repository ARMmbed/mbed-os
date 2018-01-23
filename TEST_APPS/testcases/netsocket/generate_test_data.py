#
# Copyright (c) 2017, Arm Limited and affiliates.
#
import random, string

'''
This script is intended to be a common test data generator.
Currently it implements randomUppercaseAsciiString, randomLowercaseAsciiString methods.
'''

def randomUppercaseAsciiString(length):
    return ''.join(random.choice(string.ascii_uppercase) for i in range(length))


def randomLowercaseAsciiString(length):
    return ''.join(random.choice(string.ascii_lowercase) for i in range(length))
