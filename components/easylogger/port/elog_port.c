/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2015-04-28
 */
 
#include <components/easylogger/inc/elog.h>

#include "oslib_config.h"
///< 两个模块都开启，才能使能对应代码，否则直接函数置空
#if defined(OSLIB_UART_MODULE_ENABLED) &&  defined(OSLIB_LOG_MODULE_ENABLED)
#include "elog_port.h"
#include "stdio.h"
#include "oslib_uart.h"
#include "cmsis_os2.h"
#ifndef elog_huart
///<未设置串口，使用主串口完成日志功能
#define elog_huart huart_major
#endif
///< 输出锁
static osSemaphoreId_t elogLock_sema;
///< 异步缓冲区锁
static osSemaphoreId_t elogAsyncBuff_sema;
///< 异步输出消息队列参数
static osMessageQueueAttr_t elogAsync_queue_attr = {
        .name = "elogAsync_queue"
};
///< 异步输出消息队列
static osMessageQueueId_t elogAsync_queue;
///< 异步输出任务
static osThreadId_t elogTask;
///< 异步输出任务参数
static osThreadAttr_t elogTask_attr = {
        .name = "ElogAsync",
        .priority = osPriorityLow,
        .stack_size = 128 * 4,
};
/**
 * ELOG初始化函数。初始化elog模块，设定输出格式，启动异步输出任务。
 */
void OSLIB_Elog_Init(){
    if(elog_init()==ELOG_NO_ERR)
    {
        elog_set_fmt(ELOG_LVL_ASSERT,   ELOG_DEFFMT_ASSERT);
        elog_set_fmt(ELOG_LVL_ERROR,    ELOG_DEFFMT_ERROR);
        elog_set_fmt(ELOG_LVL_WARN,     ELOG_DEFFMT_WARN);
        elog_set_fmt(ELOG_LVL_INFO,     ELOG_DEFFMT_INFO);
        elog_set_fmt(ELOG_LVL_DEBUG,    ELOG_DEFFMT_DEBUG);
        elog_set_fmt(ELOG_LVL_VERBOSE,  ELOG_DEFFMT_VERBOSE);
        elogTask = osThreadNew(StartElogTask,NULL,&elogTask_attr);
        elog_start();
    }
}
/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {
    ElogErrCode result = ELOG_NO_ERR;

    /* add your code here */
    elogLock_sema = osSemaphoreNew(1,1,NULL);

    elogAsyncBuff_sema = osSemaphoreNew(1,1,NULL);
    elogAsync_queue = osMessageQueueNew(32,sizeof(size_t),&elogAsync_queue_attr);

    return result;
}

/**
 * EasyLogger port deinitialize
 *
 */
void elog_port_deinit(void) {
    /* add your code here */

}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size) {
    /* add your code here */
    OSLIB_UART_SendData(&elog_huart,log,size);
}

/**
 * async lock
 */
void elog_port_async_lock(void) {
    
    /* add your code here */
    while (osSemaphoreAcquire(elogAsyncBuff_sema,osWaitForever) != osOK)
        ;

}

/**
 * async unlock
 */
void elog_port_async_unlock(void) {
    
    /* add your code here */
    osSemaphoreRelease(elogAsyncBuff_sema);

}

/**
 * output lock
 */
void elog_port_output_lock(void) {

    /* add your code here */
    while (osSemaphoreAcquire(elogLock_sema,osWaitForever) != osOK)
        ;

}

/**
 * output unlock
 */
void elog_port_output_unlock(void) {

    /* add your code here */
    osSemaphoreRelease(elogLock_sema);

}
/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void) {
    /* add your code here */
    static char cur_system_time[11] = "";
    snprintf(cur_system_time, 11, "%lu", osKernelGetTickCount());
    return cur_system_time;
}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) {
    
    /* add your code here */
    return "";
    
}

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void) {
    
    /* add your code here */
    return "";
    
}
void elog_async_output_notice(size_t size) {
    static size_t msg_ = 0;
    msg_  = size;
    osMessageQueuePut(elogAsync_queue,&msg_,0,0);
}

/**
* @brief Function implementing the ElogTask thread.
* @param argument: Not used
* @retval None
*/
void StartElogTask(void *argument){

    size_t gotLogSize = 0;
    static char poll_get_buf[ELOG_LINE_BUF_SIZE - 4];
    size_t request_log_size = 0;
    uint8_t msg_prio ;
    osStatus_t ret;

    while (1)
    {
        while (( ret = osMessageQueueGet(elogAsync_queue,&request_log_size,&msg_prio,osWaitForever)) != osOK)
            ;

        gotLogSize = elog_async_get_log(poll_get_buf , request_log_size );

        if (gotLogSize)
        {
            elog_port_output(poll_get_buf, gotLogSize);
        }
    }
    osThreadExit();
}
#else /// 不能进行任何实现

/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {
    ElogErrCode result = ELOG_NO_ERR;

    /* add your code here */
    return result;
}

/**
 * EasyLogger port deinitialize
 *
 */
void elog_port_deinit(void) {
    /* add your code here */
}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size) {
    /* add your code here */
}

/**
 * output lock
 */
void elog_port_output_lock(void) {
    /* add your code here */
}

/**
 * output unlock
 */
void elog_port_output_unlock(void) {
    /* add your code here */
}

/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void) {
    /* add your code here */
    return "";
}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) {
    /* add your code here */
    return "";
}

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void) {
    /* add your code here */
    return "";
}
void elog_async_output_notice(size_t size) {
}
void OSLIB_Elog_Init(){
    return;
}
#endif