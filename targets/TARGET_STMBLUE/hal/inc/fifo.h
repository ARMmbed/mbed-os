/**
  ******************************************************************************
  * @file    fifo.h 
  * @author  VMA RF Application Team
  * @version V1.0.0
  * @date    July-2015
  * @brief   
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

#include <stdint.h>
#ifndef __FIFO_H__
#define __FIFO_H__

typedef struct circular_fifo_s {
  uint16_t tail;
  uint16_t head;
  uint16_t max_size;
  uint8_t *buffer;
  uint8_t alignment;
} circular_fifo_t;

void fifo_init(circular_fifo_t *fifo, uint16_t max_size, uint8_t  *buffer, uint8_t alignment);
uint16_t fifo_size(circular_fifo_t *fifo);
uint8_t fifo_put(circular_fifo_t *fifo, uint16_t size, uint8_t  *buffer);
uint8_t fifo_put_var_len_item(circular_fifo_t *fifo, uint16_t size, uint8_t  *buffer);
uint8_t fifo_get(circular_fifo_t *fifo, uint16_t size, uint8_t  *buffer);
uint8_t fifo_discard(circular_fifo_t *fifo, uint16_t size);
uint8_t fifo_get_ptr(circular_fifo_t *fifo, uint16_t size, uint8_t **ptr);
uint8_t fifo_get_var_len_item(circular_fifo_t *fifo, uint16_t *size, uint8_t  *buffer);
uint8_t fifo_get_ptr_var_len_item(circular_fifo_t *fifo, uint16_t *size, uint8_t  **ptr);
uint8_t fifo_discard_var_len_item(circular_fifo_t *fifo);
void fifo_flush(circular_fifo_t *fifo);
#endif /* __FIFO_H__ */
