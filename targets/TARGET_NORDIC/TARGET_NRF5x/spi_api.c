#include "spi_api.h"
#include "nrf_drv_spi.h"
#include "nrf_drv_spis.h"
#include "nrf_drv_common.h"

#if DEVICE_SPI

#define SPI_INSTANCE_COUNT 3
#define INSTANCE_ID 0

static const nrf_drv_spi_t nordic_nrf5_spi_instance[3] = {NRF_DRV_SPI_INSTANCE(0), NRF_DRV_SPI_INSTANCE(1), NRF_DRV_SPI_INSTANCE(2)};

/* Current obj owner of the peripheral. By default the peripheral is configured at the beginning
   of each transaction. If the owner and mode hasn't changed the settings are kept as-is.
*/
static spi_t *nordic_nrf5_owner[3] = { NULL, NULL, NULL };

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    struct spi_s *spi_inst = obj;
    // Use the default config 
    nrf_drv_spi_config_t config = NRF_DRV_SPI_DEFAULT_CONFIG;
    config.sck_pin = sclk;
    config.mosi_pin = mosi;
    config.miso_pin = miso;
    config.ss_pin = ssel; 
    memcpy(&(spi_inst->config), &config, sizeof(nrf_drv_spi_config_t));
    /* TODO: use pin configuration to choose instance instead of hard coded value */
    int instance = 0;
    nrf_drv_spi_t spi_drv_inst = nordic_nrf5_spi_instance[instance];
    spi_inst->instance = instance;
    memcpy(&(spi_inst->spi_drv_inst), &spi_drv_inst, sizeof(nrf_drv_spi_t)); 
    
}

void spi_free(spi_t *obj)
{
    struct spi_s *spi_inst = obj;
    nrf_drv_spi_uninit(&(spi_inst->spi_drv_inst)); 
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    struct spi_s *spi_inst = obj;
    nrf_drv_spi_config_t *config = &(spi_inst->config);
    if(mode == 0)
        config->mode = NRF_DRV_SPI_MODE_0;
    else if(mode == 1)
        config->mode = NRF_DRV_SPI_MODE_1;
    else if(mode == 2)
        config->mode = NRF_DRV_SPI_MODE_2;
    else if(mode == 3)
        config->mode = NRF_DRV_SPI_MODE_3;
    int instance = spi_inst->instance;
    nordic_nrf5_owner[instance] = NULL;

    //TODO: bits???
}
         

void spi_frequency(spi_t *obj, int hz)
{
    struct spi_s *spi_inst = obj;
    nrf_drv_spi_config_t *config = &(spi_inst->config);
    nrf_drv_spi_frequency_t new_freq;
    if (hz < 250000) {
        new_freq = NRF_DRV_SPI_FREQ_125K;
    } else if (hz < 500000) {
        new_freq = NRF_DRV_SPI_FREQ_250K;
    } else if (hz < 1000000) {
        new_freq = NRF_DRV_SPI_FREQ_500K;
    } else if (hz < 2000000) {
        new_freq = NRF_DRV_SPI_FREQ_1M;
    } else if (hz < 4000000) {
        new_freq = NRF_DRV_SPI_FREQ_2M;
    } else if (hz < 8000000) {
        new_freq = NRF_DRV_SPI_FREQ_4M;
    } else {
        new_freq = NRF_DRV_SPI_FREQ_8M;
    }
    config->frequency = new_freq;
    int instance = spi_inst->instance;
    nordic_nrf5_owner[instance] = NULL;
}
         

int  spi_master_write(spi_t *obj, int value)
{
    struct spi_s *spi_obj = obj;
    const uint8_t tx_buff[1] = {(uint8_t) value};
    uint8_t rx_buff[1];
    int instance = spi_obj->instance;
    if(nordic_nrf5_owner[instance] != obj) {
        spi_free(obj);
        uint32_t err = nrf_drv_spi_init(&(spi_obj->spi_drv_inst), &(spi_obj->config), NULL, NULL); 
        nordic_nrf5_owner[instance] = obj;
    }
    nrf_drv_spi_transfer(&(spi_obj->spi_drv_inst), tx_buff, 1, rx_buff, 1);
    return rx_buff[0];
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, char write_fill)
{
    struct spi_s *spi_obj = obj;
    int instance = spi_obj->instance;
    if(nordic_nrf5_owner[instance] != obj) {
        spi_free(obj);
        uint32_t err = nrf_drv_spi_init(&(spi_obj->spi_drv_inst), &(spi_obj->config), NULL, NULL); 
        nordic_nrf5_owner[instance] = obj;
    }
    int max = (rx_length < tx_length) ? tx_length : rx_length;
    
    int tx_offset = 0;
    int rx_offset = 0;
    // The following code will write/read the data 255 bytes at a time
    // Check if the tx_length > 255
    while(tx_length > 255) {
        // Read all of rx_length or 255 
        int rx_read_ln = rx_length > 255 ? 255 : rx_length;
        nrf_drv_spi_transfer(&(spi_obj->spi_drv_inst), (const uint8_t*)(tx_buffer+tx_offset), 255, (uint8_t *)(rx_buffer+rx_offset), rx_read_ln);
        tx_offset += 255;
        tx_length -= 255;
        rx_length -= rx_read_ln;
        rx_offset += rx_read_ln;
    }
    // Handle extra tx/rx data
    if(tx_length > 0) {
        int rx_read_ln = rx_length > 255 ? 255 : rx_length;
        rx_offset = rx_read_ln > 0 ? rx_offset : 0;
        nrf_drv_spi_transfer(&(spi_obj->spi_drv_inst), (const uint8_t*)(tx_buffer+tx_offset), tx_length, (uint8_t*)(rx_buffer+rx_offset), rx_read_ln);
        rx_length -= rx_read_ln;
    }
    
    if (rx_length > 0) { 
        while(rx_length > 255){
            nrf_drv_spi_transfer(&(spi_obj->spi_drv_inst), (const uint8_t*)tx_buffer, 0, (uint8_t*)(rx_buffer+rx_offset), 255);
            rx_offset += 255;
            rx_length -= 255;
        }
        nrf_drv_spi_transfer(&(spi_obj->spi_drv_inst), (const uint8_t*)tx_buffer, 0, (uint8_t*)(rx_buffer+rx_offset), rx_length);
    }
    
    return max; 
} 

int  spi_slave_receive(spi_t *obj)
{
    return 0;
}

int  spi_slave_read(spi_t *obj)
{
    return 0;
}

void spi_slave_write(spi_t *obj, int value)
{
    return;
}

int  spi_busy(spi_t *obj)
{
    struct spi_s *spi_obj = obj;
    return nrf_drv_spi_busy(&(spi_obj->spi_drv_inst)); 
}

uint8_t spi_get_module(spi_t *obj)
{
    return 0;
}


#if DEVICE_SPI_ASYNCH

void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint);

uint32_t spi_irq_handler_asynch(spi_t *obj);

uint8_t spi_active(spi_t *obj);

void spi_abort_asynch(spi_t *obj);

#endif // DEVICE_SPI_ASYNCH

#endif // DEVICE_SPI
