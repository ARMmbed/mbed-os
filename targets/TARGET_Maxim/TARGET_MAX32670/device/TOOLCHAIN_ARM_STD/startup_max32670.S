;*******************************************************************************
; Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
;
; Permission is hereby granted, free of charge, to any person obtaining a
; copy of this software and associated documentation files (the "Software"),
; to deal in the Software without restriction, including without limitation
; the rights to use, copy, modify, merge, publish, distribute, sublicense,
; and/or sell copies of the Software, and to permit persons to whom the
; Software is furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included
; in all copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
; OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
; MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
; IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
; OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
; ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
; OTHER DEALINGS IN THE SOFTWARE.
;
; Except as contained in this notice, the name of Maxim Integrated
; Products, Inc. shall not be used except as stated in the Maxim Integrated
; Products, Inc. Branding Policy.
;
; The mere transfer of this software does not imply any licenses
; of trade secrets, proprietary technology, copyrights, patents,
; trademarks, maskwork rights, or any other form of intellectual
; property whatsoever. Maxim Integrated Products, Inc. retains all
; ownership rights.
;*******************************************************************************

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size
                IMPORT  |Image$$ARM_LIB_STACK$$ZI$$Limit|

__Vectors       DCD     |Image$$ARM_LIB_STACK$$ZI$$Limit| ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; Device-specific Interrupts
                DCD     PF_IRQHandler             ; 0x10  0x0040  16: Power Fail
                DCD     WDT0_IRQHandler           ; 0x11  0x0044  17: Watchdog 0
                DCD     RSV02_IRQHandler          ; 0x12  0x0048  18: USB
                DCD     RTC_IRQHandler            ; 0x13  0x004C  19: RTC
                DCD     TRNG_IRQHandler           ; 0x14  0x0050  20: True Random Number Gener
                DCD     TMR0_IRQHandler           ; 0x15  0x0054  21: Timer 0
                DCD     TMR1_IRQHandler           ; 0x16  0x0058  22: Timer 1
                DCD     TMR2_IRQHandler           ; 0x17  0x005C  23: Timer 2
                DCD     TMR3_IRQHandler           ; 0x18  0x0060  24: Timer 3
                DCD     TMR4_IRQHandler           ; 0x19  0x0064  25: Timer 4
                DCD     TMR5_IRQHandler           ; 0x1A  0x0068  26: Timer 5
                DCD     RSV11_IRQHandler          ; 0x1B  0x006C  27: Reserved
                DCD     RSV12_IRQHandler          ; 0x1C  0x0070  28: Reserved
                DCD     I2C0_IRQHandler           ; 0x1D  0x0074  29: I2C0
                DCD     UART0_IRQHandler          ; 0x1E  0x0078  30: UART 0
                DCD     UART1_IRQHandler          ; 0x1F  0x007C  31: UART 1
                DCD     SPI0_IRQHandler           ; 0x20  0x0080  32: SPI0
                DCD     SPI1_IRQHandler           ; 0x21  0x0084  33: SPI1 
                DCD     SPI2_IRQHandler           ; 0x22  0x0088  34: SPI2 
                DCD     RSV19_IRQHandler          ; 0x23  0x008C  35: Reserved
                DCD     RSV20_IRQHandler          ; 0x24  0x0090  36: Reserved 
                DCD     RSV21_IRQHandler          ; 0x25  0x0094  37: Reserved
                DCD     RSV22_IRQHandler          ; 0x26  0x0098  38: Reserved
                DCD     FLC0_IRQHandler           ; 0x27  0x009C  39: Flash Controller 0
                DCD     GPIO0_IRQHandler          ; 0x28  0x00A0  40: GPIO0
                DCD     GPIO1_IRQHandler          ; 0x29  0x00A4  41: GPIO1
                DCD     RSV26_IRQHandler          ; 0x2A  0x00A8  42: Reserved
                DCD     RSV27_IRQHandler          ; 0x2B  0x00AC  43: Reserved
                DCD     DMA0_IRQHandler           ; 0x2C  0x00B0  44: DMA0
                DCD     DMA1_IRQHandler           ; 0x2D  0x00B4  45: DMA1
                DCD     DMA2_IRQHandler           ; 0x2E  0x00B8  46: DMA2
                DCD     DMA3_IRQHandler           ; 0x2F  0x00BC  47: DMA3
                DCD     RSV32_IRQHandler          ; 0x30  0x00C0  48: Reserved
                DCD     RSV33_IRQHandler          ; 0x31  0x00C4  49: Reserved
                DCD     UART2_IRQHandler          ; 0x32  0x00C8  50: UART 2
                DCD     RSV35_IRQHandler          ; 0x33  0x00CC  51: Reserved
                DCD     I2C1_IRQHandler           ; 0x34  0x00D0  52: I2C1
                DCD     RSV37_IRQHandler          ; 0x35  0x00D4  53: Reserved 
                DCD     RSV38_IRQHandler          ; 0x36  0x00D8  54: Reserved 
                DCD     RSV39_IRQHandler          ; 0x37  0x00DC  55: Reserved 
                DCD     RSV40_IRQHandler          ; 0x38  0x00E0  56: Reserved 
                DCD     RSV41_IRQHandler          ; 0x39  0x00E4  57: Reserved 
                DCD     RSV42_IRQHandler          ; 0x3A  0x00E8  58: Reserved 
                DCD     RSV43_IRQHandler          ; 0x3B  0x00EC  59: Reserved
                DCD     RSV44_IRQHandler          ; 0x3C  0x00F0  60: Reserved
                DCD     RSV45_IRQHandler          ; 0x3D  0x00F4  61: Reserved 
                DCD     RSV46_IRQHandler          ; 0x3E  0x00F8  62: Reserved 
                DCD     RSV47_IRQHandler          ; 0x3F  0x00FC  63: Reserved 
                DCD     RSV48_IRQHandler          ; 0x40  0x0100  64: Reserved 
                DCD     RSV49_IRQHandler          ; 0x41  0x0104  65: Reserved 
                DCD     RSV50_IRQHandler          ; 0x42  0x0108  66: Reserved 
                DCD     RSV51_IRQHandler          ; 0x43  0x010C  67: Reserved
                DCD     RSV52_IRQHandler          ; 0x44  0x0110  68: Reserved
                DCD     RSV53_IRQHandler          ; 0x45  0x0114  69: Reserved 
                DCD     RSV54_IRQHandler          ; 0x46  0x0118  70: Reserved
                DCD     RSV55_IRQHandler          ; 0x47  0x011C  71: Reserved 
                DCD     RSV56_IRQHandler          ; 0x48  0x0120  72: Reserved 
                DCD     WDT1_IRQHandler           ; 0x49  0x0124  73: Watchdog 1 
                DCD     RSV57_IRQHandler          ; 0x4A  0x0128  74: Reserved 
                DCD     RSV58_IRQHandler          ; 0x4B  0x012C  75: Reserved
                DCD     RSV59_IRQHandler          ; 0x4C  0x0130  76: Reserved 
                DCD     RSV61_IRQHandler          ; 0x4D  0x0134  77: Reserved 
                DCD     I2C2_IRQHandler           ; 0x4E  0x0138  78: Reserved 
                DCD     RSV63_IRQHandler          ; 0x4F  0x013C  79: Reserved
                DCD     RSV64_IRQHandler          ; 0x50  0x0140  80: Reserved
                DCD     RSV65_IRQHandler          ; 0x51  0x0144  81: Reserved 
                DCD     RSV66_IRQHandler          ; 0x52  0x0148  82: Reserved 
                DCD     RSV67_IRQHandler          ; 0x53  0x014C  83: Reserved
                DCD     DMA4_IRQHandler           ; 0x54  0x0150  84: DMA4 
                DCD     DMA5_IRQHandler           ; 0x55  0x0154  85: DMA5 
                DCD     DMA6_IRQHandler           ; 0x56  0x0158  86: DMA6 
                DCD     DMA7_IRQHandler           ; 0x57  0x015C  87: DMA7 
                DCD     DMA8_IRQHandler           ; 0x58  0x0160  88: DMA8 
                DCD     DMA9_IRQHandler           ; 0x59  0x0164  89: DMA9 
                DCD     DMA10_IRQHandler          ; 0x5A  0x0168  90: DMA10 
                DCD     DMA11_IRQHandler          ; 0x5B  0x016C  91: DMA11 
                DCD     DMA12_IRQHandler          ; 0x5C  0x0170  92: DMA12 
                DCD     DMA13_IRQHandler          ; 0x5D  0x0174  93: DMA13 
                DCD     DMA14_IRQHandler          ; 0x5E  0x0178  94: DMA14 
                DCD     DMA15_IRQHandler          ; 0x5F  0x017C  95: DMA15 
                DCD     RSV80_IRQHandler          ; 0x60  0x0180  96: Reserved 
                DCD     RSV81_IRQHandler          ; 0x61  0x0184  97: Reserved
                DCD     ECC_IRQHandler            ; 0x62  0x0188  98: Error Correction 
                DCD     RSV83_IRQHandler          ; 0x63  0x018C  99: Reserved 
                DCD     RSV84_IRQHandler          ; 0x64 0x0190   100: Reserved 
                DCD     RSV85_IRQHandler          ; 0x65  0x0194  101: Reserved
                DCD     RSV86_IRQHandler          ; 0x66  0x0198  102: Reserved
                DCD     RSV87_IRQHandler          ; 0x67  0x019C  103: Reserved
                DCD     UART3_IRQHandler          ; 0x68  0x01A0  104: UART 3 
                DCD     RSV89_IRQHandler          ; 0x69  0x01A4  105: Reserved 
                DCD     RSV90_IRQHandler          ; 0x6A  0x01A8  106: UART 5 
                DCD     RSV91_IRQHandler          ; 0x6B  0x01AC  107: Reserved 
                DCD     RSV92_IRQHandler          ; 0x6C  0x01B0  108: Reserved  
                DCD     RSV93_IRQHandler          ; 0x6D  0x01B1  109: Reserved 
                DCD     RSV94_IRQHandler          ; 0x6E  0x01B2  109: Reserved
                DCD     RSV95_IRQHandler          ; 0x6D  0x01B1  109: Reserved 
                DCD     RSV96_IRQHandler          ; 0x6E  0x01B2  109: Reserved
                DCD     AES_IRQHandler            ; 0x6D  0x01B1  109: AES  
                DCD     CRC_IRQHandler            ; 0x6E  0x01B2  109: CRC
                DCD     I2S_IRQHandler            ; 0x6E  0x01B2  109: I2S
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

Reset_Handler   PROC
                EXPORT Reset_Handler                    [WEAK]
                IMPORT __main
                IMPORT SystemInit
                IMPORT PreInit
                LDR     R0, =PreInit
                BLX     R0
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
__SPIN          
                WFI
                BL __SPIN
                ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler             PROC
                        EXPORT NMI_Handler              [WEAK]     
                        B      .
                        ENDP

HardFault_Handler       PROC
                        EXPORT HardFault_Handler        [WEAK]             
                        B      .
                        ENDP

MemManage_Handler       PROC
                        EXPORT MemManage_Handler        [WEAK]
                        B      .
                        ENDP

BusFault_Handler        PROC
                        EXPORT BusFault_Handler         [WEAK]
                        B      .
                        ENDP

UsageFault_Handler      PROC
                        EXPORT UsageFault_Handler       [WEAK]
                        B      .
                        ENDP

SVC_Handler             PROC
                        EXPORT SVC_Handler              [WEAK]
                        B      .
                        ENDP

DebugMon_Handler        PROC
                        EXPORT DebugMon_Handler         [WEAK]
                        B      .
                        ENDP

PendSV_Handler          PROC
                        EXPORT PendSV_Handler           [WEAK]
                        B      .
                        ENDP

SysTick_Handler         PROC
                        EXPORT SysTick_Handler          [WEAK]
                        B      .
                        ENDP

Default_Handler         PROC
        ; MAX32670 interrupts
        EXPORT PF_IRQHandler             [WEAK] ;  0x10  0x0040  16: Power Fail
        EXPORT WDT0_IRQHandler           [WEAK] ;  0x11  0x0044  17: Watchdog 0
        EXPORT RSV02_IRQHandler          [WEAK] ;  0x12  0x0048  18: Reserved
        EXPORT RTC_IRQHandler            [WEAK] ;  0x13  0x004C  19: RTC
        EXPORT TRNG_IRQHandler           [WEAK] ;  0x14  0x0050  20: True Random Number Gener
        EXPORT TMR0_IRQHandler           [WEAK] ;  0x15  0x0054  21: Timer 0
        EXPORT TMR1_IRQHandler           [WEAK] ;  0x16  0x0058  22: Timer 1
        EXPORT TMR2_IRQHandler           [WEAK] ;  0x17  0x005C  23: Timer 2
        EXPORT TMR3_IRQHandler           [WEAK] ;  0x18  0x0060  24: Timer 3
        EXPORT TMR4_IRQHandler           [WEAK] ;  0x19  0x0064  25: Timer 4
        EXPORT TMR5_IRQHandler           [WEAK] ;  0x1A  0x0068  26: Timer 5
        EXPORT RSV11_IRQHandler          [WEAK] ;  0x1B  0x006C  27: Reserved
        EXPORT RSV12_IRQHandler          [WEAK] ;  0x1C  0x0070  28: Reserved
        EXPORT I2C0_IRQHandler           [WEAK] ;  0x1D  0x0074  29: I2C0
        EXPORT UART0_IRQHandler          [WEAK] ;  0x1E  0x0078  30: UART 0
        EXPORT UART1_IRQHandler          [WEAK] ;  0x1F  0x007C  31: UART 1
        EXPORT SPI0_IRQHandler           [WEAK] ;  0x20  0x0080  32: SPI0
        EXPORT SPI1_IRQHandler           [WEAK] ;  0x21  0x0084  33: SPI1
        EXPORT SPI2_IRQHandler           [WEAK] ;  0x22  0x0088  34: SPI2
        EXPORT RSV19_IRQHandler          [WEAK] ;  0x23  0x008C  35: Reserved
        EXPORT RSV20_IRQHandler          [WEAK] ;  0x24  0x0090  36: Reserved
        EXPORT RSV21_IRQHandler          [WEAK] ;  0x25  0x0094  37: Reserved
        EXPORT RSV22_IRQHandler          [WEAK] ;  0x26  0x0098  38: Reserved
        EXPORT FLC0_IRQHandler           [WEAK] ;  0x27  0x009C  39: Flash Controller 0
        EXPORT GPIO0_IRQHandler          [WEAK] ;  0x28  0x00A0  40: GPIO0
        EXPORT GPIO1_IRQHandler          [WEAK] ;  0x29  0x00A4  41: GPIO1
        EXPORT RSV26_IRQHandler          [WEAK] ;  0x2A  0x00A8  42: Reserved
        EXPORT RSV27_IRQHandler          [WEAK] ;  0x2B  0x00AC  43: Reserved
        EXPORT DMA0_IRQHandler           [WEAK] ;  0x2C  0x00B0  44: DMA0
        EXPORT DMA1_IRQHandler           [WEAK] ;  0x2D  0x00B4  45: DMA1
        EXPORT DMA2_IRQHandler           [WEAK] ;  0x2E  0x00B8  46: DMA2
        EXPORT DMA3_IRQHandler           [WEAK] ;  0x2F  0x00BC  47: DMA3
        EXPORT RSV32_IRQHandler          [WEAK] ;  0x30  0x00C0  48: Reserved
        EXPORT RSV33_IRQHandler          [WEAK] ;  0x31  0x00C4  49: Reserved
        EXPORT UART2_IRQHandler          [WEAK] ;  0x32  0x00C8  50: UART 2
        EXPORT RSV35_IRQHandler          [WEAK] ;  0x33  0x00CC  51: Reserved
        EXPORT I2C1_IRQHandler           [WEAK] ;  0x34  0x00D0  52: I2C1
        EXPORT RSV37_IRQHandler          [WEAK] ;  0x35  0x00D4  53: Reserved
        EXPORT RSV38_IRQHandler          [WEAK] ;  0x36  0x00D8  54: Reserved
        EXPORT RSV39_IRQHandler          [WEAK] ;  0x37  0x00DC  55: Reserved
        EXPORT RSV40_IRQHandler          [WEAK] ;  0x38  0x00E0  56: Reserved
        EXPORT RSV41_IRQHandler          [WEAK] ;  0x39  0x00E4  57: Reserved 
        EXPORT RSV42_IRQHandler          [WEAK] ;  0x3A  0x00E8  58: BTLE Reserved
        EXPORT RSV43_IRQHandler          [WEAK] ;  0x3B  0x00EC  59: Reserved
        EXPORT RSV44_IRQHandler          [WEAK] ;  0x3C  0x00F0  60: Reserved
        EXPORT RSV45_IRQHandler          [WEAK] ;  0x3D  0x00F4  61: Reserved
        EXPORT RSV46_IRQHandler          [WEAK] ;  0x3E  0x00F8  62: Reserved
        EXPORT RSV47_IRQHandler          [WEAK] ;  0x3F  0x00FC  63: Reserved
        EXPORT RSV48_IRQHandler          [WEAK] ;  0x40  0x0100  64: Reserved
        EXPORT RSV49_IRQHandler          [WEAK] ;  0x41  0x0104  65: Reserved
        EXPORT RSV50_IRQHandler          [WEAK] ;  0x42  0x0108  66: Reserved
        EXPORT RSV51_IRQHandler          [WEAK] ;  0x43  0x010C  67: BReserved
        EXPORT RSV52_IRQHandler          [WEAK] ;  0x44  0x0110  68: Reserved
        EXPORT RSV53_IRQHandler          [WEAK] ;  0x45  0x0114  69: Reserved
        EXPORT RSV54_IRQHandler          [WEAK] ;  0x46  0x0118  70: Reserved
        EXPORT RSV55_IRQHandler          [WEAK] ;  0x47  0x011C  71: Reserved
        EXPORT RSV56_IRQHandler          [WEAK] ;  0x48  0x0120  72: Reserved
        EXPORT WDT1_IRQHandler           [WEAK] ;  0x49  0x0124  73: Watchdog 1
        EXPORT RSV57_IRQHandler          [WEAK] ;  0x4A  0x0128  74: Reserved
        EXPORT RSV58_IRQHandler          [WEAK] ;  0x4B  0x012C  75: Reserved
        EXPORT RSV59_IRQHandler          [WEAK] ;  0x4C  0x0130  76: Reserved
        EXPORT RSV61_IRQHandler          [WEAK] ;  0x4D  0x0134  77: Reserved
        EXPORT I2C2_IRQHandler           [WEAK] ;  0x4E  0x0138  78: I2C 1
        EXPORT RSV63_IRQHandler          [WEAK] ;  0x4F  0x013C  79: Reserved
        EXPORT RSV64_IRQHandler          [WEAK] ;  0x50  0x0140  80: Reserved
        EXPORT RSV65_IRQHandler          [WEAK] ;  0x51  0x0144  81: Reserved
        EXPORT RSV66_IRQHandler          [WEAK] ;  0x52  0x0148  82: Reserved
        EXPORT RSV67_IRQHandler          [WEAK] ;  0x53  0x014C  83: Reserved
        EXPORT DMA4_IRQHandler           [WEAK] ;  0x54  0x0150  84: DMA4
        EXPORT DMA5_IRQHandler           [WEAK] ;  0x55  0x0154  85: DMA5
        EXPORT DMA6_IRQHandler           [WEAK] ;  0x56  0x0158  86: DMA6
        EXPORT DMA7_IRQHandler           [WEAK] ;  0x57  0x015C  87: DMA7
        EXPORT DMA8_IRQHandler           [WEAK] ;  0x58  0x0160  88: DMA8
        EXPORT DMA9_IRQHandler           [WEAK] ;  0x59  0x0164  89: DMA9
        EXPORT DMA10_IRQHandler          [WEAK] ;  0x5A  0x0168  90: DMA10
        EXPORT DMA11_IRQHandler          [WEAK] ;  0x5B  0x016C  91: DMA11
        EXPORT DMA12_IRQHandler          [WEAK] ;  0x5C  0x0170  92: DMA12
        EXPORT DMA13_IRQHandler          [WEAK] ;  0x5D  0x0174  93: DMA13
        EXPORT DMA14_IRQHandler          [WEAK] ;  0x5E  0x0178  94: DMA14
        EXPORT DMA15_IRQHandler          [WEAK] ;  0x5F  0x017C  95: DMA15
        EXPORT RSV80_IRQHandler          [WEAK] ;  0x60  0x0180  96: Reserved
        EXPORT RSV81_IRQHandler          [WEAK] ;  0x61  0x0184  97: Reserved
        EXPORT ECC_IRQHandler            [WEAK] ;  0x62  0x0188  98: Error Correction 
        EXPORT RSV83_IRQHandler          [WEAK] ;  0x63  0x018C  99: Reserved 
        EXPORT RSV84_IRQHandler          [WEAK] ;  0x64 0x0190  100: Reserved 
        EXPORT RSV85_IRQHandler          [WEAK] ;  0x65  0x0194  101: Reserved 
        EXPORT RSV86_IRQHandler          [WEAK] ;  0x66  0x0198  102: Reserved
        EXPORT RSV87_IRQHandler          [WEAK] ;  0x67  0x019C  103: Reserved
        EXPORT UART3_IRQHandler          [WEAK] ;  0x68  0x01A0  104: UART 3 
        EXPORT RSV89_IRQHandler          [WEAK] ;  0x69  0x01A4  105: Reserved 
        EXPORT RSV90_IRQHandler          [WEAK] ;  0x6A  0x01A8  106: Reserved  
        EXPORT RSV91_IRQHandler          [WEAK] ;  0x6B  0x01AC  107: Reserved  
        EXPORT RSV92_IRQHandler          [WEAK] ;  0x6C  0x01B0  108: Reserved  
        EXPORT RSV93_IRQHandler          [WEAK] ;  0x6D  0x01B4  109: Reserved  
        EXPORT RSV94_IRQHandler          [WEAK] ;  0x6E  0x01B8  110: Reserved 
        EXPORT RSV95_IRQHandler          [WEAK] ;  0x6F  0x01B1  109: Reserved 
        EXPORT RSV96_IRQHandler          [WEAK] ;  0x70  0x01B2  109: Reserved
        EXPORT AES_IRQHandler            [WEAK] ;  0x71  0x01B1  109: AES  
        EXPORT CRC_IRQHandler            [WEAK] ;  0x72  0x01B2  109: CRC
        EXPORT I2S_IRQHandler            [WEAK] ;  0x73  0x01B2  109: I2S  
                      
PF_IRQHandler          ;  0x10  0x0040  16: Power Fail
WDT0_IRQHandler        ;  0x11  0x0044  17: Watchdog 0
RSV02_IRQHandler       ;  0x12  0x0048  18: Reserved
RTC_IRQHandler         ;  0x13  0x004C  19: RTC
TRNG_IRQHandler        ;  0x14  0x0050  20: True Random Number Gener
TMR0_IRQHandler        ;  0x15  0x0054  21: Timer 0
TMR1_IRQHandler        ;  0x16  0x0058  22: Timer 1
TMR2_IRQHandler        ;  0x17  0x005C  23: Timer 2
TMR3_IRQHandler        ;  0x18  0x0060  24: Timer 3
TMR4_IRQHandler        ;  0x19  0x0064  25: Timer 4
TMR5_IRQHandler        ;  0x1A  0x0068  26: Timer 5
RSV11_IRQHandler       ;  0x1B  0x006C  27: Reserved
RSV12_IRQHandler       ;  0x1C  0x0070  28: Reserved
I2C0_IRQHandler        ;  0x1D  0x0074  29: I2C0
UART0_IRQHandler       ;  0x1E  0x0078  30: UART 0
UART1_IRQHandler       ;  0x1F  0x007C  31: UART 1
SPI0_IRQHandler        ;  0x20  0x0080  32: SPI0
SPI1_IRQHandler        ;  0x21  0x0084  33: SPI1
SPI2_IRQHandler        ;  0x22  0x0088  34: SPI2
RSV19_IRQHandler       ;  0x23  0x008C  35: Reserved
RSV20_IRQHandler       ;  0x24  0x0090  36: Reserved
RSV21_IRQHandler       ;  0x25  0x0094  37: Reserved
RSV22_IRQHandler       ;  0x26  0x0098  38: Reserved
FLC0_IRQHandler        ;  0x27  0x009C  39: Flash Controller 0
GPIO0_IRQHandler       ;  0x28  0x00A0  40: GPIO0
GPIO1_IRQHandler       ;  0x29  0x00A4  41: GPIO1
RSV26_IRQHandler       ;  0x2A  0x00A8  42: Reserved
RSV27_IRQHandler       ;  0x2B  0x00AC  43: Reserved
DMA0_IRQHandler        ;  0x2C  0x00B0  44: DMA0
DMA1_IRQHandler        ;  0x2D  0x00B4  45: DMA1
DMA2_IRQHandler        ;  0x2E  0x00B8  46: DMA2
DMA3_IRQHandler        ;  0x2F  0x00BC  47: DMA3
RSV32_IRQHandler       ;  0x30  0x00C0  48: Reserved
RSV33_IRQHandler       ;  0x31  0x00C4  49: Reserved
UART2_IRQHandler       ;  0x32  0x00C8  50: UART 2
RSV35_IRQHandler       ;  0x33  0x00CC  51: Reserved
I2C1_IRQHandler        ;  0x34  0x00D0  52: I2C1
RSV37_IRQHandler       ;  0x35  0x00D4  53: Reserved
RSV38_IRQHandler       ;  0x36  0x00D8  54: Reserved
RSV39_IRQHandler       ;  0x37  0x00DC  55: Reserved
RSV40_IRQHandler       ;  0x38  0x00E0  56: Reserved
RSV41_IRQHandler       ;  0x39  0x00E4  57: Reserved 
RSV42_IRQHandler       ;  0x3A  0x00E8  58: BTLE Reserved
RSV43_IRQHandler       ;  0x3B  0x00EC  59: Reserved
RSV44_IRQHandler       ;  0x3C  0x00F0  60: Reserved
RSV45_IRQHandler       ;  0x3D  0x00F4  61: Reserved
RSV46_IRQHandler       ;  0x3E  0x00F8  62: Reserved
RSV47_IRQHandler       ;  0x3F  0x00FC  63: Reserved
RSV48_IRQHandler       ;  0x40  0x0100  64: Reserved
RSV49_IRQHandler       ;  0x41  0x0104  65: Reserved
RSV50_IRQHandler       ;  0x42  0x0108  66: Reserved
RSV51_IRQHandler       ;  0x43  0x010C  67: BReserved
RSV52_IRQHandler       ;  0x44  0x0110  68: Reserved
RSV53_IRQHandler       ;  0x45  0x0114  69: Reserved
RSV54_IRQHandler       ;  0x46  0x0118  70: Reserved
RSV55_IRQHandler       ;  0x47  0x011C  71: Reserved
RSV56_IRQHandler       ;  0x48  0x0120  72: Reserved
WDT1_IRQHandler        ;  0x49  0x0124  73: Watchdog 1
RSV57_IRQHandler       ;  0x4A  0x0128  74: Reserved
RSV58_IRQHandler       ;  0x4B  0x012C  75: Reserved
RSV59_IRQHandler       ;  0x4C  0x0130  76: Reserved
RSV61_IRQHandler       ;  0x4D  0x0134  77: Reserved
I2C2_IRQHandler        ;  0x4E  0x0138  78: I2C 1
RSV63_IRQHandler       ;  0x4F  0x013C  79: Reserved
RSV64_IRQHandler       ;  0x50  0x0140  80: Reserved
RSV65_IRQHandler       ;  0x51  0x0144  81: Reserved
RSV66_IRQHandler       ;  0x52  0x0148  82: Reserved
RSV67_IRQHandler       ;  0x53  0x014C  83: Reserved
DMA4_IRQHandler        ;  0x54  0x0150  84: DMA4
DMA5_IRQHandler        ;  0x55  0x0154  85: DMA5
DMA6_IRQHandler        ;  0x56  0x0158  86: DMA6
DMA7_IRQHandler        ;  0x57  0x015C  87: DMA7
DMA8_IRQHandler        ;  0x58  0x0160  88: DMA8
DMA9_IRQHandler        ;  0x59  0x0164  89: DMA9
DMA10_IRQHandler       ;  0x5A  0x0168  90: DMA10
DMA11_IRQHandler       ;  0x5B  0x016C  91: DMA11
DMA12_IRQHandler       ;  0x5C  0x0170  92: DMA12
DMA13_IRQHandler       ;  0x5D  0x0174  93: DMA13
DMA14_IRQHandler       ;  0x5E  0x0178  94: DMA14
DMA15_IRQHandler       ;  0x5F  0x017C  95: DMA15
RSV80_IRQHandler       ;  0x60  0x0180  96: Reserved
RSV81_IRQHandler       ;  0x61  0x0184  97: Reserved
ECC_IRQHandler         ;  0x62  0x0188  98: Error Correction 
RSV83_IRQHandler       ;  0x63  0x018C  99: Reserved 
RSV84_IRQHandler       ;  0x64 0x0190  100: Reserved 
RSV85_IRQHandler       ;  0x65  0x0194  101: Reserved 
RSV86_IRQHandler       ;  0x66  0x0198  102: Reserved
RSV87_IRQHandler       ;  0x67  0x019C  103: Reserved
UART3_IRQHandler       ;  0x68  0x01A0  104: UART 3 
RSV89_IRQHandler       ;  0x69  0x01A4  105: Reserved 
RSV90_IRQHandler       ;  0x6A  0x01A8  106: Reserved  
RSV91_IRQHandler       ;  0x6B  0x01AC  107: Reserved  
RSV92_IRQHandler       ;  0x6C  0x01B0  108: Reserved  
RSV93_IRQHandler       ;  0x6D  0x01B4  109: Reserved  
RSV94_IRQHandler       ;  0x6E  0x01B8  110: Reserved 
RSV95_IRQHandler       ;  0x6F  0x01B1  109: Reserved 
RSV96_IRQHandler       ;  0x70  0x01B2  109: Reserved
AES_IRQHandler         ;  0x71  0x01B1  109: AES  
CRC_IRQHandler         ;  0x72  0x01B2  109: CRC
I2S_IRQHandler         ;  0x73  0x01B2  109: I2S             

        B .
        ENDP
        ALIGN
        END
