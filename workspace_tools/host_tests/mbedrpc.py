# mbedRPC.py - mbed RPC interface for Python
#
##Copyright (c) 2010 ARM Ltd
##
##Permission is hereby granted, free of charge, to any person obtaining a copy
##of this software and associated documentation files (the "Software"), to deal
##in the Software without restriction, including without limitation the rights
##to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
##copies of the Software, and to permit persons to whom the Software is
##furnished to do so, subject to the following conditions:
## 
##The above copyright notice and this permission notice shall be included in
##all copies or substantial portions of the Software.
## 
##THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
##IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
##FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
##AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
##LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
##OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
##THE SOFTWARE.
#
#Example:
#> from mbedRPC import*
#> mbed = SerialRPC("COM5",9600);
#> myled = DigitalOut(mbed, LED1);
#> myled.write(1)
#> 

import serial, urllib2, time


class pin():
    def __init__(self, id):
        self.name = id

LED1 = pin("LED1")
LED2 = pin("LED2")
LED3 = pin("LED3")
LED4 = pin("LED4")

p5 = pin("p5")
p6 = pin("p6")
p7 = pin("p7")
p8 = pin("p8")
p9 = pin("p9")
p10 = pin("p10")
p11 = pin("p11")
p12 = pin("p12")
p13 = pin("p13")
p14 = pin("p14")
p15 = pin("p15")
p16 = pin("p16")
p17 = pin("p17")
p18 = pin("p18")
p19 = pin("p19")
p20 = pin("p20")
p21 = pin("p21")
p22 = pin("p22")
p23 = pin("p23")
p24 = pin("p24")
p25 = pin("p25")
p26 = pin("p26")
p27 = pin("p27")
p28 = pin("p28")
p29 = pin("p29")
p30 = pin("p30")


#mbed super class
class mbed:
    def __init__(self):
            print("This will work as a demo but no transport mechanism has been selected")
    
    def rpc(self, name, method, args):
            print("Superclass method not overridden")

#Transport mechanisms, derived from mbed

class SerialRPC(mbed):
    def __init__(self, port, baud=9600, reset=True, debug=False):
        self.ser = serial.Serial(port)
        self.ser.setBaudrate(baud)
        self.ser.flushInput()
        self.ser.flushOutput()
        self.debug = debug
        if reset:
            if debug:
                print "Reset mbed"
            self.ser.sendBreak()
            time.sleep(2)
    
    def rpc(self, name, method, args):
        request = "/" + name + "/" + method + " " + " ".join(args)
        if self.debug:
            print "[RPC::TX] %s" % request
        self.ser.write(request + "\n")
        
        while True:
            response = self.ser.readline().strip()
            if self.debug:
                print "[RPC::RX] %s" % response
            
            # Ignore comments
            if not response.startswith('#'): break
        return response


class HTTPRPC(mbed):
    def __init__(self, ip):
        self.host = "http://" + ip
    
    def rpc(self, name, method, args):
        response = urllib2.urlopen(self.host + "/rpc/" + name + "/" + method + "," + ",".join(args))
        return response.read().strip()


#mbed Interfaces

class DigitalOut():
    def __init__(self, this_mbed , mpin):
        self.mbed = this_mbed
        if isinstance(mpin, str):
            self.name = mpin
        elif isinstance(mpin, pin):
            self.name = self.mbed.rpc("DigitalOut", "new", [mpin.name])
     
    def __del__(self):
        r = self.mbed.rpc(self.name, "delete", [])
    
    def write(self, value):
        r = self.mbed.rpc(self.name, "write", [str(value)])
    
    def read(self):
        r = self.mbed.rpc(self.name, "read", [])
        return int(r)


class AnalogIn():
    def __init__(self, this_mbed , mpin):
        self.mbed = this_mbed
        if isinstance(mpin, str):
            self.name = mpin
        elif isinstance(mpin, pin):
            self.name = self.mbed.rpc("AnalogIn", "new", [mpin.name])
    
    def __del__(self):
        r = self.mbed.rpc(self.name, "delete", [])
    
    def read(self):
        r = self.mbed.rpc(self.name, "read", [])
        return float(r)
    
    def read_u16(self):
        r = self.mbed.rpc(self.name, "read_u16", [])
        return int(r)


class AnalogOut():
    def __init__(self, this_mbed , mpin):
        self.mbed = this_mbed
        if isinstance(mpin, str):
            self.name = mpin
        elif isinstance(mpin, pin):
            self.name = self.mbed.rpc("AnalogOut", "new", [mpin.name])
    
    def __del__(self):
        r = self.mbed.rpc(self.name, "delete", [])
    
    def write(self, value):
        r = self.mbed.rpc(self.name, "write", [str(value)])
    
    def write_u16(self, value):
        r = self.mbed.rpc(self.name, "write_u16", [str(value)])
    
    def read(self):
        r = self.mbed.rpc(self.name, "read", [])
        return float(r)


class DigitalIn():
    def __init__(self, this_mbed , mpin):
        self.mbed = this_mbed
        if isinstance(mpin, str):
            self.name = mpin
        elif isinstance(mpin, pin):
            self.name = self.mbed.rpc("DigitalIn", "new", [mpin.name])
    
    def __del__(self):
        r = self.mbed.rpc(self.name, "delete", [])
    
    def read(self):
        r = self.mbed.rpc(self.name, "read", [])
        return int(r)


class PwmOut():
    def __init__(self, this_mbed , mpin):
        self.mbed = this_mbed
        if isinstance(mpin, str):
            self.name = mpin
        elif isinstance(mpin, pin):
            self.name = self.mbed.rpc("PwmOut", "new", [mpin.name])
    
    def __del__(self):
        r = self.mbed.rpc(self.name, "delete", [])
    
    def write(self, value):
        r = self.mbed.rpc(self.name, "write", [str(value)])
    
    def read(self):
        r = self.mbed.rpc(self.name, "read", [])
        return float(r)
    
    def period(self, value):
        r = self.mbed.rpc(self.name, "period", [str(value)])
        
    def period_ms(self, value):
        r = self.mbed.rpc(self.name, "period_ms", [str(value)])
    
    def period_us(self, value):
        r = self.mbed.rpc(self.name, "period_us", [str(value)])
        
    def puslewidth(self, value):
        r = self.mbed.rpc(self.name, "pulsewidth", [str(value)])
    
    def puslewidth_ms(self, value):
        r = self.mbed.rpc(self.name, "pulsewidth_ms", [str(value)])

    def puslewidth_us(self, value):
        r = self.mbed.rpc(self.name, "pulsewidth_us", [str(value)])


class Serial():
    def __init__(self, this_mbed , tx, rx = ""):
        self.mbed = this_mbed
        if isinstance(tx, str):
            self.name = mpin
        elif isinstance(mpin, pin):
            self.name = self.mbed.rpc("Serial", "new", [tx.name, rx.name])
    
    def __del__(self):
        r = self.mbed.rpc(self.name, "delete", [])
    
    def putc(self, value):
        r = self.mbed.rpc(self.name, "putc", [str(value)])
    
    def puts(self, value):
        r = self.mbed.rpc(self.name, "puts", [ "\"" + str(value) + "\""])
    
    def getc(self):
        r = self.mbed.rpc(self.name, "getc", [])
        return int(r)


class RPCFunction():
    def __init__(self, this_mbed , name):
        self.mbed = this_mbed
        if isinstance(name, str):
            self.name = name
    
    def __del__(self):
        r = self.mbed.rpc(self.name, "delete", [])
    
    def read(self):
        r = self.mbed.rpc(self.name, "read", [])
        return int(r)
    
    def run(self, input):
        r = self.mbed.rpc(self.name, "run", [input])
        return r


class RPCVariable():
    def __init__(self, this_mbed , name):
        self.mbed = this_mbed
        if isinstance(name, str):
            self.name = name
    
    def __del__(self):
        r = self.mbed.rpc(self.name, "delete", [])
    
    def write(self, value):
        self.mbed.rpc(self.name, "write", [str(value)])
    
    def read(self):
        r = self.mbed.rpc(self.name, "read", [])
        return r


def wait(s):
    time.sleep(s)
