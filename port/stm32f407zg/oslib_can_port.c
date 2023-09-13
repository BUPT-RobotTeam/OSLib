#include "oslib_config.h"

#ifdef OSLIB_CAN_MODULE_ENABLED

#include "can/oslib_can.h"
#include "can/oslib_can_dispatch.h"

/*-初始化函数--------------------------------------------------*/
#if defined(CAN1_ENABLED)
extern CAN_IDRecord_t CAN1_DISPATCH_RECORD_LIST[];
extern size_t CAN1_DISPATCH_RECORD_LIST_SIZE;
#endif

#if defined(CAN2_ENABLED)
extern CAN_IDRecord_t CAN2_DISPATCH_RECORD_LIST[];
extern size_t CAN2_DISPATCH_RECORD_LIST_SIZE;
#endif

/**
 * @brief OSLIB-CAN总线相关组件初始化
 */
void OSLIB_CAN_Init()
{
#if defined(CAN1_ENABLED)
    static OSLIB_CAN_Handle_t can1_handle;
    OSLIB_CAN_Handle_Init(&can1_handle, &hcan1, "CAN1", CAN1_RX_QUEUE_SIZE);
    static OSLIB_CAN_Dispatch_t can1_dispatch;
    OSLIB_CAN_Dispatch_Init(&can1_dispatch, &can1_handle, CAN1_DISPATCH_RECORD_LIST, CAN1_DISPATCH_RECORD_LIST_SIZE);
#endif
#if defined(CAN2_ENABLED)
    static OSLIB_CAN_Handle_t can2_handle;
    OSLIB_CAN_Handle_Init(&can2_handle, &hcan2, "CAN2", CAN2_RX_QUEUE_SIZE);
    static OSLIB_CAN_Dispatch_t can2_dispatch;
    OSLIB_CAN_Dispatch_Init(&can2_dispatch, &can2_handle, CAN2_DISPATCH_RECORD_LIST, CAN2_DISPATCH_RECORD_LIST_SIZE);
#endif
#if USE_OSLIB_CAN_EXAMPLE
    CAN_Example_Init();
#endif
}

#endif // OSLIB_CAN_MODULE_ENABLED
