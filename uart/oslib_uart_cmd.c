/**
 * @file oslib_uart_cmd.c
 * @author KYzhang
 * @brief OSLIB-UART命令行
 * @version 0.2
 * @date 2021-01-04
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "oslib_config.h"

#ifdef OSLIB_UART_MODULE_ENABLED

#include <string.h>
#include "FreeRTOS.h"

#include "uart/oslib_uart.h"
#include "uart/oslib_uart_cmd.h"
#include "utils/oslib_hash.h"

#if USE_OSLIB_UART_CLI

#ifdef OSLIB_LOG_MODULE_ENABLED
#include "components/easylogger/port/elog_cmd.h"
#endif

#define COMMANDLINE_MAX_COUNT   5   /* 一次可接收的命令行数 */
#define PARAMETER_MAX_COUNT     10  /* 一条命令中可容纳的参数数目 */

const char *WelcomeMessage = "OSLIB CommandLine\r\n"; /* 欢迎信息 */

/*-串口命令行处理函数-----------------------------------------------*/
static void OSLIB_UART_CLI_StrTok(char *str, const char *delim, char *tokens[], size_t *count)
{
    char *left = NULL;
    char *right = str;
    size_t size = *count;
    size_t index = 0;

    for (; *right != '\0'; right++)
    {
        const char *iter = delim;
        for (; *iter; iter++)
            if (*right == *iter)
                break;
        if (*iter)
        {
            *right = '\0';
            if (left != NULL && index < size)
            {
                tokens[index++] = left;
                left = NULL;
            }
        }
        else
        {
            if (left == NULL)
                left = right;
        }
    }

    if (left != NULL && index < size)
        tokens[index++] = left;

    *count = index;
}

/**
 * @brief 命令分割函数, 将接收到的多条命令按';'分隔开
 * @param buffer        接收缓冲区
 * @param commands      分割出的多条命令
 * @return int          命令的行数
 */
static int OSLIB_UART_CLI_CommandSplit(OSLIB_UART_Handle_t *uart_handle, char *buffer, char *commands[])
{
    const char *delim = ";\r\n";
    size_t count = COMMANDLINE_MAX_COUNT;
    OSLIB_UART_CLI_StrTok(buffer, delim, commands, &count);
    return count;
}

/**
 * @brief 命令处理函数, 完成命令解析和回调
 * @param command       命令
 * @return int          0-成功, 1-失败
 */
static int OSLIB_UART_CLI_ProcessCommand(OSLIB_UART_CLI_t *uart_cli, char *command)
{
    OSLIB_UART_Handle_t *uart_handle = uart_cli->uart_handle;
    const char *delim = " ";
    size_t argc = PARAMETER_MAX_COUNT;
    char *argv[PARAMETER_MAX_COUNT];

    OSLIB_UART_CLI_StrTok(command, delim, argv, &argc);

    if (argc < 1)
    {
        OSLIB_UART_Printf(uart_handle->huart, "No Input!\r\n");
        return 1;
    }
    UART_CLI_Command_t *cmd;
    if (NULL == (cmd = (UART_CLI_Command_t *)HashTable_get(uart_cli->table, argv[0])))
    {
        OSLIB_UART_Printf(uart_handle->huart, "Command Not Found!\r\n");
        return 1;
    }
    cmd->callback(uart_handle, argc, argv);
    return 0;
}

/*-任务函数定义--------------------------------------------------*/

/**
 * @brief 串口命令行执行任务
 * @param argument 参数未使用
 */
static void UART_CLI_Task(void *argument)
{
    OSLIB_UART_CLI_t *uart_cli = (OSLIB_UART_CLI_t *)argument;
    OSLIB_UART_Handle_t *uart_handle = uart_cli->uart_handle;
    OSLIB_UART_Printf(uart_handle->huart, WelcomeMessage); // 打印欢迎消息
    for (;;)
    {
        osSemaphoreAcquire(uart_handle->rx.dma.rx_sema, osWaitForever);
        int cli_count = 0;
        char *cli[COMMANDLINE_MAX_COUNT];
        cli_count = OSLIB_UART_CLI_CommandSplit(uart_handle, (char *)uart_handle->rx.dma.rx_task_buffer, cli);
        for (size_t i = 0; i < cli_count; i++)
        {
            OSLIB_UART_Printf(uart_handle->huart, ">>> %s\r\n", cli[i]);
            OSLIB_UART_CLI_ProcessCommand(uart_cli, cli[i]);
        }
    }
}
/*-帮助命令函数--------------------------------------------------*/

static void _Command_Help(const void *key, void **value, void *c1)
{
    OSLIB_UART_Handle_t *uart_handle = (OSLIB_UART_Handle_t *)c1;
    char *help = ((UART_CLI_Command_t *)(*value))->help_string;
    OSLIB_UART_Printf(uart_handle->huart, "%s: %s\r\n", key, help);
}

static void Command_Help(OSLIB_UART_Handle_t *uart_handle, int argc, char *argv[])
{
    HashTable_map(((OSLIB_UART_CLI_t *)(uart_handle->plugin))->table, _Command_Help, (void *)uart_handle);
}

/*-接口函数----------------------------------------------------*/
/**
 * @brief 串口命令行初始化
 * @note  对指令的添加需要在该函数调用之后
 */
void OSLIB_UART_CLI_Init(OSLIB_UART_CLI_t *uart_cli, OSLIB_UART_Handle_t *uart_handle, UART_CLI_Command_t *cmd_list, size_t size)
{
    // 创建命令处理任务
    static osThreadAttr_t task_attr = {
        .priority = osPriorityRealtime,
        .stack_size = 512 * 4,
    };
    uart_handle->plugin = (void *)uart_cli;
    uart_cli->task = osThreadNew(UART_CLI_Task, uart_cli, &task_attr);
    uart_cli->table = HashTable_create(cmpStr, hashStr, NULL);
    uart_cli->uart_handle = uart_handle;

    OSLIB_UART_CLI_AddCommand(uart_handle, "help", "Show this page", Command_Help);

    for (size_t i = 0; i < size; i++)
    {
        UART_CLI_Command_t *cmd = &cmd_list[i];
        HashTable_insert(uart_cli->table, cmd->command, cmd);
    }
#ifdef OSLIB_LOG_MODULE_ENABLED
    OSLIB_UART_CLI_AddElogCMD(uart_handle);
#endif
}

/**
 * @brief 串口命令行添加命令
 * @param command       命令
 * @param help_string   帮助信息
 * @param callback      回调函数
 */
void OSLIB_UART_CLI_AddCommand(OSLIB_UART_Handle_t *uart_handle, char *command, char *help_string, OSLIB_UART_CLI_Callback_t callback)
{
    UART_CLI_Command_t *new_cmd = (UART_CLI_Command_t *)pvPortMalloc(sizeof(UART_CLI_Command_t));
    new_cmd->command = command;
    new_cmd->help_string = help_string;
    new_cmd->callback = callback;
    HashTable_insert(((OSLIB_UART_CLI_t *)(uart_handle->plugin))->table, new_cmd->command, new_cmd);
}

#endif

#endif // OSLIB_UART_MODULE_ENABLED
