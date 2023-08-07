"""
This script can be used to reimport a newer version of the RPi Pico SDK.

Since the SDK is quite large, we only want to copy in specific files, not the whole
thing.  Additionally, certain identifiers need to be renamed in order to not conflict with the
Mbed ones.  This script takes care of that too, and provides a one-click way to do the import.
"""

import pathlib
import shutil
import sys
from typing import List, Tuple

this_script_dir = pathlib.Path(__file__).resolve().parent

# List of identifiers to rename b/c they clash with Mbed symbols
IDENTIFIERS_TO_RENAME: List[Tuple[bytes, bytes]] = [
    (b"gpio_irq_handler", b"pico_sdk_gpio_irq_handler"),
    (b"gpio_init", b"pico_sdk_gpio_init"),
    (b"i2c_init", b"pico_sdk_i2c_init"),
    (b"rtc_init", b"pico_sdk_rtc_init"),
    (b"spi_init", b"pico_sdk_spi_init"),

    # Rename IRQ handlers to the CMSIS exception names.
    # Pico SDK does this with macros, but easier to just
    # do it here.
    # Based on cmsis/include/rename_exceptions.h.
    (b"isr_nmi", b"NMI_Handler"),
    (b"isr_hardfault", b"HardFault_Handler"),
    (b"isr_svcall", b"SVC_Handler"),
    (b"isr_pendsv", b"PendSV_Handler"),
    (b"isr_systick", b"SysTick_Handler"),
    (b"isr_irq0", b"TIMER_IRQ_0_Handler"),
    (b"isr_irq1", b"TIMER_IRQ_1_Handler"),
    (b"isr_irq2", b"TIMER_IRQ_2_Handler"),
    (b"isr_irq3", b"TIMER_IRQ_3_Handler"),
    (b"isr_irq4", b"PWM_IRQ_WRAP_Handler"),
    (b"isr_irq5", b"USBCTRL_IRQ_Handler"),
    (b"isr_irq6", b"XIP_IRQ_Handler"),
    (b"isr_irq7", b"PIO0_IRQ_0_Handler"),
    (b"isr_irq8", b"PIO0_IRQ_1_Handler"),
    (b"isr_irq9", b"PIO1_IRQ_0_Handler"),
    (b"isr_irq10", b"PIO1_IRQ_1_Handler"),
    (b"isr_irq11", b"DMA_IRQ_0_Handler"),
    (b"isr_irq12", b"DMA_IRQ_1_Handler"),
    (b"isr_irq13", b"IO_IRQ_BANK0_Handler"),
    (b"isr_irq14", b"IO_IRQ_QSPI_Handler"),
    (b"isr_irq15", b"SIO_IRQ_PROC0_Handler"),
    (b"isr_irq16", b"SIO_IRQ_PROC1_Handler"),
    (b"isr_irq17", b"CLOCKS_IRQ_Handler"),
    (b"isr_irq18", b"SPI0_IRQ_Handler"),
    (b"isr_irq19", b"SPI1_IRQ_Handler"),
    (b"isr_irq20", b"UART0_IRQ_Handler"),
    (b"isr_irq21", b"UART1_IRQ_Handler"),
    (b"isr_irq22", b"ADC_IRQ_FIFO_Handler"),
    (b"isr_irq23", b"I2C0_IRQ_Handler"),
    (b"isr_irq24", b"I2C1_IRQ_Handler"),
    (b"isr_irq25", b"RTC_IRQ_Handler"),
]

# List of files and directories which need to be copied into Mbed.
# From directories, only .c, .h, and .S files will be copied, nothing else.
FILES_DIRS_TO_COPY: List[pathlib.Path] = [
    pathlib.Path("LICENSE.txt"),
    pathlib.Path("pico_sdk_version.cmake"),
    pathlib.Path("src") / "rp2_common" / "hardware_base",
    pathlib.Path("src") / "rp2_common" / "hardware_adc",
    pathlib.Path("src") / "rp2_common" / "hardware_resets",
    pathlib.Path("src") / "rp2_common" / "hardware_pwm",
    pathlib.Path("src") / "rp2_common" / "hardware_flash",
    pathlib.Path("src") / "rp2_common" / "hardware_uart",
    pathlib.Path("src") / "rp2_common" / "hardware_spi",
    pathlib.Path("src") / "rp2_common" / "hardware_i2c",
    pathlib.Path("src") / "rp2_common" / "hardware_gpio",
    pathlib.Path("src") / "rp2_common" / "hardware_xosc",
    pathlib.Path("src") / "rp2_common" / "hardware_irq",
    pathlib.Path("src") / "rp2_common" / "hardware_pll",
    pathlib.Path("src") / "rp2_common" / "hardware_watchdog",
    pathlib.Path("src") / "rp2_common" / "hardware_clocks",
    pathlib.Path("src") / "rp2_common" / "hardware_claim",
    pathlib.Path("src") / "rp2_common" / "hardware_timer",
    pathlib.Path("src") / "rp2_common" / "hardware_sync",
    pathlib.Path("src") / "rp2_common" / "hardware_rtc",
    pathlib.Path("src") / "rp2_common" / "pico_bootrom",
    pathlib.Path("src") / "rp2_common" / "pico_platform",
    pathlib.Path("src") / "rp2_common" / "cmsis" / "stub",
    pathlib.Path("src") / "common" / "pico_time",
    pathlib.Path("src") / "common" / "pico_sync",
    pathlib.Path("src") / "common" / "pico_base",
    pathlib.Path("src") / "common" / "pico_base" / "include" / "pico" / "version.h.in",
    pathlib.Path("src") / "common" / "pico_binary_info",
    pathlib.Path("src") / "common" / "pico_util",
    pathlib.Path("src") / "rp2_common" / "pico_fix" / "rp2040_usb_device_enumeration",
    pathlib.Path("src") / "rp2040" / "hardware_structs",
    pathlib.Path("src") / "rp2040" / "hardware_regs",
    pathlib.Path("src") / "boards" / "include" / "boards" / "pico.h",
]

if len(sys.argv) != 2:
    print(f"Error: Usage: {sys.argv[0]} <path to pico-sdk to import>")
    exit(1)

sdk_path = pathlib.Path(sys.argv[1]).resolve()
if not sdk_path.is_dir():
    print(f"{sdk_path} is not a valid SDK path!")

# Find all files which need to be copied
files_to_copy: List[pathlib.Path] = []
for file_or_dir in FILES_DIRS_TO_COPY:
    if (sdk_path / file_or_dir).is_file():
        files_to_copy.append((sdk_path / file_or_dir))
    elif (sdk_path / file_or_dir).is_dir():
        files_in_folder = list((sdk_path / file_or_dir).rglob("*.c"))
        files_in_folder.extend((sdk_path / file_or_dir).rglob("*.h"))
        files_in_folder.extend((sdk_path / file_or_dir).rglob("*.S"))

        if len(files_in_folder) == 0:
            print(f"Warning: No copyable files found in directory {file_or_dir}.")

        files_to_copy.extend(files_in_folder)
    else:
        print(f"Error: path {file_or_dir} not found in pico_sdk!")
        exit(1)

# Remove everything in the sdk folder in Mbed
sdk_dest_folder = this_script_dir / "pico-sdk"
if sdk_dest_folder.exists():
    shutil.rmtree(sdk_dest_folder)

# Now start copying things over
for source_file_path in files_to_copy:

    # Load file
    with open(source_file_path, "rb") as source_file:
        source_file_contents = source_file.read()

    # Perform replacements
    for old_identifier, new_identifier in IDENTIFIERS_TO_RENAME:
        source_file_contents = source_file_contents.replace(old_identifier, new_identifier)

    # Figure out new path
    relative_path = source_file_path.relative_to(sdk_path)
    dest_file_path = sdk_dest_folder / relative_path

    # Write new contents
    dest_file_path.parent.mkdir(parents=True, exist_ok=True)
    with open(dest_file_path, "wb") as dest_file:
        dest_file.write(source_file_contents)

    print(f"Copied {relative_path}")