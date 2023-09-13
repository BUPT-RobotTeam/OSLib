/**
 * @file vofaDataDriver.c
 * @author LiNY2
 * @brief 实现向串口输出buptDebug协议的数据，给vofa解析。
 * 协议格式：(浮点数为小端)
 * (*msg) 0xf0 0x8f (*dataArray) (uint8)gid (uint8)channel 0xff 0f80 '\r' '\n'
 * 输出的浮点数是按按字节传输的，是完整的float数，精度同单片机存储精度。
 * MAX_CHANNEL 是可以更改的，但需要和插件一起更改。
 * 更改发送串口需要到头文件中更改
 * @version 0.1
 * @date 2022-12-10
 *
 * @copyright Copyright (c) 2022
 *
 */
/* Includes ---------------------------------------------------*/
#include "components/vofaDataDriver/vofaDataDriver.h"

#include <stdint.h>
#include <string.h>


/* Scope Functions---------------------------------------------------*/
/**
 * @brief 向串口输出buptDebug协议的数据给vofa解析，最多支持6通道数据同时输出，
 * 如果要输出更多数据，请分组输出。
 * @param txBuffer[in] 消息将要装入的buffer
 * @param msg 前面附加信息，不支持格式字符
 * @param dataArray 要输出的数组
 * @param channel 通道数量，最多6通道
 * @param gid 数据分组号，建议相同的msg输出相同的gid
 * @return 返回数据大小
 */
uint16_t vofaDataPut(char txBuffer[] ,const char *msg ,float *dataArray, int channel,int gid)
{
//	while (HAL_DMA_GetState(VOFA_UART.hdmatx) == HAL_DMA_STATE_BUSY)
//		HAL_Delay(1);
//    osMutexAcquire();
	//最多8通道
	if (channel < 1 || channel > MAX_CHANNEL)
	{
		return 0 ;
	}
	uint16_t ind = 0;//index
    uint16_t msgLen = strlen(msg);
	memcpy(txBuffer,msg,msgLen);
	ind = msgLen;
	// push frame head
	txBuffer[ind++] = (char)(0xF0);
	txBuffer[ind++] = (char)(0x8f);
	//push data stream
	memcpy(txBuffer + ind, dataArray , channel * sizeof(float));
	ind += channel * sizeof(float);
	// push gid , channel
	txBuffer[ind++] = (char)(gid);
	txBuffer[ind++] = (char)(channel);
	//push frame tail
	txBuffer[ind++] = (char)(0xff);
	txBuffer[ind++] = (char)(0x80);
	txBuffer[ind++] = '\r';
	txBuffer[ind++] = '\n';

    return ind;
	// 等待DMA准备完毕
//	HAL_UART_Transmit_DMA(&VOFA_UART, (uint8_t *)txBuffer, ind);
//    OSLIB_Uart_SendData(&VOFA_UART,(uint8_t *)txBuffer,ind);
}

