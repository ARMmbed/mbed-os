mbed port to NXP LPC43xx
========================
Updated: 06/24/13

The NXP LPC43xx microcontrollers are the first to include multiple Cortex-M
cores in a single microcontroller package. This port allows mbed developers
to take advantage of the LPC43xx in their application using APIs that they
are familiar with. Some of the key features of the LPC43xx include:

* Dual core ARM Cortex-M4/M0 both capable of up to 204 MHz
* Up to 264 KB SRAM, 1 MB internal flash
* Two High-speed USB 2.0 interfaces
* Ethernet MAC
* LCD interface
* Quad-SPI Flash Interface (SPIFI)
* State Configurable Timer (SCT)
* Serial GPIO (SGPIO)
* Up to 164 GPIO

The NXP LPC18xx is a single core Cortex-M3 implementation that is compatible
with the LPC43XX for cost-sensitive applications not requiring multiple cores.

mbed port to the LPC43XX - Micromint USA <support@micromint.com>

Compatibility
-------------
* This port has been tested with the following boards:
    Board                    MCU        RAM/Flash
    Micromint Bambino 200    LPC4330    264K SRAM/4 MB SPIFI flash

* Ethernet, USB and microSD filesystem drivers will be available when the
  Bambino 200E is released.

* This port uses offline toolchains. Development and testing has been done
  mainly with the Keil MDK 4.70. Some testing has been done with IAR 6.5.
  Eventually Keil, IAR and GCC CodeRed will be supported.

* CMSIS-DAP debugging is not currently implemented. To debug use a JTAG.
  The NXP DFU tool can be used for flash programming.

* This port should support NXP LPC43XX and LPC18XX variants with a single
  codebase. The core declaration specifies the binaries to be built:
    mbed define      CMSIS define  MCU Target
    __CORTEX_M4      CORE_M4       LPC43xx Cortex-M4
    __CORTEX_M0      CORE_M0       LPC43xx Cortex-M0
    __CORTEX_M3      CORE_M3       LPC18xx Cortex-M3
  These MCUs all share the peripheral IP, common driver code is feasible.
  Yet each variant can have different memory segments, peripherals, etc.
  Plus, each board design can integrate different external peripherals
  or interfaces. A future release of the mbed SDK and its build tools will
  support specifying the target board when building binaries. At this time
  building binaries for different targets requires an external project or
  Makefile.

* No testing has been done with LPC18xx hardware. At the very least supporting
  the LPC18xx would require different compiler flags, additional CMSIS core_cm3
  code as well as minor driver code changes.

Notes
-----
* On the LPC43xx the hardware pin name and the GPIO pin name are not the same,
  requiring different offsets for the SCU and GPIO registers. To simplify logic
  the pin identifier encodes the offsets. Macros are used for decoding.
  For example, P6_11 corresponds to GPIO3[7] and is encoded/decoded as follows:

    P6_11 = MBED_PIN(0x06, 11, 3, 7) = 0x032C0067

    MBED_SCU_REG(P6_11)  = 0x4008632C      MBED_GPIO_PORT(P6_11) = 3
    MBED_GPIO_REG(P6_11) = 0x400F4000      MBED_GPIO_PIN(P6_11)  = 7

* The LPC43xx implements GPIO pin and group interrupts. Any pin in the 8 32-bit
  GPIO ports can interrupt (LPC4350 supports up to 164). On group interrupts a
  pin can only interrupt on the rising or falling edge, not both as required
  by the mbed InterruptIn class. Also, group interrupts can't be cleared
  individually. This implementation uses pin interrupts (8 on M4/M3, 1 on M0).
  A future implementation may provide group interrupt support.
