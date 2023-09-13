#include "oslib_config.h"

#ifdef OSLIB_UART_MODULE_ENABLED

#include "uart/oslib_uart.h"
#include "uart/oslib_uart_cmd.h"
#ifdef OSLIB_CAN_MODULE_ENABLED
    #include "can/oslib_can.h"
#endif

#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"

/* 在这里添加命令回调函数的定义或外部声明 */
static void Command_Hello(OSLIB_UART_Handle_t *uart_handle, int argc, char *argv[])
{
    OSLIB_UART_Printf(uart_handle->huart, "Hello SimpleLib!\r\n");
}

static void Command_Echo(OSLIB_UART_Handle_t *uart_handle, int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
        OSLIB_UART_Printf(uart_handle->huart, "Echo: %s\r\n", argv[i]);
}

#ifdef OSLIB_CAN_MODULE_ENABLED
static void Command_CanSend(OSLIB_UART_Handle_t *uart_handle, int argc, char *argv[])
{
    if (argc >= 3) {
        can_msg msg;
        msg.in[0] = 0x40005850;
        msg.in[1] = 0x00000000;
        uint32_t id = strtol(argv[2], NULL, 16);
        if (argv[1][0] == 's') {
            can_send_msg(id, &msg);
            OSLIB_UART_Printf(uart_handle->huart, "CanSend:std[%x]\r\n", id);
        } else if (argv[1][0] == 'e') {
            can_ext_send_msg(id, &msg);
            OSLIB_UART_Printf(uart_handle->huart, "CanSend:ext[%x]\r\n", id);
        } else
            OSLIB_UART_Printf(uart_handle->huart, "Param Error!\r\n");
    } else {
        OSLIB_UART_Printf(uart_handle->huart, "Param Error!\r\n");
    }
}
#endif

static void Command_Task(OSLIB_UART_Handle_t *uart_handle, int argc, char *argv[])
{
    OSLIB_UART_Printf(uart_handle->huart, "Free Heap: %dB\r\n", xPortGetFreeHeapSize());
#if ((configUSE_TRACE_FACILITY == 1) && (configUSE_STATS_FORMATTING_FUNCTIONS > 0) && (configSUPPORT_DYNAMIC_ALLOCATION == 1))
    static char tasklist_buffer[256];
    OSLIB_UART_Printf(uart_handle->huart, "Name          \tState\tPrio\tStack\tRank\r\n");
    vTaskList(tasklist_buffer);
    OSLIB_UART_Printf(uart_handle->huart, tasklist_buffer);
#endif
    // uprintf("Name          \tCount\tUtilization\r\n");
    // vTaskGetRunTimeStats(tasklist_buffer);
    // uprintf(tasklist_buffer);
}

/* 在这里完成命令到回调函数的映射 */
UART_CLI_Command_t UART_CommandList[] = 
{
    { "hello",      "Print \"Hello SimpleLib!\"",   Command_Hello   },
    { "echo",       "Echo message",                 Command_Echo    },
    { "tasks",      "Show task list",               Command_Task    },
#ifdef OSLIB_CAN_MODULE_ENABLED
    { "cansend",    "cansend [ std | ext ] <id>",   Command_CanSend },
#endif
};
size_t UART_CommandListSize = sizeof(UART_CommandList) / sizeof(UART_CLI_Command_t);

#endif // OSLIB_UART_MODULE_ENABLED
