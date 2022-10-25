/**
 * @file    circular_module_config.h
 * @author  StarLine Busorgin_S
 * @date    24 Oct 2022
 * @version 1.0.0
 * @brief   Circular buffer API and UART send data from buffer
 *
 */

#ifndef __CIRCULAR_BUFF_CONFIG_H__
#define __CIRCULAR_BUFF_CONFIG_H__

/* DEFINES -------------------------------------------------------------------*/
#define SIZE_BUFF    100U /**< Размер кольцевого буфера в байтах */

/**< UART */
#define BAUDRATE    115200UL /**< Скорость передачи данных */
#define WORDLENGHT  8U       /**< Длина пакета */
#define STOPBITS    1U       /**< Количество стоп битов */
#define PARITY      0U       /**< Чётность: 0 - нет, 1 - проеврка включена */

#endif /**< __CIRCULAR_BUFF_H__ */
