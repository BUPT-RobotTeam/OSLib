/**
 * @file oslib_uart.c
 * @author KYzhang
 * @brief OSLIB-UART发送接收管理
 * @version 0.2
 * @date 2021-01-04
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "oslib_config.h"

#ifdef OSLIB_UART_MODULE_ENABLED

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "uart/oslib_uart.h"
#include "uart/oslib_uart_cmd.h"
#include "debug/oslib_debug.h"

/*-列表定义------------------------------------------------------*/
static OSLIB_UART_Handle_t *uart_handle_list_head = NULL;
static OSLIB_UART_Handle_t *uart_handle_list_tail = NULL;

OSLIB_UART_Handle_t *OSLIB_UART_Handle_Get(UART_HandleTypeDef *huart)
{
    /* 遍历列表 */
    for (OSLIB_UART_Handle_t *handle = uart_handle_list_head; handle != NULL; handle = handle->next)
        if (handle->huart == huart)
            return handle;
    return NULL;
}

static void OSLIB_UART_Handle_Register(OSLIB_UART_Handle_t *uart_handle)
{
    /* 尾插法加入到列表中 */
    uart_handle->next = NULL;
    if (uart_handle_list_head == NULL)
    {
        uart_handle_list_head = uart_handle;
        uart_handle_list_tail = uart_handle;
    }
    else
    {
        uart_handle_list_tail->next = uart_handle;
        uart_handle_list_tail = uart_handle;
    }
}
/*-HAL库中断回调函数----------------------------------------------*/

/**
 * @brief 串口发送中断回调函数
 * @param huart 串口句柄指针
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    OSLIB_UART_Handle_t *uart_handle = OSLIB_UART_Handle_Get(huart);

    if (uart_handle == NULL)
        return;

    Debug("%s Tx", uart_handle->name);
    osSemaphoreRelease(uart_handle->tx_sema);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    OSLIB_UART_Handle_t *uart_handle = OSLIB_UART_Handle_Get(huart);

    if (uart_handle == NULL)
        return;

    if (uart_handle->rx_mode != OSLIB_UART_RX_MODE_IT)
        return;

    Debug("%s: Rx IT", uart_handle->name);
    HAL_UART_Receive_IT(huart, (uint8_t *)&uart_handle->rx.it.rx_char, 1);
    osMessageQueuePut(uart_handle->rx.it.rx_queue, &uart_handle->rx.it.rx_char, NULL, 0);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    OSLIB_UART_Handle_t *uart_handle = OSLIB_UART_Handle_Get(huart);

    if (uart_handle == NULL)
        return;

    if (huart->ErrorCode & HAL_UART_ERROR_PE)
        Warn("%s: Parity Error", uart_handle->name);
    if (huart->ErrorCode & HAL_UART_ERROR_NE)
        Warn("%s: Noise Error", uart_handle->name);
    if (huart->ErrorCode & HAL_UART_ERROR_FE)
        Warn("%s: Frame Error", uart_handle->name);
    if (huart->ErrorCode & HAL_UART_ERROR_ORE)
        Warn("%s: Overrun Error", uart_handle->name);
    if (huart->ErrorCode & HAL_UART_ERROR_DMA)
        Warn("%s: DMA transfer Error", uart_handle->name);

    osSemaphoreRelease(uart_handle->tx_sema);
}

/**
 * @brief 串口空闲中断回调函数
 * @param huart 串口句柄指针
 */
void OSLIB_UART_RxIdleCallback(UART_HandleTypeDef *huart)
{
    OSLIB_UART_Handle_t *uart_handle = OSLIB_UART_Handle_Get(huart);

    if (uart_handle == NULL)
        return;

    if (uart_handle->rx_mode != OSLIB_UART_RX_MODE_IDLE_DMA)
        return;

    Debug("%s: Rx DMA", uart_handle->name);
    __HAL_UART_CLEAR_IDLEFLAG(huart);
    /*HAL_UART_DMAStop(huart);*/
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);
    HAL_DMA_Abort(huart->hdmarx);
    CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);
    huart->RxState = HAL_UART_STATE_READY;
    uint32_t count = uart_handle->rx.dma.rx_buffer_size - 1 - __HAL_DMA_GET_COUNTER(huart->hdmarx);
    if (count > 0)
    {
        memcpy(uart_handle->rx.dma.rx_task_buffer, uart_handle->rx.dma.rx_buffer, count);
        uart_handle->rx.dma.rx_buffer_len = count;
        uart_handle->rx.dma.rx_task_buffer[count] = '\0';
        HAL_UART_Receive_DMA(huart, (uint8_t *)uart_handle->rx.dma.rx_buffer, uart_handle->rx.dma.rx_buffer_size - 1);
        osSemaphoreRelease(uart_handle->rx.dma.rx_sema);
    }
    else
        HAL_UART_Receive_DMA(huart, (uint8_t *)uart_handle->rx.dma.rx_buffer, uart_handle->rx.dma.rx_buffer_size - 1);
}

/*-接口函数----------------------------------------------------*/

void OSLIB_UART_Handle_DMA_Init(OSLIB_UART_Handle_t *uart_handle, UART_HandleTypeDef *huart, const char *name,
                                uint8_t *tx_buffer, size_t tx_buffer_size,
                                uint8_t *rx_buffer, uint8_t *rx_task_buffer, size_t rx_buffer_size)
{
    uart_handle->huart = huart;

    if (name == NULL)
        uart_handle->name = "UART";
    else
        uart_handle->name = name;

    // TODO: 如果缓冲区为NULL, 则从堆里面分配
    uart_handle->tx_sema = osSemaphoreNew(1, 1, NULL);
    uart_handle->tx_buffer = tx_buffer;
    uart_handle->tx_buffer_size = tx_buffer_size;

    uart_handle->rx_mode = OSLIB_UART_RX_MODE_IDLE_DMA;
    uart_handle->rx.dma.rx_sema = osSemaphoreNew(1, 0, NULL);
    uart_handle->rx.dma.rx_buffer = rx_buffer;
    uart_handle->rx.dma.rx_task_buffer = rx_task_buffer;
    uart_handle->rx.dma.rx_buffer_size = rx_buffer_size;
    uart_handle->rx.dma.rx_buffer_len = 0;

    OSLIB_UART_Handle_Register(uart_handle);

    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(huart, (uint8_t *)uart_handle->rx.dma.rx_buffer, uart_handle->rx.dma.rx_buffer_size - 1);
}

void OSLIB_UART_Handle_IT_Init(OSLIB_UART_Handle_t *uart_handle, UART_HandleTypeDef *huart, const char *name,
                               uint8_t *tx_buffer, size_t tx_buffer_size,
                               size_t rx_queue_size)
{
    uart_handle->huart = huart;

    if (name == NULL)
        uart_handle->name = "UART";
    else
        uart_handle->name = name;

    // TODO: 如果缓冲区为NULL, 则从堆里面分配
    uart_handle->tx_sema = osSemaphoreNew(1, 1, NULL);
    uart_handle->tx_buffer = tx_buffer;
    uart_handle->tx_buffer_size = tx_buffer_size;

    uart_handle->rx_mode = OSLIB_UART_RX_MODE_IT;
    uart_handle->rx.it.rx_queue = osMessageQueueNew(rx_queue_size, sizeof(uint8_t), NULL);
    uart_handle->rx.it.rx_queue_size = rx_queue_size;

    OSLIB_UART_Handle_Register(uart_handle);

    HAL_UART_Receive_IT(huart, (uint8_t *)&uart_handle->rx.it.rx_char, 1);
}

/**
 * @brief 串口发送格式化字符串 注意: 请不要在中断中调用该函数!! 如果需要输出调试信息, 请使用LOG宏
 * @param huart     串口句柄
 * @param fmt       待发送字符串
 * @param ...       字符串参数
 */
void OSLIB_UART_Printf(UART_HandleTypeDef *huart, const char *fmt, ...)
{
    OSLIB_UART_Handle_t *uart_handle = OSLIB_UART_Handle_Get(huart);

    if (uart_handle == NULL)
        return;

    while (osSemaphoreAcquire(uart_handle->tx_sema, osWaitForever) != osOK)
        ;
    while (HAL_DMA_GetState(huart->hdmatx) == HAL_DMA_STATE_BUSY)
        osDelay(1);
    int size = 0;
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    size = vsnprintf((char *)uart_handle->tx_buffer, uart_handle->tx_buffer_size, fmt, arg_ptr);
    if (size >= uart_handle->tx_buffer_size)
    {
        Warn("UART: Tx Too long:%d", size);
        size = uart_handle->tx_buffer_size - 1;
    }
    va_end(arg_ptr);
    HAL_UART_Transmit_DMA(huart, (uint8_t *)uart_handle->tx_buffer, size);
}

/**
 * @brief 串口发送数据
 * @param huart 串口句柄
 * @param msg   待发送数据
 * @param size  待发送数据的字节数
 */
void OSLIB_UART_SendData(UART_HandleTypeDef *huart, const uint8_t *msg, uint32_t size)
{
    OSLIB_UART_Handle_t *uart_handle = OSLIB_UART_Handle_Get(huart);

    if (uart_handle == NULL)
        return;

    while (osSemaphoreAcquire(uart_handle->tx_sema, osWaitForever) != osOK)
        ;
    while (HAL_DMA_GetState(huart->hdmatx) == HAL_DMA_STATE_BUSY)
        osDelay(1);
    if (size >= uart_handle->tx_buffer_size)
    {
        Warn("UART: Tx Too long:%d", size);
        size = uart_handle->tx_buffer_size - 1;
    }
    memcpy(uart_handle->tx_buffer, msg, size);
    HAL_UART_Transmit_DMA(huart, (uint8_t *)uart_handle->tx_buffer, size);
}

#endif // OSLIB_UART_MODULE_ENABLED
