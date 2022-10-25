/**
 * @file    circular_module.c
 * @author  StarLine Busorgin_S
 * @date    24 Oct 2022
 * @version 1.0.0
 * @brief   Circular buffer API and UART send data from buffer
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "circular_module.h"
#include "circular_module_config.h"

/* DEFINES -------------------------------------------------------------------*/
#define TRUE    1U
#define FALSE   0U

/* Private variables ---------------------------------------------------------*/
static uart_send_handler_t m_uart_send_callback;

static uint8_t m_synch_flag = TRUE;

/* Private function prototypes -----------------------------------------------*/

/* Public functions ---------------------------------------------------------*/
void put_byte_to_buffer(uint8_t databyte, circular_buffer_t *buf)
{
    uint8_t start;
    uint8_t end;
    uint8_t mutex = buf->mutex;

    while(mutex == TRUE){
        mutex = buf->mutex;
    }
    buf->mutex = TRUE; /**< Захват мьютекса */
    buf->buffer[buf->end++] = databyte;
    buf->end %= buf->size; /**< Как только индекс конца буфера превысит макс.
                            индек массива m_buffer, значение m_end обнулится */
    if(buf->end == buf->start){
        buf->start++;
        buf->start %= buf->size; /**< Как только индекс начала буфера превысит
                    макс. индек массива m_buffer, значение m_start обнулится */
    }

    /**< Как только буфер получает первый байт данных, вызывается callback
        отправки по UART. Если буфер заполняется быстрее, чем выполняется
        отпрравка по UART, то последующая передача инициируется из
        обработчика прерывания UART */
    if(m_synch_flag == TRUE){
        m_synch_flag = FALSE;
        if(m_uart_send_callback != 0){
            buf->start++;
            buf->start %= buf->size; /**< Как только индекс начала буфера превысит
                    макс. индек массива m_buffer, значение m_start обнулится */
            buf->mutex = FALSE; /**< Возврат мьютекса */
            m_uart_send_callback(databyte);
        }
    }
}

uint8_t pop_byte_from_buffer(circular_buffer_t *buf)
{
    uint8_t mutex = buf->mutex;

    while(mutex == TRUE){
        mutex = buf->mutex;
    }

    buf->mutex = TRUE; /**< Захват мьютекса */
    uint8_t databyte = buf->buffer[buf->start++];
    buf->start %= buf->size; /**< Как только индекс начала буфера 
    превысит макс. индек массива m_buffer, значение m_start обнулится */

    if(buf->end == buf->start){
        m_synch_flag = TRUE;
    }
    buf->mutex = FALSE; /**< Возврат мьютекса */
    return databyte;
}

buff_err_status_t buffer_init(circular_buffer_t *buf, uint8_t *buf_array, uint16_t size,
                              uart_send_handler_t send_handler)
{
    m_uart_send_callback = send_handler;
    buf->buffer = buf_array;
    buf->mutex = 0;
    buf->size = size;
    buf->end = 0;
    buf->start = 0;

    for(uint8_t i = 0; i < size; i++){
        buf_array[i] = 0;
    }
    return (buf->buffer ? NO_ERR_BUFF : ERR_BUFF);
}

void uart_init(uart_init_handler_t uart_init_handler)
{
    uart_init_t const uart_param = {
        .baudrate =    BAUDRATE,
        .worldlenght = WORDLENGHT,
        .stopbits =    STOPBITS,
        .parity =      PARITY
    };
    uart_init_handler(uart_param);
}

void uart_event_transmit(circular_buffer_t *buf)
{
    uint8_t databyte;
    if((buf->mutex == FALSE) && (buf->end != buf->start)){
        if(m_uart_send_callback != 0){
            databyte = buf->buffer[buf->start++];
            buf->start %= buf->size; /**< Как только индекс начала буфера 
            превысит макс. индек массива m_buffer, значение m_start 
            обнулится */
            m_uart_send_callback(databyte);
        }
    } else if((buf->mutex != FALSE) || (buf->end == buf->start)){
            m_synch_flag = TRUE;
    }
}

