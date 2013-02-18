from host_test import Test, Simple
from sys import stdout

class NETTest(Simple):
    def __init__(self):
        Test.__init__(self)
        self.mbed.init_serial(115200)
        self.mbed.reset()

if __name__ == '__main__':
    NETTest().run()
