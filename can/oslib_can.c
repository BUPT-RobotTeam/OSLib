/**
 * @file oslib_can.c
 * @author KYzhang
 * @brief OSLIB-CAN发送接收管理
 * @version 0.2
 * @date 2021-01-04
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "oslib_config.h"

#ifdef OSLIB_CAN_MODULE_ENABLED

#include <string.h>

#include "FreeRTOS.h"
#include "cmsis_os2.h"

#include "can/oslib_can.h"
#include "can/oslib_can_dispatch.h"
#include "debug/oslib_debug.h"

/*-列表定义---------------------------------------------------*/
static OSLIB_CAN_Handle_t *can_handle_list_head = NULL;
static OSLIB_CAN_Handle_t *can_handle_list_tail = NULL;

static OSLIB_CAN_Handle_t *OSLIB_CAN_Handle_Get(CAN_HandleTypeDef *hcan)
{
    /* 遍历列表 */
    for (OSLIB_CAN_Handle_t *handle = can_handle_list_head; handle != NULL; handle = handle->next)
        if (handle->hcan == hcan)
            return handle;
    return NULL;
}

static void OSLIB_CAN_Handle_Register(OSLIB_CAN_Handle_t *can_handle)
{
    /* 尾插法加入到列表中 */
    can_handle->next = NULL;
    if (can_handle_list_head == NULL)
    {
        can_handle_list_head = can_handle;
        can_handle_list_tail = can_handle;
    }
    else
    {
        can_handle_list_tail->next = can_handle;
        can_handle_list_tail = can_handle;
    }
}
/*-HAL库中断回调函数----------------------------------------------*/
/**
 * @brief 发送邮箱0发送完成中断
 * @param hcan CAN句柄
 */
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    OSLIB_CAN_Handle_t *can_handle = OSLIB_CAN_Handle_Get(hcan);

    if (can_handle == NULL)
        return;

    Debug("%s: Tx Mailbox0", can_handle->name);
    osSemaphoreRelease(can_handle->tx_sema);
}
/**
 * @brief 发送邮箱1发送完成中断
 * @param hcan CAN句柄
 */
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
    OSLIB_CAN_Handle_t *can_handle = OSLIB_CAN_Handle_Get(hcan);

    if (can_handle == NULL)
        return;

    Debug("%s: Tx Mailbox1", can_handle->name);
    osSemaphoreRelease(can_handle->tx_sema);
}
/**
 * @brief 发送邮箱2发送完成中断
 * @param hcan CAN句柄
 */
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
    OSLIB_CAN_Handle_t *can_handle = OSLIB_CAN_Handle_Get(hcan);

    if (can_handle == NULL)
        return;

    Debug("%s: Tx Mailbox2", can_handle->name);
    osSemaphoreRelease(can_handle->tx_sema);
}

/**
 * @brief 接收FIFO0接收完成中断
 * @param hcan CAN句柄
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    OSLIB_CAN_Handle_t *can_handle = OSLIB_CAN_Handle_Get(hcan);
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
    CAN_ConnMessage message;

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data);
    
    if (can_handle == NULL)
        return;

    // Debug("%s: Rx FIFO0", can_handle->name);
    message.id = (header.IDE == CAN_ID_STD) ? header.StdId : header.ExtId;
    message.ide = header.IDE;
    message.rtr = header.RTR;
    message.len = header.DLC;
    message.fifo = CAN_RX_FIFO0;
    memset(message.payload.ch, 0, 8);
    memcpy(message.payload.ch, data, header.DLC);
    osMessageQueuePut(can_handle->rx_queue, &message, 0, 0);
}

void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)
{
}

#define VescMask (~(0x1FUL << 8))
/**
 * @brief 接收FIFO1完成中断
 * @param hcan CAN句柄
 */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    OSLIB_CAN_Handle_t *can_handle = OSLIB_CAN_Handle_Get(hcan);
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
    CAN_ConnMessage message;

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &header, data);

    if (can_handle == NULL)
        return;

    // Debug("%s: Rx FIFO1", can_handle->name);
    message.id = (header.IDE == CAN_ID_STD) ? header.StdId : header.ExtId;
    message.ide = header.IDE;
    message.rtr = header.RTR;
    message.len = header.DLC;
    message.fifo = CAN_RX_FIFO1;
    memset(message.payload.ch, 0, 8);
    memcpy(message.payload.ch, data, header.DLC);
    osMessageQueuePut(can_handle->rx_queue, &message, 0, 0);
}

/**
 * @brief 接收FIFO1满中断
 * @param hcan CAN句柄
 */
void HAL_CAN_RxFifo1FullCallback(CAN_HandleTypeDef *hcan)
{
}

/**
 * @brief 错误中断, 使用调试宏发送错误信息
 * @param hcan CAN句柄
 */
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
    OSLIB_CAN_Handle_t *can_handle = OSLIB_CAN_Handle_Get(hcan);

    if (can_handle == NULL)
        return;

    if (hcan->ErrorCode & (HAL_CAN_ERROR_TX_ALST0 | HAL_CAN_ERROR_TX_ALST1 | HAL_CAN_ERROR_TX_ALST2)) // 仲裁丢失
        Warn("%s: Tx Arbitration lost", hcan);
    else if (hcan->ErrorCode & (HAL_CAN_ERROR_TX_TERR0 | HAL_CAN_ERROR_TX_TERR1 | HAL_CAN_ERROR_TX_TERR2)) // 传输错误
        Warn("%s: Tx Transmission error", hcan);
    else
        Warn("%s: Unknown error", hcan);
    osSemaphoreRelease(can_handle->tx_sema);
}

/*-接口函数----------------------------------------------------*/

/*TODO*/
void OSLIB_CAN_Handle_Init(OSLIB_CAN_Handle_t *can_handle, CAN_HandleTypeDef *hcan, const char *name, size_t rx_queue_size)
{
    if (can_handle == NULL)
        can_handle = pvPortMalloc(sizeof(OSLIB_CAN_Handle_t));

    can_handle->hcan = hcan;

    /* 信号量初始化 */
    can_handle->tx_sema = osSemaphoreNew(3, 3, NULL);
    can_handle->rx_queue = osMessageQueueNew(rx_queue_size, sizeof(CAN_ConnMessage), NULL);

    /* 名称字段初始化 */
    if (name == NULL)
        can_handle->name = "CAN";
    else
        can_handle->name = name;

    /* 启动CAN */
    if (HAL_CAN_Start(hcan) != HAL_OK)
        Error_Handler();

    /* 使能发送中断 */
    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
        Error_Handler();

    /* 使能接收中断 */
    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
        Error_Handler();
    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_FULL) != HAL_OK)
        Error_Handler();
    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
        Error_Handler();
    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO1_FULL) != HAL_OK)
        Error_Handler();

    OSLIB_CAN_Handle_Register(can_handle);
}

/**
 * @brief CAN消息发送函数, 可发送拓展ID数据帧和标准ID数据帧, 如果ID过长, 会自动认为是拓展帧
 * @param hcan CAN句柄
 * @param idtype CANID类型 标准帧:CAN_ID_STD 拓展帧:CAN_ID_EXT
 * @param id CAN报文ID
 * @param msg CAN报文地址
 */
void OSLIB_CAN_SendMessage(CAN_HandleTypeDef *hcan, uint32_t idtype, uint32_t id, CAN_Message *msg)
{
    OSLIB_CAN_Handle_t *can_handle = OSLIB_CAN_Handle_Get(hcan);

    if (can_handle == NULL)
        return;

    assert_param(IS_CAN_IDTYPE(idtype));
    if (!IS_CAN_STDID(id))
        idtype = CAN_ID_EXT;

    CAN_TxHeaderTypeDef header = {id, id, idtype, CAN_RTR_DATA, 8, DISABLE};
    uint32_t mailbox;

    while (osSemaphoreAcquire(can_handle->tx_sema, osWaitForever) != osOK)
        ;
    while (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0)
        osDelay(1);

    HAL_CAN_AddTxMessage(hcan, &header, msg->ui8, &mailbox);
}

#endif // OSLIB_CAN_MODULE_ENABLED
