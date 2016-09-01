    .syntax unified
    .arch armv8-m.main

/* Memory Model
   The HEAP starts at the end of the DATA section and grows upward.

   The STACK starts at the end of the RAM and grows downward.

   The HEAP and stack STACK are only checked at compile time:
   (DATA_SIZE + HEAP_SIZE + STACK_SIZE) < RAM_SIZE

   This is just a check for the bare minimum for the Heap+Stack area before
   aborting compilation, it is not the run time limit:
   Heap_Size + Stack_Size = 0x80 + 0x80 = 0x100
 */
    .section .stack
    .align 3
#ifdef __STACK_SIZE
    .equ    Stack_Size, __STACK_SIZE
#else
    .equ    Stack_Size, 0x400
#endif
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    Stack_Size
    .size __StackLimit, . - __StackLimit
__StackTop:
    .size __StackTop, . - __StackTop

    .section .heap
    .align 3
#ifdef __HEAP_SIZE
    .equ    Heap_Size, __HEAP_SIZE
#else
    .equ    Heap_Size, 0xC00
#endif
    .globl    __HeapBase
    .globl    __HeapLimit
__HeapBase:
    .space    Heap_Size
    .size __HeapBase, . - __HeapBase
__HeapLimit:
    .size __HeapLimit, . - __HeapLimit

    .section .isr_vector,"a",%progbits
    .align 2
    .globl __isr_vector
__isr_vector:
    .long    __StackTop                 /* Top of Stack */
    .long    Reset_Handler              /* Reset Handler */
    .long    NMI_Handler                /* NMI Handler */
    .long    HardFault_Handler          /* Hard Fault Handler */
    .long    MemManage_Handler          /* MPU Fault Handler */
    .long    BusFault_Handler           /* Bus Fault Handler */
    .long    UsageFault_Handler         /* Usage Fault Handler */
    .long    SecureFault_Handler        /* Secure Fault Handler */
    .long    0                          /* Reserved */
    .long    0                          /* Reserved */
    .long    0                          /* Reserved */
    .long    SVC_Handler                /* SVCall Handler */
    .long    DebugMon_Handler           /* Debug Monitor Handler */
    .long    0                          /* Reserved */
    .long    PendSV_Handler             /* PendSV Handler */
    .long    SysTick_Handler            /* SysTick Handler */

    /* External interrupts */
    .long    UART0RX_Handler            /*  0 UART 0 receive interrupt */
    .long    UART0TX_Handler            /*  1 UART 0 transmit interrupt */
    .long    UART1RX_Handler            /*  2 UART 1 receive interrupt */
    .long    UART1TX_Handler            /*  3 UART 1 transmit interrupt */
    .long    UART2RX_Handler            /*  4 UART 2 receive interrupt */
    .long    UART2TX_Handler            /*  5 UART 2 transmit interrupt */
    .long    GPIO0ALL_Handler           /*  6 GPIO 0 combined interrupt */
    .long    GPIO1ALL_Handler           /*  7 GPIO 1 combined interrupt */
    .long    TIMER0_Handler             /*  8 Timer 0 interrupt */
    .long    TIMER1_Handler             /*  9 Timer 1 interrupt */
    .long    DUALTIMER_Handler          /* 10 Dual Timer interrupt */
    .long    SPI_0_1_Handler            /* 11 SPI #0, #1 interrupt */
    .long    UART_0_1_2_OVF_Handler     /* 12 UART overflow (0, 1 & 2) interrupt */
    .long    ETHERNET_Handler           /* 13 Ethernet interrupt */
    .long    I2S_Handler                /* 14 Audio I2S interrupt */
    .long    TOUCHSCREEN_Handler        /* 15 Touch Screen interrupt */
    .long    GPIO2_Handler              /* 16 GPIO 2 combined interrupt */
    .long    GPIO3_Handler              /* 17 GPIO 3 combined interrupt */
    .long    UART3RX_Handler            /* 18 UART 3 receive interrupt */
    .long    UART3TX_Handler            /* 19 UART 3 transmit interrupt */
    .long    UART4RX_Handler            /* 20 UART 4 receive interrupt */
    .long    UART4TX_Handler            /* 21 UART 4 transmit interrupt */
    .long    SPI_2_Handler              /* 22 SPI #2 interrupt */
    .long    SPI_3_4_Handler            /* 23 SPI #3, SPI #4 interrupt */
    .long    GPIO0_0_Handler            /* 24 GPIO 0 individual interrupt ( 0) */
    .long    GPIO0_1_Handler            /* 25 GPIO 0 individual interrupt ( 1) */
    .long    GPIO0_2_Handler            /* 26 GPIO 0 individual interrupt ( 2) */
    .long    GPIO0_3_Handler            /* 27 GPIO 0 individual interrupt ( 3) */
    .long    GPIO0_4_Handler            /* 28 GPIO 0 individual interrupt ( 4) */
    .long    GPIO0_5_Handler            /* 29 GPIO 0 individual interrupt ( 5) */
    .long    GPIO0_6_Handler            /* 30 GPIO 0 individual interrupt ( 6) */
    .long    GPIO0_7_Handler            /* 31 GPIO 0 individual interrupt ( 7) */
    .long    GPIO1_0_Handler            /* 32 GPIO 1 individual interrupt ( 0) */
    .long    GPIO1_1_Handler            /* 33 GPIO 1 individual interrupt ( 1) */
    .long    GPIO1_2_Handler            /* 34 GPIO 1 individual interrupt ( 2) */
    .long    GPIO1_3_Handler            /* 35 GPIO 1 individual interrupt ( 3) */
    .long    GPIO1_4_Handler            /* 36 GPIO 1 individual interrupt ( 4) */
    .long    GPIO1_5_Handler            /* 37 GPIO 1 individual interrupt ( 5) */
    .long    GPIO1_6_Handler            /* 38 GPIO 1 individual interrupt ( 6) */
    .long    GPIO1_7_Handler            /* 39 GPIO 1 individual interrupt ( 7) */
    .long    GPIO1_8_Handler            /* 40 GPIO 1 individual interrupt ( 0) */
    .long    GPIO1_9_Handler            /* 41 GPIO 1 individual interrupt ( 9) */
    .long    GPIO1_10_Handler           /* 42 GPIO 1 individual interrupt (10) */
    .long    GPIO1_11_Handler           /* 43 GPIO 1 individual interrupt (11) */
    .long    GPIO1_12_Handler           /* 44 GPIO 1 individual interrupt (12) */
    .long    GPIO1_13_Handler           /* 45 GPIO 1 individual interrupt (13) */
    .long    GPIO1_14_Handler           /* 46 GPIO 1 individual interrupt (14) */
    .long    GPIO1_15_Handler           /* 47 GPIO 1 individual interrupt (15) */
    .long    SPI_0B_Handler             /* 48 SPI #0 interrupt */
    .long    0                          /* 49 Reserved */
    .long    SECURETIMER0_Handler       /* 50 Secure Timer 0 interrupt */
    .long    SECURETIMER1_Handler       /* 51 Secure Timer 1 interrupt */
    .long    SPI_1B_Handler             /* 52 SPI #1 interrupt */
    .long    SPI_2B_Handler             /* 53 SPI #2 interrupt */
    .long    SPI_3B_Handler             /* 54 SPI #3 interrupt */
    .long    SPI_4B_Handler             /* 55 SPI #4 interrupt */

    .size    __isr_vector, . - __isr_vector

    .section .text.Reset_Handler
    .thumb
    .thumb_func
    .align  2
    .globl   Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
    ldr    r0, =SystemInit
    blx    r0
    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

.LC0:
    cmp     r2, r3
    ittt    lt
    ldrlt   r0, [r1], #4
    strlt   r0, [r2], #4
    blt    .LC0

#if defined(FEATURE_UVISOR) && defined(TARGET_UVISOR_SUPPORTED)
    ldr r0, =uvisor_init
    blx r0
#endif /* defined(FEATURE_UVISOR) && defined(TARGET_UVISOR_SUPPORTED) */

    ldr    r0, =_start
    bx    r0

    .pool
    .size Reset_Handler, . - Reset_Handler

    .text
/*
 * Macro to define default handlers. Default handler
 * will be weak symbol and just dead loops. They can be
 * overwritten by other handlers
 */
    .macro    def_default_handler    handler_name
    .align 1
    .thumb_func
    .weak    \handler_name
    .type    \handler_name, %function
\handler_name :
    b    .
    .size    \handler_name, . - \handler_name
    .endm

    def_default_handler    NMI_Handler
    def_default_handler    HardFault_Handler
    def_default_handler    MemManage_Handler
    def_default_handler    BusFault_Handler
    def_default_handler    UsageFault_Handler
    def_default_handler    SecureFault_Handler
    def_default_handler    SVC_Handler
    def_default_handler    DebugMon_Handler
    def_default_handler    PendSV_Handler
    def_default_handler    SysTick_Handler
    def_default_handler    Default_Handler

    .macro    def_irq_default_handler    handler_name
    .weak     \handler_name
    .set      \handler_name, Default_Handler
    .endm

    /* External interrupts */
    def_irq_default_handler     UARTRX0_Handler
    def_irq_default_handler     UART0RX_Handler
    def_irq_default_handler     UART0TX_Handler
    def_irq_default_handler     UART1RX_Handler
    def_irq_default_handler     UART1TX_Handler
    def_irq_default_handler     UART2RX_Handler
    def_irq_default_handler     UART2TX_Handler
    def_irq_default_handler     GPIO0ALL_Handler
    def_irq_default_handler     GPIO1ALL_Handler
    def_irq_default_handler     TIMER0_Handler
    def_irq_default_handler     TIMER1_Handler
    def_irq_default_handler     DUALTIMER_Handler
    def_irq_default_handler     SPI_0_1_Handler
    def_irq_default_handler     UART_0_1_2_OVF_Handler
    def_irq_default_handler     ETHERNET_Handler
    def_irq_default_handler     I2S_Handler
    def_irq_default_handler     TOUCHSCREEN_Handler
    def_irq_default_handler     GPIO2_Handler
    def_irq_default_handler     GPIO3_Handler
    def_irq_default_handler     UART3RX_Handler
    def_irq_default_handler     UART3TX_Handler
    def_irq_default_handler     UART4RX_Handler
    def_irq_default_handler     UART4TX_Handler
    def_irq_default_handler     SPI_2_Handler
    def_irq_default_handler     SPI_3_4_Handler
    def_irq_default_handler     GPIO0_0_Handler
    def_irq_default_handler     GPIO0_1_Handler
    def_irq_default_handler     GPIO0_2_Handler
    def_irq_default_handler     GPIO0_3_Handler
    def_irq_default_handler     GPIO0_4_Handler
    def_irq_default_handler     GPIO0_5_Handler
    def_irq_default_handler     GPIO0_6_Handler
    def_irq_default_handler     GPIO0_7_Handler
    def_irq_default_handler     GPIO1_0_Handler
    def_irq_default_handler     GPIO1_1_Handler
    def_irq_default_handler     GPIO1_2_Handler
    def_irq_default_handler     GPIO1_3_Handler
    def_irq_default_handler     GPIO1_4_Handler
    def_irq_default_handler     GPIO1_5_Handler
    def_irq_default_handler     GPIO1_6_Handler
    def_irq_default_handler     GPIO1_7_Handler
    def_irq_default_handler     GPIO1_8_Handler
    def_irq_default_handler     GPIO1_9_Handler
    def_irq_default_handler     GPIO1_10_Handler
    def_irq_default_handler     GPIO1_11_Handler
    def_irq_default_handler     GPIO1_12_Handler
    def_irq_default_handler     GPIO1_13_Handler
    def_irq_default_handler     GPIO1_14_Handler
    def_irq_default_handler     GPIO1_15_Handler
    def_irq_default_handler     SPI_0B_Handler
    def_irq_default_handler     SECURETIMER0_Handler
    def_irq_default_handler     SECURETIMER1_Handler
    def_irq_default_handler     SPI_1B_Handler
    def_irq_default_handler     SPI_2B_Handler
    def_irq_default_handler     SPI_3B_Handler
    def_irq_default_handler     SPI_4B_Handler

    .end
