/**
 * @file oslib_can_example.h
 * @author KYzhang
 * @brief OSLIB-CAN接收处理任务范例
 * @version 0.2
 * @date 2021-01-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef OSLIB_CAN_EXAMPLE_H
#define OSLIB_CAN_EXAMPLE_H

#include "oslib_config.h"

#ifdef OSLIB_CAN_MODULE_ENABLED

#include "cmsis_os2.h"

#include "can/oslib_can.h"

#if USE_OSLIB_CAN_EXAMPLE
extern osMessageQueueId_t ExampleCanTaskQueue;
extern void CAN_Example_Init(void);
#endif

#endif // OSLIB_CAN_MODULE_ENABLED

#endif
