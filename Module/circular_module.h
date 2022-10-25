/**
 * @file    circular_module.h
 * @author  StarLine Busorgin_S
 * @date    24 Oct 2022
 * @version 1.0.0
 * @brief   Circular buffer API and UART send data from buffer
 *
 */

#ifndef __CIRCULAR_BUFF_H__
#define __CIRCULAR_BUFF_H__

/* HEADER FILE INCLUDES ------------------------------------------------------*/
#include <stdint.h>

/* DEFINES -------------------------------------------------------------------*/
#define M_DATA_LENGTH_8BIT    8U
#define M_DATA_LENGTH_9BIT    9U
#define M_UART_STOPBITS_1     1U
#define M_UART_STOPBITS_2     2U
#define M_UART_PARITY_NONE    0U
#define M_UART_PARITY_EVEN    1U
#define M_UART_PARITY_ODD     2U

/* TYPES ---------------------------------------------------------------------*/
typedef struct
{
    uint8_t  *buffer;
    uint8_t  mutex;
    uint16_t start;
    uint16_t end;
    uint16_t size;
} circular_buffer_t;

typedef struct
{
    uint32_t baudrate;    /**< Скорость передачи данных */
    uint32_t worldlenght; /**< Длина пакета в битах */
    uint32_t stopbits;    /**< Количество стоп битов */
    uint32_t parity;      /**< Чётность: 0 - нет, 1 - проеврка включена */
} uart_init_t;

typedef enum
{
    NO_ERR_BUFF,    /**< no error */
    ERR_BUFF,       /**< Buffer error */
    BUFF_NOT_EMPTY, /**< Buffer is not empty */
    BUFF_EMPTY      /**< Buffer is empty */
} buff_err_status_t;

typedef void (*uart_send_handler_t) (uint8_t databyte);
typedef void (*uart_init_handler_t) (uart_init_t uart_param);

/* FUNCTION PROTOTYPES -------------------------------------------------------*/

/**
 * @brief Функция записи байта в кольцевой буфер
 *
 * @param[in] databyte - байт данных для записи
 * @param[in] buf - указатель на переменную типа circular_buffer_t
 */
void put_byte_to_buffer(uint8_t databyte, circular_buffer_t *buf);

/**
 * @brief Функция извлечения байта из кольцевого буфера
 *
 * @param[in] buf - указатель на переменную типа circular_buffer_t
 *
 * @return uint8_t databyte - байт данных из буфера
 */
uint8_t pop_byte_from_buffer(circular_buffer_t *buf);

/**
 * @brief Функция инициализации кольцевого буфера
 *
 * @param[in] buf - указатель на переменную типа circular_buffer_t
 * @param[in] buf_array - указатель на начало массива данных
 * @param[in] size - размер кольцевого буфера в байтах
 * @param[in] send_handler - указатель на функцию отправки байта по UART
 *
 * @return buff_err_status_t - код ошибки
 */
buff_err_status_t buffer_init(circular_buffer_t *buf, uint8_t *buf_array,
                        uint16_t size, uart_send_handler_t send_handler);

/**
 * @brief Функция инициализации UART
 *
 * @param[in] uart_init_handler - указатель на функцию инициализации
 */
void uart_init(uart_init_handler_t uart_init_handler);

/**
 * @brief Функция обработки события отправки байта по UART
 *
 * @param[in] buf - указатель на переменную типа circular_buffer_t
 */
void uart_event_transmit(circular_buffer_t *buf);

#endif /**< __CIRCULAR_BUFF_H__ */

