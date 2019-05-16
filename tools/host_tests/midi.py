"""
Copyright (c) 2016 ARM Limited. All rights reserved.

SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

from __future__ import print_function
import sys
import re
import time
import mido
from mido import Message


def test_midi_in(port):
	expected_messages_count=0
	while expected_messages_count < 7:
		for message in port.iter_pending():
			if message.type in ('note_on', 'note_off', 'program_change', 'sysex'):
				yield message
				expected_messages_count+=1
		time.sleep(0.1)

def test_midi_loopback(input_port):
	expected_messages_count=0
	while expected_messages_count < 1:
		for message in input_port.iter_pending():
			print('Test MIDI OUT loopback received {}'.format(message.hex()))
			expected_messages_count+=1

def test_midi_out_loopback(output_port,input_port):
	print("Test MIDI OUT loopback")
	output_port.send(Message('program_change', program=1))
	test_midi_loopback(input_port)

	output_port.send(Message('note_on', note=21))
	test_midi_loopback(input_port)

	output_port.send(Message('note_off', note=21))
	test_midi_loopback(input_port)

	output_port.send(Message('sysex', data=[0x7E,0x7F,0x09,0x01]))
	test_midi_loopback(input_port)

	output_port.send(Message('sysex', data=[0x7F,0x7F,0x04,0x01,0x7F,0x7F]))
	test_midi_loopback(input_port)

	output_port.send(Message('sysex', data=[0x41,0x10,0x42,0x12,0x40,0x00,0x7F,0x00,0x41]))
	test_midi_loopback(input_port)

	output_port.send(Message('sysex', data=[0x41,0x10,0x42,0x12,0x40,0x00,0x04,0x7F,0x3D]))
	test_midi_loopback(input_port)

portname=""

while portname=="":
	print("Wait for MIDI IN plug ...")
	for name in mido.get_input_names():
		matchObj = re.match( r'Mbed', name)

		if matchObj:
			portname=name
	time.sleep( 1 )

try:
	input_port = mido.open_input(portname)
	output_port = mido.open_output(portname)
	
	print('Using {}'.format(input_port))

	print("Test MIDI IN")

	for message in test_midi_in(input_port):
		print('Test MIDI IN received {}'.format(message.hex()))

	test_midi_out_loopback(output_port,input_port)
except KeyboardInterrupt:
    pass
