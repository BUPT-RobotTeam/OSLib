/**
 * @file oslib_can.h
 * @author KYzhang
 * @brief OSLIB-CAN发送接收管理
 * @version 0.2
 * @date 2021-01-04
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __OSLIB_CAN_H
#define __OSLIB_CAN_H

#include "oslib_config.h"

#ifdef OSLIB_CAN_MODULE_ENABLED

#include "cmsis_os2.h"
#include "can.h"

#include "utils/oslib_hash.h"

/*-对F103简陋的兼容----------------------------*/

#ifdef STM32F103xB
#define hcan1 hcan
#endif

/*-数据结构-----------------------------------*/

typedef union
{
    char ch[8];
    uint8_t ui8[8];
    uint16_t ui16[4];
    int16_t i16[4];
    int in[2];
    float fl[2];
    double df;
} CAN_Message;

typedef CAN_Message can_msg;

// FIXME 由于对齐特性, CAN_ConnMessage占16个字节而非12个字节
typedef struct
{
    uint32_t id;
    uint8_t ide;
    uint8_t rtr;
    uint8_t len;
    uint8_t fifo;
    CAN_Message payload;
} CAN_ConnMessage;

typedef struct _OSLIB_CAN_Handle_t
{
    CAN_HandleTypeDef *hcan;
    osSemaphoreId_t tx_sema;
    osMessageQueueId_t rx_queue;
    const char *name;
    void *plugin;
    struct _OSLIB_CAN_Handle_t *next;
} OSLIB_CAN_Handle_t;

/*-接口函数-----------------------------------*/

extern void OSLIB_CAN_Handle_Init(OSLIB_CAN_Handle_t *can_handle, CAN_HandleTypeDef *hcan, const char *name, size_t rx_queue_size);
extern void OSLIB_CAN_SendMessage(CAN_HandleTypeDef *hcan, uint32_t idtype, uint32_t id, CAN_Message *msg);
extern void OSLIB_CAN_Init(void);

/*-兼容定义-----------------------------------*/

#define can_send_msg(id, msg) OSLIB_CAN_SendMessage(&hcan1, CAN_ID_STD, id, msg)
#define can_ext_send_msg(id, msg) OSLIB_CAN_SendMessage(&hcan1, CAN_ID_EXT, id, msg)

#endif // OSLIB_CAN_MODULE_ENABLED

#endif // __OSLIB_CAN_H
