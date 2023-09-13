/**
 * @file vofaDataDriver.h
 * @author LiNY2
 * @brief 实现向串口输出buptDebug协议的数据，给vofa解析。
 * 协议格式：(浮点数为小端)
 * (*msg) 0xf0 0x8f (*dataArray) (uint8)gid (uint8)channel 0xff 0f80 '\r' '\n'
 * 输出的浮点数是按按字节传输的，是完整的float数，精度同单片机存储精度。
 * MAX_CHANNEL 是可以更改的，但需要和插件一起更改。
 * 更改发送串口需要到头文件中更改
 * @date 2022-12-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef VOFADATADRIVER_H_
#define VOFADATADRIVER_H_



/* Includes ---------------------------------------------------*/
#include "stdint.h"


/* Public Macro---------------------------------------------------*/
#define VOFA_UART (huart1)
#define MAX_CHANNEL (6)
/* Public Types---------------------------------------------------*/

/* Datatypes---------------------------------------------------*/


/* Packet Functions ---------------------------------------------------*/
uint16_t vofaDataPut(char txBuffer[] , const char *msg, float *dataArray, int channel, int gid);

//#endif // SLIB_USE_TOOLBOXSCOPE

#endif /* TOOLBOXSCOPE_H_ */