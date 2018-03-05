#include <string.h>
#include "stm32f4xx_hal.h"
#include "mbed_toolchain.h"

#define C029_OTP_START_ADDRESS           (0x1FFF7800U)
#define C029_OTP_END_ADDRESS             (C029_OTP_START_ADDRESS + (16*32))
#define C029_MAC_ETHERNET_ID             (3)

typedef MBED_PACKED(struct) C029_OTP_Header {
    uint8_t id;
    uint8_t len;
    uint8_t data[];
} C029_OTP_Header;

static int _macRetrieved = 0;
static char _macAddr[6] = { 0x02, 0x02, 0xF7, 0xF0, 0x00, 0x00 };

static C029_OTP_Header *increment(C029_OTP_Header *pTemp)
{
    uint8_t len = 0;
    uint8_t id = 0;
    uint8_t *p = (uint8_t*)pTemp;

    memcpy((void*)&id, (void*)pTemp, 1);

    if (id == 0xFF){
        p++;
    } else {
        p++;
        memcpy((void*)&len, (void*)p++, 1);
        p += len;
    }
    return (C029_OTP_Header*)p;
}

/**
 * Override HAL Eth Init function
 */
void HAL_ETH_MspInit(ETH_HandleTypeDef* heth)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    if (heth->Instance == ETH) {

        /* Enable GPIOs clocks */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();

        /** ETH GPIO Configuration
          RMII_REF_CLK ----------------------> PA1
          RMII_MDIO -------------------------> PA2
          RMII_MDC --------------------------> PC1
          RMII_MII_CRS_DV -------------------> PA7
          RMII_MII_RXD0 ---------------------> PC4
          RMII_MII_RXD1 ---------------------> PC5
          RMII_MII_RXER ---------------------> PG2
          RMII_MII_TX_EN --------------------> PB11
          RMII_MII_TXD0 ---------------------> PB12
          RMII_MII_TXD1 ---------------------> PB13
         */
        /* Configure PA1, PA2 and PA7 */
        GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructure.Pull = GPIO_PULLUP;
        GPIO_InitStructure.Pin = GPIO_PIN_2 | GPIO_PIN_7;
        GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.Pull = GPIO_NOPULL;
        GPIO_InitStructure.Pin = GPIO_PIN_1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* Configure PB13 */
        GPIO_InitStructure.Pin = GPIO_PIN_13 | GPIO_PIN_11 | GPIO_PIN_12;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

        /* Configure PC1, PC4 and PC5 */
        GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

        /* Enable the Ethernet global Interrupt */
        HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
        HAL_NVIC_EnableIRQ(ETH_IRQn);

        /* Enable ETHERNET clock  */
        __HAL_RCC_ETH_CLK_ENABLE();
    }
}

/**
 * Override HAL Eth DeInit function
 */
void HAL_ETH_MspDeInit(ETH_HandleTypeDef* heth)
{
    if (heth->Instance == ETH) {
        /* Peripheral clock disable */
        __HAL_RCC_ETH_CLK_DISABLE();

        /** ETH GPIO Configuration
          RMII_REF_CLK ----------------------> PA1
          RMII_MDIO -------------------------> PA2
          RMII_MDC --------------------------> PC1
          RMII_MII_CRS_DV -------------------> PA7
          RMII_MII_RXD0 ---------------------> PC4
          RMII_MII_RXD1 ---------------------> PC5
          RMII_MII_RXER ---------------------> PG2
          RMII_MII_TX_EN --------------------> PB11
          RMII_MII_TXD0 ---------------------> PB12
          RMII_MII_TXD1 ---------------------> PB13
         */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7);
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13 | GPIO_PIN_11 | GPIO_PIN_12);
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5);

        /* Disable the Ethernet global Interrupt */
        NVIC_DisableIRQ(ETH_IRQn);
    }
}

uint8_t mbed_otp_mac_address(char *mac)
{
    C029_OTP_Header *pFound = NULL;
    C029_OTP_Header *pTemp = (C029_OTP_Header*)C029_OTP_START_ADDRESS;
    C029_OTP_Header temp;

    if (_macRetrieved == 0) {
        while ((pTemp >= (C029_OTP_Header*)C029_OTP_START_ADDRESS) && (pTemp < (C029_OTP_Header*)C029_OTP_END_ADDRESS)){
            memcpy((void*)&temp, (void*)pTemp, sizeof(temp));
            if (temp.id == C029_MAC_ETHERNET_ID){
                pFound = pTemp;
                break;
            }
            pTemp = increment(pTemp);
        }
        if (pFound != NULL) {
            memcpy(_macAddr, pFound->data, 6);
            _macRetrieved = 1;
        }
    }
    memcpy(mac, _macAddr, 6);

    return 1;
}
