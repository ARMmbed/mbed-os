from host_test import Test


class EchoTest(Test):
    def __init__(self):
        Test.__init__(self)
        self.mbed.init_serial(115200)
        self.mbed.reset()
    
    def test(self):
        self.mbed.flush()
        self.notify("Starting the ECHO test")
        TEST="longer serial test"
        check = True
        for i in range(1, 100):
            self.mbed.serial.write(TEST + "\n")
            l = self.mbed.serial.readline().strip()
            if not l: continue
            
            if l != TEST:
                check = False
                self.notify('"%s" != "%s"' % (l, TEST))
            else:
                if (i % 10) == 0:
                    self.notify('.')
        
        return check


if __name__ == '__main__':
    EchoTest().run()
