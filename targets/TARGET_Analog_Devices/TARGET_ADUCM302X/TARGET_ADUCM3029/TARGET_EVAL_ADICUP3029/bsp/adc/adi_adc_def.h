/*! \cond PRIVATE */

#ifndef ADI_ADC_DEF
#define ADI_ADC_DEF

#include <drivers/adc/adi_adc.h>
#include <adi_processor.h>

#if defined(__ECC__)
#define ALIGN
#define ALIGN4  _Pragma("align(4)")
#elif defined(__ICCARM__)
#define ALIGN   _Pragma("pack()")
#define ALIGN4  _Pragma("pack(4)")
#elif defined (__GNUC__)
#define ALIGN   _Pragma("pack()")
#define ALIGN4  _Pragma("pack(4)")
#endif


#define IS_IN_STATE(X)         ((pDevice->nDriverStatus & (uint32_t)(X)) == (uint32_t)(X))
#define IS_NOT_IN_STATE(X)     ((pDevice->nDriverStatus & (uint32_t)(X)) == 0u)
#define IS_IN_ALL_STATES(X)    ((pDevice->nDriverStatus & (uint32_t)(X)) == (uint32_t)(X))
#define IS_IN_ANY_STATE(X)     ((pDevice->nDriverStatus & (uint32_t)(X)) != 0u)
#define IS_NOT_IN_ANY_STATE(X) ((pDevice->nDriverStatus & (uint32_t)(X)) == 0u)

#define SET_STATE(X)  (pDevice->nDriverStatus |= (uint32_t)(X))
#define CLR_STATE(X)  (pDevice->nDriverStatus &= ~((uint32_t)(X)))

#define NUM_ADC_CHANNELS      			(8u)
#define NUM_ADC_COMPARATOR_CHANNELS             (4u)

/* To keep state for the driver for error checking */
typedef enum __ADC_STATUS {
    ADC_STATUS_POWERED_UP            = (1u << 0),
    ADC_STATUS_SUB_SYSTEM_EN         = (1u << 1),
    ADC_STATUS_SUB_SYSTEM_READY      = (1u << 2),
    
    ADC_STATUS_NON_BLOCKING_EN       = (1u << 3),
    ADC_STATUS_BLOCKING_EN           = (1u << 4), 
    ADC_STATUS_COMPARATOR_EN         = (1u << 5),
    
    ADC_STATUS_SAMPLING_IN_PROGRESS  = (1u << 6),
    ADC_STATUS_CALIBRATION_EN        = (1u << 7),
    ADC_STATUS_CALIBRATION_DONE      = (1u << 8),
    
    ADC_STATUS_BATTERY_DONE          = (1u << 9),
    
    ADC_STATUS_OVERSAMPLING_EN       = (1u << 10),
    ADC_STATUS_AVGERAGING_EN         = (1u << 11),
    
    ADC_STATUS_TEMP_SENSOR_EN        = (1u << 12),
    
    ADC_STATUS_TMP_DONE              = (1u << 13),
    ADC_STATUS_TMP2_DONE             = (1u << 14),
} ADC_STATUS;

typedef enum __ADC_FIFO_MODE {
    ADC_FIFO_MODE_INIT,
    ADC_FIFO_MODE_ENABLED,
    ADC_FIFO_MODE_INTERRUPT_PROCESS,
    ADC_FIFO_MODE_INTERRUPT_OVERFLOW,
    ADC_FIFO_MODE_DMA_BUFFER_PROCESS,
    ADC_FIFO_MODE_DMA_INVALID_DESC,
    ADC_FIFO_MODE_ABORT
} ADC_FIFO_MODE;

typedef enum __ADC_BUFFER_CONFIG {
    ADC_BUFFER_CONFIG_BUFFER_SINGLE_CONV_EN = ((uint32_t)1u << 1u),
    ADC_BUFFER_CONFIG_BUFFER_AUTO_MODE_EN   = ((uint32_t)1u << 0u),
} ADC_BUFFER_CONFIG;


typedef enum __ADC_BUFFER_STATUS {
    ADC_BUFFER_STATUS_OK        = ((uint32_t)1u << 0u),
    ADC_BUFFER_STATUS_OVERFLOW  = ((uint32_t)1u << 1u)
} ADC_BUFFER_STATUS;

typedef struct __ADC_INT_BUFFER {
    uint16_t                  nConfig;
    uint16_t                  nStatus;
    ADI_ADC_BUFFER           *pUserBuffer;
    uint16_t*                 pCurDataBuffer;
    uint32_t                  nNumSamplesRemaining;
    uint32_t                  nChannels;
} ADC_INT_BUFFER;

typedef struct __ADC_ACTIVE_DATA {
    uint32_t nCurChannel;
} ADC_ACTIVE_DATA;

typedef ADI_ADC_RESULT (*ADC_MANAGE_FIFO_FUNC)(struct __ADI_ADC_DEVICE *pDevice, ADC_FIFO_MODE eFifoMode);

typedef struct __ADI_ADC_DEVICE
{
    volatile uint32_t    nDriverStatus;
    ADI_ADC_TypeDef      *pReg;
    void*                pCBParam;
    ADI_CALLBACK         pfCallback;

    ADC_ACTIVE_DATA      ActData;
    ADC_MANAGE_FIFO_FUNC pfManageFifo;

    ADC_INT_BUFFER       s_Buffer;
    uint8_t              ComparitorHi;
    uint8_t              ComparitorLo;
    uint8_t              ComparitorHys;
    
    SEM_VAR_DECLR
} ADI_ADC_DEVICE;

typedef struct __ADI_ADC_INFO
{
    ADI_ADC_HANDLE     hDevice;
    ADI_ADC_TypeDef*   pReg;
} ADI_ADC_INFO;

#endif /* ADI_ADC_DEF */

/*! \endcond */
