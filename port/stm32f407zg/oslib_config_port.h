
#ifndef __OSLIB_CONFIG_PORT_H
#define __OSLIB_CONFIG_PORT_H

#include "oslib_config_user.h"

/*-UART默认值--------------------------------------------------*/
#define TxBufferSize 128 // 发送缓冲区大小
#define RxBufferSize 64  // 接收缓冲区大小

#ifdef UART1_ENABLED

    #ifndef SELECT_UART1_RXMODE
    #define SELECT_UART1_RXMODE 0
    #endif // SELECT_UART1_RXMODE

    #ifndef UART1_TX_BUFFER_SIZE
    #define UART1_TX_BUFFER_SIZE TxBufferSize
    #endif // UART1_TX_BUFFER_SIZE

    #ifndef UART1_RX_BUFFER_SIZE
    #define UART1_RX_BUFFER_SIZE RxBufferSize
    #endif // UART_RX_BUFFER_SIZE

#endif // UART1_ENABLED


#ifdef UART2_ENABLED

    #ifndef SELECT_UART2_RXMODE
    #define SELECT_UART2_RXMODE 0
    #endif // SELECT_UART2_RXMODE

    #ifndef UART2_TX_BUFFER_SIZE
    #define UART2_TX_BUFFER_SIZE TxBufferSize
    #endif // UART2_TX_BUFFER_SIZE

    #ifndef UART2_RX_BUFFER_SIZE
    #define UART2_RX_BUFFER_SIZE RxBufferSize
    #endif // UART2_RX_BUFFER_SIZE

#endif // UART2_ENABLED


#ifdef UART3_ENABLED

    #ifndef SELECT_UART3_RXMODE
    #define SELECT_UART3_RXMODE 0
    #endif // SELECT_UART3_RXMODE

    #ifndef UART3_TX_BUFFER_SIZE
    #define UART3_TX_BUFFER_SIZE TxBufferSize
    #endif // UART3_TX_BUFFER_SIZE

    #ifndef UART3_RX_BUFFER_SIZE
    #define UART3_RX_BUFFER_SIZE RxBufferSize
    #endif // UART3_RX_BUFFER_SIZE

#endif // UART3_ENABLED


#ifdef UART4_ENABLED

    #ifndef SELECT_UART4_RXMODE
    #define SELECT_UART4_RXMODE 0
    #endif // SELECT_UART4_RXMODE

    #ifndef UART4_TX_BUFFER_SIZE
    #define UART4_TX_BUFFER_SIZE TxBufferSize
    #endif // UART4_TX_BUFFER_SIZE

    #ifndef UART4_RX_BUFFER_SIZE
    #define UART4_RX_BUFFER_SIZE RxBufferSize
    #endif // UART4_RX_BUFFER_SIZE

#endif // UART4_ENABLED


#ifdef UART5_ENABLED

    #ifndef SELECT_UART5_RXMODE
    #define SELECT_UART5_RXMODE 0
    #endif // SELECT_UART5_RXMODE

    #ifndef UART5_TX_BUFFER_SIZE
    #define UART5_TX_BUFFER_SIZE TxBufferSize
    #endif // UART5_TX_BUFFER_SIZE

    #ifndef UART5_RX_BUFFER_SIZE
    #define UART5_RX_BUFFER_SIZE RxBufferSize
    #endif // UART5_RX_BUFFER_SIZE

#endif // UART5_ENABLED


#ifdef UART6_ENABLED

    #ifndef SELECT_UART6_RXMODE
    #define SELECT_UART6_RXMODE 0
    #endif // SELECT_UART6_RXMODE

    #ifndef UART6_TX_BUFFER_SIZE
    #define UART6_TX_BUFFER_SIZE TxBufferSize
    #endif // UART6_TX_BUFFER_SIZE

    #ifndef UART6_RX_BUFFER_SIZE
    #define UART6_RX_BUFFER_SIZE RxBufferSize
    #endif // UART6_RX_BUFFER_SIZE

#endif // UART6_ENABLED

/*-主串口--------------------------------------------*/

#ifndef huart_major

    #if SELECT_MAJOR_UART == 1 && defined(UART1_ENABLED)
        #define huart_major huart1
    #elif SELECT_MAJOR_UART == 2 && defined(UART2_ENABLED)
        #define huart_major huart2
    #elif SELECT_MAJOR_UART == 3 && defined(UART3_ENABLED)
        #define huart_major huart3
    #elif SELECT_MAJOR_UART == 4 && defined(UART4_ENABLED)
        #define huart_major huart4
    #elif SELECT_MAJOR_UART == 5 && defined(UART5_ENABLED)
        #define huart_major huart5
    #elif SELECT_MAJOR_UART == 6 && defined(UART6_ENABLED)
        #define huart_major huart6
    #else
        #error "[OSLIB] Invalid UART"#SELECT_MAJOR_UART"for CLI"
    #endif // SELECT_MAJOR_UART

#endif // huart_major

/*-串口命令行--------------------------------------------*/

#if USE_OSLIB_UART_CLI

    #ifndef UART_CLI_COMMAND_LIST
    #define UART_CLI_COMMAND_LIST       UART_CommandList
    #define UART_CLI_COMMAND_LIST_SIZE  UART_CommandListSize
    #else
    #ifndef UART_CLI_COMMAND_LIST_SIZE
        #error [OSLIB] "UART_CLI_COMMAND_LIST_SIZE" is not defined
    #endif
    #endif // UART_CLI_COMMAND_LIST

#endif // USE_OSLIB_UART_CLI

/*-CAN默认值--------------------------------------------------*/

#ifdef CAN1_ENABLED

    #ifndef CAN1_RX_QUEUE_SIZE
    #define CAN1_RX_QUEUE_SIZE  8
    #endif // CAN1_RX_QUEUE_SIZE

    #ifndef CAN1_DISPATCH_RECORD_LIST
    #define CAN1_DISPATCH_RECORD_LIST       CAN1_RecordList
    #define CAN1_DISPATCH_RECORD_LIST_SIZE  CAN1_RecordListSize
    #else
    #ifndef CAN1_DISPATCH_RECORD_LIST_SIZE
        #error [OSLIB] "CAN1_DISPATCH_RECORD_LIST_SIZE" is not defined
    #endif // CAN1_DISPATCH_RECORD_LIST_SIZE
    #endif // CAN1_DISPATCH_RECORD_LIST

#endif // CAN1_ENABLED


#ifdef CAN2_ENABLED

    #ifndef CAN2_RX_QUEUE_SIZE
    #define CAN2_RX_QUEUE_SIZE  8
    #endif // CAN2_RX_QUEUE_SIZE

    #ifndef CAN2_DISPATCH_RECORD_LIST
    #define CAN2_DISPATCH_RECORD_LIST       CAN2_RecordList
    #define CAN2_DISPATCH_RECORD_LIST_SIZE  CAN2_RecordListSize
    #else
    #ifndef CAN2_DISPATCH_RECORD_LIST_SIZE
        #error [OSLIB] "CAN2_DISPATCH_RECORD_LIST_SIZE" is not defined
    #endif // CAN2_DISPATCH_RECORD_LIST_SIZE
    #endif // CAN2_DISPATCH_RECORD_LIST

#endif // CAN2_ENABLED

#endif /* __OSLIB_CONFIG_PORT_H */
