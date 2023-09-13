/**
 * @file oslib_uart_cmd.h
 * @author KYzhang
 * @brief OSLIB-UART命令行
 * @version 0.1
 * @date 2021-01-04
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __OSLIB_UART_CMD_H
#define __OSLIB_UART_CMD_H

#include "oslib_config.h"

#ifdef OSLIB_UART_MODULE_ENABLED

#include "cmsis_os2.h"

#include "uart/oslib_uart.h"
#include "utils/oslib_hash.h"

/*-指令哈希表定义---------------------------------*/

typedef void (*OSLIB_UART_CLI_Callback_t)(OSLIB_UART_Handle_t *uart_handle, int argc, char *argv[]);

typedef struct
{
	char *command;						/* 命令 */
	char *help_string;					/* 使用帮助 */
	OSLIB_UART_CLI_Callback_t callback; /* 回调函数 */
	// int8_t expect_params;			/* 预期的参数个数 */
} UART_CLI_Command_t;

typedef struct
{
	OSLIB_UART_Handle_t *uart_handle;
	osThreadId_t task;
	HashTable table; // 命令哈希表
} OSLIB_UART_CLI_t;

/*-接口函数--------------------------------------*/
#if USE_OSLIB_UART_CLI
extern void OSLIB_UART_CLI_Init(OSLIB_UART_CLI_t *uart_cli, OSLIB_UART_Handle_t *uart_handle, UART_CLI_Command_t *cmd_list, size_t size);
extern void OSLIB_UART_CLI_AddCommand(OSLIB_UART_Handle_t *uart_handle, char *command, char *help_string, OSLIB_UART_CLI_Callback_t callback);
#endif

#endif // OSLIB_UART_MODULE_ENABLED

#endif // __OSLIB_UART_CMD_H
