/**
 * @file oslib_can_dispatch.h
 * @author KYzhang
 * @brief OSLIB-CAN接收分配模块
 * @version 0.2
 * @date 2021-01-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __OSLIB_CAN_DISPATCH_H
#define __OSLIB_CAN_DISPATCH_H

#include "oslib_config.h"

#ifdef OSLIB_CAN_MODULE_ENABLED

#include "cmsis_os2.h"

#include "can/oslib_can.h"
#include "utils/oslib_hash.h"

/*-数据结构-----------------------------------*/
typedef void (*CAN_Callback)(CAN_ConnMessage *msg);
typedef enum
{
    CAN_IDTYPE_STD  = 0,
    CAN_IDTYPE_EXT  = 1,
    CAN_IDTYPE_VESC = 2,
} CAN_IDTYPE;

typedef struct
{
    uint32_t id;               /* CAN ID */
    CAN_IDTYPE idtype;         /* TYPE: CAN_ID_STD/CAN_ID_EXT */
    osMessageQueueId_t *queue; /* 任务对应的消息队列 */
    CAN_Callback callback;     /* 回调函数 */
} CAN_IDRecord_t;

typedef struct
{
    OSLIB_CAN_Handle_t *can_handle;
    osThreadId_t task;
    HashTable table;
} OSLIB_CAN_Dispatch_t;

/*-宏-----------------------------------------*/
#define CANx_Record_Queue(id, idtype, queue) \
    {id, idtype, queue, NULL}
#define CANx_Record_Callback(id, idtype, callback) \
    {id, idtype, NULL, callback}

/*-接口---------------------------------------*/
extern void OSLIB_CAN_Dispatch_Init(OSLIB_CAN_Dispatch_t *can_dispatch, OSLIB_CAN_Handle_t *can_handle, 
                                    CAN_IDRecord_t *can_record_list, size_t can_record_list_size);

#endif // OSLIB_CAN_MODULE_ENABLED

#endif // __OSLIB_CAN_DISPATCH_H
