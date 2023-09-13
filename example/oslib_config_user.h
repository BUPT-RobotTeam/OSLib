/* OSLIB模块选择 */
#define OSLIB_DEBUG_MODULE_ENABLED  // 调试输出功能
#define OSLIB_UART_MODULE_ENABLED   // 串口功能
#define OSLIB_LOG_MODULE_ENABLED  // 日志输出功能
#define OSLIB_CAN_MODULE_ENABLED    // CAN功能

/* 选择使用的外设, 需要和HAL库的配置匹配 */
#define UART1_ENABLED
#define SELECT_UART1_RXMODE     0
#define UART1_TX_BUFFER_SIZE    256
#define UART1_RX_BUFFER_SIZE    128

#define UART2_ENABLED
// #define SELECT_UART2_RX_MODE    0
// #define UART2_TX_BUFFER_SIZE    128
// #define UART2_RX_BUFFER_SIZE    64

#define CAN1_ENABLED
// #define CAN2_ENABLED

/* 串口命令行 */
#define USE_OSLIB_UART_CLI      1
#define SELECT_MAJOR_UART       1     // 选择主串口, 即指定一个使用串口命令行的串口, 允许取值[1-UART1/.../6-UART6]
#define elog_huart                 (huart1)