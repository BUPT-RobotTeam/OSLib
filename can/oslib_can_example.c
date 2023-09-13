/**
 * @file oslib_can_example.c
 * @author KYzhang
 * @brief OSLIB-CAN接收处理任务范例
 * @version 0.2
 * @date 2021-01-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "oslib_config.h"

#ifdef OSLIB_CAN_MODULE_ENABLED

#include "can/oslib_can_example.h"
#ifdef OSLIB_UART_MODULE_ENABLED
#include "uart/oslib_uart.h"
#endif

#if USE_OSLIB_CAN_EXAMPLE
/*-队列定义--------------------------------*/
osMessageQueueId_t ExampleCanTaskQueue;
const osMessageQueueAttr_t ExampleCanTaskQueue_Attribute = {
    .name = "ExampleCanQueue"};

/*-任务定义--------------------------------*/
static osThreadId_t ExampleCanTaskHandle;
const osThreadAttr_t ExampleCanTask_Attribute = {
    .name = "ExampleCanTask",
    .priority = (osPriority_t)osPriorityNormal,
    .stack_size = 128 * 4};

/*-任务函数--------------------------------*/

/**
 * @brief 1. CAN示例任务, 必要时应使用裁剪宏删去
 *        2. 建议不要在该文件中添加CAN任务, 在其他文件添加CAN任务并启动后, 只要在can_func.c文件中添加任务要使用的消息队列即可
 * @param argument 参数未使用
 */
static void ExampleCanTask(void *argument)
{
    UNUSED(argument);
    for (;;)
    {
        static CAN_ConnMessage msg;
        osMessageQueueGet(ExampleCanTaskQueue, &msg, NULL, osWaitForever);
        /* user functional code start */
        uprintf("CAN: ExampleTask [%x]\r\n", msg.id);
        /* user functional code end */
    }
}

/*-初始化示例任务------------------------------*/
void CAN_Example_Init()
{
    ExampleCanTaskQueue = osMessageQueueNew(8, sizeof(CAN_ConnMessage), &ExampleCanTaskQueue_Attribute);
    ExampleCanTaskHandle = osThreadNew(ExampleCanTask, NULL, &ExampleCanTask_Attribute);
    UNUSED(ExampleCanTaskHandle);
}
#endif

#endif // OSLIB_CAN_MODULE_ENABLED
