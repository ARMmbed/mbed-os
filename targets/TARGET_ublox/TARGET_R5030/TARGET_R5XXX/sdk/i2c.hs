; 
; Copyright (C) u-blox
; All rights reserved.
; This source file is the sole property of u-blox. Reproduction or utilization
; of this source in whole or part is forbidden without the written consent of
; u-blox.
; 
; FILE: i2c.hs

I2C_CRSET EQU 0x000
I2C_CRCLEAR EQU 0x004
I2C_CR EQU 0x008
I2C_INTENABLESET EQU 0x010
I2C_INTENABLECLEAR EQU 0x014
I2C_INTENABLE EQU 0x018
I2C_SR EQU 0x020
I2C_OAR EQU 0x024
I2C_CCR EQU 0x028
I2C_TXFLUSH EQU 0x02C
I2C_RXFLUSH EQU 0x030
I2C_TXWORDCOUNT EQU 0x034
I2C_RXWORDCOUNT EQU 0x038
I2C_NRBR EQU 0x03C
I2C_TXBUFFER EQU 0x080
I2C_RXBUFFER EQU 0x100
; EOF: i2c.hs
   END
