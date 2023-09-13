/**
 * @file oslib_config.h
 * @author KYzhang
 * @brief OSLIB配置
 * @version 0.2
 * @date 2021-01-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __OSLIB_CONFIG_H
#define __OSLIB_CONFIG_H

#include "main.h"
#include "oslib_config_user.h"
#include "oslib_config_port.h"

/* 调试输出等级 */
#ifndef SELECT_DEBUG_LEVEL
#define SELECT_DEBUG_LEVEL      OSLIB_DEBUG_LEVEL_DEBUG    
                                        // 调试输出等级, 共有7级, 允许取值
                                        // [OFF/FATAL/ERROR/WARN/INFO/DEBUG/ALL]
#endif

/* 串口命令行 */
#ifndef USE_OSLIB_UART_CLI
#define USE_OSLIB_UART_CLI      1       // 是否使用串口命令行, 允许取值[1-Yes/0-No]
#endif

/* CAN范例任务 */
#ifndef USE_OSLIB_CAN_EXAMPLE
#define USE_OSLIB_CAN_EXAMPLE   0       // 是否使用CAN范例任务处理CAN报文, 该任务会将报文ID通过主串口打印出来, 允许取值[1-Yes/0-No]
#endif

/* 根据实际HAL库情况取消对应的OSLIB模块 */
#ifndef HAL_UART_MODULE_ENABLED
#undef OSLIB_UART_MODULE_ENABLED
#undef OSLIB_LOG_MODULE_ENABLED
#endif

#ifndef HAL_CAN_MODULE_ENABLED
#undef OSLIB_CAN_MODULE_ENABLED
#endif
#endif // __OSLIB_CONFIG_H
