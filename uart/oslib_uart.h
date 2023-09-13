/**
 * @file oslib_uart.h
 * @author KYzhang
 * @brief OSLIB-UART发送接收管理
 * @version 0.2
 * @date 2021-01-04
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __OSLIB_UART_H
#define __OSLIB_UART_H

#include "oslib_config.h"

#ifdef OSLIB_UART_MODULE_ENABLED

#include "cmsis_os2.h"
#include "usart.h"

/*-OSLIB柄定义---------------------------------*/

#define OSLIB_UART_RX_MODE_IDLE_DMA 0
#define OSLIB_UART_RX_MODE_IT 1

typedef struct _OSLIB_UART_Handle_t
{
    UART_HandleTypeDef *huart;
    uint32_t rx_mode;
    osSemaphoreId_t tx_sema;
    uint8_t *tx_buffer;
    size_t tx_buffer_size;
    union
    {
        struct
        {
            osSemaphoreId_t rx_sema;
            uint8_t *rx_buffer;
            uint8_t *rx_task_buffer;
            size_t rx_buffer_len;
            size_t rx_buffer_size;
        } dma;
        struct
        {
            osMessageQueueId_t rx_queue;
            uint8_t rx_char;
            size_t rx_queue_size;
        } it;
    } rx;
    const char *name;
    void *plugin;
    struct _OSLIB_UART_Handle_t *next;
} OSLIB_UART_Handle_t;

/*-接口函数-----------------------------------*/

extern OSLIB_UART_Handle_t *OSLIB_UART_Handle_Get(UART_HandleTypeDef *huart);
extern void OSLIB_UART_Printf(UART_HandleTypeDef *huart, const char *fmt, ...);
extern void OSLIB_UART_SendData(UART_HandleTypeDef *huart, const uint8_t *msg, uint32_t size);
extern void OSLIB_UART_RxIdleCallback(UART_HandleTypeDef *huart);
extern void OSLIB_UART_Handle_DMA_Init(OSLIB_UART_Handle_t *uart_handle, UART_HandleTypeDef *huart, const char *name,
                                       uint8_t *tx_buffer, size_t tx_buffer_size,
                                       uint8_t *rx_buffer, uint8_t *rx_task_buffer, size_t rx_buffer_size);
extern void OSLIB_UART_Handle_IT_Init(OSLIB_UART_Handle_t *uart_handle, UART_HandleTypeDef *huart, const char *name,
                                      uint8_t *tx_buffer, size_t tx_buffer_size,
                                      size_t rx_queue_size);
extern void OSLIB_UART_Init(void);

/*-兼容定义-----------------------------------*/

#ifdef huart_major
#define uprintf(fmt, args...) OSLIB_UART_Printf(&huart_major, fmt, ##args)
#endif
#define uprintf_to(huart, fmt, args...) OSLIB_UART_Printf(huart, fmt, ##args)

#endif // OSLIB_UART_MODULE_ENABLED

#endif // __OSLIB_UART_H
