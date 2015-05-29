"""
mbed SDK
Copyright (c) 2010-2013 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


Example:
> from mbedRPC import*
> mbed = SerialRPC("COM5",9600);
> myled = DigitalOut(mbed, "myled"); <--- Where the text in quotations matches your 
										  RPC pin definition's second parameter, in 
										  this case it could be RpcDigitalOut myled(LED1,"myled");
> myled.write(1)
>
"""

import serial, urllib2, time

#mbed super class
class mbed:
	def __init__(self):
		print("This will work as a demo but no transport mechanism has been selected")
		
	def rpc(self, name, method, args):
		print("Superclass method not overridden")

#Transport mechanisms, derived from mbed
class SerialRPC(mbed):

	def __init__(self,port, baud):
		self.ser = serial.Serial(port)
		self.ser.setBaudrate(baud)

	def rpc(self, name, method, args):
		#creates the command to be sent serially - /name/method arg1 arg2 arg3 ... argN
		str = "/" + name + "/" + method + " " + " ".join(args) + "\n"
		#prints the command being executed
		print str 
		#writes the command to serial
		self.ser.write(str) 
		#strips trailing characters from the line just written
		ret_val = self.ser.readline().strip() 
		return ret_val

class HTTPRPC(mbed):

	def __init__(self, ip):
		self.host = "http://" + ip

	def rpc(self, name, method, args):
		response = urllib2.urlopen(self.host + "/rpc/" + name + "/" + method + "," + ",".join(args))
		return response.read().strip()

		
#generic mbed interface super class		
class mbed_interface():
	#initialize an mbed interface with a transport mechanism and pin name
	def __init__(self, this_mbed , mpin):
		self.mbed = this_mbed
		if isinstance(mpin,str):
			self.name = mpin
	def __del__(self):	
		r = self.mbed.rpc(self.name, "delete", [])
	#generic read
	def read(self):
		r = self.mbed.rpc(self.name, "read", [])
		return int(r)
		
#for classes that need write functionality - inherits from the generic reading interface
class mbed_interface_write(mbed_interface):
	
	def __init__(self, this_mbed , mpin):
		mbed_interface.__init__(self,this_mbed,mpin)
	#generic write 
	def write(self, value):
		r = self.mbed.rpc(self.name, "write", [str(value)])	

#mbed interfaces
class DigitalOut(mbed_interface_write):

	def __init__(self, this_mbed , mpin):
		mbed_interface_write.__init__(self,this_mbed,mpin)
	  
class AnalogIn(mbed_interface):

	def __init__(self, this_mbed , mpin):
		mbed_interface.__init__(self,this_mbed,mpin)

	def read_u16(self):
		r = self.mbed.rpc(self.name, "read_u16", [])
		return int(r)

class AnalogOut(mbed_interface_write):

	def __init__(self, this_mbed , mpin):
		mbed_interface_write.__init__(self,this_mbed,mpin)

	def write_u16(self, value):
		self.mbed.rpc(self.name, "write_u16", [str(value)])

	def read(self):
		r = self.mbed.rpc(self.name, "read", [])
		return float(r)

class DigitalIn(mbed_interface):

	def __init__(self, this_mbed , mpin):
		mbed_interface.__init__(self,this_mbed,mpin)

class PwmOut(mbed_interface_write):
	
	def __init__(self, this_mbed , mpin):
		mbed_interface_write.__init__(self,this_mbed,mpin)

	def read(self):
		r = self.mbed.rpc(self.name, "read", [])
		return r

	def period(self, value):
		self.mbed.rpc(self.name, "period", [str(value)])
		
	def period_ms(self, value):
		self.mbed.rpc(self.name, "period_ms", [str(value)])

	def period_us(self, value):
		self.mbed.rpc(self.name, "period_us", [str(value)])
		
	def pulsewidth(self, value):
		self.mbed.rpc(self.name, "pulsewidth", [str(value)])
		
	def pulsewidth_ms(self, value):
		self.mbed.rpc(self.name, "pulsewidth_ms", [str(value)])

	def pulsewidth_us(self, value):
		self.mbed.rpc(self.name, "pulsewidth_us", [str(value)])

class RPCFunction(mbed_interface):

	def __init__(self, this_mbed , name):
		mbed_interface.__init__(self,this_mbed,name)

	def run(self, input):
		r = self.mbed.rpc(self.name, "run", [input])
		return r

class RPCVariable(mbed_interface_write):

	def __init__(self, this_mbed , name):
		mbed_interface_write.__init__(self,this_mbed,name)

	def read(self):
		r = self.mbed.rpc(self.name, "read", [])
		return r
#Serial
class Serial():
		
	def __init__(self, this_mbed , tx, rx = ""):
		self.mbed = this_mbed
		if isinstance(tx, str):
			self.name = tx
	def __del__(self):
		r = self.mbed.rpc(self.name, "delete", [])

	def putc(self, value):
		r = self.mbed.rpc(self.name, "putc", [str(value)])

	def puts(self, value):
		r = self.mbed.rpc(self.name, "puts", [ "\"" + str(value) + "\""])

	def getc(self):
		r = self.mbed.rpc(self.name, "getc", [])
		return int(r)


def wait(s):
	time.sleep(s)
