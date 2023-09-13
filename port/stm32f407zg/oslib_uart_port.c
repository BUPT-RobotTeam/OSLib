#include "oslib_config.h"

#ifdef OSLIB_UART_MODULE_ENABLED

#include "uart/oslib_uart.h"
#include "uart/oslib_uart_cmd.h"

/*-初始化函数--------------------------------------------------*/
#if USE_OSLIB_UART_CLI
extern UART_CLI_Command_t UART_CLI_COMMAND_LIST[];
extern size_t UART_CLI_COMMAND_LIST_SIZE;
#endif

/**
 * @brief OSLIB-串口相关组件初始化
 */
void OSLIB_UART_Init()
{
#if defined(UART1_ENABLED)
    static OSLIB_UART_Handle_t uart1_handle;
    static uint8_t uart1_tx_buffer[UART1_TX_BUFFER_SIZE];
#if SELECT_UART1_RXMODE == 0
    static uint8_t uart1_rx_buffer[UART1_RX_BUFFER_SIZE];
    static uint8_t uart1_rx_task_buffer[UART1_RX_BUFFER_SIZE];
    OSLIB_UART_Handle_DMA_Init(&uart1_handle, &huart1, "USART1",
                               uart1_tx_buffer, UART1_TX_BUFFER_SIZE,
                               uart1_rx_buffer, uart1_rx_task_buffer, UART1_RX_BUFFER_SIZE);
#else
    OSLIB_UART_Handle_IT_Init(&uart1_handle, &huart1, "USART1",
                              uart1_tx_buffer, UART1_TX_BUFFER_SIZE, UART1_RX_BUFFER_SIZE);
#endif
#endif

#if defined(UART2_ENABLED)
    static OSLIB_UART_Handle_t uart2_handle;
    static uint8_t uart2_tx_buffer[UART2_TX_BUFFER_SIZE];
#if SELECT_UART2_RXMODE == 0
    static uint8_t uart2_rx_buffer[UART2_RX_BUFFER_SIZE];
    static uint8_t uart2_rx_task_buffer[UART2_RX_BUFFER_SIZE];
    OSLIB_UART_Handle_DMA_Init(&uart2_handle, &huart2, "USART2",
                               uart2_tx_buffer, UART2_TX_BUFFER_SIZE,
                               uart2_rx_buffer, uart2_rx_task_buffer, UART2_RX_BUFFER_SIZE);
#else
    OSLIB_UART_Handle_IT_Init(&uart2_handle, &huart2, "USART2",
                              uart2_tx_buffer, UART2_TX_BUFFER_SIZE, UART2_RX_BUFFER_SIZE);
#endif
#endif

#if defined(UART3_ENABLED)
    static OSLIB_UART_Handle_t uart3_handle;
    static uint8_t uart3_tx_buffer[UART3_TX_BUFFER_SIZE];
#if SELECT_UART3_RXMODE == 0
    static uint8_t uart3_rx_buffer[UART3_RX_BUFFER_SIZE];
    static uint8_t uart3_rx_task_buffer[UART3_RX_BUFFER_SIZE];
    OSLIB_UART_Handle_DMA_Init(&uart3_handle, &huart3, "USART3",
                               uart3_tx_buffer, UART3_TX_BUFFER_SIZE,
                               uart3_rx_buffer, uart3_rx_task_buffer, UART3_RX_BUFFER_SIZE);
#else
    OSLIB_UART_Handle_IT_Init(&uart3_handle, &huart3, "USART3",
                              uart3_tx_buffer, UART3_TX_BUFFER_SIZE, UART3_RX_BUFFER_SIZE);
#endif
#endif

#if defined(UART4_ENABLED)
    static OSLIB_UART_Handle_t uart4_handle;
    static uint8_t uart4_tx_buffer[UART4_TX_BUFFER_SIZE];
#if SELECT_UART4_RXMODE == 0
    static uint8_t uart4_rx_buffer[UART4_RX_BUFFER_SIZE];
    static uint8_t uart4_rx_task_buffer[UART4_RX_BUFFER_SIZE];
    OSLIB_UART_Handle_DMA_Init(&uart4_handle, &huart4, "UART4",
                               uart4_tx_buffer, UART4_TX_BUFFER_SIZE,
                               uart4_rx_buffer, uart4_rx_task_buffer, UART4_RX_BUFFER_SIZE);
#else
    OSLIB_UART_Handle_IT_Init(&uart4_handle, &huart4, "UART4",
                              uart4_tx_buffer, UART4_TX_BUFFER_SIZE, UART4_RX_BUFFER_SIZE);
#endif
#endif

#if defined(UART5_ENABLED)
    static OSLIB_UART_Handle_t uart5_handle;
    static uint8_t uart5_tx_buffer[UART5_TX_BUFFER_SIZE];
#if SELECT_UART5_RXMODE == 0
    static uint8_t uart5_rx_buffer[UART5_RX_BUFFER_SIZE];
    static uint8_t uart5_rx_task_buffer[UART5_RX_BUFFER_SIZE];
    OSLIB_UART_Handle_DMA_Init(&uart5_handle, &huart5, "UART5",
                               uart5_tx_buffer, UART5_TX_BUFFER_SIZE,
                               uart5_rx_buffer, uart5_rx_task_buffer, UART5_RX_BUFFER_SIZE);
#else
    OSLIB_UART_Handle_IT_Init(&uart5_handle, &huart5, "UART5",
                              uart5_tx_buffer, UART5_TX_BUFFER_SIZE, UART5_RX_BUFFER_SIZE);
#endif
#endif

#if defined(UART6_ENABLED)
    static OSLIB_UART_Handle_t uart6_handle;
    static uint8_t uart6_tx_buffer[UART6_TX_BUFFER_SIZE];
#if SELECT_UART6_RXMODE == 0
    static uint8_t uart6_rx_buffer[UART6_RX_BUFFER_SIZE];
    static uint8_t uart6_rx_task_buffer[UART6_RX_BUFFER_SIZE];
    OSLIB_UART_Handle_DMA_Init(&uart6_handle, &huart6, "USART6",
                               uart6_tx_buffer, UART6_TX_BUFFER_SIZE,
                               uart6_rx_buffer, uart6_rx_task_buffer, UART6_RX_BUFFER_SIZE);
#else
    OSLIB_UART_Handle_IT_Init(&uart6_handle, &huart6, "USART6",
                              uart6_tx_buffer, UART6_TX_BUFFER_SIZE, UART6_RX_BUFFER_SIZE);
#endif
#endif

    /* 任务初始化 */
#if USE_OSLIB_UART_CLI
    static OSLIB_UART_CLI_t uart_cli;
#if SELECT_MAJOR_UART == 1 && defined(UART1_ENABLED)
    OSLIB_UART_CLI_Init(&uart_cli, &uart1_handle, UART_CLI_COMMAND_LIST, UART_CLI_COMMAND_LIST_SIZE);
#elif SELECT_MAJOR_UART == 2 && defined(UART2_ENABLED)
    OSLIB_UART_CLI_Init(&uart_cli, &uart2_handle, UART_CLI_COMMAND_LIST, UART_CLI_COMMAND_LIST_SIZE);
#elif SELECT_MAJOR_UART == 3 && defined(UART3_ENABLED)
    OSLIB_UART_CLI_Init(&uart_cli, &uart3_handle, UART_CLI_COMMAND_LIST, UART_CLI_COMMAND_LIST_SIZE);
#elif SELECT_MAJOR_UART == 4 && defined(UART4_ENABLED)
    OSLIB_UART_CLI_Init(&uart_cli, &uart4_handle, UART_CLI_COMMAND_LIST, UART_CLI_COMMAND_LIST_SIZE);
#elif SELECT_MAJOR_UART == 5 && defined(UART5_ENABLED)
    OSLIB_UART_CLI_Init(&uart_cli, &uart5_handle, UART_CLI_COMMAND_LIST, UART_CLI_COMMAND_LIST_SIZE);
#elif SELECT_MAJOR_UART == 6 && defined(UART6_ENABLED)
    OSLIB_UART_CLI_Init(&uart_cli, &uart6_handle, UART_CLI_COMMAND_LIST, UART_CLI_COMMAND_LIST_SIZE);
#endif
#endif
}

#endif // OSLIB_UART_MODULE_ENABLED
