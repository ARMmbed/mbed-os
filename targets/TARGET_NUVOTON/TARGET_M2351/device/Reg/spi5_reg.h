/**************************************************************************//**
 * @file     spi5_reg.h
 * @version  V1.00
 * @brief    SPI5 register definition header file
 *
 * @copyright (C) 2017 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __SPI5_REG_H__
#define __SPI5_REG_H__

/*---------------------- DSRC Serial Peripheral Interface Controller -------------------------*/
/**
    @addtogroup SPI5 DSRC Serial Peripheral Interface Controller(SPI5)
    Memory Mapped Structure for SPI5 Controller
@{ */

typedef struct
{


    /**
     * @var SPI5_T::CTL
     * Offset: 0x00  SPI Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |GOBUSY    |SPI Transfer Control Bit and Busy Status
     * |        |          |0 = Writing this bit 0 will stop data transfer if SPI is transferring.
     * |        |          |1 = In Master mode, writing 1 to this bit will start the SPI data transfer; In Slave mode, writing u20181' to this bit indicates that the slave is ready to communicate with a master.
     * |        |          |If the FIFO mode is disabled, during the data transfer, this bit keeps the value of u20181'
     * |        |          |As the transfer is finished, this bit will be cleared automatically
     * |        |          |Software can read this bit to check if the SPI is in busy status.
     * |        |          |In FIFO mode, this bit will be controlled by hardware
     * |        |          |Software should not modify this bit
     * |        |          |In slave mode, this bit always returns 1 when software reads this register
     * |        |          |In master mode, this bit reflects the busy or idle status of SPI.
     * |        |          |Note:
     * |        |          |1.When FIFO mode is disabled, all configurations should be set before writing 1 to the GOBUSY bit in the SPI_CTL register.
     * |        |          |2
     * |        |          |When FIFO bit is disabled and the software uses TX or RX PDMA function to transfer data, this bit will be cleared after the PDMA controller finishes the data transfer.
     * |[1]     |RXNEG     |Receive on Negative Edge
     * |        |          |0 = The received data is latched on the rising edge of SPI_CLK.
     * |        |          |1 = The received data is latched on the falling edge of SPI_CLK.
     * |        |          |Note: Refer to Edge section.
     * |        |          |Note:
     * |[2]     |TXNEG     |Transmit on Negative Edge
     * |        |          |0 = The transmitted data output is changed on the rising edge of SPI_CLK.
     * |        |          |1 = The transmitted data output is changed on the falling edge of SPI_CLK.
     * |        |          |Note: Refer to Edge section.
     * |        |          |Note:
     * |[7:3]   |DWIDTH    |Data Width
     * |        |          |This field specifies how many bits can be transmitted / received in one transaction
     * |        |          |The minimum bit length is 8 bits and can be up to 32 bits.
     * |        |          |0x1~0x7: reserved
     * |        |          |0x8 = 8 bits are transmitted in one transaction.
     * |        |          |0x9 = 9 bits are transmitted in one transaction.
     * |        |          |0xA = 10 bits are transmitted in one transaction.
     * |        |          |u2026
     * |        |          |0x1F = 31 bits are transmitted in one transaction.
     * |        |          |0x0 = 32 bits are transmitted in one transaction.
     * |        |          |Note:
     * |[10]    |LSB       |Send LSB First
     * |        |          |0 = The MSB, which bit of transmit/receive register depends on the setting of DWIDTH (SPI_CTL[7:3]), is transmitted/received first.
     * |        |          |1 = The LSB, bit 0 of the SPI_TX, is sent first to the the SPI data output pin, and the first bit received from the SPI data input pin will be put in the LSB position of the RX register (SPI_RX
     * |        |          |Note: Refer to LSB first section.
     * |[11]    |CLKPOL    |Clock Polarity
     * |        |          |0 = The default level of SPI_CLK is low.
     * |        |          |1 = The default level of SPI_CLK is high.
     * |        |          |Note: Refer to Clock Parity section.
     * |        |          |Note:
     * |[15:12] |SUSPITV   |Suspend Interval (Master Only)
     * |        |          |These four bits provide configurable suspend interval between two successive transmit/receive transaction in a transfer
     * |        |          |The suspend interval is from the last falling clock edge of the current transaction to the first rising clock edge of the successive transaction if CLKPOL = 0
     * |        |          |If CLKPOL = 1, the interval is from the rising clock edge to the falling clock edge.
     * |        |          |The default value is 0x3
     * |        |          |The desired suspend interval is obtained according to the following equation: (SUSPITV[3:0] + 0.5) * period of SPI_CLK
     * |        |          |For example,
     * |        |          |SUSPITV = 0x0 u2026. 0.5 SPI_CLK clock cycle.
     * |        |          |SUSPITV = 0x1 u2026. 1.5 SPI_CLK clock cycle.
     * |        |          |u2026u2026
     * |        |          |SUSPITV = 0xE u2026. 14.5 SPI_CLK clock cycle.
     * |        |          |SUSPITV = 0xF u2026. 15.5 SPI_CLK clock cycle.
     * |        |          |Note:
     * |[17]    |UNITIEN   |Unit Transfer Interrupt Enable Bit
     * |        |          |0 = SPI unit transfer interrupt Disabled.
     * |        |          |1 = SPI unit transfer interrupt Enabled.
     * |[18]    |SLAVE     |Slave Mode Selection
     * |        |          |0 = SPI controller set as Master mode.
     * |        |          |1 = SPI controller set as Slave mode.
     * |        |          |Note: Refer to Slave Selection section
     * |        |          |Note:
     * |[19]    |REORDER   |Byte Reorder Function Enable Bit
     * |        |          |0 = Byte reorder function Disabled.
     * |        |          |1 = Enable byte reorder function and insert a byte suspend interval among each byte
     * |        |          |The byte reorder function is only available when DWIDTH is configured as 16, 24, and 32 bits.
     * |        |          |Note: The suspend interval is defined in SUSPITV. Refer to Byte Reorder section.
     * |        |          |Note: Byte Suspend is only used in SPI Byte Reorder mode.
     * |[21]    |FIFOM     |FIFO Mode Enable Bit
     * |        |          |0 = FIFO mode Disabled (in Normal mode).
     * |        |          |1 = FIFO mode Enabled.
     * |        |          |Note: Refer to FIFO Mode section.
     * |        |          |Note:
     * |[30]    |WKSSEN    |Wake-up by Slave Select Enable Bit
     * |        |          |0 = Wake-up function Disabled.
     * |        |          |1 = Wake-up function Enabled.
     * |        |          |Note: The Slave select wake-up function is only available in SPI Slave mode
     * |        |          |When the system enters power-down mode, the system can be wake-up from the SPI controller if this bit and PDWKIEN (CLK_PWRCTL[5]) are enabled and there is any toggle on the SPI_SS port
     * |        |          |After the system wake-up, this bit must be cleared by user to disable the wake-up requirement.
     * |        |          |Note: The wake up event will not assert the SPI interrupt, but user can read the corresponding status bit to check its occurrence.
     * |[31]    |WKCLKEN   |Wake-up by SPI Clock Enable Bit
     * |        |          |0 = Wake-up function Disabled.
     * |        |          |1 = Wake-up function Enabled.
     * |        |          |Note: When the system enters power-down mode, the system can be wake-up from the SPI controller if this bit and PDWKIEN (CLK_PWRCTL[5]) are enabled and there is any toggle on the SPI_CLK port
     * |        |          |After the system wake-up, this bit must be cleared by user to disable the wake-up requirement.
     * |        |          |Note: The wake up event will not assert the SPI interrupt, but user can read the corresponding status bit to check its occurrence.
     * @var SPI5_T::STATUS
     * Offset: 0x04  SPI Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |RXEMPTY   |Receive FIFO Buffer Empty Indicator (Read Only)
     * |        |          |0 = Received data FIFO is not empty in the FIFO mode.
     * |        |          |1 = Received data FIFO is empty in the FIFO mode.
     * |[1]     |RXFULL    |Receive FIFO Buffer Full Indicator (Read Only)
     * |        |          |0 = Received data FIFO is not full in FIFO mode.
     * |        |          |1 = Received data FIFO is full in the FIFO mode.
     * |[2]     |TXEMPTY   |Transmit FIFO Buffer Empty Indicator (Read Only)
     * |        |          |0 = Transmitted data FIFO is not empty in the FIFO mode.
     * |        |          |1 =Transmitted data FIFO is empty in the FIFO mode.
     * |[3]     |TXFULL    |Transmit FIFO Buffer Full Indicator (Read Only)
     * |        |          |0 = Transmitted data FIFO is not full in the FIFO mode.
     * |        |          |1 = Transmitted data FIFO is full in the FIFO mode.
     * |[4]     |LTRIGF    |Level Trigger Accomplish Flag (Read Only)
     * |        |          |In Slave mode, this bit indicates whether the received bit number meets the requirement or not after the current transaction done.
     * |        |          |0 = The transferred bit length of one transaction does not meet the specified requirement.
     * |        |          |1 = The transferred bit length meets the specified requirement which defined in DWIDTH.
     * |        |          |Note: This bit is read only
     * |        |          |As the software sets the GOBUSY bit to 1, the LTRIGF will be cleared to 0 after 4 SPI peripheral clock periods plus 1 system clock period
     * |        |          |In FIFO mode, this bit is unmeaning.
     * |[6]     |SLVSTAIF  |Slave Start Interrupt Flag
     * |        |          |It is used to dedicate that the transfer has started in Slave mode with no slave select.
     * |        |          |0 = Slave started transfer no active.
     * |        |          |1 = Transfer has started in Slave mode with no slave select
     * |        |          |It is automatically cleared by transfer done or writing u20181'.
     * |[7]     |UNITIF    |Unit Transfer Interrupt Flag
     * |        |          |0 = No transaction has been finished since this bit was cleared to 0.
     * |        |          |1 = SPI controller has finished one unit transfer.
     * |        |          |Note 1: If SSINAIEN (SPI_SSCTL[16]) is set to 1, this bit will be asserted again when transfer is done and the slave select signal becomes inactive from active condition.
     * |        |          |Note 2: This bit will be cleared by writing 1 to it.
     * |[8]     |RXTHIF    |RX FIFO Threshold Interrupt Flag (Read Only)
     * |        |          |0 = RX valid data counts are less than or equal to RXTH (SPI_FIFOCTL[26:24]).
     * |        |          |1 = RX valid data counts are larger than RXTH.
     * |        |          |Note: If RXTHIEN (SPI_FIFOCTL[2]) = 1 and RXTHIF = 1, SPI will generate interrupt.
     * |[9]     |RXOVIF    |Receive FIFO Overrun Interrupt Flag
     * |        |          |0 = No FIFO overrun.
     * |        |          |1 = Receive FIFO overrun.
     * |        |          |Note 1: If SPI receives data when RX FIFO is full, this bit will set to 1, and the received data will be dropped.
     * |        |          |Note 2: This bit will be cleared by writing 1 to it.
     * |[10]    |TXTHIF    |Transmit FIFO Threshold Interrupt Flag (Read Only)
     * |        |          |0 = TX valid data counts are larger than TXTH (SPI_FIFOCTL[30:28]).
     * |        |          |1 = TX valid data counts are less than or equal to TXTH.
     * |[12]    |RXTOIF    |Receive Time-out Interrupt Flag
     * |        |          |0 = There is not time-out event on the received buffer.
     * |        |          |1 = Time-out event active in RX FIFO is not empty.
     * |        |          |Refer to Time Out section.
     * |        |          |Note: This bit will be cleared by writing 1 to it.
     * |[13]    |SLVTOIF   |Slave Time-out Interrupt Flag
     * |        |          |If SLVTOIEN (SPI_SSCTL[6]) is set to 1, this bit will be asserted when slave time-out event occur
     * |        |          |Software can clear this bit by setting RXFBCLR (SPI_FIFOCTL[0]) or writing 1 to clear this bit.
     * |        |          |0 = Slave time-out does not occur yet.
     * |        |          |1 = Slave time-out has occurred.
     * |[15]    |SLVTXSKE  |Slave Mode Transmit Skew Buffer Empty Status
     * |        |          |This bit indicates the empty status of transmit skew buffer which is used in Slave mode.
     * |[19:16] |RXCNT     |Receive FIFO Data Counts (Read Only)
     * |        |          |This bit field indicates the valid data count of receive FIFO buffer.
     * |[23:20] |TXCNT     |Transmit FIFO Data Counts (Read Only)
     * |        |          |This bit field indicates the valid data count of transmit FIFO buffer.
     * |[30]    |WKSSIF    |Wake-up by Slave Select Interrupt Flag
     * |        |          |When chip is woken up from power-down mode by the toggle event on SPI_SS port, this bit is set to 1
     * |        |          |This bit can be cleared by writing u20181' to it.
     * |[31]    |WKCLKIF   |Wake-up by SPI Clock Interrupt Flag
     * |        |          |When chip is woken up from power-down mode by the toggle event on SPI_CLK port, this bit is set to 1
     * |        |          |This bit can be cleared by writing u20181' to it.
     * @var SPI5_T::CLKDIV
     * Offset: 0x08  SPI Clock Divider Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:0]   |DIVIDER   |Clock Divider
     * |        |          |The value is the 1th frequency divider of the PCLK to generate the serial clock of SPI_CLK
     * |        |          |The desired frequency is obtained according to the following equation:
     * |        |          |Where
     * |        |          |is the SPI peripheral clock source
     * |        |          |It is defined in the CLK_CLKSEL2[15:14] in Clock control section (CLK_BA + 0x18).
     * |        |          |Note:
     * @var SPI5_T::SSCTL
     * Offset: 0x0C  SPI Slave Select Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |SS        |Slave Selection Control (Master Only)
     * |        |          |If AUTOSS bit (SPI_SSCTL[3]) is cleared, writing 1 to SS (SPI_SSCTL[0]) bit sets the SPI_SS line to an active state and writing 0 sets the line back to inactive state
     * |        |          |If AUTOSS = 0,.
     * |        |          |0 = SPI_SS is inactive.
     * |        |          |1 =SPI_SS is active.
     * |        |          |If AUTOSS bit is set, writing 1 to this field will select appropriate SPI_SS line to be automatically driven to active state for the duration of the transaction, and will be driven to inactive state for the rest of the time
     * |        |          |(The active level of SPI_SS is specified in SSACTPOL).
     * |        |          |If AUTOSS =1,.
     * |        |          |0 = SPI_SS is inactive.
     * |        |          |1 = SPI_SS is active on the duration of transaction.
     * |        |          |Note:
     * |        |          |1
     * |        |          |This interface can only drive one device/slave at a given time
     * |        |          |Therefore, the slaves select of the selected device must be set to its active level before starting any read or write transfer.
     * |        |          |2
     * |        |          |SPI_SS is also defined as device/slave select input in Slave mode
     * |        |          |And that the slave select input must be driven by edge active trigger which level depend on the SSACTPOL setting, otherwise the SPI slave core will go into dead path until the edge active triggers again or reset the SPI core by software
     * |[2]     |SSACTPOL  |Slave Selection Active Polarity
     * |        |          |It defines the active polarity of slave selection signal (SPI_SS).
     * |        |          |0 = The SPI_SS slave select signal is active Low.
     * |        |          |1 = The SPI_SS slave select signal is active High.
     * |[3]     |AUTOSS    |Automatic Slave Selection Function Enable Bit (Master Only)
     * |        |          |0 = If this bit is set as 0, slave select signals are asserted and de-asserted by setting and clearing related bits in SS (SPI_SSCTL[0]).
     * |        |          |1 = If this bit is set as 1, SPI_SS signals are generated automatically
     * |        |          |It means that device/slave select signal, which is set in SS (SPI_SSCTL[0]) is asserted by the SPI controller when transmit/receive is started, and is de-asserted after each transaction is done.
     * |[4]     |SSLTRIG   |Slave Select Level Trigger Control
     * |        |          |0 = The input slave select signal is edge-trigger.
     * |        |          |1 = The slave select signal will be level-trigger
     * |        |          |It depends on SSACTPOL to decide the signal is active low or active high.
     * |[5]     |SLV3WIRE  |Slave 3-wire Mode Enable Bit
     * |        |          |This bit is used to ignore the slave select signal in Slave mode
     * |        |          |The SPI controller can work with 3-wire interface including SPI_CLK, SPI_MISO, and SPI_MOSI when it is set as a slave device.
     * |        |          |0 = The controller is 4-wire bi-direction interface.
     * |        |          |1 = The controller is 3-wire bi-direction interface in Slave mode
     * |        |          |When this bit is set as 1, the controller start to transmit/receive data after the GOBUSY bit active and the SPI clock input.
     * |        |          |Note 1: Refer to No Slave Select Mode.
     * |        |          |Note 2: In no slave select signal mode, hardware will set the SSLTRIG (SPI_SSCTL[4]) as 1 automatically.
     * |[6]     |SLVTOIEN  |Slave Time-out Interrupt Enable Bit
     * |        |          |This bit is used to enable the slave time-out function in slave mode and there will be an interrupt if slave time-out event occur
     * |        |          |0 = Slave time-out function and interrupt both Disabled.
     * |        |          |1 = Slave time-out function and interrupt both Enabled.
     * |[8]     |SLVABORT  |Abort in Slave Mode with No Slave Selected
     * |        |          |0 = No force the slave abort.
     * |        |          |1 = Force the current transfer done in no slave select mode.
     * |        |          |Refer to No Slave Select Mode.
     * |        |          |Note: It is auto cleared to 0 by hardware when the abort event is active.
     * |[9]     |SSTAIEN   |Slave Start Interrupt Enable Bit
     * |        |          |0 = Transfer start interrupt Disabled in no slave select mode.
     * |        |          |1 = Transaction start interrupt Enabled in no slave select mode
     * |        |          |It is cleared when the current transfer done or the SLVSTAIF bit cleared (write 1 clear).
     * |        |          |Refer to No Slave Select Mode.
     * |[16]    |SSINAIEN  |Slave Select Inactive Interrupt Enable Bit
     * |        |          |It is used to enable the interrupt when the transfer has done in slave mode.
     * |        |          |0 = No any interrupt, even there is slave select inactive event.
     * |        |          |1 = There is interrupt event when the slave select signal becomes inactive from active condition
     * |        |          |It is used to inform the user to know that the transaction has finished and the slave select into the inactive state.
     * |[29:20] |SLVTOCNT  |Slave Mode Time-out Period
     * |        |          |In Slave mode, these bits indicate the time-out period when there is bus clock input during slave select active
     * |        |          |The clock source of the time-out counter is Slave peripheral clock
     * |        |          |If the value is 0, it indicates the slave mode time-out function is disabled.
     * @var SPI5_T::RX
     * Offset: 0x10  SPI Receive Data FIFO Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[31:0]  |RX        |Receive Data Register (Read Only)
     * |        |          |The received data can be read on it
     * |        |          |If the FIFO bit is set as 1, the user also checks the RXEMPTY (SPI_STATUS[0]), to check if there is any more received data or not.
     * @var SPI5_T::TX
     * Offset: 0x20  SPI Transmit Data FIFO Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[31:0]  |TX        |Transmit Data Register (Write Only)
     * |        |          |The Data Transmit Registers hold the data to be transmitted in the next transfer
     * |        |          |The number of valid bits depends on the setting of transmit bit length field of the SPI_CTL register.
     * |        |          |For example, if DWIDTH is set to 0x8, the bit SPI_TX[7:0] will be transmitted in next transfer
     * |        |          |If DWIDTH is set to 0x0, the SPI controller will perform a 32-bit transfer.
     * |        |          |Note:
     * |        |          |If the SPI controller operates as slave device and FIFO mode is disabled, software must update the transmit data register before setting the GOBUSY bit to 1
     * @var SPI5_T::PDMACTL
     * Offset: 0x38  SPI PDMA Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |TXPDMAEN  |Transmit PDMA Enable Bit
     * |        |          |0 = Transmit PDMA function Disabled.
     * |        |          |1 = Transmit PDMA function Enabled.
     * |        |          |Refer to PDMA section for more detail information.
     * |        |          |Note:
     * |        |          |1
     * |        |          |Two transaction need minimal 18 APB clock + 8 SPI peripheral clocks suspend interval in master mode for edge mode and 18 APB clock + 9.5 SPI peripheral clocks for level mode.
     * |        |          |Hardware will clear this bit to 0 automatically after PDMA transfer done.
     * |[1]     |RXPDMAEN  |Receiving PDMA Enable Bit
     * |        |          |0 = Receiver PDMA function Disabled.
     * |        |          |1 = Receiver PDMA function Enabled.
     * |        |          |Refer to PDMA section for more detail information.
     * |        |          |Note:
     * |        |          |Hardware will clear this bit to 0 automatically after PDMA transfer done.
     * |        |          |In Slave mode and the FIFO bit is disabled, if the receive PDMA is enabled but the transmit PDMA is disabled, the minimal suspend interval between two successive transactions input is need to be larger than 9 SPI peripheral clock + 4 APB clock for edge mode and 9.5 SPI peripheral clock + 4 APB clock
     * |[2]     |PDMARST   |PDMA Reset
     * |        |          |It is used to reset the SPI PDMA function into default state.
     * |        |          |0 = After reset PDMA function or in normal operation.
     * |        |          |1 = Reset PDMA function.
     * |        |          |Note: It is auto cleared to 0 after the reset function has done.
     * @var SPI5_T::FIFOCTL
     * Offset: 0x3C  SPI FIFO Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |RXFBCLR   |Receive FIFO Buffer Clear
     * |        |          |0 = No clear the received FIFO.
     * |        |          |1 = Clear the received FIFO.
     * |        |          |Note: This bit is used to clear the receiver counter in FIFO Mode
     * |        |          |This bit can be written 1 to clear the receiver counter and this bit will be cleared to 0 automatically after clearing receiving counter
     * |        |          |After the clear operation, the flag of RXEMPTY in SPI_STATUS[0] will be set to 1.
     * |[1]     |TXFBCLR   |Transmit FIFO Buffer Clear
     * |        |          |0 = Not clear the transmitted FIFO.
     * |        |          |1 = Clear the transmitted FIFO.
     * |        |          |Note: This bit is used to clear the transmit counter in FIFO Mode
     * |        |          |This bit can be written 1 to clear the transmitting counter and this bit will be cleared to 0 automatically after clearing transmitting counter
     * |        |          |After the clear operation, the flag of TXEMPTY in SPI_STATUS[2] will be set to 1.
     * |[2]     |RXTHIEN   |Receive Threshold Interrupt Enable Bit
     * |        |          |0 = RX threshold interrupt Disabled.
     * |        |          |1 = RX threshold interrupt Enabled.
     * |[3]     |TXTHIEN   |Transmit Threshold Interrupt Enable Bit
     * |        |          |0 = TX threshold interrupt Disabled.
     * |        |          |1 = TX threshold interrupt Enabled.
     * |[4]     |RXOVIEN   |Receive FIFO Overrun Interrupt Enable Bit
     * |        |          |0 = RX FIFO overrun interrupt Disabled.
     * |        |          |1 = RX FIFO overrun interrupt Enabled.
     * |[7]     |RXTOIEN   |RX Read Time Out Interrupt Enable Bit
     * |        |          |0 = RX read Timeout Interrupt Disabled.
     * |        |          |1 = RX read Timeout Interrupt Enabled.
     * |[26:24] |RXTH      |Received FIFO Threshold
     * |        |          |If RX valid data counts are larger than RXTH, RXTHIF (SPI_STATUS[8]) will be set to 1..
     * |[30:28] |TXTH      |Transmit FIFO Threshold
     * |        |          |If TX valid data counts are less than or equal to TXTH, TXTHIF (SPI_STATUS[10]) will be set to 1.
     */
    __IO uint32_t CTL;                   /*!< [0x0000] SPI Control Register                                             */
    __IO uint32_t STATUS;                /*!< [0x0004] SPI Status Register                                              */
    __IO uint32_t CLKDIV;                /*!< [0x0008] SPI Clock Divider Register                                       */
    __IO uint32_t SSCTL;                 /*!< [0x000c] SPI Slave Select Control Register                                */
    __I  uint32_t RX;                    /*!< [0x0010] SPI Receive Data FIFO Register                                   */
    __I  uint32_t RESERVE0[3];
    __O  uint32_t TX;                    /*!< [0x0020] SPI Transmit Data FIFO Register                                  */
    __I  uint32_t RESERVE1[5];
    __IO uint32_t PDMACTL;               /*!< [0x0038] SPI PDMA Control Register                                        */
    __IO uint32_t FIFOCTL;               /*!< [0x003c] SPI FIFO Control Register                                        */

} SPI5_T;

/**
    @addtogroup SPI5_CONST SPI5 Bit Field Definition
    Constant Definitions for SPI5 Controller
@{ */

#define SPI5_CTL_GOBUSY_Pos              (0)                                               /*!< SPI5_T::CTL: GOBUSY Position           */
#define SPI5_CTL_GOBUSY_Msk              (0x1ul << SPI5_CTL_GOBUSY_Pos)                    /*!< SPI5_T::CTL: GOBUSY Mask               */

#define SPI5_CTL_RXNEG_Pos               (1)                                               /*!< SPI5_T::CTL: RXNEG Position            */
#define SPI5_CTL_RXNEG_Msk               (0x1ul << SPI5_CTL_RXNEG_Pos)                     /*!< SPI5_T::CTL: RXNEG Mask                */

#define SPI5_CTL_TXNEG_Pos               (2)                                               /*!< SPI5_T::CTL: TXNEG Position            */
#define SPI5_CTL_TXNEG_Msk               (0x1ul << SPI5_CTL_TXNEG_Pos)                     /*!< SPI5_T::CTL: TXNEG Mask                */

#define SPI5_CTL_DWIDTH_Pos              (3)                                               /*!< SPI5_T::CTL: DWIDTH Position           */
#define SPI5_CTL_DWIDTH_Msk              (0x1ful << SPI5_CTL_DWIDTH_Pos)                   /*!< SPI5_T::CTL: DWIDTH Mask               */

#define SPI5_CTL_LSB_Pos                 (10)                                              /*!< SPI5_T::CTL: LSB Position              */
#define SPI5_CTL_LSB_Msk                 (0x1ul << SPI5_CTL_LSB_Pos)                       /*!< SPI5_T::CTL: LSB Mask                  */

#define SPI5_CTL_CLKPOL_Pos              (11)                                              /*!< SPI5_T::CTL: CLKPOL Position           */
#define SPI5_CTL_CLKPOL_Msk              (0x1ul << SPI5_CTL_CLKPOL_Pos)                    /*!< SPI5_T::CTL: CLKPOL Mask               */

#define SPI5_CTL_SUSPITV_Pos             (12)                                              /*!< SPI5_T::CTL: SUSPITV Position          */
#define SPI5_CTL_SUSPITV_Msk             (0xful << SPI5_CTL_SUSPITV_Pos)                   /*!< SPI5_T::CTL: SUSPITV Mask              */

#define SPI5_CTL_UNITIEN_Pos             (17)                                              /*!< SPI5_T::CTL: UNITIEN Position          */
#define SPI5_CTL_UNITIEN_Msk             (0x1ul << SPI5_CTL_UNITIEN_Pos)                   /*!< SPI5_T::CTL: UNITIEN Mask              */

#define SPI5_CTL_SLAVE_Pos               (18)                                              /*!< SPI5_T::CTL: SLAVE Position            */
#define SPI5_CTL_SLAVE_Msk               (0x1ul << SPI5_CTL_SLAVE_Pos)                     /*!< SPI5_T::CTL: SLAVE Mask                */

#define SPI5_CTL_REORDER_Pos             (19)                                              /*!< SPI5_T::CTL: REORDER Position          */
#define SPI5_CTL_REORDER_Msk             (0x1ul << SPI5_CTL_REORDER_Pos)                   /*!< SPI5_T::CTL: REORDER Mask              */

#define SPI5_CTL_FIFOM_Pos               (21)                                              /*!< SPI5_T::CTL: FIFOM Position            */
#define SPI5_CTL_FIFOM_Msk               (0x1ul << SPI5_CTL_FIFOM_Pos)                     /*!< SPI5_T::CTL: FIFOM Mask                */

#define SPI5_CTL_WKSSEN_Pos              (30)                                              /*!< SPI5_T::CTL: WKSSEN Position           */
#define SPI5_CTL_WKSSEN_Msk              (0x1ul << SPI5_CTL_WKSSEN_Pos)                    /*!< SPI5_T::CTL: WKSSEN Mask               */

#define SPI5_CTL_WKCLKEN_Pos             (31)                                              /*!< SPI5_T::CTL: WKCLKEN Position          */
#define SPI5_CTL_WKCLKEN_Msk             (0x1ul << SPI5_CTL_WKCLKEN_Pos)                   /*!< SPI5_T::CTL: WKCLKEN Mask              */

#define SPI5_STATUS_RXEMPTY_Pos          (0)                                               /*!< SPI5_T::STATUS: RXEMPTY Position       */
#define SPI5_STATUS_RXEMPTY_Msk          (0x1ul << SPI5_STATUS_RXEMPTY_Pos)                /*!< SPI5_T::STATUS: RXEMPTY Mask           */

#define SPI5_STATUS_RXFULL_Pos           (1)                                               /*!< SPI5_T::STATUS: RXFULL Position        */
#define SPI5_STATUS_RXFULL_Msk           (0x1ul << SPI5_STATUS_RXFULL_Pos)                 /*!< SPI5_T::STATUS: RXFULL Mask            */

#define SPI5_STATUS_TXEMPTY_Pos          (2)                                               /*!< SPI5_T::STATUS: TXEMPTY Position       */
#define SPI5_STATUS_TXEMPTY_Msk          (0x1ul << SPI5_STATUS_TXEMPTY_Pos)                /*!< SPI5_T::STATUS: TXEMPTY Mask           */

#define SPI5_STATUS_TXFULL_Pos           (3)                                               /*!< SPI5_T::STATUS: TXFULL Position        */
#define SPI5_STATUS_TXFULL_Msk           (0x1ul << SPI5_STATUS_TXFULL_Pos)                 /*!< SPI5_T::STATUS: TXFULL Mask            */

#define SPI5_STATUS_LTRIGF_Pos           (4)                                               /*!< SPI5_T::STATUS: LTRIGF Position        */
#define SPI5_STATUS_LTRIGF_Msk           (0x1ul << SPI5_STATUS_LTRIGF_Pos)                 /*!< SPI5_T::STATUS: LTRIGF Mask            */

#define SPI5_STATUS_SLVSTAIF_Pos         (6)                                               /*!< SPI5_T::STATUS: SLVSTAIF Position      */
#define SPI5_STATUS_SLVSTAIF_Msk         (0x1ul << SPI5_STATUS_SLVSTAIF_Pos)               /*!< SPI5_T::STATUS: SLVSTAIF Mask          */

#define SPI5_STATUS_UNITIF_Pos           (7)                                               /*!< SPI5_T::STATUS: UNITIF Position        */
#define SPI5_STATUS_UNITIF_Msk           (0x1ul << SPI5_STATUS_UNITIF_Pos)                 /*!< SPI5_T::STATUS: UNITIF Mask            */

#define SPI5_STATUS_RXTHIF_Pos           (8)                                               /*!< SPI5_T::STATUS: RXTHIF Position        */
#define SPI5_STATUS_RXTHIF_Msk           (0x1ul << SPI5_STATUS_RXTHIF_Pos)                 /*!< SPI5_T::STATUS: RXTHIF Mask            */

#define SPI5_STATUS_RXOVIF_Pos           (9)                                               /*!< SPI5_T::STATUS: RXOVIF Position        */
#define SPI5_STATUS_RXOVIF_Msk           (0x1ul << SPI5_STATUS_RXOVIF_Pos)                 /*!< SPI5_T::STATUS: RXOVIF Mask            */

#define SPI5_STATUS_TXTHIF_Pos           (10)                                              /*!< SPI5_T::STATUS: TXTHIF Position        */
#define SPI5_STATUS_TXTHIF_Msk           (0x1ul << SPI5_STATUS_TXTHIF_Pos)                 /*!< SPI5_T::STATUS: TXTHIF Mask            */

#define SPI5_STATUS_RXTOIF_Pos           (12)                                              /*!< SPI5_T::STATUS: RXTOIF Position        */
#define SPI5_STATUS_RXTOIF_Msk           (0x1ul << SPI5_STATUS_RXTOIF_Pos)                 /*!< SPI5_T::STATUS: RXTOIF Mask            */

#define SPI5_STATUS_SLVTOIF_Pos          (13)                                              /*!< SPI5_T::STATUS: SLVTOIF Position       */
#define SPI5_STATUS_SLVTOIF_Msk          (0x1ul << SPI5_STATUS_SLVTOIF_Pos)                /*!< SPI5_T::STATUS: SLVTOIF Mask           */

#define SPI5_STATUS_SLVTXSKE_Pos         (15)                                              /*!< SPI5_T::STATUS: SLVTXSKE Position      */
#define SPI5_STATUS_SLVTXSKE_Msk         (0x1ul << SPI5_STATUS_SLVTXSKE_Pos)               /*!< SPI5_T::STATUS: SLVTXSKE Mask          */

#define SPI5_STATUS_RXCNT_Pos            (16)                                              /*!< SPI5_T::STATUS: RXCNT Position         */
#define SPI5_STATUS_RXCNT_Msk            (0xful << SPI5_STATUS_RXCNT_Pos)                  /*!< SPI5_T::STATUS: RXCNT Mask             */

#define SPI5_STATUS_TXCNT_Pos            (20)                                              /*!< SPI5_T::STATUS: TXCNT Position         */
#define SPI5_STATUS_TXCNT_Msk            (0xful << SPI5_STATUS_TXCNT_Pos)                  /*!< SPI5_T::STATUS: TXCNT Mask             */

#define SPI5_STATUS_WKSSIF_Pos           (30)                                              /*!< SPI5_T::STATUS: WKSSIF Position        */
#define SPI5_STATUS_WKSSIF_Msk           (0x1ul << SPI5_STATUS_WKSSIF_Pos)                 /*!< SPI5_T::STATUS: WKSSIF Mask            */

#define SPI5_STATUS_WKCLKIF_Pos          (31)                                              /*!< SPI5_T::STATUS: WKCLKIF Position       */
#define SPI5_STATUS_WKCLKIF_Msk          (0x1ul << SPI5_STATUS_WKCLKIF_Pos)                /*!< SPI5_T::STATUS: WKCLKIF Mask           */

#define SPI5_CLKDIV_DIVIDER_Pos          (0)                                               /*!< SPI5_T::CLKDIV: DIVIDER Position       */
#define SPI5_CLKDIV_DIVIDER_Msk          (0xfful << SPI5_CLKDIV_DIVIDER_Pos)               /*!< SPI5_T::CLKDIV: DIVIDER Mask           */

#define SPI5_SSCTL_SS_Pos                (0)                                               /*!< SPI5_T::SSCTL: SS Position             */
#define SPI5_SSCTL_SS_Msk                (0x1ul << SPI5_SSCTL_SS_Pos)                      /*!< SPI5_T::SSCTL: SS Mask                 */

#define SPI5_SSCTL_SSACTPOL_Pos          (2)                                               /*!< SPI5_T::SSCTL: SSACTPOL Position       */
#define SPI5_SSCTL_SSACTPOL_Msk          (0x1ul << SPI5_SSCTL_SSACTPOL_Pos)                /*!< SPI5_T::SSCTL: SSACTPOL Mask           */

#define SPI5_SSCTL_AUTOSS_Pos            (3)                                               /*!< SPI5_T::SSCTL: AUTOSS Position         */
#define SPI5_SSCTL_AUTOSS_Msk            (0x1ul << SPI5_SSCTL_AUTOSS_Pos)                  /*!< SPI5_T::SSCTL: AUTOSS Mask             */

#define SPI5_SSCTL_SSLTRIG_Pos           (4)                                               /*!< SPI5_T::SSCTL: SSLTRIG Position        */
#define SPI5_SSCTL_SSLTRIG_Msk           (0x1ul << SPI5_SSCTL_SSLTRIG_Pos)                 /*!< SPI5_T::SSCTL: SSLTRIG Mask            */

#define SPI5_SSCTL_SLV3WIRE_Pos          (5)                                               /*!< SPI5_T::SSCTL: SLV3WIRE Position       */
#define SPI5_SSCTL_SLV3WIRE_Msk          (0x1ul << SPI5_SSCTL_SLV3WIRE_Pos)                /*!< SPI5_T::SSCTL: SLV3WIRE Mask           */

#define SPI5_SSCTL_SLVTOIEN_Pos          (6)                                               /*!< SPI5_T::SSCTL: SLVTOIEN Position       */
#define SPI5_SSCTL_SLVTOIEN_Msk          (0x1ul << SPI5_SSCTL_SLVTOIEN_Pos)                /*!< SPI5_T::SSCTL: SLVTOIEN Mask           */

#define SPI5_SSCTL_SLVABORT_Pos          (8)                                               /*!< SPI5_T::SSCTL: SLVABORT Position       */
#define SPI5_SSCTL_SLVABORT_Msk          (0x1ul << SPI5_SSCTL_SLVABORT_Pos)                /*!< SPI5_T::SSCTL: SLVABORT Mask           */

#define SPI5_SSCTL_SSTAIEN_Pos           (9)                                               /*!< SPI5_T::SSCTL: SSTAIEN Position        */
#define SPI5_SSCTL_SSTAIEN_Msk           (0x1ul << SPI5_SSCTL_SSTAIEN_Pos)                 /*!< SPI5_T::SSCTL: SSTAIEN Mask            */

#define SPI5_SSCTL_SSINAIEN_Pos          (16)                                              /*!< SPI5_T::SSCTL: SSINAIEN Position       */
#define SPI5_SSCTL_SSINAIEN_Msk          (0x1ul << SPI5_SSCTL_SSINAIEN_Pos)                /*!< SPI5_T::SSCTL: SSINAIEN Mask           */

#define SPI5_SSCTL_SLVTOCNT_Pos          (20)                                              /*!< SPI5_T::SSCTL: SLVTOCNT Position       */
#define SPI5_SSCTL_SLVTOCNT_Msk          (0x3fful << SPI5_SSCTL_SLVTOCNT_Pos)              /*!< SPI5_T::SSCTL: SLVTOCNT Mask           */

#define SPI5_RX_RX_Pos                   (0)                                               /*!< SPI5_T::RX: RX Position                */
#define SPI5_RX_RX_Msk                   (0xfffffffful << SPI5_RX_RX_Pos)                  /*!< SPI5_T::RX: RX Mask                    */

#define SPI5_TX_TX_Pos                   (0)                                               /*!< SPI5_T::TX: TX Position                */
#define SPI5_TX_TX_Msk                   (0xfffffffful << SPI5_TX_TX_Pos)                  /*!< SPI5_T::TX: TX Mask                    */

#define SPI5_PDMACTL_TXPDMAEN_Pos        (0)                                               /*!< SPI5_T::PDMACTL: TXPDMAEN Position     */
#define SPI5_PDMACTL_TXPDMAEN_Msk        (0x1ul << SPI5_PDMACTL_TXPDMAEN_Pos)              /*!< SPI5_T::PDMACTL: TXPDMAEN Mask         */

#define SPI5_PDMACTL_RXPDMAEN_Pos        (1)                                               /*!< SPI5_T::PDMACTL: RXPDMAEN Position     */
#define SPI5_PDMACTL_RXPDMAEN_Msk        (0x1ul << SPI5_PDMACTL_RXPDMAEN_Pos)              /*!< SPI5_T::PDMACTL: RXPDMAEN Mask         */

#define SPI5_PDMACTL_PDMARST_Pos         (2)                                               /*!< SPI5_T::PDMACTL: PDMARST Position      */
#define SPI5_PDMACTL_PDMARST_Msk         (0x1ul << SPI5_PDMACTL_PDMARST_Pos)               /*!< SPI5_T::PDMACTL: PDMARST Mask          */

#define SPI5_FIFOCTL_RXFBCLR_Pos         (0)                                               /*!< SPI5_T::FIFOCTL: RXFBCLR Position      */
#define SPI5_FIFOCTL_RXFBCLR_Msk         (0x1ul << SPI5_FIFOCTL_RXFBCLR_Pos)               /*!< SPI5_T::FIFOCTL: RXFBCLR Mask          */

#define SPI5_FIFOCTL_TXFBCLR_Pos         (1)                                               /*!< SPI5_T::FIFOCTL: TXFBCLR Position      */
#define SPI5_FIFOCTL_TXFBCLR_Msk         (0x1ul << SPI5_FIFOCTL_TXFBCLR_Pos)               /*!< SPI5_T::FIFOCTL: TXFBCLR Mask          */

#define SPI5_FIFOCTL_RXTHIEN_Pos         (2)                                               /*!< SPI5_T::FIFOCTL: RXTHIEN Position      */
#define SPI5_FIFOCTL_RXTHIEN_Msk         (0x1ul << SPI5_FIFOCTL_RXTHIEN_Pos)               /*!< SPI5_T::FIFOCTL: RXTHIEN Mask          */

#define SPI5_FIFOCTL_TXTHIEN_Pos         (3)                                               /*!< SPI5_T::FIFOCTL: TXTHIEN Position      */
#define SPI5_FIFOCTL_TXTHIEN_Msk         (0x1ul << SPI5_FIFOCTL_TXTHIEN_Pos)               /*!< SPI5_T::FIFOCTL: TXTHIEN Mask          */

#define SPI5_FIFOCTL_RXOVIEN_Pos         (4)                                               /*!< SPI5_T::FIFOCTL: RXOVIEN Position      */
#define SPI5_FIFOCTL_RXOVIEN_Msk         (0x1ul << SPI5_FIFOCTL_RXOVIEN_Pos)               /*!< SPI5_T::FIFOCTL: RXOVIEN Mask          */

#define SPI5_FIFOCTL_RXTOIEN_Pos         (7)                                               /*!< SPI5_T::FIFOCTL: RXTOIEN Position      */
#define SPI5_FIFOCTL_RXTOIEN_Msk         (0x1ul << SPI5_FIFOCTL_RXTOIEN_Pos)               /*!< SPI5_T::FIFOCTL: RXTOIEN Mask          */

#define SPI5_FIFOCTL_RXTH_Pos            (24)                                              /*!< SPI5_T::FIFOCTL: RXTH Position         */
#define SPI5_FIFOCTL_RXTH_Msk            (0x7ul << SPI5_FIFOCTL_RXTH_Pos)                  /*!< SPI5_T::FIFOCTL: RXTH Mask             */

#define SPI5_FIFOCTL_TXTH_Pos            (28)                                              /*!< SPI5_T::FIFOCTL: TXTH Position         */
#define SPI5_FIFOCTL_TXTH_Msk            (0x7ul << SPI5_FIFOCTL_TXTH_Pos)                  /*!< SPI5_T::FIFOCTL: TXTH Mask             */

/**@}*/ /* SPI5_CONST */
/**@}*/ /* end of SPI5 register group */

#endif /* __SPI5_REG_H__ */
