/**
 * @file oslib.c
 * @author KYzhang
 * @brief OSLIB各组件初始化
 * @version 0.2
 * @date 2021-01-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "oslib.h"


#if defined(OSLIB_LOG_MODULE_ENABLED)
    #include "components/easylogger/port/elog_port.h"
#endif
//TODO: 降低各进程占用的内存空间
//TODO: 监控内存分配情况
/**
 * @brief 集中初始化OSLIB中的各组件
 */
void OSLIB_Init()
{
#if defined(OSLIB_UART_MODULE_ENABLED)
    OSLIB_UART_Init();
#endif
#if defined(OSLIB_CAN_MODULE_ENABLED)
    OSLIB_CAN_Init();
#endif
#if defined(OSLIB_LOG_MODULE_ENABLED)
    OSLIB_Elog_Init();
#endif

    Debug("OSLIB Init Done.");
}
