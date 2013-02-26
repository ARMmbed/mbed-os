from workspace_tools.paths import *
from workspace_tools.data.support import *

TEST_CMSIS_LIB = join(TEST_DIR, "cmsis", "lib")
TEST_MBED_LIB = join(TEST_DIR, "mbed", "env")

PERIPHERALS = join(TEST_DIR, "peripherals")
BENCHMARKS_DIR = join(TEST_DIR, "benchmarks")
SD = join(TEST_DIR, "sd")

"""
Wiring:
  * Ground:
    * LPC1*: p1
    * KL25Z: GND
  
  * Vout
    * LPC1*: p40
    * KL25Z: P3V3
  
  * TMP102 (I2C):
      * LPC1*: (SDA=p28 , SCL=p27)
      * KL25Z: (SDA=PTC9, SCL=PTC8)
  
  * digital_loop (Digital(In|Out|InOut), InterruptIn):
      * LPC1*: (p5   <-> p25 )
      * KL25Z: (PTA1 <-> PTC7)
  
  * port_loop (Port(In|Out|InOut)):
      * LPC1*: (p5   <-> p25 ), (p6   <-> p26 )
      * KL25Z: (PTA1 <-> PTC7), (PTA2 <-> PTC0)
  
  * analog_loop (AnalogIn, AnalogOut):
      * LPC1*: (p17   <-> p18 )
      * KL25Z: (PTE30 <-> PTC2)
  
  * SD (SPI):
      * LPC1*: (mosi=p11 , miso=p12 , sclk=p13 , cs=p14 )
      * KL25Z: (mosi=PTD2, miso=PTD3, sclk=PTD1, cs=PTD0)
      
  * MMA7660 (I2C):
      * LPC1*: (SDA=p28 , SCL=p27)
"""
TESTS = [
    # Automated MBED tests
    {
        "id": "MBED_A1", "description": "MBED: Basic",
        "source_dir": join(TEST_DIR, "mbed", "basic"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
    },
    {
        "id": "MBED_A2", "description": "MBED: semihost file system",
        "source_dir": join(TEST_DIR, "mbed", "file"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "mcu": ["LPC1768", "LPC2368", "LPC11U24"]
    },
    {
        "id": "MBED_A3", "description": "MBED: C++ STL",
        "source_dir": join(TEST_DIR, "mbed", "stl"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
    },
    {
        "id": "MBED_A4", "description": "MBED: I2C TMP102",
        "source_dir": join(TEST_DIR, "mbed", "i2c_TMP102"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, join(PERIPHERALS, 'TMP102')],
        "automated": True,
        "peripherals": ["TMP102"]
    },
    {
        "id": "MBED_A5", "description": "MBED: DigitalIn DigitalOut",
        "source_dir": join(TEST_DIR, "mbed", "digitalin_digitalout"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "peripherals": ["digital_loop"]
    },
    {
        "id": "MBED_A6", "description": "MBED: DigitalInOut",
        "source_dir": join(TEST_DIR, "mbed", "digitalinout"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "peripherals": ["digital_loop"]
    },
    {
        "id": "MBED_A7", "description": "MBED: InterruptIn",
        "source_dir": join(TEST_DIR, "mbed", "interruptin"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "peripherals": ["digital_loop"]
    },
    {
        "id": "MBED_A8", "description": "MBED: Analog",
        "source_dir": join(TEST_DIR, "mbed", "analog"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "peripherals": ["analog_loop"],
        "mcu": ["LPC1768", "LPC2368", "KL25Z"]
    },
    {
        "id": "MBED_A9", "description": "MBED: Serial Echo at 115200",
        "source_dir": join(TEST_DIR, "mbed", "echo"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "host_test": "echo"
    },
    {
        "id": "MBED_A10", "description": "MBED: PortOut PortIn",
        "source_dir": join(TEST_DIR, "mbed", "portout_portin"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "peripherals": ["port_loop"],
        "supported": DEFAULT_SUPPORT,
        "automated": True,
    },
    {
        "id": "MBED_A11", "description": "MBED: PortInOut",
        "source_dir": join(TEST_DIR, "mbed", "portinout"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "peripherals": ["port_loop"],
        "supported": DEFAULT_SUPPORT,
        "automated": True,
    },
    {
        "id": "MBED_A12", "description": "MBED: SD File System",
        "source_dir": join(TEST_DIR, "mbed", "sd"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, SD_FS, FAT_FS],
        "automated": True,
        "peripherals": ["SD"]
    },
    {
        "id": "MBED_A13", "description": "MBED: I2C MMA7660",
        "source_dir": join(TEST_DIR, "mbed", "i2c_MMA7660"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, join(PERIPHERALS, 'MMA7660')],
        "automated": True,
        "peripherals": ["MMA7660"]
    },
    
    # Not automated MBED tests
    {
        "id": "MBED_1", "description": "MBED: I2C SRF08",
        "source_dir": join(TEST_DIR, "mbed", "i2c_SRF08"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, join(PERIPHERALS, 'SRF08')],
        "peripherals": ["SRF08"]
    },
    {
        "id": "MBED_2", "description": "MBED: stdio",
        "source_dir": join(TEST_DIR, "mbed", "stdio"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_3", "description": "MBED: PortOut",
        "source_dir": join(TEST_DIR, "mbed", "portout"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_4", "description": "MBED: Sleep",
        "source_dir": join(TEST_DIR, "mbed", "sleep"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "supported": LPC11U24_SUPPORT,
        "duration": 30
    },
    {
        "id": "MBED_5", "description": "MBED: PWM",
        "source_dir": join(TEST_DIR, "mbed", "pwm"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
    },
    {
        "id": "MBED_6", "description": "MBED: SW Reset",
        "source_dir": join(TEST_DIR, "mbed", "reset"),
        "dependencies": [MBED_LIBRARIES],
        "duration": 15
    },
    {
        "id": "MBED_7", "description": "MBED: stdio  benchmark",
        "source_dir": join(TEST_DIR, "mbed", "stdio_benchmark"),
        "dependencies": [MBED_LIBRARIES],
        "duration": 40
    },
    {
        "id": "MBED_8", "description": "MBED: SPI",
        "source_dir": join(TEST_DIR, "mbed", "spi"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_9", "description": "MBED: Sleep Timeout",
        "source_dir": join(TEST_DIR, "mbed", "sleep_timeout"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_10", "description": "MBED: Hello World",
        "source_dir": join(TEST_DIR, "mbed", "hello"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_11", "description": "MBED: Ticker",
        "source_dir": join(TEST_DIR, "mbed", "ticker"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_12", "description": "MBED: C++",
        "source_dir": join(TEST_DIR, "mbed", "cpp"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
    },
    {
        "id": "MBED_13", "description": "MBED: Heap & Stack",
        "source_dir": join(TEST_DIR, "mbed", "heap_and_stack"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
    },
    {
        "id": "MBED_14", "description": "MBED: Serial Interrupt",
        "source_dir": join(TEST_DIR, "mbed", "serial_interrupt"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
    },
    {
        "id": "MBED_15", "description": "MBED: RPC",
        "source_dir": join(TEST_DIR, "mbed", "rpc"),
        "dependencies": [MBED_LIBRARIES, join(LIB_DIR, "mbed", "rpc")],
        "host_test": "rpc",
    },
    {
        "id": "MBED_16", "description": "MBED: RTC",
        "source_dir": join(TEST_DIR, "mbed", "rtc"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_17", "description": "MBED: MODSERIAL",
        "source_dir": join(TEST_DIR, "mbed", "modserial"),
        "dependencies": [MBED_LIBRARIES, join(TEST_DIR, "MODSERIAL")],
    },
    {
        "id": "MBED_18", "description": "MBED: Serial Interrupt 2",
        "source_dir": join(TEST_DIR, "mbed", "serial_interrupt_2"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_19", "description": "MBED: Local FS Directory",
        "source_dir": join(TEST_DIR, "mbed", "dir"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_20", "description": "MBED: SD FS Directory",
        "source_dir": join(TEST_DIR, "mbed", "dir_sd"),
        "dependencies": [MBED_LIBRARIES, SD_FS, FAT_FS],
        "peripherals": ["SD"]
    },
    {
        "id": "MBED_21", "description": "MBED: InterruptIn 2",
        "source_dir": join(TEST_DIR, "mbed", "interruptin_2"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_22", "description": "MBED: freopen Stream",
        "source_dir": join(TEST_DIR, "mbed", "freopen"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_23", "description": "MBED: Semihost",
        "source_dir": join(TEST_DIR, "mbed", "semihost"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
    },
    # CMSIS RTOS tests
    {
        "id": "CMSIS_RTOS_1", "description": "CMSIS RTOS: Basic",
        "source_dir": join(TEST_DIR, "rtos", "cmsis", "basic"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES],
    },
    {
        "id": "CMSIS_RTOS_2", "description": "CMSIS RTOS: Mutex",
        "source_dir": join(TEST_DIR, "rtos", "cmsis", "mutex"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES],
        "duration": 20
    },
    {
        "id": "CMSIS_RTOS_3", "description": "CMSIS RTOS: Semaphore",
        "source_dir": join(TEST_DIR, "rtos", "cmsis", "semaphore"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES],
        "duration": 20
    },
    {
        "id": "CMSIS_RTOS_4", "description": "CMSIS RTOS: Signals",
        "source_dir": join(TEST_DIR, "rtos", "cmsis", "signals"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES],
    },
    {
        "id": "CMSIS_RTOS_5", "description": "CMSIS RTOS: Queue",
        "source_dir": join(TEST_DIR, "rtos", "cmsis", "queue"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES],
        "duration": 20
    },
    {
        "id": "CMSIS_RTOS_6", "description": "CMSIS RTOS: Mail",
        "source_dir": join(TEST_DIR, "rtos", "cmsis", "mail"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES],
        "duration": 20
    },
    {
        "id": "CMSIS_RTOS_7", "description": "CMSIS RTOS: Timer",
        "source_dir": join(TEST_DIR, "rtos", "cmsis", "timer"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES],
    },
    {
        "id": "CMSIS_RTOS_8", "description": "CMSIS RTOS: ISR",
        "source_dir": join(TEST_DIR, "rtos", "cmsis", "isr"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES],
    },
    
    # mbed RTOS tests
    {
        "id": "RTOS_1", "description": "RTOS: Basic",
        "source_dir": join(TEST_DIR, "rtos", "mbed", "basic"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES],
    },
    {
        "id": "RTOS_2", "description": "RTOS: Mutex",
        "source_dir": join(TEST_DIR, "rtos", "mbed", "mutex"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES],
        "duration": 20
    },
    {
        "id": "RTOS_3", "description": "RTOS: Semaphore",
        "source_dir": join(TEST_DIR, "rtos", "mbed", "semaphore"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES],
    },
    {
        "id": "RTOS_4", "description": "RTOS: Signals",
        "source_dir": join(TEST_DIR, "rtos", "mbed", "signals"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES],
    },
    {
        "id": "RTOS_5", "description": "RTOS: Queue",
        "source_dir": join(TEST_DIR, "rtos", "mbed", "queue"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES],
    },
    {
        "id": "RTOS_6", "description": "RTOS: Mail",
        "source_dir": join(TEST_DIR, "rtos", "mbed", "mail"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES],
    },
    {
        "id": "RTOS_7", "description": "RTOS: Timer",
        "source_dir": join(TEST_DIR, "rtos", "mbed", "timer"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES],
    },
    {
        "id": "RTOS_8", "description": "RTOS: ISR",
        "source_dir": join(TEST_DIR, "rtos", "mbed", "isr"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES],
    },
    {
        "id": "RTOS_9", "description": "RTOS: File",
        "source_dir": join(TEST_DIR, "rtos", "mbed", "file"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES, TEST_MBED_LIB, SD_FS, FAT_FS],
    },
    
    # Networking Tests
    {
        "id": "NET_1", "description": "NET: TCP client hello world",
        "source_dir": join(TEST_DIR, "net", "helloworld", "tcpclient"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES, ETH_LIBRARY],
        "supported": CORTEX_ARM_SUPPORT,
    },
    {
        "id": "NET_2", "description": "NET: UDP client hello world",
        "source_dir": join(TEST_DIR, "net", "helloworld", "udpclient"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES, ETH_LIBRARY],
        "supported": CORTEX_ARM_SUPPORT,
    },
    {
        "id": "NET_3", "description": "NET: TCP echo server",
        "source_dir": join(TEST_DIR, "net", "echo", "tcp_server"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES, ETH_LIBRARY],
        "supported": CORTEX_ARM_SUPPORT,
    },
    {
        "id": "NET_4", "description": "NET: TCP echo client",
        "source_dir": join(TEST_DIR, "net", "echo", "tcp_client"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES, ETH_LIBRARY],
        "supported": CORTEX_ARM_SUPPORT,
    },
    {
        "id": "NET_5", "description": "NET: UDP echo server",
        "source_dir": join(TEST_DIR, "net", "echo", "udp_server"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES, ETH_LIBRARY],
        "supported": CORTEX_ARM_SUPPORT,
    },
    {
        "id": "NET_6", "description": "NET: UDP echo client",
        "source_dir": join(TEST_DIR, "net", "echo", "udp_client"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES, ETH_LIBRARY],
        "supported": CORTEX_ARM_SUPPORT,
    },
    {
        "id": "NET_7", "description": "NET: HTTP client",
        "source_dir": join(TEST_DIR, "net", "protocols", "HTTPClient_HelloWorld"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES, ETH_LIBRARY],
        "supported": CORTEX_ARM_SUPPORT,
    },
    {
        "id": "NET_8", "description": "NET: NTP client",
        "source_dir": join(TEST_DIR, "net", "protocols", "NTPClient_HelloWorld"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES, ETH_LIBRARY],
        "supported": CORTEX_ARM_SUPPORT,
    },
    
    # Vodafone tests
    {
        "id": "VF_1", "description": "Vodafone: HTTP client",
        "source_dir": join(TEST_DIR, "net", "vodafone", "HTTPClient_HelloWorld"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES, VODAFONE_LIBRARY, TEST_MBED_LIB],
        "supported": CORTEX_ARM_SUPPORT,
        "automated": False,
    },
    {
        "id": "VF_2", "description": "Vodafone: USSD & SMS Test",
        "source_dir": join(TEST_DIR, "net", "vodafone", "USSD_SMS_HelloWorld"),
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES, VODAFONE_LIBRARY, TEST_MBED_LIB],
        "supported": CORTEX_ARM_SUPPORT,
        "automated": False,
    },
    
    # USB Tests
    {
        "id": "USB_1", "description": "USB: Mouse",
        "source_dir": join(TEST_DIR, "usb", "device", "basic"),
        "dependencies": [MBED_LIBRARIES, USB_LIBRARIES],
        "supported": CORTEX_ARM_SUPPORT,
    },
    {
        "id": "USB_2", "description": "USB: Keyboard",
        "source_dir": join(TEST_DIR, "usb", "device", "keyboard"),
        "dependencies": [MBED_LIBRARIES, USB_LIBRARIES],
        "supported": CORTEX_ARM_SUPPORT,
    },
    {
        "id": "USB_3", "description": "USB: Mouse_Keyboard",
        "source_dir": join(TEST_DIR, "usb", "device", "keyboard"),
        "dependencies": [MBED_LIBRARIES, USB_LIBRARIES],
        "supported": CORTEX_ARM_SUPPORT,
    },
    {
        "id": "USB_4", "description": "USB: Serial Port",
        "source_dir": join(TEST_DIR, "usb", "device", "serial"),
        "dependencies": [MBED_LIBRARIES, USB_LIBRARIES],
        "supported": CORTEX_ARM_SUPPORT,
    },
    {
        "id": "USB_5", "description": "USB: Generic HID",
        "source_dir": join(TEST_DIR, "usb", "device", "raw_hid"),
        "dependencies": [MBED_LIBRARIES, USB_LIBRARIES],
        "supported": CORTEX_ARM_SUPPORT,
    },
    {
        "id": "USB_6", "description": "USB: MIDI",
        "source_dir": join(TEST_DIR, "usb", "device", "midi"),
        "dependencies": [MBED_LIBRARIES, USB_LIBRARIES],
        "supported": CORTEX_ARM_SUPPORT,
    },
    {
        "id": "USB_7", "description": "USB: AUDIO",
        "source_dir": join(TEST_DIR, "usb", "device", "audio"),
        "dependencies": [MBED_LIBRARIES, USB_LIBRARIES],
        "supported": CORTEX_ARM_SUPPORT,
    },
    
    # CMSIS DSP
    {
        "id": "CMSIS_DSP_1", "description": "CMSIS DSP: FIR",
        "source_dir": join(TEST_DIR, "dsp", "cmsis", "fir_f32"),
        "dependencies": [MBED_LIBRARIES, DSP_LIBRARIES],
        "supported": CORTEX_ARM_SUPPORT,
    },
    
    # mbed DSP
    {
        "id": "DSP_1", "description": "DSP: FIR",
        "source_dir": join(TEST_DIR, "dsp", "mbed", "fir_f32"),
        "dependencies": [MBED_LIBRARIES, DSP_LIBRARIES],
        "supported": CORTEX_ARM_SUPPORT,
    },
    
    # Examples
    {
        "id": "EXAMPLE_1", "description": "/dev/null",
        "source_dir": join(TEST_DIR, "mbed", "dev_null"),
        "dependencies": [MBED_LIBRARIES],
    },
]

class Test:
    DEFAULTS = {
        'dependencies': None,
        'duration': 10,
        'host_test': 'host_test',
        'automated': False,
        'peripherals': None,
        'extra_files': None
    }
    def __init__(self, n):
        self.n = n
        self.__dict__.update(Test.DEFAULTS)
        self.__dict__.update(TESTS[n])
    
    def is_supported(self, target, toolchain):
        if not hasattr(self, 'supported'):
            return True
        return (target in self.supported) and (toolchain in self.supported[target])
    
    def get_description(self):
        if hasattr(self, 'description'):
            return self.description
        else:
            return self.id

TEST_MAP = dict([(test['id'], Test(i)) for i, test in enumerate(TESTS)])
