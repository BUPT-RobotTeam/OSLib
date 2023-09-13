/**
 * @file oslib_debug.h
 * @author KYzhang
 * @brief OSLIB-Debug功能, 使用Jlink自带的RTT方式输出
 * @version 0.2
 * @date 2021-01-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "oslib_config.h"

#ifdef OSLIB_DEBUG_MODULE_ENABLED
    #include "debug/SEGGER_RTT.h"
    #define Log(fmt, args...) SEGGER_RTT_printf(0, fmt, ##args)
#else
    #define Log(fmt, args...)    do {} while(0)
#endif // OSLIB_DEBUG_MODULE_ENABLED

#define OSLIB_DEBUG_LEVEL_OFF         0
#define OSLIB_DEBUG_LEVEL_USER        1
#define OSLIB_DEBUG_LEVEL_FATAL       1
#define OSLIB_DEBUG_LEVEL_ERROR       2
#define OSLIB_DEBUG_LEVEL_WARN        3
#define OSLIB_DEBUG_LEVEL_INFO        4
#define OSLIB_DEBUG_LEVEL_DEBUG       5
#define OSLIB_DEBUG_LEVEL_ALL         5

#if SELECT_DEBUG_LEVEL == OSLIB_DEBUG_LEVEL_OFF
    #define UDebug(fmt, args...) do {} while(0)
    #define Fatal(fmt, args...)  do {} while(0)
    #define Error(fmt, args...)  do {} while(0)
    #define Warn(fmt, args...)   do {} while(0)
    #define Info(fmt, args...)   do {} while(0)
    #define Debug(fmt, args...)  do {} while(0)
#elif SELECT_DEBUG_LEVEL == OSLIB_DEBUG_LEVEL_FATAL || SELECT_DEBUG_LEVEL == OSLIB_DEBUG_LEVEL_USER
    #define UDebug(fmt, args...) do {Log("[User] ");Log(fmt, ##args);Log("\n");} while(0)
    #define Fatal(fmt, args...)  do {Log("[Fatal]");Log(fmt, ##args);Log("\n");} while(0)
    #define Error(fmt, args...)  do {} while(0)
    #define Warn(fmt, args...)   do {} while(0)
    #define Info(fmt, args...)   do {} while(0)
    #define Debug(fmt, args...)  do {} while(0)
#elif SELECT_DEBUG_LEVEL == OSLIB_DEBUG_LEVEL_ERROR
    #define UDebug(fmt, args...) do {Log("[User] ");Log(fmt, ##args);Log("\n");} while(0)
    #define Fatal(fmt, args...)  do {Log("[Fatal]");Log(fmt, ##args);Log("\n");} while(0)
    #define Error(fmt, args...)  do {Log("[Error]");Log(fmt, ##args);Log("\n");} while(0)
    #define Warn(fmt, args...)   do {} while(0)
    #define Info(fmt, args...)   do {} while(0)
    #define Debug(fmt, args...)  do {} while(0)
#elif SELECT_DEBUG_LEVEL == OSLIB_DEBUG_LEVEL_WARN
    #define UDebug(fmt, args...) do {Log("[User] ");Log(fmt, ##args);Log("\n");} while(0)
    #define Fatal(fmt, args...)  do {Log("[Fatal]");Log(fmt, ##args);Log("\n");} while(0)
    #define Error(fmt, args...)  do {Log("[Error]");Log(fmt, ##args);Log("\n");} while(0)
    #define Warn(fmt, args...)   do {Log("[Warn] ");Log(fmt, ##args);Log("\n");} while(0)
    #define Info(fmt, args...)   do {} while(0)
    #define Debug(fmt, args...)  do {} while(0)
#elif SELECT_DEBUG_LEVEL == OSLIB_DEBUG_LEVEL_INFO
    #define UDebug(fmt, args...) do {Log("[User] ");Log(fmt, ##args);Log("\n");} while(0)
    #define Fatal(fmt, args...)  do {Log("[Fatal]");Log(fmt, ##args);Log("\n");} while(0)
    #define Error(fmt, args...)  do {Log("[Error]");Log(fmt, ##args);Log("\n");} while(0)
    #define Warn(fmt, args...)   do {Log("[Warn] ");Log(fmt, ##args);Log("\n");} while(0)
    #define Info(fmt, args...)   do {Log("[Info] ");Log(fmt, ##args);Log("\n");} while(0)
    #define Debug(fmt, args...)  do {} while(0)
#elif SELECT_DEBUG_LEVEL == OSLIB_DEBUG_LEVEL_DEBUG || SELECT_DEBUG_LEVEL == OSLIB_DEBUG_LEVEL_ALL
    #define UDebug(fmt, args...) do {Log("[User] ");Log(fmt, ##args);Log("\n");} while(0)
    #define Fatal(fmt, args...)  do {Log("[Fatal]");Log(fmt, ##args);Log("\n");} while(0)
    #define Error(fmt, args...)  do {Log("[Error]");Log(fmt, ##args);Log("\n");} while(0)
    #define Warn(fmt, args...)   do {Log("[Warn] ");Log(fmt, ##args);Log("\n");} while(0)
    #define Info(fmt, args...)   do {Log("[Info] ");Log(fmt, ##args);Log("\n");} while(0)
    #define Debug(fmt, args...)  do {Log("[Debug]");Log(fmt, ##args);Log("\n");} while(0)
#else
    #error "[OSLIB] Debug level is wrong"
#endif
