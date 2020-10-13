/*******************************************************************************
* DISCLAIMER
*******************************************************************************/
/*******************************************************************************
* File Name : sdhi_iodefine.h
* $Rev: $
* $Date::                           $
* Description : Definition of I/O Register for RZ/A1
******************************************************************************/
#ifndef SDHI_IODEFINE_H
#define SDHI_IODEFINE_H
/* ->QAC 0639 : Over 127 members (C90) */
/* ->QAC 0857 : Over 1024 #define (C90) */
/* ->MISRA 18.4 : Pack unpack union */ /* ->SEC M1.6.2 */
/* ->SEC M1.10.1 : Not magic number */

#define SDHI0 (*(struct st_sdhi  *)0xE804E000uL) /* SDHI0 */
#define SDHI1 (*(struct st_sdhi  *)0xE804E800uL) /* SDHI1 */


/* Start of channel array defines of SDHI */

/* Channel array defines of SDHI */
/*(Sample) value = SDHI[ channel ]->SD_CMD; */
#define SDHI_COUNT  (2)
#define SDHI_ADDRESS_LIST \
{   /* ->MISRA 11.3 */ /* ->SEC R2.7.1 */ \
    &SDHI0 \
    &SDHI1 \
}   /* <-MISRA 11.3 */ /* <-SEC R2.7.1 */ /* { } is for MISRA 19.4 */

/* End of channel array defines of SDHI */


#define SD_CMD_0           (SDHI0.SD_CMD)
#define SD_ARG0_0          (SDHI0.SD_ARG0)
#define SD_ARG1_0          (SDHI0.SD_ARG1)
#define SD_STOP_0          (SDHI0.SD_STOP)
#define SD_SECCNT_0        (SDHI0.SD_SECCNT)
#define SD_RSP00_0         (SDHI0.SD_RSP00)
#define SD_RSP01_0         (SDHI0.SD_RSP01)
#define SD_RSP02_0         (SDHI0.SD_RSP02)
#define SD_RSP03_0         (SDHI0.SD_RSP03)
#define SD_RSP04_0         (SDHI0.SD_RSP04)
#define SD_RSP05_0         (SDHI0.SD_RSP05)
#define SD_RSP06_0         (SDHI0.SD_RSP06)
#define SD_RSP07_0         (SDHI0.SD_RSP07)
#define SD_INFO1_0         (SDHI0.SD_INFO1)
#define SD_INFO2_0         (SDHI0.SD_INFO2)
#define SD_INFO1_MASK_0    (SDHI0.SD_INFO1_MASK)
#define SD_INFO2_MASK_0    (SDHI0.SD_INFO2_MASK)
#define SD_CLK_CTRL_0      (SDHI0.SD_CLK_CTRL)
#define SD_SIZE_0          (SDHI0.SD_SIZE)
#define SD_OPTION_0        (SDHI0.SD_OPTION)
#define SD_ERR_STS1_0      (SDHI0.SD_ERR_STS1)
#define SD_ERR_STS2_0      (SDHI0.SD_ERR_STS2)
#define SD_BUF0_0          (SDHI0.SD_BUF0)
#define SDIO_MODE_0        (SDHI0.SDIO_MODE)
#define SDIO_INFO1_0       (SDHI0.SDIO_INFO1)
#define SDIO_INFO1_MASK_0  (SDHI0.SDIO_INFO1_MASK)
#define CC_EXT_MODE_0      (SDHI0.CC_EXT_MODE)
#define SOFT_RST_0         (SDHI0.SOFT_RST)
#define VERSION_0          (SDHI0.VERSION)
#define EXT_SWAP_0         (SDHI0.EXT_SWAP)

#define SD_CMD_1           (SDHI1.SD_CMD)
#define SD_ARG0_1          (SDHI1.SD_ARG0)
#define SD_ARG1_1          (SDHI1.SD_ARG1)
#define SD_STOP_1          (SDHI1.SD_STOP)
#define SD_SECCNT_1        (SDHI1.SD_SECCNT)
#define SD_RSP00_1         (SDHI1.SD_RSP00)
#define SD_RSP01_1         (SDHI1.SD_RSP01)
#define SD_RSP02_1         (SDHI1.SD_RSP02)
#define SD_RSP03_1         (SDHI1.SD_RSP03)
#define SD_RSP04_1         (SDHI1.SD_RSP04)
#define SD_RSP05_1         (SDHI1.SD_RSP05)
#define SD_RSP06_1         (SDHI1.SD_RSP06)
#define SD_RSP07_1         (SDHI1.SD_RSP07)
#define SD_INFO1_1         (SDHI1.SD_INFO1)
#define SD_INFO2_1         (SDHI1.SD_INFO2)
#define SD_INFO1_MASK_1    (SDHI1.SD_INFO1_MASK)
#define SD_INFO2_MASK_1    (SDHI1.SD_INFO2_MASK)
#define SD_CLK_CTRL_1      (SDHI1.SD_CLK_CTRL)
#define SD_SIZE_1          (SDHI1.SD_SIZE)
#define SD_OPTION_1        (SDHI1.SD_OPTION)
#define SD_ERR_STS1_1      (SDHI1.SD_ERR_STS1)
#define SD_ERR_STS2_1      (SDHI1.SD_ERR_STS2)
#define SD_BUF0_1          (SDHI1.SD_BUF0)
#define SDIO_MODE_1        (SDHI1.SDIO_MODE)
#define SDIO_INFO1_1       (SDHI1.SDIO_INFO1)
#define SDIO_INFO1_MASK_1  (SDHI1.SDIO_INFO1_MASK)
#define CC_EXT_MODE_1      (SDHI1.CC_EXT_MODE)
#define SOFT_RST_1         (SDHI1.SOFT_RST)
#define VERSION_1          (SDHI1.VERSION)
#define EXT_SWAP_1         (SDHI1.EXT_SWAP)

typedef struct st_sdhi
{
                                                               /* SDHI             */
    volatile uint16_t  SD_CMD;                                 /*  SD_CMD          */
    volatile uint8_t   dummy1[2];                              /*                  */
    volatile uint16_t  SD_ARG0;                                /*  SSLDR           */
    volatile uint16_t  SD_ARG1;                                /*  SSLDR           */
    volatile uint16_t  SD_STOP;                                /*  SPBCR           */
    volatile uint16_t  SD_SECCNT;                              /*  SPBCR           */
    volatile uint16_t  SD_RSP00;                               /*  SPBCR           */
    volatile uint16_t  SD_RSP01;                               /*  SPBCR           */
    volatile uint16_t  SD_RSP02;                               /*  SPBCR           */
    volatile uint16_t  SD_RSP03;                               /*  SPBCR           */
    volatile uint16_t  SD_RSP04;                               /*  SPBCR           */
    volatile uint16_t  SD_RSP05;                               /*  SPBCR           */
    volatile uint16_t  SD_RSP06;                               /*  SPBCR           */
    volatile uint16_t  SD_RSP07;                               /*  SPBCR           */
    volatile uint16_t  SD_INFO1;                               /*  SPBCR           */
    volatile uint16_t  SD_INFO2;                               /*  SPBCR           */
    volatile uint16_t  SD_INFO1_MASK;                          /*  SPBCR           */
    volatile uint16_t  SD_INFO2_MASK;                          /*  SPBCR           */
    volatile uint16_t  SD_CLK_CTRL;                            /*  SPBCR           */
    volatile uint16_t  SD_SIZE;                                /*  SPBCR           */
    volatile uint16_t  SD_OPTION;                              /*  SPBCR           */
    volatile uint8_t   dummy2[2];                              /*                  */
    volatile uint16_t  SD_ERR_STS1;                            /*  SPBCR           */
    volatile uint16_t  SD_ERR_STS2;                            /*  SPBCR           */
    volatile uint32_t  SD_BUF0;                                /*  DRCR            */
    volatile uint16_t  SDIO_MODE;                              /*  DRCMR           */
    volatile uint16_t  SDIO_INFO1;                             /*  DREAR           */
    volatile uint16_t  SDIO_INFO1_MASK;                        /*  DROPR           */
    volatile uint8_t   dummy3[158];                            /*                  */
    volatile uint16_t  CC_EXT_MODE;                            /*  DRENR           */
    volatile uint8_t   dummy4[6];                              /*                  */
    volatile uint16_t  SOFT_RST;                               /*  SMCR            */
    volatile uint16_t  VERSION;                                /*  SMCMR           */
    volatile uint8_t   dummy5[12];                             /*                  */
    volatile uint16_t  EXT_SWAP;                               /*  SMADR           */
} r_io_sdhi_t;


/* Channel array defines of SDHI (2)*/
#ifdef  DECLARE_SDHI_CHANNELS
volatile struct st_sdhi*  SDHI[ SDHI_COUNT ] =
    /* ->MISRA 11.3 */ /* ->SEC R2.7.1 */
    SDHI_ADDRESS_LIST;
    /* <-MISRA 11.3 */ /* <-SEC R2.7.1 */
#endif  /* DECLARE_SPIBSC_CHANNELS */
/* End of channel array defines of SDHI (2)*/


/* <-SEC M1.10.1 */
/* <-MISRA 18.4 */ /* <-SEC M1.6.2 */
/* <-QAC 0857 */
/* <-QAC 0639 */
#endif

